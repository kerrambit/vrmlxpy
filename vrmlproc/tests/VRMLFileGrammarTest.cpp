#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <vector>

#include <boost/variant.hpp>

#include <vrml_processing.hpp>
#include <VRMLNode.hpp>
#include <Vec3fArray.hpp>
#include <Vec3f.hpp>
#include <VRMLUnits.hpp>

#include "test-data/VRMLFileGrammarTestDataset.hpp"

#define BOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT

TEST_CASE("Parse VRML File - Valid Input - Simple VRML File", "[parsing][valid]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(simple);
    REQUIRE(parseResult);

    vrml_proc::parser::VRMLNode root = parseResult.value().at(0);
    CHECK(root.header == "WorldInfo");

    {
        auto field = root.fields.at(0);
        CHECK(field.name == "title");

        REQUIRE(boost::get<std::string>(&field.value) != nullptr);
        auto valueAsString = boost::get<std::string>(field.value);
        CHECK(valueAsString == "cube.wrl");
    }

    {
        auto field = root.fields.at(1);
        CHECK(field.name == "info");

        REQUIRE(boost::get<std::string>(&field.value) != nullptr);
        auto valueAsString = boost::get<std::string>(field.value);
        CHECK(valueAsString == "VRML file.");
    }
}

TEST_CASE("Parse VRML File - Valid Input - Two Simple Nodes", "[parsing][valid]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(two_simple_nodes);
    REQUIRE(parseResult);

    {
        vrml_proc::parser::VRMLNode root = parseResult.value().at(0);
        CHECK(root.header == "WorldInfo");

        {
            auto field = root.fields.at(0);
            CHECK(field.name == "title");

            REQUIRE(boost::get<std::string>(&field.value) != nullptr);
            auto valueAsString = boost::get<std::string>(field.value);
            CHECK(valueAsString == "cube.wrl");
        }

        {
            auto field = root.fields.at(1);
            CHECK(field.name == "info");

            REQUIRE(boost::get<std::string>(&field.value) != nullptr);
            auto valueAsString = boost::get<std::string>(field.value);
            CHECK(valueAsString == "VRML file.");
        }
    }

    {
        vrml_proc::parser::VRMLNode root = parseResult.value().at(1);
        CHECK(root.header == "Group");

        {
            auto field = root.fields.at(0);
            CHECK(field.name == "geometry");

            auto* value = boost::get<vrml_proc::parser::Vec3fArray>(&field.value);
            REQUIRE(value != nullptr);
            CHECK_THAT(value->vectors.at(0).x, Catch::Matchers::WithinAbs(58.5, 0.1));
            CHECK_THAT(value->vectors.at(0).y, Catch::Matchers::WithinAbs(458.02, 0.1));
            CHECK_THAT(value->vectors.at(0).z, Catch::Matchers::WithinAbs(68.78, 0.1));
        }

        {
            auto field = root.fields.at(1);
            CHECK(field.name == "geometry");

            auto* value = boost::get<vrml_proc::parser::Vec3f>(&field.value);
            REQUIRE(value != nullptr);
            CHECK_THAT(value->x, Catch::Matchers::WithinAbs(100.001001, 0.1));
            CHECK_THAT(value->y, Catch::Matchers::WithinAbs(5.55317, 0.1));
            CHECK_THAT(value->z, Catch::Matchers::WithinAbs(-3.05561, 0.1));
        }
    }
}

TEST_CASE("Parse VRML File - Valid Input - Node In Node", "[parsing][valid]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(node_in_node);
    REQUIRE(parseResult);

    {
        vrml_proc::parser::VRMLNode root = parseResult.value().at(0);
        CHECK(root.header == "Shape");

        auto field = root.fields.at(0);
        CHECK(field.name == "appearance");

        auto* value = boost::get<vrml_proc::parser::VRMLNode>(&field.value);
        REQUIRE(value != nullptr);
            
        CHECK(value->header == "Appearence");
        {
            auto field = value->fields.at(0);
            CHECK(field.name == "ambientIntensity");
            REQUIRE(boost::get<vrml_proc::parser::float32_t>(&field.value) != nullptr);
            auto value = boost::get<vrml_proc::parser::float32_t>(field.value);
            CHECK_THAT(value, Catch::Matchers::WithinAbs(0.02, 0.1));
        }
    }
}

