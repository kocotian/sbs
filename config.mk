VERSION = 0.1

INC = -Iinclude
CFLAGS = -Wall -Wextra -Wconversion -pedantic -std=c99 ${INC} \
		 -D_XOPEN_SOURCE=700 -DVERSION=\"${VERSION}\"
LDFLAGS =

CC = gcc
