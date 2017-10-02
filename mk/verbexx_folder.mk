##################################################################################################################################
#
#
#   =================    -----------------------------------
#   verbexx_folder.mk -- included makefile with folder names  (use with Windows command-line nmake)
#   =================    -----------------------------------
#
#
##################################################################################################################################
 
EXDIR  = E:\Data01\program 

################### folders ################################

SRC_DIR_CORE     = $(EXDIR)\$(EXNAME)\src\base\#
SRC_DIR_EX       = $(EXDIR)\$(EXNAME)\src\base\#
SRC_DIR_IMPORTS  = $(EXDIR)\$(EXNAME)\src\imports\#
SRC_DIR_OTHER    = $(EXDIR)\$(EXNAME)\src\other\#

INC_DIR_CORE     = $(EXDIR)\$(EXNAME)\include\base\#
INC_DIR_EX       = $(EXDIR)\$(EXNAME)\include\base\#
INC_DIR_IMPORTS  = $(EXDIR)\$(EXNAME)\include\imports\#
INC_DIR_OTHER    = $(EXDIR)\$(EXNAME)\include\other\#

OBJ_DIR          = $(EXDIR)\$(EXNAME)\object\#
PCH_DIR          = $(EXDIR)\$(EXNAME)\pch\#
LIB_DIR          = $(EXDIR)\$(EXNAME)\lib\#
MK_DIR           = $(EXDIR)\$(EXNAME)\mk\#   

EXE_DIR          = $(EXDIR)\exe\#
DLL_DIR          = $(EXDIR)\dll\#


################################################################################################################################  