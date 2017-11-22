// h_ex_interface.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ================ 
////            h_ex_interface.h -- Public ex-oriented functions, and related structures, etc. 
////            ================
////     
////   
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_ex_types.h"            


/////////////// MACROs for displaying verb (debug)  ////////////////////////////////////////////////////////

#define M_dv_all(  x )  M_if(log_verbs() >= log_E::all , x)
#define M_dv_most( x )  M_if(log_verbs() >= log_E::most, x)
#define M_dv_some( x )  M_if(log_verbs() >= log_E::some, x)
#define M_dv_few(  x )  M_if(log_verbs() >= log_E::few , x) 




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////  Macros for verbs to use when extracting parms  -- also see get_right_keyword(), etc. callable functions 
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// i = expression  k = keyword name    f = field to set   v = value to set field to,  t = type to cast parm value to field value     
//
//         examples:      M_get_right_keyword_nval(  expression, L"display", xparm.display, true   )
//                        M_get_right_keyword_string(expression, L"name"   , xparm.name            )
//                        M_get_right_keyword_int64( expression, L"width"  , xparm.width  , int32_t)
//
//
//                        M_get_right_keyword_vlist(expression, L"filenames",  filenames_vl)

#define M_get_right_keyword_nval(     i, k, f, v)   {if (i.rparms.eval_kws.count(k) > 0) f  =    (v                                                    );  }
#define M_get_right_keyword_string(   i, k, f   )   {if (i.rparms.eval_kws.count(k) > 0) f  =     i.rparms.eval_kws.find(k)->second.string              ;  }
#define M_get_right_keyword_int64(    i, k, f, t)   {if (i.rparms.eval_kws.count(k) > 0) f  = (t)(i.rparms.eval_kws.find(k)->second.int64              );  }
#define M_get_right_keyword_float64(  i, k, f, t)   {if (i.rparms.eval_kws.count(k) > 0) f  = (t)(i.rparms.eval_kws.find(k)->second.float64            );  }


#define M_get_right_keyword_vlist(    i, k, l   )               \
vlist_S *l##_p {nullptr};                                       \
vlist_S  l     {       };                                       \
                                                                \
if (i.rparms.eval_kws.count(k) > 0)                             \
    l##_p = i.rparms.eval_kws.find(k)->second.vlist_sp.get();   \
if (l##_p != nullptr)                                           \
    l = *l##_p;   


#define M_get_left_keyword_nval(      i, k, f, v)   {if (i.lparms.eval_kws.count(k) > 0) f  =    (v                                                    );  }
#define M_get_left_keyword_string(    i, k, f   )   {if (i.lparms.eval_kws.count(k) > 0) f  =     i.lparms.eval_kws.find(k)->second.string              ;  }
#define M_get_left_keyword_int64(     i, k, f, t)   {if (i.lparms.eval_kws.count(k) > 0) f  = (t)(i.lparms.eval_kws.find(k)->second.int64              );  }
#define M_get_left_keyword_float64(   i, k, f, t)   {if (i.lparms.eval_kws.count(k) > 0) f  = (t)(i.lparms.eval_kws.find(k)->second.float64            );  }


#define M_get_left_keyword_vlist(    i, k, l   )                \
vlist_S *l##_p {nullptr};                                       \
vlist_S  l     {       };                                       \
                                                                \
if (i.rparms.eval_kws.count(k) > 0)                             \
    l##_p = i.lparms.eval_kws.find(k)->second.vlist_sp.get();   \
if (l##_p != nullptr)                                           \
    l = *l##_p;  





// l = vlist  k = keyword name    f = field to set   v = value to set field to,  t = type to cast parm value to field value
//
//         examples:      M_get_nest_keyword_nval(  vlist, L"display", xparm.display, true   )
//                        M_get_nest_keyword_string(vlist, L"name"   , xparm.name   ,        )
//                        M_get_nest_keyword_int64( vlist, L"width"  , xparm.width  , int32_t)

#define M_get_nest_keyword_nval(     l, k, f, v)   {if (l.eval_kws.count(k) > 0) f =    (v                                 );  }
#define M_get_nest_keyword_string(   l, k, f   )   {if (l.eval_kws.count(k) > 0) f =     l.eval_kws.find(k)->second.string  ;  }
#define M_get_nest_keyword_int64(    l, k, f, t)   {if (l.eval_kws.count(k) > 0) f = (t)(l.eval_kws.find(k)->second.int64  );  }
#define M_get_nest_keyword_float64(  l, k, f, t)   {if (l.eval_kws.count(k) > 0) f = (t)(l.eval_kws.find(k)->second.float64);  }

#define M_get_nest_keyword_vlist(    l, k, v   )                \
vlist_S *v##_p {nullptr};                                       \
vlist_S  v     {       };                                       \
                                                                \
if (l.eval_kws.count(k) > 0)                                    \
    v##_p = l.eval_kws.find(k)->second.vlist_sp.get();          \
if (v##_p != nullptr)                                           \
    v = *v##_p;  







// following MACROs get n-th right/left positional parm, if present
//
//    example -- xparm.length = (int16_t)M_get_right_pos_int64(expression, 3) -- gets 4th positional parm  
//
//    i = expression  ,  n = n-th positional parm, l = nested vlist
//


#define M_get_right_pos(                   i, n      )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n))                  : value_S       { } )
#define M_get_right_pos_int32(             i, n      )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).int32            : 0                 )
#define M_get_right_pos_int64(             i, n      )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).int64            : 0                 )
#define M_get_right_pos_int32_d(           i, n, d   )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).int32            : (d)               )
#define M_get_right_pos_int64_d(           i, n, d   )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).int64            : (d)               )
#define M_get_right_pos_float64(           i, n      )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).float64          : 0.0               )
#define M_get_right_pos_string(            i, n      )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).string           : std::wstring  { } )
#define M_get_right_pos_ident(             i, n      )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).string           : std::wstring  { } )
#define M_get_right_pos_typdef_p(          i, n      )             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).typdef_sp.get()  : nullptr           )
                                                                                                                                                             
#define M_get_left_pos(                    i, n      )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n))                  : value_S       { } )
#define M_get_left_pos_int32(              i, n      )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).int32            : 0                 )
#define M_get_left_pos_int64(              i, n      )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).int64            : 0                 )
#define M_get_left_pos_int32_d(            i, n, d   )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).int32            : (d)               )
#define M_get_left_pos_int64_d(            i, n, d   )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).int64            : (d)               )
#define M_get_left_pos_float64(            i, n      )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).float64          : 0.0               )
#define M_get_left_pos_string(             i, n      )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).string           : std::wstring  { } )
#define M_get_left_pos_ident(              i, n      )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).string           : std::wstring  { } )
#define M_get_left_pos_typdef_p(           i, n      )             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).typdef_sp.get()  : nullptr           )
                                                                                                                                                             
#define M_get_nest_pos(                    l, n      )             ( (l)     .values.size() > (n) ? (l)     .values.at((n))                  : value_S       { } )
#define M_get_nest_pos_int32(              l, n      )             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).int32            : 0                 )
#define M_get_nest_pos_int64(              l, n      )             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).int64            : 0                 )
#define M_get_nest_pos_int32_d(            l, n, d   )             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).int32            : (d)               )
#define M_get_nest_pos_int64_d(            l, n, d   )             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).int64            : (d)               )
#define M_get_nest_pos_float64(            l, n      )             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).float64          : 0.0               )
#define M_get_nest_pos_string(             l, n      )             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).string           : std::wstring  { } )
#define M_get_nest_pos_ident(              l, n      )             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).string           : std::wstring  { } )
#define M_get_nest_pos_typdef_p(           l, n      )             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).typdef_sp.get()  : nullptr           )



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////  Macros for use in applying template functions to values of different types
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//       Type-oriented MACROs for unary functions -- value input / field output
//       ---------------------------------------------------------------------

#define M_verb_unary_f_integer(r, f, v1)                                          \
{                                                                                 \
             if (v1.ty == type_E::int8   ) r = f(v1.int8   );                     \
        else if (v1.ty == type_E::int16  ) r = f(v1.int16  );                     \
        else if (v1.ty == type_E::int32  ) r = f(v1.int32  );                     \
        else if (v1.ty == type_E::int64  ) r = f(v1.int64  );                     \
        else if (v1.ty == type_E::uint8  ) r = f(v1.uint8  );                     \
        else if (v1.ty == type_E::uint16 ) r = f(v1.uint16 );                     \
        else if (v1.ty == type_E::uint32 ) r = f(v1.uint32 );                     \
        else if (v1.ty == type_E::uint64 ) r = f(v1.uint64 );                     \
        else M_throw("M_verb_unary_f_integer() -- unknown value type passed in"); \
}

#define M_verb_unary_f_intbool(r, f, v1)                                          \
{                                                                                 \
             if (v1.ty == type_E::boolean) r = f(v1.boolean);                     \
        else if (v1.ty == type_E::int8   ) r = f(v1.int8   );                     \
        else if (v1.ty == type_E::int16  ) r = f(v1.int16  );                     \
        else if (v1.ty == type_E::int32  ) r = f(v1.int32  );                     \
        else if (v1.ty == type_E::int64  ) r = f(v1.int64  );                     \
        else if (v1.ty == type_E::uint8  ) r = f(v1.uint8  );                     \
        else if (v1.ty == type_E::uint16 ) r = f(v1.uint16 );                     \
        else if (v1.ty == type_E::uint32 ) r = f(v1.uint32 );                     \
        else if (v1.ty == type_E::uint64 ) r = f(v1.uint64 );                     \
        else M_throw("M_verb_unary_f_intbool() -- unknown value type passed in"); \
}

#define M_verb_unary_f_arith(r, f, v1)                                           \
{                                                                                \
             if (v1.ty == type_E::int8   ) r = f(v1.int8   );                    \
        else if (v1.ty == type_E::int16  ) r = f(v1.int16  );                    \
        else if (v1.ty == type_E::int32  ) r = f(v1.int32  );                    \
        else if (v1.ty == type_E::int64  ) r = f(v1.int64  );                    \
        else if (v1.ty == type_E::uint8  ) r = f(v1.uint8  );                    \
        else if (v1.ty == type_E::uint16 ) r = f(v1.uint16 );                    \
        else if (v1.ty == type_E::uint32 ) r = f(v1.uint32 );                    \
        else if (v1.ty == type_E::uint64 ) r = f(v1.uint64 );                    \
        else if (v1.ty == type_E::float32) r = f(v1.float32);                    \
        else if (v1.ty == type_E::float64) r = f(v1.float64);                    \
        else M_throw("M_verb_unary_f_arith() -- unknown value type passed in");  \
}

#define M_verb_unary_f_compare(r, f, v1)                                           \
{                                                                                  \
             if (v1.ty == type_E::int8   ) r = f(v1.int8   );                      \
        else if (v1.ty == type_E::int16  ) r = f(v1.int16  );                      \
        else if (v1.ty == type_E::int32  ) r = f(v1.int32  );                      \
        else if (v1.ty == type_E::int64  ) r = f(v1.int64  );                      \
        else if (v1.ty == type_E::uint8  ) r = f(v1.uint8  );                      \
        else if (v1.ty == type_E::uint16 ) r = f(v1.uint16 );                      \
        else if (v1.ty == type_E::uint32 ) r = f(v1.uint32 );                      \
        else if (v1.ty == type_E::uint64 ) r = f(v1.uint64 );                      \
        else if (v1.ty == type_E::float32) r = f(v1.float32);                      \
        else if (v1.ty == type_E::float64) r = f(v1.float64);                      \
        else if (v1.ty == type_E::string ) r = f(v1.string );                      \
        else if (v1.ty == type_E::boolean) r = f(v1.boolean);                      \
        else if (v1.ty == type_E::unit   ) r = f((uint8_t)0);                      \
        else M_throw("M_verb_unary_f_compmare() -- unknown value type passed in"); \
}



//       Type-oriented MACROs for unary functions -- value input / value output
//       ----------------------------------------------------------------------

#define M_verb_unary_fval_integer(vr, f, v1)                                         \
{                                                                                    \
             if (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8   ));            \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16  ));            \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32  ));            \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64  ));            \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8  ));            \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16 ));            \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32 ));            \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64 ));            \
        else M_throw("M_verb_unary_fval_integer() -- unknown value type passed in"); \
}

