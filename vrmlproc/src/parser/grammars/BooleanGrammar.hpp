#pragma once

#include <boost/spirit/include/qi.hpp>

namespace vrml_proc {
    namespace parser {

        template<typename Iterator, typename Skipper>
        struct BooleanGrammar : boost::spirit::qi::grammar<Iterator, bool(), Skipper> {

            BooleanGrammar()
                : BooleanGrammar::base_type(start) {

                start = (boost::spirit::qi::lit("TRUE")[boost::spirit::qi::_val = true] | boost::spirit::qi::lit("FALSE")[boost::spirit::qi::_val = false]);

                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            boost::spirit::qi::rule<Iterator, bool(), Skipper> start;
        };
    }
}
