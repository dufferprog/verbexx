// map_water.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     map_water.cpp -- water detection functions (for the mapdata_t structure) 
////     =============
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
#include "h_map_data.h"
#include "h_map_raster.h"



//  Internal functions
//  ------------------

static void     water_detect(     mapdata_S& , thp_S&, const mapparm_S&); 

static void     water_map3(       mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&); 
static void     water_map5(       mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);  
static void     water_map7(       mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);   
static void     water_map9(       mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);  
static void     water_map11(      mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);  
static void     water_map13(      mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);  
static void     water_map15(      mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&); 
static void     water_map17(      mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);
static void     water_map19(      mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);
static void     water_map21(      mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&); 
static void     water_map23(      mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);
static void     water_map25(      mapdata_S&, thp_S&, int32_t, int32_t, const mapparm_S&);

static void     water_fill(       mapdata_S& , thp_S&, const mapparm_S&); 




// MACROs for restricting lake detection between certain elevations
//
//    Ocean only -- only check below 1 meter
//    Both       -- check if below 1 meter, and between lo and hi bounds
//
//

// MACRO to declare variables for water_mapNN() functions -- not NN-dependent

#define M_DDN(x)   int d##x = x * stride;
#define M_DV(x)    int ht_min_lk, ht_max_lk, ht_max_ocean, was_water; int lake_n = x; int32_t new_ct = 0; int r0 = thp.e_r0; int rm = thp.e_rm; int c0 = thp.e_c0; int cm = thp.e_cm;



// MACRO to generate finishing-up code + return 

#define M_finish()                                                                                                                                                  \
{                                                                                                                                                                   \
    thp.r32 = new_ct;                                                                                                                                               \
                                                                                                                                                                    \
    if (parm.slice.display)                                                                                                                                         \
        {M_out_lk(L"Slice %3d: %dx%d water detect ended -- new_ct=%8d rows=%5d/%5d  cols=%5d/%5d") % lake_n % lake_n % thp.thread_n % new_ct % r0 % rm % c0 % cm;}  \
                                                                                                                                                                    \
    return;                                                                                                                                                         \
}

// MACRO to generate starting code

#define M_start()                                                                                                           \
{                                                                                                                           \
    M_SHT()                                                                                                                 \
                                                                                                                            \
    M__(M_out(L"starting water check %dx%d -- ht_min_lake=%d  ht_max_lake=%d  ht_max_ocean=%d") % lake_n % lake_n % ht_min_lk % ht_max_lk % ht_max_ocean;)  \
    M__(M_out(L"r0=%d rm=%d c0=%d cm=%d") % r0 % rm % c0 % cm;)                                                             \
}





// MACRO to check each height before setting water attribute -- allow check for ht=0 or below if ocean detect is allowed
                       
#define M_CHT(h)   (   ((h) <= ht_max_ocean) || ( ((h) >= ht_min_lk) && ((h) <= ht_max_lk) )   )


// MACRO to set height bounds at start of routine

#define M_SHT()                           \
{                                         \
    if (check == WATER_NOLAKE)            \
    {                                     \
        ht_min_lk = 30000;                \
        ht_max_lk = 30001;                \
    }                                     \
    else if (check == WATER_ICECAP)       \
    {                                     \
        ht_min_lk = parm.water.ht_min;    \
        ht_max_lk = 1000;                 \
    }                                     \
    else  /* must be WATER_LAKE */        \
    {                                     \
        ht_min_lk = parm.water.ht_min;    \
        ht_max_lk = parm.water.ht_max;    \
    }                                     \
    if (parm.water.check_ocean)           \
        ht_max_ocean = 0;                 \
    else                                  \
        ht_max_ocean = -30000;            \
}                                         



