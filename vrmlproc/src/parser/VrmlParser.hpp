#pragma once

// #define BOOST_SPIRIT_DEBUG

#include "Parser.hpp"
#include "VrmlFile.hpp"
#include "VrmlFileGrammar.hpp"
#include "CommentSkipper.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {

        class VRMLPROCESSING_API VrmlParser : public Parser<VrmlFile> {
        public:
            VrmlParser() = delete;

            VrmlParser(VrmlNodeManager& manager)
                : Parser<VrmlFile>(), m_manager(manager), m_grammar(manager), m_skipper() {} 

            ParserResult<VrmlFile> Parse(std::string& string) override;

        private:
            VrmlFileGrammar<std::string::iterator, CommentSkipper> m_grammar;
            CommentSkipper m_skipper;
            VrmlNodeManager& m_manager;
        };
    }
}
