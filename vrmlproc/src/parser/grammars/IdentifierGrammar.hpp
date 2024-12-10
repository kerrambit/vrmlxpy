#pragma once

#include <boost/spirit/include/qi.hpp>
#include <string>

namespace vrml_proc {
    namespace parser {

        template<typename Iterator, typename Skipper>
        struct IdentifierGrammar : boost::spirit::qi::grammar<Iterator, std::string(), Skipper> {

            IdentifierGrammar()
                : IdentifierGrammar::base_type(start) {

                start = boost::spirit::qi::lexeme[
                    (boost::spirit::qi::char_("!$%&()*;:<=>?@^_`|~") | boost::spirit::qi::alpha) >>
                        *(boost::spirit::qi::alnum | boost::spirit::qi::char_("!$%&()*;:<=>?@^_`|~"))
                ];

                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            boost::spirit::qi::rule<Iterator, std::string(), Skipper> start;
        };
    }
}
