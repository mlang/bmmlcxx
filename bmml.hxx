#ifndef BMML_HXX
#define BMML_HXX

#include <xml/content>
#include <xml/parser>
#include <xml/qname>
#include <xml/serializer>

#include <map>
#include <memory>
#include <string>
#include <vector>

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

  template<typename T> std::shared_ptr<T> find_element() const {
    for (auto &&e : elements_)
      if (auto t = std::dynamic_pointer_cast<T>(e)) return t;
    return {};
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

// Start of schema specific element classes
//
//---------------------------------------------------------------------------//

class abbr_name : public dom::element {
  REGISTER_DECLARATION(abbr_name);

public:
  abbr_name(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class accidental : public dom::element {
  REGISTER_DECLARATION(accidental);

public:
  accidental(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class accordion_register : public dom::element {
  REGISTER_DECLARATION(accordion_register);

public:
  accordion_register(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class accordion_row : public dom::element {
  REGISTER_DECLARATION(accordion_row);

public:
  accordion_row(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class alteration : public dom::element {
  REGISTER_DECLARATION(alteration);

public:
  alteration(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

};

class alternation : public dom::element {
  REGISTER_DECLARATION(alternation);

public:
  alternation(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class alternation_ref : public dom::element {
  REGISTER_DECLARATION(alternation_ref);

public:
  alternation_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class appoggiatura_ref : public dom::element {
  REGISTER_DECLARATION(appoggiatura_ref);

public:
  appoggiatura_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class barline : public dom::element {
  REGISTER_DECLARATION(barline);

public:
  barline(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class barline_type : public dom::element {
  REGISTER_DECLARATION(barline_type);

public:
  barline_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class barre : public dom::element {
  REGISTER_DECLARATION(barre);

public:
  barre(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class bow : public dom::element {
  REGISTER_DECLARATION(bow);

public:
  bow(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class breath : public dom::element {
  REGISTER_DECLARATION(breath);

public:
  breath(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class chord : public dom::element {
  REGISTER_DECLARATION(chord);

public:
  chord(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class chord_type : public dom::element {
  REGISTER_DECLARATION(chord_type);

public:
  chord_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string notes() const {
    return attributes().find(xml::qname("notes"))->second;
  }
  void notes(std::string const& value) {
    attributes()[xml::qname("notes")] = value;
  }


  std::string root() const {
    return attributes().find(xml::qname("root"))->second;
  }
  void root(std::string const& value) {
    attributes()[xml::qname("root")] = value;
  }

};

class clef : public dom::element {
  REGISTER_DECLARATION(clef);

public:
  clef(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string name() const {
    return attributes().find(xml::qname("name"))->second;
  }
  void name(std::string const& value) {
    attributes()[xml::qname("name")] = value;
  }

};

class coda : public dom::element {
  REGISTER_DECLARATION(coda);

public:
  coda(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class dot : public dom::element {
  REGISTER_DECLARATION(dot);

public:
  dot(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class duration : public dom::element {
  REGISTER_DECLARATION(duration);

public:
  duration(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

};

class dynamic : public dom::element {
  REGISTER_DECLARATION(dynamic);

public:
  dynamic(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class editorial_mark : public dom::element {
  REGISTER_DECLARATION(editorial_mark);

public:
  editorial_mark(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class ending : public dom::element {
  REGISTER_DECLARATION(ending);

public:
  ending(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class fingering : public dom::element {
  REGISTER_DECLARATION(fingering);

public:
  fingering(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class foot_crossing : public dom::element {
  REGISTER_DECLARATION(foot_crossing);

public:
  foot_crossing(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class generic_text : public dom::element {
  REGISTER_DECLARATION(generic_text);

public:
  generic_text(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class hand : public dom::element {
  REGISTER_DECLARATION(hand);

public:
  hand(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class harmonic : public dom::element {
  REGISTER_DECLARATION(harmonic);

public:
  harmonic(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class inaccord : public dom::element {
  REGISTER_DECLARATION(inaccord);

public:
  inaccord(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class interval : public dom::element {
  REGISTER_DECLARATION(interval);

public:
  interval(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class interval_type : public dom::element {
  REGISTER_DECLARATION(interval_type);

public:
  interval_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class line_of_continuation : public dom::element {
  REGISTER_DECLARATION(line_of_continuation);

public:
  line_of_continuation(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class lyric : public dom::element {
  REGISTER_DECLARATION(lyric);

public:
  lyric(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class lyric_prefix : public dom::element {
  REGISTER_DECLARATION(lyric_prefix);

public:
  lyric_prefix(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class lyric_repeat : public dom::element {
  REGISTER_DECLARATION(lyric_repeat);

public:
  lyric_repeat(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class lyric_repetition : public dom::element {
  REGISTER_DECLARATION(lyric_repetition);

public:
  lyric_repetition(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class metronome_equal : public dom::element {
  REGISTER_DECLARATION(metronome_equal);

public:
  metronome_equal(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class metronome_note_type : public dom::element {
  REGISTER_DECLARATION(metronome_note_type);

public:
  metronome_note_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class metronome_value : public dom::element {
  REGISTER_DECLARATION(metronome_value);

public:
  metronome_value(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class midi_instrument : public dom::element {
  REGISTER_DECLARATION(midi_instrument);

public:
  midi_instrument(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class midi_metronome : public dom::element {
  REGISTER_DECLARATION(midi_metronome);

public:
  midi_metronome(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class name : public dom::element {
  REGISTER_DECLARATION(name);

public:
  name(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class newline : public dom::element {
  REGISTER_DECLARATION(newline);

public:
  newline(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class note : public dom::element {
  REGISTER_DECLARATION(note);

public:
  note(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class note_type : public dom::element {
  REGISTER_DECLARATION(note_type);

public:
  note_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class nuance : public dom::element {
  REGISTER_DECLARATION(nuance);

public:
  nuance(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class nuance_ref : public dom::element {
  REGISTER_DECLARATION(nuance_ref);

public:
  nuance_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class octave : public dom::element {
  REGISTER_DECLARATION(octave);

public:
  octave(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class organ_pedal : public dom::element {
  REGISTER_DECLARATION(organ_pedal);

public:
  organ_pedal(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class ornament : public dom::element {
  REGISTER_DECLARATION(ornament);

public:
  ornament(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class ornament_type : public dom::element {
  REGISTER_DECLARATION(ornament_type);

public:
  ornament_type(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class part : public dom::element {
  REGISTER_DECLARATION(part);

public:
  part(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class part_data : public dom::element {
  REGISTER_DECLARATION(part_data);

public:
  part_data(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class pedal : public dom::element {
  REGISTER_DECLARATION(pedal);

public:
  pedal(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class pitch : public dom::element {
  REGISTER_DECLARATION(pitch);

public:
  pitch(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }

};

class pizzicato : public dom::element {
  REGISTER_DECLARATION(pizzicato);

public:
  pizzicato(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class rasgueado : public dom::element {
  REGISTER_DECLARATION(rasgueado);

public:
  rasgueado(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class repeat : public dom::element {
  REGISTER_DECLARATION(repeat);

public:
  repeat(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class rest : public dom::element {
  REGISTER_DECLARATION(rest);

public:
  rest(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class separator : public dom::element {
  REGISTER_DECLARATION(separator);

public:
  separator(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class shift_line : public dom::element {
  REGISTER_DECLARATION(shift_line);

public:
  shift_line(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class slur : public dom::element {
  REGISTER_DECLARATION(slur);

public:
  slur(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class slur_ref : public dom::element {
  REGISTER_DECLARATION(slur_ref);

public:
  slur_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string start_ref() const {
    return attributes().find(xml::qname("start_ref"))->second;
  }
  void start_ref(std::string const& value) {
    attributes()[xml::qname("start_ref")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class stem : public dom::element {
  REGISTER_DECLARATION(stem);

public:
  stem(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class string : public dom::element {
  REGISTER_DECLARATION(string);

public:
  string(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class string_fingering : public dom::element {
  REGISTER_DECLARATION(string_fingering);

public:
  string_fingering(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class string_position : public dom::element {
  REGISTER_DECLARATION(string_position);

public:
  string_position(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class stroke : public dom::element {
  REGISTER_DECLARATION(stroke);

public:
  stroke(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class syllabic_mute : public dom::element {
  REGISTER_DECLARATION(syllabic_mute);

public:
  syllabic_mute(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class syllabic_parenthesis : public dom::element {
  REGISTER_DECLARATION(syllabic_parenthesis);

public:
  syllabic_parenthesis(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class syllabic_slur : public dom::element {
  REGISTER_DECLARATION(syllabic_slur);

public:
  syllabic_slur(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class syllabic_text : public dom::element {
  REGISTER_DECLARATION(syllabic_text);

public:
  syllabic_text(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class syllable : public dom::element {
  REGISTER_DECLARATION(syllable);

public:
  syllable(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class syllable_mute : public dom::element {
  REGISTER_DECLARATION(syllable_mute);

public:
  syllable_mute(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class syllable_ref : public dom::element {
  REGISTER_DECLARATION(syllable_ref);

public:
  syllable_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class tie : public dom::element {
  REGISTER_DECLARATION(tie);

public:
  tie(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class tie_ref : public dom::element {
  REGISTER_DECLARATION(tie_ref);

public:
  tie_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string start_ref() const {
    return attributes().find(xml::qname("start_ref"))->second;
  }
  void start_ref(std::string const& value) {
    attributes()[xml::qname("start_ref")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string values() const {
    return attributes().find(xml::qname("values"))->second;
  }
  void values(std::string const& value) {
    attributes()[xml::qname("values")] = value;
  }

};

class tremolo : public dom::element {
  REGISTER_DECLARATION(tremolo);

public:
  tremolo(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class tremolo_ref : public dom::element {
  REGISTER_DECLARATION(tremolo_ref);

public:
  tremolo_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class tuplet : public dom::element {
  REGISTER_DECLARATION(tuplet);

public:
  tuplet(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string value() const {
    return attributes().find(xml::qname("value"))->second;
  }
  void value(std::string const& value) {
    attributes()[xml::qname("value")] = value;
  }

};

class tuplet_ref : public dom::element {
  REGISTER_DECLARATION(tuplet_ref);

public:
  tuplet_ref(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }


  std::string notes() const {
    return attributes().find(xml::qname("notes"))->second;
  }
  void notes(std::string const& value) {
    attributes()[xml::qname("notes")] = value;
  }

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


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class value_prefix : public dom::element {
  REGISTER_DECLARATION(value_prefix);

public:
  value_prefix(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }


  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& value) {
    attributes()[xml::qname("id")] = value;
  }

};

class score : public dom::element {
  REGISTER_DECLARATION(score);
public:

  score(xml::parser& p, bool start_end = true) : element(p, start_end) {
  }

  std::string const& version() const {
    return attributes().find(xml::qname("version"))->second;
  }
  void version(std::string const& value) {
    attributes()[xml::qname("version")] = value;
  }

  std::shared_ptr<score_header> header() const { return find_element<score_header>(); }
  std::shared_ptr<score_data> data() const { return find_element<score_data>(); }
};

} // namespace bmml

#endif
