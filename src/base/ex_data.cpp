﻿// ex_data.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===========
////     ex_data.cpp -- data-oriented and type-oriented evaluation-time functions
////     ===========
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
#include "h_ex_parse.h"
#include "h_ex_verb.h"





////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   to_results() -- convert value to "plain" results
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
results_S to_results(const value_S& value) try
{
    results_S results { }; 

    *(value_S *)&results = value;               // copy value_S fields into the value_S base portion of the results structure
    return results; 
}
M_endf

/////////////////////  simple, hard-coded true/false results  ////////////////////////////////////////////


results_S  true_results(       ) {return to_results(type_val( true  ));} 
results_S false_results(       ) {return to_results(type_val( false ));} 
results_S    tf_results(bool tf) {return to_results(type_val( tf    ));} 



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   to_single_results() -- convert input results (may be multiple results) to "plain" results -- if possible  
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
int to_single_results(results_S& results) try
{
    // error R/C, if error results, special results, or no_results -- don't alter passed-in results value 

    if (results.error || results.special_results || results.no_results)
        return -1; 


    // if not multiple results, passed-in results are OK as-is -- no need to alter anything 

    if (!results.multiple_results)
        return 0; 


    // handle multiple results -- bad, if number of results is not 1, or there are keyword results in the vlist -- these results cannot be converted to "plain" results

    if  ((results.vlist_sp->value_ct != 1) || (results.vlist_sp->kw_ct != 0) )
        return -1;


    // single result in vlist, even though "multiple results" flag was on -- convert these results to "plain" results to be passed back
 
    results = to_results(results.vlist_sp->values.at(0));               // replace passed-in results with new "plain" results derived from 1st (only) value in vlist
    return 0; 
}
M_endf
   


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unit_results() -- return unit results_S 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

