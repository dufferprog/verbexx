// ex_intoken.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ==============
////     ex_intoken.cpp -- functions, etc. token_stream_C class
////     ==============
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
#include "h_ex_lex.h"




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       local static items
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace static_N
{
static cache_S  id_cache             { };       // cached location ID strings (BUG -- ??????? need to make sure this doesn't get too large ?????????)

static uint64_t last_char_source_id  {0};       // saved to provide good location info for next END token
static uint32_t last_char_lineno     {0};       // saved to provide good location info for next END token
static size_t   last_char_linepos    {0};       // saved to provide good location info for next END token

static uint64_t raw_token_count      {0};       // raw token counter 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////    
////       Global functions to get counters 
////     
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


uint64_t get_raw_token_count(void     ) {return static_N::raw_token_count;       }




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////      +------------------------------------------------+
////      | Global function to display strings in ID cache | 
////      +------------------------------------------------+
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void display_id_cache() try
{
    M_out(L"\n------------------------------------------------------------------------------------------------------");
    M_out(L"display_id_cache(): Source ID string cache -- string_ct = %d :") % static_N::id_cache.string_ct; 
    
    for (auto i = 0; i < static_N::id_cache.string_ct; i++)        
       M_out(L"                  : id[%d] = %|10t|«%s»") % i % static_N::id_cache.strings.at(i);     
    
    M_out(L"------------------------------------------------------------------------------------------------------\n");
    return; 
}
M_endf




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////      +------------------------------------------------+
////      | Global function to add string to ID cache      | 
////      +------------------------------------------------+
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint64_t add_cached_id(const std::wstring& ws) try
{
    // see if caller's source string is already the last one in the cache

    auto string_ct = static_N::id_cache.string_ct; 

    if (string_ct > 0)
    {
        std::wstring last_ws = static_N::id_cache.strings.at(string_ct-1);

        if (ws == last_ws) 
            return string_ct-1;        // just reuse last source string ID
    }



    // ???????????????????? make sure cache is not already at max size -- if so do special processing  ?????????????????????
    // ???????????????????? make sure cache is not already at max size -- if so do special processing  ?????????????????????
    // ???????????????????? make sure cache is not already at max size -- if so do special processing  ?????????????????????
    // ???????????????????? make sure cache is not already at max size -- if so do special processing  ?????????????????????
    // ???????????????????? make sure cache is not already at max size -- if so do special processing  ?????????????????????


    // caller's string is not last one in cache -- need to add new one

    uint64_t id = (static_N::id_cache.string_ct)++;     // capture 0-based ID = count before incrementing
    
    static_N::id_cache.strings.push_back(ws);           // add caller's ID string to cache

    if (static_N::id_cache.strings.size() != static_N::id_cache.string_ct)
    {    
        M_out_emsg(L"add_cached_id(): strings.size() = %d -- string_ct = %d -- should be the same") % static_N::id_cache.strings.size() % static_N::id_cache.string_ct; 
        M_throw("add_cached_id(): internal error -- string_ct does not agree with strings.size()")
    }
    return id; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////   get_cached_id() -- global function to get string ID from cache 
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring get_cached_id(uint64_t id) try
{
    if (id >= static_N::id_cache.string_ct)
    {
        M_out_emsg(L"get_cached_id(): requested cached string ID (%d) is greater than max allowed (%d)") % id % static_N::id_cache.string_ct ; 
        return std::wstring { L"????????????????? invalid string cache ID ?????????????????????" };     
    }   

    return static_N::id_cache.strings.at(id);   // let any out-of-range error cause exception
}
M_endf




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////    is_digit() -- static function to check based numeric digits
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  ------------------------------------------------------------------
//  Numeric digit value map -- up to base 36 -- no accented characters
//  ------------------------------------------------------------------   

static const std::unordered_map<char32_t, uint32_t>  digit_map
{
  { utf32_N::DIGIT_ZERO                      , 0  }
, { utf32_N::DIGIT_ONE                       , 1  }  
, { utf32_N::DIGIT_TWO                       , 2  }  
, { utf32_N::DIGIT_THREE                     , 3  }  
, { utf32_N::DIGIT_FOUR                      , 4  }  
, { utf32_N::DIGIT_FIVE                      , 5  }  
, { utf32_N::DIGIT_SIX                       , 6  }  
, { utf32_N::DIGIT_SEVEN                     , 7  }  
, { utf32_N::DIGIT_EIGHT                     , 8  }  
, { utf32_N::DIGIT_NINE                      , 9  }  
, { utf32_N::LATIN_CAPITAL_LETTER_A          , 10 }  
, { utf32_N::LATIN_SMALL_LETTER_A            , 10 }  
, { utf32_N::LATIN_CAPITAL_LETTER_B          , 11 }  
, { utf32_N::LATIN_SMALL_LETTER_B            , 11 }  
, { utf32_N::LATIN_CAPITAL_LETTER_C          , 12 }
, { utf32_N::LATIN_SMALL_LETTER_C            , 12 }  
, { utf32_N::LATIN_CAPITAL_LETTER_D          , 13 }
, { utf32_N::LATIN_SMALL_LETTER_D            , 13 }  
, { utf32_N::LATIN_CAPITAL_LETTER_E          , 14 }  
, { utf32_N::LATIN_SMALL_LETTER_E            , 14 }  
, { utf32_N::LATIN_CAPITAL_LETTER_F          , 15 }  
, { utf32_N::LATIN_SMALL_LETTER_F            , 15 }  
, { utf32_N::LATIN_CAPITAL_LETTER_G          , 16 }  
, { utf32_N::LATIN_SMALL_LETTER_G            , 16 }  
, { utf32_N::LATIN_CAPITAL_LETTER_H          , 17 }  
, { utf32_N::LATIN_SMALL_LETTER_H            , 17 }  
, { utf32_N::LATIN_CAPITAL_LETTER_I          , 18 }  
, { utf32_N::LATIN_SMALL_LETTER_I            , 18 }  
, { utf32_N::LATIN_CAPITAL_LETTER_J          , 19 }  
, { utf32_N::LATIN_SMALL_LETTER_J            , 19 }  
, { utf32_N::LATIN_CAPITAL_LETTER_K          , 20 }  
, { utf32_N::LATIN_SMALL_LETTER_K            , 20 }  
, { utf32_N::LATIN_CAPITAL_LETTER_L          , 21 }  
, { utf32_N::LATIN_SMALL_LETTER_L            , 21 }  
, { utf32_N::LATIN_CAPITAL_LETTER_M          , 22 }  
, { utf32_N::LATIN_SMALL_LETTER_M            , 22 }  
, { utf32_N::LATIN_CAPITAL_LETTER_N          , 23 }  
, { utf32_N::LATIN_SMALL_LETTER_N            , 23 }  
, { utf32_N::LATIN_CAPITAL_LETTER_O          , 24 }  
, { utf32_N::LATIN_SMALL_LETTER_O            , 24 }  
, { utf32_N::LATIN_CAPITAL_LETTER_P          , 25 }  
, { utf32_N::LATIN_SMALL_LETTER_P            , 25 }  
, { utf32_N::LATIN_CAPITAL_LETTER_Q          , 26 }  
, { utf32_N::LATIN_SMALL_LETTER_Q            , 26 }  
, { utf32_N::LATIN_CAPITAL_LETTER_R          , 27 }  
, { utf32_N::LATIN_SMALL_LETTER_R            , 27 }  
, { utf32_N::LATIN_CAPITAL_LETTER_S          , 28 }  
, { utf32_N::LATIN_SMALL_LETTER_S            , 28 }  
, { utf32_N::LATIN_CAPITAL_LETTER_T          , 29 }  
, { utf32_N::LATIN_SMALL_LETTER_T            , 29 }  
, { utf32_N::LATIN_CAPITAL_LETTER_U          , 30 }  
, { utf32_N::LATIN_SMALL_LETTER_U            , 30 }  
, { utf32_N::LATIN_CAPITAL_LETTER_V          , 31 }  
, { utf32_N::LATIN_SMALL_LETTER_V            , 31 }  
, { utf32_N::LATIN_CAPITAL_LETTER_W          , 32 }  
, { utf32_N::LATIN_SMALL_LETTER_W            , 32 }  
, { utf32_N::LATIN_CAPITAL_LETTER_X          , 33 }  
, { utf32_N::LATIN_SMALL_LETTER_X            , 33 }  
, { utf32_N::LATIN_CAPITAL_LETTER_Y          , 34 }  
, { utf32_N::LATIN_SMALL_LETTER_Y            , 34 }  
, { utf32_N::LATIN_CAPITAL_LETTER_Z          , 35 }  
, { utf32_N::LATIN_SMALL_LETTER_Z            , 35 }  
}; 


//  --------------------------------------------------------------------------------------------------
//  is_digit() -- return true if input char32_t value is valid digit for passed-in base (default = 16)
//  --------------------------------------------------------------------------------------------------  

static bool is_digit(char32_t ch32, int base = 16) try
{
    auto digit_pos = digit_map.find(ch32);

    if (digit_pos == digit_map.end()) 
        return false; 
       
    if (digit_pos->second < base)
        return true; 
    else
        return false; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///     TTTTT      OOO      K   K     EEEEE     NN    N                       
///       T       O   O     K  K      E         N N   N                   
///       T       O   O     KKK       EEEEE     N  N  N                   
///       T       O   O     K  K      E         N   N N            
///       T        OOO      K   K     EEEEE     N    NN                   
///
///
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   type_str() -- return printable string with token type -- helper function for display()
//   ----------
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring token_C::type_str() const try
{
     std::wstring ident {};

    // handle case of leading/trailing sigil(s) first -- allowed only for identifiers or extended identifiers (not numeric identifiers)

    if ( has_leading_sigil || has_trailing_sigil )
    {
         if (has_leading_sigil)
            ident = (wchar_t)leading_sigil;
         
         if (type == token_E::identifier)
             ident += L"identifier";
         else
         if (type == token_E::extended_identifier) 
             ident += L"extended_identifier";
         else
         if (type == token_E::error)     // identifier tokens can get switched to error tokens 
             ident += L"error";
         else
             ident += L"unexpected_sigil_token";
         
         if (has_trailing_sigil)
            ident += (wchar_t)trailing_sigil;

         return ident;      
    }


    // regular case
                                                               
    ident =  (type == token_E::none                        ) ?           L"NONE                   "   :
             (type == token_E::error                       ) ?           L"ERROR                  "   :
             (type == token_E::eof                         ) ?           L"EOF                    "   :
             (type == token_E::end                         ) ?           L"END                    "   :
             (type == token_E::eol                         ) ?           L"EOL                    "   :
             (type == token_E::unsupported                 ) ?           L"<><>unsupported<><>    "   :
             (type == token_E::invalid                     ) ?           L"invalid                "   :
             (type == token_E::whitespace                  ) ?           L"whitespace             "   :
             (type == token_E::line_comment                ) ?           L"line_comment           "   :
             (type == token_E::block_comment               ) ?           L"block_comment          "   :
             (type == token_E::nestable_comment            ) ?           L"nestable_comment       "   :
             (type == token_E::eof_comment                 ) ?           L"EOF_comment            "   :
             (type == token_E::retained_line_comment       ) ?           L"retained_line_comment  "   :
             (type == token_E::retained_block_comment      ) ?           L"retained_block_comment "   :
             (type == token_E::retained_eof_comment        ) ?           L"retained_EOF_comment   "   :
             (type == token_E::full_stop                   ) ?           L"full_stop              "   :
             (type == token_E::comma                       ) ?           L"comma                  "   :
             (type == token_E::semicolon                   ) ?           L"semicolon              "   :
             (type == token_E::colon                       ) ?           L"colon                  "   :
             (type == token_E::special_char                ) ?           L"special_char           "   :
             (type == token_E::misc_char                   ) ?           L"misc_char              "   :
             (type == token_E::punctuation_char            ) ?           L"punctuation_char       "   :
             (type == token_E::accent_char                 ) ?           L"accent_char            "   :
             (type == token_E::open_paren                  ) ?           L"open_paren             "   :
             (type == token_E::close_paren                 ) ?           L"close_paren            "   :
             (type == token_E::open_bracket                ) ?           L"open_bracket           "   :
             (type == token_E::close_bracket               ) ?           L"close_bracket          "   :
             (type == token_E::open_brace                  ) ?           L"open_brace             "   :
             (type == token_E::close_brace                 ) ?           L"close_brace            "   :
             (type == token_E::open_angle_bracket          ) ?           L"open_angle_bracket     "   :
             (type == token_E::close_angle_bracket         ) ?           L"close_angle_bracket    "   :
             (type == token_E::open_shell_bracket          ) ?           L"open_shell_bracket     "   :
             (type == token_E::close_shell_bracket         ) ?           L"close_shell_bracket    "   :
             (type == token_E::open_quill_bracket          ) ?           L"open_quill_bracket     "   :
             (type == token_E::close_quill_bracket         ) ?           L"close_quill_bracket    "   :
             (type == token_E::open_dot_bracket            ) ?           L"open_dot_bracket       "   :
             (type == token_E::close_dot_bracket           ) ?           L"close_dot_bracket      "   :
             (type == token_E::open_misc_bracket           ) ?           L"open_misc_bracket      "   :
             (type == token_E::close_misc_bracket          ) ?           L"close_misc_bracket     "   :
             (type == token_E::number                      ) ?           L"number                 "   :
             (type == token_E::unit                        ) ?           L"unit                   "   :
             (type == token_E::boolean                     ) ?           L"boolean                "   :
             (type == token_E::int8                        ) ?           L"int8                   "   :
             (type == token_E::uint8                       ) ?           L"uint8                  "   :
             (type == token_E::int16                       ) ?           L"int16                  "   :
             (type == token_E::uint16                      ) ?           L"uint16                 "   :
             (type == token_E::int32                       ) ?           L"int32                  "   :
             (type == token_E::uint32                      ) ?           L"uint32                 "   :
             (type == token_E::int64                       ) ?           L"int64                  "   :
             (type == token_E::uint64                      ) ?           L"uint64                 "   :
             (type == token_E::float32                     ) ?           L"float32                "   :
             (type == token_E::float64                     ) ?           L"float64                "   :
             (type == token_E::identifier                  ) ?           L"identifier             "   :
             (type == token_E::numeric_identifier          ) ?           L"numeric_identifier     "   :
             (type == token_E::extended_identifier         ) ?           L"extended_identifier    "   :
             (type == token_E::oper                        ) ?           L"operator               "   :
             (type == token_E::string                      ) ?           L"string                 "   :
             (type == token_E::vanishing_separator         ) ?           L"vanishing separator    "   :
                                                                         L"???????????            "   ;

    return ident; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   display() -- print out token for debugging, errors 
//   ---------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_C::display(const std::wstring& ws, bool show_orig_str) const try
{
    std::wstring val  {                                          };
    std::wstring ostr { orig_str + (attached_paren ? L"(" : L"") };

    if      (type == token_E::uint8   )  val = std::wstring { L"   value=" } + std::to_wstring(uint8     ); 
    else if (type == token_E::uint16  )  val = std::wstring { L"   value=" } + std::to_wstring(uint16    );   
    else if (type == token_E::uint32  )  val = std::wstring { L"   value=" } + std::to_wstring(uint32    );   
    else if (type == token_E::uint64  )  val = std::wstring { L"   value=" } + std::to_wstring(uint64    ); 
    else if (type == token_E::int8    )  val = std::wstring { L"   value=" } + std::to_wstring( int8     );  
    else if (type == token_E::int16   )  val = std::wstring { L"   value=" } + std::to_wstring( int16    );  
    else if (type == token_E::int32   )  val = std::wstring { L"   value=" } + std::to_wstring( int32    );  
    else if (type == token_E::int64   )  val = std::wstring { L"   value=" } + std::to_wstring( int64    ); 
    else if (type == token_E::boolean )  val = std::wstring { L"   value=" } + std::to_wstring(boolean   ); 
    else if (type == token_E::unit    )  val = std::wstring { L"   value=" } + std::wstring { L"<UNIT>"  };

    else if (type == token_E::float64) 
    {
        //(no exponential support)  val = "   value=" + std::to_wstring(float64) + "   ";  
        wchar_t buf[400] {}; 
        (void)swprintf(buf, (sizeof buf)/(sizeof buf[0]) - 1, L"   value=%-#.20f", float64);
        val = buf; 
    }
    else if (type == token_E::float32) 
    {
        //(no exponential support)  val = "   value=" + std::to_wstring(float32) + "   ";  
        wchar_t buf[400] {}; 
        (void)swprintf(buf, (sizeof buf)/(sizeof buf[0]) - 1, L"   value=%-#.20f", (float64_T)float32);
        val = buf; 
    }
    else if (only_char != (char32_t)0)
    {
        wchar_t buf [400] {};
        (void)swprintf(buf, (sizeof buf)/(sizeof buf[0]) - 1, L"   1st_char32=%08X", (uint32_t)only_char);
        val = buf;     
    }

    //       ws        ty  uty ty_str id1                 id2                      filename1              str    str        val
    M_out( L"%s: type=%02d/%02d:%-24s pos1=%04d:%05d:%03d pos2=%04d:%05d:%03d source1=«%s» %|160t| token= «%s»  «%s» %|235t| %s")
         % ws
         % (int)(type)
         % (int)(utype)
         % type_str()
         % source_id1 % lineno1 % linepos1
         % source_id2 % lineno2 % linepos2 
         % M_wsl(get_cached_id(source_id1), 55)
         % M_wsl(show_orig_str ? ostr :  str, 32)
         % M_wsl(show_orig_str ?  str : ostr, 32)
         % val
         ; 
                    
    return; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   loc_str() -- return printable token location 
//   ---------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring token_C::loc_str() const try
{
    return  L"input source = «"
          + get_cached_id(source_id1)
          + L"» -- start line:column = "
          + std::to_wstring(lineno1)
          + L":"
          + std::to_wstring(linepos1)
          + L" -- end line:column = " 
          + std::to_wstring(lineno2) 
          + L":" 
          + std::to_wstring(linepos2)
          ;
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///     TTTTT    OOO     K   K    EEEEE    NN    N           SSSS   TTTTT   RRRR    EEEEE    AAA     M     M           
///       T     O   O    K  K     E        N N   N          S         T     R   R   E       A   A    MM   MM     
///       T     O   O    KKK      EEEEE    N  N  N           SSS      T     RRRR    EEEEE   AAAAA    M M M M    
///       T     O   O    K  K     E        N   N N              S     T     R  R    E       A   A    M  M  M 
///       T      OOO     K   A    EEEEEE   N    NN   =====  SSSS      T     R   R   EEEEE   A   A    M     M      
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// destructor
// ----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

token_stream_C::~token_stream_C()       // destructor -- can't throw
{
    M__(M_out(L"~token_stream_C() called -- destructor -- m_in_filename = \"%s\"") % m_in_filename;)
  

    //   Make sure all open files on the stack (if any) are closed and freed up 

    close_all_instreams();         
    return; 
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//public:
//
//
// attach_file() -- add new file to file_stack  
// -------------
//
//               -- allocates new instream_C and infile_C + char_steam_C for the new file and associates them
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                            
int token_stream_C::attach_file(const std::wstring& pathname, api_err_S& err) try    
{
    M__(M_out(L"token_stream_C::attach_file(\"%s\") called -- putback stack size = %d      raw token stack size = %d ") % info.filename % m_token_stack.size() % m_raw_token_stack.size();)
   
    err = api_err_S { };                               // clean out passback error structure before starting

    if (m_error_seen)
    {
        api_err_non_system( err 
                          , non_syserr_E::token_stream_attach_error
                          , L"token_stream_C::attach_file() was called to attach a file to the token_stream_C after a prior error had been reported"
                          );
        M_out_emsg(L"token_stream_C::attach file() called after error had been reported -- \"%s\"") %  m_in_filename;
        count_error();
        return -1;
    }

    
    //  Create new infile_C object -- check if file is open -- return with error if open() failed

    infile_C* infile_p = new infile_C { pathname, err }; // passed-in pathname info causes open() to be done                  

    if (!(infile_p->is_open()))
    {
        delete infile_p; 
        m_error_seen = true;                             // stop any further input
        return -1;                                       // infile_C constructor should have filled in the api_err_S data
    }

    m_in_filename = pathname;                            // file is open -- set active filename for use in token error messages
    m_source_id   = infile_p->get_source_id();           // capture source id for error messages 


    //  Allocate new char_stream to be stacked, and attach the pre-opened input file

    m_char_stream_p = new char_stream_C {};              // get new char_stream to be associated with this file -- anchor it as the active/current char_stream -- this pointer does not own the char_stream  
    m_char_stream_p->attach_file(infile_p);              // attach the new file to the new char_stream
    m_char_stream_p->set_trigraph_char(m_trigraph_char); // propogate current trigraph char to new char_stream_S


    //  Allocate new instream_C structure, fill it in with the new infile_p and new m_char_stream_p, than add instream_C structure to m_instream_stack  

    m_instream_p = new instream_C {};                    // allocate a new instream_C structure to be added to stack (overwrite old pointer) 
    m_instream_p->infile_p = infile_p;                   // save pointer to pre-opened file in instream_C structure  
    m_instream_p->char_stream_p = m_char_stream_p;       // anchor the new char_stream in the instream_C structure  
    m_instream_stack.push(m_instream_p);                 // add to top of instream stack  

 
    // save current token stack and raw token stack in new instream-C object (for later restoration)

    m_instream_p->token_stack     = m_token_stack;
    m_instream_p->raw_token_stack = m_raw_token_stack;
    m_token_stack.clear();
    m_raw_token_stack.clear();
  

    if (m_instream_stack.size() > m_max_instream_nest)
        M_throw_v("token_stream_C::attach_file(): number of nested instream_C objects is more than %d") % m_max_instream_nest)); 

        
    return 0; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//public:
//
//
// close() -- free up stuff and reset states -- leave stats and configuration intact  
// =======
//
//          
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::close() try   
{
    // get rid of everything

    close_all_instreams(); 

    m_in_filename.clear();
    m_token_stack.clear();
    m_raw_token_stack.clear();


    // reset state

    m_attach_mode = true; 
    m_error_seen  = false; 

    // note: all configuration settings and statistics are left as-is, so token stream can be reused 

    return;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//public:
//
//
// refresh() -- refresh token stream for reuse (after error, etc.)  
// =========
//
//          
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::refresh() try   
{
    // just do close() to get rid of everything, but keep counters, etc.

    close(); 
  
    return;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//public:
//
//
// attach_string() -- add new string to file_stack  
// -------------
//
//               -- allocates new instream_C and instring_C + char_steam_C for the new string and associates them
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// "plain" string/string name version 

int token_stream_C::attach_string(const std::string& s, const std::string& n) try   
{
    M__(M_out(L"token_stream_C::attach_string(std::string, \"%s\") called") % to_wstring(n);)
  
    if (m_error_seen)
    {
        M_out_emsg(L"token_stream_C::attach_string(std::string) called after error had been reported -- \"%s\"") %  m_in_filename;
        count_error();
        return -1;
    }

    
   //  Create new instring_C object -- any errors will be picked up by attach_string_common()

    instring_C* instring_p   = new instring_C {s, n};     
    m_in_filename            = to_wstring(n);                     // set active name for use in token error messages

    return attach_string_common(instring_p);   
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////

// plain string data, wstring name version 

int token_stream_C::attach_string(const std::string& s, const std::wstring& n) try   
{
    M__(M_out(L"token_stream_C::attach_string(std::string, \"%s\") called") % n;)
  
    if (m_error_seen)
    {
        M_out_emsg(L"token_stream_C::attach_string(std::string) called after error had been reported -- \"%s\"") %  m_in_filename;
        count_error();
        return -1;
    }

    
    //  Create new instring_C object -- any errors will be picked up by attach_string_common()

    instring_C* instring_p = new instring_C {s, n};                    
    m_in_filename            = n;                                 //  set active name for use in token error messages

    return attach_string_common(instring_p);   
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////

// wstring data, wstring name version 

int token_stream_C::attach_string(const std::wstring& s, const std::wstring& n) try   
{
    M__(M_out(L"token_stream_C::attach_string(std::wstring, \"%s\") called") % n;)
  
    if (m_error_seen)
    {
        M_out_emsg(L"token_stream_C::attach_string(std::wstring) called after error had been reported -- \"%s\"") %  m_in_filename;
        count_error(); 
        return -1;
    }

    
    //  Create new instring_C object -- any errors will be picked up by attach_string_common()

    instring_C* instring_p   = new instring_C {s, n};             // need to use shared_ptr here to avoid storage leak ??????????????????????????? pass by move ref???       
    m_in_filename            = n;                                 // set active name for use in error messages
    m_source_id              = instring_p->get_source_id();       // capture source id for error messages 

    return attach_string_common(instring_p);
   
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////

// routine with common attach_string() code

int token_stream_C::attach_string_common(instring_C *instring_p) try   
{    
     m_source_id  = instring_p->get_source_id();         // capture source id for error messages 


    //  Check if pre-created instring_C is open -- return with error if not

    if (!(instring_p->is_open()))
    {
        M_out_emsg(L"token_stream_C::attach_string_common() -- input string is not open");  

        delete instring_p; 
        m_error_seen = true;                             // stop any further input
        return -1;
    }


    //  Allocate new char_stream to be stacked, and attach the pre-opened input file

    m_char_stream_p = new char_stream_C {};              // get new char_stream to be associated with this file -- anchor it as the active/current char_stream a -- this pointer does not own the char_stream  
    m_char_stream_p->attach_string(instring_p);          // attach the new string to the new char_stream
    m_char_stream_p->set_trigraph_char(m_trigraph_char); // propogate current trigraph char to new char_stream_S


    //  Allocate new instream_C structure, fill it in with the new infile_p and new m_char_stream_p, then add instream_C structure to m_instream_stack  

    m_instream_p = new instream_C {};                    // allocate a new instream_C structure to be added to stack  (overwrite old pointer)  
    m_instream_p->instring_p = instring_p;               // save pointer to pre-opened file in instream_C structure  
    m_instream_p->char_stream_p = m_char_stream_p;       // anchor the new char_stream in the instream_C structure  
    m_instream_stack.push(m_instream_p);                 // add to top of instream stack  
                                                       
       
    // save current token stack and raw token stack in new instream-C object (for later restoration)

    m_instream_p->token_stack     = m_token_stack;
    m_instream_p->raw_token_stack = m_raw_token_stack;
    m_token_stack.clear();
    m_raw_token_stack.clear();
  
    M__(M_out(L"token_stream_C::attach_string_common() -- attach_string nest = %d") % m_instream_stack.size();)

    if (m_instream_stack.size() > m_max_instream_nest)
    {
        M_out_emsg(L"token_stream_C::attach_string_common(): error -- instream_C nest level = %d -- more than max = %d") % m_instream_stack.size() % m_max_instream_nest;
        M_throw_v("token_stream_C::attach_string_common(): number of nested instream_C objects is more than %d") % m_max_instream_nest)); 
    }


    return 0; 
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  close_all_instreams()
//  ---------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::close_all_instreams() try 
{
   
    //   Make sure all open files on the stack (if any) are closed and the associated char_streams are freed up 

    m_instream_p = nullptr;                               // clear out, since it will be invalid soon

    while (!m_instream_stack.empty())    
    {
        instream_C* in_stream_p = m_instream_stack.top(); // access top stacked instream_C structure

        if (in_stream_p->instring_p != nullptr)
            in_stream_p->instring_p->close();             // close associated string (if any)
        if (in_stream_p->infile_p != nullptr)
            in_stream_p->infile_p->close();               // close associated file (if any) (probably already closed)

        delete  in_stream_p->char_stream_p;               // delete associated character stream 

        if (in_stream_p->instring_p != nullptr)
            delete in_stream_p->instring_p;               // free up instring_C structure 
        if (in_stream_p->infile_p != nullptr)
            delete in_stream_p->infile_p;                 // free up infile_C structure 

        delete  in_stream_p;                              // delete instream_C structure that wll be removed from stack 

        m_instream_stack.pop();                           // move to next queued instream_C structure

    }

    m_char_stream_p = nullptr;                            // no valid char_stream anymore

    // leave current filename set (although invalid) for final error messages -- capture 

    return; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  close_current_instream()
//  ------------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::close_current_instream() try
{
    M__(M_out(L"token_stream_C::close_current_instream(\"%s\") called -- stack size = %d   raw_stack_size = %d") % m_in_filename % m_token_stack.size() % m_raw_token_stack.size();)

    if (m_raw_token_stack.size() > 0)
        m_raw_token_stack.front().display();
    

    // move anything from saved token putback stacks onto back end of current putback token stacks
    // (m_instream_p->stacks should be going away, so don't clear() them here

    auto szr = m_instream_p->raw_token_stack.size();

    for (auto i = 0; i < szr; i++)       
        m_raw_token_stack.push_back(m_instream_p->raw_token_stack.at(i));
  
    auto sz = m_instream_p->token_stack.size();


    for (auto i = 0; i < sz; i++)
        m_token_stack.push_back(m_instream_p->token_stack.at(i));


    //  Remove top instream_C structure from stack and close files, free up things, etc.

    m_instream_p = nullptr;                                         // will soon be invalid, so clear it out

    instream_C *in_stream_p = m_instream_stack.top();               // access newest stacked instream_C structure 
    m_instream_stack.pop();                                         // remove pointer to it from stack 
 
    if (in_stream_p->instring_p != nullptr)
        in_stream_p->instring_p->close();                           // close associated string (if any)
    if (in_stream_p->infile_p != nullptr)
        in_stream_p->infile_p->close();                             // may not exist -- may not be open -- self-close()d already 

    delete in_stream_p->char_stream_p;                              // free up the char stream 

    if (in_stream_p->instring_p != nullptr)
        delete in_stream_p->instring_p;                             // free up instring_C structure 
    if (in_stream_p->infile_p != nullptr)
        delete in_stream_p->infile_p;                               // free up infile_C structure 

    delete in_stream_p ;                                            // delete instream_C structure that was removed from the stack


    //  Revert to next (older) instream on stack (if any)
    //  -------------------------------------------------

    if (m_instream_stack.empty())                                   // that was last/only file or string?
    {                                                              
        m_char_stream_p = nullptr;                                  // clear out file pointer to prevent more I/O
        m_in_filename = std::wstring{};                             // no valid filename now
        m_source_id   = 0;                                         
    }                                                              
    else                                                            // need to revert to prior instream_C on stack 
    {                                                              
        m_instream_p    = m_instream_stack.top();                   // look at top instream_C structue on stack after pop()   (overwrite old pointer)
        m_char_stream_p = m_instream_p->char_stream_p;              // switch back to char_stream_C at top of stack 

        if (m_instream_p->infile_p != nullptr)
        {
            m_in_filename = m_instream_p->infile_p->get_filename(); // revert to old filename -- get from top stacked infile_C 
            m_source_id   = m_instream_p->infile_p->get_source_id(); 
        }
        else if (m_instream_p->instring_p != nullptr)
        {
            m_in_filename = m_instream_p->instring_p->get_name();   // revert to old string name -- get from top stacked infile_C 
            m_source_id   = m_instream_p->instring_p->get_source_id(); 
        }
        else
        {
            M_throw("token_stream_C::close_current_instream(): both m_instream_p->infile_p and m_instream_p->instring_p are 0");
            return;      // should not get here
        }

        ///////////////////// does putback stack need to be restored ?????

    }    

    M__(M_out(L"token_stream_C::close_current_instream() returning -- \"%s\"") % m_in_filename;)
    return; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// get_char() -- call char_stream_p->get_char() if m_char_stream_p is not nullptr  
// ----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int token_stream_C::get_char(in_char_S& ch) try
{
    //  if char_stream available -- just call get_char()

    if (m_char_stream_p != nullptr)
    {
        auto rc = m_char_stream_p->get_char(ch);

        // save away char location, for use later when char_E::end is passed back (after stream closure)

        static_N::last_char_source_id = ch.source_id;
        static_N::last_char_lineno    = ch.lineno; 
        static_N::last_char_linepos   = ch.linepos; 

        return rc; 
    }
    else                                                    // all char streams must be ended now 
    {                                                       // pass back manufactured "end" character
        ch = in_char_S {};                                  // blank out all fields first (clears out filename string)    
        ch.classs    = char_E::end;                         // set everything to end 
        ch.family    = char_E::end; 
        ch.type      = char_E::end; 
        ch.subtype   = char_E::end;
        ch.ch32      = 0; 
        ch.wch1      = 0; 
        ch.wch2      = 0; 
        ch.source_id = static_N::last_char_source_id ;      // used saved values from when stream was valid
        ch.lineno    = static_N::last_char_lineno    ;      // used saved values from when stream was valid
        ch.linepos   = static_N::last_char_linepos +1;      // used saved values from when stream was valid

        M__(M_out(L"token_stream_C::get_char() -- char_stream_p is NULL -- passing back ch.subtype = char_E::end");)
    }

    return 0;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// peek_char() -- call char_stream_p->peek_char() if char_stream_p is not nullptr  
// -----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int token_stream_C::peek_char(in_char_S& ch, size_t n) try
{
    M__(M_out(L"token_stream_C::peek_char(%d) called") % n;)

    //  if char_stream available -- just call peek_char()

    if (m_char_stream_p != nullptr)
    {
        auto rc =  m_char_stream_p->peek_char(ch, n); 

        // save away char location, for use later when char_E::end is passed back (after stream closure)

        static_N::last_char_source_id = ch.source_id;
        static_N::last_char_lineno    = ch.lineno; 
        static_N::last_char_linepos   = ch.linepos; 

        return rc;  
    }
    else                                                  // all char streams must be ended now 
    {                                                     // pass back manufactured "end" character
        ch = in_char_S {};                                // blank out all fields first (clears out filename string)    
        ch.classs    = char_E::end;                       // set everything to end 
        ch.family    = char_E::end; 
        ch.type      = char_E::end; 
        ch.subtype   = char_E::end;
        ch.ch32      = 0; 
        ch.wch1      = 0; 
        ch.wch2      = 0; 
        ch.source_id = static_N::last_char_source_id ;     // used saved values from when stream was valid
        ch.lineno    = static_N::last_char_lineno    ;     // used saved values from when stream was valid
        ch.linepos   = static_N::last_char_linepos +1;     // used saved values from when stream was valid

        M__(M_out(L"token_stream_C::peek_char() -- char_stream_p is NULL -- passing back ch.subtype = char_E::end");)
    }

    return 0;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// putback_char() -- call char_stream_p->putback_char() if m_char_stream_p is not nullptr  
// --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::putback_char(const in_char_S& ch) try
{
    //  if char_stream available -- just call putback_char()

    if (m_char_stream_p != nullptr)
        return m_char_stream_p->putback_char(ch);     

    return;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// discard_char() -- call char_stream_p->discard_char() if m_char_stream_p is not nullptr  
// --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int token_stream_C::discard_char(size_t n) try
{
    //  if char_stream available -- just call discard_char()

    if (m_char_stream_p != nullptr)
        return m_char_stream_p->discard_char(n);     

    return 0;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// get_token() -- get next (or any put-back) token in stream  
// -----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::get_token(token_C& token) try             // get (and consume) next token from stream  
{
    M__(M_out(L"token_stream_C::get_token() called");)

    // pass back top put-back token (if any on stack)

    if (!m_token_stack.empty())
    {
        token = m_token_stack.front();                        // should have been fileld in before is was put back   
        m_token_stack.pop_front();                            // consume token 

        if (token.type == token_E::error)
            return -1;                                        // if returning prior error, return with error R/C
        else
            return 0;                                         // else returned char is OK                   
    }
    

    // token putback stack is empty -- need to get next composite token from the character stream

    int rc = fetch_composite_token(token);                    // get next composite token from the input character stream
    return rc; 
}
M_endf 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// peek_token() -- return next (or n-th next) token without consuming it -- ends up on putback stack (or is left on putback stack)  
// -----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::peek_token(token_C& token, size_t n) try              // get (and consume) next character from stream  
{
     M__(M_out(L"token_stream_C::peek_token(%d) called") % n;)


    // make sure 'n' is greater than 0 

    if (n <= 0)
    {
        M_out_emsg(L"token_stream_C::peek_token(token, %u) called with bad char position parm (%)") % n % n;
        return -1; 
    }


    // pass back requested token, if it is on the existing putback stack -- oldest are at start [0], so n-th oldest has index [n-1]  

    size_t sz = m_token_stack.size();                            // number of put-back tokens on stack 

    if (sz >= n)
    {
        token = m_token_stack.at(n-1);                           // get n-th oldest (composite) token  
           
        M__(M_out(L"token_stream_C::peek_token -------------------------------------->");)
        M__(token.display();)
        M__(M_out(L"<-------------------------------------------");)


        // leave token on stack

        if (token.type == token_E::error)
            return -1;                                         // if returning prior error, return with error R/C
        else
            return 0;                                          // else returned token is OK             
    }
       

    // Loop to add enough tokens to new end of putback stack, so that n-th oldest token is on the putback stack
    // --------------------------------------------------------------------------------------------------------
    
    int     rc  {0}; 
    token_C tok {}; 

    for (auto i = 0;  i < (n - sz); ++i)
    {
        int rci = fetch_composite_token(tok);      // get next composite token     
        if (rci != 0) rc = rci;                    // remember any non-zero R/C
        m_token_stack.push_back(tok);              // add newest composite token to back end of putback stack
    }

    if (m_token_stack.size() != n)
    {
        M_out_emsg(L"token_stream_C::peek_token(token, %u) -- unexpected number of elements (%u) on putback stack") % n % m_token_stack.size();
        rc = -1;
    }


    //  Desired character should be at the back of the stack now -- return it but leave it on the stack 

    token = m_token_stack.back();                  // passing back last (newest) token on stack -- should be the right one, if R/C is 0  
    M__(M_out(L"token_stream_C::peek_token() ---------------------------------------------------->");)
    M__(token.display();)
    M__(M_out(L"<------------------------------------");)
       
    return rc; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// discard_token() -- discard n-th token on putback stack (if present)  -- 1 = top token 
// --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::discard_token(size_t n) try                      
{
     // discard top or n-th put-back character (if any)

     if ( (n > 0) && (m_token_stack.size() >= n) )
     {
         if (n == 1)
         {
             m_token_stack.pop_front();                               // remove oldest composite token on putback queue 
         }
         else
         {
             auto it = m_token_stack.begin(); 
             m_token_stack.erase(it + (n - 1));                       // remove n-th oldest composite token on putback queue         
         }
         return 0;                                                       
     }
      

     // n is bad, or not enough characters on putback queue -- don't do anything -- R/C is -1 
 
     return -1;     
}
M_endf 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// putback_token() -- put back char (onto queued token stack)  -- (these should be composite tokens)
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
 
void token_stream_C::putback_token(const token_C& token) try      // putback token to stream -- no need to be connected to a file
{
    m_token_stack.push_front(token);                             // put at oldest position on stack   
    return;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// is_empty() -- return true, if putback stack is empty  
// ----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
bool token_stream_C::is_empty() const try
{     
    return m_token_stack.empty();
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// putback_size() -- return number of put-back characters on queue  
// --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
size_t token_stream_C::putback_size() const try
{     
    return m_token_stack.size();
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// set_xxxxx() -- functions to set/reset boolean flags and other configuration values 
// -----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                             
void token_stream_C::set_quiet_mode(                      bool     tf                    ) try { m_quiet_mode                     = tf  ; return; }  M_endf
void token_stream_C::set_suppress_echo(                   bool     tf                    ) try { m_suppress_echo                  = tf  ; return; }  M_endf
void token_stream_C::set_eol_is_whitespace(               bool     tf                    ) try { m_eol_is_whitespace              = tf  ; return; }  M_endf
void token_stream_C::set_eof_is_whitespace(               bool     tf                    ) try { m_eof_is_whitespace              = tf  ; return; }  M_endf
void token_stream_C::set_comment_is_whitespace(           bool     tf                    ) try { m_comment_is_whitespace          = tf  ; return; }  M_endf
void token_stream_C::set_combine_whitespace(              bool     tf                    ) try { m_combine_whitespace             = tf  ; return; }  M_endf
void token_stream_C::set_combine_strings(                 bool     tf                    ) try { m_combine_strings                = tf  ; return; }  M_endf
void token_stream_C::set_skip_whitespace(                 bool     tf                    ) try { m_skip_whitespace                = tf  ; return; }  M_endf
void token_stream_C::set_allow_num_identifier(            bool     tf                    ) try { m_allow_num_identifier           = tf  ; return; }  M_endf
void token_stream_C::set_allow_extd_identifier(           bool     tf                    ) try { m_allow_extd_identifier          = tf  ; return; }  M_endf
void token_stream_C::set_report_extd_identifier(          bool     tf                    ) try { m_report_extd_identifier         = tf  ; return; }  M_endf
void token_stream_C::set_preserve_sigils(                 bool     tf                    ) try { m_preserve_sigils                = tf  ; return; }  M_endf
void token_stream_C::set_always_attach_plus_minus(        bool     tf                    ) try { m_always_attach_plus_minus       = tf  ; return; }  M_endf 
void token_stream_C::set_never_attach_plus_minus(         bool     tf                    ) try { m_never_attach_plus_minus        = tf  ; return; }  M_endf
void token_stream_C::set_allow_leading_id_sigils(         bool     tf                    ) try { m_allow_leading_id_sigils        = tf  ; return; }  M_endf
void token_stream_C::set_allow_trailing_id_sigils(        bool     tf                    ) try { m_allow_trailing_id_sigils       = tf  ; return; }  M_endf
void token_stream_C::set_allow_leading_op_sigils(         bool     tf                    ) try { m_allow_leading_op_sigils        = tf  ; return; }  M_endf
void token_stream_C::set_allow_trailing_op_sigils(        bool     tf                    ) try { m_allow_trailing_op_sigils       = tf  ; return; }  M_endf
void token_stream_C::set_allow_paren_sigils(              bool     tf                    ) try { m_allow_paren_sigils             = tf  ; return; }  M_endf
void token_stream_C::set_allow_attached_paren(            bool     tf                    ) try { m_allow_attached_paren           = tf  ; return; }  M_endf
void token_stream_C::set_allow_in_same_char_op(           bool     tf                    ) try { m_allow_in_same_char_op          = tf  ; return; }  M_endf
                                 

// functions to set configurable characters

void token_stream_C::set_trigraph_char(                   char32_t ch32                  ) try { m_trigraph_char                  = ch32; return; }  M_endf
void token_stream_C::set_vanishing_separator_char(        char32_t ch32                  ) try { m_vanishing_separator_ch         = ch32; return; }  M_endf
void token_stream_C::set_line_continuation_char(          char32_t ch32                  ) try { m_line_continuation_ch           = ch32; return; }  M_endf
void token_stream_C::set_always_sign_char(                char32_t ch32                  ) try { m_always_sign_ch                 = ch32; return; }  M_endf

void token_stream_C::set_type1_string_start_char(         char32_t ch32                  ) try { m_type1_string_start_ch          = ch32; return; }  M_endf
void token_stream_C::set_type2_string_start_char(         char32_t ch32                  ) try { m_type2_string_start_ch          = ch32; return; }  M_endf
void token_stream_C::set_type1_string_end_char(           char32_t ch32                  ) try { m_type1_string_end_ch            = ch32; return; }  M_endf
void token_stream_C::set_type2_string_end_char(           char32_t ch32                  ) try { m_type2_string_end_ch            = ch32; return; }  M_endf
void token_stream_C::set_type1_string_escape_char(        char32_t ch32                  ) try { m_type1_string_escape_ch         = ch32; return; }  M_endf
void token_stream_C::set_type2_string_escape_char(        char32_t ch32                  ) try { m_type1_string_escape_ch         = ch32; return; }  M_endf
void token_stream_C::set_raw_string_prefix_char(          char32_t ch32                  ) try { m_raw_string_prefix_ch           = ch32; return; }  M_endf
void token_stream_C::set_multiline_string_prefix_char(    char32_t ch32                  ) try { m_multiline_string_prefix_ch     = ch32; return; }  M_endf
void token_stream_C::set_word_string_start_char(          char32_t ch32                  ) try { m_word_string_start_ch           = ch32; return; }  M_endf

void token_stream_C::set_unechoed_line_comment_char(      char32_t ch32                  ) try { m_unechoed_line_comment_ch       = ch32; return; }  M_endf
void token_stream_C::set_echoed_line_comment_char(        char32_t ch32                  ) try { m_echoed_line_comment_ch         = ch32; return; }  M_endf

void token_stream_C::set_comment_1st_char(                char32_t ch32                  ) try { m_comment_1st_ch                 = ch32; return; }  M_endf      
void token_stream_C::set_unechoed_line_comment_2nd_char(  char32_t ch32                  ) try { m_unechoed_line_comment_2nd_ch   = ch32; return; }  M_endf      
void token_stream_C::set_echoed_line_comment_2nd_char(    char32_t ch32                  ) try { m_echoed_line_comment_2nd_ch     = ch32; return; }  M_endf      
void token_stream_C::set_suppress_eol_comment_2nd_char(   char32_t ch32                  ) try { m_suppress_eol_comment_2nd_ch    = ch32; return; }  M_endf      
                                                                                                                             
void token_stream_C::set_block_comment_2nd_char(          char32_t ch32                  ) try { m_block_comment_2nd_ch           = ch32; return; }  M_endf      
void token_stream_C::set_block_comment_3rd_char(          char32_t ch32                  ) try { m_block_comment_3rd_ch           = ch32; return; }  M_endf      
void token_stream_C::set_block_comment_4th_char(          char32_t ch32                  ) try { m_block_comment_4th_ch           = ch32; return; }  M_endf      
void token_stream_C::set_nest_comment_2nd_char(           char32_t ch32                  ) try { m_nest_comment_2nd_ch            = ch32; return; }  M_endf      
void token_stream_C::set_nest_comment_3rd_char(           char32_t ch32                  ) try { m_nest_comment_3rd_ch            = ch32; return; }  M_endf      
void token_stream_C::set_nest_comment_4th_char(           char32_t ch32                  ) try { m_nest_comment_4th_ch            = ch32; return; }  M_endf 
void token_stream_C::set_eof_comment_2nd_char(            char32_t ch32                  ) try { m_eof_comment_2nd_ch             = ch32; return; }  M_endf    

void token_stream_C::set_retained_line_comment_2nd_char(  char32_t ch32                  ) try { m_retained_line_comment_2nd_ch   = ch32; return; }  M_endf 
void token_stream_C::set_retained_block_comment_2nd_char( char32_t ch32                  ) try { m_retained_block_comment_2nd_ch  = ch32; return; }  M_endf      
void token_stream_C::set_retained_block_comment_3rd_char( char32_t ch32                  ) try { m_retained_block_comment_3rd_ch  = ch32; return; }  M_endf 
void token_stream_C::set_retained_block_comment_4th_char( char32_t ch32                  ) try { m_retained_block_comment_4th_ch  = ch32; return; }  M_endf 
void token_stream_C::set_retained_eof_comment_2nd_char(   char32_t ch32                  ) try { m_retained_eof_comment_2nd_ch    = ch32; return; }  M_endf 
                                                                                                                          
void token_stream_C::set_leading_sigils(          const std::vector<char32_t>& ls        ) try { m_leading_sigils           = ls  ;       return; }  M_endf
void token_stream_C::set_leading_ident_sigils(    const std::vector<char32_t>& ls        ) try { m_leading_ident_sigils     = ls  ;       return; }  M_endf
void token_stream_C::set_trailing_ident_sigils(   const std::vector<char32_t>& ts        ) try { m_trailing_ident_sigils    = ts  ;       return; }  M_endf
void token_stream_C::set_leading_oper_sigils(     const std::vector<char32_t>& ls        ) try { m_leading_oper_sigils      = ls  ;       return; }  M_endf
void token_stream_C::set_trailing_oper_sigils(    const std::vector<char32_t>& ts        ) try { m_trailing_oper_sigils     = ts  ;       return; }  M_endf
void token_stream_C::set_paren_sigils(            const std::vector<char32_t>&  s        ) try { m_paren_sigils             =  s  ;       return; }  M_endf
void token_stream_C::set_in_same_char_op(         const std::vector<char32_t>&  s        ) try { m_in_same_char_op          =  s  ;       return; }  M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////  helper functions used by fetch_raw_token() 
////  ------------------------------------------
////
////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    get_integer_value() -- convert string of digits in token into numeric value of proper length
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int token_stream_C::get_integer_value(token_C& token, int int_length, bool un_signed, int base) try
{
    int rc {0}; 
      

    // handle signed integers  (regular minus sign (not any alternate sign char, like the always_sign char) must be present at beginning, if number is negative) -- note that boolean and unit must be unsigned 
    // ----------------------

    if (!un_signed)                                           // signed integer?       
    {   
        if (int_length == 8)                                  // int8_t value? 
        {
            rc = to_int8(token.str, token.int8, base);        // convert value into int8_t field 

            if (rc == 0)                
                token.type = token_E::int8;                
            else
                invalid_token(token, L"Cannot obtain value for int8 literal");    
        }
        else if  (int_length == 16)                           // int16_t value? 
        {
            rc = to_int16(token.str, token.int16, base);      // convert value into int16_t field 

            if (rc == 0)                
                token.type = token_E::int16;                
            else
                invalid_token(token, L"Cannot obtain value for int16 literal");    
        }
        else if  (int_length == 32)                           // int32_t value? 
        {
            rc = to_int32(token.str, token.int32, base);      // convert value into int32_t field 

            if (rc == 0)                
                token.type = token_E::int32;                
            else
                invalid_token(token, L"Cannot obtain value for int32 literal");    
        }
        else                                                  // must be int64_t value? 
        {
            rc = to_int64(token.str, token.int64, base);      // convert value into int64_t field 

            if (rc == 0)                
                token.type = token_E::int64;                
            else
                invalid_token(token, L"Cannot obtain value for int64 literal");    
        }     
    }


    // handle unsigned integers  
    // ------------------------
    
    else                                                      // must be unsigned integer - no sign, and U or u seen 
    {
        ///////////////// process unit values ///////////////////////

        if (int_length == 0)                                  // unit value? 
        {
            uint64_t local_uint64 { 0 };                      // local workarea for conversion

            rc = to_uint64(token.str, local_uint64, base);    // convert value into uint8_t field 

            if (rc == 0) 
            {
                if (local_uint64 == 0)
                    token.type = token_E::unit; 
                else                                          // value is not 0
                    invalid_token(token, L"Cannot obtain value for unit literal -- integer value must be 0"); 
            }
            else                                              // not a valid uint8_t
            {
                invalid_token(token, L"Cannot obtain value for boolean literal"); 
            }
        }

        ///////////////// process boolean values ///////////////////////

        else if (int_length == 1)                             // boolean value? 
        {
            uint64_t local_uint64 { 0 };                      // local workarea for conversion

            rc = to_uint64(token.str, local_uint64, base);    // convert value into uint8_t field 

            if (rc == 0) 
            {
                if      (local_uint64 == 0)
                {
                    token.type = token_E::boolean;
                    token.boolean = false; 
                }
                else if (local_uint64 == 1)
                {
                    token.type = token_E::boolean;
                    token.boolean = true; 
                }
                else                                            // value is not 0 or 1
                {
                    invalid_token(token, L"Cannot obtain value for boolean literal -- integer value must be 0 or 1"); 
                }
            }
            else                                                // not a valid uint8_t
            {
                invalid_token(token, L"Cannot obtain value for boolean literal"); 
            }
        }

        //////////////////////////////////////////////////////////////////////////////   

        else if (int_length == 8)                                  // uint8_t value? 
        {
            rc = to_uint8(token.str, token.uint8, base);      // convert value into uint8_t field 

            if (rc == 0)                
                token.type = token_E::uint8;                
            else
                invalid_token(token, L"Cannot obtain value for uint8 literal");    
        }
        else if  (int_length == 16)                           // uint16_t value? 
        {
            rc = to_uint16(token.str, token.uint16, base);    // convert value into uint16_t field 

            if (rc == 0)                
                token.type = token_E::uint16;                
            else
                invalid_token(token, L"Cannot obtain value for uint16 literal");    
        }                                                                                                        

        else if  (int_length == 32)                           // uint32_t value? 
        {
            rc = to_uint32(token.str, token.uint32, base);    // convert value into uint32_t field 

            if (rc == 0)                
                token.type = token_E::uint32;                
            else
                invalid_token(token, L"Cannot obtain value for uint32 literal");    
        }
        else                                                  // must be uint64_t value? 
        {
            rc = to_uint64(token.str, token.uint64, base);    // convert value into uint64_t field 

            if (rc == 0)                
                token.type = token_E::uint64;                
            else
                invalid_token(token, L"Cannot obtain value for uint64 literal");    
        }            
    } 

    return rc; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// invalid_token() -- mark token as invalid and put out token-oriented error message 
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::invalid_token(token_C& token, const wchar_t *text_p) try
{
    token.type = token_E::invalid;

    if (!m_quiet_mode)
        M_out_emsg( L"%s -- file=\"%s\"  start=src:line:col(%d:%d:%d)  end=src:line:col(%d:%d:%d)  text=\"%s\"")
                  % text_p
                  % get_cached_id(token.source_id1)
                  % token.source_id1 % token.lineno1 % token.linepos1
                  % token.source_id2 % token.lineno2 % token.linepos2
                  % M_wsl(token.orig_str, 55)
                  ;  

    count_error();
    return;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// start_token() -- start new (empty) token  -- with specified type 
// -------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::start_token(token_C& token, token_E type, bool preserve_orig_str) try
{
    token.type = type; 
    token.str  = std::wstring{}; 
    
    if (!preserve_orig_str)
    {
        token.orig_str   = std::wstring{};  
        token.lineno1    = 0; 
        token.linepos1   = 0;
        token.lineno2    = 0; 
        token.linepos2   = 0;
        token.source_id1 = 0; 
        token.source_id2 = 0;
    }

    return;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  addto_token() -- append char to in-progress token  
//  -------------
//
//     switch token type to "error", if error character is added to token
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int token_stream_C::addto_token(token_C& token, const in_char_S& ch) try
{
     if (token.lineno1 == 0)   // not filled in yet (or this is very start of an input file)?
     {
         token.source_id1 = ch.source_id;
         token.lineno1    = ch.lineno; 
         token.linepos1   = ch.linepos;
     }

     token.source_id2 = ch.source_id;
     token.lineno2    = ch.lineno; 
     token.linepos2   = ch.linepos;

     if (ch.wch1 != (wchar_t)0x0000)
     {
         token.str      += ch.wch1;
         token.orig_str += ch.wch1;
     }
     if (ch.wch2 != (wchar_t)0x0000)
     {
         token.str      += ch.wch2;
         token.orig_str += ch.wch2;
     }

     if (token.only_char == (char32_t)0)    // not set yet
         token.only_char = ch.ch32; 
     
     if (ch.type == char_E::error)
     {
         token.type = token_E::error;
         count_error();
         m_error_seen = true; 
         return -1;
     }

     return 0;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  addto_token_subst() -- append substitute char(s) to in-progress token (but not to original_token)  
//  -------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// version with std::wstring substitute string 
// -------------------------------------------

int token_stream_C::addto_token_subst(token_C& token, const in_char_S& ch, const std::wstring& subst) try
{
     if (token.lineno1 == 0)   // not filled in yet (or this is very start of an input file)?
     {
         token.source_id1 = ch.source_id;
         token.lineno1    = ch.lineno; 
         token.linepos1   = ch.linepos;
     }

     token.source_id2 = ch.source_id;
     token.lineno2    = ch.lineno; 
     token.linepos2   = ch.linepos;

     token.str += subst;

     if (token.only_char == (char32_t)0)    // not set yet    ????????????????????????
         token.only_char = ch.ch32;         // note -- this is not the 1st char of substitute string ???????  what if substitute string is longer than 1 char??

     return 0;
}
M_endf


// version with char32_t substitute char (assumed to be 0000xxxx codepoint)  
// -------------------------------------

int token_stream_C::addto_token_subst(token_C& token, const in_char_S& ch, char32_t subst) try
{
     if (token.lineno1 == 0)   // not filled in yet (or this is very start of an input file)?
     {
         token.source_id1 = ch.source_id;
         token.lineno1    = ch.lineno; 
         token.linepos1   = ch.linepos;
     }

     token.source_id2 = ch.source_id;
     token.lineno2    = ch.lineno; 
     token.linepos2   = ch.linepos;

     token.str += (wchar_t)subst;

     if (token.only_char == (char32_t)0)    // not set yet    ????????????????????????
         token.only_char = subst; 

     return 0;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  addto_orig_token() -- append char to in-progress original token string, only  
//  ------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::addto_orig_token(token_C& token, const in_char_S& ch) try
{
    if (token.lineno1 == 0)   // not filled in yet (or this is very start of an input file)?
     {
         token.source_id1 = ch.source_id;
         token.lineno1    = ch.lineno; 
         token.linepos1   = ch.linepos;
     }

     token.source_id2 = ch.source_id;
     token.lineno2    = ch.lineno; 
     token.linepos2   = ch.linepos;

     if (ch.wch1 != (wchar_t)0x0000)     
         token.orig_str += ch.wch1;
    
     if (ch.wch2 != (wchar_t)0x0000)    
         token.orig_str += ch.wch2;   

     return;
}
M_endf





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  add_leading_sigil() -- add leading sigil to token (needs to be called before adding any other chars to token)  
//  ------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::add_leading_sigil(token_C& token, const in_char_S& ch_sigil) try
{
    // add sigil char to token string, as required

    if (m_preserve_sigils)
        addto_token(token, ch_sigil); 
    else
        addto_orig_token(token, ch_sigil);


    // flag this token as having a leading sigil

    token.has_leading_sigil = true;
    token.leading_sigil     = ch_sigil.ch32;                       // save sigil character away in output token 
    
    return;
}
M_endf





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  add_trailing_sigil() -- add trailing sigil to token (needs to be called after adding all other chars to token)  
//  -------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::add_trailing_sigil(token_C& token, const in_char_S& ch_sigil) try
{
    // add sigil char to token string, as required

    if (m_preserve_sigils)
        addto_token(token, ch_sigil); 
    else
        addto_orig_token(token, ch_sigil);


    // flag this token as having a trailing sigil

    token.has_trailing_sigil = true;
    token.trailing_sigil     = ch_sigil.ch32;                       // save sigil character away in output token 
    
    return;
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  past_end_token() -- went past end of token -- put last char back -- if error, add error info to token  
//  ---------------
//
//     switch token type to "error", if error character ended the token
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int token_stream_C::past_end_token(token_C& token, const in_char_S& ch) try
{
    int rc = 0; 

    putback_char(ch);

    if (ch.subtype == char_E::error)   // handle any error character that stopped the token -- becomes part of token info
    {
       addto_token(token, ch);         // will set error_seen flag
       rc = -1; 
    }

    return rc;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  one_char_token() -- complete processing for single-character token -- input char already peek()ed, but still on char_stream -- consume it now 
//  ----------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::one_char_token(token_C& token, token_E type, const in_char_S& ch) try
{
    start_token(token, type);                             
    addto_token(token, ch);

    discard_char();                                       // consume char for this token 
    return;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  escaped_char_token() -- complete processing for single-character token with escaped char -- escape char and escaped char already peek()ed, but still on char_stream  
//  --------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void token_stream_C::escaped_char_token(token_C& token, token_E type, const in_char_S& ch, const in_char_S& ch1, char32_t subst) try   
{
    M__(M_out(L"escaped_char_token() called");) 

    start_token(      token, type      );                  // start token with passed-in type based on substiture cher
    addto_orig_token( token, ch        );                  // add the escape character to orig token string only
    addto_orig_token( token, ch1       );                  // add the raw escaped character to orig token string only
    addto_token_subst(token, ch1, subst);                  // add the substitute char to token string only (not orig token string)  


    // consume the two characters left on the input stream when called 

    discard_char(2);                                       // escape char + escaped char
    M__(M_out(L"escaped_char_token() returning");) 
    return;
}
M_endf  



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  operator_token() -- gather up longest string of operator characters into a token   
//  ----------------
//
//               imbedded "//" starts a line comment, "/*"starts a block comment", "/~" starts an EOF comment, etc.  
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::operator_token(token_C& token, bool leading_sigil_present) try     
{
   int         rc = 0; 
   in_char_S   ch {}; 

   start_token(token, token_E::oper);


    // handle any leading sigil character -- if flag is on, 1st char is known to be sigil, and 2nd one is known to be alpha, currency, or separator 

    if (leading_sigil_present)                                          // add sigil to token (or just the original token), as requested 
    {                                                                     
        M__(M_out(L"operator_token() -- adding leading sigil");)          
        get_char(ch);                                                   // 1st one is known to be sigil character 
        add_leading_sigil(token , ch);                                  // add leading sigil to token
    }


   // look at characters that are part of operator itself (or start a comment)
   // ---------------------------------------------------

   for(;;)
   {
       get_char(ch);                                                    // 1st one is known to be an operator char
        

       // check for any comments starting in the middle of the apparent operator token -- these end the operator token immediately  

       if (ch.ch32 == m_comment_1st_ch)                                 // "/" -- might be start of comment
       {
           in_char_S ch1 {}; 
           peek_char(ch1);                                              // look at char following the 1st comment char = solidus?   
                                            
           if (
               (ch1.ch32 == m_unechoed_line_comment_2nd_ch  )           // "//"  -- must be start of line           comment
               ||                                                     
               (ch1.ch32 == m_echoed_line_comment_2nd_ch    )           // "/??" -- must be start of echoed line    comment
               ||                                                     
               (ch1.ch32 == m_eof_comment_2nd_ch            )           // "/]"  -- must be start of EOF            comment
               ||                                           
               (ch1.ch32 == m_block_comment_2nd_ch          )           // "/*"  -- must be start of block          comment
               ||                                                     
               (ch1.ch32 == m_suppress_eol_comment_2nd_ch   )           // "/??" -- must be start of suppress eol   comment
               ||                                                     
               (ch1.ch32 == m_nest_comment_2nd_ch           )           // "/["  -- must be start of nestable block comment
               ||                                           
               (ch1.ch32 == m_retained_line_comment_2nd_ch  )           // "/#"  -- must be start of retained line  comment
               ||                                                     
               (ch1.ch32 == m_retained_block_comment_2nd_ch )           // "/{"  -- must be start of retained block comment
               ||                                                     
               (ch1.ch32 == m_retained_eof_comment_2nd_ch   )           // "/}"  -- must be start of retained EOF   comment
              )                                                 
           {
                M__(M_out(L"operator_token() -- comment starting while gathering operator token");)

                //   start of comment is not part of the operator token
                rc = past_end_token(token, ch);                         // put back first "/", etc. for next time
                break; 
           }           
       }


       // not the start of a comment in the middle of the operator string

       if (ch.type == char_E::oper)                
           addto_token(token, ch); 

       else

       // reached a character that is trailing sigil or not part of operator at all 
                                                         
       {       
           M__(M_out(L"operator_token() -- operator-ending char = %d") % (int)(ch.ch32)); 
       
           if ( m_allow_trailing_op_sigils && is_in_vector(ch.ch32, m_trailing_oper_sigils) )
           {
               M__(M_out(L"operator_token() -- adding trailing sigil");)
               add_trailing_sigil(token, ch);               
           }
           else    // not a trailing sigil -- usual token ending -- put character back, etc.
           {
               rc = past_end_token(token, ch);  
           }

           break;
       }   
   }

   return rc; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  same_operator_token() -- gather up longest string of dot/comma,etc. characters into an operator token (all chars in token must be same (ex: ...   ,,  ''''', etc.) -or- \ (backslash) 
//  ---------------------
//                
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::same_operator_token(token_C& token, const in_char_S& op_char, bool leading_sigil_present) try     
{
   int         rc = 0; 
   in_char_S   ch {}; 

   start_token(token, token_E::oper);


    // handle any leading sigil character -- if flag is on, 1st char is known to be sigil, and 2nd one is known to be a same operator character 

    if (leading_sigil_present)                                   // add sigil to token (or just the original token), as requested 
    {   
        M__(M_out(L"same_operator_token() -- adding leading sigil");)
        get_char(ch);                                            // 1st one is known to be sigil character 
        add_leading_sigil(token , ch);                           // add leading sigil to token
    }


   // look at characters that are part of same char operator itself
   // -------------------------------------------------------------

   for(;;)
   {
       get_char(ch);                                                     // 1st one is known to be an operator char         

       if (ch.ch32 == op_char.ch32)                                      // same char as 1st one is allowed in same-char operator token
           addto_token(token, ch);  

       else if ( is_in_vector(ch.ch32, m_in_same_char_op) )              // -or- is in the list of extra characters allowed in same-char operator tokens 
           addto_token(token, ch); 

       else

       // reached a character that is trailing sigil or not part of operator at all 
                                                         
       {       
           M__(M_out(L"same_operator_token() -- operator-ending char = %d") % (int)(ch.ch32)); 
       
           if ( m_allow_trailing_op_sigils && is_in_vector(ch.ch32, m_trailing_oper_sigils ) )
           {
               M__(M_out(L"same_operator_token() -- adding trailing sigil");)
               add_trailing_sigil(token, ch);               
           }
           else    // not a trailing sigil -- usual token ending -- put character back, etc.
           {
               rc = past_end_token(token, ch);  
           }

           break;
       }   
   }

   return rc; 
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  whitespace_token() -- gather up pure whitespace characters (no comments)   
//  ------------------
//  
//  "\" allowed only if followed by more whitespace characters and then EOL (or EOF) -- causes EOL to be ignored
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::whitespace_token(token_C& token) try      
{
    int        rc               {0    }; 
    in_char_S  ch               {     }; 
    bool       escape_seen      {false}; 
    bool       whitespace_seen  {false}; 

    start_token(token, token_E::whitespace);

    for(;;)
    {
        get_char(ch);      // 1st one known to be whitespace char
             
        if (ch.subtype == char_E::whitespace)
        {
            addto_orig_token(token, ch);
            whitespace_seen = true;   
        }
        else if (ch.subtype == char_E::f_continuation) 
        {
            addto_orig_token(token, ch);
            escape_seen = true;        
        }
        else if (ch.subtype == char_E::eol)
        {
            if (escape_seen)
            {
                break;   // don't put back the EOL -- just end the token accumulation with no putback  
            }
            else
            {
                rc = past_end_token(token, ch);   // put back EOL for next time -- end the toke accumulation                  
                break;
            }        
        }
        else if (
                 (ch.subtype == char_E::eof)
                 ||
                 (ch.subtype == char_E::error)
                )
        {                                        // error or EOF is ending the whitespace -- don't action the escape (if any), but don't complain either   
            rc = past_end_token(token, ch);      // put back EOL for next time -- end the toke accumulation                  
            break;
        }
        else                                     // normal (non-whitespace) characters are ending the whitespace -- any escape is invalid -- complain
        {
            if (escape_seen)
            {
                if (whitespace_seen)       // escape character as part of whitespace not being ended because of EOL
                    invalid_token(token, L"Escape/continuation character was seen in whitespace, but not at end of line");               
                else                       // escape character, but not part of any whitespace
                    invalid_token(token, L"Unexpected escape/continuation character was seen prior to end of line");               
            }

            rc = past_end_token(token, ch);   // went past end of token                  
            break;
        }           
    }

    if (token.type == token_E::whitespace)
        token.str = L" ";                      // passed-back string for whitespace should be one blank character

    return rc; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  identifier_token() -- gather up pure identifier (extended identifier, if currency symbols are present and allowed)   
//  ------------------
//
//                       
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::identifier_token(token_C& token, bool leading_sigil_present) try      
{
    int       rc            {0    }; 
    in_char_S ch            {     }; 
    
    start_token(token, token_E::identifier);                     // start with simple identifier -- may get modified below


    // handle any leading sigil character -- if flag is on, 1st char is known to be sigil, and 2nd one is known to be alpha, currency, or separator 

    if (leading_sigil_present)                                   // add sigil to token (or just the original token), as requested 
    {  
        M__(M_out(L"identifier_token() -- adding leading sigil");)
        get_char(ch);                                            // 1st one is known to be sigil character 
        add_leading_sigil(token , ch);                           // add leading sigil to token
    }


    // main loop to gather up rest of identifier
    // -----------------------------------------
    
    for(;;)
    {
        get_char(ch);                                            // 1st one known to be not a digit -- must be alpha, currency, or separator
               
        if (
            (ch.type    == char_E::alpha )
            ||
            (ch.type    == char_E::sep   )
            ||
            (ch.type    == char_E::num   )
           ) 
        {
             addto_token(token, ch);
        }
        else if (
                  (m_allow_extd_identifier)                      // only include accents and currency if extended identifiers are allowed
                  &&
                  (ch.type == char_E::currency)                  // if currency symbol is present, flag as "extended identifier", if parser wants to know
                )
        {
            addto_token(token, ch);
            if (m_report_extd_identifier)
                token.type = token_E::extended_identifier;
        }
        else 
            
        // reached a character that is trailing sigil or not part of identifier at all 

        {       
            M__(M_out(L"identifier_token() -- identifier-ending char = %d") % (int)(ch.ch32)); 

            if ( m_allow_trailing_id_sigils && is_in_vector(ch.ch32, m_trailing_ident_sigils) ) 
            {
                M__(M_out(L"identifier_token() -- adding trailing sigil");)
                add_trailing_sigil(token, ch);               
            }
            else    // not a trailing sigil -- usual token ending -- put character back, etc.
            {
                // set attached_paren flag, if this identifier token ended because of an adjacent parenthesis (no intervening whitespace)

                if ( (m_allow_attached_paren) && (ch.subtype == char_E::open_paren1) )
                {
                    M__(M_out(L"identifier_token() -- attached parenthesis");) 
                    token.attached_paren = true; 
                }

                rc = past_end_token(token, ch);  
            }

            break;
        }           
    }

    return rc; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  line_comment() -- gather up line comment token   (preserve EOL or consume it based on input parm) -- echo it, if required
//  --------------
//
//      "//" ,  has been peek()ed before this routine is called, but not consumed              
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::line_comment(token_C& token, bool no_eol, bool echo, size_t intro_len) try       
{
    int         rc = 0; 
    in_char_S   ch {}; 

    m_char_stream_p->set_process_trigraphs(false);      // suppress trigraph processing in comments

    start_token(token, m_comment_is_whitespace ? token_E::whitespace : token_E::line_comment); 

    for(;;)   // loop until EOL, EOF, or error stops the line comment
    {
        get_char(ch);                                   // first 1 or 2 can be   "//"  or      (defaults)
            
        if (
            (ch.subtype == char_E::error)
            ||
            (ch.subtype == char_E::eof  )            
           )
        {
            rc = past_end_token(token, ch);             // went past end of token                  
            break;                                      
        }                                               
        else if (ch.subtype == char_E::eol  )           
        {                                               
            if (no_eol)                                 // want to get rid of this EOL ??
                ;                                       // yes -- do not put back EOL -- next token will be at start of new line 
            else                                        
                rc = past_end_token(token, ch);         // no -- keep EOL by putting it back for next time  
            break;                                      // stop accumulating line comment in either case
        }                                               
        else                                            
        {                                               
            addto_orig_token(token, ch);                 // add char to to original token (for error messages) but not main token string
        }           
    } 

    m_char_stream_p->set_process_trigraphs(true);        // re-enable trigraph processing, now that comment is done


    // if required, echo the comment text, if any characters on line following the line comment introducer 
    // --------------------------------------------------------------------------------------------------- 

    if ( echo && (!m_suppress_echo) )
    {
        if (token.orig_str.size() > intro_len)  
            M_out(L"%s") % token.orig_str.substr(intro_len);
        else
            M_out(L"%s") % L""; 
    }

    return rc; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  retained_line_comment() -- gather up retained line comment token   
//  -----------------------
//
//      "/#" ,  has been peek()ed before this routine is called, but not consumed
//
//      note: trigraph processing is not disabled in retained line comments, which are passed back up to the parser for processing
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::retained_line_comment(token_C& token) try       
{
    int         rc { 0 }; 
    in_char_S   ch {   }; 
    int         ct { 0 };

    start_token(token, token_E::retained_line_comment); 

    for(;;)   // loop until EOL, EOF, or error stops the retained line comment
    {
        get_char(ch);                                   // first 1 or 2 can be   "/#"  (defaults)
            
        if (
            (ch.subtype == char_E::error)
            ||
            (ch.subtype == char_E::eof  )            
           )
        {
            rc = past_end_token(token, ch);             // went past end of token                  
            break;                                      
        }                                               
        else if (ch.subtype == char_E::eol  )           
        {                                               
            break;                                      // stop accumulating retained line comment in either case
        }                                               
        else                                            
        {   
            ct++;                                       // increment valid char counter

            if (ct > 2)                                 // don't include 1st two chars ("/#") in output token
               addto_token(token, ch);                  // add char to to output token 
            else                                        // 1st two chars
               addto_orig_token(token, ch);             // always add characters to to original token (for error messages), but not the output token
        }           
    } 
   
    return rc; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  retained_eof_comment() -- gather up retained EOF comment token   
//  ----------------------
//
//      "/}" ,  has been peek()ed before this routine is called, but not consumed
//
//      note: trigraph processing is not disabled in retained line comments, which are passed back up to the parser for processing
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::retained_eof_comment(token_C& token) try       
{
    int         rc { 0 }; 
    in_char_S   ch {   }; 
    int         ct { 0 };

    M__(M_out(L"retained_eof_comment() -- called");)

    start_token(token, token_E::retained_eof_comment); 

    for(;;)   // loop until EOF, or error stops the retained line comment
    {
        get_char(ch);                                               // first 1 or 2 can be   "/#"  (defaults)
                                                                   
        if (ch.subtype == char_E::eof)                              // normal end of EOF comment
        {   
            M__(M_out(L"retained_eof_comment() -- EOF reached");)
            past_end_token(token, ch);                              // EOF is past end of token, so put it back for next time
            break;                                                  // end the for(;;) loop
        }  
        else if (ch.subtype == char_E::eol)
        {
            M__(M_out(L"retained_eof_comment() -- adding new-line");)
            addto_orig_token(token, ch );                           // add the EOL char to orig token string only
            addto_token_subst(token, ch, std::wstring {L"\n"});     // add the substitute NL char(s) to token only (not orig_token)
        } 
        else if (ch.subtype == char_E::error)        
        {
            rc = past_end_token(token, ch);                         // went past end of token                  
            break;                                                  
        }                                                           
        else if (ch.subtype == char_E::eol  )                       
        {                                                           
            break;                                                  // stop accumulating retained line comment in either case
        }                                                           
        else                                                        
        {                                                         
            ct++;                                                   // increment valid char counter
            M__(M_out(L"retained_eof_comment() -- adding char -- ct=%d") % ct;) 

            if (ct > 2)                                             // don't include 1st two chars ("/#") in output token
               addto_token(token, ch);                              // add char to to output token 
            else                                                    // 1st two chars
               addto_orig_token(token, ch);                         // always add characters to to original token (for error messages), but not the output token
        }           
    } 
   
    return rc; 
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  eof_comment() -- gather up EOF comment token   
//  -------------
//              
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::eof_comment(token_C& token) try     
{
    int         rc = 0; 
    in_char_S   ch  {}; 
    in_char_S   ch1 {};

    m_char_stream_p->set_process_trigraphs(false);       // suppress trigraph processing in comments

    start_token(token, m_comment_is_whitespace ? token_E::whitespace : token_E::eof_comment); 


    //  consume initial "/-" and add to original token  (already peeked)

    get_char(ch);                                        // known to be "/"
    get_char(ch1);                                       // known to be "*"
    addto_orig_token(token, ch);                         // add the initial "/" to start of original comment string 
    addto_orig_token(token, ch1);                        // add the initial "*" to start of original comment string

                
    for(;;)   // loop until  EOF, or error stops the EOF comment 
    {
        get_char(ch);      

        if (ch.subtype == char_E::eof)                   // normal end of EOF comment
        {
            past_end_token(token, ch);                   // EOF is past end of token, so put it back for next time
            break;                                       // end the for(;;) loop
        }
        else if (ch.subtype == char_E::error)
        {
            rc = past_end_token(token, ch);              // put back error indication for next time -- comment token type replaced with "error"
            break;                                       // end the for(;;) loop 
        }
        else                                             // not error, EOF, or "/*" -- just keep adding to token 
        {
            addto_orig_token(token, ch);                 // add char to to original token string only
        }           
    }


    m_char_stream_p->set_process_trigraphs(true);        // re-enable trigraph processing, now that comment is done
    return rc; 
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  block_comment() -- gather up block comment token   
//  ---------------
//              
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::block_comment(token_C& token) try     
{
    int         rc = 0; 
    in_char_S   ch  {}; 
    in_char_S   ch1 {};

    m_char_stream_p->set_process_trigraphs(false);                // suppress trigraph processing in comments

    start_token(token, m_comment_is_whitespace ? token_E::whitespace : token_E::block_comment); 


    //  consume initial "/*" to avoid allowing "/*/"as a valid block comment (already peeked) -- add these to original token  

    get_char(ch);                                                 // known to be "/"
    get_char(ch1);                                                // known to be "*"
    addto_orig_token(token, ch);                                  // add the initial "/" to start of original comment string 
    addto_orig_token(token, ch1);                                 // add the initial "*" to start of original comment string

                
    for(;;)   // loop until "*/",  EOF, or error stops the line comment -- note "/*/" is smallest comment
    {
        get_char(ch);      
               
        if (ch.ch32 == m_block_comment_3rd_ch)                    // "*" found -- might be end of comment 
        {
            peek_char(ch1);                                       // look at character following the "*"
            if (ch1.ch32 == m_block_comment_4th_ch)               // "*/" ends the comment 
            {
                addto_orig_token(token, ch);                      // add the "*" to end of original comment string 
                addto_orig_token(token, ch1);                     // add the "/" to end of original comment string
                discard_char();                                   // discard the peek()ed "/"                 
                break;                                           
            }                                                    
            else                                                  // not "*/" -- not end of comment
            {                                                    
                addto_orig_token(token, ch);                      // add "*" to original comment string
            }
        }
        else if (ch.subtype == char_E::eof)
        {
            invalid_token(token, L"Block comment was still open when end-of-file was reached");
            past_end_token(token, ch);                            // EOF is past end of token, so put it back
            break;                                                // end the for(;;) loop
        }                                                       
        else if (ch.subtype == char_E::error)                   
        {                                                       
            rc = past_end_token(token, ch);                       // put back error indication for next time -- comment token type replaced with "error"
            break;                                                // end the for(;;) loop 
        }                                                       
        else                                                      // not error, EOF, or "/*" -- just keep adding to token 
        {                                                       
            addto_orig_token(token, ch);                          // add char to to original token string only
        }                                                       
    }                                                           
                                                                
                                                                
    m_char_stream_p->set_process_trigraphs(true);                 // re-enable trigraph processing, now that comment is done
    return rc; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  nestable_comment() -- gather up nestable block comment token   
//  ------------------
//              
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::nestable_comment(token_C& token)  try     
{
    int              rc    {0}; 
    in_char_S        ch    { }; 
    in_char_S        ch1   { };
    uint64_t    nest_level {0};        

    m_char_stream_p->set_process_trigraphs(false);       // suppress trigraph processing in comments

    start_token(token, m_comment_is_whitespace ? token_E::whitespace : token_E::nestable_comment); 


    //  consume initial "/<"  before starting   

    get_char(ch);                                         // known to be "/"      (default)
    get_char(ch1);                                        // known to be "<"      (default)
    addto_orig_token(token, ch);                          // add the initial "/" to start of original comment string 
    addto_orig_token(token, ch1);                         // add the initial "<" to start of original comment string
    nest_level = 1;
                
    for(;;)   // loop until ">/" at proper nest level,  EOF, or error stops the nestable block comment -- note "/<>/" is smallest nestable comment
    {
        get_char(ch);      
          
        if (ch.ch32 == m_comment_1st_ch)                  // "/" found -- might be start of inner nested comment 
        {                                              
            peek_char(ch1);                            
            if (ch1.ch32 == m_nest_comment_2nd_ch)        // "/<"found -- increment nest level
            {                                          
                addto_orig_token(token, ch);              // add the "/" to end of original comment string 
                addto_orig_token(token, ch1);             // add the "/<" to end of original comment string
                discard_char();                           // discard the peek()ed "<"   ( "/" was consumed earlier)
                ++nest_level;                          
            }                                          
            else                                          // not "/<" -- not start of nested comment
            {                                          
                addto_orig_token(token, ch);              // add "/" to original comment string
            }                                          
        }                                              
        else if (ch.ch32 == m_nest_comment_3rd_ch)        // ">" found -- might be end of comment 
        {
            peek_char(ch1);                               // look at character following the ">"
            if (ch1.ch32 == m_nest_comment_4th_ch)        // ">/" ends the nested comment 
            {
                addto_orig_token(token, ch);              // add the ">" to end of original comment string 
                addto_orig_token(token, ch1);             // add the "/" to end of original comment string
                discard_char();                           // discard the peek()ed "/"   
                --nest_level;

                if (nest_level == 0)                      // if this ">/" matched outermost "/<", block comment is done , otherwise keep going 
                    break;                                // end the for(;;) loop
            }                                          
            else                                          // not ">/" -- not end of comment
            {                                             
                addto_orig_token(token, ch);              // add ">" to original comment string
            }
        }
        else if (ch.subtype == char_E::eof)
        {
            invalid_token(token, L"Nestable block comment was still open when end-of-file was reached");
            past_end_token(token, ch);                   // EOF is past end of token, so put it back
            break;                                       // end the for(;;) loop
        }                                               
        else if (ch.subtype == char_E::error)
        {
            rc = past_end_token(token, ch);              // put back error indication for next time -- comment token type replaced with "error"
            break;                                       // end the for(;;) loop 
        }                                                
        else                                             // not error, EOF, or "/" or ">" -- just keep adding to token 
        {                                                
            addto_orig_token(token, ch);                 // add char to original token string only
        }                                                
    }
    

    m_char_stream_p->set_process_trigraphs(true);        // re-enable trigraph processing, now that comment is done
    return rc; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  retained_block_comment() -- gather up (nestable) retained block comment token     
//  ------------------------
//
//      note: trigraph processing is not turned off in retained comments, that are passed up to the parser for processing
//              
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::retained_block_comment(token_C& token)  try     
{
    int              rc    {0}; 
    in_char_S        ch    { }; 
    in_char_S        ch1   { };
    uint64_t    nest_level {0};        

    
    start_token(token, token_E::retained_block_comment); 


    //  consume initial "/{"  before starting   

    get_char(ch);                                                   // known to be "/"   (default)
    get_char(ch1);                                                  // known to be "{"   (default)
    addto_orig_token(token, ch);                                    // add the initial "/" to start of original comment string 
    addto_orig_token(token, ch1);                                   // add the initial "{" to start of original comment string
    nest_level = 1;                                               
                
    for(;;)   // loop until "}/" at proper nest level,  EOF, or error stops the retained block comment -- note "/{}/" is smallest retained block comment
    {
        get_char(ch);      
          
        if (ch.ch32 == m_comment_1st_ch)                            // "/" found -- might be start of inner nested comment 
        {                                                          
            peek_char(ch1);                                        
            if (ch1.ch32 == m_retained_block_comment_2nd_ch)        // "/{" found -- increment nest level
            {                                                      
                addto_token(token, ch);                             // add the "/" to end of retained comment string 
                addto_token(token, ch1);                            // add the "{" to end of retained comment string
                discard_char();                                     // discard the peek()ed "{"   ( "/" was consumed earlier)
                ++nest_level;                                       
            }                                                       
            else                                                    // not "/{" -- not start of nested comment
            {   
                addto_token(token, ch);                             // add "/" to retained comment string
            }                                                       
        }                                                           
        else if (ch.ch32 == m_retained_block_comment_3rd_ch)        // "}" found -- might be end of comment 
        {                                                           
            peek_char(ch1);                                         // look at character following the "}"
            if (ch1.ch32 == m_retained_block_comment_4th_ch)        // "}/" ends the nested comment 
            {                                                      
                --nest_level;                                       // reduce nest level by one
                discard_char();                                     // discard the peek()ed "/"   
                                                                   
                if (nest_level > 0)                                 // looking at nested "}/ --  not looking at final "}/" 
                {                                                  
                    addto_token(token, ch);                         // add the "{" to end of original comment string 
                    addto_token(token, ch1);                        // add the "/" to end of original comment string
                }                                                  
                else                                                // This "}/" matched outermost "/{", block comment is done , otherwise keep going 
                {                                                  
                    addto_orig_token(token, ch);                    // add the "}" to end of original comment string, but not output token 
                    addto_orig_token(token, ch1);                   // add the "/" to end of original comment string, but not output token
                    break;                                          // end the for(;;) loop
                }                                                  
            }                                                       
            else                                                    // not "}/" -- not end of comment
            {                                                       
                addto_token(token, ch);                             // add "}" to output retained comment string
            }                                                       
        }
        else if (ch.subtype == char_E::eol)
        {
            addto_orig_token(token, ch );                           // add the EOL char to orig token string only
            addto_token_subst(token, ch, std::wstring {L"\n"});     // add the substitute NL char(s) to token only (not orig_token)
        } 
        else if (ch.subtype == char_E::eof)
        {
            invalid_token(token, L"Retained block comment (with pre-processor data) was still open when end-of-file was reached");
            past_end_token(token, ch);                              // EOF is past end of token, so put it back
            break;                                                  // end the for(;;) loop
        }   
        else if (ch.subtype == char_E::error)               
        {                                                   
            rc = past_end_token(token, ch);                         // put back error indication for next time -- comment token type replaced with "error"
            break;                                                  // end the for(;;) loop 
        }                                                           
        else                                                        // not error, EOF, or "/" or ">" -- just keep adding to token 
        {                                                           
            addto_token(token, ch);                                 // add char to retained  token string only
        }                                                          
    }
    
    M__(token.display(L"token_stream_C::retained_block_comment() returning");)
    return rc; 
}
M_endf
 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  word_string() -- gather up single-word string token with no escape sequences -- string ends with 1st whitespace  
//  -------------
//              
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::word_string(token_C& token) try     
{
    int         rc = 0; 
    in_char_S   ch  {}; 
    
    start_token(token, token_E::string); 
    get_char(ch);                                          // consume 1st character, which is not part of the string contents -- should be the opening delimiter (allow out-of-string escape processing)
    addto_orig_token(token, ch);                           // add the leading string delimiter to original token text for error messages only 
                                                          
    for(;;)                                                // loop until white space, EOL, EOF, or error stops the line comment 
    {                                                     
        get_char(ch);                                                          
                                                          
        if (ch.subtype == char_E::whitespace)              // whitespace char ??
        {  
            past_end_token(token, ch);                     // whitespace char is past end of token, so put it back for next time
            break;                                         // end the for(;;) loop               
        }
        else if (ch.subtype == char_E::eof)                // EOF ??
        {
            past_end_token(token, ch);                     // EOF is past end of token, so put it back for next time
            break;                                         // end the for(;;) loop
        }
        else if (ch.subtype == char_E::eol)                // EOL ??
        {
            past_end_token(token, ch);                     // EOL is past end of token, so put it back for next time
            break;                                         // end the for(;;) loop
        }
        else if (ch.subtype == char_E::error)              // error??
        {
            rc = past_end_token(token, ch);                // put back error indication for next time -- comment token type replaced with "error"
            break;                                         // end the for(;;) loop 
        }                                                 
        else                                               // not error, EOF, EOL, or whitespace -- just keep adding to string contents 
        {                                                 
            addto_token(token, ch);                        // add char to to string contents
        }                                                 
    }


    m_char_stream_p->set_process_trigraphs(true);          // restore normal trigraph processing after string is done
    return rc; 
}
M_endf






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  simple_string() -- gather up simple string token with no escape sequences  (supports doubled-up ending delimiter)     trigraphs are suppressed in string
//  ---------------
//              
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::simple_string(token_C& token, char32_t end_delim) try     
{
    int         rc = 0; 
    in_char_S   ch  {}; 
    
    m_char_stream_p->set_process_trigraphs(false);         // suppress trigraph processing in simple strings

    start_token(token, token_E::string, true);             // note: at this time, the token.orig_str may have the "M" or "R"  -- this needs to be preserved 
    get_char(ch);                                          // consume 1st character, which is not part of the string contents -- should be the opening delimiter (allow out-of-string escape processing)
    addto_orig_token(token, ch);                           // add the leading string delimiter to original token text for error messages only 
                                                          
    for(;;)                                                // loop until end_delim, EOL, EOF, or error stops the line comment 
    {                                                     
        get_char(ch);                                                          
                                                          
        if (ch.ch32 == end_delim)                          // ending delimiter found -- might be end of string (already consumed)
        {                                       
            in_char_S ch1 {}; 
            m_char_stream_p->set_process_trigraphs(true);  // restore normal trigraph processing before (perhaps) peeking next char after the string end
            peek_char(ch1);                                // look at character following the ending delimiter (any trigraphed char will not be another string delimiter)
            m_char_stream_p->set_process_trigraphs(false); // suppress trigraph processing after peeking next char 
                                                          
            if (ch1.ch32 == end_delim)                     // doubled-up ending delimiter? --  replace with one copy of ending delimiter
            {                                             
                addto_orig_token(token, ch);               // add the 1st ending delimiter in_char to orig token string only
                addto_token(token, ch1);                   // add the 2nd ending delimiter in_char to token (to get latest ending column)  
                discard_char();                            // consume the 2nd delimiter, since it was just peek()ed
            }                                             
            else                                           // not doubled-up ending delimiter -- must be end of string
            {                                             
                addto_orig_token(token, ch);               // add ending delimiter to orig string only, for error messages
                break;
            }
        }
        else if (ch.subtype == char_E::eof)
        {
            invalid_token(token, L"String was still open when End-of-file was reached");           
            past_end_token(token, ch);                     // EOF is past end of token, so put it back
            break;                                         // end the for(;;) loop
        }
        else if (ch.subtype == char_E::eol)
        {
            invalid_token(token, L"String was still open when End-of-line was reached");            
            past_end_token(token, ch);                     // EOL is past end of token, so put it back
            break;                                         // end the for(;;) loop
        }
        else if (ch.subtype == char_E::error)
        {
            rc = past_end_token(token, ch);                // put back error indication for next time -- comment token type replaced with "error"
            break;                                         // end the for(;;) loop 
        }                                                 
        else                                               // not error, EOF, EOL, or ending delimiter -- just keep adding to string contents 
        {                                                 
            addto_token(token, ch);                        // add char to to string contents
        }                                                 
    }


    m_char_stream_p->set_process_trigraphs(true);          // restore normal trigraph processing after string is done
    return rc; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  escaped_string() -- gather up string literal token -- with escape sequences   
//  ----------------
//              
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::escaped_string(token_C& token, char32_t end_delim, char32_t escape_char, bool include_nl) try     
{
    int         rc = 0; 
    in_char_S   ch  {}; 
    
    M__(M_out(L"escaped_string() -- called");)

    start_token(token, token_E::string, true);  // note: at this time, the token.orig_str may have the "M" or "R"  -- this needs to be preserved 
    get_char(ch);                               // consume 1st character, which is not part of the string contents -- should be the opening delimiter
    addto_orig_token(token, ch);                // add the leading string delimiter to original token text for error messages only 
    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //   Main loop to look at each character in string literal
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for(;;)   // loop until end_delim, EOL, EOF, or error stops the escaped string 
    {
        get_char(ch);      
               
        if (ch.ch32 == end_delim)              // (unescaped) ending delimiter found -- should be end of string (already consumed)        
        {
             addto_orig_token(token, ch);      // add ending delimiter to orig string only, for error messages
             break;
        } 


        // ===============================================================================================================================================
        // Handle escape sequence --  \"   \\   \a   \b   \f   \n   \r   \t   \v    \xNNNN, etc.
        // ===============================================================================================================================================

        else if (ch.ch32 == escape_char)       // escape character found
        {
            M__(M_out(L"escaped_string() -- processing escape character");)

            in_char_S ch1 {};    
            peek_char(ch1);                                             // look at character following the escape character
                                                                      
            if (  (ch1.ch32 == end_delim)                               // escaped ending delimiter? -- replace with one copy of ending delimiter
               || (ch1.ch32 == escape_char)                             // escaped escape char?      -- replace with one copy of escape character                 
               )                                                      
            {                                                         
                addto_orig_token(token, ch);                            // add the escape character (\) in_char to orig token string only
                addto_token(token, ch1);                                // add the ending delimiter (") or 2nd escape char (\) to token   
                discard_char();                                         // consume the escaped character (which was peek()ed) 
            }
            else
            if (  (ch1.ch32 == const_N::ch_alarm_escape     ) )         // look for \a -- escaped alarm
            {
                M__(M_out(L"escaped_string() -- processing escaped alarm");)
                addto_orig_token(token, ch );                           // add the escape character (\) to orig token string only
                addto_orig_token(token, ch1);                           // add the raw escaped character (a) to orig token string only
                addto_token_subst(token, ch1, std::wstring {L"\a"});    // add the substitute BS char(s) to token only (not orig_token)
                discard_char();                                         // consume the escaped character (which was peek()ed)  
            }
            else
            if (  (ch1.ch32 == const_N::ch_backspace_escape ) )         // look for \b -- escaped backspace
            {
                M__(M_out(L"escaped_string() -- processing escaped backspace");)
                addto_orig_token(token, ch );                           // add the escape character (\) to orig token string only
                addto_orig_token(token, ch1);                           // add the raw escaped character (b) to orig token string only
                addto_token_subst(token, ch1, std::wstring {L"\b"});    // add the substitute BS char(s) to token only (not orig_token)
                discard_char();                                         // consume the escaped character (which was peek()ed)  
            }
            else
            if (  (ch1.ch32 == const_N::ch_form_feed_escape) )          // look for \f -- escaped form-feed
            {
                M__(M_out(L"escaped_string() -- processing escaped form-feed");)
                addto_orig_token(token, ch );                           // add the escape character (\) to orig token string only
                addto_orig_token(token, ch1);                           // add the raw escaped character (f) to orig token string only
                addto_token_subst(token, ch1, std::wstring {L"\f"});    // add the substitute FF char(s) to token only (not orig_token)
                discard_char();                                         // consume the escaped character (which was peek()ed)  
            }
            else
            if (  (ch1.ch32 == const_N::ch_new_line_escape) )           // look for \n -- escaped new-line
            {
                M__(M_out(L"escaped_string() -- processing escaped new-line");)
                addto_orig_token(token, ch );                           // add the escape character (\) to orig token string only
                addto_orig_token(token, ch1);                           // add the raw escaped character (n) to orig token string only
                addto_token_subst(token, ch1, std::wstring {L"\n"});    // add the substitute NL char(s) to token only (not orig_token)
                discard_char();                                         // consume the escaped character (which was peek()ed)  
            }
            else
            if (  (ch1.ch32 == const_N::ch_carriage_return_escape) )    // look for \r -- escaped carriage-return
            {
                M__(M_out(L"escaped_string() -- processing escaped carriage-return");)
                addto_orig_token(token, ch );                           // add the escape character (\) to orig token string only
                addto_orig_token(token, ch1);                           // add the raw escaped character (r) to orig token string only
                addto_token_subst(token, ch1, std::wstring {L"\r"});    // add the substitute CR char(s) to token only (not orig_token)
                discard_char();                                         // consume the escaped character (which was peek()ed)  
            }
            else
            if (  (ch1.ch32 == const_N::ch_tab_escape) )                // look for \t -- escaped horizontal tab
            {
                M__(M_out(L"escaped_string() -- processing escaped horizontal tab");)
                addto_orig_token(token, ch );                           // add the escape character (\) to orig token string only
                addto_orig_token(token, ch1);                           // add the raw escaped character (t) to orig token string only
                addto_token_subst(token, ch1, std::wstring {L"\t"});    // add the substitute TAB char(s) to token only (not orig_token)
                discard_char();                                         // consume the escaped character (which was peek()ed)  
            }
            else
            if (  (ch1.ch32 == const_N::ch_vertical_tab_escape) )       // look for \v -- escaped vertical-tab
            {
                M__(M_out(L"escaped_string() -- processing escaped vertical tab");)
                addto_orig_token(token, ch );                           // add the escape character (\) to orig token string only
                addto_orig_token(token, ch1);                           // add the raw escaped character (v) to orig token string only
                addto_token_subst(token, ch1, std::wstring {L"\v"});    // add the substitute VT char(s) to token only (not orig_token)
                discard_char();                                         // consume the escaped character (which was peek()ed)  
            }
            else


            //   handle /xNNNN hex data escape sequence
            //   --------------------------------------

            if (  (ch1.ch32 == const_N::ch_hex_escape) )                // look for \x -- escape for 4-char hexadecimal data
            {
                M__(M_out(L"escaped_string() -- processing escaped hex data");)

                bool         bad_hex      {false};                      // local flag to indicate bad hex data
                std::wstring hex_digits   {     };                      // string of 4 hex digits for to_uint32()
                in_char_S    chx[4]       {     };                      // saved hex in_char_S for addition to original token string 
                in_char_S    chxn         {     };                      // single inchar_S to get next hex digit
                uint32_t     hex_value    {  0  };                      // converted hex value of \xNNNN   

                // inner loop to gather up 4 hex digits (NNNN)

                for (auto ix = 0; ix < 4; ix++)
                {
                    peek_char(chxn, ix + 2);                            // char 1 = x, char 2, 3, 4, 5 = N, N, N, N 

                    // stop \x processing, if error, EOF, etc., or this is not a valid base-16 (hex) digit)

                    if  (
                         ( chxn.classs != char_E::normal ) 
                         ||
                         ( !is_digit(chxn.ch32, 16)      )
                        )
                    {
                        M__(M_out(L"escaped_string() -- bad escaped hex digit[%d] -- classs=%d   ch32=%08X") % ix % ((int32_t)(chxn.classs)) % ((uint32_t)(chxn.ch32));)
                        bad_hex = true;                                 // indicate invalid hex escape sequence
                        break;                                          // break out of inner do loop
                    }
                
                    chx[ix]     = chxn;                                 // save incoming char in array, for possible addition to orig token string, if everything is OK                                   
                    hex_digits += chxn.wch1;                            // add this validated hex digit to string of he digits -- assume no surrogate pairs for valid hex digits                 
                }


                // convert hex digits to unsigned numeric value, if all 4 digits were valid

                if (!bad_hex)
                {
                   M__(M_out(L"escaped_string() -- converting escaped hex digits to uint32_t");)
                   if ( 0 != to_uint32(hex_digits, hex_value, 16) )     // convert NNNN string into uint32_t value -- should not be any problems here  
                       bad_hex = true;                                  // this shouldn't get set here
                }


                // if hex data is not good, assume this is not a hex escape sequence, just normal \ , x, digit, digit, etc. in string -- don't complain 

                if (bad_hex)
                {
                    M__(M_out(L"escaped_string() -- ignoring invalid hex escape sequence");)
                    addto_token(token, ch);                             // just add escape char (\) to to string contents and leave unconsumed escaped char (x was peek()ed) for next main loop pass -- any hex digits were also peek()ed and not consumed 
                }
                else
                {
                    M__(M_out(L"escaped_string() -- adding valid hex value to string");)

                    // valid hex escape sequence   --   \xNNNN
                    
                    addto_orig_token(token, ch    );                    // add the escape  char (\) char to orig token string only
                    addto_orig_token(token, ch1   );                    // add the raw escaped char (x) char to orig token string only
                    addto_orig_token(token, chx[0]);                    // add the 1st NNNN char char to orig token string only
                    addto_orig_token(token, chx[1]);                    // add the 2nd NNNN char char to orig token string only
                    addto_orig_token(token, chx[2]);                    // add the 3rd NNNN char char to orig token string only
                    addto_orig_token(token, chx[3]);                    // add the 4th NNNN char char to orig token string only

                    addto_token_subst(token, ch1, std::wstring {(wchar_t)hex_value});  // add hex value (as wchar_t) to token only (not orig token) 

                    discard_char(5);                                    // consume the x and 4 NNNN characters (which were just peek()ed ), and continue gathering up the string
                }
            }


            // if unknown escaped sequence (not \a, \b, \n, \x, etc.), assume this is not an escape sequence, just normal \ in string -- don't complain 
            // ----------------------------------------------------------------------------------------------------------------------------------------

            else                                                        // unknown escape sequence -- just put escape character in string and continue
            {                                                          
                addto_token(token, ch);                                 // just add escape char (\) to to string contents and leave consumed escaped (2nd) char for next main loop pass           
            }                                                          
        } 


        // handle EOF, EOL, error, etc.
        // ----------------------------

        else if (ch.subtype == char_E::eof)                             // end-of-file found in active string (error) 
        {
            invalid_token(token, L"String was still open when End-of-File was reached");           
            past_end_token(token, ch);                                  // EOF is past end of token, so put it back
            break;                                                      // end the for(;;) loop
        }
        else if (ch.subtype == char_E::eol)                             // end of line found in active string (error, unless multi-line string)
        {
            if (include_nl)                                             // multi-line string? 
            {
                addto_orig_token(token, ch );                           // add the EOL char to orig token string only
                addto_token_subst(token, ch, std::wstring {L"\n"});     // add the substitute NL char(s) to token only (not orig_token)
            }
            else                                                        // not multi-line string -- error if NL is seen in string
            {
                invalid_token(token, L"String was still open when End-of-Line was reached");
                past_end_token(token, ch);                              // EOL is past end of token, so put it back
                break;                                                  // end the for(;;) loop
            }                                                
        }
        else if (ch.subtype == char_E::error)
        {
            rc = past_end_token(token, ch);                             // put back error indication for next time -- comment token type replaced with "error"
            break;                                                      // end the for(;;) loop 
        }                                                            
        else                                                            // not error, EOF, EOL, or ending delimiter -- just keep adding to string contents 
        {                                                            
            addto_token(token, ch);                                     // add char to to string contents
        }                                                            
    }                                                                   // end of main loop

    return rc; 
}
M_endf





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  numeric_literal() -- gather up numeric literal token   
//  -----------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::numeric_literal(token_C& token) try     
{
    // peek() the 1st char, to check for +, -,0X,  etc. -- which are only valid at start of token 

    in_char_S ch  {}; 
    in_char_S ch1 {}; 
    in_char_S ch2 {}; 
    peek_char(ch);

    
    //  Check for (possible) based numeric literal, and call specialized routine  -- will handle special case, like "0X)", etc.
    //  ------------------------------------------------------------------------

    if (ch.ch32 == const_N::ch_base_prefix)           // might be start of 0X, 0x, etc. 
    {
        peek_char(ch1,2);                             // get possible X, x, B, b, O, o, etc.
       
        if (  (ch1.ch32 == const_N::ch_base2_lower)
           || (ch1.ch32 == const_N::ch_base2_upper)
           || (ch1.ch32 == const_N::ch_base8_lower)
           || (ch1.ch32 == const_N::ch_base8_upper)
           || (ch1.ch32 == const_N::ch_base16_lower)
           || (ch1.ch32 == const_N::ch_base16_upper)
           )
        {            
            return based_numeric_literal(token);       // go process based literal -- will handle invalid based literal, like 0xray             
        }
    }
    

    //  Numeric literal (base 10 only -- no exponents (yet)) -- remove any separators -- allow up to one dot (no alpha characters allowed)
    //  -----------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    //     - 1st character in char stream should be 1st digit or leading . or -
    //
    //   note: number cannot start with _ (becomes identifier) 
    
    int  rc           { 0     }; 
    int  int_length   { 64    };            // length of signed or unsigned integer
    int  float_length { 64    };            // length of floating point number
    bool first_char   { true  };
    bool dot_seen     { false };
    bool digit_seen   { false };
    bool need_trail_0 { false }; 
    bool alpha_seen   { false };
    bool sign_seen    { false };
    bool always_seen  { false };
    bool u_seen       { false };
    bool s_seen       { false };
    bool e_seen       { false };
    bool f_seen       { false };

    start_token(token, token_E::number);

    for(;;)                            // loop to gather digits, etc.
    {
        get_char(ch);      // 1st one known to be a digit or . or -
           

        //   Process decimal digit
        //   ---------------------

        if (ch.subtype == char_E::digit)
        {
             addto_token(token, ch); 
             digit_seen   = true;
             need_trail_0 = false;     // no longer need any trailing 0 after full_stop  
        }


        //   Process "."  -- should be decimal point, if we haven't seen one yet  (note .. causes immediate end of numerlc literal)
        //   -------------------------------------------------------------------

        else if ( (ch.subtype == char_E::full_stop) && (!dot_seen) && (!alpha_seen))   // check for first dot (2nd one ends number), also only if no alpha so far (numeric_identifier) ended by dot  
        {
            // check for .. (dot operator)

            in_char_S ch3 { }; 
            peek_char(ch3);                               // look at char folloring the dot

            if (ch3.subtype == char_E::full_stop)         // 2nd dot ?
            {
                rc = past_end_token(token, ch);           // 1st dot is past the end of the numeric literal (which will contain no decimal point)  
                break;                 
            }    
   

            // process single . in numeric literal

            if (!digit_seen)
            {
                token.str += L"0";          // add leading zero, if no digits seen yet
                digit_seen = true;          // the added "0" counts as a seen digit
            }

            addto_token(token, ch);         // add in the dot    
            dot_seen     = true;
            need_trail_0 = true;            // need to add a trailing 0, unless another digit seen before the end 
        }


        //   Process "+" or "-" or always_sign char -- may be sign (if at start of literal) -- for + or -,  get here only if +- attachment is allowed 
        //   ----------------------------------------------------------------------------------------------------------------------------------------

        else if ( 
                 (
                  (ch.subtype == char_E::f_always_sign)
                  ||
                  (ch.subtype == char_E::hyphen_minus)
                  ||
                  (ch.subtype == char_E::plus)
                 )
                 &&
                 first_char                     // allow plus/minus only for 1st char -- otherwise have gone past end of valid numeric literal 
                )                              
        {
            sign_seen = true; 

            if (ch.subtype == char_E::hyphen_minus)
            {
                addto_token(token, ch);         // add the minus sign to text for later conversion
            }
            else if (ch.subtype == char_E::f_always_sign)
            {
                always_seen = true;             // indicate that always_sign char was seen, not minus 
                addto_orig_token(token, ch);    // add the always_minus char to token.orig_str only -- token.str should still be empty at this point
                token.str = L"-" + token.str;   // regular minus char should now be at front of token.str, rather than the macro at start of token.orig_str
            }
            else
            {
                addto_orig_token(token, ch);    // add plus sign only to original string for error messages, etc.
            }
        }


        //   Process "_" -- separator character
        //   ----------------------------------

        else if (ch.type == char_E::sep)        // skip over any separators -- don't put them into number
        {          
            addto_orig_token(token, ch);        // don't put the separator into the number -- just into original token string     
        }


        //   ---------------------------------------------------------------------------------------------------
        //   Process "U" or "u" or "I" or "i"  -- maybe signed/unsigned flag followed by integer length, in bits
        //   ---------------------------------------------------------------------------------------------------

        else if (                 
                  (
                     // check for U or u in plain integer -- no full_stop, no sign yet, no aplha yet   (no need to check for e_seen or s_seen -- since those end the loop)

                     ( (ch.ch32 == const_N::ch_unsigned_lower) || (ch.ch32 == const_N::ch_unsigned_upper) )
                     &&
                     (!dot_seen)
                     &&
                     (!sign_seen)
                     &&
                     (!alpha_seen)
                  )
                  ||
                  (
                      // check for S or s in plain integer -- no full_stop, no aplha yet   (no need to check for e_seen or u_seen -- since those end the loop)

                      ( (ch.ch32 == const_N::ch_signed_lower) || (ch.ch32 == const_N::ch_signed_upper) )
                      &&
                      (!dot_seen)
                      &&
                      (!alpha_seen)
                  )
                )
        {    
            // valid I/i or U/u -- posibly followed by valid length specification -- 0  1  8  16  32  or   64

            addto_orig_token(token, ch);                                                                        // don't put the U/u or I/i into the number -- just into original token string
            
            if ( (ch.ch32 == const_N::ch_unsigned_lower) || (ch.ch32 == const_N::ch_unsigned_upper) )           // U or u ?
                u_seen = true; 
            else                                                                                                // must have been I or i
                s_seen = true; 
        
          
            // check for optional length specifier following the I/i or U/u -- must be 0 1 8 16 32 or 64 only  -- anything else if not a length specifier

            peek_char(ch1);                                                                                     // look at character after I/i or U/u -- must be 8, 1, 3, or 6, if it's a length specifier
                                                                                                               
            if (ch1.ch32 == utf32_N::DIGIT_EIGHT)                                                               // U8/I8 -- byte-length integer -- 8 marks the end of integer literal 
            {                                                                                                  
                int_length = 8;                                                                                 // indicate byte-length integer literal 
                get_char(ch1);                                                                                  // consume the 8 character before leaving
                addto_orig_token(token, ch1);                                                                   // add the 8 to original token only                          
                break;                                                                                          // the 8 digit ends the integer literal token
            }                                                                                                  
            if (ch1.ch32 == utf32_N::DIGIT_ZERO)                                                                // U0 -- byte-length integer -- 0 marks the end of unit literal 
            {                                                                                                  
                if (u_seen)                                                                                     // only U0 is valid
                {                                                                                              
                    int_length = 0;                                                                             // indicate 0-length unit literal 
                    get_char(ch1);                                                                              // consume the 0 character before leaving
                    addto_orig_token(token, ch1);                                                               // add the 0 to original token only                          
                    break;                                                                                      // the 0 digit ends the integer literal token
                }                                                                                              
                else                                                                                            // I0 is not valid
                {                                                                                              
                    break;                                                                                      // I ends the integer literal 
                }                                                                                              
            }
            else if ( (ch1.ch32 == utf32_N::DIGIT_ONE) || (ch1.ch32 == utf32_N::DIGIT_THREE) || (ch1.ch32 == utf32_N::DIGIT_SIX) )
            {
                peek_char(ch2, 2);                                                                              // look at character after 1, 3, or 6
            
                if (
                     ( (ch1.ch32 == utf32_N::DIGIT_ONE  ) && (ch2.ch32 == utf32_N::DIGIT_SIX)  )                //   I16 or U16 ? 
                     ||                                                                                       
                     ( (ch1.ch32 == utf32_N::DIGIT_THREE) && (ch2.ch32 == utf32_N::DIGIT_TWO)  )                //   I32 or U32 ?
                     ||                                                                                       
                     ( (ch1.ch32 == utf32_N::DIGIT_SIX  ) && (ch2.ch32 == utf32_N::DIGIT_FOUR) )                //   I64 or U64 ?
                    )
                {
                    get_char(ch1);                                                                              // consume the 1, 3, or 6  
                    get_char(ch2);                                                                              // consume the 6, 2, or 4
                    addto_orig_token(token, ch1);                                                               // add the 1st digit to the original token only
                    addto_orig_token(token, ch2);                                                               // add the 2nd digit to the original token only

                    if      (ch1.ch32 == utf32_N::DIGIT_ONE  )  int_length = 16;                                // 1x must be 16 -- set length to 16 bits 
                    else if (ch1.ch32 == utf32_N::DIGIT_THREE)  int_length = 32;                                // 3x must be 32 -- set length to 32 bits
                    else                                        int_length = 64;                                //    must be 64 -- set length to 64 bits  

                    break;                                                                                      // 2nd digit ends the integer literal
                }                                                          
                else                                                                                            // 1st and 2nd chars after I/i or U/u are not 16, 32, or 64, but 1st char after U/u could still be 1 (boolean)  
                {                                                                                              
                    if (u_seen)                                                                                 // only u1/U1 allowed for booleans -- not s1/S1
                    {                                                                                         
                         if (ch1.ch32 == utf32_N::DIGIT_ONE)                                                    // U1 or u1 -- is a boolean (2nd digit is not 6, os we don't have u16 or U16) following digits are ignored 
                         {                                                                                    
                             int_length = 1;                                                                    // indicate 1-bit length boolean literal 
                             get_char(ch1);                                                                     // consume the 1 character before leaving
                             addto_orig_token(token, ch1);                                                      // add the 1 to original token only                          
                             break;                                                                             // the 1 digit ends the integer literal token                        
                         }                                                                                    
                         else                                                                                   // 3 or 6, but not 32 or 64 -- not a boolean 
                         {                                                                                    
                             break;                                                                             // U/u or I/i end the integer literal -- not a boolean 
                         }                                                                                    
                    }                                                                                         
                    else                                                                                        // I/i -- not a boolean 
                    {                                                                                                               
                        break;                                                                                  // I/i ends the integer literal -- not valid boolean literal 
                    }                                                      
                }                                                          
            }                                                              
            else                                                                                                // 1st char not 0, 8, 1, 3, or 6 -- can't be start of length specifier
            {                                                                                                 
                break;                                                                                          // I/i or U/u ends the integer literal  
            }                                                              
                                                                           
            // shouldn't get here                                          
        }   // process I/i or U/u


        //   ---------------------------------------------------------------------------------------------
        //   Process "F" or "f" -- maybe floating point flag followed by floating point precision, in bits
        //   ---------------------------------------------------------------------------------------------

        else if (                 
                 ( (ch.ch32 == const_N::ch_float_lower) || (ch.ch32 == const_N::ch_float_upper) )
                   &&
                   (!alpha_seen)                                                                                // make sure no alpha chars seen (extended identifier)
                 )
        {    
            // valid F or f -- posibly followed by valid length specification -- 32  or   64

            addto_orig_token(token, ch);                                                                        // Don't put the F or f into the number -- just into original token string
            f_seen = true;                                                                                      // indicate that F or f has been seen
        
            
            // put in 0 or .0, if not seen yet

            if (!dot_seen)                                                                                      // number is something like 123E+77 -- need to add .0 at end
            {                                                                                                 
                token.str += L".0";                                                                             // add to output string, but not to orig_str 
                dot_seen = true;                                                                                // no need for .0 anymore 
            }                                                                                                 
            else if (need_trail_0)                                                                              // have something like 1234.E-11
            {                                                                                                 
                token.str += L"0";                                                                              // add trailing zero, if no digits seen after the dot 
                need_trail_0 = false;                                                                           // don't want this added later (to end of exponent)
            }
             
          
            // check for optional length specifier following the F or f -- must be 32 or 64 only  -- anything else if not a length specifier

            peek_char(ch1);                                                                                     // look at character after F or f -- must be 3 or 6, if it's a length specifier
 
            if ( (ch1.ch32 == utf32_N::DIGIT_ONE) || (ch1.ch32 == utf32_N::DIGIT_THREE) || (ch1.ch32 == utf32_N::DIGIT_SIX) )
            {
                peek_char(ch2, 2);                                                                              // look at character after 3, or 6
                                                                                                              
                if (                                                                                          
                     ( (ch1.ch32 == utf32_N::DIGIT_THREE) && (ch2.ch32 == utf32_N::DIGIT_TWO)  )                //   f32 or F32 ?
                     ||                                                                                       
                     ( (ch1.ch32 == utf32_N::DIGIT_SIX  ) && (ch2.ch32 == utf32_N::DIGIT_FOUR) )                //   f64 or F64 ?
                    )                                                                                         
                {                                                                                             
                    get_char(ch1);                                                                              // consume the 3, or 6  
                    get_char(ch2);                                                                              // consume the 2, or 4
                    addto_orig_token(token, ch1);                                                               // add the 1st digit to the original token only
                    addto_orig_token(token, ch2);                                                               // add the 2nd digit to the original token only
                                                                                                               
                    if (ch1.ch32 == utf32_N::DIGIT_THREE)  float_length = 32;                                   // 3x must be 32 -- set length to 32 bits
                    else                                   float_length = 64;                                   //    must be 64 -- set length to 64 bits  
                                                                                                               
                    break;                                                                                      // 2nd digit ends the integer literal
                } 
                else
                {
                    break;                                                                                      // valid number ends with f/F                                                                  
                }
                                                                                                               
            }                                                                                                  
            else                                                                                                 // 1st char not 3, or 6 -- can't be start of length specifier
            {                                                                                                  
                break;                                                                                           // S/s or U/u ends the integer literal  
            }                                                                                                 
                                                                                                              
            // shouldn't get here                                          
        }        


        //   -------------------------------------------------------------------------------------------------------
        //   Process "E" or "e"  -- maybe start of exponent, or start of next token, or part of a numeric identifier  (no need to check for u_seen or s_seen or f_seen -- since those end the number) 
        //   -------------------------------------------------------------------------------------------------------

        else if (                                                                             // check for E or e -- special meaning only valid if no alpha seen yet
                 ( (ch.ch32 == const_N::ch_exponent_lower) || (ch.ch32 == const_N::ch_exponent_upper) )
                 &&
                 (!alpha_seen)
                )
        {   
            bool exponent_sign_seen {false};


            // only valid if e12345.. or E+12345... e-12345... etc.   (note macro is not allowed here)

            peek_char(ch1);                                                                   // peek at character following the "e" 
            
            if ( (ch1.subtype == char_E::hyphen_minus) || (ch1.subtype == char_E::plus) )     // E+ or E- 
            {
                exponent_sign_seen = true;  
                peek_char(ch2, 2);                                                            // peek at char following the + or - 
                                                                                             
                if (ch2.subtype != char_E::digit)                                             // e+xxx xxx not digit -- "e" or "E" starts next token, or is part of numeric identifier     
                {                                                                            
                    if (m_allow_num_identifier && (!dot_seen) && (!sign_seen) )               // allowing numeric identifiers (like 11J), and no full_stop or sign yet seen?
                    {                                                                        
                        addto_token(token, ch);                                              
                        alpha_seen   = true;                                                  // indicate this is not a pure number
                        need_trail_0 = false;                                                 // don't add trailing 0 following alpha characters 
                        continue;                                                             // repeat loop -- don't fall through into exponent-handling code below
                    }                                                                        
                    else                                                                      // numeric identifiers not allowed or dot has been seen -- this e/E character ends the literal
                    {                                                                        
                        rc = past_end_token(token, ch);                                       // this e/E character ends the numeric literal  
                        break;                                                               
                    }                                                                        
                }                                                                            
            }                                                                                
            else if (ch1.subtype != char_E::digit)                                            // E not followed by digit, +, or -    --    this E or e ends the token, or is part of a numeric identifier -- r/c should be 0 here
            {                                                                                
                if (m_allow_num_identifier && (!dot_seen) && (!sign_seen) )                   // allowing numeric identifiers (like 11J), and no full_stop or sign yet seen?
                {                                                                            
                    addto_token(token, ch);                                                  
                    alpha_seen   = true;                                                      // indicate this is not a pure number
                    need_trail_0 = false;                                                     // don't add trailing 0 following alpha characters 
                    continue;                                                                 // repeat loop -- don't fall through into exponent-handling code below
                }                                                                            
                else                                                                          // numeric identifiers not allowed or dot has been seen -- this e/E character ends the literal
                {                                                                            
                    rc = past_end_token(token, ch);                                           // this e/E character ends the numeric literal  
                    break;                                                                   
                }
            }


            //  (If control gets here, must have E+n E-n En, so this is a valid exponent for a floating point number) 

            e_seen = true; 


            // put in 0 or .0, if not seen yet

            if (!dot_seen)                                                                     // number is something like 123E+77 -- need to add .0 at end
            {                                                                              
                token.str += L".0";                                                            // add to output string, but not to orig_str 
                dot_seen = true;                                                               // ".0" no longer needed
            }                                                                              
            else if (need_trail_0)                                                             // have something like 1234.E-11
            {                                                                              
                token.str += L"0";                                                             // add trailing zero, if no digits seen after the dot 
                need_trail_0 = false;                                                          // don't want this added later (to end of exponent)
            }                                                                              
                                                                                           
            addto_token(token, ch);                                                            // add the "e" pr "E" to literal token 
                                                                                           
            if (exponent_sign_seen)                                                        
            {                                                                              
                 addto_token(token, ch1);                                                      // add the + or - following the E to token 
                 discard_char();                                                               // get rid of peek()ed + - so char stream is sitting on 1st char in exponent number 
            }                                                                              


            //  At this point, should have one or more exponent digits in the char_stream -- next non-digit ends the token (no separators allowed in exponent) 

            for (;;)
            {
                get_char(ch);                                                                  // get next char -- may be digit of start of next token  
                                                                                              
                if (ch.subtype == char_E::digit)                                              
                {                                                                             
                    addto_token(token, ch);                                                    // add the digit to the token 
                }                                                                             
                else if (ch.type == char_E::sep)                                               // skip over any separators -- don't put them into number
                {                                                                             
                    addto_orig_token(token, ch);                                               // don't put the separator into the number -- just into original token string     
                }                                                                             
                else                                                                          
                {                                                                             
                    rc = past_end_token(token, ch);                                            // non-digit character ends the token
                    break; 
                }            
            }              
                                                                                               // continue loop to look for look for optional f32, f64                                     
        }                     


        //   Process alphabetic characters
        //   -----------------------------

        else if (
                 (ch.type == char_E::alpha)                                                    // alpha characters  and non-digit number chars don't end the number, but make it invalid -- keep count of them
                 ||                                                                          
                 (ch.type == char_E::num  )                                                    // numbers -- digits have already been filtered out, so these are misc numeric chars only
                )
        {
            if (m_allow_num_identifier && (!dot_seen) && (!sign_seen) )                        // allowing numeric identifiers (like 11J), and no full_stop or sign yet seen?
            {
                addto_token(token, ch);
                alpha_seen   = true;                                                           // indicate this is not a pure number
                need_trail_0 = false;                                                          // don't add trailing 0 following bad alpha characters            
            }                                                                                 
            else                                                                               // numeric identifiers not allowed or dot has been seen -- this charagter ends the literal
            {                                                                                 
                rc = past_end_token(token, ch);                                                //this alpha character ends the numeric literal  
                break;                 
            }
        }


        //   Process any other characters -- not part of literal -- starts next token
        //   ------------------------------------------------------------------------

        else  // reached a character that is not part of number -- put it back for next time (don't set error_seen yet) -- includes minus after 1st char or 2nd dot
        {     
            rc = past_end_token(token, ch);
            break;
        }  

        first_char = false;
    }                             // end of digit-gathering loop

    
    // make sure character following the end of the number is not a separator, digit, or letter (invalid)
    // ----------------------------------------------------------------------------------------

    peek_char(ch1);
    M__(M_out(L"numeric_literal() -- checking for stop character -- ch1.type=%d") % (int)(ch1.type);)

    if (
        (ch1.type == char_E::alpha)                                                            // don't want letters    immediately following the numeric literal
        ||                                                                                  
        (ch1.type == char_E::num  )                                                            // don't want digits     immediately following the numeric literal
        ||                                                                                  
        (ch1.type == char_E::sep  )                                                            // don't want separators immediately following the numeric literal
       )
    {
        addto_token(token, ch1);
        invalid_token(token, L"Invalid alphabetic characters, numeric digits, or separator characters occured immediately after a numeric literal");
        rc = -1; 
        return -1;
    }
    

    // Set integer/floating_pt value, based on presence of dot (if no error occurred) -- make sure no alphabetic characters seen in number -- compute value
    // ----------------------------------------------------------------------------------------------------------------------------------------------------

    if (token.type == token_E::number)                                                         // no I/O error ended the token ?
    {                                                                                          
        if (alpha_seen && !dot_seen)                                                           // must be identifier that starts with a digit (if "allow numeric identifier" flag is set)
        {                                                                                      
            token.type = token_E::numeric_identifier;                                             
        }                                                                                      
        else if (alpha_seen)                                                                   // any alpha characters and dots make number invalid -- should not get here due to earlier checks 
        {
            invalid_token(token, L"Alphabetic characters appear in a numeric literal");
            rc = -1; 
        }
        else if (dot_seen || e_seen || f_seen)                                                 // "."or "e+000" or _f32/_F64 means floating pt (if e+xxx seen ot F/f seen, .0 has already been appended, if required) 
        {                                                                                     
            if (need_trail_0)                                                                 
                token.str += L"0";                                                             // add trailing zero, if no digits seen after the dot

            if (float_length == 64)
            {
                rc = to_float64(token.str, token.float64);     
                if (rc == 0)                
                    token.type = token_E::float64;                
                else
                    invalid_token(token, L"Cannot obtain value for 64-bit floating point literal"); 
            }
            else
            {                                                                                   // must be 32-bit
                rc = to_float32(token.str, token.float32);     
                if (rc == 0)                
                    token.type = token_E::float32;                
                else
                    invalid_token(token, L"Cannot obtain value for 32-bit floating point literal"); 
            }
        }
        else                                                                                   // no dot and no alpha -- must be signed or unsigned integer
        {
            // process signed/usigned number  0-bit (unit), 1-bit (boolean), 8-bit, 16-bit, 32-bit, or 64-bit integer literal

            rc = get_integer_value(token, int_length, u_seen, 10);                    
        }
    }

    return rc; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  based_numeric_literal() -- gather up numeric literal token   
//  -----------------------
//
//    note: when called, 1st 2 chars are known to be: 0X 0x 0B 0b 0O or 0o
//    note: based integers are not recognized as boolean or unit values
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::based_numeric_literal(token_C& token) try     
{
    // decode basing character - 0X 0B, or 0O            
    // --------------------------------------

    int       base  {10};                                                                     // in case unknown basing letter passed in ???? -- should not happen
    in_char_S ch    {  };                                                                    
    in_char_S ch1   {  };                                                                     // for peeking in advance
    in_char_S ch2   {  };                                                                     // for peeking in advance
    in_char_S ch3   {  };                                                                     // for peeking in advance
                                                                                             
    peek_char(ch2, 2);                                                                        // peek at basing character 

    if      ( (ch2.ch32 == const_N::ch_base16_lower)        || (ch2.ch32 == const_N::ch_base16_upper)      ) base = 16; 
    else if ( (ch2.ch32 == const_N::ch_base8_lower )        || (ch2.ch32 == const_N::ch_base8_upper )      ) base = 8; 
    else if ( (ch2.ch32 == const_N::ch_base2_lower )        || (ch2.ch32 == const_N::ch_base2_upper )      ) base = 2;      


    // peek() the 3rd char, and check for ,0X, 0x,0b, 0o, etc.followed by valid based digit -- otherwise have: "0" (integer) "X/B/O/etc." then bad digit
    // ------------------------------------------------------------------------------------
   
    peek_char(ch3, 3);                                                                        // get 3rd character -- needs to be valid based digit (not _)
                                                                                             
    if (!is_digit(ch3.ch32, base))                                                            // char following the X/O/B is not a valid hex/decimal/binary digit? 
    {                                                                                        
        peek_char(ch1);                                                                       // get "0"for use in invalid token for error message
        addto_token(token, ch1);                                                              // put "0"       in token
        addto_token(token, ch2);                                                              // put "x"(etc.) in token
        addto_token(token, ch3);                                                              // put invalid non-digit 3rd character in token
        invalid_token(token, L"Cannot obtain value for floating point literal");              // start the "0" token -- consume the "0", leave "X"and following
        return -1;     
    }
   

    //  Add "0x/0b/0o" into orig string, but not token, consuming the 0X 0x 0b 0o, etc. -- leave the 3rd peek()ed char on char_stack
    
    start_token(token, token_E::number);
    get_char(ch); 
    addto_orig_token(token, ch);
    get_char(ch); 
    addto_orig_token(token, ch);


    //  Numeric literal (base 2,8,16 only -- no decimal point, sign,  or exponent allowed (separators are OK) -- U or u allowed at end to indicate unsigned
    //  ----------------------------------------------------------------------------------------------------------------------------------------------------
    //
    //   note: number can't start with _ , even after the 0x.  This prevents things like 0x________u64, where x_________u64 is a valid identifier 
    
    int  rc           { 0     }; 
    int  int_length   { 64    };            // length of signed or unsigned integer
    bool s_seen       { false };
    bool u_seen       { false };

    for(;;)                            // loop to gather digits, etc.
    {
        get_char(ch);      // 1st one known to be a digit or . or -
        
        // handle incoming (based) digits

        if (is_digit(ch.ch32, base))
        {
             addto_token(token, ch); 
        }        


        // handle separator characters

        else if (ch.type == char_E::sep)    // skip over any separators -- don't put them into number
        {
            // don't put the separator into the number -- just into original token string
            addto_orig_token(token, ch);           
        }

        // handle sNNN uNNN SNNN UNNN -- integer type and length

        else if (
                  (ch.ch32 == const_N::ch_signed_lower  ) || (ch.ch32 == const_N::ch_signed_upper  )
                  ||
                  (ch.ch32 == const_N::ch_unsigned_lower) || (ch.ch32 == const_N::ch_unsigned_upper)
                ) 
        {
             
            // valid S/s or U/u -- posibly followed by valid length specification -- 8  16  32  or   64

            addto_orig_token(token, ch);                   // don't put the U/u or S/s into the number -- just into original token string
            
            if ( (ch.ch32 == const_N::ch_unsigned_lower) || (ch.ch32 == const_N::ch_unsigned_upper) )   // U or u ?
                u_seen = true; 
            else                                                                                                // must have been S or s
                s_seen = true; 
        
          
            // check for optional length specifier following the S/s or U/u -- must be 8 16 32 or 64 only  -- anything else if not a length specifier


            peek_char(ch1);                                // look at character after S/s or U/u -- must be 8, 1, 3, or 6, if it's a length specifier

            if (ch1.ch32 == utf32_N::DIGIT_EIGHT)          // U8/S8 -- byte-length integer -- 8 marks the end of integer literal 
            {
                int_length = 8;                            // indicate byte-length integer literal 
                get_char(ch1);                             // consume the 8 character before leaving
                addto_orig_token(token, ch1);              // add the 8 to original token only                          
                break;                                     // the 8 digit ends to integer literal token
            }
            else if ( (ch1.ch32 == utf32_N::DIGIT_ONE) || (ch1.ch32 == utf32_N::DIGIT_THREE) || (ch1.ch32 == utf32_N::DIGIT_SIX) )
            {
                peek_char(ch2, 2);                         // look at character after 1, 3, or 6
            
                if (
                     ( (ch1.ch32 == utf32_N::DIGIT_ONE  ) && (ch2.ch32 == utf32_N::DIGIT_SIX)  )      //   S16 or U16 ? 
                     ||
                     ( (ch1.ch32 == utf32_N::DIGIT_THREE) && (ch2.ch32 == utf32_N::DIGIT_TWO)  )      //   S32 or U32 ?
                     ||
                     ( (ch1.ch32 == utf32_N::DIGIT_SIX  ) && (ch2.ch32 == utf32_N::DIGIT_FOUR) )      //   S64 or U64 ?
                    )
                {
                    get_char(ch1);                                                   // consume the 1, 3, or 6  
                    get_char(ch2);                                                   // consume the 6, 2, or 4
                    addto_orig_token(token, ch1);                                    // add the 1st digit to the original token only
                    addto_orig_token(token, ch2);                                    // add the 2nd digit to the original token only

                    if      (ch1.ch32 == utf32_N::DIGIT_ONE  )  int_length = 16;     // 1x must be 16 -- set length to 16 bits 
                    else if (ch1.ch32 == utf32_N::DIGIT_THREE)  int_length = 32;     // 3x must be 32 -- set length to 32 bits
                    else                                        int_length = 64;     //    must be 64 -- set length to 64 bits  

                    break;                                                           // 2nd digit ends the integer literal
                }                                                                  
                else                                                                 // 1st and 2nd chars after S/s are not 16, 32, or 64 
                {                                                                  
                    break;                                                           // S/s or U/u ends the integer literal 
                }                                                                  
            }                                                                      
            else                                                                     // 1st char not 8 -or- 2nd char not 1, 3, or 6 -- can't be start of length specifier
            {                                                                      
                break;                                                               // S/s or U/u ends the integer literal  
            }                                                                      
                                                                                   
            // shouldn't get here                                                  
        }                                                                            // handle s/S/u/U + NN


        // anything else is not part of a numeric literal -- ends a valid numeric literal token

        else  // reached a character that is not part of number -- put it back for next time (don't set error_seen yet) -- includes minus char or dot
        {     
            rc = past_end_token(token, ch);
            break;
        }        
    }

   
    // make sure character following the end of the number is not a separator, digit, or letter
    // ----------------------------------------------------------------------------------------

    peek_char(ch1); 
    M__(M_out(L"based_numeric_literal() -- checking for stop character -- ch1.type=%d") % (int)(ch1.type);)
 
    if (
        (ch1.type == char_E::alpha)                                                   // don't want letters    immediately following the numeric literal
        ||                                                                           
        (ch1.type == char_E::num  )                                                   // don't want digits     immediately following the numeric literal
        ||                                                                           
        (ch1.type == char_E::sep  )                                                   // don't want separators immediately following the numeric literal
       )
    {
        addto_token(token, ch1);
        invalid_token(token, L"Invalid alphabetic characters, numeric digits, or separator characters occured immediately after a based numeric literal");
        rc = -1; 
        return -1;
    }


    // Set signed/unsigned integer, based on U u 
    // -----------------------------------------

    if (token.type == token_E::number)                                 // no I/O error ended the token ?
    {
        rc = get_integer_value(token, int_length, u_seen, base);         
    }

    return rc; 
}
M_endf









///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
////////|
////////|
////////|
////////|    =================
////////|    fetch_raw_token() -- build token from input characters from file (does not combine whitespace, etc.)  
////////|    =================
////////|    
////////|    
////////|       raw tokens recognized: 
////////|    
////////|        strings, comments,  whitespace, identifiers, verbs, sigils, parenthesis and other punctuation, numeric literals, etc.
////////|    
////////|
////////|
////////|
////////|
////////+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------     
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int token_stream_C::fetch_raw_token(token_C& token) try            
{
    token = token_C { };          // initially clean out token being passed back


    // note: Raw token putback stack is assumed to be empty at this time

    int rc = 0;
    

    // if prior error has been seen, just pass back an empty error token -- do not try any file I/O, etc. 

    if (m_error_seen)
    {
        start_token(token, token_E::error);    
        return -1;
    }

    

    // ================================================================
    // look for next token, based on next character in character stream 
    // ================================================================
    
    in_char_S ch{};    
    rc = peek_char(ch);                       // see what next char is at start of token 

    char_E ch_subtype      = ch.subtype;      // set default subtype for this character 
    char_E ch_orig_subtype = ch.subtype;      // set original subtype for this character


    //  update ch_subtype if this is one of the variable-purpose chars -or- a leading sigil

    if       (ch.ch32 == m_line_continuation_ch     )   ch_subtype = char_E::f_continuation         ;
    else if  (ch.ch32 == m_vanishing_separator_ch   )   ch_subtype = char_E::f_vanishing_separator  ;
    else if  (ch.ch32 == m_always_sign_ch           )   ch_subtype = char_E::f_always_sign          ;
    else if  (ch.ch32 == m_unechoed_line_comment_ch )   ch_subtype = char_E::f_line_comment         ;
    else if  (ch.ch32 == m_echoed_line_comment_ch   )   ch_subtype = char_E::f_echoed_line_comment  ;
    else if  (ch.ch32 == m_comment_1st_ch           )   ch_subtype = char_E::f_comment_1st          ;
    else if  (ch.ch32 == m_type1_string_start_ch    )   ch_subtype = char_E::f_type1_string_start   ;
    else if  (ch.ch32 == m_type2_string_start_ch    )   ch_subtype = char_E::f_type2_string_start   ;
  
    else if  ( is_in_vector(ch.ch32, m_leading_sigils) )
       ch_subtype = char_E::f_leading_sigil; 

    M__(M_out(L"token_stream_C::fetch_raw_token() -- ch_orig_subtype = %d -- ch_subtype = %d   f_leading_sigil = %d    ") % (int)ch_orig_subtype % (int)ch_subtype % (int)(char_E::f_leading_sigil);)

    // ---------------------------------------
    // decode ch_subtype at start of raw token
    // ---------------------------------------
    
    for(;;)       // loop repeats only if 2nd pass requested by some code in the loop
    {
        switch (ch_subtype)
        {
            // handle error character at start of new token
            // --------------------------------------------
       
            case char_E::error : 
                one_char_token(token, token_E::error, ch);
                break;
           
       
            // handle END character at start of new token (all files are closed now) or else this is END of a parsed string
            // ------------------------------------------------------------------------------------------------------------
       
            case char_E::end : 
                M__(M_out(L"token_stream_C::fetch_raw_token() -- char_E::end seen");)
                one_char_token(token, token_E::end, ch);
                M__(M_out(L"token_stream_C::fetch_raw_token() -- ch.source_id = %d   ch.lineno=%d   ch.linepos=%d") % ch.source_id % ch.lineno % ch.linepos; )
                break;
       
       
            // handle EOF character at start of new token
            // ------------------------------------------
       
            case char_E::eof : 
                one_char_token(token, m_eof_is_whitespace ? token_E::whitespace : token_E::eof, ch);
                close_current_instream();                          // revert to prior file, if any             
                break;
       
       
            // handle EOL character at start of new token
            // -------------------------------------------
       
            case char_E::eol : 
                one_char_token(token, m_eol_is_whitespace ? token_E::whitespace : token_E::eol, ch);
                break;
                                    
       
            // -----------------------
            // single-character tokens
            // -----------------------
            
            case char_E::open_paren1          :
                one_char_token(token, token_E::open_paren          , ch);
                break;
            case char_E::open_bracket1        :
                one_char_token(token, token_E::open_bracket        , ch);
                break;
            case char_E::close_bracket1       :
                one_char_token(token, token_E::close_bracket       , ch);
                break;
            case char_E::open_brace1          :
                one_char_token(token, token_E::open_brace          , ch);
                break;
            case char_E::close_brace1         :
                one_char_token(token, token_E::close_brace         , ch);
                break;
            case char_E::open_angle_bracket1  :
                one_char_token(token, token_E::open_angle_bracket  , ch);             // probably error
                break;                                                              
            case char_E::close_angle_bracket1 :                                     
                one_char_token(token, token_E::close_angle_bracket , ch);             // probably error
                break;                                                              
            case char_E::open_dot_bracket     :                                     
                one_char_token(token, token_E::open_dot_bracket    , ch);             // probably error
                break;                                                              
            case char_E::close_dot_bracket    :                                     
                one_char_token(token, token_E::close_dot_bracket   , ch);             // probably error
                break;                                                              
            case char_E::open_shell_bracket1   :                                    
                one_char_token(token, token_E::open_shell_bracket  , ch);             // probably error
                break;                                                              
            case char_E::close_shell_bracket1  :                                    
                one_char_token(token, token_E::close_shell_bracket , ch);             // probably error
                break;                                                              
            case char_E::open_quill_bracket    :                                    
                one_char_token(token, token_E::open_quill_bracket  , ch);             // probably error
                break;                                                              
            case char_E::close_quill_bracket   :                                    
                one_char_token(token, token_E::close_quill_bracket , ch);             // probably error
                break;

            case char_E::open_paren2           :
            case char_E::open_bracket2         :
            case char_E::open_brace2           :
            case char_E::open_angle_bracket2   :
            case char_E::open_shell_bracket2   :
            case char_E::open_curve_bracket1   :
            case char_E::open_curve_bracket2   :
            case char_E::open_lens_bracket1    :
            case char_E::open_lens_bracket2    :
            case char_E::open_corner_bracket1  :
            case char_E::open_corner_bracket2  :
            case char_E::open_other_bracket    :
                one_char_token(token, token_E::open_misc_bracket, ch);                // probably error
                break;

            case char_E::close_paren2          :
            case char_E::close_bracket2        :
            case char_E::close_brace2          :
            case char_E::close_angle_bracket2  :
            case char_E::close_shell_bracket2  :
            case char_E::close_curve_bracket1  :
            case char_E::close_curve_bracket2  :
            case char_E::close_lens_bracket1   :
            case char_E::close_lens_bracket2   :
            case char_E::close_corner_bracket1 :
            case char_E::close_corner_bracket2 :
            case char_E::close_other_bracket   :
                one_char_token(token, token_E::close_misc_bracket  , ch);             // probably error
                break;

            case char_E::semicolon             :
                one_char_token(token, token_E::semicolon           , ch);
                break;

            case char_E::misc                  :
                one_char_token(token, token_E::misc_char           , ch);             // probably error
                break;

            case char_E::special1              :
            case char_E::special2              :
                one_char_token(token, token_E::special_char        , ch);             // probably error
                break;

            case char_E::accent1               :
            case char_E::accent2               :
                one_char_token(token, token_E::accent_char         , ch);             // probably error
                break;

            case char_E::punct1                :
            case char_E::punct2                :
                one_char_token(token, token_E::punctuation_char    , ch);             // probably error
                break;

            case char_E::f_vanishing_separator :
                one_char_token(token, token_E::vanishing_separator , ch);
                break;
    


            // --------------------------------------------------------
            // close parenthesis token -- can have trailing paren sigil
            // --------------------------------------------------------

            case char_E::close_paren1         :

            {
                in_char_S ch1 {}; 
                peek_char(ch1, 2);                                            // look at char following the paren -- 2nd oldest one -- leave it in the char stream  
                M__(M_out(L"token_stream_C::fetch_raw_token() -- parenthesis token -- following char = %08X") % (int)(ch1.ch32)); 

                one_char_token(token, token_E::close_paren, ch);              // set up basic close parenthesis token 


                // see if a valid trailing sigil for close paren immediately follows

                if ( m_allow_paren_sigils && is_in_vector(ch1.ch32, m_paren_sigils) )
                {
                    M__(M_out(L"token_stream_C::fetch_raw_token() -- parenthesis token -- adding trailing sigil");)
                    add_trailing_sigil(token, ch1); 
                    discard_char();                                          // consume trailing sigil char taht was peek()ed earlier 
                }      

                break;
            }
                                                                                                                  
       
            //  --------------------------------------------------------------------------------------------------
            //  whitespace char - gather whitespace (line-continuation char can start whitespace, or be erroneous) 
            //  -----------------------------------
       
            case char_E::f_continuation        :    
            case char_E::whitespace            :
               
                whitespace_token(token);
                break;                                                                                 
       
       
            //  ------------------------------------------------------------
            //  line_comment char - start line comment (do not suppress EOL)  -- comment started by this single char may not be supported
            //  ------------------------------------------------------------
       
            case char_E::f_line_comment        :
               
                line_comment(token, false, false, 1);                         // Do not suppress EOL after comment, do not echo, introducer length = 1 
                break; 
          
       
            //  --------------------------------------------------------------------------
            //  echoed_line_comment char - start echoed line comment (do not suppress EOL) -- comment started by this single char may not be supported
            //  --------------------------------------------------------------------------
       
            case char_E::f_echoed_line_comment :
               
                line_comment(token, false, true, 1);                          // Do not suppress EOL after comment, do echo, introducer length = 1 
                break; 
                         
       
            //  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //  1st comment char -- can be start of line_comment ( // ),  block_comment ( /* ),  EOF comment ( /~ ), nestable comment ( /[ ),  etc., or start of operator
            //  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
       
            case char_E::f_comment_1st :
            {
                in_char_S ch1 {}; 
                peek_char(ch1, 2);                                            // look at char following the "/" -- 2nd oldest one -- leave it in the char stream    

                M__(M_out(L"fetch_raw_token() -- 1st comment char seen -- ch1.ch32 = %08X") % ch1.ch32;)
                
                if      (ch1.ch32 == m_unechoed_line_comment_2nd_ch  )          // "//" starts a line comment 
                    line_comment(token, false, false, 2);                       //      do not suppress EOL after comment -- do not echo, introducer length = 2 
                                                                              
                else if (ch1.ch32 == m_echoed_line_comment_2nd_ch    )          //      (support is not not activated)
                    line_comment(token, false, true, 2);                        //      do not suppress EOL after comment -- do echo,     introducer length = 2 
                                                                              
                else if (ch1.ch32 == m_suppress_eol_comment_2nd_ch   )          //      (support is not not activated) 
                    line_comment(token, true, false, 2);                        //      do     suppress EOL after comment -- do not echo, introducer length = 2 
                                                                     
                else if (ch1.ch32 == m_eof_comment_2nd_ch            )          // "/]" starts an EOF comment -- ends with EOF
                    eof_comment(token);                                                              
                                
                else if (ch1.ch32 == m_block_comment_2nd_ch          )          // "/*" starts a block comment -- ends with "*/" or with error or EOF (not valid)
                    block_comment(token);                                     
                                                                              
                else if (ch1.ch32 == m_nest_comment_2nd_ch           )          // "/[" starts a nestable block comment -- ends with "]/" or with error or EOF (not valid) 
                    nestable_comment(token);                                         
              
                else if (ch1.ch32 == m_retained_line_comment_2nd_ch  )          // "/#" starts a retained line comment -- ends with EOL (or EOF)
                    retained_line_comment(token);                         

                else if (ch1.ch32 == m_retained_block_comment_2nd_ch )          // "/{" starts a retained block comment -- ends with "}/" or with error or EOF (not valid) 
                    retained_block_comment(token);                                                             
              
                else if (ch1.ch32 == m_retained_eof_comment_2nd_ch   )          // "/}" starts a retained EOF comment -- ends EOF (not valid) 
                    retained_eof_comment(token); 

                else                                                            // not   //   /*   /[   /]  /#   /{   /}   /~     -- must be start of start an operator token 
                {
                    operator_token(token);                                      // this assumes the comment starting character is a valid operator character, like "/" 
                }
                break;          
            }
       
       
            //  --------------------------------------
            //  digit 0-9 -- should be numeric literal 
            //  --------------------------------------
            //
            // note: number cannot start with _ (_12 is an identifier, for example) 
       
            case char_E::digit :
            
                (void)numeric_literal(token);
                break;         
       
       
            //  ---------------------------------------------------------------------------------------------
            //  full_stop -- possible numeric literal starting with decimal point, or start of a dot operator 
            //  ---------------------------------------------------------------------------------------------
       
            case char_E::full_stop :
            {
                 in_char_S ch1 {}; 
                 peek_char(ch1, 2);                                            // look at char following the "." -- 2nd oldest one -- leave it in the char stream
                         
                 if (ch1.subtype == char_E::digit)                             // is "." followed by a digit?
                 {
                     numeric_literal(token);                                   // ".nnnn" = floating point literal starting with "."
                 }
                 else                                                          // char after 1st dot is not digit   
                 {
                     same_operator_token(token, ch);                           // gather up operator token starting with the "." 
                 }
                 break;
            }
       
       
            //  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //  always_minus, plus or hyphen_minus -- possible Numeric literal starting with sign -- maybe follwed by digit or one dot then digit (-1 , +.2 OK, -_3 ,  +._4 not allowed, for example) 
            //  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
       
            case char_E::plus                   : 
            case char_E::hyphen_minus           :
            case char_E::f_always_sign          :
            {
                 if  ( 
                      (ch.subtype == char_E::f_always_sign)                       // always_minus unconditionally attaches (used as minus sign)
                      ||                                                         
                      (                                                           // must be + or -  -- need to check to see if these get attached to numeric literals 
                       ( !m_never_attach_plus_minus                     )         // only if +- attachment to numeric literals is allowedis allowed
                       &&                                                        
                       ( (m_attach_mode) || (m_always_attach_plus_minus))         // can be signed number only if this is not an infix minus/plus, or + - are always considered signs when attached to a numeric literal
                      )                                                          
                     )   
                 {
                     in_char_S ch1 {}; 
                     peek_char(ch1, 2);                                           // look at character following the plus/minus sign or always_sign char                                                                                  
                                                                               
                     if (ch1.subtype == char_E::digit)                            // is it -1  +1, etc. ?
                     {                                                         
                         numeric_literal(token);                                  // go gather up negative (or positive) number -xxxx.xxxx, +xx.xx,  etc.
                     }                                                         
                     else if (ch1.subtype == char_E::full_stop)                   // is it -.1,  +.1, etc. ?
                     {                                                         
                         in_char_S ch2 {};                                     
                         peek_char(ch2, 3);                                       // look at character following the dot
                                                                               
                         if (ch2.subtype == char_E::digit)                        // is it -.1 , +.1, etc. ?
                             numeric_literal(token);                              // go gather up negative (or positive) number -.xxxx, +.xxxx, etc.
                         else                                                     // "-."  but not followed by number -- go gather up the "-" as an operator token  
                             operator_token(token);                            
                     }                                                         
                     else                                                         // "-"    but not followed by dot or digit -- assume + or - starts an operator string, or always_sign char is an accent char token (for ecample)  
                     {
                         if (ch.subtype == char_E::f_always_sign)                 // distinguish between always_minus and plus/minus
                         {
                             ch_subtype = ch_orig_subtype;                        // on 2nd loop pass, no special interpretation for this character -- use default handling
                             continue;                                            // go back and look at this char again
                         }
                         else                                                     // this plus or minus must be start of a new operator token
                         {
                             operator_token(token);                               // start accumulating operator token (like "+" "--", etc.)
                         }
                         break; 
                     }
                 }
                 else                                                             // infix/non-attach mode -- plus/minus must be start of new operator
                 {
                     operator_token(token); 
                 }
                 break;
            }
            
            
            //  -------------------------------------------
            //  single quote -- start of single-word string  (not configurable)
            //  -------------------------------------------
           
            case char_E::both_quote1 :                                            // hard-coded single quotes
                word_string(token);                                               // ending delimiter is first whitespace after the single quote
                break;
 
       
            //  ---------------------------------------------
            //  open double quotes -- start of escaped string  
            //  ---------------------------------------------
           
            case char_E::f_type1_string_start :
            case char_E::f_type2_string_start :
       
                if (ch.ch32 == m_type1_string_start_ch)
                {
                    start_token(token, token_E::string);                                               // needed since escaped_string() and simple_string() preserve orig_str field in passed-in token 
                    escaped_string(token, m_type1_string_end_ch, m_type1_string_escape_ch, false);     // ending delim = ",   1st escape_char, NL in string is error   
                    break; 
                }
                else if (ch.ch32 == m_type2_string_start_ch)
                {
                    start_token(token, token_E::string);                                               // needed since escaped_string() and simple_string() preserve orig_str field in passed-in token 
                    escaped_string(token, m_type2_string_end_ch, m_type2_string_escape_ch, false);     // ending delim = »,   2nd escape_char, NL in string is error   
                    break; 
                }

                M_out_emsg(L"Internal error -- Unexpected open (double) quote codepoint (%08X) at line=%u column=%u in file = \"%s\"") % (uint32_t)(ch.ch32) % ch.lineno % ch.linepos % m_in_filename; 
                count_error();
                break; 
       
       
            //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------
            //  general operator char -- gather longest string of operator characters (initial + - / for comments or numeric literals, etc. are already actioned by this point)  
            //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------
           
            case char_E::oper1        :
            case char_E::oper2        :
       
                operator_token(token, false);
                break;
       
       
            //  -------------------------------------------------------------------------------
            //  alphabetic char or underscore -- must be start of identifier or modified string
            //  -------------------------------------------------------------------------------
           
            case char_E::alpha1     :
            case char_E::alpha2     :
            case char_E::alpha3     :
            case char_E::alpha4     :
            case char_E::underscore :
               
                M__(M_out(L"token_stream_C::fetch_raw_token() -- ch.ch32 = %08X     m_raw_string_prefix_ch = %08X") % (uint32_t)(ch.ch32) %  (uint32_t)(m_raw_string_prefix_ch); )

                if  (
                     (ch.ch32 == m_raw_string_prefix_ch      )                                                                  //    R ?
                     ||                                                                                                 
                     (ch.ch32 == m_multiline_string_prefix_ch)                                                                  //    M ? 
                    )                                                                                                      
                {                                                                                                          
                    in_char_S ch1 {};                                                                                      
                    peek_char(ch1, 2);                                                                                          // look at char following the raw string prefix char, to check for quotes -- 2nd oldest one -- leave it in the char stream  
                
                    M__(M_out(L"token_stream_C::fetch_raw_token() -- modified string  -- ch1.subtype = %08X      char_E::open_quote2 = %08X") % (uint32_t)(ch1.subtype) % (uint32_t)(char_E::both_quote2); )

                    // see if next char is start of a string

                    if (
                        (ch1.ch32 == m_type1_string_start_ch)
                        ||
                        (ch1.ch32 == m_type2_string_start_ch)
                       )
                    {
                        // -------------------------------------------------
                        // must be a modified string -- simple or multi-line
                        // -------------------------------------------------    

                        start_token(token, token_E::string);                                                                    // needed before adding to orig_token 
                        in_char_S ch0 {};   
                        get_char(ch0);                                                                                          // consume modified string prefix char, which simple_string()/escaped_string() does not expect to see
                        addto_orig_token(token, ch0);                                                                           // add the modified string prefix char to original token text for error messages only   

                        M__(M_out(L"token_stream_C::fetch_raw_token() -- modified string  -- ch1.ch32 = %08X") % (uint32_t)(ch1.ch32); )

                        if (ch1.ch32 == m_type1_string_start_ch)
                        {
                            if (ch.ch32 == m_raw_string_prefix_ch)
                                simple_string( token, m_type1_string_end_ch                                   );            // ending delim = " 
                            else
                                escaped_string(token, m_type1_string_end_ch , m_type1_string_escape_ch, true  );            // ending delim = " ,   1st escape_char, include any NLs in string  
                            break;  
                        }
                        else if (ch1.ch32 == m_type2_string_start_ch)
                        {
                            if (ch.ch32 == m_raw_string_prefix_ch)
                                simple_string( token, m_type2_string_end_ch                                   );            // ending delim = » 
                            else
                                escaped_string(token, m_type2_string_end_ch , m_type2_string_escape_ch, true  );            // ending delim = » ,   2nd escape_char, include any NLs in string   
                            break;  
                        }

                        M_out_emsg(L"Internal error -- Unexpected open (double) quote codepoint (%08X) following raw string prefix char at line=%u column=%u in file = \"%s\"") % (uint32_t)(ch1.ch32) % ch1.lineno % ch1.linepos % m_in_filename; 
                        count_error();
                        break; 
                    }
                }


                // not a raw string prefix -- this char must start an identifier 
   
                identifier_token(token, false);
                break;  

    


            //  -----------------------------------------------------------------------------------------------
            //  leading sigil char -- could be isolated, or attached at front of identifier, operator, or paren
            //  -----------------------------------------------------------------------------------------------
            //
            //  note: @M"string" will be treated as @M and "string", rather than @ and M"string"      

            case char_E::f_leading_sigil : 
            {
                 M__(M_out(L"token_stream_C::fetch_raw_token() -- possible leading sigil found");)

                in_char_S ch1 {};    
                peek_char(ch1, 2);                                              // look at char following the possible sigil -- 2nd oldest one -- leave it in the char stream  


                // check for leading sigils for operator  (allow only operator leading sigil subset -- must not include  ":"  "@"  etc.)

                if ( m_allow_leading_op_sigils && is_in_vector(ch.ch32, m_leading_oper_sigils) )
                {
                    M__(M_out(L"token_stream_C::fetch_raw_token() -- leading operator sigil found");)

                    // handle sigil followed by comment      

                    if (                                                         // check for single-char comment starter
                        (ch1.ch32 == m_echoed_line_comment_ch)
                        ||
                        (ch1.ch32 == m_unechoed_line_comment_ch)
                        )
                    {
                            // this is sigil followed immediately by comment -- treat as isolated sigil 

                            M__(M_out(L"token_stream_C::fetch_raw_token() -- dangling sigil found before comment -- restarting loop with default interpretation");)

                            ch_subtype = ch_orig_subtype;                        // treat this character as non-sigil
                            continue;                         
                    }


                    // check for two-character comment starter

                    if (ch1.ch32 == m_comment_1st_ch)                            // 1st comment char can be solidus, which is a valid operator character, too
                    {
                        in_char_S ch2 {}; 
                        peek_char(ch2, 3);                                       // look at char following the solidus (may indicate start of comment) -- leave it in the char stream  

                        if (
                            ( ch2.ch32 == m_unechoed_line_comment_2nd_ch  )
                            ||                                            
                            ( ch2.ch32 == m_echoed_line_comment_2nd_ch    )
                            ||                                            
                            ( ch2.ch32 == m_suppress_eol_comment_2nd_ch   )
                            ||                                            
                            ( ch2.ch32 == m_eof_comment_2nd_ch            )
                            ||                                                                  
                            ( ch2.ch32 == m_block_comment_2nd_ch          )
                            ||                                            
                            ( ch2.ch32 == m_nest_comment_2nd_ch           )
                            ||                                            
                            ( ch2.ch32 == m_eof_comment_2nd_ch            )
                            ||                                           
                            ( ch2.ch32 == m_retained_block_comment_2nd_ch )
                            ||
                            ( ch2.ch32 == m_retained_line_comment_2nd_ch  )
                            || 
                            ( ch2.ch32 == m_retained_eof_comment_2nd_ch   )
                           )
                        {
                            // this is sigil followed immediately by comment -- treat as isolated sigil 

                            M__(M_out(L"token_stream_C::fetch_raw_token() -- dangling sigil found before comment -- restarting loop with default interpretation");)

                            ch_subtype = ch_orig_subtype;                       // treat this character as non-sigil
                            continue;     
                        }
                    }


                    // handle sigil followed by general operator character  (includes operator starting with solidus)

                    M__(M_out(L"token_stream_C::fetch_raw_token() -- ch1.type=%d  oper1=%d  oper2=%d") % (int)(ch1.type) % (int)(char_E::oper1) % (int)(char_E::oper2);)  

                    if ( ch1.type == char_E::oper )
                    {    
                        M__(M_out(L"token_stream_C::fetch_raw_token() -- sigil + operator found");)

                        operator_token(token, true);               // start operator token -- indicate that 1st char left in stream is sigil -- can cause - to be stripped off numeric literal
                        break;
                    }


                    // handle sigil followed dot/comma/colon/etc. operator char (these are not general operator characters, and can only appear in same-char operator tokens)
                    
                    else if ( 
                             (ch1.subtype == char_E::full_stop)     // sigil followed by .  
                             ||
                             (ch1.subtype == char_E::comma    )     // sigil followed by , 
                             ||
                             (ch1.subtype == char_E::colon    )     // sigil followed by : 
                            )  
                    {    
                        M__(M_out(L"token_stream_C::fetch_raw_token() -- sigil + dot/comma/colon, etc. operator found");)

                        same_operator_token(token, ch1, true);     // start operator token -- indicate that 1st char left in stream is sigil -- can cause . to be stripped off numeric literal
                        break;
                    }  
                }


                // check for leading sigils for identifier

                M__(M_out(L"token_stream_C::fetch_raw_token() --check for leading identifier sigil");)

                if ( m_allow_leading_id_sigils && is_in_vector(ch.ch32, m_leading_ident_sigils) )
                {
                    M__(M_out(L"token_stream_C::fetch_raw_token() -- leading identifier sigil found");)

                    if (  
                        ( ch1.subtype == char_E::alpha1     )
                        ||                                  
                        ( ch1.subtype == char_E::alpha2     )
                        ||                                  
                        ( ch1.subtype == char_E::alpha3     )
                        ||                                  
                        ( ch1.subtype == char_E::alpha4     )
                        ||                                  
                        ( ch1.subtype == char_E::underscore )
                        ||
                        ( 
                         m_allow_extd_identifier 
                         && 
                         (ch1.subtype == char_E::currency   )  
                        )
                       )
                    {   
                        M__(M_out(L"token_stream_C::fetch_raw_token() -- sigil + identifier found");)

                        identifier_token(token, true);               // start identifier token -- indicate that 1st char left in stream is sigil
                        break;
                    }
                }

                
                // check for leading sigils for open parenthesis (close paren has only trailing sigils)

                if (
                    ch1.subtype == char_E::open_paren1
                    &&
                    ( m_allow_paren_sigils && is_in_vector(ch.ch32, m_paren_sigils) )
                   )
                {
                    M__(M_out(L"token_stream_C::fetch_raw_token() -- open parenthesis sigil found");) 

                    start_token(token, token_E::open_paren);          // start new (empty) open_paren token
                    add_leading_sigil(token, ch);                     // save leading sigil in new token 
                    discard_char();                                   // discard peek()ed sigil char  
                    addto_token(token, ch1);                          // add open paren char to token (with leading sigil already set)
                    discard_char();                                   // discard peek()ed open parenthesis

                    break;
                }


                // sigils not allowed, or this is not a valid leading sigil / token combination

                M__(M_out(L"token_stream_C::fetch_raw_token() -- dangling sigil found -- restarting loop with default interpretation");)

                ch_subtype = ch_orig_subtype;                       // treat this character as non-sigil
                continue;                                           // loop back with unconsumed sigil character
            }
         

            //  -----------------------------------------------------------------------------------------------------------------------
            //  currency char -- could be start of an extended identifier, if allowed, or else is reported as a special character token (probably an error)
            //  -----------------------------------------------------------------------------------------------------------------------

            case char_E::currency : 

                if (m_allow_extd_identifier)
                     identifier_token(token);
                else
                     one_char_token(token, token_E::special_char        , ch);   

                break; 
    

            // --------------------
            // comma operator token --  ,   ,,   ,,,   ,,,,   etc.
            // --------------------

            case char_E::comma                 :
                same_operator_token(token, ch);
                break;
       

            // --------------------
            // colon operator token --  :   ::   :::   ::::   etc.    (must be after leading sigil test ) 
            // --------------------

            case char_E::colon                 :                            
                same_operator_token(token, ch);
                break;

            
            //  -----------------------------------------------------------------------------------------------------------------
            //  handle unsupported character at start of new token -- all other character types are unexpected at start of tokens
            //  -----------------------------------------------------------------------------------------------------------------
       
            default: 
       
                if (!m_quiet_mode)
                    M_out_emsg(L"Unsupported char (%08X) at line=%u column=%u in file = \"%s\"") % (uint32_t)(ch.ch32) % ch.lineno % ch.linepos % m_in_filename; 
                count_error();
                one_char_token(token, token_E::unsupported, ch);
                break;

                                              // ======================
        }                                     // end of main switch ( ) 
                                              // ======================

        break;                                //  loop does not repeat, unless continue is done before control gets here 
    }                                         //  end of for(;;) loop


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //  Set "prefix_mode" flag, if next operator (with no preceding space) would be prefix (unary)
    //  ------------------------------------------------------------------------------------------
   
    if (
        (token.type == token_E::number                  ) 
        ||
        (token.type == token_E::int8                    ) 
        ||
        (token.type == token_E::int16                   ) 
        ||
        (token.type == token_E::int32                   ) 
        ||
        (token.type == token_E::int64                   ) 
        ||
        (token.type == token_E::uint8                   ) 
        ||
        (token.type == token_E::uint16                  ) 
        ||  
        (token.type == token_E::uint32                  ) 
        ||
        (token.type == token_E::uint64                  ) 
        ||
        (token.type == token_E::float32                 ) 
        ||
        (token.type == token_E::float64                 ) 
        ||
        ( (token.type == token_E::identifier            ) && (!token.has_leading_sigil) && (!token.has_trailing_sigil) )    // allow only identifiers without sigils  -- problem if $ sigil is ever allowed for plain identifiers ????????????????????????
        ||
        ( (token.type == token_E::extended_identifier   ) && (!token.has_leading_sigil) && (!token.has_trailing_sigil) )    // allow only identifiers without sigils 
        ||
        (token.type == token_E::close_paren             ) 
       )
    {
        m_attach_mode = false;      // next + or - should be infix, and not eligible for attachment  (if there is no whitespace between this token and the + or -)
    }
    else if (
             (token.type == token_E::whitespace         ) 
             ||
             (token.type == token_E::line_comment       ) 
             ||
             (token.type == token_E::eol                )         
            )
    {
        m_attach_mode = true;       // indicate that next + or - should be eligible for attachment to numeric literal, due to preceding whitespace 
    }
    else    // all other tokens:    operators, open parens, brackets, braces, keywords, etc.
    {
        m_attach_mode = true;   // indicate that next + or - should be eligible for attachment to numeric literal     
    }
    

    //  Set "error seen" flag and R/C, if returning an error token or unsupported character token

    if ( (token.type == token_E::error) || (token.type == token_E::unsupported) )
    {
        m_error_seen = true; 
        M__(M_out(L"token_stream_C::fetch_raw_token() -- closing all files");)
        close_all_instreams();    
        return -1;
    }
    

    // accumulate raw token count

    static_N::raw_token_count++; 

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// fetch_composite_token() -- build composite token from input characters from file (does not combine whitespace, etc.)  
// -----------------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

//-------------------------------------------------------------------------------------------------
//     helper function -- get next raw token from stack or call fetch_raw_token() if stack is empty
//-------------------------------------------------------------------------------------------------

int token_stream_C::fetch_pop_raw_token(token_C& token) try
{
    int      rc  {0};       
    token_C  tok {}; 

    if (!m_raw_token_stack.empty())
    {
        token = m_raw_token_stack.front();  
        m_raw_token_stack.pop_front(); 

        if ( (token.type == token_E::error) || (token.type == token_E::invalid) )
            rc = -1; 
    }
    else
    {
        int rc1 = fetch_raw_token(token);
    }
  
    M__(token.display(L"token_stream_C::fetch_pop_raw_token()");)    
    return rc;   
}
M_endf



//------------------------------------------------------------------------------------------------------------------------------------
//     helper function -- Skip whitespace -- keep fetching raw tokens until non-whitespace reached -- returns 1st non-whitespace token
//------------------------------------------------------------------------------------------------------------------------------------

int token_stream_C::fetch_non_whitespace(token_C& token) try
{
    int      rc  {0};       
    token_C  tok {}; 

    for(;;)   // loop.looking for non-whitespace, or error 
    {
        int rc1 = fetch_pop_raw_token(tok);
        
        if (rc1 != 0)
            rc = rc1; 
        if ( (tok.type == token_E::error) || (tok.type == token_E::invalid) )
            rc = -1; 

        if (tok.type != token_E::whitespace)
            break;                   
    }

    token = tok; 
    M__(token.display(L"token_stream_C::fetch_non_whitespace()");)
    return rc;   
}
M_endf


//------------------------------------------------------------------------------------------------------------------------------------

int token_stream_C::fetch_composite_token(token_C& token)  try            
{
    token_C tok  {}; 
    token_C tok1 {}; 
    int     rc   {0};  


    // main loop to find next token that is not a vanishing separator (which can't be passed back)

    for(;;)
    {
        M__(M_out(L"fetch_composite_token():point 0 -- raw stack size = %d") % m_raw_token_stack.size();)
        rc = fetch_pop_raw_token(tok);


        //  Combine or skip whitespace tokens, if required -- note r/c should still be 0 here, if whitespace just fetched
        //  (note: comments may have been converted to whitespace before this, so they may be handled as whitespace here) 
      
        if (tok.type == token_E::whitespace)
        {
             if (m_skip_whitespace)                              // skip whitespace takes priority over combine whitespace
             {
                 M__(M_out(L"fetch_composite_token():point 4 -- raw stack size = %d") % m_raw_token_stack.size();)
                 rc  = fetch_non_whitespace(tok);                // replace whitespace token with 1st non-whitespace token, update R/C and go check for string token
                 M__(M_out(L"fetch_composite_token():point 5 -- raw stack size = %d") % m_raw_token_stack.size();)
             }
             else if (m_combine_whitespace)
             {
                 for (;;)
                 {
                     M__(M_out(L"fetch_composite_token():point 6 -- raw stack size = %d") % m_raw_token_stack.size();)
                     fetch_pop_raw_token(tok1);                  // ignore R/C, since any error/invalid token will get saved for next time             
                     M__(M_out(L"fetch_composite_token():point 7 -- raw stack size = %d") % m_raw_token_stack.size();)
      
                     if (tok1.type == token_E::whitespace)       // another whitespace raw token -- need to combine it with the earlier whitespace tokens already in tok 
                     {
                         tok.str        = L" ";                  // normalize combined string to just one blank (could have been empty before) 
                         tok.orig_str  += tok1.orig_str;         // append new row token's original string
                         tok.linepos2   = tok1.linepos2;         // update ending line position
                         tok.lineno2    = tok1.lineno2;          // update ending line number
                         tok.source_id2 = tok1.source_id2;       // update ending source_id
                     }
                     else           // newest token is not a whitespace token -- save it away for next time, and pass back combined whitespace
                     {
                         m_raw_token_stack.push_front(tok1);     // save away non-whitespace raw token for next time (might be an error token) 
                         token = tok;                            // tok has combined whitespace raw tokens
                         return rc; 
                     }
                 }
             }
             else        //  Not combining or skipping whitespace -- need to pass back each whitespace raw token individually (raw stack is empty for next time)
             {
                 token = tok;         // pass back the raw whitespace token -- no need to look ahead
                 return rc;   
             }
        }
        
      
        // note: control gets here if: (1) 1st raw token was not whitespace, -or- (2) 1st raw token was whitespace that was skipped -- tok now has 1st non-whitespace raw token
      
      
        //  Combine any adjacent strings from a single source, if requested, but only if whitespace between strings is being skipped over
      
        if (tok.type == token_E::string)
        {
            M__(M_out(L"fetch_composite_token() -- string token seen -- m_combine_strings=%d  m_skip_whitespace=%d") % (int)m_combine_strings % (int)m_skip_whitespace;)

            if ( m_combine_strings && m_skip_whitespace )       // need to combine strings only if both these flags are set
            {
                M__(M_out(L"fetch_composite_token() -- seeing if strings need to be combined");)

                for (;;)
                {
                    fetch_non_whitespace(tok1);                 // find next non-whitespace string -- ignore R/C, since any error/invalid token will get saved for next time                 
      
                    M__(M_out(L"fetch_composite_token() -- tok1.type = %d (token_E::string = %d)") % (int)(tok1.type) % (int)(token_E::string);)

                    if (
                        (tok1.type == token_E::string)           // another string raw token -- need to combine it with the earlier whitespace tokens already in tok 
                        &&
                        (tok1.source_id1 == tok.source_id1)      // make sure not combining strings from different sources
                        &&
                        (tok1.source_id2 == tok.source_id2)      // make sure not combining strings from different sources
                       )
                    {
                        tok.str       += tok1.str;               // append new token's passback string text  
                        tok.orig_str  += tok1.orig_str;          // append new row token's original string
                        tok.linepos2   = tok1.linepos2;          // update ending line position
                        tok.lineno2    = tok1.lineno2;           // update ending line number
                        tok.source_id2 = tok1.source_id2;        // update ending source_id
                    }
                    else           // newest non-whitespace token is not a string token -- save it away for next time, and pass back combined whitespace
                    {
                        m_raw_token_stack.push_front(tok1);     // save away non-whitespace raw token for next time (might be error token)
                        token = tok;                            // tok has combined whitespace raw tokens
                        return rc; 
                    }
                }        
            }
            else      // just pass back this raw string individually (raw stack is be empty for next time)
            {
                token = tok; 
                return rc;         
            }   
        }
      
      
        //  Not whitepace or string -- all other raw tokens (except vanishing separators) get passed back individually  (raw stack will be empty for next time) 
      
        if (tok.type == token_E::vanishing_separator)  // don't pass back vanishing separator -- just repeat main loop
            continue; 

        token = tok; 
        return rc;   

    }  // end of main loop


    // note: should not get here

    M_throw("fetch_composite_token(): main loop fell through")
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////