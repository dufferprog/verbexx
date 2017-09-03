// ex_verb_ctl.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===============
////     ex_verb_ctl.cpp -- parser-oriented pre-defined verbs -- flow of control verbs
////     ===============
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////     @VERB
////     @FN
////     @UNVERB
////     @CALL
////     @XCTL 
////
////     @SHELL
////
////
////
////     @PARSE
////     @SEP   -or-  ,
////
////     @DO
////     @BLOCK
////     @LOOP
////     @IF
////     @CASE
////     @TRY
////
////     @GOTO
////     @LEAVE
////     @RETURN
////     @CONTINUE
////     @BREAK
////     @THROW
////     @RETHROW
////     @QUIT
////     @END
////     @EXIT
////     @_EXIT
////     @QUICK_EXIT
////     @ABORT
////
////
////
////    
////
////    
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Flow-of-control verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    build_verb_argvar() -- helper function verb_verb() -- builds left and right argvars_S structure in verbdef based on passed-in vlists from argvars:
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int build_verb_argvar(frame_S& eval, const e_vexpr_S vexpr, verbdef_S& verbdef, const vlist_S& l_vlist, const vlist_S& r_vlist) try
{
    int rc {0}; 

    M__(M_out(L"build_verb_argvar() -- l_vlist.values.size() = %d    r_vlist.values.size() = %d") % l_vlist.values.size() % r_vlist.values.size();)   

    std::set<std::wstring>  all_vars { };                                        // set to hold all positional and keyword variale names seen so far -- used for duplicate checking


    // put in variable names for left positional parms
    // -----------------------------------------------

    for (auto elem : l_vlist.values)                                             // elem is value_S, with a raw identifier as keyword name 
    {
        M__(M_out(L"build_verb_argvar() -- elem : l_vlist.values -- elem.string = «%S»") % elem.string;)  


        // if this variable name has been seen before (on either side), put out error message 

        if ( all_vars.find(elem.string) != all_vars.end() )
        {
            rc = -1; 
            count_error(); 

            M_out_emsg1(L"build_verb_argvar() -- variable name «%S» in left-side positional var:[ ... ] list appears more than once for this verb definition") % elem.string; 
            msgend_loc(elem, vexpr);
        }
        else                                                                     // haven't seen this variable name yet
        {
            all_vars.insert(elem.string);                                        // remember that we have now seen this variable name        
        }


        // if no error so far, add this variable name to left-side positional variable names list 

        if (rc == 0)
            verbdef.lvars.pos.push_back(elem.string);                            // save away identifier as left-side arg-n variable name      
    }


    // put in variable names for left-side keyword parms
    // -------------------------------------------------

    for (auto elem : l_vlist.eval_kws)                                           // elem.first is wstring (keyword name), elem.second is value_S, holding a raw identifier as keyword name 
    {
        M__(M_out(L"build_verb_argvar() -- elem : l_vlist.eval_kws -- elem.first = «%S»   elem.second.string = «%S»") % elem.first % elem.second.string;)  


        // if this variable name has been seen before (on either side), put out error message 

        if ( all_vars.find(elem.second.string) != all_vars.end() )
        {
            rc = -1; 
            count_error(); 

            M_out_emsg1(L"build_verb_argvar() -- variable name «%S:%S» in left-side keyword var:[ ... ] list appears more than once for this verb definition") % elem.first % elem.second.string; 
            msgend_loc(elem.second, L"name");  
        }
        else                                                                     // haven't seen this variable name yet
        {
            all_vars.insert(elem.second.string);                                 // remember that we have now seen this variable name        
        }


        // if no error so far, add this keyword/variable name to left-side keyword variable names list 

        if (rc == 0)
            verbdef.lvars.key.emplace(elem.first, elem.second.string);           // save away keyword/identifier as left-side keyword variable name      
    }  


    // put in variable names for right positional parms
    // ------------------------------------------------

    for (auto elem : r_vlist.values)                                             // elem is value_S, with a raw identifier as keyword name 
    {
        M__(M_out(L"build_verb_argvar() -- elem : r_vlist.values -- elem.string = «%S»") % elem.string;)  


        // if this variable name has been seen before (on either side), put out error message 

        if ( all_vars.find(elem.string) != all_vars.end() )
        {
            rc = -1; 
            count_error(); 

            M_out_emsg1(L"build_verb_argvar() -- variable name «%S» in right-side positional var:[ ... ] list appears more than once for this verb definition") % elem.string; 
            msg_loc(elem, L"name");  
            msgend_loc(vexpr);
        }
        else                                               // haven't seen this variable name yet
        {
            all_vars.insert(elem.string);                  // remember that we have now seen this variable name        
        }


        // if no error so far, add this variable name to right-side positional variable names list 

        if (rc == 0)
            verbdef.rvars.pos.push_back(elem.string);      // save away identifier as right-side arg-n variable name     
    }


    // put in variable names for right-side keyword parms
    // --------------------------------------------------

    for (auto elem : r_vlist.eval_kws)                                           // elem.first is wstring (keyword name), elem.second is value_S, holding a raw identifier as keyword name 
    {
        M__(M_out(L"build_verb_argvar() -- elem : r_vlist.eval_kws -- elem.first = «%S»   elem.second.string = «%S»") % elem.first % elem.second.string;)  


        // if this variable name has been seen before (on either side), put out error message 

        if ( all_vars.find(elem.second.string) != all_vars.end() )
        {
            rc = -1; 
            count_error(); 

            M_out_emsg1(L"build_verb_argvar() -- variable name «%S:%S» in right-side keyword var:[ ... ] list appears more than once for this verb definition") % elem.first % elem.second.string; 
            msg_loc(elem.second, L"name");     
            msgend_loc(vexpr);
        }
        else                                                                     // haven't seen this variable name yet
        {
            all_vars.insert(elem.second.string);                                 // remember that we have now seen this variable name        
        }


        // if no error so far, add this keyword/variable name to right-side keyword variable names list 

        if (rc == 0)
            verbdef.rvars.key.emplace(elem.first, elem.second.string);           // save away keyword/identifier as left-side keyword variable name      
    }


    // ---------------------------------------------------------------------------------------------------
      
    // pass back good/bad rc -- if error, clear out all argvar_S structures before returning

    if (rc != 0)
    {
        verbdef.lvars.pos.clear(); 
        verbdef.lvars.key.clear();
        verbdef.rvars.pos.clear(); 
        verbdef.rvars.key.clear();      
    }                                

    return rc; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    build_verb_parmtype() -- helper function verb_verb()
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void build_verb_parmtype(frame_S& frame, const e_vexpr_S vexpr, const vlist_S& vlist, parmtype_S& parmtype, bool is_keyword = false) try
{
    parmtype = parmtype_S { }; 
    
    
    // get keyword parms from caller's vlist
    // -------------------------------------

    value_S unwanted_value {};                  // no value from type-parms 

    auto unit_rc         = get_vlist_keyword(vlist, L"unit"            , unwanted_value); // r/c = -1 if not present
    auto int8_rc         = get_vlist_keyword(vlist, L"int8"            , unwanted_value); // r/c = -1 if not present
    auto int16_rc        = get_vlist_keyword(vlist, L"int16"           , unwanted_value); // r/c = -1 if not present
    auto int32_rc        = get_vlist_keyword(vlist, L"int32"           , unwanted_value); // r/c = -1 if not present
    auto int64_rc        = get_vlist_keyword(vlist, L"int64"           , unwanted_value); // r/c = -1 if not present
    auto uint8_rc        = get_vlist_keyword(vlist, L"uint8"           , unwanted_value); // r/c = -1 if not present
    auto uint16_rc       = get_vlist_keyword(vlist, L"uint16"          , unwanted_value); // r/c = -1 if not present
    auto uint32_rc       = get_vlist_keyword(vlist, L"uint32"          , unwanted_value); // r/c = -1 if not present
    auto uint64_rc       = get_vlist_keyword(vlist, L"uint64"          , unwanted_value); // r/c = -1 if not present
    auto float32_rc      = get_vlist_keyword(vlist, L"float32"         , unwanted_value); // r/c = -1 if not present
    auto float64_rc      = get_vlist_keyword(vlist, L"float64"         , unwanted_value); // r/c = -1 if not present
    auto string_rc       = get_vlist_keyword(vlist, L"string"          , unwanted_value); // r/c = -1 if not present
    auto raw_ident_rc    = get_vlist_keyword(vlist, L"raw_ident"       , unwanted_value); // r/c = -1 if not present
    auto var_ident_rc    = get_vlist_keyword(vlist, L"var_ident"       , unwanted_value); // r/c = -1 if not present
    auto const_ident_rc  = get_vlist_keyword(vlist, L"const_ident"     , unwanted_value); // r/c = -1 if not present
    auto undef_ident_rc  = get_vlist_keyword(vlist, L"undef_ident"     , unwanted_value); // r/c = -1 if not present
    auto vlist_rc        = get_vlist_keyword(vlist, L"vlist"           , unwanted_value); // r/c = -1 if not present
    auto vexpr_rc        = get_vlist_keyword(vlist, L"expression"      , unwanted_value); // r/c = -1 if not present
    auto slist_rc        = get_vlist_keyword(vlist, L"slist"           , unwanted_value); // r/c = -1 if not present
    auto verbdef_rc      = get_vlist_keyword(vlist, L"verbdef"         , unwanted_value); // r/c = -1 if not present
    auto typedef_rc      = get_vlist_keyword(vlist, L"typedef"         , unwanted_value); // r/c = -1 if not present
    auto array_rc        = get_vlist_keyword(vlist, L"array"           , unwanted_value); // r/c = -1 if not present
    auto struct_rc       = get_vlist_keyword(vlist, L"struct"          , unwanted_value); // r/c = -1 if not present
    auto ref_rc          = get_vlist_keyword(vlist, L"ref"             , unwanted_value); // r/c = -1 if not present

    int  type_ct = 1 + unit_rc     
                 + 1 + int8_rc      + 1 + int16_rc       + 1 + int32_rc       + 1 + int64_rc 
                 + 1 + uint8_rc     + 1 + uint16_rc      + 1 + uint32_rc      + 1 + uint64_rc 
                 + 1 + float32_rc   + 1 + float64_rc     + 1 + string_rc 
                 + 1 + var_ident_rc + 1 + const_ident_rc + 1 + undef_ident_rc + 1 + vlist_rc + 1 + vexpr_rc  + 1 + slist_rc + 1 + verbdef_rc;    // add up count of type keywords present 


    // evaluation flags

    auto no_eval_ident_rc = get_vlist_keyword(vlist, L"no_eval_ident"       , unwanted_value); // r/c = -1 if not present
    auto no_eval_vexpr_rc = get_vlist_keyword(vlist, L"no_eval_expression"  , unwanted_value); // r/c = -1 if not present
    auto no_eval_vlist_rc = get_vlist_keyword(vlist, L"no_eval_vlist"       , unwanted_value); // r/c = -1 if not present
        

    // arithmetic range parms

    value_S int_min_value   { }; 
    value_S int_max_value   { }; 
    value_S float_min_value { }; 
    value_S float_max_value { }; 

    auto int_min_rc   = get_vlist_keyword(vlist, L"int_min"   , int_min_value  ); // r/c = -1 if not present
    auto int_max_rc   = get_vlist_keyword(vlist, L"int_max"   , int_max_value  ); // r/c = -1 if not present
    auto float_min_rc = get_vlist_keyword(vlist, L"float_min" , float_min_value); // r/c = -1 if not present
    auto float_max_rc = get_vlist_keyword(vlist, L"float_max" , float_max_value); // r/c = -1 if not present

    
    // keyword-only sub-keywords kw_min: and kw_max: -- should not be present for positional vlist

    value_S kw_min_value   { }; 
    value_S kw_max_value   { }; 
    auto kw_min_rc   = get_vlist_keyword(vlist, L"kw_min"   , kw_min_value  ); // r/c = -1 if not present
    auto kw_max_rc   = get_vlist_keyword(vlist, L"kw_max"   , kw_max_value  ); // r/c = -1 if not present
    

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // set basic type flags in this parmtype
                                            
    if (type_ct == 0)                                
        parmtype.anything_ok = true;                    // allow anything, if no type keywords are present
    else                                                // type keywords present -- set individual flags
    {
        if(unit_rc        == 0)  parmtype.unit_ok            = true;  
        if(int8_rc        == 0)  parmtype.int8_ok            = true;
        if(int16_rc       == 0)  parmtype.int16_ok           = true; 
        if(int32_rc       == 0)  parmtype.int32_ok           = true; 
        if(int64_rc       == 0)  parmtype.int64_ok           = true; 
        if(uint8_rc       == 0)  parmtype.uint8_ok           = true; 
        if(uint16_rc      == 0)  parmtype.uint16_ok          = true; 
        if(uint32_rc      == 0)  parmtype.uint32_ok          = true; 
        if(uint64_rc      == 0)  parmtype.uint64_ok          = true; 
        if(float32_rc     == 0)  parmtype.float32_ok         = true; 
        if(float64_rc     == 0)  parmtype.float64_ok         = true; 
        if(string_rc      == 0)  parmtype.string_ok          = true; 
        if(raw_ident_rc   == 0) {parmtype.raw_ident_ok       = true; parmtype.no_eval_ident = true;}
        if(var_ident_rc   == 0) {parmtype.var_ident_ok       = true; parmtype.no_eval_ident = true;} 
        if(const_ident_rc == 0) {parmtype.const_ident_ok     = true; parmtype.no_eval_ident = true;}
        if(undef_ident_rc == 0) {parmtype.undef_ident_ok     = true; parmtype.no_eval_ident = true;}
        if(vlist_rc       == 0)  parmtype.vlist_ok           = true; 
        if(vexpr_rc       == 0)  parmtype.vexpr_ok           = true; 
        if(slist_rc       == 0)  parmtype.slist_ok           = true;    
        if(verbdef_rc     == 0)  parmtype.verbdef_ok         = true;   
        if(typedef_rc     == 0)  parmtype.typdef_ok          = true; 
        if(array_rc       == 0)  parmtype.array_ok           = true; 
        if(struct_rc      == 0)  parmtype.structure_ok       = true; 
        if(ref_rc         == 0)  parmtype.lvalue_ref_ok      = true; 
        if(ref_rc         == 0)  parmtype.rvalue_ref_ok      = true;
    }    


    // set evaluation flags in this parmtype

    if (no_eval_ident_rc == 0)    parmtype.no_eval_ident      = true; 
    if (no_eval_vexpr_rc == 0)    parmtype.no_eval_vexpr      = true; 
    if (no_eval_vlist_rc == 0)    parmtype.no_eval_vlist      = true; 


    // set int and float ranges (even is int/float parms are not allowed) -- note if int_min: is present so is int_max: (same for float_max:) -- these are in kw match sets 

    if (int_min_rc == 0)
    {
        parmtype.int64_range = true; 
        parmtype.int64_min   = std::min(int_min_value.int64, int_max_value.int64); 
        parmtype.int64_max   = std::max(int_min_value.int64, int_max_value.int64);      
    }

    if (float_min_rc == 0)
    {
        parmtype.float64_range = true; 
        parmtype.float64_min   = std::min(float_min_value.float64, float_max_value.float64); 
        parmtype.float64_max   = std::max(float_min_value.float64, float_max_value.float64);    
    }
       

    // set keyword-only fields -- these should not be present for positional parms (due to parm checking) -- note: if min_kw: is present, so is max_kw: 

    if (is_keyword)
    {
        if (kw_min_rc == 0)
        {
            if (kw_max_value.int64 == -1)
            {
                parmtype.kw_min_ct = kw_min_value.int64; 
                parmtype.kw_max_ct = -1;                      // no upper limit
            }
            else
            {
                parmtype.kw_min_ct =  std::min(kw_min_value.int64, kw_max_value.int64);
                parmtype.kw_max_ct =  std::max(kw_min_value.int64, kw_max_value.int64); 
            }
        }      
        else      // no min/max supplied -- use default of 0/1
        {
            parmtype.kw_min_ct = 0; 
            parmtype.kw_max_ct = 1;    
        }
    }

    return; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  
////    @VERB -- verb_verb() -- define non-built-in (user-defined) verb in verb_table
////  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_verb(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& parm_verbdef, results_S& results) try
{
    M__(M_out(L"verb_verb() called");)

    int rc = 0; 
    

    // optional name:"string"    keyword may      be present on right side                    -- present = add to symbol table -- absent = don't add (just return completed verb as results)
    // required block:{slist}    keyword known to be present on right side 
    // optional global:/local:   keyword on right side (local: is default)                   -- ignored if name: is not present
    // optional export:          keyword on right side  (assumed if global: present)         -- ignored if name: is not present
    // optional lexical_scope:   keyword on right side                                       -- default is lexical_scope:, if dynamic_scope: not present
    // optional dynamic_scope:   keyword on right side  
    // optional block_scope:     keyword on right side  
   
    value_S name_value  { }; 
    value_S block_value { };

    get_right_keyword(vexpr, L"block", block_value);                         // block: keyword is always present 

    auto name_rc    = get_right_keyword(vexpr, L"name",  name_value );       // name:  keyword is optional 
    auto global_rc  = get_right_keyword(vexpr, L"global"            );       // global_rc  = -1, if global:  is not present (i.e. local: or defaulted to local)
    auto export_rc  = get_right_keyword(vexpr, L"export"            );       // export_rc  = -1, if export: is not present (meaning no export) 
    auto dynamic_rc = get_right_keyword(vexpr, L"dynamic_scope"     );       // dynamic_rc = -1, if dynamic_scope: is not present 
    auto block_rc   = get_right_keyword(vexpr, L"block_scope"       );       // blokc_rc   = -1, if block_scope: is not present  


    // optional min: and max: int64 keywords on both sides = number of left/right-side positional parms allowed

    value_S l_min_value   { };
    value_S l_max_value   { };
    value_S r_min_value   { };
    value_S r_max_value   { };
    auto l_min_rc = get_left_keyword( vexpr, L"min", l_min_value);           // r/c = -1 if not present
    auto l_max_rc = get_left_keyword( vexpr, L"max", l_max_value);           // r/c = -1 if not present
    auto r_min_rc = get_right_keyword(vexpr, L"min", r_min_value);           // r/c = -1 if not present
    auto r_max_rc = get_right_keyword(vexpr, L"max", r_max_value);           // r/c = -1 if not present


    // if name was supplied, make sure it is a valid verbanme

    if (name_rc == 0)
    {
        if (!is_valid_verbname(name_value.string))
        {
            M_out_emsg1(L"verb_verb() -- error occurred when converting from string = «%s» to verbname") % name_value.string; 
            M_out(L"       name location -- %s") % value_loc_str(name_value);  
            msgend_loc(vexpr); 
      
            results = error_results();    // return error results 
            return -1;                    // failure r/c
        }
    }   


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////


    verbdef_S verbdef {} ;                 // verbdef_S for new user-defined verb
   

    // allocate new non-autodata slist_S on heap, anchor in verbdef_S, and copy passed-in slist_S into new slist_S 

    verbdef.slist_sp.reset(new slist_S {}); 
    *(verbdef.slist_sp) = *(block_value.slist_sp);


    /// set up min/max positional parm counts on each side  -- note: if min: present on a side, so is max: -- match_pairs on each side

    if (l_min_rc == 0)
    {
        if (l_max_value.int64 == -1)
            verbdef.lparms.min_ct = l_min_value.int64;
        else
            verbdef.lparms.min_ct = std::min(l_min_value.int64, l_max_value.int64);
    }
    else
        verbdef.lparms.min_ct = vexpr.lparms.value_ct;       

    if (l_max_rc == 0)
    {
        if (l_max_value.int64 == -1)
            verbdef.lparms.max_ct = -1;  
        else
            verbdef.lparms.max_ct = std::max(l_min_value.int64, l_max_value.int64);            // max: present -- use explicit value
    }                                                                                      
    else                                                                                       // max: not present
    {                                                                                      
        if (vexpr.lparms.value_ct > 0)                                                         // one or more positional parmtypes present?
            verbdef.lparms.max_ct = vexpr.lparms.value_ct;                                     // yes -- default max: = number of parmtypes
        else                                                                               
            verbdef.lparms.max_ct = 0;                                                         // assume none allowed, if no positional parmtypes
    }

    // - - - - - - - - 

    if (r_min_rc == 0)
    {
        if (r_max_value.int64 == -1)
            verbdef.rparms.min_ct = r_min_value.int64;
        else
            verbdef.rparms.min_ct = std::min(r_min_value.int64, r_max_value.int64);
    }
    else
        verbdef.rparms.min_ct = vexpr.rparms.value_ct; 

    if (r_max_rc == 0)
    {
        if (r_max_value.int64 == -1)
            verbdef.rparms.max_ct = -1;  
        else
            verbdef.rparms.max_ct = std::max(r_min_value.int64, r_max_value.int64);            // max: present -- use explicit value
    }                                                                                     
    else                                                                                       // max: not present
    {                                                                                     
        if (vexpr.rparms.value_ct > 0)                                                         // one or more positional parmtypes present?
            verbdef.rparms.max_ct = vexpr.rparms.value_ct;                                     // yes -- default max: = number of parmtypes
        else                                                                              
            verbdef.rparms.max_ct = 0;                                                         // assume none allowed, if no positional parmtypes
    }



    // set up individual left-side positional parms (if any are present)
    // -----------------------------------------------------------------

    if (vexpr.lparms.value_ct > 0)
    {
        parmtype_S parmtype {};

        // loop through the left-side positional vlist parms

        for (const auto& value : vexpr.lparms.values)
        {
            build_verb_parmtype(frame, vexpr, *(value.vlist_sp), parmtype);                    // build parmtype_S from keywords in n-th left-side vlist parm
            verbdef.lparms.values.push_back(parmtype);                                         // add parmtype_S to left-side verbdef plist
        }
    }


    // set up individual right-side positional parms (if any are present)
    // ------------------------------------------------------------------

    if (vexpr.rparms.value_ct > 0)
    {
        parmtype_S parmtype {};

        // loop through the right-side positional vlist parms

        for (const auto& value : vexpr.rparms.values)
        {
            build_verb_parmtype(frame, vexpr, *(value.vlist_sp), parmtype);                    // build parmtype_S from keywords in n-th right-side vlist parm
            verbdef.rparms.values.push_back(parmtype);                                         // add parmtype_S to right-side verbdef plist
        }
    }


    // set up individual left-side key: keyword parms (if any are present)
    // -------------------------------------------------------------------

    auto lkey_ct = vexpr.lparms.eval_kws.count(L"key");

    if (lkey_ct > 0)
    {
        parmtype_S parmtype {};


        // loop through the left-side key: keywords

        for (auto i = 0; i < lkey_ct; i++)
        {
            // get keyword name and parmtype for adding to plist 

            value_S pos0_value { };                                                            // raw keyword value from n-th instance of key:  
            auto key_value = multimap_at(vexpr.lparms.eval_kws, std::wstring(L"key"), i);      // get n-th key: parm's value 
            get_vlist_positional(*(key_value.vlist_sp), pos0_value, 0);                        // get first positional value for this instance of key:    
            build_verb_parmtype(frame, vexpr, *(key_value.vlist_sp), parmtype, true);          // build parmtype_S from keywords in n-th left-side vlist parm

            // add keyword/parmtype pair to keywords map, if not alredy present

            if (verbdef.lparms.keywords.count(pos0_value.string) > 0)                          // this keyword name already in map? 
            {
                M_out_emsg1(L"verb_verb() -- left-side keyword «%s» has already been added for verb «%s»") % pos0_value.string % name_value.string; 
                msgend_loc(vexpr);
                rc = -1;                              // set error r/c but keep going
            }
            else
            {
                verbdef.lparms.keywords.emplace(pos0_value.string, parmtype);                 // add parmtype_S to left-side keywords multimap   // shouldn't have duplicates ??????????????????????
            }
        }
    }
  

    // set up individual right-side key: keyword parms (if any are present)
    // --------------------------------------------------------------------

    auto rkey_ct = vexpr.rparms.eval_kws.count(L"key");

    if (rkey_ct > 0)
    {
        parmtype_S parmtype {};


        // loop through the left-side key: keywords

        for (auto i = 0; i < rkey_ct; i++)
        {
            // get keyword name and parmtype for adding to plist 

            value_S pos0_value { };                                                           // raw keyword value from n-th instance of key:  
            auto key_value = multimap_at(vexpr.rparms.eval_kws, std::wstring(L"key"), i);     // get n-th key: parm's value 
            get_vlist_positional(*(key_value.vlist_sp), pos0_value, 0);                       // get first positional value for this instance of key:    
            build_verb_parmtype(frame, vexpr, *(key_value.vlist_sp), parmtype, true);         // build parmtype_S from keywords in n-th right-side vlist parm

            // add keyword/parmtype pair to keywords map, if not alredy present

            if (verbdef.rparms.keywords.count(pos0_value.string) > 0)                         // this keyword name already in map? 
            {
                M_out_emsg1(L"verb_verb() -- right-side keyword «%s» has already been added for verb «%s»") % pos0_value.string % name_value.string; 
                msgend_loc(vexpr);
                rc = -1;                              // set error r/c but keep going
            }
            else
            {
                verbdef.rparms.keywords.emplace(pos0_value.string, parmtype);                // add parmtype_S to right-side keywords multimap   // shouldn't have duplicates ??????????????????????
            }
        }
    }


    // set up argvar_S values in verbdef_S -- keft/right variable names for parms
    // --------------------------------------------------------------------------

    value_S l_var_value { }; 
    value_S r_var_value { };
    vlist_S l_var_vlist { };
    vlist_S r_var_vlist { };  

    auto l_var_rc = get_left_keyword(  vexpr, L"var", l_var_value);                           // r/c = -1 if not present -- value should have vlist, if present
    auto r_var_rc = get_right_keyword( vexpr, L"var", r_var_value);                           // r/c = -1 if not present -- value should have vlist, if present

    if ( (l_var_rc == 0) && (l_var_value.vlist_sp.get() != nullptr) )
        l_var_vlist = *(l_var_value.vlist_sp);

    if ( (r_var_rc == 0) && (r_var_value.vlist_sp.get() != nullptr) )
        r_var_vlist = *(r_var_value.vlist_sp);
    
    auto avrc = build_verb_argvar(frame, vexpr, verbdef, l_var_vlist, r_var_vlist);  
    if (avrc != 0)
        rc = -1;   


    // return with error and error results if any problem occurred so far

    if (rc != 0)
    {
        results = error_results(); 
        return rc; 
    }   


    // set up pointer to non-builtin verb block: evaluation function
    
    verbdef.fcn_p      = &verb_non_builtin;


    // struct scoping info in verbdef for use when block is executed -- note: if neither dynamic_scope: nor block_scope: was present, default is lexical scope  (maximum of 1 of block_scope:, dynamic_scope:, or lexical_scope: can appear)
    // -------------------------------------------------------------

    if (dynamic_rc == 0)
    {
        verbdef.dynamic_scope = true;                                                        // dynamic scope only if  dynamic_scope:  keyword is present
    }
    else if (block_rc == 0)
    {
         verbdef.block_scope = true;                                                         // block scope only if  block_scope:  keyword is present     
    }
    else  // must be lexical scope
    {
        verbdef.lexical_scope = true;                                                        // lexical scope if dynamic_scope: was not present (lexical_scope: can be present or not -- default is lexical_scope)
        verbdef.upward_scope_sp = std::shared_ptr<frame_S> { frame.self_wp };                // for lexical upward scoping, save shared pointer to stack frame where verb is defined (i.e. current frame_S)
                                                                                             //    note: this should prevent the defining scope from going away if the function that defined this verb returns and the verb definition still exists somewhere 
                                                                                             //    note: this verb definition becomes a closure in this case 
    }
        

    // see if name:«string» parm was supplied  -- if so, need to add verb to local or global symbol table
    // --------------------------------------------------------------------------------------------------

    if (name_rc == 0)
    {    
        // add new non-built-in verb to verb table -- call verb_non-builtin() function when verb is evaluated 

        if (global_rc != 0)                                                                  // global: is not present, meaning local: keyword is present -or- defaulted?
        {
            // local verb definition -- define (non-builtin) local verb -- exported or not    
      
            auto dvrc = def_local_verb(frame, name_value.string, verbdef, false, (export_rc == 0) ? true : false); 
            if (dvrc != 0)
            {
                M_out_emsg1(L"verb_verb() -- error r/c from def_local_verb() -- adding local verb=%s") % name_value.string; 
                msgend_loc(vexpr);
            
                results = error_results();                                                   // pass back error results
                return -1;                                                                   // return with error r/c                                                     
            } 
        }
        else
        {
            // must be global: -- define (non-builtin) global verb -- note that export: is assumed (global identifiers are always exported)
      
            auto dvrc = def_global_verb(name_value.string, verbdef); 
            if (dvrc != 0)
            {
                M_out_emsg1(L"verb_verb() -- error r/c from def_global_verb() -- adding global verb=%s") % name_value.string; 
                msgend_loc(vexpr);
            
                results = error_results();                                                   // pass back error results
                return -1;                                                                   // return with error r/c                                                     
            } 
        }
    }

    // new verb definition created -- return normally with verbdef_S from verb

    results =  to_results(verbdef_val(verbdef));     
    return 0; 
}
M_endf   


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    verb_non_builtin() -- run user-defined verb
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_non_builtin(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_non_builtin() called");)


    // make sure slist pointer is non-zero 

    if (verbdef.slist_sp == nullptr)
    {
        M_out_emsg1(L"verb_non_builtin() -- verbdef.slisp_sp is nullptr for verb=%s -- should not occur") % verb_name(vexpr);
        msgend_loc(vexpr);
        M_throw_v("verb_non_builtin() -- verbdef.slist_sp is nullptr for verb=%s") % out_ws(verb_name(vexpr)) ));  
    }
       

    // run user-provided slist in new block, with parms from the vexpr   

    return eval_verb_block(frame, vexpr, verbdef, results); 
}
M_endf
 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////    @FN -- verb_fn() -- simplified way to define non-built-in (user-defined) verb in verb_table
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////  

