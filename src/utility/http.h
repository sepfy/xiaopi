#ifndef UTILITY_HTTP_H_
#define UTILITY_HTTP_H_

#include <iostream>

namespace utility {

class Http {
 public:
  static long Post(const char *url, const char *data); 
  static long Put(const char *url, const char *data); 

};

} // namespace utility

#endif // UTILITY_HTTP_H_
