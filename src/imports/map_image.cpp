// map_image.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     map_image.cpp -- outimage-drawing functions that combine the mapdata_S and outimage_S structure 
////     =============
//// 
//// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "h__include.h"
#pragma hdrstop("../pch/pch_std.pch")

#define M_IN_MAP_DLL

#include "h__common.h"

#include "h_draw_outimage.h" 

#include "h_map_types.h"
#include "h_map_global.h" 
#include "h_map_parms.h" 
#include "h_map_data.h"
#include "h_map_image.h"
#include "h_map_thd.h" 
#include "h_map_ramp.h" 
  


//  Internal functions
//  ------------------

static void put_map_thd(   outimage_S& , const mapdata_S& , const ramp_S& , thp_S&,   const put_map_parm_S&,       const mapparm_S&);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////
//////
//////     ===========================================
//////     Latitude/Longitude based graphics functions
//////     ===========================================
//////
//////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  draw_ll_text -- draw string, 1 wchar at a time -- allows for extra space, curved path, etc.
//  ============    ---------------------------------------------------------------------------
//
//  - uses tf.x_scale, tf.y_scale, tf.x_shear, tf.y_shear, tf.extra_space, as input
//  - tf.x, tf.y, and tf.rotation are filled in, as output (as well as the usual output fields) 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------
// version with passed-in text_format_S and font_S
// -----------------------------------------------

int draw_ll_text(outimage_S& outimage, const mapdata_S& mapdata, const font_S& fs, text_format_S& tf, const std::wstring& str, real_T lat1, real_T lon1, real_T rhumb, const mapparm_S& parm) try
{
    int rc = 0; 
    real_T curr_lat = lat1;  
    real_T curr_lon = lon1;  


    // initialize current xform fields from initial xform fields tf.x, y, and rotation will be filled in by draw_ll_char() based on lat/lon/rhumb direction

    tf.curr_x_scale      = tf.x_scale;
    tf.curr_y_scale      = tf.y_scale; 
    tf.curr_x_shear      = tf.x_shear;
    tf.curr_y_shear      = tf.y_shear; 
    tf.curr_extra_space  = tf.extra_space;    // extra space is supported by draw_chr()


    // main loop to process each char -- call exit routine before drawing, if one is present
    // -------------------------------------------------------------------------------------

    for (int i = 0; i < str.size(); i++)
    {
        // call user-provided exit function to possibly update items in tf structure 

        if (tf.exit_p != NULL)
        {
            rc = (*tf.exit_p)(&(str.c_str()[i]), i, fs, tf);
            if (rc != 0)
                return rc;         
        }


        // draw this one character from string (with potential extra space, etc.) -- new_x and new_y get updated

        rc = draw_ll_char(outimage, mapdata, fs, tf, &(str.c_str()[i]), curr_lat, curr_lon, rhumb, parm);
        if (rc != 0)
           return rc; 


        // update curr_lat/lon based on new_x and new_y, for next pass of loop (ignore out-of-bounds errors)

        rc_T new_c = M_map_window_c(tf.new_x, outimage, mapdata);
        rc_T new_r = M_map_window_r(tf.new_y, outimage, mapdata);  

        (void)(*(mapdata.get_lat_lon_p))(mapdata, curr_lat, curr_lon, new_r, new_c, false);
    }

    return rc; 
}
M_endf


// -----------------------------------------------------------
// version with internal text_format_S -- used gs.font_S, etc. (which must be set up already)
// -----------------------------------------------------------

