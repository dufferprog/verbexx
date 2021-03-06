﻿// ex_eval.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===========
////     ex_eval.cpp -- input stream evaluation functions
////     ===========
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


#define NOEVAL_VERBLESS false           // do evaluate values in verbless expressions


//  forward declarations for selected static functions
//  --------------------------------------------------

static int          check_verb_parms(frame_S&, const e_expression_S&, const verbdef_S&                                                                 );
static int          check_verb_value(frame_S&, const e_expression_S&, const parmtype_S&, const value_S&, const std::wstring&                           );
static int          check_verb_vlist(frame_S&, const e_expression_S&, const plist_S&   , const vlist_S&, const std::wstring&, const std::wstring& = L"");  



//    static variables 
//    ----------------

namespace static_N
{
                            // evaluation statistics and counters

uint64_t                   verb_count      {0};                  // number of verbs executed
uint64_t                   value_count     {0};                  // number of values evaluated
uint64_t                   block_count     {0};                  // number of blocks executed
uint64_t                   statement_count {0};                  // number of blocks executed

uint64_t                   frame_serial    {0};                  // serial number of most-recently allocated (or filld-in) frame_S 
uint64_t                   frame_depth     {0};                  // current number of stack frames on stack 
uint64_t                   frame_max_depth {0};                  // maximum number of frames on stack (high-water mark) 

std::shared_ptr<frame_S>   newest_frame_sp { };                  // owning pointer to newest stack frame on stack (keeps ref-count positive)  
}



                                                                                                                                                     
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
////     frame statistics/utility functions
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

uint64_t get_eval_verb_count(      void) {return static_N::verb_count            ;}
uint64_t get_eval_value_count(     void) {return static_N::value_count           ;}
uint64_t get_eval_block_count(     void) {return static_N::block_count           ;}
uint64_t get_eval_statement_count( void) {return static_N::statement_count       ;}
                                                                                 
uint64_t get_eval_frame_serial(    void) {return static_N::frame_serial          ;}
uint64_t get_eval_frame_depth(     void) {return static_N::frame_depth           ;}
uint64_t get_eval_frame_max_depth( void) {return static_N::frame_max_depth       ;} 

frame_S *get_newest_sf(            void) {return static_N::newest_frame_sp.get() ;}



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////     get_vlist_positional() -- get n-th positional value in vlist (if present)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int get_vlist_positional(const vlist_S& vlist, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th  positional parm is not present

    if (vlist.value_ct <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th positional parm is present -- return parm value

    value = vlist.values.at(n);
    return 0; 
}
M_endf





////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////     get_right_positional() -- get n-th positional value in right-side vlist (if present)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int get_right_positional(const e_expression_S& expression, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th  positional right-side parm is not present

    if (expression.rparms.value_ct <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th positional right-side parm is present -- return parm value

    value = expression.rparms.values.at(n);
    return 0; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////     get_left_positional() -- get n-th positional value in left-side vlist (if present)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int get_left_positional(const e_expression_S& expression, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th  positional left-side parm is not present

    if (expression.lparms.value_ct <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th positional left-side parm is present -- return parm value

    value = expression.lparms.values.at(n);
    return 0; 
}
M_endf  



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////     get_right_keyword() -- get n-th right-side keyword value (if present)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// version with passed-back value parm 
// -----------------------------------

int get_right_keyword(const e_expression_S& expression, const std::wstring& keyword_name, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th right keyword is not present

    if (expression.rparms.eval_kws.count(keyword_name) <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th keyword is present -- return keyword value

    value = multimap_at(expression.rparms.eval_kws, keyword_name, n);
    return 0; 
}
M_endf


// version with passed-back string parm 
// ------------------------------------

int get_right_keyword(const e_expression_S& expression, const std::wstring& keyword_name, std::wstring& ws, uint32_t n) try
{
    // return  R/C = -1, if n-th right keyword is not present -- don't modify caller's string

    if (expression.rparms.eval_kws.count(keyword_name) <= n)
        return -1; 


    // n-th keyword is present -- find keyword value

    auto value = multimap_at(expression.rparms.eval_kws, keyword_name, n);

    if (value.ty == type_E::string)
    {
        ws = value.string;
    }
    else
    {
        count_error(); 
        M_out_emsg1(L"get_right_keyword() -- verb= %s -- right keyword \"%S\" (%d) was not a string value, as expected") % verb_name(expression) % keyword_name % n;
        msgend_loc(value, expression);
        M_throw_v(   "get_right_keyword() -- verb= %s -- right keyword \"%S\" (%d) was not a string value, as expected") % out_ws(verb_name(expression)) % out_ws(keyword_name) % n ));  
        return -1;     
    }
                 
    return 0; 
}
M_endf


// version with passed-back int64_t parm 
// -------------------------------------

int get_right_keyword(const e_expression_S& expression, const std::wstring& keyword_name, int64_t& int64, uint32_t n) try
{
    // return  R/C = -1, if n-th right keyword is not present -- don't modify caller's string

    if (expression.rparms.eval_kws.count(keyword_name) <= n)
        return -1; 


    // n-th keyword is present -- find keyword value

    auto value = multimap_at(expression.rparms.eval_kws, keyword_name, n);

    if (value.ty == type_E::int64)
    {
        int64 = value.int64;
    }
    else
    {
        count_error(); 
        M_out_emsg1(L"get_right_keyword() -- verb= %s -- right keyword \"%S\" (%d) was not an int64 value, as expected") % verb_name(expression) % keyword_name % n;
        msgend_loc(value, expression);
        M_throw_v(   "get_right_keyword() -- verb= %s -- right keyword \"%S\" (%d) was not an int64 value, as expected") % out_ws(verb_name(expression)) % out_ws(keyword_name) % n ));  
        return -1;     
    }
                 
    return 0; 
}
M_endf


// version with passed-back float64_T parm 
// ---------------------------------------

int get_right_keyword(const e_expression_S& expression, const std::wstring& keyword_name, float64_T& float64, uint32_t n) try
{
    // return  R/C = -1, if n-th right keyword is not present -- don't modify caller's string

    if (expression.rparms.eval_kws.count(keyword_name) <= n)
        return -1; 


    // n-th keyword is present -- find keyword value

    auto value = multimap_at(expression.rparms.eval_kws, keyword_name, n);

    if (value.ty == type_E::float64)
    {
        float64 = value.float64;
    }
    else
    {
        count_error(); 
        M_out_emsg1(L"get_right_keyword() -- verb= %s -- right keyword \"%S\" (%d) was not a float64 value, as expected") % verb_name(expression) % keyword_name % n;
        msgend_loc(value, expression);
        M_throw_v(   "get_right_keyword() -- verb= %s -- right keyword \"%S\" (%d) was not a float64 value, as expected") % out_ws(verb_name(expression)) % out_ws(keyword_name) % n ));  
        return -1;     
    }
                 
    return 0; 
}
M_endf





// version without passed-back value parm -- just R/C 
// --------------------------------------------------

int get_right_keyword(const e_expression_S& expression, const std::wstring& keyword_name, uint32_t n) try
{
    // return  R/C = -1, if n-th right keyword is not present

    if (expression.rparms.eval_kws.count(keyword_name) <= n)
        return -1; 


    // n-th keyword is present -- return 0

    return 0; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////     get_left_keyword() -- get n-th left-side keyword value (if present)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// version with passed-back value parm
// -----------------------------------

int get_left_keyword(const e_expression_S& expression, const std::wstring& keyword_name, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th left keyword is not present

    if (expression.lparms.eval_kws.count(keyword_name) <= n)
    {
        value = unit_val();  
        return -1; 
    }


    // n-th keyword is present -- return keyword value

    value = multimap_at(expression.lparms.eval_kws, keyword_name, n);
    return 0; 
}
M_endf


// version with passed-back string parm 
// ------------------------------------

int get_left_keyword(const e_expression_S& expression, const std::wstring& keyword_name, std::wstring& ws, uint32_t n) try
{
    // return  R/C = -1, if n-th left keyword is not present -- don't modify caller's string

    if (expression.lparms.eval_kws.count(keyword_name) <= n)
        return -1; 


    // n-th keyword is present -- find keyword value

    auto value = multimap_at(expression.lparms.eval_kws, keyword_name, n);

    if (value.ty == type_E::string)
    {
        ws = value.string;
    }
    else
    {
        count_error(); 
        M_out_emsg1(L"get_left_keyword() -- verb= %s -- left keyword \"%S\" (%d) was not a string value, as expected") % verb_name(expression) % keyword_name % n;
        msgend_loc(value, expression);
        M_throw_v(   "get_left_keyword() -- verb= %s -- left keyword \"%S\" (%d) was not a string value, as expected") % out_ws(verb_name(expression)) % out_ws(keyword_name) % n ));  
        return -1;     
    }
                 
    return 0; 
}
M_endf


// version with passed-back int64_t parm 
// -------------------------------------

int get_left_keyword(const e_expression_S& expression, const std::wstring& keyword_name, int64_t& int64, uint32_t n) try
{
    // return  R/C = -1, if n-th left keyword is not present -- don't modify caller's string

    if (expression.lparms.eval_kws.count(keyword_name) <= n)
        return -1; 


    // n-th keyword is present -- find keyword value

    auto value = multimap_at(expression.lparms.eval_kws, keyword_name, n);

    if (value.ty == type_E::int64)
    {
        int64 = value.int64;
    }
    else
    {
        count_error(); 
        M_out_emsg1(L"get_left_keyword() -- verb= %s -- left keyword \"%S\" (%d) was not an int64 value, as expected") % verb_name(expression) % keyword_name % n;
        msgend_loc(value, expression);
        M_throw_v(   "get_left_keyword() -- verb= %s -- left keyword \"%S\" (%d) was not an int64 value, as expected") % out_ws(verb_name(expression)) % out_ws(keyword_name) % n ));  
        return -1;     
    }
                 
    return 0; 
}
M_endf


// version with passed-back float64_T parm 
// ----------------------------------------

int get_left_keyword(const e_expression_S& expression, const std::wstring& keyword_name, float64_T& float64, uint32_t n) try
{
    // return  R/C = -1, if n-th left keyword is not present -- don't modify caller's string

    if (expression.lparms.eval_kws.count(keyword_name) <= n)
        return -1; 


    // n-th keyword is present -- find keyword value

    auto value = multimap_at(expression.lparms.eval_kws, keyword_name, n);

    if (value.ty == type_E::float64)
    {
        float64 = value.float64;
    }
    else
    {
        count_error(); 
        M_out_emsg1(L"get_left_keyword() -- verb= %s -- left keyword \"%S\" (%d) was not a float64 value, as expected") % verb_name(expression) % keyword_name % n;
        msgend_loc(value, expression);                                                            
        M_throw_v(   "get_left_keyword() -- verb= %s -- left keyword \"%S\" (%d) was not a float64 value, as expected") % out_ws(verb_name(expression)) % out_ws(keyword_name) % n ));  
        return -1;     
    }
                 
    return 0; 
}
M_endf


// version without passed-back value 
// ---------------------------------

int get_left_keyword(const e_expression_S& expression, const std::wstring& keyword_name, uint32_t n) try
{
    // return  R/C = -1, if n-th right keyword is not present

    if (expression.lparms.eval_kws.count(keyword_name) <= n)
        return -1; 
    

    // n-th keyword is present -- return 0

    return 0; 
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////     get_vlist_keyword() -- get value of n-th occurrence of passed-in keyword name, from vlist (if present)
////                         -- get value of n-th keyword in vlist (any keyword -- if no keyword name passed-in)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// version with passed-in keyword name -- passed-in value
// ------------------------------------------------------

int get_vlist_keyword(const vlist_S& vlist, const std::wstring& keyword_name, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th  keyword is not present

    if (vlist.eval_kws.count(keyword_name) <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th keyword is present -- return keyword value

    value = multimap_at(vlist.eval_kws, keyword_name, n);
    return 0; 
}
M_endf


// version with passed-in keyword name -- no passed-in value -- just R/C only
// --------------------------------------------------------------------------

int get_vlist_keyword(const vlist_S& vlist, const std::wstring& keyword_name, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th  keyword is not present

    if (vlist.eval_kws.count(keyword_name) <= n)
        return -1; 
    

    // n-th keyword is present -- normal R/C

    return 0; 
}
M_endf




// version with no passed-in keyword name
// --------------------------------------

int get_vlist_keyword(const vlist_S& vlist, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th  keyword is not present

    if (vlist.eval_kws.size() <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th keyword is present -- return keyword value

    value = multimap_deref(vlist.eval_kws, n).second;
    return 0; 
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



//╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║                                           CCCCCCCC         HH      HH        EEEEEEEEEE         CCCCCCCC          KK      KK
//║╳╳╳╳║                                          CCCCCCCCCC        HH      HH        EEEEEEEEEE        CCCCCCCCCC         KK     KK     
//║╳╳╳╳║                                          CC      CC        HH      HH        EE                CC      CC         KK    KK  
//║╳╳╳╳║                                          CC                HH      HH        EE                CC                 KK   KK   
//║╳╳╳╳║                                          CC                HHHHHHHHHH        EEEEEEEE          CC                 KKKKKK    
//║╳╳╳╳║                                          CC                HHHHHHHHHH        EEEEEEEE          CC                 KKKKKK    
//║╳╳╳╳║                                          CC                HH      HH        EE                CC                 KK   KK   
//║╳╳╳╳║                                          CC      CC        HH      HH        EE                CC      CC         KK    KK  
//║╳╳╳╳║                                          CCCCCCCCCC        HH      HH        EEEEEEEEEE        CCCCCCCCCC         KK     KK 
//║╳╳╳╳║                                           CCCCCCCC         HH      HH        EEEEEEEEEE         CCCCCCCC          KK      KK
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
////   check_verb_value() -- complain and return error, if passed-in value doesn't match what is expected in the passed-in parmtype_S structure
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// internal helper function for check_verb_parms()

static int check_verb_value(frame_S& frame, const e_expression_S& expression, const parmtype_S& parmtype, const value_S& value, const std::wstring& ws) try
{
    int rc {0}; 

    // return OK, if no checking needed -- includes bypassing nested vlist checking

    if (parmtype.anything_ok)
        return 0; 
   

    // check this value type compared to what is allowed in passed-in parmtype_S structure

    if ( (!parmtype.nval_ok) && (value.ty == type_E::no_value) )         // keyword with no associated value
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected keyword with no following value") % verb_name(expression) % ws; 
        msgend_loc(value, expression);
        rc = -1;  
    }
       
    if ( (!parmtype.unit_ok) && (value.ty == type_E::unit) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected unit parm") % verb_name(expression) % ws; 
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.int8_ok) && (value.ty == type_E::int8) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected int8 parm = %d") % verb_name(expression) % ws % (int16_t)(value.int8);
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.int16_ok) && (value.ty == type_E::int16) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected int16 parm = %d") % verb_name(expression) % ws % value.int16;
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.int32_ok) && (value.ty == type_E::int32) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected int32 parm = %d") % verb_name(expression) % ws % value.int32;
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.int64_ok) && (value.ty == type_E::int64) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected int64 parm = %d") % verb_name(expression) % ws % value.int64;
        msgend_loc(value, expression);
        rc = -1;  
    }
    
    if ( (!parmtype.uint8_ok) && (value.ty == type_E::uint8) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected uint8 parm = %d") % verb_name(expression) % ws % (uint16_t)(value.uint8);
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.uint16_ok) && (value.ty == type_E::uint16) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected uint16 parm = %d") % verb_name(expression) % ws % value.uint16;
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.uint32_ok) && (value.ty == type_E::uint32) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected uint32 parm = %d") % verb_name(expression) % ws % value.uint32;
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.uint64_ok) && (value.ty == type_E::uint64) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected uint64 parm = %d") % verb_name(expression) % ws % value.uint64;
        msgend_loc(value, expression);
        rc = -1;  
    }


    if ( (!parmtype.float32_ok) && (value.ty == type_E::float32) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected float32 parm = %g") % verb_name(expression) % ws % value.float32;
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.float64_ok) && (value.ty == type_E::float64) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected float64 parm = %g") % verb_name(expression) % ws % value.float64;
        msgend_loc(value, expression);
        rc = -1;  
    }
    
    if ( (!parmtype.string_ok) && (value.ty == type_E::string) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected string parm = «%s»") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.verbname_ok) && (value.ty == type_E::verbname) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected verbname parm = «%s»") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
        msgend_loc(value, expression);
        rc = -1;  
    }
     


    // check (unevaluated) identifiers -- raw(all) / variable /constant / undefined
    // ----------------------------------------------------------------------------

    if (value.ty == type_E::identifier)
    {   
        if (parmtype.raw_ident_ok)                     // allow any kind of (unevaluated) identifier? -- includes typdef and verbset ????
        {
            ;                                          // OK -- no need to check identifier type (var/const/undef)
        }
        else if (                                      // see if some type of identifier is OK
                 (!parmtype.var_ident_ok  )
                  &&
                 (!parmtype.const_ident_ok)   
                  &&
                 (!parmtype.typdef_ident_ok)  
                  &&
                 (!parmtype.verbset_ident_ok)  
                  &&
                 (!parmtype.undef_ident_ok) 
                ) 
        {
            // not expecting any type of identifier -- error

            count_error(); 
            M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
            msgend_loc(value, expression);
            rc = -1;  
        }
        else                                          // expecting some specific type(s) of identifier -- variable/const/verbset/typdefundef -- need to check further
        {
            // check local, global, or all stack frames for this identifier
            //   note: assume local and global flags are not both set to true
              
            if (parmtype.check_local_env_only)
            {
                // check only local identifiers (not all) 
         
                if ( (!parmtype.var_ident_ok) && is_local_identifier_variable(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) already-defined local variable identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max) ;
                    msgend_loc(value, expression);
                    rc = -1;  
                }
         
                if ( (!parmtype.const_ident_ok) &&  is_local_identifier_const(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) already-defined local constant identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }

                if ( (!parmtype.typdef_ident_ok) &&  is_local_identifier_typdef(frame, value.string) )  
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) already-defined local typdef identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }

                if ( (!parmtype.verbset_ident_ok) &&  is_local_identifier_verbset(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) already-defined local verbset identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }

                if ( (!parmtype.undef_ident_ok) && (!is_local_identifier_defined(frame, value.string)) )
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected undefined local identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                } 
            }
         
            else if (parmtype.check_global_env_only)
            {
                // check only global identifiers (not all) 
         
                if ( (!parmtype.var_ident_ok) &&  is_global_identifier_variable(value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) global already-defined variable identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }
         
                if ( (!parmtype.const_ident_ok) &&  is_global_identifier_const(value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) global already-defined constant identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }
              
                if ( (!parmtype.typdef_ident_ok) &&  is_global_identifier_typdef(value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) global already-defined typdef identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }
              
                if ( (!parmtype.verbset_ident_ok) &&  is_global_identifier_verbset(value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) global already-defined verbset identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }

                if ( (!parmtype.undef_ident_ok) &&  (!is_global_identifier_defined(value.string)) )
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected undefined global identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                } 
            }
 
            else      // neither local or global flag is set -- check all stack frames for this identifier
            {
                // check all stack frames to see what identifier type is (var/const/undef)
          
                if ( (!parmtype.var_ident_ok) &&  is_identifier_variable(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) already-defined variable identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }
              
                if ( (!parmtype.const_ident_ok) &&  is_identifier_const(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) already-defined constant identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }
  
                if ( (!parmtype.typdef_ident_ok) &&  is_identifier_typdef(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) already-defined typdef identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }

                if ( (!parmtype.verbset_ident_ok) &&  is_identifier_verbset(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) already-defined verbset identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }
                                      
                if ( (!parmtype.undef_ident_ok) && (!is_identifier_defined(frame, value.string)) )
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected undefined identifier parm = %s") % verb_name(expression) % ws % shorten_str(value.string, const_N::loctext_max);
                    msgend_loc(value, expression);
                    rc = -1;  
                }
            }
        }               // expecting specific identifier type(s) -- variable/const/undef 
    }                   // identifier


    // check vlist, block, expression, verbdef, etc.
    // ---------------------------------------------

    M__(M_out(L"check_verb_value() -- value.ty = %d = %S") % (int)(value.ty) % type_str(value.ty);)

    if ( (!parmtype.vlist_ok) && (value.ty == type_E::vlist) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected vlist parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.expression_ok) && (value.ty == type_E::expression) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected expression parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }
    
    if ( (!parmtype.block_ok) && (value.ty == type_E::block) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected block parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }

    if ( (!parmtype.verbset_ok) && (value.ty == type_E::verbset) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected verbdef parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }                                                                        

    if ( (!parmtype.typdef_ok) && (value.ty == type_E::typdef) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected typdef parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }  


    if ( (!parmtype.array_ok) && (value.ty == type_E::array) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected array parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }  
    

    if ( (!parmtype.structure_ok) && (value.ty == type_E::structure) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected structure parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }  


    if ( (!parmtype.lvalue_ref_ok) && ((value.ty == type_E::ref) && (value.ref_sp->is_lvalue)) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected lvalue-ref parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }  


    if ( (!parmtype.rvalue_ref_ok) && ((value.ty == type_E::ref) && (value.ref_sp->is_rvalue)) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected rvalue-ref parm") % verb_name(expression) % ws;
        msgend_loc(value, expression);
        rc = -1;  
    }  


    // do int64 and float64 range checking
    // -----------------------------------

    if ( (value.ty == type_E::int64) && parmtype.int64_ok && parmtype.int64_range )
    {
        if ( (value.int64 < parmtype.int64_min) || (value.int64 > parmtype.int64_max) )
        {
            count_error(); 
            M_out_emsg1(L"check_verb_value() -- verb= %s %s -- int64 value is not in allowed range -- min=%d  value=%d  max=%d") % verb_name(expression) % ws % parmtype.int64_min % value.int64 % parmtype.int64_max;
            msgend_loc(value, expression);
            rc = -1;         
        }    
    }

    if ( (value.ty == type_E::float64) && parmtype.float64_ok && parmtype.float64_range )
    {
        if ( (value.float64 < parmtype.float64_min) || (value.float64 > parmtype.float64_max) )
        {
            count_error(); 
            M_out_emsg1(L"check_verb_value() -- verb= %s %s -- float64 value is not in allowed range -- min=%g  value=%g  max=%g") % verb_name(expression) % ws % parmtype.float64_min % value.float64 % parmtype.float64_max;
            msgend_loc(value, expression);
            rc = -1;         
        }    
    }
    

    // validate nested vlist (if any) -- if passed-in parmtype_S points to a nested plist_S
    
    if ( parmtype.vlist_ok && (value.ty == type_E::vlist) && (parmtype.plist_sp.get() != nullptr) && (value.vlist_sp.get() != nullptr) )    
    {
        auto crc = check_verb_vlist(frame, expression, *(parmtype.plist_sp), *(value.vlist_sp), L" " + ws + L"nested vlist", L"");
        if (crc != 0) rc = crc; 
    }
    
    return rc; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   check_verb_vlist() -- complain and return error, if values in passed-in vlist do not match what is expected in the passed-in plist_S structure
