SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%,$(SRCS))
CC=gcc
CFLAGS=-lpthread 

all: $(PROGS) 

%: %.c
	$(CC) $< $(CFLAGS)  -o $@ 

clean: $(PROGS)
	rm $(PROGS)
	rm -f questions_from_ta4.pdf questions_from_ta4.out questions_from_ta4.aux questions_from_ta4.log

questions: questions_from_ta4.tex
	pdflatex questions_from_ta4.tex >> /dev/null

