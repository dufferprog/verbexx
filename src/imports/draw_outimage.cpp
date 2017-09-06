// draw_outimage.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =================
////     draw_outimage.cpp -- map outimage drawing and outputting functions (to .bmp or .tif)
////     =================
////                      
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "h__include.h"
#pragma hdrstop("../pch/pch_std.pch")

#define M_IN_DRAW_DLL

#include "h__common.h"

#include "h_draw_types.h"
#include "h_draw_canvas.h"
#include "h_draw_outimage.h"
#include "h_draw_tiff.h"



//  Internal functions
//  ------------------

static int  put_DIBSection(outimage_S& , void *, rc_T, rc_T, rc_T, rc_T                                          );
static int  get_DIBSection(outimage_S& , void *, rc_T, rc_T, rc_T, rc_T                                          );
static void put_letter8(   outimage_S& , wchar_t           ,  rc_T, rc_T,            const rgb_S&                );
static void put_bitmask8(  outimage_S& , const bitmask8_T *,  rc_T, rc_T, rc_T,      const rgb_S&                );
static void init_bm(void); 



// ==================================================================================================================
//                                       Static variables
// ==================================================================================================================

namespace static_N
{
// outimage_S repository

static std::map<int64_t, std::shared_ptr<outimage_S>> outimages { }; 


// character bitamps

static bool    bm_init = false;

static uint8_t bm_blank[16];
static uint8_t bm_minus[16];
static uint8_t bm_plus[16];
static uint8_t bm_b[16];
static uint8_t bm_0[16];
static uint8_t bm_1[16]; 
static uint8_t bm_2[16];
static uint8_t bm_3[16];
static uint8_t bm_4[16];
static uint8_t bm_5[16]; 
static uint8_t bm_6[16];
static uint8_t bm_7[16];
static uint8_t bm_8[16];
static uint8_t bm_9[16];
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        find_outimage() -- find outimage(n) in repository (return nullptr, if not there)  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
outimage_S* find_outimage(int64_t outimage_id) try
{
    // make sure there is exactly one outimage in repository with caller's ID
    // ----------------------------------------------------------------------

     auto outimage_ct = static_N::outimages.count(outimage_id); 

     if (outimage_ct <= 0)
     {
         M__(M_out(L"find_outimage() -- outimage(%d) not in repository") % outimage_id;) 
         return nullptr; 
     }
     else
     if (outimage_ct > 1)
     {
         M_out_emsg(L"find_outimage() -- invalid outimages.count(%d) = %d") % outimage_id % outimage_ct;  
         return nullptr; 
     }


     // one shared_ptr<outimage_S> found -- return outimage_S address as simple pointer
     // -------------------------------------------------------------------------------

     M__(M_out(L"find_outimage() -- outimage(%d) at %p found in repository") % outimage_id % static_N::outimages.at(outimage_id).get();) 
     return static_N::outimages.at(outimage_id).get();
}
M_endf 




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        alloc_outimage() -- make sure outimage is allocated in repository for this ID  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
outimage_S* alloc_outimage(int64_t outimage_id) try
{
    // pass back any existing outimage_S in repository
    // -----------------------------------------------

    outimage_S *p = find_outimage(outimage_id);

    if (p != nullptr)
    {
        M__(M_out(L"alloc_outimage() -- existing outimage(%d) in repository at %p being returned") % outimage_id % p;) 
        return p;
    }


    // need to allocate new (default initialized) outimage_S and add to the repository
    //--------------------------------------------------------------------------------

    std::shared_ptr<outimage_S> sp(new outimage_S);     // this sp should go away when function returns
    static_N::outimages.emplace(outimage_id, sp);       // this sp should stay around until this ID is deallocated

    M__(M_out(L"alloc_outimage() -- new outimage(%d) was allocated at %p and added to repository") % outimage_id % sp.get();)


    // pass back simple address of newly-allocated outimage_S

    return sp.get();                                        
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        free_outimage() -- make sure no outimage is allocated in repository for this ID  
//|| 
//||                           (caller must have gotten rid of everything in the outimage_S before calling) 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
void free_outimage(int64_t outimage_id) try
{
    // get rid of any existing outimage_S in repository
    // ------------------------------------------------

    if (static_N::outimages.count(outimage_id) > 0)
    {
        M__(M_out(L"free_outimage() -- outimage(%d) being erased from repository") % outimage_id;)
        static_N::outimages.erase(outimage_id);   
    }
    else
    {
        M__(M_out(L"free_outimage() -- outimage(%d) not allocated in repository") % outimage_id;)
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
//||        draw_xxx() -- draw into GDI+ DIBSections  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// versions without alpha:
// -----------------------

M_DRAW_IMPEXP
int draw_line(outimage_S& outimage, const rgb_S& rgb, rc_T th, rc_T row1, rc_T col1, rc_T row2, rc_T col2) try 
{
    return outimage.canvas.draw_line(rgb, th, row1, col1, row2, col2);
}
M_endf
  

M_DRAW_IMPEXP
int draw_box(outimage_S& outimage, const rgb_S& rgb, rc_T row, rc_T col, rc_T w, rc_T h) try
{
    return outimage.canvas.draw_box(rgb, row, col, w, h); 
}
M_endf


M_DRAW_IMPEXP
int draw_frame(outimage_S& outimage, const rgb_S& rgb, rc_T th, rc_T row, rc_T col, rc_T w, rc_T h) try 
{
    return outimage.canvas.draw_frame(rgb, th, row, col, w, h);
}
M_endf
      

M_DRAW_IMPEXP
int draw_ring(outimage_S& outimage, const rgb_S& rgb, rc_T th, rc_T row, rc_T col, rc_T r) try 
{
    return outimage.canvas.draw_ring(rgb, th, row, col, r);
}
M_endf


M_DRAW_IMPEXP
int draw_circle(outimage_S& outimage, const rgb_S& rgb, rc_T row, rc_T col, rc_T r) try 
{
    return outimage.canvas.draw_circle(rgb, row, col, r);
}
M_endf


// versions with alpha:
// --------------------

M_DRAW_IMPEXP
int draw_line(outimage_S& outimage, const rgb_S& rgb, alpha_T alpha, rc_T th, rc_T row1, rc_T col1, rc_T row2, rc_T col2) try 
{
    return outimage.canvas.draw_line(rgb, alpha, th, row1, col1, row2, col2);
}
M_endf
  

M_DRAW_IMPEXP
int draw_box(outimage_S& outimage, const rgb_S& rgb, alpha_T alpha, rc_T row, rc_T col, rc_T w, rc_T h) try
{
    return outimage.canvas.draw_box(rgb, alpha, row, col, w, h); 
}
M_endf


M_DRAW_IMPEXP
int draw_frame(outimage_S& outimage, const rgb_S& rgb, alpha_T alpha, rc_T th, rc_T row, rc_T col, rc_T w, rc_T h) try 
{
    return outimage.canvas.draw_frame(rgb, alpha, th, row, col, w, h);
}
M_endf
      

M_DRAW_IMPEXP
int draw_ring(outimage_S& outimage, const rgb_S& rgb, alpha_T alpha, rc_T th, rc_T row, rc_T col, rc_T r) try 
{
    return outimage.canvas.draw_ring(rgb, alpha, th, row, col, r);
}
M_endf


M_DRAW_IMPEXP
int draw_circle(outimage_S& outimage, const rgb_S& rgb, alpha_T alpha, rc_T row, rc_T col, rc_T r) try 
{
    return outimage.canvas.draw_circle(rgb, alpha, row, col, r);
}
M_endf

   
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    draw_image() -- read in image file and draw it to the outimage (use GDI+)
//    ============
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int draw_image(outimage_S& outimage, const draw_image_parm_S& di_parm) try 
{
    M_y(M_out(L"draw_image(): filename = \"%S\"   dest_row = %d   dest_col = %d") % di_parm.filename % di_parm.dest_row % di_parm.dest_col;) 


    // open image file -- check for errors
    // -----------------------------------

    Gdiplus::Image image { di_parm.filename.c_str() };

    M_checkls_ret(image, L"draw_image() -- Gdiplus::Image constructor")  


    // get width and height frfom loaded image
    // ---------------------------------------

    auto image_width  = image.GetWidth();
    auto image_height = image.GetHeight();


    if (di_parm.display == true)
    {
        auto image_type   = image.GetType();
        auto image_flags  = image.GetFlags(); 
        auto image_res_h  = image.GetHorizontalResolution(); 
        auto image_res_v  = image.GetVerticalResolution(); 

        Gdiplus::RectF bounds_rect {}; 
        Gdiplus::Unit  bounds_unit {}; 

        image.GetBounds(&bounds_rect, &bounds_unit);
       
        M_out(L"draw_image():    filename                 = \"%S\""                  ) % di_parm.filename                                                          ; 
        M_out(L"draw_image():    dest_row                 = %d"                      ) % di_parm.dest_row                                                          ; 
        M_out(L"draw_image():    dest_col                 = %d"                      ) % di_parm.dest_col                                                          ; 
        M_out(L"draw_image():    dest_width               = %d"                      ) % di_parm.dest_w                                                            ; 
        M_out(L"draw_image():    dest_height              = %d"                      ) % di_parm.dest_h                                                            ; 
        M_out(L"draw_image():    src_row                  = %d"                      ) % di_parm.src_row                                                           ; 
        M_out(L"draw_image():    src_col                  = %d"                      ) % di_parm.src_col                                                           ; 
        M_out(L"draw_image():    src_width                = %d"                      ) % di_parm.src_w                                                             ; 
        M_out(L"draw_image():    src_height               = %d"                      ) % di_parm.src_h                                                             ; 
        M_out(L"draw_image():    trans_color_valid        = %s"                      ) % M_bool_cstr(di_parm.trans_color_valid)                                    ; 
        M_out(L"draw_image():    trans_color1             = %d %d %d"                ) % di_parm.trans_color1.r % di_parm.trans_color1.g % di_parm.trans_color1.b  ; 
        M_out(L"draw_image():    trans_color2             = %d %d %d"                ) % di_parm.trans_color2.r % di_parm.trans_color2.g % di_parm.trans_color2.b  ; 
        M_out(L"draw_image():    image_type               = %d"                      ) % (int32_t)image_type                                                       ; 
        M_out(L"draw_image():    image_width              = %d"                      ) % (int32_t)image_width                                                      ; 
        M_out(L"draw_image():    image_height             = %d"                      ) % (int32_t)image_height                                                     ; 
        M_out(L"draw_image():    image_flags              = %08X"                    ) % (uint32_t)image_flags                                                     ; 
        M_out(L"draw_image():    image_horiz_resolution   = %.6f"                    ) % (double)image_res_h                                                       ; 
        M_out(L"draw_image():    image_vert_resolution    = %.6f"                    ) % (double)image_res_v                                                       ; 
        M_out(L"draw_image():    bounds_unit              = %d"                      ) % (int32_t)bounds_unit                                                      ; 
        M_out(L"draw_image():    bounds_rect.X            = %.6f"                    ) % bounds_rect.X                                                             ; 
        M_out(L"draw_image():    bounds_rect.Y            = %.6f"                    ) % bounds_rect.Y                                                             ; 
        M_out(L"draw_image():    bounds_rect.Width        = %.6f"                    ) % bounds_rect.Width                                                         ; 
        M_out(L"draw_image():    bounds_rect.Height       = %.6f"                    ) % bounds_rect.Height                                                        ; 
    }
     

    // expected option combinations
    // ----------------------------
    //
    //  1 - dest_row/col only  (always present)
    //  2 - dest_w/h present  (no src_row/col/w/h)
    //  3 - src row/col/w/h present (no dest w/h)
    //  4 - dest_w/h + src_row/col/w/h present
    //  5 - trans_color present (with any of the above combos)
    //

    int32_t dest_col { di_parm.dest_col };       // always present
    int32_t dest_row { di_parm.dest_row };       // always present
    int32_t dest_w   { di_parm.dest_w   };       // optional -- can be -1 
    int32_t dest_h   { di_parm.dest_h   };       // optional -- can be -1 
    int32_t src_col  { di_parm.src_col  };       // optional -- can be -1
    int32_t src_row  { di_parm.src_row  };       // optional -- can be -1
    int32_t src_w    { di_parm.src_w    };       // optional -- can be -1 
    int32_t src_h    { di_parm.src_h    };       // optional -- can be -1 


    // compute bounds rectangles, when width, height, etc. are defaulted -- want dest rectangle pixel size to be same as image pixel size (i.e. ignore resolution of loaded image)
    // -----------------------------------------------------------------

    // compute src bounds, if these are defaulted
  
    if (src_col == -1) src_col = 0;                 
    if (src_row == -1) src_row = 0;                 
    if (src_w   == -1) src_w   = image_width; 
    if (src_h   == -1) src_h   = image_height; 
    if (dest_w  == -1) dest_w  = src_w;            // default = same as image width       
    if (dest_h  == -1) dest_h  = src_h;            // default = same as image width 


    // chose required version of draw_image() based on input parms
    // -----------------------------------------------------------

    int d_rc {}; 

    if (di_parm.trans_color_valid)           // case 5 ? -- need to completely fill in dest and src limits
    {
         Gdiplus::Rect dest_rect { (INT)dest_col, (INT)dest_row, (INT)dest_w, (INT)dest_h }; 

         Gdiplus::ImageAttributes image_attr { }; 
       
         image_attr.SetColorKey( Gdiplus::Color { di_parm.trans_color1.r, di_parm.trans_color1.g, di_parm.trans_color1.b }
                               , Gdiplus::Color { di_parm.trans_color2.r, di_parm.trans_color2.g, di_parm.trans_color2.b }
                               );

         d_rc = outimage.canvas.draw_image(&image, dest_rect, (INT)src_col, (INT)src_row, (INT)src_w, (INT)src_h, Gdiplus::UnitPixel, &image_attr);  
    }
    else
    if ( (di_parm.src_row == -2) )   // case 1 or case 2 ?
    {
        d_rc = outimage.canvas.draw_image(&image, (INT)dest_col, (INT)dest_row, (INT)dest_w, (INT)dest_h);     // use simple form with no src rectangle
    }
    else                             // must be case 3 or 4 -- no direct call to Gdiplus::DrawImage() with 8 INTs is available
    {
        Gdiplus::Rect dest_rect { (INT)dest_col, (INT)dest_row, (INT)dest_w, (INT)dest_h };    
    
        d_rc = outimage.canvas.draw_image(&image, dest_rect, (INT)src_col, (INT)src_row, (INT)src_w, (INT)src_h, Gdiplus::UnitPixel, nullptr);             
    }  

    return 0;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        xxx() -- text-oriented functions  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int add_private_font(outimage_S& outimage, const std::wstring& filename) try
{
    return outimage.canvas.add_font_file(filename); 
}
M_endf


M_DRAW_IMPEXP
int add_private_fonts(outimage_S& outimage, const std::wstring& folder_name, bool nested) try
{
    return outimage.canvas.add_font_folder(folder_name, nested); 
}
M_endf
 

M_DRAW_IMPEXP
int remove_private_fonts(outimage_S& outimage) try
{
    return outimage.canvas.remove_fonts(); 
}
M_endf


M_DRAW_IMPEXP
int display_installed_fonts(outimage_S& outimage) try 
{
    return outimage.canvas.display_fonts(true); 
}
M_endf


M_DRAW_IMPEXP
int display_private_fonts(outimage_S& outimage) try 
{
    return outimage.canvas.display_fonts(false); 
}
M_endf

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     get_font_count()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int get_font_count(outimage_S& outimage, bool installed, int32_t& family_ct) try 
{
    return outimage.canvas.get_family_count(installed, family_ct); 
}
M_endf  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     get_font_name() -- n-th font in collection
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int get_font_name(outimage_S& outimage, bool installed, int32_t family_ix, std::wstring& family_name) try 
{
    // get vector of all font family names in collection (installed/private)
    // ---------------------------------------------------------------------

    std::vector<std::wstring> family_names {}; 

    auto g_rc = outimage.canvas.get_family_names(installed, family_names);

    if (g_rc != 0)
        return -1;     // should have error message already


    // pass-back n-th family name (if in vector)
    // -----------------------------------------

    if ( (family_ix < 0) || (family_ix >= family_names.size()) )
    {   
        M_out_emsg(L"get_font_name(): family_ix = %d -- allowed range is 0 to %d") % family_ix % (family_names.size() - 1);   
        return -1; 
    }

    family_name = family_names.at(family_ix); 
    return 0; 
}
M_endf  


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     set_text_color()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int set_text_color(outimage_S& outimage, const rgb_S& color) try
{
    // update current text color in canvas_C

    return outimage.canvas.set_text_color(color); 
}
M_endf


M_DRAW_IMPEXP
int set_text_color(outimage_S& outimage, const rgb_S& color, alpha_T alpha) try
{
    // update current text color in canvas_C

    return outimage.canvas.set_text_color(color, alpha); 
}
M_endf                                                                                                               


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     set_text_background_color()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int set_text_background_color(outimage_S& outimage, const rgb_S& color) try
{
    // update current text background color in canvas_C

    return outimage.canvas.set_text_background_color(color); 
}
M_endf


M_DRAW_IMPEXP
int set_text_background_color(outimage_S& outimage, const rgb_S& color, alpha_T alpha) try
{
    // update current text background color in canvas_C
    
    return outimage.canvas.set_text_background_color(color, alpha); 
}
M_endf   


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     set_no_text_background()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int set_no_text_background(outimage_S& outimage) try
{
    // remove current text background color in canvas_C

    return outimage.canvas.set_no_text_background(); 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     set_font()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// simple form -- installed font collection, style=regular

M_DRAW_IMPEXP
int set_font(outimage_S& outimage, const std::wstring& family_name, real_T em_size) try
{
    return outimage.canvas.set_font(family_name, (Gdiplus::REAL)em_size, (INT)(Gdiplus::FontStyleRegular), true); 
}
M_endf


M_DRAW_IMPEXP
int set_font(outimage_S& outimage, const std::wstring& family_name, real_T em_size, uint32_t style, bool installed) try
{
    return outimage.canvas.set_font(family_name, (Gdiplus::REAL)em_size, (INT)style, installed); 
}
M_endf

// form with color setting

M_DRAW_IMPEXP
int set_font(outimage_S& outimage, const std::wstring& family_name, real_T em_size, uint32_t style, const rgb_S& color, bool installed) try
{
    // update current text color in canvas_C

    auto c_rc = outimage.canvas.set_text_color(color); 

    if (c_rc != 0)
        return -1; 


    // call set_font() (non-color version, above) to update current font family, size, etc.

    auto f_rc = set_font(outimage, family_name, em_size, style, installed); 

    if (f_rc != 0)
        return -1; 

    return 0;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     set_font_size()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// update em_size in currently active font -- keep family_name, font_style, etc.

M_DRAW_IMPEXP
int set_font_size(outimage_S& outimage, real_T em_size) try
{
    return outimage.canvas.set_font_size((Gdiplus::REAL)em_size); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     draw_text()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// simple version -- row, col only 

M_DRAW_IMPEXP
int draw_text(outimage_S& outimage, const std::wstring& ws, rc_T row, rc_T col) try    
{
    outimage.canvas.set_text_format((INT)col, (INT)row);

    return outimage.canvas.draw_str(ws);
}
M_endf


// version with rotation 

M_DRAW_IMPEXP
int draw_text(outimage_S& outimage, const std::wstring& ws, rc_T row, rc_T col, real_T rotation) try    
{
    outimage.canvas.set_text_format((INT)col, (INT)row, (Gdiplus::REAL)rotation);

    return outimage.canvas.draw_str(ws);
}
M_endf


// version with scale and shear factors

M_DRAW_IMPEXP
int draw_text(outimage_S& outimage, const std::wstring& ws, rc_T row, rc_T col, real_T rotation, real_T x_scale, real_T y_scale, real_T x_shear, real_T y_shear) try    
{
    outimage.canvas.set_text_format((INT)col, (INT)row, (Gdiplus::REAL)rotation, (Gdiplus::REAL)x_scale, (Gdiplus::REAL)y_scale, (Gdiplus::REAL)x_shear, (Gdiplus::REAL)y_shear);

    return outimage.canvas.draw_str(ws);
}
M_endf


// version with extra_space

M_DRAW_IMPEXP
int draw_text(outimage_S& outimage, const std::wstring& ws, rc_T row, rc_T col, real_T rotation, real_T x_scale, real_T y_scale, real_T x_shear, real_T y_shear, real_T extra_space) try    
{
    outimage.canvas.set_text_format((INT)col, (INT)row, (Gdiplus::REAL)rotation, (Gdiplus::REAL)x_scale, (Gdiplus::REAL)y_scale, (Gdiplus::REAL)x_shear, (Gdiplus::REAL)y_shear, (Gdiplus::REAL)extra_space);

    return outimage.canvas.draw_txt(ws);                                        // must call draw_txt() to get extra_space support
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     format_text()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int format_text(outimage_S& outimage, const format_text_parm_S& fparm) try    
{
    return outimage.canvas.format_str(fparm.str, fparm.top, fparm.left, fparm.width, fparm.height);   // simple version -- default alignment, etc. 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        set_clip_window() -- set clipping region for outimage to current window only 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int set_clip_window(outimage_S& outimage) try
{
    // set clipping region to map window bounds
    // ----------------------------------------


    Gdiplus::Rect rect { (INT)(outimage.curr_window_left_col)
                       , (INT)(outimage.curr_window_top_row )
                       , (INT)(outimage.curr_window_width   )
                       , (INT)(outimage.curr_window_height  )
                       };
              
    auto rc = outimage.canvas.set_clip(rect); 

    if (rc != 0)
       return -1; 

    return 0;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        reset_clip() -- remove clipping region 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int reset_clip(outimage_S& outimage) try
{
    // reset clipping region to whole outimage
    // ------------------------------------

    auto rc = outimage.canvas.reset_clip(); 

    if (rc != 0)
       return -1; 

    return 0;
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        put_graphics() -- copy graphics DIBSection bitmaps from canvas_C data into outimage_S buffer
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int put_graphics(outimage_S& outimage) try
{
    // flush out any pending GDI+ operations

    outimage.canvas.flush();


    // copy each DIBsection buffer into outimage buffer at proper location

    for (auto i = 0; i < outimage.canvas.m_bmct; i++)
    {
        put_DIBSection( outimage
                      , outimage.canvas.m_pvBits.at(i)
                      , outimage.canvas.m_bmpos.at(i).x
                      , outimage.canvas.m_bmpos.at(i).y
                      , outimage.canvas.m_bmpos.at(i).rows
                      , outimage.canvas.m_bmpos.at(i).cols
                      );     
    }
                                                                                                                          
    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        get_graphics() -- copy graphics outimage_S buffer data into graphics DIBSection bitmaps
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int get_graphics(outimage_S& outimage) try
{
    // flush out any pending GDI+ operations

    outimage.canvas.flush();

    
    // copy outimage buffer data from proper location into each DIBsection buffer

    for (auto i = 0; i < outimage.canvas.m_bmct; i++)
    {
        get_DIBSection( outimage
                      , outimage.canvas.m_pvBits.at(i)
                      , outimage.canvas.m_bmpos.at(i).x
                      , outimage.canvas.m_bmpos.at(i).y
                      , outimage.canvas.m_bmpos.at(i).rows
                      , outimage.canvas.m_bmpos.at(i).cols
                      );     
    }
                                                                                                                                                                      
    return 0; 
}
M_endf





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        put_DIBSection() -- copy one DIBSection's data into outimage buffer
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int put_DIBSection(outimage_S& outimage, void *pvBits, rc_T x, rc_T y, rc_T rows, rc_T cols) try
{
    // copy single DIBsection buffer into outimage buffer at proper location

    M__(M_out(L"put_DIBSection(): x=%d  y=%d  rows=%d  cols=%d") % x % y % rows % cols;) 


    if ( (rows > 0) && (cols > 0) )
    {
        for (auto row = 0; row < rows; row++)
        for (auto col = 0; col < cols; col++)
        {
           // set outimage buffer pixel, if pixel is not set to transparent color in DIBSection

           RGBTRIPLE rgbt = M_DIBSection_rgb(pvBits, outimage.canvas.m_actual_cols_bmp, row, col);

           if  (
                (rgbt.rgbtRed   != cx::bmp_transp_r)
                ||
                (rgbt.rgbtGreen != cx::bmp_transp_g)
                ||
                (rgbt.rgbtBlue  != cx::bmp_transp_b)
               )
           {
               rgb_S rgb {rgbt.rgbtRed, rgbt.rgbtGreen, rgbt.rgbtBlue};

               M_outimage_rgb(outimage, y + row, x + col) = rgb;
           }
        }
     }

    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        get_DIBSection() -- copy data from outimage buffer into one DIBSection
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_DIBSection(outimage_S& outimage, void *pvBits, rc_T x, rc_T y, rc_T rows, rc_T cols) try
{
    // copy outimage buffer data from proper location into one DIBsection

    M__(M_out(L"get_DIBSection(): x=%d  y=%d  rows=%d  cols=%d") % x % y % rows % cols;) 


    if ( (rows > 0) && (cols > 0) )
    {
        for (auto row = 0; row < rows; row++)
        for (auto col = 0; col < cols; col++)
        {
           // set DIBSection pixel from corresponding pixel in outimage buffer

           rgb_S     rgb  = M_outimage_rgb(outimage, y + row, x + col);
           RGBTRIPLE rgbt {rgb.b, rgb.g, rgb.r};                     // re-arange color bytes

           M_DIBSection_rgb(pvBits, outimage.canvas.m_actual_cols_bmp, row, col) = rgbt;           
        }
     }

    return 0; 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        put_box() -- draw solid-color box in outimage buffer  (box is clipped to outimage_S limits)
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int put_box(outimage_S& outimage, rc_T row, rc_T col, rc_T width, rc_T height, const rgb_S& color) try
{
    int               rc {0}; 

    rc_T row_max = outimage.rows - 1;     
    rc_T col_max = outimage.cols - 1; 
     
    M__(M_out(L"OutImage:    put_box -- row   : %-d"      ) % row   ;) 
    M__(M_out(L"OutImage:    put_box -- col   : %-d"      ) % col   ;) 
    M__(M_out(L"OutImage:    put_box -- width : %-d"      ) % width ;) 
    M__(M_out(L"OutImage:    put_box -- height: %-d"      ) % height;) 

    
    // return, if box does not have positive dimensions

    if (width < 1)
    {
        M_out_emsg(L"put_box(): box width (%d) is not positive") % width; 
        return -1; 
    }

    if (height < 1)
    {
        M_out_emsg(L"put_box(): box height (%d) is not positive") % height; 
        return -1; 
    }
     

    // compute row, column limits for box

    rc_T r_min = row; 
    rc_T c_min = col; 
    rc_T r_max = row + height - 1;  
    rc_T c_max = col + width  - 1; 
   

    // return, if no part of box within bounds of outimage -- no error or message 
   
    if (r_max  <  0       ) return 0;
    if (c_max  <  0       ) return 0;
    if (r_min  >  row_max ) return 0;
    if (c_min  >  col_max ) return 0;


     // compute limits of visible box

     r_min = M_max(r_min, 0      ); 
     c_min = M_max(c_min, 0      );
     r_max = M_min(r_max, row_max); 
     c_max = M_min(c_max, col_max);

    M__(M_out(L"OutImage:    put_box -- r_min   : %-d"      ) % r_min ;) 
    M__(M_out(L"OutImage:    put_box -- c_min   : %-d"      ) % c_min ;) 
    M__(M_out(L"OutImage:    put_box -- r_max   : %-d"      ) % r_max ;) 
    M__(M_out(L"OutImage:    put_box -- c_max   : %-d"      ) % c_max ;) 



    //  Main loop to set each pixel in the visible box to caller's color
    //  ----------------------------------------------------------------

    for (rc_T r = r_min; r <= r_max; r++)        // do all rows in visible box
    {
        for (rc_T c = c_min; c <= c_max; c++)    // do all columns in visible box 
        {
             M_outimage_rgb(outimage, r, c) =  color;
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
//||        put_frame() -- draw hollow frame in outimage buffer
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int put_frame(outimage_S& outimage, rc_T row, rc_T col, rc_T width, rc_T height, rc_T thick, const rgb_S& color) try
{
    int               rc {0}; 

    // return with error, if height or width is not more than twice the thickness, which must be positive 

    if (thick <= 0)
    {
        M_out_emsg(L"put_frame(): frame thickness (%d) is too small") % thick; 
        return -1; 
    } 

    if (width <= 2 * thick)
    {
        M_out_emsg(L"put_frame(): overall frame width (%d) not more than twice the frame thickness (%d)") % width % thick; 
        return -1; 
    }

    if (height <= 2 * thick)
    {
        M_out_emsg(L"put_frame(): overall frame height (%d) not more than twice the frame thickness (%d)") % height % thick; 
        return -1; 
    }


    // Draw the 4 sides of the frame

    put_box(outimage, row                  , col                   , width                   , thick                , color);   // upper      frame box
    put_box(outimage, row + height - thick , col                   , width                   , thick                , color);   // lower      frame box
    put_box(outimage, row + thick          , col                   , thick                   , height - 2 * thick   , color);   // left  side frame box
    put_box(outimage, row + thick          , col + width - thick   , thick                   , height - 2 * thick   , color);   // right side frame box

    return rc;
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        put_point() -- draw point in outimage buffer
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int put_point(outimage_S& outimage, rc_T row, rc_T col, const rgb_S& color) try
{
    int               rc {0}; 

    // return, if no part of box within bounds of outimage  
   
    if (row  <  0                ) return rc;
    if (col  <  0                ) return rc;
    if (row  >  outimage.rows - 1) return rc;
    if (col  >  outimage.cols - 1) return rc;

    
    // place point outimage buffer
       
    M_outimage_rgb(outimage, row, col) =  color;
           
    return rc;
}
M_endf





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       write_outimage() -- write data in outimage_S structure to .BMP or .TIF file 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int write_outimage(const outimage_S& outimage, const write_outimage_parm_S& wparm) try  
{
    int               rc {0}; 
    std::wstring      filename; 

    M_start_timer(c1, t1)

           
    // form complete path/file name  (without filetype -- will be set to .tiff or .bmp by file-writing routine 
    // ----------------------------
        
    filename = wparm.folder + wparm.filename;    


    // Write out 24-bit BMP or 24-bit TIFF file
    // ----------------------------------------

    if (wparm.tiff_24)
        rc = writeout_tiff24(outimage, filename, wparm.tiff24_parm); 
    else
        rc = writeout_bmp24( outimage, filename                   ); 

    
    M_out(L"Image   : Output outimage file written - elapsed time = %.3f seconds") % M_read_timer(c1, t1); 

    return rc;  
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_outimage_data() -- internal function to finish setting up outimage buffer, initialize graphics, etc. 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int create_outimage_data(outimage_S& outimage, bool display) try
{    
    //  Set up items dependent on settings already captured in the outimage_S area
    //  ------------------------------------------------------------------------

    outimage.row_l   = (int)sizeof (rgb_S) * outimage.cols; 
    outimage.pixels  = (uint64_t)(outimage.rows) * (uint64_t)(outimage.cols);
    outimage.bytes   = sizeof (rgb_S) * outimage.pixels;
    outimage.data_l  = outimage.bytes;
  
    outimage.buf.alloc(outimage.pixels);    // allocate buffer for outimage data


    // clear out map window area -- mark as invalid
    // --------------------------------------------

    outimage.curr_window_valid      = false ;
    outimage.curr_window_height     =  0    ;
    outimage.curr_window_width      =  0    ;
    outimage.curr_window_top_row    =  0    ;
    outimage.curr_window_bottom_row =  0    ;
    outimage.curr_window_left_col   =  0    ;
    outimage.curr_window_right_col  =  0    ;


    // initialize canvas_C for this outimage buffer -- return with error, if init() failed -- allow up to 10 DIBSections in canvas_C

    auto i_rc = outimage.canvas.init(outimage.rows, outimage.cols, 10);  
    if (i_rc != 0)
        return -1;


    // clear canvas_C for this outimage buffer to "transparent" color -- return with error, if clear() failed

    auto c_rc = outimage.canvas.clear(rgb_S { cx::bmp_transp_r, cx::bmp_transp_g, cx::bmp_transp_b });  
    if (c_rc != 0)
        return -1;


    // display info, if required

    if (display)
    {
        M_out(L"outimage.rows            =  %d")  %  outimage.rows                   ;
        M_out(L"outimage.cols            =  %d")  %  outimage.cols                   ;
        M_out(L"outimage.row_l           =  %u")  %  outimage.row_l                  ;
        M_out(L"outimage.pixels          =  %s")  %  M_sep(outimage.pixels)          ;
        M_out(L"outimage.bytes           =  %s")  %  M_sep(outimage.bytes)           ;
        M_out(L"outimage.data_l          =  %s")  %  M_sep(outimage.data_l)          ;
        M_out(L"outimage.buf.sz          =  %s")  %  M_sep(outimage.buf.sz)          ;
        M_out(L"outimage.buf.sz1         =  %s")  %  M_sep(outimage.buf.sz1)         ;
        M_out(L"outimage.buf.p           =  %p")  %  (void *)(outimage.buf.p)        ;
        M_out(L"outimage.buf.p1          =  %p")  %  (void *)(outimage.buf.p1)       ;
    }
  
    return 0;
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_outimage() -- get outimage buffer, etc. -- dimensions from passed-in parms 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int create_outimage(outimage_S& outimage, const outimage_create_parm_S& iparm) try
{   
    int               rc {0}; 

    //  Set up row/colum limits for RGB outimage data 
    //  ---------------------------------------------
     
    outimage.rows     = iparm.rows;
    outimage.cols     = iparm.cols;


    //  Set up items dependent on above settings
    //  ----------------------------------------

    create_outimage_data(outimage, iparm.display);   
    return rc;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        destroy_outimage() -- free up outimage buffer, etc.  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int destroy_outimage(outimage_S& outimage) try
{  
    int               rc {0}; 


    // terminate graphics for this outimage buffer  (save away r/c )
    
    rc = outimage.canvas.term();     


    // clean up outimage data

    outimage.buf.free();   

    outimage.rows     = 0; 
    outimage.cols     = 0;
    outimage.row_l    = 0;  
    outimage.pixels   = 0; 
    outimage.bytes    = 0;
    outimage.data_l   = 0;
      
    return rc;
}
M_endf






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        put_string8() -- draw string of 8-bit wide letters -- going forward (increasing column, then increasing row)
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int put_string8(outimage_S& outimage, const wchar_t *str_p, rc_T row, rc_T col, const rgb_S& rgb) try
{
    int               rc {0}; 

    init_bm();                                    // make sure bitmaps are initialized before using them

    size_t s = wcslen(str_p); 

    for (int i = 0;  i < s; i++)
    {
        put_letter8(outimage, str_p[i], row, col + i * 8, rgb);    
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
//||        put_letter8() -- draw 8-bit wide letter -- going forward (increasing column, then increasing row)
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void put_letter8(outimage_S& outimage, wchar_t ch, rc_T row, rc_T col, const rgb_S& rgb) try
{
    switch (ch)
    {
    case L' ' :  put_bitmask8(outimage, static_N::bm_blank, row, col, 16, rgb); break;    // blank  
    case L'+' :  put_bitmask8(outimage, static_N::bm_plus,  row, col, 16, rgb); break;    // plus  
    case L'-' :  put_bitmask8(outimage, static_N::bm_minus, row, col, 16, rgb); break;    // minus  
    case L'0' :  put_bitmask8(outimage, static_N::bm_0    , row, col, 16, rgb); break;    // 0  
    case L'1' :  put_bitmask8(outimage, static_N::bm_1    , row, col, 16, rgb); break;    // 1  
    case L'2' :  put_bitmask8(outimage, static_N::bm_2    , row, col, 16, rgb); break;    // 2  
    case L'3' :  put_bitmask8(outimage, static_N::bm_3    , row, col, 16, rgb); break;    // 3  
    case L'4' :  put_bitmask8(outimage, static_N::bm_4    , row, col, 16, rgb); break;    // 4  
    case L'5' :  put_bitmask8(outimage, static_N::bm_5    , row, col, 16, rgb); break;    // 5  
    case L'6' :  put_bitmask8(outimage, static_N::bm_6    , row, col, 16, rgb); break;    // 6  
    case L'7' :  put_bitmask8(outimage, static_N::bm_7    , row, col, 16, rgb); break;    // 7  
    case L'8' :  put_bitmask8(outimage, static_N::bm_8    , row, col, 16, rgb); break;    // 8 
    case L'9' :  put_bitmask8(outimage, static_N::bm_9    , row, col, 16, rgb); break;    // 9

    default  :  M_out_emsg(L"unknown char passed to put_letter8() = \'%c\'") % (char)ch;  
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
//||        put_bitmask8() -- draw 8-bit bitmask array -- going forward (increasing column, then increasing row)
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


static void put_bitmask8(outimage_S& outimage, const bitmask8_T *p, rc_T row, rc_T col, rc_T length, const rgb_S& rgb) try
{
    rc_T r = row;     
    

    // Do column loop for each byte in bitmask8  

    for (int i = 0; i < length; i++)
    {
      //  M_msg("row=%d  r=%d, col=%d, p[i]=%d", row, r, col, (int)(p[i]))

        if ((p[i] & (uint8_t)0x80) != 0)   put_point(outimage, r, col     , rgb);
        if ((p[i] & (uint8_t)0x40) != 0)   put_point(outimage, r, col + 1 , rgb);
        if ((p[i] & (uint8_t)0x20) != 0)   put_point(outimage, r, col + 2 , rgb);
        if ((p[i] & (uint8_t)0x10) != 0)   put_point(outimage, r, col + 3 , rgb);
        if ((p[i] & (uint8_t)0x08) != 0)   put_point(outimage, r, col + 4 , rgb);
        if ((p[i] & (uint8_t)0x04) != 0)   put_point(outimage, r, col + 5 , rgb);
        if ((p[i] & (uint8_t)0x02) != 0)   put_point(outimage, r, col + 6 , rgb);
        if ((p[i] & (uint8_t)0x01) != 0)   put_point(outimage, r, col + 7 , rgb);

        r++; 
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
//||        init_bm() -- init letter bit maps (if not already done) 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void init_bm(void) try
{

   //  return if already done

   if (static_N::bm_init == true) 
       return; 


   //   set up bitmask for  (template)
   //   ------------------------------
/*
   static_N::bm_b[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_b[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_b[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_b[3 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_b[4 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_b[5 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_b[6 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_b[7 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_b[8 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_b[9 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_b[10] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_b[11] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_b[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_b[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_b[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_b[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
*/

   //   set up bitmask for ' ' (blank)
   //   ------------------------------

   static_N::bm_blank[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_blank[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_blank[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_blank[3 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_blank[4 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_blank[5 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_blank[6 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_blank[7 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_blank[8 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_blank[9 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_blank[10] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_blank[11] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_blank[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_blank[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_blank[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_blank[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear



   //   set up bitmask for '+' plus
   //   ----------------------------

   static_N::bm_plus[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_plus[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_plus[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_plus[3 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // tall
   static_N::bm_plus[4 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // tall
   static_N::bm_plus[5 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // tall
   static_N::bm_plus[6 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // tall
   static_N::bm_plus[7 ] = M_bitmask8( MMM,MMM,MMM,MMM,MMM,MMM,MMM, 0 );      // lower
   static_N::bm_plus[8 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_plus[9 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_plus[10] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_plus[11] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_plus[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_plus[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_plus[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_plus[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear





   //   set up bitmask for '-' minus
   //   ----------------------------

   static_N::bm_minus[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_minus[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_minus[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_minus[3 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_minus[4 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_minus[5 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_minus[6 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_minus[7 ] = M_bitmask8( MMM,MMM,MMM,MMM,MMM,MMM,MMM, 0 );      // lower
   static_N::bm_minus[8 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_minus[9 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_minus[10] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_minus[11] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_minus[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_minus[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_minus[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_minus[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear



   //   set up bitmask for '0'
   //   ----------------------

   static_N::bm_0[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_0[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_0[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_0[3 ] = M_bitmask8(  0 , 0 ,MMM,MMM,MMM, 0 , 0 , 0 );      // tall
   static_N::bm_0[4 ] = M_bitmask8(  0 ,MMM,MMM, 0 ,MMM,MMM, 0 , 0 );      // tall
   static_N::bm_0[5 ] = M_bitmask8( MMM,MMM, 0 , 0 , 0 ,MMM,MMM, 0 );      // tall
   static_N::bm_0[6 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // tall
   static_N::bm_0[7 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // lower
   static_N::bm_0[8 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // lower
   static_N::bm_0[9 ] = M_bitmask8( MMM,MMM, 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_0[10] = M_bitmask8(  0 ,MMM,MMM, 0 ,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_0[11] = M_bitmask8(  0 , 0 ,MMM,MMM,MMM, 0 , 0 , 0 );      // lower
   static_N::bm_0[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_0[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_0[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_0[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear


   //   set up bitmask for '1'
   //   ----------------------

   static_N::bm_1[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_1[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_1[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_1[3 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // tall
   static_N::bm_1[4 ] = M_bitmask8(  0 , 0 ,MMM,MMM, 0 , 0 , 0 , 0 );      // tall
   static_N::bm_1[5 ] = M_bitmask8(  0 ,MMM,MMM,MMM, 0 , 0 , 0 , 0 );      // tall
   static_N::bm_1[6 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // tall
   static_N::bm_1[7 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_1[8 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_1[9 ] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_1[10] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_1[11] = M_bitmask8(  MMM,MMM,MMM,MMM,MMM,MMM,MMM, 0 );      // lower
   static_N::bm_1[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_1[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_1[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_1[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear


   //   set up bitmask for '2'
   //   ----------------------

   static_N::bm_2[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_2[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_2[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_2[3 ] = M_bitmask8(  0 , 0 ,MMM,MMM,MMM,MMM, 0 , 0 );      // tall
   static_N::bm_2[4 ] = M_bitmask8(  0 ,MMM,MMM, 0,  0 ,MMM,MMM, 0 );      // tall
   static_N::bm_2[5 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // tall
   static_N::bm_2[6 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM,MMM, 0 );      // tall
   static_N::bm_2[7 ] = M_bitmask8(  0 , 0 , 0 , 0 ,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_2[8 ] = M_bitmask8(  0 , 0 , 0 ,MMM,MMM, 0 , 0 , 0 );      // lower
   static_N::bm_2[9 ] = M_bitmask8(  0 , 0 ,MMM,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_2[10] = M_bitmask8(  0 ,MMM,MMM, 0 , 0 , 0 , 0 , 0 );      // lower
   static_N::bm_2[11] = M_bitmask8( MMM,MMM,MMM,MMM,MMM,MMM,MMM, 0 );      // lower
   static_N::bm_2[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_2[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_2[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_2[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear


   //   set up bitmask for '3'
   //   ----------------------

   static_N::bm_3[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_3[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_3[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_3[3 ] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // tall
   static_N::bm_3[4 ] = M_bitmask8( MMM,MMM, 0 , 0,  0 ,MMM,MMM, 0 );      // tall
   static_N::bm_3[5 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // tall
   static_N::bm_3[6 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM,MMM, 0 );      // tall
   static_N::bm_3[7 ] = M_bitmask8(  0 , 0 ,MMM,MMM,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_3[8 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_3[9 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // lower
   static_N::bm_3[10] = M_bitmask8( MMM,MMM, 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_3[11] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_3[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_3[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_3[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_3[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear


   //   set up bitmask for '4' 
   //   ----------------------

   static_N::bm_4[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_4[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_4[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_4[3 ] = M_bitmask8(  0 , 0 , 0 , 0 ,MMM,MMM, 0 , 0 );      // tall
   static_N::bm_4[4 ] = M_bitmask8(  0 , 0 , 0 ,MMM,MMM,MMM, 0 , 0 );      // tall
   static_N::bm_4[5 ] = M_bitmask8(  0 , 0 ,MMM,MMM, 0 ,MMM, 0 , 0 );      // tall
   static_N::bm_4[6 ] = M_bitmask8(  0 ,MMM,MMM, 0 , 0 ,MMM, 0 , 0 );      // tall
   static_N::bm_4[7 ] = M_bitmask8( MMM,MMM, 0 , 0 , 0 ,MMM, 0 , 0 );      // lower
   static_N::bm_4[8 ] = M_bitmask8( MMM,MMM,MMM,MMM,MMM,MMM,MMM, 0 );      // lower
   static_N::bm_4[9 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM, 0 , 0 );      // lower
   static_N::bm_4[10] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM, 0 , 0 );      // lower
   static_N::bm_4[11] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM, 0 , 0 );      // lower
   static_N::bm_4[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_4[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_4[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_4[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear



   //   set up bitmask for '5' 
   //   ----------------------

   static_N::bm_5[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_5[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_5[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_5[3 ] = M_bitmask8( MMM,MMM,MMM,MMM,MMM,MMM,MMM, 0 );      // tall
   static_N::bm_5[4 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_5[5 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_5[6 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_5[7 ] = M_bitmask8( MMM,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_5[8 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_5[9 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // lower
   static_N::bm_5[10] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_5[11] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_5[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_5[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_5[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_5[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   


   //   set up bitmask for '6'
   //   ---------------------

   static_N::bm_6[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_6[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_6[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_6[3 ] = M_bitmask8(  0 , 0 ,MMM,MMM,MMM, 0 , 0 , 0 );      // tall
   static_N::bm_6[4 ] = M_bitmask8(  0 ,MMM,MMM, 0,  0 , 0 , 0 , 0 );      // tall
   static_N::bm_6[5 ] = M_bitmask8( MMM,MMM, 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_6[6 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // tall
   static_N::bm_6[7 ] = M_bitmask8( MMM, 0 ,MMM,MMM,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_6[8 ] = M_bitmask8( MMM,MMM,MMM, 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_6[9 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // lower
   static_N::bm_6[10] = M_bitmask8( MMM,MMM, 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_6[11] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_6[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_6[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_6[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_6[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear

   

   //   set up bitmask for '7' 
   //   ----------------------

   static_N::bm_7[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_7[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_7[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_7[3 ] = M_bitmask8( MMM,MMM,MMM,MMM,MMM,MMM,MMM, 0 );      // tall
   static_N::bm_7[4 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // tall
   static_N::bm_7[5 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM,MMM, 0 );      // tall
   static_N::bm_7[6 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM, 0 , 0 );      // tall
   static_N::bm_7[7 ] = M_bitmask8(  0 , 0 , 0 , 0 ,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_7[8 ] = M_bitmask8(  0 , 0 , 0 , 0 ,MMM, 0 , 0 , 0 );      // lower
   static_N::bm_7[9 ] = M_bitmask8(  0 , 0 , 0 ,MMM,MMM, 0 , 0 , 0 );      // lower
   static_N::bm_7[10] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_7[11] = M_bitmask8(  0 , 0 , 0 ,MMM, 0 , 0 , 0 , 0 );      // lower
   static_N::bm_7[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_7[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_7[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_7[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear



   //   set up bitmask for '8'
   //   ----------------------

   static_N::bm_8[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_8[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_8[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_8[3 ] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // tall
   static_N::bm_8[4 ] = M_bitmask8( MMM,MMM, 0 , 0,  0 ,MMM,MMM, 0 );      // tall
   static_N::bm_8[5 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // tall
   static_N::bm_8[6 ] = M_bitmask8( MMM,MMM, 0 , 0 , 0 ,MMM,MMM, 0 );      // tall
   static_N::bm_8[7 ] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_8[8 ] = M_bitmask8( MMM,MMM, 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_8[9 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // lower
   static_N::bm_8[10] = M_bitmask8( MMM,MMM, 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_8[11] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_8[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_8[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_8[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_8[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear


   //   set up bitmask for '9'
   //   ----------------------

   static_N::bm_9[0 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear
   static_N::bm_9[1 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 1
   static_N::bm_9[2 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // accent 2
   static_N::bm_9[3 ] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM,MMM, 0 , 0 );      // tall
   static_N::bm_9[4 ] = M_bitmask8( MMM,MMM, 0 , 0,  0 ,MMM,MMM, 0 );      // tall
   static_N::bm_9[5 ] = M_bitmask8( MMM, 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // tall
   static_N::bm_9[6 ] = M_bitmask8( MMM,MMM, 0 , 0 ,MMM,MMM,MMM, 0 );      // tall
   static_N::bm_9[7 ] = M_bitmask8(  0 ,MMM,MMM,MMM,MMM, 0 ,MMM, 0 );      // lower
   static_N::bm_9[8 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 ,MMM, 0 );      // lower
   static_N::bm_9[9 ] = M_bitmask8(  0 , 0 , 0 , 0 , 0 ,MMM,MMM, 0 );      // lower
   static_N::bm_9[10] = M_bitmask8(  0 , 0 , 0 , 0 ,MMM,MMM, 0 , 0 );      // lower
   static_N::bm_9[11] = M_bitmask8(  0 , 0 ,MMM,MMM,MMM, 0 , 0 , 0 );      // lower
   static_N::bm_9[12] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_9[13] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_9[14] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // descender
   static_N::bm_9[15] = M_bitmask8(  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );      // clear




   static_N::bm_init = true; 
   return; 
} 
M_endf






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       writeout_tiff24() -- write outimage data to 24-bit RGB .tif file -- no compression
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

#define N_TAGS          15
#define N_TAGS_MAX      15


M_DRAW_IMPEXP
int writeout_tiff24(const outimage_S& outimage, const std::wstring& basename, const tiff24_parm_S& tparm) try
{
    using namespace tiff_N;

    int               rc {0}; 
 

    //  Fill out internal TIFF specification structure (RGB-type only, uncompressed)
    //  ----------------------------------------------------------------------------

    rgb_tiff_S tiff {};

    tiff.outimage_width      = (uint32_t)(outimage.cols);
    tiff.outimage_length     = (uint32_t)(outimage.rows);
    tiff.rows_per_strip      = tparm.rows_per_strip;
    tiff.x_resolution        = {tparm.x_resolution, 1U};
    tiff.y_resolution        = {tparm.y_resolution, 1U};
    tiff.resolution_unit     = tparm.resolution_unit;
    tiff.orientation         = tparm.orientation;

    
     // Form complete filename for .tif file

    const std::wstring  extname  {L".tif"};
    std::wstring        filename {basename + extname};


    // Open output .tif file using complete filename

    std::ofstream ftiff;

    ftiff.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    
    if (!(ftiff.good()))
    {
        M_out_emsg(L"Cannot open() \"%s\" for output") % filename;  
        return -1;
    }
    
    M_out(L"Output  : outimage file name = \"%s\"") % filename;  


    // Declare .tiff file headers, etc.
    // --------------------------------

    ifh_S     ifh;

    uint16_t  ifd_header   { N_TAGS };
    tag_S     ifd[N_TAGS_MAX];
    uint32_t  ifd_trailer  { 0 };
   
    
    // calculate sizes and offsets for .tiff file
    // ------------------------------------------

    const uint32_t  n_strips         { (tiff.outimage_length + tiff.rows_per_strip - 1) / tiff.rows_per_strip };
    const uint32_t  rows_per_strip1  { tiff.rows_per_strip };                                           // rows in all strips except last one -- not valid if just 1 strip 
    const uint32_t  rows_per_strip9  { tiff.outimage_length - (n_strips - 1) * rows_per_strip1 };       // rows in last or only strip
    const uint32_t  rows_per_strip   { n_strips == 1 ? rows_per_strip9 : rows_per_strip1 };
    const uint32_t  strip1_byte_ct   { 3 * tiff.outimage_width * rows_per_strip1 };                     // bytes in all strips except last one -- not valid if just 1 strip 
    const uint32_t  strip9_byte_ct   { 3 * tiff.outimage_width * rows_per_strip9 };                     // bytes in last or only strip   

    const uint32_t  tiff_offset      { (uint32_t)(sizeof ifh + sizeof ifd_header + ((size_t)ifd_header * sizeof (tag_S)) + sizeof ifd_trailer) };   // offset in .tiff file for overflow IFD data items data  = tiff_offset + index into buf  

    //                                  IFH, IFD      bits/sample              X/Y resolution                      Strip offsets and counts  
    const uint32_t  strip1_offset    { tiff_offset  +  3 * sizeof (uint16_t)  +  2 * 2 * sizeof (uint32_t)  +  2 * (n_strips == 1 ? 0 : n_strips) * sizeof (uint32_t) };

    const uint64_t  file_size        { (uint64_t)strip1_offset + 3ULL * (uint64_t)(tiff.outimage_length) * (uint64_t)(tiff.outimage_width) };

    M_out( L"TIFF    : columns=%u  rows=%u  strips=%u  rows/strip=%u  size=%s=%.3fMB")
         % tiff.outimage_width
         % tiff.outimage_length
         % n_strips
         % rows_per_strip
         % M_sep(file_size)
         % M_meg(file_size)
         ;


    M__(M_out(L"rows_per_strip1 = %u  rows_per_strip9 = %u  strip1_byte_ct = %u  strip9_byte_ct = %u") % rows_per_strip1 % rows_per_strip9 % strip1_byte_ct % strip9_byte_ct;)
    M__(M_out(L"tiff_offset = %u  strip1_offset = %u") % tiff_offset % strip1_offset;)
   
    if (file_size >= 4ULL * 1024ULL * 1024ULL * 1024ULL)
    {
        M_out_emsg(L".tif file size too large = %s") % M_sep(file_size);
        return -1;
    }


    // set up buffers, etc.
    // --------------------
   
    buffer_C<uint8_t> buf(strip1_offset - tiff_offset + 100);
    buf.clear(); 
 //   std::vector<uint8_t> buf;
 //   buf.reserve(strip1_offset - tiff_offset + 100);                // buffer for all data except RGB pixels and IFH, IFD -- 100 extra bytes
 //   buf.assign( strip1_offset - tiff_offset + 100, (uint8_t)0);    // clear out at start   
      

    // set up IFH
    // ----------

    M_clearv(ifh)

    ifh.id1     = C_little_endian;
    ifh.id2     = C_little_endian;
    ifh.version = C_version;
    ifh.offset0 = sizeof ifh;             // IFD immediately follows IFH 


    // set up IFD entries
    // ------------------

    int32_t   tag_n      { 0 };
    uint32_t  buf_offset { 0 };          // offset unto buf for next overflow data addition      
    
    M_clearv(ifd)                        // clear out before starting

    M_tag_u16(C_new_subfile_type                                 , C_nft_default        )  // FE

    M_tag_u32(C_outimage_width                                      , tiff.outimage_width     )  // 100

    M_tag_u32(C_outimage_length                                     , tiff.outimage_length    )  // 101

    M_tag_o(  C_bits_per_sample               , C_uint16 , 3                            )  // 102
    M_put_u16(buf, 8)
    M_put_u16(buf, 8)
    M_put_u16(buf, 8)
    
    M_tag_u16(C_compression                                       , C_cmp_none          )  // 103

    M_tag_u16(C_photometric_interpretation                        , C_pi_rgb            )  // 106
    

    if (n_strips == 1)
    {
        M_tag_u32(  C_strip_offsets                               , strip1_offset       )  // 111 -- one strip 
    }
    else
    {
        M_tag_o(  C_strip_offsets                 , C_uint32 , n_strips                 )  // 111 -- multiple strips

        for (uint32_t i = 0; i < n_strips; i++)
        {
            M_put_u32(buf, strip1_offset + strip1_byte_ct * i)
        }
    }

    M_tag_u16(C_orientation                                       , tiff.orientation    )  // 112
   
    M_tag_u16(C_samples_per_pixel                                 , 3                   )  // 115
        
    M_tag_u32(C_rows_per_strip                                    , rows_per_strip      )  // 116
       

    if (n_strips == 1)
    {
        M_tag_u32(C_strip_byte_counts                             , strip9_byte_ct      )  // 117 -- one strip 
    }
    else
    {
        M_tag_o(  C_strip_byte_counts             , C_uint32 , n_strips                 )  // 117 -- multiple strips

        for (auto i = 1; i < n_strips; i++)
        {
            M_put_u32(buf, strip1_byte_ct)
        }
        M_put_u32(buf, strip9_byte_ct)
    }
    

    M_tag_o(  C_x_resolution                  , C_rational, 1                           )  // 11A
    M_put_u32(buf, tiff.x_resolution.num)
    M_put_u32(buf, tiff.x_resolution.den)

    M_tag_o(  C_y_resolution                  , C_rational, 1                           )  // 11B
    M_put_u32(buf, tiff.y_resolution.num)
    M_put_u32(buf, tiff.y_resolution.den)

    M_tag_u16(C_planar_configuration                              , C_pc_chunky_format  )  // 11C 

    M_tag_u16(C_resolution_unit                                   , tiff.resolution_unit)  // 128

    
    // Write IFH, IFD and overflow data out to file 
    // --------------------------------------------

    ftiff.write((char *)&ifh                , sizeof ifh        ); 
    ftiff.write((char *)&ifd_header         , sizeof ifd_header );
    ftiff.write((char *)&(ifd[0])           , sizeof ifd        );
    ftiff.write((char *)&ifd_trailer        , sizeof ifd_trailer);
    
    ftiff.write((char *)(buf.p1)            , buf_offset        );  // should have final incremented length of overflow items


    // Write RGB data  to .tiff file
    // ------------------------------
    
    ftiff.write((char *)(outimage.buf.p1), outimage.bytes);     
           
    
    // Close the output .tif file and return with good R/C
    // ---------------------------------------------------
    
    if (!(ftiff.good()))
    {
        M_out_emsg(L"write()s failed for file \"%s\"") % filename;  
        return -1;
    }

    ftiff.close();
    return rc; 
}   
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       writeout_bmp24() -- write outimage data to 24-bit .BMP file (builds BITMAPINFOHEADER and BITMAPFILEHEADER)
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int writeout_bmp24(const outimage_S& outimage, const std::wstring& basename) try
{
    int               rc {0}; 

    // Form complete filename for .bmp file

    const std::wstring     extname  {L".bmp"};
    std::wstring           filename {basename + extname};


    // Open output .bmp file using complete filename

    std::ofstream fbmp;
    fbmp.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    
    if (!(fbmp.good()))
    {
        M_out_emsg(L"Cannot open() \"%s\" for output") % filename;  
        return -1;
    }
    
    M_out(L"Output  : outimage file name = \"%s\"") % filename;  


    // Allocate rgb line buffer for doing writes 

    size_t write_ct   = M_roundup_p(3 * (outimage.cols    ), 4);  
    size_t alloc_size = (size_t)M_roundup_p(write_ct + 31, 16);   // allow extra space for padding 

    buffer_C<RGBTRIPLE> rgb(alloc_size);                        // allocate buffer -- size always in bytes when using constructor to allocate buffer

    
    // Put out .BMP headers info based on data in the raster_t structure -- for now, do not omit last row, col

    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    
    rc = bmp24_headers(&fh, &ih, outimage.cols, -(outimage.rows)); 

    if (rc != 0)
        return -1;     

    fbmp.write((char *)&fh, (std::streamsize)(sizeof fh)); 
    fbmp.write((char *)&ih, (std::streamsize)(sizeof ih)); 
    

    // Put out rows and columns of raster data (rows may need padding to even word length)
    
    for (auto row = 0; row < outimage.rows; row++)         // do all rows for now
    {         
         memcpy((void *)(rgb.p1), (void *)&(M_outimage_rgb(outimage, row, 0)), (size_t)(outimage.row_l));

         // reverse order of bytes from r/g/b to b/g/r

         uint8_t r, g, b;

         for (auto col = 0; col < outimage.cols; col++)
         {
             r = ((rgb_S *)(rgb.p1))[col].r;     // from outimage buffer
             g = ((rgb_S *)(rgb.p1))[col].g;     // from outimage buffer
             b = ((rgb_S *)(rgb.p1))[col].b;     // from outimage buffer

             M_PUT_RGBT(&((rgb.p)[col]), r, g, b)  // put back in RGBTRIPLE order
         }

         M_PUT_RGBT(&((rgb.p)[outimage.cols]), 0, 0, 0)                     // zero padding (may not be needed)

         fbmp.write((char *)(rgb.p1), (std::streamsize)write_ct);          
    }


    // Free RGB line buffer, close the output .bmp file and return with good R/C
    
    rgb.free(); 
    
    if (!(fbmp.good()))
    {
        M_out_emsg(L"write()s failed for file \"%s\"") % filename; 
        return -1;
    }
    
    fbmp.close();
    return rc; 
}
M_endf


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""