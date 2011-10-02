include Makefile.local
NORMAL_WARNINGS = -Wall
MEGA_WARNINGS = -Wall -Wdeclaration-after-statement \
	-Wredundant-decls -Wmissing-noreturn -Wshadow -Wcast-align \
	-Wwrite-strings -Winline -Wformat-nonliteral -Wformat-security \
	-Wswitch -Wswitch-default -Winit-self -Wmissing-include-dirs \
	-Wundef -Waggregate-return -Wmissing-format-attribute \
	-Wnested-externs -Werror
WARNINGS = $(NORMAL_WARNINGS)
CFLAGS=-ggdb $(WARNINGS)
LDFLAGS=-ggdb $(WARNINGS) -lreadline


STATIC_LIBRARY=libdasl.a
IDASL=idasl
LIB_OBJECTS = scanner.o parser.o token.o token_list.o token_list_container.o dasl.o tm.o results.o 

clean-all-test: clean all test

all-test: all test

idasl: idasl.o $(STATIC_LIBRARY)

install: idasl localall
	cp libdasl.a /opt/local/lib/
	cp idasl /opt/local/bin/

localall: $(STATIC_LIBRARY)

all: localall
	(cd test; make all)

$(STATIC_LIBRARY): $(LIB_OBJECTS)
	ar -rcsv $@ $?

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $?

clean:
	rm -f *.o $(TEST_BINARIES) $(STATIC_LIBRARY) $(IDASL)
	(cd test; make clean)

.PHONY: test
test:
	for i in `find test -type f -not -name "*.*" -name "0*" |sort -n`; do $$i; if [ $$? != 0 ]; then exit; fi; done
