#ifndef BMML_HXX
#define BMML_HXX

#include <stdexcept>
#include <xml/content>
#include <xml/parser>
#include <xml/qname>
#include <xml/serializer>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

namespace bmml {
namespace dom {

// A simple, DOM-like in-memory representation of raw XML. It only supports
// empty, simple, and complex content (no mixed content) and is not
// particularly efficient.

class element {
public:
  using attributes_type = std::map<xml::qname, std::string>;
  using elements_type = std::vector<std::shared_ptr<element>>;

  template<typename T> static std::shared_ptr<element> create(xml::parser& p) {
    return std::make_shared<T>(p, false);
  }
  
  element(const xml::qname& name) : tag_name_(name) {}
  element(const xml::qname& name, std::string const& text)
  : tag_name_(name), text_(text) {}
  virtual ~element() = default;

  const xml::qname& tag_name() const { return tag_name_; }
  const attributes_type& attributes () const {return attributes_;}

  attributes_type& attributes () { return attributes_; }

  const std::string& text () const {return text_;}

  void text (const std::string& text) {text_ = text;}

  const elements_type& elements () const {return elements_;}
  elements_type& elements () {return elements_;}

  elements_type::const_iterator begin() const { return elements_.begin(); }
  elements_type::const_iterator end() const { return elements_.end(); }
  elements_type::iterator begin() { return elements_.begin(); }
  elements_type::iterator end() { return elements_.end(); }

  template<typename T> std::shared_ptr<T> find_element() const {
    for (auto &&e : elements_)
      if (auto t = std::dynamic_pointer_cast<T>(e)) return t;
    return {};
  }

  template<typename T> std::vector<std::shared_ptr<T>> find_elements() const {
    std::vector<std::shared_ptr<T>> result;
    for (auto &&e : elements_)
      if (auto t = std::dynamic_pointer_cast<T>(e)) result.push_back(t);
    return result;
  }

  // Parse an element. If start_end is false, then don't parse the
  // start and end of the element.
  //
  element (xml::parser&, bool start_end = true);

  // Serialize an element. If start_end is false, then don't serialize
  // the start and end of the element.
  //
  void serialize (xml::serializer&, bool start_end = true) const;

protected:
  void parse(xml::parser&, bool start_end = true);

private:
  xml::qname tag_name_;
  attributes_type attributes_;
  std::string text_;           // Simple content only.
  elements_type elements_;     // Complex content only.
};

struct factory {
  using map_type =
    std::map<xml::qname,
             std::tuple<std::shared_ptr<element>(*)(xml::parser&),
                        xml::content>>;

