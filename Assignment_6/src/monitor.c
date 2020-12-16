#include "monitor.h"

void usage(void)
{
    printf(
        "\n"
        "usage:\n"
        "\t./monitor \n"
        "\nOptions:\n\n"
        "-i <intf>,\t\t Name of the network interface will be monitored.\n"
        "-r <pcap_file>,\t\t The Packet capture will be used\n"
        "-h, \t\t\t Help message\n\n");

    exit(1);
}

int tcpPackets = 0, udpPackets = 0, tcpBytes = 0, udpBytes = 0, restPackets = 0;
networkFlowList *TCPList, *UDPList;
networkFlow *newFlow;
packetInfo *pInfo;

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_sigint);
    const char *fname;  /** Packet capture file name (e.g., test.pcap) */
    const char *device; /** Network interface name (e.g., eth0)  */

    int ch;
    if (argc < 2 || argc > 3)
        usage();
    while ((ch = getopt(argc, argv, "i:r:h")) != -1)
    {
        switch (ch)
        {
        case 'i':
            live_capture(optarg);
            break;
        case 'r':
            offline_capture(optarg);
            printStatistics();
            break;
        case 'h':
            usage();
            break;
        default:
            usage();
        }
    }

    argc -= optind;
    argv += optind;

    return 0;
}

/* **************************** Flow list methods **************************** */

int isFlowsame(networkFlow *currentFlow, networkFlow *givenFlow)
{
    int sameProtocol = currentFlow->protocol == givenFlow->protocol;
    int sameDestAddr = strcmp(currentFlow->destinationAddr, givenFlow->destinationAddr) == 0 ? 1 : 0;
    int sameSourceAddr = strcmp(currentFlow->sourceAddr, givenFlow->sourceAddr) == 0 ? 1 : 0;
    int sameDestPort = (unsigned long)currentFlow->destinationPort == (unsigned long)givenFlow->destinationPort;
    int sameSourcePort = (unsigned long)currentFlow->sourcePort == (unsigned long)givenFlow->sourcePort;

    return sameDestAddr && sameSourceAddr && sameDestPort && sameSourcePort && sameProtocol;
}

networkFlowList *pushFlow(networkFlowList *head, networkFlow *newFlow)
{
    networkFlowList *list = head;
    networkFlowLinkedList *currentFlow;
    // If list does not exists init one, else get the current flow.
    if (list == NULL)
    {
        list = malloc(sizeof(networkFlowList));
        list->sum = 1;
        list->flows = malloc(sizeof(networkFlowLinkedList));
        list->flows->flow = newFlow;
        list->flows->nextFlow = NULL;
        return list;
    }
    else
        currentFlow = list->flows;
    // Parse the list to check if flow exists
    while (currentFlow->nextFlow != NULL)
    {
        // By the time we found the flow we return the head and do nothing
        if (isFlowsame(currentFlow->flow, newFlow))
            return head;
        // get the next flow
        currentFlow = currentFlow->nextFlow;
    }
    if (isFlowsame(currentFlow->flow, newFlow))
        return head;
    // If flow not found, add it and increase list->sum
    currentFlow->nextFlow = malloc(sizeof(networkFlowLinkedList));
    currentFlow->nextFlow->flow = newFlow;
    currentFlow->nextFlow->nextFlow = NULL;
    list->sum++;
    return head;
}

/* **************************** PCAP methods **************************** */

void live_capture(const char *device)
{

    char error_buffer[PCAP_ERRBUF_SIZE]; /** Error buffer */
    pcap_t *handle;                      /** The device handle from where we want to capture */
    int packet_count_limit = 0;          /** The number of packets we want to capture ( 0 for unlimited packets) */
    int timeout_limit = 0.5 * 1000;      /** In milliseconds */

    // use pcap_open_live for opening device for receiving packets
    handle = pcap_open_live(device, BUFSIZ, packet_count_limit, timeout_limit, error_buffer);

    // if handle is Null then error happend and exit program
    if (handle == NULL)
        print(error_buffer, error);

    // Use pcap_loop to get packets and handle packets with packetHandler function
    pcap_loop(handle, packet_count_limit, packetHandler, NULL);
}

