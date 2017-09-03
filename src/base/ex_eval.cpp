// ex_eval.cpp

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



//  forward declarations for selected static functions
//  --------------------------------------------------

static int          check_verb_parms(frame_S&, const e_vexpr_S&, const verbdef_S&                                                                 );
static int          check_verb_value(frame_S&, const e_vexpr_S&, const parmtype_S&, const value_S&, const std::wstring&                           );
static int          check_verb_vlist(frame_S&, const e_vexpr_S&, const plist_S&   , const vlist_S&, const std::wstring&, const std::wstring& = L"");  



//    static variables 
//    ----------------

namespace static_N
{
uint64_t                   verb_count      {0};                  // number of verbs executed
uint64_t                   value_count     {0};                  // number of values evaluated
uint64_t                   slist_count     {0};                  // number of slists executed
uint64_t                   statement_count {0};                  // number of slists executed

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
////     frame statistics functions
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

uint64_t get_eval_verb_count(      void) {return static_N::verb_count      ;}
uint64_t get_eval_value_count(     void) {return static_N::value_count     ;}
uint64_t get_eval_slist_count(     void) {return static_N::slist_count     ;}
uint64_t get_eval_statement_count( void) {return static_N::statement_count ;}

uint64_t get_eval_frame_serial(    void) {return static_N::frame_serial    ;}
uint64_t get_eval_frame_depth(     void) {return static_N::frame_depth     ;}
uint64_t get_eval_frame_max_depth( void) {return static_N::frame_max_depth ;}




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

int get_right_positional(const e_vexpr_S& vexpr, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th  positional right-side parm is not present

    if (vexpr.rparms.value_ct <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th positional right-side parm is present -- return parm value

    value = vexpr.rparms.values.at(n);
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

int get_left_positional(const e_vexpr_S& vexpr, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th  positional left-side parm is not present

    if (vexpr.lparms.value_ct <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th positional left-side parm is present -- return parm value

    value = vexpr.lparms.values.at(n);
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

int get_right_keyword(const e_vexpr_S& vexpr, const std::wstring& keyword_name, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th right keyword is not present

    if (vexpr.rparms.eval_kws.count(keyword_name) <= n)
    {
        value = unit_val(); 
        return -1; 
    }


    // n-th keyword is present -- return keyword value

    value = multimap_at(vexpr.rparms.eval_kws, keyword_name, n);
    return 0; 
}
M_endf


// version without passed-back value parm -- just R/C 

int get_right_keyword(const e_vexpr_S& vexpr, const std::wstring& keyword_name, uint32_t n) try
{
    // return  R/C = -1, if n-th right keyword is not present

    if (vexpr.rparms.eval_kws.count(keyword_name) <= n)
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

int get_left_keyword(const e_vexpr_S& vexpr, const std::wstring& keyword_name, value_S& value, uint32_t n) try
{
    // return unit value, and R/C = -1, if n-th left keyword is not present

    if (vexpr.lparms.eval_kws.count(keyword_name) <= 0)
    {
        value = unit_val();  
        return -1; 
    }


    // n-th keyword is present -- return keyword value

    value = multimap_at(vexpr.lparms.eval_kws, keyword_name, n);
    return 0; 
}
M_endf


// version without passed-back value parm 

int get_left_keyword(const e_vexpr_S& vexpr, const std::wstring& keyword_name, uint32_t n) try
{
    // return  R/C = -1, if n-th right keyword is not present

    if (vexpr.lparms.eval_kws.count(keyword_name) <= n)
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

// version with passed-in keyword name
// -----------------------------------

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

static int check_verb_value(frame_S& frame, const e_vexpr_S& vexpr, const parmtype_S& parmtype, const value_S& value, const std::wstring& ws) try
{
    int rc {0}; 

    // return OK, if no checking needed -- includes bypassing nested vlist checking

    if (parmtype.anything_ok)
        return 0; 
   

    // check this value type compared to what is allowed in passed-in parmtype_S structure

    if ( (!parmtype.nval_ok) && (value.ty == type_E::no_value) )         // keyword with no associated value
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected keyword with no following value") % verb_name(vexpr) % ws; 
        msgend_loc(value, vexpr);
        rc = -1;  
    }
       
    if ( (!parmtype.unit_ok) && (value.ty == type_E::unit) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected unit parm") % verb_name(vexpr) % ws; 
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.int8_ok) && (value.ty == type_E::int8) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected int8 parm = %d") % verb_name(vexpr) % ws % (int16_t)(value.int8);
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.int16_ok) && (value.ty == type_E::int16) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected int16 parm = %d") % verb_name(vexpr) % ws % value.int16;
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.int32_ok) && (value.ty == type_E::int32) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected int32 parm = %d") % verb_name(vexpr) % ws % value.int32;
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.int64_ok) && (value.ty == type_E::int64) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected int64 parm = %d") % verb_name(vexpr) % ws % value.int64;
        msgend_loc(value, vexpr);
        rc = -1;  
    }
    
    if ( (!parmtype.uint8_ok) && (value.ty == type_E::uint8) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected uint8 parm = %d") % verb_name(vexpr) % ws % (uint16_t)(value.uint8);
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.uint16_ok) && (value.ty == type_E::uint16) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected uint16 parm = %d") % verb_name(vexpr) % ws % value.uint16;
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.uint32_ok) && (value.ty == type_E::uint32) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected uint32 parm = %d") % verb_name(vexpr) % ws % value.uint32;
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.uint64_ok) && (value.ty == type_E::uint64) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s --  unexpected uint64 parm = %d") % verb_name(vexpr) % ws % value.uint64;
        msgend_loc(value, vexpr);
        rc = -1;  
    }


    if ( (!parmtype.float32_ok) && (value.ty == type_E::float32) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected float32 parm = %g") % verb_name(vexpr) % ws % value.float32;
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.float64_ok) && (value.ty == type_E::float64) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected float64 parm = %g") % verb_name(vexpr) % ws % value.float64;
        msgend_loc(value, vexpr);
        rc = -1;  
    }
    
    if ( (!parmtype.string_ok) && (value.ty == type_E::string) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected string parm = «%s»") % verb_name(vexpr) % ws % value.string;
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.verbname_ok) && (value.ty == type_E::verbname) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected verbname parm = «%s»") % verb_name(vexpr) % ws % value.string;
        msgend_loc(value, vexpr);
        rc = -1;  
    }
     


    // check (unevaluated) identifiers -- raw(all) / variable /constant / undefined
    // ----------------------------------------------------------------------------

    if (value.ty == type_E::identifier)
    {   
        if (parmtype.raw_ident_ok)                     // allow any kind of (unevaluated) identifier?
        {
            ;                                          // OK -- no need to check identifier type (var/const/undef)
        }
        else if (                                      // see if some type of identifier is OK
                 (!parmtype.var_ident_ok  )
                  &&
                 (!parmtype.const_ident_ok)   
                  &&
                 (!parmtype.undef_ident_ok)   
                ) 
        {
            // not expecting any type of identifier -- error

            count_error(); 
            M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected identifier parm = %s") % verb_name(vexpr) % ws % value.string;
            msgend_loc(value, vexpr);
            rc = -1;  
        }
        else                                          // expecting some specific type(s) of identifier -- variable/const/undef -- need to check further
        {
            // check local, global, or all stack frames for this identifier
            //   note: assume local and global flags are not both set to true
              
            if (parmtype.check_local_env_only)
            {
                // check only local identifiers (not all) 
         
                if ( (!parmtype.var_ident_ok) && is_local_identifier_variable(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) pre-defined local variable identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                }
         
                if ( (!parmtype.const_ident_ok) &&  is_local_identifier_const(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) pre-defined local constant identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                }
                     
                if ( (!parmtype.undef_ident_ok) && (!is_local_identifier_defined(frame, value.string)) )
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected undefined local identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                } 
            }
         
            else if (parmtype.check_global_env_only)
            {
                // check only global identifiers (not all) 
         
                if ( (!parmtype.var_ident_ok) &&  is_global_identifier_variable(value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) global pre-defined variable identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                }
         
                if ( (!parmtype.const_ident_ok) &&  is_global_identifier_const(value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) global pre-defined constant identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                }
                     
                if ( (!parmtype.undef_ident_ok) &&  (!is_global_identifier_defined(value.string)) )
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected undefined global identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                } 
            }
 
            else      // neither local or global flag is set -- check all stack frames for this identifier
            {
                // check all stack frames to see what identifier type is (var/const/undef)
          
                if ( (!parmtype.var_ident_ok) &&  is_identifier_variable(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) pre-defined variable identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                }
              
                if ( (!parmtype.const_ident_ok) &&  is_identifier_const(frame, value.string) ) 
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected (unevaluated) pre-defined constant identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                }
                     
                if ( (!parmtype.undef_ident_ok) && (!is_identifier_defined(frame, value.string)) )
                {
                    count_error(); 
                    M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected undefined identifier parm = %s") % verb_name(vexpr) % ws % value.string;
                    msgend_loc(value, vexpr);
                    rc = -1;  
                }
            }
        }               // expecting specific identifier type(s) -- variable/const/undef 
    }                   // identifier


    // check vlist, slist, vexpr, verbdef, etc.
    // ---------------------------------------

    M__(M_out(L"check_verb_value() -- value.ty = %d = %S") % (int)(value.ty) % type_str(value.ty);)

    if ( (!parmtype.vlist_ok) && (value.ty == type_E::vlist) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected vlist parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.vexpr_ok) && (value.ty == type_E::vexpr) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected vexpr parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }
    
    if ( (!parmtype.slist_ok) && (value.ty == type_E::slist) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected slist parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }

    if ( (!parmtype.verbdef_ok) && (value.ty == type_E::verbdef) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected verbdef parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }                                                                        

    if ( (!parmtype.typdef_ok) && (value.ty == type_E::typdef) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected typdef parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }  


    if ( (!parmtype.array_ok) && (value.ty == type_E::array) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected array parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }  
    

    if ( (!parmtype.structure_ok) && (value.ty == type_E::structure) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected structure parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }  


    if ( (!parmtype.lvalue_ref_ok) && ((value.ty == type_E::ref) && (value.ref_sp->is_lvalue)) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected lvalue-ref parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }  


    if ( (!parmtype.rvalue_ref_ok) && ((value.ty == type_E::ref) && (value.ref_sp->is_rvalue)) )
    {
        count_error(); 
        M_out_emsg1(L"check_verb_value() -- verb= %s %s -- unexpected rvalue-ref parm") % verb_name(vexpr) % ws;
        msgend_loc(value, vexpr);
        rc = -1;  
    }  


    // do int64 and float64 range checking
    // -----------------------------------

    if ( (value.ty == type_E::int64) && parmtype.int64_ok && parmtype.int64_range )
    {
        if ( (value.int64 < parmtype.int64_min) || (value.int64 > parmtype.int64_max) )
        {
            count_error(); 
            M_out_emsg1(L"check_verb_value() -- verb= %s %s -- int64 value is not in allowed range -- min=%d  value=%d  max=%d") % verb_name(vexpr) % ws % parmtype.int64_min % value.int64 % parmtype.int64_max;
            msgend_loc(value, vexpr);
            rc = -1;         
        }    
    }

    if ( (value.ty == type_E::float64) && parmtype.float64_ok && parmtype.float64_range )
    {
        if ( (value.float64 < parmtype.float64_min) || (value.float64 > parmtype.float64_max) )
        {
            count_error(); 
            M_out_emsg1(L"check_verb_value() -- verb= %s %s -- float64 value is not in allowed range -- min=%g  value=%g  max=%g") % verb_name(vexpr) % ws % parmtype.float64_min % value.float64 % parmtype.float64_max;
            msgend_loc(value, vexpr);
            rc = -1;         
        }    
    }
    

    // validate nested vlist (if any) -- if passed-in parmtype_S points to a nested plist_S
    
    if ( parmtype.vlist_ok && (value.ty == type_E::vlist) && (parmtype.plist_sp.get() != nullptr) && (value.vlist_sp.get() != nullptr) )    
    {
        auto crc = check_verb_vlist(frame, vexpr, *(parmtype.plist_sp), *(value.vlist_sp), L" " + ws + L"nested vlist", L"");
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

static int check_verb_vlist(frame_S& frame, const e_vexpr_S& vexpr, const plist_S& plist, const vlist_S& vlist, const std::wstring& ws1, const std::wstring& ws2) try
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
            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s expects at least %d %s positional parm(s), but only %d were present") % verb_name(vexpr) % ws1 %  plist.min_ct % ws2 % vlist.value_ct;
            msgend_loc(vlist, vexpr);
            rc = -1;          
        }
       
        if  ( (plist.max_ct >= 0) && (vlist.value_ct > plist.max_ct) )
        {
            count_error(); 
            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s expects at most %d %s positional parm(s), but %d were present -- possible missing semicolon?") % verb_name(vexpr) % ws1 % plist.max_ct % ws2 % vlist.value_ct;
            msgend_loc(vlist, vexpr);
            rc = -1;          
        }


        // set up default parmtype_S to use if vector does not have any parmtype_S elements
      
        parmtype_S default_parmtype {};                                 // default parmtype
        default_parmtype.no_eval_ident = plist.no_eval_ident;           // copy in eval flag (set only for verbless vexprs) 
        default_parmtype.no_eval_vexpr = plist.no_eval_vexpr;           // copy in eval flag (set only for verbless vexprs) 
        default_parmtype.no_eval_vlist = plist.no_eval_vlist;           // copy in eval flag (set only for verbless vexprs) 
        default_parmtype.anything_ok = true;                            // allow anything -- default is no checking 
      
      
        // loop to look at each positional parm and do parm type checking
      
        if (!plist.no_check_positional)
        {
            for (int i = 0; i < vlist.values.size(); i++)
            {
                std::wstring ws {ws1 + ws2 + L" positional parm " + std::to_wstring(i) + L":"};
           
           
                if (plist.values.empty())                                                                   // no parmtype_S elements available? 
                {
                    auto crc = check_verb_value(frame, vexpr, default_parmtype, vlist.values.at(i), ws);    // use default one just constructed -- no checking
                    if (crc != 0)  rc = crc;                                                                // remember any error   
                }
                else                                                                                        // some parmtype_S are available in plist.values vector -- use i-th parmtype_S, or last one if not enough)               
                {
                    auto crc = check_verb_value(frame, vexpr, plist.values.at(std::min(i, (int)(plist.values.size() - 1))), vlist.values.at(i), ws);
                    if (crc != 0)  rc = crc;                                                                // remember any error
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
                M_out_emsg1(L"check_verb_vlist() -- verb= %s %s expects no %s keywords, but %d was/were present") % verb_name(vexpr) % ws1 % ws2 % vlist.kw_ct;
                msgend_loc(vlist, vexpr);
                rc = -1;          
            }
        }
        else
        {
            std::multiset<std::wstring> kw_done { }; 


            // keywords allowed -- check each keyword that is present in vlist to see if associated value type is allowed -- also check for duplicate kws, etc. 
            // ------------------------------------------------------------------------------------------------------------------------------------------------
        
            for (const auto& elem : vlist.eval_kws)                  // look at each keyword in vlist
            {
                std::wstring kw_find_name   { };                     // name used to look up keywords in plist keyword list 

                if (plist.no_check_keyword_names)                    // don't check keyword names?
                   kw_find_name = L"";                               // use dummy substitute name for entry in kw map
                else
                   kw_find_name = elem.first;                        // look for this keyword name from parms vlist 

                auto key_pct = plist.keywords.count(kw_find_name);   // find "*" or this vlist keyword in list of allowed keywords for this plist      

                if (key_pct == 0)                                    // see if this keyword is not in the list of expected keywords for this vlist 
                {
                    count_error();
                    M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- unexpected %s keyword=%s: present") % verb_name(vexpr) % ws1 % ws2 % elem.first; 

                    msg_kw_loc(elem.second);
                    msgend_loc(vlist, vexpr);
                    rc = -1;               
                }
                else if (key_pct == 1)                               // this keyword or "*"found in list of expected keywords
                {
                    // check for duplicate keywords (or too many)    --  (missing kw, or too few) is checked later

                    auto kw_parmtype = plist.keywords.at(kw_find_name);      // get parmtype_S for this keyword in vlist, or "*" (if not validating keyword names)
                    auto kw_max_ct   = kw_parmtype.kw_max_ct;                // maximum allowed occurrences of this keyword 
                    auto kw_vct      = vlist.eval_kws.count(elem.first);     // get number of occurrences of this keyword in vlist (use real kw name -- assume supplied plist allows 0-nnnn occurrences, if names are not being checked)


                    M__(M_out(L"check_verb_vlist() -- ++++++++++++++++++++++++++++++++++ find_name = %S  located kw parmtype -- no_eval_ident=%s  no_eval_vexpr=%s  no_eval_vlist=%s")
                              % kw_find_name % M_bool_cstr(kw_parmtype.no_eval_ident) % M_bool_cstr(kw_parmtype.no_eval_vexpr) % M_bool_cstr(kw_parmtype.no_eval_vlist)
                        ;
                       )

                    if ( (kw_vct > kw_max_ct) && (kw_done.find(elem.first) == kw_done.end()) )
                    {
                        if (kw_max_ct == 1)
                        {
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- %s keyword=%s: present %d times -- only once is allowed") % verb_name(vexpr) % ws1 % ws2 % elem.first % kw_vct; 
                            msg_kw_loc(elem.second);
                            msgend_loc(vlist, vexpr); 
                            rc = -1;
                        }
                        else
                        {
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- %s keyword=%s: present %d times -- maximum of %d times is allowed") % verb_name(vexpr) % ws1 % ws2 % elem.first % kw_vct % kw_max_ct; 
                            msg_kw_loc(elem.second);
                            msgend_loc(vlist, vexpr); 
                            M_out_emsgz();
                            rc = -1;
                        }
                    }

                    kw_done.insert(elem.first);                          //mark this one done, so as not to complain multiple times 
                   

                    // check located parmtype_S for this verb against actual value for keyword that is present in verb parms
      
                     auto crc = check_verb_value(frame, vexpr, kw_parmtype, elem.second, ws1 + ws2 + L" keyword=" + elem.first + L":"); 
                     if (crc != 0) rc = crc;               
                }
                else    // should not occur
                {
                    M_throw_v("check_verb_vlist() --  verb= %s %s -- more than one entry in %s expected keyword list -- keyword=%s verb=%s") % out_ws(verb_name(vexpr)) % out_ws(ws1) %  out_ws(ws2) % out_ws(elem.first) % out_ws(verb_name(vexpr)) ));
                    msgend_loc(vlist, vexpr); 
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
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- missing required %s keyword=%s:") % verb_name(vexpr) % ws1 % ws2 % elem.first;
                            msgend_loc(vlist, vexpr); 
                            M_out_emsgz();
                            rc = -1;                 
                        }
                        else if (kw_ct < min_ct)
                        {
                            M__(M_out(L"check_verb_vlist() -- not enough kws");)
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s  -- %s keyword=%s: is present only %d time(s) -- minimum required times is %d") % verb_name(vexpr) % ws1 % ws2 % elem.first % kw_ct % min_ct;
                            msgend_loc(vlist, vexpr); 
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
                     int          conflict_ct  {0};               // number of conflicting keywords found in this conflict set
                     std::wstring conflict_str { };               // list of conflicting keywords found in this set (for error message)


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
                        M_out_emsg1(L"check_verb_vlist() -- verb= %s %s  --  conflicting %s keywords found -- %s") % verb_name(vexpr) % ws1 % ws2 % conflict_str;
                        msgend_loc(vlist, vexpr); 
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
                     int          choice_ct  {0};               // number of choice keywords found in this choice set
                    
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

                        std::wstring choice_str { };               // list of choice keywords found in this set (for error message)

                        for (const auto cs_kw_name : choice_set)
                            choice_str += L" " + cs_kw_name + L": "; 


                        // put out error message

                        count_error();

                        if (choice_ct == 0)
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s  --  one of the following %s keywords must be present -- %s")     % verb_name(vexpr) % ws1 % ws2 % choice_str;
                        else
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s  --  only one of the following %s keywords can be present -- %s") % verb_name(vexpr) % ws1 % ws2 % choice_str;

                        msgend_loc(vlist, vexpr); 
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
                     std::wstring               found_str       { };         // printable list of matching keywords found in this set (for error message)
                     std::wstring               required_str    { };         // printable list of keywords required in match set  
                     std::vector<uint64_t>      kw_counts       { };         // list of actual keyword occurrence counts in vlist 
                     std::vector<std::wstring>  kw_names        { };         // names of keywords found in vlist 
                     uint64_t                   found_ct        {0};         // number of keywords in match set that are present in vlist
                     uint64_t                   required_ct     {0};         // total number of keywords present in match set


                    // inner loop to check each keyword in the match set (compute required_ct = number of kws in match set, found_ct = number of these KWs in vlist)

                    for (const auto& kw_name : match_set)
                    {
                        required_ct ++;                                      // increment number of keywords present
                        required_str += L" " + kw_name + L": ";              // add this keyword to the required string 
                        
                        auto kw_count = vlist.eval_kws.count(kw_name);       // number of times this keyword is present in vlist 

                        if (kw_count > 0)                                    // is this keyword in vlist ?
                        {
                            found_ct++;                                      // increment count of keywords present
                            found_str += L" " + kw_name + L": ";             // add this keyword to the found string 
                            kw_names .push_back(kw_name);                    // save this kw name in vector 
                            kw_counts.push_back(kw_count);                   // save this kw count in vector 
                        }
                    }


                    // if at least one keyword in the match set was present, need to check counts, etc. (all keywords missing is OK, as far as this check goes)

                    if (found_ct > 0)
                    {
                        if (found_ct != required_ct)                         // see if any keywords in the match set are missing   
                        {
                            M__(M_out(L"check_verb_vlist() -- match kws -- some kws missing");)
                            count_error();
                            M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- %s keyword match set=(%s) -- only some of these keywords are present: (%s)") % verb_name(vexpr) % ws1 % ws2 % required_str % found_str;
                            msgend_loc(vlist, vexpr); 
                            rc = -1;  
                        }
                        else                                                         // right number of keywords (all) are present 
                        {
                            auto minmax_ct = std::minmax_element(kw_counts.cbegin(), kw_counts.cend());

                            M__(M_out(L"check_verb_vlist() -- *minmax_ct.first = %d,  *minmax_ct.second = %d") % *(minmax_ct.first) % *(minmax_ct.second); )

                            if ( *(minmax_ct.first) != *(minmax_ct.second) )        // do keywords appear a different number of times in the vlist?  
                            {
                                // build string with varying counts, for error message -- kw_counts and kw_names should be the same size == found_ct == required_count

                                std::wstring counts_str { }; 

                                for (auto i = 0; i < found_ct; i++)
                                    counts_str += L" " + kw_names.at(i) + L":" + std::to_wstring(kw_counts.at(i)) + L"x "; 
                                
                                M__(M_out(L"check_verb_vlist() -- match kws -- different occurrences");)
                                count_error();
                                M_out_emsg1(L"check_verb_vlist() -- verb= %s %s -- %s keyword match set=(%s) -- differing number of occurrences: (%s) -- number of occurrences must all be the same") % verb_name(vexpr) % ws1 % ws2 % required_str % counts_str;
                                msgend_loc(vlist, vexpr); 
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
////   check_verb_parms() -- complain and return error, if parms in vexpr don't match what is expected in the verbdef_S structure
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int check_verb_parms(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef) try
{
    int rc {0}; 


    // check both left and right vlists vs plist requirements in passed-in vexpr

    auto crc1 = check_verb_vlist(frame, vexpr, verbdef.lparms, vexpr.lparms, L"", L"left-side" );  
    auto crc2 = check_verb_vlist(frame, vexpr, verbdef.rparms, vexpr.rparms, L"", L"right-side");
    if (crc1 != 0) rc = crc1; 
    if (crc2 != 0) rc = crc2;


    // global check for parms_some_required -- at least one positional parm on one side or the other 

    if (verbdef.parms_some_required)
    {
        if ( (vexpr.lparms.value_ct == 0) && (vexpr.rparms.value_ct == 0) )     // both sides have no positional parms ?  
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects at least one positional parm on one side or the other, but none were present at all") % verb_name(vexpr); 
            msgend_loc(vexpr); 
            rc = -1;           
        }
    }


    // global check for parms_not_both_sides -- cannot have positional or *keyword* parms on both sides 

    if (verbdef.parms_not_both_sides)
    {
        if ( ((vexpr.lparms.value_ct + vexpr.lparms.kw_ct) > 0) && ((vexpr.rparms.value_ct + vexpr.rparms.kw_ct) > 0) )     // both sides have positional or keyword parms ?  
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  does not expect positional or keyword parms on both left and right sides, but both left-side and right-side parms were present") % verb_name(vexpr); 
            msgend_loc(vexpr); 
            rc = -1;           
        }
    }


    // global check for parms_left_xor_right -- positional parms must exist on exactly one side (not both sides or neither side) 

    if (verbdef.parms_left_xor_right)
    {
        if ( (vexpr.lparms.value_ct == 0) && (vexpr.rparms.value_ct == 0) )     // both sides have no positional parms ?  
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects positional parms on one side or the other, but none were present on either side") % verb_name(vexpr); 
            msgend_loc(vexpr); 
            rc = -1;           
        }
        else if ( (vexpr.lparms.value_ct > 0) && (vexpr.rparms.value_ct > 0) )    // positional parms on both sides ?
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects positional parms on only one side or the other, but parms were present on both sides") % verb_name(vexpr);
            msgend_loc(vexpr); 
            rc = -1;           
        }    
    }

   
    // global check for parms_same_type - make sure all parms in both left-side and right_side positional parm lists are the same type 

    if (verbdef.parms_same_type)
    {
        if ( 
            (vexpr.rparms.val_mixed)
            ||
            (vexpr.lparms.val_mixed)
            ||
            (
             ( (vexpr.lparms.value_ct > 0) && (vexpr.rparms.value_ct > 0) ) 
             &&
             (
              (vexpr.lparms.val_unit           !=  vexpr.rparms.val_unit        )
              ||
              (vexpr.lparms.val_boolean        !=  vexpr.rparms.val_boolean     )
              ||
              (vexpr.lparms.val_int8           !=  vexpr.rparms.val_int8        )
              ||
              (vexpr.lparms.val_int16          !=  vexpr.rparms.val_int16       )
              ||
              (vexpr.lparms.val_int32          !=  vexpr.rparms.val_int32       )
              ||
              (vexpr.lparms.val_int64          !=  vexpr.rparms.val_int64       )
              ||
              (vexpr.lparms.val_uint8          !=  vexpr.rparms.val_uint8       )
              ||
              (vexpr.lparms.val_uint16         !=  vexpr.rparms.val_uint16      )
              ||
              (vexpr.lparms.val_uint32         !=  vexpr.rparms.val_uint32      )
              ||
              (vexpr.lparms.val_uint64         !=  vexpr.rparms.val_uint64      )
              ||
              (vexpr.lparms.val_float32        !=  vexpr.rparms.val_float32     )
              ||
              (vexpr.lparms.val_float64        !=  vexpr.rparms.val_float64     )
              ||
              (vexpr.lparms.val_string         !=  vexpr.rparms.val_string      )
              ||
              (vexpr.lparms.val_identifier     !=  vexpr.rparms.val_identifier  )
              ||
              (vexpr.lparms.val_vlist          !=  vexpr.rparms.val_vlist       )
              ||
              (vexpr.lparms.val_vexpr          !=  vexpr.rparms.val_vexpr       )
              ||
              (vexpr.lparms.val_slist          !=  vexpr.rparms.val_slist       )
              ||
              (vexpr.lparms.val_verbdef        !=  vexpr.rparms.val_verbdef     )
            )
           )
          )
        {
            count_error(); 
            M_out_emsg1(L"check_verb_parms() -- verb= %s  expects all positional parms (both left-side and right-side) to be the same type, but more than one type was present") % verb_name(vexpr); 
            msgend_loc(vexpr); 
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

void display_stack(const frame_S& frame) try
{
    const frame_S *frame_p  { &frame };                                        // point to passed-in frame_S as starting point in stack display


    // 1st loop to display all stack frames in the upward scope -- follow upward scope chain from passed-in frame_S 
    // ------------------------------------------------------------------------------------------------------------

    M_out(L"display_stack() -- upward scope stack frame chain, starting with passed-in stack frame\n");

    while (frame_p != nullptr)
    {
        std::wstring flags    { };
        std::wstring verbname { };

        if (frame_p->exports_done       ) flags += L"exports done  "       ;
        if (frame_p->is_verb            ) flags += L"is_verb "             ;  
        if (frame_p->is_block           ) flags += L"is_block "            ; 
        if (frame_p->is_main            ) flags += L"is_main  "            ; 
        if (frame_p->symtab_valid       ) flags += L"symtab_valid  "       ; 
        if (frame_p->lexical_scope      ) flags += L"lexical_scope "       ;
        if (frame_p->dynamic_scope      ) flags += L"dynamic_scope "       ;
        if (frame_p->block_scope        ) flags += L"block_scope "         ;

        M__(M_out(L"display_stack() -- verbanme.size() = %d") % frame_p->verbname.size();)

        if (frame_p->verbname.size() > 0)
            verbname = std::wstring {L"verbname= <"} + frame_p->verbname +  std::wstring {L">"};

        M_out(  L"frame_S:%-10d -- use=%-2d addr=%p  parent:%-10d child:%-10d main:%-10d symtab:%-10d scope:%-10d -- called=%-10d symbols=%-5d -- flags: %S  %S" )
             %  frame_p->serial 
             %  frame_p->self_wp.use_count()
             % frame_p 
             % (frame_p->parent_sp.get() == nullptr ? 0 :  frame_p->parent_sp -> serial ) 
             % (frame_p->child_p         == nullptr ? 0 :  frame_p->child_p   -> serial )
             % (frame_p->main_p          == nullptr ? 0 :  frame_p->main_p    -> serial )
             % (frame_p->symbols_p       == nullptr ? 0 :  frame_p->symbols_p -> serial )
             % (frame_p->scope_sp.get()  == nullptr ? 0 :  frame_p->scope_sp  -> serial )
             %  frame_p->verb_eval_ct
             %  frame_p->symtab.symbols.size()
             %  flags
             %  verbname
             ;        
    

        // chain upward to next scope

        if (frame_p->scope_sp.use_count() == 0)                      // don't advance past main stack frame (which should have an uninitialized  upward pointer)
           frame_p = nullptr;                                        // no more frame_S's for this loop
        else                                                     
           frame_p = frame_p->scope_sp.get();                        // advance upward to next frame_S that has a symbol table (every frame_S has a synmbol table, for now)
    }


    // 2nd loop to display all active stack frames on the stack -- follow forward chain from main frame_S 
    // --------------------------------------------------------------------------------------------------
    
    M_out(L"\ndisplay_stack() -- active stack forward chain, starting with main stack frame\n");

   frame_p = get_main_frame();                                              // start at main frame_S

    while (frame_p != nullptr)
    {
        std::wstring flags    { }; 
        std::wstring verbname { };

        if (frame_p->exports_done       ) flags += L"exports done  "       ;
        if (frame_p->is_verb            ) flags += L"is_verb  "            ;
        if (frame_p->is_block           ) flags += L"is_block "            ;
        if (frame_p->is_main            ) flags += L"is_main  "            ; 
        if (frame_p->symtab_valid       ) flags += L"symtab_valid  "       ; 
        if (frame_p->lexical_scope      ) flags += L"lexical_scope "       ;
        if (frame_p->dynamic_scope      ) flags += L"dynamic_scope "       ;
        if (frame_p->block_scope        ) flags += L"block_scope "         ;

        if (frame_p->verbname.size() > 0)
            verbname = std::wstring {L"verbname= <"} + frame_p->verbname +  std::wstring {L">"}; 

        M_out(  L"frame_S:%-10d -- use=%-2d addr=%p  parent:%-10d child:%-10d main:%-10d symtab:%-10d scope:%-10d -- called=%-10d symbols=%-5d -- flags: %S  %S" )
             %  frame_p->serial
             %  frame_p->self_wp.use_count()
             % frame_p 
             % (frame_p->parent_sp.get() == nullptr ? 0 :  frame_p->parent_sp -> serial ) 
             % (frame_p->child_p         == nullptr ? 0 :  frame_p->child_p   -> serial )
             % (frame_p->main_p          == nullptr ? 0 :  frame_p->main_p    -> serial )
             % (frame_p->symbols_p       == nullptr ? 0 :  frame_p->symbols_p -> serial )
             % (frame_p->scope_sp.get()  == nullptr ? 0 :  frame_p->scope_sp  -> serial )
             %  frame_p->verb_eval_ct
             %  frame_p->symtab.symbols.size()
             %  flags
             %  verbname
             ;       
    
       frame_p = frame_p->child_p;                                          // chain downward to child frame_S 
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
////   main_eval() -- special set up main stack frame, etc.
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void main_eval(frame_S& main_frame, int argc, wchar_t *argv[]) try
{
    // stack frame has already been set up with flags, pointers, symbol table, global table, etc. (basic setup)

    // add command line args to top-level stack frame right-side positional parm vector (vlist will have no location data)

    for (auto i = 0; i < argc; i++) 
        add_positional_value( main_frame.rparms, string_val(argv[i]) ); 
    
    return; 
}
M_endf





////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_frame() -- alocate new stack frame and add it to to top of stack 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

frame_S *add_frame(bool new_symtab) try
{
    // save away pointer to current newest stack frame (called "old frame" here) -- will go out of scope when this routine returns 

    std::shared_ptr<frame_S> old_frame_sp { static_N::newest_frame_sp };     // hold onto current newest stack frame until back chain sahred ptr is set in new stack frems  


     // allocate new frame_S, and point static newest frame shared pointer to it  

    std::shared_ptr<frame_S> new_frame_sp { new frame_S { } };              // allocate new stack frame for running this verb (this shared ptr will go out of scope when routine returns)
    static_N::newest_frame_sp = new_frame_sp;                               // this will hold onto new stack frame after shared pointer in autodata go out of scope 


    // fix up various pointers in new frame_S being added to top of stack
    // ------------------------------------------------------------------

    new_frame_sp->self_wp         = new_frame_sp;                           // set weak self-ptr for use_count() debugging and later shared_ptr initialization 
    new_frame_sp->global_p        = get_global_symtab();                    // set new frame's global symbol table pointer       

    if (old_frame_sp.use_count() > 0)                                       // are we adding 1st stack frame?
    {
        // adding regular (not main) stack frame
            
        new_frame_sp->parent_sp   = old_frame_sp;                           // set parent pointer in new stack frame  (owning pointer) 
        new_frame_sp->main_p      = old_frame_sp->main_p;                   // copy old frame's main_p to new frame_S
        old_frame_sp->child_p     = new_frame_sp.get();                     // set new frame_S as old frame's child   (non-owning pointer)
    }
    else
    {
        new_frame_sp->main_p      =  new_frame_sp.get();                    // this is main stack frame -- points to itself -- has no parent pointer  
        new_frame_sp->is_main     = true;                                   // this is main stack frame   
    }

    
    // set unique serial number in new frame

    new_frame_sp->serial          = ++static_N::frame_serial;               // set unique serial number in new frame_S     
    static_N::frame_depth++;                                                // maintain frame depth counter
    static_N::frame_max_depth     = std::max(static_N::frame_depth, static_N::frame_max_depth); 

    // set up to use symbol table in new frame_S as the local symbol table, -or- inherit current one from parent frame_S

    if (new_symtab)
    {
        new_frame_sp->symtab_valid = true;                                   // indicate that symbol table in this frame_S is to be used as newest local symbol table 
        new_frame_sp->symbols_p    = new_frame_sp.get();                     // set pointer to stack frame with local symbol table to the new frame_S (new set of local variables)                            
    }                                                                    
    else                                                                     // continue to use current symbol table as the local one
    {   
        if (old_frame_sp.use_count() > 0)                                    // don't look at old frame, if none -- assume that main stack frame being added has a valid symbol table
            new_frame_sp->symbols_p = old_frame_sp->symbols_p;               // copy parent's symbols_p to new frame_S -- don't activate symbol table in new frame_S
    }                                                                    

    return new_frame_sp.get();                                               // return plain pointer to newly-allocated stack frame
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
    std::shared_ptr<frame_S> removed_frame_sp { static_N::newest_frame_sp   }  ;     // shared ptr to hold onto stack frame being removed
    std::shared_ptr<frame_S> kept_frame_sp    { removed_frame_sp->parent_sp }  ;     // shared ptr to hold onto stack frame being kept (2nd oldest)


    M__(M_out(L"remove_frame() -- called:  removed_frame_p = %p   kept_frame_p = %p") % removed_frame_sp.get() % kept_frame_sp.get();)



  // kept_frame_sp->verb_eval_ct += removed_frame_sp->verb_eval_ct;  // add removed frame's verb count to kept's (statistical counters)


    // remove newest stack frame from top of stack  (leave any scoping pointers alone -- these may hold onto removed stack frame in case any closures point to it directly or indirectly)
    
    if (kept_frame_sp.use_count() > 0)                                                               // is there a kept stack frame (i.e. removing main stack frame this time)
        kept_frame_sp->child_p = nullptr;                                                            // kept stack frame is now childless

    removed_frame_sp->parent_sp.reset();                                                             // get rid of removed stack frame's parent pointer (kept frame will not go away, since this routine still holds a shared pointer for it)
    static_N::newest_frame_sp = kept_frame_sp;                                                       // update newest stack frame pointer (this will hold onto newest stack (if any are left on stack) frame after this routine returns)
    static_N::frame_depth--;                                                                         // maintain frame depth counter 

    return; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_block() -- add new frame_S to stack for this block,  and run passed-in slist with that frame_S using passed-in vlists as parms
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int eval_block(frame_S& parent_frame, const vlist_S& left_vlist, const vlist_S& right_vlist, const slist_S& slist, results_S& results) try
{
    // add new stack frame to the active frame_S queue

    frame_S *new_frame_p = add_frame();  


    // upward scope for a block frame_S is always starts at parent stack frame -- scoping is always lexical

    new_frame_p->is_block   = true;                                              // indicate this stack frame was created by block evaluation
    new_frame_p->block_scope = true;                                             // blocks always have unrestricted dynamic (lexical, effectively) scope upwards to parent  
    new_frame_p->scope_sp = std::shared_ptr<frame_S> { parent_frame.self_wp };   // initialize shared ptr from weak self-ptr in parent frame_S


    // set up block invocation parms in new stack frame
    
    new_frame_p->lparms =  left_vlist; 
    new_frame_p->rparms = right_vlist;


    // run block's slist under new stack frame -- remove frame_S when done

    results_S block_results {};
    auto erc = eval_slist(*new_frame_p, slist, block_results, true);
    remove_frame();                                                            // remove new stack frame from stack 
    if (erc != 0)                                                           
    {                                                                       
        results = error_results();                                             // results = error   
        return erc;                                                            // return with error r/c
    }


    //  handle @QUIT special results -- percolate any others that get percolated here  

    if (block_results.quit_flag)
    {
        results = unit_results();                                               // pass back unit results -- @QUIT special results are cosnumed 
        return 0; 
    }
                          

    // pass back results (including special results other than @QUIT) from slist evaluation

    results = block_results; 
    return 0; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_verb_block() -- add new frame_S to stack for this verb block,  and run passed-in slist with that new stack frame, using vlists in vexpr as the block parms
////                         
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int eval_verb_block(frame_S& parent_frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // add new stack frame to the active frame_S queue (and do basic setup)

    frame_S *new_frame_p = add_frame(); 


    // parent scope for a verb frame_S is parent stack frame (dynamic scope) -or- stack frame that defined the verb (lexical scope)  

    new_frame_p->dynamic_scope = verbdef.dynamic_scope;
    new_frame_p->lexical_scope = verbdef.lexical_scope;
    new_frame_p->block_scope   = verbdef.block_scope;

    if (verbdef.lexical_scope)
        new_frame_p->scope_sp = verbdef.upward_scope_sp;                                                 // start upward scope at stack frame that defined the verb, for lexical scoping
    else                                                                                                 // dynamic_scope   or   block_scope
        new_frame_p->scope_sp = std::shared_ptr<frame_S> { parent_frame.self_wp };                       // initialize shared ptr from weak sefl-ptr in parent frame_S -- upward scope starts at parent on call stack, for dynamic or block scoping  


    // set up new stack frame for this block, using parms from the passed-in vexpr
    // --------------------------------------------------------------------------

    new_frame_p->is_verb  = true;                                                                        // flag this frame_S as started by verb 
    new_frame_p->verbname = vexpr.verb_name; 
    new_frame_p->lparms   = vexpr.lparms;                                                                          
    new_frame_p->rparms   = vexpr.rparms;                                                                          
                                           

    // -------------------------------------                                                                                                         
    // set up argvars (if any) in new frame_S                                                                  
    // -------------------------------------                                                                  
                                                                                                             
    value_S unit_value { unit_val() };                                                                    // unit value, in case arg corresponding to variable name is not present
                                                                                                             
                                                                                                             
    // process left-side positional args  -- loop over variable names in left-side argvars list, assigining unit value or value of passed-in corresponding arg 
    // --------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    //    note: new_eval should be empty, and there should be no duplicates in the variable names list, so def_local_var() should not fail
                                                                                                             
    int var_n = 0;                                                                                        // left-side variable number     
                                                                                                             
    for (auto elem : verbdef.lvars.pos)                                                                   // elem is a std::wstring = variable name 
    {                                                                                                        
        int drc { };                                                                                      // return code from def_local_var
                                                                                                             
        if (var_n >= vexpr.lparms.value_ct)                                                               // this variable is past end of args for this invocation 
            drc = def_local_var(*new_frame_p, elem, unit_value,                    false, false, true);   // define local non-exported variable with unit value -- make sure values are unshared
        else
            drc = def_local_var(*new_frame_p, elem, vexpr.lparms.values.at(var_n), false, false, true);    // define local non-exported variable with value of passed-in n-th positional arg -- make sure values are unshared 
        

        // return immediately with error, if def_local_var() failed

        if (drc != 0)                                                                                     // did def_local_var() fail? -- not expected
        {
            count_error(); 
            M_out_emsg1(L"eval_verb_block() -- left-side positional argvar -- unexpected error from def_local_var(,%s,)") % elem; 
            msgend_loc(vexpr); 

            results = error_results();                                                                    // results = error
            return -1;                                                                                    // r/c     = error
        }  

        var_n++;                                                                                          // advance to next value in positional invocation parms 
    }
    

    // process right-side positional args  -- loop over variable names in right-side argvars list, assigining unit value or value of passed-in corresponding arg 
    // ---------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    //    note: new_eval should be empty, and there should be no duplicates in the variable names list, so def_local_var() should not fail
                                                                                                             
    var_n = 0;                                                                                            // right-side variable number     
                                                                                                         
    for (auto elem : verbdef.rvars.pos)                                                                   // elem is a std::wstring = variable name 
    {                                                                                                    
        int drc { };                                                                                      // return code from def_local_var
                                                                                                         
        if (var_n >= vexpr.rparms.value_ct)                                                               // this variable is past end of args for this invocation
            drc = def_local_var(*new_frame_p, elem, unit_value,                    false, false, true);   // define local non-exported variable with unit value -- make sure values are unshared
        else
            drc = def_local_var(*new_frame_p, elem, vexpr.rparms.values.at(var_n), false, false, true);   // define local non-exported variable with value of passed-in n-th positional arg  -- make sure values are unshared
        

        // return immediately with error, if def_local_var() failed

        if (drc != 0)                                                                                     // did def_local_var() fail? -- not expected
        {
            count_error(); 
            M_out_emsg1(L"eval_verb_block() -- right-side positional argvar -- unexpected error from def_local_var(,%s,)") % elem; 
            msgend_loc(vexpr); 

            results = error_results();                                                                    // results = error
            return -1;                                                                                    // r/c     = error
        }  

        var_n++;                                                                                          // advance to next value in positional invocation parms 
    }


    // process left-side keyword args  -- loop over keyword/variable names in left-side argvars list, assigining unit value or value of passed-in corresponding keyword arg 
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    //    note: new_eval should be empty, and there should be no duplicates in the variable names list, so def_local_var() should not fail
    
    std::multiset<std::wstring>  lkw_names { };                                                           // multimap with keywords processes so far  

    for (auto elem : verbdef.lvars.key)                                                                   // elem.first is a std::wstring = keyword name -- elem.second is a std::wstring = variable name 
    {                                                                                                    
        lkw_names.insert(elem.first);                                                                     // add this keyword name to combined list of keywords seen so far
        auto kwvar_ct = lkw_names.count(           elem.first);                                           // get count of how many times this keyword name has been seen so far in argvar_S
        auto kwval_ct = vexpr.lparms.eval_kws.count(elem.first);                                          // get count of how many times this keyword name is seen in the invocation parms 

        M__(M_out(L"eval_verb_block() -- left-side : kw_name = «%S»  kwvar_ct=%d  kwval_ct=%d") % elem.first % kwvar_ct % kwval_ct;) 
  
        int drc { };                                                                                     // return code from def_local_var

        if (kwvar_ct > kwval_ct)                                                                         // this kw/variable pair is past end of kw/value pairs in invocation args
            drc = def_local_var(*new_frame_p, elem.second, unit_value, false, false, true);              // define local non-exported variable with unit value -- make sure values are unshared
        else
            drc = def_local_var(*new_frame_p
                               , elem.second
                               , multimap_at(vexpr.lparms.eval_kws, elem.first, kwvar_ct-1)               // find value for n-th occurrence of this keyword in invocation keyword values multimap
                               , false, false, true
                               );                                                                         // define local non-exported variable with value of passed-in n-th positional arg -- make sure values are unshared
        

        // return immediately with error, if def_local_var() failed

        if (drc != 0)                                                                                     // did def_local_var() fail? -- not expected
        {
            count_error(); 
            M_out_emsg1(L"eval_verb_block() -- left-side keyword(%S) argvar -- unexpected error from def_local_var(,%S,)") % elem.first % elem.second; 
            msgend_loc(vexpr); 

            results = error_results();                                                                    // results = error
            return -1;                                                                                    // r/c     = error
        }  
    } 
      

    // process right-side keyword args  -- loop over keyword/variable names in right-side argvars list, assigining unit value or value of passed-in corresponding keyword arg 
    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //
    //    note: new_frame should be empty, and there should be no duplicates in the variable names list, so def_local_var() should not fail
    
    std::multiset<std::wstring>  rkw_names { };                                                           // multimap with keywords processes so far  

    for (auto elem : verbdef.rvars.key)                                                                   // elem.first is a std::wstring = keyword name -- elem.second is a std::wstring = variable name 
    {                                                                                                    
        rkw_names.insert(elem.first);                                                                     // add this keyword name to combined list of keywords seen so far
        auto kwvar_ct = rkw_names.count(           elem.first);                                           // get count of how many times this keyword name has been seen so far in argvar_S
        auto kwval_ct = vexpr.rparms.eval_kws.count(elem.first);                                           // get count of how many times this keyword name is seen in the invocation parms 

        M__(M_out(L"eval_verb_block() -- right-side : kw_name = «%S»  kwvar_ct=%d  kwval_ct=%d") % elem.first % kwvar_ct % kwval_ct;) 
  
        int drc { };                                                                                      // return code from def_local_var

        if (kwvar_ct > kwval_ct)                                                                          // this kw/variable pair is past end of kw/value pairs in invocation args
            drc = def_local_var(*new_frame_p, elem.second, unit_value, false, false, true);               // define local non-exported variable with unit value -- make sure values are unshared
        else
            drc = def_local_var(*new_frame_p
                               , elem.second
                               , multimap_at(vexpr.rparms.eval_kws, elem.first, kwvar_ct-1)                // find value for n-th occurrence of this keyword in invocation keyword values multimap
                               , false, false, true
                               );                                                                         // define local non-exported variable with value of passed-in n-th positional arg -- make sure values are unshared
        

        // return immediately with error, if def_local_var() failed

        if (drc != 0)                                                                                     // did def_local_var() fail? -- not expected
        {
            count_error(); 
            M_out_emsg1(L"eval_verb_block() -- right-side keyword(%S) argvar -- unexpected error from def_local_var(,%S,)") % elem.first % elem.second; 
            msgend_loc(vexpr); 

            results = error_results();                                                                    // results = error
            return -1;                                                                                    // r/c     = error
        }  
    } 


    // --------------------------------------------------------
    // run slist in new stack frame -- remove frame_S when done
    // --------------------------------------------------------

    results_S slist_results {};
    auto erc = eval_slist(*new_frame_p, *(verbdef.slist_sp), slist_results, true);
    remove_frame();                                                                                       // remove new frame from stack 
    if (erc != 0)                                                                                       
    {                                                                                                   
        results = error_results();                                                                        // results = error   
        return erc;                                                                                       // return with error r/c
    }


    // handle any special results -- expect @END, @THROW, @XCTL, and @RETURN -- do not expect uncaught @LEAVE, @GOTO, @CONTINUE, @BREAK, @QUIT, etc.

    if (slist_results.special_results)
    {
        if (slist_results.return_flag)                                                                    // @RETURN is expected -- neeeds to be consumed 
        {                                                                                                 
             slist_results.return_flag     = false;                                                       // consume the @RETURN         
             slist_results.special_results = false;                                                       
             results = slist_results;                                                                     // pass back value from @RETURN, with no special flags        
        } 
        else if (slist_results.xctl_flag)                                                                 // @XCTL is expected -- needs to be consumed 
        {   
             M__(M_out(L"eval_verb_block() -- saw @XCTL results -- verbdef.is_builtin = %d") % ((int)(verbdef.is_builtin));)
             results = slist_results;                                                                     // percolate @XCTL results    
        }
        else if (slist_results.end_flag)                                                                  // @END is expected
        {                                                                                                 
             results = slist_results;                                                                     // percolate @END results           
        }                                                                                                 
        else if (slist_results.throw_flag)                                                                // @THROW is expected
        {                                                                                                 
             results = slist_results;                                                                     // percolate @THROW results           
        }                                                                                                 
        else                                                                                              // unexpected special results
        {              
            if (slist_results.goto_flag)
                M_out_emsg(L"User-defined verb evaluation ended by unconsumed @GOTO «%s»") % slist_results.str;             
       
            else if (slist_results.leave_flag)
                M_out_emsg(L"User-defined verb evaluation ended by unconsumed @LEAVE «%s»") % slist_results.str;            
           
            else if (slist_results.break_flag)
                M_out_emsg(L"User-defined verb evaluation ended by unconsumed @BREAK"); 
       
            else if (slist_results.continue_flag)
                M_out_emsg(L"User-defined verb evaluation ended by unconsumed @CONTINUE");
            
            else if (slist_results.quit_flag)                                     
                M_out_emsg(L"User-defined verb evaluation ended by unconsumed @QUIT");  
          
            else if (slist_results.error)                                                   
                M_out_emsg(L"User-defined verb evaluation ended by error");

            else
                M_out_emsg(L"User-defined verb evaluation ended by unexpected special results");

            count_error(); 
            results = error_results();                                                                    // error results
            return -1;                                                                                    // return with error 
        }                                                                                                
    }                                                                                                    
    else                                                                                                  // slist not ended by @RETURN etc. - return value is not set 
    {                                                                                                    
        results = slist_results;                                                                          // pass back normal slist results
    }                                                                                                    
                                                                                                         
    return 0;                                                                                             // return normally -- results have already been passed-back
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_main_block() -- using pre-built main frame_S, run passed-in slist (parms already set up for main frame_S) -- no results passed back, just r/c
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int eval_main_block(frame_S& frame, const slist_S& slist) try
{
    // run slist under new block frame -- remove frame_S when done

    results_S block_results {};
    auto erc = eval_slist(frame, slist, block_results, true);
    if (erc != 0) return erc; 


    // handle any percolated special results from main slist evaluation

    if (block_results.special_results)
    {
        if (block_results.end_flag)      // @END flag is supposed to be percolated here for action
           return 0; 
    
        if (block_results.quit_flag)     // @QUIT flag (from inside main block) is supposed to be percolated here for action
           return 0; 
         

        // main frame_S block expects no unmatched @GOTOs, @LEAVEs, @CONTINUEs, @BREAKs, @RETURNs, @THROWs, etc.
       
        if (block_results.goto_flag)
        {
            M_out_emsg(L"main slist evaluation ended by unconsumed @GOTO «%s»  -- apparently @GOTO target label was not found") % block_results.str; 
            count_error(); 
            return -1;
        }
       
        if (block_results.return_flag)
        {
            M_out_emsg(L"main slist evaluation ended by unconsumed @RETURN %s -- apparently @RETURN was issued in main slist -- use @QUIT instead") % str_value(block_results, true, false, true); 
            count_error();
            return -1;
        }

        if (block_results.xctl_flag)
        {
            M_out_emsg(L"main slist evaluation ended by unconsumed @XCTL -- apparently @XCTL was issued in main slist"); 
            count_error();
            return -1;
        }   
       
        if (block_results.throw_flag)
        {
            M_out_emsg(L"main slist evaluation ended by uncaught @THROW %s") % str_value(block_results, true, false, true); 
            count_error();
            return -1;
        }
         
        if (block_results.leave_flag)
        {
            M_out_emsg(L"main slist evaluation ended by unconsumed @LEAVE «%s»  -- apparently @LEAVE was issued outside of any block") % block_results.str; 
            count_error();
            return -1;
        }
         
        if (block_results.break_flag)
        {
            M_out_emsg(L"main slist evaluation ended by unconsumed @BREAK -- apparently @BREAK was issued outside of any loop");
            count_error();
            return -1;
        }
       
        if (block_results.continue_flag)
        {
            M_out_emsg(L"main slist evaluation ended by unconsumed @CONTINUE -- apparently @CONTINUE was issued outside of any loop"); 
            count_error();
            return -1;
        }
       
        if (block_results.error)
        {
            M_out_emsg(L"main slist evaluation ended by error");
            count_error();
            return -1;
        }

        M_out_emsg(L"main slist evaluation ended by unexpected special results"); 
        count_error();
        return -1; 
    }
            
    return 0;            // main slist evaluation results are ignored
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
////   eval_value() -- evaluate contents of an passed-in value -- value should be either identifier or vexpr
////                -- passed-in value parm is replaced by the value (which can be another vexpr or variable -- no double evaluation -- they are left as-is)
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// internal routine to call eval_value in a loop, as long as re_eval_vexpr_results flag is on
// -----------------------------------------------------------------------------------------

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

        if (results.re_eval_vexpr_results)
            in_value = results;                                         // copy results over to input area for next call 


        loop_count++;
        if ( debug && (loop_count > 1) )
        {
            M_y(M_out(L"reeval_value() -- loop count = %d") % loop_count;)
        }
    }
    while (results.re_eval_vexpr_results);

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
        M_y(M_out(L"*************************************** eval_value() -- called  -- parmtype.no_eval_ident=%s  parmtype.no_eval_vexpr=%s  parmtype.no_eval_vlist=%s") % M_bool_cstr(eval_parmtype.no_eval_ident) % M_bool_cstr(eval_parmtype.no_eval_vexpr) % M_bool_cstr(eval_parmtype.no_eval_vlist);)
        M_y(display_value(value, L"eval_value()");)
    }

    static_N::value_count++; 


    // evaluate nested vexpr, if required 
    // ---------------------------------

    if ( (value.ty == type_E::vexpr) && (!eval_parmtype.no_eval_vexpr) )
    {

        if (debug)
        {
            M_y(M_out(L"*************************************** eval_value() -- evaluating nested vexpr");)
        }

        results_S vexpr_results { }; 

        auto erc = eval_vexpr(frame, *(value.vexpr_sp), vexpr_results, debug);   // pass vexpr attached to this value
        vexpr_results.token_ix1 = value.token_ix1;                               // copy original location info into results
        vexpr_results.token_ix2 = value.token_ix2;                             
        if (erc != 0)                                                            // error during vexpr evaluation
        {
            results = vexpr_results;                                             // pass back results from vexpr evaluation
            rc = erc;                                                            // pass back r/c
        } 
        else                                                                     // no error during vexpr evaluation
        {
             if (debug)
             {
                 M_y(M_out(L"*************************************** eval_value() -- vexpr_results.ty=%S -- returned from nested expression evaluation") % type_str(vexpr_results.ty);)
             }

             if (                                                                           
                (vexpr_results.ty == type_E::identifier)                                    // re-evaluate, if results is an identifier 
                || 
                (vexpr_results.ty == type_E::vlist)                                         // re-evaluate, if results is a vlist
                ||
                ( (vexpr_results.ty == type_E::ref) && (vexpr_results.ref_sp->auto_deref) )  // re-evaluate, if results is a reference, that should be automatically de-referenced                 
               ) 
            {
                results                      = vexpr_results;                  // return (preliminary) results from vexpr evaluation
                results.re_eval_vexpr_results = true;                          // indicate that re-evaluation of these results is required 
            }
            else                                                               // no further evaluation needed
            {
                results                    = vexpr_results;                    // return results from vexpr evaluation
                results.suppress_eval_once = false;                            // make sure no_eval_once flag is not passed back up
            }   
        }
    }


    // evaluate nested vlist, if required -- need to allocate a new one and attach it to the value -- in nested vlists, vexprs, and variables always evaluated (according to flags in default plist_S )
    // -------------------------------------------------------------------------------------------

    else if ( (value.ty == type_E::vlist) && (!eval_parmtype.no_eval_vlist) )
    {  
        if (debug)
        {
            M_y(M_out(L"*************************************** eval_value() -- evaluating nested vlist");)
        }
        
        vlist_S temp_vlist {};
        temp_vlist = *(value.vlist_sp);                                        // can't update vlist pointed to by passed-in value -- need a new copy for updating
        

        if (eval_parmtype.plist_sp.get() == nullptr )                          // nested plist not present? 
        {
            auto erc = eval_vlist(frame, temp_vlist, results, plist_S {});     // use default plist_S structure for nested vlist
            if (erc != 0) rc = erc;
        }
        else
        {
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

    else if ( (value.ty == type_E::identifier) && (!eval_parmtype.no_eval_ident) )
    {
        if (debug)
        {
            M_y(M_out(L"*************************************** eval_value() -- evaluating identifier");)
        }

        auto grc = get_identifier(frame, value.string, results);
        results.token_ix1 = value.token_ix1;                     // save original location info into passed-in results
        results.token_ix2 = value.token_ix2;

        if (debug)
        {
            M_y(display_value(value  , L"eval_value() -- input value");)
            M_y(display_value(results, L"eval_value() -- evaluated identifier ");)
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

    else if ( (value.ty == type_E::ref) && (value.ref_sp->auto_deref) && (!eval_parmtype.no_eval_ref) )
    {
        if (debug)
        {
            M_y(M_out(L"*************************************** eval_value() -- doing auto dereference");)
        }

        value_S out_val { };                                             // output value_S from dereferenced value
        auto drc = dereference_value(out_val, value);
         
        if (debug)
        {
            M_y(display_value(value  , L"eval_value() -- input reference value");)
            M_y(display_value(out_val, L"eval_value() -- dereferenced value ");)
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
        // other values don't need to be evaluated -- int, float, string, slist, unit, verbname etc. -- just copy value to output results

        if (debug)
        {
            M_y(M_out(L"*************************************** eval_value() -- copying value to output results");)
        }
        results = to_results(value); 
    }

    if (debug)
    {
        M_y(M_out(L"********* eval_value() returning -- re-eval = %s <=========================") % M_bool_cstr(results.re_eval_vexpr_results);)
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

    if ( vlist.val_vexpr || vlist.val_vlist || vlist.val_identifier || vlist.val_ref )
    {
       M__(M_out(L"eval_vlist() -- evaluation is needed");)

       // build a temporary default parmtype_S with global evaluation flags -- used if no individual parmtype_S is available for positional parms (this should be the case only when vexpr has no verb)  

       parmtype_S default_eval_parmtype {};                                  // default  parmtype 

       default_eval_parmtype.no_eval_ident  = eval_plist.no_eval_ident;      // copy in eval flag
       default_eval_parmtype.no_eval_vexpr  = eval_plist.no_eval_vexpr;      // copy in eval flag
       default_eval_parmtype.no_eval_vlist  = eval_plist.no_eval_vlist;      // copy in eval flag
       default_eval_parmtype.anything_ok    = true;                          // suppress parm type checking by default  


       // -------------------------------------------------------------------------------------------
       // loop to look at each positional parm and do evaluation (subject to verb's evaluation flags)
       // -------------------------------------------------------------------------------------------

        for (int i = 0; i < vlist.values.size(); i++)
        {
            M__(M_out(L"eval_vlist() -- loop -- i = %d    vlist.values.size() = %d" ) % i % vlist.values.size();)

            results_S val_results { }; 

            if ( (vlist.values.at(i).ty == type_E::vexpr) || (vlist.values.at(i).ty == type_E::vlist) || (vlist.values.at(i).ty == type_E::identifier) || (vlist.values.at(i).ty == type_E::ref) )
            {                 
                if (eval_plist.values.empty())                            // no plist_S available? 
                {
                    M__(M_out(L"--------------------------------------->eval_vlist() calling eval_value() -- positional -- default_parmype_S");) 
                    auto erc = eval_value(frame, vlist.values.at(i), val_results, default_eval_parmtype);
                    M__(M_out(L"--------------------------------------->eval_vlist() eval_value() returned");)
                    if (erc != 0)  rc = erc;                              // remember any error     
                }
                else                                                      // some plist_S are available in values vector -- use i-th parmtype_S or last one, if not enough)               
                {
                    M__(M_out(L"--------------------------------------->eval_vlist() calling eval_value() -- positional -- supplied parmtype_S");) 
                    auto erc = eval_value(frame, vlist.values.at(i), val_results, eval_plist.values.at(std::min(i, (int)(eval_plist.values.size() - 1))) );
                    M__(M_out(L"--------------------------------------->eval_vlist() eval_value() returned");)
                    if (erc != 0)  rc = erc;                              // remember any error
                }    
            }
            else                                                          // not vexpr, vlist, or identifier that requires evaluation
            {
                val_results = to_results(vlist.values.at(i));             // just copy value from vlist directly into results -- no need for evaluation
            }

            M__(M_out(L"eval_vlist() -- inside loop -- looking at results -- i = %d    vlist.values.size() = %d" ) % i % vlist.values.size();)


            // ------------------------------------------------------------
            // look at results from evaluating this vexpr/value in the vlist 
            // ------------------------------------------------------------  

            // return immediately, if any special flags on in results 

            if (val_results.special_results)
            {
                vlist.values.at(i) = val_results;                                 // update value in_place in vlist, from values imbedded in results
                results = val_results;                                            // pass back results with special flags
                return rc;                                                        // return with any errors
            }                                                         


            // handle usual (single) results from evaluation
            // ---------------------------------------------

            if (!val_results.multiple_results)
            {
                vlist.values.at(i) = val_results;                                 // update value in_place in vlist, from values imbedded in results
                M__(M_out(L"eval_vlist() -- (only) value.ty %S %S") % type_str(val_results.ty) % val_results.string;)

                if (val_results.re_eval_vexpr_results)
                    i--;                                                         // go back and re-evaluate results of vexpr value evaluation
            }                                                   


            // handle multiple results  -- can be 0, 1, or more values in the multiple results vlist
            // -------------------------------------------------------------------------------------

            else
            {
                 M__(M_out(L"eval_vlist() -- multiple results --  before -- i = %d   vlist.values.size() = %d" ) % i % vlist.values.size();)

                auto     vlist_it   = vlist.values.begin() + i;                                                // get iterator pointing to current (i-th) element in the positional values vector (guaranteed to exist at this point)
                uint64_t n_results { 0 };                                                                      // number of positional value results -- set to 0 in case vlist_sp is not initialized    
                uint64_t n_keys    { 0 };                                                                      // number of keyword value    results -- set to 0 in case vlist_sp is not initialized    
                
                if (val_results.vlist_sp.use_count() > 0)                                                      // vlist_sp points to vlist (with multiple values)? 
                {
                    n_results = val_results.vlist_sp->values.size();                                           // number of positional results -- can be 0, 1, 2, ...
                    n_keys    = val_results.vlist_sp->keywords.size();                                         // number of keyword results - most likely 0 
                    M__(M_out(L"eval_vlist() -- n_results = %d   n_keys = %d" ) % n_results % n_keys;)
                }
                                                                                                              
                if (n_results <= 0)                                                                            // no results from vexpr evaluation?  
                {     
                    vlist.values.erase(vlist_it);                                                              // just remove vexpr from vlist (don't replace with anything)
                    i--;                                                                                       // reduce loop index, which will be incremented later to point to new (slid-down) element now in i-th vector position
                    vlist.value_ct--;                                                                          // also reduce number of values present in this vlist
                }                                                                                             
                else if (n_results == 1)                                                                       // just one value in multiple results vlist? -- should not occur?? 
                {                                                                                            
                    vlist.values.at(i) = val_results.vlist_sp->values.at(0);                                   // replace vexpr in i-th value with the first/only value from the vexpr evaluation results 
                    M__(M_out(L"eval_vlist() -- (multiple=one) value.ty %S %S") % type_str(val_results.vlist_sp->values.at(0).ty) % val_results.vlist_sp->values.at(0).string;)
                    i--;                                                                                       // multiple results may need to be re-evaluated, since eval_value() doesn't do it (since it doesn't have access to n+1-th parmtype) 
                }                                                                                            
                else                                                                                           // actually have multiple results from vexpr evaluation
                {                                                                                            
                    vlist.values.at(i) = val_results.vlist_sp->values.at(0);                                   // replace vexpr in i-th value with the first value from the vexpr evaluation results
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


                // append any keywords (in unevaluated list) in results to unevaluated keyword list in this vlist -- kw evaluation has not started yet for this vlist, so these should be picked up down below

                if (n_keys > 0)
                {
                    auto saved_kw_ct { vlist.kw_ct };

                    vlist.keywords.insert(vlist.keywords.end(), val_results.vlist_sp->keywords.cbegin(), val_results.vlist_sp->keywords.cend()); // append all unevaluated keyword names/values from v2 (if any) to output vlist
                    vlist.kw_ct = vlist.keywords.size(); 

                    M__(M_out(L"eval_vlist() -- vlist kw_ct:  before=%d  after=%d") % saved_kw_ct % vlist.kw_ct;)
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
         for (auto& elem : vlist.keywords)
         {
             value_S        keyword_value { };
             std::wstring   keyword_name  { };    
       

             // process simple keyword name or evaluate keyword name value, if required
             // -----------------------------------------------------------------------

             if (elem.name.ty == type_E::keyname)                                         // handle simple keyword name value in this keyword_S
             {
                 keyword_name = elem.name.string;                                         // keyword name is in string field   
             }                                                                            
             else                                                                         // keyword name expression requires evaluation
             { 
                 M__(M_out(L"eval_vlist() -- evaluating keyword name expression");)

                 // need to handle non-simple keyword name -- evaluate value and check for keyword type or string type that can be converted to keyword_name string

                 results_S  keyname_results  { };                                        // results from verbname value evaluation 
                 parmtype_S keyname_parmtype { };                                        // eval_value() looks at only "no_eval" flags, which are off with default initialization

                 auto evrc = reeval_value(frame, elem.name, keyname_results, keyname_parmtype, debug);
                 if (evrc != 0) rc = evrc;

                 if (keyname_results.special_results)
                 {
                     results = keyname_results;                                          // pass back any unusual results from keyword name value evaluation
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
       
             if  ( (elem.value.ty == type_E::vexpr) || (elem.value.ty == type_E::vlist) || (elem.value.ty == type_E::identifier) || (elem.value.ty == type_E::ref) )
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
                     if (val_results.vlist_sp.get() == nullptr)              // if no vlist, assume number of results = 0
                     {
                          M__(M_out(L"eval_vlist() -- handling multiple results (without vlist) from keyword value eval_value() call");)  
                          keyword_value = unit_val();                        // no multiple results (?? should not occur??) -- just use unit keyword value                      
                     }
                     else if (val_results.vlist_sp->kw_ct > 0) 
                     {
                         M_out_emsg1(L"eval_vlist() -- evaluation of vexpr supplied as keyword value returned with %d keywords -- none are allowed in a keyword value") % val_results.vlist_sp->kw_ct; 
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
                         M_out_emsg1(L"eval_vlist() -- evaluation of vexpr supplied as keyword value returned with %d positional values -- max of one is allowed in a keyword value") % val_results.vlist_sp->value_ct; 
                         msg_loc(elem.value, L"keyword value");
                         msgend_loc(vlist);
                         results = error_results();                          // pass back error results
                         return -1;   
                     }
                 }
                 else                                                        // must be single results -- not special
                 {
                      M__(M_out(L"eval_vlist() -- handling single (non-special) from keyword value expression evaluation");)  
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
         
         updated            = true;     // indicate that vlist has been updated
         vlist.kw_eval_done = true;     // indicate that eval_kws has been filled in
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

    M__(M_out(L"=========================================> eval_vlist() returning");)
    M__(display_vlist(vlist, L"output from eval_vlist()");)
    return rc; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_vexpr() -- evaluate contents of a passed-in vexpr (values passed-in vexpr are updated in-place)
////                  
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   eval_vexpr() -- called in a loop from eval_vexpr()
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                                                                                  
static 
int eval_vexpr_1(frame_S& frame, const a_vexpr_S& orig_vexpr, results_S& results, bool debug) try
{
    e_vexpr_S     eval_vexpr     {       }; 
    int           rc             { 0     };  
    bool          lparms_updated { false }; 
    bool          rparms_updated { false };    

    M__(M_out(L"*****************************************************eval_vexpr() called");)
    M__(display_vexpr(orig_vexpr, L"eval-before", L"", false);)


    // copy data members from original_vexpr to new vexpr
    // ------------------------------------------------

    eval_vexpr.has_verb         =  orig_vexpr.has_verb         ;
    eval_vexpr.has_sigil        =  orig_vexpr.has_sigil        ;
    eval_vexpr.sigil            =  orig_vexpr.sigil            ;
    eval_vexpr.priority         =  orig_vexpr.priority         ;
    eval_vexpr.right_associate  =  orig_vexpr.right_associate  ;
    eval_vexpr.left_associate   =  orig_vexpr.left_associate   ;
    eval_vexpr.token_ix1        =  orig_vexpr.token_ix1        ;
    eval_vexpr.token_ix2        =  orig_vexpr.token_ix2        ;
    eval_vexpr.lparms           =  orig_vexpr.lparms           ;       // nested complex values (pointed to by std::shared_ptr<> ) are shared with orig a_vexpr_S
    eval_vexpr.rparms           =  orig_vexpr.rparms           ;       // nested complex values (pointed to by std::shared_ptr<> ) are shared with orig a_vexpr_S


    // =====================================================
    // get verbdef, etc, (if verb is present for this vexpr)
    // =====================================================

    verbdef_S verbdef         {       };                                             // default verbdef_S, in case verb expression is empty string
    bool      have_verbdef    { false };                                             // set to true, once non-empty verbdef found
    
    if (orig_vexpr.has_verb)
    {
        // set evaluated verb name in e_vexpr_S -- either direct from a_vexpr_S verb_value string value, or by evaluation of a_vexpr_S verb_value expression

        if (orig_vexpr.verb_value.ty == type_E::verbname)                              // handle simple verbname in this vexpr
        {
            eval_vexpr.verb_name      = orig_vexpr.verb_value.string;                  // simple verbname -- name is just string value 
            eval_vexpr.verb_token_ix1 = orig_vexpr.verb_value.token_ix1;               // copy over token indexes for debugging
            eval_vexpr.verb_token_ix2 = orig_vexpr.verb_value.token_ix2; 
        }                                                                            
        else                                                                           // verb expression requires evaluation
        { 
            // need to handle non-simple verbname -- evaluate value and check for verbdef or string that can be converted to verb_name string
            // ------------------------------------------------------------------------------------------------------------------------------

            results_S  verbname_results  { };                                          // results from verbname value evaluation  (final)
            parmtype_S verbname_parmtype { };                                          // eval_value() uses only "no_eval" flags, which are off by default

            auto evrc = reeval_value(frame, orig_vexpr.verb_value, verbname_results, verbname_parmtype, false);

            if (evrc != 0) rc = evrc;

            if (verbname_results.special_results)
            {
                results = verbname_results;                                               // pass back unusual results from verb_value evaluation
                return rc;                       
            }  

            if (evrc != 0)
            {
                M_out_emsg1(L"eval_vexpr() -- expression evaluation to obtain verbname/verbdef failed -- orig_vexpr.verb_value = %s -- cannot execute") % verb_name(orig_vexpr); 
                msgend_loc(orig_vexpr); 
                results = error_results();                                                // pass back error results
                return -1;              
            }


            // get string for verb name, if evaluation results are string or verbname -- all other types not accepted

            if ( (verbname_results.ty == type_E::verbname) || (verbname_results.ty == type_E::string) )
            {
                eval_vexpr.verb_name      = verbname_results.string;                      // save away evaluated string
                eval_vexpr.verb_token_ix1 = orig_vexpr.verb_value.token_ix1;              // copy over token indexes for debugging
                eval_vexpr.verb_token_ix2 = orig_vexpr.verb_value.token_ix2; 
            }
            else if (verbname_results.ty == type_E::verbdef)
            {
                verbdef = *(verbname_results.verbdef_sp);                                 // just capture verbdef from verbname vexpr evaluation
                have_verbdef = true;                                                      // suppress verbdef lookup down below, and indicate verbdef is already available
                eval_vexpr.verb_token_ix1 = orig_vexpr.verb_value.token_ix1;              // copy over token indexes for debugging
                eval_vexpr.verb_token_ix2 = orig_vexpr.verb_value.token_ix2; 
                M__(M_out(L"eval_vexpr() -- verbdef results");)
            }
            else
            {
                 M_out_emsg1(L"eval_vexpr() -- orig_vexpr.verb_value evaluation did not yield a string, verbname, or verbdef -- orig_vexpr.verb_value = %s  -- verbname_results.ty = %S -- cannot execute") % verb_name(orig_vexpr) % type_str(verbname_results.ty); 
                 msgend_loc(orig_vexpr); 
                 results = error_results();                                               // pass back error results
                 return -1;  
            }
        }
   

        // get verbdef for verb_name (if any) to see what type of variable and vexpr evaluation is needed in left and right parms -- end immediately, if unknown verb

        if ( (!have_verbdef) && (eval_vexpr.verb_name.size() > 0) )                        // need to lookup verbdef -and- evaluated verb_name expression is non-empty string? 
        {                                                                                 
            auto verbdef_rc = get_verb(frame, eval_vexpr.verb_name, verbdef);              // returns default, if undefined verb  
           
            if (verbdef_rc != 0)                                                           // undefined verb? 
            {
                M_out_emsg1(L"eval_vexpr() -- unknown verb = %s -- cannot execute") % eval_vexpr.verb_name; 
                msgend_loc(eval_vexpr); 
                results = error_results();                                                  // pass back error results
                return -1; 
            }  

            have_verbdef = true;                                                            // indicate that verbdef is now available 
        }
    }
    else                                                                                    // vexpr has no verb -- need to suppress all evaluation
    {
        verbdef.lparms.no_eval_ident  = true;                                               // suppress vexpr and vlist  evaluating -- just pass through values
        verbdef.lparms.no_eval_vlist  = true;    
      //verbdef.lparms.no_eval_vexpr  = true;                                               // allow nested vexprs to be evaluated           
    }


    //  Process any nested vexprs in left or right vlists -- replace vexpr with value -- updated value can be another vexpr or variable -- no double evaluation -- these will be left as-is here (verb checking may complain about them) 
    //  -------------------------------------------------

    if ( (eval_vexpr.lparms.value_ct > 0) || (eval_vexpr.lparms.kw_ct > 0) )
    {
        M__(M_out(L"eval_vexpr() -- process left vlist (lparms)");) 
        results_S vlist_results {}; 
        auto erc = eval_vlist(frame, eval_vexpr.lparms, vlist_results, verbdef.lparms);
        if (erc != 0) rc = erc;

        if (vlist_results.special_results)
        {
            results = vlist_results;       // pass back unusual results from vlist evaluation
            return rc;                       
        }
    }

    if ( (eval_vexpr.rparms.value_ct > 0) || (eval_vexpr.rparms.kw_ct > 0) )
    {
        M__(M_out(L"eval_vexpr() -- process right vlist (rparms)");) 
        results_S vlist_results {}; 
        auto erc = eval_vlist(frame, eval_vexpr.rparms, vlist_results, verbdef.rparms);
        if (erc != 0) rc = erc;

        if (vlist_results.special_results)
        {
            results = vlist_results;       // pass back unusual results from vlist evaluation
            return rc;                       
        }
    }
    
    M__(display_vexpr(eval_vexpr, L"eval-after", L"", false);)


    // don't evaluate the main verb, if any errors seen so far during vlist evaluation

    if (rc != 0)
    {
        M_out_emsg1(L"eval_vexpr() -- bypassing execution of verb= %s  because of errors while evaluating parms") % verb_name(eval_vexpr);
        msgend_loc(eval_vexpr); 
        results = error_results();                                             // pass back error results
        return rc; 
    }


    // do preliminary parm checking based on verbdef_S for main verb (if any)

     
    if (eval_vexpr.has_verb && have_verbdef)                                   // bypass parm checking if no verbdef is available
    {
      M__(M_out(L"eval_vexpr() -- doing parm checking -- calling check_verb_parms()");)
      auto crc = check_verb_parms(frame, eval_vexpr, verbdef);
      if (crc != 0) rc = crc; 
    }


    // don't evaluate the main verb, if any errors seen so far (during parm chedcking)

    if (rc != 0)
    {
        M_out_emsg1(L"eval_vexpr() -- bypassing execution of verb= %s  because of errors while checking basic parm counts and types") % verb_name(eval_vexpr); 
        msgend_loc(eval_vexpr); 
        results = error_results();                                             // pass back error results
        return rc; 
    }


    // =====================================
    // evaluate the main verb for this vexpr
    // =====================================

    if (eval_vexpr.has_verb)
    {
        // just return unit value, if no verbdef is available

        if (!have_verbdef)
        {
             results = unit_results();                                                  // return unit results ???????
        } 

        // otherwise, invoke the verb to get results
        // -----------------------------------------

        else                                                                            // evaluated verb name is not empty string
        {
            if (verbdef.fcn_p != nullptr)
            {
                if (verbdef.simplified_call)
                {
                    M__(M_out(L"eval_vexpr() -- doing simplified verb call <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)

                    // do simplified verb call -- non ex-non-internal predefined verbs
                    
                    int (*call_p)(const e_vexpr_S&, value_S&) = (int (*)(const e_vexpr_S&, value_S&))(verbdef.fcn_p);  // simplified call pointer
                    
                    value_S result_value {  };                                                                         // predefined uninitialized value   
                    auto prc = (*call_p)(eval_vexpr, result_value);                                                    // will pass back value_S converted to (non-special) result_S from verb call
                    results = to_results(result_value);                                                                // convert return value to full (non-special) results 
                                                                                                                     
                    if (prc != 0) rc = prc;                                                                            // remember any error from verb execution 
                }
                else
                {
                     M__(M_out(L"eval_vexpr() -- doing regular verb call <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)

                    // do regular verb call -- ex-internal predefined verbs

                    int (*call_p)(frame_S&, const e_vexpr_S&, const verbdef_S&, results_S&) = (int (*)(frame_S&, const e_vexpr_S&, const verbdef_S&, results_S&))(verbdef.fcn_p);  // regular call pointer
                            
                    auto prc = (*call_p)(frame, eval_vexpr, verbdef, results);           // will pass back result_S from verb call

                    if (prc != 0) rc = prc;                                              // remember any error from verb execution 
                }

                frame.verb_eval_ct++;                                                    // accumulate verb calls in current stack frame
                static_N::verb_count++;                                                  // accumulate total verb calls                  

                // Indicate that these results are from a builtin verb, if the verbdef indicates built-in verb     

                results.builtin_verb_results = verbdef.is_builtin;     
                M__(M_out(L"eval_vexpr() -- results.builtin_verb_results = %d <<<<<<<<<<<<<<<<<<<<<<<<<") % ((int)(results.builtin_verb_results));)                           
            }                                                                    
            else                                                                         // should not happen
            {
                M_throw_v("eval_vexpr() -- verbdef.fcn_p = nullptr for verb=%s") % out_ws(verb_name(eval_vexpr)) ));  
            } 
        }
    }


    // no verb present -- should be empty parens, of parens with just left-side positional values present -- no keywords allowed 
    // -------------------------------------------------------------------------------------------------------------------------

    else                                                                               
    {
        if (
        //  (eval_vexpr.lparms.value_ct   > 1)
        //  ||                         
            (eval_vexpr.rparms.value_ct   > 0)                                           // no right-side positional parms allowed without verb   (should not happen)
            ||
        //    (eval_vexpr.lparms.kw_ct      > 0)                                         // no left-side keywords          allowed without verb
        //    ||
            (eval_vexpr.rparms.kw_ct      > 0)                                           // no right-side keywords         allowed without verb   (should not happen)
           )
        {
            // invalid verbless vexpr -- error -- should not occur -- error should heve been found during parsing phase before evaluation started

            M_out_emsg1(L"eval_vexpr() -- verbless vexpr with right-side parms seen -- should not occur");
            msgend_loc(eval_vexpr);
            M_throw("eval_vexpr() -- verbless vexpr with multiple parms")         
        }
        else
        {
            M__(M_out(L"eval_vexpr() -- handling no-verb case <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////
            //////
            //////     ====================
            //////     handle no-verb vexpr -- just empty parens or vlist with left-side positional parm(s)  
            //////     ====================
            //////
            //////
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            M__(M_out(L"eval_vexpr() -- lparms.value_ct=%d   lparms.kw_ct=%d   rparms.value_ct=%d   rparms.kw_ct=%d ") % eval_vexpr.lparms.value_ct % eval_vexpr.lparms.kw_ct % eval_vexpr.rparms.value_ct % eval_vexpr.rparms.kw_ct;)

            if  (eval_vexpr.lparms.value_ct == 0)                          // just parens -- no parm inside
            {
                results = no_results();                                    // zero result values -- vlist_sp is not initialized
            }
            else if (eval_vexpr.lparms.value_ct == 1)                      // value_ct = 1 -- must be paren enclosing a single value
            {
                results = to_results(eval_vexpr.lparms.values.at(0));      // pass back 1st (only) left-side positional parm 
            }
            else                                                           // must have more than one left-side positional parm 
            {
                results = to_results(vlist_val(eval_vexpr.lparms));        // pass back whole left-side positional parm vlist 
                results.multiple_results = true;                           // indicate that we're passing back multiple values in the vlist, not a single value that is the vlist
            }
        }
    }

    M__(M_out(L"*****************************************************eval_vexpr() returning");)
    return rc; 
}
M_endf
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   eval_vexpr() -- call eval_vexpr_1() in loop, as long as @XCTL results are seen from vexpr execution
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int eval_vexpr(frame_S& frame, const a_vexpr_S& vexpr, results_S& results, bool debug) try
{
    const a_vexpr_S     *vexpr_p    {&vexpr}; 
    results_S            results_x  {      };                         // @XCTL results from eval_vexpr_1() -- should have shared ptr to vexpr_S for next loop pass 


    // main loop to call eval_vexpr_1(), and repeat loop to handle any @XCTL results
    // -----------------------------------------------------------------------------

    for (;;)
    {
        results_S results_1   { };                                    // results from eval_vexpr_1()
                                                
        auto rc = eval_vexpr_1(frame, *vexpr_p, results_1, debug);
        M__(M_out(L"eval_vexpr() -- eval_vexpr_1() returned <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); )
      
        if (rc != 0)
        {
            results = results_1;                                      // pass back (error?) results from eval_vexpr_1() 
            return rc;                                                // pass back any errors from eval_vexpr_1()
        }
       
        if ( !(results_1.special_results && results_1.xctl_flag) )
        {
            M__(M_out(L"eval_vexpr() -- returning (non-@XCTL results  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); )
            results = results_1;                                      // pass back results from eval_vexpr_1()
            return rc;                                                // pass back all results, except for @XCTL special results
        }
                               
       
        // handle @XCTL results   
        
        if (results_1.builtin_verb_results == true)
        {
            M__(M_out(L"eval_vexpr() -- processing @XCTL results -- from built-in verb <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)
            results = results_1;                                      // pass back special @XCTL results from eval_vexpr_1() -- don't action this time -- wait for user's verb (non-builtin) to come back before doing the @XCTL
            return rc;                                                
        }

        M__(M_out(L"eval_vexpr() -- processing @XCTL results -- from user-defined verb <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");)   

        results_x = results_1;                                        // hold onto vexpr_S from @XCTL results via shared ptr until next call to eval_vexpr_1()
        vexpr_p   = results_x.vexpr_sp.get();                         // next pass -- use vexpr provided with with @XCTL results
       
       
        // (loop back to call eval_vexpr_1() with new vexpr setup by  the @XCTL verb, in place of current vexpr)
    }

}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_slist() -- evaluate all vexprs in an slist -- return results from last vexpr executed (if any)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int eval_slist(frame_S& frame, const slist_S& slist, results_S& results, bool is_block_slist) try
{
    M__(M_out(L"eval_slist() -- called -- is_block_slist = %d") % is_block_slist;)  
    int rc = 0; 
    results_S slist_results {}; 

    auto                vexpr_max   = slist.vexprs.size(); 
    decltype(vexpr_max) vexpr_ix    = 0; 

    M__(M_out(L"eval_slist() -- vexpr_max=%d") % vexpr_max;)
        
    static_N::slist_count++; 


    // filter out empty slist -- need to return 0 results from this (not just unit results)

    if (vexpr_max ==0)
    {
        results = no_results();                  // zero result values
        M__(M_out(L"eval_slist() -- multiple results = 0 results");)
        return 0; 
    }


    // --------------------------------------------------------------
    // main loop to process each statement (expression) in this slist
    // --------------------------------------------------------------

    for (;;)
    { 
        M__(M_out(L"eval_slist() -- top of for(;;) loop");)

        if (vexpr_ix >= vexpr_max)
            break; 

        static_N::statement_count++; 


        //   evaluate n-th vexpr in slist, and handle error r/c
        //   -------------------------------------------------

        slist_results = results_S { }; 

        M__(M_out(L"eval_slist() -- calling eval_vexpr(%d)") % vexpr_ix;)

        auto erc = eval_vexpr(frame, slist.vexprs.at(vexpr_ix), slist_results);
        if (erc != 0)
        {
            M_out_emsg1(L"eval_slist() -- slist evaluation ending immediately because of errors during expression evaluation at statement %d") % vexpr_ix;
            msgend_loc(slist.vexprs.at(vexpr_ix));
            slist_results = error_results();  // pass back error results
            rc = erc; 
            results = error_results(); 
            return -1; 
        }


        //   ======================
        //   handle special results -- @LEAVE and @GOTO and @XCTL are processed at the slist level -- may get consumed if label matches, or percolated otherwise
        //   ======================

        if (slist_results.special_results)
        {
            M__(M_out(L"eval_slist() -- handling special results");)

            //  handle     (plain) @LEAVE      -or-       @LEAVE "target slist"
            //  ---------------------------------------------------------------

            if (slist_results.leave_flag)
            {
                // if (plain) @LEAVE   -or-   @LEAVE target    matches this slist -- end processing of this slist  

                if ( slist_results.str.empty() || (slist_results.str == slist.label) )
                {
                    M__(M_out(L"eval_slist() -- @LEAVE target found in current slist -- ending slist with unit results");)
                    results = unit_results();                   // pass back unit results -- @LEAVE is consumed 
                    return 0;
                }
              
                
                // @LEAVE is not targetting the current slist -- need to "percolate" @LEAVE results upwards to enclosing slists (if any)
                
                M__(M_out(L"eval_slist() -- @LEAVE target not found in current slist -- percolating upwards");)
                results = slist_results; 
                break;                 
            }
        

            //  handle @GOTO
            //  ------------

            if (slist_results.goto_flag)
            {
                // see if  @GOTO target  matches some label in this slist  

                auto target_ct = slist.labels.count(slist_results.str);
                if (target_ct > 0)   
                {
                    // matching label found in this slist -- reset vexpr_ix to label position and continue looping (label may point past last vexpr in this slist
                    
                    vexpr_ix = slist.labels.at(slist_results.str);
                    M__(M_out(L"eval_slist() -- @GOTO target found in current slist -- restarting slist evaluation at vexpr index = %d") % vexpr_ix;)
                    continue;
                }


                //  @GOTO target was not found in this slist -- "percolate" @GOTO results upwards to enclosing slists (if any)   


                M__(M_out(L"eval_slist() -- @GOTO target not found in current slist -- percolating upwards");)
                results = slist_results; 
                break; 
            }


            // any other special results (@RETURN, @XCTL, etc.) will cause slist evaluation to end -- special results flags are passed back to slist evaluator for handling or further percolation

            M__(M_out(L"eval_slist() -- percolating special results upward");)
            results = slist_results;      // pass back special results   
            break; 
        }


        // no error -- no special results -- normal advance to next vexpr in slist

        vexpr_ix++; 
    }
  
    results = slist_results;       // return results from last vexpr evaluated -- no special results get here 
    M__(M_out(L"eval_slist() -- normal return at end");)
    return rc; 
}
M_endf



//_________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""