// h_draw_outimage.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////
////          =================
////          h_draw_outimage.h -- functions for drawing/setting outimage data, and writing it to bmp/tiff files 
////          =================
////
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_draw_canvas.h"
#include "h_draw_tiff.h"


//------------------------------------------------
//     "Transparent" RGB colors for bitmap copying
//------------------------------------------------

namespace cx
{
constexpr uint8_t bmp_transp_r  { 0 }; 
constexpr uint8_t bmp_transp_g  { 1 };
constexpr uint8_t bmp_transp_b  { 2 };
}

//----------------------------------------------------------
//     RGB outimage data -- contains rows * cols rgb_S items  (with imbedded GDI+ graphics structure)
//----------------------------------------------------------

struct outimage_S
{ 
 public:  

    bool                 curr_window_valid      {false };         // true when set_map_window has completed
    rc_T                 curr_window_height     { 0    };         // height of current window 
    rc_T                 curr_window_width      { 0    };         // width of current window 
    rc_T                 curr_window_top_row    { 0    };         // top row of current window                
    rc_T                 curr_window_bottom_row { 0    };         // bottom row of current window  
    rc_T                 curr_window_left_col   { 0    };         // left column of current window                
    rc_T                 curr_window_right_col  { 0    };         // right column of current window  

    canvas_C             canvas                 {      };         // associated GDI+ graphics structure (default initialized)
                                              
    rc_T                 rows                   { 0    };         // number of rows in data area 
    rc_T                 cols                   { 0    };         // number of columns in data area 
    int32_t              row_l                  { 0    };         // = 3 * cols -- no padding
    uint64_t             pixels                 { 0    };         // = rows * cols
    uint64_t             bytes                  { 0    };         // = 3 * pixels
    uint64_t             data_l                 { 0    };         // number of data bytes in buffer (3x number of pixels)
    buffer_C<rgb_S>      buf                    {      };         // buffer for RGB data
};


#define M_outimage_rgb(i,r,c) ((i.buf.p)[(r) * (i.cols) + (c)])

#define M_outimage_r(  i,r,c) (M_outimage_rgb(i, r, c).r)   
#define M_outimage_g(  i,r,c) (M_outimage_rgb(i, r, c).g)  
#define M_outimage_b(  i,r,c) (M_outimage_rgb(i, r, c).b)  


//-------------------------------
//    parms for create_outimage() 
//-------------------------------

struct outimage_create_parm_S
{
    bool                    display            { false               } ;       // true -- do graticule debug display 
    rc_T                    rows               { 0                   } ;       // number of rows in outimage
    rc_T                    cols               { 0                   } ;       // number of columns in outimage
};


//----------------------------------------------
//    parms for draw_image() -- @DRAW_IMAGE verb
//----------------------------------------------

struct draw_image_parm_S
{
    bool                    display               { false               } ;       // true -- do draw_image()  debug display 
    rc_T                    dest_row              { -1                  } ;       // destination top row for image 
    rc_T                    dest_col              { -1                  } ;       // destination left column for image
    rc_T                    dest_w                { -1                  } ;       // destination width for image 
    rc_T                    dest_h                { -1                  } ;       // destination height column for image
    rc_T                    src_row               { -1                  } ;       // source top row for image 
    rc_T                    src_col               { -1                  } ;       // source left column for image
    rc_T                    src_w                 { -1                  } ;       // source width for image 
    rc_T                    src_h                 { -1                  } ;       // source height for image 
    std::wstring            filename              {                     } ;       // filename containing image data
    bool                    trans_color_valid     { false               } ;       // true -- trans_color was specified
    rgb_S                   trans_color1          { 0  , 0  , 0         } ;       // lowest  color for transparent drawing
    rgb_S                   trans_color2          { 0  , 0  , 0         } ;       // highest color for transparent drawing
};


//------------------------------------------------
//    parms for format_text() -- @FORMAT_TEXT verb
//------------------------------------------------

