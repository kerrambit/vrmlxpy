#include "VrmlParser.hpp"

#include <optional>
#include <string>
#include <vector>

#include <boost/spirit/home/qi/parse.hpp>

#include "Logger.hpp"
#include "ParserResult.hpp"
#include "ScopedTimer.hpp"
#include "VrmlFile.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManagerPopulator.hpp"
#include "FormatString.hpp"

namespace vrml_proc {

	namespace parser {

        ParserResult<VrmlFile> VrmlParser::Parse(const char* begin, const char* end)
		{
            vrml_proc::core::logger::LogInfo("Parse VRML file content.", LOGGING_INFO);

            std::vector<VrmlNode> parsedData;

            double time = 0.0;
            bool success = false;
            {
                auto timer = vrmlproc::core::utils::ScopedTimer(time);
                success = boost::spirit::qi::phrase_parse(begin, end, m_grammar, m_skipper, parsedData);
            }

            if (success && begin == end) {
                vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Parsing was successful. The whole parsing and AST creation process took ", time, "seconds."), LOGGING_INFO);

                vrml_proc::core::logger::LogInfo("Populate VrmlNodeManager with DEF nodes.", LOGGING_INFO);
                double time = 0.0;
                {
                    auto timer = vrmlproc::core::utils::ScopedTimer(time);
                    for (const auto& root : parsedData) {
                        VrmlNodeManagerPopulator::Populate(m_manager, root);
                    }
                }
                vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("DEF nodes populating has finished. The whole process took ", time, "seconds."), LOGGING_INFO);

                return parsedData;
            }

            vrml_proc::core::logger::LogInfo(vrml_proc::core::utils::FormatString("Parsing was not successful. The process took ", time, "seconds."), LOGGING_INFO);
            return {};
		}
	}
}

