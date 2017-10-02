###################################################################################################
#
#
#   ======================    -----------------------------------------
#   verbexx_base_header.mk -- included makefile with header file groups (use with Windows command-line nmake)
#   ======================    -----------------------------------------
#
#
###################################################################################################
 
                   
################### headers #######################################################################

PCH_HDRS = \
$(INC_DIR_CORE)h__include.h \
$(INC_DIR_CORE)h__utf32.h      

COMM_HDRS = \
$(INC_DIR_CORE)h__common.h \
$(INC_DIR_CORE)h__dll.h \
$(INC_DIR_CORE)h__global.h \
$(INC_DIR_CORE)h__include.h \
$(INC_DIR_CORE)h__types.h \
$(INC_DIR_CORE)h_core_clr.h \
$(INC_DIR_CORE)h_core_template.h \
$(INC_DIR_CORE)h_core_util.h 
          
EX_HDR_IF = \
$(INC_DIR_EX)h_ex_interface.h 

EX_HDRS = \
$(INC_DIR_EX)h_ex_interface.h \
$(INC_DIR_EX)h_ex_lex.h \
$(INC_DIR_EX)h_ex_parse.h \
$(INC_DIR_EX)h_ex_types.h \
$(INC_DIR_EX)h_ex_verb.h \
$(INC_DIR_EX)h_ex_verbmac.h 


############### pre-compiled headers ################################################################
  
PCH_STD = \
$(PCH_DIR)pch_std.pch

PCH_CLR = \
$(PCH_DIR)pch_clr.pch  


################################################################################################################################  