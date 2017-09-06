// h__types.h

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ==========
////            h__types.h -- common types and equates needed in many places 
////            ==========
//// 
////    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


// general types/typedefs
// ----------------------

#define M_real_T_IS_double    1   // indicate that real_T is defined double 

typedef double     real_T;        // set to float or double for performance vs. accuracy
typedef float      real32_T;
typedef double     real64_T;
typedef float      float32_T;     // simulated  inttypes.h  type
typedef double     float64_T;     // simulated  inttypes.h  type


// function types
// --------------

typedef   int (CALLBACK* do_import_T)(const std::wstring &);      // do_import() function in DLLs
//typedef int (__cdecl  *do_import_T)(const std::wstring &);      // do_import() function in DLLs



// types specific to windows, drawing, images, rasters, etc.
// ---------------------------------------------------------

typedef int32_t    rc_T;          // type for rows, cols, linear dimensions, etc.
typedef real_T     rcf_T;         // type for fractional rows, cols, linear dimensions, etc. (real_T)
typedef real_T     xy_T;          // type for X and Y coordinates on window
typedef uint64_t   sz_T;          // type for sizes and areas, pixel_counts, etc.  
typedef uint8_t    bitmask8_T;    // type for 8-bit bitmasks used for drawing letters
typedef uint8_t    alpha_T;       // type for alpha-channel parm 


// int16_U -- area for reversing bytes in a 2-byte integer 
// -------------------------------------------------------

union  int16_U             // area for reversing bytes in 16-bit integer  
{
    int16_t     int16;     // little-endian 16-bit integer
    uint16_t    uint16;    // little-endian 16-bit unsigned integer
    struct
    {
        uint8_t lo; 
        uint8_t hi;        // little-endian -- hi byte last
    } uint8;
};


// rgb_S -- rgb values i outimage_S, etc. 
// --------------------------------------

struct rgb_S                    // r, g, b triple in outimage file and elsewhere
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}; 

#define M_make_rgb(v, x, y, z) {v.r = (uint8_t)(x); v.g = (uint8_t)(y); v.b = (uint8_t)(z);}


// class enum with logging levels
// ------------------------------

typedef enum class log_E  {none, few, some, most, all} log_T;    


// -----------------------------------------------------------------------
// forward type declarations (incomplete structure/union/class/enum types)
// -----------------------------------------------------------------------

struct                       a_expression_S;
template<typename T> class   buffer_C; 
struct                       cache_S; 
class                        canvas_C;
enum class                   char_E;
class                        char_stream_C;
struct                       color_parm_S;
struct                       dbf_hdr_S;
struct                       draw_parm_S;
struct                       e_expression_S; 
struct                       fieldef_S; 
class                        filebuf_C;
struct                       font_S; 
struct                       frame_S; 
struct                       slist_S;
class                        infile_C;
class                        instream_C;
class                        instring_C;
struct                       in_char_S;
struct                       lrgb_S;
struct                       output_parm_S;
struct                       parmtype_S;
struct                       plist_S;
class                        pre_parse_C; 
struct                       results_S; 
struct                       symtab_S;
struct                       symval_S;
struct                       text_format_S; 
enum class                   tok_ut_E;
enum class                   tok_u1_E; 
enum class                   tok_u2_E; 
enum class                   tok_u3_E; 
enum class                   tok_u4_E; 
enum class                   tok_u5_E; 
enum class                   tok_u6_E; 
enum class                   tok_u7_E; 
enum class                   tok_u8_E; 
enum class                   tok_u9_E; 
class                        token_C;
enum class                   token_E;
class                        token_stream_C;
class                        tracker_C;
struct                       transform_S; 
enum class                   type_E;
struct                       typdef_S;
struct                       value_S; 
struct                       var_S; 
struct                       verbdef_S; 
struct                       vlist_S; 
class                        window_C;



