CPP=gcc
LIBRARY_NAME=../lib/libAlcubierre.a
INCLUDES=-I../include  -I/usr/local/include
CFLAGS = ${INCLUDES} -g -Wall -O2 -pipe -lm
LDFLAGS = -L/usr/local/lib
LDLIBS = -lGL -lGLU `sdl-config --cflags --libs`

SRC := $(shell echo */*.c)
OBJS := $(addprefix obj/, $(SRC:.c=.o))

${LIBRARY_NAME}: ${OBJS}
	rm -f $@
	ar cq $@ ${OBJS}

all: ${OBJS}
	$(CPP) ${OBJS} ${LDFLAGS} ${LDLIBS}

obj/%.o: %.c
	${CPP} -c -o $@ ${CFLAGS} $<

clean:
	rm ../libs/*
	rm obj/*.o
