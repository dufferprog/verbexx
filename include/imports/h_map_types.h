// h_map_types.h

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ============
////            h_map_types.h -- common map-oriented types and equates needed in many places 
////            ============
//// 
////    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


#define THREAD_MAX 250   // max number of threads to kick off
#define MAX_SLICES 5000  // maximum number of slices in thpa array -- must be more than THREAD_MAX 


// special heights in the raster data -- all other heights are valid elevations (hopefully)
// ----------------------------------------------------------------------------------------

#define RASTER_EMPTY -17477   // 0xBBBB -- unfilled data point in raster
#define RASTER_OUT   -17478   //        -- requested point is out of bounds of raster
#define RASTER_VOID  -32768   // directly from .hgt files -- void in .hgt data


// types specific to maps 
// ----------------------

typedef int32_t    ll_T;          // type for integer latitude and longitude (in small units like arc-sec, etc.)
typedef int32_t    lld_T;         // type for integer latitude and longitude in degrees
typedef real_T     llr_T;         // type for floating pt (real_T) latitude and longitude in degrees

typedef int16_t    ht_T;          // type for heights in mapdata, or elsewhere (little-endian -- not in .hgt files or hgt structure)
typedef uint16_t   attr_T;        // type for attributes in mapdata, or elsewhere  
typedef int8_t     slope_T;       // computed (exaggerated) slope value (in degrees -- -90 to +90)  
 

// Bits in the mapdata attribute flags (attr_T)
// --------------------------------------------

#define MAPDATA_OFF        0x8000    // out of bounds -- not in raster or out of bounds for projection
#define MAPDATA_VALID      0x4000    // valid RASTER/MAP data
#define MAPDATA_WATER      0x2000    // water (lake/river or ocean)
#define MAPDATA_GRATICULE  0x1000    // graticule line
#define MAPDATA_OCEAN      0x0800    // ocean (water should be on, too)
#define MAPDATA_TEMP       0x0001    // temporary flag bit for processing routines


// special height values (ht_T)
// ----------------------------

#define MAPDATA_EMPTY -17477   // 0xBBBB -- unfilled data point in raster
#define MAPDATA_OUT   -17478   //        -- requested point is out of bounds of raster
#define MAPDATA_VOID  -32768   // directly from .hgt files, by way of raster -- void in .hgt data


// map projection enum
// -------------------

typedef enum class projection_E { rectangular, mercator, polar, stereographic, orthographic, azimuthal, azimuthal_ea} projection_T;


// -----------------------------------------------------------------------
// forward type declarations (incomplete structure/union/class/enum types)
// -----------------------------------------------------------------------

struct                       create_raster_parm_S;
struct                       demfile_parm_S;            // ??????
struct                       fileinfo_S;
struct                       fill_raster_parm_S;
struct                       writeout_raster_parm_S;
struct                       graticule_file_parm_S;
struct                       grid_S;
struct                       hgt_S;
struct                       outimage_parm_S;          // ????????
struct                       outimage_S;
struct                       map_parm_S;
struct                       mapdata_S;
struct                       output_parm_S;
struct                       mapparm_S;
union                        project_U; 
struct                       ramp_S;
struct                       ramp_parm_S;               // ??????
struct                       raster_S;
struct                       raster_parm_S;             // ???
struct                       rasterfile_parm_S;         // ???
class                        shapefile_C;
struct                       shp_hdr_S;
struct                       slice_parm_S;
struct                       slope_parm_S;
struct                       thpa_S; 
struct                       thread_parm_S;
struct                       tile_S;
struct                       tile_parm_S;               // ?????
struct                       water_parm_S;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////