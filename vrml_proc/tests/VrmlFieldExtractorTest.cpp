#include <catch2/catch_test_macros.hpp>

#include <vector>
#include <string>

#include <boost/variant.hpp>

#include "test_data/VrmlFieldExtractorTestDataset.hpp"
#include <VrmlField.hpp>
#include <VrmlFieldExtractor.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>
#include <Int32Array.hpp>
#include <iostream>
#include <UseNode.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>

static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(std::string& text, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::parser::VrmlParser parser(manager);
    return parser.Parse(text);
}

TEST_CASE("IsNamePresent") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(first, manager);
    REQUIRE(parseResult);

    auto fields = parseResult.value().at(0).fields;

    CHECK(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("field1", fields));
    CHECK(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("field2", fields));
    CHECK_FALSE(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("field3", fields));
    CHECK_FALSE(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("color", fields));
}

TEST_CASE("ExtractByNameExtended - Missing Field", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fields = parseResult.value().at(0).fields;

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::Int32Array>("light", fields, invalidType);
    REQUIRE(result.has_error());
    CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::FieldNotFound);
}

TEST_CASE("ExtractByNameExtended - Validation Error", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fields = parseResult.value().at(0).fields;

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::Int32Array>("shape", fields, invalidType);
    REQUIRE(result.has_error());
    CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError);
    CHECK(invalidType.find("struct vrml_proc::parser::VrmlNode") != std::string::npos);
}

TEST_CASE("ExtractByNameExtended - VrmlNode non-recursive 1 ", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::VrmlNode* ptr = boost::get<vrml_proc::parser::VrmlNode>(&fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::VrmlNode>("shape", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}


TEST_CASE("ExtractByNameExtended - VrmlNode non-recursive 2", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::VrmlNode& shape = boost::get<vrml_proc::parser::VrmlNode>(fieldsOfRoot.at(0).value);
    auto& fieldsOfShape = shape.fields;
    vrml_proc::parser::VrmlNode* ptr = boost::get<vrml_proc::parser::VrmlNode>(&fieldsOfShape.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::VrmlNode>("geometry", fieldsOfShape, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - VrmlNode array", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>* ptr = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(&fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>("children", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - ????????????????????", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& children = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldsOfRoot.at(0).value);
    boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& wrapper = boost::get<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>(children.at(0));
    vrml_proc::parser::VrmlNode* ptr = &(wrapper.get());

    // TODO: extract from variant this vrml node

    std::cout << "Start of extracting:" << std::endl;
}