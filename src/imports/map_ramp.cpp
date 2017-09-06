// map_ramp.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ============
////     map_ramp.cpp -- color and color ramp - oriented functions
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

#include "h_draw_canvas.h"

#include "h_map_types.h"
#include "h_map_global.h"
#include "h_map_parms.h"
#include "h_map_ramp.h"




// ==================================================================================================================
//                                       Static variables
// ==================================================================================================================

namespace static_N
{

// ramp_S repository

static std::map<int64_t, std::shared_ptr<ramp_S>> ramps { }; 

}






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
//||        find_ramp() -- find ramp(n) in repository (return nullptr, if not there)  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

ramp_S* find_ramp(int64_t ramp_id) try
{
    // make sure there is exactly one ramp in repository with caller's ID
    // ------------------------------------------------------------------

     auto ramp_ct = static_N::ramps.count(ramp_id); 

     if (ramp_ct <= 0)
     {
         M__(M_out(L"find_ramp() -- ramp(%d) not in repository") % ramp_id;) 
         return nullptr; 
     }
     else
     if (ramp_ct > 1)
     {
         M_out_emsg(L"find_ramp() -- invalid ramps.count(%d) = %d") % ramp_id % ramp_ct;  
         return nullptr; 
     }


     // one shared_ptr<ramp_S> found -- return ramp_S address as simple pointer
     // -----------------------------------------------------------------------

     M__(M_out(L"find_ramp() -- ramp(%d) at %p found in repository") % ramp_id % static_N::ramps.at(ramp_id).get();) 
     return static_N::ramps.at(ramp_id).get();
}
M_endf 




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        alloc_ramp() -- make sure ramp is allocated in repository for this ID  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

