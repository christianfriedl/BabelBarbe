include Makefile.local
CFLAGS=-ggdb -Wall
LDFLAGS=-ggdb -L../ -ldasl -Wall

# 0190_rules
BINARIES=0050_tm 0070_basic_scanning 0100_token 0150_token_list 0170_token_list_container 0180_results 0200_basic_parsing 0210_more_parsing 0300_dasl_run


STATIC_LIBRARY=../libdasl.a

clean-all: clean all

all: clean $(BINARIES) $(STATIC_LIBRARY)

clean:
	rm -f *.o $(BINARIES) 

0050_tm: 0050_tm.o
	$(CC)  -o $@ $< $(LDFLAGS)
	
0070_basic_scanning: 0070_basic_scanning.o
	$(CC)  -o $@ $< $(LDFLAGS)
	
0100_token: 0100_token.o
	$(CC)  -o $@ $< $(LDFLAGS)
	
0150_token_list: 0150_token_list.o
	$(CC)  -o $@ $< $(LDFLAGS)
	
0170_token_list_container: 0170_token_list_container.o
	$(CC)  -o $@ $< $(LDFLAGS)
	
0180_results: 0180_results.o
	$(CC)  -o $@ $< $(LDFLAGS)
	
0190_rules: 0190_rules.o
	$(CC)  -o $@ $< $(LDFLAGS)
	
0200_basic_parsing: 0200_basic_parsing.o
	$(CC)  -o $@ $< $(LDFLAGS)

0210_more_parsing: 0210_more_parsing.o
	$(CC)  -o $@ $< $(LDFLAGS)

0300_dasl_run: 0300_dasl_run.o
	$(CC)  -o $@ $< $(LDFLAGS)
