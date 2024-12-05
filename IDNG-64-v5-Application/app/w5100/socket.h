/*
*
@file		socket.h
@brief	define function of socket API 
*
*/

#ifndef	_SOCKET_H_
#define	_SOCKET_H_

extern unsigned char socket(unsigned char s, unsigned char protocol, unsigned int port, unsigned char flag); // Opens a socket(TCP or UDP or IP_RAW mode)
extern void close(unsigned char s); // Close socket
extern unsigned char connect(unsigned char s, unsigned char * addr, unsigned int port); // Establish TCP connection (Active connection)
extern void disconnect(unsigned char s); // disconnect the connection
extern unsigned char listen(unsigned char s);	// Establish TCP connection (Passive connection)
extern unsigned int send(unsigned char s, const unsigned char * buf, unsigned int len); // Send data (TCP)
extern unsigned int recv(unsigned char s, unsigned char * buf, unsigned int len);	// Receive data (TCP)
extern unsigned int sendto(unsigned char s, const unsigned char * buf, unsigned int len, unsigned char * addr, unsigned int port); // Send data (UDP/IP RAW)
extern unsigned int recvfrom(unsigned char s, unsigned char * buf, unsigned int len, unsigned char * addr, unsigned int  *port); // Receive data (UDP/IP RAW)

extern unsigned int igmpsend(unsigned char s, const unsigned char * buf, unsigned int len);
#endif

