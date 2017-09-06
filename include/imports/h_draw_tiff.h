// h_draw_tiff.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     h_draw_tiff.h
////     =============
//// 
//// 
////     contains definitions for TIFF files and related items
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


// -------------------------------------------------
// bitstring_T oriented items (for LZW encoding)
// -------------------------------------------------

struct bitstring_S   // structure for one lzw-compressed strip 
{
    uint64_t               bits;  
    std::vector<uint8_t>   buf;   // should convert to buffer_C<uint8_t>

};


union shift_U  // structure for shifting bits (internal)
{
    uint32_t uint32;
    struct
    {
        uint8_t b0; 
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
    } bytes;
}; 


#define M_append(bstr, v, n)                \
{                                           \
    set_bitstring(bstemp, (v), (n));        \
    append_bitstring(bstr, bstemp);         \
}

#define M_appnum(bstr, v, n)                \
    appendnum_bitstring(bstr, (v), (n));   

#define M_bytes_used(bstr)   ((bstr.bits + 7ULL) / 8ULL)



// -------------------------------------------------
// items related to TIFF headers 
// -------------------------------------------------


// MACROs for putting data items into IFD entries -- need pre-declared buf data area 


#define M_INT8(x)      ( *( int8_t  *)&(buf_p[(x)]) )
#define M_INT16(x)     ( *( int16_t *)&(buf_p[(x)]) )
#define M_INT32(x)     ( *( int32_t *)&(buf_p[(x)]) )
#define M_UINT8(x)     ( *(uint8_t  *)&(buf_p[(x)]) )
#define M_UINT16(x)    ( *(uint16_t *)&(buf_p[(x)]) )
#define M_UINT32(x)    ( *(uint32_t *)&(buf_p[(x)]) )


// TIFF info/specification structures

struct rational_S
{
    uint32_t num;
    uint32_t den;
}; 


struct rgb_tiff_S    // little-endian, uncompressed RGB .tif request
{
    uint32_t       outimage_width;
    uint32_t       outimage_length;
    uint32_t       rows_per_strip;
    rational_S     x_resolution;
    rational_S     y_resolution;
    uint16_t       resolution_unit;
    uint16_t       orientation;
}; 


struct lzw_tiff_S    // little-endian, RGB .tif , compressed with LZW request
{
    uint32_t                outimage_width;
    uint32_t                outimage_length;
    uint32_t                rows_per_strip;
    uint32_t                n_strips;
    rational_S              x_resolution;
    rational_S              y_resolution;
    uint16_t                resolution_unit;
    uint16_t                orientation;
    std::vector<uint32_t>   strip_byte_ct;   
}; 


/////////////////// TIFF file items ///////////////////


// IFH layout -- file header

struct ifh_S
{
   char      id1;
   char      id2; 
   uint8_t   version; 
   uint8_t   res;
   uint32_t  offset0;    // offset to 1st IFD
}; 


// Layout of a single tag item in an IFD


struct tag_S
{
   uint16_t  tag_id;
   uint16_t  data_type;  
   uint32_t  data_count; 

   union
   {
       int8_t       int8; 
       uint8_t      uint8;
       int16_t      int16; 
       uint16_t     uint16;
       int32_t      int32; 
       uint32_t     uint32;
       float        float32;
       uint32_t     data_offset;  
   } u;
}; 




// -------------------------------------- 
// MACROs to put data into TIFF tag items 
// -------------------------------------- 

// "b" is variablbe name of buffer containing IFD entries, d is the data item being added

#define M_put_u16(b, d)                                              \
{                                                                    \
    uint16_t v = (d);                                                \
    memcpy((void *)(b.p1 + buf_offset), (void *)&v, sizeof v);       \
    buf_offset += sizeof v;                                          \
}


#define M_put_u32(b, d)                                            \
{                                                                  \
    uint32_t v = (d);                                              \
    memcpy((void *)(b.p1 + buf_offset), (void *)&v, sizeof v);     \
    buf_offset += sizeof v;                                        \
}


#define M_tag_u8(i, d)                        \
{                                             \
    ifd[tag_n].tag_id         = (i);          \
    ifd[tag_n].data_type      = C_uint8;      \
    ifd[tag_n].data_count     = 1  ;          \
    ifd[tag_n].u.uint8        = (d);          \
    tag_n++;                                  \
}


