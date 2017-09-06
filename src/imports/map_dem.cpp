// map_dem.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===========
////     map_dem.cpp
////     ===========
//// 
//// 
////     contains functions related to the hgt_t structure and other DEM items, DEM file sets, etc.                 
//// 
////
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

#define M_IN_MAP_DLL

#include "h__common.h"

#include "h_map_types.h"
#include "h_map_global.h"
#include "h_map_parms.h" 
#include "h_map_dem.h"
#include "h_map_raster.h"



// ==================================================================================================================
//                                       Static variables
// ==================================================================================================================

namespace static_N
{

// fileinfo_S repository
// ---------------------

static std::map<int64_t, std::shared_ptr<fileinfo_S>> fileinfos { }; 

// ------------------------------------------------------------------- 

}




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        find_fileinfo() -- find fileinfo(n) in repository (return nullptr, if not there)  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

fileinfo_S* find_fileinfo(int64_t fileinfo_id) try
{
    // make sure there is exactly one fileinfo in repository with caller's ID
    // ----------------------------------------------------------------------

     auto fileinfo_ct = static_N::fileinfos.count(fileinfo_id); 

     if (fileinfo_ct <= 0)
     {
         M__(M_out(L"find_fileinfo() -- fileinfo(%d) not in repository") % fileinfo_id;) 
         return nullptr; 
     }
     else
     if (fileinfo_ct > 1)
     {
         M_out_emsg(L"find_fileinfo() -- invalid fileinfos.count(%d) = %d") % fileinfo_id % fileinfo_ct;  
         return nullptr; 
     }


     // one shared_ptr<fileinfo_S> found -- return fileinfo_S address as simple pointer
     // ---------------------------------------------------------------------------------

     M__(M_out(L"find_fileinfo() -- fileinfo(%d) at %p found in repository") % fileinfo_id % static_N::fileinfos.at(fileinfo_id).get();) 
     return static_N::fileinfos.at(fileinfo_id).get();
}
M_endf 
 


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        alloc_fileinfo() -- make sure fileinfo is allocated in repository for this ID  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

