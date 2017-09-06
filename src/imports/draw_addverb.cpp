// draw_addverb.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ================
////     draw_addverb.cpp -- define drawing/graphics-oriented @VERBS, and do initial processing for them
////     ================
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

#define M_IN_DRAW_DLL

#include "h__common.h"

#include "h_ex_verbmac.h"
#include "h_ex_interface.h"

#include "h_draw_verb.h"


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   verb_xxxx() -- static functions to process draw-oriented verbs -- pointed to by verb table
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



// local MACRO with drawverb_xxx() function call for verb_xxx() functions  

#define M_drawverb(dv)                                                      \
{                                                                           \
    if (error_count() == 0)                                                 \
    {                                                                       \
        int  rc {0};                                                        \
        auto vrc = dv;                                                      \
        count_error(vrc);                                                   \
        if (vrc != 0) rc = -1;                                              \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        M_out_emsg(L"Bypassing %s call because of prior errors") % #dv;     \
    }                                                                       \
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DRAW_LINE -- draw line in outimage_S buffer (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_draw_line(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_draw_line() called");)
    M_drawverb( drawverb_draw_line(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DRAW_CIRCLE -- draw filled circle in outimage_S buffer (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_draw_circle(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_draw_circle() called");)
    M_drawverb( drawverb_draw_circle(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DRAW_RING -- draw ring (open circle) in outimage_S buffer (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_draw_ring(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_draw_ring() called");)
    M_drawverb( drawverb_draw_ring(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DRAW_BOX -- draw box in outimage_S buffer (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_draw_box(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_draw_box() called");)
    M_drawverb( drawverb_draw_box(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @PUT_BOX -- draw box directly in outimage_S buffer (bypass GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_put_box(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_put_box() called");)
    M_drawverb( drawverb_put_box(expression) )
    return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DRAW_FRAME -- draw frame in outimage_S buffer (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_draw_frame(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_draw_frame() called");)
    M_drawverb( drawverb_draw_frame(expression) )
    return 0;
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @PUT_FRAME -- draw frame directly in outimage_S buffer (bypass GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_put_frame(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_put_frame() called");)
    M_drawverb( drawverb_put_frame(expression) )
    return 0;
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DRAW_IMAGE -- read in image file and draw it in outimage_S buffer (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_draw_image(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_draw_image() called");)
    M_drawverb( drawverb_draw_image(expression) )
    return 0; 
}
M_endf

 
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @SET_FONT -- set font size, color, etc. for next @DRAW_TEXT call (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_set_font(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_set_font() called");)
    M_drawverb( drawverb_set_font(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @SET_FONT_SIZE -- update font size in current outimage_S active font (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_set_font_size(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_set_font_size() called");)
    M_drawverb( drawverb_set_font_size(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @SET_SET_TEXT_COLOR -- set current font/text color (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_set_text_color(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_set_text_color() called");)
    M_drawverb( drawverb_set_text_color(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @SET_SET_TEXT_BACKGROUND_COLOR -- set current font/text background color (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_set_text_background_color(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_set_text_background_color() called");)
    M_drawverb( drawverb_set_text_background_color(expression) )
    return 0;
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @SET_SET_NO_TEXT_COLOR -- set current font/text to no background (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_set_no_text_background(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_set_text_no_background() called");)
    M_drawverb( drawverb_set_no_text_background(expression) )
    return 0; 
}
M_endf     


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @ADD_PRIVATE_FONT -- add font file to private font collection (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_add_private_font(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_add_private_font() called");)
    M_drawverb( drawverb_add_private_font(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @ADD_PRIVATE_FONTS -- add font folder to private font collection (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_add_private_fonts(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_add_private_fonts() called");)
    M_drawverb( drawverb_add_private_fonts(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @REMOVE_PRIVATE_FONTS -- remove all fonts from private font collection (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_remove_private_fonts(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_remove_private_fonts() called");)
    M_drawverb( drawverb_remove_private_fonts(expression) )
    return 0; 
}
M_endf
 

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DISPLAY_PRIVATE_FONTS -- display list of fonts in private font collection (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_display_private_fonts(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_display_private_fonts() called");)
    M_drawverb( drawverb_display_private_fonts(expression) )
    return 0;
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DISPLAY_INSTALLED_FONTS -- display list of installed fonts (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_display_installed_fonts(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_display_instaleld_fonts() called");)
    M_drawverb( drawverb_display_installed_fonts(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DRAW_TEXT -- draw text line in outimage buffer (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_draw_text(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_draw_text() called");)
    M_drawverb( drawverb_draw_text(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @FORMAT_TEXT -- fofrmat text string withing supplied rectangle (use GDI+)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_format_text(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_format_text() called");)
    M_drawverb( drawverb_format_text(expression) )
    return 0; 
}
M_endf
  


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @PUT_GRAPHICS -- copy GDI+ graphics data into outimage buffer
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_put_graphics(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_put_graphics() called");)
    M_drawverb( drawverb_put_graphics(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @GET_GRAPHICS -- copy outimage buffer data into GDI+ graphics DIBSections
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_get_graphics(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_get_graphics() called");)
    M_drawverb( drawverb_get_graphics(expression) )
    return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @CLEAR_GRAPHICS -- reset GDI+ DIBSections to cleared-out color
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_clear_graphics(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_clear_graphics() called");)
    M_drawverb( drawverb_clear_graphics(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @CREATE_OUTIMAGE n_outimage -- create outimage structure (specified size)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_create_outimage(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_create_outimage() called");)
    M_drawverb( drawverb_create_outimage(expression) )
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @WRITE_OUTIMAGE n_outimage -- write outimage data out to file
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_write_outimage(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_write_outimage() called");)
    M_drawverb( drawverb_write_outimage(expression) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @EXTRACT_OUTIMAGE -- extract info from outimage structure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_extract_outimage(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"verb_extract_outimage() called");)
    M_drawverb( drawverb_extract_outimage(expression, value) )
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @DESTROY_OUTIMAGE n_outimage -- free up outimage structure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

static int verb_destroy_outimage(const e_expression_S& expression, value_S& value) try
{    
    M__(M_out(L"verb_destroy_outimage() called");)
    M_drawverb( drawverb_destroy_outimage(expression) )
    return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_draw_verbs() -- add map-oriented verbs to verb table
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void add_draw_verbs(const std::wstring& prefix) try
{
    // =====================================
    // @CREATE_OUTIMAGE 
    // =====================================

    {
        M_vt_nofix(                                       vt                                                    )       // no positional parms
        M_vt_int64rc_required_right_kw(                   vt, L"outimage_n" , 1         , 2                     )       // required -- outimage number    = 1 or 2
        M_vt_int64rc_optional_right_kw(                   vt, L"display"    , 0         , 1                     )       // debug display (1 = yes, 0 = no)
        M_vt_int64rc_required_right_kw(                   vt, L"rows"       , 10        , 50000                 )
        M_vt_int64rc_required_right_kw(                   vt, L"cols"       , 10        , 50000                 )

        M_vt_add_simplified(prefix + L"CREATE_OUTIMAGE",  vt, verb_create_outimage                              );
    }



    // =====================================
    // @WRITE_OUTIMAGE
    // =====================================

    {
        M_vt_nofix(                                             vt                                                                   )   // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                                  ,  1       , 2    )   // required -- outimage number    = 1 or 2
        M_vt_int64rc_optional_right_kw(                         vt, L"display"                                     ,  0       , 1    )   // debug display (1 = yes, 0 = no)
        M_vt_string_required_right_kw(                          vt, L"folder"                                                        )   // required -- complete path name -- no default
        M_vt_string_required_right_kw(                          vt, L"filename"                                                      )   // required -- complete filename -- no default (can include folders)
        M_vt_nval_optional_right_kw(                            vt, L"bmp_24"                                                        )   // if present, causes 24-bit .bmp output

        // tiff_24:[xx xx xx] keyword                
        {
            M_vt_nested_plist(                                                    npl, 0, 0                                          )
                                                                          
            M_vt_int64rc_optional_nest_kw(                                        npl, L"x_resolution"             ,   72    , 5760  )
            M_vt_int64rc_optional_nest_kw(                                        npl, L"y_resolution"             ,   72    , 5760  )
            M_vt_int64rc_optional_nest_kw(                                        npl, L"rows_per_strip"           ,   1     , 200000)
            M_vt_nval_optional_nest_kw(                                           npl, L"resolution_unit_inch"                       )
            M_vt_nval_optional_nest_kw(                                           npl, L"resolution_unit_cm"                         )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_top_left"                       )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_top_right"                      )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_bottom_left"                    )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_bottom_right"                   )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_left_top"                       )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_right_top"                      )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_left_bottom"                    )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_right_bottom"                   )
            M_vt_nval_optional_nest_kw(                                           npl, L"orientation_rot_90_ccw"                     )
                                                                                  
            M_vt_nest_conflict_pair(                                              npl, L"resolution_unit_inch", L"resolution_unit_cm")
            M_vt_nest_conflict_9way(                                              npl, L"orientation_top_left"      
                                                                                     , L"orientation_top_right"     
                                                                                     , L"orientation_bottom_left"   
                                                                                     , L"orientation_bottom_right"  
                                                                                     , L"orientation_left_top"      
                                                                                     , L"orientation_right_top"     
                                                                                     , L"orientation_left_bottom"  
                                                                                     , L"orientation_right_bottom"   
                                                                                     , L"orientation_rot_90_ccw"    
                                                                                                                                     )

            M_vt_vlist_optional_right_kw(                       vt, L"tiff_24",   npl                                                )               // add in nested plist to right keyword
        }

        M_vt_right_conflict_pair(                               vt, L"tiff_24", L"bmp_24"                                            )
        M_vt_add_simplified(prefix + L"WRITE_OUTIMAGE",         vt, verb_write_outimage                                              );
    }
  


     
    // =====================================
    // @DRAW_LINE
    // =====================================

    {
        M_vt_nofix(                                             vt                                                         )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2     )  // required -- outimage number = 1 or 2
                                                              
        M_vt_int64rc_required_right_kw(                         vt, L"row1"                           ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"col1"                           ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"row2"                           ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"col2"                           ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"thickness"                      ,   1      ,   40000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"alpha"                          ,   0      ,   255   )  // optional -- alpha for line
                                                              
        {   //  color: [r g b]]                               
            M_vt_nested_plist(                                                                     npl,   3       ,      3 )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
                                                                                                    
            M_vt_vlist_required_right_kw(                       vt, L"color"                     , npl                     )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"DRAW_LINE",              vt, verb_draw_line                                         );
    }




    // =====================================
    // @DRAW_CIRCLE
    // =====================================

    {
        M_vt_nofix(                                             vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
                                                              
        M_vt_int64rc_required_right_kw(                         vt, L"row"                              ,   0  ,   90000 )  // row    of center point
        M_vt_int64rc_required_right_kw(                         vt, L"col"                              ,   0  ,   90000 )  // column of center point
        M_vt_int64rc_required_right_kw(                         vt, L"radius"                           ,   1  ,   45000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"alpha"                            ,   0  ,   255   )  // optional -- alpha for circle
                                                              
        {   //  color: [r g b]]                               
            M_vt_nested_plist(                                                                      npl ,   3  ,      3  )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
                                                                                                    
            M_vt_vlist_required_right_kw(                       vt, L"color"                      , npl                  )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"DRAW_CIRCLE",            vt, verb_draw_circle                                     );
    }


    // =====================================
    // @DRAW_RING
    // =====================================

    {
        M_vt_nofix(                                             vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
                                                             
        M_vt_int64rc_required_right_kw(                         vt, L"row"                              ,   0  ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"col"                              ,   0  ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"radius"                           ,   1  ,   45000 )
        M_vt_int64rc_required_right_kw(                         vt, L"thickness"                        ,   1  ,   45000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"alpha"                            ,   0  ,   255   )  // optional -- alpha for ring
                                                             
        {   //  color: [r g b]]                              
            M_vt_nested_plist(                                                                      npl ,   3  ,      3  )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
                                                                                                    
            M_vt_vlist_required_right_kw(                       vt, L"color"                      , npl                  )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"DRAW_RING",              vt, verb_draw_ring                                       );
    }



    // =====================================
    // @DRAW_BOX
    // =====================================

    {
        M_vt_nofix(                                             vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
                                                              
        M_vt_int64rc_required_right_kw(                         vt, L"top"                              ,   0  ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"left"                             ,   0  ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"width"                            ,   3  ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"height"                           ,   3  ,   90000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"alpha"                            ,   0  ,   255   )  // optional -- alpha for box
                                                              
        {   //  color: [r g b]]                               
            M_vt_nested_plist(                                                                      npl ,   3  ,      3  )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
                                                                                                    
            M_vt_vlist_required_right_kw(                       vt, L"color"                      , npl                  )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"DRAW_BOX",               vt, verb_draw_box                                        );
    }


    // =====================================
    // @PUT_BOX
    // =====================================

    {
        M_vt_nofix(                                             vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
                                                              
        M_vt_int64rc_required_right_kw(                         vt, L"top"                              ,   0  ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"left"                             ,   0  ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"width"                            ,   3  ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"height"                           ,   3  ,   90000 )
                                                              
        {   //  color: [r g b]]                               
            M_vt_nested_plist(                                                                      npl ,   3  ,      3  )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
            M_vt_int64rc_nest_pos(                                                                  npl ,   0  ,    255  )
                                                                                                    
            M_vt_vlist_required_right_kw(                       vt, L"color"                      , npl                  )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"PUT_BOX",                vt, verb_put_box                                         );
    }


    // =====================================
    // @DRAW_FRAME
    // =====================================

    {
        M_vt_nofix(                                             vt                                                         )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2     )  // required -- outimage number = 1 or 2
                                                              
        M_vt_int64rc_required_right_kw(                         vt, L"top"                            ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"left"                           ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"width"                          ,   3      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"height"                         ,   3      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"thickness"                      ,   1      ,   40000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"alpha"                          ,   0      ,   255   )  // optional -- alpha for frame
                                                              
        {   //  color: [r g b]]                               
            M_vt_nested_plist(                                                                     npl,   3       ,      3 )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
                                                                                                    
            M_vt_vlist_required_right_kw(                       vt, L"color"                     , npl                     )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"DRAW_FRAME",             vt, verb_draw_frame                                        );
    }
    

    // =====================================
    // @PUT_FRAME
    // =====================================

    {
        M_vt_nofix(                         vt                                                         )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2     )  // required -- outimage number = 1 or 2
                                                              
        M_vt_int64rc_required_right_kw(                         vt, L"top"                            ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"left"                           ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"width"                          ,   3      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"height"                         ,   3      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"thickness"                      ,   1      ,   40000 )
                                                              
        {   //  color: [r g b]]                               
            M_vt_nested_plist(                                                                     npl,   3       ,      3 )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
                                                                                                    
            M_vt_vlist_required_right_kw(                       vt, L"color"                     , npl                     )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"PUT_FRAME",              vt, verb_put_frame                                         );
    }


    // =====================================
    // @DRAW_IMAGE
    // =====================================

    {
        M_vt_nofix(                                             vt                                                         )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2     )  // required -- outimage number = 1 or 2
        M_vt_string_required_right_kw(                          vt, L"filename"                                            )  // required -- filename (with drive, path, .ttf etc.) -- no default 
        M_vt_int64rc_optional_right_kw(                         vt, L"display"                        ,   0      ,   1     )  // debug display (1 = yes, 0 = no)
                                                               
        M_vt_int64rc_required_right_kw(                         vt, L"dest_row"                       ,   0      ,   90000 )
        M_vt_int64rc_required_right_kw(                         vt, L"dest_col"                       ,   0      ,   90000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"dest_width"                     ,   1      ,   90000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"dest_height"                    ,   1      ,   90000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"src_row"                        ,   0      ,   90000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"src_col"                        ,   0      ,   90000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"src_width"                      ,   1      ,   90000 )
        M_vt_int64rc_optional_right_kw(                         vt, L"src_height"                     ,   1      ,   90000 ) 

        {   //  trans_color1: [r g b]]    -- transparent color 1 (optional)
            M_vt_nested_plist(                                                                     npl,   3       ,      3 )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
                                                                                                    
            M_vt_vlist_optional_right_kw(                       vt, L"trans_color1"              , npl                     )  // add in nested plist to right parms
        }
  
        {   //  trans_color2: [r g b]]    -- transparent color 2 (optional)
            M_vt_nested_plist(                                                                     npl,   3       ,      3 )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255 )
                                                                                                    
            M_vt_vlist_optional_right_kw(                       vt, L"trans_color2"              , npl                     )  // add in nested plist to right parms
        }


        M_vt_right_match_pair(                                  vt, L"trans_color1"  , L"trans_color2"                     )    
        M_vt_right_match_pair(                                  vt, L"dest_w"  , L"dest_h"                                 )                
        M_vt_right_match_4way(                                  vt, L"src_row" , L"src_col" , L"src_width" , L"src_height" )                
      

        M_vt_add_simplified(prefix + L"DRAW_IMAGE",             vt, verb_draw_image                                        );
    }
  

    // =====================================
    // @DISPLAY_INSTALLED_FONTS
    // =====================================

    {
        M_vt_nofix(                                              vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                          vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
         
        M_vt_add_simplified(prefix + L"DISPLAY_INSTALLED_FONTS", vt, verb_display_installed_fonts                         );
    }


    // =====================================
    // @DISPLAY_PRIVATE_FONTS
    // =====================================

    {
        M_vt_nofix(                                              vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                          vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
         
        M_vt_add_simplified(prefix + L"DISPLAY_PRIVATE_FONTS",   vt, verb_display_private_fonts                           );
    }


    // =====================================
    // @ADD_PRIVATE_FONT
    // =====================================

    {
        M_vt_nofix(                                              vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                          vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
        M_vt_string_required_right_kw(                           vt, L"filename"                                          )  // required -- filename (with drive, path, .ttf etc.) -- no default 

        M_vt_add_simplified(prefix + L"ADD_PRIVATE_FONT",        vt, verb_add_private_font                                );
    }

    
    // =====================================
    // @ADD_PRIVATE_FONTS
    // =====================================

    {
        M_vt_nofix(                                              vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                          vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
        M_vt_string_required_right_kw(                           vt, L"folder"                                            )  // required -- folder name (with drive, path, final \ etc.) -- no default 
        M_vt_nval_optional_right_kw(                             vt, L"nested"                                            )  // add fonts in nested subfolders, too

        M_vt_add_simplified(prefix + L"ADD_PRIVATE_FONTS",       vt, verb_add_private_fonts                               );
    }


    // =====================================
    // @REMOVE_PRIVATE_FONTS
    // =====================================

    {
        M_vt_nofix(                                     vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                 vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
         
        M_vt_add_simplified(L"REMOVE_PRIVATE_FONTS",    vt, verb_remove_private_fonts                            );
    }



    // =====================================
    // @SET_TEXT_COLOR
    // =====================================

    {
        M_vt_nofix(                                             vt                                                           )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2       )  // required -- outimage number = 1 or 2
                                                               
        M_vt_int64rc_optional_right_kw(                         vt, L"alpha"                          ,   0      ,   255     )  // optional -- alpha for text color
                                                               
        {   //  color: [r g b]]                                
            M_vt_nested_plist(                                                                     npl,   3       ,      3   )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255   )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255   )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255   )
                                                                                                    
            M_vt_vlist_optional_right_kw(                       vt, L"color"                     , npl                       )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"SET_TEXT_COLOR",         vt, verb_set_text_color                                      );
    }


    // =====================================
    // @SET_TEXT_BACKGROUND_COLOR
    // =====================================

    {
        M_vt_nofix(                                                   vt                                                           )  // no positional parms
        M_vt_int64rc_required_right_kw(                               vt, L"outimage_n"                     ,   1      ,   2       )  // required -- outimage number = 1 or 2
                                                                     
        M_vt_int64rc_optional_right_kw(                               vt, L"alpha"                          ,   0      ,   255     )  // optional -- alpha for text color
                                                                     
        {   //  color: [r g b]]                                      
            M_vt_nested_plist(                                                                           npl,   3       ,      3   )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                       npl,   0       ,    255   )
            M_vt_int64rc_nest_pos(                                                                       npl,   0       ,    255   )
            M_vt_int64rc_nest_pos(                                                                       npl,   0       ,    255   )
                                                                                                          
            M_vt_vlist_optional_right_kw(                             vt, L"color"                     , npl                       )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"SET_TEXT_BACKGROUND_COLOR",    vt, verb_set_text_background_color                           );
    }


    // =====================================
    // @SET_NO_TEXT_BACKGROUND
    // =====================================

    {
        M_vt_nofix(                                              vt                                                       )  // no positional parms
        M_vt_int64rc_required_right_kw(                          vt, L"outimage_n"                       ,   1  , 2       )  // required -- outimage number = 1 or 2
         
        M_vt_add_simplified(prefix + L"SET_NO_TEXT_BACKGROUND",  vt, verb_set_no_text_background                          );
    } 


    // =====================================
    // @SET_FONT
    // =====================================

    {
        M_vt_nofix(                                             vt                                                           )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2       )  // required -- outimage number = 1 or 2
                                                               
        M_vt_string_required_right_kw(                          vt, L"family_name"                                           )  // required -- font family name -- no default
        M_vt_nval_optional_right_kw(                            vt, L"private"                                               )  // use font in private collection
        M_vt_float64rc_optional_right_kw(                       vt, L"em_size"                        ,   4.0    ,   45000.0 )  // em_size is optional
        M_vt_nval_optional_right_kw(                            vt, L"bold"                                                  )
        M_vt_nval_optional_right_kw(                            vt, L"italic"                                                )
        M_vt_nval_optional_right_kw(                            vt, L"underline"                                             )
        M_vt_nval_optional_right_kw(                            vt, L"strikeout"                                             )
                                                               
        {   //  color: [r g b]]                                
            M_vt_nested_plist(                                                                     npl,   3       ,      3   )  // exactly 3 positional sub-parms required
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255   )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255   )
            M_vt_int64rc_nest_pos(                                                                 npl,   0       ,    255   )
                                                                                                    
            M_vt_vlist_optional_right_kw(                       vt, L"color"                     , npl                       )  // add in nested plist to right parms
        }
  
        M_vt_add_simplified(prefix + L"SET_FONT",               vt, verb_set_font                                            );
    }


    // =====================================
    // @SET_FONT_SIZE
    // =====================================

    {
        M_vt_nofix(                                             vt                                                           )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2       )  // required -- outimage number = 1 or 2
                                                               
        M_vt_float64rc_required_right_kw(                       vt, L"em_size"                        ,   4.0    ,   45000.0 )  // em_size is required
  
        M_vt_add_simplified(prefix + L"SET_FONT_SIZE",          vt, verb_set_font_size                                       );
    }



    // =====================================
    // @DRAW_TEXT
    // =====================================

    {
        M_vt_nofix(                                             vt                                                           )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2       )  // required -- outimage number = 1 or 2
                                                               
        M_vt_string_required_right_kw(                          vt, L"text"                                                  )   // required -- text string to draw (no default)
        M_vt_int64rc_required_right_kw(                         vt, L"row"                            ,   0      ,   90000   )   // required -- starting row
        M_vt_int64rc_required_right_kw(                         vt, L"col"                            ,   0      ,   90000   )   // required -- starting column
        M_vt_float64rc_optional_right_kw(                       vt, L"rotation"                       ,   -360.0 ,   360.0   )
        M_vt_float64rc_optional_right_kw(                       vt, L"width"                          ,   0.0001 ,  1000.0   )
        M_vt_float64rc_optional_right_kw(                       vt, L"height"                         ,   0.0001 ,  1000.0   )
        M_vt_float64rc_optional_right_kw(                       vt, L"x_shear"                        , -1000.0  ,  1000.0   )
        M_vt_float64rc_optional_right_kw(                       vt, L"y_shear"                        , -1000.0  ,  1000.0   )
        M_vt_float64rc_optional_right_kw(                       vt, L"extra_space"                    , -1000.0  ,  1000.0   )
  
        M_vt_add_simplified(prefix + L"DRAW_TEXT",              vt, verb_draw_text                                           );
    }


    // =====================================
    // @FORMAT_TEXT
    // =====================================

    {
        M_vt_nofix(                                             vt                                                           )  // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n"                     ,   1      ,   2       )  // required -- outimage number = 1 or 2
                                                              
        M_vt_string_required_right_kw(                          vt, L"text"                                                  )   // required -- text string to draw (no default)
        M_vt_int64rc_required_right_kw(                         vt, L"top"                            ,   0      ,   90000   )   // required -- top of rectangle
        M_vt_int64rc_required_right_kw(                         vt, L"left"                           ,   0      ,   90000   )   // required -- left side of rectangle
        M_vt_int64rc_required_right_kw(                         vt, L"width"                          ,   1      ,   90000   )   // required -- width of rectangle         
        M_vt_int64rc_required_right_kw(                         vt, L"height"                         ,   2      ,   90000   )   // required -- height of rectangle    
  
        M_vt_add_simplified(prefix + L"FORMAT_TEXT",            vt, verb_format_text                                         );
    }



    // =====================================
    // @PUT_GRAPHICS    
    // =====================================

    {
        M_vt_nofix(                                             vt                          )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n",  1, 2    )                                       // required -- outimage number    = 1 or 2

        M_vt_add_simplified(prefix + L"PUT_GRAPHICS",           vt, verb_put_graphics       );
    }


    // =====================================
    // @GET_GRAPHICS    
    // =====================================

    {
        M_vt_nofix(                                             vt                          )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n",  1, 2    )                                       // required -- outimage number    = 1 or 2
                                                               
        M_vt_add_simplified(prefix + L"GET_GRAPHICS",           vt, verb_get_graphics       );
    }      


    // =====================================
    // @CLEAR_GRAPHICS    
    // =====================================

    {
        M_vt_nofix(                                             vt                          )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n",  1, 2    )                                       // required -- outimage number    = 1 or 2

        M_vt_add_simplified(prefix + L"CLEAR_GRAPHICS",         vt, verb_clear_graphics     );
    }


    // =====================================
    // @EXTRACT_OUTIMAGE 
    // =====================================

    {
        M_vt_nofix(                                             vt                                      )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n",    1, 2              )                                       // required -- outimage number = 1 or 2
        M_vt_nval_optional_right_kw(                            vt, L"cols"                             )                                       // fields to extract -- only one allowed
        M_vt_nval_optional_right_kw(                            vt, L"rows"                             )
        M_vt_nval_optional_right_kw(                            vt, L"pixels"                           )
        M_vt_nval_optional_right_kw(                            vt, L"bytes"                            )
        M_vt_nval_optional_right_kw(                            vt, L"installed_font_ct"                )
        M_vt_nval_optional_right_kw(                            vt, L"private_font_ct"                  )
        M_vt_int64rc_optional_right_kw(                         vt, L"installed_font_name" , 0, 100000  )                                       // private_font_name:n
        M_vt_int64rc_optional_right_kw(                         vt, L"private_font_name"   , 0, 100000  )                                       // private_font_name:n
                                                              
        M_vt_right_conflict_8way(                               vt, L"cols" , L"rows", L"pixels", L"bytes", L"installed_font_ct", L"private_font_ct", L"installed_font_name", L"private_font_name")
                                                                                        
        M_vt_add_simplified(prefix + L"EXTRACT_OUTIMAGE",       vt, verb_extract_outimage               );
    }
     


    // =====================================
    // @DESTROY_OUTIMAGE    
    // =====================================

    {
        M_vt_nofix(                                             vt                                )                                       // no positional parms
        M_vt_int64rc_required_right_kw(                         vt, L"outimage_n" ,  1, 2         )                                       // required -- outimage number    = 1 or 2

        M_vt_add_simplified(prefix + L"DESTROY_OUTIMAGE",       vt, verb_destroy_outimage         );
    }
    
    // =====================================

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return; 
}
M_endf


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""