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
    int timeout_limit = 1 * 1000;        /** In milliseconds */

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
    // printf("packetSize = %d", size);
    newFlow = malloc(sizeof(networkFlow));
    pInfo = malloc(sizeof(packetInfo));

    struct ether_header *eth = (struct ether_header *)packet;
    if (ntohs(eth->ether_type) == ETHERTYPE_IP)
    {
        struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ether_header));
        // create 2 sockaddr_in to get destination and source addresses
        struct sockaddr_in source, dest;
        memset(&source, 0, sizeof(source));
        memset(&dest, 0, sizeof(dest));
        source.sin_addr.s_addr = ip_header->saddr;
        dest.sin_addr.s_addr = ip_header->daddr;

        char *prt = ((unsigned int)ip_header->protocol) == 6 ? "TCP" : "UDP";

        strcpy(pInfo->protocol, prt);
        strcpy(pInfo->sourceAddr, inet_ntoa(source.sin_addr));
        strcpy(pInfo->destinationAddr, inet_ntoa(dest.sin_addr));
        // add info to network flow
        newFlow->protocol = ip_header->protocol;
        strcpy(newFlow->sourceAddr, inet_ntoa(source.sin_addr));
        strcpy(newFlow->destinationAddr, inet_ntoa(dest.sin_addr));

        // ------------------------------ next header decode -------------------
        unsigned short iphdrlen;
        int otherHeadersSize;
        switch (ip_header->protocol)
        {
        case 6: //TCP Protocol
            tcpPackets++;
            tcpBytes += size;
            // ----

            iphdrlen = ip_header->ihl * 4;
            struct tcphdr *tcph = (struct tcphdr *)(packet + iphdrlen + sizeof(struct ether_header));
            otherHeadersSize = sizeof(struct ether_header) + iphdrlen + tcph->doff * 4;

            // add info to packet Info
            // printf("------- %u\n", tcph->th_seq);
            // printf("------- %u\n", tcph->th_ack);
            pInfo->sourcePort = ntohs(tcph->source);
            pInfo->destinationPort = ntohs(tcph->dest);
            pInfo->headerLenght = (unsigned int)tcph->doff * 4;
            pInfo->payloadLenght = size - otherHeadersSize;
            // add info to network flow
            newFlow->destinationPort = ntohs(tcph->dest);
            newFlow->sourcePort = ntohs(tcph->source);

            // ---

            TCPList = pushFlow(TCPList, newFlow);
            printPacketInfo(pInfo);
            break;
        case 17: //UDP Protocol
            udpPackets++;
            udpBytes += size;
            
            iphdrlen = ip_header->ihl * 4;
            struct udphdr *udph = (struct udphdr *)(packet + iphdrlen + sizeof(struct ether_header));
            otherHeadersSize = sizeof(struct ether_header) + iphdrlen + sizeof(udph);

            // add info to packet Info
            pInfo->sourcePort = ntohs(udph->source);
            pInfo->destinationPort = ntohs(udph->dest);
            pInfo->headerLenght = sizeof(udph);
            pInfo->payloadLenght = size - otherHeadersSize;
            // add info to network flow
            newFlow->destinationPort = ntohs(udph->dest);
            newFlow->sourcePort = ntohs(udph->source);

            UDPList = pushFlow(UDPList, newFlow);
            printPacketInfo(pInfo);
            break;
        default: //Every other protocol apart from tcp udp
            restPackets++;
            break;
        }
        
    }
    else if (ntohs(eth->ether_type) == ETHERTYPE_IPV6)
    {
        struct ip6_hdr *ipv6_h = (struct ip6_hdr*)(packet + sizeof(struct ethhdr));
        char buffer[INET6_ADDRSTRLEN];
        struct in6_addr source = ipv6_h->ip6_src;
        struct in6_addr dest  = ipv6_h->ip6_dst;
        char src[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &source, src, INET6_ADDRSTRLEN);
        char dst[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &dest, dst, INET6_ADDRSTRLEN);
        char *prt;
        if (ipv6_h->ip6_ctlun.ip6_un1.ip6_un1_nxt == 6)
            prt = "TCP";
        else if (ipv6_h->ip6_ctlun.ip6_un1.ip6_un1_nxt == 17)
            prt = "UDP";
        else 
            prt = "OTHER";
        // printf("address = %s",src);
        // printf("address = %s",dst);
        // printf("protocol = %d", ipv6_h->ip6_ctlun.ip6_un1.ip6_un1_nxt );
        strcpy(pInfo->protocol, prt);
        strcpy(pInfo->sourceAddr, src);
        strcpy(pInfo->destinationAddr, dst);
        // // add info to network flow
        newFlow->protocol = ipv6_h->ip6_ctlun.ip6_un1.ip6_un1_nxt;
        strcpy(newFlow->sourceAddr, src);
        strcpy(newFlow->destinationAddr, dst);

        // ------------------------------ next header decode -------------------
        unsigned short iphdrlen;
        int otherHeadersSize;
        switch (ipv6_h->ip6_ctlun.ip6_un1.ip6_un1_nxt)
        {
        case 6: //TCP Protocol
            tcpPackets++;
            tcpBytes += size;
            struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct ip6_hdr) + sizeof(struct ether_header));
            otherHeadersSize = sizeof(struct ether_header) + sizeof(struct ip6_hdr) + (unsigned int)tcph->doff * 4;

            // add info to packet Info
            // printf("------- %u\n", tcph->th_seq);
            // printf("------- %u\n", tcph->th_ack);
            pInfo->sourcePort = ntohs(tcph->source);
            pInfo->destinationPort = ntohs(tcph->dest);
            pInfo->headerLenght = (unsigned int)tcph->doff * 4;
            pInfo->payloadLenght = size - otherHeadersSize;
            // add info to network flow
            newFlow->destinationPort = ntohs(tcph->dest);
            newFlow->sourcePort = ntohs(tcph->source);
            TCPList = pushFlow(TCPList, newFlow);
            printPacketInfo(pInfo);
            break;
        case 17: //UDP Protocol
            udpPackets++;
            udpBytes += size;
            struct udphdr *udph = (struct udphdr *)(packet + sizeof(struct ip6_hdr) + sizeof(struct ether_header));
            otherHeadersSize = sizeof(struct ether_header) + sizeof(struct ip6_hdr) + sizeof(udph);

            // add info to packet Info
            pInfo->sourcePort = ntohs(udph->source);
            pInfo->destinationPort = ntohs(udph->dest);
            pInfo->headerLenght = sizeof(udph);
            pInfo->payloadLenght = size - otherHeadersSize;
            // add info to network flow
            newFlow->destinationPort = ntohs(udph->dest);
            newFlow->sourcePort = ntohs(udph->source);
            UDPList = pushFlow(UDPList, newFlow);
            printPacketInfo(pInfo);
            break;
        default: //Every other protocol apart from tcp udp
            restPackets++;
            break;
        }
    }
    else
    {
        restPackets++;
    }
    
}