#define M_verb_unary_fval_intbool(vr, f, v1)                                         \
{                                                                                    \
             if (v1.ty == type_E::boolean) vr = type_val( f(v1.boolean));            \
        else if (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8   ));            \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16  ));            \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32  ));            \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64  ));            \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8  ));            \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16 ));            \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32 ));            \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64 ));            \
        else M_throw("M_verb_unary_fval_intbool() -- unknown value type passed in"); \
}

#define M_verb_unary_fval_arith(vr, f, v1)                                            \
{                                                                                     \
             if (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8   ));             \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16  ));             \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32  ));             \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64  ));             \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8  ));             \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16 ));             \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32 ));             \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64 ));             \
        else if (v1.ty == type_E::float32) vr = type_val( f(v1.float32));             \
        else if (v1.ty == type_E::float64) vr = type_val( f(v1.float64));             \
        else M_throw("M_verb_unary_fval_arith() -- unknown value type passed in");    \
}

#define M_verb_unary_fval_compare(vr, f, v1)                                          \
{                                                                                     \
             if (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8   ));             \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16  ));             \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32  ));             \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64  ));             \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8  ));             \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16 ));             \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32 ));             \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64 ));             \
        else if (v1.ty == type_E::float32) vr = type_val( f(v1.float32));             \
        else if (v1.ty == type_E::float64) vr = type_val( f(v1.float64));             \
        else if (v1.ty == type_E::string ) vr = type_val( f(v1.string ));             \
        else if (v1.ty == type_E::boolean) vr = type_val( f(v1.boolean));             \
        else if (v1.ty == type_E::unit   ) vr = unit_val(              );             \
        else M_throw("M_verb_unary_fval_compare() -- unknown value type passed in");  \
}
 


//       Type-oriented MACROs for unary functions -- value input / value output -- with location info
//       --------------------------------------------------------------------------------------------

#define M_verb_unary_fvalix_integer(vr, f, v1, ix1, ix2)                                       \
{                                                                                              \
             if (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8   ), ix1, ix2);            \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16  ), ix1, ix2);            \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32  ), ix1, ix2);            \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64  ), ix1, ix2);            \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8  ), ix1, ix2);            \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16 ), ix1, ix2);            \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32 ), ix1, ix2);            \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64 ), ix1, ix2);            \
        else M_throw("M_verb_unary_fvalix_integer() -- unknown value type passed in");         \
}

