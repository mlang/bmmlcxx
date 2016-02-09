#include "bmml.hxx"

#include <iostream>
#include <xml/parser>
#include <xml/serializer>

using namespace std;
using namespace xml;

bmml::dom::factory::map_type *bmml::dom::factory::default_map{};

namespace {

bool whitespace (const string& s) {
  for (char c : s) {
    if (c != 0x20 && c != 0x0A && c != 0x0D && c != 0x09) return false;
  }

  return true;
}

} // namespace

bmml::dom::element::element(parser& p, bool start_end) {
  parse(p, start_end);
}

void bmml::dom::element::parse(parser& p, bool start_end) {
  if (start_end) p.next_expect(parser::start_element);

  name_ = p.qname();
  for (auto &&a : p.attribute_map()) attributes_[a.first] = a.second.value;

  // Parse content (nested elements or text).
  //
  while (p.peek () != parser::end_element) {
    switch (p.next()) {
    case parser::start_element: {
      if (!text_.empty()) {
        if (!whitespace(text_)) throw parsing(p, "element in simple content");

        text_.clear();
      }

      elements_.push_back(dom::factory::make(p));
      p.next_expect(parser::end_element, elements_.back()->name());

      break;
    }
    case parser::characters: {
      if (!elements_.empty ()) {
        if (!whitespace (p.value ()))
          throw parsing (p, "characters in complex content");

        break; // Ignore whitespaces.
      }

      text_ += p.value ();
      break;
    }
    default:
      break; // Ignore any other events.
    }
  }

  if (start_end) p.next_expect(parser::end_element, name_);
}

void bmml::dom::element::serialize(serializer& s, bool start_end) const {
  if (start_end) s.start_element(name_);
  for (auto &&a : attributes_) s.attribute (a.first, a.second);

  // Serialize content (nested elements or text).
  //
  if (!elements_.empty ()) {
    for (auto &&e : elements_) e->serialize(s);
  } else if (!text_.empty()) s.characters(text_);

  if (start_end) s.end_element ();
}

REGISTER_DEFINITION(rest_type, qname("rest_type"), content::simple);
REGISTER_DEFINITION(part, qname("part"), content::complex);
REGISTER_DEFINITION(score, qname("score"), content::complex);

