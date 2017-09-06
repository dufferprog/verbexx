// map_grid.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ============
////     map_grid.cpp -- functions for the grid_S structure 
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
#include "h_map_grid.h"



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

// llr_T version with grid_S --- also see .tile.cpp for tile_S grid versions

int adjust_longitude(const grid_S& grid, llr_T lat, llr_T& parm_lon) try
{
    llr_T lon = parm_lon;


    // normalize longitude first (-180.0 to +180.0)

    lon = M_fix_lr(lon);

    
    // try to correct out-of bounds input longitude to bring within nominal raster range, by adding or subtracting 360.0 degrees
       
    if      (lon > grid.e) lon -= 360.0;        
    else if (lon < grid.w) lon += 360.0;
   
    // if latitude or longitude is still out of nominal bounds, return -1 

    if ( (lon < grid.w) || (lon > grid.e) || (lat < grid.s) || (lat > grid.n) )
    {
        return -1;   
    }

    // pass back adjusted logitude (latitude is not adjusted, but is used in range check)

    parm_lon = lon; 

    return 0; 

}
M_endf



//// ll_T version with grid_S --- also see .tile.cpp for tile_S grid versions

int adjust_longitude(const grid_S& grid, ll_T lat, ll_T& parm_lon) try
{
        ll_T lon = parm_lon;


    // normalize longitude first (-180.0 to +180.0)

    lon = M_fix_l(lon);

    
    // try to correct out-of bounds input longitude to bring within nominal raster range, by adding or subtracting 360 degrees
       
    if      (lon >  grid.east) lon -= M_arcsec_360;        
    else if (lon <  grid.west) lon += M_arcsec_360;
   

    // if latitude or longitude is still out of nominal bounds, return -1 

    if ( (lon < grid.west) || (lon > grid.east) || (lat < grid.south) || (lat > grid.north) )
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
//||       get_neighbor_row_col() -- given lat/lon and a grid, compute row/column nearest r/c, and neighbor r/c         
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ----------------------------
// version with grid + lat/lon
// ----------------------------


int get_neighbor_row_col(const grid_S& grid, llr_T lat, llr_T lon, rcf_T& row_f, rcf_T& col_f, rc_T& row, rc_T& col, rc_T& r1, rc_T& r2, rc_T& c1, rc_T& c2) try
{
    // get closest row/col, and also floating pt row/col coordinates -- can still be slightly out of bounds if lat/lon are just outside range of pixel centers           
       
    row_f = M_grid_row_d(grid, lat);       // use for later interpolation 
    col_f = M_grid_col_d(grid, lon);       // use this value for interpolation

    return get_neighbor_row_col(grid, row_f, col_f, row, col, r1, r2, c1, c2); 
   
}
M_endf


// ----------------------------
// version with grid + row_f/col_f already calculated intead of lat/lon
// ----------------------------


int get_neighbor_row_col(const grid_S& grid, rcf_T row_f, rcf_T col_f, rc_T& row, rc_T& col, rc_T& r1, rc_T& r2, rc_T& c1, rc_T& c2) try
{
    // get closest row/col, and also floating pt row/col coordinates -- can still be slightly out of bounds if lat/lon are just outside range of pixel centers           
       
 
    row   = M_round(row_f);                // always nearest neighbor for attributes, etc.
    row   = M_lim(0, row, grid.rows - 1);  // make sure row is within valid range


    col   = M_round(col_f);                // use nearest neighbor for attributes, etc.  
    col   = M_lim(0, col, grid.cols - 1);  // make sure col is within valid range


    // compute two row numbers for use in interpolation -- can be same if row_d is past the center of the first/last pixel 

    if (row_f <= 0.0)
    {
        r1 = 0; 
        r2 = 0;     
    }
    else
    {
      if (row_f >= grid.rows - 1)
      {
          r1 = grid.rows - 1; 
          r2 = grid.rows - 1; 
      }
      else
      {
          r1 = M_round_toward_zero(row_f);
          r2 = M_round_away_zero(row_f);      
      }
    }


    // compute two column numbers for use in interpolation -- can be same if col_d is past the center of the first/last pixel 

    if (col_f <= 0.0)
    {
        c1 = 0; 
        c2 = 0;     
    }
    else
    {
      if (col_f >= grid.cols - 1)
      {
          c1 = grid.cols - 1; 
          c2 = grid.cols - 1; 
      }
      else
      {
          c1 = M_round_toward_zero(col_f);
          c2 = M_round_away_zero(col_f);      
      }
    }

    return 0; 

}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_interpolation_weights() -- given row/col and integral neighbor rows/cols, compute interpolation weights         
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int get_interpolation_weights(rcf_T row_f, rcf_T col_f, rc_T r1, rc_T r2, rc_T c1, rc_T c2, real_T& f11, real_T& f12, real_T& f21, real_T& f22) try
{
     real_T   rf1, rf2, cf1, cf2;    // row/col subfactors


    // compute row subfactors for interpolation 

    if (r2 != r1)
       rf2 = (row_f - (real_T)r1) /  (real_T)(r2 - r1);
    else
       rf2 = 0.5;                           // rows are same (exact hit, or just past edge pixel center) -- just take half and half of same row
      
    rf1 = 1.0 - rf2; 


    // compute column subfactors for interpolation 
    
    if (c2 != c1)
       cf2 = (col_f - (real_T)c1) /  (real_T)(c2 - c1);
    else
       cf2 = 0.5;                           // columns are same (exact hit, or just past edge pixel center) -- just take half and half of same column
                   
    cf1 = 1.0 - cf2;
   

    // compute interpolation weights based on row and column subfactors

    f11 = rf1 * cf1;  
    f12 = rf1 * cf2;
    f21 = rf2 * cf1; 
    f22 = rf2 * cf2; 
    
    return 0; 

}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       init_grid() -- fill in grid info based on caller's parms
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int init_grid(grid_S& grid, ll_T south, ll_T west, ll_T extent_ns, ll_T extent_ew, int32_t pts_per_deg_ns, int32_t pts_per_deg_ew, bool extra_row_col, bool pixels_aligned) try
{
    int rc = 0; 
      
    M__(M_out(L"init_grid(): south=%d  west=%d  extent_ns=%d  extent_ew=%d pts_per_deg_ns=%d  pts_per_deg_ew=%d extra_row_col=%s") % south % west % extent_ns % extent_ew % pts_per_deg_ns % pts_per_deg_ew % M_bool_cstr(extra_row_col);)

    grid.extra_row_col   = extra_row_col;
    grid.pixels_aligned  = pixels_aligned;
    grid.pixels_centered = !pixels_aligned;


    // set basic( nominal) grid limits directly from input parms

    grid.extent_ns      = extent_ns;
    grid.extent_ew      = extent_ew;
    grid.south          = south;
    grid.west           = west;
    grid.pts_per_deg_ns = pts_per_deg_ns; 
    grid.pts_per_deg_ew = pts_per_deg_ew; 


    // compute other limits, etc.

    grid.north          = south + extent_ns;  
    grid.east           = west  + extent_ew;
     
    grid.s              = M_l_to_lr(grid.south     );
    grid.w              = M_l_to_lr(grid.west      );
    grid.n              = M_l_to_lr(grid.north     );
    grid.e              = M_l_to_lr(grid.east      );
    grid.ns             = M_l_to_lr(grid.extent_ns );
    grid.ew             = M_l_to_lr(grid.extent_ew );

    grid.ppd_ns         = (real_T)(grid.pts_per_deg_ns);
    grid.ppd_ew         = (real_T)(grid.pts_per_deg_ew);
    grid.dpp_ns         = 1.0 / grid.pts_per_deg_ns;
    grid.dpp_ew         = 1.0 / grid.pts_per_deg_ew;

    grid.spp_ns         = 3600.0 / grid.ppd_ns;
    grid.spp_ew         = 3600.0 / grid.ppd_ew;


    // compute rows, columns and number of points
        
    grid.cols = (int32_t)(((uint64_t)(grid.pts_per_deg_ew) * (uint64_t)(grid.extent_ew)) / (uint64_t)3600);
    grid.rows = (int32_t)(((uint64_t)(grid.pts_per_deg_ns) * (uint64_t)(grid.extent_ns)) / (uint64_t)3600);

    if (extra_row_col)
    {
        grid.cols ++; 
        grid.rows ++; 
    }

    grid.npts           = (uint64_t)(grid.cols) * (uint64_t)(grid.rows); 


    // compute offsets, edges, pixel centers, etc. -- based on extra row/col flag setting   ??????? need to check extra row/col and also aligned/centered pixels
    // ----------------------------------------------------------------------------------

    if (extra_row_col)
    {
         // pixel centers are aligned with nominal grid  

         grid.offset_ns = 0.0; 
         grid.offset_ew = 0.0; 

         grid.s_pixel   = grid.s; 
         grid.w_pixel   = grid.w;
         grid.n_pixel   = grid.n; 
         grid.e_pixel   = grid.e;

         // Pixel edges expand outward 0.5 pixel, if there is an extra row/column 

         grid.s_edge    = grid.s - 0.5 * grid.dpp_ns;    // can go past -90.0  
         grid.w_edge    = grid.w - 0.5 * grid.dpp_ew;   
         grid.n_edge    = grid.n + 0.5 * grid.dpp_ns;    // can go past +90.0
         grid.e_edge    = grid.e + 0.5 * grid.dpp_ew;   
    }
    else       // no extra_row
    {
         // limiting pixel centers are adjusted inward 0.5 pixel ( = offset)

         grid.offset_ns = 0.5 * grid.dpp_ns; 
         grid.offset_ew = 0.5 * grid.dpp_ew;  

         grid.s_pixel   = grid.s + grid.offset_ns; 
         grid.w_pixel   = grid.w + grid.offset_ew;
         grid.n_pixel   = grid.n - grid.offset_ns; 
         grid.e_pixel   = grid.e - grid.offset_ew;

         // Edges are same as nominal values,  when there is no extra row/column 

         grid.s_edge    = grid.s; 
         grid.w_edge    = grid.w;
         grid.n_edge    = grid.n; 
         grid.e_edge    = grid.e;
    }

    return rc;        
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       display_grid() -- display grid values for debugging
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_grid(const grid_S& grid) try
{   
    M_out(L"grid.south                     = %-7d (%.16f deg)" )   % grid.south           % M_l_to_lr(grid.south    ) ;
    M_out(L"grid.west                      = %-7d (%.16f deg)" )   % grid.west            % M_l_to_lr(grid.west     ) ;
    M_out(L"grid.north                     = %-7d (%.16f deg)" )   % grid.north           % M_l_to_lr(grid.north    ) ;
    M_out(L"grid.east                      = %-7d (%.16f deg)" )   % grid.east            % M_l_to_lr(grid.east     ) ;
    M_out(L"grid.extent_ns                 = %-7d (%.16f deg)" )   % grid.extent_ns       % M_l_to_lr(grid.extent_ns) ;
    M_out(L"grid.extent_ew                 = %-7d (%.16f deg)" )   % grid.extent_ew       % M_l_to_lr(grid.extent_ew) ;
    M_out(L"grid.pts_per_deg_ns            = %d"               )   % grid.pts_per_deg_ns                              ;
    M_out(L"grid.pts_per_deg_ew            = %d"               )   % grid.pts_per_deg_ew                              ;
                                                                                                            
    M_out(L"grid.extra_row_col             = %s"               )   % M_bool_cstr(grid.extra_row_col)                  ;
    M_out(L"grid.pixels_aligned            = %s"               )   % M_bool_cstr(grid.pixels_aligned)                 ;
    M_out(L"grid.pixels_centered           = %s"               )   % M_bool_cstr(grid.pixels_centered)                ;
    M_out(L"grid.cols                      = %d"               )   % grid.cols                                        ;
    M_out(L"grid.rows                      = %d"               )   % grid.rows                                        ;
    M_out(L"grid.npts                      = %s"               )   % M_sep(grid.npts)                                 ;
                                                                                                             
    M_out(L"grid.s                         = %.20f"            )   % grid.s                                           ;
    M_out(L"grid.w                         = %.20f"            )   % grid.w                                           ;
    M_out(L"grid.n                         = %.20f"            )   % grid.n                                           ; 
    M_out(L"grid.e                         = %.20f"            )   % grid.e                                           ; 
    M_out(L"grid.ns                        = %.20f"            )   % grid.ns                                          ; 
    M_out(L"grid.ew                        = %.20f"            )   % grid.ew                                          ; 
    M_out(L"grid.ppd_ns                    = %.20f"            )   % grid.ppd_ns                                      ; 
    M_out(L"grid.ppd_ew                    = %.20f"            )   % grid.ppd_ew                                      ; 
    M_out(L"grid.dpp_ns                    = %.20f"            )   % grid.dpp_ns                                      ; 
    M_out(L"grid.dpp_ew                    = %.20f"            )   % grid.dpp_ew                                      ;
    M_out(L"grid.spp_ns                    = %.20f"            )   % grid.spp_ns                                      ; 
    M_out(L"grid.spp_ew                    = %.20f"            )   % grid.spp_ew                                      ;
                                                                                                           
    M_out(L"grid.offset_ns                 = %.20f"            )   % grid.offset_ns                                   ;
    M_out(L"grid.offset_ew                 = %.20f"            )   % grid.offset_ew                                   ;
    M_out(L"grid.s_pixel                   = %.20f"            )   % grid.s_pixel                                     ; 
    M_out(L"grid.w_pixel                   = %.20f"            )   % grid.w_pixel                                     ; 
    M_out(L"grid.n_pixel                   = %.20f"            )   % grid.n_pixel                                     ; 
    M_out(L"grid.e_pixel                   = %.20f"            )   % grid.e_pixel                                     ; 
    M_out(L"grid.s_edge                    = %.20f"            )   % grid.s_edge                                      ; 
    M_out(L"grid.w_edge                    = %.20f"            )   % grid.w_edge                                      ; 
    M_out(L"grid.n_edge                    = %.20f"            )   % grid.n_edge                                      ; 
    M_out(L"grid.e_edge                    = %.20f"            )   % grid.e_edge                                      ;    

    return;       
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       reposition_grid() -- alter positioning (but not size/spacing) of grid
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int reposition_grid(grid_S& grid, ll_T south, ll_T west) try
{
    int rc = 0; 
      

    // update basic( nominal) grid limits directly from input parms

    grid.south          = south;
    grid.west           = west;


    // compute other limits, etc.

    grid.north          = south + grid.extent_ns;  
    grid.east           = west  + grid.extent_ew;
     
    grid.s              = M_l_to_lr(grid.south);
    grid.w              = M_l_to_lr(grid.west );
    grid.n              = M_l_to_lr(grid.north);
    grid.e              = M_l_to_lr(grid.east );

    
    // compute offsets, edges, pixel centers, etc. -- based on extra row/col flag setting
    // ----------------------------------------------------------------------------------

    if (grid.extra_row_col)
    {
         // pixel centers are aligned with nominal grid  

         grid.s_pixel   = grid.s; 
         grid.w_pixel   = grid.w;
         grid.n_pixel   = grid.n; 
         grid.e_pixel   = grid.e;

         // Edges expand outward 0.5 pixel, if there is an extra row/column 

         grid.s_edge    = grid.s - 0.5 * grid.dpp_ns;    // can go past -90.0  
         grid.w_edge    = grid.w - 0.5 * grid.dpp_ew;   
         grid.n_edge    = grid.n + 0.5 * grid.dpp_ns;    // can go past +90.0
         grid.e_edge    = grid.e + 0.5 * grid.dpp_ew;   
    }
    else       // no extra_row
    {
         // limiting pixel centers are adjusted inward 0.5 pixel ( = offset)

         grid.s_pixel   = grid.s + grid.offset_ns; 
         grid.w_pixel   = grid.w + grid.offset_ew;
         grid.n_pixel   = grid.n - grid.offset_ns; 
         grid.e_pixel   = grid.e - grid.offset_ew;

         // Edges are same as nominal values,  when there is no extra row/column 

         grid.s_edge    = grid.s; 
         grid.w_edge    = grid.w;
         grid.n_edge    = grid.n; 
         grid.e_edge    = grid.e;
    }
    
    return rc;        
}
M_endf



//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""