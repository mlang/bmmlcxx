#!/usr/bin/env python3
from lxml.etree import DTD, parse

bmml = DTD('bmml.dtd')
CLASS_HEADER_TEMPLATE = """
class {name} : public dom::element {{
  REGISTER_DECLARATION({name});

public:
  {name}(xml::parser& p, bool start_end = true) : dom::element(p, start_end) {{
  }}

"""

REQUIRED_STRING_ATTRIBUTE_TEMPLATE = """
  std::string {name}() const {{
    return attributes().find(xml::qname("{name}"))->second;
  }}
  void {name}(std::string const& value) {{
    attributes()[xml::qname("{name}")] = value;
  }}
"""

def hpp():
  for e in bmml.iterelements():
    print(CLASS_HEADER_TEMPLATE.format(name = e.name))
    for a in e.iterattributes():
      if a.name == 'id' and a.type == 'id' and a.default == 'required':
        print(REQUIRED_STRING_ATTRIBUTE_TEMPLATE.format(name = "id"))
    print("};")

REGISTRATION_TEMPLATE = """REGISTER_DEFINITION({name}, qname("{name}"), content::{type});"""

def cpp():
  for e in bmml.iterelements():
    type = e.type
    if type == 'mixed':
      type = 'simple'
    if type == 'element':
      type = 'complex'

    print(REGISTRATION_TEMPLATE.format(name = e.name, type = type))


hpp()
cpp()

