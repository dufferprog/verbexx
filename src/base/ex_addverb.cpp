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
////   add_predefined_typdefs() -- add all pre-defined typedefs to global environment 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void add_predefined_typdefs() try
{
    def_parm_S parm { };
    parm.builtin   = true;  

    def_global_typdef(L"INT8_T"       , make_atomic_typdef(type_E::int8       ), parm);
    def_global_typdef(L"INT16_T"      , make_atomic_typdef(type_E::int16      ), parm);
    def_global_typdef(L"INT32_T"      , make_atomic_typdef(type_E::int32      ), parm);
    def_global_typdef(L"INT64_T"      , make_atomic_typdef(type_E::int64      ), parm);
    def_global_typdef(L"INT_T"        , make_atomic_typdef(type_E::int64      ), parm);
    def_global_typdef(L"UINT8_T"      , make_atomic_typdef(type_E::uint8      ), parm);
    def_global_typdef(L"UINT16_T"     , make_atomic_typdef(type_E::uint16     ), parm);
    def_global_typdef(L"UINT32_T"     , make_atomic_typdef(type_E::uint32     ), parm);
    def_global_typdef(L"UINT64_T"     , make_atomic_typdef(type_E::uint64     ), parm);
    def_global_typdef(L"UINT_T"       , make_atomic_typdef(type_E::uint64     ), parm);
    def_global_typdef(L"FLOAT32_T"    , make_atomic_typdef(type_E::float32    ), parm);
    def_global_typdef(L"FLOAT64_T"    , make_atomic_typdef(type_E::float64    ), parm);
    def_global_typdef(L"FLOAT_T"      , make_atomic_typdef(type_E::float64    ), parm);
                                                                                
    def_global_typdef(L"UNIT_T"       , make_atomic_typdef(type_E::unit       ), parm);
    def_global_typdef(L"BOOL_T"       , make_atomic_typdef(type_E::boolean    ), parm);
    def_global_typdef(L"STRING_T"     , make_atomic_typdef(type_E::string     ), parm);
    def_global_typdef(L"VERBNAME_T"   , make_atomic_typdef(type_E::verbname   ), parm);
    def_global_typdef(L"KEYNAME_T"    , make_atomic_typdef(type_E::keyname    ), parm);
    def_global_typdef(L"IDENTIFIER_T" , make_atomic_typdef(type_E::identifier ), parm);
    def_global_typdef(L"VLIST_T"      , make_atomic_typdef(type_E::vlist      ), parm);
    def_global_typdef(L"EXPRESSION_T" , make_atomic_typdef(type_E::expression ), parm);
    def_global_typdef(L"BLOCK_T"      , make_atomic_typdef(type_E::block      ), parm);
    def_global_typdef(L"VERBSET_T"    , make_atomic_typdef(type_E::verbset    ), parm);
    def_global_typdef(L"TYPEDEF_T"    , make_atomic_typdef(type_E::typdef     ), parm);
    def_global_typdef(L"REF_T"        , make_atomic_typdef(type_E::ref        ), parm);
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_predefined_constants() -- add all pre-defined constants to global environment 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void add_predefined_constants() try
{
    // add global constants 

    def_parm_S parm { };
    parm.constant  = true;
    parm.builtin   = true;
    parm.no_shadow = true;                  // don't allow TRUE and FALSE to be shadowed

    def_global_const(L"UNIT"                , unit_val(         ), parm );
    def_global_const(L"TRUE"                , boolean_val(true  ), parm );
    def_global_const(L"FALSE"               , boolean_val(false ), parm );
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_predefined_verbs() -- add all pre-defined general verbs to global environment 
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
    //  v1 v2 v3 ... -> { function body }          // simplified version of @FN for lambdas
    // info:"string"
    // init:
    // { 
    //    initialization block --
    //    run when verb is defined
    // };
    //             -- no function name (lambda only) 
    //             -- no left side parms   
    //             -- no keyword parms
    //             -- no optional parms (number of right-side or left-side positional parms = number of arg variables in list
    //             -- no parmtype info
    //
    //
    //  sample invocation:
    //  -----------------
    //
    //   @VAR vf = a b c ->              // define lambda
    //   {
    //       @SAY «lambda called»;
    //       a a b b c c;
    //   }
    //   dynamic_scope: noclose:   
    //      
    //
    //
    // note: if no scope specification is present, it defaults to "lexical scope" if the enclosing scope is still around, or "no scope" if the enclosing scope has disappeared
    // note: close: is the default, unlike with @VERB and @FN
    //
    // =============================================================================================================================================================

    {    
        M_vt_anyfix(                              vt, 0, M_int64_max, 1, 1                                    )            // 0-N left positional parms      /    1 right positional parm 


        // left positional parms -- (optional) -- raw identifiers (argvar names)

        M_vt_raw_ident_left_pos(                  vt                                                          )            // must be raw identifiers (not evaluated)


        // 1st right positional parm is required -- must be code block

        M_vt_block_right_pos(                      vt                                                         )           // required -- { main executable code for this verb }        


        // optional init: or init:{ ...} right keyword parm --= causes allocation of persistetn environment at verb definition time 

        {
            M_vt_optional_kw_parm(                     kp                                                     )           // optional   init:{ ...  }   or   init:  right keyword parm
            kp.nval_ok  = true;                                                                                           // allow init:  
            kp.block_ok = true;                                                                                           // allow init:{ ... }
            M_vt_add_r_kw(             L"init",    vt, kp                                                     )
        }

        M_vt_string_optional_right_kw(             vt, L"info"                                                )

        M_vt_nval_optional_right_kw (              vt, L"close"                                               )
        M_vt_nval_optional_right_kw (              vt, L"noclose"                                             )
        M_vt_nval_optional_right_kw (              vt, L"by_alias"                                            )            // enable by-alias processing
      
        M_vt_int64rc_optional_right_kw (           vt, L"priority"   , (M_int64_min + 1) , M_int64_max        )            // don't collide with "not specified" value
        M_vt_nval_optional_right_kw (              vt, L"left_associate"                                      )
        M_vt_nval_optional_right_kw (              vt, L"right_associate"                                     )

        M_vt_right_conflict_pair(                  vt, L"close" , L"noclose"                                  )
        M_vt_right_conflict_pair(                  vt, L"left_associate" , L"right_associate"                 )

        M_vt_custom_eval(                          vt                                                         ) 
        M_vt_info(                                 vt, L"arg arg ... -> {code} |init:{code}| keywords: ..."   )
        M_vt_add(L"->",                            vt, verb_lambda                                            )
    }   






    // =============================================================================================================================================================
    //
    // ident [v1 v2 v3 ...] @FN [v4 v5 v6 ...] info:"string"          // simplified version of @VERB
    // {
    //    function body ...
    // } 
    // init:
    // { 
    //    initialization block --
    //    run when verb is defined
    // };
    //             -- no keyword parms
    //             -- no optional parms (number of right-side or left-side positional parms = number of arg variables in list
    //             -- no parmtype info
    //
    //
    //  sample invocations:
    //  ------------------
    //
    //   verb1 @FN [a b c]  dynamic_scope:            // define named function
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
    //
    // note: if no scope specification is present, it defaults to "lexical scope" if the enclosing scope is still around, or "no scope" if the enclosing scope has disappeared
    //
    //
    // =============================================================================================================================================================

    {    
        M_vt_anyfix(                              vt, 0, 2, 1, 2                                                  )      // 0, 1, or 2 left positional parms      /    1, or 2 right positional parms 


        // 1st left positional parm -- (optional) left positional parm -- undefined identifier, or string -- verb name, -or- vlist with arg var names

        {   
            M_vt_nested_plist(                             pln, 0, M_int64_max                                     )      // positional parm can be list of 0-N variable names  
            M_vt_raw_ident_nest_pos(                       pln                                                     )      // must be raw identifiers (not evaluated)

            M_vt_pos_parm(                            pt                                                           )                  
            pt.undef_ident_ok      = true;                                                                               // undefind identifier is OK -- new verbset will be crested    
            pt.verbset_ident_ok    = true;                                                                               // already-defined verbset identifier OK -- add new verb to existing verbset
            pt.eval.no_eval_ident  = true;                                                                               // don't evaluate identifiers, since they might be undefined -- do evaluate expressions, etc.
            pt.string_ok           = true;                                                                               // string is OK for verbname
            pt.vlist_ok            = true;                                                                               // 1st parm can be a vlist 
            M_vt_add_nested_plist(                    pt,  pln                                                     )     // nested parm list for vlist 
            M_vt_add_l_pos(                       vt, pt                                                           )         
        }


        // 2nd left positional parm -- (optional) left positional parm -- vlist with arg var names

        {   
            M_vt_nested_plist(                             pln, 0, M_int64_max                                     )      // positional parm can be list of 0-N variable names  
            M_vt_raw_ident_nest_pos(                       pln                                                     )      // must be raw identifiers (not evaluated)

            M_vt_pos_parm(                            pt                                                           )                  
            pt.vlist_ok = true;                                                                                          // 1st parm can be a vlist 
            M_vt_add_nested_plist(                    pt,  pln                                                     )     // nested parm list for vlist 
            M_vt_add_l_pos(                       vt, pt                                                           )         
        }
        

        // 1st right positional parm -- required: can be -- vlist with argument variable names -or- code block  

        {                                                                                                                // right-side positional parm -- vlist with parms  
            M_vt_nested_plist(                             pln, 0, M_int64_max                                     )     // positional parm can be list of 0-N variable names  
            M_vt_raw_ident_nest_pos(                       pln                                                     )     // must be raw identifiers (not evaluated)

            M_vt_pos_parm(                             pt                                                          )            
            pt.vlist_ok = true;                                                                                          // 1st parm can be a vlist 
            pt.block_ok = true;                                                                                          // 1st parm can also be a block 
            M_vt_add_nested_plist(                     pt, pln                                                     )     // nested parm list for vlist 
            M_vt_add_r_pos(                        vt, pt                                                          )     
        }   


        // 2nd right parm is optional -- if present, it must be a block

        M_vt_block_right_pos(                      vt                                                         )           // optional -- { main executable code for this verb }        


        // optional init: or init:{ ...} right keyword parm --= causes allocation of persistetn environment at verb definition time 

        {
            M_vt_optional_kw_parm(                     kp                                                     )           // optional   init:{ ...  }   or   init:  right keyword parm
            kp.nval_ok  = true;                                                                                           // allow init:  
            kp.block_ok = true;                                                                                           // allow init:{ ... }
            M_vt_add_r_kw(             L"init",    vt, kp                                                     )
        }

        M_vt_string_optional_right_kw(             vt, L"info"                                                )
        M_vt_nval_optional_right_kw (              vt, L"global"                                              )
        M_vt_nval_optional_right_kw (              vt, L"local"                                               )
        M_vt_nval_optional_right_kw (              vt, L"verbmain"                                            )
        M_vt_nval_optional_right_kw (              vt, L"static"                                              )
#ifdef M_EXPOSE_SUPPORT
        M_vt_nval_optional_right_kw (              vt, L"expose"                                              )
#endif
        M_vt_nval_optional_right_kw (              vt, L"dynamic_scope"                                       )
        M_vt_nval_optional_right_kw (              vt, L"lexical_scope"                                       )
#ifdef M_EXPOSE_SUPPORT
        M_vt_nval_optional_right_kw (              vt, L"dynall_scope"                                        )
#endif
        M_vt_nval_optional_right_kw (              vt, L"same_scope"                                          )
        M_vt_nval_optional_right_kw (              vt, L"no_scope"                                            )
        M_vt_nval_optional_right_kw (              vt, L"close"                                               )
        M_vt_nval_optional_right_kw (              vt, L"noclose"                                             )
        M_vt_nval_optional_right_kw (              vt, L"by_alias"                                            )            // enable by-alias processing
      
        M_vt_int64rc_optional_right_kw (           vt, L"priority"   , (M_int64_min + 1) , M_int64_max        )            // don't collide with "not specified" value
        M_vt_nval_optional_right_kw (              vt, L"left_associate"                                      )
        M_vt_nval_optional_right_kw (              vt, L"right_associate"                                     )

        M_vt_right_conflict_pair(                  vt, L"close" , L"noclose"                                  )
        M_vt_right_conflict_pair(                  vt, L"left_associate" , L"right_associate"                 )

        M_vt_right_conflict_4way(                  vt, L"global", L"local", L"verbmain", L"static"            )
        M_vt_right_conflict_pair(                  vt, L"close" , L"dynamic_scope"                            )
#ifdef M_EXPOSE_SUPPORT
        M_vt_right_conflict_pair(                  vt, L"close" , L"dynall_scope"                             )
#endif
        M_vt_right_conflict_pair(                  vt, L"close" , L"no_scope"                                 )
        M_vt_right_conflict_pair(                  vt, L"close" , L"same_scope"                               )
        M_vt_right_conflict_pair(                  vt, L"init"  , L"same_scope"                               )
#ifdef M_EXPOSE_SUPPORT
        M_vt_right_conflict_5way(                  vt, L"dynamic_scope", L"lexical_scope", L"dynall_scope", L"no_scope", L"same_scope" )
#else
        M_vt_right_conflict_4way(                  vt, L"dynamic_scope", L"lexical_scope",                  L"no_scope", L"same_scope" )
#endif
        M_vt_custom_eval(                          vt                                                         ) 
        M_vt_info(                                 vt, L"|name| @FN [arg arg ...] {code} |init:{code}| keywords: ..."  )
        M_vt_add(L"FN",                            vt, verb_fn                                                )
    }   



    // =============================================================================================================================================================
    //
    // left-side stuff  @VERB  name:"string"   right-side stuff   block:{block} -- define non-builtin verb
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
    //         code that is run each time
    //         verb is called          
    //    }
    //    init:
    //    { 
    //       initialization block --
    //       run when verb is defined
    //    };
    //  
    //
    //
    // sample (complex) invocation:
    // ---------------------------
    //
    //
    //   [int64: float64: string:]
    //   [float64: float32: float_min:0.0 float_max:100.0]                                                              // this one will be used for 1st left-side positional parm
    //   [vlist: block: verbdef: int8: int16: int32: int64: uint8: uint16: uint32: uint64: float32: float64:]           // this one will be used for 3rd, 4th, 5th ... left-side positional positional parms
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
    //   info:"descriptive string"                                                                                      // (optional) string for error messages        
    //
    //   local:                                                                                                         // put in local scope 
    //   dynamic_scope:                                                                                                 // use dynamic upward scoping
    //   block:
    //   {
    //       code that is run each time
    //       verb is called         
    //   }
    //   init:
    //   { 
    //       initialization block --
    //       run when verb is defined
    //   };

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
            M_vt_nval_optional_nest_kw(                pl, L"block"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"verbset"                                         )
            M_vt_nval_optional_nest_kw(                pl, L"typedef"                                         )
            M_vt_nval_optional_nest_kw(                pl, L"ref"                                             )
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_ident"                                   )  
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_expression"                              )
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_vlist"                                   ) 
            M_vt_nval_optional_nest_kw(                pl, L"no_eval_ref"                                     ) 
            M_vt_float64_optional_nest_kw(             pl, L"float_min"                                       )
            M_vt_float64_optional_nest_kw(             pl, L"float_max"                                       )
            M_vt_int64_optional_nest_kw(               pl, L"int_min"                                         )
            M_vt_int64_optional_nest_kw(               pl, L"int_max"                                         )
                                                                                                            
            M_vt_nest_match_pair(                      pl, L"float_min", L"float_max"                         )   
            M_vt_nest_match_pair(                      pl, L"int_min"  , L"int_max"                           ) 
                                                                                                            
            M_vt_vlist_left_pos(                   vt, pl                                                     )        // [ unit: int64: ... ] [ block: int64: ... ] ...   allowed on left  side
            M_vt_vlist_right_pos(                  vt, pl                                                     )        // [ unit: int64: ... ] [ block: int64: ... ] ...   allowed on right side
        }                                                                                                   
                                                                                                            
        // unnested keyword parms -- left-side and right-side  
        //
        // note: if no scope specification is present, it defaults to "lexical scope" if the enclosing scope is still around, or "no scope" if the enclosing scope has disappeared
                                                                                                            
        M_vt_string_optional_right_kw(             vt, L"name"                                                )
        M_vt_string_optional_right_kw(             vt, L"info"                                                )
        {
            M_vt_optional_kw_parm(                     kp                                                     )      // optional   init:{ ...  }   or   init:  right keyword parm
            kp.nval_ok  = true;                                                                                      // allow init:  
            kp.block_ok = true;                                                                                      // allow init:{ ... }
            M_vt_add_r_kw(             L"init",    vt, kp                                                     )
        }
        M_vt_block_required_right_kw(              vt, L"block"                                               )      // function body 
        M_vt_block_optional_right_kw(              vt, L"init"                                                )      // optional initialization block 
        M_vt_nval_optional_right_kw (              vt, L"global"                                              )
        M_vt_nval_optional_right_kw (              vt, L"local"                                               )
        M_vt_nval_optional_right_kw (              vt, L"verbmain"                                            )
        M_vt_nval_optional_right_kw (              vt, L"static"                                              )
#ifdef M_EXPOSE_SUPPORT
        M_vt_nval_optional_right_kw (              vt, L"expose"                                              )
#endif
        M_vt_nval_optional_right_kw (              vt, L"dynamic_scope"                                       )
        M_vt_nval_optional_right_kw (              vt, L"lexical_scope"                                       )
#ifdef M_EXPOSE_SUPPORT
        M_vt_nval_optional_right_kw (              vt, L"dynall_scope"                                        )
#endif
        M_vt_nval_optional_right_kw (              vt, L"same_scope"                                          )
        M_vt_nval_optional_right_kw (              vt, L"no_scope"                                            )
        M_vt_nval_optional_right_kw (              vt, L"close"                                               )
        M_vt_nval_optional_right_kw (              vt, L"noclose"                                             )
        M_vt_nval_optional_right_kw (              vt, L"by_alias"                                            )      // enable by-alias processing
                                                                    
        M_vt_int64rc_optional_right_kw (           vt, L"priority"   , (M_int64_min + 1) , M_int64_max        )      // don't collide with "not specified" value
        M_vt_nval_optional_right_kw (              vt, L"left_associate"                                      )
        M_vt_nval_optional_right_kw (              vt, L"right_associate"                                     )

        M_vt_int64rc_optional_right_kw(            vt, L"min", 0, M_int64_max                                 )      // min/max numbers of positional parms          
        M_vt_int64rc_optional_right_kw(            vt, L"max",-1, M_int64_max                                 ) 
        M_vt_int64rc_optional_left_kw(             vt, L"min", 0, M_int64_max                                 )                
        M_vt_int64rc_optional_left_kw(             vt, L"max",-1, M_int64_max                                 )
        
        M_vt_right_conflict_pair(                  vt, L"close" , L"noclose"                                  )
        M_vt_right_conflict_pair(                  vt, L"left_associate" , L"right_associate"                 )
        M_vt_right_match_pair(                     vt, L"min"   , L"max"                                      )   
        M_vt_left_match_pair(                      vt, L"min"   , L"max"                                      ) 
        M_vt_right_conflict_pair(                  vt, L"close" , L"dynamic_scope"                            )
#ifdef M_EXPOSE_SUPPORT
        M_vt_right_conflict_pair(                  vt, L"close" , L"dynall_scope"                             )
#endif
        M_vt_right_conflict_pair(                  vt, L"close" , L"no_scope"                                 )
        M_vt_right_conflict_pair(                  vt, L"close" , L"same_scope"                               )
        M_vt_right_conflict_pair(                  vt, L"init"  , L"same_scope"                               )
        M_vt_right_conflict_4way(                  vt, L"global", L"local", L"verbmain", L"static"            )
#ifdef M_EXPOSE_SUPPORT
        M_vt_right_conflict_5way(                  vt, L"dynamic_scope", L"lexical_scope", L"dynall_scope", L"no_scope", L"same_scope" )
#else
        M_vt_right_conflict_4way(                  vt, L"dynamic_scope", L"lexical_scope",                  L"no_scope", L"same_scope" )
#endif


        // key: ["kw-name" kw_min: nnn   kw_max: nnn   int64:  float64:  unit: ...etc....   ] -- keyword parm, with nested vlist  (kw_name required, int64: float64: are one or more accepted types for this keyword)
        //      note: can't declare types, limits, etc., for parms inside of any nested vlists  
        //      note: there can be multiple occurrences of these keyword parm descriptions on both sides 

        {   
            M_vt_nested_plist(                         pl, 1, 1                                               )    // mandatory positional parm is the keyword name 
            M_vt_raw_ident_nest_pos(                   pl                                                     )    // keyword name (as "raw"identifier)
            M_vt_int64rc_optional_nest_kw(             pl, L"kw_min", 0 , M_int64_max                         )    // ???
            M_vt_int64rc_optional_nest_kw(             pl, L"kw_max", -1, M_int64_max                         )    // ???
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
            M_vt_nval_optional_nest_kw(                pl, L"block"                                           )
            M_vt_nval_optional_nest_kw(                pl, L"verbset"                                         ) 
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

            M_vt_vlist_optional_left_kw(  vt,  L"var", pl                                                     )           // var:[ ... ] allowed on left  side -- positional parameter names
            M_vt_vlist_optional_right_kw( vt,  L"var", pl                                                     )           // var:[ ... ] allowed on right side -- positional parameter names
        }    

        M_vt_custom_eval(                          vt                                                         )  
        M_vt_info(                                 vt, L"... parms ... @VERB |name:«name»|   ... parms ...  block:{code} |init:{code}|   keywords: ..."   )              
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
        M_vt_nval_optional_right_kw (     vt, L"verbmain"           )
        M_vt_nval_optional_right_kw (     vt, L"static"             )
        M_vt_right_conflict_4way(         vt, L"global", L"local", L"verbmain", L"static" )

        M_vt_info(                        vt, L"@UNVERB 'name 'name ..."          )
        M_vt_add(L"UNVERB",               vt, verb_unverb           )
    }   


    // ====================================
    // @INITVERB  "string" 
    // ====================================
   
    {
        M_vt_unary_prefix(                vt                        )
        M_vt_string_right_pos(            vt                        ) 
        M_vt_nval_optional_right_kw (     vt, L"global"             )
        M_vt_nval_optional_right_kw (     vt, L"local"              )
        M_vt_nval_optional_right_kw (     vt, L"verbmain"           )
        M_vt_nval_optional_right_kw (     vt, L"static"             )
        M_vt_right_conflict_4way(         vt, L"global", L"local", L"verbmain", L"static" )

        M_vt_info(                        vt, L"@INITVERB 'name"    )
        M_vt_add(L"INITVERB",             vt, verb_initverb         )
    }   
        

    // ===================================================================================================
    // @INCLUDE file:"name of file"   -or   @INCLUDE string:"text of string to be included" id:"debug ID"  (preprocessor only)
    // ===================================================================================================
   
    {
        M_vt_nofix(                         vt                        )  
        M_vt_string_optional_right_kw(      vt, L"string"             )                      // optional string to include
        M_vt_string_optional_right_kw(      vt, L"file"               )                      // optional file to include
        M_vt_string_optional_right_kw(      vt, L"id"                 )                      // debug ID for included string
        M_vt_right_choice_pair(             vt, L"string", L"file"    )                      // must have one or the other
        M_vt_right_conflict_pair(           vt, L"file", L"id"        )                      // can't have both id: and file:

        M_vt_info(                          vt, L"@INCLUDE  file:'path | string:\"string text\" <id:\"debug ID\">" )
        M_vt_add(L"INCLUDE",                vt, verb_include          )
    }   


    // ================================================================================================
    // @SKIPTO "target label"  -- skip to next pre-processor block containing this label                 (preprocessor only)
    // ================================================================================================
   
    {
        M_vt_unary_prefix(                  vt                        ) 
        M_vt_string_right_pos(              vt                        )

        M_vt_info(                          vt, L"@SKIPTO 'label"     )
        M_vt_add(L"SKIPTO",                 vt, verb_skipto           )
    }   
                                                           
   
    // ================================================================================================
    // @SKIP   -- skip to end of pre-processor text block                                                (preprocessor only) 
    // ================================================================================================
   
    {
        M_vt_nofix(                         vt                        ) 
        M_vt_info(                          vt, L"@SKIP"              )
        M_vt_add(L"SKIP",                   vt, verb_skip             )
    }   


    
    // ================================================================================================
    // @IMPORT "name of DLL" 
    // ================================================================================================
   
    {
        M_vt_unary_prefix(                  vt                        ) 
        M_vt_info(                          vt, L"@IMPORT 'dll_name"  )
        M_vt_add(L"IMPORT",                 vt, verb_import           )
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
        M_vt_info(                          vt, L"@PARSE «string text»  < name:«string ID» >"  )
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
        
        M_vt_info(                          vt, L"@CALL 'verb_name  < left:[left parms] >  < right:[right parms] >"  )
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

        M_vt_info(                    vt, L"@XCTL  'verb_name  < left:[left parms] >  < right:[right parms] >"  )
        M_vt_add(L"XCTL",             vt, verb_xctl   )
    }
    



    // ===============================================================
    // @SHELL "cmd string"   capture: 
    // ===============================================================
   
    {
        M_vt_unary_prefix(            vt              )  
        M_vt_nval_optional_right_kw(  vt, L"capture"  )                      // optional capture:   (to capture shell command output 
                                                            
        M_vt_string_right_pos(        vt              )                      // shell command string -- required

        M_vt_info(                    vt, L"@SHELL «shell command»  < capture: >"  )
        M_vt_add(L"SHELL"       ,     vt, verb_shell  )
    }  


