// map_raster.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ==============
////     map_raster.cpp -- functions for the raster_S structure 
////     ==============
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
#include "h_map_thd.h"
#include "h_map_parms.h" 
#include "h_map_dem.h"
#include "h_map_grid.h"
#include "h_map_raster.h"
#include "h_map_data.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// internal persistent work area layout -- used for performance across numerous calls in loop   ???????????????????????? why not just the buffer_C ???????
// ------------------------------------------------------------------------------------------

struct workarea_S
{
    buffer_C<rc_T> buf; 
}; 



// ==================================================================================================================
//                                       Static variables
// ==================================================================================================================

namespace static_N
{

// raster_S repository
// -------------------

static std::map<int64_t, std::shared_ptr<raster_S>> rasters { }; 

// ------------------------------------------------------------------------------------- 

}
    


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






// ----------------------------
// Internal function prototypes
// ----------------------------

static void create_raster_data(   raster_S&                                                                                                                );
static void addin_raster(         raster_S&,        hgt_S&, ll_T, ll_T, ll_T, ll_T, const     fill_raster_parm_S&, const fileinfo_S& fileinfo, workarea_S& );
static int  writeout_file(        raster_S&,        hgt_S&, ll_T, ll_T, ll_T, ll_T, const writeout_raster_parm_S&, const fileinfo_S& fileinfo              );
static void set_raster_data(      raster_S&,  const hgt_S&, ll_T, ll_T, ll_T, ll_T, const     fill_raster_parm_S&, const fileinfo_S&         , workarea_S& );
static void get_raster_data(      raster_S&,        hgt_S&, ll_T, ll_T, ll_T, ll_T, const writeout_raster_parm_S&, const fileinfo_S&                       );
static int  check_raster_data(    raster_S&,                ll_T, ll_T, ll_T, ll_T, const     fill_raster_parm_S&                                          );
static void refill_raster_data(   raster_S&,  const hgt_S&,                         const     fill_raster_parm_S&                                          );

static int  check_window(         const grid_S&, ll_T, ll_T, const std::wstring&); 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        find_raster() -- find raster(n) in repository (return nullptr, if not there)  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

raster_S* find_raster(int64_t raster_id) try
{
    // make sure there is exactly one raster in repository with caller's ID
    // --------------------------------------------------------------------

     auto raster_ct = static_N::rasters.count(raster_id); 

     if (raster_ct <= 0)
     {
         M__(M_out(L"find_raster() -- raster(%d) not in repository") % raster_id;) 
         return nullptr; 
     }
     else
     if (raster_ct > 1)
     {
         M_out_emsg(L"find_raster() -- invalid rasters.count(%d) = %d") % raster_id % raster_ct;  
         return nullptr; 
     }


     // one shared_ptr<raster_S> found -- return raster_S address as simple pointer
     // ---------------------------------------------------------------------------

     M__(M_out(L"find_raster() -- raster(%d) at %p found in repository") % raster_id % static_N::rasters.at(raster_id).get();) 
     return static_N::rasters.at(raster_id).get();
}
M_endf 




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        alloc_raster() -- make sure raster is allocated in repository for this ID  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