int draw_ll_text(outimage_S& outimage, const mapdata_S& mapdata, const std::wstring& str, real_T lat1, real_T lon1, real_T rhumb, const mapparm_S& parm) try
{
    text_format_S tf { };                                                                               // local text_format_S area -- default initialization 
    copy_text_format(tf, *(outimage.canvas.m_tf_p));                                                          // copy settings from global text_format_S to local text_format_s
    set_text_format(tf);                                                                                // x,y  and scale factors, etc. to defaults here -- will get updated to starting values later

    return draw_ll_text(outimage, mapdata, outimage.canvas.m_font, tf, str, lat1, lon1, rhumb, parm);         // call main draw_ll_text() to output the string    
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  draw_llfix_text -- draw string, 1 wchar at a time -- allows for extra space, etc.
//  ===============    --------------------------------------------------------------
//
//  (input rotation is absolute, not a rhumb direction -- text does not follow graticle curves, etc.)
//
//  - uses tf.x_scale, tf.y_scale, tf.x_shear, tf.y_shear, tf.extra_space, as input
//  - tf.x, tf.y, and tf.rotation are filled in, as output (as well as the usual output fields) 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------
// version with passed-in text_format_S and font_S
// -----------------------------------------------

int draw_llfix_text(outimage_S& outimage, const mapdata_S& mapdata, const font_S& fs, text_format_S& tf, const std::wstring& str, real_T lat1, real_T lon1, real_T rotation, const mapparm_S& parm) try
{
    int rc = 0; 
    real_T curr_lat = lat1;  
    real_T curr_lon = lon1;  


    // initialize current xform fields from initial xform fields tf.x, y

    tf.curr_x_scale      = tf.x_scale;
    tf.curr_y_scale      = tf.y_scale; 
    tf.curr_x_shear      = tf.x_shear;
    tf.curr_y_shear      = tf.y_shear; 
    tf.curr_extra_space  = tf.extra_space;    // extra space is supported by draw_chr()


    // main loop to process each char -- call exit routine before drawing, if one is present
    // -------------------------------------------------------------------------------------

    for (int i = 0; i < str.size(); i++)
    {
        // call user-provided exit function to possibly update items in tf structure 

        if (tf.exit_p != NULL)
        {
            rc = (*tf.exit_p)(&(str.c_str()[i]), i, fs, tf);
            if (rc != 0)
                return rc;         
        }


        // draw this one character from string (with potential extra space, etc.) -- new_x and new_y get updated

        rc = draw_llfix_char(outimage, mapdata, fs, tf, &(str.c_str()[i]), curr_lat, curr_lon, rotation, parm);
        if (rc != 0)
           return rc; 


        // update curr_lat/lon based on new_x and new_y, for next pass of loop (ignore out-of-bounds errors)

        rc_T new_c = M_map_window_c(tf.new_x, outimage, mapdata);
        rc_T new_r = M_map_window_r(tf.new_y, outimage, mapdata);  

        (void)(*(mapdata.get_lat_lon_p))(mapdata, curr_lat, curr_lon, new_r, new_c, false);
    }

    return rc; 
}
M_endf


// -----------------------------------------------------------
// version with internal text_format_S -- used gs.font_S, etc. (which must be set up already)
// -----------------------------------------------------------

int draw_llfix_text(outimage_S& outimage, const mapdata_S& mapdata, const std::wstring& str, real_T lat1, real_T lon1, real_T rotation, const mapparm_S& parm) try
{                                                                                                             
    text_format_S tf { };                                                                                          // local text_format_S area -- default initialization 
    copy_text_format(tf, *(outimage.canvas.m_tf_p));                                                               // copy settings from global text_format_S to local text_format_s
    set_text_format(tf);                                                                                           // x,y  and scale factors, etc. to defaults here -- will get updated to starting values later

    return draw_llfix_text(outimage, mapdata, outimage.canvas.m_font, tf, str, lat1, lon1, rotation, parm);        // call main draw_llfix_text() to output the string    
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  draw_ll_char -- draw 1 wchar at lat/lon (input rhumb direction is adjusted to follow graticle)
//  ============    -----------------------
//
//  - uses tf.x_scale, tf.y_scale, tf.x_shear, tf.y_shear, tf.extra_space, as input
//  - tf.x, tf.y, and tf.rotation are filled in, as output (as well as the usual output fields) 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------
// version with passed-in text_format_S and font_S
// -----------------------------------------------

int draw_ll_char(outimage_S& outimage, const mapdata_S& mapdata, const font_S& fs, text_format_S& tf, const WCHAR *p, real_T lat1, real_T lon1, real_T rhumb, const mapparm_S& parm) try
{
    real_T rowf1;
    real_T colf1;
    real_T rowf2;
    real_T colf2;
    int rc; 
    

    // get X, Y pixel coordinates for caller's starting latitude/longitude (error is row/col can't be determined

    auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf1, colf1, lat1, lon1, false);

    if ( (rc_rc != M_ROW_COL_RC_OK) && (rc_rc != M_ROW_COL_RC_OUTMAP) )
        return -1; 

    tf.x        = M_map_window_x(colf1, outimage, mapdata);
    tf.y        = M_map_window_y(rowf1, outimage, mapdata);
    tf.rotation = (Gdiplus::REAL)rhumb;                                // approximate -- will be updated later 
    
    
    // initialize current formatting fields from initial formatting fields
     
    tf.curr_x           = tf.x; 
    tf.curr_y           = tf.y;
    tf.curr_rotation    = tf.rotation;  
        

    // call measure_str() to get length or string to be drawn -- temporarily set very small non-0 rotation -- seems to affect the output width
    // width is in tf.output_width field
      
    rc = outimage.canvas.measure_str(p, (size_t)1, fs, tf, true);
    M_checkrc_ret(rc, L"draw_ll_char(): measure_str()")
    M__(M_out(L"output_width=%.6f    extra_width=%.6f") % tf.output_width % tf.extra_width;)


    // get gdiplus rotation for the character (only) at this lat/lon, corresponding to input rhumb angle  (this is the negative of the actual line rotation)
    
    tf.rotation                  = -(Gdiplus::REAL)ll_line_rotation(mapdata, lat1, lon1, rhumb, tf.output_width                  , parm);  // don't include extra space when computing rotation for the character itself
    Gdiplus::REAL extra_rotation = -(Gdiplus::REAL)ll_line_rotation(mapdata, lat1, lon1, rhumb, tf.output_width + tf.extra_width , parm);  // rotation used to comp new_x and new_y  
    tf.curr_rotation             = tf.rotation;
         

    // draw the char (and/or) background

    rc = outimage.canvas.draw_chr(p, fs, tf);
    M_checkrc_ret(rc, L"draw_ll_char(): canvas_C::draw_chr()")   


    // recompute new_x and new_y using the rotation that takes extra_space into account

    text_new_xy(tf, extra_rotation);  

    M__(M_out("new_x=%.6f (%.6f)    new_y=%.6f (%.6f)    output_width=%.6f") % tf.new_x % colf2 % tf.new_y % rowf2 % tf.output_width;)
      
    return 0; 
}
M_endf


// -----------------------------------------------------------
// version with internal text_format_S -- used gs.font_S, etc. (which must be set up already)
// -----------------------------------------------------------

int draw_ll_char(outimage_S& outimage, const mapdata_S& mapdata, const WCHAR *p, real_T lat1, real_T lon1, real_T rhumb, const mapparm_S& parm) try
{
    text_format_S tf { };                                                                                          // local text_format_S area -- default initialization 
    copy_text_format(tf, *(outimage.canvas.m_tf_p));                                                               // copy settings from global text_format_S to local text_format_s
    set_text_format(tf);                                                                                           // x,y  and scale factors, etc. to defaults here -- will get updated to starting values later

    return draw_ll_char(outimage, mapdata, outimage.canvas.m_font, tf, p, lat1, lon1, rhumb, parm);                // call main draw_ll_char() to output the character    
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  draw_llfix_char -- draw 1 wchar at lat/lon (input rotation is not adjusted to follow graticle)
//  ===============    -----------------------
//
//  - uses tf.x_scale, tf.y_scale, tf.x_shear, tf.y_shear, tf.extra_space, as input
//  - tf.x, tf.y, and tf.rotation are filled in, as output (as well as the usual output fields) 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------
// version with passed-in text_format_S and font_S
// -----------------------------------------------

int draw_llfix_char(outimage_S& outimage, const mapdata_S& mapdata, const font_S& fs, text_format_S& tf, const WCHAR *p, real_T lat1, real_T lon1, real_T rotation, const mapparm_S& parm) try
{
    real_T rowf1;
    real_T colf1;
    real_T rowf2;
    real_T colf2;
    int rc; 
    

    // get X, Y pixel coordinates for caller's starting latitude/longitude

    auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf1, colf1, lat1, lon1, false);

    if ( (rc_rc != M_ROW_COL_RC_OK) && (rc_rc != M_ROW_COL_RC_OUTMAP) )
        return -1; 


    tf.x        = M_map_window_x(colf1, outimage, mapdata);
    tf.y        = M_map_window_y(rowf1, outimage, mapdata);
    tf.rotation = (Gdiplus::REAL)rotation;         // will not be updated later 
    
    
    // initialize current formatting fields from initial formatting fields
     
    tf.curr_x           = tf.x; 
    tf.curr_y           = tf.y;
    tf.curr_rotation    = tf.rotation;  
        

    // draw the char (and/or) background

    rc = outimage.canvas.draw_chr(p, fs, tf);
    M_checkrc_ret(rc, L"draw_ll_char(): canvas_C::draw_chr()")   
         
    return 0; 
}
M_endf


// -----------------------------------------------------------
// version with internal text_format_S -- used gs.font_S, etc. (which must be set up already)
// -----------------------------------------------------------

int draw_llfix_char(outimage_S& outimage, const mapdata_S& mapdata, const WCHAR *p, real_T lat1, real_T lon1, real_T rotation, const mapparm_S& parm) try
{
    text_format_S tf { };                                                                                          // local text_format_S area -- default initialization 
    copy_text_format(tf, *(outimage.canvas.m_tf_p));                                                               // copy settings from global text_format_S to local text_format_s
    set_text_format(tf);                                                                                           // x,y  and scale factors, etc. to defaults here -- will get updated to starting values later

    return draw_llfix_char(outimage, mapdata, outimage.canvas.m_font, tf, p, lat1, lon1, rotation, parm);          // call main draw_llfix_char() to output the character    
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  draw_ll_str -- straight (but tilted) string at lat/lon
//  ===========    ---------------------------------------
//
//  - uses tf.x_scale, tf.y_scale, tf.x_shear, tf.y_shear, tf.extra_space, as input
//  - tf.x, tf.y, and tf.rotation are filled in, as output (as well as the usual output fields) 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------
// version with passed-in text_format_S and font_S
// -----------------------------------------------

int draw_ll_str(outimage_S& outimage, const mapdata_S& mapdata, const font_S& fs, text_format_S& tf, const std::wstring& str, real_T lat1, real_T lon1, real_T rhumb, const mapparm_S& parm) try
{
    real_T rowf1;
    real_T colf1;
    real_T rowf2;
    real_T colf2;
    int rc; 
   

    // return now (error), if no input characters

    size_t sz = str.size(); 

    if (sz == 0) 
        return -1;  
 

    // get X, Y pixel coordinates for starting latitude/longitude

    auto rc_rc =  (*(mapdata.get_row_col_p))(mapdata, rowf1, colf1, lat1, lon1, false);

    if ( (rc_rc != M_ROW_COL_RC_OK) && (rc_rc != M_ROW_COL_RC_OUTMAP) )
        return -1; 


    tf.x        = M_map_window_x(colf1, outimage, mapdata);
    tf.y        = M_map_window_y(rowf1, outimage, mapdata);
    tf.rotation = (Gdiplus::REAL)rhumb;                            // approximate -- will be updated later 
    
    
    // initialize current xform fields from initial xform fields
     
    tf.curr_x         = tf.x; 
    tf.curr_y         = tf.y;
    tf.curr_rotation  = tf.rotation;    
    tf.curr_x_scale   = tf.x_scale;
    tf.curr_y_scale   = tf.y_scale; 
    tf.curr_x_shear   = tf.x_shear;
    tf.curr_y_shear   = tf.y_shear; 
    

    // call measure_str() to get length or string to be drawn -- temporarily set very small non-0 rotation -- seems to affect the output width
    // width is in tf.output_width field
      
    rc = outimage.canvas.measure_str(str.c_str(), str.size(), fs, tf, true);
    M_checkrc_ret(rc, L"draw_ll_str(): canvas_C::measure_str()")
    M__(M_out(L"output_width=%.6f") % tf.output_width;)


    // get rotation at this lat/lon, corresponding to input rhumb angle 
    
    tf.rotation      = -(Gdiplus::REAL)ll_line_rotation(mapdata, lat1, lon1, rhumb, tf.output_width, parm);
    tf.curr_rotation = tf.rotation;


    // draw the string (and/or) background

    M__(M_out(L"draw_ll_str(2): tf.draw_background = %d") % ((int32_t)(outimage.canvas.m_tf_p->draw_background));)
    M__(M_out(L"draw_ll_str(2): tf.text_brush_p = %p   tf.background_brush_p = %p") % outimage.canvas.m_tf_p->text_brush_p % outimage.canvas.m_tf_p->background_brush_p;) 

    rc = outimage.canvas.draw_str(str, fs, tf);
    M_checkrc_ret(rc, L"draw_ll_str(): canvas_C::draw_str()")   

    M__(M_out(L"new_x=%.6f (%.6f)    new_y=%.6f (%.6f)    output_width=%.6f") % tf.new_x % colf2 % tf.new_y % rowf2 % tf.output_width;)
      
    return 0; 
}
M_endf


// -----------------------------------------------------------
// version with internal text_format_S -- used gs.font_S, etc. (which must be set up already)
// -----------------------------------------------------------

int draw_ll_str(outimage_S& outimage, const mapdata_S& mapdata, const std::wstring& str, real_T lat1, real_T lon1, real_T rhumb, const mapparm_S& parm) try
{
    text_format_S tf { };                                                                                          // local text_format_S area -- default initialization 
    copy_text_format(tf, *(outimage.canvas.m_tf_p));                                                               // copy settings from global text_format_S to local text_format_s
    set_text_format(tf);                                                                                           // x,y  and scale factors, etc. to defaults here -- will get updated to starting values later

    M__(M_out(L"draw_ll_str(1): tf.draw_background = %d") % ((int32_t)(outimage.canvas.m_tf_p->draw_background));)
    M__(M_out(L"draw_ll_str(1): tf.text_brush_p = %p   tf.background_brush_p = %p") % outimage.canvas.m_tf_p->text_brush_p % outimage.canvas.m_tf_p->background_brush_p;) 

    return draw_ll_str(outimage, mapdata, outimage.canvas.m_font, tf, str, lat1, lon1, rhumb, parm);               // call main draw_ll_str() to output the string    
}
M_endf





/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  draw_llfix_str -- straight string at lat/lon -- caller passes in fixed angle -- ignores graticle slope 
//  ==============    --------------------------
//
//  - uses tf.x_scale, tf.y_scale, tf.x_shear, tf.y_shear, tf.extra_space, as input
//  - tf.x, tf.y, and tf.rotation are filled in, as output (as well as the usual output fields) 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------
// version with passed-in text_format_S and font_S
// -----------------------------------------------

int draw_llfix_str(outimage_S& outimage, const mapdata_S& mapdata, const font_S& fs, text_format_S& tf, const std::wstring& str, real_T lat1, real_T lon1, real_T rotation, const mapparm_S& parm) try
{
    real_T rowf1;
    real_T colf1;
    real_T rowf2;
    real_T colf2;
    int rc; 
   

    // return now (error), if no input characters

    size_t sz = str.size(); 

    if (sz == 0) 
        return -1;  


    // get X, Y pixel coordinates for starting latitude/longitude

    auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf1, colf1, lat1, lon1, false);

    if ( (rc_rc != M_ROW_COL_RC_OK) && (rc_rc != M_ROW_COL_RC_OUTMAP) )
        return -1; 


    tf.x        = M_map_window_x(colf1, outimage, mapdata);
    tf.y        = M_map_window_y(rowf1, outimage, mapdata);
    tf.rotation = (Gdiplus::REAL)rotation;         // fixed rotation -- does not get updatedr 
    
    
    // initialize current xform fields from initial xform fields
     
    tf.curr_x         = tf.x; 
    tf.curr_y         = tf.y;
    tf.curr_rotation  = tf.rotation;    
    tf.curr_x_scale   = tf.x_scale;
    tf.curr_y_scale   = tf.y_scale; 
    tf.curr_x_shear   = tf.x_shear;
    tf.curr_y_shear   = tf.y_shear; 


    // draw the string (and/or) background

    rc = outimage.canvas.draw_str(str, fs, tf);
    M_checkrc_ret(rc, L"draw_ll_str(): canvas-C::draw_str()")   
      
    return 0; 
}
M_endf


// -----------------------------------------------------------
// version with internal text_format_S -- used gs.font_S, etc. (which must be set up already)
// -----------------------------------------------------------

int draw_llfix_str(outimage_S& outimage, const mapdata_S& mapdata, const std::wstring& str, real_T lat1, real_T lon1, real_T rotation, const mapparm_S& parm) try
{
    text_format_S tf { };                                                                                          // local text_format_S area -- default initialization 
    copy_text_format(tf, *(outimage.canvas.m_tf_p));                                                               // copy settings from global text_format_S to local text_format_s
    set_text_format(tf);                                                                                           // x,y  and scale factors, etc. to defaults here -- will get updated to starting values later

    return draw_llfix_str(outimage, mapdata, outimage.canvas.m_font, tf, str, lat1, lon1, rotation, parm);         // call main draw_llfix_str() to output the string    
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  draw_ll_line -- draw line at lat/lon -- line will be curved to follow graticule, etc.  
//  ============    --------------------
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// form with caller-supplied pen
// -----------------------------

int draw_ll_line(outimage_S& outimage, const mapdata_S& mapdata, const Gdiplus::Pen *pen_p, real_T lat1, real_T lon1, real_T lat2, real_T lon2, real_T step_deg, const mapparm_S& parm) try
{
    Gdiplus::PointF pts[1001]; 
    int             npts; 
    real_T          rowf;
    real_T          colf;
    

    // compute number of points for draw_lines -- max = 1000
    
    real_T          len = hypot(lat2 - lat1, lon2 - lon1);   // very approximate formula 
           
    if (step_deg >= 0.01)  
       npts = M_lim(2, len/step_deg, 1000); 
    else
       npts = 1000;

     M__(M_out(L"len = %.6f     step_deg=%.6f       npts = %d") % len % step_deg % npts;) 


     // loop to populate pts array with up to 1000 intermediate points

     for (int i=0; i <= npts; i++)
     {
         real_T fract = (real_T)i / (real_T)(npts - 1);
     
         real_T lat = lat1 + fract * (lat2 - lat1);
         real_T lon = lon1 + fract * (lon2 - lon1);
           
         auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, true); 

         if ( (rc_rc == M_ROW_COL_RC_OK) || (rc_rc == M_ROW_COL_RC_OUTMAP) )    // only add valid returned row/col to list of points
         { 
              pts[i].X  = M_map_window_x(colf, outimage, mapdata);
              pts[i].Y  = M_map_window_y(rowf, outimage, mapdata);
       
              M__(M_out("i = %d    lat = %.6f     lon = %.6f    row = %.6f/%.6f    col = %.6f/%.6f") % i % lat % lon % rowf  % pts[i].Y % colf % pts[i].Y;)
         }
     }

     // ??????? need to interrupt line drawing for invalid points and resume drawing once valid ones are returned again ???????? 

     // draw lines through points in array     

     outimage.canvas.draw_lines(pen_p, pts, npts);


    return 0; 
}
M_endf


// -----------------------------------
// form with parms to build simple pen
// -----------------------------------

int draw_ll_line(outimage_S& outimage, const mapdata_S& mapdata, const rgb_S& color, alpha_T alpha, int32_t pen_width,  real_T lat1, real_T lon1, real_T lat2, real_T lon2, real_T step_deg, const mapparm_S& parm) try
{
    Gdiplus::Pen temp_pen(Gdiplus::Color(alpha, color.r, color.g, color.b), (Gdiplus::REAL)pen_width);

    return draw_ll_line(outimage, mapdata, &temp_pen, lat1, lon1, lat2, lon2, step_deg, parm);
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  draw_ll_lines -- draw lines at lat/lon -- input vectors of lat lon points (endpoints of line segments) -- segments will be curved to fit graticule, etc.  
//  =============    ----------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// form with caller-supplied pen
// -----------------------------

int draw_ll_lines(outimage_S& outimage, const mapdata_S& mapdata, const Gdiplus::Pen *pen_p, const std::vector<real_T>& v_lat, const std::vector<real_T>& v_lon, real_T step_deg, const mapparm_S& parm) try
{
    Gdiplus::PointF pts[10001]; 
    int             pts_ix   {0}; 
    int             npts; 
    real_T          rowf;
    real_T          colf;
    

    // check out input vectors first
    // -----------------------------

    if (v_lat.size() < 2)
    {
        M_out_emsg(L"draw_ll_lines() -- input vector of latitudes has only %d point(s)") % v_lat.size(); 
        return -1;     
    }

    if (v_lon.size() < 2)
    {
        M_out_emsg(L"draw_ll_lines() -- input vector of longitudes has only %d point(s)") % v_lon.size(); 
        return -1;     
    }

    if (v_lon.size() != v_lat.size())
    {
        M_out_emsg(L"draw_ll_lines() -- input vectors of longitudes and latitudes are not same size:  v_lat.size()=%d  v_lon.size()=%d") % v_lat.size() % v_lon.size(); 
        return -1;     
    }


    // -----------------------------------------------
    // Main loop to draw each line segment in polyline
    // -----------------------------------------------

    auto n_segments = v_lat.size() - 1;            // same as v_lon.size()

    int i0 {0};                                    // 1st loop pass -- include 1st endpoint 


    for (auto ix = 0; ix < n_segments; ix++)
    {
        auto lat1 = v_lat.at(ix)  ; 
        auto lat2 = v_lat.at(ix+1); 
        auto lon1 = v_lon.at(ix)  ; 
        auto lon2 = v_lon.at(ix+1);

        // compute number of points for draw_lines -- max = 1000
    
        real_T len = hypot(lat2 - lat1, lon2 - lon1);   // very approximate formula 
           
        if (step_deg >= 0.01)  
           npts = M_lim(2, len/step_deg, 1000); 
        else
           npts = 1000;

        M__(M_out(L"draw_ll_lines() -- ix=%d  len=%.6f   step_deg=%.6f  npts = %d") % ix % len % step_deg % npts;) 


        //  inner loop to populate pts array with all intermediate points for this line segment
        //  -----------------------------------------------------------------------------------
         
        for (int i=i0; i < npts; i++)
        {
            real_T fract = (real_T)i / (real_T)(npts - 1);
     
            real_T lat = lat1 + fract * (lat2 - lat1);
            real_T lon = lon1 + fract * (lon2 - lon1);
           
            auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, true); 

            if ( (rc_rc == M_ROW_COL_RC_OK) || (rc_rc == M_ROW_COL_RC_OUTMAP) )    // only add valid returned row/col to list of points
            { 
                 pts[pts_ix].X  = M_map_window_x(colf, outimage, mapdata);
                 pts[pts_ix].Y  = M_map_window_y(rowf, outimage, mapdata);
                 M__(M_out("i=%d   ix=%d    lat = %.6f     lon = %.6f    row = %.6f/%.6f    col = %.6f/%.6f") % i % ix % lat % lon % rowf  % pts[ix].Y % colf % pts[ix].Y;)
                 pts_ix++;
                 i0 = 1;      // no need to do 1st endpoint next time 
            }
            else
            {
                 i0 = 0;     // need to include 1st endpoint next time, since this point was not drawn
            }
        }

        // ??????? need to interrupt line drawing for invalid points and resume drawing once valid ones are returned again ???????? 

        // draw lines through points in array, if enough points have been accumulated in array to endanger overflow next loop pass     

        if (pts_ix >= 10000 - 1000 - 100)
        {
            M__(M_out(L"draw_ll_lines() -- drawing lines -- number of points = %d") % pts_ix;) 

            outimage.canvas.draw_lines(pen_p, pts, pts_ix);  // draw out accumulated points
            pts_ix = 0;                                   // start filling from index 0 next time
            i0 = 0;                                       // need to include 1st endpoint next time
        }
    }            // end of outer loop
    

    // draw out any left-over points at end of main loop

    if (pts_ix > 0)
    {
        M__(M_out(L"draw_ll_lines() -- drawing leftover lines -- number of points = %d") % pts_ix;) 

        outimage.canvas.draw_lines(pen_p, pts, pts_ix);  // draw out accumulated points
        pts_ix = 0;                                   // not needed ??
        i0 = 0;                                       // not needed ??
    }  

    return 0; 
}
M_endf


// -----------------------------------
// form with parms to build simple pen  ??????? not yet ??????
// -----------------------------------
/*
int draw_ll_lines(outimage_S& outimage, const mapdata_S& mapdata, const rgb_S& color, alpha_T alpha, int32_t pen_width,  real_T lat1, real_T lon1, real_T lat2, real_T lon2, real_T step_deg, const mapparm_S& parm) try
{
    Gdiplus::Pen temp_pen(Gdiplus::Color(alpha, color.r, color.g, color.b), (Gdiplus::REAL)pen_width);

    return draw_ll_lines(outimage, mapdata, &temp_pen, lat1, lon1, lat2, lon2, step_deg, parm);
}
M_endf
*/




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  draw_ll_curve -- draw curve at lat/lon -- curve will follow graticule, etc.  
//  =============    ----------------------
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int draw_ll_curve(outimage_S& outimage, const mapdata_S& mapdata, const Gdiplus::Pen *pen_p, real_T lat1, real_T lon1, real_T lat2, real_T lon2, real_T step_deg, const mapparm_S& parm) try
{
    Gdiplus::PointF pts[1001]; 
    int             npts; 
    real_T          rowf;
    real_T          colf;
    

    // compute number of points for draw_curve -- max = 1000
    // -----------------------------------------------------
    
    real_T          len = hypot(lat2 - lat1, lon2 - lon1);   
           
    if (step_deg >= 0.01)  
       npts = M_lim(2, len/step_deg, 1000);  
    else
       npts = 1000;

     M_y(M_out(L"len = %.6f     step_deg=%.6f       npts = %d") % len % step_deg % npts;) 


     // loop to populate pts array with up to 1000 intermediate points
     // --------------------------------------------------------------

     for (int i=0; i <= npts; i++)
     {
         real_T fract = (real_T)i / (real_T)(npts - 1);
     
         real_T lat = lat1 + fract * (lat2 - lat1);
         real_T lon = lon1 + fract * (lon2 - lon1);
           
         auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, true);

         if ( (rc_rc == M_ROW_COL_RC_OK) || (rc_rc == M_ROW_COL_RC_OUTMAP) )    // only add valid returned row/col to list of points
         {
             pts[i].X  = M_map_window_x(colf, outimage, mapdata);
             pts[i].Y  = M_map_window_y(rowf, outimage, mapdata);

             M_y(M_out("i = %d    lat = %.6f     lon = %.6f    row = %.6f/%.6f    col = %.6f/%.6f") % i % lat % lon % rowf % pts[i].Y % colf % pts[i].Y;)
         }
     }


    // draw curve through points in array
    // ----------------------------------

    outimage.canvas.draw_curve(pen_p, pts, npts);   

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  draw_ll_path() -- draw caller's (outline-in) path at lat/lon -- path will be rotated to line up with local graticule orientation (east-west) 
//  ==============    --------------------------------------------------------------------------------------------------------------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int draw_ll_path(outimage_S& outimage, const mapdata_S& mapdata, const Gdiplus::Pen *pen_p, const Gdiplus::Brush *background_brush_p, const Gdiplus::GraphicsPath *path_p, llr_T lat, llr_T lon, const mapparm_S& parm) try
{
    M__(M_out(L"draw_ll_path() -- called: lat=%.6f  lon=%.6f") % lat % lon;)

    Gdiplus::GraphicsPath  path {Gdiplus::FillModeWinding};           // local path -- starts out empty 
    Gdiplus::Matrix        mx   {                        };           // transform matrix for fill_path()
    

    // get X, Y Gdiplus pixel coordinates for passed-in latitude/longitude
    // -------------------------------------------------------------------

    rcf_T rowf    {}; 
    rcf_T colf    {};

    auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, false);

    if ( (rc_rc != M_ROW_COL_RC_OK) && (rc_rc != M_ROW_COL_RC_OUTMAP) )
        return -1; 
             
    Gdiplus::REAL x    {(Gdiplus::REAL)M_map_window_x(colf, outimage, mapdata)};
    Gdiplus::REAL y    {(Gdiplus::REAL)M_map_window_y(rowf, outimage, mapdata)};


    // Set up transform matrix based on row/col for lat/lon and local graticule orientation
    // ------------------------------------------------------------------------------------
  
    real_T lat_rot { 0.0 }; 
    real_T lon_rot { 0.0 }; 

    auto r_rc = get_ll_rotation(mapdata, lat_rot, lon_rot, lat, lon, parm);   // rotation is mathematical
    if (r_rc != 0)
       return -1;

    mx.Translate(x, y);
    mx.Rotate(-(Gdiplus::REAL)lon_rot);                                             // need to use negative math rotation for gdiplus


    //  Copy passed-in path into modifiable local path 
    //  ----------------------------------------------
   
    path.AddPath(path_p, false);                                     // path was empty before add, this is all that's in it 
  

    // Write path bounding rectangle with passed-in background brush (if any)
    // ----------------------------------------------------------------------

    if (background_brush_p != nullptr)
    { 
        Gdiplus::Matrix        mxb              {                                    };        // transform matrix for GetBounds()
        Gdiplus::GraphicsPath  pathb            {Gdiplus::FillModeWinding            };        // graphics path for background                  
        Gdiplus::RectF         bounds_rect      {                                    };        // bounds rectangle from GetBounds
        Gdiplus::Pen           background_pen   {Gdiplus::Color { 0, 0, 0}, 1        };        // only thickness is used from this pen when computing bounding rectangle
                                                                       
        path.GetBounds(&bounds_rect, &mxb, &background_pen);
                                                                                               
        pathb.AddRectangle(bounds_rect);                                                       // add bounding rectangle into background path object
        pathb.Transform(&mx);                                                                  // apply computed transform to match graticule                                                                                                
        outimage.canvas.fill_path(background_brush_p, &pathb);                                    // draw filled-in path object
    }
  
 
    // Transform path and draw it (outline) into into graphics_S DIBSections
    // ---------------------------------------------------------------------
 
    path.Transform(&mx); 
    auto g_rc = outimage.canvas.draw_path(pen_p, &path);     
    if (g_rc != 0)
        return -1;

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  draw_llfix_path() -- draw caller's (outline) path at lat/lon -- path will not be rotated  
//  =================    ---------------------------------------------------------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int draw_llfix_path(outimage_S& outimage, const mapdata_S& mapdata, const Gdiplus::Pen *pen_p, const Gdiplus::Brush *background_brush_p, const Gdiplus::GraphicsPath *path_p, llr_T lat, llr_T lon, const mapparm_S& parm) try
{
    M__(M_out(L"fill_llfix_path() -- called: lat=%.6f  lon=%.6f") % lat % lon;)

    Gdiplus::GraphicsPath  path {Gdiplus::FillModeWinding};           // local path -- starts out empty 
    Gdiplus::Matrix        mx   {                        };           // transform matrix for fill_path()
    

    // get X, Y Gdiplus pixel coordinates for passed-in latitude/longitude
    // -------------------------------------------------------------------

    rcf_T rowf    {}; 
    rcf_T colf    {};

    auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, false);

    if ( (rc_rc != M_ROW_COL_RC_OK) && (rc_rc != M_ROW_COL_RC_OUTMAP) )
        return -1; 
             
    Gdiplus::REAL x    {(Gdiplus::REAL)M_map_window_x(colf, outimage, mapdata)};
    Gdiplus::REAL y    {(Gdiplus::REAL)M_map_window_y(rowf, outimage, mapdata)};

  
    // Set up transform matrix based on row/col for lat/lon (do not try to match local graticule orientation)
    // ------------------------------------------------------------------------------------------------------
  
    mx.Translate(x, y);
  

    //  Copy passed-in path into local (modifiable) path 
    //  ------------------------------------------------
   
    path.AddPath(path_p, false);                                     // path was empty before add, this is all that's in it 


    // Write path bounding rectangle with passed-in background brush (if any)
    // ----------------------------------------------------------------------

    if (background_brush_p != nullptr)
    { 
        Gdiplus::Matrix        mxb              {                                    };        // transform matrix for GetBounds()
        Gdiplus::GraphicsPath  pathb            {Gdiplus::FillModeWinding            };        // graphics path for background                  
        Gdiplus::RectF         bounds_rect      {                                    };        // bounds rectangle from GetBounds
        Gdiplus::Pen           background_pen   {Gdiplus::Color { 0, 0, 0}, 1        };        // only thickness is used from this pen when computing bounding rectangle
                                                                       
        path.GetBounds(&bounds_rect, &mxb, &background_pen);
                                                                                               
        pathb.AddRectangle(bounds_rect);                                                       // add bounding rectangle into background path object
        pathb.Transform(&mx);                                                                  // apply computed transform to match graticule                                                                                                
        outimage.canvas.fill_path(background_brush_p, &pathb);                                 // draw filled-in path object
    }
   
 
    // Transform path and draw it (outline) into into graphics_S DIBSections
    // ---------------------------------------------------------------------
   
    path.Transform(&mx); 
    auto g_rc = outimage.canvas.draw_path(pen_p, &path);     
    if (g_rc != 0)
        return -1;

    return 0;  
}
M_endf





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  fill_ll_path() -- draw caller's (filled-in) path at lat/lon -- path will be rotated to line up with local graticule orientation (east-west) 
//  ==============    -------------------------------------------------------------------------------------------------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fill_ll_path(outimage_S& outimage, const mapdata_S& mapdata, const Gdiplus::Brush *brush_p, const Gdiplus::Brush *background_brush_p, const Gdiplus::GraphicsPath *path_p, llr_T lat, llr_T lon, const mapparm_S& parm) try
{
    M__(M_out(L"fill_ll_path() -- called: lat=%.6f  lon=%.6f") % lat % lon;)

    Gdiplus::GraphicsPath  path {Gdiplus::FillModeWinding};           // local path -- starts out empty 
    Gdiplus::Matrix        mx   {                        };           // transform matrix for fill_path()
    

    // get X, Y Gdiplus pixel coordinates for passed-in latitude/longitude
    // -------------------------------------------------------------------

    rcf_T rowf    {}; 
    rcf_T colf    {};

    auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, false);

    if ( (rc_rc != M_ROW_COL_RC_OK) && (rc_rc != M_ROW_COL_RC_OUTMAP) )
        return -1; 
             
    Gdiplus::REAL x    {(Gdiplus::REAL)M_map_window_x(colf, outimage, mapdata)};
    Gdiplus::REAL y    {(Gdiplus::REAL)M_map_window_y(rowf, outimage, mapdata)};


    // Set up transform matrix based on row/col for lat/lon and local graticule orientation
    // ------------------------------------------------------------------------------------
  
    real_T lat_rot { 0.0 }; 
    real_T lon_rot { 0.0 }; 

    auto r_rc = get_ll_rotation(mapdata, lat_rot, lon_rot, lat, lon, parm);   // rotation is mathematical
    if (r_rc != 0)
       return -1;

    mx.Translate(x, y);
    mx.Rotate(-(Gdiplus::REAL)lon_rot);                                             // need to use negative math rotation for gdiplus


    //  Copy passed-in path into modifiable local path 
    //  ----------------------------------------------
   
    path.AddPath(path_p, false);                                     // path was empty before add, this is all that's in it 
  

    // Write path bounding rectangle with passed-in background brush (if any)
    // ----------------------------------------------------------------------

    if (background_brush_p != nullptr)
    { 
        Gdiplus::Matrix        mxb              {                                    };        // transform matrix for GetBounds()
        Gdiplus::GraphicsPath  pathb            {Gdiplus::FillModeWinding            };        // graphics path for background                  
        Gdiplus::RectF         bounds_rect      {                                    };        // bounds rectangle from GetBounds
        Gdiplus::Pen           background_pen   {Gdiplus::Color { 0, 0, 0}, 1        };        // only thickness is used from this pen when computing bounding rectangle
                                                                       
        path.GetBounds(&bounds_rect, &mxb, &background_pen);
                                                                                               
        pathb.AddRectangle(bounds_rect);                                                       // add bounding rectangle into background path object
        pathb.Transform(&mx);                                                                  // apply computed transform to match graticule                                                                                                
        outimage.canvas.fill_path(background_brush_p, &pathb);                                 // draw filled-in path object
    }
        
 
    // Transform path and draw it (filled-in) into into graphics_S DIBSections
    // -----------------------------------------------------------------------
        
    path.Transform(&mx); 
    auto g_rc = outimage.canvas.fill_path(brush_p, &path);     
    if (g_rc != 0)
        return -1;

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  fill_llfix_path() -- draw caller's (filled-in) path at lat/lon -- path will not be rotated  
//  =================    ---------------------------------------------------------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fill_llfix_path(outimage_S& outimage, const mapdata_S& mapdata, const Gdiplus::Brush *brush_p, const Gdiplus::Brush *background_brush_p, const Gdiplus::GraphicsPath *path_p, llr_T lat, llr_T lon, const mapparm_S& parm) try
{
    M__(M_out(L"fill_llfix_path() -- called: lat=%.6f  lon=%.6f") % lat % lon;)

    Gdiplus::GraphicsPath  path {Gdiplus::FillModeWinding};           // local path -- starts out empty 
    Gdiplus::Matrix        mx   {                        };           // transform matrix for fill_path()
    

    // get X, Y Gdiplus pixel coordinates for passed-in latitude/longitude
    // -------------------------------------------------------------------

    rcf_T rowf    {}; 
    rcf_T colf    {};

    auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, false);

    if ( (rc_rc != M_ROW_COL_RC_OK) && (rc_rc != M_ROW_COL_RC_OUTMAP) )
        return -1; 
             
    Gdiplus::REAL x    {(Gdiplus::REAL)M_map_window_x(colf, outimage, mapdata)};
    Gdiplus::REAL y    {(Gdiplus::REAL)M_map_window_y(rowf, outimage, mapdata)};

  
    // Set up transform matrix based on row/col for lat/lon (do not try to match local graticule orientation)
    // ------------------------------------------------------------------------------------------------------
  
    mx.Translate(x, y);
  

    //  Copy passed-in path into local (modifiable) path 
    //  ------------------------------------------------
   
    path.AddPath(path_p, false);                                     // path was empty before add, this is all that's in it 


    // Write path bounding rectangle with passed-in background brush (if any)
    // ----------------------------------------------------------------------

    if (background_brush_p != nullptr)
    { 
        Gdiplus::Matrix        mxb              {                                    };        // transform matrix for GetBounds()
        Gdiplus::GraphicsPath  pathb            {Gdiplus::FillModeWinding            };        // graphics path for background                  
        Gdiplus::RectF         bounds_rect      {                                    };        // bounds rectangle from GetBounds
        Gdiplus::Pen           background_pen   {Gdiplus::Color { 0, 0, 0}, 1        };        // only thickness is used from this pen when computing bounding rectangle
                                                                       
        path.GetBounds(&bounds_rect, &mxb, &background_pen);
                                                                                               
        pathb.AddRectangle(bounds_rect);                                                       // add bounding rectangle into background path object
        pathb.Transform(&mx);                                                                  // apply computed transform to match graticule                                                                                                
        outimage.canvas.fill_path(background_brush_p, &pathb);                                 // draw filled-in path object
    }
   
 
    // Transform path and draw it (filled-in) into into graphics_S DIBSections
    // -----------------------------------------------------------------------
   
    path.Transform(&mx); 
    auto g_rc = outimage.canvas.fill_path(brush_p, &path);     
    if (g_rc != 0)
        return -1;

    return 0;  
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// ----------------------------------------------------------------------------------------------
// get_rc_distance() --  internal routine to compute row/col distance given two lat/lon endpoints
// ----------------------------------------------------------------------------------------------

