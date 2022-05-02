CXXFLAGS+=-Wall -Wextra -g
LDFLAGS+=-lmuduo_net -lmuduo_base -pthread 
CC = g++

target:server client

server:server_main.o server.o
	${CC} ${CXXFLAGS} -o server server_main.o server.o ${LDFLAGS}

server_main.o:server_main.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

server.o:server.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

client:client_main.o client.o
	${CC} ${CXXFLAGS} -o client client_main.o client.o ${LDFLAGS}

client_main.o:client_main.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

client.o:client.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

.phony:
clean:
	rm -rf ./*.o server client