#pragma once
#include <string>

#include <boost/spirit/include/qi.hpp>
#include "BaseGrammar.hpp"

namespace vrml_proc {
    namespace parser {

        template<typename Iterator, typename Skipper>
        class IdentifierGrammar : public boost::spirit::qi::grammar<Iterator, std::string(), Skipper>, public BaseGrammar<Iterator, std::string(), Skipper> {
            public:
                IdentifierGrammar()
                    : IdentifierGrammar::base_type(this->m_start) {

                    this->m_start = boost::spirit::qi::lexeme[
                        (boost::spirit::qi::char_("!$%&()*;:<=>?@^_`|~") | boost::spirit::qi::alpha) >>
                            *(boost::spirit::qi::alnum | boost::spirit::qi::char_("!$%&()*;:<=>?@^_`|~"))
                    ];

                    BOOST_SPIRIT_DEBUG_NODE(this->m_start);
                }
        };
    }
}
