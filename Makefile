 CC = gcc
CFLAGS = -Iinclude -mconsole  -Wall -Wextra -Werror -pedantic -O0

all: a.exe

a.exe: src/*.c
    $(CC) $(CFLAGS) src/*.c -o a.exe

clean:
    rm -f a.exe