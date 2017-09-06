// map_data.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ============
////     map_data.cpp -- mapdata_S-oriented functions 
////     ============
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
#include "h_map_thd.h"

#include "h_map_raster.h"
#include "h_map_data.h"




// ==================================================================================================================
//                                       Static variables
// ==================================================================================================================

namespace static_N
{

// mapdata_S repository

static std::map<int64_t, std::shared_ptr<mapdata_S>> maps { }; 

}






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ------------------------------------------
// function prototypes for internal functions
// ------------------------------------------


static int          create_mapdata(            mapdata_S&, rc_T, rc_T,                                          const create_map_parm_S&,    const mapparm_S&);                                                                                                                                             
static int          create_map_rect(           mapdata_S&,                                                      const create_map_parm_S&,    const mapparm_S&);
static int          create_map_merc(           mapdata_S&,                                                      const create_map_parm_S&,    const mapparm_S&);
static int          create_map_polar(          mapdata_S&,                                                      const create_map_parm_S&,    const mapparm_S&);
static int          create_map_azim(           mapdata_S&,                                                      const create_map_parm_S&,    const mapparm_S&);
//static int          create_map_az_ea(          mapdata_S&,                                                      const create_map_parm_S&,    const mapparm_S&);
static int          create_map_stereo(         mapdata_S&,                                                      const create_map_parm_S&,    const mapparm_S&);
static int          create_map_ortho(          mapdata_S&,                                                      const create_map_parm_S&,    const mapparm_S&);
                                                                                                                                            
static int          get_lat_lon_rect(    const mapdata_S&, llr_T&, llr_T&, rcf_T, rcf_T, bool = true);                                  
static int          get_row_col_rect(    const mapdata_S&, rcf_T&, rcf_T&, llr_T, llr_T, bool = true);                                  
                                                                                                                                            
static int          get_lat_lon_merc(    const mapdata_S&, llr_T&, llr_T&, rcf_T, rcf_T, bool = true);                                        
static int          get_row_col_merc(    const mapdata_S&, rcf_T&, rcf_T&, llr_T, llr_T, bool = true);                                        
                                                                                                                                            
static int          get_lat_lon_polar(   const mapdata_S&, llr_T&, llr_T&, rcf_T, rcf_T, bool = true);                                         
static int          get_row_col_polar(   const mapdata_S&, rcf_T&, rcf_T&, llr_T, llr_T, bool = true);                                         
                                                                                                                                            
static int          get_lat_lon_stereo(  const mapdata_S&, llr_T&, llr_T&, rcf_T, rcf_T, bool = true);                                         
static int          get_row_col_stereo(  const mapdata_S&, rcf_T&, rcf_T&, llr_T, llr_T, bool = true);                                         
 
static int          get_lat_lon_ortho(   const mapdata_S&, llr_T&, llr_T&, rcf_T, rcf_T, bool = true);                                         
static int          get_row_col_ortho(   const mapdata_S&, rcf_T&, rcf_T&, llr_T, llr_T, bool = true);       

//static int          get_lat_lon_azim(    const mapdata_S&, llr_T&, llr_T&, rcf_T, rcf_T, bool = true);                                         
//static int          get_row_col_azim(    const mapdata_S&, rcf_T&, rcf_T&, llr_T, llr_T, bool = true);     

//static int          get_lat_lon_az_ea(    const mapdata_S&, llr_T&, llr_T&, rcf_T, rcf_T, bool = true);                                         
//static int          get_row_col_az_ea(    const mapdata_S&, rcf_T&, rcf_T&, llr_T, llr_T, bool = true);    

static void         fill_map_thd(              mapdata_S&, const raster_S&, thp_S&, const fill_map_parm_S&,                                  const mapparm_S&);
static void         slope_map_thd(             mapdata_S&,                  thp_S&, const slope_map_parm_S&,                                 const mapparm_S&);
 
static int          get_min_max_ppd(     const mapdata_S&, real_T&, real_T&,                                     const create_map_parm_S&,   const mapparm_S&);
static int          get_min_max_ll(      const mapdata_S&, llr_T&, llr_T&, llr_T&, llr_T&,                       const create_map_parm_S&,   const mapparm_S&);
static int          get_edge_lat_lon(    const mapdata_S&, llr_T&, llr_T&, llr_T&, llr_T&,                       const create_map_parm_S&,   const mapparm_S&); 
static int          get_samples_lat_lon( const mapdata_S&, llr_T&, llr_T&, llr_T&, llr_T&,                       const create_map_parm_S&,   const mapparm_S&);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        find_map() -- find map(n) in repository (return nullptr, if not there)  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

mapdata_S* find_map(int64_t map_id) try
{
    // make sure there is exactly one map in repository with caller's ID
    // -----------------------------------------------------------------

     auto map_ct = static_N::maps.count(map_id); 

     if (map_ct <= 0)
     {
         M__(M_out(L"find_map() -- map(%d) not in repository") % map_id;) 
         return nullptr; 
     }
     else
     if (map_ct > 1)
     {
         M_out_emsg(L"find_map() -- invalid maps.count(%d) = %d") % map_id % map_ct;  
         return nullptr; 
     }


     // one shared_ptr<mapdata_S> found -- return mapdata_S address as simple pointer
     // -----------------------------------------------------------------------------

     M__(M_out(L"find_map() -- map(%d) at %p found in repository") % map_id % static_N::maps.at(map_id).get();) 
     return static_N::maps.at(map_id).get();
}
M_endf 




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        alloc_map() -- make sure map is allocated in repository for this ID  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

mapdata_S* alloc_map(int64_t map_id) try
{
    // pass back any existing mapdata_S in repository
    // ----------------------------------------------

    mapdata_S *p = find_map(map_id);

    if (p != nullptr)
    {
        M__(M_out(L"alloc_map() -- existing map(%d) in repository at %p being returned") % map_id % p;) 
        return p;
    }


    // need to allocate new (default initialized) mapdata_S and add to the repository
    //-------------------------------------------------------------------------------

    std::shared_ptr<mapdata_S> sp(new mapdata_S);     // this sp should go away when function returns
    static_N::maps.emplace(map_id, sp);       // this sp should stay around until this ID is deallocated

    M__(M_out(L"alloc_map() -- new map(%d) was allocated at %p and added to repository") % map_id % sp.get();)


    // pass back simple address of newly-allocated mapdata_S

    return sp.get();                                        
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        free_map() -- make sure no map is allocated in repository for this ID  
//|| 
//||                           (caller must have gotten rid of everything in the mapdata_S before calling) 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void free_map(int64_t map_id) try
{
    // get rid of any existing mapdata_S in repository
    // -----------------------------------------------

    if (static_N::maps.count(map_id) > 0)
    {
        M__(M_out(L"free_map() -- map(%d) being erased from repository") % map_id;)
        static_N::maps.erase(map_id);   
    }
    else
    {
        M__(M_out(L"free_map() -- map(%d) not allocated in repository") % map_id;)
    }
   
    return;                                           
}
M_endf      







//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_map() 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int create_map(mapdata_S& mapdata, const create_map_parm_S& mparm, const mapparm_S& parm)  try
{
    int rc;
   
    if (mparm.display >= 2)
    {
        M_out(L"map:     parm-center_ppd    = %d"              ) % mparm.center_ppd    ;
        M_out(L"map:     parm-samples       = %d"              ) % mparm.samples       ;
        M_out(L"map:     parm-padding       = %-12.6f"         ) % mparm.padding       ;
        M_out(L"map:     parm-south_limit   = %-12.6f"         ) % mparm.south_limit   ;
        M_out(L"map:     parm-north_limit   = %-12.6f"         ) % mparm.north_limit   ;
        M_out(L"map:     parm-west_limit    = %-12.6f"         ) % mparm.west_limit    ;
        M_out(L"map:     parm-east_limit    = %-12.6f"         ) % mparm.east_limit    ;       
    }
       
    switch (mparm.projection)
    {
        case projection_E::rectangular :
            
            if (mparm.display >= 2)
            {
                M_out(L"map:     parm-projection    = rectangluar"     )                       ;
                M_out(L"map:     parm-south         = %-12.6f"         ) % mparm.rect.south    ;
                M_out(L"map:     parm-west          = %-12.6f"         ) % mparm.rect.west     ;
                M_out(L"map:     parm-deg_ns        = %-12.6f"         ) % mparm.rect.deg_ns   ;
                M_out(L"map:     parm-deg_ew        = %-12.6f"         ) % mparm.rect.deg_ew   ;
                M_out(L"map:     parm-ns_factor     = %-12.6f"         ) % mparm.rect.ns_factor;
            } 

            rc = create_map_rect(mapdata, mparm, parm); 
            break; 



        case projection_E::mercator :

            if (mparm.display >= 2)
            {
                M_out(L"map:     parm-projection    = mercator"        )                       ;
                M_out(L"map:     parm-south         = %-12.6f"         ) % mparm.merc.south    ;
                M_out(L"map:     parm-west          = %-12.6f"         ) % mparm.merc.west     ;
                M_out(L"map:     parm-deg_ns        = %-12.6f"         ) % mparm.merc.deg_ns   ;
                M_out(L"map:     parm-deg_ew        = %-12.6f"         ) % mparm.merc.deg_ew   ;                 
            } 

            rc = create_map_merc(mapdata, mparm, parm); 
            break;
            


        case projection_E::polar :

            if (mparm.display >= 2)
            {
                M_out(L"map:     parm-projection    = polar"           )                          ;                 
                M_out(L"map:     parm-lon0          = %-12.6f"         ) % mparm.polar.lon0       ;
                M_out(L"map:     parm-deg_rows      = %-12.6f"         ) % mparm.polar.deg_rows   ;
                M_out(L"map:     parm-deg_cols      = %-12.6f"         ) % mparm.polar.deg_cols   ;
                M_out(L"map:     parm-deg_row0      = %-12.6f"         ) % mparm.polar.deg_row0   ;  
                M_out(L"map:     parm-deg_col0      = %-12.6f"         ) % mparm.polar.deg_col0   ;
                M_out(L"map:     parm-lat_adj       = %-12.6f"         ) % mparm.polar.lat_adj    ;         
            } 

            rc = create_map_polar(mapdata, mparm, parm); 
            break;


        case projection_E::azimuthal :

            if (mparm.display >= 2)
            {
                M_out(L"map:     parm-projection    = azimuthal"   )                             ;                 
                M_out(L"map:     parm-lat0          = %-12.6f"         ) % mparm.azim.lat0       ;
                M_out(L"map:     parm-lon0          = %-12.6f"         ) % mparm.azim.lon0       ;
                M_out(L"map:     parm-deg_cols      = %-12.6f"         ) % mparm.azim.deg_cols   ;
                M_out(L"map:     parm-deg_rows      = %-12.6f"         ) % mparm.azim.deg_rows   ;  
                M_out(L"map:     parm-cols          = %-12.6f"         ) % mparm.azim.cols       ;
                M_out(L"map:     parm-rows          = %-12.6f"         ) % mparm.azim.rows       ;         
            } 

            rc = create_map_azim(mapdata, mparm, parm); 
            break;
      

         case projection_E::azimuthal_ea :

            if (mparm.display >= 2)
            {
                M_out(L"map:     parm-projection    = azimuthal equal area"   )                   ;                 
                M_out(L"map:     parm-lat0          = %-12.6f"         ) % mparm.az_ea.lat0       ;
                M_out(L"map:     parm-lon0          = %-12.6f"         ) % mparm.az_ea.lon0       ;
                M_out(L"map:     parm-deg_cols      = %-12.6f"         ) % mparm.az_ea.deg_cols   ;
                M_out(L"map:     parm-deg_rows      = %-12.6f"         ) % mparm.az_ea.deg_rows   ;  
                M_out(L"map:     parm-cols          = %-12.6f"         ) % mparm.az_ea.cols       ;
                M_out(L"map:     parm-rows          = %-12.6f"         ) % mparm.az_ea.rows       ;         
            } 

           // rc = create_map_az_ea(mapdata, mparm, parm); 
            break;


        case projection_E::stereographic :

            if (mparm.display >= 2)
            {
                M_out(L"map:     parm-projection    = stereographic"   )                           ;                 
                M_out(L"map:     parm-lat0          = %-12.6f"         ) % mparm.stereo.lat0       ;
                M_out(L"map:     parm-lon0          = %-12.6f"         ) % mparm.stereo.lon0       ;
                M_out(L"map:     parm-deg_cols      = %-12.6f"         ) % mparm.stereo.deg_cols   ;
                M_out(L"map:     parm-deg_rows      = %-12.6f"         ) % mparm.stereo.deg_rows   ;  
                M_out(L"map:     parm-cols          = %-12.6f"         ) % mparm.stereo.cols       ;
                M_out(L"map:     parm-rows          = %-12.6f"         ) % mparm.stereo.rows       ;         
            } 

            rc = create_map_stereo(mapdata, mparm, parm); 
            break;
      


        case projection_E::orthographic :

            if (mparm.display >= 2)
            {
                M_out(L"map:     parm-projection    = orthographic"    )                          ;                
                M_out(L"map:     parm-lat0          = %-12.6f"         ) % mparm.ortho.lat0       ;
                M_out(L"map:     parm-lon0          = %-12.6f"         ) % mparm.ortho.lon0       ;
                M_out(L"map:     parm-deg_cols      = %-12.6f"         ) % mparm.ortho.deg_cols   ;
                M_out(L"map:     parm-deg_rows      = %-12.6f"         ) % mparm.ortho.deg_rows   ;  
                M_out(L"map:     parm-cols          = %-12.6f"         ) % mparm.ortho.cols       ;
                M_out(L"map:     parm-rows          = %-12.6f"         ) % mparm.ortho.rows       ;         
            } 

            rc = create_map_ortho(mapdata, mparm, parm); 
            break;



        default :     // unknown projection
            M_out_emsg(L"Unknown projection type = %d") %  (int32_t)(mparm.projection);
            rc = -1; 
            break;
    }


    if ( (rc == 0) && (mparm.display >= 1) )
        M_out(L"Map     : limits -- S/N= %.6f / %.6f    W/E= %.6f / %.6f")  % mapdata.min_latitude % mapdata.max_latitude % mapdata.min_longitude % mapdata.max_longitude ; 

    return rc; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_map_rect() 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//   Local functions for rectangular projection rows, cols, lat, lon 
//   ---------------------------------------------------------------


static int get_lat_lon_rect(const mapdata_S& mapdata, llr_T& lat_r, llr_T& lon_r, rcf_T row, rcf_T col, bool error_check) try
{
    if ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
    {
        llr_T lat = mapdata.u.rect.north - row / mapdata.u.rect.ppd_ns;
        llr_T lon = mapdata.u.rect.west  + col / mapdata.u.rect.ppd_ew; 


        // do lat/lon limits checking before returning lat/lon

        if  (            
             (lat > mapdata.north_limit) ||
             (lat < mapdata.south_limit) ||
             (lon > mapdata.east_limit)  ||
             (lon < mapdata.west_limit)   
            )
        {
            M__(M_out(L"get_lat_lon_rect() -- lat/lon=%-20.10f/%-20.10f   north/south=%-20.10f/%-20.10f  east/west=%-20.10f/%-20.10f") % lat % lon % mapdata.north_limit % mapdata.south_limit % mapdata.east_limit % mapdata.west_limit;)
            lat_r = -90.0;
            lon_r = -180.0;         
            return -1;           // return error -- results are out of limits
        }
        else
        {
            M__(M_out(L"row=%-20.10f  col=%-20.10f ---->  lat=%-20.10f   lon=%-20.10f") % row % col % lat % lon;)
            lat_r = lat; 
            lon_r = lon;
            return 0;             // return OK -- results should be valid   
        }
    }
    else
    {
        lat_r = -90.0;
        lon_r = -180.0; 
        return -1; 
    }
}
M_endf



static int get_row_col_rect(const mapdata_S& mapdata, rcf_T& row_r, rcf_T& col_r, llr_T lat, llr_T lon, bool error_check) try
{
    // do lat/lon limits checking before returning getting row/col

    if ( (lat > 90.0) || (lat < -90.0) )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_INVALID;           // return error -- input lat/lon is invalid
    }  

    if  (            
         (lat > mapdata.north_limit) ||
         (lat < mapdata.south_limit) ||
         (lon > mapdata.east_limit)  ||
         (lon < mapdata.west_limit)   
        )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_OUTLIMITS;           // return error -- input lat/lon is out-of bounds
    }


    //  get row/col, since input lat/lon is within limits
    //  -------------------------------------------------
   
    if ( (lat >= mapdata.u.rect.south) && (lat <= mapdata.u.rect.north) && (lon >= mapdata.u.rect.west) && (lon <= mapdata.u.rect.east) )
    {
        row_r = (mapdata.u.rect.north - lat) * mapdata.u.rect.ppd_ns;
        col_r = (lon - mapdata.u.rect.west) * mapdata.u.rect.ppd_ew;        
        return M_ROW_COL_RC_OK; 
    }
    else
    {
        row_r = (mapdata.u.rect.north - lat) * mapdata.u.rect.ppd_ns;     // will be out of bounds
        col_r = (lon - mapdata.u.rect.west) * mapdata.u.rect.ppd_ew;      // will be out of bounds
        return M_ROW_COL_RC_OUTMAP; 
    }


    // error -- should not get here
    // ----------------------------

    row_r = 0.0;
    col_r = 0.0;         
    return M_ROW_COL_RC_ERROR;           // return unexpected error


}
M_endf



/////////////////////////////////////////////////////////////////////////////////