#define M_verb_unary_fvalix_intbool(vr, f, v1, ix1, ix2)                                       \
{                                                                                              \
             if (v1.ty == type_E::boolean) vr = type_val( f(v1.boolean), ix1, ix2);            \
        else if (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8   ), ix1, ix2);            \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16  ), ix1, ix2);            \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32  ), ix1, ix2);            \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64  ), ix1, ix2);            \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8  ), ix1, ix2);            \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16 ), ix1, ix2);            \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32 ), ix1, ix2);            \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64 ), ix1, ix2);            \
        else M_throw("M_verb_unary_fvalix_intbool() -- unknown value type passed in");         \
}

#define M_verb_unary_fvalix_arith(vr, f, v1, ix1, ix2)                                          \
{                                                                                               \
             if (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8   ), ix1, ix2);             \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16  ), ix1, ix2);             \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32  ), ix1, ix2);             \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64  ), ix1, ix2);             \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8  ), ix1, ix2);             \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16 ), ix1, ix2);             \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32 ), ix1, ix2);             \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64 ), ix1, ix2);             \
        else if (v1.ty == type_E::float32) vr = type_val( f(v1.float32), ix1, ix2);             \
        else if (v1.ty == type_E::float64) vr = type_val( f(v1.float64), ix1, ix2);             \
        else M_throw("M_verb_unary_fvalix_arith() -- unknown value type passed in");           \
}

#define M_verb_unary_fvalix_compare(vr, f, v1, ix1, ix2)                                        \
{                                                                                               \
             if (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8   ), ix1, ix2);             \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16  ), ix1, ix2);             \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32  ), ix1, ix2);             \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64  ), ix1, ix2);             \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8  ), ix1, ix2);             \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16 ), ix1, ix2);             \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32 ), ix1, ix2);             \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64 ), ix1, ix2);             \
        else if (v1.ty == type_E::float32) vr = type_val( f(v1.float32), ix1, ix2);             \
        else if (v1.ty == type_E::float64) vr = type_val( f(v1.float64), ix1, ix2);             \
        else if (v1.ty == type_E::string ) vr = type_val( f(v1.string ), ix1, ix2);             \
        else if (v1.ty == type_E::boolean) vr = type_val( f(v1.boolean), ix1, ix2);             \
        else if (v1.ty == type_E::unit   ) vr = unit_val(                ix1, ix2);             \
        else M_throw("M_verb_unary_fvalix_compare() -- unknown value type passed in");          \
}






//       Type-oriented MACROs for binary functions -- value input / value output  (two input types)
//       -----------------------------------------------------------------------


#define M_verb_binary_fval_integer_integer(vr, f, v1, v2)                                                                \
{                                                                                                                        \
             if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.int8   , v2.int8  ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.int8   , v2.int16 ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.int8   , v2.int32 ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.int8   , v2.int64 ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.int8   , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.int8   , v2.uint16) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.int8   , v2.uint32) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.int8   , v2.uint64) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.int16  , v2.int8  ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.int16  , v2.int16 ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.int16  , v2.int32 ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.int16  , v2.int64 ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.int16  , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.int16  , v2.uint16) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.int16  , v2.uint32) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.int16  , v2.uint64) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.int32  , v2.int8  ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.int32  , v2.int16 ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.int32  , v2.int32 ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.int32  , v2.int64 ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.int32  , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.int32  , v2.uint16) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.int32  , v2.uint32) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.int32  , v2.uint64) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.int64  , v2.int8  ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.int64  , v2.int16 ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.int64  , v2.int32 ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.int64  , v2.int64 ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.int64  , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.int64  , v2.uint16) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.int64  , v2.uint32) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.int64  , v2.uint64) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.uint8  , v2.int8  ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.uint8  , v2.int16 ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.uint8  , v2.int32 ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.uint8  , v2.int64 ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.uint8  , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.uint8  , v2.uint16) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.uint8  , v2.uint32) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.uint8  , v2.uint64) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.uint16 , v2.int8  ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.uint16 , v2.int16 ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.uint16 , v2.int32 ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.uint16 , v2.int64 ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.uint16 , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.uint16 , v2.uint16) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.uint16 , v2.uint32) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.uint16 , v2.uint64) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.uint32 , v2.int8  ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.uint32 , v2.int16 ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.uint32 , v2.int32 ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.uint32 , v2.int64 ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.uint32 , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.uint32 , v2.uint16) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.uint32 , v2.uint32) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.uint32 , v2.uint64) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.uint64 , v2.int8  ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.uint64 , v2.int16 ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.uint64 , v2.int32 ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.uint64 , v2.int64 ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.uint64 , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.uint64 , v2.uint16) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.uint64 , v2.uint32) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.uint64 , v2.uint64) );    \
}                                        


#define M_verb_binary_fval_signed_integer(vr, f, v1, v2)                                                                 \
{                                                                                                                        \
             if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.int8   , v2.int8  ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.int8   , v2.int16 ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.int8   , v2.int32 ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.int8   , v2.int64 ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.int8   , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.int8   , v2.uint16) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.int8   , v2.uint32) );    \
        else if ( (v1.ty == type_E::int8  ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.int8   , v2.uint64) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.int16  , v2.int8  ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.int16  , v2.int16 ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.int16  , v2.int32 ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.int16  , v2.int64 ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.int16  , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.int16  , v2.uint16) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.int16  , v2.uint32) );    \
        else if ( (v1.ty == type_E::int16 ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.int16  , v2.uint64) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.int32  , v2.int8  ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.int32  , v2.int16 ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.int32  , v2.int32 ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.int32  , v2.int64 ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.int32  , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.int32  , v2.uint16) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.int32  , v2.uint32) );    \
        else if ( (v1.ty == type_E::int32 ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.int32  , v2.uint64) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.int64  , v2.int8  ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.int64  , v2.int16 ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.int64  , v2.int32 ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.int64  , v2.int64 ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.int64  , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.int64  , v2.uint16) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.int64  , v2.uint32) );    \
        else if ( (v1.ty == type_E::int64 ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.int64  , v2.uint64) );    \
}                            


#define M_verb_binary_fval_unsigned_integer(vr, f, v1, v2)                                                               \
{                                                                                                                        \
             if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.uint8  , v2.int8  ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.uint8  , v2.int16 ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.uint8  , v2.int32 ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.uint8  , v2.int64 ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.uint8  , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.uint8  , v2.uint16) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.uint8  , v2.uint32) );    \
        else if ( (v1.ty == type_E::uint8 ) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.uint8  , v2.uint64) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.uint16 , v2.int8  ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.uint16 , v2.int16 ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.uint16 , v2.int32 ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.uint16 , v2.int64 ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.uint16 , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.uint16 , v2.uint16) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.uint16 , v2.uint32) );    \
        else if ( (v1.ty == type_E::uint16) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.uint16 , v2.uint64) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.uint32 , v2.int8  ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.uint32 , v2.int16 ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.uint32 , v2.int32 ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.uint32 , v2.int64 ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.uint32 , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.uint32 , v2.uint16) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.uint32 , v2.uint32) );    \
        else if ( (v1.ty == type_E::uint32) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.uint32 , v2.uint64) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::int8  ) ) vr = type_val( f(v1.uint64 , v2.int8  ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::int16 ) ) vr = type_val( f(v1.uint64 , v2.int16 ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::int32 ) ) vr = type_val( f(v1.uint64 , v2.int32 ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::int64 ) ) vr = type_val( f(v1.uint64 , v2.int64 ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::uint8 ) ) vr = type_val( f(v1.uint64 , v2.uint8 ) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::uint16) ) vr = type_val( f(v1.uint64 , v2.uint16) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::uint32) ) vr = type_val( f(v1.uint64 , v2.uint32) );    \
        else if ( (v1.ty == type_E::uint64) && (v2.ty == type_E::uint64) ) vr = type_val( f(v1.uint64 , v2.uint64) );    \
}                               

