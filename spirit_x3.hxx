#if !defined(BMML_SPIRIT_X3_HPP)
#define BMML_SPIRIT_X3_HPP

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
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
          boost::spirit::x3::skip_over(first, last, context);
          if (first != last)
          if (auto val = std::dynamic_pointer_cast<T>(*first)) {
            boost::spirit::x3::traits::move_to(val, attr);
            ++first;

            return true;
          }

          return false;
        }
      };
      template<typename T, typename Predicate>
      struct dynamic_pointer_parser_with_predicate : boost::spirit::x3::parser<
        dynamic_pointer_parser_with_predicate<T, Predicate>
      > {
        using attribute_type = std::shared_ptr<T>;
        dynamic_pointer_parser_with_predicate(Predicate const& pred) : pred(pred) {}
        template < typename Iterator, typename Context, typename RContext
                 , typename Attribute>
        bool parse( Iterator& first, Iterator const& last
                  , Context const& context, RContext& rcontext
                  , Attribute& attr) const {
          boost::spirit::x3::skip_over(first, last, context);
          if (first != last)
          if (auto val = std::dynamic_pointer_cast<T>(*first)) {
            auto iter = val->begin();
            if (pred(*val)) {
              boost::spirit::x3::traits::move_to(val, attr);
              ++first;

              return true;
            }
          }

          return false;
        }
        Predicate pred;
      };

      template<typename T> dynamic_pointer_parser<T> shared_ptr() {
        return {};
      }
      template<typename T, typename Predicate>
      dynamic_pointer_parser_with_predicate<T, Predicate>
      shared_ptr(Predicate&& pred) {
        return {std::forward<Predicate>(pred)};
      }
      inline auto element() { return shared_ptr<bmml::dom::element>(); }
      inline auto with_id() { return shared_ptr<bmml::with_id>(); }
      inline auto with_id(std::string const& id) {
        return shared_ptr<bmml::with_id>(
          [id](bmml::with_id const& obj) {
            return obj.id() == id;
          }
        );
      }
      inline auto barline() { return shared_ptr<bmml::barline>(); }
      inline auto generic_text() { return shared_ptr<bmml::generic_text>(); }
      inline auto inaccord() { return shared_ptr<bmml::inaccord>(); }
      inline auto inaccord(bmml::inaccord_t value) {
        return shared_ptr<bmml::inaccord>(
          [value](bmml::inaccord const& obj) {
            return obj.value() == value;
          }
        );
      }
      inline auto music_hyphen() { return shared_ptr<bmml::music_hyphen>(); }
      inline auto newline() { return shared_ptr<bmml::newline>(); }
      inline auto part_name() { return shared_ptr<bmml::part_name>(); }
      inline auto score() { return shared_ptr<bmml::score>(); }
      inline auto separator() { return shared_ptr<bmml::separator>(); }
      inline auto space() { return shared_ptr<bmml::space>(); }
    }
  }
}

#endif // !defined(BMML_SPIRIT_X3_HPP)