#if 0 
    // =================================
    // , (@SEPARATE)  separator operator -- eval args   
    // =================================

    {
        M_vt_nary_anyfix(             vt             ) 

        {
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                       // want to allow all parms (evaluated)
            M_vt_add_l_pos(           vt, pt)
        }   

        {                             
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                        // want to allow all parms (evaluated)
            M_vt_add_r_pos(           vt, pt)    
        } 


        {                                                                                                // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_multi_kw_parm(kp, 0, std::numeric_limits<int64_t>::max() );
            kp.anything_ok             = true; 
  
            M_vt_add_r_kw(L"",        vt, kp         )                                                   // both left side and right side nameless KWs 
            M_vt_add_l_kw(L"",        vt, kp         ) 
        }
            
        vt.d.lparms.no_check_keyword_names  = true;                                                      // want to allow any keyword names
        vt.d.rparms.no_check_keyword_names  = true;                                                      // want to allow any keyword names

        M_vt_custom_eval(  vt                        )
        M_vt_priority(     vt, verb_pri_N::_separate )    
        M_vt_info(         vt, L"< any parms > , < any parms >"  )
        M_vt_add(L",",     vt, verb_separate         )
    }
#endif

    
    // ==================================
    // , (@SEPARATE)  separator operator -- no eval args    
    // ==================================

    {
        M_vt_nary_anyfix(             vt             ) 

        {
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                       // want to pass through all parms verbatim (unevaluated)
            pt.eval.no_eval_ident         = true; 
            pt.eval.no_eval_expression    = true; 
            pt.eval.no_eval_vlist         = true; 
            pt.eval.no_eval_ref           = true; 
            M_vt_add_l_pos(           vt, pt)
        }   

        {                             
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                        // want to pass through all parms verbatim (unevaluated)
            pt.eval.no_eval_ident         = true; 
            pt.eval.no_eval_expression    = true; 
            pt.eval.no_eval_vlist         = true; 
            pt.eval.no_eval_ref           = true; 
            M_vt_add_r_pos(           vt, pt)    
        } 


        {                                                                                                // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_multi_kw_parm(kp, 0, std::numeric_limits<int64_t>::max() );
            kp.anything_ok             = true; 
            kp.eval.no_eval_ident      = true;
            kp.eval.no_eval_expression = true;
            kp.eval.no_eval_vlist      = true;
            kp.eval.no_eval_ref        = true;
 
            M_vt_add_r_kw(L"",        vt, kp         )                                                   // both left side and right side nameless KWs 
            M_vt_add_l_kw(L"",        vt, kp         ) 
        }
            
        vt.d.lparms.no_check_keyword_names  = true;                                                      // want to allow any keyword names
        vt.d.rparms.no_check_keyword_names  = true;                                                      // want to allow any keyword names

        M_vt_custom_eval(  vt                        )
        M_vt_priority(     vt, verb_pri_N::_separate )    
        M_vt_info(         vt, L"< any parms > , < any parms >"  )
        M_vt_add(L","   ,  vt, verb_separate         )
    }


