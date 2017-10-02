// h_core_util.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     h_core_util.h
////     =============
//// 
//// 
////     contains utility items and functions in    core_util.cpp    -and-    core_msdn.cpp    -and-    core_file.cpp
//// 
////    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h__types.h"
                               

//---------------------------------------------------------------------------------------------------------------------
//     General use MACRO items
//---------------------------------------------------------------------------------------------------------------------


#define M_elements(x) ( (sizeof x)/(sizeof x[0]) )


// ----------------
// Binary constants
// ----------------

#define M_1K             1024 
#define M_1M          1048576  
#define M_1G       1073741824 
#define M_2G       2147483648ULL
#define M_4G       4294967296ULL
#define M_8G       8589934592ULL
#define M_16G     17179869184ULL

namespace const_N
{
constexpr uint32_t  n_1k       {         1024    };
constexpr uint32_t  n_1m       {      1048576    };
constexpr uint32_t  n_1g       {   1073741824    };
constexpr uint32_t  n_2g       {   2147483648U   };
constexpr uint64_t  n_4g       {   4294967296ULL };
constexpr uint64_t  n_8g       {   8589934592ULL };
constexpr uint64_t  n_16g      {  17179869184ULL }; 
}    

#define M_meg(x)  ((double)(x) / (double)M_1M)
#define M_gig(x)  ((double)(x) / (double)M_1G)


// ---------------------
// Numeric limits MACROs
// ---------------------

#define M_int8_max    ( std::numeric_limits<int8_t    >::max() )
#define M_int16_max   ( std::numeric_limits<int16_t   >::max() )
#define M_int32_max   ( std::numeric_limits<int32_t   >::max() )
#define M_int64_max   ( std::numeric_limits<int64_t   >::max() )
#define M_uint8_max   ( std::numeric_limits<uint8_t   >::max() )
#define M_uint16_max  ( std::numeric_limits<uint16_t  >::max() )
#define M_uint32_max  ( std::numeric_limits<uint32_t  >::max() )
#define M_uint64_max  ( std::numeric_limits<uint64_t  >::max() )
#define M_float32_max ( std::numeric_limits<float32_T >::max() )
#define M_float64_max ( std::numeric_limits<float64_T >::max() )
#define M_real_max    ( std::numeric_limits<real_T    >::max() )

#define M_int8_min    ( std::numeric_limits<int8_t    >::min() )
#define M_int16_min   ( std::numeric_limits<int16_t   >::min() )
#define M_int32_min   ( std::numeric_limits<int32_t   >::min() )
#define M_int64_min   ( std::numeric_limits<int64_t   >::min() )
#define M_uint8_min   ( std::numeric_limits<uint8_t   >::min() )
#define M_uint16_min  ( std::numeric_limits<uint16_t  >::min() )
#define M_uint32_min  ( std::numeric_limits<uint32_t  >::min() )
#define M_uint64_min  ( std::numeric_limits<uint64_t  >::min() )
#define M_float32_min ( std::numeric_limits<float32_T >::min() )         // closest to zero
#define M_float64_min ( std::numeric_limits<float64_T >::min() )         // closest to zero
#define M_real_min    ( std::numeric_limits<real_T    >::min() )         // closest to zero 

namespace const_N
{
constexpr int8_t    int8_max    { std::numeric_limits<int8_t    >::max() };
constexpr int16_t   int16_max   { std::numeric_limits<int16_t   >::max() };
constexpr int32_t   int32_max   { std::numeric_limits<int32_t   >::max() };
constexpr int64_t   int64_max   { std::numeric_limits<int64_t   >::max() };
constexpr uint8_t   uint8_max   { std::numeric_limits<uint8_t   >::max() };
constexpr uint16_t  uint16_max  { std::numeric_limits<uint16_t  >::max() };
constexpr uint32_t  uint32_max  { std::numeric_limits<uint32_t  >::max() };
constexpr uint64_t  uint64_max  { std::numeric_limits<uint64_t  >::max() };
constexpr float32_T float32_max { std::numeric_limits<float32_T >::max() };
constexpr float64_T float64_max { std::numeric_limits<float64_T >::max() };
constexpr real_T    real_max    { std::numeric_limits<real_T    >::max() };
 
constexpr int8_t    int8_min    { std::numeric_limits<int8_t    >::min() };
constexpr int16_t   int16_min   { std::numeric_limits<int16_t   >::min() };
constexpr int32_t   int32_min   { std::numeric_limits<int32_t   >::min() };
constexpr int64_t   int64_min   { std::numeric_limits<int64_t   >::min() };
constexpr uint8_t   uint8_min   { std::numeric_limits<uint8_t   >::min() };
constexpr uint16_t  uint16_min  { std::numeric_limits<uint16_t  >::min() };
constexpr uint32_t  uint32_min  { std::numeric_limits<uint32_t  >::min() };
constexpr uint64_t  uint64_min  { std::numeric_limits<uint64_t  >::min() };
constexpr float32_T float32_min { std::numeric_limits<float32_T >::min() };        // closest to zero
constexpr float64_T float64_min { std::numeric_limits<float64_T >::min() };        // closest to zero
constexpr real_T    real_min    { std::numeric_limits<real_T    >::min() };        // closest to zero                            
}    


// ---------------------
// UTF-16 oriented items
// ---------------------

namespace const_N
{                                                                        
constexpr uint32_t utf16_max             { 0x0010FFFFU };                             // maximum UTF-32 code point allowed for UTF-16 encoding  
constexpr uint32_t utf16_plane0_max      { 0x0000FFFFU };                             // maximum UTF-32 code point in BMP for UTF-16 (really UCS-2) 
constexpr uint32_t utf16_plane1_base     { 0x00010000U };                             // Subtract this from value from char32_t before extracting hi and low 10 bit fields
constexpr int      utf16_shift10         { 10          };                             // 10-bit shift for converting to/from char32_t and wchar_t 
constexpr uint32_t utf16_mask_lo10       { 0x000003FFU };                             // Apply this mask to char32_t to extract lo 10 bits
constexpr uint32_t utf16_mask_hi10       { 0x000FFC00U };                             // Apply this mask to char32_t to extract hi 10 bits
constexpr uint32_t utf16_surrogate_lo    { 0x0000D800U };                             // Lowest code point for surrogate range
constexpr uint32_t utf16_surrogate_hi    { 0x0000DFFFU };                             // Highest code point for surrogate range
constexpr uint32_t utf16_surrogate_mask  { 0x0000DC00U };                             // Mask for isolating UTF-16 surrogate range (D800 - DFFF)
constexpr uint32_t utf16_leading_base    { 0x0000D800U };                             // starting code point for leading surrogates (D800 - DBFF)
constexpr uint32_t utf16_trailing_base   { 0x0000DC00U };                             // starting code point for trailing surrogates (DC00 - DFFF)
}

#define M_is_lead_surrogate( wch) ( (((uint32_t)wch) & const_N::utf16_surrogate_mask) == const_N::utf16_leading_base  )
#define M_is_trail_surrogate(wch) ( (((uint32_t)wch) & const_N::utf16_surrogate_mask) == const_N::utf16_trailing_base )



// -------------------
// text file BOM items
// -------------------

namespace const_N
{ 
constexpr char    *utf32_le_bom    { "\xFF\xFE\x00\x00"            };                             // UTF-32-LE BOM  
constexpr char    *utf32_be_bom    { "\x00\x00\xFE\xFF"            };                             // UTF-32-BE BOM 
constexpr char    *utf16_le_bom    { "\xFF\xFE"                    };                             // UTF-16-LE BOM  
constexpr char    *utf16_be_bom    { "\xFE\xFF"                    };                             // UTF-16-BE BOM  
constexpr char    *utf8_bom        { "\xEF\xBB\xBF"                };                             // UTF-8     BOM  
constexpr char    *utf8_bom_w      { "\xEF\00\xBB\x00\xBF\00"      };                             // UTF-8     BOM   erroneously converted to wchar_t  
}


// ------------------------
// Simple Numeric functions
// ------------------------

#define M_min(x,y)         ( ((x) < (y)) ? (x) : (y) )
#define M_max(x,y)         ( ((x) > (y)) ? (x) : (y) )

#define M_lim(l, x, h)     (   ((x) < (l)) ? (l) : ( ((x) > (h)) ? (h) : (x) )   ) 
#define M_stdlim(l, x, h)  ( std::min( (std::max((l), (x))) , (h) )              )

#define M_fabs(x)    ( ((x) >= 0.0) ? (x) : -(x) )    

#define M_iabs(x)    ( ((x) >= 0  ) ? (x) : -(x) )   


// ---------------
// rounding MACROs
// ---------------

// numerator/denominator -> integer -- rounded up/down/towards0 

#define M_divide_roundup_p(    x,r) (((x) + (r) - 1) / (r))                                                // positive x -- round up, towards +_infinity                               -- assumes r is positive
#define M_divide_rounddown_p(  x,r) ( (x)            / (r))                                                // positive x -  round down, towards 0                                      -- assumes r is positive
                                                                       
#define M_divide_roundup_n(    x,r) ( (x)            / (r))                                                // negative x -- rounds up, towards 0                                       -- assumes r is positive
#define M_divide_rounddown_n(  x,r) (((x) - (r) + 1) / (r))                                                // negative x -- rounds down, towards -infinity                             -- assumes r is positive
                                                                       
#define M_divide_roundtoward0( x,r) ( (x)            / (r))                                                // +x and -x  -- rounds towards 0 (down for positive x, up for negative x)  -- assumes r is positive

#define M_divide_roundup(      x,r) ( (x) < 0 ? M_divide_roundup_n(  x,r) : M_divide_roundup_p(  x,r) )    // +x and -x   -- rounds up  , towards +infinity                            -- assumes r is positive
#define M_divide_rounddown(    x,r) ( (x) < 0 ? M_divide_rounddown_n(x,r) : M_divide_rounddown_p(x,r) )    // +x and -x   -- rounds down, towards -infinity                            -- assumes r is positive


// round up/down/towards0 -- integer x to nearest multiple of r

#define M_roundup_p(           x,r) ( (((x) + (r) - 1) / (r)) * (r) )                                      // positive x -- round up, towards +_infinity                               -- assumes r is positive
#define M_rounddown_p(         x,r) ( ( (x)            / (r)) * (r) )                                      // positive x -  round down, towards 0                                      -- assumes r is positive
                                                                                                         
#define M_roundup_n(           x,r) ( ( (x)            / (r)) * (r) )                                      // negative x -- rounds up, towards 0                                       -- assumes r is positive
#define M_rounddown_n(         x,r) ( (((x) - (r) + 1) / (r)) * (r) )                                      // negative x -- rounds down, towards -infinity                             -- assumes r is positive
                                                                                                         
#define M_roundtoward0(        x,r) ( ( (x)            / (r)) * (r) )                                      // +x and -x  -- rounds towards 0 (down for positive x, up for negative x)  -- assumes r is positive
                                                                                                         
