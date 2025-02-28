#pragma once

// #define BOOST_SPIRIT_DEBUG

#include "CommentSkipper.hpp"
#include "Parser.hpp"
#include "VrmlFile.hpp"
#include "VrmlFileGrammar.hpp"
#include "VrmlProcessingExport.hpp"
#include "BufferView.hpp"

#include "MemoryMappedFileReader.hpp"

namespace vrml_proc {
  namespace parser {

    class VRMLPROCESSING_API VrmlParser : public Parser<BufferView, VrmlFile> {
     public:
      VrmlParser() = delete;

      VrmlParser(VrmlNodeManager& manager)
          : Parser<BufferView, VrmlFile>(), m_manager(manager), m_grammar(), m_skipper() {}

      ParserResult<VrmlFile> Parse(BufferView buffer) override;

     private:
      VrmlFileGrammar<const char*, CommentSkipper> m_grammar;
      CommentSkipper m_skipper;
      VrmlNodeManager& m_manager;
    };
  }  // namespace parser
}  // namespace vrml_proc
