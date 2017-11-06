###############################################################################################################################
#
# 
#    ================== 
#    verbexx_imports.mk - makefile for Windows command-line nmake
#    ==================
#
#
#
#  makes for:   map.dll
#               draw.dll
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

MK = \
$(MK_DIR)$(EXNAME)_imports.mk \
$(MK_DIR)$(EXNAME)_folder.mk \
$(MK_DIR)$(EXNAME)_implib.mk \
$(MK_DIR)$(EXNAME)_base_header.mk \
$(MK_DIR)$(EXNAME)_imports_header.mk 


############# compile/link command options #################################################################################### 

COMPILE    = cl
LINK       = link
SEPARATOR  = @echo ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CFLAGS = $(CFLAGS_ALWAYS) /Yu /nologo /I$(INC_DIR_CORE) /I$(INC_DIR_EX) /I$(INC_DIR_IMPORTS) /Fo$(OBJ_DIR) /Fp$(PCH_DIR)pch_std.pch
    
LFLAGS_DLL = /STACK:$(STACK_SIZE) /NOLOGO /LIBPATH:$(LIB_DIR) /DLL 


###############################################################################################################################

PCH_OBJS = \
$(OBJ_DIR)pch_std.obj \
$(OBJ_DIR)pch_clr.obj 


DRAW_DLL_OBJS = \
$(OBJ_DIR)draw_dll.obj \
$(OBJ_DIR)draw_addverb.obj \
$(OBJ_DIR)draw_canvas.obj \
$(OBJ_DIR)draw_main.obj \
$(OBJ_DIR)draw_outimage.obj \
$(OBJ_DIR)draw_verb.obj \
$(OBJ_DIR)draw_window.obj
     
          
MAP_DLL_OBJS = \
$(OBJ_DIR)map_dll.obj \
$(OBJ_DIR)map_addverb.obj \
$(OBJ_DIR)map_annotate.obj \
$(OBJ_DIR)map_data.obj \
$(OBJ_DIR)map_dem.obj \
$(OBJ_DIR)map_grid.obj \
$(OBJ_DIR)map_image.obj \
$(OBJ_DIR)map_main.obj \
$(OBJ_DIR)map_parms.obj \
$(OBJ_DIR)map_ramp.obj \
$(OBJ_DIR)map_raster.obj \
$(OBJ_DIR)map_shapefile.obj \
$(OBJ_DIR)map_textfile.obj \
$(OBJ_DIR)map_thd.obj \
$(OBJ_DIR)map_tile.obj \
$(OBJ_DIR)map_verb.obj \
$(OBJ_DIR)map_water.obj   


###############################################################################################################################
    

DRAW_LIBS = \
$(LIB_DIR)$(EXNAME)_core.lib \
$(LIB_DIR)$(EXNAME)_ex.lib

MAP_LIBS = \
$(LIB_DIR)$(EXNAME)_core.lib \
$(LIB_DIR)$(EXNAME)_ex.lib \
$(LIB_DIR)$(EXNAME)_draw.lib



###############################################################################################################################
#                                phony make target 
###############################################################################################################################

ALL = \
$(DLL_DIR)$(EXNAME)_draw.dll \
$(DLL_DIR)$(EXNAME)_map.dll  


# show all DLLs in directory

all : $(ALL)
  dir $(DLL_DIR)  
   
 
###############################################################################################################################
#                                    .dll link commands 
###############################################################################################################################
                                                 
$(LIB_DIR)draw.lib $(DLL_DIR)$(EXNAME)_draw.dll      : $(MK) $(PCH_OBJS) $(DRAW_DLL_OBJS)                $(DRAW_LIBS)
  $(SEPARATOR)
  $(LINK) $(LFLAGS_DLL)                                      $(PCH_OBJS) $(DRAW_DLL_OBJS)    $(IMP_LIBS) $(DRAW_LIBS)                /OUT:$(DLL_DIR)$(EXNAME)_draw.dll   /IMPLIB:$(LIB_DIR)$(EXNAME)_draw.lib
  $(SEPARATOR) 
# dumpbin /EXPORTS $(DLL_DIR)$(EXNAME)_draw.dll

$(LIB_DIR)map.lib $(DLL_DIR)$(EXNAME)_map.dll        : $(MK) $(PCH_OBJS) $(MAP_DLL_OBJS)                 $(MAP_LIBS)
  $(SEPARATOR)
  $(LINK) $(LFLAGS_DLL)                                      $(PCH_OBJS) $(MAP_DLL_OBJS)     $(IMP_LIBS) $(MAP_LIBS)                 /OUT:$(DLL_DIR)$(EXNAME)_map.dll     /IMPLIB:$(LIB_DIR)$(EXNAME)_map.lib
  $(SEPARATOR) 
# dumpbin /EXPORTS $(DLL_DIR)$(EXNAME)_map.dll                                       


###############################################################################################################################
#                                    compile commands 
###############################################################################################################################


