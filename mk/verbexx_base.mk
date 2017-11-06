####################################################################################################################
#
#    =============== 
#    verbexx_base.mk - makefile for Windows command-line nmake
#    ===============
#
#
#  makes:   verbexx.exe 
#           core.dll
#           ex.dll
#
####################################################################################################################
                  
EXNAME = verbexx

!INCLUDE $(EXNAME)_folder.mk
!INCLUDE $(EXNAME)_implib.mk
!INCLUDE $(EXNAME)_base_header.mk

MK = \
$(MK_DIR)$(EXNAME)_base.mk \
$(MK_DIR)$(EXNAME)_folder.mk \
$(MK_DIR)$(EXNAME)_implib.mk \
$(MK_DIR)$(EXNAME)_base_header.mk 

             
############# compile/link command options #########################################################################

COMPILE    = cl
LINK       = link
SEPARATOR  = @echo ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CFLAGS           = $(CFLAGS_ALWAYS) /Yu  /nologo              /I$(INC_DIR_CORE) /I$(INC_DIR_EX)     /Fo$(OBJ_DIR) /Fp$(PCH_DIR)pch_std.pch
CFLAGS_CLR       = $(CFLAGS_ALWAYS) /Yu  /nologo         /clr /I$(INC_DIR_CORE)                     /Fo$(OBJ_DIR) /Fp$(PCH_DIR)pch_clr.pch
CFLAGS_PCH       = $(CFLAGS_ALWAYS) /Yc  /showIncludes        /I$(INC_DIR_CORE)                     /Fo$(OBJ_DIR) /Fp$(PCH_DIR)pch_std.pch
CFLAGS_PCHCLR    = $(CFLAGS_ALWAYS) /Yc                  /clr /I$(INC_DIR_CORE)                     /Fo$(OBJ_DIR) /Fp$(PCH_DIR)pch_clr.pch   

LFLAGS     = /STACK:$(STACK_SIZE) /NOLOGO /LIBPATH:$(LIB_DIR)      
LFLAGS_DLL = /STACK:$(STACK_SIZE) /NOLOGO /LIBPATH:$(LIB_DIR) /DLL 


#####################################################################################################################

PCH_OBJS = \
$(OBJ_DIR)pch_std.obj \
$(OBJ_DIR)pch_clr.obj 


EXE_OBJS = \
$(OBJ_DIR)main.obj 


CORE_DLL_OBJS = \
$(OBJ_DIR)core_dll.obj \
$(OBJ_DIR)core_clr.obj \
$(OBJ_DIR)core_error.obj \
$(OBJ_DIR)core_msdn.obj \
$(OBJ_DIR)core_sys.obj \
$(OBJ_DIR)core_util.obj  


EX_DLL_OBJS = \
$(OBJ_DIR)ex_dll.obj \
$(OBJ_DIR)ex_addverb.obj \
$(OBJ_DIR)ex_data.obj \
$(OBJ_DIR)ex_display.obj \
$(OBJ_DIR)ex_environ.obj \
$(OBJ_DIR)ex_eval.obj \
$(OBJ_DIR)ex_inchar.obj \
$(OBJ_DIR)ex_infile.obj \
$(OBJ_DIR)ex_interface.obj \
$(OBJ_DIR)ex_intoken.obj \
$(OBJ_DIR)ex_main.obj \
$(OBJ_DIR)ex_parse.obj \
$(OBJ_DIR)ex_preprocess.obj \
$(OBJ_DIR)ex_verb_ctl.obj \
$(OBJ_DIR)ex_verb_op.obj \
$(OBJ_DIR)ex_verb_util.obj  


######################################################################################################################
       
EXE_LIBS = \
$(LIB_DIR)$(EXNAME)_ex.lib 

EX_LIBS = \
$(LIB_DIR)$(EXNAME)_core.lib 


######################################################################################################################
#                                phony make target 
######################################################################################################################

