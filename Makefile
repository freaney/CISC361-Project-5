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

t3.o: t3.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c t3.c

t3: t3.o t_lib.a Makefile
	${CC} ${CFLAGS} t3.o t_lib.a -o t3

t5.o: t5.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c t5.c

t5: t5.o t_lib.a Makefile
	${CC} ${CFLAGS} t5.o t_lib.a -o t5

t6.o: t6.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c t6.c

t6: t6.o t_lib.a Makefile
	${CC} ${CFLAGS} t6.o t_lib.a -o t6

t8.o: t8.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c t8.c

t8: t8.o t_lib.a Makefile
	${CC} ${CFLAGS} t8.o t_lib.a -o t8


t9.o: t9.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c t9.c

t9: t9.o t_lib.a Makefile
	${CC} ${CFLAGS} t9.o t_lib.a -o t9

t10.o: t10.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c t10.c

t10: t10.o t_lib.a Makefile
	${CC} ${CFLAGS} t10.o t_lib.a -o t10

t11.o: t11.c ud_thread.h Makefile
	${CC} ${CFLAGS} -c t11.c

t11: t11.o t_lib.a Makefile
	${CC} ${CFLAGS} t11.o t_lib.a -o t11

clean:
	rm -f t_lib.a *.o t1 tx t3 t5 t6 t8 t9 t10 t11 ${EXECS} ${LIBOBJS} ${TSTOBJS} 
