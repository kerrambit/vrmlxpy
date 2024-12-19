#pragma once

#include <string>

#include <boost/spirit/include/qi.hpp>

namespace vrml_proc {
    namespace parser {

        struct CommentSkipper : boost::spirit::qi::grammar<std::string::iterator> {

            CommentSkipper()
                : CommentSkipper::base_type(start) {

                // Inspired by https://stackoverflow.com/questions/44529864/how-to-skip-line-block-nested-block-comments-in-boost-spirit.
                singleLineComment = '#' >> *(boost::spirit::qi::char_ - boost::spirit::qi::eol) >> (boost::spirit::qi::eol | boost::spirit::qi::eoi);
                start = boost::spirit::ascii::space | singleLineComment;

                BOOST_SPIRIT_DEBUG_NODE(singleLineComment);
                BOOST_SPIRIT_DEBUG_NODE(start);
            }

            boost::spirit::qi::rule<std::string::iterator> singleLineComment;
            boost::spirit::qi::rule<std::string::iterator> start;
        };
    }
}
