include Makefile.local
CFLAGS=-ggdb -Wall -I../
LDFLAGS=-ggdb -L../ -lbnf -Wall

BINARIES=0125_Token 0187_Scanner 0250_Logger 0500_ASTLeaf


STATIC_LIBRARY=../libbnf.a

clean-all: clean all

all: clean $(BINARIES) $(STATIC_LIBRARY)

clean:
	rm -f *.o $(BINARIES) 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $?

%: %.o
	$(CC)  -o $@ $< $(LDFLAGS)
