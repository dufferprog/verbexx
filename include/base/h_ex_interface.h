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


/////////////// MACROs for displaying verb (debug)  ////////////////////////////////////////////////////////

#define M_dv_all(  x )  M_if(log_verbs() >= log_E::all , x)
#define M_dv_most( x )  M_if(log_verbs() >= log_E::most, x)
#define M_dv_some( x )  M_if(log_verbs() >= log_E::some, x)
#define M_dv_few(  x )  M_if(log_verbs() >= log_E::few , x) 




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////  Macros for verbs to use when extracting parms
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// i = vexpr  k = keyword name    f = field to set   v = value to set field to,  t = type to cast parm value to field value     
//
//         examples:      M_get_right_keyword_nval(  vexpr, L"display", xparm.display, true   )
//                        M_get_right_keyword_string(vexpr, L"name"   , xparm.name            )
//                        M_get_right_keyword_int64( vexpr, L"width"  , xparm.width  , int32_t)
//
//
//                        M_get_right_keyword_vlist(vexpr, L"filenames",  filenames_vl)

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
//    example -- xparm.length = (int2_t)M_get_right_pos_int64(vexpr, 3) -- gets 4th positional parm  
//
//    i = vexpr  ,  n = n-th positional parm, l = nested vlist
//


#define M_get_right_pos(                   i, n)             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n))                  : value_S       { } )
#define M_get_right_pos_int64(             i, n)             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).int64            : 0                 )
#define M_get_right_pos_float64(           i, n)             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).float64          : 0.0               )
#define M_get_right_pos_string(            i, n)             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).string           : std::wstring  { } )
#define M_get_right_pos_ident(             i, n)             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).string           : std::wstring  { } )
#define M_get_right_pos_typdef_p(          i, n)             ( i.rparms.values.size() > (n) ? i.rparms.values.at((n)).typdef_sp.get()  : nullptr           )
                                                                                                                                                       
#define M_get_left_pos(                    i, n)             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n))                  : value_S       { } )
#define M_get_left_pos_int64(              i, n)             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).int64            : 0                 )
#define M_get_left_pos_float64(            i, n)             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).float64          : 0.0               )
#define M_get_left_pos_string(             i, n)             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).string           : std::wstring  { } )
#define M_get_left_pos_ident(              i, n)             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).string           : std::wstring  { } )
#define M_get_left_pos_typdef_p(           i, n)             ( i.lparms.values.size() > (n) ? i.lparms.values.at((n)).typdef_sp.get()  : nullptr           )
                                                                                                                                                       
#define M_get_nest_pos(                    l, n)             ( (l)     .values.size() > (n) ? (l)     .values.at((n))                  : value_S       { } )
#define M_get_nest_pos_int64(              l, n)             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).int64            : 0                 )
#define M_get_nest_pos_float64(            l, n)             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).float64          : 0.0               )
#define M_get_nest_pos_string(             l, n)             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).string           : std::wstring  { } )
#define M_get_nest_pos_ident(              l, n)             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).string           : std::wstring  { } )
#define M_get_nest_pos_typdef_p(           l, n)             ( (l)     .values.size() > (n) ? (l)     .values.at((n)).typdef_sp.get()  : nullptr           )



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

 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// =========================================
