#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <optional>
#include <string>

#include <boost/spirit/home/qi/parse.hpp>
#include <boost/variant/detail/apply_visitor_unary.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>

#include "CommentSkipper.hpp"
#include "Logger.hpp"
#include "VrmlFileGrammar.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include <UseNode.hpp>
#include <vector>

static std::optional<vrml_proc::parser::VrmlNode> ParseVrmlNode(std::string& text, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::parser::VrmlNode data;
    auto iterator = text.begin();

    vrml_proc::parser::VrmlNodeGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar(manager);
    vrml_proc::parser::CommentSkipper skipper;

    if (boost::spirit::qi::phrase_parse(iterator, text.end(), grammar, skipper, data)) {
        return data;
    }

    return {};
}

static std::optional<std::vector<std::shared_ptr<vrml_proc::parser::VrmlNode>>> ParseVrmlFile(std::string& text, vrml_proc::parser::VrmlNodeManager& manager) {
    std::vector<std::shared_ptr<vrml_proc::parser::VrmlNode>> data;
    using iterator_type = std::string::iterator;
    iterator_type begin = text.begin();
    iterator_type end = text.end();

    vrml_proc::parser::VrmlNodeGrammar<std::string::iterator, vrml_proc::parser::CommentSkipper> grammar(manager);
    vrml_proc::parser::CommentSkipper skipper;

    while (begin != end) {
        std::shared_ptr<vrml_proc::parser::VrmlNode> node = std::make_shared<vrml_proc::parser::VrmlNode>();
        bool success = boost::spirit::qi::phrase_parse(begin, end, grammar, skipper, *node);

        if (success) {
            std::cout << "Parsed a VrmlNode!" << std::endl;
            std::cout << "Stored node address: " << node.get() << std::endl;
            data.push_back(node);
            std::cout << "Stored node address: " << node.get() << std::endl;
        }
        else {
            return {};
        }
    }

    return data;
}


//TEST_CASE("Parse VrmlNode - Valid Input", "[parsing][valid]") {
//
//    vrml_proc::core::logger::InitLogging();
//
//    std::string input = R"(
//
//	#VRML V2.0 utf8
//
//        Group {
//		    children [
//					    DEF id Group {
//                               bboxCenter 58.7 0.0 15
//                          }
//				     ]
//	    }
//    )";
//
//    vrml_proc::parser::VrmlNodeManager manager;
//    auto parseResult = ParseVrmlNode(input, manager);
//    REQUIRE(parseResult);
//
//    auto& root = parseResult.value();
//    auto& field = root.fields.at(0);
//
//    CHECK(field.name == "children");
//
//    auto* value = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(&field.value);
//    REQUIRE(value != nullptr);
//
//    uintptr_t variantPtr;
//    {
//        auto& variant = value->at(0);
//
//        struct VrmlNodeVisitor : public boost::static_visitor<void> {
//            void operator()(vrml_proc::parser::VrmlNode& vrmlNode) {
//
//                REQUIRE(vrmlNode.header == "Group");
//                ptr = reinterpret_cast<uintptr_t>(&vrmlNode);
//
//                REQUIRE(vrmlNode.definitionName.has_value());
//                CHECK(vrmlNode.definitionName.value() == "id");
//
//                auto& field = vrmlNode.fields.at(0);
//                CHECK(field.name == "bboxCenter");
//
//                REQUIRE(boost::get<vrml_proc::parser::Vec3f>(&field.value) != nullptr);
//                auto* bboxCenter = boost::get<vrml_proc::parser::Vec3f>(&field.value);
//
//                CHECK_THAT(bboxCenter->x, Catch::Matchers::WithinAbs(58.7, 0.01));
//                CHECK_THAT(bboxCenter->y, Catch::Matchers::WithinAbs(0.0, 0.0));
//                CHECK_THAT(bboxCenter->z, Catch::Matchers::WithinAbs(15.0, 0.0));
//            } 
//
//            void operator()(vrml_proc::parser::UseNode& useNode) {
//                FAIL("Unexpected type inside the variant.");
//            }
//
//            uintptr_t ptr;
//        };
//
//        VrmlNodeVisitor visitor;
//        boost::apply_visitor(visitor, variant);
//        variantPtr = visitor.ptr;
//    }
//
//    auto def = manager.GetDefinitionNode("id");
//    CHECK(def.has_value());
//    CHECK(reinterpret_cast<uintptr_t>(&def.value().get()) == variantPtr);
//}