fileinfo_S* alloc_fileinfo(int64_t fileinfo_id) try
{
    // pass back any existing fileinfo_S in repository
    // ------------------------------------------------

    fileinfo_S *p = find_fileinfo(fileinfo_id);

    if (p != nullptr)
    {
        M__(M_out(L"alloc_fileinfo() -- existing fileinfo(%d) in repository at %p being returned") % fileinfo_id % p;) 
        return p;
    }


    // need to allocate new (default initialized) fileinfo_S and add to the repository
    //---------------------------------------------------------------------------------

    std::shared_ptr<fileinfo_S> sp(new fileinfo_S);     // this sp should go away when function returns
    static_N::fileinfos.emplace(fileinfo_id, sp);       // this sp should stay around until this ID is deallocated

    M__(M_out(L"alloc_fileinfo() -- new fileinfo(%d) was allocated at %p and added to repository") % fileinfo_id % sp.get();)


    // pass back simple address of newly-allocated fileinfo_S

    return sp.get();                                        
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||        free_fileinfo() -- make sure no fileinfo is allocated in repository for this ID  
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void free_fileinfo(int64_t fileinfo_id) try
{
    // get rid of any existing fileinfo_S in repository
    // ----------------------------------------------

    if (static_N::fileinfos.count(fileinfo_id) > 0)
    {
        M__(M_out(L"free_fileinfo() -- fileinfo(%d) being erased from repository") % fileinfo_id;)
        static_N::fileinfos.erase(fileinfo_id);   
    }
    else
    {
        M__(M_out(L"free_fileinfo() -- fileinfo(%d) not allocated in repository") % fileinfo_id;)
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
//||       display_fileinfo() -- display fields in fileinfo_S structure
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_fileinfo(const fileinfo_S& fileinfo) try
{                                                 
    M_out(L"fileinfo.origin_ns         = %-7d (%21.16f deg)" )     % fileinfo.origin_ns       % M_l_to_lr(fileinfo.origin_ns)            ;
    M_out(L"fileinfo.origin_ew         = %-7d (%21.16f deg)" )     % fileinfo.origin_ew       % M_l_to_lr(fileinfo.origin_ew)            ;
    M_out(L"fileinfo.step_ns           = %-7d (%21.16f deg)" )     % fileinfo.step_ns         % M_l_to_lr(fileinfo.step_ns  )            ;
    M_out(L"fileinfo.step_ew           = %-7d (%21.16f deg)" )     % fileinfo.step_ew         % M_l_to_lr(fileinfo.step_ew  )            ;
    M_out(L"fileinfo.limit_s           = %-7d (%21.16f deg)" )     % fileinfo.limit_s         % M_l_to_lr(fileinfo.limit_s  )            ;
    M_out(L"fileinfo.limit_w           = %-7d (%21.16f deg)" )     % fileinfo.limit_w         % M_l_to_lr(fileinfo.limit_w  )            ;
    M_out(L"fileinfo.limit_n           = %-7d (%21.16f deg)" )     % fileinfo.limit_n         % M_l_to_lr(fileinfo.limit_n  )            ;
    M_out(L"fileinfo.limit_e           = %-7d (%21.16f deg)" )     % fileinfo.limit_e         % M_l_to_lr(fileinfo.limit_e  )            ;
    M_out(L"fileinfo.number_ns         = %d"                 )     % fileinfo.number_ns                                                  ;
    M_out(L"fileinfo.number_ew         = %d"                 )     % fileinfo.number_ew                                                  ;
                                                                                                                                        
    M_out(L"fileinfo.fn_sw_corner      = %s"                 )     % M_bool_cstr(fileinfo.fn_sw_corner     )                             ;
    M_out(L"fileinfo.fn_nw_corner      = %s"                 )     % M_bool_cstr(fileinfo.fn_nw_corner     )                             ;
    M_out(L"fileinfo.fn_upper_case     = %s"                 )     % M_bool_cstr(fileinfo.fn_upper_case    )                             ;
    M_out(L"fileinfo.fn_lower_case     = %s"                 )     % M_bool_cstr(fileinfo.fn_lower_case    )                             ;
    M_out(L"fileinfo.fn_lat_then_lon   = %s"                 )     % M_bool_cstr(fileinfo.fn_lat_then_lon  )                             ;
    M_out(L"fileinfo.fn_lon_then_lat   = %s"                 )     % M_bool_cstr(fileinfo.fn_lon_then_lat  )                             ;
    M_out(L"fileinfo.fn_global         = %s"                 )     % M_bool_cstr(fileinfo.fn_global        )                             ;
    M_out(L"fileinfo.fn_many           = %s"                 )     % M_bool_cstr(fileinfo.fn_many          )                             ;
    M_out(L"fileinfo.fixed_suffix      = %s"                 )     % M_bool_cstr(fileinfo.fixed_suffix     )                             ;
    M_out(L"fileinfo.varying_suffix    = %s"                 )     % M_bool_cstr(fileinfo.varying_suffix   )                             ;
    M_out(L"fileinfo.multi_ft          = %s"                 )     % M_bool_cstr(fileinfo.multi_ft         )                             ;
    M_out(L"fileinfo.single_ft         = %s"                 )     % M_bool_cstr(fileinfo.single_ft        )                             ;
                                                                                                                 
    M_out(L"fileinfo.folder            = \"%s\""             )     % fileinfo.folder.c_str()                                             ;
    M_out(L"fileinfo.prefix            = \"%s\""             )     % fileinfo.prefix.c_str()                                             ;
    M_out(L"fileinfo.separator         = \"%s\""             )     % fileinfo.separator.c_str()                                          ;
    

    // display 0, 1, n suffixes
    // ------------------------

    M_out(L"fileinfo.suffix_ct         = %d"                 )     % fileinfo.suffix_ct                                                  ;

    if (fileinfo.suffix_ct <= 0)
    {
        M_out(
          L"fileinfo.suffixes[]        = \"\""               )                                                                           ;      
    }
    else
    {
        auto i {0}; 
        for (auto const &suffix : fileinfo.suffixes)
        {
            M_out(
          L"fileinfo.suffixes[%d]      = \"%s\""             )     % (i++) % suffix                                                      ; 
        }
    
    }
    
    // ----------------------------------------------
    
    M_out(L"fileinfo.filetype          = \"%s\""             )     % fileinfo.filetype.c_str()                                           ;
    
    M_out(L"fileinfo.raster_data       = %s"                 )     % M_bool_cstr(fileinfo.raster_data      )                             ;
    M_out(L"fileinfo.hgt_layout        = %s"                 )     % M_bool_cstr(fileinfo.hgt_layout       )                             ;
    M_out(L"fileinfo.simple_dt2_layout = %s"                 )     % M_bool_cstr(fileinfo.simple_dt2_layout)                             ;
    M_out(L"fileinfo.file_min_size     = %d"                 )     % fileinfo.file_min_size                                              ;
    M_out(L"fileinfo.file_max_size     = %d"                 )     % fileinfo.file_max_size                                              ;

    M_out(L"fileinfo.float_32          = %s"                 )     % M_bool_cstr(fileinfo.float_32         )                             ;
    M_out(L"fileinfo.int_16            = %s"                 )     % M_bool_cstr(fileinfo.int_16           )                             ;
    M_out(L"fileinfo.big_endian        = %s"                 )     % M_bool_cstr(fileinfo.big_endian       )                             ;
    M_out(L"fileinfo.little_endian     = %s"                 )     % M_bool_cstr(fileinfo.little_endian    )                             ;
    M_out(L"fileinfo.signed_magnitude  = %s"                 )     % M_bool_cstr(fileinfo.signed_magnitude )                             ;
    M_out(L"fileinfo.twos_compliment   = %s"                 )     % M_bool_cstr(fileinfo.twos_compliment  )                             ;

    M_out(L"fileinfo.pixels_aligned    = %s"                 )     % M_bool_cstr(fileinfo.pixels_aligned   )                             ;
    M_out(L"fileinfo.pixels_centered   = %s"                 )     % M_bool_cstr(fileinfo.pixels_centered  )                             ;
    M_out(L"fileinfo.extra_row_col     = %s"                 )     % M_bool_cstr(fileinfo.extra_row_col    )                             ;
    M_out(L"fileinfo.no_extra_row_col  = %s"                 )     % M_bool_cstr(fileinfo.no_extra_row_col )                             ;
    M_out(L"fileinfo.extent_ns         = %d"                 )     % fileinfo.extent_ns                                                  ;
    M_out(L"fileinfo.extent_ew         = %d"                 )     % fileinfo.extent_ew                                                  ;
    M_out(L"fileinfo.ppd_ns            = %d"                 )     % fileinfo.ppd_ns                                                     ;
    M_out(L"fileinfo.ppd_ew            = %d"                 )     % fileinfo.ppd_ew                                                     ;
                                          
    return;
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       readin_file() -- input is complete filename, min max size, buffer 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// --------------------------------
// version with passed-in databuf_C
// --------------------------------

int readin_file(const std::wstring& filename, databuf_C& dbuf, uint64_t min_ct, uint64_t max_ct) try
{
     dbuf.ct = 0;                              // make sure data count is not set from last timr (in case of error)
     dbuf.realloc1(max_ct + 1);                // make sure buffer is long enough

     return readin_file(filename, (void *)(dbuf.p1), dbuf.sz1, min_ct, max_ct, dbuf.ct);
}
M_endf


// ------------------------------
// version with passed-in pointer
// ------------------------------

int readin_file(const std::wstring& filename, void *out_p, uint64_t out_l, uint64_t min_ct, uint64_t max_ct, uint64_t& read_ct) try
{
    read_ct = 0;                    // clear out, in case of error


    // make sure buffer is longer than max length expected
    // ---------------------------------------------------

    if (min_ct > max_ct)
    {
        M_out_emsg(L"readin_file(): min_ct (%s) larger than max_ct (%s) -- filename = \"%s\"") % M_sep(min_ct) % M_sep(max_ct) % filename; 
        M_throw("readin_file(): min_ct larger than max_ct") 
        return -2;     // should not get here
    } 

    if (out_l <= max_ct)
    {
        M_out_emsg(L"readin_file(): buffer length (%s) not larger than max_ct (%s) -- filename = \"%s\"") % M_sep(out_l) % M_sep(max_ct) % filename; 
        M_throw("readin_file(): buffer length not larger than than max_ct") 
        return -2;    // should not get here
    }
   

    // Open input file -- return with error if open fails (but no error message) 
    // --------------------------------------------------
         
    std::ifstream ifile(filename, std::ios::in | std::ios::binary);

    if ( !(ifile.good()) )
    {
        M__(M_out(L"readin_filename(): Can't open() input file \"%s\"") % filename;)
        return -1;
    }


    // read in file and check required lengths
    // ---------------------------------------  
    
    ifile.read((char *)out_p, (size_t)out_l);     // fill in output buffer  
    read_ct = (uint64_t)(ifile.gcount());         // fill in output read count field
  
    M__(M_out(L"readin_file(): read_ct = %d  filename=\"%s\"") % read_ct % filename;) 

    ifile.close();

    if ( (read_ct < min_ct) || (read_ct > max_ct) )    
    {
        M_out_emsg(L"readin_file(): read ct (%s) not in expected range = %s to %s bytes -- filename = \"%s\"") % M_sep(read_ct) % M_sep(min_ct) % M_sep(max_ct) % filename; 
        M_throw("readin_hgt(): readin_hgt(): read ct not in expected range") 
        return -2;     // should not get here
    }


    // no problems -- return normally
    // ------------------------------

    return 0;
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       readin_hgt()  -- input south/west in arc-sec -- extent_ns, ew provided to make sure whole window extent is present in the file 
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int readin_hgt(hgt_S& hgt, ll_T south, ll_T west, ll_T extent_ns, ll_T extent_ew, const fill_raster_parm_S& fparm, const fileinfo_S& fileinfo) try
{
    std::streamsize      rd_ct; 
    
    M__(M_out(L"readin_hgt():  south=%d (%.16f deg)  west=%d (%.16f deg)  ns=%d (%.16f deg)  ew=%d (%.16f deg)") % south % M_l_to_lr(south) % west % M_l_to_lr(west) % extent_ns % M_l_to_lr(extent_ns) % extent_ew % M_l_to_lr(extent_ew);)


    // get filename for caller's lat/lon, and see if already in .hgt buffer
    // --------------------------------------------------------------------
      
     tile_filename(hgt.tile, south, west, extent_ns, extent_ew, fileinfo, fparm.debug_tile);  

     M__(M_out(L"readin_hgt(): hgt.file = \"%s\"     hgt.tile.filename = \"%s\"") % hgt.file %  hgt.tile.filename;)

     if (hgt.file == hgt.tile.filename)
     {
         M__(M_out(L"readin_hgt(): Suppressing read of file \"%s\" (already in .hgt buffer)") % hgt.tile.filename;) 
         return 1;     // indicating read-suppressed -- don't need to update/reuse .hgt grid/buffer, etc.
     }
     

     // will need to read in new .hgt file -- clear out old info -- reposition hgt.grid to align with located file   
     // (note: tile_filename() has set the tile.curr_south and tile.curr_west fields to align with the tile boundaries for the file with tile.filename) 
     // (note: extent_ns, ew aren't involved here, since it the file coverage area can exceed these extents)

     reuse_hgt(hgt,  hgt.tile.curr_south, hgt.tile.curr_west);

  
    // Open hgt file -- return with error if open fails 
    // ------------------------------------------------
         
    std::ifstream hfile(hgt.tile.filename, std::ios::in | std::ios::binary);

    if ( !(hfile.good()) )
    {
        if (fparm.display_filenames)
           M_out(L"readin_hgt(): input .hgt filename = \"%s\" (not found)") % hgt.tile.filename;  

        M__(M_out(L"readin_hgt(): Can't open() input .hgt file \"%s\" for %d %d") % hgt.tile.filename % south % west;)
        return -1;
    }

    if (fparm.display_filenames)
        M_out(L"readin_hgt(): input .hgt filename = \"%s\" found") % hgt.tile.filename;  
    

    // Read in file, then close, and check input length (note: no check for file longer than expected)
    // ------------------------------------------------ 

    M__(M_out(L"readin_hgt(): Input .hgt pathname = \"%s\"  hgt.buf.sz1 = %s") % hgt.tile.filename % M_sep(hgt.buf.sz1);)

    hfile.read((char *)(hgt.buf.p), (std::streamsize)(hgt.buf.sz1));

    rd_ct = hfile.gcount();
  
    M__(M_out(L"readin_hgt(): rd_ct(gcount)=%d") % (int)rd_ct;) 

    hfile.close();

  //  if (rd_ct < hgt.buf.sz1)    // ??????????????????????????????????????????????
  //  {
   //     M_out_emsg(L"readin_hgt(): readin_hgt(): read ct less than expected -- filename = \"%s\"   read ct = %s  expecting = %s") % hgt.tile.filename % M_sep(rd_ct) % M_sep(hgt.buf.sz1); 
    //    M_throw("readin_hgt(): readin_hgt(): read ct less than expected") 
     //   return -1;     // should not get here
 //   }


    // good -- set read count in hgt structure and return good status (save name of file currently in buffer)
    // --------------------------------------------------------------

    hgt.file = hgt.tile.filename; 
    hgt.ct = (uint64_t)rd_ct; 
    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       writeout_hgt()  -- input lat/lon in arc-sec
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int writeout_hgt(hgt_S& hgt, const writeout_raster_parm_S& wparm, const fileinfo_S& fileinfo) try
{
    // compose complete filename string first
    // --------------------------------------

    tile_filename(hgt.tile, hgt.grid.south, hgt.grid.west, hgt.grid.extent_ns, hgt.grid.extent_ew, fileinfo, wparm.debug_tile);   


    // Make sure hgt buffer is completely full  
    // ---------------------------------------

    if (hgt.ct != hgt.buf.sz1 - sizeof (ht_T))        // compare bytes of points filled with fillable portion of buffer
    {    
        M_out(L"writeout_hgt(): hgt buffer not full buf.sz1=%s  count=%s -- output .hgt file \"%s\" for lat=%d=%.6f  lon=%d=%.6f") 
              % M_sep(hgt.buf.sz1) % M_sep(hgt.ct) % hgt.tile.filename % hgt.grid.south % M_l_to_lr(hgt.grid.south) % hgt.grid.west % M_l_to_lr(hgt.grid.west);
        return -1;
    }


    // Open hgt file (with create/truncate option) -- return with error if open fails 
    // ------------------------------------------------------------------------------
      
    std::ofstream hfile(hgt.tile.filename, std::ios::out | std::ios::trunc | std::ios::binary);

    if ( !(hfile.good()) )
    {
        M_out_emsg(L"writeout_hgt(): Can't open() output .hgt file \"%s\" -- fail()=%d eof()=%d bad()=%d  -- lat=%d=%.6f  lon=%d=%.6f") 
                  % hgt.tile.filename % hfile.fail() % hfile.eof() % hfile.bad() % hgt.grid.south % M_l_to_lr(hgt.grid.south) % hgt.grid.west % M_l_to_lr(hgt.grid.west);; 

        return -1;
    }
    

    // Write out file, then close, and check for errors -- write count is from the hgt buffer  
    // -------------------------------------------------------------------------------------- 

    M_out(L"writeout_hgt(): Output .hgt pathname = \"%s\"   bytes=%u") % hgt.tile.filename % hgt.ct;   

    hfile.write((char *)(hgt.buf.p), (std::streamsize)(hgt.ct)); 

    if ( !(hfile.good()) )
    {
        hfile.close();
        M_out_emsg(L"writeout_hgt(): Can't write() %u bytes to  to output .hgt file \"%s\" for lat=%d=%.6f  lon=%d=%.6f") 
                  % hgt.ct % hgt.tile.filename % hgt.grid.south % M_l_to_lr(hgt.grid.south) % hgt.grid.west % M_l_to_lr(hgt.grid.west); 
        return -1;
    }     

    hfile.close();

    if ( !(hfile.good()) )
    {
        M_out_emsg(L"writeout_hgt(): Can't close() output .hgt file \"%s\" for lat=%d=%.6f  lon=%d=%.6f") 
                  % hgt.tile.filename % hgt.grid.south % M_l_to_lr(hgt.grid.south) % hgt.grid.west % M_l_to_lr(hgt.grid.west);
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
//||       show_hgt_fields()
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void show_hgt_fields(const hgt_S& hgt) try
{
    display_tile(hgt.tile);
    display_grid(hgt.grid);

    M_out(L"hgt.buf.sz           = %s = %.3fM"     )                     % M_sep(hgt.buf.sz)   % M_meg(hgt.buf.sz)      ;
    M_out(L"hgt.buf.sz1          = %s = %.3fMB"    )                     % M_sep(hgt.buf.sz1)  % M_meg(hgt.buf.sz1)     ;
    M_out(L"hgt.ct               = %s"             )                     % M_sep(hgt.ct)                                ;
    M_out(L"hgt.little_end       = %s"             )                     % M_bool_cstr(hgt.little_end)                  ;
    M_out(L"hgt.buf.p            = %p"             )                     % hgt.buf.p                                    ; 
    M_out(L"hgt.buf.p1           = %p"             )                     % (void *)(hgt.buf.p1)                         ;  

    return; 
}
M_endf


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       create_hgt()  -- imbedded grid set to SW corner tile of filegroup
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void create_hgt(hgt_S& hgt, const fileinfo_S& fileinfo, bool display) try       
{
   // make sure tile in hgt_S is reset to initial state before starting

   reset_tile(hgt.tile); 


   // fill in grid for a single DEM file/tile, based on south/west corner of filegroup coverage area (all fields except ones based current lat/lon should stay same) 

   M__(M_out(L"create_hgt() -- extent_ns=%d   extent_ew=%d   ppd_ns=%d   ppd_ew=%d <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<") % fileinfo.extent_ns % fileinfo.extent_ew % fileinfo.ppd_ns % fileinfo.ppd_ew;)

   init_grid( hgt.grid
            , fileinfo.origin_ns        // start off in southwest corner of filegroup coverage area (may not be needed in raster)
            , fileinfo.origin_ew        // start off in southwest corner of filegroup coverage area (may not be needed in raster)
            , fileinfo.extent_ns        // extent of one file/tile in filegroup
            , fileinfo.extent_ew        // extent of one file/tile in filegroup
            , fileinfo.ppd_ns
            , fileinfo.ppd_ew
            , fileinfo.extra_row_col
            , fileinfo.pixels_aligned
            );  


   // Set little-endian/big-endian flag based on file parms

   hgt.little_end = fileinfo.little_endian;


   // Allocate buffer large enought to hold npts * ht_T items  + 1 (for read length verification)

   hgt.buf.alloc(1 + hgt.grid.npts);   // .npts already filled in (computed) by init_grid(), above


   // mark buffer as empty (not containing data from any file in filegroup)

   hgt.ct   = 0; 
   hgt.file = L"";        // clear out name of file currently in buffer  
    

   // display filled-in hgt, if required

   if (display)
       show_hgt_fields(hgt);
    
    return;   
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       destroy_hgt()
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void destroy_hgt(hgt_S& hgt) try
{
    hgt.ct = 0; 
    hgt.buf.free();

    hgt.file.clear();
    reset_tile(hgt.tile);
  
    return; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       reuse_hgt() -- reuse existing hgt_t structure -- just update latitude, longitude -- keep same buffer  size
//||                      set data count to 0
//||
//||
//||       this routine assumes south and west parms don't need adjusting to align with .hgt file tile boundaries 
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void reuse_hgt(hgt_S& hgt, ll_T south, ll_T west) try
{     
   // fill in grid based on input south/west edge (should be aligned with tile boundary for possible incoming file)

   reposition_grid(hgt.grid, south, west);
   

   // clear out .hgt data count and file, before reading in new data -- current data is not what is wanted in .hgt

   hgt.ct = 0;   
   hgt.file.clear();  

   return;
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_elev_ble() -- from big-endian or little-endian .hgt buffer
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t  get_elev_ble(const hgt_S& hgt, rc_T row, rc_T col) try
{
    int16_U u;              // area for reversing bytes in little-endian 16-bit integer

    M__(M_out(L"get_elev_ble() row/col = %d/%d") % row % col;)


    if (hgt.little_end)
    {
       u.int16 = M_hgt_ht2(hgt, row, col);                           // little-endian -- lo-byte + hi-byte in correct order     
    }
    else
    {
       // bytes in wrong order -- need to be copied individually
   
       u.uint8.hi = M_hgt_ht0(hgt, row, col);                        // big-endian -- hi byte first 
       u.uint8.lo = M_hgt_ht1(hgt, row, col);                        // big-endian -- lo byte second 
    }

    return (int32_t)(u.int16); 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_elev_ble_nearest() -- from big-endian or little-endian .hgt buffer
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t  get_elev_ble_nearest(const hgt_S& hgt, rcf_T row_f, rcf_T col_f) try
{
    int16_U  u;              // area for reversing bytes in little-endian 16-bit integer
    rc_T     row, col;
    
    M__(M_out(L"get_elev_ble_nearest() row/col = %.6f/%.6f") % row_f % col_f;)

    row = M_round(row_f); 
    col = M_round(col_f); 


    if (hgt.little_end)
    {
       u.int16 = M_hgt_ht2(hgt, row, col);                         // little-endian -- lo-byte + hi-byte in correct order       
    }
    else
    {
       // bytes in wrong order -- need to be copied individually
   
       u.uint8.hi = M_hgt_ht0(hgt, row, col);                      // big-endian -- hi byte first 
       u.uint8.lo = M_hgt_ht1(hgt, row, col);                      // big-endian -- lo byte second 
    }

    return (int32_t)(u.int16); 
}
M_endf




//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_elev_ble_interpolate() -- from big-endian or little-endian .hgt buffer
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t  get_elev_ble_interpolate(const hgt_S& hgt, real_T row_f, real_T col_f) try
{
    int16_U  u;              // area for reversing bytes in little-endian 16-bit integer

    rc_T     row  {0};
    rc_T     col  {0};

    rc_T     r1   {0};
    rc_T     r2   {0};
    rc_T     c1   {0};
    rc_T     c2   {0};

    int32_t  h, h11, h12, h21, h22; 

    real_T   f11 {0.0};
    real_T   f12 {0.0};
    real_T   f21 {0.0};
    real_T   f22 {0.0};       

    M__(M_out(L"get_elev_ble_interpolate() row/col = %.6f/%.6f") % row_f % col_f;)    

    get_neighbor_row_col(hgt.grid, row_f, col_f, row, col, r1, r2, c1, c2);     // sets row, col, c1, c2, r1, r2 


    // check nearest neighbor point  (row, col)  for void or empty -- no interpolation if so

    if (hgt.little_end)
    {
       u.int16 = M_hgt_ht2(hgt, row, col);                           // little-endian -- lo-byte + hi-byte in correct order       
    }
    else
    {
       // bytes in wrong order -- need to be copied individually
   
       u.uint8.hi = M_hgt_ht0(hgt, row, col);                        // big-endian -- hi byte first 
       u.uint8.lo = M_hgt_ht1(hgt, row, col);                        // big-endian -- lo byte second 
    }

    h = (int32_t)(u.int16);                                          // interim nearest neighbor height 


    // bypass interpolation, if nearest point is abnormal

    if ( (h == RASTER_VOID) || (h == RASTER_EMPTY) )  
    {
       return h;   
    }


    // get 1/2/4 heights for interpolation -- any of these may be abnormal

    
    if (hgt.little_end)
    {
       u.int16 = M_hgt_ht2(hgt, r1, c1);                           // little-endian -- lo-byte + hi-byte in correct order 
       h11     = (int32_t)(u.int16); 
  
       u.int16 = M_hgt_ht2(hgt, r1, c2);                           // little-endian -- lo-byte + hi-byte in correct order 
       h12     = (int32_t)(u.int16); 
       
       u.int16 = M_hgt_ht2(hgt, r2, c1);                           // little-endian -- lo-byte + hi-byte in correct order 
       h21     = (int32_t)(u.int16); 
       
       u.int16 = M_hgt_ht2(hgt, r2, c2);                           // little-endian -- lo-byte + hi-byte in correct order 
       h22     = (int32_t)(u.int16); 
    }
    else
    {
       // bytes in wrong order -- need to be copied individually
   
       u.uint8.hi = M_hgt_ht0(hgt, r1, c1);                        // big-endian -- hi byte first 
       u.uint8.lo = M_hgt_ht1(hgt, r1, c1);                        // big-endian -- lo byte second 
       h11        = (int32_t)(u.int16); 

       u.uint8.hi = M_hgt_ht0(hgt, r1, c2);                        // big-endian -- hi byte first 
       u.uint8.lo = M_hgt_ht1(hgt, r1, c2);                        // big-endian -- lo byte second 
       h12        = (int32_t)(u.int16); 

       u.uint8.hi = M_hgt_ht0(hgt, r2, c1);                        // big-endian -- hi byte first 
       u.uint8.lo = M_hgt_ht1(hgt, r2, c1);                        // big-endian -- lo byte second 
       h21        = (int32_t)(u.int16); 

       u.uint8.hi = M_hgt_ht0(hgt, r2, c2);                        // big-endian -- hi byte first 
       u.uint8.lo = M_hgt_ht1(hgt, r2, c2);                        // big-endian -- lo byte second 
       h22        = (int32_t)(u.int16); 
    }

    M__(M_out(L"r1/r2=%d/%d  c1/c2=%d/%d  h=%d/%d/%d/%d") % r1 % r2 % c1 % c2 % (int)h11 % (int)h12 % (int)h21 % (int)h22;)

    // bypass interpolation if any of these 1/2/4 heights are not normal height values

    if (
        ( (h11 == RASTER_VOID) || (h11 == RASTER_EMPTY) )
        ||
        ( (h12 == RASTER_VOID) || (h12 == RASTER_EMPTY) )
        ||
        ( (h21 == RASTER_VOID) || (h21 == RASTER_EMPTY) )
        ||
        ( (h22 == RASTER_VOID) || (h22 == RASTER_EMPTY) )
       )
    {
        return h;    
    }


    // compute interpolated height based on neighbors

    get_interpolation_weights(row_f, col_f, r1, r2, c1, c2, f11, f12, f21, f22);      // sets f11, f12, f21, f22

    h = (int16_t)M_round(f11 * (real_T)h11   +   f12 * (real_T)h12   +   f21 * (real_T)h21   +   f22 * (real_T)h22);

    M__(M_out(L"f11=%-10.6f  f12=%-10.6f f21=%-10.6f  f22=%-10.6f  sum=%-10.6f   ht=%d") % f11 % f12 % f21 % f22 % (f11 + f12 + f21 + f22) % (int)ht;) 
    return h; 
}
M_endf





//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       get_elev_be() -- get elevation from big-endian .hgt buffer only  -- pass in exact row/col
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int32_t  get_elev_be(const hgt_S& hgt, rc_T row, rc_T col) try
{
    union
    {
        int16_t int16;
        struct
        {
            uint8_t lo; 
            uint8_t hi;    // little-endian -- hi byte last
        } uint8;
    } u;

    M__(M_out(L"get_hgt() row/col = %d/%d") % row % col;)
            
   // bytes in wrong order -- need to be copied individually
 
    u.uint8.hi = M_hgt_ht0(hgt, row, col);                        // big-endian -- hi byte first 
    u.uint8.lo = M_hgt_ht1(hgt, row, col);                        // big-endian -- lo byte second 
   
    return (int32_t)(u.int16); 
}
M_endf




//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""