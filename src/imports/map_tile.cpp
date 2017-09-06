// map_tile.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ============
////     map_tile.cpp -- functions for the tile_S structure 
////     ============
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
#include "h_map_dem.h"
#include "h_map_tile.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       adjust_longitude() -- given lat/lon, try to adjust ( + - 360 only) to get in valid range for grid         
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// llr_T version with tile_S -- also see .grid.cpp for grid_S versions
// -------------------------------------------------------------------
//
//  assume for now:: at west/south edge is OK -- at east/north edge is adjusted
//

int adjust_longitude(const tile_S& tile, llr_T  lat, llr_T& parm_lon, const fileinfo_S& fileinfo) try
{
   llr_T lon = parm_lon;


    // normalize longitude first (-180.0 to +180.0)

    lon = M_fix_lr(lon);

    M__(M_out(L"parm_lat=%d   parm_lon=%d   lon=%d") %  parm_lat % parm_lon % lon;)
    
    // try to correct out-of bounds input longitude to bring within nominal raster range, by adding or subtracting 360.0 degrees
       
    if      (lon >= M_l_to_lr(fileinfo.limit_e)) lon -= 360.0;        
    else if (lon <  M_l_to_lr(fileinfo.limit_w)) lon += 360.0;
   

    // if latitude or longitude is still out of nominal bounds, return -1 

    if ( (lon < M_l_to_lr(fileinfo.limit_w)) || (lon >= M_l_to_lr(fileinfo.limit_e)) || (lat < M_l_to_lr(fileinfo.limit_s)) || (lat >= M_l_to_lr(fileinfo.limit_n)) )
    {
        return -1;   
    }


    // pass back adjusted logitude (latitude is not adjusted, but is used in range check)

    parm_lon = lon; 

    return 0; 

}
M_endf


//   ------------------------------------------------------------------ 
//   ll_T version with tile_S -- also see .grid.cpp for grid_S versions
//    -----------------------------------------------------------------

