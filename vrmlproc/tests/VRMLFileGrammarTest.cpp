#include <catch2/catch_test_macros.hpp>
#include <string>

#include <vrml_processing.hpp>

#include "test-data/VRMLFileGrammarTestDataset.hpp"

TEST_CASE("Parse VRMLFile - Valid Input - Simple VRML File", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(simple);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Two Simple Nodes", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(two_simple_nodes);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Node In Node", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(node_in_node);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Quite Deep Recursive Nodes", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(quite_deep_recursive_nodes);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Group With Nodes Array", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(group_with_nodes_array);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Simple DEF", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(simple_def);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Simple USE", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(simple_use);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Children With Both USE, DEF And Normal type nodes", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(children_with_both_USE_DEF_and_normal_type_nodes);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Classic", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(classic);
    CHECK(parseResult);
}

TEST_CASE("Parse VRMLFile - Valid Input - Node With Switch", "[parsing]") {

    bool parseResult = vrml_proc::parseVRMLFile(node_with_switch);
    CHECK(parseResult);
}

