#ifndef TXT_ENGLISH_H
#define TXT_ENGLISH_H


#define TXT_EXIT_PROC_COUNT	"Only one instance of Process class is alowed!"

#define TXT_ERROR		"Error"
#define TXT_IN_CALL		"in call to"

#define TXT_EXCPT_KILL		"can't kill nonexisting process!"
//#define TXT_EXCPT_ROUTE		"can't create route from slave!"
#define TXT_EXCPT_MSG_BIG	"message too big!"
#define TXT_EXCPT_R_DSTRY       "tried to read from destroyed connection!"
#define TXT_EXCPT_W_DSTRY       "tried to write to destroyed connection!"
#define TXT_EXCPT_MSG_UNKNWN	"- unknown message received!"
#define TXT_EXCPT_MSG_INLVD	"tried to create invalid message!"
#define TXT_EXCPT_NO_MEM	"not enough memory for message creation!"
#define TXT_EXCPT_INVLD_REF	"tried to access invalid reference!"
#define TXT_EXCPT_WRITE		"error occured while writing to socket!"
#define TXT_EXCPT_READ		"error occured while reading from socket!"
#define TXT_EXCPT_SELECT	"select error: sockfd not set!"
#define TXT_EXCPT_ACCEPT	"error occured while accepting TCP connection!"

#define TXT_ERRC_ROUTE  	"can't create route - wrong pid number or " \
				"it already exists!"

#define TXT_CONN_VALID		"- connection validated."
#define TXT_TRMNT_REQ		"- request for termination received."
#define TXT_TRMNT_NTC		"- notice of termination received."
#define TXT_TRMNT_ACK		"- termination acknowledge received."

#define TXT_ROUTE_CREAT		"- creating route."
#define TXT_INVALID_ROUTE       "can't create route - invalid slave numbers!"
#define TXT_ATTCH_REQ		"- request for mem attach received."
#define TXT_STRTN_REQ		"- request for starting new process received."
#define TXT_SEND_REQ		"- request for sending received."

#define TXT_DSCNCT_MASTER       "- disconnecting with master."
#define TXT_NO_MEM	        "not enough memory to read message!"


#endif
