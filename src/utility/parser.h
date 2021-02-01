#ifndef UTILITY_PARSER_H_
#define UTILITY_PARSER_H_

#include <vector>
#include <iostream>
#include <string>

namespace utility {

class Parser {
 public:
  static std::vector<std::string> Split(std::string str, std::string del);  

};

} // namespace utility

#endif // UTILITY_PARSER_H_
