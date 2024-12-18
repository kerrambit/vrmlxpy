#pragma once

#include <string>

#include <boost/spirit/include/qi.hpp>

namespace vrml_proc {
    namespace parser {

        template<typename Iterator, typename Skipper>
        struct QuotedStringGrammar : boost::spirit::qi::grammar<Iterator, std::string(), Skipper> {

            QuotedStringGrammar()
                : QuotedStringGrammar::base_type(start) {

                start = boost::spirit::qi::lexeme['"' >> +(boost::spirit::qi::char_ - '"') >> '"'];

                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            boost::spirit::qi::rule<Iterator, std::string(), Skipper> start;
        };
    }
}
