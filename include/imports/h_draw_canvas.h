//h_draw_canvas.h 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////
////            =============== 
////            h_draw_canvas.h -- items for GDI, GDI+ multi-bmp, and bmp file-oriented items (header)  
////            ===============
//// 
////
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       
#pragma once



//---------------------------------------------------------------------------------------------------------------------
//     RGBTRIPLE array in BMP file items   and   RGB QUAD array in BMP files
//---------------------------------------------------------------------------------------------------------------------

#define M_PUT_RGBT(p, r, g, b) {(p)->rgbtRed = (BYTE)(r); (p)->rgbtGreen = (BYTE)(g); (p)->rgbtBlue = (BYTE)(b);                            } 
#define M_PUT_RGBQ(p, r, g, b) {(p)->rgbRed  = (BYTE)(r); (p)->rgbGreen  = (BYTE)(g); (p)->rgbBlue  = (BYTE)(b); (p)->rgbReserved = (BYTE)0;} 


// fetch RGBTRIPLE from DIBSection pixel buffer

#define M_DIBSection_rgb(p,cols,r,c) (((RGBTRIPLE *)(p))[(r) * (cols) + (c)])


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------------------------------------------------
//     Transform control structure
//---------------------------------------------------------------------------------------------------------------------
   
struct transform_S
{
    Gdiplus::REAL                tx         {0.0};                 
    Gdiplus::REAL                ty         {0.0};                  
    Gdiplus::REAL                rotation   {0.0}; 
    Gdiplus::REAL                x_scale    {1.0};
    Gdiplus::REAL                y_scale    {1.0};
    Gdiplus::REAL                x_shear    {0.0}; 
    Gdiplus::REAL                y_shear    {0.0}; 


    std::vector<std::unique_ptr<Gdiplus::Matrix> > matrix_p   {   };      // vector of pointers to transform matrixes -- one per DC/bitmap/etc.


  //  Gdiplus::Matrix              matrix1    {   };      // Matrix for 1st DC 
  //  Gdiplus::Matrix              matrix2    {   };      // Matrix for 2nd DC
  //  Gdiplus::Matrix              matrix3    {   };      // Matrix for 3rd DC 
  //  Gdiplus::Matrix              matrix4    {   };      // Matrix for 4th DC
  //  Gdiplus::Matrix              matrix5    {   };      // Matrix for 5th DC 
  //  Gdiplus::Matrix              matrix6    {   };      // Matrix for 6th DC
};



//---------------------------------------------------------------------------------------------------------------------
//     Font control structure
//---------------------------------------------------------------------------------------------------------------------

struct font_S
{
    const Gdiplus::Font *font_p            {nullptr}   ;
    INT                  font_style        {       }   ; 
    Gdiplus::Unit        font_unit         {       }   ;
    Gdiplus::REAL        font_size         {       }   ;
    Gdiplus::REAL        font_height       {       }   ;
    std::wstring         family_name       {       }   ;
    bool                 installed         {       }   ; 

    UINT16               em_height         {       }   ;                            
    Gdiplus::REAL        line_spacing      {       }   ; 
    Gdiplus::REAL        cell_ascent       {       }   ; 
    Gdiplus::REAL        cell_descent      {       }   ; 
    Gdiplus::REAL        cell_height       {       }   ; 
    Gdiplus::REAL        leading           {       }   ; 
    Gdiplus::REAL        center_adjust     {       }   ;
    Gdiplus::REAL        non_spacing_width {       }   ;    // assumed width for non-spacing characters at end of 1-char string = non_spacing_width * font_height 
};




//---------------------------------------------------------------------------------------------------------------------
//     Text formatting control structure
//---------------------------------------------------------------------------------------------------------------------

struct text_format_S
{
    Gdiplus::SolidBrush         *text_brush_p        {nullptr};       // non-owning pointer
    Gdiplus::SolidBrush         *background_brush_p  {nullptr};       // non-owning pointer
    Gdiplus::StringFormat       *string_format_p     {nullptr};       // non-owning pointer

    Gdiplus::REAL                x                   {0.0}    ;       // starting X position
    Gdiplus::REAL                y                   {0.0}    ;       // starting Y position
    Gdiplus::REAL                rotation            {0.0}    ;       // rotation at start 
    Gdiplus::REAL                x_scale             {1.0}    ;       // x-scale at start
    Gdiplus::REAL                y_scale             {1.0}    ;       // y-scale at start  
    Gdiplus::REAL                x_shear             {0.0}    ;       // x-shear at start
    Gdiplus::REAL                y_shear             {0.0}    ;       // y-shear at start  
    Gdiplus::REAL                extra_space         {0.0}    ;       // extra space between characters -- gets multiplied by font_size and is scaled by x_scale (used by draw_text() only)

