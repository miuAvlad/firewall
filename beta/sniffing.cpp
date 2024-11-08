#include <pcap.h>/****/
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>      // pentru funcția ioctl
#include <net/if.h>         // pentru structura ifreq
#include <linux/wireless.h> // pentru structura iwreq
#include <sys/socket.h>

struct ieee80211_radiotap_header
{

    uint8_t it_version; // Versiunea header-ului radiotap (de obicei 0)
    uint8_t it_pad;     // Padding
    uint8_t it_len;     // Lungimea header-ului radiotap
    uint8_t it_present; // Ce campuri sunt prezente
};
struct ieee80211_header
{

    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_control;
    uint8_t addr4[6];
};
bool isInterfaceInMonitorMode(const char *ifname)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Cautam un socket
    if (sockfd < 0)
    {
        std::cerr << "Nu s-a putut deschide socket-ul\n";
        return false;
    }
    struct iwreq req;
    memset(&req, 0, sizeof(struct iwreq));   // Initializam structura cu 0 (all bytes of structure)
    strncpy(req.ifr_name, ifname, IFNAMSIZ); // Setam numele interfetei

    // Folosim ioctl pentru a obtine modul interfetei
    if (ioctl(sockfd, SIOCGIWMODE, &req) == -1)
    {
        std::cerr << "Nu s-au putut obtine informatiile despre modul interfetei\n";
        close(sockfd);
        return false;
    }

    close(sockfd);

    // Verificam modul interfetei
    if (req.u.mode == IW_MODE_MONITOR)
    {
        std::cout << ifname << " Este in mod monitor. \n";
        return true;
    }
    else
    {
        std::cout << ifname << " Nu este in modul monitor.\n";
        return false;
    }
}
bool checkAddr4Present(struct ieee80211_header *wifi_header)
{
    uint16_t frame_control = wifi_header->frame_control;
    int toDs = (frame_control & 0x0100) >> 8;
    int fromDs = (frame_control & 0x0200) >> 9;

    if (toDs && fromDs)
    {
        std::cout << "Addr4 present \n";
        return true;
    }
    else
    {
        std::cout << "Addr4 not present \n";
        return false;
    }
}
// Callback function
void packetHandler(unsigned char *userData, const struct pcap_pkthdr *pcap_pkthdr, const u_char *packet)
{

    int radiotap_len;
    struct ieee80211_header *wifi_header;
    int wifi_header_len;
    // Verificam dca interfata wifi este in modul monitor ca sa citim sau nu header-ul radiotap
    if (isInterfaceInMonitorMode("wl2ps0"))
    {
        struct ieee80211_radiotap_header *rtap_header = (struct ieee80211_radiotap_header *)(packet);
        radiotap_len = rtap_header->it_len;
        wifi_header = (struct ieee80211_header *)(packet + radiotap_len);
        wifi_header_len = checkAddr4Present(wifi_header) ? 30 : 24;
    }
    else
    {
        wifi_header = (struct ieee80211_header *)(packet);
        wifi_header_len = checkAddr4Present(wifi_header) ? 30 : 24;
    }

    struct ip *ipHeader = (struct ip *)(packet + wifi_header_len);                   // Offsetul pentru headerul IP dupa Ethernet
    struct tcphdr *tcpHeader = (struct tcphdr *)(packet + 14 + ipHeader->ip_hl * 4); // offset TCP dupa IP ip_hl indica lungimea in multiplii de 4 bytes

    if (ipHeader->ip_v == 4)
    {
        std::cout << "Ipv4\n";
    }
}

int main()
{
    char errbuff[PCAP_ERRBUF_SIZE];
    pcap_if_t *allDevs;
    pcap_if_t *device;

    if (pcap_findalldevs(&allDevs, errbuff) == -1)
    {
        std::cerr << "Error finding network interfaces" << errbuff << std::endl;
        return 1;
    }
    // Alegem prima interfata
    device = allDevs;

    while (device != nullptr)
    {
        if (std::string(device->name) == "wlp2s0")
        {
            break;
        }
        device = device->next;
    }

    if (device == nullptr)
    {
        std::cerr << "Nu s-a gasit interfata wlp2s0 (wi-fi)\n";
        return 1;
    }

    // Deschide prima interfata pentru captura, necesita sudo
    pcap_t *handle = pcap_open_live(device->name, 65535, 1, 1000, errbuff);
    if (handle == nullptr)
    {
        std::cerr << "Nu s-a putut deschide interfata" << errbuff << std::endl;
        return 1;
    }
    // Filtram pentru TCP
    struct bpf_program fp;
    if (pcap_compile(handle, &fp, "tcp", 0, PCAP_NETMASK_UNKNOWN) == -1)
    {
        std::cerr << "Eroare la compilarea filtrului" << std::endl;
        return 1;
    }
    if (pcap_setfilter(handle, &fp) == -1)
    {
        std::cerr << "Eroare la setarea filtrului" << std::endl;
        return 1;
    }

    pcap_freealldevs(allDevs);
    pcap_close(handle);
    std::cout << isInterfaceInMonitorMode("wlp2s0");

    // Loop pentru capturarea pachetelor
    return 0;
    pcap_loop(handle, 10, packetHandler, nullptr);
    pcap_freealldevs(allDevs);
    pcap_close(handle);

    return 0;
}