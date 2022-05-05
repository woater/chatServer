CXXFLAGS+=-Wall -Wextra -g
LDFLAGS+=-lmuduo_net -lmuduo_base -pthread 
CC = g++

target:loadTest

loadTest:loadTest.o lengthCodec.o
	${CC} ${CXXFLAGS} -o $@ loadTest.o lengthCodec.o ${LDFLAGS}

loadTest.o:loadTest.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

lengthCodec.o:lengthCodec/lengthCodec.cpp
	${CC} ${CXXFLAGS} -c -o $@ $? ${LDFLAGS}

.phony:
clean:
	rm -rf ./*.o loadTest