#if 0 
    // ======================================
    // \ (@SEPARATE)  half-separator operator -- no eval args -- left-side args not allowed -- no keywords allowed   
    // ======================================

    {
        M_vt_nary_prefix(             vt             ) 

        {                             
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                        // want to pass through all parms verbatim (unevaluated)
            pt.eval.no_eval_ident         = true; 
            pt.eval.no_eval_expression    = true; 
            pt.eval.no_eval_vlist         = true; 
            pt.eval.no_eval_ref           = true; 
            M_vt_add_r_pos(           vt, pt)    
        } 

        M_vt_custom_eval(  vt                        )
        M_vt_priority(     vt, verb_pri_N::_separate )    
        M_vt_info(         vt, L"\\ parm1 parm2 parm3 ..."  )
        M_vt_add(L"\\"  ,  vt, verb_separate         )
    }
 #endif



 #if 0 
    // =================================
    // ,, (@SEQUENCE)  sequence operator -- eval args on right side (eval on left_side, too)    
    // =================================

    {
        M_vt_nary_anyfix(             vt             ) 

        {
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                       // left side -- want to accept all parms (evaluated), before discarding the results
            M_vt_add_l_pos(           vt, pt)
        }   

        {                             
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                        // right side -- want to pass through all parms verbatim (unevaluated) -- like @SEPARATE
            M_vt_add_r_pos(           vt, pt)    
        } 


        {                                                                                                // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_multi_kw_parm(kp, 0, std::numeric_limits<int64_t>::max() );
            kp.anything_ok             = true;             
            M_vt_add_l_kw(L"",        vt, kp         )                                                   // left side -- accept all keywords (evaluated)
        }
            
        vt.d.lparms.no_check_keyword_names  = true;                                                      // want to allow any keyword names on left side 

        {                                                                                                // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_multi_kw_parm(kp, 0, std::numeric_limits<int64_t>::max() );
            kp.anything_ok             = true; 
            M_vt_add_r_kw(L"",        vt, kp         )                                                   // right side -- accept all keywords (unevaluated) -- these will be passed through         
        }

        vt.d.rparms.no_check_keyword_names  = true;                                                      // want to allow any keyword names on rignt side

        M_vt_custom_eval(  vt                        )
        M_vt_priority(     vt, verb_pri_N::_sequence )    
        M_vt_info(         vt, L"< any parms > ,, < any parms >"  )
        M_vt_add(L",," ,   vt, verb_sequence         )
    }