////                      -- note vlist should contain evaluated keywords
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// internal helper function for check_verb_parms()

static int check_verb_vlist(frame_S& frame, const e_expression_S& expression, const plist_S& plist, const vlist_S& vlist, const std::wstring& ws1, const std::wstring& ws2) try
{
    int rc {0}; 


    // check positional parms
    // ----------------------

    if (!plist.no_check_positional)
    { 
        // check positional parm count
       
        if (vlist.value_ct < plist.min_ct)
        {
            count_error(); 
            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s expects at least %d %s positional parm(s), but only %d were present") % verb_name(expression) % ws1 %  plist.min_ct % ws2 % vlist.value_ct;
            msgend_loc(vlist, expression);
            rc = -1;          
        }
       
        M__(M_out(L"check_verb_vlist() -- verb = %S  plist.max_ct = %d  vlist.value_ct = %d")  % verb_name(expression) % plist.max_ct % vlist.value_ct; )


        if  ( (plist.max_ct >= 0) && (vlist.value_ct > plist.max_ct) )
        {
            count_error(); 
            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s expects at most %d %s positional parm(s), but %d were present -- possible missing semicolon?") % verb_name(expression) % ws1 % plist.max_ct % ws2 % vlist.value_ct;
            msgend_loc(vlist, expression);
            rc = -1;          
        }


        // set up default parmtype_S to use if vector does not have any parmtype_S elements (note this code is not executed during verb overload set pre-expansion)
      
        parmtype_S default_parmtype {};                                                                                                              // default parmtype
        default_parmtype.eval         = plist.eval;                                                                                                  // copy in eval flags (set only for verbless expression)    
        default_parmtype.anything_ok  = true;                                                                                                        // allow anything -- default is no checking 
      
      
        // loop to look at each positional parm and do parm type checking
      
        if (!plist.no_check_positional)
        {
            for (int i = 0; i < vlist.values.size(); i++)
            {
                std::wstring ws {ws1 + ws2 + L" positional parm " + std::to_wstring(i) + L":"};
           
           
                if (plist.values.empty())                                                                                                            // no parmtype_S elements available? 
                {                                                                                                                                   
                    auto crc = check_verb_value(frame, expression, default_parmtype, vlist.values.at(i), ws);                                        // use default one just constructed -- no checking
                    if (crc != 0)  rc = crc;                                                                                                         // remember any error   
                }                                                                                                                                   
                else                                                                                                                                 // some parmtype_S are available in plist.values vector -- use i-th parmtype_S, or last one if not enough)               
                {
                    auto crc = check_verb_value(frame, expression, plist.values.at(std::min(i, (int)(plist.values.size() - 1))), vlist.values.at(i), ws);
                    if (crc != 0)  rc = crc;                                                                                                         // remember any error
                }           
            }
        }
    }


    // -------------------
    // check keyword parms 
    // -------------------
    
    if (!plist.no_check_keywords)
    {
        if (plist.keywords.size() == 0)
        {
            if (vlist.eval_kws.size() > 0)
            {
                count_error(); 
                M_out_emsg1(L"check_verb_vlist() -- verb= %s %s expects no %s keywords, but %d was/were present") % verb_name(expression) % ws1 % ws2 % vlist.kw_ct;     // unevaluated keyword count
                msgend_loc(vlist, expression);
                rc = -1;          
            }
        }
        else
        {
            std::multiset<std::wstring> kw_done { }; 


            // keywords allowed -- check each keyword that is present in vlist to see if associated value type is allowed -- also check for duplicate kws, etc. 
            // ------------------------------------------------------------------------------------------------------------------------------------------------
        
            for (const auto& elem : vlist.eval_kws)                          // look at each keyword in vlist
            {                                                               
                std::wstring kw_find_name   { };                             // name used to look up keywords in plist keyword list 
                                                                            
                if (plist.no_check_keyword_names)                            // don't check keyword names?
                   kw_find_name = L"";                                       // use dummy substitute name for entry in kw map
                else                                                        
                   kw_find_name = elem.first;                                // look for this keyword name from parms vlist 
                                                                            
                auto key_pct = plist.keywords.count(kw_find_name);           // find "*" or this vlist keyword in list of allowed keywords for this plist      
                                                                            
                if (key_pct == 0)                                            // see if this keyword is not in the list of expected keywords for this vlist 
                {
                    count_error();
                    M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- unexpected %s keyword=%s: present") % verb_name(expression) % ws1 % ws2 % elem.first; 

                    msg_kw_loc(elem.second);
                    msgend_loc(vlist, expression);
                    rc = -1;               
                }
                else if (key_pct == 1)                                       // this keyword or "*"found in list of expected keywords
                {
                    // check for duplicate keywords (or too many)    --  (missing kw, or too few) is checked later

                    auto kw_parmtype = plist.keywords.at(kw_find_name);      // get parmtype_S for this keyword in vlist, or "*" (if not validating keyword names)
                    auto kw_max_ct   = kw_parmtype.kw_max_ct;                // maximum allowed occurrences of this keyword 
                    auto kw_vct      = vlist.eval_kws.count(elem.first);     // get number of occurrences of this keyword in vlist (use real kw name -- assume supplied plist allows 0-nnnn occurrences, if names are not being checked)


                    M__(M_out(L"check_verb_vlist() -- ++++++++++++++++++++++++++++++++++ find_name = %S  located kw parmtype -- no_eval_ident=%S  no_eval_expression=%S  no_eval_vlist=%S   no_eval_ref=%S    verbless=%S")
                              % kw_find_name % M_bool_cstr(kw_parmtype.eval.no_eval_ident) % M_bool_cstr(kw_parmtype.eval.no_eval_expression) % M_bool_cstr(kw_parmtype.eval.no_eval_vlist)% M_bool_cstr(kw_parmtype.eval.no_eval_ref)
                              %                M_bool_cstr(kw_parmtype.eval.verbless)
                        ;
                       )

                    if ( (kw_vct > kw_max_ct) && (kw_done.find(elem.first) == kw_done.end()) )
                    {
                        if (kw_max_ct == 1)
                        {
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- %s keyword=%s: present %d times -- only once is allowed") % verb_name(expression) % ws1 % ws2 % elem.first % kw_vct; 
                            msg_kw_loc(elem.second);
                            msgend_loc(vlist, expression); 
                            rc = -1;
                        }
                        else
                        {
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- %s keyword=%s: present %d times -- maximum of %d times is allowed") % verb_name(expression) % ws1 % ws2 % elem.first % kw_vct % kw_max_ct; 
                            msg_kw_loc(elem.second);
                            msgend_loc(vlist, expression); 
                            M_out_emsgz();
                            rc = -1;
                        }
                    }

                    kw_done.insert(elem.first);                             //mark this one done, so as not to complain multiple times 
                   

                    // check located parmtype_S for this verb against actual value for keyword that is present in verb parms
      
                     auto crc = check_verb_value(frame, expression, kw_parmtype, elem.second, ws1 + ws2 + L" keyword=" + elem.first + L":"); 
                     if (crc != 0) rc = crc;               
                }
                else    // should not occur
                {   M_out_emsg1(L"check_verb_vlist() --  verb= %S %S -- more than one entry in %S expected keyword list -- keyword=%S verb=%S") % verb_name(expression) % ws1 % ws2 % elem.first % verb_name(expression); 
                    msgend_loc(vlist, expression); 
                    M_throw_v(  "check_verb_vlist() --  verb= %s %s -- more than one entry in %s expected keyword list -- keyword=%s verb=%s") % out_ws(verb_name(expression)) % out_ws(ws1) %  out_ws(ws2) % out_ws(elem.first) % out_ws(verb_name(expression)) ));
                    return -2;                                              // should not get here
                }         
            }   
      
      
            //  Look at each keyword in plist to see if there are any required ones missing from vlist, or if there are too few occurrences (too many has already been checked)
            //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------
      
            if ( !(plist.no_check_keyword_names) )                          // bypass missing required kw check if kw names are not being checked  
            {
                for (const auto& elem : (plist.keywords))                   // look at each exepected keyword for this plist
                {
                    auto min_ct = elem.second.kw_min_ct; 
               
                    M__(M_out(L"check_verb_vlist() -- min_ct = %d") % min_ct;)
               
                    if (min_ct > 0)                                         // see if this keyword is required
                    {
                        auto kw_ct = vlist.eval_kws.count(elem.first);      // actual number of occurrences for this keyword
               
                        if ( (kw_ct == 0) && (min_ct >= 1) )                // see if this requried keyword is missing in passed-in vlist keyword parms        
                        {
                            M__(M_out(L"check_verb_vlist() -- missing required kws");)
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- missing required %s keyword=%s:") % verb_name(expression) % ws1 % ws2 % elem.first;
                            msgend_loc(vlist, expression); 
                            M_out_emsgz();
                            rc = -1;                 
                        }
                        else if (kw_ct < min_ct)
                        {
                            M__(M_out(L"check_verb_vlist() -- not enough kws");)
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s  -- %s keyword=%s: is present only %d time(s) -- minimum required times is %d") % verb_name(expression) % ws1 % ws2 % elem.first % kw_ct % min_ct;
                            msgend_loc(vlist, expression); 
                            rc = -1;                 
                        }
                    }         
                }
            }


            // ---------------------------------------------------------------------------------------------------------------------------------------------
            // check for conflicting keywords (if there are any conflict sets for this plist) -- assume plist has no conflict sets, if not checking kw names 
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            //
            // - for each conflict set, either 0 or 1 of the keywords in the conflict set can be present in the vlist
            //

            if (plist.conflicts.size() > 0)   
            {
                // outer loop to look at each conflict set for this plist 

                 for (const auto& conflict_set : plist.conflicts)
                 {
                     int          conflict_ct  {0};                        // number of conflicting keywords found in this conflict set
                     std::wstring conflict_str { };                        // list of conflicting keywords found in this set (for error message)
                                                                         

                     // inner loop to count how many keywords in vlist are in this set (should be 0 or 1)

                    for (const auto& vl_kw_name : vlist.eval_kws)
                    {
                        if (conflict_set.count(vl_kw_name.first) > 0)      // this keyword is in conflict set ?
                        {
                            conflict_ct++; 
                            conflict_str += L" " + vl_kw_name.first + L": ";                          
                        }
                    }


                    // complain if more than one keyword in conflict list was present in this vlist
                
                    if (conflict_ct > 1)
                    {
                        count_error();
                        M_out_emsg1(L"check_verb_vlist() -- verb= %s %s  --  conflicting %s keywords found -- %s") % verb_name(expression) % ws1 % ws2 % conflict_str;
                        msgend_loc(vlist, expression); 
                        rc = -1;                       
                    } 
                 }                               // loop to ckeck each conflict set for this plist
            }                                    // keyword conflict sets present for this plist 



            // ---------------------------------------------------------------------------------------------------------------------------------------------
            // check for multiple choice keywords (if there are any choice sets for this plist) -- assume plist has no choice sets, if not checking kw names 
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            //
            // - for each choice set, one of the keywords in the choice set must be present in the vlist
  
   
            if (plist.choices.size() > 0)   
            {
                // outer loop to look at each choice set for this plist 

                 for (const auto& choice_set : plist.choices)
                 {
                     int          choice_ct  {0};                        // number of choice keywords found in this choice set
                    
                     // inner loop to count how many keywords in vlist are in this set (should be 1 or more)

                    for (const auto& vl_kw_name : vlist.eval_kws)
                    {
                        if (choice_set.count(vl_kw_name.first) > 0)      // this keyword is in choice set ?
                            choice_ct++;                                          
                    }


                    // complain if no keyword, or more than one keyword in the multiple choice set was found in the vlist
                
                    if (choice_ct != 1)
                    {
                        // get string with keywords, one or more of which must be present 

                        std::wstring choice_str { };                      // list of choice keywords found in this set (for error message)

                        for (const auto cs_kw_name : choice_set)
                            choice_str += L" " + cs_kw_name + L": "; 


                        // put out error message

                        count_error();

                        if (choice_ct == 0)
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s  --  one of the following %s keywords must be present -- %s")     % verb_name(expression) % ws1 % ws2 % choice_str;
                        else
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s  --  only one of the following %s keywords can be present -- %s") % verb_name(expression) % ws1 % ws2 % choice_str;

                        msgend_loc(vlist, expression); 
                        rc = -1;                       
                    } 
                 }                               // loop to ckeck each choice set for this plist
            }                                    // keyword choice sets present for this plist 

            

            // --------------------------------------------------------------------------------------------------------------------------------------------
            // check for required matching keywords (if there are any match sets for this plist) -- assume no match sets in plist, if not checking kw names
            // --------------------------------------------------------------------------------------------------------------------------------------------
            //
            //    - either none or all of the keywords in each match set must be present in the vlist
            //    - all keywords in each match set must be occur the same number of times in the vlist
            //

            if (plist.matches.size() > 0)   
            {
                // outer loop to look at each match set in list 

                 for (const auto& match_set : plist.matches)
                 {
                     std::wstring               found_str       {   };         // printable list of matching keywords found in this set (for error message)
                     std::wstring               required_str    {   };         // printable list of keywords required in match set  
                     std::vector<uint64_t>      kw_counts       {   };         // list of actual keyword occurrence counts in vlist 
                     std::vector<std::wstring>  kw_names        {   };         // names of keywords found in vlist 
                     uint64_t                   found_ct        { 0 };         // number of keywords in match set that are present in vlist
                     uint64_t                   required_ct     { 0 };         // total number of keywords present in match set


                    // inner loop to check each keyword in the match set (compute required_ct = number of kws in match set, found_ct = number of these KWs in vlist)

                    for (const auto& kw_name : match_set)
                    {
                        required_ct ++;                                        // increment number of keywords present
                        required_str += L" " + kw_name + L": ";                // add this keyword to the required string 
                                                                              
                        auto kw_count = vlist.eval_kws.count(kw_name);         // number of times this keyword is present in vlist 
                                                                              
                        if (kw_count > 0)                                      // is this keyword in vlist ?
                        {                                                     
                            found_ct++;                                        // increment count of keywords present
                            found_str += L" " + kw_name + L": ";               // add this keyword to the found string 
                            kw_names .push_back(kw_name);                      // save this kw name in vector 
                            kw_counts.push_back(kw_count);                     // save this kw count in vector 
                        }
                    }


                    // if at least one keyword in the match set was present, need to check counts, etc. (all keywords missing is OK, as far as this check goes)

                    if (found_ct > 0)
                    {
                        if (found_ct != required_ct)                           // see if any keywords in the match set are missing   
                        {
                            M__(M_out(L"check_verb_vlist() -- match kws -- some kws missing");)
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- %s keyword match set=(%s) -- only some of these keywords are present: (%s)") % verb_name(expression) % ws1 % ws2 % required_str % found_str;
                            msgend_loc(vlist, expression); 
                            rc = -1;  
                        }
                        else                                                   // right number of keywords (all) are present 
                        {
                            auto minmax_ct = std::minmax_element(kw_counts.cbegin(), kw_counts.cend());

                            M__(M_out(L"check_verb_vlist() -- *minmax_ct.first = %d,  *minmax_ct.second = %d") % *(minmax_ct.first) % *(minmax_ct.second); )

                            if ( *(minmax_ct.first) != *(minmax_ct.second) )  // do keywords appear a different number of times in the vlist?  
                            {
                                // build string with varying counts, for error message -- kw_counts and kw_names should be the same size == found_ct == required_count

                                std::wstring counts_str { }; 

                                for (auto i = 0; i < found_ct; i++)
                                    counts_str += L" " + kw_names.at(i) + L":" + std::to_wstring(kw_counts.at(i)) + L"x "; 
                                
                                M__(M_out(L"check_verb_vlist() -- match kws -- different occurrences");)
                                count_error();
                                M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- %s keyword match set=(%s) -- differing number of occurrences: (%s) -- number of occurrences must all be the same") % verb_name(expression) % ws1 % ws2 % required_str % counts_str;
                                msgend_loc(vlist, expression); 
                                rc = -1;  
                            }
                        }                        
                    }                            // at least one keyword in match set was present in vlist            
                }                                // loop to check each match set for this plist
            }                                    // keyword match sets present for this plist                                      
        }                                        // keywords allowed
    }                                            // no check keywords
          
    return rc; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   check_verb_parms() -- complain and return error, if parms in expression don't match what is expected in the verbdef_S structure
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// this function checks to see if they match those required by a specific verbdef_S 