ramp_S* alloc_ramp(int64_t ramp_id) try
{
    // pass back any existing ramp_S in repository
    // -------------------------------------------

    ramp_S *p = find_ramp(ramp_id);

    if (p != nullptr)
    {
        M__(M_out(L"alloc_ramp() -- existing ramp(%d) in repository at %p being returned") % ramp_id % p;) 
        return p;
    }


    // need to allocate new (default initialized) ramp_S and add to the repository
    //----------------------------------------------------------------------------

    std::shared_ptr<ramp_S> sp(new ramp_S);     // this sp should go away when function returns
    static_N::ramps.emplace(ramp_id, sp);       // this sp should stay around until this ID is deallocated

    M__(M_out(L"alloc_ramp() -- new ramp(%d) was allocated at %p and added to repository") % ramp_id % sp.get();)


    // pass back simple address of newly-allocated ramp_S

    return sp.get();                                        
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        free_ramp() -- make sure no ramp is allocated in repository for this ID  
//|| 
//||                           (caller must have gotten rid of everything in the ramp_S before calling) 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void free_ramp(int64_t ramp_id) try
{
    // get rid of any existing ramp_S in repository
    // --------------------------------------------

    if (static_N::ramps.count(ramp_id) > 0)
    {
        M__(M_out(L"free_ramp() -- ramp(%d) being erased from repository") % ramp_id;)
        static_N::ramps.erase(ramp_id);   
    }
    else
    {
        M__(M_out(L"free_ramp() -- ramp(%d) not allocated in repository") % ramp_id;)
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
//||       setup_ramp() --   finish setup of passed-in raw color ramp 
//||
//||                    --   LRGB vectors should be completely filled in before this routine is called
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int setup_ramp(ramp_S& ramp, const mapparm_S& parm) try
{
    int      rescale_low_ix_land {0};          // index of last elevation value below lower adjust limit  
    int      rescale_hi_ix_ocean {0};          // index of last elevation value below lower adjust limit 
    int      rc = 0;                
   
     
    // save away number of LRGB entries in land and ocean vectors

    ramp.n_land  = ramp.lrgb_land.size(); 
    ramp.n_ocean = ramp.lrgb_ocean.size(); 


    // complain if land vector has fewer than 5 elements, of if ocean vector has 1 to 4 elements
    // -----------------------------------------------------------------------------------------

    if (ramp.n_land < 5)
    {
         M_out_emsg(L"setup ramp() -- Only %d entries in land LRGB vector (minimum allowed is 5)") % ramp.n_land; 
         rc = -1; 
    }  
   
    if ((ramp.n_ocean > 0) && (ramp.n_ocean < 5))
    {
         M_out_emsg(L"setup ramp() -- Only %d entries in ocean LRGB vector (minimum allowed is 5)") % ramp.n_ocean; 
         rc = -1;   
    }  

    if (rc != 0)
        return rc; 
                                      

    // complain if lowest heights in lRGB entries are not monotonically increasing 
    // ---------------------------------------------------------------------------

    for (auto i = 1; i < ramp.n_land; i++)
    {
        if (ramp.lrgb_land.at(i).l <= ramp.lrgb_land.at(i-1).l) 
        {
            M_out_emsg(L"setup ramp() -- Minimum height in land LRGB vector entry %d is not higher than prior one -- %d is not greater than %d)") % i % ramp.lrgb_land.at(i).l, ramp.lrgb_land.at(i-1); 
            rc = -1; 
        }      
    }

    for (auto i = 1; i < ramp.n_ocean; i++)
    {
        if (ramp.lrgb_ocean.at(i).l <= ramp.lrgb_ocean.at(i-1).l) 
        {
            M_out_emsg(L"setup ramp() -- Minimum height in ocean LRGB vector entry %d is not higher than prior one -- %d is not greater than %d)") % i % ramp.lrgb_ocean.at(i).l, ramp.lrgb_ocean.at(i-1); 
            rc = -1; 
        }      
    } 

    if (rc != 0)
        return rc;


    // Display input parms in ramp, if required
    // ----------------------------------------

    if (ramp.parm.display > 0)
    {
        M_out(L"Ramp    : display                = %s"     ) % M_bool_cstr(ramp.parm.display)                 ; 
        M_out(L"Ramp    : rescale_land           = %s"     ) % M_bool_cstr(ramp.parm.rescale_land)            ;    
        M_out(L"Ramp    : rescale_ocean          = %s"     ) % M_bool_cstr(ramp.parm.rescale_ocean)           ;
        M_out(L"Ramp    : rescale_ht_land        = %d"     ) % ramp.parm.rescale_ht_land                      ;
        M_out(L"Ramp    : rescale_dp_ocean       = %d"     ) % ramp.parm.rescale_dp_ocean                     ;
        M_out(L"Ramp    : rescale_low_land       = %d"     ) % ramp.parm.rescale_low_land                     ;
        M_out(L"Ramp    : rescale_shallow_ocean  = %d"     ) % ramp.parm.rescale_shallow_ocean                ;
        M_out(L"Ramp    : rescale_base_land      = %d"     ) % ramp.parm.rescale_base_land                    ;
        M_out(L"Ramp    : rescale_step_land      = %-12.6f") % ramp.parm.rescale_step_land                    ;
        M_out(L"Ramp    : rescale_step_ocean     = %-12.6f") % ramp.parm.rescale_step_ocean                   ;                                                                                                  
        M_out(L"Ramp    : rescale_gamma_land     = %-12.6f") % ramp.parm.rescale_gamma_land                   ;
        M_out(L"Ramp    : rescale_gamma_ocean    = %-12.6f") % ramp.parm.rescale_gamma_ocean                  ;
        M_out(L"Ramp    : pastel_factor_land     = %-12.6f") % ramp.parm.pastel_factor_land                   ;
        M_out(L"Ramp    : pastel_factor_ocean    = %-12.6f") % ramp.parm.pastel_factor_ocean                  ;  
        M_out(L"Ramp    : shade_darkest_land     = %-12.6f") % ramp.parm.shade_darkest_land                   ;
        M_out(L"Ramp    : shade_lightest_land    = %-12.6f") % ramp.parm.shade_lightest_land                  ;
        M_out(L"Ramp    : shade_midtone_land     = %-12.6f") % ramp.parm.shade_midtone_land                   ;
        M_out(L"Ramp    : dark_angle_land        = %-12.6f") % ramp.parm.dark_angle_land                      ;
        M_out(L"Ramp    : bright_angle_land      = %-12.6f") % ramp.parm.bright_angle_land                    ;
        M_out(L"Ramp    : shade_darkest_ocean    = %-12.6f") % ramp.parm.shade_darkest_ocean                  ;
        M_out(L"Ramp    : shade_lightest_ocean   = %-12.6f") % ramp.parm.shade_lightest_ocean                 ;
        M_out(L"Ramp    : shade_midtone_ocean    = %-12.6f") % ramp.parm.shade_midtone_ocean                  ;
        M_out(L"Ramp    : dark_angle_ocean       = %-12.6f") % ramp.parm.dark_angle_ocean                     ;
        M_out(L"Ramp    : bright_angle_ocean     = %-12.6f") % ramp.parm.bright_angle_ocean                   ;
    }
       

    // --------------------------------------------------------------------------------------------------
    //
    // process each lrgb_S entry in land LRGB vector 
    //
    //   - apply pastel factor, if required
    //   - find last lrgb vector index whose height is below the lower adjust limit for height rescaling (land vector only)
    //
    // --------------------------------------------------------------------------------------------------

    for (auto i = 0; i < ramp.n_land; i++)
    {
        // adjust land colors, if land pastel factor is non-0

        if (ramp.parm.pastel_factor_land != 0.0)
        {
            auto r = ramp.lrgb_land.at(i).r; 
            auto g = ramp.lrgb_land.at(i).g; 
            auto b = ramp.lrgb_land.at(i).b; 
            
            ramp.lrgb_land.at(i).r = (uint8_t)( (real_T)r + ramp.parm.pastel_factor_land * (real_T)(255 - r) );
            ramp.lrgb_land.at(i).g = (uint8_t)( (real_T)g + ramp.parm.pastel_factor_land * (real_T)(255 - g) );
            ramp.lrgb_land.at(i).b = (uint8_t)( (real_T)b + ramp.parm.pastel_factor_land * (real_T)(255 - b) );
        }
        

        // remember last land lrgb vector index that had height below the lower adjust limit for rescaling

        if (ramp.lrgb_land.at(i).l < ramp.parm.rescale_low_land)
            rescale_low_ix_land = i; 
    }

    if (rescale_low_ix_land + 1 < ramp.n_land)                                  // increment only if not at end already
        rescale_low_ix_land++;                                                  // want index with 1st value to be adjusted, not highest unadjusted one


    // --------------------------------------------------------------------------------------------------
    //
    // process each lrgb_S entry in ocean LRGB vector 
    //
    //   - apply pastel factor, if required
    //
    // --------------------------------------------------------------------------------------------------
       
    for (auto i = 0; i < ramp.n_ocean; i++)
    {
        if (ramp.parm.pastel_factor_ocean != 0.0)
        {
            // adjust ocean colors, if ocean pastel factor is non-0
    
            auto r = ramp.lrgb_ocean.at(i).r; 
            auto g = ramp.lrgb_ocean.at(i).g; 
            auto b = ramp.lrgb_ocean.at(i).b; 
            
            ramp.lrgb_ocean.at(i).r = (uint8_t)( (real_T)r + ramp.parm.pastel_factor_ocean * (real_T)(255 - r) );
            ramp.lrgb_ocean.at(i).g = (uint8_t)( (real_T)g + ramp.parm.pastel_factor_ocean * (real_T)(255 - g) );
            ramp.lrgb_ocean.at(i).b = (uint8_t)( (real_T)b + ramp.parm.pastel_factor_ocean * (real_T)(255 - b) );
        }


        // remember last ocean lrgb vector index that should be rescaled 
                
        if (ramp.lrgb_ocean.at(i).l <= ramp.parm.rescale_shallow_ocean)
            rescale_hi_ix_ocean = i;        
    }
     

    //--------------------------------------------------------------------------------------------------------------------------------------
    //
    // fill in rest of info in the ramp based on what was passed in
    //
    //--------------------------------------------------------------------------------------------------------------------------------------
        
    if (ramp.n_land > 0)
    {
        ramp.l_min_land   = ramp.lrgb_land.at( 0                ).l;
        ramp.l_max_land   = ramp.lrgb_land.at( ramp.n_land   - 1).l;        
    }
    else
    {
        ramp.l_min_land  = 0; 
        ramp.l_max_land  = 0;     
    }

    if (ramp.n_ocean > 0)
    {
        ramp.l_min_ocean  = ramp.lrgb_ocean.at(0                ).l;
        ramp.l_max_ocean  = ramp.lrgb_ocean.at(ramp.n_ocean - 1 ).l;
    }
    else
    {
        ramp.l_min_ocean  = 0; 
        ramp.l_max_ocean  = 0;     
    }

    ramp.ix_lo_land   = -12000;   // start at lowest height  -- !!!!!!!!!!!!! note: these must match dimensions of ramp.ix_land and .ix_ocean   !!!!!!!!!
    ramp.ix_hi_land   =  10000;   // stop at max height 
    ramp.ix_lo_ocean  = -12000;   // start at lowest depth
    ramp.ix_hi_ocean  =  0;       // stop at min depth 
          
    
    // save away (in local variables) original unrescaled heights/depths

    int32_t l_min_land_orig   = ramp.l_min_land;               // lowest unrescaled land height in input ramp
    int32_t l_max_land_orig   = ramp.l_max_land;               // highest unrescaled land height in input ramp
    int32_t l_min_ocean_orig  = ramp.l_min_ocean;              // shallowest unrescaled ocean depth in input ramp
    int32_t l_max_ocean_orig  = ramp.l_max_ocean;              // shallowest unrescaled ocean depth in input ramp


    // ----------------------------------------------------------------------------------------------------------------------------------------------------------
    // Rescale the heights in the land LRGB vector (if they are above rescale_low_land meters) by a linear/gamma factor so RGB table covers caller's height range
    //
    //
    //    two adjustments added together: 
    //
    //    - linear step adjustment, based on specified minimum step size
    //    - additional adjustment (gamma or linear), if any, required to get from min to max rescaled height range  
    //
    // ----------------------------------------------------------------------------------------------------------------------------------------------------------

    real_T  rescale_low_land     = (real_T)(ramp.parm.rescale_low_land);                                          // lowest value to be adjusted when rescaling 
    real_T  rescale_ht_land      = (real_T)(ramp.parm.rescale_ht_land);                                           // max rescaled height
    real_T  max_orig_ht_land     = (real_T)(ramp.l_max_land);                                                     // highest unrescaled height in input ramp
    real_T  rescale_base_land    = (real_T)(ramp.parm.rescale_base_land);                                         // add this to each rescaled value -- rescale_low_land should be 0, if rescale_base_land is non-zero, to avoid a "ledge" in the rescaled ramp    
       
    real_T  max_step_adj_land    = (real_T)(ramp.n_land - rescale_low_ix_land - 1) * ramp.parm.rescale_step_land; // linear min-step-based adjustment for last step = number of rescaled index steps being done * supplied rescale step value 
    
    if (ramp.parm.display >= 2)
    {
        M_out(L"rescale_low_ix_land = %d"                       ) % rescale_low_ix_land     ;
        M_out(L"rescale_low_land    = %12.5f"                   ) % rescale_low_land        ;
        M_out(L"rescale_ht_land     = %12.5f"                   ) % rescale_ht_land         ;
        M_out(L"max_orig_ht_land    = %12.5f"                   ) % max_orig_ht_land        ;
        M_out(L"rescale_base_land   = %12.5f"                   ) % rescale_base_land       ;
        M_out(L"max_step_adj_land   = %12.5f"                   ) % max_step_adj_land       ;
    }
    
    if ( ramp.parm.rescale_land                                                                                       // rescaling requested?
         && 
         (
          (rescale_ht_land  > 0.0)                                                                                    // valid rescale height supplied?
          &&
          (rescale_ht_land  > rescale_low_land)                                                                       // rescaled max height is above lower rescale limit?
          && 
          (max_orig_ht_land > rescale_low_land)                                                                       // some ramp heights are above minimum value to be rescaled?  (don't want to divide by 0, below)
         )
       )                           
    {
        // main rescale factor = (rescaled height range (minus rescale base) - max linear adjustment due to steps) / original unrescaled height range

        real_T factor = (rescale_ht_land - rescale_low_land - rescale_base_land - max_step_adj_land) / (max_orig_ht_land - rescale_low_land);    // will be invalid if max_step_adj_land  >  height range after rescaling
        if (factor < 0.0) factor = 0.0;                                                                                                          // make sure not negative
        M__(M_out(L"Factor (land)       = %12.5f") % factor;) 


        // loop to adjust each elevation value in land LRGB vector, that is above the minimum adjust value
        // -----------------------------------------------------------------------------------------------

        for (auto j = 0; j < ramp.n_land; j++)                                                                  // loop, looking at each land RGB table entry  (start at lowest height)
        {
            real_T lcalc = (real_T)((ramp.lrgb_land.at(j)).l);                                                  // j-th unadjusted level from ramp

            if (lcalc > rescale_low_land)                                                                       // adjust only if above lower rescale limit
            {  
                real_T linear_step_adj = (real_T)(j - rescale_low_ix_land) * ramp.parm.rescale_step_land;       // linear adjustment to be added to main gamma-influenced adjustment


                //  compute "gamma" correction factor 0.0 to 1.0 ??  (if rescale_gamma is not 1.0)
             
                real_T gamma_f = 1.0; 
             
                if ( 
                     (ramp.parm.rescale_gamma_land != 1.0)                                                      // leave gamma_f at 1.0, if rescale_gamma is 1.0 (linear rescale)
                     &&
                     (max_orig_ht_land > rescale_low_land) && (lcalc < max_orig_ht_land)                        // max_orig_ht_land > lcalc > rescale_low_land ??  -- gamma computation is valid
                   )                                                   
                {                                                                                            
                    real_T fract = (lcalc - rescale_low_land) / (max_orig_ht_land - rescale_low_land);          // percentage of way up from low to high unrescaled heights
                    
                    real_T pow_f = pow(fract, ramp.parm.rescale_gamma_land); 
                    gamma_f = pow_f / fract; 
             
                    M__(M_out(L"land -- index=%d  fract=%12.6f  pow_f=%12.6f  gamma_f=%12.6f") % j % fract % pow_f % gamma_f; )   
                }


                // rescaled height = low rescale limit +  main adjstment (if any) + linear step adjustment
             
                (ramp.lrgb_land.at(j)).l = (int32_t)( rescale_low_land + factor * gamma_f * (lcalc - rescale_low_land) + rescale_base_land + linear_step_adj );

                M__(M_out(L"rgb[%d].l = %d (adjusted) -- land") % j % ramp.lrgb_land.at(j).l;) 
            }
        }

        ramp.l_min_land = (ramp.lrgb_land.at(0              )).l;                                               // adjust l_min -- shouldn't need this 
        ramp.l_max_land = (ramp.lrgb_land.at(ramp.n_land - 1)).l;                                               // adjust l_max
    }
     

    // ------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Rescale the depths in ocean LRGB vector (if thay are below rescale_shallow_ocean meters) by a linear/gamma factor so RGB table covers caller's depth range
    //
    //
    //    two adjustments added together: 
    //
    //    - linear step adjustment, based on specified minimum step size (should be negative)
    //    - additional adjustment (gamma or linear), if any, required to get from max to min rescaled height range  (should be negative)
    //
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------
  
    real_T  rescale_shallow_ocean   = (real_T)(ramp.parm.rescale_shallow_ocean);                                      // shallowest depth to be adjusted when rescaling 
    real_T  rescale_dp_ocean        = (real_T)(ramp.parm.rescale_dp_ocean);                                           // max rescaled ocean depth
    real_T  max_orig_dp_ocean       = (real_T)(ramp.l_min_ocean);                                                     // deepest unrescaled depth in input ramp        
    real_T  max_step_adj_ocean      = (real_T)(rescale_hi_ix_ocean) * ramp.parm.rescale_step_ocean;                   // linear min-step-based adjustment for last step = number of rescaled index steps being done * supplied rescale step value 
    
    M__(M_out(L"rescale_hi_ix_ocean   = %d"                       ) % rescale_hi_ix_ocean     ;)
    M__(M_out(L"rescale_shallow_ocean = %12.5f"                   ) % rescale_shallow_ocean   ;)
    M__(M_out(L"rescale_dp_ocean      = %12.5f"                   ) % rescale_dp_ocean        ;)
    M__(M_out(L"max_orig_dp_ocean     = %12.5f"                   ) % max_orig_dp_ocean       ;)
    M__(M_out(L"max_step_adj_ocean    = %12.5f"                   ) % max_step_adj_ocean      ;)
 
    
    if ( ramp.parm.rescale_ocean                                                                                      // rescaling requested?
         && 
         (
          (rescale_dp_ocean  < 0.0)                                                                                   // valid rescale depth supplied?
          &&                                             
          (rescale_dp_ocean  < rescale_shallow_ocean)                                                                 // rescaled max depth is below shallow rescale limit?
          && 
          (max_orig_dp_ocean < rescale_shallow_ocean)                                                                 // some ramp depths are below minimum value to be rescaled?  (don't want to divide by 0, below)
         )
       )                           
    {
        // main rescale factor = (depth range after rescaling - max linear adjustment due to steps) / original unrescaled depth range

        real_T factor = (rescale_shallow_ocean - rescale_dp_ocean - max_step_adj_ocean) / (rescale_shallow_ocean - max_orig_dp_ocean);    // will be invalid if max_step_adj_land  >  height range after rescaling
        if (factor < 0.0) factor = 0.0;                                                                                                   // make sure not negative
        M__(M_out(L"Factor (ocean)      = %12.5f") % factor;) 


        // loop to adjust each depth value in ocean LRGB vector, that is below the shallow adjust limit
        // --------------------------------------------------------------------------------------------

        for (auto j = 0; j < ramp.n_ocean; j++)                                                                       // loop, looking at each ocean RGB table entry (deepest first)
        {
            real_T lcalc = (real_T)((ramp.lrgb_ocean.at(j)).l);                                                       // j-th unadjusted level from ramp

            if (lcalc < rescale_shallow_ocean)                                                                        // adjust only if below shallow limit for rescale
            {
                real_T linear_step_adj = (real_T)(j - rescale_hi_ix_ocean) * ramp.parm.rescale_step_ocean;            // linear adjustment to be added to main gamma-influenced adjustment (negative number)


                //  compute "gamma" correction factor 0.0 to 1.0

                real_T gamma_f = 1.0; 

                if ( 
                     (ramp.parm.rescale_gamma_ocean != 1.0)                                                           // leave gamma_f at 1.0, if rescale_gamma is 1.0 (linear rescale)
                     &&
                     (max_orig_dp_ocean < rescale_shallow_ocean) && (lcalc > max_orig_dp_ocean)                       // max_orig_dp_ocean < lcalc < rescale_shallow_ocean ??  -- gamma computation is valid
                   ) 
                {
                    real_T fract = (lcalc - rescale_shallow_ocean) / (max_orig_dp_ocean - rescale_shallow_ocean);     // percentage of way up from low to high unrescaled heights  (should be negative/negative = positive fraction 0.0 to 1.0)
                
                    real_T pow_f = pow(fract, ramp.parm.rescale_gamma_ocean); 
                    gamma_f = pow_f / fract; 
               
                    M__(M_out(L"ocean -- index=%d  fract=%12.6f  pow_f=%12.6f  gamma_f=%12.6f") % j % fract % pow_f % gamma_f; )   
                }
                   

                // rescaled depth = shallow rescale limit + main adjstment (if any) + linear step adjustment (both of these terms should be negative)

                (ramp.lrgb_ocean.at(j)).l = (int32_t)( rescale_shallow_ocean + factor * gamma_f * (lcalc - rescale_shallow_ocean) + linear_step_adj );          

                M__(M_out(L"rgb[%d].l = %d (adjusted) -- ocean") % j % ramp.lrgb_ocean.at(j).l;) 
            }
        }

        ramp.l_min_ocean = (ramp.lrgb_ocean.at(0               )).l;                                                  // adjust l_min -- shouldn't need this 
        ramp.l_max_ocean = (ramp.lrgb_ocean.at(ramp.n_ocean - 1)).l;                                                  // adjust l_max
    }


    // ------------------------------------------------------------------------------------------------------------------------------------
    // if requested, display show adjusted color ramp heights/depths and colors (pastel adjustment and rescale height/depth have been done)
    // ------------------------------------------------------------------------------------------------------------------------------------

    
    if (ramp.parm.display >= 2)
    {
        for (auto i = 0; i < ramp.n_land; i++)
        {
            M_out( L"Color ramp (land): [%3d] : h=%6d  r=%3d g=%3d b=%3d")
                 %  i
                 %  ramp.lrgb_land.at(i).l
                 %  ramp.lrgb_land.at(i).r
                 %  ramp.lrgb_land.at(i).g
                 %  ramp.lrgb_land.at(i).b
                 ;
        }

        for (auto i = 0; i < ramp.n_ocean; i++)
        {
            M_out( L"Color ramp (ocean): [%3d] : h=%6d  r=%3d g=%3d b=%3d")
                 %  i
                 %  ramp.lrgb_ocean.at(i).l
                 %  ramp.lrgb_ocean.at(i).r
                 %  ramp.lrgb_ocean.at(i).g
                 %  ramp.lrgb_ocean.at(i).b
                 ;
        }
    }


    // ----------------------------------------
    // Loop to set land RGB array index entries
    // ----------------------------------------
        
    auto j = 0;                                               // index for land and ocean RGB arrays -- incremented as needed

    for (auto i = 0; i < ramp.ix_hi_land - ramp.ix_lo_land + 1; i++) // i = 0 to hi-lo ... heights from ix_lo to ix_hi
    {
        if      (i + ramp.ix_lo_land <= ramp.l_min_land)      // before bottom of RGB heights? 
        {
            ramp.ix_land[i] = 0;                              // If so, use first value
        }
        else if (i + ramp.ix_lo_land >= ramp.l_max_land)      // past top of RGB heights?
        {
            ramp.ix_land[i] = (ramp.n_land) - 1;              // If so, use last value 
        }
        else                                                  // highest RGB element not seen yet
        {                
            while ( ((ramp.lrgb_land.at(j)).l <= (i + ramp.ix_lo_land)) && (j < ramp.n_land) ) // Advance RGB index, until one is found that is not too low
            {
                j++;                                          // advance to next L_RGB array element
                M__(M_out(L"adjust j: i=%d j=%d ix_lo=%d lrgb[j].l=%d") % i % j % ramp.ix_lo_land % (ramp.lrgb_land.at(j)).l; )  
            }

            if (j > 0)                                        // not at start of LRGB array? 
                ramp.ix_land[i] = j - 1;                      // use prior element (not too hi)
            else
                ramp.ix_land[i] = 0;                          // use first LRGB element
        }

    }
  
    
    // -----------------------------------------
    // Loop to set ocean RGB array index entries
    // -----------------------------------------
        
    j = 0;                                                    // reset before processing ocean index

    for (auto i = 0; i < ramp.ix_hi_ocean - ramp.ix_lo_ocean + 1; i++) // i = 0 to hi-lo ... heights from ix_lo to ix_hi
    {
        if      (i + ramp.ix_lo_ocean <= ramp.l_min_ocean)    // before bottom of RGB heights? 
        {
            ramp.ix_ocean[i] = 0;                             // If so, use first value
        }
        else if (i + ramp.ix_lo_ocean >= ramp.l_max_ocean)    // past top of RGB heights?
        {
            ramp.ix_ocean[i] = (ramp.n_ocean) - 1;            // If so, use last value 
        }
        else                                                  // highest RGB element not seen yet
        {                
            while ( ((ramp.lrgb_ocean.at(j)).l <= (i + ramp.ix_lo_ocean)) && (j < ramp.n_ocean) ) // Advance RGB index, until one is found that is not too low
            {
                j++;                                          // advance to next L_RGB array element
                M__(M_out(L"adjust j: i=%d j=%d ix_lo=%d lrgb[j].l=%d") % i % j % ramp.ix_lo_ocean % (ramp.lrgb_ocean.at(j)).l; )  
            }

            if (j > 0)                                        // not at start of LRGB array? 
                ramp.ix_ocean[i] = j - 1;                     // use prior element (not too hi)
            else
                ramp.ix_ocean[i] = 0;                         // use first LRGB element
        }

    }

    //-----------------------------------------------
    // display rescaling info (if rescaling was done)
    // ----------------------------------------------

    if (ramp.parm.rescale_land)
        M_out(L"Ramp    : number of land colors  = %-4d   maximum land height value (original/adjusted)  = %d/%d") % ramp.n_land  % l_max_land_orig  % ramp.l_max_land ; 
    if (ramp.parm.rescale_ocean)
        M_out(L"Ramp    : number of ocean colors = %-4d   maximum ocean depth value (original/adjusted) = %d/%d") % ramp.n_ocean % l_min_ocean_orig % ramp.l_min_ocean;

    M_out(L"Ramp    : number of land/ocean colors = %d/%d     min/max land height value = %d/%d     min/max ocean depth = %d/%d (after rescale)") % ramp.n_land % ramp.n_ocean % ramp.l_min_land % ramp.l_max_land % ramp.l_min_ocean % ramp.l_max_ocean;


    // dump out every 100th land and ocean ramp index entry for debugging, if required

    if (ramp.parm.display >= 2)
    {
        M_out(L"Ramp    : number of land/ocean colors = %d/%d     min/max land height value = %d/%d     min/max ocean depth = %d/%d (after rescale)") % ramp.n_land % ramp.n_ocean % ramp.l_min_land % ramp.l_max_land % ramp.l_min_ocean % ramp.l_max_ocean;

        for (int i = 0; i < ramp.ix_hi_land - ramp.ix_lo_land + 1; i += 100)
        {
            M_out(L"Color ramp: ix_land[%d] = %d") % (i + ramp.ix_lo_land) % ramp.ix_land[i];
        }
        for (int i = 0; i < ramp.ix_hi_ocean - ramp.ix_lo_ocean + 1; i += 100)
        {
            M_out(L"Color ramp: ix_ocean[%d] = %d") % (i + ramp.ix_lo_ocean) % ramp.ix_ocean[i];
        }
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
//||       rgbtriple_color24() -- put r, g, b bytes for a height/slope/attribute value into single RGBTRIPLE output area
//||       ===================
//||
//||                      looks up color corresponding to passed-in height+attribute in passed-in ramp structure  
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void rgbtriple_color24(ht_T h, attr_T a, slope_T s, RGBTRIPLE *p, const ramp_S& ramp, const rgb_color24_parm_S& r24parm, const mapparm_S& parm) try
{
    rgb_S rgb = rgb_color24(h, a, s, ramp, r24parm, parm);

    M_PUT_RGBT(p, rgb.r, rgb.g, rgb.b)
    return; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       rgb_color24() -- return rgb_T structure for this height, slope, and attribute
//||       =============
//||
//||                      looks up color corresponding to passed-in height+attribute in passed-in ramp structure  
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

rgb_S rgb_color24(ht_T h, attr_T a, slope_T s, const ramp_S& ramp, const rgb_color24_parm_S& r24parm, const mapparm_S& parm) try
{
    uint8_t   r, g, b;
   

    //
    //     special colors -- priority:    1) OFF         -- off map 
    //                                    2) GRATICULE   -- priority over void, water, invalid
    //                                    3) not VALID   -- priority over water & void (should not be on together) 
    //                                    4) VOID        -- priority over WATER (water attr could be on, though)
    //                                    5) WATER/OCEAN -- priority over regular height coloring 
    //                                    6)             -- regular data point -- colored by height 
    //
    //
    
    if (a & MAPDATA_GRATICULE) 
    {
        r = parm.color.graticule.r; 
        g = parm.color.graticule.g; 
        b = parm.color.graticule.b; 
    }
    else if (a & MAPDATA_OFF)
    {
        r = parm.color.outside_raster.r; 
        g = parm.color.outside_raster.g; 
        b = parm.color.outside_raster.b; 
    }
    else if (!(a & MAPDATA_VALID)) 
    {
        r = parm.color.outside_dem.r; 
        g = parm.color.outside_dem.g;  
        b = parm.color.outside_dem.b;   
    }
    else if (h == MAPDATA_VOID) 
    {
        r = parm.color.dem_void.r; 
        g = parm.color.dem_void.g;  
        b = parm.color.dem_void.b; 
    }
    else if ( (a & MAPDATA_WATER) || (a & MAPDATA_OCEAN) )  // lake = WATER only, ocean = OCEAN or OCEAN+WATER 
    {
        if (a & MAPDATA_OCEAN)
        {
            if ( (ramp.n_ocean == 0) || (!(r24parm.ocean_bathymetry)) ) 
            {
                // no ocean hypsometry or ocean ramp data -- just use default ocean color

                r = parm.color.ocean.r; 
                g = parm.color.ocean.g; 
                b = parm.color.ocean.b; 
            }
            else   // OK to do ocean hypsometry 
            {
                 // compute color for ocean point -- get basic r/g/b value from the color ramp as indexed by passed-in height 
                 // ----------------------------------------------------------------------------------------------------------

                 M__(M_out(L"rgb_color24() -- n = %d   l_max = %d   ix_lo = %d    ix_hi = %d") % ramp.n_ocean % ramp.l_max_ocean % ramp.ix_lo_ocean % ramp.ix_hi_ocean;)


                 int ix1 = h - ramp.ix_lo_ocean; 

                 if (ix1 < 0) 
                    ix1 = 0;                                                    // make sure we aren't below ramp min ht
                 else if (ix1 > ramp.ix_hi_ocean - ramp.ix_lo_ocean)
                    ix1 = ramp.ix_hi_ocean - ramp.ix_lo_ocean - 1;              // make sure we aren't over ramp max ht

                 int ix2 = ramp.ix_ocean[ix1];

                 M__(M_out(L"ocean rgb_color24(): h=%d ix1=%d ix2=%d") % h % ix1 % ix2;)

                 r = ramp.lrgb_ocean.at(ix2).r; 
                 g = ramp.lrgb_ocean.at(ix2).g; 
                 b = ramp.lrgb_ocean.at(ix2).b; 

                 M__(M_out(L"ocean rgb_color24(): r=%d g=%d b=%d") % r % g % b;) 

            }


            // adjust color for slope, if required -- ocean shaded relief processing 
            // ---------------------------------------------------------------------
            
            if (r24parm.shaded_relief == true)
            {
                real_T dark_factor;
           
                if ( (h != MAPDATA_VOID) && (a & MAPDATA_VALID) && (!(a & MAPDATA_OFF)) )
                {
                    if (s >= 0)
                    {
                        real_T pos_factor = (ramp.parm.shade_lightest_ocean - ramp.parm.shade_midtone_ocean) / ramp.parm.bright_angle_ocean;
                        dark_factor = M_lim(ramp.parm.shade_darkest_ocean, ramp.parm.shade_midtone_ocean + (real_T)s * pos_factor, ramp.parm.shade_lightest_ocean); 
                    }
                    else
                    {
                        real_T neg_factor = (ramp.parm.shade_midtone_ocean - ramp.parm.shade_darkest_ocean) / ramp.parm.dark_angle_ocean;
                        dark_factor = M_lim(ramp.parm.shade_darkest_ocean, ramp.parm.shade_midtone_ocean + (real_T)s * neg_factor, ramp.parm.shade_lightest_ocean);   
                    }
                }
                else
                {
                    dark_factor = ramp.parm.shade_lightest_ocean;                
                }
                        
                r = (uint8_t)(dark_factor * (real_T)r); 
                g = (uint8_t)(dark_factor * (real_T)g); 
                b = (uint8_t)(dark_factor * (real_T)b);      
            }            
        }
        else    // not ocean -- lake or river -- use water color only
        {
            r = parm.color.water.r; 
            g = parm.color.water.g; 
            b = parm.color.water.b; 
        }
    }
    else  // non-ocean point -- land 
    {
        if ( (ramp.n_land == 0) || (!(r24parm.land_hypsometry)) ) 
        {
            // no land hyposometry or land ramp data -- just use default land color

            r = parm.color.land.r; 
            g = parm.color.land.g; 
            b = parm.color.land.b; 
            //r = 255; 
            //g = 255; 
            //b = 255;  
        }
        else   // OK to do land hypsometry
        {    
            // compute color for normal point -- get basic r/g/b value from the color ramp as indexed by passed-in height 
            // ----------------------------------------------------------------------------------------------------------
          
            M__(M_out(L"rgb_color24() -- n = %d   l_max = %d   ix_lo = %d    ix_hi = %d") % ramp.n_land % ramp.l_max_land % ramp.ix_lo_land % ramp.ix_hi_land;)
          
          
            int ix1 = h - ramp.ix_lo_land; 
          
            if (ix1 < 0) 
                ix1 = 0;                                                    // make sure we aren't below ramp min ht
            else if (ix1 > ramp.ix_hi_land - ramp.ix_lo_land)
                ix1 = ramp.ix_hi_land - ramp.ix_lo_land - 1;                // make sure we aren't over ramp max ht
          
            int ix2 = ramp.ix_land[ix1];
          
            M__(M_out(L"rgb_color24(): h=%d ix1=%d ix2=%d") % h % ix1 % ix2;)
          
            r = ramp.lrgb_land.at(ix2).r; 
            g = ramp.lrgb_land.at(ix2).g; 
            b = ramp.lrgb_land.at(ix2).b; 
          
            M__(M_out(L"rgb_color24(): r=%d g=%d b=%d") % r % g % b;)
        }
              
          
        // adjust color for slope, if required -- land shaded relief processing 
        // --------------------------------------------------------------------
        
        if (r24parm.shaded_relief == true)
        {
            real_T dark_factor;
       
            if ( (h != MAPDATA_VOID) && (a & MAPDATA_VALID) && (!(a & MAPDATA_OFF)) )
            {
                if (s >= 0)
                {
                    real_T pos_factor = (ramp.parm.shade_lightest_land - ramp.parm.shade_midtone_land) / ramp.parm.bright_angle_land;
                    dark_factor = M_lim(ramp.parm.shade_darkest_land, ramp.parm.shade_midtone_land + (real_T)s * pos_factor, ramp.parm.shade_lightest_land); 
                }
                else
                {
                    real_T neg_factor = (ramp.parm.shade_midtone_land - ramp.parm.shade_darkest_land) / ramp.parm.dark_angle_land;
                    dark_factor = M_lim(ramp.parm.shade_darkest_land, ramp.parm.shade_midtone_land + (real_T)s * neg_factor, ramp.parm.shade_lightest_land);   
                }
            }
            else
            {
                dark_factor = ramp.parm.shade_lightest_land;                
            }
                    
            r = (uint8_t)(dark_factor * (real_T)r); 
            g = (uint8_t)(dark_factor * (real_T)g); 
            b = (uint8_t)(dark_factor * (real_T)b);      
        }        
    }


    // construct rgb_T value and return it

    M__(M_out(L"r = %d") % (int)r;)
    M__(M_out(L"g = %d") % (int)g;)
    M__(M_out(L"b = %d") % (int)b;)

    rgb_S rgb {r, g, b}; 

    return rgb; 
}
M_endf



//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""