#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "BaseGrammar.hpp"
#include "Int32Array.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Int32Array,
    (std::vector<int32_t>, integers)
)

namespace vrml_proc {
    namespace parser {

        template <typename Iterator, typename Skipper>
        class Int32ArrayGrammar
            : public boost::spirit::qi::grammar<Iterator, Int32Array(), Skipper>,
            public BaseGrammar<Iterator, Int32Array(), Skipper> {

        public:
            Int32ArrayGrammar()
                : Int32ArrayGrammar::base_type(this->m_start) {

                this->m_start =
                    "[" >> (boost::spirit::qi::int_ % ",") >> "]";

                BOOST_SPIRIT_DEBUG_NODE(this->m_start);
            }
        };
    }
}