    bool                         draw_text           {true }  ;       // true -> draw text characters
    bool                         draw_background     {false}  ;       // true -> draw text background box


    int                          (*exit_p)(const WCHAR *, int, const font_S&, text_format_S&)
                                                     {nullptr};       // exit function called before each char is drawn (by draw_text() only)
    void                         *exit_parm          {nullptr};       // pointer to parm area for exit routine

    // updated fields for text formatting (draw_char() and draw_str() only)

    Gdiplus::REAL                 curr_x              {0.0}    ;       // X position of current char in string (updated as draw_text() progresses) 
    Gdiplus::REAL                 curr_y              {0.0}    ;       // Y position of current char in string (updated as draw_text() progresses)
    Gdiplus::REAL                 curr_rotation       {0.0}    ;       // current rotation (exit function can change in draw_text())  
    Gdiplus::REAL                 curr_x_scale        {1.0}    ;       // current x-scale (exit function can change in draw_text())
    Gdiplus::REAL                 curr_y_scale        {1.0}    ;       // current y-scale (exit function can change in draw_text())
    Gdiplus::REAL                 curr_x_shear        {0.0}    ;       // current x-shear (exit function can change in draw_text())
    Gdiplus::REAL                 curr_y_shear        {0.0}    ;       // current y-shear (exit function can change in draw_text())
    Gdiplus::REAL                 curr_extra_space    {0.0}    ;       // currret extra space (cam be forced to 0.0 in draw_str())


    // output fields

    transform_S                  curr_transform      {   }    ;       // current transform when current char/string was last drawn 
    Gdiplus::REAL                output_width        {0.0}    ;       // output width of last char or whole string 
    Gdiplus::REAL                extra_width         {0.0}    ;       // additional width due to requested extra_space 
    Gdiplus::REAL                new_x               {0.0}    ;       // X position at end of string
    Gdiplus::REAL                new_y               {0.0}    ;       // Y position at end of string
    Gdiplus::RectF               bound_rect          {   }    ;       // Bounding rectangle (not rotated -- valid only while global transform is in effect)
    Gdiplus::PointF              bounds_poly[4]      {   }    ;       // Bounding rectangle (rotated) for whole string, or last char
};



//---------------------------------------------------------------------------------------------------------------------
//     multi-BMP GDI/GDI+ resource control structure
//---------------------------------------------------------------------------------------------------------------------

//  structure with bitmap position info
//  -----------------------------------
//
//  note: if default values are present { 0 }, for example, can't use initializer list to construct bmpos_S
//

struct bmpos_S
{
    rc_T    x         ;         // starting column   for this bitmap    
    rc_T    y         ;         // starting row      for this bitmap
    rc_T    rows      ;         // number of rows    for this bitmap    
    rc_T    cols      ;         // number of columns for this bitmap 
};


////////////////////////////////////////////

class canvas_C
{
public:
    //////////////////////////////////////////////////////////////////////////////////////

    std::vector<bmpos_S>                         m_bmpos                 {       } ;    // vector with bitmap positions 
    std::vector<HDC>                             m_hdc                   {       } ;    // vector with HDCs
    std::vector<HBITMAP>                         m_hbitmap               {       } ;    // vector with HBITMAPs
    std::vector<HGDIOBJ>                         m_hgdiobj_saved         {       } ;    // vector with HGDIOBJs (original GDI object handle that was replaced, when n-th DIBSection was selected in to n-th DC) 
    std::vector<VOID *>                          m_pvBits                {       } ;    // vector with pointers to start of pixel data in n-th DIBSection
    std::vector<Gdiplus::Graphics *>             m_gr_p                  {       } ;    // vector with pointers to Gdiplus::Graphics objects used by this canvas_S object

    /////////////////////////////////////////////////////////////////////////////////////
                                                           
    int32_t                m_bmct                  {0    }   ;    // number of bitmaps used for this canvas
    bool                   m_usable                {false}   ;    // true -- canvas is usable (fully initialized)
    bool                   m_init                  {false}   ;    // true -- initialization was started
    int32_t                m_debug                 {0    }   ;    // 0 = no debug messages, 1 = main debug messages, 2 = more debug messages, 3 = all debug messages   
    rc_T                   m_width                 {0    }   ;    // total width  of this canvas
    rc_T                   m_height                {0    }   ;    // total height of this canvas
    rc_T                   m_actual_cols_bmp       {0    }   ;    // actual number of columns in each bitmap (rounded up to multiple of 4)

private:
    bool                   m_default_transform     {true }   ;    // always set back to default transform when drawing items other than text
    BITMAPINFO             m_bmi                   {     }   ;    // BITMAPINFO data used to create all DIBSections (all have same dimensions)


