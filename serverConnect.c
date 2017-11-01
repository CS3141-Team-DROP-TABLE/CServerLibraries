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


/**
 * Open a TCP connection to a server. 
 * Args:
 *	conn: a connection struct with port and server set
 *          conn.port is an integer representing the port to connect to
 *	    conn.server is a string litteral containing the ip of the destination
 *                      (x.x.x.x)
 */
int open_tcp(struct connection *conn){
  //setup sockaddr for server connection
  
  memset(&(conn->sa), 0, sizeof(struct sockaddr_in));
  conn->sa.sin_family = AF_INET;
  conn->sa.sin_port = htons(conn->port);
  inet_pton(AF_INET, (conn->server), &(conn->sa.sin_addr));

  
  
  //create socket
  conn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(connect(conn->sockfd, (struct sockaddr*)&(conn->sa), sizeof(struct sockaddr_in)) < 0) {
    strerror(errno);
  }

  return conn->sockfd;
}



/**
 * Open a TCP connection to a server. 
 * Args:
 *	conn: a connection struct of an open connection
 *
 */
void close_tcp(struct connection *conn){
  shutdown(conn->sockfd, SHUT_RDWR);
  close(conn->sockfd);
  conn->sockfd = -1;
}


/**
 * Makes a TLS connection on an open TCP socket.
 * Args:
 *	conn: a connection struct of an open tcp connection
 *	trustfile: a string containing the path of the trustfile
 *	servername: a string containing the name that the server's cert is signed with
 *	sname_len: the length of the server name
 *
 * Return:
 * 	Returns 1 on success, 0 on failure.
 */
int connect_TLS(struct connection *conn, char *trustfile, char *servername, size_t sname_len){
  if(gnutls_check_version("3.4.6") == NULL){
    fprintf(stderr, "GNUtls 3.4.6 Is required.\n");
    return 0;
  }

  if(gnutls_global_init() != 0){
    fprintf(stderr, "GNUtls failed to initialize\n");
    return 0;
  }

  if(gnutls_certificate_allocate_credentials(&(conn->xcred)) != GNUTLS_E_SUCCESS){
    fprintf(stderr, "GNUtls Error allocating credentials\n");
    return 0;
  }

  if(gnutls_certificate_set_x509_trust_file(conn->xcred,
					    trustfile,
					    GNUTLS_X509_FMT_PEM) <= 0){
    fprintf(stderr, "GNUtls Error setting trust file\n");
    return 0;
  }

  if(gnutls_init(&(conn->session), GNUTLS_CLIENT) != GNUTLS_E_SUCCESS){
    fprintf(stderr, "GNUtls Error on session init\n");
    return 0;
  }

  if(gnutls_server_name_set(conn->session, GNUTLS_NAME_DNS, servername, sname_len) != GNUTLS_E_SUCCESS){
    fprintf(stderr, "GNUtls Error setting server name\n");
    return 0;
  }

  if(gnutls_set_default_priority(conn->session)  != GNUTLS_E_SUCCESS){
    fprintf(stderr, "GNUtls Error setting priority\n");
    return 0;
  }

  if(gnutls_credentials_set(conn->session, GNUTLS_CRD_CERTIFICATE, conn->xcred)  != GNUTLS_E_SUCCESS){
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


/**
 * Sends bytes on an encrypted socket
 * Args:
 *	conn: a connection struct containing an open TCP connection that has had TLS established
 *	msg: a pointer to the message
 *	msg_sz: the number of bytes in the message
 */
int tls_send(struct connection *conn, char *msg, size_t msg_sz){
  return gnutls_record_send(conn->session, msg, msg_sz);
}

/**
 * recives bytes from an encrypted socket
 * Args:
 *	conn: a connection struct containing an open TCP connection that has had TLS established
 * 	buffer: the destination for the read data
 *	buf_sz: the maximum number of bytes to read
 *
 * Return:
 *	Returns the number of bytes read. 0 on EOF, negative on Error.
 */
int tls_recv(struct connection *conn, char *buffer, size_t buf_sz){
  return gnutls_record_recv(conn->session, buffer, buf_sz);
}


/**
 * Disconnect TLS from a socket
 * Args:
 * 	conn: a connection struct containing an open TCP connection that has had TLS established
 */
void disconnect_tls(struct connection *conn){
  gnutls_bye(conn->session, GNUTLS_SHUT_RDWR);
  gnutls_deinit(conn->session);
  conn->session = NULL;
  gnutls_certificate_free_credentials(conn->xcred);
}

/**
 * Calls gnutls_global_deinit()
 */
void deinit_tls(){
  gnutls_global_deinit();
}
