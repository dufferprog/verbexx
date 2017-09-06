// ex_verb_op.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ==============
////     ex_verb_op.cpp -- parser-oriented pre-defined verbs -- math, string, compare, logical, assignment operators
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    >>>      operator -- right assignment
//    <<<  =   operator -- left assignment 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// local helper functions to set/get variable or reference   (used by all assignment-type verbs)
// -------------------------------------------------------
//
// note: assumes type is ref or ident (etc.)  

int set_lvalue(frame_S& frame, const value_S& lvalue, const value_S& rvalue, bool unshare = true) try
{
    if (lvalue.ty == type_E::ref)
        return set_via_reference(lvalue, rvalue, unshare);                // reference lvalue -- update referred-to value/buffer, etc. from rvalue
    else                                                                  // assume type is ident (or at least has variable name to be updated)
        return update_var(frame, lvalue.string, rvalue, false, unshare);  // variable lvalue from rvalue -- update by name 
}
M_endf

//////////////////////////////////

int get_lvalue(frame_S& frame, const value_S& lvalue, value_S& rvalue) try
{
    if (lvalue.ty == type_E::ref)
        return dereference_value(rvalue, lvalue);                        // dereference lvalue -- rvalue gets referred-to value
    else                                                                 // assume type is ident (or at least has variable name to be updated)
        return get_identifier(frame, lvalue.string, rvalue);             // should be defined based on earlier checking -- will get unit value, if not defined  
}
M_endf  



//----------------------------------------------------------------------------------------
//   verb_left_assign()    var1 var2 var3 ...  = value1 value2 value3 ...  share: unshare:
//----------------------------------------------------------------------------------------

