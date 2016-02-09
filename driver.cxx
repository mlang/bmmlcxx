#include <fstream>
#include <iostream>

#include <xml/parser>
#include <xml/serializer>

#include "bmml.hxx"

using namespace std;
using namespace xml;

int main (int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " <xml-file>" << endl;
    return EXIT_FAILURE;
  }

  try {
    // Parse the input document and construct an in-memory, DOM-like
    // raw XML representation for each object element. Also serialize
    // the data back to XML as we parse.
    //
    ifstream ifs (argv[1]);
    parser p (ifs, argv[1]);

    p.next_expect(parser::start_element, "score", content::complex);
    auto root = make_shared<bmml::score>(p, false);
    p.next_expect(parser::end_element, "score");

    for (auto x : root->data()->elements()) {
      if (auto p = dynamic_pointer_cast<bmml::part>(x)) {
        cout << p->name() << endl;
      }
    }

    serializer s (cout, "output");
    root->serialize(s);
  }
  catch (const xml::exception& e)
  {
    cerr << e.what () << endl;
    return 1;
  }
}
