CXXFLAGS+=-Wall -Wextra -O2
LDFLAGS+=-lmuduo_net -lmuduo_base -pthread 
CC = g++

target:server

server:server_main.o server.o lengthCodec.o
	${CC} ${CXXFLAGS} -o server server_main.o server.o lengthCodec.o ${LDFLAGS}

server_main.o:server_main.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

server.o:server.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

lengthCodec.o:lengthCodec.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

.phony:
clean:
	rm -rf ./*.o server