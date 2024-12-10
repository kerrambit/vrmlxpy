#pragma once

#include <string>

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/include/qi.hpp>

namespace vrml_proc {
    namespace parser {

        // Inspired by https://stackoverflow.com/questions/44529864/how-to-skip-line-block-nested-block-comments-in-boost-spirit.
        struct CommentSkipper : boost::spirit::qi::grammar<std::string::iterator> {

            CommentSkipper()
                : CommentSkipper::base_type(start) {

                single_line_comment = '#' >> *(boost::spirit::qi::char_ - boost::spirit::qi::eol) >> (boost::spirit::qi::eol | boost::spirit::qi::eoi);
                start = boost::spirit::ascii::space | single_line_comment;

                BOOST_SPIRIT_DEBUG_NODE(single_line_comment);
                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            boost::spirit::qi::rule<std::string::iterator> single_line_comment;
            boost::spirit::qi::rule<std::string::iterator> start;
        };
    }
}
