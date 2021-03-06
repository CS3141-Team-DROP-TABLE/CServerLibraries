#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#include <ping.h>
#include <linkedList.h>

/*
 * Checksum: Process from RFC 1071
 * The 16 bit one's complement of the one's complement sum of all 16
 * bit words in the header.
 */

unsigned short calc_cksum(unsigned short *data, size_t len){
  int sum = 0;
  unsigned short output = 0;
  unsigned short *marker = data;
  size_t bytes_left = len;

  
  // Sum all the 16 bit ints
  
  while(bytes_left > 1){
    sum += *marker++;
    bytes_left -= 2;
  }

  
  // Deal with remaining byte
   

  if(bytes_left == 1){
    *(unsigned char*)(&output) = *(unsigned char *)marker;
    sum += output;
  }

  
   // Add the upper 16 bits to the lower 16 bits,
   // add the resulting upper 16 bits,
   // then truncate any remaining bits
   //
  sum = (sum & 0xffff) + (sum >> 16); 
  sum += (sum >> 16);
  output = ~sum;

  return output;
}



char *create_packet(){
  char *retval = (char*)malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
  struct iphdr *ip = (struct iphdr*)retval;
  struct icmphdr *icmp = (struct icmphdr*)(retval+sizeof(struct iphdr));
  memset(ip, 0, sizeof(struct iphdr));
  memset(icmp, 0x00, sizeof(struct icmphdr));

  
  ip->ihl = 	5;
  ip->version = 4;
  ip->tos =	0;
  ip->tot_len = (sizeof(struct iphdr) + sizeof(struct icmphdr));
  ip->id = 	htons(random());
  ip->ttl =	255;
  ip->protocol= IPPROTO_ICMP;
  ip->check =	calc_cksum((unsigned short*)ip, sizeof(struct iphdr));

  
  icmp->type = 	        ICMP_ECHO;
  icmp->code =		0;
  icmp->un.echo.id = 	0;
  icmp->un.echo.sequence = 0;
  icmp->checksum = 0;
  icmp->checksum = calc_cksum((unsigned short*)icmp, sizeof(struct icmphdr));

  return retval;
}

void set_id(char *packet, u_int16_t id){
  struct icmphdr *icmp = (struct icmphdr*)(packet+sizeof(struct iphdr));
  icmp->un.echo.id = id;
  icmp->checksum = 0;
  icmp->checksum =  calc_cksum((unsigned short*)icmp, sizeof(struct icmphdr));

}

void set_src(char *packet, in_addr_t src){
  struct iphdr *ip = (struct iphdr*)packet;
  ip->saddr =	src;
}

void set_dst(char *packet, in_addr_t dst){
  struct iphdr *ip = (struct iphdr*)packet;
  ip->daddr =	dst;
}


int create_raw_socket(){
  return socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
}


int set_sock_opt(int sockfd){
  int optval = 1;
  if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int)) != 0){
    fprintf(stderr, "Error setting socket options: %s \n", strerror(errno)); 
    
  }
  return optval;
}

int send_ping(char* packet, in_addr_t dst, int sockfd){
  struct iphdr *ip = (struct iphdr*)packet;
  struct sockaddr_in snd_conn;

  snd_conn.sin_family = AF_INET;
  snd_conn.sin_addr.s_addr = dst;


  set_dst(packet, dst);
  struct icmphdr *icmp = (struct icmphdr*)(packet+sizeof(struct iphdr));
 
  
  return sendto(sockfd, packet, ip->tot_len, 0, (struct sockaddr*)&snd_conn, sizeof(struct sockaddr));
  //return send (sockfd, packet, ip->tot_len, 0);
}

in_addr_t get_ip(char *iface, size_t ifname_len, int sockfd){
    int fd;
    struct ifreq ifr;
     
    ifr.ifr_addr.sa_family = AF_INET;    
    memcpy(ifr.ifr_name, iface, ifname_len);
     
    ioctl(sockfd, SIOCGIFADDR, &ifr);

   
    return ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
}




