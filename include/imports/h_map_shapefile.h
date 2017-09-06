// h_map_shapefile.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =================
////     h_map_shapefile.h   -- shapefile oriented definitions (map_shapefile.c)  
////     =================
////    
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
#pragma once
#include "h_map_types.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// .shp and .shx 100-byte file header (used by shapefile_C) -- converted to little endian, reserved bytes skipped
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// shapefile data items
// --------------------

struct shp_pt_S                 // shapefile point (longitude, usually latitude) 
{
    real64_T x  {0.0} ;
    real64_T y  {0.0} ;    
}; 


struct shp_ptm_S : public shp_pt_S
{
    real64_T m  {0.0} ; 
}; 


struct shp_ptz_S : public shp_pt_S
{
    real64_T z  {0.0} ; 
    real64_T m  {0.0} ; 
}; 


struct shp_box_S   
{
    real64_T xmin  {0.0} ;
    real64_T ymin  {0.0} ;
    real64_T xmax  {0.0} ;
    real64_T ymax  {0.0} ;
};


struct shp_boxm_S  : public shp_box_S  // includes M Range 
{
    real64_T mmin  {0.0} ;     // optional -- in .shp M Range 
    real64_T mmax  {0.0} ;     // optional -- in .shp M Range 
};


struct shp_boxz_S  : public shp_box_S  // includes Z Range and M Range 
{
    real64_T zmin  {0.0} ;     // optional -- in .shp Z Range 
    real64_T zmax  {0.0} ;     // optional -- in .shp Z Range 
    real64_T mmin  {0.0} ;     // optional -- in .shp M Range 
    real64_T mmax  {0.0} ;     // optional -- in .shp M Range 
};



// .shp and .shx header
// --------------------

struct shp_hdr_S
{
    int32_t   file_code    {0  };  
    int32_t   file_length  {0  };
    int32_t   version      {0  };
    int32_t   shapes_type  {0  };

    shp_boxz_S box              ;   // bounding box for all items in this file
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// .dbf 32-byte file header (used by shapefile_C) + vector of record descriptors
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// .dbf field types

enum class dbf_ft_E { binary         //   B
                    , character      //   C
                    , date           //   D -- 8-byte date YYYYMMDD
                    , floating_pt    //   F
                    , general        //   G
                    , integer        //   I
                    , logical        //   L 
                    , memo           //   M
                    , numeric        //   N 
                    , picture        //   P 
                    , datetime       //   T 
                    , currency       //   Y 
                    , unknown        //   anything else
                    };


// extracted items from .dbf field descriptor -- 32-byte version -- saved in .dbf fd vector
// ------------------------------------------

struct dbf_fd_S
{
    std::string name                            ;    // field name (ASCII)            -- not wide char string
    char        type_c    {' '}                 ;    // field type (as ASCII char)    -- not wide char
    dbf_ft_E    type      {dbf_ft_E::unknown}   ;    // field type (as enum member)
    uint32_t    length    {0}                   ;    // field length
    uint32_t    offset    {0}                   ;    // field offset from start of record
    uint32_t    dec_ct    {0}                   ;    // decimal count
};

    
// extracted items from .dbf header -- 32-byte version
// ---------------------------------------------------

struct dbf_hdr_S
{
    uint8_t                version   {0} ; 
    uint8_t                yy        {0} ; 
    uint8_t                mm        {0} ; 
    uint8_t                dd        {0} ; 
    uint32_t               n_rec     {0} ;    // number of records in the file
    uint32_t               hdr_len   {0} ;    // length of header -or- start of data
               
    uint32_t               rec_len   {0} ;    // length of each record
    uint32_t               n_fld     {0} ;    // number of fields in each record (computed = (hdr_len-1)/32 - 1) 
    uint64_t               file_len  {0} ;    // computed file length (should match actual size)  
    std::vector<dbf_fd_S>  fd        {0} ;    // vector of field descriptors with offsets
};


// extracted field data from .dbf record
// -------------------------------------

struct dbf_fld_S
{
     dbf_ft_E    type          {dbf_ft_E::unknown}  ;
     int32_t     dec_places    {-1}                 ;          // decimal places in field (only if number type)      
     std::string all_chars                          ;          // character string without leading and trailing blanks  (ASCII -- not wide char)
     std::string chars                              ;          // character string without leading and trailing blanks  (ASCII -- not wide char)
     real64_T    real64        {0.0}                ;          // decoded value 
     int64_t     int64         {0}                  ;          // decoded value
};



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   shapefile_C -- class for reading in .shx, .shp, and .dbf shapefile
//   ===========    ---------------------------------------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

class shapefile_C 
{
public: 
                       shapefile_C()                    = default;                         // default contructor
                       shapefile_C(const shapefile_C&)  = delete;                          // copy constructor
                       shapefile_C(shapefile_C&&)       = delete;                          // move constructor
                      ~shapefile_C()                    = default;                         // destructor
    shapefile_C        operator=(const shapefile_C&)    = delete;                          // copy assignment
    shapefile_C&       operator=(shapefile_C&&)         = delete;                          // move assignment
   
    int                readin();                                                           // read .shx, .shp, .dbf files
    int                readin(const std::wstring&);                                        // read .shx, .shp, .dbf files
    int                readin(const std::wstring&, const std::wstring&);                   // read .shx, .shp, .dbf files
    void               freeup();                                                           // freeup any buffer space occupied by files
    int                draw(mapdata_S&, outimage_S&, draw_parm_S&, const mapparm_S&);         // draw out shapefile items into outimage.gs graphics target