#define M_roundup(             x,r) ( (x) < 0 ? M_roundup_n(  x,r) : M_roundup_p(  x,r) )                  // +x and -x   -- rounds up  , towards +infinity                            -- assumes r is positive
#define M_rounddown(           x,r) ( (x) < 0 ? M_rounddown_n(x,r) : M_rounddown_p(x,r) )                  // +x and -x   -- rounds down, towards -infinity                            -- assumes r is positive


// floating point -> integer rounding   -- consider using std::floor() std::ceil(), std::trunc(), std::round(), etc. -- except for M_round_away_zero()
// should use pred(1.0), next_before(1.0) etc for .9999999 etc.

#ifdef M_real_T_IS_double       // double versions

#define M_round(x)              ((int32_t)((x) < 0.0 ? (x) - 0.5            : (x) + 0.5            ))       // x = real_T. rounded to int32_t 
#define M_round_toward_zero(x)  ((int32_t)((x)                                                     ))       // x = real_T, rounded to int32_t
#define M_round_away_zero(x)    ((int32_t)((x) < 0.0 ? (x) - 0.999999999999 : (x) + 0.999999999999 ))       // x = real_T, rounded to int32_t 

#else                           // float versions

#define M_round(x)              ((int32_t)((x) < 0.0 ? (x) - 0.5            : (x) + 0.5            ))       // x = real_T. rounded to int32_t 
#define M_round_toward_zero(x)  ((int32_t)((x)                                                     ))       // x = real_T, rounded to int32_t
#define M_round_away_zero(x)    ((int32_t)((x) < 0.0 ? (x) - 0.999999       : (x) + 0.999999       ))       // x = real_T, rounded to int32_t 
#endif

#define M_round_down(x)         {if ((x) < 0.0)                                                    \
                                   then { M_round_away_zero(  x); }                                \
                                   else { M_round_toward_zero(x); }                                \
                                }

#define M_round_up(x)           {if ((x) < 0.0)                                                    \
                                   then { M_round_toward_zero(x); }                                \
                                   else { M_round_away_zero(  x); }                                \
                                }



// -------------------------------
// storage setting/clearing MACROs
// -------------------------------

#define M_clearv(v)         (std::memset( (void *)&(v),  0 ,  sizeof (v) ));  
#define M_clearp(p,l)       (std::memset( (void *)(p),   0 , (size_t)(l) ));  
#define M_setv(v,x)         (std::memset( (void *)&(v), (x),  sizeof (v) )); 
#define M_setp(p,l,x)       (std::memset( (void *)(p),  (x), (size_t)(l) ));


// ---------------------------------------------------------------
// MACROs used to access bytes in a data area x or pointed to by p
// ---------------------------------------------------------------

#define M_byte0_of(x)  ( ((uint8_t *)&(x))[ 0 ] )
#define M_byte1_of(x)  ( ((uint8_t *)&(x))[ 1 ] )
#define M_byte2_of(x)  ( ((uint8_t *)&(x))[ 2 ] )
#define M_byte3_of(x)  ( ((uint8_t *)&(x))[ 3 ] )
#define M_byte4_of(x)  ( ((uint8_t *)&(x))[ 4 ] )
#define M_byte5_of(x)  ( ((uint8_t *)&(x))[ 5 ] )
#define M_byte6_of(x)  ( ((uint8_t *)&(x))[ 6 ] )
#define M_byte7_of(x)  ( ((uint8_t *)&(x))[ 7 ] )

#define M_byte_of(x,n) ( ((uint8_t *)&(x))[(n)] )

//----------------------------------------------

#define M_byte0_at(p)  ( ((uint8_t *)(p))[ 0 ] )
#define M_byte1_at(p)  ( ((uint8_t *)(p))[ 1 ] )
#define M_byte2_at(p)  ( ((uint8_t *)(p))[ 2 ] )
#define M_byte3_at(p)  ( ((uint8_t *)(p))[ 3 ] )
#define M_byte4_at(p)  ( ((uint8_t *)(p))[ 4 ] )
#define M_byte5_at(p)  ( ((uint8_t *)(p))[ 5 ] )
#define M_byte6_at(p)  ( ((uint8_t *)(p))[ 6 ] )
#define M_byte7_at(p)  ( ((uint8_t *)(p))[ 7 ] )
#define M_byte8_at(p)  ( ((uint8_t *)(p))[ 8 ] )
#define M_byte9_at(p)  ( ((uint8_t *)(p))[ 9 ] )
#define M_byte10_at(p) ( ((uint8_t *)(p))[ 10] )
#define M_byte11_at(p) ( ((uint8_t *)(p))[ 11] )
#define M_byte12_at(p) ( ((uint8_t *)(p))[ 12] )
#define M_byte13_at(p) ( ((uint8_t *)(p))[ 13] )
#define M_byte14_at(p) ( ((uint8_t *)(p))[ 14] )
#define M_byte15_at(p) ( ((uint8_t *)(p))[ 15] )

#define M_byte_at(p,n) ( ((uint8_t *)(p))[(n)] )

//----------------------------------------------

#define M_hword0_of(x)  ( ((uint16_t *)&(x))[ 0 ] )
#define M_hword1_of(x)  ( ((uint16_t *)&(x))[ 1 ] )
#define M_hword2_of(x)  ( ((uint16_t *)&(x))[ 2 ] )
#define M_hword3_of(x)  ( ((uint16_t *)&(x))[ 3 ] )
#define M_hword4_of(x)  ( ((uint16_t *)&(x))[ 4 ] )
#define M_hword5_of(x)  ( ((uint16_t *)&(x))[ 5 ] )
#define M_hword6_of(x)  ( ((uint16_t *)&(x))[ 6 ] )
#define M_hword7_of(x)  ( ((uint16_t *)&(x))[ 7 ] )

#define M_hword_of(x,n) ( ((uint16_t *)&(x))[(n)] ) 

//----------------------------------------------

#define M_word0_of(x)  ( ((uint32_t *)&(x))[ 0 ] )
#define M_word1_of(x)  ( ((uint32_t *)&(x))[ 1 ] )
#define M_word2_of(x)  ( ((uint32_t *)&(x))[ 2 ] )
#define M_word3_of(x)  ( ((uint32_t *)&(x))[ 3 ] )
#define M_word4_of(x)  ( ((uint32_t *)&(x))[ 4 ] )
#define M_word5_of(x)  ( ((uint32_t *)&(x))[ 5 ] )
#define M_word6_of(x)  ( ((uint32_t *)&(x))[ 6 ] )
#define M_word7_of(x)  ( ((uint32_t *)&(x))[ 7 ] )

#define M_word_of(x,n) ( ((uint32_t *)&(x))[(n)] )

//----------------------------------------------

#define M_add_ptr(p,n) ( ((uint8_t *)(p)) + (n) )

#define    M_int8_at(p)  ( *(   int8_t    *)(p) )
#define   M_int16_at(p)  ( *(  int16_t    *)(p) )
#define   M_int32_at(p)  ( *(  int32_t    *)(p) )
#define   M_int64_at(p)  ( *(  int64_t    *)(p) )
#define   M_uint8_at(p)  ( *(  uint8_t    *)(p) )
#define  M_uint16_at(p)  ( *( uint16_t    *)(p) )
#define  M_uint32_at(p)  ( *( uint32_t    *)(p) )
#define  M_uint64_at(p)  ( *( uint64_t    *)(p) )
#define M_float32_at(p)  ( *(float32_T    *)(p) )
#define M_float64_at(p)  ( *(float64_T    *)(p) )
#define  M_real32_at(p)  ( *( real32_T    *)(p) )
#define  M_real64_at(p)  ( *( real64_T    *)(p) )



// --------------------
// miscellaneous MACROs
// --------------------

#define M_sep(x)  (M_add_separators((x)).c_str())   



// ------------------------------------------
// MACROs for latitude/longitude computations
//
//
//  Normalize longitude to range -180 to 180
//
//      M_fix_l(l)       valid range = -540   to 540   dergees -- l is ll_T  in arc-sec
//      M_fix_ld(l)      valid range = -540   to 540   degrees -- l is lld_T in degrees
//      M_fix_lr(l)      valid range = -540.0 to 540.0 degrees -- l is llr_T in degrees
//
//
//  Compute difference (east - west) between two longitudes, assuming 0 or 1 wrappings around the 180 to -180 meridian 
//
//      m_dif_l(west,east)     with east limit < west limit wraps -180/180 degrees only once  -- w and e are ll_T (integers) integers in arc-sec
//      m_dif_lf(west,east)    with east limit < west limit wraps -180/180 degrees only once  -- w and e are real_T in degrees  
//
// ------------------------------------------

#define M_arcsec(d)  ((d) * 3600)        // d is in degrees

#define M_arcsec_1    3600
#define M_arcsec_90   324000
#define M_arcsec_180  648000
#define M_arcsec_360  1296000
#define M_arcsec_540  1944000
#define M_arcsec_720  2592000

namespace const_N
{
constexpr int32_t arcsec_1   { 3600    };
constexpr int32_t arcsec_30  { 108000  };
constexpr int32_t arcsec_45  { 162000  };
constexpr int32_t arcsec_60  { 216000  };
constexpr int32_t arcsec_90  { 324000  };
constexpr int32_t arcsec_120 { 432000  };
constexpr int32_t arcsec_180 { 648000  };
constexpr int32_t arcsec_270 { 972000  };
constexpr int32_t arcsec_360 { 1296000 };
constexpr int32_t arcsec_540 { 1944000 };
constexpr int32_t arcsec_720 { 2592000 };
}  


#define M_fix_l(l)    ( ( (l) < -M_arcsec_180  ) ? ((l) + M_arcsec_360  ) : (((l) > M_arcsec_180  ) ? ((l) - M_arcsec_360  ) : (l)) )
#define M_fix_ld(l)   ( ( (l) <          -180  ) ? ((l) +          360  ) : (((l) >          180  ) ? ((l) -          360  ) : (l)) )
#define M_fix_lr(l)   ( ( (l) <          -180.0) ? ((l) +          360.0) : (((l) >          180.0) ? ((l) -          360.0) : (l)) )

#define M_dif_l(w,e)  ( ((e) <= (w)) ? ((e) + M_arcsec_360   - (w)) : ((e) - (w)) )
#define M_dif_ld(w,e) ( ((e) <= (w)) ? ((e) +          360   - (w)) : ((e) - (w)) )
#define M_dif_lr(w,e) ( ((e) <= (w)) ? ((e) +          360.0 - (w)) : ((e) - (w)) )

#define M_ld_to_l(l)  ( (l) * 3600 )
#define M_ld_to_lr(l) ( ((real_T)(l)) )
#define M_l_to_lr(l)  ( ((real_T)(l)) / 3600.0 )
#define M_l_to_ld(l)  ( (l) / 3600 )


// ------------------------------------------------------------------------------------------------------------
// Macros to reverse order of bytes while copying -- big-endian to little-endian or little-endian to big-endian 
// ------------------------------------------------------------------------------------------------------------

#define M_copyswap2(x,y)           \
{                                  \
    M_byte0_of(x) = M_byte1_of(y); \
    M_byte1_of(x) = M_byte0_of(y); \
}

