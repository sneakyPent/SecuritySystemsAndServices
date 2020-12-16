#ifndef _MONITOR_H
#define _MONITOR_H


#include <unistd.h>
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
#include <netinet/ip6.h>
#include <arpa/inet.h>

/**
 * @brief Enumerator for the print function mode
 * 
 * @param info 0
 * @param error 1
 * @param success 2
 * 
 */
enum mode
{
    info,
    error,
    success
};

/**
 * @brief A struct to store the asked packet informations
 * 
 */
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

/**
 * @brief A struct to store the network flows found in the traffic
 * 
 */
typedef struct nflow
{
    int protocol;
    char destinationAddr[64];
    char sourceAddr[64];
    uint16_t destinationPort;
    uint16_t sourcePort;
    uint32_t expectedAck;
} networkFlow;

/**
 * @brief A linked list for the traffic's network flows
 * 
 */
typedef struct nflowLinkedList
{
    networkFlow *flow;                /** The current file name*/
    struct nflowLinkedList *nextFlow; /** Pointer to the next file of File list */

} networkFlowLinkedList;

/**
 * @brief Struct to store the traffic's network flow list and its length of flows
 * 
 */
typedef struct nFlowList
{
    int sum;                      /** The sum of files from list head until the end of the list */
    networkFlowLinkedList *flows; /** Pointer to the next file of File list */

} networkFlowList;

/* **************************** Flow list methods **************************** */

/**
 * @brief Check if two given network flows are identicals 
 * 
 * @param currentFlow 
 * @param givenFlow 
 * @return int 
 */
int isFlowsame(networkFlow *currentFlow, networkFlow *givenFlow);

/**
 * @brief Push the given flow to the given networkFlowList 
 * 
 * @param head 
 * @param newFlow 
 * @return networkFlowList* The head of the updated list
 */
networkFlowList *pushFlow(networkFlowList *head, networkFlow *newFlow);


/* **************************** PCAP methods **************************** */

/**
 * @brief Create a libpcap connection with the given device name and start receiving packets with pcap_loop function  
 * 
 * @param device 
 */
void live_capture(const char *device);

/**
 * @brief Start reading packets from the given pcap file with pcap_loop function 
 * 
 * @param fname 
 */
void offline_capture(const char *fname);

/**
 * @brief Function will be given as argument to pcap_loop function to handle the received packets
 * 
 * @param args 
 * @param header 
 * @param packet 
 */
void packetHandler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

/**
 * @brief Catch the sigint signal so as to print the asked info
 * 
 * @param sig 
 */
void handle_sigint(int sig);

/* **************************** Decoding Headers methods **************************** */

/**
 * @brief Decodes the given ipv6 header and get the asked packet information (protocol, destination Addr, source Addr)
 * 
 * @param ipv6Header 
 * @param newFlow 
 * @param pInfo 
 */
void decodeIpv6Header(const struct ip6_hdr *ipv6Header, networkFlow *newFlow, packetInfo *pInfo);

/**
 * @brief Decodes the given ipv4 header and get the asked packet information (protocol, destination Addr, source Addr)
 * 
 * @param ipHeader 
 * @param newFlow 
 * @param pInfo 
 */
void decodeIpv4Header(const struct iphdr *ipHeader, networkFlow *newFlow, packetInfo *pInfo);

/**
 * @brief Decodes the given TCP header and get the asked packet information (tcp_hdr length, payload length, destination port, source port)
 * 
 * @param header 
 * @param packetSize 
 * @param newFlow 
 * @param pInfo 
 */
void decodeTcpHeader(const  struct tcphdr * header, int packetSize, networkFlow *newFlow, packetInfo *pInfo);

/**
 * @brief Decodes the given TCP header and get the asked packet information (udp_hdr length, payload length, destination port, source port)
 * 
 * @param udph 
 * @param udpAndPayloadSize 
 * @param newFlow 
 * @param pInfo 
 */
void decodeUdpHeader(const struct udphdr *udph, int udpAndPayloadSize, networkFlow *newFlow, packetInfo *pInfo);

/* **************************** Print methods **************************** */

/**
 * @brief Print the info stored in the given packet
 * 
 * @param pInfo The package want to print
 */
void printPacketInfo(packetInfo *pInfo);

/**
 * @brief Print statistics asked in the 12th part
 * 
 */
void printStatistics();

void printFlow(networkFlow *givenFlow);

void printFlowList(networkFlowList *);


/**
 * @brief Prints the given string with specific color depends on given mode (used for debugging)
 * 
 * @param str 
 * @param md info, error, success
 */
void print(char *str, enum mode md);

#endif /* _MONITOR_H */