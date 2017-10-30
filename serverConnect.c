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


int connect_TLS(struct connection *conn, char *trustfile, char *servername, size_t sname_len){
  if(gnutls_check_version("3.4.6") == NULL){
    fprintf(stderr, "GNUtls 3.4.6 Is required.\n");
    return 0;
  }

  if(gnutls_global_init() <= 0){
    fprintf(stderr, "GNUtls failed to initialize\n");
    return 0;
  }

  if(gnutls_certificate_allocate_credentials(&(conn->xcred)) <= 0){
    fprintf(stderr, "GNUtls Error allocating credentials\n");
    return 0;
  }

  if(gnutls_certificate_set_x509_trust_file(conn->xcred,
					    trustfile,
					    GNUTLS_X509_FMT_PEM) <= 0){
    fprintf(stderr, "GNUtls Error setting trust file\n");
    return 0;
  }

  if(gnutls_init(&(conn->session), GNUTLS_CLIENT) <= 0){
    fprintf(stderr, "GNUtls Error on session init\n");
    return 0;
  }

  if(gnutls_server_name_set(conn->session, GNUTLS_NAME_DNS, servername, sname_len) <= 0){
    fprintf(stderr, "GNUtls Error setting server name\n");
    return 0;
  }

  if(gnutls_set_default_priority(conn->session) <= 0){
    fprintf(stderr, "GNUtls Error setting priority\n");
    return 0;
  }

  if(gnutls_credentials_set(conn->session, GNUTLS_CRD_CERTIFICATE, conn->xcred) <= 0){
    fprintf(stderr, "GNUtls Error setting credentials\n");
    return 0;
  }

  open_tcp(conn);

  gnutls_transport_set_int(conn->session, conn->sockfd);
  gnutls_handshake_set_timeout(conn->session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);


  int ret;
  do{
    ret = gnutls_handshake(conn->session);
  } while (ret < 0 && gnutls_error_is_fatal(ret) == 0);
  if(ret < 0){
    fprintf(stderr, "GNUtls Hanshake Failed\n");
    return 0;
  }


  return 1;

}
