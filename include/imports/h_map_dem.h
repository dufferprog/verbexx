// h_map_dem.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////      
////     ===========
////     h_map_dem.h
////     ===========
//// 
//// 
////     contains definitions for DEM file contents and related items
//// 
////    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
#pragma once
#include "h_map_types.h"
#include "h_map_grid.h"
#include "h_map_tile.h"


//-----------------------------------------------------------------------------------------------------------------------------------
//     DEM file data set coverage info (files on disk, etc.)  
//-----------------------------------------------------------------------------------------------------------------------------------

// tile-oriented metadata for file groups that cover latitude/longitude regions
// ----------------------------------------------------------------------------

struct fileinfo_tile_S
{
public:

//  grid-oriented info

    ll_T                       limit_s                {   0   } ;     // nominal south limit for coverage (in arc-sec)                   
    ll_T                       limit_w                {   0   } ;     // nominal west  limit for coverage (in arc-sec) 
    ll_T                       limit_n                {   0   } ;     // nominal north limit for coverage (in arc-sec) 
    ll_T                       limit_e                {   0   } ;     // nominal east  limit for coverage (in arc-sec) 
                                                                      
    ll_T                       origin_ns              {   0   } ;     // nominal S origin (in arc-sec) for tiles/files in folder (if not global)     
    ll_T                       origin_ew              {   0   } ;     // nominal W origin (in arc-sec) for tiles/files in folder (if not global)
    ll_T                       step_ns                {   0   } ;     // N-S step (in arc-sec) for tiles/files in folder (if not global)   
    ll_T                       step_ew                {   0   } ;     // E-W step (in arc-sec) for tiles/files in folder (if not global)
                                        
    int32_t                    number_ns              {   0   } ;     // number of tiles in N-S direction (computed from nominal limits and step_ns)
    int32_t                    number_ew              {   0   } ;     // number of tiles in E-W direction (computed from nominal limits and step_ew)


//  filename-oriented info 

    bool                       fn_global              { false } ;     // true = only one file for whole earth -- no lat/lon in basename
    bool                       fn_many                { false } ;     // true = many files -- lat/lon are part of generated basename 
                             
    bool                       fn_sw_corner           { false } ;     // true = filenames use nominal SW corner latitude/longitude -- false = nominal NW corner
    bool                       fn_nw_corner           { false } ;     // true = filenames use nominal NW corner latitude/longitude -- false = nominal SW corner
    bool                       fn_upper_case          { false } ;     // true = N, E, S, W in filenames are upper case
    bool                       fn_lower_case          { false } ;     // true = n, e, s, w in filenames are lower case
    bool                       fn_lat_then_lon        { false } ;     // true = latitude first in filename
    bool                       fn_lon_then_lat        { false } ;     // true = longitude first in filename
                             
    bool                       multi_ft               { false } ;     // true = multiple filetypes in filegroup -- filetype field is empty 
    bool                       single_ft              { false } ;     // true = on file and filetype per filegroup -- filetype field filled in 
    bool                       varying_suffix         { false } ;     // true = basename suffix varies -- multiple suffixes in .suffix vector 
    bool                       fixed_suffix           { false } ;     // true = basename suffix fixed (or none) -- single suffix in .suffix vector (may be empty, if no suffix) 

    std::wstring               folder                 {       } ;     // folder for dem file(s)
    std::wstring               prefix                 {       } ;     // prefix for basename   -- before lat/lon
    std::wstring               separator              {       } ;     // separator in basename -- between lat and lon
    uint64_t                   suffix_ct              {   0   } ;     // number of suffixes in vector               
    std::vector<std::wstring>  suffixes               {       } ;     // suffix(es) for basename   -- after lat/lon  -- can be 0, 1, or multiple suffixes
    std::wstring               filetype               {       } ;     // filetype for dem file(s) -- empty string if multi_ft flag is set
};                                       


// raster-oriented metadata for file groups that contain raster data 
// -----------------------------------------------------------------

struct fileinfo_raster_S 
{
public:

    // overall data layout

    bool          hgt_layout             { false     } ;     // simple .hgt (or .hgtle) layout
    bool          simple_dt2_layout      { false     } ;     // simple .dt2 file layout (i.e. fixed length, no missing data, evenly spaced, etc.))

    size_t        file_min_size          { 0         } ;     // expected minimum file size 
    size_t        file_max_size          { 0         } ;     // expected maximum file size 


    // datatype-oriented parms for DEM data

    bool          little_endian          { false     } ;     // true = file data items are little-endian;  false = big-endian 
    bool          big_endian             { false     } ;     // true = file data items are big-endian;     false = little-endian 
    bool          int_16                 { false     } ;     // true = file data items are int16_t 
    bool          float_32               { false     } ;     // true = file data items are float32_t
    bool          twos_compliment        { false     } ;     // true = negative integer items are two's compliment 
    bool          signed_magnitude       { false     } ;     // true = negative integer items are signed magnitude 


