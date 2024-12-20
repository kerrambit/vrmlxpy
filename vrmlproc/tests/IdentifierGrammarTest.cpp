#include <catch2/catch_test_macros.hpp>

#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_UNICODE

#include <optional>
#include <string>

#include "CommentSkipper.hpp"
#include "IdentifierGrammar.hpp"


static std::optional<std::string> ParseIdentifier(std::string& text) {

    auto iterator = text.begin();

    vrml_proc::parser::IdentifierGrammar<std::string::iterator, vrml_proc::parser::CommentSkipper> grammar;
    std::string data;
    vrml_proc::parser::CommentSkipper skipper;

    bool success = boost::spirit::qi::phrase_parse(iterator, text.end(), grammar, skipper, data);
    if (success) {
        return data;
    }

    return {};
}

TEST_CASE("Parse Identifier - Simple ID", "[parsing][valid]") {

    std::string identifier = "ID";
    auto parseResult = ParseIdentifier(identifier);
    REQUIRE(parseResult.has_value());   
}

TEST_CASE("Parse Identifier - Complicated ID", "[parsing][valid]") {

    std::string identifier = "èesko";
    auto parseResult = ParseIdentifier(identifier);
    REQUIRE(parseResult.has_value());
}