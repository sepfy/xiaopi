#ifndef UTILITY_NET_H_
#define UTILITY_NET_H_

#include <vector>
#include <iostream>
#include <string>

namespace utility {

class Net {
 public:
  static std::string GetIpAddr(std::string interface);
};

} // namespace utility

#endif // UTILITY_NET_H_
