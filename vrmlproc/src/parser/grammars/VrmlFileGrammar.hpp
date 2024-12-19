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
#include "BaseGrammar.hpp"

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

        // For more information, see: https://www.boost.org/doc/libs/1_87_0/libs/spirit/doc/html/spirit/qi/reference/numeric/real.html.
        struct Float32Policy : boost::spirit::qi::real_policies<float32_t>
        {
            static bool const expect_dot = true;
        };

        template <typename Iterator, typename Skipper>
        class VrmlFileGrammar
            : public boost::spirit::qi::grammar<Iterator, std::vector<VRMLNode>(), Skipper>,
            public BaseGrammar <Iterator, std::vector<VRMLNode>(), Skipper> {

        public:
            VrmlFileGrammar(VRMLNodeManager& manager) : VrmlFileGrammar::base_type(this->m_start), m_manager(manager) {

                m_identifier = std::make_unique<IdentifierGrammar<Iterator, Skipper>>();

                m_quotedString = std::make_unique<QuotedStringGrammar<Iterator, Skipper>>();

                m_vec3f = std::make_unique<Vec3fGrammar<Iterator, Skipper>>();

                m_vec4f = std::make_unique<Vec4fGrammar<Iterator, Skipper>>();

                m_vec3fArray = std::make_unique<Vec3fArrayGrammar<Iterator, Skipper>>();

                m_int32Array = std::make_unique<Int32ArrayGrammar<Iterator, Skipper>>();

                m_boolean = std::make_unique<BooleanGrammar<Iterator, Skipper>>();

                m_vrmlFieldValue = (m_quotedString->GetStartRule() | m_boolean->GetStartRule() | m_vec3fArray->GetStartRule() | m_int32Array->GetStartRule() | m_vec4f->GetStartRule() | m_vec3f->GetStartRule() | boost::spirit::qi::real_parser<float32_t, Float32Policy>() | boost::spirit::qi::int_ | m_useNode | m_vrmlNode | m_vrmlNodeArray);

                m_vrmlField = (m_identifier->GetStartRule() >> m_vrmlFieldValue)
                    [
                        boost::spirit::qi::_val = boost::phoenix::construct<vrml_proc::parser::VRMLField>(boost::spirit::qi::_1, boost::spirit::qi::_2)
                    ];

                m_vrmlNode = (-(boost::spirit::qi::lit("DEF") >> m_identifier->GetStartRule()) >> m_identifier->GetStartRule() >> boost::spirit::qi::lit("{") >> *(m_vrmlField) >> boost::spirit::qi::lit("}"))
                    [
                        boost::phoenix::bind(&VRMLNode::definition_name, boost::phoenix::ref(boost::spirit::qi::_val)) = (boost::spirit::qi::_1),
                            boost::phoenix::bind(&VRMLNode::header, boost::phoenix::ref(boost::spirit::qi::_val)) = (boost::spirit::qi::_2),
                            boost::phoenix::bind(&VRMLNode::fields, boost::phoenix::ref(boost::spirit::qi::_val)) = (boost::spirit::qi::_3),
                            boost::phoenix::bind(
                                [](VRMLNode& node, VRMLNodeManager& manager) {
                                    if (node.definition_name.has_value()) {
                                        auto node_ptr = std::make_shared<VRMLNode>(node);
                                        manager.AddDefinitionNode(node.definition_name.value(), node_ptr);
                                    }
                                },
                                boost::spirit::qi::_val, boost::phoenix::ref(manager)
                            )
                    ];

                m_vrmlNodeArray = "["
                    >> ((m_vrmlNode | m_useNode) % ",")
                    >> "]";

                m_useNode = boost::spirit::qi::lit("USE") >> m_identifier->GetStartRule();

                this->m_start = *(m_vrmlNode);

                BOOST_SPIRIT_DEBUG_NODE(this->m_start);
                BOOST_SPIRIT_DEBUG_NODE(m_vrmlFieldValue);
                BOOST_SPIRIT_DEBUG_NODE(m_vrmlField);
                BOOST_SPIRIT_DEBUG_NODE(m_vrmlNode);
            }

        private:
            boost::spirit::qi::rule<Iterator, VRMLNode(), Skipper> m_vrmlNode;
            boost::spirit::qi::rule<Iterator, USENode(), Skipper> m_useNode;
            boost::spirit::qi::rule<Iterator, VRMLField(), Skipper> m_vrmlField;
            boost::spirit::qi::rule<Iterator, VRMLFieldValue(), Skipper> m_vrmlFieldValue;
            boost::spirit::qi::rule<Iterator, std::vector<boost::variant<boost::recursive_wrapper<VRMLNode>, boost::recursive_wrapper<USENode>>>(), Skipper> m_vrmlNodeArray;

            std::unique_ptr<IdentifierGrammar<Iterator, Skipper>> m_identifier;
            std::unique_ptr<Vec3fGrammar<Iterator, Skipper>> m_vec3f;
            std::unique_ptr<Vec4fGrammar<Iterator, Skipper>> m_vec4f;
            std::unique_ptr<Vec3fArrayGrammar<Iterator, Skipper>> m_vec3fArray;
            std::unique_ptr<Int32ArrayGrammar<Iterator, Skipper>> m_int32Array;
            std::unique_ptr<QuotedStringGrammar<Iterator, Skipper>> m_quotedString;
            std::unique_ptr<BooleanGrammar<Iterator, Skipper>> m_boolean;

            VRMLNodeManager& m_manager;
        };
    }
}

