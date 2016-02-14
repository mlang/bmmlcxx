#include <fstream>
#include <iostream>

#include <xml/parser>
#include <xml/serializer>

#include "bmml.hxx"

using namespace std;
using namespace xml;

int main (int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " <bmml-file>" << endl;
    return EXIT_FAILURE;
  }

  try {
    ifstream ifs { argv[1] };

    if (ifs.good()) {
      parser p{ifs, argv[1]};

      p.next_expect(parser::start_element, "score", content::complex);
      auto root = make_shared<bmml::score>(p, false);
      p.next_expect(parser::end_element, "score");

      cout << root;

      cout << endl << "List of notes and rests:" << endl;
      for (auto x : *root->data()) {
	if (auto p = dynamic_pointer_cast<bmml::part>(x)) {
	  for (auto pe : *p) {
	    if (auto n = dynamic_pointer_cast<bmml::note>(pe)) {
	      cout << n << " =";
	      auto nd = n->find_element<bmml::note_data>();
	      if (auto dur = nd->find_element<bmml::duration>())
		cout << ' ' << int(*dur);
	      else if (auto app = nd->find_element<bmml::appoggiatura_ref>())
		cout << ' ' << "appoggiatura";
	      auto pch = nd->find_element<bmml::pitch>();
	      cout << ' ' << int(*pch);
	      if (auto al = nd->find_element<bmml::alteration>())
		cout << ' ' << int(*al);
	      cout << endl;
	    } else if (auto r = dynamic_pointer_cast<bmml::rest>(pe)) {
	      auto rd = r->find_element<bmml::rest_data>();
	      auto dur = rd->find_element<bmml::duration>();
	      cout << r << " = " << int(*dur) << endl;
	    }
	  }
	}
      }

      serializer s(cout, "output");
      root->serialize(s);
    }
  } catch (const xml::exception& e) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
}
