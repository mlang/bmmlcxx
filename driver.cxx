#include <fstream>
#include <iostream>
#include <xml/exception>

#include "bmml.hxx"
#include "spirit_x3.hxx"
#include <boost/spirit/home/x3.hpp>

using bmml::inaccord_t;
using bmml::spirit::barline;
using bmml::spirit::space;
using bmml::spirit::newline;
using bmml::spirit::separator;
using bmml::spirit::inaccord;
using bmml::spirit::generic_text;
using bmml::spirit::music_hyphen;
using bmml::spirit::part_name;
using bmml::spirit::element;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
namespace x3 = boost::spirit::x3;

using partial_voice_t = bmml::dom::element::elements_type;
using partial_measure_t = std::vector<partial_voice_t>;
using voice_t = std::vector<partial_measure_t>;
using measure_t = std::vector<voice_t>;

std::ostream& operator<<(std::ostream& out, measure_t const& m) {
  for (auto v : m) {
    out << "[";
    for (auto pm : v) {
      out << "{";
      for (auto pv : pm) {
        out << "(";
        bool first = true;
        for (auto e : pv) {
          if (!first) cout << ", "; else first = false;
          out << e->tag_name();
          if (auto n = e->as<bmml::note>()) {
            auto nd = n->find_element<bmml::note_data>();
            auto dur = nd->find_element<bmml::duration>();
            if (dur) out << " " << int(*dur);
          } else if (auto r = e->as<bmml::rest>()) {
            auto rd = r->find_element<bmml::rest_data>();
            auto dur = rd->find_element<bmml::duration>();
            out << " " << int(*dur);
          }
        }
        out << ")";
      }
      out << "}";
    }
    out << "]";
  }

  return out;
}

auto layout = space() | newline()
            | music_hyphen() | separator()
            | generic_text() | part_name()
            ;

x3::rule<struct partial_voice_rule, partial_voice_t> const partial_voice = "partial_voice";
x3::rule<struct partial_measure_rule, partial_measure_t> const partial_measure = "partial_measure";
x3::rule<struct voice_rule, voice_t> const voice = "voice";
x3::rule<struct measure_rule, measure_t> const measure = "measure";

auto partial_voice_def = +(element() - (inaccord() | barline()));
auto partial_measure_def = partial_voice % inaccord(inaccord_t::division);
auto voice_def = partial_measure % inaccord(inaccord_t::part);
auto measure_def = voice % inaccord(inaccord_t::full);

BOOST_SPIRIT_DEFINE(partial_voice, partial_measure, voice, measure);

// Put score content into part specific measure structures.
std::map<std::string, std::vector<measure_t>>
get_parts(std::shared_ptr<bmml::score> score) {
  std::map<std::string, std::vector<measure_t>> parts;

  for (auto sdc : *score->data()) {
    if (auto ts = sdc->as<bmml::time_signature>()) {
      cout << "global ts " << ts->values() << endl;
    } else if (auto p = sdc->as<bmml::part>()) {
      std::vector<measure_t> measures;
      auto iter = p->begin();
      if (x3::phrase_parse(iter, p->end(),
            measure % barline(), layout, measures)) {
        parts[p->id()].insert(parts[p->id()].end(), measures.begin(), measures.end());
      }
    }
  }

  return parts;
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " [<bmml-file>...]" << endl;
    return EXIT_FAILURE;
  }

  try {
    for (int i = 1; i < argc; ++i) {
      unsigned int full{0}, part{0}, division{0};
      ifstream ifs{argv[i]};

      if (ifs.good()) {
        auto score = bmml::parse(ifs, argv[i]);

        for (auto p : get_parts(score)) {
          cout << p.first << endl;
          for (auto m : p.second) {
            cout << m << endl;
          }
        }
      } else {
        cerr << "Unable to open '" << argv[i] << "'." << endl;
      }

      if (full && part && division) {
        cout << argv[i] << ": "
             << full << " full, "
             << part << " part, "
             << division << " division"
             << endl;
      }
    }
  } catch (const xml::exception& e) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
}
