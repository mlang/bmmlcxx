# $ python3 -c "from codegen import *; hpp()"
# $ python3 -c "from codegen import *; cpp()"

from lxml.etree import DTD, parse

bmml = DTD('bmml.dtd')

CLASS_HEADER_TEMPLATE = """
class {name} : public dom::element {{
  REGISTER_DECLARATION({name});

public:
  {name}(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {{
  }}
"""

REQUIRED_STRING_ATTRIBUTE_DECLARATION = """
  std::string {name}() const;
  void {name}(std::string const&);
"""
REQUIRED_STRING_ATTRIBUTE_DEFINITION = """
std::string bmml::{class_name}::{name}() const {{
  return attributes().find(qname("{name}"))->second;
}}

void bmml::{class_name}::{name}(std::string const& value) {{
  attributes()[qname("{name}")] = value;
}}
"""

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
  'ornament': {'declaration': """
  std::vector<std::shared_ptr<accidental>> accidentals() const;
  std::shared_ptr<bmml::ornament_type> ornament_type() const;
""", 'definition': """
vector<shared_ptr<bmml::accidental>> bmml::ornament::accidentals() const {
  return find_elements<bmml::accidental>();
}

shared_ptr<bmml::ornament_type> bmml::ornament::ornament_type() const {
  return find_element<bmml::ornament_type>();
}
"""},
  'score': {'declaration': """
  std::shared_ptr<score_header> header() const;
  std::shared_ptr<score_data> data() const;
""", 'definition': """
shared_ptr<bmml::score_header> bmml::score::header() const {
  return find_element<bmml::score_header>();
}

shared_ptr<bmml::score_data> bmml::score::data() const {
  return find_element<bmml::score_data>();
}
"""}
}

enumerations = {
('up', 'down'): {'name': 'up_down'},
('8th_or_128th', 'quarter_or_64th', 'half_or_32nd', 'whole_or_16th',
 'brevis', 'longa'): {'name': 'ambiguous_value'},
('A', 'B', 'C', 'D', 'E', 'F', 'G'): {'name': 'diatonic_step'},
('start', 'stop'): {'name': 'start_stop'},
('left', 'right'): {'name': 'hand_type'}
}

def mangle(name):
  if name == '8th_or_128th':
    name = 'eighth_or_128th'
  return name

def enum_defs():
  for key, value in enumerations.items():
    print("""
enum class {name} {{
  {values}
}};
""".format(name=value['name'],
           values=',\n  '.join(map(mangle, key))), end='')

def required_enumeration_declaration(method_name, values):
  if values in enumerations:
    enum_name = enumerations[values]['name']
    print("""
  {enum_name} {method_name}() const;
  void {method_name}({enum_name});
""".format(enum_name=enum_name, method_name=method_name), end='')

def required_enumeration_definition(class_name, method_name, values):
  if values in enumerations:
    enum_name = enumerations[values]['name']
    print("""
bmml::{enum_name} bmml::{class_name}::{method_name}() const {{
  auto iter = attributes().find(qname("{attribute_name}"));
  if (iter != attributes().end()) {{
""".format(enum_name=enum_name,
           class_name=class_name,
           method_name=method_name,
           attribute_name=method_name), end='')
    first = True
    for enum in values:
      if first:
        print("         if ", end='')
      else:
        print("    else if ", end='')
      print("""(iter->second == "{name}") return {enum_name}::{enum};""".format(name=enum, enum_name=enum_name, enum=mangle(enum)))
      first = False
    print("""
    throw illegal_enumeration();
  }
  throw missing_attribute();
}
""", end='')

    #--------------------------------------------------------------------------

    print("""
void bmml::{class_name}::{method_name}(bmml::{enum_name} value) {{
  switch (value) {{
""".format(enum_name=enum_name, method_name=method_name, class_name=class_name), end='')
    for enum in values:
      print("""  case bmml::{enum_name}::{enum}:
    attributes()[qname("{attribute_name}")] = "{text}";
    break;
""".format(enum_name=enum_name, enum=mangle(enum), text=enum, attribute_name=method_name), end='')
    print("""
  default:
    throw illegal_enumeration();
  }
}
""")

def hpp():
  enum_defs()
  for e in bmml.iterelements():
    if e.name in forwards:
      print(forwards[e.name])
    print(CLASS_HEADER_TEMPLATE.format(name = e.name), end='')
    for a in e.iterattributes():
      if (a.type == 'id' or a.type == 'cdata' or a.type == 'idref') and a.default == 'required':
        print(REQUIRED_STRING_ATTRIBUTE_DECLARATION.format(class_name = e.name, name = a.name), end='')
      elif a.type == 'enumeration' and a.default == 'required':
        required_enumeration_declaration(a.name, tuple(a.values()))
    if e.name in methods:
      print(methods[e.name]['declaration'])
    print("};")

REGISTRATION_TEMPLATE = """REGISTER_DEFINITION({name}, qname("{name}"), content::{type});"""

def cpp():
  for e in bmml.iterelements():
    type = e.type
    if type == 'mixed':
      type = 'simple'
    if type == 'element':
      type = 'complex'
    if type == 'any':
      type = 'mixed'

    print(REGISTRATION_TEMPLATE.format(name = e.name, type = type))
    for a in e.iterattributes():
      if (a.type == 'id' or a.type == 'cdata' or a.type == 'idref') and a.default == 'required':
        print(REQUIRED_STRING_ATTRIBUTE_DEFINITION.format(class_name = e.name, name = a.name))
      elif a.type == 'enumeration' and a.default == 'required':
        required_enumeration_definition(e.name, a.name, tuple(a.values()))
    if e.name in methods:
      print(methods[e.name]['definition'])

def list():
  for e in bmml.iterelements():
    print(e.name, e.type)
    for a in e.iterattributes():
      print("  ", a.name, a.type, a.default, a.default_value)


