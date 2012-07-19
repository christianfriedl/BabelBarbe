include Makefile.local
CFLAGS=-ggdb -save-temps -Wall -I../ -I/opt/local/include
LDFLAGS=-ggdb -lcgenerics -L../ -lbbarbe -L/opt/local/lib -lpcre -Wall

BINARIES=0125_Token 0140_BNFScannerNode 0160_BNFScannerRule 0187_Scanner 0200_BNFScannerWithRuleset 0500_BNFAst 0750_BNF_RDParser 0800_BNFParserWithRuleset 0900_BNFCodeGenerator
CURRENT_BINARIES=0900_BNFCodeGenerator


STATIC_LIBRARY=../libbbarbe.a

current: clean $(CURRENT_BINARIES)
	for i in $(CURRENT_BINARIES); do ./$$i; if [ $$? != 0 ]; then exit; fi; done

clean-all: all

all: clean $(BINARIES) $(STATIC_LIBRARY) tags

clean:
	rm -f *.o $(BINARIES) 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $?

%: %.o
	$(CC)  -o $@ $< $(LDFLAGS)
	rm -f *.i *.s

tags: $(BINARIES)
	ctags *.c ../../../CGenerics/src/*.{c,h}