    // grid-oriented parms -- independent of latitude and longitude of individual tile

    bool          pixels_aligned         { false     } ;     // pixel centers are aligned at nominal grid points (usually means one extra row/column)
    bool          pixels_centered        { false     } ;     // pixel centers are offset from nominal grid points (usually no extra row/column)
    bool          extra_row_col          { false     } ;     // true = files have 1 extra row/col -- usually pixels not centered
    bool          no_extra_row_col       { false     } ;     // true = files have do not extra row/col -- usually pixels centered
                                                     
    ll_T          extent_ns              {   0       } ;     // one nominal file N-S coverage size (in arc-sec) -- usually 3600 
    ll_T          extent_ew              {   0       } ;     // one file nominal E-W coverage size (in arc-sec) -- usually 3600 
    int32_t       ppd_ns                 {   0       } ;     // pixels per N-S degree in grid for each tile   
    int32_t       ppd_ew                 {   0       } ;     // pixels per E-W degree in grid for each tile   
};


// combined file info tiling + raster + ????
// -----------------------------------------

struct fileinfo_S : fileinfo_tile_S, fileinfo_raster_S 
{
public:
    bool          raster_data            { false } ;    // true if this fileset contains raster data (not shapefiles, etc.)
};


// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------
//     Array of .hgt data (big-endian or little endian) -- has data from one rectangular .hgt file, but with added size/location info
//-----------------------------------------------------------------------------------------------------------------------------------

struct hgt_S 
{
    // file name, tile, and grid fields

    tile_S            tile                     ;        // filename  for data currently in hgt buffer                                  
    grid_S            grid                     ;        // grid info for data currently in hgt buffer


    // data array control fields

    bool              little_end     {true}    ;        // TRUE = little_endian data                          (input parm)
    buffer_C<ht_T>    buf                      ;        // buffer for read-in .hgt data                       (may be big-endian)
    uint64_t          ct             {0ULL}    ;        // number of bytes of data present in buffer
    std::wstring      file                     ;        // name of file currently in buffer                   (for caching)      
}; 


// MACROs for getting a height from the hgt_t buffer

#define M_hgt_ht( b,r,c)     (((b).buf.p)[(r) * ((b).grid.cols) + (c)])    // use to get both bytes (useful if little-endian)

#define M_hgt_ht0(b,r,c)      *(int8_t * )&M_hgt_ht(b, r, c)               // get 1st byte 
#define M_hgt_ht1(b,r,c)    *(((int8_t * )&M_hgt_ht(b, r, c)) + 1)         // get 2nd byte  
#define M_hgt_ht2(b,r,c)     *(int16_t * )&M_hgt_ht(b, r, c)               // get both bytes

#define M_hgt_ht_p( b,r,c)    (           &M_hgt_ht(b, r, c)     )
#define M_hgt_ht0_p(b,r,c)    ( (int8_t *)&M_hgt_ht(b, r, c)     )         // get ptr to 1st byte (hi)
#define M_hgt_ht1_p(b,r,c)    (((int8_t *)&M_hgt_ht(b, r, c)) + 1)         // get ptr to 2nd byte (lo) 



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  



//---------------------------------------------------------------------------------------------------------------------
//     hgt_S and DEM file-oriented function prototypes
//---------------------------------------------------------------------------------------------------------------------

fileinfo_S   *find_fileinfo(                 int64_t                                                                         );      // locate fileinfo in repository with passed-in ID
fileinfo_S   *alloc_fileinfo(                int64_t                                                                         );      // make sure fileinfo is allocated in repository for passed-in ID (passes back simple pointer address of fileinfo_S) 
void          free_fileinfo(                 int64_t                                                                         );      // make sure fileinfo is not allocated in repository for passed-in ID 
void          display_fileinfo(        const fileinfo_S&                                                                     );      // display fields in passed-in fileinfo structure 

int           readin_file(             const std::wstring&, databuf_C&,       uint64_t, uint64_t                             );
int           readin_file(             const std::wstring&, void *, uint64_t, uint64_t, uint64_t, uint64_t&                  ); 

void          create_hgt(                    hgt_S&,                         const fileinfo_S&, bool = false                 );
void          destroy_hgt(                   hgt_S&                                                                          );
void          reuse_hgt(                     hgt_S&, ll_T, ll_T                                                              );
int           readin_hgt(                    hgt_S&, ll_T, ll_T, ll_T, ll_T, const fill_raster_parm_S&    , const fileinfo_S&); 
int           writeout_hgt(                  hgt_S&,                         const writeout_raster_parm_S&, const fileinfo_S&);
int32_t       get_elev_ble(            const hgt_S&, rc_T, rc_T                                                              ); 
int32_t       get_elev_be(             const hgt_S&, rc_T, rc_T                                                              );
int32_t       get_elev_ble_nearest(    const hgt_S&, rcf_T , rcf_T                                                           );
int32_t       get_elev_ble_interpolate(const hgt_S&, rcf_T , rcf_T                                                           ); 

      

//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""