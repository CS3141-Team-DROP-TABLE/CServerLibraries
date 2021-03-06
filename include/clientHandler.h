#ifndef CLIENT_HANDLER
#define CLIENT_HANDLER


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#include <gnutls/gnutls.h>


struct connection_args{
  pthread_t thread_handle;
  int iret;

  
  int connection_fd;
  //int socket_fd;
  socklen_t client_addrlen;
  struct sockaddr_in client_socket;

  gnutls_certificate_credentials_t *x509_cred;
  gnutls_priority_t *priority_cache;
  gnutls_session_t session;

};




#endif
