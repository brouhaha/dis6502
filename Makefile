OBJS = main.o initopts.o lex.o ref.o print.o tbl.o trace_queue.o
SRCS = dis.h main.c initopts.c lex.l ref.c print.c tbl.c trace_queue.c
CFLAGS = -g -Wall

dis6502:	$(OBJS)
		cc $(OBJS) -o dis6502

tbl.o:		dis.h tbl.c
#		cc -c tbl.c

initopts.o:	dis.h initopts.c

main.o:		dis.h main.c

lex.o:		lex.c

lex.c:		dis.h lex.l

ref.o:		dis.h ref.c

print.o:	dis.h print.c

trace_queue.o:	dis.h trace_queue.c

dis.man:	dis.1
		nroff -man dis.1 > dis.man

install:	dis
		cp dis /a/rgb/bin/dis6502

clean:
		rm -f $(OBJS) lex.c dis.man

clobber:	clean
		rm -f dis6502

ckpt:		$(SRCS)
		ci -l $(SRCS)

lint: dis.h main.c initopts.c lex.c ref.c print.c tbl.c 
		lint  dis.h main.c initopts.c lex.c ref.c print.c tbl.c 

# shar:		Makefile dis.1 $(SRCS)
# 		shar -f shar Makefile dis.1 $(SRCS)
