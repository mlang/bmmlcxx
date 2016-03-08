#!/usr/bin/env python
from __future__ import print_function
from jinja2 import DictLoader, Environment

LIBRARY_HEADER = """
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

class element : public std::enable_shared_from_this<element> {
public:
  using attributes_type = std::map<xml::qname, std::string>;
  using elements_type = std::vector<std::shared_ptr<element>>;

  element(const xml::qname& name) : tag_name_(name) {}
  virtual ~element() = default;

  xml::qname const& tag_name() const { return tag_name_; }

  attributes_type const& attributes() const { return attributes_; }
  attributes_type&       attributes()       { return attributes_; }

  std::string const& text() const { return text_; }

  void text(std::string const& text) { text_ = text; }

  elements_type const& elements() const {return elements_;}
  elements_type&       elements()       { return elements_; }

  elements_type::const_iterator begin() const { return elements_.begin(); }
  elements_type::const_iterator end() const { return elements_.end(); }
  elements_type::iterator begin() { return elements_.begin(); }
  elements_type::iterator end() { return elements_.end(); }

  element *parent() { return parent_; }
  element const *parent() const { return parent_; }

  // Parse an element. If start_end is false, then don't parse the
  // start and end of the element.
  //
  element (xml::parser&, bool start_end = true, element *parent = nullptr);

  // Serialize an element. If start_end is false, then don't serialize
  // the start and end of the element.
  //
  void serialize (xml::serializer&, bool start_end = true) const;

  // Utilities
  //
  template<typename T> auto as() const {
    return std::dynamic_pointer_cast<T const>(shared_from_this());
  }
  template<typename T> auto as() {
    return std::dynamic_pointer_cast<T>(shared_from_this());
  }

  template<typename T> bool is() const {
    return dynamic_cast<T const *>(this) != nullptr;
  }
  template<typename T, typename U, typename... More> bool is() const {
    return is<T>() || is<U, More...>();
  }

  template<typename T> std::shared_ptr<T> find_element() const {
    for (auto &&e : elements_)
      if (auto t = e->as<T>()) return t;
    return {};
  }

  template<typename T> std::vector<std::shared_ptr<T>> find_elements() const {
    std::vector<std::shared_ptr<T>> result;
    for (auto &&e : elements_)
      if (auto t = e->as<T>())
        result.push_back(std::move(t));

    return result;
  }

  template<typename T> static std::shared_ptr<element> create(xml::parser& p, element *parent) {
    return std::make_shared<T>(p, false, parent);
  }

private:
  xml::qname tag_name_;
  attributes_type attributes_;
  std::string text_;           // Simple content only.
  elements_type elements_;     // Complex content only.
  element *parent_;
};

class factory {
public:
  static std::shared_ptr<element> make(xml::parser& parser, element* parent);

protected:
  struct element_info {
    xml::content content_type;
    std::shared_ptr<element> (*create)(xml::parser&, element *parent);
  };

  using map_type = std::map<xml::qname, element_info>;

  static map_type *get_map() {
    if (!map) map = new map_type;

    return map;
  }

private:
  static map_type *map;
};

template<typename T>
struct register_element : factory {
  register_element(xml::qname const& name, xml::content const& content) {
    get_map()->insert({name, element_info{content, &element::create<T>}});
  }
};

#define REGISTER_DECLARATION(NAME)                       \\
static dom::register_element<NAME> _factory_registration;

#define REGISTER_DEFINITION(NAME, QNAME, CONTENT)        \\
bmml::dom::register_element<bmml::NAME>                  \\
bmml::NAME::_factory_registration{QNAME, CONTENT}

class id : public std::string {
public:
  id(std::string const& id) : std::string{id} {}
};

}

class with_id : public dom::element {
protected:
  with_id(xml::parser& p, bool start_end = true, dom::element *parent = nullptr)
  : dom::element(p, start_end, parent) {
  }

public:
  dom::id id() const;
  void id(dom::id const&);
};

namespace dom {
        
class id_ref : public std::string {
  element *toplevel;
public:
  id_ref(std::string const& id, dom::element *toplevel)
  : std::string{id}, toplevel{toplevel} {}

  std::shared_ptr<bmml::with_id> operator->() const {
    if (auto e = toplevel->as<with_id>())
      if (e->id() == *this) return e;
                
    for (auto child : *toplevel) {
      auto ref = id_ref{static_cast<std::string>(*this), child.get()};
      if (auto e = ref.operator->())
        return std::move(e);
    }

    return {};
  }
};

class id_ref_to_const : public std::string {
  element const *toplevel;
public:
  id_ref_to_const(std::string const& id, dom::element const *toplevel)
  : std::string{id}, toplevel{toplevel} {}
          
  std::shared_ptr<bmml::with_id const> operator->() const {
    if (auto e = toplevel->as<with_id>())
      if (e->id() == *this) return e;
                          
    for (auto child : *toplevel) {
      auto ref = id_ref_to_const{static_cast<std::string>(*this), child.get()};
      if (auto e = ref.operator->()) return std::move(e);
    }
                              
    return {};
  }
};

} // namespace dom

class with_id_ref : public dom::element {
protected:
  with_id_ref(xml::parser& p, bool start_end = true, dom::element *parent = nullptr)
  : dom::element(p, start_end, parent) {
  }
          
public:
  dom::id_ref_to_const id() const;
  dom::id_ref id();
  void id(dom::id_ref const&);
};

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

{%- for enum_class, values in enum_classes %}
enum class {{enum_class}} {
  {{ values | map('mangle') | join(',\n  ') }}
};
{% endfor %}

{%- for elem in dtd.iterelements() %}
  {%- if elem.name in forwards_for %}
    {%- for forward in forwards_for[elem.name] %}
class {{forward}};
    {%- endfor %}
  {%- endif %}

  {%- set parent_class = superclass(elem) %}
class {{elem.name}} final : public {{parent_class}} {
  REGISTER_DECLARATION({{elem.name}});

public:
  {{elem.name}}(xml::parser& p, bool start_end = true, dom::element *parent = nullptr)
  : {{parent_class}}(p, start_end, parent) {
  }

  {%- for attr in elem.iterattributes() %}
    {%- if attr is required_string_attribute %}

  std::string {{attr.name}}() const;
  void {{attr.name}}(std::string const&);

    {%- elif attr is implied_string_attribute %}

  optional<std::string> {{attr.name}}() const;
  void {{attr.name}}(optional<std::string>);

    {%- elif attr is boolean_enumeration_attribute and attr.default == 'implied' %}

  optional<bool> {{attr.name}}() const;
  void {{attr.name}}(optional<bool>);

    {%- elif attr is known_enumeration_attribute %}
      {%- set type = enumerations[tuple(attr.values())]['name'] %}
      {%- if attr.default == 'implied' %}
        {%- set type = "optional<" + type + ">" %}
      {%- endif %}

  {{type}} {{attr.name}}() const;
  void {{attr.name}}({{type}});

    {%- endif %}
  {%- endfor %}
  {%- if elem.name in extra_methods %}
{{ extra_methods[elem.name]['declaration'] }}
  {%- endif %}
};
{%- endfor %}

// ------------------------------------------------------------------------- //

std::shared_ptr<score> parse(std::istream&, std::string const& name);

template<typename T>
typename std::enable_if<std::is_base_of<dom::element, T>::value, std::ostream&>::type
operator<<(std::ostream &out, std::shared_ptr<T> elem) {
  if (!elem->template is<note_data, rest_data, score_header>()) {
    auto const& text = elem->text();
    if (text.empty()) for (auto child : *elem) out << child; else out << text;
  }

  return out;
}

} // namespace bmml

#endif
"""

