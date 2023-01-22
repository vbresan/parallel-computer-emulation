#include "TCPConnection.h"

#include "Wrapper.h"
#include "Message.h"
#include "non_blocking.h"

#include <string>
#include <arpa/inet.h>
#include <stdlib.h>	// atoi
#include <sys/param.h>	// MAXHOSTNAMELEN

#define MAX_PORT_LEN sizeof("65535")

using namespace Wrapper;

/*****************************************************************************
  
 Handle class 
  
*****************************************************************************/

class TCPConnection::Impl
{
 public:
   	static std::string s_netaddr;
   	static sockaddr_in netaddr;
   	static int         count;
   
   	std::string s_servaddr;
   	sockaddr_in servaddr;
   	int         listenfd;
   	int         connfd;
   
   	Impl();
   	Impl(char const * const i_servaddr);
   	~Impl();
};


/*----------------------------------------------------------------------------
 inicijalizacija static atributa
----------------------------------------------------------------------------*/

std::string TCPConnection::Impl::s_netaddr = "";
sockaddr_in TCPConnection::Impl::netaddr;
int         TCPConnection::Impl::count     = 0;


/*----------------------------------------------------------------------------
 konstruktor 
----------------------------------------------------------------------------*/

TCPConnection::Impl::Impl()
{
   if(!count++)
     {  char      hostname[MAXHOSTNAMELEN];
	char      hostaddr[INET_ADDRSTRLEN];
	hostent * p_hostent;
   
	Throw::gethostname(hostname, MAXHOSTNAMELEN);
	p_hostent = Throw::gethostbyname(hostname);
   
	memset(&netaddr, 0, sizeof(netaddr));
	netaddr.sin_family      = AF_INET;
	netaddr.sin_addr.s_addr = *((uint32_t *) p_hostent->h_addr_list[0]);
	netaddr.sin_port        = htons(0);
   
	inet_ntop(AF_INET, &netaddr.sin_addr, hostaddr, INET_ADDRSTRLEN);
	s_netaddr = hostaddr;
     }
   
   unsigned int length = sizeof(sockaddr_in);
   char         ac_port[MAX_PORT_LEN];
   
   listenfd   = Throw::socket(AF_INET, SOCK_STREAM, 0);
   servaddr   = netaddr;
   s_servaddr = s_netaddr;
   
   Throw::bind(listenfd, (sockaddr *) &servaddr, sizeof(servaddr));
   Throw::getsockname(listenfd, (sockaddr *) &servaddr, &length);
   Throw::listen(listenfd, 5);
   
   snprintf(ac_port, MAX_PORT_LEN, "%i", ntohs(servaddr.sin_port));
   s_servaddr += ':';
   s_servaddr += ac_port;
   
   std::cerr << "Listening on " << s_servaddr << std::endl;
}


TCPConnection::Impl::Impl
  (
   char const * const i_servaddr
  )
{  
   if(!count++)
     {  char      hostname[MAXHOSTNAMELEN];
	char      hostaddr[INET_ADDRSTRLEN];
	hostent * p_hostent;
   
	Throw::gethostname(hostname, MAXHOSTNAMELEN);
	p_hostent = Throw::gethostbyname(hostname);
   
	memset(&netaddr, 0, sizeof(netaddr));
	netaddr.sin_family      = AF_INET;
	netaddr.sin_addr.s_addr = *((uint32_t *) p_hostent->h_addr_list[0]);
	netaddr.sin_port        = htons(0);
   
	inet_ntop(AF_INET, &netaddr.sin_addr, hostaddr, INET_ADDRSTRLEN);
	s_netaddr = hostaddr;
     }
   
   s_servaddr = i_servaddr;
   std::string addr = s_servaddr.substr(0, s_servaddr.find(':'));
   std::string port = s_servaddr.substr(s_servaddr.find(':') + 1);

   inet_pton(AF_INET, addr.c_str(), &servaddr.sin_addr);
   servaddr.sin_port = htons(atoi(port.c_str()));
   
   std::cerr << "Connecting to " << addr << ':' << port << std::endl;
   
   connfd = Throw::socket(AF_INET, SOCK_STREAM, 0);
   Throw::connect(connfd, (sockaddr *) &servaddr, sizeof(servaddr));
}


/*----------------------------------------------------------------------------
 destruktor 
----------------------------------------------------------------------------*/

TCPConnection::Impl::~Impl()
{
   close(connfd);
   --count;
   
   std::cerr << "Zatvaram TCP vezu!" << std::endl;
}


/*****************************************************************************
  
 TCPConnection 
  
*****************************************************************************/

/*----------------------------------------------------------------------------
 konstruktor(i)
----------------------------------------------------------------------------*/

TCPConnection::TCPConnection
  (
   char const * const i_servaddr
  )
:  Connection(0, true),
   p(new Impl(i_servaddr))
{  
}


TCPConnection::TCPConnection()
:  Connection(0, true),
   p(new Impl)  
{
}


/*----------------------------------------------------------------------------
 destruktor
----------------------------------------------------------------------------*/

TCPConnection::~TCPConnection()
   throw()
{
   delete p;
}


/*----------------------------------------------------------------------------
 public metode 
----------------------------------------------------------------------------*/

bool
TCPConnection::destroyed()
   const
   throw()
{
   return false;
}


write_rv
TCPConnection::write
  (
   Message const & i_msg
  )
   throw(bad_alloc)
{  int          mid    = htonl(i_msg.mid());
   unsigned int len    = htonl(i_msg.len());
   char *       buffer = new char [i_msg.len()];
   write_rv     ret_val;
   
   i_msg.copy_data_to(buffer);
   if((ret_val = writen(p->connfd, &mid, sizeof(int))) != write_OK)
	return ret_val;
   
   while((ret_val = writen(p->connfd, &len, sizeof(unsigned int))) != write_OK)
	if(ret_val == conn_dstrw)
	     return ret_val;
   
   while((ret_val = writen(p->connfd, buffer, i_msg.len())) != write_OK)
	if(ret_val == conn_dstrw)
	     return ret_val;
   
   delete [] buffer;
   return ret_val;
}


read_rv
TCPConnection::read
  (
   Message * o_msg
  )
{  int          mid;
   unsigned int len;
   char *       buffer;
   read_rv      ret_val;
   
   if((ret_val = readn(p->connfd, &mid, sizeof(int))) != read_OK)
	return ret_val;
   
   while((ret_val = readn(p->connfd, &len, sizeof(unsigned int))) != read_OK)
	if(ret_val == conn_dstrr)
	     return ret_val;
   
   mid = ntohl(mid);
   len = ntohl(len);
   buffer = new char [len];
   
   while((ret_val = readn(p->connfd, buffer, len)) != read_OK)
	if(ret_val == conn_dstrr)
	     return ret_val;
   
   *o_msg = Message(mid, len, buffer);
   
   delete [] buffer;
   return ret_val;
}


void
TCPConnection::connfd
  (
   int const i_connfd
  )
   throw()
{
   p->connfd = i_connfd;
}


std::string const &
TCPConnection::servaddr()
   const
   throw()
{
   return p->s_servaddr;
}


int
TCPConnection::listenfd()
   const
   throw()
{
   return p->listenfd;
}


void
TCPConnection::close_listenfd()
   const
   throw()
{
   close(p->listenfd);
}
