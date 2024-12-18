#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Int32Array.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Int32Array,
    (std::vector<int32_t>, integers)
)

namespace vrml_proc {
    namespace parser {

        template <typename Iterator, typename Skipper>
        struct Int32ArrayGrammar : boost::spirit::qi::grammar<Iterator, Int32Array(), Skipper> {

            Int32ArrayGrammar() : Int32ArrayGrammar::base_type(start) {

                start = "["
                    >> (boost::spirit::qi::int_ % ",")
                    >> "]";

                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            boost::spirit::qi::rule<Iterator, Int32Array(), Skipper> start;
        };
    }
}