LIBRARY_IMPLEMENTATION = """
#include "bmml.hxx"

#include <iostream>
#include <xml/parser>
#include <xml/serializer>

#include <boost/lexical_cast.hpp>

using std::make_shared;
using std::shared_ptr;
using std::vector;
using xml::content;
using xml::parser;
using xml::parsing;
using xml::qname;
using xml::serializer;
using bmml::optional;

bmml::dom::factory::map_type *bmml::dom::factory::map{};

namespace {

bool whitespace (const std::string& s) {
  for (char c : s) {
    if (c != 0x20 && c != 0x0A && c != 0x0D && c != 0x09) return false;
  }

  return true;
}

} // namespace

bmml::dom::element::element(parser& p, bool start_end, bmml::dom::element *parent)
: parent_{parent} {
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

      elements_.push_back(dom::factory::make(p, this));
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

shared_ptr<bmml::dom::element> bmml::dom::factory::make(xml::parser& p, bmml::dom::element *parent) {
  auto name = p.qname();
  auto iter = get_map()->find(name);
  if (iter == get_map()->end()) {
    return std::make_shared<element>(p, false, parent);
  }

  auto const& element = iter->second;
  auto content = element.content_type;

  // WORKAROUND: Some BMML documents in the wild are not conforming to BMML 0.8
  // insofar as they have a barline element with simple content (no barline_type
  // subelement).  This trips up the parser since according to the DTD,
  // barline should be of complex type.
  if (name == qname{"barline"} &&
      p.attribute_map().find(qname{"value"}) != p.attribute_map().end()) {
    content = content::simple;
  }

  p.content(content);
  return element.create(p, parent);
}

bmml::dom::id bmml::with_id::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) return dom::id{iter->second};
            
  throw missing_attribute{};
}

void bmml::with_id::id(dom::id const& value) {
  attributes()[qname{"id"}] = value;
}

bmml::dom::id_ref_to_const bmml::with_id_ref::id() const {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) {
    auto parent = this->parent();
    while (parent && parent->parent()) parent = parent->parent();
    return dom::id_ref_to_const{iter->second, parent};
  }            
  throw missing_attribute{};
}

bmml::dom::id_ref bmml::with_id_ref::id() {
  auto iter = attributes().find(qname{"id"});
  if (iter != attributes().end()) {
    auto parent = this->parent();
    while (parent && parent->parent()) parent = parent->parent();
    return dom::id_ref{iter->second, parent};
  }
  throw missing_attribute{};
}

void bmml::with_id_ref::id(dom::id_ref const& value) {
  attributes()[qname{"id"}] = value;
}

{% for elem in dtd.iterelements() %}
REGISTER_DEFINITION({{elem.name}}, qname("{{elem.name}}"), content::{{elem | content_type}});

  {%- for attr in elem.iterattributes() %}
    {%- if attr is required_string_attribute %}

std::string bmml::{{elem.name}}::{{attr.name}}() const {
  auto iter = attributes().find(qname{"{{attr.name}}"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::{{elem.name}}::{{attr.name}}(std::string const& value) {
  attributes()[qname{"{{attr.name}}"}] = value;
}

    {%- elif attr is implied_string_attribute %}

optional<std::string> bmml::{{elem.name}}::{{attr.name}}() const {
  static const qname attr{"{{attr.name}}"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::{{elem.name}}::{{attr.name}}(optional<std::string> opt_value) {
  static const qname attr{"{{attr.name}}"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
}

    {%- elif attr is boolean_enumeration_attribute and attr.default == 'implied' %}

optional<bool> bmml::{{elem.name}}::{{attr.name}}() const {
  static const qname attr{"{{attr.name}}"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::{{elem.name}}::{{attr.name}}(optional<bool> opt_value) {
  static const qname attr{"{{attr.name}}"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}

    {%- elif attr is known_enumeration_attribute %}
      {%- set enum = enumerations[tuple(attr.values())]['name'] %}
      {%- if attr.default == 'required' %}

bmml::{{enum}} bmml::{{elem.name}}::{{attr.name}}() const {
  auto iter = attributes().find(qname{"{{attr.name}}"});

  if (iter != attributes().end()) {
        {%- for value in attr.values() %}
    {% if not loop.first %}else {% else %}     {% endif -%}
    if (iter->second == "{{value}}") return {{enum}}::{{value | mangle}};
        {%- endfor %}

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::{{elem.name}}::{{attr.name}}(bmml::{{enum}} value) {
  static qname const attr{"{{attr.name}}"};

  switch (value) {
        {%- for value in attr.values() %}
  case {{enum}}::{{value | mangle}}:
    attributes()[attr] = "{{value}}";
    break;
        {%- endfor %}

  default:
    throw illegal_enumeration{};
  }
}

      {%- elif attr.default == 'implied' %}

optional<bmml::{{enum}}> bmml::{{elem.name}}::{{attr.name}}() const {
  auto iter = attributes().find(qname{"{{attr.name}}"});

  if (iter != attributes().end()) {
        {%- for value in attr.values() %}
    {% if not loop.first %}else {% else %}     {% endif -%}
    if (iter->second == "{{value}}") return { {{enum}}::{{value | mangle}} };
        {%- endfor %}
  }

  return {};
}

void bmml::{{elem.name}}::{{attr.name}}(optional<bmml::{{enum}}> opt_value) {
  static const qname attr{"{{attr.name}}"};

  if (opt_value) {
    switch (*opt_value) {
        {%- for value in attr.values() %}
    case {{enum}}::{{value | mangle}}:
      attributes()[attr] = "{{value}}";
      break;
        {%- endfor %}

    default:
      throw illegal_enumeration{};
    }
  } else {
    attributes().erase(attr);
  }
}

      {%- endif %}
    {%- endif %}
  {%- endfor %}
  {%- if elem.name in extra_methods -%}
    {{ extra_methods[elem.name]['definition'] }}
  {%- endif %}
{% endfor %}
std::shared_ptr<bmml::score> bmml::parse(std::istream& in, std::string const& name) {
  parser p{in, name};
        
  p.next_expect(parser::start_element, "score", content::complex);
  auto result = make_shared<bmml::score>(p, false);
  p.next_expect(parser::end_element, "score");

  return result;
}
"""

