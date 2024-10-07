#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <unistd.h>
#include <net/if.h>

#define BUFFER_SIZE 4096

int main()
{
    int sockfd;

    struct sockaddr_nl sa;
    struct nlmsghdr *nlh;
    struct ifinfomsg *ifi;
    struct iovec iov;
    struct msghdr msg;

    // Creare socket netlink
    sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sockfd < 0)
    {
        std::cerr << "Eroare la deschiderea socket-ului netlink.\n";
        return -1;
    }

    // Initializare adresa Netlink
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = getpid();
    sa.nl_groups = 0; // Fara multicast

    // Asociare socket cu procesul curent
    if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        std::cerr << "Eroare la bind.\n";
        close(sockfd);
        return -1;
    }

    // Alocare memmorie pentru mesaj netlink
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(BUFFER_SIZE));
    memset(nlh, 0, NLMSG_SPACE(BUFFER_SIZE));

    // Setare parametrii mesajului netlink
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
    nlh->nlmsg_type = RTM_NEWLINK; // Adaugare/modificare interfata
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;
    nlh->nlmsg_seq = 1;
    nlh->nlmsg_pid = getpid();

    // Seteaza informatiile despre interfata de retea
    ifi = (struct ifinfomsg *)NLMSG_DATA(nlh); // NLMSG_DATA returneaza un pointer catre zona de date asociata mesajului netlink, spatiul dupa headerul netlink
    ifi->ifi_family = AF_UNSPEC;
    ifi->ifi_index = if_nametoindex("enp3s0f1");
    ifi->ifi_flags &= ~IFF_UP; // Elimină flag-ul IFF_UP pentru DOWN
    ifi->ifi_change = IFF_UP;  // Spune kernelului că doar flagul IFF_UP este schimbat


    // Configurarea mesajului de trimis
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&sa;
    msg.msg_namelen = sizeof(sa);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Trimitere mesaj Netlink
    if (sendmsg(sockfd, &msg, 0) < 0)
    {
        std::cerr << "Eroare la trimiterea mesajului netlink!\n";
        close(sockfd);
        free(nlh);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    struct iovec iov_r = {buffer, sizeof(buffer)};
    struct msghdr msg_r = {(struct sockaddr_nl *)&sa, sizeof(sa), &iov_r, 1, NULL, 0, 0};
    int len = recvmsg(sockfd, &msg_r, 0);
    if (len < 0)
    {
        perror("Eroare la recvmsg");
    }
    else
    {
        std::cout << "Mesaj primit: " << std::endl;
        for (int i = 0; i < len; i++)
        {
            printf("%02x ", (unsigned char)buffer[i]); 
        }
        std::cout << std::endl;
    }

    std::cout << "Mesajul netlink a fost trimis PID:" << getpid();
    close(sockfd);
    free(nlh);
    return 0;
}