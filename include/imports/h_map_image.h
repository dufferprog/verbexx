// h_map_image.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////            =============
////            h_map_image.h -- functions and assorted items for map_image.cpp  
////            =============
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
#pragma once
#include "h_map_types.h"
#include "h_draw_types.h"


// MACROs to adjust raw row/col info to/from map projection from/to x/y in outimage_S buffer, based on current map_window
// --------------------------------------------------------------------------------------------------------------------

// input = map row/col  -- output = outimage x/y 

#define M_map_window_x(map_col, im, md)  ( (Gdiplus::REAL)map_col + (Gdiplus::REAL)(im.curr_window_left_col) - (Gdiplus::REAL)(md.map_window_left_col) )
#define M_map_window_y(map_row, im, md)  ( (Gdiplus::REAL)map_row + (Gdiplus::REAL)(im.curr_window_top_row ) - (Gdiplus::REAL)(md.map_window_top_row ) )

// input = outimage x/y    -- outpout = map row/col

#define M_map_window_c(outimage_x, im, md)  ( (Gdiplus::REAL)outimage_x + (Gdiplus::REAL)(md.map_window_left_col) - (Gdiplus::REAL)(im.curr_window_left_col) )
#define M_map_window_r(outimage_y, im, md)  ( (Gdiplus::REAL)outimage_y + (Gdiplus::REAL)(md.map_window_top_row ) - (Gdiplus::REAL)(im.curr_window_top_row ) )


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------
//    parms for set_map_window() -- @SET_MAP_WINDOW verb
//------------------------------------------------------

struct set_map_window_parm_S
{
    bool                    display               { false               } ;       // true -- do set_map_window()  debug display 
    rc_T                    outimage_top_row      { 0                   } ;       // top of map window is drawn starting at this row in outimage buffer 
    rc_T                    outimage_left_column  { 0                   } ;       // left side of map window is drawn starting at this column in outimage buffer
    rc_T                    map_top_row           { 0                   } ;       // top row of map to be drawn (window -- default is start at top of map) 
    rc_T                    map_left_column       { 0                   } ;       // left row of map to be drawn (window -- default is start at 1st column in map) 
    rc_T                    map_width             { 0                   } ;       // number of map columns to draw (0 = all -- default) 
    rc_T                    map_height            { 0                   } ;       // number of map rows to draw    (0 = all -- default) 
};



//-----------------------------------------
//    parms for put_map() -- @PUT_MAP verb
//-----------------------------------------

struct put_map_parm_S
{
    bool                    display            { false                 } ;       // true -- do put_map()  debug display 
    bool                    shaded_relief      { false                 } ;       // default is no slope processing (shaded relief)
    bool                    land_hypsometry    { true                  } ;       // default is land hypsometry
    bool                    ocean_bathymetry   { false                 } ;       // default is no ocean bathymetry      
};



//---------------------------------------------------------
//    size/color parms for put_ramp() 
//---------------------------------------------------------

struct ramp_put_parm_S
{
    bool                    display            { false               } ;       // default = no debug display 
    bool                    ocean              { false               } ;       // true: draw ocean ramp, false: draw land ramp
    rc_T                    top_row            { 0                   } ;       // top/starting row for ramp
    rc_T                    left_col           { 0                   } ;       // left/starting column for ramp
    rc_T                    width              { 0                   } ;       // overall width of ramp
    rc_T                    height             { 0                   } ;       // overall height or ramp 
    rc_T                    border             { 0                   } ;       // thickness of border  (0 = none)
    rc_T                    margin             { 1                   } ;       // thickness of margin (top/bottom/right/left are all same)
    rc_T                    text_space         { 1                   } ;       // horizontal space between box and start of text 
    rgb_S                   background_color   { 255, 255, 255       } ;       // color of background
    rgb_S                   text_color         { 0  , 0  , 0         } ;       // color of elevation text
    rgb_S                   border_color       { 0  , 0  , 0         } ;       // color of border
};   



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------------
// map annotation functions  
// ------------------------

