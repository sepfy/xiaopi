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

  int fd;
  struct ifreq ifr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  ifr.ifr_addr.sa_family = AF_INET;

  strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);
  ioctl(fd, SIOCGIFADDR, &ifr);
  close(fd);

  std::string addr(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

 return addr;
}

} // namespace utility