struct format_text_parm_S
{
    bool                    display               { false               } ;       // true -- do format_text() debug display 
    std::wstring            str                   {                     } ;       // string to be formatted
    rc_T                    top                   { -1                  } ;       // top row of rectangle 
    rc_T                    left                  { -1                  } ;       // left column of rectangle
    rc_T                    width                 { -1                  } ;       // width of rectangle 
    rc_T                    height                { -1                  } ;       // height of rectangle
};


//-------------------------------------------
//    output file parms for writeout_tiff24() 
//-------------------------------------------

struct tiff24_parm_S
{
    bool                    display            { false                 } ;       // default = no debug display 
    uint32_t                rows_per_strip     { 25                    } ;       // TIFF file rows_per_strip value 
    uint32_t                x_resolution       { 1200                  } ;       // X resolution for TIFF file
    uint32_t                y_resolution       { 1200                  } ;       // Y resolution for TIFF file
    uint16_t                resolution_unit    { tiff_N::C_ru_inch     } ;       // TIFF file resolution unit (inch or cm)
    uint16_t                orientation        { tiff_N::C_or_top_left } ;       // TIFF orientation -- top/left, etc. 
};


//------------------------------------------
//    output file parms for write_outimage() 
//------------------------------------------

struct write_outimage_parm_S
{
    bool                    display            { false                 } ;       // default = no debug display 
    bool                    tiff_24            { true                  } ;       // default = 24-bit TIFF file
    std::wstring            folder             {                       } ;       // folder name for output outimage file
    std::wstring            filename           {                       } ;       // complete filename for output outimage file -- can include folder names start
    tiff24_parm_S           tiff24_parm        {                       } ;       // parms for writeout_tiff24 (if needed)
};


//---------------------------------------------------------------------------------------------------------------------
//     outimage-oriented function prototypes
//---------------------------------------------------------------------------------------------------------------------

typedef uint8_t bitmask8_T; 

M_DRAW_IMPEXP outimage_S *find_outimage(   int64_t   );      // locate outimage in repository with passed-in ID
M_DRAW_IMPEXP outimage_S *alloc_outimage(  int64_t   );      // make sure outimage is allocated in repository for passed-in ID (passes back simple pointer address of outimage_S) 
M_DRAW_IMPEXP void        free_outimage(   int64_t   );      // make sure outimage is not allocated in repository for passed-in ID (caller should get rid of everything in outimage_S first) 


M_DRAW_IMPEXP  int      writeout_tiff24(       const outimage_S&,                          const std::wstring&,      const tiff24_parm_S&                                   ); 
M_DRAW_IMPEXP  int      writeout_bmp24(        const outimage_S&,                          const std::wstring&                                                              ); 

