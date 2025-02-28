#pragma once

namespace vrml_proc {
  namespace parser {

    template <typename Iterator, typename ReturnType, typename Skipper>
    class BaseGrammar {
     public:
      virtual ~BaseGrammar() = default;
      boost::spirit::qi::rule<Iterator, ReturnType, Skipper> const& GetStartRule() const { return m_start; }

     protected:
      BaseGrammar() = default;
      boost::spirit::qi::rule<Iterator, ReturnType, Skipper> m_start;
      bool HasError() const { return m_hasError; }
      void SetError(bool value) { m_hasError = value; }

     private:
      bool m_hasError = false;
      BaseGrammar(const BaseGrammar&) = delete;
      BaseGrammar& operator=(const BaseGrammar&) = delete;
      BaseGrammar(BaseGrammar&&) = delete;
      BaseGrammar& operator=(BaseGrammar&&) = delete;
    };
  }  // namespace parser
}  // namespace vrml_proc
