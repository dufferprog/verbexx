// h_draw_types.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////
////          ==============
////          h_draw_types.h -- forward declarations of types used in draw_xxx() functions 
////          ==============
////
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h__types.h"             // draw is dependent on common types


// types specific to windows, drawing, images, rasters, bitmaps, etc.
// ------------------------------------------------------------------

typedef int32_t    rc_T;          // type for rows, cols, linear dimensions, etc.                                
typedef real_T     rcf_T;         // type for fractional rows, cols, linear dimensions, etc. (real_T)            
typedef real_T     xy_T;          // type for X and Y coordinates on window                                      
typedef uint8_t    bitmask8_T;    // type for 8-bit bitmasks used for drawing letters                            
typedef uint8_t    alpha_T;       // type for alpha-channel parm                                                 


// rgb_S -- rgb values i outimage_S, etc. 
// --------------------------------------

struct rgb_S                    // r, g, b triple in outimage file and elsewhere       
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}; 

#define M_make_rgb(v, x, y, z) {v.r = (uint8_t)(x); v.g = (uint8_t)(y); v.b = (uint8_t)(z);}          




// -----------------------------------------------------------------------
// forward type declarations (incomplete structure/union/class/enum types)
// -----------------------------------------------------------------------

class                        canvas_C;   
struct                       draw_image_parm_S;
struct                       font_S;  
struct                       format_text_parm_S;
struct                       outimage_S;
struct                       outimage_create_parm_S;
struct                       text_format_S;
struct                       tiff24_parm_S;
struct                       transform_S;                
class                        window_C;  
struct                       write_outimage_parm_S;   

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////