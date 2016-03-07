#include <fstream>
#include <iostream>
#include <sstream>
#include <xml/exception>

#include "bmml.hxx"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;

string usage(string program_name) {
  stringstream out;
  out << "usage: " << program_name << " [[OPTIONS] <filename.bmml>]..." << endl
      << endl
      << "OPTIONS:" << endl
      << "  -n    Add newline at end of score (default)" << endl
      << "  -N    Do NOT add newline at end of score" << endl
      << endl
      << "Option processing stops when '--' is given." << endl;
  return out.str();
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    cerr << usage(argv[0]);
    return EXIT_FAILURE;
  }

  try {
    bool nleos = true;
    bool proc_flags = true;

    for (int i = 1; i < argc; ++i) {
      string arg{argv[i]};
      if (proc_flags && arg.size() == 2 && arg[0] == '-') {
        switch (arg[1]) {
        case 'n': nleos = false; break;
        case 'N': nleos = true;  break;
        case '-': proc_flags = false; break;
        default:
          cerr << "Unknown command-line argument '" << arg << "'." << endl
               << usage(argv[0]);
          return EXIT_FAILURE;
        }
      } else {
        ifstream ifs{arg};

        if (ifs.good()) {
          cout << bmml::parse(ifs, argv[i]);
          if (nleos) cout << endl;
        } else {
          cerr << "Unable to open file '" << arg << "'." << endl;
          return EXIT_FAILURE;
        }
      }
    }
  } catch (xml::exception const& e) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
}
