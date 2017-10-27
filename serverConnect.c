#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <serverConnect.h>


int open_tcp(struct connection *conn){
  //setup sockaddr for server connection
  memset(&conn->sa, 0, sizeof(struct sockaddr_in));
  conn->sa.sin_family = AF_INET;
  conn->sa.sin_port = htons(conn->port);
  inet_pton(AF_INET, conn->server, &(conn->sa.sin_addr));

  //create socket
  conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(connect(conn->sockfd, (struct sockaddr*)&(conn->sa), sizeof(struct sockaddr_in)) < 0) {
    strerror(errno);
  }

  return conn->sockfd;
}



void close_tcp(struct connection *conn){
  shutdown(conn->sockfd, SHUT_RDWR);
  close(conn->sockfd);
  conn->sockfd = -1;
}


void connect_TLS(struct connection *conn){
  if(gnutls_check_version("3.4.6") == NULL){
    fprintf(stderr, "GNUtls 3.4.6 Is required.\n");
    exit();
  }

  if(gnutls_global_init() <= 0){
    fprintf(stderr, "GNUtls failed to initialize\n");
    exit();
  }


}
