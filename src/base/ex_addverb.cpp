// ex_addverb.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ==============
////     ex_addverb.cpp -- add parser-oriented pre-defined verbs to verb table
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
#include "h_ex_verbmac.h"
#include "h_ex_parse.h"
#include "h_ex_verb.h"



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_predefined_typdefs() -- add all pre-defined typedefs global symbol table 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void add_predefined_typdefs() try
{
    def_global_typdef(L"INT8_T"       , make_atomic_typdef(type_E::int8       ), true);
    def_global_typdef(L"INT16_T"      , make_atomic_typdef(type_E::int16      ), true);
    def_global_typdef(L"INT32_T"      , make_atomic_typdef(type_E::int32      ), true);
    def_global_typdef(L"INT64_T"      , make_atomic_typdef(type_E::int64      ), true);
    def_global_typdef(L"INT_T"        , make_atomic_typdef(type_E::int64      ), true);
    def_global_typdef(L"UINT8_T"      , make_atomic_typdef(type_E::uint8      ), true);
    def_global_typdef(L"UINT16_T"     , make_atomic_typdef(type_E::uint16     ), true);
    def_global_typdef(L"UINT32_T"     , make_atomic_typdef(type_E::uint32     ), true);
    def_global_typdef(L"UINT64_T"     , make_atomic_typdef(type_E::uint64     ), true);
    def_global_typdef(L"UINT_T"       , make_atomic_typdef(type_E::uint64     ), true);
    def_global_typdef(L"FLOAT32_T"    , make_atomic_typdef(type_E::float32    ), true);
    def_global_typdef(L"FLOAT64_T"    , make_atomic_typdef(type_E::float64    ), true);
    def_global_typdef(L"FLOAT_T"      , make_atomic_typdef(type_E::float64    ), true);
                                                                              
    def_global_typdef(L"UNIT_T"       , make_atomic_typdef(type_E::unit       ), true);
    def_global_typdef(L"BOOL_T"       , make_atomic_typdef(type_E::boolean    ), true);
    def_global_typdef(L"STRING_T"     , make_atomic_typdef(type_E::string     ), true);
    def_global_typdef(L"VERBNAME_T"   , make_atomic_typdef(type_E::verbname   ), true);
    def_global_typdef(L"KEYNAME_T"    , make_atomic_typdef(type_E::keyname    ), true);
    def_global_typdef(L"IDENTIFIER_T" , make_atomic_typdef(type_E::identifier ), true);
    def_global_typdef(L"VLIST_T"      , make_atomic_typdef(type_E::vlist      ), true);
    def_global_typdef(L"EXPRESSION_T" , make_atomic_typdef(type_E::vexpr      ), true);
    def_global_typdef(L"SLIST_T"      , make_atomic_typdef(type_E::slist      ), true);
    def_global_typdef(L"VERBDEF_T"    , make_atomic_typdef(type_E::verbdef    ), true);
    def_global_typdef(L"TYPEDEF_T"    , make_atomic_typdef(type_E::typdef     ), true);
    def_global_typdef(L"REF_T"        , make_atomic_typdef(type_E::ref        ), true);
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_predefined_verbs() -- add all pre-defined general verbs to global symbol table 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void add_predefined_verbs() try
{ 

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   flow-of-control verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // =============================================================================================================================================================
    //
    // ident @FN ident [v1 v2 v3 ...] {slist};          // simplified version of @VERB
    //
    //             -- no keywords
    //             -- no left-side parms
    //             -- no optional parms (number of right-side positional parms = number of arg variables in list
    //             -- no parmtype info
    //
    //
    //  sample invocations:
    //  ------------------
    //
    //   verb1 @FN [a b c]  dynamic_scope: °           // define named function
    //   {
    //       @SAY      «verb1 called»;
    //       @RETURN   a a b b c c;
    //   }
    //
    //
    //   a = @FN [a b c]                              // anonymous function
    //       {
    //           @SAY      «verb1 called»;
    //           @RETURN   a a b b c c;
    //       }
    //      
    //
    // =============================================================================================================================================================

    {
        M_vt_anyfix(                               vt, 0, 1, 2, 2                                             )   
        M_vt_undef_ident_left_pos(                 vt                                                         )   // optional

        {   
            M_vt_nested_plist(                         pl, 0, std::numeric_limits<int64_t>::max()             )           // mandatory positional parm is 1st variable name -- addional ones are optional 
            M_vt_raw_ident_nest_pos(                   pl                                                     )           // keyword name string

            M_vt_vlist_right_pos(                  vt, pl                                                     )           // var:[ ... ] allowed on left  side
        }    

        M_vt_slist_right_pos(                      vt                                                         )   // required

        M_vt_nval_optional_right_kw (              vt, L"global"                                              )
        M_vt_nval_optional_right_kw (              vt, L"local"                                               )
        M_vt_nval_optional_right_kw (              vt, L"export"                                              )
        M_vt_nval_optional_right_kw (              vt, L"dynamic_scope"                                       )
        M_vt_nval_optional_right_kw (              vt, L"lexical_scope"                                       )
        M_vt_nval_optional_right_kw (              vt, L"block_scope"                                         )
      
        M_vt_right_conflict_pair(                  vt, L"global", L"local"                                    )
        M_vt_right_conflict_3way(                  vt, L"dynamic_scope", L"lexical_scope", L"block_scope"     )

        M_vt_add(L"FN",                            vt, verb_fn                                                )
    }   



    // =============================================================================================================================================================
    //
    // left-side stuff  @VERB  name:"string"   right-side stuff   block:{slist} -- define non-builtin verb
    //
    //
    //  sample (simple) invocation:
    //  --------------------------
    //
    //
    //    @VERB name:«verb1»
    //    [int64:  ]
    //    [float64:]
    //    block:
    //    {
    //         @SAY «verb1 called»;
    //         @RETURN 6;          
    //    }
    //  
    //
    //
    // sample (complex) invocation:
    // ---------------------------
    //
    //
    //   [int64: float64: string:]
    //   [float64: float32: float_min:0.0 float_max:100.0]                                                              // this one will be used for 1st left-side positional parm
    //   [vlist: slist: verbdef: int8: int16: int32: int64: uint8: uint16: uint32: uint64: float32: float64:]           // this one will be used for 3rd, 4th, 5th ... left-side positional positional parms
    //   min:1  max:5                                                                                                   // allow 1-5 left-side positional parms 
    //
    //   key:[kw1  kw_min:0 kw_max:3   int64: int32: float32: float64: ]                                                // 0-3       occurrences  of kw1: are allowed on left side 
    //   key:[kw2                      int64: int_min:0 int_max:1000   ]                                                // 1         occurrence   of kw2: is  allowed on left side
    //   key:[kw3  kw_min:0 kw_max: -1 int64: int_min:0 int_max:10     ]                                                // 0 or more occurrences  of kw3: are allowed on left side
    //
    //   var:[v1 v2 v3 ... vn   key1:v6 key1:v7 key2:v8 key3:v9          ]                                              // variables to get initialized with parm values -- left side   
    //
    //   @VERB name:«verb2»                                                                                          
    //                                                                                                               
    //   [int64:                   int_min:-100  int_max:100  ]                                                         // used for 1st right-side positional parm                   
    //   [float64: float32:        float_min:1.0 float_max:2.0]                                                         // this one will be used for 2nd, 3rd, 4th ... right-side positional parms
    //   min:0 max:-1                                                                                                   // allow 0 or more right-side positional parms
    //                                                                                                               
    //   key:[kw1  kw_min:0 kw_max:3   int64: int32: float32: float64: ]                                                // 0-3       occurrences  of kw1: are allowed on right side 
    //   key:[kw2                      int64: int_min:0 int_max:1000   ]                                                // 1         occurrence   of kw2: is  allowed on right side
    //   key:[kw4  kw_min:0 kw_max: -1 int64: int_min:0 int_max:10     ]                                                // 0 or more occurrences  of kw4: are allowed on right side 
    //
    //   var:[v1 v2 v3 ... vn   key1:v6 key1:v7 key2:v8 key3:v9          ]                                              // variables to get initialized with parm values -- right side   
    //
    //   local:                                                                                                         // put in local scope (global: is default)
    //   export:                                                                                                        // export this verb to callee's, etc.
    //   dynamic_scope:                                                                                                 // use dynamic upward scoping
    //   block:
    //   {
    //        @SAY «verb2 called»;
    //        @RETURN 1.6;          
    //   }
    //
    //
    //
    //
    //
    //
    // ============================================================================

    {
        M_vt_anyfix(                               vt, 0, -1, 0, -1)   


        // [int64:  float64:  unit: ...etc....   ] -- positional parm description, with nested vlist  (int64: float64: ... are one or more accepted types for this positional parm)
        //      note: can't declare types, limits, etc., for parms inside of any nested vlists 
        //      note: can't declare match sets, conflict sets, or choice sets for keywords
        //      note: there can be multiple occurrences of these positional parm descriptions on both sides 
          
        { 
            M_vt_nested_plist(                         pl, 0, 0                                               )
                                                                                                            
            M_vt_nval_optional_nest_kw(                pl, L"unit"                                            )
            M_vt_nval_optional_nest_kw(                pl, L"int8"                                            )
            M_vt_nval_optional_nest_kw(                pl, L"int16"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"int32"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"int64"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"uint8"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"uint16"                                          )
            M_vt_nval_optional_nest_kw(                pl, L"uint32"                                          )
            M_vt_nval_optional_nest_kw(                pl, L"uint64"                                          )
            M_vt_nval_optional_nest_kw(                pl, L"float32"                                         )
            M_vt_nval_optional_nest_kw(                pl, L"float64"                                         )
            M_vt_nval_optional_nest_kw(                pl, L"string"                                          )
            M_vt_nval_optional_nest_kw(                pl, L"raw_ident"                                       )
            M_vt_nval_optional_nest_kw(                pl, L"var_ident"                                       )
            M_vt_nval_optional_nest_kw(                pl, L"const_ident"                                     )
            M_vt_nval_optional_nest_kw(                pl, L"undef_ident"                                     )
            M_vt_nval_optional_nest_kw(                pl, L"vlist"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"expression"                                      )
            M_vt_nval_optional_nest_kw(                pl, L"slist"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"verbdef"                                         )
            M_vt_nval_optional_nest_kw(                pl, L"typedef"                                         )
            M_vt_nval_optional_nest_kw(                pl, L"ref"                                             )
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_ident"                                   )  
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_expression"                              )
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_vlist"                                   ) 
            M_vt_float64_optional_nest_kw(             pl, L"float_min"                                       )
            M_vt_float64_optional_nest_kw(             pl, L"float_max"                                       )
            M_vt_int64_optional_nest_kw(               pl, L"int_min"                                         )
            M_vt_int64_optional_nest_kw(               pl, L"int_max"                                         )
                                                                                                            
            M_vt_nest_match_pair(                      pl, L"float_min", L"float_max"                         )   
            M_vt_nest_match_pair(                      pl, L"int_min"  , L"int_max"                           ) 
                                                                                                            
            M_vt_vlist_left_pos(                   vt, pl                                                     )   // [ unit: int64: ... ] [ slist: int64: ... ] ...   allowed on left  side
            M_vt_vlist_right_pos(                  vt, pl                                                     )   // [ unit: int64: ... ] [ slist: int64: ... ] ...   allowed on right side
        }                                                                                                   
                                                                                                            
        // unnested keyword parms -- left-side and right-side                                               
                                                                                                            
        M_vt_string_optional_right_kw(             vt, L"name"                                                )
        M_vt_slist_required_right_kw(              vt, L"block"                                               ) 
        M_vt_nval_optional_right_kw (              vt, L"global"                                              )
        M_vt_nval_optional_right_kw (              vt, L"local"                                               )
        M_vt_nval_optional_right_kw (              vt, L"export"                                              )
        M_vt_nval_optional_right_kw (              vt, L"dynamic_scope"                                       )
        M_vt_nval_optional_right_kw (              vt, L"lexical_scope"                                       )
        M_vt_nval_optional_right_kw (              vt, L"block_scope"                                         )
                                                                                                            
        M_vt_int64rc_optional_right_kw(            vt, L"min", 0, M_int64_max                                 )                
        M_vt_int64rc_optional_right_kw(            vt, L"max",-1, M_int64_max                                 ) 
        M_vt_int64rc_optional_left_kw(             vt, L"min", 0, M_int64_max                                 )                
        M_vt_int64rc_optional_left_kw(             vt, L"max",-1, M_int64_max                                 )
                                                                                                            
        M_vt_right_match_pair(                     vt, L"min"   , L"max"                                      )   
        M_vt_left_match_pair(                      vt, L"min"   , L"max"                                      ) 
        M_vt_right_conflict_pair(                  vt, L"global", L"local"                                    )
        M_vt_right_conflict_3way(                  vt, L"dynamic_scope", L"lexical_scope", L"block_scope"     )


        // key: ["kw-name" kw_min: nnn   kw_max: nnn   int64:  float64:  unit: ...etc....   ] -- keyword parm, with nested vlist  (kw_name required, int64: float64: are one or more accepted types for this keyword)
        //      note: can't declare types, limits, etc., for parms inside of any nested vlists  
        //      note: there can be multiple occurrences of these keyword parm descriptions on both sides 

        {   
            M_vt_nested_plist(                         pl, 1, 1                                               )    // mandatory positional parm is the keyword name 
            M_vt_raw_ident_nest_pos(                   pl                                                     )    // keyword name (as "raw"identifier)
            M_vt_int64rc_optional_nest_kw(             pl, L"kw_min", 0 , std::numeric_limits<int64_t>::max() )    // ???
            M_vt_int64rc_optional_nest_kw(             pl, L"kw_max", -1, std::numeric_limits<int64_t>::max() )    // ???
            M_vt_nval_optional_nest_kw(                pl, L"unit"                                            )
            M_vt_nval_optional_nest_kw(                pl, L"int8"                                            )
            M_vt_nval_optional_nest_kw(                pl, L"int16"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"int32"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"int64"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"uint8"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"uint16"                                          )
            M_vt_nval_optional_nest_kw(                pl, L"uint32"                                          )
            M_vt_nval_optional_nest_kw(                pl, L"uint64"                                          )
            M_vt_nval_optional_nest_kw(                pl, L"float32"                                         )
            M_vt_nval_optional_nest_kw(                pl, L"float64"                                         )
            M_vt_nval_optional_nest_kw(                pl, L"string"                                          )
            M_vt_nval_optional_nest_kw(                pl, L"var_ident"                                       )
            M_vt_nval_optional_nest_kw(                pl, L"const_ident"                                     )
            M_vt_nval_optional_nest_kw(                pl, L"undef_ident"                                     )
            M_vt_nval_optional_nest_kw(                pl, L"vlist"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"expression"                                      )
            M_vt_nval_optional_nest_kw(                pl, L"slist"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"verbdef"                                         ) 
            M_vt_nval_optional_nest_kw(                pl, L"typedef"                                         ) 
            M_vt_nval_optional_nest_kw(                pl, L"array"                                           ) 
            M_vt_nval_optional_nest_kw(                pl, L"struct"                                          ) 
            M_vt_nval_optional_nest_kw(                pl, L"ref"                                             ) 
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_ident"                                   )  
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_expression"                              )
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_vlist"                                   ) 
            M_vt_float64_optional_nest_kw(             pl, L"float_min"                                       )
            M_vt_float64_optional_nest_kw(             pl, L"float_max"                                       )
            M_vt_int64_optional_nest_kw(               pl, L"int_min"                                         )
            M_vt_int64_optional_nest_kw(               pl, L"int_max"                                         )

            M_vt_nest_match_pair(                      pl, L"kw_min"     , L"kw_max"                          )  
            M_vt_nest_match_pair(                      pl, L"float_min"  , L"float_max"                       )   
            M_vt_nest_match_pair(                      pl, L"int_min"    , L"int_max"                         ) 

            M_vt_vlist_multi_left_kw(  vt,     L"key", pl, 0, std::numeric_limits<int64_t>::max()             )           //  key:["name" ...   ] key:["name" ...  ] ...  allowed on left  side
            M_vt_vlist_multi_right_kw( vt,     L"key", pl, 0, std::numeric_limits<int64_t>::max()             )           //  key:["name" ...   ] key:["name" ...  ] ...  allowed on right side
        } 


        // var: [v1 v2 v3 .. vn   kw1:vk11 kw1:vk12 kw1:vk13 kw2:vk21 kw3:vk31 ... ] -- variable names for positional and keyword parms (1st-level only)
        // var:[...] is optional and can appear only once on left side and once on right side 

        {   
            M_vt_nested_plist(                         pl, 1, std::numeric_limits<int64_t>::max()             )           // mandatory positional parm is 1st variable name -- additional ones are optional 
            M_vt_raw_ident_nest_pos(                   pl                                                     )           // keyword name string
                                                                                                                         
            M_vt_raw_ident_multi_nest_kw(              pl, L"", 0, std::numeric_limits<int64_t>::max()        )           // any keyword name can be present, any number of times, "" is required undefined kw-name placeholder, here    
            
                                                       pl.no_check_keyword_names = true;                                  // want to allow any keyword names, but check their types                                      

            M_vt_vlist_optional_left_kw(  vt,  L"var", pl                                                     )           // var:[ ... ] allowed on left  side
            M_vt_vlist_optional_right_kw( vt,  L"var", pl                                                     )           // var:[ ... ] allowed on right side
        }    

        M_vt_add(L"VERB",                          vt, verb_verb                                              )
    }
                        
     
    // ====================================
    // @UNVERB     "string" "string" ... 
    // ====================================
   
    {
        M_vt_nary_prefix(                 vt                        )
        M_vt_string_right_pos(            vt                        ) 
        M_vt_nval_optional_right_kw (     vt, L"global"             )
        M_vt_nval_optional_right_kw (     vt, L"local"              )
        M_vt_right_conflict_pair(         vt, L"global", L"local"   )
        M_vt_add(L"UNVERB",               vt, verb_unverb           )
    }   




    // ================================================================================================
    // @PARSE "string"   name:"name of string -- for debugging"  continue: -or- end: (if parsing error)
    // ================================================================================================
   
    {
        M_vt_unary_prefix(                  vt                        )  
        M_vt_string_optional_right_kw(      vt, L"name"               )                      // optional name:debug_name -- appears in error messages
        M_vt_nval_optional_right_kw(        vt, L"continue"           )                      // optional continue:   continue running, if parsing error
        M_vt_nval_optional_right_kw(        vt, L"end"                )                      // optional end:        end run,          if parsing error    (default)
        M_vt_right_conflict_pair(           vt, L"continue", L"end"   )
                                                                  
        M_vt_string_right_pos(              vt                        )                      // string to be parsed 
        M_vt_add(L"PARSE",                  vt, verb_parse            )
    }   




    // =========================================
    // @CALL "string" left:[vlist] right:[vlist] 
    // =========================================
   
    {
        M_vt_unary_prefix(                  vt              )  
        M_vt_vlist0_optional_right_kw(      vt, L"left"     )
        M_vt_vlist0_optional_right_kw(      vt, L"right"    )
                                                            
        M_vt_string_right_pos(              vt              )                      // verb name -- required
                                            
        M_vt_add(L"CALL",                   vt, verb_call   )
    }   
     



    // =========================================
    // @XCTL "string" left:[vlist] right:[vlist] 
    // =========================================

    {
        M_vt_unary_prefix(vt)
        M_vt_vlist0_optional_right_kw(vt, L"left"     )
        M_vt_vlist0_optional_right_kw(vt, L"right"    )

        M_vt_string_right_pos(        vt              )                      // verb name -- required

        M_vt_add(L"XCTL",             vt, verb_xctl   )
    }
    



    // ===============================================================
    // @SHELL "cmd string"   capture: 
    // ===============================================================
   
    {
        M_vt_unary_prefix(            vt              )  
        M_vt_nval_optional_right_kw(  vt, L"capture"  )                      // optional capture:   (to capture shell command outpout 
                                                            
        M_vt_string_right_pos(        vt              )                      // shell command string -- required

        M_vt_add(L"SHELL"       ,     vt, verb_shell  )
    }  



    // ==================================
    // , @SEP   binary separator operator    
    // ==================================

    {
        M_vt_nary_anyfix(             vt             ) 
        {
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                       // want to pass through all parms verbatim (unevaluated)
            pt.no_eval_ident              = true; 
            pt.no_eval_vexpr              = true; 
            pt.no_eval_vlist              = true; 
            pt.no_eval_ref                = true; 
            M_vt_add_l_pos(           vt, pt)
        }                             
        {                             
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                        // want to pass through all parms verbatim (unevaluated)
            pt.no_eval_ident              = true; 
            pt.no_eval_vexpr              = true; 
            pt.no_eval_vlist              = true; 
            pt.no_eval_ref                = true; 
            M_vt_add_r_pos(           vt, pt)    
        } 

        M_vt_any_multi_left_kw(       vt, L"", 0, std::numeric_limits<int64_t>::max()        )           // any keyword name can be present, any number of times, "" is required undefined kw-name placeholder, here 
        M_vt_any_multi_right_kw(      vt, L"", 0, std::numeric_limits<int64_t>::max()        )           // any keyword name can be present, any number of times, "" is required undefined kw-name placeholder, here 
            
        vt.lparms.no_check_keyword_names  = true;                                                        // want to allow any keyword names, but check their types 
        vt.rparms.no_check_keyword_names  = true;                                                        // want to allow any keyword names, but check their types 

        vt.priority = verb_priority_separate; 
        M_vt_add(L","  ,   vt, verb_separate)
        M_vt_add(L"SEP",   vt, verb_separate)
    }

    
    // ===========================================================
    // @BREAK      -- end slist evaluation -- stop looping (if in loop)
    // @CONTINUE   -- end slist evaluation -- keep looping (if in loop)
    // @END        -- end all evaluation 
    // @QUIT       -- end evaluation of lowest enclosing block 
    // @RETHROW    -- do @THROW with current block 1st positional parm 
    // @ABORT      -- do abort()
    // @EXIT       -- do exit(-1)
    // @QUICK_EXIT -- do quick_exit(-1)
    // @_EXIT      -- do _exit(-1)
    // =============================================================

    {
        M_vt_nofix(vt) 
        M_vt_add(L"BREAK"         , vt, verb_break          )
        M_vt_add(L"CONTINUE"      , vt, verb_continue       )
        M_vt_add(L"END"           , vt, verb_end            )
        M_vt_add(L"QUIT"          , vt, verb_quit           )
        M_vt_add(L"RETHROW"       , vt, verb_rethrow        )
        M_vt_add(L"ABORT"         , vt, verb_abort          )
        M_vt_add(L"EXIT"          , vt, verb_exit           )
        M_vt_add(L"_EXIT"         , vt, verb__exit          )
        M_vt_add(L"QUICK_EXIT"    , vt, verb_quick_exit     )
    }


    // ============================================================================
    // @RETURN value -- value is optional
    // ============================================================================

    {
        M_vt_nary_prefix(         vt             )               // 0 to N right positional parms are allowed 
        M_vt_assigntype_right_pos(vt             )
        M_vt_add(L"RETURN",       vt, verb_return)
    }  


    // ============================================================================
    // @THROW value -- value is required
    // ============================================================================

    {
        M_vt_unary_prefix(    vt) 
        M_vt_assigntype_right_pos(vt)
        M_vt_add(L"THROW", vt, verb_throw)
    }  


    // ============================================================================
    // @LEAVE  -or-  @LEAVE "target" -- end current (or enclosing) slist evaluation
    // ============================================================================

    {
        M_vt_prefix(          vt, 0, 1      )                     // 0 or 1 right positional parms are allowed 
        M_vt_string_right_pos(vt            )                     // leave target (if present) needs to be a string
        M_vt_add(L"LEAVE",    vt, verb_leave)
    }

    
    // ===========================================================
    // @GOTO "target" -- restart slist evaluation after label
    // ===========================================================

    {
        M_vt_unary_prefix(    vt       ) 
        M_vt_string_right_pos(vt       )         // goto target needs to be a string
        M_vt_add(L"GOTO", vt, verb_goto)
    }

    
    // =====================================
    // @DO {slist}   continue:
    // =====================================

    {
        M_vt_unary_prefix(                  vt                        )
        M_vt_slist_right_pos(               vt                        ) 

        M_vt_nval_optional_right_kw(        vt, L"continue"           )                      // optional continue:   continue running, if parsing error
        M_vt_nval_optional_right_kw(        vt, L"end"                )                      // optional end:        end run,          if parsing error    (default)
        M_vt_right_conflict_pair(           vt, L"continue", L"end"   )

        M_vt_add(L"DO",                     vt, verb_do               )
    }
 

    // =====================================
    // @TRY {slist} catch:{slist}
    // =====================================
          
    {
        M_vt_unary_prefix(           vt            )  
        M_vt_slist_right_pos(        vt            ) 
        
        M_vt_slist_optional_right_kw(vt, L"catch"  )
        M_vt_slist_optional_right_kw(vt, L"finally")

        M_vt_add(L"TRY",             vt, verb_try  )
    }


    // =========================================
    // @BLOCK left:[vlist] right:[vlist] {slist}
    // =========================================

    {
        M_vt_unary_prefix(            vt             )
        M_vt_slist_right_pos(         vt             )

        M_vt_vlist0_optional_right_kw(vt, L"left"   )
        M_vt_vlist0_optional_right_kw(vt, L"right"  )

        M_vt_add(L"BLOCK",            vt, verb_block)
    }



    // =====================================
    // @CASE when:(expression) {slist} when:(expression) {slist} ... else:{slist} 
    // =====================================

    {
        M_vt_nary_prefix(             vt         )
        M_vt_slist_right_pos(         vt         )

        {                                                 // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_optional_kw_parm(kp)
            kp.kw_min_ct      =  0;
            kp.kw_max_ct      = -1;          // multiple when: keywords allowed
            kp.no_eval_ident  = true;
            kp.no_eval_vexpr  = true;
            kp.int8_ok        = true; 
            kp.int16_ok       = true;
            kp.int32_ok       = true;
            kp.int64_ok       = true;
            kp.uint8_ok       = true;
            kp.uint16_ok      = true;
            kp.uint32_ok      = true;
            kp.uint64_ok      = true;
            kp.var_ident_ok   = true;
            kp.const_ident_ok = true;
            kp.undef_ident_ok = true;
            kp.vexpr_ok       = true;
            M_vt_add_r_kw(L"when",   vt, kp       ) 
        }

        M_vt_slist_optional_right_kw(vt, L"else"  )
 
        M_vt_add(L"CASE",            vt, verb_case)
    }
                               
    
    // =====================================
    // @LOOP while:int-value  {slist}
    // =====================================
          
    {
        M_vt_unary_prefix(           vt             )  

        {                                                   // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_optional_kw_parm(kp);

            kp.no_eval_ident  = true;
            kp.no_eval_vexpr  = true;
            kp.boolean_ok     = true; 
            kp.int8_ok        = true; 
            kp.int16_ok       = true;
            kp.int32_ok       = true;
            kp.int64_ok       = true;
            kp.uint8_ok       = true;
            kp.uint16_ok      = true;
            kp.uint32_ok      = true;
            kp.uint64_ok      = true;
            kp.var_ident_ok   = true;
            kp.const_ident_ok = true;
            kp.undef_ident_ok = true;
            kp.vexpr_ok       = true;

            M_vt_add_r_kw(L"until",  vt, kp        ) 
            M_vt_add_r_kw(L"while",  vt, kp        ) 
        }
        
        M_vt_slist_optional_right_kw(vt, L"init"   ) 
        M_vt_slist_optional_right_kw(vt, L"next"   )

        M_vt_slist_right_pos(        vt            )

        M_vt_add(L"LOOP",            vt, verb_loop)
    }

    
    // =====================================
    // @IF value then:{slist} else:{slist}
    // =====================================

    {
        M_vt_unary_prefix(            vt           ) 

        M_vt_slist_optional_right_kw( vt, L"then"  )
        M_vt_slist_optional_right_kw( vt, L"else"  )  

        M_vt_int_right_pos(           vt           )

        M_vt_add(L"IF",               vt, verb_if  )
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   variable definition and assignment verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // =======================================================================================================
    // @VAR ident ident ident ... value:value  global: export: share: unshare:  -- value: keyword is optional
    // =======================================================================================================
   
    {
        M_vt_nary_prefix(                 vt                        )                                                                     
        M_vt_assigntype_optional_right_kw(vt, L"value"              )
        M_vt_nval_optional_right_kw (     vt, L"global"             )
        M_vt_nval_optional_right_kw (     vt, L"local"              )
        M_vt_nval_optional_right_kw (     vt, L"export"             )
        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )

        M_vt_right_conflict_pair(         vt, L"global", L"local"   )
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" ) 

        M_vt_any_ident_right_pos(         vt                        )

        M_vt_add(L"VAR",                  vt, verb_var              )
    }

   
    // =======================================================================================
    // @CONST ident value:value global: export: share: unshare:  -- value: keyword is required   
    // =======================================================================================

    {
        M_vt_unary_prefix(                vt                        )                                                                      
        M_vt_assigntype_optional_right_kw(vt, L"value"              )
        M_vt_nval_optional_right_kw (     vt, L"global"             )
        M_vt_nval_optional_right_kw (     vt, L"local"              )
        M_vt_nval_optional_right_kw (     vt, L"export"             )
        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )

        M_vt_right_conflict_pair(         vt, L"global", L"local"   )
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" ) 

        M_vt_any_ident_right_pos(         vt                        )
             
        M_vt_add(L"CONST",                vt, verb_const)
    }


    // ===============================
    // @NOEVAL ident   share: noshare: 
    // ===============================

    {
        M_vt_unary_prefix(                vt                        )
 
        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" )  

        M_vt_any_ident_right_pos(         vt                        )

        M_vt_add(L"NOEVAL"  ,             vt, verb_noeval           )
    }  


    // ===========================
    // @EVAL value value value ...
    // ===========================

    {
        M_vt_nary_prefix(                 vt                        )      
        M_vt_any_right_pos(               vt                        )      
        M_vt_add(L"EVAL",                 vt, verb_eval             );        
    }

    // ======================
    // @UNSHARE value 
    // ======================

    {
        M_vt_unary_prefix(                vt                        ) 
        M_vt_assigntype_right_pos(        vt                        )

        M_vt_add(L"UNSHARE",              vt, verb_unshare          )
    }  


    // ======================
    // @EXPORT    ident ident ...
    // @UNEXPORT  ident ident ...
    // ======================

    {
        M_vt_nary_prefix(        vt               )
        M_vt_def_ident_right_pos(vt               )                                                                                                                                    
        M_vt_add(L"EXPORT"  ,    vt, verb_export  )
        M_vt_add(L"UNEXPORT",    vt, verb_unexport)
    }  


    // ====================================
    // @UNVAR     ident ident ...   global:
    // ====================================
   
    {
        M_vt_nary_prefix(                 vt                        )
        M_vt_any_ident_right_pos(         vt                        )    
        M_vt_nval_optional_right_kw(      vt, L"global"             ) 
        M_vt_nval_optional_right_kw(      vt, L"local"              )
        M_vt_right_conflict_pair(         vt, L"global", L"local"   )
        M_vt_add(L"UNVAR",                vt, verb_unvar            )
    }   


    // ================================================
    // >=>  =  <=<  left and right assignment operators
    // ================================================
    
    {                                      /* 1 or more vars */
        M_vt_anyfix(                      vt, 1, -1, 0, -1          )  
        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" ) 

        M_vt_lvalue_left_pos(             vt                        )
        M_vt_assigntype_right_pos(        vt                        )

        vt.priority        = verb_priority_assign; 
        vt.right_associate = true; 
        M_vt_add(L"="  ,                  vt, verb_left_assign      )
        M_vt_add(L"<=<",                  vt, verb_left_assign      )               
    }

    {                                     /* 1 or more vars */
        M_vt_anyfix(                      vt, 0, -1, 1, -1          ) 

        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" ) 

        M_vt_lvalue_right_pos(            vt                        )
        M_vt_assigntype_left_pos(         vt                        )

        vt.priority  = verb_priority_assign;
        M_vt_add(L">=>",                  vt, verb_right_assign     )              
    }
      

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   arithmetic and string assignment operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // =========================================
    // ++ -- unary increment/decrement operators
    // =========================================

    {
        M_vt_unary_onefix(        vt                     )                     // unary prefix or postfix
        M_vt_lvalue_left_pos(     vt                     )                     // either left -or- right allowed 
        M_vt_lvalue_right_pos(    vt                     )                     // either right -or- left allowed

        vt.priority               = verb_priority_increment;
        M_vt_add(L"++", vt, verb_increment)
        M_vt_add(L"--", vt, verb_decrement)
    }


    // =======================================================
    // binary arithmetic xxx_eq operators      += -= *= /= ^=
    // =======================================================

     { 
        M_vt_binary_infix(       vt                     )
        M_vt_lvalue_left_pos(    vt                     )
        M_vt_arith_right_pos(    vt                     )

        vt.priority               = verb_priority_assign; 
        vt.right_associate        = true; 
        M_vt_add(L"+=" ,         vt, verb_add_eq        )
        M_vt_add(L"-=" ,         vt, verb_subtract_eq   )
        M_vt_add(L"*=" ,         vt, verb_multiply_eq   )
        M_vt_add(L"×=" ,         vt, verb_multiply_eq   )
        M_vt_add(L"/=" ,         vt, verb_divide_eq     )
        M_vt_add(L"÷=" ,         vt, verb_divide_eq     )
        M_vt_add(L"**=",         vt, verb_power_eq      )        
    }


    // ================================================
    //  binary integer xxx_eq operator     %=   @REM_EQ
    // ================================================

    { 
        M_vt_binary_infix(       vt                      )
        M_vt_lvalue_left_pos(    vt                      )
        M_vt_int_right_pos(      vt                      )     // only supported for integers only

        vt.priority               = verb_priority_assign; 
        vt.right_associate        = true; 
        M_vt_add(L"%="     ,     vt, verb_remainder_eq   )
        M_vt_add(L"REM_EQ" ,     vt, verb_remainder_eq   )
    }


    // ===========================================
    //  binary string concatenate_eq() verb     |=   
    // ===========================================

    { 
        M_vt_binary_infix(       vt                   )
        M_vt_lvalue_left_pos(    vt                   )
        M_vt_string_right_pos(   vt                   )     // only supported for strings
       
        vt.priority = verb_priority_assign; 
        vt.right_associate        = true; 
        M_vt_add(L"|=",          vt, verb_concatenate_eq )
    }


    // ================================================================================================================
    // binary bitwise/shift xxx_eq operators      @BITAND_EQ  @BITOR_EQ  @BITXOR_EQ   ∧∧=  ∨∨=  ⊻⊻=    >>=   <<=   >->=
    // ================================================================================================================

    //   ??????????? missing @BITNOT_EQ    or   ~=     these are unary prefix  ??????????????????????????? 

     { 
        M_vt_binary_infix(       vt                                   )
        M_vt_lvalue_left_pos(    vt                                   )
        M_vt_int_right_pos(      vt                                   )        // only supported for integers  

        vt.priority               = verb_priority_assign; 
        vt.right_associate        = true; 
        M_vt_add(L"BITAND_EQ" ,  vt, verb_bitand_eq                   )
        M_vt_add(L"∧∧="       ,  vt, verb_bitand_eq                   )
        M_vt_add(L"BITOR_EQ"  ,  vt, verb_bitor_eq                    )
        M_vt_add(L"∨∨="       ,  vt, verb_bitor_eq                    )
        M_vt_add(L"BITXOR_EQ" ,  vt, verb_bitxor_eq                   )
        M_vt_add(L"⊻⊻="      ,   vt, verb_bitxor_eq                   )
        M_vt_add(L"<<="       ,  vt, verb_shift_left_eq               )
        M_vt_add(L">>="       ,  vt, verb_shift_right_logical_eq      )
        M_vt_add(L">->="      ,  vt, verb_shift_right_arithmetic_eq   )
    }
       

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   comparison operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // =========================================================================================
    // ==  ≡  ¬=  !=  ≠  >  <  >=  <=  @EQ  @NE  @LT  @GT  @LE  @GE  binary comparison operators
    // =========================================================================================

    {
        M_vt_binary_infix(     vt         )
        vt.parms_same_type     = true;
        M_vt_compare_right_pos(vt         ) 
        M_vt_compare_left_pos( vt         )

        vt.priority = verb_priority_compare;
        M_vt_add(L"EQ",        vt, verb_eq)
        M_vt_add(L"==",        vt, verb_eq)
        M_vt_add(L"≡" ,        vt, verb_eq)

        M_vt_add(L"NE",        vt, verb_ne)
        M_vt_add(L"¬=",        vt, verb_ne)
        M_vt_add(L"!=",        vt, verb_ne)
        M_vt_add(L"≠" ,        vt, verb_ne)

        M_vt_add(L"LT",        vt, verb_lt)
        M_vt_add(L"<" ,        vt, verb_lt)

        M_vt_add(L"GT",        vt, verb_gt)
        M_vt_add(L">" ,        vt, verb_gt)

        M_vt_add(L"LE",        vt, verb_le)
        M_vt_add(L"<=",        vt, verb_le)
        M_vt_add(L"≤" ,        vt, verb_le)

        M_vt_add(L"GE",        vt, verb_ge)
        M_vt_add(L">=",        vt, verb_ge)
        M_vt_add(L"≥" ,        vt, verb_ge)
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   logical operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // ==============================================================
    //  ∧  &&  ∨  ||  ⊻    binary logical operators (@AND  @OR  @XOR)
    // ============================================================== 

    // without lazy evaluation

    {
        M_vt_binary_infix(  vt           )
        M_vt_int_right_pos( vt           ) 
        M_vt_int_left_pos(  vt           ) 
      
        vt.priority = verb_priority_boolean;
        M_vt_add(L"XOR"  ,  vt, verb_xor )
        M_vt_add(L"⊻"    ,  vt, verb_xor )
    }


    // with lazy evaluation

    {
        M_vt_binary_infix(  vt)

        M_vt_int_left_pos(  vt           ) 

        {                                          // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_pos_parm(      pt       )
            pt.no_eval_ident   = true; 
            pt.no_eval_vexpr   = true; 
            pt.int8_ok         = true;
            pt.int16_ok        = true;
            pt.int32_ok        = true;
            pt.int64_ok        = true;
            pt.uint8_ok        = true;
            pt.uint16_ok       = true;
            pt.uint32_ok       = true;
            pt.uint64_ok       = true;
            pt.var_ident_ok    = true;
            pt.const_ident_ok  = true;
            pt.undef_ident_ok  = true;
            pt.vexpr_ok        = true;
            M_vt_add_r_pos( vt, pt     )
        }
      
        vt.priority = verb_priority_boolean;
        M_vt_add(L"AND"  , vt, verb_and)
        M_vt_add(L"∧"    , vt, verb_and)
        M_vt_add(L"&&"   , vt, verb_and)
        M_vt_add(L"OR"   , vt, verb_or )
        M_vt_add(L"∨"    , vt, verb_or )
        M_vt_add(L"||"   , vt, verb_or )
    }

       
    // =====================================
    //  ¬  !  unary logical operators (@NOT)
    // ===================================== 

    {
        M_vt_unary_prefix(  vt          )
        M_vt_int_left_pos(  vt          ) 
        
        vt.priority         = verb_priority_not;
        vt.right_associate  = true; 

        M_vt_add(L"NOT" ,   vt, verb_not)
        M_vt_add(L"¬"   ,   vt, verb_not)
        M_vt_add(L"!"   ,   vt, verb_not)
    }
   

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   bitwise operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    
    // ===============================================
    // ∧∧  ∨∨  ⊻⊻  binary/bilateral bitwise operators (@BITAND   @BITOR  @BITXOR) 
    // ===============================================

    {
        M_vt_binary_infix(      vt          )

        vt.parms_same_type     = true;

        M_vt_int_left_pos(      vt          )
        M_vt_int_right_pos(     vt          )


        vt.priority = verb_priority_bitwise; 

        M_vt_add(L"∧∧"    ,    vt, verb_bitand )
        M_vt_add(L"BITAND",    vt, verb_bitand )
        M_vt_add(L"∨∨"    ,    vt, verb_bitor  )
        M_vt_add(L"BITOR" ,    vt, verb_bitor  )
        M_vt_add(L"⊻⊻"    ,    vt, verb_bitxor )
        M_vt_add(L"BITXOR",    vt, verb_bitxor )  
    }


    // ===================================
    // ~  unary bitwise operator (@BITNOT)       
    // ===================================

    {
        M_vt_unary_prefix(     vt          )
        M_vt_int_left_pos(     vt          ) 
        
        vt.priority         = verb_priority_bitnot;
        vt.right_associate  = true; 

        M_vt_add(L"BITNOT" ,   vt, verb_bitnot)
        M_vt_add(L"~"      ,   vt, verb_bitnot) 
    }


    // ===================================
    // >>  >->  <<  binary shift operators  
    // ===================================

    {
        M_vt_binary_infix(      vt          )

       // vt.parms_same_type     = true;        -- any mix of integer types allowedint

        M_vt_int_left_pos(      vt          )
        M_vt_int_right_pos(     vt          )  

        vt.priority = verb_priority_shift; 

        M_vt_add(L"<<"    ,    vt, verb_shift_left            )
        M_vt_add(L">>"    ,    vt, verb_shift_right_logical   )
        M_vt_add(L">->"   ,    vt, verb_shift_right_arithmetic)
    } 


    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   arithmetic and string operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // ====================================
    // + * / ** binary arithmetic operators
    // ====================================

    {
        M_vt_binary_infix(    vt          )

        vt.parms_same_type     = true;

        M_vt_arith_left_pos(  vt          )
        M_vt_arith_right_pos( vt          )

        vt.priority = verb_priority_add; 
        M_vt_add(L"+",        vt, verb_add)

        vt.priority = verb_priority_multiply; 
        M_vt_add(L"*",        vt, verb_multiply)
        M_vt_add(L"/",        vt, verb_divide)
      
        vt.priority = verb_priority_power; 
        M_vt_add(L"**",       vt, verb_power)
    }


    // ================================================
    // @REM %  binary remainder operator (integer only)
    // ================================================

    {
        M_vt_binary_infix(  vt)

        vt.parms_same_type     = true;

        M_vt_int_left_pos(  vt          )
        M_vt_int_right_pos( vt          ) 

        vt.priority = verb_priority_multiply; 
        M_vt_add(L"%"  ,    vt, verb_remainder)
        M_vt_add(L"REM",    vt, verb_remainder)
    }


    // ======================================
    // - unary or binary arithmetic operators
    // ======================================

    {
        M_vt_anyfix(          vt, 0, 1, 1, 1)           // one right side parm required, left side parm is optional  
        vt.priority = verb_priority_add;

        vt.parms_same_type     = true;

        M_vt_arith_left_pos(  vt            )
        M_vt_arith_right_pos( vt            )

        M_vt_add(L"-",        vt, verb_minus)        
    }


    // ==========================
    // |  binary string operators
    // ==========================

    {
        M_vt_binary_infix(     vt                  ) 
                                                
        M_vt_string_left_pos(  vt                  )
        M_vt_string_right_pos( vt                  )

        M_vt_add(L"|",         vt, verb_concatenate)
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   aggregate-oriented verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // =============================================================================================
    //
    //  @AGG  len: integer    type: typedef_name 
    //
    //    - at least one kw must be present
    //    - if both are present, len must be at least as long as the length of type:
    //    - if type: is omitted, type of aggregate will be array, with len: elements of type UINT8_T   
    //
    // ============================================================================================= 

    {
        M_vt_nofix(                             vt                                                         )                                                                                      
        M_vt_int64rc_optional_right_kw(         vt, L"len"   , 1, std::numeric_limits<int64_t>::max()      )          
        M_vt_typdef_optional_right_kw(          vt, L"type"                                                )  
        M_vt_add(L"AGG"                       , vt, verb_agg                                               ) 
    } 


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   type-oriented verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   

    // ====================================================
    //
    //  @TYPE name:typedef_name 
    //        global:
    //        export:
    //       // unit:
    //       // bool:
    //        int8:
    //        uint8:
    //        int16:
    //        uint16:
    //        int32:
    //        uint32:
    //        int64:
    //        uint64:
    //        float32:
    //        float64:
    //        array:[n (@TYPE ...)]
    //        struct:[                                                  
    //                 field_name1:[(@TYPE ...) (optional)offset:nn] 
    //                 field_name2:[(@TYPE ...) (optional)offset:nn]
    //               ]
    //
    // ==================================================== 

    {                                   
        M_vt_nofix(                       vt                                                                             )     
                                                                                                                           
        M_vt_any_ident_optional_right_kw( vt, L"name"                                                                    )           // has to be tested later, since definnition can be in local or global scope
        M_vt_nval_optional_right_kw (     vt, L"global"                                                                  )           // default is local -- ignored if name: is not present
        M_vt_nval_optional_right_kw (     vt, L"export"                                                                  )           // default is don't export -- ignored if global: (or name: is not present)
        M_vt_nval_optional_right_kw (     vt, L"unit"                                                                    )
        M_vt_nval_optional_right_kw (     vt, L"bool"                                                                    )
        M_vt_nval_optional_right_kw (     vt, L"int8"                                                                    )
        M_vt_nval_optional_right_kw (     vt, L"uint8"                                                                   )
        M_vt_nval_optional_right_kw (     vt, L"int16"                                                                   )
        M_vt_nval_optional_right_kw (     vt, L"uint16"                                                                  )
        M_vt_nval_optional_right_kw (     vt, L"int32"                                                                   )
        M_vt_nval_optional_right_kw (     vt, L"uint32"                                                                  )
        M_vt_nval_optional_right_kw (     vt, L"int64"                                                                   )
        M_vt_nval_optional_right_kw (     vt, L"uint64"                                                                  )
        M_vt_nval_optional_right_kw (     vt, L"float32"                                                                 )
        M_vt_nval_optional_right_kw (     vt, L"float64"                                                                 )

        // array:[ n_elements element_type_t ] 

        {
            M_vt_nested_plist(                              pl, 2, 2                                                     )           // mandatory positional parms are:   number of array elements -and- type of each array element 
            M_vt_int64rc_nest_pos(                          pl, 1, std::numeric_limits<int64_t>::max()                   )           // mandatory -- number of array elements
            M_vt_typdef_nest_pos(                           pl                                                           )           // mandatory -- type of each array element                                                                    
                                                                                                                        
            M_vt_vlist_optional_right_kw( vt, L"array"    , pl                                                           )
        }  
         

        // struct:[
        //           [  fieldname_1    type1_t                  ]
        //           [  fieldname_2    type2_t     offset:100   ]
        //           .                             same:
        //           .                             high:
        //           .
        //           [  fieldname_n    typen_t     skip:8       ]
        //        ]

        {
            M_vt_nested_plist(                              pl, 1, std::numeric_limits<int64_t>::max()                   )           // mandatory positional parms are:  1-N field definition vlists
            {                                                                                                           
                M_vt_nested_plist(                              pln, 2, 2                                                )           // 2 mandatory positional parms = field name    &   typename
                M_vt_raw_ident_nest_pos(                        pln                                                      )           // 1st nested positional parm is fieldname -- raw identifier                     
                M_vt_typdef_nest_pos(                           pln                                                      )           // 2nd nested positional parm is field type -- typdef
                M_vt_int64rc_optional_nest_kw(                  pln, L"offset", 0, std::numeric_limits<int64_t>::max()   )           // optional offset:nnnn keyword  (offset must not be negative)
                M_vt_int64_optional_nest_kw(                    pln, L"skip"                                             )           // optional skip:nnnn   keyword  (skip can be positive or negative)
                M_vt_nval_optional_nest_kw(                     pln, L"same"                                             )           // optional same        keyword  
                M_vt_nval_optional_nest_kw(                     pln, L"high"                                             )           // optional high        keyword 
                M_vt_nest_conflict_3way(                        pln, L"offset", L"same", L"high"                         )           // cannot have offset: high: and same: for same field

                M_vt_vlist_nest_pos(                        pl, pln                                                      )  
            }                                                                                                          
            M_vt_vlist_optional_right_kw( vt, L"struct"   , pl                                                           )
        } 

        {
          M_vt_conflict_set(                 cs                                  )
          M_vt_add_conflict_kw(              cs, L"unit"                         ) 
          M_vt_add_conflict_kw(              cs, L"bool"                         ) 
          M_vt_add_conflict_kw(              cs, L"int8"                         ) 
          M_vt_add_conflict_kw(              cs, L"uint8"                        ) 
          M_vt_add_conflict_kw(              cs, L"int16"                        ) 
          M_vt_add_conflict_kw(              cs, L"uint16"                       ) 
          M_vt_add_conflict_kw(              cs, L"int32"                        )
          M_vt_add_conflict_kw(              cs, L"uint32"                       )
          M_vt_add_conflict_kw(              cs, L"int64"                        )
          M_vt_add_conflict_kw(              cs, L"uint64"                       )
          M_vt_add_conflict_kw(              cs, L"float32"                      )
          M_vt_add_conflict_kw(              cs, L"float64"                      )  
          M_vt_add_conflict_kw(              cs, L"array"                        ) 
          M_vt_add_conflict_kw(              cs, L"struct"                       ) 
          M_vt_add_r_conflict_set(      vt,  cs                                  )  
        }                                                                       
                                                                                
        M_vt_add(L"TYPE"              , vt, verb_type                            ) 
     }   

     

    // ====================================================
    //  @TO_XXXX type conversion verbs
    // ==================================================== 

    {
        M_vt_unary_prefix(      vt                    )
        M_vt_compare_right_pos( vt                    )  

        M_vt_add(L"TO_BOOL"     , vt, verb_to_bool    )
        M_vt_add(L"TO_INT8"     , vt, verb_to_int8    )
        M_vt_add(L"TO_INT16"    , vt, verb_to_int16   )
        M_vt_add(L"TO_INT32"    , vt, verb_to_int32   )
        M_vt_add(L"TO_INT64"    , vt, verb_to_int64   )
        M_vt_add(L"TO_UINT8"    , vt, verb_to_uint8   )
        M_vt_add(L"TO_UINT16"   , vt, verb_to_uint16  )
        M_vt_add(L"TO_UINT32"   , vt, verb_to_uint32  )
        M_vt_add(L"TO_UINT64"   , vt, verb_to_uint64  ) 
        M_vt_add(L"TO_FLOAT32"  , vt, verb_to_float32 ) 
        M_vt_add(L"TO_FLOAT64"  , vt, verb_to_float64 ) 
     }   

     //////////////////////////////////////////////////

     {
        M_vt_unary_prefix(      vt               )
        M_vt_string_right_pos(  vt               )
        M_vt_add(L"TO_IDENT"  , vt, verb_to_ident) 
     }

     ////////////////////////////////////////////////

     {
        M_vt_unary_prefix(   vt)

        {
            M_vt_pos_parm(       pt       )            // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  compare types + vlist  
            pt.unit_ok    = true;
            pt.boolean_ok = true;
            pt.int8_ok    = true;
            pt.int16_ok   = true;
            pt.int32_ok   = true;
            pt.int64_ok   = true;
            pt.uint8_ok   = true;
            pt.uint16_ok  = true;
            pt.uint32_ok  = true;
            pt.uint64_ok  = true;
            pt.float32_ok = true;
            pt.float64_ok = true;
            pt.string_ok  = true;
            pt.vlist_ok   = true;
            M_vt_add_r_pos(  vt, pt       )
        }

        M_vt_add(L"TO_STR" , vt, verb_to_str)
     }    


    // ====================================================
    //  @IS_XXXX type checking verbs
    // ==================================================== 

    {
        M_vt_unary_prefix(      vt                 )

        {                                                 // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_pos_parm(          pt             )
            pt.anything_ok   = true;
            pt.no_eval_vlist = true;                      // no need to evaluate a vlist parm to see what type it is or isn't                 
            M_vt_add_r_pos(     vt, pt             )
        }

        M_vt_add(L"IS_BOOL"      , vt, verb_is_bool       )
        M_vt_add(L"IS_INT8"      , vt, verb_is_int8       )
        M_vt_add(L"IS_INT16"     , vt, verb_is_int16      )
        M_vt_add(L"IS_INT32"     , vt, verb_is_int32      )
        M_vt_add(L"IS_INT64"     , vt, verb_is_int64      )
        M_vt_add(L"IS_UINT8"     , vt, verb_is_uint8      )
        M_vt_add(L"IS_UINT16"    , vt, verb_is_uint16     )
        M_vt_add(L"IS_UINT32"    , vt, verb_is_uint32     )
        M_vt_add(L"IS_UINT64"    , vt, verb_is_uint64     )
        M_vt_add(L"IS_FLOAT32"   , vt, verb_is_float32    )
        M_vt_add(L"IS_FLOAT64"   , vt, verb_is_float64    )
        M_vt_add(L"IS_INT"       , vt, verb_is_int        )
        M_vt_add(L"IS_FLOAT"     , vt, verb_is_float      )
        M_vt_add(L"IS_SIGNED"    , vt, verb_is_signed     )
        M_vt_add(L"IS_UNSIGNED"  , vt, verb_is_unsigned   )
        M_vt_add(L"IS_NUMERIC"   , vt, verb_is_numeric    )
        M_vt_add(L"IS_ATOM"      , vt, verb_is_atom       )
        M_vt_add(L"IS_TRUE"      , vt, verb_is_true       )
        M_vt_add(L"IS_FALSE"     , vt, verb_is_false      )
        M_vt_add(L"IS_STRING"    , vt, verb_is_string     )
        M_vt_add(L"IS_VLIST"     , vt, verb_is_vlist      )
        M_vt_add(L"IS_SLIST"     , vt, verb_is_slist      )
        M_vt_add(L"IS_UNIT"      , vt, verb_is_unit       ) 
     }

     ////////////////////////////////////////////////////////////

     {
        M_vt_unary_prefix(vt)                                                                                                    
        
        {                                             // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_pos_parm(          pt             )
            pt.anything_ok   = true;
            pt.no_eval_vlist = true;                  // no need to evaluate vlists parm to see what type it is or isn't 
            pt.no_eval_vexpr = true;                  // want to leave any vexprs unevaluated, so vexpr parms can be reported
            M_vt_add_r_pos(     vt, pt             )
        }

        M_vt_add(L"IS_ITEM"   , vt, verb_is_vexpr   )  
    }
      
    ////////////////////////////////////////////////////////////

    {
        M_vt_unary_prefix(            vt                             )
        M_vt_any_ident_right_pos(     vt                             )
        M_vt_nval_optional_right_kw ( vt, L"local"                   )
        M_vt_nval_optional_right_kw ( vt, L"global"                  )
        M_vt_nval_optional_right_kw ( vt, L"all"                     ) 
        M_vt_right_conflict_3way(     vt, L"local", L"global", L"all") 

        M_vt_add(L"IS_VAR"     , vt, verb_is_var   ) 
        M_vt_add(L"IS_CONST"   , vt, verb_is_const )
        M_vt_add(L"IS_DEF"     , vt, verb_is_def   )
    }
     

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   structure, array, argument and vlist extraction verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // =============================================================================
    // array # n -- array subscripting  -- return n-th element in array
    // =============================================================================

    {
        M_vt_binary_infix(                vt                     )


        M_vt_int64rc_right_pos(           vt, 0, M_int64_max     )  // right-side integer -- range = 0 to N -- array index

        {
            M_vt_pos_parm(                    pt                 )
            pt.no_eval_ident   = true;                              // don't evaluate, if identifier -- need to get at original symval_s, value_S, and buffer, etc.
            pt.no_eval_ref     = true;                              // don't evaluate, if ref        -- need to get at original symval_s, value_S, and buffer, etc.
            pt.var_ident_ok    = true;                              // can be variable identifier -- must have array assigned to it currently (checked later)
            pt.const_ident_ok  = true;                              // can be constant identifier -- must have array assigned to it currently (checked later)
            pt.array_ok        = true;                              // can be an array value (after evaluation)
            pt.lvalue_ref_ok   = true;                              // can be an reference (should be to array) -- probably from multiple subscript functions
            pt.rvalue_ref_ok   = true;                              // can be an reference (should be to array) -- probably from multiple subscript functions

            M_vt_add_l_pos(               vt, pt                 )  // left positional parm for array source
        }
       
        vt.priority = verb_priority_subscript; 
        M_vt_add(L"#",                    vt, verb_subscript     )
    }
     

    // =============================================================================
    // struct . fieldname -- field selection
    // =============================================================================

    {
        M_vt_binary_infix(                vt                     )


        M_vt_raw_ident_right_pos(         vt                     )  // right-side raw identifier = fieldname

        {
            M_vt_pos_parm(                    pt                 )
            pt.no_eval_ident   = true;                              // don't evaluate, if identifier -- need to get at original symval_s, value_S, and buffer, etc.
            pt.no_eval_ref     = true;                              // don't evaluate, if ref        -- need to get at original symval_s, value_S, and buffer, etc.
            pt.var_ident_ok    = true;                              // can be variable identifier -- must have struct assigned to it currently (checked later)
            pt.const_ident_ok  = true;                              // can be constant identifier -- must have struct assigned to it currently (checked later)
            pt.structure_ok    = true;                              // can be an struct value (after evaluation)
            pt.lvalue_ref_ok   = true;                              // can be an reference (should be to struct) -- probably from multiple subscript functions
            pt.rvalue_ref_ok   = true;                              // can be an reference (should be to struct) -- probably from multiple subscript functions

            M_vt_add_l_pos(               vt, pt                 )  // left positional parm for array source
        }
       
        vt.priority = verb_priority_select; 
        M_vt_add(L".",                    vt, verb_select        )
    }
    

    // ====================================================================================================================
    // @GETENV "name" -- get setting for environment variable "name"
    // ====================================================================================================================

    {
        M_vt_unary_prefix(      vt              )
        M_vt_string_right_pos(  vt              )         
        M_vt_add(L"GETENV",     vt, verb_getenv )
    }
    

    // ====================================================================================================================
    // @ARG_CT left:               -- return total number of left-side  positional arguments
    // @ARG_CT right:              -- return total number of right-side positional arguments  
    // @ARG_CT left:  allkeys:     -- return total number of left-side  keyword arguments
    // @ARG_CT right: allkeys;     -- return total number of right-side keyword arguments     
    // @ARG_CT left:  key:"xxx"    -- return number of times left-side  keyword "xxx" appears
    // @ARG_CT right: key:"xxx"    -- return number of times right-side keyword "xxx" appears
    // ====================================================================================================================

    {
        M_vt_nofix(        vt)

        M_vt_string_optional_right_kw(    vt, L"key"              )
        M_vt_nval_optional_right_kw(      vt, L"allkeys"          )
        M_vt_nval_optional_right_kw(      vt, L"right"            )
        M_vt_nval_optional_right_kw(      vt, L"left"             )  
        M_vt_right_conflict_pair(         vt, L"left", L"right"   )
        M_vt_right_conflict_pair(         vt, L"key" , L"allkeys" ) 
        
        M_vt_add(L"ARG_CT", vt, verb_arg_ct)
    }
   

    // =================================================================
    // @ARGS -- return vlist value with either left: or right: arg vlist
    // =================================================================

    {
        M_vt_nofix(        vt)

        M_vt_nval_optional_right_kw(      vt, L"right"          )
        M_vt_nval_optional_right_kw(      vt, L"left"           )  
        M_vt_right_conflict_pair(         vt, L"left", L"right" )
        
        M_vt_add(L"ARGS", vt, verb_args)
    }
     

    // ====================================================================================================================
    // @ARG n left:               -- return n-th left-side  positional argument
    // @ARG n right:              -- return n-th right-side positional argument  
    // @ARG n left:  allkeys:     -- return n-th left-side  keyword arguments
    // @ARG n right: allkeys;     -- return n-th right-side keyword arguments     
    // @ARG n left:  key:"xxx"    -- return n-th left-side  keyword "xxx" argument
    // @ARG n right: key:"xxx"    -- return n-th right-side keyword "xxx" argument
    // ====================================================================================================================

    {
        M_vt_unary_prefix(                vt                                         ) 
        M_vt_int64rc_right_pos(           vt, 0, std::numeric_limits<int64_t>::max() )    // range = 0 to N
        M_vt_nval_optional_right_kw(      vt, L"allkeys"                             )
        M_vt_string_optional_right_kw(    vt, L"key"                                 )
        M_vt_nval_optional_right_kw(      vt, L"right"                               )
        M_vt_nval_optional_right_kw(      vt, L"left"                                )  
        M_vt_right_conflict_pair(         vt, L"left", L"right"                      ) 
        M_vt_right_conflict_pair(         vt, L"key" , L"allkeys"                    )  

        M_vt_add(L"ARG", vt, verb_arg)
    }


    // =========================================================================================================================================================
    // v1 v2 v3 ... v999 rest:v0 key:"keyname" @ARG_ASSIGN v11 v12 v13 ... v9999 rest:v00 key:"keyname" - assign positional/keyword values in args to  variables
    // =========================================================================================================================================================

    {
        M_vt_anyfix(                          vt, 0, M_int64_max, 0, M_int64_max) 
        M_vt_nonconst_ident_left_pos(         vt                                )
        M_vt_nonconst_ident_optional_left_kw( vt, L"rest"                       )
        M_vt_string_optional_left_kw(         vt, L"key"                        )
     
        M_vt_nonconst_ident_right_pos(        vt                                )
        M_vt_nonconst_ident_optional_right_kw(vt, L"rest"                       )
        M_vt_string_optional_right_kw(        vt, L"key"                        )

        M_vt_add(L"ARG_ASSIGN",               vt, verb_arg_assign               )
    }  
 

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

    // =============================================================================
    // [vlist] @AT n              -- return n-th positional value in vlist
    // [vlist] @AT n allkeys:     -- return n-th keyword value in vlist
    // [vlist] @AT n key:"xxx"    -- return value from n-th occurence of keyword "xxx" vlist      //
    // =============================================================================

    {
        M_vt_binary_infix(                vt                     )
        M_vt_int64rc_right_pos(           vt, 0, M_int64_max     )  // right-side integer -- range = 0 to N
        M_vt_string_optional_right_kw(    vt, L"key"             )
        M_vt_nval_optional_right_kw(      vt, L"allkeys"         )
        M_vt_right_conflict_pair(         vt, L"key" , L"allkeys") 

        M_vt_vlist0_left_pos(             vt                     ) 

        vt.priority = verb_priority_at; 
        M_vt_add(L"AT",                   vt, verb_at            )
    }
     

    // ============================================================================================================
    // @VL_CT [vlist]              - return total number of positional values in vlist
    // @VL_CT [vlist] allkeys:     - return total number of keywords in vlist
    // @VL_CT [vlist] key:"xxx"    - return number occurrences of keyword "xxx" in vlist
    // ============================================================================================================

    {
        M_vt_unary_prefix(                vt                       )
        M_vt_vlist0_right_pos(            vt                       )
        M_vt_string_optional_right_kw(    vt, L"key"               )
        M_vt_nval_optional_right_kw(      vt, L"allkeys"           )
        M_vt_right_conflict_pair(         vt, L"key" , L"allkeys"  )  

        M_vt_add(L"VL_CT",                vt, verb_vl_ct           )
    }
      
    
    // ==================================================================================================================
    // v1 v2 v3 ... v999 rest:v0 @VL_ASSIGN [vlist] key:"string" - assign positional/keyword values in vlist to variables
    // ==================================================================================================================

    {
        M_vt_anyfix(                          vt, 1, M_int64_max, 1, 1)
        M_vt_nonconst_ident_left_pos(         vt                      )
        M_vt_nonconst_ident_optional_left_kw( vt, L"rest"             )
        M_vt_string_optional_right_kw(        vt, L"key"              )
        M_vt_vlist0_right_pos(                vt                      )  

        M_vt_add(L"VL_ASSIGN",                vt, verb_vl_assign      )
    } 
       

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   output-oriented verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 

    // =======================================================
    // @DISPLAY - display various things (no positional parms)
    // =======================================================

    {
        M_vt_nofix(vt) 
        M_vt_nval_optional_right_kw( vt, L"locale"              )
        M_vt_nval_optional_right_kw( vt, L"numerics"            )
        M_vt_nval_optional_right_kw( vt, L"stack"               )
        M_vt_nval_optional_right_kw( vt, L"all_vars"            )
        M_vt_nval_optional_right_kw( vt, L"builtin_verbs"       )
        M_vt_nval_optional_right_kw( vt, L"added_verbs"         )
        M_vt_nval_optional_right_kw( vt, L"all_verbs"           )
        M_vt_nval_optional_right_kw( vt, L"builtin_types"       )
        M_vt_nval_optional_right_kw( vt, L"added_types"         )
        M_vt_nval_optional_right_kw( vt, L"all_types"           )
        M_vt_nval_optional_right_kw( vt, L"id_cache"            )


        // vars:  ['v1 'v2 'v3 'v4 ...] -- display specified variables
        // types: ['v1 'v2 'v3 'v4 ...] -- display specified type definitions
        // verbs: ['v1 'v2 'v3 'v4 ...] -- display specified verb definitions

        {   
            M_vt_nested_plist(                             pl, 1, std::numeric_limits<int64_t>::max()             )           // mandatory positional parm is 1st identifier -- additional ones are optional 
            M_vt_string_nest_pos(                          pl                                                     )           // string with name of type, verb, variable to be displayed (1-N of these)
                                                                                                              
            M_vt_vlist_optional_right_kw( vt,  L"vars"   , pl                                                     )           // vars:  [ ... ] allowed on right side
            M_vt_vlist_optional_right_kw( vt,  L"types"  , pl                                                     )           // types: [ ... ] allowed on right side
            M_vt_vlist_optional_right_kw( vt,  L"verbs"  , pl                                                     )           // verbs: [ ... ] allowed on right side
        }  

        M_vt_add(L"DISPLAY", vt, verb_display)
    }    

  
    // ==========================
    // @SAY value value value ...
    // @STR value value value ...
    // ==========================

    {
        M_vt_nary_prefix(             vt                        )      
        M_vt_any_right_pos(           vt                        )      
        M_vt_nval_optional_right_kw(  vt, L"debug"              )
        M_vt_nval_optional_right_kw(  vt, L"debugx"             )
        M_vt_right_conflict_pair(     vt, L"debug" , L"debugx"  ) 

        M_vt_add(L"STR",              vt, verb_str              );        
    }

    {
        M_vt_nary_prefix(             vt                        )      
        M_vt_any_right_pos(           vt                        )     
        M_vt_nval_optional_right_kw(  vt, L"debug"              )
        M_vt_nval_optional_right_kw(  vt, L"debugx"             )
        M_vt_right_conflict_pair(     vt, L"debug" , L"debugx"  ) 
        M_vt_nval_optional_right_kw(  vt, L"no_nl"              )

        M_vt_add(L"SAY",              vt, verb_say              );       
    }


    // ==========================
    // @STDIN  (no parms)
    // @STDOUT string-parm
    // @STDERR string-parm
    // ==========================

    {
        M_vt_nofix(                       vt                     )
        M_vt_add(L"STDIN",                vt, verb_stdin         );
    }

    {
        M_vt_unary_prefix(                vt                     )
        M_vt_string_right_pos(            vt                     )
     
        M_vt_add(L"STDOUT",               vt, verb_stdout        );
    }

    {
        M_vt_unary_prefix(                vt                     )
        M_vt_string_right_pos(            vt                     )
     
        M_vt_add(L"STDERR",               vt, verb_stderr        );
    }




    // =======================================
    // @DEBUG - set logging and debug settings 
    // =======================================

    {
        M_vt_nofix(vt) 
        M_vt_nval_optional_right_kw(  vt, L"no_verbs"              )
        M_vt_nval_optional_right_kw(  vt, L"few_verbs"             )
        M_vt_nval_optional_right_kw(  vt, L"some_verbs"            )
        M_vt_nval_optional_right_kw(  vt, L"most_verbs"            )
        M_vt_nval_optional_right_kw(  vt, L"all_verbs"             )   

        M_vt_right_conflict_5way(     vt, L"no_verbs" , L"few_verbs",  L"some_verbs" , L"most_verbs", L"all_verbs")  

        M_vt_add(L"DEBUG", vt, verb_debug)
    } 







    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return; 
}
M_endf



//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""