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
  return dynamic_pointer_cast<bmml::space>(e)
      || dynamic_pointer_cast<bmml::newline>(e)
      || dynamic_pointer_cast<bmml::music_hyphen>(e)
      || dynamic_pointer_cast<bmml::separator>(e)
      || dynamic_pointer_cast<bmml::generic_text>(e)
      || dynamic_pointer_cast<bmml::part_name>(e);
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

        for (auto sdc : *score->data()) {
          if (auto ts = dynamic_pointer_cast<bmml::time_signature>(sdc)) {
            cout << "global ts" << endl;
          } else if (auto p = dynamic_pointer_cast<bmml::part>(sdc)) {
            measure current_measure{};
            current_measure.emplace_back();
            current_measure.back().emplace_back();
            current_measure.back().back().emplace_back();
            for (auto pc : *p) {
              if (auto inaccord = dynamic_pointer_cast<bmml::inaccord>(pc)) {
                switch (inaccord->value()) {
                case bmml::inaccord_t::full:
                  current_measure.emplace_back();
                case bmml::inaccord_t::part:
                  current_measure.back().emplace_back();
                case bmml::inaccord_t::division:
                  current_measure.back().back().emplace_back();
                }
              } else if (auto bl = dynamic_pointer_cast<bmml::barline>(pc)) {
                cout << current_measure << endl;

                current_measure.clear();
                current_measure.emplace_back();
                current_measure.back().emplace_back();
                current_measure.back().back().emplace_back();
              } else if (!is_layout_element(pc)) {
                current_measure.back().back().back().push_back(pc);
              }
            }
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