int    draw_ll_line(            outimage_S&, const mapdata_S&, const Gdiplus::Pen *                               , real_T, real_T, real_T, real_T, real_T,                 const mapparm_S&);
int    draw_ll_curve(           outimage_S&, const mapdata_S&, const Gdiplus::Pen *                               , real_T, real_T, real_T, real_T, real_T,                 const mapparm_S&);
int    draw_ll_line(            outimage_S&, const mapdata_S&, const rgb_S&, alpha_T, int32_t                     , real_T, real_T, real_T, real_T, real_T,                 const mapparm_S&);
int    draw_ll_curve(           outimage_S&, const mapdata_S&, const rgb_S&, alpha_T, int32_t                     , real_T, real_T, real_T, real_T, real_T,                 const mapparm_S&);
                               
int    draw_ll_lines(           outimage_S&, const mapdata_S&, const Gdiplus::Pen *, const std::vector<real_T>&, const std::vector<real_T>&, real_T,                        const mapparm_S&);
                               
int    draw_ll_text(            outimage_S&, const mapdata_S&, const font_S&, text_format_S&, const std::wstring& , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_ll_text(            outimage_S&, const mapdata_S&,                                const std::wstring& , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_llfix_text(         outimage_S&, const mapdata_S&, const font_S&, text_format_S&, const std::wstring& , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_llfix_text(         outimage_S&, const mapdata_S&,                                const std::wstring& , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_ll_str(             outimage_S&, const mapdata_S&, const font_S&, text_format_S&, const std::wstring& , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_ll_str(             outimage_S&, const mapdata_S&,                                const std::wstring& , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_llfix_str(          outimage_S&, const mapdata_S&, const font_S&, text_format_S&, const std::wstring& , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_llfix_str(          outimage_S&, const mapdata_S&,                                const std::wstring& , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_ll_char(            outimage_S&, const mapdata_S&, const font_S&, text_format_S&, const WCHAR *       , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_ll_char(            outimage_S&, const mapdata_S&,                                const WCHAR *       , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_llfix_char(         outimage_S&, const mapdata_S&, const font_S&, text_format_S&, const WCHAR *       , real_T, real_T, real_T,                                 const mapparm_S&);
int    draw_llfix_char(         outimage_S&, const mapdata_S&,                                const WCHAR *       , real_T, real_T, real_T,                                 const mapparm_S&);
                               
int    draw_ll_path(            outimage_S&, const mapdata_S&, const Gdiplus::Pen *  , const Gdiplus::Brush *, const Gdiplus::GraphicsPath *, llr_T lat, llr_T lon,         const mapparm_S&);
int    fill_ll_path(            outimage_S&, const mapdata_S&, const Gdiplus::Brush *, const Gdiplus::Brush *, const Gdiplus::GraphicsPath *, llr_T lat, llr_T lon,         const mapparm_S&);
int    draw_llfix_path(         outimage_S&, const mapdata_S&, const Gdiplus::Pen *  , const Gdiplus::Brush *, const Gdiplus::GraphicsPath *, llr_T lat, llr_T lon,         const mapparm_S&);
int    fill_llfix_path(         outimage_S&, const mapdata_S&, const Gdiplus::Brush *, const Gdiplus::Brush *, const Gdiplus::GraphicsPath *, llr_T lat, llr_T lon,         const mapparm_S&);


real_T ll_line_rotation(           const mapdata_S&, llr_T, llr_T, real_T, real_T,                                                                                          const mapparm_S&); 


// --------------------------------------------
// mapimage_S and outimage_S combined functions
// --------------------------------------------

int    create_map_outimage(     outimage_S&, const mapdata_S&,                                  const outimage_create_parm_S&,                                              const mapparm_S&);
int    set_map_window(          outimage_S&,       mapdata_S&,                                  const set_map_window_parm_S& ,                                              const mapparm_S&);    
int    verify_map_window( const outimage_S&, const mapdata_S&,                                                                                                              const mapparm_S&);
int    put_map(                 outimage_S&, const mapdata_S&,             ramp_S&,             const put_map_parm_S&,                                                      const mapparm_S&);    // can't use const ramp_s& -- causes unresolved
int    put_ramp(                outimage_S&,                               ramp_S&,             const ramp_put_parm_S&,                                                     const mapparm_S&);    // can't use const ramp_s& -- causes unresolved
int    draw_ramp(               outimage_S&,                               ramp_S&,             const ramp_put_parm_S&,                                                     const mapparm_S&);    // can't use const ramp_s& -- causes unresolved                                                                                                                                                                    


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////