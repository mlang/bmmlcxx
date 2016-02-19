#include <fstream>
#include <iostream>

#include <xml/parser>
#include <xml/serializer>

#include "bmml.hxx"

using namespace std;
using namespace xml;

int main (int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " [<filename.bmml>...]" << endl;
    return EXIT_FAILURE;
  }

  try {
    for (int i = 1; i < argc; ++i) {
      ifstream ifs{argv[i]};

      if (ifs.good()) {
        parser p{ifs, argv[i]};

        p.next_expect(parser::start_element, "score", content::complex);
        cout << make_shared<bmml::score>(p, false) << endl;
        p.next_expect(parser::end_element, "score");
      } else {
        cerr << "Unable to open '" << argv[i] << "'." << endl;
        return EXIT_FAILURE;
      }
    }
  } catch (xml::exception const& e) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
}