int verb_fn(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& parm_verbdef, results_S& results) try
{
    M__(M_out(L"verb_fn() called");)

    int rc = 0; 
    

    // optional left-side undefined identifier -- verb name (unless anonymous function)      -- present = add to symbol table -- absent = don't add (just return completed verb as results)
    // required right-side vlist (may be empty) with argument variable names
    // required right-side slist with function body for this verb
  
    // optional global:/local:   keyword on right side (local: is default)                   -- ignored if name: is not present
    // optional export:          keyword on right side  (assumed if global: present)         -- ignored if name: is not present
    // optional lexical_scope:   keyword on right side                                       -- default is lexical_scope:, if dynamic_scope: not present
    // optional dynamic_scope:   keyword on right side  
   
    value_S name_value  { };                                                      // name identifier -- should be undefined and valid name -- value is in name_value.string field
    value_S block_value { };     
    value_S r_var_value { };                                                      // should be vlist with variable names (may be empty)
    vlist_S r_var_vlist { };                                                      // extracted vlist from r_var_value 
    vlist_S l_var_vlist { };                                                      // always empty -- verbs defined with @FN don't have left-side parms
    int64_t r_var_ct    {0};                                                      // nimber of right positional parms required

    auto name_rc    = get_left_positional( vexpr, name_value        , 0);         // name_rc = -1, if no function name supplied  
    auto r_var_rc   = get_right_positional(vexpr, r_var_value       , 0);         // argvars vlist shold always be present, but may be empty
    auto slist_rc   = get_right_positional(vexpr, block_value       , 1);         // function code block should always be present  
    auto global_rc  = get_right_keyword(   vexpr, L"global"            );         // global_rc  = -1, if global:  is not present (i.e. local: or defaulted to local)
    auto export_rc  = get_right_keyword(   vexpr, L"export"            );         // export_rc  = -1, if export: is not present (meaning no export) 
    auto dynamic_rc = get_right_keyword(   vexpr, L"dynamic_scope"     );         // dynamic_rc = -1, if dynamic_scope: is not present 
    auto block_rc   = get_right_keyword(   vexpr, L"block_scope"       );         // blokc_rc   = -1, if block_scope: is not present  
                                                                              
    if ( (r_var_rc == 0) && (r_var_value.vlist_sp.get() != nullptr ) )
    {
        r_var_ct    = r_var_value.vlist_sp->values.size();                        // number of args expected
        r_var_vlist = *(r_var_value.vlist_sp);                                    // vlist with arg var names
    }      


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////


    verbdef_S verbdef {} ;                 // verbdef_S for new user-defined verb
   

    // allocate new non-autodata slist_S on heap, anchor in verbdef_S, and copy passed-in slist_S into new slist_S 

    verbdef.slist_sp.reset(new slist_S {}); 
    *(verbdef.slist_sp) = *(block_value.slist_sp);


    /// set up min/max positional parm counts on each side based on number of arg vars present

    verbdef.lparms.min_ct = 0;
    verbdef.lparms.max_ct = 0;
    verbdef.rparms.min_ct = r_var_ct; 
    verbdef.rparms.max_ct = r_var_ct;

    auto avrc = build_verb_argvar(frame, vexpr, verbdef, l_var_vlist, r_var_vlist);  
    if (avrc != 0)
        rc = -1;   


    // return with error results if any problem occurred so far

    if (rc != 0)
    {
        results = error_results(); 
        return rc; 
    }   


    // set up pointer to non-builtin verb block: evaluation function
    
    verbdef.fcn_p      = &verb_non_builtin;


    // record scoping info in verbdef for use when block is executed -- note: if neither dynamic_scope: nor block_scope: was present, default is lexical scope  (maximum of 1 of block_scope:, dynamic_scope:, or lexical_scope: can appear)
    // -------------------------------------------------------------

    if (dynamic_rc == 0)
    {
        verbdef.dynamic_scope = true;                                                        // dynamic scope only if  dynamic_scope:  keyword is present
    }
    else if (block_rc == 0)
    {
         verbdef.block_scope = true;                                                         // block scope only if  block_scope:  keyword is present     
    }
    else  // must be lexical scope
    {
        verbdef.lexical_scope = true;                                                        // lexical scope if dynamic_scope: was not present (lexical_scope: can be present or not -- default is lexical_scope)
        verbdef.upward_scope_sp = std::shared_ptr<frame_S> { frame.self_wp };                // for lexical upward scoping, save shared pointer to stack frame where verb is defined (i.e. current frame_S)
                                                                                             //    note: this should prevent the defining scope from going away if the function that defined this verb returns and the verb definition still exists somewhere 
                                                                                             //    note: this verb definition becomes a closure in this case 
    }
        

    // see if function name was supplied  -- if so, need to add verb to local or global symbol table
    // ---------------------------------------------------------------------------------------------

    if (name_rc == 0)
    {    
        // add new non-built-in verb to verb table -- call verb_non-builtin() function when verb is evaluated 

        if (global_rc != 0)                                                                  // global: is not present, meaning local: keyword is present -or- defaulted?
        {
            // local verb definition -- define (non-builtin) local verb -- exported or not    
      
            auto dvrc = def_local_verb(frame, name_value.string, verbdef, false, (export_rc == 0) ? true : false); 
            if (dvrc != 0)
            {
                M_out_emsg1(L"verb_fn() -- error r/c from def_local_verb() -- adding local verb=%s") % name_value.string; 
                msgend_loc(vexpr);
            
                results = error_results();                                                   // pass back error results
                return -1;                                                                   // return with error r/c                                                     
            } 
        }
        else
        {
            // must be global: -- define (non-builtin) global verb -- note that export: is assumed (global identifiers are always exported)
      
            auto dvrc = def_global_verb(name_value.string, verbdef); 
            if (dvrc != 0)
            {
                M_out_emsg1(L"verb_fn() -- error r/c from def_global_verb() -- adding global verb=%s") % name_value.string; 
                msgend_loc(vexpr);
            
                results = error_results();                                                   // pass back error results
                return -1;                                                                   // return with error r/c                                                     
            } 
        }
    }


    // new verb definition created -- return normally with verbdef_S from verb

    results =  to_results(verbdef_val(verbdef));     
    return 0; 
}
M_endf  




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @UNVERB "string" "string" ...
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_unverb(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional string parms  -- optional local: or global: parms (mutually exclusve)
    M__(M_out(L"verb_unverb() called");)
    int rc {0}; 


    // get global: keyword   (default is local:)
    // -------------------

    auto global_rc  = get_right_keyword(vexpr, L"global");   // r/c = -1, if global:  keyword is not present, r/c = 0 if present



    // -----------------------------------------
    // remove verbs in loop, if they are defined 
    // -----------------------------------------

    for (const auto& val : vexpr.rparms.values)
    {
        M__(M_out(L"verb_unverb() -- range for -- val.string = %s") % val.string; )
             

        if (global_rc == 0)
        {
            // remove global verb
            // ------------------

            if (is_global_identifier_defined(val.string))               // see if this identifier is currently defined -- global scope only
            {
                if (!is_global_identifier_verb(val.string))
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove global verb = %s, because global identifier %s is not a verb") % val.string % val.string;
                     msgend_loc(val, vexpr);
                     rc = -1; 
                }
          
                if (!is_global_identifier_builtin(val.string))
                {
                    auto rrc = undef_global_verb(val.string);           // remove verb from verb table
                   
                    // errors are unexpected here
                   
                    if (rrc != 0)
                    {
                        //count_error(); already counted in undef_global_verb()
                        M_out_emsg1(L"verb_unverb() -- unexpected error from undef_global_verb() -- unable to undefine verb = %s") % val.string;
                        msgend_loc(val, vexpr);
                        rc = -1;  
                    } 
                }
                else
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove built-in global verb = %s") % val.string;
                     msgend_loc(val, vexpr);
                     rc = -1; 
                }
            } 
            else
            {
                count_error(); 
                M_out_emsg1(L"verb_unverb() -- cannot remove global verb = %s, because global identifier %s is not defined") % val.string % val.string;
                msgend_loc(val, vexpr);
                rc = -1;        
            }
        }
        else
        {
            // remove verb from local scope
            // ----------------------------
        
            if (is_local_identifier_defined(frame, val.string))               // see if this identifier is currently defined -- local scope only
            {
                if (!is_local_identifier_verb(frame, val.string))
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove local verb = %s, because identifier %s in local scope is not a verb") % val.string % val.string;
                     msgend_loc(val, vexpr);
                     rc = -1; 
                }
          
                if (!is_local_identifier_builtin(frame, val.string))          // local verbs should never be built-in ????
                {
                    auto rrc = undef_local_verb(frame, val.string);           // remove verb from verb table
                   
                    // errors are unexpected here
                   
                    if (rrc != 0)
                    {
                        //count_error(); already counted in undef_local_verb()
                        M_out_emsg1(L"verb_unverb() -- unexpected error from undef_local_verb() -- unable to undefine verb = %s") % val.string;
                        msgend_loc(val, vexpr);
                        rc = -1;  
                    } 
                }
                else                                                        // shouldn't get here
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove local built-in local verb = %s") % val.string;
                     msgend_loc(val, vexpr);
                     rc = -1; 
                }
            } 
            else
            {
                count_error(); 
                M_out_emsg1(L"verb_unverb() -- cannot remove local verb = %s, because identifier %s is not defined in local scope") % val.string % val.string;
                msgend_loc(val, vexpr);
                rc = -1;        
            }
        }
    }            // end of for() loop 


    // return normally, or with error

    if (rc != 0) 
        results = error_results();                                     // output results = error results    
    else
        results = unit_results();                                      // output results = unit results
   
    return rc; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    make_vexpr() -- common code for @CALL and @XCTL 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

