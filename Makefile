#================================[[ beg-code ]]================================#



#===(user defined)=========================================#
# basename of executable, header, and all code files
NAME_BASE  = arachne
# additional standard and outside libraries
LIB_STD    = -lm -lX11 -lGL -lGLU
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lyX11_debug -lyGOD_debug -lyKINE_debug -lyGOLEM_debug -lyFONT_debug -lySTR_debug -lyVIKEYS_debug -lyCOLOR_debug -lyGLTEX_debug
# directory for production code, no trailing slash
INST_DIR   = /usr/local/bin


#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
#install_post       :
#remove_post        :



#================================[[ end-code ]]================================#
