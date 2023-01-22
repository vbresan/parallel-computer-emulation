#ifndef ERROR_FUNCT_H
#define ERROR_FUNCT_H

class Exception;


void displ_error(char const * fname) throw();
void throw_excpt(char const * fname) throw(Exception);


#endif