  static std::shared_ptr<element> make(xml::parser& p) {
    auto name = p.qname();
    auto iter = get_map()->find(name);
    if (iter == get_map()->end()) {
      return std::make_shared<element>(p, false);
    }
    p.content(std::get<1>(iter->second));
    return std::get<0>(iter->second)(p);
  }

protected:
  static map_type *get_map() {
    if (!default_map) default_map = new map_type;

    return default_map;
  }

private:
  static map_type *default_map;
};

template<typename T>
struct register_element : factory {
  register_element(xml::qname const& name, xml::content const& content) {
    get_map()->insert({name, std::make_tuple(&element::create<T>, content)});
  }
};

#define REGISTER_DECLARATION(NAME)                       \
static dom::register_element<NAME> _factory_registration;

#define REGISTER_DEFINITION(NAME, QNAME, CONTENT)        \
bmml::dom::register_element<bmml::NAME>                  \
bmml::NAME::_factory_registration{QNAME, CONTENT}

} // namespace dom

class illegal_enumeration: public std::runtime_error {
public:
  illegal_enumeration() : std::runtime_error("Illegal enum") {}
};

class missing_attribute: public std::runtime_error {
public:
  missing_attribute() : std::runtime_error("Missing attribute") {}
};

using boost::optional;

// ------------------------------------------------------------------------- //

enum class inaccord_t {
  full,
  part,
  division
};

enum class full_half_vertical {
  full,
  half,
  vertical
};

enum class left_middle_right {
  left,
  middle,
  right
};

enum class up_down {
  up,
  down
};

enum class diatonic_step {
  A,
  B,
  C,
  D,
  E,
  F,
  G
};

enum class ambiguous_value {
  eighth_or_128th,
  quarter_or_64th,
  half_or_32nd,
  whole_or_16th,
  brevis,
  longa
};

enum class left_right {
  left,
  right
};

enum class glissando_start_stop {
  glissando,
  start,
  stop
};

enum class natural_artificial {
  natural,
  artificial
};

enum class value_prefix_t {
  separator,
  large,
  small,
  twohundredfiftysixth
};

enum class above_below {
  above,
  below
};

enum class organ_pedal_t {
  left_toe,
  left_heel,
  right_toe,
  right_heel
};

enum class start_stop_continue {
  start,
  stop,
  continue_
};

enum class full_half_caesura {
  full,
  half,
  caesura
};

enum class start_stop {
  start,
  stop
};

class abbr_name : public dom::element {
  REGISTER_DECLARATION(abbr_name);

public:
  abbr_name(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> value() const;
  void value(optional<std::string>);
};

class accidental : public dom::element {
  REGISTER_DECLARATION(accidental);

public:
  accidental(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> editorial() const;
  void editorial(optional<std::string>);

  optional<std::string> step() const;
  void step(optional<std::string>);

  optional<above_below> placement() const;
  void placement(optional<above_below>);

  optional<std::string> cancel() const;
  void cancel(optional<std::string>);

  std::string value() const;
  void value(std::string const&);
};

class accordion_register : public dom::element {
  REGISTER_DECLARATION(accordion_register);

public:
  accordion_register(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class accordion_row : public dom::element {
  REGISTER_DECLARATION(accordion_row);

public:
  accordion_row(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class alteration : public dom::element {
  REGISTER_DECLARATION(alteration);

public:
  alteration(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  operator int() const;
  alteration& operator=(int);
};

class alternation : public dom::element {
  REGISTER_DECLARATION(alternation);

public:
  alternation(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class alternation_ref : public dom::element {
  REGISTER_DECLARATION(alternation_ref);

public:
  alternation_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  start_stop type() const;
  void type(start_stop);
};

class appoggiatura_ref : public dom::element {
  REGISTER_DECLARATION(appoggiatura_ref);

public:
  appoggiatura_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class barline : public dom::element {
  REGISTER_DECLARATION(barline);

public:
  barline(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<left_middle_right> type() const;
  void type(optional<left_middle_right>);
};

class barline_type : public dom::element {
  REGISTER_DECLARATION(barline_type);

public:
  barline_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class barre : public dom::element {
  REGISTER_DECLARATION(barre);

public:
  barre(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  full_half_vertical value() const;
  void value(full_half_vertical);
};

class bow : public dom::element {
  REGISTER_DECLARATION(bow);

public:
  bow(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  up_down value() const;
  void value(up_down);
};

class breath : public dom::element {
  REGISTER_DECLARATION(breath);

public:
  breath(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  full_half_caesura value() const;
  void value(full_half_caesura);
};

class chord : public dom::element {
  REGISTER_DECLARATION(chord);

public:
  chord(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class chord_data : public dom::element {
  REGISTER_DECLARATION(chord_data);

public:
  chord_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class chord_prefix : public dom::element {
  REGISTER_DECLARATION(chord_prefix);

public:
  chord_prefix(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class chord_type : public dom::element {
  REGISTER_DECLARATION(chord_type);

public:
  chord_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> value() const;
  void value(optional<std::string>);

  optional<std::string> dot() const;
  void dot(optional<std::string>);

  optional<std::string> duration() const;
  void duration(optional<std::string>);

  optional<std::string> bass() const;
  void bass(optional<std::string>);

  std::string notes() const;
  void notes(std::string const&);

  std::string root() const;
  void root(std::string const&);
};

class clef : public dom::element {
  REGISTER_DECLARATION(clef);

public:
  clef(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> cross_staff() const;
  void cross_staff(optional<bool>);

  optional<above_below> eight() const;
  void eight(optional<above_below>);

  optional<std::string> line() const;
  void line(optional<std::string>);

  std::string name() const;
  void name(std::string const&);
};

class coda : public dom::element {
  REGISTER_DECLARATION(coda);

public:
  coda(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class dot : public dom::element {
  REGISTER_DECLARATION(dot);

public:
  dot(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class duration : public dom::element {
  REGISTER_DECLARATION(duration);

public:
  duration(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  operator int() const;
  duration& operator=(int);
};

class dynamic : public dom::element {
  REGISTER_DECLARATION(dynamic);

public:
  dynamic(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class editorial_mark : public dom::element {
  REGISTER_DECLARATION(editorial_mark);

public:
  editorial_mark(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class ending : public dom::element {
  REGISTER_DECLARATION(ending);

public:
  ending(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class family : public dom::element {
  REGISTER_DECLARATION(family);

public:
  family(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class fermata : public dom::element {
  REGISTER_DECLARATION(fermata);

public:
  fermata(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> shape() const;
  void shape(optional<std::string>);
};

class fingering : public dom::element {
  REGISTER_DECLARATION(fingering);

public:
  fingering(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> alternative() const;
  void alternative(optional<std::string>);

  optional<std::string> value() const;
  void value(optional<std::string>);
};

class foot_crossing : public dom::element {
  REGISTER_DECLARATION(foot_crossing);

public:
  foot_crossing(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class generic_text : public dom::element {
  REGISTER_DECLARATION(generic_text);

public:
  generic_text(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> lang() const;
  void lang(optional<std::string>);

  optional<std::string> value() const;
  void value(optional<std::string>);

  optional<std::string> type() const;
  void type(optional<std::string>);
};

class hand : public dom::element {
  REGISTER_DECLARATION(hand);

public:
  hand(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<up_down> chord_dir() const;
  void chord_dir(optional<up_down>);

  left_right value() const;
  void value(left_right);
};

class harmonic : public dom::element {
  REGISTER_DECLARATION(harmonic);

public:
  harmonic(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  natural_artificial value() const;
  void value(natural_artificial);
};

class inaccord : public dom::element {
  REGISTER_DECLARATION(inaccord);

public:
  inaccord(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  inaccord_t value() const;
  void value(inaccord_t);
};

class interval : public dom::element {
  REGISTER_DECLARATION(interval);

public:
  interval(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class interval_data : public dom::element {
  REGISTER_DECLARATION(interval_data);

public:
  interval_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class interval_ref : public dom::element {
  REGISTER_DECLARATION(interval_ref);

public:
  interval_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class interval_type : public dom::element {
  REGISTER_DECLARATION(interval_type);

public:
  interval_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  std::string value() const;
  void value(std::string const&);
};

class intervals : public dom::element {
  REGISTER_DECLARATION(intervals);

public:
  intervals(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class key_signature : public dom::element {
  REGISTER_DECLARATION(key_signature);

public:
  key_signature(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> cancel() const;
  void cancel(optional<std::string>);

  std::string value() const;
  void value(std::string const&);
};

class line_of_continuation : public dom::element {
  REGISTER_DECLARATION(line_of_continuation);

public:
  line_of_continuation(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  start_stop value() const;
  void value(start_stop);
};

class lyric : public dom::element {
  REGISTER_DECLARATION(lyric);

public:
  lyric(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> lang() const;
  void lang(optional<std::string>);
};

class lyric_prefix : public dom::element {
  REGISTER_DECLARATION(lyric_prefix);

public:
  lyric_prefix(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class lyric_repeat : public dom::element {
  REGISTER_DECLARATION(lyric_repeat);

public:
  lyric_repeat(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class lyric_repetition : public dom::element {
  REGISTER_DECLARATION(lyric_repetition);

public:
  lyric_repetition(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);
};

class lyrics : public dom::element {
  REGISTER_DECLARATION(lyrics);

public:
  lyrics(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class merged_text : public dom::element {
  REGISTER_DECLARATION(merged_text);

public:
  merged_text(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class meta_data : public dom::element {
  REGISTER_DECLARATION(meta_data);

public:
  meta_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class metronome : public dom::element {
  REGISTER_DECLARATION(metronome);

public:
  metronome(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class metronome_equal : public dom::element {
  REGISTER_DECLARATION(metronome_equal);

public:
  metronome_equal(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class metronome_note_type : public dom::element {
  REGISTER_DECLARATION(metronome_note_type);

public:
  metronome_note_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> dot() const;
  void dot(optional<bool>);

  ambiguous_value value() const;
  void value(ambiguous_value);
};

class metronome_value : public dom::element {
  REGISTER_DECLARATION(metronome_value);

public:
  metronome_value(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class midi_instrument : public dom::element {
  REGISTER_DECLARATION(midi_instrument);

public:
  midi_instrument(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string value() const;
  void value(std::string const&);
};

class midi_metronome : public dom::element {
  REGISTER_DECLARATION(midi_metronome);

public:
  midi_metronome(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string value() const;
  void value(std::string const&);
};

class multimeasure : public dom::element {
  REGISTER_DECLARATION(multimeasure);

public:
  multimeasure(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class music_hyphen : public dom::element {
  REGISTER_DECLARATION(music_hyphen);

public:
  music_hyphen(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class name : public dom::element {
  REGISTER_DECLARATION(name);

public:
  name(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> value() const;
  void value(optional<std::string>);
};

class newline : public dom::element {
  REGISTER_DECLARATION(newline);

public:
  newline(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class note : public dom::element {
  REGISTER_DECLARATION(note);

public:
  note(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class note_data : public dom::element {
  REGISTER_DECLARATION(note_data);

public:
  note_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class note_ref : public dom::element {
  REGISTER_DECLARATION(note_ref);

public:
  note_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class note_type : public dom::element {
  REGISTER_DECLARATION(note_type);

public:
  note_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  ambiguous_value value() const;
  void value(ambiguous_value);

  diatonic_step name() const;
  void name(diatonic_step);
};

class nuance : public dom::element {
  REGISTER_DECLARATION(nuance);

public:
  nuance(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  std::string value() const;
  void value(std::string const&);
};

class nuance_ref : public dom::element {
  REGISTER_DECLARATION(nuance_ref);

public:
  nuance_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class nuances : public dom::element {
  REGISTER_DECLARATION(nuances);

public:
  nuances(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class number : public dom::element {
  REGISTER_DECLARATION(number);

public:
  number(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class octave : public dom::element {
  REGISTER_DECLARATION(octave);

public:
  octave(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> position() const;
  void position(optional<std::string>);

  std::string value() const;
  void value(std::string const&);
};

class organ_pedal : public dom::element {
  REGISTER_DECLARATION(organ_pedal);

public:
  organ_pedal(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<organ_pedal_t> substitution() const;
  void substitution(optional<organ_pedal_t>);

  organ_pedal_t value() const;
  void value(organ_pedal_t);
};

class ornament_type;


class ornament : public dom::element {
  REGISTER_DECLARATION(ornament);

public:
  ornament(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::vector<std::shared_ptr<accidental>> accidentals() const;
  std::shared_ptr<bmml::ornament_type> ornament_type() const;
};

class ornament_type : public dom::element {
  REGISTER_DECLARATION(ornament_type);

public:
  ornament_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  std::string value() const;
  void value(std::string const&);
};

class part : public dom::element {
  REGISTER_DECLARATION(part);

public:
  part(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class part_data : public dom::element {
  REGISTER_DECLARATION(part_data);

public:
  part_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> transpose() const;
  void transpose(optional<std::string>);

  optional<std::string> lang() const;
  void lang(optional<std::string>);

  optional<up_down> chord_dir() const;
  void chord_dir(optional<up_down>);
};

class part_list : public dom::element {
  REGISTER_DECLARATION(part_list);

public:
  part_list(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class part_name : public dom::element {
  REGISTER_DECLARATION(part_name);

public:
  part_name(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class pedal : public dom::element {
  REGISTER_DECLARATION(pedal);

public:
  pedal(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class pitch : public dom::element {
  REGISTER_DECLARATION(pitch);

public:
  pitch(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  operator int() const;
  pitch& operator=(int);
};

class pizzicato : public dom::element {
  REGISTER_DECLARATION(pizzicato);

public:
  pizzicato(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  left_right value() const;
  void value(left_right);
};

class rasgueado : public dom::element {
  REGISTER_DECLARATION(rasgueado);

public:
  rasgueado(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  up_down value() const;
  void value(up_down);
};

class repeat : public dom::element {
  REGISTER_DECLARATION(repeat);

public:
  repeat(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class repeat_data : public dom::element {
  REGISTER_DECLARATION(repeat_data);

public:
  repeat_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class repeat_ref : public dom::element {
  REGISTER_DECLARATION(repeat_ref);

public:
  repeat_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class repeats : public dom::element {
  REGISTER_DECLARATION(repeats);

public:
  repeats(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class repetition : public dom::element {
  REGISTER_DECLARATION(repetition);

public:
  repetition(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> separation() const;
  void separation(optional<std::string>);

  std::string value() const;
  void value(std::string const&);
};

class rest : public dom::element {
  REGISTER_DECLARATION(rest);

public:
  rest(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class rest_data : public dom::element {
  REGISTER_DECLARATION(rest_data);

public:
  rest_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class rest_type : public dom::element {
  REGISTER_DECLARATION(rest_type);

public:
  rest_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> multimeasure() const;
  void multimeasure(optional<std::string>);

  ambiguous_value value() const;
  void value(ambiguous_value);
};

class rhythmic_group : public dom::element {
  REGISTER_DECLARATION(rhythmic_group);

public:
  rhythmic_group(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class right_string_fingering : public dom::element {
  REGISTER_DECLARATION(right_string_fingering);

public:
  right_string_fingering(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class score_header;
class score_data;


class score : public dom::element {
  REGISTER_DECLARATION(score);

public:
  score(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string version() const;
  void version(std::string const&);

  std::shared_ptr<score_header> header() const;
  std::shared_ptr<score_data> data() const;
};

class score_data : public dom::element {
  REGISTER_DECLARATION(score_data);

public:
  score_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class score_header : public dom::element {
  REGISTER_DECLARATION(score_header);

public:
  score_header(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class segno : public dom::element {
  REGISTER_DECLARATION(segno);

public:
  segno(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class separator : public dom::element {
  REGISTER_DECLARATION(separator);

public:
  separator(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class shift_line : public dom::element {
  REGISTER_DECLARATION(shift_line);

public:
  shift_line(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  glissando_start_stop value() const;
  void value(glissando_start_stop);
};

class slur : public dom::element {
  REGISTER_DECLARATION(slur);

public:
  slur(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class slur_ref : public dom::element {
  REGISTER_DECLARATION(slur_ref);

public:
  slur_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string start_ref() const;
  void start_ref(std::string const&);

  start_stop_continue type() const;
  void type(start_stop_continue);
};

class slurs : public dom::element {
  REGISTER_DECLARATION(slurs);

public:
  slurs(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class space : public dom::element {
  REGISTER_DECLARATION(space);

public:
  space(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class stem : public dom::element {
  REGISTER_DECLARATION(stem);

public:
  stem(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class stem_data : public dom::element {
  REGISTER_DECLARATION(stem_data);

public:
  stem_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class stem_type : public dom::element {
  REGISTER_DECLARATION(stem_type);

public:
  stem_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class string : public dom::element {
  REGISTER_DECLARATION(string);

public:
  string(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  std::string value() const;
  void value(std::string const&);
};

class string_fingering : public dom::element {
  REGISTER_DECLARATION(string_fingering);

public:
  string_fingering(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<start_stop> line_of_continuation() const;
  void line_of_continuation(optional<start_stop>);

  std::string value() const;
  void value(std::string const&);
};

class string_position : public dom::element {
  REGISTER_DECLARATION(string_position);

public:
  string_position(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class stroke : public dom::element {
  REGISTER_DECLARATION(stroke);

public:
  stroke(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  up_down value() const;
  void value(up_down);
};

class syllabic_mute : public dom::element {
  REGISTER_DECLARATION(syllabic_mute);

public:
  syllabic_mute(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);
};

class syllabic_parenthesis : public dom::element {
  REGISTER_DECLARATION(syllabic_parenthesis);

public:
  syllabic_parenthesis(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class syllabic_slur : public dom::element {
  REGISTER_DECLARATION(syllabic_slur);

public:
  syllabic_slur(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> verses() const;
  void verses(optional<std::string>);

  std::string value() const;
  void value(std::string const&);
};

class syllabic_text : public dom::element {
  REGISTER_DECLARATION(syllabic_text);

public:
  syllabic_text(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<std::string> lang() const;
  void lang(optional<std::string>);

  optional<std::string> value() const;
  void value(optional<std::string>);
};

class syllable : public dom::element {
  REGISTER_DECLARATION(syllable);

public:
  syllable(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class syllable_mute : public dom::element {
  REGISTER_DECLARATION(syllable_mute);

public:
  syllable_mute(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class syllable_ref : public dom::element {
  REGISTER_DECLARATION(syllable_ref);

public:
  syllable_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class tie : public dom::element {
  REGISTER_DECLARATION(tie);

public:
  tie(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string value() const;
  void value(std::string const&);
};

class tie_ref : public dom::element {
  REGISTER_DECLARATION(tie_ref);

public:
  tie_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  std::string start_ref() const;
  void start_ref(std::string const&);

  start_stop type() const;
  void type(start_stop);
};

class ties : public dom::element {
  REGISTER_DECLARATION(ties);

public:
  ties(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class time_signature : public dom::element {
  REGISTER_DECLARATION(time_signature);

public:
  time_signature(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> single_number() const;
  void single_number(optional<bool>);

  optional<bool> figure() const;
  void figure(optional<bool>);

  std::string values() const;
  void values(std::string const&);
};

class tremolo : public dom::element {
  REGISTER_DECLARATION(tremolo);

public:
  tremolo(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  std::string value() const;
  void value(std::string const&);
};

class tremolo_ref : public dom::element {
  REGISTER_DECLARATION(tremolo_ref);

public:
  tremolo_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class tuplet : public dom::element {
  REGISTER_DECLARATION(tuplet);

public:
  tuplet(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  optional<bool> doubled() const;
  void doubled(optional<bool>);

  std::string value() const;
  void value(std::string const&);
};

class tuplet_ref : public dom::element {
  REGISTER_DECLARATION(tuplet_ref);

public:
  tuplet_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  start_stop_continue type() const;
  void type(start_stop_continue);

  std::string notes() const;
  void notes(std::string const&);
};

class tuplets : public dom::element {
  REGISTER_DECLARATION(tuplets);

public:
  tuplets(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
};

class unknown : public dom::element {
  REGISTER_DECLARATION(unknown);

public:
  unknown(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);
};

class value_prefix : public dom::element {
  REGISTER_DECLARATION(value_prefix);

public:
  value_prefix(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

  std::string id() const;
  void id(std::string const&);

  value_prefix_t value() const;
  void value(value_prefix_t);
};

template<typename T>
typename std::enable_if<std::is_base_of<dom::element, T>::value, std::ostream&>::type
operator<<(std::ostream &os, std::shared_ptr<T> e) {
  if (!std::dynamic_pointer_cast<note_data>(e) &&
      !std::dynamic_pointer_cast<rest_data>(e) &&
      !std::dynamic_pointer_cast<score_header>(e))
  {
    auto const& text = e->text();
    if (text.empty()) for (auto c : *e) os << c; else os << text;
  }

  return os;
}

} // namespace bmml

#endif