static int check_verb_parms(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef) try
{
    int rc {0}; 


    // check both left and right vlists vs plist requirements in passed-in expression

    auto crc1 = check_verb_vlist(frame, expression, verbdef.lparms, expression.lparms, L"", L"left-side" );  
    auto crc2 = check_verb_vlist(frame, expression, verbdef.rparms, expression.rparms, L"", L"right-side");
    if (crc1 != 0) rc = crc1; 
    if (crc2 != 0) rc = crc2;


    // global check for parms_some_required -- at least one positional parm on one side or the other 

    if (verbdef.parms_some_required)
    {
        if ( (expression.lparms.value_ct == 0) && (expression.rparms.value_ct == 0) )     // both sides have no positional parms ?  
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects at least one positional parm on one side or the other, but none were present at all") % verb_name(expression); 
            msgend_loc(expression); 
            rc = -1;           
        }
    }


    // global check for parms_same_number -- number of positional parms on both left and right sides must be the same 

    if (verbdef.parms_some_required)
    {
        if (expression.lparms.value_ct != expression.rparms.value_ct)                   // both sides have same number of positional parms ?  
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects the same number of positional parms on each side --  left-side has %d, right-side has %d") % verb_name(expression) % expression.lparms.value_ct % expression.rparms.value_ct; 
            msgend_loc(expression); 
            rc = -1;           
        }
    }
    

    // global check for parms_not_both_sides -- cannot have positional or *keyword* parms on both sides 

    if (verbdef.parms_not_both_sides)
    {
        if ( ((expression.lparms.value_ct + expression.lparms.kw_ct) > 0) && ((expression.rparms.value_ct + expression.rparms.kw_ct) > 0) )     // both sides have positional or evaluated keyword parms ?  
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  does not expect positional or keyword parms on both left and right sides, but both left-side and right-side parms were present") % verb_name(expression); 
            msgend_loc(expression); 
            rc = -1;           
        }
    }


    // global check for parms_left_xor_right -- positional parms must exist on exactly one side (not both sides or neither side) 

    if (verbdef.parms_left_xor_right)
    {
        if ( (expression.lparms.value_ct == 0) && (expression.rparms.value_ct == 0) )     // both sides have no positional parms ?  
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects positional parms on one side or the other, but none were present on either side") % verb_name(expression); 
            msgend_loc(expression); 
            rc = -1;           
        }
        else if ( (expression.lparms.value_ct > 0) && (expression.rparms.value_ct > 0) )    // positional parms on both sides ?
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects positional parms on only one side or the other, but parms were present on both sides") % verb_name(expression);
            msgend_loc(expression); 
            rc = -1;           
        }    
    }

   
    // global check for parms_same_type - make sure all parms in both left-side and right_side positional parm lists are the same type 

    if (verbdef.parms_same_type)
    {
        if ( 
            (expression.rparms.val_mixed)
            ||
            (expression.lparms.val_mixed)
            ||
            (
             ( (expression.lparms.value_ct > 0) && (expression.rparms.value_ct > 0) ) 
             &&
             (
              (expression.lparms.val_unit           !=  expression.rparms.val_unit        )
              ||
              (expression.lparms.val_boolean        !=  expression.rparms.val_boolean     )
              ||
              (expression.lparms.val_int8           !=  expression.rparms.val_int8        )
              ||
              (expression.lparms.val_int16          !=  expression.rparms.val_int16       )
              ||
              (expression.lparms.val_int32          !=  expression.rparms.val_int32       )
              ||
              (expression.lparms.val_int64          !=  expression.rparms.val_int64       )
              ||
              (expression.lparms.val_uint8          !=  expression.rparms.val_uint8       )
              ||
              (expression.lparms.val_uint16         !=  expression.rparms.val_uint16      )
              ||
              (expression.lparms.val_uint32         !=  expression.rparms.val_uint32      )
              ||
              (expression.lparms.val_uint64         !=  expression.rparms.val_uint64      )
              ||
              (expression.lparms.val_float32        !=  expression.rparms.val_float32     )
              ||
              (expression.lparms.val_float64        !=  expression.rparms.val_float64     )
              ||
              (expression.lparms.val_string         !=  expression.rparms.val_string      )
              ||
              (expression.lparms.val_identifier     !=  expression.rparms.val_identifier  )
              ||
              (expression.lparms.val_vlist          !=  expression.rparms.val_vlist       )
              ||
              (expression.lparms.val_expression     !=  expression.rparms.val_expression  )
              ||
              (expression.lparms.val_block          !=  expression.rparms.val_block       )
              ||
              (expression.lparms.val_verbset        !=  expression.rparms.val_verbset     )
            )
           )
          )
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects all positional parms (both left-side and right-side) to be the same type, but more than one type was present") % verb_name(expression); 
            msgend_loc(expression); 
            rc = -1;  
        }
    }

    return rc;   
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

//╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║                                          FFFFFFFFFF        RRRRRRRRR             AA             MM      MM        EEEEEEEEEE
//║╳╳╳╳║                                          FFFFFFFFFF        RRRRRRRRRR           AAAA            MMM    MMM        EEEEEEEEEE
//║╳╳╳╳║                                          FF                RR      RR          AA  AA           MMMM  MMMM        EE        
//║╳╳╳╳║                                          FF                RR      RR         AA    AA          MM MMMM MM        EE        
//║╳╳╳╳║                                          FFFFFFFF          RRRRRRRRRR        AA      AA         MM  MM  MM        EEEEEEEE  
//║╳╳╳╳║                                          FFFFFFFF          RRRRRRRRR         AAAAAAAAAA         MM      MM        EEEEEEEE  
//║╳╳╳╳║                                          FF                RR   RR           AAAAAAAAAA         MM      MM        EE        
//║╳╳╳╳║                                          FF                RR    RR          AA      AA         MM      MM        EE        
//║╳╳╳╳║                                          FF                RR     RR         AA      AA         MM      MM        EEEEEEEEEE
//║╳╳╳╳║                                          FF                RR      RR        AA      AA         MM      MM        EEEEEEEEEE
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
////   frame_cmdline_parms() -- special set up of (main) stack frame with cmdline parms
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void frame_cmdline_parms(frame_S& frame, int argc, wchar_t *argv[]) try
{
    // note: passed-in stack frame has already been set up with flags, pointers, environment, global envoronment, etc. (basic setup)

    setup_global_environ();                           // initialize global environment before using it


    // add command line args to passed-in stack frame right-side positional parm vector (vlist will have no location data)
    // also add global constants with names ARG_C ARG_0 ARG_1 ARG_2 ARG_3 ...

    for (auto i = 0; i < argc; i++) 
    {
        value_S parm_val = string_val(argv[i]);
        add_positional_value( frame.rparms, parm_val );
        def_global_const( std::wstring { L"ARG_" } + std::to_wstring(i), parm_val );
    }
    
    def_global_const( std::wstring { L"ARG_C" }, int64_val(argc) );

    return; 
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   new_frame() -- allocate new stack frame, but don't anchor it anywhere 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::shared_ptr<frame_S> new_frame() try                                     // caller needs to assume responsibility for new frame_S being allocated 
{
    // allocate new frame_S, holding onto it with a local shared pointer 

    std::shared_ptr<frame_S> new_frame_sp { new frame_S { } };               // allocate new stack frame and anchor it locally  
    M__(M_out(L"new_frame() called 1 -- new_frame_sp.use_count() = %d") % new_frame_sp.use_count();)


    // do basic (persistent) setup -- dynamic stack chain pointers will be set up when this stack frame is activated and placed on the stack 
  
    new_frame_sp->self_wp            = new_frame_sp;                           // set weak self-ptr for use_count() debugging and later shared_ptr initialization 
    new_frame_sp->serial             = ++static_N::frame_serial;               // set unique serial number in new frame_S, for debugging   
    new_frame_sp->env.sernum         = -(new_frame_sp->serial);                // also save negative value in imbedded environ_S, for debugging
    new_frame_sp->local_sf_p         = new_frame_sp.get();                     // set pointer to stack frame with local environment to this stack frame -- environment has local variables for this stack frame (can be replaced later for same_scope: verbs)
    new_frame_sp->search_sf_p        = new_frame_sp.get();                     // set pointer to stack frame with local environment to this stack frame -- starting point for identifier search
    new_frame_sp->environ_p          = &(new_frame_sp->env);                   // set pointer to local environment -- can be replaced by ptr to shared environment by caller


    M__(M_out(L"new_frame() called 3 -- new_frame_sp.use_count() = %d") % new_frame_sp.use_count();)

    return new_frame_sp; 
}
M_endf







////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_new_frame() -- alocate new stack frame and add it to to top of stack 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


frame_S *add_new_frame() try
{
    // allocate new frame_S -- anchor it locally for now  

    std::shared_ptr<frame_S> new_frame_sp = new_frame();  
    M__(M_out(L"add_new_frame() called 1 -- new_frame_sp.use_count() = %d") % new_frame_sp.use_count();)


    // add this stack frame to the top of the stack 

    (void)add_frame(new_frame_sp);
    M__(M_out(L"add_new_frame() called 3 -- new_frame_sp.use_count() = %d") % new_frame_sp.use_count();)


    return new_frame_sp.get();
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_frame() -- add passed-in stack frame to to top of stack 
////                  (stack pointer chain will assume ownership of this stack frame)
//// 
////
////               sets:   
////         
////                 - parent and child SF pointers
////                 - main   SF ptr 
////
////               does not set: (caller must set)
//// 
////                 - shared ptr to upward scope
////                 - verb main SF pointer
////                 - persistent SF ptr
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

frame_S *add_frame(const std::shared_ptr<frame_S>& frame_sp) try
{
    // save away pointer to current newest stack frame (called "old frame" here) -- will go out of scope when this routine returns 

    std::shared_ptr<frame_S> old_frame_sp { static_N::newest_frame_sp };     // hold onto current newest stack frame until back chain shared ptr is set in new stack frems  


    // anchor new stack frame in stck frame queue head -- this will be thte owning pointer for the new stack frame  
   
    static_N::newest_frame_sp = frame_sp;                                   // (!!!!!!!!!! owning pointer !!!!!!!!!!!!!) this will hold onto new stack frame after add_frame() returns


    // fix up various pointers in new frame_S being added to top of stack -- no need to do anything fi adding main SF
    // ------------------------------------------------------------------
 
    if (old_frame_sp.use_count() > 0)                                       // are we adding 1st stack frame?
    {                                                      
        // stack was not empty -- adding regular/nested (not main) stack frame
            
        frame_sp->parent_sp    = old_frame_sp;                              // set parent pointer in new stack frame  (!!!!!!!!!!!!!!!!!!!! owning pointer !!!!!!!!!!!!!!!!!!!!!!) 
        old_frame_sp->child_p  = frame_sp.get();                            // set new frame_S as old frame's child   (non-owning pointer)
    }
 
    
    // maintain frame depth counter, etc.

    static_N::frame_depth++;                                                // maintain frame depth counter
    static_N::frame_max_depth     = std::max(static_N::frame_depth, static_N::frame_max_depth); 

    return frame_sp.get();                                                   // return pointer to newly-added stack frame
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   remove_frame() -- remove newest stack frame from top of stack (this will cause stack frame be freed up, unless caller is holding onto a shared pointer to it)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void remove_frame() try
{
    std::shared_ptr<frame_S> removed_frame_sp { static_N::newest_frame_sp   }  ;        // shared ptr to hold onto stack frame being removed
    std::shared_ptr<frame_S> kept_frame_sp    { removed_frame_sp->parent_sp }  ;        // shared ptr to hold onto stack frame being kept (2nd oldest)  -- may be empty ??????


    M__(M_out(L"remove_frame() -- called:  removed_frame_p = %p   kept_frame_p = %p") % removed_frame_sp.get() % kept_frame_sp.get();)

    // kept_frame_sp->verb_eval_ct += removed_frame_sp->verb_eval_ct;                   // add removed frame's verb count to kept's (statistical counters)

    // remove newest stack frame from top of stack  (leave any scoping pointers alone -- these may hold onto removed stack frame in case something points to it directly or indirectly)
    
    if (kept_frame_sp.use_count() > 0)                                                  // is there a kept stack frame (i.e. removing main stack frame this time)
        kept_frame_sp->child_p = nullptr;                                               // kept stack frame is now childless

    removed_frame_sp->parent_sp.reset();                                                // get rid of removed stack frame's parent pointer (kept frame will not go away, since this routine still holds a shared pointer for it)
    static_N::newest_frame_sp = kept_frame_sp;                                          // update newest stack frame pointer (this will hold onto newest stack (if any are left on stack) frame after this routine returns)
    static_N::frame_depth--;                                                            // maintain frame depth counter 

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


//╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║                                          BBBBBBBBB         LL               OOOOOOOO            CCCCCCCC         KK      KK
//║╳╳╳╳║                                          BBBBBBBBBB        LL              OOOOOOOOOO          CCCCCCCCCC        KK     KK  
//║╳╳╳╳║                                          BB      BB        LL              OO      OO          CC      CC        KK    KK  
//║╳╳╳╳║                                          BB      BB        LL              OO      OO          CC                KK   KK   
//║╳╳╳╳║                                          BBBBBBBBB         LL              OO      OO          CC                KKKKKK    
//║╳╳╳╳║                                          BBBBBBBBB         LL              OO      OO          CC                KKKKKK    
//║╳╳╳╳║                                          BB      BB        LL              OO      OO          CC                KK   KK   
//║╳╳╳╳║                                          BB      BB        LL              OO      OO          CC      CC        KK    KK  
//║╳╳╳╳║                                          BBBBBBBBBB        LLLLLLLLLL      OOOOOOOOOO          CCCCCCCCCC        KK     KK 
//║╳╳╳╳║                                          BBBBBBBBB         LLLLLLLLLL       OOOOOOOO            CCCCCCCC         KK      KK
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
////   eval_frame_block() -- add new frame_S to stack for this (non-verb) frameblock,  and run passed-in block with that frame_S using passed-in vlists as parms
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int eval_frame_block(frame_S& parent_frame, const vlist_S& left_vlist, const vlist_S& right_vlist, const block_S& block, results_S& results) try
{
    // add new stack frame to the active frame_S queue

    frame_S *new_frame_p   = add_new_frame();                                                

    new_frame_p->persist_sf_p   = new_frame_p->parent_sp->persist_sf_p;                     // copy parent frame's persistent SF pointer into new SF
    new_frame_p->verbmain_sf_p  = new_frame_p->parent_sp->verbmain_sf_p;                    // copy parent frame's verb main  SF pointer into new SF


    // upward scope for a frameblock frame_S is always starts at parent stack frame -- scoping is always dynall-type (both dynamic and lexical)

    new_frame_p->is_frameblock      = true;                                                 // indicate this stack frame was created by frameblock evaluation
#ifdef M_EXPOSE_SUPPORT
    new_frame_p->dynall_scope       = true;                                                 // frameblocks always have unrestricted dynamic (lexical, effectively) scope upwards to parent 
#endif
    new_frame_p->scope_sp           = std::shared_ptr<frame_S> { parent_frame.self_wp };    // initialize shared ptr from weak self-ptr in parent frame_S

    new_frame_p->env.is_frameblock  = true;                                                 // also flag embeded environment as belonging to a frameblock (not verb)


    // set up frameblock invocation parms in new stack frame
    
    new_frame_p->lparms =  left_vlist; 
    new_frame_p->rparms = right_vlist;


    // run frameblock's block under new stack frame -- remove frame_S when done

    results_S frameblock_results {};
    auto erc = eval_block(*new_frame_p, block, frameblock_results);
    remove_frame();                                                                         // remove new stack frame from stack 
    if (erc != 0)                                                                         
    {                                                                                     
        results = error_results();                                                          // results = error   
        return erc;                                                                         // return with error r/c
    }                                                                                     


    //  handle @QUIT special results -- percolate any others that get percolated here  

    if (frameblock_results.quit_flag)
    {    
        frameblock_results.return_flag     = false;                                         // consume the @QUIT         
        frameblock_results.special_results = false;           
        results                            = frameblock_results;                            // pass back @QUIT results -- special results are consumed
        return 0; 
    }
                          

    // pass back results (including special results other than @QUIT) from block evaluation

    results = frameblock_results; 
    return 0; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_verbinit_block() -- add new frame_S to stack for this verb-block,  and run passed-in block with that new stack frame, using vlists in expression as the verb-block parms
////                         
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// note: proper verbdef_S from the overload set has already been picked and passed to this funcion

int eval_verbinit_block(const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"eval_verbinit_block() -- called");)

    // ---------------------------------------------------------------------------------------------------------------------------
    //  add new SF pointing to the verbdef_S persistent environment to active frame_S queue, and setup upward scope pointers, etc.
    // ---------------------------------------------------------------------------------------------------------------------------
           
    frame_S *persist_frame_p          = add_new_frame();                                              
    persist_frame_p->environ_p        = verbdef.persist_env_sp.get();                                     // point new persistent SF to environ_S of verbdef_S's persistent environment
    persist_frame_p->is_persistent    = true;                                                             // mark new SF as persistent env
    persist_frame_p->persist_sf_p     = persist_frame_p;                                                  // persistenf SF ptr points to self   
    persist_frame_p->local_sf_p       = nullptr;                                                          // don't allow non-static variable definitions in the init: block                                                                                                      
                                                                                                          // no verb main SF pointer in init SF -- leave search_s_p pointing to persistent_sf
    // set up for dynamic scope to parent SF 

#ifdef M_EXPOSE_SUPPORT
    persist_frame_p->dynall_scope     = true;
#elseif
    persist_frame_p->dynamic_scope    = true;
#endif
                                          
    persist_frame_p->scope_sp = std::shared_ptr<frame_S> { persist_frame_p->parent_sp->self_wp };         // initialize scope ptr from weak self-ptr in parent frame_S -- upward scope starts at parent on call stack, for dynamic or dynall scoping 
 
    M__(M_out(L"eval_verbinit_block() -- SF setup done");)


    // -----------------------------------------------------------------------------------------------
    // run verb's init block in the new/shared stack frame(s) -- remove new frame_S (if any) when done
    // -----------------------------------------------------------------------------------------------

    M__(M_out(L"eval_verbinit_block() -- calling eval_block block use count = %d") % verbdef.init_block_sp.use_count() ;)
    results_S block_results {};
    auto erc = eval_block(*persist_frame_p, *(verbdef.init_block_sp), block_results);
 

    // remove the stack frame obtained earlier
    // ---------------------------------------

    remove_frame();                                                                                       // remove persistent SF from stack -- should go away, but persistent environment should stay around due to shared ptr in verbdef_S 
                                                                                                        
                                                                                                                
                                                                                                                
    // handle any errors from verb block evaluation                                                             
    // --------------------------------------------                                                             
                                                                                                                
    if (erc != 0)                                                                                               
    {                                                                                                           
        results = error_results();                                                                                 // results = error   
        return erc;                                                                                                // return with error r/c
    }


    // handle any special results -- expect @END, @THROW, @XCTL, @GOTO longjmp:, and @RETURN -- do not expect uncaught @LEAVE, @GOTO, @CONTINUE, @BREAK, @QUIT, etc.

    if (block_results.special_results)
    {
        if (block_results.return_flag)                                                                             // @RETURN is expected -- needs to be consumed here
        {                                                                                                          
             block_results.return_flag     = false;                                                                // consume the @RETURN         
             block_results.special_results = false;                                                                
             results = unit_results();                                                                             // ignore @RETURN value -- use substitute UNIT value      
        }                                                                                                       
        else if (block_results.end_flag)                                                                           // @END is expected
        {                                                                                                          
             results = block_results;                                                                              // percolate @END results           
        }  
        else if (block_results.throw_flag)                                                                         // @THROW is expected
        {                                                                                                          
             results = block_results;                                                                              // percolate @THROW results           
        }                                                                                                      
        else if (block_results.skip_flag)                                                                          // @SKIPTO is expected
        {                                                                                                          
             results = block_results;                                                                              // percolate @SKIPTO results           
        }    
        else if (block_results.lgoto_flag)                                                                         // @GOTO longjmp: is expected
        {                                                                                                          
             results = block_results;                                                                              // percolate @GOTO longjmp: results           
        } 
        else                                                                                                       // unexpected special results
        {              
            if (block_results.goto_flag)
                M_out_emsg(L"User-defined verb (\"%S\") init: block evaluation ended by unconsumed @GOTO «%S»")      % verbdef.info % block_results.str;    // ?????? long @GOTO not allowed ????? -- need to add config flag ????   
       
            else if (block_results.xctl_flag)
                M_out_emsg(L"User-defined verb (\"%S\") init: block evaluation ended by unconsumed @XCTL «%S»")      % verbdef.info % block_results.str;  

            else if (block_results.leave_flag)
                M_out_emsg(L"User-defined verb (\"%S\") init: block evaluation ended by unconsumed @LEAVE «%S»")     % verbdef.info % block_results.str;            
           
            else if (block_results.break_flag)
                M_out_emsg(L"User-defined verb (\"%S\") init: block evaluation ended by unconsumed @BREAK")          % verbdef.info; 
       
            else if (block_results.continue_flag)
                M_out_emsg(L"User-defined verb (\"%S\") init: block evaluation ended by unconsumed @CONTINUE")       % verbdef.info;
            
            else if (block_results.quit_flag)                                     
                M_out_emsg(L"User-defined verb (\"%S\") init: block evaluation ended by unconsumed @QUIT")           % verbdef.info;  
          
            else if (block_results.error)                                                   
                M_out_emsg(L"User-defined verb (\"%S\") init: block evaluation ended by error")                      % verbdef.info;

            else
                M_out_emsg(L"User-defined verb (\"%S\") init: block evaluation ended by unexpected special results") % verbdef.info;

            count_error(); 
            results = error_results();                                                                              // error results
            return -1;                                                                                              // return with error 
        }                                                                                                          
    }                                                                                                              
    else                                                                                                            // block not ended by @RETURN etc. - return value is not set 
    {                                                                                                              
        results = unit_results();                                                                                   // ignore normal block results -- pass back unit value instead 
    }                                                                                                              
                                                                                                                   
    return 0;                                                                                                       // return normally -- results have already been passed-back
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_verb_block() -- add new frame_S to stack for this verb-block,  and run passed-in block with that new stack frame, using vlists in expression as the verb-block parms
////                         
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  local set_parm_vars() helper function 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

static
int set_parm_vars(frame_S *frame_p, e_expression_S expression, const verbdef_S& verbdef, const argvar_S& argvar, const vlist_S vlist) try
{
    // ----------------------------------
    // process list of positional argvars   (right-side or left-side)
    // ----------------------------------

    //    note: unless sharing a SF with caller, *frame_p should be empty, and there should be no duplicates in the variable names list, so def_local_var() should not fail
    
    int argvar_n { 0 };                                                                                            // parameter number     
                                                                                                                 
    def_parm_S def_parm {   };                                                                                     // local parm for def_parms_var()                                                                                         
    def_parm.unshare = true;                                                                                       // make sure parm values are unshared
                                                                                                                 
    for (auto elem : argvar.pos)                                                                                   // elem is a std::wstring = arg variable name 
    {                                                                                                                 
        int drc { 0 };                                                                                             // return code from def_local_var()  
         
        // see if weak alias processing or regular parm passing is required for this argvar

        if ( verbdef.by_alias_ok && (elem.front() == L'_') )                                                       // by_alias: and this argvar starts with underscore char?
        {
            // get parm using weak alias -- error if this parm is not present in vlist, or is not an identifier
            // ------------------------------------------------------------------------------------------------

            M__(M_out(L"set_parm_vars() -- get parm %S by_alias:") % elem;)


            // error, if not enough values in passed-in vlist to create alias for this argvar

            if (argvar_n >= vlist.value_ct)
            {
                count_error(); 
                M_out_emsg1(L"set_parm_vars() -- only %d parms passsed in -- cannot make weak alias for argvar %d = \"%S\"") % vlist.value_ct % argvar_n % elem; 
                msgend_loc(expression); 
                return -1;           
            }  


            // error, if not matching parm was not passed in as an identifier that can be aliased

            if (vlist.values.at(argvar_n).ty != type_E::identifier)
            {
                count_error(); 
                M_out_emsg1(L"set_parm_vars() -- passed-in parm %d is not an identifier -- cannot make weak alias for argvar = \"%S\"") % argvar_n % elem; 
                msgend_loc(expression); 
                return -1;           
            } 


            // create weak alias in parms frame using n-th argvar and passed-in identifier 

            drc = def_parms_weak_alias(*frame_p, vlist.values.at(argvar_n).string, elem, def_parm_S { });


            // return immediately with error, if def_parms_weak_alias() failed                                              
                                                                                                                     
            if (drc != 0)                                                                                              // did def_local_var() fail? -- not expected
            {
                count_error(); 
                M_out_emsg1(L"set_parm_vars() -- when setting positional argvar %d = \"%S\" as a weak alias -- error from def_parms_weak_alias()") % argvar_n % elem; 
                msgend_loc(expression); 
                return -1;                                                                                             // r/c     = error
            }
        }
        else
        {
            // regular parm passing -- set local variable to value passed in this parm
            // -----------------------------------------------------------------------         
         
            if (argvar_n >= vlist.value_ct)                                                                            // this variable is past end of parms for this invocation
                drc = def_parms_var(*frame_p, elem, unit_val(),                def_parm);                              // define parm variable with unit value -- make sure values are unshared
            else                                                                                                     
                drc = def_parms_var(*frame_p, elem, vlist.values.at(argvar_n), def_parm);                              // define parm variable with value of passed-in n-th positional parm -- make sure values are unshared 
                                                                                                                     
                                                                                                                     
            // return immediately with error, if def_parms_var() failed                                              
                                                                                                                     
            if (drc != 0)                                                                                              // did def_local_var() fail? -- not expected
            {
                count_error(); 
                M_out_emsg1(L"set_parm_vars() -- when setting positional argvar %d = \"%S\" -- error from def_local_var()") % argvar_n % elem; 
                msgend_loc(expression); 
                return -1;                                                                                             // r/c     = error
            }                                                                                                       
        } 


        // continue loop to process next argvar

        argvar_n++;                                                                                                // advance to next value in positional argvar list 
    }

     
    // -------------------------------
    // process list of keyword argvars     (right-side or left-side)
    // -------------------------------

    std::multiset<std::wstring>  lkw_names { };                                                                    // multimap with keywords processed so far  
                                                                                                                 
    for (auto elem : argvar.key)                                                                                   // elem.first is a std::wstring = keyword name -- elem.second is a std::wstring = variable name 
    {                                                                                                             
        lkw_names.insert(elem.first);                                                                              // add this keyword name to combined list of keywords seen so far
        auto kwvar_ct = lkw_names.count(elem.first);                                                               // get count of how many times this keyword name has been seen so far in argvar_S
        auto kwval_ct = vlist.eval_kws.count(elem.first);                                                          // get count of how many times this keyword name is seen in the invocation parms 

        M__(M_out(L"set_parm_vars() -- left-side : kw_name = «%S»  kwvar_ct=%d  kwval_ct=%d") % elem.first % kwvar_ct % kwval_ct;) 
  
        int drc { };                                                                                               // return code from def_local_var
      

        // see if weak alias processing or regular parm passing is required for this argvar

        if ( verbdef.by_alias_ok && (elem.second.front() == L'_') )                                                // by_alias: and this argvar starts with underscore char?
        {
            // get parm using weak alias -- error if this parm is not present in vlist, or is not an identifier
            // ------------------------------------------------------------------------------------------------

            M__(M_out(L"set_parm_vars() -- get parm %S by_alias:") % elem.second;)


            // error, if not enough values in passed-in vlist to create alias for this argvar

            if (kwvar_ct > kwval_ct)
            {
                count_error(); 
                M_out_emsg1(L"set_parm_vars() -- only %d parms passsed in -- cannot make weak alias for argvar %d = \"%S\"") % kwval_ct % kwvar_ct % elem.second; 
                msgend_loc(expression); 
                return -1;           
            }  


            // error, if not matching parm was not passed in as an identifier that can be aliased

            if (multimap_at(vlist.eval_kws, elem.first, kwvar_ct-1).ty != type_E::identifier)
            {
                count_error(); 
                M_out_emsg1(L"set_parm_vars() -- passed-in parm %d is not an identifier -- cannot make weak alias for argvar = \"%S\"") % (kwvar_ct - 1) % elem.second; 
                msgend_loc(expression); 
                return -1;           
            } 


            // create weak alias in parms frame using n-th argvar and passed-in identifier 

            drc = def_parms_weak_alias(*frame_p, multimap_at(vlist.eval_kws, elem.first, kwvar_ct-1).string, elem.second, def_parm_S { });


            // return immediately with error, if def_parms_weak_alias() failed                                              
                                                                                                                     
            if (drc != 0)                                                                                              // did def_local_var() fail? -- not expected
            {
                count_error(); 
                M_out_emsg1(L"set_parm_vars() -- when setting positional argvar %d = \"%S\" as a weak alias -- error from def_parms_weak_alias()") % (kwvar_ct - 1) % elem.second; 
                msgend_loc(expression); 
                return -1;                                                                                             // r/c     = error
            }
        }
        else
        { 
            // regular parm passing -- set local variable to value passed in this parm
            // -----------------------------------------------------------------------  

            if (kwvar_ct > kwval_ct)                                                                                   // this kw/variable pair is past end of kw/value pairs in invocation args
                drc = def_parms_var(*frame_p, elem.second, unit_val(), def_parm);                                      // define local parm variable with unit value -- make sure values are unshared
            else                                                                                                   
                drc = def_parms_var(*frame_p                                                                  
                                   , elem.second                                                                   
                                   , multimap_at(vlist.eval_kws, elem.first, kwvar_ct-1)                               // find value for n-th occurrence of this keyword in invocation keyword values multimap
                                   , def_parm                                                                          
                                   );                                                                                  // define local non-exposed variable with value of passed-in n-th positional arg -- make sure values are unshared
                                                                                                                   
                                                                                                                   
            // return immediately with error, if def_local_var() failed                                            
                                                                                                                   
            if (drc != 0)                                                                                              // did def_local_var() fail? -- not expected
            {
                count_error(); 
                M_out_emsg1(L"set_parm_vars() -- keyword(%S) argvar -- error from def_local_var(,%S,)") % elem.first % elem.second; 
                msgend_loc(expression); 
                return -1;                                                                                             // r/c     = error
            } 
        }
    } 
     
    // if no errors found, return normally

    return 0; 
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  main eval_verb_block() function 
////////////////////////////////////////////////////////////////////////////////////////////////////////////


// note: proper verbdef_S from the overload set has already been picked and passed to this funcion

int eval_verb_block(frame_S& parent_frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"eval_verb_block() -- called");)

    frame_S *eval_frame_p { nullptr };  


    // ==================================================================================================================================================
    // obtain new SF(s) or reuse current SF, depending on same_scope: option on the verb definition 
    // ==================================================================================================================================================

  
    // figure out upward scope for this verb 
    // -------------------------------------

    std::shared_ptr<frame_S> scope_sp { };                                                                    // temporary scope shared pointer to be copied into persistent SF (if any) or main verb SF                                                                                           

    if (verbdef.lexical_scope)
    {
        // upward scope pointer for new stack frame is the captured upward scope when verb was defined (if it is still around -- close: option should keep this SF around for life of verb definition) 
       
        if (verbdef.defining_scope_wp.expired())                                                              // enclosing SF has gone away? 
        {                                                                                                    
            if (verbdef.scope_defaulted)                                                                      // lexical_scope: not coded on verb definition -- use no_scope:, since enclosing scope has gone away
            {                                                                                                
                scope_sp.reset();                                                                             // no upward scope for this stack frame (verb must not rely on unbound variables)  
            }                                                                                                
            else                                                                                              // error -- explicit lexical_scope and enclosing scope has disappeared (close: must not have been specified) 
            {
                count_error(); 
                M_out_emsg1(L"eval_verb_block() -- the enclosing scope from the time of verb definition no longer exists -- cannot invoke the lexically-scoped verb"); 
                msgend_loc(expression); 
                results = error_results();                                                                    // results = error
                return -1;                                                                                    // r/c     = error
            }                                                                                               
        }                                                                                                   
        else                                                                                                  // enclosing SF still around -- set as upward scope 
        {                                                                                                   
            scope_sp = std::shared_ptr<frame_S> { verbdef.defining_scope_wp };                                // start upward scope at stack frame that defined the verb, for lexical scoping
        }
    }
    else if (verbdef.no_scope)                                                                                // verbdef has no_scope: option
    {
        scope_sp.reset();                                                                                     // no upward scope for this stack frame (verb must not rely on unbound variables)    
    }                                                                                                     
    else                                                                                                      // must be   shared_scope: ,  dynamic_scope:   -or-   dynall_scope: option on verbdef
    {                                                                                                     
        scope_sp = std::shared_ptr<frame_S> { parent_frame.self_wp };                                         // initialize shared ptr from weak self-ptr in parent frame_S -- upward scope starts at parent on call stack, for dynamic or dynall scoping 
    }

    
    // see if this verbdef has a persistent environment (note: init:{} causes persistent env -- not allowed with shared_scope: )
    // ------------------------------------------------

    frame_S *persist_frame_p { nullptr };                                                                     // nullptr will be replaced by address of new persistent SF (if any)  

    if (verbdef.persist_env_sp.get() != nullptr)
    {
        M__(M_out(L"eval_verb_block() -- has persistent environment");)

        // ---------------------------------------------------------------------------------------------------------------------------
        //  add new SF pointing to the verbdef_S persistent environment to active frame_S queue, and setup upward scope pointers, etc.
        // ---------------------------------------------------------------------------------------------------------------------------
       
        persist_frame_p                   = add_new_frame();                                                  
        persist_frame_p->environ_p        = verbdef.persist_env_sp.get();                                     // point new persistent SF to environ_S of verbdef_S's persistent environment
        persist_frame_p->is_persistent    = true;                                                             // mark new SF as persistent env
        persist_frame_p->persist_sf_p     = persist_frame_p;                                                  // persistenf SF ptr points to self   
  
  
        // copy over scope flags from verbdef_S into persistent SF frame_S, and save away scope shared ptr as computed earlier (note: same_scope verbs should not have persistent SF)   
       
        persist_frame_p->dynamic_scope    = verbdef.dynamic_scope;
        persist_frame_p->lexical_scope    = verbdef.lexical_scope;
#ifdef M_EXPOSE_SUPPORT
        persist_frame_p->dynall_scope     = verbdef.dynall_scope;
#endif
        persist_frame_p->no_scope         = verbdef.no_scope;
       
        persist_frame_p->scope_sp         = scope_sp;                                                         // set up scope in persistent SF based on verbdef scope: option                                                  


        // replace scope_sp computed above with a shared ptr to the persistenf SF -- this will be the upward scope for the 2nd SF for this verb           

        std::shared_ptr<frame_S> temp_sp {persist_frame_p->self_wp};                                          // need to set local shared_ptr from weak self ptr (will soon go out of scope)
        scope_sp = temp_sp;                                                                                   // upward scope SF for main verb SF is this persistent SF (std::shared_ptr)
    }
  
    M__(M_out(L"eval_verb_block() -- setting up verb main SF");)

    // ------------------------------------------------------------------------------------------------------------------------
    // obtain and setup 2nd or only (main) SF for this verb invocation -- chain it after the persistent SF (if any) just set up
    // ------------------------------------------------------------------------------------------------------------------------
  
    // add a new stack frame to the active frame_S queue after the persistent SF (if any) just added (and do basic setup)
  
    frame_S *new_frame_p = add_new_frame();                                                                   
                                                                                                             
                                                                                                             
    // if persistent SF was obtained always mark 2nd SF as dynall scope, pointing upward to persistent SF (note same_scoep: not allowed with persistent SF)                                

    if (persist_frame_p != nullptr)
    {
        M__(M_out(L"eval_verb_block() -- verb main SF -- have persistent SF");)
#ifdef M_EXPOSE_SUPPORT
        new_frame_p->dynall_scope  = true; 
#else
        new_frame_p->dynamic_scope = true; 
#endif
        new_frame_p->persist_sf_p      = persist_frame_p;                                                     // set persistent SF pointer in dynamic SF to persistent SF for this verb
        persist_frame_p->verbmain_sf_p = new_frame_p;                                                         // also set verb main SF pointer in persistent SF to the new verb main SF  
    }                                                                                                     
    else                                                                                                      // no persistent SF was set up
    {   
        M__(M_out(L"eval_verb_block() -- verb main SF -- have no persistent SF");)

        // set verb main SF scope flags from verbdef flags, as usual                                      
                                                                                                          
        new_frame_p->dynamic_scope    = verbdef.dynamic_scope;                                        
        new_frame_p->lexical_scope    = verbdef.lexical_scope;                                        
#ifdef M_EXPOSE_SUPPORT                                                                                       
        new_frame_p->dynall_scope     = verbdef.dynall_scope;                                         
#endif                                                                                                        
        new_frame_p->no_scope         = verbdef.no_scope;   
        new_frame_p->same_scope       = verbdef.same_scope;  
    }                                                                                                     
                                                                                                          
    M__(M_out(L"eval_verb_block() -- verb main SF -- basic setup");) 


    //  do basic setup that doesn't depend on presence/absence of persistent SF  -- note that add_new_frame() has already set up the local_sf_p and search_sf_p pointers in the new SF
    //  -----------------------------------------------------------------------
                                                                                                          
    new_frame_p->scope_sp = scope_sp;                                                                         // scope sp = computed scope for this verb, -or- ptr to persistent SF 

    if (new_frame_p->same_scope)                                                                              // is new non-persistent SF for same_scope: verb (parms-only SF)?                                                                                
    {                                                                                                           
        new_frame_p->is_parmsonly       = true;                                                               // flag new SF as parms-only SF
        new_frame_p->env.is_parmsonly   = true;                                                               // also flag the imbedded environment as parms-only
        new_frame_p->verbmain_sf_p      = parent_frame.verbmain_sf_p;                                         // set verbmain   SF pointer from parent SF  
        new_frame_p->persist_sf_p       = parent_frame.persist_sf_p;                                          // set persistent SF pointer from parent SF  
        new_frame_p->local_sf_p         = parent_frame.local_sf_p;                                            // set local      SF pointer from parent SF 
                                                                                                              //     search     SF pointer stays pointing to the new SF
    }
    else                                                                                                      // not setting up parms-only SF for this verb (i.e. same_scope:)
    {
        new_frame_p->is_verbmain     = true;                                                                  // flag this frame_S as started by verb 
        new_frame_p->env.is_verbmain = true;                                                                  // flag imbedded environment as belonging to verb 
        new_frame_p->verbmain_sf_p   = new_frame_p;                                                           // set verb main  SF pointer in dynamic SF to self 
                                                                                                              // persistent SF ptr was set above, local SF and search SF pointers stay the same -- set by add_new_frame()
    }
    
    M__(M_out(L"eval_verb_block() -- SF setup done");)
            

    // --------------------------------------------------------                                                         
    // set up vlists and argvars (if any) in evaluation frame_S                                                                
    // --------------------------------------------------------     

    // set up verbname and left/right vlists in new stack frame

    new_frame_p->verbname     = expression.verb_name;    
    new_frame_p->lparms       = expression.lparms;                                                                                  
    new_frame_p->rparms       = expression.rparms;    
    
                                                                                           
                                                                                                             
    // process left-side vlist and arg-list
    // ------------------------------------
    
    auto src = set_parm_vars(new_frame_p, expression, verbdef, verbdef.lvars, expression.lparms);                  // do left side vlist and argvar list

                                                                                                  
    // return immediately with error, if set_parm_vars() failed                                              
                                                                                                                 
    if (src != 0)                                                                                                  // did def_local_var() fail? -- not expected
    {
        count_error(); 
        M_out_emsg1(L"eval_verb_block() -- set_parm_vars() failed for left side vlist/argvar list -- verb will not be executed"); 
        msgend_loc(expression); 
        results = error_results();                                                                                 // results = error
        return -1;                                                                                                 // r/c     = error
    }                                                                                                       


    // process right-side vlist and arg-list
    // -------------------------------------
    
    src = set_parm_vars(new_frame_p, expression, verbdef, verbdef.rvars, expression.rparms);                       // do right side vlist and argvar list

                                                                                                  
    // return immediately with error, if set_parm_vars() failed                                              
                                                                                                                 
    if (src != 0)                                                                                                  // did def_local_var() fail? -- not expected
    {
        count_error(); 
        M_out_emsg1(L"eval_verb_block() -- set_parm_vars() failed for right side vlist/argvar list -- verb will not be executed"); 
        msgend_loc(expression); 
        results = error_results();                                                                                 // results = error
        return -1;                                                                                                 // r/c     = error
    } 
    

    // ------------------------------------------------------------------------------------------
    // run verb's block in the new/shared stack frame(s) -- remove new frame_S (if any) when done
    // ------------------------------------------------------------------------------------------

    results_S block_results {};
    auto erc = eval_block(*new_frame_p, *(verbdef.verb_block_sp), block_results);
 

    // if not sharing caller's stack frame(s), remove the stack frames obtained earlier
    // --------------------------------------------------------------------------------


    remove_frame();                                                                                                // remove new dynamic    frame from stack -- should go away 

    if (verbdef.persist_env_sp.get() != nullptr)                                                                   // does this verb have a persistent environmnent and persistend SF? 
        remove_frame();                                                                                            // remove new persistent frame from stack -- should stay around (because of shared ptr to it in the verbdef) 
                                                                                                         
                                                                                                                
                                                                                                                
    // handle any errors from verb block evaluation                                                             
    // --------------------------------------------                                                             
                                                                                                                
    if (erc != 0)                                                                                               
    {                                                                                                           
        results = error_results();                                                                                 // results = error   
        return erc;                                                                                                // return with error r/c
    }


    // handle any special results -- expect @END, @THROW, @XCTL, @GOTO longjmp:, and @RETURN -- do not expect uncaught @LEAVE, @GOTO, @CONTINUE, @BREAK, @QUIT, etc. (unless percolating everything)
    // -------------------------------------------------------------------------------------------------------------------------------------------------------------

    if (block_results.special_results)
    {
        if (verbdef.percolate_all)                                                                                 // is this (same_scope:) verb percolating everything ?  
        {
            results = block_results;                                                                               // don't consume anything (especially @RETURN) -- just percolate 
        }


        // not percolating everything -- do normal verb special results handling (especially @RETURN)
  
        else if (block_results.return_flag)                                                                        // @RETURN is expected -- needs to be consumed 
        {                                                                                                          
            block_results.return_flag     = false;                                                                 // consume the @RETURN         
            block_results.special_results = false;                                                                
            results = block_results;                                                                               // pass back value from @RETURN, with no special flags        
        }                                                                                                       
        else if (block_results.xctl_flag)                                                                          // @XCTL is expected -- needs to be consumed as this verb returns, but not right here 
        {   
            M__(M_out(L"eval_verb_block() -- saw @XCTL results -- verbdef.is_builtin = %d") % ((int)(verbdef.is_builtin));)
            results = block_results;                                                                               // percolate @XCTL results    
        }                                                                                                      
        else if (block_results.end_flag)                                                                           // @END is expected
        {                                                                                                          
            results = block_results;                                                                               // percolate @END results           
        } 
        else if (block_results.throw_flag)                                                                         // @THROW is expected
        {                                                                                                          
            results = block_results;                                                                               // percolate @THROW results           
        }                                                                                                      
        else if (block_results.skip_flag)                                                                          // @SKIPTO is expected
        {                                                                                                          
            results = block_results;                                                                               // percolate @SKIPTO results           
        }   
        else if (block_results.lgoto_flag)                                                                         // @GOTO longjmp: is expected
        {                                                                                                          
            results = block_results;                                                                               // percolate @GOTO longjmp: results           
        }     
        else                                                                                                       // unexpected special results
        {              
            if (block_results.goto_flag)
                M_out_emsg(L"User-defined verb (\"%S\") evaluation ended by unconsumed @GOTO «%s»") % verbdef.info % block_results.str;    // ?????? long @GOTO not allowed ????? -or- longjump: option 
       
            else if (block_results.leave_flag)
                M_out_emsg(L"User-defined verb (\"%S\") evaluation ended by unconsumed @LEAVE «%s»") % verbdef.info % block_results.str;            
                      
            else if (block_results.quit_flag)                                     
                M_out_emsg(L"User-defined verb (\"%S\") evaluation ended by unconsumed @QUIT") % verbdef.info;  
          
            else if (block_results.break_flag)
                M_out_emsg(L"User-defined verb (\"%S\") evaluation ended by unconsumed @BREAK") % verbdef.info; 
       
            else if (block_results.continue_flag)
                M_out_emsg(L"User-defined verb (\"%S\") evaluation ended by unconsumed @CONTINUE") % verbdef.info;

            else if (block_results.error)                                                   
                M_out_emsg(L"User-defined verb (\"%S\") evaluation ended by error") % verbdef.info;

            else
                M_out_emsg(L"User-defined verb (\"%S\") evaluation ended by unexpected special results") % verbdef.info;

            count_error(); 
            results = error_results();                                                                              // error results
            return -1;                                                                                              // return with error 
        }                                                                                                          
    }                                                                                                              
    else                                                                                                            // block not ended by @RETURN etc. - return value is not set 
    {                                                                                                              
        results = block_results;                                                                                    // pass back normal block results
    }                                                                                                              
                                                                                                                   
    return 0;                                                                                                       // return normally -- results have already been passed-back
}
M_endf





////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_main_block() -- using pre-built main frame_S, run passed-in block (parms already set up for main frame_S) -- no results passed back, just r/c
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int eval_main_block(frame_S& frame, const block_S& block, int& wmain_rc) try
{
    // run block prebuilt main frame_S 

    results_S mainblock_results {};
    auto erc = eval_block(frame, block, mainblock_results);
    if (erc != 0)
    {
        wmain_rc = -1;
        return erc; 
    }


    // handle any percolated special results from normal block evaluation of main block

    if (mainblock_results.special_results)
    {
        if (mainblock_results.end_flag)                                                 // @END flag is supposed to be percolated here for action
        {
           wmain_rc = mainblock_results.int32;                                          // return code may have been set by "@END wmain_rc"  verb
           M__(M_out(L"eval_main_block() returning -- wmain_rc = %d") % wmain_rc;) 
           return 0; 
        }  


        // mainblock expects no unmatched @GOTOs, @LEAVEs, @CONTINUEs, @BREAKs, @RETURNs, @THROWs, @XCTLs, @QUITs, etc. -- (only @END is expected here) 
       
        if (mainblock_results.goto_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @GOTO \"%S\"  -- apparently  @GOTO  target label was not found") % mainblock_results.str; 
            count_error(); 
            wmain_rc = -1; 
            return -1;
        }
       
        if (mainblock_results.lgoto_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @GOTO \"%S\" longjmp:  -- apparently  @GOTO longjmp:  target label was not found") % mainblock_results.str; 
            count_error(); 
            wmain_rc = -1; 
            return -1;
        }

        if (mainblock_results.return_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @RETURN %S -- apparently @RETURN was issued in main block -- use @END instead") % str_value(mainblock_results); 
            count_error();
            wmain_rc = -1;
            return -1;
        }

        if (mainblock_results.xctl_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @XCTL -- apparently @XCTL was issued in main block"); 
            count_error();
            wmain_rc = -1;
            return -1;
        }   

        if (mainblock_results.quit_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @QUIT -- apparently @QUIT was issued outside of any block passed to the @BLOCK verb -- use @END to end the main block"); 
            count_error();
            wmain_rc = -1;
            return -1;
        }   

        if (mainblock_results.throw_flag)
        {
            M_out_emsg(L"main block evaluation ended by uncaught @THROW %S") % str_value(mainblock_results); 
            count_error();
            wmain_rc = -1;
            return -1;
        }
         
        if (mainblock_results.skip_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @SKIPTO \"%S\"") % str_results_string(mainblock_results); 
            count_error();
            wmain_rc = -1;
            return -1;
        }                     

        if (mainblock_results.leave_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @LEAVE \"%S\"  -- apparently @LEAVE was issued outside of any block") % str_results_string(mainblock_results); 
            count_error();
            wmain_rc = -1;
            return -1;
        }
         
        if (mainblock_results.break_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @BREAK -- apparently @BREAK was issued outside of any loop");
            count_error();
            wmain_rc = -1;
            return -1;
        }
       
        if (mainblock_results.continue_flag)
        {
            M_out_emsg(L"main block evaluation ended by unconsumed @CONTINUE -- apparently @CONTINUE was issued outside of any loop"); 
            count_error();
            wmain_rc = -1;
            return -1;
        }
       
        if (mainblock_results.error)
        {
            M_out_emsg(L"main block evaluation ended by error");
            count_error();
            wmain_rc = -1;
            return -1;
        }

        M_out_emsg(L"main block evaluation ended by unexpected special results"); 
        count_error();
        wmain_rc = -1;
        return -1; 
    }
    
    wmain_rc = 0;                                  // normal wmain_rc() -- nothing unusual
    return 0;                                      // main block evaluation results are ignored
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





//╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║                                          EEEEEEEEEE        VV      VV            AA             LL            
//║╳╳╳╳║                                          EEEEEEEEEE        VV      VV           AAAA            LL        
//║╳╳╳╳║                                          EE                VV      VV          AA  AA           LL        
//║╳╳╳╳║                                          EE                VV      VV         AA    AA          LL        
//║╳╳╳╳║                                          EEEEEEEE           VV    VV         AA      AA         LL        
//║╳╳╳╳║                                          EEEEEEEE           VV    VV         AAAAAAAAAA         LL        
//║╳╳╳╳║                                          EE                  VV  VV          AAAAAAAAAA         LL        
//║╳╳╳╳║                                          EE                  VV  VV          AA      AA         LL        
//║╳╳╳╳║                                          EEEEEEEEEE           VVVV           AA      AA         LLLLLLLLLL
//║╳╳╳╳║                                          EEEEEEEEEE            VV            AA      AA         LLLLLLLLLL
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
////   eval_value() -- evaluate contents of an passed-in value -- value should be either identifier or expression
////                -- passed-in value parm is replaced by the value (which can be another expression or variable -- no double evaluation -- they are left as-is)
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// internal routine to call eval_value in a loop, as long as re_eval_expression_results flag is on
// -----------------------------------------------------------------------------------------------