static int get_rc_distance(const mapdata_S& mapdata, real_T& dist, llr_T lat1, llr_T lon1, llr_T lat2, llr_T lon2) try
{
    rcf_T rowf1;
    rcf_T colf1;
    rcf_T rowf2;
    rcf_T colf2;   

    // compute pixel length of line between two passed-in lat/lon points   
    // ------------------------------------------------------------------

    auto rc1_rc = (*(mapdata.get_row_col_p))(mapdata, rowf1, colf1, lat1, lon1, true);
    auto rc2_rc = (*(mapdata.get_row_col_p))(mapdata, rowf2, colf2, lat2, lon2, true);

    M__(M_out(L"get_rc_distance() -- lat1/lon1 = %.6f/%.6f      lat2/lon2 = %.6f/%.6f  rc1/rc2 = %d/%d") % lat1 % lon1 % lat2 % lon2 % rc1_rc % rc2_rc;)

    if (
        ( (rc1_rc != M_ROW_COL_RC_OK) && (rc1_rc != M_ROW_COL_RC_OUTMAP) )    // only accept valid returned row/col
        ||
        ( (rc2_rc != M_ROW_COL_RC_OK) && (rc2_rc != M_ROW_COL_RC_OUTMAP) )
       )
    {
        return -1;      // can't calculate rotation when no valid row/col for one or both ends    
    }
    
    dist = hypot(rowf2 - rowf1, colf2 - colf1); 
    M__(M_out(L"get_rc_distance() -- distance = %.6f     lat1/lon1 = %.6f/%.6f     lat2/lon2 = %.6f/%.6f") % dist % lat1 % lon1 % lat2 % lon2;)
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ll_line_rotation() -- compute lat/lon-based line segment rotation (between two endpoints)  based on input rhumb angle(in degrees) and line length in pixels  
//  ==================    -------------------------------------------------------------------------------------------------------------------------------------
//
//   note: 0 degree rhumb angle = towards east
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

real_T ll_line_rotation(const mapdata_S& mapdata, llr_T lat1, llr_T lon1, real_T rhumbd, real_T len, const mapparm_S& parm) try
{
    M__(M_out(L"ll_rotation() called -- lat/lon = %.6d/%.6d    len = %.6f  rhumb = %.6f") % lat1 % lon1 % len % rhumbd;)   

    // error if input length is non-zero
    // ---------------------------------

    if (len == 0.0)
       return 0.0; 


    // compute approximate PPD for passed-in rhumb angle
    // -------------------------------------------------

    real_T ppd_lat, ppd_lon;

    auto glp_rc = get_ll_ppd(mapdata, ppd_lat, ppd_lon, lat1, lon1, parm);

    if (glp_rc != 0)     // if actual PPD for this lat/lon can't be computed, use approximate average PPD for whole map (hopefully iterations will converge fast enough from this poor 1st approximation to get accurate end result)
    {
        ppd_lat = 0.5 * ((mapdata.min_ppd + mapdata.max_ppd));
        ppd_lon = 0.5 * ((mapdata.min_ppd + mapdata.max_ppd));
    }
 
    M__(M_out(L"ll_rotation() -- at starting point: ppd_lat = %.6d   ppd_lon = %.6d -- glp_rc = %d") % ppd_lat % ppd_lon % glp_rc;)   


    //  compute guessed lat/lon of other end, assuming ppd is constant along whole line length ( will fail here, if latitude is about +90 or -90 degrees ????????????????????????)
    //  --------------------------------------------------------------------------------------
    
    real_T lat2      {lat1 + (len / ppd_lat) * M_sind(rhumbd)}; 
    real_T lon2      {lon1 + (len / ppd_lon) * M_cosd(rhumbd)}; 
    real_T ppdav_lat {                                       };
    real_T ppdav_lon {                                       };

  
    // compute pixel length and average PPD along line between input starting point and guessed lat/lon at other end  
    // -------------------------------------------------------------------------------------------------------------

    real_T dist; 
    auto gd_rc = get_rc_distance(mapdata, dist, lat1, lon1, lat2, lon2); 
    if (gd_rc != 0) return 0.0;                                           // if this failed, can't return any valid rotation


    //  if required, compute 1st improved guessed lat/lon of other end (use average PPD just computed)
    //  ---------------------------------------------------------------------------------

    if  ( (dist/len < 0.99999) || (dist/len > 1.00001) )
    {
        M__(M_out(L"ll_rotation() -- passed-in len = %.6f  rhumb = %.6f -- 1st computed ending lat/lon = %.6f/%.6f    1st computed distance = %.6f (dist/len=%.16d") % len % rhumbd % lat2 % lon2 % dist % (dist / len);)     

        ppdav_lat = ppd_lat*dist/len;     // adjusted average PPD 
        ppdav_lon = ppd_lon*dist/len;     // adjusted average PPD

        M__(M_out(L"ll_rotation() -- PPD at starting point = %.6f/%.6f    1st average PPD = %.6f/%.6f") % ppd_lat % ppd_lon % ppdav_lat % ppdav_lon;)     


        //  compute 1st improved guessed lat/lon of other end (use average PPD just computed)
        //  ---------------------------------------------------------------------------------
    
        lat2 = lat1 + (len / ppdav_lat) * M_sind(rhumbd); 
        lon2 = lon1 + (len / ppdav_lon) * M_cosd(rhumbd); 

        gd_rc = get_rc_distance(mapdata, dist, lat1, lon1, lat2, lon2);       // use updated lat/lon of other end
        if (gd_rc != 0) return 0.0;                                           // if this failed, can't return any valid rotation
    }


    // if required, recompute pixel length and average PPD along line between input starting point and improved guessed lat/lon at other end  
    // -------------------------------------------------------------------------------------------------------------------------------------

    if  ( (dist/len < 0.99999) || (dist/len > 1.00001) )
    {  
        M__(M_out(L"ll_rotation() -- passed-in len = %.6f  rhumb = %.6f -- 2nd computed ending lat/lon = %.6f/%.6f    2nd computed distance = %.6f (dist/len=%.16d") % len % rhumbd % lat2 % lon2 % dist % (dist / len);)     

        ppdav_lat *= (dist/len);     // readjusted average PPD 
        ppdav_lon *= (dist/len);     // readjusted average PPD

        M__(M_out(L"ll_rotation() -- PPD at starting point = %.6f/%.6f    2nd average PPD = %.6f/%.6f") % ppd_lat % ppd_lon % ppdav_lat % ppdav_lon;)     


        //  compute 2nd improved guessed lat/lon at other end
        //  -------------------------------------------------
    
        lat2 = lat1 + (len / ppdav_lat) * M_sind(rhumbd); 
        lon2 = lon1 + (len / ppdav_lon) * M_cosd(rhumbd); 

        gd_rc = get_rc_distance(mapdata, dist, lat1, lon1, lat2, lon2);       // use updated lat/lon of other end
        if (gd_rc != 0) return 0.0;     
    }

    // if required, recompute pixel length and average PPD, once again, along line between input starting point and improved guessed lat/lon at other end  
    // --------------------------------------------------------------------------------------------------------------------------------------------------

    if  ( (dist/len < 0.99999) || (dist/len > 1.00001) )
    {
        M__(M_out(L"ll_rotation() -- passed-in len = %.6f  rhumb = %.6f -- 3rd computed ending lat/lon = %.6f/%.6f    2nd computed distance = %.6f (dist/len=%.16d") % len % rhumbd % lat2 % lon2 % dist % (dist / len);)     
     
        ppdav_lat *= (dist/len);     // readjusted average PPD 
        ppdav_lon *= (dist/len);     // readjusted average PPD
     
        M__(M_out(L"ll_rotation() -- PPD at starting point = %.6f/%.6f    3rd average PPD = %.6f/%.6f") % ppd_lat % ppd_lon % ppdav_lat % ppdav_lon;)     
     
     
        //  compute 3rd improved guessed lat/lon at other end
        //  -------------------------------------------------
        
        lat2 = lat1 + (len / ppdav_lat) * M_sind(rhumbd); 
        lon2 = lon1 + (len / ppdav_lon) * M_cosd(rhumbd); 
   
        get_rc_distance(mapdata, dist, lat1, lon1, lat2, lon2);       // use updated lat/lon of other end 
    }
   

    // put out message, if final dist/len ratio is not too accurate
    // ------------------------------------------------------------

    if  ( (dist/len < 0.99999) || (dist/len > 1.00001) )
        M_out(L"ll_rotation() -- final adjusted dist/len = %.16f   lat1/lon1 = %.8f/%.8f    lat2/lon2 = %.8f/%.8f <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<") % (dist/len) % lat1 %lon1 % lat2 % lon2; 
     

    // compute ending row column and rotation of line based on final updated ending lat/lon
    // ------------------------------------------------------------------------------------

    real_T rowf1;
    real_T colf1;
    real_T rowf2;
    real_T colf2;
    
    auto rc1_rc = (*(mapdata.get_row_col_p))(mapdata, rowf1, colf1, lat1, lon1, false);
    auto rc2_rc = (*(mapdata.get_row_col_p))(mapdata, rowf2, colf2, lat2, lon2, false);

    if (
        ( (rc1_rc != M_ROW_COL_RC_OK) && (rc1_rc != M_ROW_COL_RC_OUTMAP) )    // only accept valid returned row/col
        ||
        ( (rc2_rc != M_ROW_COL_RC_OK) && (rc2_rc != M_ROW_COL_RC_OUTMAP) )
       )
    {
           return 0.0;      // can't calculate rotation when no valid row/col for one or both ends    
    } 

    real_T rotation = M_atan2d(rowf1 - rowf2, colf2 - colf1);    // lower row numbers are higher Y, note: gdiplus transform rotation is the negative of the passed-back rotation 
     
    M__(M_out(L"ll_rotation() -- -  -   -   -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  ");)
    M__(M_out(L"ll_rotation() -- lat1=%.6f   lon1=%.6f  rhumb=%.6f   len=%.6f   lat2=%.6f   lon2=%.6f") % lat1 % lon1 % rhumbd % len % lat2 % lon2; )                                             
    M__(M_out(L"ll_rotation() -- rowf1=%.6f   colf1=%.6f   rowf2=%.4f    colf2=%.4f") % rowf1 % colf1 % rowf2 % colf2;)
    M__(M_out(L"ll_rotation() -- rotation = %.6f") % rotation;)
      

   // real_T rot_lat, rot_lon; 
   // get_ll_rotation(mapdata, rot_lat,rot_lon, lat1, lon1, parm);
   // M__(M_out(L"ll_rotation() -- output rotation = %.8f   lat/lon rotation = %.8f/%.8f") % rotation % rot_lat % rot_lon;)


    return rotation; 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        set_map_window() -- set up map window for following put-map() and annotate_map() functions 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int set_map_window(outimage_S& outimage, mapdata_S& mapdata, const set_map_window_parm_S& fparm, const mapparm_S& parm) try
{
    // do debug printout of incoming set map window parms

    if (fparm.display)
    {
        M_out(L"OutImage:    set_map_window() -- outimage_top_row      : %-d"      ) % fparm.outimage_top_row; 
        M_out(L"OutImage:    set_map_window() -- outimage_left_column  : %-d"      ) % fparm.outimage_left_column; 
        M_out(L"MapData:     set_map_window() -- map_top_row           : %-d"      ) % fparm.map_top_row; 
        M_out(L"MapData:     set_map_window() -- map_left_column       : %-d"      ) % fparm.map_left_column; 
        M_out(L"MapData:     set_map_window() -- map_width             : %-d"      ) % fparm.map_width; 
        M_out(L"MapData:     set_map_window() -- map_height            : %-d"      ) % fparm.map_height; 
    }                                                                                            
     

    // compute actual dimensions of map window (check for validity)
    // ------------------------------------------------------------

    rc_T window_width     {fparm.map_width };
    rc_T window_height    {fparm.map_height}; 


    if ( (fparm.map_top_row < 0) || (fparm.map_top_row >= mapdata.rows) ) 
    {
        M_out_emsg(L"set_map_window() -- map_top_row (%d) invalid -- must be between 0 and %d") % fparm.map_top_row % (mapdata.rows - 1);     
        return -1; 
    }

    if ( (fparm.map_left_column < 0) || (fparm.map_left_column >= mapdata.cols) ) 
    {
        M_out_emsg(L"set_map_window() -- map_left_column (%d) invalid -- must be between 0 and %d") % fparm.map_left_column % (mapdata.cols - 1);     
        return -1; 
    }
      
    if (window_height <= 0)   window_height = mapdata.rows - fparm.map_top_row;        // 0 means entire map (starting at top row, left column)    
    if (window_width  <= 0)   window_width  = mapdata.cols - fparm.map_left_column;    // 0 means entire map (starting at top row, left column) 

    if (fparm.map_top_row + window_height > mapdata.rows )
    {
        M_out_emsg(L"set_map_window() -- map (window) height (%d) too large -- must be less than %d (map.rows-map_top_row = %d-%d)") % window_height % (mapdata.rows - fparm.map_top_row) % mapdata.rows % fparm.map_top_row;    
        return -1; 
    }

    if (fparm.map_left_column + window_width > mapdata.cols )
    {
        M_out_emsg(L"set_map_window() -- map (window) width (%d) too large -- must be less than %d (map.cols-map_left_col = %d-%d)") % window_width % (mapdata.cols - fparm.map_left_column) % mapdata.cols % fparm.map_left_column;    
        return -1; 
    }


    // validate outimage starting row/col and make sure map window doesn't overflow outimage buffer 
    // --------------------------------------------------------------------------------------

    if ( (fparm.outimage_top_row < 0) || (fparm.outimage_top_row >= outimage.rows) ) 
    {
        M_out_emsg(L"set_map_window() -- outimage_top_row (%d) invalid -- must be between 0 and %d") % fparm.outimage_top_row % (outimage.rows - 1);     
        return -1; 
    }
   
    if ( (fparm.outimage_left_column < 0) || (fparm.outimage_left_column >= outimage.cols) ) 
    {
        M_out_emsg(L"set_map_window() -- outimage_left_column (%d) invalid -- must be between 0 and %d") % fparm.outimage_left_column % (outimage.cols - 1);     
        return -1; 
    }

    if (fparm.outimage_top_row + window_height > outimage.rows )
    {
        M_out_emsg(L"set_map_window() -- map (window) height (%d) too large -- must be less than %d (outimage.rows-outimage_top_row = %d-%d)") % window_height % (outimage.rows - fparm.outimage_top_row) % outimage.rows % fparm.outimage_top_row;    
        return -1; 
    }

    if (fparm.outimage_left_column + window_width > outimage.cols )
    {
        M_out_emsg(L"set_map_window() -- map (window) width (%d) too large -- must be less than %d (outimage.cols-outimage_left_column = %d-%d)") % window_width % (outimage.cols - fparm.outimage_left_column) % outimage.cols % fparm.outimage_left_column;    
        return -1; 
    }
    

    // save map window settings in outimage_S and mapdata_S areas
    // --------------------------------------------------------

    outimage.curr_window_width          = window_width                                    ;
    outimage.curr_window_height         = window_height                                   ;
    outimage.curr_window_top_row        = fparm.outimage_top_row                          ; 
    outimage.curr_window_left_col       = fparm.outimage_left_column                      ;
    outimage.curr_window_bottom_row     = fparm.outimage_top_row     + window_height - 1  ; 
    outimage.curr_window_right_col      = fparm.outimage_left_column + window_width  - 1  ;
    outimage.curr_window_valid          = true                                            ;
                                                                                       
                                                                                       
    mapdata.map_window_width           = window_width                                    ;
    mapdata.map_window_height          = window_height                                   ;
    mapdata.map_window_top_row         = fparm.map_top_row                               ; 
    mapdata.map_window_left_col        = fparm.map_left_column                           ;
    mapdata.map_window_bottom_row      = fparm.map_top_row     + window_height - 1       ; 
    mapdata.map_window_right_col       = fparm.map_left_column + window_width  - 1       ;
    mapdata.map_window_valid           = true                                            ;
    

    if (fparm.display)
    {                                                                                                     
        M_out(L"OutImage:    set_map_window() -- map_window_width         : %-d") %  outimage.curr_window_width         ;
        M_out(L"OutImage:    set_map_window() -- map_window_height        : %-d") %  outimage.curr_window_height        ;
        M_out(L"OutImage:    set_map_window() -- map_window_top_row       : %-d") %  outimage.curr_window_top_row       ;
        M_out(L"OutImage:    set_map_window() -- map_window_left_column   : %-d") %  outimage.curr_window_left_col      ; 
        M_out(L"OutImage:    set_map_window() -- map_window_bottom_row    : %-d") %  outimage.curr_window_bottom_row    ;
        M_out(L"OutImage:    set_map_window() -- map_window_right_column  : %-d") %  outimage.curr_window_right_col     ;

        M_out(L"MapData:     set_map_window() -- map_window_width         : %-d") %  mapdata.map_window_width          ;
        M_out(L"MapData:     set_map_window() -- map_window_height        : %-d") %  mapdata.map_window_height         ;
        M_out(L"MapData:     set_map_window() -- map_window_top_row       : %-d") %  mapdata.map_window_top_row        ;
        M_out(L"MapData:     set_map_window() -- map_window_left_column   : %-d") %  mapdata.map_window_left_col       ; 
        M_out(L"MapData:     set_map_window() -- map_window_bottom_row    : %-d") %  mapdata.map_window_bottom_row     ;
        M_out(L"MapData:     set_map_window() -- map_window_right_column  : %-d") %  mapdata.map_window_right_col      ;
    } 
   
    return 0;
}
M_endf





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                                                                                                  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                                                                                                  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        verify_map_window() -- make sure map window is valid and same size in both mapdata_S and outimage_S 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int verify_map_window(const outimage_S& outimage, const mapdata_S& mapdata, const mapparm_S& parm) try
{
    if (outimage.curr_window_valid == false)
    {
        M_out_emsg(L"verify_map_window() -- outimage.curr_window_valid is false -- valid map_window is required in outimage_S"); 
        return -1; 
    }

    if (mapdata.map_window_valid == false)
    {
        M_out_emsg(L"verify_map_window() -- mapdata.map_window_valid is false -- valid map_window is required in mapdata_S"); 
        return -1; 
    }

    if (mapdata.map_window_width != outimage.curr_window_width)
    {
        M_out_emsg(L"verify_map_window() -- map_window in mapdata_S and curr_window in outimage_S are different widths -- mapdata_S window width = %d    outimage_S window width = %d") % mapdata.map_window_width % outimage.curr_window_width;
        return -1;
    }

     if (mapdata.map_window_height != outimage.curr_window_height)
    {
        M_out_emsg(L"verify_map_window() -- map_window in mapdata_S and curr_window in outimage_S are different heights -- mapdata_S window height = %d    outimage_S window height = %d") % mapdata.map_window_height % outimage.curr_window_height;
        return -1;
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
//||        put_map() -- fill in RGB data in outimage_S buffer from data in mapdata_S 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//int put_map(outimage_S& outimage, const mapdata_S& mapdata, const ramp_S& ramp, const put_map_parm_S& fparm, const mapparm_S& parm) try   can't use const ramp_S& -- causes unresolved
int put_map(  outimage_S& outimage, const mapdata_S& mapdata,       ramp_S& ramp, const put_map_parm_S& fparm, const mapparm_S& parm) try
{
    int               rc {0}; 


    // do debug printout of incoming put_map() put parms, saved ramp_S parms, and setup map window spec in mapdata_S and outimage_S areas
    // --------------------------------------------------------------------------------------------------------------------------------

    if (fparm.display)
    {
        M_out(L"OutImage:    put_map() -- shaded_relief                       : %s"       ) % M_bool_cstr(fparm.shaded_relief    )   ;
        M_out(L"OutImage:    put_map() -- land_hypsometry                     : %s"       ) % M_bool_cstr(fparm.land_hypsometry  )   ;
        M_out(L"OutImage:    put_map() -- ocean_bathymetry                    : %s"       ) % M_bool_cstr(fparm.ocean_bathymetry )   ;
        M_out(L"OutImage:    put_map() -- shade_darkest_land                  : %-12.6f"  ) % ramp.parm.shade_darkest_land           ; 
        M_out(L"OutImage:    put_map() -- shade_lightest_land                 : %-12.6f"  ) % ramp.parm.shade_lightest_land          ;
        M_out(L"OutImage:    put_map() -- shade_midtone_land                  : %-12.6f"  ) % ramp.parm.shade_midtone_land           ;
        M_out(L"OutImage:    put_map() -- bright_angle_land                   : %-12.6f"  ) % ramp.parm.bright_angle_land            ;
        M_out(L"OutImage:    put_map() -- dark_angle_land                     : %-12.6f"  ) % ramp.parm.dark_angle_land              ;
        M_out(L"OutImage:    put_map() -- shade_darkest_ocean                 : %-12.6f"  ) % ramp.parm.shade_darkest_ocean          ; 
        M_out(L"OutImage:    put_map() -- shade_lightest_ocean                : %-12.6f"  ) % ramp.parm.shade_lightest_ocean         ;
        M_out(L"OutImage:    put_map() -- shade_midtone_ocean                 : %-12.6f"  ) % ramp.parm.shade_midtone_ocean          ;
        M_out(L"OutImage:    put_map() -- bright_angle_ocean                  : %-12.6f"  ) % ramp.parm.bright_angle_ocean           ;
        M_out(L"OutImage:    put_map() -- dark_angle_ocean                    : %-12.6f"  ) % ramp.parm.dark_angle_ocean             ;  
                                                                                                     
        M_out(L"OutImage:    put_map() -- outimage.curr_window_width          : %-d"      ) %  outimage.curr_window_width            ;
        M_out(L"OutImage:    put_map() -- outimage.curr_window_height         : %-d"      ) %  outimage.curr_window_height           ;
        M_out(L"OutImage:    put_map() -- outimage.curr_window_top_row        : %-d"      ) %  outimage.curr_window_top_row          ;
        M_out(L"OutImage:    put_map() -- outimage.curr_window_left_column    : %-d"      ) %  outimage.curr_window_left_col         ; 
        M_out(L"OutImage:    put_map() -- outimage.curr_window_bottom_row     : %-d"      ) %  outimage.curr_window_bottom_row       ;
        M_out(L"OutImage:    put_map() -- outimage.curr_window_right_column   : %-d"      ) %  outimage.curr_window_right_col        ;
                                                                                                                            
        M_out(L"MapData:     put_map() -- mapdata.map_window_width            : %-d"      ) %  mapdata.map_window_width              ;
        M_out(L"MapData:     put_map() -- mapdata.map_window_height           : %-d"      ) %  mapdata.map_window_height             ;
        M_out(L"MapData:     put_map() -- mapdata.map_window_top_row          : %-d"      ) %  mapdata.map_window_top_row            ;
        M_out(L"MapData:     put_map() -- mapdata.map_window_left_column      : %-d"      ) %  mapdata.map_window_left_col           ; 
        M_out(L"MapData:     put_map() -- mapdata.map_window_bottom_row       : %-d"      ) %  mapdata.map_window_bottom_row         ;
        M_out(L"MapData:     put_map() -- mapdata.map_window_right_column     : %-d"      ) %  mapdata.map_window_right_col          ; 
    }                                                                                            
     

    // verify that map windows in outimage_S and mapdata_S are valid and same size
    // ---------------------------------------------------------------------------

    auto v_rc = verify_map_window(outimage, mapdata, parm); 
    if (v_rc != 0)
        return -1;
 

    // Multi-threaded -- Convert ht/attr/slope data in outimage_S to color in RGB outimage area 
    // ----------------------------------------------------------------------------------------
     
    std::thread    thd[THREAD_MAX];
    thpa_S         thpa;                       // thread parms
    int32_t        i; 

    M_start_timer(c1, t1)
   
    slice_row(thpa, outimage.curr_window_top_row, outimage.curr_window_bottom_row, outimage.curr_window_left_col, outimage.curr_window_right_col, parm.thread.n_outimage_fill, true, parm);   // allow up to n threads for outimage RGB fill operation    
    M_out(L"Map     : outimage RGB fill starting -- %d threads") % thpa.n_threads ; 
               
    for (i = 0; i < thpa.n_threads; i++)
        thd[i] = std::thread( put_map_thd,   std::ref(outimage), std::cref(mapdata), std::cref(ramp), std::ref((thpa.thp)[i]), std::cref(fparm), std::cref(parm) );
         
    for (i = 0; i < thpa.n_threads; i++)
        thd[i].join();         
    
    M_out(L"OutImage : put_map() done -- elapsed time = %.3f seconds") % M_read_timer(c1, t1) ;

    return rc;
}
M_endf



///////////////////////////////////////////////////////////


static void put_map_thd(outimage_S& outimage, const mapdata_S& mapdata, const ramp_S& ramp, thp_S& thp, const put_map_parm_S& fparm, const mapparm_S& parm) try
{
    rc_T row_min = thp.e_r0;          // row/col limits in outimage for this thread 
    rc_T row_max = thp.e_rm;
    rc_T col_min = thp.e_c0;
    rc_T col_max = thp.e_cm;
   
    if (parm.thread.display)
        {M_out_lk(L"Thread %3d: starting -- rows=%-5d/%-5d  cols=%-5d/%-5d") % thp.thread_n % row_min % row_max % col_min % col_max;}


    // set up parm for rgb_color24()

    rgb_color24_parm_S r24parm {}; 

    r24parm.display            = fparm.display          ;        
    r24parm.shaded_relief      = fparm.shaded_relief    ;
    r24parm.land_hypsometry    = fparm.land_hypsometry  ;
    r24parm.ocean_bathymetry   = fparm.ocean_bathymetry ;
   

    //  Main loop to set each pixel in the outimage slice from the mapdata_S (map)
    //  -----------------------------------------------------------------------

    for (rc_T row = row_min; row <= row_max; row++)           // do all rows in passed-in slice
    {
        for (rc_T col = col_min; col <= col_max; col++)       // do all columns in passed-in slice 
        {
            rc_T m_row = row - outimage.curr_window_top_row     + mapdata.map_window_top_row;             // get corresponding row in map
            rc_T m_col = col - outimage.curr_window_left_col    + mapdata.map_window_left_col;            // get corresponding column in map

            if ( (m_row >= 0) && (m_col >= 0) && (m_row < mapdata.rows) && (m_col < mapdata.cols) )
            {
                //  Process valid point from map -- set color based on height, slope, etc.

                ht_T     h = M_mapdata_ht(   mapdata, m_row, m_col); 
                attr_T   a = M_mapdata_attr( mapdata, m_row, m_col);
                slope_T  s = M_mapdata_slope(mapdata, m_row, m_col);
           
                rgb_S rgb = rgb_color24(h, a, s, ramp, r24parm, parm);
                M_outimage_rgb(outimage, row, col) = rgb;
            }
            else //  This point in outimage is out-of-bounds on the map -- set out-of-bounds color
            {
                M_outimage_rgb(outimage, row, col) =  parm.color.outside_map;
            }
         }           
    }

    if (parm.thread.display)
        {M_out_lk(L"Thread %3d: ending   -- rows=%-5d/%-5d  cols=%-5d/%-5d") % thp.thread_n % row_min % row_max % col_min % col_max;}
 
    return;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        put_ramp() -- draw ramp display directly in outimage
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//int32_t put_ramp(outimage_S& outimage,  const ramp_S& ramp,  const ramp_put_parm_S& rparm, const mapparm_S& parm) try -- will not link -- unresolved if const ramp_S& is used
int       put_ramp(outimage_S& outimage,        ramp_S& ramp,  const ramp_put_parm_S& rparm, const mapparm_S& parm) try
{
    int             rc {0}; 

    // set up letter sizes  (fixed sizes for now)

    rc_T wl         {8 }; 
    rc_T hl         {16}; 


    // set up to draw land or ocean ramp LRGB vector

    std::vector<lrgb_S> lrgb; 
    std::wstring        ramp_type;   
    int                 start_ix;           // starting index -- may want to leave out 1st (-32767) box
    int                 text_len;           // max text length for box elevation lables 
    int                 text_offset;        // offset from top of box to top of text letters
    
    if (rparm.ocean)
    {
        lrgb        = ramp.lrgb_ocean; 
        start_ix    = 0; 
        text_len    = 6; 
        text_offset = 0; 
        ramp_type   = L"Ocean";
    }
    else
    {
        lrgb        = ramp.lrgb_land; 
        start_ix    = 0; 
        text_len    = 5;
        text_offset = 0;
        ramp_type   = L"Land";
    }

    auto n_rgb    {lrgb.size()};             // = number of LRGB entries in the ramp 


    // complain, if no LRGB entries in ramp 

    if (n_rgb < 1)
    {
        M_out_emsg(L"put_ramp() -- %s ramp to be drawn has empty LRGB vector") % ramp_type; 
        return -1; 
    }

    if (n_rgb - start_ix < 1)
    {
        M_out_emsg(L"put_ramp() -- %s ramp to be drawn does not have enough LRGB entries to be drawn") % ramp_type; 
        return -1; 
    }

    // compute width and height of color boxes in ramp , and then re-adjusted overall height to keep bottom margin as requested 

    rc_T top_margin = rparm.margin + std::max(0, -text_offset);                                                                   // might need extra space for 1st label
    rc_T w_box      =  rparm.width  - 2 * rparm.border - 2 * rparm.margin - rparm.text_space - text_len * wl          ;           // allow for scale labels of 5 or 6 chars
    rc_T h_box      = (rparm.height - 2 * rparm.border -     rparm.margin - top_margin)             / (n_rgb - start_ix);         // perhaps don't include 1st -32767 ramp entry   
    rc_T adj_height =                 2 * rparm.border +     rparm.margin + top_margin      + h_box * (n_rgb - start_ix);         // readjust, since   n*h_box   may be considerable less than 1/n-th of height 

    // complain if w and/or h are too small to be drawn  

    if (w_box < 36)
    {
        M_out_emsg(L"put_ramp() -- color boxes for %s ramp are not wide enough to be drawn -- width = %d") % ramp_type % w_box; 
        return -1;     
    }

    if (h_box < 18)
    {
        M_out_emsg(L"put_ramp() -- color boxes for %s ramp are not tall enough to be drawn -- height = %d") % ramp_type % h_box; 
        return -1;     
    }  


    // draw filled-in main backgound box with optional border
    // ------------------------------------------------------    
  
    put_box(outimage, rparm.top_row, rparm.left_col, rparm.width, adj_height, rparm.background_color);

    if (rparm.border > 0)
        put_frame(outimage, rparm.top_row, rparm.left_col, rparm.width, adj_height, rparm.border, rparm.border_color);
                                                                                                                  
  

    // draw ramp color boxes
    // ---------------------

    rc_T r_box = rparm.top_row  + rparm.border + top_margin;            // row for 1st color box
    rc_T c_box = rparm.left_col + rparm.border + rparm.margin;          // column for all color boxes

    rgb_S box_rgb1, box_rgb2, box_rgb3, box_rgb4, box_rgb5, box_rgb6, box_rgb7; 

    rc_T w_box1 = w_box/12;                                                         // width for 1st part of box
    rc_T w_box2 = w_box/12;                                                         // width for 2nd part of box
    rc_T w_box3 = w_box/12;                                                         // width for 3nd part of box
    rc_T w_box5 = w_box/12;                                                         // width for 5th part of box
    rc_T w_box6 = w_box/12;                                                         // width for 6th part of box 
    rc_T w_box7 = w_box/12;                                                         // width for 7th part of box
    rc_T w_box4 = w_box - w_box1 - w_box2 - w_box3 - w_box5 - w_box6 - w_box7;      // width for 4rd part of box (make sure wide enough to rest of box)
    rc_T c_box1 = c_box;                                                            // starting column for sub-box
    rc_T c_box2 = c_box1 + w_box1;                                                  // starting column for sub-box
    rc_T c_box3 = c_box2 + w_box2;                                                  // starting column for sub-box
    rc_T c_box4 = c_box3 + w_box3;                                                  // starting column for sub-box
    rc_T c_box5 = c_box4 + w_box4;                                                  // starting column for sub-box
    rc_T c_box6 = c_box5 + w_box5;                                                  // starting column for sub-box
    rc_T c_box7 = c_box6 + w_box6;                                                  // starting column for sub-box
    

    real_T shade_darkest; 
    real_T shade_lightest;
    real_T shade_midtone;

    if (rparm.ocean)
    {
        shade_darkest  = ramp.parm.shade_darkest_ocean; 
        shade_lightest = ramp.parm.shade_lightest_ocean;
        shade_midtone  = ramp.parm.shade_midtone_ocean;
    }
    else
    {
        shade_darkest  = ramp.parm.shade_darkest_land; 
        shade_lightest = ramp.parm.shade_lightest_land;
        shade_midtone  = ramp.parm.shade_midtone_land;    
    }



    for (auto i = start_ix; i < n_rgb; i++)                                         // perhaps don't do 1st -32767 ramp entry    ????????
    {
        wchar_t buf[50] {};

        M_make_rgb( box_rgb1                
                  , (real_T)(lrgb.at(i).r) * shade_darkest
                  , (real_T)(lrgb.at(i).g) * shade_darkest
                  , (real_T)(lrgb.at(i).b) * shade_darkest
                  )
        M_make_rgb( box_rgb2                
                  , (real_T)(lrgb.at(i).r) * (0.6667 * shade_darkest + 0.3333 * shade_midtone)
                  , (real_T)(lrgb.at(i).g) * (0.6667 * shade_darkest + 0.3333 * shade_midtone)
                  , (real_T)(lrgb.at(i).b) * (0.6667 * shade_darkest + 0.3333 * shade_midtone)
                  )
        M_make_rgb( box_rgb3                
                  , (real_T)(lrgb.at(i).r) * (0.3333 * shade_darkest + 0.6667 * shade_midtone)
                  , (real_T)(lrgb.at(i).g) * (0.3333 * shade_darkest + 0.6667 * shade_midtone)
                  , (real_T)(lrgb.at(i).b) * (0.3333 * shade_darkest + 0.6667 * shade_midtone)
                  )                                                                                      
        M_make_rgb( box_rgb4                
                  , (real_T)(lrgb.at(i).r) * shade_midtone
                  , (real_T)(lrgb.at(i).g) * shade_midtone
                  , (real_T)(lrgb.at(i).b) * shade_midtone
                  )
        M_make_rgb( box_rgb5                
                  , (real_T)(lrgb.at(i).r) * (0.6667 * shade_midtone + 0.3333 * shade_lightest)
                  , (real_T)(lrgb.at(i).g) * (0.6667 * shade_midtone + 0.3333 * shade_lightest)
                  , (real_T)(lrgb.at(i).b) * (0.6667 * shade_midtone + 0.3333 * shade_lightest)
                  )                                    
        M_make_rgb( box_rgb6                
                  , (real_T)(lrgb.at(i).r) * (0.3333 * shade_midtone + 0.6667 * shade_lightest)
                  , (real_T)(lrgb.at(i).g) * (0.3333 * shade_midtone + 0.6667 * shade_lightest)
                  , (real_T)(lrgb.at(i).b) * (0.3333 * shade_midtone + 0.6667 * shade_lightest)
                  )                                                                                       
        M_make_rgb( box_rgb7                
                  , (real_T)(lrgb.at(i).r) * shade_lightest
                  , (real_T)(lrgb.at(i).g) * shade_lightest
                  , (real_T)(lrgb.at(i).b) * shade_lightest
                  )
      
        put_box(outimage, r_box, c_box1, w_box1, h_box, box_rgb1);
        put_box(outimage, r_box, c_box2, w_box2, h_box, box_rgb2);  
        put_box(outimage, r_box, c_box3, w_box3, h_box, box_rgb3);  
        put_box(outimage, r_box, c_box4, w_box4, h_box, box_rgb4);  
        put_box(outimage, r_box, c_box5, w_box5, h_box, box_rgb5); 
        put_box(outimage, r_box, c_box6, w_box6, h_box, box_rgb6);  
        put_box(outimage, r_box, c_box7, w_box7, h_box, box_rgb7); 

        swprintf(buf, (sizeof buf)/(sizeof buf[0]) - 1, L"%d", (int)(lrgb.at(i).l));        
        put_string8(outimage, buf, r_box + text_offset, c_box + w_box + rparm.text_space, rparm.text_color);
        
        r_box += h_box; 
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
//||        draw_ramp() -- draw ramp display using GDI+
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//int32_t draw_ramp(outimage_S& outimage,  const ramp_S& ramp,  const ramp_put_parm_S& rparm, const mapparm_S& parm) try -- will not link -- unresolved if const ramp_S& is used
int draw_ramp(      outimage_S& outimage,        ramp_S& ramp,  const ramp_put_parm_S& rparm, const mapparm_S& parm) try
{
    int             rc {0}; 

    // set up letter sizes  (guessed sizes for now)

    rc_T wl         {8 }; 
    rc_T hl         {16}; 


    // set up to draw land or ocean ramp LRGB vector

    std::vector<lrgb_S> lrgb; 
    std::wstring        ramp_type;   
    int                 start_ix;           // starting index -- may want to leave out 1st (-32767) box
    int                 text_len;           // max text characters for box elevation lables 
    int                 text_offset;        // offset from top of box to top of text letters
    
    if (rparm.ocean)
    {
        lrgb        = ramp.lrgb_ocean; 
        start_ix    = 0; 
        text_len    = 6; 
        text_offset = 0; 
        ramp_type   = L"Ocean";
    }
    else
    {
        lrgb        = ramp.lrgb_land; 
        start_ix    = 0; 
        text_len    = 5;
        text_offset = 0;
        ramp_type   = L"Land";
    }

    auto n_rgb    {lrgb.size()};             // = number of LRGB entries in the ramp 


    // complain, if no LRGB entries in ramp 

    if (n_rgb < 1)
    {
        M_out_emsg(L"draw_ramp() -- %s ramp to be drawn has empty LRGB vector") % ramp_type; 
        return -1; 
    }

    if (n_rgb - start_ix < 1)
    {
        M_out_emsg(L"draw_ramp() -- %s ramp to be drawn does not have enough LRGB entries to be drawn") % ramp_type; 
        return -1; 
    }

    // compute width and height of color boxes in ramp , and then re-adjusted overall height to keep bottom margin as requested 

    rc_T top_margin = rparm.margin + std::max(0, -text_offset);                                                                   // might need extra space for 1st label
    rc_T w_box      =  rparm.width  - 2 * rparm.border - 2 * rparm.margin - rparm.text_space - text_len * wl          ;           // allow for scale labels of 5 or 6 chars
    rc_T h_box      = (rparm.height - 2 * rparm.border -     rparm.margin - top_margin)             / (n_rgb - start_ix);         // perhaps don't include 1st -32767 ramp entry   
    rc_T adj_height =                 2 * rparm.border +     rparm.margin + top_margin      + h_box * (n_rgb - start_ix);         // readjust, since   n*h_box   may be considerable less than 1/n-th of height 

    // complain if w and/or h are too small to be drawn  

    if (w_box < 36)
    {
        M_out_emsg(L"draw_ramp() -- color boxes for %s ramp are not wide enough to be drawn -- width = %d") % ramp_type % w_box; 
        return -1;     
    }

    if (h_box < 18)
    {
        M_out_emsg(L"draw_ramp() -- color boxes for %s ramp are not tall enough to be drawn -- height = %d") % ramp_type % h_box; 
        return -1;     
    }  


    // draw filled-in main backgound box with optional border
    // ------------------------------------------------------    
  
    draw_box(outimage, rparm.background_color, rparm.top_row, rparm.left_col, rparm.width, adj_height);

   if (rparm.border > 0)
       draw_frame(outimage, rparm.border_color, rparm.border, rparm.top_row, rparm.left_col, rparm.width, adj_height);
                                                                                                                  
  

    // compute location of ramp color boxes and sub-boxes
    // --------------------------------------------------

    rc_T r_box = rparm.top_row  + rparm.border + top_margin;            // row for 1st color box
    rc_T c_box = rparm.left_col + rparm.border + rparm.margin;          // column for all color boxes
    

    rgb_S box_rgb1, box_rgb2, box_rgb3, box_rgb4, box_rgb5, box_rgb6, box_rgb7; 

    rc_T w_box1 = w_box/12;                                                         // width for 1st part of box
    rc_T w_box2 = w_box/12;                                                         // width for 2nd part of box
    rc_T w_box3 = w_box/12;                                                         // width for 3nd part of box
    rc_T w_box5 = w_box/12;                                                         // width for 5th part of box
    rc_T w_box6 = w_box/12;                                                         // width for 6th part of box 
    rc_T w_box7 = w_box/12;                                                         // width for 7th part of box
    rc_T w_box4 = w_box - w_box1 - w_box2 - w_box3 - w_box5 - w_box6 - w_box7;      // width for 4rd part of box (make sure wide enough to rest of box)
    rc_T c_box1 = c_box;                                                            // starting column for sub-box
    rc_T c_box2 = c_box1 + w_box1;                                                  // starting column for sub-box
    rc_T c_box3 = c_box2 + w_box2;                                                  // starting column for sub-box
    rc_T c_box4 = c_box3 + w_box3;                                                  // starting column for sub-box
    rc_T c_box5 = c_box4 + w_box4;                                                  // starting column for sub-box
    rc_T c_box6 = c_box5 + w_box5;                                                  // starting column for sub-box
    rc_T c_box7 = c_box6 + w_box6;                                                  // starting column for sub-box
    

    real_T shade_darkest; 
    real_T shade_lightest;
    real_T shade_midtone;

    if (rparm.ocean)
    {
        shade_darkest  = ramp.parm.shade_darkest_ocean; 
        shade_lightest = ramp.parm.shade_lightest_ocean;
        shade_midtone  = ramp.parm.shade_midtone_ocean;
    }
    else
    {
        shade_darkest  = ramp.parm.shade_darkest_land; 
        shade_lightest = ramp.parm.shade_lightest_land;
        shade_midtone  = ramp.parm.shade_midtone_land;    
    }


    // draw ramp color boxes and labels
    // --------------------------------

    set_font(outimage, L"Segoe UI", 16.0); 
    set_text_color(outimage, rparm.text_color); 

    for (auto i = start_ix; i < n_rgb; i++)                                         // perhaps don't do 1st -32767 ramp entry    ????????
    {
        wchar_t buf[50] {};

        M_make_rgb( box_rgb1                
                  , (real_T)(lrgb.at(i).r) * shade_darkest
                  , (real_T)(lrgb.at(i).g) * shade_darkest
                  , (real_T)(lrgb.at(i).b) * shade_darkest
                  )
        M_make_rgb( box_rgb2                
                  , (real_T)(lrgb.at(i).r) * (0.6667 * shade_darkest + 0.3333 * shade_midtone)
                  , (real_T)(lrgb.at(i).g) * (0.6667 * shade_darkest + 0.3333 * shade_midtone)
                  , (real_T)(lrgb.at(i).b) * (0.6667 * shade_darkest + 0.3333 * shade_midtone)
                  )
        M_make_rgb( box_rgb3                
                  , (real_T)(lrgb.at(i).r) * (0.3333 * shade_darkest + 0.6667 * shade_midtone)
                  , (real_T)(lrgb.at(i).g) * (0.3333 * shade_darkest + 0.6667 * shade_midtone)
                  , (real_T)(lrgb.at(i).b) * (0.3333 * shade_darkest + 0.6667 * shade_midtone)
                  )                                                                                      
        M_make_rgb( box_rgb4                
                  , (real_T)(lrgb.at(i).r) * shade_midtone
                  , (real_T)(lrgb.at(i).g) * shade_midtone
                  , (real_T)(lrgb.at(i).b) * shade_midtone
                  )
        M_make_rgb( box_rgb5                
                  , (real_T)(lrgb.at(i).r) * (0.6667 * shade_midtone + 0.3333 * shade_lightest)
                  , (real_T)(lrgb.at(i).g) * (0.6667 * shade_midtone + 0.3333 * shade_lightest)
                  , (real_T)(lrgb.at(i).b) * (0.6667 * shade_midtone + 0.3333 * shade_lightest)
                  )                                    
        M_make_rgb( box_rgb6                
                  , (real_T)(lrgb.at(i).r) * (0.3333 * shade_midtone + 0.6667 * shade_lightest)
                  , (real_T)(lrgb.at(i).g) * (0.3333 * shade_midtone + 0.6667 * shade_lightest)
                  , (real_T)(lrgb.at(i).b) * (0.3333 * shade_midtone + 0.6667 * shade_lightest)
                  )                                                                                       
        M_make_rgb( box_rgb7                
                  , (real_T)(lrgb.at(i).r) * shade_lightest
                  , (real_T)(lrgb.at(i).g) * shade_lightest
                  , (real_T)(lrgb.at(i).b) * shade_lightest
                  )
      
        draw_box(outimage, box_rgb1, r_box, c_box1, w_box1, h_box);
        draw_box(outimage, box_rgb2, r_box, c_box2, w_box2, h_box);  
        draw_box(outimage, box_rgb3, r_box, c_box3, w_box3, h_box);  
        draw_box(outimage, box_rgb4, r_box, c_box4, w_box4, h_box);  
        draw_box(outimage, box_rgb5, r_box, c_box5, w_box5, h_box); 
        draw_box(outimage, box_rgb6, r_box, c_box6, w_box6, h_box);  
        draw_box(outimage, box_rgb7, r_box, c_box7, w_box7, h_box); 

        swprintf(buf, (sizeof buf)/(sizeof buf[0]) - 1, L"%d", (int)(lrgb.at(i).l));  
        draw_text(outimage, std::wstring{buf}, r_box + text_offset, c_box + w_box + rparm.text_space);
            
        r_box += h_box; 
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
//||       create_map_outimage() -- get outimage buffer, etc. -- dimensions from associated map 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int create_map_outimage(outimage_S& outimage, const mapdata_S& mapdata, const outimage_create_parm_S& iparm_in, const mapparm_S& parm) try
{   
    int                     rc     { 0        };
    outimage_create_parm_S  iparm  { iparm_in };  



    //  Set up row/colum limits for RGB outimage data (from associated map dimensions)
    //  ---------------------------------------------

    iparm.rows = mapdata.rows;
    iparm.cols = mapdata.cols;


    //  Set up items dependent on above settings
    //  ----------------------------------------

    create_outimage(outimage, iparm);   
    return rc;
}
M_endf  



//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""