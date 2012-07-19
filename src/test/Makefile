include Makefile.local
CFLAGS=-ggdb -save-temps -Wall -I../ -I/opt/local/include
LDFLAGS=-ggdb -lcgenerics -L../ -lbbarbe -L/opt/local/lib -lpcre -Wall

BINARIES=0125_Token 0140_BBScannerNode 0160_BBScannerRule 0187_Scanner 0200_BBScannerWithRuleset 0500_BBAst 0750_BB_RDParser 0800_BBParserWithRuleset 0900_BBCodeGenerator
CURRENT_BINARIES=0900_BBCodeGenerator


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

calcMain.o: calcMain.c

calcRuleset.o: calcRuleset.c

calc: calcMain.o calcRuleset.o
	$(CC)  -o calc calcMain.o calcRuleset.o $(LDFLAGS)
	rm -f *.i *.s

tags: $(BINARIES)
	ctags *.c ../../../CGenerics/src/*.{c,h}
