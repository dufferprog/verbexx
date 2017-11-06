// ex_display.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ==============
////     ex_display.cpp -- display blocks, vlists, expressions, values, stacks, variables, etc.
////     ==============
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

#define M_IN_EX_DLL

#include "h__common.h"

#include "h_ex_interface.h"
#include "h_ex_parse.h"
#include "h_ex_verb.h"      


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  forward declarations for selected static functions 
//  --------------------------------------------------

static std::wstring str_parmtype(        const parmtype_S&                                                );
static void         display_parmtype(    const parmtype_S&, const std::wstring&, const std::wstring& = L"");
static void         display_plist(       const plist_S&   , const std::wstring&, const std::wstring& = L"");




//╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║                                               SSSSSSSSS       TTTTTTTTTT         RRRRRRRRR          IIIIIIIIII         NN      NN         GGGGGGGG 
//║╳╳╳╳║                                              SSSSSSSSSS       TTTTTTTTTT         RRRRRRRRRR         IIIIIIIIII         NNN     NN        GGGGGGGGGG
//║╳╳╳╳║                                              SS                   TT             RR      RR             II             NNNN    NN        GG      GG
//║╳╳╳╳║                                              SS                   TT             RR      RR             II             NN NN   NN        GG        
//║╳╳╳╳║                                               SSSSSS              TT             RRRRRRRRRR             II             NN  NN  NN        GG        
//║╳╳╳╳║                                                 SSSSSS            TT             RRRRRRRRR              II             NN   NN NN        GG    GGGG
//║╳╳╳╳║                                                      SS           TT             RR   RR                II             NN    NNNN        GG    GGGG
//║╳╳╳╳║                                                      SS           TT             RR    RR               II             NN     NNN        GG      GG
//║╳╳╳╳║                                              SSSSSSSSSS           TT             RR     RR          IIIIIIIIII         NN      NN        GGGGGGGGGG
//║╳╳╳╳║                                              SSSSSSSSS            TT             RR      RR         IIIIIIIIII         NN      NN         GGGGGGGG 
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════







