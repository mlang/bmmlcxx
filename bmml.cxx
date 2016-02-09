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
REGISTER_DEFINITION(accidental, qname("accidental"), content::simple);
REGISTER_DEFINITION(accordion_register, qname("accordion_register"), content::simple);
REGISTER_DEFINITION(accordion_row, qname("accordion_row"), content::simple);
REGISTER_DEFINITION(alteration, qname("alteration"), content::simple);
REGISTER_DEFINITION(alternation, qname("alternation"), content::simple);
REGISTER_DEFINITION(alternation_ref, qname("alternation_ref"), content::empty);
REGISTER_DEFINITION(appoggiatura_ref, qname("appoggiatura_ref"), content::empty);
REGISTER_DEFINITION(barline, qname("barline"), content::complex);
REGISTER_DEFINITION(barline_type, qname("barline_type"), content::simple);
REGISTER_DEFINITION(barre, qname("barre"), content::simple);
REGISTER_DEFINITION(bow, qname("bow"), content::simple);
REGISTER_DEFINITION(breath, qname("breath"), content::simple);
REGISTER_DEFINITION(chord, qname("chord"), content::complex);
REGISTER_DEFINITION(chord_data, qname("chord_data"), content::complex);
REGISTER_DEFINITION(chord_prefix, qname("chord_prefix"), content::simple);
REGISTER_DEFINITION(chord_type, qname("chord_type"), content::simple);
REGISTER_DEFINITION(clef, qname("clef"), content::simple);
REGISTER_DEFINITION(coda, qname("coda"), content::simple);
REGISTER_DEFINITION(dot, qname("dot"), content::simple);
REGISTER_DEFINITION(duration, qname("duration"), content::simple);
REGISTER_DEFINITION(dynamic, qname("dynamic"), content::simple);
REGISTER_DEFINITION(editorial_mark, qname("editorial_mark"), content::simple);
REGISTER_DEFINITION(ending, qname("ending"), content::simple);
REGISTER_DEFINITION(family, qname("family"), content::simple);
REGISTER_DEFINITION(fermata, qname("fermata"), content::simple);
REGISTER_DEFINITION(fingering, qname("fingering"), content::simple);
REGISTER_DEFINITION(foot_crossing, qname("foot_crossing"), content::simple);
REGISTER_DEFINITION(generic_text, qname("generic_text"), content::simple);
REGISTER_DEFINITION(hand, qname("hand"), content::simple);
REGISTER_DEFINITION(harmonic, qname("harmonic"), content::simple);
REGISTER_DEFINITION(inaccord, qname("inaccord"), content::simple);
REGISTER_DEFINITION(interval, qname("interval"), content::complex);
REGISTER_DEFINITION(interval_data, qname("interval_data"), content::complex);
REGISTER_DEFINITION(interval_ref, qname("interval_ref"), content::complex);
REGISTER_DEFINITION(interval_type, qname("interval_type"), content::simple);
REGISTER_DEFINITION(intervals, qname("intervals"), content::complex);
REGISTER_DEFINITION(key_signature, qname("key_signature"), content::simple);
REGISTER_DEFINITION(line_of_continuation, qname("line_of_continuation"), content::simple);
REGISTER_DEFINITION(lyric, qname("lyric"), content::complex);
REGISTER_DEFINITION(lyric_prefix, qname("lyric_prefix"), content::simple);
REGISTER_DEFINITION(lyric_repeat, qname("lyric_repeat"), content::complex);
REGISTER_DEFINITION(lyric_repetition, qname("lyric_repetition"), content::simple);
REGISTER_DEFINITION(lyrics, qname("lyrics"), content::complex);
REGISTER_DEFINITION(merged_text, qname("merged_text"), content::simple);
REGISTER_DEFINITION(meta_data, qname("meta_data"), content::mixed);
REGISTER_DEFINITION(metronome, qname("metronome"), content::complex);
REGISTER_DEFINITION(metronome_equal, qname("metronome_equal"), content::simple);
REGISTER_DEFINITION(metronome_note_type, qname("metronome_note_type"), content::simple);
REGISTER_DEFINITION(metronome_value, qname("metronome_value"), content::simple);
REGISTER_DEFINITION(midi_instrument, qname("midi_instrument"), content::empty);
REGISTER_DEFINITION(midi_metronome, qname("midi_metronome"), content::empty);
REGISTER_DEFINITION(multimeasure, qname("multimeasure"), content::simple);
REGISTER_DEFINITION(music_hyphen, qname("music_hyphen"), content::simple);
REGISTER_DEFINITION(name, qname("name"), content::simple);
REGISTER_DEFINITION(newline, qname("newline"), content::simple);
REGISTER_DEFINITION(note, qname("note"), content::complex);
REGISTER_DEFINITION(note_data, qname("note_data"), content::complex);
REGISTER_DEFINITION(note_ref, qname("note_ref"), content::empty);
REGISTER_DEFINITION(note_type, qname("note_type"), content::simple);
REGISTER_DEFINITION(nuance, qname("nuance"), content::simple);
REGISTER_DEFINITION(nuance_ref, qname("nuance_ref"), content::empty);
REGISTER_DEFINITION(nuances, qname("nuances"), content::complex);
REGISTER_DEFINITION(number, qname("number"), content::simple);
REGISTER_DEFINITION(octave, qname("octave"), content::simple);
REGISTER_DEFINITION(organ_pedal, qname("organ_pedal"), content::simple);
REGISTER_DEFINITION(ornament, qname("ornament"), content::complex);
REGISTER_DEFINITION(ornament_type, qname("ornament_type"), content::simple);
REGISTER_DEFINITION(part, qname("part"), content::complex);
REGISTER_DEFINITION(part_data, qname("part_data"), content::complex);
REGISTER_DEFINITION(part_list, qname("part_list"), content::complex);
REGISTER_DEFINITION(part_name, qname("part_name"), content::simple);
REGISTER_DEFINITION(pedal, qname("pedal"), content::simple);
REGISTER_DEFINITION(pitch, qname("pitch"), content::simple);
REGISTER_DEFINITION(pizzicato, qname("pizzicato"), content::simple);
REGISTER_DEFINITION(rasgueado, qname("rasgueado"), content::simple);
REGISTER_DEFINITION(repeat, qname("repeat"), content::complex);
REGISTER_DEFINITION(repeat_data, qname("repeat_data"), content::complex);
REGISTER_DEFINITION(repeat_ref, qname("repeat_ref"), content::complex);
REGISTER_DEFINITION(repeats, qname("repeats"), content::complex);
REGISTER_DEFINITION(repetition, qname("repetition"), content::simple);
REGISTER_DEFINITION(rest, qname("rest"), content::complex);
REGISTER_DEFINITION(rest_data, qname("rest_data"), content::complex);
REGISTER_DEFINITION(rest_type, qname("rest_type"), content::simple);
REGISTER_DEFINITION(rhythmic_group, qname("rhythmic_group"), content::simple);
REGISTER_DEFINITION(right_string_fingering, qname("right_string_fingering"), content::simple);
REGISTER_DEFINITION(score, qname("score"), content::complex);

