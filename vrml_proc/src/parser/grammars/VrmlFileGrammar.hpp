#pragma once

#include <memory>
#include <string>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/phoenix/phoenix.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/optional/optional_io.hpp>

#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "VrmlField.hpp"
#include "VrmlFile.hpp"
#include "VrmlNode.hpp"
#include "UseNode.hpp"
#include "VrmlNodeManager.hpp"

#include "IdentifierGrammar.hpp"
#include "Vec3fGrammar.hpp"
#include "Vec3fArrayGrammar.hpp"
#include "Vec4fGrammar.hpp"
#include "Int32ArrayGrammar.hpp"
#include "QuotedStringGrammar.hpp"
#include "BooleanGrammar.hpp"
#include "BaseGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::VrmlNode,
    (boost::optional<std::string>, definitionName)
    (std::string, header)
    (std::vector<vrml_proc::parser::VrmlField>, fields)
)

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::UseNode,
    (std::string, identifier)
)

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::VrmlField,
    (std::string, name)
    (vrml_proc::parser::VrmlFieldValue, value)
)

namespace vrml_proc {
    namespace parser {

        // For more information, see: https://www.boost.org/doc/libs/1_87_0/libs/spirit/doc/html/spirit/qi/reference/numeric/real.html.
        struct Float32Policy : boost::spirit::qi::real_policies<float32_t>
        {
            static bool const expect_dot = true;
        };

        template <typename Iterator, typename Skipper>
        class VrmlNodeGrammar : public boost::spirit::qi::grammar<Iterator, VrmlNode(), Skipper>, public BaseGrammar <Iterator, VrmlNode(), Skipper> {

        public:
            VrmlNodeGrammar(VrmlNodeManager& manager) : VrmlNodeGrammar::base_type(this->m_start), m_manager(manager) {

                m_identifier = std::make_unique<IdentifierGrammar<Iterator, Skipper>>();

                m_quotedString = std::make_unique<QuotedStringGrammar<Iterator, Skipper>>();

                m_vec3f = std::make_unique<Vec3fGrammar<Iterator, Skipper>>();

                m_vec4f = std::make_unique<Vec4fGrammar<Iterator, Skipper>>();

                m_vec3fArray = std::make_unique<Vec3fArrayGrammar<Iterator, Skipper>>();

                m_int32Array = std::make_unique<Int32ArrayGrammar<Iterator, Skipper>>();

                m_boolean = std::make_unique<BooleanGrammar<Iterator, Skipper>>();

                m_vrmlFieldValue = (m_quotedString->GetStartRule() | m_boolean->GetStartRule() | m_vec3fArray->GetStartRule() | m_int32Array->GetStartRule() | m_vec4f->GetStartRule() | m_vec3f->GetStartRule() | boost::spirit::qi::real_parser<float32_t, Float32Policy>() | boost::spirit::qi::int_ | m_useNode | m_vrmlNode | m_vrmlNodeArray);

                m_vrmlField = (m_identifier->GetStartRule() >> m_vrmlFieldValue);

                using boost::spirit::qi::on_success;
                on_success(m_vrmlField,
                    boost::phoenix::bind(&VrmlNodeGrammar::onMainFieldParsed, this, boost::spirit::qi::_val)
                );

                m_vrmlNodeArray =
                    ("[" >>
                        ((m_vrmlNode | m_useNode) % ",") >>
                        "]");

                using boost::spirit::qi::on_success;
                on_success(m_vrmlNodeArray,
                    boost::phoenix::bind(&VrmlNodeGrammar::onArrayParsed, this, boost::spirit::qi::_val)
                );

                using boost::spirit::qi::on_success;
                on_success(m_vrmlFieldValue,
                    boost::phoenix::bind(&VrmlNodeGrammar::onFieldParsed, this, boost::spirit::qi::_val)
                );

                m_useNode = boost::spirit::qi::lit("USE") >> m_identifier->GetStartRule();

                m_vrmlNode = (-(boost::spirit::qi::lit("DEF") >> m_identifier->GetStartRule()) >> m_identifier->GetStartRule() >> boost::spirit::qi::lit("{") >> *(m_vrmlField) >> boost::spirit::qi::lit("}"));

                using boost::spirit::qi::on_success;
                on_success(m_vrmlNode,
                    boost::phoenix::bind(&VrmlNodeGrammar::onVrmlNodeParsed, this, boost::spirit::qi::_val)
                );


                this->m_start = m_vrmlNode;

                BOOST_SPIRIT_DEBUG_NODE(this->m_start);
                BOOST_SPIRIT_DEBUG_NODE(m_vrmlFieldValue);
                BOOST_SPIRIT_DEBUG_NODE(m_vrmlField);
                BOOST_SPIRIT_DEBUG_NODE(m_vrmlNode);
            }

        private:

            void onVrmlNodeParsed(const vrml_proc::parser::VrmlNode& node) {
                std::cout << "node (address: " << &(node) << ") :'<" << node << ">'" << std::endl;
                StoreNode(node);
            }

