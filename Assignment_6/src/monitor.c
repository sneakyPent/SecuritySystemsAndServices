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


void decode_ip_header(const u_char *packet, int size)
{
    unsigned short iphdrlen;
    struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ethhdr));
    iphdrlen = ip_header->ihl * 4;
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip_header->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip_header->daddr;

    printf("| IP Header\n");
    printf("-> |-IP Version         : %d\n", (unsigned int)ip_header->version);
    printf("   |-IP Header Length   : %d Bytes\n", ((unsigned int)(ip_header->ihl)) * 4);
    printf("   |-Protocol           : %d\n", ((unsigned int)ip_header->protocol));
    printf("   |-Source IP          : %s\n", inet_ntoa(source.sin_addr));
    printf("   |-Destination IP     : %s\n", inet_ntoa(dest.sin_addr));
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

    struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
    struct tcphdr *tcph = (struct tcphdr *)(packet + iphdrlen + sizeof(struct ethhdr));
    int header_size = sizeof(struct ethhdr) + iphdrlen + tcph->doff * 4;

    printf("\n");
    printf("|TCP Header\n");
    printf("-> |-Source Port      : %u\n", ntohs(tcph->source));
    printf("   |-Destination Port : %u\n", ntohs(tcph->dest));
    printf("   |-Header Length      : %d BYTES\n", (unsigned int)tcph->doff * 4);
    printf("   |-Payload Length     : %d BYTES\n", size - header_size);
    printf("\n###########################################################\n\n");
}



void decode_UDP(const u_char *packet, int size)
{
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
    struct udphdr *udph = (struct udphdr *)(packet + iphdrlen + sizeof(struct ethhdr));
    int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof udph;

    printf("\n|UDP Header\n");
    printf("-> |-Source Port      : %d\n", ntohs(udph->source));
    printf("   |-Destination Port : %d\n", ntohs(udph->dest));
    printf("   |-Header Length    : %d\n", ntohs(udph->len));
    printf("   |-Payload Length   : %d BYTES\n", size - header_size);
    printf("\n###########################################################\n\n");
}

void my_packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    int size = header->len;
    //Get the IP Header part of this packet , excluding the ethernet header
    struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
    switch (iph->protocol) //Check the Protocol and do accordingly...
    {
    case 6: //TCP Protocol
        tcpPackets++;
        tcpBytes += size;
        decode_ip_header(packet, size);
        decode_TCP(packet, size);
        break;
    case 17: //UDP Protocol
        udpPackets++;
        udpBytes += size;
        decode_ip_header(packet, size);
        decode_UDP(packet, size);
        break;
    default: //Every other protocol apart from tcp udp
        restPackets++;
        break;
    }
}

void printStatistics()
{
    printf("\n\n");
    printf("Total number of packets received:  %d\n", restPackets + udpPackets + tcpPackets);
    printf("Total number of TCP packets received: %d\n", tcpPackets);
    printf("Total number of UDP packets received: %d\n", udpPackets);
    printf("Total bytes of TCP packets received : %d\n", tcpBytes);
    printf("Total bytes of UDP packets received : %d\n", udpBytes);
}

void handle_sigint(int sig)
{
    printStatistics();
    exit(1);
}


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

{

    char error_buffer[PCAP_ERRBUF_SIZE]; /** Error buffer */
    pcap_t *handle;                      /** The device handle from where we want to capture */
    int packet_count_limit = 0;          /** The number of packets we want to capture ( 0 for unlimited packets) */
    int timeout_limit = 1 * 1000;        /** In milliseconds */

    /* Open device for live capture */
    handle = pcap_open_live(device, BUFSIZ, packet_count_limit, timeout_limit, error_buffer);

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
    int tcpPacks=0, udpPacks=0;
    if ( TCPList != NULL)
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