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


                                                                                                                          
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Local MACRO to call eval_cond() and return if R/C is non-zero  (see definition below)

#define M_eval_cond(v, s, r)                        \
{                                                   \
    auto e_rc = eval_cond(frame, (v), vexpr, s, r); \
    if (e_rc != 0)                                  \
    {                                               \
        results = r;                                \
        return std::min(e_rc, 0);                   \
    }                                               \
}




/////////////////////////////////////////////////////////////////////////////
// verb priorities -- static constants
/////////////////////////////////////////////////////////////////////////////


static const int verb_priority_attached_paren      {  1000 };  
                                                     
static const int verb_priority_select              {   120 };  
static const int verb_priority_subscript           {   120 };  
                                                     
static const int verb_priority_increment           {   100 };
static const int verb_priority_not                 {   100 };
static const int verb_priority_bitnot              {   100 };
                                                     
static const int verb_priority_at                  {    80 };  
static const int verb_priority_power               {    70 };
static const int verb_priority_multiply            {    60 }; 
static const int verb_priority_add                 {    50 }; 
static const int verb_priority_shift               {    40 }; 
static const int verb_priority_compare             {    30 }; 
static const int verb_priority_bitwise             {    20 }; 
static const int verb_priority_boolean             {    10 };
static const int verb_priority_default             {     0 };
static const int verb_priority_assign              {   -10 };
                                                     
static const int verb_priority_separate            { -1000 };

 


//////////////////////////////////////////////////////////////////////////
//    external functions -- ex_addverb.cpp 
//////////////////////////////////////////////////////////////////////////

void add_predefined_typdefs();
void add_predefined_verbs(); 



//////////////////////////////////////////////////////////////////////////
//    external functions -- verb_xx.cpp 
//////////////////////////////////////////////////////////////////////////
   
//   externalized helper functions -- verb.cpp

 int eval_cond(frame_S&l, const value_S&, const e_vexpr_S&, const std::wstring&, results_S&);


//   flow-of-control verbs -- verb_ctl.cpp

int verb_non_builtin(                 frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_verb(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_fn(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_unverb(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_call(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_xctl(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_parse(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_shell(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_separate(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_break(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_goto(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_leave(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);  
int verb_return(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);  
int verb_throw(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&); 
int verb_rethrow(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&); 
int verb_try(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);  
int verb_continue(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_quit(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_end(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_exit(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb__exit(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_quick_exit(                  frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_abort(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_do(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_block(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_case(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_if(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_loop(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
                                                          

// Variable definition and assignment verbs -- verb_util.cpp

int verb_var(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_const(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_noeval(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_eval(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_unshare(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_export(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_unexport(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_unvar(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
                                                               


// plain assignment operators -- verb_op.cpp


int verb_left_assign(                 frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_right_assign(                frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);


// Arithmetic and string assignment operators -- verb_op.cpp

int verb_increment(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_decrement(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_add_eq(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_subtract_eq(                 frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);                                                                           
int verb_multiply_eq(                 frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_divide_eq(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_power_eq(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_remainder_eq(                frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_concatenate_eq(              frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
                                                               

// bitwise assignment operators  -- verb_op.cpp

int verb_bitand_eq(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_bitor_eq(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_bitxor_eq(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
//int verb_bitnot_eq(                 frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_shift_left_eq(               frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_shift_right_logical_eq(      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_shift_right_arithmetic_eq(   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&); 


// Comparison operators -- verb_op.cpp                             

int verb_eq(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_ne(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_lt(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_gt(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_le(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_ge(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
                                    

// logical operators  -- verb_op.cpp

int verb_and(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_or(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_xor(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_not(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);


// bitwise operators  -- verb_op.cpp

int verb_bitand(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_bitor(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_bitxor(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_bitnot(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_shift_left(                  frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_shift_right_logical(         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_shift_right_arithmetic(      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
                                                              

// arithmetic and string operators  -- verb_op.cpp

int verb_add(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_minus(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_multiply(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_divide(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_power(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_remainder(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_concatenate(                 frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);


// aggregate-oriented verbs  -- verb_util.cpp

 int verb_agg(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
 

// type-oriented verbs  -- verb_util.cpp

int verb_type(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);

int verb_to_bool(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_int8(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_int16(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_int32(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_int64(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_uint8(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_uint16(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_uint32(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_uint64(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_float32(                  frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_float64(                  frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_str(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_to_ident(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
                                    
int verb_is_unit(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_bool(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_int8(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_int16(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_int32(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_int64(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_uint8(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_uint16(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_uint32(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_uint64(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_float32(                  frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_float64(                  frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_int(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_float(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_signed(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_unsigned(                 frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_numeric(                  frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_atom(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_true(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_false(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_string(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_var(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_const(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_def(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_vexpr(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_vlist(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_is_slist(                    frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);


// structure, array, argument and vlist extracion verbs  -- verb_util.cpp

int verb_subscript(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_select(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);

int verb_getenv(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_arg_assign(                  frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_arg_ct(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_args(                        frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_arg(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
                                                         
int verb_vl_assign(                   frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_at(                          frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_vl_ct(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);


//  Console Input/Output-oriented verbs  -- verb_util.cpp

int verb_str(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_say(                         frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_stdin(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_stdout(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_stderr(                      frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);
int verb_display(                     frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);               
int verb_debug(                       frame_S& eval, const e_vexpr_S&, const verbdef_S&, results_S&);    


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////