static int create_map_rect(mapdata_S& mapdata, const create_map_parm_S& mparm, const mapparm_S& parm) try
{
    int      rc = 0;
    int32_t  rows;
    int32_t  cols;
    real_T   factor_ns;
    real_T   ppd_ns; 

  
    
    real_T south, west, deg_ns, deg_ew, ppd_ew;


    south    = mparm.rect.south;  
    west     = mparm.rect.west; 
    deg_ew   = mparm.rect.deg_ew; 
    deg_ns   = mparm.rect.deg_ns; 
    ppd_ew   = (real_T)(mparm.center_ppd); 
      

    // Compute size of map  

    if (mparm.rect.ns_factor > 0.0)
        factor_ns = mparm.rect.ns_factor; 
    else
        factor_ns = M_min( 24.0, M_secd(south + 0.5*deg_ns) ); 
                            
    ppd_ns = ppd_ew * factor_ns;
             
    cols = 1 + M_round(deg_ew * ppd_ew);  
    rows = 1 + M_round(deg_ns * ppd_ns);
    
   M__(M_out(L"factor_ns  = %12.6f") % factor_ns;)
   M__(M_out(L"rows       = %d"    ) % rows;)
   M__(M_out(L"cols       = %d"    ) % cols;)
  

    // Create basic mapdata_t structure -- return imemdiately, if error

    rc = create_mapdata(mapdata, rows, cols, mparm, parm);
    
    if (rc != 0) 
        return rc; 
                                              
    
    // Fill in common items directly from mparms
    // -----------------------------------------

    mapdata.north_limit  =  mparm.north_limit; 
    mapdata.south_limit  =  mparm.south_limit; 
    mapdata.east_limit   =  mparm.east_limit; 
    mapdata.west_limit   =  mparm.west_limit; 
                                              

    // fill in rectangular projection-specific items
    // ---------------------------------------------
    
    mapdata.projection       = projection_E::rectangular;
    mapdata.u.rect.south     = south; 
    mapdata.u.rect.west      = west;
    mapdata.u.rect.north     = south + deg_ns;
    mapdata.u.rect.east      = west  + deg_ew;
    mapdata.u.rect.factor_ns = factor_ns;
    mapdata.u.rect.ppd_ew    = ppd_ew; 
    mapdata.u.rect.ppd_ns    = ppd_ew * factor_ns;


    // fill in projection-dependent generic functions
    // ----------------------------------------------
      
    mapdata.get_lat_lon_p   = &get_lat_lon_rect;
    mapdata.get_row_col_p   = &get_row_col_rect;



    // fill in projection-dependent standard items
    // -------------------------------------------

    mapdata.ref_latitude   = south + 0.5 * deg_ns;               // middle latitude 
    mapdata.ref_longitude  = west  + 0.5 * deg_ew;               // middle longitude
    mapdata.ref_ppd        = ppd_ew * (1.0 + factor_ns)/2.0;     // average scale factor
    mapdata.ref_extent_ew  = deg_ew; 
    mapdata.ref_extent_ns  = deg_ns; 

    mapdata.min_latitude   = south; 
    mapdata.min_longitude  = west; 
    mapdata.max_latitude   = south + deg_ns; 
    mapdata.max_longitude  = west + deg_ew;    
    mapdata.min_ppd        = ppd_ew * M_min(1.0, factor_ns);
    mapdata.max_ppd        = ppd_ew * M_max(1.0, factor_ns); 


    // debug display, if requested
    
    if (mparm.display >= 2)
    {
        M_out(L"Rectangular projection parms: -----------------------------------");
        M_out(L"south                = %-.20f")   %  mapdata.u.rect.south          ;    
        M_out(L"west                 = %-.20f")   %  mapdata.u.rect.west           ;     
        M_out(L"north                = %-.20f")   %  mapdata.u.rect.north          ;    
        M_out(L"east                 = %-.20f")   %  mapdata.u.rect.east           ;     
        M_out(L"factor_ns            = %-.20f")   %  mapdata.u.rect.factor_ns      ;
        M_out(L"ppd_ew               = %-.20f")   %  mapdata.u.rect.ppd_ew         ;   
        M_out(L"ppd_ns               = %-.20f")   %  mapdata.u.rect.ppd_ns         ;  
        M_out(L"rows                 = %d"    )   %  mapdata.rows                  ;
        M_out(L"cols                 = %d"    )   %  mapdata.cols                  ;
        M_out(L"ref_latitude         = %-.20f")   %  mapdata.ref_latitude          ;  
        M_out(L"ref_longitude        = %-.20f")   %  mapdata.ref_longitude         ;  
        M_out(L"ref_ppd              = %-.20f")   %  mapdata.ref_ppd               ;  
        M_out(L"ref_extent_ew        = %-.20f")   %  mapdata.ref_extent_ew         ; 
        M_out(L"ref_extent_ns        = %-.20f")   %  mapdata.ref_extent_ns         ;
        M_out(L"north_limit          = %-.20f")   %  mapdata.north_limit           ;
        M_out(L"south_limit          = %-.20f")   %  mapdata.south_limit           ;
        M_out(L"east_limit           = %-.20f")   %  mapdata.east_limit            ;
        M_out(L"west_limit           = %-.20f")   %  mapdata.west_limit            ;
        M_out(L"min_latitude         = %-.20f")   %  mapdata.min_latitude          ; 
        M_out(L"max_latitude         = %-.20f")   %  mapdata.max_latitude          ;  
        M_out(L"min_longitude        = %-.20f")   %  mapdata.min_longitude         ; 
        M_out(L"max_longitude        = %-.20f")   %  mapdata.max_longitude         ;
        M_out(L"min_ppd              = %-.20f")   %  mapdata.min_ppd               ; 
        M_out(L"max_ppd              = %-.20f")   %  mapdata.max_ppd               ;
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
//||       create_map_merc() 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// MERC map: Mercator transform -- parm is latitude in degrees -- gives y/R

#define M_mercator_yr(d) log(tan(M_PI_4 + M_rad(d)/2.0))  


// MERC map :Mercator inverse transform -- parm = y/R -- output is latitude in degrees

#define M_mercator_i(yr)  M_deg(2.0 * atan(exp(yr)) - M_PI_2)    



//   Local functions for Mercator rows, cols, lat, lon 
//   -------------------------------------------------

static int get_lat_lon_merc(const mapdata_S& mapdata, llr_T&  lat_r, llr_T& lon_r, rcf_T row, rcf_T col, bool error_check) try
{
    if ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
    {
        real_T yr  = (mapdata.u.merc.y_n - row) / mapdata.u.merc.r;   // compute y/r for given row
        llr_T  lat = M_mercator_i(yr);  
        llr_T  lon = mapdata.u.merc.west + col / mapdata.u.merc.ppd_ew; 

        // do lat/lon limits checking before returning lat/lon

        if  (            
             (lat > mapdata.north_limit) ||
             (lat < mapdata.south_limit) ||
             (lon > mapdata.east_limit)  ||
             (lon < mapdata.west_limit)   
            )
        {
            M__(M_out(L"get_lat_lon_merc() -- lat/lon=%-20.10f/%-20.10f   north/south=%-20.10f/%-20.10f  east/west=%-20.10f/%-20.10f") % lat % lon % mapdata.north_limit % mapdata.south_limit % mapdata.east_limit % mapdata.west_limit;)
            lat_r = -90.0;
            lon_r = -180.0;         
            return -1;           // return error -- results are out of limits
        }
        else
        {
            M__(M_out(L"row=%-20.10f  col=%-20.10f ---->  lat=%-20.10f   lon=%-20.10f") % row % col % lat % lon;)
            lat_r = lat; 
            lon_r = lon;
            return 0;             // return OK -- results should be valid   
        }
    }
    else
    {
        lat_r = -90.0;
        lon_r = -180.0; 
        return -1; 
    }
}
M_endf


static int get_row_col_merc(const mapdata_S& mapdata, rcf_T& row_r, rcf_T& col_r, llr_T lat, llr_T lon, bool error_check) try
{
    // do lat/lon limits checking before getting row/col

    if ( (lat > 90.0) || (lat < -90.0) )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_INVALID;           // return error -- input lat/lon is not valid
    }   


    if  (            
         (lat > mapdata.north_limit) ||
         (lat < mapdata.south_limit) ||
         (lon > mapdata.east_limit)  ||
         (lon < mapdata.west_limit)   
        )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_OUTLIMITS;           // return error -- input lat/lon is out of plotted area
    }

    if ( (lat == 90.0) || (lat == -90.0) )      // only points not covered by projection are north and south poles
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_OUTPROJ;           // return error -- input lat/lon is off projected area
    }   

 
    //  get row/col, since input lat/lon is within limits for this map and projection
    //  -----------------------------------------------------------------------------

    real_T yr  = M_mercator_yr(lat); 
    row_r = mapdata.u.merc.y_n - mapdata.u.merc.r * yr;
    col_r = (lon - mapdata.u.merc.west) * mapdata.u.merc.ppd_ew; 


    if ( (lat >= mapdata.u.merc.south) && (lat <= mapdata.u.merc.north) && (lon >= mapdata.u.merc.west) && (lon <= mapdata.u.merc.east) )
    {
        return M_ROW_COL_RC_OK; 
    }
    else
    {
        return M_ROW_COL_RC_OUTMAP; 
    }


    // should not get here
    // -------------------

    row_r = 0.0;
    col_r = 0.0;         
    return M_ROW_COL_RC_ERROR;           // return should-not-occur error 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////