#define M_copyswap2_p(x,y)         \
{                                  \
    M_byte0_at(x) = M_byte1_at(y); \
    M_byte1_at(x) = M_byte0_at(y); \
}


#define M_copyswap4(x,y)           \
{                                  \
    M_byte0_of(x) = M_byte3_of(y); \
    M_byte1_of(x) = M_byte2_of(y); \
    M_byte2_of(x) = M_byte1_of(y); \
    M_byte3_of(x) = M_byte0_of(y); \
}

#define M_copyswap4_p(x,y)         \
{                                  \
    M_byte0_at(x) = M_byte3_at(y); \
    M_byte1_at(x) = M_byte2_at(y); \
    M_byte2_at(x) = M_byte1_at(y); \
    M_byte3_at(x) = M_byte0_at(y); \
 }

#define M_copyswap8(x,y)           \
{                                  \
    M_byte0_of(x) = M_byte7_of(y); \
    M_byte1_of(x) = M_byte6_of(y); \
    M_byte2_of(x) = M_byte5_of(y); \
    M_byte3_of(x) = M_byte4_of(y); \
    M_byte4_of(x) = M_byte3_of(y); \
    M_byte5_of(x) = M_byte2_of(y); \
    M_byte6_of(x) = M_byte1_of(y); \
    M_byte7_of(x) = M_byte0_of(y); \
}

#define M_copyswap8_p(x,y)         \
{                                  \
    M_byte0_at(x) = M_byte7_at(y); \
    M_byte1_at(x) = M_byte6_at(y); \
    M_byte2_at(x) = M_byte5_at(y); \
    M_byte3_at(x) = M_byte4_at(y); \
    M_byte4_at(x) = M_byte3_at(y); \
    M_byte5_at(x) = M_byte2_at(y); \
    M_byte6_at(x) = M_byte1_at(y); \
    M_byte7_at(x) = M_byte0_at(y); \
}


// -------------------------------------------------------------------------------
// GDI+ Macros
// -------------------------------------------------------------------------------

#ifndef M_OUT_NARROW

// ------------------------------------------------------------------------------------------------

#define M_gls(    x,t) M_out(L"%s:  " L#x L".GetLastStatus() = %d" ) % (t) % x.GetLastStatus();
#define M_glsp(   x,t) M_out(L"%s:  " L#x L"->GetLastStatus() = %d") % (t) % x->GetLastStatus();


#define M_checks(s,t)                                                  \
{                                                                      \
    if ((int)(s) != 0)                                                 \
        M_out_emsg(L"%s:  Status = %d") % (t) % (s);                   \
}




#define M_checks_ret(s,t)                                              \
{                                                                      \
    if ((int)(s) != 0)                                                 \
    {                                                                  \
        M_out_emsg(L"%s:  Status = %d") % (t) % (s);                   \
        return -1;                                                     \
    }                                                                  \
}