#define M_NLE(x) if( ( col > c0 + x) && (row > r0 + x) && ( col < cm - x) && (row < rm - x) )   // don't place new lake too close to any edge -- x = closest to edge parm


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map() -- set water attribute(s) -- call appropriate version on threads 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void water_map(mapdata_S& mapdata, const mapparm_S& parm) try
{
    std::thread thd[THREAD_MAX];
    thpa_S      thpa;                       // thread parms
             
     M__(M_out(L"****************************************************************** water_map() called");)

    // fast return, if no water checking     

    if (
        (parm.water.check_ocean == false)
         &&
        (parm.water.check_lake == WATER_NOLAKE)
       )
        return;
    

    // Do water detection on threads
    // -----------------------------

    M_start_timer(c1, t1)  

    slice_map(mapdata, thpa, parm.thread.n_water, parm); // get thread slice info based on hgt parms -- n threads max for initial water detect

    M_out(L"Water   : ocean/lake detection starting -- %d threads") % thpa.n_threads;

    for (auto i = 0; i < thpa.n_threads; i++)
    {
         thd[i] = std::thread( water_detect,    std::ref(mapdata), std::ref((thpa.thp)[i]), std::cref(parm) );
    }

    M__(M_out(L"Waiting for threads to end");)
        
    for (auto i = 0; i < thpa.n_threads; i++)
    {
        thd[i].join();        
    }
        
    M_out(L"Water   : detection threads ended -- elapsed time = %.3f seconds") %  M_read_timer(c1, t1);

        

    //  Always do water fill on threads -- water detect will do spotty detection if stride > 1
    //  --------------------------------------------------------------------------------------   
   
    M_set_timer(c1, t1); 
   
    slice_map(mapdata, thpa, parm.thread.n_water, parm);   // allow up to n threads for water expansion

    M_out(L"Water   : fill starting -- %d threads") % thpa.n_threads;

    for (auto i = 0; i < thpa.n_threads; i++)
    {
        thd[i] = std::thread( water_fill,   std::ref(mapdata), std::ref((thpa.thp)[i]), std::cref(parm) );
    }

    M__(M_out(L"Waiting for threads to end");)

    for ( auto i = 0; i < thpa.n_threads; i++)
    {
       thd[i].join(); 
    }

    M_out(L"Water   : %d threads ended -- elapsed time = %.3f seconds") % thpa.n_threads % M_read_timer(c1, t1);
     
    return; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       adaptive_water_setup() -- set water attribute(s) -- on thread
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//   Compute min_lake and stride based on DEM data in subslice
//   ---------------------------------------------------------
//   
//   rc = 0 -- OK , rc = -1 -- no valid data -- can bypass water detect
//   modifies check_out, stride_out, and min_lake_out output parms


static int adaptive_water_setup(const mapdata_S& mapdata, const thp_S& thp, int32_t& stride_out, int32_t& min_lake_out, int32_t& check_out, const mapparm_S& parm) try
{
    M__(M_out(L"****************************************************************** adaptive_water_setup() called");)

    
    // get subslice limits from passed-in thp entry
    // --------------------------------------------
    
    rc_T sr0 = thp.e_r0;
    rc_T srm = thp.e_rm;
    rc_T sc0 = thp.e_c0;
    rc_T scm = thp.e_cm;


    //  Get latitude of midpoint and see if this subslice is in the icecap zone -- supress lake detect in icecap zone
    //  -------------------------------------------------------------------------------------------------------------

    if (parm.water.check_lake != WATER_NOLAKE)
    {
        double lat, lon_unwanted; 

        if (
            (   0 == (*(mapdata.get_lat_lon_p))( mapdata, lat, lon_unwanted, 0.5 * (real_T)(sr0 + srm), 0.5 * (real_T)(sc0 + scm), true )    )  
            &&
            (   (lat >= parm.water.a_icecap_lat_n) || (lat <= parm.water.a_icecap_lat_s)   )
           )
        {
            M__(M_out(L"Icecap zone -- center latitude = %.3f") % lat; )
            check_out = WATER_ICECAP;        // suppress lake check above 1000m in icecap zone 
        }
        else
        {
            check_out = parm.water.check_lake;
        }
    }      


    // See if there are any valid points in this subslice
    // --------------------------------------------------

    for (rc_T row = sr0; row <= srm; row++)
    {
        for(rc_T col = sc0; col <= scm; col++)
        {
            attr_T a = M_mapdata_attr(mapdata, row, col); 
            ht_T   h = M_mapdata_ht(  mapdata, row, col);

            if (  ((a & MAPDATA_VALID) != 0) && (h != RASTER_VOID)  )
                goto valid;          
        }          
    }


    //  Return now, if no valid points found in this subslice, or if either subslice dimension is less than 3 (should not occur?) or too small for specified roughness neighborhood size
    //  -----------------------------------------------------------------------------------------------------

    return -1; 
valid:; 

    rc_T xy = parm.water.a_neighbor_xy;  

    if ( (srm - sr0 < 3) || (scm - sc0 < 3) )    // too small for lake detection?
        return -1;

    if ( (srm - sr0 <= 2 * xy + 1) || (scm - sc0 <= 2 * xy + 1) )  // too small for specified roughness check neighborhood?
        return -1;

    
    
    // Look at a sample of 10 * a_samples points not on edge -- save max of a_samples valid roughness values in sample_ht; 
    // -------------------------------------------------------------------------------------------------------------------

    int32_t a_samples = parm.water.a_samples;      

    std::vector<rc_T> sample;
    sample.reserve(a_samples);  
     
    auto rand_row = std::bind( std::uniform_int_distribution<rc_T>{sr0 + xy, srm - xy}      // don't get left/right edge pixels                          
                             , std::default_random_engine{1}
                             ); 
    auto rand_col = std::bind( std::uniform_int_distribution<rc_T>{sc0 + xy, scm - xy}      // don't get top/bottom row pixels                          
                             , std::default_random_engine{1234567890}
                             ); 
                               
    int a_ct = 0; 
      
    for (int i = 0; i < 10 * a_samples; ++i)    // look at a maximum of 10 * a_samples points, trying to find a_samples valid roughness values
    {
       // get a random point and look at it and all 4 corners of roughness neighborhood (make sure all are valid)

       rc_T row = (rc_T)rand_row();
       rc_T col = (rc_T)rand_col();

       attr_T a11 = M_mapdata_attr(mapdata, row - xy, col - xy); 
       ht_T   h11 = M_mapdata_ht(  mapdata, row - xy, col - xy);

       attr_T a13 = M_mapdata_attr(mapdata, row - xy, col + xy); 
       ht_T   h13 = M_mapdata_ht(  mapdata, row - xy, col + xy);

       attr_T a22 = M_mapdata_attr(mapdata, row     , col     ); 
       ht_T   h22 = M_mapdata_ht(  mapdata, row     , col     );
       
       attr_T a31 = M_mapdata_attr(mapdata, row + xy, col - xy); 
       ht_T   h31 = M_mapdata_ht(  mapdata, row + xy, col - xy);

       attr_T a33 = M_mapdata_attr(mapdata, row + xy, col + xy); 
       ht_T   h33 = M_mapdata_ht(  mapdata, row + xy, col + xy);

        if (
            ( ((a11 & MAPDATA_VALID) != 0) && (h11 != RASTER_VOID) )
            &&
            ( ((a13 & MAPDATA_VALID) != 0) && (h13 != RASTER_VOID) )
            &&
            ( ((a22 & MAPDATA_VALID) != 0) && (h22 != RASTER_VOID) )
            &&
            ( ((a31 & MAPDATA_VALID) != 0) && (h31 != RASTER_VOID) )
            &&
            ( ((a33 & MAPDATA_VALID) != 0) && (h33 != RASTER_VOID) )
           ) 
        {
            ht_T h_min = M_min(h22, M_min(M_min(h11, h13 ), M_min(h31, h33)));
            ht_T h_max = M_max(h22, M_max(M_max(h11, h13 ), M_max(h31, h33)));

            sample.push_back(h_max - h_min);    // save away local 3x3 roughess value (diagonals only)       
            ++a_ct;

            // need code to add random sampled points in neighborhood and check for max, min -- use min_max_elem on vector of heights 


        }

        if (a_ct == a_samples) break;      // exit if enough heights saved away; 
    }


    //  Sort 1st half of heights in sample_ht, and get median roughness value of the sample points (may have less than a_samples points)
    //  ------------------------------------------------------------------------------------------

    ht_T roughness; 

    if (a_ct > 2)      // enough points to do partial sort? 
    {
        std::partial_sort( sample.begin()
                         , sample.begin() +  (1 + a_ct / 2)            // sorts from 0 to (a_ct / 2) only            
                         , sample.end()
                         );

        if (a_ct % 2 == 0)                                               // even number of valid samples?
            roughness = (sample[a_ct / 2 - 1] +  sample[a_ct / 2]) / 2;  // get average of two middle ones
        else
            roughness = sample[a_ct / 2];                                // just get middle one
    }
    else   // 0, 1, or 2 elements saved in sample
    {
        if (a_ct == 2)              // only 2 valid points 
            roughness = (sample[0] + sample[1]) / 2;
    
        else if (a_ct == 1)
            roughness = sample[0];  // only roughness value saved (1 valid point in 10000) 

        else                        // couldn't find any valid points after checking 10000        
            roughness = 0;          
    }


    /*
    for (rc_T row = sr0; row <= srm; row++)
    {
        for(rc_T col = sc0; col <= scm; col++)
        {
            attr_T a = M_mapdata_attr(mapdata, row, col); 
            ht_T   h = M_mapdata_ht(  mapdata, row, col);
            
            if (  ((a & MAPDATA_VALID) != 0) && (h != RASTER_VOID)  )
            {
                ++ct_valid; 
                h_min = M_min(h_min, h);
                h_max = M_max(h_max, h);                 
            }
        }          
    }

    ht_T h_diff = h_max - h_min;   
    */

    // compute stride and min_lake to use for water detect  

    int32_t min_lake  = parm.water.a_hi_min_lake;                                 // start with highest parm values before adjustment
    int32_t hi_lo     = parm.water.a_hi_min_lake - parm.water.a_lo_min_lake;     // max - min spread

      
    // adjust min_lake based on median roughness 

    if      (roughness > 10)
        min_lake -= hi_lo;              //12 
    else if (roughness > 8)
        min_lake -= (hi_lo * 10) / 12;  //10 
    else if (roughness > 6)
        min_lake -= (hi_lo * 8 ) / 12;  //8
    else if (roughness > 5) 
        min_lake -= (hi_lo * 6 ) / 12;  //6
    else if (roughness > 4) 
        min_lake -= (hi_lo * 4 ) / 12;  //4 
    else if (roughness > 3) 
        min_lake -= (hi_lo * 2 ) / 12;  //2 
  

    min_lake = 1 + (min_lake / 2) * 2;

    int32_t stride = parm.water.a_hi_stride - M_max(0, roughness - 1);  // roughness r0, r1 : hi_stride, r2 : hi_stride-1, r3 : hi_stride-2, etc.
    

    // make sure adjusted values are not lower than min allowed  

    min_lake = M_max(min_lake, parm.water.a_lo_min_lake);
    stride   = M_max(stride  , parm.water.a_lo_stride  ); 
        
    M__(M_out(L"a_ct = %d  roughness = %d stride=%d  min_lake=%d") % a_ct % (int)roughness % stride % min_lake;)

    stride_out   = stride;
    min_lake_out = min_lake;

    return 0; 

}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_detect() -- set water attribute(s) -- on thread
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


static void water_detect(mapdata_S& mapdata, thp_S& thp_in, const mapparm_S& parm) try
{
    thpa_S      thpa;                       // thread/slice parms
    
    M__(M_out(L"****************************************************************** water_detect() called");)

    
    rc_T r0 = thp_in.e_r0; 
    rc_T rm = thp_in.e_rm;
    rc_T c0 = thp_in.e_c0;
    rc_T cm = thp_in.e_cm;
       

    //  check min_lake_size parms before starting -- can this be redone with constant unordered sets?

    if (!(parm.water.adaptive))
    {
        if (
            (parm.water.min_lake != 3 ) &&
            (parm.water.min_lake != 5 ) &&
            (parm.water.min_lake != 7 ) &&
            (parm.water.min_lake != 9 ) &&
            (parm.water.min_lake != 11) &&
            (parm.water.min_lake != 13) &&
            (parm.water.min_lake != 15) &&
            (parm.water.min_lake != 17) &&
            (parm.water.min_lake != 19) &&
            (parm.water.min_lake != 21) &&
            (parm.water.min_lake != 23) &&
            (parm.water.min_lake != 25)
           )
        {
            M_out_emsg(L"Water detect bypassed -- unexpected value for water detect size parm (%d)") % parm.water.min_lake; 
            return;
        }    
    }
    else // must be adaptive water detect
    {
        if (
            (parm.water.a_lo_min_lake != 3 ) &&
            (parm.water.a_lo_min_lake != 5 ) &&
            (parm.water.a_lo_min_lake != 7 ) &&
            (parm.water.a_lo_min_lake != 9 ) &&
            (parm.water.a_lo_min_lake != 11) &&
            (parm.water.a_lo_min_lake != 13) &&
            (parm.water.a_lo_min_lake != 15) &&
            (parm.water.a_lo_min_lake != 17) &&
            (parm.water.a_lo_min_lake != 19) &&
            (parm.water.a_lo_min_lake != 21) &&
            (parm.water.a_lo_min_lake != 23) &&
            (parm.water.a_lo_min_lake != 25)
           )
        {
            M_out_emsg(L"Water detect bypassed -- unexpected value for adaptive water detect lowest adjusted size (%d)") % parm.water.a_lo_min_lake;
            return;
        }  

        if (
            (parm.water.a_hi_min_lake != 3 ) &&
            (parm.water.a_hi_min_lake != 5 ) &&
            (parm.water.a_hi_min_lake != 7 ) &&
            (parm.water.a_hi_min_lake != 9 ) &&
            (parm.water.a_hi_min_lake != 11) &&
            (parm.water.a_hi_min_lake != 13) &&
            (parm.water.a_hi_min_lake != 15) &&
            (parm.water.a_hi_min_lake != 17) &&
            (parm.water.a_hi_min_lake != 19) &&
            (parm.water.a_hi_min_lake != 21) &&
            (parm.water.a_hi_min_lake != 23) &&
            (parm.water.a_hi_min_lake != 25)
           )
        {
            M_out_emsg(L"Water detect bypassed -- unexpected value for adaptive water detect highest adjusted size (%d)") % parm.water.a_lo_min_lake;
            return;
        }       
    }
    

    // slice input slice in thread parm area into subslices 

    slice_row_col(thpa, r0, rm, c0, cm, MAX_SLICES, false, parm);


    //  loop to process each subslice
    //  -----------------------------

    int32_t new_ct  = 0; 
    int32_t skip_ct = 0; 

    int32_t ct_ml3,  ct_ml5,  ct_ml7,  ct_ml9,  ct_ml11,  ct_ml13,  ct_ml15,  ct_ml17,  ct_ml19,  ct_ml21,  ct_ml23,  ct_ml25;
            ct_ml3 = ct_ml5 = ct_ml7 = ct_ml9 = ct_ml11 = ct_ml13 = ct_ml15 = ct_ml17 = ct_ml19 = ct_ml21 = ct_ml23 = ct_ml25 = 0; 

    int32_t ct_s1,  ct_s2,  ct_s3,  ct_s4,  ct_s5,  ct_s6,  ct_s7,  ct_s8,  ct_s9,  ct_shi; 
            ct_s1 = ct_s2 = ct_s3 = ct_s4 = ct_s5 = ct_s6 = ct_s7 = ct_s8 = ct_s9 = ct_shi = 0; 

    for (auto i = 0; i < thpa.n_threads; i++)
    {
        // set up default non-adaptive parms, in case adaptive water detect is turned off

        int     rc       = 0; 
        int32_t min_lake = parm.water.min_lake;
        int32_t stride   = parm.water.stride;
        int32_t check    = parm.water.check_lake;     


        // Do adaptive water check preparations, if required (called function sets min_lake and stride as output parms)

        if (parm.water.adaptive)
            rc = adaptive_water_setup(mapdata, thpa.thp[i], stride, min_lake, check, parm);         


        // rc = 0 -- good data in this subslice -- need to do water detection for this subslice

        if (rc == 0) 
        {
           // gather stride stats
           // -------------------

            switch (stride)
            {
                case 1  : ++ct_s1;  break; 
                case 2  : ++ct_s2;  break; 
                case 3  : ++ct_s3;  break; 
                case 4  : ++ct_s4;  break; 
                case 5  : ++ct_s5;  break; 
                case 6  : ++ct_s6;  break;
                case 7  : ++ct_s7;  break;
                case 8  : ++ct_s8;  break;
                case 9  : ++ct_s9;  break;
                default : ++ct_shi; break; 
            }


            //  Call proper water detect routine based on min lake size 
            //  -------------------------------------------------------

            thpa.thp[i].r32 = 0; 

             switch (min_lake)
             {
                 case  3 : water_map3( mapdata, thpa.thp[i], stride, check, parm); ++ct_ml3;  break; 
                 case  5 : water_map5( mapdata, thpa.thp[i], stride, check, parm); ++ct_ml5;  break; 
                 case  7 : water_map7( mapdata, thpa.thp[i], stride, check, parm); ++ct_ml7;  break; 
                 case  9 : water_map9( mapdata, thpa.thp[i], stride, check, parm); ++ct_ml9;  break; 
                 case 11 : water_map11(mapdata, thpa.thp[i], stride, check, parm); ++ct_ml11; break; 
                 case 13 : water_map13(mapdata, thpa.thp[i], stride, check, parm); ++ct_ml13; break; 
                 case 15 : water_map15(mapdata, thpa.thp[i], stride, check, parm); ++ct_ml15; break; 
                 case 17 : water_map17(mapdata, thpa.thp[i], stride, check, parm); ++ct_ml17; break;
                 case 19 : water_map19(mapdata, thpa.thp[i], stride, check, parm); ++ct_ml19; break;
                 case 21 : water_map21(mapdata, thpa.thp[i], stride, check, parm); ++ct_ml21; break; 
                 case 23 : water_map23(mapdata, thpa.thp[i], stride, check, parm); ++ct_ml23; break;
                 case 25 : water_map25(mapdata, thpa.thp[i], stride, check, parm); ++ct_ml25; break;
                        
                 default :  // should not happen ????
                     M_out_emsg(L"Water detect bypassed -- unexpected (adjusted) value for water detect size (%d)") % min_lake; 
                     return;
                     break;
             }

             new_ct += thpa.thp[i].r32;
        }
        else   // Rc not 0 -- no valid data in subslice -- skip call to water_mapx()
        {
            ++skip_ct;  
            //  M_msg("No valid data this subslice")
        }
    }

    if (parm.thread.display)
    {
        M_cout_lock();
        M_out( L"Thread %3d: water detect ended --  skipped/total sections=%-4d/%4d  detections=%-8d rows=%5d/%5d  cols=%5d/%5d") % thp_in.thread_n % skip_ct % thpa.n_threads % new_ct % r0 % rm % c0 % cm;   
        M_out( L"Thread %3d: stride counts  : 1=%-4d 2=%-4d 3=%-4d 4=%-4d 5=%-4d 6=%-4d 7=%-4d 8=%-4d 9=%-4d 10+=%-4d\n"
               L"            min_lake counts: 3=%-4d 5=%-4d 7=%-4d 9=%-4d 11=%-4d 13=%-4d 15=%-4d 17=%-4d 19=%-4d 21=%-4d 23=%-4d 25=%-4d")    
             % thp_in.thread_n % ct_s1 % ct_s2  % ct_s3  % ct_s4  % ct_s5   % ct_s6   % ct_s7   % ct_s8   % ct_s9   % ct_shi
             %                  ct_ml3 % ct_ml5 % ct_ml7 % ct_ml9 % ct_ml11 % ct_ml13 % ct_ml15 % ct_ml17 % ct_ml19 % ct_ml21 % ct_ml23 % ct_ml25
             ;
    }

   M__(M_out("water_detect() returning");)
   return; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map3() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper MACROs for this routine



//// declaration MACROs

#define M_DH3(x)   int16_t x##1, x##2, x##3; 

//         up through M_DDN(N-1 / 2)
#define M_DD3()    M_DDN(1)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR3()   int r1, row, r3; 
#define M_DC3()   int c1, col, c3;


///// code-generating MACROs

#define M_CH3(v)   if (h22 != v) goto mismatch3; 
#define M_W3(r,c)  M_mapdata_attr(mapdata, r, c) |= MAPDATA_WATER;


#define M_NL3(x) if( ( col > c0 + x) && (row > r0 + x) && ( col < cm - x) && (row < rm - x) )   // don't place new lake too close to any edge



////////////////////////////////////////////////////



static void water_map3(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD3()
    M_DR3()
    M_DC3()
    M_DH3(h1);
    M_DH3(h2);
    M_DH3(h3);

    M_DV(3)

    //////////////////////////////////

    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row (with stride) 
    {
        r1 = M_max(r0, row - d1);
        r3 = M_min(rm, row + d1); 

        // set hx1 hx2    for 1st column

        h12 = M_mapdata_ht( mapdata, r1 ,  c0 ); h11 = -11111;	   
        h22 = M_mapdata_ht( mapdata, row,  c0 ); h21 = -11111;	  
        h32 = M_mapdata_ht( mapdata, r3  , c0 ); h31 = -11111;   

        was_water = 0; 

        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride) 
        {
            c3 = M_min(cm, col + d1); 

            // h11,h12 etc set at start of this line (before loop)

            h13 = M_mapdata_ht( mapdata, r1,  c3 ); 
            h23 = M_mapdata_ht( mapdata, row, c3 );
            h33 = M_mapdata_ht( mapdata, r3 , c3 );

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h22 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_CH3(h11)   M_CH3(h13)   M_CH3(h13)      // MACROs goto mismatch5 label
                M_CH3(h21)                M_CH3(h23) 
                M_CH3(h31)   M_CH3(h32)   M_CH3(h33) 

                if M_CHT(h22)     // exclude low-lying coastal areas and higher elevations, etc.
                {

                    if (was_water == 1)
                    {
                        M_W3(r1 , c3)     // just set leading col of pixels to water
                        M_W3(row, c3)
                        M_W3(r3 , c3)
                    }
                    else
                    {    
                      M_NL3(0)                             // don't start new lake too close to edge 
                      {
                        c1 = M_max(c0, col - d1);           //  compute clipped column numbers
                        //  c3 already set earlier

               //         M_W3(r1 , c1)   M_W3(r1 , col)   M_W3(r1 , c3)  					 
               //         M_W3(row, c1)   M_W3(row, col)   M_W3(row, c3)  
               //         M_W3(r3 , c1)   M_W3(r3 , col)   M_W3(r3 , c3) 
                        M_W3(row, col)     // just set center point to water -- fill will take care of rest of lake

                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                      }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }
                goto match3;
mismatch3:      was_water = 0;  // make sure any current lake is ended
match3:         ;
            }       // valid data

            // slide down data points for next inner loop pass

            h11 = h12; h12 = h13;  
            h21 = h22; h22 = h23; 
            h31 = h32; h32 = h33; 

        }           // column loop
    }               // row loop
            
    M_finish() 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map5() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper MACROs for this routine



//// declaration MACROs

#define M_DH5(x)   int16_t x##1, x##2, x##3, x##4, x##5; 

//         up through M_DDN(N-1 / 2)
#define M_DD5()    M_DDN(1) M_DDN(2)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR5()   int r1, r2, row, r4, r5; 
#define M_DC5()   int c1, c2, col, c4, c5;


///// code-generating MACROs

#define M_CH5(v)   if (h33 != v) goto mismatch5;
#define M_C5(x)    M_CH5(x##1) M_CH5(x##2) M_CH5(x##3) M_CH5(x##4) M_CH5(x##5)
#define M_W5(r,c)  M_mapdata_attr(mapdata, r, c) |= MAPDATA_WATER;
#define M_WM5(x)   M_W5(x,c1) M_W5(x,c2) M_W5(x,col) M_W5(x,c4) M_W5(x,c5) 


#define M_SL5(x)   x##1 = x##2; x##2 = x##3; x##3 = x##4; x##4 = x##5;

#define M_SI5(x,r)                           \
        x##1 = -11111;                       \
        x##2 = -11111;                       \
        x##3 = M_mapdata_ht(mapdata, r,  c0    ); \
        x##4 = M_mapdata_ht(mapdata, r,  c0 + d1);\
        x##5 = M_mapdata_ht(mapdata, r,  c0 + d2);   
       
#define M_NL5(x) if( ( col > c0 + x) && (row > r0 + x) && ( col < cm - x) && (row < rm - x) )   // don't place new lake too close to any edge



////////////////////////////////////////////////////


static void water_map5(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD5()
    M_DR5()
    M_DC5()
    M_DH5(h1);
    M_DH5(h2);
    M_DH5(h3);
    M_DH5(h4);
    M_DH5(h5);
    
    M_DV(5)

    //////////////////////////////////

    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row (with stride)  
    {
        r1 = M_max(r0, row - d2);  
        r2 = M_max(r0, row - d1);
        r4 = M_min(rm, row + d1); 
        r5 = M_min(rm, row + d2); 

        // set hx1 hx2 hx3 hx4  for 1st column

        M_SI5(h1, r1)
        M_SI5(h2, r2)
        M_SI5(h3, row)
        M_SI5(h4, r4)
        M_SI5(h5, r5)

        was_water = 0; 

        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride) 
        {
            c5 = M_min(cm, col + d2); 

            // h11,h12 etc set at start of this line (before loop)

            h15 = M_mapdata_ht(mapdata, r1,  c5 ); 
            h25 = M_mapdata_ht(mapdata, r2,  c5 );
            h35 = M_mapdata_ht(mapdata, row, c5 );
            h45 = M_mapdata_ht(mapdata, r4 , c5 );
            h55 = M_mapdata_ht(mapdata, r5 , c5 );

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h33 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C5(h3)     // do least clipped compare first
                M_C5(h1)
                M_C5(h2)     // MACROs goto mismtch label below

                M_C5(h4)
                M_C5(h5)
              
                if M_CHT(h33)     // exclude low-lying coastal areas and higher elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W5(r1 , c5)     // just set leading col of pixels to water
                        M_W5(r2 , c5)
                        M_W5(row, c5)
                        M_W5(r4,  c5)
                        M_W5(r5 , c5)
                    }
                    else
                    {    
                       M_NL5(0)                            // don't start new lake too close to edge
                       {
                        c1 = M_max(c0, col - d2);           //  compute clipped column numbers
                        c2 = M_max(c0, col - d1);
                        c4 = M_min(cm, col + d1);           //  c5 already set earlier

                    //    M_WM5(r1)
                    //    M_WM5(r2)
                    //    M_WM5(row)
                    //    M_WM5(r4)
                    //    M_WM5(r5)
                        M_W5(row, col)     // just set center point to water -- fill will take care of rest of lake

                        new_ct++;
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }
                goto match5;
mismatch5:      was_water = 0;  // make sure any current lake is ended
match5:         ;            
            }       // valid data

            // slide down data points for next inner loop pass
            
            M_SL5(h1)
            M_SL5(h2)
            M_SL5(h3)
            M_SL5(h4)
            M_SL5(h5)
            
        }           // column loop
    }              // row loop

    M_finish()  
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map7() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper MACROs for this routine



//// declaration MACROs

#define M_DH7(x)   int16_t x##1, x##2, x##3, x##4, x##5, x##6, x##7; 

//         up through M_DDN(N-1 / 2)
#define M_DD7()    M_DDN(1) M_DDN(2) M_DDN(3)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR7()   int r1, r2, r3, row, r5, r6, r7; 
#define M_DC7()   int c1, c2, c3, col, c5, c6, c7;


///// code-generating MACROs

#define M_CH7(v)   if (h44 != v) goto mismatch7;
#define M_C7(x)    M_CH7(x##1) M_CH7(x##2) M_CH7(x##3) M_CH7(x##4) M_CH7(x##5) M_CH7(x##6) M_CH7(x##7) 
#define M_W7(r,c)  M_mapdata_attr(mapdata, r, c) |= MAPDATA_WATER;
#define M_WM7(x)   M_W7(x,c1) M_W7(x,c2) M_W7(x,c3) M_W7(x,col) M_W7(x,c5) M_W7(x,c6) M_W7(x,c7) 

#define M_SL7(x)   x##1 = x##2; x##2 = x##3; x##3 = x##4; x##4 = x##5; x##5 = x##6; x##6 = x##7;

#define M_SI7(x,r)                           \
        x##1 = -11111;                       \
        x##2 = -11111;                       \
        x##3 = -11111;                       \
        x##4 = M_mapdata_ht(mapdata, r,  c0    ); \
        x##5 = M_mapdata_ht(mapdata, r,  c0 + d1);\
        x##6 = M_mapdata_ht(mapdata, r,  c0 + d2);\
        x##7 = M_mapdata_ht(mapdata, r,  c0 + d3);

#define M_NL7(x) if( ( col > c0 + x) && (row > r0 + x) && ( col < cm - x) && (row < rm - x) )   // don't place new lake too close to any edge



////////////////////////////////////////////////////


static void water_map7(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD7()
    M_DR7()
    M_DC7()
    M_DH7(h1);
    M_DH7(h2);
    M_DH7(h3);
    M_DH7(h4);
    M_DH7(h5);
    M_DH7(h6);
    M_DH7(h7);

    M_DV(7)

    //////////////////////////////////
    
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row (with stride)  
    {
        r1 = M_max(r0, row - d3);  
        r2 = M_max(r0, row - d2);
        r3 = M_max(r0, row - d1);
        r5 = M_min(rm, row + d1); 
        r6 = M_min(rm, row + d2); 
        r7 = M_min(rm, row + d3);

        // set hx1 hx2 hx3 hx4 hx5 hx6 (but not hx7) for column 0
        
        M_SI7(h1, r1)
        M_SI7(h2, r2)
        M_SI7(h3, r3)
        M_SI7(h4, row)
        M_SI7(h5, r5)
        M_SI7(h6, r6)
        M_SI7(h7, r7)
                
        was_water = 0; 

        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride) 
        {
            c7 = M_min(cm, col +d3); 

            // h11,h12 etc set at start of this line (before loop)

            h17 = M_mapdata_ht(mapdata, r1,  c7 ); 
            h27 = M_mapdata_ht(mapdata, r2,  c7 );
            h37 = M_mapdata_ht(mapdata, r3 , c7 );
            h47 = M_mapdata_ht(mapdata, row, c7 );
            h57 = M_mapdata_ht(mapdata, r5 , c7 );
            h67 = M_mapdata_ht(mapdata, r6 , c7 );
            h77 = M_mapdata_ht(mapdata, r7 , c7 );

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h44 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C7(h4)     // do minimally-clipped row first 

                M_C7(h1)     // MACROs goto mismatch label below
                M_C7(h2)
                M_C7(h3)
                
                M_C7(h5)
                M_C7(h6)
                M_C7(h7)
                                            

                if M_CHT(h44)     // exclude low-lying coastal areas and higher elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W7(r1 , c7)     // just set leading col of pixels to water
                        M_W7(r2 , c7)
                        M_W7(r3 , c7)
                        M_W7(row, c7)
                        M_W7(r5,  c7)
                        M_W7(r6 , c7)
                        M_W7(r7 , c7)
                    }
                    else
                    {  
                       M_NL7(0)                            // don't start new lake too close to edge
                       {
                        c1 = M_max(c0, col - d3);           //  compute clipped column numbers
                        c2 = M_max(c0, col - d2);
                        c3 = M_max(c0, col - d1);
                        c5 = M_min(cm, col + d1);           //  c7 already set earlier
                        c6 = M_min(cm, col + d2);

                    //    M_WM7(r1)
                    //    M_WM7(r2)
                    //    M_WM7(r3)
                    //    M_WM7(row)
                    //    M_WM7(r5)
                    //    M_WM7(r6)
                    //    M_WM7(r7)
                        M_W7(row, col)     // just set center point to water -- fill will take care of rest of lake
                           
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match7;
mismatch7: was_water = 0;    // make sure to end lake due to mismatch ht
match7:; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL7(h1)
            M_SL7(h2)
            M_SL7(h3)
            M_SL7(h4)
            M_SL7(h5)
            M_SL7(h6)
            M_SL7(h7)
        }          // column loop
    }              // row loop
  
    M_finish() 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map9() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper MACROs for this routine


//// declaration MACROs

#define M_DH9(x)   int16_t x##1, x##2, x##3, x##4, x##5, x##6, x##7, x##8, x##9; 

//         up through M_DDN(N-1 / 2)
#define M_DD9()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR9()   int r1, r2, r3, r4, row, r6, r7, r8, r9; 
#define M_DC9()   int c1, c2, c3, c4, col, c6, c7, c8, c9;


///// code-generating MACROs

#define M_CH9(v)   if (h55 != v) goto mismatch9; 
#define M_C9(x)    M_CH9(x##1) M_CH9(x##2) M_CH9(x##3) M_CH9(x##4) M_CH9(x##5) M_CH9(x##6) M_CH9(x##7) M_CH9(x##8) M_CH9(x##9) 

#define M_W9(r,c)  M_mapdata_attr(mapdata, r, c) |= MAPDATA_WATER;

#define M_SL9(x)   x##1 = x##2; x##2 = x##3; x##3 = x##4; x##4 = x##5; x##5 = x##6; x##6 = x##7; x##7 = x##8; x##8 = x##9;

#define M_WM9(x)   M_W9(x,c1) M_W9(x,c2) M_W9(x,c3) M_W9(x,c4) M_W9(x,col) M_W9(x,c6) M_W9(x,c7) M_W9(x,c8) M_W9(x,c9) 

#define M_SI9(x,r)                           \
        x##1 = -11111;                       \
        x##2 = -11111;                       \
        x##3 = -11111;                       \
        x##4 = -11111;                       \
        x##5 = M_mapdata_ht(mapdata, r,  c0    ); \
        x##6 = M_mapdata_ht(mapdata, r,  c0 + d1);\
        x##7 = M_mapdata_ht(mapdata, r,  c0 + d2);\
        x##8 = M_mapdata_ht(mapdata, r,  c0 + d3);

#define M_NL9(x) if( ( col > c0 + x) && (row > r0 + x) && ( col < cm - x) && (row < rm - x) )   // don't place new lake too close to any edge


////////////////////////////////////////////////////



static void water_map9(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD9()
    M_DR9()
    M_DC9()
    M_DH9(h1);      // declare nxn group hxx
    M_DH9(h2);
    M_DH9(h3);
    M_DH9(h4);
    M_DH9(h5);
    M_DH9(h6);
    M_DH9(h7);
    M_DH9(h8);
    M_DH9(h9);
   
    M_DV(9)

    //////////////////////////////////      
    
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row  (with stride) 
    {
        r1 = M_max(r0, row - d4);  
        r2 = M_max(r0, row - d3);
        r3 = M_max(r0, row - d2);
        r4 = M_max(r0, row - d1);
        r6 = M_min(rm, row + d1); 
        r7 = M_min(rm, row + d2); 
        r8 = M_min(rm, row + d3);
        r9 = M_min(rm, row + d4);

        // set hx1 hx2 hx3 hx4 hx5 hx6 hx7 hx8 (but not hx9) for column 0

        M_SI9(h1, r1)
        M_SI9(h2, r2)
        M_SI9(h3, r3)
        M_SI9(h4, r4)
        M_SI9(h5, row)
        M_SI9(h6, r6)
        M_SI9(h7, r7)
        M_SI9(h8, r8)
        M_SI9(h9, r9)
        
        was_water = 0; 



        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride) 
        {
            c9 = M_min(cm, col + d4); 

            // h11,h12 etc set at start of this line (before loop)

            h19 = M_mapdata_ht(mapdata, r1,  c9 ); 
            h29 = M_mapdata_ht(mapdata, r2,  c9 );
            h39 = M_mapdata_ht(mapdata, r3 , c9 );
            h49 = M_mapdata_ht(mapdata, r4 , c9 );
            h59 = M_mapdata_ht(mapdata, row, c9 );
            h69 = M_mapdata_ht(mapdata, r6 , c9 );
            h79 = M_mapdata_ht(mapdata, r7 , c9 );
            h89 = M_mapdata_ht(mapdata, r8 , c9 );
            h99 = M_mapdata_ht(mapdata, r9 , c9 );

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h55 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C9(h5)  // do minimally-clipped row first

                M_C9(h1) // MACROs go to mismatch label
                M_C9(h2)
                M_C9(h3)
                M_C9(h4)
                
                M_C9(h6)
                M_C9(h7)
                M_C9(h8)
                M_C9(h9)
              
                if M_CHT(h55)     // exclude low-lying coastal areas and higher elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W9(r1 , c9)     // just set leading col of pixels to water
                        M_W9(r2 , c9)
                        M_W9(r3 , c9)
                        M_W9(r4 , c9)
                        M_W9(row, c9)
                        M_W9(r6,  c9)
                        M_W9(r7 , c9)
                        M_W9(r8 , c9)
                        M_W9(r9 , c9)
                     }
                     else
                     {  
                       M_NL9(0)                            // don't start new lake too close to edge
                       {
                        c1 = M_max(c0, col - d4);           //  compute clipped column numbers
                        c2 = M_max(c0, col - d3);
                        c3 = M_max(c0, col - d2);
                        c4 = M_max(c0, col - d1);
                        c6 = M_min(cm, col + d1);           //  c9 already set earlier
                        c7 = M_min(cm, col + d2);
                        c8 = M_min(cm, col + d3);

                     //   M_WM9(r1)
                     //   M_WM9(r2)
                     //   M_WM9(r3)
                     //   M_WM9(r4)
                     //   M_WM9(row)
                     //   M_WM9(r6)
                     //   M_WM9(r7)
                     //   M_WM9(r8)
                     //   M_WM9(r9)
                        M_W9(row, col)     // just set center point to water -- fill will take care of rest of lake
                        
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match9;
mismatch9:      was_water = 0;    // make sure to end lake due to mismatch ht
match9:         ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL9(h1)
            M_SL9(h2)
            M_SL9(h3)
            M_SL9(h4)
            M_SL9(h5)
            M_SL9(h6)
            M_SL9(h7)
            M_SL9(h8)
            M_SL9(h9)
        
        }          // column loop
    }              // row loop

    M_finish() 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map11() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper MACROs for this routine

//// declaration MACROs

#define M_DH11(x)   int16_t x##01, x##02, x##03, x##04, x##05, x##06, x##07, x##08, x##09, \
                     x##10, x##11; 


//         up through M_DDN(N-1 / 2)
#define M_DD11()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4) M_DDN(5)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR11()   int r1, r2, r3, r4, r5, row, r7, r8, r9, r10, r11; 
#define M_DC11()   int c1, c2, c3, c4, c5, col, c7, c8, c9, c10, c11;


///// code-generating MACROs

#define M_CH11(v)   if (h0606 != v) goto mismatch11; 
#define M_C11(x)    M_CH11(x##01) M_CH11(x##02) M_CH11(x##03) M_CH11(x##04) M_CH11(x##05) M_CH11(x##06) M_CH11(x##07) M_CH11(x##08) M_CH11(x##09) M_CH11(x##10) M_CH11(x##11) 

#define M_W11(r,c)  M_mapdata_attr(mapdata, r, c) |= MAPDATA_WATER;

#define M_SL11(x)   x##01 = x##02; x##02 = x##03; x##03 = x##04; x##04 = x##05; x##05 = x##06; x##06 = x##07; x##07 = x##08; x##08 = x##09; x##09 = x##10; x##10 = x##11;

#define M_WM11(x)   M_W11(x,c1) M_W11(x,c2) M_W11(x,c3) M_W11(x,c4) M_W11(x,c5) M_W11(x,col) M_W11(x,c7) M_W11(x,c8) M_W11(x,c9) M_W11(x,c10) M_W11(x,c11) 

#define M_SI11(x,r)                                 \
        x##01 = -11111;                             \
        x##02 = -11111;                             \
        x##03 = -11111;                             \
        x##04 = -11111;                             \
        x##05 = -11111;                             \
        x##06 = M_mapdata_ht(mapdata, r,  c0    );  \
        x##07 = M_mapdata_ht(mapdata, r,  c0 + d1); \
        x##08 = M_mapdata_ht(mapdata, r,  c0 + d2); \
        x##09 = M_mapdata_ht(mapdata, r,  c0 + d3); \
        x##10 = M_mapdata_ht(mapdata, r,  c0 + d4);

#define M_NL11(x) if( ( col > c0 + x) && (row > r0 + x) && ( col < cm - x) && (row < rm - x) )   // don't place new lake too close to any edge        



////////////////////////////////////////////////////


static void water_map11(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD11()
    M_DR11()
    M_DC11() 
    M_DH11(h01);      // declare n x n group hxxyy
    M_DH11(h02);
    M_DH11(h03);
    M_DH11(h04);
    M_DH11(h05);
    M_DH11(h06);
    M_DH11(h07);
    M_DH11(h08);
    M_DH11(h09);
    M_DH11(h10);
    M_DH11(h11);
    
    M_DV(11)

    //////////////////////////////////
        
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row  (with stride) 
    {
        r1  = M_max(r0, row - d5);  
        r2  = M_max(r0, row - d4);
        r3  = M_max(r0, row - d3);
        r4  = M_max(r0, row - d2);
        r5  = M_max(r0, row - d1); 
        //////
        r7  = M_min(rm, row + d1); 
        r8  = M_min(rm, row + d2);
        r9  = M_min(rm, row + d3);
        r10 = M_min(rm, row + d4);
        r11 = M_min(rm, row + d5);

        // set hx01 hx02 hx03 hx04 hx05 hx06 hx07 hx08 hx09 hx10 (but not hx11) for column 0
        
        M_SI11(h01, r1 )        
        M_SI11(h02, r2 )        
        M_SI11(h03, r3 )        
        M_SI11(h04, r4 )        
        M_SI11(h05, r5 )         
        M_SI11(h06, row)       
        M_SI11(h07, r7 )        
        M_SI11(h08, r8 )         
        M_SI11(h09, r9 )          
        M_SI11(h10, r10)         
        M_SI11(h11, r11)
        
        was_water = 0; 



        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride) 
        {
            c11 = M_min(cm, col + d5); 

            // h11,h12 etc set at start of this line (before loop)

            h0111 = M_mapdata_ht(mapdata, r1,  c11); 
            h0211 = M_mapdata_ht(mapdata, r2,  c11);
            h0311 = M_mapdata_ht(mapdata, r3 , c11);
            h0411 = M_mapdata_ht(mapdata, r4 , c11);
            h0511 = M_mapdata_ht(mapdata, r5 , c11);
            h0611 = M_mapdata_ht(mapdata, row, c11);
            h0711 = M_mapdata_ht(mapdata, r7 , c11);
            h0811 = M_mapdata_ht(mapdata, r8 , c11);
            h0911 = M_mapdata_ht(mapdata, r9 , c11);
            h1011 = M_mapdata_ht(mapdata, r10, c11);
            h1111 = M_mapdata_ht(mapdata, r11, c11);


            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h0606 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C11(h06) // do minimally-clipped row, first 

                M_C11(h01) // compare MACROs go to mismatch label -- check all n x n points (some may be clipped)
                M_C11(h02)
                M_C11(h03)
                M_C11(h04)
                M_C11(h05)

                M_C11(h07)
                M_C11(h08)
                M_C11(h09)
                M_C11(h10)
                M_C11(h11)
              
                if M_CHT(h0606)     // exclude low-lying coastal areas and higher elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W11(r1 , c11)     // just set leading col of pixels to water
                        M_W11(r2 , c11)
                        M_W11(r3 , c11)
                        M_W11(r4 , c11)
                        M_W11(r5 , c11)
                        M_W11(row, c11)
                        M_W11(r7 , c11)
                        M_W11(r8 , c11)
                        M_W11(r9 , c11)
                        M_W11(r10, c11)
                        M_W11(r11, c11)
                     }
                     else
                     {   
                       M_NL11(0)                            // don't start new lake too close to edge
                       {
                        c1  = M_max(c0, col - d5);           //  compute clipped column numbers
                        c2  = M_max(c0, col - d4);
                        c3  = M_max(c0, col - d3);
                        c4  = M_max(c0, col - d2);
                        c5  = M_max(c0, col - d1);

                        c7  = M_min(cm, col + d1);           
                        c8  = M_min(cm, col + d2);
                        c9  = M_min(cm, col + d3);
                        c10 = M_min(cm, col + d4);
                     // c11 = already set earlier

                   //     M_WM11(r1 )                         // set all n x n points to water
                   //     M_WM11(r2 )
                   //     M_WM11(r3 )
                   //     M_WM11(r4 )
                   //     M_WM11(r5 )
                   //     M_WM11(row)
                   //     M_WM11(r7 )
                   //     M_WM11(r8 )
                   //     M_WM11(r9 )
                   //     M_WM11(r10)
                   //     M_WM11(r11)
                        M_W11(row, col)     // just set center point to water -- fill will take care of rest of lake
                        
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match11;
mismatch11:     was_water = 0;    // make sure to end lake due to mismatch ht
match11:         ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL11(h01)
            M_SL11(h02)
            M_SL11(h03)
            M_SL11(h04)
            M_SL11(h05)
            M_SL11(h06)
            M_SL11(h07)
            M_SL11(h08)
            M_SL11(h09)
            M_SL11(h10)
            M_SL11(h11)
        
        }          // column loop
    }              // row loop

    M_finish() 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map13() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper MACROs for this routine


//// declaration MACROs

#define M_DH13(x)   int16_t x##01, x##02, x##03, x##04, x##05, x##06, x##07, x##08, x##09, \
                     x##10, x##11, x##12, x##13; 

//         up through M_DDN(N-1 / 2)
#define M_DD13()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4) M_DDN(5) M_DDN(6)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR13()   int r1, r2, r3, r4, r5, r6, row, r8, r9, r10, r11, r12, r13; 
#define M_DC13()   int c1, c2, c3, c4, c5, c6, col, c8, c9, c10, c11, c12, c13;


///// code-generating MACROs

#define M_CH13(v)   if (h0707 != v) goto mismatch13; 
#define M_C13(x)    M_CH13(x##01) M_CH13(x##02) M_CH13(x##03) M_CH13(x##04) M_CH13(x##05) M_CH13(x##06) M_CH13(x##07) M_CH13(x##08) M_CH13(x##09) M_CH13(x##10) M_CH13(x##11) M_CH13(x##12) M_CH13(x##13) 

#define M_W13(r,c)  M_mapdata_attr(mapdata, r, c) |= MAPDATA_WATER;

#define M_SL13(x)   x##01 = x##02; x##02 = x##03; x##03 = x##04; x##04 = x##05; x##05 = x##06; x##06 = x##07; x##07 = x##08; x##08 = x##09; x##09 = x##10; x##10 = x##11; x##11 = x##12; x##12 = x##13;

#define M_WM13(x)   M_W13(x,c1) M_W13(x,c2) M_W13(x,c3) M_W13(x,c4) M_W13(x,c5) M_W13(x,c6) M_W13(x,col) M_W13(x,c8) M_W13(x,c9) M_W13(x,c10) M_W13(x,c11) M_W13(x,c12) M_W13(x,c13) 

#define M_SI13(x,r)                                 \
        x##01 = -11111;                             \
        x##02 = -11111;                             \
        x##03 = -11111;                             \
        x##04 = -11111;                             \
        x##05 = -11111;                             \
        x##06 = -11111;                             \
        x##07 = M_mapdata_ht(mapdata, r,  c0    );  \
        x##08 = M_mapdata_ht(mapdata, r,  c0 + d1); \
        x##09 = M_mapdata_ht(mapdata, r,  c0 + d2); \
        x##10 = M_mapdata_ht(mapdata, r,  c0 + d3); \
        x##11 = M_mapdata_ht(mapdata, r,  c0 + d4); \
        x##12 = M_mapdata_ht(mapdata, r,  c0 + d5);
        
#define M_NL13(x) if( ( col > c0 + x) && (row > r0 + x) && ( col < cm - x) && (row < rm - x) )   // don't place new lake too close to any edge



////////////////////////////////////////////////////



static void water_map13(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD13()
    M_DR13()
    M_DC13()
    M_DH13(h01);      // declare n x n group hxxyy
    M_DH13(h02);
    M_DH13(h03);
    M_DH13(h04);
    M_DH13(h05);
    M_DH13(h06);
    M_DH13(h07);
    M_DH13(h08);
    M_DH13(h09);
    M_DH13(h10);
    M_DH13(h11);
    M_DH13(h12);
    M_DH13(h13);

    M_DV(13)
 
    //////////////////////////////////
    
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row (with stride) 
    {
        r1  = M_max(r0, row - d6);
        r2  = M_max(r0, row - d5);  
        r3  = M_max(r0, row - d4);
        r4  = M_max(r0, row - d3);
        r5  = M_max(r0, row - d2);
        r6  = M_max(r0, row - d1); 
        //////
        r8  = M_min(rm, row + d1); 
        r9  = M_min(rm, row + d2);
        r10 = M_min(rm, row + d3);
        r11 = M_min(rm, row + d4);
        r12 = M_min(rm, row + d5);
        r13 = M_min(rm, row + d6);

        // set hx01 hx02 hx03 hx04 hx05 hx06 hx07 hx08 hx09 hx10 hx11 hx12 (but not hx13) for column 0
        
        M_SI13(h01, r1 )        
        M_SI13(h02, r2 )        
        M_SI13(h03, r3 )        
        M_SI13(h04, r4 )        
        M_SI13(h05, r5 ) 
        M_SI13(h06, r6 ) 
        M_SI13(h07, row)       
        M_SI13(h08, r8 )         
        M_SI13(h09, r9 )          
        M_SI13(h10, r10)         
        M_SI13(h11, r11)
        M_SI13(h12, r12)
        M_SI13(h13, r13)
        
        
        was_water = 0; 



        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride) 
        {
            c13 = M_min(cm, col + d6); 

            // h11,h12 etc set at start of this line (before loop)

            h0113 = M_mapdata_ht(mapdata, r1,  c13); 
            h0213 = M_mapdata_ht(mapdata, r2,  c13);
            h0313 = M_mapdata_ht(mapdata, r3 , c13);
            h0413 = M_mapdata_ht(mapdata, r4 , c13);
            h0513 = M_mapdata_ht(mapdata, r5 , c13);
            h0613 = M_mapdata_ht(mapdata, r6 , c13);
            h0713 = M_mapdata_ht(mapdata, row, c13);
            h0813 = M_mapdata_ht(mapdata, r8 , c13);
            h0913 = M_mapdata_ht(mapdata, r9 , c13);
            h1013 = M_mapdata_ht(mapdata, r10, c13);
            h1113 = M_mapdata_ht(mapdata, r11, c13);
            h1213 = M_mapdata_ht(mapdata, r12, c13);
            h1313 = M_mapdata_ht(mapdata, r13, c13);


            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h0707 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C13(h07) // do minimally-clipped row, first 

                M_C13(h01) // compare MACROs go to mismatch label -- check all n x n points (some may be clipped)
                M_C13(h02)
                M_C13(h03)
                M_C13(h04)
                M_C13(h05)
                M_C13(h06)

                M_C13(h08)
                M_C13(h09)
                M_C13(h10)
                M_C13(h11)
                M_C13(h12)
                M_C13(h13)            

              
                if M_CHT(h0707)     // exclude low-lying coastal areas and higher elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W13(r1 , c13)     // just set leading col of pixels to water
                        M_W13(r2 , c13)
                        M_W13(r3 , c13)
                        M_W13(r4 , c13)
                        M_W13(r5 , c13)
                        M_W13(r6 , c13)
                        M_W13(row, c13)
                        M_W13(r8 , c13)
                        M_W13(r9 , c13)
                        M_W13(r10, c13)
                        M_W13(r11, c13)
                        M_W13(r12, c13)
                        M_W13(r13, c13)
                     }
                     else
                     {   
                       M_NL13(0)                            // don't start new lake too close to edge
                       {
                        c1  = M_max(c0, col - d6);           //  compute clipped column numbers
                        c2  = M_max(c0, col - d5);
                        c3  = M_max(c0, col - d4);
                        c4  = M_max(c0, col - d3);
                        c5  = M_max(c0, col - d2);
                        c6  = M_max(c0, col - d1);

                        c8  = M_min(cm, col + d1);           
                        c9  = M_min(cm, col + d2);
                        c10 = M_min(cm, col + d3);
                        c11 = M_min(cm, col + d4);
                        c12 = M_min(cm, col + d5);
                     // c13 = already set earlier

               //         M_WM13(r1 )                         // set all n x n points to water
               //         M_WM13(r2 )
                //        M_WM13(r3 )
                //        M_WM13(r4 )
                //        M_WM13(r5 )
                //        M_WM13(r6 )
                //        M_WM13(row)
                //        M_WM13(r8 )
                //        M_WM13(r9 )
                //        M_WM13(r10)
                //        M_WM13(r11)
                //        M_WM13(r12)
                //        M_WM13(r13)
                        M_W13(row, col)     // just set center point to water -- fill will take care of rest of lake
                        
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match13;
mismatch13:     was_water = 0;    // make sure to end lake due to mismatch ht
match13:         ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL13(h01)
            M_SL13(h02)
            M_SL13(h03)
            M_SL13(h04)
            M_SL13(h05)
            M_SL13(h06)
            M_SL13(h07)
            M_SL13(h08)
            M_SL13(h09)
            M_SL13(h10)
            M_SL13(h11)
            M_SL13(h12)
            M_SL13(h13)
        
        }          // column loop
    }              // row loop

    M_finish()  
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map15() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper MACROs for this routine




//// declaration MACROs

#define M_DH15(x)   int16_t x##01, x##02, x##03, x##04, x##05, x##06, x##07, x##08, x##09, \
                     x##10, x##11, x##12, x##13, x##14, x##15;   

//         up through M_DDN(N-1 / 2)
#define M_DD15()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4) M_DDN(5) M_DDN(6) M_DDN(7)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR15()   int r1, r2, r3, r4, r5, r6, r7, row, r9, r10, r11, r12, r13, r14, r15; 
#define M_DC15()   int c1, c2, c3, c4, c5, c6, c7, col, c9, c10, c11, c12, c13, c14, c15;


///// code-generating MACROs

#define M_CH15(v)   if (h0808 != v) goto mismatch15; 
#define M_C15(x)    M_CH15(x##01) M_CH15(x##02) M_CH15(x##03) M_CH15(x##04) M_CH15(x##05) M_CH15(x##06) M_CH15(x##07) M_CH15(x##08) M_CH15(x##09) M_CH15(x##10) M_CH15(x##11) M_CH15(x##12) M_CH15(x##13) M_CH15(x##14) M_CH15(x##15) 

#define M_W15(r,c)  M_mapdata_attr(mapdata, r, c) |= MAPDATA_WATER;

#define M_SL15(x)   x##01=x##02;x##02=x##03;x##03=x##04;x##04=x##05;x##05=x##06;x##06=x##07;x##07=x##08;x##08=x##09;x##09=x##10;x##10=x##11;x##11=x##12;x##12=x##13;x##13=x##14;x##14=x##15;

#define M_WM15(x)   M_W15(x,c1) M_W15(x,c2) M_W15(x,c3) M_W15(x,c4) M_W15(x,c5) M_W15(x,c6) M_W15(x,c7) M_W15(x,col) M_W15(x,c9) M_W15(x,c10) M_W15(x,c11) M_W15(x,c12) M_W15(x,c13) M_W15(x,c14) M_W15(x,c15) 


#define M_SI15(x,r)                                  \
        x##01 = -11111;                              \
        x##02 = -11111;                              \
        x##03 = -11111;                              \
        x##04 = -11111;                              \
        x##05 = -11111;                              \
        x##06 = -11111;                              \
        x##07 = -11111;                              \
        x##08 = M_mapdata_ht(mapdata, r,  c0    );   \
        x##09 = M_mapdata_ht(mapdata, r,  c0 + 1);   \
        x##10 = M_mapdata_ht(mapdata, r,  c0 + 2);   \
        x##11 = M_mapdata_ht(mapdata, r,  c0 + 3);   \
        x##12 = M_mapdata_ht(mapdata, r,  c0 + 4);   \
        x##13 = M_mapdata_ht(mapdata, r,  c0 + 5);   \
        x##14 = M_mapdata_ht(mapdata, r,  c0 + 6);

        // don't do x15



#define M_NL15(x) if( ( col > c0 + x) && (row > r0 + x) && ( col < cm - x) && (row < rm - x) )   // don't place new lake too close to any edge



////////////////////////////////////////////////////


static void water_map15(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD15()
    M_DR15()
    M_DC15()
    M_DH15(h01);      // declare n x n group hxxyy
    M_DH15(h02);
    M_DH15(h03);
    M_DH15(h04);
    M_DH15(h05);
    M_DH15(h06);
    M_DH15(h07);
    M_DH15(h08);
    M_DH15(h09);
    M_DH15(h10);
    M_DH15(h11);
    M_DH15(h12);
    M_DH15(h13);
    M_DH15(h14);
    M_DH15(h15);
    
    M_DV(15)
 
    //////////////////////////////////
        
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row -- with stride 
    {
        r1  = M_max(r0, row - d7);
        r2  = M_max(r0, row - d6);
        r3  = M_max(r0, row - d5);  
        r4  = M_max(r0, row - d4);
        r5  = M_max(r0, row - d3);
        r6  = M_max(r0, row - d2);
        r7  = M_max(r0, row - d1); 
        //////
        r9  = M_min(rm, row + d1); 
        r10 = M_min(rm, row + d2);
        r11 = M_min(rm, row + d3);
        r12 = M_min(rm, row + d4);
        r13 = M_min(rm, row + d5);
        r14 = M_min(rm, row + d6);
        r15 = M_min(rm, row + d7);

        // set hx01 hx02 hx03 hx04 hx05 hx06 hx07 hx08 hx09 hx10 hx11 hx12 hx13 hx14 (but not hx15) for column 0
        
        M_SI15(h01, r1 )        
        M_SI15(h02, r2 )        
        M_SI15(h03, r3 )        
        M_SI15(h04, r4 )        
        M_SI15(h05, r5 ) 
        M_SI15(h06, r6 ) 
        M_SI15(h07, r7 )       
        M_SI15(h08, row)         
        M_SI15(h09, r9 )          
        M_SI15(h10, r10)         
        M_SI15(h11, r11)
        M_SI15(h12, r12)
        M_SI15(h13, r13)
        M_SI15(h14, r14)
        M_SI15(h15, r15)
        
        
        was_water = 0; 



        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride)
        {
            c15 = M_min(cm, col + d7); 

            // h11,h12 etc set at start of this line (before loop)

            h0115 = M_mapdata_ht(mapdata, r1,  c15); 
            h0215 = M_mapdata_ht(mapdata, r2,  c15);
            h0315 = M_mapdata_ht(mapdata, r3 , c15);
            h0415 = M_mapdata_ht(mapdata, r4 , c15);
            h0515 = M_mapdata_ht(mapdata, r5 , c15);
            h0615 = M_mapdata_ht(mapdata, r6 , c15);
            h0715 = M_mapdata_ht(mapdata, r7 , c15);
            h0815 = M_mapdata_ht(mapdata, row, c15);
            h0915 = M_mapdata_ht(mapdata, r9 , c15);
            h1015 = M_mapdata_ht(mapdata, r10, c15);
            h1115 = M_mapdata_ht(mapdata, r11, c15);
            h1215 = M_mapdata_ht(mapdata, r12, c15);
            h1315 = M_mapdata_ht(mapdata, r13, c15);
            h1415 = M_mapdata_ht(mapdata, r14, c15);
            h1515 = M_mapdata_ht(mapdata, r15, c15);


            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h0808 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C15(h08) // do minimally-clipped row, first 

                M_C15(h01) // compare MACROs go to mismatch label -- check all n x n points (some may be clipped)
                M_C15(h02)
                M_C15(h03)
                M_C15(h04)
                M_C15(h05)
                M_C15(h06)
                M_C15(h07)

                M_C15(h09)
                M_C15(h10)
                M_C15(h11)
                M_C15(h12)
                M_C15(h13)
                M_C15(h14)
                M_C15(h15)

              
                if M_CHT(h0808)     // exclude low-lying coastal areas and highter elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W15(r1 , c15)     // just set leading col of pixels to water
                        M_W15(r2 , c15)
                        M_W15(r3 , c15)
                        M_W15(r4 , c15)
                        M_W15(r5 , c15)
                        M_W15(r6 , c15)
                        M_W15(r7 , c15)
                        M_W15(row, c15)
                        M_W15(r9 , c15)
                        M_W15(r10, c15)
                        M_W15(r11, c15)
                        M_W15(r12, c15)
                        M_W15(r13, c15)
                        M_W15(r14, c15)
                        M_W15(r15, c15)
                     }
                     else
                     {   
                       M_NL15(0)                              // don't place lake too close to edge 
                       {
                        c1  = M_max(c0, col - d7);           //  compute clipped column numbers
                        c2  = M_max(c0, col - d6);
                        c3  = M_max(c0, col - d5);
                        c4  = M_max(c0, col - d4);
                        c5  = M_max(c0, col - d3);
                        c6  = M_max(c0, col - d2);
                        c7  = M_max(c0, col - d1);

                        c9  = M_min(cm, col + d1);           
                        c10 = M_min(cm, col + d2);
                        c11 = M_min(cm, col + d3);
                        c12 = M_min(cm, col + d4);
                        c13 = M_min(cm, col + d5);
                        c14 = M_min(cm, col + d6);
                     // c15 = already set earlier

                 //       M_WM15(r1 )                         // set all n x n points to water
                 //       M_WM15(r2 )
                 //       M_WM15(r3 )
                 //       M_WM15(r4 )
                 //       M_WM15(r5 )
                 //       M_WM15(r6 )
                 //       M_WM15(r7 )
                 //       M_WM15(row )    
                 //       M_WM15(r9 )
                 //       M_WM15(r10)
                 //       M_WM15(r11)
                 //       M_WM15(r12)
                 //       M_WM15(r13)
                 //       M_WM15(r14)
                 //       M_WM15(r15)
                        M_W15(row, col)     // just set center point to water -- fill will take care of rest of lake
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match15;
mismatch15:     was_water = 0;    // make sure to end lake due to mismatch ht
match15:         ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL15(h01)
            M_SL15(h02)
            M_SL15(h03)
            M_SL15(h04)
            M_SL15(h05)
            M_SL15(h06)
            M_SL15(h07)
            M_SL15(h08)
            M_SL15(h09)
            M_SL15(h10)
            M_SL15(h11)
            M_SL15(h12)
            M_SL15(h13)
            M_SL15(h14)
            M_SL15(h15)
        
        }          // column loop
    }              // row loop

    M_finish()  
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map17() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper MACROs for this routine

#undef  M_HNN
#undef  M_HNX
#undef  M_CNX
#undef  M_DNN
#undef  M_CHNN
#define M_HNN()    h0909
#define M_HNX()    h0917
#define M_CNX()    c17
#define M_DNN()    d8
#define M_CHNN(v)  if (M_HNN() != v) goto mismatch;


///// declaration MACROs

#define M_DH17(x)   int16_t x##01, x##02, x##03, x##04, x##05, x##06, x##07, x##08, x##09, \
                     x##10, x##11, x##12, x##13, x##14, x##15, x##16, x##17;   


//         up through M_DDN(N-1 / 2)
#define M_DD17()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4) M_DDN(5) M_DDN(6) M_DDN(7) M_DDN(8)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR17()   int r01, r02, r03, r04, r05, r06, r07, r08, row, r10, r11, r12, r13, r14, r15, r16, r17; 
#define M_DC17()   int c01, c02, c03, c04, c05, c06, c07, c08, col, c10, c11, c12, c13, c14, c15, c16, c17;


///// code-generating MACROs

#define M_C17(x)      M_CHNN(x##01) M_CHNN(x##02) M_CHNN(x##03) M_CHNN(x##04) M_CHNN(x##05) M_CHNN(x##06) M_CHNN(x##07) M_CHNN(x##08) M_CHNN(x##09) \
        M_CHNN(x##10) M_CHNN(x##11) M_CHNN(x##12) M_CHNN(x##13) M_CHNN(x##14) M_CHNN(x##15) M_CHNN(x##16) M_CHNN(x##17)


#define M_W17(r)    M_mapdata_attr(mapdata, r, M_CNX()) |= MAPDATA_WATER;



#define M_SL17(x)   x##01=x##02; x##02=x##03; x##03=x##04; x##04=x##05; x##05=x##06; x##06=x##07; x##07=x##08; x##08=x##09; x##09=x##10;\
       x##10=x##11; x##11=x##12; x##12=x##13; x##13=x##14; x##14=x##15; x##15=x##16; x##16=x##17;


#define M_SI17(x,r)                                  \
        x##01 = -11111;                              \
        x##02 = -11111;                              \
        x##03 = -11111;                              \
        x##04 = -11111;                              \
        x##05 = -11111;                              \
        x##06 = -11111;                              \
        x##07 = -11111;                              \
        x##08 = -11111;                              \
        x##09 = M_mapdata_ht(mapdata, r,  c0    );   \
        x##10 = M_mapdata_ht(mapdata, r,  c0 + 1);   \
        x##11 = M_mapdata_ht(mapdata, r,  c0 + 2);   \
        x##12 = M_mapdata_ht(mapdata, r,  c0 + 3);   \
        x##13 = M_mapdata_ht(mapdata, r,  c0 + 4);   \
        x##14 = M_mapdata_ht(mapdata, r,  c0 + 5);   \
        x##15 = M_mapdata_ht(mapdata, r,  c0 + 6);   \
        x##16 = M_mapdata_ht(mapdata, r,  c0 + 7);

        // don't do x##N   --  x##01 to x##(N-1 / 2) = -11111;      x#(N+1 / 2) thru X(N-1) set to c0 + 1 thru c0 + c(N-3 / 2)


#define M_SH17(x) h##x##17 = M_mapdata_ht(mapdata, r##x, M_CNX());


////////////////////////////////////////////////////


static void water_map17(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD17()
    M_DR17()
    M_DC17()
    M_DH17(h01)      // declare n x n group hxxyy
    M_DH17(h02)
    M_DH17(h03)
    M_DH17(h04)
    M_DH17(h05)
    M_DH17(h06)
    M_DH17(h07)
    M_DH17(h08)
    M_DH17(h09)
    M_DH17(h10)
    M_DH17(h11)
    M_DH17(h12)
    M_DH17(h13)
    M_DH17(h14)
    M_DH17(h15)
    M_DH17(h16)
    M_DH17(h17)

    M_DV(17)
    
    /////////////////////////  
        
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row -- with stride 
    {
        r01  = M_max(r0, row - d8);
        r02  = M_max(r0, row - d7);
        r03  = M_max(r0, row - d6);  
        r04  = M_max(r0, row - d5);
        r05  = M_max(r0, row - d4);
        r06  = M_max(r0, row - d3);
        r07  = M_max(r0, row - d2); 
        r08  = M_max(r0, row - d1);
        ////// r09         
        r10 = M_min(rm, row + d1);
        r11 = M_min(rm, row + d2);
        r12 = M_min(rm, row + d3);
        r13 = M_min(rm, row + d4);
        r14 = M_min(rm, row + d5);
        r15 = M_min(rm, row + d6);
        r16 = M_min(rm, row + d7);
        r17 = M_min(rm, row + d8);

        // set hx01 hx02 hx03 hx04 hx05 hx06 hx07 hx08 hx09 hx10 hx11 hx12 hx13 hx14 ..  (but not hxN) for column 0
        
        M_SI17(h01, r01)        
        M_SI17(h02, r02)        
        M_SI17(h03, r03)        
        M_SI17(h04, r04)        
        M_SI17(h05, r05) 
        M_SI17(h06, r06) 
        M_SI17(h07, r07) 
        M_SI17(h08, r08)
        M_SI17(h09, row)         
        M_SI17(h10, r10)         
        M_SI17(h11, r11)
        M_SI17(h12, r12)
        M_SI17(h13, r13)
        M_SI17(h14, r14)
        M_SI17(h15, r15)
        M_SI17(h16, r16)
        M_SI17(h17, r17)
        
        was_water = 0; 


        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride)
        {
            M_CNX() = M_min(cm, col + M_DNN());  // "col + d(N - 1 / 2)"

            // h0101,h0102 etc set at start of this line (before loop)

            M_SH17(01)
            M_SH17(02)
            M_SH17(03)
            M_SH17(04)
            M_SH17(05)
            M_SH17(06)
            M_SH17(07)
            M_SH17(08)
            M_HNX() = M_mapdata_ht(mapdata, row, M_CNX());
            M_SH17(10)
            M_SH17(11)
            M_SH17(12)
            M_SH17(13)
            M_SH17(14)
            M_SH17(15)
            M_SH17(16)
            M_SH17(17)
            

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (M_HNN() != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C17(h09)  // do minimally-clipped row, first -- h(N+1 / 2)

                M_C17(h01) // compare MACROs go to mismatch label -- check all n x n points (some may be clipped)
                M_C17(h02)
                M_C17(h03)
                M_C17(h04)
                M_C17(h05)
                M_C17(h06)
                M_C17(h07)
                M_C17(h08)
                //    h09  already done above
                M_C17(h10)
                M_C17(h11)
                M_C17(h12)
                M_C17(h13)
                M_C17(h14)
                M_C17(h15)
                M_C17(h16)
                M_C17(h17)

              
                if M_CHT(M_HNN())     // exclude low-lying coastal areas and highter elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W17(r01)     // just set leading col of pixels to water
                        M_W17(r02)
                        M_W17(r03)
                        M_W17(r04)
                        M_W17(r05)
                        M_W17(r06)
                        M_W17(r07)
                        M_W17(r08)
                        M_W17(row)                        
                        M_W17(r10)
                        M_W17(r11)
                        M_W17(r12)
                        M_W17(r13)
                        M_W17(r14)
                        M_W17(r15)
                        M_W17(r16)
                        M_W17(r17)
                     }
                     else
                     {   
                       M_NLE(0)                               // don't place lake too close to edge 
                       {
                        c01  = M_max(c0, col - d8);           //  compute clipped column numbers
                        c02  = M_max(c0, col - d7);
                        c03  = M_max(c0, col - d6);
                        c04  = M_max(c0, col - d5);
                        c05  = M_max(c0, col - d4);
                        c06  = M_max(c0, col - d3);
                        c07  = M_max(c0, col - d2);
                        c08  = M_max(c0, col - d1);
                        // don't do c(N+1 / 2) = "col"  
                        c10 = M_min(cm, col + d1);
                        c11 = M_min(cm, col + d2);
                        c12 = M_min(cm, col + d3);
                        c13 = M_min(cm, col + d4);
                        c14 = M_min(cm, col + d5);
                        c15 = M_min(cm, col + d6);
                        c16 = M_min(cm, col + d7);
                     // c17 = already set earlier

                        M_mapdata_attr(mapdata, row, col) |= MAPDATA_WATER;   // just set center point to water -- fill will take care of rest of lake
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match;
mismatch:       was_water = 0;    // make sure to end lake due to mismatch ht
match:          ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL17(h01)
            M_SL17(h02)
            M_SL17(h03)
            M_SL17(h04)
            M_SL17(h05)
            M_SL17(h06)
            M_SL17(h07)
            M_SL17(h08)
            M_SL17(h09)
            M_SL17(h10)
            M_SL17(h11)
            M_SL17(h12)
            M_SL17(h13)
            M_SL17(h14)
            M_SL17(h15)
            M_SL17(h16)
            M_SL17(h17)
        
        }          // column loop
    }              // row loop

    M_finish()  
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map19() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// helper MACROs for this routine

#undef  M_HNN
#undef  M_HNX
#undef  M_CNX
#undef  M_DNN
#undef  M_CHNN
#define M_HNN()    h1010
#define M_HNX()    h1019
#define M_CNX()    c19
#define M_DNN()    d9
#define M_CHNN(v)  if (M_HNN() != v) goto mismatch;


///// declaration MACROs

#define M_DH19(x)   int16_t x##01, x##02, x##03, x##04, x##05, x##06, x##07, x##08, x##09, \
                     x##10, x##11, x##12, x##13, x##14, x##15, x##16, x##17, x##18, x##19;

//         up through M_DDN(N-1 / 2)
#define M_DD19()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4) M_DDN(5) M_DDN(6) M_DDN(7) M_DDN(8) M_DDN(9)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR19()   int r01, r02, r03, r04, r05, r06, r07, r08, r09, row, r11, r12, r13, r14, r15, r16, r17, r18, r19; 
#define M_DC19()   int c01, c02, c03, c04, c05, c06, c07, c08, c09, col, c11, c12, c13, c14, c15, c16, c17, c18, c19;


///// code-generating MACROs

#define M_C19(x)      M_CHNN(x##01) M_CHNN(x##02) M_CHNN(x##03) M_CHNN(x##04) M_CHNN(x##05) M_CHNN(x##06) M_CHNN(x##07) M_CHNN(x##08) M_CHNN(x##09) \
        M_CHNN(x##10) M_CHNN(x##11) M_CHNN(x##12) M_CHNN(x##13) M_CHNN(x##14) M_CHNN(x##15) M_CHNN(x##16) M_CHNN(x##17) M_CHNN(x##18) M_CHNN(x##19)


#define M_W19(r)    M_mapdata_attr(mapdata, r, M_CNX()) |= MAPDATA_WATER;



#define M_SL19(x)   x##01=x##02; x##02=x##03; x##03=x##04; x##04=x##05; x##05=x##06; x##06=x##07; x##07=x##08; x##08=x##09; x##09=x##10;\
       x##10=x##11; x##11=x##12; x##12=x##13; x##13=x##14; x##14=x##15; x##15=x##16; x##16=x##17; x##17=x##18; x##18=x##19;


#define M_SI19(x,r)                                  \
        x##01 = -11111;                              \
        x##02 = -11111;                              \
        x##03 = -11111;                              \
        x##04 = -11111;                              \
        x##05 = -11111;                              \
        x##06 = -11111;                              \
        x##07 = -11111;                              \
        x##08 = -11111;                              \
        x##09 = -11111;                              \
        x##10 = M_mapdata_ht(mapdata, r,  c0    );   \
        x##11 = M_mapdata_ht(mapdata, r,  c0 + 1);   \
        x##12 = M_mapdata_ht(mapdata, r,  c0 + 2);   \
        x##13 = M_mapdata_ht(mapdata, r,  c0 + 3);   \
        x##14 = M_mapdata_ht(mapdata, r,  c0 + 4);   \
        x##15 = M_mapdata_ht(mapdata, r,  c0 + 5);   \
        x##16 = M_mapdata_ht(mapdata, r,  c0 + 6);   \
        x##17 = M_mapdata_ht(mapdata, r,  c0 + 7);   \
        x##18 = M_mapdata_ht(mapdata, r,  c0 + 8);   
      //x##19  
      // don't do x##N   --  x##01 to x##(N-1 / 2) = -11111;      x#(N+1 / 2) thru X(N-1) set to c0 + 1 thru c0 + c(N-3 / 2)


#define M_SH19(x) h##x##19 = M_mapdata_ht(mapdata, r##x, M_CNX());




////////////////////////////////////////////////////


static void water_map19(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD19()
    M_DR19()
    M_DC19()

    M_DH19(h01)      // declare n x n group hxxyy
    M_DH19(h02)
    M_DH19(h03)
    M_DH19(h04)
    M_DH19(h05)
    M_DH19(h06)
    M_DH19(h07)
    M_DH19(h08)
    M_DH19(h09)
    M_DH19(h10)
    M_DH19(h11)
    M_DH19(h12)
    M_DH19(h13)
    M_DH19(h14)
    M_DH19(h15)
    M_DH19(h16)
    M_DH19(h17)
    M_DH19(h18)
    M_DH19(h19)

    M_DV(19)
    
    /////////////////////////  
        
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row -- with stride 
    {
        r01  = M_max(r0, row - d9);
        r02  = M_max(r0, row - d8);
        r03  = M_max(r0, row - d7);  
        r04  = M_max(r0, row - d6);
        r05  = M_max(r0, row - d5);
        r06  = M_max(r0, row - d4);
        r07  = M_max(r0, row - d3); 
        r08  = M_max(r0, row - d2);
        r09  = M_max(r0, row - d1);
        ////// r10         
        r11 = M_min(rm, row + d1);
        r12 = M_min(rm, row + d2);
        r13 = M_min(rm, row + d3);
        r14 = M_min(rm, row + d4);
        r15 = M_min(rm, row + d5);
        r16 = M_min(rm, row + d6);
        r17 = M_min(rm, row + d7);
        r18 = M_min(rm, row + d8);
        r19 = M_min(rm, row + d9);

        // set hx01 hx02 hx03 hx04 hx05 hx06 hx07 hx08 hx09 hx10 hx11 hx12 hx13 hx14 ..  (but not hxN) for column 0
        
        M_SI19(h01, r01)        
        M_SI19(h02, r02)        
        M_SI19(h03, r03)        
        M_SI19(h04, r04)        
        M_SI19(h05, r05) 
        M_SI19(h06, r06) 
        M_SI19(h07, r07) 
        M_SI19(h08, r08)
        M_SI19(h09, r09)         
        M_SI19(h10, row)         
        M_SI19(h11, r11)
        M_SI19(h12, r12)
        M_SI19(h13, r13)
        M_SI19(h14, r14)
        M_SI19(h15, r15)
        M_SI19(h16, r16)
        M_SI19(h17, r17)
        M_SI19(h18, r18)
        M_SI19(h19, r19)
        
        was_water = 0; 


        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride)
        {
            M_CNX() = M_min(cm, col + M_DNN());       // "col + d(N - 1 / 2)"

            // h0101,h0102 etc set at start of this line (before loop)

            M_SH19(01)
            M_SH19(02)
            M_SH19(03)
            M_SH19(04)
            M_SH19(05)
            M_SH19(06)
            M_SH19(07)
            M_SH19(08)
            M_SH19(09)
            M_HNX() = M_mapdata_ht(mapdata, row, M_CNX());
            M_SH19(11)
            M_SH19(12)
            M_SH19(13)
            M_SH19(14)
            M_SH19(15)
            M_SH19(16)
            M_SH19(17)
            M_SH19(18)
            M_SH19(19)
            

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h0909 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C19(h10)  // do minimally-clipped row, first -- h(N+1 / 2)

                M_C19(h01) // compare MACROs go to mismatch label -- check all n x n points (some may be clipped)
                M_C19(h02)
                M_C19(h03)
                M_C19(h04)
                M_C19(h05)
                M_C19(h06)
                M_C19(h07)
                M_C19(h08)
                M_C19(h09)
                //    h10  already done above
                M_C19(h11)
                M_C19(h12)
                M_C19(h13)
                M_C19(h14)
                M_C19(h15)
                M_C19(h16)
                M_C19(h17)
                M_C19(h18)
                M_C19(h19)

              
                if M_CHT(M_HNN())     // exclude low-lying coastal areas and highter elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W19(r01)     // just set leading col of pixels to water
                        M_W19(r02)
                        M_W19(r03)
                        M_W19(r04)
                        M_W19(r05)
                        M_W19(r06)
                        M_W19(r07)
                        M_W19(r08)
                        M_W19(r09)                        
                        M_W19(row)
                        M_W19(r11)
                        M_W19(r12)
                        M_W19(r13)
                        M_W19(r14)
                        M_W19(r15)
                        M_W19(r16)
                        M_W19(r17)
                        M_W19(r18)
                        M_W19(r19)
                     }
                     else
                     {   
                       M_NLE(0)                              // don't place lake too close to edge 
                       {
                        c01  = M_max(c0, col - d9);           //  compute clipped column numbers
                        c02  = M_max(c0, col - d8);
                        c03  = M_max(c0, col - d7);
                        c04  = M_max(c0, col - d6);
                        c05  = M_max(c0, col - d5);
                        c06  = M_max(c0, col - d4);
                        c07  = M_max(c0, col - d3);
                        c08  = M_max(c0, col - d2);
                        c09  = M_max(c0, col - d1);
                        // don't do c(N+1 / 2) = "col"  
                        c11 = M_min(cm, col + d1);
                        c12 = M_min(cm, col + d2);
                        c13 = M_min(cm, col + d3);
                        c14 = M_min(cm, col + d4);
                        c15 = M_min(cm, col + d5);
                        c16 = M_min(cm, col + d6);
                        c17 = M_min(cm, col + d7);
                        c18 = M_min(cm, col + d8);
                     // c19 = already set earlier

                        M_mapdata_attr(mapdata, row, col) |= MAPDATA_WATER;   // just set center point to water -- fill will take care of rest of lake
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match;
mismatch:       was_water = 0;    // make sure to end lake due to mismatch ht
match:          ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL19(h01)
            M_SL19(h02)
            M_SL19(h03)
            M_SL19(h04)
            M_SL19(h05)
            M_SL19(h06)
            M_SL19(h07)
            M_SL19(h08)
            M_SL19(h09)
            M_SL19(h10)
            M_SL19(h11)
            M_SL19(h12)
            M_SL19(h13)
            M_SL19(h14)
            M_SL19(h15)
            M_SL19(h16)
            M_SL19(h17)
            M_SL19(h18)
            M_SL19(h19)
        
        }          // column loop
    }              // row loop

    M_finish()  
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map21() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// helper MACROs for this routine

#undef  M_HNN
#undef  M_HNX
#undef  M_CNX
#undef  M_DNN
#undef  M_CHNN
#define M_HNN()    h1111
#define M_HNX()    h1121
#define M_CNX()    c21
#define M_DNN()    d10
#define M_CHNN(v)  if (M_HNN() != v) goto mismatch; 


///// declaration MACROs

#define M_DH21(x)   int16_t x##01, x##02, x##03, x##04, x##05, x##06, x##07, x##08, x##09, \
                     x##10, x##11, x##12, x##13, x##14, x##15, x##16, x##17, x##18, x##19, \
                     x##20, x##21;

//         up through M_DDN(N-1 / 2)
#define M_DD21()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4) M_DDN(5) M_DDN(6) M_DDN(7) M_DDN(8) M_DDN(9) M_DDN(10)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR21()   int r01, r02, r03, r04, r05, r06, r07, r08, r09, r10, row, r12, r13, r14, r15, r16, r17, r18, r19, r20, r21; 
#define M_DC21()   int c01, c02, c03, c04, c05, c06, c07, c08, c09, c10, col, c12, c13, c14, c15, c16, c17, c18, c19, c20, c21;


///// code-generating MACROs

#define M_C21(x)      M_CHNN(x##01) M_CHNN(x##02) M_CHNN(x##03) M_CHNN(x##04) M_CHNN(x##05) M_CHNN(x##06) M_CHNN(x##07) M_CHNN(x##08) M_CHNN(x##09) \
        M_CHNN(x##10) M_CHNN(x##11) M_CHNN(x##12) M_CHNN(x##13) M_CHNN(x##14) M_CHNN(x##15) M_CHNN(x##16) M_CHNN(x##17) M_CHNN(x##18) M_CHNN(x##19) \
        M_CHNN(x##20) M_CHNN(x##21)

#define M_W21(r)    M_mapdata_attr(mapdata, r, M_CNX()) |= MAPDATA_WATER;



#define M_SL21(x)   x##01=x##02; x##02=x##03; x##03=x##04; x##04=x##05; x##05=x##06; x##06=x##07; x##07=x##08; x##08=x##09; x##09=x##10;\
       x##10=x##11; x##11=x##12; x##12=x##13; x##13=x##14; x##14=x##15; x##15=x##16; x##16=x##17; x##17=x##18; x##18=x##19; x##19=x##20;\
       x##20=x##21;

#define M_SI21(x,r)                                  \
        x##01 = -11111;                              \
        x##02 = -11111;                              \
        x##03 = -11111;                              \
        x##04 = -11111;                              \
        x##05 = -11111;                              \
        x##06 = -11111;                              \
        x##07 = -11111;                              \
        x##08 = -11111;                              \
        x##09 = -11111;                              \
        x##10 = -11111;                              \
        x##11 = M_mapdata_ht(mapdata, r,  c0    );   \
        x##12 = M_mapdata_ht(mapdata, r,  c0 + 1);   \
        x##13 = M_mapdata_ht(mapdata, r,  c0 + 2);   \
        x##14 = M_mapdata_ht(mapdata, r,  c0 + 3);   \
        x##15 = M_mapdata_ht(mapdata, r,  c0 + 4);   \
        x##16 = M_mapdata_ht(mapdata, r,  c0 + 5);   \
        x##17 = M_mapdata_ht(mapdata, r,  c0 + 6);   \
        x##18 = M_mapdata_ht(mapdata, r,  c0 + 7);   \
        x##19 = M_mapdata_ht(mapdata, r,  c0 + 8);   \
        x##20 = M_mapdata_ht(mapdata, r,  c0 + 9);   
      //x##21  
      // don't do x##N   --  x##01 to x##(N-1 / 2) = -11111;      x#(N+1 / 2) thru X(N-1) set to c0 + 1 thru c0 + c(N-3 / 2)


#define M_SH21(x) h##x##21 = M_mapdata_ht(mapdata, r##x, M_CNX());




////////////////////////////////////////////////////


static void water_map21(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD21()
    M_DR21()
    M_DC21()

    M_DH21(h01)      // declare n x n group hxxyy
    M_DH21(h02)
    M_DH21(h03)
    M_DH21(h04)
    M_DH21(h05)
    M_DH21(h06)
    M_DH21(h07)
    M_DH21(h08)
    M_DH21(h09)
    M_DH21(h10)
    M_DH21(h11)
    M_DH21(h12)
    M_DH21(h13)
    M_DH21(h14)
    M_DH21(h15)
    M_DH21(h16)
    M_DH21(h17)
    M_DH21(h18)
    M_DH21(h19)
    M_DH21(h20)
    M_DH21(h21)

    M_DV(21)
    
    /////////////////////////  
        
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row -- with stride 
    {
        r01  = M_max(r0, row - d10 );
        r02  = M_max(r0, row - d9 );
        r03  = M_max(r0, row - d8 );  
        r04  = M_max(r0, row - d7 );
        r05  = M_max(r0, row - d6 );
        r06  = M_max(r0, row - d5 );
        r07  = M_max(r0, row - d4 ); 
        r08  = M_max(r0, row - d3 );
        r09  = M_max(r0, row - d2 );
        r10  = M_max(r0, row - d1 );
        ////// r11         
        r12 = M_min(rm, row + d1);
        r13 = M_min(rm, row + d2);
        r14 = M_min(rm, row + d3);
        r15 = M_min(rm, row + d4);
        r16 = M_min(rm, row + d5);
        r17 = M_min(rm, row + d6);
        r18 = M_min(rm, row + d7);
        r19 = M_min(rm, row + d8);
        r20 = M_min(rm, row + d9);
        r21 = M_min(rm, row + d10);

        // set hx01 hx02 hx03 hx04 hx05 hx06 hx07 hx08 hx09 hx10 hx11 hx12 hx13 hx14 ..  (but not hxN) for column 0
        
        M_SI21(h01, r01)        
        M_SI21(h02, r02)        
        M_SI21(h03, r03)        
        M_SI21(h04, r04)        
        M_SI21(h05, r05) 
        M_SI21(h06, r06) 
        M_SI21(h07, r07) 
        M_SI21(h08, r08)
        M_SI21(h09, r09)         
        M_SI21(h10, r10)         
        M_SI21(h11, row)
        M_SI21(h12, r12)
        M_SI21(h13, r13)
        M_SI21(h14, r14)
        M_SI21(h15, r15)
        M_SI21(h16, r16)
        M_SI21(h17, r17)
        M_SI21(h18, r18)
        M_SI21(h19, r19)
        M_SI21(h20, r20)
        M_SI21(h21, r21)
        
        was_water = 0; 


        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride)
        {
            M_CNX() = M_min(cm, col + M_DNN());       // "col + d(N - 1 / 2)"

            // h0101,h0102 etc set at start of this line (before loop)

            M_SH21(01)
            M_SH21(02)
            M_SH21(03)
            M_SH21(04)
            M_SH21(05)
            M_SH21(06)
            M_SH21(07)
            M_SH21(08)
            M_SH21(09)
            M_SH21(10)
            M_HNX() = M_mapdata_ht(mapdata, row, M_CNX());
            M_SH21(12)
            M_SH21(13)
            M_SH21(14)
            M_SH21(15)
            M_SH21(16)
            M_SH21(17)
            M_SH21(18)
            M_SH21(19)
            M_SH21(20)
            M_SH21(21)
            

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h0909 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C21(h11)  // do minimally-clipped row, first -- h(N+1 / 2)

                M_C21(h01) // compare MACROs go to mismatch label -- check all n x n points (some may be clipped)
                M_C21(h02)
                M_C21(h03)
                M_C21(h04)
                M_C21(h05)
                M_C21(h06)
                M_C21(h07)
                M_C21(h08)
                M_C21(h09)
                M_C21(h10)
                //    h11  already done above
                M_C21(h12)
                M_C21(h13)
                M_C21(h14)
                M_C21(h15)
                M_C21(h16)
                M_C21(h17)
                M_C21(h18)
                M_C21(h19)
                M_C21(h20)
                M_C21(h21)

              
                if M_CHT(M_HNN())     // exclude low-lying coastal areas and highter elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W21(r01)     // just set leading col of pixels to water
                        M_W21(r02)
                        M_W21(r03)
                        M_W21(r04)
                        M_W21(r05)
                        M_W21(r06)
                        M_W21(r07)
                        M_W21(r08)
                        M_W21(r09)
                        M_W21(r10)
                        M_W21(row)
                        M_W21(r12)
                        M_W21(r13)
                        M_W21(r14)
                        M_W21(r15)
                        M_W21(r16)
                        M_W21(r17)
                        M_W21(r18)
                        M_W21(r19)
                        M_W21(r20)
                        M_W21(r21)
                     }
                     else
                     {   
                       M_NLE(0)                              // don't place lake too close to edge 
                       {
                        c01  = M_max(c0, col - d10);        //  compute clipped column numbers
                        c02  = M_max(c0, col - d9);
                        c03  = M_max(c0, col - d8);
                        c04  = M_max(c0, col - d7);
                        c05  = M_max(c0, col - d6);
                        c06  = M_max(c0, col - d5);
                        c07  = M_max(c0, col - d4);
                        c08  = M_max(c0, col - d3);
                        c09  = M_max(c0, col - d2);
                        c10  = M_max(c0, col - d1);
                        // don't do c(N+1 / 2) = "col"  
                        c12 = M_min(cm, col + d1);
                        c13 = M_min(cm, col + d2);
                        c14 = M_min(cm, col + d3);
                        c15 = M_min(cm, col + d4);
                        c16 = M_min(cm, col + d5);
                        c17 = M_min(cm, col + d6);
                        c18 = M_min(cm, col + d7);
                        c19 = M_min(cm, col + d8);
                        c20 = M_min(cm, col + d9);
                     // c21 = already set earlier

                        M_mapdata_attr(mapdata, row, col) |= MAPDATA_WATER;   // just set center point to water -- fill will take care of rest of lake
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match;
mismatch:       was_water = 0;    // make sure to end lake due to mismatch ht
match:          ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL21(h01)
            M_SL21(h02)
            M_SL21(h03)
            M_SL21(h04)
            M_SL21(h05)
            M_SL21(h06)
            M_SL21(h07)
            M_SL21(h08)
            M_SL21(h09)
            M_SL21(h10)
            M_SL21(h11)
            M_SL21(h12)
            M_SL21(h13)
            M_SL21(h14)
            M_SL21(h15)
            M_SL21(h16)
            M_SL21(h17)
            M_SL21(h18)
            M_SL21(h19)
            M_SL21(h20)
            M_SL21(h21)
        
        }          // column loop
    }              // row loop

    M_finish()  
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map23() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// helper MACROs for this routine

#undef  M_HNN
#undef  M_HNX
#undef  M_CNX
#undef  M_DNN
#undef  M_CHNN
#define M_HNN()    h1212
#define M_HNX()    h1223
#define M_CNX()    c23
#define M_DNN()    d11
#define M_CHNN(v)  if (M_HNN() != v) goto mismatch; 


///// declaration MACROs

#define M_DH23(x)   int16_t x##01, x##02, x##03, x##04, x##05, x##06, x##07, x##08, x##09, \
                     x##10, x##11, x##12, x##13, x##14, x##15, x##16, x##17, x##18, x##19, \
                     x##20, x##21, x##22, x##23;

//         up through M_DDN(N-1 / 2)
#define M_DD23()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4) M_DDN(5) M_DDN(6) M_DDN(7) M_DDN(8) M_DDN(9) M_DDN(10) M_DDN(11)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR23()   int r01, r02, r03, r04, r05, r06, r07, r08, r09, r10, r11, row, r13, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23; 
#define M_DC23()   int c01, c02, c03, c04, c05, c06, c07, c08, c09, c10, c11, col, c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23;


///// code-generating MACROs

#define M_C23(x)      M_CHNN(x##01) M_CHNN(x##02) M_CHNN(x##03) M_CHNN(x##04) M_CHNN(x##05) M_CHNN(x##06) M_CHNN(x##07) M_CHNN(x##08) M_CHNN(x##09) \
        M_CHNN(x##10) M_CHNN(x##11) M_CHNN(x##12) M_CHNN(x##13) M_CHNN(x##14) M_CHNN(x##15) M_CHNN(x##16) M_CHNN(x##17) M_CHNN(x##18) M_CHNN(x##19) \
        M_CHNN(x##20) M_CHNN(x##21) M_CHNN(x##22) M_CHNN(x##23)

#define M_W23(r)    M_mapdata_attr(mapdata, r, M_CNX()) |= MAPDATA_WATER;



#define M_SL23(x)   x##01=x##02; x##02=x##03; x##03=x##04; x##04=x##05; x##05=x##06; x##06=x##07; x##07=x##08; x##08=x##09; x##09=x##10;\
       x##10=x##11; x##11=x##12; x##12=x##13; x##13=x##14; x##14=x##15; x##15=x##16; x##16=x##17; x##17=x##18; x##18=x##19; x##19=x##20;\
       x##20=x##21; x##21=x##22; x##22=x##23;

#define M_SI23(x,r)                                  \
        x##01 = -11111;                              \
        x##02 = -11111;                              \
        x##03 = -11111;                              \
        x##04 = -11111;                              \
        x##05 = -11111;                              \
        x##06 = -11111;                              \
        x##07 = -11111;                              \
        x##08 = -11111;                              \
        x##09 = -11111;                              \
        x##10 = -11111;                              \
        x##11 = -11111;                              \
        x##12 = M_mapdata_ht(mapdata, r,  c0    );   \
        x##13 = M_mapdata_ht(mapdata, r,  c0 + 1);   \
        x##14 = M_mapdata_ht(mapdata, r,  c0 + 2);   \
        x##15 = M_mapdata_ht(mapdata, r,  c0 + 3);   \
        x##16 = M_mapdata_ht(mapdata, r,  c0 + 4);   \
        x##17 = M_mapdata_ht(mapdata, r,  c0 + 5);   \
        x##18 = M_mapdata_ht(mapdata, r,  c0 + 6);   \
        x##19 = M_mapdata_ht(mapdata, r,  c0 + 7);   \
        x##20 = M_mapdata_ht(mapdata, r,  c0 + 8);   \
        x##21 = M_mapdata_ht(mapdata, r,  c0 + 9);   \
        x##22 = M_mapdata_ht(mapdata, r,  c0 + 10);   
      //x##23  
      // don't do x##N   --  x##01 to x##(N-1 / 2) = -11111;      x#(N+1 / 2) thru X(N-1) set to c0 + 1 thru c0 + c(N-3 / 2)


#define M_SH23(x) h##x##23 = M_mapdata_ht(mapdata, r##x, M_CNX());




////////////////////////////////////////////////////


static void water_map23(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD23()
    M_DR23()
    M_DC23()

    M_DH23(h01)      // declare n x n group hxxyy
    M_DH23(h02)
    M_DH23(h03)
    M_DH23(h04)
    M_DH23(h05)
    M_DH23(h06)
    M_DH23(h07)
    M_DH23(h08)
    M_DH23(h09)
    M_DH23(h10)
    M_DH23(h11)
    M_DH23(h12)
    M_DH23(h13)
    M_DH23(h14)
    M_DH23(h15)
    M_DH23(h16)
    M_DH23(h17)
    M_DH23(h18)
    M_DH23(h19)
    M_DH23(h20)
    M_DH23(h21)
    M_DH23(h22)
    M_DH23(h23)

    M_DV(23)
    
    /////////////////////////  
        
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row -- with stride 
    {
        r01  = M_max(r0, row - d11);
        r02  = M_max(r0, row - d10);
        r03  = M_max(r0, row - d9 );
        r04  = M_max(r0, row - d8 );  
        r05  = M_max(r0, row - d7 );
        r06  = M_max(r0, row - d6 );
        r07  = M_max(r0, row - d5 );
        r08  = M_max(r0, row - d4 ); 
        r09  = M_max(r0, row - d3 );
        r10  = M_max(r0, row - d2 );
        r11  = M_max(r0, row - d1 );
        ////// r12         
        r13 = M_min(rm, row + d1 );
        r14 = M_min(rm, row + d2 );
        r15 = M_min(rm, row + d3 );
        r16 = M_min(rm, row + d4 );
        r17 = M_min(rm, row + d5 );
        r18 = M_min(rm, row + d6 );
        r19 = M_min(rm, row + d7 );
        r20 = M_min(rm, row + d8 );
        r21 = M_min(rm, row + d9 );
        r22 = M_min(rm, row + d10);
        r23 = M_min(rm, row + d11);

        // set hx01 hx02 hx03 hx04 hx05 hx06 hx07 hx08 hx09 hx10 hx11 hx12 hx13 hx14 ..  (but not hxN) for column 0
        
        M_SI23(h01, r01)        
        M_SI23(h02, r02)        
        M_SI23(h03, r03)        
        M_SI23(h04, r04)        
        M_SI23(h05, r05) 
        M_SI23(h06, r06) 
        M_SI23(h07, r07) 
        M_SI23(h08, r08)
        M_SI23(h09, r09)         
        M_SI23(h10, r10)         
        M_SI23(h11, r11)
        M_SI23(h12, row)
        M_SI23(h13, r13)
        M_SI23(h14, r14)
        M_SI23(h15, r15)
        M_SI23(h16, r16)
        M_SI23(h17, r17)
        M_SI23(h18, r18)
        M_SI23(h19, r19)
        M_SI23(h20, r20)
        M_SI23(h21, r21)
        M_SI23(h22, r22)
        M_SI23(h23, r23)
        
        was_water = 0; 


        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride)
        {
            M_CNX() = M_min(cm, col + M_DNN());       // "col + d(N - 1 / 2)"

            // h0101,h0102 etc set at start of this line (before loop)

            M_SH23(01)
            M_SH23(02)
            M_SH23(03)
            M_SH23(04)
            M_SH23(05)
            M_SH23(06)
            M_SH23(07)
            M_SH23(08)
            M_SH23(09)
            M_SH23(10)
            M_SH23(11)
            M_HNX() = M_mapdata_ht(mapdata, row, M_CNX());
            M_SH23(13)
            M_SH23(14)
            M_SH23(15)
            M_SH23(16)
            M_SH23(17)
            M_SH23(18)
            M_SH23(19)
            M_SH23(20)
            M_SH23(21)
            M_SH23(22)
            M_SH23(23)
            

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h0909 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C23(h12)  // do minimally-clipped row, first -- h(N+1 / 2)

                M_C23(h01) // compare MACROs go to mismatch label -- check all n x n points (some may be clipped)
                M_C23(h02)
                M_C23(h03)
                M_C23(h04)
                M_C23(h05)
                M_C23(h06)
                M_C23(h07)
                M_C23(h08)
                M_C23(h09)
                M_C23(h10)
                M_C23(h11)
                //    h12  already done above
                M_C23(h13)
                M_C23(h14)
                M_C23(h15)
                M_C23(h16)
                M_C23(h17)
                M_C23(h18)
                M_C23(h19)
                M_C23(h20)
                M_C23(h21)
                M_C23(h22)
                M_C23(h23)

              
                if M_CHT(M_HNN())     // exclude low-lying coastal areas and highter elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W23(r01)     // just set leading col of pixels to water
                        M_W23(r02)
                        M_W23(r03)
                        M_W23(r04)
                        M_W23(r05)
                        M_W23(r06)
                        M_W23(r07)
                        M_W23(r08)
                        M_W23(r09)
                        M_W23(r10)
                        M_W23(r11)
                        M_W23(row)
                        M_W23(r13)
                        M_W23(r14)
                        M_W23(r15)
                        M_W23(r16)
                        M_W23(r17)
                        M_W23(r18)
                        M_W23(r19)
                        M_W23(r20)
                        M_W23(r21)
                        M_W23(r22)
                        M_W23(r23)
                     }
                     else
                     {   
                       M_NLE(0)                              // don't place lake too close to edge 
                       {
                        c01  = M_max(c0, col - d11);        //  compute clipped column numbers
                        c02  = M_max(c0, col - d10);
                        c03  = M_max(c0, col - d9);
                        c04  = M_max(c0, col - d8);
                        c05  = M_max(c0, col - d7);
                        c06  = M_max(c0, col - d6);
                        c07  = M_max(c0, col - d5);
                        c08  = M_max(c0, col - d4);
                        c09  = M_max(c0, col - d3);
                        c10  = M_max(c0, col - d2);
                        c11  = M_max(c0, col - d1);
                        // don't do c(N+1 / 2) = "col"  
                        c13 = M_min(cm, col + d1);
                        c14 = M_min(cm, col + d2);
                        c15 = M_min(cm, col + d3);
                        c16 = M_min(cm, col + d4);
                        c17 = M_min(cm, col + d5);
                        c18 = M_min(cm, col + d6);
                        c19 = M_min(cm, col + d7);
                        c20 = M_min(cm, col + d8);
                        c21 = M_min(cm, col + d9);
                        c22 = M_min(cm, col + d10);
                     // c23 = already set earlier

                        M_mapdata_attr(mapdata, row, col) |= MAPDATA_WATER;   // just set center point to water -- fill will take care of rest of lake
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match;
mismatch:       was_water = 0;    // make sure to end lake due to mismatch ht
match:          ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL23(h01)
            M_SL23(h02)
            M_SL23(h03)
            M_SL23(h04)
            M_SL23(h05)
            M_SL23(h06)
            M_SL23(h07)
            M_SL23(h08)
            M_SL23(h09)
            M_SL23(h10)
            M_SL23(h11)
            M_SL23(h12)
            M_SL23(h13)
            M_SL23(h14)
            M_SL23(h15)
            M_SL23(h16)
            M_SL23(h17)
            M_SL23(h18)
            M_SL23(h19)
            M_SL23(h20)
            M_SL23(h21)
            M_SL23(h22)
            M_SL23(h23)
        
        }          // column loop
    }              // row loop

    M_finish()  
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_map25() -- set water attribute
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// helper MACROs for this routine

#undef  M_HNN
#undef  M_HNX
#undef  M_CNX
#undef  M_DNN
#undef  M_CHNN
#define M_HNN()    h1313
#define M_HNX()    h1325
#define M_CNX()    c25
#define M_DNN()    d12
#define M_CHNN(v)  if (M_HNN() != v) goto mismatch; 


///// declaration MACROs

#define M_DH25(x)   int16_t x##01, x##02, x##03, x##04, x##05, x##06, x##07, x##08, x##09, \
                     x##10, x##11, x##12, x##13, x##14, x##15, x##16, x##17, x##18, x##19, \
                     x##20, x##21, x##22, x##23, x##24, x##25;

//         up through M_DDN(N-1 / 2)
#define M_DD25()    M_DDN(1) M_DDN(2) M_DDN(3) M_DDN(4) M_DDN(5) M_DDN(6) M_DDN(7) M_DDN(8) M_DDN(9) M_DDN(10) M_DDN(11) M_DDN(12)

//         row, col replaces r(N+1 / 2) and c(N+1 / 2) 
#define M_DR25()   int r01, r02, r03, r04, r05, r06, r07, r08, r09, r10, r11, r12, row, r14, r15, r16, r17, r18, r19, r20, r21, r22, r23, r24, r25; 
#define M_DC25()   int c01, c02, c03, c04, c05, c06, c07, c08, c09, c10, c11, c12, col, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25;


///// code-generating MACROs

#define M_C25(x)      M_CHNN(x##01) M_CHNN(x##02) M_CHNN(x##03) M_CHNN(x##04) M_CHNN(x##05) M_CHNN(x##06) M_CHNN(x##07) M_CHNN(x##08) M_CHNN(x##09) \
        M_CHNN(x##10) M_CHNN(x##11) M_CHNN(x##12) M_CHNN(x##13) M_CHNN(x##14) M_CHNN(x##15) M_CHNN(x##16) M_CHNN(x##17) M_CHNN(x##18) M_CHNN(x##19) \
        M_CHNN(x##20) M_CHNN(x##21) M_CHNN(x##22) M_CHNN(x##23) M_CHNN(x##24) M_CHNN(x##25)

#define M_W25(r)    M_mapdata_attr(mapdata, r, M_CNX()) |= MAPDATA_WATER;



#define M_SL25(x)   x##01=x##02; x##02=x##03; x##03=x##04; x##04=x##05; x##05=x##06; x##06=x##07; x##07=x##08; x##08=x##09; x##09=x##10;\
       x##10=x##11; x##11=x##12; x##12=x##13; x##13=x##14; x##14=x##15; x##15=x##16; x##16=x##17; x##17=x##18; x##18=x##19; x##19=x##20;\
       x##20=x##21; x##21=x##22; x##22=x##23; x##23=x##24; x##24=x##25;

#define M_SI25(x,r)                                  \
        x##01 = -11111;                              \
        x##02 = -11111;                              \
        x##03 = -11111;                              \
        x##04 = -11111;                              \
        x##05 = -11111;                              \
        x##06 = -11111;                              \
        x##07 = -11111;                              \
        x##08 = -11111;                              \
        x##09 = -11111;                              \
        x##10 = -11111;                              \
        x##11 = -11111;                              \
        x##12 = -11111;                              \
        x##13 = M_mapdata_ht(mapdata, r,  c0    );   \
        x##14 = M_mapdata_ht(mapdata, r,  c0 + 1);   \
        x##15 = M_mapdata_ht(mapdata, r,  c0 + 2);   \
        x##16 = M_mapdata_ht(mapdata, r,  c0 + 3);   \
        x##17 = M_mapdata_ht(mapdata, r,  c0 + 4);   \
        x##18 = M_mapdata_ht(mapdata, r,  c0 + 5);   \
        x##19 = M_mapdata_ht(mapdata, r,  c0 + 6);   \
        x##20 = M_mapdata_ht(mapdata, r,  c0 + 7);   \
        x##21 = M_mapdata_ht(mapdata, r,  c0 + 8);   \
        x##22 = M_mapdata_ht(mapdata, r,  c0 + 9);   \
        x##23 = M_mapdata_ht(mapdata, r,  c0 + 10);  \
        x##24 = M_mapdata_ht(mapdata, r,  c0 + 11);   
      //x##25  
      // don't do x##N   --  x##01 to x##(N-1 / 2) = -11111;      x#(N+1 / 2) thru X(N-1) set to c0 + 1 thru c0 + c(N-3 / 2)


#define M_SH25(x) h##x##25 = M_mapdata_ht(mapdata, r##x, M_CNX());




////////////////////////////////////////////////////


static void water_map25(mapdata_S& mapdata, thp_S& thp, int32_t stride, int32_t check, const mapparm_S& parm) try
{
    // standard declarations for water_mapNN() functions

    M_DD25()
    M_DR25()
    M_DC25()

    M_DH25(h01)      // declare n x n group hxxyy
    M_DH25(h02)
    M_DH25(h03)
    M_DH25(h04)
    M_DH25(h05)
    M_DH25(h06)
    M_DH25(h07)
    M_DH25(h08)
    M_DH25(h09)
    M_DH25(h10)
    M_DH25(h11)
    M_DH25(h12)
    M_DH25(h13)
    M_DH25(h14)
    M_DH25(h15)
    M_DH25(h16)
    M_DH25(h17)
    M_DH25(h18)
    M_DH25(h19)
    M_DH25(h20)
    M_DH25(h21)
    M_DH25(h22)
    M_DH25(h23)
    M_DH25(h24)
    M_DH25(h25)

    M_DV(25)
    
    /////////////////////////  
        
    M_start()

    for (row = r0; row <= rm; row += stride)    // outer loop -- one for each row -- with stride 
    {   
        r01  = M_max(r0, row - d12);
        r02  = M_max(r0, row - d11);
        r03  = M_max(r0, row - d10);
        r04  = M_max(r0, row - d9 );
        r05  = M_max(r0, row - d8 );  
        r06  = M_max(r0, row - d7 );
        r07  = M_max(r0, row - d6 );
        r08  = M_max(r0, row - d5 );
        r09  = M_max(r0, row - d4 ); 
        r10  = M_max(r0, row - d3 );
        r11  = M_max(r0, row - d2 );
        r12  = M_max(r0, row - d1 );
        ////// r13         
        r14 = M_min(rm, row + d1 );
        r15 = M_min(rm, row + d2 );
        r16 = M_min(rm, row + d3 );
        r17 = M_min(rm, row + d4 );
        r18 = M_min(rm, row + d5 );
        r19 = M_min(rm, row + d6 );
        r20 = M_min(rm, row + d7 );
        r21 = M_min(rm, row + d8 );
        r22 = M_min(rm, row + d9 );
        r23 = M_min(rm, row + d10);
        r24 = M_min(rm, row + d11);
        r25 = M_min(rm, row + d12);

        // set hx01 hx02 hx03 hx04 hx05 hx06 hx07 hx08 hx09 hx10 hx11 hx12 hx13 hx14 ..  (but not hxN) for column 0
        
        M_SI25(h01, r01)        
        M_SI25(h02, r02)        
        M_SI25(h03, r03)        
        M_SI25(h04, r04)        
        M_SI25(h05, r05) 
        M_SI25(h06, r06) 
        M_SI25(h07, r07) 
        M_SI25(h08, r08)
        M_SI25(h09, r09)         
        M_SI25(h10, r10)         
        M_SI25(h11, r11)
        M_SI25(h12, r12)
        M_SI25(h13, row)
        M_SI25(h14, r14)
        M_SI25(h15, r15)
        M_SI25(h16, r16)
        M_SI25(h17, r17)
        M_SI25(h18, r18)
        M_SI25(h19, r19)
        M_SI25(h20, r20)
        M_SI25(h21, r21)
        M_SI25(h22, r22)
        M_SI25(h23, r23)
        M_SI25(h24, r24)
        M_SI25(h25, r25)
        
        was_water = 0; 


        for (col=c0; col <= cm; col += stride)   // inner loop -- once for each column (with stride)
        {
            M_CNX() = M_min(cm, col + M_DNN());       // "col + d(N - 1 / 2)"

            // h0101,h0102 etc set at start of this line (before loop)

            M_SH25(01)
            M_SH25(02)
            M_SH25(03)
            M_SH25(04)
            M_SH25(05)
            M_SH25(06)
            M_SH25(07)
            M_SH25(08)
            M_SH25(09)
            M_SH25(10)
            M_SH25(11)
            M_SH25(12)
            M_HNX() = M_mapdata_ht(mapdata, row, M_CNX());
            M_SH25(14)
            M_SH25(15)
            M_SH25(16)
            M_SH25(17)
            M_SH25(18)
            M_SH25(19)
            M_SH25(20)
            M_SH25(21)
            M_SH25(22)
            M_SH25(23)
            M_SH25(24)
            M_SH25(25)
            

            if ((M_mapdata_attr(mapdata, row, col) & MAPDATA_VALID) && (h0909 != MAPDATA_VOID))  // dont do compares, if not a valid data point
            {
                M_C25(h13)  // do minimally-clipped row, first -- h(N+1 / 2)

                M_C25(h01) // compare MACROs go to mismatch label -- check all n x n points (some may be clipped)
                M_C25(h02)
                M_C25(h03)
                M_C25(h04)
                M_C25(h05)
                M_C25(h06)
                M_C25(h07)
                M_C25(h08)
                M_C25(h09)
                M_C25(h10)
                M_C25(h11)
                M_C25(h12)
                //    h13  already done above
                M_C25(h13)
                M_C25(h14)
                M_C25(h15)
                M_C25(h16)
                M_C25(h17)
                M_C25(h18)
                M_C25(h19)
                M_C25(h20)
                M_C25(h21)
                M_C25(h22)
                M_C25(h23)
                M_C25(h24)
                M_C25(h25)

              
                if M_CHT(M_HNN())     // exclude low-lying coastal areas and highter elevations, etc.
                {
                    if (was_water == 1)
                    {
                        M_W25(r01)     // just set leading col of pixels to water
                        M_W25(r02)
                        M_W25(r03)
                        M_W25(r04)
                        M_W25(r05)
                        M_W25(r06)
                        M_W25(r07)
                        M_W25(r08)
                        M_W25(r09)
                        M_W25(r10)
                        M_W25(r11)
                        M_W25(r12)
                        M_W25(row)
                        M_W25(r14)
                        M_W25(r15)
                        M_W25(r16)
                        M_W25(r17)
                        M_W25(r18)
                        M_W25(r19)
                        M_W25(r20)
                        M_W25(r21)
                        M_W25(r22)
                        M_W25(r23)
                        M_W25(r24)
                        M_W25(r25)
                     }
                     else
                     {   
                       M_NLE(0)                              // don't place lake too close to edge 
                       {
                        c01  = M_max(c0, col - d12);        //  compute clipped column numbers
                        c02  = M_max(c0, col - d11);
                        c03  = M_max(c0, col - d10);
                        c04  = M_max(c0, col - d9);
                        c05  = M_max(c0, col - d8);
                        c06  = M_max(c0, col - d7);
                        c07  = M_max(c0, col - d6);
                        c08  = M_max(c0, col - d5);
                        c09  = M_max(c0, col - d4);
                        c10  = M_max(c0, col - d3);
                        c11  = M_max(c0, col - d2);
                        c12  = M_max(c0, col - d1);
                        // don't do c(N+1 / 2) = "col"
                        c14 = M_min(cm, col + d1);
                        c15 = M_min(cm, col + d2);
                        c16 = M_min(cm, col + d3);
                        c17 = M_min(cm, col + d4);
                        c18 = M_min(cm, col + d5);
                        c19 = M_min(cm, col + d6);
                        c20 = M_min(cm, col + d7);
                        c21 = M_min(cm, col + d8);
                        c22 = M_min(cm, col + d9);
                        c23 = M_min(cm, col + d10);
                        c24 = M_min(cm, col + d11);
                     // c25 = already set earlier

                        M_mapdata_attr(mapdata, row, col) |= MAPDATA_WATER;   // just set center point to water -- fill will take care of rest of lake
                        new_ct++; 
                        was_water = 1;                     // indicate that we are in a lake now
                       }
                    }
                }
                else
                {
                    was_water = 0;     // end current lake if now in lowland area
                }

                goto match;
mismatch:       was_water = 0;    // make sure to end lake due to mismatch ht
match:          ; 
            }       // valid data

            // slide down data points for next inner loop pass

            M_SL25(h01)
            M_SL25(h02)
            M_SL25(h03)
            M_SL25(h04)
            M_SL25(h05)
            M_SL25(h06)
            M_SL25(h07)
            M_SL25(h08)
            M_SL25(h09)
            M_SL25(h10)
            M_SL25(h11)
            M_SL25(h12)
            M_SL25(h13)
            M_SL25(h14)
            M_SL25(h15)
            M_SL25(h16)
            M_SL25(h17)
            M_SL25(h18)
            M_SL25(h19)
            M_SL25(h20)
            M_SL25(h21)
            M_SL25(h22)
            M_SL25(h23)
            M_SL25(h24)
            M_SL25(h25)
        
        }          // column loop
    }              // row loop

    M_finish()  
}
M_endf





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       water_fill() -- fill existing lakes -- recursive method
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// Local type declarations

struct ct_S
{
    int64_t pushed; 
    int64_t popped;
    int64_t max_stack;
    int64_t root;
}; 




struct rc_S
{
    rc_T row;
    rc_T col; 
}; 



// Local MACRO definitions
//
//   p = pointer to mapdata_t structure
//
//   s   = stack<rc__t> for pushing
//   r,c = row, col
//   k   = ct__t structure for statistics  
//   h   = height of "main" point 


#define M_push_point(s, r, c, k)                             \
{                                                            \
    rc_S rc;                                                 \
    rc.row = (r);                                            \
    rc.col = (c);                                            \
    s.push(rc);                                              \
    (k.pushed)++;                                            \
    k.max_stack = M_max(k.max_stack, k.pushed - k.popped);   \
}


#define M_check_neighbor(p, s, r, c, h, k)                                                   \
{                                                                                            \
    attr_T *attr2_p = &M_mapdata_attr(mapdata, (r), (c));                                    \
                                                                                             \
    /* is neighbor not water, not already on stack (or done), and valid? */                  \
    if ( ((*attr2_p & (MAPDATA_WATER | MAPDATA_TEMP) ) == 0) && (*attr2_p & MAPDATA_VALID) ) \
    {                                                                                        \
       ht_T ht2   = M_mapdata_ht(mapdata, (r), (c));                                         \
                                                                                             \
       /* this neighbor at same height as "main" point ? */                                  \
       if (ht2 == (h))                                                                       \
       {                                                                                     \
          /* add this neighbor to the checking stack */                                      \
          M_push_point(s, r, c, k);                                                          \
                                                                                             \
          /* indicate that this pixel does not need pushing onto stack anymore */            \
          *attr2_p |= (MAPDATA_WATER | MAPDATA_TEMP);                                        \
       }                                                                                     \
    }                                                                                        \
}

////////////////////////////////////////////////////////////////////////////////////////////////////


static void water_fill(mapdata_S& mapdata, thp_S& thp, const mapparm_S&  parm) try
{
    rc_T     row, col, rowu, colu; 

    rc_T     r0     = thp.e_r0; 
    rc_T     rm     = thp.e_rm;
    rc_T     c0     = thp.e_c0;
    rc_T     cm     = thp.e_cm;
   
    ct_S ct; 
   

    std::stack<rc_S> st;
    
    ct.pushed    = 0;
    ct.popped    = 0; 
    ct.max_stack = 0;
    ct.root      = 0; 

    int32_t ct_show = 0; 
    
    M__(M_out(L"------------------- Water fill -- starting -- thread = %d") % thp.thread_n;)


    // pass loop -- split fill into two passes -- 1st pass fills only  in N-S direction, 2nd pass does all 8 directions 
    //                                            this seems to greatly reduce the max queue depth is water detect was done with a stride > 1 


    for (auto pass = 1; pass <= 2; pass++)
    {
        /////   outer loop to process each row

        for (row = r0; row <= rm; row++)
        {
            ///// inner loop to process each column

            for (col = c0; col <= cm; col++)
            {
                attr_T *attr_p = &M_mapdata_attr(mapdata, row, col);

                if ( (*attr_p & MAPDATA_WATER) && ((*attr_p & MAPDATA_TEMP) == 0) ) 
                {
                    ht_T ht1 = M_mapdata_ht(mapdata, row, col); 


                    ///// check 8 neighbor points to see if they need to be switched to water and then checked

                    if   (row > r0)                  M_check_neighbor(p, st, row - 1, col    , ht1, ct)   // North neighbor 
                    if   (row < rm)                  M_check_neighbor(p, st, row + 1, col    , ht1, ct)   // South neighbor 
                    if   (col > c0)                  M_check_neighbor(p, st, row    , col - 1, ht1, ct)   // West neighbor 
                    if   (col < cm)                  M_check_neighbor(p, st, row    , col + 1, ht1, ct)   // East neighbor 
                    if ( (row > r0) && (col > c0) )  M_check_neighbor(p, st, row - 1, col - 1, ht1, ct)   // Northwest neighbor 
                    if ( (row < rm) && (col > c0) )  M_check_neighbor(p, st, row + 1, col - 1, ht1, ct)   // Southwest neighbor 
                    if ( (row > r0) && (col < cm) )  M_check_neighbor(p, st, row - 1, col + 1, ht1, ct)   // Northeast neighbor 
                    if ( (row < rm) && (col < cm) )  M_check_neighbor(p, st, row + 1, col + 1, ht1, ct)   // Northeast neighbor 

                    /*    
                    if (ct.pushed % 100 == 0)
                    {
                    SHOW6("============> row=%d  col=%d  added=%lld  pushed=%lld  popped=%lld  max_stack=%lld", row, col, ct.added, ct.pushed, ct.popped, ct.max_stack)

                    ct_show++;   if (ct_show > 10000) exit(0); 
                    }
                    */

                    // indicate that this pixel is "done"-- neighbors have been checked already -- do not add to stack  
 
                    *attr_p |= MAPDATA_TEMP;  
                    ct.root++; 
                }

                // remove and check the adjacent neighbors of any points on the stack

                while (st.empty() == false)
                {
                    rc_S rc = st.top();
                    st.pop();
                    ct.popped++;

                    // points being unstacked are already marked water and "done" so its neighbors just need to be unconditionally checked

                    rowu = rc.row;
                    colu = rc.col;

                    M__(M_out(L"<======= popped item row=%d  col=%d") % rowu % colu;)  



                    int htu = M_mapdata_ht(mapdata, rowu, colu);

                    ///// check 2/8 neighbor points to see if then need to be switched to water and then be checked
                
                    if   (rowu > r0)                   M_check_neighbor(p, st, rowu - 1, colu    , htu, ct)   // North neighbor 
                    if   (rowu < rm)                   M_check_neighbor(p, st, rowu + 1, colu    , htu, ct)   // South neighbor 

                    if (pass > 1 )
                    {
                        if   (colu > c0)                   M_check_neighbor(p, st, rowu    , colu - 1, htu, ct)   // West neighbor 
                        if   (colu < cm)                   M_check_neighbor(p, st, rowu    , colu + 1, htu, ct)   // East neighbor 
                        if ( (rowu > r0) && (colu > c0) )  M_check_neighbor(p, st, rowu - 1, colu - 1, htu, ct)   // Northwest neighbor 
                        if ( (rowu < rm) && (colu > c0) )  M_check_neighbor(p, st, rowu + 1, colu - 1, htu, ct)   // Southwest neighbor 
                        if ( (rowu > r0) && (colu < cm) )  M_check_neighbor(p, st, rowu - 1, colu + 1, htu, ct)   // Northeast neighbor 
                        if ( (rowu < rm) && (colu < cm) )  M_check_neighbor(p, st, rowu + 1, colu + 1, htu, ct)   // Northeast neighbor 
                    }


                    /*
                    if (ct.pushed % 100 == 0)
                    {
                        M_dmsg("rowu=%d  colu=%d  added=%lld  pushed=%lld  popped=%lld  max_stack=%lld", rowu, colu, ct.added, ct.pushed, ct.popped, ct.max_stack)                      
                        ct_show++; 
                        if (ct_show > 10000) exit(0); 
                    }
                    */
                }    // loop to remove stack elements
             }    // inner loop
         }        // outer loop

         M__(M_out(L"Thread %d: Pass=%d -- push=%-8lld  pop=%-8lld  max_q=%-8lld  root=%d") % thp.thread_n % pass % ct.pushed % ct.popped % ct.max_stack % ct.root;)

         clear_temp_map(mapdata, r0, rm, c0, cm);           // clean out temprary after each pass, so passes are independent

    }        // pass loop
    

    // Set MAPDATA_OCEAN attribute flag, based on MAPDATA_WATER at height = 0
    // ----------------------------------------------------------------------

    int32_t ocean_ct {0};                           // count number of ocean points found
    int32_t water_ct {0};                           // count number of water points found

    for (row = r0; row <= rm; row++)
    {
        for (col = c0; col <= cm; col++)
        {
            attr_T *ao_p = &M_mapdata_attr(mapdata, row, col);
             
            if (*ao_p & MAPDATA_WATER)              // pixel flagged as water?
            {
                ht_T ho = M_mapdata_ht(mapdata, row, col);

                if (ho == 0)                       // elevation is 0? 
                {
                    *ao_p |= MAPDATA_OCEAN;         // indicate that this is an ocean pixel 
                    ocean_ct++; 
                }
                water_ct++;                     
            }
        }
    }

     
    // pass back number of points added total, last pass = 1, etc.
    --------------------------------------------------------------
       
    thp.r32   = 1;

    if (parm.thread.display)
        {M_out_lk(L"Thread %d: Water fill ended -- push=%-8lld  pop=%-8lld  max_q=%-8lld  root=%d -- water=%d  ocean=%d") % thp.thread_n % ct.pushed % ct.popped % ct.max_stack % ct.root % water_ct % ocean_ct;}

    return;
}
M_endf




//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""