###############################################################################################################################
#
# 
#    ================ 
#    verbexx_other.mk - makefile for Windows command-line nmake
#    ================
#
#
#
#  makes for:   audio.dll
#
#    note: assumes output from verbexx_base.mk is already available  
# 
#
###############################################################################################################################
 
EXNAME = verbexx
  
!INCLUDE $(EXNAME)_folder.mk
!INCLUDE $(EXNAME)_implib.mk
!INCLUDE $(EXNAME)_base_header.mk
!INCLUDE $(EXNAME)_imports_header.mk
!INCLUDE $(EXNAME)_other_header.mk

MK = \
$(MK_DIR)$(EXNAME)_other.mk \
$(MK_DIR)$(EXNAME)_folder.mk \
$(MK_DIR)$(EXNAME)_implib.mk \
$(MK_DIR)$(EXNAME)_base_header.mk \
$(MK_DIR)$(EXNAME)_imports_header.mk \
$(MK_DIR)$(EXNAME)_other_header.mk 

############# compile/link command options #################################################################################### 

COMPILE    = cl
LINK       = link
SEPARATOR  = @echo ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CFLAGS = /Yu /EHa /MD /nologo /O2 /Zm256 /I$(INC_DIR_CORE) /I$(INC_DIR_EX) /I$(INC_DIR_IMPORTS) /I$(INC_DIR_OTHER) /Fo$(OBJ_DIR) /Fp$(PCH_DIR)pch_std.pch
    
LFLAGS_DLL = /STACK:200000000 /NOLOGO /LIBPATH:$(LIB_DIR) /DLL 


###############################################################################################################################

PCH_OBJS = \
$(OBJ_DIR)pch_std.obj \
$(OBJ_DIR)pch_clr.obj 


AUDIO_DLL_OBJS = \
$(OBJ_DIR)audio_dll.obj \
$(OBJ_DIR)audio.obj \
$(OBJ_DIR)audio_main.obj


###############################################################################################################################


AUDIO_LIBS = \
$(LIB_DIR)$(EXNAME)_core.lib \
$(LIB_DIR)$(EXNAME)_ex.lib \
$(LIB_DIR)$(EXNAME)_draw.lib



###############################################################################################################################
#                                phony make target 
###############################################################################################################################

ALL = \
$(DLL_DIR)$(EXNAME)_audio.dll 


# show all DLLs in directory

all : $(ALL)
  dir $(DLL_DIR)  
   
 
###############################################################################################################################
#                                    .dll link commands 
###############################################################################################################################

$(LIB_DIR)audio.lib $(DLL_DIR)$(EXNAME)_audio.dll    : $(MK) $(PCH_OBJS) $(AUDIO_DLL_OBJS)               $(AUDIO_LIBS)
  $(SEPARATOR)
  $(LINK) $(LFLAGS_DLL)                                      $(PCH_OBJS) $(AUDIO_DLL_OBJS)   $(IMP_LIBS) $(AUDIO_LIBS)              /OUT:$(DLL_DIR)$(EXNAME)_audio.dll    /IMPLIB:$(LIB_DIR)$(EXNAME)_audio.lib
  $(SEPARATOR)                                                                                                                                        
# dumpbin /EXPORTS $(DLL_DIR)$(EXNAME)_audio.dll   


###############################################################################################################################
#                                    compile commands 
###############################################################################################################################


############# compile commands for AUDIO_DLL_OBJS ############################################################################## 

$(OBJ_DIR)audio.obj         : $(SRC_DIR_OTHER)audio.cpp          $(MK)  $(PCH_STD) $(COMM_HDRS)               $(DRAW_HDRS) $(AUDIO_HDRS)                     
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_OTHER)audio.cpp    
 
$(OBJ_DIR)audio_dll.obj     : $(SRC_DIR_OTHER)audio_dll.cpp      $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) $(DRAW_HDRS) $(AUDIO_HDRS)     
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_OTHER)audio_dll.cpp  
                                                   
$(OBJ_DIR)audio_main.obj    : $(SRC_DIR_OTHER)audio_main.cpp     $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) $(DRAW_HDRS) $(AUDIO_HDRS)     
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_OTHER)audio_main.cpp    


################################################################################################################################  