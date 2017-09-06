// draw_window.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===============
////     draw_window.cpp
////     ===============
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

#include "h_draw_window.h"
#include "h_draw_canvas.h"
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// MACRO to check for unitialized window_C
// ---------------------------------------

#define M_window_init(fcn)                                                           \
{                                                                                    \
    /* return immediately with error, if this window_C object is not initialized */  \
    /* ------------------------------------------------------------------------- */  \
                                                                                     \
    if (!m_init)                                                                     \
    {                                                                                \
        M_out_emsg(L"window_C::" L#fcn L"() -- window_C object is not initialized"); \
        return -1;                                                                   \
    }                                                                                \
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    MACROs for on_xxxxxx functions called from WndProc() 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// MACROs to get window_C pointer    -- sets window_p to window_C pointer from hwnd -- hwnd/umsg/wparam/lparam = parms from WndProc, fname = function mame for error message
// ------------------------------

#define M_get_window_p(hwnd)                                 \
window_C *window_p;                                          \
{                                                            \
    auto gwlp_rc = GetWindowLongPtr((hwnd), GWLP_USERDATA);  \
    window_p = (window_C *)gwlp_rc;                          \
}


#define M_window_p(fname, hwnd, umsg)                                                     \
M_get_window_p((hwnd))                                                                    \
if (window_p == nullptr)                                                                  \
{                                                                                         \
    M_out_emsg(L#fname L"() -- <" L#umsg "> -- called with window_C pointer = nullptr");  \
    return 1;                                                                             \
}





// MACRO to put out initial debug message
// --------------------------------------

#define M_window_debug0(level, fname, umsg)                                                                                                           \
{                                                                                                                                                     \
    if (window_p->m_debug >= (level))                                                                                                                 \
        M_out(L#fname L"() -- <" L#umsg ">   umsg = %d") % (umsg);                                                                                    \
}


#define M_window_debug1(level, fname, umsg, parm)                                                                                                     \
{                                                                                                                                                     \
    if (window_p->m_debug >= (level))                                                                                                                 \
        M_out(L#fname L"() -- <" L#umsg ">   umsg = %d    parm = 0x%08X (%d)") % (umsg) % (parm) % (parm);                                            \
} 


#define M_window_debug2(level, fname, umsg, parm1, parm2)                                                                                             \
{                                                                                                                                                     \
    if (window_p->m_debug >= (level))                                                                                                                 \
        M_out(L#fname L"() -- <" L#umsg ">   umsg = %d    parm1 = 0x%08X (%d)   parm2 = 0x%08X (%d)")                                                 \
             % (umsg) % (parm1) % (parm1) % (parm2) % (parm2);                                                                                        \
}


#define M_window_debug3(level, fname, umsg, parm1, parm2, parm3)                                                                                      \
{                                                                                                                                                     \
    if (window_p->m_debug >= (level))                                                                                                                 \
        M_out(L#fname L"() -- <" L#umsg ">   umsg = %d    parm1 = 0x%08X (%d)   parm2 = 0x%08X (%d)   parm3 = 0x%08X (%d)")                           \
             % (umsg) % (parm1) % (parm1) % (parm2) % (parm2) % (parm3) % (parm3);                                                                    \
}


#define M_window_debug4(level, fname, umsg, parm1, parm2, parm3, parm4)                                                                               \
{                                                                                                                                                     \
    if (window_p->m_debug >= (level))                                                                                                                 \
        M_out(L#fname L"() -- <" L#umsg ">   umsg = %d    parm1 = 0x%08X (%d)   parm2 = 0x%08X (%d)   parm3 = 0x%08X (%d)   parm4 = 0x%08X (%d)")     \
             % (umsg) % (parm1) % (parm1) % (parm2) % (parm2) % (parm3) % (parm3) % (parm4) % (parm4);                                                \
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT CALLBACK WndProc(            HWND, UINT, WPARAM, LPARAM);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///                 W   W      IIIII      N   N     DDDD       OOO      W   W   
///                 W   W        I        NN  N     D   D     O   O     W   W   
///                 W W W        I        N N N     D   D     O   O     W W W   
///                 WW WW        I        N  NN     D   D     O   O     WW WW   
///                 W   W      IIIII      N   N     DDDD       OOO      W   W   
///
///
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ----------
// destructor
// ----------

M_DRAW_IMPEXP
window_C:: ~window_C()
{
     M_y(M_out(L"window_C::~window_C() called -- destructor");) 

     term(); 
     return; 
}



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       window_C::init() -- initialize window and imbeddded canvas_C object 
//||       ================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// version with passed-in class and window names
// ---------------------------------------------

M_DRAW_IMPEXP
int  window_C::init(rc_T rows, rc_T columns, const std::wstring& class_name, const std::wstring& window_name) try
{   
     m_class_name  = class_name ;      // override defaults, to allow multiple windows
     m_window_name = window_name; 

     return init(rows, columns); 
}
M_endf

////////////////////////////////////////////////////////////////////////////////////////////////////

// version where class name and window name area already saved in window_C (maybe default values)
// ----------------------------------------------------------------------------------------------

M_DRAW_IMPEXP
int  window_C::init(rc_T rows, rc_T columns) try
{
     if (m_debug >= 1)
         M_out(L"window_C::init() called"); 


    // free up old resources, if this window_C object is already initialized
    // ----------------------------------------------------------------------

    if (m_init)
        term(); 

    m_init = true;                        // indicate than initialization has started

    m_hinstance = GetModuleHandle((LPCTSTR)NULL); 
        

    // set up WNDCLASSEX and copy it into the window_C object
    // ------------------------------------------------------

    WNDCLASSEX WndClass = { sizeof (WNDCLASSEX)
                          , CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS
                          , (WNDPROC)(::WndProc) 
                          , 0 
                          , 0
                          , m_hinstance
                          , LoadIcon(0,IDI_APPLICATION)
                          , LoadCursor(0,IDC_ARROW)
                          , HBRUSH(COLOR_WINDOW+1)
                          , 0
                          , (LPCTSTR)(m_class_name.c_str())
                          , LoadIcon(0,IDI_APPLICATION)
                          };

    m_wnd_classex = WndClass;                   


    // Register the WNDCLASSEX -- return with error if this fails
    // ----------------------------------------------------------

    m_atom = ::RegisterClassEx(&(m_wnd_classex));

    if (m_atom == 0)
    {
        M_out_emsg(L"window_C::init() -- ::RegisterClassEx() failed -- atom = %d") % m_atom;
        term();
        return -1;
    }
   

    // create the window -- return imemdiately with error, if this fails
    // -----------------------------------------------------------------

    HWND hWnd = ::CreateWindowEx( WS_EX_OVERLAPPEDWINDOW 
                                , (LPCTSTR)(m_class_name.c_str()) 
                                , (LPCTSTR)(m_window_name.c_str())
                                , WS_OVERLAPPEDWINDOW  | WS_HSCROLL | WS_VSCROLL
                                , CW_USEDEFAULT
                                , CW_USEDEFAULT
                                , CW_USEDEFAULT
                                , CW_USEDEFAULT
                                , (HWND)NULL 
                                , (HMENU)NULL
                                , m_hinstance 
                                , (LPVOID)NULL
                                );         

    if (hWnd == 0)
    {
        M_out_emsg(L"window_C::init() -- ::CreateWindowEx() failed"); 
        term(); 
        return -1; 
    }  
    

    // save away handle to created window and set up pointer to window_C in GWLP_USERDATA slot for this window   (will be used for WM_PAINT handler, etc.)
    // -------------------------------------------------------------------------------------------------------

    m_hwnd = hWnd;
 
    auto swlp_rc = ::SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
    auto gwlp_rc = ::GetWindowLongPtr(m_hwnd, GWLP_USERDATA);
    
    if (m_debug >= 3)
       M_out(L"window_C::init() -- this = %p   gwlp_rc = %016x") % this % gwlp_rc;
     
    if ((LONG_PTR)this != gwlp_rc)
    {
        M_out_emsg(L"window_C::init() -- GetWindowLongPtr did not return expected value -- this = %p   gwlp_rc = %016x") % this % gwlp_rc;
        term(); 
        return -1; 
    }

     

    // initialize imbedded canvas_C object (this make sure GDI+ is started)
    // -------------------------------------

    auto gi_rc =  m_canvas.init(rows, columns, 1);    // only allow single DIBSection for internal canvas

    if (gi_rc != 0)
    {  
        M_out_emsg(L"window_C::init() -- canvas_C::init() failed with r/c = %d") % gi_rc; 
        term();                  // clean up window-C before returning 
        return -1;               // startup_gdi() should have put out any error message    
    }
    
    m_canvas_p   = &m_canvas;    // start off pointing to internal (imbedded) canvas 

    m_bmp_vsize  = rows;         // save away vsize for scrolling 
    m_bmp_hsize  = columns;      // save away hsize for scrolling

    m_hscroll    = 0;            // start off at scroll position 0
    m_vscroll    = 0;            // start off at scroll position 0

    m_mouse_xpos = 0;            // last reported mouse xpos
    m_mouse_ypos = 0;            // last reported mouse ypos 

    reset_area();                // set default xy area to whole canvas 
    reset_xy();                  // get rid of any old xy scaling  


    // create solid brush for painting off-canvas area of window
    // ---------------------------------------------------------

    m_brush_off_canvas = ::CreateSolidBrush(RGB(225, 225, 225));


    // put out final debug message and return normally
    // -----------------------------------------------

    if (m_debug >= 1)
        M_out(L"window_C::init() -- returning");

    return 0;  
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       window_C::term() -- get rid of window_C resources  
//||       ================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int window_C::term() try
{
    // return immediately, if this window_C object is not initialized
    // --------------------------------------------------------------

    if (!m_init)
       return 0; 

    if (m_debug >= 1)
        M_out(L"window_C::term() called");


    // clean up all resources for this window_C object
    // -----------------------------------------------

    m_canvas.term();                                                              // release all resources in imbedded canvas_C object

    ::DeleteObject(m_brush_off_canvas);                                           // get rid of GDI brush

    if (m_hwnd != 0)
    {
        ::DestroyWindow(m_hwnd);                                                  // get rid of window      
        m_hwnd = 0;                                                               // indicate that window is no longer created
    }

    if (m_atom != 0)
    {    
        ::UnregisterClass( (LPCTSTR)(m_class_name.c_str())                        // unregister this class -- ingnore return code 
                         , m_hinstance         
                         ); 
        m_atom = 0;                                                               // indicate that class is no longer registered
    }
    
    m_canvas_p = nullptr;                                                         // clear out attached canvas pointer
    m_init     = false;                                                           // indicate not initialized at all now

    return 0; 
}
M_endf  


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       window_C::display() -- display fields for debugging
//||       ===================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int window_C::display() try 
{
    M_window_init(display)         // return with error, if not initialized
 
    if (m_debug >= 1)
        M_out(L"window_C::display() -- called");
 

    // Display selected fields in window_C object
    // ------------------------------------------



    M_out(L"window_C::display() -- m_debug        = %s    ") %  M_bool_cstr(m_debug);  

    M_out(L"window_C::display() -- m_class_name   = \"%S\"") %  m_class_name    ; 
    M_out(L"window_C::display() -- m_window_name  = \"%S\"") %  m_window_name   ; 

    M_out(L"window_C::display() -- m_win_hsize    = %d    ") %  m_win_hsize     ;                             
    M_out(L"window_C::display() -- m_win_vsize    = %d    ") %  m_win_vsize     ;
    M_out(L"window_C::display() -- m_bmp_hsize    = %d    ") %  m_bmp_hsize     ;
    M_out(L"window_C::display() -- m_bmp_vsize    = %d    ") %  m_bmp_vsize     ;
    M_out(L"window_C::display() -- m_hscroll      = %d    ") %  m_hscroll       ;
    M_out(L"window_C::display() -- m_vscroll      = %d    ") %  m_vscroll       ;
    M_out(L"window_C::display() -- m_scroll_line  = %d    ") %  m_scroll_line   ;
    M_out(L"window_C::display() -- m_scroll_page  = %d    ") %  m_scroll_page   ;
    M_out(L"window_C::display() -- m_hscroll_max  = %d    ") %  m_hscroll_max   ;
    M_out(L"window_C::display() -- m_vscroll_max  = %d    ") %  m_vscroll_max   ;
    M_out(L"window_C::display() -- m_area_top     = %.8f  ") %  m_area_top      ;
    M_out(L"window_C::display() -- m_area_left    = %.8f  ") %  m_area_left     ;
    M_out(L"window_C::display() -- m_area_width   = %.8f  ") %  m_area_width    ;
    M_out(L"window_C::display() -- m_area_height  = %.8f  ") %  m_area_height   ; 
    M_out(L"window_C::display() -- m_org_x        = %.8f  ") %  m_org_x         ;
    M_out(L"window_C::display() -- m_org_y        = %.8f  ") %  m_org_y         ;
    M_out(L"window_C::display() -- m_scale_x      = %.8f  ") %  m_scale_x       ;
    M_out(L"window_C::display() -- m_scale_y      = %.8f  ") %  m_scale_y       ; 
    M_out(L"window_C::display() -- m_mouse_xpos   = %d    ") %  m_mouse_xpos    ;
    M_out(L"window_C::display() -- m_mouse_ypos   = %d    ") %  m_mouse_ypos    ; 
    M_out(L"window_C::display() -- m_mouse_col    = %d    ") %  m_mouse_col     ;
    M_out(L"window_C::display() -- m_mouse_row    = %d    ") %  m_mouse_row     ; 
    M_out(L"window_C::display() -- m_mouse_x      = %.8f  ") %  m_mouse_x       ;
    M_out(L"window_C::display() -- m_mouse_y      = %.8f  ") %  m_mouse_y       ;   

    return 0; 
}
M_endf  

//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       window_C::clear() -- clear out attached canvas_C bitmaps to all white
//||       =================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int window_C::clear() try 
{
    M_window_init(clear)               // return with error, if not initialized

    if (m_debug >= 1)
        M_out(L"window_C::clear() -- called");

    return m_canvas_p->gdi__clear(Gdiplus::Color { 255, 255, 255 });  
}
M_endf  





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       window_C::show() -- show passed-in window and wait for end before returning (has message loop)
//||       ================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int window_C::show() try 
{
    M_window_init(show)         // return with error, if not initialized


    if (m_debug >= 1)
        M_out(L"window_C::show() -- called");
   

    // cause window to get displayed on screen
    // ---------------------------------------  

    ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);  

    auto u_rc = ::UpdateWindow( m_hwnd); 

    if (u_rc == false)
    {
        M_out_emsg(L"window_C::show() -- ::UpdateWindow() failed -- r/c = false"); 
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
//||        window_C::attach_canvas() -- attach external canvas to this window
//||       
//||           note: window will not own the canvas_C object
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int window_C::attach_canvas(canvas_C *canvas_p) try
{
    M_window_init(attach_canvas)         // return with error, if not initialized
    

    // return if canvas does not have at least one bitmap
    // --------------------------------------------------
                                                        
    if (canvas_p->m_bmct <= 0)
    {
        M_out_emsg(L"window_C::attach_canvas() -- canvas to attached has no bitmaps -- is not initialized?");    
        return -1; 
    }
    
    m_canvas_p  = canvas_p;             // replace any internal or external canvas currently in use
    m_bmp_hsize = m_canvas_p->m_width;  // save canvas width in window_C 
    m_bmp_vsize = m_canvas_p->m_height; // save height width in window_C  

    reset_area();                       // reset xy area to cover whole attached canvas by default
    reset_xy();                         // get rid of old xy scaling  

    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        window_C::detach_canvas() -- detach external canvas from this window -- replace with internal canvas
//||       
//||           
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int window_C::detach_canvas(canvas_C *canvas_p) try
{
    M_window_init(detach_canvas)         // return with error, if not initialized
    

    // switch back to internal canvas
    // ------------------------------   

    m_canvas_p  = &m_canvas;            // replace any internal or external canvas currently in use with the internal one 
    m_bmp_hsize = m_canvas_p->m_width;  // save canvas width in window_C 
    m_bmp_vsize = m_canvas_p->m_height; // save height width in window_C 

    reset_area();                       // reset xy area to cover whole attached canvas by default
    reset_xy();                         // get rid of old xy scaling  
   
    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        window_C::set_window_text() -- set window text in title bar 
//||       
//||           
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int window_C::set_window_text(std::wstring str) try
{
    M_window_init(set_window_text)         // return with error, if not initialized

    if (m_debug >= 3)
        M_out(L"window_C::set_window_text() -- called");

    auto swt_rc = ::SetWindowText(m_hwnd, (LPCTSTR)(str.c_str()));

    if (swt_rc == 0)
    { 
        M_out_emsg(L"window_C::set_window_text() -- R/C = %d from ::SetWindowText()") % swt_rc;  
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
//||        xy-oriented functions
//||       
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int window_C::set_xy(rcf_T x0_col, rcf_T y0_row, real_T x_scale, real_T y_scale) try 
{
    M_window_init(set_xy)                // return with error, if not initialized


    // return immediately with error, x_scale or y_scale are 0.0
    // ---------------------------------------------------------
                                                            
    if ( (x_scale == 0.0) || (y_scale == 0.0) )
    {
        M_out_emsg(L"window_C::set_xy() -- X scale (%.8f) and Y scale (%.8f) cannot be 0.0") % x_scale % y_scale; 
        return -1; 
    }


    // save caller's xy values in window_C
    // -----------------------------------


    m_org_x   = x0_col; 
    m_org_y   = y0_row; 
    m_scale_x = x_scale; 
    m_scale_y = -y_scale;                          // y_scale -- positive means y increases upwards   

    return 0;
}
M_endf


//////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int window_C::set_xy_limits(xy_T x_left, xy_T x_right, xy_T y_bottom, xy_T y_top) try 
{
    M_window_init(set_xy_limits)                  // return with error, if not initialized


    // return immediately with error, top = bottom or left = right
    // -----------------------------------------------------------
                                                            
    if ( (x_left == x_right) || (y_bottom == y_top) )
    {
        M_out_emsg(L"window_C::set_xy() -- X left/right (%.8f/%.8f) and Y bottom/top (%.8f/%.8f) are not valid") % x_left % x_right % y_bottom % y_top; 
        return -1; 
    }


    // convert input values and save in window_C
    // -----------------------------------------

    m_scale_x  = m_area_width  / (x_right  - x_left);  
    m_scale_y  = m_area_height / (y_bottom - y_top );              // will usually be negative, since normally   y_top > y_bottom   

    m_org_x   = (-x_left) * m_scale_x + m_area_left;   
    m_org_y   = (-y_top ) * m_scale_y + m_area_top ;     

    return 0;
}
M_endf  

/////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int window_C::reset_xy() try 
{
    M_window_init(reset_xy)                // return with error, if not initialized

    m_org_x   = 0.0; 
    m_org_y   = 0.0; 
    m_scale_x = 1.0; 
    m_scale_y = 1.0;                      // y_scale -- negative means y increases downwards   

    return 0;
}
M_endf


////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int window_C::set_area(rcf_T left, rcf_T top, rcf_T width, rcf_T height) try 
{
    M_window_init(set_area)                     // return with error, if not initialized


    // return immediately with error, width or height is not greater than 0.0
    // ----------------------------------------------------------------------
                                                            
    if ( (width <= 0.0) || (height <= 0.0) )
    {
        M_out_emsg(L"window_C::set_xy() -- width (%.8d) and height (%.8d) are not valid -- both must be greater than 0.0") % width % height; 
        return -1; 
    }
        
    m_area_top     = top;          
    m_area_left    = left;         
    m_area_width   = width;        
    m_area_height  = height;       

    return 0;
}
M_endf


///////////////////////////////////////////////////////////////////////////////// 

M_DRAW_IMPEXP
int window_C::reset_area() try 
{
    M_window_init(reset_area)                   // return with error, if not initialized

    m_area_top     = 0.0;                       // default = top of canvas
    m_area_left    = 0.0;                       // default = left edge of canvas
    m_area_width   = (rcf_T)m_bmp_hsize;        // default = whole canvas
    m_area_height  = (rcf_T)m_bmp_vsize;        // default = whole canvas

    return 0;
}
M_endf

///////////////////////////////////////////////////////////////////////////////// 

M_DRAW_IMPEXP
int window_C::mouse_xy(int x, int y) try 
{
    M_window_init(mouse_xy)                     // return with error, if not initialized


    // compute mouse x and y positions
    // -------------------------------

    m_mouse_xpos =  x; 
    m_mouse_ypos =  y; 

    m_mouse_col  = m_mouse_xpos + m_hscroll; 
    m_mouse_row  = m_mouse_ypos + m_vscroll;

    m_mouse_x    = to_x(m_mouse_col);                                                                                                                                                        
    m_mouse_y    = to_y(m_mouse_row);


    // update mouse position info on screen
    // ------------------------------------

    set_window_text( (boost::wformat(L"Mouse position: xpos=%d  ypos=%d   col=%d  row=%d  X=%.8f  y=%.8f") % m_mouse_xpos % m_mouse_ypos % m_mouse_col % m_mouse_row % m_mouse_x % m_mouse_y).str() ); 

    return 0;
}
M_endf  


///////////////////////////////////////////////////////////////////////////////// 

M_DRAW_IMPEXP
int window_C::hscroll(rc_T new_hscroll_in) try 
{
    M_window_init(hscroll)                     // return with error, if not initialized


    // clip new scroll position to stay within scrolling limits
    // --------------------------------------------------------

    rc_T new_hscroll { M_stdlim(0, new_hscroll_in, m_hscroll_max) };   // make sure new scroll position stays within bounds
     
    M__(M_out(L"new_hscroll = %d    m_hscroll = %d") % new_hscroll % m_hscroll;) 
     
     
    // handle new scroll position (if it changed)
    // ------------------------------------------
  
    if (new_hscroll != m_hscroll)
    {
        // scroll the window
        // -----------------
  
        rc_T scroll_amount  = new_hscroll - m_hscroll;
        m_hscroll = new_hscroll;
    
        M__(M_out(L"scroll_amount = %d  m_hscroll = %d  m_vscroll = %d") % scroll_amount % m_hscroll % m_vscroll;) 
  
        ::ScrollWindowEx( m_hwnd
                        , -scroll_amount
                        , 0
                        , (CONST RECT *)nullptr
                        , (CONST RECT *)nullptr
                        , (HRGN        )nullptr
                        , (PRECT       )nullptr
                        , SW_INVALIDATE
                        );
  
        ::UpdateWindow(m_hwnd); 
  
  
        // update horizontal scroll bar position
        // -------------------------------------

        ::SetScrollPos(m_hwnd, SB_HORZ, (int)m_hscroll, TRUE);
    }         
  
    return 0;
}
M_endf  




///////////////////////////////////////////////////////////////////////////////// 

M_DRAW_IMPEXP
int window_C::vscroll(rc_T new_vscroll_in) try 
{
    M_window_init(vscroll)                     // return with error, if not initialized


    // clip new scroll position to stay within scrolling limits
    // -------------------------------------------------------- 
                                                                   
    rc_T new_vscroll { M_stdlim(0, new_vscroll_in, m_vscroll_max) };   // make sure new scroll position stays within bounds
     
    M__(M_out(L"new_vscroll = %d    m_vscroll = %d") % new_vscroll % m_vscroll;) 
  
  
    // handle new scroll position (if it changed)
    // ------------------------------------------
  
    if (new_vscroll != m_vscroll)
    {
        // scroll the window
        // -----------------
  
        rc_T scroll_amount  = new_vscroll - m_vscroll;
        m_vscroll           = new_vscroll;
    
        M__(M_out(L"scroll_amount = %d  m_vhcroll = %d  m_vscroll = %d") % scroll_amount % m_hscroll % m_vscroll;) 
  
        ::ScrollWindowEx( m_hwnd
                        , 0
                        , -scroll_amount
                        , (CONST RECT *)nullptr
                        , (CONST RECT *)nullptr
                        , (HRGN        )nullptr
                        , (PRECT       )nullptr
                        , SW_INVALIDATE
                        );  
  
        ::UpdateWindow(m_hwnd); 
  
  
        // update vertical scroll bar position
        // -----------------------------------
 
        ::SetScrollPos(m_hwnd, SB_VERT, (int)m_vscroll, TRUE);
    }   
  
    return 0;
}
M_endf  


///////////////////////////////////////////////////////////////////////////////// 

M_DRAW_IMPEXP
int window_C::hvscroll(rc_T new_hscroll_in, rc_T new_vscroll_in) try 
{
    M_window_init(hvscroll)                     // return with error, if not initialized


    // clip new scroll positions to stay within scrolling limits
    // --------------------------------------------------------

    rc_T new_hscroll { M_stdlim(0, new_hscroll_in, m_hscroll_max) };   // make sure new scroll position stays within bounds
    rc_T new_vscroll { M_stdlim(0, new_vscroll_in, m_vscroll_max) };   // make sure new scroll position stays within bounds
     
    M__(M_out(L"new_hscroll = %d    m_hscroll = %d     new_vscroll = %d    m_vscroll = %d") % new_hscroll % m_hscroll % new_vscroll % m_vscroll;) 
     
     
    // handle new scroll position (if it changed)
    // ------------------------------------------
  
    if ( (new_hscroll != m_hscroll) || (new_vscroll != m_vscroll) )
    {
        // scroll the window horizontally and/or vertically
        // ------------------------------------------------
  
        rc_T hscroll_amount  = new_hscroll - m_hscroll;
        rc_T vscroll_amount  = new_vscroll - m_vscroll;
        m_hscroll = new_hscroll;
        m_vscroll = new_vscroll;
    
        M__(M_out(L"hscroll_amount = %d   vscroll_amount = %d    m_hscroll = %d    m_vscroll = %d") % hscroll_amount % vscroll_amount % m_hscroll % m_vscroll;) 
  
        ::ScrollWindowEx( m_hwnd
                        , -hscroll_amount
                        , -vscroll_amount
                        , (CONST RECT *)nullptr
                        , (CONST RECT *)nullptr
                        , (HRGN        )nullptr
                        , (PRECT       )nullptr
                        , SW_INVALIDATE
                        );
  
        ::UpdateWindow(m_hwnd); 
  
  
        // update horizontal and vertical scroll bar position
        // --------------------------------------------------
        
        if (hscroll_amount != 0)
            ::SetScrollPos(m_hwnd, SB_HORZ, (int)m_hscroll, TRUE);

        if (vscroll_amount != 0)
            ::SetScrollPos(m_hwnd, SB_VERT, (int)m_vscroll, TRUE);  
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
//||        toxxxx() -- internal functions fo trnsform coordinates, widths, heights, etc.
//||        =======     -----------------------------------------------------------------
//||           
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

xy_T  window_C::to_x(  rcf_T col) try {return (xy_T )((col - m_org_x) / m_scale_x)              ;} M_endf
xy_T  window_C::to_y(  rcf_T row) try {return (xy_T )((row - m_org_y) / m_scale_y)              ;} M_endf
rcf_T window_C::to_row( xy_T y  ) try {return (rcf_T)((y *              m_scale_y)  + m_org_y  );} M_endf
rcf_T window_C::to_col( xy_T x  ) try {return (rcf_T)((x *              m_scale_x)  + m_org_x  );} M_endf
rcf_T window_C::to_w(   xy_T w  ) try {return (rcf_T) (w *     std::abs(m_scale_x))             ;} M_endf
rcf_T window_C::to_h(   xy_T h  ) try {return (rcf_T) (h *     std::abs(m_scale_y))             ;} M_endf


// to_points() -- transform vector of xy_S to vector of Gdiplus::PointF
// --------------------------------------------------------------------

std::vector<Gdiplus::PointF> window_C::to_points(std::vector<xy_S> xys) try
{
    std::vector<Gdiplus::PointF> points { }; 

    for (auto xy : xys)
           points.push_back(Gdiplus::PointF { ((Gdiplus::REAL)(to_col(xy.x))), ((Gdiplus::REAL)(to_row(xy.y))) });

     return points; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        draw_xxx() -- draw into attached canvas imbedded in window_C object -- uses xy coordinates rather than row/col
//||        ==========    ------------------------------------------------------------------------------------------------
//||       
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// -----------------------
// versions without alpha:    (these just call the alpha versions with alpha=255=opaque)
// -----------------------

M_DRAW_IMPEXP int window_C::draw_line(         const rgb_S& rgb, rcf_T th, xy_T x1      , xy_T y1      , xy_T x2, xy_T y2        ) try {return draw_line(         rgb, 255, th, x1      , y1      , x2, y2    );} M_endf
M_DRAW_IMPEXP int window_C::draw_box(          const rgb_S& rgb,           xy_T x_left  , xy_T y_top   , xy_T w , xy_T h         ) try {return draw_box(          rgb, 255,     x_left  , y_top   , w , h     );} M_endf
M_DRAW_IMPEXP int window_C::draw_frame(        const rgb_S& rgb, rcf_T th, xy_T x_left  , xy_T y_top   , xy_T w , xy_T h         ) try {return draw_frame(        rgb, 255, th, x_left  , y_top   , w , h     );} M_endf
M_DRAW_IMPEXP int window_C::draw_circle(       const rgb_S& rgb,           xy_T x_center, xy_T y_center,                  rcf_T r) try {return draw_circle(       rgb, 255,     x_center, y_center,         r );} M_endf
M_DRAW_IMPEXP int window_C::draw_ring(         const rgb_S& rgb, rcf_T th, xy_T x_center, xy_T y_center,                  rcf_T r) try {return draw_ring(         rgb, 255, th, x_center, y_center,         r );} M_endf

M_DRAW_IMPEXP int window_C::draw_beziers(      const rgb_S& rgb, rcf_T th, const std::vector<xy_S>& xys                          ) try {return draw_beziers(      rgb, 255, th, xys                           );} M_endf
M_DRAW_IMPEXP int window_C::draw_closed_curve( const rgb_S& rgb, rcf_T th, const std::vector<xy_S>& xys                          ) try {return draw_closed_curve( rgb, 255, th, xys                           );} M_endf
M_DRAW_IMPEXP int window_C::draw_curve(        const rgb_S& rgb, rcf_T th, const std::vector<xy_S>& xys                          ) try {return draw_curve(        rgb, 255, th, xys                           );} M_endf 
M_DRAW_IMPEXP int window_C::draw_lines(        const rgb_S& rgb, rcf_T th, const std::vector<xy_S>& xys                          ) try {return draw_lines(        rgb, 255, th, xys                           );} M_endf
M_DRAW_IMPEXP int window_C::draw_polygon(      const rgb_S& rgb, rcf_T th, const std::vector<xy_S>& xys                          ) try {return draw_polygon(      rgb, 255, th, xys                           );} M_endf
M_DRAW_IMPEXP int window_C::fill_closed_curve( const rgb_S& rgb,           const std::vector<xy_S>& xys                          ) try {return fill_closed_curve( rgb, 255,     xys                           );} M_endf
M_DRAW_IMPEXP int window_C::fill_polygon(      const rgb_S& rgb,           const std::vector<xy_S>& xys                          ) try {return fill_polygon(      rgb, 255,     xys                           );} M_endf


// --------------------
// versions with alpha:
// --------------------

M_DRAW_IMPEXP
int window_C::draw_line(const rgb_S& rgb, alpha_T alpha, rcf_T th, xy_T x1, xy_T y1, xy_T x2, xy_T y2) try 
{
    M_window_init(draw_line)
    return m_canvas_p->draw_line(rgb, alpha, th, to_row(y1), to_col(x1), to_row(y2), to_col(x2));
}
M_endf  


M_DRAW_IMPEXP
int window_C::draw_box(const rgb_S& rgb, alpha_T alpha, xy_T x_left, xy_T y_top, xy_T w, xy_T h) try
{
    M_window_init(draw_box)

    if (m_debug)
    {
        M_out(L"draw_box(alpha) -- x_left=%.8f  y_top=%.8f   w=%.8f    h=%.8f  row_top=%.8f   col_left=%.8f   width=%.8f   height=%.8f") % x_left % y_top % w % h % to_row(y_top) % to_col(x_left) % to_w(w) % to_h(h);    
    }

    return m_canvas_p->draw_box(rgb, alpha, to_row(y_top), to_col(x_left), to_w(w), to_h(h)); 
}
M_endf


M_DRAW_IMPEXP
int window_C::draw_frame(const rgb_S& rgb, alpha_T alpha, rcf_T th, xy_T x_left, xy_T y_top, xy_T w, xy_T h) try 
{
    M_window_init(draw_frame)
    return m_canvas_p->draw_frame(rgb, alpha, th, to_row(y_top), to_col(x_left), to_w(w), to_h(h));
}
M_endf
    

M_DRAW_IMPEXP
int window_C::draw_circle(const rgb_S& rgb, alpha_T alpha, xy_T x_center, xy_T y_center, rcf_T r) try 
{
    M_window_init(draw_circle)

    if (m_debug)
    {
        M_out(L"draw_circle(alpha) -- x_center=%.8f  y_center=%.8f   row=%.8f    col=%.8f   r=%.8f") % x_center % y_center % to_row(y_center) % to_col(x_center) % r;    
    }

    return m_canvas_p->draw_circle(rgb, alpha, to_row(y_center), to_col(x_center), r);
}
M_endf
 

M_DRAW_IMPEXP
int window_C::draw_ring(const rgb_S& rgb, alpha_T alpha, rcf_T th, xy_T x_center, xy_T y_center, rcf_T r) try 
{
    M_window_init(draw_ring)
    return m_canvas_p->draw_ring(rgb, alpha, th, to_row(y_center), to_col(x_center), r);
}
M_endf


M_DRAW_IMPEXP
int window_C::draw_beziers(const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<xy_S>& xys) try 
{
    M_window_init(draw_beziers)
    return m_canvas_p->draw_beziers(rgb, alpha, th, to_points(xys));
}
M_endf


M_DRAW_IMPEXP
int window_C::draw_closed_curve(const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<xy_S>& xys) try 
{
    M_window_init(draw_closed_curve)
    return m_canvas_p->draw_closed_curve(rgb, alpha, th, to_points(xys));
}
M_endf


M_DRAW_IMPEXP
int window_C::draw_curve(const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<xy_S>& xys) try 
{
    M_window_init(draw_curve)
    return m_canvas_p->draw_curve(rgb, alpha, th, to_points(xys));
}
M_endf   


M_DRAW_IMPEXP
int window_C::draw_lines(const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<xy_S>& xys) try 
{
    M_window_init(draw_lines)
    return m_canvas_p->draw_lines(rgb, alpha, th, to_points(xys));
}
M_endf


M_DRAW_IMPEXP
int window_C::draw_polygon(const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<xy_S>& xys) try 
{
    M_window_init(draw_polygon)
    return m_canvas_p->draw_polygon(rgb, alpha, th, to_points(xys));
}
M_endf


M_DRAW_IMPEXP
int window_C::fill_closed_curve(const rgb_S& rgb, alpha_T alpha, const std::vector<xy_S>& xys) try 
{
    M_window_init(fill_closed_curve)
    return m_canvas_p->fill_closed_curve(rgb, alpha, to_points(xys));
}
M_endf


M_DRAW_IMPEXP
int window_C::fill_polygon(const rgb_S& rgb, alpha_T alpha, const std::vector<xy_S>& xys) try 
{
    M_window_init(fill_polygon)
    return m_canvas_p->fill_polygon(rgb, alpha, to_points(xys));
}
M_endf   


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




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       message_loop() -- loop, handling windows messages -- waits for end of windows
//||       ==============
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int message_loop() try 
{
    // Main loop to handle windows event messages
    // ------------------------------------------
                          
    MSG msg { }; 

    while (::GetMessage(&msg, 0, 0, 0 ))
    {
       // ::TranslateMessage(&msg);
        ::DispatchMessage( &msg);
    }  

    return 0; 
}
M_endf  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_create()
//    --------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_create(HWND hwnd, LPCREATESTRUCT lCrateStruct) try
{
    M_window_p(         on_wm_create, hwnd, WM_CREATE)
    M_window_debug0( 2, on_wm_create,       WM_CREATE)

    return 0; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_destroy()
//    ---------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_destroy(HWND hwnd) try
{
    M_window_p(         on_wm_destroy, hwnd, WM_DESTROY)
    M_window_debug0( 2, on_wm_destroy,       WM_DESTROY)

    PostQuitMessage(0) ;                                     // causes all windows to end
    return 0; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_erasebkgnd()
//    ------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_erasebkgnd(HWND hwnd, HDC hdc) try
{
    M_window_p(         on_wm_erasebkgnd, hwnd, WM_ERASEBKGND)
    M_window_debug0( 2, on_wm_erasebkgnd,       WM_ERASEBKGND)
          
    return 1;                                                 // no further erasing required
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_lbuttondown()   note: handles both WM_LBUTTONDOWN and WM_LBUTTONDBLCLK
//    -------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_lbuttondown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) try
{
    M_window_p(on_wm_lbuttondown, hwnd, WM_LBUTTONDOWN) 

    if (fDoubleClick)
        M_window_debug4( 2, on_wm_lbuttondown, WM_LBUTTONDBLCLK, fDoubleClick, x, y, keyFlags)  
    else
        M_window_debug4( 2, on_wm_lbuttondown, WM_LBUTTONDOWN  , fDoubleClick, x, y, keyFlags) 

    window_p->mouse_xy(x, y);              // update saved mouse x,y position in window
    return  0;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_lbuttonup()   
//    -----------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_lbuttonup(HWND hwnd, int x, int y, UINT keyFlags) try
{
    M_window_p(         on_wm_lbuttonup, hwnd, WM_LBUTTONUP                )
    M_window_debug3( 2, on_wm_lbuttonup,       WM_LBUTTONUP, x, y, keyFlags)  

    window_p->mouse_xy(x, y);              // update saved mouse x,y position in window
    return  0;
}
M_endf  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_mbuttondown()   note: handles both WM_MBUTTONDOWN and WM_MBUTTONDBLCLK
//    -------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_mbuttondown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) try
{
    M_window_p(on_wm_mbuttondown, hwnd, WM_MBUTTONDOWN)

    if (fDoubleClick)
        M_window_debug4( 2, on_wm_mbuttondown, WM_MBUTTONDBLCLK, fDoubleClick, x, y, keyFlags)  
    else
        M_window_debug4( 2, on_wm_mbuttondown, WM_MBUTTONDOWN  , fDoubleClick, x, y, keyFlags)  

    window_p->mouse_xy(x, y);              // update saved mouse x,y position in window
    return  0;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_mbuttonup()   
//    -----------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_mbuttonup(HWND hwnd, int x, int y, UINT keyFlags) try
{
    M_window_p(         on_wm_mbuttonup, hwnd, WM_MBUTTONUP                )
    M_window_debug3( 2, on_wm_mbuttonup,       WM_MBUTTONUP, x, y, keyFlags)  

    window_p->mouse_xy(x, y);              // update saved mouse x,y position in window
    return  0;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_mousemove()
//    -----------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_mousemove(HWND hwnd, int x, int y, UINT keyFlags) try
{
    M_window_p(         on_wm_mousemove, hwnd, WM_MOUSEMOVE)
    M_window_debug3( 3, on_wm_mousemove,       WM_MOUSEMOVE, x, y, keyFlags)


    // if middle mouse button (scroll wheel) is pressed -- do scrolling
    // ----------------------------------------------------------------

    if ( (keyFlags & MK_MBUTTON) != 0 )
    {
        int32_t move_x = x - window_p->m_mouse_xpos; 
        int32_t move_y = y - window_p->m_mouse_ypos;

        window_p->hvscroll(window_p->m_hscroll - move_x, window_p->m_vscroll - move_y); 
    }

    window_p->mouse_xy(x, y);              // update saved mouse x,y position in window (after any scrolling)
    return 0; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_mousewheel()
//    ------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_mousewheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys) try
{
    M_window_p(         on_wm_mousewheel, hwnd, WM_MOUSEWHEEL)
    M_window_debug4( 2, on_wm_mousewheel,       WM_MOUSEWHEEL, xPos, yPos, zDelta, fwKeys)   

    rc_T scroll_amount { (rc_T)((real_T)(window_p->m_scroll_wheel) * ((real_T)zDelta / (real_T)WHEEL_DELTA)) };    // integer number of pixels to scroll  
                                                    
                                                                                    
    // scroll horizontally if one of the shift keys is pressed, vertically otherwise
    // -----------------------------------------------------------------------------

    if ( (fwKeys & MK_SHIFT) != 0 )      
        window_p->hscroll(window_p->m_hscroll + scroll_amount);                    // horizontally scroll window as requested                                        
    else
        window_p->vscroll(window_p->m_vscroll + scroll_amount);                    // vertically scroll window as requested                            

    window_p->mouse_xy(xPos, yPos);                                                // update saved mouse x,y position in window  (after any scrolling)

    return 0; 
}
M_endf   



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_paint()
//    -------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_paint(HWND hwnd) try
{
    M_window_p(         on_wm_paint, hwnd, WM_PAINT)
    M_window_debug0( 2, on_wm_paint,       WM_PAINT)


    // fill in PAINTSTRUCT for incoming HWND
    // -------------------------------------

    PAINTSTRUCT paintstruct { };
    HDC hdc = BeginPaint(hwnd, &paintstruct);

    M__(M_out(L"on_wm_paint() -- m_hscroll = %d  m_vscroll = %d   -- rcPaint: top=%d  bottom=%d left=%d right=%d -----------------------------------------------------------------------------------------------" ) 
        % window_p->m_hscroll % window_p->m_vscroll % paintstruct.rcPaint.top % paintstruct.rcPaint.bottom % paintstruct.rcPaint.left % paintstruct.rcPaint.right ;) 

    
    // paint requested part of window (in paintstruct.rcPaint) that is covered by the canvas bitmap
    // --------------------------------------------------------------------------------------------

    window_p->m_canvas_p->bit_blt( hdc                                                           // HDC passed in to on_paint
                                 , paintstruct.rcPaint.left                                      // left edge of dest paint rectangle
                                 , paintstruct.rcPaint.top                                       // top edge of dest paint rectangle
                                 , paintstruct.rcPaint.right  - paintstruct.rcPaint.left         // .right is one past actual right column
                                 , paintstruct.rcPaint.bottom - paintstruct.rcPaint.top          // .left  is one past actual bottom row
                                 , window_p->m_hscroll + paintstruct.rcPaint.left                // left start column in canavs_S 
                                 , window_p->m_vscroll + paintstruct.rcPaint.top                 // top start  row in canvas_S
                                 , SRCCOPY
                                 );
    

    // fill out any part of requested area that is bigger than canvas size (if window exceeds canvas in both dimensions, part of window will get painted twice) 
    // -------------------------------------------------------------------

    if (
         (paintstruct.rcPaint.right > window_p->m_bmp_hsize)
         ||
         (paintstruct.rcPaint.bottom > window_p->m_bmp_vsize)
       )
    { 
        if (paintstruct.rcPaint.right > window_p->m_bmp_hsize)      // requested area wider than canvas size? 
        {
             RECT rect {};  
       
             rect.left    = window_p->m_bmp_hsize;                 // start at right edge of canvas
             rect.right   = paintstruct.rcPaint.right;
             rect.top     = paintstruct.rcPaint.top;               // whole height
             rect.bottom  = paintstruct.rcPaint.bottom;
       
             FillRect(hdc, &rect, window_p->m_brush_off_canvas);   // paint area to the right of the canvas
        }
       
       
        if (paintstruct.rcPaint.bottom > window_p->m_bmp_vsize)    // requested area taller than canvas size? 
        {
             RECT rect {};  
       
             rect.left    = paintstruct.rcPaint.left;
             rect.right   = paintstruct.rcPaint.right;             // whole width
             rect.top     = window_p->m_bmp_vsize;                 // start at bottom of canvas
             rect.bottom  = paintstruct.rcPaint.bottom;
       
             FillRect(hdc, &rect, window_p->m_brush_off_canvas);   // paint area below the canvas
        }
    }
                                  

    // free up PAINTSTRUCT and return
    // ------------------------------
      
    EndPaint(hwnd, &paintstruct);   
    return 0; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_rbuttondown()   note: handles both WM_RBUTTONDOWN and WM_RBUTTONDBLCLK
//    -------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_rbuttondown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) try
{
    M_window_p(on_wm_rbuttondown, hwnd, WM_RBUTTONDOWN) 

    if (fDoubleClick)
        M_window_debug4( 2, on_wm_rbuttondown, WM_RBUTTONDBLCLK, fDoubleClick, x, y, keyFlags)  
    else
        M_window_debug4( 2, on_wm_rbuttondown, WM_RBUTTONDOWN  , fDoubleClick, x, y, keyFlags) 

    window_p->mouse_xy(x, y);                    // update saved mouse x,y position in window
    return  0;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_rbuttonup()   
//    -----------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_rbuttonup(HWND hwnd, int x, int y, UINT keyFlags) try
{
    M_window_p(         on_wm_rbuttonup, hwnd, WM_RBUTTONUP                )
    M_window_debug3( 2, on_wm_rbuttonup,       WM_RBUTTONUP, x, y, keyFlags)  

    window_p->mouse_xy(x, y);              // update saved mouse x,y position in window
    return  0;
}
M_endf





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_size()
//    ------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_size(HWND hwnd, UINT state, int cx, int cy) try
{
    M_window_p(         on_wm_size, hwnd, WM_SIZE)
    M_window_debug3( 3, on_wm_size,       WM_SIZE, state, cx, cy)
 
    SCROLLINFO scrollinfo {  };
 
            
    // Update scroll limit due to resize in window_C object, and make sure current scroll positions do not exceed maximum values
    // -------------------------------------------------------------------------------------------------------------------------
    
    window_p->m_win_hsize = cx; 
    window_p->m_win_vsize = cy;
    
    window_p->m_hscroll_max = std::max(window_p->m_bmp_hsize - window_p->m_win_hsize, 0);
    window_p->m_vscroll_max = std::max(window_p->m_bmp_vsize - window_p->m_win_vsize, 0);
    
    window_p->m_hscroll   = std::min(window_p->m_hscroll, window_p->m_hscroll_max); 
    window_p->m_vscroll   = std::min(window_p->m_vscroll, window_p->m_vscroll_max); 
    
    
    // Redraw scroll bars
    // ------------------
    
    scrollinfo.cbSize     = sizeof scrollinfo; 
    scrollinfo.fMask      = SIF_RANGE | SIF_PAGE | SIF_POS;  
    scrollinfo.nMin       = 0; 
    scrollinfo.nMax       = (int)(window_p->m_bmp_hsize - 1); 
    scrollinfo.nPage      = (UINT)(window_p->m_win_hsize); 
    scrollinfo.nPos       = (int)(window_p->m_hscroll); 
    
    SetScrollInfo(hwnd, SB_HORZ, &scrollinfo, TRUE);
    
    scrollinfo.cbSize     = sizeof scrollinfo; 
    scrollinfo.fMask      = SIF_RANGE | SIF_PAGE | SIF_POS;  
    scrollinfo.nMin       = 0; 
    scrollinfo.nMax       = (int)(window_p->m_bmp_vsize - 1); 
    scrollinfo.nPage      = (UINT)(window_p->m_win_vsize); 
    scrollinfo.nPos       = (int)(window_p->m_vscroll); 
    
    SetScrollInfo(hwnd, SB_VERT, &scrollinfo, TRUE);   

    return 0; 
}
M_endf  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_hscroll()
//    ---------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_hscroll(HWND hwnd, HWND lParam, UINT code, int pos) try
{
    M_window_p(         on_wm_hscroll, hwnd, WM_HSCROLL)
    M_window_debug2( 3, on_wm_hscroll,       WM_HSCROLL, code, pos)

    rc_T  new_hscroll { }; 
   

    // process specific scroll action
    // ------------------------------
     
    switch ( code )
    {
    case SB_THUMBTRACK : 
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  SB_THUMBTRACK -- %d") % pos;  
        new_hscroll = pos; 
        break;
   
    case SB_THUMBPOSITION : 
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  SB_THUMBPOSITION -- %d") % pos;
  
        new_hscroll = pos; 
        break; 
  
    case SB_PAGEUP        :
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  SB_PAGEUP");
        new_hscroll = window_p->m_hscroll - window_p->m_scroll_page;            
        break;
  
    case SB_PAGEDOWN      :
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  SB_PAGEDOWN");
        new_hscroll = window_p->m_hscroll + window_p->m_scroll_page;            
        break;
  
    case SB_LINEUP        :
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  SB_LINEUP");
        new_hscroll = window_p->m_hscroll - window_p->m_scroll_line;            
        break;
  
    case SB_LINEDOWN      :
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  SB_LINEDOWN");
        new_hscroll = window_p->m_hscroll + window_p->m_scroll_line;            
        break;  
   
    case SB_TOP           :
        if ( (window_p != nullptr) && (window_p->m_debug >= 2) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  SB_TOP");
        new_hscroll = 0;             
        break;  

    case SB_BOTTOM        :
        if ( (window_p != nullptr) && (window_p->m_debug >= 2) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  SB_BOTTOM");
        new_hscroll = window_p->m_hscroll_max;             
        break;  

    default : 
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_HSCROLL  /  (unknown=%d)") % code;
        new_hscroll = window_p->m_hscroll;                       // don't change scroll position
        break; 
    }
      

    // horizontally scroll the window to new position
    // ----------------------------------------------

    window_p->hscroll(new_hscroll);
    return 0; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    on_wm_vscroll()
//    ---------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT on_wm_vscroll(HWND hwnd, HWND lParam, UINT code, int pos) try
{
    M_window_p(         on_wm_vscroll, hwnd, WM_VSCROLL)
    M_window_debug2( 3, on_wm_vscroll,       WM_VSCROLL, code, pos)

    rc_T  new_vscroll { }; 


    // process specific scroll action
    // ------------------------------

    switch ( code )
    {
    case SB_THUMBTRACK     : 
       if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  SB_THUMBTRACK -- %d") % pos; 
        new_vscroll = pos; 
        break; 
   
    case SB_THUMBPOSITION : 
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  SB_THUMBPOSITION -- %d") % pos;
        new_vscroll = pos; 
        break; 
 
    case SB_PAGEUP        :
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  SB_PAGEUP");
        new_vscroll = window_p->m_vscroll - window_p->m_scroll_page;            
        break;
   
    case SB_PAGEDOWN      :
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  SB_PAGDOWN");
        new_vscroll = window_p->m_vscroll + window_p->m_scroll_page;            
        break;
 
    case SB_LINEUP        :
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  SB_LINEUP");
        new_vscroll = window_p->m_vscroll - window_p->m_scroll_line;            
        break;
   
    case SB_LINEDOWN      :
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  SB_LINEDOWN");
        new_vscroll = window_p->m_vscroll + window_p->m_scroll_line;            
        break;  
 
    case SB_TOP           :
        if ( (window_p != nullptr) && (window_p->m_debug >= 2) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  SB_TOP");
        new_vscroll = 0;             
        break;  

    case SB_BOTTOM        :
        if ( (window_p != nullptr) && (window_p->m_debug >= 2) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  SB_BOTTOM");
        new_vscroll = window_p->m_vscroll_max;             
        break;   

    default : 
        if ( (window_p != nullptr) && (window_p->m_debug >= 3) )
             M_out(L"WndProc() called -- event = WM_VSCROLL  /  (unknown=%d)") % code;
        new_vscroll = window_p->m_vscroll;                       // don't change scroll position
        break; 
    }
     

    // vertically scroll the window to new position
    // --------------------------------------------

    window_p->vscroll(new_vscroll);             
    return 0; 
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    Windows Procedure (not external)
//    -----------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
    // get address of window_C, if available (may be 0)
    // ------------------------------------------------

    auto gwlp_rc = GetWindowLongPtr(hwnd, GWLP_USERDATA);

    M__(M_out(L"WndProc() called umsg = %d = 0x%04X -- gwlp_rc = %016X") % umsg % umsg % gwlp_rc;)


    window_C *window_p = (window_C *)gwlp_rc; 

    if ( (window_p != nullptr) && (window_p->m_debug >= 4) )
       M_out(L"WndProc() called -- umsg=0x%08X (%d)   wparam = %0x08X (%d)   lparam=%0x08X (%d)") % umsg % umsg % wParam % wParam % lParam % lParam;
  

    // Process incoming message
    // ------------------------

    switch(umsg)
    {
        // ------------------------
        // Handle selected messages
        // ------------------------

        HANDLE_MSG(hwnd, WM_CREATE          , on_wm_create           );   break;
        HANDLE_MSG(hwnd, WM_DESTROY         , on_wm_destroy          );   break;
        HANDLE_MSG(hwnd, WM_ERASEBKGND      , on_wm_erasebkgnd       );   break;
        HANDLE_MSG(hwnd, WM_HSCROLL         , on_wm_hscroll          );   break;
        HANDLE_MSG(hwnd, WM_LBUTTONDOWN     , on_wm_lbuttondown      );   break;
        HANDLE_MSG(hwnd, WM_LBUTTONUP       , on_wm_lbuttonup        );   break;
        HANDLE_MSG(hwnd, WM_MBUTTONDOWN     , on_wm_mbuttondown      );   break;
        HANDLE_MSG(hwnd, WM_MBUTTONUP       , on_wm_mbuttonup        );   break;
        HANDLE_MSG(hwnd, WM_MOUSEMOVE       , on_wm_mousemove        );   break;
        HANDLE_MSG(hwnd, WM_MOUSEWHEEL      , on_wm_mousewheel       );   break;
        HANDLE_MSG(hwnd, WM_PAINT           , on_wm_paint            );   break;
        HANDLE_MSG(hwnd, WM_RBUTTONDOWN     , on_wm_rbuttondown      );   break;
        HANDLE_MSG(hwnd, WM_RBUTTONUP       , on_wm_rbuttonup        );   break;
        HANDLE_MSG(hwnd, WM_SIZE            , on_wm_size             );   break;
        HANDLE_MSG(hwnd, WM_VSCROLL         , on_wm_vscroll          );   break;
   

        // ---------------------------------------------------------------------------------------------
        // All other messages - just pass through to DefWindowProc()
        // ---------------------------------------------------------------------------------------------

        default:             
            break;

    }

    return DefWindowProc(hwnd, umsg, wParam, lParam);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        set_sys_color() -- set one system color 
//||       
//||           
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int set_sys_color(int32_t id, const rgb_S& rgb) try
{
    INT      elements    [1] {(INT)id                 };
    COLORREF rgb_values  [1] {RGB(rgb.r, rgb.g, rgb.b)};

    //M_out(L"calling SSC()");
    auto ssc_rc = ::SetSysColors(1, elements, rgb_values);

    if (ssc_rc == 0)
    { 
        M_out_emsg(L"window_C::set_sys_color() -- R/C = %d from ::SetSysColors()") % ssc_rc;  
        return -1;
    }

    return 0; 
}
M_endf     


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""