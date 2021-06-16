#ifndef UTILITY_SECURITY_H_
#define UTILITY_SECURITY_H_

#include <string>
#include <iostream>

namespace utility {
namespace security {

  std::string Base64Encode(const unsigned char* buffer, size_t length);
  std::string Base58Encode(const unsigned char* pbegin, size_t len);
  std::string Hmac(std::string data, std::string key);

} // namespace security
} // namespace utility
#endif // UTILITY_SECURITY_H_
