// h_map_parms.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            =============
////            h_map_parms.h -- collected map-oriented run parameters, etc.  
////            =============
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_map_types.h"


//---------------------------------------------------------------------------------------------------------------------
//     Thread-oriented parms
//---------------------------------------------------------------------------------------------------------------------

struct thread_parm_S
{
public:
    bool      display;          // display thread info
    int32_t   max;              // maximum threads allowed
    int32_t   n_water;          // target number of threads for water detection, etc.
    int32_t   n_slope;          // target number of threads for slope 
    int32_t   n_map_fill;       // target number of threads for map fill
    int32_t   n_outimage_fill;  // target number of threads for outimage fill
}; 

//---------------------------------------------------------------------------------------------------------------------
//     Slice-oriented parms
//---------------------------------------------------------------------------------------------------------------------

struct slice_parm_S
{
public:
    bool      display;          // display individual slice info
    int32_t   max;              // maximum allowed number of slices (per single thpa usage)  
    int64_t   thread_area;      // slice must have at least this many rows*cols when used for threading 
    int64_t   nonthread_area;   // slice must have at least this many rows*cols when not used for threading 
    int32_t   thread_dim;       // slice must have at least this many rows*cols when used for threading 
    int32_t   nonthread_dim;    // slice must have at least this many rows*cols when not used for threading 
}; 



//---------------------------------------------------------------------------------------------------------------------
//     Water detection oriented configuration parms
//---------------------------------------------------------------------------------------------------------------------

// values for "check" field below -- should become an enum class

#define WATER_NOLAKE  0            // no lake check
#define WATER_LAKE    1            // check lakes
#define WATER_ICECAP  2            // check lakes below 1000m only (internal parm -- set automatically in icecap zones if adaptive water check in effect)

struct water_parm_S
{
public:
    int32_t   check_lake;        // see values above
    bool      check_ocean;       // true -- do ocean check, false = no ocean check    
    int32_t   min_lake;          // minimum detectable lake 3x3 5x5 7x7, etc.
    int32_t   stride;            // stride for water detection -- 1, 2, 3, etc.  
    int32_t   lk_rem;            // remove lakes narrower/shorter than this (pixels) 
    int32_t   lb_rem;            // remove land bridges narrower/shorter than this (pixels)
    int32_t   expand;            // maximum number of passes for water fill loops
    int32_t   ht_min;            // suppress lakes check below this height
    int32_t   ht_max;            // suppress lake check above this height   

    bool      adaptive;          // true -- do adaptive water check 
    lld_T     a_icecap_lat_n;    // ocean detect only above this latitude
    lld_T     a_icecap_lat_s;    // ocean detect only below this latitude
    int32_t   a_lo_min_lake;     // lowest adaptive min_lake setting  
    int32_t   a_hi_min_lake;     // highest adaptive min_lake setting  
    int32_t   a_lo_stride;       // lowest adaptive stride setting  
    int32_t   a_hi_stride;       // highest adaptive stride setting 
    int32_t   a_neighbor_xy;     // check for roughness in this +-x/y box 
    int32_t   a_samples;         // number of random samples in subslice for adaptive parm setting  
    int32_t   a_neighbor_samples;// numer of random samples (besides corners/middle to take in neighborhood
}; 
 


//=====================================================================================================================
//     map and mapdata_t oriented configuration parms
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
//     special colors in parm area
//---------------------------------------------------------------------------------------------------------------------

struct color_parm_S
{
public:
    rgb_S          water;              // Water area -- base color -- no bathymetry  (not ocean)             ***
    rgb_S          ocean;              // Ocean area -- base color -- no bathymetry                          ***
    rgb_S          land;               // Land area  -- base color -- no hypsometry                          ***
    rgb_S          graticule;          // graticule lines                                                    ***
    rgb_S          dem_void;           // void area in DEM file (in raster, etc.)                            ***
    rgb_S          outside_dem;        // area in raster not covered by any .hgt file (or other DEM file)    ***
    rgb_S          outside_raster;     // area in map that was not in raster                                 ***
    rgb_S          outside_projection; // area in map that is out-of-bounds for the projection               ***
    rgb_S          outside_map;        // are in outimage that was not in mapdata                            ***
};


//---------------------------------------------------------------------------------------------------------------------
//     combined parms structure
//---------------------------------------------------------------------------------------------------------------------

struct mapparm_S
{
public:

    //  imbedded parm areas

    thread_parm_S     thread;
    slice_parm_S      slice;
    water_parm_S      water; 
 //   demfile_parm_S    file;
 //   raster_parm_S     raster;
    color_parm_S      color; 
};





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   options-oriented functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void      default_options(      mapparm_S&);    // set option/parms to default value 
void      display_options(const mapparm_S&);    // display current options 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////