TEST_CASE("Parse VrmlFile - Valid Input", "[parsing][valid]") {

    vrml_proc::core::logger::InitLogging();

    std::string input = R"(

	#VRML V2.0 utf8

        Group {
		    children [
					    DEF id Group {
                               bboxCenter 58.7 0.0 15
                          }
				     ]
	    }

        Group {
		    children [
					    DEF id Group {
                               bboxCenter 58.7 0.0 15
                          }
				     ]
	    }
    )";

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(input, manager);
    REQUIRE(parseResult);

    auto& root = *(parseResult.value().at(0));
    std::cout << "root: " << &root << std::endl;
    auto& field = root.fields.at(0);
    CHECK(field.name == "children");

    auto* value = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(&field.value);
    REQUIRE(value != nullptr);

    uintptr_t variantPtr;
    {
        auto& variant = value->at(0);

        struct VrmlNodeVisitor : public boost::static_visitor<void> {
            void operator()(vrml_proc::parser::VrmlNode& vrmlNode) {

                REQUIRE(vrmlNode.header == "Group");
                ptr = reinterpret_cast<uintptr_t>(&vrmlNode);

                REQUIRE(vrmlNode.definitionName.has_value());
                CHECK(vrmlNode.definitionName.value() == "id");

                auto& field = vrmlNode.fields.at(0);
                CHECK(field.name == "bboxCenter");

                REQUIRE(boost::get<vrml_proc::parser::Vec3f>(&field.value) != nullptr);
                auto* bboxCenter = boost::get<vrml_proc::parser::Vec3f>(&field.value);

                CHECK_THAT(bboxCenter->x, Catch::Matchers::WithinAbs(58.7, 0.01));
                CHECK_THAT(bboxCenter->y, Catch::Matchers::WithinAbs(0.0, 0.0));
                CHECK_THAT(bboxCenter->z, Catch::Matchers::WithinAbs(15.0, 0.0));
            }

            void operator()(vrml_proc::parser::UseNode& useNode) {
                FAIL("Unexpected type inside the variant.");
            }

            uintptr_t ptr;
        };

        VrmlNodeVisitor visitor;
        boost::apply_visitor(visitor, variant);
        variantPtr = visitor.ptr;
    }

    REQUIRE(manager.GetDefNodesTotalCount() == 1);
    auto def = manager.GetDefinitionNode("id");
    CHECK(def.has_value());
    CHECK_FALSE(reinterpret_cast<uintptr_t>(&def.value().get()) == variantPtr);

    {
        auto& root = *(parseResult.value().at(1));
        std::cout << "root: " << &root << std::endl;
        auto& field = root.fields.at(0);
        CHECK(field.name == "children");

        auto* value = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(&field.value);
        REQUIRE(value != nullptr);

        uintptr_t variantPtr;
        {
            auto& variant = value->at(0);

            struct VrmlNodeVisitor : public boost::static_visitor<void> {
                void operator()(vrml_proc::parser::VrmlNode& vrmlNode) {

                    REQUIRE(vrmlNode.header == "Group");
                    ptr = reinterpret_cast<uintptr_t>(&vrmlNode);

                    REQUIRE(vrmlNode.definitionName.has_value());
                    CHECK(vrmlNode.definitionName.value() == "id");

                    auto& field = vrmlNode.fields.at(0);
                    CHECK(field.name == "bboxCenter");

                    REQUIRE(boost::get<vrml_proc::parser::Vec3f>(&field.value) != nullptr);
                    auto* bboxCenter = boost::get<vrml_proc::parser::Vec3f>(&field.value);

                    CHECK_THAT(bboxCenter->x, Catch::Matchers::WithinAbs(58.7, 0.01));
                    CHECK_THAT(bboxCenter->y, Catch::Matchers::WithinAbs(0.0, 0.0));
                    CHECK_THAT(bboxCenter->z, Catch::Matchers::WithinAbs(15.0, 0.0));
                }

                void operator()(vrml_proc::parser::UseNode& useNode) {
                    FAIL("Unexpected type inside the variant.");
                }

                uintptr_t ptr;
            };

            VrmlNodeVisitor visitor;
            boost::apply_visitor(visitor, variant);
            variantPtr = visitor.ptr;
        }

        auto def = manager.GetDefinitionNode("id");
        CHECK(def.has_value());
        CHECK(reinterpret_cast<uintptr_t>(&def.value().get()) == variantPtr);
    }
}

TEST_CASE("VrmlNodeManager") {

    vrml_proc::parser::VrmlNodeManager manager;

    vrml_proc::parser::VrmlNode one;
    one.header = "Group";
    one.definitionName = boost::optional<std::string>("id");

    REQUIRE(manager.GetDefNodesTotalCount() == 0);
    REQUIRE_FALSE(manager.GetDefinitionNode("id").has_value());

    manager.AddDefinitionNode(one.definitionName.value(), one);

    REQUIRE(manager.GetDefNodesTotalCount() == 1);
    REQUIRE(manager.GetDefinitionNode("id").has_value());
    CHECK(&(manager.GetDefinitionNode("id").value().get()) == &one);
    
    vrml_proc::parser::VrmlNode two;
    two.header = "Anchor";
    two.definitionName = boost::optional<std::string>("id");

    manager.AddDefinitionNode(two.definitionName.value(), two);

    REQUIRE(manager.GetDefNodesTotalCount() == 1);
    REQUIRE(manager.GetDefinitionNode("id").has_value());
    CHECK(&(manager.GetDefinitionNode("id").value().get()) == &two);

    vrml_proc::parser::VrmlNode three;
    three.header = "Collision";
    three.definitionName = boost::optional<std::string>("_1");

    manager.AddDefinitionNode(three.definitionName.value(), three);

    REQUIRE(manager.GetDefNodesTotalCount() == 2);
    REQUIRE(manager.GetDefinitionNode("id").has_value());
    CHECK(&(manager.GetDefinitionNode("id").value().get()) == &two);
    REQUIRE(manager.GetDefinitionNode("_1").has_value());
    CHECK(&(manager.GetDefinitionNode("_1").value().get()) == &three);
}
