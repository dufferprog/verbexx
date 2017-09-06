// h_map_data.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////            ============
////            h_map_data.h -- functions and other items for map_data.cpp  
////            ============
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
#pragma once
#include "h_map_types.h"

       
//---------------------------------------------------------------------------------------------------------------------
//     Map data structure
//---------------------------------------------------------------------------------------------------------------------

union project_U
{
    // Rectangular projection-specific items

    struct 
    {
        llr_T     south;             // south edge -- degrees
        llr_T     west;              // west  edge -- degrees
        llr_T     north;             // north edge -- degrees 
        llr_T     east;              // east  edge -- degrees
        real_T    factor_ns;         // N-S stretch factor
        real_T    ppd_ew;            // number of data points per degree - E-W
        real_T    ppd_ns;            // number of dada points per degree - N-S            
    } rect;


    // Mercator projection-specific items

    struct 
    {
        llr_T     south;             // south edge -- degrees
        llr_T     west;              // west  edge -- degrees
        llr_T     north;             // north edge -- degrees 
        llr_T     east;              // east  edge -- degrees
        real_T    ppd_ew;            // number of data points per degree - E-W
        real_T    r;                 // Mercator radius = R   
        real_T    y_s;               // Mercator y value at south edge
        real_T    y_n;               // Mercator y value at north edge         
    } merc;


    // Polar projection-specific items

    struct 
    {
        real_T    ppd;               // points per degree along meridians
        llr_T     lon0;              // basic longitude (points downward) -- degrees
        rcf_T     row0;              // pole is at this row 
        rcf_T     col0;              // pole is at this column  
        real_T    lat_adj;           // latitude adjust factor
    } polar;


    // Stereographic projection-specific items -- lat0, lon0 always at center of map area

    struct 
    {
        real_T    r;                 // stereographic radius = ppd * 180/pi
        llr_T     lat0;              // latitude of center
        llr_T     lon0;              // longitude of center
        llr_T     row0;              // lat0, lon0 is at this row 
        llr_T     col0;              // lat0, lon0 is at this column  
        real_T    sin_lat0;          // M_sind(lat0) -- for speedier transform functions
        real_T    cos_lat0;          // M_cosd(lat0) -- for speedier transform functions
    } stereo;


    // Orthographic projection-specific items -- lat0, lon0 always at center of map area

    struct 
    {
        real_T    r;                 // orthographic radius = ppd * 180/pi
        llr_T     lat0;              // latitude of center
        llr_T     lon0;              // longitude of center
        llr_T     row0;              // lat0, lon0 is at this row 
        llr_T     col0;              // lat0, lon0 is at this column  
        real_T    sin_lat0;          // M_sind(lat0) -- for speedier transform functions
        real_T    cos_lat0;          // M_cosd(lat0) -- for speedier transform functions
    } ortho;


    // Azimuthal equidistant projection-specific items -- lat0, lon0 always at center of map area

    struct 
    {
        real_T    r;                 // orthographic radius = ppd * 180/pi
        llr_T     lat0;              // latitude of center
        llr_T     lon0;              // longitude of center
        llr_T     row0;              // lat0, lon0 is at this row 
        llr_T     col0;              // lat0, lon0 is at this column  
        real_T    sin_lat0;          // M_sind(lat0) -- for speedier transform functions
        real_T    cos_lat0;          // M_cosd(lat0) -- for speedier transform functions
    } azim;


    // Azimuthal Equal Area projection-specific items -- lat0, lon0 always at center of map area

    struct 
    {
        real_T    r;                 // orthographic radius = ppd * 180/pi
        llr_T     lat0;              // latitude of center
        llr_T     lon0;              // longitude of center
        llr_T     row0;              // lat0, lon0 is at this row 
        llr_T     col0;              // lat0, lon0 is at this column  
        real_T    sin_lat0;          // M_sind(lat0) -- for speedier transform functions
        real_T    cos_lat0;          // M_cosd(lat0) -- for speedier transform functions
    } az_ea;



};