PCDATA_OPERATOR_DECLARATION = """
  operator {{type}}() const;
  {{class}}& operator=({{type}});
"""

PCDATA_OPERATOR_DEFINITION = """
bmml::{{class}}::operator {{type}}() const {
  return boost::lexical_cast<{{type}}>(text());
}

bmml::{{class}}& bmml::{{class}}::operator=({{type}} value) {
  text(boost::lexical_cast<std::string>(value));
  return *this;
}
"""

templates = Environment(loader=DictLoader(globals()))

from lxml.etree import DTD

enumerations = {
  ('full', 'part', 'division'):
  {'name': 'inaccord_t'},
  ('left_toe', 'left_heel', 'right_toe', 'right_heel'):
  {'name': 'organ_pedal_t'},
  ('full', 'half', 'caesura'):
  {'name': 'full_half_caesura'},
  ('full', 'half', 'vertical'):
  {'name': 'full_half_vertical'},
  ('glissando', 'start', 'stop'):
  {'name': 'glissando_start_stop'},
  ('natural', 'artificial'):
  {'name': 'natural_artificial'},
  ('up', 'down'):
  {'name': 'up_down'},
  ('above', 'below'):
  {'name': 'above_below'},
  ('8th_or_128th', 'quarter_or_64th', 'half_or_32nd', 'whole_or_16th',
   'brevis', 'longa'):
  {'name': 'ambiguous_value'},
  ('A', 'B', 'C', 'D', 'E', 'F', 'G'):
  {'name': 'diatonic_step'},
  ('start', 'stop'):
  {'name': 'start_stop'},
  ('start', 'stop', 'continue'):
  {'name': 'start_stop_continue'},
  ('left', 'right'):
  {'name': 'left_right'},
  ('left', 'middle', 'right'):
  {'name': 'left_middle_right'},
  ('separator', 'large', 'small', '256th'):
  {'name': 'value_prefix_t'}
}

