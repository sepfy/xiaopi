#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "utility/parser.h"

namespace utility {
namespace parser {

std::vector<std::string> Split(std::string str, std::string del) {

  std::vector<std::string> v;
  int found;
  std::string str2 = str;
  while (str2.find(del) != std::string::npos) {
    found = str2.find(del);
    v.push_back(str2.substr(0, found));
    str2 = str2.substr(found + 1);
  }    
  v.push_back(str2);

  return v;
}

} // namespace parser
} // namespace utility

