#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "caller.h"

namespace utility {

std::string Caller::PipeCall(std::string cmd) {

  std::string result;
  FILE * p_file = NULL;
  char buf[8192];

  p_file = popen(cmd.c_str(), "r");
  if (!p_file) {
    fprintf(stderr, "Erro to popen");
  }

  while(fgets(buf, sizeof(buf), p_file) != NULL) {
    result.append(buf);
  }

  pclose(p_file);

  return result;
}

} // namespace utility