void handle_sigint(int sig)
{
    printStatistics();
    exit(1);
}

/* **************************** Decoding Headers methods **************************** */

void decode_ip_header(const u_char *packet, networkFlow *newFlow, packetInfo *pInfo)
{
    struct ethhdr *eth = (struct ethhdr *)packet;
    // get the ip header from the packet
    struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ethhdr));

    // create 2 sockaddr_in to get destination and source addresses
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    memset(&dest, 0, sizeof(dest));
    source.sin_addr.s_addr = ip_header->saddr;
    dest.sin_addr.s_addr = ip_header->daddr;

    char *prt = ((unsigned int)ip_header->protocol) == 6 ? "TCP" : "UDP";

    strcpy(pInfo->protocol, prt);
    strcpy(pInfo->sourceAddr, inet_ntoa(source.sin_addr));
    strcpy(pInfo->destinationAddr, inet_ntoa(dest.sin_addr));
    // add info to network flow
    newFlow->protocol = ip_header->protocol;
    strcpy(newFlow->sourceAddr, inet_ntoa(source.sin_addr));
    strcpy(newFlow->destinationAddr, inet_ntoa(dest.sin_addr));
}

void decode_TCP(const u_char *packet, int packetSize, networkFlow *newFlow, packetInfo *pInfo)
{
    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
    struct tcphdr *tcph = (struct tcphdr *)(packet + iphdrlen + sizeof(struct ethhdr));
    int otherHeadersSize = sizeof(struct ethhdr) + iphdrlen + tcph->doff * 4;

    // add info to packet Info
    printf("------- %u\n", tcph->th_seq);
    printf("------- %u\n", tcph->th_ack);
    pInfo->sourcePort = ntohs(tcph->source);
    pInfo->destinationPort = ntohs(tcph->dest);
    pInfo->headerLenght = (unsigned int)tcph->doff * 4;
    pInfo->payloadLenght = packetSize - otherHeadersSize;
    // add info to network flow
    newFlow->destinationPort = ntohs(tcph->dest);
    newFlow->sourcePort = ntohs(tcph->source);
}

void decode_UDP(const u_char *packet, int packetSize, networkFlow *newFlow, packetInfo *pInfo)
{
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
    struct udphdr *udph = (struct udphdr *)(packet + iphdrlen + sizeof(struct ethhdr));
    int otherHeadersSize = sizeof(struct ethhdr) + iphdrlen + sizeof(udph);

    // add info to packet Info
    pInfo->sourcePort = ntohs(udph->source);
    pInfo->destinationPort = ntohs(udph->dest);
    pInfo->headerLenght = sizeof(udph);
    pInfo->payloadLenght = packetSize - otherHeadersSize;
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
    printf("%d\t", givenFlow->protocol);
    printf("%s\t", givenFlow->destinationAddr);
    printf("%s\t", givenFlow->sourceAddr);
    printf("%lu\t", givenFlow->destinationPort);
    printf("%lu\n", givenFlow->sourcePort);
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

    /* Our function to output some info */
    pcap_loop(handle, packet_count_limit, my_packet_handler, NULL);
}