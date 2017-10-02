// h_map_verb.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ============ 
////            h_map_verb.h -- functions in map_verb.cpp and map_addverb.cpp
////            ============
////     
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_ex_interface.h"                // verb definitions need ex types
#include "h_map_types.h"




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_get_fileinfo_n -- MACRO to get value from fileinfo_n keyword
//
//    - assume fileinfo_n keyword exists with int64_t 
//
//    - MACRO parm is name of function for error message (not used)
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////// 

#define M_get_fileinfo_n(mt)                                                                       \
int64_t n_fileinfo {1};                                                                            \
                                                                                                   \
M_get_right_keyword_int64(expression, L"fileinfo_n", n_fileinfo, int64_t)                          \
fileinfo_S *fileinfo_p { find_fileinfo(n_fileinfo) };  




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_get_raster_n -- MACRO to get value from raster_n keyword
//
//    - assume raster_n keyword exists with int64_t 
//
//    - MACRO parm is name of function for error message (not used)
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////// 

#define M_get_raster_n(mt)                                                                     \
int64_t n_raster {1};                                                                          \
                                                                                               \
M_get_right_keyword_int64(expression, L"raster_n", n_raster, int64_t)                          \
raster_S *raster_p { find_raster(n_raster) };  




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_get_map_n -- MACRO to get value from map_n keyword
//
//    - assume map_n keyword exists with int64_t 
//
//    - MACRO parm is name of function for error message (not used)
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////// 

#define M_get_map_n(mt)                                                                     \
int64_t n_map {1};                                                                          \
                                                                                            \
M_get_right_keyword_int64(expression, L"map_n", n_map, int64_t)                             \
mapdata_S *map_p { find_map(n_map) };  




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_get_ramp_n -- MACRO to get value from ramp_n keyword
//
//    - assume ramp_n keyword exists with int64_t value
//
//    - MACRO parm is name of function for error message (not used)
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////    

#define M_get_ramp_n(mt)                                                                    \
int64_t n_ramp {1};                                                                         \
                                                                                            \
