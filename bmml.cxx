#include "bmml.hxx"

#include <iostream>
#include <xml/parser>
#include <xml/serializer>

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace xml;
using bmml::optional;

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

  tag_name_ = p.qname();
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
      p.next_expect(parser::end_element, elements_.back()->tag_name());

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

  if (start_end) p.next_expect(parser::end_element, tag_name_);
}

void bmml::dom::element::serialize(serializer& s, bool start_end) const {
  if (start_end) s.start_element(tag_name_);
  for (auto &&a : attributes_) s.attribute (a.first, a.second);

  // Serialize content (nested elements or text).
  //
  if (!elements_.empty ()) {
    for (auto &&e : elements_) e->serialize(s);
  } else if (!text_.empty()) s.characters(text_);

  if (start_end) s.end_element ();
}

REGISTER_DEFINITION(abbr_name, qname("abbr_name"), content::simple);

std::string bmml::abbr_name::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::abbr_name::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::abbr_name::value() const {
  static const qname attr{"value"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::abbr_name::value(optional<std::string> opt_value) {
  static const qname attr{"value"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(accidental, qname("accidental"), content::simple);

std::string bmml::accidental::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::accidental::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::accidental::editorial() const {
  static const qname attr{"editorial"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::accidental::editorial(optional<std::string> opt_value) {
  static const qname attr{"editorial"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::accidental::step() const {
  static const qname attr{"step"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::accidental::step(optional<std::string> opt_value) {
  static const qname attr{"step"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<bmml::above_below> bmml::accidental::placement() const {
  auto iter = attributes().find(qname{"placement"});

  if (iter != attributes().end()) {
         if (iter->second == "above") return { above_below::above };
    else if (iter->second == "below") return { above_below::below };
  }

  return {};
}

void bmml::accidental::placement(optional<bmml::above_below> opt_value) {
  if (opt_value) {
    switch (*opt_value) {
    case bmml::above_below::above:
      attributes()[qname{"placement"}] = "above";
      break;

    case bmml::above_below::below:
      attributes()[qname{"placement"}] = "below";
      break;

    default:
      throw illegal_enumeration{};
    }
  } else {
    attributes().erase(qname{"placement"});
  }
}

optional<std::string> bmml::accidental::cancel() const {
  static const qname attr{"cancel"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::accidental::cancel(optional<std::string> opt_value) {
  static const qname attr{"cancel"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::accidental::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::accidental::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(accordion_register, qname("accordion_register"), content::simple);

std::string bmml::accordion_register::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::accordion_register::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::accordion_register::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::accordion_register::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(accordion_row, qname("accordion_row"), content::simple);

std::string bmml::accordion_row::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::accordion_row::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::accordion_row::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::accordion_row::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(alteration, qname("alteration"), content::simple);

REGISTER_DEFINITION(alternation, qname("alternation"), content::simple);

std::string bmml::alternation::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::alternation::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::alternation::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::alternation::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(alternation_ref, qname("alternation_ref"), content::empty);

std::string bmml::alternation_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::alternation_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::start_stop bmml::alternation_ref::type() const {
  auto iter = attributes().find(qname{"type"});

  if (iter != attributes().end()) {
         if (iter->second == "start") return start_stop::start;
    else if (iter->second == "stop") return start_stop::stop;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::alternation_ref::type(bmml::start_stop value) {
  switch (value) {
  case bmml::start_stop::start:
    attributes()[qname{"type"}] = "start";
    break;

  case bmml::start_stop::stop:
    attributes()[qname{"type"}] = "stop";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(appoggiatura_ref, qname("appoggiatura_ref"), content::empty);

std::string bmml::appoggiatura_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::appoggiatura_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(barline, qname("barline"), content::complex);

std::string bmml::barline::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::barline::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bmml::left_middle_right> bmml::barline::type() const {
  auto iter = attributes().find(qname{"type"});

  if (iter != attributes().end()) {
         if (iter->second == "left") return { left_middle_right::left };
    else if (iter->second == "middle") return { left_middle_right::middle };
    else if (iter->second == "right") return { left_middle_right::right };
  }

  return {};
}

void bmml::barline::type(optional<bmml::left_middle_right> opt_value) {
  if (opt_value) {
    switch (*opt_value) {
    case bmml::left_middle_right::left:
      attributes()[qname{"type"}] = "left";
      break;

    case bmml::left_middle_right::middle:
      attributes()[qname{"type"}] = "middle";
      break;

    case bmml::left_middle_right::right:
      attributes()[qname{"type"}] = "right";
      break;

    default:
      throw illegal_enumeration{};
    }
  } else {
    attributes().erase(qname{"type"});
  }
}

REGISTER_DEFINITION(barline_type, qname("barline_type"), content::simple);

std::string bmml::barline_type::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::barline_type::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::barline_type::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::barline_type::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(barre, qname("barre"), content::simple);

std::string bmml::barre::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::barre::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::full_half_vertical bmml::barre::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "full") return full_half_vertical::full;
    else if (iter->second == "half") return full_half_vertical::half;
    else if (iter->second == "vertical") return full_half_vertical::vertical;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::barre::value(bmml::full_half_vertical value) {
  switch (value) {
  case bmml::full_half_vertical::full:
    attributes()[qname{"value"}] = "full";
    break;

  case bmml::full_half_vertical::half:
    attributes()[qname{"value"}] = "half";
    break;

  case bmml::full_half_vertical::vertical:
    attributes()[qname{"value"}] = "vertical";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(bow, qname("bow"), content::simple);

std::string bmml::bow::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::bow::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::bow::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::bow::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

bmml::up_down bmml::bow::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "up") return up_down::up;
    else if (iter->second == "down") return up_down::down;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::bow::value(bmml::up_down value) {
  switch (value) {
  case bmml::up_down::up:
    attributes()[qname{"value"}] = "up";
    break;

  case bmml::up_down::down:
    attributes()[qname{"value"}] = "down";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(breath, qname("breath"), content::simple);

std::string bmml::breath::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::breath::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::full_half_caesura bmml::breath::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "full") return full_half_caesura::full;
    else if (iter->second == "half") return full_half_caesura::half;
    else if (iter->second == "caesura") return full_half_caesura::caesura;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::breath::value(bmml::full_half_caesura value) {
  switch (value) {
  case bmml::full_half_caesura::full:
    attributes()[qname{"value"}] = "full";
    break;

  case bmml::full_half_caesura::half:
    attributes()[qname{"value"}] = "half";
    break;

  case bmml::full_half_caesura::caesura:
    attributes()[qname{"value"}] = "caesura";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(chord, qname("chord"), content::complex);

std::string bmml::chord::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::chord::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(chord_data, qname("chord_data"), content::complex);

REGISTER_DEFINITION(chord_prefix, qname("chord_prefix"), content::simple);

std::string bmml::chord_prefix::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::chord_prefix::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(chord_type, qname("chord_type"), content::simple);

std::string bmml::chord_type::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::chord_type::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::chord_type::value() const {
  static const qname attr{"value"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::chord_type::value(optional<std::string> opt_value) {
  static const qname attr{"value"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::chord_type::dot() const {
  static const qname attr{"dot"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::chord_type::dot(optional<std::string> opt_value) {
  static const qname attr{"dot"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::chord_type::duration() const {
  static const qname attr{"duration"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::chord_type::duration(optional<std::string> opt_value) {
  static const qname attr{"duration"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::chord_type::bass() const {
  static const qname attr{"bass"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::chord_type::bass(optional<std::string> opt_value) {
  static const qname attr{"bass"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::chord_type::notes() const {
  auto iter = attributes().find(qname{"notes"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::chord_type::notes(std::string const& value) {
  attributes()[qname{"notes"}] = value;
}

std::string bmml::chord_type::root() const {
  auto iter = attributes().find(qname{"root"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::chord_type::root(std::string const& value) {
  attributes()[qname{"root"}] = value;
}

REGISTER_DEFINITION(clef, qname("clef"), content::simple);

std::string bmml::clef::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::clef::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::clef::cross_staff() const {
  static const qname attr{"cross_staff"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::clef::cross_staff(optional<bool> opt_value) {
  static const qname attr{"cross_staff"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

optional<bmml::above_below> bmml::clef::eight() const {
  auto iter = attributes().find(qname{"eight"});

  if (iter != attributes().end()) {
         if (iter->second == "above") return { above_below::above };
    else if (iter->second == "below") return { above_below::below };
  }

  return {};
}

void bmml::clef::eight(optional<bmml::above_below> opt_value) {
  if (opt_value) {
    switch (*opt_value) {
    case bmml::above_below::above:
      attributes()[qname{"eight"}] = "above";
      break;

    case bmml::above_below::below:
      attributes()[qname{"eight"}] = "below";
      break;

    default:
      throw illegal_enumeration{};
    }
  } else {
    attributes().erase(qname{"eight"});
  }
}

optional<std::string> bmml::clef::line() const {
  static const qname attr{"line"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::clef::line(optional<std::string> opt_value) {
  static const qname attr{"line"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::clef::name() const {
  auto iter = attributes().find(qname{"name"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::clef::name(std::string const& value) {
  attributes()[qname{"name"}] = value;
}

REGISTER_DEFINITION(coda, qname("coda"), content::simple);

std::string bmml::coda::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::coda::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(dot, qname("dot"), content::simple);

std::string bmml::dot::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::dot::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::dot::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::dot::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(duration, qname("duration"), content::simple);

bmml::duration::operator int() const {
  return boost::lexical_cast<int>(text());
}


REGISTER_DEFINITION(dynamic, qname("dynamic"), content::simple);

std::string bmml::dynamic::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::dynamic::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::dynamic::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::dynamic::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(editorial_mark, qname("editorial_mark"), content::simple);

std::string bmml::editorial_mark::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::editorial_mark::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(ending, qname("ending"), content::simple);

std::string bmml::ending::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::ending::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::ending::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::ending::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(family, qname("family"), content::simple);

REGISTER_DEFINITION(fermata, qname("fermata"), content::simple);

std::string bmml::fermata::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::fermata::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::fermata::shape() const {
  static const qname attr{"shape"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::fermata::shape(optional<std::string> opt_value) {
  static const qname attr{"shape"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(fingering, qname("fingering"), content::simple);

std::string bmml::fingering::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::fingering::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::fingering::alternative() const {
  static const qname attr{"alternative"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::fingering::alternative(optional<std::string> opt_value) {
  static const qname attr{"alternative"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::fingering::value() const {
  static const qname attr{"value"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::fingering::value(optional<std::string> opt_value) {
  static const qname attr{"value"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(foot_crossing, qname("foot_crossing"), content::simple);

std::string bmml::foot_crossing::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::foot_crossing::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(generic_text, qname("generic_text"), content::simple);

std::string bmml::generic_text::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::generic_text::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::generic_text::lang() const {
  static const qname attr{"lang"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::generic_text::lang(optional<std::string> opt_value) {
  static const qname attr{"lang"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::generic_text::value() const {
  static const qname attr{"value"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::generic_text::value(optional<std::string> opt_value) {
  static const qname attr{"value"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::generic_text::type() const {
  static const qname attr{"type"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::generic_text::type(optional<std::string> opt_value) {
  static const qname attr{"type"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(hand, qname("hand"), content::simple);

std::string bmml::hand::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::hand::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bmml::up_down> bmml::hand::chord_dir() const {
  auto iter = attributes().find(qname{"chord_dir"});

  if (iter != attributes().end()) {
         if (iter->second == "up") return { up_down::up };
    else if (iter->second == "down") return { up_down::down };
  }

  return {};
}

void bmml::hand::chord_dir(optional<bmml::up_down> opt_value) {
  if (opt_value) {
    switch (*opt_value) {
    case bmml::up_down::up:
      attributes()[qname{"chord_dir"}] = "up";
      break;

    case bmml::up_down::down:
      attributes()[qname{"chord_dir"}] = "down";
      break;

    default:
      throw illegal_enumeration{};
    }
  } else {
    attributes().erase(qname{"chord_dir"});
  }
}

bmml::left_right bmml::hand::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "left") return left_right::left;
    else if (iter->second == "right") return left_right::right;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::hand::value(bmml::left_right value) {
  switch (value) {
  case bmml::left_right::left:
    attributes()[qname{"value"}] = "left";
    break;

  case bmml::left_right::right:
    attributes()[qname{"value"}] = "right";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(harmonic, qname("harmonic"), content::simple);

std::string bmml::harmonic::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::harmonic::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::harmonic::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::harmonic::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

bmml::natural_artificial bmml::harmonic::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "natural") return natural_artificial::natural;
    else if (iter->second == "artificial") return natural_artificial::artificial;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::harmonic::value(bmml::natural_artificial value) {
  switch (value) {
  case bmml::natural_artificial::natural:
    attributes()[qname{"value"}] = "natural";
    break;

  case bmml::natural_artificial::artificial:
    attributes()[qname{"value"}] = "artificial";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(inaccord, qname("inaccord"), content::simple);

std::string bmml::inaccord::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::inaccord::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::inaccord::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::inaccord::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(interval, qname("interval"), content::complex);

std::string bmml::interval::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::interval::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(interval_data, qname("interval_data"), content::complex);

REGISTER_DEFINITION(interval_ref, qname("interval_ref"), content::complex);

std::string bmml::interval_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::interval_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(interval_type, qname("interval_type"), content::simple);

std::string bmml::interval_type::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::interval_type::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::interval_type::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::interval_type::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::interval_type::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::interval_type::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(intervals, qname("intervals"), content::complex);

REGISTER_DEFINITION(key_signature, qname("key_signature"), content::simple);

std::string bmml::key_signature::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::key_signature::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::key_signature::cancel() const {
  static const qname attr{"cancel"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::key_signature::cancel(optional<std::string> opt_value) {
  static const qname attr{"cancel"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::key_signature::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::key_signature::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(line_of_continuation, qname("line_of_continuation"), content::simple);

std::string bmml::line_of_continuation::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::line_of_continuation::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::start_stop bmml::line_of_continuation::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "start") return start_stop::start;
    else if (iter->second == "stop") return start_stop::stop;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::line_of_continuation::value(bmml::start_stop value) {
  switch (value) {
  case bmml::start_stop::start:
    attributes()[qname{"value"}] = "start";
    break;

  case bmml::start_stop::stop:
    attributes()[qname{"value"}] = "stop";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(lyric, qname("lyric"), content::complex);

std::string bmml::lyric::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::lyric::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::lyric::lang() const {
  static const qname attr{"lang"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::lyric::lang(optional<std::string> opt_value) {
  static const qname attr{"lang"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(lyric_prefix, qname("lyric_prefix"), content::simple);

std::string bmml::lyric_prefix::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::lyric_prefix::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(lyric_repeat, qname("lyric_repeat"), content::complex);

std::string bmml::lyric_repeat::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::lyric_repeat::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(lyric_repetition, qname("lyric_repetition"), content::simple);

std::string bmml::lyric_repetition::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::lyric_repetition::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::lyric_repetition::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::lyric_repetition::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(lyrics, qname("lyrics"), content::complex);

REGISTER_DEFINITION(merged_text, qname("merged_text"), content::simple);

std::string bmml::merged_text::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::merged_text::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::merged_text::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::merged_text::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(meta_data, qname("meta_data"), content::mixed);

REGISTER_DEFINITION(metronome, qname("metronome"), content::complex);

std::string bmml::metronome::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::metronome::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(metronome_equal, qname("metronome_equal"), content::simple);

std::string bmml::metronome_equal::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::metronome_equal::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(metronome_note_type, qname("metronome_note_type"), content::simple);

std::string bmml::metronome_note_type::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::metronome_note_type::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::metronome_note_type::dot() const {
  static const qname attr{"dot"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::metronome_note_type::dot(optional<bool> opt_value) {
  static const qname attr{"dot"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

bmml::ambiguous_value bmml::metronome_note_type::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "8th_or_128th") return ambiguous_value::eighth_or_128th;
    else if (iter->second == "quarter_or_64th") return ambiguous_value::quarter_or_64th;
    else if (iter->second == "half_or_32nd") return ambiguous_value::half_or_32nd;
    else if (iter->second == "whole_or_16th") return ambiguous_value::whole_or_16th;
    else if (iter->second == "brevis") return ambiguous_value::brevis;
    else if (iter->second == "longa") return ambiguous_value::longa;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::metronome_note_type::value(bmml::ambiguous_value value) {
  switch (value) {
  case bmml::ambiguous_value::eighth_or_128th:
    attributes()[qname{"value"}] = "8th_or_128th";
    break;

  case bmml::ambiguous_value::quarter_or_64th:
    attributes()[qname{"value"}] = "quarter_or_64th";
    break;

  case bmml::ambiguous_value::half_or_32nd:
    attributes()[qname{"value"}] = "half_or_32nd";
    break;

  case bmml::ambiguous_value::whole_or_16th:
    attributes()[qname{"value"}] = "whole_or_16th";
    break;

  case bmml::ambiguous_value::brevis:
    attributes()[qname{"value"}] = "brevis";
    break;

  case bmml::ambiguous_value::longa:
    attributes()[qname{"value"}] = "longa";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(metronome_value, qname("metronome_value"), content::simple);

std::string bmml::metronome_value::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::metronome_value::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::metronome_value::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::metronome_value::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(midi_instrument, qname("midi_instrument"), content::empty);

std::string bmml::midi_instrument::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::midi_instrument::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(midi_metronome, qname("midi_metronome"), content::empty);

std::string bmml::midi_metronome::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::midi_metronome::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(multimeasure, qname("multimeasure"), content::simple);

REGISTER_DEFINITION(music_hyphen, qname("music_hyphen"), content::simple);

std::string bmml::music_hyphen::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::music_hyphen::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(name, qname("name"), content::simple);

std::string bmml::name::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::name::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::name::value() const {
  static const qname attr{"value"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::name::value(optional<std::string> opt_value) {
  static const qname attr{"value"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(newline, qname("newline"), content::simple);

std::string bmml::newline::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::newline::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(note, qname("note"), content::complex);

std::string bmml::note::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::note::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(note_data, qname("note_data"), content::complex);

REGISTER_DEFINITION(note_ref, qname("note_ref"), content::empty);

std::string bmml::note_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::note_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(note_type, qname("note_type"), content::simple);

std::string bmml::note_type::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::note_type::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::ambiguous_value bmml::note_type::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "8th_or_128th") return ambiguous_value::eighth_or_128th;
    else if (iter->second == "quarter_or_64th") return ambiguous_value::quarter_or_64th;
    else if (iter->second == "half_or_32nd") return ambiguous_value::half_or_32nd;
    else if (iter->second == "whole_or_16th") return ambiguous_value::whole_or_16th;
    else if (iter->second == "brevis") return ambiguous_value::brevis;
    else if (iter->second == "longa") return ambiguous_value::longa;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::note_type::value(bmml::ambiguous_value value) {
  switch (value) {
  case bmml::ambiguous_value::eighth_or_128th:
    attributes()[qname{"value"}] = "8th_or_128th";
    break;

  case bmml::ambiguous_value::quarter_or_64th:
    attributes()[qname{"value"}] = "quarter_or_64th";
    break;

  case bmml::ambiguous_value::half_or_32nd:
    attributes()[qname{"value"}] = "half_or_32nd";
    break;

  case bmml::ambiguous_value::whole_or_16th:
    attributes()[qname{"value"}] = "whole_or_16th";
    break;

  case bmml::ambiguous_value::brevis:
    attributes()[qname{"value"}] = "brevis";
    break;

  case bmml::ambiguous_value::longa:
    attributes()[qname{"value"}] = "longa";
    break;

  default:
    throw illegal_enumeration{};
  }
}

bmml::diatonic_step bmml::note_type::name() const {
  auto iter = attributes().find(qname{"name"});

  if (iter != attributes().end()) {
         if (iter->second == "A") return diatonic_step::A;
    else if (iter->second == "B") return diatonic_step::B;
    else if (iter->second == "C") return diatonic_step::C;
    else if (iter->second == "D") return diatonic_step::D;
    else if (iter->second == "E") return diatonic_step::E;
    else if (iter->second == "F") return diatonic_step::F;
    else if (iter->second == "G") return diatonic_step::G;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::note_type::name(bmml::diatonic_step value) {
  switch (value) {
  case bmml::diatonic_step::A:
    attributes()[qname{"name"}] = "A";
    break;

  case bmml::diatonic_step::B:
    attributes()[qname{"name"}] = "B";
    break;

  case bmml::diatonic_step::C:
    attributes()[qname{"name"}] = "C";
    break;

  case bmml::diatonic_step::D:
    attributes()[qname{"name"}] = "D";
    break;

  case bmml::diatonic_step::E:
    attributes()[qname{"name"}] = "E";
    break;

  case bmml::diatonic_step::F:
    attributes()[qname{"name"}] = "F";
    break;

  case bmml::diatonic_step::G:
    attributes()[qname{"name"}] = "G";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(nuance, qname("nuance"), content::simple);

std::string bmml::nuance::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::nuance::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::nuance::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::nuance::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::nuance::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::nuance::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(nuance_ref, qname("nuance_ref"), content::empty);

std::string bmml::nuance_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::nuance_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(nuances, qname("nuances"), content::complex);

REGISTER_DEFINITION(number, qname("number"), content::simple);

std::string bmml::number::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::number::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::number::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::number::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(octave, qname("octave"), content::simple);

std::string bmml::octave::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::octave::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::octave::position() const {
  static const qname attr{"position"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::octave::position(optional<std::string> opt_value) {
  static const qname attr{"position"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::octave::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::octave::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(organ_pedal, qname("organ_pedal"), content::simple);

std::string bmml::organ_pedal::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::organ_pedal::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(ornament, qname("ornament"), content::complex);

std::string bmml::ornament::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::ornament::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

vector<shared_ptr<bmml::accidental>> bmml::ornament::accidentals() const {
  return find_elements<bmml::accidental>();
}

shared_ptr<bmml::ornament_type> bmml::ornament::ornament_type() const {
  return find_element<bmml::ornament_type>();
}


REGISTER_DEFINITION(ornament_type, qname("ornament_type"), content::simple);

std::string bmml::ornament_type::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::ornament_type::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::ornament_type::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::ornament_type::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::ornament_type::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::ornament_type::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(part, qname("part"), content::complex);

std::string bmml::part::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::part::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(part_data, qname("part_data"), content::complex);

std::string bmml::part_data::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::part_data::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::part_data::transpose() const {
  static const qname attr{"transpose"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::part_data::transpose(optional<std::string> opt_value) {
  static const qname attr{"transpose"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::part_data::lang() const {
  static const qname attr{"lang"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::part_data::lang(optional<std::string> opt_value) {
  static const qname attr{"lang"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<bmml::up_down> bmml::part_data::chord_dir() const {
  auto iter = attributes().find(qname{"chord_dir"});

  if (iter != attributes().end()) {
         if (iter->second == "up") return { up_down::up };
    else if (iter->second == "down") return { up_down::down };
  }

  return {};
}

void bmml::part_data::chord_dir(optional<bmml::up_down> opt_value) {
  if (opt_value) {
    switch (*opt_value) {
    case bmml::up_down::up:
      attributes()[qname{"chord_dir"}] = "up";
      break;

    case bmml::up_down::down:
      attributes()[qname{"chord_dir"}] = "down";
      break;

    default:
      throw illegal_enumeration{};
    }
  } else {
    attributes().erase(qname{"chord_dir"});
  }
}

REGISTER_DEFINITION(part_list, qname("part_list"), content::complex);

REGISTER_DEFINITION(part_name, qname("part_name"), content::simple);

std::string bmml::part_name::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::part_name::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(pedal, qname("pedal"), content::simple);

std::string bmml::pedal::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::pedal::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::pedal::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::pedal::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(pitch, qname("pitch"), content::simple);

bmml::pitch::operator int() const {
  return boost::lexical_cast<int>(text());
}


REGISTER_DEFINITION(pizzicato, qname("pizzicato"), content::simple);

std::string bmml::pizzicato::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::pizzicato::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::pizzicato::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::pizzicato::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

bmml::left_right bmml::pizzicato::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "left") return left_right::left;
    else if (iter->second == "right") return left_right::right;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::pizzicato::value(bmml::left_right value) {
  switch (value) {
  case bmml::left_right::left:
    attributes()[qname{"value"}] = "left";
    break;

  case bmml::left_right::right:
    attributes()[qname{"value"}] = "right";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(rasgueado, qname("rasgueado"), content::simple);

std::string bmml::rasgueado::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::rasgueado::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::up_down bmml::rasgueado::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "up") return up_down::up;
    else if (iter->second == "down") return up_down::down;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::rasgueado::value(bmml::up_down value) {
  switch (value) {
  case bmml::up_down::up:
    attributes()[qname{"value"}] = "up";
    break;

  case bmml::up_down::down:
    attributes()[qname{"value"}] = "down";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(repeat, qname("repeat"), content::complex);

std::string bmml::repeat::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::repeat::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(repeat_data, qname("repeat_data"), content::complex);

REGISTER_DEFINITION(repeat_ref, qname("repeat_ref"), content::complex);

std::string bmml::repeat_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::repeat_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(repeats, qname("repeats"), content::complex);

REGISTER_DEFINITION(repetition, qname("repetition"), content::simple);

std::string bmml::repetition::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::repetition::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::repetition::separation() const {
  static const qname attr{"separation"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::repetition::separation(optional<std::string> opt_value) {
  static const qname attr{"separation"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::repetition::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::repetition::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(rest, qname("rest"), content::complex);

std::string bmml::rest::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::rest::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(rest_data, qname("rest_data"), content::complex);

REGISTER_DEFINITION(rest_type, qname("rest_type"), content::simple);

std::string bmml::rest_type::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::rest_type::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::rest_type::multimeasure() const {
  static const qname attr{"multimeasure"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::rest_type::multimeasure(optional<std::string> opt_value) {
  static const qname attr{"multimeasure"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

bmml::ambiguous_value bmml::rest_type::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "8th_or_128th") return ambiguous_value::eighth_or_128th;
    else if (iter->second == "quarter_or_64th") return ambiguous_value::quarter_or_64th;
    else if (iter->second == "half_or_32nd") return ambiguous_value::half_or_32nd;
    else if (iter->second == "whole_or_16th") return ambiguous_value::whole_or_16th;
    else if (iter->second == "brevis") return ambiguous_value::brevis;
    else if (iter->second == "longa") return ambiguous_value::longa;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::rest_type::value(bmml::ambiguous_value value) {
  switch (value) {
  case bmml::ambiguous_value::eighth_or_128th:
    attributes()[qname{"value"}] = "8th_or_128th";
    break;

  case bmml::ambiguous_value::quarter_or_64th:
    attributes()[qname{"value"}] = "quarter_or_64th";
    break;

  case bmml::ambiguous_value::half_or_32nd:
    attributes()[qname{"value"}] = "half_or_32nd";
    break;

  case bmml::ambiguous_value::whole_or_16th:
    attributes()[qname{"value"}] = "whole_or_16th";
    break;

  case bmml::ambiguous_value::brevis:
    attributes()[qname{"value"}] = "brevis";
    break;

  case bmml::ambiguous_value::longa:
    attributes()[qname{"value"}] = "longa";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(rhythmic_group, qname("rhythmic_group"), content::simple);

REGISTER_DEFINITION(right_string_fingering, qname("right_string_fingering"), content::simple);

std::string bmml::right_string_fingering::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::right_string_fingering::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::right_string_fingering::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::right_string_fingering::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(score, qname("score"), content::complex);

std::string bmml::score::version() const {
  auto iter = attributes().find(qname{"version"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::score::version(std::string const& value) {
  attributes()[qname{"version"}] = value;
}

shared_ptr<bmml::score_header> bmml::score::header() const {
  return find_element<bmml::score_header>();
}

shared_ptr<bmml::score_data> bmml::score::data() const {
  return find_element<bmml::score_data>();
}


REGISTER_DEFINITION(score_data, qname("score_data"), content::complex);

REGISTER_DEFINITION(score_header, qname("score_header"), content::complex);

REGISTER_DEFINITION(segno, qname("segno"), content::simple);

std::string bmml::segno::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::segno::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(separator, qname("separator"), content::simple);

std::string bmml::separator::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::separator::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(shift_line, qname("shift_line"), content::simple);

std::string bmml::shift_line::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::shift_line::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::glissando_start_stop bmml::shift_line::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "glissando") return glissando_start_stop::glissando;
    else if (iter->second == "start") return glissando_start_stop::start;
    else if (iter->second == "stop") return glissando_start_stop::stop;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::shift_line::value(bmml::glissando_start_stop value) {
  switch (value) {
  case bmml::glissando_start_stop::glissando:
    attributes()[qname{"value"}] = "glissando";
    break;

  case bmml::glissando_start_stop::start:
    attributes()[qname{"value"}] = "start";
    break;

  case bmml::glissando_start_stop::stop:
    attributes()[qname{"value"}] = "stop";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(slur, qname("slur"), content::simple);

std::string bmml::slur::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::slur::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::slur::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::slur::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(slur_ref, qname("slur_ref"), content::empty);

std::string bmml::slur_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::slur_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::slur_ref::start_ref() const {
  auto iter = attributes().find(qname{"start_ref"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::slur_ref::start_ref(std::string const& value) {
  attributes()[qname{"start_ref"}] = value;
}

bmml::start_stop_continue bmml::slur_ref::type() const {
  auto iter = attributes().find(qname{"type"});

  if (iter != attributes().end()) {
         if (iter->second == "start") return start_stop_continue::start;
    else if (iter->second == "stop") return start_stop_continue::stop;
    else if (iter->second == "continue") return start_stop_continue::continue_;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::slur_ref::type(bmml::start_stop_continue value) {
  switch (value) {
  case bmml::start_stop_continue::start:
    attributes()[qname{"type"}] = "start";
    break;

  case bmml::start_stop_continue::stop:
    attributes()[qname{"type"}] = "stop";
    break;

  case bmml::start_stop_continue::continue_:
    attributes()[qname{"type"}] = "continue";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(slurs, qname("slurs"), content::complex);

REGISTER_DEFINITION(space, qname("space"), content::simple);

std::string bmml::space::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::space::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(stem, qname("stem"), content::complex);

std::string bmml::stem::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::stem::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(stem_data, qname("stem_data"), content::complex);

REGISTER_DEFINITION(stem_type, qname("stem_type"), content::simple);

std::string bmml::stem_type::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::stem_type::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::stem_type::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::stem_type::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(string, qname("string"), content::simple);

std::string bmml::string::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::string::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::string::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::string::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::string::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::string::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(string_fingering, qname("string_fingering"), content::simple);

std::string bmml::string_fingering::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::string_fingering::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bmml::start_stop> bmml::string_fingering::line_of_continuation() const {
  auto iter = attributes().find(qname{"line_of_continuation"});

  if (iter != attributes().end()) {
         if (iter->second == "start") return { start_stop::start };
    else if (iter->second == "stop") return { start_stop::stop };
  }

  return {};
}

void bmml::string_fingering::line_of_continuation(optional<bmml::start_stop> opt_value) {
  if (opt_value) {
    switch (*opt_value) {
    case bmml::start_stop::start:
      attributes()[qname{"line_of_continuation"}] = "start";
      break;

    case bmml::start_stop::stop:
      attributes()[qname{"line_of_continuation"}] = "stop";
      break;

    default:
      throw illegal_enumeration{};
    }
  } else {
    attributes().erase(qname{"line_of_continuation"});
  }
}

std::string bmml::string_fingering::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::string_fingering::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(string_position, qname("string_position"), content::simple);

std::string bmml::string_position::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::string_position::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::string_position::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::string_position::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(stroke, qname("stroke"), content::simple);

std::string bmml::stroke::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::stroke::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::stroke::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::stroke::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

bmml::up_down bmml::stroke::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "up") return up_down::up;
    else if (iter->second == "down") return up_down::down;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::stroke::value(bmml::up_down value) {
  switch (value) {
  case bmml::up_down::up:
    attributes()[qname{"value"}] = "up";
    break;

  case bmml::up_down::down:
    attributes()[qname{"value"}] = "down";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(syllabic_mute, qname("syllabic_mute"), content::simple);

std::string bmml::syllabic_mute::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::syllabic_mute::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::syllabic_mute::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::syllabic_mute::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(syllabic_parenthesis, qname("syllabic_parenthesis"), content::simple);

std::string bmml::syllabic_parenthesis::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::syllabic_parenthesis::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(syllabic_slur, qname("syllabic_slur"), content::simple);

std::string bmml::syllabic_slur::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::syllabic_slur::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::syllabic_slur::verses() const {
  static const qname attr{"verses"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::syllabic_slur::verses(optional<std::string> opt_value) {
  static const qname attr{"verses"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::syllabic_slur::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::syllabic_slur::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(syllabic_text, qname("syllabic_text"), content::simple);

std::string bmml::syllabic_text::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::syllabic_text::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<std::string> bmml::syllabic_text::lang() const {
  static const qname attr{"lang"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::syllabic_text::lang(optional<std::string> opt_value) {
  static const qname attr{"lang"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

optional<std::string> bmml::syllabic_text::value() const {
  static const qname attr{"value"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::syllabic_text::value(optional<std::string> opt_value) {
  static const qname attr{"value"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

REGISTER_DEFINITION(syllable, qname("syllable"), content::complex);

std::string bmml::syllable::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::syllable::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(syllable_mute, qname("syllable_mute"), content::complex);

std::string bmml::syllable_mute::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::syllable_mute::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(syllable_ref, qname("syllable_ref"), content::empty);

std::string bmml::syllable_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::syllable_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(tie, qname("tie"), content::simple);

std::string bmml::tie::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tie::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::tie::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tie::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(tie_ref, qname("tie_ref"), content::empty);

std::string bmml::tie_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tie_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

std::string bmml::tie_ref::start_ref() const {
  auto iter = attributes().find(qname{"start_ref"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tie_ref::start_ref(std::string const& value) {
  attributes()[qname{"start_ref"}] = value;
}

bmml::start_stop bmml::tie_ref::type() const {
  auto iter = attributes().find(qname{"type"});

  if (iter != attributes().end()) {
         if (iter->second == "start") return start_stop::start;
    else if (iter->second == "stop") return start_stop::stop;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::tie_ref::type(bmml::start_stop value) {
  switch (value) {
  case bmml::start_stop::start:
    attributes()[qname{"type"}] = "start";
    break;

  case bmml::start_stop::stop:
    attributes()[qname{"type"}] = "stop";
    break;

  default:
    throw illegal_enumeration{};
  }
}

REGISTER_DEFINITION(ties, qname("ties"), content::complex);

REGISTER_DEFINITION(time_signature, qname("time_signature"), content::simple);

std::string bmml::time_signature::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::time_signature::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::time_signature::single_number() const {
  static const qname attr{"single_number"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::time_signature::single_number(optional<bool> opt_value) {
  static const qname attr{"single_number"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

optional<bool> bmml::time_signature::figure() const {
  static const qname attr{"figure"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::time_signature::figure(optional<bool> opt_value) {
  static const qname attr{"figure"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::time_signature::values() const {
  auto iter = attributes().find(qname{"values"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::time_signature::values(std::string const& value) {
  attributes()[qname{"values"}] = value;
}

REGISTER_DEFINITION(tremolo, qname("tremolo"), content::simple);

std::string bmml::tremolo::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tremolo::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::tremolo::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::tremolo::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::tremolo::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tremolo::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(tremolo_ref, qname("tremolo_ref"), content::empty);

std::string bmml::tremolo_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tremolo_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(tuplet, qname("tuplet"), content::simple);

std::string bmml::tuplet::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tuplet::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

optional<bool> bmml::tuplet::doubled() const {
  static const qname attr{"doubled"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::tuplet::doubled(optional<bool> opt_value) {
  static const qname attr{"doubled"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

std::string bmml::tuplet::value() const {
  auto iter = attributes().find(qname{"value"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tuplet::value(std::string const& value) {
  attributes()[qname{"value"}] = value;
}

REGISTER_DEFINITION(tuplet_ref, qname("tuplet_ref"), content::empty);

std::string bmml::tuplet_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tuplet_ref::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::start_stop_continue bmml::tuplet_ref::type() const {
  auto iter = attributes().find(qname{"type"});

  if (iter != attributes().end()) {
         if (iter->second == "start") return start_stop_continue::start;
    else if (iter->second == "stop") return start_stop_continue::stop;
    else if (iter->second == "continue") return start_stop_continue::continue_;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::tuplet_ref::type(bmml::start_stop_continue value) {
  switch (value) {
  case bmml::start_stop_continue::start:
    attributes()[qname{"type"}] = "start";
    break;

  case bmml::start_stop_continue::stop:
    attributes()[qname{"type"}] = "stop";
    break;

  case bmml::start_stop_continue::continue_:
    attributes()[qname{"type"}] = "continue";
    break;

  default:
    throw illegal_enumeration{};
  }
}

std::string bmml::tuplet_ref::notes() const {
  auto iter = attributes().find(qname{"notes"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::tuplet_ref::notes(std::string const& value) {
  attributes()[qname{"notes"}] = value;
}

REGISTER_DEFINITION(tuplets, qname("tuplets"), content::complex);

REGISTER_DEFINITION(unknown, qname("unknown"), content::simple);

std::string bmml::unknown::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::unknown::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

REGISTER_DEFINITION(value_prefix, qname("value_prefix"), content::simple);

std::string bmml::value_prefix::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::value_prefix::id(std::string const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::value_prefix_t bmml::value_prefix::value() const {
  auto iter = attributes().find(qname{"value"});

  if (iter != attributes().end()) {
         if (iter->second == "separator") return value_prefix_t::separator;
    else if (iter->second == "large") return value_prefix_t::large;
    else if (iter->second == "small") return value_prefix_t::small;
    else if (iter->second == "256th") return value_prefix_t::twohundredfiftysixth;

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::value_prefix::value(bmml::value_prefix_t value) {
  switch (value) {
  case bmml::value_prefix_t::separator:
    attributes()[qname{"value"}] = "separator";
    break;

  case bmml::value_prefix_t::large:
    attributes()[qname{"value"}] = "large";
    break;

  case bmml::value_prefix_t::small:
    attributes()[qname{"value"}] = "small";
    break;

  case bmml::value_prefix_t::twohundredfiftysixth:
    attributes()[qname{"value"}] = "256th";
    break;

  default:
    throw illegal_enumeration{};
  }
}
