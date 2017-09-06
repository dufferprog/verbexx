// h_map_grid.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ============
////            h_map_grid.h -- grid_S and functions  
////            ============
//// 
////     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_map_types.h"
     

//---------------------------------------------------------------------------------------------------------------------
//     Georeferencing grid info for one tile, or whole raster
//---------------------------------------------------------------------------------------------------------------------

struct grid_S
{
    rc_T      cols              {0   };        // number of columns of data
    rc_T      rows              {0   };        // number of rows of data
    uint64_t  npts              {0   };        // number of points = rows * columns 
   
    bool      extra_row_col     {true};        // true  = has one extra row/col with overlapping data for next tile (nominal edges are edge pixel centers)
                                               // false = has no extra row/col  with overlapping data for next tile (nominal edges are at edges of 1st/last row/col pixels)  
    bool      pixels_aligned    {true};        // true  = pixel centers are aligned with grid        -- usually implies one extra row/rol
                                               // false = pixel centers are centered in grid squares -- usually implies no extra row/col  
    bool      pixels_centered   {true};        // false = pixel centers are aligned with grid        -- usually implies one extra row/rol
                                               // true  = pixel centers are centered in grid squares -- usually implies no extra row/col  

    // nominal "edges" of raster 
                                
    ll_T      south             {0   };        // south limit        -- degrees * 3600 (nominal)
    ll_T      west              {0   };        // west  limit        -- degrees * 3600 (nominal)
    ll_T      north             {0   };        // north limit        -- degrees * 3600 (nominal)
    ll_T      east              {0   };        // east  limit        -- degrees * 3600 (nominal)
    ll_T      extent_ns         {0   };        // north-south extent -- degrees * 3600 (nominal) 
    ll_T      extent_ew         {0   };        // east-west   extent -- degrees * 3600 (nominal)                                
                                
    llr_T     s                 {0.0 };        // floating point value for south/3600     (nominal) (degrees)
    llr_T     w                 {0.0 };        // floating point value for west/3600      (nominal) (degrees)
    llr_T     n                 {0.0 };        // floating point value for north/3600     (nominal) (degrees)
    llr_T     e                 {0.0 };        // floating point value for north/3600     (nominal) (degrees)
    llr_T     ns                {0.0 };        // floating point value for extent_ns/3600 (nominal) (degrees)
    llr_T     ew                {0.0 };        // floating point value for extent_ew/3600 (nominal) (degrees)                               
                                
    int32_t   pts_per_deg_ns    {0   };        // number of data points per degree - n-s direction 
    int32_t   pts_per_deg_ew    {0   };        // number of data points per degree - e-w direction
    real_T    ppd_ns            {0.0 };        // floating pt value for pts_per_deg_ns 
    real_T    ppd_ew            {0.0 };        // floating pt value for pts_per_deg_ew
    real_T    spp_ns            {0.0 };        // floating pt value for arcsec/pixel N-S = 3600.0/ppd_ns 
    real_T    spp_ew            {0.0 };        // floating pt value for arcsec/pixel E-W = 3600.0/ppd_ew 
    real_T    dpp_ns            {0.0 };        // degrees per pixel -- N-S  
    real_T    dpp_ew            {0.0 };        // degrees per pixel -- E-W 


    // true grid/raster limits, dependent on extra_row_col setting and nominal limits

    llr_T     offset_ns         {0.0 };        // N-S offset of pixel centers from nominal latitude (degrees)                
    llr_T     offset_ew         {0.0 };        // E-W offset of pixel centers from nominal latitude (degrees)               

    llr_T     s_pixel           {0.0 };        // latitude of pixel centers in bottom row (degrees)
    llr_T     n_pixel           {0.0 };        // latitude of pixel centers in top row  (degrees)
    llr_T     w_pixel           {0.0 };        // longitude of pixel centers in left column (degrees)
    llr_T     e_pixel           {0.0 };        // longitude of pixel centers in right column (degrees)

    llr_T     s_edge            {0.0 };        // latitude of pixel bottoms in bottom row (degrees)
    llr_T     n_edge            {0.0 };        // latitude of pixel tops in top row  (degrees)
    llr_T     w_edge            {0.0 };        // longitude of pixel left edges in left column (degrees)
    llr_T     e_edge            {0.0 };        // longitude of pixel right_edge in right column (degrees)
};


// MACROs to compute row/col in grid (raster/hgt/etc.), given the latitude or longitude   -- l = degrees (real_T = llr_T) 

#define M_grid_row(g, l) M_round( ((g).n_pixel - (l)) * (g).ppd_ns ) 
#define M_grid_col(g, l) M_round( ((l) - (g).w_pixel) * (g).ppd_ew ) 

#define M_grid_row_d(g, l) ( ((g).n_pixel - (l)) * (g).ppd_ns ) 
#define M_grid_col_d(g, l) ( ((l) - (g).w_pixel) * (g).ppd_ew ) 


//---------------------------------------------------------------------------------------------------------------------
//   grid_S function prototypes
//---------------------------------------------------------------------------------------------------------------------

int      adjust_longitude(       const   grid_S&, llr_T, llr_T&);  // grid versions only -- also see h.tile_h
int      adjust_longitude(       const   grid_S&, ll_T,  ll_T& );  // grid versions only -- also see h_tile.h

int      init_grid(                      grid_S&, ll_T, ll_T, ll_T, ll_T, int32_t, int32_t, bool, bool);
int      reposition_grid(                grid_S&, ll_T, ll_T                                          );
void     display_grid(           const   grid_S&                                                      );

int      get_neighbor_row_col(   const   grid_S&, llr_T,  llr_T, rcf_T&, rcf_T&, rc_T&, rc_T&, rc_T&, rc_T&, rc_T&, rc_T&);
int      get_neighbor_row_col(   const   grid_S&,                rcf_T , rcf_T , rc_T&, rc_T&, rc_T&, rc_T&, rc_T&, rc_T&);

int      get_interpolation_weights(rcf_T, rcf_T, rc_T, rc_T, rc_T, rc_T, real_T&, real_T&, real_T&, real_T&);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////