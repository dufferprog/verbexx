// draw_canvas.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===============
////     draw_canvas.cpp -- GDI+ multi-bmp and BMP file-oriented functions
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

#include "h_draw_types.h"
#include "h_draw_canvas.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// internal MACRO to optionally force default transform before drawing things 
/// -------------------------------------------------------------------------- 

#define DEFAULT_TRANSFORM(t)                                \
{                                                           \
    if (m_default_transform == true)                        \
    {                                                       \
       int rc = default_transform();                        \
       M_checkrc_ret(rc, t L": default_transform()")        \
    }                                                       \
}





/// internal MACRO to apply operation to all active Gdiplus::Graphics objects 
/// ------------------------------------------------------------------------- 
///
/// lfcn = local function name       (ex: reset_transform   )
/// gdif = GDI+ function name        (ex: ResetTransform    )
/// gdip = parms for GDI+ function   (ex: (x,y,x2,y2)       )


// version with status: 

#define APPLY_OP(lfcn, gdif, gdip)                                                \
{                                                                                 \
    for (auto i = 0; i < m_bmct; i++)                                             \
    {                                                                             \
        sts = m_gr_p.at(i)->gdif##gdip;                                           \
        M_checks_ret(sts, L#lfcn  L"() : after gs.m_gr_p[i]->"  L#gdif L#gdip)    \
    }                                                                             \
}

// version without status: 

#define APPLY_OP_N(lfcn, gdif, gdip)                                              \
{                                                                                 \
    for (auto i = 0; i < m_bmct; i++)                                             \
        m_gr_p.at(i)->gdif##gdip;                                                 \
}
 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





// --------------------------------------------------------------------------------------------------------------------
//      static variables
// --------------------------------------------------------------------------------------------------------------------

namespace static_N
{
 static        bool                          is_gdi_started        { false             };    // true if GDI+ is started   
 static        ULONG_PTR                     gdi_tok               { (ULONG_PTR)nullptr};    // GDI+ token returned by GdiplusStartup
}  


 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       startup_gdi() -- start up GDI+ (if not already started) 
//||       =============
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""  

M_DRAW_IMPEXP
int startup_gdi() try
{
    M__(M_out(L"startup_gdi() called -- is_gdi_started=%d") % (int)static_N::is_gdi_started;)

    if (static_N::is_gdi_started == false)
    {
        Gdiplus::GdiplusStartupInput gdi_start_input;                                                      // use all defaults

        Gdiplus::Status sts = Gdiplus::GdiplusStartup(&(static_N::gdi_tok), &gdi_start_input, NULL);       // start up GDI+ with defaults

        if ((int)sts != 0)
        {
            M_out_emsg(L"startup_gdi(): GdiplusStartup() failed -- status = %d") % sts;
            return -1; 
        }

        atexit(       shutdown_gdi);   // make sure GDI+ is shut down before (normal) exit
        at_quick_exit(shutdown_gdi);   // make sure GDI+ is shut down before (quick)  exit 

        static_N::is_gdi_started = true;  
        M_out(L"startup_gdi(): GDI+ started up -- GdiplusStartup() status = %d") % sts;  
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
//||       shutdown_gdi() -- shut down GDI+ (if started up)  -- this can be atexit() routine
//||       ==============
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
void shutdown_gdi() try
{
    M__(M_out(L"shutdown_gdi() called -- is_gdi_started=%d") % (int)static_N::is_gdi_started;)

    if (static_N::is_gdi_started)
    {
        Gdiplus::GdiplusShutdown(static_N::gdi_tok);

        M__(M_out(L"shutdown_gdi(): GDI+ is now shut down");)

        static_N::is_gdi_started = false; 
        static_N::gdi_tok        = (ULONG_PTR)nullptr; 
    }

    return; 
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




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///                    CCCC       A       N   N     V   V        A        SSSS
///                   C          A A      NN  N     V   V       A A      S    
///                   C         A   A     N N N      V V       A   A      SSS 
///                   C         AAAAA     N  NN      V V       AAAAA         S
///                    CCCC     A   A     N   N       V        A   A     SSSS 
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
canvas_C:: ~canvas_C()
{
     if (m_debug > 0)
         M_out(L"canvas_C::~canvas_C() called -- destructor"); 

     term(); 
     return; 
}



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::init() -- get GDI+ resources and anchor them in canvas_C area 
//||       ================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

#define MAX_BMP_DIM    30000                      // allow max dimension of 30000 before splitting
#define MAX_BMP_PIXELS ((uint64_t)(M_1M * 650))   // allow max of 650M points in bitmap before splitting (max size for some viewers)  -- was 500M


M_DRAW_IMPEXP
int  canvas_C::init(rc_T rows, rc_T columns, int32_t max_bmp) try
{
     if (m_debug > 0)
         M_out(L"canvas_C::init() called"); 


    // free up old resources, if this canvas_C object is already initialized (or partly initialized)
    // ---------------------------------------------------------------------

    if (m_init)
        term(); 

    m_init = true;     // initialization has been started


    // ========================================================================================================================
    // compute partitioning of whole bitmap into horizontal and vertical tiles/slices -- equal sizes (but may overlap slightly)
    // ========================================================================================================================
    
    uint64_t pixels { (uint64_t)rows * (uint64_t)columns };   

    rc_T     n_rows   { };  
    rc_T     n_cols   { };  
 
    if (rows > columns)                                                                             // canvas is tall?
    {
        // split up canvas height into minimum number of rows of BMPs that will cover the canvas height 

        n_rows = M_divide_roundup(rows, MAX_BMP_DIM);                                               // compute minimum number of rows that must be used to cover canvas hight    
        rc_T max_cols_bmp = M_divide_rounddown(MAX_BMP_PIXELS, M_divide_roundup(rows, n_rows));     // most number of columns per bitmap that keeps BMP pixel count less tham max
        n_cols = M_divide_roundup(columns, max_cols_bmp);                                           // use fewest columns of BMPs that don't exceed maximum pixel count each   
    }
    else                                                                                            // canvas is wide
    {
        // split up canvas width into minimum number of columns of BMPs that will cover the canvas width 

        n_cols = M_divide_roundup(columns, MAX_BMP_DIM);                                            // compute minimum number of cols that must be used to cover canvas width    
        rc_T max_rows_bmp = M_divide_rounddown(MAX_BMP_PIXELS, M_divide_roundup(columns, n_cols));  // most number of columns per bitmap that keeps BMP pixel count less tham max
        n_rows = M_divide_roundup(rows, max_rows_bmp);                                              // use fewest columns of BMPs that don't exceed maximum pixel count each   
    }                                        


    // compute bmpos_S values for each BMP in tile array, and add them to bmpos_S vector
    // ---------------------------------------------------------------------------------  

    rc_T     rows_bmp { M_divide_roundup(rows     , n_rows) };    // number of pixel rows    in each BMP     (all BMPS are same size)
    rc_T     cols_bmp { M_divide_roundup(columns  , n_cols) };    // number of pixel columns in each BMP   

    for (auto i = 0; i < n_rows; i++)
    {
        for (auto j = 0; j < n_cols; j++)
        {
            rc_T  row0 { std::min(i * rows_bmp, rows    - rows_bmp) };     // don't let last row    of BMPs extend below last row    of pixels -- last row    of BMPs may overlap 2nd-to-last row 
            rc_T  col0 { std::min(j * cols_bmp, columns - cols_bmp) };     // don't let last column of BMPs extend past  last column of pixels -- last column of BMPs may overlap 2nd-to-last column 

            m_bmpos.push_back(bmpos_S {col0, row0, rows_bmp, cols_bmp}); 
        } 
    }

  
    // save away dimensions, etc.
    // ------------------------- 

    m_bmct            = n_rows * n_cols;                       // number of DCs/BMPs in this canvas
    m_actual_cols_bmp = M_roundup(cols_bmp, 4);                // pixel rows in each BMP need to be even 32-bit length  
    m_width           = columns; 
    m_height          = rows; 


    // debug printout
    // --------------

    if (m_debug > 0)
        M_out(L"canvas_C::init() -- graphics sizes     : rows=%d columns=%d   bmct=%d  rows_bmp=%d cols_bmp=%d actual_cols_bmp=%d   BMPsize=%u (max=%u)") 
              % m_height % m_width % m_bmct % rows_bmp % cols_bmp % m_actual_cols_bmp % (3ULL * (uint64_t)rows_bmp * (uint64_t)(m_actual_cols_bmp)) % (3ULL * MAX_BMP_PIXELS) ; 
        
    if (m_debug > 1)
    {
        for (auto i = 0; i < m_bmct; i++)
        {
           M_y(M_out(L"canvas_C::init() -- position of bit map %d : x=%d   y=%d   rows=%d   cols=%d") % i % m_bmpos.at(i).x %  m_bmpos.at(i).y % m_bmpos.at(i).rows % m_bmpos.at(i).cols;)      
        }
    }

    // error if too many bitmaps are required to cover the requested canvas size
    // -------------------------------------------------------------------------
    
    if (m_bmct > max_bmp)
    {
        M_out_emsg(L"canvas_C::init() -- too many (%d) bitmaps required -- max allowed = %d") % m_bmct % max_bmp; 
        return -1;
    }
    

    // Make sure GDI+ is started up -- return immediately, if error 
    // ------------------------------------------------------------     

    auto gdi_rc = ::startup_gdi(); 
    if (gdi_rc != 0)
    {
        M_out_emsg(L"canvas_C::init() -- ::startup_gdi() failed with R/C = %d") % gdi_rc; 
        return -1; 
    }
    

    // create GDI memory DCs
    // ---------------------

    for (auto i = 0; i < m_bmct; i++)
    {
        auto hdc = ::CreateCompatibleDC(NULL);
      
        if (hdc == NULL)
        {
            M_out_emsg(L"canvas_C::init(): i=%d     CreateCompatibleDC(NULL) failed") % i;
            return -1;
        }

        m_hdc.push_back(hdc);         // save handle to this memory DC in vector

        if (m_debug > 1)
            M_out("canvas_C::init() -- m_hdc[%d] = %X") % i % m_hdc.at(i);
    }


    // create GDI DIB sections (all are of equal size, but may overlap -- header is same for all bitmaps))
    // -----------------------
    
    m_bmi.bmiHeader.biSize             = sizeof (BITMAPINFOHEADER); // no color table present
    m_bmi.bmiHeader.biWidth            = M_roundup(cols_bmp, 4);    // round up to multiple of 4 
    m_bmi.bmiHeader.biHeight           = -rows_bmp; // top-down bitmap
    m_bmi.bmiHeader.biPlanes           = 1;
    m_bmi.bmiHeader.biBitCount         = 24;        // RGB values in array -- no color table 
    m_bmi.bmiHeader.biCompression      = BI_RGB;    // no compression
    m_bmi.bmiHeader.biSizeImage        = 0;         // not used for BI_RGB (no compression)
    m_bmi.bmiHeader.biXPelsPerMeter    = 25000;     // 25 pixels per mm -- not used by CreateDIBSection()
    m_bmi.bmiHeader.biYPelsPerMeter    = 25000;     // 25 pixels per mm -- not used by CreateDIBSection()
    m_bmi.bmiHeader.biClrUsed          = 0;         // No color table
    m_bmi.bmiHeader.biClrImportant     = 0;         // No color table
    
    if (m_debug > 1)
       M_out(L"canvas_C::init():  biWidth = %d   biHeight = %d ") % m_bmi.bmiHeader.biWidth % m_bmi.bmiHeader.biHeight;


    for (auto i = 0; i < m_bmct; i++)
    {
        void *pvBits { };                              // output from CreateDIBSection()

        auto hbitmap = ::CreateDIBSection( m_hdc.at(i)
                                         , &m_bmi               
                                         , DIB_RGB_COLORS     // no palette
                                         , &(pvBits)          // filled in local ptr to pixel bits in bitmap
                                         , NULL               // no hSection
                                         , 0                  // ignored 
                                         );

        if (hbitmap == nullptr)
        {
            M_out_emsg(L"canvas_C::init(): i=%d  CreateDIBsection() failed") % i;
            return -1; 
        }  

        m_hbitmap.push_back(hbitmap);                  // add hbitmap to vector of these
        m_pvBits.push_back(pvBits);                    // add pvBits to vector of these


        if (m_debug > 1)
        {
            M_out("canvas_C::init() -- m_hbitmap[%d]  = %X") % i % m_hbitmap.at(i);
            M_out("canvas_C::init() -- m_pvBits[%d]   = %X") % i % m_pvBits.at(i) ;
        }
    }


    // select DIB sections into the memory DCs -- save away original selection for later delete
    // ---------------------------------------
     
    for (auto i = 0; i < m_bmct; i++)
    {
        auto orig = ::SelectObject(m_hdc.at(i), m_hbitmap.at(i));

        if (orig == nullptr )
        {
           M_out_emsg(L"canvas_C::init(): i=%d   ::SelectObject(m_hdc[%d], m_hbitmap[%d]) failed") % i % i % i;
           return -1; 
        } 

        m_hgdiobj_saved.push_back(orig);


        if (m_debug > 1)
            M_out(L"canvas_C::init() -- hgdiobj_saved[%d] = %X") % i % m_hgdiobj_saved.back();
    }
    

    //  create GDI+ graphics objects for GDI DCs just created
    //  -----------------------------------------------------
   
    for (auto i = 0; i < m_bmct; i++)
    {
        auto p = new Gdiplus::Graphics(m_hdc.at(i));

        if (m_debug > 1)
            M_out(L"canvas_C::init() -- i=%d   p=%p") % i % p;

        if (p == nullptr)
        {
            M_out_emsg(L"canvas_C::init(): operator new did not create GDI+ graphics object %d") % i;
            return -1; 
        }

        M_checklsp_ret(p, L"canvas_C::init() : After n-th Graphics constructor")
        m_gr_p.push_back(p);
    }


    // Set up default transformation for all valid DCs  
    // -----------------------------------------------     
    
    default_transform();
    m_default_transform = false;           // don't always go back to default transform when drawing things other than text 
  

    // Set up default brush for drawing text (background brush is not allocated here)
    // --------------------------------------

    m_text_br_p  = new Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 0));       


    // Set up default string format -- set flags so that MeasureString() will include trailing blanks
    // ----------------------------------------------------------------------------------------------

    m_sf_p = new Gdiplus::StringFormat(Gdiplus::StringFormat::GenericTypographic()); 
    
    auto sf_rc = m_sf_p->SetFormatFlags(Gdiplus::StringFormatFlagsMeasureTrailingSpaces); 
    M_checks_ret(sf_rc, L"canvas_C::init() -- Gdiplus::SetFormatFlags()")



    // Set up current font for drawing text  -- generic serif font (exit with bad R/C, if error)
    // -----------------------------------------------------------------------------------------
    //
    //  Have to go through operator new here, since imbedded text_format_S causes the imbedded Gdiplus:Matrix.reset() operation to fail with R/C = 2
    //

    m_curr_font_p = new Gdiplus::Font(Gdiplus::FontFamily::GenericSerif(), 16.0, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    auto i_rc = ::init_font(m_font, *(m_curr_font_p), true); 

    if (i_rc != 0)
    {
        delete m_curr_font_p; 
        m_curr_font_p = nullptr; 
        return -1; 
    }


    // set up default text format -- x, y, rotation -- will be set individually for each draw_text() invocation 
    // --------------------------------------------------------------------------------------------------------

    m_tf_p = new text_format_S(); 
    ::init_text_format(*m_tf_p, m_text_br_p, nullptr, m_sf_p);   // use default settings at start


    // set up empty private font collection
    // ------------------------------------

    m_pfc_p = new Gdiplus::PrivateFontCollection(); 


    m_usable = true;           // mark this canvas as usable (fully initialized, but not yet cleared)


    // Clear the DIBSections with a default color (white) background
    // ------------------------------------------------------------- 

    auto c_rc = clear();

    if (c_rc != 0)
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
//||       canvas_C::clear() -- reset GDI+ DIBSections to a color   
//||       =================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ============================================
// no color specified -- clear to default color  (white)
// ============================================

M_DRAW_IMPEXP
int canvas_C::clear() try
{
    return clear(rgb_S { 255, 255, 255 });
}
M_endf
 

// ===================================
// version with caller-specified color
// ===================================

M_DRAW_IMPEXP
int canvas_C::clear(const rgb_S rgb ) try
{
    if (m_debug > 0)
        M_out(L"canvas_C::clear() called"); 


    // Return immediately, if this canvas_C object is not initialized
    // ----------------------------------------------------------------

    if (!m_usable)
    {
       M_out_emsg(L"canvas_C::clear() called on unusable canvas");
       return -1; 
    }


    // Do GDI+ clear function with passed-in color
    // -------------------------------------------
                    
    auto rc = gdi__clear(Gdiplus::Color(rgb.r, rgb.g, rgb.b) );
    return (int)rc; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::flush() -- flush GDI+ for this canvas_C object  
//||       =================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::flush() try
{
    if (m_debug > 0)
        M_out(L"canvas_C::flush() called"); 


    // Return immediately, if this canvas_C object is not initialized
    // ----------------------------------------------------------------

    if (!m_usable)
    {
       M_out_emsg(L"canvas_C::flush() called on unusable canvas");
       return -1; 
    }


    // Do GDI+ flush function 
    // ----------------------

    auto rc = gdi__flush(Gdiplus::FlushIntentionSync);
    return (int)rc; 
}
M_endf    


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::bit_blt() -- do BitBlt from cnvas_C DCs to caller's passed-in DC  
//||       ===================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::bit_blt(HDC dest_dc, int dest_x, int dest_y, int dest_w, int dest_h, int src_x, int src_y, DWORD rop) try
{
    if (m_debug > 0)
        M_out(L"canvas_C::bit_blt() called -- dest_x=%d  dest_y=%d  dest_w=%d  dest_h=%d  src_x=%d  src_y=%d") %  dest_x % dest_y % dest_w % dest_h % src_x % src_y; 


    // Return immediately, if this canvas_C object is not initialized
    // ----------------------------------------------------------------

    if (!m_usable)
    {
       M_out_emsg(L"canvas_C::bit_blt() called on unusable canvas");
       return -1; 
    }


    // Do GDI BitBlt function from each DC in canvas_C to caller's DC
    // --------------------------------------------------------------
    //
    //                                            
    //       +----------------------------------------------------------------------------------------------------------------------------------+
    //       | Canvas_C                                                      A                      A                                           |
    //       |                                                               | (scroll_y)           | src_y                                     |
    //       |                                     Caller's DC (window)      V                      |                                           |
    //       |     (scroll_x)        +--------------------------------------------------------------|-------------------------+                 |
    //       |<--------------------->|         dest_dc (caller's window)          A                 |                         |                 |
    //       |                       |                                            |  dest_y         |                         |                 |
    //       |                       |                                            V                 V                         |                 |
    //       |                       |              +-------------------------------------------------------------+           |                 |
    //       |                       |              |  caller's paint rect                A                       |           |                 |
    //       |                       |   dest_x     |                                     |                       |           |                 |
    //       |    src_x              |<------------>|                                     | dest_h                |           |                 |
    //       |<------------------------------------>|            dest_w                   |                       |           |                 |
    //       |                       |              |<------------------------------------|---------------------->|           |                 |
    //       |                       |              |                                     |                       |           |                 |
    //       |                       |              |                                     V                       |           |                 |
    //       |                       |              +-------------------------------------------------------------+           |                 |
    //       |                       |                                                                                        |                 |
    //       |                       +----------------------------------------------------------------------------------------+                 |
    //       |                                                                                                                                  |
    //       |                                                                                                                                  |
    //       |                                                                                                                                  |
    //       |                                                                                                                                  |
    //       +----------------------------------------------------------------------------------------------------------------------------------+
    //                                         
    // 

    reset_transform();               // ::BitBlt() works only if source has no transform in effect
    
    for (auto i = 0; i < m_bmct; i++)
    {
        // Find intersection rectangle of this BMP's rectangle and caller's source rectangle
        // --------------------------------------------------------------------------------- 

        int bmp_x { (int)(m_bmpos.at(i).x   ) }; 
        int bmp_y { (int)(m_bmpos.at(i).y   ) };
        int bmp_w { (int)(m_bmpos.at(i).cols) };
        int bmp_h { (int)(m_bmpos.at(i).rows) };

        Gdiplus::Rect intersect { bmp_x, bmp_y, bmp_w, bmp_h };                           // start with rectangle for this BMP in canvas_C                                                                                   
        auto i_rc = intersect.Intersect(Gdiplus::Rect {src_x, src_y, dest_w, dest_h} );   // replace with intersection of this BMP rectangle and caller's source rectangle
        
        if (m_debug > 1)
            M_out( L"canvas_C::bit_blt() i=%d -- bmp_x=%d  bmp_y=%d  bmp_w=%d  bmp_h=%d -- intersect.X=%d  intersect.Y=%d  intersect.Width=%d  intersect.Height=%d")
                 % i % bmp_x % bmp_y % bmp_w % bmp_h % intersect.X % intersect.Y % intersect.Width % intersect.Height;
                 


        reset_transform(); 
        

        // Do partial BitBlt() from tyhis BMP if this intersection rectangle is non-empty
        // ------------------------------------------------------------------------------

        if (i_rc != 0)
        {
            if (m_debug > 1)
                M_out( L"canvas_C::bit_blt() calling BitBlt() i=%d -- nXDest=%d  nYDest=%d  nWidth=%d  nHeight=%d -- nXSrc=%d  nYSrc=%d")
                     % i % std::max(dest_x, bmp_x - src_x) % std::max(dest_y, bmp_y - src_y) % intersect.Width % intersect.Height % (intersect.X - bmp_x) % (intersect.Y - bmp_y);
                     

            auto b_rc = ::BitBlt( dest_dc                                   // destination DC
                                , std::max(dest_x, bmp_x - src_x)           // destination X
                                , std::max(dest_y, bmp_y - src_y)           // destination Y
                                , intersect.Width                           // Source and destination width
                                , intersect.Height                          // Source and destination Height
                                , m_hdc.at(i)                               // Source DC 
                                , intersect.X - bmp_x                       // starting column in this BMP
                                , intersect.Y - bmp_y                       // stating row in this BMP 
                                , rop                                       // raster operation (passed thru)
                             ); 

            if (b_rc == 0)
            {
                 M_out_emsg(L"canvas_C::bit_blt() -- R/C = 0 from ::BitBlt() -- ::GetLastError() = ") %  ::GetLastError();
                 return -1; 
            }
         }       // intersection rectangle is non empty
    }            // loop for each DC in canvas_C


    DEFAULT_TRANSFORM(L"bit_blt()")

    return 0; 
}
M_endf    


              
//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::term() -- get rid of GDI+ resources  
//||       ================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::term() try
{
    if (m_debug > 0)
        M_out(L"canvas_C::term() called"); 


    // return immediately, if this canvas_C object is not initialized at all
    // ---------------------------------------------------------------------

    if (!m_init)
       return 0; 
 

    // make sure everything is done before deleting stuff (only if fully active)
    // -------------------------------------------------------------------------

    if (m_usable)
        flush();            


    // get rid of text_format_S, font_S, text brush and private font collection, etc.
    // ------------------------------------------------------------------------------
    
    if (m_curr_font_p != nullptr)
    {
        delete m_curr_font_p;                // get rid of font_S object
        m_curr_font_p = nullptr; 
    }

    if (m_tf_p != nullptr)
    {
        delete m_tf_p;                       // get rid of text_format_S
        m_tf_p = nullptr; 
    }

    if (m_sf_p != nullptr)
    {
        delete m_sf_p;                       // get rid of GDI+ StringFormat 
        m_sf_p = nullptr; 
 
    }

    if (m_pfc_p != nullptr)
    {
        delete m_pfc_p;                      // get rid of GDI+ private font collection
        m_pfc_p = nullptr; 
    }

    if (m_text_br_p != nullptr)
    {
        delete m_text_br_p;                  // get rid of GDI+ current text brush  
        m_text_br_p = nullptr;
    }

    if (m_text_background_br_p != nullptr)
    {
        delete m_text_background_br_p;       // get rid of GDI+ current text background brush  
        m_text_background_br_p = nullptr;
    }


    // get rid of any GDI+ graphics objects first
    // ------------------------------------------

    for (auto i = 0; i < m_gr_p.size(); i++)
        delete m_gr_p.at(i); 
 
    m_gr_p.clear(); 


    // get rid of GDI DIBsections
    //---------------------------
      
    for (auto i = 0; i < m_hdc.size(); i++)
    {
        auto old_obj = ::SelectObject(m_hdc.at(i), m_hgdiobj_saved.at(i));    // deselect bitmap before deleting the bitmap
    
        if (old_obj == NULL) 
           M_out_emsg(L"canvas_C::term(): deselect bitmap[%d] failed") % i;  
    }

    for (auto i = 0; i < m_hbitmap.size(); i++)
    {
        auto rc = ::DeleteObject(m_hbitmap.at(i));    
    
        if (rc == 0)
            M_out_emsg(L"canvas_C::term(): ::DeleteObject(m_hbitmap[%d]) failed") % i; 
    }

    m_hgdiobj_saved.clear(); 
    m_hbitmap.clear(); 
    m_pvBits.clear(); 


    // get rid of GDI DCs
    //-------------------

    for (auto i = 0; i < m_hdc.size(); i++)
    {
        auto rc = ::DeleteDC(m_hdc.at(i));    
    
        if (rc == 0)
            M_out_emsg(L"canvas_C::term(): ::DeleteDC(m_hdc[%d]) failed") % i;     
    }

    m_hdc.clear(); 


    // mark bitmap slots as invalid and clear out m_init to indicate this canvas_C is not active 
    // -----------------------------------------------------------------------------------------

    m_bmpos.clear(); 

    m_bmct         = 0;
    m_width        = 0; 
    m_height       = 0;  

    m_init         = false;  

    return 0;
}
M_endf  



//__________________________________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|           =========================================================
//\/\/\/\|           multi-DC/Multi-DIBSection transform and drawing functions  
//\/\/\/\|           =========================================================
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|
//\/\/\/\\__________________________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||       canvas_C::init_transform_s() -- fill-in passed-in transform_S structure (also recompute imbedded GDI+ Matrixes in vector)
//||       ============================    --------------------------------------- 
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::init_transform_s(transform_S& tr, Gdiplus::REAL tx, Gdiplus::REAL ty, Gdiplus::REAL rotation, Gdiplus::REAL x_scale, Gdiplus::REAL y_scale, Gdiplus::REAL x_shear, Gdiplus::REAL y_shear) try
{
    if (m_debug > 0)
        M_out(L"canvas_C::init_transform_s(): called");

    tr.tx        = tx; 
    tr.ty        = ty;
    tr.rotation  = rotation; 
    tr.x_scale   = x_scale; 
    tr.y_scale   = y_scale; 
    tr.x_shear   = x_shear; 
    tr.y_shear   = y_shear; 


    //  create vector of pointers to empty Gdiplus::Matrix objects  -- one per DC/bitmap

    tr.matrix_p.clear();       // get rid of any leftover stuff from prior usage
  
    for (auto i = 0; i < m_bmct; i++)
    {
        tr.matrix_p.push_back(std::unique_ptr<Gdiplus::Matrix> { new Gdiplus::Matrix {} }); 
    }

    return update_transform_s(tr);  
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||       update_transform_s() -- recompute Gdiplus Matrixes in passed-in transform_S, based on current values in transform_S
//||       ====================    -------------------------------------------------------------------------------------- 
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::update_transform_s(transform_S& tr) try
{
    Gdiplus::Status sts;

    if (m_debug > 0)
        M_out(L"canvas_C::update_transform_s(): called");

    for (auto i = 0; i < m_bmct; i++)
    {
        sts = tr.matrix_p.at(i)->Reset();                                           // reset this transform matrix to default state
        M_checks_ret(sts, L"canvas_C::update_transform_s() -- Matrix[i]::Reset()") 

        sts = tr.matrix_p.at(i)->Translate(tr.tx - (Gdiplus::REAL)(m_bmpos.at(i).x), tr.ty - (Gdiplus::REAL)(m_bmpos.at(i).y));  
        M_checks_ret(sts, L"canvas_C::update_transform_s() -- Matrix[i]::Translate()")
        
        sts = tr.matrix_p.at(i)->Rotate(tr.rotation);
        M_checks_ret(sts, L"canvas_C::update_transform_s() -- Matrix[i]::Rotate()")
        
        sts = tr.matrix_p.at(i)->Shear(tr.x_shear, tr.y_shear);  
        M_checks_ret(sts, L"canvas_C::update_transform_s() -- Matrix[i]::Shear()")
        
        sts = tr.matrix_p.at(i)->Scale(tr.x_scale, tr.y_scale);  
        M_checks_ret(sts, L"canvas_C::update_transform_s() -- Matrix[i]::Scale()")       
    }

    if (m_debug > 0)
        M_out("tx=%.6f  ty=%.6f  rotation=%.6f  x_scale=%.6f  y_scale=%.6f   x_shear=%.6f  y_shear=%.6f") % tr.tx % tr.ty % tr.rotation % tr.x_scale % tr.y_scale % tr.x_shear % tr.y_shear;    

   // {
   //      Gdiplus::REAL m1[6], m2[6];   
   //  
   //      tr.matrix1.GetElements(m1);
   //      tr.matrix2.GetElements(m2);
   //
   //      M_out( "matrix1 = %-12.6f  %-12.6f         matrix2 = %-12.6f    %-12.6f") % m1[0] % m1[1] % m2[0] % m2[1];
   //      M_out( "          %-12.6f  %-12.6f                   %-12.6f    %-12.6f") % m1[2] % m1[3] % m2[2] % m2[3];
   //      M_out( "          %-12.6f  %-12.6f                   %-12.6f    %-12.6f") % m1[4] % m1[5] % m2[4] % m2[5];    
   // }
    
    return 0;   
}
M_endf   


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       multi-DC/DIBSection GDI+ transform-related functions
//||       ==================================================== 
//||    
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// combined higher-level function to call individual GDI+ muti-DC/DIBSection transform functions
// ---------------------------------------------------------------------------------------------

M_DRAW_IMPEXP
int canvas_C::set_transform(Gdiplus::REAL tx, Gdiplus::REAL ty, Gdiplus::REAL r, Gdiplus::REAL x_scale, Gdiplus::REAL y_scale)
{
    int              rc = 0;    
    Gdiplus::Status  sts; 

    if (m_debug > 0)
        M_out(L"set_transform():  tx=%.6f    ty=%.6f    r=%.6f     x_scale=%.6f     y_scale=%.6f") % tx % ty % r % x_scale % y_scale;
 
    
    rc += translate_transform(tx, ty);
      
    if (r != 0.0)
       rc += rotate_transform(r);
     
    if ( (x_scale != 1.0) || (y_scale != 1.0 ) )
       rc += scale_transform(x_scale, y_scale);
        
    return rc;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::reset_transform()
{
    if (m_debug > 0)
        M_out(L"reset_transform() called");

    Gdiplus::Status sts; 
    APPLY_OP(reset_transform, ResetTransform, ())
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::default_transform()
{
    if (m_debug > 0)
        M_out(L"default_transform() called");

    Gdiplus::Status sts; 

    for (auto i = 0; i < m_bmct; i++)
    {
        sts = m_gr_p.at(i)->ResetTransform();
        M_checks_ret(sts, L"default_transform() : After m_gr_p[i]->ResetTransform()")
    
        sts = m_gr_p.at(i)->TranslateTransform(-(Gdiplus::REAL)(m_bmpos.at(i).x), - (Gdiplus::REAL)(m_bmpos.at(i).y));   
        M_checks_ret(sts, L"default_transform() : After m_gr_p[i]->TranslateTransform()")     
    }
  
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::set_transform(const transform_S& tr)
{
    if (m_debug > 0)
        M_out(L"set_transform(): called");

    Gdiplus::Status  sts; 
    Gdiplus::Matrix  m  { };  

    // ( note: can't use APPLY_OP, since different matrix has to be used for each call to GDI+ )

    for (auto i = 0; i < m_bmct; i++)
    {
        sts = m_gr_p.at(i)->SetTransform(tr.matrix_p.at(i).get()); 
        M_checks_ret(sts, L"set_transform() : After m_gr_p[i]->SetTransform()")    
    }

    return 0;
}
    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::translate_transform(Gdiplus::REAL tx, Gdiplus::REAL ty)
{
    if (m_debug > 0)
        M_out(L"translate_transform(): called");

    Gdiplus::Status sts; 

    for (auto i = 0; i < m_bmct; i++)
    {
        sts = m_gr_p.at(i)->TranslateTransform(tx - (Gdiplus::REAL)(m_bmpos.at(i).x), ty - (Gdiplus::REAL)(m_bmpos.at(i).y));   
        M_checks_ret(sts, L"default_transform() : After m_gr_p[i]->TranslateTransform()")     
    }

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::rotate_transform(Gdiplus::REAL r)
{
    if (m_debug > 0)
        M_out(L"rotate_transform(): called");

    Gdiplus::Status sts; 
    APPLY_OP(rotate_transform, RotateTransform, (r) )
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::scale_transform(Gdiplus::REAL x_scale, Gdiplus::REAL y_scale)
{
    if (m_debug > 0)
        M_out(L"scale_transform(): called");

    Gdiplus::Status sts; 
    APPLY_OP(scale_transform, ScaleTransform, (x_scale, y_scale) )
    return 0;
}  


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||       multi-DC/DIBSection GDI+ drawing/filling functions
//||       ================================================== 
//|| 
//||       - these correspond to methods for the Gdiplus::graphics class, but operate on all the DIBSection bitmaps
//||         anchored in the canvas_C control area   
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::draw_arc(const Gdiplus::Pen *pen_p, INT x, INT y, INT width, INT height, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_arc()")
    APPLY_OP(draw_arc, DrawArc, (pen_p, x, y, width, height, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_arc(const Gdiplus::Pen *pen_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL width, Gdiplus::REAL height, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_arc()")
    APPLY_OP(draw_arc, DrawArc, (pen_p, x, y, width, height, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_arc(const Gdiplus::Pen *pen_p, const Gdiplus::Rect rect, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_arc()")
    APPLY_OP(draw_arc, DrawArc, (pen_p, rect, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_arc(const Gdiplus::Pen *pen_p, const Gdiplus::RectF rect, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_arc()")
    APPLY_OP(draw_arc, DrawArc, (pen_p, rect, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_bezier(const Gdiplus::Pen *pen_p, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_bezier()")
    APPLY_OP(draw_bezier, DrawBezier, (pen_p, x1, y1, x2, y2, x3, y3, x4, y4) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_bezier(const Gdiplus::Pen *pen_p, Gdiplus::REAL x1, Gdiplus::REAL y1, Gdiplus::REAL x2, Gdiplus::REAL y2, Gdiplus::REAL x3, Gdiplus::REAL y3, Gdiplus::REAL x4, Gdiplus::REAL y4)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_bezier()")
    APPLY_OP(draw_bezier, DrawBezier, (pen_p, x1, y1, x2, y2, x3, y3, x4, y4) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_bezier(const Gdiplus::Pen *pen_p, const Gdiplus::Point& p1, const Gdiplus::Point& p2, const Gdiplus::Point& p3, const Gdiplus::Point& p4)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_bezier()")
    APPLY_OP(draw_bezier, DrawBezier, (pen_p, p1, p2, p3, p4) )
    return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_bezier(const Gdiplus::Pen *pen_p, const Gdiplus::PointF& p1, const Gdiplus::PointF& p2, const Gdiplus::PointF& p3, const Gdiplus::PointF& p4)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_bezier()")
    APPLY_OP(draw_bezier, DrawBezier, (pen_p, p1, p2, p3, p4) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_beziers(const Gdiplus::Pen *pen_p, const Gdiplus::Point *p_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_beziers()")
    APPLY_OP(draw_beziers, DrawBeziers, (pen_p, p_p, count) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_beziers(const Gdiplus::Pen *pen_p, const Gdiplus::PointF *p_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_beziers()")
    APPLY_OP(draw_beziers, DrawBeziers, (pen_p, p_p, count) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_closed_curve(const Gdiplus::Pen *pen_p, const Gdiplus::Point *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_closed_curve()")
    APPLY_OP(draw_closed_curve, DrawClosedCurve, (pen_p, points_p, count) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_closed_curve(const Gdiplus::Pen *pen_p, const Gdiplus::PointF *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_closed_curve()")
    APPLY_OP(draw_closed_curve, DrawClosedCurve, (pen_p, points_p, count) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_closed_curve(const Gdiplus::Pen *pen_p, const Gdiplus::Point *points_p, INT count, Gdiplus::REAL tension)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_closed_curve()")
    APPLY_OP(draw_closed_curve, DrawClosedCurve, (pen_p, points_p, count, tension) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_closed_curve(const Gdiplus::Pen *pen_p, const Gdiplus::PointF *points_p, INT count, Gdiplus::REAL tension)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_closed_curve()")
    APPLY_OP(draw_closed_curve, DrawClosedCurve, (pen_p, points_p, count, tension) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_curve(const Gdiplus::Pen *pen_p, const Gdiplus::Point *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_curve()")
    APPLY_OP(draw_curve, DrawCurve, (pen_p, points_p, count) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_curve(const Gdiplus::Pen *pen_p, const Gdiplus::PointF *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_curve()")
    APPLY_OP(draw_curve, DrawCurve, (pen_p, points_p, count) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_curve(const Gdiplus::Pen *pen_p, const Gdiplus::Point *points_p, INT count, Gdiplus::REAL tension)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_curve()")
    APPLY_OP(draw_curve, DrawCurve, (pen_p, points_p, count, tension) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_curve(const Gdiplus::Pen *pen_p, const Gdiplus::PointF *points_p, INT count, Gdiplus::REAL tension)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_curve()")
    APPLY_OP(draw_curve, DrawCurve, (pen_p, points_p, count, tension) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_curve(const Gdiplus::Pen *pen_p, const Gdiplus::Point *points_p, INT count, INT offset, INT n, Gdiplus::REAL tension)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_curve()")
    APPLY_OP(draw_curve, DrawCurve, (pen_p, points_p, count, offset, n, tension) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_curve(const Gdiplus::Pen *pen_p, const Gdiplus::PointF *points_p, INT count, INT offset, INT n, Gdiplus::REAL tension)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_curve()")
    APPLY_OP(draw_curve, DrawCurve, (pen_p, points_p, count, offset, n, tension) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_ellipse(const Gdiplus::Pen *pen_p, INT x, INT y, INT w, INT h)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_ellipse()")
    APPLY_OP(draw_ellipse, DrawEllipse, (pen_p, x, y, w, h) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_ellipse(const Gdiplus::Pen *pen_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL w, Gdiplus::REAL h)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_ellipse()")
    APPLY_OP(draw_ellipse, DrawEllipse, (pen_p, x, y, w, h) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::canvas_C::draw_ellipse(const Gdiplus::Pen *pen_p, const Gdiplus::Rect& r)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_ellipse()")
    APPLY_OP(draw_ellipse, DrawEllipse, (pen_p, r) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::canvas_C::draw_ellipse(const Gdiplus::Pen *pen_p,  const Gdiplus::RectF& r)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_ellipse()")
    APPLY_OP(draw_ellipse, DrawEllipse, (pen_p, r) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_image(Gdiplus::Image *image_p, INT x, INT y)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_image()")
    APPLY_OP(draw_image, DrawImage, (image_p, x, y) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_image(Gdiplus::Image *image_p, INT x, INT y, INT width, INT height)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_image()")
    APPLY_OP(draw_image, DrawImage, (image_p, x, y, width, height) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_image(Gdiplus::Image *image_p, INT x, INT y, INT src_x, INT src_y, INT src_width, INT src_height)  // src unit always defaulted
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_image()")
    APPLY_OP(draw_image, DrawImage, (image_p, x, y, src_x, src_y, src_width, src_height, Gdiplus::UnitPixel) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_image(Gdiplus::Image *image_p, const Gdiplus::Point& point)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_image()")
    APPLY_OP(draw_image, DrawImage, (image_p, point) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_image(Gdiplus::Image *image_p, const Gdiplus::Rect& rect)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_image()")
    APPLY_OP(draw_image, DrawImage, (image_p, rect) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_image(Gdiplus::Image *image_p, const Gdiplus::Point *points_p, INT count)  
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_image()")
    APPLY_OP(draw_image, DrawImage, (image_p, points_p, count) )
    return 0;
}
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_image(Gdiplus::Image *image_p, const Gdiplus::Rect& dest_rect, INT src_x, INT src_y, INT src_width, INT src_height, Gdiplus::Unit src_unit, Gdiplus::ImageAttributes *attr_p)  // abort callback not supported
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_image()")
    APPLY_OP(draw_image, DrawImage, (image_p, dest_rect, src_x, src_y, src_width, src_height, src_unit, attr_p, (Gdiplus::DrawImageAbort)nullptr, nullptr) )
    return 0;
}
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_image(Gdiplus::Image *image_p, const Gdiplus::Point *points_p, INT count, INT src_x, INT src_y, INT src_width, INT src_height, Gdiplus::Unit src_unit, Gdiplus::ImageAttributes *attr_p)  // abort callback not supported
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_image()")
    APPLY_OP(draw_image, DrawImage, (image_p, points_p, count, src_x, src_y, src_width, src_height, src_unit, attr_p, (Gdiplus::DrawImageAbort)nullptr, nullptr) )
    return 0;
}
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_line(const Gdiplus::Pen *pen_p, INT x1, INT y1, INT x2, INT y2)
{
    Gdiplus::Status sts; 
    
    DEFAULT_TRANSFORM(L"draw_line()")
    APPLY_OP(draw_line, DrawLine, (pen_p, x1, y1, x2, y2) )  
    return 0;
}
    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_line(const Gdiplus::Pen *pen_p, Gdiplus::REAL x1, Gdiplus::REAL y1, Gdiplus::REAL x2, Gdiplus::REAL y2)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_line()")
    APPLY_OP(draw_line, DrawLine, (pen_p, x1, y1, x2, y2) )  
    return 0;
}
   

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_line(const Gdiplus::Pen *pen_p, const Gdiplus::Point& p1, const Gdiplus::Point& p2)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_line()")
    APPLY_OP(draw_line, DrawLine, (pen_p, p1, p2) ) 
    return 0;
}
   

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_line(const Gdiplus::Pen *pen_p, const Gdiplus::PointF& p1, const Gdiplus::PointF& p2)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_line()")
    APPLY_OP(draw_line, DrawLine, (pen_p, p1, p2) ) 
    return 0;
}
    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_lines(const Gdiplus::Pen *pen_p, const Gdiplus::Point *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_lines()")
    APPLY_OP(draw_lines, DrawLines, (pen_p, points_p, count) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_lines(const Gdiplus::Pen *pen_p, const Gdiplus::PointF *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_lines()")
    APPLY_OP(draw_lines, DrawLines, (pen_p, points_p, count) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_path(const Gdiplus::Pen *pen_p, const Gdiplus::GraphicsPath *path_p)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_path()")
    APPLY_OP(draw_path, DrawPath, (pen_p, path_p) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_pie(const Gdiplus::Pen *pen_p, INT x, INT y, INT width, INT height, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_pie()")
    APPLY_OP(draw_pie, DrawPie, (pen_p, x, y, width, height, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_pie(const Gdiplus::Pen *pen_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL width, Gdiplus::REAL height, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_pie()")
    APPLY_OP(draw_pie, DrawPie, (pen_p, x, y, width, height, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_pie(const Gdiplus::Pen *pen_p, const Gdiplus::Rect rect, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_pie()")
    APPLY_OP(draw_pie, DrawPie, (pen_p, rect, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_pie(const Gdiplus::Pen *pen_p, const Gdiplus::RectF rect, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_pie()")
    APPLY_OP(draw_pie, DrawPie, (pen_p, rect, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_polygon(const Gdiplus::Pen *pen_p, const Gdiplus::Point *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_polygon()")
    APPLY_OP(draw_polygon, DrawPolygon, (pen_p, points_p, count) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_polygon(const Gdiplus::Pen *pen_p, const Gdiplus::PointF *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_polygon()")
    APPLY_OP(draw_polygon, DrawPolygon, (pen_p, points_p, count) ) 
    return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_rectangle(const Gdiplus::Pen *pen_p, INT x, INT y, INT w, INT h)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_rectangle()")
    APPLY_OP(draw_rectangle, DrawRectangle, (pen_p, x, y, w, h) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_rectangle(const Gdiplus::Pen *pen_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL w, Gdiplus::REAL h)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_rectangle()")
    APPLY_OP(draw_rectangle, DrawRectangle, (pen_p, x, y, w, h) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_rectangle(const Gdiplus::Pen *pen_p, const Gdiplus::Rect& r)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_rectangle()")
    APPLY_OP(draw_rectangle, DrawRectangle, (pen_p, r) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_rectangle(const Gdiplus::Pen *pen_p,  const Gdiplus::RectF& r)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_rectangle()")
    APPLY_OP(draw_rectangle, DrawRectangle, (pen_p, r) ) 
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_rectangles(const Gdiplus::Pen *pen_p, const Gdiplus::Rect *r_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_rectangles()")
    APPLY_OP(draw_rectangles, DrawRectangles, (pen_p, r_p, count) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_rectangles(const Gdiplus::Pen *pen_p,  const Gdiplus::RectF *r_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"draw_rectangles()")
    APPLY_OP(draw_rectangles, DrawRectangles, (pen_p, r_p, count) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_string(const WCHAR *wchar_p, INT length, const Gdiplus::Font *font_p, const Gdiplus::PointF& point, const Gdiplus::Brush *brush_p)
{
    Gdiplus::Status sts; 
    APPLY_OP(draw_string, DrawString, (wchar_p, length, font_p, point, brush_p) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_string(const WCHAR *wchar_p, INT length, const Gdiplus::Font *font_p, const Gdiplus::PointF& point, const Gdiplus::StringFormat *string_format_p, const Gdiplus::Brush *brush_p)
{
    Gdiplus::Status sts; 
    APPLY_OP(draw_string, DrawString, (wchar_p, length, font_p, point, string_format_p, brush_p) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_string(const WCHAR *wchar_p, INT length, const Gdiplus::Font *font_p, const Gdiplus::RectF& rect, const Gdiplus::StringFormat *string_format_p, const Gdiplus::Brush *brush_p)
{
    Gdiplus::Status sts; 
    APPLY_OP(draw_string, DrawString, (wchar_p, length, font_p, rect, string_format_p, brush_p) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_closed_curve(const Gdiplus::Brush *brush_p, const Gdiplus::Point *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_closed_curve()")
    APPLY_OP(fill_closed_curve, FillClosedCurve, (brush_p, points_p, count) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_closed_curve(const Gdiplus::Brush *brush_p, const Gdiplus::PointF *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_closed_curve()")
    APPLY_OP(fill_closed_curve, FillClosedCurve, (brush_p, points_p, count) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_closed_curve(const Gdiplus::Brush *brush_p, const Gdiplus::Point *points_p, INT count, Gdiplus::FillMode fm, Gdiplus::REAL tension)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_closed_curve()")
    APPLY_OP(fill_closed_curve, FillClosedCurve, (brush_p, points_p, count, fm, tension) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_closed_curve(const Gdiplus::Brush *brush_p, const Gdiplus::PointF *points_p, INT count, Gdiplus::FillMode fm, Gdiplus::REAL tension)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_closed_curve()")
    APPLY_OP(fill_closed_curve, FillClosedCurve, (brush_p, points_p, count, fm, tension) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_ellipse(const Gdiplus::Brush *brush_p, INT x, INT y, INT w, INT h)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_ellipse()")
    APPLY_OP(fill_ellipse, FillEllipse, (brush_p, x, y, w, h) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_ellipse(const Gdiplus::Brush *brush_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL w, Gdiplus::REAL h)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_ellipse()")
    APPLY_OP(fill_ellipse, FillEllipse, (brush_p, x, y, w, h) )
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_ellipse(const Gdiplus::Brush *brush_p, const Gdiplus::Rect& r)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_ellipse()")
    APPLY_OP(fill_ellipse, FillEllipse, (brush_p, r) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_ellipse(const Gdiplus::Brush *brush_p,  const Gdiplus::RectF& r)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_ellipse()")
    APPLY_OP(fill_ellipse, FillEllipse, (brush_p, r) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_path(const Gdiplus::Brush *brush_p, const Gdiplus::GraphicsPath *path_p)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_path()")
    APPLY_OP(fill_path, FillPath, (brush_p, path_p) )
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_pie(const Gdiplus::Brush *brush_p, INT x, INT y, INT width, INT height, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_pie()")
    APPLY_OP(fill_pie, FillPie, (brush_p, x, y, width, height, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_pie(const Gdiplus::Brush *brush_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL width, Gdiplus::REAL height, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_pie()")
    APPLY_OP(fill_pie, FillPie, (brush_p, x, y, width, height, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_pie(const Gdiplus::Brush *brush_p, const Gdiplus::Rect rect, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_pie()")
    APPLY_OP(fill_pie, FillPie, (brush_p, rect, start_angle, sweep_angle) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_pie(const Gdiplus::Brush *brush_p, const Gdiplus::RectF rect, Gdiplus::REAL start_angle, Gdiplus::REAL sweep_angle)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_pie()")
    APPLY_OP(fill_pie, FillPie, (brush_p, rect, start_angle, sweep_angle) )
    return 0;
}  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_polygon(const Gdiplus::Brush *brush_p, const Gdiplus::Point *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_polygon()")
    APPLY_OP(fill_polygon, FillPolygon, (brush_p, points_p, count) ) 
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_polygon(const Gdiplus::Brush *brush_p, const Gdiplus::PointF *points_p, INT count)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_polygon()")
    APPLY_OP(fill_polygon, FillPolygon, (brush_p, points_p, count) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_polygon(const Gdiplus::Brush *brush_p, const Gdiplus::Point *points_p, INT count, Gdiplus::FillMode fm)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_polygon()")
    APPLY_OP(fill_polygon, FillPolygon, (brush_p, points_p, count, fm) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_polygon(const Gdiplus::Brush *brush_p, const Gdiplus::PointF *points_p, INT count, Gdiplus::FillMode fm)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_polygon()")
    APPLY_OP(fill_polygon, FillPolygon, (brush_p, points_p, count, fm) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_rectangle(const Gdiplus::Brush *brush_p, INT x, INT y, INT w, INT h)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_rectangle()")
    APPLY_OP(fill_rectangle, FillRectangle, (brush_p, x, y, w, h) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_rectangle(const Gdiplus::Brush *brush_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL w, Gdiplus::REAL h)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_rectangle()")
    APPLY_OP(fill_rectangle, FillRectangle, (brush_p, x, y, w, h) )
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_rectangle(const Gdiplus::Brush *brush_p, const Gdiplus::Rect& r)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_rectangle()")
    APPLY_OP(fill_rectangle, FillRectangle, (brush_p, r) )
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_rectangle(const Gdiplus::Brush *brush_p,  const Gdiplus::RectF& r)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_rectangle()")
    APPLY_OP(fill_rectangle, FillRectangle, (brush_p, r) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_rectangles(const Gdiplus::Brush *brush_p, const Gdiplus::Rect *r_p, INT ct)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_rectangles()")
    APPLY_OP(fill_rectangles, FillRectangles, (brush_p, r_p, ct) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_rectangles(const Gdiplus::Brush *brush_p, const Gdiplus::RectF *r_p, INT ct)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_rectangles()")
    APPLY_OP(fill_rectangles, FillRectangles, (brush_p, r_p, ct) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::fill_region(const Gdiplus::Brush *brush_p, const Gdiplus::Region *region_p)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"fill_region()")
    APPLY_OP(fill_region, FillRegion, (brush_p, region_p) )
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::gdi__clear(Gdiplus::Color color)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"gdi__clear()")
    APPLY_OP(gei__clear, Clear, (color) )
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::gdi__flush(Gdiplus::FlushIntention flush_intention)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"gdi__flush()")
    APPLY_OP_N(gdi__flush, Flush, (flush_intention) )
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::reset_clip()
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"reset_clip()")
    APPLY_OP(reset_clip, ResetClip, ( ) )
    return 0;
}
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::set_clip(const Gdiplus::Rect& rect)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"set_clip()")
    APPLY_OP(set_clip, SetClip, (rect) )
    return 0;
}

M_DRAW_IMPEXP
int canvas_C::set_clip(const Gdiplus::Rect& rect, Gdiplus::CombineMode combine_mode)
{
    Gdiplus::Status sts; 

    DEFAULT_TRANSFORM(L"set_clip()")
    APPLY_OP(set_clip, SetClip, (rect, combine_mode) )
    return 0;
}



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||      Multi-DC/DIBSection convenience functions  
//||      -----------------------------------------
//|| 
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       higher-level drawing functions -- pass in rgb_S, etc. -- not brush, pen
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// -----------------------
// versions without alpha:  (these just call alpha versions with alpha set to 255 (opaque))
// -----------------------

// fixed-point versions

M_DRAW_IMPEXP
int canvas_C::draw_line(const rgb_S& rgb, rc_T th, rc_T row1, rc_T col1, rc_T row2, rc_T col2) try 
{
    return draw_line(rgb, 255, th, row1, col1, row2, col2);
}
M_endf
  

M_DRAW_IMPEXP
int canvas_C::draw_box(const rgb_S& rgb, rc_T row, rc_T col, rc_T w, rc_T h) try
{
    return draw_box(rgb, 255, row, col, w, h);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_frame(const rgb_S& rgb, rc_T th, rc_T row, rc_T col, rc_T w, rc_T h) try 
{
    return draw_frame(rgb, 255, th, row, col, w, h);
}
M_endf
      

M_DRAW_IMPEXP
int canvas_C::draw_ring(const rgb_S& rgb, rc_T th, rc_T row, rc_T col, rc_T r) try 
{
    return draw_ring(rgb, 255, th, row, col, r);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_circle(const rgb_S& rgb, rc_T row, rc_T col, rc_T r) try 
{
    return draw_circle(rgb, 255, row, col, r);
}
M_endf


////// floating point versions

M_DRAW_IMPEXP
int canvas_C::draw_line(const rgb_S& rgb, rcf_T th, rcf_T row1, rcf_T col1, rcf_T row2, rcf_T col2) try 
{
    return draw_line(rgb, 255, th, row1, col1, row2, col2);
}
M_endf
  

M_DRAW_IMPEXP
int canvas_C::draw_box(const rgb_S& rgb, rcf_T row, rcf_T col, rcf_T w, rcf_T h) try
{
    return draw_box(rgb, 255, row, col, w, h);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_frame(const rgb_S& rgb, rcf_T th, rcf_T row, rcf_T col, rcf_T w, rcf_T h) try 
{
    return draw_frame(rgb, 255, th, row, col, w, h);
}
M_endf
      

M_DRAW_IMPEXP
int canvas_C::draw_ring(const rgb_S& rgb, rcf_T th, rcf_T row, rcf_T col, rcf_T r) try 
{
    return draw_ring(rgb, 255, th, row, col, r);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_circle(const rgb_S& rgb, rcf_T row, rcf_T col, rcf_T r) try 
{
    return draw_circle(rgb, 255, row, col, r);
}
M_endf



// versions with alpha:    
// --------------------

// fixed-point versions

M_DRAW_IMPEXP
int canvas_C::draw_line(const rgb_S& rgb, alpha_T alpha, rc_T th, rc_T row1, rc_T col1, rc_T row2, rc_T col2) try 
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_line(&pen, (INT)col1, (INT)row1, (INT)col2, (INT)row2);
}
M_endf
  

M_DRAW_IMPEXP
int canvas_C::draw_box(const rgb_S& rgb, alpha_T alpha, rc_T row, rc_T col, rc_T w, rc_T h) try
{
    Gdiplus::SolidBrush brush { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b) };   
    
    return fill_rectangle(&brush, (INT)col, (INT)row, (INT)w, (INT)h);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_frame(const rgb_S& rgb, alpha_T alpha, rc_T th, rc_T row, rc_T col, rc_T w, rc_T h) try 
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_rectangle(&pen, (INT)col, (INT)row, (INT)w, (INT)h);
}
M_endf
      

M_DRAW_IMPEXP
int canvas_C::draw_ring(const rgb_S& rgb, alpha_T alpha, rc_T th, rc_T row, rc_T col, rc_T r) try 
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_ellipse(&pen, (INT)(col - r), (INT)(row - r), (INT)(2 * r), (INT)(2 * r));
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_circle(const rgb_S& rgb, alpha_T alpha, rc_T row, rc_T col, rc_T r) try 
{       
    Gdiplus::SolidBrush brush { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b) };   
    
    return fill_ellipse(&brush, (INT)(col - r), (INT)(row - r), (INT)(2 * r), (INT)(2 * r));
}
M_endf


 // floating-point versions

M_DRAW_IMPEXP
int canvas_C::draw_line(const rgb_S& rgb, alpha_T alpha, rcf_T th, rcf_T row1, rcf_T col1, rcf_T row2, rcf_T col2) try 
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_line(&pen, (Gdiplus::REAL)col1, (Gdiplus::REAL)row1, (Gdiplus::REAL)col2, (Gdiplus::REAL)row2);
}
M_endf
  

M_DRAW_IMPEXP
int canvas_C::draw_box(const rgb_S& rgb, alpha_T alpha, rcf_T row, rcf_T col, rcf_T w, rcf_T h) try
{
    Gdiplus::SolidBrush brush { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b) };   
    
    return fill_rectangle(&brush, (Gdiplus::REAL)col, (Gdiplus::REAL)row, (Gdiplus::REAL)w, (Gdiplus::REAL)h);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_frame(const rgb_S& rgb, alpha_T alpha, rcf_T th, rcf_T row, rcf_T col, rcf_T w, rcf_T h) try 
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_rectangle(&pen, (Gdiplus::REAL)col, (Gdiplus::REAL)row, (Gdiplus::REAL)w, (Gdiplus::REAL)h);
}
M_endf
      

M_DRAW_IMPEXP
int canvas_C::draw_ring(const rgb_S& rgb, alpha_T alpha, rcf_T th, rcf_T row, rcf_T col, rcf_T r) try 
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_ellipse(&pen, (Gdiplus::REAL)(col - r), (Gdiplus::REAL)(row - r), (Gdiplus::REAL)(2.0 * r), (Gdiplus::REAL)(2.0 * r));
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_circle(const rgb_S& rgb, alpha_T alpha, rcf_T row, rcf_T col, rcf_T r) try 
{
    Gdiplus::SolidBrush brush { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b) };   
    
    return fill_ellipse(&brush, (Gdiplus::REAL)(col - r), (Gdiplus::REAL)(row - r), (Gdiplus::REAL)(2.0 * r), (Gdiplus::REAL)(2.0 * r));
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       low-level circle drawing -- pass in radius and center point coordinates
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int canvas_C::draw_circle(const Gdiplus::Pen *pen_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL r) try 
{
    return draw_ellipse(pen_p, x - r, y - r, 2.0 * r, 2.0 * r);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::draw_circle(const Gdiplus::Pen *pen_p, Gdiplus::PointF pt, Gdiplus::REAL r) try
{
    return draw_ellipse(pen_p, pt.X - r, pt.Y - r, 2.0 * r, 2.0 * r);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::fill_circle(const Gdiplus::Brush *brush_p, Gdiplus::REAL x, Gdiplus::REAL y, Gdiplus::REAL r) try
{
    return fill_ellipse(brush_p, x - r, y - r, 2.0 * r, 2.0 * r);
}
M_endf


M_DRAW_IMPEXP
int canvas_C::fill_circle(const Gdiplus::Brush *brush_p, Gdiplus::PointF pt, Gdiplus::REAL r) try
{
    return fill_ellipse(brush_p, pt.X - r, pt.Y - r, 2.0 * r, 2.0 * r);
}
M_endf   



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     canvas_C::draw_beziers() -- high-level drawing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
// fixed-point

M_DRAW_IMPEXP
int canvas_C::draw_beziers( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    return draw_beziers(rgb, 255, th, points);   
}
M_endf


// fixed point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_beziers( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_beziers(&pen, points.data(), (INT)(points.size()));   
}
M_endf


// floating-point 

M_DRAW_IMPEXP
int canvas_C::draw_beziers( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    return draw_beziers(rgb, 255, th, points);   
}
M_endf


// floating point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_beziers( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_beziers(&pen, points.data(), (INT)(points.size()));   
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     canvas_C::draw_curve() -- high-level drawing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
// fixed-point

M_DRAW_IMPEXP
int canvas_C::draw_curve( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    return draw_curve(rgb, 255, th, points);   
}
M_endf


// fixed point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_curve( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_curve(&pen, points.data(), (INT)(points.size()));   
}
M_endf


// floating-point 

M_DRAW_IMPEXP
int canvas_C::draw_curve( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    return draw_curve(rgb, 255, th, points);   
}
M_endf


// floating point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_curve( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th };  

    return draw_curve(&pen, points.data(), (INT)(points.size()));   
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     canvas_C::draw_closed_curve() -- high-level drawing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
// fixed-point

M_DRAW_IMPEXP
int canvas_C::draw_closed_curve( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    return draw_closed_curve(rgb, 255, th, points);   
}
M_endf


// fixed point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_closed_curve( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_closed_curve(&pen, points.data(), (INT)(points.size()));   
}
M_endf


// floating-point 

M_DRAW_IMPEXP
int canvas_C::draw_closed_curve( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    return draw_closed_curve(rgb, 255, th, points);   
}
M_endf


// floating point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_closed_curve( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_closed_curve(&pen, points.data(), (INT)(points.size()));   
}
M_endf  


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     canvas_C::draw_lines() -- high-level drawing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
// fixed-point

M_DRAW_IMPEXP
int canvas_C::draw_lines( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    return draw_lines(rgb, 255, th, points);   
}
M_endf


// fixed point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_lines( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_lines(&pen, points.data(), (INT)(points.size()));   
}
M_endf


// floating-point 

M_DRAW_IMPEXP
int canvas_C::draw_lines( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    return draw_lines(rgb, 255, th, points);   
}
M_endf


// floating point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_lines( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th };   

    return draw_lines(&pen, points.data(), (INT)(points.size()));   
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     canvas_C::draw_polygon() -- high-level drawing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
// fixed-point

M_DRAW_IMPEXP
int canvas_C::draw_polygon( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    return draw_polygon(rgb, 255, th, points);   
}
M_endf


// fixed point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_polygon( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::Point>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_polygon(&pen, points.data(), (INT)(points.size()));   
}
M_endf


// floating-point 

M_DRAW_IMPEXP
int canvas_C::draw_polygon( const rgb_S& rgb, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    return draw_polygon(rgb, 255, th, points);   
}
M_endf


// floating point with alpha

M_DRAW_IMPEXP
int canvas_C::draw_polygon( const rgb_S& rgb, alpha_T alpha, rcf_T th, const std::vector<Gdiplus::PointF>& points) try
{
    Gdiplus::Pen pen { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b), (Gdiplus::REAL)th }; 

    return draw_polygon(&pen, points.data(), (INT)(points.size()));   
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     canvas_C::fill_closed_curve() -- high-level drawing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
// fixed-point

M_DRAW_IMPEXP
int canvas_C::fill_closed_curve( const rgb_S& rgb, const std::vector<Gdiplus::Point>& points) try
{
    return fill_closed_curve(rgb, 255, points);   
}
M_endf


// fixed point with alpha

M_DRAW_IMPEXP
int canvas_C::fill_closed_curve( const rgb_S& rgb, alpha_T alpha, const std::vector<Gdiplus::Point>& points) try
{
    Gdiplus::SolidBrush brush { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b) };

    return fill_closed_curve(&brush, points.data(), (INT)(points.size()));   
}
M_endf


// floating-point 

M_DRAW_IMPEXP
int canvas_C::fill_closed_curve( const rgb_S& rgb, const std::vector<Gdiplus::PointF>& points) try
{
    return fill_closed_curve(rgb, 255, points);   
}
M_endf


// floating point with alpha

M_DRAW_IMPEXP
int canvas_C::fill_closed_curve( const rgb_S& rgb, alpha_T alpha, const std::vector<Gdiplus::PointF>& points) try
{
    M__(M_out(L"alpha=%d") % alpha;) 

    Gdiplus::SolidBrush brush { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b) };   

    return fill_closed_curve(&brush, points.data(), (INT)(points.size()));   
}
M_endf  


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     canvas_C::fill_polygon() -- high-level drawing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
// fixed-point

M_DRAW_IMPEXP
int canvas_C::fill_polygon( const rgb_S& rgb, const std::vector<Gdiplus::Point>& points) try
{
    return fill_polygon(rgb, 255, points);   
}
M_endf


// fixed point with alpha

M_DRAW_IMPEXP
int canvas_C::fill_polygon( const rgb_S& rgb, alpha_T alpha, const std::vector<Gdiplus::Point>& points) try
{
    Gdiplus::SolidBrush brush { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b) };

    return fill_polygon(&brush, points.data(), (INT)(points.size()));   
}
M_endf


// floating-point 

M_DRAW_IMPEXP
int canvas_C::fill_polygon( const rgb_S& rgb, const std::vector<Gdiplus::PointF>& points) try
{
    return fill_polygon(rgb, 255, points);   
}
M_endf


// floating point with alpha

M_DRAW_IMPEXP
int canvas_C::fill_polygon( const rgb_S& rgb, alpha_T alpha, const std::vector<Gdiplus::PointF>& points) try
{
    Gdiplus::SolidBrush brush { Gdiplus::Color(alpha, rgb.r, rgb.g, rgb.b) };   

    return fill_polygon(&brush, points.data(), (INT)(points.size()));   
}
M_endf  


//__________________________________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|            =========================================
//\/\/\/\|            Canvas_C-related string drawing functions 
//\/\/\/\|            =========================================
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|
//\/\/\/\\__________________________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//|| 
//||       canvas_C::draw_str()-- draws all wchars in std::wstring 
//||       ====================   -------------------------------- 
//||       
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// internal form -- uses passed-in Gdiplus::Font and test_format_S
// ---------------------------------------------------------------

M_DRAW_IMPEXP
int  canvas_C::draw_str(const std::wstring& str, const font_S& fs, text_format_S& tf) try
{
    int rc;
  
    if (m_debug > 0)
        M_out(L"canvas_C::draw_str() -- tf.draw_background = %d") % ((int32_t)(tf.draw_background));

    
    // initialize current xform fields from initial xform fields

    tf.curr_x           = tf.x; 
    tf.curr_y           = tf.y;
    tf.curr_rotation    = tf.rotation; 
    tf.curr_x_scale     = tf.x_scale;
    tf.curr_y_scale     = tf.y_scale; 
    tf.curr_x_shear     = tf.x_shear;
    tf.curr_y_shear     = tf.y_shear; 
    tf.curr_extra_space = 0.0;              // not supported   -- see draw_txt() for support


    // compute adjusted x and y values to to reposition centerline at transform origin 

    Gdiplus::REAL adj_x  =   fs.center_adjust * M_sind(tf.curr_rotation)  * tf.y_scale; 
    Gdiplus::REAL adj_y  = -(fs.center_adjust * M_cosd(tf.curr_rotation)) * tf.y_scale; 

    
    // set up transform, as required (to put "center line" of text at caller's position) 

    bool saved_default_transform = m_default_transform;     // save setting
    m_default_transform = false;                            // don't re-default transform on every call

    init_transform_s(tf.curr_transform, tf.curr_x + adj_x, tf.curr_y + adj_y, tf.curr_rotation, tf.curr_x_scale, tf.curr_y_scale, tf.curr_x_shear, tf.curr_y_shear);                                                       
    
   
    rc = reset_transform();
    M_checkrc_ret(rc, L"canvas_C::draw_str(): reset_transform()")
    rc = set_transform(tf.curr_transform);
    M_checkrc_ret(rc, L"canvas_C::draw_str(): set_transform()")
   
  
    // measure width of string, and set output fields in text_format structure (note: only do measurement in 1st DC -- seems to work even when entirely out of bounds)
    // -----------------------------------------------------------------------
   
    measure_str(str.c_str(), str.size(), fs, tf);        // sets tf.output_width field
    ::text_new_xy(tf, tf.curr_rotation);                 // update new_x and new_y fields based on cur_rotation

  
    M__(M_out(L"canvas_C::draw_str(): gr1: x=%.6f   y=%.6f    rot=%.6f    scale=%.6f/%.6f    width=%.6f    new_x=%.6f   new_y=%.6f")
              % tf.curr_x % tf.curr_y % tf.curr_rotation % tf.curr_x_scale % tf.curr_y_scale % tf.output_width % tf.new_x % tf.new_y;) 

    M__(M_out(L"canvas_C::draw_str(): rect_out -- X=%.6f   Y=%.6f   Width=%.6f   Height=%.6f", rect_out.X) % rect_out.Y % rect_out.Width % rect_out.Height;)

    tf.bounds_poly[0] = ::transform_point(tf.bound_rect.X                         , tf.bound_rect.Y                        , tf.curr_transform);
    tf.bounds_poly[1] = ::transform_point(tf.bound_rect.X  + tf.bound_rect.Width  , tf.bound_rect.Y                        , tf.curr_transform);
    tf.bounds_poly[2] = ::transform_point(tf.bound_rect.X  + tf.bound_rect.Width  , tf.bound_rect.Y + tf.bound_rect.Height , tf.curr_transform);
    tf.bounds_poly[3] = ::transform_point(tf.bound_rect.X                         , tf.bound_rect.Y + tf.bound_rect.Height , tf.curr_transform);

    M__(M_out(L"canvas_C::draw_str(): bounds_poly[0] =  %.6f, %.6f") % tf.bounds_poly[0].X % tf.bounds_poly[0].Y;)
    M__(M_out(L"canvas_C::draw_str(): bounds_poly[1] =  %.6f, %.6f") % tf.bounds_poly[1].X % tf.bounds_poly[1].Y;)
    M__(M_out(L"canvas_C::draw_str(): bounds_poly[2] =  %.6f, %.6f") % tf.bounds_poly[2].X % tf.bounds_poly[2].Y;)
    M__(M_out(L"canvas_C::draw_str(): bounds_poly[3] =  %.6f, %.6f") % tf.bounds_poly[3].X % tf.bounds_poly[3].Y;)
    
    M__(M_out(L"canvas_C::draw_str(): tf.draw_background = %d    tf.text_brush_p = %p    tf.background_brush_p = %p") % ((int32_t)(tf.draw_background)) % tf.text_brush_p % tf.background_brush_p;) 
      

    // draw backgrouhd and string in all DCs, as required

    if ( tf.draw_background && (tf.background_brush_p != nullptr) )
    {  
        M__(M_out(L"canvas_C::draw_str() -- doing fill_rectangle()");)
        rc = fill_rectangle(tf.background_brush_p, tf.bound_rect);
        M_checkrc_ret(rc, L"canvas_C::draw_str(): fill_rectangle()")
    }
    
    if (tf.draw_text)
    {
        rc = draw_string(str.c_str(), (int)(str.size()), fs.font_p, Gdiplus::PointF(0,0), tf.string_format_p, tf.text_brush_p);
        M_checkrc_ret(rc, L"canvas_C::draw_str(): draw_string()")
    }


    // reset transform back to normal, and return  
    // ------------------------------------------

    m_default_transform = saved_default_transform;              // restore setting
    rc = default_transform();
    M_checkrc_ret(rc, L"canvas_C::draw_str(): reset_transform()")

    return 0; 
}
M_endf


// ---------------------------------------------------------------------------------
// external form -- uses Gdiplus::Font and text_format_S imbedded in canvas_C area
// ---------------------------------------------------------------------------------

M_DRAW_IMPEXP
int  canvas_C::draw_str(const std::wstring& str) try
{
    return draw_str(str, m_font, *m_tf_p);   
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//|| 
//||       canvas_C::format_str()-- formats input std::wstring in passed-in rectangle (horizontal formatting only)  
//||       ======================   ------------------------------------------------- 
//||       
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  canvas_C::format_str(const std::wstring& str, const Gdiplus::RectF& rect, const Gdiplus::StringFormat *sf_p) try
{
    if (m_debug > 0)
        M_out(L"canvas_C::format_str()");  


    // make sure font and text_brush are set up
    // ----------------------------------------

    if (m_curr_font_p == nullptr)
    {
        M_out_emsg(L"canvas_C::format_str() -- current font is not set up in canvas_C area");
        return -1; 
    }

    if (m_text_br_p == nullptr)
    {
        M_out_emsg(L"canvas_C::format_str() -- current text brush is not set up in canvas_C area");
        return -1; 
    }


    // Draw string using passed-in Gdiplus::Rect and Gdiplus::StringFormat
    // -------------------------------------------------------------------

    auto d_rc = draw_string((const WCHAR *)(str.c_str()), (INT)(str.size()), m_curr_font_p, rect, sf_p, m_text_br_p);
    M_checkrc_ret(d_rc, L"canvas_C::format_str(): draw_string()")

    return 0; 
}
M_endf




// --------------------------------------------------------------------------------
// simplified form with passed-in top, left, width, height for formatting rectangle  -- string_format is defaulted
// --------------------------------------------------------------------------------

M_DRAW_IMPEXP
int  canvas_C::format_str(const std::wstring& str, rc_T top, rc_T left, rc_T width, rc_T height) try
{
    if (m_debug > 0)
        M_out(L"canvas_C::format_str()");  

    Gdiplus::RectF        rect { (Gdiplus::REAL)left, (Gdiplus::REAL)top, (Gdiplus::REAL)width, (Gdiplus::REAL)height };     // initialize formatting rectangle with caller's parms
    Gdiplus::StringFormat sf   {                                                                                      };     // use default string formatting within rectangle    


    // set up default center alignment (vertical and horizontal)
    // -------------------------------

    auto s_rc = sf.SetAlignment(Gdiplus::StringAlignmentCenter);
    M_checkrc_ret(s_rc, L"canvas_C::format_str(): SetAlignment()");

    auto sl_rc = sf.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    M_checkrc_ret(sl_rc, L"canvas_C::format_str(): SetLineAlignment()");


    // call regular format_str() function with Gdiplus::RectF and Gdiplus::StringFormat
    // --------------------------------------------------------------------------------

    return format_str(str, rect, &sf);
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||       canvas_C::measure_str()-- measures width of formatted string  (or character)
//||       =======================   ---------------------------------- 
//||       
//||       ignores any effect shear would have on the output length
//||
//||       - uses curr_x_scale field from passed-in text_format_S to compute the output_width 
//||       - does not compute transformed bounding polygon, etc. 
//||       - output_width and bounding rectangle do not include any extra spacing before the next output char
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  canvas_C::measure_str(const WCHAR *str_p, size_t len, const font_S& fs, text_format_S& tf, bool set_nonzero_rotation) try
{
    int rc;
    
    if (m_debug > 0)
    {
        M_out(L"canvas_C::measure_str(): called"                 )                       ;
        M_out(L"canvas_C::measure_str(): fs.font_p          = %p") % fs.font_p           ; 
        M_out(L"canvas_C::measure_str(): tf.string_format_p = %p") % tf.string_format_p  ; 
        M_out(L"canvas_C::measure_str(): m_gr_p[1]          = %p") % m_gr_p.at(0)        ; 
    }

    // if required, set non-zero rotation to get more consistent measurement    

    if (set_nonzero_rotation)
    {
       rc = reset_transform();
       M_checkrc_ret(rc, L"canvas_C::measure_str(): reset_transform()")

       rc = rotate_transform(0.001);
       M_checkrc_ret(rc, L"canvas_C::measure_str(): reset_transform()")   
    }


    // Measure width of string, and set output fields in text_format structure (note: only do measurement in 1st DC -- seems to work even when entirely out of bounds)
    // -----------------------------------------------------------------------
      
    Gdiplus::RectF rect_layout(0.0,  0.0, 50000.0, 50000.0);   // format inside this rectangle

    if (m_debug > 1)
    {
        M_out(L"canvas_C::measure_str(): str_p               = %p"       ) % str_p                   ;
        M_out(L"canvas_C::measure_str(): (int)len            = %d"       ) % (int)len                ;
        M_out(L"canvas_C::measure_str(): fs.font_p           = %p"       ) % fs.font_p               ;
        M_out(L"canvas_C::measure_str(): tf.string_format_p  = %p"       ) % tf.string_format_p      ;
        M_out(L"canvas_C::measure_str(): &(tf.bound_rect)    = %p"       ) % &(tf.bound_rect)        ;
    }

    m_gr_p.at(0)->MeasureString(str_p, (int)len, fs.font_p, rect_layout, tf.string_format_p, &(tf.bound_rect));
    M_checklsp_ret(m_gr_p.at(0), L"canvas_C::measure_str() : gr[1] MeasureString()")  
 
    if (
        (len > 0)
        &&
        (tf.bound_rect.Width <= 0.01 * fs.font_size)
       )
    {
        tf.output_width = fs.non_spacing_width * fs.font_size * tf.curr_x_scale;
    }
    else
    {
        tf.output_width = tf.bound_rect.Width * tf.curr_x_scale;
    }
    
    tf.extra_width = tf.curr_extra_space * fs.font_size * tf.curr_x_scale;  // curr_spacing should be 0.0 when exra space is ignored
    
 
    // if required, reset after using non-zero rotation    

    if (set_nonzero_rotation)
    {
       rc = reset_transform();
       M_checkrc_ret(rc, L"canvas_C::measure_str(): reset_transform()")
    }

    if (m_debug > 1)
    {
        M_out(L"canvas_C::measure_str(): gr[1]: scale=%.6f/%.6f    output_width=%.6f") % tf.curr_x_scale % tf.curr_y_scale % tf.output_width; 
        M_out(L"canvas_C::measure_str(): rect_out -- X=%.6f   Y=%.6f   Width=%.6f   Height=%.6f") % tf.bound_rect.X % tf.bound_rect.Y % tf.bound_rect.Width % tf.bound_rect.Height;
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
//||      canvas_C::draw_txt() -- draws all wchars in std::wstring (calls draw_chr() in a loop) 
//||      ====================      
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// internal form -- uses passed-in Gdiplus::Font and test_format_S
// ---------------------------------------------------------------

M_DRAW_IMPEXP
int  canvas_C::draw_txt(const std::wstring& str, const font_S& fs, text_format_S& tf) try
{
    int rc = 0; 

    // initialize current xform fields from initial xform fields

    tf.curr_x            = tf.x; 
    tf.curr_y            = tf.y;
    tf.curr_rotation     = tf.rotation; 
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

        rc = draw_chr(&(str.c_str()[i]), fs, tf);
        if (rc != 0)
           return rc; 

        tf.curr_x = tf.new_x; 
        tf.curr_y = tf.new_y;          
    }

    return rc; 
}
M_endf


// ---------------------------------------------------------------------------------
// external form -- uses Gdiplus::Font and text_format_S imbedded in canvas_C area
// ---------------------------------------------------------------------------------

M_DRAW_IMPEXP
int  canvas_C::draw_txt(const std::wstring& str) try
{
    return draw_txt(str, m_font, *m_tf_p);   
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||      canvas_C::draw_chr() -- draws one WCHAR (called in a loop by draw_txt())  
//||      ====================      
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// internal form -- uses passed-in Gdiplus::Font and test_format_S
// ---------------------------------------------------------------

M_DRAW_IMPEXP
int  canvas_C::draw_chr(const WCHAR *p, const font_S& fs, text_format_S& tf) try
{
    int rc; 

    // compute adjusted x and y values to to reposition centerline at transform origin 

    Gdiplus::REAL adj_x  =   fs.center_adjust * M_sind(tf.curr_rotation)  * tf.curr_y_scale; 
    Gdiplus::REAL adj_y  = -(fs.center_adjust * M_cosd(tf.curr_rotation)) * tf.curr_y_scale; 
    

    // set up transform, as required (to put "center line" of text at caller's position) 

    bool saved_default_transform = m_default_transform;     // save setting
    m_default_transform = false;                            // don't re-default transform on every call

    init_transform_s(tf.curr_transform, tf.curr_x + adj_x, tf.curr_y + adj_y, tf.curr_rotation, tf.curr_x_scale, tf.curr_y_scale, tf.curr_x_shear, tf.curr_y_shear);  

    rc = reset_transform();
    M_checkrc_ret(rc, L"canvas_C::draw_chr(): reset_transform()")
    rc = set_transform(tf.curr_transform);
    M_checkrc_ret(rc, L"canvas_C::draw_chr(): set_transform()")

           
    // measure width of character, and set output fields in text_format structure (note: only do measurement in 1st DC -- seems to work even when entirely out of bounds)
    // --------------------------------------------------------------------------
    
    measure_str(p, 1, fs, tf);                          // sets tf.output_width field
    text_new_xy(tf, tf.curr_rotation);                  // update new_x and new_y fields based on cur_rotation -- should include any extra space
          
    M__(M_out(L"canvas_C::draw_chr(): gr1: x=%.6f   y=%.6f    rot=%.6f    scale=%.6f/%.6f    width=%.6f    new_x=%.6f   new_y=%.6f")
              % tf.curr_x % tf.curr_y % tf.curr_rotation % tf.curr_x_scale % tf.curr_y_sscale % tf.output_width % tf.new_x % tf.new_y;)  

    M__(M_out(L"canvas_C::draw_chr(): rect_out -- X=%.6f   Y=%.6f   Width=%.6f   Height=%.6f") % rect_out.X % rect_out.Y % rect_out.Width % rect_out.Height;)

    tf.bounds_poly[0] = transform_point(tf.bound_rect.X                         , tf.bound_rect.Y                        , tf.curr_transform);
    tf.bounds_poly[1] = transform_point(tf.bound_rect.X  + tf.bound_rect.Width  , tf.bound_rect.Y                        , tf.curr_transform);
    tf.bounds_poly[2] = transform_point(tf.bound_rect.X  + tf.bound_rect.Width  , tf.bound_rect.Y + tf.bound_rect.Height , tf.curr_transform);
    tf.bounds_poly[3] = transform_point(tf.bound_rect.X                         , tf.bound_rect.Y + tf.bound_rect.Height , tf.curr_transform);

    M__(M_out(L"canvas_C::draw_chr(): bounds_poly[0] =  %.6f, %.6f") % tf.bounds_poly[0].X % tf.bounds_poly[0].Y;)
    M__(M_out(L"canvas_C::draw_chr(): bounds_poly[1] =  %.6f, %.6f") % tf.bounds_poly[1].X % tf.bounds_poly[1].Y;)
    M__(M_out(L"canvas_C::draw_chr(): bounds_poly[2] =  %.6f, %.6f") % tf.bounds_poly[2].X % tf.bounds_poly[2].Y;)
    M__(M_out(L"canvas_C::draw_chr(): bounds_poly[3] =  %.6f, %.6f") % tf.bounds_poly[3].X % tf.bounds_poly[3].Y;)


    // draw background and 1 character in both DCs, as required

    if ( tf.draw_background && (tf.background_brush_p != nullptr) )
    {    
        rc = fill_rectangle(tf.background_brush_p, tf.bound_rect);
        M_checkrc_ret(rc, L"canvas_C::draw_chr(): fill_rectangle()")
    }
    
    if (tf.draw_text)
    {
        rc = draw_string(p, 1, fs.font_p, Gdiplus::PointF(0,0), tf.string_format_p, tf.text_brush_p);
        M_checkrc_ret(rc, L"canvas_C::draw_chr(): draw_string()")
    }
    

    // reset transform back to normal, and return  
    // ------------------------------------------

    m_default_transform = saved_default_transform;              // restore setting
    rc = default_transform();
    M_checkrc_ret(rc, L"canvas_C::draw_chr(): reset_transform()")
  
    return 0; 
}
M_endf


// ---------------------------------------------------------------------------------
// external form -- uses Gdiplus::Font and text_format_S imbedded in canvas_C area
// ---------------------------------------------------------------------------------

M_DRAW_IMPEXP
int canvas_C::draw_chr(const WCHAR *p) try
{
    return draw_chr(p, m_font, *m_tf_p);   
}
M_endf




//__________________________________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|           ====================================================================
//\/\/\/\|           Canvas_C-related text/string format and font setup/display functions  
//\/\/\/\|           ====================================================================
//\/\/\/\|
//\/\/\/\|
//\/\/\/\|
//\/\/\/\\__________________________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::set_font()  -- updates imbedded font_S in canvas_C and Gdiplus::Font object achored in canvas_C area  
//||       ====================  
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::set_font(const std::wstring& family_name, Gdiplus::REAL em_size, INT style, bool installed) try
{
    // replace current Gdiplus::Font anchored in canvas_C with a new one based on caller's parms

    delete m_curr_font_p; 


    // create font from private collection or installed font collection
    // ----------------------------------------------------------------

    if (installed)
       m_curr_font_p = new Gdiplus::Font(family_name.c_str(), em_size, style, Gdiplus::UnitPixel);
    else
       m_curr_font_p = new Gdiplus::Font(family_name.c_str(), em_size, style, Gdiplus::UnitPixel, m_pfc_p);


    // initialize font_S object embedded in canvas_C are based on newly-updated Gdiplus::FONT object anchored in canvas_C area 

    auto i_rc = init_font(installed); 

    if (i_rc != 0)
    {
        delete m_curr_font_p;             // get rid of unusable Gdiplus::Font object
        m_curr_font_p = nullptr; 
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
//||       canvas_C::set_font_size()  -- updates size of imbedded font_S in canvas_C and Gdiplus::Font achored in canvas_C area  
//||       =========================  
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::set_font_size(Gdiplus::REAL em_size) try
{
    // Error, if no current font is set up in canvas_C
    // -------------------------------------------------

    if (m_curr_font_p == nullptr)
    {
        M_out_emsg(L"canvas_C::set_font_size() -- No current font set up -- cannot change size");
        return -1; 
    }   


    // get family name, em_size/unit, and style from current font in canvas_C  (for use when creating the new font)
    // ------------------------------------------------------------------------

    std::wstring   family_name   { m_font.family_name };
    INT            font_style    { m_font.font_style  };
    Gdiplus::Unit  font_unit     { m_font.font_unit   };
    bool           installed     { m_font.installed   };


    // replace current Gdiplus::Font anchored in canvas_C with a new one with different em_size
    // ------------------------------------------------------------------------------------------

    delete m_curr_font_p; 


    // create font from private collection or installed font collection

    if (installed)
       m_curr_font_p = new Gdiplus::Font(family_name.c_str(), em_size, font_style, font_unit         );
    else
       m_curr_font_p = new Gdiplus::Font(family_name.c_str(), em_size, font_style, font_unit, m_pfc_p);


    // initialize font_S object embedded in canvas_C are based on newly-updated Gdiplus::FONT object anchored in canvas_C area 

    auto i_rc = init_font(installed); 

    if (i_rc != 0)
    {
        delete m_curr_font_p;             // get rid of unusable Gdiplus::Font object
        m_curr_font_p = nullptr; 
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
//||       canvas_C::set_text_color() -- update current text brush with new color   
//||       ==========================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// version without alpha 

M_DRAW_IMPEXP
int canvas_C::set_text_color(const rgb_S& color) try
{
    auto rc = m_text_br_p->SetColor(Gdiplus::Color(color.r, color.g, color.b)); 
    // hopefully brush doesn't move, so that pointer in *m_tf_p doesn't need updating
    return (int)rc; 
}
M_endf


// version with alpha

M_DRAW_IMPEXP
int canvas_C::set_text_color(const rgb_S& color, alpha_T alpha) try
{
    auto rc = m_text_br_p->SetColor(Gdiplus::Color(alpha, color.r, color.g, color.b)); 
    // hopefully brush doesn't move, so that pointer in *m_tf_p doesn't need updating
    return (int)rc; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::set_no_text_background() -- get rid of text background brush (prevents drawing text background boxes)   
//||       ==================================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::set_no_text_background() try
{
    // make sure background brush is not allocated
    // -------------------------------------------

    if (m_text_background_br_p != nullptr)
    {
        delete m_text_background_br_p; 
        m_text_background_br_p = nullptr;   
    }


    // make sure background brush in text_format_S matches updated background brush in the canvas_C area 
    // ---------------------------------------------------------------------------------------------------

    m_tf_p->background_brush_p = m_text_background_br_p;                                
    m_tf_p->draw_background = false;          

    return 0; 
}
M_endf
 


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::set_text_background_color() -- update current text brush with new color   
//||       =====================================
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
// version without alpha 

M_DRAW_IMPEXP
int canvas_C::set_text_background_color(const rgb_S& color) try
{
    return set_text_background_color(color, (alpha_T)255U);      // just call version with alpha=255 (opaque)
}
M_endf


// version with alpha 

M_DRAW_IMPEXP
int canvas_C::set_text_background_color(const rgb_S& color, alpha_T alpha) try
{
    M__(M_out(L"canvas_C::set_text_background_color(): a r g b = %d %d %d %d") % alpha % color.r % color.g % color.b;) 


    // make sure background brush is allocated   
    // ---------------------------------------

    if (m_text_background_br_p == nullptr)
        m_text_background_br_p = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 255));    // use default colors for now 


    // make sure background brush in text_format_S matches updated background brush in the canvas_C area 
    // ---------------------------------------------------------------------------------------------------

    m_tf_p->background_brush_p = m_text_background_br_p;                                // set to background brush just anchored in canvas_C area 

    M__(M_out(L"canvas_C::set_text_background_color(): tf.background_brush_p = %p   a/r/g/b = %d/%d/%d/%d") % m_tf_p->background_brush_p % alpha % color.r % color.g % color.b;) 
    
    auto rc = m_text_background_br_p->SetColor(Gdiplus::Color(alpha, color.r, color.g, color.b));
    m_tf_p->draw_background = true;     

    M__(M_out(L"canvas_C::set_text_background_color() -- tf.draw_background = %d") % ((int32_t)(m_tf_p->draw_background));)

    // hopefully brush doesn't move, so that pointer in text_format doesn't need updating

    return (int)rc; 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       text_format_S and font_S functions that affect default font_S and text_format_S already in canvas_C  
//||       ====================================================================================================  
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// set up current text_format_S anchored in canvas_C structure 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int  canvas_C::set_text_format(INT x, INT y, Gdiplus::REAL rotation, Gdiplus::REAL x_scale, Gdiplus::REAL y_scale, Gdiplus::REAL x_shear, Gdiplus::REAL y_shear, Gdiplus::REAL extra_space) try
{
    return ::set_text_format(*m_tf_p, x, y, rotation, x_scale, y_scale, x_shear, y_shear, extra_space);  
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////   set up Gdiplus::StringFormat text_fmt_S that is anchored in the canvas_C area -- default is to use Gdiplus::StringFormat anchored in the canvas_C area
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int  canvas_C::set_text_strfmt(Gdiplus::StringFormat *sf_p) try
{
    if (m_sf_p == nullptr)
        return ::set_text_strfmt(*m_tf_p, m_sf_p);
    else
        return ::set_text_strfmt(*m_tf_p,   sf_p);     // use passed-in Gdiplus::StringFormat
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////   initialize font_S in canvas_C from current Gdiplus::Font object anchored in canvas_C
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int  canvas_C::init_font(bool installed) try
{
    return ::init_font(m_font, *m_curr_font_p, installed); 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::remove_fonts()  -- remove all fonts in the private font collection anchored in canvas_C  
//||       ========================  
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::remove_fonts() try 
{
    // replace current Gdiplus::FontCollection with an empty one

    delete m_pfc_p; 
    m_pfc_p = new Gdiplus::PrivateFontCollection(); 
    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::add_font_file()  -- add font to the private font collection anchored in canvas_C  
//||       =========================  
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::add_font_file(const std::wstring& filename) try 
{
    // add caller's font file to private font collection

    M__(M_out(L"canvas_C::add_font_file(): filename = \"%S\"") % filename;)

    auto sts = m_pfc_p->AddFontFile(filename.c_str()); 
    M_checks_ret(sts, L"canvas_C::add_font_file() -- Adding font")
    return 0; 
}
M_endf   


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::add_font_folder()  -- add all fonts in folder to the private font collection anchored in canvas_C  
//||       ===========================  
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::add_font_folder(const std::wstring& folder_name, bool nested) try 
{
    std::vector<std::wstring> pathnames {  };
    

    // Get list of all files in caller's directory -- all are supposed to be font files -- return with error if none
    // -------------------------------------------------------------------------------------------------------------
    //
    // note: incoming folder_name should have trailing "\" already 


    auto f_rc = ::find_files(folder_name, pathnames, nested);

    if ( (f_rc != 0) || (pathnames.size() <= 0) )
    {
       M_out_emsg(L"canvas_C::add_font_folder(): find_files() did not find any files in folder = \"%S\"") % folder_name;   
       return -1;                                      
    }


    // Add each font file in list to private font collection (exit if there is any error while adding fonts)
    // -----------------------------------------------------

    for (auto pathname : pathnames)
    {
        auto a_rc = add_font_file(pathname);  
        if (a_rc != 0)
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
//||       canvas_C::display_fonts()  -- display list of installed fonts, or private fonts currently in canvas_C  
//||       =========================  
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::display_fonts(bool installed) try 
{
    // get vector of family names (either installed or private)
    // --------------------------------------------------------

    std::vector<std::wstring> family_names {}; 

    auto g_rc = get_family_names(installed, family_names);
    if (g_rc != 0)
       return -1;     // should have error message already
   

    // Display font family names
    // -------------------------

    int32_t ix {0};  

    for (auto family_name : family_names)
    {
        M_out("canvas_C::display_fonts(): family_name[%3d] = «%S»") % ix++ % family_name;    
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
//||       canvas_C::get_family_count() -- pass back number of font families in the installed fonts or private font collection anchored in canvas_C  
//||       ============================  
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::get_family_count(bool installed, int32_t& family_count) try 
{
    Gdiplus::FontCollection          *fc_p                          {nullptr}; 
    Gdiplus::InstalledFontCollection  installed_font_collection     { } ; 
   

    // Set up to look at PrivateFontCollection (in canvas_C) or InstalledFontCollection
    // ----------------------------------------------------------------------------------

    if (installed)
        fc_p = &installed_font_collection; 
    else
        fc_p = m_pfc_p;  


    // Error, if no private font family (should not happen?)   
    // --------------------------------                

    if (fc_p == nullptr)
    {
        M_out_emsg(L"canvas_C::get_family_count(): no font collection pointer in canvas_C");
        return -1; 
    }
     

    // Determine count of font families in collection (0 is OK here)  
    // ----------------------------------------------                          

    family_count = (int32_t)(fc_p->GetFamilyCount());
    M__(M_out(L"canvas_C::get_family_count(): family_count = %d") % family_count;) 
       
    return 0; 
}
M_endf

 
//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       canvas_C::get_family_names()  -- pass back vector with all font family names in the installed fonts or private font collection in canvas_C   
//||       ============================             
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int canvas_C::get_family_names(bool installed, std::vector<std::wstring>& family_names) try 
{
    Gdiplus::FontCollection          *fc_p                          {nullptr}; 
    Gdiplus::InstalledFontCollection  installed_font_collection     { } ; 
    int32_t                           family_count                  {0} ; 
    INT                               families_found                {0} ; 


    // Set up to look at PrivateFontCollection (in canvas_C) or InstalledFontCollection
    // ----------------------------------------------------------------------------------

    if (installed)
        fc_p = &installed_font_collection; 
    else
        fc_p = m_pfc_p;  


    // Determine count of font families in collection  (return error, if this fails, or if there are no fonts in the collection)
    // -----------------------------------------------                          

    auto g_rc = get_family_count(installed, family_count);
    if (g_rc != 0)
       return -1;     // should have error message already
           
    if (family_count == 0)
    {
        M_out_emsg(L"canvas_C::get_families(): get_family_count() returned no font families in font collection"); 
        return -1;
    }
   

    // Get array of font families in the font collection
    // -------------------------------------------------

    Gdiplus::FontFamily* font_families_p = new Gdiplus::FontFamily[(INT)family_count];

    fc_p->GetFamilies(family_count, font_families_p, &families_found);       

    M__(M_out(L"canvas_C::get_families(): families_found = %d") % families_found;) 

    if (families_found == 0)
    {
        M_out_emsg(L"canvas_C::get_families(): Gdiplus::GetFamilies() returned no font families in font collection"); 
        return -1;
    }


    // Build output vector of font family names
    // ----------------------------------------

    WCHAR family_name[LF_FACESIZE] {};

    for (auto i = 0; i < families_found; i++)
    {
        font_families_p[i].GetFamilyName(family_name);    
    
        M__(M_out("canvas_C::get_families(): family_name[%3d] = «%S»") % i % std::wstring(family_name);)

        family_names.push_back(std::wstring{family_name} );
    }  

    return 0; 
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
//||       init_text_format()   -- initialize passed-in text_format_S area
//||       ==================  
//||
//||          -- starting point always set to 0,0 
//||          -- acquires no resources (no need for term() function) 
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  init_text_format(text_format_S& tf, Gdiplus::SolidBrush *text_brush_p, Gdiplus::SolidBrush *background_brush_p, Gdiplus::StringFormat *m_sf_p) try
{
    tf.draw_text         = true; 
  
    tf.exit_p            = nullptr;          // can be supplied later 
    tf.exit_parm         = nullptr; 


    // set initial formatting fields to default values
    
    set_text_format(tf);


    // set interim fields fields to default values

    tf.curr_x            = 0.0; 
    tf.curr_y            = 0.0; 
    tf.curr_rotation     = 0.0;
    tf.curr_x_scale      = 1.0; 
    tf.curr_y_scale      = 1.0; 
    tf.curr_x_shear      = 0.0; 
    tf.curr_y_shear      = 0.0; 
    tf.new_x             = 0.0;
    tf.new_y             = 0.0;


    // use passed-in text foreground/background brush, and Gdiplus::StringFormat for now -- user can update these later

    tf.text_brush_p       = text_brush_p;                                      // set to passed-in text brush (non-owning pointer) 
    tf.background_brush_p = background_brush_p;                                // set to passed-in text brush (non-owning pointer) 

    if (tf.background_brush_p == nullptr)
        tf.draw_background = false; 
    else
        tf.draw_background = false; 


    set_text_strfmt(tf, m_sf_p);                                                 // set to passed-in  Gdiplus::StringFormat -- can be updated later

    
    // output fields, bounding rectangles, and bounding polygons are not filled in here

    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       set_text_format() -- set starting text format values from caller's parms 
//||       ================= 
//||                            (does not set brushes, StringFormat, etc.)
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  set_text_format(text_format_S& tf, INT x, INT y, Gdiplus::REAL rotation, Gdiplus::REAL x_scale, Gdiplus::REAL y_scale, Gdiplus::REAL x_shear, Gdiplus::REAL y_shear, Gdiplus::REAL extra_space) try
{
    tf.x                 = (Gdiplus::REAL)x; 
    tf.y                 = (Gdiplus::REAL)y;
    tf.rotation          = rotation;
    tf.x_scale           = x_scale;
    tf.y_scale           = y_scale;
    tf.x_shear           = x_shear;
    tf.y_shear           = y_shear;
    tf.extra_space       = extra_space;     
 

     // brushes, StringFormat exit stuff, output fields, bounding rectangles, and bounding polygons are not filled in here

    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       set_text_strfmt() -- setup Gdiplus::StringFormat in passed-in text_format_S area 
//||       ================= 
//||                           
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  set_text_strfmt(text_format_S& tf, Gdiplus::StringFormat *m_sf_p) try
{
    // If caller did not supply a StringFormat parm, default GenericTypographic() cannot be anchored here -- so report error

    if (m_sf_p == nullptr)
    {
        M_out_emsg(L"set_text_strfmt(): no Gdiplus::StringFormat passed in");
        return -1; 
    }
    else
    {
        tf.string_format_p = m_sf_p; 
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
//||       copy_text_format()  
//||       ================== 
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  copy_text_format(text_format_S& tf, const text_format_S& tf_old) try
{
    tf.x                  = tf_old.x; 
    tf.y                  = tf_old.y;
    tf.rotation           = tf_old.rotation;
    tf.x_scale            = tf_old.x_scale;
    tf.y_scale            = tf_old.y_scale;
    tf.x_shear            = tf_old.x_shear;
    tf.y_shear            = tf_old.y_shear;
    tf.extra_space        = tf_old.extra_space;   
    tf.draw_text          = tf_old.draw_text; 
    tf.draw_background    = tf_old.draw_background;  

    tf.text_brush_p       = tf_old.text_brush_p;             // non-owning pointer
    tf.background_brush_p = tf_old.background_brush_p;       // non-owning pointer 
    tf.string_format_p    = tf_old.string_format_p;          // non-owning pointer
    tf.exit_p             = tf_old.exit_p; 
    tf.exit_parm          = tf_old.exit_parm;

    tf.curr_x             = tf_old.curr_x; 
    tf.curr_y             = tf_old.curr_y; 
    tf.curr_rotation      = tf_old.curr_rotation;
    tf.curr_x_scale       = tf_old.curr_x_scale; 
    tf.curr_y_scale       = tf_old.curr_y_scale; 
    tf.curr_x_shear       = tf_old.curr_x_shear; 
    tf.curr_y_shear       = tf_old.curr_y_shear; 
          
    
    // output fields do not get copied here

    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||       text_new_xy() -- updates new_x and new_y fields in passed-in text_format_S 
//||       =============    --------------------------------------------------------- 
//||       
//||         note: always assumes extra_width is valid -- extra_width should be 0.0 when using draw_str()   
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
void  text_new_xy(text_format_S& tf, Gdiplus::REAL rotation) try
{
    tf.new_x = tf.curr_x + M_cosd(rotation) * (tf.output_width + tf.extra_width); 
 
    tf.new_y = tf.curr_y + M_sind(rotation) * (tf.output_width + tf.extra_width);

    return;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||       transform_point() -- transform X,Y values to new coordinates 
//||       =================    --------------------------------------- 
//||
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
Gdiplus::PointF transform_point(const Gdiplus::PointF& pt, Gdiplus::REAL tx, Gdiplus::REAL ty, Gdiplus::REAL rotation, Gdiplus::REAL x_scale, Gdiplus::REAL y_scale) try
{
    Gdiplus::PointF ptx; 


    // note: scaling is applied first, then rotation, finally translation

    ptx.X = tx + M_cosd(rotation) * (pt.X * x_scale) - M_sind(rotation) * (pt.Y * y_scale);   
    ptx.Y = ty + M_cosd(rotation) * (pt.Y * y_scale) + M_sind(rotation) * (pt.X * x_scale);   
    
    return ptx; 
}
M_endf



M_DRAW_IMPEXP
Gdiplus::PointF transform_point(Gdiplus::REAL X, Gdiplus::REAL Y, Gdiplus::REAL tx, Gdiplus::REAL ty, Gdiplus::REAL rotation, Gdiplus::REAL x_scale, Gdiplus::REAL y_scale) try
{
    Gdiplus::PointF ptx; 


    // note: scaling is applied first, then rotation, finally translation

    ptx.X = tx + M_cosd(rotation) * (X * x_scale) - M_sind(rotation) * (Y * y_scale);   
    ptx.Y = ty + M_cosd(rotation) * (Y * y_scale) + M_sind(rotation) * (X * x_scale);   
    
    return ptx; 
}
M_endf


M_DRAW_IMPEXP
Gdiplus::PointF transform_point(const Gdiplus::PointF& pt, const transform_S& tr) try
{
    Gdiplus::PointF ptx(pt); 

    // note: only use 1st matrix to get absolute 0-based point coordinates

    tr.matrix_p.at(0)->TransformPoints(&ptx, 1);    
    
    return ptx; 
}
M_endf



M_DRAW_IMPEXP
Gdiplus::PointF transform_point(Gdiplus::REAL X, Gdiplus::REAL Y, const transform_S& tr) try
{
    Gdiplus::PointF ptx(X, Y); 

   // note: only use 1st matrix to get absolute 0-based point coordinates

    tr.matrix_p.at(0)->TransformPoints(&ptx, 1);  
    
    return ptx; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       init_font() -- initialized passed-in font_S area   
//||       ===========  
//||
//||          -- acquires no resources (no need for term() function)
//||          -- contains non-owning pointer to caller's font
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  init_font(font_S& fs, const Gdiplus::Font& f, bool installed) try
{
    Gdiplus::Status     sts; 
    Gdiplus::FontFamily ff;
    WCHAR               family_name[LF_FACESIZE];

    if (f.IsAvailable() == false)
    {
        M_out_emsg(L"init_font(): Error -- Input Font is not available");
        return -1;    
    }

    fs.font_p = &f;                          // non-owning pointer -- do not use delete() on this pointer


    sts = f.GetFamily(&ff);

    if (sts != 0)
    {
        M_out_emsg(L"init_font(): Error -- Status from GetFamily() = %d") % sts;
        return -1;    
    }
      
    if (ff.IsAvailable() == false)
    {
        M_out_emsg(L"init_font(): Error -- input FontFamily is not available");
        return -1;    
    }

    sts = ff.GetFamilyName(family_name);

    if (sts != 0)
    {
        M_out_emsg(L"init_font(): Error -- Status from GetFamilyName() = %d") % sts;
        return -1;    
    }
    

    fs.family_name       = std::wstring(family_name);
    fs.installed         = installed; 
    fs.font_style        = f.GetStyle();
    fs.font_unit         = f.GetUnit();
    fs.font_size         = f.GetSize();
    fs.font_height       = f.GetHeight(1.0);  // Assume UnitPixel is being used
                        
    fs.em_height         = ff.GetEmHeight(fs.font_style); 

    Gdiplus::REAL factor = fs.font_size / (Gdiplus::REAL)(fs.em_height);     // conversion factor: "design units" to pixels

    fs.line_spacing  = factor * ff.GetLineSpacing(fs.font_style);              
    fs.cell_ascent   = factor * ff.GetCellAscent(fs.font_style); 
    fs.cell_descent  = factor * ff.GetCellDescent(fs.font_style);
     
    fs.cell_height   = fs.cell_ascent + fs.cell_descent;  
    fs.leading       = fs.cell_height - fs.font_size; 

    fs.non_spacing_width = 0.25;     // hard-coded, for now 

 
    M__(M_out("init_font(): family_name    = \"%S\""       ) % fs.family_name           ;)
    M__(M_out("init_font(): installed      = %s"           ) % M_bool_cstr(fs.installed);)
    M__(M_out("init_font(): font_style     = %d"           ) % fs.font_style            ;)
    M__(M_out("init_font(): font_unit      = %d"           ) % fs.font_unit             ;)
    M__(M_out("init_font(): font_size      = %.6f pixels"  ) % fs.font_size             ;)
    M__(M_out("init_font(): font_height    = %.6f pixels"  ) % fs.font_height           ;)
    M__(M_out("init_font(): em_height      = %d"           ) % fs.em_height             ;)
    M__(M_out("init_font(): line_spacing   = %.6f pixels"  ) % fs.line_spacing          ;)
    M__(M_out("init_font(): cell_ascent    = %.6f pixels"  ) % fs.cell_ascent           ;)
    M__(M_out("init_font(): cell_descent   = %.6f pixels"  ) % fs.cell_descent          ;)
    M__(M_out("init_font(): cell_height    = %.6f pixels"  ) % fs.cell_height           ;)
    M__(M_out("init_font(): leading        = %.6f pixels"  ) % fs.leading               ;)
 
                                                                                           
   
    //  Compute text line adjustment -- top of line to "center of line"
    //  ---------------------------------------------------------------
    //
    //   This is the average of 1/2 of cell height  and  1/2 of leading + cell ascender
    //
    //

    fs.center_adjust = ( (0.5 * fs.cell_height) + (0.5 * (fs.leading + fs.cell_ascent)) ) / 2.0 ;
    
    
    M__(M_out("init_font(): center_adjust  = %.6f pixels"  ) % fs.center_adjust;) 
    return 0;  
}
M_endf





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||      Functions to add geometric items to a GDI+ graphics path  
//||      --------------------------------------------------------
//|| 
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   ===========
//   add_lines_r -- add in regularly-spaced radial lines out to radius (single radius) -- needs to be widened by caller before drawing
//   ===========
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int add_lines_r(Gdiplus::GraphicsPath& gp, int32_t n, real_T radius, real_T rotd, real_T x, real_T y) try
{  
    Gdiplus::REAL x2;
    Gdiplus::REAL y2;

    real_T rot_step = 360.0 / (real_T)n;  

    if (n < 1)
    {
        M_out_emsg(L"add_lines_r() : number of points = %d -- min allowed = 1") % n;
        return -1;
    }

     
    if (( n % 2) == 0)
    {
         // handle n = even -- draw every other spoke, reflected around origin

        for (int i = 0; i < n/2; i++)
        {
            x2 = radius * M_cosd(rotd + (real_T)i * rot_step);
            y2 = radius * M_sind(rotd + (real_T)i * rot_step); 
            gp.StartFigure();                                                              // make sure this line starts a new figure in the path
            gp.AddLine((Gdiplus::REAL)(x - x2), (Gdiplus::REAL)(y - y2), (Gdiplus::REAL)(x + x2), (Gdiplus::REAL)(y + y2));
        }
    
    }
    else
    {
        // handle n = odd -- draw each spoke individually

        for (int i = 0; i < n; i++)
        {
            x2 = radius * M_cosd(rotd + (real_T)i * rot_step);
            y2 = radius * M_sind(rotd + (real_T)i * rot_step); 
            gp.StartFigure();                 // make sure this line starts a new figure in the path
            gp.AddLine((Gdiplus::REAL)x, (Gdiplus::REAL)y, (Gdiplus::REAL)(x + x2), (Gdiplus::REAL)(y + y2));
        }
    }

    gp.StartFigure();                     // make sure next item added is in a separate figure in the path

    return 0; 
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   =============
//   add_polygon_r -- add in regular polygon (single radius)
//   =============
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int add_polygon_r(Gdiplus::GraphicsPath& gp, int32_t n, real_T radius, real_T rotd, real_T x, real_T y) try
{
    if (n < 3)
    {
        M_out_emsg(L"add_polygon_r() : number of points = %d -- min allowed = 3") % n;
        return -1;
    }
    
    if (n > 100)
    {
        M_out_emsg(L"add_polygon_r() : number of points = %d -- max allowed = 100") % n;
        return -1;
    }

    Gdiplus::PointF pts[101];

    real_T rot_step = 360.0 / (real_T)n;  

    for (int i = 0; i < n; i++)
    {
        pts[i].X = x + radius * M_cosd(rotd + (real_T)i * rot_step);
        pts[i].Y = y + radius * M_sind(rotd + (real_T)i * rot_step);    
    }

    gp.AddPolygon(pts, n);


    return 0; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   ==============
//   add_polygon_r2 -- add in star-like polygon (double radius -- alternating points on each radius)
//   ==============     
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

M_DRAW_IMPEXP
int add_polygon_r2(Gdiplus::GraphicsPath& gp, int32_t n, real_T radius1, real_T radius2, real_T rotd, real_T x, real_T y) try
{
    if (n < 3)
    {
        M_out_emsg(L"add_polygon_r() : number of points = %d -- min allowed = 3") % n;
        return -1;
    }
    
    if (n > 100)
    {
        M_out_emsg(L"add_polygon_r() : number of points = %d -- max allowed = 100") % n;
        return -1;
    }

    Gdiplus::PointF pts[101];
    
    real_T rot_step = 360.0 / (real_T)n;  
    real_T radius; 

    for (int i = 0; i < n; i++)
    {
        if (i % 2 == 0)
            radius = radius1; 
        else
            radius = radius2; 


        pts[i].X = x + radius * M_cosd(rotd + (real_T)i * rot_step);
        pts[i].Y = y + radius * M_sind(rotd + (real_T)i * rot_step);    
    }

    gp.AddPolygon(pts, n);


    return 0; 
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
//||       bmp24_headers() -- fill in headers for 24-bit uncompressed RGB .bmp file
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  bmp24_headers( BITMAPFILEHEADER  *fh_p 
                  , BITMAPINFOHEADER *ih_p 
                  , int32_t           columns
                  , int32_t           rows
                  ) 
try
{
    size_t   hdr_size;
    size_t   pixel_size;
    uint64_t requested_size;

    hdr_size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    M__(M_dmsg("BMP columns = %d", columns)) 
    M__(M_dmsg("BMP rows    = %d", rows))  

    M__(M_dmsg("BMP hdr size = %u", (uint32_t)hdr_size))

    requested_size = M_roundup_p(3ULL * (uint64_t)columns, 4ULL) * (uint64_t)M_iabs(rows);

    M_out( L"Output  : .bmp file size -- %d x %d = %llu RGB bytes (%.3f MB total)")
         % columns
         % rows
         % requested_size
         % ((real_T)(requested_size + (uint64_t)hdr_size) / 1048576.0)
         ;

    if (requested_size > (uint64_t)UINT_MAX - (uint64_t)hdr_size)
    {
        M_out_emsg(L"Requested .BMP file size = %llu -- too long") %  requested_size;
        return -1;
    }

    pixel_size = (size_t)(requested_size);


    fh_p->bfType            = 0x4d42;                                 // "BM" 
    fh_p->bfSize            = (DWORD)(hdr_size + pixel_size); 
    fh_p->bfReserved1       = 0;
    fh_p->bfReserved2       = 0;
    fh_p->bfOffBits         = (DWORD)hdr_size;

    ih_p->biSize            = (DWORD)sizeof(BITMAPINFOHEADER);
    ih_p->biWidth           = (LONG)columns;
    ih_p->biHeight          = (LONG)rows;
    ih_p->biPlanes          = (WORD)1;
    ih_p->biBitCount        = (WORD)24;
    ih_p->biCompression     = (DWORD)BI_RGB;
    ih_p->biSizeImage       = (DWORD)pixel_size;
    ih_p->biXPelsPerMeter   = (LONG)5000;
    ih_p->biYPelsPerMeter   = (LONG)5000;
    ih_p->biClrUsed         = (DWORD)0;
    ih_p->biClrImportant    = (DWORD)0;

    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       bmp8_headers() -- fill in headers for 8-bit uncompressed RGB .bmp file
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  bmp8_headers( BITMAPFILEHEADER  *fh_p 
                 , BITMAPINFOHEADER *ih_p 
                 , int32_t           columns
                 , int32_t           rows
                 , int32_t           colors
                 ) 
try
{
    size_t   hdr_size;
    size_t   pixel_size;
    size_t   color_table_size;
    uint64_t requested_size;

    hdr_size         = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    color_table_size = colors * sizeof RGBQUAD;

    M__(M_dmsg("BMP columns = %d", columns)) 
    M__(M_dmsg("BMP rows    = %d", rows))  
    M__(M_dmsg("BMP colors  = %d", colors))  
   
    M__(M_dmsg("BMP hdr size         = %u", (uint32_t)hdr_size))
    M__(M_dmsg("BMP color table size = %u", (uint32_t)color_table_size))

    requested_size = M_roundup_p((uint64_t)columns, 4ULL) * (uint64_t)M_iabs(rows);

    M_out( L"Output  : .bmp file size -- %d x %d = %llu index bytes (%.3f MB total) -- colors = %d")
         % columns
         % rows
         % requested_size 
         % ((real_T)(requested_size + (uint64_t)hdr_size + (uint64_t)color_table_size) / 1048576.0)
         % colors
         ;
    
    if (requested_size > (uint64_t)UINT_MAX - (uint64_t)hdr_size - color_table_size)
    {
        M_out_emsg(L"Requested .BMP file size = %llu -- too long") % requested_size;
        return -1;
    }

    if (colors > 256)
    {
        M_out_emsg(L"Color table entries > 256 for 8-bit bitmap -- colors=%d") % colors;
        return -1;
    }

    pixel_size = (size_t)(requested_size);


    fh_p->bfType            = 0x4d42;                                                 // "BM" 
    fh_p->bfSize            = (DWORD)(hdr_size + color_table_size + pixel_size); 
    fh_p->bfReserved1       = 0;
    fh_p->bfReserved2       = 0;
    fh_p->bfOffBits         = (DWORD)hdr_size + (DWORD)color_table_size;

    ih_p->biSize            = (DWORD)sizeof(BITMAPINFOHEADER);
    ih_p->biWidth           = (LONG)columns;
    ih_p->biHeight          = (LONG)rows;
    ih_p->biPlanes          = (WORD)1;
    ih_p->biBitCount        = (WORD)8;
    ih_p->biCompression     = (DWORD)BI_RGB;
    ih_p->biSizeImage       = (DWORD)pixel_size;
    ih_p->biXPelsPerMeter   = (LONG)5000;
    ih_p->biYPelsPerMeter   = (LONG)5000;
    ih_p->biClrUsed         = (DWORD)colors;
    ih_p->biClrImportant    = (DWORD)0;

    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       bmp8_rle_headers() -- fill in headers for 8-bit compressed RGB .bmp file (with RLE)
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int  bmp8_rle_headers( BITMAPFILEHEADER  *fh_p 
                     , BITMAPINFOHEADER *ih_p 
                     , int32_t           columns
                     , int32_t           rows
                     , int32_t           colors
                     , uint64_t          pixel_size
                     )
try
{
    size_t   hdr_size;
    size_t   color_table_size;
    size_t   bmp_area;
    real_T   pct; 
    
    hdr_size         = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);
    color_table_size = colors * sizeof RGBQUAD;

    M__(M_dmsg("BMP columns = %d", columns)) 
    M__(M_dmsg("BMP rows    = %d", rows))  
    M__(M_dmsg("BMP colors  = %d", colors))  
    M__(M_dmsg("BMP px size = %lld", pixel_size)) 
   
    M__(M_dmsg("BMP hdr size         = %u", (uint32_t)hdr_size))
    M__(M_dmsg("BMP color table size = %u", (uint32_t)color_table_size))
    
    bmp_area = (uint64_t)rows * (uint64_t)columns;
    pct      = 100.0 * (real_T)pixel_size / (real_T)bmp_area;
           
    M_out( L"Output  : .bmp colors = %d -- dimensions = %d x %d = %llu pixels = %llu RLE bytes - %.2f %% (%.3f MB total)")
         % colors
         % columns
         % rows
         % bmp_area
         % pixel_size 
         % pct
         % ((real_T)(pixel_size + (uint64_t)hdr_size + (uint64_t)color_table_size) / 1048576.0)
         ;

    if (pixel_size > (uint64_t)UINT_MAX - (uint64_t)hdr_size - (uint64_t)color_table_size)
    {
        M_out_emsg(L"Requested .BMP file size = %llu -- too long") %  pixel_size;
        return -1;
    }

    if (colors > 256)
    {
        M_out_emsg(L"Color table entries > 256 for 8-bit bitmap -- colors=%d") % colors;
        return -1;
    }

  
    fh_p->bfType            = 0x4d42;                                                 // "BM" 
    fh_p->bfSize            = (DWORD)(hdr_size + color_table_size + pixel_size); 
    fh_p->bfReserved1       = 0;
    fh_p->bfReserved2       = 0;
    fh_p->bfOffBits         = (DWORD)hdr_size + (DWORD)color_table_size;

    ih_p->biSize            = (DWORD)sizeof(BITMAPINFOHEADER);
    ih_p->biWidth           = (LONG)columns;
    ih_p->biHeight          = (LONG)rows;
    ih_p->biPlanes          = (WORD)1;
    ih_p->biBitCount        = (WORD)8;
    ih_p->biCompression     = (DWORD)BI_RLE8;
    ih_p->biSizeImage       = (DWORD)pixel_size;
    ih_p->biXPelsPerMeter   = (LONG)5000;
    ih_p->biYPelsPerMeter   = (LONG)5000;
    ih_p->biClrUsed         = (DWORD)colors;
    ih_p->biClrImportant    = (DWORD)0;

    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       write_bmp() -- writes DIB or DIB section to .BMP file (builds BITMAPFILEHEADER only)
//||
//||                        (bit map outimage data and color table must have proper padding/alignment already) 
//||       
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

M_DRAW_IMPEXP
int write_bmp( const BITMAPINFOHEADER  *ih_p 
             , const void              *color_table_p
             , size_t                   color_table_size
             , const void              *outimage_p
             , size_t                   outimage_size 
             , const std::wstring&      basename
             )
try
{
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
    
    M_out(L"Output  : BMP file name = \"%s\"") % filename;  


    // Build BITMAPFILEHEADER based on input info
   
    size_t hdr_size = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);

    BITMAPFILEHEADER fh; 

    fh.bfType            = 0x4d42;   // "BM" 
    fh.bfSize            = (DWORD)(hdr_size + color_table_size + outimage_size); 
    fh.bfReserved1       = 0;
    fh.bfReserved2       = 0;
    fh.bfOffBits         = (DWORD)hdr_size + (DWORD)color_table_size;

    
    // write out headers, color table and outimage pixel data to .bmp file

    fbmp.write((char *)&fh , (std::streamsize)(sizeof (BITMAPFILEHEADER))); 
    fbmp.write((char *)ih_p, (std::streamsize)(sizeof (BITMAPINFOHEADER)));     

    if ( (color_table_p != NULL) && (color_table_size > 0) )
        fbmp.write((char *)color_table_p, (std::streamsize)color_table_size); 

    if ( (outimage_p != NULL) && (outimage_size > 0) )
        fbmp.write((char *)outimage_p, (std::streamsize)outimage_size); 
     

    if (!(fbmp.good()))
    {
        M_out_emsg(L"write()s failed for file \"%s\"") % filename;  
        return -1;
    }
    

    // close file and return -- ignore R/C

    fbmp.close();
    return 0; 
}
M_endf

//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""