LIBS=../common/fit_results.o
CGILIB=-L/usr/local/lib -lcgic
HEADERS=../include/*.h
COMMON=
INC=../include
INCLUDES=-I../include -I/usr/local/include
DEBUG=-DDEBUG
OBJECTS=googlev1.o
CC=gcc
STRIP=strip
TESTDIR=../tests/
CFLAGS+=-std=c99
all: $(OBJECTS) $(PROGS)

googlev1.o: googlev1.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEBUG) googlev1.c -c

clean:
	rm -f $(OBJECTS) $(PROGS) *~