raster_S* alloc_raster(int64_t raster_id) try
{
    // pass back any existing raster_S in repository
    // ---------------------------------------------

    raster_S *p = find_raster(raster_id);

    if (p != nullptr)
    {
        M__(M_out(L"alloc_raster() -- existing raster(%d) in repository at %p being returned") % raster_id % p;) 
        return p;
    }


    // need to allocate new (default initialized) raster_S and add to the repository
    //------------------------------------------------------------------------------

    std::shared_ptr<raster_S> sp(new raster_S);     // this sp should go away when function returns
    static_N::rasters.emplace(raster_id, sp);       // this sp should stay around until this ID is deallocated

    M__(M_out(L"alloc_raster() -- new raster(%d) was allocated at %p and added to repository") % raster_id % sp.get();)


    // pass back simple address of newly-allocated raster_S

    return sp.get();                                        
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        free_raster() -- make sure no raster is allocated in repository for this ID  
//|| 
//||                           (caller must have gotten rid of everything in the raster_S before calling) 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void free_raster(int64_t raster_id) try
{
    // get rid of any existing raster_S in repository
    // ----------------------------------------------

    if (static_N::rasters.count(raster_id) > 0)
    {
        M__(M_out(L"free_raster() -- raster(%d) being erased from repository") % raster_id;)
        static_N::rasters.erase(raster_id);   
    }
    else
    {
        M__(M_out(L"free_raster() -- raster(%d) not allocated in repository") % raster_id;)
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
//||       get_raster_interpolate() -- given lat/lon, give back info (interpolated results)         
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void get_raster_interpolate(const raster_S& raster, llr_T parm_lat, llr_T parm_lon, ht_T& ht, bool complain_if_out) try
{
    //          c1      col_d      c2
    //         +------------------------
    //         |  
    //     r1  |  h11               h21                    
    //  row_d  |           h                  
    //     r2  |  h12               h22                            

    ht_T      h, h11, h12, h21, h22; 
    rc_T      row, col, r1, r2, c1, c2;

    rcf_T    row_f, col_f;
    real_T   f11, f12, f21, f22;
    llr_T    lat,lon; 

    
    // try to correct out-of bounds input longitude to bring within nominal raster range, by adding or subtracting 360.0

    lat = parm_lat; 
    lon = parm_lon;
    int rc = adjust_longitude(raster.grid, lat, lon); 

    if (rc != 0)     // can't adjust to be in-bounds
    {
        if (complain_if_out)
            M_out(L"get_raster_interpolate() -- input latitude/longitude is out of raster bounds -- lat=%.20f  lon=%.20f") % lat % lon; 

        ht = RASTER_OUT; 
        return;   
    }
    

    // note:: computed row, col can still be slightly out of pixel bounds, if raster.extra_row_col flag is false  
    // get closest row/col, neighbors, and and also floating pt row/col coordinates    

    get_neighbor_row_col(raster.grid, lat, lon, row_f, col_f, row, col, r1, r2, c1, c2);
      

    // check nearest neighbor point for void or empty -- no interpolation if so

    h = M_raster_ht(raster, row, col);       // interim h for testing or use if abnormal point or other problems

    if ( (h == RASTER_VOID) || (h == RASTER_EMPTY) )  
    {
       ht = h;  
       return;   
    }

    M__(M_out(L"row_f/col_f=%.6f/%.6f   row/col=%d/%d   ht=%d") % row_f % col_f % row % col % (int)h;)


    // get 1/2/4 heights for interpolation -- any of these may be abnormal

    h11 = M_raster_ht(raster, r1, c1); 
    h12 = M_raster_ht(raster, r1, c2); 
    h21 = M_raster_ht(raster, r2, c1); 
    h22 = M_raster_ht(raster, r2, c2);        

    M__(M_out(L"r1/r2=%d/%d  c1/c2=%d/%d  h=%d/%d/%d/%d") % r1 % r2 % c1 % c2 % (int)h11 % (int)h12 % (int)h21 % (int)h22;)
        

    // bypass interpolation if any of these 1/2/4 heights are not normal height values

    if (
        ( (h11 == RASTER_VOID) || (h11 == RASTER_EMPTY) )
        ||
        ( (h12 == RASTER_VOID) || (h12 == RASTER_EMPTY) )
        ||
        ( (h21 == RASTER_VOID) || (h21 == RASTER_EMPTY) )
        ||
        ( (h22 == RASTER_VOID) || (h22 == RASTER_EMPTY) )
       )
    {
        ht = h;       // no interpolation -- just pass back interim value
        return;    
    }


    // compute interpolated height based on neighbors

    get_interpolation_weights(row_f, col_f, r1, r2, c1, c2, f11, f12, f21, f22);      // sets f11, f12, f21, f22

    ht = (int16_t)M_round(f11 * (real_T)h11   +   f12 * (real_T)h12   +   f21 * (real_T)h21   +   f22 * (real_T)h22);

    M__(M_out(L"f11=%-10.6f  f12=%-10.6f f21=%-10.6f  f22=%-10.6f  sum=%-10.6f   ht=%d") % f11 % f12 % f21 % f22 % (f11 + f12 + f21 + f22) % (int)ht;); 
 
    return; 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_raster_nearest() -- given lat/lon, give back info (closest row/column result)         
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void get_raster_nearest(const raster_S& raster, llr_T parm_lat, llr_T parm_lon, ht_T& ht, bool complain_if_out) try
{
    ht_T     h;
    rc_T     row, col;
    llr_T    lat,lon; 
    llr_T    west, east; 

  
    // try to correct out-of bounds input longitude to bring within nominal raster range, by adding or subtracting 360.0

    lat = parm_lat; 
    lon = parm_lon;
    int rc = adjust_longitude(raster.grid, lat, lon); 

    if (rc != 0)     // can't adjust to be in-bounds
    {
       if (complain_if_out)
           M_out(L"get_raster_nearest() -- input latitude/longitude is out of raster bounds -- lat=%.20f  lon=%.20f") % lat % lon; 

       ht = RASTER_OUT; 
       return;   
    }


   // note:: computed row, col can still be slightly out of pixel bounds, if raster.extra_row_col flag is false   
    

   // get closest row/col, making sure to exclude values +-1 past edges           
       

    row   = M_grid_row(  raster.grid, lat);       // nearest row
    row   = M_lim(0, row, raster.grid.rows - 1);  // make sure row is within valid range

    col   = M_grid_col(  raster.grid, lon);       // nearest column  
    col   = M_lim(0, col, raster.grid.cols - 1);  // make sure col is within valid range


    // get height value from nearest neightbor row
    
    ht = M_raster_ht(raster, row, col);             // use nearest neighbor for height
    return; 
}
M_endf






//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       slice_raster() -- partition the raster data for multi-threading 
//||
//||
//||        pp -> array of THREAD_MAX thp_t structures (output area -- filled in)
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void slice_raster(const raster_S& raster, thpa_S& thpa, int32_t max_threads, const mapparm_S& mapparm) try
{
    slice_row(thpa, raster.row_min, raster.row_max, raster.col_min, raster.col_max, max_threads, true, mapparm); 
    return;
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       fill_raster() -- add data from hgt_t structure into raster_t structure (whole raster) 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void fill_raster(raster_S& raster, const fill_raster_parm_S& fparm, const fileinfo_S& fileinfo) try
{
    hgt_S      hgt         { };                  // local hgt_T structure
    workarea_S workarea    { };                  // persistent work area


    // start timer
    // -----------

    std::chrono::steady_clock  clk        {           } ;                  
    auto                       tp0        { clk.now() } ;
    auto                       tp1        { tp0       } ;
    auto                       tp2        { tp1       } ;
    real_T                     elapsed    {           } ;
    real_T                     since_last {           } ;


    // initialize hgt_S before starting
    // --------------------------------

    create_hgt(hgt, fileinfo);            
     
    
    // ---------------------------------------------------------------------------------------------------------------
    // compute starting/stopping window limits to fill the raster (which must have been initialized with limits, etc.)
    // --------------------------------------------------------------------------------------------------------------
    
    // note -- it is assumed here that windows are small enough so that entire window can be filled by one (or part of one) input file  
    //      -- it is also assumed that window alignment in multiples of the size does not cause window vs tile alignment issues 

    ll_T step_ns   { raster.window_ns              } ;       
    ll_T step_ew   { raster.window_ew              } ;        
    ll_T lat_start { raster.grid.south             } ; 
    ll_T lat_stop  { raster.grid.north - step_ns   } ;        
    ll_T lon_start { raster.grid.west              } ; 
    ll_T lon_stop  { raster.grid.east  - step_ew   } ; 
 

    // round starting values down to nearest multiple of window size -- round ending values up (down = towards -infinity  up = towards +infinity)
    // ???????? is this still needed here -- should have already checked fixed raster limits vs specified window size
    
    M__(M_out(L"fill_raster() -- unrounded: lat_start=%d (%.16f deg)  lon_start=%d (%.16f deg)  lat_stop = %d (%.16f deg)  lon_stop=%d (%.16f deg)") 
             % lat_start   % M_l_to_lr(lat_start) 
             % lon_start   % M_l_to_lr(lon_start)
             % lat_stop    % M_l_to_lr(lat_stop) 
             % lon_stop    % M_l_to_lr(lon_stop)
             ;
       )
            
    lat_start = M_rounddown(lat_start, step_ns);
    lon_start = M_rounddown(lon_start, step_ew);
    lat_stop  = M_roundup(  lat_stop , step_ns);
    lon_stop  = M_roundup(  lon_stop , step_ew);
    
    M__(M_out(L"fill_raster()-- rounded  : lat_start=%d (%.16f deg)  lon_start=%d (%.16f deg)  lat_stop = %d (%.16f deg)  lon_stop=%d (%.16f deg)") 
             % lat_start   % M_l_to_lr(lat_start) 
             % lon_start   % M_l_to_lr(lon_start)
             % lat_stop    % M_l_to_lr(lat_stop) 
             % lon_stop    % M_l_to_lr(lon_stop)
             ;
       )


    // -----------------------------------------------------------------------------------------------
    // Read in .hgt files and add them into the raster area -- always add in data one window at a time  
    // -----------------------------------------------------------------------------------------------
        
    for (ll_T lat = lat_start; lat <= lat_stop; lat += step_ns)
    {
        for (ll_T lon = lon_start; lon <= lon_stop; lon += step_ew)
        {
            addin_raster(raster, hgt, lat, lon, step_ns, step_ew, fparm, fileinfo, workarea);                             

            // put out progress message if it's been 3 seconds or more since the last one

            tp2 = clk.now(); 
            since_last = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1)).count()) / 1000.0;

            raster.windows_done++; 
 
            if (since_last >= 3.0)
            {
                elapsed = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp0)).count()) / 1000.0;
                  
                M_out( L"Raster  : (%.3f sec) latitude = %+-03d   windows=%5u/%u   files: read-in=%-5u   reused=%u   missing=%-5u   files/sec = %.3f")
                     % elapsed
                     % M_l_to_ld(lat)                                      // show current window latitude in degrees
                     % raster.windows_done  
                     % ((raster.grid.extent_ew  / step_ew) * (raster.grid.extent_ns / step_ns))  
                     % raster.files_read
                     % raster.files_reused
                     % raster.files_missing
                     % (((real_T)(raster.files_read)) / elapsed)
                     ;
                 
                tp1 = tp2;      // reset last message time for next 3-second time                                  
            }
         
        }	 
    }
   

    //  end processing -- put out elapsed time and statistics
    //  -----------------------------------------------------

    elapsed = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp0)).count()) / 1000.0;
    
    M_out( L"Raster  : windows=%u (%.3f x %.3f deg)    files: read-in=%u reused=%u missing=%u total=%u    elapsed time = %.3f seconds   files/sec = %.3f")
         % raster.windows_done
         % M_l_to_lr(step_ns)
         % M_l_to_lr(step_ew)
         % raster.files_read
         % raster.files_reused
         % raster.files_missing
         % (raster.files_read + raster.files_reused + raster.files_missing)
         % elapsed
         % (((real_T)(raster.files_read)) / elapsed)
         ;
    M_out( L"Raster  : bytes read = %s (%.3fMB)    voidfills_needed = %u   voidfills_skipped = %u   voidpoints = %u    bytes/sec = %.1f (%.3f MB/sec)")
         % M_sep(raster.bytes_read)
         % M_meg(raster.bytes_read)
         % raster.voidfills_needed
         % raster.voidfills_not_needed
         % raster.voidpoints
         % (((real_T)(raster.bytes_read)) / elapsed)
         % (((real_T)(raster.bytes_read)) / (elapsed * 1024576.0))
         ;

    if (fparm.display)
    {
        M_out(L"Raster  : cols = %-5d  valid col_min = %-5d  valid col_max = %-5d") % raster.grid.cols % raster.col_min % raster.col_max;
        M_out(L"Raster  : rows = %-5d  valid row_min = %-5d  valid row_max = %-5d") % raster.grid.rows % raster.row_min % raster.row_max;
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
//||       selective_fill_raster() -- add data from hgt_t structure into raster_t structure (whole raster) 
//||                               -- check map and only add in those .hgt files that show on map    
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
                                            
void selective_fill_raster(raster_S& raster, const fill_raster_parm_S& fparm, const mapdata_S& mapdata, const fileinfo_S& fileinfo) try
{
     hgt_S      hgt       { } ;            // local hgt_t structure    
     workarea_S workarea  { } ;            // persistent work area  


     // start timer
     // -----------

     std::chrono::steady_clock  clk {                 } ;      
     auto                       tp0 { clk.now()       } ;
     auto                       tp1 { tp0             } ;
     auto                       tp2 { tp1             } ;
     real_T                     elapsed, since_last     ;
   

     // initialize local hgt_S area
     // ---------------------------
    
     create_hgt(hgt, fileinfo);    

    
     // •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
     // compute starting/stopping window limits to fill the raster
     // •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
     
     // note -- it is assumed here that windows are small enough so that entire window can be filled by one (or part of one) input file  
     //      -- it is also assumed that window alignment in multiples of the size does not cause window vs tile alignment issues 
     
     // set up to fetch files forwards or backwards (by latitude)
    
     int32_t  d         { 0                                   } ;                        // 1 = forward latitude step, -1 = backwards step
     ll_T     lat_start { 0                                   } ;
     ll_T     lat_stop  { 0                                   } ;
     ll_T     lat_step  { 0                                   } ;
     ll_T     lon_start { raster.grid.west                    } ;
     ll_T     lon_stop  { raster.grid.east - raster.window_ew } ;
    
    
     if (fparm.s_to_n)               // S to N (normal)
     {
         d         = 1; 
         lat_step  = raster.window_ns; 
         lat_start = raster.grid.south;
         lat_stop  = raster.grid.north - raster.window_ns;
     }
     else                          // fill direction should be N to S
     {
         d         = -1; 
         lat_step  = -raster.window_ns; 
         lat_start = raster.grid.north - raster.window_ns;
         lat_stop  = raster.grid.south;
     }


     // round starting values down to nearest multiple of window size -- round ending values up (down = towards -infinity  up = towards +infinity)
     
     M__(M_out(L"selective_fill_raster() -- unrounded: lat_start=%d (%.16f deg)  lon_start=%d (%.16f deg)  lat_stop = %d (%.16f deg)  lon_stop=%d (%.16f deg)") 
              % lat_start   % M_l_to_lr(lat_start) 
              % lon_start   % M_l_to_lr(lon_start)
              % lat_stop    % M_l_to_lr(lat_stop) 
              % lon_stop    % M_l_to_lr(lon_stop)
              ;
        )
            
     lat_start = M_rounddown(lat_start, raster.window_ns);
     lon_start = M_rounddown(lon_start, raster.window_ew);
     lat_stop  = M_roundup(  lat_stop , raster.window_ns);
     lon_stop  = M_roundup(  lon_stop , raster.window_ew);
    
     M__(M_out(L"selective_fill_raster() -- rounded  : lat_start=%d (%.16f deg)  lon_start=%d (%.16f deg)  lat_stop = %d (%.16f deg)  lon_stop=%d (%.16f deg)") 
              % lat_start   % M_l_to_lr(lat_start) 
              % lon_start   % M_l_to_lr(lon_start)
              % lat_stop    % M_l_to_lr(lat_stop) 
              % lon_stop    % M_l_to_lr(lon_stop)
              ;
        )

   
     
     // •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
     // Read in .hgt files and add them into the raster area -- assumes 1-degree .hgt files
     // •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
     
     // obtain map corner lat/lon for use below
     // ---------------------------------------

     llr_T c1_lat { };
     llr_T c1_lon { }; 
     llr_T c2_lat { };
     llr_T c2_lon { }; 
     llr_T c3_lat { };
     llr_T c3_lon { }; 
     llr_T c4_lat { };
     llr_T c4_lon { }; 

     auto g_rc1 = (*(mapdata.get_lat_lon_p))( mapdata, c1_lat, c1_lon, 0.0           , 0.0            , true);  
     auto g_rc2 = (*(mapdata.get_lat_lon_p))( mapdata, c2_lat, c2_lon, 0.0           , mapdata.maxcol , true ); 
     auto g_rc3 = (*(mapdata.get_lat_lon_p))( mapdata, c3_lat, c3_lon, mapdata.maxrow, 0.0            , true ); 
     auto g_rc4 = (*(mapdata.get_lat_lon_p))( mapdata, c4_lat, c4_lon, mapdata.maxrow, mapdata.maxcol , true ); 

     if (fparm.display)
     {
         M_out(L"selective_fill_raster(): corner1=%.6f/%.6f  corner2=%.6f/%.6f  corner3=%.6f/%.6f  corner4=%.6f/%.6f g_rc=%d %d %d %d") % c1_lat % c1_lon % c2_lat % c2_lon % c3_lat % c3_lon % c4_lat % c4_lon % g_rc1 % g_rc2 % g_rc3 % g_rc4; 
     }


     // ========================
     // main loop to fetch files
     // ======================== 

     for (ll_T lat = lat_start; d * lat <= d * lat_stop; lat += lat_step)
     {
         for (ll_T lon = lon_start; lon <= lon_stop; lon += raster.window_ew)
         {
             // see if any corner of this 1x1 degree window would show up on the final map -- this algorithm is questionable   -- replace random sample of N internal points + corners (and edges) ??

             real_T unwanted;      // actual output is not wanted

             auto rc1 = (*(mapdata.get_row_col_p))( mapdata, unwanted, unwanted, M_l_to_lr(lat                   ), M_l_to_lr(lon                  ), true );
             auto rc2 = (*(mapdata.get_row_col_p))( mapdata, unwanted, unwanted, M_l_to_lr(lat + raster.window_ns), M_l_to_lr(lon                  ), true );
             auto rc3 = (*(mapdata.get_row_col_p))( mapdata, unwanted, unwanted, M_l_to_lr(lat                   ), M_l_to_lr(lon + raster.window_ew), true );
             auto rc4 = (*(mapdata.get_row_col_p))( mapdata, unwanted, unwanted, M_l_to_lr(lat + raster.window_ns), M_l_to_lr(lon + raster.window_ew), true );

             if ( (rc1 == M_ROW_COL_RC_OK) || (rc2 == M_ROW_COL_RC_OK) || (rc3 == M_ROW_COL_RC_OK) || (rc4 == M_ROW_COL_RC_OK) )
             {
                 addin_raster(raster, hgt, lat, lon, raster.window_ns, raster.window_ew, fparm, fileinfo, workarea); 
             }
             else    // do further checking to see if this window needs filling
             {
                 // see if any corner of map lies within this NxN1 degree window 

                 if  ( 
                      ( c1_lat >= M_l_to_lr(lat                   ) )
                      &&
                      ( c1_lat <= M_l_to_lr(lat + raster.window_ns) )
                      &&
                      ( c1_lon >= M_l_to_lr(lon                   ) )
                      &&
                      ( c1_lon <= M_l_to_lr(lon + raster.window_ew) )                     
                     )
                 {
                     if (fparm.display)
                     { 
                         M_out(L"selective_fill_raster(): corner 1 is inside this window");
                     }

                     addin_raster(raster, hgt, lat, lon, raster.window_ns, raster.window_ew, fparm, fileinfo, workarea); 
                 }
                 else 
                 if  ( 
                      ( c2_lat >= M_l_to_lr(lat                   ) )
                      &&
                      ( c2_lat <= M_l_to_lr(lat + raster.window_ns) )
                      &&
                      ( c2_lon >= M_l_to_lr(lon                   ) )
                      &&
                      ( c2_lon <= M_l_to_lr(lon + raster.window_ew) )                     
                     )
                 {
                     if (fparm.display)
                     { 
                         M_out(L"selective_fill_raster(): corner 2 is inside this window"); 
                     }

                     addin_raster(raster, hgt, lat, lon, raster.window_ns, raster.window_ew, fparm, fileinfo, workarea); 
                 }
                 else 
                 if  ( 
                      ( c3_lat >= M_l_to_lr(lat                   ) )
                      &&
                      ( c3_lat <= M_l_to_lr(lat + raster.window_ns) )
                      &&
                      ( c3_lon >= M_l_to_lr(lon                   ) )
                      &&
                      ( c3_lon <= M_l_to_lr(lon + raster.window_ew) )                     
                     )
                 {
                     if (fparm.display)
                     { 
                         M_out(L"selective_fill_raster(): corner 3 is inside this window");
                     }

                     addin_raster(raster, hgt, lat, lon, raster.window_ns, raster.window_ew, fparm, fileinfo, workarea); 
                 }
                 else 
                 if  ( 
                      ( c4_lat >= M_l_to_lr(lat                   ) )
                      &&
                      ( c4_lat <= M_l_to_lr(lat + raster.window_ns) )
                      &&
                      ( c4_lon >= M_l_to_lr(lon                   ) )
                      &&
                      ( c4_lon <= M_l_to_lr(lon + raster.window_ew) )                     
                     )
                 {
                     if (fparm.display)
                     { 
                         M_out(L"selective_fill_raster(): corner 4 is inside this window");
                     }

                     addin_raster(raster, hgt, lat, lon, raster.window_ns, raster.window_ew, fparm, fileinfo, workarea); 
                 }
                 else             // ?????????????????????? the above two checks don't cover all cases -- need sampling of internal/edge points???????????????????????)
                 {
                     raster.files_skipped++;                  
                 }  
             }

             raster.windows_done++; 


             // put out progress message if it's been 3 seconds or more since the last one

             tp2 = clk.now(); 
             since_last = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1)).count()) / 1000.0;

             if (since_last >= 3.0)
             {
                 elapsed = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp0)).count()) / 1000.0;

                 M_out( L"Raster  : (%.3f sec) latitude = %+-03d   windows= %5u/%u   files: read-in=%-5u   reused=%-5u   missing=%-5u   skipped=%-5u   files/sec = %.3f")
                      % elapsed
                      % M_l_to_ld(lat)                     // show current window latitude in degrees
                      % raster.windows_done 
                      % ((raster.grid.extent_ew  / raster.window_ew) * (raster.grid.extent_ns / raster.window_ns))
                      % raster.files_read
                      % raster.files_reused
                      % raster.files_missing
                      % raster.files_skipped
                      % (((real_T)(raster.files_read)) / elapsed)
                      ;
                  tp1 = tp2;      // reset last message time for next 3-second time 
              }

         }	 
     }
     

     //  end processing -- put put elapsed time and statistics
     //  -----------------------------------------------------
    
     elapsed = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp0)).count()) / 1000.0;
     
     elapsed = M_max(elapsed, 0.0001);
     
     M_out( L"Raster  : windows=%u (%.3f x %.3f deg)    files: read-in=%d reused=%u missing=%u skipped=%u total=%u     elapsed time = %.3f seconds  files/sec = %.3f")
          % raster.windows_done
          % M_l_to_lr(raster.window_ns)
          % M_l_to_lr(raster.window_ew)
          % raster.files_read
          % raster.files_reused
          % raster.files_missing
          % raster.files_skipped
          % (raster.files_read + raster.files_reused + raster.files_missing + raster.files_skipped)
          % elapsed
          % (((real_T)(raster.files_read)) / elapsed)
          ;
     M_out( L"Raster  : bytes read = %s (%.3fMB)   voidfills_needed = %u   voidfills_skipped = %u   voidpoints = %u   bytes/sec = %.1f (%.3f MB/sec)")
          % M_sep(raster.bytes_read)
          % M_meg(raster.bytes_read)
          % raster.voidfills_needed
          % raster.voidfills_not_needed
          % raster.voidpoints
          % (((real_T)(raster.bytes_read)) / elapsed)
          % (((real_T)(raster.bytes_read)) / (elapsed * 1024576.0))
          ;

     if (fparm.display)
     {
         M_out(L"Raster  : cols = %-5d  valid col_min = %-5d  valid col_max = %-5d") % raster.grid.cols % raster.col_min % raster.col_max;
         M_out(L"Raster  : rows = %-5d  valid row_min = %-5d  valid row_max = %-5d") % raster.grid.rows % raster.row_min % raster.row_max;
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
//||       addin_raster() -- add in data from .hgt files to raster_t structure  -- internal function
//||  
//||          -- processes NxM square degrees at a time  
//||          -- input lat/lon is in arc-sec
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void addin_raster(raster_S& raster, hgt_S& hgt, ll_T lat, ll_T lon, ll_T extent_ns, ll_T extent_ew, const fill_raster_parm_S& fparm, const fileinfo_S& fileinfo, workarea_S& workarea) try    
{
    int rc;          // R/C from this routine  
      
    if (fparm.debug_tile)
    {
        M_out(L"addin_raster():  lat=%d (%.16f deg)  lon=%d (%.16f deg)  ns=%d (%.16f deg)  ew=%d (%.16f deg)") % lat % M_l_to_lr(lat) % lon % M_l_to_lr(lon) % extent_ns % M_l_to_lr(extent_ns) % extent_ew % M_l_to_lr(extent_ew);
    }


    // fill in this window in raster (base fill)
    // -----------------------------------------

    if (fparm.base_fill)
    {
        auto rcb = readin_hgt(hgt, lat, lon, extent_ns, extent_ew, fparm, fileinfo); 

        if (rcb >= 0)
        {
            if (rcb == 0)
            {
                raster.bytes_read += hgt.ct;
                raster.basefill_files_read++; 
                raster.files_read++; 
            }
        
            else   // assume r/C = 1 = suppressed-read
            {
                raster.files_reused++;
            }

            set_raster_data(raster, hgt, lat, lon, extent_ns, extent_ew, fparm, fileinfo, workarea);       // ignore R/C 
        }
        else
        {
            raster.basefill_files_missing++;  
            raster.files_missing++;  
        }
    }
      
 
    // fill in any void points in raster window
    // ----------------------------------------

    if (fparm.void_fill)
    {
        auto rcc = check_raster_data(raster, lat, lon, extent_ns, extent_ew, fparm);  
        if (rcc == -1)
        {
            auto rcv = readin_hgt(hgt, lat, lon, extent_ns, extent_ew, fparm, fileinfo); 

            if (rcv >= 0)
            {
                if (rcv == 0) 
                {
                    raster.bytes_read += hgt.ct;
                    raster.voidfill_files_read++; 
                    raster.files_read++; 
                }
                else
                {
                    raster.files_reused++;
                }

                (void)refill_raster_data(raster, hgt, fparm);       // ignore R/C   
            }
            else
            {
                raster.voidfill_files_missing++; 
                raster.files_missing++;
            }
        }
    }


    // fill in any empty points in this raster window
    // ----------------------------------------------

    if (fparm.empty_fill)
    {
        auto rcc = check_raster_data(raster, lat, lon, extent_ns, extent_ew, fparm);  
        if (rcc == -1)
        {
            auto rce = readin_hgt(hgt, lat, lon, extent_ns, extent_ew, fparm, fileinfo); 

            if (rce >= 0)
            {
                if (rce == 0) 
                {
                    raster.bytes_read += hgt.ct;
                    raster.emptyfill_files_read++; 
                    raster.files_read++; 
                }
                else
                {
                    raster.files_reused++;
                }

                (void)refill_raster_data(raster, hgt, fparm);       // ignore R/C   
            }
            else
            {
                raster.emptyfill_files_missing++;
                raster.files_missing++;
            }
        }
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
//||       set_raster_data() -- add data from hgt_t structure into raster_t structure  -- internal function
//||                            completely overlay anything already in raster (which should be empty)
//||
//||
//||         
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


//                         input lat/lon are degrees*3600 to match raster and hgt south, west, etc.

static void set_raster_data(raster_S& raster, const hgt_S& hgt, ll_T south_in, ll_T west_in, ll_T extent_ns_in, ll_T extent_ew_in, const fill_raster_parm_S& fparm, const fileinfo_S& fileinfo, workarea_S& workarea) try
{
    real_T    row_advf_h           { 0.0   } ;            // used only when .hgt does not have exact coverage of all raster points  
    real_T    col_advf_h           { 0.0   } ;            // used only when .hgt does not have exact coverage of all raster points
    real_T    row_startf_h         { 0.0   } ;            // start row of NxM data area in .hgt -- used when interpolation needed in .hgt   
    real_T    col_startf_h         { 0.0   } ;            // start col of NxM data area in .hgt -- used when interpolation needed in .hgt   
     
    rc_T      col_adv_h            { 0     } ;            // used only when .hgt has exact coverage for all raster points 
    rc_T      row_adv_h            { 0     } ;            // used only when .hgt has exact coverage for all raster points
    rc_T      row_start_h          { 0     } ;            // start row of NxM data area in .hgt -- used when no interpolation needed in .hgt 
    rc_T      col_start_h          { 0     } ;            // start col of NxM data area in .hgt -- used when no interpolation needed in .hgt

                                   
    bool      no_column_skip       { false } ;            // hgt_S column scale is same as raster column scale -- whole rows can be copied from hgt_S to raster_S at once
    bool      exact_hgt_coverage   { false } ;            // hgt_S has points aligned with all points in the window of the raster_S (hgt_S may have 'N' times as many points, where N is integer
 

    //
    //    - Current window (SW corner is lat/lon_r)  must lie entirely inside one .hgt file in the coverage array.  (This should have been checked already, when the file was read into the buffer) 
    //
    //    - Parts of current window can lie outside of the raster grid -- only that protion of window that intersects raster grid gets copied   
    //
    //    - incoming lon_r (and lat) should be based on raster grid limits -- and should gradually sweep across the raster.  
    //
    //    - the lon_h corresponding to lon_r is computed, and used to read-in (or reuse) the proper .hgt file in the coverage array 
    //
    //
 

    if (fparm.debug_tile)
    {
        M_out( L"set_raster_data() -- south_in=%d (%.16f deg)  west_in=%d (%.16f deg)  extent_ns_in=%d (%.16f deg)  extent_ew_in=%d (%.16f deg)")
             % south_in     % M_l_to_lr(south_in) 
             % west_in      % M_l_to_lr(west_in) 
             % extent_ns_in % M_l_to_lr(extent_ns_in) 
             % extent_ew_in % M_l_to_lr(extent_ew_in) 
             ;
    }


    //  Adjust south, west, north, east edges of window to the intersection of the incoming window and the raster grid
    //  ---------------------------------------------------------------------------------------------------------------

    ll_T south_r   = south_in; 
    ll_T west_r    = west_in; 
    ll_T extent_ns = extent_ns_in; 
    ll_T extent_ew = extent_ew_in; 
    ll_T north_r   = south_r + extent_ns; 
    ll_T east_r    = west_r  + extent_ew; 

    south_r   = M_max(south_r, raster.grid.south);  
    north_r   = M_min(north_r, raster.grid.north); 
    west_r    = M_max(west_r , raster.grid.west );  
    east_r    = M_min(east_r , raster.grid.east );

    extent_ns = north_r - south_r;      // recompute these after adjusting the limits   
    extent_ew = east_r  - west_r ;      // can't use M_dif_l() here, since we wan't bad neg value if east <= west    


    if (fparm.debug_tile)
    {
        M_out(  L"set_raster_data() -- south_r=%d (%.16f deg)  west_r=%d (%.16f deg)  north_r=%d (%.16f deg)  east_r=%d (%.16f deg) extent_ns=%d (%.16f deg)  extent_ew=%d (%.16f deg)")
              % south_r   % M_l_to_lr(south_r) 
              % west_r    % M_l_to_lr(west_r) 
              % north_r   % M_l_to_lr(north_r) 
              % east_r    % M_l_to_lr(east_r) 
              % extent_ns % M_l_to_lr(extent_ns) 
              % extent_ew % M_l_to_lr(extent_ew) 
              ;
        
    
        if ( (extent_ns < extent_ns_in) || (extent_ew < extent_ew_in) )
        {
            M_out( L"set_raster_data() -- clipping: extent_ns=%d (%.16f deg)  extent_ew=%d (%.16f deg)  extent_ns_in=%d (%.16f deg)  extent_ew_in=%d (%.16f deg)")
                 % extent_ns    % M_l_to_lr(extent_ns   ) 
                 % extent_ew    % M_l_to_lr(extent_ew   ) 
                 % extent_ns_in % M_l_to_lr(extent_ns_in) 
                 % extent_ew_in % M_l_to_lr(extent_ew_in) 
                 ;          
        }
    }   


    // make sure window has a valid intersection with the raster grid (use nominal limits)

    if ( (extent_ns <= 0) || (extent_ew <= 0) )
    {
        M_out_emsg1( L"set_raster_data() : south_in=%d (%.16f deg)  west_in=%d (%.16f deg)  extent_ns_in=%d (%.16f deg)  extent_ew_in=%d (%.16f deg)")
                   % south_in     % M_l_to_lr(south_in) 
                   % west_in      % M_l_to_lr(west_in) 
                   % extent_ns_in % M_l_to_lr(extent_ns_in) 
                   % extent_ew_in % M_l_to_lr(extent_ew_in) 
                   ;
        M_out_emsg2( L"set_raster_data() : raster.grid.south=%d (%.16f deg)  raster.grid.west=%d (%.16f deg)  raster.grid.north=%d (%.16f deg)  raster.grid.east=%d (%.16f deg)")
                   % raster.grid.south % M_l_to_lr(raster.grid.south) 
                   % raster.grid.west  % M_l_to_lr(raster.grid.west)
                   % raster.grid.north % M_l_to_lr(raster.grid.north)
                   % raster.grid.east  % M_l_to_lr(raster.grid.east)
                   ;
        M_throw("set_raster_data() :  requested window does not intersect raster grid")
        return;           // should not get here
    }

    
    // make sure values are in limits now (no SIGSEGV) -- use nominal limits/edges, etc. -- ignore pixel offsets

    if (south_r < raster.grid.south)
    {
        M_throw_v("set_raster_data(): South edge of window area (%d) less than south edge of raster data (%d)") % south_r % raster.grid.south ));
        return;    // should not get here 
    }

    if (north_r > raster.grid.north)
    {
        M_throw_v("set_raster_data(): North edge of window area (%d) more than north edge of raster data (%d)") % north_r % raster.grid.north));
        return;    // should not get here 
    }

    if (west_r < raster.grid.west)
    {
        M_throw_v("set_raster_data(): West edge of window area (%d) less than west edge of raster data (%d)") % west_r % raster.grid.west));
        return;    // should not get here 
    }

    if (east_r > raster.grid.east)
    {
        M_throw_v("set_raster_data(): East edge of window area (%d) more than east edge of raster data (%d)") % east_r % raster.grid.east));
        return;   // should not get here 
    }

    
    //  Get corresponding start hgt_S longitude/latitude that corresponds to incoming raster grid longitude/latitude (these are nominal lat/lon -- no pixel offsets in raster_S are accounted for here)
   
    ll_T south_h = south_r; 
    ll_T north_h = north_r;
    ll_T west_h  = west_r;  

    int rc = adjust_longitude(hgt.tile, south_h, west_h, fileinfo);     // updates west_h, if required  
    if (rc != 0) 
        M_throw_v("set_raster_data(): non-zero R/C (%d) from adjust_longitude()") % rc)); 

    ll_T east_h = west_h + extent_ew;                         // known not to go off edge, or wrap around, etc.


    if (fparm.debug_tile)
    {
        M_out(  L"set_raster_data() -- south_h=%d (%.16f deg)  west_h=%d (%.16f deg)  north_h=%d (%.16f deg)  north_h=%d (%.16f deg)")
              % south_h   % M_l_to_lr(south_h) 
              % west_h    % M_l_to_lr(west_h) 
              % north_h   % M_l_to_lr(north_h) 
              % east_h    % M_l_to_lr(east_h) 
              ;
    }   

    

    // -------------------------------------------------------------------------------------------------------------------------------------
    // Set up .hgt access loop controls based on relative fineness of raster and hgt_S data, sw corner latitude of window, window size, etc.
    // -------------------------------------------------------------------------------------------------------------------------------------
    
    if (
        (hgt.grid.pts_per_deg_ew == raster.grid.pts_per_deg_ew)
        &&
        (hgt.grid.extra_row_col == raster.grid.extra_row_col)                           // no direct copy if pixel centers are not aligned between raster and .hgt (don't have to worry about differing pixel offsets in this 
        &&
        (hgt.grid.pixels_aligned == raster.grid.pixels_aligned) 
       )
    {
        raster.windows_whole_rows++;
        no_column_skip = true;                                                          // will be picking up consecutive columns from .hgt file  
        M__(M_out(L"set_raster_data() -- no_column_skip");)
    }
    
    if (
        (
         (hgt.grid.pts_per_deg_ns == raster.grid.pts_per_deg_ns)
         &&
         (hgt.grid.pts_per_deg_ew == raster.grid.pts_per_deg_ew)                        // 1st aligned case -- if pixels are same dimension and are aligned same in both hgt_S and raster_S
         &&
         (hgt.grid.extra_row_col  == raster.grid.extra_row_col )  
         &&
         (hgt.grid.pixels_aligned == raster.grid.pixels_aligned)  
        )
        ||                                                                              // -or-
        (
          (0 == hgt.grid.pts_per_deg_ew % raster.grid.pts_per_deg_ew)
          &&
          (0 == hgt.grid.pts_per_deg_ns % raster.grid.pts_per_deg_ns)                   // 2nd aligned case -- raster pixels are even multiple times hgt pixels (both ways) and raster and hgt pixel cemters are both on nominal grid 
          &&
          (hgt.grid.extra_row_col)
          &&
          (raster.grid.extra_row_col)    
          &&
          (hgt.grid.pixels_aligned)                                                     // make sure both have aligned pixels -- don't have to worry about differing pixel offsets in case raster_S ppd differs from hgt_S ppd
          &&
          (raster.grid.pixels_aligned)                                    
        )                                                                                 
       )
    {
        exact_hgt_coverage = true; 
        raster.windows_exact_coverage++;

        // note:  in this case, pixel center offsets are same in both raster_S and hgt_S, so they can be ignored -- just use nominal grid points in both -- hgt_S row/col are integers -- no interpolation needed

        row_start_h = ((int64_t)(hgt.grid.north - north_h)         * (int64_t)(hgt.grid.pts_per_deg_ns)) / (int64_t)M_arcsec_1; 
        col_start_h = ((int64_t)(west_h         - hgt.grid.west  ) * (int64_t)(hgt.grid.pts_per_deg_ew)) / (int64_t)M_arcsec_1;

        col_adv_h = hgt.grid.pts_per_deg_ew / raster.grid.pts_per_deg_ew;              // advance hgt_S data row this much for each raster row 
        row_adv_h = hgt.grid.pts_per_deg_ns / raster.grid.pts_per_deg_ns;              // advance hgt_S data col this much for each raster col 
 

        if (fparm.debug_tile)
        {
            M_out(L"set_raster_data() -- south_h=%d    hgt.grid.north=%d   hgt.grid.pts_per_deg_ns=%d") % south_h   % hgt.grid.north % hgt.grid.pts_per_deg_ns;
            M_out(L"set_raster_data() -- west_h =%d    hgt.grid.west=%d    hgt.grid.pts_per_deg_ew=%d") % west_h    % hgt.grid.west  % hgt.grid.pts_per_deg_ew;

            M_out(L"set_raster_data() -- exact coverage: col_start_h=%d    row_start_h=%d    col_adv_h=%d   row_adv_h=%d") % col_start_h % row_start_h % col_adv_h % row_adv_h;
        }  
    }
    else  // must interpolate or use nearest neighbor, etc. -- fractional columns in hgt_S data area -- must account for possibly mismatched pixel center offsets       
    {
        exact_hgt_coverage = false; 
        raster.windows_inexact_coverage++;

        // note that hgt.grid.n_pixel and .w_pixel are already adjusted for pixel center offsets from nominal grid, and the raster grid pixel offsets are included in calculations below

        row_startf_h =  ( hgt.grid.n_pixel - (M_l_to_lr(north_h) - raster.grid.offset_ns)) * (real_T)(hgt.grid.pts_per_deg_ns);  
        col_startf_h =  ( M_l_to_lr(west_h) + raster.grid.offset_ew - hgt.grid.w_pixel   ) * (real_T)(hgt.grid.pts_per_deg_ew);  

        col_advf_h = (real_T)(hgt.grid.pts_per_deg_ew) / (real_T)(raster.grid.pts_per_deg_ew);       // advance .hgt row this much for each raster row (may be fractional)
        row_advf_h = (real_T)(hgt.grid.pts_per_deg_ns) / (real_T)(raster.grid.pts_per_deg_ns);       // advance .hgt col this much for each raster col (may be fractional) 

        if (fparm.debug_tile)
        {
            M_out(L"set_raster_data() -- no exact coverage: w_pixel=%.6f  col_startf_h=%.6f    n_pixel=%.6f  row_startf_h=%.6f    col_advf_h=%.6f    row_advf_h=%.6f") 
                  % hgt.grid.w_pixel % col_startf_h % hgt.grid.n_pixel % row_startf_h % col_advf_h % row_advf_h;
        }
    }
    
    
    // -----------------------------------------------------------------------------
    // Set up raster access loop controls -- step is always 1 column -- always exact (any fractional row/col is done on the .hgt buffer access)
    // -----------------------------------------------------------------------------
    
    // compute offsets of NxM degree window within raster coverage area 

    rc_T row_offset_r = (rc_T)(((raster.grid.north - north_r         ) * raster.grid.pts_per_deg_ns) / M_arcsec_1); 
    rc_T col_offset_r = (rc_T)(((west_r            - raster.grid.west) * raster.grid.pts_per_deg_ew) / M_arcsec_1); 


    // compute number of rows and columns of data to fill in raster structure, based on number of arc-seconds in NxM degree window
    // raster squares may have one more row column than this computation shows, so the +1 must be added if the raster has an extra row/col 

    rc_T rows_r = (rc_T)((extent_ns * raster.grid.pts_per_deg_ns) / M_arcsec_1); 
    rc_T cols_r = (rc_T)((extent_ew * raster.grid.pts_per_deg_ew) / M_arcsec_1);

    if ( (rows_r < 3) || (cols_r < 3) )   // make sure window is not too small to do proper fill ??? something wrong here ?????
    {
        M_out_emsg1( L"set_raster_data(): window too small-- rows_r=%d  cols_r=%d") % rows_r % cols_r; 
        M_out_emsg2( L"south_r=%d (%.16f deg)  west_r=%d (%.16f deg)  north_r=%d (%.16f deg)  east_r=%d (%.16f deg) extent_ns=%d (%.16f deg)  extent_ew=%d (%.16f deg)")
                   % south_r   % M_l_to_lr(south_r) 
                   % west_r    % M_l_to_lr(west_r) 
                   % north_r   % M_l_to_lr(north_r) 
                   % east_r    % M_l_to_lr(east_r) 
                   % extent_ns % M_l_to_lr(extent_ns) 
                   % extent_ew % M_l_to_lr(extent_ew) 
                   ;     
        M_throw_v("set_raster_data(): window too small -- rows_r=%d  cols_r=%d") % rows_r % cols_r));
        return;      // shouldn't get here
    }

    if (raster.grid.extra_row_col)     // extra row/col for each window -- will cause overlapped copies, except at right/top edges, where this extra row/col is actually necessary to complete the raster
    {
        rows_r++;   
        cols_r++; 
    }
   
    if (fparm.debug_tile)
    {
        M_out(L"set_raster_data() -- raster row/col offset = %d/%d   raster rows/cols = %d/%d") % row_offset_r % col_offset_r % rows_r % cols_r;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///    hgt_S points align with raster -- no interpolation needed (fast copy possible if no columns being skipped in hgt_S data) 
    ///
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (exact_hgt_coverage)    
    {
        rc_T row_h = row_start_h;                         // always start at proper row in hgt_S data buffer

        for (rc_T row_r = row_offset_r; row_r < row_offset_r + rows_r; row_r++)          // outer loop for each row in hgt data 
        {        
            if (no_column_skip)                             // no hgt columns to be skipped -- copy whole row at a time        
            { 
                // ----------------------------------------------------------------------------------------------------------
                // not skipping any columns in the hgt_S data -- memcpy() can be used in place of inner loops for each column
                // ----------------------------------------------------------------------------------------------------------

                if (hgt.little_end)
                {
                    // little endian hgt_S data -- can move whole row in, since bytes in the row are in same order in hgt_S data and raster buffer

                    int16_t   *p0  = (int16_t *)&M_raster_ht(raster, row_r, col_offset_r);           // ptr to 1st integer in row subset      (low part of int16) -- little endian
                    int16_t   *hp0 = (int16_t *)M_hgt_ht0_p(hgt, row_h, col_start_h);                // ptr to 1st byte in row to be copied   (low part of int16) -- little endian

                    // copy whole row of bytes from hgt_S buffer to a subrow in the raster_S buffer
            
                    raster.set_whole_row_le++;

                    memcpy( (void *)p0                                   // subrow start in raster -- will become 1st lo byte 
                          , (void *)hp0                                  // start of .hgt row == 1st lo byte, etc.
                          , (size_t)((sizeof (int16_t)) * (cols_r))      // copy 1st + last col from hgt_S -- last one will get overlaid on later copy, if this is not last subrow in raster buffer   
                         );
                }
                else // hgt_S data is big-endian, but not skipping any columns in hgt_S data 
                {
                    int8_t   *p0  = (int8_t *)&M_raster_ht(raster, row_r, col_offset_r);            // ptr to 1st byte in row subset  (low part of int16) -- little endian
                    int8_t   *hp0 =            M_hgt_ht0_p(hgt, row_h, col_start_h);                // ptr to 1st byte to be copied in row    (hi part of int16) -- big endian
                    int8_t   *hp1 =            M_hgt_ht1_p(hgt, row_h, col_start_h);                // ptr to 2nd byte to be copied in row    (lo part of int16) -- big endian
                    int i;                                                              // local loop counter
           
                   // copy bytes from hgt buffer to raster buffer, but shifted down one byte -- data in raster_S is always little-endian 
                   //
                   // (this should put high bytes in proper place in this raster row -- need to get the low bytes one by one down below.
                   
                   raster.set_whole_row_be++;

                   memcpy( (void *)(p0 + 1)                               // row segment start +1 -- will become 1st hi byte 
                         , (void *)hp0                                    // start of hgt_S row == 1st hi byte, etc.
                         , (size_t)((sizeof (int16_t)) * (cols_r) - 1)    // don't copy last lo byte from .hgt_S area  
                         );

                   // copy in low bytes one by one by looping over columns

                   for (i = 0; i < cols_r; i++)
                   {                                 
                     //M_msg("row_e=%d/%d row_h=%d ep0-2=%p  hp0=%p &ep0=%p",row_e, rows_e + row_offset, row_h, ep0, hp0, &ep0);
                               
                       *p0        = *hp1;                            // move low byte of int16 into raster_S 
                     //*(p0 + 1)  = *hp0;//////                      // move hi byte of int16 into raster_S -- already done above
              
                       hp1 += 2;                                     // move up 2 bytes, to next lo byte in hgt_S area  
                       p0  += 2;                                     // move up 2 bytes, to next lo hyte in raster_S area
                   } 

                }  // big-endian
            }      // no column skip
                   // -----------------------------------------------------------------
            else   // skipping columns in hgt_S data -- need to use point-by-point loop  
                   // -----------------------------------------------------------------
            {
                if (hgt.little_end)   // little-endian hgt_S data
                {
                    rc_T col_h = col_start_h;     // alway start at proper column in hgt_S data

                    raster.set_aligned_le += cols_r; 

                    for (rc_T col_r = col_offset_r; col_r < col_offset_r + cols_r; col_r++)      // inner loop for each column in this row of hgt data
                    {                                                             
                        M_raster_ht(raster, row_r, col_r) = M_hgt_ht2(hgt, row_h, col_h);                        
                        col_h += col_adv_h;                                                      // advance to next column in hgt_S data (will skip over columns)
                    }
                }
                else                // big-endian hgt_S data
                {
                    rc_T col_h = col_start_h;           // always start at proper column in hgt_S data

                    raster.set_aligned_be += cols_r;

                    for (rc_T col_r = col_offset_r; col_r < col_offset_r + cols_r; col_r++)      // inner loop for each column in this row of hgt_S data
                    {                  
                        M_copyswap2( M_raster_ht(raster, row_r, col_r)
                                   , M_hgt_ht(   hgt,    row_h, col_h) 
                                   )
                        col_h += col_adv_h;            // advance to next column in hgt_S data (will skip over columns)
                    }
                }
            }    // column skip

            // advance to next row in hgt_S data 

            row_h += row_adv_h; 

        }   // row loop        
    }       // exact hgt_S coverage


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///    .hgt points do not align with raster -- do interpolation or use nearby neighbor in .hgt file
    ///
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    else
    {
        // ===============================================================================
        // copy data from hgt_S data using interpolation for raster points that don't align 
        // ===============================================================================

        if (fparm.interpolate)   
        {
            for (rc_T row_r = row_offset_r; row_r < row_offset_r + rows_r; row_r++)                     // outer loop for each row in raster and hgt_S data 
            {
                real_T rowf_h = row_startf_h + row_advf_h * (real_T)(row_r - row_offset_r);             // row number may be fractional when interpolating     
                
                raster.set_interpolated_ble += cols_r;

                for (rc_T col_r = col_offset_r; col_r < col_offset_r + cols_r; col_r++)                 // inner loop for each column in this row of hgt_S data
                {
                    real_T colf_h = col_startf_h + col_advf_h * (real_T)(col_r - col_offset_r);         // column number may be fractional when interpolating    

                    M_raster_ht(raster, row_r, col_r) = (int16_t)get_elev_ble_interpolate(hgt, rowf_h, colf_h);                    
                }                
            }      // row loop        
        }          // doing interpolation

        // ===========================================================
        // copy data from hgt_S data using nearby neighbor in .hgt file 
        // ===========================================================

        else   // no interpolation required
        {
            //  set up array of pre-computed nearest hgt_S column numbers for use in loops (in case columns are not 1-1 between hgt_S and raster_S

            workarea.buf.realloc(cols_r);     // make sure buffer is big enough to hold all column indexes

   
   
            for (rc_T col_ix = 0, col_r = col_offset_r; col_r < col_offset_r + cols_r; col_r++)      
            {
                workarea.buf.p[col_ix++] = M_lim(0, (rc_T)(col_startf_h + col_advf_h * (real_T)(col_r - col_offset_r)), hgt.grid.cols - 1);    
                M__(M_out(L"set_raster_data() -- buf.p[%d] = %d") % (col_ix - 1) % workarea.buf.p[col_ix - 1];)
            }
   


            // outer row loop to copy data points

            for (rc_T row_r = row_offset_r; row_r < row_offset_r + rows_r; row_r++)                                                // outer loop for each row in raster and hgt_S data 
            {
                rc_T row_h = M_lim(0, (rc_T)(row_startf_h + row_advf_h * (real_T)(row_r - row_offset_r)), hgt.grid.rows - 1);      // find nearest row 
       
                if (hgt.little_end)   // little-endian hgt_S data
                {
                    raster.set_nearest_neighbor_le += cols_r; 

                    for (rc_T col_ix = 0, col_r = col_offset_r; col_r < col_offset_r + cols_r; col_r++)                            // inner loop for each column in this row of hgt_S data
                    {
                        rc_T col_h = workarea.buf.p[col_ix++];                                                                     // pre-computed hgt_S data column
                        M_raster_ht(raster, row_r, col_r) = M_hgt_ht2(hgt, row_h, col_h);                    
                    }
                }
                else                // big-endian hgt_S data
                {
                    raster.set_nearest_neighbor_be += cols_r; 

                    for (rc_T col_ix = 0, col_r = col_offset_r; col_r < col_offset_r + cols_r; col_r++)                            // inner loop for each column in this row of hgt data
                    {                 
                        rc_T col_h = workarea.buf.p[col_ix++];                                                                     // pre-computed .hgt file column
                   
                        M_copyswap2( M_raster_ht(raster, row_r, col_r)
                                   , M_hgt_ht(   hgt,    row_h, col_h) 
                                   )
                        M__(M_outg(L"set_raster_data() -- row_r=%d  col_r=%d  row_h=%d  col_h=%d  ht=%04X  hgt_h=%04X") % row_r % col_r % row_h % col_h % M_raster_ht(raster, row_r, col_r) % M_hgt_ht2(hgt, row_h, col_h);)

                    }
                }    // big endian hgt_S data
            }        // row loop
        }            // no interpolation needed (use nearby neighbor)
    }                // not exact hgt_Scoverage 




    // add to number of hgt_S windows added in and min/max valid column for later use

    raster.hgt_ct++; 

    raster.col_min = M_min(raster.col_min, col_offset_r             );
    raster.col_max = M_max(raster.col_max, col_offset_r + cols_r - 1); 
    raster.row_min = M_min(raster.row_min, row_offset_r             );
    raster.row_max = M_max(raster.row_max, row_offset_r + rows_r - 1);
          
    return;  
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       check_raster_data() -- check for void/missing points in that part of raster covered by passed-in hgt_S (not hi performance)
//||                                
//||
//||       rc = 0 -- no refills needed
//||       rc = -1 -- refills needed
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int check_raster_data(raster_S& raster, ll_T lat, ll_T lon, ll_T extent_ns, ll_T extent_ew, const fill_raster_parm_S& fparm) try
{
    ll_T   south  { lat                  }; 
    ll_T   north  { lat + extent_ns      }; 
    ll_T   west   { lon                  };
    ll_T   east   { lat + extent_ew      }; 


    //  Make sure not called for base_fill

    if (fparm.base_fill)
    {
        M_throw_v("check_raster_data(): unexpected call with fparm.base_fill flag set") )); 
        return -2;       // should not get here 
    } 


    //  Make sure window fits entirely inside raster area 

    if (south < raster.grid.south)
    {
        M_throw_v("check_raster_data(): South edge of check window (%d) less than south edge of raster data (%d)") % south % raster.grid.south)); 
        return -2;       // should not get here 
    }

    if (north > raster.grid.north)
    {
        M_throw_v("check_raster_data(): North edge of check window (%d) more than north edge of raster data (%d)") % north % raster.grid.north)); 
        return -2;       // should not get here 
    }

    if (west < raster.grid.west)
    {
        M_throw_v("check_raster_data(): West edge of check window (%d) less than west edge of raster data (%d)") % west % raster.grid.west));
       return -2;        // should not get here 
    }

    if (east > raster.grid.east)
    {
        M_throw_v("check_raster_data(): East edge of check window (%d) more than east edge of raster data (%d)") % east % raster.grid.east));
        return -2;       // should not get here 
    }
    

    // compute offsets of hgt data within raster coverage area

    rc_T row_offset = (rc_T)(((raster.grid.north - north            ) * raster.grid.pts_per_deg_ns) / M_arcsec_1); 
    rc_T col_offset = (rc_T)(((west              - raster.grid.west ) * raster.grid.pts_per_deg_ew) / M_arcsec_1); 


    // compute number of rows and columns of data in raster structure, based on number of arc-seconds in passed-in window bounds
    
    rc_T   extra_row_col    { 0 } ; 

    if (raster.grid.extra_row_col)
        extra_row_col = 1;  

    rc_T rows_e = (rc_T)((extent_ns * raster.grid.pts_per_deg_ns) / M_arcsec_1 + extra_row_col); 
    rc_T cols_e = (rc_T)((extent_ew * raster.grid.pts_per_deg_ew) / M_arcsec_1 + extra_row_col);

    M_y(M_out(L"check_raster_data(): row offset   = %d") % row_offset;) 
    M_y(M_out(L"check_raster_data(): col offset   = %d") % col_offset;)
    M_y(M_out(L"check_raster_data(): raster rows  = %d") % rows_e;) 
    M_y(M_out(L"check_raster_data(): raster cols  = %d") % cols_e;)
    

    // =========================================================
    // check for voids in raster for area covered by this window
    // =========================================================

    for (rc_T row_e = row_offset; row_e < row_offset + rows_e; row_e++)          // outer loop for each row in hgt data 
    {
        for (rc_T col_e = col_offset; col_e < col_offset + cols_e; col_e++)      // inner loop for each column in this row of hgt data
        {
            if (fparm.void_fill)
            {
                if (RASTER_VOID == M_raster_ht(raster, row_e, col_e))
                {
                    raster.voidfills_needed++;
                    return -1;                                                   // exception -- VOID found
                }
            }

            if (fparm.empty_fill)
            {
                if (RASTER_EMPTY == M_raster_ht(raster, row_e, col_e))
                {
                    raster.emptyfills_needed++;
                    return -1;                                                   // exception -- VOID found
                }
            }
        }        
    }        // row loop
    

    if (fparm.void_fill)
       raster.voidfills_not_needed++;

    if (fparm.empty_fill)
       raster.emptyfills_not_needed++;

    return 0;  
}
M_endf





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       refill_raster_data() -- replace void/empty points in raster from hgt_S (not hi performance)
//||                                
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ????????? needs to account for mismatched pixel offsets in raster vs hgt buffer -- interpolation ?????
// ????????? needs to account for mismatched pixel offsets (centered/aligned) between hgt data and raster data
                                                                        
