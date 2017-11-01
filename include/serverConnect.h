#ifndef SERVER_CONNECT
#define SERVER_CONNECT
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


struct connection{
  int port;
  char *server;
  struct sockaddr_in sa;
  int sockfd;
  gnutls_certificate_credentials_t xcred;
  gnutls_session_t session;
};

/**
 * Open a TCP connection to a server. 
 * Args:
 *	conn: a connection struct with port and server set
 *          conn.port is an integer representing the port to connect to
 *	    conn.server is a string containing the ip of the destination
 *                      (x.x.x.x)
 */
int open_tcp(struct connection *conn);

/**
 * Open a TCP connection to a server. 
 * Args:
 *	conn: a connection struct of an open connection
 *
 */
void close_tcp(struct connection *conn);


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
int connect_TLS(struct connection *conn, char *trustfile, char *servername, size_t sname_len);


/**
 * Sends bytes on an encrypted socket
 * Args:
 *	conn: a connection struct containing an open TCP connection that has had TLS established
 *	msg: a pointer to the message
 *	msg_sz: the number of bytes in the message
 */
int tls_send(struct connection *conn, char *msg, size_t msg_sz);



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
int tls_recv(struct connection *conn, char *buffer, size_t buf_sz);


/**
 * Disconnect TLS from a socket
 * Args:
 * 	conn: a connection struct containing an open TCP connection that has had TLS established
 */
void disconnect_tls(struct connection *conn);

/**
 * Calls gnutls_global_deinit()
 */
void deinit_tls();

#endif 
