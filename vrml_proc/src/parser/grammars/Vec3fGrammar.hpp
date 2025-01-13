#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Vec3f.hpp"
#include "BaseGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Vec3f,
    (vrml_proc::parser::float32_t, x)
    (vrml_proc::parser::float32_t, y)
    (vrml_proc::parser::float32_t, z)
)

namespace vrml_proc {
    namespace parser {

        template<typename Iterator, typename Skipper>
        struct Vec3fGrammar
            : public boost::spirit::qi::grammar<Iterator, Vec3f(), Skipper>,
            public BaseGrammar<Iterator, Vec3f(), Skipper> {

        public:
            Vec3fGrammar() : Vec3fGrammar::base_type(this->m_start) {
                this->m_start = boost::spirit::qi::float_ >> boost::spirit::qi::float_ >> boost::spirit::qi::float_;

                BOOST_SPIRIT_DEBUG_NODE(this->m_start);
            }
        };
    }
}
