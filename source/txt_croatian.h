#ifndef TXT_CROATIAN_H
#define TXT_CROATIAN_H


#define TXT_EXIT_PROC_COUNT  "Dozvoljena je samo jedna instanca klase Process!"

#define TXT_ERROR	     "Pogreska"
#define TXT_IN_CALL	     "u pozivu funkcije"

#define TXT_EXCPT_KILL	     "ubijanje nepostojeceg procesa nije moguce!"
//#define TXT_EXCPT_ROUTE	     "kreiranje veze iz slave-a nije moguce!"
#define TXT_EXCPT_MSG_BIG    "prevelika poruka!"
#define TXT_EXCPT_R_DSTRY    "pokusao citati sa unistene veze!"
#define TXT_EXCPT_W_DSTRY    "pokusao pisati na unistenu vezu!"
#define TXT_EXCPT_MSG_UNKNWN "- primljena nepoznata poruka!"
#define TXT_EXCPT_MSG_INVLD  "pokusao kreirati nevaljanu poruku!"
#define TXT_EXCPT_NO_MEM     "nema dovoljno memorije za kreiranje poruke!"
#define TXT_EXCPT_INVLD_REF  "pokusao pristupiti nevazecoj adresi!"
#define TXT_EXCPT_WRITE	     "javila se greska prilikom pisanja na socket!"
#define TXT_EXCPT_READ	     "javila se greska prilikom citanja sa socketa!"
#define TXT_EXCPT_SELECT     "select pogreska: sockfd nije postavljen!"
#define TXT_EXCPT_ACCEPT     "javila se greska prilikom prihvacanja TCP veze!"

#define TXT_ERRC_ROUTE	     "nemoguce kreiranje veze - ista vec postoji"   \
			     " ili je dan krivi PID!"

#define TXT_CONN_VALID	     "- veza potvrdjena."
#define TXT_TRMNT_REQ	     "- primljen zahtjev za izlazak."
#define TXT_TRMNT_NTC	     "- primljena poruka o izlasku."
#define TXT_TRMNT_ACK	     "- primljena potvrda saznanja o izlasku."

#define TXT_ROUTE_CREAT	     "- stvaram vezu."
#define TXT_INVALID_ROUTE    "nemoguce kreiranje veze - neispravni brojevi" \
			     " slave procesa!"
#define TXT_ATTCH_REQ	     "- primljen zahtjev za kacenjem na memoriju."
#define TXT_STRTN_REQ	     "- primljen zahtjev za pokretanjem novog procesa."
#define TXT_SEND_REQ	     "- primljen zahtjev za slanjem."

#define TXT_DSCNCT_MASTER    "- prekidam vezu s master-om."
#define TXT_NO_MEM           "nema dovoljno memorije za citanje poruke!"


#endif
