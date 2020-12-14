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

typedef struct nflow
{
    int protocol;
    char destinationAddr[64];
    char sourceAddr[64];
    uint16_t destinationPort;
    uint16_t sourcePort;
} networkFlow;

typedef struct nflowLinkedList
{
    networkFlow *flow;                /** The current file name*/
    struct nflowLinkedList *nextFlow; /** Pointer to the next file of File list */

} networkFlowLinkedList;

typedef struct nFlowList
{
    int sum;                      /** The sum of files from list head until the end of the list */
    networkFlowLinkedList *flows; /** Pointer to the next file of File list */

} networkFlowList;



#endif /* _MONITOR_H */