static void refill_raster_data(raster_S& raster, const hgt_S& hgt, const fill_raster_parm_S& fparm) try
{
    //  Make sure not called for base_fill

    if (fparm.base_fill)
    {
        M_throw_v("refill_raster_data(): unexpected call with fparm.base_fill flag set") )); 
        return;           // should not get here 
    } 


    //  Make sure .hgt file fits entirely inside raster area

    if (hgt.grid.south < raster.grid.south)
    {
        M_throw_v("refill_raster_data(): South edge of hgt data (%d) less than south edge of raster data (%d)") % hgt.grid.south % raster.grid.south));
        return;           // should not get here
    }

    if (hgt.grid.north > raster.grid.north)
    {
        M_throw_v("refill_raster_data(): North edge of hgt data (%d) more than north edge of raster data (%d)") % hgt.grid.north % raster.grid.north));
        return;          // should not get here 
    }

    if (hgt.grid.west < raster.grid.west)
    {
        M_throw_v("refill_raster_data(): West edge of hgt data (%d) less than west edge of raster data (%d)") % hgt.grid.west % raster.grid.west));
        return;          // should not get here 
    }

    if (hgt.grid.east > raster.grid.east)
    {
        M_throw_v("refill_raster_data(): East edge of hgt data (%d) more than east edge of raster data (%d)") % hgt.grid.east % raster.grid.east));
        return;          // should not get here 
    }
  

    // compute raster reduction factor -- note: assume raster grid is 1/1 or 1/2, 1/3, 1/N the size of the .hgt file -- no interpolation by this routine -- pick's every N-th point from ,hgt file 

    real_T factor_ns = (real_T)(hgt.grid.pts_per_deg_ns) / (real_T)(raster.grid.pts_per_deg_ns); 
    real_T factor_ew = (real_T)(hgt.grid.pts_per_deg_ew) / (real_T)(raster.grid.pts_per_deg_ew);
    
    M_y(M_out(L"refill_raster_data(): Raster reduction factors = N-S:%12.6f  E-W:%12.6f") % factor_ns % factor_ew;)
  

    // compute offsets of hgt data within raster coverage area

    rc_T row_offset = ((raster.grid.north - hgt.grid.north   ) * raster.grid.pts_per_deg_ns) / M_arcsec_1; 
    rc_T col_offset = ((hgt.grid.west     - raster.grid.west ) * raster.grid.pts_per_deg_ew) / M_arcsec_1; 


    // compute number of rows and columns of data in raster structure, based on number of arc-seconds in hgt structure
 
    rc_T   extra_row_col    { 0 } ; 

    if (raster.grid.extra_row_col)
        extra_row_col = 1;  
  
    rc_T rows_e = (hgt.grid.extent_ns * raster.grid.pts_per_deg_ns) / M_arcsec_1 + extra_row_col; 
    rc_T cols_e = (hgt.grid.extent_ew * raster.grid.pts_per_deg_ew) / M_arcsec_1 + extra_row_col;

    M_y(M_out(L"refill_raster_data(): row offset   = %d") % row_offset;) 
    M_y(M_out(L"refill_raster_data(): col offset   = %d") % col_offset;)
    M_y(M_out(L"refill_raster_data(): raster rows  = %d") % rows_e;) 
    M_y(M_out(L"refill_raster_data(): raster cols  = %d") % cols_e;)
    

    // ====================================================================
    // replace void/empty points in raster window with data from hgt buffer         
    // ====================================================================

    for (rc_T row_e = row_offset; row_e < row_offset + rows_e; row_e++)          // outer loop for each row in raster window 
    {
        rc_T row_h = M_lim(0, (rc_T)(factor_ns * (real_T)(row_e - row_offset)), hgt.grid.rows - 1);
        

        // handle void_fill case
        // ---------------------

        if (fparm.void_fill)
        {
            for (rc_T col_e = col_offset; col_e < col_offset + cols_e; col_e++)      // inner loop for each column in this row of raster window
            {
                if (RASTER_VOID == M_raster_ht(raster, row_e, col_e))
                {
                    rc_T col_h = M_lim(0, (rc_T)(factor_ew * (real_T)(col_e - col_offset)), hgt.grid.cols - 1);                                                          
                       
                    M_raster_ht(raster, row_e, col_e) = (ht_T)get_elev_ble(hgt, row_h, col_h);    // ?? what about interpolation ????
                    raster.voidpoints++;
                }
            }  
        }


        // handle empty_fill case
        // ----------------------
      
        if (fparm.empty_fill)
        {
            for (rc_T col_e = col_offset; col_e < col_offset + cols_e; col_e++)      // inner loop for each column in this row of raster window
            {
                if (RASTER_EMPTY == M_raster_ht(raster, row_e, col_e))
                {
                    rc_T col_h = M_lim(0, (rc_T)(factor_ew * (real_T)(col_e - col_offset)), hgt.grid.cols - 1);                                                          
                       
                    M_raster_ht(raster, row_e, col_e) = (ht_T)get_elev_ble(hgt, row_h, col_h);      // ?? what about interpolation ????
                    raster.emptypoints++;
                }
            }  
        }
    }        // row loop
              
    return;  
}
M_endf
  