struct mapdata_S
{
    rc_T      cols                  { 0    };           // number of columns of data
    rc_T      rows                  { 0    };           // number of rows of data
    uint64_t  npts                  { 0    };           // number of points = rows * columns 
                                                       
    rcf_T     maxrow                { 0    };           // maximum row number
    rcf_T     maxcol                { 0    };           // maximum col number 
                                                        
    llr_T     ref_latitude          { 0.0  };           // refererence latitude -- for filenames, etc.
    llr_T     ref_longitude         { 0.0  };           // reference longitude -- for filenames, etc.
    real_T    ref_ppd               { 0.0  };           // reference pixels per degree -- for filenames, etc.
    llr_T     ref_extent_ew         { 0.0  };           // reference extent E-W -- for names, etc.      
    llr_T     ref_extent_ns         { 0.0  };           // reference extent N-S -- for filenames, etc.
                                            
    llr_T     north_limit           { 0.0  };           // northern limit for plotted area
    llr_T     south_limit           { 0.0  };           // southern limit for plotted area
    llr_T     east_limit            { 0.0  };           // eastern  limit for plotted area
    llr_T     west_limit            { 0.0  };           // western  limit for plotted area
                                        
    llr_T     min_latitude          { 0.0  };           // minimum latitude anywhere on map (can be lower than actual value) 
    llr_T     max_latitude          { 0.0  };           // maximum latitude anywhere on map (can be higher than actual value) 
    llr_T     min_longitude         { 0.0  };           // minimum longitude anywhere on map (can be lower than actual value)
    llr_T     max_longitude         { 0.0  };           // maximum longitude anywhere on map (can be higher than actual value)
    real_T    min_ppd               { 0.0  };           // minimum pixels per degree anywhere on map (E-W or N-S, whichever is smaller) 
    real_T    max_ppd               { 0.0  };           // maximum pixels per degree anywhere on map (E-W or N-S, whichever is higher)


    bool      map_window_valid      {false };         // true when set_map_window has completed
    rc_T      map_window_height     { 0    };         // height of map window 
    rc_T      map_window_width      { 0    };         // width of map window 
    rc_T      map_window_top_row    { 0    };         // top row of map window                
    rc_T      map_window_bottom_row { 0    };         // bottom row of map window  
    rc_T      map_window_left_col   { 0    };         // left column of map window                
    rc_T      map_window_right_col  { 0    };         // right column of map window  


    // pointers to generic functions -- transform and inverse transform functions

    int       (*get_lat_lon_p)(const mapdata_S&, llr_T&, llr_T&, rcf_T, rcf_T, bool)  {nullptr}; // get latitude and longitude, given row/col
    int       (*get_row_col_p)(const mapdata_S&, rcf_T&, rcf_T&, llr_T, llr_T, bool)  {nullptr}; // get row/col, given lat/lon


    // pointers to allocated buffers, etc.
    
    buffer_C<int16_t>  buf          {      };
    buffer_C<attr_T>   attr         {      };
    buffer_C<int8_t>   slope        {      }; 

    // following part differs, depending on the projection

    projection_T  projection;     // projection used (mercator, stereographic, etc.)
    project_U u;                  // projection-specific data

}; 


#define M_mapdata_ht(        m,r,c) (((m).buf.p  )[(int64_t)(r) * (int64_t)((m).cols) + (int64_t)(c)])
#define M_mapdata_attr(      m,r,c) (((m).attr.p )[(int64_t)(r) * (int64_t)((m).cols) + (int64_t)(c)])
#define M_mapdata_slope(     m,r,c) (((m).slope.p)[(int64_t)(r) * (int64_t)((m).cols) + (int64_t)(c)])

#define M_mapdata_is_water(m, r, c) (M_mapdata_attr((m),(r),(c)) & MAPDATA_WATER)
#define M_mapdata_is_valid(m, r, c) (M_mapdata_attr((m),(r),(c)) & MAPDATA_VALID)