#define M_checkls(x,t)                                                 \
{                                                                      \
    auto ls = x.GetLastStatus();                                       \
    if ((int)ls != 0)                                                  \
        M_out_emsg(L"%s:  " L#x L".GetLastStatus() = %d") % (t) % ls;  \
}


#define M_checkls_ret(x,t)                                            \
{                                                                     \
    auto ls = x.GetLastStatus();                                      \
    if ((int)ls != 0)                                                 \
    {                                                                 \
        M_out_emsg(L"%s:  " L#x L".GetLastStatus() = %d") % (t) % ls; \
        return -1;                                                    \
    }                                                                 \
}


#define M_checklsp(x,t)                                               \
{                                                                     \
    auto ls = x->GetLastStatus();                                     \
    if ((int)ls != 0)                                                 \
        M_out_emsg("L%s:  " L#x L"->GetLastStatus() = %d") % (t) % ls;\
}


#define M_checklsp_ret(x,t)                                           \
{                                                                     \
    auto ls = x->GetLastStatus();                                     \
    if ((int)ls != 0)                                                 \
    {                                                                 \
        M_out_emsg(L"%s:  " L#x L"->GetLastStatus() = %d") % (t) % ls;\
        return -1;                                                    \
    }                                                                 \
}


#define M_checkrc_ret(x,t)                                            \
{                                                                     \
    if ((x) != 0)                                                     \
    {                                                                 \
        M_out_emsg(L"%s: R/C = %d") % (t) % (x);                      \
        return -1;                                                    \
    }                                                                 \
}

// ------------------------------------------------------------------------------------------------

#else            // obsolete narrow char versions

// ------------------------------------------------------------------------------------------------

#define M_gls(    x,t) M_out("%s:  " #x ".GetLastStatus() = %d")  % (t) % x.GetLastStatus();
#define M_glsp(   x,t) M_out("%s:  " #x "->GetLastStatus() = %d") % (t) % x->GetLastStatus();


#define M_checks(s,t)                                               \
{                                                                   \
    if ((int)(s) != 0)                                              \
        M_out_emsg("%s:  Status = %d") % (t) % (s);                 \
}




#define M_checks_ret(s,t)                                           \
{                                                                   \
    if ((int)(s) != 0)                                              \
    {                                                               \
        M_out_emsg("%s:  Status = %d") % (t) % (s);                 \
        return -1;                                                  \
    }                                                               \
}


#define M_checkls(x,t)                                              \
{                                                                   \
    auto ls = x.GetLastStatus();                                    \
    if ((int)ls != 0)                                               \
        M_out_emsg("%s:  " #x ".GetLastStatus() = %d") % (t) % ls;  \
}


#define M_checkls_ret(x,t)                                          \
{                                                                   \
    auto ls = x.GetLastStatus();                                    \
    if ((int)ls != 0)                                               \
    {                                                               \
        M_out_emsg("%s:  " #x ".GetLastStatus() = %d") % (t) % ls;  \
        return -1;                                                  \
    }                                                               \
}


#define M_checklsp(x,t)                                             \
{                                                                   \
    auto ls = x->GetLastStatus();                                   \
    if ((int)ls != 0)                                               \
        M_out_emsg("%s:  " #x "->GetLastStatus() = %d") % (t) % ls; \
}


#define M_checklsp_ret(x,t)                                         \
{                                                                   \
    auto ls = x->GetLastStatus();                                   \
    if ((int)ls != 0)                                               \
    {                                                               \
        M_out_emsg("%s:  " #x "->GetLastStatus() = %d") % (t) % ls; \
        return -1;                                                  \
    }                                                               \
}


#define M_checkrc_ret(x,t)                                          \
{                                                                   \
    if ((x) != 0)                                                   \
    {                                                               \
        M_out_emsg("%s: R/C = %d") % (t) % (x);                     \
        return -1;                                                  \
    }                                                               \
}

// ------------------------------------------------------------------------------------------------

#endif




//---------------------------------------------------------------------------------------------------------------------
//     Timer/Clock MACROs -- C++
//---------------------------------------------------------------------------------------------------------------------

// M_start_timer(c, t1) -- c = clock variable name,  t1 = starting timepoint variable name
// M_set_timer  (c, t1) -- updates t1 with latest closk time 
// M_read_timer( c, t1) -- returns elapsed time in milliseconds

#define M_start_timer(c, t1)        \
std::chrono::steady_clock c;        \
auto t1 = c.now(); 

#define M_set_timer(c, t1)          \
t1 = c.now(); 

#define M_read_timer(c, t1)  ((real_T)((std::chrono::duration_cast<std::chrono::milliseconds>(c.now() - t1)).count()) / 1000.0)    
   


//---------------------------------------------------------------------------------------------------------------------
//     Intel/Microsoft performance counter MACROs
//---------------------------------------------------------------------------------------------------------------------

#define M_start_time(  pc1)      int64_t pc1 { query_performance_counter() };
#define M_elapsed_time(pc1)      performance_counter_interval(pc1);  



//---------------------------------------------------------------------------------------------------------------------
//     Trigonometric functions in degrees (double arguments)
//---------------------------------------------------------------------------------------------------------------------

#define M_rad(d) ( ((double)(d)) * (M_PI / 180.0))   // convert degrees to radians

#define M_deg(r) ( (r) * (180.0 / M_PI))             // convert radians to degrees 

#define M_cosd(d)      (             std::cos( M_rad(       d)))
#define M_sind(d)      (             std::sin( M_rad(       d)))
#define M_tand(d)      (             std::tan( M_rad(       d)))
#define M_secd(d)      (       1.0 / std::cos( M_rad(       d)))     //  bad, if cos(d) = 0.0 
#define M_cscd(d)      (       1.0 / std::sin( M_rad(       d)))     //  bad, if cos(d) = 0.0
#define M_cotd(d)      (             std::tan( M_rad(90.0 - d)))
#define M_atand(x    ) (       M_deg(std::atan(             x)))
#define M_asind(x    ) (       M_deg(std::asin(             x)))
#define M_acosd(x    ) (       M_deg(std::acos(             x)))
#define M_acotd(x    ) (90.0 - M_deg(std::atan(             x)))
#define M_asecd(x    ) (       M_deg(std::acos(       1.0 / x)))     //   bad, if x = 0.0
#define M_acscd(x    ) (       M_deg(std::asin(       1.0 / x)))     //   bad, if x = 0.0

#define M_atan2d(x, y) (       M_deg(std::atan2(         x, y)))



//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫



//---------------------------------------------------------------------------------------------------------------------
//    Error, Debugging and printout  MACROs
//---------------------------------------------------------------------------------------------------------------------

#ifndef M_OUT_NARROW 
#define M_bool_cstr(v)   ( (v) ?               L"{true}"  :               L"{false}"  )
#define M_bool_str(v)    ( (v) ? std::wstring {L"{true}"} : std::wstring {L"{false}"} )
#else
#define M_bool_cstr(v)   ( (v) ?                "{true}" :                 "{false}"  )
#define M_bool_str(v)    ( (v) ? std::string  {L"{true}"} : std::string  {L"{false}"} )
#endif





// error and exception MACROs

#define M__add_L(s) L##s
#define M_add_L(s) M__add_L(s)

#ifndef M_OUT_NARROW 
#define M_endf_clr            catch(...){{               M_out_emsg(M_add_L(__FUNCSIG__) L" ending because of exception");} throw;                            }
#define M_endf                catch(...){{M_cout_lock(); M_out_emsg(M_add_L(__FUNCSIG__) L" ending because of exception");} throw;                            }
#define M_endf_handle         catch(...){{M_cout_lock(); M_out_emsg(M_add_L(__FUNCSIG__) L" ending because of exception");} handle_exception();               }
#define M_endf_handle_r(rc)   catch(...){{M_cout_lock(); M_out_emsg(M_add_L(__FUNCSIG__) L" ending because of exception");} handle_exception(); return rc;    }
#else                                                                                                                                                        
#define M_endf_clr            catch(...){{               M_out_emsg(        __FUNCSIG__   " ending because of exception");} throw;                            }
#define M_endf                catch(...){{M_cout_lock(); M_out_emsg(        __FUNCSIG__   " ending because of exception");} throw;                            }
#define M_endf_handle         catch(...){{M_cout_lock(); M_out_emsg(        __FUNCSIG__   " ending because of exception");} handle_exception();               }
#define M_endf_handle_r(rc)   catch(...){{M_cout_lock(); M_out_emsg(        __FUNCSIG__   " ending because of exception");} handle_exception(); return rc;    }
#endif


// messsages 
#ifndef M_OUT_NARROW 

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define NMSG1 L"\n________________________________________________________________________________________________________________________________________________" \
              L"\n~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~\n"


#define NMSG2     L"~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~" \
                L"\n¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n\n"


#define EMSG1 L"\n____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" \
              L"\n!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!!  ERROR !!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!\n"


#define EMSG2     L"!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!!  ERROR !!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!" \
                L"\n¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n\n"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#else

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define NMSG1  "\n________________________________________________________________________________________________________________________________________________" \
               "\n~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~\n"


#define NMSG2    "~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~~~~~~~ NOTE ~~~~~~" \
               "\n¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n\n"


#define EMSG1  "\n____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" \
               "\n!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!!  ERROR !!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!\n"


#define EMSG2    "!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!!  ERROR !!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!!!!!!! ERROR !!!!!!" \
               "\n¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n\n"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#endif


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// old sprintf()-based messages -- not supported with wide char output
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef M_OUT_NARROW

#define M_msg(  f, ...) {printf(                             f "\n"        , __VA_ARGS__);}
#define M_msg1( f, ...) {printf(                             f             , __VA_ARGS__);}  // message to be continued on same line (no \n)
#define M_msg2( f, ...) {printf(                             f "\n"        , __VA_ARGS__);}  // continuation of message -- same as M_msg()
    
#define M_emsg( f, ...) {fprintf(stderr, EMSG1 "[%%%%%%%%] " f "\n" , EMSG2, __VA_ARGS__);}  // single-line error message
#define M_emsg1(f, ...) {fprintf(stderr, EMSG1 "[%%%%%%%%] " f "\n" ,        __VA_ARGS__);}  // first line of multiline error message
#define M_emsg0(f, ...) {fprintf(stderr,       "[%%%%%%%%] " f "\n" ,        __VA_ARGS__);}  // middle line(s) of multiline error message
#define M_emsg2(f, ...) {fprintf(stderr,       "[%%%%%%%%] " f "\n" , EMSG2, __VA_ARGS__);}  // last line of multiline error message

#define M_note( f, ...) {fprintf(stderr, NMSG1 "++++++ "     f "\n" , NMSG2, __VA_ARGS__);}  // single-line note 
#define M_note1(f, ...) {fprintf(stderr, NMSG1 "++++++ "     f "\n"        , __VA_ARGS__);}  // first line of multiline note
#define M_note0(f, ...) {fprintf(stderr,       "++++++ "     f "\n"        , __VA_ARGS__);}  // middle line(s) of multiline note
#define M_note2(f, ...) {fprintf(stderr,       "++++++ "     f "\n",  NMSG2, __VA_ARGS__);}  // last line of multiline note

#define M_error(f, ...) { char errbuf[1001]; _snprintf(errbuf, sizeof errbuf, f, __VA_ARGS__); errbuf[1000] = 0x00; throw std::runtime_error(errbuf); } 


// debug

#define M_show_um(um) M_bfmt( "bucket_ct=%u    max_bucket_ct=%u   load_factor=%.6f   max_load_factor=%.6f   size=%u   max_size=%u")   \
                            %  um.bucket_count()         \
                            %  um.max_bucket_count()     \
                            %  um.load_factor()          \
                            %  um.max_load_factor()      \
                            %  um.size()                 \
                            %  um.max_size()             \
                            ;

#ifdef SHOW_DEBUG
#define M_dmsg( f, ...)  {printf(f "\n", __VA_ARGS__);}
#define M_dmsg1(f, ...)  {printf(f     , __VA_ARGS__);}               // first line(s) of multiline message
#define M_dmsg2(f, ...)  {printf(f "\n", __VA_ARGS__);}               // last line of multiline message
#define M_dwide( w     ) {std::wcout << (w);}
#else
#define M_dmsg( f, ...)
#define M_dmsg1(f, ...)
#define M_dmsg2(f, ...)
#define M_dwide(w     )
#endif

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // defined(M_OUT_NARROW)
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// MACROs to include/exclude code
// ------------------------------

#define M__(x) 
#define M_n(x) 
#define M_y(x) x

#define M_if(c, x)  { if(c) {x} }

 

//  ------------------------------------------------
//  boost::format() MACROs
//  ------------------------------------------------
//
// sample use:      M_cout("x=%d  y=%d") % 11 % 22; 
//
//                  {
//                     M_cout_lock();
//                     M_cout("x=%d  y=%d") % 11 % 22;
//                     M_cout("z=%d")       % 33;
//                  }
//
//                  { M_out_lk("x=%d y=%d") % 11 % 22; }    // mutex unlocked when lock_guard goes out of scope
//
//
//
//


// Switchable MACROs to convert wstring to string and shorten strings/wstrings

#ifndef  M_OUT_NARROW
#define M_ws(ws)        ws 
#define M_wsl(ws, l)    ((::shorten_str((ws), (l))).c_str())
#else
#define M_ws(ws)        ((::out_ws((ws))).c_str()) 
#define M_wsl(ws, l)    ((::shorten_str(::out_ws((ws)), (l))).c_str())
#endif



// Switchable output MACRO versions (narrow char or widechar (UTF-16)) -- based on M_OUT_NARROW manifest constant

#ifndef  M_OUT_NARROW
#define M_out(      f)       M_outw( f)                                  // 
#define M_out1(     f)       M_out1w(f)                                  //  no newline -- message continues to next MACRO
#define M_out2(     f)       M_out2w(f)                                  //  continued messsage line (same as M_out())
#define M_err(      f)       M_errw( f)                                  // 
#define M_err1(     f)       M_err1w(f)                                  //  no newline -- message continues to next MACRO
#define M_err2(     f)       M_err2w(f)                                  //  continued messsage line (same as M_err())
#else
#define M_out(      f)       M_outn( f)                                  // 
#define M_out1(     f)       M_out1n(f)                                  //  no newline -- message continues to next MACRO
#define M_out2(     f)       M_out2n(f)                                  //  continued messsage line (same as M_out())
#define M_err(      f)       M_errn( f)                                  // 
#define M_err1(     f)       M_err1n(f)                                  //  no newline -- message continues to next MACRO
#define M_err2(     f)       M_err2n(f)                                  //  continued messsage line (same as M_out())
#endif


// widechar-only versions

#define M_wout(     f)       std::wcout << boost::wformat( f L"\n")   // 
#define M_wout1(    f)       std::wcout << boost::wformat( f      )   //  no newline -- message continues to next MACRO
#define M_wout2(    f)       std::wcout << boost::wformat( f L"\n")   //  continued messsage line (same as M_out())


// base MACROs for switchable versions 

#define M_outn(     f)       std::cout  << boost::format(  f  "\n")   // 
#define M_out1n(    f)       std::cout  << boost::format(  f      )   //  no newline -- message continues to next MACRO
#define M_out2n(    f)       std::cout  << boost::format(  f  "\n")   //  continued messsage line (same as M_out())

#define M_outw(     f)       std::wcout << boost::wformat( f L"\n")   // 
#define M_out1w(    f)       std::wcout << boost::wformat( f      )   //  no newline -- message continues to next MACRO
#define M_out2w(    f)       std::wcout << boost::wformat( f L"\n")   //  continued messsage line (same as M_out())

#define M_errn(     f)       std::cerr  << boost::format(  f  "\n")   // 
#define M_err1n(    f)       std::cerr  << boost::format(  f      )   //  no newline -- message continues to next MACRO
#define M_err2n(    f)       std::cerr  << boost::format(  f  "\n")   //  continued messsage line (same as M_err())

#define M_errw(     f)       std::wcerr << boost::wformat( f L"\n")   // 
#define M_err1w(    f)       std::wcerr << boost::wformat( f      )   //  no newline -- message continues to next MACRO
#define M_err2w(    f)       std::wcerr << boost::wformat( f L"\n")   //  continued messsage line (same as M_err())








// specific stream versions  

#define M_cout(     f)       std::cout  << boost::format(     f  "\n")
#define M_cerr(     f)       std::cerr  << boost::format(     f  "\n")
#define M_clog(     f)       std::clog  << boost::format(     f  "\n")
#define M_wcout(    f)       std::wcout << boost::wformat(    f L"\n")
#define M_wcerr(    f)       std::wcerr << boost::wformat(    f L"\n")
#define M_wclog(    f)       std::wclog << boost::wformat(    f L"\n")


/////////////////////////////////

#define M_cout_lock()        std::lock_guard<std::recursive_mutex> lock_guard_cout(global_N::mx_cout) // one mutex for all output streams

// combined locked output MACROs

#define M_out_lk(   f)       M_cout_lock(); M_out(  f)                  // narrow/wide form 
#define M_err_lk(   f)       M_cout_lock(); M_err(  f)                  // narrow/wide form 
#define M_cout_lk(  f)       M_cout_lock(); M_cout( f) 
#define M_cerr_lk(  f)       M_cout_lock(); M_cerr( f) 
#define M_clog_lk(  f)       M_cout_lock(); M_clog( f) 
#define M_wcout_lk( f)       M_cout_lock(); M_wcout(f) 
#define M_wcerr_lk( f)       M_cout_lock(); M_wcerr(f)
#define M_wclog_lk( f)       M_cout_lock(); M_wclog(f)


// note messages
//
//
//          usage:      M_out_note("note text %d %d") % 11 % 22; 
//
//                      M_out_note1("first  line %d %d) % 11 % 12;   
//                      M_out_note0("middle line %d %d) % 21 % 22; 
//                      M_out_note2("last   line %d %d) % 31 % 32; 
//
//                     {M_out_lk_note("note text %d %d") % 11 % 22;}
//

#ifndef M_OUT_NARROW

#define M_out_note( f)    M_out(    NMSG1 L"++++++ "     f L"\n" NMSG2)  // single-line note 
#define M_out_note1(f)    M_out(    NMSG1 L"++++++ "     f L"\n"      )  // first line of multiline note
#define M_out_note0(f)    M_out(          L"++++++ "     f L"\n"      )  // middle line(s) of multiline note
#define M_out_note2(f)    M_out(          L"++++++ "     f L"\n" NMSG2)  // last line of multiline note
#define M_out_notez()     M_out(                                 NMSG2)  // last empty line of multiline note

#define M_out_lk_note( f) M_out_lk( NMSG1 L"++++++ "     f L"\n" NMSG2)  // single-line note -- with lock

#else

#define M_out_note( f)    M_out(    NMSG1  "++++++ "     f  "\n" NMSG2)  // single-line note 
#define M_out_note1(f)    M_out(    NMSG1  "++++++ "     f  "\n"      )  // first line of multiline note
#define M_out_note0(f)    M_out(           "++++++ "     f  "\n"      )  // middle line(s) of multiline note
#define M_out_note2(f)    M_out(           "++++++ "     f  "\n" NMSG2)  // last line of multiline note
#define M_out_notez()     M_out(                                 NMSG2)  // last empty line of multiline note

#define M_out_lk_note( f) M_out_lk( NMSG1  "++++++ "     f       NMSG2)  // single-line note -- with lock

#endif


// error messages
//
//
//          usage:      M_out_emsg("error text %d %d") % 11 % 22; 
//
//                      M_out_emsg1("first  line %d %d) % 11 % 12;   
//                      M_out_emsg0("middle line %d %d) % 21 % 22; 
//                     
//                      M_out_emsg2("last   line %d %d) % 31 % 32; 
//                      -or-
//                      M_out_emsgz();  
//
//                     {M_out_lk_emsg("error text %d %d") % 11 % 22;}        
//

#ifndef M_OUT_NARROW

#define M_out_emsg( f)    M_err(     EMSG1    L"!!!!!! " f L"\n" EMSG2)   // single-line error message
#define M_out_emsg1(f)    M_err1(    EMSG1    L"!!!!!! " f L"\n"      )   // first line of multiline error message
#define M_out_emsg0(f)    M_err1(             L"!!!!!! " f L"\n"      )   // middle line(s) of multiline error message
#define M_out_emsg2(f)    M_err2(             L"!!!!!! " f L"\n" EMSG2)   // last line of multiline error message
#define M_out_emsgz()     M_err2(                                EMSG2)   // closing line of multiline error message

#define M_out_lk_emsg( f) M_err_lk(  EMSG1    L"!!!!!! " f L"\n" EMSG2)   // single-line error message -- with lock 

#else

#define M_out_emsg( f)    M_err(     EMSG1     "!!!!!! " f  "\n" EMSG2)   // single-line error message
#define M_out_emsg1(f)    M_err1(    EMSG1     "!!!!!! " f  "\n"      )   // first line of multiline error message
#define M_out_emsg0(f)    M_err1(              "!!!!!! " f  "\n"      )   // middle line(s) of multiline error message
#define M_out_emsg2(f)    M_err2(              "!!!!!! " f  "\n" EMSG2)   // last line of multiline error message
#define M_out_emsgz()     M_err2(                                EMSG2)   // closing line of multiline error message
                                                                        
#define M_out_lk_emsg( f) M_err_lk(  EMSG1     "!!!!!! " f  "\n" EMSG2)   // single-line error message -- with lock 

#endif


// throw exception with .what() = formatted message
//
//            M_throw_v("error %d %d") % 11 % 22 )); 
//            M_throw("error text")
//
//

#define M_throw_v(f)       throw std::runtime_error(boost::str(boost::format(f)
#define M_throw(  f)      {throw std::runtime_error(                         f);}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// template functions  ///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
//
// multimap_at() -- return n-th value in a multimap (exception, if item is not present)
//
////////////////////////////////////////////////////////////////////////////////////

template<typename T1, typename T2>
T2 multimap_at(const std::multimap<T1, T2>& mm, const T1& key, uint64_t n = 0)
{
    // simple case when n = 0

    if (n == 0)
    {
        auto it = mm.find(key);

        if (it == mm.end())
        {
            M_out_emsg(L"multimap_at() -- passed-in key was not present in multimap");
            M_throw(    "multimap_at() -- passed-in key was not present in multimap")
        }
        else
            return it->second; 
    }


    // general case -- cause exception if n-th key is not present in the multimap

    auto ct = mm.count(key); 

    if (ct <= n)
    {
        M_out_emsg(L"multimap_at() -- passed-in key[%d] was not present in multimap") % n;
        M_throw_v(  "multimap_at() -- passed-in key[%d] was not present in multimap") % n));
    }


    // find position of n-th element for passed-in key 

    auto it = mm.find(key);

    if (n > 0)  
        for (auto i = 0; i < n; i++) it++;

    return it->second;    // return value for n-th element in multi-map  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// multimap_deref() -- return n-th key/value pair in a multimap (exception, if 'n' is past end of multimap)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T1, typename T2>
std::pair<T1, T2> multimap_deref(const std::multimap<T1, T2>& mm, uint64_t n)
{
    // make sure 'n' is not past end of multimap

    if (n >= mm.size())
    {
         M_out_emsg(L"multimap_deref() -- passed-in index (n=%d) is past end of multimap (size()=%d)") % n % mm.size();
         M_throw(    "multimap_deref() -- passed-in index is past end of multimap")       
    }


    //   use incrementation to get iterator that points to n-th item (0-based) 

    auto it = mm.cbegin(); 

    if (n > 0) 
        for (auto i = 0; i < n; i++) it++;

    return *it;        
}



///////////////////////////////////////////////////////
// convenience functions using M_copyswap_xx MACROs
///////////////////////////////////////////////////////

template<typename T> 
T bigend2(const void *p)
{
    T x;
    M_copyswap2_p(&x, p)
    return x;
}

template<typename T> 
T bigend4(const void *p)
{
    T x;
    M_copyswap4_p(&x, p)
    return x;
}

template<typename T> 
T bigend8(const void *p)
{
    T x;
    M_copyswap8_p(&x, p)
    return x;
}


/////////////////////////////////////////////////
//   F_max(), F_min(), F_lim(), F_roundup()
/////////////////////////////////////////////////


// ??????????????????? should redo these with variadic templates ????????????????????????????

/////////// F_max(x,y) ///////////

template<typename T> inline  const T& 
F_max(const T& p1, const T& p2)         // use if max() is usurped via MFC MACRO definition (can use (max)(x,y), too)
{
    return (p1 > p2) ? p1 : p2;  
}

////////// F_min(x,y) ////////////////

template<typename T> inline const T& 
F_min(const T& p1, const T& p2)         // use if min() is usurped by MFC MACRO definition (can use (min)(x,y), too)
{
    return (p1 < p2) ? p1 : p2;  
}

///////////// F_max(x,y,z) ////////////////

template<typename T> inline const T& 
F_max( const T& p1
     , const T& p2
     , const T& p3
     )         
{
    return (p1 > p2) ? ( (p3 > p1) ? p3 : p1 )
                     : ( (p3 > p2) ? p3 : p2 )
                     ;
}

/////////// F_max(w,x,y,z) //////////////////

template<typename T> inline const T& 
F_max( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     )         
{
    return F_max(F_max(p1, p2), F_max(p3, p4));                     
}


/////////// F_max(v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_max( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     )         
{
    return F_max(F_max(p1, p2, p3), F_max(p4, p5));                     
}

/////////// F_max(u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_max( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     )         
{
    return F_max(F_max(p1, p2, p3), F_max(p4, p5, p6));                     
}

/////////// F_min(x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     )         
{
    return (p1 < p2) ? ( (p3 < p1) ? p3 : p1 )
                     : ( (p3 < p2) ? p3 : p2 )
                     ;
}


/////////// F_min(w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     )         
{
    return F_min(F_min(p1, p2), F_min(p3, p4));                     
}


/////////// F_min(v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5));                     
}

/////////// F_min(u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5, p6));                     
}

/////////// F_min(t,u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     , const T& p7
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5, p6), p7);                     
}

/////////// F_min(s,t,u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     , const T& p7
     , const T& p8
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5, p6), F_min(p7, p8));                     
}