#endif 
    
    // ==================================
    // ,, (@SEQUENCE)  sequence operator -- no-eval args on right side (eval on left_side)     
    // ==================================

    {
        M_vt_nary_anyfix(             vt             ) 

        {
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                        // left side -- want to accept all parms (evaluated), before discarding the results
            M_vt_add_l_pos(           vt, pt)
        }   

        {                             
            M_vt_pos_parm(                pt)                  
            pt.anything_ok                = true;                                                        // right side -- want to pass through all parms verbatim (unevaluated) -- like @SEPARATE
            pt.eval.no_eval_ident         = true; 
            pt.eval.no_eval_expression    = true; 
            pt.eval.no_eval_vlist         = true; 
            pt.eval.no_eval_ref           = true; 
            M_vt_add_r_pos(           vt, pt)    
        } 


        {                                                                                                // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_multi_kw_parm(kp, 0, std::numeric_limits<int64_t>::max() );
            kp.anything_ok             = true; 
           
            M_vt_add_l_kw(L"",        vt, kp         )                                                   // left side -- accept all keywords (evaluated)
        }
            
        vt.d.lparms.no_check_keyword_names  = true;                                                      // want to allow any keyword names on left side 

        {                                                                                                // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_multi_kw_parm(kp, 0, std::numeric_limits<int64_t>::max() );
            kp.anything_ok             = true; 
            kp.eval.no_eval_ident      = true;
            kp.eval.no_eval_expression = true;
            kp.eval.no_eval_vlist      = true;
            kp.eval.no_eval_ref        = true;
 
            M_vt_add_r_kw(L"",        vt, kp         )                                                   // right side -- accept all keywords (unevaluated) -- these will be passed through         
        }

        vt.d.rparms.no_check_keyword_names  = true;                                                      // want to allow any keyword names on rignt side

        M_vt_custom_eval(  vt                        )
        M_vt_priority(     vt, verb_pri_N::_sequence )    
        M_vt_info(         vt, L"< any parms > ,,\\ < any parms >"  )
        M_vt_add(L",,"   , vt, verb_sequence         )
    }





    // ===========================================================
    // @CONTINUE   -- end @LOOP block evaluation -- keep looping (if in loop)
    // @END        -- end all evaluation (main block), if in evaluation phase   --OR--   end parsing/proprocessing, if in preprocessor phase 
    // @RETHROW    -- do @THROW with current frameblock 1st positional parm 
    // @ERROR      -- increment error count
    // @EXIT       -- do exit() quick_exit(), _Exit(), exit(), abort(), terminate(), etc.
    // @BREAK      -- end @LOOP block evaluation -- stop looping (if in loop)
    // @QUIT       -- end innermost @BLOCK block (frameblock) evaluation  
    // @LEAVE      -- end innermost block evaluation (or innermost block that has matching block label) 
    // =============================================================

    {
        M_vt_nofix(vt) 

        M_vt_info(                  vt, L"@CONTINUE"        )    // does continue need a value, in case loop ends -- or does last executed block have the value
        M_vt_add(L"CONTINUE"      , vt, verb_continue       )

        M_vt_info(                  vt, L"@RETHROW"         )
        M_vt_add(L"RETHROW"       , vt, verb_rethrow        )
 
        M_vt_info(                  vt, L"@ERROR"           )
        M_vt_add(L"ERROR"         , vt, verb_error          )
    }


    // ============================================================================
    // @END integer_value -- value is optional 
    // ============================================================================

    {
        M_vt_prefix(           vt, 0, 1                     ) 
        M_vt_int64rc_right_pos(vt, M_int32_min, M_int32_max )

        M_vt_info(             vt, L"@END wmain()-R/C "     )
        M_vt_add(L"END"      , vt, verb_end                 )
    }



    // ===================================================================================
    // @EXIT integer_value    exit:  _exit:  _Exit:  quick_exit:  terminate:  abort:, etc. 
    // ===================================================================================

    {
        M_vt_prefix(                  vt, 0, 1                                                   ) 
        M_vt_int64rc_right_pos(       vt, M_int32_min, M_int32_max                               )
        M_vt_nval_optional_right_kw(  vt, L"exit"                                                )  
        M_vt_nval_optional_right_kw(  vt, L"_exit"                                               )  
        M_vt_nval_optional_right_kw(  vt, L"_Exit"                                               )  
        M_vt_nval_optional_right_kw(  vt, L"abort"                                               ) 
        M_vt_nval_optional_right_kw(  vt, L"quick_exit"                                          ) 
        M_vt_nval_optional_right_kw(  vt, L"terminate"                                           ) 
        M_vt_right_conflict_6way(     vt, L"exit", L"_exit", L"_Exit", L"abort", L"quick_exit", L"terminate()"      )    // default is exit

        M_vt_info(                    vt, L"@EXIT exit()-code <exit:|_exit:|_Exit:|abort:|quick_exit:|terminate:>"  )
        M_vt_add(L"EXIT"            , vt, verb_exit                                              )
    }



    // ============================================================================
    // @BREAK value(s) -- values are optional 
    // ============================================================================

    {
        M_vt_nary_prefix(         vt             )                     // 0 to N right positional parms are allowed 
        M_vt_assigntype_right_pos(vt             )
        M_vt_no_check_right_kws(  vt             )                     // allow any right-side keyword parms
   
        M_vt_info(                vt, L"@BREAK  <value1 value2 ...>" )
        M_vt_add(L"BREAK",        vt, verb_break)
    }  


    // ============================================================================
    // @QUIT value(s) -- values are optional  
    // ============================================================================

    {
        M_vt_nary_prefix(         vt             )                     // 0 to N right positional parms are allowed                      
        M_vt_assigntype_right_pos(vt             )
        M_vt_no_check_right_kws(  vt             )                     // allow any right-side keyword parms

        M_vt_info(                vt, L"@QUIT  <value1 value2 ...>" )
        M_vt_add(L"QUIT",         vt, verb_quit)
    }  


    // ============================================================================================================================================
    // @LEAVE  -or-  "target" @LEAVE value1 value2 value3 ... -- end current (or enclosing) block evaluation --or-- block containing label "target"
    // ============================================================================================================================================

    {
        M_vt_anyfix(            vt, 0, 1, 0, -1)                     // 0 or 1 left positional parms are allowed, 0-N right positional parms area allowed  
        M_vt_string_left_pos(   vt             )                     // left-side leave target (if present) needs to be a string
        M_vt_no_check_right_kws(vt             )                     // allow any right-side keyword parms

        M_vt_info(              vt, L"< 'label > @LEAVE <value1 value2 ...>" )
        M_vt_add(L"LEAVE",      vt, verb_leave)
    }
    

    // ============================================================================
    // @RETURN value(s) -- values are optional  
    // ============================================================================

    {
        M_vt_nary_prefix(         vt             )                 // 0 to N right positional parms are allowed 
        M_vt_assigntype_right_pos(vt             )
        M_vt_no_check_right_kws(  vt             )                 // allow any right-side keyword parms

        M_vt_info(                vt, L"@RETURN  <value1 value2 ...>" )
        M_vt_add(L"RETURN",       vt, verb_return)
    }  


    // ============================================================================
    // @THROW value -- value is required
    // ============================================================================

    {
        M_vt_unary_prefix(         vt            ) 
        M_vt_assigntype_right_pos( vt            )

        M_vt_info(                 vt, L"@THROW value" )
        M_vt_add(L"THROW",         vt, verb_throw)
    }  


    // =============================================================================
    // @GOTO "target" longjmp: -- restart active block evaluation after target label
    // =============================================================================

    {
        M_vt_unary_prefix(           vt              ) 
        M_vt_string_right_pos(       vt              )                      // goto target needs to be a string
        M_vt_nval_optional_right_kw( vt, L"longjmp"  )                      // optional longjmp:   allows jumping out of a function to target label in upper (calling) blocks

        M_vt_info(                   vt, L"@GOTO 'label <longjmp:>" )
        M_vt_add(L"GOTO",            vt, verb_goto   )
    }

    
    // =====================================
    // @DO {block}   continue:
    // =====================================

    {
        M_vt_unary_prefix(                  vt                        )
        M_vt_block_right_pos(               vt                        ) 

        M_vt_nval_optional_right_kw(        vt, L"continue"           )                      // optional continue:   continue running, if parsing error
        M_vt_nval_optional_right_kw(        vt, L"end"                )                      // optional end:        end run,          if parsing error    (default)
        M_vt_right_conflict_pair(           vt, L"continue", L"end"   )

        M_vt_info(                          vt, L"@DO {block}  < continue: | end: >" )
        M_vt_add(L"DO",                     vt, verb_do               )
    }
 

    // =====================================
    // @TRY {block} catch:{block}
    // =====================================
          
    {
        M_vt_unary_prefix(           vt            )  
        M_vt_block_right_pos(        vt            ) 
        
        M_vt_block_optional_right_kw(vt, L"catch"  )
        M_vt_block_optional_right_kw(vt, L"finally")

        M_vt_info(                   vt, L"@TRY {block}  < catch:{block} >   < finally:{block} >" )
        M_vt_add(L"TRY",             vt, verb_try  )
    }


    // =========================================
    // @BLOCK left:[vlist] right:[vlist] {block}
    // =========================================

    {
        M_vt_unary_prefix(            vt            )
        M_vt_block_right_pos(         vt            )

        M_vt_vlist0_optional_right_kw(vt, L"left"   )
        M_vt_vlist0_optional_right_kw(vt, L"right"  )

        M_vt_info(                    vt, L"@BLOCK {block}  < left:[left parms] >   < right:[right parms] >" )
        M_vt_add(L"BLOCK",            vt, verb_block)
    }



    // ==========================================================================
    // @CASE when:(expression) {block} when:(expression) {block} ... else:{block} 
    // ==========================================================================

    {
        M_vt_nary_prefix(             vt         )
        M_vt_block_right_pos(         vt         )

        {                                                 // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_optional_kw_parm(kp)
            kp.kw_min_ct                =  0;
            kp.kw_max_ct                = -1;                      // multiple when: keywords allowed
            kp.eval.no_eval_ident       = true;
            kp.eval.no_eval_expression  = true;
            kp.int8_ok                  = true; 
            kp.int16_ok                 = true;
            kp.int32_ok                 = true;
            kp.int64_ok                 = true;
            kp.uint8_ok                 = true;
            kp.uint16_ok                = true;
            kp.uint32_ok                = true;
            kp.uint64_ok                = true;
            kp.var_ident_ok             = true;
            kp.const_ident_ok           = true;
            kp.undef_ident_ok           = true;   // ???????? in case prior case clauses code cause variable to get defined ???????????
            kp.expression_ok            = true;
            M_vt_add_r_kw(L"when",   vt, kp       ) 
        }

        M_vt_block_optional_right_kw(vt, L"else"  )
 
        M_vt_custom_eval(            vt           ) 

        M_vt_info(                   vt, L"@CASE  when:(expr) {block} when:(expr) {block} ...  < else:{block} >" )
        M_vt_add(L"CASE",            vt, verb_case)
    }
                               
    
    // =====================================
    // @LOOP while:int-value  {block}
    // =====================================
          
    {
        M_vt_unary_prefix(           vt             )  

        {                                                   // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_optional_kw_parm(kp);

            kp.eval.no_eval_ident      = true;
            kp.eval.no_eval_expression = true;
            kp.boolean_ok              = true; 
            kp.int8_ok                 = true; 
            kp.int16_ok                = true;
            kp.int32_ok                = true;
            kp.int64_ok                = true;
            kp.uint8_ok                = true;
            kp.uint16_ok               = true;
            kp.uint32_ok               = true;
            kp.uint64_ok               = true;
            kp.var_ident_ok            = true;
            kp.const_ident_ok          = true;
            kp.undef_ident_ok          = true;          // may get defined in loop body ??????
            kp.expression_ok           = true;

            M_vt_add_r_kw(L"until",  vt, kp        ) 
            M_vt_add_r_kw(L"while",  vt, kp        ) 
        }
        
        M_vt_block_optional_right_kw(vt, L"init"   ) 
        M_vt_block_optional_right_kw(vt, L"next"   )

        M_vt_block_right_pos(        vt            )

        M_vt_custom_eval(            vt            ) 

        M_vt_info(                   vt, L"@LOOP {block}  < init:{block} >  <while:(expr) >   < until:(expr) >   < next:{block} >" )
        M_vt_add(L"LOOP",            vt, verb_loop )
    }

    
    // =====================================
    // @IF value then:{block} else:{block}
    // =====================================

    {
        M_vt_unary_prefix(            vt           ) 

        M_vt_block_optional_right_kw( vt, L"then"  )
        M_vt_block_optional_right_kw( vt, L"else"  )  

        M_vt_int_right_pos(           vt           )

        M_vt_info(                    vt, L"@IF (expr) < then:{block} >  < else:{block} >" )
        M_vt_add(L"IF",               vt, verb_if  )
    }


    // ======================================
    // (@COND) = "?"  value ? {block} {block}
    // ======================================

    {
        M_vt_anyfix(                  vt, 1, 1, 1, 2               )            // 1 left positional parms -- 1-2 right positional parm 
        M_vt_int_left_pos(            vt                           )            // condition is an integer
        M_vt_block_right_pos(         vt                           )            // block done, if true  -- required
        M_vt_block_right_pos(         vt                           )            // block done, if false -- optional

        M_vt_info(                    vt, L"(expr) ? {block} < {block} >" )
        M_vt_add(L"?",                vt, verb_cond )
    }




    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   variable definition and assignment verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // =======================================================================================================
    // @VAR ident ident ident ... value:value  global: local: static: expose: share: unshare:  -- value: keyword is optional
    // =======================================================================================================
   
    {
        M_vt_nary_prefix(                 vt                        )                                                                     
        M_vt_assigntype_optional_right_kw(vt, L"value"              )
        M_vt_nval_optional_right_kw (     vt, L"global"             )
        M_vt_nval_optional_right_kw (     vt, L"local"              )
        M_vt_nval_optional_right_kw (     vt, L"verbmain"           )
        M_vt_nval_optional_right_kw (     vt, L"static"             )
#ifdef M_EXPOSE_SUPPORT
        M_vt_nval_optional_right_kw (     vt, L"expose"             )
#endif
        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )

        M_vt_right_conflict_4way(         vt, L"global", L"local", L"verbmain", L"static")
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" ) 

        M_vt_any_ident_right_pos(         vt                        )

        M_vt_no_eval_right_ident(         vt                        )

        M_vt_info(                        vt, L"@VAR name1 name2 ... < value:val > < global: | static: | verbmain: | local: >  < share: | unshare: >" )
        M_vt_add(L"VAR",                  vt, verb_var              )
    }

   
    // =======================================================================================
    // @CONST ident value:value global: expose: local: static: share: unshare:  -- value: keyword is required   
    // =======================================================================================

    {
        M_vt_unary_prefix(                vt                        )                                                                      
        M_vt_assigntype_optional_right_kw(vt, L"value"              )
        M_vt_nval_optional_right_kw (     vt, L"global"             )
        M_vt_nval_optional_right_kw (     vt, L"local"              )
        M_vt_nval_optional_right_kw (     vt, L"verbmain"           )
        M_vt_nval_optional_right_kw (     vt, L"static"             )
#ifdef M_EXPOSE_SUPPORT
        M_vt_nval_optional_right_kw (     vt, L"expose"             )
#endif
        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )

        M_vt_right_conflict_4way(         vt, L"global", L"local", L"verbmain", L"static")
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" ) 

        M_vt_any_ident_right_pos(         vt                        )
        
        M_vt_no_eval_right_ident(         vt                        )

        M_vt_info(                        vt, L"@CONST name1 name2 ... < value:val > < global: | static: | verbmain: | local: >  < share: | unshare: >" )
        M_vt_add(L"CONST",                vt, verb_const)
    }


    // =======================================================================================================
    // ident ident ident ... @ALIAS ident ident ident ...   global: local: static: expose: 
    // =======================================================================================================
   
    {
        M_vt_nary_infix(                  vt                        )                                                                     

        M_vt_nval_optional_right_kw (     vt, L"weak"               )

        M_vt_nval_optional_right_kw (     vt, L"global"             )
        M_vt_nval_optional_right_kw (     vt, L"local"              )
        M_vt_nval_optional_right_kw (     vt, L"verbmain"           )
        M_vt_nval_optional_right_kw (     vt, L"static"             )
#ifdef M_EXPOSE_SUPPORT
        M_vt_nval_optional_right_kw (     vt, L"expose"             )
#endif
        M_vt_right_conflict_4way(         vt, L"global", L"local", L"verbmain", L"static")

        M_vt_any_ident_left_pos(          vt                        )
        M_vt_no_eval_left_ident(          vt                        )
        M_vt_any_ident_right_pos(         vt                        )
        M_vt_no_eval_right_ident(         vt                        )
        vt.d.parms_same_number            = true;
        vt.d.parms_some_required          = true;

        M_vt_info(                        vt, L"name1 name2 ... @ALIAS name3 name4 ... < global: | static: | verbmain: | local: >" )
        M_vt_add(L"ALIAS",                vt, verb_alias            )
    }


    // ===============================
    // \  (@PASS)   ident 
    // ===============================

    {
        M_vt_unary_prefix(                vt                        )
        M_vt_any_ident_right_pos(         vt                        )    
        M_vt_no_eval_right_ident(         vt                        )

        M_vt_info(                        vt, L"\\ identifier"      )
        M_vt_add(L"\\"  ,                 vt, verb_pass             )
    }  
          