############# compile commands for DRAW_DLL_OBJS ##############################################################################

$(OBJ_DIR)draw_addverb.obj  : $(SRC_DIR_IMPORTS)draw_addverb.cpp   $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) $(DRAW_HDRS)                   
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)draw_addverb.cpp                                                

$(OBJ_DIR)draw_canvas.obj   : $(SRC_DIR_IMPORTS)draw_canvas.cpp    $(MK)  $(PCH_STD) $(COMM_HDRS)               $(DRAW_HDRS)  
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)draw_canvas.cpp

$(OBJ_DIR)draw_dll.obj      : $(SRC_DIR_IMPORTS)draw_dll.cpp       $(MK)  $(PCH_STD) $(COMM_HDRS)                             
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)draw_dll.cpp  

$(OBJ_DIR)draw_main.obj     : $(SRC_DIR_IMPORTS)draw_main.cpp      $(MK)  $(PCH_STD) $(COMM_HDRS)               $(DRAW_HDRS)  
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)draw_main.cpp   

$(OBJ_DIR)draw_outimage.obj : $(SRC_DIR_IMPORTS)draw_outimage.cpp  $(MK)  $(PCH_STD) $(COMM_HDRS)               $(DRAW_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)draw_outimage.cpp

$(OBJ_DIR)draw_verb.obj     : $(SRC_DIR_IMPORTS)draw_verb.cpp      $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) $(DRAW_HDRS)                   
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)draw_verb.cpp  

$(OBJ_DIR)draw_window.obj   : $(SRC_DIR_IMPORTS)draw_window.cpp    $(MK)  $(PCH_STD) $(COMM_HDRS)               $(DRAW_HDRS)  
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)draw_window.cpp


############# compile commands for MAP_DLL_OBJS ############################################################################### 

$(OBJ_DIR)map_addverb.obj   : $(SRC_DIR_IMPORTS)map_addverb.cpp    $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) $(MAP_HDRS)                    
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_addverb.cpp

$(OBJ_DIR)map_annotate.obj  : $(SRC_DIR_IMPORTS)map_annotate.cpp   $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS) $(DRAW_HDRS)                       
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_annotate.cpp

$(OBJ_DIR)map_data.obj      : $(SRC_DIR_IMPORTS)map_data.cpp       $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS) $(DRAW_HDRS)                       
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_data.cpp

$(OBJ_DIR)map_dem.obj       : $(SRC_DIR_IMPORTS)map_dem.cpp        $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_dem.cpp

$(OBJ_DIR)map_dll.obj       : $(SRC_DIR_IMPORTS)map_dll.cpp        $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) $(MAP_HDRS) $(DRAW_HDRS)       
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_dll.cpp

$(OBJ_DIR)map_grid.obj      : $(SRC_DIR_IMPORTS)map_grid.cpp       $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_grid.cpp

$(OBJ_DIR)map_image.obj     : $(SRC_DIR_IMPORTS)map_image.cpp      $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS) $(DRAW_HDRS)                        
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_image.cpp

$(OBJ_DIR)map_main.obj      : $(SRC_DIR_IMPORTS)map_main.cpp       $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) $(MAP_HDRS)                    
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_main.cpp

$(OBJ_DIR)map_parms.obj     : $(SRC_DIR_IMPORTS)map_parms.cpp      $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS)  
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_parms.cpp

$(OBJ_DIR)map_ramp.obj      : $(SRC_DIR_IMPORTS)map_ramp.cpp       $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS) $(DRAW_HDRS)                        
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_ramp.cpp

$(OBJ_DIR)map_raster.obj    : $(SRC_DIR_IMPORTS)map_raster.cpp     $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_raster.cpp
                                       
$(OBJ_DIR)map_shapefile.obj : $(SRC_DIR_IMPORTS)map_shapefile.cpp  $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS) $(DRAW_HDRS)                        
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_shapefile.cpp

$(OBJ_DIR)map_textfile.obj  : $(SRC_DIR_IMPORTS)map_textfile.cpp   $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS)                         
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_textfile.cpp
                                                 
$(OBJ_DIR)map_thd.obj       : $(SRC_DIR_IMPORTS)map_thd.cpp        $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_thd.cpp

$(OBJ_DIR)map_tile.obj      : $(SRC_DIR_IMPORTS)map_tile.cpp       $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS)
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_tile.cpp

$(OBJ_DIR)map_verb.obj      : $(SRC_DIR_IMPORTS)map_verb.cpp       $(MK)  $(PCH_STD) $(COMM_HDRS) $(EX_IF_HDRS) $(MAP_HDRS) $(DRAW_HDRS)       
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_verb.cpp

$(OBJ_DIR)map_water.obj     : $(SRC_DIR_IMPORTS)map_water.cpp      $(MK)  $(PCH_STD) $(COMM_HDRS)               $(MAP_HDRS) 
  $(COMPILE) /c $(CFLAGS)     $(SRC_DIR_IMPORTS)map_water.cpp


################################################################################################################################  