#define M_verb_binary_fval_float_float(vr, f, v1, v2)                                                                        \
{                                                                                                                            \
             if ( (v1.ty == type_E::float32) && (v2.ty == type_E::float32) ) vr = type_val( f(v1.float32 , v2.float32) );    \
        else if ( (v1.ty == type_E::float32) && (v2.ty == type_E::float64) ) vr = type_val( f(v1.float32 , v2.float64) );    \
        else if ( (v1.ty == type_E::float64) && (v2.ty == type_E::float32) ) vr = type_val( f(v1.float64 , v2.float32) );    \
        else if ( (v1.ty == type_E::float64) && (v2.ty == type_E::float64) ) vr = type_val( f(v1.float64 , v2.float64) );    \
}




//       Type-oriented MACROs for binary functions -- value input / value output  (both input types are same)
//       ---------------------------------------------------------------------------------------------------- 

#define M_verb_binary_fval_integer(vr, f, v1, v2)                                          \
{                                                                                          \
        if      (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8    , v2.int8   ) );    \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16   , v2.int16  ) );    \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32   , v2.int32  ) );    \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64   , v2.int64  ) );    \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8   , v2.uint8  ) );    \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16  , v2.uint16 ) );    \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32  , v2.uint32 ) );    \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64  , v2.uint64 ) );    \
}


#define M_verb_binary_fval_signed(vr, f, v1, v2)                                           \
{                                                                                          \
        if      (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8    , v2.int8   ) );    \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16   , v2.int16  ) );    \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32   , v2.int32  ) );    \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64   , v2.int64  ) );    \
}


#define M_verb_binary_fval_unsigned(vr, f, v1, v2)                                         \
{                                                                                          \
        if      (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8   , v2.uint8  ) );    \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16  , v2.uint16 ) );    \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32  , v2.uint32 ) );    \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64  , v2.uint64 ) );    \
}


#define M_verb_binary_fval_float(vr, f, v1, v2)                                            \
{                                                                                          \
        if      (v1.ty == type_E::float32) vr = type_val( f(v1.float32 , v2.float32) );    \
        else if (v1.ty == type_E::float64) vr = type_val( f(v1.float64 , v2.float64) );    \
}


#define M_verb_binary_fval_arith(vr, f, v1, v2)                                            \
{                                                                                          \
        if      (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8    , v2.int8   ) );    \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16   , v2.int16  ) );    \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32   , v2.int32  ) );    \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64   , v2.int64  ) );    \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8   , v2.uint8  ) );    \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16  , v2.uint16 ) );    \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32  , v2.uint32 ) );    \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64  , v2.uint64 ) );    \
        else if (v1.ty == type_E::float32) vr = type_val( f(v1.float32 , v2.float32) );    \
        else if (v1.ty == type_E::float64) vr = type_val( f(v1.float64 , v2.float64) );    \
        else M_throw("M_verb_binary_fval_arith() -- unknown value type passed in");        \
}


#define M_verb_binary_fval_compare(vr, f, v1, v2)                                          \
{                                                                                          \
        if      (v1.ty == type_E::int8   ) vr = type_val( f(v1.int8    , v2.int8   ) );    \
        else if (v1.ty == type_E::int16  ) vr = type_val( f(v1.int16   , v2.int16  ) );    \
        else if (v1.ty == type_E::int32  ) vr = type_val( f(v1.int32   , v2.int32  ) );    \
        else if (v1.ty == type_E::int64  ) vr = type_val( f(v1.int64   , v2.int64  ) );    \
        else if (v1.ty == type_E::uint8  ) vr = type_val( f(v1.uint8   , v2.uint8  ) );    \
        else if (v1.ty == type_E::uint16 ) vr = type_val( f(v1.uint16  , v2.uint16 ) );    \
        else if (v1.ty == type_E::uint32 ) vr = type_val( f(v1.uint32  , v2.uint32 ) );    \
        else if (v1.ty == type_E::uint64 ) vr = type_val( f(v1.uint64  , v2.uint64 ) );    \
        else if (v1.ty == type_E::float32) vr = type_val( f(v1.float32 , v2.float32) );    \
        else if (v1.ty == type_E::float64) vr = type_val( f(v1.float64 , v2.float64) );    \
        else if (v1.ty == type_E::string ) vr = type_val( f(v1.string  , v2.string ) );    \
        else if (v1.ty == type_E::boolean) vr = type_val( f(v1.boolean , v2.string ) );    \
        else if (v1.ty == type_E::unit   ) vr = type_val( f(v1.int8    , v2.int8   ) );    \
        else M_throw("M_verb_binary_fval_compare() -- unknown value type passed in");      \
}

 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// ==================================================
// types needed for non-ex_xxxx.cpp pre-defined verbs
// ==================================================


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    -------------------
//    typdef_S  structure
//    -------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// value types -- typdef_S::kind field -and- value_S::ty field

enum class type_E { none          // not valid for values -- default initialization value 
                  , special       // special results -- used in results_S only -- not valid in regular values 
                  , error         // appears in value_S::ty field when some function reports an error
                  , no_value      // indicates that this keyword or results_S does not have an associated value

                  , unit          // no length associated with this type -- can appear in value_S::ty field, cannot appear in aggregate map types
                  , boolean       // no length associated with this type -- can appear in value_S::ty field, cannot appear in aggregate map types

                  // fixed-length (atomic) types  -- can appear in value_S::ty field and also as types of elements or fields in an aggregate type

                  , int8          //     
                  , int16         // 
                  , int32         // 
                  , int64         // 
                  , uint8         // 
                  , uint16        // 
                  , uint32        // 
                  , uint64        // 
                  , float32       // 
                  , float64       //  

                  // non-fixed-length types -- can appear as a value_S::ty field, but not in an aggregate type definition  

                  , string        // UTF-16 string -- general
                  , identifier    // UTF-16 string -- name of identifier
                  , verbname      // UTF-16 string -- name of verb  (can have operator characters, like + -, etc.)         
                  , keyname       // UTF-16 string -- name of keyword (same characters as identifier) 
                  , vlist         // 
                  , expression    // 
                  , block         // 
                  , verbset       // 
                  , typdef        // 
                  , ref           // reference to another value_S or data in buffer from another value_S

                  // aggregate types for value_S with buffer_sp->buffer_C  
                  //
                  //   - value_S::ty contains type_E::array or type_E::structure
                  //   - value_S::buffer_sp points to data buffer for these types
                  //   - value_S::typdef_sp points to associated mapping type, with array/structure details

                  , array         // aggregate type for mapping -- uses acount and atype_sp fields below  
                  , structure     // aggregate type for mapping -- uses fnames and fdefs below 
                  };                    


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct typdef_S
{
    type_E                                                  kind              {    };           // primary value type
    size_t                                                  tsize             { 0  };           // will be 0 (not valid) for non-atomic types like none, special, error, unit, string, identifier, verbname, keyname, type, vlist. expression, block, verbdef, etc.
                                                                                                // non-0     (valid    ) only for atomic and aggregate types  
    uint64_t                                                acount            { 0  };           // if .kind = type_E::array, this is the number of elements in the array (1-N) -- fixed number
    std::shared_ptr<typdef_S>                               atype_sp          {    };           // if .kind = type_E::array, this is a pointer to the type of each element in the array
    std::map<std::wstring, uint64_t>                        fnames            {    };           // if .kind = type_E::structure, these are the names of each field, with corresponding index into fdefs
    std::vector<fieldef_S>                                  fdefs             {    };           // if .kind = type_E::structure , this is a vector with fieldefs -- indexed by fnames: elem.second  
};


