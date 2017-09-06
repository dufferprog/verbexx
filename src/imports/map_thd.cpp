// map_thd.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===========
////     map_thd.cpp -- Thread-oriented functions
////     ===========
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

#include "h_map_types.h"
#include "h_map_global.h"
#include "h_map_parms.h"
#include "h_map_thd.h"


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       slice_area() -- create thread parms with slicing an area by by row and column 
//||
//||
//||        thpa = array of THREAD_MAX thp_t structures (output area -- filled in)
//||
//||        thd = true if slicing for a thread, false if nonthread slicing
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


void slice_row_col(thpa_S& thpa, rc_T rmin, rc_T rmax, rc_T cmin, rc_T cmax, int32_t in_slices, bool is_thd, const mapparm_S& parm) try
{   
    const mapparm_S * parm_p = &parm;  //// temp

    // select thread vs non-thread parms based on input parm

    bool    display; 
    int64_t area; 
    int32_t max_slices;

    if (is_thd)
    {
        display    = parm_p->thread.display;
        area       = parm_p->slice.thread_area; 
        max_slices = M_max(1, M_min(parm_p->thread.max, in_slices)); 
    }
    else
    {
        display    = parm_p->slice.display;
        area       = parm_p->slice.nonthread_area;
        max_slices = M_max(1, M_min(parm_p->slice.max, in_slices));
    }


    //  compute max number of slices, based on caller's parms
    //  -----------------------------------------------------

    int32_t rows = (rmax - rmin) + 1;                    // number of rows to do 
    int32_t cols = (cmax - cmin) + 1;                    // number of columns to do 
    
    int32_t slices = (((int64_t)rows) * (int64_t)cols) / area;           // try for not more than one slice per xxxx points
    slices = M_lim(1, slices, max_slices);                               // limit threads to use -- between 1 and caller's max_slices 

    M__(M_out(L"slice_rc(): max_slices=%d  slices=%d") % max_slices % slices;)
    

    // find aspect ration of passed-in region

    int32_t dh = M_max(rows, cols);          // larger  dimension
    int32_t dl = M_min(rows, cols);          // smaller dimension
    real_T  dr = (real_T)dh / (real_T)dl;    // aspect ratio
       
        
    //  to get squarish slices, would like to slice region using (dr * sqrt(slices/dr))  by  (sqrt(slices/dr)) slices  
  
    real_T  d1 = dr * sqrt((real_T)slices / dr);
    real_T  d2 = sqrt((real_T)slices / dr);     

    int32_t s1 = (int32_t)M_round(d1);     // should not be 0
    int32_t s2 = (int32_t)M_round(d2);     // may be 0 

    M__(M_out(L"rows/cols = %d/%d  dr=%.6f  slices=%d  d1=%.6f  d2=%.6f  s1=%d   s2=%d") % rows % cols % dr % slices % d1 % d2 % s1 % s2;)
    
    s2 = M_max(1, s2);  
    s1 = M_max(1, slices / s2); 
    int32_t sh = M_max(s1, s2);
    int32_t sl = M_min(s1, s2);


    // add one more to larger dimension, if that does not cause slices to exceed max allowed

    if ((sh + 1) * sl <= slices)
        ++sh; 
    
    slices = sh * sl;    // adjusted number of slices (probably reduced)
    
  
    M__(M_out(L" --- adjusted -- slices = %d:  %dx%d") % slices % sh % sl;)

    int32_t slices_r;
    int32_t slices_c;

    if (rows > cols)
    {
        slices_r = sh; 
        slices_c = sl; 
    }
    else
    {
        slices_c = sh; 
        slices_r = sl;  
    }
        

    thpa.n_threads = slices;                                 // pass back to caller 
          
    if (display)
        M_out(L"Slices  : rows,cols=%dx%d  slices=%d : %dx%d  rows/slice=%d  cols/slice=%d") % rows % cols % slices % slices_r % slices_c % (rows/slices_r) % (cols/slices_c); 


    // Fill in thread parms with rows/column slicing values
    // ----------------------------------------------------

    int32_t th = 0; 

    for (int32_t slice_c = 0; slice_c < slices_c; slice_c++) 
    {
        for (int32_t slice_r = 0; slice_r < slices_r; slice_r++)
        {
            thpa.thp[th].thread_n = th; 

            thpa.thp[th].e_r0 = rmin + (slice_r * rows) / slices_r;
            
            if (slice_r < (slices_r - 1))
                thpa.thp[th].e_rm = rmin - 1 + ((slice_r + 1) * rows) / slices_r;
            else
                thpa.thp[th].e_rm = rmax;    // use exact value for final row
               
            
            // Fill in column limits for this slice
            
            thpa.thp[th].e_c0 = cmin + (slice_c * cols)/slices_c;

            if (slice_c < (slices_c - 1))
                thpa.thp[th].e_cm = cmin - 1 + ((slice_c + 1) * cols) / slices_c;
            else
                thpa.thp[th].e_cm = cmax;                  // use exact value for final row
          

            // Advance to next slot in the thp area

            th++; 
        }
    }
    
#ifdef SHOW_DEBUG 
    for (auto i = 0; i < slices; i++)
    {
       M_out(L"thread=%-2d: r0=%-5d  rm=%-5d  c0=%-5d  cm=%-5d") % i % (thpa.thp[i].e_r0) % thpa.thp[i].e_rm % thpa.thp[i].e_c0 % thpa.thp[i].e_cm;   
    }
#endif

    return;
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       slice_row() -- create thread parms with slicing by row only 
//||
//||
//||        pp -> array of THREAD_MAX thp_t structures (output area -- filled in)
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void slice_row(thpa_S& thpa, rc_T rmin, rc_T rmax, rc_T cmin, rc_T cmax, int32_t in_slices, bool is_thd, const mapparm_S& parm) try 
{
    int32_t threads;
    int32_t slices_r;
    int32_t s, s1, s2; 

    int32_t rows, cols;
    int32_t th; 
    int32_t slice_r; 
    
    
    // select thread vs non-thread parms based on input parm

    bool    display; 
    int64_t area;
    int32_t dim;
    int32_t max_slices;

    if (is_thd)
    {
        display    = parm.thread.display;
        area       = parm.slice.thread_area; 
        dim        = parm.slice.nonthread_dim;
        max_slices = M_max(1, M_min(parm.thread.max, in_slices)); 
    }
    else
    {
        display    = parm.slice.display;
        area       = parm.slice.nonthread_area;
        dim        = parm.slice.nonthread_dim;
        max_slices = M_max(1, M_min(parm.slice.max, in_slices));
    }


    //  compute thread slices, based on caller's parms
    //  ----------------------------------------------

    rows = rmax - rmin + 1;                    // number of rows to do 
    cols = cmax - cmin + 1;                    // number of columns to do 
        
    threads = (((int64_t)rows) * (int64_t)cols) / area;             // try for one thread per xxxxxxx points
    threads = M_min(threads, rows / dim);                           // must have at least xxx rows per thread 
    threads = M_lim(1, threads, max_slices);                        // limit threads to use -- between 1 and caller's max_threads 
       
    slices_r = threads; 
      
    thpa.n_threads = threads;                                 // pass back to caller 
   
    if (display)     
        M_out(L"Slices  : rows,cols=%dx%d  slices=%d : %dx%d  rows/slice=%d  cols/slice=%d") % rows % cols % threads % slices_r % 1 % (rows/slices_r) % cols; 


    // Fill in thread parms with rows/column slicing values
    // ----------------------------------------------------

    th = 0; 
        
    for (slice_r = 0; slice_r < slices_r; slice_r++)
    {
        thpa.thp[th].thread_n = th; 
        thpa.thp[th].e_r0 = rmin + (slice_r * rows) / slices_r;
        
        if (slice_r < (slices_r - 1))
            thpa.thp[th].e_rm = rmin - 1  + ((slice_r + 1) * rows) / slices_r;
        else
            thpa.thp[th].e_rm = rmax;    // use exact value for final row
  
            
        // Fill in column limits for this sclice
            
        thpa.thp[th].e_c0 = cmin;
        thpa.thp[th].e_cm = cmax;                       

        // Advance to next slot in the thp area
  
        th++; 
    }
    

    // debug printout

#ifdef SHOW_DEBUG 
    for (th = 0; th < threads; th++)
    {
       M_out(L"thread=%-2d: r0=%-5d  rm=%-5d  c0=%-5d  cm=%-5d") % th % thpa.thp[th].e_r0 % thpa.thp[th].e_rm % thpa.thp[th].e_c0 % thpa.thp[th].e_cm;   
    }
#endif

    return;
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       slice_col() -- create thread parms with slicing by column only 
//||
//||
//||        pp -> array of THREAD_MAX thp_t structures (output area -- filled in)
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void slice_col(thpa_S& thpa, rc_T rmin, rc_T rmax, rc_T cmin, rc_T cmax, int32_t in_slices, bool is_thd, const mapparm_S& parm) try
{
    int32_t threads;
    int32_t slices_c;
    int32_t s, s1, s2; 

    int32_t rows, cols;
    int32_t th; 
    int32_t slice_c; 
    

    // select thread vs non-thread parms based on input parm

    bool    display; 
    int64_t area;
    int32_t dim;  
    int32_t max_slices;

    if (is_thd)
    {
        display    = parm.thread.display;
        area       = parm.slice.thread_area; 
        dim        = parm.slice.thread_dim;
        max_slices = M_max(1, M_min(parm.thread.max, in_slices)); 
    }
    else
    {
        display    = parm.slice.display;
        area       = parm.slice.nonthread_area;
        dim        = parm.slice.nonthread_dim;
        max_slices = M_max(1, M_min(parm.slice.max, in_slices));
    }


    //  compute thread slices, based on caller's parms
    //  ----------------------------------------------

    rows = rmax - rmin + 1;                    // number of rows to do 
    cols = cmax - cmin + 1;                    // number of columns to do 
    
    threads = (((int64_t)rows) * (int64_t)cols) / area;      // try for one thread per xxxxxxx points
    threads = M_min(threads, cols / dim);                    // must have at least xxx columns per thread 
    threads = M_lim(1, threads, max_slices);                 // limit threads to use -- between 1 and caller's max_threads 
  
    slices_c = threads; 
      
    thpa.n_threads = threads;                                 // pass back to caller 
  
    if (display)          
        M_out(L"Slices  : rows,cols=%dx%d  slices=%d : %dx%d  rows/slice=%d  cols/slice=%d") % rows % cols % threads % 1 % slices_c % rows % (cols/slices_c); 


    // Fill in thread parms with rows/column slicing values
    // ----------------------------------------------------

    th = 0; 

    for (slice_c = 0; slice_c < slices_c; slice_c++) 
    {      
        thpa.thp[th].thread_n = th; 

        thpa.thp[th].e_r0 = rmin;
        thpa.thp[th].e_rm = rmax;    
          
            
        // Fill in column limits for this sclice
            
        thpa.thp[th].e_c0 = cmin + (slice_c * cols)/slices_c;

        if (slice_c < (slices_c - 1))
            thpa.thp[th].e_cm = cmin - 1 + ((slice_c + 1) * cols) / slices_c;
        else
            thpa.thp[th].e_cm = cmax;                  // use exact value for final row
          

        // Advance to next slot in the thp area

        th++; 
    }
  

    // debug prinout

#ifdef SHOW_DEBUG 
    for (th = 0; th < threads; th++)
    {
       M_out(L"thread=%-2d: r0=%-5d  rm=%-5d  c0=%-5d  cm=%-5d") % th % thpa.thp[th].e_r0 % thpa.thp[th].e_rm % thpa.thp[th].e_c0 % thpa.thp[th].e_cm;   
    }
#endif

    return;
}
M_endf



//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""