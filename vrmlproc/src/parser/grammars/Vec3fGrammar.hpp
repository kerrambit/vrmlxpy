#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Vec3f.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Vec3f,
    (vrml_proc::parser::float32_t, x)
    (vrml_proc::parser::float32_t, y)
    (vrml_proc::parser::float32_t, z)
)

namespace vrml_proc {
    namespace parser {

        template<typename Iterator, typename Skipper>
        struct Vec3fGrammar : boost::spirit::qi::grammar<Iterator, Vec3f(), Skipper> {

            Vec3fGrammar() : Vec3fGrammar::base_type(start) {
                start = boost::spirit::qi::float_ >> boost::spirit::qi::float_ >> boost::spirit::qi::float_;

                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            boost::spirit::qi::rule<Iterator, Vec3f(), Skipper> start;
        };
    }
}