M_DRAW_IMPEXP  int      create_outimage(             outimage_S&,                                                    const outimage_create_parm_S&                          );
M_DRAW_IMPEXP  int      destroy_outimage(            outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      write_outimage(        const outimage_S&,                                                    const write_outimage_parm_S&                           );

M_DRAW_IMPEXP  int      set_clip_window(             outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      reset_clip(                  outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      put_graphics(                outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      get_graphics(                outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      put_point(                   outimage_S&,                      rc_T, rc_T,                   const rgb_S&                                           );
M_DRAW_IMPEXP  int      put_string8(                 outimage_S&, const wchar_t *   ,  rc_T, rc_T,                   const rgb_S&                                           );
M_DRAW_IMPEXP  int      put_box(                     outimage_S&,                      rc_T, rc_T, rc_T, rc_T,       const rgb_S&                                           );
M_DRAW_IMPEXP  int      put_frame(                   outimage_S&,                      rc_T, rc_T, rc_T, rc_T, rc_T, const rgb_S&                                           );
                                                                                                                                                         
M_DRAW_IMPEXP  int      draw_line(                   outimage_S&,               const rgb_S&               , rc_T, rc_T, rc_T, rc_T, rc_T                                   );
M_DRAW_IMPEXP  int      draw_box(                    outimage_S&,               const rgb_S&               , rc_T, rc_T, rc_T, rc_T                                         );
M_DRAW_IMPEXP  int      draw_frame(                  outimage_S&,               const rgb_S&               , rc_T, rc_T, rc_T, rc_T, rc_T                                   );
M_DRAW_IMPEXP  int      draw_ring(                   outimage_S&,               const rgb_S&               , rc_T, rc_T, rc_T, rc_T                                         ); 
M_DRAW_IMPEXP  int      draw_circle(                 outimage_S&,               const rgb_S&               , rc_T, rc_T, rc_T                                               ); 
                                                                                        
M_DRAW_IMPEXP  int      draw_line(                   outimage_S&,               const rgb_S&, alpha_T      , rc_T, rc_T, rc_T, rc_T, rc_T                                   );
M_DRAW_IMPEXP  int      draw_box(                    outimage_S&,               const rgb_S&, alpha_T      , rc_T, rc_T, rc_T, rc_T                                         );
M_DRAW_IMPEXP  int      draw_frame(                  outimage_S&,               const rgb_S&, alpha_T      , rc_T, rc_T, rc_T, rc_T, rc_T                                   );
M_DRAW_IMPEXP  int      draw_ring(                   outimage_S&,               const rgb_S&, alpha_T      , rc_T, rc_T, rc_T, rc_T                                         ); 
M_DRAW_IMPEXP  int      draw_circle(                 outimage_S&,               const rgb_S&, alpha_T      , rc_T, rc_T, rc_T                                               ); 
M_DRAW_IMPEXP  int      draw_image(                  outimage_S&,                                                    const draw_image_parm_S&                               );  

M_DRAW_IMPEXP  int      set_font(                    outimage_S&, const std::wstring&                      , real_T                                                         ); 
M_DRAW_IMPEXP  int      set_font(                    outimage_S&, const std::wstring&                      , real_T, uint32_t                 , bool                        ); 
M_DRAW_IMPEXP  int      set_font(                    outimage_S&, const std::wstring&                      , real_T, uint32_t, const rgb_S&   , bool                        ); 
M_DRAW_IMPEXP  int      set_font_size(               outimage_S&,                                            real_T                                                         ); 
M_DRAW_IMPEXP  int      set_text_color(              outimage_S&,               const rgb_S&                                                                                );
M_DRAW_IMPEXP  int      set_text_color(              outimage_S&,               const rgb_S&, alpha_T                                                                       );
M_DRAW_IMPEXP  int      set_text_background_color(   outimage_S&,               const rgb_S&                                                                                );
M_DRAW_IMPEXP  int      set_text_background_color(   outimage_S&,               const rgb_S&, alpha_T                                                                       );
M_DRAW_IMPEXP  int      set_no_text_background(      outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      draw_text(                   outimage_S&, const std::wstring&                      , rc_T, rc_T                                                     ); 
M_DRAW_IMPEXP  int      draw_text(                   outimage_S&, const std::wstring&                      , rc_T, rc_T, real_T                                             ); 
M_DRAW_IMPEXP  int      draw_text(                   outimage_S&, const std::wstring&                      , rc_T, rc_T, real_T, real_T, real_T, real_T, real_T             );  
M_DRAW_IMPEXP  int      draw_text(                   outimage_S&, const std::wstring&                      , rc_T, rc_T, real_T, real_T, real_T, real_T, real_T, real_T     );  
M_DRAW_IMPEXP  int      format_text(                 outimage_S&,                                                    const format_text_parm_S&                              );
                                       
M_DRAW_IMPEXP  int      display_installed_fonts(     outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      display_private_fonts(       outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      remove_private_fonts(        outimage_S&                                                                                                            );
M_DRAW_IMPEXP  int      add_private_font(            outimage_S&, const std::wstring&                                                                                       );
M_DRAW_IMPEXP  int      add_private_fonts(           outimage_S&, const std::wstring&, bool                                                                                 );                                     
M_DRAW_IMPEXP  int      get_font_count(              outimage_S&, bool, int32_t&                                                                                            );
M_DRAW_IMPEXP  int      get_font_name(               outimage_S&, bool, int32_t, std::wstring&                                                                              ); 


////////////////////////////////////////////////////////////////

#define MMM 1
#define M_bitmask8(a,b,c,d,e,f,g,h)  (128*a + 64*b + 32*c + 16*d + 8*e + 4*f + 2*g + h) // last bit (h) is always 0 for normal letter/number



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////