void offline_capture(const char *fname)
{
    char error_buffer[PCAP_ERRBUF_SIZE]; /** Error buffer */
    pcap_t *handle;                      /** The device handle from where we want to capture */
    int packet_count_limit = 0;          /** The number of packets we want to capture ( 0 for unlimited packets) */

    // use pcap_open_offline for receiving packets form pcap file
    handle = pcap_open_offline(fname, error_buffer);

    // if handle is Null then error happend and exit program
    if (handle == NULL)
        print(error_buffer, error);

    /* Our function to output some info */
    pcap_loop(handle, packet_count_limit, packetHandler, NULL);
}

void packetHandler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    // packet layers --  ethernet Header -> ip Header -> TCP/UCP Header ->data

    int size = header->len;
    newFlow = malloc(sizeof(networkFlow));
    pInfo = malloc(sizeof(packetInfo));

    // Getting the ethernet header and to get the the ethernet type
    struct ether_header *eth = (struct ether_header *)packet;
    // check if ip version is 4 or 6 or any ither
    if (ntohs(eth->ether_type) == ETHERTYPE_IP)
    {
        // Getting the ipv4 header and passing it in decodeIpv4Header function
        struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));
        // decoding the ipv4 header to collect every info needed
        decodeIpv4Header(ip_header, newFlow, pInfo);

        // ----------------------- next header -----------------------
        unsigned short iphdrlen = ip_header->ihl * 4;
        int protocolAndPayloadSize = size - sizeof(struct ether_header) - iphdrlen;
        switch (ip_header->protocol)
        {
        case 6:               //TCP Protocol
            tcpPackets++;     // increase tcp packet counter by 1
            tcpBytes += size; // increase tcp bytes counter by the total package size
            // Getting the tcp header and passing it in decodeTcpHeader function
            struct tcphdr *tcph = (struct tcphdr *)(packet + iphdrlen + sizeof(struct ether_header));
            // decoding the tcp header to collect every info needed
            decodeTcpHeader(tcph, protocolAndPayloadSize, newFlow, pInfo);
            // Add the specific network flow in the TCP network flows list if not already exist
            TCPList = pushFlow(TCPList, newFlow);
            // Print the collected packet's info
            printPacketInfo(pInfo);
            break;
        case 17:              //UDP Protocol
            udpPackets++;     // increase tcp packet counter by 1
            udpBytes += size; // increase tcp bytes counter by the total package size
            // Getting the udp header and passing it in decodeUdpHeader function
            struct udphdr *udph = (struct udphdr *)(packet + iphdrlen + sizeof(struct ether_header));
            // decoding the udp header to collect every info needed
            decodeUdpHeader(udph, protocolAndPayloadSize, newFlow, pInfo);
            // Add the specific network flow in the UDP network flows list if not already exist
            UDPList = pushFlow(UDPList, newFlow);
            // Print the collected packet's info
            printPacketInfo(pInfo);
            break;
        default:           //Every other protocol apart from tcp udp
            restPackets++; // increase rest packet counter by 1
            break;
        }
    }
    else if (ntohs(eth->ether_type) == ETHERTYPE_IPV6)
    {
        // Getting the ipv6 header and passing it in decodeIpv6Header function
        struct ip6_hdr *ipv6_h = (struct ip6_hdr *)(packet + sizeof(struct ethhdr));
        // decoding the ipv6 header to collect every info needed
        decodeIpv6Header(ipv6_h, newFlow, pInfo);

        // ----------------------- next header -----------------------
        int protocolAndPayloadSize = size - sizeof(struct ether_header) - sizeof(struct ip6_hdr);
        switch (ipv6_h->ip6_ctlun.ip6_un1.ip6_un1_nxt)
        {
        case 6:               //TCP Protocol
            tcpPackets++;     // increase tcp packet counter by 1
            tcpBytes += size; // increase tcp bytes counter by the total package size
            // Getting the tcp header and passing it in decodeTcpHeader function
            struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct ip6_hdr) + sizeof(struct ether_header));
            // decoding the tcp header to collect every info needed
            decodeTcpHeader(tcph, protocolAndPayloadSize, newFlow, pInfo);
            // Add the specific network flow in the TCP network flows list if not already exist
            TCPList = pushFlow(TCPList, newFlow);
            // Print the collected packet's info
            printPacketInfo(pInfo);
            break;
        case 17:              //UDP Protocol
            udpPackets++;     // increase tcp packet counter by 1
            udpBytes += size; // increase tcp bytes counter by the total package size
            // Getting the udp header and passing it in decodeUdpHeader function
            struct udphdr *udph = (struct udphdr *)(packet + sizeof(struct ip6_hdr) + sizeof(struct ether_header));
            // decoding the udp header to collect every info needed
            decodeUdpHeader(udph, protocolAndPayloadSize, newFlow, pInfo);
            // Add the specific network flow in the UDP network flows list if not already exist
            UDPList = pushFlow(UDPList, newFlow);
            // Print the collected packet's info
            printPacketInfo(pInfo);
            break;
        default: //Every other protocol apart from tcp udp
            restPackets++;
            break;
        }
    }
    else
        restPackets++; // increase rest packet counter by 1
}

