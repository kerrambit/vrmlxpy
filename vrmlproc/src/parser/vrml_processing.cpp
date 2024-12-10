#include <iostream>
#include <string>
#include <array>
#include <variant>
#include <memory>
#include <optional>
#include <utility>
#include <fstream>
#include <sstream>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/phoenix/phoenix.hpp>

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "VRMLUnits.hpp"
#include "vrml_processing.hpp"
#include "VRMLField.hpp"
#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Int32Array.hpp"
#include "VRMLNode.hpp"
#include "USENode.hpp"
#include "VRMLNodeManager.hpp"

#include "CommentSkipper.hpp"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

// -----------------------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Vec3f,
    (vrml_proc::parser::float32_t, x)
    (vrml_proc::parser::float32_t, y)
    (vrml_proc::parser::float32_t, z)
)

template<typename Iterator, typename Skipper>
struct Vec3fGrammar : qi::grammar<Iterator, vrml_proc::parser::Vec3f(), Skipper> {

    Vec3fGrammar() : Vec3fGrammar::base_type(start) {
        start = qi::float_ >> qi::float_ >> qi::float_;

        BOOST_SPIRIT_DEBUG_NODE(start);
    }

    qi::rule<Iterator, vrml_proc::parser::Vec3f(), Skipper> start;
};

bool vrml_proc::parseVec3f(std::string& text) {

    auto it = text.begin();
    Vec3fGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar;
    vrml_proc::parser::Vec3f data;
    vrml_proc::parser::CommentSkipper skipper;
    bool success = qi::phrase_parse(it, text.end(), grammar, skipper, data);

    if (success && it == text.end()) {
        std::cout << data << std::endl;
        return true;
    }
    else {
        std::cerr << "Parsing failed at: " << std::string(it, text.end()) << "\n";
        return false;
    }
}

// -----------------------------------------------------------------------------------------


BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Vec3fArray,
    (std::vector<vrml_proc::parser::Vec3f>, vectors)
)

template <typename Iterator, typename Skipper>
struct Vec3fArrayGrammar : qi::grammar<Iterator, vrml_proc::parser::Vec3fArray(), Skipper> {

    Vec3fArrayGrammar() : Vec3fArrayGrammar::base_type(start) {

        vec3f = std::make_unique<Vec3fGrammar<Iterator, Skipper>>();

        start = qi::lit('[') >>
                -((vec3f->start % qi::lit(',')) >> -qi::lit(',')) >>
                qi::lit(']');

        BOOST_SPIRIT_DEBUG_NODE(start);
    }

    std::unique_ptr<Vec3fGrammar<Iterator, Skipper>> vec3f;
    qi::rule<Iterator, vrml_proc::parser::Vec3fArray(), Skipper> start;
};

bool vrml_proc::parseVec3fArray(std::string& text) {

    auto it = text.begin();
    Vec3fArrayGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar;
    vrml_proc::parser::Vec3fArray data;
    vrml_proc::parser::CommentSkipper skipper;
    bool success = qi::phrase_parse(it, text.end(), grammar, skipper, data);

    if (success && it == text.end()) {
        std::cout << data << std::endl;
        return true;
    }
    else {
        std::cerr << "Parsing failed at: " << std::string(it, text.end()) << "\n";
        return false;
    }
}

// -----------------------------------------------------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Int32Array,
    (std::vector<int32_t>, integers)
)

template <typename Iterator, typename Skipper>
struct Int32ArrayGrammar : qi::grammar<Iterator, vrml_proc::parser::Int32Array(), Skipper> {

    Int32ArrayGrammar() : Int32ArrayGrammar::base_type(start) {

        start = "["
            >> (qi::int_ % ",")
            >> "]";

        BOOST_SPIRIT_DEBUG_NODE(start);
    }

    qi::rule<Iterator, vrml_proc::parser::Int32Array(), Skipper> start;
};

bool vrml_proc::parseInt32Array(std::string& text) {

    auto it = text.begin();
    Int32ArrayGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar;
    vrml_proc::parser::Int32Array data;
    vrml_proc::parser::CommentSkipper skipper;
    bool success = qi::phrase_parse(it, text.end(), grammar, skipper, data);

    if (success && it == text.end()) {
        std::cout << data << std::endl;
        return true;
    }
    else {
        std::cerr << "Parsing failed at: " << std::string(it, text.end()) << "\n";
        return false;
    }
}

 // -----------------------------------------------------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::VRMLNode,
    (boost::optional<std::string>, definition_name)
    (std::string, header)
    (std::vector<vrml_proc::parser::VRMLField>, fields)
)

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::USENode,
    (std::string, identifier)
)

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::VRMLField,
    (std::string, name)
    (vrml_proc::parser::VRMLFieldValue, value)
)

template <typename Iterator, typename Skipper>
struct VRMLGrammar : qi::grammar<Iterator, std::vector<vrml_proc::parser::VRMLNode>(), Skipper> {

