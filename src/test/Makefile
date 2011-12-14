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

0125_Token: 0125_Token.o
	$(CC)  -o $@ $< $(LDFLAGS)

0187_Scanner: 0187_Scanner.o
	$(CC)  -o $@ $< $(LDFLAGS)

0250_Logger: 0250_Logger.o
	$(CC)  -o $@ $< $(LDFLAGS)

0500_ASTLeaf: 0500_ASTLeaf.o
	$(CC)  -o $@ $< $(LDFLAGS)

%: %.o
	$(CC)  -o $@ $< $(LDFLAGS)
