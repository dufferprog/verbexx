// map_addverb.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===============
////     map_addverb.cpp -- define map-oriented @VERBS, and do initial processing for them
////     ===============
//// 
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
                
#include "h_ex_interface.h"
#include "h_ex_verbmac.h"
 
#include "h_map_types.h"
#include "h_map_global.h"
#include "h_map_parms.h" 
#include "h_map_verb.h"


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   verb_xxxx() -- static functions to process map-oriented verbs -- pointed to by verb table
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



// local MACRO with mapverb_xxx() function call for verb_xxx() functions  

#define M_mapverb(mv)                                                       \
{                                                                           \
    if (error_count() == 0)                                                 \
    {                                                                       \
        int  rc {0};                                                        \
        auto vrc = mv;                                                      \
        count_error(vrc);                                                   \
        if (vrc != 0) rc = -1;                                              \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        M_out_emsg(L"Bypassing %s call because of prior errors") % #mv;     \
    }                                                                       \
}




/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @MAP_OPTION -- process map options
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_map_option(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_map_option() called");)
    M_mapverb( mapverb_map_option(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DISPLAY_MAP_OPTIONS -- display all mapping options
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_display_map_options(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_display_map_options() called");)
    M_mapverb( mapverb_display_map_options(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @RAMP -- build color ramp based on parms
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_ramp(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_ramp() called");)
    M_mapverb( mapverb_ramp(expression, *global_N::parm_p) )
    return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DRAW_RAMP -- draw ramp in outimage_S buffer
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_draw_ramp(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_draw_ramp() called");)
    M_mapverb( mapverb_draw_ramp(expression, *global_N::parm_p) )
    return 0; 
}
M_endf
  

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @PUT_RAMP -- draw ramp directly in outimage_S buffer (bypass GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_put_ramp(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_put_ramp() called");)
    M_mapverb( mapverb_put_ramp(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DESTROY_RAMP n_ramp -- deallocate ramp_S in ramp repository
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_destroy_ramp(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_destroy_ramp() called");)
    M_mapverb( mapverb_destroy_ramp(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @CREATE_FILEINFO n_fileinfo -- allocate file_info_S in fileinfo repository
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_create_fileinfo(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_create_fileinfo() called");)
    M_mapverb( mapverb_create_fileinfo(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DISPLAY_FILEINFO n_fileinfo -- display contents of this file_info_S in repository
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_display_fileinfo(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_display_fileinfo() called");)
    M_mapverb( mapverb_display_fileinfo(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DESTROY_FILEINFO n_fileinfo -- deallocate fileinfo_S in fileinfo repository
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_destroy_fileinfo(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_destroy_fileinfo() called");)
    M_mapverb( mapverb_destroy_fileinfo(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @CREATE_FIXED_RASTER n_raster -- create raster structure (specified size)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_create_fixed_raster(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_create_fixed_raster() called");)  
    M_mapverb( mapverb_create_fixed_raster(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @CREATE_AUTO_RASTER n_map n_raster -- create auto raster (based on map coverage area)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_create_auto_raster(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_create_auto_raster() called");)
    M_mapverb( mapverb_create_auto_raster(expression) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @FILL_WHOLE_RASTER n_raster -- fill entire raster
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_fill_whole_raster(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_fill_whole_raster() called");)
    M_mapverb( mapverb_fill_whole_raster(expression, *global_N::parm_p) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @SELECTIVE_FILL_RASTER n_map n_raster -- fill raster sections needed by map coverage
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_selective_fill_raster(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_selective_fill_raster() called");)
    M_mapverb( mapverb_selective_fill_raster(expression, *global_N::parm_p) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @SMOOTH_RASTER n_raster -- smooth data in raster_S
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_smooth_raster(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_smooth_raster() called");)
    M_mapverb( mapverb_smooth_raster(expression) )
    return 0; 
}
M_endf
 

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DISPLAY_RASTER n_raster -- display raster data
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_display_raster(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_display_raster() called");)
    M_mapverb( mapverb_display_raster(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @WRITEOUT_RASTER n_raster -- write entire raster to filegroup
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_writeout_raster(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_writeout_raster() called");)
    M_mapverb( mapverb_writeout_raster(expression) )
    return 0; 
}
M_endf
   

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DESTROY_RASTER n_raster -- free up raster structure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_destroy_raster(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_destroy_raster() called");)
    M_mapverb( mapverb_destroy_raster(expression) )
    return 0; 
}
M_endf
  

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @CREATE_MAP n_map -- create map structure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_create_map(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_create_map() called");)
    M_mapverb( mapverb_create_map(expression, *global_N::parm_p) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @ADD_DEM n_map n_raster -- fill map structure from raster data
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_add_dem(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_add_dem() called");)
    M_mapverb( mapverb_fill_map(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DETECT_WATER n_map -- do water detection for filled-in map
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_detect_water(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_detect_water() called");)
    M_mapverb( mapverb_water_map(expression, *global_N::parm_p) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @ADD_GRATICULE n_map -- add graticule to map
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_add_graticule(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_add_graticule() called");)
    M_mapverb( mapverb_graticule_map(expression, *global_N::parm_p) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @ADD_SLOPES n_map -- do shaded_relief processing for filled-in map
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_add_slopes(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_add_slopes() called");)
    M_mapverb( mapverb_slope_map(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @EXTRACT_MAP -- extract info from map structure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_extract_map(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_extract_map() called");)
    M_mapverb( mapverb_extract_map(expression, value, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DESTROY_MAP n_map -- free up map structure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_destroy_map(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_destroy_map() called");)
    M_mapverb( mapverb_destroy_map(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @CREATE_MAP_OUTIMAGE n_map n_outimage -- create outimage (based on map dimensions)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_create_map_outimage(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_create_map_outimage() called");)
    M_mapverb( mapverb_create_map_outimage(expression, *global_N::parm_p) )
    return 0; 
}
M_endf
         



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @SET_MAP_WINDOW -- set up map window for @PUT_MAP and @ANNOTATE_MAP
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_set_map_window(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_set_map_window() called");)
    M_mapverb( mapverb_set_map_window(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @PUT_MAP -- draw map directly in outimage_S buffer (no GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_put_map(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_put_map() called");)
    M_mapverb( mapverb_put_map(expression, *global_N::parm_p) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @ANNOTATE_MAP -- add map-oriented text, etc. to graphics buffers (uses GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_annotate_map(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_annotate_map() called");)
    M_mapverb( mapverb_annotate_map(expression, *global_N::parm_p) )
    return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_map_verbs() -- add map-oriented verbs to verb table
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void add_map_verbs(const std::wstring& prefix) try
{
    
    // =====================================
    // @MAP_OPTION -- map options
    // =====================================
   
    {
        M_vt_nofix(vt)
 

        // color: keyword -- special map colors
        // ------------------------------------

        {
            M_vt_nested_plist(                   npl, 0, 0)
        
            {   // color: [ water: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                                
                M_vt_vlist_optional_nest_kw(     npl, L"water"                    , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }
          
            {   // color: [ ocean: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                                
                M_vt_vlist_optional_nest_kw(     npl, L"ocean"                    , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }

            {   // color: [ land: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                                
                M_vt_vlist_optional_nest_kw(     npl, L"land"                     , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }

            {   // color: [ graticule: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                     
                M_vt_vlist_optional_nest_kw(     npl, L"graticule"                , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }
        
            {   // color: [ dem_void: [r g b]]
                M_vt_nested_plist(                                                 n2pl,     3,      3     )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                             n2pl,     0,    255     )
                M_vt_int64rc_nest_pos(                                             n2pl,     0,    255     )
                M_vt_int64rc_nest_pos(                                             n2pl,     0,    255     )
                                                                     
                M_vt_vlist_optional_nest_kw(     npl, L"dem_void"                 , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }
        
            {   // color: [ outside_dem: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                     
                M_vt_vlist_optional_nest_kw(     npl, L"outside_dem"              , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }

            {   // color: [ outside_raster: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                     
                M_vt_vlist_optional_nest_kw(     npl, L"outside_raster"           , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }

            {   // color: [ outside_projection: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                     
                M_vt_vlist_optional_nest_kw(     npl, L"outside_projection"       , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }

            {   // color: [ outside_map: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                     
                M_vt_vlist_optional_nest_kw(     npl, L"outside_map"              , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }

            {   // color: [ gdi_cleared: [r g b]]
                M_vt_nested_plist(                                                  n2pl,     3,      3    )   // exactly 3 positional parms required
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                M_vt_int64rc_nest_pos(                                              n2pl,     0,    255    )
                                                                     
                M_vt_vlist_optional_nest_kw(     npl, L"gdi_cleared"              , n2pl                   )   // add in 2nd nested plist to 1st nested plist
            }

            M_vt_vlist_optional_right_kw(vt, L"color",  npl)                                   // add kw with nested 1st plist to right parms
        }
   

        // slice: keyword -- thread/no-thread slice control options
        // --------------------------------------------------------

        {
            M_vt_nested_plist(                   npl, 0, 0)
       
            M_vt_nval_optional_nest_kw(          npl, L"display"                                         )
            M_vt_nval_optional_nest_kw(          npl, L"no_display"                                      )

            M_vt_int64rc_optional_nest_kw(       npl, L"max"                     ,   1      , MAX_SLICES )
            M_vt_int64rc_optional_nest_kw(       npl, L"min_area_thread"         ,   2500   , 8000000000 )
            M_vt_int64rc_optional_nest_kw(       npl, L"min_area_nothread"       ,   2500   , 8000000000 ) 
            M_vt_int64rc_optional_nest_kw(       npl, L"min_dimension_thread"    ,   10     , 200000     )
            M_vt_int64rc_optional_nest_kw(       npl, L"min_dimension_nothread"  ,   10     , 200000     )

            M_vt_nest_conflict_pair(             npl, L"display" , L"no_display"  )

            M_vt_vlist_optional_right_kw(vt, L"slice",  npl)                                   // add kw with nested 1st plist to right parms
        }
 


        // thread: keyword -- thread/no-thread slice control options
        // ---------------------------------------------------------

        {
            M_vt_nested_plist(                   npl, 0, 0)
       
            M_vt_nval_optional_nest_kw(          npl, L"display"                                         )
            M_vt_nval_optional_nest_kw(          npl, L"no_display"                                      )

            M_vt_int64rc_optional_nest_kw(       npl, L"max"                     ,   1      , THREAD_MAX )
            M_vt_int64rc_optional_nest_kw(       npl, L"water_detect"            ,   1      , THREAD_MAX )
            M_vt_int64rc_optional_nest_kw(       npl, L"compute_slopes"          ,   1      , THREAD_MAX )
            M_vt_int64rc_optional_nest_kw(       npl, L"map_fill"                ,   1      , THREAD_MAX )
            M_vt_int64rc_optional_nest_kw(       npl, L"outimage_fill"           ,   1      , THREAD_MAX )

            M_vt_nest_conflict_pair(             npl, L"display" , L"no_display"  )

            M_vt_vlist_optional_right_kw(vt, L"thread",  npl)                                   // add kw with nested 1st plist to right parms
        }
    

        // water_detect: keyword -- water detection options
        // ------------------------------------------------

        {
            M_vt_nested_plist(                                npl, 0, 0)
                                                           
            M_vt_nval_optional_nest_kw(                       npl, L"lake"                                          )
            M_vt_nval_optional_nest_kw(                       npl, L"no_lake"                                       )
            M_vt_nval_optional_nest_kw(                       npl, L"ocean"                                         )
            M_vt_nval_optional_nest_kw(                       npl, L"no_ocean"                                      )
                                                           
            M_vt_int64rc_optional_nest_kw(                    npl, L"min_size"        ,   3      ,   25             )
            M_vt_int64rc_optional_nest_kw(                    npl, L"stride"          ,   1      ,   10             )
            M_vt_int64rc_optional_nest_kw(                    npl, L"min_lake_ht"     ,   0      ,   9000           )
            M_vt_int64rc_optional_nest_kw(                    npl, L"max_lake_ht"     ,   0      ,   9000           )
            M_vt_nval_optional_nest_kw(                       npl, L"no_adaptive"                                   )

            {
                // water_detect: [adaptive: xx xx xx   ] nested keyword                

                 M_vt_nested_plist(                                  n2pl, 0, 0)
                                                     
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"icecap_n_latitude"   ,   0    ,    90  )
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"icecap_s_latitude"   ,   0    ,   -90  )
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"lo_min_size"         ,   3    ,    25  )
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"hi_min_size"         ,   3    ,    25  )
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"lo_stride"           ,   1    ,    10  )
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"hi_stride"           ,   1    ,    10  )
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"neighbor_row_col"    ,   1    ,    10  )
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"subslice_samples"    , 100    ,  10000 )
                 M_vt_int64rc_optional_nest_kw(                      n2pl, L"neighbor_samples"    ,   0    ,    100 )

                 M_vt_vlist_optional_nest_kw(    npl, L"adaptive",   n2pl                                           )         // add in 2nd nested plist to 1st nested plist
            }
        
            M_vt_nest_conflict_pair(                           npl, L"lake"       , L"no_lake"                      )
            M_vt_nest_conflict_pair(                           npl, L"ocean"      , L"no_ocean"                     )
            M_vt_nest_conflict_pair(                           npl, L"adaptive"   , L"no_adaptive"                  )
                                                                                                                   
            M_vt_vlist_optional_right_kw(vt, L"water_detect",  npl                                                  )                            // add kw with nested 1st plist to right parms
        }

        M_vt_add_simplified(prefix + L"MAP_OPTION", vt, verb_map_option                                            ); 
    }



    // =====================================
    // @RAMP
    // =====================================

    {
        M_vt_nofix(                         vt                                                  )            // no positional parms
        M_vt_int64rc_required_right_kw(     vt, L"ramp_n"   ,    1, 100                         )            // required -- ramp number = 1 to 100   
        M_vt_int64rc_optional_right_kw(     vt, L"display"                 , 0        , 2       )

        M_vt_nval_optional_right_kw(        vt, L"clear"                                        )
        M_vt_nval_optional_right_kw(        vt, L"no_clear"                                     )
        M_vt_nval_optional_right_kw(        vt, L"finish"                                       )
        M_vt_nval_optional_right_kw(        vt, L"no_finish"                                    )
        M_vt_int64rc_optional_right_kw(     vt, L"land_rescale"            , 0        , 1       )
        M_vt_int64rc_optional_right_kw(     vt, L"ocean_rescale"           , 0        , 1       )

        M_vt_int64rc_optional_right_kw(     vt, L"land_rescale_height"     ,   0      ,   20000 )
        M_vt_int64rc_optional_right_kw(     vt, L"ocean_rescale_depth"     ,   -24000 ,       0 )
        M_vt_int64rc_optional_right_kw(     vt, L"lowest_land_rescale"     ,   -1000  ,   10000 )
        M_vt_int64rc_optional_right_kw(     vt, L"shallowest_ocean_rescale",   -12000 ,    0    )
        M_vt_int64rc_optional_right_kw(     vt, L"land_rescale_base"       ,   -1000  ,   10000 )
        M_vt_float64rc_optional_right_kw(   vt, L"land_rescale_gamma"      ,   0.01   ,    20.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"ocean_rescale_gamma"     ,   0.01   ,    20.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"land_rescale_step"       ,   0.0    ,   500.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"ocean_rescale_step"      ,   0.0    ,   500.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"land_pastel_factor"      ,   0.0    ,     1.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"ocean_pastel_factor"     ,   0.0    ,     1.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"land_shade_darkest"      ,   0.0    ,     1.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"land_shade_lightest"     ,   0.0    ,     1.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"land_shade_midtone"      ,   0.0    ,     1.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"land_dark_angle"         ,   0.0    ,     90.0)    
        M_vt_float64rc_optional_right_kw(   vt, L"land_bright_angle"       ,   0.0    ,     90.0) 
        M_vt_float64rc_optional_right_kw(   vt, L"ocean_shade_darkest"     ,   0.0    ,     1.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"ocean_shade_lightest"    ,   0.0    ,     1.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"ocean_shade_midtone"     ,   0.0    ,     1.0 )
        M_vt_float64rc_optional_right_kw(   vt, L"ocean_dark_angle"        ,   0.0    ,     90.0)    
        M_vt_float64rc_optional_right_kw(   vt, L"ocean_bright_angle"      ,   0.0    ,     90.0)  
                                                 
        {   // land_color: [r g b]
            M_vt_nested_plist(                                         npl,     3,      3    )            // exactly 3 positional parms required for land_color: keyword vlist
            M_vt_int64rc_nest_pos(                                     npl,     0,    255    )
            M_vt_int64rc_nest_pos(                                     npl,     0,    255    )
            M_vt_int64rc_nest_pos(                                     npl,     0,    255    )
                                                            
            M_vt_vlist_multi_right_kw(      vt, L"land_color",         npl,     0, -1        )            // add in land_color: kw with nested plist to main verbtype -- 5-N occurrences allowed
        }

        {   // ocean_color: [r g b]
            M_vt_nested_plist(                                         npl,     3,      3    )            // exactly 3 positional parms required for ocean_color: keyword vlist
            M_vt_int64rc_nest_pos(                                     npl,     0,    255    )
            M_vt_int64rc_nest_pos(                                     npl,     0,    255    )
            M_vt_int64rc_nest_pos(                                     npl,     0,    255    )
                                                            
            M_vt_vlist_multi_right_kw(      vt, L"ocean_color",        npl,     0, -1        )            // add in ocean_color: kw with nested plist to main verbtype -- 5-N occurrences allowed
        }

        M_vt_int64_multi_right_kw(          vt, L"land_ht"   ,                           0, -1        )   // add in land_ht:  kw -- 0-N occurrences allowed 
        M_vt_int64_multi_right_kw(          vt, L"ocean_dp"  ,                           0, -1        )   // add in ocean_dp: kw -- 0-N occurrences allowed 

        M_vt_right_match_pair(              vt, L"land_color"    , L"land_ht"         )                   // land_color:  and land_ht:  must appear same number of times  
        M_vt_right_match_pair(              vt, L"ocean_color"   , L"ocean_dp"        )                   // ocean_color: and ocean_dp: must appear same number of times  

        M_vt_right_conflict_pair(           vt, L"clear"         , L"no_clear"                            )
        M_vt_right_conflict_pair(           vt, L"finish"        , L"no_finish"                           )
        M_vt_right_conflict_3way(           vt, L"land_rescale"  , L"no_land_rescale" , L"v_land_rescale" )
        M_vt_right_conflict_3way(           vt, L"ocean_rescale" , L"no_ocean_rescale", L"v_ocean_rescale")

        M_vt_add_simplified(prefix + L"RAMP", vt, verb_ramp                                               );
    }



    // =====================================
    // @DRAW_RAMP
    // =====================================

    {
        M_vt_nofix(                         vt                                                  )            // no positional parms
        M_vt_int64rc_required_right_kw(     vt, L"ramp_n"     ,    1, 100                       )            // required -- ramp number = 1 to 100
        M_vt_int64rc_required_right_kw(     vt, L"outimage_n" ,    1, 100                       )            // required -- outimage number = 1 to 100
        M_vt_int64rc_optional_right_kw(     vt, L"display"    ,    0, 1                         )            // debug display (1 = yes, 0 = no)
                                                                                               
        M_vt_nval_optional_right_kw(        vt, L"ocean"                                        )
        M_vt_nval_optional_right_kw(        vt, L"land"                                         )
   
        M_vt_int64rc_required_right_kw(     vt, L"top_row"                 ,   0      ,   50000 )
        M_vt_int64rc_required_right_kw(     vt, L"left_col"                ,   0      ,   50000 )
        M_vt_int64rc_required_right_kw(     vt, L"width"                   ,   75     ,    5000 )
        M_vt_int64rc_required_right_kw(     vt, L"height"                  ,   500    ,   25000 )
        M_vt_int64rc_optional_right_kw(     vt, L"border"                  ,   0      ,    100  )
        M_vt_int64rc_optional_right_kw(     vt, L"margin"                  ,   1      ,    100  )
        M_vt_int64rc_optional_right_kw(     vt, L"text_space"              ,   1      ,    100  )


        {   //  background_color: [r g b]]
            M_vt_nested_plist(                                                  pl,     3,      3    )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
                                                                                
            M_vt_vlist_optional_right_kw(   vt, L"background_color"           , pl                   )  // add in nested plist to right parms
        }
        {   //  text_color: [r g b]]
            M_vt_nested_plist(                                                  pl,     3,      3    )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
                                                                                
            M_vt_vlist_optional_right_kw(   vt, L"text_color"                 , pl                   )  // add in nested plist to right parms
        }
        {   //  border_color: [r g b]]
            M_vt_nested_plist(                                                  pl,     3,      3    )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
                                                                                
            M_vt_vlist_optional_right_kw(   vt, L"border_color"               , pl                   )  // add in nested plist to right parms
        }

        M_vt_right_conflict_pair(           vt, L"ocean"         , L"land"                           )
  
        M_vt_add_simplified(prefix + L"DRAW_RAMP", vt, verb_draw_ramp                                );
    }
   

    // =====================================
    // @PUT_RAMP
    // =====================================

    {
        M_vt_nofix(                         vt                                                  )            // no positional parms
        M_vt_int64rc_required_right_kw(     vt, L"ramp_n"     ,    1, 100                       )            // required -- ramp number = 1 to 100
        M_vt_int64rc_required_right_kw(     vt, L"outimage_n" ,    1, 100                       )            // required -- outimage number = 1 to 100
        M_vt_int64rc_optional_right_kw(     vt, L"display"    ,    0, 1                         )            // debug display (1 = yes, 0 = no)
                                                                                               
        M_vt_nval_optional_right_kw(        vt, L"ocean"                                        )
        M_vt_nval_optional_right_kw(        vt, L"land"                                         )
   
        M_vt_int64rc_required_right_kw(     vt, L"top_row"                 ,   0      ,   50000 )
        M_vt_int64rc_required_right_kw(     vt, L"left_col"                ,   0      ,   50000 )
        M_vt_int64rc_required_right_kw(     vt, L"width"                   ,   75     ,    5000 )
        M_vt_int64rc_required_right_kw(     vt, L"height"                  ,   500    ,   25000 )
        M_vt_int64rc_optional_right_kw(     vt, L"border"                  ,   0      ,    100  )
        M_vt_int64rc_optional_right_kw(     vt, L"margin"                  ,   1      ,    100  )
        M_vt_int64rc_optional_right_kw(     vt, L"text_space"              ,   1      ,    100  )


        {   //  background_color: [r g b]]
            M_vt_nested_plist(                                                  pl,     3,      3    )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
                                                                                
            M_vt_vlist_optional_right_kw(   vt, L"background_color"           , pl                   )  // add in nested plist to right parms
        }
        {   //  text_color: [r g b]]
            M_vt_nested_plist(                                                  pl,     3,      3    )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
                                                                                
            M_vt_vlist_optional_right_kw(   vt, L"text_color"                 , pl                   )  // add in nested plist to right parms
        }
        {   //  border_color: [r g b]]
            M_vt_nested_plist(                                                  pl,     3,      3    )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
            M_vt_int64rc_nest_pos(                                              pl,     0,    255    )
                                                                                
            M_vt_vlist_optional_right_kw(   vt, L"border_color"               , pl                   )  // add in nested plist to right parms
        }

        M_vt_right_conflict_pair(           vt, L"ocean"         , L"land"                           )
  
        M_vt_add_simplified(prefix + L"PUT_RAMP", vt, verb_put_ramp                                  );
    }



    // =====================================
    // @DESTROY_RAMP 
    // =====================================

    {
        M_vt_nofix(                                  vt                        )                                       // no positional parms
        M_vt_int64rc_required_right_kw(              vt, L"ramp_n",  1, 100    )                                       // required -- ramp number = 1 to 100

        M_vt_add_simplified(prefix + L"DESTROY_RAMP",vt, verb_destroy_ramp     );
    }                                



    // =====================================
    // @DISPLAY_MAP_OPTIONS 
    // =====================================

    {
        M_vt_nofix(                                          vt                            )    
        M_vt_add_simplified(prefix + L"DISPLAY_MAP_OPTIONS", vt, verb_display_map_options  );
    }


    
    // =====================================
    // @CREATE_FILEINFO 
    // =====================================
    
    {
        M_vt_nofix(                         vt                              )                                       // no positional parms
        M_vt_int64rc_required_right_kw(     vt, L"fileinfo_n",  1, 100      )                                       // required -- fileinfo number = 1 to 100
        M_vt_int64rc_optional_right_kw(     vt, L"display"                           ,      0    ,     2     )      // display level 0, 1, or 2  
        M_vt_nval_required_right_kw(        vt, L"raster_data"              )                                       // present -- means this file group has raster data (temp -- required for now)
        
        {
            // grid: [xx xx xx   ] nested keyword                
      
             M_vt_nested_plist(                            npl, 0, 0                                                              )                                                             
             M_vt_int64rc_optional_nest_kw(                npl, L"step"           , 60                      , M_arcsec_180        )
             M_vt_int64rc_optional_nest_kw(                npl, L"step_ns"        , 60                      , M_arcsec_180        )
             M_vt_int64rc_optional_nest_kw(                npl, L"step_ew"        , 60                      , M_arcsec_360        )
             M_vt_int64rc_required_nest_kw(                npl, L"origin_ns"      , -M_arcsec_90            , M_arcsec_90  - 60   )
             M_vt_int64rc_required_nest_kw(                npl, L"origin_ew"      , -M_arcsec_180           , M_arcsec_360 - 60   )
             M_vt_int64rc_required_nest_kw(                npl, L"south_limit"    , -M_arcsec_90            , M_arcsec_90  - 60   )
             M_vt_int64rc_required_nest_kw(                npl, L"west_limit"     , -M_arcsec_180           , M_arcsec_360 - 60   )
             M_vt_int64rc_required_nest_kw(                npl, L"north_limit"    , -M_arcsec_90  + 60      , M_arcsec_90         )
             M_vt_int64rc_required_nest_kw(                npl, L"east_limit"     , -M_arcsec_180 + 60      , M_arcsec_360        )
             
             M_vt_nest_choice_pair(                        npl, L"step"   , L"step_ns"                                            )
             M_vt_nest_choice_pair(                        npl, L"step"   , L"step_ew"                                            )
             M_vt_nest_match_pair(                         npl, L"step_ns", L"step_ew"                                            )
                                                          
             M_vt_vlist_required_right_kw(  vt, L"grid",   npl                                                                    )         // add in nested plist         
        }

        {
            // filenames: [xx xx xx   ] nested keyword                
     
             M_vt_nested_plist(                            npl, 0, 0)
                                                  
             M_vt_string_required_nest_kw(                 npl, L"folder"                                                         )
             M_vt_string_optional_nest_kw(                 npl, L"prefix"                                                         )
             M_vt_string_optional_nest_kw(                 npl, L"separator"                                                      )
             M_vt_string_optional_nest_kw(                 npl, L"suffix"                                                         )
             M_vt_string_optional_nest_kw(                 npl, L"filetype"                                                       )

             M_vt_nval_optional_nest_kw(                   npl, L"sw_corner"                                                      )
             M_vt_nval_optional_nest_kw(                   npl, L"nw_corner"                                                      )
             M_vt_nval_optional_nest_kw(                   npl, L"upper_case"                                                     )
             M_vt_nval_optional_nest_kw(                   npl, L"lower_case"                                                     )
             M_vt_nval_optional_nest_kw(                   npl, L"lat_then_lon"                                                   )
             M_vt_nval_optional_nest_kw(                   npl, L"lon_then_lat"                                                   )
             M_vt_nval_optional_nest_kw(                   npl, L"global_fn"                                                      )
             M_vt_nval_optional_nest_kw(                   npl, L"many_fn"                                                        )
             M_vt_nval_optional_nest_kw(                   npl, L"varying_suffix"                                                 )
             M_vt_nval_optional_nest_kw(                   npl, L"fixed_suffix"                                                   )
             M_vt_nval_optional_nest_kw(                   npl, L"no_suffix"                                                      )
             M_vt_nval_optional_nest_kw(                   npl, L"multi_ft"                                                       )
             M_vt_nval_optional_nest_kw(                   npl, L"single_ft"                                                      )               

             M_vt_nest_conflict_pair(                      npl, L"sw_corner"             , L"nw_corner"                           )
             M_vt_nest_conflict_pair(                      npl, L"upper_case"            , L"lower_case"                          )
             M_vt_nest_conflict_pair(                      npl, L"lat_then_lon"          , L"lon_then_lat"                        )

             M_vt_nest_choice_pair(                        npl, L"global_fn"             , L"many_fn"                             )

             M_vt_nest_conflict_pair(                      npl, L"global_fn"             , L"sw_corner"                           )
             M_vt_nest_conflict_pair(                      npl, L"global_fn"             , L"nw_corner"                           )
             M_vt_nest_conflict_pair(                      npl, L"global_fn"             , L"upper_case"                          )
             M_vt_nest_conflict_pair(                      npl, L"global_fn"             , L"lower_case"                          )
             M_vt_nest_conflict_pair(                      npl, L"global_fn"             , L"lat_then_lon"                        )
             M_vt_nest_conflict_pair(                      npl, L"global_fn"             , L"lon_then_lat"                        )
            
             M_vt_nest_choice_3way(                        npl, L"fixed_suffix"          , L"varying_suffix"  , L"no_suffix"      )
             M_vt_nest_choice_pair(                        npl, L"multi_ft"              , L"single_ft"                           )
             M_vt_nest_conflict_pair(                      npl, L"multi_ft"              , L"filetype"                            )
             M_vt_nest_match_pair(                         npl, L"single_ft"             , L"filetype"                            )
                                                                                
             M_vt_vlist_required_right_kw( vt,L"filenames",npl                                                                    )         // add in nested plist
        }
    
        {
            // data: [xx xx xx   ] nested keyword                
     
             M_vt_nested_plist(                            npl, 0, 0)

             M_vt_int64rc_optional_nest_kw(                npl, L"file_size"             ,        1 , 1000000000000               )
             M_vt_int64rc_optional_nest_kw(                npl, L"file_min_size"         ,        1 , 1000000000000               )
             M_vt_int64rc_optional_nest_kw(                npl, L"file_max_size"         ,        1 , 1000000000000               )

             M_vt_nval_optional_nest_kw(                   npl, L"hgt_layout"                                                     )
             M_vt_nval_optional_nest_kw(                   npl, L"simple_dt2_layout"                                              ) 
                                                        
             M_vt_nval_optional_nest_kw(                   npl, L"int16"                                                          )
             M_vt_nval_optional_nest_kw(                   npl, L"float32"                                                        ) 
             M_vt_nval_optional_nest_kw(                   npl, L"little_endian"                                                  )
             M_vt_nval_optional_nest_kw(                   npl, L"big_endian"                                                     )
             M_vt_nval_optional_nest_kw(                   npl, L"signed_magnitude"                                               )
             M_vt_nval_optional_nest_kw(                   npl, L"twos_compliment"                                                )
                                                        
             M_vt_nval_optional_nest_kw(                   npl, L"extra_row_col"                                                  )
             M_vt_nval_optional_nest_kw(                   npl, L"no_extra_row_col"                                               ) 
             M_vt_nval_optional_nest_kw(                   npl, L"pixels_aligned"                                                 )
             M_vt_nval_optional_nest_kw(                   npl, L"pixels_centered"                                                ) 

             M_vt_int64rc_optional_nest_kw(                npl, L"extent"                ,       60 ,  648000                     )      // should be same as step    ??
             M_vt_int64rc_optional_nest_kw(                npl, L"extent_ns"             ,       60 ,  648000                     )      // should be same as step_ns ??
             M_vt_int64rc_optional_nest_kw(                npl, L"extent_ew"             ,       60 , 1296000                     )      // should be same as step_ew ??
             M_vt_int64rc_optional_nest_kw(                npl, L"ppd"                   ,       10 ,   10800                     )
             M_vt_int64rc_optional_nest_kw(                npl, L"ppd_ns"                ,       10 ,   10800                     )
             M_vt_int64rc_optional_nest_kw(                npl, L"ppd_ew"                ,       10 ,   10800                     )
 
             M_vt_nest_choice_pair(                        npl, L"file_size"             , L"file_min_size"                       ) 
             M_vt_nest_choice_pair(                        npl, L"file_size"             , L"file_max_size"                       )
             M_vt_nest_match_pair(                         npl, L"file_min_size"         , L"file_max_size"                       )

             M_vt_nest_conflict_pair(                      npl, L"hgt_layout"            , L"simple_dt2_layout"                   ) 
             M_vt_nest_conflict_pair(                      npl, L"hgt_layout"            , L"signed_magnitude"                    ) 
             M_vt_nest_conflict_pair(                      npl, L"simple_dt2_layout"     , L"twos_compliment"                     )
             M_vt_nest_conflict_pair(                      npl, L"simple_dt2_layout"     , L"big_endian"                          )
             M_vt_nest_conflict_pair(                      npl, L"simple_dt2_layout"     , L"float32"                             )

             M_vt_nest_choice_pair(                        npl, L"int16"                 , L"float32"                             ) 
             M_vt_nest_choice_pair(                        npl, L"little_endian"         , L"big_endian"                          )
             M_vt_nest_choice_pair(                        npl, L"signed_magnitude"      , L"twos_compliment"                     )
             M_vt_nest_choice_pair(                        npl, L"pixels_aligned"        , L"pixels_centered"                     )
             M_vt_nest_choice_pair(                        npl, L"extra_row_col"         , L"no_extra_row_col"                    ) 

             M_vt_nest_choice_pair(                        npl, L"ppd"                   , L"ppd_ns"                              )
             M_vt_nest_choice_pair(                        npl, L"ppd"                   , L"ppd_ew"                              )
             M_vt_nest_match_pair(                         npl, L"ppd_ns"                , L"ppd_ew"                              )     

             M_vt_nest_choice_pair(                        npl, L"extent"                , L"extent_ns"                           )
             M_vt_nest_choice_pair(                        npl, L"extent"                , L"extent_ew"                           )
             M_vt_nest_match_pair(                         npl, L"extent_ns"             , L"extent_ew"                           )
                                                                                        
             M_vt_vlist_required_right_kw( vt, L"data",    npl                                                                    )         // add in nested plist
        } 

        M_vt_add_simplified(prefix + L"CREATE_FILEINFO",vt, verb_create_fileinfo                                                  );
    }



    // =====================================
    // @DISPLAY_FILEINFO 
    // =====================================
    
    {
        M_vt_nofix(                                         vt                              )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                     vt, L"fileinfo_n",  1, 100      )                                       // required -- fileinfo number = 1 to 100
    
        M_vt_add_simplified(prefix + L"DISPLAY_FILEINFO",   vt, verb_display_fileinfo       );
    }
                                                                    


    // =====================================
    // @DESTROY_FILEINFO 
    // =====================================
    
    {
        M_vt_nofix(                                         vt                              )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                     vt, L"fileinfo_n",  1, 100      )                                       // required -- fileinfo number = 1 to 100
           
        M_vt_add_simplified(prefix + L"DESTROY_FILEINFO",   vt, verb_destroy_fileinfo       );
    }
    


    // =====================================
    // @CREATE_FIXED_RASTER 
    // =====================================

    {
        M_vt_nofix(                                             vt                                                    )             // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"raster_n",  1, 100                              )             // required -- raster number = 1 to 100
                                                                                                                     
        M_vt_nval_optional_right_kw(                            vt, L"display"                                        )
        M_vt_nval_optional_right_kw(                            vt, L"no_display"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"extra_row_col"                                  )
        M_vt_nval_optional_right_kw(                            vt, L"no_extra_row_col"                               )  
        M_vt_int64rc_required_right_kw(                         vt, L"south_edge"   ,  -90, 89                        )             // required -- nominal south edge of raster (in degrees)
        M_vt_int64rc_required_right_kw(                         vt, L"west_edge"    ,  -360, 359                      )             // required -- nominal west  edge of raster (in degrees)
        M_vt_int64rc_required_right_kw(                         vt, L"ns_extent"    ,  1, 180                         )             // required -- nominal N-S extent of raster (in degrees)
        M_vt_int64rc_required_right_kw(                         vt, L"ew_extent"    ,  1, 360                         )             // required -- nominal E-W extent of raster (in degrees)
        M_vt_int64rc_optional_right_kw(                         vt, L"window"       ,  1, 180                         )             // optional -- nominal N-S and E-W extent of window (in degrees)
        M_vt_int64rc_optional_right_kw(                         vt, L"window_ns"    ,  1, 180                         )             // optional -- nominal N-S extent of window (in degrees)
        M_vt_int64rc_optional_right_kw(                         vt, L"window_ew"    ,  1, 360                         )             // optional -- nominal E-W extent of window (in degrees)
        M_vt_int64rc_optional_right_kw(                         vt, L"ppd"          ,  1, 10800                       )             // optional -- ppd = 1 to 10800 (1/3 arc-sec per pixel) -- N-S and E-W are same 
        M_vt_int64rc_optional_right_kw(                         vt, L"ppd_ns"       ,  1, 10800                       )             // optional -- ppd = 1 to 10800 (1/3 arc-sec per pixel) -- N-S  
        M_vt_int64rc_optional_right_kw(                         vt, L"ppd_ew"       ,  1, 10800                       )             // optional -- ppd = 1 to 10800 (1/3 arc-sec per pixel) -- E-W 

        M_vt_right_conflict_pair(                               vt, L"display"              , L"no_display"           )
        M_vt_right_choice_pair(                                 vt, L"extra_row_col"        , L"no_extra_row_col"     )

        M_vt_right_choice_pair(                                 vt, L"ppd"                  , L"ppd_ns"               )
        M_vt_right_choice_pair(                                 vt, L"ppd"                  , L"ppd_ew"               )
        M_vt_right_match_pair(                                  vt, L"ppd_ns"               , L"ppd_ew"               ) 

        M_vt_right_conflict_pair(                               vt, L"window"               , L"window_ns"            )
        M_vt_right_conflict_pair(                               vt, L"window"               , L"window_ew"            )
        M_vt_right_match_pair(                                  vt, L"window_ns"            , L"window_ew"            ) 
     
        M_vt_add_simplified(prefix + L"CREATE_FIXED_RASTER",    vt, verb_create_fixed_raster                          );
    }



    // =====================================
    // @CREATE_AUTO_RASTER 
    // =====================================

    {
        M_vt_nofix(                                             vt                                                    )             // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"raster_n",  1, 100                              )             // required -- raster number = 1 to 100
        M_vt_int64rc_required_right_kw(                         vt, L"map_n"   ,  1, 100                              )             // required -- map number    = 1 to 100

        M_vt_nval_optional_right_kw(                            vt, L"display"                                        )
        M_vt_nval_optional_right_kw(                            vt, L"no_display"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"extra_row_col"                                  )
        M_vt_nval_optional_right_kw(                            vt, L"no_extra_row_col"                               )
        M_vt_int64rc_optional_right_kw(                         vt, L"window"       ,  1, 180                         )             // optional -- nominal N-S and E-W extent of window (in degrees)
        M_vt_int64rc_optional_right_kw(                         vt, L"window_ns"    ,  1, 180                         )             // optional -- nominal N-S extent of window (in degrees)
        M_vt_int64rc_optional_right_kw(                         vt, L"window_ew"    ,  1, 360                         )             // optional -- nominal E-W extent of window (in degrees)
        M_vt_int64rc_optional_right_kw(                         vt, L"ppd"          ,  1, 10800                       )             // optional -- ppd = 1 to 10800 (1/3 arc-sec per pixel) -- N-S and E-W are same 
        M_vt_int64rc_optional_right_kw(                         vt, L"ppd_ns"       ,  1, 10800                       )             // optional -- ppd = 1 to 10800 (1/3 arc-sec per pixel) -- N-S  
        M_vt_int64rc_optional_right_kw(                         vt, L"ppd_ew"       ,  1, 10800                       )             // optional -- ppd = 1 to 10800 (1/3 arc-sec per pixel) -- E-W 

        M_vt_right_conflict_pair(                               vt, L"display"              , L"no_display"           )
        M_vt_right_choice_pair(                                 vt, L"extra_row_col"        , L"no_extra_row_col"     )

        M_vt_right_choice_pair(                                 vt, L"ppd"                  , L"ppd_ns"               )
        M_vt_right_choice_pair(                                 vt, L"ppd"                  , L"ppd_ew"               )
        M_vt_right_match_pair(                                  vt, L"ppd_ns"               , L"ppd_ew"               ) 
       
        M_vt_right_conflict_pair(                               vt, L"window"               , L"window_ns"            )
        M_vt_right_conflict_pair(                               vt, L"window"               , L"window_ew"            )
        M_vt_right_match_pair(                                  vt, L"window_ns"            , L"window_ew"            ) 
        
        M_vt_add_simplified(prefix + L"CREATE_AUTO_RASTER",     vt, verb_create_auto_raster                           );
    }



    // =====================================
    // @FILL_WHOLE_RASTER 
    // =====================================

    {
        M_vt_nofix(                                             vt                                                    )        // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"raster_n"    ,  1, 100                          )        // required -- raster number   = 1 to 100
        M_vt_int64rc_required_right_kw(                         vt, L"fileinfo_n"  ,  1, 100                          )        // required -- fileinfo number = 1 to 100

        M_vt_nval_optional_right_kw(                            vt, L"display"                                        )
        M_vt_nval_optional_right_kw(                            vt, L"no_display"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"display_filenames"                              )
        M_vt_nval_optional_right_kw(                            vt, L"no_display_filenames"                           )
        M_vt_nval_optional_right_kw(                            vt, L"debug_tile"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"no_debug_tile"                                  )
        M_vt_nval_optional_right_kw(                            vt, L"interpolate"                                    )
        M_vt_nval_optional_right_kw(                            vt, L"no_interpolate"                                 )
        M_vt_nval_optional_right_kw(                            vt, L"base_fill"                                      )
        M_vt_nval_optional_right_kw(                            vt, L"empty_fill"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"void_fill"                                      )

        M_vt_right_conflict_pair(                               vt, L"display"              , L"no_display"           )
        M_vt_right_conflict_pair(                               vt, L"display_filenames"    , L"no_display_filenames" )
        M_vt_right_conflict_pair(                               vt, L"debug_tile"           , L"no_debug_tile"        )
        M_vt_right_conflict_pair(                               vt, L"interpolate"          , L"no_interpolate"       )
        M_vt_right_choice_3way(                                 vt, L"base_fill"            , L"empty_fill"           , L"void_fill"          )  

        M_vt_add_simplified(prefix + L"FILL_WHOLE_RASTER",      vt, verb_fill_whole_raster                            );
    }



    // =====================================
    // @SELECTIVE_FILL_RASTER 
    // =====================================

    {
        M_vt_nofix(                                             vt                                                    )        // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"raster_n"    ,  1, 100                          )        // required -- raster number = 1 to 100
        M_vt_int64rc_required_right_kw(                         vt, L"map_n"       ,  1, 100                          )        // required -- map number    = 1 to 100
        M_vt_int64rc_required_right_kw(                         vt, L"fileinfo_n"  ,  1, 100                          )        // required -- fileinfo number = 1 to 100

        M_vt_nval_optional_right_kw(                            vt, L"display"                                        )
        M_vt_nval_optional_right_kw(                            vt, L"no_display"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"display_filenames"                              )
        M_vt_nval_optional_right_kw(                            vt, L"no_display_filenames"                           )
        M_vt_nval_optional_right_kw(                            vt, L"debug_tile"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"no_debug_tile"                                  )
        M_vt_nval_optional_right_kw(                            vt, L"interpolate"                                    )
        M_vt_nval_optional_right_kw(                            vt, L"no_interpolate"                                 )
        M_vt_nval_optional_right_kw(                            vt, L"fill_s_to_n"                                    )
        M_vt_nval_optional_right_kw(                            vt, L"fill_n_to_s"                                    )
        M_vt_nval_optional_right_kw(                            vt, L"base_fill"                                      )
        M_vt_nval_optional_right_kw(                            vt, L"empty_fill"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"void_fill"                                      )

        M_vt_right_conflict_pair(                               vt, L"display"              , L"no_display"           )
        M_vt_right_conflict_pair(                               vt, L"display_filenames"    , L"no_display_filenames" )
        M_vt_right_conflict_pair(                               vt, L"debug_tile"           , L"no_debug_tile"        )
        M_vt_right_conflict_pair(                               vt, L"interpolate"          , L"no_interpolate"       )
        M_vt_right_conflict_pair(                               vt, L"fill_s_to_n"          , L"fill_n_to_s"          )
        M_vt_right_choice_3way(                                 vt, L"base_fill"            , L"empty_fill"           , L"void_fill"          ) 
   
        M_vt_add_simplified(prefix + L"SELECTIVE_FILL_RASTER",  vt, verb_selective_fill_raster  );
    }



    // =====================================
    // @SMOOTH_RASTER 
    // =====================================

    {
        M_vt_nofix(                                             vt                          )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"raster_n",  1, 100    )                                       // required -- raster number = 1 to 100
        M_vt_int64rc_required_right_kw(                         vt, L"passes"  ,  0, 1000   )                                       // required -- number of smoothing passes

        M_vt_add_simplified(prefix + L"SMOOTH_RASTER",          vt, verb_smooth_raster     );
    }

   

    // =====================================
    // @DISPLAY_RASTER 
    // =====================================

    {
        M_vt_nofix(                                             vt                          )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"raster_n",  1, 100    )                                       // required -- raster number = 1 to 100

        M_vt_add_simplified(prefix + L"DISPLAY_RASTER",         vt, verb_display_raster     );
    }


    // =====================================
    // @WRITEOUT_RASTER 
    // =====================================

    {
        M_vt_nofix(                                             vt                                                    )        // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"raster_n"    ,  1, 100                          )        // required -- raster number   = 1 to 100
        M_vt_int64rc_required_right_kw(                         vt, L"fileinfo_n"  ,  1, 100                          )        // required -- fileinfo number = 1 to 100

        M_vt_nval_optional_right_kw(                            vt, L"suppress_empty"                                 )
        M_vt_nval_optional_right_kw(                            vt, L"no_suppress_empty"                              )
        M_vt_nval_optional_right_kw(                            vt, L"set_empty_height"                               )
        M_vt_nval_optional_right_kw(                            vt, L"no_set_empty_height"                            )
                                                           
        M_vt_int64rc_optional_right_kw(                         vt, L"empty_height",  -32700, 32700                   )       // optional -- empty height to use

        M_vt_nval_optional_right_kw(                            vt, L"display"                                        )
        M_vt_nval_optional_right_kw(                            vt, L"no_display"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"display_filenames"                              )
        M_vt_nval_optional_right_kw(                            vt, L"no_display_filenames"                           )
        M_vt_nval_optional_right_kw(                            vt, L"debug_tile"                                     )
        M_vt_nval_optional_right_kw(                            vt, L"no_debug_tile"                                  )

        M_vt_right_match_pair(                                  vt, L"set_empty_height"     , L"empty_height"         )

        M_vt_right_conflict_pair(                               vt, L"suppress_empty"       , L"no_suppress_empty"    )
        M_vt_right_conflict_pair(                               vt, L"set_empty_height"     , L"no_set_empty_height"  )

        M_vt_right_conflict_pair(                               vt, L"display"              , L"no_display"           )
        M_vt_right_conflict_pair(                               vt, L"display_filenames"    , L"no_display_filenames" )
        M_vt_right_conflict_pair(                               vt, L"debug_tile"           , L"no_debug_tile"        )

        M_vt_add_simplified(prefix + L"WRITEOUT_RASTER",        vt, verb_writeout_raster                              );
    }
    
       
    // =====================================
    // @DESTROY_RASTER 
    // =====================================

    {
        M_vt_nofix(                                             vt                          )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"raster_n",  1, 100    )                                       // required -- raster number = 1 to 100

        M_vt_add_simplified(prefix + L"DESTROY_RASTER",         vt, verb_destroy_raster     );
    }
     

    // =====================================
    // @CREATE_MAP 
    // =====================================

    {
        M_vt_nofix(                           vt                                                                      )      // no positional parms
        M_vt_int64rc_required_right_kw(       vt, L"map_n"                                    ,      1    ,     100   )      // required -- map number    = 1 to 100
        
        M_vt_int64rc_optional_right_kw(       vt, L"display"                                  ,      0    ,     2     )      // display level 0, 1, or 2
        M_vt_int64rc_optional_right_kw(       vt, L"samples"                                  ,      100  , 100000000 )      // display level 0, 1, or 2
        M_vt_float64rc_optional_right_kw(     vt, L"padding"                                  ,      0.0  , 10.0      )      // extra padding (in degrees) for minlat/maxlon, etc.

        M_vt_float64rc_optional_right_kw(     vt, L"south_limit"                              ,    -90.0  ,    90.0   )
        M_vt_float64rc_optional_right_kw(     vt, L"north_limit"                              ,    -90.0  ,    90.0   )
        M_vt_float64rc_optional_right_kw(     vt, L"west_limit"                               ,   -360.0  ,   360.0   )
        M_vt_float64rc_optional_right_kw(     vt, L"east_limit"                               ,   -360.0  ,   360.0   )
      
        {
            // rectangular: [xx xx xx]                   

             M_vt_nested_plist(                                          npl, 0, 0)
                
             M_vt_int64rc_optional_nest_kw(                              npl, L"center_ppd"   ,      5    , 10800   )
             M_vt_float64rc_optional_nest_kw(                            npl, L"south"        ,    -90.0  ,    90.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"west"         ,   -180.0  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_ns"       ,      0.1  ,   360.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_ew"       ,      0.1  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"ns_factor"    ,      0.1  ,   50.0  )

             M_vt_vlist_optional_right_kw(     vt, L"rectangular"      , npl                                        )         // add in nested plist
        }

        {
            // mercator: [xx xx xx]                 

             M_vt_nested_plist(                                          npl, 0, 0)
                
             M_vt_int64rc_optional_nest_kw(                              npl, L"center_ppd"   ,      5    , 10800   )
             M_vt_float64rc_optional_nest_kw(                            npl, L"south"        ,    -90.0  ,    90.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"west"         ,   -180.0  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_ns"       ,      0.1  ,   360.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_ew"       ,      0.1  ,   180.0 )

             M_vt_vlist_optional_right_kw(     vt, L"mercator"         , npl                                        )         // add in nested plist 
        }

        {
            // polar: [xx xx xx]                 

             M_vt_nested_plist(                                          npl, 0, 0)
                
             M_vt_int64rc_optional_nest_kw(                              npl, L"center_ppd"   ,      5    , 10800   )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lon0"         ,   -180.0  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lat_adj"      ,   -180.0  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_rows"     ,      0.1  ,   360.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_cols"     ,      0.1  ,   360.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_row0"     ,   -170.0  ,   170.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_col0"     ,   -170.0  ,   170.0 )

             M_vt_vlist_optional_right_kw(    vt, L"polar"             , npl                                        )         // add in nested plist 
        }

        {
            // stereographic: [xx xx xx]                 

             M_vt_nested_plist(                                          npl, 0, 0)
                
             M_vt_int64rc_optional_nest_kw(                              npl, L"center_ppd"   ,      5    , 10800   )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lat0"         ,    -90.0  ,    90.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lon0"         ,   -180.0  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_rows"     ,      0.1  ,   360.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_cols"     ,      0.1  ,   360.0 )
             M_vt_int64rc_optional_nest_kw(                              npl, L"rows"         ,     50    ,200000   )
             M_vt_int64rc_optional_nest_kw(                              npl, L"cols"         ,     50    ,200000   )

             M_vt_nest_conflict_pair(                                    npl, L"deg_rows   ", L"rows"               )
             M_vt_nest_conflict_pair(                                    npl, L"deg_cols   ", L"cols"               )

             M_vt_vlist_optional_right_kw(    vt, L"stereographic"     , npl                                        )         // add in nested plist
        }
   
        {
            // orthographic: [xx xx xx]                 

             M_vt_nested_plist(                                          npl, 0, 0)
                
             M_vt_int64rc_optional_nest_kw(                              npl, L"center_ppd"   ,      5    , 10800   )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lat0"         ,    -90.0  ,    90.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lon0"         ,   -180.0  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_rows"     ,      0.1  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_cols"     ,      0.1  ,   180.0 )
             M_vt_int64rc_optional_nest_kw(                              npl, L"rows"         ,     50    ,200000   )
             M_vt_int64rc_optional_nest_kw(                              npl, L"cols"         ,     50    ,200000   )

             M_vt_nest_conflict_pair(                                    npl, L"deg_rows   ", L"rows"               )
             M_vt_nest_conflict_pair(                                    npl, L"deg_cols   ", L"cols"               )

             M_vt_vlist_optional_right_kw(    vt, L"orthographic"      , npl                                        )         // add in nested plist
        }

        {
            // azimuthal: [xx xx xx]                 

             M_vt_nested_plist(                                          npl, 0, 0)
                
             M_vt_int64rc_optional_nest_kw(                              npl, L"center_ppd"   ,      5    , 10800   )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lat0"         ,    -90.0  ,    90.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lon0"         ,   -180.0  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_rows"     ,      0.1  ,   360.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_cols"     ,      0.1  ,   360.0 )
             M_vt_int64rc_optional_nest_kw(                              npl, L"rows"         ,     50    ,200000   )
             M_vt_int64rc_optional_nest_kw(                              npl, L"cols"         ,     50    ,200000   )

             M_vt_nest_conflict_pair(                                    npl, L"deg_rows   ", L"rows"               )
             M_vt_nest_conflict_pair(                                    npl, L"deg_cols   ", L"cols"               )

             M_vt_vlist_optional_right_kw(    vt, L"azimuthal"         , npl                                        )         // add in nested plist
        }

        {
            // azimuthal_equal_area: [xx xx xx]                 

             M_vt_nested_plist(                                          npl, 0, 0)
                
             M_vt_int64rc_optional_nest_kw(                              npl, L"center_ppd"   ,      5    , 10800   )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lat0"         ,    -90.0  ,    90.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"lon0"         ,   -180.0  ,   180.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_rows"     ,      0.1  ,   360.0 )
             M_vt_float64rc_optional_nest_kw(                            npl, L"deg_cols"     ,      0.1  ,   360.0 )
             M_vt_int64rc_optional_nest_kw(                              npl, L"rows"         ,     50    ,200000   )
             M_vt_int64rc_optional_nest_kw(                              npl, L"cols"         ,     50    ,200000   )

             M_vt_nest_conflict_pair(                                    npl, L"deg_rows   ", L"rows"               )
             M_vt_nest_conflict_pair(                                    npl, L"deg_cols   ", L"cols"               )

             M_vt_vlist_optional_right_kw(    vt, L"azimuthal_equal_area", npl                                      )         // add in nested plist
        }


        M_vt_right_conflict_7way(             vt, L"rectangular"
                                                , L"mercator"
                                                , L"polar"
                                                , L"azimuthal"
                                                , L"azimuthal_equal_area"  
                                                , L"stereographic"                                                  
                                                , L"orthographic"                                                   )
    
        M_vt_add_simplified(prefix + L"CREATE_MAP",   vt, verb_create_map                                           );
    }



    // =====================================
    // @ADD_DEM 
    // =====================================

    {
        M_vt_nofix(                                  vt                                             )    // no positional parms
        M_vt_int64rc_required_right_kw(              vt, L"raster_n"            ,  1       , 100    )    // required -- raster number = 1 to 100
        M_vt_int64rc_required_right_kw(              vt, L"map_n"               ,  1       , 100    )    // required -- map number    = 1 to 100
                                                   
        M_vt_int64rc_optional_right_kw(              vt, L"display"             ,  0       , 1      )    // debug display (1 = yes, 0 = no)
        M_vt_int64rc_optional_right_kw(              vt, L"sampling_method"     ,  1       , 2      )    // sampling method (1 = nearest neighbor, 2 = linear interpolate)
        M_vt_nval_optional_right_kw(                 vt, L"normal_fill"                             )    // do normal fill
        M_vt_nval_optional_right_kw(                 vt, L"ocean_fill"                              )    // do ocean fill
                                                   
        M_vt_right_conflict_pair(                    vt, L"normal_fill" , L"ocean_fill"             )

        M_vt_add_simplified(prefix + L"ADD_DEM",     vt, verb_add_dem                               );
    }



    // =====================================
    // @DETECT_WATER 
    // =====================================

    {
        M_vt_nofix(                                   vt                          )                                       // no positional parms
        M_vt_int64rc_required_right_kw(               vt, L"map_n"   ,  1, 100    )                                       // required -- map number    = 1 to 100
        M_vt_add_simplified(prefix + L"DETECT_WATER", vt, verb_detect_water       );
    }



    // =====================================
    // @ADD_GRATICULE 
    // =====================================

    {
        M_vt_nofix(                                   vt                                                  )    // no positional parms
        M_vt_int64rc_required_right_kw(               vt, L"map_n"           ,   1      ,    100          )    // required -- map number    = 1 to 1002
        M_vt_int64rc_optional_right_kw(               vt, L"display"         ,   0      ,    1            )    // debug display (1 = yes, 0 = no)
        M_vt_int64rc_optional_right_kw(               vt, L"weight"          ,   1      ,    100          )
        M_vt_int64rc_optional_right_kw(               vt, L"ns_spacing"      ,   2      ,    M_arcsec_180 )
        M_vt_int64rc_optional_right_kw(               vt, L"ew_spacing"      ,   2      ,    M_arcsec_180 ) 
         
            
        // ew_reduce: [xx xx xx]  keyword

        {                 
             M_vt_nested_plist(                                         npl, 0, 0                         )                                                       
             M_vt_int64rc_optional_nest_kw(                             npl, L"factor_1"   ,   1    , 90  )
             M_vt_int64rc_optional_nest_kw(                             npl, L"factor_2"   ,   1    , 90  )
             M_vt_int64rc_optional_nest_kw(                             npl, L"factor_3"   ,   1    , 90  )
             M_vt_int64rc_optional_nest_kw(                             npl, L"latitude_1" ,   0    , 90  )
             M_vt_int64rc_optional_nest_kw(                             npl, L"latitude_2" ,   0    , 90  )
             M_vt_int64rc_optional_nest_kw(                             npl, L"latitude_3" ,   0    , 90  )
             M_vt_vlist_optional_right_kw(            vt, L"ew_reduce", npl                               )         // add in nested plist to ew_reduce keyword vlist
        }
      
        M_vt_add_simplified(prefix + L"ADD_GRATICULE",vt, verb_add_graticule                              );
    }



    // =====================================
    // @ADD_SLOPES 
    // =====================================

    {
        M_vt_nofix(                                  vt                                                    )       // no positional parms
        M_vt_int64rc_required_right_kw(              vt, L"map_n"                ,  1,    100              )       // required -- map number (= 1 to 100)
        M_vt_int64rc_optional_right_kw(              vt, L"display"              ,  0,    1                )       // debug display   (1 = yes, 0 = no) 
        M_vt_int64rc_optional_right_kw(              vt, L"land"                 ,  0,    1                )       // do land slopes  (1 = yes, 0 = no) 
        M_vt_int64rc_optional_right_kw(              vt, L"ocean"                ,  0,    1                )       // do ocean slopes (1 = yes, 0 = no) 
        M_vt_float64rc_optional_right_kw(            vt, L"land_vert_factor"     ,   0.00   ,   1000.0     )
        M_vt_float64rc_optional_right_kw(            vt, L"ocean_vert_factor"    ,   0.00   ,   1000.0     )
                                                    
        M_vt_nval_optional_right_kw(                 vt, L"light_n"                                        )
        M_vt_nval_optional_right_kw(                 vt, L"light_ne"                                       )
        M_vt_nval_optional_right_kw(                 vt, L"light_e"                                        )
        M_vt_nval_optional_right_kw(                 vt, L"light_se"                                       )
        M_vt_nval_optional_right_kw(                 vt, L"light_s"                                        )
        M_vt_nval_optional_right_kw(                 vt, L"light_sw"                                       )
        M_vt_nval_optional_right_kw(                 vt, L"light_w"                                        )
        M_vt_nval_optional_right_kw(                 vt, L"light_nw"                                       )
        M_vt_nval_optional_right_kw(                 vt, L"light_above"                                    )
                                                    
        M_vt_right_conflict_9way(                    vt, L"light_n", L"light_ne", L"light_e", L"light_se", L"light_s", L"light_sw", L"light_w", L"light_nw", L"light_above")  

        M_vt_add_simplified(prefix + L"ADD_SLOPES",  vt, verb_add_slopes                                   );
    }

     
    // =====================================
    // @EXTRACT_MAP 
    // =====================================

    {
        M_vt_nofix(                                  vt                    )                                       // no positional parms
        M_vt_int64rc_required_right_kw(              vt, L"map_n",   1, 100)                                       // required -- map number = 1 to 100
        M_vt_nval_optional_right_kw(                 vt, L"cols"           )                                       // fields to extract -- only one allowed
        M_vt_nval_optional_right_kw(                 vt, L"rows"           )
        M_vt_nval_optional_right_kw(                 vt, L"maxcol"         )
        M_vt_nval_optional_right_kw(                 vt, L"maxrow"         )
        M_vt_nval_optional_right_kw(                 vt, L"npts"           )
        M_vt_nval_optional_right_kw(                 vt, L"mapname_suffix" )
        M_vt_right_conflict_6way(                    vt, L"cols" , L"rows", L"maxcol", L"maxrow", L"npts", L"mapname_suffix")

        M_vt_add_simplified(prefix + L"EXTRACT_MAP", vt, verb_extract_map  );
    }

    
    // =====================================
    // @DESTROY_MAP 
    // =====================================

    {
        M_vt_nofix(                                  vt                          )     // no positional parms
        M_vt_int64rc_required_right_kw(              vt, L"map_n"   ,  1, 100    )     // required -- map number    = 1 to 100

        M_vt_add_simplified(prefix + L"DESTROY_MAP", vt, verb_destroy_map        );
    }
     
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    // =====================================
    // @CREATE_MAP_OUTIMAGE 
    // =====================================

    {
        M_vt_nofix(                                         vt                            )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                     vt, L"outimage_n",  1, 100    )                                       // required -- outimage number    = 1 to 100
        M_vt_int64rc_required_right_kw(                     vt, L"map_n"     ,  1, 100    )                                       // required -- map number         = 1 to 100
        M_vt_int64rc_optional_right_kw(                     vt, L"display"   ,  0, 1      )                                       // debug display (1 = yes, 0 = no)

        M_vt_add_simplified(prefix + L"CREATE_MAP_OUTIMAGE",vt, verb_create_map_outimage  );
    }
  



    // =====================================
    // @SET_MAP_WINDOW
    // =====================================

    {
        M_vt_nofix(                                     vt                                          )          // no positional parms
        M_vt_int64rc_required_right_kw(                 vt, L"outimage_n"             ,    1, 100   )          // required -- outimage number = 1 to 100
        M_vt_int64rc_required_right_kw(                 vt, L"map_n"                  ,    1, 100   )          // required -- map      number = 1 to 100
        M_vt_int64rc_optional_right_kw(                 vt, L"display"                ,    0, 1     )          // debug display       (1 = yes, 0 = no)
                                                       
        M_vt_int64rc_optional_right_kw(                 vt, L"outimage_top_row"       ,    0, 60000 )          // top of map window is drawn starting at this row in outimage buffer 
        M_vt_int64rc_optional_right_kw(                 vt, L"outimage_left_column"   ,    0, 60000 )          // left side of map window is drawn starting at this column in outimage buffer
        M_vt_int64rc_optional_right_kw(                 vt, L"map_top_row"            ,    0, 60000 )          // top row of map to be drawn (window -- default is start at top of map) 
        M_vt_int64rc_optional_right_kw(                 vt, L"map_left_column"        ,    0, 60000 )          // left row of map to be drawn (window -- default is start at 1st column in map) 
        M_vt_int64rc_optional_right_kw(                 vt, L"map_width"              ,    0, 60000 )          // number of map columns to draw (0 = all -- default) 
        M_vt_int64rc_optional_right_kw(                 vt, L"map_height"             ,    0, 60000 )          // number of map rows to draw    (0 = all -- default)   

        M_vt_add_simplified(prefix + L"SET_MAP_WINDOW", vt, verb_set_map_window                     );
    }

          
    // =====================================
    // @PUT_MAP
    // =====================================

    {
        M_vt_nofix(                                  vt                                          )          // no positional parms
        M_vt_int64rc_required_right_kw(              vt, L"outimage_n"             ,    1, 100   )          // required -- outimage number = 1 to 100
        M_vt_int64rc_required_right_kw(              vt, L"map_n"                  ,    1, 100   )          // required -- map      number = 1 to 100
        M_vt_int64rc_required_right_kw(              vt, L"ramp_n"                 ,    1, 100   )          // required -- ramp     number = 1 to 100
        M_vt_int64rc_optional_right_kw(              vt, L"display"                ,    0, 1     )          // debug display       (1 = yes, 0 = no)
        M_vt_int64rc_optional_right_kw(              vt, L"shaded_relief"          ,    0, 1     )          // do shaded relief    (1 = yes, 0 = no)
        M_vt_int64rc_optional_right_kw(              vt, L"land_hypsometry"        ,    0, 1     )          // do land hypsometry  (1 = yes, 0 = no)
        M_vt_int64rc_optional_right_kw(              vt, L"ocean_bathymetry"       ,    0, 1     )          // do ocean bathymetry (1 = yes, 0 = no)
    //  M_vt_int64rc_optional_right_kw(              vt, L"outimage_top_row"       ,    0, 60000 )          // top of map window is drawn starting at this row in outimage buffer 
    //  M_vt_int64rc_optional_right_kw(              vt, L"outimage_left_column"   ,    0, 60000 )          // left side of map window is drawn starting at this column in outimage buffer
    //  M_vt_int64rc_optional_right_kw(              vt, L"map_top_row"            ,    0, 60000 )          // top row of map to be drawn (window -- default is start at top of map) 
    //  M_vt_int64rc_optional_right_kw(              vt, L"map_left_column"        ,    0, 60000 )          // left row of map to be drawn (window -- default is start at 1st column in map) 
    //  M_vt_int64rc_optional_right_kw(              vt, L"map_width"              ,    0, 60000 )          // number of map columns to draw (0 = all -- default) 
    //  M_vt_int64rc_optional_right_kw(              vt, L"map_height"             ,    0, 60000 )          // number of map rows to draw    (0 = all -- default)   

        M_vt_add_simplified(prefix + L"PUT_MAP",     vt, verb_put_map                            );
    }


    // =====================================
    // @ANNOTATE_MAP                           ?????????????????????? temp ???????????????????????
    // =====================================

    {
        M_vt_nofix(                                   vt                                            )          // no positional parms
        M_vt_int64rc_required_right_kw(               vt, L"outimage_n"               ,    1, 100   )          // required -- outimage number = 1 to 100
        M_vt_int64rc_required_right_kw(               vt, L"map_n"                    ,    1, 100   )          // required -- map      number = 1 to 100
                                                          

        M_vt_add_simplified(prefix + L"ANNOTATE_MAP", vt, verb_annotate_map                         );
    }  


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return; 
}
M_endf


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""