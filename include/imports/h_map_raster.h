

// h_map_raster.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////            ==============
////            h_map_raster.h -- raster_S and functions  
////            ==============
////
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         
#pragma once
#include "h_map_types.h"
#include "h_map_grid.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// index values in 8-bit BMP color table

#define RASTER_OFF_IX       0
#define RASTER_NVALID_IX    1
#define RASTER_VOID_IX      2
#define RASTER_WATER_IX     3
#define RASTER_GRID_IX      4 

#define RASTER_SPECIAL_COLORS   5


// special heights in the raster data -- all other heights are valid elevations (hopefully)

#define RASTER_MAX_BAD -17477   // values below this are not normal
#define RASTER_EMPTY   -17477   // 0xBBBB -- unfilled data point in raster
#define RASTER_OUT     -17478   // requested point is out of bounds of raster (returned (substitute) value only -- no data point in raster has this value) 
#define RASTER_VOID    -32768   // directly from .hgt files -- void in .hgt data



//---------------------------------------------------------------------------------------------------------------------
//     Array of raster data -- elevation data is always litttle-endian
//---------------------------------------------------------------------------------------------------------------------

struct raster_S 
{
    bool                filled             { false } ;   // true = raster has been filled, at least partially, and can be used for a map 
    bool                complete           { false } ;   // true = raster has been completely filled (@FILL_WHOLE_RASTER), and can be written out to files
    bool                autolim            { false } ;   // true = raster limits computed automatically from associated map limits
                                                     
    grid_S              grid;                            // imbedded pixel grid info  (does not contain buffer for data points)
                                                                                      
    ll_T                window_ns          { 0     } ;   // NS window extent -- arc-sec
    ll_T                window_ew          { 0     } ;   // EW window extent -- arc-sec
                                                     
    uint64_t            hgt_ct             { 0     } ;   // number of hgt items added 
    rc_T                col_min            { 0     } ;   // lowest column number with data     
    rc_T                col_max            { 0     } ;   // highest column number with data
    rc_T                row_min            { 0     } ;   // lowest row number with data
    rc_T                row_max            { 0     } ;   // highest row number with data
          

    // datatype-oriented flags for raster data (always little-endian, and int_16 for now)

    bool                little_endian      { true  } ;   // true = file data items are little-endian;  false = big-endian     (always true  for raster)
    bool                big_endian         { false } ;   // true = file data items are big-endian;     false = little-endian  (always false for raster) 
    bool                int_16             { true  } ;   // true = file data items are int16_t                                (always true  for raster)
    bool                float_32           { false } ;   // true = file data items are float32_t                              (always false for raster)



    buffer_C<ht_T>      buf;                             // data buffer 
     

    ////////////////////// statistical area

    // -- file-oriented counters (fill raster, writeout raster)

    uint64_t            windows_done                {0};       // number of windows processed 
    uint64_t            bytes_written               {0};       // total number of bytes written to   DEM files 
    uint64_t            bytes_read                  {0};       // total number of bytes read in from DEM files 
    uint64_t            files_skipped               {0};       // DEM files skipped (selective raster fill)
    uint64_t            files_read                  {0};       // total DEM files read in
    uint64_t            files_reused                {0};       // total DEM files with suppressed read (reused data in buffer)
    uint64_t            files_missing               {0};       // total DEM files missing

    uint64_t            files_written               {0};       // total DEM files written out
    uint64_t            files_suppressed            {0};       // total empty DEM files not written out (suppressed)
                                                    
    uint64_t            basefill_files_read         {0};       // number of DEM files read for base_fill
    uint64_t            basefill_files_missing      {0};       // number of DEM files missing for base_fill
                                                    
    uint64_t            voidfill_files_read         {0};       // number of DEM files read    for void_fill
    uint64_t            voidfill_files_missing      {0};       // number of DEM files missing for void_fill
    uint64_t            voidfills_needed            {0};       // number of void fills needed
    uint64_t            voidfills_not_needed        {0};       // number of void fills not needed
                                                    
    uint64_t            emptyfill_files_read        {0};       // number of DEM files read    for empty_fill 
    uint64_t            emptyfill_files_missing     {0};       // number of DEM files missing for empty_fill 
    uint64_t            emptyfills_needed           {0};       // number of empty fills needed
    uint64_t            emptyfills_not_needed       {0};       // number of empty fills not needed


    // -- data copying-oriented counters (raster fill)

    uint64_t            windows_whole_rows          {0};       // number of windows where whole rows can be copied at once
    uint64_t            windows_exact_coverage      {0};       // number of windows where hgt_S and raster_S grid points are     aligned exactly
    uint64_t            windows_inexact_coverage    {0};       // number of windows where hgt_S and raster_S grid points are not aligned exactly
                                                       