// types needed for non-ex pre-defined verbs
// =========================================


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
                  , unit          // no length associated with this type -- can appear in value_S::ty field, cannot appear in aggregate map types
                  , boolean       // no length associated with this type -- can appear in value_S::ty field, cannot appear in aggregate map types
                  , no_value      // indicates that this keyword does not have an associated value

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
                  , vexpr         // 
                  , slist         // 
                  , verbdef       // 
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
    size_t                                                  tsize             { 0  };           // will be 0 (not valid) for non-atomic types like none, special, error, unit, string, identifier, verbname, keyname, type, vlist. vexpr, slist, verbdef, etc.
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
//    ref_S  structure                                            note: vexprs pointed to by the shared pointers here never get unshared
//    ------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ref_S
{   

    bool                                                    in_buffer         {false};           // true, if this refers to data in *(value_sp->buffer_sp) -- i.e., not a full value_S reference 
    bool                                                    is_lvalue         {false};           // true, if this is an lvalue reference 
    bool                                                    is_rvalue         {false};           // true, if this is an rvalue reference 
    bool                                                    auto_deref        {false};           // true, if this is should be auto dereferenced (reference not explicitly created by user) 
    std::shared_ptr<value_S>                                value_sp          {     };           // pointer to value_S being referenced -- never nullptr  
    uint64_t                                                offset            { 0   };           // offset into *(value_sp->buffer_sp), if required -- 0 if full *value_sp reference 
    std::shared_ptr<typdef_S>                               typdef_sp         {     };           // typdef_S for vexpr/field referenced -- nullptr, if full *value_sp reference    
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
        bool        boolean;                                                            // valid only if type is type_E::boolean  
        int8_t      int8;                                                               // valid only if type is type_E::int8  
        int16_t     int16;                                                              // valid only if type is type_E::int16 
        int32_t     int32;                                                              // valid only if type is type_E::int32 
        int64_t     int64;                                                              // valid only if type is type_E::int64 
        uint8_t     uint8;                                                              // valid only if type is type_E::uint8 
        uint16_t    uint16;                                                             // valid only if type is type_E::uint16 
        uint32_t    uint32;                                                             // valid only if type is type_E::uint32 
        uint64_t    uint64                                      { 0             };      // valid only if type is type_E::uint64       -- default initialize all 8 bytes to 0 
        float32_T   float32;                                                            // valid only if type is type_E::float32  
        float64_T   float64;                                                            // valid only if type is type_E::float64  
    };                                                                            
                                                                            
    std::wstring                   string                                        ;      // valid when .ty is identifier, verbname,  keyname, or string -- used for string literal -or- identifier/verb name/keyword name
                                                                                
    int64_t                        kw_token_ix                  { -1            };      // token index (for error messages) for keyword owning this value -- not used if this value does not belong to a keyword
    int64_t                        token_ix1                    { -1            };      // starting token index (for error messages) for this value, if known  
    int64_t                        token_ix2                    { -1            };      // ending   token index (for error messages) for this value, if known 
                                                                            

    //  shared pointers to non-local data -- will be set only when corresponding type is saved in this value_S, as indicated by value_S::ty

    std::shared_ptr<   vlist_S >   vlist_sp                     {               };      // pointer to vlist_S    (if any)    -- if .ty = type_E::vlist
    std::shared_ptr< a_vexpr_S >   vexpr_sp                     {               };      // pointer to a_vexpr_S  (if any)    -- if .ty = type_E::vexpr
    std::shared_ptr<   slist_S >   slist_sp                     {               };      // pointer to slist_S    (if any)    -- if .ty = type_E::slist
    std::shared_ptr< verbdef_S >   verbdef_sp                   {               };      // pointer to verbdef_S  (if any)    -- if .ty = type_E::verbdef
    std::shared_ptr<  typdef_S >   typdef_sp                    {               };      // pointer to typdef_S   (if any)    -- if .ty = type_E::typdef   -or-   type_E::structure    -or-   type_E::array
    std::shared_ptr<    buf8_T >   buffer_sp                    {               };      // pointer to buf8_T     (if any)    -- if .ty = type_E::array    -or-   type_E::structure
    std::shared_ptr<     ref_S >   ref_sp                       {               };      // pointer to ref_S      (if any)    -- if .ty = type_E::ref     
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
    uint64_t                                  kw_ct            { 0     };      // number of literal-keywords present
                                         
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
    bool                                      val_vexpr        { false };      // true = one or more positional values are vexpr_S
    bool                                      val_slist        { false };      // true = one or more positional values are slist_S
    bool                                      val_verbdef      { false };      // true = one or more positional values are verbdef_S
    bool                                      val_typdef       { false };      // true = one or more positional values are typdef_S
    bool                                      val_array        { false };      // true = one or more positional values are array
    bool                                      val_structure    { false };      // true = one or more positional values are structure
    bool                                      val_ref          { false };      // true = one or more positional values are ref_S
                                         
    bool                                      val_mixed        { false };      // true = more than one type of positional value seen (excluding identifiers)
                                         
    bool                                      kw_vexpr         { false };      // true = one or more keyword values are vexprs 
    bool                                      kw_vlist         { false };      // true = one or more keyword values are vlists 
    bool                                      kw_identifier    { false };      // true = one or more keyword values are identifiers 
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
//    -------------------
//    e_vexpr_S structure -- copied-over vexpr_S for use during verb execution (resolved verb name and keyword names, with evaluated values, etc.)
//    -------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct e_vexpr_S 
{
    bool                                       has_verb         { false };      // true -- verb present,   false -- no verb found for this vexpr
    bool                                       has_sigil        { false };      // true -- original verb had leading @ sigil
    char32_t                                   sigil            {       };      // if has_sigil is true, sigil is saved here 
                                              
    int                                        priority         { 0     };      // verb priority 
    bool                                       right_associate  { false };      // true = always right-to-left associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix 
    bool                                       left_associate   { false };      // true = always left-to-right associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix
                                              
    int64_t                                    token_ix1        { -1    };      // starting token index for this vexpr, if known  
    int64_t                                    token_ix2        { -1    };      // ending   token index for this vexpr, if known 
                                                                        
    std::wstring                               verb_name        {       };      // string with evaluated verb name 
    int64_t                                    verb_token_ix1   { -1    };      // original starting token index for evaluated verb name vexpr, if known 
    int64_t                                    verb_token_ix2   { -1    };      // original ending   token index for evaluated verb name vexpr, if known
                                                                         
    vlist_S                                    lparms           {       };      // left-side  parameters for verb or option  
    vlist_S                                    rparms           {       };      // right-side parameters for verb or option 
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


    // global checking flags for positional parms -- used only when vexpr has no verb, (meaning it has positional parmtypes in the values vector)

    bool                                   no_eval_ident                { false };      // true -- don't evaluate identifiers in positional parms  
    bool                                   no_eval_vexpr                { false };      // true -- don't evaluate vexprs in positional parms  
    bool                                   no_eval_vlist                { false };      // true -- don't evaluate (nested) vlists in positional parms 
                            
    int64_t                                min_ct                       { 0     };      // minimum number of positional parms allowed 
    int64_t                                max_ct                       { -1    };      // maximum number of positional parms allowed (-1 = no limit)

    std::vector<parmtype_S>                values                                ;      // vector of positional args (can be empty, if global arg checking used)
    std::map<std::wstring, parmtype_S>     keywords                              ;      // "list" of keyword args allowed (some may be required) 

    std::vector<std::set<std::wstring>>    choices                               ;      // list of one/multiple choice sets of keywords (1 or more must be present) 
    std::vector<std::set<std::wstring>>    conflicts                             ;      // list of conflicting sets of keywords (0 or 1 must be present)
    std::vector<std::set<std::wstring>>    matches                               ;      // list of sets of keywords which must be present together -- same number of occurrences in vlist (can be 0, 1, 2, but all must occur same number of times)
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
//    --------------------
//    verbdef_S  structure -- verb definition info
//    --------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct verbdef_S
{
    bool                                  is_builtin                                              {         };    // true -- when verb is built-in (not user defined)
    bool                                  simplified_call                                         { false   };    // true -- call verb without frame_S, verbdef_S, and with value_S rather than results_S 
    bool                                  parms_same_type                                         { false   };    // true -- all positional parms (both sides) must be same type
    bool                                  parms_some_required                                     { false   };    // true -- at least one positional parm is required, either on left-side or right-side (used when both sides support 0-N positional parms)  
    bool                                  parms_left_xor_right                                    { false   };    // true -- positional parms required either on left or on right (not both sides, or neither side)
    bool                                  parms_not_both_sides                                    { false   };    // true -- positional and keyword parms cannot appear on both sides (parms on either side, or neither side is OK)

    bool                                  lexical_scope                                           { false   };    // true -- verb has lexical upward scope (causes closure with upward scopes at definition time to be captured)
    bool                                  dynamic_scope                                           { false   };    // true -- verb has dynamic upward scope -- only exported upward-scoped symbols (on call chain) are visible    
    bool                                  block_scope                                             { false   };    // true -- verb has block upward scope = dynamic + all upward-scoped symbols (on call chain) are visible    
    std::shared_ptr<frame_S>              upward_scope_sp                                         {         };    // if lexical scope, this points to scope where verb was defined -- shared owning pointer                                       

    int                                   priority                                                { 0       };    // priority of this verb during expression evaluation
    bool                                  right_associate                                         { false   };    // true = always right-to-left associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix 
    bool                                  left_associate                                          { false   };    // true = always left-to-right associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix
    void                                 *fcn_p                                                   { nullptr };    // pointer to function (in C++ parser code) to process this verb (non-owning pointer)

    std::shared_ptr<slist_S>              slist_sp                                                { nullptr };    // pointer to code block for this verb (if any -- only for non built-in verbs) --= owning pointer  
                                                                                                  
    plist_S                               lparms                                                             ;    //  left-side positional and keyword descriptions
    plist_S                               rparms                                                             ;    // right-side positional and keyword descriptions
    argvar_S                              lvars                                                              ;    // variable names for left  side parms
    argvar_S                              rvars                                                              ;    // variable names for right side parms
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

    bool                           no_eval_ident                { false };      // don't evaluate, if parm is an identifier 
    bool                           no_eval_vexpr                { false };      // don't evaluate, if parm is an vexpr
    bool                           no_eval_vlist                { false };      // don't evaluate, if parm is a  vlist
    bool                           no_eval_ref                  { false };      // don't evaluate/dereference, if parm is a  ref

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
    bool                           raw_ident_ok                 { false };      // ok for vlue to be  any (raw/unevaluated) identifier (defined, or undefined)
    bool                           var_ident_ok                 { false };      // ok for value to be defined variable identifier  (not a constant)
    bool                           const_ident_ok               { false };      // ok for value to be defined constant identifier (not a variable) 
    bool                           undef_ident_ok               { false };      // ok for value to be undefined identifier (and not defined)
    bool                           vlist_ok                     { false };      // ok for value to be vlist -- optional pointer is set, if verb cares about value types in vlist
    bool                           vexpr_ok                     { false };      // ok for value to be vexpr  
    bool                           slist_ok                     { false };      // ok for value to be slist 
    bool                           verbdef_ok                   { false };      // ok for value to be verbdef
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

M_EX_IMPEXP int          add_predefined_verb(        const std::wstring&, verbdef_S&);
M_EX_IMPEXP int          attach_plist(               parmtype_S&, const plist_S&);

M_EX_IMPEXP value_S      set_type_value( int64_t             );  
M_EX_IMPEXP value_S      set_type_value( float64_T           );  
M_EX_IMPEXP value_S      set_type_value( const std::wstring& );  


/////////////////// public character and token-oriented functions  -- in ex_interface.cpp 

M_EX_IMPEXP bool is_valid_identifier(const std::wstring&); 
M_EX_IMPEXP bool is_valid_operator(  const std::wstring&); 
M_EX_IMPEXP bool is_valid_verbname(  const std::wstring&); 


/////////////////// public setup and processing functions  -- in ex_interface.cpp

M_EX_IMPEXP uint64_t      parse_setup_default(                     ); 
M_EX_IMPEXP uint64_t      process_input_default(                   ); 
M_EX_IMPEXP uint64_t      process_cmdline_default( int, wchar_t *[]);
             
M_EX_IMPEXP void          count_error(uint64_t = 1ULL); 
M_EX_IMPEXP uint64_t      error_count();
M_EX_IMPEXP frame_S      *get_main_frame();  
M_EX_IMPEXP symtab_S     *get_global_symtab();  
M_EX_IMPEXP pre_parse_C  *get_main_preparse(); 


///////////////// location string and other debug-message-oriented routines  ///////////////////////////////////

//M_EX_IMPEXP std::wstring vexpr_loc_str(  const a_vexpr_S& );                       // in ex_parse.h
//M_EX_IMPEXP std::wstring  verb_loc_str(  const a_vexpr_S& );                       // in ex_parse.h
  M_EX_IMPEXP std::wstring vexpr_loc_str(  const e_vexpr_S& );                    
  M_EX_IMPEXP std::wstring  verb_loc_str(  const e_vexpr_S& );                    
//M_EX_IMPEXP std::wstring vlist_loc_str(  const vlist_S&);                          // in ex_parse.h
//M_EX_IMPEXP std::wstring value_loc_str(  const value_S&);                          // in ex_parse.h
//M_EX_IMPEXP std::wstring    kw_loc_str(  const value_S&);                          // in ex_parse.h

//M_EX_IMPEXP void                msg_loc( const   value_S&                      );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   value_S&, const std::wstring& );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   vlist_S&                      );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   vlist_S&, const std::wstring& );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc(                   const a_vexpr_S&    );  // in ex_parse.h
  M_EX_IMPEXP void                msg_loc(                   const e_vexpr_S&    );  
//M_EX_IMPEXP void                msg_loc( const   value_S&, const a_vexpr_S&    );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   value_S&, const e_vexpr_S&    );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   vlist_S&, const a_vexpr_S&    );  // in ex_parse.h
//M_EX_IMPEXP void                msg_loc( const   vlist_S&, const e_vexpr_S&    );  // in ex_parse.h

//M_EX_IMPEXP void             msg_kw_loc( const   value_S&                      );  // in ex_parse.h

//M_EX_IMPEXP void             msgend_loc( const   value_S&                      );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   value_S&, const std::wstring& );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   vlist_S&                      );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   vlist_S&, const std::wstring& );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc(                   const a_vexpr_S&    );  // in ex_parse.h
  M_EX_IMPEXP void             msgend_loc(                   const e_vexpr_S&    );  
//M_EX_IMPEXP void             msgend_loc( const   value_S&, const a_vexpr_S&    );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   value_S&, const e_vexpr_S&    );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   vlist_S&, const a_vexpr_S&    );  // in ex_parse.h
//M_EX_IMPEXP void             msgend_loc( const   vlist_S&, const e_vexpr_S&    );  // in ex_parse.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////