/////////// F_min(r,s,t,u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     , const T& p7
     , const T& p8
     , const T& p9
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5, p6), F_min(p7, p8, p9));                     
}



///////////// F_lim(lo,x,hi) //////////

template<typename T> inline const T& 
F_lim(const T& l, const T& p, const T& h)
{
    return (p < l) ? l
                   : ( (p > h) ? h : p )
                   ;  
}


///////////// F_roundup(x, r) ////////// (only works for integer types)

template<typename T> inline T 
F_roundup(const T& x, const T& r)
{
    return ((x + r - 1) / r) * r; 
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///    api_err_S -- consolidated error info and message text  
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 

// --------------------------------------------------------------------------------------------------------------------
//     non_syserr_E -- code value to indicate what local or non-API function was issued just before the error 
// --------------------------------------------------------------------------------------------------------------------

typedef enum class non_syserr_E                                                        // which non-system error was detected
{                                                                                    
    none                               ,                                               // default value -- for initialization 
    unexpected_directory               ,                                               // directory name passed in as filename for I/O operation
    unsupported_encoding               ,                                               // UTF-32 BOM seen in input file 
    tfile_open_wfopen_s_utf16_be       ,                                               // tried to use _wfopen_s() with UTF16-BE file encoding
    tfile_open_wfopen_s_utf32          ,                                               // tried to use _wfopen_s() with UTF32    file encoding
    tfile_open_wfopen_unknown_encoding ,                                               // tried to use _wfopen_s() with unknown  file encoding
    tfile_open_stream_utf32            ,                                               // tried to use C++ stream with UTF-32 file encoding
    tfile_open_default_utf32           ,                                               // tried to use default I/O method with UTF-32 file encoding  
    tfile_open_no_encoding             ,                                               // File encoding not passed into       tfile_C:: open()
    tfile_open_multiple_encodings      ,                                               // Multiple file encodings passed into tfile_C:: open()
    tfile_open_no_io_method            ,                                               // I/O method not passed into          tfile_C:: open()
    tfile_open_multiple_io_methods     ,                                               // Multiple I/O methods passed into    tfile_C:: open()
    tfile_getline_error                ,                                               // tfile_C::getline() called after prior error reported, and no close()/re-open() done  
    tfile_getline_closed               ,                                               // tfile_C::getline() called when file is not open() 
    token_stream_attach_error                                                          // token_stream_C::attach_file() called to attach a file after prior error was returned 
} non_syserr_T; 
 


// --------------------------------------------------------------------------------------------------------------------
//     api_op_E -- code value to indicate what API was issued just before the error 
// --------------------------------------------------------------------------------------------------------------------

typedef enum class api_op_E                                                           // last API operation being performed
{                                                                                  
    none                   ,                                                          // default value -- for initialization 
    non_system             ,                                                          // used after internally detected error (like bad encoding, file not open)
    GetFileAttributesExW   ,                                        
    _wfopen_s              ,
    fclose                 ,
    fread_s                ,
    fgetws                 ,
    ifstream_constructor   ,
    ifstream_open          ,
    ifstream_close         ,
    ifstream_getline       ,
    wifstream_constructor  ,
    wifstream_open         ,
    wifstream_imbue        ,
    wifstream_close        ,
    wifstream_getline      ,
    wstring_convert        ,
    MultiByteToWideChar    ,
    getline           
} api_op_T; 



// --------------------------------------------------------------------------------------------------------------------
//     api_err_S -- consolidated error info and message etxt for various API errors 
// --------------------------------------------------------------------------------------------------------------------


struct api_err_S                                                                           // error messages and error codes, etc. from file operations
{
    // consolidated error status

    bool                        error_occurred                  { false              };    // set when any type of error (system-detected or internally-detected) occurred
    std::wstring                error_text                      {                    };    // consolidated error message for this error  
                                                                                     
                                                                                     
    // I/O error flags -- describes what type of I/O error occurred                  
                                                                                     
    bool                        system_error_occurred           { false              };    // error from ::GetFileAttributes()  (or other win32 API)  has occurred
    bool                        errno_error_occurred            { false              };    // set when ::_wopen_s() or ::fread_s() ::close() etc.
    bool                        stream_error_occurred           { false              };    // set when std::ifstream or std::wifstream error occurs
    bool                        exception_caught                { false              };    // exception was thrown by an invoked C++ service and caught
    bool                        non_system_error_occurred       { false              };    // set when error detected other than ones from called system functions

    api_op_T                    api_op                          { api_op_E::none     };    // indicates which I/O operation (or other operation) got this error   


    // error info from non-system errors

    non_syserr_T                non_syserr                      { non_syserr_E::none };    // indicates which non-system error was found


    // error info from system errors (GetFileAttributes() or other WIN32 APIs, etc.)

    uint32_t                    system_error_code               { 0                   };    // Windows DWORD type from GetLastError() 
    std::wstring                system_error_text               {                     };    // Windows system message text for this system_error_code
                                                                                      
                                                                                      
    // error info from error-based errors (from FILE I/O operations, etc.)                                  
                                                                                      
    int                         errno_code                      { 0                   };    // Windows errno_t type 
    std::wstring                errno_text                      {                     };    // error text from _wcstrerror() for this errno 
    

    // error info from C++ exception                                  

    std::wstring                exception_text                  {                     };    // text from .what() converted to std::wstring 
                                                                        

    // error info from C++ stream-based errors                                                                  
                                                                                      
    uint32_t                    stream_state                    { 0                   };    // stream state reported by rdflags()
                                                                                      
    bool                        stream_goodbit                  { false               };    // goodbit as reported by good()  
    bool                        stream_eofbit                   { false               };    // eofbit  as reported by eof()
    bool                        stream_failbit                  { false               };    // failbit as reported by fail()
    bool                        stream_badbit                   { false               };    // badbit  as reported by bad()   
};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///    buffer_C<T> class    -- template functions -- compiler generated -- no need to DLL import/export
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> 
class buffer_C
{
public:
                  buffer_C(                   ) = default    ;   // default  constructor
    explicit      buffer_C(  uint64_t         )              ;   // uint64_t constructor
                  buffer_C(  const buffer_C&  )              ;   // copy     constructor
                  buffer_C(  buffer_C&&       )              ;   // move     constructor
                 ~buffer_C(                   )              ;   // destructor
    buffer_C&     operator=( const buffer_C&  )              ;   // copy assignment
    buffer_C&     operator=( buffer_C&&       )              ;   // move assignment
                                                             
    void          alloc(     uint64_t         )              ;   // allocate buffer (free any old one) -- input size in sizeof (T)
    void          alloc1(    uint64_t         )              ;   // allocate buffer (free any old one) -- input size in bytes
    void          free(                       )              ;   // free buffer, if any
    void          realloc(   uint64_t         )              ;   // get new buffer is current one is too small -- input size in sizeof (T)
    void          realloc1(  uint64_t         )              ;   // get new buffer is current one is too small -- input size in bytes
                                                            
    void          clear(                      )              ;   // zero out entire buffer area
    void          assign1(   char             )              ;   // set entire buffer to some one-byte value

    ///////////////////  Data members ////////////////////////////////////////////////////////////

    T            *p                              { nullptr } ;   // typed pointer to start of buffer data
    uint8_t      *p1                             { nullptr } ;   // byte  pointer to start of buffer data
    uint64_t      sz                             { 0ULL    } ;   // number of (typed) items in allocated buffer
    uint64_t      sz1                            { 0ULL    } ;   // size of allocated buffer, in bytes
private:                                                     
    uint32_t      copy_ct                        { 0U      } ;   // copy count  (for debug tracking only)                                                    
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///    tracker_C class 
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class tracker_C
{
public:
    M_CORE_IMPEXP                  tracker_C(                );   // default constructor
    M_CORE_IMPEXP    explicit      tracker_C(int             );   // int     constructor (with ID)
    M_CORE_IMPEXP                  tracker_C(const tracker_C&);   // copy    constructor
    M_CORE_IMPEXP                  tracker_C(tracker_C&&     );   // move    constructor
    M_CORE_IMPEXP                 ~tracker_C(                );   // destructor
    M_CORE_IMPEXP    tracker_C&    operator=(const tracker_C&);   // copy assignment
    M_CORE_IMPEXP    tracker_C&    operator=(tracker_C&&     );   // move assignment

    ///////////////////  Data members ////////////////////////////////////////////////////////////

                     int           id            { 0 }        ;   // ID for this class -- set when directly constructed, copied from parent when moved/copied
                     int           copy_id       { 0 }        ;   // copy ID = parent's copy ID + 0 or 1 -- start out at 0 for directly constructed instances 
                     int           move_id       { 0 }        ;   // move ID = parent's move ID + 0 or 1 -- start out at 0 for directly constructed instances
                     std::wstring  lineage       {   }        ;   // copy/move lineage for this instance      
};



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///   buf8_T  -- non-template version of buffer_C -- specialized for uint8_t
///   buf16_T -- non-template version of buffer_C -- specialized for uint16_t
///   buf32_T -- non-template version of buffer_C -- specialized for uint32_t
///   buf64_T -- non-template version of buffer_C -- specialized for uint64_t
///   =======    -----------------------------------------------------------
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

typedef class buffer_C< uint8_t>   buf8_T; 
typedef class buffer_C< uint16_t> buf16_T;
typedef class buffer_C< uint32_t> buf32_T;
typedef class buffer_C< uint64_t> buf64_T;



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///   databuf_C -- class for holding unformatted data -- buf8_T plus added count field
///   =========    -------------------------------------------------------------------
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

class databuf_C : public buf8_T
{
public:

    M_CORE_IMPEXP                     databuf_C(                ) = default     ;       // default contructor
                                      databuf_C(const databuf_C&) = delete      ;       // copy constructor      (deleted)
                                      databuf_C(databuf_C&&     ) = delete      ;       // move constructor      (deleted)
    M_CORE_IMPEXP                    ~databuf_C(                ) = default     ;       // destructor
                   filebuf_C          operator=(const databuf_C&) = delete      ;       // copy assignment        deleted)
                   filebuf_C&         operator=(databuf_C&&     ) = delete      ;       // move assignment        deleted)
   
                   uint64_t           ct                          { 0 }         ;       // number of bytes of data contained in buffer
};



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///   filebuf_C -- class for reading in whole binary files and saving contents
///   =========    -----------------------------------------------------------
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

class filebuf_C : public buf8_T
{
public: 
    M_CORE_IMPEXP                     filebuf_C(                ) = default     ;       // default contructor
    M_CORE_IMPEXP                     filebuf_C(const filebuf_C&) = delete      ;       // copy constructor      (deleted)
    M_CORE_IMPEXP                     filebuf_C(filebuf_C&&     ) = delete      ;       // move constructor      (deleted)
    M_CORE_IMPEXP                    ~filebuf_C(                ) = default     ;       // destructor
    M_CORE_IMPEXP  filebuf_C          operator=(const filebuf_C&) = delete      ;       // copy assignment       (deleted)
    M_CORE_IMPEXP  filebuf_C&         operator=(filebuf_C&&     ) = delete      ;       // move assignment       (deleted)

    M_CORE_IMPEXP  int64_t            getsize(const std::wstring&, bool = true) ;       // return size of file, or -1 if stat() fails -- sets error_flag 
    M_CORE_IMPEXP  int                readin( const std::wstring&             ) ;       // open file, read in file, close file (all data from file in buffer)
    M_CORE_IMPEXP  void               freeup(                                 ) ;       // freeup any buffer space

    ///////////////////////// data members ////////////////////////////////////

                   std::wstring       m_pathname      {       }                 ;       // complete filename for one file whose contents are in buffer 
                   bool               m_error_flag    { false }                 ;       // true, if error has been seen                       
                   uint64_t           m_ct            { 0     }                 ;       // number of bytes from file in buffer
};



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///   tfile_C -- class for reading in encoded text files, one line at a time
///   =======    -----------------------------------------------------------
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


// --------------------------------------------------------------------------------------------------------------------
//     tfile_parm_S -- I/O operation request/configuration parms
// --------------------------------------------------------------------------------------------------------------------

struct tfile_parm_S                                                                   // parameters to control type of I/O routines to use
{                                                                                  
    bool                        use_file                 { false };                   // true -- use C FILE*                              -and-   fgets() or fgetws()
    bool                        use_stream               { false };                   // true -- use C++ std:ifstream or std::wifstream   -and-   std::fstream::getline()   or  std::getline() 
    bool                        use_default              { false };                   // true -- pick use_file or use_stream based on input file info 
    bool                        remove_line_end          { false };                   // true -- remove \r or \n at end of passsed-back line            
};


// --------------------------------------------------------------------------------------------------------------------
//     tfile_info_S -- pre-determined information about a text file
// --------------------------------------------------------------------------------------------------------------------

struct tfile_info_S                                                    // output from pathname_info() and 
{
    std::wstring                filename                 {       };    // pathname -- filled in if file (not directory) found

    bool                        is_directory             { false }; 
    bool                        is_file                  { false };    
    WIN32_FILE_ATTRIBUTE_DATA   file_attrs               { 0     };    // raw file attributes from Windows -- filled in from call to GetFileAttributes() 

    size_t                      length                   { 0     };
                                
    bool                        is_ascii                 { false };    // no BOM found, so ACII (1252?) assumed
    bool                        is_utf8                  { false };
    bool                        is_utf16_le              { false };
    bool                        is_utf16_be              { false };
    bool                        is_utf32_le              { false };
    bool                        is_utf32_be              { false };
                                
    size_t                      bom_length               { 0     }; 
};



// --------------------------------------------------------------------------------------------------------------------
//     tfile_C::  --  text file I/O class
// --------------------------------------------------------------------------------------------------------------------

class tfile_C
{
public: 
    M_CORE_IMPEXP                     tfile_C(                      ) = default                                   ;     // default contructor
                                      tfile_C(const tfile_C&        ) = delete                                    ;     // copy constructor    (deleted)
                                      tfile_C(tfile_C&&             ) = delete                                    ;     // move constructor    (deleted)
    M_CORE_IMPEXP                    ~tfile_C(                      )                                             ;     // destructor
                   filebuf_C          operator=(const tfile_C&      ) = delete                                    ;     // copy assignment     (deleted)
                   filebuf_C&         operator=(tfile_C&&           ) = delete                                    ;     // move assignment     (deleted)
    
    M_CORE_IMPEXP  int                open(                     const std::wstring&,    api_err_S&)               ;     // open  file or stream -- default options -- input text file -- pass in pathname
    M_CORE_IMPEXP  int                open(                     const tfile_info_S&,    api_err_S&)               ;     // open  file or stream -- default options -- input text file -- pass in file info
    M_CORE_IMPEXP  int                open(const tfile_parm_S&, const tfile_info_S&,    api_err_S&)               ;     // open  file or stream -- custom options
    M_CORE_IMPEXP  int                close(                                            api_err_S&)               ;     // close file or stream
    M_CORE_IMPEXP  int                close(                                                      )               ;     // close file or stream -- don't fill in any api_err_S
    M_CORE_IMPEXP  int                getline(         std::wstring&,                   api_err_S&)               ;     // get one line of input data

private:
                   int                get_stream_state(  api_op_T,                      api_err_S&, bool=false)   ;     // get stream state after I/O operation
                   int                get_exception_info(api_op_T,                      api_err_S&)               ;     // get info from exception thrown after I/O-oriented operation 
                   int                get_file_state(    api_op_T,  int, errno_t,       api_err_S&)               ;     // get FILE   state after I/O operation



    ///////////////////////// data members ////////////////////////////////////


    //             saved option settings and file info from open() 

                   tfile_parm_S       m_parm          {         };                                                      // saved tfile_parm_S input to open() 
                   tfile_info_S       m_info          {         };                                                      // saved tfile_info_S input to open() 
                                                                                                                     
                                                                                                                     
    //             file controls                                                                                     
                                                                                                                     
                   std::wstring       m_filename      {         };                                                      // name for current input file
                   uint64_t           m_lineno        { 0       };                                                      // line numberf in file -- for error messages
                                                                                                                     
                   bool               m_file_open     { false   };                                                      // true: *m_file_p    is open
                   bool               m_stream_open   { false   };                                                      // true: *m_stream_p  is open
                   bool               m_wstream_open  { false   };                                                      // true: *m_wstream_p is open
                                                                                                                     
                   FILE              *m_file_p        { nullptr };                                                      // current input file FILE     
                   std::ifstream     *m_stream_p      { nullptr };                                                      // current input file (for ASCII or UTF-8 input)
                   std::wifstream    *m_wstream_p     { nullptr };                                                      // current input file (for UTF-16 input)
                                                                                                                     
                   bool               m_goodbit       { false   };                                                      // std:ios::goodbit after last I/O operation 
                   bool               m_eofbit        { false   };                                                      // std:ios::eofbit  after last I/O operation
                   bool               m_failbit       { false   };                                                      // std:ios::failbit after last I/O operation
                   bool               m_badbit        { false   };                                                      // std:ios::badbit  after last I/O operation
                                                                                                                     
                   int                m_io_rc         { 0       };                                                      // R/C from    last FILE I/O operation
                   errno_t            m_errno         { 0       };                                                      // errno after last FILE I/O operation
                                                                                                                     
                                                                                                                     
     //            file state and status                                                                             
                                                                                                                     
                   bool               m_is_open       { false   };                                                       // true: file is open -- either as C++ stream or C file
                   bool               m_1st_line_done { false   };                                                       // true -- 1st line already processed (passed back)
                   bool               m_eof_seen      { false   };                                                       // EOF: seen on read()
                   bool               m_error_seen    { false   };                                                       // fail() bit set after read()


     //            persistent buffer for fgetws()

                   buffer_C<wchar_t>  m_readbuf       {         };                                                       // empty buffer, until fopen()
                   size_t             m_readbuf_l     { 0       };                                                       // length of read buffer   
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///    Functions from MSDN (Microsoft) -- in msdn.cpp
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_CORE_IMPEXP  int GetEncoderClsid(const WCHAR*, CLSID*);




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///    Utility functions in core_util.cpp
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------
// string-oriented functions
// -------------------------

M_CORE_IMPEXP  std::wstring    to_wstring(const std::string&   , UINT = CP_ACP);
M_CORE_IMPEXP  std::wstring    to_wstring(const char *         , UINT = CP_ACP);
M_CORE_IMPEXP  std::string     to_string( const std::wstring&  , UINT = CP_ACP);
M_CORE_IMPEXP  std::string     to_string( const wchar_t *      , UINT = CP_ACP);

M_CORE_IMPEXP  std::wstring    to_wstring(                  unit_T   );  // substitute for std::to_wstring() for unit_T type
M_CORE_IMPEXP  std::wstring    to_wstring(                  bool     );  // substitute for std::to_wstring() for bool type

M_CORE_IMPEXP  int             to_char32( wchar_t, wchar_t    , char32_t&,           bool = true);
M_CORE_IMPEXP  int             to_wchars( char32_t            , wchar_t&, wchar_t&              ); 
       
M_CORE_IMPEXP  std::wstring    to_wstring(                  unit_T   );  // substitute for std::to_wstring() for unit_T type
M_CORE_IMPEXP  std::wstring    to_wstring(                  bool     );  // substitute for std::to_wstring() for bool type


// string to/from conversion

M_CORE_IMPEXP  int             to_int8(    const std::wstring&,    int8_t&, int = 10);
M_CORE_IMPEXP  int             to_int16(   const std::wstring&,   int16_t&, int = 10);
M_CORE_IMPEXP  int             to_int32(   const std::wstring&,   int32_t&, int = 10);
M_CORE_IMPEXP  int             to_int64(   const std::wstring&,   int64_t&, int = 10);
M_CORE_IMPEXP  int             to_uint8(   const std::wstring&,   uint8_t&, int = 10);
M_CORE_IMPEXP  int             to_uint16(  const std::wstring&,  uint16_t&, int = 10);
M_CORE_IMPEXP  int             to_uint32(  const std::wstring&,  uint32_t&, int = 10);
M_CORE_IMPEXP  int             to_uint64(  const std::wstring&,  uint64_t&, int = 10); 
M_CORE_IMPEXP  int             to_float32( const std::wstring&, float32_T&          );
M_CORE_IMPEXP  int             to_float64( const std::wstring&, float64_T&          );       


// string insert formatting functions (using swprintf) 
// ---------------------------------------------------

M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, unit_T                        );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, bool                          );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int8_t                        );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int16_t                       );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int32_t                       );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int64_t                       );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, uint8_t                       );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, uint16_t                      );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, uint32_t                      );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, uint64_t                      );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, float32_T                     );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, float64_T                     );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, const std::wstring&           );

