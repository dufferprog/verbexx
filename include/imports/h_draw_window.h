// h_draw_window.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////      
////     ===============
////     h_draw_window.h
////     ===============
//// 
//// 
////    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
#pragma once
#include "h_draw_types.h"
#include "h_draw_canvas.h"



//   xy_S -- structure with x,y point coordinates -- analagous to Gdiplus::PointF
//   ----------------------------------------------------------------------------
          
struct xy_S
{
   xy_T x; 
   xy_T y;
};    



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   --------------
//   window_C class
//   --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

class window_C
{
public:
    int32_t              m_debug                     { 0       };                  // 0 = no debug messages, 1 = main debug messages, 2 = more debug messages, 3 = all debug messages     
    bool                 m_init                      { false   };                  // true = window has been initialized (at least partly) 
    std::wstring         m_class_name                {L"c"     };                  // default class name for this window
    std::wstring         m_window_name               {L"w"     };                  // default window name for this window

    HINSTANCE            m_hinstance                 {         };                  // captured HINSTANCE when window was created
    HWND                 m_hwnd                      {  0      };                  // handle to created window
    ATOM                 m_atom                      {  0      };                  // ATOM returned from RegisterClassEx() 
    WNDCLASSEX           m_wnd_classex               {         };                  // saved WNDCLASSEX structure from create_window()

    canvas_C             m_canvas                    {         };                  // imbedded graphics object -- default constructed for now -- needs init() before use
    canvas_C            *m_canvas_p                  { nullptr };                  // pointer to internal or external canvas attached to this window   

    rc_T                 m_win_hsize                 {  0      };                  // horizontal window size
    rc_T                 m_win_vsize                 {  0      };                  // vertical   window size

    rc_T                 m_bmp_hsize                 {  0      };                  // horizontal graphics bmp size  (current canvas)
    rc_T                 m_bmp_vsize                 {  0      };                  // vertical   graphics bmp size  (current canvas)

    rc_T                 m_hscroll                   {  0      };                  // current horizontal scroll position
    rc_T                 m_vscroll                   {  0      };                  // current vertical   scroll position
    rc_T                 m_hscroll_max               {  0      };                  // maximum horizontal scroll position
    rc_T                 m_vscroll_max               {  0      };                  // maximum vertical   scroll position 
    rc_T                 m_scroll_line               {  25     };                  // amount to scroll when scrolling arrow is clicked
    rc_T                 m_scroll_wheel              {  75     };                  // amount to scroll with eack step of the scrolling wheel
    rc_T                 m_scroll_page               {  250    };                  // amount to scroll mouse is clicked on shaft above/below scroll button

    rcf_T                m_area_top                  {  0.0    };                  // top row     of area (default = top of canvas)
    rcf_T                m_area_left                 {  0.0    };                  // left column of area (default = left edge of canvas)
    rcf_T                m_area_width                {  0.0    };                  // width       of area (default = whole canvas -- m_bmp_hsize)   
    rcf_T                m_area_height               {  0.0    };                  // height      of area (default = whole canvas -- m_bmp_vsize)  

    rcf_T                m_org_x                     {  0.0    };                  // origin col for X coordinates (within current area) 
    rcf_T                m_org_y                     {  0.0    };                  // origin row for Y coordinates (within current area) 
    real_T               m_scale_x                   {  1.0    };                  // scale for X coordinates -- pixels per unit  
    real_T               m_scale_y                   {  1.0    };                  // scale for X coordinates -- pixels per unit (negative number means Y increases upwards on screen) 

    int32_t              m_mouse_xpos                {  0      };                  // last reported mouse xpos
    int32_t              m_mouse_ypos                {  0      };                  // last reported mouse ypos   
    rc_T                 m_mouse_col                 {  0      };                  // last reported mouse xpos converted to canvas column
    rc_T                 m_mouse_row                 {  0      };                  // last reported mouse ypos converted to canvas row
    xy_T                 m_mouse_x                   {  0      };                  // last reported mouse xpos converted to canvas X
    xy_T                 m_mouse_y                   {  0      };                  // last reported mouse ypos converted to canvas Y

    HBRUSH               m_brush_off_canvas          {         };                  // brush used for painting off-canvas areas in window   

    ////////////////////////////////////////////////////////////////////////////////////////////////

                         window_C()                         = default;                                // default constructor -- need to use init() below before using window
                         window_C(const window_C&)          = delete;                                 // copy constructor
                         window_C(window_C&&)               = delete;                                 // move constructor
    M_DRAW_IMPEXP       ~window_C();                                                                  // destructor
    window_C&            operator=(const window_C&)         = delete;                                 // copy assignment
    window_C&            operator=(window_C&&)              = delete;                                 // move assignment  

    // ------------------------------------------------------------------------------------------------

    M_DRAW_IMPEXP int    init(rc_T, rc_T                                          );                  // initialize window -- width and height is for imbedded canvas_C object
    M_DRAW_IMPEXP int    init(rc_T, rc_T, const std::wstring&, const std::wstring&);                  // initialize window -- width and height is for imbedded canvas_C object -- strings are windows class and window name
    M_DRAW_IMPEXP int    term();                                                                      // terminate  window -- free up everything 
    M_DRAW_IMPEXP int    clear();                                                                     // clear out canvas_C for this window_C object to all white
    M_DRAW_IMPEXP int    show();                                                                      // show       window -- cause window to be displayed on screen  
    M_DRAW_IMPEXP int    display();                                                                   // display window_C fields for debug

    M_DRAW_IMPEXP int    attach_canvas( canvas_C * );                                                 // attach external canvas to this window
    M_DRAW_IMPEXP int    detach_canvas( canvas_C * );                                                 // detach external canvas from this window (switch back to internal canvas)

    // -------------------------------------------------------------------------------------------------

