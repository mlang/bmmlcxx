#if !defined(BMML_SPIRIT_X3_HPP)
#define BMML_SPIRIT_X3_HPP

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#include <bmml.hxx>

namespace bmml {
  namespace spirit {
    inline namespace x3 {
      template<typename T>
      struct dynamic_pointer_parser : boost::spirit::x3::parser<
        dynamic_pointer_parser<T>
      > {
        using attribute_type = std::shared_ptr<T>;
        template < typename Iterator, typename Context, typename RContext
                 , typename Attribute>
        bool parse( Iterator& first, Iterator const& last
                  , Context const& context, RContext& rcontext
                  , Attribute& attr) const {
          if (auto val = std::dynamic_pointer_cast<T>(*first)) {
            boost::spirit::x3::traits::move_to(val, attr);
            ++first;

            return true;
          }

          return false;
        }
      };

      template <typename T> dynamic_pointer_parser<T> shared_ptr = {};
      inline auto with_id() { return shared_ptr<bmml::with_id>; }
      inline auto generic_text() { return shared_ptr<bmml::generic_text>; }
      inline auto newline() { return shared_ptr<bmml::newline>; }
      inline auto score() { return shared_ptr<bmml::score>; }
      inline auto space() { return shared_ptr<bmml::space>; }
    }
  }
}

#endif // !defined(BMML_SPIRIT_X3_HPP)