struct fieldef_S 
{
    uint64_t                                                offset            { 0  };           // offset from start of structure to start of this field
    std::shared_ptr<typdef_S>                               ftype_sp          {    };           // typdef for this field in structure
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------
//    ref_S  structure                                            note: items pointed to by the shared pointers here never get unshared
//    ------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ref_S
{   

    bool                                                    in_buffer         {false};           // true, if this refers to data in *(refval_sp->buffer_sp) -- i.e., not a full value_S reference 
    bool                                                    is_lvalue         {false};           // true, if this is an lvalue reference 
    bool                                                    is_rvalue         {false};           // true, if this is an rvalue reference 
    bool                                                    auto_deref        {false};           // true, if this is should be auto dereferenced (reference not explicitly created by user) 
    std::shared_ptr<value_S>                                refval_sp         {     };           // pointer to value_S being referenced -- never nullptr  
    uint64_t                                                offset            { 0   };           // offset into *(refval_sp->buffer_sp), if required -- 0 if full *refval_sp reference 
    std::shared_ptr<typdef_S>                               typdef_sp         {     };           // typdef_S for expression/field referenced -- nullptr,  if full *refval_sp reference    
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------
//    value_S  structure
//    ------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct value_S
{
    type_E                        ty                            { type_E::none  };      // default initialize to value type = none (needs to be set)   
     
    union                                                                               // value -- only if numeric type
    {  
        unit_T                          unit;                                          // no data here
        bool                            boolean;                                       // valid only if type is type_E::boolean  
        int8_t                          int8;                                          // valid only if type is type_E::int8  
        int16_t                         int16;                                         // valid only if type is type_E::int16 
        int32_t                         int32;                                         // valid only if type is type_E::int32 
        int64_t                         int64;                                         // valid only if type is type_E::int64 
        uint8_t                         uint8;                                         // valid only if type is type_E::uint8 
        uint16_t                        uint16;                                        // valid only if type is type_E::uint16 
        uint32_t                        uint32;                                        // valid only if type is type_E::uint32 
        uint64_t                        uint64                  { 0             };     // valid only if type is type_E::uint64       -- default initialize all 8 bytes to 0 
        float32_T                       float32;                                       // valid only if type is type_E::float32  
        float64_T                       float64;                                       // valid only if type is type_E::float64  
    };                                                                                                
                                                                            
    std::wstring                        string                                   ;      // valid when .ty is identifier, verbname,  keyname, or string -- used for string literal -or- identifier/verb name/keyword name
                                                                                     
    int64_t                             kw_token_ix             { -1            };      // token index (for error messages) for keyword owning this value -- not used if this value does not belong to a keyword
    int64_t                             token_ix1               { -1            };      // starting token index (for error messages) for this value, if known  
    int64_t                             token_ix2               { -1            };      // ending   token index (for error messages) for this value, if known 
                                                                       

    //  shared pointers to non-local data -- will be set only when corresponding type is saved in this value_S, as indicated by value_S::ty

    std::shared_ptr<    vlist_S     >       vlist_sp            {               };      // pointer to vlist_S         (if any)       -- if .ty = type_E::vlist
    std::shared_ptr< a_expression_S >  expression_sp            {               };      // pointer to a_expression_S  (if any)       -- if .ty = type_E::expression
    std::shared_ptr<    block_S     >       block_sp            {               };      // pointer to block_S         (if any)       -- if .ty = type_E::block
    std::shared_ptr<  verbset_S     >     verbset_sp            {               };      // pointer to verbset_S       (if any)       -- if .ty = type_E::verbset
    std::shared_ptr<   typdef_S     >      typdef_sp            {               };      // pointer to typdef_S        (if any)       -- if .ty = type_E::typdef   -or-   type_E::structure    -or-   type_E::array
    std::shared_ptr<     buf8_T     >      buffer_sp            {               };      // pointer to buf8_T          (if any)       -- if .ty = type_E::array    -or-   type_E::structure
    std::shared_ptr<      ref_S     >         ref_sp            {               };      // pointer to ref_S           (if any)       -- if .ty = type_E::ref    

    // flags

    bool                                suppress_eval_once      { false         };      // suppress evaluation once
    bool                                suppress_eval           { false         };      // suppress evaluation until this flag is turned off
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------
//    vlist_S  structure
//    ------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct keyword_S
{
    value_S   name     ;                // name of keyword (string) or expression yielding a string
    value_S   value    ;                // value of keyword (or expression, etc)
};




struct vlist_S
{
    uint64_t                                  value_ct         { 0     };      // number of positional values    
    uint64_t                                  kw_ct            { 0     };      // number of keywords present -- if kw_eval_done, this is number of evaluated keywords in the multimap, if not, it's the number of keywords in the vector  
                                           
    bool                                      val_unit         { false };      // true = one or more positional values are UNIT_T
    bool                                      val_boolean      { false };      // true = one or more positional values are BOOL_T
    bool                                      val_int8         { false };      // true = one or more positional values are INT8_T
    bool                                      val_int16        { false };      // true = one or more positional values are INT16_T
    bool                                      val_int32        { false };      // true = one or more positional values are INT32_T
    bool                                      val_int64        { false };      // true = one or more positional values are INT64_T
    bool                                      val_uint8        { false };      // true = one or more positional values are UINT8_T
    bool                                      val_uint16       { false };      // true = one or more positional values are UINT16_T
    bool                                      val_uint32       { false };      // true = one or more positional values are UINT32_T
    bool                                      val_uint64       { false };      // true = one or more positional values are UINT64_T
    bool                                      val_float32      { false };      // true = one or more positional values are FLOAT32_T
    bool                                      val_float64      { false };      // true = one or more positional values are FLOAT64_T
    bool                                      val_string       { false };      // true = one or more positional values are std::wstring
    bool                                      val_identifier   { false };      // true = one or more positional values are identifier
    bool                                      val_vlist        { false };      // true = one or more positional values are vlist_S
    bool                                      val_expression   { false };      // true = one or more positional values are expression_S
    bool                                      val_block        { false };      // true = one or more positional values are block_S
    bool                                      val_verbset      { false };      // true = one or more positional values are verbset_S
    bool                                      val_typdef       { false };      // true = one or more positional values are typdef_S
    bool                                      val_array        { false };      // true = one or more positional values are array
    bool                                      val_structure    { false };      // true = one or more positional values are structure
    bool                                      val_ref          { false };      // true = one or more positional values are ref_S
                                         
    bool                                      val_mixed        { false };      // true = more than one type of positional value seen (excluding identifiers)
                                         
    bool                                      kw_expression    { false };      // true = one or more keyword values is an expression 
    bool                                      kw_vlist         { false };      // true = one or more keyword values is an vlist 
    bool                                      kw_identifier    { false };      // true = one or more keyword values is an identifier 
    bool                                      kw_eval_done     { false };      // true = eval_kws has been filled in 
                                         
    int64_t                                   token_ix1        { -1    };      // starting token index for this vlist, if known  
    int64_t                                   token_ix2        { -1    };      // ending   token index for this vlist, if known 

    std::vector<value_S>                      values           {       };      // vector of positional values (can be empty)
    std::vector<keyword_S>                    keywords         {       };      // list of unevaluated keywords found during parsing (can be empty)
    std::multimap<std::wstring, value_S>      eval_kws         {       };      // "list" of evaluated keywords and associated values present (multiples allowed, but may not be supported by the verb being called)
 };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------------
//    e_expression_S structure -- copied-over a_expression_S for use during verb execution (resolved verb name and keyword names, with evaluated values, etc.)
//    ------------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct e_expression_S 
{
    bool                                       has_verb         { false };      // true -- verb present,   false -- no verb found for this expression
    bool                                       has_sigil        { false };      // true -- original verb had leading @ sigil
    char32_t                                   sigil            {       };      // if has_sigil is true, sigil is saved here 
                                              
    int                                        priority         { 0     };      // verb priority 
    bool                                       right_associate  { false };      // true = always right-to-left associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix 
    bool                                       left_associate   { false };      // true = always left-to-right associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix
                                              
    int64_t                                    token_ix1        { -1    };      // starting token index for this expression, if known  
    int64_t                                    token_ix2        { -1    };      // ending   token index for this expression, if known 
                                                                        
    std::wstring                               verb_name        {       };      // string with evaluated verb name 
    int64_t                                    verb_token_ix1   { -1    };      // original starting token index for evaluated verb name expression, if known 
    int64_t                                    verb_token_ix2   { -1    };      // original ending   token index for evaluated verb name expression, if known
                                                                         
    vlist_S                                    lparms           {       };      // left-side  parameters for verb or option  
    vlist_S                                    rparms           {       };      // right-side parameters for verb or option 
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ---------------------
//    arg_eval_S  structure -- no-eval flags for left-side or right-side positional parms
//    ---------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// overload set definition
// -----------------------

struct arg_eval_S                       // argument evaluation flags
{
    bool                                 no_eval_ident                  { false };      // true -- don't evaluate identifier positional agruments before invoking the verb
    bool                                 no_eval_expression             { false };      // true -- don't evaluate expression positional agruments before invoking the verb
    bool                                 no_eval_vlist                  { false };      // true -- don't evaluate vlist      positional agruments before invoking the verb
    bool                                 no_eval_ref                    { false };      // true -- don't evaluate ref        positional agruments before invoking the verb
    bool                                 verbless                       { false };      // true -- temp flag in dummy plist/verbdef -- this evaluation is being done for a verbless expresion   
  //bool                                 no_eval_kw_ident               { false };      // true -- don't evaluate identifier keyword    agruments before invoking the verb
  //bool                                 no_eval_kw_expression          { false };      // true -- don't evaluate expression keyword    agruments before invoking the verb
  //bool                                 no_eval_kw_vlist               { false };      // true -- don't evaluate vlist      keyword    agruments before invoking the verb
  //bool                                 no_eval_kw_ref                 { false };      // true -- don't evaluate ref        keyword    agruments before invoking the verb
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ---------------------
//    parmtype_S  structure
//    ---------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct parmtype_S
{
    int64_t                        kw_min_ct                    { 0     };      // min number of times keyword can appear -- not used when checking positional parms 
    int64_t                        kw_max_ct                    { 0     };      // max number of times keyword can appear -- not used when checking positional parms  

    arg_eval_S                     eval                         {       };      // evaluation control flags
    bool                           anything_ok                  { false };      // all parm types OK -- no checking needed
    bool                           check_local_env_only         { false };      // check only local  stack frame when doing identifier checks 
    bool                           check_global_env_only        { false };      // check only global stack frame when doing identifier checks 
    
    bool                           nval_ok                      { false };      // ok for keyword to have no following value
    bool                           boolean_ok                   { false };      // ok for value to be BOOL_T
    bool                           unit_ok                      { false };      // ok for value to be UNIT_T
    bool                           int8_ok                      { false };      // ok for value to be INT8_T
    bool                           int16_ok                     { false };      // ok for value to be INT16_T
    bool                           int32_ok                     { false };      // ok for value to be INT32_T
    bool                           int64_ok                     { false };      // ok for value to be INT64_T
    bool                           uint8_ok                     { false };      // ok for value to be UINT8_T
    bool                           uint16_ok                    { false };      // ok for value to be UINT16_T
    bool                           uint32_ok                    { false };      // ok for value to be UINT32_T
    bool                           uint64_ok                    { false };      // ok for value to be UINT64_T
    bool                           float32_ok                   { false };      // ok for value to be FLOAT32_T
    bool                           float64_ok                   { false };      // ok for value to be FLOAT64_T
    bool                           string_ok                    { false };      // ok for value to be STRING_T
    bool                           verbname_ok                  { false };      // ok for value to be verbname  
    bool                           raw_ident_ok                 { false };      // ok for vlue to be  any (raw/unevaluated) identifier (defined, or undefined) -- including typdef or verbdef??
    bool                           var_ident_ok                 { false };      // ok for value to be defined variable identifier  (not a constant)
    bool                           const_ident_ok               { false };      // ok for value to be defined constant identifier (not a variable) 
    bool                           typdef_ident_ok              { false };      // ok for value to be defined typdef identifier 
    bool                           verbset_ident_ok             { false };      // ok for value to be defined vebset identifier 
    bool                           undef_ident_ok               { false };      // ok for value to be undefined identifier (and not defined)
    bool                           vlist_ok                     { false };      // ok for value to be vlist -- optional pointer is set, if verb cares about value types in vlist
    bool                           expression_ok                { false };      // ok for value to be expression  
    bool                           block_ok                     { false };      // ok for value to be block 
    bool                           verbset_ok                   { false };      // ok for value to be verbset
    bool                           typdef_ok                    { false };      // ok for value to be typdef
    bool                           array_ok                     { false };      // ok for value to be array
    bool                           structure_ok                 { false };      // ok for value to be structure
    bool                           lvalue_ref_ok                { false };      // ok for value to be lvalue-type ref 
    bool                           rvalue_ref_ok                { false };      // ok for value to be rvalue-type ref 
    

    //  min/max values for range checking -- only supported for int64 and float 64 parms

    bool                           int64_range                  { false };      // true -- do int64   range check
    bool                           float64_range                { false };      // true -- do float64 range check

    int64_t                        int64_min                    { 0     };      // min int64   value allowed
    int64_t                        int64_max                    { 0     };      // max int64   value allowed
    float64_T                      float64_min                  { 0.0   };      // min float64 value allowed
    float64_T                      float64_max                  { 0.0   };      // max float64 value allowed


    //  pointer to nested argument list -- valid only when vlist_ok flag is set

    std::shared_ptr<plist_S>       plist_sp                              ;     // pointer to nested plist -- can be set if vlist is allowed as a parm value  
                                                                               // note: uses stub definition for plist_S -- real definition of plist_S is below
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------
//    plist_S  structure -- left-side or right-side parm types for verbdef_S
//    ------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct plist_S
{
    bool                                   no_check_positional          { false };      // don't check positional parms -- anything is   OK (both number and type)
    bool                                   no_check_keywords            { false };      // don't check keyword parms    -- anything is   OK (both number,name, and type)
    bool                                   no_check_keyword_names       { false };      // don't check keyword names    -- any names are OK (number and types must match 1st/only kw in multimap, with name = "*")

    arg_eval_S                             eval                         {       };      // global evaluation flags for positional parms -- used only when expression has no verb, or when verb is part of an overload set (before proper verbdef is chosen) 
                                                                                    
    int64_t                                min_ct                       { 0     };      // minimum number of positional parms allowed 
    int64_t                                max_ct                       { -1    };      // maximum number of positional parms allowed (-1 = no limit)

    std::vector<parmtype_S>                values                       {       };      // vector of positional args (can be empty, if global arg checking used)
    std::map<std::wstring, parmtype_S>     keywords                     {       };      // "list" of keyword args allowed (some may be required) 
                                                                        
    std::vector<std::set<std::wstring>>    choices                      {       };      // list of one/multiple choice sets of keywords (1 or more must be present) 
    std::vector<std::set<std::wstring>>    conflicts                    {       };      // list of conflicting sets of keywords (0 or 1 must be present)
    std::vector<std::set<std::wstring>>    matches                      {       };      // list of sets of keywords which must be present together -- same number of occurrences in vlist (can be 0, 1, 2, but all must occur same number of times)
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    -------------------
//    argvar_S  structure -- lists of variables to be set to left-side and right_side invocation parms -- for verbdef_S 
//    -------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct argvar_S
{
    std::vector<std::wstring>                     pos        {         };    // list of variables          to receive positional parm values
    std::multimap<std::wstring, std::wstring>     key        {         };    // list of keywords/variables to receive keyword    parm values
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    -------------------------------
//    verbdef_S/verbset_S  structures -- verb definition info
//    -------------------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// verb priorities 
/////////////////////////////////////////////////////////////////////////////

namespace verb_pri_N
{
constexpr int64_t _not_specified     {  M_int64_min };    // use same priority as pre-existing verb in verbset (default if first verb in verbset) -- note: all verbs in an overload set must have the same priority (and associativity) 
constexpr int64_t _attached_paren    {    1000'000L };                                                       
constexpr int64_t _select            {     120'000L };  
constexpr int64_t _subscript         {     120'000L };                                                      
constexpr int64_t _increment         {     100'000L };
constexpr int64_t _not               {     100'000L };
constexpr int64_t _bitnot            {     100'000L };                                                     
constexpr int64_t _at                {      80'000L };  
constexpr int64_t _power             {      70'000L };
constexpr int64_t _multiply          {      60'000L }; 
constexpr int64_t _add               {      50'000L }; 
constexpr int64_t _shift             {      40'000L }; 
constexpr int64_t _compare           {      30'000L }; 
constexpr int64_t _bitwise           {      20'000L }; 
constexpr int64_t _boolean           {      10'000L };
constexpr int64_t _default           {           0L };
constexpr int64_t _assign            {     -10'000L };                                                     
constexpr int64_t _separate          {    -990'000L };
constexpr int64_t _sequence          {   -1000'000L };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

struct verbdef_S
{
    bool                                  is_builtin                                              {         };    // true -- when verb is built-in (not user defined)
    bool                                  verbless                                                { false   };    // true -- this is a temporary dummy verbdef_s used during verbless expression evaluation 
    bool                                  simplified_call                                         { false   };    // true -- call verb without frame_S, verbdef_S, and with value_S rather than results_S 
    bool                                  by_alias_ok                                             { false   };    // true -- arg variable names starting with "_" (example: "_arg1") are assumed to be passed by alias 
    bool                                  parms_same_type                                         { false   };    // true -- all positional parms (both sides) must be same type
    bool                                  parms_same_number                                       { false   };    // true -- must be same number of positional parms on left and right sides (can be 0-N)  
    bool                                  parms_some_required                                     { false   };    // true -- at least one positional parm is required, either on left-side or right-side (used when both sides support 0-N positional parms)  
    bool                                  parms_left_xor_right                                    { false   };    // true -- positional parms required either on left or on right (not both sides, or neither side)
    bool                                  parms_not_both_sides                                    { false   };    // true -- positional and keyword parms cannot appear on both sides (parms on either side, or neither side is OK)

    bool                                  percolate_all                                           { false   };    // true -- percolate all special results to calling verb (used with same_scope: verbs)  

    bool                                  lexical_scope                                           { false   };    // true -- verb has lexical upward scope (causes closure with upward scopes at definition time to be captured)
    bool                                  dynamic_scope                                           { false   };    // true -- verb has dynamic upward scope -- only exposed upward-scoped symbols (on call chain) are visible 
#ifdef M_EXPOSE_SUPPORT
    bool                                  dynall_scope                                            { false   };    // true -- verb has dynall  upward scope = dynamic + all upward-scoped symbols (on call chain) are visible, not just exposed ones 
#endif
    bool                                  same_scope                                              { false   };    // true -- verb is does not have its oen static/dynamic environments -- uses caller's stack frame(s) 
    bool                                  no_scope                                                { false   };    // true -- verb is self-contained (with input parms) -- no upward scope is established (although global variables/verbs/etc. are available) 
    bool                                  scope_defaulted                                         { false   };    // true -- lexical scope was defaulted, not explicitly requested

    std::weak_ptr<frame_S>                defining_scope_wp                                       {         };    // if lexical scope, this points to scope where verb was defined -- alway set for any lexically-scoped user-defined verb
    std::shared_ptr<frame_S>              defining_scope_sp   /** SP **/                          {         };    // if closure, this points to scope where verb was defined -- shared owning pointer -- used only to keep enclosing scopes from going away                                     
    std::shared_ptr<environ_S>            persist_env_sp      /** SP **/                          {         };    // persistent (static/closure) environment pointer -- exists for lifetime of function definition (non-builtin functions only)

    void                                 *fcn_p                                                   { nullptr };    // pointer to function (in C++ parser code) to process this verb (non-owning pointer)
    std::shared_ptr<block_S>              verb_block_sp       /** SP **/                          { nullptr };    // pointer to verb-block for this verb (if any -- only for user-defined, non built-in verbs) -- ***owning pointer***  
    std::shared_ptr<block_S>              init_block_sp       /** SP **/                          { nullptr };    // pointer to init-block for this verb (if any -- only if init: option specified for user-defined, non built-in verbs) -- ***owning pointer***  

    plist_S                               lparms                                                  {         };    // left-side positional and keyword descriptions
    plist_S                               rparms                                                  {         };    // right-side positional and keyword descriptions
    argvar_S                              lvars                                                   {         };    // variable names for left  side parms
    argvar_S                              rvars                                                   {         };    // variable names for right side parms
    std::wstring                          info                                          { L"no description" };    // descriptive message for debugging and overload match failure messages              
};




struct verbset_S
{
    std::vector<verbdef_S>                verbs                                                   {         };    // verbdefs in the overload set
    int64_t                               priority                             { verb_pri_N::_not_specified };    // evaluation priority in expressions -- all verbs in overload set must have same priority and associativity
    bool                                  right_associate                                         { false   };    // true = always right-to-left associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix 
    bool                                  left_associate                                          { false   };    // true = always left-to-right associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix
    arg_eval_S                            left_eval                                               {         };    // left-side  argument evaluation flags  
    arg_eval_S                            right_eval                                              {         };    // right-side argument evaluation flags 
    bool                                  custom_eval                                             { false   };    // custom eval flag settings on a per-parm basis (especially keywords) -- can't be overloaded   
    bool                                  has_builtin                                             { false   };    // at least one if the verbdef_S's is built-in 
    bool                                  verbless                                                { false   };    // true -- this is a temporary dummy verbset_s used during verbless expression evaluation 
};

 
#if 0 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ---------------------
//    parmtype_S  structure
//    ---------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct parmtype_S
{
    int64_t                        kw_min_ct                    { 0     };      // min number of times keyword can appear -- not used when checking positional parms 
    int64_t                        kw_max_ct                    { 0     };      // max number of times keyword can appear -- not used when checking positional parms  

    arg_eval_S                     eval                         {       };      // evaluation control flags
    bool                           anything_ok                  { false };      // all parm types OK -- no checking needed
    bool                           check_local_env_only         { false };      // check only local  stack frame when doing identifier checks 
    bool                           check_global_env_only        { false };      // check only global stack frame when doing identifier checks 
    
    bool                           nval_ok                      { false };      // ok for keyword to have no following value
    bool                           boolean_ok                   { false };      // ok for value to be BOOL_T
    bool                           unit_ok                      { false };      // ok for value to be UNIT_T
    bool                           int8_ok                      { false };      // ok for value to be INT8_T
    bool                           int16_ok                     { false };      // ok for value to be INT16_T
    bool                           int32_ok                     { false };      // ok for value to be INT32_T
    bool                           int64_ok                     { false };      // ok for value to be INT64_T
    bool                           uint8_ok                     { false };      // ok for value to be UINT8_T
    bool                           uint16_ok                    { false };      // ok for value to be UINT16_T
    bool                           uint32_ok                    { false };      // ok for value to be UINT32_T
    bool                           uint64_ok                    { false };      // ok for value to be UINT64_T
    bool                           float32_ok                   { false };      // ok for value to be FLOAT32_T
    bool                           float64_ok                   { false };      // ok for value to be FLOAT64_T
    bool                           string_ok                    { false };      // ok for value to be STRING_T
    bool                           verbname_ok                  { false };      // ok for value to be verbname  
    bool                           raw_ident_ok                 { false };      // ok for vlue to be  any (raw/unevaluated) identifier (defined, or undefined) -- including typdef or verbdef??
    bool                           var_ident_ok                 { false };      // ok for value to be defined variable identifier  (not a constant)
    bool                           const_ident_ok               { false };      // ok for value to be defined constant identifier (not a variable) 
    bool                           typdef_ident_ok              { false };      // ok for value to be defined typdef identifier 
    bool                           verbset_ident_ok             { false };      // ok for value to be defined vebset identifier 
    bool                           undef_ident_ok               { false };      // ok for value to be undefined identifier (and not defined)
    bool                           vlist_ok                     { false };      // ok for value to be vlist -- optional pointer is set, if verb cares about value types in vlist
    bool                           expression_ok                { false };      // ok for value to be expression  
    bool                           block_ok                     { false };      // ok for value to be block 
    bool                           verbset_ok                   { false };      // ok for value to be verbset
    bool                           typdef_ok                    { false };      // ok for value to be typdef
    bool                           array_ok                     { false };      // ok for value to be array
    bool                           structure_ok                 { false };      // ok for value to be structure
    bool                           lvalue_ref_ok                { false };      // ok for value to be lvalue-type ref 
    bool                           rvalue_ref_ok                { false };      // ok for value to be rvalue-type ref 
    

    //  min/max values for range checking -- only supported for int64 and float 64 parms

    bool                           int64_range                  { false };      // true -- do int64   range check
    bool                           float64_range                { false };      // true -- do float64 range check

    int64_t                        int64_min                    { 0     };      // min int64   value allowed
    int64_t                        int64_max                    { 0     };      // max int64   value allowed
    float64_T                      float64_min                  { 0.0   };      // min float64 value allowed
    float64_T                      float64_max                  { 0.0   };      // max float64 value allowed


    //  pointer to nested argument list -- valid only when vlist_ok flag is set

    std::shared_ptr<plist_S>       plist_sp                              ;     // pointer to nested plist -- can be set if vlist is allowed as a parm value  

};
#endif

                                                                                                                          
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////// public ex main() function -- in ex_main.cpp

M_EX_IMPEXP int          ex_main(int, wchar_t *[]);


/////////////////// public verb-oriented functions -- in ex_interface.cpp

M_EX_IMPEXP int          add_predefined_verb(        const std::wstring&, const verbset_S&);
M_EX_IMPEXP int          attach_plist(               parmtype_S&, const plist_S&);

M_EX_IMPEXP value_S      set_type_value( int64_t             );  
M_EX_IMPEXP value_S      set_type_value( float64_T           );  
M_EX_IMPEXP value_S      set_type_value( const std::wstring& );  


/////////////////// public character and token-oriented functions  -- in ex_interface.cpp 

M_EX_IMPEXP bool is_valid_identifier(const std::wstring&); 
M_EX_IMPEXP bool is_valid_operator(  const std::wstring&); 
M_EX_IMPEXP bool is_valid_verbname(  const std::wstring&); 


/////////////////// public setup and processing functions  -- in ex_interface.cpp


M_EX_IMPEXP int                       process_main_ext(          const std::wstring&                      ); 
M_EX_IMPEXP std::wstring              process_cmdline_ext(       int, wchar_t *[]                         );
M_EX_IMPEXP int                       pending_attach_file_ext(   const std::wstring&                      );    // file version
M_EX_IMPEXP int                       pending_attach_string_ext( const std::wstring&, const std::wstring& );    // string version   
M_EX_IMPEXP int                       import_dll(                const std::wstring&, const std::wstring& ); 
M_EX_IMPEXP int                       set_skip_ext(              const std::wstring&                      ); 

M_EX_IMPEXP void                      count_error(uint64_t = 1ULL); 
M_EX_IMPEXP uint64_t                  error_count();
M_EX_IMPEXP std::shared_ptr<frame_S>  get_pp_frame();  
M_EX_IMPEXP frame_S                  *get_main_frame();  
M_EX_IMPEXP void                      setup_global_environ(); 
M_EX_IMPEXP environ_S                *get_global_environ();  



////////////////// public verb functions   -- in ex_verb.cpp 

void add_predefined_typdefs();   
void add_predefined_constants();      
void add_predefined_verbs();          
 


///////////////// location string and other debug-message-oriented routines  ///////////////////////////////////

//M_EX_IMPEXP std::wstring expression_loc_str( const a_expression_S& );                  // in ex_parse.h
//M_EX_IMPEXP std::wstring       verb_loc_str( const a_expression_S& );                  // in ex_parse.h
  M_EX_IMPEXP std::wstring expression_loc_str( const e_expression_S& );                    
  M_EX_IMPEXP std::wstring       verb_loc_str( const e_expression_S& );                    
//M_EX_IMPEXP std::wstring      vlist_loc_str( const vlist_S&);                          // in ex_parse.h
//M_EX_IMPEXP std::wstring      value_loc_str( const value_S&);                          // in ex_parse.h
//M_EX_IMPEXP std::wstring         kw_loc_str( const value_S&);                          // in ex_parse.h

//M_EX_IMPEXP void                msg_loc( const   value_S&                          );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   value_S&, const std::wstring&     );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   vlist_S&                          );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   vlist_S&, const std::wstring&     );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc(                   const a_expression_S&   );  // in ex_parse.h
  M_EX_IMPEXP void                msg_loc(                   const e_expression_S&   );  
//M_EX_IMPEXP void                msg_loc( const   value_S&, const a_expression_S&   );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   value_S&, const e_expression_S&   );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   vlist_S&, const a_expression_S&   );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   vlist_S&, const e_expression_S&   );  // in ex_parse.h

//M_EX_IMPEXP void             msg_kw_loc( const   value_S&                          );  // in ex_parse.h
                                                                                     
//M_EX_IMPEXP void             msgend_loc( const   value_S&                          );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   value_S&, const std::wstring&     );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   vlist_S&                          );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   vlist_S&, const std::wstring&     );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc(                   const a_expression_S&   );  // in ex_parse.h
  M_EX_IMPEXP void             msgend_loc(                   const e_expression_S&   );  
//M_EX_IMPEXP void             msgend_loc( const   value_S&, const a_expression_S&   );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   value_S&, const e_expression_S&   );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   vlist_S&, const a_expression_S&   );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   vlist_S&, const e_expression_S&   );  // in ex_parse.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////