#include <fstream>
#include <iostream>

#include <bmml.hxx>
#include <spirit_x3.hxx>
#include <boost/spirit/home/x3.hpp>

using std::begin;
using std::cout;
using bmml::dom::element;
using std::end;
using std::endl;
using bmml::spirit::generic_text;
using std::ifstream;
using bmml::spirit::newline;
using bmml::spirit::space;
using std::vector;
namespace x3 = boost::spirit::x3;

int main(int argc, char *argv[]) {
  ifstream file(argv[1]);
  auto score = bmml::parse(file, argv[1]);
  if (auto score_data = score->data()) {
    vector<std::shared_ptr<element>> text;
    auto iter = begin(*score_data);
    if (x3::parse(iter, end(*score_data), *( space()
                                           | newline()
                                           | generic_text()
                                           ), text)) {
      cout << text.size() << " text elements at beginning." << endl;
      cout << (*iter)->tag_name() << endl;
    }
  }
}


