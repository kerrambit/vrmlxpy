#pragma once

#include <memory>
#include <string>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/phoenix/phoenix.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "VRMLField.hpp"
#include "VRMLNode.hpp"
#include "USENode.hpp"
#include "VRMLNodeManager.hpp"

#include "IdentifierGrammar.hpp"
#include "Vec3fGrammar.hpp"
#include "Vec3fArrayGrammar.hpp"
#include "Vec4fGrammar.hpp"
#include "Int32ArrayGrammar.hpp"
#include "QuotedStringGrammar.hpp"
#include "BooleanGrammar.hpp"

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

namespace vrml_proc {
    namespace parser {

        template <typename Iterator, typename Skipper>
        struct VrmlFileGrammar : boost::spirit::qi::grammar<Iterator, std::vector<VRMLNode>(), Skipper> {

            VrmlFileGrammar(VRMLNodeManager& manager) : VrmlFileGrammar::base_type(start), manager(manager) {

                identifier = std::make_unique<IdentifierGrammar<Iterator, Skipper>>();

                quoted_string = std::make_unique<QuotedStringGrammar<Iterator, Skipper>>();

                vec3f = std::make_unique<Vec3fGrammar<Iterator, Skipper>>();

                vec4f = std::make_unique<Vec4fGrammar<Iterator, Skipper>>();

                vec3f_array = std::make_unique<Vec3fArrayGrammar<Iterator, Skipper>>();

                int32_array = std::make_unique<Int32ArrayGrammar<Iterator, Skipper>>();

                boolean = std::make_unique<BooleanGrammar<Iterator, Skipper>>();

                field_value = (quoted_string->start | boolean->start | vec3f_array->start | int32_array->start | vec4f->start | vec3f->start | boost::spirit::qi::float_ | boost::spirit::qi::int_ | use_node | vrml_node | vrml_node_array);

                field = (identifier->start >> field_value)
                    [
                        boost::spirit::qi::_val = boost::phoenix::construct<vrml_proc::parser::VRMLField>(boost::spirit::qi::_1, boost::spirit::qi::_2)
                    ];

                vrml_node = (-(boost::spirit::qi::lit("DEF") >> identifier->start) >> identifier->start >> boost::spirit::qi::lit("{") >> *(field) >> boost::spirit::qi::lit("}"))
                    [
                        boost::phoenix::bind(&VRMLNode::definition_name, boost::phoenix::ref(boost::spirit::qi::_val)) = (boost::spirit::qi::_1),
                            boost::phoenix::bind(&VRMLNode::header, boost::phoenix::ref(boost::spirit::qi::_val)) = (boost::spirit::qi::_2),
                            boost::phoenix::bind(&VRMLNode::fields, boost::phoenix::ref(boost::spirit::qi::_val)) = (boost::spirit::qi::_3),
                            boost::phoenix::bind(
                                [](VRMLNode& node, VRMLNodeManager& manager) {
                                    if (node.definition_name) {
                                        auto node_ptr = std::make_shared<VRMLNode>(node);
                                        manager.AddDefinitionNode(*node.definition_name, node_ptr);
                                    }
                                },
                                boost::spirit::qi::_val, boost::phoenix::ref(manager)
                            )
                    ];

                vrml_node_array = "["
                    >> ((vrml_node | use_node) % ",")
                    >> "]";

                use_node = boost::spirit::qi::lit("USE") >> identifier->start;

                start = *(vrml_node);

                BOOST_SPIRIT_DEBUG_NODE(start);
                BOOST_SPIRIT_DEBUG_NODE(field_value);
                BOOST_SPIRIT_DEBUG_NODE(field);
                BOOST_SPIRIT_DEBUG_NODE(vrml_node);
            }

            boost::spirit::qi::rule<Iterator, std::vector<VRMLNode>(), Skipper> start;
            boost::spirit::qi::rule<Iterator, VRMLNode(), Skipper> vrml_node;
            boost::spirit::qi::rule<Iterator, USENode(), Skipper> use_node;
            boost::spirit::qi::rule<Iterator, VRMLField(), Skipper> field;
            boost::spirit::qi::rule<Iterator, VRMLFieldValue(), Skipper> field_value;
            boost::spirit::qi::rule<Iterator, std::vector<boost::variant<boost::recursive_wrapper<VRMLNode>, boost::recursive_wrapper<USENode>>>(), Skipper> vrml_node_array;

            std::unique_ptr<IdentifierGrammar<Iterator, Skipper>> identifier;
            std::unique_ptr<Vec3fGrammar<Iterator, Skipper>> vec3f;
            std::unique_ptr<Vec4fGrammar<Iterator, Skipper>> vec4f;
            std::unique_ptr<Vec3fArrayGrammar<Iterator, Skipper>> vec3f_array;
            std::unique_ptr<Int32ArrayGrammar<Iterator, Skipper>> int32_array;
            std::unique_ptr<QuotedStringGrammar<Iterator, Skipper>> quoted_string;
            std::unique_ptr<BooleanGrammar<Iterator, Skipper>> boolean;

            VRMLNodeManager& manager;
        };
    }
}

