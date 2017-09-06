// h_map_ramp.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////
////            ============
////            h_map_ramp.h -- color and color-ramp oriented functions  
////            ============
//// 
////
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_map_types.h"

//---------------------------------------------------------------------------------------------------------------------
//     Color ramp structure
//---------------------------------------------------------------------------------------------------------------------

typedef struct lrgb_S
{
    ht_T     l;              // lowest height for this color
    uint8_t  r;              // red value
    uint8_t  g;
    uint8_t  b;
} lrgb_T; 


class ramp_S
{
public:
    struct 
    {
        int32_t          display               {0    } ;   // 0 = no display, 1 = display parms, etc., 2 = display ramp entries
        bool             rescale_land          {false} ;   // do rescale operation on land  LRGB vector 
        bool             rescale_ocean         {false} ;   // do rescale operation on ocean LRGB vector 
        int32_t          rescale_ht_land       {0    } ;   // max land height in rescaled ramp (0 = no rescale)
        int32_t          rescale_dp_ocean      {0    } ;   // min ocean depth in rescaled ramp (0 = no rescale)
        int32_t          rescale_low_land      {0    } ;   // don't rescale land below this height 
        int32_t          rescale_shallow_ocean {0    } ;   // don't rescale ocean shallower than this depth 
        int32_t          rescale_base_land     {0    } ;   // add this to rescaled land heights
        real_T           rescale_step_land     {0.0  } ;   // minimum rescale step for land
        real_T           rescale_step_ocean    {0.0  } ;   // minimum rescale step for ocean
        real_T           rescale_gamma_land    {1.0  } ;   // land rescale gamma value
        real_T           rescale_gamma_ocean   {1.0  } ;   // ocean rescale gamma value
        
        real_T           pastel_factor_land    {0.0  } ;   // pastel factor for land colors   0.0 = no pastel
        real_T           pastel_factor_ocean   {0.0  } ;   // pastel factor for ocean colors  0.0 = no pastel

        real_T           shade_darkest_land    {0.25 } ;   // multiply ramp colors by this factor to adjust for darkest   slope shading -- land
        real_T           shade_lightest_land   {1.0  } ;   // multiply ramp colors by this factor to adjust for brightest slope shading -- land
        real_T           shade_midtone_land    {0.625} ;   // multiply ramp colors by this factor to adjust for midtone   slope shading -- land
        real_T           shade_darkest_ocean   {0.25 } ;   // multiply ramp colors by this factor to adjust for darkest   slope shading -- ocean
        real_T           shade_lightest_ocean  {1.0  } ;   // multiply ramp colors by this factor to adjust for brightest slope shading -- ocean
        real_T           shade_midtone_ocean   {0.625} ;   // multiply ramp colors by this factor to adjust for midtone   slope shading -- ocean
        real_T           dark_angle_land       {45.0 } ;   // slope angle (away from light) that gives darkest tone -- land
        real_T           bright_angle_land     {45.0 } ;   // slope angle (towards light) that gives brightest tone -- land
        real_T           dark_angle_ocean      {45.0 } ;   // slope angle (away from light) that gives darkest tone -- ocean
        real_T           bright_angle_ocean    {45.0 } ;   // slope angle (towards light) that gives brightest tone -- ocean

    }                    parm;                             // captured ramp parms

    bool                 complete              {false} ;   // ramp has been completely set up -- OK to use
    int32_t              n_land                {0    } ;   // number of rgb elements in land lrgb vector
    int32_t              n_ocean               {0    } ;   // number of rgb elements in ocean lrgb vector
    int32_t              l_min_land            {0    } ;   // lowest  "l" value in lrgb array (land)
    int32_t              l_max_land            {0    } ;   // highest "l" value in lrgb array (land)
    int32_t              l_min_ocean           {0    } ;   // lowest  "l" value in lrgb array (ocean)  -- should be -32767
    int32_t              l_max_ocean           {0    } ;   // highest "l" value in lrgb array (ocean)  -- should be 0
    int32_t              ix_lo_land            {0    } ;   // lowest height in land ramp_index
    int32_t              ix_lo_ocean           {0    } ;   // lowest height in ocean ramp_index
    int32_t              ix_hi_land            {0    } ;   // highest height in land ramp index
    int32_t              ix_hi_ocean           {0    } ;   // highest height in land ramp index
    int32_t              ix_land[22001]        {     } ;   // index for heights -12000 to 10000 meters (land)
    int32_t              ix_ocean[12001]       {     } ;   // index for heights -12000 to 0 meters (ocean)
    std::vector<lrgb_S>  lrgb_land             {     } ;   // vector of heights and colors for land (above and below sea level)
    std::vector<lrgb_S>  lrgb_ocean            {     } ;   // vector of heights and colors for ocean depths (optional)
};


struct rgb_color24_parm_S
{
public:
    bool                    display            { false               } ;       // true -- do map  debug display 
    bool                    shaded_relief      { false               } ;       // default is no slope processing (shaded relief)
    bool                    land_hypsometry    { true                } ;       // default is land hypsometry
    bool                    ocean_bathymetry   { false               } ;       // default is no ocean bathymetry 
};  


//---------------------------------------------------------------------------------------------------------------------
//     color and ramp functions
//---------------------------------------------------------------------------------------------------------------------


ramp_S *find_ramp(   int64_t   );      // locate ramp in repository with passed-in ID
ramp_S *alloc_ramp(  int64_t   );      // make sure ramp is allocated in repository for passed-in ID (passes back simple pointer address of ramp_S) 
void    free_ramp(   int64_t   );      // make sure ramp is not allocated in repository for passed-in ID (caller should get rid of everything in ramp_S first) 

rgb_S rgb_color24(       ht_T, attr_T, slope_T,                 const ramp_S&,   const rgb_color24_parm_S&,     const mapparm_S&);
void  rgbtriple_color24( ht_T, attr_T, slope_T,    RGBTRIPLE *, const ramp_S&,   const rgb_color24_parm_S&,     const mapparm_S&);   
int   setup_ramp(                                                     ramp_S&,                                  const mapparm_S&); 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////