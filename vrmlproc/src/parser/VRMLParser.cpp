#include "VrmlParser.hpp"

#include <boost/spirit/include/qi.hpp>

namespace vrml_proc {

	namespace parser {

        ParserResult<VrmlFile> VrmlParser::Parse(std::string& string)
		{
            auto iterator = string.begin();
            std::vector<VrmlNode> parsedData;

            bool success = boost::spirit::qi::phrase_parse(iterator, string.end(), m_grammar, m_skipper, parsedData);

            if (success && iterator == string.end()) {
                return std::optional(parsedData);
            }

            return {};
		}
	}
}

