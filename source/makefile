CXX      = g++
OPTIONS  = -pedantic -W -Wall
LIBS     = libpce.a
OBJS     = Process.o MessageProcessor.o MasterProcess.o SlaveProcess.o \
	   start_new.o Child.o Connection.o SharedMem.o Destroyed.o \
	   CircularPtr.o Exception.o error_funct.o Wrapper.o shm_lock.o \
	   debug.o Message.o TCPConnection.o non_blocking.o \
	   connect_nonb.o accept_nonb.o

all             : ${LIBS}

test		: console validity0 validity1 test_start_new tcp_validity0 \
		  benchmark tcp_benchmark pmatrix

clean		:
		rm *~ -f



libpce.a	: ${OBJS}
		ar cr $@ ${OBJS}
		ranlib $@




console         : console.o ${LIBS}
		${CXX} console.o ${LIBS} -o $@

validity0       : validity0.o ${LIBS}
		${CXX} validity0.o ${LIBS} -o $@

validity1       : validity1.o ${LIBS}
		${CXX} validity1.o ${LIBS} -o $@

test_start_new	: test_start_new.o ${LIBS}
		${CXX} test_start_new.o ${LIBS} -o $@

tcp_validity0   : tcp_validity0.o ${LIBS}
		${CXX} tcp_validity0.o ${LIBS} -o $@

benchmark       : benchmark.o ${LIBS}
		${CXX} benchmark.o ${LIBS} -o $@

tcp_benchmark   : tcp_benchmark.o ${LIBS}
		${CXX} tcp_benchmark.o ${LIBS} -o $@

pmatrix   	: pmatrix.o ${LIBS}
		${CXX} pmatrix.o ${LIBS} -o $@


console.o  	: console.cpp pce.h
		${CXX} ${OPTIONS} -c console.cpp

validity0.o     : validity0.cpp pce.h Message.h
		${CXX} ${OPTIONS} -c validity0.cpp

validity1.o     : validity1.cpp pce.h Message.h
		${CXX} ${OPTIONS} -c validity1.cpp

test_start_new.o: test_start_new.cpp pce.h
		${CXX} ${OPTIONS} -c test_start_new.cpp

tcp_validity0.o : tcp_validity0.cpp pce.h Message.h
		${CXX} ${OPTIONS} -c tcp_validity0.cpp

benchmark.o     : benchmark.cpp pce.h Message.h
		${CXX} ${OPTIONS} -c benchmark.cpp

tcp_benchmark.o : tcp_benchmark.cpp pce.h Message.h
		${CXX} ${OPTIONS} -c tcp_benchmark.cpp

pmatrix.o 	: pmatrix.cpp pce.h Message.h
		${CXX} ${OPTIONS} -c pmatrix.cpp



Destroyed.h	: Connection.h
		touch $@

MasterProcess.h : MessageProcessor.h
		touch $@

MessageProcessor.h: Process.h
		touch $@

non_blocking.h	: Connection.h
		touch $@

SharedMem.h	: Connection.h
		touch $@

SlaveProcess.h	: MessageProcessor.h
		touch $@

pce.h	: MessageProcessor.h
		touch $@

TCPConnection.h : Connection.h
		touch $@

txt.h		: txt_croatian.h txt_english.h
		touch $@



accept_nonb.o	: accept_nonb.cpp accept_nonb.h Wrapper.h txt.h Exception.h
		${CXX} ${OPTIONS} -c accept_nonb.cpp

Child.o		: Child.cpp Child.h Wrapper.h
		${CXX} ${OPTIONS} -c Child.cpp

CircularPtr.o	: CircularPtr.cpp CircularPtr.h
		${CXX} ${OPTIONS} -c CircularPtr.cpp

connect_nonb.o	: connect_nonb.cpp connect_nonb.h Wrapper.h Exception.h
		${CXX} ${OPTIONS} -c connect_nonb.cpp

Connection.o	: Connection.cpp Connection.h Wrapper.h
		${CXX} ${OPTIONS} -c Connection.cpp

debug.o		: debug.cpp debug.h
		${CXX} ${OPTIONS} -c debug.cpp

Destroyed.o	: Destroyed.cpp Destroyed.h txt.h Exception.h
		${CXX} ${OPTIONS} -c Destroyed.cpp

error_funct.o	: error_funct.cpp error_funct.h txt.h Exception.h
		${CXX} ${OPTIONS} -c error_funct.cpp

Exception.o	: Exception.cpp Exception.h
		${CXX} ${OPTIONS} -c Exception.cpp

MasterProcess.o : MasterProcess.cpp MasterProcess.h builtin_msg.h Child.h \
		  SharedMem.h txt.h Wrapper.h Message.h TCPConnection.h \
		  Exception.h
		${CXX} ${OPTIONS} -c MasterProcess.cpp

Message.o	: Message.cpp Message.h Exception.h
		${CXX} ${OPTIONS} -c Message.cpp

MessageProcessor.o: MessageProcessor.cpp MessageProcessor.h SharedMem.h txt.h \
		    Message.h Exception.h TCPConnection.h Wrapper.h
		${CXX} ${OPTIONS} -c MessageProcessor.cpp

Process.o       : Process.cpp Process.h txt.h builtin_msg.h exit_errors.h \
		  SharedMem.h Destroyed.h
		${CXX} ${OPTIONS} -c Process.cpp

non_blocking.o	: non_blocking.cpp non_blocking.h Exception.h
		${CXX} ${OPTIONS} -c non_blocking.cpp

SharedMem.o	: SharedMem.cpp SharedMem.h Exception.h Wrapper.h txt.h \
		  CircularPtr.h builtin_msg.h shm_lock.h debug.h Message.h
		${CXX} ${OPTIONS} -c SharedMem.cpp

shm_lock.o	: shm_lock.cpp shm_lock.h
		${CXX} ${OPTIONS} -c shm_lock.cpp

SlaveProcess.o  : SlaveProcess.cpp SlaveProcess.h builtin_msg.h txt.h \
		  Message.h TCPConnection.h Exception.h
		${CXX} ${OPTIONS} -c SlaveProcess.cpp

start_new.o	: start_new.cpp pce.h Wrapper.h
		${CXX} ${OPTIONS} -c start_new.cpp

TCPConnection.o : TCPConnection.cpp TCPConnection.h Wrapper.h non_blocking.h \
		  Message.h
		${CXX} ${OPTIONS} -c TCPConnection.cpp

Wrapper.o	: Wrapper.cpp Wrapper.h error_funct.h exit_errors.h \
		  connect_nonb.h accept_nonb.h
		${CXX} ${OPTIONS} -c Wrapper.cpp
