CXXFLAGS+=-Wall -Wextra -g
LDFLAGS+=-lmuduo_net -lmuduo_base -pthread 
CC = g++

target:client

client:client_main.o client.o lengthCodec.o
	${CC} ${CXXFLAGS} -o client client_main.o client.o lengthCodec.o ${LDFLAGS}

client_main.o:client_main.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

client.o:client.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

lengthCodec.o:lengthCodec.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

.phony:
clean:
	rm -rf ./*.o client