shared_ptr<bmml::score_header> bmml::score::header() const {
  return find_element<bmml::score_header>();
}
shared_ptr<bmml::score_data> bmml::score::data() const {
  return find_element<bmml::score_data>();
}
REGISTER_DEFINITION(score_data, qname("score_data"), content::complex);
REGISTER_DEFINITION(score_header, qname("score_header"), content::complex);
REGISTER_DEFINITION(segno, qname("segno"), content::simple);
REGISTER_DEFINITION(separator, qname("separator"), content::simple);
REGISTER_DEFINITION(shift_line, qname("shift_line"), content::simple);
REGISTER_DEFINITION(slur, qname("slur"), content::simple);
REGISTER_DEFINITION(slur_ref, qname("slur_ref"), content::empty);
REGISTER_DEFINITION(slurs, qname("slurs"), content::complex);
REGISTER_DEFINITION(space, qname("space"), content::simple);
REGISTER_DEFINITION(stem, qname("stem"), content::complex);
REGISTER_DEFINITION(stem_data, qname("stem_data"), content::complex);
REGISTER_DEFINITION(stem_type, qname("stem_type"), content::simple);
REGISTER_DEFINITION(string, qname("string"), content::simple);
REGISTER_DEFINITION(string_fingering, qname("string_fingering"), content::simple);
REGISTER_DEFINITION(string_position, qname("string_position"), content::simple);
REGISTER_DEFINITION(stroke, qname("stroke"), content::simple);
REGISTER_DEFINITION(syllabic_mute, qname("syllabic_mute"), content::simple);
REGISTER_DEFINITION(syllabic_parenthesis, qname("syllabic_parenthesis"), content::simple);
REGISTER_DEFINITION(syllabic_slur, qname("syllabic_slur"), content::simple);
REGISTER_DEFINITION(syllabic_text, qname("syllabic_text"), content::simple);
REGISTER_DEFINITION(syllable, qname("syllable"), content::complex);
REGISTER_DEFINITION(syllable_mute, qname("syllable_mute"), content::complex);
REGISTER_DEFINITION(syllable_ref, qname("syllable_ref"), content::empty);
REGISTER_DEFINITION(tie, qname("tie"), content::simple);
REGISTER_DEFINITION(tie_ref, qname("tie_ref"), content::empty);
REGISTER_DEFINITION(ties, qname("ties"), content::complex);
REGISTER_DEFINITION(time_signature, qname("time_signature"), content::simple);
REGISTER_DEFINITION(tremolo, qname("tremolo"), content::simple);
REGISTER_DEFINITION(tremolo_ref, qname("tremolo_ref"), content::empty);
REGISTER_DEFINITION(tuplet, qname("tuplet"), content::simple);
REGISTER_DEFINITION(tuplet_ref, qname("tuplet_ref"), content::empty);
REGISTER_DEFINITION(tuplets, qname("tuplets"), content::complex);
REGISTER_DEFINITION(unknown, qname("unknown"), content::simple);
REGISTER_DEFINITION(value_prefix, qname("value_prefix"), content::simple);
