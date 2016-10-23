BASE    = arachne
TEST    = arachne_unit

COMP    = gcc -std=gnu89 -Wall -Wextra -c
LINK    = gcc 
OBJS    = ${BASE}_kine.o ${BASE}_draw.o ${BASE}_dlist.o ${BASE}_stat.o ${BASE}_dyna.o ${BASE}_scrp.o
INCS    = -I/usr/local/include
LIBS    = -L/usr/local/lib -lyX11 -lyGOD -lyVAR -lyFONT -lX11 -lGL -lGLU -lm


COPY    = cp -f
CLEAN   = rm -f
ECHO    = @echo


##---(objects)----------------------------#

all                     : ${BASE}

${BASE}                 : ${BASE}_main.o ${OBJS}
	${LINK}   -o ${BASE}  ${BASE}_main.o ${OBJS}  ${LIBS}

test                    : ${BASE}_unit.o ${BASE}_test.o ${OBJS}
	${LINK}   -o ${TEST}  ${BASE}_unit.o ${BASE}_test.o ${OBJS}  ${LIBS} -lyUNIT -lyVAR



##---(objects)----------------------------#

${BASE}_main.o          : ${BASE}.h ${BASE}_main.c
	${COMP}  ${BASE}_main.c

${BASE}_kine.o          : ${BASE}.h ${BASE}_kine.c
	${COMP}  ${BASE}_kine.c

${BASE}_draw.o          : ${BASE}.h ${BASE}_draw.c
	${COMP}  ${BASE}_draw.c

${BASE}_dlist.o         : ${BASE}.h ${BASE}_dlist.c
	${COMP}  ${BASE}_dlist.c

${BASE}_scrp.o          : ${BASE}.h ${BASE}_scrp.c
	${COMP}  ${BASE}_scrp.c

${BASE}_stat.o          : ${BASE}.h ${BASE}_stat.c
	${COMP}  ${BASE}_stat.c

${BASE}_dyna.o          : ${BASE}.h ${BASE}_dyna.c
	${COMP}  ${BASE}_dyna.c

${BASE}_test.o          : ${BASE}.h ${BASE}_test.c
	${COMP}  ${BASE}_test.c

${BASE}_unit.o          : ${BASE}.h ${BASE}.unit
	uUNIT    ${BASE}
	mv ${TEST}.code ${TEST}.c
	${COMP}  ${BASE}_unit.c  ${INCS}




bigclean                :
	${CLEAN} '.'*.swp

clean                   :
	${CLEAN} *.o
	${CLEAN} *~
	${CLEAN} temp*
	${CLEAN} ${BASE}

install                 : ${BASE}
	${ECHO}  installing in b_nvdo
	${COPY}  ${BASE}  /usr/local/bin
	chmod    0755     /usr/local/bin/${BASE}
