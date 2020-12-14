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