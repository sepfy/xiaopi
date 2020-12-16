#ifndef UTILITY_CALLER_H_
#define UTILITY_CALLER_H_

#include <iostream>
#include <string>

namespace utility {

class Caller {
 public:
  static std::string PipeCall(std::string cmd);

};


} // namespace utility


#endif // UTILITY_CALLER_H_
