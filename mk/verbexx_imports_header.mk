####################################################################################################################################
#
#
#   =========================    -----------------------------------------------------------
#   verbexx_imports_header.mk -- included makefile with header file groups for imported dlls (use with Windows command-line nmake)
#   =========================    -----------------------------------------------------------
#
#
####################################################################################################################################
                         

################### headers ########################################################################################################

DRAW_HDRS = \
$(INC_DIR_IMPORTS)h_draw_canvas.h \
$(INC_DIR_IMPORTS)h_draw_verb.h \
$(INC_DIR_IMPORTS)h_draw_types.h \
$(INC_DIR_IMPORTS)h_draw_outimage.h \
$(INC_DIR_IMPORTS)h_draw_tiff.h \
$(INC_DIR_IMPORTS)h_draw_window.h
 
MAP_HDRS = \
$(INC_DIR_IMPORTS)h_map_annotate.h \
$(INC_DIR_IMPORTS)h_map_dem.h \
$(INC_DIR_IMPORTS)h_map_grid.h \
$(INC_DIR_IMPORTS)h_map_data.h \
$(INC_DIR_IMPORTS)h_map_global.h \
$(INC_DIR_IMPORTS)h_map_image.h \
$(INC_DIR_IMPORTS)h_map_parms.h \
$(INC_DIR_IMPORTS)h_map_verb.h \
$(INC_DIR_IMPORTS)h_map_ramp.h \
$(INC_DIR_IMPORTS)h_map_raster.h \
$(INC_DIR_IMPORTS)h_map_shapefile.h \
$(INC_DIR_IMPORTS)h_map_textfile.h \
$(INC_DIR_IMPORTS)h_map_thd.h \
$(INC_DIR_IMPORTS)h_map_tile.h \
$(INC_DIR_IMPORTS)h_map_types.h

####################################################################################################################################  