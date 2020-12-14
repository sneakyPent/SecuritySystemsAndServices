#ifndef _MONITOR_H
#define _MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <signal.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
typedef struct packet{
    char protocol[6];
    char destinationAddr[64];
    char sourceAddr[64];
    uint16_t destinationPort;
    uint16_t sourcePort;
    int headerLenght;
    int payloadLenght;
    int retransmitted;
}packetInfo;



#endif /* _MONITOR_H */