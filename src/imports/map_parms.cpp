// map_parms.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     map_parms.cpp -- options-oriented functions
////     =============
//// 
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "h__include.h"
#pragma hdrstop("../pch/pch_std.pch")

#define M_IN_MAP_DLL

#include "h__common.h"

#include "h_map_types.h"
#include "h_map_global.h"
#include "h_map_parms.h"


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_options() -- print out all options in effect 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_options(const mapparm_S& parm) try
{
    M_out(L"\n---------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Options currently in effect:");
    M_out(L"---------------------------\n");
  
    M_out(L"slice.display                         = %s"          )  %  M_bool_cstr(parm.slice.display)                ;
    M_out(L"slice.max                             = %d"          )  %  parm.slice.max                                 ;
    M_out(L"slice.thread_area                     = %lld"        )  %  parm.slice.thread_area                         ;
    M_out(L"slice.nonthread_area                  = %lld"        )  %  parm.slice.nonthread_area                      ;
    M_out(L"slice.thread_dim                      = %d"          )  %  parm.slice.thread_dim                          ;
    M_out(L"slice.nonthread_dim                   = %d"          )  %  parm.slice.nonthread_dim                       ;
                                                                                                                      
    M_out(L"thread.display                        = %s"          )  %  M_bool_cstr(parm.thread.display)               ; 
    M_out(L"thread.max                            = %d"          )  %  parm.thread.max                                ;
    M_out(L"thread.n_water                        = %d"          )  %  parm.thread.n_water                            ; 
    M_out(L"thread.n_slope                        = %d"          )  %  parm.thread.n_slope                            ;
    M_out(L"thread.n_map_fill                     = %d"          )  %  parm.thread.n_map_fill                         ;
    M_out(L"thread.n_outimage_fill                = %d"          )  %  parm.thread.n_outimage_fill                    ;
                                                                                                                    
    M_out(L"water.check_lake                      = %d"          )  %  parm.water.check_lake                          ; 
    M_out(L"water.check_ocean                     = %s"          )  %  M_bool_cstr(parm.water.check_ocean)            ; 
    M_out(L"water.min_lake                        = %d"          )  %  parm.water.min_lake                            ; 
    M_out(L"water.stride                          = %d"          )  %  parm.water.stride                              ;
    M_out(L"water.ht_min                          = %d"          )  %  parm.water.ht_min                              ; 
    M_out(L"water.ht_max                          = %d"          )  %  parm.water.ht_max                              ; 
    M_out(L"water.adaptive                        = %s"          )  %  M_bool_cstr(parm.water.adaptive)               ; 
    M_out(L"water.a_icecap_lat_n                  = %d"          )  %  parm.water.a_icecap_lat_n                      ; 
    M_out(L"water.a_icecap_lat_s                  = %d"          )  %  parm.water.a_icecap_lat_s                      ; 
    M_out(L"water.a_lo_min_lake                   = %d"          )  %  parm.water.a_lo_min_lake                       ; 
    M_out(L"water.a_hi_min_lake                   = %d"          )  %  parm.water.a_hi_min_lake                       ; 
    M_out(L"water.a_lo_stride                     = %d"          )  %  parm.water.a_lo_stride                         ; 
    M_out(L"water.a_hi_stride                     = %d"          )  %  parm.water.a_hi_stride                         ; 
    M_out(L"water.a_neighbor_xy                   = %d"          )  %  parm.water.a_neighbor_xy                       ; 
    M_out(L"water.a_samples                       = %d"          )  %  parm.water.a_samples                           ; 
    M_out(L"water.a_neighbor_samples              = %d"          )  %  parm.water.a_neighbor_samples                  ; 

    M_out(L"color.water                           = %3d %3d %3d" )  %  (int)(parm.color.water.r                     ) 
                                                                    %  (int)(parm.color.water.g                     ) 
                                                                    %  (int)(parm.color.water.b                     ) ;
    M_out(L"color.ocean                           = %3d %3d %3d" )  %  (int)(parm.color.ocean.r                     ) 
                                                                    %  (int)(parm.color.ocean.g                     ) 
                                                                    %  (int)(parm.color.ocean.b                     ) ;
    M_out(L"color.land                            = %3d %3d %3d" )  %  (int)(parm.color.land.r                      ) 
                                                                    %  (int)(parm.color.land.g                      ) 
                                                                    %  (int)(parm.color.land.b                      ) ;
    M_out(L"color.graticule                       = %3d %3d %3d" )  %  (int)(parm.color.graticule.r                 ) 
                                                                    %  (int)(parm.color.graticule.g                 ) 
                                                                    %  (int)(parm.color.graticule.b                 ) ;         
    M_out(L"color.dem_void                        = %3d %3d %3d" )  %  (int)(parm.color.dem_void.r                  ) 
                                                                    %  (int)(parm.color.dem_void.g                  ) 
                                                                    %  (int)(parm.color.dem_void.b                  ) ;
    M_out(L"color.outside_dem                     = %3d %3d %3d" )  %  (int)(parm.color.outside_dem.r               ) 
                                                                    %  (int)(parm.color.outside_dem.g               ) 
                                                                    %  (int)(parm.color.outside_dem.b               ) ;
    M_out(L"color.outside_raster                  = %3d %3d %3d" )  %  (int)(parm.color.outside_raster.r            ) 
                                                                    %  (int)(parm.color.outside_raster.g            ) 
                                                                    %  (int)(parm.color.outside_raster.b            ) ; 
    M_out(L"color.outside_projection              = %3d %3d %3d" )  %  (int)(parm.color.outside_projection.r        ) 
                                                                    %  (int)(parm.color.outside_projection.g        ) 
                                                                    %  (int)(parm.color.outside_projection.b        ) ;
    M_out(L"color.outside_map                     = %3d %3d %3d" )  %  (int)(parm.color.outside_map.r               ) 
                                                                    %  (int)(parm.color.outside_map.g               ) 
                                                                    %  (int)(parm.color.outside_map.b               ) ;    
    M_out(L"\n------------------------------------------------------------------------------------------------------------------------------------------------------\n");                                                  
    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   default_options() -- set up defaults for all options
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



// file pathnames and directories (defaults)
// ------------------------------

void default_options(mapparm_S& parm) try
{
    parm.slice.display                        = false               ;
    parm.slice.max                            = MAX_SLICES          ;  
    parm.slice.thread_area                    = 1000000LL           ;                 
    parm.slice.nonthread_area                 = 20000LL;            ;
    parm.slice.thread_dim                     = 100LL               ;                 
    parm.slice.nonthread_dim                  = 100LL               ;
                                              
    parm.thread.display                       = false               ;
    parm.thread.max                           = 100;                ; 
    parm.thread.n_water                       = 8                   ;
    parm.thread.n_slope                       = 16                  ;
    parm.thread.n_map_fill                    = 8                   ;
    parm.thread.n_outimage_fill               = 8                   ;
                                              
    parm.water.check_lake                     = WATER_LAKE          ; 
    parm.water.check_ocean                    = true                ;
    parm.water.min_lake                       = 15                  ;   // minimum size for lake detection 
    parm.water.stride                         = 1                   ; 
    parm.water.lk_rem                         = 0                   ;   // obsolete 
    parm.water.lb_rem                         = 0                   ;   // obsolete 
    parm.water.expand                         = -1                  ;   // can't change -- 1, 2, 3, etc are obsolete 
    parm.water.ht_min                         = 3                   ;   // minimum height for lake detection -- 0 or below is also OK   
    parm.water.ht_max                         = 9000                ;   // max height for lake detection   
    parm.water.adaptive                       = true                ;         
    parm.water.a_icecap_lat_n                 = 60                  ;
    parm.water.a_icecap_lat_s                 = -67                 ;
    parm.water.a_lo_min_lake                  = 3                   ;
    parm.water.a_hi_min_lake                  = 15                  ;
    parm.water.a_lo_stride                    = 1                   ;
    parm.water.a_hi_stride                    = 6                   ;
    parm.water.a_neighbor_xy                  = 2                   ;
    parm.water.a_samples                      = 1000                ;
    parm.water.a_neighbor_samples             = 6                   ;
     
    parm.color.water.r                        = 210                 ;   // very light cyan
    parm.color.water.g                        = 255                 ;
    parm.color.water.b                        = 255                 ;
    parm.color.ocean.r                        = 200                 ;   // light cyan
    parm.color.ocean.g                        = 240                 ;
    parm.color.ocean.b                        = 240                 ;
    parm.color.land.r                         = 255                 ;   // light brown
    parm.color.land.g                         = 224                 ;
    parm.color.land.b                         = 192                 ;
    parm.color.graticule.r                    = 0                   ;   // black
    parm.color.graticule.g                    = 0                   ;
    parm.color.graticule.b                    = 0                   ;
    parm.color.dem_void.r                     = 127                 ;   // 50% gray
    parm.color.dem_void.g                     = 127                 ; 
    parm.color.dem_void.b                     = 127                 ;
    parm.color.outside_dem.r                  = 240                 ;   // light gray, luminance close to water/ocean colors (above)
    parm.color.outside_dem.g                  = 240                 ;
    parm.color.outside_dem.b                  = 240                 ;
    parm.color.outside_raster.r               = 63                  ;   // 25% gray
    parm.color.outside_raster.g               = 63                  ;
    parm.color.outside_raster.b               = 63                  ;
    parm.color.outside_projection.r           = 0                   ;   // black
    parm.color.outside_projection.g           = 0                   ;
    parm.color.outside_projection.b           = 0                   ;
    parm.color.outside_map.r                  = 255                 ;   // white
    parm.color.outside_map.g                  = 255                 ; 
    parm.color.outside_map.b                  = 255                 ;
                                             
    return; 
}
M_endf


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""