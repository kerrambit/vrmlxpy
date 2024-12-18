#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Vec4f.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Vec4f,
    (vrml_proc::parser::float32_t, x)
    (vrml_proc::parser::float32_t, y)
    (vrml_proc::parser::float32_t, z)
    (vrml_proc::parser::float32_t, w)
)

namespace vrml_proc {
    namespace parser {

        template<typename Iterator, typename Skipper>
        struct Vec4fGrammar : boost::spirit::qi::grammar<Iterator, Vec4f(), Skipper> {

            Vec4fGrammar() : Vec4fGrammar::base_type(start) {
                start = boost::spirit::qi::float_ >> boost::spirit::qi::float_ >> boost::spirit::qi::float_ >> boost::spirit::qi::float_;

                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            boost::spirit::qi::rule<Iterator, Vec4f(), Skipper> start;
        };
    }
}