M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, unit_T                   );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, bool                     );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, int8_t                   );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, int16_t                  );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, int32_t                  );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, int64_t                  );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, uint8_t                  );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, uint16_t                 );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, uint32_t                 );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, uint64_t                 );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, float32_T                );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, float64_T                );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&, int, const std::wstring&      );

M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, unit_T               );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, bool                 );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, int8_t               );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, int16_t              );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, int32_t              );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, int64_t              );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, uint8_t              );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, uint16_t             );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, uint32_t             );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, uint64_t             );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, float32_T            );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, float64_T            );
M_CORE_IMPEXP  std::wstring    fmt_str(const std::wstring&,int, int, const std::wstring&  );


// miscellaneous string formatting functions
// -----------------------------------------

M_CORE_IMPEXP  std::wstring    fmt_ptr(const void *); 

M_CORE_IMPEXP  std::string     out_ws(     std::wstring         );
M_CORE_IMPEXP  std::string     shorten_str(std::string  , size_t);
M_CORE_IMPEXP  std::wstring    shorten_str(std::wstring , size_t);

M_CORE_IMPEXP std::wstring     deg_to_d_m_s(real_T, int32_t);
                                                             
M_CORE_IMPEXP int              remove_line_end(wchar_t *    );
M_CORE_IMPEXP int              remove_line_end( char   *    );
M_CORE_IMPEXP int              remove_line_end(std::wstring&);
M_CORE_IMPEXP int              remove_line_end(std::string& );