int adjust_longitude(const tile_S& tile, ll_T lat, ll_T& parm_lon, const fileinfo_S& fileinfo) try
{
    ll_T lon = parm_lon;


    // normalize longitude first (-180.0 to +180.0)

    lon = M_fix_l(lon);

    M__(M_out(L"parm_lat=%d (%.16f deg)   parm_lon=%d (%.16f deg)   lon=%d (%.16f deg)") % parm_lat % M_l_to_lr(parm_lat) % parm_lon % M_l_to_lr(parm_lon) % lon % M_l_to_lr(lon); )
    
    // try to correct out-of bounds input longitude to bring within nominal raster range, by adding or subtracting 360 degrees
       
    if      (lon >= fileinfo.limit_e) lon -= M_arcsec_360;        
    else if (lon <  fileinfo.limit_w) lon += M_arcsec_360;
    
    M__(M_out(L"lon=%d (%.16f deg)   fileinfo.limit_e=%d (%.16f deg)   fileinfo.limit_w=%d (%.16f deg)") % lon % M_l_to_lr(lon) % fileinfo.limit_e % M_l_to_lr(fileinfo.limit_e) % fileinfo.limit_w & M_l_to_lr(fileinfo.limit_w);)
        

    // if latitude or longitude is still out of nominal bounds, return -1 

    if ( (lon < fileinfo.limit_w) || (lon >= fileinfo.limit_e) || (lat < fileinfo.limit_s) || (lat >= fileinfo.limit_n) )
    {
        return -1;   
    }

    // pass back adjusted logitude (latitude is not adjusted, but is used in range check)

    parm_lon = lon; 

    return 0; 

}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       tile_filename() -- find filename for tile that covers caller's input area
//||
//||
//||         -- input south/west in arc-sec
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void tile_filename(tile_S& tile, ll_T south_in, ll_T west_in, ll_T extent_ns, ll_T extent_ew, const fileinfo_S& fileinfo, bool debug_tile, int suffix_n) try
{
    // clear out old output info in tile (in case of error)
    // ----------------------------------------------------

    reset_tile(tile);


    // clip input longitude to range -180 to 180 degrees
    // -------------------------------------------------
          
 //   ll_T x = west_in + M_arcsec_180;                 // should be in range of 0 - 359 degrees
    
 //   if (x >= 0)
 //      x = x % M_arcsec_360;                            // bring large numbers back down to 0-359 degrees
 //   else
 //      x = x % M_arcsec_360 + M_arcsec_360;             // bring large negative numbers back to 0-359 degrees

 //   ll_T west = x - M_arcsec_180;                    // should be in range -180 to 179 degrees
   
 //   ll_T west = M_fix_l(west_in);

    
    //  Adjust longitude to be within fn tiling range (-180 to +179 Degrees, 0 to 359, -90 to 269, -270 to 89, etc.)
    //  ------------------------------------------------------------------------------------------------------------
        
   // if (west < tile.origin_ew)
   //     west += M_arcsec_360;  
  //  else if (west > tile.origin_ew + M_arcsec_360)  
   //     west -= M_arcsec_360; 

  
    ll_T south = south_in;
    ll_T west  = west_in; 

    int rc = adjust_longitude(tile, south, west, fileinfo);

    ll_T north = south + extent_ns; 
    ll_T east  = west  + extent_ew; 

    if (debug_tile)
    {
        M_out(L"tile_filename(): south_in = %d (%.16f deg)   west_in = %d (%.16f deg)   extent_ns = %d (%.16f deg)   extent_ew = %d (%.16f deg)") % south_in % M_l_to_lr(south_in) % west_in % M_l_to_lr(west_in) % extent_ns % M_l_to_lr(extent_ns) % extent_ew % M_l_to_lr(extent_ew); 
        M_out(L"tile_filename(): south    = %d (%.16f deg)      west = %d (%.16f deg)       north = %d (%.16f deg)        east = %d (%.16f deg)") % south    % M_l_to_lr(south)    % west    % M_l_to_lr(west)    % north     % M_l_to_lr(north)     % east      % M_l_to_lr(east)     ;
    }

    if (rc != 0) 
    {
        M_out_emsg1(L"tile_filename(): Requested SW corner not in tile coverage area -- south=%d (%.16f deg) west=%d (%.16f deg)") % south_in % M_l_to_lr(south_in) % west_in % M_l_to_lr(west_in);
        M_out_emsg2(L"fileinfo.limit_s=%d (%.16f deg)  fileinfo.limit_w=%d (%.16f deg)  fileinfo.limit_n=%d (%.16f deg)  fileinfo.limit_e=%d (%.16f deg)") % fileinfo.limit_s % M_l_to_lr(fileinfo.limit_s) % fileinfo.limit_w % M_l_to_lr(fileinfo.limit_w) % fileinfo.limit_n % M_l_to_lr(fileinfo.limit_n) % fileinfo.limit_e % M_l_to_lr(fileinfo.limit_e);
        M_throw(     "tile_filename(): Requested SW corner not in tile coverage area")
    }


    // Make sure caller's request is not out-of-bounds of tile coverage 
    // ----------------------------------------------------------------

    if (
        (south < fileinfo.limit_s)
        ||
        (west  < fileinfo.limit_w)
        ||
        (north > fileinfo.limit_n)
        ||
        (east  > fileinfo.limit_e)
       )
    {
        M_out_emsg1(L"tile_filename(): one or more requested tile corners are outside the bounds of filegroup coverage area :");  
        M_out_emsg0(L"               :         south_in = %d (%.16f deg)           west_in = %d (%.16f deg)         extent_ns = %d (%.16f deg)         extent_ew = %d (%.16f deg)")
                   %  south_in   % M_l_to_lr(south_in)   % west_in   % M_l_to_lr(west_in)   % extent_ns  %  M_l_to_lr(extent_ns)  % extent_ew %  M_l_to_lr(extent_ew); 
        M_out_emsg0(L"               :            south = %d (%.16f deg)              west = %d (%.16f deg)             north = %d (%.16f deg)              east = %d (%.16f deg)")
                   %  south      % M_l_to_lr(south)      % west      % M_l_to_lr(west)      % north      %  M_l_to_lr(north)      % east      %  M_l_to_lr(east)     ;
        M_out_emsg2(L"               : fileinfo.limit_s = %d (%.16f deg)  fileinfo.limit_w = %d (%.16f deg)  fileinfo.limit_n = %d (%.16f deg)  fileinfo.limit_e = %d (%.16f deg)")
                   %  fileinfo.limit_s % M_l_to_lr(fileinfo.limit_s) % fileinfo.limit_w % M_l_to_lr(fileinfo.limit_w) % fileinfo.limit_n %  M_l_to_lr(fileinfo.limit_n) % fileinfo.limit_e %  M_l_to_lr(fileinfo.limit_e);
        M_throw(     "tile_filename(): one or more requested tile corners are outside the bounds of filegroup coverage area")
        return;        // shouldn't get here 
    }
    

    // find out which DEM file caller's input lat/lon lies in 
    // ------------------------------------------------------
    
    ll_T tile_lat;
    ll_T tile_lon; 
    ll_T curr_south;
    ll_T curr_west;
    ll_T curr_north;
    ll_T curr_east;

    if (fileinfo.fn_sw_corner)                                                                              
    {
        tile_lat   = fileinfo.origin_ns + M_rounddown(south - fileinfo.origin_ns,  fileinfo.step_ns); 
        curr_south = tile_lat; 
        curr_north = tile_lat + fileinfo.step_ns;

        if (debug_tile)
        {
            M_out(L"tile_filename(): SW corner filename basing -- tile_lat = %d (%.16f deg)   origin_ns = %d (%.16f deg)   step_ns = %d (%.16f deg)")
                 % tile_lat % M_l_to_lr(tile_lat) % fileinfo.origin_ns % M_l_to_lr(fileinfo.origin_ns) % fileinfo.step_ns % M_l_to_lr(fileinfo.step_ns);
        }  
    }
    else                              // must be NW corner-based naming
    {
        tile_lat   = fileinfo.origin_ns + M_roundup(north - fileinfo.origin_ns , fileinfo.step_ns); 
      //  tile_lat   = fileinfo.origin_ns + fileinfo.step_ns + ( (north - fileinfo.origin_ns) / fileinfo.step_ns) * fileinfo.step_ns; 
        curr_north = tile_lat; 
        curr_south = tile_lat - fileinfo.step_ns;

        if (debug_tile)
        {
            M_out(L"tile_filename(): NW corner filename basing -- tile_lat = %d (%.16f deg)   origin_ns = %d (%.16f deg)   step_ns = %d (%.16f deg)")
                 % tile_lat % M_l_to_lr(tile_lat) % fileinfo.origin_ns % M_l_to_lr(fileinfo.origin_ns) % fileinfo.step_ns % M_l_to_lr(fileinfo.step_ns);
        }
    }

    tile_lon  = fileinfo.origin_ew + M_rounddown(west - fileinfo.origin_ew, fileinfo.step_ew); 
    curr_west = tile_lon; 
    curr_east = tile_lon + fileinfo.step_ew; 

    if (debug_tile)
    {
        M_y(M_out(L"tile_filename(): tile_lat   = %d (%.16f deg)   tile_lon  = %d (%.16f deg)")
                 % tile_lat % M_l_to_lr(tile_lat) % tile_lon % M_l_to_lr(tile_lon);)
        M_y(M_out(L"tile_filename(): curr_south = %d (%.16f deg)   curr_west = %d (%.16f deg)   curr_north = %d (%.16f deg)   curr_east = %d (%.16f deg)")
                 % curr_south % M_l_to_lr(curr_south) % curr_west % M_l_to_lr(curr_west) % curr_north % M_l_to_lr(curr_north) % curr_east % M_l_to_lr(curr_east);)
    }

    // Make sure computed tile lat/lon are even degrees (temporary check???)
    // ------------------------------------------------

    if ( (tile_lat % 3600 != 0) || (tile_lon % 3600 != 0) )
    {    
        M_out_emsg(L"tile_filename(): computed tile corner lat/lon are not whole degrees -- tile_lat=%d (%.16f deg)  tile_lon=%d (%.16f deg)") % tile_lat % M_l_to_lr(tile_lat) % tile_lon % M_l_to_lr(tile_lon) ;
        M_throw(    "tile_filename(): computed tile corner lat/lon are not whole degrees")
    }


    // Make sure caller's request fits inside the located DEM file (no spanning 2 or 4 files) 
    // --------------------------------------------------------------------------------------

    if (
        (south < curr_south)
        ||
        (west  < curr_west)
        ||
        (north > curr_north)
        ||
        (east  > curr_east)
       )
    {
        M_out_emsg1(L"tile_filename(): Entire requested area does not fit in one tile :"); 
        M_out_emsg0(L"               :           south_in = %d (%.16f deg)              west_in = %d (%.16f deg)         extent_ns = %d (%.16f deg)        extent_ew = %d (%.16f deg)")
                   %  south_in   % M_l_to_lr(south_in)   % west_in   % M_l_to_lr(west_in)   % extent_ns  %  M_l_to_lr(extent_ns)  % extent_ew %  M_l_to_lr(extent_ew) ; 
        M_out_emsg0(L"               :              south = %d (%.16f deg)                west = %d (%.16f deg)             north = %d (%.16f deg)              east = %d (%.16f deg)")
                   %  south      % M_l_to_lr(south)      % west      % M_l_to_lr(west)      % north      %  M_l_to_lr(north)      % east      %  M_l_to_lr(east)      ;
        M_out_emsg0(L"               :         curr_south = %d (%.16f deg)           curr_west = %d (%.16f deg)        curr_north = %d (%.16f deg)         curr_east = %d (%.16f deg)")
                   %  curr_south % M_l_to_lr(curr_south) % curr_west % M_l_to_lr(curr_west) % curr_north %  M_l_to_lr(curr_north) % curr_east %  M_l_to_lr(curr_east) ; 
        M_out_emsg0(L"               : fileinfo.origin_ns = %d (%.16f deg)  fileinfo.origin_ew = %d (%.16f deg)  fileinfo.step_ns = %d (%.16f deg)  fileinfo.step_ew = %d (%.16f deg)")
                   %  fileinfo.origin_ns % M_l_to_lr(fileinfo.origin_ns) % fileinfo.origin_ew % M_l_to_lr(fileinfo.origin_ew) % fileinfo.step_ns %  M_l_to_lr(fileinfo.step_ns) % fileinfo.step_ew %  M_l_to_lr(fileinfo.step_ew);
        M_out_emsg2(L"               :           tile_lat = %d (%.16f deg)            tile_lon = %d (%.16f deg)")
                   %  tile_lat      % M_l_to_lr(tile_lat)      % tile_lon      % M_l_to_lr(tile_lon)  ;

        M_throw(     "tile_filename(): Entire requested area does not fit in one tile")
        return;                       // control should not reach here
    }
  

    // get basename suffix to use for this filename
    // --------------------------------------------
                                                       
    std::wstring suffix { };                           // start with empty suffix

    if (fileinfo.suffix_ct > 0)
    {
        if (suffix_n >= fileinfo.suffix_ct)            // see if passed-in suffex nuber is valid
        {
            M_out_emsg(L"tile_filename(): requested suffix number (%d) too high -- number of suffixes available = %d") % suffix_n % fileinfo.suffix_ct; 
            M_throw(    "tile_filename(): requested suffix number too high")
            return;                                   // should not get here
        }
    
        suffix = fileinfo.suffixes.at(suffix_n);
    }
    

    //  set curr_south, west, north, east field in output area -- bounds of located tile
    //  --------------------------------------------------------------------------------

    tile.curr_south = curr_south; 
    tile.curr_west  = curr_west; 
    tile.curr_north = curr_north; 
    tile.curr_east  = curr_east; 


    //  set global filename and return (if no lat/lon-dependent filenames)
    //  ------------------------------------------------------------------

    if (fileinfo.fn_global)
    {
        tile.basename = fileinfo.prefix + suffix;                        // one of these should probably have the basename, the other is probably empty     
        tile.filename = fileinfo.folder + tile.basename;

        if (!fileinfo.multi_ft)
            tile.filename += fileinfo.filetype;
           
        return; 
    }
    

    // Local filename -- Determine file name based on tile latitude/longitude -- NnnEnnn wnnn_snn, etc (swprintf() output is in fn[]
    // -----------------------------------------------------------------------------------------------------------------------------

    lld_T fn_lat = tile_lat / 3600;        //   should be whole number of degrees here
    lld_T fn_lon = tile_lon / 3600;        //   should be whole number of degrees here

    wchar_t     fn1[16] {};     // for swprintf      lat or lon
    wchar_t     fn2[16] {};     // for swprintf      lon or lat
      
    wchar_t    s;               // S or s 
    wchar_t    w;               // W or w
    wchar_t    n;               // N or n
    wchar_t    e;               // E or e

    if (fileinfo.fn_upper_case)
    {
        s = L'S'; 
        w = L'W'; 
        n = L'N'; 
        e = L'E';     
    }
    else
    {
        s = L's'; 
        w = L'w'; 
        n = L'n'; 
        e = L'e';     
    }

    if (fileinfo .fn_lat_then_lon)       // latitude comes first in basename
    {
        if (fn_lat < 0) 
            (void)swprintf(fn1, (size_t)10, L"%c%02d", s, -fn_lat); 
        else
            (void)swprintf(fn1, (size_t)10, L"%c%02d", n,  fn_lat); 

        if (fn_lon < 0) 
            (void)swprintf(fn2, (size_t)12, L"%c%03d", w, -fn_lon); 
        else
            (void)swprintf(fn2, (size_t)12, L"%c%03d", e,  fn_lon);
    }
    else                                 // longitude comes first in baseanme
    {
        if (fn_lon < 0) 
            (void)swprintf(fn1, (size_t)12, L"%c%03d", w, -fn_lon); 
        else
            (void)swprintf(fn1, (size_t)12, L"%c%03d", e,  fn_lon);

         if (fn_lat < 0) 
            (void)swprintf(fn2, (size_t)10, L"%c%02d", s, -fn_lat); 
        else
            (void)swprintf(fn2, (size_t)10, L"%c%02d", n,  fn_lat); 
    }

    tile.basename = fileinfo.prefix + std::wstring{fn1} + fileinfo.separator + std::wstring{fn2} + suffix; 
    tile.filename = fileinfo.folder + tile.basename;

    if (!fileinfo.multi_ft)
        tile.filename += fileinfo.filetype;
    
    if (debug_tile)
    {
        M_out(L"tile_filename(): tile.basename = \"%S\"") % tile.basename; 
        M_out(L"tile_filename(): tile.filename = \"%S\"") % tile.filename;
    }
     
    return;   
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       display_tile() -- display fields in tile_S structure
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_tile(const tile_S& tile)
{   
    M_out(L"tile.basename        = \"%s\""             )     % tile.basename.c_str()                                        ;
    M_out(L"tile.filename        = \"%s\""             )     % tile.filename.c_str()                                        ;
    M_out(L"tile.curr_south      = %-7d (%21.16f deg)" )     % tile.curr_south      % M_l_to_lr(tile.curr_south)            ;
    M_out(L"tile.curr_west       = %-7d (%21.16f deg)" )     % tile.curr_west       % M_l_to_lr(tile.curr_west )            ;
    M_out(L"tile.curr_north      = %-7d (%21.16f deg)" )     % tile.curr_north      % M_l_to_lr(tile.curr_north)            ;
    M_out(L"tile.curr_east       = %-7d (%21.16f deg)" )     % tile.curr_east       % M_l_to_lr(tile.curr_east )            ;
                                          
    return;
}



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       reset_tile() -- reset volatile tile fields to initial state 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void reset_tile(tile_S& tile) try
{    
    // clear out current state and output fields

    tile.basename.clear();        
    tile.filename.clear();
    tile.curr_south = INT_MIN; 
    tile.curr_west  = INT_MIN;
    tile.curr_north = INT_MIN;
    tile.curr_east  = INT_MIN;  

    return;
}
M_endf


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""