// draw_verb.c

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     draw_verb.cpp -- process draw-oriented verbs -- called by corresponding static functions in draw_addverb.cpp
////     =============
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

#include "h_ex_interface.h"

#include "h_draw_verb.h" 
#include "h_draw_canvas.h"
#include "h_draw_outimage.h"
#include "h_draw_tiff.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
//
//   drawverb_color_vlist_rgb() -- internal helper function to process rgb values in color vlist 
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void drawverb_color_vlist_rgb(const vlist_S vlist, rgb_S& rgb) try
{
    // known that there are 3 integer positional parms 0-255 in passed-in vlist

    rgb.r = (uint8_t)(vlist.values.at(0).int64); 
    rgb.g = (uint8_t)(vlist.values.at(1).int64); 
    rgb.b = (uint8_t)(vlist.values.at(2).int64); 
   	M__(M_out(L"drawverb_color_vlist_rgb() -- r=%d g=%d b=%d") % rgb.r % rgb.g % rgb.b;)
    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_draw_line() -- draw line in outimage area  (use GDI+)
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_draw_line(const e_expression_S& expression) try
{
    M__(M_out(         L"drawverb_draw_line() called");) 
    M_get_outimage_n(  L"drawverb_draw_line"           )
    M_check_outimage_n(L"DRAW_LINE"                    )

    M_dv_some(M_out(L"=======> Doing   @DRAW_LINE outimage_n:%d    <=======") % n_outimage;) 
               

    // get line drawing parms from options
    // -----------------------------------

    rc_T     row1      {    };
    rc_T     col1      {    };
    rc_T     row2      {    };
    rc_T     col2      {    };
    rc_T     thickness {    };
    rgb_S    color     {    };
    alpha_T  alpha     {255U}; 
     
    M_get_right_keyword_int64(    expression, L"row1"             , row1        , rc_T     )
    M_get_right_keyword_int64(    expression, L"col1"             , col1        , rc_T     )
    M_get_right_keyword_int64(    expression, L"row2"             , row2        , rc_T     )
    M_get_right_keyword_int64(    expression, L"col2"             , col2        , rc_T     )
    M_get_right_keyword_int64(    expression, L"thickness"        , thickness   , rc_T     )
    M_get_right_keyword_int64(    expression, L"alpha"            , alpha       , alpha_T  )

    if (expression.rparms.eval_kws.count(L"color"     ) > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);
      

    // call draw_line() to draw the line in the outimage structure 
    // -----------------------------------------------------------

    auto rc = draw_line(*outimage_p, color, alpha, thickness, col1, row1, col2, row2);    

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_draw_box() -- draw solid color box in outimage area (use GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_draw_box(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_draw_box() called");) 
    
    M_get_outimage_n(L"drawverb_draw_box")
    M_check_outimage_n(L"DRAW_BOX")
   
 
    M_dv_some(M_out(L"=======> Doing   @DRAW_BOX outimage_n:%d <=======") % n_outimage;)
               

    // get box drawing parms from options
    // -----------------------------------

    rc_T     top      {    };
    rc_T     left     {    };
    rc_T     width    {    };
    rc_T     height   {    };
    rgb_S    color    {    };
    alpha_T  alpha    {255U}; 
    
    M_get_right_keyword_int64(    expression, L"top"             , top         , rc_T     )
    M_get_right_keyword_int64(    expression, L"left"            , left        , rc_T     )
    M_get_right_keyword_int64(    expression, L"width"           , width       , rc_T     )
    M_get_right_keyword_int64(    expression, L"height"          , height      , rc_T     )
    M_get_right_keyword_int64(    expression, L"alpha"           , alpha       , alpha_T  )

    if (expression.rparms.eval_kws.count(L"color"     ) > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);
 

    // call draw_box() to draw the box in the outimage structure 
    // ---------------------------------------------------------

    auto rc = draw_box(*outimage_p, color, alpha, top, left, width, height);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_put_box() -- draw solid color box directly in outimage area (bypass GDI+)  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_put_box(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_put_box() called");) 
    
    M_get_outimage_n(L"drawverb_put_box")
    M_check_outimage_n(L"PUT_BOX")
   
    M_dv_some(M_out(L"=======> Doing   @PUT_BOX outimage_n:%d <=======") % n_outimage;)
               

    // get box drawing parms from options
    // -----------------------------------

    rc_T  top      {};
    rc_T  left     {};
    rc_T  width    {};
    rc_T  height   {};
    rgb_S color    {}; 
    
    M_get_right_keyword_int64(    expression, L"top"             , top         , rc_T  )
    M_get_right_keyword_int64(    expression, L"left"            , left        , rc_T  )
    M_get_right_keyword_int64(    expression, L"width"           , width       , rc_T  )
    M_get_right_keyword_int64(    expression, L"height"          , height      , rc_T  )

    if (expression.rparms.eval_kws.count(L"color"     ) > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);
 

    // call put_box() to draw the box in the outimage structure
    // --------------------------------------------------------

    auto rc = put_box(*outimage_p, top, left, width, height, color);    

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_draw_circle() -- draw filled-in circle in outimage area (use GDI+)
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_draw_circle(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_draw_circle) called");) 
    M_get_outimage_n(L"drawverb_draw_circle")
    M_check_outimage_n(L"DRAW_CIRCLE")
  
    M_dv_some(M_out(L"=======> Doing   @DRAW_CIRCLE outimage_n:%d    <=======") % n_outimage;)
               

    // get circle drawing parms from options
    // -------------------------------------

    rc_T    row       {    };
    rc_T    col       {    };
    rc_T    radius    {    };
    rgb_S   color     {    };
    alpha_T alpha     {255U}; 
     
    M_get_right_keyword_int64(    expression, L"row"             , row         , rc_T     )
    M_get_right_keyword_int64(    expression, L"col"             , col         , rc_T     )
    M_get_right_keyword_int64(    expression, L"radius"          , radius      , rc_T     )
    M_get_right_keyword_int64(    expression, L"alpha"           , alpha       , alpha_T  )

    if (expression.rparms.eval_kws.count(L"color"     ) > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);
      

    // call draw_circle() to draw the circle in the GDI+ DIBSections
    // -------------------------------------------------------------

    auto rc = draw_circle(*outimage_p, color, alpha, row, col, radius);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_draw_ring() -- draw hollow circle in outimage area (use GDI+)
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_draw_ring(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_draw_ring) called");) 
    M_get_outimage_n(L"drawverb_draw_ring")
    M_check_outimage_n(L"DRAW_RING")
  
    M_dv_some(M_out(L"=======> Doing   @DRAW_RING outimage_n:%d    <=======") % n_outimage;)
               

    // get circle drawing parms from options
    // -------------------------------------

    rc_T    row       {    };
    rc_T    col       {    };
    rc_T    radius    {    };
    rc_T    thickness {    };
    rgb_S   color     {    };
    alpha_T alpha     {255U}; 
     
    M_get_right_keyword_int64(    expression, L"row"             , row         , rc_T     )
    M_get_right_keyword_int64(    expression, L"col"             , col         , rc_T     )
    M_get_right_keyword_int64(    expression, L"radius"          , radius      , rc_T     ) 
    M_get_right_keyword_int64(    expression, L"thickness"       , thickness   , rc_T     )
    M_get_right_keyword_int64(    expression, L"alpha"           , alpha       , alpha_T  )

    if (expression.rparms.eval_kws.count(L"color"     ) > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);
      

    // call draw_ring() to draw the ring in the GDI+ DIBSections
    // ---------------------------------------------------------

    auto rc = draw_ring(*outimage_p, color, alpha, thickness, row, col, radius);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_draw_frame() -- draw hollow frame in outimage area  (use GDI+)
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_draw_frame(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_draw_frame) called");) 
    M_get_outimage_n(L"drawverb_draw_frame")
    M_check_outimage_n(L"DRAW_FRAME")
     
    M_dv_some(M_out(L"=======> Doing   @DRAW_FRAME outimage_n:%d    <=======") % n_outimage;)
               

    // get frame drawing parms from options
    // ------------------------------------

    rc_T    top       {    };
    rc_T    left      {    };
    rc_T    width     {    };
    rc_T    height    {    };
    rc_T    thickness {    };
    rgb_S   color     {    };
    alpha_T alpha     {255U}; 
        
    M_get_right_keyword_int64(    expression, L"top"             , top         , rc_T     )
    M_get_right_keyword_int64(    expression, L"left"            , left        , rc_T     )
    M_get_right_keyword_int64(    expression, L"width"           , width       , rc_T     )
    M_get_right_keyword_int64(    expression, L"height"          , height      , rc_T     )
    M_get_right_keyword_int64(    expression, L"thickness"       , thickness   , rc_T     )
    M_get_right_keyword_int64(    expression, L"alpha"           , alpha       , alpha_T  )

    if (expression.rparms.eval_kws.count(L"color"     ) > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);
      

    // call draw_frame() to draw the frame in the outimage structure
    // -------------------------------------------------------------

    auto rc = draw_frame(*outimage_p, color, alpha, thickness, top, left, width, height);    

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_put_frame() -- draw hollow frame directly in outimage area (bypass GDI+)  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_put_frame(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_put_frame) called");) 
    M_get_outimage_n(L"drawverb_put_frame")
    M_check_outimage_n(L"PUT_FRAME")
  
    M_dv_some(M_out(L"=======> Doing   @PUT_FRAME outimage_n:%d    <=======") % n_outimage;)
               

    // get frame putting parms from options
    // ------------------------------------

    rc_T  top       {};
    rc_T  left      {};
    rc_T  width     {};
    rc_T  height    {};
    rc_T  thickness {};
    rgb_S color     {}; 
     
    M_get_right_keyword_int64(    expression, L"top"             , top         , rc_T  )
    M_get_right_keyword_int64(    expression, L"left"            , left        , rc_T  )
    M_get_right_keyword_int64(    expression, L"width"           , width       , rc_T  )
    M_get_right_keyword_int64(    expression, L"height"          , height      , rc_T  )
    M_get_right_keyword_int64(    expression, L"thickness"       , thickness   , rc_T  )

    if (expression.rparms.eval_kws.count(L"color"     ) > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);
      

    // call put_frame() to draw the frame in the outimage structure 
    // ------------------------------------------------------------

    auto rc = put_frame(*outimage_p, top, left, width, height, thickness, color);    

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_draw_image() -- read in image file and draw it into outimage (use GDI+)  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_draw_image(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_draw_image() called");) 
    M_get_outimage_n(L"drawverb_draw_image")
    M_check_outimage_n(L"DRAW_IMAGE")
   
    M_dv_few(M_out(L"=======> Doing   @DRAW_IMAGE outimage_n:%d <=======") % n_outimage;)
               

    // get image drawing parms from options
    // ------------------------------------

    draw_image_parm_S di_parm {};                           // set up with default values
   
    M_get_right_keyword_int64(    expression, L"display"         , di_parm.display     , bool  )
    M_get_right_keyword_string(   expression, L"filename"        , di_parm.filename            )
    M_get_right_keyword_int64(    expression, L"dest_row"        , di_parm.dest_row    , rc_T  )
    M_get_right_keyword_int64(    expression, L"dest_col"        , di_parm.dest_col    , rc_T  )
    M_get_right_keyword_int64(    expression, L"dest_width"      , di_parm.dest_w      , rc_T  )
    M_get_right_keyword_int64(    expression, L"dest_height"     , di_parm.dest_h      , rc_T  )
    M_get_right_keyword_int64(    expression, L"src_row"         , di_parm.src_row     , rc_T  )
    M_get_right_keyword_int64(    expression, L"src_col"         , di_parm.src_col     , rc_T  )
    M_get_right_keyword_int64(    expression, L"src_width"       , di_parm.src_w       , rc_T  )
    M_get_right_keyword_int64(    expression, L"src_height"      , di_parm.src_h       , rc_T  )

    if (expression.rparms.eval_kws.count(L"trans_color1") > 0) 
    {
        drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"trans_color1"   )->second.vlist_sp), di_parm.trans_color1);
        di_parm.trans_color_valid = true; 
    }
  
    if (expression.rparms.eval_kws.count(L"trans_color2") > 0) 
    {
        drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"trans_color2"   )->second.vlist_sp), di_parm.trans_color2);
        di_parm.trans_color_valid = true; 
    }




    // call draw_image() to read in the file and draw the image in the outimage structure 
    // ----------------------------------------------------------------------------------

    auto rc = draw_image(*outimage_p, di_parm);    

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf




///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_display_installed_fonts() -- display all installed fonts (use GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_display_installed_fonts(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_display_installed_fonts() called");) 
    
    M_get_outimage_n(L"drawverb_display_installed_fonts")
    M_check_outimage_n(L"DISPLAY_INSTALLED_FONTS")
  
    M_dv_few(M_out(L"=======> Doing   @DISPLAY_INSTALLED_FONTS outimage_n:%d <=======") % n_outimage;)
               
     

    // call display_installed_fonts() to display family names for all installed fonts 
    // ------------------------------------------------------------------------------

    auto rc = display_installed_fonts(*outimage_p);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_display_private_fonts() -- display all fonts in private colelction (use GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_display_private_fonts(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_display_private_fonts() called");) 
    
    M_get_outimage_n(L"drawverb_display_private_fonts")
    M_check_outimage_n(L"DISPLAY_PRIVATE_FONTS")
  
    M_dv_few(M_out(L"=======> Doing   @DISPLAY_PRIVATE_FONTS outimage_n:%d <=======") % n_outimage;)
               
     

    // call display_private_fonts() to display family names for all fonts in private collection 
    // ----------------------------------------------------------------------------------------

    auto rc = display_private_fonts(*outimage_p);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_add_private_font() -- add font to private collection (use GDI+) -- one file
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_add_private_font(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_add_private_font() called");) 
    
    M_get_outimage_n(L"drawverb_add_private_font")
    M_check_outimage_n(L"ADD_PRIVATE_FONT")

    M_dv_some(M_out(L"=======> Doing   @ADD_PRIVATE_FONT outimage_n:%d <=======") % n_outimage;)
               
     
    // get filename from passed-in arguments (filename is amndatory, so it shuold be present)
    // -------------------------------------

    std::wstring filename {};  
    M_get_right_keyword_string(expression, L"filename", filename)


    // call add_private_font() to add this font to private font collection 
    // -------------------------------------------------------------------

    auto rc = add_private_font(*outimage_p, filename);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_add_private_fonts() -- add fonts to private collection (use GDI+) - all fonts in folder  (may include subfolders)
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_add_private_fonts(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_add_private_fonts() called");) 
    
    M_get_outimage_n(L"drawverb_add_private_fonts")
    M_check_outimage_n(L"ADD_PRIVATE_FONTS")
   
     M_dv_few(M_out(L"=======> Doing   @ADD_PRIVATE_FONTS outimage_n:%d <=======") % n_outimage;)
               
     
    // get filename from passed-in arguments (filename is mandatory, so it should be present)
    // -------------------------------------

    std::wstring folder {     };
    bool         nested {false}; 

    M_get_right_keyword_string(expression,    L"folder",    folder        )
    M_get_right_keyword_nval(  expression,    L"nested",    nested,   true)


    // call add_private_fonts() to add this font folder to private font collection 
    // ---------------------------------------------------------------------------

    auto rc = add_private_fonts(*outimage_p, folder, nested);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



  
///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_remove_private_fonts() -- remove all fonts in private collection (use GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_remove_private_fonts(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_remove_private_fonts() called");) 
    
    M_get_outimage_n(L"drawverb_remove_private_fonts")
    M_check_outimage_n(L"REMOVE_PRIVATE_FONTS")

    M_dv_few(M_out(L"=======> Doing   @REMOVE_PRIVATE_FONTS outimage_n:%d <=======") % n_outimage;)
               
     

    // call remove_private_fonts() to remove all fonts in private font collection 
    // --------------------------------------------------------------------------

    auto rc = remove_private_fonts(*outimage_p);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf




///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_set_font() -- set font family, size, style, etc. (uses GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_set_font(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_set_font() called");) 
    
    M_get_outimage_n(L"drawverb_set_font")
    M_check_outimage_n(L"SET_FONT")
  
    M_dv_some(M_out(L"=======> Doing   @SET_FONT outimage_n:%d <=======") % n_outimage;)
    
    std::wstring   family_name   {}; 
    bool           installed     {true};                          // default - use installed fonts (not private font collection)
    real_T         em_size       {16.0};                          // default size (very small)  
    uint32_t       style         {Gdiplus::FontStyleRegular};     // default style = regular
    uint32_t       style_bold    {0};                             // default style = not bold
    uint32_t       style_italic  {0};                             // default style = not italic
    uint32_t       style_ul      {0};                             // default style = not underlined
    uint32_t       style_so      {0};                             // default style = not strikeout
    bool           set_color     {false};                         // true = color:[] keyword present
    rgb_S          color         {}; 
     
    M_get_right_keyword_string(    expression, L"family_name"        , family_name                                       )
    M_get_right_keyword_nval(      expression, L"private"            , installed          , false                        )
    M_get_right_keyword_float64(   expression, L"em_size"            , em_size            , real_T                       )
    M_get_right_keyword_nval(      expression, L"bold"               , style_bold         , Gdiplus::FontStyleBold       )
    M_get_right_keyword_nval(      expression, L"italic"             , style_italic       , Gdiplus::FontStyleItalic     )
    M_get_right_keyword_nval(      expression, L"underline"          , style_ul           , Gdiplus::FontStyleUnderline  )
    M_get_right_keyword_nval(      expression, L"strikeout"          , style_so           , Gdiplus::FontStyleStrikeout  )
   

    M__(M_out(L"family_name = %S   em_size = %10.5f" ) % family_name % em_size;) 
    M__(M_out(L"keywords.count = %d") % expression.rparms.eval_kws.count(L"color");  )

    if (expression.rparms.eval_kws.count(L"color"     ) > 0)       
    {
        drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color); 
        M__(M_out(L"color present -- r/g/b = %d %d %d") % color.r % color.g % color.b;)
        set_color = true;         
    }

    style = style | style_bold | style_italic | style_ul | style_so;    // combined style


    // call set_font() to set font family, size, style, etc. 
    // -----------------------------------------------------

    int s_rc; 

    if (set_color == true)
       s_rc = set_font(*outimage_p, family_name, em_size, style, color, installed);    // set color this time
    else
       s_rc = set_font(*outimage_p, family_name, em_size, style,        installed);    // leave color unchanged

    if (s_rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_set_font_size() -- update em_size in current active font in outimage_S area (uses GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_set_font_size(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_set_font_size() called");) 
    
    M_get_outimage_n(L"drawverb_set_font_size")
    M_check_outimage_n(L"SET_FONT_SIZE")
  
    M_dv_some(M_out(L"=======> Doing   @SET_FONT_SIZE outimage_n:%d <=======") % n_outimage;)
    
    real_T         em_size       {16.0};                            
         
    M_get_right_keyword_float64(   expression, L"em_size"            , em_size            , real_T                       )

    M__(M_out(L"em_size = %10.5f" ) % em_size;) 


    // call set_font_size() to set font family, size, style, etc. 
    // -----------------------------------------------------

    auto s_rc = set_font_size(*outimage_p,  em_size);    

    if (s_rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_set_text_color() -- set font/text color (uses GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_set_text_color(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_set_text_color() called");) 
    
    M_get_outimage_n(L"drawverb_set_text_color")
    M_check_outimage_n(L"SET_TEXT_COLOR")

    M_dv_some(M_out(L"=======> Doing   @SET_TEXT_COLOR outimage_n:%d <=======") % n_outimage;)
    

    // process keyword parms -- color: and alpha:
    //-------------------------------------------
 
    rgb_S   color {    }; 
    alpha_T alpha {255U}; 

    M_get_right_keyword_int64(    expression, L"alpha"             , alpha         , alpha_T  )

    if (expression.rparms.eval_kws.count(L"color") > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);



    // call set_text_color() to set current font/text color  
    // ----------------------------------------------------

    auto s_rc = set_text_color(*outimage_p, color, alpha); 
    if (s_rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



 ///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_set_text_background_color() -- set font/text background color (uses GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_set_text_background_color(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_set_text_background_color() called");) 
    
    M_get_outimage_n(L"drawverb_set_text_background_color")
    M_check_outimage_n(L"SET_TEXT_BACKGROUND_COLOR")
    
    M_dv_some(M_out(L"=======> Doing   @SET_TEXT_BACKGROUND_COLOR outimage_n:%d <=======") % n_outimage;)
    

    // process keyword parms -- color: and alpha:
    //-------------------------------------------
 
    rgb_S   color {    }; 
    alpha_T alpha {255U}; 

    M_get_right_keyword_int64(    expression, L"alpha"             , alpha         , alpha_T  )

    if (expression.rparms.eval_kws.count(L"color") > 0)       drawverb_color_vlist_rgb(*(expression.rparms.eval_kws.find(L"color"   )->second.vlist_sp), color);
     

    // call set_text_background_color() to set current font/text color  
    // ---------------------------------------------------------------

    auto s_rc = set_text_background_color(*outimage_p, color, alpha); 
    if (s_rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_set_no_text_background() -- set to not write text background (use GDI+) 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_set_no_text_background(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_set_no_text_background() called");) 
    
    M_get_outimage_n(L"drawverb_set_no_text_background")
    M_check_outimage_n(L"SET_NO_TEXT_BACKGROUND")

    M_dv_some(M_out(L"=======> Doing   @SET_NO_TEXT_BACKGROUND outimage_n:%d <=======") % n_outimage;)
               
     

    // call set_no_text_background() to stop drawing text background boxes 
    // --------------------------------------------------------------------

    auto rc = set_no_text_background(*outimage_p);     

    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_draw_text() -- draw text string (use GDI+)
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_draw_text(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_draw_text() called");) 
    
    M_get_outimage_n(L"drawverb_draw_text")
    M_check_outimage_n(L"DRAW_TEXT")
  
    M_dv_some(M_out(L"=======> Doing   @DRAW_TEXT outimage_n:%d <=======") % n_outimage;)
    
    std::wstring   text          {}; 
    rc_T           row           {};  
    rc_T           col           {};  
    real_T         rotation      {0.0};
    real_T         x_scale       {1.0}; 
    real_T         y_scale       {1.0};
    real_T         x_shear       {0.0}; 
    real_T         y_shear       {0.0}; 
    real_T         extra_space   {0.0};
     
    M_get_right_keyword_string(    expression, L"text"               , text                                            )
    M_get_right_keyword_float64(   expression, L"rotation"           , rotation         , real_T                       )
    M_get_right_keyword_int64(     expression, L"row"                , row              , rc_T                         )
    M_get_right_keyword_int64(     expression, L"col"                , col              , rc_T                         )
    M_get_right_keyword_float64(   expression, L"width"              , x_scale          , real_T                       )
    M_get_right_keyword_float64(   expression, L"height"             , y_scale          , real_T                       )
    M_get_right_keyword_float64(   expression, L"x_shear"            , x_shear          , real_T                       )
    M_get_right_keyword_float64(   expression, L"y_shear"            , y_shear          , real_T                       )
    M_get_right_keyword_float64(   expression, L"extra_space"        , extra_space      , real_T                       )
  

    // call set_font() to set font family, size, style, etc. 
    // -----------------------------------------------------
     
    int d_rc {0};     


    if (extra_space == 0.0)
      d_rc = draw_text(*outimage_p, text, row, col, rotation, x_scale, y_scale, x_shear, y_shear);     
    else
      d_rc = draw_text(*outimage_p, text, row, col, rotation, x_scale, y_scale, x_shear, y_shear, extra_space); 

    if (d_rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


///_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_format_text() -- format text string within rectangle (use GDI+)
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_format_text(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_format_text() called");) 
    
    M_get_outimage_n(L"drawverb_format_text")
    M_check_outimage_n(L"FORMAT_TEXT")

    M_dv_some(M_out(L"=======> Doing   @FORMAT_TEXT outimage_n:%d <=======") % n_outimage;)

    format_text_parm_S fparm {  }; 
     
    M_get_right_keyword_string(    expression, L"text"               , fparm.str                                             )
    M_get_right_keyword_int64(     expression, L"top"                , fparm.top              , rc_T                         )
    M_get_right_keyword_int64(     expression, L"left"               , fparm.left             , rc_T                         )
    M_get_right_keyword_int64(     expression, L"width"              , fparm.width            , rc_T                         )
    M_get_right_keyword_int64(     expression, L"height"             , fparm.height           , rc_T                         )
  

    // call format_text() to format caller's text in rectangle 
    // -------------------------------------------------------    

    auto rc = format_text(*outimage_p, fparm);  

    if (rc != 0) 
        return 1;  
    else      
        return 0;  
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_clear_graphics() -- reset GSI+ graphics to cleared-out color  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_clear_graphics(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_clear_graphics() called");) 
    
    M_get_outimage_n(L"drawverb_clear_graphics")
    M_check_outimage_n(L"CLEAR_GRAPHICS")

    M_dv_few(M_out(L"=======> Doing   @CLEAR_GRAPHICS outimage_n:%d <=======") % n_outimage;)
    

    // call clear_graphics() to copy GDI+ graphics data into outimage buffer
    // --------------------------------------------------------------------- 

    auto rc = outimage_p->canvas.clear();
    
    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_put_graphics() -- copy GDI+ graphics data into outimage buffer  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_put_graphics(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_put_graphics() called");) 
    
    M_get_outimage_n(L"drawverb_put_graphics")
    M_check_outimage_n(L"PUT_GRAPHICS")

    M_dv_few(M_out(L"=======> Doing   @PUT_GRAPHICS outimage_n:%d <=======") % n_outimage;)
      
  
    // call put_graphics() to copy GDI+ graphics data into outimage buffer
    // ------------------------------------------------------------------- 

    auto rc = put_graphics(*outimage_p);
    
    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_get_graphics() -- copy outimage buffer data into GDI+ graphics DIBSections  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_get_graphics(const e_expression_S& expression) try
{
    M__(M_out(L"drawverb_get_graphics() called");) 
    
    M_get_outimage_n(L"drawverb_get_graphics")
    M_check_outimage_n(L"GET_GRAPHICS")
  
    M_dv_few(M_out(L"=======> Doing   @GET_GRAPHICS outimage_n:%d <=======") % n_outimage;)
      
  
    // call get_graphics() to copy outimage buffer data into GDI+ graphics DIBSections
    // ------------------------------------------------------------------------------- 

    auto rc = get_graphics(*outimage_p);
    
    if (rc != 0) 
        return 1;  
    else      
        return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///          O U T I M A G E 
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_create_outimage() -- do CREATE_OUTIMAGE    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_create_outimage(const e_expression_S& expression) try
{
    int         rc = 0;
           
    M_get_outimage_n(L"drawverb_create_outimage")       // declares outimage_p and n_outimage, used below


    // Get outimage create parms from keyword options
    // ----------------------------------------------

    outimage_create_parm_S iparm {}; 

    M_get_right_keyword_int64(expression, L"display" , iparm.display   , bool   )


    // Get dimensions from keyword options  (required -- should be present)
    // -----------------------------------
   
    M_get_right_keyword_int64(expression, L"rows"    , iparm.rows      , rc_T)
    M_get_right_keyword_int64(expression, L"cols"    , iparm.cols      , rc_T)


    // Destroy outimage first, if already active
    // -----------------------------------------

    if (outimage_p != nullptr)
    {
        M_dv_few(M_out(L"=======> @CREATE_OUTIMAGE outimage_n:%d -- destroying old outimage first <======") % n_outimage;)

        rc = drawverb_destroy_outimage(expression); 
        outimage_p = nullptr;                    // clear out our own local outimage pointer

        if (rc != 0) return rc; 
    }


    // create outimage and mark as active
    // ----------------------------------

 //   if (parm.verb_display >= 1)
 //       M_out(L"=======> Doing   @CREATE_OUTIMAGE outimage_n:%d   <=======") % n_outimage;
    
    outimage_p = alloc_outimage(n_outimage);                  // make sure outimage(n_outimage) is allocated
    create_outimage(*outimage_p, iparm);    
    return 0; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_extract_outimage() 
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


int32_t drawverb_extract_outimage(const e_expression_S& expression, value_S& value) try
{
    M__(M_out(L"drawverb_extract_outimage() called");) 
   

    M_get_outimage_n(L"drawverb_extract_outimage")
    M_check_outimage_n(L"EXTRACT_OUTIMAGE")

    M_dv_some(M_out(L"=======> Doing   @EXTRACT_OUTIMAGE outimage_n:%d <=======") % n_outimage;)


    // return requested outimage info  (maximum of one of these keywords can be present)
    // ------------------------------ 

    if (expression.rparms.eval_kws.count(L"cols"                            ) > 0) value = set_type_value( (int64_t)(outimage_p->cols                     ) );
    if (expression.rparms.eval_kws.count(L"rows"                            ) > 0) value = set_type_value( (int64_t)(outimage_p->rows                     ) );
    if (expression.rparms.eval_kws.count(L"pixels"                          ) > 0) value = set_type_value( (int64_t)(outimage_p->pixels                   ) );
    if (expression.rparms.eval_kws.count(L"bytes"                           ) > 0) value = set_type_value( (int64_t)(outimage_p->bytes                    ) );
    
    if (expression.rparms.eval_kws.count(L"installed_font_ct"               ) > 0)
    {  
        int32_t installed_font_ct {0};  
        
        auto g_rc = get_font_count(*outimage_p, true, installed_font_ct);
        
        if (g_rc == 0)
        {
            value = set_type_value( (int64_t)(installed_font_ct) );
            return 0; 
        }
        else
        {
            return -1;
        }
    }

    if (expression.rparms.eval_kws.count(L"private_font_ct"                 ) > 0) 
    {  
        int32_t private_font_ct {0};  
        
        auto g_rc = get_font_count(*outimage_p, false, private_font_ct);
        
        if (g_rc == 0)
        {
            value = set_type_value( (int64_t)(private_font_ct) );
            return 0; 
        }
        else
        {
            return -1;
        }
    }   
    
    if (expression.rparms.eval_kws.count(L"installed_font_name"               ) > 0)
    {  
        int32_t font_ix {0};
        M_get_right_keyword_int64(   expression, L"installed_font_name",font_ix, int32_t)
        
        std::wstring family_name {}; 
        auto g_rc = get_font_name(*outimage_p, true, font_ix, family_name);
        
        if (g_rc == 0)
        {
            value = set_type_value(family_name);
            return 0; 
        }
        else
        {
            return -1;
        }
    }

    if (expression.rparms.eval_kws.count(L"private_font_name"               ) > 0)
    {  
        int32_t font_ix {0};
        M_get_right_keyword_int64(   expression, L"private_font_name",font_ix, int32_t)
        
        std::wstring family_name {}; 
        auto g_rc = get_font_name(*outimage_p, false, font_ix, family_name);
        
        if (g_rc == 0)
        {
            value = set_type_value(family_name);
            return 0; 
        }
        else
        {
            return -1;
        }
    }


    // get here when no extracted field was specified

    return 0; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_write_outimage() -- do WRITE_OUTIMAGE    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_write_outimage(const e_expression_S& expression) try
{
    M_get_outimage_n(   L"drawverb_write_outimage")
    M_check_outimage_n( L"WRITE_OUTIMAGE")
  

    // get parms for output file, etc.
    // -------------------------------

    write_outimage_parm_S wparm {};              // with hard-coded defaults only 
     
    M_get_right_keyword_int64( expression, L"display"    , wparm.display    , bool )
    M_get_right_keyword_nval(  expression, L"bmp_24"     , wparm.tiff_24    , false)     // if bmp_24 specified, override default of tiff_24 = true 
    M_get_right_keyword_string(expression, L"folder"     , wparm.folder            )
    M_get_right_keyword_string(expression, L"filename"   , wparm.filename          ) 

    if (expression.rparms.eval_kws.count(L"tiff_24") > 0)
    {    
        vlist_S vlist = *(expression.rparms.eval_kws.find(L"tiff_24")->second.vlist_sp);

        M_get_nest_keyword_nval(  vlist, L"orientation_bottom_left"     , wparm.tiff24_parm.orientation      , tiff_N::C_or_bottom_left  ) 
        M_get_nest_keyword_nval(  vlist, L"orientation_bottom_right"    , wparm.tiff24_parm.orientation      , tiff_N::C_or_bottom_right ) 
        M_get_nest_keyword_nval(  vlist, L"orientation_left_bottom"     , wparm.tiff24_parm.orientation      , tiff_N::C_or_left_bottom  ) 
        M_get_nest_keyword_nval(  vlist, L"orientation_left_top"        , wparm.tiff24_parm.orientation      , tiff_N::C_or_left_top     ) 
        M_get_nest_keyword_nval(  vlist, L"orientation_right_bottom"    , wparm.tiff24_parm.orientation      , tiff_N::C_or_right_bottom ) 
        M_get_nest_keyword_nval(  vlist, L"orientation_right_top"       , wparm.tiff24_parm.orientation      , tiff_N::C_or_right_top    )  
        M_get_nest_keyword_nval(  vlist, L"orientation_rot_90_ccw"      , wparm.tiff24_parm.orientation      , tiff_N::C_or_left_bottom  ) 
        M_get_nest_keyword_nval(  vlist, L"orientation_top_left"        , wparm.tiff24_parm.orientation      , tiff_N::C_or_top_left     ) 
        M_get_nest_keyword_nval(  vlist, L"orientation_top_right"       , wparm.tiff24_parm.orientation      , tiff_N::C_or_top_right    ) 
        M_get_nest_keyword_nval(  vlist, L"resolution_unit_cm"          , wparm.tiff24_parm.resolution_unit  , tiff_N::C_ru_centimeter   ) 
        M_get_nest_keyword_nval(  vlist, L"resolution_unit_inch"        , wparm.tiff24_parm.resolution_unit  , tiff_N::C_ru_inch         ) 

        M_get_nest_keyword_int64( vlist, L"rows_per_strip"              , wparm.tiff24_parm.rows_per_strip   , int32_t                   )
        M_get_nest_keyword_int64( vlist, L"x_resolution"                , wparm.tiff24_parm.x_resolution     , int32_t                   )
        M_get_nest_keyword_int64( vlist, L"y_resolution"                , wparm.tiff24_parm.y_resolution     , int32_t                   )
    }


    //  Write out outimage
    //  ------------------

    M_dv_few(M_out(L"=======> Doing   @WRITE_OUTIMAGE outimage_n:%d   <=======") % n_outimage;)

	   auto rc = write_outimage(*outimage_p, wparm);    

    if (rc != 0) 
        return 1;   
    else
        return 0; 
}
M_endf

  


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   drawverb_destroy_outimage() -- do DESTROY_OUTIMAGE    processing
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t drawverb_destroy_outimage(const e_expression_S& expression) try
{
    M_get_outimage_n(   L"drawverb_destroy_outimage" )     // declares outimage_p and n_outimage, used below
    M_check_outimage_n( L"DESTROY_OUTIMAGE"          )


    // destroy outimage and mark as inactive
    // -------------------------------------
   
    M_dv_few(M_out(L"=======> Doing   @DESTROY_OUTIMAGE outimage_n:%d   <=======") % n_outimage;)
      
    auto rc = destroy_outimage(*outimage_p);   
    free_outimage(n_outimage);
    outimage_p = nullptr; 
    return rc;
}
M_endf





//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""