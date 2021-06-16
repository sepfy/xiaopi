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
namespace net {

std::string GetIpAddr(std::string interface) {

  int fd, ret;
  struct ifreq ifr;
  std::string addr = "Disconnect";

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

std::string GetNetmask(std::string interface) {

  int fd, ret;
  struct ifreq ifr;
  std::string addr = "Disconnect";

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  ifr.ifr_addr.sa_family = AF_INET;

  strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);
  ret = ioctl(fd, SIOCGIFNETMASK, &ifr);
  if(ret != -1) {
    addr = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
  }
  close(fd);

  return addr;
}

std::string GetHwAddr(std::string interface) {
  uint8_t *ptr;
  int fd, ret;
  struct ifreq ifr;
  std::string addr = "Disconnect";
  char buf[64];
  fd = socket(AF_INET, SOCK_DGRAM, 0);

  ifr.ifr_addr.sa_family = AF_INET;

  strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ-1);
  ret = ioctl(fd, SIOCGIFHWADDR, &ifr);
  if(ret != -1) {
    ptr = (uint8_t*)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0];
    sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3), *(ptr + 4), *(ptr + 5));
  }

  close(fd);
  addr = buf;
  return addr;
}

} // namespace net
} // namespace utility