////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   type_str() -- convert type_E enum value to printable string  (free function -- see also type_str() in token_C: 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring type_str(type_E kind) try
{
    switch (kind)
    {
        case type_E::none        : return std::wstring { L"none"                      };      break;
        case type_E::no_value    : return std::wstring { L"no_value"                  };      break;
        case type_E::special     : return std::wstring { L"special"                   };      break;
        case type_E::error       : return std::wstring { L"error"                     };      break;
        case type_E::unit        : return std::wstring { L"unit"                      };      break;
        case type_E::boolean     : return std::wstring { L"boolean"                   };      break;
        case type_E::int8        : return std::wstring { L"int8"                      };      break;    
        case type_E::int16       : return std::wstring { L"int16"                     };      break;
        case type_E::int32       : return std::wstring { L"int32"                     };      break;
        case type_E::int64       : return std::wstring { L"int64"                     };      break;
        case type_E::uint8       : return std::wstring { L"uint8"                     };      break;
        case type_E::uint16      : return std::wstring { L"uint16"                    };      break;
        case type_E::uint32      : return std::wstring { L"uint32"                    };      break;
        case type_E::uint64      : return std::wstring { L"uint64"                    };      break;
        case type_E::float32     : return std::wstring { L"float32"                   };      break;
        case type_E::float64     : return std::wstring { L"float64"                   };      break;
        case type_E::array       : return std::wstring { L"array"                     };      break;
        case type_E::structure   : return std::wstring { L"structure"                 };      break;
        case type_E::string      : return std::wstring { L"string"                    };      break;
        case type_E::identifier  : return std::wstring { L"identifier"                };      break;
        case type_E::verbname    : return std::wstring { L"verbname"                  };      break;
        case type_E::keyname     : return std::wstring { L"keyname"                   };      break;
        case type_E::typdef      : return std::wstring { L"typedef"                   };      break;
        case type_E::ref         : return std::wstring { L"ref"                       };      break;
        case type_E::vlist       : return std::wstring { L"vlist"                     };      break;
        case type_E::expression  : return std::wstring { L"expression"                };      break;
        case type_E::block       : return std::wstring { L"block"                     };      break;
        case type_E::verbset     : return std::wstring { L"verbset"                   };      break; 
        default                  : return std::wstring { L"???-Unknown_type_E::-???"  };      break; 
    }
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   verb_name() -- return printable verb name (with sigil, if present) 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring verb_name(const a_expression_S& expression) try
{
    std::wstring name {};


    if (expression.verb_value.ty == type_E::verbname)                   // handle simple verbname 
    {
        if (expression.has_sigil)
            name += (wchar_t)(expression.sigil);
       
        name += expression.verb_value.string;
    }
    else                                                                // handle expression yielding verbname 
    {
        name  = L"<"; 
        name += str_value(expression.verb_value, true, false, false);   // debug formatting for value  
        name += L">";                          
    }


    return name;
}
M_endf

///////////////////////////////////////////////////////////////

std::wstring verb_name(const e_expression_S& eval_expression) try
{
    std::wstring name {};

    if (eval_expression.has_sigil)
        name += (wchar_t)(eval_expression.sigil);
       
    name += eval_expression.verb_name;

    return name;
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   str_value() -- create printable string from passed-in value 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring str_value(const value_S& value, bool debug, bool debugx, bool nest) try
{
    std::wstring str        { };          // start with empty main string  
    std::wstring str_d1     { };          // debug prefix string  
    std::wstring str_d2     { };          // debug postfix string

    std::wstring fmt_int8   { }; 
    std::wstring fmt_int16  { }; 
    std::wstring fmt_int32  { };
    std::wstring fmt_int64  { };
    std::wstring fmt_uint8  { }; 
    std::wstring fmt_uint16 { }; 
    std::wstring fmt_uint32 { };
    std::wstring fmt_uint64 { };


    // set format strings based on debugx or not (debugx not supported for floating point values)

    if (debugx)
    {
        fmt_int8   = L"0x%02hX"     ;
        fmt_int16  = L"0x%04hX"     ;
        fmt_int32  = L"0x%08X"      ;
        fmt_int64  = L"0x%016I64X"  ;
        fmt_uint8  = L"0x%02hX"     ;
        fmt_uint16 = L"0x%04hX"     ;
        fmt_uint32 = L"0x%08X"      ;
        fmt_uint64 = L"0x%016I64X"  ;
    }
    else
    {
        fmt_int8   = L"%hd"    ;
        fmt_int16  = L"%hd"    ;
        fmt_int32  = L"%d"     ;
        fmt_int64  = L"%I64d"  ;
        fmt_uint8  = L"%hu"    ;
        fmt_uint16 = L"%hu"    ;
        fmt_uint32 = L"%u"     ;
        fmt_uint64 = L"%I64u"  ;
    } 


    // create main formatted string and debug mode prefix/suffix string    

    if       (value.ty == type_E::string)
    {
        str    = value.string; 
        str_d1 = std::wstring { const_N::chws_string_start }; 
        str_d2 = std::wstring { const_N::chws_string_end   };
    }
    else if  (value.ty == type_E::int8)
    {   
        if (debugx)     
            str = fmt_str(fmt_int8, (uint16_t)(value.uint8));
        else
            str = fmt_str(fmt_int8, (int16_t )(value.int8 ));

        str_d2  = std::wstring {L"_"} + std::wstring {const_N::chws_signed_lower} + std::wstring {L"8"}; 
    }
    else if  (value.ty == type_E::int16)
    {
        str     = fmt_str(fmt_int16, value.int16);
        str_d2  = std::wstring {L"_"} + std::wstring {const_N::chws_signed_lower} + std::wstring {L"16"}; 
    }
    else if  (value.ty == type_E::int32)
    {
        str     = fmt_str(fmt_int32, value.int32);
        str_d2  = std::wstring {L"_"} + std::wstring {const_N::chws_signed_lower} + std::wstring {L"32"};
    }
    else if  (value.ty == type_E::int64)
    {
        str     = fmt_str(fmt_int64, value.int64);
        str_d2  = std::wstring {L"_"} + std::wstring {const_N::chws_signed_lower} + std::wstring {L"64"};
    }
    else if  (value.ty == type_E::unit)
    {
        str     = std::wstring { L"0_" } + std::wstring {const_N::chws_unsigned_lower} + std::wstring {L"0"};  
        str_d2  = std::wstring {L""};
    }
    else if  (value.ty == type_E::boolean)
    {
        if (value.boolean)
            str  = std::wstring { L"1_" } + std::wstring {const_N::chws_unsigned_lower} + std::wstring {L"1"};
        else
            str  = std::wstring { L"0_" } + std::wstring {const_N::chws_unsigned_lower} + std::wstring {L"1"};
        
        str_d2  = std::wstring {L""};
    }
    else if  (value.ty == type_E::uint8)
    {
        str     = fmt_str(fmt_uint8, (uint16_t)(value.uint8)); 
        str_d2  = std::wstring {L"_"} + std::wstring {const_N::chws_unsigned_lower} + std::wstring {L"8"};
    }
    else if  (value.ty == type_E::uint16)
    {
        str    = fmt_str(fmt_uint16, value.uint16); 
        str_d2 = std::wstring {L"_"} + std::wstring {const_N::chws_unsigned_lower} + std::wstring {L"16"};
    }
    else if  (value.ty == type_E::uint32)
    {
        str    = fmt_str(fmt_uint32, value.uint32); 
        str_d2 = std::wstring {L"_"} + std::wstring {const_N::chws_unsigned_lower} + std::wstring {L"32"};
    }
    else if  (value.ty == type_E::uint64)
    {
        str    = fmt_str(fmt_uint64, value.uint64); 
        str_d2 = std::wstring {L"_"} + std::wstring {const_N::chws_unsigned_lower} + std::wstring {L"64"};
    }
    else if  (value.ty == type_E::float32)
    {
        str    = fmt_str(L"%#.9g", (float64_T)(value.float32));
        str_d2 = std::wstring {L"_"} + std::wstring {const_N::chws_float_lower} + std::wstring {L"32"};;
    }
    else if  (value.ty == type_E::float64)
    {
        str    = fmt_str(L"%#.18g", value.float64);
        str_d2 = std::wstring {L"_"} + std::wstring {const_N::chws_float_lower} + std::wstring {L"64"};
    }
    else if  (value.ty == type_E::block) 
    {
        str =  L"{block}";
    }
    else if  (value.ty == type_E::verbset) 
    {
        str =  L"<verbset>";
    }
    else if  (value.ty == type_E::typdef) 
    {
        str =  std::wstring { L"<typedef:" } + type_str(value.typdef_sp->kind) + L">";
    }
    else if  (value.ty == type_E::ref) 
    {
        str =  L"<ref>";
    }
    else if  (value.ty == type_E::structure) 
    {
        str =  L"<struct>";
    }
    else if  (value.ty == type_E::array) 
    {
        str =  L"<array>";
    }
    else if  (value.ty == type_E::expression)
    {
        str =  L"(expression)";
    }
    else if  (value.ty == type_E::vlist) 
    {
        if (nest)
            str = L"[" + str_vlist(*(value.vlist_sp), debug, debugx, nest) + L"]";
        else
            str =  L"[vlist]";
    }
    else if  (value.ty == type_E::identifier)
    {
        str    = value.string;
        str_d1 = L"identifier:"; 
    }
    else if  (value.ty == type_E::verbname)
    {
        str    = value.string;
        str_d1 = L"verbname:"; 
    }
    else if  (value.ty == type_E::keyname)
    {
        str    = value.string;
        str_d1 = L"keyname:"; 
    }
    else if  (value.ty == type_E::none)
    {
        str    = L"uninitialized value";
        str_d1 = L"none:"; 
    }
    else  
    {
        str =  L"??? unknown value type ???";
    }   


    // add prefix and/or suffix debug strings, if this is a debug mode display

    if ( debug || debugx)
        str = str_d1 + str + str_d2; 


    return str;             // return completed string
}                                                
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   str_vlist() -- create printable string from passed-in vlist 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring str_vlist(const vlist_S& vlist, bool debug, bool debugx, bool nest) try
{
    std::wstring str      {     };       // start with open bracket 
    bool        add_space {false};       // true -- need to add space before current formatted value 


    // add positional values to string 

    if (vlist.value_ct > 0)
    {
        for (const auto& value : vlist.values)
        {
             if (add_space)    
                str += L" ";
    
             auto strv = str_value(value, debug, debugx, nest);

             if (strv.length() > 0)
             {
                 str += str_value(value, debug, debugx, nest);
                 add_space = true;                         // space before next value is needed 
             }
             else
             {
                 add_space = false;                        // no space needed if value string is zero-length  
             }
        }
    }


    // add evaluated/unevaluated keyword values to string
    // --------------------------------------------------

    if (vlist.kw_eval_done)                                // keyword evaluation has been done for this vlist ? 
    {
        if (vlist.eval_kws.size() > 0)                     // add any evaluated keywords
        {
            for (const auto& elem : vlist.eval_kws)
            {
                 if (add_space)    
                    str += L" ";
        
                 str +=  elem.first + L":" + str_value(elem.second, debug, debugx, nest);
                 add_space = true;                         // space before next value is needed    
            }
        }
    }
    else                                                   // keyword evaluation has not yet been done for this vlist (or there was nothing to evaluate)
    {
        if (vlist.keywords.size() > 0)                     // add any unevaluated keywords to string    
        {
            for (const auto& elem : vlist.keywords)
            {
                if (add_space)    
                   str += L" ";
    
                if (elem.name.ty == type_E::string)
                    str +=  elem.name.string + L":"           + str_value(elem.value, debug, debugx, nest);
                else
                    str +=  L"`( keyword name expression `):" + str_value(elem.value, debug, debugx, nest);
                                                        
                add_space = true;                          // space before next value is needed    
            }
        }    
    }  

    return str;                                            // return completed string
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   results_msg_string() -- pass back string for error messages 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring results_msg_string(const results_S& results) try
{
    // display results flags and string

    std::wstring ret_ws { }; 

    if (results.multiple_results             ) ret_ws += L"multiple_results "           ; 
    if (results.no_results                   ) ret_ws += L"no_results "                 ; 
    if (results.ignore_results               ) ret_ws += L"ignore_results "             ; 
    if (results.re_eval_expression_results   ) ret_ws += L"re_eval_expression_results " ;
    if (results.builtin_verb_results         ) ret_ws += L"builtin_verb_results "       ;
    if (results.special_results              ) ret_ws += L"special_results "            ; 
    if (results.error                        ) ret_ws += L"error "                      ; 
    if (results.break_flag                   ) ret_ws += L"@BREAK "                     ;
    if (results.continue_flag                ) ret_ws += L"@CONTINUE "                  ;
    if (results.quit_flag                    ) ret_ws += L"@QUIT "                      ; 
    if (results.end_flag                     ) ret_ws += L"@END "                       ;
    if (results.leave_flag                   ) ret_ws += L"@LEAVE "                     ;
    if (results.goto_flag                    ) ret_ws += L"@GOTO "                      ;
    if (results.xctl_flag                    ) ret_ws += L"@XCTL "                      ;
    if (results.return_flag                  ) ret_ws += L"@RETURN "                    ;
    if (results.throw_flag                   ) ret_ws += L"@THROW "                     ;
    if (results.skip_flag                    ) ret_ws += L"@SKIP "                      ;

    if (results.str.size() > 0)
        ret_ws += ( std::wstring { L" string = \""} + results.str + std::wstring { L"\"" });
    
    return ret_ws; 
}
M_endf






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║                                          DDDDDDDD          IIIIIIIIII         SSSSSSSSS        PPPPPPPPP         LL                   AA            YY      YY
//║╳╳╳╳║                                          DDDDDDDDD         IIIIIIIIII        SSSSSSSSSS        PPPPPPPPPP        LL                  AAAA            YY    YY 
//║╳╳╳╳║                                          DD      DD            II            SS                PP      PP        LL                 AA  AA            YY  YY  
//║╳╳╳╳║                                          DD      DD            II            SS                PP      PP        LL                AA    AA            YYYY   
//║╳╳╳╳║                                          DD      DD            II             SSSSSS           PPPPPPPPPP        LL               AA      AA            YY    
//║╳╳╳╳║                                          DD      DD            II               SSSSSS         PPPPPPPPP         LL               AAAAAAAAAA            YY    
//║╳╳╳╳║                                          DD      DD            II                    SS        PP                LL               AAAAAAAAAA            YY    
//║╳╳╳╳║                                          DD      DD            II                    SS        PP                LL               AA      AA            YY    
//║╳╳╳╳║                                          DDDDDDDDD         IIIIIIIIII        SSSSSSSSSS        PP                LLLLLLLLLL       AA      AA            YY    
//║╳╳╳╳║                                          DDDDDDDD          IIIIIIIIII        SSSSSSSSS         PP                LLLLLLLLLL       AA      AA            YY    
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_statistics() -- display statistival info on console 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_statistics() try
{
    auto       p_elapsed    { get_parse_elapsed() };
    auto       e_elapsed    { get_eval_elapsed()  };
    float64_T  verbs_per_s  { 0.0                 };


    // get estimate for parse-phase and eval-phase elapsed times (up to now), if these are not yet filled in

    if (p_elapsed < 0.0)
    {
        p_elapsed = elapsed_pc_time(2);
    }

    if (e_elapsed < 0.0)
    {
        e_elapsed = elapsed_pc_time(3);    
    }

    if (e_elapsed > 0.0)
       verbs_per_s = ((float64_T)(get_eval_verb_count())) / e_elapsed;


    M_out( L" "                                                                                   ) ;
    M_out( L"-----------------------------------------------------------------------------------" ) ;
    M_out( L"                : characters=%d  tokens=%d"                                          )  % get_character_count()   % get_raw_token_count() ;
    M_out( L"                : blocks=%d  statements=%d  verbs=%d  values=%d"                     )  % get_eval_block_count()  % get_eval_statement_count() % get_eval_verb_count() % get_eval_value_count() ;
    M_out( L"                : stack frames = %d, max stack depth = %d"                           )  % get_eval_frame_serial() % get_eval_frame_max_depth() ;
    M_out( L"main parse phase: elapsed_time = %.6f seconds"                                       )  % p_elapsed                             ; 
    M_out( L"evaluation phase: elapsed_time = %.6f seconds   verbs/sec = %.1f"                    )  % e_elapsed               % verbs_per_s ;
    M_out( L"-----------------------------------------------------------------------------------" ) ;
    return; 
}
M_endf






////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_block() -- display passed-in block 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_block(const block_S& block, const std::wstring& ws, const std::wstring& mod, bool show_expressions, bool suppress_nesting, const std::wstring& nest) try
{
    // display data from block header 

    M_out(L"%s %s %s: %80t block: expression_ct=%d  main_label=«%s»") % ws % nest % mod % block.expression_ct % block.label;   


    // display all labels in this block

    if (block.labels.size() > 0)
        for (const auto& elem : block.labels)
            M_out(L"%s %s %s %80t        label=«%s» %110t expression_index=%d") % ws % nest % mod % elem.first % elem.second;  
    

    // display expressions, if requested

    if ( (block.expression_ct > 0) && (show_expressions) )
    {
        auto i = 0; 
        for (const auto& expression : block.expressions)
        {
            M_out(L"- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
            display_expression(expression, ws, mod + L"{" + std::to_wstring(i++) + L"} ", suppress_nesting, nest + L"."); 
        }
        M_out(L"- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
    }

    return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_expression() -- display passed-in expression, optionally with nested expressions, etc.
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_expression(const a_expression_S& expression, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    std::wstring type_s  {L"no verb   "}; 
     

    // set up type and location strings for main verb in this token

    if (expression.has_verb)   type_s = L"has verb  ";

    if (mod == L"") 
    {          
        M_out(L"%s %sexpression -- %-15s name=%s %77t -- parms:   %125t -- loc = %s -- main verb loc = %s" ) % ws % nest %      type_s % verb_name(expression) % expression_loc_str(expression) % verb_loc_str(expression);
    }
    else
    {
        M_out(L"%s %s%sexpression -- %-15s name=%s %77t -- parms: %125t -- loc = %s -- main verb loc = %s") % ws % nest % mod % type_s % verb_name(expression) % expression_loc_str(expression) % verb_loc_str(expression);
    }

    display_vlist(expression.lparms, ws, mod + L"(L )", suppress_nesting, nest + L".");
    display_vlist(expression.rparms, ws, mod + L"( R)", suppress_nesting, nest + L".");
        
    return; 
}
M_endf

///////////////////////////////////////////////////////

void display_expression(const e_expression_S& eval_expression, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    std::wstring type_s  {L"verb missing "}; 
     

    // set up type and location strings for main verb in this token

    if (eval_expression.has_verb)   type_s = L"has verb  ";

    if (mod == L"") 
    {          
        M_out(L"%s %sexpression -- %-15s name=%s %77t -- parms:   %125t -- loc = %s -- main verb loc = %s" ) % ws % nest %      type_s % verb_name(eval_expression) % expression_loc_str(eval_expression) % verb_loc_str(eval_expression);
    }
    else
    {
        M_out(L"%s %s%sexpression -- %-15s name=%s %77t -- parms: %125t -- loc = %s -- main verb loc = %s") % ws % nest % mod % type_s % verb_name(eval_expression) % expression_loc_str(eval_expression) % verb_loc_str(eval_expression);
    }

    display_vlist(eval_expression.lparms, ws, mod + L"(L )", suppress_nesting, nest + L".");
    display_vlist(eval_expression.rparms, ws, mod + L"( R)", suppress_nesting, nest + L".");
        
    return; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_vlist() -- display passed-in vlist 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_vlist(const vlist_S& vlist, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    // display data from vlist header 

    std::wstring flags {}; 

    if (vlist.val_mixed      ) flags += L"val_mixed "      ; 
    if (vlist.val_block      ) flags += L"val_block "      ;
    if (vlist.val_expression ) flags += L"val_expression " ;
    if (vlist.val_vlist      ) flags += L"val_vlist "      ; 
    if (vlist.val_verbset    ) flags += L"val_verbset "    ; 
    if (vlist.val_typdef     ) flags += L"val_typdef "     ; 
    if (vlist.val_array      ) flags += L"val_array "      ;
    if (vlist.val_structure  ) flags += L"val_structure "  ;
    if (vlist.val_ref        ) flags += L"val_ref "        ; 
    if (vlist.val_unit       ) flags += L"val_unit "       ;
    if (vlist.val_boolean    ) flags += L"val_boolean "    ;
    if (vlist.val_int8       ) flags += L"val_int8 "       ;
    if (vlist.val_int16      ) flags += L"val_int16 "      ;
    if (vlist.val_int32      ) flags += L"val_int32 "      ;
    if (vlist.val_int64      ) flags += L"val_int64 "      ; 
    if (vlist.val_uint8      ) flags += L"val_uint8 "      ;
    if (vlist.val_uint16     ) flags += L"val_uint16 "     ;
    if (vlist.val_uint32     ) flags += L"val_uint32 "     ;
    if (vlist.val_uint64     ) flags += L"val_uint64 "     ; 
    if (vlist.val_float64    ) flags += L"val_float64 "    ; 
    if (vlist.val_string     ) flags += L"val_string "     ; 
    if (vlist.val_identifier ) flags += L"val_identifier " ;

    M_out(L"%s %s%s -- %|80t| vlist: value_ct=%d  kw_ct=%d  %|125t| -- loc= %s -- flags= <%s>") % ws % nest % mod % vlist.value_ct % vlist.kw_ct % vlist_loc_str(vlist) % flags;   


    // display positional values, if any

    if (vlist.value_ct > 0)
    {
        auto i = 0; 
        for (const auto& value : vlist.values)
            display_value(value, ws, mod + L"[" + std::to_wstring(i++) + L"] ", suppress_nesting, nest + L".");     
    }


    // display evaluated/unevaluated keyword values, if any

    if (vlist.eval_kws.size() > 0)                    // display evaluated keywords, if any
    {
        for (const auto& elem : vlist.eval_kws)
            display_value(elem.second, ws, mod + L"[" + elem.first + L":] ", suppress_nesting, nest + L".");     
    }
    else                                              // no evaluated keywords 
    {
        if (vlist.keywords.size() > 0)                // display unevaluated keywords, if any
        {
            for (const auto& elem : vlist.keywords)
            {   
                if (elem.name.ty == type_E::string) 
                    display_value(elem.value, ws, mod + L"[<" + elem.name.string + L":>] ", suppress_nesting, nest + L"."); 
                else
                    display_value(elem.value, ws, mod + L"[<?expression?>:] "             , suppress_nesting, nest + L"."); 
            }
        }
    }

    return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_value() -- display passed-in value 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_value(const value_S& value, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    
    // construct flags string
    // ----------------------

    std::wstring flags {}; 

    std ::wstring ix = std::to_wstring(value.token_ix1) + L":" + std::to_wstring(value.token_ix2); 

         if (value.ty == type_E::none        ) flags = L"none"         ; 
    else if (value.ty == type_E::no_value    ) flags = L"no_value "    ;
    else if (value.ty == type_E::identifier  ) flags = L"identifier "  ;
    else if (value.ty == type_E::verbname    ) flags = L"verbname "    ;
    else if (value.ty == type_E::keyname     ) flags = L"keyname "     ;
    else if (value.ty == type_E::vlist       ) flags = L"vlist ["      + fmt_ptr(value.vlist_sp.get()       ) + L"] "  ;
    else if (value.ty == type_E::expression  ) flags = L"expression (" + fmt_ptr(value.expression_sp.get()  ) + L") "  ;
    else if (value.ty == type_E::block       ) flags = L"block {"      + fmt_ptr(value.block_sp.get()       ) + L"} "  ;
    else if (value.ty == type_E::verbset     ) flags = L"verbset <"    + fmt_ptr(value.verbset_sp.get()    ) + L"> "  ;
    else if (value.ty == type_E::typdef      ) flags = L"typedef <"    + fmt_ptr(value.typdef_sp.get()      ) + L"> "  ;
    else if (value.ty == type_E::ref         ) flags = L"ref <"        + fmt_ptr(value.ref_sp.get()         ) + L"> "  ;
    else if (value.ty == type_E::array       ) flags = L"array <"      + fmt_ptr(value.buffer_sp.get()      ) + L"> "  ;
    else if (value.ty == type_E::structure   ) flags = L"structure <"  + fmt_ptr(value.buffer_sp.get()      ) + L"> "  ;
    else if (value.ty == type_E::unit        ) flags = L"unit "        ;
    else if (value.ty == type_E::boolean     ) flags = L"boolean "     ;
    else if (value.ty == type_E::int8        ) flags = L"int8 "        ; 
    else if (value.ty == type_E::int16       ) flags = L"int16 "       ; 
    else if (value.ty == type_E::int32       ) flags = L"int32 "       ; 
    else if (value.ty == type_E::int64       ) flags = L"int64 "       ; 
    else if (value.ty == type_E::uint8       ) flags = L"uint8 "       ; 
    else if (value.ty == type_E::uint16      ) flags = L"uint16 "      ; 
    else if (value.ty == type_E::uint32      ) flags = L"uint32 "      ; 
    else if (value.ty == type_E::uint64      ) flags = L"uint64 "      ;
    else if (value.ty == type_E::float32     ) flags = L"float32 "     ; 
    else if (value.ty == type_E::float64     ) flags = L"float64 "     ; 
    else if (value.ty == type_E::string      ) flags = L"string "      ; 
    else if (value.ty == type_E::error       ) flags = L"error "       ; 
    else if (value.ty == type_E::special     ) flags = L"special "     ; 
    else                                       flags = L" ???unknown value type???  " ; 
 
    M__(M_out(L"display_value() -- point 1 -- value.ty = %S") % type_str(value.ty));


    // display value of data for this value
    // ------------------------------------

    if      (value.ty == type_E::boolean )                                                                                                 
        M_out(L"%s %s%s %|80t|-- boolean    = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.boolean    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
    else if (value.ty == type_E::int8    )                                                                                                 
        M_out(L"%s %s%s %|80t|-- int8       = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.int8       % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::int16   )                                                                                                 
        M_out(L"%s %s%s %|80t|-- int16      = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.int16      % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::int32   )                                                                                                 
        M_out(L"%s %s%s %|80t|-- int32      = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.int32      % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::int64   )                                                                                                 
        M_out(L"%s %s%s %|80t|-- int64      = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.int64      % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if  (value.ty == type_E::uint8   )                                                                                                
        M_out(L"%s %s%s %|80t|-- uint8      = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.uint8      % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::uint16  )                                                                                                 
        M_out(L"%s %s%s %|80t|-- uint16     = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.uint16     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::uint32  )                                                                                                 
        M_out(L"%s %s%s %|80t|-- uint32     = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.uint32     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::uint64  )                                                                                                 
        M_out(L"%s %s%s %|80t|-- uint64     = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.uint64     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::float32 )                                                                                                 
        M_out(L"%s %s%s %|80t|-- float32    = %.16f %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.float32    % fmt_ptr(&value) % flags % ix % value_loc_str(value);
                                                                                                                                                                              
    else if (value.ty == type_E::float64 )                                                                                                 
        M_out(L"%s %s%s %|80t|-- float64    = %.16f %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.float64    % fmt_ptr(&value) % flags % ix % value_loc_str(value);
                                                                                                                                          
    else if (value.ty == type_E::string  )                                                                                                 
        M_out(L"%s %s%s %|80t|-- string     = «%s»  %|125t| -- addr=%S  flags = <%s>   ix = %s -- %s")       % ws % nest % mod % value.string     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::identifier  )                                                                                             
        M_out(L"%s %s%s %|80t|-- identifier = «%s»  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.string     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::verbname    )                                                                                             
        M_out(L"%s %s%s %|80t|-- verbname   = «%s»  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.string     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::keyname     )                                                                                             
        M_out(L"%s %s%s %|80t|-- keyname    = «%s»  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.string     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::vlist) 
    {
        M_out(L"%s %s%s %|80t|-- nested vlist %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")               % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
        display_vlist(*(value.vlist_sp), ws, mod, suppress_nesting, nest);   
    }
 
    else if (value.ty == type_E::expression) 
    {
        M_out(L"%s %s%s %|80t|-- nested expression  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")              % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 

        if (!suppress_nesting)
            display_expression(*(value.expression_sp), ws, mod, suppress_nesting, nest);  
    }
 
    else if (value.ty == type_E::block)
    { 
        M_out(L"%s %s%s %|80t|-- nested block %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")               % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s %|80t|-- nested block -- display suppressed")                                    % ws % nest % mod;
        else
            display_block(*(value.block_sp), ws, mod, true, suppress_nesting, nest);
    }
 
    else if (value.ty == type_E::verbset)
    { 
        M_out(L"%s %s%s %|80t|-- nested verbdef %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")             % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s %|80t|-- nested verbdef -- display suppressed")                                  % ws % nest % mod;
        else
            display_verbset( *(value.verbset_sp) );  
    }
     
    else if (value.ty == type_E::typdef)
    { 
        M_out(L"%s %s%s %|80t|-- nested typedef %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")             % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s -- %|80t|-- nested typedef -- display suppressed")                               % ws % nest % mod;
        else
            display_typdef(L"", *(value.typdef_sp), L"" );                                           // empty type name, typedef_S, nest string (empty)  
    }
    
    else if (value.ty == type_E::ref)
    { 
        M_out(L"%s %s%s %|80t|-- nested ref %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                 % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s -- %|80t| nested ref -- display suppressed")                                     % ws % nest % mod;
        else
            display_ref(*(value.ref_sp));                                                           
    }
  
    else if (value.ty == type_E::array)
    { 
        M_out(L"%s %s%s %|80t|-- nested array %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")              % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s -- %|80t| nested typedef/buffer for array -- display suppressed")               % ws % nest % mod;
        else
        {
            display_typdef(L"array typedef", *(value.typdef_sp),      L"    " );                     // typedef_S, nest string (empty)      
            display_buffer(L"array buffer" , *(value.buffer_sp), 256, L"    " );                     // display (partial) data from buffer  
        }
    }

    else if (value.ty == type_E::structure)
    { 
        M_out(L"%s %s%s %|80t|-- nested structure %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")           % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s %|80t|-- nested typedef/buffer for structure -- display suppressed")             % ws % nest % mod;
        else
        {
            display_typdef(L"structure typedef", *(value.typdef_sp),      L"    " );                 // typedef_S, nest string (empty)      
            display_buffer(L"structure buffer" , *(value.buffer_sp), 256, L"    " );                 // display (partial) data from buffer  
        }                                                                                     
    }
   
    else if (value.ty == type_E::unit) 
        M_out(L"%s %s%s %|80t|-- unit     %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);  
    
    else if (value.ty == type_E::no_value) 
        M_out(L"%s %s%s %|80t|-- no_value %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 

    else if (value.ty == type_E::none) 
        M_out(L"%s %s%s %|80t|-- none     %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 

    else if (value.ty == type_E::error) 
        M_out(L"%s %s%s %|80t|-- error    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);  
   
    else if (value.ty == type_E::special) 
        M_out(L"%s %s%s %|80t|-- special  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);  

    else
        M_out(L"%s %s%s %|80t|-- ??unknown value type(%d)?? %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s") % ws % nest % mod % (int)(value.ty)  % fmt_ptr(&value) % flags % ix % value_loc_str(value);  

    return; 
}
M_endf
 


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_results() -- display passed-in results 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_results(const results_S& results, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    // display results flags and string

    std::wstring flags { }; 

    if (results.multiple_results             ) flags += L"multiple_results "           ; 
    if (results.no_results                   ) flags += L"no_results "                 ; 
    if (results.re_eval_expression_results   ) flags += L"re_eval_expression_results " ;
    if (results.builtin_verb_results         ) flags += L"builtin_verb_results "       ;
    if (results.special_results              ) flags += L"special_results "            ; 
    if (results.error                        ) flags += L"error "                      ; 
    if (results.break_flag                   ) flags += L"break_flag "                 ;
    if (results.continue_flag                ) flags += L"continue_flag "              ;
    if (results.quit_flag                    ) flags += L"quit_flag "                  ; 
    if (results.end_flag                     ) flags += L"end_flag "                   ;
    if (results.leave_flag                   ) flags += L"leave_flag "                 ;
    if (results.goto_flag                    ) flags += L"goto_flag "                  ;
    if (results.xctl_flag                    ) flags += L"xctl_flag "                  ;
    if (results.return_flag                  ) flags += L"return_flag "                ;
    if (results.throw_flag                   ) flags += L"throw_flag "                 ;
    if (results.skip_flag                    ) flags += L"skip_flag "                  ;

     M_out(L"%s %s%s %|80t|-- results.flags = <%s>"  ) % ws % nest % mod % flags;
     M_out(L"%s %s%s %|80t|-- results.str   = \"%S\"") % ws % nest % mod % results.str;

    // display the value_S structure imbedded in results_S

    display_value(results, ws, mod, suppress_nesting, nest);    
    return; 
}
M_endf



    

////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_buffer() -- display length and (partial) contents of buffer_C object 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_buffer(const std::wstring& ws, const buf8_T& buffer, size_t max_bytes, const std::wstring& indent) try
{
    // put out heading line, if caller provided one

    if (ws.size() > 0)
        M_out(L"\n                  %S%S%80t -- buffer length = %d = 0x%04X  (displayed bytes = %d) -- addr=%S") % indent % ws % buffer.sz1 % buffer.sz1 % std::min(max_bytes, buffer.sz1) % fmt_ptr(&buffer); 


    // display buffer data -- blank lines before and after -- max bytes to display is set by caller

    M_out(L" "); 
    show_hex( (void *)(buffer.p1),  std::min(buffer.sz1, max_bytes), indent + L"                 ", false );  // false = suppress heading in show_hex()
    M_out(L" ");

    return; 
}
M_endf 
  


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_verbset()   -- displays verb definition details  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper functions for display_verbset()

static std::wstring str_parmtype(const parmtype_S& parmtype) try
{
    std::wstring parm_flags { };

    if (parmtype.int64_range             )   parm_flags += L"int64_range  "           ;    
    if (parmtype.float64_range           )   parm_flags += L"float64_range  "         ;
    if (parmtype.eval.no_eval_ident      )   parm_flags += L"no_eval_ident  "         ;
    if (parmtype.eval.no_eval_expression )   parm_flags += L"no_eval_expression  "    ;
    if (parmtype.eval.no_eval_vlist      )   parm_flags += L"no_eval_vlist  "         ;
    if (parmtype.eval.no_eval_ref        )   parm_flags += L"no_eval_ref  "           ;
    if (parmtype.anything_ok             )   parm_flags += L"anything_ok  "           ;
    if (parmtype.nval_ok                 )   parm_flags += L"nval_ok  "               ; 
    if (parmtype.unit_ok                 )   parm_flags += L"unit_ok  "               ; 
    if (parmtype.int8_ok                 )   parm_flags += L"int8_ok  "               ; 
    if (parmtype.int16_ok                )   parm_flags += L"int16_ok  "              ; 
    if (parmtype.int32_ok                )   parm_flags += L"int32_ok  "              ; 
    if (parmtype.int64_ok                )   parm_flags += L"int64_ok  "              ; 
    if (parmtype.uint8_ok                )   parm_flags += L"uint8_ok  "              ; 
    if (parmtype.uint16_ok               )   parm_flags += L"uint16_ok  "             ; 
    if (parmtype.uint32_ok               )   parm_flags += L"uint32_ok  "             ; 
    if (parmtype.uint64_ok               )   parm_flags += L"uint64_ok  "             ; 
    if (parmtype.float32_ok              )   parm_flags += L"float32_ok  "            ;
    if (parmtype.float64_ok              )   parm_flags += L"float64_ok  "            ; 
    if (parmtype.string_ok               )   parm_flags += L"string_ok  "             ; 
    if (parmtype.verbname_ok             )   parm_flags += L"verbname_ok  "           ; 
    if (parmtype.check_local_env_only    )   parm_flags += L"ck_local_env  "          ; 
    if (parmtype.check_global_env_only   )   parm_flags += L"ck_global_env  "         ;
    if (parmtype.raw_ident_ok            )   parm_flags += L"raw_ident_ok  "          ;
    if (parmtype.var_ident_ok            )   parm_flags += L"var_ident_ok  "          ; 
    if (parmtype.const_ident_ok          )   parm_flags += L"const_ident_ok  "        ; 
    if (parmtype.undef_ident_ok          )   parm_flags += L"undef_ident_ok  "        ;
    if (parmtype.vlist_ok                )   parm_flags += L"vlist_ok  "              ;                 
    if (parmtype.expression_ok           )   parm_flags += L"expression_ok  "         ; 
    if (parmtype.block_ok                )   parm_flags += L"block_ok  "              ;
    if (parmtype.verbset_ok              )   parm_flags += L"verbset_ok  "            ;
    if (parmtype.typdef_ok               )   parm_flags += L"typdef_ok  "             ;
    if (parmtype.array_ok                )   parm_flags += L"array_ok  "              ;
    if (parmtype.structure_ok            )   parm_flags += L"structure_ok  "          ;
    if (parmtype.lvalue_ref_ok           )   parm_flags += L"lvalue_ref_ok  "         ;
    if (parmtype.rvalue_ref_ok           )   parm_flags += L"rvalue_ref_ok  "         ;
    
    return parm_flags; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_parmtype(const parmtype_S& parmtype, const std::wstring& ws, const std::wstring& nest) try
{
    // display summary info for this parmtype_S

    M__(M_out(L"display_parmtype() called -- ws = «%s»") % ws;)


    // gather up int64 and float64 ranges

    std::wstring ranges {};
    std::wstring minmax {};

    if (parmtype.int64_range)
        ranges += L" [" + std::to_wstring(parmtype.int64_min  )  + L" to " + std::to_wstring(parmtype.int64_max  ) + L"] ";

    if (parmtype.float64_range)
        ranges += L" [" + std::to_wstring(parmtype.float64_min)  + L" to " + std::to_wstring(parmtype.float64_max) + L"] ";


    // make printable string with keyword min/max counts (if not 0-0 = optional single keyword) 

    if ((parmtype.kw_min_ct != 0) || (parmtype.kw_max_ct != 0) )
    {
        minmax = L" min/max= " + std::to_wstring(parmtype.kw_min_ct) + L"/";

        if ( (parmtype.kw_max_ct == -1) || (parmtype.kw_max_ct >= 1000000000000) )
            minmax += L"N";
        else
            minmax += std::to_wstring(parmtype.kw_max_ct);
    }


    // put out one line with parmtype info

    M_out(L"%31t--%s%s%s%s %125t-- %s") % nest % ws % minmax % ranges % str_parmtype(parmtype); 


    // display any attached plist, with individual sub-parm requirement info 

    if (parmtype.plist_sp.get() != nullptr)
    {
        display_plist(*(parmtype.plist_sp), ws + L": nested plist", nest + L" ");     
    }

    return;
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_plist(const plist_S& plist, const std::wstring& ws, const std::wstring& nest) try
{
    M__(M_out(L"display_plist() called -- ws = «%s»") % ws;)

    std::wstring parm_flags { }; 

    if (plist.no_check_keywords          )   parm_flags += L"no_chk_keywords  "       ; 
    if (plist.no_check_keyword_names     )   parm_flags += L"no_chk_kw_names  "       ;
    if (plist.no_check_positional        )   parm_flags += L"no_chk_positional  "     ;
    if (plist.eval.no_eval_ident         )   parm_flags += L"no_eval_ident  "         ;                 
    if (plist.eval.no_eval_expression    )   parm_flags += L"no_eval_expression  "    ; 
    if (plist.eval.no_eval_vlist         )   parm_flags += L"no_eval_vlist  "         ; 
    if (plist.eval.no_eval_ref           )   parm_flags += L"no_eval_ref  "           ; 

    std::wstring max_str{ }; 

    if ( (plist.max_ct == -1) || (plist.max_ct >= 1000000000000) ) 
        max_str = L"N";
    else
        max_str = std::to_wstring(plist.max_ct);


    M_out(L"%31t--%s%s: parms min/max=%3d/%-3s  keywords=%-3d %125t-- %s") 
      % nest
      % ws
      % plist.min_ct 
      % max_str 
      % plist.keywords.size()
      % parm_flags;


    // display any keyword conflict sets for this plist 

    if (plist.conflicts.size() > 0)
    {
        int i {0}; 

        for (const auto& cset : plist.conflicts)   // loop through vector of conflict sets 
        {
            std::wstring kws {};                   // list of keywords in conflict set 
            
            for(const auto& kw : cset)             // loop through each keyword name in conflict set 
                kws += kw + L":   ";               // add keyword name to printable string              
      
            M_out(L"%31t--%s%s: kw conflict set [%d] : %125t-- %s") % nest % ws % i % kws; 
            i++; 
        }    
    }


    // display any keyword choice sets for this plist 

    if (plist.choices.size() > 0)
    {
        int i {0}; 

        for (const auto& cset : plist.choices)     // loop through vector of choice sets 
        {
            std::wstring kws {};                   // list of keywords in choice set 
            
            for(const auto& kw : cset)             // loop through each keyword name in choice set 
                kws += kw + L":   ";               // add keyword name to printable string              
      
            M_out(L"%31t--%s%s: kw choice(s) set [%d] : %125t-- %s") % nest % ws % i % kws; 
            i++; 
        }    
    }
   

    // display any keyword match sets for this plist 

    if (plist.matches.size() > 0)
    {
        int i {0}; 

        for (const auto& mset : plist.matches)     // loop through vector of match sets 
        {
            std::wstring kws {};                   // list of keywords in match set 
            
            for(const auto& kw : mset)             // loop through each keyword name in conflict set 
                kws += kw + L":   ";              // add keyword name to printable string              
      
            M_out(L"%31t--%s%s: kw match set    [%d] : %125t-- %s") % nest % ws % i % kws; 
            i++; 
        }    
    }


    // individually display and positional parms that have parmtype_S descriptions for this plist

    if (plist.values.size() > 0)
    {
        int i {0};  

        for (const auto& elem : plist.values) 
        {                        /*  01234567890123456*/
            display_parmtype(elem, L"            parm[" + std::to_wstring(i) + L"]", nest + L" ");
            i++; 
        }
    }


    // individually display any keywords that are present for this plist 

    if (plist.keywords.size() > 0)
    {
        for (const auto& elem : plist.keywords)
                                        /*  01234567890123456*/    
            display_parmtype(elem.second, L"            keyword = " + elem.first + L":", nest + L" ");
    }
        
    return;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_argvar(const argvar_S& argvar, const std::wstring& ws) try
{
    M__(M_out(L"display_argvar() called -- ws = «%s»") % ws;)

    std::wstring arg_str {   };
    int var_ct           { 0 }; 


    // add variables for positional parms to string for display

    for (const auto var_name: argvar.pos)
    {
        arg_str += var_name; 
        arg_str += std::wstring { L" " };
        var_ct++;  
    }

    if (var_ct > 0)                           
        arg_str += std::wstring { L"   "};          // add extra space between positional and keyword variable names


    // add keyword:variable parms to string for display

    for (const auto elem: argvar.key)              // elem.first -- string = keyword name,  elem.second -- string = variable name
    {
        arg_str += elem.first;
        arg_str += std::wstring { L":" };
        arg_str += elem.second;
        arg_str += std::wstring { L" " };
        var_ct++;  
    }


    // display printable string with argvars, if any were present

    if (var_ct > 0)
        M_out(L"%31t--%S: argvars = %S") % ws % arg_str;
 
    return;
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////
// display_verbset()
//////////////////////////////////////////////////////////////////////////////////////////////////////

void display_verbset(const verbset_S& verbset) try
{
    // put out first line with verbdef global info 
    // -------------------------------------------

    std::wstring verbset_flags      { }; 

    if (verbset.right_associate                  )  verbset_flags += L"right_associate  "             ;
    if (verbset.left_associate                   )  verbset_flags += L"left_associate  "              ;
    if (verbset.custom_eval                      )  verbset_flags += L"**CUSTOM_EVAL**  "             ;
    if (verbset.has_builtin                      )  verbset_flags += L"has builtin  "                 ;
    if (verbset.left_eval.no_eval_ident          )  verbset_flags += L"left.no_eval_ident  "          ;
    if (verbset.left_eval.no_eval_expression     )  verbset_flags += L"left.no_eval_expression  "     ;
    if (verbset.left_eval.no_eval_vlist          )  verbset_flags += L"left.no_eval_vlist  "          ;
    if (verbset.left_eval.no_eval_ref            )  verbset_flags += L"left.no_eval_ref  "            ;
  //if (verbset.left_eval.no_eval_kw_ident       )  verbset_flags += L"left.no_eval_kw_ident  "       ;
  //if (verbset.left_eval.no_eval_kw_expression  )  verbset_flags += L"left.no_eval_kw_expression  "  ;
  //if (verbset.left_eval.no_eval_kw_vlist       )  verbset_flags += L"left.no_eval_kw_vlist  "       ;
  //if (verbset.left_eval.no_eval_kw_ref         )  verbset_flags += L"left.no_eval_kw_ref  "         ;
    if (verbset.right_eval.no_eval_ident         )  verbset_flags += L"right.no_eval_ident  "         ;
    if (verbset.right_eval.no_eval_expression    )  verbset_flags += L"right.no_eval_expression  "    ;
    if (verbset.right_eval.no_eval_vlist         )  verbset_flags += L"right.no_eval_vlist  "         ;
    if (verbset.right_eval.no_eval_ref           )  verbset_flags += L"right.no_eval_ref  "           ;
  //if (verbset.left_eval.no_eval_kw_ident       )  verbset_flags += L"left.no_eval_kw_ident  "       ;
  //if (verbset.left_eval.no_eval_kw_expression  )  verbset_flags += L"left.no_eval_kw_expression  "  ;
  //if (verbset.left_eval.no_eval_kw_vlist       )  verbset_flags += L"left.no_eval_kw_vlist  "       ;
  //if (verbset.left_eval.no_eval_kw_ref         )  verbset_flags += L"left.no_eval_kw_ref  "         ;

    M_out(L"%25t-- verbset: addr=%S  verb count = %d  priority=%-8d %125t-- flags: %S\n") 
          % fmt_ptr(&verbset)
          % verbset.verbs.size()
          % verbset.priority
          % verbset_flags
          ;


    // main loop to print out each verb definition in verbset
    // ------------------------------------------------------

    for (auto i = 0; i < verbset.verbs.size(); i++)
    {
        std::wstring verb_flags { }; 
  
        if (verbset.verbs.at(i).simplified_call     )   verb_flags += L"simplified_call  "             ;
        if (verbset.verbs.at(i).parms_same_type     )   verb_flags += L"parms_same_type  "             ;
        if (verbset.verbs.at(i).parms_some_required )   verb_flags += L"parms_some_required  "         ;
        if (verbset.verbs.at(i).parms_left_xor_right)   verb_flags += L"parms_left_xor_right  "        ;
        if (verbset.verbs.at(i).parms_not_both_sides)   verb_flags += L"parms_not_both_sides  "        ;
        if (verbset.verbs.at(i).percolate_all       )   verb_flags += L"percolate_all  "               ;
        if (verbset.verbs.at(i).lexical_scope       )   verb_flags += L"lexical_scope  "               ;
        if (verbset.verbs.at(i).dynamic_scope       )   verb_flags += L"dynamic_scope  "               ; 
#ifdef M_EXPOSE_SUPPORT
        if (verbset.verbs.at(i).dynall_scope        )   verb_flags += L"dynall_scope  "                ; 
#endif
        if (verbset.verbs.at(i).same_scope          )   verb_flags += L"same_scope  "                  ;
        if (verbset.verbs.at(i).no_scope            )   verb_flags += L"no_scope  "                    ;
        if (verbset.verbs.at(i).scope_defaulted     )   verb_flags += L"scope_defaulted  "             ;  
        if (verbset.verbs.at(i).is_builtin          )   verb_flags += L"(built-in verb)  "             ;  
    
        M_out(L"%28t-- verb[%-2d]  addr=%S  fcn_p=%S  verb_block_p=%S  init_block_p=%S  %125t-- persist-SF:%d  %S  info: \"%S\"") 
             % i
             % fmt_ptr(&(verbset.verbs.at(i)))
             % fmt_ptr(verbset.verbs.at(i).fcn_p)
             % fmt_ptr(verbset.verbs.at(i).verb_block_sp.get())
             % fmt_ptr(verbset.verbs.at(i).init_block_sp.get())
             % ( verbset.verbs.at(i).persist_env_sp.get() == nullptr ? 0 : verbset.verbs.at(i).persist_env_sp->sernum )  
             % verb_flags
             % verbset.verbs.at(i).info
             ;
        

        // display left-side and right-side argvar_S structures for this verb
       
        display_argvar(verbset.verbs.at(i).lvars, L" left-side " );
        display_argvar(verbset.verbs.at(i).rvars, L" right-side" );
       
       
        // display left-side and right-side plists for this verb
       
        display_plist(verbset.verbs.at(i).lparms, L" left-side " );
        display_plist(verbset.verbs.at(i).rparms, L" right-side" );

        M_out(L"");
    }

    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_typdef()   -- displays type definition details  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_typdef(const std::wstring& desc, const typdef_S& ty, const std::wstring& indent_str) try
{
    // put out line with typdef main info
 
    M_out(L"                  %S%S%80t-- kind=%d=%S%102t tsize=%-2d (%04X)  acount=%d  fdefs.size=%d -- addr=%S") 
         % indent_str
         % desc 
         % (int)(ty.kind)
         % type_str(ty.kind)
         % ty.tsize
         % ty.tsize
         % ty.acount
         % ty.fdefs.size()
         % fmt_ptr(&ty)
         ;

    // display nested array element types, or structure field types

    if (ty.atype_sp.get() != nullptr)
         display_typdef(L"array element:", *(ty.atype_sp), indent_str + L"    ");

    if (ty.fnames.size() > 0)
    {
        for (auto& elem : ty.fnames)     // elem.first = field name (std::wstring) , elem.second = uint64_t -- index into fdefs vector 
        { 
            if (ty.fdefs.size() > elem.second )
                display_fieldef(elem.first, ty.fdefs.at(elem.second), indent_str + L"    ");
            else
                M_out(L"%Sunexpected error -- ty.fdefs too small to display this field's fieldef_S") % indent_str;
        }
    }

    return; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////

void display_fieldef(const std::wstring& field_name, const fieldef_S& fd, const std::wstring& indent_str) try
{
    // put out line with fieldef/typdef main info  

   M_out(L"%S%S %80t-- field offset=%-4d (%04X)  typedef info:") 
        % indent_str 
        % field_name 
        % fd.offset
        % fd.offset
        ;

    if (fd.ftype_sp.get() != nullptr) 
        display_typdef(L"", *(fd.ftype_sp), indent_str + L"    ");
    else
        M_out(L"%Sunexpected error -- typdef_sp = not initialized") % indent_str;

    return; 
}
M_endf

 

////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_ref()   -- displays ref details  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_ref(const ref_S& ref) try
{
    // get printable flags string

    std::wstring flags { }; 

    if (ref.in_buffer   )        flags += L"in_buffer  " ;
    if (ref.is_lvalue   )        flags += L"is_lvalue  " ;
    if (ref.is_rvalue   )        flags += L"is_rvalue  " ;
    if (ref.auto_deref  )        flags += L"auto_deref  ";


    // put out line with ref main info
 
    M_out(L"%80t-- offset=%d (0x%04X)  flags=<%S>   typdef_sp=%S    value_sp=%S") 
         % ref.offset
         % ref.offset
         % flags 
         % fmt_ptr(ref.typdef_sp.get())
         % fmt_ptr(ref.value_sp.get())
         ;

    // display nested array element types, or structure field types

    if (ref.typdef_sp.get() != nullptr)
         display_typdef(L"ref -- associated typdef", *(ref.typdef_sp), L"");

    if (ref.value_sp.get() != nullptr)
         display_value(*(ref.value_sp), L"             ", L"ref -- associated value:", false, L"    ");

    return; 
}
M_endf








/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   symval_flag_str() -- creates printable string for symval_S flag bit settings  -- internal helper function
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static std::wstring symval_flag_str(const symval_S& symval) try
{
    std::wstring str    { };                    // start with empty string


    // add builtin prefix to (empty) string

    if (symval.is_builtin)
        str += L"built-in "; 
         

    // add basic type to string

    if (symval.is_verbset)
    {
        str += L"verbset ";
    }
    else if (symval.is_typdef)
    {
        str += L"typedef ";
    }
    else                                        // must be var or const 
    {  
        if (symval.is_const) 
            str += L"const "; 
        else
            str += L"var ";
    }
             

    // add alias and exposed flags (assume string is not empty at this point

    std::wstring spacer = { };                  // spacing string between output words -- starts off empty

    if (symval.is_alias     )     str += L" is_alias  "     ;
#ifdef M_EXPOSE_SUPPORT
    if (symval.is_exposed   )     str += L" is_exposed  "   ;
#endif
    if (symval.no_shadow    )     str += L" no_shadow  "    ;
    if (symval.no_undefine  )     str += L" no_undefine  "  ;
    if (symval.no_update    )     str += L" no_update  "    ;  

    return str; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   sf_flag_str()     -- returns a string with stack frame and environment flag settings (and SF verbname)  -- internal helper function
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
static
std::wstring sf_flag_str(const frame_S *frame_p) try
{
    std::wstring sf_flags      { };
    std::wstring environ_flags { };
    std::wstring verbname      { };
#ifdef M_EXPOSE_SUPPORT
    if (frame_p->exposes_done              ) sf_flags      += L"SF-exposes_done  "       ;
#endif
    if (frame_p->is_main                   ) sf_flags      += L"SF-is_main  "            ;
    if (frame_p->is_preprocess             ) sf_flags      += L"SF-is_preprocess  "      ;
    if (frame_p->is_persistent             ) sf_flags      += L"SF-is_persistent  "      ; 
    if (frame_p->is_verbmain               ) sf_flags      += L"SF-is_verbmain  "        ;  
    if (frame_p->is_frameblock             ) sf_flags      += L"SF-is_frameblock  "      ; 
    if (frame_p->is_parmsonly              ) sf_flags      += L"SF-is_parmsonly  "       ; 
    if (frame_p->shared_environ            ) sf_flags      += L"SF-shared_environ  "     ;
    if (frame_p->lexical_scope             ) sf_flags      += L"SF-lexical_scope  "      ;
    if (frame_p->dynamic_scope             ) sf_flags      += L"SF-dynamic_scope  "      ;
#ifdef M_EXPOSE_SUPPORT
    if (frame_p->dynall_scope              ) sf_flags      += L"SF-dynall_scope  "       ;
#endif
    if (frame_p->no_scope                  ) sf_flags      += L"SF-no_scope  "           ;
    if (frame_p->same_scope                ) sf_flags      += L"SF-same_scope  "         ;

    if (frame_p->environ_p->is_global      ) environ_flags += L"ENV-is_global  "         ; 
    if (frame_p->environ_p->is_main        ) environ_flags += L"ENV-is_main  "           ; 
    if (frame_p->environ_p->is_preprocess  ) environ_flags += L"ENV-is_preprocess  "     ; 
    if (frame_p->environ_p->is_persistent  ) environ_flags += L"ENV-is_persistent  "     ; 
    if (frame_p->environ_p->is_verbmain    ) environ_flags += L"ENV-is_verbmain  "       ;
    if (frame_p->environ_p->is_frameblock  ) environ_flags += L"ENV-is_frameblock  "     ; 
    if (frame_p->environ_p->is_parmsonly   ) environ_flags += L"ENV-is_parmsonly  "      ; 

    M__(M_out(L"display_stack() -- verbname.size() = %d") % frame_p->verbname.size();)

    if (frame_p->verbname.size() > 0)
        verbname = std::wstring {L" verbname= <"} + frame_p->verbname +  std::wstring {L">"};

    return sf_flags + environ_flags + verbname;
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_stack()     -- displays chained stack frames starting at the passed-in one  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// helper function to display one stack frame
// ------------------------------------------

static
void display_stack_frame(const frame_S *frame_p) try
{
    if (frame_p->is_main)
    {
        M_out(  L"Main SF:%-10d -- use=%-2d addr=%p  parent-SF:%-7d child-SF:%-7d persist-SF:%-7d verbmain-SF:%-7d local-SF:%-7d search_SF=%-7d scope-SF:%-7d *environ-p:%-7d -- verbs called=%-10d symbol_ct=%-5d -- flags: %S" )
             %  frame_p->serial 
             %  frame_p->self_wp.use_count()
             % frame_p 
             % (frame_p->parent_sp.get() == nullptr ? 0 :            frame_p->parent_sp     -> serial  ) 
             % (frame_p->child_p         == nullptr ? 0 :            frame_p->child_p       -> serial  )
             % (frame_p->persist_sf_p    == nullptr ? 0 :            frame_p->persist_sf_p  -> serial  )
             % (frame_p->verbmain_sf_p   == nullptr ? 0 :            frame_p->verbmain_sf_p -> serial  )
             % (frame_p->local_sf_p      == nullptr ? 0 :            frame_p->local_sf_p    -> serial  )
             % (frame_p->search_sf_p     == nullptr ? 0 :            frame_p->search_sf_p   -> serial  )
             % (frame_p->scope_sp.get()  == nullptr ? 0 :            frame_p->scope_sp      -> serial  )
             % (frame_p->environ_p       == nullptr ? 0 :            frame_p->environ_p     -> sernum  )
             %  frame_p->verb_eval_ct
             %  frame_p->environ_p->symbols.size()
             %  sf_flag_str(frame_p)
             ; 
    }
    else
    {
        M_out(  L"Nest SF:%-10d -- use=%-2d addr=%p  parent-SF:%-7d child-SF:%-7d persist-SF:%-7d verbmain-SF:%-7d local-SF:%-7d search_SF=%-7d scope-SF:%-7d *environ-p:%-7d -- verbs called=%-10d symbol_ct=%-5d -- flags: %S" )
             %  frame_p->serial 
             %  frame_p->self_wp.use_count()
             % frame_p 
             % (frame_p->parent_sp.get() == nullptr ? 0 :            frame_p->parent_sp     -> serial  ) 
             % (frame_p->child_p         == nullptr ? 0 :            frame_p->child_p       -> serial  )
             % (frame_p->persist_sf_p    == nullptr ? 0 :            frame_p->persist_sf_p  -> serial  )
             % (frame_p->verbmain_sf_p   == nullptr ? 0 :            frame_p->verbmain_sf_p -> serial  )
             % (frame_p->local_sf_p      == nullptr ? 0 :            frame_p->local_sf_p    -> serial  )
             % (frame_p->search_sf_p     == nullptr ? 0 :            frame_p->search_sf_p   -> serial  )
             % (frame_p->scope_sp.get()  == nullptr ? 0 :            frame_p->scope_sp      -> serial  )
             % (frame_p->environ_p       == nullptr ? 0 :            frame_p->environ_p     -> sernum  )
             %  frame_p->verb_eval_ct
             %  frame_p->environ_p->symbols.size()
             %  sf_flag_str(frame_p)
             ;      
    }
}
M_endf



// display all active stack frames
// -------------------------------

void display_stack(const frame_S& frame) try
{
    const frame_S *frame_p  { &frame };                              // point to passed-in frame_S as starting point in stack display


    // 1st loop to display all stack frames in the upward scope -- follow upward scope chain from passed-in frame_S 
    // ------------------------------------------------------------------------------------------------------------

    M_out(L"display_stack() -- upward scope stack frame chain, starting with passed-in stack frame\n");

    while (frame_p != nullptr)
    {
        display_stack_frame(frame_p);                                // print out info for this stack frame


        // chain upward to next scope

        if (frame_p->scope_sp.use_count() == 0)                      // don't advance past main stack frame (which should have an uninitialized  upward pointer)
           frame_p = nullptr;                                        // no more frame_S's for this loop
        else                                                     
           frame_p = frame_p->scope_sp.get();                        // advance upward to next frame_S that has an environment (every frame_S has a synmbol table, for now)
    }


    // 2nd loop to display all active stack frames on the stack -- follow forward chain from main frame_S 
    // --------------------------------------------------------------------------------------------------
    
    M_out(L"\ndisplay_stack() -- active stack forward chain, starting with main stack frame\n");

   frame_p = get_main_frame();                                       // start at main frame_S

    while (frame_p != nullptr)
    {
        display_stack_frame(frame_p);                                // print out info for this stack frame 
        frame_p = frame_p->child_p;                                  // chain downward to child frame_S 
    }
       
    return; 
}
M_endf

 



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_all_vars() -- displays all variables currently in the environment (omits fixed verb definitions, but includes variables holding a verb definition)
////   display_vars()     -- display selected verbs
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_all_vars(const frame_S& frame_in) try
{
    int n = 0;                                     // accumulated variable counter

    frame_S *frame_p = frame_in.search_sf_p;       // point to incoming frame_S search starting environment


    M_out(L"--------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Upward scope stack frame chain from starting stack-frame:%d/%d") % frame_p->serial % frame_p->environ_p->sernum;


    // outer loop to display local environments for each nested frame_S -- chain backward using upward scope pointer
    // -------------------------------------------------------------------------------------------------------------

    do
    {
        M_out(L"------------------------------------------------------------------\n");
 
        if (frame_p->is_main)
            M_out( L"Main stack frame [%d/%d] %30t-- total number of identifiers = %d %122t -- flags: %S  -- list of variables:\n") 
                 % frame_p->serial % frame_p->environ_p->sernum % (frame_p->environ_p->symbols.size()) % sf_flag_str(frame_p);
        else
            M_out( L"Stack frame [%d/%d] %30t-- total number of identifiers = %d %122t -- flags: %S  -- list of variables:\n")
                 % frame_p->serial % frame_p->environ_p->sernum % (frame_p->environ_p->symbols.size()) % sf_flag_str(frame_p);
        
        for(const auto& elem : frame_p->environ_p->symbols)
        {
            std::wstring convar { }; 
      
            if ( !elem.second.is_alias )
            {
                if ( (elem.second.is_verbset) || (elem.second.is_typdef) )    
                    continue;                                                       // skip over any verb/type definitions in environment -- include all aliases (even to verbset or typdef)
            }

            M_out(L"n= %-3d ID= %-20s %79t (%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serno;
            display_value(*(elem.second.value_sp), L"                                                                  ", L"", false);
            n++;
        }

        M_out(L" ");

        if (frame_p->scope_sp.use_count() > 0)                                      // don't advance past main stack frame
           frame_p = frame_p->scope_sp->search_sf_p;                                // advance upward to next frame_S in this scope 
        else                                                                      
           frame_p = nullptr;                                                       // no more stack frames
    }                                                                             
    while (frame_p != nullptr);                                                     // stop loop after all stack frames are done 
                                                                                  

    // display global environment
    // --------------------------
         
    M_out(L"Global environment -- total number of identifiers = %d -- list of variables:\n") % get_global_environ()->symbols.size() ;

    for(const auto& elem : get_global_environ()->symbols) 
    {
        if (!elem.second.is_alias)
        {
            if ( (elem.second.is_verbset) || (elem.second.is_typdef) )
                continue;                                                             // skip over any verb/type definitions in environment (but do show aliases, even of verbdef or typdef)
        }

        M_out(L"n= %-3d ID= %-20s %79t (%S)   use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serno;
        display_value(*(elem.second.value_sp),     L"                                                                  ", L"", false);
        n++; 
    }   
      
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");

    return; 
}
M_endf


////////////////////////////////////////////////////////////////////////

void display_vars(const frame_S& frame, const vlist_S& vlist) try
{
    M_out(L"--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Selected variables/constants visible from stack-frame:%d/%d") % frame.serial % frame.environ_p->sernum ;
    M_out(L"--------------------------------------------------------------------------\n");


    // loop to look at value_S structures visible from this scope for passed-in verb-name 

    int n = 0; 
    for (const auto& value : vlist.values)
    {
        symval_S symval { };                                                                                                                                         // will cause use_ct() to be one more than expected
        auto grc = get_var(frame, value.string, symval);
    
        if (grc == 0)
        {
            M_out(L"n=%-3d ID= %-20s %79t (%S)  use_ct=%d  serial=%d") % n % value.string % symval_flag_str(symval) % (symval.value_sp.use_count() - 1) % symval.serno;  // correct for extra use_ct()
            display_value(*(symval.value_sp),    L"                                                                  ", L"", false); 
        }
        else
            M_out(L"n=%-3d ID= %-20s %79t -- variable is undefined or not visible from this stack frame") % n % value.string; 

        M_out(L" ");
        n++; 
    }

    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"); 
    return; 
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_all_verbsets() -- displays all verbs currently in the verb table 
////   display_verbsets()     -- displays subset of verbs  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_all_verbsets(const frame_S& frame_in, bool show_builtin, bool show_defined) try
{
    int n = 0;                                                         // accumulated verb counter
                                                                 
    frame_S *frame_p  = frame_in.search_sf_p;                          // point to search starting SF for incoming frame_S 


    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"List of all verbs visible from starting stack-frame:%d/%d") % frame_p->serial % frame_p->environ_p->sernum ; 
   

    // outer loop to display local environments for each nested frame_S (use scope_sp to chain backwards through the stack frames, some of which might not be on the active stack)
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    do
    {
        M_out(L"----------------------------------------------------------------\n");
    
        if (frame_p->is_main)
            M_out( L"Main stack frame [%d/%d] %30t-- total number of identifiers = %d %122t -- flags: %S  -- list of verbs:\n") 
                 % frame_p->serial 
                 % frame_p->environ_p->sernum
                 % frame_p->environ_p->symbols.size() 
                 % sf_flag_str(frame_p)
                 ;
        else
            M_out( L"Stack frame [%d/%d] %30t-- total number of identifiers = %d %122t -- flags: %S  -- list of verbs:\n")
                 % frame_p->serial 
                 % frame_p->environ_p->sernum
                 % frame_p->environ_p->symbols.size() 
                 % sf_flag_str(frame_p)
                 ;
        
        for(const auto& elem : frame_p->environ_p->symbols)
        {
            if (elem.second.is_verbset)                             // only look at verb definitions (not variables currently containing verb definitions)
            {
                if (
                    (   elem.second.is_builtin  && show_builtin )
                    ||
                    ( (!elem.second.is_builtin) && show_defined )
                   )
                {
                    M_out(L"n=%-3d ID=\"%S\" %25t(  %S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serno;
                    display_verbset(*(elem.second.value_sp->verbset_sp)); 
                 // M_out(L" ");
                    n++;    
                }
            }
        }


        // chain upward to next scope (if any)

        if (frame_p->scope_sp.use_count() == 0)                      // don't advance past main stack frame  (which should have an uninitialized  upward scope pointer)
           frame_p = nullptr;                                        // no more frame_S's for this loop
        else                                                     
           frame_p = frame_p->scope_sp.get();                        // advance upward to next frame_S that has an environment ??? (every frame_S has a symbol table, for now)
    }                                                            
    while (frame_p != nullptr);                                      // stop loop after all frame_S's are done 


    // display verbs in global environment
    // -----------------------------------
         
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Global environment -- total number of identifiers = %d -- list of verbs:") % (get_global_environ()->symbols.size()) ;
    M_out(L"------------------------------------------------------------------------------\n");

    for(const auto& elem : get_global_environ()->symbols) 
    {
        if (elem.second.is_verbset)                                  // only show verbs (not variables or constants currently containing a verb definition)
        {
            if (
                (   elem.second.is_builtin  && show_builtin )
                ||
                ( (!elem.second.is_builtin) && show_defined )
               )
            {
                M_out(L"n=%-3d ID=\"%S\" %25t(%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serno;
                display_verbset(*(elem.second.value_sp->verbset_sp)); 
                n++;
            }
        }
    }   
      
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");

    return; 
}
M_endf


////////////////////////////////////////////////////////////////////////

void display_verbsets(const frame_S& frame, const vlist_S& vlist) try
{
    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Selected verbs visible from stack-frame:%d/%d") % frame.serial % frame.environ_p->sernum;
    M_out(L"------------------------------------------------------------\n");


    // loop to look at verbdef_S structure visible from this scope for passed-in verb-name 

    int n = 0; 
    for (const auto& value : vlist.values)
    {
        symval_S symval { };                                                                                                                                       // will cause use_ct() to be one more than expected
        auto grc = get_verb(frame, value.string, symval);                                                                                                          // fills in symval
    
        if (grc == 0)
        {
            M_out(L"n=%-3d ID=\"%S\"%25t(%S)  use_ct=%d  serial=%d") % n % value.string % symval_flag_str(symval) % (symval.value_sp.use_count() - 1) % symval.serno;  // correct for extra use_ct()
            display_verbset(*(symval.value_sp->verbset_sp)); 
        }
        else
            M_out(L"n=%-3d ID=\"%S\"%25t-- verb is undefined or not visible from this stack frame") % n % value.string; 

        M_out(L" ");
        n++; 
    }

    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"); 
    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_all_typdefs() -- displays all verbs currently in the verb table 
////   display_typdefs()     -- displays subset of verbs  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_all_typdefs(const frame_S& frame_in, bool show_builtin, bool show_defined) try
{
    int n = 0;                                                         // accumulated verb counter
                                                                 
    frame_S *frame_p  = frame_in.search_sf_p;                          // point to search starting SF for incoming frame_S


    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"List of all typedefs visible from starting stack-frame:%d/%d") % frame_p->serial % frame_p->environ_p->sernum; 
   

    // outer loop to display local environments for each nested frame_S (use scope_sp to chain backwards through the stack frames, some of which might not be on the active stack)
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    do
    {
        M_out(L"-------------------------------------------------------------------\n");

        if (frame_p->is_main)
            M_out( L"Main stack frame [%d/%d] %30t-- total number of identifiers = %d %122t -- flags: %S  -- list of typedefs:\n") 
                 % frame_p->serial
                 % frame_p->environ_p->sernum 
                 % frame_p->environ_p->symbols.size() 
                 % sf_flag_str(frame_p)
                 ;
        else
            M_out( L"Stack frame [%d/%d] %30t-- total number of identifiers = %d %122t -- flags: %S  -- list of typedefs:\n")
                 % frame_p->serial
                 % frame_p->environ_p->sernum
                 % frame_p->environ_p->symbols.size() 
                 % sf_flag_str(frame_p)
                 ;
        
        for(const auto& elem : frame_p->environ_p->symbols)
        {
            std::wstring convar { }; 
      
            if (elem.second.is_typdef)                               // only look at type definitions (not variables currently containing type definitions) -- include aliases, though
            {
                if (
                    (   elem.second.is_builtin  && show_builtin )
                    ||
                    ( (!elem.second.is_builtin) && show_defined )
                   )
                {
                    M_out(L"n= %-3d ID= %-13s %78t (%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serno;
                    display_typdef(L"", *(elem.second.value_sp->typdef_sp), L""); 
                    M_out(L" ");
                    n++;    
                }
            }
        }


        // chain upward to next scope (if any)

        if (frame_p->scope_sp.use_count() == 0)                      // don't advance past main stack frame  (which should have an uninitialized  upward scope pointer)
           frame_p = nullptr;                                        // no more frame_S's for this loop
        else                                                     
           frame_p = frame_p->scope_sp.get();                        // advance upward to next frame_S that has an environment (every frame_S has a synmbol table, for now)
    }                                                            
    while (frame_p != nullptr);                                      // stop loop after all frame_S's are done 


    // display typdefs in global environment
    // -------------------------------------
         
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Global environment -- total number of identifiers = %d -- list of typedefs:") % (get_global_environ()->symbols.size()) ;
    M_out(L"---------------------------------------------------------------------------------\n");

    for(const auto& elem : get_global_environ()->symbols) 
    {
        if (elem.second.is_typdef)                                  // only show typdefs (not variables or constants currently containing a type definition) -- include aliases, though
        {
            if (
                (   elem.second.is_builtin  && show_builtin )
                ||
                ( (!elem.second.is_builtin) && show_defined )
               )
            {
                M_out(L"n= %-3d ID= %-13s %78t (%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serno;
                display_typdef(L"", *(elem.second.value_sp->typdef_sp), L""); 
                n++;
            }
        }
    }   
      
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");

    return; 
}
M_endf


////////////////////////////////////////////////////////////////////////

void display_typdefs(const frame_S& frame, const vlist_S& vlist) try
{
    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Selected typedefs visible from stack-frame:%d/%d") % frame.serial % frame.environ_p->sernum ;
    M_out(L"----------------------------------------------------------\n");


    // loop to look at typdef_S structure visible from this scope for passed-in type-name 

    int n = 0; 
    for (const auto& value : vlist.values)
    {
        symval_S symval { };                                                                                                                                                     // will cause use_ct() to be one more than expected
        auto grc = get_typdef(frame, value.string, symval);
    
        if (grc == 0)
        {
            M_out(L"n=%-3d ID= %-13s %78t (%S)  use_ct=%d  serial=%d") % n % value.string % symval_flag_str(symval) % (symval.value_sp.use_count() - 1) % symval.serno;    // correct for extra use count
            display_typdef(L"", *(symval.value_sp->typdef_sp), L""); 
        }
        else
            M_out(L"n=%-3d ID= %s%25t-- typedef is undefined or not visible from this stack frame") % n % value.string; 

        M_out(L" ");
        n++; 
    }

    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"); 
    return; 
}
M_endf

  





//_________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""