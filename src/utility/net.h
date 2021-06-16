#ifndef UTILITY_NET_H_
#define UTILITY_NET_H_

#include <vector>
#include <iostream>
#include <string>

namespace utility {
namespace net {

  std::string GetIpAddr(std::string interface);
  std::string GetNetmask(std::string interface);
  std::string GetHwAddr(std::string interface);

} // namespace net
} // namespace utility

#endif // UTILITY_NET_H_