#define M_tag_u16(i, d)                       \
{                                             \
    ifd[tag_n].tag_id         = (i);          \
    ifd[tag_n].data_type      = C_uint16;     \
    ifd[tag_n].data_count     = 1  ;          \
    ifd[tag_n].u.uint16       = (d);          \
    tag_n++;                                  \
}


#define M_tag_u32(i, d)                       \
{                                             \
    ifd[tag_n].tag_id         = (i);          \
    ifd[tag_n].data_type      = C_uint32;     \
    ifd[tag_n].data_count     = 1  ;          \
    ifd[tag_n].u.uint32       = (d);          \
    tag_n++;                                  \
}


#define M_tag_o(i, t, c)                                    \
{                                                           \
    ifd[tag_n].tag_id         = (i);                        \
    ifd[tag_n].data_type      = (t);                        \
    ifd[tag_n].data_count     = (c);                        \
    ifd[tag_n].u.data_offset    = tiff_offset + buf_offset; \
    tag_n++;                                                \
}


// ---------------------------- 
// Constants used in TIFF files (all in tiff namespace)
// ---------------------------- 


namespace tiff_N
{

// id1 and id2 and version bytes

const char     C_big_endian                    = 'M';
const char     C_little_endian                 = 'I';
const uint8_t  C_version                       = 42;


// TIFF tag types 

const uint16_t C_new_subfile_type           = 0x00FE; 
const uint16_t C_subfile_type               = 0x00FF; 
const uint16_t C_outimage_width             = 0x0100;   // value can be uint32_t or uint16_t
const uint16_t C_outimage_length            = 0x0101;   // value can be uint32_t or uint16_t
const uint16_t C_bits_per_sample            = 0x0102;   // value is uint16_t
const uint16_t C_compression                = 0x0103; 
const uint16_t C_photometric_interpretation = 0x0106; 
const uint16_t C_thresholding               = 0x0107; 
const uint16_t C_cell_width                 = 0x0108;   // value is uint16_t
const uint16_t C_cell_length                = 0x0109;   // value is uint16_t
const uint16_t C_fill_order                 = 0x010A; 
const uint16_t C_outimage_description       = 0x010E;   // value is ASCII string 
const uint16_t C_make                       = 0x010F;   // value is ASCII string
const uint16_t C_model                      = 0x0110;   // value is ASCII string
const uint16_t C_strip_offsets              = 0x0111;   // values are uint16_t or uint32_t
const uint16_t C_orientation                = 0x0112;   // value is uint16_t 
const uint16_t C_samples_per_pixel          = 0x0115;   // value is uint16_t
const uint16_t C_rows_per_strip             = 0x0116;   // value is uint16_t or uint32_t
const uint16_t C_strip_byte_counts          = 0x0117;   // values are uint16_t or uint32_t
const uint16_t C_min_sample_value           = 0x0118;   // value is uint16_t -- default = 0
const uint16_t C_max_sample_value           = 0x0119;   // value is uint16_t -- default = 255, 32767, etc. 
const uint16_t C_x_resolution               = 0x011A;   // value is uint32_t/uint32_t = pixels per resolution unit 
const uint16_t C_y_resolution               = 0x011B;   // value is uint32_t/uint32_t = pixels per resolution unit 
const uint16_t C_planar_configuration       = 0x011C;
const uint16_t C_free_offsets               = 0x0120;   // values are uint32_t
const uint16_t C_free_byte_counts           = 0x0121;   // values are uint32_t
const uint16_t C_gray_response_unit         = 0x0122;
const uint16_t C_gray_response_curve        = 0x0123;   // values are short
const uint16_t C_resolution_unit            = 0x0128;
const uint16_t C_software                   = 0x0131;   // value is ASCII string 
const uint16_t C_date_time                  = 0x0132;   // value is ASCII string 
const uint16_t C_artist                     = 0x013B;   // value is ASCII string 
const uint16_t C_host_computer              = 0x013C;   // value is ASCII string 
const uint16_t C_predictor                  = 0x013D;   // values are uint16_t
const uint16_t C_color_map                  = 0x0140;   // values are uint16_t
const uint16_t C_extra_samples              = 0x0152;   
const uint16_t C_copyright                  = 0x8298;   // value is ASCII string 


// new_file_subtype values

const uint32_t C_nft_default                = 0;
const uint32_t C_nft_reduced_outimage       = 1;
const uint32_t C_nft_page                   = 2;
const uint32_t C_nft_mask                   = 4;
const uint32_t C_nft_mrc_imaging_model      = 8;


// file_subtype values

const uint16_t C_ft_outimage               = 1;
const uint16_t C_ft_reduced_outimage       = 2;
const uint16_t C_ft_page                   = 3;


// compression

const uint16_t C_cmp_none                  = 1;  // default
const uint16_t C_cmp_ccitt_rle             = 2;
const uint16_t C_cmp_ccitt_group_3         = 3;
const uint16_t C_cmp_ccitt_group_4         = 4;
const uint16_t C_cmp_lzw                   = 5;
const uint16_t C_cmp_old_jpeg              = 6;
const uint16_t C_cmp_new_jpeg              = 7;
const uint16_t C_cmp_adobe_deflate         = 8;
const uint16_t C_cmp_itu_t_t85             = 9;
const uint16_t C_cmp_itu_t_t43             = 10;
const uint16_t C_cmp_packbits              = 32773;


// photometric interpretation

const uint16_t C_pi_white_is_zero          = 0;
const uint16_t C_pi_black_is_zero          = 1;
const uint16_t C_pi_rgb                    = 2;
const uint16_t C_pi_palette_color          = 3;
const uint16_t C_pi_transparency_mask      = 4;
const uint16_t C_pi_separated              = 5;
const uint16_t C_pi_y_cb_cr                = 6;
const uint16_t C_pi_cie_lab                = 8;
const uint16_t C_pi_icc_lab                = 9;
const uint16_t C_pi_itu_lab                = 10;
const uint16_t C_pi_cfa                    = 32803;
const uint16_t C_pi_linear_raw             = 34892;


// thresholding

const uint16_t C_thr_no_dithering          = 1;
const uint16_t C_thr_halftone              = 2;
const uint16_t C_thr_error_diffusion       = 3;


// fill order

const uint16_t C_fo_msb_to_lsb             = 1;  // default
const uint16_t C_fo_lsb_to_msb             = 2;


// orientation

const uint16_t C_or_top_left               = 1;  // default
const uint16_t C_or_top_right              = 2;
const uint16_t C_or_bottom_right           = 3;
const uint16_t C_or_bottom_left            = 4;
const uint16_t C_or_left_top               = 5;  
const uint16_t C_or_right_top              = 6;
const uint16_t C_or_right_bottom           = 7;
const uint16_t C_or_left_bottom            = 8;


// planar configuration

const uint16_t C_pc_chunky_format          = 1;  // default
const uint16_t C_pc_planar_format          = 2;


// gray response unit

const uint16_t C_gr_tenths                 = 1;  
const uint16_t C_gr_hundredths             = 2;  // default
const uint16_t C_gr_thousandths            = 3;  
const uint16_t C_gr_10_thousandths         = 4;  
const uint16_t C_gr_100_thousandths        = 5;  


// resolution_unit

const uint16_t C_ru_none                   = 1;  
const uint16_t C_ru_inch                   = 2;  // default
const uint16_t C_ru_centimeter             = 3;


// predictor 
const uint16_t C_pd_none                   = 1;  
const uint16_t C_pd_horizontal_difference  = 2;
const uint16_t C_pd_flt_pt_horiz_diff      = 3;



// extra samples (N_values, one for each extra sample)

const uint16_t C_es_unspecified            = 0;  
const uint16_t C_es_associated_alpha       = 1;
const uint16_t C_es_unassociated_alpha     = 2;




// TIFF item datatypes

const int16_t C_uint8              = 1;
const int16_t C_ascii              = 2;
const int16_t C_uint16             = 3;  
const int16_t C_uint32             = 4;
const int16_t C_rational           = 5;  
const int16_t C_int8               = 6;
const int16_t C_undefined          = 7; 
const int16_t C_int16              = 8; 
const int16_t C_int32              = 9;
const int16_t C_float              = 10; 
const int16_t C_double             = 11; 


} // namespace tiff_N


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""





