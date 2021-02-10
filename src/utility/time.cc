#include <stdlib.h>
#include <sys/time.h>

#include "utility/time.h"

namespace utility {
namespace time {

uint64_t getms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1.0e+3 + tv.tv_usec/1000;
}

}  // time
}  // utility