// return codes (*from get_row_col_p)() functions -- note: some code is dependent on value ordering balow
// ----------------------------------------------

#define M_ROW_COL_RC_OK          0        // returned row/column is valid
#define M_ROW_COL_RC_OUTMAP     -1        // returned row/column is out-of-bounds for this map (but within plotting limits and input lat/lon are within map projection area) 
#define M_ROW_COL_RC_OUTLIMITS  -2        // input lat/lon is not within specified plotting limits (if any) 
#define M_ROW_COL_RC_OUTPROJ    -3        // no valid row/col -- beyond limits of map projection 
#define M_ROW_COL_RC_INVALID    -4        // input lat/lon is not valid (i.e. latitude not between -90.0 and 90.0) 
#define M_ROW_COL_RC_ERROR      -5        // should-not-occur error occurred 

//---------------------------------------------------------------------------------------------------------------------
//     create_map parms
//---------------------------------------------------------------------------------------------------------------------

struct create_map_parm_S
{
public:
    // common parms for all projection types

    int32_t        display                    { 0                         } ;         // display level -- 0 = none, 1 = some, 2 = more details 
    projection_T   projection                 { projection_E::mercator    } ;         // default = mercator projection
    int32_t        center_ppd                 { 1200                      } ;         // map scale at center or other reference point (or perhaps everywhere)

    llr_T          north_limit                { 100.0000                  } ;         // northern limit for plotted area
    llr_T          south_limit                { -100.0000                 } ;         // southern limit for plotted area
    llr_T          east_limit                 { 1000.0                    } ;         // eastern  limit for plotted area
    llr_T          west_limit                 { -1000.0                   } ;         // western  limit for plotted area 
   
    int32_t        samples                    { 300000                    } ;         // number of samples to take when looking for max/min lat/lon, etc.
    llr_T          padding                    { 0.5                       } ;         // extra padding (in degrees) when computing minlat/maxlat/minlon/maxlon 


    // Mercator projection-specific items
      
    struct 
    {
        real_T     south                      { 41.0                      } ;         // for mercator projection -- south edge (deg)
        real_T     west                       { -75.0                     } ;         // for mercator projection -- west edge (deg)
        real_T     deg_ew                     { 1.0                       } ;         // for mercator projection -- E-W extent 
        real_T     deg_ns                     { 1.0                       } ;         // for mercator projection -- N-S extent        
    } merc;


    // Rectangular projection-specific items

    struct 
    {
        real_T     south                      { 41.0                      } ;         // for rectangular projection -- south edge (deg)
        real_T     west                       { -75.0                     } ;         // for rectangular projection -- west edge (deg)
        real_T     deg_ew                     { 1.0                       } ;         // for rectangular projection -- E-W extent 
        real_T     deg_ns                     { 1.0                       } ;         // for rectangular projection -- N-S extent
        real_T     ns_factor                  { 0.0                       } ;         // for rectangular projection -- stretch factor  <= 0.0  means dynamic latitude-dependent default = secant of average latitude
                                                                                      //                                               > 0.0 means use this value   
    } rect;                                                                           
    
    
    // (modified) Polar projection-specific items
    
    struct 
    {                                         
        real_T     lon0                       { 0.0                       } ;         // for (modified) polar projection -- base longitude (points downward)
        real_T     deg_rows                   { 15.0                      } ;         // for (modified) polar projection -- center row covers this many degrees, if pole is at center 
        real_T     deg_cols                   { 15.0                      } ;         // for (modified) polar projection -- center col covers this many degrees, if pole is at center
        real_T     deg_row0                   { 7.5                       } ;         // for (modified) polar projection -- center is offset this far down from top (in degrees)  
        real_T     deg_col0                   { 7.5                       } ;         // for (modified) polar projection -- center is offset this far over from left edge (in degrees)  
        real_T     lat_adj                    { 0.0                       } ;         // for (modified) polar projection -- latitude adjust factor  
    } polar;
    

