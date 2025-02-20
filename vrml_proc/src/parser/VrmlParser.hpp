#pragma once

// #define BOOST_SPIRIT_DEBUG

#include "CommentSkipper.hpp"
#include "Parser.hpp"
#include "VrmlFile.hpp"
#include "VrmlFileGrammar.hpp"
#include "VrmlProcessingExport.hpp"

#include "MemoryMappedFileReader.hpp"

namespace vrml_proc {
    namespace parser {

        class VRMLPROCESSING_API VrmlParser : public Parser<const char*, VrmlFile> {
        public:
            VrmlParser() = delete;

            VrmlParser(VrmlNodeManager& manager)
                : Parser<const char*, VrmlFile>(), m_manager(manager), m_grammar(), m_skipper() {}

            ParserResult<VrmlFile> Parse(const char* begin, const char* end) override;

        private:
            VrmlFileGrammar<const char*, CommentSkipper> m_grammar;
            CommentSkipper m_skipper;
            VrmlNodeManager& m_manager;
        };
    }
}
