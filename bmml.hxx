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
  
  element(const xml::qname& name) : name_(name) {}
  element(const xml::qname& name, std::string const& text)
  : name_(name), text_(text) {}
  virtual ~element() = default;

  const xml::qname& name() const { return name_; }
  const attributes_type& attributes () const {return attributes_;}

  attributes_type& attributes () { return attributes_; }

  const std::string& text () const {return text_;}

  void text (const std::string& text) {text_ = text;}

  const elements_type& elements () const {return elements_;}
  elements_type& elements () {return elements_;}

  std::shared_ptr<element> find_element_by_name(xml::qname const& name) const {
    for (auto &&e : elements_) if (e->name() == name) return e;
    return nullptr;
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
  xml::qname name_;
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

class rest_type : public dom::element {
  REGISTER_DECLARATION(rest_type);
public:

  rest_type(xml::parser& p, bool start_end = true) : element(p, start_end) {
  }

  std::string id() const {
    return attributes().find(xml::qname("id"))->second;
  }
  void id(std::string const& new_id) {
    attributes()[xml::qname("id")] = new_id;
  }
};

class part : public dom::element {
  REGISTER_DECLARATION(part);
public:

  part(xml::parser& p, bool start_end = true) : element(p, start_end) {
  }

  std::string id() const {
    auto x = attributes().find(xml::qname("id"));
    return x->second;
  }
  void id(std::string const& new_id) {
    attributes()[xml::qname("id")] = new_id;
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

  std::shared_ptr<element> header() const {
    return find_element_by_name(xml::qname("score_header"));
  }
  std::shared_ptr<element> data() const {
    return find_element_by_name(xml::qname("score_data"));
  }
};

} // namespace bmml

#endif
