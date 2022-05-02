CXXFLAGS+=-Wall -Wextra
LDFLAGS+=-lmuduo_net -lmuduo_base -pthread 
CC = g++

target:main

main:main.o server.o
	${CC} ${CXXFLAGS} -o $@ $? ${LDFLAGS}

main.o:main.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

server.o:server.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

.phony:
clean:
	rm -rf ./*.o ./main