    // text output control areas

    Gdiplus::SolidBrush   *m_text_br_p             {nullptr} ;    // pointer to default text-writing Brush object            -- owning pointer -- obtained with operator new
    Gdiplus::SolidBrush   *m_text_background_br_p  {nullptr} ;    // pointer to default text-background writing Brush object -- owning pointer -- obtained with operator new
    const Gdiplus::Font   *m_curr_font_p           {nullptr} ;    // pointer to current Font object                          -- owning pointer -- obtained with operator new
    Gdiplus::StringFormat *m_sf_p                  {nullptr} ;    // pointer to current StringFormat object                  -- owning pointer -- obtained with operator new  

    Gdiplus::PrivateFontCollection *m_pfc_p        {nullptr} ;    // pointer to current private font collection


public:
    font_S                 m_font                  {       } ;    // current font_S        structure for drawing text 
    text_format_S         *m_tf_p                  {nullptr} ;    // pointer to current text_format_S strucure     -- owning pointer -- obtained with operator new (can't imbed text_format_S here -- Gdiplus::Matrix.Reset() fails)   




    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //               member functions
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:                       
                     canvas_C()                     = default;                                  // default constructor
                     canvas_C(const canvas_C&)      = delete;                                   // copy constructor
                     canvas_C(canvas_C&&)           = delete;                                   // move constructor
    M_DRAW_IMPEXP    ~canvas_C();                                                               // destructor
    canvas_C&        operator=(const canvas_C&)     = delete;                                   // copy assignment
    canvas_C&        operator=(canvas_C&&)          = delete;                                   // move assignment  

    // ---------------------------------------------------------------------------------------------------------------------------
      
    M_DRAW_IMPEXP int              init(rc_T, rc_T, int32_t);                                                 // initialize canvas_C object for use 
    M_DRAW_IMPEXP int              term();                                                                    // terminate canvas_C object -- free up all resources 
    M_DRAW_IMPEXP int              flush();                                                                   // flush GDI+ for this canvas_C object 
    M_DRAW_IMPEXP int              clear();                                                                   // clear out bitmaps in canvas_C object 
    M_DRAW_IMPEXP int              clear(const rgb_S);                                                        // clear out bitmaps in canvas_C object with specified color

    M_DRAW_IMPEXP int              bit_blt(HDC, int, int, int, int, int, int, DWORD);                         // multi-DC GDI BitBlt -- source is DCs in canvas_C object

    // -----------------------------------------------------------------------------------------------------------------------------
    //               transform functions
    // -----------------------------------------------------------------------------------------------------------------------------
    
    M_DRAW_IMPEXP int              init_transform_s(    transform_S&, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0);    
    M_DRAW_IMPEXP int              update_transform_s(  transform_S&);

    M_DRAW_IMPEXP int              reset_transform();      
    M_DRAW_IMPEXP int              default_transform();  