    VRMLGrammar(vrml_proc::parser::VRMLNodeManager& manager) : VRMLGrammar::base_type(start), manager(manager) {

        identifier = qi::lexeme[qi::alpha >> *qi::alnum]; // TODO: limit it to the VRML 2.0 specs

        quoted_string = qi::lexeme['"' >> +(qi::char_ - '"') >> '"'];

        vec3f = std::make_unique<Vec3fGrammar<Iterator, Skipper>>();

        vec3f_array = std::make_unique<Vec3fArrayGrammar<Iterator, Skipper>>();

        int32_array = std::make_unique<Int32ArrayGrammar<Iterator, Skipper>>();

        field_value = (quoted_string | vec3f_array->start | int32_array->start | vec3f->start | qi::float_ | qi::int_ | use_node | vrml_node | vrml_node_array);

        field = (identifier >> field_value)
            [
                qi::_val = boost::phoenix::construct<vrml_proc::parser::VRMLField>(qi::_1, qi::_2)
            ];

        vrml_node = (-(qi::lit("DEF") >> identifier) >> identifier >> qi::lit("{") >> *(field) >> qi::lit("}"))
            [
                boost::phoenix::bind(&vrml_proc::parser::VRMLNode::definition_name, boost::phoenix::ref(qi::_val)) = (qi::_1),
                boost::phoenix::bind(&vrml_proc::parser::VRMLNode::header, boost::phoenix::ref(qi::_val)) = (qi::_2),
                boost::phoenix::bind(&vrml_proc::parser::VRMLNode::fields, boost::phoenix::ref(qi::_val)) = (qi::_3),
                boost::phoenix::bind(
                    [](vrml_proc::parser::VRMLNode& node, vrml_proc::parser::VRMLNodeManager& manager) {
                        if (node.definition_name) {
                            auto node_ptr = std::make_shared<vrml_proc::parser::VRMLNode>(node);
                            manager.AddDefinitionNode(*node.definition_name, node_ptr);
                        }
                    },
                    qi::_val, boost::phoenix::ref(manager)
                )
            ];

        vrml_node_array = "["
            >> ((vrml_node | use_node) % ",")
            >> "]";

        use_node = qi::lit("USE") >> identifier;

        start = *(vrml_node);

         BOOST_SPIRIT_DEBUG_NODE(start);
         BOOST_SPIRIT_DEBUG_NODE(identifier);
         BOOST_SPIRIT_DEBUG_NODE(quoted_string);
         BOOST_SPIRIT_DEBUG_NODE(field_value);
         BOOST_SPIRIT_DEBUG_NODE(field);
         BOOST_SPIRIT_DEBUG_NODE(vrml_node);
    }

    qi::rule<Iterator, std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VRMLNode>, boost::recursive_wrapper<vrml_proc::parser::USENode>>>(), Skipper> vrml_node_array;
    qi::rule<Iterator, vrml_proc::parser::VRMLNode(), Skipper> vrml_node;
    qi::rule<Iterator, vrml_proc::parser::USENode(), Skipper> use_node;
    qi::rule<Iterator, std::vector<vrml_proc::parser::VRMLNode>(), Skipper> start;
    qi::rule<Iterator, std::string(), Skipper> identifier;
    std::unique_ptr<Vec3fGrammar<Iterator, Skipper>> vec3f;
    std::unique_ptr<Vec3fArrayGrammar<Iterator, Skipper>> vec3f_array;
    std::unique_ptr<Int32ArrayGrammar<Iterator, Skipper>> int32_array;
    qi::rule<Iterator, vrml_proc::parser::VRMLField(), Skipper> field;
    qi::rule<Iterator, vrml_proc::parser::VRMLFieldValue(), Skipper> field_value;
    qi::rule<Iterator, std::string(), Skipper> quoted_string;

    vrml_proc::parser::VRMLNodeManager& manager;
};

bool vrml_proc::parseVRMLFile(std::string& text) {

    vrml_proc::parser::VRMLNodeManager manager;

    auto it = text.begin();
    VRMLGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar(manager);
    std::vector<vrml_proc::parser::VRMLNode> data;
    vrml_proc::parser::CommentSkipper skipper;

    bool success = qi::phrase_parse(it, text.end(), grammar, skipper, data);

    if (success && it == text.end()) {
        std::cout << "Parsing passed:\n";
        
        uint16_t node_index = 1;
        for (const auto& node : data) {
            std::cout << node_index << ". Node\n";
            std::cout << "------------------------------------------\n";
            std::cout << node << "\n";
            std::cout << "------------------------------------------";
            std::cout << std::endl;
            node_index++;
        }

        std::cout << "\nVRMLNodeManager after parsing: \n";
        manager.Display(std::cout);
        std::cout << "\n" << std::endl;

        return true;
    }
    else {
        std::cerr << "Parsing failed at: " << std::string(it, text.end()) << "\n";
        return false;
    }
}

namespace vrml_proc {
    LoadFileResult LoadFile(const std::string& filepath) {

        std::ifstream file(filepath, std::ios::in);

        if (!file) {
            return { };
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string file_contents = buffer.str();

        return { std::move(file_contents) };
    }
}