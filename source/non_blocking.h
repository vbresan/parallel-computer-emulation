#ifndef NON_BLOCKING_H
#define NON_BLOCKING_H

#include "Connection.h"

class Exception;


read_rv  readn (int filedes, void * buff, size_t nbytes) throw(Exception);
write_rv writen(int filedes, const void * buff, size_t nbytes)throw(Exception);

#endif
