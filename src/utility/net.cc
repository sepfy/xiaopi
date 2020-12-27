#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "utility/net.h"

namespace utility {

std::string Net::GetIpAddr(std::string interface) {

  int fd, ret;
  struct ifreq ifr;
  std::string addr = "";

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  ifr.ifr_addr.sa_family = AF_INET;

  strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);
  ret = ioctl(fd, SIOCGIFADDR, &ifr);
  if(ret != -1) {
    addr = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
  }
  close(fd);

  return addr;
}

} // namespace utility