#if 0
    // ===============================
    // @NOEVAL ident   share: noshare: 
    // ===============================

    {
        M_vt_unary_prefix(                vt                        )
 
        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" )  

        M_vt_any_ident_right_pos(         vt                        )    // ??? need to strengthen  this to cover everything, not just identifiers -- also allow multi parms, keywords, etc. (like SEP), but set sticky noeval flag in the value_S ??? 
        M_vt_no_eval_right_ident(         vt                        )

        M_vt_info(                        vt, L"@NOEVAL identifier  < share: | unshare: >" )
        M_vt_add(L"NOEVAL"  ,             vt, verb_noeval           )
    }  


    // ===========================
    // @EVAL value value value ...
    // ===========================

    {
        M_vt_nary_prefix(                 vt                         )      
        M_vt_any_right_pos(               vt                         ) 

        M_vt_info(                        vt, L"@EVAL val1 val2 ..." )
        M_vt_add(L"EVAL",                 vt, verb_eval              );        
    }
#endif

    // ======================
    // @UNSHARE value 
    // ======================

    {
        M_vt_unary_prefix(                vt                        ) 
        M_vt_assigntype_right_pos(        vt                        )

        M_vt_info(                        vt, L"@UNSHARE value"     )
        M_vt_add(L"UNSHARE",              vt, verb_unshare          )
    }  


#ifdef M_EXPOSE_SUPPORT
    // ======================
    // @EXPOSE    ident ident ...
    // @UNEXPOSE  ident ident ...
    // ======================

    {
        M_vt_nary_prefix(        vt                            )
        M_vt_def_ident_right_pos(vt                            )  
        M_vt_no_eval_right_ident(vt                            )

        M_vt_nval_optional_right_kw ( vt, L"local"             )
        M_vt_nval_optional_right_kw ( vt, L"verbmain"          )
        M_vt_nval_optional_right_kw ( vt, L"static"            )
        M_vt_right_conflict_3way(     vt, L"local", L"verbmain", L"static")

        M_vt_info(                    vt, L"@EXPOSE identifier < local: | verbmain: | static: > "     )
        M_vt_add(L"EXPOSE"  ,         vt, verb_expose               )
                                     
        M_vt_info(                    vt, L"@UNEXPOSE identifier < local: | verbmain: | static: > "   )
        M_vt_add(L"UNEXPOSE",         vt, verb_unexpose             )
    }  
#endif


    // ===================================================
    // @UNVAR     ident ident ...   global: local: static: 
    // ===================================================
   
    {
        M_vt_nary_prefix(                 vt                        )
        M_vt_any_ident_right_pos(         vt                        )    
        M_vt_nval_optional_right_kw(      vt, L"global"             ) 
        M_vt_nval_optional_right_kw(      vt, L"local"              )
        M_vt_nval_optional_right_kw (     vt, L"verbmain"          )
        M_vt_nval_optional_right_kw(      vt, L"static"             )
        M_vt_right_conflict_4way(         vt, L"global", L"verbmain", L"local", L"static")

        M_vt_info(                        vt, L"@UNVAR identifier  < global: | static: | verbmain: | local: >")
        M_vt_add(L"UNVAR",                vt, verb_unvar            )
    }   


    // =====================================================
    // @UNALIAS     ident ident ...   global: local: static:
    // =====================================================
   
    {
        M_vt_nary_prefix(                 vt                        )
        M_vt_any_ident_right_pos(         vt                        )    
        M_vt_nval_optional_right_kw(      vt, L"global"             ) 
        M_vt_nval_optional_right_kw(      vt, L"local"              )
        M_vt_nval_optional_right_kw (     vt, L"verbmain"           )
        M_vt_nval_optional_right_kw(      vt, L"static"             )
        M_vt_right_conflict_4way(         vt, L"global", L"verbmain", L"local", L"static")

        M_vt_info(                        vt, L"@UNALIAS identifier  < global: | static: | verbmain: | local: >")
        M_vt_add(L"UNALIAS",              vt, verb_unalias          )
    }   



    // ===========================================
    //  =    left assignment operator
    // >=>   right assignment operator
    // ===========================================
    
    {                                      /* 1 or more vars */
        M_vt_anyfix(                      vt, 1, -1, 0, -1          )  
        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" ) 

        M_vt_lvalue_left_pos(             vt                        )
        M_vt_assigntype_right_pos(        vt                        )

        M_vt_no_eval_left_lvalue(         vt                        )     
        M_vt_priority(                    vt, verb_pri_N::_assign   ) 
        M_vt_right_associate(             vt                        )

        M_vt_info(                        vt, L"ident1 ident2 ...  =  value1 value2 ... < share: | unshare: >")
        M_vt_add(L"="  ,                  vt, verb_left_assign      )
        
    }

 #if 0 
    {                                     /* 1 or more vars */
        M_vt_anyfix(                      vt, 0, -1, 1, -1          ) 

        M_vt_nval_optional_right_kw (     vt, L"share"              )
        M_vt_nval_optional_right_kw (     vt, L"unshare"            )
        M_vt_right_conflict_pair(         vt, L"share" , L"unshare" ) 

        M_vt_lvalue_right_pos(            vt                        )
        M_vt_assigntype_left_pos(         vt                        )
        M_vt_no_eval_right_lvalue(        vt                        )  
        M_vt_priority(                    vt, verb_pri_N::_assign   ) 
        M_vt_add(L">=>",                  vt, verb_right_assign     )              
    }
