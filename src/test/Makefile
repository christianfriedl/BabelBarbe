include Makefile.local
CFLAGS=-ggdb -Wall -I../ -I/opt/local/include
LDFLAGS=-ggdb -lcgenerics -L../ -lbnf -L/opt/local/lib -lpcre -Wall

BINARIES=0125_Token 0140_BNFScannerNode 0160_BNFScannerRule 0187_Scanner # 0250_Logger 0500_ASTLeaf


STATIC_LIBRARY=../libbnf.a

clean-all: clean all

all: clean $(BINARIES) $(STATIC_LIBRARY)

clean:
	rm -f *.o $(BINARIES) 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $?

%: %.o
	$(CC)  -o $@ $< $(LDFLAGS)
