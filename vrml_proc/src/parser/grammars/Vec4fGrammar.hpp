#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Vec4f.hpp"
#include "BaseGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::Vec4f,
                          (vrml_proc::parser::float32_t, x)(vrml_proc::parser::float32_t,
                                                            y)(vrml_proc::parser::float32_t,
                                                               z)(vrml_proc::parser::float32_t, w))

namespace vrml_proc {
  namespace parser {

    template <typename Iterator, typename Skipper>
    struct Vec4fGrammar : public boost::spirit::qi::grammar<Iterator, Vec4f(), Skipper>,
                          public BaseGrammar<Iterator, Vec4f(), Skipper> {
      Vec4fGrammar() : Vec4fGrammar::base_type(this->m_start) {
        this->m_start = boost::spirit::qi::float_ >> boost::spirit::qi::float_ >> boost::spirit::qi::float_ >>
                        boost::spirit::qi::float_;

        BOOST_SPIRIT_DEBUG_NODE(this->m_start);
      }
    };
  }  // namespace parser
}  // namespace vrml_proc