static int reeval_value(frame_S& frame, const value_S& value, results_S& results, const parmtype_S& eval_parmtype, bool debug = false) try
{
    int       loop_count {0}; 
    int       erc        {0}; 
    value_S   in_value   {value}; 


    // loop to call eval_value() as required
    // -------------------------------------

    do 
    {
        erc = eval_value(frame, in_value, results, eval_parmtype, debug);
        if (erc != 0) return erc;                                       // return immediately, if error

        if (results.re_eval_expression_results)
            in_value = results;                                         // copy results over to input area for next call 


        loop_count++;
        if ( debug && (loop_count > 1) )
        {
            M__(M_out(L"reeval_value() -- loop count = %d") % loop_count;)
        }
    }
    while (results.re_eval_expression_results);

    return erc; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int eval_value(frame_S& frame, const value_S& value, results_S& results, const parmtype_S& eval_parmtype, bool debug) try
{
    int rc {0}; 
    M__(display_value(results, L"eval_value() input results");)

    if (debug)
    {
        M__(M_out( L"*************************************** eval_value() -- called  -- parmtype.no_eval_ident=%S   parmtype.no_eval_expression=%S   parmtype.no_eval_vlist=%S   parmtype.no_eval_ref=%S   parmtype.verbless=%S ") 
                 % M_bool_cstr(eval_parmtype.eval.no_eval_ident) % M_bool_cstr(eval_parmtype.eval.no_eval_expression) % M_bool_cstr(eval_parmtype.eval.no_eval_vlist) % M_bool_cstr(eval_parmtype.eval.no_eval_ref) % M_bool_cstr(eval_parmtype.eval.verbless);)
        M__(display_value(value, L"eval_value()");)
    }

    static_N::value_count++; 


    // get local copy of effective evaluation flags for this value -- set flags to suppress evaluation, if value is flagged as "suppress evaluation"
    // -----------------------------------------------------------

    arg_eval_S eval { eval_parmtype.eval };                                                                       // local copy of passed-in (constant) evaluation flags
    
    if (value.suppress_eval_once || value.suppress_eval)                                                          // set all no_eval_xxx flags, if suppressing evaluation of passed-in value this time
    {
        eval.no_eval_expression    = true;
        eval.no_eval_vlist         = true;
        eval.no_eval_ident         = true;
        eval.no_eval_ident         = true;    
    }  


    // evaluate nested expression, if required 
    // ---------------------------------------

    if ( (value.ty == type_E::expression) && (!eval.no_eval_expression) )
    {

        if (debug)
        {
            M__(M_out(L"*************************************** eval_value() -- evaluating nested expression");)
        }

        results_S expression_results { }; 

        auto erc = eval_expression(frame, *(value.expression_sp), expression_results, debug);      // pass expression attached to this value
        expression_results.token_ix1 = value.token_ix1;                                            // copy original location info into results
        expression_results.token_ix2 = value.token_ix2;   

        if (erc != 0)                                                                              // error during expression evaluation
        {                                                                                      
            results = expression_results;                                                          // pass back results from expression evaluation
            rc = erc;                                                                              // pass back r/c
        }                                                                                      
        else                                                                                       // no error during expression evaluation
        {
             if (debug)
             {
                 M__(M_out(L"*************************************** eval_value() -- expression_results.ty=%S -- returned from nested expression evaluation") % type_str(expression_results.ty);)
             }

             if (                                                                           
                (expression_results.ty == type_E::identifier)                                      // re-evaluate, if results is an identifier 
                || 
                (expression_results.ty == type_E::vlist)                                           // re-evaluate, if results is a vlist
                ||
                ( (expression_results.ty == type_E::ref) && (expression_results.ref_sp->auto_deref) )  // re-evaluate, if results is a reference, that should be automatically de-referenced                 
               ) 
            {
                results                            = expression_results;                           // return (preliminary) results from expression evaluation
                results.re_eval_expression_results = true;                                         // indicate that re-evaluation of these results is required 
            }                                                                                
            else                                                                                   // no further evaluation needed
            {                                                                                
                results                    = expression_results;                                   // return results from expression evaluation                 
            }   
        }
    }


    // evaluate nested vlist, if required -- need to allocate a new one and attach it to the value -- in nested vlists, expressions, and variables always evaluated (according to flags in default plist_S )
    // -------------------------------------------------------------------------------------------

    else if ( (value.ty == type_E::vlist) && (!eval.no_eval_vlist) )
    {  
        if (debug)
        {
            M__(M_out(L"*************************************** eval_value() -- evaluating nested vlist");)
        }
        
        vlist_S temp_vlist {};
        temp_vlist = *(value.vlist_sp);                                        // can't update vlist pointed to by passed-in value -- need a new copy for updating
        

        if (eval_parmtype.plist_sp.get() == nullptr )                          // nested plist not present? 
        {
            M__(M_out(L" eval_value() -- calling eval_vlist() -- evaluating nested vlist - 1");)
            auto erc = eval_vlist(frame, temp_vlist, results, plist_S {});     // use default plist_S structure for nested vlist
            if (erc != 0) rc = erc;
        }
        else
        {   
            M__(M_out(L" eval_value() -- calling eval_vlist() -- evaluating nested vlist - 2");)
            auto erc = eval_vlist(frame, temp_vlist, results, *(eval_parmtype.plist_sp));   // use nested plist from passed-in parmtype
            if (erc != 0) rc = erc;
        }
 
        if (rc == 0)                                                            // bypass vlist anchoring, if error occurred
        {
            M__(M_out(L"*************************************** eval_value() -- calling set_vlist_value()");)
            set_vlist_value(results, temp_vlist, true);                         // anchor new vlist in passed-in results
            M__(M_out(L"*************************************** eval_value() -- set_vlist_value() returned");)
        }

        results.token_ix1 = value.token_ix1;                                    // save original location info into passed-in results
        results.token_ix2 = value.token_ix2;
    }


    // evaluate identifier, if required -- complain if not defined
    // -----------------------------------------------------------

    else if ( (value.ty == type_E::identifier) && (!eval.no_eval_ident) )
    {
        if (debug)
        {
            M__(M_out(L"*************************************** eval_value() -- evaluating identifier");)
        }

        auto grc = get_identifier(frame, value.string, results);
        results.token_ix1 = value.token_ix1;                                    // save original location info into passed-in results
        results.token_ix2 = value.token_ix2;

        if (debug)
        {
            M__(display_value(value  , L"eval_value() -- input value");)
            M__(display_value(results, L"eval_value() -- evaluated identifier ");)
        }

        if (grc != 0)
        {
            count_error();
            M_out_emsg1(L"eval_value(): evaluating undefined identifier «%s» -- verb using this parameter will not be executed") % value.string; 
            msgend_loc(value);
            rc = grc;
        }
    }


    // do auto dereference, if required -- complain if any errors
    // ----------------------------------------------------------

    else if ( (value.ty == type_E::ref) && (value.ref_sp->auto_deref) && (!eval.no_eval_ref) )
    {
        if (debug)
        {
            M__(M_out(L"*************************************** eval_value() -- doing auto dereference");)
        }

        value_S out_val { };                                                    // output value_S from dereferenced value
        auto drc = dereference_value(out_val, value);
         
        if (debug)
        {
            M__(display_value(value  , L"eval_value() -- input reference value");)
            M__(display_value(out_val, L"eval_value() -- dereferenced value ");)
        }

        if (drc != 0)
        {
            count_error();
            M_out_emsg1(L"eval_value(): unexpected error while dereferencing value -- verb using this parameter will not be executed") % value.string; 
            msgend_loc(value);
            rc = drc;
        }

        results = to_results(out_val);
    }
    else      
    {
        // other values don't need to be evaluated -- int, float, string, block, unit, verbname etc. -- just copy value to output results
        // (control also gets here if evaluation is suppressed or evaluation flags call for no evalaution 

        if (debug)
        {
            M__(M_out(L"*************************************** eval_value() -- copying value to output results");)
        }
        results = to_results(value); 
    }


    // copy-over suppress evaluation flags into results, as appropriate   
    // ----------------------------------------------------------------

    if (value.suppress_eval)
    {
        results.suppress_eval      = true;                                 // "suppress_eval" stays on until explicitly turned off (this flag is probably already on from the copy of input value to results) 
    }
    else if (value.suppress_eval_once) 
    {    
        if (eval.verbless)
            results.suppress_eval_once = true;                             // "suppress eval once" is not reset when evaluating a verbless expression (this flag is probably already on from the copy of input value to results)
        else
            results.suppress_eval_once = false;                            // "suppress eval once" is reset after 1st use, unless this is verbless expression 
    }

    if (debug)
    {
        M__(M_out(L"********* eval_value() returning -- re-eval = %s <=========================") % M_bool_cstr(results.re_eval_expression_results);)
    }

    return rc; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_vlist() -- evaluate contents of an passed-in vlist (values in passed-in vlist are updated in-place)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//////////////////////// helper function

static parmtype_S get_keyword_parmtype(const plist_S& eval_plist, const std::wstring& keyname) try
{
    if (eval_plist.keywords.count(keyname) > 0)                   // see if kw-name (or "") is missing in plist_S
    {
        return eval_plist.keywords.at(keyname);                   // return located parmtype_S for keyword name
    }
    else                                                          // no parmtype_S for this keyword name
    {
        if (eval_plist.keywords.count(std::wstring{ L"" }) > 0)   // see if "" missing keyword parmtype_S is supplied in plist
            return eval_plist.keywords.at(std::wstring{ L"" });   // missing kw-name substitute parmtype_S supplied -- pass it back
        else
            return parmtype_S {};                                 // kw-name or "" not found -- use default evaluation parmtype_S
    }   
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////

int eval_vlist(frame_S& frame, vlist_S& vlist, results_S& results, const plist_S& eval_plist, bool debug) try
{
    bool updated { false };
    int  rc      { 0     }; 

    M__(M_out(L"=========================================> eval_vlist() called");)
    M__(display_vlist(vlist, L"passed-in to eval_vlist()");)

    if ( vlist.val_expression || vlist.val_vlist || vlist.val_identifier || vlist.val_ref )
    {
       M__(M_out(L"eval_vlist() -- evaluation is needed");)


       // build a temporary default parmtype_S with global evaluation flags -- used if no plist is completely empty (contains no parmtypes) for positional parms (this should be the case only when expression has no verb)  
                                                                  

       parmtype_S default_eval_parmtype {};                                               // default parmtype 

       default_eval_parmtype.eval         = eval_plist.eval;                              // copy in parmtype flags from passed-in plist_S
       default_eval_parmtype.anything_ok  = true;                                         // suppress parm type checking by default  


       // -------------------------------------------------------------------------------------------
       // loop to look at each positional parm and do evaluation (subject to verb's evaluation flags)
       // (note: if plist is empty (verbless expression), use default_parmtype just set up)
       //        if more values in vlist than in parmtypes in plist, use the last the last one for the additional values) 
       // -------------------------------------------------------------------------------------------

        for (int i = 0; i < vlist.values.size(); i++)
        {
            M__(M_out(L"eval_vlist() -- loop -- i = %d    vlist.values.size() = %d" ) % i % vlist.values.size();)

            results_S val_results { }; 

            if ( (vlist.values.at(i).ty == type_E::expression) || (vlist.values.at(i).ty == type_E::vlist) || (vlist.values.at(i).ty == type_E::identifier) || (vlist.values.at(i).ty == type_E::ref) )
            {                 
                if (eval_plist.values.empty())                                       // no plist_S available? 
                {
                    M__(M_out(L"--------------------------------------->eval_vlist() calling eval_value() -- positional -- default_parmype_S");) 
                    auto erc = eval_value(frame, vlist.values.at(i), val_results, default_eval_parmtype);
                    M__(M_out(L"--------------------------------------->eval_vlist() eval_value() returned");)
                    if (erc != 0)  rc = erc;                                         // remember any error     
                }
                else                                                                 // some plist_S are available in values vector -- use i-th parmtype_S or last one, if not enough)               
                {
                    M__(M_out(L"--------------------------------------->eval_vlist() calling eval_value() -- positional -- supplied parmtype_S");) 
                    auto erc = eval_value(frame, vlist.values.at(i), val_results, eval_plist.values.at(std::min(i, (int)(eval_plist.values.size() - 1))) );
                    M__(M_out(L"--------------------------------------->eval_vlist() eval_value() returned");)
                    if (erc != 0)  rc = erc;                                         // remember any error
                }                                                                   
            }                                                                       
            else                                                                     // not expression, vlist, or identifier that requires evaluation
            {                                                                       
                val_results = to_results(vlist.values.at(i));                        // just copy value from vlist directly into results -- no need for evaluation
            }                                                                       

            M__(M_out(L"eval_vlist() -- inside loop -- looking at results -- i = %d    vlist.values.size() = %d" ) % i % vlist.values.size();)


            // ------------------------------------------------------------------
            // look at results from evaluating this expression/value in the vlist 
            // ------------------------------------------------------------------  

            // return immediately, if any special flags on in results 

            if (val_results.special_results)
            {
                vlist.values.at(i) = val_results;                                      // update value in_place in vlist, from values imbedded in results
                results = val_results;                                                 // pass back results with special flags
                return rc;                                                             // return with any errors
            }                                                                         


            // handle usual (single) results from evaluation
            // ---------------------------------------------

            if (!val_results.multiple_results)
            {
                vlist.values.at(i) = val_results;                                       // update value in_place in vlist, from values imbedded in results
                M__(M_out(L"eval_vlist() -- (only) value.ty %S %S") % type_str(val_results.ty) % val_results.string;)

                if (val_results.re_eval_expression_results)
                    i--;                                                               // go back and re-evaluate results of expression value evaluation
            }                                                                      


            // handle multiple results  -- can be 0, 1, or more values in the multiple results vlist (or perhaps no vlist at all)
            // -------------------------------------------------------------------------------------

            else
            {
                M__(M_out(L"eval_vlist() -- multiple results --  before -- i = %d   vlist.values.size() = %d" ) % i % vlist.values.size();)
                M__(display_vlist(*(val_results.vlist_sp), L"eval_vlist() -- adding in multiple results");)

                auto     vlist_it   = vlist.values.begin() + i;                                                // get iterator pointing to current (i-th) element in the positional values vector (guaranteed to exist at this point)
                uint64_t n_results { 0 };                                                                      // number of positional value results -- set to 0 in case vlist_sp is not initialized    
                uint64_t n_keys    { 0 };                                                                      // number of keyword value    results -- set to 0 in case vlist_sp is not initialized    
                
                

                if ( (!val_results.no_results) && (val_results.vlist_sp.use_count() > 0) )                     // vlist_sp points to vlist (with multiple values)? 
                {
                    n_results = val_results.vlist_sp->values.size();                                           // number of positional results -- can be 0, 1, 2, ...

                    if (val_results.vlist_sp->kw_eval_done)                                                    // Keywords in results vlist have been evaluated ?
                        n_keys = val_results.vlist_sp->eval_kws.size();                                        // get number of keyword results from evaluated keywords multimap
                    else                                                                                       // Keywords in results have not been evaluated ?
                        n_keys = val_results.vlist_sp->keywords.size();                                        // get number of keyword results from unevaluated keywords vector 

                    M__(M_out(L"eval_vlist() -- n_results = %d   n_keys = %d" ) % n_results % n_keys;)
                }
                                                                                                              
                if (n_results <= 0)                                                                            // no results from expression evaluation?  
                {     
                    vlist.values.erase(vlist_it);                                                              // just remove expression from vlist (don't replace with anything)
                    i--;                                                                                       // reduce loop index, which will be incremented later to point to new (slid-down) element now in i-th vector position
                    vlist.value_ct--;                                                                          // also reduce number of values present in this vlist
                }                                                                                             
                else if (n_results == 1)                                                                       // just one value in multiple results vlist? -- should not occur?? 
                {                                                                                            
                    vlist.values.at(i) = val_results.vlist_sp->values.at(0);                                   // replace expression in i-th value with the first/only value from the expression evaluation results 
                    M__(M_out(L"eval_vlist() -- (multiple=one) value.ty %S %S") % type_str(val_results.vlist_sp->values.at(0).ty) % val_results.vlist_sp->values.at(0).string;)
                    i--;                                                                                       // multiple results may need to be re-evaluated, since eval_value() doesn't do it (since it doesn't have access to n+1-th parmtype) 
                }                                                                                            
                else                                                                                           // actually have multiple results from expression evaluation
                {                                                                                            
                    vlist.values.at(i) = val_results.vlist_sp->values.at(0);                                   // replace expression in i-th value with the first value from the expression evaluation results
                    M__(M_out(L"eval_vlist() -- (multiple=0) value.ty %S %S") % type_str(val_results.vlist_sp->values.at(0).ty) % val_results.vlist_sp->values.at(0).string;)                                                            

                    // inner loop to insert multiple results values into vlist being evaluated
                                       
                    for (auto ii = 1; ii < n_results; ii++)
                    {
                        vlist_it = vlist.values.insert(vlist_it + 1, val_results.vlist_sp->values.at(ii));     // insert n-th multiple value after one that was just set in vlist 
                        M__(M_out(L"eval_vlist() -- (multiple=%d) value.ty %S %S") % ii % type_str(val_results.vlist_sp->values.at(ii).ty) % val_results.vlist_sp->values.at(ii).string;)
                   //     i++;                                                                                 // adjust vlist index for outer loop to skip past values just inserted 
                        vlist.value_ct++;                                                                      // also increment number of values present in this vlist

                        M__(M_out(L"eval_vlist() -- multiple results --  during -- i = %d   vlist.values.size() = %d" ) % i % vlist.values.size();)
                    }
              
                    i--;                                                                                      // multiple results may need to be re-evaluated, since eval_value() doesn't do it (since it doesn't have access to n+1-th parmtype) 
                }


                // Append any keywords in the results to the unevaluated keyword list in this vlist -- kw evaluation has not started yet for this vlist, so these should be picked up down below
                // (these are keywords in the results from evaluating a positional parm)

                if (n_keys > 0)
                {
                    auto saved_kw_ct { vlist.kw_ct };                                                                                                // unevaluated kw count before any additions

                    if (val_results.vlist_sp->kw_eval_done)                                                                                          // if keywords are already evaluated, need to convert them into unevaluated format 
                        refresh_evaluated_vlist( *(val_results.vlist_sp) );                                                                          // convertback to unevaluated format
 
                    vlist.keywords.insert(vlist.keywords.end(), val_results.vlist_sp->keywords.cbegin(), val_results.vlist_sp->keywords.cend());     // apppend all unevaluated keyword names/values from v2 (if any) to output vlist

                    vlist.kw_ct = vlist.keywords.size();                                                                                             // update unevaluated kw count 
                    M__(M_out(L"eval_vlist() -- vlist unevaluated kw_ct:  before=%d  after=%d") % saved_kw_ct % vlist.kw_ct;)
                }

                M__(M_out(L"eval_vlist() -- multiple results --  after  -- i = %d  vlist.values.size() = %d" ) % i % vlist.values.size();)  
            } 
        }
      
        updated = true; 
     }
     else
     {
         M__(M_out(L"eval_vlist() -- evaluation not needed" );) 
     }


     // ----------------------------------------------------------------------------------------------------------------------------------
     // loop to evaluate (subject to verb's evaluation flags) each keyword in vlist keywords vector, and save results to eval_kws multimap
     // ----------------------------------------------------------------------------------------------------------------------------------

     if (vlist.keywords.size() > 0)
     {
         // set up special parmtype, used to evaluate any expression for the keyword name itself. 

         parmtype_S keyword_parmtype { };                                                   // eval_value() looks at only "no_eval" flags, which are off with default initialization
       //keyword_parmtype.eval = eval_plist.eval;                                           // copy in parmtype flag  from passed-in plist_S -- never suppress KW name expression evaluation ????????

         for (auto& elem : vlist.keywords)
         {
             value_S        keyword_value { };
             std::wstring   keyword_name  { };    
       

             // process simple keyword name or evaluate keyword name value, if required
             // -----------------------------------------------------------------------

             if (elem.name.ty == type_E::keyname)                                           // handle simple keyword name value in this keyword_S
             {
                 keyword_name = elem.name.string;                                           // keyword name is in string field   
             }                                                                            
             else                                                                           // keyword name expression requires evaluation
             { 
                 M__(M_out(L"eval_vlist() -- evaluating keyword name expression");)

                 // need to handle non-simple keyword name -- evaluate value and check for keyword type or string type that can be converted to keyword_name string

                 results_S  keyname_results  { };                                         // results from verbname value evaluation 

                 auto evrc = reeval_value(frame, elem.name, keyname_results, keyword_parmtype, debug);
                 if (evrc != 0) rc = evrc;

                 if (keyname_results.special_results)
                 {
                     results = keyname_results;                                           // pass back any unusual results from keyword name value evaluation
                     return rc;                       
                 }  

                 if (evrc != 0)
                 {                         
                     M_out_emsg1(L"eval_vlist() -- keyword name evaluation failed -- cannot continue processing keyword value"); 
                     M_out(L"    keyword name location -- %s") % value_loc_str(elem.name);
                     M_out(L"           vlist location -- %s") % vlist_loc_str(vlist);
                     M_out_emsgz(); 
                     results = error_results();                                           // pass back error results
                     return -1;              
                 }


                 // get string for keyword name, if evaluation results are string type or keyword type -- all other types not accepted

                if ( (keyname_results.ty == type_E::keyname) || (keyname_results.ty == type_E::string) )
                {
                    keyword_name = keyname_results.string;                               // set local keyword name from evaluated string result
                }
                else
                {   
                    M_out_emsg1(L"eval_vlist() -- keyword name evaluation did not yield a string type or keyword type -- cannot continue processing keyword value"); 
                    M_out(L"    keyword name location -- %s") % value_loc_str(elem.name);
                    M_out(L"           vlist location -- %s") % vlist_loc_str(vlist);
                    M_out_emsgz(); 
                    results = error_results();                                           // pass back error results
                    return -1;  
                }
             }  

             M__(M_out(L"eval_vlist() -- keyword name = \"%S\"") % keyword_name;)


             // evaluate keyword value (now that name is known, can look at verb's keyword evaluation flags)
             // --------------------------------------------------------------------------------------------

             parmtype_S eval_parmtype = get_keyword_parmtype(eval_plist, keyword_name);     // will have to evaluate any name expressions here
       
             if  ( (elem.value.ty == type_E::expression) || (elem.value.ty == type_E::vlist) || (elem.value.ty == type_E::identifier) || (elem.value.ty == type_E::ref) )
             {
                 M__(M_out(L"--------------------------------------->eval_vlist() calling eval_value() -- keyword");)
                 results_S val_results  { };
       
                 auto erc = eval_value(frame, elem.value, val_results, eval_parmtype);
             
                 M__(M_out(L"---------------------------------------> eval_value() returned -- r/c = %d") % erc;)

                 if (erc != 0)  rc = erc;                                    // remember any error
       
                 if (val_results.special_results)
                 {
                     M__(M_out(L"---------------------------------------> eval_value() returned with special results");)

                     results = val_results;                                  // pass back results with special flags
                     return rc;                                              // return with any errors
                 }
       

                 // handle multiple results -- 0 or 1 result allowed -- no keyword results allowed
                 // ------------------------------------------------------------------------------

                 M__(M_out(L"---------------------------------------> eval_value() returned with non-special results");)

                 if (val_results.multiple_results)
                 {
                     if ( (val_results.no_results) || (val_results.vlist_sp.get() == nullptr) )  // if no vlist, assume number of results = 0
                     {
                          M__(M_out(L"eval_vlist() -- handling multiple results (without vlist) from keyword value eval_value() call");)  
                          keyword_value = unit_val();                        // no multiple results (?? should not occur??) -- just use unit keyword value                      
                     }
                     else if (val_results.vlist_sp->kw_ct > 0) 
                     {
                         M_out_emsg1(L"eval_vlist() -- evaluation of expression supplied as keyword value returned with %d keywords -- none are allowed in a keyword value") % val_results.vlist_sp->kw_ct; 
                         msg_loc(elem.value, L"keyword value");
                         msgend_loc(vlist);
                         results = error_results();                          // pass back error results
                         return -1;  
                     }
                     else if (val_results.vlist_sp->value_ct == 0)
                     {
                         keyword_value = unit_val();                         // no multiple results (?? should not occur??) -- just use unit keyword value                       
                     }
                     else if (val_results.vlist_sp->value_ct == 1)
                     {
                         keyword_value = val_results.vlist_sp->values.at(0); // one multiple result (?? should not occur??) -- just use 1st value in multiple results vlist                        
                     }
                     else                                                    // 2 or more values in multiple results -- not allowed for keyword 
                     {
                         M_out_emsg1(L"eval_vlist() -- evaluation of expression supplied as keyword value returned with %d positional values -- max of one is allowed in a keyword value") % val_results.vlist_sp->value_ct; 
                         msg_loc(elem.value, L"keyword value");
                         msgend_loc(vlist);
                         results = error_results();                          // pass back error results
                         return -1;   
                     }
                 }
                 else                                                        // must be single results -- not special
                 {
                      M__(M_out(L"eval_vlist() -- handling single (non-special) result from keyword value expression evaluation");)  
                      keyword_value = val_results;                           // set keyword value from evaluated value_S portion of results_S 
                 }
             }
             else                                                            // no need to evaluate supplied keyword value -- just use it directly
             {
                 keyword_value = elem.value;                                 // just copy unevaluated value             
             }
                 
       
             //  Add evaluated keyword name (std::wstring) and evaluated keyword value to multimap of evaluated keywords 
             //  -------------------------------------------------------------------------------------------------------
             //
             //  Note: if evaluated keyword name is empty std::wstring, don't add this one to the evaluated keywords map -- note that keyword value (if any) has already been evaluated and may have caused side effects, etc.
             
             if (keyword_name.length() > 0)
                  vlist.eval_kws.emplace(keyword_name, keyword_value);  
         }
         
         updated            = true;                           // indicate that vlist has been updated
         vlist.kw_eval_done = true;                           // indicate that eval_kws has been filled in
         vlist.kw_ct        = vlist.eval_kws.size();          // update keyrord count o reflect number of evaluated keywords   
    }

    if (updated) 
        set_vlist_flags(vlist);

    if (rc != 0)
    {
        results = error_results();      // pass back error results
    }
    else
    {
        M__(M_out(L"=========================================> eval_vlist() setting up none results");)
        results = results_S { };       // uninitializedt results, when there is no error 
    }

    M__(display_vlist(vlist, L"output from eval_vlist()");)
    M__(M_out(L"=========================================> eval_vlist() returning");)
    return rc; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_expression() -- evaluate contents of a passed-in expression (values passed-in expression are updated in-place)
////                  
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   eval_expression() -- called in a loop from eval_expression()
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                                                                                  
static 
int eval_expression_1(frame_S& frame, const a_expression_S& orig_expression, results_S& results, bool debug, bool noeval_verbless) try
{
    e_expression_S    eval_expression    {       }; 
    int               rc                 { 0     };  
    bool              lparms_updated     { false }; 
    bool              rparms_updated     { false };    

    M__(M_out(L"*****************************************************eval_expression() called");)
    M__(display_expression(orig_expression, L"eval-before", L"", false);)


    // copy data members from original_expression to new expression
    // ----------------------------------------------------

    eval_expression.has_verb         =  orig_expression.has_verb         ;
    eval_expression.has_sigil        =  orig_expression.has_sigil        ;
    eval_expression.sigil            =  orig_expression.sigil            ;
    eval_expression.priority         =  orig_expression.priority         ;
    eval_expression.right_associate  =  orig_expression.right_associate  ;
    eval_expression.left_associate   =  orig_expression.left_associate   ;
    eval_expression.token_ix1        =  orig_expression.token_ix1        ;
    eval_expression.token_ix2        =  orig_expression.token_ix2        ;
    eval_expression.lparms           =  orig_expression.lparms           ;               // nested complex values (pointed to by std::shared_ptr<> ) are shared with orig a_expression_S
    eval_expression.rparms           =  orig_expression.rparms           ;               // nested complex values (pointed to by std::shared_ptr<> ) are shared with orig a_expression_S


    // ==========================================================
    // get verbdef, etc, (if verb is present for this expression)
    // ==========================================================

    verbdef_S temp            {       }; 
    verbset_S verbset         {       };                                                 // will be filled in (overwritten), if real verb is present -- skeleton eval flags are filled in for verbless expressions
    verbset.verbs.push_back(temp);                                                       // default verbset_S, in case verb expression is empty string (and there is no verb)

    bool      have_verbdef    { false };                                                 // set to true, once non-empty verbdef found
    
    if (orig_expression.has_verb)
    {
        // set evaluated verb name in e_expression_S -- either direct from a_expression_S verb_value string value, or by evaluation of a_expression_S verb_value expression

        if (orig_expression.verb_value.ty == type_E::verbname)                           // handle simple verbname in this expression
        {
            eval_expression.verb_name      = orig_expression.verb_value.string;          // simple verbname -- name is just string value 
            eval_expression.verb_token_ix1 = orig_expression.verb_value.token_ix1;       // copy over token indexes for debugging
            eval_expression.verb_token_ix2 = orig_expression.verb_value.token_ix2; 
        }                                                                            
        else                                                                             // verb expression requires evaluation
        {                                                                               
            // need to handle non-simple verbname -- evaluate value and check for verbset or string that can be converted to verb_name string
            // -------------------------------------------------------------------------------------------------------------------------------

            results_S  verbname_results  { };                                            // results from verbname value evaluation  (final)
            parmtype_S verbname_parmtype { };                                            // eval_value() uses only "no_eval" flags, which are off by default

            auto evrc = reeval_value(frame, orig_expression.verb_value, verbname_results, verbname_parmtype, false);

            if (evrc != 0) rc = evrc;

            if (verbname_results.special_results)
            {
                M__(M_out(L"eval_expression() -- special results from verbname expression = \"%S\"") % str_results_string(verbname_results);)  
                results = verbname_results;                                              // pass back unusual results from verb_value evaluation
                return rc;                       
            }  

            if (evrc != 0)
            {
                M_out_emsg1(L"eval_expression() -- expression evaluation to obtain verbname/verbdef failed -- orig_expression.verb_value = %S -- cannot execute") % verb_name(orig_expression); 
                msgend_loc(orig_expression); 
                results = error_results();                                                // pass back error results
                return -1;              
            }


            // get string for verbset_S or verb name, if evaluation results are string or verbname -- all other types not accepted

            if ( (verbname_results.ty == type_E::verbname) || (verbname_results.ty == type_E::string) )
            {
                eval_expression.verb_name      = verbname_results.string;                  // save away evaluated string
                eval_expression.verb_token_ix1 = orig_expression.verb_value.token_ix1;     // copy over token indexes for debugging
                eval_expression.verb_token_ix2 = orig_expression.verb_value.token_ix2; 
            }
            else if (verbname_results.ty == type_E::verbset)
            {
                verbset = *(verbname_results.verbset_sp);                                 // just capture verbdef from verbname expression evaluation
                have_verbdef = true;                                                      // suppress verbdef lookup down below, and indicate verbdef is already available
                eval_expression.verb_token_ix1 = orig_expression.verb_value.token_ix1;    // copy over token indexes for debugging
                eval_expression.verb_token_ix2 = orig_expression.verb_value.token_ix2; 
                M__(M_out(L"eval_expression() -- verbdef results");)
            }
            else
            {
                 M_out_emsg1(L"eval_expression() -- orig_expression.verb_value evaluation did not yield a string, verbname, or verbdef -- orig_expression.verb_value = %s  -- verbname_results.ty = %S -- cannot execute") % verb_name(orig_expression) % type_str(verbname_results.ty); 
                 msgend_loc(orig_expression); 
                 results = error_results();                                                // pass back error results
                 return -1;  
            }
        }
   

        // get verbset for verb_name (if any) to see what type of variable and expression evaluation is needed in left and right parms -- end immediately, if unknown verb

        if ( (!have_verbdef) && (eval_expression.verb_name.size() > 0) )                    // need to lookup verbdef -and- evaluated verb_name expression is non-empty string? 
        {                                                                                 
            auto verbdef_rc = get_verb(frame, eval_expression.verb_name, verbset);          // returns default, if undefined verb 
           
            if (verbdef_rc != 0)                                                            // undefined verb? 
            {
                M__(M_out(L"eval_expression() -- ix1/ix2 = %d/%d -- verb-ix1/ix2 = %d/%d") % eval_expression.token_ix1 % eval_expression.token_ix2 % eval_expression.verb_token_ix1 % eval_expression.verb_token_ix2;)

                if (is_identifier_defined(frame, eval_expression.verb_name))
                    M_out_emsg1(L"eval_expression() -- defined name \"%S\" is not the name of a verb -- cannot execute") % eval_expression.verb_name; 
                else
                    M_out_emsg1(L"eval_expression() -- verbname = \"%S\" is not defined -- cannot execute") % eval_expression.verb_name; 
               
                msgend_loc(eval_expression); 
                results = error_results();                                                  // pass back error results
                return -1; 
            }  

            have_verbdef = true;                                                            // indicate that verbdef is now available 
        }
    }
    else                                                                                    // this expression has no verb -- may need to suppress evaluation of values in vlist 
    {
        // set up dummy verbset/verbdef for verbless expression
        // ----------------------------------------------------

        verbset.verbless                                        = true;                     // indicate that this is a verbless (dummy) verbset_S
        verbset.verbs.at(0).verbless                            = true;                     // indicate that this is a verbless (dummy) verbdef_S
        verbset.verbs.at(0).lparms.eval.verbless                = true;                     // indicate that this is verbless evaluation
        verbset.verbs.at(0).rparms.eval.verbless                = true;                     // indicate that this is verbless evaluation   -- note: probably no right side vlist when verbless
       
        if (noeval_verbless)                                                                // should vlist ident, sub expression, etc. evaluation be suppressed when verbless? (configuration flag)
        {
            verbset.verbs.at(0).lparms.eval.no_eval_ident       = true;                                            
            verbset.verbs.at(0).lparms.eval.no_eval_vlist       = true;    
       //   verbset.verbs.at(0).lparms.eval.no_eval_expression  = true;                     // evaluate nested expressions, only
            verbset.verbs.at(0).lparms.eval.no_eval_ref         = true; 
            verbset.verbs.at(0).rparms.eval.no_eval_ident       = true;                     //  note: probably no right side vlist when verbless, so these aren't needed                       
            verbset.verbs.at(0).rparms.eval.no_eval_vlist       = true;    
       //   verbset.verbs.at(0).rparms.eval.no_eval_expression  = true;                     // evaluate nested expressions, only
            verbset.verbs.at(0).rparms.eval.no_eval_ref         = true; 
        }                              
    }


    //  Process any nested expressions in left or right vlists -- replace expression with value -- updated value can be another expression or variable -- no double evaluation -- these will be left as-is here (verb checking may complain about them) 
    //  ------------------------------------------------------

    if ( (eval_expression.lparms.value_ct > 0) || (eval_expression.lparms.kw_ct > 0) )                        // un-evaluated kw count
    {
        M__(M_out(L"eval_expression() -- process left vlist (lparms) -- calling eval_vlist()");) 
        results_S vlist_results {}; 
        auto erc = eval_vlist(frame, eval_expression.lparms, vlist_results, verbset.verbs.at(0).lparms);      

        if (erc != 0) rc = erc;

        if (vlist_results.special_results)
        {
            M__(M_out(L"eval_expression() -- special results from left-side eval_vlist() for verb = \"%S\" -- special results = \"%S\"") % verb_name(eval_expression) % str_results(vlist_results, true, false, true);)    
            results = vlist_results;                                                                          // pass back unusual results from vlist evaluation
            return rc;                       
        }
    }

    if ( (eval_expression.rparms.value_ct > 0) || (eval_expression.rparms.kw_ct > 0) )                       // un-evaluated kw count
    {
        M__(M_out(L"eval_expression() -- process right vlist (rparms) -- calling eval_vlist()");) 
        results_S vlist_results {}; 
        auto erc = eval_vlist(frame, eval_expression.rparms, vlist_results, verbset.verbs.at(0).rparms);     

        if (erc != 0) rc = erc;

        if (vlist_results.special_results)
        {
            M__(M_out(L"eval_expression() -- special results from right-side eval_vlist() for verb = \"%S\" -- special results = \"%S\"") % verb_name(eval_expression) % str_results(vlist_results, true, false, true);)  
            results = vlist_results;                                                                        // pass back unusual results from vlist evaluation
            return rc;                       
        }
    }
    
    M__(display_expression(eval_expression, L"eval-after", L"", false);)


    // don't evaluate the main verb, if any errors seen so far during vlist evaluation

    if (rc != 0)
    {
        M_out_emsg1(L"eval_expression() -- bypassing execution of verb= %s  because of errors while evaluating parms") % verb_name(eval_expression);
        msgend_loc(eval_expression); 
        results = error_results();                                                            // pass back error results
        return rc; 
    }


    // do preliminary parm checking based on verbdef_S for main verb (if any)
    // ----------------------------------------------------------------------

    // ???????????????????? verbdef_S selection from the verbset_S returned from get_verb() will be required here  ?????? 

    verbdef_S verbdef = verbset.verbs.at(0); 

    if (eval_expression.has_verb && have_verbdef)                                             // bypass parm checking if no verbdef is available
    {
      M__(M_out(L"eval_expression() -- doing parm checking -- calling check_verb_parms()");)
      auto crc = check_verb_parms(frame, eval_expression, verbdef);
      if (crc != 0) rc = crc; 
    }


    // don't evaluate the main verb, if any errors seen so far (during parm chedcking)

    if (rc != 0)
    {
        M_out_emsg1(L"eval_expression() -- bypassing execution of verb= %s  because of errors while checking basic parm counts and types") % verb_name(eval_expression); 
        msgend_loc(eval_expression); 
        results = error_results();                                                             // pass back error results
        return rc; 
    }


    // ==========================================
    // evaluate the main verb for this expression
    // ==========================================

    if (eval_expression.has_verb)
    {
        // just return unit value, if no verbdef is available

        if (!have_verbdef)
        {
             results = unit_results();                                                          // return unit results ???????   -- shold this be an eror message ???????
        } 

        // otherwise, invoke the verb to get results
        // -----------------------------------------

        else                                                                                    // evaluated verb name is not empty string
        {
            if (verbdef.fcn_p != nullptr)
            {
                if (verbdef.simplified_call)
                {
                    M__(M_out(L"eval_expression() -- doing simplified verb call <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)

                    // do simplified verb call -- non ex-non-internal predefined verbs
                    
                    int (*call_p)(const e_expression_S&, value_S&) = (int (*)(const e_expression_S&, value_S&))(verbdef.fcn_p);  // simplified call pointer
                    
                    value_S result_value {  };                                                                                   // predefined uninitialized value   
                    auto prc = (*call_p)(eval_expression, result_value);                                                         // will pass back value_S converted to (non-special) result_S from verb call
                    results = to_results(result_value);                                                                          // convert return value to full (non-special) results 
                                                                                                                     
                    if (prc != 0) rc = prc;                                                                                      // remember any error from verb execution 
                }
                else
                {
                     M__(M_out(L"eval_expression() -- doing regular verb call <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)

                    // do regular verb call -- ex-internal predefined verbs

                    int (*call_p)(frame_S&, const e_expression_S&, const verbdef_S&, results_S&) = (int (*)(frame_S&, const e_expression_S&, const verbdef_S&, results_S&))(verbdef.fcn_p);  // regular call pointer
                            
                    auto prc = (*call_p)(frame, eval_expression, verbdef, results);                                              // will pass back result_S from verb call
                                                                                                                      
                    if (prc != 0) rc = prc;                                                                                      // remember any error from verb execution 
                }                                                                                                               
                                                                                                                                
                frame.verb_eval_ct++;                                                                                            // accumulate verb calls in current stack frame
                static_N::verb_count++;                                                                                          // accumulate total verb calls                  
                               
                // Indicate that these results are from a builtin verb, if the verbdef indicates built-in verb     

                results.builtin_verb_results = verbdef.is_builtin;     
                M__(M_out(L"eval_expression() -- results.builtin_verb_results = %d <<<<<<<<<<<<<<<<<<<<<<<<<") % ((int)(results.builtin_verb_results));)                           
            }                                                                    
            else                                                                                                                 // should not happen
            {
                M_out_emsg1(L"eval_expression() -- verbdef.fcn_p = nullptr for verb=%S") %         verb_name(eval_expression)    ;  
                M_throw_v(   "eval_expression() -- verbdef.fcn_p = nullptr for verb=%S") % out_ws( verb_name(eval_expression)) ));  
            } 
        }
    }


    // no verb present -- should be empty parens, or parens with just left-side positional values/keywords present -- no right parms expected here 
    // -------------------------------------------------------------------------------------------------------------------------------------------

    else                                                                               
    {
        if (
            (eval_expression.rparms.value_ct   > 0)                                                                            // no right-side positional parms allowed without verb   (should not happen)
            ||                                                                                                            
            (eval_expression.rparms.kw_ct > 0     )                                                                            // no right-side keywords         allowed without verb   (should not happen)
           )
        {
            // invalid verbless expression -- error -- should not occur -- error should heve been found during parsing phase before evaluation started

            M_out_emsg1(L"eval_expression() -- verbless expression with right-side positional or keyword parms seen -- should not occur");
            msgend_loc(eval_expression);
            M_throw("eval_expression() -- verbless expression with right-side positional or keyword parms")         
        }
        else
        {
            M__(M_out(L"eval_expression() -- handling no-verb case <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////
            //////
            //////     =========================
            //////     handle no-verb expression -- just empty expression or vlist with left-side positional/keyword parm(s)  
            //////     =========================
            //////
            //////
            //////     Just take left side vlist resulting from verbless evaluation and return it as the results from this expresion 
            //////
            //////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            M__(M_out( L"eval_expression() -- lparms.value_ct=%d   lparms.kw_ct=%d   rparms.value_ct=%d   rparms.kw_ct=%d ") 
                     % eval_expression.lparms.value_ct % eval_expression.lparms.kw_ct % eval_expression.rparms.value_ct % eval_expression.rparms.kw_ct;)
 

            // make sure any keywords in verbless left-side parms vlist are in unevaluated format
            // ----------------------------------------------------------------------------------
         
            if (eval_expression.lparms.kw_eval_done)                          // has keyword multimap been filled in? 
                refresh_evaluated_vlist(eval_expression.lparms);              // make it look like keywords have not been evaluated yet


            // pass back left-side vlist from earlier verbless evaluation
            // ----------------------------------------------------------

            if ( (eval_expression.lparms.kw_ct > 0) || (eval_expression.lparms.value_ct > 1) )
            {
                M__(display_vlist(eval_expression.lparms, L"eval_expression() -- passing back from verbless expression");)
                results = to_results(vlist_val(eval_expression.lparms));        // pass back whole left-side positional parm vlist -- will be multiple results
                results.multiple_results = true;                                // indicate that multiple results are being returned
            }
            else if (eval_expression.lparms.value_ct == 0)                      // just parens -- no parms inside (or they all went away during evaluation)
            {
                results = no_results();                                         // zero result values -- vlist_sp is not initialized
            }
            else                                                                // must be single positional value, with no keywords
            {
                results = to_results(eval_expression.lparms.values.at(0));      // pass back 1st/only left-side positional parm 
            }
        }
    }

    M__(M_out(L"*****************************************************eval_expression() returning");)

    if (results.special_results)
    {
         M__(M_out(L"eval_expression() -- special results from  eval_expresssion() for verb = \"%S\" -- special results = \"%S\"") % verb_name(eval_expression) % str_results(results, true, false, true);)
    }
    return rc; 
}
M_endf
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   eval_expression() -- call eval_expression_1() in loop, as long as @XCTL results are seen from expression execution
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int eval_expression(frame_S& frame, const a_expression_S& expression, results_S& results, bool debug) try
{
    const a_expression_S     *expression_p    {&expression}; 
    results_S            results_x  {      };                         // @XCTL results from eval_expression_1() -- should have shared ptr to expression_S for next loop pass 


    // main loop to call eval_expression_1(), and repeat loop to handle any @XCTL results
    // -----------------------------------------------------------------------------

    for (;;)
    {
        results_S results_1   { };                                    // results from eval_expression_1()
                                                
        auto rc = eval_expression_1(frame, *expression_p, results_1, debug, NOEVAL_VERBLESS);    
        M__(M_out(L"eval_expression() -- eval_expression_1() returned <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); )
      
        if (rc != 0)
        {
            results = results_1;                                      // pass back (error?) results from eval_expression_1() 
            return rc;                                                // pass back any errors from eval_expression_1()
        }
       
        if ( !(results_1.special_results && results_1.xctl_flag) )
        {
            M__(M_out(L"eval_expression() -- returning (non-@XCTL results  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); )
            results = results_1;                                      // pass back results from eval_expression_1()
            return rc;                                                // pass back all results, except for @XCTL special results
        }
                               
       
        // handle @XCTL results   
        
        if (results_1.builtin_verb_results == true)
        {
            M__(M_out(L"eval_expression() -- processing @XCTL results -- from built-in verb <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)
            results = results_1;                                      // pass back special @XCTL results from eval_expression_1() -- don't action this time -- wait for user's verb (non-builtin) to come back before doing the @XCTL
            return rc;                                                
        }

        M__(M_out(L"eval_expression() -- processing @XCTL results -- from user-defined verb <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)   

        results_x = results_1;                                        // hold onto expression_S from @XCTL results via shared ptr until next call to eval_expression_1()
        expression_p   = results_x.expression_sp.get();                         // next pass -- use expression provided with with @XCTL results
       
       
        // (loop back to call eval_expression_1() with new expression setup by  the @XCTL verb, in place of current expression)
    }

}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_block() -- evaluate all expressions in a block -- return results from last expression evaluated (if any)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int eval_block(frame_S& frame, const block_S& block, results_S& results) try
{
    M__(M_out(L"eval_block() -- called");)  
    int rc = 0; 
    results_S block_results { no_results() };               // start with no results, in case 1st expression is something like @CONTINUE with ignored results

    auto                     expression_max   = block.expressions.size(); 
    decltype(expression_max) expression_ix    = 0; 

    M__(M_out(L"eval_block() -- expression_max=%d") % expression_max;)
        
    static_N::block_count++; 


    // filter out empty block -- need to return 0 results from this (not just unit results)

    if (expression_max == 0)
    {
        results = no_results();                  // zero result values
        M__(M_out(L"eval_block() -- multiple results = 0 results");)
        return 0; 
    }


    // --------------------------------------------------------------
    // main loop to process each statement (expression) in this block
    // --------------------------------------------------------------

    for (;;)
    { 
        M__(M_out(L"eval_block() -- top of for(;;) loop");)

        if (expression_ix >= expression_max)
            break; 

        static_N::statement_count++; 


        //   evaluate n-th expression in block, and handle error r/c
        //   -------------------------------------------------------

        results_S expression_results { }; 

        M__(M_out(L"eval_block() -- calling eval_expression(%d)") % expression_ix;)

        auto erc = eval_expression(frame, block.expressions.at(expression_ix), expression_results);
        if (erc != 0)
        {
            M_out_emsg1(L"eval_block() -- block evaluation ending immediately because of errors during expression evaluation at statement %d") % expression_ix;
            msgend_loc(block.expressions.at(expression_ix));
            rc = erc; 
            results = error_results(); 
            return -1; 
        }


        // save away expression results as the block results, unless the "ignore results" special flag is set

        if (!expression_results.ignore_results)
            block_results = expression_results;



        //   ======================
        //   handle special results -- @LEAVE and @GOTO are processed at the block level -- may get consumed if label matches, or percolated otherwise    --    other special results are passed back  
        //   ====================== 

        if (expression_results.special_results)
        {
            M__(M_out(L"eval_block() -- handling special results");)

            //  handle     (plain) @LEAVE      -or-       @LEAVE "target block"
            //  ---------------------------------------------------------------

            if (expression_results.leave_flag)
            {
                // if (plain) @LEAVE   -or-   @LEAVE target    matches this block -- end processing of this block  

                if ( expression_results.str.empty() || (expression_results.str == block.label) )
                {
                    M__(M_out(L"eval_block() -- @LEAVE target found in current block -- ending block with unit results");)

                    expression_results.leave_flag      = false;           // consume the @LEAVE         
                    expression_results.special_results = false;     
                    results = expression_results;                         // pass back @LEAVE results -- @LEAVE is consumed 
                    return 0;
                }
              
                
                // @LEAVE is not targetting the current block -- need to "percolate" @LEAVE results upwards to enclosing blocks (if any)
                
                M__(M_out(L"eval_block() -- @LEAVE target not found in current block -- percolating upwards");)
                results = expression_results; 
                break;                 
            }
        

            //  handle @GOTO or @GOTO longjmp:
            //  ------------------------------

            if (expression_results.goto_flag || expression_results.lgoto_flag )
            {
                // see if  @GOTO target  matches some label in this block  

                auto target_ct = block.labels.count(expression_results.str);
                if (target_ct > 0)   
                {
                    // matching label found in this block -- reset expression_ix to label position and continue looping (label may point past last expression in this block
                    
                    expression_ix = block.labels.at(expression_results.str);
                    M__(M_out(L"eval_block() -- @GOTO target found in current block -- restarting block evaluation at expression index = %d") % expression_ix;)
                    continue;
                }


                //  @GOTO target was not found in this block -- "percolate" @GOTO results upwards to enclosing blocks (if any)   


                M__(M_out(L"eval_block() -- @GOTO target not found in current block -- percolating upwards");)
                results = expression_results; 
                break; 
            }


            //  handle @SKIPTO
            //  --------------

            if (expression_results.skip_flag )
            {
                //  percolate @SKIPTO results 

                M__(M_out(L"eval_block() -- percolating @SKIPTO results -- results = \"%S\"") % str_results_string(expression_results);)
                results = expression_results; 
                break; 
            }
                

            // any other special results (@RETURN, @XCTL, @QUIT, etc.) will cause block evaluation to end -- empty special results flags are passed back to block evaluator for handling or further percolation

            M__(M_out(L"eval_block() -- percolating special results upward  -- results = \"%S\"") % str_results_string(block_results);)
            results = block_results;                                   // pass back special results -- don't use expression_results here, since @CONTINUE results are not saved as the block results  
            break; 
        }


        // no error -- no special results -- normal advance to next expression in block

        M__(M_out(L"eval_block() -- no special results from expression(%d)") % expression_ix;)
        expression_ix++; 
    }
  
    results = block_results;                                           // return results from last expression evaluated -- no special results get here 
    M__(M_out(L"eval_block() -- normal return at end");)
    return rc; 
}
M_endf



//_________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""