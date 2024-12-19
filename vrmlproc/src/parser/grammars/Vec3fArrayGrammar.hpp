#pragma once

#include <memory>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Vec3fGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(
    vrml_proc::parser::Vec3fArray,
    (std::vector<vrml_proc::parser::Vec3f>, vectors)
)

namespace vrml_proc {
    namespace parser {

        template <typename Iterator, typename Skipper>
        struct Vec3fArrayGrammar : boost::spirit::qi::grammar<Iterator, Vec3fArray(), Skipper> {

            Vec3fArrayGrammar() : Vec3fArrayGrammar::base_type(start) {

                vec3f = std::make_unique<Vec3fGrammar<Iterator, Skipper>>();

                start = boost::spirit::qi::lit('[') >>
                    -((vec3f->start % boost::spirit::qi::lit(',')) >> -boost::spirit::qi::lit(',')) >>
                    boost::spirit::qi::lit(']');

                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            std::unique_ptr<Vec3fGrammar<Iterator, Skipper>> vec3f;
            boost::spirit::qi::rule<Iterator, Vec3fArray(), Skipper> start;
        };
    }
}
