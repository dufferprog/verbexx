// map_verb.c

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ============
////     map_verb.cpp -- process map-oriented verbs and options -- called by corresponding static functions in map_addverb.cpp
////     ============
//// 
//// 
////    
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "h__include.h"
#pragma hdrstop("../pch/pch_std.pch")

#define M_IN_MAP_DLL

#include "h__common.h"

#include "h_ex_interface.h"

#include "h_draw_outimage.h"
#include "h_draw_verb.h"

#include "h_map_types.h"
#include "h_map_global.h"
#include "h_map_parms.h"  
#include "h_map_data.h"
#include "h_map_annotate.h"
#include "h_map_image.h"
#include "h_map_ramp.h"
#include "h_map_tile.h"
#include "h_map_dem.h"
#include "h_map_raster.h"
#include "h_map_verb.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///          O P T I O N S 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
//
//   mapverb_color_vlist_rgb() -- internal helper function to process rgb values color vlist 
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void mapverb_color_vlist_rgb(const vlist_S vlist, rgb_S& rgb) try
{
    // known that there are 3 integer positional parms 0-255 in passed-in vlist

    rgb.r = (uint8_t)(vlist.values.at(0).int64); 
    rgb.g = (uint8_t)(vlist.values.at(1).int64); 
    rgb.b = (uint8_t)(vlist.values.at(2).int64); 
    	M__(M_out(L"mapverb_color_vlist_rgb() -- r=%d g=%d b=%d") % rgb.r % rgb.g % rgb.b;)
    return; 
}
M_endf






////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_map_option() -- set global mapparm_S based on keyword options from @MAP_OPTIONS verb  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int32_t mapverb_option_color(const e_expression_S& expression, const vlist_S vlist, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_option_color() called");)

    int rc {0};        // count of errors  
   

    // loop to look at sub-keywords 

    for (const auto& elem : vlist.eval_kws)
    {
      		M__(M_out(L"mapverb_option_color(): elem.first=%s") % elem.first;)

             if (elem.first == L"dem_void"          ) mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.dem_void            );            
        else if (elem.first == L"graticule"         ) mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.graticule           );
        else if (elem.first == L"ocean"             ) mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.ocean               );
        else if (elem.first == L"land"              ) mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.land                );
        else if (elem.first == L"outside_dem"       ) mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.outside_dem         );
        else if (elem.first == L"outside_map"       ) mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.outside_map         );
        else if (elem.first == L"outside_projection") mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.outside_projection  );
        else if (elem.first == L"outside_raster"    ) mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.outside_raster      ); 
        else if (elem.first == L"water"             ) mapverb_color_vlist_rgb(*(elem.second.vlist_sp), parm.color.water               );
       
        else    // unexpected top-level keyword -- should not occur
        {
            M_out_emsg(L"mapverb_option_color() -- unknown top-level keyword=%s:") % elem.first; 
            rc++; 
        }
    }

    return rc;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////
// 
//   mapverb_option_slice
//
///////////////////////////////////////////////////////////////////////////////////

