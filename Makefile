include config.mk

SRC = sbs.c util.c
OBJ = ${SRC:.c=.o}

all: sbs

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $<

sbs: ${OBJ}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

clean:
	rm ${OBJ} sbs *.rej *.orig

.PHONY: all clean
