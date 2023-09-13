#include <string>
#include <sstream>
#include <net/if.h>
#include <ifaddrs.h>
#include <cstring>
#include <sys/ioctl.h>
#include <iostream>

int main() {

    // control socket, args don't really matter
    int s = socket(AF_INET, SOCK_DGRAM,0);

    struct ifreq ifr;
    struct ifaddrs *addrs, *head;
    std::string ifname;
    std::stringstream mac;
    char buffer[3];

    getifaddrs(&addrs);

    // need to maintain head pointer to call free on later
    head = addrs;

    while(addrs) {

        /*
        * Look for the first physical interface, hopefully it is following either the old or new naming convention
        * Haven't figured out a way to determine "physicalness" of an interface other than based of name
        */
       if(std::strstr(addrs->ifa_name, "eth") != nullptr || std::strstr(addrs->ifa_name, "enp") != nullptr &&
            addrs->ifa_addr->sa_family == AF_PACKET) {
                ifname = addrs->ifa_name;
                // found first physical interface
                break;
        }
        addrs = addrs->ifa_next;
    }

    freeifaddrs(head);
    
    std::memset(&ifr, 0, sizeof(ifr));

    strncpy(ifr.ifr_name, ifname.c_str(), ifname.size());

    ioctl(s, SIOCGIFHWADDR, &ifr);

    for(int i = 0; i < 6 ; i++) {
        std::memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%.2X", (unsigned char)ifr.ifr_hwaddr.sa_data[i]);
        if (i < 5) {
            mac << buffer << ":";
        } else {
            mac << buffer;
        }
    }

    std::cout << "Interface: " << ifname << " MAC ADDRESS: " << mac.str() << std::endl;

    return 0;
}