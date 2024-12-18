## Next Generation Firewall 
# Project Overview

The goal of this project is to develop a Next Generation Firewall that is designed to work across multiple platforms. Currently, I am focusing on Linux integration as the first step in this process.
# Development

- Port Scanning: This feature works by utilizing the output of bash commands to gather information about open ports on the system.

- Port and PID Mapping: This feature enables the mapping of ports to process identifiers (PIDs), allowing for the termination of processes in response to the detection of malicious packets.

- Wi-Fi Network Interface Sniffing: The application includes the ability to capture and analyze network packets on Wi-Fi interfaces. This feature provides insight into the Wi-Fi packet format and aids in the potential development of a user-space firewall.

- Network Interface Configuration: To support packet routing at the OSI Layer 3/4, ongoing work involves understanding Linux kernel networking. The goal of configure_NIC is to utilize the Netlink library for communication between user space and kernel space
# Future Features

The firewall will evolve to include more advanced features, such as

- Port Filtering: Allowing users to block or allow traffic based on port  numbers.
- TLS/SSL Inspection: Inspecting encrypted traffic to enhance security without compromising privacy.

# Next Steps

- Port filtering 
- Stateful firewalling
- Aplication-level firewall | TLS/SSL inspection
- IDS 

# Note

- HELP.txt serves as a personal reference for my ideas and technical details.
- The comments within the code are written in a mix of Romanian and English, specifically for my own use. They help me recall what I've done, especially if I step away from the project for an extended period and need a refresher.
- I plan to include diagrams and screenshots of the file system and file formats to provide a clearer understanding of the inner workings of the project.
- As of this commit, another_port_scanner.cpp is designed to function with both TCP IPv4 and TCP IPv6 protocols(/proc/net/tcp has the same format as /proc/net/tcp6).

# links to resources used
    https://manpages.debian.org/testing/libnetfilter-queue-doc/nfq_handle_packet.3.en.html
    https://man7.org/linux/man-pages/man7/netlink.7.html
    https://www.frozentux.net/iptables-tutorial/iptables-tutorial.html
    http://charette.no-ip.com:81/programming/doxygen/netfilter/structnfq__handle.html