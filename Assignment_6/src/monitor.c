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



void decode_UDP(const u_char *packet, int size)
{
    unsigned short iphdrlen;
	struct iphdr *iph = (struct iphdr *)(packet +  sizeof(struct ethhdr));
	iphdrlen = iph->ihl*4;
	struct udphdr *udph = (struct udphdr*)(packet + iphdrlen  + sizeof(struct ethhdr));
	int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;
			
	
	printf("\n|UDP Header\n");
	printf("-> |-Source Port      : %d\n" , ntohs(udph->source));
	printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
	printf("   |-Header Length    : %d\n" ,  ntohs(udph->len));
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


void live_capture(const char *device)
{

    char error_buffer[PCAP_ERRBUF_SIZE];    /** Error buffer */
    pcap_t *handle;                         /** The device handle from where we want to capture */
    int packet_count_limit = 0;             /** The number of packets we want to capture ( 0 for unlimited packets) */
    int timeout_limit = 1 * 1000;           /** In milliseconds */

    /* Open device for live capture */
    handle = pcap_open_live(device, BUFSIZ, packet_count_limit, timeout_limit, error_buffer);

    if (handle == NULL)
        print(error_buffer, error);
        
    /* Our function to output some info */
    pcap_loop(handle, packet_count_limit, my_packet_handler, NULL);
}

void offline_capture(const char *fname){
    char error_buffer[PCAP_ERRBUF_SIZE]; /** Error buffer */
    pcap_t *handle;                      /** The device handle from where we want to capture */
    int packet_count_limit = 0;          /** The number of packets we want to capture ( 0 for unlimited packets) */
    /* Open device for live capture */
    handle = pcap_open_offline(fname, error_buffer);

    if (handle == NULL)
        print(error_buffer, error);

    /* Our function to output some info */
    pcap_loop(handle, packet_count_limit, my_packet_handler, NULL);
}