templates.filters['content_type'] = lambda e: \
  {'mixed': 'simple', 'element': 'complex', 'any': 'mixed'}.get(e.type, e.type)
templates.filters['mangle'] = lambda ident: \
  {'8th_or_128th': 'eighth_or_128th',
   '256th': 'twohundredfiftysixth',
   'continue': 'continue_'}.get(ident, ident)
templates.globals['tuple'] = tuple
templates.globals['superclass'] = lambda elem: \
  next(({'id': 'with_id', 'idref': 'with_id_ref'}.get(a.type)
        for a in elem.iterattributes() if a.name == 'id'),
       'dom::element')
templates.tests['required_id_attribute'] = lambda a: \
  a.type == 'id' and a.default == 'required'
templates.tests['required_idref_attribute'] = lambda a: \
  a.type == 'idref' and a.default == 'required'
templates.tests['required_string_attribute'] = lambda a: \
  a.type == 'cdata' and a.default == 'required'
templates.tests['implied_string_attribute'] = lambda a: \
  a.type in ['id', 'cdata', 'idref'] and a.default == 'implied'
templates.tests['boolean_enumeration_attribute'] = lambda a: \
  a.type == 'enumeration' and a.values() == ['true', 'false']
templates.tests['known_enumeration_attribute'] = lambda a: \
  a.type == 'enumeration' and tuple(a.values()) in enumerations

