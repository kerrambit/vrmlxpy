#pragma once

#include "Parser.hpp"
#include "VRMLFile.hpp"
#include "VrmlFileGrammar.hpp"
#include "CommentSkipper.hpp"

namespace vrml_proc {
    namespace parser {

        class VrmlParser : public Parser<VRMLFile> {
        public:
            VrmlParser() = delete;

            VrmlParser(VRMLNodeManager& manager)
                : Parser<VRMLFile>(), m_manager(manager), m_grammar(manager), m_skipper() {} 

            ParserResult<VRMLFile> Parse(std::string& string) override;

        private:
            VrmlFileGrammar<std::string::iterator, CommentSkipper> m_grammar;
            CommentSkipper m_skipper;
            VRMLNodeManager& m_manager;
        };
    }
}
