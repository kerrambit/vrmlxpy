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

namespace vrml_proc {

	namespace parser {

        ParserResult<VrmlFile> VrmlParser::Parse(std::string& string)
		{
            LOG_INFO() << "Parse VRML file content.";

            auto iterator = string.begin();
            std::vector<VrmlNode> parsedData;

            double time = 0.0;
            bool success = false;
            {
                auto timer = vrmlproc::core::utils::ScopedTimer(time);
                success = boost::spirit::qi::phrase_parse(iterator, string.end(), m_grammar, m_skipper, parsedData);
            }

            if (success && iterator == string.end()) {
                LOG_INFO() << "Parsing was successful. The whole parsing and AST creation process took " << time << " seconds.";

                LOG_INFO() << "Populate VrmlNodeManager with DEF nodes.";
                double time = 0.0;
                {
                    auto timer = vrmlproc::core::utils::ScopedTimer(time);
                    for (const auto& root : parsedData) {
                        VrmlNodeManagerPopulator::Populate(m_manager, root);
                    }
                }
                LOG_INFO() << "DEF nodes populating has finished. The whole process took " << time << " seconds.";

                return parsedData;
            }

            LOG_ERROR() << "Parsing was not successful. Process took " << time << " seconds.";
            return {};
		}
	}
}

