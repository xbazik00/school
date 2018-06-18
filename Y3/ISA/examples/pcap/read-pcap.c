/*
 * read-pcap.c: read the pcap file given as the first argument and prints Ethernet headers
 *
 * Usage: ./read-pcap <file-name>
 *
 * (c) Petr Matousek, 2015
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h> 
#include <err.h>

#ifdef __linux__            // for Linux
#include <netinet/ether.h> 
#include <time.h>
#include <pcap/pcap.h>
#endif

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE (256)
#endif

#define SIZE_ETHERNET (14)       // offset of Ethernet header to L3 protocol


int main(int argc, char *argv[]){
  int n;
  char errbuf[PCAP_ERRBUF_SIZE];  // constant defined in pcap.h
  const u_char *packet;
  struct ip *my_ip;
  const struct tcphdr *my_tcp;    // pointer to the beginning of TCP header
  const struct udphdr *my_udp;    // pointer to the beginning of UDP header
  struct pcap_pkthdr header;  
  struct ether_header *eptr;
  pcap_t *handle;                 // file/device handler
  u_int size_ip;
  
  if (argc != 2)                  // one parameter expected
    errx(1,"Usage: %s <filename>",argv[0]);
  
  // open the input file
    if ((handle = pcap_open_offline(argv[1],errbuf)) == NULL)
    err(1,"Can't open file %s for reading",argv[1]);
  
  printf("Opening file %s for reading ...\n\n", argv[1]);
  n = 0;
  
  // read packets from the file
  while ((packet = pcap_next(handle,&header)) != NULL){
    n++;
    // print the packet header data
    printf("Packet no. %d:\n",n);
    printf("\tLength %d, received at %s",header.len,ctime((const time_t*)&header.ts.tv_sec));  
    
    
    // read the Ethernet header
    eptr = (struct ether_header *) packet;
    printf("\tSource MAC: %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost)) ;
    printf("\tDestination MAC: %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_dhost)) ;
    
    switch (ntohs(eptr->ether_type)){               // see /usr/include/net/ethernet.h for types
    case ETHERTYPE_IP: // IPv4 packet
      printf("\tEthernet type is  0x%x, i.e. IP packet \n", ntohs(eptr->ether_type));
      my_ip = (struct ip*) (packet+SIZE_ETHERNET);        // skip Ethernet header
      size_ip = my_ip->ip_hl*4;                           // length of IP header
      
      printf("\tIP: id 0x%x, hlen %d bytes, version %d, total length %d bytes, TTL %d\n",ntohs(my_ip->ip_id),size_ip,my_ip->ip_v,ntohs(my_ip->ip_len),my_ip->ip_ttl);
      printf("\tIP src = %s, ",inet_ntoa(my_ip->ip_src));
      printf("IP dst = %s",inet_ntoa(my_ip->ip_dst));
      
      switch (my_ip->ip_p){
      case 2: // IGMP protocol
	printf(", protocol IGMP (%d)\n",my_ip->ip_p);
	break;
      case 6: // TCP protocol
	printf(", protocol TCP (%d)\n",my_ip->ip_p);
	my_tcp = (struct tcphdr *) (packet+SIZE_ETHERNET+size_ip); // pointer to the TCP header
	printf("\tSrc port = %d, dst port = %d, seq = %u",ntohs(my_tcp->th_sport), ntohs(my_tcp->th_dport), ntohl(my_tcp->th_seq));
	if (my_tcp->th_flags & TH_SYN)
	  printf(", SYN");
	if (my_tcp->th_flags & TH_FIN)
	  printf(", FIN");
	if (my_tcp->th_flags & TH_RST)
	  printf(", RST");
	if (my_tcp->th_flags & TH_PUSH)
	  printf(", PUSH");
	if (my_tcp->th_flags & TH_ACK)
	  printf(", ACK");
	printf("\n");
	break;
      case 17: // UDP protocol
	printf(", protocol UDP (%d)\n",my_ip->ip_p);
	my_udp = (struct udphdr *) (packet+SIZE_ETHERNET+size_ip); // pointer to the UDP header
	printf("\tSrc port = %d, dst port = %d, length %d\n",ntohs(my_udp->uh_sport), ntohs(my_udp->uh_dport), ntohs(my_udp->uh_ulen));
	break;
      default: 
	printf(", protocol %d\n",my_ip->ip_p);
      }
      break;
    case ETHERTYPE_IPV6:  // IPv6
      printf("\tEthernet type is 0x%x, i.e., IPv6 packet\n",ntohs(eptr->ether_type));
      break;
    case ETHERTYPE_ARP:  // ARP
      printf("\tEthernet type is 0x%x, i.e., ARP packet\n",ntohs(eptr->ether_type));
      break;
    default:
      printf("\tEthernet type 0x%x, not IPv4\n", ntohs(eptr->ether_type));
    } 
  }
  printf("End of file reached ...\n");
  
  // close the capture device and deallocate resources
  pcap_close(handle);
  return 0;
}