    ///////////////////////// data members ////////////////////////////////////

    std::wstring         m_folder            {        };               // folder containing shapefiles 
    std::wstring         m_basename          {        };               // filename without filetype for .shx, .shp, and .dbf file 
    std::wstring         m_shx_ft            {L".shx" };               // filetype for .shx file
    std::wstring         m_shp_ft            {L".shp" };               // filetype for .shp file
    std::wstring         m_dbf_ft            {L".dbf" };               // filetype for .dbf file

    bool                 m_skip_dbf          {false   };               // true = don't bother reading in .dbf file (if any) 
    bool                 m_contents_present  {false   };               // true = .shx and .shp files read in 
    bool                 m_dbf_present       {false   };               // true = .dbf file read in  
    bool                 m_contents_valid    {false   };               // true = .shx and .shp file contents verified, index built, etc.
    bool                 m_dbf_valid         {false   };               // true = dbf file contents valid - dbf_hdr filled in with field offsets, etc.   
     
    shp_hdr_S            m_shx_hdr;                                    // fields extracted from 100-byte .shx file header
    shp_hdr_S            m_shp_hdr;                                    // fields extracted from 100-byte .shp file header
    dbf_hdr_S            m_dbf_hdr;                                    // fields extracted from .dbf header and field attributes  

    std::vector<int32_t> m_offsets           {        };               // offsets to records in .shp file
    std::vector<int32_t> m_content_lengths   {        };               // content lengths of records in .shp file

    filebuf_C            m_shx_buf;                                    // buffer for .shx file 
    filebuf_C            m_shp_buf;                                    // buffer for .shp file     
    filebuf_C            m_dbf_buf;                                    // buffer for .dbf file 
};

// Associated (non-member) functions

int extract_shp_header(const filebuf_C&,       shp_hdr_S&);  
int extract_dbf_header(const filebuf_C&,       dbf_hdr_S&); 
int extract_dbf_field( const filebuf_C&, const dbf_hdr_S&, dbf_fld_S&, uint32_t, uint32_t);







// -----------------------------------------
// parms for shapefile item drawing routines
// -----------------------------------------

struct draw_parm_S
{
    bool draw_point                                                                  {false}   ;    // true    = use DrawPath()
    bool fill_point                                                                  {true}    ;    // default = use FillPath()
    bool draw_line                                                                   {true}    ; 
    bool draw_outer_poly                                                             {true}    ; 
    bool draw_inner_poly                                                             {true}    ;
    bool fill_poly                                                                   {true}    ;
                                                                             
    Gdiplus::GraphicsPath *point_path_p                                              {nullptr} ;     // brushes and pens, etc. need to be filled in before use
    Gdiplus::Pen          *point_pen_p                                               {nullptr} ;     // pen used for DrawPath() 
    Gdiplus::Brush        *point_brush_p                                             {nullptr} ;     // brush used for FillPath()
    Gdiplus::Brush        *point_background_brush_p                                  {nullptr} ;     // brush used for FillPath() when  drawing bounds rectangle
                                                                                     
    Gdiplus::Pen          *line_pen1_p                                               {nullptr} ; 
    Gdiplus::Pen          *line_pen2_p                                               {nullptr} ; 
                                                                                     
    Gdiplus::Brush        *poly_brush_p                                              {nullptr} ; 
    Gdiplus::Pen          *outer_poly_pen1_p                                         {nullptr} ; 
    Gdiplus::Pen          *outer_poly_pen2_p                                         {nullptr} ;  
    Gdiplus::Pen          *inner_poly_pen1_p                                         {nullptr} ; 
    Gdiplus::Pen          *inner_poly_pen2_p                                         {nullptr} ; 


    //  callback routine pointers
    //  -------------------------

    int                  (*point_callback_p)(    const shapefile_C *, draw_parm_S&, const mapparm_S&)  {nullptr} ; 
    int                  (*polyline_callback_p )(const shapefile_C *, draw_parm_S&, const mapparm_S&)  {nullptr} ; 
    int                  (*polygon_callback_p )( const shapefile_C *, draw_parm_S&, const mapparm_S&)  {nullptr} ;
  

    //  callback routine parms (generic pointer -- callee will have to cast into a useful local pointer)
    //  ----------------------

    void                  *point_callback_parm                                        {nullptr} ;  
    void                  *polyline_callback_parm                                     {nullptr} ; 
    void                  *polygon_callback_parm                                      {nullptr} ; 



    //  items filled in by main shapefile drawing routine
    //  ------------------------------------------------

    shapefile_C            *shapefile_p                                              {nullptr} ;     // needs to be filled in if callbacks are done  
    int32_t                record_no                                                 {-1}      ;     // invalid value until filled in   
};


// array of GDI points to be plotted, etc. (large array -- in long-duration storage -- passed to inner routines)  
// ---------------------------------------

#define MAX_GDI_PTS 1000000

struct gdipts_S
{
       Gdiplus::PointF pts[MAX_GDI_PTS];
};

 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------------------------------------------------
//     shapefile-oriented function prototypes (external)
//---------------------------------------------------------------------------------------------------------------------
  
int readin_swbd(shapefile_C&, ll_T, ll_T,           const mapparm_S&);  

int test_shapefile(mapdata_S& mapdata, outimage_S&, const mapparm_S&);

//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""