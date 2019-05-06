# Makefile for UD CISC user-level thread library

CC = gcc
CFLAGS = -g

LIBOBJS = t_lib.o 

TSTOBJS = test00.o 

# specify the executable 

EXECS = test00

# specify the source files

LIBSRCS = t_lib.c

TSTSRCS = test00.c

# ar creates the static thread library

t_lib.a: ${LIBOBJS} Makefile
	ar rcs t_lib.a ${LIBOBJS}

# here, we specify how each file should be compiled, what
# files they depend on, etc.

t_lib.o: t_lib.c t_lib.h Makefile
	${CC} ${CFLAGS} -c t_lib.c

test00.o: test00.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c test00.c

test00: test00.o t_lib.a Makefile
	${CC} ${CFLAGS} test00.o t_lib.a -o test00

t1.o: t1.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c t1.c

t1: t1.o t_lib.a Makefile
	${CC} ${CFLAGS} t1.o t_lib.a -o t1

tx.o: tx.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c tx.c

tx: tx.o t_lib.a Makefile
	${CC} ${CFLAGS} tx.o t_lib.a -o tx

clean:
	rm -f t_lib.a *.o t1 tx ${EXECS} ${LIBOBJS} ${TSTOBJS} 