int verb_left_assign(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are 1-N left positional non-constant identifiers (pre-defined) parm and 0-N right positional evaluated parms of assignable type

    M__(M_out(L"verb_left_assign() called");)

    auto unshare_rc = get_right_keyword(expression, L"unshare");                                                           // r/c = -1 if not present   
                                                                                                                  
    auto lsize = expression.lparms.values.size();                                                                          // number of variables to set
    auto rsize = expression.rparms.values.size();                                                                          // number of available values


    // loop to set each variable/reference in left-side vlist 
    // ------------------------------------------------------

    for(auto i = 0; i < lsize; i++)
    {
        int src {0};                                                                                                   // return code from set_lvalue()

        if (i < rsize)                                                                                                 // value is available for setting this variable/reference
            src = set_lvalue(frame, expression.lparms.values.at(i), expression.rparms.values.at(i),  unshare_rc == 0); // fails if undefined, or const 
        else                                                                                                           // no value available for this variable/reference -- just set to unit value
            src = set_lvalue(frame, expression.lparms.values.at(i), unit_val()               ,  unshare_rc == 0);      // fails if undefined, or const -- this will always fail for buffer-type references  
                                                                                                                     
        if (src != 0)                                                                                                  // did set_lvalue() fail?
        {                                                                                                            
            results =  error_results();                                                                                // pass back error resultsr 
            return -1;                                                                                                 // return with error
        } 
    }


    // set up single results (1 value present) or multiple results (0 or more than 1 value present) 
    // --------------------------------------------------------------------------------------------

    if (rsize == 1)                                                                                                    // just one value to be assigned
    {
        results = to_results(expression.rparms.values.at(0));                                                          // output value = single value assigned to single variable
    }
    else                                                                                                               // 0 or more than 1 value -- need to pass back multiple results
    {
        vlist_S vlist { };                                                                                             // clean results vlist -- no unshare: keyword, etc. 
        vlist.values = expression.rparms.values;                                                                       // copy over just the right-side positional parms = all values to be assigned   
        results = to_results(vlist_val(vlist));                                                                        // convert vlist to results
        results.multiple_results = true;                                                                               // pass back vlist as multiple results
    }

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   verb_right_assign()    value >>> var share: unshare:
//---------------------------------------------------------------------------------

int verb_right_assign(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are 1-N right positional non-constant identifiers (pre-defiend) parm and 0-N left positional evaluated parms of assignable type

    M__(M_out(L"verb_right_assign() called");)


    auto unshare_rc = get_right_keyword(expression, L"unshare");                                                             // r/c = -1 if not present   

    auto lsize = expression.lparms.values.size();                                                                            // number of available values
    auto rsize = expression.rparms.values.size();                                                                            // number of variables to set


    // loop to set each variable in right-side vlist 
    // ---------------------------------------------

    for(auto i = 0; i < rsize; i++)
    {
        int src {0};                                                                                                   // return code from set_lvalue()

        if (i < rsize)                                                                                                 // value is available for setting this variable/reference
            src = set_lvalue(frame, expression.rparms.values.at(i), expression.lparms.values.at(i),  unshare_rc == 0); // fails if undefined, or const 
        else                                                                                                           // no value available for this variable/reference -- just set to unit value
            src = set_lvalue(frame, expression.rparms.values.at(i), unit_val()                ,  unshare_rc == 0);     // fails if undefined, or const -- this will always fail for buffer-type references  
                                                                                                                     
        if (src != 0)                                                                                                  // did set_lvalue() fail?
        {                                                                                                            
            results =  error_results();                                                                                // pass back error results 
            return -1;                                                                                                 // return with error
        } 
    }


    // set up single results (1 value present) or multiple results (0 or more than 1 value present)

    if (lsize == 1)                                                                                                    // just one value to be assigned
    {
        results = to_results(expression.lparms.values.at(0));                                                          // output value = single value assigned to single variable
    }
    else                                                                                                               // 0 or more than 1 value -- need to pass back multiple results
    {
        vlist_S vlist { };                                                                                             // clean results vlist -- no unshare: keyword, etc. 
        vlist.values = expression.lparms.values;                                                                       // copy over just the left-side positional parms = all values to be assigned   
        results = to_results(vlist_val(vlist));                                                                        // convert vlist to results
        results.multiple_results = true;                                                                               // pass back vlist as multiple results
    }

    return 0; 
}
M_endf





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Arithmetic and string assignment operator verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════

  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    ++ -- increment and decrement
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T> inline
T verb__pre_incr(T& x)  {return ++x;}

template<typename T> inline
T verb__pre_decr(T& x)  {return --x;}

template<typename T> inline
T verb__post_incr(T& x) {return x++;}

template<typename T> inline
T verb__post_decr(T& x) {return x--;}
 

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//   incr_decr()
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

// helper function to do increment or decrement

static int incr_decr(frame_S& frame, const e_expression_S& expression, results_S& results, bool do_incr, const std::wstring& opws) try
{
    value_S value { };

    // already known that there is one left-side or one right-side pre-defined non-constant identifier, and no other parms

    int rc {0}; 

    if (expression.lparms.value_ct > 0)      // parms on left (postfix version)?
    {                                   
        // postfix version -- output value is before increment
        // ---------------------------------------------------

        // make sure variable (known to be defined) has an integer value  

        value_S val {}; 

        auto grc = get_lvalue(frame, expression.lparms.values.at(0), val);                    // should be defined based on earlier checking -- will get unit value, if not defined
        if (grc != 0)                                                                         // did get_lvalue() fail?
        {                                                                               
            results =  error_results();                                                       // pass back error results 
            return -1;                                                                        // return with error
        } 
        
        if (!is_value_integer(val))
        {
            M_out_emsg1(L"incr_decr() -- verb= %s (postfix), variable= %s has a non-integer value -- cannot %s") %verb_name(expression) % expression.lparms.values.at(0).string % opws;
            msgend_loc(expression.lparms.values.at(0), expression);
            results = error_results();                                                        // pass back error value 
            rc = -1; 
        }
        else
        {
            if (do_incr)                                                                      // do increment/decrement as requested
                M_verb_unary_fval_integer(value, verb__post_incr, val)                        // sets value
            else                                                              
                M_verb_unary_fval_integer(value, verb__post_decr, val)                        // sets value

            auto src = set_lvalue(frame, expression.lparms.values.at(0), val); 
            if (src != 0)                                                                     // did set_lvalue() fail?
            {                                                                               
                results =  error_results();                                                   // pass back error results 
                return -1;                                                                    // return with error
            } 
        }    
    }
    else                                // parms are on right (prefix version)
    {
         // prefix version -- output value is after increment
         // ------------------------------------------------- 
    
         // make sure variable (known to be defined) has an integer value  

        value_S val {}; 

        auto grc = get_lvalue(frame, expression.rparms.values.at(0), val);                    // should be defined based on earlier checking -- will get unit value, if not defined
        if (grc != 0)                                                                         // did get_lvalue() fail?
        {                                                                               
            results =  error_results();                                                       // pass back error results 
            return -1;                                                                        // return with error
        } 
        
        if (!is_value_integer(val))
        {
            M_out_emsg1(L"incr_decr() -- verb= %s (prefix), variable= %s has a non-integer value -- cannot %s") %verb_name(expression) % expression.rparms.values.at(0).string % opws;
            msgend_loc(expression.rparms.values.at(0), expression);
            results = error_results();                                                        // pass back error value 
            rc = -1; 
        }
        else
        {
            if (do_incr)                                                                      // do increment/decrement as requested
                M_verb_unary_fval_integer(value, verb__pre_incr, val)                         // sets value 
            else
                M_verb_unary_fval_integer(value, verb__pre_decr, val)                         // sets value 
                        
            auto src = set_lvalue(frame, expression.rparms.values.at(0), val);
            if (src != 0)                                                                     // did set_lvalue() fail?
            {                                                                               
                results =  error_results();                                                   // pass back error results 
                return -1;                                                                    // return with error
            } 
        }   
    }

    results = to_results(value);         // output results = results of operation
    return rc;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_increment()
//---------------------------------------------------------------------------------

int verb_increment(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_increment() called");)

    // already known that there is one left-side or one right-side pre-defined non-constant identifier/lvalue, and no other parms

    return incr_decr(frame, expression, results, true, L"increment"); 
}
M_endf


//---------------------------------------------------------------------------------
//   verb_decrement()
//---------------------------------------------------------------------------------

int verb_decrement(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side or one right-side pre-defined non-constant identifier/lvalue, and no other parms

    M__(M_out(L"verb_decrement() called");)

    return incr_decr(frame, expression, results, false, L"decrement");
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------
//   get_eq_values()  -- helper function for xxxx_eq verbs
//---------------------------------------------------------------------------------

static int get_eq_values(frame_S &frame, const e_expression_S& expression, value_S& lvalue, value_S& value1, value_S& value2, results_S& results, bool must_match = true) try
{
    value_S value0 {expression.lparms.values.at(0)};    // get unsubstituted value of left-side parm (save for error messages0)     
    lvalue = value0;                                    // pass back variable name/lvalue

    value2 = expression.rparms.values.at(0);            // get right-side value -- should be a string, int64 or float64 value


    // get value from left-side variable/lvalue -- make sure value type is same as right-side parm  

    auto grc = get_lvalue(frame, lvalue, value1);   
    if (grc != 0) return -1;                            // return immediately, if error occurred
         

    if (must_match)
    {
        if (value1.ty == value2.ty) 
            return 0;                                   // value types match    
       
       
        // error -- substituted variable value type does not match type of match right-side parm 
        
        M_out_emsg1(L"get_eq_values() -- verb=%s, left-side parm type (%S) does not match type of right-side parm (%S) -- cannot execute verb") % verb_name(expression) % type_str(value1.ty) % type_str(value2.ty);
        msg_loc(value0, L"value1");
        msg_loc(value2, L"value2");
        msgend_loc(expression);
        results = error_results();                      // pass back error value 
        return -1; 
    }
    else                                                // exact match not required -- make sure substituted variable is in same class as right-side parm 
    {
        if (is_same_class(value1, value2))
            return 0; 
       
        // error -- substituted variable value is not in same class as right-side parm 
      
        M_out_emsg1(L"get_eq_values() -- verb=%s, type left-side parm (%S) is not in same class as type of right-side parm (%S) -- cannot execute verb") % verb_name(expression) % type_str(value1.ty) % type_str(value2.ty);
        msg_loc(value0, L"value1");                     // location before substitution
        msg_loc(value2, L"value2");
        msgend_loc(expression);
        results = error_results();                      // pass back error value 
        return -1; 
    }  
}

M_endf


//---------------------------------------------------------------------------------
//   verb_add_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__add_eq(T& x, T y)  {x += y; return x;}


int verb_add_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side float64 or int64 positional parm

    M__(M_out(L"verb_add_eq() called");)
    value_S value { };               

    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing arithmetic value 
    value_S        value2   { };  // right-side value should be an arithmetic value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable + right-side parm 

    M_verb_binary_fval_arith(value, verb__add_eq, value1, value2)           // sets output value = value1 

    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }

    // return with results = value assigned to variable

    results = to_results(value);
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_subtract_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__subtract_eq(T& x, T y)  {x -= y; return x;}


int verb_subtract_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side arithmetic positional parm

    M__(M_out(L"verb_subtract_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing arithmetic value 
    value_S        value2   { };  // right-side value should be an int64 or float64 value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable - right-side parm 

    M_verb_binary_fval_arith(value, verb__subtract_eq, value1, value2)        // sets output value = value1 

    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_multiply_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__multiply_eq(T& x, T y)  {x *= y; return x;}


int verb_multiply_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side arithmetic positional parm

    M__(M_out(L"verb_multiply_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing arithmetic value 
    value_S        value2   { };  // right-side value should be an arithmetic value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable * right-side parm 

    M_verb_binary_fval_arith(value, verb__multiply_eq, value1, value2)        // sets output value = value1 
   
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_divide_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__divide_eq(T& x, T y)  {x /= y; return x;}
                                                   

int verb_divide_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side arithmetic positional parm

    M__(M_out(L"verb_divide_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing arithmetic value 
    value_S        value2   { };  // right-side value should be an arithmetic value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable / right-side parm 
 
    M_verb_binary_fval_arith(value, verb__divide_eq, value1, value2)        // sets output value = value1 
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_power_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__power_eq(T& x, T y) {x = power(x, y); return x;}


int verb_power_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side arithmetic positional parm

    M__(M_out(L"verb_power_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing arithmetic value 
    value_S        value2   { };  // right-side value should be an arithmetic value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results); 
    if (grc != 0) return grc;                                                 // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable ^ right-side parm 

    M_verb_binary_fval_arith(value, verb__power_eq, value1, value2)        // sets output value = value1 
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);
    return 0;
}
M_endf
  

//---------------------------------------------------------------------------------
//   verb_remainder_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__remainder_eq(T& x, T y)  {x %= y; return x;}
                                                 

int verb_remainder_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side integer positional parm

    M__(M_out(L"verb_remainder_eq() called");)
    value_S value { };


    // get left-side and right-side frameuated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing integer value 
    value_S        value2   { };  // right-side value should be an integer value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable % right-side parm 

    M_verb_binary_fval_integer(value, verb__remainder_eq, value1, value2)        // sets output value = value1 
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);

    return 0;
}
M_endf



//---------------------------------------------------------------------------------
//   verb_concatenate_eq()
//---------------------------------------------------------------------------------

int verb_concatenate_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side string positional parm

    M__(M_out(L"verb_concatenate_eq() called");)


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing string value 
    value_S        value2   { };  // right-side value should be a string value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable + right-side parm (string concatenation) 

    value1.string = value1.string + value2.string;
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value1);

    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_bitand_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__bitand_eq(T& x, T y)  {x &= y; return x;}
                                                 

int verb_bitand_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side integer positional parm

    M__(M_out(L"verb_bitand_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing integer value 
    value_S        value2   { };  // right-side value should be an integer value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable & right-side parm 

    M_verb_binary_fval_integer(value, verb__bitand_eq, value1, value2)        // sets output value = value1 
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);

    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_bitor_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__bitor_eq(T& x, T y)  {x |= y; return x;}
                                                 

int verb_bitor_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side integer positional parm

    M__(M_out(L"verb_bitor_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing integer value 
    value_S        value2   { };  // right-side value should be an integer value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable & right-side parm 

    M_verb_binary_fval_integer(value, verb__bitor_eq, value1, value2)        // sets output value = value1 
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);

    return 0;
}
M_endf



//---------------------------------------------------------------------------------
//   verb_bitxor_eq()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__bitxor_eq(T& x, T y)  {x ^= y; return x;}
                                                 

int verb_bitxor_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side integer positional parm

    M__(M_out(L"verb_bitxor_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };  // left-side value should be a defined variable containing integer value 
    value_S        value2   { };  // right-side value should be an integer value
    value_S        lvalue   { };  // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results);
    if (grc != 0) return grc;                                                // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable & right-side parm 

    M_verb_binary_fval_integer(value, verb__bitxor_eq, value1, value2)        // sets output value = value1 
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);

    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_shift_left_eq()
//---------------------------------------------------------------------------------

template<typename T1, typename T2> inline
T1 verb__shift_left_eq(T1& x, T2 y)
{
    if  (y >= 0)
        x <<= y;
    else                    // negative left shift amount -- do right shift instead  
        x >>= -y; 
    return x;
}
                                                 

int verb_shift_left_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side integer positional parm

    M__(M_out(L"verb_shift_left_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };                                                          // left-side value should be a defined variable containing integer value 
    value_S        value2   { };                                                          // right-side value should be an integer value
    value_S        lvalue   { };                                                          // left-side value to be updated -- variable or reference

    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results, false);
    if (grc != 0) return grc;                                                             // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable & right-side parm 

    M_verb_binary_fval_integer_integer(value, verb__shift_left_eq, value1, value2)        // sets output value = value1 
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);

    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_shift_right_arithmetic_eq()
//---------------------------------------------------------------------------------

template<typename T1, typename T2> inline
T1 verb__shift_right_arithmetic_eq(T1& x, T2 y) 
{
    if  (y >= 0)
        x >>= y;
    else                    // negative right shift amount -- do left shift instead  
        x <<= -y; 
    return x;
}                                                 


int verb_shift_right_arithmetic_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side integer positional parm

    M__(M_out(L"verb_shift_right_arithmetic_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };                                                                      // left-side value should be a defined variable containing integer value 
    value_S        value2   { };                                                                      // right-side value should be an integer value
    value_S        lvalue   { };                                                                      // left-side value to be updated -- variable or reference
                                                                                                   
    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results, false);                       
    if (grc != 0) return grc;                                                                         // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable & right-side parm 

    M_verb_binary_fval_integer_integer(value, verb__shift_right_arithmetic_eq, value1, value2)        // sets output value = value1 
    
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);

    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_shift_right_logical_eq()
//---------------------------------------------------------------------------------


template<typename T1, typename T2> inline
T1 verb__shift_right_logical_eq_1(T1& x, T2 y) 
{
    if  (y >= 0)
        x = (T1)( (std::make_unsigned<T1>::type)x >>  y );
    else                                                                    // negative right shift amount -- do left shift instead  
        x = (T1)( (std::make_unsigned<T1>::type)x << -y );

    return x;
}    
 

template<typename T1, typename T2> inline
T1 verb__shift_right_logical_eq_2(T1& x, T2 y) 
{

    if  (y >= 0)
        x >>= y;
    else                    // negative right shift amount -- do left shift instead  
        x <<= -y; 
    return x;
}   


int verb_shift_right_logical_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left-side defined variable positional parm, and one right-side integer positional parm

    M__(M_out(L"verb_shift_right_logical_eq() called");)
    value_S value { };


    // get left-side and right-side evaluated values -- do checking, etc. 

    value_S        value1   { };                                                                      // left-side value should be a defined variable containing integer value 
    value_S        value2   { };                                                                      // right-side value should be an integer value
    value_S        lvalue   { };                                                                      // left-side value to be updated -- variable or reference
                                                                                                   
    auto grc = get_eq_values(frame, expression, lvalue, value1, value2, results, false);                       
    if (grc != 0) return grc;                                                                         // get_eq_values() should have already set appropriate results, if there was an error


    // set variable to:    evaluated variable & right-side parm 

    if (is_value_signed(expression.lparms.values.at(0)))     
        M_verb_binary_fval_signed_integer(  value, verb__shift_right_logical_eq_1, value1, value2) 
    else
        M_verb_binary_fval_unsigned_integer(value, verb__shift_right_logical_eq_2, value1, value2) 
        
    auto src = set_lvalue(frame, lvalue, value1);
    if (src != 0)
    {
        results = error_results(); 
        return -1; 
    }


    // return with results = value assigned to variable

    results = to_results(value);

    return 0;
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Comparison operator verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    binary comparison operators
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// internal helper funcction for compmarison operators

static int verb_compare_check(const e_expression_S& expression, results_S& results)
{
     if (is_same_class(expression.lparms.values.at(0), expression.rparms.values.at(0)))
         return 0; 

     // error -- the two values cannot be compared, since they are not the same class

      
     M_out_emsg1(L"verb__compare_check() -- verb=%s, left-side and right-side values cannot ne compared because they are not the same class") % verb_name(expression);
     msg_loc(expression.lparms.values.at(0), L" left value");  
     msg_loc(expression.rparms.values.at(0), L"right value"); 
     msgend_loc(expression);
     results = error_results();     // pass back error value  (ready for caller to pass back) 
     return -1;
}


// helper MACRO for binary comparison operators

#define M_verb_compare(vr, f, v1, v2, tf)                                                                        \
{                                                                                                                \
    auto crc =  verb_compare_check(expression, results);                                                         \
    if (crc != 0) return crc;                        /* if error, error results have been set up already   */    \
                                                                                                                 \
    if (v1.ty == type_E::unit)                                                                                   \
        vr = type_val(tf);                           /* both values unit -- return true/false (passed in) */     \
    else if (v1.ty == type_E::string)                                                                            \
        vr = type_val( f(v1.string , v2.string) );                                                               \
    else if (is_value_float(v1))                                                                                 \
        M_verb_binary_fval_float_float(vr, f, v1, v2)                                                            \
    else if (is_value_integer(v1))                                                                               \
        M_verb_binary_fval_integer_integer(vr, f, v1, v2)                                                        \
    else                                                                                                         \
        M_throw("M_verb_compare() -- unknown value type(s) passed in");                                          \
}                                                                                                                


//---------------------------------------------------------------------------------
//   verb_eq()
//---------------------------------------------------------------------------------

template<typename T1, typename T2> inline
bool verb__eq(T1 x, T2 y) {return (x == y);}


int verb_eq(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right comparable positional parm -- both are same type
    
    M__(M_out(L"verb_eq() called");)
    M_verb_compare(value, verb__eq, expression.lparms.values.at(0), expression.rparms.values.at(0), 1) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_ne()
//---------------------------------------------------------------------------------

template<typename T1, typename T2> inline
bool verb__ne(T1 x, T2 y) {return (x != y);}


int verb_ne(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right comparable positional parm -- both are same type
    
    M__(M_out(L"verb_ne() called");)
    M_verb_compare(value, verb__ne, expression.lparms.values.at(0), expression.rparms.values.at(0), 0)
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_lt()
//---------------------------------------------------------------------------------

template<typename T1, typename T2> inline
bool verb__lt(T1 x, T2 y) {return (x < y);}


int verb_lt(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right comparable positional parm -- both are same type
    
    M__(M_out(L"verb_lt() called");)
    M_verb_compare(value, verb__lt, expression.lparms.values.at(0), expression.rparms.values.at(0), 0) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_gt()
//---------------------------------------------------------------------------------

template<typename T1, typename T2> inline
bool verb__gt(T1 x, T2 y) {return (x > y);}


int verb_gt(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right comparable positional parm -- both are same type
    
    M__(M_out(L"verb_gt() called");)
    M_verb_compare(value, verb__gt, expression.lparms.values.at(0), expression.rparms.values.at(0), 0)
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_le()
//---------------------------------------------------------------------------------

template<typename T1, typename T2> inline
bool verb__le(T1 x, T2 y) {return (x <= y);}


int verb_le(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right comparable positional parm -- both are same type
    
    M__(M_out(L"verb_le() called");)
    M_verb_compare(value, verb__le, expression.lparms.values.at(0), expression.rparms.values.at(0), 1)
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_ge()
//---------------------------------------------------------------------------------

template<typename T1, typename T2> inline
bool verb__ge(T1 x, T2 y) {return (x >= y);}


int verb_ge(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right comparable positional parm -- both are same type
    
    M__(M_out(L"verb_ge() called");)
    M_verb_compare(value, verb__ge, expression.lparms.values.at(0), expression.rparms.values.at(0), 1)
    results = to_results(value);         
    return 0;
}
M_endf
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Logical operator verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    unary and binary logical operators
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------
//   verb_and()
//---------------------------------------------------------------------------------

template<typename T>  inline
bool verb__is_true(T x) {return x != 0;}


////////// with lazy evaluation  

int verb_and(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };
    bool    tf    { };   

    // already known that there is one left int64 positional parm, and one right int64, expression, or identifier parm (that might need evaluating)

    M__(M_out(L"verb_and() called");)

    // look at left-side parm -- output = false, if left side is false

    M_verb_unary_f_intbool(tf, verb__is_true, expression.lparms.values.at(0)) 
   
    if (!tf)
    {
        value = type_val(false);
    }
    else
    {
        // left side is true -- output is true/false value of right side 

        results_S right_results { }; 
        M_eval_cond(expression.rparms.values.at(0), L"verb= " + verb_name(expression) + L" right parm", right_results)  
    
        M_verb_unary_f_intbool(tf, verb__is_true, right_results) 
        value = type_val(tf);        
    }

    results = to_results(value);         // output results = results of operation
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_or()
//---------------------------------------------------------------------------------

////////// with lazy evaluation      

int verb_or(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };
    bool    tf    { };  

    // already known that there is one left int64 positional parm, and one right int64, expression, or identifier parm (that might need evaluating)

    M__(M_out(L"verb_or() called");)

  
    // check left side parm -- output is true, if left side is true

    M_verb_unary_f_intbool(tf, verb__is_true, expression.lparms.values.at(0)) 
    if (tf)
    {
        value = type_val(true); 
    }
    else
    {
        // left side is false -- output is true/false value of right side 

        results_S right_results { }; 
        M_eval_cond(expression.rparms.values.at(0), L"verb= " + verb_name(expression) + L" right parm", right_results)  
    
        M_verb_unary_f_intbool(tf, verb__is_true, right_results)
        value = type_val(tf); 
    }

    results = to_results(value);         // output results = results of operation
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_xor()
//---------------------------------------------------------------------------------

template<typename T1, typename T2>  inline
bool verb__xor(T1 x, T2 y) {return ( (!x) != (!y) );}
 
/////// without lazy evaluation

int verb_xor(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right int64 positional parm 
    M__(M_out(L"verb_xor() called");)

    M_verb_binary_fval_integer_integer(value, verb__xor, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_not()
//---------------------------------------------------------------------------------

template<typename T>  inline
bool verb__not(T x) {return (!x);}


int verb_not(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one right int64 positional parm  
    M__(M_out(L"verb_not() called");) 

    M_verb_unary_fval_intbool(value, verb__not, expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf



 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Bitwise operator verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//---------------------------------------------------------------------------------
//   verb_bitand()
//---------------------------------------------------------------------------------

template<typename T>  inline
T verb__bitand(T x, T y) {return x & y;}


int verb_bitand(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right integer parm 
    M__(M_out(L"verb_bitand() called");)

    M_verb_binary_fval_integer(value, verb__bitand, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_bitor()
//---------------------------------------------------------------------------------

template<typename T>  inline
T verb__bitor(T x, T y) {return x | y;}


int verb_bitor(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right integer parm 
    M__(M_out(L"verb_bitor() called");)

    M_verb_binary_fval_integer(value, verb__bitor, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf




//---------------------------------------------------------------------------------
//   verb_bitxor()
//---------------------------------------------------------------------------------

template<typename T>  inline
T verb__bitxor(T x, T y) {return x ^ y;}


int verb_bitxor(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right integer parm 
    M__(M_out(L"verb_bitxor() called");)

    M_verb_binary_fval_integer(value, verb__bitxor, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_bitnot()
//---------------------------------------------------------------------------------

template<typename T>  inline
T verb__bitnot(T x) {return ~x;}


int verb_bitnot(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one right integer parm  
    M__(M_out(L"verb_bitand() called");) 

    M_verb_unary_fval_integer(value, verb__bitnot, expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_shift_left()
//---------------------------------------------------------------------------------

template<typename T1, typename T2>  inline
T1 verb__shift_left(T1 x, T2 y) 
{
    if  (y >= 0)
        return x << y;
    else                    // negative left shift amount -- do right shift instead  
        return x >> -y; 
}


int verb_shift_left(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right integer parm 
    M__(M_out(L"verb_shift_left() called");)

    M_verb_binary_fval_integer_integer(value, verb__shift_left, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_shift_right_arithmetic()
//---------------------------------------------------------------------------------

template<typename T1, typename T2>  inline
T1 verb__shift_right_arithmetic(T1 x, T2 y) 
{
    if  (y >= 0)
        return x >> y;
    else                    // negative right shift amount -- do left shift instead  
        return x << -y; 
}


int verb_shift_right_arithmetic(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right integer parm 
    M__(M_out(L"verb_shift_right_arithmetic() called");)

    M_verb_binary_fval_integer_integer(value, verb__shift_right_arithmetic, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf



//---------------------------------------------------------------------------------
//   verb_shift_right_logical()
//---------------------------------------------------------------------------------

template<typename T1, typename T2>  inline
T1 verb__shift_right_logical_1(T1 x, T2 y) 
{
    if  (y >= 0)
        return (T1)((std::make_unsigned<T1>::type)x >>  y);
    else                                                        // negative right shift amount -- do left shift instead  
        return (T1)((std::make_unsigned<T1>::type)x << -y); 
}


template<typename T1, typename T2>  inline
T1 verb__shift_right_logical_2(T1 x, T2 y) 
{
    if  (y >= 0)
        return x >> y;
    else                    // negative right shift amount -- do left shift instead  
        return x << -y; 
}


int verb_shift_right_logical(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right integer parm 
    M__(M_out(L"verb_shift_right_logical() called");)

    if (is_value_signed(expression.lparms.values.at(0)))     
        M_verb_binary_fval_signed_integer(  value, verb__shift_right_logical_1, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    else
        M_verb_binary_fval_unsigned_integer(value, verb__shift_right_logical_2, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Binary arithmetic and string operators
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    binary arithmetic operators
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------
//   verb_add()
//---------------------------------------------------------------------------------
                                                                                   
template<typename T> inline
T verb__add(T x, T y)  {return x + y;}


int verb_add(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right arithmetic positional parm -- both are same type

    M__(M_out(L"verb_add() called");)
    M_verb_binary_fval_arith( value, verb__add, expression.lparms.values.at(0), expression.rparms.values.at(0) ) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_minus()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__subtract(T x, T y) {return x - y;}

template<typename T> inline
T verb__negate(T x) {return -x;}

//////////////////////////////// can be negation (unary prefix) or subtract (binary)

int verb_minus(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is zero or one left and one right arithmetic positional parm -- both are same type

    M__(M_out(L"verb_minus() called");)

    if (expression.lparms.value_ct == 0)   // unary minus? 
         M_verb_unary_fval_arith(  value, verb__negate,   expression.rparms.values.at(0)                             )
    else
         M_verb_binary_fval_arith( value, verb__subtract, expression.lparms.values.at(0), expression.rparms.values.at(0) ) 

    results = to_results(value);         // output results = results of operation
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_multiply()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__multiply(T x, T y) {return x * y;}


int verb_multiply(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right arithmetic positional parm -- both are same type

    M__(M_out(L"verb_multiply() called");)
    M_verb_binary_fval_arith(value, verb__multiply, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_divide()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__divide(T x, T y) {return x / y;}


int verb_divide(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right arithmetic positional parm -- both are same type

    M__(M_out(L"verb_divide() called");)
    M_verb_binary_fval_arith(value, verb__divide, expression.lparms.values.at(0), expression.rparms.values.at(0))    
    results = to_results(value);         
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_power()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__power(T x, T y) {return power(x, y);}


int verb_power(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right arithmetic positional parm -- both are same type

    M__(M_out(L"verb_power() called");)
    M_verb_binary_fval_arith(value, verb__power, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);     
    return 0;
}
M_endf


//---------------------------------------------------------------------------------
//   verb_remainder()
//---------------------------------------------------------------------------------

template<typename T> inline
T verb__remainder(T x, T y) {return x % y;}


int verb_remainder(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    value_S value { };

    // already known that there is one left and one right integer positional parm -- both are same type

    M__(M_out(L"verb_remainder() called");)        
    M_verb_binary_fval_integer(value, verb__remainder, expression.lparms.values.at(0), expression.rparms.values.at(0)) 
    results = to_results(value);         
    return 0;
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    binary string operators
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------
//   verb_concatenate()
//---------------------------------------------------------------------------------

int verb_concatenate(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one left and one right string positional parm

    M__(M_out(L"verb_concatenate() called");)

    results = to_results(string_val(expression.lparms.values.at(0).string + expression.rparms.values.at(0).string)); 
    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""