#endif     


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   arithmetic and string assignment operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // =========================================
    // ++ -- unary increment/decrement operators
    // =========================================

    {
        M_vt_unary_onefix(        vt                         )                     // unary prefix or postfix
        M_vt_lvalue_left_pos(     vt                         )                     // either left -or- right allowed 
        M_vt_lvalue_right_pos(    vt                         )                     // either right -or- left allowed


        M_vt_no_eval_left_lvalue( vt                         ) 
        M_vt_no_eval_right_lvalue(vt                         ) 
        M_vt_priority(            vt, verb_pri_N::_increment )
       
        M_vt_info(                vt, L"lvalue ++    -or-    ++ lvalue" )
        M_vt_add(L"++",           vt, verb_increment         )
     
        M_vt_info(                vt, L"lvalue --    -or-    -- lvalue" )
        M_vt_add(L"--",           vt, verb_decrement         )
    }


    // =======================================================
    // binary arithmetic xxx_eq operators      += -= *= /= ^=
    // =======================================================

     { 
        M_vt_binary_infix(       vt                      )
        M_vt_lvalue_left_pos(    vt                      )
        M_vt_arith_right_pos(    vt                      )

        M_vt_no_eval_left_lvalue(vt                      ) 
        M_vt_priority(           vt, verb_pri_N::_assign ) 
        M_vt_right_associate(    vt                      )

        M_vt_info(               vt, L"lvalue += value"  )
        M_vt_add(L"+=" ,         vt, verb_add_eq         )

        M_vt_info(               vt, L"lvalue -= value"  )
        M_vt_add(L"-=" ,         vt, verb_subtract_eq    )

        M_vt_info(               vt, L"lvalue *= value"  )
        M_vt_add(L"*=" ,         vt, verb_multiply_eq    )

     // M_vt_add(L"×=" ,         vt, verb_multiply_eq    )

        M_vt_info(               vt, L"lvalue /= value"  )
        M_vt_add(L"/=" ,         vt, verb_divide_eq      )

     // M_vt_add(L"÷=" ,         vt, verb_divide_eq      )

        M_vt_info(               vt, L"lvalue **= value" )
        M_vt_add(L"**=",         vt, verb_power_eq       )      // ????? not in C  
    }


    // ================================================
    //  binary integer xxx_eq operator     %=   @REM_EQ
    // ================================================

    { 
        M_vt_binary_infix(       vt                      )
        M_vt_lvalue_left_pos(    vt                      )
        M_vt_int_right_pos(      vt                      )     // only supported for integers 

        M_vt_no_eval_left_lvalue(vt                      ) 
        M_vt_priority(           vt, verb_pri_N::_assign ) 
        M_vt_right_associate(    vt                      )

        M_vt_info(               vt, L"lvalue %= integer_value"  )
        M_vt_add(L"%="     ,     vt, verb_remainder_eq   )

     // M_vt_add(L"REM_EQ" ,     vt, verb_remainder_eq   )
    }


    // ===========================================
    //  binary string concatenate_eq() verb     |=   
    // ===========================================

    { 
        M_vt_binary_infix(       vt                      )
        M_vt_lvalue_left_pos(    vt                      )
        M_vt_string_right_pos(   vt                      )     // only supported for strings
       
        M_vt_no_eval_left_lvalue(vt                      ) 
        M_vt_priority(           vt, verb_pri_N::_assign ) 
        M_vt_right_associate(    vt                      )

        M_vt_info(               vt, L"lvalue |||= string_value"  )
        M_vt_add(L"|||=",        vt, verb_concatenate_eq )
    }


    // ================================================================================================================
    // binary bitwise/shift xxx_eq operators      @BITAND_EQ  @BITOR_EQ  @BITXOR_EQ   ∧∧=  ∨∨=  ⊻⊻=    >>=   <<=   >->=
    // ================================================================================================================

    //   ??????????? missing @BITNOT_EQ    or   ~=     these are unary prefix  ??????????????????????????? 

     { 
        M_vt_binary_infix(       vt                                   )
        M_vt_lvalue_left_pos(    vt                                   )
        M_vt_int_right_pos(      vt                                   )        // only supported for integers  
 
        M_vt_no_eval_left_lvalue(vt                                   )      
        M_vt_priority(           vt, verb_pri_N::_assign              )  
        M_vt_right_associate(    vt                                   )

        M_vt_info(               vt, L"lvalue &= integer_value"       )
        M_vt_add(L"&="        ,  vt, verb_bitand_eq                   )

     // M_vt_add(L"BITAND_EQ" ,  vt, verb_bitand_eq                   )
     // M_vt_add(L"∧∧="       ,  vt, verb_bitand_eq                   )

        M_vt_info(               vt, L"lvalue |= integer_value"       )
        M_vt_add(L"|="        ,  vt, verb_bitor_eq                    )
            
     // M_vt_add(L"BITOR_EQ"  ,  vt, verb_bitor_eq                    )
     // M_vt_add(L"∨∨="       ,  vt, verb_bitor_eq                    )

        M_vt_info(               vt, L"lvalue ^= integer_value"       )
        M_vt_add(L"^="        ,  vt, verb_bitxor_eq                   )

     // M_vt_add(L"BITXOR_EQ" ,  vt, verb_bitxor_eq                   )
     // M_vt_add(L"⊻⊻="      ,   vt, verb_bitxor_eq                   )

        M_vt_info(               vt, L"lvalue <<= integer_value"      )
        M_vt_add(L"<<="       ,  vt, verb_shift_left_eq               )

        M_vt_info(               vt, L"lvalue >>= integer_value"      )
        M_vt_add(L">>="       ,  vt, verb_shift_right_logical_eq      )

        M_vt_info(               vt, L"lvalue >->= integer_value"     )
        M_vt_add(L">->="      ,  vt, verb_shift_right_arithmetic_eq   )        // ??????? not in C
    }
       

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   comparison operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // =========================================================================================
    // ==  ≡  ¬=  !=  ≠  >  <  >=  <=  @EQ  @NE  @LT  @GT  @LE  @GE  binary comparison operators
    // =========================================================================================

    {
        M_vt_binary_infix(     vt         )
        vt.d.parms_same_type     = true;
        M_vt_compare_right_pos(vt         ) 
        M_vt_compare_left_pos( vt         )


        M_vt_priority(         vt, verb_pri_N::_compare )

     // M_vt_add(L"EQ",        vt, verb_eq)

        M_vt_info(             vt, L"value == value"    )
        M_vt_add(L"==",        vt, verb_eq)

     // M_vt_add(L"≡" ,        vt, verb_eq)
     // M_vt_add(L"NE",        vt, verb_ne)
     // M_vt_add(L"¬=",        vt, verb_ne)

        M_vt_info(             vt, L"value != value"    )
        M_vt_add(L"!=",        vt, verb_ne)

     // M_vt_add(L"≠" ,        vt, verb_ne)
     // M_vt_add(L"LT",        vt, verb_lt)

        M_vt_info(             vt, L"value < value"     )
        M_vt_add(L"<" ,        vt, verb_lt)

     // M_vt_add(L"GT",        vt, verb_gt)

        M_vt_info(             vt, L"value > value"     )
        M_vt_add(L">" ,        vt, verb_gt)

     // M_vt_add(L"LE",        vt, verb_le)

        M_vt_info(             vt, L"value <= value"    )
        M_vt_add(L"<=",        vt, verb_le)

     // M_vt_add(L"≤" ,        vt, verb_le)
     // M_vt_add(L"GE",        vt, verb_ge)

        M_vt_info(             vt, L"value >= value"    )
        M_vt_add(L">=",        vt, verb_ge)

     // M_vt_add(L"≥" ,        vt, verb_ge)
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   logical operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // ======================================
    //  &&  ||  ^^   binary logical operators 
    // ====================================== 

    // without lazy evaluation
    // -----------------------

    {
        M_vt_binary_infix(  vt           )
        M_vt_int_right_pos( vt           ) 
        M_vt_int_left_pos(  vt           ) 
      
        M_vt_priority(      vt, verb_pri_N::_boolean )
    
        M_vt_info(          vt, L"value ^^ value"    )
        M_vt_add(L"^^"  ,   vt, verb_xor )           // ????????????????????  not in C

     // M_vt_add(L"⊻"    ,  vt, verb_xor )           // ????????????????????
     // M_vt_add(L"XOR"  ,  vt, verb_xor )           // ????????????????????
    }


    // with lazy evaluation
    // --------------------

    {
        M_vt_binary_infix(            vt             )

        M_vt_int_left_pos(            vt             ) 

        {                                          // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            M_vt_pos_parm(                pt         )
       //     pt.eval.no_eval_ident       = true; 
       //     pt.eval.no_eval_expression  = true; 
            pt.int8_ok                  = true;
            pt.int16_ok                 = true;
            pt.int32_ok                 = true;
            pt.int64_ok                 = true;
            pt.uint8_ok                 = true;
            pt.uint16_ok                = true;
            pt.uint32_ok                = true;
            pt.uint64_ok                = true;
            pt.var_ident_ok             = true;
            pt.const_ident_ok           = true;
            pt.undef_ident_ok           = true;
            pt.expression_ok            = true;
            M_vt_add_r_pos(           vt, pt                  )
        }
      
        M_vt_no_eval_left_ident(      vt                       ) 
        M_vt_no_eval_left_expression( vt                       ) 

        M_vt_priority(                vt, verb_pri_N::_boolean )

        M_vt_info(                    vt, L"value && value"    )
        M_vt_add(L"&&"   ,            vt, verb_and)

     // M_vt_add(L"AND"  ,            vt, verb_and)
     // M_vt_add(L"∧"    ,            vt, verb_and)
 
        M_vt_info(                    vt, L"value && value"    )
        M_vt_add(L"||"   ,            vt, verb_or )
   
     // M_vt_add(L"OR"   ,            vt, verb_or )
     // M_vt_add(L"∨"    ,            vt, verb_or )
            
    }

       
    // =====================================
    //  !  unary logical operators (@NOT)
    // ===================================== 

    {
        M_vt_unary_prefix(   vt                     )
        M_vt_int_left_pos(   vt                     ) 
        
        M_vt_priority(       vt,  verb_pri_N::_not  )
        M_vt_right_associate(vt                     )

        M_vt_info(           vt, L"! value"         )
        M_vt_add(L"!"   ,    vt, verb_not           )
            
     // M_vt_add(L"NOT" ,    vt, verb_not           )
     // M_vt_add(L"¬"   ,    vt, verb_not           )
    }
   

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   bitwise operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
    
    // ==============================================
    // &   |  ^    binary/bilateral bitwise operators  
    // ==============================================

    {
        M_vt_binary_infix(      vt                                     )
                                                                     
        vt.d.parms_same_type     = true;                             
                                                                     
        M_vt_int_left_pos(      vt                                     )
        M_vt_int_right_pos(     vt                                     )


        M_vt_priority(         vt, verb_pri_N::_bitwise                ) 

        M_vt_info(             vt, L"integer_value & integer_value"    )
        M_vt_add(L"&"     ,    vt, verb_bitand                         )
                                                                       
     // M_vt_add(L"∧∧"    ,    vt, verb_bitand                         )
     // M_vt_add(L"BITAND",    vt, verb_bitand                         )

        M_vt_info(             vt, L"integer_value | integer_value"    )
        M_vt_add(L"|"     ,    vt, verb_bitor                          )
                                                                       
     // M_vt_add(L"∨∨"    ,    vt, verb_bitor                          )
     // M_vt_add(L"BITOR" ,    vt, verb_bitor                          )

        M_vt_info(             vt, L"integer_value ^ integer_value"    )
        M_vt_add(L"^"     ,    vt, verb_bitxor                         )
                                                                      
     // M_vt_add(L"⊻⊻"    ,    vt, verb_bitxor                         )
     // M_vt_add(L"BITXOR",    vt, verb_bitxor                         )  
    }


    // =========================
    // ~  unary bitwise operator      
    // ==========================

    {
        M_vt_unary_prefix(     vt                      )
        M_vt_int_left_pos(     vt                      ) 
        
        M_vt_priority(         vt, verb_pri_N::_bitnot )
        M_vt_right_associate(  vt                      )

        M_vt_info(             vt, L"! integer_value"  )
        M_vt_add(L"~"      ,   vt, verb_bitnot         ) 

     // M_vt_add(L"BITNOT" ,   vt, verb_bitnot         )
    }


    // ===================================
    // >>  >->  <<  binary shift operators  
    // ===================================

    {
        M_vt_binary_infix(      vt                             )
        M_vt_int_left_pos(      vt                             )
        M_vt_int_right_pos(     vt                             )  

        M_vt_priority(          vt, verb_pri_N::_shift         ) 

        M_vt_info(              vt, L"<< integer_value"        )
        M_vt_add(L"<<"    ,     vt, verb_shift_left            )

        M_vt_info(              vt, L">> integer_value"        )
        M_vt_add(L">>"    ,     vt, verb_shift_right_logical   )

        M_vt_info(              vt, L">-> integer_value"       )
        M_vt_add(L">->"   ,     vt, verb_shift_right_arithmetic)        // ???????????  not in C
    } 


    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   arithmetic and string operators
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // ====================================
    // + * / ** binary arithmetic operators ( - is later )
    // ====================================

    {
        M_vt_binary_infix(    vt          )

        vt.d.parms_same_type = true;

        M_vt_arith_left_pos(  vt          )
        M_vt_arith_right_pos( vt          )

        M_vt_priority(        vt,verb_pri_N::_add) 
        M_vt_info(            vt, L"arithmetic_value + arithmetic_value"    )
        M_vt_add(L"+",        vt, verb_add)

        M_vt_priority(        vt,verb_pri_N::_multiply) 
        M_vt_info(            vt, L"arithmetic_value * arithmetic_value"    )
        M_vt_add(L"*",        vt, verb_multiply)

        M_vt_info(            vt, L"arithnetic_value / arithmetic_value"    )
        M_vt_add(L"/",        vt, verb_divide)
      
        M_vt_priority(        vt,verb_pri_N::_power) 
        M_vt_info(            vt, L"arithmetic_value ** arithmetic_value"    )
        M_vt_add(L"**",       vt, verb_power)              // ??????????????       not in C
    }


    // ===========================================
    // %  binary remainder operator (integer only)
    // ===========================================

    {
        M_vt_binary_infix(  vt)

        vt.d.parms_same_type     = true;

        M_vt_int_left_pos(  vt          )
        M_vt_int_right_pos( vt          ) 

        M_vt_priority(      vt,verb_pri_N::_multiply) 
        M_vt_info(          vt, L"integer_value % integer_value"    )
        M_vt_add(L"%"  ,    vt, verb_remainder)

     // M_vt_add(L"REM",    vt, verb_remainder)
    }


    // ======================================
    // - unary or binary arithmetic operators
    // ======================================

    {
        M_vt_anyfix(          vt, 0, 1, 1, 1)           // one right side parm required, left side parm is optional  
        M_vt_priority(        vt,verb_pri_N::_add)

        vt.d.parms_same_type     = true;

        M_vt_arith_left_pos(  vt            )
        M_vt_arith_right_pos( vt            )

        M_vt_info(            vt, L"arithmetic_value - arithmetic_value    -or-     - arithmetic_value" )
        M_vt_add(L"-",        vt, verb_minus)        
    }


    // ============================
    // |||  binary string operators
    // ============================

    {
        M_vt_binary_infix(     vt                  ) 
                                                
        M_vt_string_left_pos(  vt                  )
        M_vt_string_right_pos( vt                  )

        M_vt_info(             vt, L"string ||| string"    )
        M_vt_add(L"|||",       vt, verb_concatenate)   // ???? needs to become + (or ++?) or be  overloaded with bitwise logical OR
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
        M_vt_int64rc_optional_right_kw(         vt, L"len"   , 1, M_int64_max                              )          
        M_vt_typdef_optional_right_kw(          vt, L"type"                                                ) 

        M_vt_info(                              vt, L"@AGG  < len:int >   < type:typedef >"                )
        M_vt_add(L"AGG"                       , vt, verb_agg                                               ) 
    } 


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   type-oriented verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   

    // ====================================================
    //
    //  @TYPE name:typedef_name 
    //        global:
    //        expose:
    //        unit:
    //        bool:
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
        M_vt_nval_optional_right_kw (     vt, L"global"                                                                  )           // this typename goes ino global     environment -- ignored if name: is not present
        M_vt_nval_optional_right_kw (     vt, L"local"                                                                   )           // this typename goes into local     environment -- ignored if name: is not present
        M_vt_nval_optional_right_kw (     vt, L"verbmain"                                                                )           // this typename goes into verbmain  environment -- ignored if name: is not present
        M_vt_nval_optional_right_kw (     vt, L"static"                                                                  )           // this typename goes into static    environment -- ignored if name: is not present
#ifdef M_EXPOSE_SUPPORT
        M_vt_nval_optional_right_kw (     vt, L"expose"                                                                  )           // default is don't expose -- ignored if global: (or name: is not present)
#endif
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

        M_vt_right_conflict_4way(         vt, L"global", L"local", L"verbmain", L"static"                                )

        // array:[ n_elements element_type_t ] 

        {
            M_vt_nested_plist(                              pl, 2, 2                                                     )           // mandatory positional parms are:   number of array elements -and- type of each array element 
            M_vt_int64rc_nest_pos(                          pl, 1, M_int64_max                                           )           // mandatory -- number of array elements
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
                M_vt_int64rc_optional_nest_kw(                  pln, L"offset", 0, M_int64_max                           )           // optional offset:nnnn keyword  (offset must not be negative)
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
        
        M_vt_custom_eval(               vt                                       )  
        M_vt_info(                      vt, L"@TYPE  |name:«name»|  |global:|  |atomic types ...|  |array:[num_elements typdef]|  struct:[ [ ... ] [ ... ] ]"    )
        M_vt_add(L"TYPE"              , vt, verb_type                            ) 
     }   

     

    // ====================================================
    //  @TO_XXXX type conversion verbs                            ?? get rid of these once types are in place
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
        M_vt_unary_prefix(         vt                  )
        M_vt_string_right_pos(     vt                  )
        M_vt_add(L"TO_IDENT"     , vt, verb_to_ident   ) 
        M_vt_add(L"TO_VERBNAME"  , vt, verb_to_verbname) 
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
    //  @IS_XXXX type checking verbs                                         ?? get rid of these once types are in place
    // ==================================================== 

    {
        M_vt_unary_prefix(      vt                           )
        M_vt_any_right_pos(     vt                           )                                                      

        M_vt_add(L"IS_UNIT"         , vt, verb_is_unit       ) 
        M_vt_add(L"IS_BOOL"         , vt, verb_is_bool       )
        M_vt_add(L"IS_INT8"         , vt, verb_is_int8       )
        M_vt_add(L"IS_INT16"        , vt, verb_is_int16      )
        M_vt_add(L"IS_INT32"        , vt, verb_is_int32      )
        M_vt_add(L"IS_INT64"        , vt, verb_is_int64      )
        M_vt_add(L"IS_UINT8"        , vt, verb_is_uint8      )
        M_vt_add(L"IS_UINT16"       , vt, verb_is_uint16     )
        M_vt_add(L"IS_UINT32"       , vt, verb_is_uint32     )
        M_vt_add(L"IS_UINT64"       , vt, verb_is_uint64     )
        M_vt_add(L"IS_FLOAT32"      , vt, verb_is_float32    )
        M_vt_add(L"IS_FLOAT64"      , vt, verb_is_float64    )
        M_vt_add(L"IS_STRING"       , vt, verb_is_string     )
        M_vt_add(L"IS_INT"          , vt, verb_is_int        )
        M_vt_add(L"IS_FLOAT"        , vt, verb_is_float      )
        M_vt_add(L"IS_SIGNED"       , vt, verb_is_signed     )
        M_vt_add(L"IS_UNSIGNED"     , vt, verb_is_unsigned   )
        M_vt_add(L"IS_NUMERIC"      , vt, verb_is_numeric    )
        M_vt_add(L"IS_ATOM"         , vt, verb_is_atom       )
        M_vt_add(L"IS_TRUE"         , vt, verb_is_true       )
        M_vt_add(L"IS_FALSE"        , vt, verb_is_false      )
        M_vt_add(L"IS_BLOCK"        , vt, verb_is_block      )
        M_vt_add(L"IS_VERBNAME"     , vt, verb_is_verbname   )         // ?????
    }


    ////////////////////////////////////////////////////////////

    {
        M_vt_unary_prefix(vt) 
        M_vt_any_right_pos(                  vt                     )    
    
        M_vt_no_eval_right_ident(            vt                     ) 
        M_vt_add(L"IS_IDENTIFIER"          , vt, verb_is_identifier )  
    }


    ////////////////////////////////////////////////////////////

    {
        M_vt_unary_prefix(vt)                                                                                                    
        M_vt_any_right_pos(           vt                        )    

   //   M_vt_no_eval_right_vlist(     vt                        ) 
        M_vt_add(L"IS_VLIST"        , vt, verb_is_vlist         )  
    }
          

    ////////////////////////////////////////////////////////////

    {
        M_vt_unary_prefix(vt) 
        M_vt_any_right_pos(             vt                     )    

   //   M_vt_no_eval_right_vlist(       vt                     ) 
        M_vt_no_eval_right_expression(  vt                     ) 
        M_vt_add(L"IS_EXPRESSION"     , vt, verb_is_expression )  
    }
      
    ////////////////////////////////////////////////////////////

    {
        M_vt_unary_prefix(            vt                             )
        M_vt_any_ident_right_pos(     vt                             )
        M_vt_nval_optional_right_kw ( vt, L"local"                   )
        M_vt_nval_optional_right_kw ( vt, L"verbmain"                )
        M_vt_nval_optional_right_kw ( vt, L"static"                  )
        M_vt_nval_optional_right_kw ( vt, L"global"                  )
        M_vt_nval_optional_right_kw ( vt, L"all"                     ) 
        M_vt_right_conflict_5way(     vt, L"local", L"verbmain", L"global", L"static", L"all") 

        M_vt_no_eval_right_ident(     vt                             )            // no evaluation of right-side identifiers
        M_vt_add(L"IS_VAR"          , vt, verb_is_var                ) 
        M_vt_add(L"IS_CONST"        , vt, verb_is_const              )
        M_vt_add(L"IS_DEF"          , vt, verb_is_def                )
    }
     

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   structure, array, argument and vlist extraction verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    // =============================================================================
    // array # n -- array subscripting  -- return n-th element in array
    // =============================================================================

    {
        M_vt_binary_infix(                vt                         )                                                                  
                                                                   
        M_vt_int64rc_right_pos(           vt, 0, M_int64_max         )  // right-side integer -- range = 0 to N -- array index

        {                                                          
            M_vt_pos_parm(                    pt                     )
         //   pt.eval.no_eval_ident   = true;                             // don't evaluate, if identifier -- need to get at original symval_s, value_S, and buffer, etc.
         //   pt.eval.no_eval_ref     = true;                             // don't evaluate, if ref        -- need to get at original symval_s, value_S, and buffer, etc.
            pt.var_ident_ok         = true;                             // can be variable identifier -- must have array assigned to it currently (checked later)
            pt.const_ident_ok       = true;                             // can be constant identifier -- must have array assigned to it currently (checked later)
            pt.array_ok             = true;                             // can be an array value (after evaluation)
            pt.lvalue_ref_ok        = true;                             // can be an reference (should be to array) -- probably from multiple subscript functions
            pt.rvalue_ref_ok        = true;                             // can be an reference (should be to array) -- probably from multiple subscript functions
                                                                   
            M_vt_add_l_pos(               vt, pt                     )  // left positional parm for array source
        }
       
        M_vt_no_eval_left_ident(          vt                         ) 
        M_vt_no_eval_left_ref(            vt                         ) 
        M_vt_priority(                    vt,verb_pri_N::_subscript  ) 

        M_vt_info(                        vt, L"array # int64_t"     ) 
        M_vt_add(L"#",                    vt, verb_subscript         )
    }
     

    // =============================================================================
    // struct . fieldname -- field selection                                          // need overloaded version that accepts string for fieldname
    // =============================================================================

    {
        M_vt_binary_infix(                vt                         )
                                                                   
                                                                   
        M_vt_raw_ident_right_pos(         vt                         )  // right-side raw identifier = fieldname
                                                                   
        {                                                          
            M_vt_pos_parm(                    pt                     )
       //     pt.eval.no_eval_ident   = true;                             // don't evaluate, if identifier -- need to get at original symval_s, value_S, and buffer, etc.
       //     pt.eval.no_eval_ref     = true;                             // don't evaluate, if ref        -- need to get at original symval_s, value_S, and buffer, etc.
            pt.var_ident_ok         = true;                             // can be variable identifier -- must have struct assigned to it currently (checked later)
            pt.const_ident_ok       = true;                             // can be constant identifier -- must have struct assigned to it currently (checked later)
            pt.structure_ok         = true;                             // can be an struct value (after evaluation)
            pt.lvalue_ref_ok        = true;                             // can be an reference (should be to struct) -- probably from multiple subscript functions
            pt.rvalue_ref_ok        = true;                             // can be an reference (should be to struct) -- probably from multiple subscript functions
                                                                   
            M_vt_add_l_pos(               vt, pt                     )  // left positional parm for array source
        }                                                          
       
        M_vt_no_eval_left_ident(          vt                         ) 
        M_vt_no_eval_left_ref(            vt                         ) 
        M_vt_priority(                    vt, verb_pri_N::_select    ) 

        M_vt_info(                        vt, L"struct . field_name" ) 
        M_vt_add(L".",                    vt, verb_select            )
    }
    

    // ====================================================================================================================
    // @GETENV "name" -- get setting for environment variable "name"
    // ====================================================================================================================

    {
        M_vt_unary_prefix(      vt                              )
        M_vt_string_right_pos(  vt                              )  
                                                              
        M_vt_info(              vt, L"@GETENV «envar name»"     ) 
        M_vt_add(L"GETENV",     vt, verb_getenv                 )
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
       

        M_vt_info(                        vt, L"@ARG_CT    | left: | right: |    |key: «keyword_name» | all_keys |"     ) 
        M_vt_add(L"ARG_CT",               vt, verb_arg_ct)
    }
   

    // =================================================================
    // @ARGS -- return vlist value with either left: or right: arg vlist
    // =================================================================

    {
        M_vt_nofix(        vt)

        M_vt_nval_optional_right_kw(      vt, L"right"          )
        M_vt_nval_optional_right_kw(      vt, L"left"           )  
        M_vt_right_conflict_pair(         vt, L"left", L"right" )
        
        M_vt_info(                        vt, L"@ARGS   | left: | right: |"     ) 
        M_vt_add(L"ARGS",                 vt, verb_args)
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
        M_vt_int64rc_right_pos(           vt, 0, M_int64_max                         )    // range = 0 to N
        M_vt_nval_optional_right_kw(      vt, L"allkeys"                             )
        M_vt_string_optional_right_kw(    vt, L"key"                                 )
        M_vt_nval_optional_right_kw(      vt, L"right"                               )
        M_vt_nval_optional_right_kw(      vt, L"left"                                )  
        M_vt_right_conflict_pair(         vt, L"left", L"right"                      ) 
        M_vt_right_conflict_pair(         vt, L"key" , L"allkeys"                    )  

        M_vt_info(                        vt, L"@ARG    | left: | right: |    |key: «keyword_name» | all_keys |"     ) 
        M_vt_add(L"ARG",                  vt, verb_arg)
    }


#if 0
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

        M_vt_custom_eval(                     vt                                ) 

        M_vt_info(                            vt, L"ident1 ident2 ident3 ...  rest:ident  key:«keyword_name»  @ARG_ASSIGN  ident1 ident2 ident3 ...  rest:ident  key:«keyword_name»"     ) 
        M_vt_add(L"ARG_ASSIGN",               vt, verb_arg_assign               )
    }  
