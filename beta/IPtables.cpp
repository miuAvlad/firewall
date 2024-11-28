#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>

#include <arpa/inet.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

#define	BLOCKED_IP "93.185.216.32"

static int callback_function(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data)
{
	return 0;
}

int main(){
	const char  command = 'sudo iptables -A INPUT -p tcp --dport 80 -j NFQUEUE --queue-num 0';

	struct nfq_handle *h = nfq_open();
	struct nf1_q_handle *qh;
	struct nfnl_handle *nh;
	int fd;
	int rv;
	char buffer[4096] __attribute__((aligned));// inutil teoretic dar interesant, mai intai sa functioneze4 si dupa optimizare 

	if(!h){
		std::cerr<<"Eroare la deschidere netfilter queue.\n";
		exit(1);
	}
    if(!nfq_create_queue(h,0,&callback_function,NULL)){
		std::cerr<<"Eroare la crearea cozii.\n";
		exit(1);
	}
	
	return 0;
}