TEST_CASE("Parse VRML File - Valid Input - Quite Deep Recursive Nodes", "[parsing][valid]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(quite_deep_recursive_nodes);
    REQUIRE(parseResult);

    {
        vrml_proc::parser::VRMLNode root = parseResult.value().at(0);
        CHECK(root.header == "Shape");

        auto field = root.fields.at(0);
        CHECK(field.name == "appearance");

        auto* value = boost::get<vrml_proc::parser::VRMLNode>(&field.value);
        REQUIRE(value != nullptr);

        CHECK(value->header == "Appearence");
        {
            auto field = value->fields.at(0);
            CHECK(field.name == "ambientIntensity");
            REQUIRE(boost::get<vrml_proc::parser::float32_t>(&field.value) != nullptr);
            auto ambientIntensity = boost::get<vrml_proc::parser::float32_t>(field.value);
            CHECK_THAT(ambientIntensity, Catch::Matchers::WithinAbs(0.02, 0.1));

            {
                auto field = value->fields.at(1);
                CHECK(field.name == "appearance");
                auto* value = boost::get<vrml_proc::parser::VRMLNode>(&field.value);
                REQUIRE(value != nullptr);
                CHECK(value->header == "Appearence");
                auto inner_field = value->fields.at(0);
                CHECK(inner_field.name == "lightExposure");
                REQUIRE(boost::get<int32_t>(&inner_field.value) != nullptr);
                auto lightExposure = boost::get<int32_t>(inner_field.value);
                CHECK(lightExposure == 42);
            }
        }
    }
}

TEST_CASE("Parse VRMLFile - Valid Input - Group With Nodes Array", "[parsing][valid]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(group_with_nodes_array);
    REQUIRE(parseResult);

    {
        vrml_proc::parser::VRMLNode root = parseResult.value().at(0);
        CHECK(root.header == "Group");

        auto field = root.fields.at(0);
        CHECK(field.name == "children");

        auto* value = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VRMLNode>, boost::recursive_wrapper<vrml_proc::parser::USENode>>>>(&field.value);
        REQUIRE(value != nullptr);

        {
            auto& first_variant = value->at(0);

            struct VrmlNodeVisitor : public boost::static_visitor<void> {
                void operator()(vrml_proc::parser::VRMLNode& vrml_node) const {

                    REQUIRE(vrml_node.header == "Group");
                    auto field = vrml_node.fields.at(0);
                    CHECK(field.name == "bboxCenter");

                    REQUIRE(boost::get<vrml_proc::parser::Vec3f>(&field.value) != nullptr);
                    auto bboxCenter = boost::get<vrml_proc::parser::Vec3f>(&field.value);

                    CHECK_THAT(bboxCenter->x, Catch::Matchers::WithinAbs(0.058, 0.01));
                    CHECK_THAT(bboxCenter->y, Catch::Matchers::WithinAbs(0.0, 0.0));
                    CHECK_THAT(bboxCenter->z, Catch::Matchers::WithinAbs(15.0, 0.0));
                }

                void operator()(vrml_proc::parser::USENode& use_node) const {
                    FAIL("Unexpected type inside the variant.");
                }
            };

            VrmlNodeVisitor visitor;
            boost::apply_visitor(visitor, first_variant);
        }

        {
            auto& second_variant = value->at(1);

            struct SecondVrmlNodeVisitor : public boost::static_visitor<void> {
                void operator()(vrml_proc::parser::VRMLNode& vrml_node) const {

                    REQUIRE(vrml_node.header == "Group");
                    auto field = vrml_node.fields.at(0);
                    CHECK(field.name == "bboxSize");

                    REQUIRE(boost::get<vrml_proc::parser::Vec3f>(&field.value) != nullptr);
                    auto bboxCenter = boost::get<vrml_proc::parser::Vec3f>(&field.value);

                    CHECK_THAT(bboxCenter->x, Catch::Matchers::WithinAbs(-1.0, 0.0));
                    CHECK_THAT(bboxCenter->y, Catch::Matchers::WithinAbs(-1.0, 0.0));
                    CHECK_THAT(bboxCenter->z, Catch::Matchers::WithinAbs(-1.0, 0.0));
                }

                void operator()(vrml_proc::parser::USENode& use_node) const {
                    FAIL("Unexpected type inside the variant.");
                }
            };

            SecondVrmlNodeVisitor visitor;
            boost::apply_visitor(visitor, second_variant);
        }
    }
}

TEST_CASE("Parse VRMLFile - Valid Input - Simple DEF", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(simple_def);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Simple USE", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(simple_use);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Children With Both USE, DEF And Normal type nodes", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(children_with_both_USE_DEF_and_normal_type_nodes);
    CHECK(parseResult);
}


TEST_CASE("Parse VRMLFile - Valid Input - Node With Switch", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(node_with_switch);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Node With Boolean", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(node_with_boolean);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Empty node", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(empty_node);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Classic", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(classic);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Complicated node", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(complicated_node);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Complicated node with complicated DEF names", "[parsing]") {

    auto parseResult = vrml_proc::parser::ParseVrmlFile(complicated_node_with_complicated_def_names);
    CHECK(parseResult);
}
