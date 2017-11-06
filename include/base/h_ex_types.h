// h_ex_types.h

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ============
////            h_ex_types.h -- common types for the lexer/parser/evaluator, etc. 
////            ============
//// 
////    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h__types.h"             // ex stuff is dependent on common types


// -----------------------------------------------------------------------
// forward type declarations (incomplete structure/union/class/enum types)
// -----------------------------------------------------------------------

struct                       a_expression_S;
struct                       attach_info_S;
struct                       cache_S;
enum class                   char_E;
class                        char_stream_C;
struct                       e_expression_S; 
struct                       fieldef_S; 
struct                       frame_S; 
struct                       block_S;
class                        infile_C;
class                        instream_C;
class                        instring_C;
struct                       in_char_S;
struct                       parmtype_S;
struct                       plist_S;
class                        pre_process_C; 
struct                       results_S; 
struct                       environ_S;
struct                       symval_S;
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
enum class                   type_E;
struct                       typdef_S;
struct                       value_S; 
struct                       var_S; 
struct                       verbadd_S;
struct                       verbdef_S; 
struct                       verbset_S; 
struct                       vlist_S; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////