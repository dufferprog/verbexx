// h_draw_verb.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            =============
////            h_draw_verb.h -- functions in draw_verb.cpp and draw_addverb.cpp
////            =============
////     
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_ex_interface.h"                // verb definitions need ex types
#include "h_draw_types.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_get_outimage_n -- MACRO to get value from outimage_n keyword
//
//    - assume outimage_n keyword exists with int64_t 
//
//    - MACRO parm is name of function for error message (not used)
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////


#define M_get_outimage_n(mt)                                                                \
int64_t n_outimage {1};                                                                     \
                                                                                            \
M_get_right_keyword_int64(expression, L"outimage_n", n_outimage, int64_t)                   \
outimage_S *outimage_p { find_outimage(n_outimage) };                                                    

 


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    M_check_outimage_n -- MACRO to make sure outimage is allocated in repository
//
//    - assume n_outimage and outimage_p are already set up for selected outimage 
//
//    - MACRO parm is name of user verb for error message
//
//      note: uses n_outimage defined in M_get_outimage 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define M_check_outimage_n(mt)                                                                                    \
if (outimage_p == nullptr)                                                                                        \
{                                                                                                                 \
     M_out_emsg(L"=======> @%s failed -- outimage %d is not allocated in repository <======") % mt % n_outimage;  \
     return 1;                                                                                                    \
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// external function -- in draw_addverb.cpp 
// ----------------------------------------

void add_draw_verbs(const std::wstring&);


//   external verb-actioning functions -- in draw_verb.cpp
//   -----------------------------------------------------
   
int32_t drawverb_create_outimage(             const e_expression_S&               );        
int32_t drawverb_write_outimage(              const e_expression_S&               ); 
int32_t drawverb_destroy_outimage(            const e_expression_S&               );                                               
                                
int32_t drawverb_draw_line(                   const e_expression_S&               );
int32_t drawverb_draw_circle(                 const e_expression_S&               );
int32_t drawverb_draw_ring(                   const e_expression_S&               );
int32_t drawverb_draw_box(                    const e_expression_S&               );
int32_t drawverb_put_box(                     const e_expression_S&               );
int32_t drawverb_draw_frame(                  const e_expression_S&               );
int32_t drawverb_put_frame(                   const e_expression_S&               );
int32_t drawverb_draw_image(                  const e_expression_S&               );
                                     
int32_t drawverb_display_installed_fonts(     const e_expression_S&               );
int32_t drawverb_display_private_fonts(       const e_expression_S&               );  
int32_t drawverb_add_private_font(            const e_expression_S&               );  
int32_t drawverb_add_private_fonts(           const e_expression_S&               );  
int32_t drawverb_remove_private_fonts(        const e_expression_S&               );  
int32_t drawverb_set_font(                    const e_expression_S&               );
int32_t drawverb_set_font_size(               const e_expression_S&               );
int32_t drawverb_set_text_color(              const e_expression_S&               );
int32_t drawverb_set_text_background_color(   const e_expression_S&               );
int32_t drawverb_set_no_text_background(      const e_expression_S&               );
int32_t drawverb_draw_text(                   const e_expression_S&               );
int32_t drawverb_format_text(                 const e_expression_S&               );
        
int32_t drawverb_clear_graphics(              const e_expression_S&               ); 
int32_t drawverb_put_graphics(                const e_expression_S&               ); 
int32_t drawverb_get_graphics(                const e_expression_S&               );                                                
        
int32_t drawverb_extract_outimage(            const e_expression_S&, value_S&     );
int32_t drawverb_create_outimage(             const e_expression_S&               );
int32_t drawverb_write_outimage(              const e_expression_S&               ); 
int32_t drawverb_destroy_outimage(            const e_expression_S&               );


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////