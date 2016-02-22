#include <fstream>
#include <iostream>

#include "bmml.hxx"

using namespace std;

int main (int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " [<filename.bmml>...]" << endl;
    return EXIT_FAILURE;
  }

  try {
    for (int i = 1; i < argc; ++i) {
      ifstream ifs{argv[i]};

      if (ifs.good()) {
        cout << bmml::parse(ifs, argv[i]) << endl;
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