///////// general use typedef's in Windows //////////////////////////////////////////////////
//
//
//  note: int8_t, etc, come from inttypes.h
//
//
//
//            built-in types        other names (typedef's)        
//  -------   ------------------    ------------------------------------------ 
//
//            void                  VOID <winnt.h>  *LPVOID
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//            char                  *PSZ <windef.h>           
//                                  CHAR <winnt.h>  *PCHAR <winnt.h>  *PCH  *LPCH
//                                                  *PSTR  *LPSTR  *NPSTR   
//                                  CCHAR <winnt.h>     
//
//  int8_t    signed char 
//  
//
// 
//            unsigned char         UCHAR <windef.h>  *PUCHAR
//  uint8_t                         BYTE  <windef.h>  *PBYTE  *LPBYTE 
//                                  FCHAR <winnt.h>
//                                  BOOLEAN <winnt.h>  *PBOOLEAN
//
//
//            __int8
//            signed __int8
//
//
//            unsigned __int8
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
//
//            short                 SHORT <winnt.h>  *PSHORT
//  int16_t   short int             HALF_PTR <basetsd.h> *PHALF_PTR
//            signed short
//            signed short int
//
//            __int16
//            signed __int16
//
//
//            unsigned short        USHORT <windef.h>  *PUSHORT          
//  uint16_t  unsigned short int    WORD   <windef.h>  *PWORD  *LPWORD
//                                  wchar_t <basetypes.h> 
//                                  WCHAR <winnt.h>  *PWCHAR  *PWCH    *LPWCH
//                                                   *PWSTR   *LPWSTR  *NWPSTR
//                                  FSHORT <winnt.h>
//                                  UHALF_PTR <basetsd.h>  *PUHALF_PTR
//
//
//            unsigned __int16
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//            int                   BOOL <windef.h>  *PBOOL  *LPBOOL
//            signed                INT  <windef.h>  *PINT   *LPINT
//            signed int            LONG32 <basetsd.h>  *PLONG32
//                                  INT32 <basetsd.h>  *PINT32
//                                  INT_PTR <basetsd.h>  *PINT_PTR  
//                                  SSIZE_T <basetsd.h>  *PSSIZE_T                            
//
// 
//            __int32
//            signed __int32
//
//
//            unsigned              UINT <windef.h>  *PUINT
//  uint32_t  unsigned int          WPARAM <wtypes.h> 
//                                  ULONG32 <basetsd.h> *PULONG32
//                                  UINT32 <basetsd.h>  *PUINT32
//                                  DWORD32 <basetsd.h> *PDWORD32 
//                                  UINT_PTR <basetsd.h>  *PUINT_PTR
//                                  SIZE_T <basetsd.h>  *PSIZE_T
//
//       
//            unsigned __int32 
//
//
//            long                  LONG <winnt.h>  *PLONG  *LPLONG <windef.h>
//            long int              LPARAM <windef.h>
//            signed long           LRESULT <windef.h>
//            signed long int
//
//
//            unsigned long         ULONG <windef.h> *PULONG    
//            unsigned long int     DWORD <windef.h> *PDWORD  *LPDWORD
//                                  FLONG <winnt.h>
//
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//            __int64               LONGLONG <winnt.h>        *PLONGLONG
//  int64_t   signed __int64        LONG64 <basetsd.h>        *PLONG64
//                                  INT64 <basetsd.h>         *PINT64
//                                  
//                                  LARGE_INTEGER.QuadPart    *(PLARGE_INTEGER.QuadPart)  <winnt.h> 
//
//            unsigned __int64      ULONGLONG <winnt.h>       *PULONGLONG
//  uint64_t                        DWORDLONG <winnt.h>       *PDWORDLONG    
//                                  ULONG64 <basetsd.h>       *PULONG64
//                                  UINT64 <basetsd.h>        *PUINT64      
//                                  DWORD64 <basetsd.h>       *PDWORD64 
//                                  
//                                  ULARGE_INTEGER.QuadPart   *(PULARGE_INTEGER.QuadPart)  <winnt.h>
//
// --------------------------------------------------------------------------------
//
//             float                FLOAT <windef.h>             *PFLOAT     
//                                  Gdiplus::REAL <gdiplus.h>
//                                  real_T <h_types.h>
//                                  real32_t <h_types.h>                               
//
//             double               DOUBLE <wtypes.h>
//                                  real64_t <h_types.h>  
//
//             long double
//
//
/////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////