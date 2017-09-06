// map_shapefile.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =================
////     map_shapefile.cpp -- shapefile-oriented functions
////     =================
//// 
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
#include "h_map_dem.h" 
#include "h_map_tile.h" 
#include "h_map_data.h"
#include "h_map_image.h"
#include "h_map_shapefile.h"

#define PPD 216      // pixels per degree ?????? 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    Internal functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
struct ne_elevation_callback_parm_S
{
    Gdiplus::FontFamily   *font_family_p     {nullptr};
    Gdiplus::StringFormat *string_format_p   {nullptr}; 
};

static int  ne_elevation_callback(const shapefile_C *, draw_parm_S&                                                                                                                        , const mapparm_S&);
 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int  draw_shp_point(    mapdata_S&, outimage_S&, draw_parm_S&, const shp_pt_S&,                                                                                                       const mapparm_S&);
static int  draw_shp_polygon(  mapdata_S&, outimage_S&, draw_parm_S&, const shp_box_S&,  const std::vector<int32_t>&, const std::vector<int32_t>&, const std::vector< shp_pt_S>&, gdipts_S&, const mapparm_S&);
static int  draw_shp_polyline( mapdata_S&, outimage_S&, draw_parm_S&, const shp_box_S&,  const std::vector<int32_t>&, const std::vector<int32_t>&, const std::vector< shp_pt_S>&, gdipts_S&, const mapparm_S&);
static bool is_shp_poly_cw(                                                              const std::vector<shp_pt_S>&, int32_t, int32_t);