    M_DRAW_IMPEXP int              translate_transform(  Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              rotate_transform(     Gdiplus::REAL);
    M_DRAW_IMPEXP int              scale_transform(      Gdiplus::REAL, Gdiplus::REAL);
  
    M_DRAW_IMPEXP int              set_transform(        Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 1.0);
    M_DRAW_IMPEXP int              set_transform(        const transform_S&);

    
    // --------------------------------------------------------------------------------------------------------------------------------
    //               Multi-DC/DIBSection drawing functions
    // --------------------------------------------------------------------------------------------------------------------------------

    M_DRAW_IMPEXP int              draw_arc(             const Gdiplus::Pen   *, INT , INT , INT , INT , Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_arc(             const Gdiplus::Pen   *, Gdiplus::REAL,          Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_arc(             const Gdiplus::Pen   *, const Gdiplus::Rect   , Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_arc(             const Gdiplus::Pen   *, const Gdiplus::RectF  , Gdiplus::REAL, Gdiplus::REAL);
                    
    M_DRAW_IMPEXP int              draw_bezier(          const Gdiplus::Pen   *, INT , INT , INT , INT , INT , INT , INT , INT );
    M_DRAW_IMPEXP int              draw_bezier(          const Gdiplus::Pen   *, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_bezier(          const Gdiplus::Pen   *, const Gdiplus::Point&  , const Gdiplus::Point& , const Gdiplus::Point& , const Gdiplus::Point& );
    M_DRAW_IMPEXP int              draw_bezier(          const Gdiplus::Pen   *, const Gdiplus::PointF& , const Gdiplus::PointF&, const Gdiplus::PointF&, const Gdiplus::PointF&);
    M_DRAW_IMPEXP int              draw_beziers(         const Gdiplus::Pen   *, const Gdiplus::Point  *, INT);
    M_DRAW_IMPEXP int              draw_beziers(         const Gdiplus::Pen   *, const Gdiplus::PointF *, INT);
                    
    M_DRAW_IMPEXP int              draw_closed_curve(    const Gdiplus::Pen   *, const Gdiplus::Point  *, INT);
    M_DRAW_IMPEXP int              draw_closed_curve(    const Gdiplus::Pen   *, const Gdiplus::PointF *, INT);
    M_DRAW_IMPEXP int              draw_closed_curve(    const Gdiplus::Pen   *, const Gdiplus::Point  *, INT, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_closed_curve(    const Gdiplus::Pen   *, const Gdiplus::PointF *, INT, Gdiplus::REAL);
                    
    M_DRAW_IMPEXP int              draw_curve(           const Gdiplus::Pen   *, const Gdiplus::Point  *, INT);
    M_DRAW_IMPEXP int              draw_curve(           const Gdiplus::Pen   *, const Gdiplus::PointF *, INT);
    M_DRAW_IMPEXP int              draw_curve(           const Gdiplus::Pen   *, const Gdiplus::Point  *, INT, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_curve(           const Gdiplus::Pen   *, const Gdiplus::PointF *, INT, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_curve(           const Gdiplus::Pen   *, const Gdiplus::Point  *, INT, INT, INT, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_curve(           const Gdiplus::Pen   *, const Gdiplus::PointF *, INT, INT, INT, Gdiplus::REAL);
                    
    M_DRAW_IMPEXP int              draw_ellipse(         const Gdiplus::Pen   *, INT , INT , INT , INT );
    M_DRAW_IMPEXP int              draw_ellipse(         const Gdiplus::Pen   *, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_ellipse(         const Gdiplus::Pen   *, const Gdiplus::Rect& );
    M_DRAW_IMPEXP int              draw_ellipse(         const Gdiplus::Pen   *, const Gdiplus::RectF&);
                    
    M_DRAW_IMPEXP int              draw_image(                 Gdiplus::Image *, INT, INT                                                                                   );
    M_DRAW_IMPEXP int              draw_image(                 Gdiplus::Image *, INT, INT, INT, INT                                                                         );
    M_DRAW_IMPEXP int              draw_image(                 Gdiplus::Image *, INT, INT, INT, INT, INT, INT                                                               );
    M_DRAW_IMPEXP int              draw_image(                 Gdiplus::Image *, const Gdiplus::Point&                                                                      );   
    M_DRAW_IMPEXP int              draw_image(                 Gdiplus::Image *, const Gdiplus::Rect&                                                                       );    
    M_DRAW_IMPEXP int              draw_image(                 Gdiplus::Image *, const Gdiplus::Point *,  INT                                                               );  
    M_DRAW_IMPEXP int              draw_image(                 Gdiplus::Image *, const Gdiplus::Rect&,         INT, INT, INT, INT, Gdiplus::Unit, Gdiplus::ImageAttributes *); 
    M_DRAW_IMPEXP int              draw_image(                 Gdiplus::Image *, const Gdiplus::Point *,  INT, INT, INT, INT, INT, Gdiplus::Unit, Gdiplus::ImageAttributes *);    
                    
    M_DRAW_IMPEXP int              draw_line(            const Gdiplus::Pen   *, INT , INT , INT , INT );
    M_DRAW_IMPEXP int              draw_line(            const Gdiplus::Pen   *, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_line(            const Gdiplus::Pen   *, const Gdiplus::Point& , const Gdiplus::Point& );
    M_DRAW_IMPEXP int              draw_line(            const Gdiplus::Pen   *, const Gdiplus::PointF&, const Gdiplus::PointF&);
    M_DRAW_IMPEXP int              draw_lines(           const Gdiplus::Pen   *, const Gdiplus::Point  *, INT);
    M_DRAW_IMPEXP int              draw_lines(           const Gdiplus::Pen   *, const Gdiplus::PointF *, INT);
                    
    M_DRAW_IMPEXP int              draw_path(            const Gdiplus::Pen   *, const Gdiplus::GraphicsPath *);
                    
    M_DRAW_IMPEXP int              draw_pie(             const Gdiplus::Pen   *, INT , INT , INT , INT , Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_pie(             const Gdiplus::Pen   *, Gdiplus::REAL,          Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_pie(             const Gdiplus::Pen   *, const Gdiplus::Rect   , Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_pie(             const Gdiplus::Pen   *, const Gdiplus::RectF  , Gdiplus::REAL, Gdiplus::REAL);
                    
    M_DRAW_IMPEXP int              draw_polygon(         const Gdiplus::Pen   *, const Gdiplus::Point  *, INT);
    M_DRAW_IMPEXP int              draw_polygon(         const Gdiplus::Pen   *, const Gdiplus::PointF *, INT);
                    
    M_DRAW_IMPEXP int              draw_rectangle(       const Gdiplus::Pen   *, INT , INT , INT , INT );
    M_DRAW_IMPEXP int              draw_rectangle(       const Gdiplus::Pen   *, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              draw_rectangle(       const Gdiplus::Pen   *, const Gdiplus::Rect& );
    M_DRAW_IMPEXP int              draw_rectangle(       const Gdiplus::Pen   *, const Gdiplus::RectF&);
    M_DRAW_IMPEXP int              draw_rectangles(      const Gdiplus::Pen   *, const Gdiplus::Rect  *, INT);
    M_DRAW_IMPEXP int              draw_rectangles(      const Gdiplus::Pen   *, const Gdiplus::RectF *, INT);
                    
    M_DRAW_IMPEXP int              draw_string(          const WCHAR *, INT, const Gdiplus::Font *, const Gdiplus::PointF&,                                const Gdiplus::Brush *);
    M_DRAW_IMPEXP int              draw_string(          const WCHAR *, INT, const Gdiplus::Font *, const Gdiplus::PointF&, const Gdiplus::StringFormat *, const Gdiplus::Brush *);
    M_DRAW_IMPEXP int              draw_string(          const WCHAR *, INT, const Gdiplus::Font *, const Gdiplus::RectF& , const Gdiplus::StringFormat *, const Gdiplus::Brush *);
                    
    M_DRAW_IMPEXP int              fill_closed_curve(    const Gdiplus::Brush *, const Gdiplus::Point  *, INT);
    M_DRAW_IMPEXP int              fill_closed_curve(    const Gdiplus::Brush *, const Gdiplus::PointF *, INT);
    M_DRAW_IMPEXP int              fill_closed_curve(    const Gdiplus::Brush *, const Gdiplus::Point  *, INT, Gdiplus::FillMode, Gdiplus::REAL);
    M_DRAW_IMPEXP int              fill_closed_curve(    const Gdiplus::Brush *, const Gdiplus::PointF *, INT, Gdiplus::FillMode, Gdiplus::REAL);
                    
    M_DRAW_IMPEXP int              fill_ellipse(         const Gdiplus::Brush *, INT , INT , INT , INT );
    M_DRAW_IMPEXP int              fill_ellipse(         const Gdiplus::Brush *, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              fill_ellipse(         const Gdiplus::Brush *, const Gdiplus::Rect& );
    M_DRAW_IMPEXP int              fill_ellipse(         const Gdiplus::Brush *, const Gdiplus::RectF&);
                    
    M_DRAW_IMPEXP int              fill_path(            const Gdiplus::Brush *, const Gdiplus::GraphicsPath *);
                    
    M_DRAW_IMPEXP int              fill_pie(             const Gdiplus::Brush *, INT , INT , INT , INT , Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              fill_pie(             const Gdiplus::Brush *, Gdiplus::REAL,          Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              fill_pie(             const Gdiplus::Brush *, const Gdiplus::Rect   , Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              fill_pie(             const Gdiplus::Brush *, const Gdiplus::RectF  , Gdiplus::REAL, Gdiplus::REAL);
                    
    M_DRAW_IMPEXP int              fill_polygon(         const Gdiplus::Brush *, const Gdiplus::Point  *, INT);
    M_DRAW_IMPEXP int              fill_polygon(         const Gdiplus::Brush *, const Gdiplus::PointF *, INT);
    M_DRAW_IMPEXP int              fill_polygon(         const Gdiplus::Brush *, const Gdiplus::Point  *, INT, Gdiplus::FillMode);
    M_DRAW_IMPEXP int              fill_polygon(         const Gdiplus::Brush *, const Gdiplus::PointF *, INT, Gdiplus::FillMode);
                    
    M_DRAW_IMPEXP int              fill_rectangle(       const Gdiplus::Brush *, INT , INT , INT , INT );
    M_DRAW_IMPEXP int              fill_rectangle(       const Gdiplus::Brush *, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL, Gdiplus::REAL);
    M_DRAW_IMPEXP int              fill_rectangle(       const Gdiplus::Brush *, const Gdiplus::Rect& );
    M_DRAW_IMPEXP int              fill_rectangle(       const Gdiplus::Brush *, const Gdiplus::RectF&);
                    
    M_DRAW_IMPEXP int              fill_rectangles(      const Gdiplus::Brush *, const Gdiplus::Rect  *, INT);
    M_DRAW_IMPEXP int              fill_rectangles(      const Gdiplus::Brush *, const Gdiplus::RectF *, INT);
                    
    M_DRAW_IMPEXP int              fill_region(          const Gdiplus::Brush *, const Gdiplus::Region *);
                    
    M_DRAW_IMPEXP int              gdi__clear(                 Gdiplus::Color); 
    M_DRAW_IMPEXP int              gdi__flush(                 Gdiplus::FlushIntention);  
                    
    M_DRAW_IMPEXP int              reset_clip();
    M_DRAW_IMPEXP int              set_clip(             const Gdiplus::Rect&                      );
    M_DRAW_IMPEXP int              set_clip(             const Gdiplus::Rect&, Gdiplus::CombineMode); 


    // --------------------------------------------------------------------------------------------------------------------------------
    //               Multi-DC/DIBSection convenience drawing functions
    // --------------------------------------------------------------------------------------------------------------------------------
     
    M_DRAW_IMPEXP int              draw_line(            const rgb_S&,            rc_T , rc_T , rc_T , rc_T , rc_T   ); 
    M_DRAW_IMPEXP int              draw_box(             const rgb_S&,            rc_T , rc_T , rc_T , rc_T          );
    M_DRAW_IMPEXP int              draw_frame(           const rgb_S&,            rc_T , rc_T , rc_T , rc_T , rc_T   );
    M_DRAW_IMPEXP int              draw_circle(          const rgb_S&,            rc_T , rc_T , rc_T                 );
    M_DRAW_IMPEXP int              draw_ring(            const rgb_S&,            rc_T , rc_T , rc_T , rc_T          );
                                                                                                       
    M_DRAW_IMPEXP int              draw_line(            const rgb_S&,            rcf_T, rcf_T, rcf_T, rcf_T, rcf_T  ); 
    M_DRAW_IMPEXP int              draw_box(             const rgb_S&,            rcf_T, rcf_T, rcf_T, rcf_T         );
    M_DRAW_IMPEXP int              draw_frame(           const rgb_S&,            rcf_T, rcf_T, rcf_T, rcf_T, rcf_T  );
    M_DRAW_IMPEXP int              draw_circle(          const rgb_S&,            rcf_T, rcf_T, rcf_T                );
    M_DRAW_IMPEXP int              draw_ring(            const rgb_S&,            rcf_T, rcf_T, rcf_T, rcf_T         );
                                                                                                       
    M_DRAW_IMPEXP int              draw_line(            const rgb_S&, alpha_T,   rc_T , rc_T , rc_T , rc_T , rc_T   ); 
    M_DRAW_IMPEXP int              draw_box(             const rgb_S&, alpha_T,   rc_T , rc_T , rc_T , rc_T          );
    M_DRAW_IMPEXP int              draw_frame(           const rgb_S&, alpha_T,   rc_T , rc_T , rc_T , rc_T , rc_T   );
    M_DRAW_IMPEXP int              draw_circle(          const rgb_S&, alpha_T,   rc_T , rc_T , rc_T                 );
    M_DRAW_IMPEXP int              draw_ring(            const rgb_S&, alpha_T,   rc_T , rc_T , rc_T , rc_T          );
                                                                                                       
    M_DRAW_IMPEXP int              draw_line(            const rgb_S&, alpha_T,   rcf_T, rcf_T, rcf_T, rcf_T, rcf_T  ); 
    M_DRAW_IMPEXP int              draw_box(             const rgb_S&, alpha_T,   rcf_T, rcf_T, rcf_T, rcf_T         );
    M_DRAW_IMPEXP int              draw_frame(           const rgb_S&, alpha_T,   rcf_T, rcf_T, rcf_T, rcf_T, rcf_T  );
    M_DRAW_IMPEXP int              draw_circle(          const rgb_S&, alpha_T,   rcf_T, rcf_T, rcf_T                );
    M_DRAW_IMPEXP int              draw_ring(            const rgb_S&, alpha_T,   rcf_T, rcf_T, rcf_T, rcf_T         );
                                           
    M_DRAW_IMPEXP int              draw_circle(          const Gdiplus::Pen   *, Gdiplus::PointF, Gdiplus::REAL                );
    M_DRAW_IMPEXP int              draw_circle(          const Gdiplus::Pen   *, Gdiplus::REAL,   Gdiplus::REAL, Gdiplus::REAL ); 
    M_DRAW_IMPEXP int              fill_circle(          const Gdiplus::Brush *, Gdiplus::PointF, Gdiplus::REAL                );
    M_DRAW_IMPEXP int              fill_circle(          const Gdiplus::Brush *, Gdiplus::REAL,   Gdiplus::REAL, Gdiplus::REAL );

    //               --------------------------------------------------------------------------------------

    M_DRAW_IMPEXP int              draw_beziers(         const rgb_S&,            rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_beziers(         const rgb_S&,            rcf_T, const std::vector<Gdiplus::PointF>&    );  
    M_DRAW_IMPEXP int              draw_beziers(         const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_beziers(         const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::PointF>&    );
     
    M_DRAW_IMPEXP int              draw_closed_curve(    const rgb_S&,            rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_closed_curve(    const rgb_S&,            rcf_T, const std::vector<Gdiplus::PointF>&    );  
    M_DRAW_IMPEXP int              draw_closed_curve(    const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_closed_curve(    const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::PointF>&    ); 

    M_DRAW_IMPEXP int              draw_curve(           const rgb_S&,            rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_curve(           const rgb_S&,            rcf_T, const std::vector<Gdiplus::PointF>&    );  
    M_DRAW_IMPEXP int              draw_curve(           const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_curve(           const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::PointF>&    );  
                                           
    M_DRAW_IMPEXP int              draw_lines(           const rgb_S&,            rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_lines(           const rgb_S&,            rcf_T, const std::vector<Gdiplus::PointF>&    );  
    M_DRAW_IMPEXP int              draw_lines(           const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_lines(           const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::PointF>&    ); 
                                           
    M_DRAW_IMPEXP int              draw_polygon(         const rgb_S&,            rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_polygon(         const rgb_S&,            rcf_T, const std::vector<Gdiplus::PointF>&    );  
    M_DRAW_IMPEXP int              draw_polygon(         const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              draw_polygon(         const rgb_S&, alpha_T,   rcf_T, const std::vector<Gdiplus::PointF>&    ); 

    M_DRAW_IMPEXP int              fill_closed_curve(    const rgb_S&,                   const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              fill_closed_curve(    const rgb_S&,                   const std::vector<Gdiplus::PointF>&    );  
    M_DRAW_IMPEXP int              fill_closed_curve(    const rgb_S&, alpha_T,          const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              fill_closed_curve(    const rgb_S&, alpha_T,          const std::vector<Gdiplus::PointF>&    );
                                                                          
    M_DRAW_IMPEXP int              fill_polygon(         const rgb_S&,                   const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              fill_polygon(         const rgb_S&,                   const std::vector<Gdiplus::PointF>&    );  
    M_DRAW_IMPEXP int              fill_polygon(         const rgb_S&, alpha_T,          const std::vector<Gdiplus::Point>&     );  
    M_DRAW_IMPEXP int              fill_polygon(         const rgb_S&, alpha_T,          const std::vector<Gdiplus::PointF>&    );


    // --------------------------------------------------------------------------------------------------------------------------------
    //               Multi-DC/DIBSection convenience drawing functions
    // --------------------------------------------------------------------------------------------------------------------------------  

    M_DRAW_IMPEXP int              draw_txt(                     const std::wstring&,           const font_S&, text_format_S&                                        ); // internal form
    M_DRAW_IMPEXP int              draw_txt(                     const std::wstring&                                                                                 ); // external form
    M_DRAW_IMPEXP int              draw_str(                     const std::wstring&,           const font_S&, text_format_S&                                        ); // internal form
    M_DRAW_IMPEXP int              draw_str(                     const std::wstring&                                                                                 ); // external form
    M_DRAW_IMPEXP int              format_str(                   const std::wstring&,                          const Gdiplus::RectF&, const Gdiplus::StringFormat *  );
    M_DRAW_IMPEXP int              format_str(                   const std::wstring&,                          rc_T, rc_T, rc_T, rc_T                                );                      
    M_DRAW_IMPEXP int              draw_chr(                     const WCHAR *,                 const font_S&, text_format_S&                                        );
    M_DRAW_IMPEXP int              draw_chr(                     const WCHAR *                                                                                       );                     
    M_DRAW_IMPEXP int              measure_str(                  const WCHAR *,         size_t, const font_S&, text_format_S&, bool = false                          );   


    // --------------------------------------------------------------------------------------------------------------------------------
    //               Text format setup functions
    // -------------------------------------------------------------------------------------------------------------------------------- 


    M_DRAW_IMPEXP int              set_text_format(              INT = 0, INT = 0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0);    // external
    M_DRAW_IMPEXP int              set_text_color(               const rgb_S&                      ); 
    M_DRAW_IMPEXP int              set_text_color(               const rgb_S&, alpha_T             );                                                                       // external  
    M_DRAW_IMPEXP int              set_no_text_background(                                         );                                                                       // external   
    M_DRAW_IMPEXP int              set_text_background_color(    const rgb_S&                      );                                                                       // external   
    M_DRAW_IMPEXP int              set_text_background_color(    const rgb_S&, alpha_T             );                                                                       // external    
    M_DRAW_IMPEXP int              set_text_strfmt(              Gdiplus::StringFormat * = nullptr );                                                                       // external  


    // --------------------------------------------------------------------------------------------------------------------------------
    //               font-oriented functions
    // --------------------------------------------------------------------------------------------------------------------------------  

    M_DRAW_IMPEXP int              init_font(                    bool                                             );    // external 
    M_DRAW_IMPEXP int              set_font(                     const std::wstring&, Gdiplus::REAL, INT, bool    );    // external   
    M_DRAW_IMPEXP int              set_font_size(                                     Gdiplus::REAL               );    // external  
    M_DRAW_IMPEXP int              add_font_file(                const std::wstring&                              );    // external   
    M_DRAW_IMPEXP int              add_font_folder(              const std::wstring&, bool                        );    // external   
    M_DRAW_IMPEXP int              remove_fonts(                                                                  );    // external
    M_DRAW_IMPEXP int              display_fonts(                bool                                             );    // external 
    M_DRAW_IMPEXP int              get_family_count(             bool, int32_t&                                   );    // external
    M_DRAW_IMPEXP int              get_family_names(             bool, std::vector<std::wstring>&                 );    // external    
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------
//     BMP header - oriented functions
//---------------------------------------------------------------------------------------------------------------------

M_DRAW_IMPEXP int      bmp24_headers(     BITMAPFILEHEADER *, BITMAPINFOHEADER *, int32_t, int32_t                   );
M_DRAW_IMPEXP int      bmp8_headers(      BITMAPFILEHEADER *, BITMAPINFOHEADER *, int32_t, int32_t, int32_t          );
M_DRAW_IMPEXP int      bmp8_rle_headers(  BITMAPFILEHEADER *, BITMAPINFOHEADER *, int32_t, int32_t, int32_t, uint64_t);



//---------------------------------------------------------------------------------------------------------------------
//     .bmp file - oriented functions
//---------------------------------------------------------------------------------------------------------------------

M_DRAW_IMPEXP int      write_bmp( const BITMAPINFOHEADER  *, const void *, size_t, const void *, size_t, const std::wstring&);


//---------------------------------------------------------------------------------------------------------------------
//     non-canvas_C::   GDI+ graphics functions
//---------------------------------------------------------------------------------------------------------------------
                  

// initialization/termination functions
// ------------------------------------

M_DRAW_IMPEXP int   startup_gdi();
M_DRAW_IMPEXP void shutdown_gdi(); 
 

// text-oriented and font-oriented functions
// -----------------------------------------

M_DRAW_IMPEXP int  init_text_format(text_format_S&, Gdiplus::SolidBrush *, Gdiplus::SolidBrush *, Gdiplus::StringFormat *);                        // internal
M_DRAW_IMPEXP int  copy_text_format(text_format_S&, const text_format_S&);
M_DRAW_IMPEXP int  set_text_format( text_format_S&,         INT = 0, INT = 0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0);              // internal
M_DRAW_IMPEXP int  set_text_strfmt( text_format_S&,         Gdiplus::StringFormat *);                                                                             // internal
M_DRAW_IMPEXP void text_new_xy(     text_format_S&, Gdiplus::REAL);

M_DRAW_IMPEXP int  init_font(       font_S&,        const Gdiplus::Font&,       bool                                     );                        // internal 


// Utility functions (internal)
// -----------------

M_DRAW_IMPEXP Gdiplus::PointF transform_point(const Gdiplus::PointF&, const transform_S&);
M_DRAW_IMPEXP Gdiplus::PointF transform_point(Gdiplus::REAL, Gdiplus::REAL            , const transform_S&);
M_DRAW_IMPEXP Gdiplus::PointF transform_point(const Gdiplus::PointF&, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 1.0);
M_DRAW_IMPEXP Gdiplus::PointF transform_point(Gdiplus::REAL, Gdiplus::REAL,                Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 0.0, Gdiplus::REAL = 1.0, Gdiplus::REAL = 1.0);


// functions to add items to a GDI+ graphics path (low_level)
// ----------------------------------------------

M_DRAW_IMPEXP int add_lines_r(   Gdiplus::GraphicsPath&, int32_t, real_T,         real_T = -90.0, real_T = 0.0, real_T = 0.0); 
M_DRAW_IMPEXP int add_polygon_r( Gdiplus::GraphicsPath&, int32_t, real_T,         real_T = -90.0, real_T = 0.0, real_T = 0.0);
M_DRAW_IMPEXP int add_polygon_r2(Gdiplus::GraphicsPath&, int32_t, real_T, real_T, real_T = -90.0, real_T = 0.0, real_T = 0.0);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////