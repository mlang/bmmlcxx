# $ python3 -c "from codegen import *; hpp()"
# $ python3 -c "from codegen import *; cpp()"

from jinja2 import DictLoader, Environment
from lxml.etree import DTD

bmml = DTD('bmml.dtd')

CLASS_HEADER = """
class {{class}} : public dom::element {
  REGISTER_DECLARATION({{class}});

public:
  {{class}}(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {
  }
"""

REQUIRED_STRING_ATTRIBUTE_DECLARATION = """
  std::string {{method}}() const;
  void {{method}}(std::string const&);
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

IMPLIED_BOOL_ATTRIBUTE_DECLARATION = """
  optional<bool> {{method}}() const;
  void {{method}}(optional<bool>);
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

ENUMERATION_ATTRIBUTE_DECLARATION = """
  {{type}} {{method}}() const;
  void {{method}}({{type}});
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

IMPLIED_STRING_ATTRIBUTE_DECLARATION = """
  optional<std::string> {{method}}() const;
  void {{method}}(optional<std::string>);
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

def mangle(name):
  return {'8th_or_128th': 'eighth_or_128th',
          '256th': 'twohundredfiftysixth',
          'continue': 'continue_'}.get(name, name)

templates.filters['mangle'] = mangle

def template(name):
  return templates.get_template(name)

forwards = {
  'ornament': """
class ornament_type;
""",
  'score': """
class score_header;
class score_data;
"""
}

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

def enum_defs():
  for key, value in enumerations.items():
    print("""
enum class {name} {{
  {values}
}};
""".format(name=value['name'],
           values=',\n  '.join(map(mangle, key))), end='')

def enumeration_declaration(method_name, values, default):
  if values in enumerations:
    type = enumerations[values]['name']
    if default == 'implied':
      type = """optional<{}>""".format(type)
    print(template('ENUMERATION_ATTRIBUTE_DECLARATION').render(
            {'type': type, 'method': method_name}))

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

def hpp():
  enum_defs()
  for e in bmml.iterelements():
    if e.name in forwards:
      print(forwards[e.name])
    print(template('CLASS_HEADER').render({'class': e.name}))
    for a in e.iterattributes():
      if (a.type == 'id' or a.type == 'cdata' or a.type == 'idref') and a.default == 'required':
        print(template('REQUIRED_STRING_ATTRIBUTE_DECLARATION').render({'method': a.name}))
      elif a.type == 'cdata' and a.default == 'implied':
        print(template('IMPLIED_STRING_ATTRIBUTE_DECLARATION').render({'method': a.name}))
      elif a.type == 'enumeration' and a.values() != ['true','false']:
        enumeration_declaration(a.name, tuple(a.values()), a.default)
      elif a.type == 'enumeration' and a.default == 'implied' and a.values() == ['true', 'false']:
        print(template('IMPLIED_BOOL_ATTRIBUTE_DECLARATION').render({'method':  a.name}))
    if e.name in methods:
      print(methods[e.name]['declaration'], end='')
    print("};")

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

def list():
  for e in bmml.iterelements():
    print(e.name, e.type)
    for a in e.iterattributes():
      print("  ", a.name, a.type, a.default, a.default_value)


