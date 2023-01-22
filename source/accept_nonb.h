#ifndef ACCEPT_NONB_H
#define ACCEPT_NONB_H

#include <sys/types.h>
#include <sys/socket.h>

class Exception;


int accept_nonb(int s, sockaddr * addr, socklen_t * addrlen) throw(Exception);

#endif