#endif 

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

    // =============================================================================
    // [vlist] @AT n              -- return n-th positional value in vlist
    // [vlist] @AT n allkeys:     -- return n-th keyword value in vlist
    // [vlist] @AT n key:"xxx"    -- return value from n-th occurence of keyword "xxx" in vlist 
    // =============================================================================

    {
        M_vt_binary_infix(                vt                     )
        M_vt_int64rc_right_pos(           vt, 0, M_int64_max     )  // right-side integer -- range = 0 to N
        M_vt_string_optional_right_kw(    vt, L"key"             )
        M_vt_nval_optional_right_kw(      vt, L"allkeys"         )
        M_vt_right_conflict_pair(         vt, L"key" , L"allkeys") 

        M_vt_vlist0_left_pos(             vt                     ) 

        M_vt_priority(                    vt,verb_pri_N::_at     ) 
        M_vt_info(                        vt, L"[vlist] @AT int64_t     |key: «keyword_name» | all_keys |"     ) 
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

        M_vt_info(                        vt, L"@VL_CT [vlist]   |key: «keyword_name» | all_keys |"     ) 
        M_vt_add(L"VL_CT",                vt, verb_vl_ct           )
    }
      

#if 0   
    // ==================================================================================================================
    // v1 v2 v3 ... v999 rest:v0 @VL_ASSIGN [vlist] key:"string" - assign positional/keyword values in vlist to variables
    // ==================================================================================================================

    {
        M_vt_anyfix(                          vt, 1, M_int64_max, 1, 1)
        M_vt_nonconst_ident_left_pos(         vt                      )
        M_vt_nonconst_ident_optional_left_kw( vt, L"rest"             )
        M_vt_string_optional_right_kw(        vt, L"key"              )
        M_vt_vlist0_right_pos(                vt                      )  

        M_vt_custom_eval(                     vt                      ) 

        M_vt_info(                            vt, L"ident1 ident2 ident3 ...  rest:ident @VL_ASSIGN [vlist] key:«keyword_name»"     ) 
        M_vt_add(L"VL_ASSIGN",                vt, verb_vl_assign      )
    } 