//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int test_shapefile(mapdata_S& mapdata, outimage_S& outimage, const mapparm_S& parm) try
{
    int        rc {0};

        Gdiplus::SolidBrush lakes_brush(            Gdiplus::Color(240, 255, 255)   );   // cyan-blue
        Gdiplus::Pen        lakes_pen(              Gdiplus::Color( 85, 170, 213), 1);   // cyan-blue
        Gdiplus::Pen        rivers_pen(             Gdiplus::Color( 85, 170, 213), 2);   // cyan-blue
        Gdiplus::SolidBrush historic_lakes_brush(   Gdiplus::Color(245, 250, 250)   );   // cyan-blue (lower saturation)
        Gdiplus::Pen        historic_lakes_pen(     Gdiplus::Color(137, 170, 191), 1);   // cyan-blue
        Gdiplus::SolidBrush pluvial_lakes_brush(    Gdiplus::Color(223, 223, 223)   );   // gray
        Gdiplus::Pen        pluvial_lakes_pen(      Gdiplus::Color(170, 170, 170), 1);   // cyan-blue
        Gdiplus::Pen        glacier_pen(            Gdiplus::Color(170, 170, 170), 1);   // gray
        Gdiplus::SolidBrush glacier_brush(          Gdiplus::Color(255, 255, 255)   );   // white
        Gdiplus::Pen        ice_shelf_pen(          Gdiplus::Color(160, 170, 170), 3);   // blue gray
        Gdiplus::SolidBrush ice_shelf_brush(        Gdiplus::Color(248, 255, 255)   );   // cyan white
        Gdiplus::Pen        playa_pen(              Gdiplus::Color(216, 208, 192), 1);   // tan
        Gdiplus::SolidBrush playa_brush(            Gdiplus::Color(246, 238, 222)   );   // tan
        Gdiplus::Pen        coastline_pen(          Gdiplus::Color( 85, 170, 213), 2);   // cyan-blue
        Gdiplus::Pen        reefs_pen(              Gdiplus::Color(  0,   0, 170), 2);   // blue
        Gdiplus::SolidBrush countries_brush(        Gdiplus::Color(211, 203, 187)   );   // ???



        Gdiplus::Pen        boundary0_pen1(         Gdiplus::Color(127, 170, 85,127), 12);   /*red -  magenta */    boundary0_pen1.SetLineJoin( Gdiplus::LineJoinRound); 
                                                                                                                    boundary0_pen1.SetStartCap( Gdiplus::LineCapRound);
                                                                                                                    boundary0_pen1.SetEndCap(   Gdiplus::LineCapRound);

        Gdiplus::Pen        boundary0_pen2(         Gdiplus::Color(255, 0,   0,   0), 5);   /* black */             boundary0_pen2.SetDashStyle(Gdiplus::DashStyleDashDotDot);
                                                                                                                    boundary0_pen2.SetStartCap( Gdiplus::LineCapRound);
                                                                                                                    boundary0_pen2.SetEndCap(   Gdiplus::LineCapRound);



        Gdiplus::Pen        boundary1_pen1(         Gdiplus::Color(63, 170, 0, 85), 3);   /*red -  magenta */      boundary1_pen1.SetLineJoin( Gdiplus::LineJoinRound); 
                                                                                                                   boundary1_pen1.SetStartCap( Gdiplus::LineCapRound);
                                                                                                                   boundary1_pen1.SetEndCap(   Gdiplus::LineCapRound);
        Gdiplus::Pen        boundary1_pen2(         Gdiplus::Color(  0,   0,   0), 1);   /* black */               boundary1_pen2.SetDashStyle(Gdiplus::DashStyleDot);
        Gdiplus::Pen        geographic_line_pen(    Gdiplus::Color(170,   0,   0), 3);   /* red */            geographic_line_pen.SetDashStyle(Gdiplus::DashStyleDash);

        Gdiplus::Pen        urban_pen(              Gdiplus::Color(170,  85,  85), 1);   // pale red
        Gdiplus::SolidBrush urban_brush(            Gdiplus::Color(210, 120, 120)   );   // pale red

        Gdiplus::SolidBrush ocean_0_brush(          Gdiplus::Color(240, 255, 255)   );   // light cyan blue        
        Gdiplus::SolidBrush ocean_200_brush(        Gdiplus::Color(230, 245, 250)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_1000_brush(       Gdiplus::Color(220, 235, 245)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_2000_brush(       Gdiplus::Color(210, 225, 240)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_3000_brush(       Gdiplus::Color(200, 215, 235)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_4000_brush(       Gdiplus::Color(190, 205, 230)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_5000_brush(       Gdiplus::Color(180, 195, 225)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_6000_brush(       Gdiplus::Color(170, 185, 220)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_7000_brush(       Gdiplus::Color(160, 175, 215)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_8000_brush(       Gdiplus::Color(150, 165, 210)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_9000_brush(       Gdiplus::Color(140, 155, 205)   );   // light cyan blue
        Gdiplus::SolidBrush ocean_10000_brush(      Gdiplus::Color(130, 145, 200)   );   // light cyan blue

        Gdiplus::Pen        graticule_1_pen(        Gdiplus::Color( 85, 170, 255), 1);   // cyan
        Gdiplus::Pen        graticule_5_pen(        Gdiplus::Color( 85, 170, 255), 2);   // cyan
        Gdiplus::Pen        graticule_30_pen(       Gdiplus::Color( 85, 170, 255), 4);   // cyan

        Gdiplus::FontFamily   elevation_font_family(       L"Arial Black");
        Gdiplus::StringFormat elevation_string_format;
        Gdiplus::GraphicsPath elevation_path(              Gdiplus::FillModeWinding              );    
        Gdiplus::Pen          elevation_pen(               Gdiplus::Color(        0,   0,   0), 2);   //   not used? 
        Gdiplus::SolidBrush   elevation_brush(             Gdiplus::Color(        0,   0,   0)   );   // 
        Gdiplus::SolidBrush   elevation_background_brush(  Gdiplus::Color( 127, 255, 255, 255)   );   // 

        ///////////////////////////////////////////////////////////

        // draw parms

        draw_parm_S swbd_parm; 
        draw_parm_S graticule_1_parm; 
        draw_parm_S graticule_5_parm; 
        draw_parm_S graticule_30_parm; 
        draw_parm_S elevation_parm; 
        draw_parm_S ocean_0_parm; 
        draw_parm_S ocean_200_parm; 
        draw_parm_S ocean_1000_parm; 
        draw_parm_S ocean_2000_parm;
        draw_parm_S ocean_3000_parm;
        draw_parm_S ocean_4000_parm;
        draw_parm_S ocean_5000_parm;
        draw_parm_S ocean_6000_parm;
        draw_parm_S ocean_7000_parm;
        draw_parm_S ocean_8000_parm;
        draw_parm_S ocean_9000_parm;
        draw_parm_S ocean_10000_parm;
        draw_parm_S ice_shelf_parm;
        draw_parm_S glacier_parm; 
        draw_parm_S playa_parm; 
        draw_parm_S lakes_parm; 
        draw_parm_S historic_lakes_parm; 
        draw_parm_S pluvial_lakes_parm;
        draw_parm_S rivers_parm;
        draw_parm_S coastline_parm; 
        draw_parm_S reefs_parm; 
        draw_parm_S boundary0_parm;
        draw_parm_S countries_parm; 
        draw_parm_S provinces_parm;  
        draw_parm_S urban_parm; 
        draw_parm_S geographic_line_parm;
         

        // callback parms

        ne_elevation_callback_parm_S ne_elevation_callback_parm;  


        // fill in parms


        ne_elevation_callback_parm.font_family_p   = &elevation_font_family;
        ne_elevation_callback_parm.string_format_p = &elevation_string_format;
     
        elevation_parm.fill_point                = true; 
        elevation_parm.draw_point                = false; 
        elevation_parm.point_pen_p               = &elevation_pen;   
        elevation_parm.point_brush_p             = &elevation_brush;  
        elevation_parm.point_background_brush_p  = &elevation_background_brush; 
        elevation_parm.point_path_p              = &elevation_path; 
        elevation_parm.point_callback_p          = &ne_elevation_callback; 
        elevation_parm.point_callback_parm       = &ne_elevation_callback_parm; 

        //////


        ocean_0_parm.draw_outer_poly       = false;
        ocean_0_parm.draw_inner_poly       = false;
        ocean_0_parm.poly_brush_p          = &ocean_0_brush;
        ocean_200_parm.draw_outer_poly     = false;
        ocean_200_parm.draw_inner_poly     = false;
        ocean_200_parm.poly_brush_p        = &ocean_200_brush;
        ocean_1000_parm.draw_outer_poly    = false;
        ocean_1000_parm.draw_inner_poly    = false;
        ocean_1000_parm.poly_brush_p       = &ocean_1000_brush;
        ocean_2000_parm.draw_outer_poly    = false;
        ocean_2000_parm.draw_inner_poly    = false;
        ocean_2000_parm.poly_brush_p       = &ocean_2000_brush;
        ocean_3000_parm.draw_outer_poly    = false;
        ocean_3000_parm.draw_inner_poly    = false;
        ocean_3000_parm.poly_brush_p       = &ocean_3000_brush;
        ocean_4000_parm.draw_outer_poly    = false;
        ocean_4000_parm.draw_inner_poly    = false;
        ocean_4000_parm.poly_brush_p       = &ocean_4000_brush;
        ocean_5000_parm.draw_outer_poly    = false;
        ocean_5000_parm.draw_inner_poly    = false;
        ocean_5000_parm.poly_brush_p       = &ocean_5000_brush;
        ocean_6000_parm.draw_outer_poly    = false;
        ocean_6000_parm.draw_inner_poly    = false;
        ocean_6000_parm.poly_brush_p       = &ocean_6000_brush;
        ocean_7000_parm.draw_outer_poly    = false;
        ocean_7000_parm.draw_inner_poly    = false;
        ocean_7000_parm.poly_brush_p       = &ocean_7000_brush;
        ocean_8000_parm.draw_outer_poly    = false;
        ocean_8000_parm.draw_inner_poly    = false;
        ocean_8000_parm.poly_brush_p       = &ocean_8000_brush;
        ocean_9000_parm.draw_outer_poly    = false;
        ocean_9000_parm.draw_inner_poly    = false;
        ocean_9000_parm.poly_brush_p       = &ocean_9000_brush;
        ocean_10000_parm.draw_outer_poly   = false;
        ocean_10000_parm.draw_inner_poly   = false;
        ocean_10000_parm.poly_brush_p      = &ocean_10000_brush;
        

        glacier_parm.poly_brush_p            =  &glacier_brush;
        glacier_parm.inner_poly_pen1_p       =  &glacier_pen;
        glacier_parm.outer_poly_pen1_p       =  &glacier_pen;

        ice_shelf_parm.poly_brush_p          =  &ice_shelf_brush;
        ice_shelf_parm.inner_poly_pen1_p     =  &ice_shelf_pen;
        ice_shelf_parm.outer_poly_pen1_p     =  &ice_shelf_pen;
        

        playa_parm.poly_brush_p              =  &playa_brush;
        playa_parm.inner_poly_pen1_p         =  &playa_pen;
        playa_parm.outer_poly_pen1_p         =  &playa_pen;

        lakes_parm.poly_brush_p              =  &lakes_brush;
        lakes_parm.inner_poly_pen1_p         =  &lakes_pen;
        lakes_parm.outer_poly_pen1_p         =  &lakes_pen;

        swbd_parm.poly_brush_p              =  &lakes_brush;
        swbd_parm.inner_poly_pen1_p         =  &lakes_pen;
        swbd_parm.outer_poly_pen1_p         =  &lakes_pen;




        historic_lakes_parm.poly_brush_p     =  &historic_lakes_brush;
        historic_lakes_parm.inner_poly_pen1_p=  &historic_lakes_pen;
        historic_lakes_parm.outer_poly_pen1_p=  &historic_lakes_pen;

        pluvial_lakes_parm.poly_brush_p      =  &pluvial_lakes_brush;
        pluvial_lakes_parm.inner_poly_pen1_p =  &pluvial_lakes_pen;
        pluvial_lakes_parm.outer_poly_pen1_p =  &pluvial_lakes_pen;

        rivers_parm.line_pen1_p              =  &rivers_pen;
        coastline_parm.line_pen1_p           =  &coastline_pen;
        reefs_parm.line_pen1_p               =  &reefs_pen;

        boundary0_parm.line_pen1_p           =  &boundary0_pen1;
        boundary0_parm.line_pen2_p           =  &boundary0_pen2;

        geographic_line_parm.line_pen1_p     =  &geographic_line_pen;
        graticule_1_parm.line_pen1_p         =  &graticule_1_pen;
        graticule_5_parm.line_pen1_p         =  &graticule_5_pen;
        graticule_30_parm.line_pen1_p        =  &graticule_30_pen;


        countries_parm.draw_outer_poly       = false;
        countries_parm.draw_inner_poly       = false;
        countries_parm.poly_brush_p          = &countries_brush; 

        provinces_parm.fill_poly             = false;
        provinces_parm.inner_poly_pen1_p     = &boundary1_pen1;  
        provinces_parm.inner_poly_pen2_p     = &boundary1_pen2; 
        provinces_parm.outer_poly_pen1_p     = &boundary1_pen1;
        provinces_parm.outer_poly_pen2_p     = &boundary1_pen2;

        urban_parm.poly_brush_p              =  &urban_brush;
        urban_parm.inner_poly_pen1_p         =  &urban_pen;
        urban_parm.outer_poly_pen1_p         =  &urban_pen;



        shapefile_C sf2;
        sf2.m_folder = std::wstring(L"D:\\Data03s\\GIS\\SRTM\\SWBD_v2_1\\");
        sf2.m_skip_dbf = true; 
         

        shapefile_C sf { };       
        sf.m_folder = std::wstring(L"E:\\Data01\\GIS\\Natural_Earth\\shapefile\\");
          
          // area features

     //     sf.readin(L"ne_10m_admin_0_countries"                         );            sf.draw(mapdata, outimage, countries_parm  , parm);    // brown background only
     //     sf.readin(L"ne_10m_admin_1_states_provinces"                   );           sf.draw(mapdata, outimage, provinces_parm  , parm);
     //     sf.readin(L"ne_10m_ocean"                                      );           sf.draw(mapdata, outimage, ocean_0_parm    , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_K_200"                           );           sf.draw(mapdata, outimage, ocean_200_parm  , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_J_1000"                          );           sf.draw(mapdata, outimage, ocean_1000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_I_2000"                          );           sf.draw(mapdata, outimage, ocean_2000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_H_3000"                          );           sf.draw(mapdata, outimage, ocean_3000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_G_4000"                          );           sf.draw(mapdata, outimage, ocean_4000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_F_5000"                          );           sf.draw(mapdata, outimage, ocean_5000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_E_6000"                          );           sf.draw(mapdata, outimage, ocean_6000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_D_7000"                          );           sf.draw(mapdata, outimage, ocean_7000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_C_8000"                          );           sf.draw(mapdata, outimage, ocean_8000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_B_9000"                          );           sf.draw(mapdata, outimage, ocean_9000_parm , parm);    // pale blue background only
     //     sf.readin(L"ne_10m_bathymetry_A_10000"                         );           sf.draw(mapdata, outimage, ocean_10000_parm, parm);    // pale blue background only


    //      sf.readin(L"ne_10m_glaciated_areas"                            );           sf.draw(mapdata, outimage, glacier_parm    , parm);
    //      sf.readin(L"ne_10m_antarctic_ice_shelves_polys"                );           sf.draw(mapdata, outimage, ice_shelf_parm  , parm);
    //      sf.readin(L"ne_10m_playas"                                     );           sf.draw(mapdata, outimage, playa_parm      , parm);
    //      sf.readin(L"ne_10m_urban_areas"                                );           sf.draw(mapdata, outimage, urban_parm      , parm);
        
    //      sf.readin(L"ne_10m_rivers_lake_centerlines"                    );           sf.draw(mapdata, outimage, rivers_parm     , parm);
    //      sf.readin(L"ne_10m_rivers_north_america"                       );           sf.draw(mapdata, outimage, rivers_parm     , parm);
    //      sf.readin(L"ne_10m_rivers_europe"                              );           sf.draw(mapdata, outimage, rivers_parm     , parm);

//          sf.readin(L"ne_10m_lakes_pluvial"                              );           sf.draw(mapdata, outimage, pluvial_lakes_parm , parm);
//          sf.readin(L"ne_10m_lakes_historic"                             );           sf.draw(mapdata, outimage, historic_lakes_parm, parm);
//          sf.readin(L"ne_10m_lakes"                                      );           sf.draw(mapdata, outimage, lakes_parm      , parm);
//          sf.readin(L"ne_10m_lakes_europe"                               );           sf.draw(mapdata, outimage, lakes_parm      , parm);
//          sf.readin(L"ne_10m_lakes_north_america"                        );           sf.draw(mapdata, outimage, lakes_parm      , parm);


//          shuttle water body data
//          -----------------------
 /*           
            for (int32_t j=-180; j < 180; j++)
            {
                for (int32_t i=-60; i < 61; i++)
                {
                   int rc = readin_swbd(sf2, i, j, parm);
               
 //                  if (rc == 0)
 //                      sf2.draw(mapdata, outimage, swbd_parm, parm);
                }
            }
 */     


          // graticules, etc.

//          sf.readin(L"ne_10m_graticules_1"                               );           sf.draw(mapdata, outimage, graticule_1_parm    , parm);
 //         sf.readin(L"ne_10m_graticules_5"                               );           sf.draw(mapdata, outimage, graticule_5_parm    , parm);
  //        sf.readin(L"ne_10m_graticules_30"                              );           sf.draw(mapdata, outimage, graticule_30_parm   , parm);
                    
            sf.readin(L"ne_10m_geographic_lines"                           );           sf.draw(mapdata, outimage, geographic_line_parm, parm);


          // linear features
          
          //  sf.readin(L"ne_10m_coastline"                                  );         sf.draw(mapdata, outimage, coastline_parm, parm);
            sf.readin(L"ne_10m_reefs"                                        );         sf.draw(mapdata, outimage, reefs_parm,     parm);
          //  sf.readin(L"ne_10m_minor_islands_coastline"                    );         sf.draw(mapdata, outimage, coastline_parm, parm);
      
            sf.readin(L"ne_10m_admin_0_boundary_lines_land"                );           sf.draw(mapdata, outimage, boundary0_parm, parm);
            sf.readin(L"ne_10m_admin_0_boundary_lines_maritime_indicator"  );           sf.draw(mapdata, outimage, boundary0_parm, parm);
            sf.readin(L"ne_10m_admin_0_pacific_groupings"                  );           sf.draw(mapdata, outimage, boundary0_parm, parm);


          // point features

           sf.readin(L"ne_10m_geography_regions_elevation_points"         );            sf.draw(mapdata, outimage, elevation_parm, parm);  
         



    return 0;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//   ne_elevation_callback() -- point callback function for Natural Earth spot elevations -- sets up graphic path to be drawn on map
//   =======================    ---------------------------------------------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int ne_elevation_callback(const shapefile_C *sf_p, draw_parm_S& draw_parm, const mapparm_S& parm) try
{
    ne_elevation_callback_parm_S *p = (ne_elevation_callback_parm_S *)(draw_parm.point_callback_parm);   // get local properly-typed pointer to passed-in callback parm area (contains pointer to StringFormat and FontFamily)

    M__(M_out(L"ne_elevation_callback() called -- draw_parm.record_no = %d") % draw_parm.record_no;)


    dbf_fld_S fld_0;  // scalerank   int64
    dbf_fld_S fld_1;  // featuerclass = "mountain" or other  char
    dbf_fld_S fld_2;  // name     char
    dbf_fld_S fld_3;  // elevation int64


    extract_dbf_field(draw_parm.shapefile_p->m_dbf_buf, draw_parm.shapefile_p->m_dbf_hdr, fld_0, draw_parm.record_no, 0);
    extract_dbf_field(draw_parm.shapefile_p->m_dbf_buf, draw_parm.shapefile_p->m_dbf_hdr, fld_1, draw_parm.record_no, 1);
    extract_dbf_field(draw_parm.shapefile_p->m_dbf_buf, draw_parm.shapefile_p->m_dbf_hdr, fld_2, draw_parm.record_no, 2);
    extract_dbf_field(draw_parm.shapefile_p->m_dbf_buf, draw_parm.shapefile_p->m_dbf_hdr, fld_3, draw_parm.record_no, 3);  

    M__(M_out(L"ne_elevation_callback() -- dbf recno=%-4u : scalerank=%-2d  featureclass=\"%s\" %90t name=\"%s\" %135t elevation=%d") % draw_parm.record_no % fld_0.int64 % fld_1.chars.c_str() % fld_2.chars.c_str() % fld_3.int64;)

    wchar_t buf[100]; 
    swprintf(buf, L"%I64d", fld_3.int64);


    //  Clear out point path and add in items for this call  (point path will be drawn into the graphic_S DIBSection buffers after this function returns)
    //  ---------------------------------------------------
    
    Gdiplus::Pen widen_pen {Gdiplus::Color{0,0,0}, 3};           // only thickness is used for this pen 

    draw_parm.point_path_p->Reset();                             // clear out path before adding stuff
    add_lines_r(*(draw_parm.point_path_p), 4, 6.0);              // add asterisk-like item into the graphics path 
    draw_parm.point_path_p->Widen(&widen_pen);                   // set line width of asterisk-like object

    draw_parm.point_path_p->AddString(buf, -1, p->font_family_p,  Gdiplus::FontStyleRegular, 16.0, Gdiplus::PointF {3.0, -11.0}, p->string_format_p); 


    // return with normal R/C -- will cause this path to get drawn
    // -----------------------------------------------------------

    return 0; 
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///    graphics functions for drawing 
///
///
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//      draw_shp_point()
//      ==============
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int draw_shp_point(mapdata_S& mapdata, outimage_S& outimage, draw_parm_S& draw_parm, const shp_pt_S& pt_shp, const mapparm_S& parm) try
{
   // Gdiplus::GraphicsPath  path {Gdiplus::FillModeWinding};   // local path -- starts out empty 
  //  Gdiplus::PointF        pt   {                        };
  //  Gdiplus::Matrix        mx   {                        }; 
   

    //  Call the point callback function -- can update some fields in the parm (especially the GraphicsPath, Brush, Pen, etc.), and return non-zero if this point is not to be drawn
    //  --------------------------------

    if (draw_parm.point_callback_p != nullptr)
    {
        int rc = (*(draw_parm.point_callback_p))(draw_parm.shapefile_p, draw_parm, parm);
    
        if (rc != 0)
           return 0;     
    }
    

    //  Add outline path or filled-in path (or both) to main outimage

    if (draw_parm.fill_point)
        fill_ll_path(outimage, mapdata, draw_parm.point_brush_p, draw_parm.point_background_brush_p , draw_parm.point_path_p, pt_shp.y, pt_shp.x, parm);

    if (draw_parm.draw_point)
        draw_llfix_path(outimage, mapdata, draw_parm.point_pen_p, draw_parm.point_background_brush_p, draw_parm.point_path_p, pt_shp.y, pt_shp.x, parm);
      
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//      draw_shp_polyline()
//      =================
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int draw_shp_polyline(mapdata_S& mapdata, outimage_S& outimage, draw_parm_S& draw_parm, const shp_box_S&, const std::vector<int32_t>& ptix, const std::vector<int32_t>& npoints, const std::vector< shp_pt_S>& points, gdipts_S& gdipts, const mapparm_S& parm) try
{
    shp_pt_S           pt_shp; 
   // Gdiplus::PointF    pt;
   // INT npts; 

    std::vector<real_T> v_lat { };    // input vector of latitudes  for draw_ll_lines() 
    std::vector<real_T> v_lon { };    // input vector of longitudes for draw_ll_lines()

    M__(M_out(L"draw_shp_polyline() called -- ptix.size()=%u    npoints.size()=%u    points.size()=%u") % ptix.size() % npoints.size() % points.size();)


    //  Call the polyline callback function -- can update some fields in the parm (especially the Brush, Pen, etc.), and return non-zero if this polygon is not to be drawn
    //  -----------------------------------

    if (draw_parm.polyline_callback_p != nullptr)
    {
        int rc = (*(draw_parm.polyline_callback_p))(draw_parm.shapefile_p, draw_parm, parm);
    
        if (rc != 0)
           return 0;     
    }
    

    //  Outer loop to process each multi-point line
    //  ===========================================

    for (auto i = 0; i < ptix.size(); i++ )
    {
        auto ix = ptix.at(i); 
        auto np = npoints.at(i);
      
        v_lat.clear();     // get rid of any old points from last loop pass
        v_lon.clear(); 


        // inner loop to add points to lat/lon vectors for draw_ll_lines() -- for points in single multi-point line
        // ---------------------------------------------------------------

        for (auto j = ix; j < ix + np; j++)
        {
            shp_pt_S pt_shp = points.at(j);
                      
            //    M_msg("j=%-4d   pt.x=%-20.12f    pt.x=%-20.12f", j, pt_shp.x, pt_shp.y)  

            v_lat.push_back( (real_T)(pt_shp.y) );
            v_lon.push_back( (real_T)(pt_shp.x) );
        }

        if (draw_parm.draw_line)
        {
           if (draw_parm.line_pen1_p == nullptr)
           {
               M_out_emsg(L"draw_shp_polyline() -- line_pen 1 pointer is 0");
               return -1;
           }

           draw_ll_lines(outimage, mapdata, draw_parm.line_pen1_p, v_lat, v_lon, 0.2, parm);

           if (draw_parm.line_pen2_p != nullptr)             // draw inner or 2nd line on top of first one, if required
           {   
               draw_ll_lines(outimage, mapdata, draw_parm.line_pen2_p, v_lat, v_lon, 0.2, parm);
           }
        }
    }      // end outer loop -- once per multi-point line


    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//      is_shp_poly_cw() -- determine if polygon is clockwise (outer .shp ring) or counterclockwise (inner .shp ring)
//      ================
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool is_shp_poly_cw(const std::vector<shp_pt_S>& points, int32_t ix, int32_t np) try
{
    real64_T    sum = 0.0;
    shp_pt_S    p1, p2;
    

    // Use shoelace formula to get 2x signed area of polygon ( + is clockwise, - is counterclockwise)

    M_out(L"is_shp_poly_cw() -- ix=%d  np=%d") % ix % np;
    p1 = points.at(ix);    

    for (int i = ix + 1; i < ix + np; i++)     // already have p1 for 1st point
    {
        p2 = points.at(i);
        sum += (p2.x - p1.x) * (p1.y + p2.y); 
        p1 = p2; 
    }

    M_out(L"is_shp_poly_cw() -- sum = %.12f") % sum;

    if (sum > 0.0)
        return true;
    else
        return false;
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//      draw_shp_polygon() (with holes)
//      ==================
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int draw_shp_polygon(mapdata_S& mapdata, outimage_S& outimage, draw_parm_S& draw_parm, const shp_box_S& box, const std::vector<int32_t>& ptix, const std::vector<int32_t>& npoints, const std::vector< shp_pt_S>& points, gdipts_S& gdipts, const mapparm_S& parm) try
{
    shp_pt_S           pt_shp; 
    Gdiplus::PointF    pt;
    INT npts; 
    
    Gdiplus::GraphicsPath inner_path;    // GDI+ path for inner rings
    Gdiplus::GraphicsPath outer_path;    // GDI+ path for outer rings

    
    M_out(L"draw_shp_polygon() called -- points.size() = %u") % (uint64_t)(points.size());


    //  Call the polygon callback function -- can update some fields in the parm (especially the Brush, Pen, etc.), and return non-zero if this polygon is not to be drawn
    //  ----------------------------------

    if (draw_parm.polygon_callback_p != nullptr)
    {
        int rc = (*(draw_parm.polygon_callback_p))(draw_parm.shapefile_p, draw_parm, parm);
    
        if (rc != 0)
           return 0;     
    }
    

    //  Outer loop to process each ring in the polygon
    //  ----------------------------------------------- 

    for (int i = 0; i < ptix.size(); i++ )
    {
        int32_t ix = ptix.at(i); 
        int32_t np = npoints.at(i);
        npts       = 0;                                     // clear out GDI+ polygon point list
        

        //  Inner loop to process each point in the ring -- builds point list for GDI+ polygon
        //  --------------------------------------------

        for (int j = ix; j < ix + np; j++)
        {
            shp_pt_S pt_shp = points.at(j);
      
            pt.X = (Gdiplus::REAL)PPD * (180.0 + pt_shp.x);   // 30-sec/per pixel scale
            pt.Y = (Gdiplus::REAL)PPD * ( 90.0 - pt_shp.y);
            gdipts.pts[npts++] = pt;

            if (npts >= MAX_GDI_PTS)
            {
                M_out_emsg(L"draw_shp_polygon() -- too many points in polygon ring (%d)") % np;
                return -1; 
            }
        }


        // Add this ring (polygon) to inner or outer path -- CW == outer ring, CCW = inner ring

        bool cw = is_shp_poly_cw(points, ix, np);
           
        if (cw)
            outer_path.AddPolygon(gdipts.pts, npts);      
        else          
            inner_path.AddPolygon(gdipts.pts, npts);
    }


    // If parms say to fill polygon (but not holes), create region with filled-in area between outer rings and inner rings -- blank otherwise

    if (draw_parm.fill_poly)
    {
        if (draw_parm.poly_brush_p == nullptr)
        {
            M_out_emsg(L"draw_shp_polygon() -- poly brush pointer is 0");
            return -1;
        }

        Gdiplus::Region between_region(&outer_path);
        between_region.Exclude(&inner_path);
        outimage.canvas.fill_region(draw_parm.poly_brush_p, &between_region);
    }


    // Draw outer and inner ring outlines, if required 

    if (draw_parm.draw_outer_poly)
    {  
        if (draw_parm.outer_poly_pen1_p == nullptr)
        {
            M_out_emsg(L"draw_shp_polygon() -- outer_poly_pen1 pointer is 0");
            return -1;
        }

        outimage.canvas.draw_path(draw_parm.outer_poly_pen1_p, &outer_path);

        if (draw_parm.outer_poly_pen2_p != nullptr)
        {
            outimage.canvas.draw_path(draw_parm.outer_poly_pen2_p, &outer_path);
        }
    }

    if (draw_parm.draw_inner_poly)
    {
        if (draw_parm.inner_poly_pen1_p == nullptr)
        {
            M_out_emsg("draw_shp_polygon() -- inner_poly_pen1 pointer is 0");
            return -1;
        }

        outimage.canvas.draw_path(draw_parm.inner_poly_pen1_p, &inner_path);

        if (draw_parm.inner_poly_pen2_p != nullptr)
        {
            outimage.canvas.draw_path(draw_parm.inner_poly_pen2_p, &inner_path);
        }
    }
     
    return 0; 
}
M_endf




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////        ================================
////        Member functions for shapefile_C
////        ================================
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  shapefile_C::freeup() -- get rid of any current read-in shapefiles and buffers, etc.
//  =====================    ----------------------------------------------------------- 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void shapefile_C::freeup() try
{
    M_y(M_out(L"shapefile_C::freeup() -- called");)

    m_dbf_valid        = false; 
    m_dbf_present      = false;
    m_contents_valid   = false;      // buffers will be freed
    m_contents_present = false;      // buffers will be freed


    m_offsets.clear();
    m_content_lengths.clear();

    m_shx_buf.freeup();
    m_shp_buf.freeup();
    m_dbf_buf.freeup();

    // leave folder, filetype, etc. intact 
     
    return; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  shapefile_C::readin() -- (no parms -- assume folder, basename and extensions are filled in
//  =====================    ----------------------------------------------------------------- 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int shapefile_C::readin() try
{
    int rc; 

    m_dbf_valid        = false;        // in case of error (or .dbf is being skipped this time)
    m_dbf_present      = false;        // in case of error (or .dbf is being skipped this time)
    m_contents_valid   = false;        // in case of error
    m_contents_present = false;        // in case of error
    m_offsets.clear();                 // not valid anymore
    m_content_lengths.clear();         // not valid anymore

    M__(M_out(L"shapefile_C::readin() -- called");)


    // read in files (.dbf is optional) -- return if any required file was not read in properly

    M__(M_out(L"shapefile_C::readin() -- doing readin(\"%S\")") % (m_folder + m_basename + m_shx_ft);) 

    rc = m_shx_buf.readin(m_folder + m_basename + m_shx_ft); 

    if (rc == 0)
    {
       M__(M_out(L"shapefile_C::readin() -- doing readin(\"%S\")") % (m_folder + m_basename + m_shp_ft);) 
       rc = m_shp_buf.readin(m_folder + m_basename + m_shp_ft); 
    }

    if ( (rc == 0) && (!m_skip_dbf) )
    {
        M__(M_out(L"shapefile_C::readin() -- doing readin(\"%S\")") % (m_folder + m_basename + m_dbf_ft);) 
        rc = m_dbf_buf.readin(m_folder + m_basename + m_dbf_ft);
        if (rc == 0)
            m_dbf_present = true; 
    }

    if (rc != 0)
    {
        M_out_emsg(L"shapefile_C::readin() failed because of reported errors");
        this->freeup();
        return -1;
    }

    m_contents_present = true; 
   

    // extract fields from .shp, .shx, and .dbf headers, and check for validity

    rc = extract_shp_header(m_shx_buf, m_shx_hdr);

    if (rc == 0)
        rc = extract_shp_header(m_shp_buf, m_shp_hdr); 

    if ( (rc == 0) && (!m_skip_dbf) )
        rc = extract_dbf_header(m_dbf_buf, m_dbf_hdr);

    if (rc != 0)
    {
        M_out_emsg(L"shapefile_C::readin() failed because of reported errors");
        this->freeup();
        return -1;
    }


    // Make sure bounding boxes match in .shx and .shp file headers
    // ------------------------------------------------------------

    if ( 0 != memcmp((void *)&(m_shp_hdr.box), (void *)&(m_shx_hdr.box), sizeof (shp_boxz_S)) ) 
    {
        M_out_emsg1(L"shapefile_C::readin() -- .shx file bounding box does not match .shp file bounding box");
        M_out_emsg0(L"                      -- .shx bounding box = xmin/ymin=%.12f/%.12f  xmax/ymax=%.12f/%.12f  zmin/zmax=%.12f/%.12f   mmin/mmax=%.12f/%.12f")
               % m_shx_hdr.box.xmin
               % m_shx_hdr.box.ymin
               % m_shx_hdr.box.xmax
               % m_shx_hdr.box.ymax
               % m_shx_hdr.box.zmin
               % m_shx_hdr.box.zmax
               % m_shx_hdr.box.mmin
               % m_shx_hdr.box.mmax              
               ;
        M_out_emsg2(L"                      -- .shp bounding box = xmin/ymin=%.12f/%.12f  xmax/ymax=%.12f/%.12f  zmin/zmax=%.12f/%.12f   mmin/mmax=%.12f/%.12f")
               % m_shp_hdr.box.xmin
               % m_shp_hdr.box.ymin
               % m_shp_hdr.box.xmax
               % m_shp_hdr.box.ymax
               % m_shp_hdr.box.zmin
               % m_shp_hdr.box.zmax
               % m_shp_hdr.box.mmin
               % m_shp_hdr.box.mmax              
               ;
        this->freeup(); 
        return -1; 
    }


    // make sure length of .shx file has even number of index entries > 1
    // ------------------------------------------------------------------
    
    uint32_t  num_ix    = (uint32_t)((m_shx_buf.m_ct - 100ULL) / 8ULL); 
    uint32_t  remainder = (uint32_t)((m_shx_buf.m_ct - 100ULL) % 8ULL); 
    
    if (num_ix == 0ULL)
    {
        M_out_emsg(L"shapefile_C::readin() --  .shx file length (%u) not long enough for any index entries -- filename = \"%s\"") % m_shx_buf.m_ct % m_shx_buf.m_pathname.c_str();
        this->freeup();
        return -1;   
    }

    if (remainder != 0ULL)
    {
        M_out_emsg(L"shapefile_C::readin() --  .shx file length (%u) not is not even multiple (100 + 8*N) -- filename = \"%s\"") % m_shx_buf.m_ct % m_shx_buf.m_pathname.c_str();
        this->freeup();
        return -1;   
    }
    

    // build index to records in .shp file using offsets in .shx file
    // --------------------------------------------------------------
    
    uint8_t *ix_rec_p          = m_shx_buf.p + 100ULL;                    // 1st index record starts right after 100-byte header 
    int32_t  prior_shp_rec_end = 100;                                    // .shp header ended at +100 
     
    m_offsets.reserve(        num_ix);
    m_content_lengths.reserve(num_ix);

    for (int n = 1; n <= num_ix; n++)
    {
        // add offset and content length to index vectors
         
        int32_t   shp_rec_offset         = bigend_int32( ix_rec_p + 0ULL) * 2; 
        int32_t   shp_rec_content_length = bigend_int32( ix_rec_p + 4ULL) * 2;

        m_offsets.push_back(shp_rec_offset); 
        m_content_lengths.push_back(shp_rec_content_length);


        // check for gaps and complain

        if (prior_shp_rec_end != shp_rec_offset)         
            M_out(L"shapefile_C::readin() -- ++++++++++++++++ %d-halfword gap detected between offsets %d and %d -- file=\"%S\"  record-n=%d") % (shp_rec_offset - prior_shp_rec_end) % prior_shp_rec_end % shp_rec_offset % m_shp_buf.m_pathname %n;
        
        M__(M_out(L"shapefile_C::readin() -- record-n=%-4d    offset(x2)=%-8d    content_length(x2)=%-8d   prior_end_offset(x2)=%d") % n % (shp_rec_offset * 2) % (shp_rec_content_length * 2) % (prior_shp_rec_end * 2);)

        prior_shp_rec_end = shp_rec_offset + shp_rec_content_length + 8;  // used for gap check in next loop pass

        M__(M_out(L"shapefile_C::readin() -- record-n=%-4d    offset(x2)=%-8d    content_length(x2)=%-8d   current_end_offset(x2)=%d") % n % (shp_rec_offset * 2) % (shp_rec_content_length * 2) % (prior_shp_rec_end * 2);)
       
       
        // check contents of .shp file record header pointed to by this index entry -- record number and contents length must match
        // ------------------------------------------------------------------------

        uint8_t *shp_rec_p = m_shp_buf.p + shp_rec_offset;          // point to n-th record header in .shp file

        int32_t   rec_n      = bigend_int32(shp_rec_p +  0ULL);
        int32_t   content_l  = bigend_int32(shp_rec_p +  4ULL) * 2;

        if (rec_n != n)
        {        
            this->freeup();
            M_out_emsg(L"shapefile_C::readin() --  .shx index entry %d does not point to record %d in .shp file -- filename = \"%S\"") % n % rec_n % m_shp_buf.m_pathname;
            return -1;   
        }
        
        if (content_l != shp_rec_content_length)
        {        
            this->freeup();
            M_out_emsg(L"shapefile_C::readin() --  content length (%d) in .shx index entry %d does match content length (%d) in .shp record header -- filename = \"%s\"") % shp_rec_content_length % n % content_l % m_shp_buf.m_pathname.c_str();
            return -1;   
        }
        
      
        // step to next index entry in .shx file

        ix_rec_p += 8ULL;     // step forward in .shx file to next index entry
    }


    // make sure shx and dbf files have same number of records


    M__(M_out(L"shapefile_C::readin() -- offsets.size() = %u      dbf_hdr.n_rec = %u") % offsets.size() % dbf_hdr.n_rec;)

    if ( (!m_skip_dbf) && (m_offsets.size() != m_dbf_hdr.n_rec) )
    {        
        this->freeup();
        M_out(L"shapefile_C::readin() --  number of records in .shx file (%u) not equal to number of records in .dbf file (%u)-- filename = \"%s\"") % m_offsets.size() % m_dbf_hdr.n_rec % m_dbf_buf.m_pathname.c_str();
        return -1;   
    }


    // mark everything valid and return normally 

    m_dbf_valid      = m_dbf_present;        // .dbf may not be present
    m_contents_valid = true;  
    return 0; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  shapefile_C::readin() -- (1 parm -- set basename into shapefile structure and call readin() -- assumes folder and extensions are already set 
//  =====================    ------------------------------------------------------------------ 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int shapefile_C::readin(const std::wstring& bn) try
{
    m_basename = bn; 
    return this->readin(); 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  shapefile_C::readin() -- (2 parms -- set folder and basename into shapefile structure and call readin() -- assumes extensions are already set 
//  =====================    ------------------------------------------------------------------------------ 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int shapefile_C::readin(const std::wstring& foldr, const std::wstring& bn ) try
{
    m_basename = bn; 
    m_folder   = foldr;
    return this->readin(); 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  shapefile_C::draw() -- draw out items in pre-opened shape file
//  ===================    --------------------------------------- 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int shapefile_C::draw(mapdata_S& mapdata, outimage_S& outimage, draw_parm_S& draw_parm, const mapparm_S& parm  ) try
{
    int rc; 
    gdipts_S gdipts;                                           // array of Gdiplus::Point to be drawn

    M__(M_out(L"shapefile_C::draw() called");)

    if (!m_contents_valid)
    {
         M_out_emsg(L"shapefile_C::draw() called with invalid contents in shapefile structure");   
         return -1; 
    }
    


    /// check overall bounding box in here ////  ?????????????????????????????



    //////////////////////////////////////////////////////////////////////////////////////////

    draw_parm.shapefile_p = this;         // save this in draw_parm in case drawing routines need it 

    
    /////////////////////////////////////////////////////////////////////////////////
    // main loop to look at shape records pointed to by index vector in shapefile_C
    /////////////////////////////////////////////////////////////////////////////////

    std::vector<int32_t>        part_ptix;       // vector of indexes
    std::vector<int32_t>        part_npts;       // vector of point counts
    std::vector<shp_pt_S>       points;          // vector of points
    shp_pt_S                    pt;
    shp_boxz_S                  box; 
 
    for (int n = 0; n < m_offsets.size(); n++)
    {
        uint8_t *rec_p      = m_shp_buf.p + m_offsets.at(n);
        uint8_t *content_p  = rec_p + 8ULL; 
        int32_t  content_l  = m_content_lengths.at(n); 
        int32_t  shape_type = M_int32_at(content_p + 0ULL); 

        M__(M_out(L"shapefile_C::draw() -- ***************************************************************************************************************************");)
        M__(M_out(L"shapefile_C::draw() -- n = %-6d       rec offset = %-7d       shape_type = %d") % n % (rec_p - m_shp_buf.p) % shape_type;)
    
        draw_parm.record_no = n;    // save this here, in case drawing routines need it

        if (shape_type == 0)     // Null
        {
            M_out(L"shapefile_C::draw() -- shape_type=0 (Null) not supported");        
        }
        else

        //------------------------------------------------------------------------------------------
        //  Point
        //------------------------------------------------------------------------------------------  
                                 
        if (shape_type == 1)     //  Point 
        {                       
            pt.x = M_real64_at(content_p + 4ULL ); 
            pt.y = M_real64_at(content_p + 12ULL);


            M__(M_out(L"shapefile_C::draw() -- Details for Point:  X=%.12f    Y=%.12f") % pt.x % pt.y;)     
            draw_shp_point(mapdata, outimage, draw_parm, pt, parm);
        }


        //------------------------------------------------------------------------------------------
        //  Polyline
        //------------------------------------------------------------------------------------------
        
        else                     
        if (shape_type == 3)     //  Polyline                                   
        {
             box.xmin            = M_real64_at(content_p +   4ULL );
             box.ymin            = M_real64_at(content_p +  12ULL ); 
             box.xmax            = M_real64_at(content_p +  20ULL ); 
             box.ymax            = M_real64_at(content_p +  28ULL ); 
             box.zmin            = 0.0; 
             box.zmax            = 0.0; 
             box.mmin            = 0.0; 
             box.mmax            = 0.0; 
             int32_t  num_parts  = M_int32_at( content_p +  36ULL ); 
             int32_t  num_points = M_int32_at( content_p +  40ULL ); 
             M__(M_out(L"shapefile_C::draw() -- Details for Polyline : ");)  
             M__(M_out(L"shapefile_C::draw() -- box1,2,3,4 = %-.12f, %-.12f, %-.12f, %-.12f     num_parts = %d    num_points = %d") % box.xmin % box.ymin % box.xmax % box.ymax % num_parts % num_points;)
       
             uint8_t *parts_p           =  content_p + 44ULL;     // part index starts here

             part_ptix.clear();
             part_npts.clear();
                points.clear(); 
             part_ptix.reserve(num_parts);
             part_npts.reserve(num_parts);
                points.reserve(num_points); 
           

             //  Build point index and number of points vectors 
             //  ----------------------------------------------

             for (int i = 0; i < num_parts; i++) 
             {
                 int32_t  ix = M_int32_at(parts_p + 4 * i); 
                 part_ptix.push_back(ix);      
                 M__(M_out(L"shapefile_C::draw() -- part=%-6d  index=%-6d   offset=%d") % i % ix % ((parts_p + 4 * i) - shp_buf.p);)
             }        

             for (int i = 1; i <= part_ptix.size(); i++)
             {
                 int curr_ix = part_ptix.at(i - 1);

                 if (i < part_ptix.size() )
                     part_npts.push_back(part_ptix.at(i) - curr_ix);
                 else
                     part_npts.push_back(num_points - curr_ix );             
             }
                    

             //  Build point vector for draw routine
             //  -----------------------------------

             uint8_t *points_p = content_p + 44UL + 4 * num_parts;  // points list starts after part index 
              
             for (int i = 0; i < num_points; i++) 
             {
                  pt.x = M_real64_at(points_p     + 16 * i); 
                  pt.y = M_real64_at(points_p + 8 + 16 * i);
                  points.push_back(pt);               
                  M__(M_out(L"shapefile_C::draw() -- point[%6d] = %20.12f/%20.12f -- offset=%u") % i % pt.x % pt.y % ((points_p + 8 + 16 * i) - shp_buf.p);)                
             } 
           

            //  Draw out lines from .shp file  

            draw_shp_polyline(mapdata, outimage, draw_parm, box, part_ptix, part_npts, points, gdipts, parm);
        }


        //------------------------------------------------------------------------------------------
        //  Polygon  or  PolygonZ  
        //------------------------------------------------------------------------------------------
     
        else                                                         
        if ((shape_type == 5) || (shape_type == 15))                 // Polygon  or  PolygonZ  
        {                                                            
             box.xmin            = M_real64_at(content_p +   4ULL );
             box.ymin            = M_real64_at(content_p +  12ULL ); 
             box.xmax            = M_real64_at(content_p +  20ULL ); 
             box.ymax            = M_real64_at(content_p +  28ULL ); 
             box.zmin            = 0.0; 
             box.zmax            = 0.0; 
             box.mmin            = 0.0; 
             box.mmax            = 0.0; 
             int32_t  num_parts  = M_int32_at( content_p +  36ULL ); 
             int32_t  num_points = M_int32_at( content_p +  40ULL ); 
             M_out(L"shapefile_C::draw() -- Details for Polygon or PolygonZ : ");
             M_out(L"shapefile_C::draw() -- box1,2,3,4 = %-.12f, %-.12f, %-.12f, %-.12f     num_parts = %d    num_points = %d") % box.xmin % box.ymin % box.xmax % box.ymax % num_parts % num_points;
       
             uint8_t *parts_p           =  content_p + 44ULL;     // part index starts here

             part_ptix.clear();
             part_npts.clear();
                points.clear(); 
             part_ptix.reserve(num_parts);
             part_npts.reserve(num_parts);
                points.reserve(num_points); 
           

             // build point index and number of points vectors 
             // ----------------------------------------------

             for (int i = 0; i < num_parts; i++) 
             {
                 int32_t  ix = M_int32_at(parts_p + 4 * i);  
                 part_ptix.push_back(ix);
                 M_out(L"shapefile_C::draw() -- part index %6d = %6d   offset=%d") % i % ix % ((parts_p + 4 * i) - m_shp_buf.p);
             }        

             for (int i = 1; i <= part_ptix.size(); i++)
             {
                 int curr_ix = part_ptix.at(i - 1);

                 if (i < part_ptix.size() )
                     part_npts.push_back(part_ptix.at(i) - curr_ix);
                 else
                     part_npts.push_back(num_points - curr_ix );
             
             }
                      

             // Build point vector for drawing routine
             // --------------------------------------

             uint8_t *points_p = content_p + 44UL + 4 * num_parts;  // points list starts after part index 
              
             for (int i = 0; i < num_points; i++) 
             {
                  pt.x = M_real64_at(points_p     + 16 * i); 
                  pt.y = M_real64_at(points_p + 8 + 16 * i);
                  points.push_back(pt);               
                  M_out(L"shapefile_C::draw() -- point[%6d] = %20.12f, %20.12f = %20.12f, %20.12f,    offset=%u") % i % pt.x % pt.y % (3600.0 * pt.x, 3600 * pt.y) % ((points_p + 8 + 16 * i) - m_shp_buf.p);                  
             } 
           

            // Draw out polygons from shp file  

            draw_shp_polygon(mapdata, outimage, draw_parm, box, part_ptix, part_npts, points, gdipts, parm);
         
        }

        // ------------------------------------------------------------------------------------------------------------------

        else
        if (shape_type == 8)  // Multipoint
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=8 (Multipoint) not supported") ;
        }

        else
        if (shape_type == 11)  // PointZ
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=11 (PointZ) not supported");
        }

        else
        if (shape_type == 13)  // PolylineZ
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=13 (PolylineZ) not supported") ;
        }

        else
        if (shape_type == 18)  // MultipointZ
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=18 (MultipointZ) not supported");
        }
   
        else
        if (shape_type == 21)  // PointM
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=21 (PointM) not supported");
        }
   
        else
        if (shape_type == 23)  // PolylineM
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=23 (PolylineM) not supported");
        }
    
        else
        if (shape_type == 25)  // PolygonM
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=25 (PolygonM) not supported");
        }
     
        else
        if (shape_type == 28)  // MultipointM
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=28 (MultipointM) not supported");
        }
     
        else
        if (shape_type == 31)  // Multipatch
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=31 (Multipatch) not supported");
        }
    
        else
        {
            M_out_emsg(L"shapefile_C::draw() -- shape_type=%d (?????????????) not supported") % shape_type;
        }          
    }         

    /////////////////////////////////////////////////// 
    // end of loop to process each record in .shp file  
    ///////////////////////////////////////////////////
    
    return 0;
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////
//////    Non-member functions related to shapefile_C class
//////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  extract_shp_header -- extract fields from .shp or .shx header
//  ==================    --------------------------------------- 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int extract_shp_header(const filebuf_C& fbuf, shp_hdr_S& hdr) try 
{
    if (fbuf.m_ct < 100ULL)
    {
        M__(M_out_emsg(L"extract_shp_header() -- file size for \"%S\" is less than 100 bytes") % fbuf.m_pathname % fbuf.m_ct;)    
        return -1;
    }


    hdr.file_code   = bigend_int32( fbuf.p1 +  0ULL); 
    hdr.file_length = bigend_int32( fbuf.p1 + 24ULL) * 2;
    hdr.version     = M_int32_at(   fbuf.p1 + 28ULL);
    hdr.shapes_type = M_int32_at(   fbuf.p1 + 32ULL);
    hdr.box.xmin    = M_real64_at(  fbuf.p1 + 36ULL);
    hdr.box.ymin    = M_real64_at(  fbuf.p1 + 44ULL);
    hdr.box.xmax    = M_real64_at(  fbuf.p1 + 52ULL);
    hdr.box.ymax    = M_real64_at(  fbuf.p1 + 60ULL);
    hdr.box.zmin    = M_real64_at(  fbuf.p1 + 68ULL);
    hdr.box.zmax    = M_real64_at(  fbuf.p1 + 76ULL);
    hdr.box.mmin    = M_real64_at(  fbuf.p1 + 84ULL);
    hdr.box.mmax    = M_real64_at(  fbuf.p1 + 92ULL);

    M__(M_out(L"extract_shp_header() -- shapefile header for file \"%S\"") % fbuf.pathname;)
    
    M__(M_out(L"extract_shp_header() -- file_code       = %d"     ) % hdr.file_code      ;)
    M__(M_out(L"extract_shp_header() -- file_length(x2) = %d"     ) % hdr.file_length    ;)
    M__(M_out(L"extract_shp_header() -- version         = %d"     ) % hdr.version        ;)
    M__(M_out(L"extract_shp_header() -- shapestype      = %d"     ) % hdr.shapes_type    ;)
    M__(M_out(L"extract_shp_header() -- box.xmin        = %.12f"  ) % hdr.box.xmin       ;)
    M__(M_out(L"extract_shp_header() -- box.xmax        = %.12f"  ) % hdr.box.xmax       ;)
    M__(M_out(L"extract_shp_header() -- box.ymin        = %.12f"  ) % hdr.box.ymin       ;)
    M__(M_out(L"extract_shp_header() -- box.ymax        = %.12f"  ) % hdr.box.ymax       ;)
    M__(M_out(L"extract_shp_header() -- box.zmin        = %.12f"  ) % hdr.box.zmin       ;)
    M__(M_out(L"extract_shp_header() -- box.zmax        = %.12f"  ) % hdr.box.zmax       ;)
    M__(M_out(L"extract_shp_header() -- box.mmin        = %.12f"  ) % hdr.box.mmin       ;)
    M__(M_out(L"extract_shp_header() -- box.mmax        = %.12f"  ) % hdr.box.mmax       ;)
         
    if (hdr.file_code != 9994)
    {
        M_out_emsg(L"extract_shp_header() -- file code in header = %d -- should be 9994 for file \"%s\"") % hdr.file_code % fbuf.m_pathname.c_str();    
        return -1;
    }

    if ((uint64_t)(hdr.file_length) != fbuf.m_ct)
    {
        M_out_emsg(L"extract_shp_header() -- file length in header = %d -- should be %I64u for file \"%s\"") % hdr.file_length % fbuf.m_ct, fbuf.m_pathname.c_str();    
        return -1;
    }

    if (hdr.version != 1000)
    {
        M_out_emsg(L"extract_shp_header() -- version in header = %d -- should be 1000 for file \"%s\"") % hdr.version % fbuf.m_pathname.c_str();    
        return -1;
    }
    
    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  extract_dbf_field -- extract n-th field from m-th .dbf record in file  (both parms 0-based)
//  =================    ------------------------------------------------ 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int extract_dbf_field(const filebuf_C& fbuf, const dbf_hdr_S& hdr, dbf_fld_S& fld, uint32_t rec_n, uint32_t fld_n) try
{
    // error if field_n exceeds hdr.n_fld
    // error if rec_n exceeds hdr.n_rec

    M__(M_out("extract_dbf_field -- rec_n=%u  fld_n=%u") % rec_n % fld_n;)


    if (fld_n >= hdr.n_fld)
    {
        M_out_emsg(L"extract_dbf_field() -- requested field number (%u) exceeds last field (%u) in record") % fld_n % (hdr.n_fld - 1U) ;
        return -1; 
    }
    if (rec_n >= hdr.n_rec)
    {
        M_out_emsg(L"extract_dbf_field() -- requested record number (%u) exceeds last record (%u) in file") % rec_n % (hdr.n_rec - 1U) ;
        return -1; 
    }


    //  locate n-th field in m-th record in .dbf file 
   
    dbf_fd_S fd  = hdr.fd.at(fld_n);                                                            // locate field descriptor for n-th field in each record  
    

    M__(M_out(L"extract_dbf_field() -- record=%-5u   field=%-3u   field-name=\"%-10S\"   offset=%-5u     length=%-3u   dec_ct=%-2u") % rec_n % fld_n % to_wstring(fd.name) % fd.offset % (int)(fd.length) % fd.dec_ct;)  


    char *fld_p  = (char *)(fbuf.p1 + hdr.hdr_len + rec_n * hdr.rec_len + fd.offset);           // point to start of field  -- note: not wide char

 
    // extract all characters -and- characters without leading and trailing blanks 
    
    fld.all_chars.assign(fld_p, (std::string::size_type)(fd.length));                 // extract all characters
    

    /*
    if (fld.all_chars.size() > 0)
    {
        std::string::size_type first = fld.all_chars.find_first_not_of(' ');          // find 1st non-blank char (if any) 

        if (first != std::string::npos)                                               // string not all blank? 
        {
            fld.chars = fld.all_chars.substr(first);                                  // leading blanks are gone

            std::string::size_type last = fld.all_chars.find_last_not_of(' ');        // find last non-blank char 

            if (last != std::string::npos)
                fld.chars = fld.chars.substr(0, last + 1);                            // trim off trailing blanks
        }
    }
    */
       
    fld.chars = boost::trim_copy(fld.all_chars);  
    fld.type  = fd.type; 

    if (fld.type == dbf_ft_E::numeric)
    {
        fld.dec_places = fd.dec_ct;

        if (fld.dec_places = 0)
        {
            fld.int64  = std::stoll(fld.chars);
            fld.real64 = fld.int64;        
        }
        else
        {
            fld.real64 = std::stod(fld.chars);
            fld.int64  = fld.real64;    
        }
    }
    else
    {
        fld.dec_places = -1; 
        fld.real64     = 0.0;
        fld.int64      = 0;     
    }
    
    M__(M_out(L"extract_dbf_field() -- record=%-5u   field=%-3u   field-name=\"%-10S\"   offset=%-5u     length=%-3u   dec_ct=%-2u   real64=%30.12f  int64=%-22d   char=\"%S\"") % rec_n % fld_n % to_wstring(fd.name) % fd.offset % ((int)(fd.length)) % fd.dec_ct % fld.real64 % fld.int64 % to_wstring(fld.chars);)        
    
    return 0; 
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  extract_dbf_header -- extract fields from .dbf
//  ==================    ------------------------ 
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int extract_dbf_header(const filebuf_C& fbuf, dbf_hdr_S& hdr) try 
{
    if (fbuf.m_ct < 32ULL)
    {
        M__(M_out(L"extract_dbf_header() -- file size for \"%s\" is less than 32 bytes") % fbuf.pathname.c_str() % fbuf.m_ct ;)    
        return -1;
    }


    //  Get fields from 1st part of header
    //  ----------------------------------

    hdr.version     =          M_uint8_at(   fbuf.p1 +  0ULL);    // ideally 2 or 3
    hdr.yy          =          M_uint8_at(   fbuf.p1 +  1ULL);
    hdr.mm          =          M_uint8_at(   fbuf.p1 +  2ULL);
    hdr.dd          =          M_uint8_at(   fbuf.p1 +  3ULL);
    hdr.n_rec       =         M_uint32_at(   fbuf.p1 +  4ULL); 
    hdr.hdr_len     = (uint32_t)M_uint16_at( fbuf.p1 +  8ULL);    // can also be start of 1st data record
    hdr.rec_len     = (uint32_t)M_uint16_at( fbuf.p1 + 10ULL);

    hdr.file_len    = (uint64_t)(hdr.hdr_len) + (uint64_t)(hdr.n_rec) * (uint64_t)(hdr.rec_len);  


    // compute number of fields based on header length (check for errors) 

    if ( (hdr.hdr_len - 1U) % 32U != 0 )   // final header byte is 0x0D byte
    {
        M_out_emsg(L"extract_dbf_header() -- header len = %u -- not one more than an even multiple of 32 bytes") % hdr.hdr_len;
        return -1;    
    }

    if (hdr.hdr_len < 32)
    {
        M_out_emsg(L"extract_dbf_header() -- header len (%u) is less than 32 bytes -- too short to compure number of fields") % hdr.hdr_len;
        return -1;    
    }
    
    hdr.n_fld = (hdr.hdr_len - 1U) / 32U - 1U; 


    // Make sure claimed size is not longer than actual file length -- make sure 0 or 1 extra bytes at end (1 extra byte should be 0x1A)

    if (hdr.file_len > fbuf.m_ct)
    {
        M_out_emsg(L"extract_dbf_header() -- hdr_len (%u) + n_rec (%u) * rec_len (%u) = %I64u exceeds actual file length (%u)") % hdr.hdr_len % hdr.n_rec % hdr.rec_len % hdr.file_len % fbuf.m_ct;
        return -1;    
    }

    if (hdr.file_len + 1U < fbuf.m_ct)
        M_out(L"extract_dbf_header() -- extra space at end of file: hdr_len (%u) + n_rec (%u) * rec_len (%u) = %u -- actual file length (%u)") % hdr.hdr_len % hdr.n_rec % hdr.rec_len % hdr.file_len % fbuf.m_ct;

    if ( (hdr.file_len + 1U == fbuf.m_ct) && (fbuf.p1[hdr.file_len] != 0x1A) )                                               // if 1 extra byte at end, it should be 0x1A 
        M_out(L"extract_dbf_header() -- extra byte at end of file = 0x%02X -- should be 0x1A") % fbuf.p1[hdr.file_len] ;






    M__(M_out(L"extract_dbf_header() -- shapefile .dbf header for file \"%S\"") % fbuf.m_pathname;)
    
    M__(M_out(L"extract_dbf_header() -- version       = %d"             ) % hdr.version                                                  ;)
    M__(M_out(L"extract_dbf_header() -- YY/MM/DD      = %u/%02u/%02u"   ) % (uint32_t)(hdr.yy) % (uint32_t)(hdr.mm) % (uint32_t)(hdr.dd) ;)
    M__(M_out(L"extract_dbf_header() -- n_rec         = %u"             ) % hdr.n_rec                                                    ;)
    M__(M_out(L"extract_dbf_header() -- hdr_len       = %u"             ) % hdr.hdr_len                                                  ;)
    M__(M_out(L"extract_dbf_header() -- rec_len       = %u"             ) % hdr.rec_len                                                  ;)
    M__(M_out(L"extract_dbf_header() -- file_len(ct)  = %u (%u)"        ) % hdr.file_len % fbuf.ct                                       ;)
    M__(M_out(L"extract_dbf_header() -- n_fld         = %u"             ) % hdr.n_fld                                                    ;)
  

    //  Build vector of field descriptors
    //  ---------------------------------

    uint8_t *fd_p = fbuf.p1 +  32ULL;    // point just past 1st 32-byte part of header
    uint32_t offset = 1U;                // 1st field starts after delete indicator (blank or *)

    hdr.fd.clear();                      // remove anything from last time

    for (int i = 0; i < hdr.n_fld; i++)
    { 
        char buf[20]; 
        dbf_fd_S fd;                                             // one .dbf field descriptor 
     

        sprintf(buf, "%.10s", (char *)&M_uint8_at(fd_p +  0ULL));
        fd.name     =  buf;                                      // should be NULL-terminated 
        fd.type_c   =  (char   )M_uint8_at(fd_p + 11ULL); 
        
        switch (fd.type_c)
        {
        case 'B' :    fd.type = dbf_ft_E::binary;       break;   // can also be double
        case 'C' :    fd.type = dbf_ft_E::character;    break; 
        case 'D' :    fd.type = dbf_ft_E::date;         break; 
        case 'F' :    fd.type = dbf_ft_E::floating_pt;  break; 
        case 'G' :    fd.type = dbf_ft_E::general;      break; 
        case 'I' :    fd.type = dbf_ft_E::integer;      break;
        case 'L' :    fd.type = dbf_ft_E::logical;      break;
        case 'M' :    fd.type = dbf_ft_E::memo;         break; 
        case 'N' :    fd.type = dbf_ft_E::numeric;      break; 
        case 'P' :    fd.type = dbf_ft_E::picture;      break; 
        case 'T' :    fd.type = dbf_ft_E::datetime;     break; 
        case 'Y' :    fd.type = dbf_ft_E::currency;     break; 
        default :     fd.type = dbf_ft_E::unknown;      break; 
        }
    
        fd.length = (uint32_t)M_uint8_at(fd_p + 16ULL);
        fd.offset = offset; 
        fd.dec_ct = (uint32_t)M_uint8_at(fd_p + 17ULL);

        hdr.fd.push_back(fd);             // add this field descriptor to end of vector in extracted .dbf header

        M__(M_out(L"extract_dbf_header() -- adding field %-4d  :  name=\"%-11S\"   type_c=%c    offset=%-6u (0x%04X)    length=%-4u   dec_ct=%u") % i % to_wstring(fd.name) % fd.type_c % fd.offset % fd.offset % fd.length % fd.dec_ct;)

        offset += fd.length;              // add this field to current offset for next field  
        fd_p   += 32;                     // strp to next field descriptor -- 32-bytes each 
    }

    // log out completed vector with field descriptors

 //   for (int i = 0; i < hdr.fd.size(); i++)
 //   {
 //       dbf_fd_S fd = hdr.fd.at(i);  
 //       M__(M_out(L"extract_dbf_header() -- field %-4d  :  name=\"%-11S\"   type_c=%c    offset=%-6u (0x%04X)    length=%-4u   dec_ct=%u") % i % to_wstring(fd.name) % fd.type_c % fd.offset % fd.offset % fd.length % fd.dec_ct;)    
 //   }
    

    // make sure current offset = record length specified in header  

    if (offset != hdr.rec_len)
    {
        M_out_emsg(L"extract_dbf_header() -- last field ending offset (%u) not same as record length (%u) in header") % offset % hdr.rec_len;
        return -1;    
    }
        
    return 0; 
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  readin_swbd() -- read in SWBD file for caller's passed-in latitde/longitude
//  =============    ------------------------ ---------------------------------
//
//
//  note: caller needs to fill in shapefile_C folder/path already -- this routine generates only the filename(s)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int readin_swbd(shapefile_C& sf, ll_T south, ll_T west, const mapparm_S& parm) try
{
    std::string          filename;  
    std::string          suffix; 
    struct               _stat64 stat_buf;
    int                  stat_rc;       
  

    // Fill in tile_S with SWBD tiling info used to generate filenames   ??????????? needs to be redone need fileinfo_S for shapefiles ????????????????????
    // ---------------------------------------------------------------

    fileinfo_S fileinfo { }; 

    fileinfo.fn_sw_corner      = true; 
    fileinfo.fn_upper_case     = false; 
    fileinfo.fn_lat_then_lon   = false; 
    fileinfo.fn_global         = false; 
    fileinfo.multi_ft          = false;     // look for .shp only
    
    fileinfo.limit_s        = -M_arcsec_90; 
    fileinfo.limit_w        = -M_arcsec_180;
    fileinfo.limit_n        =  M_arcsec_90;
    fileinfo.limit_e        =  M_arcsec_180;

    fileinfo.origin_ns      = -M_arcsec_90;  
    fileinfo.origin_ew      = -M_arcsec_180;
    fileinfo.step_ns        =  M_arcsec_1;
    fileinfo.step_ew        =  M_arcsec_1;

    fileinfo.folder         = sf.m_folder;  
    fileinfo.prefix         = L"";  
  //  fileinfo.suffix         = L"";           // will be set to a, i, s, n, e, etc.
    fileinfo.filetype       = sf.m_shp_ft;     // just look for .shp files  

    tile_S tile { }; 
    reset_tile(tile);
    display_tile(tile); 


    // try to find a file for caller's lat, lon, and get filesize
    // ----------------------------------------------------------

 //   fileinfo.suffix = L"a";    ???????????????????

    tile_filename(tile, south, west, M_arcsec_1, M_arcsec_1, fileinfo); 
    stat_rc  = _wstat64(tile.filename.c_str(), &stat_buf);
    M_out(L"readin_sdwb() -- stat_rc(...a) = %d") % stat_rc;
        
    if (stat_rc != 0)
    {
        // fileinfo.suffix = L"e"; 
        tile_filename(tile, south, west, M_arcsec_1, M_arcsec_1, fileinfo);   
        stat_rc  = _wstat64(tile.filename.c_str(), &stat_buf);
        M_out(L"readin_sdwb() -- stat_rc(...e) = %d") % stat_rc;
    }

    if (stat_rc != 0)
    {
        // fileinfo.suffix = L"f"; 
        tile_filename(tile, south, west, M_arcsec_1, M_arcsec_1, fileinfo);   
        stat_rc  = _wstat64(tile.filename.c_str(), &stat_buf);
        M_out(L"readin_sdwb() -- stat_rc(...f) = %d") % stat_rc;
    }

    if (stat_rc != 0)
    {
        // fileinfo.suffix = L"i"; 
        tile_filename(tile, south, west, M_arcsec_1, M_arcsec_1, fileinfo);   
        stat_rc  = _wstat64(tile.filename.c_str(), &stat_buf);
        M_out(L"readin_sdwb() -- stat_rc(...i) = %d") % stat_rc;
    }

    if (stat_rc != 0)
    {
        // fileinfo.suffix = L"n"; 
        tile_filename(tile, south, west, M_arcsec_1, M_arcsec_1, fileinfo);   
        stat_rc  = _wstat64(tile.filename.c_str(), &stat_buf);
        M_out(L"readin_sdwb() -- stat_rc(...n) = %d") % stat_rc;
    }

    if (stat_rc != 0)
    {
        // fileinfo.suffix = L"s";      ??????????????????????????????????????????????????????????
        tile_filename(tile, south, west, M_arcsec_1, M_arcsec_1, fileinfo);  
        stat_rc  = _wstat64(tile.filename.c_str(), &stat_buf);
        M_out(L"readin_sdwb() -- stat_rc(...s) = %d") % stat_rc;
    }

    if (stat_rc != 0) 
        return -1;


    uint64_t sz = (uint64_t)(stat_buf.st_size);

    M_out(L"readin_sdwb() -- stat() size = %I64u -- filename=\"%s") % sz % filename.c_str(); 


    // Copy basename from tile_S to shapefile_S and shp.readin()

    sf.m_basename = tile.basename; 

    return sf.readin(sf.m_basename);   
}
M_endf

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////