//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       writeout_raster() -- write data from filled raster to output DEM filegroup 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int writeout_raster(raster_S& raster, const writeout_raster_parm_S& wparm, const fileinfo_S& fileinfo) try
{
    int        rc          {0};                  // return code from this function  
    hgt_S      hgt         { };                  // local hgt_T structure
    

    // --------------------------------------------------------------------------------
    // preliminary checks -- make sure raster and outpout filegroup have matching parms  (means data points in raster exactly line up with data points in output file -- no interpolation, etc. needed)
    // --------------------------------------------------------------------------------


    // make sure raster is completely filled
    // -------------------------------------


    if (!raster.complete)
    {
        M_out_emsg(L"writeout_raster(): raster is not completely filled prior to being written out to filegroup");
        return -1; 
    } 


    // make sure filegroup limits can cover whole raster (or more) 
    // -----------------------------------------------------------

    if (raster.grid.south < fileinfo.limit_s)
    {
        M_out_emsg(L"writeout_raster(): raster grid nominal south limit (%d = %.6f) less than filegroup coverage nominal south limit (%d = %.6f") 
                  % raster.grid.south % M_l_to_lr(raster.grid.south) % fileinfo.limit_s % M_l_to_lr(fileinfo.limit_s);     
        rc = -1; 
    }

    if (raster.grid.west < fileinfo.limit_w)
    {
        M_out_emsg(L"writeout_raster(): raster grid nominal west limit (%d = %.6f) less than filegroup coverage nominal west limit (%d = %.6f") 
                  % raster.grid.west % M_l_to_lr(raster.grid.west) % fileinfo.limit_w % M_l_to_lr(fileinfo.limit_w);     
        rc = -1; 
    }

    if (raster.grid.north > fileinfo.limit_n)
    {
        M_out_emsg(L"writeout_raster(): raster grid nominal north limit (%d = %.6f) greater than filegroup coverage nominal north limit (%d = %.6f") 
                  % raster.grid.north % M_l_to_lr(raster.grid.north) % fileinfo.limit_n % M_l_to_lr(fileinfo.limit_n);     
        rc = -1; 
    }

    if (raster.grid.east > fileinfo.limit_e)
    {
        M_out_emsg(L"writeout_raster(): raster grid nominal east limit (%d = %.6f) greater than filegroup coverage nominal east limit (%d = %.6f") 
                  % raster.grid.east % M_l_to_lr(raster.grid.east) % fileinfo.limit_e % M_l_to_lr(fileinfo.limit_e);     
        rc = -1; 
    }  

 
    // make sure raster grid and filegroup have same pixels per degree (scale)
    // -----------------------------------------------------------------------

    if (raster.grid.pts_per_deg_ns != fileinfo.ppd_ns)   
    {
        M_out_emsg(L"writeout_raster(): raster grid N-S ppd (%d) differs from filegroup N-S ppd (%d)") % raster.grid.pts_per_deg_ns % fileinfo.ppd_ns; 
        rc = -1; 
    }
    
    if (raster.grid.pts_per_deg_ew != fileinfo.ppd_ew)   
    {
        M_out_emsg(L"writeout_raster(): raster grid E-W ppd (%d) differs from filegroup E-W ppd (%d)") % raster.grid.pts_per_deg_ew % fileinfo.ppd_ew; 
        rc = -1; 
    }
  

    // make sure big/little-endianness  and data types match
    // -----------------------------------------------------

    if (
        (raster.little_endian != fileinfo.little_endian)
        ||
        (raster.big_endian    != fileinfo.big_endian   )
       )
    {
        M_out_emsg(L"writeout_raster(): raster grid and filegroup do not have same endian-ness -- raster little/big = %s/%s  -- filegroup little/big = %s/%s") 
                  % M_bool_cstr(raster.little_endian  ) % M_bool_cstr(raster.big_endian  ) 
                  % M_bool_cstr(fileinfo.little_endian) % M_bool_cstr(fileinfo.big_endian) ; 
        rc = -1; 
    }
  
    if (
        (raster.int_16   != fileinfo.int_16  )
        ||
        (raster.float_32 != fileinfo.float_32)
       )
    {
        M_out_emsg(L"writeout_raster(): raster grid and filegroup do not have same data type -- raster int16/float32 = %s/%s  -- filegroup int16/float32 = %s/%s") 
                  % M_bool_cstr(raster.int_16  ) % M_bool_cstr(raster.float_32  ) 
                  % M_bool_cstr(fileinfo.int_16) % M_bool_cstr(fileinfo.float_32) ; 
        rc = -1; 
    }
    
    
    // make sure pixel alignment and extra row/col match between filegroup and raster grid
    // -----------------------------------------------------------------------------------

    if (raster.grid.extra_row_col != fileinfo.extra_row_col)
    {
        M_out_emsg(L"writeout_raster(): raster grid and filegroup do not have extra row/col setting -- raster extra_row_col = %s -- filegroup extra_row_col = %s") 
                  % M_bool_cstr(raster.grid.extra_row_col)  
                  % M_bool_cstr(fileinfo.extra_row_col   ) ; 
        rc = -1; 
    }
    
    if (
        (raster.grid.pixels_aligned  != fileinfo.pixels_aligned )
        ||
        (raster.grid.pixels_centered != fileinfo.pixels_centered)
       )
    {
        M_out_emsg(L"writeout_raster(): raster grid and filegroup do not have pixel alignment -- raster aligned/centered = %s/%s  -- filegroup aligned/centered = %s/%s") 
                  % M_bool_cstr(raster.grid.pixels_aligned) % M_bool_cstr(raster.grid.pixels_centered) 
                  % M_bool_cstr(fileinfo.pixels_aligned   ) % M_bool_cstr(fileinfo.pixels_centered   ) ; 
        rc = -1; 
    }


    // make sure raster limits are even multiples of individual file extents
    // --------------------------------------------------------------------- 

    auto rcc = check_window(raster.grid, fileinfo.extent_ns, fileinfo.extent_ew, L"file-extent");      
    if (rcc != 0) 
       rc = rcc; 


    // return immediately, if any error was found during preliminary checking
    // ----------------------------------------------------------------------

    if (rc != 0) 
       return rc;              


    // =================================================================================================================================================

    // start timer
    // -----------

    std::chrono::steady_clock  clk        {           } ;                  
    auto                       tp0        { clk.now() } ;
    auto                       tp1        { tp0       } ;
    auto                       tp2        { tp1       } ;
    real_T                     elapsed    {           } ;
    real_T                     since_last {           } ;


    // initialize hgt_S before starting
    // --------------------------------

    create_hgt(hgt, fileinfo);            
    raster.windows_done = 0;                              // clear out window count from read-in phase   -- ?????????????????????? should have a raster_reset_stats() function ?????????????????????

    
    // ---------------------------------------------------------------------------------------------------------------
    // compute starting/stopping window limits to fill the raster (which must have been initialized with limits, etc.)
    // --------------------------------------------------------------------------------------------------------------
 
    ll_T step_ns   { fileinfo.extent_ns            } ;       
    ll_T step_ew   { fileinfo.extent_ew            } ;        
    ll_T lat_start { raster.grid.south             } ; 
    ll_T lat_stop  { raster.grid.north - step_ns   } ;        
    ll_T lon_start { raster.grid.west              } ; 
    ll_T lon_stop  { raster.grid.east  - step_ew   } ; 
  

    // ---------------------------------------------------
    // Write out .hgtxx files from data in the raster area  
    // ---------------------------------------------------

    for (ll_T lat = lat_start; lat <= lat_stop; lat += step_ns)
    {
        for (ll_T lon = lon_start; lon <= lon_stop; lon += step_ew)
        {
            auto wfrc = writeout_file(raster, hgt, lat, lon, step_ns, step_ew, wparm, fileinfo);                             

            if (wfrc != 0)
            {
                M_out_emsg(L"writeout_raster(): error writing out file -- returning immediately with error");
                return wfrc; 
            } 


            // put out progress message if it's been 3 seconds or more since the last one

            tp2 = clk.now(); 
            since_last = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1)).count()) / 1000.0;

            raster.windows_done++; 
 
            if (since_last >= 3.0)
            {
                elapsed = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp0)).count()) / 1000.0;
                  
                M_out( L"Raster  : (%.3f sec) latitude = %+-03d   windows=%5u/%u   files: written-out=%-5u   suppressed=%-5u   files/sec = %.3f")
                     % elapsed
                     % M_l_to_ld(lat)                                      // show current window latitude in degrees
                     % raster.windows_done  
                     % ((raster.grid.extent_ew  / step_ew) * (raster.grid.extent_ns / step_ns)) 
                     % raster.files_written
                     % raster.files_suppressed
                     % (((real_T)(raster.files_written)) / elapsed)
                     ;
                 
                tp1 = tp2;      // reset last message time for next 3-second time                                  
            }
         
        }	 
    }
   

    //  end processing -- put out elapsed time and statistics
    //  -----------------------------------------------------

    elapsed = (real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp0)).count()) / 1000.0;
    
    M_out( L"Raster  : windows=%u (%.3f x %.3f deg)    files: written-out=%u  suppressed=%u  elapsed time = %.3f seconds   files/sec = %.3f")
         % raster.windows_done
         % M_l_to_lr(step_ns)
         % M_l_to_lr(step_ew)
         % raster.files_written
         % raster.files_suppressed
         % elapsed
         % (((real_T)(raster.files_written)) / elapsed)
         ;
    M_out( L"Raster  : bytes written = %s (%.3fMB)  bytes/sec = %.1f (%.3f MB/sec)")
         % M_sep(raster.bytes_written)
         % M_meg(raster.bytes_written)
         % (((real_T)(raster.bytes_written)) / elapsed)
         % (((real_T)(raster.bytes_written)) / (elapsed * 1024576.0))
         ;

    return 0; 
}
M_endf