void handle_sigint(int sig)
{
    printStatistics();
    exit(1);
}

/* **************************** Decoding Headers methods **************************** */

void decodeIpv6Header(const struct ip6_hdr *ipv6Header, networkFlow *newFlow, packetInfo *pInfo)
{
    // create 2 sockaddr_in to get destination and source addresses
    char buffer[INET6_ADDRSTRLEN];
    struct in6_addr source = ipv6Header->ip6_src;
    struct in6_addr dest = ipv6Header->ip6_dst;
    char src[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &source, src, INET6_ADDRSTRLEN);
    char dst[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &dest, dst, INET6_ADDRSTRLEN);
    char *prt;
    if (ipv6Header->ip6_ctlun.ip6_un1.ip6_un1_nxt == 6)
        prt = "TCP";
    else if (ipv6Header->ip6_ctlun.ip6_un1.ip6_un1_nxt == 17)
        prt = "UDP";
    else
        prt = "OTHER";

    // add info to packet Info
    strcpy(pInfo->protocol, prt);
    strcpy(pInfo->sourceAddr, src);
    strcpy(pInfo->destinationAddr, dst);
    // // add info to network flow
    newFlow->protocol = ipv6Header->ip6_ctlun.ip6_un1.ip6_un1_nxt;
    strcpy(newFlow->sourceAddr, src);
    strcpy(newFlow->destinationAddr, dst);
}

void decodeIpv4Header(const struct iphdr *ipHeader, networkFlow *newFlow, packetInfo *pInfo)
{
    // create 2 sockaddr_in to get destination and source addresses
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    memset(&dest, 0, sizeof(dest));
    source.sin_addr.s_addr = ipHeader->saddr;
    dest.sin_addr.s_addr = ipHeader->daddr;

    // Translate protocol to TCP/UDP/OTHER
    char *prt;
    if ((unsigned int)ipHeader->protocol == 6)
        prt = "TCP";
    else if ((unsigned int)ipHeader->protocol == 17)
        prt = "UDP";
    else
        prt = "OTHER";

    // add info to packet Info
    strcpy(pInfo->protocol, prt);
    strcpy(pInfo->sourceAddr, inet_ntoa(source.sin_addr));
    strcpy(pInfo->destinationAddr, inet_ntoa(dest.sin_addr));
    // add info to network flow
    newFlow->protocol = ipHeader->protocol;
    strcpy(newFlow->sourceAddr, inet_ntoa(source.sin_addr));
    strcpy(newFlow->destinationAddr, inet_ntoa(dest.sin_addr));
}