    M_DRAW_IMPEXP int    set_window_text(const std::wstring);                                         // set window text in title bar of main window

    M_DRAW_IMPEXP int    set_xy(       rcf_T, rcf_T, xy_T , xy_T );                                   // pass in origin and scale factors (positive Y scale factor = Y increases upwards)
    M_DRAW_IMPEXP int    set_xy_limits(xy_T , xy_T , xy_T , xy_T );                                   // pass in top/bottom, left/right edges in xy units
    M_DRAW_IMPEXP int    reset_xy(                               );                                   // reset x,y scale to defailt values
    M_DRAW_IMPEXP int    set_area(     rcf_T, rcf_T, rcf_T, rcf_T);                                   // set area  (can exceed canvas boundaries)
    M_DRAW_IMPEXP int    reset_area(                             );                                   // reset area back to default value (whole canvas)
    M_DRAW_IMPEXP int    mouse_xy(     int, int                  );                                   // save away mouse position 
    M_DRAW_IMPEXP int    hscroll(      rc_T                      );                                   // do horizontal scrolling 
    M_DRAW_IMPEXP int    vscroll(      rc_T                      );                                   // do vertical scrolling 
    M_DRAW_IMPEXP int    hvscroll(     rc_T, rc_T                );                                   // do horizontal and vertical scrolling 
                                                                                                   
private:                                                                                           
    rcf_T                to_row(xy_T);                                                                // convert input y to output row
    rcf_T                to_col(xy_T);                                                                // convert input x to output column
    rcf_T                to_w(  xy_T);                                                                // convert input width  in xy units to width in columns
    rcf_T                to_h(  xy_T);                                                                // convert input height in xy units to height in rows
    xy_T                 to_x(rcf_T);                                                                 // convert input column to output x value 
    xy_T                 to_y(rcf_T);                                                                 // convert input row    to output y value 

    std::vector<Gdiplus::PointF> window_C::to_points(std::vector<xy_S> xys);                          // transform vector of xy_S to vector of Gdiplus::PointF 

public:
    M_DRAW_IMPEXP int    draw_line(          const rgb_S&               , rcf_T, xy_T, xy_T, xy_T, xy_T          );
    M_DRAW_IMPEXP int    draw_line(          const rgb_S&, alpha_T      , rcf_T, xy_T, xy_T, xy_T, xy_T          );

    M_DRAW_IMPEXP int    draw_beziers(       const rgb_S&               , rcf_T, const std::vector<xy_S>&        );
    M_DRAW_IMPEXP int    draw_beziers(       const rgb_S&, alpha_T      , rcf_T, const std::vector<xy_S>&        );   
    M_DRAW_IMPEXP int    draw_closed_curve(  const rgb_S&               , rcf_T, const std::vector<xy_S>&        );
    M_DRAW_IMPEXP int    draw_closed_curve(  const rgb_S&, alpha_T      , rcf_T, const std::vector<xy_S>&        );   
    M_DRAW_IMPEXP int    draw_curve(         const rgb_S&               , rcf_T, const std::vector<xy_S>&        );
    M_DRAW_IMPEXP int    draw_curve(         const rgb_S&, alpha_T      , rcf_T, const std::vector<xy_S>&        );  
    M_DRAW_IMPEXP int    draw_lines(         const rgb_S&               , rcf_T, const std::vector<xy_S>&        );
    M_DRAW_IMPEXP int    draw_lines(         const rgb_S&, alpha_T      , rcf_T, const std::vector<xy_S>&        ); 
    M_DRAW_IMPEXP int    draw_polygon(       const rgb_S&               , rcf_T, const std::vector<xy_S>&        );
    M_DRAW_IMPEXP int    draw_polygon(       const rgb_S&, alpha_T      , rcf_T, const std::vector<xy_S>&        ); 
    M_DRAW_IMPEXP int    fill_closed_curve(  const rgb_S&               ,        const std::vector<xy_S>&        );
    M_DRAW_IMPEXP int    fill_closed_curve(  const rgb_S&, alpha_T      ,        const std::vector<xy_S>&        );   
    M_DRAW_IMPEXP int    fill_polygon(       const rgb_S&               ,        const std::vector<xy_S>&        );
    M_DRAW_IMPEXP int    fill_polygon(       const rgb_S&, alpha_T      ,        const std::vector<xy_S>&        ); 


    M_DRAW_IMPEXP int    draw_box(           const rgb_S&               ,        xy_T, xy_T, xy_T, xy_T          );
    M_DRAW_IMPEXP int    draw_box(           const rgb_S&, alpha_T      ,        xy_T, xy_T, xy_T, xy_T          );
    M_DRAW_IMPEXP int    draw_frame(         const rgb_S&               , rcf_T, xy_T, xy_T, xy_T, xy_T          );
    M_DRAW_IMPEXP int    draw_frame(         const rgb_S&, alpha_T      , rcf_T, xy_T, xy_T, xy_T, xy_T          );
    M_DRAW_IMPEXP int    draw_circle(        const rgb_S&               ,        xy_T, xy_T,            rcf_T    );
    M_DRAW_IMPEXP int    draw_circle(        const rgb_S&, alpha_T      ,        xy_T, xy_T,            rcf_T    ); 
    M_DRAW_IMPEXP int    draw_ring(          const rgb_S&               , rcf_T, xy_T, xy_T,            rcf_T    );                                                                       
    M_DRAW_IMPEXP int    draw_ring(          const rgb_S&, alpha_T      , rcf_T, xy_T, xy_T,            rcf_T    ); 
};                                                                                                   



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// standalone external functions -- not part of window_C::

M_DRAW_IMPEXP int      message_loop(); 
M_DRAW_IMPEXP int      set_sys_color(int32_t, const rgb_S&);  


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""