#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <boost/spirit/include/qi.hpp>

#include "vrml_processing.hpp"
#include "VRMLField.hpp"
#include "Vec3f.hpp"
#include "Vec4f.hpp"
#include "Vec3fArray.hpp"
#include "Int32Array.hpp"
#include "VRMLNode.hpp"
#include "VRMLNodeManager.hpp"

#include "CommentSkipper.hpp"
#include "VRMLNodeGrammar.hpp"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

// -----------------------------------------------------------

bool vrml_proc::parseVec3f(std::string& text) {

    auto it = text.begin();
    vrml_proc::parser::Vec3fGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar;
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

// -----------------------------------------------------------

bool vrml_proc::parseVec4f(std::string& text) {

    auto it = text.begin();
    vrml_proc::parser::Vec4fGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar;
    vrml_proc::parser::Vec4f data;
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

bool vrml_proc::parseVec3fArray(std::string& text) {

    auto it = text.begin();
    vrml_proc::parser::Vec3fArrayGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar;
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

bool vrml_proc::parseInt32Array(std::string& text) {

    auto it = text.begin();
    vrml_proc::parser::Int32ArrayGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar;
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

bool vrml_proc::parseVRMLFile(std::string& text) {

    vrml_proc::parser::VRMLNodeManager manager;

    auto it = text.begin();
    vrml_proc::parser::VRMLNodeGrammar <std::string::iterator, vrml_proc::parser::CommentSkipper> grammar(manager);
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