    // Azimuthal projection-specific items -- lat0, lon0 always at center of map area
    
    struct 
    {
        real_T     lat0                       { 41.0                      } ;       // for azimuthal projection -- latitude of center point
        real_T     lon0                       { -75.0                     } ;       // for azimuthal projection -- longitude of center point 
        real_T     deg_cols                   { 1.0                       } ;       // for azimuthal projection -- horizontal extent (degrees)
        real_T     deg_rows                   { 1.0                       } ;       // for azimuthal projection -- vertical extent (degrees)  
        int32_t    cols                       { 0                         } ;       // for azimuthal projection -- cols (non-zero: overrrides deg_cols)   
        int32_t    rows                       { 0                         } ;       // for azimuthal projection -- rows (non-zero: overrides deg rows)
    } azim;


    // Azimuthal equal area projection-specific items -- lat0, lon0 always at center of map area
    
    struct 
    {
        real_T     lat0                       { 41.0                      } ;       // for azimuthal equal area projection -- latitude of center point
        real_T     lon0                       { -75.0                     } ;       // for azimuthal equal area projection -- longitude of center point 
        real_T     deg_cols                   { 1.0                       } ;       // for azimuthal equal area projection -- horizontal extent (degrees)
        real_T     deg_rows                   { 1.0                       } ;       // for azimuthal equal area projection -- vertical extent (degrees)  
        int32_t    cols                       { 0                         } ;       // for azimuthal equal area projection -- cols (non-zero: overrrides deg_cols)   
        int32_t    rows                       { 0                         } ;       // for azimuthal equal area projection -- rows (non-zero: overrides deg rows)
    } az_ea;
   

    // Stereographic projection-specific items -- lat0, lon0 always at center of map area
    
    struct 
    {
        real_T     lat0                       { 41.0                      } ;       // for stereographic projection -- latitude of center point
        real_T     lon0                       { -75.0                     } ;       // for stereographic projection -- longitude of center point 
        real_T     deg_cols                   { 1.0                       } ;       // for stereographic projection -- horizontal extent (degrees)
        real_T     deg_rows                   { 1.0                       } ;       // for stereographic projection -- vertical extent (degrees)  
        int32_t    cols                       { 0                         } ;       // for stereographic projection -- cols (non-zero: overrrides deg_cols)   
        int32_t    rows                       { 0                         } ;       // for stereographic projection -- rows (non-zero: overrides deg rows)
    } stereo;  


    // Orthographic projection-specific items -- lat0, lon0 always at center of map area
    
    struct 
    {
        real_T     lat0                       { 41.0                      } ;       // for orthographic projection -- latitude of center point
        real_T     lon0                       { -75.0                     } ;       // for orthographic projection -- longitude of center point 
        real_T     deg_cols                   { 1.0                       } ;       // for orthographic projection -- horizontal extent (degrees)
        real_T     deg_rows                   { 1.0                       } ;       // for orthographic projection -- vertical extent (degrees)  
        int32_t    cols                       { 0                         } ;       // for orthographic projection -- cols (non-zero: overrrides deg_cols)   
        int32_t    rows                       { 0                         } ;       // for orthographic projection -- rows (non-zero: overrides deg rows)
    } ortho; 
};  



//---------------------------------------------------------------------------------------------------------------------
//     fill_map parms
//---------------------------------------------------------------------------------------------------------------------

struct fill_map_parm_S
{
public:
    int32_t        display                    { 0                         } ;         // display level -- 0 = none, 1 = some, 2 = more details 
    int32_t        sampling_method            { 2                         } ;         // raster sampling method 1 = nearest neighbor, 2 = linear interpolate (default)
    bool           normal_fill                { true                      } ;         // fill all points in map        
    bool           ocean_fill                 { false                     } ;         // fill only the points marked as ocean  
};  





//---------------------------------------------------------------------------------------------------------------------
//     slope_map parms
//---------------------------------------------------------------------------------------------------------------------

enum class light_dir_E { n, ne, e, se, s, sw, w, nw, above };

