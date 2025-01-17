#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <vadefs.h>
#include <vector>

#include <boost/variant/get.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>

#include "test_data/VrmlFieldExtractorTestDataset.hpp"
#include <Int32Array.hpp>
#include <ParserResult.hpp>
#include <UseNode.hpp>
#include <Vec3f.hpp>
#include <Vec3fArray.hpp>
#include <Vec4f.hpp>
#include <VrmlField.hpp>
#include <VrmlFieldExtractor.hpp>
#include <VrmlFile.hpp>
#include <VrmlNode.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>

static vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> ParseVrmlFile(std::string& text, vrml_proc::parser::VrmlNodeManager& manager) {

    vrml_proc::parser::VrmlParser parser(manager);
    return parser.Parse(text);
}

TEST_CASE("IsNamePresent - Valid", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(first, manager);
    REQUIRE(parseResult);

    auto& fields = parseResult.value().at(0).fields;

    CHECK(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("field1", fields));
    CHECK(vrml_proc::parser::model::utils::VrmlFieldExtractor::IsNamePresent("field2", fields));
}

TEST_CASE("IsNamePresent - Inalid", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(first, manager);
    REQUIRE(parseResult);

    auto& fields = parseResult.value().at(0).fields;

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

TEST_CASE("ExtractByNameExtended - VrmlNode Non-recursive I.", "[valid]") {

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


TEST_CASE("ExtractByNameExtended - VrmlNode Non-recursive II.", "[valid]") {

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

TEST_CASE("ExtractByNameExtended - VrmlNode Array", "[valid]") {

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

TEST_CASE("ExtractByNameExtended - Empty Array of Vec3f's", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(emptyArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::Vec3fArray* ptr = boost::get<vrml_proc::parser::Vec3fArray>(&fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::Vec3fArray>("array", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Empty Array of Int32's", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(emptyArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    auto& x = fieldsOfRoot.at(0).value;
    vrml_proc::parser::Vec3fArray* ptr = boost::get<vrml_proc::parser::Vec3fArray>(&fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::Int32Array>("array", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(reinterpret_cast<uintptr_t>(ptr) == reinterpret_cast<uintptr_t>(&(result.value().get())));
}

TEST_CASE("ExtractByNameExtended - String", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::string* ptr = boost::get<std::string>(&fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<std::string>("string", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Bool", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    bool* ptr = boost::get<bool>(&fieldsOfRoot.at(1).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<bool>("bool", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Vec3f", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::Vec3f* ptr = boost::get<vrml_proc::parser::Vec3f>(&fieldsOfRoot.at(2).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::Vec3f>("vec3f", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Vec3fArray", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::Vec3fArray* ptr = boost::get<vrml_proc::parser::Vec3fArray>(&fieldsOfRoot.at(3).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::Vec3fArray>("vec3farray", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Vec4", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::Vec4f* ptr = boost::get<vrml_proc::parser::Vec4f>(&fieldsOfRoot.at(4).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::Vec4f>("vec4f", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - Int32Array", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::Int32Array* ptr = boost::get<vrml_proc::parser::Int32Array>(&fieldsOfRoot.at(5).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::Int32Array>("int32array", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - float", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    float* ptr = boost::get<float>(&fieldsOfRoot.at(6).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<float>("float", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - int32", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(simpleEntityTypes, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    int32_t* ptr = boost::get<int32_t>(&fieldsOfRoot.at(7).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<int32_t>("int32", fieldsOfRoot, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractByNameExtended - UseNode", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::VrmlNode& shape = boost::get<vrml_proc::parser::VrmlNode>(fieldsOfRoot.at(0).value);
    vrml_proc::parser::UseNode* ptr = boost::get<vrml_proc::parser::UseNode>(&shape.fields.at(2).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<vrml_proc::parser::UseNode>("validUseId", shape.fields, invalidType);
    REQUIRE(result.has_value());
    CHECK(ptr == &(result.value().get()));
}

TEST_CASE("ExtractFromVariantExtended - Invalid I.", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& children = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldsOfRoot.at(0).value);
    vrml_proc::parser::VrmlNode& node = boost::get<vrml_proc::parser::VrmlNode>(children.at(0));

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant = children.at(0);
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariantExtended<boost::recursive_wrapper<vrml_proc::parser::UseNode>>(variant, invalidType);
    REQUIRE(!result.has_value());
    CHECK(invalidType.find("struct vrml_proc::parser::VrmlNode") != std::string::npos);
}

TEST_CASE("ExtractFromVariantExtended - Invalid II.", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& children = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant = children.at(1);
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariantExtended<vrml_proc::parser::VrmlNode>(variant, invalidType);
    REQUIRE(!result.has_value());
    CHECK(invalidType.find("struct vrml_proc::parser::UseNode") != std::string::npos);
}

TEST_CASE("ExtractFromVariantExtended - VrmlNode", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& children = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldsOfRoot.at(0).value);
    vrml_proc::parser::VrmlNode& node = boost::get<vrml_proc::parser::VrmlNode>(children.at(0));
    vrml_proc::parser::VrmlNode* ptr = &node;

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant = children.at(0);
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariantExtended<vrml_proc::parser::VrmlNode>(variant, invalidType);
    REQUIRE(result.has_value());
    CHECK(&result.value().get() == ptr);
}

TEST_CASE("ExtractFromVariantExtended - UseNode", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& children = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldsOfRoot.at(0).value);
    vrml_proc::parser::UseNode& node = boost::get<vrml_proc::parser::UseNode>(children.at(1));
    vrml_proc::parser::UseNode* ptr = &node;

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType;
    const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant = children.at(1);
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariantExtended<vrml_proc::parser::UseNode>(variant, invalidType);
    REQUIRE(result.has_value());
    CHECK(&result.value().get() == ptr);
}

TEST_CASE("ExtractVrmlNodeFromVariantExtended - VrmlNode", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& children = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldsOfRoot.at(0).value);
    vrml_proc::parser::VrmlNode& node = boost::get<vrml_proc::parser::VrmlNode>(children.at(0));
    vrml_proc::parser::VrmlNode* ptr = &node;

    const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant = children.at(0);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType; std::string useId;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantExtended(variant, manager, invalidType, useId);
    REQUIRE(result.has_value());
    std::reference_wrapper<const vrml_proc::parser::VrmlNode>& wrapper = result.value();
    CHECK(&wrapper.get() == ptr);
}

TEST_CASE("ExtractVrmlNodeFromVariantExtended - UseNode", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& children = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldsOfRoot.at(0).value);

    const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant = children.at(1);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType; std::string useId;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantExtended(variant, manager, invalidType, useId);
    REQUIRE(result.has_value());
    std::reference_wrapper<const vrml_proc::parser::VrmlNode>& wrapper = result.value();
    CHECK(&wrapper.get() == &(*manager.GetDefinitionNode("id")));
}

TEST_CASE("ExtractVrmlNodeFromVariantExtended - UseNode Is Missing", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(children, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& children = boost::get<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>(fieldsOfRoot.at(0).value);

    const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant = children.at(2);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType; std::string useId;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantExtended(variant, manager, invalidType, useId);
    REQUIRE(result.has_error());
    CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeFromVariantError::UnknownUseNode);
    CHECK(useId == "missingId");
}

TEST_CASE("ExtractVrmlNodeExtended - Field Not Found", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::VrmlNode& shape = boost::get<vrml_proc::parser::VrmlNode>(fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType; std::string useId;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended("logic", shape.fields, manager, invalidType, useId);
    REQUIRE(result.has_error());
    CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::FieldNotFound);
}

TEST_CASE("ExtractVrmlNodeExtended - Validation Error", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::VrmlNode& shape = boost::get<vrml_proc::parser::VrmlNode>(fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType; std::string useId;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended("appearance", shape.fields, manager, invalidType, useId);
    REQUIRE(result.has_error());
    CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError);
    CHECK(invalidType.find("struct vrml_proc::parser::Vec4f") != std::string::npos);
}

TEST_CASE("ExtractVrmlNodeExtended - Missing USE Id Error", "[invalid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::VrmlNode& shape = boost::get<vrml_proc::parser::VrmlNode>(fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType; std::string useId;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended("invalidUseId", shape.fields, manager, invalidType, useId);
    REQUIRE(result.has_error());
    CHECK(result.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::UnknownUseNode);
    CHECK(useId == "idd");
}

TEST_CASE("ExtractVrmlNodeExtended - USENode", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::VrmlNode& shape = boost::get<vrml_proc::parser::VrmlNode>(fieldsOfRoot.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType; std::string useId;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended("validUseId", shape.fields, manager, invalidType, useId);
    REQUIRE(result.has_value());
    CHECK(&(*manager.GetDefinitionNode("id")) == &(result.value().get()));
}

TEST_CASE("ExtractVrmlNodeExtended - VrmlNode", "[valid]") {

    vrml_proc::parser::VrmlNodeManager manager;
    auto parseResult = ParseVrmlFile(nodesArray, manager);
    REQUIRE(parseResult);

    auto& fieldsOfRoot = parseResult.value().at(0).fields;
    vrml_proc::parser::VrmlNode& shape = boost::get<vrml_proc::parser::VrmlNode>(fieldsOfRoot.at(0).value);
    vrml_proc::parser::VrmlNode& geometry = boost::get<vrml_proc::parser::VrmlNode>(shape.fields.at(0).value);

    std::cout << "Start of extracting:" << std::endl;
    std::string invalidType; std::string useId;
    auto result = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended("geometry", shape.fields, manager, invalidType, useId);
    REQUIRE(result.has_value());
    CHECK(&(result.value().get()) == &geometry);
}
