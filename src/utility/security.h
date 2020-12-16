#ifndef UTILITY_SECURITY_H_
#define UTILITY_SECURITY_H_

#include <string>
#include <iostream>

namespace utility {

class Security {
 public:
  static std::string Base64Encode(const unsigned char* buffer, size_t length);
  static std::string Hmac(std::string data, std::string key);
};

}
#endif // UTILITY_SECURITY_H_