static int create_map_merc(mapdata_S& mapdata, const create_map_parm_S& mparm, const mapparm_S& parm) try                 
{
    int      rc = 0;
    rc_T     rows;
    rc_T     cols;
    real_T   r;              // mercator radius
    real_T   y_n;            // mercator y value at north edge
    real_T   y_s;            // mercator y value at south edge
    
    llr_T south, west, deg_ns, deg_ew, ppd_ew;
           
    south    = mparm.merc.south;  
    west     = mparm.merc.west; 
    deg_ew   = mparm.merc.deg_ew; 
    deg_ns   = mparm.merc.deg_ns;          
    

    ppd_ew   = (real_T)(mparm.center_ppd); 


    // Make sure poles are not included 
    //---------------------------------

    if ( (south < -89.0) || (south + deg_ns > 89.0) )           
    {
        M_out_emsg(L"Mercator projection N-S limits are bad:  S=%-20.10f  N=%-20.10f") % south % (south + deg_ns);
        return -1; 
    }

        
    // Compute size of map 
    //--------------------
    
    r = 180.0/M_PI * ppd_ew;      // mercator radius
    
    y_s = r * M_mercator_yr(south); 
    y_n = r * M_mercator_yr(south + deg_ns); 
                    
    cols = 1 + M_round(deg_ew * ppd_ew);  
    rows = 1 + M_round(y_n - y_s);
    
    M__(M_out(L"ppd_ew    = %-20.10f"  ) % ppd_ew  ;)
    M__(M_out(L"R         = %-20.10f"  ) % r       ;)
    M__(M_out(L"y_n       = %-20.10f"  ) % y_n     ;)
    M__(M_out(L"y_s       = %-20.10f"  ) % y_s     ;)
    M__(M_out(L"rows      = %d"        ) % rows    ;)
    M__(M_out(L"cols      = %d"        ) % cols    ;)
    

    // Create basic mapdata_t structure -- return imemdiately, if error
    // ----------------------------------------------------------------

    rc = create_mapdata(mapdata, rows, cols, mparm, parm);
    
    if (rc != 0) 
        return rc; 
                                              

    // Fill in common items directly from mparms
    // -----------------------------------------

    mapdata.north_limit  =  mparm.north_limit; 
    mapdata.south_limit  =  mparm.south_limit; 
    mapdata.east_limit   =  mparm.east_limit; 
    mapdata.west_limit   =  mparm.west_limit; 
                                              

    // Fill in Mercator projection-specific  items
    // -------------------------------------------
    
    mapdata.projection       = projection_E::mercator; 
    mapdata.u.merc.south     = south; 
    mapdata.u.merc.west      = west;
    mapdata.u.merc.north     = south + deg_ns;
    mapdata.u.merc.east      = west  + deg_ew;
    mapdata.u.merc.ppd_ew    = ppd_ew; 
    mapdata.u.merc.r         = r; 
    mapdata.u.merc.y_s       = y_s;
    mapdata.u.merc.y_n       = y_n;


    // fill in projection-dependent generic functions
    // ----------------------------------------------

    mapdata.get_lat_lon_p   = &get_lat_lon_merc;
    mapdata.get_row_col_p   = &get_row_col_merc;
    

    // fill in projection-dependent standard items
    // -------------------------------------------

    mapdata.ref_latitude   = south + 0.5 * deg_ns;  // average latitude (not at center)
    mapdata.ref_longitude  = west  + 0.5 * deg_ew;  // middle longitude
    mapdata.ref_ppd        = ppd_ew;                // scale at equator in PPD
    mapdata.ref_extent_ew  = deg_ew; 
    mapdata.ref_extent_ns  = deg_ns; 

    mapdata.min_latitude   = south; 
    mapdata.min_longitude  = west; 
    mapdata.max_latitude   = south + deg_ns; 
    mapdata.max_longitude  = west + deg_ew;

    if ( (mapdata.min_latitude < 0.0) && (mapdata.max_latitude > 0.0) ) // map crosses equator?
        mapdata.min_ppd = mapdata.ref_ppd; 
    else
        mapdata.min_ppd = mapdata.ref_ppd * M_min(M_secd(mapdata.min_latitude), M_secd(mapdata.max_latitude));

    mapdata.max_ppd = mapdata.ref_ppd * M_max(M_secd(mapdata.min_latitude), M_secd(mapdata.max_latitude)); 

    
    // debug display, if requested

    if (mparm.display >= 2)
    {
        M_out(L"Mercator projection parms: -------------------------------------");
        M_out(L"south                = %-.20f")  %  mapdata.u.merc.south          ;    
        M_out(L"west                 = %-.20f")  %  mapdata.u.merc.west           ;     
        M_out(L"north                = %-.20f")  %  mapdata.u.merc.north          ;    
        M_out(L"east                 = %-.20f")  %  mapdata.u.merc.east           ;     
        M_out(L"ppd_ew               = %-.20f")  %  mapdata.u.merc.ppd_ew         ;   
        M_out(L"r                    = %-.20f")  %  mapdata.u.merc.r              ;
        M_out(L"y_n                  = %-.20f")  %  mapdata.u.merc.y_n            ;
        M_out(L"y_s                  = %-.20f")  %  mapdata.u.merc.y_s            ;
        M_out(L"rows                 = %d"    )  %  mapdata.rows                  ;
        M_out(L"cols                 = %d"    )  %  mapdata.cols                  ;
        M_out(L"ref_latitude         = %-.20f")  %  mapdata.ref_latitude          ;  
        M_out(L"ref_longitude        = %-.20f")  %  mapdata.ref_longitude         ;  
        M_out(L"ref_ppd              = %-.20f")  %  mapdata.ref_ppd               ;  
        M_out(L"ref_extent_ew        = %-.20f")  %  mapdata.ref_extent_ew         ; 
        M_out(L"ref_extent_ns        = %-.20f")  %  mapdata.ref_extent_ns         ;
        M_out(L"north_limit          = %-.20f")  %  mapdata.north_limit           ;
        M_out(L"south_limit          = %-.20f")  %  mapdata.south_limit           ;
        M_out(L"east_limit           = %-.20f")  %  mapdata.east_limit            ;
        M_out(L"west_limit           = %-.20f")  %  mapdata.west_limit            ;
        M_out(L"min_latitude         = %-.20f")  %  mapdata.min_latitude          ; 
        M_out(L"max_latitude         = %-.20f")  %  mapdata.max_latitude          ;  
        M_out(L"min_longitude        = %-.20f")  %  mapdata.min_longitude         ; 
        M_out(L"max_longitude        = %-.20f")  %  mapdata.max_longitude         ; 
        M_out(L"min_ppd              = %-.20f")  %  mapdata.min_ppd               ; 
        M_out(L"max_ppd              = %-.20f")  %  mapdata.max_ppd               ;
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
//||       create_map_polar() 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



//   Local functions for Polar projection rows, cols, lat, lon 
//   ---------------------------------------------------------

static int get_lat_lon_polar(const mapdata_S& mapdata, llr_T& lat_r, llr_T& lon_r, rcf_T row, rcf_T col, bool error_check) try
{
    if ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
    {
        real_T radius = sqrt((row - mapdata.u.polar.row0) * (row - mapdata.u.polar.row0) + (col - mapdata.u.polar.col0) * (col - mapdata.u.polar.col0));  
        llr_T  lat    = 90.0 - mapdata.u.polar.lat_adj - radius / mapdata.u.polar.ppd;  

        if (lat >= -90.0)
        {
            real_T angle;
            llr_T lon;

            if (row == mapdata.u.polar.row0)    // can't do atan if row = row0 
            {
                if (col == mapdata.u.polar.col0)
                    angle = 0.0; 
                else if (col > mapdata.u.polar.col0)
                    angle = 90.0; 
                else
                    angle = -90.0;
            }
            else
            {
                if (row > mapdata.u.polar.row0)
                    angle = M_atand( (col - mapdata.u.polar.col0) / (row - mapdata.u.polar.row0) );
                else
                    angle = 180.0 + M_atand( (col- mapdata.u.polar.col0) / (row - mapdata.u.polar.row0) ); 
            }

            // angle is in range -90.0 to 270.0 (really should be -180.0 to 180.0)

            // compute output longitude in range -180.0 to +180.0 

            lon = angle + mapdata.u.polar.lon0; 

            if (lon > 180.0)            // past 180 degrees
                lon -= 360.0;      // convert to west longitude 
            else if (lon < -180.0)
                lon += 360.0;      // convert to east longitude


            // do lat/lon limits checking before returning lat/lon
       
            if  (            
                 (lat > mapdata.north_limit) ||
                 (lat < mapdata.south_limit) ||
                 (lon > mapdata.east_limit)  ||
                 (lon < mapdata.west_limit)   
                )
            {
                M__(M_out(L"get_lat_lon_polar() -- lat/lon=%-20.10f/%-20.10f   north/south=%-20.10f/%-20.10f  east/west=%-20.10f/%-20.10f") % lat % lon % mapdata.north_limit % mapdata.south_limit % mapdata.east_limit % mapdata.west_limit;)
                lat_r = -90.0;
                lon_r = -180.0;         
                return -1;           // return error -- results are out of limits
            }
            else
            {
                M__(M_out(L"row=%-20.10f  col=%-20.10f ---->  lat=%-20.10f   lon=%-20.10f") % row % col % lat % lon;)
                lat_r = lat; 
                lon_r = lon;
                return 0;             // return OK -- results should be valid   
            }
         }         
    }
    
    // error occured -- return -1, etc.
       
    lat_r = -90.0;
    lon_r = -180.0; 
    return -1;     
}
M_endf


//------------------------------------------------------------------------------------------------------------

static int get_row_col_polar(const mapdata_S& mapdata, rcf_T& row_r, rcf_T& col_r, llr_T lat, llr_T lon, bool error_check) try
{
    // do lat/lon limits checking before returning getting row/col

    if ( (lat > 90.0) || (lat < -90.0) )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_INVALID;           // return error -- input lat/lon is not valid
    }   

    if  (            
         (lat > mapdata.north_limit) ||
         (lat < mapdata.south_limit) ||
         (lon > mapdata.east_limit)  ||
         (lon < mapdata.west_limit)   
        )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_OUTLIMITS;           // return error -- input lat/lon is out-of limits
    }


    //  get row/col, since input lat/lon is within limits
    //  -------------------------------------------------

    real_T radius = (90.0 - (mapdata.u.polar.lat_adj + lat)) * mapdata.u.polar.ppd; 

    if (radius >= 0.0)
    {
        rcf_T row    = mapdata.u.polar.row0 + radius * M_cosd(lon - mapdata.u.polar.lon0); 
        rcf_T col    = mapdata.u.polar.col0 + radius * M_sind(lon - mapdata.u.polar.lon0); 

        row_r = row; 
        col_r = col;  

        if ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
        {
            return M_ROW_COL_RC_OK; 
        }
        else
        {
            return M_ROW_COL_RC_OUTMAP; 
        }
    }

    
    // radius is invalid -- must be off projection  -- cannot compute row/col
    // ----------------------------------------------------------------------
       
    row_r = 0.0; 
    col_r = 0.0; 
    return M_ROW_COL_RC_OUTPROJ;     
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int create_map_polar(mapdata_S& mapdata, const create_map_parm_S& mparm, const mapparm_S& parm) try                  
{
    int      rc = 0;
    real_T   ppd;
    llr_T    lon0;
    rcf_T    row0, col0, rowmax, colmax;
    llr_T    lat1, lat2, lat3, lat4;
    llr_T    lon1, lon2, lon3, lon4;
    llr_T    maxlat, minlat, maxlon, minlon, midlat;
    real_T   lat_adj, deg_row0;
    llr_T    ref_extent_ew, ref_extent_ns;
    llr_T    unwanted;
    rc_T     rows;
    rc_T     cols;

  
    
        
    // convert input parms to internal POLAR parms
    // -------------------------------------------
    
    lon0       = mparm.polar.lon0;
    ppd        = (real_T)(mparm.center_ppd); 
    lat_adj    = mparm.polar.lat_adj;
    deg_row0   = mparm.polar.deg_row0;


    // compute automatic lat adjust, if requested - only works for rightside up maps, where deg_row0 < 0.0
    // -----------------------------------------------------------------------------------------------------

    if (lat_adj < 0.0)
    {
        midlat = 90.0 + mparm.polar.deg_row0 - 0.5 * mparm.polar.deg_rows; // apparent middle latitude of area of interest
        
        if (midlat <= -90.0)
        {
            M_out_emsg(L"Cannot compute auto lat_adjust -- target mid latitude = %-20.10f (deg_rows=%-20.10f  deg_row0=%-20.10f") %  midlat % mparm.polar.deg_rows % mparm.polar.deg_row0;
            return -1; 
        }

        lat_adj = 90.0 - midlat - (180.0/M_PI) * M_sind(90.0 - midlat);

        if (lat_adj > 90.0 - mparm.polar.deg_row0)   // does lat_adj clip off top of area of interest?
        {
            M_out_emsg(L"Auto lat_adjust clips off top -- lat_adj=%-20.10f (deg_rows=%-20.10f  deg_row0=%-20.10f") % lat_adj % mparm.polar.deg_rows % mparm.polar.deg_row0;
            return -1; 
        }

        deg_row0 = deg_row0 + lat_adj;

        M_out(L"---> Automatic lat_adj = %-20.10f  -- midlat=%-20.10f  adjusted polar_deg_row0 = %-20.10f") % lat_adj % midlat % deg_row0; 

    }


    rows           = M_round(ppd * mparm.polar.deg_rows) + 1; 
    cols           = M_round(ppd * mparm.polar.deg_cols) + 1;
    ref_extent_ew  = mparm.polar.deg_rows;        
    ref_extent_ns  = mparm.polar.deg_cols;            
    row0           = (rcf_T)M_round(ppd * deg_row0);
    col0           = (rcf_T)M_round(ppd * mparm.polar.deg_col0);
    rowmax         = (rcf_T)(rows - 1);  
    colmax         = (rcf_T)(cols - 1); 
    
    M__(M_out(L"lon0      = %-20.10f") % lon0   ;)
    M__(M_out(L"ppd       = %-20.10f") % ppd    ;)
    M__(M_out(L"lat_adj   = %-20.10f") % lat_adj;)
    M__(M_out(L"row0      = %-20.10f") % row0   ;)
    M__(M_out(L"col0      = %-20.10f") % col0   ;)
    M__(M_out(L"rowmax    = %-20.10f") % rowmax ;)
    M__(M_out(L"colmax    = %-20.10f") % colmax ;)
    M__(M_out(L"rows      = %d"      ) % rows   ;)
    M__(M_out(L"cols      = %d"      ) % cols   ;)
    
    
    // Create basic mapdata_t structure -- return imemdiately, if error
    // ----------------------------------------------------------------

    rc = create_mapdata(mapdata, rows, cols, mparm, parm);
    
    if (rc != 0) 
        return rc; 
                                              
    
    // Fill in common items directly from mparms
    // -----------------------------------------

    mapdata.north_limit  =  mparm.north_limit; 
    mapdata.south_limit  =  mparm.south_limit; 
    mapdata.east_limit   =  mparm.east_limit; 
    mapdata.west_limit   =  mparm.west_limit; 
                                              

    // Fill in POLAR projection-specific items (for use by generic function calls below)
    // ---------------------------------------------------------------------------------
    
    mapdata.projection       = projection_E::polar; 
    mapdata.u.polar.row0     = row0;
    mapdata.u.polar.col0     = col0;
    mapdata.u.polar.lon0     = lon0;
    mapdata.u.polar.ppd      = ppd;
    mapdata.u.polar.lat_adj  = lat_adj; 


    // fill in projection-dependent generic functions now, so get_min_max_ll() can call them as needed
    // -----------------------------------------------------------------------------------------------
     
    mapdata.get_lat_lon_p   = &get_lat_lon_polar;
    mapdata.get_row_col_p   = &get_row_col_polar;

    
    // Compute latitude and longitude limits    
    // -------------------------------------

    if  ( (row0 > 0.0) && (col0 > 0.0) && (row0 < rowmax) && (col0 < colmax) )
    {
        // row0, col0 inside of map area -- min latitude should be at some corner, max is at row0, col0 = 90.0 -- longitude is from -180 to +180 around row0, col0

        maxlat  =  90.0;
        minlon  = -180.0; 
        maxlon  =  180.0; 

        (void)get_lat_lon_polar(mapdata, lat1, unwanted, 0.0     , 0.0    ); 
        (void)get_lat_lon_polar(mapdata, lat2, unwanted, 0.0     , colmax );
        (void)get_lat_lon_polar(mapdata, lat3, unwanted, rowmax  , 0.0    );
        (void)get_lat_lon_polar(mapdata, lat4, unwanted, rowmax  , colmax );
        
        minlat = M_max(-90.0, M_min(M_min(lat1, lat2), M_min(lat3, lat4)));                
    }
    else // pole is not inside the map bounds -- search edges, etc.  to find min/max latitudes
    {
        (void)get_min_max_ll(mapdata, minlat, maxlat, minlon, maxlon, mparm, parm); 
    }

    
    
    // fill in projection-dependent standard items for external use
    // ------------------------------------------------------------

    mapdata.ref_latitude   = 0.5 * (minlat + maxlat);  // average latitude (not at center)
    mapdata.ref_longitude  = 0.5 * (minlon + maxlon);  // middle longitude (not at center)
    mapdata.ref_ppd        = ppd;                      // scale along meridians
    mapdata.ref_extent_ew  = ref_extent_ew;            // nominal extent 
    mapdata.ref_extent_ns  = ref_extent_ns;            // nominal extent 

    mapdata.min_latitude   = minlat; 
    mapdata.min_longitude  = minlon; 
    mapdata.max_latitude   = maxlat; 
    mapdata.max_longitude  = maxlon;

    mapdata.min_ppd        = ppd;                       // smallest scale is along meridians (or at pole) 

    if (minlat == -90.0)                           // map reaches south pole? 
        mapdata.max_ppd = ppd * 20.0;                   // use some large value
    else
        mapdata.max_ppd = ppd * M_min(20.0, M_rad(90.0 - minlat)/M_sind(90.0 - minlat));  // limit max ppd to 20 x standard PPD, in case map goes way south.  



    // debug display, if requested     

    if (mparm.display >= 2)
    {
        M_out(L"Polar projection parms: ----------------------------------------");
        M_out(L"row0                 = %-.20f")  %  mapdata.u.polar.row0          ;    
        M_out(L"col0                 = %-.20f")  %  mapdata.u.polar.col0          ;     
        M_out(L"lon0                 = %-.20f")  %  mapdata.u.polar.lon0          ;     
        M_out(L"ppd                  = %-.20f")  %  mapdata.u.polar.ppd           ;
        M_out(L"lat_adj              = %-.20f")  %  mapdata.u.polar.lat_adj       ; 
        M_out(L"rows                 = %d"    )  %  mapdata.rows                  ;
        M_out(L"cols                 = %d"    )  %  mapdata.cols                  ;
        M_out(L"ref_latitude         = %-.20f")  %  mapdata.ref_latitude          ;  
        M_out(L"ref_longitude        = %-.20f")  %  mapdata.ref_longitude         ;  
        M_out(L"ref_ppd              = %-.20f")  %  mapdata.ref_ppd               ;  
        M_out(L"ref_extent_ew        = %-.20f")  %  mapdata.ref_extent_ew         ; 
        M_out(L"ref_extent_ns        = %-.20f")  %  mapdata.ref_extent_ns         ;
        M_out(L"north_limit          = %-.20f")  %  mapdata.north_limit           ;
        M_out(L"south_limit          = %-.20f")  %  mapdata.south_limit           ;
        M_out(L"east_limit           = %-.20f")  %  mapdata.east_limit            ;
        M_out(L"west_limit           = %-.20f")  %  mapdata.west_limit            ;
        M_out(L"min_latitude         = %-.20f")  %  mapdata.min_latitude          ; 
        M_out(L"max_latitude         = %-.20f")  %  mapdata.max_latitude          ;  
        M_out(L"min_longitude        = %-.20f")  %  mapdata.min_longitude         ; 
        M_out(L"max_longitude        = %-.20f")  %  mapdata.max_longitude         ;
        M_out(L"min_ppd              = %-.20f")  %  mapdata.min_ppd               ; 
        M_out(L"max_ppd              = %-.20f")  %  mapdata.max_ppd               ;
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
//||       create_map_stereo() -- stereographic projection
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



//   Local functions for Stereographic projection rows, cols, lat, lon 
//   -----------------------------------------------------------------

static int get_lat_lon_stereo(const mapdata_S& mapdata, llr_T& lat_r, llr_T& lon_r, rcf_T row, rcf_T col, bool error_check) try
{
    if ( 
         (error_check == false)
         ||
         ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
       )
    {
        llr_T lat, lon;                               // output values 
        llr_T lat0       = mapdata.u.stereo.lat0;
        llr_T lon0       = mapdata.u.stereo.lon0;
        real_T sin_lat0  = mapdata.u.stereo.sin_lat0;
        real_T cos_lat0  = mapdata.u.stereo.cos_lat0;
        real_T x         = col - mapdata.u.stereo.col0;      // x in pixels  x = easting 
        real_T y         = mapdata.u.stereo.row0 - row;      // y in pixels  y = northing   
        
        real_T radius = sqrt(x * x + y * y); 
        real_T c      =  2.0 * atan(radius / (2.0 * mapdata.u.stereo.r)); 
        real_T sin_c  = sin(c);
        real_T cos_c  = cos(c); 

        M__(M_out(L"get_lat_lon_stereo() -- row=%-20.10f col=%-20.10f x=%-20.10f y=%-20.10f radius=%-20.10f c=%-20.10f=%-20.10f") % row % col % x % y % radius % c % M_deg(c);)

        if (radius == 0.0)
        {
            lat = lat0;
            lon = lon0;
        }
        else
        {
            lat = M_asind(cos_c * sin_lat0 + (y / radius) *  sin_c * cos_lat0);

            if (lat0 == 90.0)
            {
                lon = lon0 + M_atan2d(x, -y);   // hopefully atan2 handles y=0 case               
            }
            else if (lat0 == -90.0)
            {
                lon = lon0 + M_atan2d(x, y); 
            }
            else
            {
                lon = lon0 + M_atan2d(x * sin_c, radius * cos_lat0 * cos_c - y * sin_lat0 * sin_c);    

            }
        }
        

        // do lat/lon limits checking before returning lat/lon

        if  (            
             (lat > mapdata.north_limit) ||
             (lat < mapdata.south_limit) ||
             (lon > mapdata.east_limit)  ||
             (lon < mapdata.west_limit)   
            )
        {
            M__(M_out(L"get_lat_lon_stereo() -- lat/lon=%-20.10f/%-20.10f   north/south=%-20.10f/%-20.10f  east/west=%-20.10f/%-20.10f") % lat % lon % mapdata.north_limit % mapdata.south_limit % mapdata.east_limit % mapdata.west_limit;)
            lat_r = -90.0;
            lon_r = -180.0;         
            return -1;           // return error -- results are out of limits
        }
        else
        {
            M__(M_out(L"get_lat_lon_stereo() -- row=%-20.10f  col=%-20.10f ---->  lat=%-20.10f   lon=%-20.10f") % row % col % lat % lon;)
            lat_r = lat; 
            lon_r = lon;
            return 0;             // return OK -- results should be valid   
        }
    }
    
    // error occured -- return -1, etc.
      
    M__(M_out(L"get_lat_lon_stereo() -- row=%-20.10f  col=%-20.10f ??error??") % row % col;)
    lat_r = -90.0;
    lon_r = -180.0; 
    return -1;     
}
M_endf


//-------------------------------------------------------------------------------------------------------------

static int get_row_col_stereo(const mapdata_S& mapdata, rcf_T& row_r, rcf_T& col_r, llr_T lat, llr_T lon, bool error_check) try
{
    // do lat/lon limits checking before returning getting row/col

    if ( (lat < -90.0) && (lat > 90.0) )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_INVALID;            // return error -- input lat/lon is not valid
    }
  
    if  (            
         (lat > mapdata.north_limit) ||
         (lat < mapdata.south_limit) ||
         (lon > mapdata.east_limit)  ||
         (lon < mapdata.west_limit)   
        )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_OUTLIMITS;           // return error -- input lat/lon is out-of bounds
    }


    //  get row/col, since input lat/lon is within limits  
    //  -------------------------------------------------
    //
    //  note:  for stereographic, only point that is out of projection is antipode of lat0/lon0

    llr_T  lon0 = mapdata.u.stereo.lon0;
    llr_T  lat0 = mapdata.u.stereo.lat0;
    llr_T  dlon;
    rcf_T  row, col;
    real_T k; 
    real_T xr, yr;                // x/R y/R in formulae 
    
    dlon = lon - lon0; 

    // specialize the north polar case:

    if (lat0 == 90.0)
    {
        if (lat == -90.0)       // tan will be infinite -- can't get xr yr
        {
            row_r = 0.0;
            col_r = 0.0;
            return M_ROW_COL_RC_OUTPROJ;
        }
        
        real_T tan_lat = tan(M_PI_4 - M_rad(0.5 * lat));

        xr =  2.0 * tan_lat * M_sind(dlon); 
        yr = -2.0 * tan_lat * M_cosd(dlon);
    }

    // specialize the south polar case

    else if (lat0 == -90.0)
    {
        if (lat == 90.0)       // tan will be infinite -- can't get xr yr
        {
            row_r = 0.0;
            col_r = 0.0;
            return M_ROW_COL_RC_OUTPROJ;
        }
   
        real_T tan_lat = tan(M_PI_4 + M_rad(0.5 * lat));

        xr =  2.0 * tan_lat * M_sind(dlon); 
        yr = -2.0 * tan_lat * M_cosd(dlon);
    }

    // specialize the equatorial case

    else if (lat0 == 0.0)
    {
        real_T cos_lat  =  M_cosd(lat);
   
        real_T denom = 1.0 + cos_lat * M_cosd(dlon);

        if (denom == 0.0)      // must be at antipode? -- 180.0 degrees away from lon0 -- only if cos_lat = 1.0, and M_cosd(dlon) = -1.0
        {
            row_r = 0.0;
            col_r = 0.0;
            return M_ROW_COL_RC_OUTPROJ;
        } 

        k  = 2.0 / denom; 
        xr = k * cos_lat * M_sind(dlon); 
        yr = k * M_sind(lat);             
    }
    else // general case -- neither polar nor equatorial
    {
       real_T cos_dlon = M_cosd(dlon);
       real_T sin_dlon = M_sind(dlon);
       real_T cos_lat  = M_cosd(lat); 
       real_T sin_lat  = M_sind(lat);
       real_T sin_lat0 = mapdata.u.stereo.sin_lat0;
       real_T cos_lat0 = mapdata.u.stereo.cos_lat0;

       real_T denom    = 1.0 + sin_lat0 * sin_lat + cos_lat0 * cos_lat * cos_dlon;

       if (denom == 0.0)      // must be at antipode? -- 180.0+-360.0 degrees away from lat0/lon0 
       {
           row_r = 0.0;
           col_r = 0.0;
           return M_ROW_COL_RC_OUTPROJ;
       } 

       k  = 2.0 / denom;
       xr = k * cos_lat * sin_dlon; 
       yr = k * (cos_lat0 * sin_lat - sin_lat0 * cos_lat * cos_dlon);
       
       M__(M_out(L"get_row_col_stereo() -- general case: k=%-20.10f yr=%-20.10f") % k % yr;)
    }
    
    // compute actual row/col based on x/R and y/R, and reject if out of bounds for this map

    col = mapdata.u.stereo.col0 + mapdata.u.stereo.r * xr;  // x is easting -- towards right -- higher columns 
    row = mapdata.u.stereo.row0 - mapdata.u.stereo.r * yr;  // y is northing -- towards top -- lower columns

    M__(M_out(L"get_row_col_stereo() -- lat=%-20.10f  lon=%-20.10f ::: row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)

    if ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
    {
        M__(M_out(L"get_row_col_stereo() -- lat=%-20.10f  lon=%-20.10f --->  row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)
        row_r = row; 
        col_r = col;
        return M_ROW_COL_RC_OK; 
    } 
    else
    {
        M__(M_out(L"get_row_col_stereo() -- lat=%-20.10f  lon=%-20.10f --->  row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)
        row_r = row; 
        col_r = col;                     // off map, but valid input latitude  -- stereographic map stretches out to infinity in all directions
        return M_ROW_COL_RC_OUTMAP;      // indicate off map, and out-of-bounds row, col has been returned 
    } 
  
    
    // unexpected error occured -- control chould not get here
      
    row_r = 0.0; 
    col_r = 0.0;    
    return M_ROW_COL_RC_ERROR;     // indicate unexpected error
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int create_map_stereo(mapdata_S& mapdata, const create_map_parm_S& mparm, const mapparm_S& parm) try                  
{
    int      rc = 0;
    real_T   r, ppd;
    llr_T    lon0, lat0;
    rcf_T    row0, col0; // rowmax, colmax;
    llr_T    lat1, lat2, lat3, lat4;
    llr_T    lon1, lon2, lon3, lon4;
  //  llr_T    maxlat, minlat, maxlon, minlon;
    real_T   sin_lat0, cos_lat0;
    llr_T    ref_extent_ew, ref_extent_ns;
    rc_T     rows, cols;
 //   llr_T    unwanted;

   
        
    
    // convert input parms to internal STEREO parms
    // --------------------------------------------
    
    lat0       = mparm.stereo.lat0;
    lon0       = mparm.stereo.lon0;
    ppd        = (real_T)(mparm.center_ppd);    
    r          = (180.0/M_PI) * ppd; 

    if (mparm.stereo.rows > 0)
    {
        rows = mparm.stereo.rows;                // row_max can be even or odd
        ref_extent_ns = 4.0 * M_atand(((real_T)rows - 1.0) / (4.0 * r));
    }
    else
    {
       ref_extent_ns = mparm.stereo.deg_rows;  
       rows          = M_round(4.0 * r * M_tand(0.25 * mparm.stereo.deg_rows)) + 1; 

       if ( ((rows - 1) % 2) != 0 ) rows++;    // make sure rowmax will be even
    }


    if (mparm.stereo.cols > 0)
    {
        cols = mparm.stereo.cols;                // col_max can be even or odd
        ref_extent_ew = 4.0 * M_atand(((real_T)cols - 1.0) / (4.0 * r));
    }
    else
    {
        ref_extent_ew = mparm.stereo.deg_cols;  
        cols          = M_round(4.0 * r * M_tand(0.25 * mparm.stereo.deg_cols)) + 1; 

        if ( ((cols - 1) % 2) != 0 ) cols++;    // make sure colmax will be even
    }
  
    row0    = (real_T)((rows - 1)/2);           // can be xxx.5 if input rows is even (rows should be odd here, if computed)
    col0    = (real_T)((cols - 1)/2);           // can be xxx.5 if input cols is even (rows should be odd here, if computed)

  //  rowmax  = (real_T)(rows - 1);  
  //  colmax  = (real_T)(cols - 1); 
  
    sin_lat0 = M_sind(lat0);
    cos_lat0 = M_cosd(lat0);
    
    M__(M_out(L"create_map_stereo() -- lat0      = %-20.10f"  ) % lat0    ;)
    M__(M_out(L"create_map_stereo() -- lon0      = %-20.10f"  ) % lon0    ;)
    M__(M_out(L"create_map_stereo() -- row0      = %-20.10f"  ) % row0    ;)
    M__(M_out(L"create_map_stereo() -- col0      = %-20.10f"  ) % col0    ;)
    M__(M_out(L"create_map_stereo() -- ppd       = %-20.10f"  ) % ppd     ;)
    M__(M_out(L"create_map_stereo() -- r         = %-20.10f"  ) % r       ;)
    M__(M_out(L"create_map_stereo() -- sin_lat0  = %-20.10f"  ) % sin_lat0;)
    M__(M_out(L"create_map_stereo() -- cos_lat0  = %-20.10f"  ) % cos_lat0;)
    M__(M_out(L"create_map_stereo() -- col0      = %-20.10f"  ) % col0    ;)
  //  M__(M_out(L"create_map_stereo() -- rowmax    = %-20.10f"  ) % rowmax  ;)
  //  M__(M_out(L"create_map_stereo() -- colmax    = %-20.10f"  ) % colmax  ;)
    M__(M_out(L"create_map_stereo() -- rows      = %d"        ) % rows    ;)
    M__(M_out(L"create_map_stereo() -- cols      = %d"        ) % cols    ;)
     
    
    // Create basic mapdata_T structure -- return imemdiately, if error
    // ----------------------------------------------------------------
    
    rc = create_mapdata(mapdata, rows, cols, mparm, parm);
    
    if (rc != 0) 
        return rc; 
    
    
    // Fill in common items directly from mparms
    // -----------------------------------------

    mapdata.north_limit  =  mparm.north_limit; 
    mapdata.south_limit  =  mparm.south_limit; 
    mapdata.east_limit   =  mparm.east_limit; 
    mapdata.west_limit   =  mparm.west_limit; 
                                              

    // Fill in STEREO projection-specific items (for use by generic function calls below)
    // ---------------------------------------------------------------------------------
    
    mapdata.projection        = projection_E::stereographic; 
    mapdata.u.stereo.lat0     = lat0;
    mapdata.u.stereo.lon0     = lon0;
    mapdata.u.stereo.row0     = row0;
    mapdata.u.stereo.col0     = col0;
    mapdata.u.stereo.r        = r;
    mapdata.u.stereo.sin_lat0 = sin_lat0; 
    mapdata.u.stereo.cos_lat0 = cos_lat0;  
  

    // fill in projection-dependent generic functions now, so get_min_max_ll() can call them as needed
    // ------------------------------------------------------------------------------------------------
      
    mapdata.get_lat_lon_p   = &get_lat_lon_stereo;
    mapdata.get_row_col_p   = &get_row_col_stereo;


    // Compute latitude and longitude limits   (invalid, due to north_limit, etc.)
    // -------------------------------------

#if 0 
    if  ( ((90.0 - lat0) < (0.5 * mparm.stereo.deg_rows)) &&  ((90.0 + lat0) < (0.5 * mparm.stereo.deg_rows)) )
    {
        // north and south poles are both inside map -- need to do all latitudes and longitudes 
        
        minlat  = -90.0; 
        maxlat  =  90.0;
        minlon  = -180.0; 
        maxlon  =  180.0; 
    }
    else if ( (90.0 - lat0) < (0.5 * mparm.stereo.deg_rows) )  
    {
        // only north pole is inside map -- max lon  = 90.0 -- longitude is from -180 to +180 around north pole
        
        maxlat  =  90.0;
        minlon  = -180.0; 
        maxlon  =  180.0; 
        
        if (lat0 == 90.0)     // centered on north pole?
        {
            // min lat should be at some corner (or all corners)

            (void)get_lat_lon_stereo(mapdata, lat1, unwanted, 0.0     , 0.0    ); 
            (void)get_lat_lon_stereo(mapdata, lat2, unwanted, 0.0     , colmax );
            (void)get_lat_lon_stereo(mapdata, lat3, unwanted, rowmax  , 0.0    );
            (void)get_lat_lon_stereo(mapdata, lat4, unwanted, rowmax  , colmax );
        
            minlat = std::max(-90.0, std::min(std::min(lat1, lat2), std::min(lat3, lat4)));  
        }
        else   // not centered at pole -- need to check edge for min lat (only)
        {
            (void)get_min_max_ll(mapdata, minlat, unwanted, unwanted, unwanted, mparm, parm); 
        }
    }
    else if ( (90.0 + lat0) < (0.5 * mparm.stereo.deg_rows) )  
    {
        // only south pole is inside map --  min lat = -90.0 -- longitude is from -180 to +180 around south pole

        minlat  = -90.0;
        minlon  = -180.0; 
        maxlon  =  180.0; 

        if (lat0 == -90.0)       // centered on south pole?
        {
            // max lat should be at some corner (or all corners)

           (void)get_lat_lon_stereo(mapdata, lat1, unwanted, 0.0     , 0.0    ); 
           (void)get_lat_lon_stereo(mapdata, lat2, unwanted, 0.0     , colmax );
           (void)get_lat_lon_stereo(mapdata, lat3, unwanted, rowmax  , 0.0    );
           (void)get_lat_lon_stereo(mapdata, lat4, unwanted, rowmax  , colmax );
        
           maxlat = std::min(90.0, std::max(std::max(lat1, lat2), std::max(lat3, lat4)));    
        }
        else   // not centered at pole -- need to check edges, etc. for max lat (only)
        {
            (void)get_min_max_ll(mapdata, unwanted, maxlat, unwanted, unwanted, mparm, parm); 
        }
    }
    else // pole is not inside the map bounds -- search edges, etc to find min/max latitudes
    {
        (void)get_min_max_ll(mapdata, minlat, maxlat, minlon, maxlon, mparm, parm); 
    }
#endif
    
    // get min/max lat/lon via sampling, edge traversal, etc.
    // ------------------------------------------------------

    llr_T  maxlat, minlat, maxlon, minlon, midlat;

    (void)get_min_max_ll(mapdata, minlat, maxlat, minlon, maxlon, mparm, parm); 

    
    // get min/max ppd via sampling, etc.
    // -----------------------------------

     real_T   minppd, maxppd;
     (void)get_min_max_ppd(mapdata, minppd, maxppd, mparm, parm);

         
    // fill in projection-dependent standard items for external use
    // ------------------------------------------------------------

    mapdata.ref_latitude   = lat0;                      // average latitude (not at center)
    mapdata.ref_longitude  = lon0;                      // middle longitude (not at center)
    mapdata.ref_ppd        = ppd;                       // scale along meridians
    mapdata.ref_extent_ns  = ref_extent_ns;             // nominal extent 
    mapdata.ref_extent_ew  = ref_extent_ew;             // nominal extent 

    mapdata.min_latitude   = minlat; 
    mapdata.min_longitude  = minlon; 
    mapdata.max_latitude   = maxlat; 
    mapdata.max_longitude  = maxlon;

    mapdata.min_ppd        = minppd;                    
    mapdata.max_ppd        = maxppd;                       


#if 0
    // max scale factor is at some corner: factor = (sec(c/2))^2 -- c = angle from center point (in radians)
    // angle is 2 * arctan(pixel_distance/ 2r)  
    
    real_T dist_11 = sqrt(          row0  *           row0  +            col0  *           col0 );
    real_T dist_12 = sqrt((rowmax - row0) * (rowmax - row0) +            col0  *           col0 );
    real_T dist_21 = sqrt(          row0  *           row0  +  (colmax - col0) * (colmax - col0));
    real_T dist_22 = sqrt((rowmax - row0) * (rowmax - row0) +  (colmax - col0) * (colmax - col0));

    real_T dist_max  = std::max(std::max(dist_11, dist_12), std::max(dist_21, dist_21));
    real_T angle_max = 2.0 * M_atand(dist_max / (2.0 * r)); 

    if (mparm.display >= 2)
       M_out(L"dist_max = %.6f  (%.6f %.6f %.6f %.6f) r = %.6f ===> angle_max=%.6f") % dist_max % dist_11 % dist_12 % dist_21 % dist_22 % r % angle_max;
            
    mapdata.max_ppd = ppd * M_secd(angle_max/2.0) * M_secd(angle_max/2.0);      
#endif    

    // debug display, if requested     
    
    if (mparm.display >= 2)
    {   
        M_out(L"Stereographic projection parms: ---------------------------------");
        M_out(L"lat0                 = %-.20f" )  %  mapdata.u.stereo.lat0         ;    
        M_out(L"lon0                 = %-.20f" )  %  mapdata.u.stereo.lon0         ;  
        M_out(L"row0                 = %-.20f" )  %  mapdata.u.stereo.row0         ;    
        M_out(L"col0                 = %-.20f" )  %  mapdata.u.stereo.col0         ;     
        M_out(L"r                    = %-.20f" )  %  mapdata.u.stereo.r            ;
        M_out(L"sin_lat0             = %-.20f" )  %  mapdata.u.stereo.sin_lat0     ;    
        M_out(L"cos_lat0             = %-.20f" )  %  mapdata.u.stereo.cos_lat0     ;  
        M_out(L"rows                 = %d"     )  %  mapdata.rows                  ;
        M_out(L"cols                 = %d"     )  %  mapdata.cols                  ;
        M_out(L"ref_latitude         = %-.20f" )  %  mapdata.ref_latitude          ;  
        M_out(L"ref_longitude        = %-.20f" )  %  mapdata.ref_longitude         ;  
        M_out(L"ref_ppd              = %-.20f" )  %  mapdata.ref_ppd               ;  
        M_out(L"ref_extent_ew        = %-.20f" )  %  mapdata.ref_extent_ew         ; 
        M_out(L"ref_extent_ns        = %-.20f" )  %  mapdata.ref_extent_ns         ;
        M_out(L"north_limit          = %-.20f" )  %  mapdata.north_limit           ;
        M_out(L"south_limit          = %-.20f" )  %  mapdata.south_limit           ;
        M_out(L"east_limit           = %-.20f" )  %  mapdata.east_limit            ;
        M_out(L"west_limit           = %-.20f" )  %  mapdata.west_limit            ;
        M_out(L"min_latitude         = %-.20f" )  %  mapdata.min_latitude          ; 
        M_out(L"max_latitude         = %-.20f" )  %  mapdata.max_latitude          ;  
        M_out(L"min_longitude        = %-.20f" )  %  mapdata.min_longitude         ; 
        M_out(L"max_longitude        = %-.20f" )  %  mapdata.max_longitude         ;
        M_out(L"min_ppd              = %-.20f" )  %  mapdata.min_ppd               ; 
        M_out(L"max_ppd              = %-.20f" )  %  mapdata.max_ppd               ;
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
//||       create_map_ortho() -- orthographic projection
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//   Local functions for Orthographic projection rows, cols, lat, lon 
//   ----------------------------------------------------------------

static int get_lat_lon_ortho(const mapdata_S& mapdata, llr_T& lat_r, llr_T& lon_r, rcf_T row, rcf_T col, bool error_check) try
{
    if ( 
         (error_check == false)
         ||
         ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
       )
    {
        llr_T lat, lon;                                        // output values 
        llr_T lat0       { mapdata.u.ortho.lat0       } ;
        llr_T lon0       { mapdata.u.ortho.lon0       } ;
        real_T sin_lat0  { mapdata.u.ortho.sin_lat0   } ;
        real_T cos_lat0  { mapdata.u.ortho.cos_lat0   } ;
        real_T x         { col - mapdata.u.ortho.col0 } ;      // x in pixels  x = easting 
        real_T y         { mapdata.u.ortho.row0 - row } ;      // y in pixels  y = northing   
        
        real_T radius = sqrt(x * x + y * y); 


        // see if x, y is within the plotted area -- error, if not

        if (radius > mapdata.u.ortho.r)
        {
            lat_r = -90.0;
            lon_r = -180.0;         
            return -1;
        }

        real_T c     { asin(radius / mapdata.u.ortho.r) } ; 
        real_T sin_c { sin(c)                           } ; 
        real_T cos_c { cos(c)                           } ; 
 
        M__(M_out(L"get_lat_lon_ortho() -- row=%-20.10f   col=%-20.10f   x=%-20.10f   y=%-20.10f   radius=%-20.10f   c=%-20.10f   c(deg)=%-20.10f") % row % col % x % y % radius % c % M_deg(c);)

        if (radius == 0.0)
        {
            lat = lat0;
            lon = lon0;
        }
        else
        {
            lat = M_asind(cos_c * sin_lat0 + (y / radius) *  sin_c * cos_lat0);

            if (lat0 == 90.0)                   // north polar case
            {
                lon = lon0 + M_atan2d(x, -y);   // hopefully atan2 handles y=0 case               
            }
            else if (lat0 == -90.0)             // south polar case
            {
                lon = lon0 + M_atan2d(x, y); 
            }
            else                                // general case
            {
                lon = lon0 + M_atan2d(x * sin_c, radius * cos_lat0 * cos_c - y * sin_lat0 * sin_c);
            }
        }
        
        M__(M_out(L"get_lat_lon_ortho() -- row=%-20.10f  col=%-20.10f ---->  lat=%-20.10f   lon=%-20.10f") % row % col % lat % lon;)


        // do lat/lon limits checking before returning lat/lon

        if  (            
             (lat > mapdata.north_limit) ||
             (lat < mapdata.south_limit) ||
             (lon > mapdata.east_limit)  ||
             (lon < mapdata.west_limit)   
            )
        {
            M__(M_out(L"get_lat_lon_ortho() -- lat/lon=%-20.10f/%-20.10f   north/south=%-20.10f/%-20.10f  east/west=%-20.10f/%-20.10f") % lat % lon % mapdata.north_limit % mapdata.south_limit % mapdata.east_limit % mapdata.west_limit;)
            lat_r = -90.0;
            lon_r = -180.0;         
            return -1;           // return error -- results are out of limits
        }
        else
        {
            lat_r = lat; 
            lon_r = lon;
            return 0;             // return OK -- results should be valid   
        }
    }
    

    // error occured -- return -1, etc.
      
    M__(M_out(L"get_lat_lon_ortho() -- row=%-20.10f  col=%-20.10f ??error??") % row % col;)
    lat_r = -90.0;
    lon_r = -180.0; 
    return -1;     
}
M_endf


//-------------------------------------------------------------------------------------------------------------

static int get_row_col_ortho(const mapdata_S& mapdata, rcf_T& row_r, rcf_T& col_r, llr_T lat, llr_T lon, bool error_check) try
{
    // note: error_check is ignored here



    // do lat/lon limits checking before getting row/col
    // -------------------------------------------------

    if ( (lat < -90.0) || (lat > 90.0) )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_INVALID;             // return error -- input lat/lon is not valid
    }  

    if  (            
         (lat > mapdata.north_limit) ||
         (lat < mapdata.south_limit) ||
         (lon > mapdata.east_limit)  ||
         (lon < mapdata.west_limit)   
        )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_OUTLIMITS;           // return error -- input lat/lon is out-of specified limits
    }
    

    // within limits -- get row, col 
    // -----------------------------

    llr_T  lon0 { mapdata.u.ortho.lon0 } ;
    llr_T  lat0 { mapdata.u.ortho.lat0 } ;
    llr_T  dlon { lon - lon0           } ;
    rcf_T  row, col;
    real_T xr, yr;                                 // x/R y/R in formulae   

    real_T sin_lat0 { mapdata.u.ortho.sin_lat0 } ; 
    real_T cos_lat0 { mapdata.u.ortho.cos_lat0 } ;
    real_T sin_dlon { M_sind(dlon)             } ;
    real_T cos_dlon { M_cosd(dlon)             } ; 
    real_T sin_lat  { M_sind(lat)              } ; 
    real_T cos_lat  { M_cosd(lat)              } ; 


    // find out if requested lat/lon is plottable/visible on this map  -- limited to -90.0 to 90.0 degrees from center point (modulo 360.0) 
    // ------------------------------------------------------------------------------------------------------------------------------------

    if ((sin_lat0 * sin_lat + cos_lat0 * cos_lat * cos_dlon) < 0.0)
    {
        row_r = 0.0; 
        col_r = 0.0; 
        return M_ROW_COL_RC_OUTPROJ;     // indicate out of area covered by projection  
    }
    

    // specialize the north polar case:

    if (lat0 == 90.0)
    {
        xr =  cos_lat * sin_dlon; 
        yr = -cos_lat * cos_dlon;
    } 

    // specialize the south polar case

    else if (lat0 == -90.0)
    {
        xr =  cos_lat * sin_dlon; 
        yr =  cos_lat * cos_dlon;
    } 

    // specialize the equatorial case

    else if (lat0 == 0.0)
    {
        xr =  cos_lat * sin_dlon; 
        yr =  sin_lat;            
    }
    else // general case -- neither polar nor equatorial
    {
       xr = cos_lat * sin_dlon; 
       yr = cos_lat0 * sin_lat - sin_lat0 * cos_lat * cos_dlon;
       
       M__(M_out(L"get_row_col_ortho() -- general case: xr=%-20.10f  yr=%-20.10f") % xr % yr;)
    }
    

    // compute actual row/col based on x/R and y/R, and reject if out of bounds

    col = mapdata.u.ortho.col0 + mapdata.u.ortho.r * xr;  // x is easting -- towards right -- higher columns 
    row = mapdata.u.ortho.row0 - mapdata.u.ortho.r * yr;  // y is northing -- towards top -- lower columns

    M__(M_out(L"get_row_col_ortho() -- lat=%-20.10f  lon=%-20.10f ::: row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)

    if ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
    {
        M__(M_out(L"get_row_col_ortho() -- lat=%-20.10f  lon=%-20.10f --->  row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)
        row_r = row; 
        col_r = col;
        return M_ROW_COL_RC_OK;        // row/col inside mapped area
    } 
    else
    {
        M__(M_out(L"get_row_col_ortho() -- lat=%-20.10f  lon=%-20.10f --->  row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)
        row_r = row; 
        col_r = col;                    // off map, but valid input latitude  -- orthographic map may stretch out beyond map boundaries
        return M_ROW_COL_RC_OUTMAP;     // indicate off map, and out-of-bounds row, col has been returned 
    } 

    
    // error occured -- should not get here
      
    row_r = 0.0; 
    col_r = 0.0;    
    return M_ROW_COL_RC_ERROR;              // indicate error
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int create_map_ortho(mapdata_S& mapdata, const create_map_parm_S& mparm, const mapparm_S& parm) try                  
{
    int      rc { 0 };
        
    
    // convert input parms to internal ORTHO parms
    // -------------------------------------------
    
    llr_T  lat0       { mparm.ortho.lat0           } ;
    llr_T  lon0       { mparm.ortho.lon0           } ;
    real_T ppd        { (real_T)(mparm.center_ppd) } ;    
    real_T r          { (180.0/M_PI) * ppd         } ; 


    // rows/columns and n-s/e-w -- compute extents
    // -------------------------------------------
    
    llr_T    ref_extent_ew, ref_extent_ns;
    rc_T     rows, cols;
   
    if (mparm.ortho.rows > 0)                                 // rows explititly specified ?
    {
        rows = mparm.ortho.rows;                              // row_max can be even or odd

        if (rows < 2.0 * r)
            ref_extent_ns = 2.0 * M_asind(rows / (2.0 * r));  // compute extent based on passed-in rows
        else
            ref_extent_ns = 180.0;                            // maximum ns extent, if too many rows 
    }
    else                                                      // compute rows based on desired deg-rows
    {
       if (mparm.ortho.deg_rows > 180.0)
       {
           M_out_emsg(L"create_map_ortho() -- deg_rows too large -- %-12.6f exceeds max value of 180.0 degrees") % mparm.ortho.deg_rows; 
           return -1; 
       }
        
       ref_extent_ns = mparm.ortho.deg_rows;  
       rows          = M_round(2.0 * r * M_sind(mparm.ortho.deg_rows / 2.0)) + 1; 

       if ( ((rows - 1) % 2) != 0 ) rows++;                   // make sure rowmax will be even
    }

    if (mparm.ortho.cols > 0)                                 // cols explicitly specified ?
    {
        cols = mparm.ortho.cols;                              // col_max can be even or odd

        if (cols < 2.0 * r)
            ref_extent_ew = 2.0 * M_asind(cols / (2.0 * r));  // compute extent based on passed-in cols
        else
            ref_extent_ew = 180.0;                            // maximum ns extent, if too many cols 
    }
    else                                                      // compute cols based on desired deg-cols
    {
       if (mparm.ortho.deg_cols > 180.0)
       {
           M_out_emsg(L"create_map_ortho() -- deg_cols too large -- %-12.6f exceeds max value of 180.0 degrees") % mparm.ortho.deg_cols; 
           return -1; 
       }
        
       ref_extent_ew = mparm.ortho.deg_cols;  
       cols          = M_round(2.0 * r * M_sind(mparm.ortho.deg_cols / 2.0)) + 1; 

       if ( ((cols - 1) % 2) != 0 ) cols++;    // make sure colmax will be even
    }
   
  
    rcf_T row0    = (real_T)((rows - 1)/2);           // can be xxx.5 if input rows is even (rows should be odd here, if computed)
    rcf_T col0    = (real_T)((cols - 1)/2);           // can be xxx.5 if input cols is even (rows should be odd here, if computed)


    real_T sin_lat0 { M_sind(lat0) } ;
    real_T cos_lat0 { M_cosd(lat0) } ;
    
    M__(M_out(L"create_map_ortho() -- lat0      = %-20.10f"  ) % lat0    ;)
    M__(M_out(L"create_map_ortho() -- lon0      = %-20.10f"  ) % lon0    ;)
    M__(M_out(L"create_map_ortho() -- row0      = %-20.10f"  ) % row0    ;)
    M__(M_out(L"create_map_ortho() -- col0      = %-20.10f"  ) % col0    ;)
    M__(M_out(L"create_map_ortho() -- ppd       = %-20.10f"  ) % ppd     ;)
    M__(M_out(L"create_map_ortho() -- r         = %-20.10f"  ) % r       ;)
    M__(M_out(L"create_map_ortho() -- sin_lat0  = %-20.10f"  ) % sin_lat0;)
    M__(M_out(L"create_map_ortho() -- cos_lat0  = %-20.10f"  ) % cos_lat0;)
    M__(M_out(L"create_map_ortho() -- col0      = %-20.10f"  ) % col0    ;)
    M__(M_out(L"create_map_ortho() -- rows      = %d"        ) % rows    ;)
    M__(M_out(L"create_map_ortho() -- cols      = %d"        ) % cols    ;)
    
    
    // Create basic mapdata_T structure -- return imemdiately, if error
    // ----------------------------------------------------------------
    
    rc = create_mapdata(mapdata, rows, cols, mparm, parm);
    
    if (rc != 0) 
        return rc; 
    

    // Fill in common items directly from mparms
    // -----------------------------------------

    mapdata.north_limit  =  mparm.north_limit; 
    mapdata.south_limit  =  mparm.south_limit; 
    mapdata.east_limit   =  mparm.east_limit; 
    mapdata.west_limit   =  mparm.west_limit; 


    // Fill in ORTHO projection-specific items (for use by generic function calls below)
    // ---------------------------------------------------------------------------------
    
    mapdata.projection       = projection_E::orthographic; 
    mapdata.u.ortho.lat0     = lat0;
    mapdata.u.ortho.lon0     = lon0;
    mapdata.u.ortho.row0     = row0;
    mapdata.u.ortho.col0     = col0;
    mapdata.u.ortho.r        = r;
    mapdata.u.ortho.sin_lat0 = sin_lat0; 
    mapdata.u.ortho.cos_lat0 = cos_lat0;  
  

    // fill in projection-dependent generic functions now, so get_min_max_ll() can call them as needed
    // -----------------------------------------------------------------------------------------------
      
    mapdata.get_lat_lon_p   = &get_lat_lon_ortho;
    mapdata.get_row_col_p   = &get_row_col_ortho;


    // get min/max lat/lon via sampling, edge traversal, etc.
    // ------------------------------------------------------

    llr_T  maxlat, minlat, maxlon, minlon, midlat;

    (void)get_min_max_ll(mapdata, minlat, maxlat, minlon, maxlon, mparm, parm); 

    
    // get min/max ppd via sampling, etc.
    // -----------------------------------

     real_T   minppd, maxppd;
     (void)get_min_max_ppd(mapdata, minppd, maxppd, mparm, parm);


    
    // fill in projection-dependent standard items for external use
    // ------------------------------------------------------------

    mapdata.ref_latitude   = lat0;                      // average latitude (not at center)
    mapdata.ref_longitude  = lon0;                      // middle longitude (not at center)
    mapdata.ref_ppd        = ppd;                       // scale along meridians
    mapdata.ref_extent_ns  = ref_extent_ns;             // nominal extent 
    mapdata.ref_extent_ew  = ref_extent_ew;             // nominal extent 

    mapdata.min_latitude   = minlat; 
    mapdata.min_longitude  = minlon; 
    mapdata.max_latitude   = maxlat; 
    mapdata.max_longitude  = maxlon; 
    mapdata.min_ppd        = minppd;                    
    mapdata.max_ppd        = maxppd;                    
    

    // debug display, if requested     
    
    if (mparm.display >= 2)
    {   
        M_out(L"Orthographic projection parms: -----------------------------------");
        M_out(L"lat0                 = %-.20f" )  %  mapdata.u.ortho.lat0          ;    
        M_out(L"lon0                 = %-.20f" )  %  mapdata.u.ortho.lon0          ;  
        M_out(L"row0                 = %-.20f" )  %  mapdata.u.ortho.row0          ;    
        M_out(L"col0                 = %-.20f" )  %  mapdata.u.ortho.col0          ;     
        M_out(L"r                    = %-.20f" )  %  mapdata.u.ortho.r             ;
        M_out(L"sin_lat0             = %-.20f" )  %  mapdata.u.ortho.sin_lat0      ;    
        M_out(L"cos_lat0             = %-.20f" )  %  mapdata.u.ortho.cos_lat0      ;  
        M_out(L"rows                 = %d"     )  %  mapdata.rows                  ;
        M_out(L"cols                 = %d"     )  %  mapdata.cols                  ;
        M_out(L"ref_latitude         = %-.20f" )  %  mapdata.ref_latitude          ;  
        M_out(L"ref_longitude        = %-.20f" )  %  mapdata.ref_longitude         ;  
        M_out(L"ref_ppd              = %-.20f" )  %  mapdata.ref_ppd               ;  
        M_out(L"ref_extent_ew        = %-.20f" )  %  mapdata.ref_extent_ew         ; 
        M_out(L"ref_extent_ns        = %-.20f" )  %  mapdata.ref_extent_ns         ;
        M_out(L"north_limit          = %-.20f")  %  mapdata.north_limit           ;
        M_out(L"south_limit          = %-.20f")  %  mapdata.south_limit           ;
        M_out(L"east_limit           = %-.20f")  %  mapdata.east_limit            ;
        M_out(L"west_limit           = %-.20f")  %  mapdata.west_limit            ;
        M_out(L"min_latitude         = %-.20f" )  %  mapdata.min_latitude          ; 
        M_out(L"max_latitude         = %-.20f" )  %  mapdata.max_latitude          ;  
        M_out(L"min_longitude        = %-.20f" )  %  mapdata.min_longitude         ; 
        M_out(L"max_longitude        = %-.20f" )  %  mapdata.max_longitude         ;
        M_out(L"min_ppd              = %-.20f" )  %  mapdata.min_ppd               ; 
        M_out(L"max_ppd              = %-.20f" )  %  mapdata.max_ppd               ;
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
//||       create_map_azim() -- azimuthal equidistant projection 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//   Local functions for Azimuthal Equidistant projection rows, cols, lat, lon 
//   -------------------------------------------------------------------------

static int get_lat_lon_azim(const mapdata_S& mapdata, llr_T& lat_r, llr_T& lon_r, rcf_T row, rcf_T col, bool error_check) try
{
    if ( 
         (error_check == false)
         ||
         ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
       )
    {
        llr_T lat, lon;                                        // output values 
        llr_T lat0       { mapdata.u.azim.lat0       } ;
        llr_T lon0       { mapdata.u.azim.lon0       } ;
        real_T sin_lat0  { mapdata.u.azim.sin_lat0   } ;
        real_T cos_lat0  { mapdata.u.azim.cos_lat0   } ;
        real_T x         { col - mapdata.u.azim.col0 } ;      // x in pixels  x = easting 
        real_T y         { mapdata.u.azim.row0 - row } ;      // y in pixels  y = northing   
        
        real_T radius = sqrt(x * x + y * y); 


        // see if x, y is within the plotted area -- error, if not

        if (radius > mapdata.u.azim.r * M_PI)                // --------------- different than orthographic -------------
        {
            lat_r = -90.0;
            lon_r = -180.0;         
            return -1;
        }

        real_T c     { radius / mapdata.u.azim.r  } ;        // ------------- different than orthographic -------------
        real_T sin_c { sin(c)                     } ; 
        real_T cos_c { cos(c)                     } ; 
 
        M__(M_out(L"get_lat_lon_azim() -- row=%-20.10f   col=%-20.10f   x=%-20.10f   y=%-20.10f   radius=%-20.10f   c=%-20.10f   c(deg)=%-20.10f") % row % col % x % y % radius % c % M_deg(c);)

        if (radius == 0.0)
        {
            lat = lat0;
            lon = lon0;
        }
        else
        {
            lat = M_asind(cos_c * sin_lat0 + (y / radius) *  sin_c * cos_lat0);

            if (lat0 == 90.0)                   // north polar case
            {
                lon = lon0 + M_atan2d(x, -y);   // hopefully atan2 handles y=0 case               
            }
            else if (lat0 == -90.0)             // south polar case
            {
                lon = lon0 + M_atan2d(x, y); 
            }
            else                                // general case
            {
                lon = lon0 + M_atan2d(x * sin_c, radius * cos_lat0 * cos_c - y * sin_lat0 * sin_c);
            }
        }
        
        M__(M_out(L"get_lat_lon_azim() -- row=%-20.10f  col=%-20.10f ---->  lat=%-20.10f   lon=%-20.10f") % row % col % lat % lon;)


        // do lat/lon limits checking before returning lat/lon

        if  (            
             (lat > mapdata.north_limit) ||
             (lat < mapdata.south_limit) ||
             (lon > mapdata.east_limit)  ||
             (lon < mapdata.west_limit)   
            )
        {
            M__(M_out(L"get_lat_lon_azim() -- lat/lon=%-20.10f/%-20.10f   north/south=%-20.10f/%-20.10f  east/west=%-20.10f/%-20.10f") % lat % lon % mapdata.north_limit % mapdata.south_limit % mapdata.east_limit % mapdata.west_limit;)
            lat_r = -90.0;
            lon_r = -180.0;         
            return -1;           // return error -- results are out of limits
        }
        else
        {
            lat_r = lat; 
            lon_r = lon;
            return 0;             // return OK -- results should be valid   
        }
    }
    

    // error occured -- return -1, etc.
      
    M__(M_out(L"get_lat_lon_azim() -- row=%-20.10f  col=%-20.10f ??error??") % row % col;)
    lat_r = -90.0;
    lon_r = -180.0; 
    return -1;     
}
M_endf


//-------------------------------------------------------------------------------------------------------------

static int get_row_col_azim(const mapdata_S& mapdata, rcf_T& row_r, rcf_T& col_r, llr_T lat, llr_T lon, bool error_check) try
{
    // note: error_check is ignored here -- always in effect



    // do lat/lon limits checking before getting row/col
    // -------------------------------------------------

    if ( (lat < -90.0) || (lat > 90.0) )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_INVALID;             // return error -- input lat/lon is not valid
    }  

    if  (            
         (lat > mapdata.north_limit) ||
         (lat < mapdata.south_limit) ||
         (lon > mapdata.east_limit)  ||
         (lon < mapdata.west_limit)   
        )
    {
        row_r = 0.0;
        col_r = 0.0;         
        return M_ROW_COL_RC_OUTLIMITS;           // return error -- input lat/lon is out-of specified limits
    }
    

    // within limits -- get row, col 
    // -----------------------------

    llr_T  lon0 { mapdata.u.azim.lon0 } ;
    llr_T  lat0 { mapdata.u.azim.lat0 } ;
    llr_T  dlon { lon - lon0          } ;
    rcf_T  row, col;
    real_T xr, yr;                                 // x/R y/R in formulae   

    real_T sin_lat0 { mapdata.u.azim.sin_lat0                            } ; 
    real_T cos_lat0 { mapdata.u.azim.cos_lat0                            } ;
    real_T sin_dlon { M_sind(dlon)                                       } ;
    real_T cos_dlon { M_cosd(dlon)                                       } ; 
    real_T sin_lat  { M_sind(lat)                                        } ; 
    real_T cos_lat  { M_cosd(lat)                                        } ;     
    real_T cos_c    { sin_lat0 * sin_lat + cos_lat0 * cos_lat * cos_dlon } ;   

    M__(M_out(L"get_row_col_azim() -- sin_dlon=%.20f  cos_dlon=%.20f  sin_lat=%.20f  cos_lat=%.20f  cos_c=%.20f") % sin_dlon % cos_dlon % sin_lat % cos_lat % cos_c;)


    // find out if requested lat/lon is plottable/visible on this map  -- cannot be exactly 180 degrees from center point      
    // ------------------------------------------------------------------------------------------------------------------

    if (cos_c < -0.99999999)              // cos_c reaches -1.0 180 degrees from center point
    {
        row_r = 0.0; 
        col_r = 0.0; 
        return M_ROW_COL_RC_OUTPROJ;      // indicate out of area covered by projection  
    }
 

    // specialize the north polar case:

    if (lat0 == 90.0)
    {
        xr =  (M_PI/2.0 - M_rad(lon)) * sin_dlon; 
        yr = -(M_PI/2.0 - M_rad(lon)) * cos_dlon;

        M__(M_out(L"get_row_col_azim() -- north polar case: xr=%-20.10f  yr=%-20.10f") % xr % yr;)
    } 


    // specialize the south polar case

    else if (lat0 == -90.0)
    {
        xr =  (M_PI/2.0 + M_rad(lon)) * sin_dlon; 
        yr =  (M_PI/2.0 + M_rad(lon)) * cos_dlon;

        M__(M_out(L"get_row_col_azim() -- south polar case: xr=%-20.10f  yr=%-20.10f") % xr % yr;)
    } 


    // specialize the case where cos_c = 1.0 -- xr and yr are 0.0

    if (cos_c > 0.99999999)
    {
         xr = 0.0;
         yr = 0.0;

         M__(M_out(L"get_row_col_azim() -- map center case: xr=%-20.10f  yr=%-20.10f") % xr % yr;)
    } 

    else  // not polar, not at map center point or opposite side from center point 
    {
        real_T c { acos(cos_c) };
        real_T k { c / sin(c)  };

        M__(M_out(L"get_row_col_azim() -- cos_c=%.20f  c=%.20f  k=%.20f") % cos_c % c % k;)

        // specialize the equatorial case

        if (lat0 == 0.0)
        {
            xr =  k * cos_lat * sin_dlon; 
            yr =  k * sin_lat; 
            M__(M_out(L"get_row_col_azim() -- equatorial case: xr=%-20.10f  yr=%-20.10f") % xr % yr;)
        }
        else // general case -- neither polar nor equatorial
        {
           xr = k * cos_lat * sin_dlon; 
           yr = k * (cos_lat0 * sin_lat - sin_lat0 * cos_lat * cos_dlon);
       
           M__(M_out(L"get_row_col_azim() -- general case: xr=%-20.10f  yr=%-20.10f") % xr % yr;)
        }
    }
    

    // compute actual row/col based on x/R and y/R, and reject if out of bounds
    // ------------------------------------------------------------------------

    col = mapdata.u.azim.col0 + mapdata.u.azim.r * xr;  // x is easting -- towards right -- higher columns 
    row = mapdata.u.azim.row0 - mapdata.u.azim.r * yr;  // y is northing -- towards top -- lower columns

    M__(M_out(L"get_row_col_azim() -- lat=%-20.10f  lon=%-20.10f ::: row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)

    if ( (row >= 0.0) && (row <= mapdata.maxrow) && (col >= 0.0) && (col <= mapdata.maxcol) )
    {
        M__(M_out(L"get_row_col_azim() -- RC_OK -- lat=%-20.10f  lon=%-20.10f --->  row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)
        row_r = row; 
        col_r = col;
        return M_ROW_COL_RC_OK;        // row/col inside mapped area
    } 
    else
    {
        M__(M_out(L"get_row_col_azim() -- RC_OUTMAP -- lat=%-20.10f  lon=%-20.10f --->  row=%-20.10f   col=%-20.10f") % lat % lon % row % col;)
        row_r = row; 
        col_r = col;                    // off map, but valid input latitude  -- orthographic map may stretch out beyond map boundaries
        return M_ROW_COL_RC_OUTMAP;     // indicate off map, and out-of-bounds row, col has been returned 
    } 

    
    // error occured -- should not get here
      
    row_r = 0.0; 
    col_r = 0.0;    
    return M_ROW_COL_RC_ERROR;              // indicate error
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int create_map_azim(mapdata_S& mapdata, const create_map_parm_S& mparm, const mapparm_S& parm) try                  
{
    int      rc { 0 };
        
    
    // convert input parms to internal ORTHO parms
    // -------------------------------------------
    
    llr_T  lat0       { mparm.azim.lat0            } ;
    llr_T  lon0       { mparm.azim.lon0            } ;
    real_T ppd        { (real_T)(mparm.center_ppd) } ;    
    real_T r          { (180.0/M_PI) * ppd         } ; 


    // rows/columns and n-s/e-w -- compute extents
    // -------------------------------------------
    
    llr_T    ref_extent_ew, ref_extent_ns;
    rc_T     rows, cols;
   
    if (mparm.azim.rows > 0)                                 // rows explititly specified ?
    {
        rows = mparm.azim.rows;                              // row_max can be even or odd

        if (rows < 360.0 * ppd)
            ref_extent_ns =  rows / ppd;                     // compute extent (in degrees) based on passed-in rows
        else
            ref_extent_ns = 360.0;                           // maximum ns extent, if too many rows 
    }
    else                                                     // compute rows based on desired deg-rows
    {
       if (mparm.azim.deg_rows > 360.0)
       {
           M_out_emsg(L"create_map_azim() -- deg_rows too large -- %-12.6f exceeds max value of 360.0 degrees") % mparm.ortho.deg_rows; 
           return -1; 
       }
        
       ref_extent_ns = mparm.azim.deg_rows;  
       rows          = M_round(ppd * mparm.azim.deg_rows) + 1; 

       if ( ((rows - 1) % 2) != 0 ) rows++;                   // make sure rowmax will be even
    }

    if (mparm.azim.cols > 0)                                  // cols explicitly specified ?
    {
        cols = mparm.azim.cols;                               // col_max can be even or odd

        if (cols < 360.0 * ppd)
            ref_extent_ew = cols / ppd;                       // compute extent (in degrees) based on passed-in cols
        else
            ref_extent_ew = 360.0;                            // maximum ns extent, if too many cols 
    }
    else                                                      // compute cols based on desired deg-cols
    {
       if (mparm.azim.deg_cols > 360.0)
       {
           M_out_emsg(L"create_map_azim() -- deg_cols too large -- %-12.6f exceeds max value of 360.0 degrees") % mparm.ortho.deg_cols; 
           return -1; 
       }
        
       ref_extent_ew = mparm.azim.deg_cols;  
       cols          = M_round(ppd * mparm.azim.deg_cols) + 1;  

       if ( ((cols - 1) % 2) != 0 ) cols++;    // make sure colmax will be even
    }
   
  
    rcf_T row0    = (real_T)((rows - 1)/2);           // can be xxx.5 if input rows is even   (rows should be odd here, if computed)
    rcf_T col0    = (real_T)((cols - 1)/2);           // can be xxx.5 if input cols is even   (cols should be odd here, if computed)


    real_T sin_lat0 { M_sind(lat0) } ;
    real_T cos_lat0 { M_cosd(lat0) } ;
    
    M__(M_out(L"create_map_azim() -- lat0      = %-20.10f"  ) % lat0    ;)
    M__(M_out(L"create_map_azim() -- lon0      = %-20.10f"  ) % lon0    ;)
    M__(M_out(L"create_map_azim() -- row0      = %-20.10f"  ) % row0    ;)
    M__(M_out(L"create_map_azim() -- col0      = %-20.10f"  ) % col0    ;)
    M__(M_out(L"create_map_azim() -- ppd       = %-20.10f"  ) % ppd     ;)
    M__(M_out(L"create_map_azim() -- r         = %-20.10f"  ) % r       ;)
    M__(M_out(L"create_map_azim() -- sin_lat0  = %-20.10f"  ) % sin_lat0;)
    M__(M_out(L"create_map_azim() -- cos_lat0  = %-20.10f"  ) % cos_lat0;)
    M__(M_out(L"create_map_azim() -- col0      = %-20.10f"  ) % col0    ;)
    M__(M_out(L"create_map_azim() -- rows      = %d"        ) % rows    ;)
    M__(M_out(L"create_map_azim() -- cols      = %d"        ) % cols    ;)
    
    
    // Create basic mapdata_T structure -- return imemdiately, if error
    // ----------------------------------------------------------------
    
    rc = create_mapdata(mapdata, rows, cols, mparm, parm);
    
    if (rc != 0) 
        return rc; 
    

    // Fill in common items directly from mparms
    // -----------------------------------------

    mapdata.north_limit  =  mparm.north_limit; 
    mapdata.south_limit  =  mparm.south_limit; 
    mapdata.east_limit   =  mparm.east_limit; 
    mapdata.west_limit   =  mparm.west_limit; 


    // Fill in Azimuthal projection-specific items (for use by generic function calls below)
    // -------------------------------------------------------------------------------------
    
    mapdata.projection      = projection_E::azimuthal; 
    mapdata.u.azim.lat0     = lat0;
    mapdata.u.azim.lon0     = lon0;
    mapdata.u.azim.row0     = row0;
    mapdata.u.azim.col0     = col0;
    mapdata.u.azim.r        = r;
    mapdata.u.azim.sin_lat0 = sin_lat0; 
    mapdata.u.azim.cos_lat0 = cos_lat0;  
  

    // fill in projection-dependent generic functions now, so get_min_max_ll() can call them as needed
    // -----------------------------------------------------------------------------------------------
      
    mapdata.get_lat_lon_p   = &get_lat_lon_azim;
    mapdata.get_row_col_p   = &get_row_col_azim;


    // get min/max lat/lon via sampling, edge traversal, etc.
    // ------------------------------------------------------

    llr_T  maxlat, minlat, maxlon, minlon, midlat;

    (void)get_min_max_ll(mapdata, minlat, maxlat, minlon, maxlon, mparm, parm); 

    
    // get min/max ppd via sampling, etc.
    // -----------------------------------

     real_T   minppd, maxppd;
     (void)get_min_max_ppd(mapdata, minppd, maxppd, mparm, parm);


    
    // fill in projection-dependent standard items for external use
    // ------------------------------------------------------------

    mapdata.ref_latitude   = lat0;                      // average latitude (not at center)
    mapdata.ref_longitude  = lon0;                      // middle longitude (not at center)
    mapdata.ref_ppd        = ppd;                       // scale along meridians
    mapdata.ref_extent_ns  = ref_extent_ns;             // nominal extent 
    mapdata.ref_extent_ew  = ref_extent_ew;             // nominal extent 

    mapdata.min_latitude   = minlat; 
    mapdata.min_longitude  = minlon; 
    mapdata.max_latitude   = maxlat; 
    mapdata.max_longitude  = maxlon; 
    mapdata.min_ppd        = minppd;                    
    mapdata.max_ppd        = maxppd;                    
    

    // debug display, if requested     
    
    if (mparm.display >= 2)
    {   
        M_out(L"Azimuthal equidistant projection parms: -----------------------------------");
        M_out(L"lat0                 = %-.20f" )  %  mapdata.u.azim.lat0           ;    
        M_out(L"lon0                 = %-.20f" )  %  mapdata.u.azim.lon0           ;  
        M_out(L"row0                 = %-.20f" )  %  mapdata.u.azim.row0           ;    
        M_out(L"col0                 = %-.20f" )  %  mapdata.u.azim.col0           ;     
        M_out(L"r                    = %-.20f" )  %  mapdata.u.azim.r              ;
        M_out(L"sin_lat0             = %-.20f" )  %  mapdata.u.azim.sin_lat0       ;    
        M_out(L"cos_lat0             = %-.20f" )  %  mapdata.u.azim.cos_lat0       ;  
        M_out(L"rows                 = %d"     )  %  mapdata.rows                  ;
        M_out(L"cols                 = %d"     )  %  mapdata.cols                  ;
        M_out(L"ref_latitude         = %-.20f" )  %  mapdata.ref_latitude          ;  
        M_out(L"ref_longitude        = %-.20f" )  %  mapdata.ref_longitude         ;  
        M_out(L"ref_ppd              = %-.20f" )  %  mapdata.ref_ppd               ;  
        M_out(L"ref_extent_ew        = %-.20f" )  %  mapdata.ref_extent_ew         ; 
        M_out(L"ref_extent_ns        = %-.20f" )  %  mapdata.ref_extent_ns         ;
        M_out(L"north_limit          = %-.20f" )  %  mapdata.north_limit           ;
        M_out(L"south_limit          = %-.20f" )  %  mapdata.south_limit           ;
        M_out(L"east_limit           = %-.20f" )  %  mapdata.east_limit            ;
        M_out(L"west_limit           = %-.20f" )  %  mapdata.west_limit            ;
        M_out(L"min_latitude         = %-.20f" )  %  mapdata.min_latitude          ; 
        M_out(L"max_latitude         = %-.20f" )  %  mapdata.max_latitude          ;  
        M_out(L"min_longitude        = %-.20f" )  %  mapdata.min_longitude         ; 
        M_out(L"max_longitude        = %-.20f" )  %  mapdata.max_longitude         ;
        M_out(L"min_ppd              = %-.20f" )  %  mapdata.min_ppd               ; 
        M_out(L"max_ppd              = %-.20f" )  %  mapdata.max_ppd               ;
    }    

    return 0; 
}
M_endf






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_min_max_ppd() -- find min/max ppd on map (may be approximate)  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_min_max_ppd(const mapdata_S& mapdata, real_T& minppd_r, real_T& maxppd_r, const create_map_parm_S& mparm, const mapparm_S& parm) try
{
    rcf_T row, col; 
    rcf_T lat, lon;

   
    // set up random number generators

    auto rand_row = std::bind( std::uniform_real_distribution<rcf_T>{0, mapdata.maxrow - 1.0}     // don't do last row                          
                             , std::default_random_engine{1}
                             ); 
    auto rand_col = std::bind( std::uniform_real_distribution<rcf_T>{0, mapdata.maxcol - 1.0}      // don't do last column                    
                             , std::default_random_engine{1234567890}
                             ); 
   

    real_T   minppd   {  1000000.0 }; 
    real_T   maxppd   {  0.0       };
    real_T   ppd_row, ppd_col;


    //// --------------------------------------
    //// loop -- once for each sample requested
    //// --------------------------------------

    for (auto i = 1; i <= mparm.samples/5; i++)
    {
        row = rand_row();  
        col = rand_col(); 

        auto rc = get_rc_ppd(mapdata, ppd_row, ppd_col, row, col, parm);

        if (rc == 0)
        {
            minppd = std::min(std::min(ppd_row, ppd_col), minppd);
            maxppd = std::max(std::max(ppd_row, ppd_col), maxppd); 
        }
        
        if (mparm.display >= 2)
        {
            if ( 
                (i == 10)             ||
                (i == 20)             ||
                (i == 50)             ||
                (i == 100)            || 
                (i == 1000)           ||
                (i == 2000)           ||
                (i == 5000)           ||
                (i == 10000)          ||
                (i == 20000)          ||
                (i == mparm.samples)  ||
                ((i % 50000) == 0)
               )
            {
              M_out(L"get_min_max_ppd() -- i=%-7d row=%-10.4f  col=%-10.4f  minppd=%-20.12f  maxppd=%-20.12f") % i % row % col % minppd % maxppd;
            } 
        }
    }
  

    // return min/max values in caller's variables

    minppd_r = minppd;
    maxppd_r = maxppd; 


    return 0; 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_rc_ppd() -- find row and column ppd at passed-in x/y point on map   
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int get_rc_ppd(const mapdata_S& mapdata, real_T& ppd_row, real_T& ppd_col, rcf_T row, rcf_T col, const mapparm_S& parm) try
{
    llr_T lat0, lon0, lat1, lon1, lat2, lon2;      


    // get latitude and longitude around passed-in row/col
    // ---------------------------------------------------

    auto rc0 = (*(mapdata.get_lat_lon_p))(mapdata, lat0, lon0, row      , col      , true); 
    auto rc1 = (*(mapdata.get_lat_lon_p))(mapdata, lat1, lon1, row + 1.0, col      , true); 
    auto rc2 = (*(mapdata.get_lat_lon_p))(mapdata, lat2, lon2, row      , col + 1.0, true); 

    if (( rc0 != 0) || (rc1 != 0) || (rc2 != 0) )                                            // if 1st attempt failed (up and right), try going down and left
    {
       rc0 = (*(mapdata.get_lat_lon_p))(mapdata, lat0, lon0, row      , col      , true);
       rc1 = (*(mapdata.get_lat_lon_p))(mapdata, lat1, lon1, row - 1.0, col      , true);
       rc2 = (*(mapdata.get_lat_lon_p))(mapdata, lat2, lon2, row      , col - 1.0, true);

       if (( rc0 != 0) || (rc1 != 0) || (rc2 != 0) )   
           return -1;                                                                       // both attempts failed -- return with error
    }


    // compute DPP in row and column directions

    real_T dpp_col = std::sqrt( (lat2 - lat0) * (lat2 - lat0) + (lon2 - lon0) * (lon2 - lon0) );
    real_T dpp_row = std::sqrt( (lat1 - lat0) * (lat1 - lat0) + (lon1 - lon0) * (lon1 - lon0) );


    // convert to PPD and pass values back  

    if (dpp_col != 0)
        ppd_col = 1.0/dpp_col;
    else
        ppd_col = 1000000.0; 

    if (dpp_row != 0)
        ppd_row = 1.0/dpp_row;
    else
        ppd_row = 1000000.0; 


    M__(M_out(L"get_rc_ppd() -- ppd=%-16.8f/%-16.8f    dpp=%-16.8f/%-16.8f  ll0=%-16.8f/%-16.8f  ll1=%-16.8f/%-16.8f  ll2=%-16.8f/%-16.8f") % ppd_row % ppd_col % dpp_row % dpp_col % lat0 % lon0 % lat1 % lon1 % lat2 % lon2;)


    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_ll_ppd() -- find latitude and longitude ppd at passed-in lat/lon point on map   
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int get_ll_ppd(const mapdata_S& mapdata, real_T& ppd_lat, real_T& ppd_lon, rcf_T lat, rcf_T lon, const mapparm_S& parm) try
{
    rcf_T row0, col0, row1, col1, row2, col2;      

    llr_T delta {0.01};       // look this far away from passed-in lat/lon when computing ppd


    // get row/col around passed-in lat/lon
    // ------------------------------------

    auto rc0 = (*(mapdata.get_row_col_p))(mapdata, row0, col0, lat        , lon        , true); 
    auto rc1 = (*(mapdata.get_row_col_p))(mapdata, row1, col1, lat + delta, lon        , true); 
    auto rc2 = (*(mapdata.get_row_col_p))(mapdata, row2, col2, lat        , lon + delta, true);  

    M__(M_out(L"get_ll_ppd() -- lat/lon = %.6f/%.6f   rc = %d/%d/%d") % lat % lon % rc0 % rc1 % rc2;)

    if (( rc0 <= M_ROW_COL_RC_OUTPROJ) || (rc1 <= M_ROW_COL_RC_OUTPROJ) || (rc2 <= M_ROW_COL_RC_OUTPROJ) )    // if 1st attempt failed (N and E), try going S and W
    {
       rc0 = (*(mapdata.get_lat_lon_p))(mapdata, row0, col0, lat        , lon        , true);
       rc1 = (*(mapdata.get_lat_lon_p))(mapdata, row1, col1, lat - delta, lon        , true);
       rc2 = (*(mapdata.get_lat_lon_p))(mapdata, row2, col2, lat        , lon - delta, true);

       if (( rc0 >= M_ROW_COL_RC_OUTPROJ) || (rc1 >= M_ROW_COL_RC_OUTPROJ) || (rc2 >= M_ROW_COL_RC_OUTPROJ) )    
           return -1;                                                                                         // both attempts failed -- return with error
    }


    // compute PPD in latitude and longitude directions
    // ------------------------------------------------

    ppd_lat = hypot(row1 - row0, col1 - col0) / delta; 
    ppd_lon = hypot(row2 - row0, col2 - col0) / delta; 

   // ppd_lat = std::sqrt( (row1 - row0) * (row1 - row0) + (col1 - col0) * (col1 - col0) ) / delta;
   // ppd_lon = std::sqrt( (row2 - row0) * (row2 - row0) + (col2 - col0) * (col2 - col0) ) / delta;
    
    M__(M_out(L"get_ll_ppd() -- ppd=%-16.8f/%-16.8f  rc0=%-16.8f/%-16.8f  rc1=%-16.8f/%-16.8f  rc2=%-16.8f/%-16.8f") % ppd_lat % ppd_lon %  row0 % col0 % row1 % col1 % row2 % col2;)
   
    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_ll_rotation() -- find latitude and longitude based graticule rotation at passed-in lat/lon point on map   
//||
//||                            (0 degrees  = increasing longitude to the right)
//||                            (90 degrees = increasing latitude straight up)
//|| 
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int get_ll_rotation(const mapdata_S& mapdata, real_T& rot_lat, real_T& rot_lon, rcf_T lat, rcf_T lon, const mapparm_S& parm) try
{
    rcf_T row0, col0, row1, col1, row2, col2;      

    llr_T delta {0.01};       // look this far away from passed-in lat/lon when computing ppd


    // get row/col around passed-in lat/lon
    // ------------------------------------

    auto rc0 = (*(mapdata.get_row_col_p))(mapdata, row0, col0, lat        , lon        , true); 
    auto rc1 = (*(mapdata.get_row_col_p))(mapdata, row1, col1, lat + delta, lon        , true); 
    auto rc2 = (*(mapdata.get_row_col_p))(mapdata, row2, col2, lat        , lon + delta, true);  

    llr_T diff_row_lat = row1 - row0; 
    llr_T diff_col_lat = col1 - col0;  
    llr_T diff_row_lon = row2 - row0; 
    llr_T diff_col_lon = col2 - col0;  

    M__(M_out(L"get_ll_ppd() -- Going N and E -- lat/lon = %.6f/%.6f   rc = %d/%d/%d") % lat % lon % rc0 % rc1 % rc2;)

    if (( rc0 <= M_ROW_COL_RC_OUTPROJ) || (rc1 <= M_ROW_COL_RC_OUTPROJ) || (rc2 <= M_ROW_COL_RC_OUTPROJ) )    // if 1st attempt failed (N and E), try going S and W
    {
       M_y(M_out(L"get_ll_ppd() -- retrying due to bad R/C going N and E -- rc = %d/%d/%d") % lat % lon % rc0 % rc1 % rc2;)

       rc0 = (*(mapdata.get_lat_lon_p))(mapdata, row0, col0, lat        , lon        , true);
       rc1 = (*(mapdata.get_lat_lon_p))(mapdata, row1, col1, lat - delta, lon        , true);
       rc2 = (*(mapdata.get_lat_lon_p))(mapdata, row2, col2, lat        , lon - delta, true);

       M_y(M_out(L"get_ll_ppd() -- Going S and W -- lat/lon = %.6f/%.6f   rc = %d/%d/%d") % lat % lon % rc0 % rc1 % rc2;)

       if (( rc0 >= M_ROW_COL_RC_OUTPROJ) || (rc1 >= M_ROW_COL_RC_OUTPROJ) || (rc2 >= M_ROW_COL_RC_OUTPROJ) )    
           return -1;                                 // both attempts failed -- return with error

       diff_row_lat = row0 - row1; 
       diff_col_lat = col0 - col1;  
       diff_row_lon = row0 - row2; 
       diff_col_lon = col0 - col2;
    }


    // compute rotation angle in latitude and longitude directions
    // -----------------------------------------------------------
    //
    // note: lower row numbers are higher Y, so row diff sign is negative
    // note: this is math rotation angle on map -- gdlplus transform rotation is the negative of this  
    //

    rot_lat = M_atan2d(-diff_row_lat, diff_col_lat);
    rot_lon = M_atan2d(-diff_row_lon, diff_col_lon);
  
    M__(M_out(L"get_ll_rotation() -- lat/lon=%.6f/%.6f  rotation=%-.8f/%-.8f  diff_r/c_lat=%.8f/%.8f  diff_r/c_lon=%.8f/%.8f") % lat % lon % rot_lat % rot_lon %  diff_row_lat % diff_col_lat % diff_row_lon % diff_col_lon;)
   
    return 0; 
}
M_endf





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_min_max_ll() -- find min/max lat/lon on map (may be approximate)  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_min_max_ll(const mapdata_S& mapdata, llr_T& minlat_r, llr_T& maxlat_r, llr_T&  minlon_r, llr_T& maxlon_r, const create_map_parm_S& mparm, const mapparm_S& parm) try
{
    // use random sampling approach
    // ----------------------------

    llr_T   minlat   {  1000.0 }; 
    llr_T   maxlat   { -1000.0 };
    llr_T   minlon   {  1000.0 }; 
    llr_T   maxlon   { -1000.0 }; 
    
    (void)get_samples_lat_lon(mapdata, minlat, maxlat, minlon, maxlon, mparm, parm);    // should always work -- don't check r/c


    if (mparm.display > 1)
    {
        M_out(L"get_min_max_ll() -- from samples :  minlat=%-20.10f     maxlat=%-20.10f     minlon=%-20.10f    maxlon=%-20.10f") % minlat  % maxlat  % minlon  % maxlon;
    }

    // use edge traversal approach
    // ---------------------------

    llr_T   minlat1  {  1000.0 }; 
    llr_T   maxlat1  { -1000.0 };
    llr_T   minlon1  {  1000.0 }; 
    llr_T   maxlon1  { -1000.0 };  

    auto rc = get_edge_lat_lon(mapdata, minlat1, maxlat1, minlon1, maxlon1, mparm, parm);    // should always work 
   
    if (mparm.display > 1)
    {
        M_out(L"get_min_max_ll() -- from edge    : minlat1=%-20.10f    maxlat1=%-20.10f    minlon1=%-20.10f   maxlon1=%-20.10f   rc=%d") % minlat1 % maxlat1 % minlon1 % maxlon1 % rc;
    }
     
    if (rc == 0)
    {
        minlat = std::min(minlat, minlat1); 
        maxlat = std::max(maxlat, maxlat1);
        minlon = std::min(minlon, minlon1); 
        maxlon = std::max(maxlon, maxlon1);  
    } 

    if (mparm.display > 1)
    {
        M_out(L"get_min_max_ll() -- from both    :  minlat=%-20.10f     maxlat=%-20.10f     minlon=%-20.10f    maxlon=%-20.10f") % minlat % maxlat % minlon % maxlon;
    }


    // return min/max values in caller's variables

    minlat_r = minlat;
    maxlat_r = maxlat; 
    minlon_r = minlon;
    maxlon_r = maxlon;

    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_edge_lat_lon() -- find min/max lat/lon on edge of map  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


static int get_edge_lat_lon(const mapdata_S& mapdata, llr_T& minlat_r, llr_T& maxlat_r, llr_T&  minlon_r, llr_T& maxlon_r, const create_map_parm_S& mparm, const mapparm_S& parm) try
{
    rcf_T row, col; 
    llr_T lat, lon;
     
    llr_T    minlat   {  1000.0 }; 
    llr_T    maxlat   { -1000.0 };
    llr_T    minlon   {  1000.0 }; 
    llr_T    maxlon   { -1000.0 }; 
    int32_t  n_points { 0       }; 


    //// ----------------------------------------------
    //// first loop -- move along right and left edges
    //// ----------------------------------------------

    for (row = 0.0; row <= mapdata.maxrow; row += 1.0)
    {       
        // get lat/lon on some point on left edge -- col=0.0 
        // ------------------------------------------------

        auto rc = (*(mapdata.get_lat_lon_p))(mapdata, lat, lon, row, 0.0, true); 
            

        // If this edge point is valid, update max/min lat/lon seen so far

        if (rc == 0)
        {         
            n_points++; 
            minlat = std::min(minlat, lat);
            maxlat = std::max(maxlat, lat); 
            minlon = std::min(minlon, lon);
            maxlon = std::max(maxlon, lon); 
        }


        // get lat/lon on some point on right edge -- col=maxcol 
        // ------------------------------------------------------

        rc = (*(mapdata.get_lat_lon_p))(mapdata, lat, lon, row, mapdata.maxcol, true); 
          

        // If this edge point is valid, update max/min lat/lon seen so far

        if (rc == 0)
        {         
            n_points++; 
            minlat = std::min(minlat, lat);
            maxlat = std::max(maxlat, lat); 
            minlon = std::min(minlon, lon);
            maxlon = std::max(maxlon, lon); 
        } 

    }


    //// ----------------------------------------------
    //// second loop -- move along top and bottom edges
    //// ----------------------------------------------

    for (col = 0.0; col <= mapdata.maxcol; col += 1.0)
    {
        // get lat/lon on some point on top edge -- row=0.0 
        // ------------------------------------------------

        auto rc = (*(mapdata.get_lat_lon_p))(mapdata, lat, lon, 0.0, col, true); 
          

        // If this edge point is valid, update max/min lat/lon seen so far

        if (rc == 0)
        {         
            n_points++; 
            minlat = std::min(minlat, lat);
            maxlat = std::max(maxlat, lat); 
            minlon = std::min(minlon, lon);
            maxlon = std::max(maxlon, lon); 
        } 


        // get lat/lon on some point on bottom edge -- row=maxrow 
        // ------------------------------------------------------

        rc = (*(mapdata.get_lat_lon_p))(mapdata, lat, lon, mapdata.maxrow, col, true); 
          

        // If this edge point is valid, update max/min lat/lon seen so far

        if (rc == 0)
        {         
            n_points++; 
            minlat = std::min(minlat, lat);
            maxlat = std::max(maxlat, lat); 
            minlon = std::min(minlon, lon);
            maxlon = std::max(maxlon, lon); 
        } 
    }


    // return min/max values in caller's variables
     
    if (n_points > 0)
    {
        minlat_r = minlat;
        maxlat_r = maxlat; 
        minlon_r = minlon;
        maxlon_r = maxlon;
        return 0; 
    }
    else                                   // no valid points seen -- error r/c -- output values not set 
    {
       return -1;
    }
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_samples_lat_lon() -- find min/max lat/lon in map by sampling random rows and columns  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_samples_lat_lon(const mapdata_S& mapdata, llr_T& minlat_r, llr_T& maxlat_r, llr_T&  minlon_r, llr_T& maxlon_r, const create_map_parm_S& mparm, const mapparm_S& parm) try
{
    rcf_T row, col; 
    rcf_T lat, lon;

   
    // set up random number generators

    auto rand_row = std::bind( std::uniform_real_distribution<rcf_T>{0, mapdata.maxrow}                               
                             , std::default_random_engine{1}
                             ); 
    auto rand_col = std::bind( std::uniform_real_distribution<rcf_T>{0, mapdata.maxcol}                          
                             , std::default_random_engine{1234567890}
                             ); 

    llr_T   minlat   {  1000.0 }; 
    llr_T   maxlat   { -1000.0 };
    llr_T   minlon   {  1000.0 }; 
    llr_T   maxlon   { -1000.0 }; 


    //// --------------------------------------
    //// loop -- once for each sample requested
    //// --------------------------------------

    for (auto i = 1; i <= mparm.samples; i++)
    {
        row = rand_row();  
        col = rand_col(); 

        auto rc = (*(mapdata.get_lat_lon_p))(mapdata, lat, lon, row, col, true);

        if (rc == 0)
        {
            minlat = std::min(minlat, lat);
            maxlat = std::max(maxlat, lat); 
            minlon = std::min(minlon, lon);
            maxlon = std::max(maxlon, lon); 
        }


        if (mparm.display >= 2)
        {
            if ( 
                (i == 10)             ||
                (i == 20)             ||
                (i == 50)             ||
                (i == 100)            || 
                (i == 1000)           ||
                (i == 2000)           ||
                (i == 5000)           ||
                (i == 10000)          ||
                (i == 20000)          ||
                (i == mparm.samples)  ||
                ((i % 50000) == 0)
               )
            {
              M_out(L"get_samples_lat_lon() -- i=%-7d row=%-10.4f  col=%-10.4f  minlat=%-20.12f  maxlat=%-20.12f  minlon=%-20.12f  maxlon=%-20.12f") % i % row % col % minlat % maxlat % minlon % maxlon;
            } 
        }
    }
  


    // apply minlat/minlon/etc. padding, but don't go out-of-bounds
   
    if (mparm.display >= 2)
    {
        M_out(L"get_samples_lat_lon() -- padding=%-20.10f    minlat=%-20.10f    maxlat=%-20.10f    minlon=%-20.10f   maxlon=%-20.10f") % mparm.padding % minlat % maxlat % minlon % maxlon;
    }


    minlat = std::max(std::max(-90.0 , mapdata.south_limit), minlat - mparm.padding);
    maxlat = std::min(std::min( 90.0 , mapdata.north_limit), maxlat + mparm.padding);
    minlon = std::max(std::max(-360.0, mapdata.west_limit ), minlon - mparm.padding);
    maxlon = std::min(std::min( 360.0, mapdata.east_limit ), maxlon + mparm.padding);
    
    if (mparm.display >= 2)
    {
        M_out(L"get_samples_lat_lon() -- padding=%-20.10f    minlat=%-20.10f    maxlat=%-20.10f    minlon=%-20.10f   maxlon=%-20.10f") % mparm.padding % minlat % maxlat % minlon % maxlon;
    }


    // return min/max values in caller's variables

    minlat_r = minlat;
    maxlat_r = maxlat; 
    minlon_r = minlon;
    maxlon_r = maxlon;

    return 0; 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_mapdata() -- empty buffer initialized to 0xBBBB = -30567
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int create_mapdata(mapdata_S& mapdata, rc_T rows, rc_T cols, const create_map_parm_S& mparm, const mapparm_S& parm) try                   
{
   

    M_clearv(mapdata.u);   // clear out whole structure first -- projection specific areas don't get filled in in this routine

    mapdata.cols           = cols;
    mapdata.rows           = rows;
    mapdata.npts           = (uint64_t)(mapdata.cols) * (uint64_t)(mapdata.rows); 
    mapdata.maxrow         = (real_T)(rows - 1); 
    mapdata.maxcol         = (real_T)(cols - 1); 


    // allocate buffers 
    // -----------------

     mapdata.buf.alloc(   mapdata.npts);
     mapdata.attr.alloc(  mapdata.npts);
     mapdata.slope.alloc( mapdata.npts);


    // initialize data in buffers
    // --------------------------
    
    mapdata.buf.assign1(0xBB);        // set all bytes to "missing data" height value = 0xBBBB 
    mapdata.attr.clear(); 
    mapdata.slope.clear();
    

    // clear out map window area -- mark as invalid
    // -----------------------------------------

     mapdata.map_window_valid        = false ;
     mapdata.map_window_height       =  0    ;
     mapdata.map_window_width        =  0    ;
     mapdata.map_window_top_row      =  0    ;
     mapdata.map_window_bottom_row   =  0    ;
     mapdata.map_window_left_col     =  0    ;
     mapdata.map_window_right_col    =  0    ;

      
    // optional logout of mapdata fields
    // ---------------------------------
    
    if (mparm.display >= 2)
    {
        M_out(L"mapdata.cols         = %d"       )    % mapdata.cols                 ;
        M_out(L"mapdata.rows         = %d"       )    % mapdata.rows                 ;
        M_out(L"mapdata.maxrow       = %-10.1f"  )    % mapdata.maxrow               ;
        M_out(L"mapdata.maxcol       = %-10.1f"  )    % mapdata.maxcol               ;
        M_out(L"mapdata.npts         = %s"       )    % M_sep(mapdata.npts)          ;
        M_out(L"mapdata.buf.p        = %p"       )    % (void *)(mapdata.buf.p)      ;
        M_out(L"mapdata.buf.p1       = %p"       )    % (void *)(mapdata.buf.p1)     ;
        M_out(L"mapdata.attr.p       = %p"       )    % (void *)(mapdata.attr.p)     ;
        M_out(L"mapdata.attr.p1      = %p"       )    % (void *)(mapdata.attr.p1)    ;
        M_out(L"mapdata.slope.p      = %p"       )    % (void *)(mapdata.slope.p)    ;
        M_out(L"mapdata.slope.p1     = %p"       )    % (void *)(mapdata.slope.p1)   ;
        M_out(L"mapdata.buf.sz       = %s"       )    % M_sep(mapdata.buf.sz   )     ;
        M_out(L"mapdata.buf.sz1      = %s"       )    % M_sep(mapdata.buf.sz1  )     ;
        M_out(L"mapdata.attr.sz      = %s"       )    % M_sep(mapdata.attr.sz  )     ;
        M_out(L"mapdata.attr.sz1     = %s"       )    % M_sep(mapdata.attr.sz1 )     ;
        M_out(L"mapdata.slope.sz     = %s"       )    % M_sep(mapdata.slope.sz )     ;
        M_out(L"mapdata.slope.sz1    = %s"       )    % M_sep(mapdata.slope.sz1)     ;
    }

    if (mparm.display >= 1)
        M_out(L"Map     : size=%.3f MB  rows=%d  cols=%d") % M_meg(mapdata.buf.sz1 + mapdata.attr.sz1 + mapdata.slope.sz1) % mapdata.rows % mapdata.cols;

    return 0;
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       destroy_map()
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void destroy_map(mapdata_S& mapdata, const mapparm_S& parm) try
{
    // free up buffers

    mapdata.buf.free();
    mapdata.attr.free();
    mapdata.slope.free();
    return; 
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       clear_temp_map() -- clear out temporary attribute flag in map
//||
//||
//||        pp -> array of THREAD_MAX thp_t structures (output area -- filled in)
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void clear_temp_map(mapdata_S& mapdata, rc_T r0, rc_T rm, rc_T c0, rc_T cm)  try
{
    attr_T mask = ~MAPDATA_TEMP;  

    for (rc_T row = r0; row <= rm; row++)
    {
       for(rc_T col = c0; col <= cm; col++)
       {
            M_mapdata_attr(mapdata, row, col) &= mask;
       }
    }

    return;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       slice_map() -- partition the map data for multi-threading 
//||
//||
//||
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void slice_map(const mapdata_S& mapdata, thpa_S& thpa, int32_t max_threads, const mapparm_S& parm) try
{
    slice_row_col(thpa, 0, mapdata.rows - 1, 0, mapdata.cols - 1, max_threads, true, parm); 
    return;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       slice_map_row() -- partition the map data for multi-threading -- row slices
//||
//||
//||       
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void slice_map_row(const mapdata_S& mapdata, thpa_S& thpa, int32_t max_threads, const mapparm_S& parm) try
{
    slice_row(thpa, 0, mapdata.rows - 1, 0, mapdata.cols - 1, max_threads, true, parm); 
    return;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       slice_map_col() -- partition the map data for multi-threading  -- column slices
//||
//||
//||      
//||
//||        
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void slice_map_col(const mapdata_S& mapdata, thpa_S& thpa, int32_t max_threads, const mapparm_S& parm) try 
{
    slice_col(thpa, 0, mapdata.rows - 1, 0, mapdata.cols - 1, max_threads, true, parm); 
    return;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       fill_map()  -- fill in map elevations from raster  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void fill_map(mapdata_S& mapdata, const raster_S& raster, const fill_map_parm_S& fparm, const mapparm_S& parm) try
{ 
    std::thread    thd[THREAD_MAX];
    thpa_S         thpa;                       // thread parms
    int32_t        i; 
    
    if (fparm.display >= 1)
    {
        M_out(L"Map     : fill-parm.sampling_method      = %d") % fparm.sampling_method; 
        M_out(L"Map     : fill-parm.normal_fill          = %s") % M_bool_cstr(fparm.normal_fill); 
        M_out(L"Map     : fill-parm.ocean_fill           = %s") % M_bool_cstr(fparm.ocean_fill);


    
    }
    

    M_start_timer(c1, t1)
       
    slice_map_row(mapdata, thpa, parm.thread.n_map_fill, parm);   // allow up to n threads for mapdata fill operation

    if (fparm.ocean_fill)
        M_out(L"Map     : ocean-only pixel fill starting -- %d threads"   ) % thpa.n_threads;
    else
        M_out(L"Map     : unconditional pixel fill starting -- %d threads") % thpa.n_threads; 
  
    for (i = 0; i < thpa.n_threads; i++)
    {
        thd[i] = std::thread( fill_map_thd,   std::ref(mapdata), std::cref(raster), std::ref((thpa.thp)[i]), std::cref(fparm), std::cref(parm) );
    }
     
    for (i = 0; i < thpa.n_threads; i++)
    {
        thd[i].join();         
    }   
    
    M_out(L"Map     : pixel fill ending -- elapsed time = %.3f seconds") % M_read_timer(c1, t1);

    return;  
}
M_endf

/////////////////////////////////////

static void fill_map_thd(mapdata_S& mapdata, const raster_S& raster, thp_S& thp, const fill_map_parm_S& fparm, const mapparm_S& parm) try
{ 
    int32_t  row, erow; 
    int32_t  col, ecol;  
    real_T   lat, lon;
    attr_T   attr; 
    ht_T     ht; 
    int      rc;

    int32_t  row_min         { thp.e_r0  } ; 
    int32_t  row_max         { thp.e_rm  } ;
    int32_t  col_min         { thp.e_c0  } ;
    int32_t  col_max         { thp.e_cm  } ;
    
    int32_t  ct_bad_proj     { 0         } ; 
    int32_t  ct_bad_raster   { 0         } ;
    int32_t  ct_ocean        { 0         } ;
    int32_t  ct_ocean_bad    { 0         } ;
    int32_t  ct_ocean_valid  { 0         } ;
    real_T   bad_lon_lo      {  100000.0 } ;
    real_T   bad_lon_hi      { -100000.0 } ;
    real_T   bad_lat_lo      {  100000.0 } ;
    real_T   bad_lat_hi      { -100000.0 } ;

    M__(M_out(L"Thread %3d: starting-- rows=%-5d/%-5d  cols=%-5d/%-5d") % thp.thread_n % row_min % row_max % col_min % col_max;)


    // Do nearest neighbor sampling of the points in the raster
    // --------------------------------------------------------

    if (fparm.sampling_method == 1)
    {
        for (row = row_min; row <= row_max; row++)
        {
            for (col = col_min; col <= col_max; col++)
            {
                // if doing ocean fill only, skip to next map pixel unless this pixel has the OCEAN attribute bit or is not valid

                if (fparm.ocean_fill)
                {
                    attr = M_mapdata_attr(mapdata, row, col); 
                
                    if ( (attr & MAPDATA_VALID) && (!(attr & MAPDATA_OCEAN)) )
                       continue;                   
                }

                rc = (*(mapdata.get_lat_lon_p))(mapdata, lat, lon, (real_T)row, (real_T)col, true);   // find latitude/longitude for this map pixel

                if (rc == 0) 
                {
                    get_raster_nearest(raster, lat, lon, ht);          // extract height from raster based on lat/lon
       

                    // special ocean-only fill processing -- just update height (depth) value from raster, if valid, set depth and mark valid, if not currently valid

                    if (fparm.ocean_fill)                       
                    {
                        if ( (ht != RASTER_OUT) && (ht != RASTER_EMPTY) )
                        {
                            if (attr & MAPDATA_VALID)
                            {
                                M_mapdata_ht(mapdata, row, col) = ht;       // update ocean depth field only
                                ct_ocean++;
                            }
                            else    // attribute is not valid -- switch to VALID + OCEAN + WATER
                            {
                                M_mapdata_ht(  mapdata, row, col) = ht;     // update ocean depth field
                                attr |= (MAPDATA_OCEAN | MAPDATA_VALID);    // indicate valid 
                                M_mapdata_attr(mapdata, row, col) = attr;   // update attribute byte
                                ct_ocean_valid++;            
                            }
                        }
                         else
                        {
                            ct_ocean_bad++;
                        }
                        continue;    
                    }


                    // normal (unconditional fill processing

                    // set mapdata attr byte based on returned height

                    attr = 0;                        // start with empty attr bytes for mapdata

                    if (ht == RASTER_OUT)            // out-of-bounds of raster?
                        attr = MAPDATA_OFF;          // indicate so in attr bytes
                    else if (ht != RASTER_EMPTY)     // not unfilled (invalid) data byte in raster?
                        attr = MAPDATA_VALID;        // indicate valid point in mapdata

                    M_mapdata_attr(mapdata, row, col) = attr;
                    M_mapdata_ht(  mapdata, row, col) = ht; 

                    if (attr & MAPDATA_OFF)
                    {
                        ct_bad_raster++;
                        bad_lat_hi = M_max(bad_lat_hi, lat); 
                        bad_lat_lo = M_min(bad_lat_lo, lat); 
                        bad_lon_hi = M_max(bad_lon_hi, lon); 
                        bad_lon_lo = M_min(bad_lon_lo, lon); 
                    }
                }
                else   // this x,y point is out of bounds for the map projection
                {
                    if (!fparm.ocean_fill)               // should already be flagged from last time, if this is ocean fill mode 
                    {
                        M_mapdata_attr(mapdata, row, col) |= MAPDATA_OFF;
                        ct_bad_proj++;
                    }                    
                }    
            }
        }
    }


    // Do linear interpolation sampling of the points in the raster
    // ------------------------------------------------------------

    else
    {
        for (row = row_min; row <= row_max; row++)
        {
            for (col = col_min; col <= col_max; col++)
            {
                // if doing ocean fill only, skip to next map pixel unless this pixel has the OCEAN attribute bit or is not VALID

                if (fparm.ocean_fill)
                {
                    attr = M_mapdata_attr(mapdata, row, col); 
                                    
                    if ( (attr & MAPDATA_VALID) && (!(attr & MAPDATA_OCEAN)) )
                       continue;                 
                }
                 
                rc = (*(mapdata.get_lat_lon_p))(mapdata, lat, lon, (real_T)row, (real_T)col, true); 

                if (rc == 0) 
                {
                    get_raster_interpolate(raster, lat, lon, ht);
       

                    // special ocean-only fill processing -- just update height (depth) value from raster, if valid, set depth and mark valid, if not currently valid

                    if (fparm.ocean_fill)                       
                    {
                        if ( (ht != RASTER_OUT) && (ht != RASTER_EMPTY) )
                        {
                            if (attr & MAPDATA_VALID)
                            {
                                M_mapdata_ht(mapdata, row, col) = ht;       // update ocean depth field only
                                ct_ocean++;
                            }
                            else    // attribute is not valid -- switch to VALID + OCEAN + WATER
                            {
                                M_mapdata_ht(  mapdata, row, col) = ht;     // update ocean depth field
                                attr |= (MAPDATA_OCEAN | MAPDATA_VALID);    // indicate valid 
                                M_mapdata_attr(mapdata, row, col) = attr;   // update attribute byte
                                ct_ocean_valid++;            
                            }
                        }
                         else
                        {
                            ct_ocean_bad++;
                        }
                        continue;    
                    }

                    // normal (unconditional fill processing

                    // set mapdata attr byte based on returned height

                    attr = 0;                        // start with empty attr bytes for mapdata

                    if (ht == RASTER_OUT)            // out-of-bounds of raster?
                        attr = MAPDATA_OFF;          // indicate so in attr bytes
                    else if (ht != RASTER_EMPTY)     // not unfilled (invalid) data byte in raster?
                        attr = MAPDATA_VALID;        // indicate valid point in mapdata

                    M_mapdata_attr(mapdata, row, col) = attr;
                    M_mapdata_ht(  mapdata, row, col) = ht; 

                    if (attr & MAPDATA_OFF)
                    {
                        ct_bad_raster++;
                        bad_lat_hi = M_max(bad_lat_hi, lat); 
                        bad_lat_lo = M_min(bad_lat_lo, lat); 
                        bad_lon_hi = M_max(bad_lon_hi, lon); 
                        bad_lon_lo = M_min(bad_lon_lo, lon); 
                    }
                }
                else   // this x,y point is out of bounds for the map projection
                {
                    if (!fparm.ocean_fill)               // should already be flagged from last time, if this is ocean fill mode 
                    {
                        M_mapdata_attr(mapdata, row, col) |= MAPDATA_OFF; 
                        ct_bad_proj++;
                    }                    
                }  

            }
        }    
    }    
    
    if (parm.thread.display)
        {M_out_lk(L"Thread %3d: ending -- rows=%-5d/%-5d  cols=%-5d/%-5d  bad_proj/raster=%d/%d  ocean=%d/%d  bad_ocean=%d") % thp.thread_n % row_min % row_max % col_min % col_max % ct_bad_proj % ct_bad_raster % ct_ocean % ct_ocean_valid % ct_ocean_bad;}

    if (ct_bad_raster > 0)
    {
        if (raster.autolim)
            {M_out_lk_emsg(L"Thread %3d: missing autolim raster data range: lat=%.6f/%.6f  lon=%.6f/%.6f") % thp.thread_n % bad_lat_lo % bad_lat_hi % bad_lon_lo % bad_lon_hi;} 
        else
            {M_out_lk(L"Thread %3d: missing raster data range: -- lat=%.6f/%.6f  lon=%.6f/%.6f") % thp.thread_n % bad_lat_lo % bad_lat_hi % bad_lon_lo % bad_lon_hi;}  
    }

    return;  
}
M_endf





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       slope_map() 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void slope_map(mapdata_S& mapdata, const slope_map_parm_S& sparm, const mapparm_S& parm) try
{  
    std::thread    thd[THREAD_MAX];
    thpa_S         thpa;                       // thread parms
    int32_t        i;   
  

    if (sparm.display)
    {
        M_out(L"Slope   : land_vert_factor      = %-12.6f" ) % sparm.land_vert_factor; 
        M_out(L"Slope   : ocean_vert_factor     = %-12.6f" ) % sparm.ocean_vert_factor; 
        M_out(L"Slope   : do_land               = %s"      ) % M_bool_cstr(sparm.do_land ); 
        M_out(L"Slope   : do_ocean              = %s"      ) % M_bool_cstr(sparm.do_ocean); 

        M_out(L"Slope   : light_dir (string)    = %s"      ) % (sparm.light_dir == light_dir_E::n     ? L"N"           :
                                                                sparm.light_dir == light_dir_E::ne    ? L"NE"          :
                                                                sparm.light_dir == light_dir_E::e     ? L"E"           :
                                                                sparm.light_dir == light_dir_E::se    ? L"SE"          :
                                                                sparm.light_dir == light_dir_E::s     ? L"S"           :
                                                                sparm.light_dir == light_dir_E::sw    ? L"SW"          :
                                                                sparm.light_dir == light_dir_E::w     ? L"W"           :
                                                                sparm.light_dir == light_dir_E::nw    ? L"NW"          :
                                                                sparm.light_dir == light_dir_E::above ? L"above"       :
                                                                                                        L"??unknown??" 
                                                               );
    }
 
    M_start_timer(c1, t1)
    slice_map_row(mapdata, thpa, parm.thread.n_slope, parm);   // allow up to n threads for slope computations

    M_out(L"Map     : slope calculations starting -- %d threads") % thpa.n_threads;

    for (i = 0; i < thpa.n_threads; i++)
    {
        thd[i] = std::thread( slope_map_thd,    std::ref(mapdata), std::ref((thpa.thp)[i]), std::cref(sparm), std::cref(parm) );
    }
     
    for (i = 0; i < thpa.n_threads; i++)
    {
        thd[i].join();         
    }   
  
    M_out(L"Map     : slope calculations ending -- elapsed time = %.3f seconds") % M_read_timer(c1, t1);

    return;  
}
M_endf

////////////////////////////////////////////////////////////////////////////////////

static void slope_map_thd(mapdata_S& mapdata, thp_S& thp, const slope_map_parm_S& sparm, const mapparm_S& parm) try
{ 
    int32_t  row, col, saved_col;
    int32_t  r1, r2, c1, c2; 
    int32_t  rn, rne, re, rse, rs, rsw, rw, rnw; 
    int32_t  cn, cne, ce, cse, cs, csw, cw, cnw; 

    attr_T   attr, neighbor_attr; 
    real_T   slope, slope0, slope1;
    ht_T     ht1, ht2; 
    real_T   lat1, lat2, lon1, lon2;
    int      rc1, rc2;
    real_T   horiz, vert, saved_horiz;
    bool     fast_path;  
    int32_t  max_loop_pass {0};             // default max loop passes = 1, unless lighting direction is above


    // set up slice limits

    int32_t r0      { thp.e_r0         }; 
    int32_t rm      { thp.e_rm         };
    int32_t c0      { thp.e_c0         };
    int32_t cm      { thp.e_cm         };


    // set up whole map limits

    int32_t row0    { 0                }; 
    int32_t rowmax  { mapdata.rows - 1 }; 
    int32_t col0    { 0                }; 
    int32_t colmax  { mapdata.cols - 1 }; 


    if (sparm.light_dir == light_dir_E::above)
        max_loop_pass = 1;                                // need two inner loop passes, for above lighting
     


    M__(M_out(L"Thread %3d: starting-- rows=%-5d/%-5d  cols=%-5d/%-5d") % thp.thread_n % r0 % rm % c0 % cm;)


    // compute slope at each point in map
    // ----------------------------------

    
    for (row = r0; row <= rm; row++)
    {
        saved_col = -11111;    // invalidate saved fastpath column

        for (col = c0; col <= cm; col++)
        {
            // Skip this point unless valid non-water point or valid ocean point (may have water attribute, too)

            attr = M_mapdata_attr(mapdata, row, col);

            if ( ( (attr & MAPDATA_WATER) && (!(attr & MAPDATA_OCEAN)) ) 
                 ||
                 (!(attr & MAPDATA_VALID))
               )
                continue; 


            // Skip this point is this type of slope is not being added

            if ( (!sparm.do_ocean) && (attr & MAPDATA_OCEAN) ) 
                continue;

            if ( (!sparm.do_land) && (!(attr & MAPDATA_OCEAN)) ) 
                continue;  


            // inner loop of 1 or 2 passes (for light_dir_E:above only)
                         
            for (auto loop_pass = 0; loop_pass <= max_loop_pass; loop_pass++)
            {
                // use input light_dir or adjust it if light_dir is "above"

                light_dir_E light_dir {sparm.light_dir};   

                if (light_dir == light_dir_E::above)
                {
                    if (loop_pass == 0)
                        light_dir = light_dir_E::n;            // use N on 1st pass
                    else
                        light_dir = light_dir_E::w;            // use W on 2nd pass
                }
                          

                // fast path -- NW lighting, not top row, or left column, both points valid  
                // ------------------------------------------------------------------------
            
                if (
                   (sparm.light_dir == light_dir_E::nw)
                   &&
                   (row > row0)
                   &&
                   (col > col0)
                   &&
                   (M_mapdata_is_valid(mapdata, row - 1, col - 1))
                  )
                {
                    r1 = row - 1;
                    c1 = col - 1; 
                    r2 = row;
                    c2  = col;  
                    fast_path = true;
                }
                else
                {
                    fast_path = false;
                
                    // find up to 8 valid neighbors to choose from -- assume this is not a 1x1 map (do this on 1st loop pass onle -- rn, cn, etc. are same on 2nd loop pass)
                    // -------------------------------------------
                

                    if (loop_pass == 0)
                    {
                        rn = rne = re = rse = rs = rsw = rw = rnw = -1;    // initialize all neighbors to -1 first      
                        cn = cne = ce = cse = cs = csw = cw = cnw = -1;
                     
                        if ( (row > row0   )                   ) { rn  =  row - 1;    cn  = col;     }  
                        if ( (row > row0   ) && (col < colmax) ) { rne =  row - 1;    cne = col + 1; } 
                        if (                    (col < colmax) ) { re  =  row;        ce  = col + 1; }
                        if ( (row < rowmax ) && (col < colmax) ) { rse =  row + 1;    cse = col + 1; }
                        if ( (row < rowmax )                   ) { rs  =  row + 1;    cs  = col;     } 
                        if ( (row < rowmax ) && (col > col0  ) ) { rsw =  row + 1;    csw = col - 1; }
                        if (                    (col > col0  ) ) { rw  =  row;        cw  = col - 1; }
                        if ( (row > row0   ) && (col > col0  ) ) { rnw =  row - 1;    cnw = col - 1; }    
                     
                        if  ( (rn  != -1) && (!(M_mapdata_is_valid(mapdata, rn , cn  ))) ) { rn  = -1;          cn  = -1;      }
                        if  ( (rne != -1) && (!(M_mapdata_is_valid(mapdata, rne, cne ))) ) { rne = -1;          cne = -1;      }
                        if  ( (re  != -1) && (!(M_mapdata_is_valid(mapdata, re , ce  ))) ) { re  = -1;          ce  = -1;      }
                        if  ( (rse != -1) && (!(M_mapdata_is_valid(mapdata, rse, cse ))) ) { rse = -1;          cse = -1;      }
                        if  ( (rs  != -1) && (!(M_mapdata_is_valid(mapdata, rs , cs  ))) ) { rs  = -1;          cs  = -1;      }
                        if  ( (rsw != -1) && (!(M_mapdata_is_valid(mapdata, rsw, csw ))) ) { rsw = -1;          csw = -1;      }
                        if  ( (rw  != -1) && (!(M_mapdata_is_valid(mapdata, rw , cw  ))) ) { rw  = -1;          cw  = -1;      }
                        if  ( (rnw != -1) && (!(M_mapdata_is_valid(mapdata, rnw, cnw ))) ) { rnw = -1;          cnw = -1;      }
                    }


                    // pick best neighbor, based on lighting direction (done for all loop passes, since direction will diffferent for each loop pass)
                    // -----------------------------------------------
                 
                    switch (light_dir)
                    {
                        //   light direction from North
                
                        case light_dir_E::n :
                         
                            if      (rn  >= 0) {r1 = rn ; c1 = cn ; r2 = row; c2 = col; }  // N   center
                            else if (rs  >= 0) {r1 = row; c1 = col; r2 = rs;  c2 = cs ; }  // center   S
                            else if (rnw >= 0) {r1 = rnw; c1 = cnw; r2 = row; c2 = col; }  // NW   center
                            else if (rne >= 0) {r1 = rne; c1 = cne; r2 = row; c2 = col; }  // NE   center
                            else if (rsw >= 0) {r1 = row; c1 = col; r2 = rsw; c2 = csw; }  // center  SW
                            else if (rse >= 0) {r1 = row; c1 = col; r2 = rse; c2 = cse; }  // center  SE                     
                            else continue;                                                 // no useful neighbors -- don't do this point
                            break;
                   
                
                        // light direction from Northeast
                
                        case light_dir_E::ne :
                         
                            if      (rne >= 0) {r1 = rne; c1 = cne; r2 = row; c2 = col; }  // NE   center
                            else if (rsw >= 0) {r1 = row; c1 = col; r2 = rsw; c2 = csw; }  // center   SW
                            else if (rn  >= 0) {r1 = rn ; c1 = cn ; r2 = row; c2 = col; }  // N   center
                            else if (rs  >= 0) {r1 = row; c1 = col; r2 = rs;  c2 = cs ; }  // center   S
                            else if (re  >= 0) {r1 = re ; c1 = ce ; r2 = row; c2 = col; }  // E   center
                            else if (rw  >= 0) {r1 = row; c1 = col; r2 = rw;  c2 = cw ; }  // center   W 
                            else continue;                                                 // no useful neighbors -- don't do this point
                            break;
                        
                
                         // light direction from East
                
                       case light_dir_E::e :
                        
                            if      (re  >= 0) {r1 = re ; c1 = ce ; r2 = row; c2 = col; }  // E   center
                            else if (rw  >= 0) {r1 = row; c1 = col; r2 = rw ; c2 = cw ; }  // center   W
                            else if (rne >= 0) {r1 = rne; c1 = cne; r2 = row; c2 = col; }  // NE  center
                            else if (rsw >= 0) {r1 = row; c1 = col; r2 = rsw; c2 = csw; }  // center  SW
                            else if (rse >= 0) {r1 = rse; c1 = cse; r2 = row; c2 = col; }  // SE   center
                            else if (rnw >= 0) {r1 = row; c1 = col; r2 = rnw; c2 = cnw; }  // center  NW 
                            else continue;                                                 // no useful neighbors -- don't do this point
                            break;
                
                
                        // light direction from Southeast
                
                        case light_dir_E::se :
                        
                            if      (rse >= 0) {r1 = rse; c1 = cse; r2 = row; c2 = col; }  // SE   center
                            else if (rnw >= 0) {r1 = row; c1 = col; r2 = rnw; c2 = cnw; }  // center   NW
                            else if (rs  >= 0) {r1 = rn ; c1 = cn ; r2 = row; c2 = col; }  // S   center
                            else if (rn  >= 0) {r1 = row; c1 = col; r2 = rn;  c2 = cn ; }  // center   N
                            else if (re  >= 0) {r1 = re ; c1 = ce ; r2 = row; c2 = col; }  // E   center
                            else if (rw  >= 0) {r1 = row; c1 = col; r2 = rw;  c2 = cw ; }  // center   W 
                            else continue;                                                 // no useful neighbors -- don't do this point
                            break;
                        
                
                        //   light direction from South
                
                        case light_dir_E::s :
                          
                            if      (rs  >= 0) {r1 = rs ; c1 = cs ; r2 = row; c2 = col; }  // S   center
                            else if (rn  >= 0) {r1 = row; c1 = col; r2 = rn ; c2 = cn ; }  // center   N
                            else if (rsw >= 0) {r1 = rsw; c1 = csw; r2 = row; c2 = col; }  // SW   center
                            else if (rse >= 0) {r1 = rse; c1 = cse; r2 = row; c2 = col; }  // SE   center      
                            else if (rnw >= 0) {r1 = row; c1 = col; r2 = rnw; c2 = cnw; }  // center   NW
                            else if (rne >= 0) {r1 = row; c1 = col; r2 = rne; c2 = cne; }  // center   NE
                            else continue;                                                 // no useful neighbors -- don't do this point
                            break;
                
                
                        // light direction from Southwest
                
                        case light_dir_E::sw :
                        
                            if      (rsw >= 0) {r1 = rsw; c1 = csw; r2 = row; c2 = col; }  // SW   center
                            else if (rne >= 0) {r1 = row; c1 = col; r2 = rne; c2 = cne; }  // center   NE
                            else if (rs  >= 0) {r1 = rn ; c1 = cn ; r2 = row; c2 = col; }  // S   center
                            else if (rn  >= 0) {r1 = row; c1 = col; r2 = rn;  c2 = cn ; }  // center   N
                            else if (rw  >= 0) {r1 = rw ; c1 = cw ; r2 = row; c2 = col; }  // W   center
                            else if (re  >= 0) {r1 = row; c1 = col; r2 = re;  c2 = ce ; }  // center   E 
                            else continue;                                                 // no useful neighbors -- don't do this point
                            break;
                
                
                        // light direction from West
                
                        case light_dir_E::w :
                        
                            if      (rw  >= 0) {r1 = rw ; c1 = cw ; r2 = row; c2 = col; }  // W   center
                            else if (re  >= 0) {r1 = row; c1 = col; r2 = re ; c2 = ce ; }  // center   E
                            else if (rnw >= 0) {r1 = rnw; c1 = cnw; r2 = row; c2 = col; }  // NW  center
                            else if (rse >= 0) {r1 = row; c1 = col; r2 = rse; c2 = cse; }  // center  SE
                            else if (rsw >= 0) {r1 = rsw; c1 = csw; r2 = row; c2 = col; }  // SW   center
                            else if (rne >= 0) {r1 = row; c1 = col; r2 = rne; c2 = cne; }  // center  NE 
                            else continue;                                                 // no useful neighbors -- don't do this point
                            break;
                        
                
                        // light direction from Northwest
                
                        case light_dir_E::nw :
                        
                            if      (rnw >= 0) {r1 = rnw; c1 = cnw; r2 = row; c2 = col; }  // NW   center
                            else if (rse >= 0) {r1 = row; c1 = col; r2 = rse; c2 = cse; }  // center   SE
                            else if (rn  >= 0) {r1 = rn ; c1 = cn ; r2 = row; c2 = col; }  // N   center
                            else if (rs  >= 0) {r1 = row; c1 = col; r2 = rs;  c2 = cs ; }  // center   S
                            else if (rw  >= 0) {r1 = rw ; c1 = cw ; r2 = row; c2 = col; }  // W   center
                            else if (re  >= 0) {r1 = row; c1 = col; r2 = re;  c2 = ce ; }  // center   E 
                            else continue;                                                 // no useful neighbors -- don't do this point
                            break;
                
                
                        default : // should not get here 
                            break;
                    }
                }
           

                // get horizontal distance separating r1,c1 and r2,c2 (in meters) -- this distance may differ on each inner loop pass  
                 
                if ( (fast_path = true) && (col - saved_col <  sparm.reuse_horiz_spacing) )
                {
                   horiz = saved_horiz;       // reuse earlier value of horiz, if running fast path, and horiz was computed less than 'n' columns ago        
                }
                else
                {
                    // do horizontal distance computation -- skip this point (by ending inner loop) unless both lat/lon values are valid 
                
                    rc1 = (*(mapdata.get_lat_lon_p))(mapdata, lat1, lon1, (real_T)r1, (real_T)c1, true);
                    rc2 = (*(mapdata.get_lat_lon_p))(mapdata, lat2, lon2, (real_T)r2, (real_T)c2, true);
                
                    if ( (rc1 != 0) || (rc2 != 0) )
                        break;                          // end inner loop, if horizontal distance can't be computed at this point      
                                  
                    horiz = 111325.0 * sqrt( (lat1 - lat2) * (lat1 - lat2) + M_cosd(0.5 * (lat1 + lat2)) * (lon1 - lon2) * (lon1 - lon2) );
                
                    // save away horizontal value, if running on fast path (meaning distance is along NW diagonal between points)
                
                    if (fast_path == true)
                    {
                         saved_horiz = horiz;
                         saved_col   = col; 
                    }
                }
                 

                // get vertical distance of ht2 - ht1 (signed value) -- + value means sloping upwards away from light source (good illumination)
                
                ht1 = M_mapdata_ht(mapdata, r1, c1);
                ht2 = M_mapdata_ht(mapdata, r2, c2);
                
                vert  = (real_T)(ht2 - ht1);
                
                
                // Compute exaggerated slope (for this loop pass), in degrees -- should be from -90 to +90
                
                if (horiz == 0.0)
                {
                    if (vert = 0.0)
                        slope = 0.0;
                    else if (vert > 0.0)
                        slope = 90.0;
                    else
                        slope = -90.0;
                }
                else
                {
                    if (attr & MAPDATA_OCEAN)
                        slope = M_atand( (sparm.ocean_vert_factor * vert) / horiz );       // use ocean exaggeration factor
                    else
                        slope = M_atand( (sparm.land_vert_factor  * vert) / horiz );       // use land  exaggeration factor
                }
       

                // for light_dir_E:above, do pass-dependent slope processing 

                if (sparm.light_dir == light_dir_E::above)
                {
                    if (loop_pass == 0)
                    {
                         slope0 = std::abs(slope);
                    }
                    else     // final slope is max of E-W slope and N-S slope
                    {
                         slope1 = std::abs(slope); 
                         slope  = -std::max(slope0, slope1);
                    }
                }


                // on final inner loop pass, save final slope value in mapdata slope buffer 

                if (loop_pass == max_loop_pass)
                   M_mapdata_slope(mapdata, row, col) = (int8_t)M_round(slope);

                //  if (vert != 0.0)
                //      SHOW10("row=%d  col=%d  r1/r2=%d/%d  c1/c2=%d/%d  vert=%.6f   horiz=%.6f   slope=%.6f   slope=%d", row, col, r1, r2, c1, c2, vert, horiz, slope, (int)M_mapdata_slope(p, row, col))
           
            }  // end of inner loop -- multi-pass only if lighting direction is light_dir_E::above 
        }      // end of column loop
    }          // end of row loop

    if (parm.thread.display)
        {M_out_lk(L"Thread %3d: ending -- rows=%-5d/%-5d  cols=%-5d/%-5d") %  thp.thread_n % r0 % rm % c0 % cm;}
        
    return;  
}
M_endf






//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       graticule_map() 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void graticule_map(mapdata_S& mapdata, const graticule_map_parm_S& gparm, const mapparm_S& parm) try
{     
    int32_t i;
    int32_t row, col, row0, col0;
    real_T  rowf, colf;
    real_T  lat, lon;
    real_T  ll_incr;
    
    real_T lat_interval = M_l_to_lr(gparm.ns_spacing); 
    real_T lon_interval = M_l_to_lr(gparm.ew_spacing);

    int32_t lat_mult_lo, lat_mult_hi;
    int32_t lon_mult_lo, lon_mult_hi;
        
    M_start_timer(c1, t1)

    M_out(L"Map     : Graticule processing starting") ;

    ll_incr = 0.9 / mapdata.max_ppd;          // point to point step size -- slightly finer than max PPD for this map 

    if (gparm.display)
    {
        M_out(L"map    : graticule weight           = %d"    ) % gparm.weight       ;
        M_out(L"map    : graticule ns_spacing       = %d"    ) % gparm.ns_spacing   ;
        M_out(L"map    : graticule ew_spacing       = %d"    ) % gparm.ew_spacing   ;
        M_out(L"map    : graticule reduce_1         = %d"    ) % gparm.reduce_1     ;
        M_out(L"map    : graticule reduce_2         = %d"    ) % gparm.reduce_2     ;
        M_out(L"map    : graticule reduce_3         = %d"    ) % gparm.reduce_3     ;
        M_out(L"map    : graticule hi_lat_1         = %d"    ) % gparm.hi_lat_1     ;
        M_out(L"map    : graticule hi_lat_2         = %d"    ) % gparm.hi_lat_2     ;
        M_out(L"map    : graticule hi_lat_3         = %d"    ) % gparm.hi_lat_3     ;
        M_out(L"map    : graticule lat_interval     = %-.20f") % lat_interval       ;
        M_out(L"map    : graticule lon_interval     = %-.20f") % lon_interval       ;
        M_out(L"map    : graticule ll_incr          = %-.20f") % ll_incr            ;
    }


   ////////////////////////////////////////////////////////////
   //////////////// put in longitude lines -- N-S grid lines
   /////////////////////////////////////////////////////////

   if (lon_interval > 0.0)
   {
       //  Compute longitude line (meridian) multipliers
       //  ---------------------------------------------

       if (mapdata.min_longitude >= 0.0) 
           lon_mult_lo = M_round_away_zero(  mapdata.min_longitude / lon_interval);
       else
           lon_mult_lo = M_round_toward_zero(mapdata.min_longitude / lon_interval);

       if (mapdata.max_longitude >= 0.0)
           lon_mult_hi = M_round_toward_zero(mapdata.max_longitude / lon_interval);
       else
           lon_mult_hi = M_round_away_zero(  mapdata.max_longitude / lon_interval);
       

       //  Outer loop to draw each meridian
       //  --------------------------------

       for (i = lon_mult_lo; i <= lon_mult_hi; i++) 
       {
           lon = ((real_T)i) * lon_interval; 

          
           M__(M_out(L"min-lon=%-12.6f  max-lon=%-12.6f, i=%d  lon=%-12.6f") % mapdata.min_longitude % mapdata.max_longitude % i % lon;)
           M__(M_out(L"min-lat=%-12.6f  max-lat=%-12.6f   incr=%-12.6f"    ) % mapdata.min_latitude  % mapdata.max_latitude  % ll_incr;)  
          

           //  Inner loop to move along longitude for this meridian
           //  ----------------------------------------------------

           for (lat = mapdata.min_latitude; lat <= mapdata.max_latitude; lat += ll_incr)
           {
               //  Do EW gridline reduction near poles, if requested
               //  -------------------------------------------------

               if      ( (lat >= (real_T)(gparm.hi_lat_3)) || (lat <= (real_T)(-gparm.hi_lat_3)) )
               {
                   if ( (i % gparm.reduce_3) != 0 ) continue;
               }
               else if ( (lat >= (real_T)(gparm.hi_lat_2)) || (lat <= (real_T)(-gparm.hi_lat_2)) )
               {
                   if ( (i % gparm.reduce_2) != 0 ) continue;
               }
               else if ( (lat >= (real_T)(gparm.hi_lat_1)) || (lat <= (real_T)(-gparm.hi_lat_1)) )
               {
                   if ( (i % gparm.reduce_1) != 0 ) continue;
               }


               //  Put in one (thick) point on graticule line
               //  ------------------------------------------

               auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, true);   // get x/y location for center point
               
               if (rc_rc == M_ROW_COL_RC_OK) 
               {
                   row0 = M_round(rowf - ((real_T)(gparm.weight)) / 2.0); 
                   col0 = M_round(colf - ((real_T)(gparm.weight)) / 2.0); 

                   M__(M_out(L"lat=%-12.6f  lon=%-12.6f, row0=%d, col0=%d") % lat % lon % row0 % col0; )

            
                   // add in graticule box with dimensions = weight x weight -- could make this a solid circle

                   for (row = row0; row < row0 + gparm.weight; row++)
                   {
                       for (col = col0; col < col0 + gparm.weight; col++)
                       {
                           if ( (row >= 0) && (row < mapdata.rows) && (col >= 0) && (col < mapdata.cols) )  // make sure not to go out of bounds
                               M_mapdata_attr(mapdata, row, col) |= MAPDATA_GRATICULE;                       
                       }                  
                   }
               }
           }
       }
   }      


   /////////////////////////////////////////////////////////////
   //////////////// put in latitude lines -- E-W graticule lines
   /////////////////////////////////////////////////////////////

   if (lat_interval > 0.0)
   {
       //  Compute latitude line (parallel) multipliers
       //  --------------------------------------------

       if (mapdata.min_latitude >= 0.0) 
           lat_mult_lo = M_round_away_zero(  mapdata.min_latitude / lat_interval);
       else
           lat_mult_lo = M_round_toward_zero(mapdata.min_latitude / lat_interval);

       if (mapdata.max_latitude >= 0.0)
           lat_mult_hi = M_round_toward_zero(mapdata.max_latitude / lat_interval);
       else
           lat_mult_hi = M_round_away_zero(  mapdata.max_latitude / lat_interval);


       //  Outer loop to draw each parallel
       //  --------------------------------
       
       for (i = lat_mult_lo; i <= lat_mult_hi; i++) 
       {
           lat = ((real_T)i) * lat_interval; 

           
           M__(M_out(L"min-lat=%-12.6f  max-lat=%-12.6f   i=%d  lat=%-12.6f" ) % mapdata.min_latitude  % mapdata.max_latitude  % i % lat;)
           M__(M_out(L"min-lon=%-12.6f  max-lon=%-12.6f   incr=%-12.6f"      ) % mapdata.min_longitude % mapdata.max_longitude % ll_incr;)
           

           //  Inner loop to move along latitude for this parallel
           //  ---------------------------------------------------

           for (lon = mapdata.min_longitude; lon <= mapdata.max_longitude; lon += ll_incr)
           {
               //  Put in one (thick) point on graticule line
               //  ------------------------------------------

               auto rc_rc = (*(mapdata.get_row_col_p))(mapdata, rowf, colf, lat, lon, true);
               
               if (rc_rc == M_ROW_COL_RC_OK) 
               {
                   row0 = M_round(rowf - (real_T)(gparm.weight) / 2.0); 
                   col0 = M_round(colf - (real_T)(gparm.weight) / 2.0); 

                   M__(M_out(L"lat=%-12.6f  lon=%-12.6f, row0=%d, col0=%d") % lat % lon % row0 % col0;)

            
                   //  Add in graticule box with dimensions = weight x weight -- could make this a solid circle

                   for (row = row0; row < row0 + gparm.weight; row++)
                   {
                       for (col = col0; col < col0 + gparm.weight; col++)
                       {
                           if ( (row >= 0) && (row < mapdata.rows) && (col >= 0) && (col < mapdata.cols) )     // make sure not to go out of bounds
                               M_mapdata_attr(mapdata, row, col) |= MAPDATA_GRATICULE;                       
                       }                  
                   }
               }
           }
       }
   }   
   
   M_out(L"Map     : Graticule processing ending -- elapsed time = %.3f seconds") % M_read_timer(c1, t1);

   return; 
}
M_endf

//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""