void decodeTcpHeader(const struct tcphdr *header, int tcpAndPayloadSize, networkFlow *newFlow, packetInfo *pInfo)
{
    // add info to packet Info
    pInfo->sourcePort = ntohs(header->source);
    pInfo->destinationPort = ntohs(header->dest);
    pInfo->headerLenght = (unsigned int)header->doff * 4;
    pInfo->payloadLenght = tcpAndPayloadSize - pInfo->headerLenght;
    // add info to network flow
    newFlow->destinationPort = ntohs(header->dest);
    newFlow->sourcePort = ntohs(header->source);
}

void decodeUdpHeader(const struct udphdr *udph, int udpAndPayloadSize, networkFlow *newFlow, packetInfo *pInfo)
{
    // add info to packet Info
    pInfo->sourcePort = ntohs(udph->source);
    pInfo->destinationPort = ntohs(udph->dest);
    pInfo->headerLenght = sizeof(udph);
    pInfo->payloadLenght = udpAndPayloadSize - pInfo->headerLenght;
    // add info to network flow
    newFlow->destinationPort = ntohs(udph->dest);
    newFlow->sourcePort = ntohs(udph->source);
}

/* **************************** Print methods **************************** */

void printPacketInfo(packetInfo *pInfo)
{
    printf("| Packet Info:\n");
    printf("->  |-Source IP         : %s\n", pInfo->sourceAddr);
    printf("    |-Destination IP    : %s\n", pInfo->destinationAddr);
    printf("    |-Source Port       : %u\n", pInfo->sourcePort);
    printf("    |-Destination Port  : %u\n", pInfo->destinationPort);
    printf("    |-Protocol          : %s\n", pInfo->protocol);
    printf("    |-Header Length     : %d BYTES\n", pInfo->headerLenght);
    printf("    |-Payload Length    : %d BYTES\n", pInfo->payloadLenght);
}

void printStatistics()
{
    printf("\n\n");
    int tcpPacks = 0, udpPacks = 0;
    if (TCPList != NULL)
        tcpPacks = TCPList->sum;
    if (UDPList != NULL)
        udpPacks = UDPList->sum;
    printf("Total number of network flows captured:  %d\n", tcpPacks + udpPacks);
    printf("Number of TCP network flows captured:  %d\n", tcpPacks);
    printf("Number of UDP network flows captured:  %d\n", udpPacks);
    printf("Total number of packets received:  %d\n", restPackets + udpPackets + tcpPackets);
    printf("Total number of TCP packets received: %d\n", tcpPackets);
    printf("Total number of UDP packets received: %d\n", udpPackets);
    printf("Total bytes of TCP packets received : %d\n", tcpBytes);
    printf("Total bytes of UDP packets received : %d\n", udpBytes);
}

void printFlow(networkFlow *givenFlow)
{
    printf("%s  \t\t\t", givenFlow->sourceAddr);
    printf("%lu  \t\t\t", givenFlow->sourcePort);
    printf("%s\t\t\t", givenFlow->destinationAddr);
    printf("%lu\t\t\t", givenFlow->destinationPort);
    printf("%d\n", givenFlow->protocol);
}

void printFlowList(networkFlowList *list)
{
    networkFlowLinkedList *currentFlow = list->flows;
    while (currentFlow->nextFlow != NULL)
    {
        printFlow(currentFlow->flow);
        currentFlow = currentFlow->nextFlow;
    }
    if (currentFlow != NULL)
        printFlow(currentFlow->flow);
}

void print(char *str, enum mode md)
{
    switch (md)
    {
    case error:
        printf("\033[1;31m");
        printf("[ERROR]: \033[0m %s\n", str);
        exit(EXIT_FAILURE);
        break;
    case info:
        printf("\033[0;36m");
        printf("[INFO]: \033[0m %s\n", str);
        break;
    case success:
        printf("\033[0;32m");
        printf("%s\033[0m\n", str);
        break;
    default:
        printf("%s\n", str);
        break;
    }
}
