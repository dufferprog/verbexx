// h_map_annotate.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ================
////     h_map_annotate.h
////     ================
//// 
//// 
////     contains definitions for elevation database
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_map_types.h"
                                                          

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                             
//     MACROs -- for use inside annotate.cpp
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

// MACRO to convert deg/min/sec to degrees
// ---------------------------------------

#define M_dms(d, m, s)  ( (real_T)(d) + (std::copysign(((real_T)(m)), ((real_T)(d))))/60.0 + (std::copysign(((real_T)(s)), ((real_T)(d))))/3600.0 )


// Note: For the following, need to definebefore using MACRO:
//
//            -  M_db_put(elev) to save away the completed elev_S item
//            -  elev_S elev { };  

 
// deg min sec -- no prominence arg
// --------------------------------

#define M_db_dms(n, z, yd,ym,ys, xd,xm,xs)                           \
{                                                                    \
  elev.h    = (int32_t)M_round((real_T)(z));                         \
  elev.lat  = M_dms((yd), (ym), (ys));                               \
  elev.lon  = M_dms((xd), (xm), (xs));                               \
  elev.name = n;                                                     \
  M_db_put(elev)                                                     \
}


// degrees only -- no prominence arg  
// ---------------------------------

#define M_db_deg(n, z, yd, xd)                                       \
{                                                                    \
  elev.h    = (int32_t)M_round((real_T)(z));                         \
  elev.lat  = (yd);                                                  \
  elev.lon  = (xd);                                                  \
  elev.name = n;                                                     \
  M_db_put(elev)                                                     \
}

 
// deg min sec -- includes prominence arg
// --------------------------------------

#define M_db_dmsp(n, z,p, yd,ym,ys, xd,xm,xs)                        \
{                                                                    \
  elev.h    = (int32_t)M_round((real_T)(z));                         \
  elev.prom = (int32_t)M_round((real_T)(p));                         \
  elev.lat  = M_dms((yd), (ym), (ys));                               \
  elev.lon  = M_dms((xd), (xm), (xs));                               \
  elev.name = n;                                                     \
  M_db_put(elev)                                                     \
}


// deg min sec -- elevation in feet -- no prominence arg
// -----------------------------------------------------

#define M_db_fdms(n, z, yd,ym,ys, xd,xm,xs)                          \
{                                                                    \
  elev_S elev {};                                                    \
  elev.h    = (int32_t)M_round((real_T)(z) * 0.304801);              \
  elev.lat  = M_dms((yd), (ym), (ys));                               \
  elev.lon  = M_dms((xd), (xm), (xs));                               \
  elev.name = n;                                                     \
  M_db_put(elev)                                                     \
}    


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// -------------------------------------
// one entry in elevation database array 
// -------------------------------------

struct elev_S
{
    int32_t        h     { 0 } ;   // elevation  
    int32_t        prom  {-1 } ;   // prominence  (-1 if not set)
    real_T         lat   {0.0} ;   // latitude in degrees
    real_T         lon   {0.0} ;   // longitude in degrees
    std::wstring   name  {   } ;   // name for spot elevation
};


// ----------------------------------
// external functions in annotate.cpp
// ----------------------------------

int annotate_map(          mapdata_S&, outimage_S&,                                            const mapparm_S&);
int annotate_spot_elev(    mapdata_S&, outimage_S&,                                            const mapparm_S&); 

////////////////////////////////////////////////////////////////////////////////////////////////////////////