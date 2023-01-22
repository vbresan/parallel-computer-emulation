#ifndef CONNECT_NONB_H
#define CONNECT_NONB_H


#include <sys/socket.h>

class Exception;


int connect_nonb(int sockfd, sockaddr const * saptr, 
		 socklen_t salen, int nsec)		throw(Exception);

#endif