            void onMainFieldParsed(const vrml_proc::parser::VrmlField& field) {
                std::cout << "Parsing vrml field" << std::endl;
                onFieldParsed(field.value);
            }

            struct NodeVisitor : public boost::static_visitor<void> {
                // Constructor to accept the VrmlNodeManager reference
                NodeVisitor( VrmlNodeManager& manager) : manager(manager) {}

                void operator()(const boost::recursive_wrapper<VrmlNode>& vrmlNodeWrapper) const {
                    const VrmlNode& vrmlNode = vrmlNodeWrapper.get(); // Dereference the wrapper to get the actual VrmlNode
                    std::cout << "Address of VrmlNode: " << &vrmlNode << std::endl;
                    std::cout << vrmlNode << std::endl;
                }

                void operator()(const VrmlNode& node) const {
                    std::cout << "Address of VrmlNode: " << &node << std::endl;
                    std::cout << node << std::endl;
                    manager.AddDefinitionNode(node.definitionName.value(), node); // Use manager to add the node
                }

                void operator()(const boost::recursive_wrapper<UseNode>& useNodeWrapper) const {
                    const UseNode& useNode = useNodeWrapper.get(); // Dereference the wrapper to get the actual UseNode
                    std::cout << "Address of UseNode: " << &useNode << std::endl;
                    std::cout << useNode << std::endl;
                }

            private:
                 VrmlNodeManager& manager; // Store the reference to VrmlNodeManager
            };



            void onArrayParsed(const std::vector<boost::variant<boost::recursive_wrapper<VrmlNode>, boost::recursive_wrapper<UseNode>>>& nodes) {
                std::cout << "Parsed VrmlNodeArray, addresses of nodes:" << std::endl;
                NodeVisitor visitor(m_manager);
                for (const auto& node_variant : nodes) {
                    boost::apply_visitor([this](auto& node) {
                        // Print the address of the object wrapped in the variant
                        std::cout << "Address of node: " << &node << std::endl;
                        std::cout << node << std::endl;
                        }, node_variant);
                    boost::apply_visitor(visitor, node_variant);
                }
            }

            void onFieldParsed(const boost::variant<std::string, bool, Vec3fArray, Int32Array, float32_t, int32_t, Vec4f, Vec3f, UseNode, boost::recursive_wrapper<struct VrmlNode>, std::vector<boost::variant<boost::recursive_wrapper<struct VrmlNode>, boost::recursive_wrapper<struct UseNode>>>>& value) {
                std::cout << "Parsed VrmlField, addresses of data:" << std::endl;
                boost::apply_visitor([this](auto& value) {
                    // Print the address of the object wrapped in the variant
                    std::cout << "Address of field data: " << &value << std::endl;
                    }, value);
            }

            void StoreNode(const VrmlNode& node) {
                if (node.definitionName.has_value() && node.definitionName.value() != "") {
                    m_manager.AddDefinitionNode(node.definitionName.value(), node);
                }
            }

            boost::spirit::qi::rule<Iterator, VrmlNode(), Skipper> m_vrmlNode;
            boost::spirit::qi::rule<Iterator, UseNode(), Skipper> m_useNode;
            boost::spirit::qi::rule<Iterator, VrmlField(), Skipper> m_vrmlField;
            boost::spirit::qi::rule<Iterator, VrmlFieldValue(), Skipper> m_vrmlFieldValue;
            boost::spirit::qi::rule<Iterator, std::vector<boost::variant<boost::recursive_wrapper<VrmlNode>, boost::recursive_wrapper<UseNode>>>(), Skipper> m_vrmlNodeArray;

            std::unique_ptr<IdentifierGrammar<Iterator, Skipper>> m_identifier;
            std::unique_ptr<Vec3fGrammar<Iterator, Skipper>> m_vec3f;
            std::unique_ptr<Vec4fGrammar<Iterator, Skipper>> m_vec4f;
            std::unique_ptr<Vec3fArrayGrammar<Iterator, Skipper>> m_vec3fArray;
            std::unique_ptr<Int32ArrayGrammar<Iterator, Skipper>> m_int32Array;
            std::unique_ptr<QuotedStringGrammar<Iterator, Skipper>> m_quotedString;
            std::unique_ptr<BooleanGrammar<Iterator, Skipper>> m_boolean;

            VrmlNodeManager& m_manager;
        };

        template <typename Iterator, typename Skipper>
        class VrmlFileGrammar : public boost::spirit::qi::grammar<Iterator, VrmlFile(), Skipper>, public BaseGrammar<Iterator, VrmlFile(), Skipper> {

        public:
            VrmlFileGrammar(VrmlNodeManager& manager) : VrmlFileGrammar::base_type(this->m_start), m_manager(manager) {

                m_vrmlNode = std::make_unique<VrmlNodeGrammar<Iterator, Skipper>>(m_manager);

                this->m_start = *(m_vrmlNode->GetStartRule());

                BOOST_SPIRIT_DEBUG_NODE(this->m_start);
            }

        private:
            std::unique_ptr<VrmlNodeGrammar<Iterator, Skipper>> m_vrmlNode;
            VrmlNodeManager& m_manager;
        };
    }
}