static int32_t mapverb_option_slice(const e_expression_S& expression, const vlist_S vlist, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_option_slice() called");)        

    int rc {0};        // count of errors  
    

    for (const auto& elem : vlist.eval_kws)
    {
             if (elem.first == L"display"                ) parm.slice.display         = true;
        else if (elem.first == L"max"                    ) parm.slice.max             = (int32_t)(elem.second.int64);
        else if (elem.first == L"min_area_nothread"      ) parm.slice.nonthread_area  = (int32_t)(elem.second.int64);
        else if (elem.first == L"min_area_thread"        ) parm.slice.thread_area     = (int32_t)(elem.second.int64);
        else if (elem.first == L"min_dimension_nothread" ) parm.slice.nonthread_dim   = (int32_t)(elem.second.int64);
        else if (elem.first == L"min_dimension_thread"   ) parm.slice.thread_dim      = (int32_t)(elem.second.int64);
        else if (elem.first == L"no_display"             ) parm.slice.display         = false; 
        else    // unexpected top-level keyword -- should not occur
        {
            M_out_emsg(L"mapverb_option_slice() -- unknown top-level keyword=%s:") % elem.first; 
            rc++; 
        }   
    }

    return rc;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////
// 
//   mapverb_option_thread
//
///////////////////////////////////////////////////////////////////////////////////

static int32_t mapverb_option_thread(const e_expression_S& expression, const vlist_S vlist, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_option_thread() called");)

    int rc {0};        // count of errors  


    for (const auto& elem : vlist.eval_kws)
    {
             if (elem.first == L"compute_slopes"    ) parm.thread.n_slope          = (int32_t)(elem.second.int64);
        else if (elem.first == L"display"           ) parm.thread.display          = true;
        else if (elem.first == L"outimage_fill"     ) parm.thread.n_outimage_fill  = (int32_t)(elem.second.int64);
        else if (elem.first == L"map_fill"          ) parm.thread.n_map_fill       = (int32_t)(elem.second.int64);
        else if (elem.first == L"max"               ) parm.thread.max              = (int32_t)(elem.second.int64);
        else if (elem.first == L"no_display"        ) parm.thread.display          = false; 
        else if (elem.first == L"water_detect"      ) parm.thread.n_water          = (int32_t)(elem.second.int64);
        else    // unexpected top-level keyword -- should not occur
        {
            M_out_emsg(L"mapverb_option_thread() -- unknown top-level keyword=%s:") % elem.first; 
            rc++; 
        }   
    }
    
    return rc;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////
// 
//   mapverb_option_water_detect
//
///////////////////////////////////////////////////////////////////////////////////

static int32_t mapverb_option_water_detect(const e_expression_S& expression, const vlist_S vlist, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_option_water_detect() called");)
  
    int rc {0};        // count of errors  


    for (const auto& elem : vlist.eval_kws)
    {
        if (elem.first == L"adaptive")
        {          
            parm.water.adaptive = true;

            for (const auto& elem2 : (*(elem.second.vlist_sp)).eval_kws)
            {
                     if (elem2.first == L"hi_min_size"         ) parm.water.a_hi_min_lake      = (int32_t)(elem2.second.int64);
                else if (elem2.first == L"hi_stride"           ) parm.water.a_hi_stride        = (int32_t)(elem2.second.int64);
                else if (elem2.first == L"icecap_n_latitude"   ) parm.water.a_icecap_lat_n     = (int32_t)(elem2.second.int64);
                else if (elem2.first == L"icecap_s_latitude"   ) parm.water.a_icecap_lat_s     = (int32_t)(elem2.second.int64);
                else if (elem2.first == L"lo_min_size"         ) parm.water.a_lo_min_lake      = (int32_t)(elem2.second.int64);
                else if (elem2.first == L"lo_stride"           ) parm.water.a_lo_stride        = (int32_t)(elem2.second.int64);
                else if (elem2.first == L"neighbor_row_col"    ) parm.water.a_neighbor_xy      = (int32_t)(elem2.second.int64);
                else if (elem2.first == L"neighbor_samples"    ) parm.water.a_neighbor_samples = (int32_t)(elem2.second.int64);
                else if (elem2.first == L"subslice_samples"    ) parm.water.a_samples          = (int32_t)(elem2.second.int64);
                else
                {
                   M_out_emsg(L"mapverb_option_water_detect() -- unknown adaptive: sub-keyword=%s:") % elem2.first; 
                   rc++;
                }
            }
        }   
        else if (elem.first == L"lake"              ) parm.water.check_lake   = WATER_LAKE; 
        else if (elem.first == L"max_lake_ht"       ) parm.water.ht_max       = (int32_t)(elem.second.int64);
        else if (elem.first == L"min_lake_ht"       ) parm.water.ht_min       = (int32_t)(elem.second.int64);
        else if (elem.first == L"min_size"          ) parm.water.min_lake     = (int32_t)(elem.second.int64);
        else if (elem.first == L"no_adaptive"       ) parm.water.adaptive     = false;  
        else if (elem.first == L"no_lake"           ) parm.water.check_lake   = WATER_NOLAKE; 
        else if (elem.first == L"no_ocean"          ) parm.water.check_ocean  = false; 
        else if (elem.first == L"ocean"             ) parm.water.check_ocean  = true;
        else if (elem.first == L"stride"            ) parm.water.stride       = (int32_t)(elem.second.int64);
        else    // unexpected top-level keyword -- should not occur
        {
            M_out_emsg(L"mapverb_option_water_detect() -- unknown top-level keyword=%s:") % elem.first; 
            rc++; 
        }   
    }

    return rc;
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////
//
//
//   mapverb_map_option() -- main function for @OPTION verb 
//
//
/////////////////////////////////////////////////////////////////////////////////////////

int32_t mapverb_map_option(const e_expression_S& expression, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_map_option() called");)

    int rc {0};        // count of errors  

    M_dv_some(M_out(L"=======> Doing   @MAP_OPTION  <=======");)  

    if (expression.rparms.kw_ct > 0)
    {
        // main loop to look at each top-level keyword for this invocation of the @OPTION verb 

        for (const auto& elem : expression.rparms.eval_kws)
        {
                 if (elem.first == L"color"             ) rc += mapverb_option_color(        expression, *(elem.second.vlist_sp), parm);            
            else if (elem.first == L"slice"             ) rc += mapverb_option_slice(        expression, *(elem.second.vlist_sp), parm); 
            else if (elem.first == L"thread"            ) rc += mapverb_option_thread(       expression, *(elem.second.vlist_sp), parm);
            else if (elem.first == L"water_detect"      ) rc += mapverb_option_water_detect( expression, *(elem.second.vlist_sp), parm);

            else    // unexpected top-level keyword -- should not occur
            {
                M_out_emsg(L"mapverb_map_option() -- unknown top-level keyword=%s:") % elem.first; 
                rc++; 
            }
        }   
    }

    return rc; 
}
M_endf





////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_display_map_options() -- main function or @DISPLAY_OPTIONS verb
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


int32_t mapverb_display_map_options(const e_expression_S& expression, mapparm_S& parm) try
{

    M_dv_some(M_out(L"=======> Doing   @DISPLAY_MAP_OPTIONS  <=======");)  
    display_options(parm); 
    return 0; 
}
M_endf





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///          F I L E I N F O 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_create_fileinfo() -- do @CREATE_FILEINFO    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_create_fileinfo(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;

    M_get_fileinfo_n(L"mapverb_create_fileinfo")

 
    // Destroy fileinfo first, if already allocated
    // --------------------------------------------

    if (fileinfo_p != nullptr)
    {
        M_dv_few(M_out(L"=======> @CREATE_FILEINFO fileinfo_n:%d -- destroying old fileinfo first <======") % n_fileinfo;)

        rc = mapverb_destroy_fileinfo(expression, parm);  
        if (rc != 0) return rc;  

        fileinfo_p = nullptr;                    // clear out local copy of fileinfo_p after destroy_fileinfo()
    }


    // allocate new (uninitialized) fileinfo_S in repository 
    // -----------------------------------------------------

    M_dv_few(M_out(L"=======> Doing   @CREATE_FILEINFO fileinfo_n:%d   <=======") % n_fileinfo;)

    fileinfo_p = alloc_fileinfo(n_fileinfo);                // allocate new fileinfo_S in repository 


    // fill in newly-allocated located fileinfo based on @CREATE_FILEINFO verb options
    // -------------------------------------------------------------------------------

    int64_t      display {0};                  // local variable for display setting   
    std::wstring suffix  { };                  // local variable to collect for suffix string (if any specified)

    M_get_right_keyword_int64(  expression      ,   L"display"                , display                           , int64_t  )   // remember display setting in local variable
    M_get_right_keyword_nval(   expression      ,   L"raster_data"            , fileinfo_p->raster_data           , true     )
                                                                            
    M_get_right_keyword_vlist(  expression      ,   L"grid"                   , grid_vlist                                   )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"east_limit"             , fileinfo_p->limit_e               , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"north_limit"            , fileinfo_p->limit_n               , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"south_limit"            , fileinfo_p->limit_s               , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"west_limit"             , fileinfo_p->limit_w               , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"origin_ew"              , fileinfo_p->origin_ew             , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"origin_ns"              , fileinfo_p->origin_ns             , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"step"                   , fileinfo_p->step_ew               , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"step"                   , fileinfo_p->step_ns               , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"step_ew"                , fileinfo_p->step_ew               , ll_T     )
    M_get_nest_keyword_int64(   grid_vlist      ,   L"step_ns"                , fileinfo_p->step_ns               , ll_T     )
  
    // --------------------------------------------------------------------------------------------------------------------------

    M_get_right_keyword_vlist(  expression      ,   L"filenames"              , filenames_vlist                              )
    M_get_nest_keyword_string(  filenames_vlist ,   L"folder"                 , fileinfo_p->folder                           )
    M_get_nest_keyword_string(  filenames_vlist ,   L"prefix"                 , fileinfo_p->prefix                           )
    M_get_nest_keyword_string(  filenames_vlist ,   L"separator"              , fileinfo_p->separator                        )
    M_get_nest_keyword_string(  filenames_vlist ,   L"suffix"                 , suffix                                       )
    M_get_nest_keyword_string(  filenames_vlist ,   L"filetype"               , fileinfo_p->filetype                         )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"sw_corner"              , fileinfo_p->fn_sw_corner          , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"nw_corner"              , fileinfo_p->fn_nw_corner          , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"upper_case"             , fileinfo_p->fn_upper_case         , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"lower_case"             , fileinfo_p->fn_lower_case         , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"lat_then_lon"           , fileinfo_p->fn_lat_then_lon       , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"lon_then_lat"           , fileinfo_p->fn_lon_then_lat       , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"global_fn"              , fileinfo_p->fn_global             , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"many_fn"                , fileinfo_p->fn_many               , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"multi_ft"               , fileinfo_p->multi_ft              , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"single_ft"              , fileinfo_p->single_ft             , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"no_suffix"              , fileinfo_p->fixed_suffix          , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"fixed_suffix"           , fileinfo_p->fixed_suffix          , true     )
    M_get_nest_keyword_nval(    filenames_vlist ,   L"varying_suffix"         , fileinfo_p->varying_suffix        , true     ) 

    // --------------------------------------------------------------------------------------------------------------------------

    M_get_right_keyword_vlist(  expression      ,   L"data"                   , data_vlist                                   )

    M_get_nest_keyword_int64(   data_vlist      ,   L"file_size"              , fileinfo_p->file_min_size         , size_t   )  
    M_get_nest_keyword_int64(   data_vlist      ,   L"file_size"              , fileinfo_p->file_max_size         , size_t   ) 
    M_get_nest_keyword_int64(   data_vlist      ,   L"file_min_size"          , fileinfo_p->file_min_size         , size_t   )  
    M_get_nest_keyword_int64(   data_vlist      ,   L"file_max_size"          , fileinfo_p->file_max_size         , size_t   ) 

    M_get_nest_keyword_nval(    data_vlist      ,   L"hgt_layout"             , fileinfo_p->hgt_layout            , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"simple_dt2_layout"      , fileinfo_p->simple_dt2_layout     , true     )   
                               
    M_get_nest_keyword_nval(    data_vlist      ,   L"float32"                , fileinfo_p->float_32              , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"int16"                  , fileinfo_p->int_16                , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"big_endian"             , fileinfo_p->big_endian            , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"little_endian"          , fileinfo_p->little_endian         , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"signed_magnitude"       , fileinfo_p->signed_magnitude      , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"twos_compliment"        , fileinfo_p->twos_compliment       , true     )
                               
    M_get_nest_keyword_nval(    data_vlist      ,   L"pixels_aligned"         , fileinfo_p->pixels_aligned        , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"pixels_centered"        , fileinfo_p->pixels_centered       , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"extra_row_col"          , fileinfo_p->extra_row_col         , true     )
    M_get_nest_keyword_nval(    data_vlist      ,   L"no_extra_row_col"       , fileinfo_p->no_extra_row_col      , true     )

    M_get_nest_keyword_int64(   data_vlist      ,   L"extent"                 , fileinfo_p->extent_ew             , ll_T     )
    M_get_nest_keyword_int64(   data_vlist      ,   L"extent"                 , fileinfo_p->extent_ns             , ll_T     )
    M_get_nest_keyword_int64(   data_vlist      ,   L"extent_ew"              , fileinfo_p->extent_ew             , ll_T     )
    M_get_nest_keyword_int64(   data_vlist      ,   L"extent_ns"              , fileinfo_p->extent_ns             , ll_T     )

    M_get_nest_keyword_int64(   data_vlist      ,   L"ppd"                    , fileinfo_p->ppd_ew                , ll_T     )
    M_get_nest_keyword_int64(   data_vlist      ,   L"ppd"                    , fileinfo_p->ppd_ns                , ll_T     )
    M_get_nest_keyword_int64(   data_vlist      ,   L"ppd_ew"                 , fileinfo_p->ppd_ew                , ll_T     )
    M_get_nest_keyword_int64(   data_vlist      ,   L"ppd_ns"                 , fileinfo_p->ppd_ns                , ll_T     )


    // fill in suffixes vector

    if (fileinfo_p->varying_suffix)
    {
        // need to split up suffix string and add individual suffixes to the suffixes vector
        // ---------------------------------------------------------------------------------
    
        boost::algorithm::split(fileinfo_p->suffixes, suffix, boost::algorithm::is_any_of(L" "));   
        fileinfo_p->suffix_ct = (fileinfo_p->suffixes).size(); 
    }
    else        // 0 or 1 suffix
    {
        if (suffix.empty())
        {
            fileinfo_p->suffix_ct = 0;      // no suffixes
        }
        else                                // 1 suffix
        {
            (fileinfo_p->suffixes).push_back(suffix);
            fileinfo_p->suffix_ct = 1; 
        }
    }


    // extra parm checks that aren't supported in the verb definition
    // --------------------------------------------------------------

    int extra_error_ct {0}; 

    if (fileinfo_p->fn_many)
    {
         if ( (fileinfo_p->fn_sw_corner == false) && (fileinfo_p->fn_nw_corner == false) )
         {
             M_out_emsg1( L"mapverb_create_fileinfo() -- when «many_fn:» is specified, either «sw_corner:» or «nw_corner:» is required");
             msgend_loc(expression); 
             extra_error_ct++;     
         }
      
         if ( (fileinfo_p->fn_upper_case == false) && (fileinfo_p->fn_lower_case == false) )
         {
             M_out_emsg1( L"mapverb_create_fileinfo() -- when «many_fn:» is specified, either «upper_case:» or «lower_case:» is required");
             msgend_loc(expression); 
             extra_error_ct++;     
         }
    
         if ( (fileinfo_p->fn_lat_then_lon == false) && (fileinfo_p->fn_lon_then_lat == false) )
         {
             M_out_emsg1( L"mapverb_create_fileinfo() -- when «many_fn:» is specified, either «lat_then_lon:» or «lon_then_lat:» is required");
             msgend_loc(expression); 
             extra_error_ct++;     
         }  
    }

    if (extra_error_ct > 0)
        return -1; 


    // compute derived fields (and check for errors)
    // ---------------------------------------------

    ll_T rem_ns = (fileinfo_p->limit_n - fileinfo_p->limit_s) % fileinfo_p->step_ns;        
    ll_T rem_ew = (fileinfo_p->limit_e - fileinfo_p->limit_w) % fileinfo_p->step_ew;  

    if (rem_ns != 0)
    {    
        M_out_emsg1( L"mapverb_create_fileinfo() -- N-S coverage is not a whole multiple of step size -- south=%d (%.16f deg)  north=%d (%.16f deg)  step_ns=%d (%.16f deg)")
                   % fileinfo_p->limit_s % M_l_to_lr(fileinfo_p->limit_s) % fileinfo_p->limit_n % M_l_to_lr(fileinfo_p->limit_n) % fileinfo_p->step_ns % M_l_to_lr(fileinfo_p->step_ns); 
        msgend_loc(expression);  
        return -1;       
    }

    if (rem_ew != 0)
    {   
        M_out_emsg1( L"mapverb_create_fileinfo() -- E-W coverage is not a whole multiple of step size -- west=%d (%.16f deg)  east=%d (%.16f deg)  step_ew=%d (%.16f deg)")
                   % fileinfo_p->limit_w % M_l_to_lr(fileinfo_p->limit_w) % fileinfo_p->limit_e % M_l_to_lr(fileinfo_p->limit_e) % fileinfo_p->step_ew % M_l_to_lr(fileinfo_p->step_ew);
        msgend_loc(expression);  
        return -1;        
    }


    // save derived fields in fileinfo_S

    fileinfo_p->number_ns = (fileinfo_p->limit_n - fileinfo_p->limit_s) / fileinfo_p->step_ns;        
    fileinfo_p->number_ew = (fileinfo_p->limit_e - fileinfo_p->limit_w) / fileinfo_p->step_ew;     


    // display finished fileinfo_S, if requested

    if (display > 0)
        display_fileinfo(*fileinfo_p);
 
    return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_display_fileinfo() -- do @DISPLAY_FILEINFO    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_display_fileinfo(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;  
    
    M_get_fileinfo_n(  L"mapverb_display_fileinfo")
    M_check_fileinfo_n(L"DISPLAY_FILEINFO")            
               

    // display settings in located fileinfo_S 
    // --------------------------------------
   
    M_dv_some(M_out(L"=======> Doing   @DISPLAY_FILEINFO fileinfo_n:%d   <=======") % n_fileinfo;)


    M_out(L"\nmapverb_display_fileinfo(): -------- contents of fileinfo_n:%d ----------------------") % n_fileinfo;
    display_fileinfo(*fileinfo_p); 
    M_out(  L"-----------------------------------------------------------------------------\n");
    return 0; 
}
M_endf   



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_destroy_fileinfo() -- do @DESTROY_FILEINFO    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_destroy_fileinfo(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;  
    
    M_get_fileinfo_n(  L"mapverb_destroy_fileinfo")
    M_check_fileinfo_n(L"DESTROY_FILEINFO")            
               

    // deallocate fileinfo from repository -- fileinfo_S has nothing to clean up before deallocation 
    // ---------------------------------------------------------------------------------------------
   
    M_dv_few(M_out(L"=======> Doing   @DESTROY_FILEINFO tile_n:%d   <=======") % n_fileinfo;)

    free_fileinfo(n_fileinfo);            // get rid of this fileinfo_S in repository
    fileinfo_p = nullptr;                 // ?? not needed (yet) ?? 
    return 0; 
}
M_endf





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///          R A S T E R 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_create_fixed_raster() -- do @CREATE_FIXED_RASTER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_create_fixed_raster(const e_expression_S& expression) try
{
    int                  rc     { 0 };
    create_raster_parm_S rparm  {   };

    M_get_raster_n(L"mapverb_create_fixed_raster")


    // fill in create_raster() parms from options on the @CREATE_RASTER verb
    // ---------------------------------------------------------------------

    M_get_right_keyword_nval(   expression            , L"display"                  , rparm.display                  , true     ) 
    M_get_right_keyword_nval(   expression            , L"no_display"               , rparm.display                  , false    ) 
    M_get_right_keyword_nval(   expression            , L"extra_row_col"            , rparm.extra_row_col            , true     )
    M_get_right_keyword_nval(   expression            , L"no_extra_row_col"         , rparm.extra_row_col            , false    )

    M_get_right_keyword_int64(  expression            , L"south_edge"               , rparm.lat_s                    , lld_T    )
    M_get_right_keyword_int64(  expression            , L"west_edge"                , rparm.lon_w                    , lld_T    )
    M_get_right_keyword_int64(  expression            , L"ns_extent"                , rparm.deg_ns                   , lld_T    )
    M_get_right_keyword_int64(  expression            , L"ew_extent"                , rparm.deg_ew                   , lld_T    )
      
    M_get_right_keyword_int64(  expression            , L"window"                   , rparm.window_ns                , lld_T    )
    M_get_right_keyword_int64(  expression            , L"window"                   , rparm.window_ew                , lld_T    )
    M_get_right_keyword_int64(  expression            , L"window_ns"                , rparm.window_ns                , lld_T    )
    M_get_right_keyword_int64(  expression            , L"window_ew"                , rparm.window_ew                , lld_T    )

    M_get_right_keyword_int64(  expression            , L"ppd"                      , rparm.ppd_ns                   , int32_t  )
    M_get_right_keyword_int64(  expression            , L"ppd"                      , rparm.ppd_ew                   , int32_t  )  
    M_get_right_keyword_int64(  expression            , L"ppd_ns"                   , rparm.ppd_ns                   , int32_t  )
    M_get_right_keyword_int64(  expression            , L"ppd_ew"                   , rparm.ppd_ew                   , int32_t  )  


    // Destroy raster first, if already allocated (and initialized)
    // ------------------------------------------------------------

    if (raster_p != nullptr)
    {
       M_dv_few(M_out(L"=======> @CREATE_FIXED_RASTER raster_n:%d -- destroying old raster first <======") % n_raster;)

       rc = mapverb_destroy_raster(expression);  
       if (rc != 0) return rc;  

       raster_p = nullptr;                      // clear out local copy of raster_p after destroy_raster()
    }


    // allocate new raster_S and do initialization
    // -------------------------------------------

    M_dv_few(M_out(L"=======> Doing   @CREATE_FIXED_RASTER raster_n:%d   <=======") % n_raster;)

    raster_p = alloc_raster(n_raster);              // allocate new raster_S in repository
    rc = create_raster(*raster_p, rparm);           // set up the new raster_S

    if (rc != 0)
    {
        M_out_emsg(L"mapverb_create_fixed_raster(): @CREATE_FIXED_RASTER failed");
    }   

    return rc; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_create_auto_raster() -- do @CREATE_AUTO_RASTER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_create_auto_raster(const e_expression_S& expression) try
{
    int                  rc     { 0 };
    create_raster_parm_S rparm  {   };
    
    M_get_raster_n(  L"mapverb_create_auto_raster")
    M_get_map_n(     L"mapverb_create_auto_raster")
    M_check_map_n(   L"CREATE_AUTO_RASTER")
    
    
    // fill in create_auto_raster() parms from options on the @CREATE_AUTO_RASTER verb
    // -------------------------------------------------------------------------------
    
    M_get_right_keyword_nval(   expression            , L"display"                  , rparm.display                  , true     ) 
    M_get_right_keyword_nval(   expression            , L"no_display"               , rparm.display                  , false    ) 
    M_get_right_keyword_nval(   expression            , L"extra_row_col"            , rparm.extra_row_col            , true     )
    M_get_right_keyword_nval(   expression            , L"no_extra_row_col"         , rparm.extra_row_col            , false    )

    M_get_right_keyword_int64(  expression            , L"window"                   , rparm.window_ns                , lld_T    )
    M_get_right_keyword_int64(  expression            , L"window"                   , rparm.window_ew                , lld_T    )
    M_get_right_keyword_int64(  expression            , L"window_ns"                , rparm.window_ns                , lld_T    )
    M_get_right_keyword_int64(  expression            , L"window_ew"                , rparm.window_ew                , lld_T    )

    M_get_right_keyword_int64(  expression            , L"ppd"                      , rparm.ppd_ns                   , int32_t  )
    M_get_right_keyword_int64(  expression            , L"ppd"                      , rparm.ppd_ew                   , int32_t  )  
    M_get_right_keyword_int64(  expression            , L"ppd_ns"                   , rparm.ppd_ns                   , int32_t  )
    M_get_right_keyword_int64(  expression            , L"ppd_ew"                   , rparm.ppd_ew                   , int32_t  ) 
    
    
    //  Destroy raster first, if already allocated (and initialized)
    //  ------------------------------------------------------------
    
    if (raster_p != nullptr)
    {         
        M_dv_few(M_out(L"=======> @CREATE_AUTO_RASTER raster_n:%d map_n:%d  -- destroying old raster first <======") % n_raster % n_map;)
        
        rc = mapverb_destroy_raster(expression);  
    
        if (rc != 0) return rc; 
        raster_p = nullptr;                                // clear out local copy of raster_p, since destroy raster freed up the raster_S from the repository
    }
    
    
    //  create raster and mark as active
    //  --------------------------------
    
    M_dv_few(M_out(L"=======> Doing   @CREATE_AUTO_RASTER raster_n:%d map_n:%d   <=======") % n_raster % n_map;)
    
    raster_p = alloc_raster(n_raster);                    // allocate new raster_S in repository
    rc = create_auto_raster(*raster_p, rparm, *map_p);    // initialize new raster_S 

    if (rc != 0)
    {
        M_out_emsg(L"mapverb_create_auto_raster(): @CREATE_AUTO_RASTER failed");
    }  

    return rc; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_fill_whole_raster() -- do @FILL_WHOLE_RASTER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_fill_whole_raster(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;

    M_get_raster_n(     L"mapverb_fill_whole_raster" )
    M_get_fileinfo_n(   L"mapverb_fill_whole_raster" )
    M_checka_raster_n(  L"FILL_WHOLE_RASTER"         )
    M_check_fileinfo_n( L"FILL_WHOLE_RASTER"         )
  

    // set up fill_raster_parm_S from verb options
    // -------------------------------------------

    fill_raster_parm_S fparm { }; 

    M_get_right_keyword_nval(  expression            , L"display"                  , fparm.display                  , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_display"               , fparm.display                  , false    ) 
    M_get_right_keyword_nval(  expression            , L"display_filenames"        , fparm.display_filenames        , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_display_filenames"     , fparm.display_filenames        , false    ) 
    M_get_right_keyword_nval(  expression            , L"debug_tile"               , fparm.debug_tile               , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_debug_tile"            , fparm.debug_tile               , false    ) 
    M_get_right_keyword_nval(  expression            , L"interpolate"              , fparm.interpolate              , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_interpolate"           , fparm.interpolate              , false    ) 
    M_get_right_keyword_nval(  expression            , L"fill_s_to_n"              , fparm.s_to_n                   , true     )
    M_get_right_keyword_nval(  expression            , L"fill_n_to_s"              , fparm.n_to_s                   , true     )
    M_get_right_keyword_nval(  expression            , L"base_fill"                , fparm.base_fill                , true     )
    M_get_right_keyword_nval(  expression            , L"empty_fill"               , fparm.empty_fill               , true     )
    M_get_right_keyword_nval(  expression            , L"void_fill"                , fparm.void_fill                , true     )


    //  Fill whole raster
    //  -----------------

    M_dv_few(M_out(L"=======> Doing   @FILL_WHOLE_RASTER raster_n:%d fileinfo_n:%d <=======") % n_raster % n_fileinfo;)

    fill_raster(*raster_p, fparm, *fileinfo_p);
    raster_p->filled  = true;                                    // indicate that raster is filled (enough data for maps)
    raster_p->complete = true;                                   // indicate that raster is completely filled (enough data for writeout_raster())
    return 0; 
}
M_endf
 


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_selective_fill_raster() -- do @SELECTIVE_FILL_RASTER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_selective_fill_raster(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;

    M_get_raster_n(      L"mapverb_selective_fill_raster" )
    M_get_map_n(         L"mapverb_selective_fill_raster" )
    M_get_fileinfo_n(    L"mapverb_selective_fill_raster" )
    M_checka_raster_n(   L"SELECTIVE_FILL_RASTER"         )
    M_check_map_n(       L"SELECTIVE_FILL_RASTER"         )
    M_check_fileinfo_n(  L"SELECTIVE_FILL_RASTER"         )   
    

    // set up fill_raster_parm_S from verb options
    // -------------------------------------------

    fill_raster_parm_S fparm { }; 

    M_get_right_keyword_nval(  expression            , L"display"                  , fparm.display                  , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_display"               , fparm.display                  , false    ) 
    M_get_right_keyword_nval(  expression            , L"display_filenames"        , fparm.display_filenames        , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_display_filenames"     , fparm.display_filenames        , false    ) 
    M_get_right_keyword_nval(  expression            , L"debug_tile"               , fparm.debug_tile               , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_debug_tile"            , fparm.debug_tile               , false    ) 
    M_get_right_keyword_nval(  expression            , L"interpolate"              , fparm.interpolate              , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_interpolate"           , fparm.interpolate              , false    ) 
    M_get_right_keyword_nval(  expression            , L"fill_s_to_n"              , fparm.s_to_n                   , true     )
    M_get_right_keyword_nval(  expression            , L"fill_n_to_s"              , fparm.n_to_s                   , true     )
    M_get_right_keyword_nval(  expression            , L"base_fill"                , fparm.base_fill                , true     )
    M_get_right_keyword_nval(  expression            , L"empty_fill"               , fparm.empty_fill               , true     )
    M_get_right_keyword_nval(  expression            , L"void_fill"                , fparm.void_fill                , true     )

    
    //  create raster and mark as active
    //  --------------------------------

    M_dv_few(M_out(L"=======> Doing   @SELECTIVE_FILL_RASTER raster_n:%d map_n:%d fileinfo_n:%d <=======") % n_raster % n_map % n_fileinfo;)

    selective_fill_raster(*raster_p, fparm, *map_p, *fileinfo_p); 
    raster_p->filled = true;                                              // indicate that raster is filled (enough data for maps)
    return 0; 
}
M_endf


 ////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_smooth_raster() -- do @SMOOTH_RASTER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_smooth_raster(const e_expression_S& expression) try
{
    int         rc { 0 } ;  
    
    M_get_raster_n(  L"mapverb_smooth_raster")
    M_check_raster_n(L"SMOOTH_RASTER")            
    


    // extract passes: option
    // ----------------------

    int32_t passes { 0 };

    M_get_right_keyword_int64(expression, L"passes", passes, int32_t);

                          
    // smooth out data in raster_S
    // ---------------------------
   
    M_dv_few(M_out(L"=======> Doing   @SMOOTH_RASTER raster_n:%d  passes:%d <=======") % n_raster % passes;)

    smooth_raster(*raster_p, passes);    
    return 0; 
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_display_raster() -- do @DISPLAY_RASTER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_display_raster(const e_expression_S& expression) try
{
    int         rc { 0 } ;  
    
    M_get_raster_n(   L"mapverb_display_raster")
    M_checka_raster_n(L"DISPLAY_RASTER")            
               

    // display raster and imbedded grid 
    // --------------------------------
   
    M_dv_few(M_out(L"=======> Doing   @DISPLAY_RASTER raster_n:%d   <=======") % n_raster;)

    display_raster(*raster_p); 
    return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_writeout_raster() -- do @WRITEOUT_RASTER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_writeout_raster(const e_expression_S& expression) try
{
    int         rc { 0 } ;

    M_get_raster_n(     L"mapverb_writeout_raster" )
    M_get_fileinfo_n(   L"mapverb_writeout_raster" )
    M_check_raster_n(   L"WRITEOUT_RASTER"         )
    M_check_fileinfo_n( L"WRIETOUT_RASTER"         )
  

    // set up writeout_raster_parm_S from verb options
    // -----------------------------------------------

    writeout_raster_parm_S wparm { }; 

    M_get_right_keyword_nval(  expression            , L"suppress_empty"           , wparm.suppress_empty           , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_suppress_empty"        , wparm.suppress_empty           , false    ) 
    M_get_right_keyword_nval(  expression            , L"set_empty_height"         , wparm.set_empty_height         , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_set_empty_height"      , wparm.set_empty_height         , false    ) 
    M_get_right_keyword_int64( expression            , L"empty_height"             , wparm.empty_height             , ht_T     )

    M_get_right_keyword_nval(  expression            , L"display"                  , wparm.display                  , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_display"               , wparm.display                  , false    ) 
    M_get_right_keyword_nval(  expression            , L"display_filenames"        , wparm.display_filenames        , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_display_filenames"     , wparm.display_filenames        , false    ) 
    M_get_right_keyword_nval(  expression            , L"debug_tile"               , wparm.debug_tile               , true     ) 
    M_get_right_keyword_nval(  expression            , L"no_debug_tile"            , wparm.debug_tile               , false    ) 
     

    //  Write out whole raster
    //  ----------------------

    M_dv_few(M_out(L"=======> Doing   @WRITEOUT_RASTER raster_n:%d fileinfo_n:%d <=======") % n_raster % n_fileinfo;)

    rc = writeout_raster(*raster_p, wparm, *fileinfo_p);  

    if (rc != 0)
    {
        M_out_emsg(L"mapverb_writeout_raster(): @WRITEOUT_RASTER failed");
    }  

    return rc; 
}
M_endf 



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_destroy_raster() -- do @DESTROY_RASTER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_destroy_raster(const e_expression_S& expression) try
{
    int         rc { 0 } ;  
    
    M_get_raster_n(   L"mapverb_destroy_raster")
    M_checka_raster_n(L"DESTROY_RASTER")            
               

    // destroy raster and deallocate from repository
    // ---------------------------------------------
   
    M_dv_few(M_out(L"=======> Doing   @DESTROY_RASTER raster_n:%d   <=======") % n_raster;)

    destroy_raster(*raster_p);    
    free_raster(n_raster);                // get rid of this raster in repository
    raster_p = nullptr;                   // ?? not needed (yet) ?? 
    return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///          R A M P 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_ramp() -- build color ramp based on vlist parms  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_ramp(const e_expression_S& expression, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_ramp() called");)     
    M_get_ramp_n(L"mapverb_ramp")                        //   sets n_ramp and ramp_p (possibly to nullptr if ramp(n) is not already allocated in the repository)     
 
    M_dv_few(M_out(L"=======> Doing   @RAMP ramp_n:%d   <=======") % n_ramp;)
       

    // Make sure ramp(n) is allocated in repository.   Clear out all current ramp info, if "clear" keyword is present (even if just allocated)
    // ---------------------------------------------------------------------------------------------------------------------------------------

    ramp_p = alloc_ramp(n_ramp);                         // ramp_p may be same as returned from M_get_ramp_n (if already allocated) or point to newly-allocated ramp_S in repository 

    if (expression.rparms.eval_kws.count(L"clear") > 0)
        *ramp_p = ramp_S{};                              // resets the "complete" flag, too
   

    // Fill in  / append to land LRGB vector in color ramp based on land_ht: and land_color: parms
    // -------------------------------------------------------------------------------------------

    lrgb_S lrgb { };

    auto ht_ct_land    = expression.rparms.eval_kws.count(L"land_ht"      );
    auto color_ct_land = expression.rparms.eval_kws.count(L"land_color"   );
    auto ht_it_land    = expression.rparms.eval_kws.find( L"land_ht"      );
    auto color_it_land = expression.rparms.eval_kws.find( L"land_color"   );

    M__(M_out(L"mapverb_ramp() -- ramp_number=%d  ht_ct_land=%d  color_ct_land=%d") % n_ramp % ht_ct_land % color_ct_land;) 


    for (auto i = 0; i < ht_ct_land; i++)             // i used only in debug message, below
    {
        lrgb.l = (ht_T)(ht_it_land->second.int64);

        vlist_S color_vlist = *(color_it_land->second.vlist_sp); 

        lrgb.r = (uint8_t)(color_vlist.values.at(0).int64);
        lrgb.g = (uint8_t)(color_vlist.values.at(1).int64);
        lrgb.b = (uint8_t)(color_vlist.values.at(2).int64);

        M__(M_out(L"mapverb_ramp() -- land LRGB[%d] -- l=%-6d r=%-3d  g=%-3d  b=%-3d") % i % lrgb.l % lrgb.r % lrgb.g % lrgb.b;)

        (*ramp_p).lrgb_land.push_back(lrgb);          // add lrgb value to lrgb vector in ramp_S

        ht_it_land++;                                 // advance height iterator
        color_it_land++;                              // advance color iterator
    }


    // Fill in / append to ocean LRGB vector in color ramp based on ocean_ht: and ocean_color: parms
    // ----------------------------------------------------------------------------------------------

    auto dp_ct_ocean    = expression.rparms.eval_kws.count(L"ocean_dp"      );
    auto color_ct_ocean = expression.rparms.eval_kws.count(L"ocean_color"   );
    auto dp_it_ocean    = expression.rparms.eval_kws.find( L"ocean_dp"      );
    auto color_it_ocean = expression.rparms.eval_kws.find( L"ocean_color"   );

    M__(M_out(L"mapverb_ramp() -- ramp_number=%d  dp_ct_ocean=%d  color_ct_ocean=%d") % n_ramp % dp_ct_ocean % color_ct_ocean;) 


    for (auto i = 0; i < dp_ct_ocean; i++)             // i used only in debug message, below
    {
        lrgb.l = (ht_T)(dp_it_ocean->second.int64);

        vlist_S color_vlist = *(color_it_ocean->second.vlist_sp); 

        lrgb.r = (uint8_t)(color_vlist.values.at(0).int64);
        lrgb.g = (uint8_t)(color_vlist.values.at(1).int64);
        lrgb.b = (uint8_t)(color_vlist.values.at(2).int64);

        M__(M_out(L"mapverb_ramp() -- ocean LRGB[%d] -- l=%-6d r=%-3d  g=%-3d  b=%-3d") % i % lrgb.l % lrgb.r % lrgb.g % lrgb.b;)

        (*ramp_p).lrgb_ocean.push_back(lrgb);         // add lrgb value to lrgb vector in ramp_S

        dp_it_ocean++;                                // advance height iterator
        color_it_ocean++;                             // advance color iterator
    }


    // --------------------------------------------------
    // finish up the ramp, if "finish" keyword is present
    // --------------------------------------------------

    if (expression.rparms.eval_kws.count(L"finish") > 0)
    {   
        // capture ramp finishing options
        // ------------------------------

        M_get_right_keyword_int64(    expression, L"display"                    ,ramp_p->parm.display        , int32_t  )
        M_get_right_keyword_int64(    expression, L"land_rescale"               ,ramp_p->parm.rescale_land   , bool     )
        M_get_right_keyword_int64(    expression, L"ocean_rescale"              ,ramp_p->parm.rescale_ocean  , bool     )


        // capture ramp verb parms into ramp being finished    
        // ------------------------------------------------

        M_get_right_keyword_int64(   expression, L"land_rescale_height"         , ramp_p->parm.rescale_ht_land        , int32_t)
        M_get_right_keyword_int64(   expression, L"ocean_rescale_depth"         , ramp_p->parm.rescale_dp_ocean       , int32_t)
        M_get_right_keyword_int64(   expression, L"lowest_land_rescale"         , ramp_p->parm.rescale_low_land       , int32_t)
        M_get_right_keyword_int64(   expression, L"shallowest_ocean_rescale"    , ramp_p->parm.rescale_shallow_ocean  , int32_t)
        M_get_right_keyword_int64(   expression, L"land_rescale_base"           , ramp_p->parm.rescale_base_land      , int32_t)
        M_get_right_keyword_float64( expression, L"land_rescale_gamma"          , ramp_p->parm.rescale_gamma_land     , real_T )
        M_get_right_keyword_float64( expression, L"ocean_rescale_gamma"         , ramp_p->parm.rescale_gamma_ocean    , real_T )
        M_get_right_keyword_float64( expression, L"land_rescale_step"           , ramp_p->parm.rescale_step_land      , real_T )
        M_get_right_keyword_float64( expression, L"ocean_rescale_step"          , ramp_p->parm.rescale_step_ocean     , real_T )
        M_get_right_keyword_float64( expression, L"land_pastel_factor"          , ramp_p->parm.pastel_factor_land     , real_T )
        M_get_right_keyword_float64( expression, L"ocean_pastel_factor"         , ramp_p->parm.pastel_factor_ocean    , real_T )

        M_get_right_keyword_float64( expression, L"land_shade_darkest"          , ramp_p->parm.shade_darkest_land     , real_T )
        M_get_right_keyword_float64( expression, L"land_shade_lightest"         , ramp_p->parm.shade_lightest_land    , real_T )
        M_get_right_keyword_float64( expression, L"land_shade_midtone"          , ramp_p->parm.shade_midtone_land     , real_T )
        M_get_right_keyword_float64( expression, L"land_dark_angle"             , ramp_p->parm.dark_angle_land        , real_T )
        M_get_right_keyword_float64( expression, L"land_bright_angle"           , ramp_p->parm.bright_angle_land      , real_T )
        M_get_right_keyword_float64( expression, L"ocean_shade_darkest"         , ramp_p->parm.shade_darkest_ocean    , real_T )
        M_get_right_keyword_float64( expression, L"ocean_shade_lightest"        , ramp_p->parm.shade_lightest_ocean   , real_T )
        M_get_right_keyword_float64( expression, L"ocean_shade_midtone"         , ramp_p->parm.shade_midtone_ocean    , real_T )
        M_get_right_keyword_float64( expression, L"ocean_dark_angle"            , ramp_p->parm.dark_angle_ocean       , real_T )
        M_get_right_keyword_float64( expression, L"ocean_bright_angle"          , ramp_p->parm.bright_angle_ocean     , real_T )


        // call setup_ramp() to finish setting up the ramp
        // -----------------------------------------------

        auto rc = setup_ramp(*ramp_p, parm);   // do the actual set_ramp(); 

        if (rc != 0) 
            return 1;  
   
        ramp_p->complete = true;               // indicate that this ramp is complete/valid now  
    }
      
    return 0; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_draw_ramp() -- draw color ramp into outimage area (use GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_draw_ramp(const e_expression_S& expression, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_draw_ramp() called");) 
    
    M_get_ramp_n(    L"mapverb_draw_ramp")
    M_get_outimage_n(L"mapverb_draw_ramp")
    M_check_ramp_n(    L"DRAW_RAMP")
    M_check_outimage_n(L"DRAW_RAMP")
    
    M_dv_few(M_out(L"=======> Doing   @DRAW_RAMP ramp_n:%d outimage_n:%d   <=======") % n_ramp % n_outimage;)
    

    // get ramp drawing parms from options
    // -----------------------------------

    ramp_put_parm_S rparm {};      // set default options

    M_get_right_keyword_int64(    expression, L"display"          ,rparm.display        , bool     )
    M_get_right_keyword_nval(     expression, L"ocean"           , rparm.ocean          , true     )
    M_get_right_keyword_int64(    expression, L"top_row"         , rparm.top_row        , int32_t  )
    M_get_right_keyword_int64(    expression, L"left_col"        , rparm.left_col       , int32_t  )
    M_get_right_keyword_int64(    expression, L"width"           , rparm.width          , int32_t  )
    M_get_right_keyword_int64(    expression, L"height"          , rparm.height         , int32_t  )
    M_get_right_keyword_int64(    expression, L"border"          , rparm.border         , int32_t  )
    M_get_right_keyword_int64(    expression, L"margin"          , rparm.margin         , int32_t  )

    if (expression.rparms.eval_kws.count(L"background_color"  ) > 0)       mapverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"background_color"   )->second.vlist_sp), rparm.background_color);
    if (expression.rparms.eval_kws.count(L"text_color"        ) > 0)       mapverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"text_color"         )->second.vlist_sp), rparm.text_color      );
    if (expression.rparms.eval_kws.count(L"border_color"      ) > 0)       mapverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"border_color"       )->second.vlist_sp), rparm.border_color    );


    // call draw_ramp() to draw the ramp directly in the outimage structure
    // --------------------------------------------------------------------

    auto rc = draw_ramp(*outimage_p, *ramp_p, rparm, parm);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


 ////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_put_ramp() -- draw color ramp directly in outimage area (bypass GDI+)  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_put_ramp(const e_expression_S& expression, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_put_ramp() called");) 
    
    M_get_ramp_n(    L"mapverb_put_ramp")
    M_get_outimage_n(L"mapverb_put_ramp")
    M_check_ramp_n(    L"PUT_RAMP")
    M_check_outimage_n(L"PUT_RAMP")
    
    M_dv_some(M_out(L"=======> Doing   @PUT_RAMP ramp_n:%d outimage_n:%d   <=======") % n_ramp % n_outimage;)
         

    // get ramp drawing parms from options
    // -----------------------------------

    ramp_put_parm_S rparm {};      // set default options

    M_get_right_keyword_int64(    expression, L"display"          ,rparm.display        , bool     )
    M_get_right_keyword_nval(     expression, L"ocean"           , rparm.ocean          , true     )
    M_get_right_keyword_int64(    expression, L"top_row"         , rparm.top_row        , int32_t  )
    M_get_right_keyword_int64(    expression, L"left_col"        , rparm.left_col       , int32_t  )
    M_get_right_keyword_int64(    expression, L"width"           , rparm.width          , int32_t  )
    M_get_right_keyword_int64(    expression, L"height"          , rparm.height         , int32_t  )
    M_get_right_keyword_int64(    expression, L"border"          , rparm.border         , int32_t  )
    M_get_right_keyword_int64(    expression, L"margin"          , rparm.margin         , int32_t  )

    if (expression.rparms.eval_kws.count(L"background_color"  ) > 0)       mapverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"background_color"   )->second.vlist_sp), rparm.background_color);
    if (expression.rparms.eval_kws.count(L"text_color"        ) > 0)       mapverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"text_color"         )->second.vlist_sp), rparm.text_color      );
    if (expression.rparms.eval_kws.count(L"border_color"      ) > 0)       mapverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"border_color"       )->second.vlist_sp), rparm.border_color    );


    // call put_ramp() to draw the ramp directly into the outimage structure
    // ---------------------------------------------------------------------

    auto rc = put_ramp(*outimage_p, *ramp_p, rparm, parm);    

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_destroy_ramp() -- do DESTROY_RAMP    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_destroy_ramp(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;  
    
    M_get_ramp_n(L"mapverb_destroy_ramp")
                 

    // complain if ramp(n) is not allocated -- doesn't need to be initialized to be destroyed, so m_check_ramp_n() MACRO can't be used here
    // ------------------------------------------------------------------------------------------------------------------------------------

    if (ramp_p == nullptr)                                                                                  
    {                                                                                                       
         M_out_emsg(L"=======> @DESTROY_RAMP -- ramp %d is not allocated in repository <======") % n_ramp;
         return 1;                                                                                          
    }   


    // get rid or this ramp from the repository
    // ----------------------------------------
   
    M_dv_few(M_out(L"=======> Doing  @DESTROY_RAMP ramp_n:%d   <=======") % n_ramp;)
                                                                                        
    free_ramp(n_ramp);                // just rempve it from repositoary -- no ramp-specific cleanup needed  
    return 0; 
}
M_endf    




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///          M A P 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_create_map() -- do CREATE_MAP(n1)    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_create_map(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;

    M_get_map_n(L"mapverb_create_map")                    // sets n_map and map_p
  

    // fill in mparm structure based on supplied verb parms
    // ----------------------------------------------------

    create_map_parm_S mparm {};                           // set default values into parm structure

    M_get_right_keyword_int64(    expression, L"display"             , mparm.display           , int32_t    )
    M_get_right_keyword_float64(  expression, L"north_limit"         , mparm.north_limit       , llr_T      )
    M_get_right_keyword_float64(  expression, L"south_limit"         , mparm.south_limit       , llr_T      )
    M_get_right_keyword_float64(  expression, L"east_limit"          , mparm.east_limit        , llr_T      )
    M_get_right_keyword_float64(  expression, L"west_limit"          , mparm.west_limit        , llr_T      )
    M_get_right_keyword_int64(    expression, L"samples"             , mparm.samples           , int32_t    )
    M_get_right_keyword_float64(  expression, L"padding"             , mparm.padding           , llr_T      )

    if (expression.rparms.eval_kws.count(L"rectangular") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"rectangular")->second.vlist_sp);

        mparm.projection = projection_E::rectangular;

        M_get_nest_keyword_int64(   vlist, L"center_ppd"       , mparm.center_ppd        , int32_t ) 
        M_get_nest_keyword_float64( vlist, L"deg_ew"           , mparm.rect.deg_ew       , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_ns"           , mparm.rect.deg_ns       , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"ns_factor"        , mparm.rect.ns_factor    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"south"            , mparm.rect.south        , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"west"             , mparm.rect.west         , real_T  ) 
          
    } 
    
    if (expression.rparms.eval_kws.count(L"mercator") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"mercator")->second.vlist_sp);

        mparm.projection = projection_E::mercator;

        M_get_nest_keyword_int64(   vlist, L"center_ppd"       , mparm.center_ppd        , int32_t ) 
        M_get_nest_keyword_float64( vlist, L"deg_ew"           , mparm.merc.deg_ew       , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_ns"           , mparm.merc.deg_ns       , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"south"            , mparm.merc.south        , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"west"             , mparm.merc.west         , real_T  )             
    }  

    if (expression.rparms.eval_kws.count(L"polar") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"polar")->second.vlist_sp);

        mparm.projection = projection_E::polar;

        M_get_nest_keyword_int64(   vlist, L"center_ppd"       , mparm.center_ppd        , int32_t ) 
        M_get_nest_keyword_float64( vlist, L"deg_col0"         , mparm.polar.deg_col0    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_cols"         , mparm.polar.deg_cols    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_row0"         , mparm.polar.deg_row0    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_rows"         , mparm.polar.deg_rows    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lat_adj"          , mparm.polar.lat_adj     , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lon0"             , mparm.polar.lon0        , real_T  ) 
    }

    if (expression.rparms.eval_kws.count(L"azimuthal") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"azimuthal")->second.vlist_sp);

        mparm.projection = projection_E::azimuthal;

        M_get_nest_keyword_int64(   vlist, L"center_ppd"       , mparm.center_ppd        , int32_t ) 
        M_get_nest_keyword_int64(   vlist, L"cols"             , mparm.azim.cols         , int32_t ) 
        M_get_nest_keyword_float64( vlist, L"deg_cols"         , mparm.azim.deg_cols     , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_rows"         , mparm.azim.deg_rows     , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lat0"             , mparm.azim.lat0         , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lon0"             , mparm.azim.lon0         , real_T  ) 
        M_get_nest_keyword_int64(   vlist, L"rows"             , mparm.azim.rows         , int32_t )  
    }

    if (expression.rparms.eval_kws.count(L"azimuthal_equal_area") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"azimuthal_equal_area")->second.vlist_sp);

        mparm.projection = projection_E::azimuthal;

        M_get_nest_keyword_int64(   vlist, L"center_ppd"       , mparm.center_ppd        , int32_t ) 
        M_get_nest_keyword_int64(   vlist, L"cols"             , mparm.az_ea.cols        , int32_t ) 
        M_get_nest_keyword_float64( vlist, L"deg_cols"         , mparm.az_ea.deg_cols    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_rows"         , mparm.az_ea.deg_rows    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lat0"             , mparm.az_ea.lat0        , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lon0"             , mparm.az_ea.lon0        , real_T  ) 
        M_get_nest_keyword_int64(   vlist, L"rows"             , mparm.az_ea.rows        , int32_t )  
    }

    if (expression.rparms.eval_kws.count(L"stereographic") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"stereographic")->second.vlist_sp);

        mparm.projection = projection_E::stereographic;

        M_get_nest_keyword_int64(   vlist, L"center_ppd"       , mparm.center_ppd        , int32_t ) 
        M_get_nest_keyword_int64(   vlist, L"cols"             , mparm.stereo.cols       , int32_t ) 
        M_get_nest_keyword_float64( vlist, L"deg_cols"         , mparm.stereo.deg_cols   , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_rows"         , mparm.stereo.deg_rows   , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lat0"             , mparm.stereo.lat0       , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lon0"             , mparm.stereo.lon0       , real_T  )  
        M_get_nest_keyword_int64(   vlist, L"rows"             , mparm.stereo.rows       , int32_t )  
    }
    
    if (expression.rparms.eval_kws.count(L"orthographic") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"orthographic")->second.vlist_sp);

        mparm.projection = projection_E::orthographic;

        M_get_nest_keyword_int64(   vlist, L"center_ppd"       , mparm.center_ppd        , int32_t ) 
        M_get_nest_keyword_int64(   vlist, L"cols"             , mparm.ortho.cols        , int32_t ) 
        M_get_nest_keyword_float64( vlist, L"deg_cols"         , mparm.ortho.deg_cols    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"deg_rows"         , mparm.ortho.deg_rows    , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lat0"             , mparm.ortho.lat0        , real_T  ) 
        M_get_nest_keyword_float64( vlist, L"lon0"             , mparm.ortho.lon0        , real_T  ) 
        M_get_nest_keyword_int64(   vlist, L"rows"             , mparm.ortho.rows        , int32_t )  
    }
     




    // Destroy map first, if already active
    // ------------------------------------

    if (map_p != nullptr)
    {
        M_dv_few(M_out(L"=======> @CREATE_MAP map_n:%d -- destroying old map first <======") % n_map;)

        rc = mapverb_destroy_map(expression, parm);         

        if (rc != 0) return rc; 
        map_p = nullptr;                      // clear out local copy of map_p after destroy_map()
    }


    // create map and mark as active
    // -----------------------------

    M_dv_few(M_out(L"=======> Doing   @CREATE_MAP map_n:%d   <=======") % n_map;)

    map_p = alloc_map(n_map);                  // allocate map(n) in map repository
    rc    = create_map(*map_p, mparm, parm);   // do the create_map(); 

    if (rc != 0) 
        return 1;  

    return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_fill_map() -- do ADD_DEM    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_fill_map(const e_expression_S& expression, mapparm_S& parm) try
{
    M_get_raster_n(   L"mapverb_fill_map")
    M_get_map_n(      L"mapverb_fill_map")
    M_check_raster_n( L"ADD_DEM")
    M_check_map_n(    L"ADD_DEM")
    

    // set parms for fill_map() based on @ADD_DEM verb parms
    // -----------------------------------------------------

    fill_map_parm_S fparm {};                   // set up default fill_map() parms 

    M_get_right_keyword_int64(    expression, L"display"             , fparm.display           , int32_t    )

    M_get_right_keyword_int64(    expression, L"sampling_method"     , fparm.sampling_method   , int32_t    )
    M_get_right_keyword_nval(     expression, L"ocean_fill"          , fparm.ocean_fill        , true       )
    M_get_right_keyword_nval(     expression, L"ocean_fill"          , fparm.normal_fill       , false      )
    M_get_right_keyword_nval(     expression, L"normal_fill"         , fparm.normal_fill       , true       )
    M_get_right_keyword_nval(     expression, L"normal_fill"         , fparm.ocean_fill        , false      )


    //  add DEM data to map from raster
    //  -------------------------------

    M_dv_few(M_out(L"=======> Doing   @ADD_DEM map_n:%d raster_n:%d   <=======") % n_map % n_raster;)

    fill_map(*map_p, *raster_p, fparm, parm);     
   	return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_graticule_map() -- do @ADD_GRATICULE    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_graticule_map(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;

    M_get_map_n(     L"mapverb_graticule_map")
    M_check_map_n(   L"ADD_GRATICULE")

    graticule_map_parm_S gparm {} ;      // set up default values

    M_get_right_keyword_int64( expression, L"display"             , gparm.display    , bool    )    
    M_get_right_keyword_int64( expression, L"ew_spacing"          , gparm.ew_spacing , int32_t )
    M_get_right_keyword_int64( expression, L"ns_spacing"          , gparm.ns_spacing , int32_t ) 
    M_get_right_keyword_int64( expression, L"weight"              , gparm.weight     , int32_t ) 

    if (expression.rparms.eval_kws.count(L"ew_reduce") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"ew_reduce")->second.vlist_sp);

        M_get_nest_keyword_int64( vlist, L"factor_1"          , gparm.reduce_1     , int32_t ) 
        M_get_nest_keyword_int64( vlist, L"factor_2"          , gparm.reduce_2     , int32_t ) 
        M_get_nest_keyword_int64( vlist, L"factor_3"          , gparm.reduce_3     , int32_t ) 
        M_get_nest_keyword_int64( vlist, L"latitude_1"        , gparm.hi_lat_1     , int32_t ) 
        M_get_nest_keyword_int64( vlist, L"latitude_2"        , gparm.hi_lat_2     , int32_t ) 
        M_get_nest_keyword_int64( vlist, L"latitude_3"        , gparm.hi_lat_3     , int32_t )        
    }


    //  Add graticule data to map_S
    //  ---------------------------

    M_dv_few(M_out(L"=======> Doing   @ADD_GRATICULE map_n:%d   <=======") % n_map;)

    graticule_map(*map_p, gparm, parm); 
    return rc; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_water_map() -- do DETECT_WATER    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_water_map(const e_expression_S& expression, mapparm_S& parm) try
{
    M_get_map_n(     L"mapverb_water_map")
    M_check_map_n(   L"DETECT_WATER")
        

    //  Detect water
    //  ------------

    M_dv_few(M_out(L"=======> Doing   @DETECT_WATER map_n:%d   <=======") % n_map;)

    water_map(*map_p, parm);     
    return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_slope_map() -- do ADD_SLOPES    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_slope_map(const e_expression_S& expression, mapparm_S& parm) try
{
    // get parms from @ADD_SLOPES verb

    slope_map_parm_S sparm {};                  // set up defaults for slope_map()

    M_get_map_n(     L"mapverb_slope_map")
    M_check_map_n(   L"ADD_SLOPES")
   
    M_get_right_keyword_int64(   expression, L"display"           , sparm.display             , bool               )
    M_get_right_keyword_int64(   expression, L"land"              , sparm.do_land             , bool               )
    M_get_right_keyword_int64(   expression, L"ocean"             , sparm.do_ocean            , bool               )
    M_get_right_keyword_float64( expression, L"land_vert_factor"  , sparm.land_vert_factor    , real_T             )
    M_get_right_keyword_float64( expression, L"ocean_vert_factor" , sparm.ocean_vert_factor   , real_T             )
    M_get_right_keyword_nval(    expression, L"light_n"           , sparm.light_dir           , light_dir_E::n     )
    M_get_right_keyword_nval(    expression, L"light_ne"          , sparm.light_dir           , light_dir_E::ne    )
    M_get_right_keyword_nval(    expression, L"light_e"           , sparm.light_dir           , light_dir_E::e     )
    M_get_right_keyword_nval(    expression, L"light_se"          , sparm.light_dir           , light_dir_E::se    )
    M_get_right_keyword_nval(    expression, L"light_s"           , sparm.light_dir           , light_dir_E::s     )
    M_get_right_keyword_nval(    expression, L"light_sw"          , sparm.light_dir           , light_dir_E::sw    )
    M_get_right_keyword_nval(    expression, L"light_w"           , sparm.light_dir           , light_dir_E::w     )
    M_get_right_keyword_nval(    expression, L"light_nw"          , sparm.light_dir           , light_dir_E::nw    )
    M_get_right_keyword_nval(    expression, L"light_above"       , sparm.light_dir           , light_dir_E::above )


    //  Do slope computations
    //  ---------------------

    M_dv_few(M_out(L"=======> Doing   @ADD_SLOPES map_n:%d   <=======") % n_map;)

    slope_map(*map_p, sparm, parm);       
    return 0; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_extract_map() 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


int32_t mapverb_extract_map(const e_expression_S& expression, value_S& value, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_extract_map() called");) 
   
    M_get_map_n(L"mapverb_extract_map")
    M_check_map_n(L"EXTRACT_MAP")

    M_dv_some(M_out(L"=======> Doing   @EXTRACT_MAP map_n:%d   <=======") % n_map;)
                               

    // return requested map info  (only one of these keywords can be present)
    // -------------------------

    if (expression.rparms.eval_kws.count(L"cols"               ) > 0) value = set_type_value( (int64_t)(map_p->cols    ) );
    if (expression.rparms.eval_kws.count(L"rows"               ) > 0) value = set_type_value( (int64_t)(map_p->rows    ) );
    if (expression.rparms.eval_kws.count(L"maxcol"             ) > 0) value = set_type_value(           map_p->maxcol    );
    if (expression.rparms.eval_kws.count(L"maxrow"             ) > 0) value = set_type_value(           map_p->maxrow    );
    if (expression.rparms.eval_kws.count(L"npts"               ) > 0) value = set_type_value( (int64_t)(map_p->npts    ) );
                                                                                    
    if (expression.rparms.eval_kws.count(L"mapname_suffix"     ) > 0) 
    {
        // convert map dimensions and PPD to mapname suffix string --example -- _N56°00'00''_W005°00'00''_NSxEW=013°00'00''x010°00'00''_1800ppd 

        std::wstring mapname_suffix {}; 

        mapname_suffix += ( std::wstring{L"_"      } + deg_to_d_m_s( map_p->ref_latitude  , 1) );
        mapname_suffix += ( std::wstring{L"_"      } + deg_to_d_m_s( map_p->ref_longitude , 2) );
        mapname_suffix += ( std::wstring{L"_NSxEW="} + deg_to_d_m_s( map_p->ref_extent_ns , 0) );
        mapname_suffix += ( std::wstring{L"x"      } + deg_to_d_m_s( map_p->ref_extent_ew , 0) );
       
        mapname_suffix += ( std::wstring{L"_"      } + std::to_wstring((int)M_round(map_p->ref_ppd)) + std::wstring{L"ppd"} );
        value = set_type_value(mapname_suffix);
     }
    
    return 0; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_destroy_map() -- do DESTROY_MAP    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_destroy_map(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;

    M_get_map_n(     L"mapverb_destroy_map")    // sets n_map and map_p
    M_check_map_n(   L"DESTROY_MAP")
    

    // destroy map and mark as inactive
    // --------------------------------
   
    M_dv_few(M_out(L"=======> Doing   @DESTROY_MAP map_n:%d   <=======") % n_map;)

    destroy_map(*map_p, parm); 
    free_map(n_map);               // get rid of map(n) in map repository
    map_p = nullptr;               // ?? not needed (yet) ??   
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///          O U T I M A G E 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_create_map_outimage() -- do CREATE_MAP_OUTIMAGE    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_create_map_outimage(const e_expression_S& expression, mapparm_S& parm) try
{
    int         rc { 0 } ;
       
    M_get_outimage_n(  L"mapverb_create_map_outimage")      // sets n_outimage and outimage_p -- used below
    M_get_map_n(       L"mapverb_create_map_outimage")
    M_check_map_n(     L"CREATE_MAP_OUTIMAGE"        )



    // Get outimage create parms from keyword options (display option, only -- dimentions come from map)
    // -------------------------------------------

    outimage_create_parm_S iparm {}; 

    M_get_right_keyword_int64(expression, L"display" , iparm.display   , bool   )
      

    //  Destroy outimage first, if already active
    //  -----------------------------------------

    if (outimage_p != nullptr)
    {
        M_dv_few(M_out(L"=======> CREATE_MAP_OUTIMAGE outimage_n:%d map_n:%d -- destroying old outimage first <======") % n_outimage % n_map;)
        
        auto rc = destroy_outimage(*outimage_p);   
        free_outimage(n_outimage);
        outimage_p = nullptr;                                                    // clear out out own local outimage pointer

        if (rc != 0) return rc; 
    }


    //  create outimage and mark as active
    //  ----------------------------------

    M_dv_few(M_out(L"=======> Doing   @CREATE_MAP_OUTIMAGE outimage_n:%d map_n:%d   <=======") % n_outimage % n_map;)

    outimage_p = alloc_outimage(n_outimage);                                    // point to new outimage
    create_map_outimage(*outimage_p, *map_p, iparm, parm);    
   

    return 0; 
}
M_endf       



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_set_map_window() -- set up map window for put_map() and annotate_map()  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_set_map_window(const e_expression_S& expression, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_set_map_window() called");) 
    
    M_get_outimage_n(L"mapverb_set_map_window")
    M_get_map_n(  L"mapverb_set_map_window")

    M_check_outimage_n(L"SET_MAP_WINDOW")
    M_check_map_n(  L"SET_MAP_WINDOW")
    
    M_dv_some(M_out(L"=======> Doing   @SET_MAP_WINDOW outimage_n:%d map_n:%d <=======") % n_outimage % n_map;)
               

    // get set_map_window() @SET_MAP_WINDOW parms
    // ------------------------------------------

    set_map_window_parm_S fparm {};                // set up defaults in parms for set_map_window()

    M_get_right_keyword_int64(   expression, L"display"               , fparm.display               , bool   )
    M_get_right_keyword_int64(   expression, L"outimage_top_row"      , fparm.outimage_top_row      , rc_T   )
    M_get_right_keyword_int64(   expression, L"outimage_left_column"  , fparm.outimage_left_column  , rc_T   )
    M_get_right_keyword_int64(   expression, L"map_top_row"           , fparm.map_top_row           , rc_T   )
    M_get_right_keyword_int64(   expression, L"map_left_column"       , fparm.map_left_column       , rc_T   )
    M_get_right_keyword_int64(   expression, L"map_width"             , fparm.map_width             , rc_T   )
    M_get_right_keyword_int64(   expression, L"map_height"            , fparm.map_height            , rc_T   )


    // call set_map_window() to establish the map window data in the outimage structure
    // --------------------------------------------------------------------------------

    auto rc = set_map_window(*outimage_p, *map_p, fparm, parm);
    
    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf





////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_put_map() -- drap map data from mapdata_S into outimage_S buffer  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_put_map(const e_expression_S& expression, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_put_map() called");) 
    
    M_get_outimage_n(L"mapverb_put_map")
    M_get_map_n(     L"mapverb_put_map")
    M_get_ramp_n(    L"mapverb_put_map")

    M_check_outimage_n(L"PUT_MAP")
    M_check_map_n(     L"PUT_MAP")
    M_check_ramp_n(    L"PUT_MAP")
   
    M_dv_few(M_out(L"=======> Doing   @PUT_MAP outimage_n:%d map_n:%d ramp_n:%d  <=======") % n_outimage % n_map % n_ramp;)
               

    // set put_map() parms from @PUT_MAP args
    // --------------------------------------

    put_map_parm_S fparm {};                // set up defaults in parms for put_map()

    M_get_right_keyword_int64(   expression, L"display"               , fparm.display               , bool   )
    M_get_right_keyword_int64(   expression, L"shaded_relief"         , fparm.shaded_relief         , bool   )
    M_get_right_keyword_int64(   expression, L"land_hypsometry"       , fparm.land_hypsometry       , bool   )
    M_get_right_keyword_int64(   expression, L"ocean_bathymetry"      , fparm.ocean_bathymetry      , bool   )
    

    // call put_map() to draw the map data in the outimage structure
    // -------------------------------------------------------------

    auto rc = put_map(*outimage_p, *map_p, *ramp_p, fparm, parm);
    
    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   mapverb_annotate_map() -- put text (etc.) annotations in outimage graphics area based on associated map  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t mapverb_annotate_map(const e_expression_S& expression, mapparm_S& parm) try
{
    M__(M_out(L"mapverb_annotate_map() called");) 
    
    M_get_outimage_n(L"mapverb_annotate_map")
    M_get_map_n(     L"mapverb_annotate_map")

    M_check_outimage_n(L"ANNOTATE_MAP")
    M_check_map_n(     L"ANNOTATE_MAP")

    M_dv_few(M_out(L"=======> Doing   @ANNOTATE_MAP outimage_n:%d map_n:%d  <=======") % n_outimage % n_map;)             
     

    // call annotate_map() to add text, etc.
    // -------------------------------------

    auto a_rc = annotate_map(*map_p, *outimage_p, parm);
    if (a_rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf    

//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""