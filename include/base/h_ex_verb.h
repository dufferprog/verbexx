// h_ex_verb.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            =========== 
////            h_ex_verb.h -- functions in   ex_verb_util.cpp,   ex_verb_op.cpp,   ex_verb_ctl.cpp, and   ex_addverb.cpp (pre-defined general verbs) 
////            ===========
////     
////   
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_ex_types.h"            


                                                                                                                          
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//    error-reporting MACROs
/////////////////////////////////////////////////////////////////////////////

// increment error_count and report error results
// ----------------------------------------------

#define M_verb_error1(rv)                                \
{                                                        \
     count_error();                                      \
     rv = error_results();                               \
     return -1;                                          \
}


// just report error results -- don't increment counter
// ----------------------------------------------------

#define M_verb_error0(rv)                                \
{                                                        \
     rv = error_results();                               \
     return -1;                                          \
}


// increment error_count, put out location message, and report error results
// -------------------------------------------------------------------------

#define M_verb_error1_loc(rv, v, e)                      \
{                                                        \
     count_error();                                      \
     msgend_loc((v), (e));                               \
     rv = error_results();                               \
     return -1;                                          \
}


// just put out location message and report error results -- don't increment counter
// ---------------------------------------------------------------------------------

#define M_verb_error0_loc(rv, v, e)                      \
{                                                        \
     msgend_loc((v), (e));                               \
     rv = error_results();                               \
     return -1;                                          \
}




/////////////////////////////////////////////////////////////////////////////

// MACRO to call eval_cond() and return if R/C is non-zero  (see definition below)
// eval_cond will return -1 if error and +1 if special results -- both these cause immediate return without consuming the special results

#define M_eval_cond(v, s, r)                             \
{                                                        \
    auto e_rc = eval_cond(frame, (v), expression, s, r); \
    if (e_rc != 0)                                       \
    {                                                    \
        results = r;                                     \
        return std::min(e_rc, 0);                        \
    }                                                    \
}


//////////////////////////////////////////////////////////////////////////
//    external functions -- ex_addverb.cpp 
//////////////////////////////////////////////////////////////////////////

//void add_predefined_typdefs();        in ex_interface.h
//void add_predefined_verbs();          in ex_interface.h



//////////////////////////////////////////////////////////////////////////
//     ex_verb_ctl.cpp 
//////////////////////////////////////////////////////////////////////////
   
//   externalized utility and helper functions 
//   -----------------------------------------

int verb_non_builtin(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);


//   flow-of-control verbs
//   ---------------------

int verb_verb(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_lambda(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_fn(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_lambda(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_unverb(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_initverb(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_call(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_xctl(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_include(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_skip(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_skipto(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_import(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_parse(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_shell(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_separate(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_sequence(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_break(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_goto(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_leave(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);  
int verb_return(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);  
int verb_throw(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&); 
int verb_rethrow(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&); 
int verb_try(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);  
int verb_continue(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_quit(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_end(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_exit(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_error(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);  
int verb_do(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_block(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_case(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_if(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_cond(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_loop(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
                                                          

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//      ex_verb_util.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                                          
//   externalized utility and helper  functions
//   ------------------------------------------

 int eval_cond(frame_S&l, const value_S&, const e_expression_S&, const std::wstring&, results_S&);


// Variable definition and assignment verbs
// ---------------------------------------

int verb_var(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_const(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_unvar(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_alias(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_unalias(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
#ifdef M_EXPOSE_SUPPORT
int verb_expose(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_unexpose(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
#endif


// evaluation-control verbs
// ------------------------

int verb_pass(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_noeval(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_eval(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_unshare(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);


// other verbs
// -----------

int verb_getenv(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);                                                               


//  Console Input/Output-oriented and output string-oriented verbs 
// ---------------------------------------------------------------

int verb_interpolate(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_format(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_text(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_str(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_say(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_stdin(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_stdout(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_stderr(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);


// other output-oriented verbs
// ---------------------------

int verb_display(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&); 
int verb_debug(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);    
 


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//      ex_verb_op.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////
                
// plain assignment operators 
// --------------------------

int verb_left_assign(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_right_assign(                frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);


// Arithmetic and string assignment operators
// ------------------------------------------

int verb_increment(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_decrement(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_add_eq(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_subtract_eq(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);                                                                           
int verb_multiply_eq(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_divide_eq(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_power_eq(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_remainder_eq(                frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_concatenate_eq(              frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
                                                               

// bitwise assignment operators
// ----------------------------

int verb_bitand_eq(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_bitor_eq(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_bitxor_eq(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
//int verb_bitnot_eq(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_shift_left_eq(               frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_shift_right_logical_eq(      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_shift_right_arithmetic_eq(   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&); 


// Comparison operators 
// --------------------

int verb_eq(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_ne(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_lt(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_gt(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_le(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_ge(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
                                    

// logical operators
// -----------------

int verb_and(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_or(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_xor(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_not(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);


// bitwise operators 
// -----------------

int verb_bitand(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_bitor(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_bitxor(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_bitnot(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_shift_left(                  frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_shift_right_logical(         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_shift_right_arithmetic(      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
                                                              

// arithmetic and string operators
// -------------------------------

int verb_add(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_minus(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_multiply(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_divide(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_power(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_remainder(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_concatenate(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//     ex_verb_aggr.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// type-oriented verbs 
// -------------------

int verb_type(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);

int verb_to_bool(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_int8(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_int16(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_int32(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_int64(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_uint8(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_uint16(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_uint32(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_uint64(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_float32(                  frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_float64(                  frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_str(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_ident(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_to_verbname(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
                                    
int verb_is_unit(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_bool(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_int8(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_int16(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_int32(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_int64(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_uint8(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_uint16(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_uint32(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_uint64(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_float32(                  frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_float64(                  frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_int(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_float(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_signed(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_unsigned(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_numeric(                  frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_atom(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_true(                     frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_false(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_string(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_var(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_const(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_def(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_expression(               frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_vlist(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_block(                    frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_identifier(               frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_is_verbname(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);


// aggregate-oriented verbs 
//-------------------------

 int verb_agg(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
 

// structure, array, argument and vlist extracion verbs
 //----------------------------------------------------

int verb_subscript(                   frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_select(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);

//int verb_arg_assign(                frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_arg_ct(                      frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_args(                        frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_arg(                         frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
                                                         
//int verb_vl_assign(                 frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_at(                          frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);
int verb_vl_ct(                       frame_S& eval, const e_expression_S&, const verbdef_S&, results_S&);

    


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////