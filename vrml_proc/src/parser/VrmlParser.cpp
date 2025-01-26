#include "VrmlParser.hpp"

#include <boost/spirit/include/qi.hpp>

#include "Logger.hpp"
#include "ScopedTimer.hpp"

namespace vrml_proc {

	namespace parser {

        ParserResult<VrmlFile> VrmlParser::Parse(std::string& string)
		{
            auto iterator = string.begin();
            std::vector<VrmlNode> parsedData;

            double time;
            bool success;
            {
                auto timer = vrmlproc::core::utils::ScopedTimer(time);
                success = boost::spirit::qi::phrase_parse(iterator, string.end(), m_grammar, m_skipper, parsedData);
            }

            if (success && iterator == string.end()) {
                LOG_INFO() << "Parsing was successful. The whole parsing and AST creation process took " << time << " seconds.";
                return std::optional(parsedData);
            }

            LOG_ERROR() << "Parsing was not successful. Process took " << time << " seconds.";
            return {};
		}
	}
}

