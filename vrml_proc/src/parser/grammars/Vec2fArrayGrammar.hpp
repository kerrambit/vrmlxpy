#pragma once

#include <memory>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Vec2f.hpp"
#include "Vec2fArray.hpp"
#include "Vec2fGrammar.hpp"
#include "BaseGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Vec2fArray,
    (std::vector<vrml_proc::parser::Vec2f>, vectors)
)

namespace vrml_proc {
    namespace parser {

        template <typename Iterator, typename Skipper>
        class Vec2fArrayGrammar
            : public boost::spirit::qi::grammar<Iterator, Vec2fArray(), Skipper>,
            public BaseGrammar<Iterator, Vec2fArray(), Skipper> {

        public:
            Vec2fArrayGrammar() : Vec2fArrayGrammar::base_type(this->m_start) {

                m_vec2f = std::make_unique<Vec2fGrammar<Iterator, Skipper>>();

                this->m_start = boost::spirit::qi::lit('[') >>
                    -((m_vec2f->GetStartRule() % boost::spirit::qi::lit(',')) >> -boost::spirit::qi::lit(',')) >>
                    boost::spirit::qi::lit(']');

                BOOST_SPIRIT_DEBUG_NODE(this->m_start);
            }
        private:
            std::unique_ptr<Vec2fGrammar<Iterator, Skipper>> m_vec2f;
        };
    }
}
