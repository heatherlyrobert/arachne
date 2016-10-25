#===(source start)======================================================================================================================================================#



#===[[ VARABLES ]]======================================================================================================================================================#

#===(current variables)=================================================================================================================================================#
BASE    = arachne
DEBUG   = ${BASE}_debug
UNIT    = ${BASE}_unit
HDIR    = /home/monkey/arachne.spider_visualization
IDIR    = /usr/local/bin
MDIR    = /usr/share/man/man1

#===(compilier variables)===============================================================================================================================================#
# must have "-x c" on gcc line so stripped files work with alternate extensions
COMP    = gcc -c -std=gnu89 -x c -g -pg -Wall -Wextra
INCS    = -I/usr/local/include 

#===(linker options)========================================================================================================================================================================#
#------   (0)-------------- (1)-------------- (2)-------------- (3)-------------- (4)-------------- (5)-------------- (6)-------------- (7)-------------- (8)-------------- (9)--------------
LINK    = gcc
LIBDIR  = -L/usr/local/lib
LIBS    = ${LIBDIR}         -lX11             -lGL              -lGLU             -lyX11            -lyGOD            -lyFONT           -lySTR            -lyVAR            -lm
LIBD    = ${LIBS}           -lyLOG
LIBU    = ${LIBS}           -lyLOG            -lyUNIT           -lyVAR

#===(file lists)============================================================================================================================================================================#
#------   (0)-------------- (1)-------------- (2)-------------- (3)-------------- (4)-------------- (5)-------------- (6)-------------- (7)-------------- (8)-------------- (9)-------------- (A)-------------- (B)-------------- (C)-------------- (D)-------------- (5)--------------
HEADS   = ${BASE}.h
OBJS    = ${BASE}_main.os   ${BASE}_prog.os   ${BASE}_kine.os   ${BASE}_draw.os   ${BASE}_dlist.os  ${BASE}_scrp.os   ${BASE}_stat.os   ${BASE}_dyna.os   ${BASE}_test.os   ${BASE}_test.os   ${BASE}_curs.os   ${BASE}_file.os   ${BASE}_loc.os


OBJD    = ${BASE}_main.o    ${BASE}_prog.o    ${BASE}_move.o    ${BASE}_keys.o    ${BASE}_visu.o    ${BASE}_reg.o     ${BASE}_calc.o    ${BASE}_rpn.o     ${BASE}_dep.o     ${BASE}_cell.o    ${BASE}_curs.o    ${BASE}_file.o    ${BASE}_loc.o 
OBJU    = ${BASE}_unit.o    ${BASE}_prog.o    ${BASE}_move.o    ${BASE}_keys.o    ${BASE}_visu.o    ${BASE}_reg.o     ${BASE}_calc.o    ${BASE}_rpn.o     ${BASE}_dep.o     ${BASE}_cell.o    ${BASE}_curs.o    ${BASE}_file.o    ${BASE}_loc.o

#===(make variables)====================================================================================================================================================#
COPY    = cp -f
CLEAN   = rm -f
PRINT   = @printf
STRIP   = @grep -v -e " DEBUG_" -e " yLOG_" 





TEST    = arachne_unit

LINK    = gcc 
OBJS    = ${BASE}_kine.o ${BASE}_draw.o ${BASE}_dlist.o ${BASE}_stat.o ${BASE}_dyna.o ${BASE}_scrp.o
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