def template(name):
  return templates.get_template(name)

methods = {
  'ornament': {
    'declaration': """
  std::vector<std::shared_ptr<accidental>> accidentals() const;
  std::shared_ptr<bmml::ornament_type> ornament_type() const;
""",
    'definition': """
vector<shared_ptr<bmml::accidental>> bmml::ornament::accidentals() const {
  return find_elements<bmml::accidental>();
}

shared_ptr<bmml::ornament_type> bmml::ornament::ornament_type() const {
  return find_element<bmml::ornament_type>();
}
"""
  },
  'score': {
    'declaration': """
  std::shared_ptr<score_header> header() const;
  std::shared_ptr<score_data> data() const;
""",
    'definition': """
shared_ptr<bmml::score_header> bmml::score::header() const {
  return find_element<bmml::score_header>();
}

shared_ptr<bmml::score_data> bmml::score::data() const {
  return find_element<bmml::score_data>();
}
"""}
}

for element in ['alteration', 'duration', 'pitch']:
  methods[element] = {
    'declaration': template('PCDATA_OPERATOR_DECLARATION').render(
      {'class': element, 'type': 'int'}),
    'definition': template('PCDATA_OPERATOR_DEFINITION').render(
      {'class': element, 'type': 'int'})
  }

if __name__ == '__main__':
  import argparse

  cmdline = argparse.ArgumentParser()
  cmdline.add_argument("dtd")
  cmdline.add_argument("hxx")
  cmdline.add_argument("cxx")
  args = cmdline.parse_args()
  dtd = DTD(args.dtd)
  metadata = {
    'dtd': dtd,
    'enumerations': enumerations,
    'extra_methods': methods,
    'enum_classes': sorted([(v['name'], k) for k, v in enumerations.items()
                            if not v in [e.name for e in dtd.iterelements()]]),
    'forwards_for': {'ornament': ['ornament_type'],
                     'score': ['score_data', 'score_header']}
  }
  with open(args.hxx, 'w') as hxx:
    print(template('LIBRARY_HEADER').render(metadata), file=hxx)
  with open(args.cxx, 'w') as cxx:
    print(template('LIBRARY_IMPLEMENTATION').render(metadata), file=cxx)