#endif


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   output-oriented verbs
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 

    // =======================================================
    // @DISPLAY - display various things (no positional parms)
    // =======================================================

    {
        M_vt_nofix(vt) 
        M_vt_nval_optional_right_kw(  vt, L"locale"              )
        M_vt_nval_optional_right_kw(  vt, L"numerics"            )
        M_vt_nval_optional_right_kw(  vt, L"stack"               )
        M_vt_nval_optional_right_kw(  vt, L"all_vars"            )
        M_vt_nval_optional_right_kw(  vt, L"builtin_verbs"       )
        M_vt_nval_optional_right_kw(  vt, L"added_verbs"         )
        M_vt_nval_optional_right_kw(  vt, L"all_verbs"           )
        M_vt_nval_optional_right_kw(  vt, L"builtin_types"       )
        M_vt_nval_optional_right_kw(  vt, L"added_types"         )
        M_vt_nval_optional_right_kw(  vt, L"all_types"           )
        M_vt_nval_optional_right_kw(  vt, L"id_cache"            )
        M_vt_nval_optional_right_kw(  vt, L"statistics"          )
        M_vt_int64_optional_right_kw( vt, L"token_list"          )  

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


        M_vt_info(           vt, L"@DISPLAY locale: numerics: stack: all_vars: builtin_verbs: added_verbs: all_verbs: builtin_types: added_types: all_types: id_cache: statistics: ..." )
        M_vt_add(L"DISPLAY", vt, verb_display)
    }    

  
    // ==========================
    // @SAY value value value ...
    // @STR value value value ...
    // ==========================

    {
        M_vt_nary_prefix(             vt                        )  
    //    M_vt_pos_parm(                     pt                   )                  
    //        pt.anything_ok                = true;                           ????????                            // left side -- want to accept all parms (evaluated), before discarding the results
    //        M_vt_add_l_pos(           vt, pt)                        ?????????
        M_vt_any_right_pos(           vt                        )      
        M_vt_nval_optional_right_kw(  vt, L"debug"              )
        M_vt_nval_optional_right_kw(  vt, L"debugx"             )
        M_vt_right_conflict_pair(     vt, L"debug" , L"debugx"  )    /// ??? need no_nl optin here ????????? 

        M_vt_info(                    vt, L"@STR value1 value2 ...  < debug: | debugx: >" )
        M_vt_add(L"STR",              vt, verb_str              );        
    }

    {
        M_vt_nary_prefix(             vt                        )      
        M_vt_any_right_pos(           vt                        )     
        M_vt_nval_optional_right_kw(  vt, L"debug"              )
        M_vt_nval_optional_right_kw(  vt, L"debugx"             )
        M_vt_right_conflict_pair(     vt, L"debug" , L"debugx"  ) 
        M_vt_nval_optional_right_kw(  vt, L"no_nl"              )

        M_vt_info(                    vt, L"@SAY value1 value2 ...  < debug: | debugx: >  < no_nl: >" )
        M_vt_add(L"SAY",              vt, verb_say              );       
    }


    // ==========================
    // @SAYQE value value value ...
    // @STRQE value value value ...
    // ==========================

    {
        M_vt_nary_prefix(             vt                        )      

        M_vt_pos_parm(                     pt                   )                  
        pt.anything_ok             = true;                                                      
   //     pt.eval.no_eval_ident      = true; 
  //      pt.eval.no_eval_expression = true;                              // allow expressions to get evaluated
    //    pt.eval.no_eval_vlist      = true; 
    //    pt.eval.no_eval_ref        = true; 
        M_vt_add_r_pos(               vt, pt)

        M_vt_nval_optional_right_kw(  vt, L"debug"              )
        M_vt_nval_optional_right_kw(  vt, L"debugx"             )
        M_vt_right_conflict_pair(     vt, L"debug" , L"debugx"  )    /// ??? need no_nl option here ????????? 

        M_vt_no_eval_right_ident(     vt                        ) 
        M_vt_no_eval_right_vlist(     vt                        ) 
        M_vt_no_eval_right_ref(       vt                        ) 
   //   M_vt_no_eval_right_expression(vt                        )   // allow expressions to get evaluated

        M_vt_info(                    vt, L"@STRQE value1 value2 ...  < debug: | debugx: >" )
        M_vt_add(L"STRQE",            vt, verb_str              );        
    }

    {
        M_vt_nary_prefix(             vt                        ) 

        M_vt_pos_parm(                     pt                   )                  
        pt.anything_ok             = true;                                                      
  //      pt.eval.no_eval_ident      = true; 
  //      pt.eval.no_eval_expression = true;                              // allow expressions to get evaluated
  //      pt.eval.no_eval_vlist      = true; 
  //      pt.eval.no_eval_ref        = true; 
        M_vt_add_r_pos(               vt, pt)
  
        M_vt_nval_optional_right_kw(  vt, L"debug"              )
        M_vt_nval_optional_right_kw(  vt, L"debugx"             )
        M_vt_right_conflict_pair(     vt, L"debug" , L"debugx"  ) 
        M_vt_nval_optional_right_kw(  vt, L"no_nl"              )

        M_vt_no_eval_right_ident(     vt                        ) 
        M_vt_no_eval_right_vlist(     vt                        ) 
        M_vt_no_eval_right_ref(       vt                        ) 
   //   M_vt_no_eval_right_expression(vt                        )   // allow expressions to get evaluated

        M_vt_info(                    vt, L"@SAYQE value1 value2 ...  < debug: | debugx: >  < no_nl: >" )
        M_vt_add(L"SAYQE",            vt, verb_say              );       
    }
  

    // ==========================
    // @SAYQ value value value ...
    // @STRQ value value value ...
    // ==========================

    {
        M_vt_nary_prefix(             vt                        )      

        M_vt_pos_parm(                     pt                   )                  
        pt.anything_ok             = true;                                                      
   //     pt.eval.no_eval_ident      = true; 
   //     pt.eval.no_eval_expression = true; 
   //     pt.eval.no_eval_vlist      = true; 
   //     pt.eval.no_eval_ref        = true; 
        M_vt_add_r_pos(               vt, pt)

        M_vt_nval_optional_right_kw(  vt, L"debug"              )
        M_vt_nval_optional_right_kw(  vt, L"debugx"             )
        M_vt_right_conflict_pair(     vt, L"debug" , L"debugx"  )    /// ??? need no_nl option here ?????????
  
        M_vt_no_eval_right_ident(     vt                        ) 
        M_vt_no_eval_right_vlist(     vt                        ) 
        M_vt_no_eval_right_ref(       vt                        ) 
        M_vt_no_eval_right_expression(vt                        )  

        M_vt_info(                    vt, L"@STRQ value1 value2 ...  < debug: | debugx: >" )
        M_vt_add(L"STRQ",             vt, verb_str              );        
    }

    {
        M_vt_nary_prefix(             vt                        ) 

        M_vt_pos_parm(                     pt                   )                  
        pt.anything_ok             = true;                                                      
   //     pt.eval.no_eval_ident      = true; 
   //     pt.eval.no_eval_expression = true; 
   //     pt.eval.no_eval_vlist      = true; 
   //     pt.eval.no_eval_ref        = true; 
        M_vt_add_r_pos(               vt, pt)
  
        M_vt_nval_optional_right_kw(  vt, L"debug"              )
        M_vt_nval_optional_right_kw(  vt, L"debugx"             )
        M_vt_right_conflict_pair(     vt, L"debug" , L"debugx"  ) 
        M_vt_nval_optional_right_kw(  vt, L"no_nl"              )

        M_vt_no_eval_right_ident(     vt                        ) 
        M_vt_no_eval_right_vlist(     vt                        ) 
        M_vt_no_eval_right_ref(       vt                        ) 
        M_vt_no_eval_right_expression(vt                        )  

        M_vt_info(                    vt, L"@SAYQ value1 value2 ...  < debug: | debugx: >  < no_nl: >" )
        M_vt_add(L"SAYQ",             vt, verb_say              );       
    }

  

    // ===========================================
    // @INTERPOLATE "string"   begin:"{"   end:"}"
    // ===========================================

    {
        M_vt_unary_prefix(                    vt                        )      
        M_vt_string_right_pos(                vt                        )  
        M_vt_string_optional_right_kw(        vt, L"begin"              )  
        M_vt_string_optional_right_kw(        vt, L"end"                )  

        M_vt_info(                            vt, L"@INTERPOLTE «string text»   < begin:'delim > < end:'delim >")
        M_vt_add(L"INTERPOLATE",              vt, verb_interpolate      );        
    }


    // ===================================================================================
    // @TEXT {block}  -- format {BLOCK} to make it printable
    // ===================================================================================

    {
        M_vt_unary_prefix(                    vt                        )      
        M_vt_block_right_pos(                 vt                        ) 

        M_vt_info(                            vt, L"@TEXT {block}"      )
        M_vt_add(L"TEXT",                     vt, verb_text             );        
    }
    

    // ===================================================================================
    // @FORMAT fmt:"string"   value_1 value_2 ... value_N                  begin:"{"   end:"}"
    // ===================================================================================

    {
        M_vt_nary_prefix(                     vt                        )      
        M_vt_any_right_pos(                   vt                        )  
        M_vt_string_required_right_kw(        vt, L"fmt"                ) 

        M_vt_info(                            vt, L"@FORMAT fmt:«format string» value1 value2 ...")
        M_vt_add(L"FORMAT",                   vt, verb_format           );        
    }

  
    // ==========================
    // @STDIN  (no parms)
    // @STDOUT string-parm
    // @STDERR string-parm
    // ==========================

    {
        M_vt_nofix(                       vt                     )
        M_vt_info(                        vt, L"@STDIN"          )
        M_vt_add(L"STDIN",                vt, verb_stdin         );
    }

    {
        M_vt_unary_prefix(                vt                     )
        M_vt_string_right_pos(            vt                     )

        M_vt_info(                        vt, L"@STDOUT «output string»" )
        M_vt_add(L"STDOUT",               vt, verb_stdout        );
    }

    {
        M_vt_unary_prefix(                vt                     )
        M_vt_string_right_pos(            vt                     )
     
        M_vt_info(                        vt, L"@STDERR «output string»" )
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


        M_vt_info(                        vt, L"@DEBUG  |no_verbs:|few_verbs:|some_verbs:|most_verbs:|all_verbs:|" )
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