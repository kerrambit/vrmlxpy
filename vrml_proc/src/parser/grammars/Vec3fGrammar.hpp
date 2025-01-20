#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/operator.hpp>
#include <iostream>
#include <string>
#include <iterator>

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

                float32 = boost::spirit::qi::float_;
                this->m_start = float32 > float32 > float32;

                BOOST_SPIRIT_DEBUG_NODE(this->m_start);
                BOOST_SPIRIT_DEBUG_NODE(float32);
                    
                /**
                 * _1 is Iterator at the beginning of the string
                 * _2 is Iterator of the end
                 * _3 is Iterator of the error position
                 * _4 is info object containing expected rule
                 */
                boost::spirit::qi::on_error<boost::spirit::qi::fail>(
                    this->m_start,
                    std::cout << boost::phoenix::val("Error! Expecting: ")
                    << boost::spirit::qi::_4 
                    << boost::phoenix::val(". Error position")
                    << boost::phoenix::bind(&Vec3fGrammar::ConstructErrorPositionString, this, boost::spirit::qi::_1, boost::spirit::qi::_3, boost::spirit::qi::_2, boost::spirit::qi::_4, 16)
                    << std::endl
                );
            }

            enum class ConstructErrorPositionStringResult {
                Tail,
                Truncated,
                NoChange
            };

            std::string ConstructErrorPositionString(const Iterator& begin, const Iterator& errorPosition, const Iterator& end, const boost::spirit::info& expectedRule, size_t maxLength) {
                
                this->SetError(true);

                if (errorPosition == end) {
                    return " is at the end of the input string.";
                    // distance = 0;
                    // return ConstructErrorPositionStringResult::Tail;
                }
                size_t length = std::distance(errorPosition, end);
                // distance = length;

                if (length > maxLength) {
                    return ": '" + std::string(errorPosition, errorPosition + maxLength) + "...'";
                    // return ConstructErrorPositionStringResult::Truncated;
                }
                return "'" + std::string(errorPosition, end) + "'";
                // return ConstructErrorPositionStringResult::NoChange;
            }

        private:
            boost::spirit::qi::rule<Iterator, float, Skipper> float32;
        };
    }
}