ALL = \
$(DLL_DIR)$(EXNAME)_core.dll \
$(DLL_DIR)$(EXNAME)_ex.dll \
$(EXE_DIR)$(EXNAME).exe  

# show all DLLs in directory

all : $(ALL)
  dir $(DLL_DIR)  
   
    
######################################################################################################################
#                                    link commands 
######################################################################################################################

                    
#############  verbexx.exe  link command ############################################################################# 

$(EXE_DIR)$(EXNAME).exe                              : $(MK) $(PCH_OBJS) $(EXE_OBJS)                                    $(EXE_LIBS) 
  $(SEPARATOR)                                
  $(LINK) $(LFLAGS)                                          $(PCH_OBJS) $(EXE_OBJS)                        $(IMP_LIBS) $(EXE_LIBS)    /OUT:$(EXE_DIR)$(EXNAME).exe 
  $(SEPARATOR) 
  

#############  *.dll  link commands ##################################################################################

$(LIB_DIR)core.lib $(DLL_DIR)$(EXNAME)_core.dll      : $(MK) $(PCH_OBJS) $(CORE_DLL_OBJS)                
  $(SEPARATOR)   
  $(LINK) $(LFLAGS_DLL)                                      $(PCH_OBJS) $(CORE_DLL_OBJS)    $(IMP_LIBS)                               /OUT:$(DLL_DIR)$(EXNAME)_core.dll  /IMPLIB:$(LIB_DIR)$(EXNAME)_core.lib
  $(SEPARATOR) 
# dumpbin /EXPORTS $(DLL_DIR)$(EXNAME)_core.dll
 
$(LIB_DIR)ex.lib $(DLL_DIR)$(EXNAME)_ex.dll          : $(MK) $(PCH_OBJS) $(EX_DLL_OBJS)                  $(EX_LIBS)
  $(SEPARATOR)   
  $(LINK) $(LFLAGS_DLL)                                      $(PCH_OBJS) $(EX_DLL_OBJS)      $(IMP_LIBS) $(EX_LIBS)                    /OUT:$(DLL_DIR)$(EXNAME)_ex.dll    /IMPLIB:$(LIB_DIR)$(EXNAME)_ex.lib
  $(SEPARATOR) 
# dumpbin /EXPORTS $(DLL_DIR)$(EXNAME)_ex.dll


######################################################################################################################
#                                    compile commands 
######################################################################################################################


############# compile commands for PCH_OBJS ##########################################################################
 
$(PCH_DIR)pch_std.pch          : $(SRC_DIR_CORE)pch_std.cpp          $(MK)  $(PCH_HDRS)
  $(COMPILE) /c $(CFLAGS_PCH)    $(SRC_DIR_CORE)pch_std.cpp

$(OBJ_DIR)pch_std.obj          : $(SRC_DIR_CORE)pch_std.cpp          $(MK)  $(PCH_HDRS)
  $(COMPILE) /c $(CFLAGS_PCH)    $(SRC_DIR_CORE)pch_std.cpp

$(PCH_DIR)pch_clr.pch          : $(SRC_DIR_CORE)pch_clr.cpp          $(MK)  $(PCH_HDRS)  
  $(COMPILE) /c $(CFLAGS_PCHCLR) $(SRC_DIR_CORE)pch_clr.cpp

$(OBJ_DIR)pch_clr.obj          : $(SRC_DIR_CORE)pch_clr.cpp          $(MK)  $(PCH_HDRS)  
  $(COMPILE) /c $(CFLAGS_PCHCLR) $(SRC_DIR_CORE)pch_clr.cpp   


############# compile commands for EXE_OBJS (verbexx.exe file) #########################################################

$(OBJ_DIR)main.obj          : $(SRC_DIR_EX)main.cpp                  $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS)            
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)main.cpp


############### compile commands for CORE_DLL_OBJS #####################################################################

