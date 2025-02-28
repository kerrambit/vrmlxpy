#pragma once

#include <memory>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Vec3fGrammar.hpp"
#include "BaseGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::Vec3fArray, (std::vector<vrml_proc::parser::Vec3f>, vectors))

namespace vrml_proc {
  namespace parser {

    template <typename Iterator, typename Skipper>
    class Vec3fArrayGrammar : public boost::spirit::qi::grammar<Iterator, Vec3fArray(), Skipper>,
                              public BaseGrammar<Iterator, Vec3fArray(), Skipper> {
     public:
      Vec3fArrayGrammar() : Vec3fArrayGrammar::base_type(this->m_start) {
        m_vec3f = std::make_unique<Vec3fGrammar<Iterator, Skipper>>();

        this->m_start = boost::spirit::qi::lit('[') >>
                        -((m_vec3f->GetStartRule() % boost::spirit::qi::lit(',')) >> -boost::spirit::qi::lit(',')) >>
                        boost::spirit::qi::lit(']');

        BOOST_SPIRIT_DEBUG_NODE(this->m_start);
      }

     private:
      std::unique_ptr<Vec3fGrammar<Iterator, Skipper>> m_vec3f;
    };
  }  // namespace parser
}  // namespace vrml_proc
