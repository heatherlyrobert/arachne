#*============================----beg-of-source---============================*#



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
OBJS    = ${BASE}_main.os   ${BASE}_prog.os   ${BASE}_keys.os   ${BASE}_kine.os   ${BASE}_draw.os   ${BASE}_dlist.os  ${BASE}_move.os  ${BASE}_scrp.os   ${BASE}_stat.os   ${BASE}_dyna.os   ${BASE}_test.os
OBJD    = ${BASE}_main.o    ${BASE}_prog.o    ${BASE}_keys.o    ${BASE}_kine.o    ${BASE}_draw.o    ${BASE}_dlist.o   ${BASE}_move.o   ${BASE}_scrp.o    ${BASE}_stat.o    ${BASE}_dyna.o    ${BASE}_test.o 
OBJU    = ${BASE}_unit.o    ${BASE}_prog.o    ${BASE}_keys.o    ${BASE}_kine.o    ${BASE}_draw.o    ${BASE}_dlist.o   ${BASE}_move.o   ${BASE}_scrp.o    ${BASE}_stat.o    ${BASE}_dyna.o    ${BASE}_test.o 

#===(make variables)====================================================================================================================================================#
COPY    = cp -f
CLEAN   = rm -f
PRINT   = @printf
STRIP   = @grep -v -e " DEBUG_" -e " yLOG_" 



#===[[ EXECUTABLES ]]===================================================================================================================================================#

all                : ${DEBUG} ${BASE} ${UNIT}

# BASE must check OBJD rather than OBJS to compile stripped version correctly 
${BASE}            : ${OBJD}
	${LINK}  -o ${BASE}        ${OBJS}   ${LIBS}

${DEBUG}           : ${OBJD}
	${LINK}  -o ${DEBUG}       ${OBJD}   ${LIBD}

${UNIT}            : ${OBJU}
	${LINK}  -o ${UNIT}        ${OBJU}   ${LIBU}



#===[[ OBJECT FILES ]]==================================================================================================================================================#

${BASE}_main.o     : ${HEADS}       ${BASE}_main.c
	${COMP}    ${BASE}_main.c                           ${INC}
	${STRIP}   ${BASE}_main.c      > ${BASE}_main.cs
	${COMP}    ${BASE}_main.cs    -o ${BASE}_main.os    ${INC}

${BASE}_prog.o     : ${HEADS}       ${BASE}_prog.c
	${COMP}    ${BASE}_prog.c                           ${INC}
	${STRIP}   ${BASE}_prog.c      > ${BASE}_prog.cs
	${COMP}    ${BASE}_prog.cs    -o ${BASE}_prog.os    ${INC}

${BASE}_keys.o     : ${HEADS}       ${BASE}_keys.c
	${COMP}    ${BASE}_keys.c                           ${INC}
	${STRIP}   ${BASE}_keys.c      > ${BASE}_keys.cs
	${COMP}    ${BASE}_keys.cs    -o ${BASE}_keys.os    ${INC}

${BASE}_kine.o     : ${HEADS}       ${BASE}_kine.c
	${COMP}    ${BASE}_kine.c                           ${INC}
	${STRIP}   ${BASE}_kine.c      > ${BASE}_kine.cs
	${COMP}    ${BASE}_kine.cs    -o ${BASE}_kine.os    ${INC}

${BASE}_draw.o     : ${HEADS}       ${BASE}_draw.c
	${COMP}    ${BASE}_draw.c                           ${INC}
	${STRIP}   ${BASE}_draw.c      > ${BASE}_draw.cs
	${COMP}    ${BASE}_draw.cs    -o ${BASE}_draw.os    ${INC}

${BASE}_dlist.o    : ${HEADS}       ${BASE}_dlist.c
	${COMP}    ${BASE}_dlist.c                          ${INC}
	${STRIP}   ${BASE}_dlist.c     > ${BASE}_dlist.cs
	${COMP}    ${BASE}_dlist.cs   -o ${BASE}_dlist.os   ${INC}

${BASE}_move.o     : ${HEADS}       ${BASE}_move.c
	${COMP}    ${BASE}_move.c                           ${INC}
	${STRIP}   ${BASE}_move.c      > ${BASE}_move.cs
	${COMP}    ${BASE}_move.cs    -o ${BASE}_move.os    ${INC}

${BASE}_scrp.o     : ${HEADS}       ${BASE}_scrp.c
	${COMP}    ${BASE}_scrp.c                           ${INC}
	${STRIP}   ${BASE}_scrp.c      > ${BASE}_scrp.cs
	${COMP}    ${BASE}_scrp.cs    -o ${BASE}_scrp.os    ${INC}

${BASE}_stat.o     : ${HEADS}       ${BASE}_stat.c
	${COMP}    ${BASE}_stat.c                           ${INC}
	${STRIP}   ${BASE}_stat.c      > ${BASE}_stat.cs
	${COMP}    ${BASE}_stat.cs    -o ${BASE}_stat.os    ${INC}

${BASE}_dyna.o     : ${HEADS}       ${BASE}_dyna.c
	${COMP}    ${BASE}_dyna.c                           ${INC}
	${STRIP}   ${BASE}_dyna.c      > ${BASE}_dyna.cs
	${COMP}    ${BASE}_dyna.cs    -o ${BASE}_dyna.os    ${INC}

${BASE}_test.o     : ${HEADS}       ${BASE}_test.c
	${COMP}    ${BASE}_test.c                           ${INC}
	${STRIP}   ${BASE}_test.c      > ${BASE}_test.cs
	${COMP}    ${BASE}_test.cs    -o ${BASE}_test.os    ${INC}

${BASE}_unit.o     : ${BASE}.unit
	koios    ${BASE}
	${COMP}  ${BASE}_unit.c



#===[[ SCRIPTS ]]=======================================================================================================================================================#

clean              :
	#---(all versions)--------------------#
	${CLEAN} ${BASE}
	${CLEAN} ${DEBUG}
	${CLEAN} ${UNIT}
	#---(object and stripped files)-------#
	${CLEAN} ${BASE}*.o
	${CLEAN} ${BASE}*.cs
	${CLEAN} ${BASE}*.os
	${CLEAN} ${BASE}*.Sc
	${CLEAN} ${BASE}*.So
	#---(created unit code)---------------#
	${CLEAN} ${UNIT}.code
	${CLEAN} ${UNIT}.c
	#---(junk files)----------------------#
	${CLEAN} *~
	${CLEAN} temp*

bigclean           :
	${CLEAN} .*.swp

install            : ${BASE}
	#---(production version)--------------#
	${COPY}   ${BASE}    ${IDIR}/
	chown     root:root  ${IDIR}/${BASE}
	chmod     0755       ${IDIR}/${BASE}
	@sha1sum  ${BASE}
	#---(debug version)-------------------#
	${COPY}  ${DEBUG}    ${IDIR}/
	chown     root:root  ${IDIR}/${DEBUG}
	chmod     0755       ${IDIR}/${DEBUG}
	@sha1sum  ${DEBUG}
	#---(man page)------------------------#
	rm -f       ${MDIR}/${BASE}.1.bz2
	cp -f       ${BASE}.1  ${MDIR}/
	bzip2       ${MDIR}/${BASE}.1
	chmod 0644  ${MDIR}/${BASE}.1.bz2

remove             :
	#---(all versions)--------------------#
	${CLEAN}  ${IDIR}/${BASE}
	${CLEAN}  ${IDIR}/${DEBUG}
	#---(documentation)-------------------#
	${CLEAN}  ${MDIR}/${BASE}.1.bz2



#*============================----end-of-source---============================*#
