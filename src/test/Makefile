include Makefile.local
CFLAGS=-ggdb -Wall
LDFLAGS=-ggdb -L../ -lbnf -Wall

BINARIES=0500_ASTLeaf


STATIC_LIBRARY=../libbnf.a

clean-all: clean all

all: clean $(BINARIES) $(STATIC_LIBRARY)

clean:
	rm -f *.o $(BINARIES) 

0500_ASTLeaf: 0500_ASTLeaf.o
	$(CC)  -o $@ $< $(LDFLAGS)