struct slope_map_parm_S
{
public:
    bool           display             { false           } ;         // true -- do slopes debug display 
    bool           do_land             { true            } ;         // true -- do land slopes 
    bool           do_ocean            { true            } ;         // true -- do ocean slopes 
    light_dir_E    light_dir           { light_dir_E::nw } ;         // lighting direction for shaded relief 
    real_T         land_vert_factor    { 5.0             } ;         // vertical exaggeration factor when computing land slopes 
    real_T         ocean_vert_factor   { 10.0            } ;         // vertical exaggeration factor when computing ocean slopes 
    int32_t        reuse_horiz_spacing { 10              } ;         // reuse computed horizontal spacing up to this many columns away
};


//---------------------------------------------------------------------------------------------------------------------
//     graticule_map parms
//---------------------------------------------------------------------------------------------------------------------
 
struct graticule_map_parm_S
{
public:
    bool        display         { false     };          // true -- do graticule debug display 
    int32_t     weight          { 2         };          // graticule  thickness (pixels H or V)            
    ll_T        ns_spacing      { 3600      };          // N-S graticule spacing (arc-seconds)
    ll_T        ew_spacing      { 3600      };          // E-W graticule spacing (arc-seconds)
    int32_t     reduce_1        { 1         };          // Reduction factor above/below 1st hi latitide
    int32_t     reduce_2        { 1         };          // Reduction factor above/below 2nd hi latitide
    int32_t     reduce_3        { 1         };          // Reduction factor above/below 3rd hi latitide
    lld_T       hi_lat_1        { 80        };          // 1st latutude (N or S) for reduction -- should be lowest (checked last) -- in degrees
    lld_T       hi_lat_2        { 85        };          // 2nd latutude (N or S) for reduction                                    -- in degrees
    lld_T       hi_lat_3        { 89        };          // 3rd latutude (N or S) for reduction -- should be highest (checked 1st) -- in degrees
};





//---------------------------------------------------------------------------------------------------------------------
//     mapdata_t external function prototypes
//---------------------------------------------------------------------------------------------------------------------

mapdata_S *find_map(   int64_t   );      // locate map in repository with passed-in ID
mapdata_S *alloc_map(  int64_t   );      // make sure map is allocated in repository for passed-in ID (passes back simple pointer address of mapdata_S) 
void       free_map(   int64_t   );      // make sure map is not allocated in repository for passed-in ID (caller should get rid of everything in mapdata_S first) 



int      create_map(            mapdata_S& ,                     const create_map_parm_S&,        const mapparm_S&); 
void     clear_temp_map(        mapdata_S& , rc_T, rc_T, rc_T, rc_T                                            ); 
void     destroy_map(           mapdata_S& ,                                                      const mapparm_S&);
void     slice_map(       const mapdata_S& , thpa_S&,   int32_t,                                  const mapparm_S&); 
void     slice_map_row(   const mapdata_S& , thpa_S&,   int32_t,                                  const mapparm_S&); 
void     slice_map_col(   const mapdata_S& , thpa_S&,   int32_t,                                  const mapparm_S&); 
void     fill_map(              mapdata_S& , const raster_S&,    const fill_map_parm_S&,          const mapparm_S&);
void     water_map(             mapdata_S& ,                                                      const mapparm_S&);   // in water.cpp
void     graticule_map(         mapdata_S& ,                     const graticule_map_parm_S&,     const mapparm_S&);
void     slope_map(             mapdata_S& ,                     const slope_map_parm_S&,         const mapparm_S&);  
 
int      get_rc_ppd(      const mapdata_S& , real_T&, real_T&, rcf_T, rcf_T,                      const mapparm_S&);
int      get_ll_ppd(      const mapdata_S& , real_T&, real_T&, llr_T, llr_T,                      const mapparm_S&);
int      get_ll_rotation( const mapdata_S& , real_T&, real_T&, llr_T, llr_T,                      const mapparm_S&);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////