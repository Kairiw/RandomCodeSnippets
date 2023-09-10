#include <string.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <linux/ethtool.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/types.h>

/*
Code for pulling nic information from ethtool
Was related to a work project, but not sure what it was for exactly 

*/
void getInterface(char *buf) {

    // Need to maintain two stucts, one to iterate over and the other to maintain the head pointer
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);
    tmp = addrs;
    
    while(tmp) {
        // Look only for interfaces with address and aren't loopback. Should only be eth0
        if(tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET && !(tmp->ifa_flags & IFF_LOOPBACK)) {
            strcpy(buf, tmp->ifa_name);
            break;
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
}

int main() {

    // probably overkill ifa name should be less than 10 chars
    char buffer[100];

    getInterface(buffer);

    printf("%s\n", buffer);
    
    // control socket for calling ioctl, options shouldn't matter
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    struct ethtool_drvinfo drvinfo;

    memset(&ifr, 0, sizeof(ifr));
    memset(&drvinfo, 0, sizeof(drvinfo));

    strcpy(ifr.ifr_name, buffer);
    
    ifr.ifr_data = (caddr_t)&drvinfo;
    drvinfo.cmd = ETHTOOL_GDRVINFO;
    int ret = ioctl(sock, SIOCETHTOOL, &ifr);
    
    printf("%s\n", drvinfo.driver);
    return 0;
}
    
