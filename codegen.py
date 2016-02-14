# $ python3 -c "from codegen import *; hpp()"
# $ python3 -c "from codegen import *; cpp()"

from jinja2 import DictLoader, Environment
from lxml.etree import DTD

bmml = DTD('bmml.dtd')

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

#define REGISTER_DECLARATION(NAME)                       \\
static dom::register_element<NAME> _factory_registration;

#define REGISTER_DEFINITION(NAME, QNAME, CONTENT)        \\
bmml::dom::register_element<bmml::NAME>                  \\
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

class {{elem.name}} : public dom::element {
  REGISTER_DECLARATION({{elem.name}});

public:
  {{elem.name}}(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
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
"""

REQUIRED_STRING_ATTRIBUTE_DEFINITION = """
std::string bmml::{{class}}::{{method}}() const {
  auto iter = attributes().find(qname{"{{attribute}}"});
  if (iter != attributes().end()) return iter->second;

  throw missing_attribute{};
}

void bmml::{{class}}::{{method}}(std::string const& value) {
  attributes()[qname{"{{attribute}}"}] = value;
}
"""

IMPLIED_BOOL_ATTRIBUTE_DEFINITION = """
optional<bool> bmml::{{class}}::{{method}}() const {
  static const qname attr{"{{attribute}}"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) {
         if (iter->second == "true") return {true};
    else if (iter->second == "false") return {false};

    throw illegal_enumeration{};
  }

  return {};
}

void bmml::{{class}}::{{method}}(optional<bool> opt_value) {
  static const qname attr{"{{attribute}}"};

  if (opt_value) {
    attributes()[attr] = *opt_value ? "true" : "false";
  } else {
    attributes().erase(attr);
  }
}
"""

REQUIRED_ENUMERATION_ATTRIBUTE_DEFINITION = """
bmml::{{enum}} bmml::{{class}}::{{method}}() const {
  auto iter = attributes().find(qname{"{{attribute}}"});

  if (iter != attributes().end()) {
{%- for value in values %}
    {% if not loop.first %}else {% else %}     {% endif -%}
    if (iter->second == "{{value}}") return {{enum}}::{{value | mangle}};
{%- endfor %}

    throw illegal_enumeration{};
  }

  throw missing_attribute{};
}

void bmml::{{class}}::{{method}}(bmml::{{enum}} value) {
  switch (value) {
{%- for value in values %}
  case bmml::{{enum}}::{{value | mangle}}:
    attributes()[qname{"{{attribute}}"}] = "{{value}}";
    break;
{%- endfor %}

  default:
    throw illegal_enumeration{};
  }
}
"""

IMPLIED_ENUMERATION_ATTRIBUTE_DEFINITION = """
optional<bmml::{{enum}}> bmml::{{class}}::{{method}}() const {
  auto iter = attributes().find(qname{"{{attribute}}"});

  if (iter != attributes().end()) {
{%- for value in values %}
    {% if not loop.first %}else {% else %}     {% endif -%}
    if (iter->second == "{{value}}") return { {{enum}}::{{value | mangle}} };
{%- endfor %}
  }

  return {};
}

void bmml::{{class}}::{{method}}(optional<bmml::{{enum}}> opt_value) {
  if (opt_value) {
    switch (*opt_value) {
{%- for value in values %}
    case bmml::{{enum}}::{{value | mangle}}:
      attributes()[qname{"{{attribute}}"}] = "{{value}}";
      break;
{%- endfor %}

    default:
      throw illegal_enumeration{};
    }
  } else {
    attributes().erase(qname{"{{attribute}}"});
  }
}
"""

IMPLIED_STRING_ATTRIBUTE_DEFINITION = """
optional<std::string> bmml::{{class}}::{{method}}() const {
  static const qname attr{"{{attribute}}"};

  auto iter = attributes().find(attr);
  if (iter != attributes().end()) return {iter->second};

  return {};
}

void bmml::{{class}}::{{method}}(optional<std::string> opt_value) {
  static const qname attr{"{{attribute}}"};

  if (opt_value) {
    attributes()[attr] = *opt_value;
  } else {
    attributes().erase(attr);
  }
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

REGISTER_DEFINITION = """
REGISTER_DEFINITION({{class}}, qname("{{tag_name}}"), content::{{content_type}});
"""

templates = Environment(loader=DictLoader(globals()))

enumerations = {
('full', 'part', 'division'): {'name': 'inaccord_t'},
('left_toe', 'left_heel', 'right_toe', 'right_heel'): {'name': 'organ_pedal_t'},
('full', 'half', 'caesura'): {'name': 'full_half_caesura'},
('full', 'half', 'vertical'): {'name': 'full_half_vertical'},
('glissando', 'start', 'stop'): {'name': 'glissando_start_stop'},
('natural', 'artificial'): {'name': 'natural_artificial'},
('up', 'down'): {'name': 'up_down'},
('above', 'below'): {'name': 'above_below'},
('8th_or_128th', 'quarter_or_64th', 'half_or_32nd', 'whole_or_16th',
 'brevis', 'longa'): {'name': 'ambiguous_value'},
('A', 'B', 'C', 'D', 'E', 'F', 'G'): {'name': 'diatonic_step'},
('start', 'stop'): {'name': 'start_stop'},
('start', 'stop', 'continue'): {'name': 'start_stop_continue'},
('left', 'right'): {'name': 'left_right'},
('left', 'middle', 'right'): {'name': 'left_middle_right'},
('separator', 'large', 'small', '256th'): {'name': 'value_prefix_t'}
}

def mangle(name):
  return {'8th_or_128th': 'eighth_or_128th',
          '256th': 'twohundredfiftysixth',
          'continue': 'continue_'}.get(name, name)

templates.filters['mangle'] = mangle
templates.globals['tuple'] = tuple
templates.tests['required_string_attribute'] = lambda a: \
  a.type in ['id', 'cdata', 'idref'] and a.default == 'required'
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
    'declaration': template('PCDATA_OPERATOR_DECLARATION').render({'class': element,
                                                                   'type': 'int'}),
    'definition': template('PCDATA_OPERATOR_DEFINITION').render({'class': element,
                                                                 'type': 'int'})
  }

def required_enumeration_definition(class_name, method_name, values):
  if values in enumerations:
    enum_name = enumerations[values]['name']
    print(template('REQUIRED_ENUMERATION_ATTRIBUTE_DEFINITION').render(
      {'class': class_name, 'method': method_name, 'attribute': method_name,
       'enum': enum_name, 'values': values}))

def implied_enumeration_definition(class_name, method_name, values):
  if values in enumerations:
    enum_name = enumerations[values]['name']
    print(template('IMPLIED_ENUMERATION_ATTRIBUTE_DEFINITION').render(
      {'class': class_name, 'method': method_name, 'attribute': method_name,
       'enum': enum_name, 'values': values}))

def cpp():
  for e in bmml.iterelements():
    type = e.type
    if type == 'mixed':
      type = 'simple'
    if type == 'element':
      type = 'complex'
    if type == 'any':
      type = 'mixed'

    print(template('REGISTER_DEFINITION').render({'class': e.name,
                                                  'tag_name': e.name,
                                                  'content_type': type}))
    for a in e.iterattributes():
      if (a.type == 'id' or a.type == 'cdata' or a.type == 'idref') and a.default == 'required':
        print(template('REQUIRED_STRING_ATTRIBUTE_DEFINITION').render({'class': e.name, 'method': a.name, 'attribute': a.name}))
      elif a.type == 'cdata' and a.default == 'implied':
        print(template('IMPLIED_STRING_ATTRIBUTE_DEFINITION').render({'class': e.name, 'method': a.name, 'attribute': a.name}))
      elif a.type == 'enumeration' and a.default == 'implied' and a.values() == ['true', 'false']:
        print(template('IMPLIED_BOOL_ATTRIBUTE_DEFINITION').render({'class': e.name, 'method': a.name, 'attribute': a.name}))
      elif a.type == 'enumeration' and a.default == 'required':
        required_enumeration_definition(e.name, a.name, tuple(a.values()))
      elif a.type == 'enumeration' and a.default == 'implied':
        implied_enumeration_definition(e.name, a.name, tuple(a.values()))
    if e.name in methods:
      print(methods[e.name]['definition'])

def hpp():
  print(template('LIBRARY_HEADER').render(
    {'dtd': bmml,
     'enumerations': enumerations,
     'extra_methods': methods,
     'enum_classes': sorted([(v['name'], k) for k, v in enumerations.items()
                             if not v in [e.name for e in bmml.iterelements()]]),
     'forwards_for': {'ornament': ['ornament_type'],
                      'score': ['score_data', 'score_header']}
    }))

def list():
  for e in bmml.iterelements():
    print(e.name, e.type)
    for a in e.iterattributes():
      print("  ", a.name, a.type, a.default, a.default_value)