// functions to insert separator/grouping characters in (numeric) strings
// ----------------------------------------------------------------------

#define M_SEP_STRIDE         3
#define M_MAX_LENGTH_NO_SEP  5
           
M_CORE_IMPEXP  std::string     add_separators_a(  uint16_t           , char    = 0xAF   , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::string     add_separators_a(   int16_t           , char    = 0xAF   , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::string     add_separators_a(  uint32_t           , char    = 0xAF   , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::string     add_separators_a(   int32_t           , char    = 0xAF   , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::string     add_separators_a(  uint64_t           , char    = 0xAF   , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::string     add_separators_a(   int64_t           , char    = 0xAF   , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::string     add_separators_a( float32_T           , char    = 0xAF   , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::string     add_separators_a( float64_T           , char    = 0xAF   , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
                                                                                        
M_CORE_IMPEXP  std::wstring    add_separators_w(  uint16_t           , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::wstring    add_separators_w(   int16_t           , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::wstring    add_separators_w(  uint32_t           , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::wstring    add_separators_w(   int32_t           , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::wstring    add_separators_w(  uint64_t           , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::wstring    add_separators_w(   int64_t           , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::wstring    add_separators_w( float32_T           , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
M_CORE_IMPEXP  std::wstring    add_separators_w( float64_T           , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);
                                                                     
M_CORE_IMPEXP  std::wstring    add_separators_w( const std::wstring& , wchar_t = 0x00AF , uint32_t = M_MAX_LENGTH_NO_SEP, uint32_t = M_SEP_STRIDE);


// switchable occult MACROs to use wchar_t or char add_separators_x() routines (above) 

#ifndef M_OUT_NARROW
#define         M_add_separators(x) add_separators_w((x))  
#else
#define         M_add_separators(x) add_separators_a((x))  
#endif
 

// ---------------------------------------------------
// bigendian <-> little endian byte swapping functions
// ---------------------------------------------------

M_CORE_IMPEXP   int16_t  bigend_int16(  const void *p);
M_CORE_IMPEXP  uint16_t  bigend_uint16( const void *p);
M_CORE_IMPEXP   int32_t  bigend_int32(  const void *p);
M_CORE_IMPEXP  uint32_t  bigend_uint32( const void *p);
M_CORE_IMPEXP  float32_T bigend_float32(const void *p);
M_CORE_IMPEXP  float64_T bigend_float64(const void *p);
M_CORE_IMPEXP  real32_T  bigend_real32( const void *p);
M_CORE_IMPEXP  real64_T  bigend_real64( const void *p);    


// --------------
// math functions 
// --------------

M_CORE_IMPEXP  int8_t          ipow(int8_t    , int8_t   );         // i ** j   i ^ j       functions
M_CORE_IMPEXP  int16_t         ipow(int16_t   , int16_t  );
M_CORE_IMPEXP  int32_t         ipow(int32_t   , int32_t  );
M_CORE_IMPEXP  int64_t         ipow(int64_t   , int64_t  );
M_CORE_IMPEXP  uint8_t         ipow(uint8_t   , uint8_t  );
M_CORE_IMPEXP  uint16_t        ipow(uint16_t  , uint16_t );
M_CORE_IMPEXP  uint32_t        ipow(uint32_t  , uint32_t );
M_CORE_IMPEXP  uint64_t        ipow(uint64_t  , uint64_t );
                
M_CORE_IMPEXP  int8_t          power(int8_t   , int8_t   );         // complete set of overloaded power functions
M_CORE_IMPEXP  int16_t         power(int16_t  , int16_t  );
M_CORE_IMPEXP  int32_t         power(int32_t  , int32_t  );
M_CORE_IMPEXP  int64_t         power(int64_t  , int64_t  );
M_CORE_IMPEXP  uint8_t         power(uint8_t  , uint8_t  );
M_CORE_IMPEXP  uint16_t        power(uint16_t , uint16_t );
M_CORE_IMPEXP  uint32_t        power(uint32_t , uint32_t );
M_CORE_IMPEXP  uint64_t        power(uint64_t , uint64_t );
M_CORE_IMPEXP  float32_T       power(float32_T, float32_T);
M_CORE_IMPEXP  float64_T       power(float64_T, float64_T);

 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///    File-oriented, I/O-oriented, and other system-oriented functions in core_sys.cpp
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   

// --------------------------
// logging-oriented functions
// --------------------------

M_CORE_IMPEXP  log_T         log_verbs(      void  );
M_CORE_IMPEXP  void          log_verbs(      log_T );

M_CORE_IMPEXP  log_T         log_statistics( void  );
M_CORE_IMPEXP  void          log_statistics( log_T );



// -------------------
// string input/output
// -------------------

M_CORE_IMPEXP  void           write_stdout(const std::wstring&     ); 
M_CORE_IMPEXP  void           write_stdout(const wchar_t *         ); 
M_CORE_IMPEXP  void           write_stderr(const std::wstring&     ); 
M_CORE_IMPEXP  void           write_stderr(const wchar_t *         );

M_CORE_IMPEXP  int            read_stdin(        std::wstring&     );
            
M_CORE_IMPEXP  void           write_note_stdout(const std::wstring&);
M_CORE_IMPEXP  void           write_note_stdout(const wchar_t *    );



// --------------------------------
// filesystem functions
// --------------------------------

M_CORE_IMPEXP  int            pathname_info(const std::wstring&, tfile_info_S&, api_err_S&      );
M_CORE_IMPEXP  int               tfile_info(const std::wstring&, tfile_info_S&, api_err_S&, bool);            // bool = true if UTF-32 file encoding is allowed, bool = false if UTF-32 is not allowed
M_CORE_IMPEXP  int               find_files(const std::wstring&, std::vector<std::wstring>&, bool = false); 

M_CORE_IMPEXP  int            detect_bom(void *, size_t, size_t&, bool=false );                               // default is not to check for erroneous 6-byte BOMs (UTF-8 converted to wchar_t)

M_CORE_IMPEXP  int            remove_bom( std::wstring&,          bool=false );                               // default is not to check for erroneous 6-byte BOMs (UTF-8 converted to wchar_t)
M_CORE_IMPEXP  int            remove_bom( std:: string&                      ); 
M_CORE_IMPEXP  int            remove_bom( wchar_t *    ,          bool=false );                               // default is not to check for erroneous 6-byte BOMs (UTF-8 converted to wchar_t)
M_CORE_IMPEXP  int            remove_bom( char    *                          ); 




// ----------------------
// DLL-oriented functions
// ---------------------- 

M_CORE_IMPEXP  int             load_dll(                  const std::wstring&  , HMODULE&);
M_CORE_IMPEXP  int             get_dll_function(HMODULE,  const std::string&   , void *& );
M_CORE_IMPEXP  int             get_dll_function(HMODULE,  const std::wstring&  , void *& );
M_CORE_IMPEXP  int             free_dll(        HMODULE                                  );



// ---------------------------
// debug and display functions
// ---------------------------

M_CORE_IMPEXP  void            display_api_err(   const api_err_S&   );  
M_CORE_IMPEXP  void            display_tfile_info(const tfile_info_S&); 
M_CORE_IMPEXP  void            show_hex(void *, size_t, const std::wstring& = L"", bool = true);
M_CORE_IMPEXP  void            display_locale(); 
M_CORE_IMPEXP  void            display_numerics(); 



// --------------------------------
// system and environment functions
// --------------------------------

M_CORE_IMPEXP  int64_t         query_performance_counter();
M_CORE_IMPEXP  float64_T       performance_counter_interval(int64_t);

M_CORE_IMPEXP  std::string     get_env(      const  char *       );
M_CORE_IMPEXP  std::wstring    get_env(      const wchar_t *     );
M_CORE_IMPEXP  std::wstring    get_env(      const std::wstring& );

M_CORE_IMPEXP  bool            is_env_set(   const  char   *     );
M_CORE_IMPEXP  bool            is_env_set(   const wchar_t *     );
M_CORE_IMPEXP  bool            is_env_set(   const std::wstring& ); 

M_CORE_IMPEXP  int             do_system(    const std::wstring& );
M_CORE_IMPEXP  int             do_popen(     const std::wstring&, std::wstring& );

M_CORE_IMPEXP  void            setup_environment(); 




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///    error handling functions in core_error.cpp
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   

// ----------------------------
// API error-oriented functions
// ----------------------------

M_CORE_IMPEXP  std::wstring   system_error_message(DWORD);
M_CORE_IMPEXP  std::wstring   format_api_op(api_op_T);
M_CORE_IMPEXP  std::wstring   format_non_syserr(non_syserr_T);
M_CORE_IMPEXP  void           api_err_system(    api_err_S&, api_op_T ,  uint32_t     , const std::wstring&);
M_CORE_IMPEXP  void           api_err_non_system(api_err_S&           ,  non_syserr_T , const std::wstring&);
M_CORE_IMPEXP  void           api_err_errno     (api_err_S&, api_op_T ,  errno_t      , const std::wstring&);



// ------------------------------
// system error message functions
// ------------------------------

M_CORE_IMPEXP  errno_t         get_current_errno(); 
M_CORE_IMPEXP  std::wstring    errno_string(errno_t);  



// -----------------------
// exit-oriented functions
// -----------------------

M_CORE_IMPEXP  void            do_exit(      int = -1);
M_CORE_IMPEXP  void            do__exit(     int = -1);
M_CORE_IMPEXP  void            do_abort(             );
M_CORE_IMPEXP  void            do_quick_exit(int = -1); 

 

// --------------------------------------------
// exception handling and other setup functions
// --------------------------------------------  

struct se_exception_S
{
    unsigned int          code                {0}; 
    EXCEPTION_RECORD      exception_record       ;
    CONTEXT               context_record         ;
};

M_CORE_IMPEXP  void setup_exception(bool = false); 

M_CORE_IMPEXP  std::wstring exception_string(); 

M_CORE_IMPEXP  void handle_invalid_parm(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t);
M_CORE_IMPEXP  void handle_exception();
M_CORE_IMPEXP  void handle_terminate();
M_CORE_IMPEXP  void handle_unexpected(); 
M_CORE_IMPEXP  void handle_se(unsigned int, EXCEPTION_POINTERS *);

extern "C" 
M_CORE_IMPEXP  void handle_signal(int);

 



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 
//
//      A      BBBB      CCCC    DDDD     EEEEE    FFFFF     GGGG     H   H    IIIII    JJJJJ    K   K    L        M   M     N   N     OOO     OOOOO    PPPP      QQQ     RRRR      SSSS    TTTTT                                                                                                                                            
//     A A     B   B    C        D   D    E        F        G         H   H      I         J     K  K     L        MM MM     NN  N    O   O    O   O    P   P    Q   Q    R   R    S          T                                
//    A   A    BBBB     C        D   D    EEEE     FFF      G  GG     HHHHH      I         J     KKK      L        M M M     N N N    O   O    O   O    PPPP     Q   Q    RRRR      SSS       T                                               
//    AAAAA    B   B    C        D   D    E        F        G   G     H   H      I         J     K  K     L        M   M     N  NN    O   O    O   O    P        Q   Q    R  R         S      T                              
//    A   A    BBBB      CCCC    DDDD     EEEEE    F         GGGG     H   H    IIIII    J  J     K   K    LLLLL    M   M     N   N     OOO     OOOOO    P         QQQ     R   R    SSSS       T                                                             
//                                                                                       JJ                                                                          Q                   
//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//
//    U   U    V   V    W   W    X   X    Y   Y    ZZZZZ     000        1      2222     3333     4  4        4     55555      666     77777     888      999               
//    U   U    V   V    W   W     X X      Y Y        Z     0   0      11          2        3    4  4       44     5         6           7     8   8    9   9                             
//    U   U     V V     W W W      X        Y        Z      0 0 0       1          2     333     44444     4 4     5555      6666       7       888      9999                                        
//    U   U     V V     WW WW     X X       Y       Z       0   0       1        2          3       4     44444        5     6   6     7       8   8        9                     
//     UUU       V      W   W    X    X     Y      ZZZZZ     000      11111    22222    3333        4        4      555       666     7         888      999                                            
//            
// 
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//                                                                     @          @       @       @  @                                  @         @     @@@@@                @           
//                                                   @      @@@@        @        @        @       @  @        @     @@  @     @ @      @@@@     @@@@   @     @   @   @     @@@@                                                                                                     
//                                @        @         @          @       @        @                           @      @@ @     @@@@@    @ @      @  @    @ @@@ @    @ @     @  @                                                         
//    @@@@@                                          @       @@@        @        @                          @         @       @ @      @@@     @  @    @ @ @ @   @@@@@     @@@@                                                                
//                                                                      @        @                         @         @ @@    @@@@@      @ @    @  @    @ @@@@     @ @     @  @                                                  
//              @         @       @        @         @       @          @        @                        @         @  @@     @ @     @@@@      @@@@   @         @   @     @@@@                                                                 
//                       @                @                            @          @                                                     @         @     @@@@@@               @                                       
//                                                                                                                                                                               =========
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫



    // •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
    // 
    // ••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••• 
                                                                                                                                                                                                       
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
                                                                                                                                                                                                                                                
// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                
// ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀  ⁀                                                                                                                                                                       
                                                                                                                                                                                                                                                
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                                                                                                              
//    
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
////▕
////▕
////▕   
////▕
////▕
////▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
/////▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔



////▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
/////▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔


////▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
////╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
////╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
////▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔




////▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
////╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
////╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
////╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
////╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
////▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔




//╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳║
//║╳╳╳╳╳╳╳╳║
//║╳╳╳╳╳╳╳╳║
//║╳╳╳╳╳╳╳╳║
//║╳╳╳╳╳╳╳╳║   
//║╳╳╳╳╳╳╳╳║
//║╳╳╳╳╳╳╳╳║
//║╳╳╳╳╳╳╳╳║   
//║╳╳╳╳╳╳╳╳║
//║╳╳╳╳╳╳╳╳║
//║╳╳╳╳╳╳╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//___________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|          
//\/\/\/\|           
//\/\/\/\|          
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|
//\/\/\/\\____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\









////▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
////▎
////▎
////▎
////▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔



//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  


/*  1-based column numbering: 

00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112
00000000011111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999900
12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567800

*/

//≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋
//≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋≋


//∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰
//∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰∰

//∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭
//∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭∭


//≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣
//≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣


//•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//•••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••


//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫
//⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫⧫


//●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●
//●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●●


//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□


//▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚
//▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚▚


//◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼
//◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼◼


//▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣
//▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣


//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉


//▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊
//▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊▊


//████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
//████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████


//▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔

//══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´

//¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨

//______________________________________________________________________________________________________________________


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=======================================================================================================================

//------------------------------------------------------------------------------------------------------------------------

//[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

//««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««

//»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

//«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»

//........................................................................................................................

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷÷

//()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()

//(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)((_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_)(_

//{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}

//{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷{}÷

//(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)(•)

//(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)(O)

 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////