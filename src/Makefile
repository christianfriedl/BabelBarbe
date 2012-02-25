include Makefile.local
NORMAL_WARNINGS = -Wall
MEGA_WARNINGS = -Wall -Wdeclaration-after-statement \
	-Wredundant-decls -Wmissing-noreturn -Wshadow -Wcast-align \
	-Wwrite-strings -Winline -Wformat-nonliteral -Wformat-security \
	-Wswitch -Wswitch-default -Winit-self -Wmissing-include-dirs \
	-Wundef -Waggregate-return -Wmissing-format-attribute \
	-Wnested-externs -Werror
WARNINGS = $(NORMAL_WARNINGS)
CFLAGS=-ggdb -save-temps -I/opt/local/include $(WARNINGS)
LDFLAGS=-ggdb -L/opt/local/lib -lpcre $(WARNINGS) 


STATIC_LIBRARY=libbnf.a 
LIB_OBJECTS =  BNF.o BNFToken.o BNFScanner.o BNFScannerRule.o CGArrayOfBNFScannerNode.o BNFScannerRuleset.o
# ASTLeaf.o Logger.o Token.o Scanner.o BNF.o

all-test: all tags test

clean-all-test: clean all tags test

localall: $(STATIC_LIBRARY)

all: localall
	(cd test; make all)
	rm -f *.i *.s

$(STATIC_LIBRARY): $(LIB_OBJECTS)
	ar -rcsv $@ $?

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $?

clean:
	rm -f *.o $(TEST_BINARIES) $(STATIC_LIBRARY) *.e.c *.BAK *.i *.s
	(cd test; make clean)

.PHONY: test
test:
	rm -rf test/*.dSYM
	rm -rf *.dSYM
	for i in `find test -type f -not -name "*.*" -name "0*" |sort -n`; do $$i; if [ $$? != 0 ]; then exit; fi; done

pp:
	for i in $(LIB_OBJECTS); do base=`echo $$i|perl -pe"s/\.o//"`; gcc $(CFLAGS) -E -o $$base.e.c -c $$base.c; done 

pp-indent: pp
	for i in $(LIB_OBJECTS); do base=`echo $$i|perl -pe"s/\.o//"`; indent -bad -bap -i4 $$base.e.c && perl -pi -e"s/^#\d+.+$$//;" $$base.e.c && gcc -o $$base.e.o -c $$base.e.c; done

tags: $(LIB_OBJECTS)
	ctags *.{c,h} ../../CGenerics/src/*.{c,h}
	(cd test; make tags)
