#include <fstream>
#include <iostream>

#include "bmml.hxx"

using namespace std;

using partial_voice = bmml::dom::element::elements_type;
using partial_measure = std::vector<partial_voice>;
using voice = std::vector<partial_measure>;
using measure = std::vector<voice>;

ostream& operator<<(ostream& out, measure const& m) {
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

bool is_layout_element(std::shared_ptr<bmml::dom::element> e) {
  return e->is<bmml::space, bmml::newline,
               bmml::music_hyphen, bmml::separator,
               bmml::generic_text, bmml::part_name>();
}

// Put score content into part specific measure structures.
std::map<std::string, std::vector<measure>>
get_parts(shared_ptr<bmml::score> score) {
  std::map<std::string, std::vector<measure>> parts;

  for (auto sdc : *score->data()) {
    if (auto ts = sdc->as<bmml::time_signature>()) {
      cout << "global ts " << ts->values() << endl;
    } else if (auto p = sdc->as<bmml::part>()) {
      measure current_measure{};
      for (auto pc : *p) {
        if (auto inaccord = pc->as<bmml::inaccord>()) {
          switch (inaccord->value()) {
          case bmml::inaccord_t::full:
            current_measure.emplace_back();
          case bmml::inaccord_t::part:
            current_measure.back().emplace_back();
          case bmml::inaccord_t::division:
            current_measure.back().back().emplace_back();
          }
        } else if (auto bl = pc->as<bmml::barline>()) {
          if (!current_measure.empty()) {
            parts[p->id()].push_back(current_measure);

            current_measure.clear();
          }
        } else if (!is_layout_element(pc)) {
          if (current_measure.empty()) {
            current_measure.emplace_back();
            current_measure.back().emplace_back();
            current_measure.back().back().emplace_back();
          }

          current_measure.back().back().back().emplace_back(move(pc));
        }
      }
      if (!current_measure.empty()) parts[p->id()].push_back(current_measure);
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
          auto part_data = bmml::recursively_find_id(score, p.first);
          if (part_data && part_data->is<bmml::part_data>())
            cout << "Found part_data" << endl;
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