    uint64_t            set_whole_row_le            {0};       // number of little-endian rows that can be copied over
    uint64_t            set_whole_row_be            {0};       // number of    big-endian rows that can be copied over 
    uint64_t            set_aligned_le              {0};       // number of little-endian aligned data points that are copied over
    uint64_t            set_aligned_be              {0};       // number of    big-endian aligned data points that are copied over 
    uint64_t            set_nearest_neighbor_le     {0};       // number of little-endian aligned data points that are copied over using nearest neighbor
    uint64_t            set_nearest_neighbor_be     {0};       // number of    big-endian aligned data points that are copied over using nearest neighbor
    uint64_t            set_interpolated_ble        {0};       // number of    big-endian aligned data points that are copied over using nearest neighbor 

    uint64_t            voidpoints                  {0};       // total number of void points filled 
    uint64_t            emptypoints                 {0};       // total number of empty points filled  
}; 


// MACRO to access points in the raster height buffer -- row/col input

#define M_raster_ht(b,r,c)    (((b).buf.p )[(int64_t)(r) * (int64_t)((b).grid.cols) + (int64_t)(c)])



// parms for create_raster() and create_auto_raster()
// --------------------------------------------------

struct create_raster_parm_S
{
  lld_T            lat_s             { 0     } ;        // nominal south edge of elevation data in raster -- in degrees
  lld_T            lon_w             { 0     } ;        // nominal west  edge of elevation data in raster -- in degrees 
  lld_T            deg_ns            { 0     } ;        // nominal n-s extent of elevation data in raster -- in degrees
  lld_T            deg_ew            { 0     } ;        // nominal e-w extent of elevation data in raster -- in degrees
  lld_T            window_ns         { 1     } ;        // nominal n-s extent of raster-filling window    -- in degrees
  lld_T            window_ew         { 1     } ;        // nominal e-w extent of raster-filling window    -- in degrees
  int32_t          ppd_ns            { 0     } ;        // pixels per degree N-S in the raster
  int32_t          ppd_ew            { 0     } ;        // pixels per degree E-W in the raster
  bool             extra_row_col     { true  } ;        // raster has one extra row and column (like some .hgt files) -- implies aligned (vs centered) pixels for rasters (but not DEM files) -- note: raster always little-endian
  bool             display           { false } ;        // display debug data for raster creation 
};


// parms for fill_raster() and selective_fill_raster()
// ---------------------------------------------------

struct fill_raster_parm_S
{
  bool             s_to_n            { true  } ;        // fill starting from south edge of raster -- ignored for fill_raster()
  bool             n_to_s            { false } ;        // fill starting from north edge of raster -- ignored for fill_raster()
  bool             interpolate       { true  } ;        // true: raster point interpolated between nearest neighbors in DEM file,      false: use nearest neighbor
  bool             base_fill         { false } ;        // true: unconditional fill of raster points from DEM file data
  bool             empty_fill        { false } ;        // true: fill only missing/unfilled points in raster from DEM file data
  bool             void_fill         { false } ;        // true: fill any void points in raster from DEM file data
  bool             display           { false } ;        // display debug data for raster fill 
  bool             display_filenames { false } ;        // display DEM filenames for raster fill 
  bool             debug_tile        { false } ;        // display tile-oriented debug messages 
};


// parms for writeout_raster()
// ---------------------------

struct writeout_raster_parm_S
{
  bool             suppress_empty    { true  } ;        // true = suppress output file write, if this file would be completely empty (all heights = RASTER_EMPTY) 
  bool             set_empty_height  { false } ;        // true = replace RSATER_EMPTY in output file with empty_height (see below)
  bool             display           { false } ;        // display debug data for raster fill 
  bool             display_filenames { false } ;        // display DEM filenames for raster fill 
  bool             debug_tile        { false } ;        // display tile-oriented debug messages 
  ht_T             empty_height      { 0     } ;        // if set_empty_height = true, replace all RASTER_EMPTY values with this height in output files 
};
 


//---------------------------------------------------------------------------------------------------------------------
//     raster_S function prototypes
//---------------------------------------------------------------------------------------------------------------------

raster_S *find_raster(   int64_t   );      // locate raster in repository with passed-in ID
raster_S *alloc_raster(  int64_t   );      // make sure raster is allocated in repository for passed-in ID (passes back simple pointer address of raster_S) 
void      free_raster(   int64_t   );      // make sure raster is not allocated in repository for passed-in ID (caller should get rid of everything in raster_S first)   

int      create_raster(                raster_S&, const create_raster_parm_S&                                                 );
int      create_auto_raster(           raster_S&, const create_raster_parm_S&,   const mapdata_S&                             );
void     display_raster(         const raster_S&                                                                              ); 
void     destroy_raster(               raster_S&                                                                              ); 
void     fill_raster(                  raster_S&, const fill_raster_parm_S&,                                const fileinfo_S& );  
void     smooth_raster(                raster_S&,  int32_t                                                                    );  
void     selective_fill_raster(        raster_S&, const fill_raster_parm_S&,     const mapdata_S&,          const fileinfo_S& ); 
int      writeout_raster(              raster_S&, const writeout_raster_parm_S&,                            const fileinfo_S& );  

void     get_raster_nearest(     const raster_S&, llr_T, llr_T, ht_T&, bool = false);
void     get_raster_interpolate( const raster_S&, llr_T, llr_T, ht_T&, bool = false);

void     slice_raster(           const raster_S&, thpa_S&, int32_t);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////