M_get_right_keyword_int64(expression, L"ramp_n", n_ramp, int64_t)                           \
ramp_S *ramp_p { find_ramp(n_ramp) };                                                    




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_check_fileinfo_n -- MACRO to make sure fileinfo_S is allocated in repository
//
//    - assume n_fileinfo and fileinfo_p are already set up for selected fileinfo_S
//
//    - MACRO parm is name of user verb for error message
//
//      note: uses n_fileinfo defined in M_get_fileinfo_n 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define M_check_fileinfo_n(mt)                                                                                          \
if (fileinfo_p == nullptr)                                                                                              \
{                                                                                                                       \
     M_out_emsg(L"=======> @%s failed -- fileinfo_S %d is not allocated in repository <======") % mt % n_fileinfo;      \
     return 1;                                                                                                          \
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_check_raster_n -- MACRO to make sure raster is allocated in repository and filled with data
//
//    - assume n_raster and raster_p are already set up for selected raster
//
//    - MACRO parm is name of user verb for error message
//
//      note: uses n_raster defined in M_get_raster_n 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define M_check_raster_n(mt)                                                                                  \
if (raster_p == nullptr)                                                                                      \
{                                                                                                             \
     M_out_emsg(L"=======> @%s failed -- raster %d is not allocated in repository <======") % mt % n_raster;  \
     return 1;                                                                                                \
}                                                                                                             \
                                                                                                              \
if (!(raster_p->filled))                                                                                      \
{                                                                                                             \
     M_out_emsg(L"=======> @%s failed -- raster %d is not filled-in with DEM data <======") % mt % n_raster;  \
     return 1;                                                                                                \
} 



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_checka_raster_n -- MACRO to make sure raster is allocated in repository (may be empty)
//
//    - assume n_raster and raster_p are already set up for selected raster
//
//    - MACRO parm is name of user verb for error message
//
//      note: uses n_raster defined in M_get_raster_n 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define M_checka_raster_n(mt)                                                                                 \
if (raster_p == nullptr)                                                                                      \
{                                                                                                             \
     M_out_emsg(L"=======> @%s failed -- raster %d is not allocated in repository <======") % mt % n_raster;  \
     return 1;                                                                                                \
}  



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_check_map_n -- MACRO to make sure map is allocated in repository
//
//    - assume n_map and map_p are already set up for selected map
//
//    - MACRO parm is name of user verb for error message
//
//      note: uses n_map defined in M_get_map_n 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define M_check_map_n(mt)                                                                               \
if (map_p == nullptr)                                                                                   \
{                                                                                                       \
     M_out_emsg(L"=======> @%s failed -- map %d is not allocated in repository <======") % mt % n_map;  \
     return 1;                                                                                          \
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_check_ramp_n -- MACRO to make sure ramp is allocated and completely initialized
//
//    - assume n_ramp and ramp_p are already set up for selected ramp 
//
//    - MACRO parm is name of user verb for error message
//
//      note: uses n_ramp defined in M_get_ramp_n 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define M_check_ramp_n(mt)                                                                                     \
if (ramp_p == nullptr)                                                                                         \
{                                                                                                              \
     M_out_emsg(L"=======> @%s failed -- ramp %d is not allocated in repository <======") % mt % n_ramp;       \
     return 1;                                                                                                 \
}                                                                                                              \
                                                                                                               \
if (!(ramp_p->complete))                                                                                       \
{                                                                                                              \
     M_out_emsg(L"=======> @%s failed -- ramp %d is not completely initialized <======") % mt % n_ramp;        \
     return 1;                                                                                                 \
} 




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// external function -- in map_addverb.cpp 
// ---------------------------------------
 
void add_map_verbs(const std::wstring&);


//   external verb-actioning functions -- in map_verb.cpp
//   ----------------------------------------------------

int32_t mapverb_map_option(                  const e_expression_S&,                mapparm_S&); 
int32_t mapverb_display_map_options(         const e_expression_S&,                mapparm_S&); 

int32_t mapverb_create_fileinfo(             const e_expression_S&,                mapparm_S&); 
int32_t mapverb_display_fileinfo(            const e_expression_S&,                mapparm_S&);  
int32_t mapverb_destroy_fileinfo(            const e_expression_S&,                mapparm_S&);  
 
int32_t mapverb_create_fixed_raster(         const e_expression_S&                           );
int32_t mapverb_create_auto_raster(          const e_expression_S&                           );
int32_t mapverb_fill_whole_raster(           const e_expression_S&,                mapparm_S&);
int32_t mapverb_selective_fill_raster(       const e_expression_S&,                mapparm_S&);
int32_t mapverb_smooth_raster(               const e_expression_S&                           );
int32_t mapverb_display_raster(              const e_expression_S&                           );
int32_t mapverb_writeout_raster(             const e_expression_S&                           );
int32_t mapverb_destroy_raster(              const e_expression_S&                           );
                                                          
int32_t mapverb_create_map(                  const e_expression_S&,                mapparm_S&); 
int32_t mapverb_fill_map(                    const e_expression_S&,                mapparm_S&); 
int32_t mapverb_water_map(                   const e_expression_S&,                mapparm_S&); 
int32_t mapverb_graticule_map(               const e_expression_S&,                mapparm_S&); 
int32_t mapverb_slope_map(                   const e_expression_S&,                mapparm_S&); 
int32_t mapverb_destroy_map(                 const e_expression_S&,                mapparm_S&);  
                                             
int32_t mapverb_create_map_outimage(         const e_expression_S&,                mapparm_S&);    
int32_t mapverb_set_map_window(              const e_expression_S&,                mapparm_S&);
int32_t mapverb_put_map(                     const e_expression_S&,                mapparm_S&);
int32_t mapverb_annotate_map(                const e_expression_S&,                mapparm_S&);

int32_t mapverb_ramp(                        const e_expression_S&,                mapparm_S&);
int32_t mapverb_draw_ramp(                   const e_expression_S&,                mapparm_S&);
int32_t mapverb_put_ramp(                    const e_expression_S&,                mapparm_S&);
int32_t mapverb_destroy_ramp(                const e_expression_S&,                mapparm_S&); 
                                             
int32_t mapverb_extract_map(                 const e_expression_S&, value_S&,      mapparm_S&);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////