results_S unit_results() try
{
    results_S results { };
    results.ty = type_E::unit; 
    return results; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   error_results() -- return error results_S with the error and special flags set
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

results_S error_results() try
{
    results_S results { };
    results.ty              = type_E::error; 
    results.special_results = true;
    results.error           = true;

    return results; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   special_results() -- return spacial results_S with the special flag set  -- caller needs to finish setting up 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

results_S special_results() try
{
    results_S results { };
    results.ty              = type_E::special;
    results.special_results = true;

    // caller needs to finish setting this up

    return results; 
}
M_endf

  

////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   no_results() -- return zero result_S values 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

results_S no_results() try
{
    results_S results { }; 
    results.ty               = type_E::no_value;
    results.multiple_results = true; 
    results.no_results       = true;                       // no vlist_sp -- no empty vlist attached
    return results; 
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
////   xxxx_val() -- functions to construct new value from input parm    (also see type_val() below for overloaded function set)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// individual value-creating functions
// -----------------------------------


value_S unit_val( int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::unit; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////

value_S nval_val( int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::no_value;    // only for keywords without values 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////

value_S boolean_val(bool v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::boolean; 
    value.boolean       = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////

value_S int8_val(int8_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::int8; 
    value.int8          = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////

value_S int16_val(int16_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::int16; 
    value.int16         = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////////////////////////////////////

value_S int32_val(int32_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::int32; 
    value.int32         = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////

value_S int64_val(int64_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::int64; 
    value.int64         = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////////

value_S uint8_val(uint8_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::uint8; 
    value.uint8         = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////

value_S uint16_val(uint16_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::uint16; 
    value.uint16        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////////////////////////////////////

value_S uint32_val(uint32_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::uint32; 
    value.uint32        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////

value_S uint64_val(uint64_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::uint64; 
    value.uint64        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////////////

value_S float32_val(float32_T v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::float32; 
    value.float32       = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////////////

value_S float64_val(float64_T v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::float64; 
    value.float64       = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S string_val(const std::wstring& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::string; 
    value.string        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S string_val(const wchar_t *p, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::string; 
    value.string        = std::wstring(p); 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S identifier_val(const std::wstring& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::identifier; 
    value.string        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S identifier_val(const wchar_t *p, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::identifier; 
    value.string        = std::wstring(p);
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S verbname_val(const std::wstring& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::verbname; 
    value.string        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S verbname_val(const wchar_t *p, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::verbname; 
    value.string        = std::wstring(p);
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf





/////////////////////////////////////////////////////

value_S vlist_val(const vlist_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_vlist_value(value, v, false);      // std::move is not OK
    value.token_ix1 = v.token_ix1;
    value.token_ix2 = v.token_ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S expression_val(const a_expression_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_expression_value(value, v); 
    value.token_ix1 = v.token_ix1;
    value.token_ix2 = v.token_ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S block_val(const block_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_block_value(value, v, false);    // std::move is not OK
    value.token_ix1 = v.token_ix1;
    value.token_ix2 = v.token_ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S verbset_val(const verbset_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_verbset_value(value, v, false);    // std::move is not OK
    value.token_ix1 = ix1;
    value.token_ix2 = ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S typdef_val(const typdef_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_typdef_value(value, v, false);    // std::move is not OK
    value.token_ix1 = ix1;
    value.token_ix2 = ix2;
    return value; 
}
M_endf     


/////////////////////////////////////////////////////

value_S ref_val(const ref_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_ref_value(value, v, false);       // std::move is not OK
    value.token_ix1 = ix1;
    value.token_ix2 = ix2;
    return value; 
}
M_endf     


/////////////////////////////////////////////////////

value_S buffer_val(const buf8_T& v1, const typdef_S& v2, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_buffer_value(value, v1, v2, false);    // std::move is not OK
    value.token_ix1 = ix1;
    value.token_ix2 = ix2;
    return value; 
}
M_endf  

value_S buffer_val(const buf8_T& v1, const typdef_S& v2, bool move_ok) try
{
    value_S value {}; 

    set_buffer_value(value, v1, v2, move_ok);    // std::move is not OK
    value.token_ix1 = -1;
    value.token_ix2 = -1;
    return value; 
}
M_endf  



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_val() -- overloaded functions to extract C++ data from input value_S
////                   
////
////      r/c = 0    -- value is OK
////      r/c = -1   -- input type mismatch
////      r/c = -2   -- value in value_S is out of range for requested C++ value 
////      r/c = -3   -- should-not-occur situation has occurred
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


////////////////////////////////////////////////////////////////////
//  int8_t version
////////////////////////////////////////////////////////////////////
                  
int get_val(const value_S& v, int8_t& c) try  
{
    // immediately return any exact matching value from value_S 

    if (v.ty == type_E::int8)
    {
        c = v.int8;
        return 0;
    }


    // try to convert value to int64_t -- pass back any errors

    int64_t s { };  
    
    auto rc = get_val(v, s);

    if (rc != 0)
    {
        c = s; 
        return rc; 
    }


    // return error, if int64_t value is out of range

    if ((int8_t)s != s)
    {
        c = 0; 
        return -2; 
    }


    //  value must be OK

    c = (int8_t)s;
    return 0;
}
M_endf


////////////////////////////////////////////////////////////////////
//  int16_t version
////////////////////////////////////////////////////////////////////
                  
int get_val(const value_S& v, int16_t& c) try  
{
    // immediately return any exact matching value from value_S 

    if (v.ty == type_E::int16)
    {
        c = v.int16;
        return 0;
    }


    // try to convert value to int64_t -- pass back any errors

    int64_t s { };  
    
    auto rc = get_val(v, s);

    if (rc != 0)
    {
        c = s; 
        return rc; 
    }


    // return error, if int64_t value is out of range

    if ((int16_t)s != s)
    {
        c = 0; 
        return -2; 
    }


    //  value must be OK

    c = (int16_t)s;
    return 0;
}
M_endf


////////////////////////////////////////////////////////////////////
//  int32_t version
////////////////////////////////////////////////////////////////////
                  
int get_val(const value_S& v, int32_t& c) try  
{
    // immediately return any exact matching value from value_S 

    if (v.ty == type_E::int32)
    {
        c = v.int32;
        return 0;
    }


    // try to convert value to int64_t -- pass back any errors

    int64_t s { };  
    
    auto rc = get_val(v, s);

    if (rc != 0)
    {
        c = s; 
        return rc; 
    }


    // return error, if int64_t value is out of range

    if ((int32_t)s != s)
    {
        c = 0; 
        return -2; 
    }


    //  value must be OK

    c = (int32_t)s;
    return 0;
}
M_endf
 

//////////////////////////////////////////////////////////////////////////
//     int64_t version
//////////////////////////////////////////////////////////////////////////

int get_val(const value_S& v, int64_t& c) try  
{
    // immediately return any exact matching value from value_S 

    if (v.ty == type_E::int64)
    {
        c = v.int64;
        return 0;
    }


    // return with -1, if this value is not an integer type at all (includes boolean, unit, float) 

    if (!is_value_integer(v)) 
    {
        c = 0;   
        return -1;
    }


    //  if this is a signed value, just extract value into int64_t and pass back    

    if (is_value_signed(v))             // because of prior checks, must be int8, int16, or int32 
    {
        if (v.ty == type_E::int8  ) { c = (int64_t)(v.int8 ); return 0; }        
        if (v.ty == type_E::int16 ) { c = (int64_t)(v.int16); return 0; } 
                                      c = (int64_t)(v.int32); return 0; 
    }


    // for unsigned values, make sure thay are not too large for int64_t

    else                                // must be uint8, uint16, uint32, or uint64
    {
        uint64_t u{ }; 

             if (v.ty == type_E::uint8 ) u = (uint64_t)(v.uint8 );        
        else if (v.ty == type_E::uint16) u = (uint64_t)(v.uint16); 
        else if (v.ty == type_E::uint32) u = (uint64_t)(v.uint32); 
        else                             u =            v.uint64 ;

        // see if uint64_t value is too large for int64_t

        if ( (int64_t)u < 0)
        {
            c = 0; 
            return -2; 
        }

        // extrated value is OK

        c = u; 
        return 0; 
    } 

    return -3;                    // should not get here
}
M_endf


////////////////////////////////////////////////////////////////////
//  uint8_t version
////////////////////////////////////////////////////////////////////
                  
int get_val(const value_S& v, uint8_t& c) try  
{
    // immediately return any exact matching value from value_S 

    if (v.ty == type_E::uint8)
    {
        c = v.uint8;
        return 0;
    }


    // try to convert value to uint64_t -- pass back any errors

    uint64_t s { };  
    
    auto rc = get_val(v, s);

    if (rc != 0)
    {
        c = s; 
        return rc; 
    }


    // return error, if uint64_t value is out of range

    if ((uint8_t)s != s)
    {
        c = 0; 
        return -2; 
    }


    //  value must be OK

    c = (uint8_t)s;
    return 0;
}
M_endf


////////////////////////////////////////////////////////////////////
//  uint16_t version
////////////////////////////////////////////////////////////////////
                  
int get_val(const value_S& v, uint16_t& c) try  
{
    // immediately return any exact matching value from value_S 

    if (v.ty == type_E::uint16)
    {
        c = v.uint16;
        return 0;
    }


    // try to convert value to uint64_t -- pass back any errors

    uint64_t s { };  
    
    auto rc = get_val(v, s);

    if (rc != 0)
    {
        c = s; 
        return rc; 
    }


    // return error, if uint64_t value is out of range

    if ((uint16_t)s != s)
    {
        c = 0; 
        return -2; 
    }


    //  value must be OK

    c = (uint16_t)s;
    return 0;
}
M_endf


////////////////////////////////////////////////////////////////////
//  uint32_t version
////////////////////////////////////////////////////////////////////
                  
int get_val(const value_S& v, uint32_t& c) try  
{
    // immediately return any exact matching value from value_S 

    if (v.ty == type_E::uint32)
    {
        c = v.uint16;
        return 0;
    }


    // try to convert value to uint64_t -- pass back any errors

    uint64_t s { };  
    
    auto rc = get_val(v, s);

    if (rc != 0)
    {
        c = s; 
        return rc; 
    }


    // return error, if uint64_t value is out of range

    if ((uint32_t)s != s)
    {
        c = 0; 
        return -2; 
    }


    //  value must be OK

    c = (uint32_t)s;
    return 0;
}
M_endf


//////////////////////////////////////////////////////////////////////////
//     uint64_t version
//////////////////////////////////////////////////////////////////////////

int get_val(const value_S& v, uint64_t& c) try  
{
    // immediately return any exact matching value from value_S 

    if (v.ty == type_E::uint64)
    {
        c = v.uint64;
        return 0;
    }


    // return with -1, if this value is not an integer type at all (includes boolean, unit, float) 

    if (!is_value_integer(v)) 
    {
        c = 0;   
        return -1;
    }


    //  if this is an unigned value, extract value into uint64_t and pass back    

    if (is_value_unsigned(v))             // because of prior checks, must be uint8, uint16, or uint32 
    {
        if (v.ty == type_E::uint8  ) { c = (uint64_t)(v.uint8 ); return 0; }        
        if (v.ty == type_E::uint16 ) { c = (uint64_t)(v.uint16); return 0; } 
                                       c = (uint64_t)(v.uint32); return 0; 
    }


    // for signed values, make sure they are not negative

    else                                // must be int8, int16, int32, or int64
    {
        int64_t s{ }; 

             if (v.ty == type_E::int8 ) s = (int64_t)(v.int8 );        
        else if (v.ty == type_E::int16) s = (int64_t)(v.int16); 
        else if (v.ty == type_E::int32) s = (int64_t)(v.int32); 
        else                            s =           v.int64 ;


        // error, if int64_t value is negative

        if (s < 0)
        {
            c = 0; 
            return -2; 
        }


        // extrated value is OK -- return it as uint64_t

        c = (uint64_t)s; 
        return 0; 
    } 

    return -3;                    // should not get here
}
M_endf
 

////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   type_val() -- overloaded functions to construct new value from C++ input parm
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


value_S type_val(bool v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::boolean; 
    value.boolean       = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf

//////////////////////////////////////////////////////////////////

value_S type_val(int8_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::int8; 
    value.int8          = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////

value_S type_val(int16_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::int16; 
    value.int16         = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////////////////////////////////////

value_S type_val(int32_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::int32; 
    value.int32         = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////

value_S type_val(int64_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::int64; 
    value.int64         = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////////

value_S type_val(uint8_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::uint8; 
    value.uint8         = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////

value_S type_val(uint16_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::uint16; 
    value.uint16        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////////////////////////////////////

value_S type_val(uint32_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::uint32; 
    value.uint32        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


//////////////////////////////////////////////////////////////////

value_S type_val(uint64_t v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::uint64; 
    value.uint64        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////////////

value_S type_val(float32_T v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::float32; 
    value.float32       = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


///////////////////////////////////////////////

value_S type_val(float64_T v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::float64; 
    value.float64       = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S type_val(const std::wstring& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    value.ty            = type_E::string; 
    value.string        = v; 
    value.token_ix1     = ix1;
    value.token_ix2     = ix2;

    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S type_val(const vlist_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_vlist_value(value, v, false);      // std::move is not OK
    value.token_ix1 = v.token_ix1;
    value.token_ix2 = v.token_ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S type_val(const a_expression_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_expression_value(value, v); 
    value.token_ix1 = v.token_ix1;
    value.token_ix2 = v.token_ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S type_val(const block_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_block_value(value, v, false);    // std::move is not OK
    value.token_ix1 = v.token_ix1;
    value.token_ix2 = v.token_ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S type_val(const verbset_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_verbset_value(value, v, false);    // std::move is not OK
    value.token_ix1 = ix1;
    value.token_ix2 = ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S type_val(const typdef_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_typdef_value(value, v, false);    // std::move is not OK
    value.token_ix1 = ix1;
    value.token_ix2 = ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S type_val(const ref_S& v, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_ref_value(value, v, false);    // std::move is not OK
    value.token_ix1 = ix1;
    value.token_ix2 = ix2;
    return value; 
}
M_endf


/////////////////////////////////////////////////////

value_S type_val(const buf8_T& v1, const typdef_S& v2, int64_t ix1, int64_t ix2) try
{
    value_S value {}; 

    set_buffer_value(value, v1, v2, false);    // std::move is not OK
    value.token_ix1 = ix1;
    value.token_ix2 = ix2;
    return value; 
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
////   is_value_xxxx() -- return true if value type belongs to xxxx type values 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


/////////////  is_value_unit() ///////////////////// 

bool is_value_unit(const value_S& value) try
{
    M__(M_out(L"is_value_unit() -- value.ty = %d") % (int)(value.ty); )

    if (value.ty == type_E::unit)
    {
        M__(M_out(L"is_value_unit() -- return TRUE"); )
        return true;
    }
    else
    {
        M__(M_out(L"is_value_unit() -- return FALSE"); )
        return false; 
    }
}
M_endf
 


/////////////  is_value_boolean() ///////////////////// 

bool is_value_boolean(const value_S& value) try
{
    M__(M_out(L"is_value_boolean() -- value.ty = %d") % (int)(value.ty); )

    if (value.ty == type_E::boolean)
    {
        M__(M_out(L"is_value_boolean() -- return TRUE"); )
        return true;
    }
    else
    {
        M__(M_out(L"is_value_boolean() -- return FALSE"); )
        return false; 
    }
}
M_endf
 


/////////////  is_value_string() ///////////////////// 

bool is_value_string(const value_S& value) try
{
    M__(M_out(L"is_value_string() -- value.ty = %d") % (int)(value.ty); )

    if (value.ty == type_E::string)
    {
        M__(M_out(L"is_value_string() -- return TRUE"); )
        return true;
    }
    else
    {
        M__(M_out(L"is_value_string() -- return FALSE"); )
        return false; 
    }
}
M_endf
 
  

/////////////  is_value_integer() ///////////////////// 

bool is_value_integer(const value_S& value) try
{
    M__(M_out(L"is_value_integer() -- value.ty = %d") % (int)(value.ty); )

    if (
        (value.ty == type_E::int8  )  ||
        (value.ty == type_E::int16 )  ||
        (value.ty == type_E::int32 )  ||
        (value.ty == type_E::int64 )  ||
        (value.ty == type_E::uint8 )  ||
        (value.ty == type_E::uint16)  ||
        (value.ty == type_E::uint32)  ||
        (value.ty == type_E::uint64)
       )
    {
        M__(M_out(L"is_value_integer() -- return TRUE"); )
        return true;
    }
    else
    {
        M__(M_out(L"is_value_integer() -- return FALSE"); )
        return false; 
    }
}
M_endf



/////////////  is_value_signed() ///////////////////// 

bool is_value_signed(const value_S& value) try
{
    if (
        (value.ty == type_E::int8  )  ||
        (value.ty == type_E::int16 )  ||
        (value.ty == type_E::int32 )  ||
        (value.ty == type_E::int64 ) 
       )
        return true;
    else
        return false;  
}
M_endf



/////////////  is_value_unsigned() ///////////////////// 

bool is_value_unsigned(const value_S& value) try
{
    if (
        (value.ty == type_E::uint8  )  ||
        (value.ty == type_E::uint16 )  ||
        (value.ty == type_E::uint32 )  ||
        (value.ty == type_E::uint64 ) 
       )
        return true;
    else
        return false;  
}
M_endf




////////////  is_value_float() /////////////////////

bool is_value_float(const value_S& value) try
{
    if (
        (value.ty == type_E::float32) ||
        (value.ty == type_E::float64)
       )
        return true;
    else
        return false; 
}
M_endf




/////////////  is_value_arithmetic() ///////////////////// 

bool is_value_arithmetic(const value_S& value) try
{
    if (
        (value.ty == type_E::int8  )  ||
        (value.ty == type_E::int16 )  ||
        (value.ty == type_E::int32 )  ||
        (value.ty == type_E::int64 )  ||
        (value.ty == type_E::uint8 )  ||
        (value.ty == type_E::uint16)  ||
        (value.ty == type_E::uint32)  ||
        (value.ty == type_E::uint64)  ||
        (value.ty == type_E::float32) ||
        (value.ty == type_E::float64)
       )
        return true;
    else
        return false;  
}
M_endf



/////////////  is_value_comparable() ///////////////////// 

bool is_value_comparable(const value_S& value) try
{
    if (
        (value.ty == type_E::unit  )  ||
        (value.ty == type_E::boolean) ||
        (value.ty == type_E::string)  ||
        (value.ty == type_E::int8  )  ||
        (value.ty == type_E::int16 )  ||
        (value.ty == type_E::int32 )  ||
        (value.ty == type_E::int64 )  ||
        (value.ty == type_E::uint8 )  ||
        (value.ty == type_E::uint16)  ||
        (value.ty == type_E::uint32)  ||
        (value.ty == type_E::uint64)  ||
        (value.ty == type_E::float32) ||
        (value.ty == type_E::float64)
       )
        return true;
    else
        return false; 
}
M_endf



/////////////  is_value_false() ///////////////////// 

bool is_value_false(const value_S& value) try
{
    if (
        ( (value.ty == type_E::boolean)  &&  ( value.boolean == false) )
        ||
        ( (value.ty == type_E::int8   )  &&  ( value.int8    == 0    ) )
        ||                                                           
        ( (value.ty == type_E::int16  )  &&  ( value.int16   == 0    ) )
        ||                                                           
        ( (value.ty == type_E::int32  )  &&  ( value.int32   == 0    ) )
        ||                                                           
        ( (value.ty == type_E::int64  )  &&  ( value.int64   == 0    ) )
        ||                                                           
        ( (value.ty == type_E::uint8  )  &&  ( value.uint8   == 0    ) )
        ||                                                           
        ( (value.ty == type_E::uint16 )  &&  ( value.uint16  == 0    ) )
        ||                                                           
        ( (value.ty == type_E::uint32 )  &&  ( value.uint32  == 0    ) )
        ||                                                           
        ( (value.ty == type_E::uint64 )  &&  ( value.uint64  == 0    ) )
       )
        return true;
    else
        return false; 
}
M_endf



/////////////  is_value_true() ///////////////////// 

bool is_value_true(const value_S& value) try
{
    if (
        ( (value.ty == type_E::boolean)  &&  ( value.boolean == true ) )
        ||
        ( (value.ty == type_E::int8   )  &&  ( value.int8   != 0     ) )
        ||                                                           
        ( (value.ty == type_E::int16  )  &&  ( value.int16  != 0     ) )
        ||                                                           
        ( (value.ty == type_E::int32  )  &&  ( value.int32  != 0     ) )
        ||                                                           
        ( (value.ty == type_E::int64  )  &&  ( value.int64  != 0     ) )
        ||                                                           
        ( (value.ty == type_E::uint8  )  &&  ( value.uint8  != 0     ) )
        ||                                                           
        ( (value.ty == type_E::uint16 )  &&  ( value.uint16 != 0     ) )
        ||                                                           
        ( (value.ty == type_E::uint32 )  &&  ( value.uint32 != 0     ) )
        ||                                                           
        ( (value.ty == type_E::uint64 )  &&  ( value.uint64 != 0     ) )
       )                                                             
        return true;
    else
        return false; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   is_same_class() -- return true if both value type belong to same "class" (int, float, boolean, unit, string only) 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

bool is_same_class(const value_S& value1, const value_S& value2) try
{
    if ( is_value_unit(   value1) && is_value_unit(   value2) )                 return true;
    if ( is_value_boolean(value1) && is_value_boolean(value2) )                 return true;
    if ( is_value_integer(value1) && is_value_integer(value2) )                 return true;
    if ( is_value_float(  value1) && is_value_float(  value2) )                 return true;
    if ( is_value_string( value1) && is_value_string( value2) )                 return true;

    return false; 
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
////   set_vlist_value() -- put shared_ptr to passed-in vlist_S into caller's (uninitilized) value_S structure 
////                        (allocates new vlist_S and copies passed-in vlist_S into it)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void set_vlist_value(value_S& val, const vlist_S& vlist, bool move_ok) try
{
    // make sure value is not already set

    if (val.ty != type_E::none)
    {
        M_out_emsg1(L"set_vlist_value(): passed-in value is already set: ");
        display_value(val, L"already-set value");
        M_out_emsgz();
        M_throw("set_vlist_value(): passed-in value is already set")
    }


    // allocate new non-autodata vlist_S on heap, anchor in caller's value_S, and copy passed-in vlist_S into new vlist_S  -- nested things pointed to by the value_S remain shared
   
    val.vlist_sp.reset(new vlist_S {}); 

    if (move_ok) 
        *val.vlist_sp = std::move(vlist);
    else
    {
        *val.vlist_sp = vlist;
    }


    // set type in passed-in value

    val.ty = type_E::vlist; 


    // set location fields in passed-in value

    val.token_ix1 = vlist.token_ix1; 
    val.token_ix2 = vlist.token_ix2; 
  
    return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_expression_value() -- put shared_ptr to passed-in a_expression_S into caller's (uninitialized) value_S structure 
////                             (allocates new a_expression_S and copies passed-in a_expression_S into it)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void set_expression_value(value_S& val, const a_expression_S& expression, bool move_ok) try
{
    // make sure value is not already set

    if (val.ty != type_E::none)
    {
        M_out_emsg1(L"set_expression_value(): passed-in value is already set: ");
        display_value(val, L"already-set value");
        M_out_emsgz();
        M_throw("set_expression_value(): passed-in value is already set")
    }


    // allocate new non-autodata a_expression_S on heap, anchor in caller's value_S, and copy passed-in a_expression_S into new a_expression_S    -- nested values in vlists remain shared

    val.expression_sp.reset(new a_expression_S {}); 

    if (move_ok)
        *val.expression_sp = std::move(expression);
    else
        *val.expression_sp = expression;


    // set type in passed-in value

    val.ty = type_E::expression; 


    // set location fields in passed-in value

    val.token_ix1 = expression.token_ix1; 
    val.token_ix2 = expression.token_ix2; 


    return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_block_value() -- put shared_ptr to passed-in block_S into caller's (uninitialized) value_S structure 
////                        (allocates new block_S and copies passed-in block_S into it)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void set_block_value(value_S& val, const block_S& block, bool move_ok) try
{
    // make sure value is not already set

    if (val.ty != type_E::none)
    {
        M_out_emsg1(L"set_block_value(): passed-in value is already set: ");
        display_value(val, L"already-set value");
        M_out_emsgz();
        M_throw("set_block_value(): passed-in value is already set")
    }


    // allocate new non-autodata block_S on heap, anchor in caller's value_S, and copy passed-in block_S into new block_S  -- nested values remain shared

    val.block_sp.reset(new block_S {}); 

    if (move_ok)
        *val.block_sp = std::move(block);
    else
        *val.block_sp = block;


    // set type in passed-in value

    val.ty = type_E::block; 


    // set location fields in passed-in value

    val.token_ix1 = block.token_ix1; 
    val.token_ix2 = block.token_ix2; 
     
    return; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_verbset_value() -- put shared_ptr to passed-in verbset_S into caller's (uninitialized) value_S structure 
////                          (allocates new verbset_S and copies passed-in verbset_S into it)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void set_verbset_value(value_S& val, const verbset_S& verbset, bool move_ok) try
{
    // make sure value is not already set

    if (val.ty != type_E::none)
    {
        M_out_emsg1(L"set_verbset_value(): passed-in value is already set: ");
        display_value(val, L"already-set value");
        M_out_emsgz();
        M_throw("set_verbset_value(): passed-in value is already set")
    }


    // allocate new non-autodata verbset_S on heap, anchor in caller's value_S, and copy passed-in verbset_S into new verbset_S 

    val.verbset_sp.reset(new verbset_S {}); 

    if (move_ok)
        *val.verbset_sp = std::move(verbset);
    else
        *val.verbset_sp = verbset;


    // set type in passed-in value

    val.ty = type_E::verbset; 


    // set location fields in passed-in value

    val.token_ix1 = -1; 
    val.token_ix2 = -1; 
     
    return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_typdef_value() -- put shared_ptr to passed-in typdef_S into caller's (uninitialized) value_S structure 
////                         (allocates new typdef_S and copies passed-in typdef_S into it)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void set_typdef_value(value_S& val, const typdef_S& ty, bool move_ok) try
{
    // make sure value is not already set

    if (val.ty != type_E::none)
    {
        M_out_emsg1(L"set_typdef_value(): passed-in value is already set: ");
        display_value(val, L"already-set value");
        M_out_emsgz();
        M_throw("set_typdef_value(): passed-in value is already set")
    }


    // allocate new non-autodata typdef_S on heap, anchor in caller's value_S, and copy passed-in typdef_S into new typdef_S  -- nested typdef_S values remain shared

    val.typdef_sp.reset(new typdef_S {}); 

    if (move_ok)
        *val.typdef_sp = std::move(ty);
    else
        *val.typdef_sp = ty;


    // set type in passed-in value

    val.ty = type_E::typdef; 


    // set location fields in passed-in value

    val.token_ix1 = -1; 
    val.token_ix2 = -1; 
     
    return; 
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_ref_value() -- put shared_ptr to passed-in ref_S into caller's (uninitialized) value_S structure 
////                      (allocates new ref_S and copies passed-in ref_S into it)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void set_ref_value(value_S& val, const ref_S& ref, bool move_ok) try
{
    // make sure value is not already set

    if (val.ty != type_E::none)
    {
        M_out_emsg1(L"set_ref_value(): passed-in value is already set: ");
        display_value(val, L"already-set value");
        M_out_emsgz();
        M_throw("set_ref_value(): passed-in value is already set")
    }


    // allocate new non-autodata ref_S on heap, anchor in caller's value_S, and copy passed-in ref_S into new ref_S  -- nested ref_S values remain shared

    val.ref_sp.reset(new ref_S {}); 

    if (move_ok)
        *val.ref_sp = std::move(ref);
    else
        *val.ref_sp = ref;


    // set type in passed-in value

    val.ty = type_E::ref; 


    // set location fields in passed-in value

    val.token_ix1 = -1; 
    val.token_ix2 = -1; 
     
    return; 
}
M_endf





////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_buffer_value() -- put shared_ptrs to passed-in buf8_T and typdef_S into caller's (uninitialized) value_S structure 
////                         (allocates new buf8_T and typdef_S, then copies passed-in buf8_T and typdef_S into it)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void set_buffer_value(value_S& val, const buf8_T& buffer, const typdef_S& ty, bool move_ok) try
{
    // make sure value is not already set

    if (val.ty != type_E::none)
    {
        M_out_emsg1(L"set_buffer_value(): passed-in value is already set: ");
        display_value(val, L"already-set value");
        M_out_emsgz();
        M_throw("set_buffer_value(): passed-in value is already set")
    }


    // allocate new non-autodata buf8_T on heap, anchor in caller's value_S, and copy passed-in buf8_T into new buf8_T 

    val.buffer_sp.reset(new buf8_T {}); 

    if (move_ok)
        *val.buffer_sp = std::move(buffer);
    else
        *val.buffer_sp = buffer;  


    // allocate new non-autodata typdef_S on heap, anchor in caller's value_S, and copy passed-in typdef_S into new typdef_S 

    val.typdef_sp.reset(new typdef_S {}); 

    if (move_ok)
        *val.typdef_sp = std::move(ty);
    else
        *val.typdef_sp = ty;


    // set type in passed-in value to type_E value from passed-in typdef_S  -- nested typdef_S values remain shared

    if ( (ty.kind == type_E::array) || (ty.kind == type_E::structure) )
    {
        val.ty = ty.kind; 
    }
    else
    {
        // passed-in type is not array or structure -- should-not-occur error 
    
        M_out_emsg1(L"set_buffer_value(): passed-in typdef_S is not for array or struct ");
        display_typdef(L"invalid typdef_S", ty, L"");
        M_out_emsgz();
        M_throw("set_buffer_value(): passed-in typdef is not for array or struct")      
    }


    // set location fields in passed-in value

    val.token_ix1 = -1; 
    val.token_ix2 = -1; 
     
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
////   unshare_value() -- make sure this value_S points to unique copy of nested vlists, expressions, and blocks, etc.
////                     
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
        
void unshare_value(value_S& value) try
{                                                                                                                                                                                                   
    M__(M_out(L"unshare_value() called");)


    // if value has valid vlist_S pointer, replace existing vlist_S with new one
    // -------------------------------------------------------------------------
   
   	if (value.vlist_sp.get() != nullptr)
   	{
           M__(M_out(L"unshare_value() -- before: vlist_sp = %16X") % value.vlist_sp.get(); ) 
           std::shared_ptr<vlist_S> new_vlist_sp { std::make_shared<vlist_S>() };         // get new, empty vlist_S
           *new_vlist_sp  = *(value.vlist_sp);                                            // copy all existing fields into new vlist_S -- copies shared pointers, but not anything pointed to by those shared pointers
           value.vlist_sp = new_vlist_sp;                                                 // vlist_sp in value now points to filled-in new vlist_S (still with pointers to shared/uncopied vlists and values) -- .reset(xxx) does not work
                                                                                        
           unshare_vlist(*(value.vlist_sp));                                              // update new vlist_S with unshared copies of nested vlists (if any) 
           M__(M_out(L"unshare_value() -- after: vlist_sp = %16X") % value.vlist_sp.get(); ) 
   	}


    // if value has valid a_expression_S pointer, replace existing a_expression_S with new one
    // -------------------------------------------------------------------------------

    if (value.expression_sp.get() != nullptr)
    {
        M__(M_out(L"unshare_value() -- before: expression_sp = %16X") % value.expression_sp.get(); ) 
        std::shared_ptr<a_expression_S> new_expression_sp { std::make_shared<a_expression_S>() };        // get new, empty a_expression_S
        *new_expression_sp  = *(value.expression_sp);                                                    // copy all existing fields into new a_expression_S -- copies shared pointers, but not anything pointed to by those shared pointers
        value.expression_sp = new_expression_sp;                                                         // expression_sp in value now points to filled-in new a_expression_S (still with pointers to shared/uncopied vlists and expressions)  -- .reset(xxx) does not work

        unshare_expression(*(value.expression_sp));                                                      // update new a_expression_S with unshared copies of nested objects (if any)  
        M__(M_out(L"unshare_value() -- after: expression_sp = %16X") % value.expression_sp.get(); ) 
    }


    // if value has valid block_S pointer, replace existing block_S with new one
    // -------------------------------------------------------------------------

    if (value.block_sp.get() != nullptr)
    {
        M__(M_out(L"unshare_value() -- before: block_sp = %16X") % value.block_sp.get(); ) 
        std::shared_ptr<block_S> new_block_sp { std::make_shared<block_S>() };            // get new, empty block_S
        *new_block_sp  = *(value.block_sp);                                               // copy all existing fields into new block_S -- copies shared pointers, but not anything pointed to by those shared pointers
        value.block_sp = new_block_sp;                                                    // block_sp in value now points to filled-in new block_S (still with pointers to shared/uncopied vlists and expressions)
                                                                                          
        unshare_block(*(value.block_sp));                                                 // update new block with unshared copies of nested objects (if any) 
        M__(M_out(L"unshare_value() -- after: block_sp = %16X") % value.block_sp.get(); ) 
    }


    // if value has valid verbset_S pointer, replace existing verbset_S with new one
    // -----------------------------------------------------------------------------

    if (value.verbset_sp.get() != nullptr)
    {
        M__(M_out(L"unshare_value() -- before: verbset_sp = %16X") % value.verbset_sp.get(); ) 
        std::shared_ptr<verbset_S> new_verbset_sp { std::make_shared<verbset_S>() };     // get new, empty verbset_S
        *new_verbset_sp  = *(value.verbset_sp);                                          // copy all existing fields into verbset_S -- copies shared pointers, but not anything pointed to by those shared pointers
        value.verbset_sp = new_verbset_sp;                                               // verbset_sp in value now points to filled-in new verbdef_S (still with pointers to shared/uncopied plists blocks, etc.)

        unshare_verbset(*(value.verbset_sp));                                            // update new verbdef with unshared copies of nested objects (if any) 
        M__(M_out(L"unshare_value() -- after: verbset_sp = %16X") % value.verbset_sp.get(); ) 
    }  


    // if value has valid typdef_S pointer, replace existing typdef_S with new one (this may be unnecessary, since typdef_S expressions don't change once allocated) 
    // ---------------------------------------------------------------------------

    if (value.typdef_sp.get() != nullptr)
    {
        M__(M_out(L"unshare_value() -- before: typdef_sp = %16X") % value.typdef_sp.get(); ) 
        std::shared_ptr<typdef_S> new_typdef_sp { std::make_shared<typdef_S>() };         // get new, empty typdef_S
        *new_typdef_sp = *(value.typdef_sp);                                              // copy all existing fields into typdef_S -- copies shared pointers, but not anything pointed to by those shared pointers
        value.typdef_sp = new_typdef_sp;                                                  // typdef_sp in value now points to filled-in new typdef_S (still with pointers to shared/uncopied plists blocks, etc.)
                                                                                        
        unshare_typdef(*(value.typdef_sp));                                               // update new typdef_S with unshared copies of nested objects (if any) 
        M__(M_out(L"unshare_value() -- after: typdef_sp = %16X") % value.typdef_sp.get(); ) 
    }   


    // if value has valid buffer_S pointer, replace existing buffer_S with new one
    // ---------------------------------------------------------------------------

    if (value.buffer_sp.get() != nullptr)
    {
        M__(M_out(L"unshare_value() -- before: buffer_sp = %16X") % value.buffer_sp.get(); ) 
        std::shared_ptr<buf8_T> new_buffer_sp { std::make_shared<buf8_T>() };             // get new, empty buffer_C
        *new_buffer_sp = *(value.buffer_sp);                                              // copy all bufer_C control fields and buffer data into new buffer_C 
        value.buffer_sp = new_buffer_sp;                                                  // buffer_sp in value now points to filled-in new buffer_C
          
        M__(M_out(L"unshare_value() -- after: buffer_sp = %16X") % value.buffer_sp.get(); ) 
    }   


    // note: there is nothing to unshare for ref_S pointer -- associated value_S needs to remain shared, and there should be no problem sharing any typdef_S


    M__(M_out(L"unshare_value() returning");)
   	return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unshare_vlist() -- make sure all values in this vlist_S point to unique copies of nested vlists, expression, and blocks
////                      
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void unshare_vlist(vlist_S& vlist) try
{
    M__(M_out(L"unshare_vlist() called -- &vlist = %016X") % (void *)&vlist;)


   	// loop 1 -- unshare values for all positional parms -- update values in-place 

    if (vlist.value_ct > 0)
        for (auto& value : vlist.values) unshare_value(value); 


    // loop 2 -- unshare values for all evaluated/unevaluated keyword parms -- update values in-place 

    if (vlist.eval_kws.size() > 0)                             // any evaluated keywords? 
    {
         for (auto& elem : vlist.eval_kws) unshare_value(elem.second);       
    }
    else                                                       // no evaluated keywords (yet)
    {
        if (vlist.keywords.size() > 0)                         // do unevaluated ones, if any
        {
           for (auto& keyword : vlist.keywords)
           {
               unshare_value(keyword.name );
               unshare_value(keyword.value);
           }
        }
    }
    
    M__(M_out(L"unshare_vlist() returning");)
	   return; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unshare_expression() -- make sure all values in this a_expression_S point to unique copies of nested vlists, expressions, and blocks
////                     
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void unshare_expression(a_expression_S& expression) try
{
    M__(M_out(L"unshare_expression() called -- &expression = %016X") % (void *)&expression;)

	// unshare everything in both left-side and right-side vlists 

    unshare_value(expression.verb_value); 
    unshare_vlist(expression.lparms); 
    unshare_vlist(expression.rparms);
     
    M__(M_out(L"unshare_expression() returning");)
	return; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unshare_block() -- make sure all values in this block_S point to unique copies of nested vlists, expressions, and blocks
////                      
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void unshare_block(block_S& block) try
{
    M__(M_out(L"unshare_block() called -- &block = %016X") % (void *)&block;)

	// loop 1 -- unshare values for expressions in block 

    if (block.expression_ct > 0)
        for (auto& expression : block.expressions) unshare_expression(expression); 


    M__(M_out(L"unshare_block() returning");)
	return; 
}
M_endf




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unshare_verbset() -- make sure all values in this verbset_S point to unique copies of nested plists, blocks, parmtypes, etc.
////                      
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
   
void unshare_verbset(verbset_S& verbset) try
{  
    M__(M_out(L"unshare_verbset() called -- &verbset = %016X") % (void *)&verbset;)
   

    // ---------------------------------------------------------------
    // main loop to unshare each individual verbdef_S for this verbset
    // ---------------------------------------------------------------
    
    for (auto i = 0; i < verbset.verbs.size(); i++)
    {
        // make sure nothing is shared in the plist_S fields
      
        unshare_plist(verbset.verbs.at(i).lparms); 
        unshare_plist(verbset.verbs.at(i).rparms);
      
      
        // if verbdef has valid verb_block pointer, replace existing verb block with new (unshared) one.
      
        if (verbset.verbs.at(i).verb_block_sp.get() != nullptr)
        {
            M__(M_out(L"unshare_verbset() -- before: verb_block_sp = %16X") % verbset.verbs.at(i).verb_block_sp.get(); ) 
            std::shared_ptr<block_S> new_block_sp { std::make_shared<block_S>() };                                   // get new, empty block_S
            *new_block_sp = *(verbset.verbs.at(i).verb_block_sp);                                                    // copy all existing fields into new block -- copies shared pointers, but not anything pointed to by those shared pointers
            verbset.verbs.at(i).verb_block_sp = new_block_sp;                                                        // block_sp in verbset.verbs.at(0) now points to filled-in new block (still with pointers to shared/uncopied plists and parmtype_S expressions)
                                                                                                                   
            unshare_block(*(verbset.verbs.at(i).verb_block_sp));                                                     // update new block with unshared copies of nested objects (if any) 
            M__(M_out(L"unshare_verbset() -- after: verb_block_sp = %16X") % verbset.verbs.at(i).verb_block_sp.get(); )                  
        }                                                                                                          
      
      
        // if verbdef has valid init_block pointer, replace existing init block with new (unshared) one.
      
        if (verbset.verbs.at(i).init_block_sp.get() != nullptr)
        {
            M__(M_out(L"unshare_verbset() -- before: init_block_sp = %16X") % verbset.verbs.at(i).init_block_sp.get(); ) 
            std::shared_ptr<block_S> new_block_sp { std::make_shared<block_S>() };                                   // get new, empty block_S
            *new_block_sp = *(verbset.verbs.at(i).init_block_sp);                                                    // copy all existing fields into new block -- copies shared pointers, but not anything pointed to by those shared pointers
            verbset.verbs.at(i).init_block_sp = new_block_sp;                                                        // block_sp in verbset.verbs.at(0) now points to filled-in new block (still with pointers to shared/uncopied plists and parmtype_S expressions)
                                                                                                                   
            unshare_block(*(verbset.verbs.at(i).init_block_sp));                                                     // update new block with unshared copies of nested objects (if any) 
            M__(M_out(L"unshare_verbset() -- after: init_block_sp = %16X") % verbset.verbs.at(i).init_block_sp.get(); )                  
        }    

        
        // if verbdef has a persistent environment, get rid of it and replace it with a new (empty) one -- don't attempt to copy everything defined inside the environment -- all verbdefs in the verbset will have to be re-initialized 

        if (verbset.verbs.at(i).persist_env_sp.get() != nullptr)
        {
            verbset.verbs.at(i).persist_env_sp.reset();                                                              // get rid of existing persistent environment
            
            std::shared_ptr<environ_S> persist_env_sp { new environ_S { } };                                         // allocate new persistent environment for this verb and anchor it locally, for now
        
            verbset.verbs.at(i).persist_env_sp = persist_env_sp;                                                     // set up shared ptr to new (empty) persistent env in verbdef_S        
        }
    }
    
    M__(M_out(L"unshare_verbset() returning");)
	return; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unshare_plist() -- make sure all values in this plist_S point to unique copies of nested plists, parmtypes, etc.
////                      
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void unshare_plist(plist_S& plist) try
{
    M__(M_out(L"unshare_plist() called -- &plist = %016X") % (void *)&plist;)

	// loop 1 -- unshare values for all positional parms -- update values in-place 

    if (plist.values.size() > 0)
        for (auto& parmtype : plist.values) unshare_parmtype(parmtype); 


    // loop 2 -- unshare values for all keyword parms -- update values in-place 

    if (plist.keywords.size() > 0)
        for (auto& keyword : plist.keywords) unshare_parmtype(keyword.second); 
    
    M__(M_out(L"unshare_plist() returning");)
	return; 
}
M_endf



///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unshare_parmtype() -- make sure all values in this parmtype_S point to unique copies of nested plists 
////                      
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void unshare_parmtype(parmtype_S& parmtype) try
{
    M__(M_out(L"unshare_parmtype() called -- &parmtype = %016X") % (void *)&parmtype;)


    // if parmtype has valid plist pointer, replace existing plist with new (unshared) one.

    if (parmtype.plist_sp.get() != nullptr)
    {
        M__(M_out(L"unshare_parmtype() -- before: plist_sp = %16X") % parmtype.plist_sp.get(); ) 
        std::shared_ptr<plist_S> new_plist_sp { std::make_shared<plist_S>() };                   // get new, empty plist_S
        *new_plist_sp = *(parmtype.plist_sp);                                                    // copy all existing fields into new plist -- copies shared pointers, but not anything pointed to by those shared pointers
        parmtype.plist_sp = new_plist_sp;                                                        // plist_sp in parmtype now points to filled-in new plist (still with pointers to shared/uncopied vlists, etc.)

        unshare_plist(*(parmtype.plist_sp));                                                     // update new plist with unshared copies of nested objects (if any) 
        M__(M_out(L"unshare_parmtype() -- after: plist_sp = %16X") % parmtype.plist_sp.get(); ) 
    }

    M__(M_out(L"unshare_parmtype() returning");)
	return; 
}
M_endf




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unshare_typdef() -- make sure everything in this typdef_S points to unique copies (does not make copy of input typdef)
////                      
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void unshare_typdef(typdef_S& ty) try
{
    M__(M_out(L"unshare_typdef() called -- &ty = %016X") % (void *)&ty;)


    // if type has valid atype_sp (array element type) pointer, replace existing typdef_S with new (unshared) one
    // ----------------------------------------------------------------------------------------------------------

    if (ty.atype_sp.get() != nullptr)
    {
        M__(M_out(L"unshare_typdef() -- before: atype_sp.get() = %16X") % ty.atype_sp.get(); ) 
        std::shared_ptr<typdef_S> new_typdef_sp { std::make_shared<typdef_S>() };                // get new, empty typdef_S
        *new_typdef_sp = *(ty.atype_sp);                                                         // copy all existing fields into new typdef_S -- copies shared pointers, but not anything pointed to by those shared pointers
        ty.atype_sp = new_typdef_sp;                                                             // atype_sp in typdef_S now points to filled-in new typdef_S (still with pointers to shared/uncopied nested typdef_S, etc.)

        unshare_typdef(*(ty.atype_sp));                                                          // update new typdef_S with unshared copies of nested typdef_S (if any) 
        M__(M_out(L"unshare_typdef() -- after: atype_sp.get() = %16X") % ty.atype_sp.get(); ) 
    }


    // unshare any nested typdef_S values in the structure fieldefs 
    // ------------------------------------------------------------

    if (ty.fdefs.size() > 0)                                                                     // any nested typdef_S in structure fieldefs? 
    {
        // loop to process each typdef_S in the fieldef vector 

        for (auto& elem : ty.fdefs)                                                              // elem is fieldef_S  
        { 
           if (elem.ftype_sp.get() != nullptr)
           {
               M__(M_out(L"unshare_typdef() -- before: elem.ftype_sp.get() = %16X") % elem.ftype_sp.get(); ) 
               std::shared_ptr<typdef_S> new_typdef_sp { std::make_shared<typdef_S>() };         // get new, empty typdef_S
               *new_typdef_sp = *(elem.ftype_sp);                                                // copy all existing fields into new typdef_S -- copies shared pointers, but not anything pointed to by those shared pointers
               elem.ftype_sp = new_typdef_sp;                                                    // shared_ptr for this field now points to filled-in new typdef_S (still with pointers to shared/uncopied nested typdef_S, etc.)
                                                                                                 
               unshare_typdef(*(elem.ftype_sp));                                                 // update new typdef_S with unshared copies of nested typdef_S (if any) 
               M__(M_out(L"unshare_typdef() -- after: elem.ftype_sp.get() = %16X") % elem.ftype_sp.get(); )             
           }
        }
    }
     
    M__(M_out(L"unshare_typdef() returning");)
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
////   refresh_evaluated_vlist() -- converts keywords in evaluated keywords multlimap back to vector form and places them in the unevaluated keywords vector 
////
////                                (!!!!! note: order of keywords in reconstructed unevaluated keywords list may not be the same as it was originally !!!!)
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
void refresh_evaluated_vlist(vlist_S& vlist) try
{
    if (vlist.kw_eval_done)
    {  
        // loop through evaluated keywords multimap, extracting key/value pairs

        vlist.keywords.clear();                                        // clear out unevaluated keyword vector before refilling with evaluated keywords 

        for (auto elem : vlist.eval_kws)                               // get each name value pair in the evaluated keywords ist 
        {
            vlist.keywords.push_back(keyword_S { string_val(elem.first), elem.second });         // add newly reconstructed keyword to vector of unevaluated keywords
        }


        //  update flags and keyword counts, to make vlist look like keywords have not been evaluated yet 

        vlist.eval_kws.clear();                                        // clear out evaluated keywords multimap 
        set_vlist_flags(vlist);                                        // update main vlist flags, in case anything is different 
        vlist.kw_eval_done  = false;                                   // make vlist look like it has unevaluated keywords 
        vlist.kw_ct         = vlist.keywords.size();                   // update keyword count to number of keywords in the vector (shouldn't change)                 
    }
 
    return; 
}
M_endf




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   combine_vlists() -- combine values in two vlists
////
////
////                       note: token_ix will be -1 (unknown) in output vlist ?????? 
////                      
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
int combine_vlists(vlist_S& vout, const vlist_S& v1, const vlist_S& v2, bool verbatim) try
{
    vout = vlist_S { };                                                                                           // start with empty vlist as return value 


    // combine positional values from v1 and v2 into vout   

    vout.values = v1.values;                                                                                      // put all positional values from v1 (if any) into output vlist
    if (v2.values.size() > 0)                                                                                     // bypass append operation, if v2 is empty
        vout.values.insert(vout.values.end(), v2.values.cbegin(), v2.values.cend());                              // append all positional values from v2 (if any) to output vlist
   

    // do verbatim combine -- preserve separate .keywords and .eval_kws from input vlists to output vlist 
    // --------------------------------------------------------------------------------------------------

    if (verbatim)
    {   
         // make sure v1 and v2 are both have unevaluated or evaluated keywords

        if (v1.kw_eval_done != v2.kw_eval_done)
        {
             M_out_emsg(L"combine_vlists() -- unequal keyword evaluation flags in the two input vlists:  v1=%S  v2=%S") % M_bool_str(v1.kw_eval_done) % M_bool_str(v2.kw_eval_done);     
             return -1;
        }
     

        // combine unevaluated keyword names/values from v1 and v2 into vout   
        
        vout.keywords = v1.keywords;                                                                                  // put all unevaluated keyword names/values from v1 (if any) into output vlist
        if (v2.keywords.size() > 0)                                                                                   // bypass append operation, if v2 has no keywords
            vout.keywords.insert(vout.keywords.end(), v2.keywords.cbegin(), v2.keywords.cend());                      // append all unevaluated keyword names/values from v2 (if any) to output vlist
        
        
        // combine evaluated keyword names/values (if any) from v1 and v2 into vout 
        
        vout.eval_kws = v1.eval_kws; 
        if (v2.eval_kws.size() > 0)                                                                                   // bypass append operation, if v2 has no evaluated keywords
           vout.eval_kws.insert(v2.eval_kws.cbegin(), v2.eval_kws.cend());                                            // append all evaluated keyword names/values from v2 (if any) to output vlist
        
        vout.kw_eval_done = v1.kw_eval_done;                                                                          // same as v2.kw_eval_done, because of earlier check 
    }


    // do non-verbatim combine -- copy input .eval_kws or .keywords to output .keywords -- eval_kws.done flag will not be set -- eval_kws will be empty in output vlist  
    // ================================================================================================================================================================

    else
    {
        // copy evaluated or unevaluated keywords from 1st input vlist over to output vlist 
        // --------------------------------------------------------------------------------

         if (v1.kw_eval_done)                                                                                         
         {
             // copy evaluated keyword names/values (if any) from v1 into vout unevaluated keyword list (ordering may change) 
                       
             if (v1.eval_kws.size() > 0)
             {
                 for (auto elem : v1.eval_kws)                                                                        // elem.first = keyword name, elem.second = keyword value 
                     vout.keywords.push_back(keyword_S { string_val(elem.first), elem.second });                      // add combined name/value structure to end of unevaluated keywords in output vlist       
             }
         }
         else
         {
             vout.keywords = v1.keywords;                                                                             // put all unevaluated keyword names/values from v1 (if any) into output vlist
         }


         // copy evaluated or unevaluated keywords from 2nd input vlist over to output vlist 
         // --------------------------------------------------------------------------------

         if (v2.kw_eval_done)                                                                                         
         {
             if (v2.eval_kws.size() > 0)
             {
                 for (auto elem : v2.eval_kws)                                                                        // elem.first = keyword name, elem.second = keyword value 
                     vout.keywords.push_back(keyword_S { string_val(elem.first), elem.second });                      // add combined name/value structure to end of unevaluated keywords in output vlist     
             }  
         }
         else                                                                                                          // no evaluated keywords in either vlist
         {
             // copy evaluated keyword names/values (if any) from v1 into vout unevaluated keyword list (ordering may change)  
                            
             if (v2.keywords.size() > 0)                                                                               // bypass append operation, if v2 has no keywords
                 vout.keywords.insert(vout.keywords.end(), v2.keywords.cbegin(), v2.keywords.cend());                  // append all unevaluated keyword names/values from v2 (if any) to output vlist         
         }      
    }


    // update miscellaneous fields and flags in output vlist
    // -----------------------------------------------------

    vout.value_ct     = vout.values.size();
    
    if (vout.kw_eval_done)
        vout.kw_ct = vout.eval_kws.size();
    else
        vout.kw_ct = vout.keywords.size(); 
        
    set_vlist_flags(vout);

    M__(display_vlist(v1  , L"input vlist 1");)
    M__(display_vlist(v2  , L"input vlist 2");)
    M__(display_vlist(vout, L"output vlist" );)

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


///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   is_type_xxxx() -- test passed-in typdef_S or type_E for various properties  
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// versions with passed in type_E parm
// -----------------------------------

bool is_type_valid(type_E kind) try
{
    switch (kind)
    {
        case type_E::none        : return true ;      break;
        case type_E::no_value    : return true ;      break;
        case type_E::special     : return true ;      break;
        case type_E::error       : return true ;      break;
        case type_E::unit        : return true ;      break;
        case type_E::boolean     : return true ;      break;
        case type_E::int8        : return true ;      break;    
        case type_E::int16       : return true ;      break;
        case type_E::int32       : return true ;      break;
        case type_E::int64       : return true ;      break;
        case type_E::uint8       : return true ;      break;
        case type_E::uint16      : return true ;      break;
        case type_E::uint32      : return true ;      break;
        case type_E::uint64      : return true ;      break;
        case type_E::float32     : return true ;      break;
        case type_E::float64     : return true ;      break;
        case type_E::array       : return true ;      break;
        case type_E::structure   : return true ;      break;
        case type_E::string      : return true ;      break;
        case type_E::identifier  : return true ;      break;
        case type_E::verbname    : return true ;      break;
        case type_E::keyname     : return true ;      break;
        case type_E::typdef      : return true ;      break;
        case type_E::ref         : return true ;      break;
        case type_E::vlist       : return true ;      break;
        case type_E::expression  : return true ;      break;
        case type_E::block       : return true ;      break;
        case type_E::verbset     : return true ;      break; 
        default                  : return false;      break; 
    }  
}
M_endf


bool is_type_atomic(type_E kind) try
{
    switch (kind)
    {
        case type_E::boolean     : return true ;      break; 
        case type_E::int8        : return true ;      break;    
        case type_E::int16       : return true ;      break;
        case type_E::int32       : return true ;      break;
        case type_E::int64       : return true ;      break;
        case type_E::uint8       : return true ;      break;
        case type_E::uint16      : return true ;      break;
        case type_E::uint32      : return true ;      break;
        case type_E::uint64      : return true ;      break;
        case type_E::float32     : return true ;      break;
        case type_E::float64     : return true ;      break;
        default                  : return false;      break;
    }  
}
M_endf


bool is_type_aggregate(type_E kind) try
{
    switch (kind)
    {
       case type_E::array       : return true ;      break;
       case type_E::structure   : return true ;      break;
       default                  : return false;      break; 
    }  
}
M_endf


bool is_type_fixed_size(type_E kind) try
{
    if ( is_type_atomic(kind) || is_type_aggregate(kind) )
        return true; 
    else
        return false; 
}
M_endf


// versions with passed in typdef_S parm
// -------------------------------------

bool is_type_valid(           const typdef_S& ty) try { return is_type_valid(        ty.kind); } M_endf
bool is_type_atomic(          const typdef_S& ty) try { return is_type_atomic(       ty.kind); } M_endf
bool is_type_aggregate(       const typdef_S& ty) try { return is_type_aggregate(    ty.kind); } M_endf
bool is_type_fixed_size(      const typdef_S& ty) try { return is_type_fixed_size(   ty.kind); } M_endf



///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   typdefs_are_same() -- return true if both typedefs are same, false otherwise 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

bool fieldefs_are_same(const fieldef_S& f1, const fieldef_S& f2) try
{
    // fieldefs are not same if offsets are different
    
    if (f1.offset != f2.offset)                                        
        return false;                                                 // offsets differ -- fieldefs are not same


    // fieldefs are same/different, depending on field typdefs

    return typdefs_are_same( *(f1.ftype_sp), *(f2.ftype_sp) );
}
M_endf


///////////////////////////////////////////////////

bool typdefs_are_same(const typdef_S& t1, const typdef_S& t2) try
{
    // see if top-level type kinds and lengths are same

    if (t1.kind != t2.kind)                                           
        return false;                                                     // top-level types differ -- typdefs are not same


    // for non-aggregate types, typdefs are same if kind is same -- no further checking required 

    if (!is_type_aggregate(t1.kind))                                      // t2.kind = t1.kind at this point
        return true;                                                      // same, if not aggregate type     
     

    // for aggregate typdefs, lengths must be same

    if (t1.tsize != t2.tsize)
        return false;                                                     // lengths differ, so following tests would fail somewhere
                   

    //  array typdefs -- same if element count and element type is same
    //  =============    ----------------------------------------------
                                                                         
    if (t1.kind == type_E::array)                                         // t1.kind =  t2.kind here
    {
        if (t1.acount != t2.acount)
            return false;                                                 // differing number of elements -- typdefs are not same

        return typdefs_are_same(*(t1.atype_sp),  *(t2.atype_sp) );        // same number of elements -- typdefs are same/different depending on the types of array elements
    }


    // structure typdefs -- need same fieldefs in same order in both typdefs for them to be same -- field names don't matter -- just the data layout
    // =================    ------------------------------------------------------------------------------------------------------------------------

    if (t1.kind == type_E::structure)                                     // t1.kind = t2.kind here
    {         
        if ( t1.fdefs.size() != t2.fdefs.size() )
            return false;                                                 // structure typdefs are not same, if number of fields is different  
         

        // see if all 'n' fieldefs for both typdefs match

        for (auto i = 0; i < t1.fdefs.size(); i++)                        // t1.fdefs.size() = t2.fdefs.size() here
        {
            if ( !fieldefs_are_same(t1.fdefs.at(i), t2.fdefs.at(i)) )     // see if i-th fieldef is same in both structure typdefs
                return false;                                             // structure typdefs are not same if any fieldef differs       
        }

        return true;                                                      // structure typdefs are same -- all fieldefs match up  
    }


    // error? -- should not get here ???
    // ======    -----------------------

    M_out_emsg(L"typedefs_are_same() -- unknown non-aggregate kind = %S seen") % type_str(t1.kind);
    return false;   
}
M_endf




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   atomic_type_size() -- return size of atomic type data based on passed-in type_E  
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

size_t atomic_type_size(type_E kind) try 
{
    switch (kind)
    {
        case type_E::boolean     : return sizeof (bool     );      break;  
        case type_E::int8        : return sizeof (int8_t   );      break;    
        case type_E::int16       : return sizeof (int16_t  );      break;
        case type_E::int32       : return sizeof (int32_t  );      break;
        case type_E::int64       : return sizeof (int64_t  );      break;
        case type_E::uint8       : return sizeof (uint8_t  );      break;
        case type_E::uint16      : return sizeof (uint16_t );      break;
        case type_E::uint32      : return sizeof (uint32_t );      break;
        case type_E::uint64      : return sizeof (uint64_t );      break;
        case type_E::float32     : return sizeof (float32_T);      break;
        case type_E::float64     : return sizeof (float64_T);      break;
    default                  : return (size_t)0         ;      break;              // non-atomic type -- just return 0
    }
}  
M_endf 




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   make_atomic_typdef() -- fill in passed-in typdef_S based on caller's requested kind  
////                        -- not used for aggregate types -- see make_array_type() and make_structure_type()
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// version with passed-in typdef_S to fill in
// ------------------------------------------

int make_atomic_typdef(type_E kind, typdef_S& out_ty) try
{
    out_ty = typdef_S { };                                // clear out passed-in typdef_S before starting

    if (is_type_atomic(kind))                             // atomic types are OK
    {
        out_ty.kind  = kind;                              // save kind in typdef_S
        out_ty.tsize = atomic_type_size(kind);            // for atomic types, save fixed length  
        return 0;                                          
    }
    else if (!is_type_aggregate(kind))                    // non-aggregate types are also OK
    {
        out_ty.kind  = kind;                              // save kind in typdef_S -- length of non-atomic type will be 0
        return 0; 
    }
    else                                                  // this routine can't make an aggregate type
    {
        count_error();
        M_out_emsg(L"make_atomic_typdef() -- aggregate type_E::%S was passed in -- cannot make atomic typdef") % type_str(kind);
        return -1;                                        // just return error 
    }  
} 
M_endf


// version that returns typdef_S
// -----------------------------

typdef_S make_atomic_typdef(type_E kind) try
{
    typdef_S out_ty = typdef_S { };                       // clear out returned typdef_S before starting

    if (is_type_atomic(kind))                             // atomic types are OK
    {
        out_ty.kind  = kind;                              // save kind in typdef_S
        out_ty.tsize = atomic_type_size(kind);            // for atomic types, save fixed length  
    }
    else if (!is_type_aggregate(kind))                    // non-aggregate types are also OK
    {
        out_ty.kind  = kind;                              // save kind in typdef_S -- length of non-atomic type will be 0
    }
    else                                                  // this routine can't make an aggregate type
    {
        count_error();
        M_out_emsg(L"make_atomic_typdef() -- aggregate type_E::%S was passed in -- cannot make atomic typdef") % type_str(kind);
    } 

    return out_ty; 
} 
M_endf




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   make_array_typdef() -- fill in passed-in typdef_S based on caller's requested element type and number of elements 
////                     
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int make_array_typdef(size_t acount, const typdef_S& atype, typdef_S& out_ty) try
{
    out_ty = typdef_S { };                                       // clear out passed-in typdef_S before starting 

    // error if passed-in array size if zero

    if (acount <= 0)
    {
        count_error();
        M_out_emsg(L"make_array_typdef() -- passed-in number of array elements (%d) is not valid -- must be greater than 0 -- cannot make array typdef ") % acount;
        return -1; 
    }

    // error if this is not a fized-size type

    if (!is_type_fixed_size(atype))
    {
        count_error();  
        M_out_emsg(L"make_array_typdef() -- passed-in type_E::%S does not have fixed length -- cannot make array typdef") % type_str(atype.kind);
        return -1; 
    }


    // fill in fields in new array typdef_S

    out_ty.kind      = type_E::array;                          // indicate that this is an array 
    out_ty.acount    = acount;                                 // number of array elements 
    out_ty.tsize     = acount * atype.tsize;                   // size of array = (number of array elements) * (fixed size of passed-in element type)

    out_ty.atype_sp.reset(new typdef_S {});                    // get new typdef_S and anchor in caller's typdef_S 
    *out_ty.atype_sp = atype;                                  // copy fields from passed-in atype to atype anchored in new typdef_S
    unshare_typdef(*out_ty.atype_sp);                          // make sure all nested types are not shared with passed-in atype

    return 0; 
}
M_endf




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   make_structure_typdef() -- fill in passed-in typdef_S based on caller's passed-in field names and definitions 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

                         
int make_structure_typdef(const std::vector<fieldparm_S>& fieldparms, typdef_S& out_ty) try
{
    int64_t max_end_offset      { 0 };                          // high water mark for field end offset
    int64_t prior_end_offset    { 0 };                          // ending offset of prior field
    int64_t prior_start_offset  { 0 };                          // starting offset of prior field
    out_ty = typdef_S           {   };                          // clear out before starting            


    // -------------------------------------------------------------------
    // main loop to process each fieldparm and add to structure containers
    // -------------------------------------------------------------------

    int field_ix { 0 };                                         // field index -- index of fieldef_S in fdefs vector, also for error messages

    for (auto& elem : fieldparms)
    {
        // make sure this field name is not a duplicate
        // --------------------------------------------

        if (out_ty.fnames.count(elem.fieldname) > 0)            // see if this fieldname is already present in fnames map
        {
            count_error();
            M_out_emsg(L"make_structure_typdef() -- field name %d:%S = %d has already been used") % field_ix % elem.fieldname;
            out_ty = typdef_S { };       
            return -1;    
        }
                       

        // compute starting/base offset for this field (before any skipped bytes)
        // -------------------------------------------

        int64_t base_offset  { 0 };                            // starting offset for field, before any skipped bytes, etc.
        int64_t start_offset { 0 };                            // starting offset for field, after any skipped bytes, etc.

        if (elem.has_offset)                                   // eplicit start/base offset specified?
        {
            base_offset = elem.offset;                         // this should have been checked for negative values already 
        }
        else if (elem.same_as_prior)                           // this field should start where prior field did?
        {
            base_offset = prior_start_offset;           
        }
        else if (elem.at_hi_watermark)                         // this field should start just past highest ending offset of any field so far?
        {
            base_offset = max_end_offset;           
        }
        else                                                   // offset: nor same: not present for this field -- just start right after prior field 
        {
            base_offset = prior_end_offset;   
        }


        // add in any bytes to be skipped before this field starts
        // -------------------------------------------------------

        if (elem.has_skip)
        {   
            start_offset = base_offset + elem.skip;    // skip bytes before starting this field
  
            if (start_offset < 0)
            {
                count_error();
                M_out_emsg(L"make_structure_typdef() -- negative starting offset for field %d:%S = %d (base offset = %d, skip = %d") % field_ix % elem.fieldname % start_offset  % base_offset % elem.skip;
                out_ty = typdef_S { };       
                return -1;                 
            }
        }
        else                                          // skip not present -- just use base offset
        {
            start_offset = base_offset; 
        }
     


        // get length for this field from typdef_S -- make sure this is not 0 (i.e. typdef is for an atomic or aggregate type)
        // -------------------------------------------------------------------------------------------------------------------

        uint64_t  field_length  = elem.typdef_p-> tsize; 
   
        if (field_length <= 0)
        {
            count_error();
            M_out_emsg(L"make_structure_typdef() -- field %d:%S cannot have 0 length -- type = %S") % field_ix % elem.fieldname % type_str(elem.typdef_p->kind);
            out_ty = typdef_S { };       
            return -1;
        }
    

        // fill in fieldef_S and add to vector and map
        // -------------------------------------------

        fieldef_S fieldef { };  
        fieldef.offset = start_offset; 

        std::shared_ptr<typdef_S> new_typdef_sp { std::make_shared<typdef_S>() }; // get new typdef_S -- don't know whether passed-in typdef is in autodata or heap 
        *new_typdef_sp = *(elem.typdef_p);                                        // fill in new typdef_S from typdef_S in fieldparm  
        fieldef.ftype_sp = new_typdef_sp;                                         // point to new typdef_S -- nested typdef_S should be immutable, so don't bother doing unshare_typdef()  
                                                                         
        out_ty.fdefs.push_back(fieldef);                                          // add this fieldef to vector -- should be at index = field_ix
        out_ty.fnames.emplace(elem.fieldname, field_ix);                          // add field name and index to fnames map


        // get ready for next loop iteration
        // ---------------------------------

        prior_start_offset = start_offset;                                        // save this field's starting offset for next loop pass
        prior_end_offset   = prior_start_offset + field_length;                   // save this field's ending   offset for next loop pass
        max_end_offset     = std::max(max_end_offset, prior_end_offset);          // keep track of high-water mark, for overall structure length
        field_ix++;                                                               // index/debug number for next field

    }   // end of main loop


    // ----------------------------------------------------


    // finish up the structure-type typdef_S 

    out_ty.tsize = max_end_offset; 
    out_ty.kind  = type_E::structure; 

    M__(display_typdef(L"output typdef_S", out_ty);)
    M__(M_out(L"make_structure_typdef() -- returning");)
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



///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   make_reference() -- construct lvalue/rvalue ref_S based on referenced value, offset, type, etc. 
////                     
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// ====================================================================================================
// version for making aggregate-type lvalue reference -- val_sp normally points to non-reference type
//
//
//  note:  allocates new typdef_S and attaches to passed-in ref_S
//  note:  val_sp needs to point to allocated value_S anchored in an symval_S
//
// ====================================================================================================

int make_reference(ref_S& ref, const std::shared_ptr<value_S>& val_sp, const typdef_S& typdef, uint64_t offset) try
{
    // not valid if passed-in typedef is not for fixed-length type (for now)
    // ---------------------------------------------------------------------
       
    if (typdef.tsize <= 0)
    {
        count_error();
        M_out_emsg(L"make_reference() -- cannot make lvalue ref using a non-fixed-length (0-size) typedef");   
        return -1; 
    }
    

    // handle ref-type values (not expected here)
    // ======================

    if (val_sp->ty == type_E::ref)
    {
        // not valid, if value does not have associated ref_S 
        // --------------------------------------------------
       
        if (val_sp->ref_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to ref-type value_S that has no associated ref_S");   
            return -1; 
        }
   

        // not valid, if value's ref_S does not have associated value_S 
        // ------------------------------------------------------------
     
        if (val_sp->ref_sp->refval_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to ref-type value_S that has no associated value_S");   
            return -1; 
        }


        // not valid, if value's ref_S does not have associated typdef_S 
        // -------------------------------------------------------------
     
        if (val_sp->ref_sp->typdef_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to ref-type value_S that has no associated typdef_S");   
            return -1; 
        }
        

        // not valid, if value is not buffer-type reference 
        // ------------------------------------------------
     
        if ( !(val_sp->ref_sp->in_buffer) )
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to non-buffer ref-type value_S");   
            return -1; 
        }


        // not valid, if passed-in offset is smaller than offset in passed-in value's ref_S 
        // --------------------------------------------------------------------------------
     
        if (offset < val_sp->ref_sp->offset)
        {
            count_error();
            M_out_emsg(L"make_reference() -- passed-in offset (%d) is less than in value.ref_sp->offset (%d) -- cannot make lvalue ref") % offset % val_sp->ref_sp->offset;   
            return -1; 
        }
     

        // not valid, if passed-in offset + size in passed-in typdef exceeds offset in passed-in value's ref_S + size in that ref_S->typdef (i.e. goes past end of data covered in existing ref_S)
        // --------------------------------------------------------------------------------------------------------------------------------
     
        if (offset + typdef.tsize > val_sp->ref_sp->offset + val_sp->ref_sp->typdef_sp->tsize)
        {
            count_error();
            M_out_emsg(L"make_reference() -- passed-in offset (%d) + typedef.tsize (%d) exceeds value.ref_sp->offset (%d) + sze in that ref_S->typdef -- cannot make lvalue ref") 
                       % offset % typdef.tsize % val_sp->ref_sp->offset % val_sp->ref_sp->typdef_sp->tsize;   
            return -1; 
        }  


        // allocate new typdef_S with fully unshared nested values
        // -------------------------------------------------------

        std::shared_ptr<typdef_S> typdef_sp { new typdef_S };                // allocate new typdef_S to be anchored in caller's ref_S
        *typdef_sp = typdef;                                                 // copy over everything in caller's typdef into new one
        unshare_typdef(*typdef_sp);                                          // make sure all nested typdef_S, etc. are also newly allocated
    

        // fill in fields in caller's ref_S
        // --------------------------------

        ref.in_buffer   = val_sp->ref_sp->in_buffer;                         // copy over buffer flag
        ref.is_lvalue   = val_sp->ref_sp->is_lvalue;                         // copy over lvalue flag
        ref.is_rvalue   = val_sp->ref_sp->is_rvalue;                         // copy over rvalue flag
        ref.auto_deref  = val_sp->ref_sp->auto_deref;                        // copy over deref  flag

        ref.refval_sp   = val_sp->ref_sp->refval_sp;                         // point this ref_S to passed-in value_S's ref_S's value_S
        ref.typdef_sp   = typdef_sp;                                         // attach newly-allocated typdef_sp to ref 
        ref.offset      = offset;                                            // save passed-in offset
    }


    // handle non-ref type values
    // ==========================

    else
    {         
        // not valid, if value does not have buffer (for now) 
        // --------------------------------------------------
       
        if (val_sp->buffer_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to value_S that has no aggregate buffer");   
            return -1; 
        }
   
       
        // not valid if offset + typdef_S length exceeds buffer length
        // -----------------------------------------------------------
       
        if (typdef.tsize + offset > val_sp->buffer_sp->sz1)
        {
            count_error();
            M_out_emsg(L"make_reference() -- requested offset (%d) + typedef length (%d) exceeds buffer length (%d) -- cannot make lvalue reference") % offset % typdef.tsize % val_sp->buffer_sp->sz1;   
            return -1; 
        }
       

        // allocate new typdef_S with fully unshared nested values
        // -------------------------------------------------------

        std::shared_ptr<typdef_S> typdef_sp { new typdef_S };               // allocate new typdef_S to be anchored in caller's ref_S
        *typdef_sp = typdef;                                                // copy over everything in caller's typdef into new one
        unshare_typdef(*typdef_sp);                                         // make sure all nested typdef_S, etc. are also newly allocated


        // fill in fields in caller's ref_S
        // --------------------------------

        ref.in_buffer  = true;                                              // indicate that this reference is to data in refval_sp->buffer  
        ref.auto_deref = true;                                              // indicate that ref was created under-the-covers (not explicitly) 
       
              // note:  caller must set  is_rvalue  or  is_lvalue  flag in this case   

        ref.refval_sp  = val_sp;                                            // point this ref_S to passed-in value
        ref.typdef_sp  = typdef_sp;                                         // attach newly-allocated typdef_sp to ref 
        ref.offset     = offset;                                            // save passed-in offset
    }


    return 0; 
}
M_endf



// =========================================================================================================================
// version for making aggregate-type lvalue reference from passed-in reference type value (non-reference value not supported)
//
//
//  note:  allocates new typdef_S and attaches to passed-in ref_S
//
// ==========================================================================================================================

int make_reference(ref_S& ref, const value_S& value, const typdef_S& typdef, uint64_t offset) try
{
    // not valid if passed-in typedef is not for fixed-length type (for now)
    // ---------------------------------------------------------------------
       
    if (typdef.tsize <= 0)
    {
        count_error();
        M_out_emsg(L"make_reference() -- cannot make lvalue ref using a non-fixed-length (0-size) typedef");   
        return -1; 
    }
    

    // handle ref-type values
    // ======================

    if (value.ty == type_E::ref)
    {
        // not valid, if value does not have associated ref_S 
        // --------------------------------------------------
       
        if (value.ref_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to ref-type value_S that has no associated ref_S");   
            return -1; 
        }
   

        // not valid, if value's ref_S does not have associated value_S 
        // ------------------------------------------------------------
     
        if (value.ref_sp->refval_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to ref-type value_S that has no associated value_S");   
            return -1; 
        }


        // not valid, if value's ref_S does not have associated typdef_S 
        // -------------------------------------------------------------
     
        if (value.ref_sp->typdef_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to ref-type value_S that has no associated typdef_S");   
            return -1; 
        }
        

        // not valid, if value is not buffer-type reference 
        // ------------------------------------------------
     
        if ( !(value.ref_sp->in_buffer) )
        {
            count_error();
            M_out_emsg(L"make_reference() -- cannot make lvalue ref to non-buffer ref-type value_S");   
            return -1; 
        }


        // not valid, if passed-in offset is smaller than offset in passed-in value's ref_S 
        // --------------------------------------------------------------------------------
     
        if (offset < value.ref_sp->offset)
        {
            count_error();
            M_out_emsg(L"make_reference() -- passed-in offset (%d) is less than in value.ref_sp->offset (%d) -- cannot make lvalue ref") % offset % value.ref_sp->offset;   
            return -1; 
        }
     

        // not valid, if passed-in offset + size in passed-in typdef exceeds offset in passed-in value's ref_S + size in that ref_S->typdef (i.e. goes past end of data covered in existing ref_S)
        // --------------------------------------------------------------------------------------------------------------------------------
     
        if (offset + typdef.tsize > value.ref_sp->offset + value.ref_sp->typdef_sp->tsize)
        {
            count_error();
            M_out_emsg(L"make_reference() -- passed-in offset (%d) + typedef.tsize (%d) exceeds value.ref_sp->offset (%d) + sze in that ref_S->typdef -- cannot make lvalue ref") 
                       % offset % typdef.tsize % value.ref_sp->offset % value.ref_sp->typdef_sp->tsize;   
            return -1; 
        }  


        // allocate new typdef_S with fully unshared nested values
        // -------------------------------------------------------

        std::shared_ptr<typdef_S> typdef_sp { new typdef_S };               // allocate new typdef_S to be anchored in caller's ref_S
        *typdef_sp = typdef;                                                // copy over everything in caller's typdef into new one
        unshare_typdef(*typdef_sp);                                         // make sure all nested typdef_S, etc. are also newly allocated
    

        // fill in fields in caller's ref_S
        // --------------------------------

        ref.in_buffer   = value.ref_sp->in_buffer;                          // copy over buffer flag
        ref.is_lvalue   = value.ref_sp->is_lvalue;                          // copy over lvalue flag
        ref.is_rvalue   = value.ref_sp->is_rvalue;                          // copy over rvalue flag
        ref.auto_deref  = value.ref_sp->auto_deref;                         // copy over deref  flag

        ref.refval_sp   = value.ref_sp->refval_sp;                          // point this ref_S to passed-in value_S's ref_S's value_S
        ref.typdef_sp   = typdef_sp;                                        // attach newly-allocated typdef_sp to ref 
        ref.offset      = offset;                                           // save passed-in offset
    }


    // handle non-ref type values
    // ==========================

    else
    {         
        // error if not reference -- must use version with passed-in val_sp 
        // ----------------------------------------------------------------
       
        count_error();
        M_out_emsg(L"make_reference() -- version with passed-in value (not std:shared_ptr<value_S>) can only be used for making references from other references");   
        return -1; 
    }
    
    return 0; 
}
M_endf



// ======================================================
// version for making full-variable type lvalue reference
// ======================================================

int make_reference(ref_S& ref, const symval_S& symval, const std::wstring& ident) try
{
    ref = ref_S { };            // reset caller's ref_S to default state, before starting


    // make sure symval_S is not for verbdef, typdef, or constant  
    // ----------------------------------------------------------

    if (symval.is_verbset)
    {
        count_error();
        M_out_emsg(L"make_reference() -- cannot make lvalue ref to %sverbset %S") % (symval.is_builtin ? L"builtin " : L"") % ident;   
        return -1; 
    }

    if (symval.is_typdef)
    {
        count_error();
        M_out_emsg(L"make_reference() -- cannot make lvalue ref to %stypedef %S") % (symval.is_builtin ? L"builtin " : L"") % ident;   
        return -1; 
    }

    if (symval.is_const)
    {
        count_error();
        M_out_emsg(L"make_reference() -- cannot make lvalue ref to %sconst %S") % (symval.is_builtin ? L"builtin " : L"") % ident;   
        return -1; 
    }


    // fill in fields in passed-in ref_S (there is no associated typdef, since in_buffer is not set 
    // ---------------------------------

    if(symval.is_const)
        ref.is_rvalue = true;                                               // refers to const -- can only be rvalue 
    else                                                                  
        ref.is_lvalue = true;                                               // refers to var   -- can be lvalue 


    // check to see if weak pointer in symval is still valid 
  
    if (symval.value_wp.expired())
    {
        count_error(); 
        M_out_emsg(L"make_reference() -- symval (weak alias?) for identifier (\"%S\") no longer points to a valid value -- cannot make reference") % ident;   
        return -1;
    }

    // note: caller must set auto_deref flag in this case  

    ref.refval_sp = symval.value_wp.lock();                                 // value_wp should never be unshared 
                                           
    return 0; 
}
M_endf




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   dereference_value() -- de-reference passed in value. 
////
////       note: this should only be called when a rvalue is required -- data sometimes gets copied -- pointers to original data may not be preserved  ?????? may need rvalue/lvalue flag parm
////                     
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int dereference_value(value_S& out_value, const value_S& in_value) try      // ??? is rvalue/lvalue flag needed ??????
{
    M__(M_out(L"dereference_value() -- called:");)
    M__(display_value(in_value, L"input value");)   

    out_value = value_S { };               // make sure output_value is cleaned out first


    // if input value is not reference, just copy it to output value
    // -------------------------------------------------------------

    if (in_value.ty != type_E::ref)
    {
        out_value = in_value; 
        unshare_value(out_value);       // get new copy of all nested values, too
        return 0;
    }

    
    // -------------------------------------------
    // check for corrupted value/ref, and complain
    // -------------------------------------------

    if (in_value.ref_sp.get() == nullptr)
    {
         count_error();
         M_out_emsg1(L"dereference_value() -- unexpected error -- ref_sp in value_S is not initialized");
         msgend_loc(in_value);
         return -1; 
    }

    if (in_value.ref_sp->refval_sp.get() == nullptr)
    {
         count_error();
         M_out_emsg1(L"dereference_value() -- unexpected error -- value_wp in input value_S attached reference is not initialized");
         msgend_loc(in_value);
         return -1; 
    }


    // check in-buffer type reference

    if (in_value.ref_sp->in_buffer)
    {   
        // in-buffer type reference -- should have valid typdef_sp and referenced value should have a buffer 

        if (in_value.ref_sp->typdef_sp.get() == nullptr)
        {
             count_error();
             M_out_emsg1(L"dereference_value() -- unexpected error -- in_buffer type reference has uninitialized typdef_sp");
             msgend_loc(in_value);
             return -1;          
        }   

        if (in_value.ref_sp->refval_sp->buffer_sp.get() == nullptr)
        {
             count_error();
             M_out_emsg1(L"dereference_value() -- unexpected error -- in_buffer type reference referred-to value has uninitialized buffer_sp (i.e. no buffer)");
             msgend_loc(in_value);
             return -1;   
        } 


        // make sure length in associated typdef is non-zero (for data copying, etc.)

        if (in_value.ref_sp->typdef_sp->tsize <= 0)
        {
             count_error();
             M_out_emsg1(L"dereference_value() -- unexpected error -- in_buffer type reference referred-to value has uninitialized buffer_sp (i.e. no buffer)");
             msgend_loc(in_value);
             return -1;   
        }   
    }


    // check non-buffer type reference

    else                                                       // not in-buffer type reference -- should have no offset or associated typdef, and should not be identifier or expression (and probably other things too)
    {                                                          // ------------------------------------------------------------------------------------------------------------------------------------------------------
        if (in_value.ref_sp->typdef_sp.get() != nullptr)
        {
             count_error();
             M_out_emsg1(L"dereference_value() -- unexpected error -- non-buffer type reference has value typdef_sp");
             msgend_loc(in_value);
             return -1;          
        }   
    
        if (in_value.ref_sp->offset != 0)
        {
             count_error();
             M_out_emsg1(L"dereference_value() -- unexpected error -- non-buffer type reference has non-zero offset = %d") % in_value.ref_sp->offset;
             msgend_loc(in_value);
             return -1;          
        }   

        if (                                                            // should not be reference to expression or identifier requiring further evaluation
            (in_value.ref_sp->refval_sp->ty == type_E::identifier)
            ||
            (in_value.ref_sp->refval_sp->ty == type_E::expression)
           )
        {
             count_error();
             M_out_emsg1(L"dereference_value() -- unexpected error -- unexpected type requiring further evaluation in referenced value = %S") % type_str(in_value.ref_sp->refval_sp->ty);
             msgend_loc(in_value);
             return -1;         
        } 
    }


    // handle in-buffer type reference  (assume this type of de-reference is for rvalue purposes)
    // -------------------------------

    if (in_value.ref_sp->in_buffer)
    {
        // set up for data copy operation into new (shorter?) buffer, or value_S fixed-types union -- make sure we don't go off of end of from-data buffer

        void  *data_p        { in_value.ref_sp->refval_sp->buffer_sp->p1 };                   // get local copy of start of from-buffer
        size_t data_len      { in_value.ref_sp->typdef_sp->tsize         };                   // get local copy of data length to copy 
        size_t data_offset   { in_value.ref_sp->offset                   };                   // get local copy of data offset (within buffer)

        M__(M_out(L"dereference_value() -- data_offset = %d   data_len = %d   data_p = %p   from-buffer size = %d") % data_offset % data_len % data_p % in_value.ref_sp->refval_sp->buffer_sp->sz1;)

        if  (data_len + data_offset > in_value.ref_sp->refval_sp->buffer_sp->sz1)
        {
             count_error();
             M_out_emsg1(L"dereference_value() -- unexpected error -- data_offset (%d) + data_len (%d) exceeds from-buffer size (%d)") % data_offset % data_len % in_value.ref_sp->refval_sp->buffer_sp->sz1;
             msgend_loc(in_value);
             return -1;       
        }


        // handle dereference that needs to be copied into new (shorter) buffer attached to output value -- ???? need to optimize out this copy -- have offset fierld in value_S ???
        // ---------------------------------------------------------------------------------------------

        if (in_value.ref_sp->typdef_sp->kind == type_E::array || in_value.ref_sp->typdef_sp->kind == type_E::structure )
        {
            out_value.buffer_sp = std::make_shared<buf8_T>(data_len);                        // get new buffer long enough for output aggregate type 

            std::memcpy( (void *)(out_value.buffer_sp->p1)                                   // into start of new buffer just allocated
                       , (void *)M_add_ptr(data_p, data_offset)                              // from proper offset in source buffer
                       , data_len                                                            // length of data to copy (known to be non-0 from prior check)
                       );  


            // set output value's typdef_sp to input value's typdef_sp -- OK, since typdef should never get updated through this typdef_sp

            out_value.typdef_sp = in_value.ref_sp->typdef_sp;                                // point to input value's ref_S's typdef
            out_value.ty        = in_value.ref_sp->typdef_sp->kind;                          // also set type_E:: in putu[pt value       
        }


        // handle dereference that yields short fixed-type value (to be copied directly inside value_S)
        // --------------------------------------------------------------------------------------------

        else
        {
             if  (data_len > sizeof (uint64_t) )
             {
                  count_error();
                  M_out_emsg1(L"dereference_value() -- unexpected error -- data_len (%d) exceeds sizeof destination uint8_t field in value_S") % data_len;
                  msgend_loc(in_value);
                  return -1;       
             } 
        

             M__(M_out(L"dereference_value() -- copy into outval.uint64 -- to-addr = %p   from-addr = %p   len = %d") % (&(out_value.uint64)) % M_add_ptr(data_p, data_offset) % data_len;)

             out_value.uint64 = 0xBBBBBBBBBBBBBBBB;                               // place visible background pattern in whole field before memcpy()

             std::memcpy( (void *)(&(out_value.uint64))                           // into fixed-type union in value_S
                        , (void *)M_add_ptr(data_p, data_offset)                  // from proper offset in source buffer
                        , data_len                                                // length of data to copy (known to be non-0 from prior check)
                        );

             out_value.ty = in_value.ref_sp->typdef_sp->kind;                     // for simple types, just need kind field from reference typdef 
        }   


        // set output value_S token indexes from referenced value's token indexes

        out_value.kw_token_ix = in_value.ref_sp->refval_sp->kw_token_ix;           // just copy over token indexes
        out_value.token_ix1   = in_value.ref_sp->refval_sp->token_ix1;             // just copy over token indexes
        out_value.token_ix2   = in_value.ref_sp->refval_sp->token_ix2;             // just copy over token indexes  
    }


    // handle full-value (non buffer) type reference  
    // ---------------------------------------------
    
    else
    {
          out_value =  *(in_value.ref_sp->refval_sp);                              // just copy all 1st-level fields from referenced value to output value 
         
          // note: no unshare_value() is done here, since this should be used as an rvalue, and no modifications to referred value will be done     
    }
    

    M__(M_out(L"dereference_value() -- returning:");)
    M__(display_value(out_value, L"output value");)

    return 0;
}
M_endf


///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_via_reference() -- set value by reference. 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int set_via_reference(const value_S& ref_val, const value_S& from_val, bool unshare) try
{
    // complain if value has no reference, of reference is not lvalue
    // --------------------------------------------------------------

    if (ref_val.ref_sp.get() == nullptr)
    {
        count_error();
        M_out_emsg1(L"set_via_reference() -- passed-in value has no associated reference");
        msgend_loc(ref_val, L"ref value");
        return -1;            
    }

    ref_S* ref_p = ref_val.ref_sp.get(); 

    if ( !(ref_p->is_lvalue) )
    {
        count_error();
        M_out_emsg1(L"set_via_reference() -- passed-in reference value is not an lvalue, as required");
        msgend_loc(ref_val, L"ref value");
        return -1;    
    }


    // complain if referred-to value is not present
    // --------------------------------------------

    if (ref_p->refval_sp.get() == nullptr)
    {
        count_error();
        M_out_emsg1(L"set_via_reference() -- passed-in reference has no associated value");
        msgend_loc(ref_val, L"ref value");
        return -1;    
    }


    // ----------------------------------
    // handle reference to data in buffer
    // ----------------------------------

    if (ref_p->in_buffer)
    {
        // complain if no associated typdef_S or referred-to value has no buffer
        // ---------------------------------------------------------------------

        if (ref_p->typdef_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg1(L"set_via_reference() -- passed-in in-buffer reference has no associated typedef");
            msgend_loc(ref_val, L"ref value");
            return -1;   
        }

        if (ref_p->refval_sp->buffer_sp.get() == nullptr)
        {
            count_error();
            M_out_emsg1(L"set_via_reference() -- in-buffer reference, but referred-to value has no data buffer");
            msgend_loc(ref_val, L"ref value");
            return -1;   
        }


        // complain if 0-length or length + offset exceeds size of target buffer
        // ---------------------------------------------------------------------
      
        uint64_t offset { ref_p->offset                                              };     // local copy of data offset in referenced buffer
        uint64_t to_len { ref_p->typdef_sp->tsize                                    };     // length of data to be set in referenced buffer
        void    *to_p   { (void *)M_add_ptr(ref_p->refval_sp->buffer_sp->p1, offset) };     // start of data to be set in referred-to buffer


        if (to_len == 0)
        {
            count_error();
            M_out_emsg1(L"set_via_reference() -- length of data to set, in associated typedef is 0");
            msgend_loc(ref_val, L"ref value");
            return -1;         
        }

        if (offset + to_len > ref_p->refval_sp->buffer_sp->sz1)
        {
            count_error();
            M_out_emsg1(L"set_via_reference() -- offset (%d) + length (%d) exceeds target buffer size (%d)") % offset % to_len % ref_p->refval_sp->buffer_sp->sz1;
            msgend_loc(ref_val, L"ref value");
            return -1;         
        }


        // handle copying of atomic-type data from inside from_val to the referred-to buffer
        // ---------------------------------------------------------------------------------
 
        if (is_type_atomic(ref_p->typdef_sp->kind))
        {
            size_t from_len { atomic_type_size(ref_p->typdef_sp->kind) };      // get local copy of source data length


            // complain, if source data length is not same as target data length
            // -----------------------------------------------------------------

            if (from_len != to_len)
            {
                count_error();
                M_out_emsg1(L"set_via_reference() -- source data length (%d) from passed-in value is not same as target data length (%d) from reference") % from_len % to_len;
                msg_loc(    ref_val, L"ref value");
                msgend_loc(from_val, L"source value");
                return -1;             
            }


            // complain, if source data type is not same as target data type
            // -------------------------------------------------------------

            if (from_val.ty != ref_p->typdef_sp->kind)
            {
                count_error();
                M_out_emsg1(L"set_via_reference() -- source data type (%S) from passed-in value is not same as target data type (%S) from reference") % type_str(from_val.ty) % type_str(ref_p->typdef_sp->kind);
                msg_loc(    ref_val, L"ref value");
                msgend_loc(from_val, L"source value");
                return -1;             
            }


            // copy atomic data from inside passed-in source value_S to proper offset in buffer
            // --------------------------------------------------------------------------------

            std::memcpy( to_p                                                    // into buffer at proper offset
                       , (void *)(&(from_val.uint64))                            // from fixed-type union in value_S                      
                       , to_len                                                  // length of data to copy (should be non-zero, and same as from_len, because of prior checks)
                       );        
        }


        // handle copying of non-atomic-type data from from_val's buffer to the referred-to buffer
        // ---------------------------------------------------------------------------------------

        else if (is_type_aggregate(ref_p->typdef_sp->kind))
        {
            // complain if from_val has no source data buffer
            // ----------------------------------------------

            if (from_val.buffer_sp.get() == nullptr)
            {
                count_error();
                M_out_emsg1(L"set_via_reference() -- in-buffer reference, but source value has no data buffer");
                msg_loc(    ref_val, L"ref value");
                msgend_loc(from_val, L"source value");
                return -1;   
            }


            // complain if from_val has no associated typdef
            // ---------------------------------------------

            if (from_val.typdef_sp.get() == nullptr)
            {
                count_error();
                M_out_emsg1(L"set_via_reference() -- in-buffer reference, but source value has no associated typedef");
                msg_loc(    ref_val, L"ref value");
                msgend_loc(from_val, L"source value");
                return -1;   
            }


            // complain if from_val typdef and passed-in ref typdef are not same type  (note: lengths are checked for equality here) 
            // ----------------------------------------------------------------------

            if ( !typdefs_are_same(*(from_val.typdef_sp), *(ref_p->typdef_sp)) )
            {
                count_error();
                M_out_emsg1(L"set_via_reference() -- source data type and target data types do not match");
                msg_loc(    ref_val, L"ref value");
                msgend_loc(from_val, L"source value");
                return -1;   
            }  


            // complain if to_len (same as from_len) exceeds from_val's buffer length 
            // ----------------------------------------------------------------------

            if ( to_len > from_val.buffer_sp->sz1)
            {
                count_error();
                M_out_emsg1(L"set_via_reference() -- source/target data length (%d) exceeds source data buffer size (%d)") % to_len % from_val.buffer_sp->sz1;
                msg_loc(    ref_val, L"ref value");
                msgend_loc(from_val, L"source value");
                return -1;   
            }  


            // copy data from start of source buffer into proper offset in target buffer
            // -------------------------------------------------------------------------

            std::memcpy( to_p                                                    // into buffer at proper offset
                       , (void *)(from_val.buffer_sp->p1)                        // from start of source data buffer                      
                       , to_len                                                  // length of data to copy (should be non-zero, and same as from_len, because of prior checks)
                       );  
        }


        // error -- unexpected source data type -- not copyable into buffer
        // ----------------------------------------------------------------

        else
        {
            count_error();
            M_out_emsg1(L"set_via_reference() -- unexpected source data type = %S -- cannot be copied into target buffer") % type_str(ref_p->typdef_sp->kind);
            msgend_loc(ref_val, L"ref value");
            return -1;         
        }   
    }  


    // --------------------------------------------------------------------------------------
    // handle whole-value reference -- just replace all value_S fields with those in from_val
    // --------------------------------------------------------------------------------------

    else
    {
        *(ref_p->refval_sp) = from_val;                           // replace referred-to value with passed-in from value
                                                               
        if (unshare)                                              // caller requests unshared value after update?
           unshare_value(*(ref_p->refval_sp));                    // unshare all nested pointers, etc. in updated value 
    }

    return 0; 
}
M_endf


//_________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""