CPP=gcc
LIBRARY_NAME=../lib/libAlcubierre.a
INCLUDES=-I../include  -I/usr/local/include
CFLAGS = ${INCLUDES} -g -Wall -O2 -pipe -lm
LDFLAGS = -L/usr/local/lib
LDLIBS = -lGL -lGLU `sdl-config --cflags --libs` -lSDL_image

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
	rm ../lib/*
	rm obj/ai/*.o
	rm obj/audio/*.o
	rm obj/core/*.o
	rm obj/graphics/*.o
	rm obj/io/*.o
	rm obj/net/*.o
	rm obj/state/*.o
	rm obj/types/*.o
