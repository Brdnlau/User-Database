CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: businessdb

businessdb: businessdb.o
	$(CC) -o businessdb businessdb.o

businessdb.o: businessdb.c businessdb.h
	$(CC) $(CFLAGS) -c businessdb.c

clean:
	rm -f businessdb businessdb.o

format:
	clang-format -i -style=file *.[ch]