//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       writeout_file() -- write out data for one file from raster_S data  -- internal function
//||  
//||          -- processes NxM square degrees at a time  
//||          -- input lat/lon is in arc-sec
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int writeout_file(raster_S& raster, hgt_S& hgt, ll_T lat, ll_T lon, ll_T extent_ns, ll_T extent_ew, const writeout_raster_parm_S& wparm, const fileinfo_S& fileinfo) try    
{
    if (wparm.debug_tile)
    {
        M_out(L"writeout_file():  lat=%d (%.16f deg)  lon=%d (%.16f deg)  ns=%d (%.16f deg)  ew=%d (%.16f deg)") % lat % M_l_to_lr(lat) % lon % M_l_to_lr(lon) % extent_ns % M_l_to_lr(extent_ns) % extent_ew % M_l_to_lr(extent_ew);
    }

    // copy data from raster_S into hgt_S buffer -- data points are known to match exactly
    // -----------------------------------------------------------------------------------

    get_raster_data(raster, hgt, lat,lon, extent_ns, extent_ew, wparm, fileinfo);


    // do write suppress and empty height setting, as required
    // -------------------------------------------------------

    bool valid_data_found {false}; 

    if ( (wparm.suppress_empty) || (wparm.set_empty_height) )
    {
        if (!wparm.set_empty_height)
        {
            // pure suppress empty loop -- no empty point height substitution -- can exit as soon as 1st non-empty point is found
        
            for (auto row = 0; row < hgt.grid.rows; row++)
            {
                for (auto col = 0; col < hgt.grid.cols; col++)
                {
                     if (RASTER_EMPTY != M_hgt_ht(hgt, row, col))
                     {
                        valid_data_found = true; 
                        goto loop_exit;
                     }
                }              
            }   

            loop_exit:;
        }
        else     // need to any replace empty points with specified height value 
        {
            for (auto row = 0; row < hgt.grid.rows; row++)
            {
                for (auto col = 0; col < hgt.grid.cols; col++)
                {
                     if (RASTER_EMPTY != M_hgt_ht(hgt, row, col))
                         valid_data_found = true;                            // indicate non-empty point was found
                     else
                         M_hgt_ht(hgt, row, col) = wparm.empty_height;       // replace empty point with passed-in height value
                }              
            } 
        }         
    }
    
    // ?? may need to set empty points to non-0 -- missing lake superior file ????


    // write out this file from filled-in hgt_S, or suppress the write
    // ---------------------------------------------------------------

    if ( wparm.suppress_empty && (!valid_data_found) ) 
    {
        raster.files_suppressed++; 
    }
    else
    {
        auto whrc = writeout_hgt(hgt, wparm, fileinfo);
        if (whrc != 0)
            return -1; 

       raster.files_written++; 
       raster.bytes_written += hgt.ct;
    }


    // write was OK or suppressed -- return normally
    // ---------------------------------------------

    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_raster_data() -- copy data from raster_S buffer to hgt_S area for writing to the file
//||                           
//||                            note: simplified vesion of get_raster_data() , where points in hgt_S are exactly lined up with those in the raster_S
//||
//||         
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


//                         input lat/lon are degrees*3600 to match raster and hgt south, west, etc.

static void get_raster_data(raster_S& raster, hgt_S& hgt, ll_T south_in, ll_T west_in, ll_T extent_ns_in, ll_T extent_ew_in, const writeout_raster_parm_S& wparm, const fileinfo_S& fileinfo) try
{
    // for output, raster_S and hgt_S will have same ppd, pixel alignment, endian-ness, etc., so pixels will align up exactly  

    if (wparm.debug_tile)
    {
        M_out( L"get_raster_data() -- south_in=%d (%.16f deg)  west_in=%d (%.16f deg)  extent_ns_in=%d (%.16f deg)  extent_ew_in=%d (%.16f deg)")
             % south_in     % M_l_to_lr(south_in) 
             % west_in      % M_l_to_lr(west_in) 
             % extent_ns_in % M_l_to_lr(extent_ns_in) 
             % extent_ew_in % M_l_to_lr(extent_ew_in) 
             ;
    }  


    //  Adjust south, west, north, east edges of window to the intersection of the incoming window and the raster grid (handles non-standard origin)   -- ??????? is this needed ??????
    //  --------------------------------------------------------------------------------------------------------------

    ll_T south_r   = south_in; 
    ll_T west_r    = west_in; 
    ll_T extent_ns = extent_ns_in; 
    ll_T extent_ew = extent_ew_in; 
    ll_T north_r   = south_r + extent_ns; 
    ll_T east_r    = west_r  + extent_ew; 

    south_r   = M_max(south_r, raster.grid.south);  
    north_r   = M_min(north_r, raster.grid.north); 
    west_r    = M_max(west_r , raster.grid.west );  
    east_r    = M_min(east_r , raster.grid.east );

    extent_ns = north_r - south_r;      // recompute these after adjusting the limits   
    extent_ew = east_r  - west_r ;      // can't use M_dif_l() here, since we wan't bad neg value if east <= west    

    if (wparm.debug_tile)
    {
        M_out(  L"get_raster_data() -- south_r=%d (%.16f deg)  west_r=%d (%.16f deg)  north_r=%d (%.16f deg)  east_r=%d (%.16f deg) extent_ns=%d (%.16f deg)  extent_ew=%d (%.16f deg)")
              % south_r   % M_l_to_lr(south_r) 
              % west_r    % M_l_to_lr(west_r) 
              % north_r   % M_l_to_lr(north_r) 
              % east_r    % M_l_to_lr(east_r) 
              % extent_ns % M_l_to_lr(extent_ns) 
              % extent_ew % M_l_to_lr(extent_ew) 
              ;
 


        if ( (extent_ns < extent_ns_in) || (extent_ew < extent_ew_in) )
        {
            M_out( L"get_raster_data() -- clipping: extent_ns=%d (%.16f deg)  extent_ew=%d (%.16f deg)  extent_ns_in=%d (%.16f deg)  extent_ew_in=%d (%.16f deg)")
                 % extent_ns    % M_l_to_lr(extent_ns   ) 
                 % extent_ew    % M_l_to_lr(extent_ew   ) 
                 % extent_ns_in % M_l_to_lr(extent_ns_in) 
                 % extent_ew_in % M_l_to_lr(extent_ew_in) 
                 ;          
        }        
    }


    // make sure window has a valid intersection with the raster grid (use nominal limits)     ????? do we need this ????

    if ( (extent_ns <= 0) || (extent_ew <= 0) )
    {
        M_out_emsg1( L"get_raster_data(): south_in=%d (%.16f deg)  west_in=%d (%.16f deg)  extent_ns_in=%d (%.16f deg)  extent_ew_in=%d (%.16f deg)")
                   % south_in     % M_l_to_lr(south_in) 
                   % west_in      % M_l_to_lr(west_in) 
                   % extent_ns_in % M_l_to_lr(extent_ns_in) 
                   % extent_ew_in % M_l_to_lr(extent_ew_in) 
                   ;
        M_out_emsg2( L"get_raster_data(): raster.grid.south=%d (%.16f deg)  raster.grid.west=%d (%.16f deg)  raster.grid.north=%d (%.16f deg)  raster.grid.east=%d (%.16f deg)")
                   % raster.grid.south % M_l_to_lr(raster.grid.south) 
                   % raster.grid.west  % M_l_to_lr(raster.grid.west)
                   % raster.grid.north % M_l_to_lr(raster.grid.north)
                   % raster.grid.east  % M_l_to_lr(raster.grid.east)
                   ;
        M_throw("get_raster_data() :  requested window does not intersect raster grid")
        return;           // should not get here
    }

    
    // make sure values are in limits now (no SIGSEGV) -- use nominal limits/edges, etc. -- ignore pixel offsets

    if (south_r < raster.grid.south)
    {
        M_throw_v("set_raster_data(): South edge of window area (%d) less than south edge of raster data (%d)") % south_r % raster.grid.south ));
        return;    // should not get here 
    }

    if (north_r > raster.grid.north)
    {
        M_throw_v("set_raster_data(): North edge of window area (%d) more than north edge of raster data (%d)") % north_r % raster.grid.north));
        return;    // should not get here 
    }

    if (west_r < raster.grid.west)
    {
        M_throw_v("set_raster_data(): West edge of window area (%d) less than west edge of raster data (%d)") % west_r % raster.grid.west));
        return;    // should not get here 
    }

    if (east_r > raster.grid.east)
    {
        M_throw_v("set_raster_data(): East edge of window area (%d) more than east edge of raster data (%d)") % east_r % raster.grid.east));
        return;   // should not get here 
    }


    //  Get corresponding start hgt_S longitude/latitude that corresponds to incoming raster grid longitude/latitude (these are nominal lat/lon -- no pixel offsets in raster_S are accounted for here)
   
    ll_T south_h = south_r; 
    ll_T north_h = north_r;
    ll_T west_h  = west_r;  

    int rc = adjust_longitude(hgt.tile, south_h, west_h, fileinfo);     // updates west_h, if required  
    if (rc != 0) 
        M_throw_v("get_raster_data(): non-zero R/C (%d) from adjust_longitude()") % rc)); 

    ll_T east_h = west_h + extent_ew;                         // known not to go off edge, or wrap around, etc.
    
    if (wparm.debug_tile)
    {
        M_out(  L"get_raster_data() -- south_h=%d (%.16f deg)  west_h=%d (%.16f deg)  north_h=%d (%.16f deg)  north_h=%d (%.16f deg)")
             % south_h   % M_l_to_lr(south_h) 
             % west_h    % M_l_to_lr(west_h) 
             % north_h   % M_l_to_lr(north_h) 
             % east_h    % M_l_to_lr(east_h) 
             ;
    }
    

    // --------------------------------------------------------
    // Set up hgt_S for data that will be copied in from raster
    // --------------------------------------------------------
     
    reuse_hgt(hgt, south_h, west_h);         // repositions grid for hgt_S to match current position
    
    if (wparm.debug_tile)
    {
        M_out(L"get_raster_data() -- south_h=%d    hgt.grid.north=%d   hgt.grid.pts_per_deg_ns=%d") % south_h   % hgt.grid.north % hgt.grid.pts_per_deg_ns;
        M_out(L"get_raster_data() -- west_h =%d    hgt.grid.west=%d    hgt.grid.pts_per_deg_ew=%d") % west_h    % hgt.grid.west  % hgt.grid.pts_per_deg_ew;
    }


    // -----------------------------------------------------------------------------
    // Set up raster access loop controls -- step is always 1 column -- always exact (any fractional row/col is done on the .hgt buffer access)
    // -----------------------------------------------------------------------------
    
    // compute offsets of NxM degree window within raster coverage area 

    rc_T row_offset_r = (rc_T)(((raster.grid.north - north_r         ) * raster.grid.pts_per_deg_ns) / M_arcsec_1); 
    rc_T col_offset_r = (rc_T)(((west_r            - raster.grid.west) * raster.grid.pts_per_deg_ew) / M_arcsec_1); 


    // data copy is 1-1 between raster and hgt_S -- number of rows/cols to copy = hgt.grid.rows/cols

    rc_T rows = hgt.grid.rows; 
    rc_T cols = hgt.grid.cols;

    if (wparm.debug_tile)
    {
        M_out(L"get_raster_data() -- raster row/col offset = %d/%d   raster rows/cols = %d/%d") % row_offset_r % col_offset_r % rows % cols; 
    }


    /////////////////////////////////////////////////////
    //   copy data from raster_S to hgt_S
    /////////////////////////////////////////////////////

    rc_T row_h = 0;                                                                  // always start with 1st row in hgt_S buffer 

    for (rc_T row_r = row_offset_r; row_r < row_offset_r + rows; row_r++)          // outer loop for each row in hgt data 
    {        
        // ----------------------------------------------------------------------------------------------------------
        // not skipping any columns in the hgt_S data -- memcpy() can be used in place of inner loops for each column
        // ----------------------------------------------------------------------------------------------------------

        // little endian .hgt data -- can move whole row in, since bytes in the row are in same order in hgt_S data and raster buffer

        int16_t   *p0  = (int16_t *)&M_raster_ht(raster, row_r, col_offset_r);           // ptr to 1st integer in row subset      (low part of int16) -- little endian
        int16_t   *hp0 = (int16_t *)M_hgt_ht0_p(hgt, row_h, 0);                          // ptr to 1st byte in row to be copied   (low part of int16) -- little endian

        // copy whole row of bytes from hgt buffer to a subrow in the raster buffer

        memcpy( (void *)hp0                                  // start of hgt_S row == 1st lo byte, etc.
              , (void *)p0                                   // subrow start in raster -- will become 1st lo byte
              , (size_t)((sizeof (int16_t)) * (cols))        // copy whole row from raster_S to hgt_S   
             );


        // advance to next row in hgt_S data    -- must fill every row in hgt_S

        row_h++;  
    }
    
    hgt.ct = 2 * rows * cols;          // update data count in hgt_S    ???????????????? need to use item length rather than "2" ???????????????????
    return;  
}
M_endf   




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_raster() -- empty buffer initialized to 0xBBBB = -30567
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int create_raster(raster_S& raster, const create_raster_parm_S& rparm) try
{    
    int rc {0}; 


    // ••••••••••••••••••••••••••••••••••••••••••••••••••••••••
    // Verify manually-set raster bounds are OK before starting    
    // ••••••••••••••••••••••••••••••••••••••••••••••••••••••••         

    if  (rparm.lat_s < -90) 
    {
        M_throw_v("create_raster(): South edge for raster is too low: %d") % rparm.lat_s ));   
        return -1;    // shouldn't get here
    }

    if  (rparm.lon_w < -360)     
    {
        M_throw_v("create_raster(): West edge for raster is too low: %d") % rparm.lon_w ));    
        return -1;    // shouldn't get here
    }

    if  (rparm.lat_s + rparm.deg_ns > 90) 
    {
        M_throw_v("create_raster(): North edge for raster is too high: %d + %d = %d:") % rparm.lat_s % rparm.deg_ns % (rparm.lat_s + rparm.deg_ns) ));    
        return -1;    // shouldn't get here
    }

    if  (rparm.lon_w + rparm.deg_ew > 540) 
    {
        M_throw_v("create_raster(): East edge for raster is too high: %d + %d = %d:") % rparm.lon_w % rparm.deg_ew % (rparm.lon_w + rparm.deg_ew) ));    
        return -1;    // shouldn't get here
    }
    

    // create raster imbedded-grid based on raster parms  (this grid covers whole rectangular raster)
    
    init_grid( raster.grid                                 
             , M_ld_to_l(rparm.lat_s )
             , M_ld_to_l(rparm.lon_w )
             , M_ld_to_l(rparm.deg_ns)
             , M_ld_to_l(rparm.deg_ew)
             , rparm.ppd_ns
             , rparm.ppd_ew
             , rparm.extra_row_col
             , rparm.extra_row_col              // pixels always aligned, if raster has extra row/col
             );    
   

    // verify that requested window size is OK for grid extent and limits

    rc = check_window( raster.grid, M_ld_to_l(rparm.window_ns), M_ld_to_l(rparm.window_ew) , L"window"); 

    raster.window_ns = M_ld_to_l(rparm.window_ns);
    raster.window_ew = M_ld_to_l(rparm.window_ew); 


    // get data buffers, etc.  -- 

    create_raster_data(raster); 


   // display initialized raster, if requested

   if (rparm.display)
       display_raster(raster);   

    return rc;        
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_auto_raster() -- set raster limits from from passed-in mapdata_t area
//||
//||
//||        gets ppd from raster parms
//||        gets n, s, e, w and sizes from map
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int create_auto_raster(raster_S& raster, const create_raster_parm_S& rparm, const mapdata_S& mapdata) try
{
    int rc {0};

    lld_T south_deg , north_deg, west_deg, east_deg;     // raster limits (in whole degrees) 
  

    // get lat/lon limits based on passed-in map  (rounded to nearest degree)

    south_deg = (lld_T)std::floor(mapdata.min_latitude );
    north_deg = (lld_T)std::ceil( mapdata.max_latitude );
    west_deg  = (lld_T)std::floor(mapdata.min_longitude);
    east_deg  = (lld_T)std::ceil( mapdata.max_longitude);
  

    // Make sure limits are valid for raster

    south_deg = M_lim(-90, south_deg, 90);
    north_deg = M_lim(-90, north_deg, 90);
    
    if (east_deg <= west_deg)                          // make sure east edge is numerically higher than west edge -- assume one wrap of longitude -180
        east_deg += 360;

    if (rparm.display)
    {
        M_out(L"create_auto_raster():             map_min_lat=%.16f  map_max_lat=%.16f   map_min_lon=%.16f   map_max_lon=%.16f") % mapdata.min_latitude % mapdata.max_latitude % mapdata.min_longitude % mapdata.max_longitude;
        M_out(L"create_auto_raster():             south_deg=%d   north_deg=%d   west_deg=%d   east_deg=%d") % south_deg % north_deg % west_deg % east_deg;
    }

    // round auto limits just computed to make them even multiples of the window size

    south_deg =  M_rounddown( (south_deg + 90 ), rparm.window_ns )- 90 ; 
    north_deg =  M_roundup(   (north_deg + 90 ), rparm.window_ns )- 90 ; 
    west_deg  =  M_rounddown( (west_deg  + 180), rparm.window_ew )- 180; 
    east_deg  =  M_roundup(   (east_deg  + 180), rparm.window_ew )- 180;  
  
    if (rparm.display)
    {
        M_out(L"create_auto_raster(): adjusted -- south_deg=%d   north_deg=%d   west_deg=%d   east_deg=%d") % south_deg % north_deg % west_deg % east_deg;
        M_out(L"create_auto_raster():             window_ns=%d   window_ew=%d") % rparm.window_ns % rparm.window_ew; 
    }


   // create grid based on computed auto limits   
       
   init_grid( raster.grid 
            , M_ld_to_l(south_deg)
            , M_ld_to_l(west_deg )
            , M_ld_to_l(north_deg ) - M_ld_to_l(south_deg)
            , M_ld_to_l(east_deg  ) - M_ld_to_l(west_deg )      // east already known to be higher, from earlier adjustment
            , rparm.ppd_ns
            , rparm.ppd_ew
            , rparm.extra_row_col
            , rparm.extra_row_col                              // pixels always aligned when raster has extra row/col
            );    
  

   // verify that requested window size is OK for grid extent and limits -- this should not fail due to prior autolimits computations

   rc = check_window(raster.grid, M_ld_to_l(rparm.window_ns), M_ld_to_l(rparm.window_ew), L"window");  

   raster.window_ns = M_ld_to_l(rparm.window_ns);
   raster.window_ew = M_ld_to_l(rparm.window_ew);  


   // Get data buffers, etc.

   create_raster_data(raster); 
   raster.autolim = true;                                         // this raster was created with map-based auto limits


   // display initialized raster, if requested

   if (rparm.display)
       display_raster(raster); 

   return rc;      
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_raster_data() -- get buffers, etc based on partly-filled-in raster from caller
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void create_raster_data(raster_S& raster) try
{
    raster.filled  = false;
    raster.autolim = false;
     

    // Statistics area to be filled in as data from .hgt files is added to the raster

    raster.hgt_ct         = 0; 
    raster.col_min        = raster.grid.cols + 1;   // will get set on first data add
    raster.col_max        = -1;                     // will get set on first data add
    raster.row_min        = raster.grid.rows + 1;   // will get set on first data add
    raster.row_max        = -1;                     // will get set on first data add
         
    M__(M_out( L"create_raster_data()  :  grid.npts=%d  rows=%d  cols=%d  South=%d (%.16f deg) North=%d (%.16f deg)  West=%d (%.16f deg) East=%d (%.16f deg)")
             % raster.grid.npts
             % raster.grid.rows  
             % raster.grid.cols  
             % raster.grid.south % M_l_to_lr(raster.grid.south)
             % raster.grid.north % M_l_to_lr(raster.grid.north)
             % raster.grid.west  % M_l_to_lr(raster.grid.west )
             % raster.grid.east  % M_l_to_lr(raster.grid.east )
             ;  
       ) 


    // Clear out all stat counters in raster_S   

    raster.windows_done               = 0;
    raster.bytes_read                 = 0;
    raster.files_skipped              = 0;
    raster.files_read                 = 0;
    raster.files_reused               = 0;
    raster.files_missing              = 0;

    raster.basefill_files_read        = 0;
    raster.basefill_files_missing     = 0;

    raster.voidfill_files_read        = 0;
    raster.voidfill_files_missing     = 0;
    raster.voidfills_needed           = 0;
    raster.voidfills_not_needed       = 0;

    raster.emptyfill_files_read       = 0;
    raster.emptyfill_files_missing    = 0;
    raster.emptyfills_needed          = 0;
    raster.emptyfills_not_needed      = 0;

    raster.windows_whole_rows         = 0;
    raster.windows_exact_coverage     = 0;
    raster.windows_inexact_coverage   = 0;
                                   
    raster.set_whole_row_le           = 0;
    raster.set_whole_row_be           = 0;
    raster.set_aligned_le             = 0;
    raster.set_aligned_be             = 0;
    raster.set_nearest_neighbor_le    = 0;
    raster.set_nearest_neighbor_be    = 0;
    raster.set_interpolated_ble       = 0;  

    raster.voidpoints                 = 0;
    raster.emptypoints                = 0;

    
    // make sure raster is not too large   (8G is max points = 16GB) -- allocate raster_S data buffer based on computed sizes

    if (raster.grid.npts > (8ULL * M_1G))
    {
         M_out_emsg1(L"create_raster_data(): **********************************************************************************************");
         M_out_emsg0(L"create_raster_data(): ******************** requested raster size too large -- %u points *******************") % raster.grid.npts;
         M_out_emsg2(L"create_raster_data(): **********************************************************************************************");
         M_throw_v("create_raster_data() -- requested raster size too large -- %u points") % raster.grid.npts ));     
         return;      // should not get here 
    }

    raster.buf.alloc(raster.grid.npts);
    raster.buf.assign1(0xBB );                  // set all items to to missing data -- ht = 0xBBBB = RASTER_EMPTY


    // always display one-line raster data summary

    M_out( L"Raster  : size=%.3fMB  rows=%d  cols=%d  South=%d (%.16f deg) North=%d (%.16f deg)  West=%d (%.16f deg) East=%d (%.16f deg)")
         % M_meg(raster.buf.sz1)
         % raster.grid.rows  
         % raster.grid.cols  
         % raster.grid.south % M_l_to_lr(raster.grid.south)
         % raster.grid.north % M_l_to_lr(raster.grid.north)
         % raster.grid.west  % M_l_to_lr(raster.grid.west )
         % raster.grid.east  % M_l_to_lr(raster.grid.east )
         ;

    return;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       smooth_raster()
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void smooth_raster(raster_S& raster, int32_t passes) try
{
    M_start_timer(clk, tstart)

    M_y(M_out(L"smooth_raster(): starting");)


    rc_T row_max = raster.grid.rows - 1; 
    rc_T col_max = raster.grid.cols - 1;


    // outer loop to do N passes of smoothing
    // --------------------------------------

    uint64_t points { 0 }; 

    for (auto pass = 0; pass < passes; pass++)
    {
        M_y(M_out(L"smooth_raster(): pass = %d/%d") % (pass + 1) % passes;)



        // middle loop to smooth by rows
        // -----------------------------   
    
        for (auto row = 1; row < row_max; row++) 
        {
            ht_T *h1_p = &M_raster_ht(raster, row, 0);
            ht_T *h2_p = &M_raster_ht(raster, row, 1);
            ht_T *h3_p = &M_raster_ht(raster, row, 2); 

            for (auto col = 1; col < col_max; col++)
            {
                int h1 = (int)(*h1_p); 
                int h2 = (int)(*h2_p);
                int h3 = (int)(*h3_p);

                if ( 
                     (h1 > RASTER_MAX_BAD)  
                     &&
                     (h2 > RASTER_MAX_BAD) 
                     &&
                     (h3 > RASTER_MAX_BAD) 
                   )
                {
                    *h2_p = (ht_T)M_round(((real_T)(h1 + h2 + h3))/3.0);                            
                }

                h1_p = h2_p; 
                h2_p = h3_p; 
                h3_p = &M_raster_ht(raster, row, col+1);
             }        
        }


        // middle loop to smooth by columns
        // --------------------------------   
    
        for (auto col = 1; col < col_max; col++) 
        {
            ht_T *h1_p = &M_raster_ht(raster, 0, col);
            ht_T *h2_p = &M_raster_ht(raster, 1, col);
            ht_T *h3_p = &M_raster_ht(raster, 2, col);   
 
            for (auto row = 1; row < row_max; row++)
            {
                int h1 = (int)(*h1_p); 
                int h2 = (int)(*h2_p);
                int h3 = (int)(*h3_p);

                if ( 
                     (h1 > RASTER_MAX_BAD)  
                     &&
                     (h2 > RASTER_MAX_BAD) 
                     &&
                     (h3 > RASTER_MAX_BAD) 
                   )
                {
                    *h2_p = (ht_T)M_round(((real_T)(h1 + h2 + h3))/3.0);       
                }

                h1_p = h2_p; 
                h2_p = h3_p; 
                h3_p = &M_raster_ht(raster, row+1, col);
            }        
        } 

        points += (2 * (col_max - 1) * (row_max - 1));
    }

    auto elapsed = M_read_timer(clk, tstart);
    M_y(M_out(L"smooth_raster(): ending  --  points smoothed = %s  --  elapsed time = %.3f sec  --  points/sec = %s") % M_sep(points) % elapsed %  M_sep( (uint64_t)(((real_T)points)/elapsed) ) ; )

    return;
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       display_raster()
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_raster(const raster_S& raster) try
{
    // display raster flags

    M_out(L"raster.filled                  = %s"               )                   % M_bool_cstr(raster.filled    )                       ;
    M_out(L"raster.complete                = %s"               )                   % M_bool_cstr(raster.complete  )                       ;
    M_out(L"raster.autolim                 = %s"               )                   % M_bool_cstr(raster.autolim   )                       ;
    M_out(L"raster.window_ns               = %-7d (%.16f deg)" )                   % raster.window_ns    % M_l_to_lr(raster.window_ns)    ;
    M_out(L"raster.window_ew               = %-7d (%.16f deg)" )                   % raster.window_ew    % M_l_to_lr(raster.window_ew)    ;


    // display imbedded grid info

    display_grid(raster.grid);

    M_out(L"raster.little_endian           = %s"               )                   % M_bool_cstr(raster.little_endian  )                  ;
    M_out(L"raster.big_endian              = %s"               )                   % M_bool_cstr(raster.big_endian     )                  ;
    M_out(L"raster.big_int_16              = %s"               )                   % M_bool_cstr(raster.int_16     )                      ;
    M_out(L"raster.big_float_32            = %s"               )                   % M_bool_cstr(raster.float_32   )                      ;


    // display main raster statistics

    M_out(L"raster.hgt_ct                  = %d"               )                   % raster.hgt_ct                                        ;
    M_out(L"raster.col_min                 = %d"               )                   % raster.col_min                                       ;
    M_out(L"raster.col_max                 = %d"               )                   % raster.col_max                                       ;
    M_out(L"raster.row_min                 = %d"               )                   % raster.row_min                                       ;
    M_out(L"raster.row_max                 = %d"               )                   % raster.row_max                                       ;
                                                              
                                                              
    // display raster buffer info                             
                                                              
    M_out(L"raster.buf.p                   = %p"               )                   % (void *)(raster.buf.p )                              ;
    M_out(L"raster.buf.p1                  = %p"               )                   % (void *)(raster.buf.p1)                              ;
    M_out(L"raster.buf.sz1                 = %s = %.3fMB"      )                   % M_sep(raster.buf.sz1) %  M_meg(raster.buf.sz1)       ;
    M_out(L"raster.buf.sz                  = %s = %.3fM"       )                   % M_sep(raster.buf.sz ) %  M_meg(raster.buf.sz )       ;
                                                              
                                                              
    // display other raster statistics                        
                                                              
    M_out(L"raster.windows_done            = %d"               )                   % raster.windows_done                                  ;
    M_out(L"raster.bytes_read              = %d"               )                   % raster.bytes_read                                    ;
    M_out(L"raster.files_read              = %d"               )                   % raster.files_read                                    ;
    M_out(L"raster.files_skipped           = %d"               )                   % raster.files_skipped                                 ;
    M_out(L"raster.files_reused            = %d"               )                   % raster.files_reused                                  ; 
    M_out(L"raster.files_missing           = %d"               )                   % raster.files_missing                                 ; 
    
    M_out(L"raster.bytes_written           = %d"               )                   % raster.bytes_written                                 ;
    M_out(L"raster.files_written           = %d"               )                   % raster.files_written                                 ;
    M_out(L"raster.files_suppressed        = %d"               )                   % raster.files_suppressed                              ;

    M_out(L"raster.basefill_files_read     = %d"               )                   % raster.basefill_files_read                           ;
    M_out(L"raster.basefill_files_missing  = %d"               )                   % raster.basefill_files_missing                        ;  
                                                              
    M_out(L"raster.voidfill_files_read     = %d"               )                   % raster.voidfill_files_read                           ;
    M_out(L"raster.voidfill_files_missing  = %d"               )                   % raster.voidfill_files_missing                        ;
    M_out(L"raster.voidfills_needed        = %d"               )                   % raster.voidfills_needed                              ;
    M_out(L"raster.voidfills_not_needed    = %d"               )                   % raster.voidfills_not_needed                          ;
                                                              
    M_out(L"raster.emptyfill_files_read    = %d"               )                   % raster.emptyfill_files_read                          ;
    M_out(L"raster.emptyfill_files_missing = %d"               )                   % raster.emptyfill_files_missing                       ;
    M_out(L"raster.emptyfills_needed       = %d"               )                   % raster.emptyfills_needed                             ;
    M_out(L"raster.emptyfills_not_needed   = %d"               )                   % raster.emptyfills_not_needed                         ;
                                                                                                                                                                                                                                                                                  
    M_out(L"windows_whole_rows             = %d"               )                   % raster.windows_whole_rows                            ;
    M_out(L"windows_exact_coverage         = %d"               )                   % raster.windows_exact_coverage                        ;
    M_out(L"windows_inexact_coverage       = %d"               )                   % raster.windows_inexact_coverage                      ;
                                                                                                                               
    M_out(L"set_whole_row_le               = %d"               )                   % raster.set_whole_row_le                              ;
    M_out(L"set_whole_row_be               = %d"               )                   % raster.set_whole_row_be                              ;
    M_out(L"set_aligned_le                 = %d"               )                   % raster.set_aligned_le                                ;
    M_out(L"set_aligned_be                 = %d"               )                   % raster.set_aligned_be                                ;
    M_out(L"set_nearest_neighbor_le        = %d"               )                   % raster.set_nearest_neighbor_le                       ;
    M_out(L"set_nearest_neighbor_be        = %d"               )                   % raster.set_nearest_neighbor_be                       ;
    M_out(L"set_interpolated_ble           = %d"               )                   % raster.set_interpolated_ble                          ; 
                                                             
    M_out(L"raster.voidpoints              = %d"               )                   % raster.voidpoints                                    ;
    M_out(L"raster.emptypoints             = %d"               )                   % raster.emptypoints                                   ;

    return; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       destroy_raster()
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void destroy_raster(raster_S& raster) try
{
    raster.buf.free(); 
    return; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        check_window() -- make sure window/step N-S and E-W extents are usable with this raster/grid  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int check_window(const grid_S& grid, ll_T window_ns, ll_T window_ew, const std::wstring& ws) try
{
    int rc {0}; 


    // check N-S window extent
    // -----------------------
    //
    //  - nominal grid extent must be even multiple of nominal window extent
    //  - nominal south edge (-90 origin) must be an even multiple of nominal window extent

    if (grid.extent_ns % window_ns != 0)
    {
        M_out_emsg(L"check_window() -- N-S grid extent (%d arc-sec = %.16f degrees) is not an even multiple of N-S %S extent (%d arc-sec = %.16f degrees)") 
                  % grid.extent_ns % M_l_to_lr(grid.extent_ns) 
                  % ws
                  % window_ns      % M_l_to_lr(window_ns     ) 
                  ;
    
        rc = -1;  
    }

    if ( (grid.south + M_arcsec_90) % window_ns != 0 )
    {
        M_out_emsg(L"check_window() -- south edge of grid (%d arc-sec = %.16f degrees) is not an even multiple of N-S %S extent (%d arc-sec = %.16f degrees)") 
                  % grid.south % M_l_to_lr(grid.south) 
                  % ws
                  % window_ns  % M_l_to_lr(window_ns ) 
                  ;
    
        rc = -1;  
    }


    // check E-W window extent
    // -----------------------
    //
    //  - nominal grid extent must be even multiple of nominal window extent
    //  - nominal west edge (-180 origin) must be an even multiple of nominal window extent

    if (grid.extent_ew % window_ew != 0)
    {
        M_out_emsg(L"check_window() -- E-W grid extent (%d arc-sec = %.16f degrees) is not an even multiple of E-W %S extent (%d arc-sec = %.16f degrees)") 
                  % grid.extent_ew % M_l_to_lr(grid.extent_ew) 
                  % ws
                  % window_ew      % M_l_to_lr(window_ew     ) 
                  ;
    
        rc = -1;  
    }

    if ( (grid.west + M_arcsec_180) % window_ew != 0 )
    {
        M_out_emsg(L"check_window() -- west edge of grid (%d arc-sec = %.16f degrees) is not an even multiple of E-W %S extent (%d arc-sec = %.16f degrees)") 
                  % grid.west % M_l_to_lr(grid.west)  
                  % ws
                  % window_ew % M_l_to_lr(window_ew) 
                  ;
    
        rc = -1;  
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