a_vexpr_S make_vexpr(frame_S& frame, const e_vexpr_S& vexpr) try
{

    // set up simulated token with verb name  -- selected fields only (type, sigils are unknown) -- location from verbname string parm 

    token_C token {                           };
    value_S value { vexpr.rparms.values.at(0) };                         // extract verb name string from right parmlist
    token.utype2 = tok_u2_E::verbname;                                   // type of simulated token is verbname (has no sigil) 
    token.str    = token.orig_str = value.string;                        // copy verb name string into token
                                                                      
    if (value.token_ix1 != -1)                                          
    {                                                                 
        token_C token1 { token_list_at(value.token_ix1) };               // extract 1st token for verbname string 
                                                                      
        token.lineno1    = token1.lineno1;                               // copy starting location info from string value token 
        token.linepos1   = token1.linepos1;                              // copy starting location info from string value token 
        token.source_id1 = token1.source_id1;                            // copy starting location info from string value token
                                                                      
        if (value.token_ix2 == -1)                                       // if 2nd token is not known, get ending info from 1st token
        {                                                             
            token.lineno2    = token1.lineno2;                           // copy ending location info from string value token 
            token.linepos2   = token1.linepos2;                          // copy ending location info from string value token 
            token.source_id2 = token1.source_id2;                        // copy ending location info from string value token         
        } 
    }

    if (value.token_ix2 != -1)                                        
    {
        token_C token2 { token_list_at(value.token_ix2) };               // extract 2nd token for verbname string 

        token.lineno2    = token2.lineno2;                               // copy ending location info from string value token 
        token.linepos2   = token2.linepos2;                              // copy ending location info from string value token 
        token.source_id2 = token2.source_id2;                            // copy ending location info from string value token

        if (value.token_ix1 == -1)                                       // if 1st token is not known, get starting info from 2nd token
        {
            token.lineno1    = token2.lineno1;                           // copy starting location info from string value token 
            token.linepos1   = token2.linepos1;                          // copy starting location info from string value token 
            token.source_id1 = token2.source_id1;                        // copy starting location info from string value token         
        } 
    }


    // add verb name to new simulated vexpr, using simulated token just constructed 

    a_vexpr_S simulated_vexpr { };                                        // simulated vexpr for calling verb
    verbdef_S verbdef { };                                                // empty verbdef for simulated vexpr 
    vexpr_set_verb(frame, simulated_vexpr, token, verbdef);               // set verb and related fields in new simulated vexpr 
    simulated_vexpr.verb_value.token_ix1 = value.token_ix1;               // replace inaccurate verb token index with token index from verb name string in right parms 
    simulated_vexpr.verb_value.token_ix2 = value.token_ix2;               // replace inaccurate verb token index with token index from verb name string in right parms 


    // get left and right vlists, if they are supplied -- add them to simulated vexpr 

    value_S left_value  { };                                              // should be vlist, or no value 
    value_S right_value { };                                              // should be vlist, or no value 
    auto lrc = get_right_keyword(vexpr, L"left" ,  left_value);           // r/c = -1, if left: not present 
    auto rrc = get_right_keyword(vexpr, L"right", right_value);           // r/c = -1, if left: not present 
                                                                       
    if (lrc == 0)                                                      
        simulated_vexpr.lparms = *(left_value.vlist_sp);                  // get non-empty vlist from value 
    if (rrc == 0)                                                    
        simulated_vexpr.rparms = *(right_value.vlist_sp);                 // get non-empty vlist from value 


    //  try to get best token indexes for vexpr based on vlists and verb name string

    simulated_vexpr.token_ix1 = simulated_vexpr.verb_value.token_ix1; 
    simulated_vexpr.token_ix2 = simulated_vexpr.verb_value.token_ix2; 


    if ( (vexpr.lparms.token_ix1 >= 0) && (vexpr.rparms.token_ix1 >= 0) )
    {
        simulated_vexpr.token_ix1 = std::min(simulated_vexpr.token_ix1, vexpr.lparms.token_ix1);
        simulated_vexpr.token_ix1 = std::min(simulated_vexpr.token_ix1, vexpr.rparms.token_ix1);     
    }
    else if (vexpr.lparms.token_ix1 >= 0)
    {
        simulated_vexpr.token_ix1 = std::min(simulated_vexpr.token_ix1, vexpr.lparms.token_ix1);
    }
    else if (vexpr.rparms.token_ix1 >= 0)
    {
        simulated_vexpr.token_ix1 = std::min(simulated_vexpr.token_ix1, vexpr.rparms.token_ix1);
    }
     
    if ( (vexpr.lparms.token_ix2 >= 0) && (vexpr.rparms.token_ix2 >= 0) )
    {
        simulated_vexpr.token_ix2 = std::max(simulated_vexpr.token_ix2, vexpr.lparms.token_ix2);
        simulated_vexpr.token_ix2 = std::max(simulated_vexpr.token_ix2, vexpr.rparms.token_ix2);     
    }
    else if (vexpr.lparms.token_ix2 >= 0)
    {
        simulated_vexpr.token_ix2 = std::max(simulated_vexpr.token_ix2, vexpr.lparms.token_ix2);
    }
    else if (vexpr.rparms.token_ix2 >= 0)
    {
        simulated_vexpr.token_ix2 = std::max(simulated_vexpr.token_ix2, vexpr.rparms.token_ix2);
    }
  
    M__(display_vexpr(simulated_vexpr, L"verb_call");)


    //  return with completed simulated vexpr

    return simulated_vexpr; 
}
M_endf




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @CALL "string" left:[vlist] right:[vlist] -- call verb "string" with left and right vlists, as supplied 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_call(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional string parms
    M__(M_out(L"verb_call() called");)


    // create simulated vexpr based on @CALL input parms

    a_vexpr_S simulated_vexpr  { make_vexpr(frame, vexpr) };
    results_S call_results     {                          }; 

     
    // execute verb, using new simulated vexpr  
 
    auto erc = eval_vexpr(frame, simulated_vexpr, call_results);

    
    // return normally, or with error

    if (erc != 0) 
        results = error_results();                                     // output results = error    
    else
        results = call_results;                                        // output results are from eval_vexpr -- may have special results (none are hendled here)
   
    return erc; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @XCTL "string" left:[vlist] right:[vlist] -- set up to xfer to verb "string" with left and right vlists, as supplied 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_xctl(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional string parms
    M__(M_out(L"verb_xctl() called");)
 
 

    // create simulated vexpr based on @CALL input parms

    a_vexpr_S simulated_vexpr { make_vexpr(frame, vexpr) };


    // set up xctl special results, using new simulated vexpr as the results value + xctl flag to set up deferred call  

    results = to_results(vexpr_val(simulated_vexpr));          // set up results with copied-over simulated vexpr
   
    results.special_results = true;                            // indicate special flags are set
    results.xctl_flag       = true;                            // cause any nested blocks in verb block to end, then do verb call with simulated vexpr
    
    M__(M_out(L"verb_xctl() returning after setting up xctl results");)
    

    // return with special xctl results

    return 0; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @PARSE "string" name:"debug name string" -- parse "string"into slist_S                                                                                                            
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_parse(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional string parm, and an optional name:"string" keyword/value 
    M__(M_out(L"verb_parse() called");)
 

    // extract string to be parsed, and debugging name from input parms
    // ----------------------------------------------------------------
    
    std::wstring parse_ws = M_get_right_pos_string(vexpr, 0);            

    std::wstring debug_name { L"parsed string" };
    M_get_right_keyword_string(vexpr, L"name", debug_name);  

    bool continue_running { false };                                                                          // default = end the run if parsing error is found
    M_get_right_keyword_nval(vexpr, L"continue", continue_running, true)                                      // don't end the run if parsing error is found, if continue: keyword is present
                                                                                                             
                                                                                                             
    slist_S out_slist{ };                                                                                     // slist_s to be filled in by parse_string()  
    auto prc = parse_string(*get_main_preparse(), frame, out_slist, parse_ws, debug_name, continue_running);  // if "continue"option, make sure to refresh eevrything after any error


    // if error occurred, pass back normal results or error results, based on continue: keyword 
    // ----------------------------------------------------------------------------------------

    if (prc != 0)
    {
        M_y(M_out(L"verb_parse() -- error rc from parse_string()");)

        if (!continue_running)
        {
            results = error_results(); 
            M_y(M_out(L"verb_parse() -- returning with R/C = -1 and error results");)
            return -1;
        }
    }


    // no error or ignored error -- pass back results with slist_S prom parsing (will be empty slist_S after error)
    // ------------------------------------------------------------------------

    results = to_results(slist_val(out_slist));      
    return 0; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @SHELL "command string"   
//    @SHELL "command string" capture: 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_shell(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional string parm, and one optional capture: keyword
    M__(M_out(L"verb_shell() called");)
 

    // extract shell command string, and optional capture: keyword 
    // -----------------------------------------------------------
    
    std::wstring cmd_string = M_get_right_pos_string(vexpr, 0);            

    bool capture { false };                                           // default is to not return the output in a string
    M_get_right_keyword_nval(vexpr, L"capture", capture, true)       // use do_popen() and return shell output as string, if capture: keyword is present


    // execute shell command using do_popen(), if captured output is required
    // ----------------------------------------------------------------------

    if (capture)
    {
        std::wstring output_string { }; 
        auto prc = do_popen(cmd_string, output_string);   
    

        // if popen() error occurred, pass back error results 
        // --------------------------------------------------

        if (prc != 0)
        {
            results = error_results(); 
            return -1; 
        }


        // otherwise, pass back string output from shell command
        // -----------------------------------------------------

        results = to_results(string_val(output_string)); 
        return 0;        
    }


    // no captured output -- execute shell command using do_system()
    // -------------------------------------------------------------

    else
    {     
        auto src = do_system(cmd_string);
        results = to_results(int64_val((int64_t)src));       // results are r/c from system()
        return 0; 
    }
}
M_endf
    


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    ,  @SEP -- separate verb
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_separate(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are 1-N left-side and 1-N right-side positional parms of any type 

    M__(M_out(L"verb_separate() called");)

    vlist_S ret_vlist  {  };                                                                   // start with empty vlist as return value 

    auto c_rc = combine_vlists(ret_vlist, vexpr.lparms, vexpr.rparms, false);                  // output =  left parms + right parms (verbatim flag = false -- get kws from (l/r)parms.eval_kws, but put into ret_vlist.keywords) 
    if (c_rc != 0)
    {
        M_out_emsg1(L"verb_separate() -- unexpected error from combine_vlists() -- separator verb (\",\") failed"); 
        M_out(L"     lparms location -- %s" ) %  vlist_loc_str(vexpr.lparms);
        M_out(L"     rparms location -- %s" ) %  vlist_loc_str(vexpr.rparms);
        msgend_loc(vexpr);
        results = error_results();                                                              // return error
        return -1;     
    }               

    results = to_results(vlist_val(ret_vlist));                                                 // place output vlist in results 
    results.multiple_results = true;                                                            // indicate that mutiple results are being returned
    return 0;
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @BREAK -- break out of loop (slist)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_break(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_break() called");)

    results                 = results_S { }; 
    results.ty              = type_E::special;
    results.special_results = true;
    results.break_flag      = true;
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @GOTO -- go to label in current or enclosing (active) slist
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_goto(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side string parm (required parm)

    M__(M_out(L"verb_goto() called");)

    results                 = results_S { }; 
    results.ty              = type_E::special;
    results.special_results = true;
    results.goto_flag       = true;
    results.str             = M_get_right_pos_string(vexpr, 0); // @GOTO target label = 1st right string parm

    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @LEAVE  -or- @LEAVE "target" -- end evaluation of current or enclosing (active) slist
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_leave(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is 0 or 1 right-side string parms (optional target parm)

    M__(M_out(L"verb_leave() called");)

    results                 = results_S { }; 
    results.ty              = type_E::special;
    results.special_results = true;
    results.leave_flag      = true;

    if (vexpr.rparms.value_ct > 0)                      // @LEAVE target supplied?  
        results.str = M_get_right_pos_string(vexpr, 0); // @LEAVE target label = 1st right string parm
    else
        results.str = L"";                              // no @LEAVE target -- set empty wstring

    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @RETURN value -- return from current enclosing verb block 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_return(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is 0 or 1 right-side string parms (optional return value)

    M__(M_out(L"verb_return() called");)


    // handle no return value case

    if (vexpr.rparms.value_ct == 0)                          
    {
        results = results_S { };                               // uninitialized results with no vlist
        results.multiple_results = true;                       // indicate multiple results (with no vlist) = 0 values in calling vexpr/vlist
    }


    // handle single return value case

    else if (vexpr.rparms.value_ct == 1)
    {
        results = to_results(vexpr.rparms.values.at(0));        // single @RETURN value = 1st right parm
    }


    // handle multiple return value case

    else
    {
        vlist_S ret_vlist  {  };                               // start with empty vlist as return value 

        ret_vlist.values = vexpr.rparms.values;                // put all right-side positional parms into vlist_S 
        results = to_results(vlist_val(ret_vlist));            // place all right-side positional parms in vlist with multiple results 
        results.multiple_results = true;                       // indicate that mutiple results are being returned
    }

    results.special_results = true;                            // indicate special flags are set
    results.return_flag     = true;                            // cause any nested blocks in verb block to end
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @THROW value -- throw exception to nearest @TRY/catch: handler 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_throw(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is 1 right-side string parm (required @THROW value)

    M__(M_out(L"verb_throw() called");)

    results                 = results_S { to_results(vexpr.rparms.values.at(0)) };  // "plain" results from 1st positional right-side parm  
    results.special_results = true;                                 
    results.throw_flag      = true;                                // indicate results are from throw
    return 0; 
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @RETHROW value -- throw exception to nearest @TRY/catch: handler -- value is 1st right positional parm for enclosing block 
//                   -- @RETHROW is meant for use in catch: blocks
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_rethrow(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there iare no parms

    M__(M_out(L"verb_rethrow() called");)

    value_S rethrow_value { unit_val() };                                          // unit rethrow value -- in case no right parms 
    (void)get_vlist_positional(frame.rparms, rethrow_value, 0);                    // r/c=-1 and at_value is unit, if 1st parm not present 

    results                 = results_S { to_results(rethrow_value) };             // "plain" results 
    results.special_results = true;                                 
    results.throw_flag      = true;                                                // indicate results are from throw
    return 0; 
}
M_endf
    


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @TRY {slist} catch:{block} -- run slist and catch any escaping @THROW results 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_try(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is 1 required right-side slist parm, and a catch: keyword parm with slist value

    M__(M_out(L"verb_try() called");)


    // @TRY -- always run 1st positional parm as slist -- save @TRY results for results prioritization   

    results_S try_results { };                                                           // results from slist evaluation -- passed back directly unless @THROW happened
    auto try_rc = eval_slist(frame, *(vexpr.rparms.values.at(0).slist_sp), try_results); // run @TRY slist, and save r/c results 

    results_S        catch_results     {     };                                          // set up uninitialized catch:   results -- for results prioritization, if no catch: keyword 
    results_S        finally_results   {     };                                          // set up uninitialized finally: results -- for results prioritization, if no finally: keyword   
    decltype(try_rc) catch_rc          {0    };                                          // set up zero  catch:   r/c             -- for results prioritization, if no catch: keyword 
    decltype(try_rc) finally_rc        {0    };                                          // set up zero  finally: r/c             -- for results prioritization, if no finally: keyword 
    bool             catch_done        {false};                                          // true, if catch:  slist was evaluation
    bool             finally_done      {false};                                          // true if finally: slist was evaluated


    // run catch: slist, if @THROW occurred during @TRY evaluation, and catch: keyword is present

    if ( (try_rc == 0) && (try_results.throw_flag) )                                     // @THROW results escaped @TRY slist evaluation? 
    {                                                                                     
        // see if catch: keyword is present 

        value_S catch_value {};                                                          // should be slist, or unit 
        if (0 == get_right_keyword(vexpr, L"catch", catch_value))                        // catch: keyword  present? 
        {   
            // run catch: slist as block with 1st right positional parm = throw value from @TRY slist evaluation

            vlist_S right_vlist { };                                                     // right vlist parms for catch: block 
            add_positional_value(right_vlist, try_results , true);                       // set 1st/only positional parm = value from @THROW results  

            catch_rc  = eval_block(frame, vlist_S { }, right_vlist, *(catch_value.slist_sp), catch_results);
            catch_done = true;
        }
    }
      

    // always run finally: slist, if present, regardless of results from @TRY and perhaps catch:

    value_S finally_value {};                                                            // should be unit or slist 
    if (0 == get_right_keyword(vexpr, L"finally", finally_value))                        // finally: keyword present? 
    {
        try_rc = eval_slist(frame, *(finally_value.slist_sp), finally_results);          // run finally: slist and save r/c results 
        finally_done = true; 
    }


    // prioritize results from @TRY, catch: and finally: slist evaluations 
    // -------------------------------------------------------------------
     
    if (!catch_done)                                                                     // catch: not run? 
    {
        if (!finally_done)                                                               // if no catch: or finally:
        {
            // only @TRY slist was run -- pass back results from @TRY

            if (try_rc != 0)
                results = error_results(); 
            else
                results = try_results; 
            return  try_rc; 
        }
        else                                                                            // finally: , but no catch: -- return most important results (finally: wins any tie) 
        {
            if (
                 (finally_rc != 0)                                                     // error from finally: slist?
                 ||                                                                    // -or-
                 (finally_results.special_results)                                     // special results from finally slist?
                 ||                                                                    // -or-
                 ( (try_rc == 0) && (!try_results.special_results) )                   // normal results from @TRY slist?        
               )                                                                       // results from finally: are more important than results from @TRY slist ??
            {
                // ignore results from regular @TRY evaluation -- use finally: results
       
                if (finally_rc != 0)
                    results = error_results();
                else
                    results = finally_results; 
                return  finally_rc;       
            } 
            else                                                                       // unusual results from @TRY slist are more important than normal results from finally: slist 
            {
                // ignore results from finally: -- use @TRY results
       
                if (try_rc != 0)
                    results = error_results();
                else
                    results = try_results; 
                return  try_rc;  
            }
        }
    }
    else                                                                               // catch: was run -- @TRY must have ended with @THROW -- always ignore @TRY results 
    {
        if (!finally_done)                                                             // catch: but no finally: 
        {
            // only @TRY and catch: slists were run -- pass back results from catch: -- ignore @THROW resuls from @TRY slist

            if (catch_rc != 0)
                results = error_results(); 
            else
                results = catch_results; 
            return  catch_rc;        
        }
        else                                                                           // @TRY, and both catch: and finally: were run -- return most important result (finally: wins any tie)
        {
            if (
                 (finally_rc != 0)                                                     // error from finally: slist?
                 ||                                                                    // -or-
                 (finally_results.special_results)                                     // special results from finally slist?
                 ||                                                                    // -or-
                 ( (catch_rc == 0) && (!catch_results.special_results) )               // normal results from catch: slist?        
               )                                                                       // results from finally: are more important than results from catch: slist ??
            {
                // ignore results from catch: evaluation -- use finally: results
       
                if (finally_rc != 0)
                    results = error_results();
                else
                    results = finally_results; 
                return  finally_rc;       
            } 
            else                                                                       // unusual results from catch: slist are more important than normal results from finally: slist 
            {
                // ignore results from finally: -- use catch: results
       
                if (catch_rc != 0)
                    results = error_results();
                else
                    results = catch_results; 
                return  catch_rc;  
            }
        }
    }

    // control should not get here
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @CONTINUE -- break out of slist in loop -- but continue looping 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_continue(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_continue() called");)

    results                 = results_S { };
    results.ty              = type_E::special;
    results.special_results = true;
    results.continue_flag   = true;
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @QUIT -- end evaluation of lowest enclosing block imediately 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_quit(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_quit() called");)

    results                 = results_S { };
    results.ty              = type_E::special;
    results.special_results = true;
    results.quit_flag       = true;
    return 0; 
}
M_endf
 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @END -- end evaluation of main slist imediately 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_end(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_end() called");)

    results                 = results_S { };
    results.ty              = type_E::special; 
    results.special_results = true;
    results.end_flag        = true;
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @EXIT -- issue exit() -- uncontrolled end 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_exit(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_exit() called");)


    do_exit();      


    // following code is not reached
    // -----------------------------

    results                 = results_S { };
    results.ty              = type_E::special; 
    results.special_results = true;
    results.end_flag        = true;
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @_EXIT -- issue _exit() -- uncontrolled end 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb__exit(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb__exit() called");)


    do__exit();      


    // following code is not reached
    // -----------------------------

    results                 = results_S { };
    results.ty              = type_E::special; 
    results.special_results = true;
    results.end_flag        = true;
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @QUICK_EXIT -- issue quick_exit() -- uncontrolled end 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_quick_exit(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_quick_exit() called");)


    do_quick_exit();      


    // following code is not reached
    // -----------------------------

    results                 = results_S { };
    results.ty              = type_E::special; 
    results.special_results = true;
    results.end_flag        = true;
    return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @ABORT -- issue abort() -- uncontrolled end 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_abort(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_abort() called");)


    do_abort();      


    // following code is not reached
    // -----------------------------

    results                 = results_S { };
    results.ty              = type_E::special; 
    results.special_results = true;
    results.end_flag        = true;
    return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @DO {slist} -- slist is required
//        continue:  
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_do(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional slist

    M__(M_out(L"verb_do() called");)

    bool continue_running { false };                                                                 // default = end the run if parsing error is found
    M_get_right_keyword_nval(vexpr, L"continue", continue_running, true)                             // don't end the run if parsing error is found, if continue: keyword is present
           
    results_S do_results {}; 
    
    M__(M_out(L"verb_do() -- calling eval_slist() ************************");)
    auto erc = eval_slist(frame, *(vexpr.rparms.values.at(0).slist_sp), do_results);                 // results (with any special flags) will be passed back directly (if no error)
    M__(M_out(L"verb_do() -- eval_slist() returned -- rc=%d *****************") % erc;)
   
    if (do_results.multiple_results)
    {
        M__(M_out(L"verb_do() -- multiple results returned by eval_slist");)
    }


    // suppress the error r/c, if continue: option present -- substitute unit results
        
    if (erc != 0)
    {
        M__(M_out(L"verb_do() -- eval_slist() returned error");)
        if (continue_running)
        {
            M__(M_out(L"verb_do() -- suppressing error");)
            results = unit_results(); 
            return 0;
        }
        else
        {
            M__(M_out(L"verb_do() -- returning error");)
            results = error_results(); 
            return erc;
        }
    }
    

    // suppress the "error" flag, if continue: option present -- substitute unit results

    if (continue_running)
    {
        M__(M_out(L"verb_do() -- eval_slist() returned OK -- continue:");)
        if (do_results.error)
        {
            M__(M_out(L"verb_do() -- ignoring the error flag");)
            results = unit_results();
            return 0;    
        }
    }

    M__(M_out(L"verb_do() --  returning normally");)  
    results = do_results;    // pass back any results (including all special flags)
    return 0;  
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @BLOCK left:[vlist] right:[vlist] {slist} -- slist is required, left: and right: are optional
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_block(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional slist, and maybe left: and right: keywords, with vlist values

    M__(M_out(L"verb_block() called");)
        

    // get left: and right: vlists (if any)

    vlist_S   left_vlist    {}; 
    vlist_S   right_vlist   {}; 

    {
        value_S key_value {}; 
        auto grc = get_right_keyword(vexpr, L"left", key_value);  // r/c = -1, if left: keyword is not present 

        if (grc == 0)                                             // left: is present 
            left_vlist = *(key_value.vlist_sp); 
    } 
    {
        value_S key_value {}; 
        auto grc = get_right_keyword(vexpr, L"right", key_value); // r/c = -1, if right: keyword is not present 

        if (grc == 0)                                             // left: is present 
            right_vlist = *(key_value.vlist_sp); 
    } 


    // runs passed-in slist in new stack frame, with parms from left: and right: keywords

    slist_S   slist         { *(vexpr.rparms.values.at(0)).slist_sp }; 
    results_S block_results {                                       };

    auto erc = eval_block(frame, left_vlist, right_vlist, slist, block_results);
    if (erc != 0)
    {
        results = error_results();                               // error -- pass back error results 
        return erc;                                              // return with error r/c
    }

    results = block_results;                                     // pass back any results (including all special flags)
    return 0;  
}
M_endf
 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @CASE when:(vexpr) when:(vexpr) ... when:(vexpr) {slist} {slist} ... {slist} else:{slist} 
//
//      -or-
//
//    @CASE when:(vexpr) {slist} when:(vexpr) {slist} ... when:(vexpr) {slist} else:{slist}
//
//
//  -- number of positional {slist} parms must equal the number of when: keywords
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_case(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that all right-side positional parms {slist} and else: is {slist}, and all when: keywords are integers (evaluated vexprs)
    // also -- only one else: keyword, and no left parms at all

    results = unit_results();                             // output value = unit (if no slist executed) 

    M__(M_out(L"verb_case() called");)


    // complain, if number of when: keywords is not same as number of positional parms with {slist}

    auto when_ct = vexpr.rparms.eval_kws.count(L"when");  // number of when: keywords
    auto pos_ct  = vexpr.rparms.values.size();            // number of positional parms ({slist})         

    M__(M_out(L"when_ct=%d  pos_ct=%d") % when_ct % pos_ct;)

    if (when_ct != pos_ct)
    {
        M_out_emsg1(L"verb_case() -- number of when: keywords (%d) does not match the number of positional {slist} parms (%d) -- case verb is not executed") % when_ct % pos_ct; 
        msgend_loc(vexpr.rparms, vexpr);
        results = error_results();                        // return error results
        return -1; 
    }


    // loop through all  when:  keywords and corresponding positional {slist}s -- execute 1st matching one, then return
    // ----------------------------------------------------------------------------------------------------------------

    for (auto i = 0; i < pos_ct; i++)
    {
        // evaluate when: condition and evaluate corresponding slist (returns immediately if error or special results flag is set)

        results_S when_results { }; 
        M_eval_cond(multimap_at(vexpr.rparms.eval_kws, std::wstring{L"when"}, i), L"@CASE when:", when_results)     // handles any special results from the when: evaluation

        if (is_value_true(when_results))                  // is condition "true" ?
        {
            results_S slist_results {}; 

            auto erc = eval_slist(frame, *(vexpr.rparms.values.at(i).slist_sp), slist_results);    // execute the matching {slist} in the positional parms
            if (erc != 0) 
            {
                results = error_results();                // replace eval results with error results, becahse of bad R/C
                return erc;
            }


            // continue looping through when: and slists if continue flag is set in slist evaluation results

            if (slist_results.continue_flag == true)
            {
                continue; 
            }
            else
            {
                if (slist_results.break_flag)                //    ??????????????????????????????
                {
                    slist_results.break_flag      = false;   // make sure break flag is not set before returning
                    slist_results.special_results = false;   // make sure break flag is not set before returning     
                }  
                results = slist_results;                     // other special results flags (error, leave, etc.) may still be on  
                return 0;
            }
        }
    }


    // loop fell through -- no matching when: tests (or last when: was continued) -- execute any {slist} for else: keyword (if present)
     
    if (vexpr.rparms.eval_kws.count(L"else") > 0)
    {
        results_S slist_results {};
        auto erc = eval_slist(frame, *(multimap_at(vexpr.rparms.eval_kws, std::wstring{L"else"}).slist_sp), slist_results); 
        if (erc != 0) 
        {
            results = error_results();               // replace eval results with error results, becahse of bad R/C
            return erc;
        }
        else
        {
            if (slist_results.break_flag || slist_results.continue_flag)
            {
                slist_results.continue_flag   = false;   // make sure continue flag is not set before returning 
                slist_results.break_flag      = false;   // make sure break flag is not set before returning
                slist_results.special_results = false;   // make sure flag is not set before returning 
            }
            results = slist_results;                 // other special results flags (error, leave, etc.) may still be on               
            return 0; 
        }
    }
    

    // must be no slist was executed -- return with unit results ??????

    results = unit_results(); 
    return 0; 
}
M_endf
   

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @IF   int-value then: {slist} else: {slist} -- both keywords are optional
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_if(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional integer parm -- then: and else: keywords are slist's (if present)

    int rc = 0; 
    results_S if_results { unit_results() };                      // output value = unit (if no slist executed) 
    M__(M_out(L"verb_if() called");)

    if ( is_value_true(vexpr.rparms.values.at(0)) )               // test condition is non-zero (true) ?   
    {    
        value_S key_value { }; 
        auto grc = get_right_keyword(vexpr, L"then", key_value);  // r/c = -1, if then: keyword is not present  
        if (grc == 0)                                             // then: is present 
        {
            auto erc = eval_slist(frame, *(key_value.slist_sp), if_results); 
            if (erc != 0)
            {
                if_results = error_results(); 
                rc = erc; 
            }
        }
    }
    else                                                          // test condition is 0 (false)
    {
        value_S key_value { };   
        auto grc = get_right_keyword(vexpr, L"else", key_value);  // r/c = -1, if then: keyword is not present 
        if (grc == 0)                                             // then: is present 
        {
            auto erc = eval_slist(frame, *(key_value.slist_sp), if_results); 
            if (erc != 0)
            {
                if_results = error_results(); 
                rc = erc; 
            }  
        }    
    }

    results = if_results;     // output results, directly from any successful then: or else: slist evaluation -- special results flags mey be set
    return rc; 
}
M_endf
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @LOOP while:int-val {slist} until:int-val -- slist required
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_loop(frame_S& frame, const e_vexpr_S& vexpr, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional slist parm -- while: or until: kws may be present and is not yet evaluated (if identifier or nested vexpr) 

    M__(M_out(L"verb_loop() called");)

    results_S loop_results { unit_results() };                        // output value = unit (in case no slists are executed)

    value_S while_value {};                                           // unevaluated value of while: keyword  
    value_S until_value {};                                           // unevaluated value of until: keyword 
    value_S init_value  {};                                           // slist value of init: keyword 
    value_S next_value  {};                                           // slist value of next: keyword 

    auto while_rc = get_right_keyword(vexpr, L"while", while_value);  // while_rc is -1, if no while: condition
    auto until_rc = get_right_keyword(vexpr, L"until", until_value);  // until_rc is -1, if no until: condition
    auto init_rc  = get_right_keyword(vexpr, L"init" , init_value );  // init_rc is -1 , if no init:  condition
    auto next_rc  = get_right_keyword(vexpr, L"next" , next_value );  // next_rc is -1 , if no next:  condition

    M__(display_value(while_value, L"while_value");)
    M__(display_value(until_value, L"until_value");)


    // always execute init: slist (if present) before starting loop body
    // -----------------------------------------------------------------

    if (init_rc == 0)                                            // init: is present?
    {
        results_S init_results {}; 
        auto erc = eval_slist(frame, *(init_value.slist_sp), init_results); 
        if (erc != 0)
        {
            results = error_results(); 
            return erc;                        // if error -- return error r/c and error results
        }  


        // handle special results  (support only continue (no-op) and break (don't run loop body at all) -- percolate all the others)

        if (init_results.special_results)
        {
            // if @BREAK -- just return with unit results -- don't run loop body 

            if (init_results.break_flag)
            {
                results = unit_results();    // unit results  -- consume the @BEAK special results
                return 0; 
            }


            // if not @CONTINUE, just percolate special results from init: slist evaluation

            else if (!init_results.continue_flag)
            {
                results = init_results;       // percolate back any unrecognized special flags
                return 0 ;                    // normal R/C (but with special results)
            }

            // must be @CONTINUE -- ignore it and start the loop
        }  
    }    
        

    // main loop, depending on evaluated value for while: keyword 
    // ---------------------------------------------------------- 

    for (;;)
    {    
        // end loop, if while: value is 0 (false) 

        if (while_rc == 0)
        {
            results_S while_results { }; 
            M_eval_cond(while_value, L"@LOOP while:", while_results) 
            if (is_value_false(while_results))
                break; 
        }

        // while: is non-zero (true) -- execute loop body

        results_S slist_results { };
        auto erc = eval_slist(frame, *(vexpr.rparms.values.at(0).slist_sp), slist_results); 

        if (erc != 0)
        {
            M_out_emsg1(L"verb_loop() -- error running loop body -- loop ends"); 
            msgend_loc(vexpr);
            results = error_results();     // pass back error results 
            return -1; 
        }


        // end loop without evaluating until: if @BREAK ended the slist evaluation  

        if (slist_results.break_flag)
            break; 


        // if any other special flag is on besides @CONTINUE, return results with special flag(s) -- loop ends immediately without evaluating until: condition 

        if ( (slist_results.special_results) && (!slist_results.continue_flag) )
        {
            results = slist_results; 
            return 0; 
        }


        // if slist did not end because of @CONTINUE, save away slist evaluation results to be used for @LOOP results, if there are no more normal loop passses   

        if (!slist_results.continue_flag)
            loop_results = slist_results;       // normal slist completion -- save away results, in case this is last one  


        // end loop, if until: condition is true 
      
        if (until_rc == 0)
        {
            results_S until_results { }; 
            M_eval_cond(until_value, L"@LOOP until:", until_results)
            if ( is_value_true(until_results) )
                break; 
        }


        // execute next: slist (if present) before starting next loop pass (which includes checking the :while results) 
        // ------------------------------------------------------------------------------------------------------------

        if (next_rc == 0)                                            // init: is present 
        {
            results_S next_results {}; 
            auto erc = eval_slist(frame, *(next_value.slist_sp), next_results); 
            if (erc != 0)
            {
                results = error_results(); 
                return erc;                              // if error -- return error r/c and error results
            }  
       

            // handle special results  (support only continue (no-op) and break (don't run loop body at all) -- percolate all the others)
       
            if (next_results.special_results)
            {
                // if @BREAK -- end loop as if break occurred during the main slist evaluation
       
                if (next_results.break_flag)
                    break;                  // end loop now, and return with appropriate loop results from last normal pass 
                         
       
                // if not @CONTINUE, just percolate special results from next: slist evaluation
       
                if (!next_results.continue_flag)
                {
                    results = next_results; 
                    return 0 ; 
                }
       
                // must be @CONTINUE -- ignore it and start the next pass
            }  
        } 

        // next loop pass starts, with checking of while: results
    }   // end of main loop


    // return, with set up results from slist evaluation -- should not have any unusual flags on 

    results = loop_results; 
    return 0; 
}
M_endf


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""