$(OBJ_DIR)core_clr.obj      : $(SRC_DIR_CORE)core_clr.cpp            $(MK)  $(PCH_CLR) $(COMM_HDRS) 
  $(COMPILE) /c $(CFLAGS_CLR) $(SRC_DIR_CORE)core_clr.cpp

$(OBJ_DIR)core_dll.obj      : $(SRC_DIR_CORE)core_dll.cpp            $(MK)  $(PCH_STD) $(COMM_HDRS)               
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_CORE)core_dll.cpp  

$(OBJ_DIR)core_error.obj    : $(SRC_DIR_CORE)core_error.cpp          $(MK)  $(PCH_STD) $(COMM_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_CORE)core_error.cpp   

$(OBJ_DIR)core_msdn.obj     : $(SRC_DIR_CORE)core_msdn.cpp           $(MK)  $(PCH_STD) $(COMM_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_CORE)core_msdn.cpp
 
$(OBJ_DIR)core_sys.obj      : $(SRC_DIR_CORE)core_sys.cpp            $(MK)  $(PCH_STD) $(COMM_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_CORE)core_sys.cpp   
                                                  
$(OBJ_DIR)core_util.obj     : $(SRC_DIR_CORE)core_util.cpp           $(MK)  $(PCH_STD) $(COMM_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_CORE)core_util.cpp   


############# compile commands for EX_DLL_OBJS #########################################################################

$(OBJ_DIR)ex_addverb.obj    : $(SRC_DIR_EX)ex_addverb.cpp            $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_addverb.cpp 

$(OBJ_DIR)ex_data.obj       : $(SRC_DIR_EX)ex_data.cpp               $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_data.cpp

$(OBJ_DIR)ex_dll.obj        : $(SRC_DIR_EX)ex_dll.cpp                $(MK)  $(PCH_STD) $(COMM_HDRS)               
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_dll.cpp

$(OBJ_DIR)ex_display.obj    : $(SRC_DIR_EX)ex_display.cpp            $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_display.cpp   

$(OBJ_DIR)ex_environ.obj    : $(SRC_DIR_EX)ex_environ.cpp            $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_environ.cpp             
                                 
$(OBJ_DIR)ex_eval.obj       : $(SRC_DIR_EX)ex_eval.cpp               $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_eval.cpp
                                                       
$(OBJ_DIR)ex_inchar.obj     : $(SRC_DIR_EX)ex_inchar.cpp             $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_inchar.cpp             
                                                                     
$(OBJ_DIR)ex_infile.obj     : $(SRC_DIR_EX)ex_infile.cpp             $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_infile.cpp             
                                                                     
$(OBJ_DIR)ex_interface.obj  : $(SRC_DIR_EX)ex_interface.cpp          $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS)    
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_interface.cpp          
                                                                     
$(OBJ_DIR)ex_intoken.obj    : $(SRC_DIR_EX)ex_intoken.cpp            $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_intoken.cpp            
                                                                     
$(OBJ_DIR)ex_main.obj       : $(SRC_DIR_EX)ex_main.cpp               $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_main.cpp               
                                                                     
$(OBJ_DIR)ex_parse.obj      : $(SRC_DIR_EX)ex_parse.cpp              $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_parse.cpp              
                                                                     
$(OBJ_DIR)ex_preprocess.obj : $(SRC_DIR_EX)ex_preprocess.cpp         $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_preprocess.cpp           
                                                    
$(OBJ_DIR)ex_verb_ctl.obj   : $(SRC_DIR_EX)ex_verb_ctl.cpp           $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_verb_ctl.cpp           
                                                                     
$(OBJ_DIR)ex_verb_op.obj    : $(SRC_DIR_EX)ex_verb_op.cpp            $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_verb_op.cpp            
                                                                     
$(OBJ_DIR)ex_verb_util.obj  : $(SRC_DIR_EX)ex_verb_util.cpp          $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_EX)ex_verb_util.cpp          


########################################################################################################################  