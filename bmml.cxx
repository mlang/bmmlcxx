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
  return attributes().find(qname("id"))->second;
}

void bmml::abbr_name::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(accidental, qname("accidental"), content::simple);

std::string bmml::accidental::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::accidental::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::accidental::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::accidental::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(accordion_register, qname("accordion_register"), content::simple);

std::string bmml::accordion_register::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::accordion_register::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::accordion_register::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::accordion_register::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(accordion_row, qname("accordion_row"), content::simple);

std::string bmml::accordion_row::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::accordion_row::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::accordion_row::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::accordion_row::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(alteration, qname("alteration"), content::simple);
REGISTER_DEFINITION(alternation, qname("alternation"), content::simple);

std::string bmml::alternation::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::alternation::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::alternation::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::alternation::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(alternation_ref, qname("alternation_ref"), content::empty);

std::string bmml::alternation_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::alternation_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::start_stop bmml::alternation_ref::type() const {
  auto iter = attributes().find(qname("type"));
  if (iter != attributes().end()) {
         if (iter->second == "start") return start_stop::start;
    else if (iter->second == "stop") return start_stop::stop;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::alternation_ref::type(bmml::start_stop value) {
  switch (value) {
  case bmml::start_stop::start:
    attributes()[qname("type")] = "start";
    break;
  case bmml::start_stop::stop:
    attributes()[qname("type")] = "stop";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(appoggiatura_ref, qname("appoggiatura_ref"), content::empty);

std::string bmml::appoggiatura_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::appoggiatura_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(barline, qname("barline"), content::complex);

std::string bmml::barline::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::barline::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(barline_type, qname("barline_type"), content::simple);

std::string bmml::barline_type::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::barline_type::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::barline_type::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::barline_type::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(barre, qname("barre"), content::simple);

std::string bmml::barre::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::barre::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(bow, qname("bow"), content::simple);

std::string bmml::bow::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::bow::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::up_down bmml::bow::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "up") return up_down::up;
    else if (iter->second == "down") return up_down::down;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::bow::value(bmml::up_down value) {
  switch (value) {
  case bmml::up_down::up:
    attributes()[qname("value")] = "up";
    break;
  case bmml::up_down::down:
    attributes()[qname("value")] = "down";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(breath, qname("breath"), content::simple);

std::string bmml::breath::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::breath::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(chord, qname("chord"), content::complex);

std::string bmml::chord::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::chord::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(chord_data, qname("chord_data"), content::complex);
REGISTER_DEFINITION(chord_prefix, qname("chord_prefix"), content::simple);

std::string bmml::chord_prefix::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::chord_prefix::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(chord_type, qname("chord_type"), content::simple);

std::string bmml::chord_type::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::chord_type::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::chord_type::notes() const {
  return attributes().find(qname("notes"))->second;
}

void bmml::chord_type::notes(std::string const& value) {
  attributes()[qname("notes")] = value;
}


std::string bmml::chord_type::root() const {
  return attributes().find(qname("root"))->second;
}

void bmml::chord_type::root(std::string const& value) {
  attributes()[qname("root")] = value;
}

REGISTER_DEFINITION(clef, qname("clef"), content::simple);

std::string bmml::clef::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::clef::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::clef::name() const {
  return attributes().find(qname("name"))->second;
}

void bmml::clef::name(std::string const& value) {
  attributes()[qname("name")] = value;
}

REGISTER_DEFINITION(coda, qname("coda"), content::simple);

std::string bmml::coda::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::coda::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(dot, qname("dot"), content::simple);

std::string bmml::dot::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::dot::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::dot::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::dot::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(duration, qname("duration"), content::simple);
REGISTER_DEFINITION(dynamic, qname("dynamic"), content::simple);

std::string bmml::dynamic::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::dynamic::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::dynamic::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::dynamic::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(editorial_mark, qname("editorial_mark"), content::simple);

std::string bmml::editorial_mark::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::editorial_mark::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(ending, qname("ending"), content::simple);

std::string bmml::ending::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::ending::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::ending::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::ending::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(family, qname("family"), content::simple);
REGISTER_DEFINITION(fermata, qname("fermata"), content::simple);

std::string bmml::fermata::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::fermata::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(fingering, qname("fingering"), content::simple);

std::string bmml::fingering::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::fingering::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(foot_crossing, qname("foot_crossing"), content::simple);

std::string bmml::foot_crossing::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::foot_crossing::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(generic_text, qname("generic_text"), content::simple);

std::string bmml::generic_text::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::generic_text::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(hand, qname("hand"), content::simple);

std::string bmml::hand::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::hand::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::hand_type bmml::hand::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "left") return hand_type::left;
    else if (iter->second == "right") return hand_type::right;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::hand::value(bmml::hand_type value) {
  switch (value) {
  case bmml::hand_type::left:
    attributes()[qname("value")] = "left";
    break;
  case bmml::hand_type::right:
    attributes()[qname("value")] = "right";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(harmonic, qname("harmonic"), content::simple);

std::string bmml::harmonic::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::harmonic::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(inaccord, qname("inaccord"), content::simple);

std::string bmml::inaccord::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::inaccord::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::inaccord::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::inaccord::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(interval, qname("interval"), content::complex);

std::string bmml::interval::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::interval::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(interval_data, qname("interval_data"), content::complex);
REGISTER_DEFINITION(interval_ref, qname("interval_ref"), content::complex);

std::string bmml::interval_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::interval_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(interval_type, qname("interval_type"), content::simple);

std::string bmml::interval_type::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::interval_type::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::interval_type::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::interval_type::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(intervals, qname("intervals"), content::complex);
REGISTER_DEFINITION(key_signature, qname("key_signature"), content::simple);

std::string bmml::key_signature::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::key_signature::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::key_signature::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::key_signature::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(line_of_continuation, qname("line_of_continuation"), content::simple);

std::string bmml::line_of_continuation::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::line_of_continuation::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::start_stop bmml::line_of_continuation::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "start") return start_stop::start;
    else if (iter->second == "stop") return start_stop::stop;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::line_of_continuation::value(bmml::start_stop value) {
  switch (value) {
  case bmml::start_stop::start:
    attributes()[qname("value")] = "start";
    break;
  case bmml::start_stop::stop:
    attributes()[qname("value")] = "stop";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(lyric, qname("lyric"), content::complex);

std::string bmml::lyric::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::lyric::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(lyric_prefix, qname("lyric_prefix"), content::simple);

std::string bmml::lyric_prefix::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::lyric_prefix::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(lyric_repeat, qname("lyric_repeat"), content::complex);

std::string bmml::lyric_repeat::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::lyric_repeat::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(lyric_repetition, qname("lyric_repetition"), content::simple);

std::string bmml::lyric_repetition::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::lyric_repetition::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(lyrics, qname("lyrics"), content::complex);
REGISTER_DEFINITION(merged_text, qname("merged_text"), content::simple);

std::string bmml::merged_text::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::merged_text::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::merged_text::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::merged_text::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(meta_data, qname("meta_data"), content::mixed);
REGISTER_DEFINITION(metronome, qname("metronome"), content::complex);

std::string bmml::metronome::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::metronome::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(metronome_equal, qname("metronome_equal"), content::simple);

std::string bmml::metronome_equal::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::metronome_equal::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(metronome_note_type, qname("metronome_note_type"), content::simple);

std::string bmml::metronome_note_type::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::metronome_note_type::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::ambiguous_value bmml::metronome_note_type::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "8th_or_128th") return ambiguous_value::eighth_or_128th;
    else if (iter->second == "quarter_or_64th") return ambiguous_value::quarter_or_64th;
    else if (iter->second == "half_or_32nd") return ambiguous_value::half_or_32nd;
    else if (iter->second == "whole_or_16th") return ambiguous_value::whole_or_16th;
    else if (iter->second == "brevis") return ambiguous_value::brevis;
    else if (iter->second == "longa") return ambiguous_value::longa;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::metronome_note_type::value(bmml::ambiguous_value value) {
  switch (value) {
  case bmml::ambiguous_value::eighth_or_128th:
    attributes()[qname("value")] = "8th_or_128th";
    break;
  case bmml::ambiguous_value::quarter_or_64th:
    attributes()[qname("value")] = "quarter_or_64th";
    break;
  case bmml::ambiguous_value::half_or_32nd:
    attributes()[qname("value")] = "half_or_32nd";
    break;
  case bmml::ambiguous_value::whole_or_16th:
    attributes()[qname("value")] = "whole_or_16th";
    break;
  case bmml::ambiguous_value::brevis:
    attributes()[qname("value")] = "brevis";
    break;
  case bmml::ambiguous_value::longa:
    attributes()[qname("value")] = "longa";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(metronome_value, qname("metronome_value"), content::simple);

std::string bmml::metronome_value::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::metronome_value::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::metronome_value::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::metronome_value::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(midi_instrument, qname("midi_instrument"), content::empty);

std::string bmml::midi_instrument::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::midi_instrument::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(midi_metronome, qname("midi_metronome"), content::empty);

std::string bmml::midi_metronome::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::midi_metronome::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(multimeasure, qname("multimeasure"), content::simple);
REGISTER_DEFINITION(music_hyphen, qname("music_hyphen"), content::simple);

std::string bmml::music_hyphen::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::music_hyphen::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(name, qname("name"), content::simple);

std::string bmml::name::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::name::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(newline, qname("newline"), content::simple);

std::string bmml::newline::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::newline::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(note, qname("note"), content::complex);

std::string bmml::note::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::note::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(note_data, qname("note_data"), content::complex);
REGISTER_DEFINITION(note_ref, qname("note_ref"), content::empty);

std::string bmml::note_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::note_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(note_type, qname("note_type"), content::simple);

std::string bmml::note_type::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::note_type::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::ambiguous_value bmml::note_type::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "8th_or_128th") return ambiguous_value::eighth_or_128th;
    else if (iter->second == "quarter_or_64th") return ambiguous_value::quarter_or_64th;
    else if (iter->second == "half_or_32nd") return ambiguous_value::half_or_32nd;
    else if (iter->second == "whole_or_16th") return ambiguous_value::whole_or_16th;
    else if (iter->second == "brevis") return ambiguous_value::brevis;
    else if (iter->second == "longa") return ambiguous_value::longa;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::note_type::value(bmml::ambiguous_value value) {
  switch (value) {
  case bmml::ambiguous_value::eighth_or_128th:
    attributes()[qname("value")] = "8th_or_128th";
    break;
  case bmml::ambiguous_value::quarter_or_64th:
    attributes()[qname("value")] = "quarter_or_64th";
    break;
  case bmml::ambiguous_value::half_or_32nd:
    attributes()[qname("value")] = "half_or_32nd";
    break;
  case bmml::ambiguous_value::whole_or_16th:
    attributes()[qname("value")] = "whole_or_16th";
    break;
  case bmml::ambiguous_value::brevis:
    attributes()[qname("value")] = "brevis";
    break;
  case bmml::ambiguous_value::longa:
    attributes()[qname("value")] = "longa";
    break;

  default:
    throw illegal_enumeration();
  }
}


bmml::diatonic_step bmml::note_type::name() const {
  auto iter = attributes().find(qname("name"));
  if (iter != attributes().end()) {
         if (iter->second == "A") return diatonic_step::A;
    else if (iter->second == "B") return diatonic_step::B;
    else if (iter->second == "C") return diatonic_step::C;
    else if (iter->second == "D") return diatonic_step::D;
    else if (iter->second == "E") return diatonic_step::E;
    else if (iter->second == "F") return diatonic_step::F;
    else if (iter->second == "G") return diatonic_step::G;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::note_type::name(bmml::diatonic_step value) {
  switch (value) {
  case bmml::diatonic_step::A:
    attributes()[qname("name")] = "A";
    break;
  case bmml::diatonic_step::B:
    attributes()[qname("name")] = "B";
    break;
  case bmml::diatonic_step::C:
    attributes()[qname("name")] = "C";
    break;
  case bmml::diatonic_step::D:
    attributes()[qname("name")] = "D";
    break;
  case bmml::diatonic_step::E:
    attributes()[qname("name")] = "E";
    break;
  case bmml::diatonic_step::F:
    attributes()[qname("name")] = "F";
    break;
  case bmml::diatonic_step::G:
    attributes()[qname("name")] = "G";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(nuance, qname("nuance"), content::simple);

std::string bmml::nuance::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::nuance::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::nuance::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::nuance::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(nuance_ref, qname("nuance_ref"), content::empty);

std::string bmml::nuance_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::nuance_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(nuances, qname("nuances"), content::complex);
REGISTER_DEFINITION(number, qname("number"), content::simple);

std::string bmml::number::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::number::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::number::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::number::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(octave, qname("octave"), content::simple);

std::string bmml::octave::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::octave::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::octave::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::octave::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(organ_pedal, qname("organ_pedal"), content::simple);

std::string bmml::organ_pedal::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::organ_pedal::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(ornament, qname("ornament"), content::complex);

std::string bmml::ornament::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::ornament::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


vector<shared_ptr<bmml::accidental>> bmml::ornament::accidentals() const {
  return find_elements<bmml::accidental>();
}

shared_ptr<bmml::ornament_type> bmml::ornament::ornament_type() const {
  return find_element<bmml::ornament_type>();
}

REGISTER_DEFINITION(ornament_type, qname("ornament_type"), content::simple);

std::string bmml::ornament_type::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::ornament_type::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::ornament_type::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::ornament_type::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(part, qname("part"), content::complex);

std::string bmml::part::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::part::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(part_data, qname("part_data"), content::complex);

std::string bmml::part_data::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::part_data::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(part_list, qname("part_list"), content::complex);
REGISTER_DEFINITION(part_name, qname("part_name"), content::simple);

std::string bmml::part_name::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::part_name::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(pedal, qname("pedal"), content::simple);

std::string bmml::pedal::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::pedal::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::pedal::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::pedal::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(pitch, qname("pitch"), content::simple);
REGISTER_DEFINITION(pizzicato, qname("pizzicato"), content::simple);

std::string bmml::pizzicato::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::pizzicato::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::hand_type bmml::pizzicato::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "left") return hand_type::left;
    else if (iter->second == "right") return hand_type::right;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::pizzicato::value(bmml::hand_type value) {
  switch (value) {
  case bmml::hand_type::left:
    attributes()[qname("value")] = "left";
    break;
  case bmml::hand_type::right:
    attributes()[qname("value")] = "right";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(rasgueado, qname("rasgueado"), content::simple);

std::string bmml::rasgueado::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::rasgueado::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::up_down bmml::rasgueado::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "up") return up_down::up;
    else if (iter->second == "down") return up_down::down;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::rasgueado::value(bmml::up_down value) {
  switch (value) {
  case bmml::up_down::up:
    attributes()[qname("value")] = "up";
    break;
  case bmml::up_down::down:
    attributes()[qname("value")] = "down";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(repeat, qname("repeat"), content::complex);

std::string bmml::repeat::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::repeat::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(repeat_data, qname("repeat_data"), content::complex);
REGISTER_DEFINITION(repeat_ref, qname("repeat_ref"), content::complex);

std::string bmml::repeat_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::repeat_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(repeats, qname("repeats"), content::complex);
REGISTER_DEFINITION(repetition, qname("repetition"), content::simple);

std::string bmml::repetition::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::repetition::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::repetition::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::repetition::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(rest, qname("rest"), content::complex);

std::string bmml::rest::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::rest::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(rest_data, qname("rest_data"), content::complex);
REGISTER_DEFINITION(rest_type, qname("rest_type"), content::simple);

std::string bmml::rest_type::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::rest_type::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::ambiguous_value bmml::rest_type::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "8th_or_128th") return ambiguous_value::eighth_or_128th;
    else if (iter->second == "quarter_or_64th") return ambiguous_value::quarter_or_64th;
    else if (iter->second == "half_or_32nd") return ambiguous_value::half_or_32nd;
    else if (iter->second == "whole_or_16th") return ambiguous_value::whole_or_16th;
    else if (iter->second == "brevis") return ambiguous_value::brevis;
    else if (iter->second == "longa") return ambiguous_value::longa;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::rest_type::value(bmml::ambiguous_value value) {
  switch (value) {
  case bmml::ambiguous_value::eighth_or_128th:
    attributes()[qname("value")] = "8th_or_128th";
    break;
  case bmml::ambiguous_value::quarter_or_64th:
    attributes()[qname("value")] = "quarter_or_64th";
    break;
  case bmml::ambiguous_value::half_or_32nd:
    attributes()[qname("value")] = "half_or_32nd";
    break;
  case bmml::ambiguous_value::whole_or_16th:
    attributes()[qname("value")] = "whole_or_16th";
    break;
  case bmml::ambiguous_value::brevis:
    attributes()[qname("value")] = "brevis";
    break;
  case bmml::ambiguous_value::longa:
    attributes()[qname("value")] = "longa";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(rhythmic_group, qname("rhythmic_group"), content::simple);
REGISTER_DEFINITION(right_string_fingering, qname("right_string_fingering"), content::simple);

std::string bmml::right_string_fingering::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::right_string_fingering::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::right_string_fingering::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::right_string_fingering::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(score, qname("score"), content::complex);

std::string bmml::score::version() const {
  return attributes().find(qname("version"))->second;
}

void bmml::score::version(std::string const& value) {
  attributes()[qname("version")] = value;
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
  return attributes().find(qname("id"))->second;
}

void bmml::segno::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(separator, qname("separator"), content::simple);

std::string bmml::separator::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::separator::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(shift_line, qname("shift_line"), content::simple);

std::string bmml::shift_line::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::shift_line::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(slur, qname("slur"), content::simple);

std::string bmml::slur::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::slur::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::slur::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::slur::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(slur_ref, qname("slur_ref"), content::empty);

std::string bmml::slur_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::slur_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::slur_ref::start_ref() const {
  return attributes().find(qname("start_ref"))->second;
}

void bmml::slur_ref::start_ref(std::string const& value) {
  attributes()[qname("start_ref")] = value;
}

REGISTER_DEFINITION(slurs, qname("slurs"), content::complex);
REGISTER_DEFINITION(space, qname("space"), content::simple);

std::string bmml::space::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::space::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(stem, qname("stem"), content::complex);

std::string bmml::stem::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::stem::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(stem_data, qname("stem_data"), content::complex);
REGISTER_DEFINITION(stem_type, qname("stem_type"), content::simple);

std::string bmml::stem_type::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::stem_type::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::stem_type::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::stem_type::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(string, qname("string"), content::simple);

std::string bmml::string::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::string::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::string::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::string::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(string_fingering, qname("string_fingering"), content::simple);

std::string bmml::string_fingering::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::string_fingering::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::string_fingering::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::string_fingering::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(string_position, qname("string_position"), content::simple);

std::string bmml::string_position::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::string_position::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::string_position::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::string_position::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(stroke, qname("stroke"), content::simple);

std::string bmml::stroke::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::stroke::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


bmml::up_down bmml::stroke::value() const {
  auto iter = attributes().find(qname("value"));
  if (iter != attributes().end()) {
         if (iter->second == "up") return up_down::up;
    else if (iter->second == "down") return up_down::down;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::stroke::value(bmml::up_down value) {
  switch (value) {
  case bmml::up_down::up:
    attributes()[qname("value")] = "up";
    break;
  case bmml::up_down::down:
    attributes()[qname("value")] = "down";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(syllabic_mute, qname("syllabic_mute"), content::simple);

std::string bmml::syllabic_mute::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::syllabic_mute::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(syllabic_parenthesis, qname("syllabic_parenthesis"), content::simple);

std::string bmml::syllabic_parenthesis::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::syllabic_parenthesis::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(syllabic_slur, qname("syllabic_slur"), content::simple);

std::string bmml::syllabic_slur::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::syllabic_slur::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::syllabic_slur::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::syllabic_slur::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(syllabic_text, qname("syllabic_text"), content::simple);

std::string bmml::syllabic_text::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::syllabic_text::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(syllable, qname("syllable"), content::complex);

std::string bmml::syllable::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::syllable::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(syllable_mute, qname("syllable_mute"), content::complex);

std::string bmml::syllable_mute::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::syllable_mute::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(syllable_ref, qname("syllable_ref"), content::empty);

std::string bmml::syllable_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::syllable_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(tie, qname("tie"), content::simple);

std::string bmml::tie::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::tie::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::tie::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::tie::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(tie_ref, qname("tie_ref"), content::empty);

std::string bmml::tie_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::tie_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::tie_ref::start_ref() const {
  return attributes().find(qname("start_ref"))->second;
}

void bmml::tie_ref::start_ref(std::string const& value) {
  attributes()[qname("start_ref")] = value;
}


bmml::start_stop bmml::tie_ref::type() const {
  auto iter = attributes().find(qname("type"));
  if (iter != attributes().end()) {
         if (iter->second == "start") return start_stop::start;
    else if (iter->second == "stop") return start_stop::stop;

    throw illegal_enumeration();
  }
  throw missing_attribute();
}

void bmml::tie_ref::type(bmml::start_stop value) {
  switch (value) {
  case bmml::start_stop::start:
    attributes()[qname("type")] = "start";
    break;
  case bmml::start_stop::stop:
    attributes()[qname("type")] = "stop";
    break;

  default:
    throw illegal_enumeration();
  }
}

REGISTER_DEFINITION(ties, qname("ties"), content::complex);
REGISTER_DEFINITION(time_signature, qname("time_signature"), content::simple);

std::string bmml::time_signature::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::time_signature::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::time_signature::values() const {
  return attributes().find(qname("values"))->second;
}

void bmml::time_signature::values(std::string const& value) {
  attributes()[qname("values")] = value;
}

REGISTER_DEFINITION(tremolo, qname("tremolo"), content::simple);

std::string bmml::tremolo::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::tremolo::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::tremolo::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::tremolo::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(tremolo_ref, qname("tremolo_ref"), content::empty);

std::string bmml::tremolo_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::tremolo_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(tuplet, qname("tuplet"), content::simple);

std::string bmml::tuplet::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::tuplet::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::tuplet::value() const {
  return attributes().find(qname("value"))->second;
}

void bmml::tuplet::value(std::string const& value) {
  attributes()[qname("value")] = value;
}

REGISTER_DEFINITION(tuplet_ref, qname("tuplet_ref"), content::empty);

std::string bmml::tuplet_ref::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::tuplet_ref::id(std::string const& value) {
  attributes()[qname("id")] = value;
}


std::string bmml::tuplet_ref::notes() const {
  return attributes().find(qname("notes"))->second;
}

void bmml::tuplet_ref::notes(std::string const& value) {
  attributes()[qname("notes")] = value;
}

REGISTER_DEFINITION(tuplets, qname("tuplets"), content::complex);
REGISTER_DEFINITION(unknown, qname("unknown"), content::simple);

std::string bmml::unknown::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::unknown::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

REGISTER_DEFINITION(value_prefix, qname("value_prefix"), content::simple);

std::string bmml::value_prefix::id() const {
  return attributes().find(qname("id"))->second;
}

void bmml::value_prefix::id(std::string const& value) {
  attributes()[qname("id")] = value;
}

