#ifndef UTILITY_HTTP_H_
#define UTILITY_HTTP_H_

#include <iostream>

namespace utility {
namespace http {

  long Post(const char *url, const char *data); 
  long Put(const char *url, const char *data); 

} // namespace http
} // namespace utility

#endif // UTILITY_HTTP_H_
