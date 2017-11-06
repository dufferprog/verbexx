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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 
//    static variables 
//    ----------------

namespace static_N
{
uint64_t            persistent_env_sernum       {0};                  // serial number of most-recently allocated (or filled-in) persistent environment
}



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
////     @IMBED
////     @SKIPTO
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
////     @SKIP
////     @SKIPTO
////     @SKIPALL
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
////     @ERROR
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
//    define_user_verb() -- helper function for verb_verb()/verb_fn() -- defines verb in proper environment
//
//    (note that any required name: checking must have been done already) 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int define_user_verb(frame_S& frame, const e_expression_S expression, verbdef_S& verbdef, const std::wstring& verbname, results_S& results) try
{
    int          rc          { 0 }; 
    verbset_S    verbset     {   }; 

    M__(M_out(L"define_user_verb() called -- verbname = \"%S\"") % verbname;)
                                                                                         
                                                                                         
    // get global:, local:, verbmain: static:, priority: etc. target environment keyword parm                
    
    value_S priority { };

    auto global_rc          = get_right_keyword(   expression, L"global"            );             // global_rc          = -1, if global:          is not present 
    auto verbmain_rc        = get_right_keyword(   expression, L"verbmain"          );             // static_rc          = -1, if verbmain:        is not present 
    auto static_rc          = get_right_keyword(   expression, L"static"            );             // static_rc          = -1, if static:          is not present 
#ifdef M_EXPOSE_SUPPORT                                                                                                                            
    auto expose_rc          = get_right_keyword(   expression, L"expose"            );             // expose_rc          = -1, if expose:          is not present (meaning no expose)
#endif                                                                                                                                             
    auto priority_rc        = get_right_keyword(   expression, L"priority", priority);             // priority_rc        = -1, if priority:        is not present 
    auto left_associate_rc  = get_right_keyword(   expression, L"left_associate"    );             // left_associate_rc  = -1, if left_associate:  is not present 
    auto right_associate_rc = get_right_keyword(   expression, L"right_associate"   );             // right_associate_rc = -1, if right_associate: is not present 


    // add passed-in verbdef to new/empty verbset 

    verbset.verbs.push_back(verbdef); 
    

    // set priority and associativity of the verbset being added

    if (priority_rc         == 0)       verbset.priority        = priority.int64; 
    if (left_associate_rc   == 0)       verbset.left_associate  = true; 
    if (right_associate_rc  == 0)       verbset.right_associate = true; 


    // if name: was provided, add this verb to the global:, verbmain: static: or local: environments
    // ---------------------------------------------------------------------------------------------

    if (verbname.size() > 0)                                                               // verb name present -- means that new verbdef needs to be added to some environemnt 
    {
        if (global_rc == 0)                                                                // global: is present, meaning this is a global verb
        {
            // verb is global: -- define (non-builtin) global verb -- note that expose: is assumed (global identifiers are always exposed)
       
            auto dvrc = def_global_verb(verbname, verbset); 
            if (dvrc != 0)
            {
                M_out_emsg1(L"define_user_verb() -- error r/c from def_global_verb() -- adding global: verb=%s") % verbname; 
                msgend_loc(expression);            
                results = error_results();                                                   // pass back error results
                return -1;                                                                   // return with error r/c                                                     
            } 
        }
        else if (static_rc == 0)
        {
            // verb is static: --  define (non-builtin) static verb -- exposed or not    
       
            def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
            parm.exposed = (expose_rc == 0) ? true : false;
#endif
       
            auto dvrc = def_static_verb(frame, verbname, verbset, parm); 
            if (dvrc != 0)
            {
                M_out_emsg1(L"define_user_verb() -- error r/c from def_static_verb() -- adding static: verb=%s") % verbname; 
                msgend_loc(expression);            
                results = error_results();                                                   // pass back error results
                return -1;                                                                   // return with error r/c                                                     
            } 
        }
        else if (verbmain_rc == 0)
        {
            // verb is verbmain: --  define (non-builtin) verbmain verb -- exposed or not    
       
            def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
            parm.exposed = (expose_rc == 0) ? true : false;
#endif
       
            auto dvrc = def_verbmain_verb(frame, verbname, verbset, parm); 
            if (dvrc != 0)
            {
                M_out_emsg1(L"define_user_verb() -- error r/c from def_verbmain_verb() -- adding verbmain: verb=%s") % verbname; 
                msgend_loc(expression);            
                results = error_results();                                                   // pass back error results
                return -1;                                                                   // return with error r/c                                                     
            } 
        }
        else                                                                                 // not global: or static:
        {
            // not global:, verbmain:, or static: -- must be local verb definition -- define (non-builtin) local verb -- exposed or not    
       
            M__(M_out(L"define_user_verb() called -- defining local verbname = \"%S\"") % verbname;)

            def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
            parm.exposed = (expose_rc == 0) ? true : false; 
#endif
       
            auto dvrc = def_local_verb(frame, verbname, verbset, parm); 
            if (dvrc != 0)
            {
                M_out_emsg1(L"define_user_verb() -- error r/c from def_local_verb() -- adding local: verb=%s") % verbname; 
                msgend_loc(expression);            
                results = error_results();                                                   // pass back error results
                return -1;                                                                   // return with error r/c                                                     
            } 
        }
    }


    // run the code in the init:{} block (if any) -- handle any errors or special results (note that verb is already defined when the init: block is run) 
    // ----------------------------------------------------------------------------------

    if (verbdef.init_block_sp.use_count() >= 1)
    {
        results_S block_results { }; 

        auto erc = eval_verbinit_block(verbdef, block_results);


        // return with error, if error occurred during evaluation of init: block

        if (erc != 0)
        {
            results = error_results();
            return -1;  
        }


        // return any special results (note that any normal results from the init: block are ignored 

        if (block_results.special_results)
        {
            results = block_results;  
            return 0; 
        }
    }

    // no init block or no special results from init block -- new verb definition created OK (or was not needed) -- return normally with single-verb verbset_S

    results =  to_results(verbset_val(verbset));  
    M__(M_out(L"define_user_verb() returning");)
    return 0;
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    build_verb_scopes() -- helper function for verb_verb()/verb_fn() -- sets up scope info and builds persistent SF
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int build_verb_scopes(frame_S& frame, const e_expression_S expression, verbdef_S& verbdef) try
{
    int rc { 0 }; 


    // set up scope/closure info in verbdef
    // ------------------------------------

    value_S init_value { };                                                                  // value from init: can be empty or a block 

    auto dynamic_rc = get_right_keyword(expression, L"dynamic_scope"                );       // dynamic_rc = -1, if dynamic_scope: is not present 
#ifdef M_EXPOSE_SUPPORT                                                           
    auto dynall_rc  = get_right_keyword(expression, L"dynall_scope"                 );       // dynall_rc  = -1, if dynall_scope:  is not present 
#endif                                                                            
    auto lexical_rc = get_right_keyword(expression, L"lexical_scope"                );       // lexical_rc = -1, if lexical_scope: is not present
    auto same_rc    = get_right_keyword(expression, L"same_scope"                   );       // same_rc    = -1, if same_scope:    is not present
    auto no_rc      = get_right_keyword(expression, L"no_scope"                     );       // no_rc      = -1, if no_scope:      is not present
    auto close_rc   = get_right_keyword(expression, L"close"                        );       // close_rc   = -1, if close:         is not present 
    auto init_rc    = get_right_keyword(expression, L"init"         , init_value    );       // init_rc    = -1  if init:          is not present 


    // put scoping info in verbdef for use when verb-block is executed -- note: if neither dynamic_scope: nor dynall_scope: was present, default is lexical scope/no scope  (maximum of 1 of dynall_scope:, dynamic_scope:, no_scope: same_scope: or lexical_scope: can appear)

    if (dynamic_rc == 0)
    {
        verbdef.dynamic_scope = true;                                                        // dynamic scope only if  dynamic_scope:  keyword is present
    }
#ifdef M_EXPOSE_SUPPORT      
    else if (dynall_rc == 0)    
    {      
         verbdef.dynall_scope = true;                                                        // dynall scope   only if  dynall_scope:    keyword is present     
    }      
#endif
    else if (same_rc == 0)
    {
         verbdef.same_scope    = true;                                                       // same scope    only if  same_scope:     keyword is present 
         verbdef.percolate_all = true;                                                       // also flag this verb as percolating all special results (especially @RETURN)
    }
    else if (no_rc == 0)
    {
         verbdef.no_scope = true;                                                            // no scope      only if  no_scope:        keyword is present     
    }
    else                                                                                     // must be lexical scope -- either defaulted or explicitly requested
    {
        verbdef.lexical_scope   = true;                                                      // set lexical scope if other scope types were not present (lexical_scope: can be present or not -- default is lexical_scope/no_scope)
        verbdef.defining_scope_wp = frame.self_wp;                                           // for lexical upward scoping, save shared pointer to stack frame where verb is defined (i.e. current frame_S)

        if (lexical_rc == -1)                                                                // was scope defaulted (to lexical/no) ?
          verbdef.scope_defaulted = true;                                                    // set flag to indicate this   

        if (close_rc == 0)                                                                   // need to close on enclosing stack frame? 
          verbdef.defining_scope_sp = std::shared_ptr<frame_S> { frame.self_wp };            //    also save shared pointer to stack frame where verb is defined (i.e. current frame_S) 
                                                                                             //    note: this should prevent the defining scope from going away if the function that defined this verb returns and the verb definition still exists somewhere 
                                                                                             //    note: this verb definition becomes a closure in this case 
    }


    //   set up init: block for this verb (if init:{} specified)  --  allocate new non-autodata block_S on heap, anchor in verbdef_S, and copy passed-in init: block_S into new block_S in heap 

    if (init_rc == 0)                                                                         // only if init:{} was coded -- otherwise, leave init_block_sp in default initialized state  
    {                                                                                       
        if (init_value.ty == type_E::block)                                                   // can have init: (no block) or init:{ ...} (with block) 
        {                                                                                   
            verbdef.init_block_sp.reset(new block_S {});                                      // allocate new empty block_S       
            *(verbdef.init_block_sp) = *(init_value.block_sp);                                // copy block_S from parm to new block_S anchored in verb definition 
        }
    }

    
    // set up persistent SF for this verb (if this verb does not share caller's environment -- same_scope: keyword) -- note: init:{} keyword cannot be present when same_scope: is present)
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     
    if (init_rc == 0)                                                                         // get persistent environment, if init:{ ... } was specified
    {
        std::shared_ptr<environ_S> persist_env_sp { new environ_S { } };                      // allocate new persistent environment fo this verb and anchor it locally, for now
        
        verbdef.persist_env_sp = persist_env_sp;                                              // set up shared ptr to persistent env in verbdef_S -- will keep persistent env around until verb is freed up
        verbdef.persist_env_sp->is_persistent = true;                                         // indicate that this is a persistent environment
        verbdef.persist_env_sp->sernum = ++static_N::persistent_env_sernum;                   // put unique (positive) serial number on this persistent environment 

        // (note -- init: block is executed in define_user_verb() helper function)
    }

    return rc;
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    build_verb_argvar() -- helper function for verb_verb()/verb_fn() -- builds left and right argvars_S structure in verbdef based on passed-in vlists from argvars:
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int build_verb_argvar(frame_S& frame, const e_expression_S expression, verbdef_S& verbdef, const vlist_S& l_vlist, const vlist_S& r_vlist) try
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
            msgend_loc(elem, expression);
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
            msgend_loc(expression);
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
            msgend_loc(expression);
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
//    build_verb_parmtype() -- helper function for verb_verb()/verb_fn()
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void build_verb_parmtype(frame_S& frame, const e_expression_S expression, const vlist_S& vlist, parmtype_S& parmtype, bool is_keyword = false) try
{
    parmtype = parmtype_S { }; 
    
    
    // get keyword parms from caller's vlist
    // -------------------------------------

    auto unit_rc         = get_vlist_keyword(vlist, L"unit"            ); // r/c = -1 if not present
    auto int8_rc         = get_vlist_keyword(vlist, L"int8"            ); // r/c = -1 if not present
    auto int16_rc        = get_vlist_keyword(vlist, L"int16"           ); // r/c = -1 if not present
    auto int32_rc        = get_vlist_keyword(vlist, L"int32"           ); // r/c = -1 if not present
    auto int64_rc        = get_vlist_keyword(vlist, L"int64"           ); // r/c = -1 if not present
    auto uint8_rc        = get_vlist_keyword(vlist, L"uint8"           ); // r/c = -1 if not present
    auto uint16_rc       = get_vlist_keyword(vlist, L"uint16"          ); // r/c = -1 if not present
    auto uint32_rc       = get_vlist_keyword(vlist, L"uint32"          ); // r/c = -1 if not present
    auto uint64_rc       = get_vlist_keyword(vlist, L"uint64"          ); // r/c = -1 if not present
    auto float32_rc      = get_vlist_keyword(vlist, L"float32"         ); // r/c = -1 if not present
    auto float64_rc      = get_vlist_keyword(vlist, L"float64"         ); // r/c = -1 if not present
    auto string_rc       = get_vlist_keyword(vlist, L"string"          ); // r/c = -1 if not present
    auto raw_ident_rc    = get_vlist_keyword(vlist, L"raw_ident"       ); // r/c = -1 if not present
    auto var_ident_rc    = get_vlist_keyword(vlist, L"var_ident"       ); // r/c = -1 if not present
    auto const_ident_rc  = get_vlist_keyword(vlist, L"const_ident"     ); // r/c = -1 if not present
    auto undef_ident_rc  = get_vlist_keyword(vlist, L"undef_ident"     ); // r/c = -1 if not present
    auto vlist_rc        = get_vlist_keyword(vlist, L"vlist"           ); // r/c = -1 if not present
    auto expression_rc   = get_vlist_keyword(vlist, L"expression"      ); // r/c = -1 if not present
    auto block_rc        = get_vlist_keyword(vlist, L"block"           ); // r/c = -1 if not present
    auto verbset_rc      = get_vlist_keyword(vlist, L"verbset"         ); // r/c = -1 if not present
    auto typedef_rc      = get_vlist_keyword(vlist, L"typedef"         ); // r/c = -1 if not present
    auto array_rc        = get_vlist_keyword(vlist, L"array"           ); // r/c = -1 if not present
    auto struct_rc       = get_vlist_keyword(vlist, L"struct"          ); // r/c = -1 if not present
    auto ref_rc          = get_vlist_keyword(vlist, L"ref"             ); // r/c = -1 if not present

    int  type_ct = 1 + unit_rc     
                 + 1 + int8_rc      + 1 + int16_rc       + 1 + int32_rc       + 1 + int64_rc 
                 + 1 + uint8_rc     + 1 + uint16_rc      + 1 + uint32_rc      + 1 + uint64_rc 
                 + 1 + float32_rc   + 1 + float64_rc     + 1 + string_rc 
                 + 1 + var_ident_rc + 1 + const_ident_rc + 1 + undef_ident_rc + 1 + vlist_rc + 1 + expression_rc  + 1 + block_rc + 1 + verbset_rc;    // add up count of type keywords present 


    // evaluation flags

    auto no_eval_ident_rc      = get_vlist_keyword(vlist, L"no_eval_ident"       ); // r/c = -1 if not present
    auto no_eval_expression_rc = get_vlist_keyword(vlist, L"no_eval_expression"  ); // r/c = -1 if not present
    auto no_eval_vlist_rc      = get_vlist_keyword(vlist, L"no_eval_vlist"       ); // r/c = -1 if not present
    auto no_eval_ref_rc        = get_vlist_keyword(vlist, L"no_eval_ref"         ); // r/c = -1 if not present


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
        if(raw_ident_rc   == 0) {parmtype.raw_ident_ok       = true; parmtype.eval.no_eval_ident = true;}
        if(var_ident_rc   == 0) {parmtype.var_ident_ok       = true; parmtype.eval.no_eval_ident = true;} 
        if(const_ident_rc == 0) {parmtype.const_ident_ok     = true; parmtype.eval.no_eval_ident = true;}
        if(undef_ident_rc == 0) {parmtype.undef_ident_ok     = true; parmtype.eval.no_eval_ident = true;}
        if(vlist_rc       == 0)  parmtype.vlist_ok           = true; 
        if(expression_rc  == 0)  parmtype.expression_ok      = true; 
        if(block_rc       == 0)  parmtype.block_ok           = true;    
        if(verbset_rc     == 0)  parmtype.verbset_ok         = true;   
        if(typedef_rc     == 0)  parmtype.typdef_ok          = true; 
        if(array_rc       == 0)  parmtype.array_ok           = true; 
        if(struct_rc      == 0)  parmtype.structure_ok       = true; 
        if(ref_rc         == 0)  parmtype.lvalue_ref_ok      = true; 
        if(ref_rc         == 0)  parmtype.rvalue_ref_ok      = true;
    }    


    // set evaluation flags in this parmtype

    if (no_eval_ident_rc      == 0)  parmtype.eval.no_eval_ident      = true; 
    if (no_eval_expression_rc == 0)  parmtype.eval.no_eval_expression = true; 
    if (no_eval_vlist_rc      == 0)  parmtype.eval.no_eval_vlist      = true; 
    if (no_eval_ref_rc        == 0)  parmtype.eval.no_eval_ref        = true; 


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

int verb_verb(frame_S& frame, const e_expression_S& expression, const verbdef_S& parm_verbdef, results_S& results) try
{
    M__(M_out(L"verb_verb() called");)

    int rc = 0; 
    

    // optional name:"string"    keyword may      be present on right side                    -- present = add to environment -- absent = don't add (just return completed verb as results)
    // optional info:"string"    keyword may      be present on right side                    -- present = add error message info string to verbdef_S
    // required block:{block}    keyword known to be present on right side 
    // optional global:/etc.     keyword on right side (local: is default)                    -- ignored if name: is not present
    // optional expose:          keyword on right side  (assumed if global: present)          -- ignored if name: is not present
    // optional lexical_scope:   keyword on right side                                        -- default is lexical_scope:, if dynamic_scope: not present
    // optional dynamic_scope:   keyword on right side  
    // optional dynall_scope:    keyword on right side  
    // optional same_scope:      keyword on right side 
    // optional no_scope:        keyword on right side 
    // optional close:           keyword on right side  
   
    value_S name_value  { }; 
    value_S info_value  { }; 
    value_S block_value { };

    (void)            get_right_keyword(expression, L"block", block_value);                 // block: keyword is always present 
    auto name_rc    = get_right_keyword(expression, L"name" , name_value );                 // name:  keyword is optional 
    auto info_rc    = get_right_keyword(expression, L"info" , info_value );                 // info:  keyword is optional 


    // optional min: and max: int64 keywords on both sides = number of left/right-side positional parms allowed

    value_S l_min_value   { };
    value_S l_max_value   { };
    value_S r_min_value   { };
    value_S r_max_value   { };
    auto l_min_rc = get_left_keyword( expression, L"min", l_min_value);                       // r/c = -1 if not present
    auto l_max_rc = get_left_keyword( expression, L"max", l_max_value);                       // r/c = -1 if not present
    auto r_min_rc = get_right_keyword(expression, L"min", r_min_value);                       // r/c = -1 if not present
    auto r_max_rc = get_right_keyword(expression, L"max", r_max_value);                       // r/c = -1 if not present


    // if name was supplied, make sure it is a valid verbname

    if (name_rc == 0)
    {
        if (!is_valid_verbname(name_value.string))
        {
            M_out_emsg1(L"verb_verb() -- error occurred when converting from string = «%s» to verbname") % name_value.string; 
            M_out(L"       name location -- %s") % value_loc_str(name_value);  
            msgend_loc(expression); 
      
            results = error_results();                                                        // return error results 
            return -1;                                                                        // failure r/c
        }
    }   


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    verbdef_S verbdef { } ;                 // verbdef_S for new user-defined verb


    // allocate new non-autodata block_S on heap for block:{} and init:{}, anchor them in verbdef_S, and copy passed-in block_S data into the new block_S areas
    // --------------------------------------------------------------------------------------------------------------------------------------------------------

    verbdef.verb_block_sp.reset(new block_S {});                                              // allocate new empty block_S
    *(verbdef.verb_block_sp) = *(block_value.block_sp);                                       // copy block_S from parm to new block_S anchored in verb definition
   

    // main verb block -- allocate new non-autodata block_S on heap, anchor in verbdef_S, and copy passed-in block_S into new block_S in heap 

    verbdef.verb_block_sp.reset(new block_S {});                                              // allocate new empty block_S
    *(verbdef.verb_block_sp) = *(block_value.block_sp);                                       // copy block_S from parm to new block_S anchored in verb definition 
    

    /// set up min/max positional parm counts on each side  -- note: if min: present on a side, so is max: -- match_pairs on each side

    if (l_min_rc == 0)
    {
        if (l_max_value.int64 == -1)
            verbdef.lparms.min_ct = l_min_value.int64;
        else
            verbdef.lparms.min_ct = std::min(l_min_value.int64, l_max_value.int64);
    }
    else
        verbdef.lparms.min_ct = expression.lparms.value_ct;       

    if (l_max_rc == 0)
    {
        if (l_max_value.int64 == -1)
            verbdef.lparms.max_ct = -1;  
        else
            verbdef.lparms.max_ct = std::max(l_min_value.int64, l_max_value.int64);            // max: present -- use explicit value
    }                                                                                      
    else                                                                                       // max: not present
    {                                                                                      
        if (expression.lparms.value_ct > 0)                                                    // one or more positional parmtypes present?
            verbdef.lparms.max_ct = expression.lparms.value_ct;                                // yes -- default max: = number of parmtypes
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
        verbdef.rparms.min_ct = expression.rparms.value_ct; 

    if (r_max_rc == 0)
    {
        if (r_max_value.int64 == -1)
            verbdef.rparms.max_ct = -1;  
        else
            verbdef.rparms.max_ct = std::max(r_min_value.int64, r_max_value.int64);            // max: present -- use explicit value
    }                                                                                     
    else                                                                                       // max: not present
    {                                                                                     
        if (expression.rparms.value_ct > 0)                                                    // one or more positional parmtypes present?
            verbdef.rparms.max_ct = expression.rparms.value_ct;                                // yes -- default max: = number of parmtypes
        else                                                                              
            verbdef.rparms.max_ct = 0;                                                         // assume none allowed, if no positional parmtypes
    }



    // set up individual left-side positional parms (if any are present)
    // -----------------------------------------------------------------

    if (expression.lparms.value_ct > 0)
    {
        parmtype_S parmtype {};

        // loop through the left-side positional vlist parms

        for (const auto& value : expression.lparms.values)
        {
            build_verb_parmtype(frame, expression, *(value.vlist_sp), parmtype);               // build parmtype_S from keywords in n-th left-side vlist parm
            verbdef.lparms.values.push_back(parmtype);                                         // add parmtype_S to left-side verbdef plist
        }
    }


    // set up individual right-side positional parms (if any are present)
    // ------------------------------------------------------------------

    if (expression.rparms.value_ct > 0)
    {
        parmtype_S parmtype {};

        // loop through the right-side positional vlist parms

        for (const auto& value : expression.rparms.values)
        {
            build_verb_parmtype(frame, expression, *(value.vlist_sp), parmtype);               // build parmtype_S from keywords in n-th right-side vlist parm
            verbdef.rparms.values.push_back(parmtype);                                         // add parmtype_S to right-side verbdef plist
        }
    }


    // set up individual left-side key: keyword parms (if any are present)
    // -------------------------------------------------------------------

    auto lkey_ct = expression.lparms.eval_kws.count(L"key");

    if (lkey_ct > 0)
    {
        parmtype_S parmtype {};


        // loop through the left-side key: keywords

        for (auto i = 0; i < lkey_ct; i++)
        {
            // get keyword name and parmtype for adding to plist 

            value_S pos0_value { };                                                            // raw keyword value from n-th instance of key:  
            auto key_value = multimap_at(expression.lparms.eval_kws, std::wstring(L"key"), i); // get n-th key: parm's value 
            get_vlist_positional(*(key_value.vlist_sp), pos0_value, 0);                        // get first positional value for this instance of key:    
            build_verb_parmtype(frame, expression, *(key_value.vlist_sp), parmtype, true);     // build parmtype_S from keywords in n-th left-side vlist parm

            // add keyword/parmtype pair to keywords map, if not alredy present

            if (verbdef.lparms.keywords.count(pos0_value.string) > 0)                          // this keyword name already in map? 
            {
                M_out_emsg1(L"verb_verb() -- left-side keyword «%s» has already been added for verb «%s»") % pos0_value.string % name_value.string; 
                msgend_loc(expression);
                rc = -1;                              // set error r/c but keep going
            }
            else
            {
                verbdef.lparms.keywords.emplace(pos0_value.string, parmtype);                  // add parmtype_S to left-side keywords multimap   // shouldn't have duplicates ??????????????????????
            }
        }
    }
  

    // set up individual right-side key: keyword parms (if any are present)
    // --------------------------------------------------------------------

    auto rkey_ct = expression.rparms.eval_kws.count(L"key");

    if (rkey_ct > 0)
    {
        parmtype_S parmtype {};


        // loop through the left-side key: keywords

        for (auto i = 0; i < rkey_ct; i++)
        {
            // get keyword name and parmtype for adding to plist 

            value_S pos0_value { };                                                               // raw keyword value from n-th instance of key:  
            auto key_value = multimap_at(expression.rparms.eval_kws, std::wstring(L"key"), i);    // get n-th key: parm's value 
            get_vlist_positional(*(key_value.vlist_sp), pos0_value, 0);                           // get first positional value for this instance of key:    
            build_verb_parmtype(frame, expression, *(key_value.vlist_sp), parmtype, true);        // build parmtype_S from keywords in n-th right-side vlist parm

            // add keyword/parmtype pair to keywords map, if not alredy present

            if (verbdef.rparms.keywords.count(pos0_value.string) > 0)                             // this keyword name already in map? 
            {
                M_out_emsg1(L"verb_verb() -- right-side keyword «%s» has already been added for verb «%s»") % pos0_value.string % name_value.string; 
                msgend_loc(expression);
                rc = -1;                              // set error r/c but keep going
            }
            else
            {
                verbdef.rparms.keywords.emplace(pos0_value.string, parmtype);                     // add parmtype_S to right-side keywords multimap   // shouldn't have duplicates ??????????????????????
            }
        }
    }


    // set up argvar_S values in verbdef_S -- keft/right variable names for parms
    // --------------------------------------------------------------------------

    value_S l_var_value { }; 
    value_S r_var_value { };
    vlist_S l_var_vlist { };
    vlist_S r_var_vlist { };  

    auto l_var_rc = get_left_keyword(  expression, L"var", l_var_value);                           // r/c = -1 if not present -- value should have vlist, if present
    auto r_var_rc = get_right_keyword( expression, L"var", r_var_value);                           // r/c = -1 if not present -- value should have vlist, if present

    if ( (l_var_rc == 0) && (l_var_value.vlist_sp.get() != nullptr) )
        l_var_vlist = *(l_var_value.vlist_sp);

    if ( (r_var_rc == 0) && (r_var_value.vlist_sp.get() != nullptr) )
        r_var_vlist = *(r_var_value.vlist_sp);
    
    auto avrc = build_verb_argvar(frame, expression, verbdef, l_var_vlist, r_var_vlist);  
    if (avrc != 0)
        rc = -1;   


    // return with error and error results if any problem occurred so far

    if (rc != 0)
    {
        results = error_results(); 
        return rc; 
    }   


    // set up pointer to non-builtin verb-block: evaluation function
    
    verbdef.fcn_p      = &verb_non_builtin;


    // put scoping/persistent SF info in verbdef

    build_verb_scopes(frame, expression, verbdef);


    // add in information string for error messages

    if (info_rc == 0)
    {
        verbdef.info = info_value.string;
    }
    else
    {
        if (name_rc == 0)
            verbdef.info = name_value.string;
        else
            verbdef.info = L"un-named verb";
    }
    

    // add the completed verbdef to the proper environment (if a name: was provided) 

    return define_user_verb(frame, expression, verbdef, name_value.string, results);
 
}
M_endf   


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    verb_non_builtin() -- function to run user-defined verb -- pointed to by verbdef
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_non_builtin(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_non_builtin() called");)


    // make sure block pointer is non-zero 

    if (verbdef.verb_block_sp == nullptr)
    {
        M_out_emsg1(L"verb_non_builtin() -- verbdef.verb_block_sp is nullptr for verb=%s -- should not occur") % verb_name(expression);
        msgend_loc(expression);
        M_throw_v("verb_non_builtin() -- verbdef.verb_block_sp is nullptr for verb=%s") % out_ws(verb_name(expression)) ));  
    }
       

    // run user-provided block in new verb-block, with parms from the expression   

    return eval_verb_block(frame, expression, verbdef, results); 
}
M_endf
 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////    @FN -- verb_fn() -- simplified way to define non-built-in (user-defined) verb in verb_table
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////  

int verb_fn(frame_S& frame, const e_expression_S& expression, const verbdef_S& parm_verbdef, results_S& results) try
{
    M__(M_out(L"verb_fn() called");)

    int rc = 0; 
    

    // optional left-side undefined identifier -- verb name (unless anonymous function)      -- present = add to environment -- absent = don't add (just return completed verb as results)
    // required right-side vlist (may be empty) with argument variable names
    // required right-side block with function body for this verb
  
    // optional global:/etc.     keyword on right side (local: is default)                   -- ignored if name: is not present
    // optional expose:          keyword on right side  (assumed if global: present)         -- ignored if name: is not present
    // optional lexical_scope:   keyword on right side                                       -- default is lexical_scope:, if dynamic_scope: not present
    // optional dynamic_scope:   keyword on right side 
    // optional dynall_scope:    keyword on right side 
    // optional same_scope:      keyword on right side 
    // optional no_scope:        keyword on right side 
    // optional close:           keyword in right side
   
    value_S name_value  { };                                                           // optional left-side positional value with verb name
    value_S info_value  { };                                                           // info: optional string for error messages
    value_S block_value { };                                                          
    value_S r_var_value { };                                                           // should be vlist with variable names (may be empty)
    vlist_S r_var_vlist { };                                                           // extracted vlist from r_var_value 
    vlist_S l_var_vlist { };                                                           // always empty -- verbs defined with @FN don't have left-side parms
    int64_t r_var_ct    {0};                                                           // nimber of right positional parms required

    auto name_rc    = get_left_positional( expression, name_value        , 0);         // name_rc = -1, if no function name supplied  
    auto r_var_rc   = get_right_positional(expression, r_var_value       , 0);         // argvars vlist shold always be present, but may be empty
    auto block_rc   = get_right_positional(expression, block_value       , 1);         // function block should always be present  
    auto info_rc    = get_right_keyword(   expression, L"info",  info_value );         // info:  keyword is optional 
                                                                              
    if ( (r_var_rc == 0) && (r_var_value.vlist_sp.get() != nullptr ) )
    {
        r_var_ct    = r_var_value.vlist_sp->values.size();                             // number of args expected
        r_var_vlist = *(r_var_value.vlist_sp);                                         // vlist with arg var names
    }  

    M__(M_out(L"verb_fn() name_rc = %d  name_value.string = \"%S\"") % name_rc % name_value.string; )

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    verbdef_S verbdef { } ;                                                            // verbdef_S for new user-defined verb


    // main verb block -- allocate new non-autodata block_S on heap, anchor in verbdef_S, and copy passed-in block_S into new block_S in heap 

    verbdef.verb_block_sp.reset(new block_S {});                                       // allocate new empty block_S
    *(verbdef.verb_block_sp) = *(block_value.block_sp);                                // copy block_S from parm to new block_S anchored in verb definition 
    

    /// set up min/max positional parm counts on each side based on number of arg vars present

    verbdef.lparms.min_ct = 0;
    verbdef.lparms.max_ct = 0;
    verbdef.rparms.min_ct = r_var_ct; 
    verbdef.rparms.max_ct = r_var_ct;

    auto avrc = build_verb_argvar(frame, expression, verbdef, l_var_vlist, r_var_vlist);  
    if (avrc != 0)
        rc = -1;   


    // return with error results if any problem occurred so far

    if (rc != 0)
    {
        results = error_results(); 
        return rc; 
    }   


    // set up pointer to non-builtin verb-block: evaluation function
    
    verbdef.fcn_p      = &verb_non_builtin;


    // put scoping/persistent SF info in verbdef

    build_verb_scopes(frame, expression, verbdef);


    // add in information string for error messages

    if (info_rc == 0)
    {
        verbdef.info = info_value.string;
    }
    else
    {
        if (name_rc == 0)
            verbdef.info = name_value.string;
        else
            verbdef.info = L"un-named verb";
    }
    

    // add the completed verbset to the proper environment (if a name: was provided) 

    return define_user_verb(frame, expression, verbdef, name_value.string, results);
}
M_endf  




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @INITVERB "string" ...
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_initverb(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional string parm  -- optional local: verbmain: static: or global: parms (mutually exclusve)
    M__(M_out(L"verb_initverb() called");)
    int rc {0}; 


    // get verb name to be re-initialized -- 1st (only) right positional parm

    value_S verbname_val { };
    (void)get_right_positional(expression, verbname_val, 0);         // R/C should be 0, since positional parm is always present


    // get global: local: verbmain: and static: keywords   (default is anywhere verb is defined)
 
    auto global_rc     = get_right_keyword(expression, L"global"   );   // global r/c    = -1, if global:     keyword is not present, r/c = 0 if present
    auto static_rc     = get_right_keyword(expression, L"static"   );   // static_rc     = -1, if static:     keyword is not present, r/c = 0 if present 
    auto verbmain_rc   = get_right_keyword(expression, L"verbmain" );   // verbmain_rc   = -1, if verbmain:   keyword is not present, r/c = 0 if present
    auto local_rc      = get_right_keyword(expression, L"local"    );   // local_rc      = -1, if local:      keyword is not present, r/c = 0 if present
    

    // try to locate verbset with caller's name
    // ----------------------------------------
    
    verbset_S verbset { };                                        // located verbset

    if (global_rc == 0)
    {
        if (-1 == get_global_verb(verbname_val.string, verbset))
        {
             count_error(); 
             M_out_emsg1(L"verb_initverb() -- global: verb \"%S\" not found -- cannot be re-initialized") % verbname_val.string;
             msgend_loc(verbname_val, expression);
             results = error_results(); 
             return -1; 
        }
    }
    else if (static_rc == 0)
    {
        if (-1 == get_static_verb(frame, verbname_val.string, verbset))
        {
             count_error(); 
             M_out_emsg1(L"verb_initverb() -- static: verb \"%S\" not found -- cannot be re-initialized") % verbname_val.string;
             msgend_loc(verbname_val, expression);
             results = error_results(); 
             return -1; 
        }
    }
    else if (verbmain_rc == 0)
    {
        if (-1 == get_verbmain_verb(frame, verbname_val.string, verbset))
        {
             count_error(); 
             M_out_emsg1(L"verb_initverb() -- verbmain: verb \"%S\" not found -- cannot be re-initialized") % verbname_val.string;
             msgend_loc(verbname_val, expression);
             results = error_results(); 
             return -1; 
        }
    }
    else if (local_rc == 0)
    {
        if (-1 == get_local_verb(frame, verbname_val.string, verbset))
        {
             count_error(); 
             M_out_emsg1(L"verb_initverb() -- local: verb \"%S\" not found -- cannot be re-initialized") % verbname_val.string;
             msgend_loc(verbname_val, expression);
             results = error_results(); 
             return -1; 
        }
    }
    else            // look for verb anywhere it might be defined
    {
        if (-1 == get_verb(frame, verbname_val.string, verbset))
        {
             count_error(); 
             M_out_emsg1(L"verb_initverb() -- verb \"%S\" not found -- cannot be re-initialized") % verbname_val.string;
             msgend_loc(verbname_val, expression);
             results = error_results(); 
             return -1; 
        }
    }


    // make sure located verbset has only one verbdef

    if (verbset.verbs.size() != 1)
    {
        count_error(); 
        M_out_emsg1(L"verb_initverb() -- located verbset for verb \"%S\" has more than one verb definition (overloaded) -- verb with name \"%S\" cannot be re-initialized") % verbname_val.string % verbname_val.string;
        msgend_loc(verbname_val, expression);
        results = error_results(); 
        return -1; 
    }


    // complain if this verb has no persistent environment

    if (verbset.verbs.at(0).persist_env_sp.get() == nullptr)
    {
        count_error(); 
        M_out_emsg1(L"verb_initverb() -- verb \"%S\" has no persistent environment -- cannot be re-initialized") % verbname_val.string;
        msgend_loc(verbname_val, expression);
        results = error_results(); 
        return -1;     
    }


    // empty out the persistent environment for this verbdef

    verbset.verbs.at(0).persist_env_sp->symbols.clear();


    // run the code in the init:{} block (if any) -- handle any errors or special results
    // ----------------------------------------------------------------------------------

    if (verbset.verbs.at(0).init_block_sp.use_count() >= 1)
    {
        results_S block_results { }; 

        auto erc = eval_verbinit_block(verbset.verbs.at(0), block_results);


        // return with error, if error occurred during evaluation of init: block

        if (erc != 0)
        {
            results = error_results();
            return -1;  
        }


        // return with any normal or special results from the init: block execution
        
        results = block_results;  
        return 0;  
    }
    

    // no init: block to run -- return with unit results, since there is nothing else to return

    results = unit_results();                                        
    return 0; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @UNVERB "string" "string" ...
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_unverb(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional string parms  -- optional local: verbmain:, or global: parms (mutually exclusve)
    M__(M_out(L"verb_unverb() called");)
    int rc {0}; 


    // get global: and static: keyword   (default is local:)
    // -------------------------------

    auto global_rc    = get_right_keyword(expression, L"global"  );   // global_r/c   = -1, if global:    keyword is not present, r/c = 0 if present
    auto verbmain_rc  = get_right_keyword(expression, L"verbmain");   // verbmain_rc  = -1, if verbmain:  keyword is not present, r/c = 0 if present 
    auto static_rc    = get_right_keyword(expression, L"static"  );   // static_rc    = -1, if static:    keyword is not present, r/c = 0 if present 
    

    // -----------------------------------------
    // remove verbs in loop, if they are defined 
    // -----------------------------------------

    for (const auto& val : expression.rparms.values)
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
                     msgend_loc(val, expression);
                     rc = -1; 
                }
          
                if (is_global_identifier_removable(val.string))
                {
                    auto rrc = undef_global_verb(val.string);           // remove verb from verb table
                   
                    // errors are unexpected here
                   
                    if (rrc != 0)
                    {
                        //count_error(); already counted in undef_global_verb()
                        M_out_emsg1(L"verb_unverb() -- unexpected error from undef_global_verb() -- unable to undefine verb = %s") % val.string;
                        msgend_loc(val, expression);
                        rc = -1;  
                    } 
                }
                else
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove (built-in?) global verb = %s") % val.string;
                     msgend_loc(val, expression);
                     rc = -1; 
                }
            } 
            else
            {
                count_error(); 
                M_out_emsg1(L"verb_unverb() -- cannot remove global verb = %s, because global identifier %s is not defined") % val.string % val.string;
                msgend_loc(val, expression);
                rc = -1;        
            }
        }
        else if (static_rc == 0)
        {
            // remove verb from static environment
            // -----------------------------------
        
            if (is_static_identifier_defined(frame, val.string))               // see if this identifier is currently defined -- static scope only
            {
                if (!is_static_identifier_verb(frame, val.string))
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove static verb = %s, because identifier %s in static environment is not a verb") % val.string % val.string;
                     msgend_loc(val, expression);
                     rc = -1; 
                }
          
                if (is_static_identifier_removable(frame, val.string))        // local verbs should never be built-in ????
                {
                    auto rrc = undef_static_verb(frame, val.string);           // remove verb from static environment
                   
                    // errors are unexpected here
                   
                    if (rrc != 0)
                    {
                        //count_error(); already counted in undef_local_verb()
                        M_out_emsg1(L"verb_unverb() -- unexpected error from undef_static_verb() -- unable to undefine verb = %s") % val.string;
                        msgend_loc(val, expression);
                        rc = -1;  
                    } 
                }
                else                                                        // shouldn't get here
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove static (built-in?) verb = %s") % val.string;
                     msgend_loc(val, expression);
                     rc = -1; 
                }
            } 
            else
            {
                count_error(); 
                M_out_emsg1(L"verb_unverb() -- cannot remove static verb = %s, because identifier %s is not defined in static environment") % val.string % val.string;
                msgend_loc(val, expression);
                rc = -1;        
            }
        }
        else if (verbmain_rc == 0)
        {
            // remove verb from verbmain environment
            // -------------------------------------
        
            if (is_verbmain_identifier_defined(frame, val.string))               // see if this identifier is currently defined -- verbmain scope only
            {
                if (!is_verbmain_identifier_verb(frame, val.string))
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove verbmain verb = %s, because identifier %s in verbmain environment is not a verb") % val.string % val.string;
                     msgend_loc(val, expression);
                     rc = -1; 
                }
          
                if (is_verbmain_identifier_removable(frame, val.string))       // verbmain verbs should never be built-in ????
                {
                    auto rrc = undef_verbmain_verb(frame, val.string);         // remove verb from verbmain environment
                   
                    // errors are unexpected here
                   
                    if (rrc != 0)
                    {
                        //count_error(); already counted in undef_verbmain_verb()
                        M_out_emsg1(L"verb_unverb() -- unexpected error from undef_verbmain_verb() -- unable to undefine verb = %s") % val.string;
                        msgend_loc(val, expression);
                        rc = -1;  
                    } 
                }
                else                                                        // shouldn't get here
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove verbmain (built-in?) verb = %s") % val.string;
                     msgend_loc(val, expression);
                     rc = -1; 
                }
            } 
            else
            {
                count_error(); 
                M_out_emsg1(L"verb_unverb() -- cannot remove verbmain verb = %s, because identifier %s is not defined in verbmain environment") % val.string % val.string;
                msgend_loc(val, expression);
                rc = -1;        
            }
        }
        else
        {
            // remove verb from local environment (frameblock or verb)
            // ----------------------------------
        
            if (is_local_identifier_defined(frame, val.string))               // see if this identifier is currently defined -- local scope only
            {
                if (!is_local_identifier_verb(frame, val.string))
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove local verb = %s, because identifier %s in local environment is not a verb") % val.string % val.string;
                     msgend_loc(val, expression);
                     rc = -1; 
                }
          
                if (is_local_identifier_removable(frame, val.string))        // local verbs should never be built-in ????
                {
                    auto rrc = undef_local_verb(frame, val.string);           // remove verb from local environment
                   
                    // errors are unexpected here
                   
                    if (rrc != 0)
                    {
                        //count_error(); already counted in undef_local_verb()
                        M_out_emsg1(L"verb_unverb() -- unexpected error from undef_local_verb() -- unable to undefine verb = %s") % val.string;
                        msgend_loc(val, expression);
                        rc = -1;  
                    } 
                }
                else                                                        // shouldn't get here
                {
                     count_error(); 
                     M_out_emsg1(L"verb_unverb() -- cannot remove local (built-in?) verb = %s") % val.string;
                     msgend_loc(val, expression);
                     rc = -1; 
                }
            } 
            else
            {
                count_error(); 
                M_out_emsg1(L"verb_unverb() -- cannot remove local verb = %s, because identifier %s is not defined in local environment") % val.string % val.string;
                msgend_loc(val, expression);
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
//    make_expression() -- common code for @CALL and @XCTL 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

a_expression_S make_expression(frame_S& frame, const e_expression_S& expression) try
{

    // set up simulated token with verb name  -- selected fields only (type, sigils are unknown) -- location from verbname string parm 

    token_C token {                           };
    value_S value { expression.rparms.values.at(0) };                    // extract verb name string from right parmlist
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


    // add verb name to new simulated expression, using simulated token just constructed 

    a_expression_S simulated_expression { };                              // simulated expression for calling verb
    verbdef_S verbdef   { };                                              // empty verbdef for simulated expression
    verbset_S verbset   { };                                           
    verbset.verbs.push_back(verbdef);                                     // make dummy verbset_S 

    expression_set_verb(frame, simulated_expression, token, verbset);     // set verb and related fields in new simulated expression 
    simulated_expression.verb_value.token_ix1 = value.token_ix1;          // replace inaccurate verb token index with token index from verb name string in right parms 
    simulated_expression.verb_value.token_ix2 = value.token_ix2;          // replace inaccurate verb token index with token index from verb name string in right parms 


    // get left and right vlists, if they are supplied -- add them to simulated expression 

    value_S left_value  { };                                              // should be vlist, or no value 
    value_S right_value { };                                              // should be vlist, or no value 
    auto lrc = get_right_keyword(expression, L"left" ,  left_value);      // r/c = -1, if left: not present 
    auto rrc = get_right_keyword(expression, L"right", right_value);      // r/c = -1, if left: not present 
                                                                       
    if (lrc == 0)                                                      
        simulated_expression.lparms = *(left_value.vlist_sp);             // get non-empty vlist from value 
    if (rrc == 0)                                                    
        simulated_expression.rparms = *(right_value.vlist_sp);            // get non-empty vlist from value 


    //  try to get best token indexes for expression based on vlists and verb name string

    simulated_expression.token_ix1 = simulated_expression.verb_value.token_ix1; 
    simulated_expression.token_ix2 = simulated_expression.verb_value.token_ix2; 


    if ( (expression.lparms.token_ix1 >= 0) && (expression.rparms.token_ix1 >= 0) )
    {
        simulated_expression.token_ix1 = std::min(simulated_expression.token_ix1, expression.lparms.token_ix1);
        simulated_expression.token_ix1 = std::min(simulated_expression.token_ix1, expression.rparms.token_ix1);     
    }
    else if (expression.lparms.token_ix1 >= 0)
    {
        simulated_expression.token_ix1 = std::min(simulated_expression.token_ix1, expression.lparms.token_ix1);
    }
    else if (expression.rparms.token_ix1 >= 0)
    {
        simulated_expression.token_ix1 = std::min(simulated_expression.token_ix1, expression.rparms.token_ix1);
    }
     
    if ( (expression.lparms.token_ix2 >= 0) && (expression.rparms.token_ix2 >= 0) )
    {
        simulated_expression.token_ix2 = std::max(simulated_expression.token_ix2, expression.lparms.token_ix2);
        simulated_expression.token_ix2 = std::max(simulated_expression.token_ix2, expression.rparms.token_ix2);     
    }
    else if (expression.lparms.token_ix2 >= 0)
    {
        simulated_expression.token_ix2 = std::max(simulated_expression.token_ix2, expression.lparms.token_ix2);
    }
    else if (expression.rparms.token_ix2 >= 0)
    {
        simulated_expression.token_ix2 = std::max(simulated_expression.token_ix2, expression.rparms.token_ix2);
    }
  
    M__(display_expression(simulated_expression, L"verb_call");)


    //  return with completed simulated expression

    return simulated_expression; 
}
M_endf




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @CALL "string" left:[vlist] right:[vlist] -- call verb "string" with left and right vlists, as supplied 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_call(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional string parms
    M__(M_out(L"verb_call() called");)


    // create simulated expression based on @CALL input parms

    a_expression_S simulated_expression  { make_expression(frame, expression) };
    results_S call_results               {                                    }; 

     
    // execute verb, using new simulated expression  
 
    auto erc = eval_expression(frame, simulated_expression, call_results);

    
    // return normally, or with error

    if (erc != 0) 
        results = error_results();                                     // output results = error    
    else
        results = call_results;                                        // output results are from eval_expression -- may have special results (none are handled here)
   
    return erc; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @XCTL "string" left:[vlist] right:[vlist] -- set up to xfer to verb "string" with left and right vlists, as supplied 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_xctl(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional string parms
    M__(M_out(L"verb_xctl() called");)
 
 

    // create simulated expression based on @CALL input parms

    a_expression_S simulated_expression { make_expression(frame, expression) };


    // set up xctl special results, using new simulated expression as the results value + xctl flag to set up deferred call  

    results = to_results(expression_val(simulated_expression));          // set up results with copied-over simulated expression
   
    results.special_results = true;                                      // indicate special flags are set
    results.xctl_flag       = true;                                      // cause any nested frameblocks in verb-block to end, then do verb call with simulated expression
    
    M__(M_out(L"verb_xctl() returning after setting up xctl results");)
    

    // return with special xctl results

    return 0; 
}
M_endf





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @IMBED file:"filename"   -or-     @IMBED string:"text to be imbedded"                                                                                                            
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_imbed(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that either string: or file: string keywords are present (but not both)
    M__(M_out(L"verb_imbed() called");)
 
    value_S file_value   {};                                                          // file:   should be string 
    value_S string_value {};                                                          // string: should be string 


    // imbed file, if file: was present
    // --------------------------------

    if (0 == get_right_keyword(expression, L"file", file_value))                    // file: keyword  present? 
    {
        M__(M_out(L"verb_imbed() -- file= \"%S\"") % file_value.string; )

        auto arc = pending_attach_ext(file_value.string); 
        if (arc != 0)
        {
            M_out_emsg1(L"verb_imbed() -- error from pending_attach_ext() for file \"%S\" -- @IMBED called outside of pre-processor text (in plain code)") % file_value.string; 
            msgend_loc(expression);
            results = error_results();                                                               // return error
            return -1;     
        }
    }


    // imbed string, if file: was present
    // --------------------------------

    if (0 == get_right_keyword(expression, L"string", string_value))                    // string: keyword  present? 
    {
        M__(M_out(L"verb_imbed() -- string= \"%S\"") % string_value.string; )

        auto arc = pending_attach_ext(string_value.string, L"string from @IMBED"); 
        if (arc != 0)
        {
            M_out_emsg1(L"verb_imbed() -- error from pending_attach_ext() for string id = \"%S\" -- @IMBED called outside of pre-processor text (in plain code)") % L"ID not supported yet"; 
            msgend_loc(expression);
            results = error_results();                                                               // return error
            return -1;     
        } 
    }


    // pending_attach_ext() OK -- all this means is that we are within pre-processor text -- any file I/O errors will be detected later

    results = no_results(); 
    return 0; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @SKIP                                                                                                             
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_skip(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M_y(M_out(L"verb_skip() called");)
 
        
    // return with special @SKIP results -- will cause frameblocks, blocks, etc. to be ended until pre-processor sees/handles it

    results = special_results();
    results.skip_flag       = true;
                                                       // don't set target label
    return 0; 
}
M_endf
         
 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @SKIPTO "label"                                                                                                             
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_skipto(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional string parm = target label
    M__(M_out(L"verb_skipto() called");)

    auto to_label = M_get_right_pos_string(expression, 0);   
    set_skip_ext(to_label);                                  // maybe let pre-processor do this ???????????????????????? 
    

    // return with special @SKIPTO results -- will cause frameblocks, blocks, etc. to be ended until pre-processor sees/handles it

    results = special_results();
    results.skip_flag       = true;
    results.str             = to_label;                      // @SKIPTO target label
    return 0; 
}
M_endf
         
   



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @IMPORT "DLL name" (without "verbexx_" prefix, and without .dll filetype)                                                                                                            
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_import(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional string parm = DLL name 
    M__(M_out(L"verb_import() called");)
 

    // extract DLL name from parms and put in "verbexx_" prefix
    // --------------------------------------------------------
    
    std::wstring dll_name { L"verbexx_" + M_get_right_pos_string(expression, 0) };
   

    // import the DLL, and complain about any errors
    // ---------------------------------------------

    auto irc = import_dll(dll_name, L"");   
    if (irc != 0)
    {
         M__(M_out(L"verb_import() -- error: bad R/C from import_dll(\"%S\", \"\"") % dll_name;) 
         M_out_emsg1(L"verb_import() -- error from import_dll() for dll = \"%S\"") % dll_name; 
         msgend_loc(expression);
         results = error_results();          
         return -1;   
    }


    // DLL load was OK -- return no results
    // ------------------------------------

    results = no_results();
    return 0; 
}
M_endf
                                           


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @PARSE "string" name:"debug name string" -- parse "string"into block_S                                                                                                            
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_parse(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional string parm, and an optional name:"string" keyword/value 
    M__(M_out(L"verb_parse() called");)
 

    // extract string to be parsed, and debugging name from input parms
    // ----------------------------------------------------------------
    
    value_S str_value { };
    auto str_rc = get_right_positional(expression, str_value, 0);
    std::wstring parse_ws { str_value.string };

    //std::wstring parse_ws = M_get_right_pos_string(expression, 0);            

    // default debug string = "parsed string << location info for parsed string >>"   

    std::wstring debug_name { L"parsed string <<" };
    debug_name += (value_loc_str(str_value) + L">>"); 


    M_get_right_keyword_string(expression, L"name", debug_name);  

    bool continue_running { false };                                                                           // default = end the run if parsing error is found
    M_get_right_keyword_nval(expression, L"continue", continue_running, true)                                  // don't end the run if parsing error is found, if continue: keyword is present
                                                                                                             
                                                                                                             
    block_S out_block{ };                                                                                      // block_S to be filled in by parse_string()  
    auto prc = parse_string(frame, out_block, parse_ws, debug_name, continue_running);                         // if "continue" option, make sure to refresh everything after any error


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


    // no error or ignored error -- pass back results with block_S prom parsing (will be empty block_S after error)
    // ------------------------------------------------------------------------

    results = to_results(block_val(out_block));      
    return 0; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @SHELL "command string"   
//    @SHELL "command string" capture: 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_shell(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional string parm, and one optional capture: keyword
    M__(M_out(L"verb_shell() called");)
 

    // extract shell command string, and optional capture: keyword 
    // -----------------------------------------------------------
    
    std::wstring cmd_string = M_get_right_pos_string(expression, 0);            

    bool capture { false };                                               // default is to not return the output in a string
    M_get_right_keyword_nval(expression, L"capture", capture, true)       // use do_popen() and return shell output as string, if capture: keyword is present


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
        results = to_results(int64_val((int64_t)src));                    // results are r/c from system()
        return 0; 
    }
}

M_endf
    


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    ,  @SEP -- separate verb
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_separate(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are 1-N left-side and 1-N right-side positional parms of any type 

    M__(M_out(L"verb_separate() called");)

    vlist_S ret_vlist  {  };                                                                     // start with empty vlist as return value 

    auto c_rc = combine_vlists(ret_vlist, expression.lparms, expression.rparms, false);          // output =  left parms + right parms (verbatim flag = false -- get kws from (l/r)parms.eval_kws, but put into ret_vlist.keywords) 
    if (c_rc != 0)
    {
        M_out_emsg1(L"verb_separate() -- unexpected error from combine_vlists() -- separator verb (\",\") failed"); 
        M_out(L"     lparms location -- %s" ) %  vlist_loc_str(expression.lparms);
        M_out(L"     rparms location -- %s" ) %  vlist_loc_str(expression.rparms);
        msgend_loc(expression);
        results = error_results();                                                               // return error
        return -1;     
    } 

    M__(display_vlist(ret_vlist, L"verb_separate() -- output from , verb");)
    results = to_results(vlist_val(ret_vlist));                                                  // place output vlist in results 
    results.multiple_results = true;                                                             // indicate that multiple results are being returned
    return 0;
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @GOTO -- go to label in current or enclosing (active) block
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_goto(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side string parm (required parm)

    M__(M_out(L"verb_goto() called");)

    results                 = special_results(); 
    results.goto_flag       = true;
    results.str             = M_get_right_pos_string(expression, 0); // @GOTO target label = 1st right string parm

    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    make_results() -- internal function to fill in results value given passed-in expression with optional result values 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void make_results(const e_expression_S expression, results_S& results) try
{
    // handle no results value case

    if ( (expression.rparms.value_ct == 0) && (expression.rparms.eval_kws.empty()) )        // no values and no keywords?                    
    {
        results = results_S { };                                                            // uninitialized results with no vlist
        results.multiple_results = true;                                                    // indicate multiple results (with no vlist) = 0 values in calling expression/vlist
    }


    // handle single results value case

    else if  ( (expression.rparms.value_ct == 1) && (expression.rparms.eval_kws.empty()) )  // only 1 value and no keywords?
    {
        results = to_results(expression.rparms.values.at(0));                               // single @RETURN value = 1st right parm
    }


    // handle multiple results values case (or results with keywords) 

    else
    {
        vlist_S results_vlist  {  };                                 // start with empty vlist as results value 
                                                               
        results_vlist = expression.rparms;                           // put all right-side parms into results vlist_S 
        results = to_results(vlist_val(results_vlist));              // place all right-side positional parms in vlist with multiple results 
        results.multiple_results = true;                             // indicate that multiple results are being returned
    } 
    

    // make sure results value is unshared

    unshare_value(results);

    return;
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @BREAK -- break out of @LOOP (block)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_break(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are no left-side parms  -- may be optional results values on right side 

    M__(M_out(L"verb_break() called");)


    // set up @BREAK-type special results

    make_results(expression, results);                               // set up 0, 1, N values in @BREAK results 
    results.special_results = true;                                  // indicate special flags are set
    results.break_flag      = true;                                  // indicate these are @BREAK results
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @QUIT -- end evaluation of lowest enclosing @BLOCK {frameblock} immediately 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_quit(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are no left-side parms  -- may be optional results values on right side

    M__(M_out(L"verb_quit() called");)
    
        
    // set up @QUIT-type special results

    make_results(expression, results);                               // set up 0, 1, N values in @QUIT results 
    results.special_results = true;                                  // indicate special flags are set
    results.quit_flag       = true;                                  // indicate these are @QUIT results
    return 0; 
}
M_endf
 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @LEAVE  -or- @LEAVE "target" -- end evaluation of current or enclosing (active) block
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_leave(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is 0 or 1 left-side string parms (optional target parm) -- may be optional results values on right side

    M__(M_out(L"verb_leave() called");)


    // set up @LEAVE-type special results

    make_results(expression, results);                               // set up 0, 1, N values in @LEAVE results      
    results.special_results = true;                                  // indicate special flags are set
    results.leave_flag      = true;                                  // indicate these are @LEAVE results

    if (expression.lparms.value_ct > 0)                              // @LEAVE target supplied?  
        results.str = M_get_left_pos_string(expression, 0);          // @LEAVE target label = 1st (only) left string parm
    else
        results.str = L"";                                           // no @LEAVE target -- set empty std::wstring

    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @RETURN value -- return from current enclosing verb-block 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_return(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is 0 or 1 right-side string parms (optional return value)

    M__(M_out(L"verb_return() called");)


    // set up @RETURN-type special results

    make_results(expression, results);                               // set up 0, 1, N values in @RETURN results                                    
    results.special_results = true;                                  // indicate special flags are set
    results.return_flag     = true;                                  // indicate these are @RETURN results
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @THROW value -- throw exception to nearest @TRY/catch: handler 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_throw(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is 1 right-side string parm (required @THROW value)

    M__(M_out(L"verb_throw() called");)

    results                 = results_S { to_results(expression.rparms.values.at(0)) };  // "plain" results from 1st positional right-side parm  
    results.special_results = true;                                 
    results.throw_flag      = true;                                                      // indicate results are from throw
    return 0; 
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @RETHROW value -- throw exception to nearest @TRY/catch: handler -- value is 1st right positional parm for enclosing frameblock 
//                   -- @RETHROW is meant for use in catch: frameblocks
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_rethrow(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
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
//    @TRY {block} catch:{block} -- run block and catch any escaping @THROW results 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_try(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is 1 required right-side block parm, and a catch: keyword parm with block value

    M__(M_out(L"verb_try() called");)


    // @TRY -- always run 1st positional parm as block -- save @TRY results for results prioritization   

    results_S try_results { };                                                            // results from block evaluation -- passed back directly unless @THROW happened
    auto try_rc = eval_block(frame, *(expression.rparms.values.at(0).block_sp), try_results); // run @TRY block, and save r/c results 

    results_S        catch_results     {     };                                           // set up uninitialized catch:   results -- for results prioritization, if no catch: keyword 
    results_S        finally_results   {     };                                           // set up uninitialized finally: results -- for results prioritization, if no finally: keyword   
    decltype(try_rc) catch_rc          {0    };                                           // set up zero  catch:   r/c             -- for results prioritization, if no catch: keyword 
    decltype(try_rc) finally_rc        {0    };                                           // set up zero  finally: r/c             -- for results prioritization, if no finally: keyword 
    bool             catch_done        {false};                                           // true, if catch:  block was evaluation
    bool             finally_done      {false};                                           // true if finally: block was evaluated


    // run catch: block, if @THROW occurred during @TRY evaluation, and catch: keyword is present

    if ( (try_rc == 0) && (try_results.throw_flag) )                                      // @THROW results escaped @TRY block evaluation? 
    {                                                                                     
        // see if catch: keyword is present 

        value_S catch_value {};                                                           // should be block, or unit 
        if (0 == get_right_keyword(expression, L"catch", catch_value))                    // catch: keyword  present? 
        {   
            // run catch: block as frameblock with 1st right positional parm = throw value from @TRY block evaluation

            vlist_S right_vlist { };                                                      // right vlist parms for catch: frameblock 
            add_positional_value(right_vlist, try_results , true);                        // set 1st/only positional parm = value from @THROW results  

            catch_rc  = eval_frame_block(frame, vlist_S { }, right_vlist, *(catch_value.block_sp), catch_results);
            catch_done = true;
        }
    }
      

    // always run finally: block, if present, regardless of results from @TRY and perhaps catch:

    value_S finally_value {};                                                             // should be unit or block 
    if (0 == get_right_keyword(expression, L"finally", finally_value))                    // finally: keyword present? 
    {
        try_rc = eval_block(frame, *(finally_value.block_sp), finally_results);           // run finally: block and save r/c results 
        finally_done = true; 
    }


    // prioritize results from @TRY, catch: and finally: block evaluations 
    // -------------------------------------------------------------------
     
    if (!catch_done)                                                                     // catch: not run? 
    {
        if (!finally_done)                                                               // if no catch: or finally:
        {
            // only @TRY block was run -- pass back results from @TRY

            if (try_rc != 0)
                results = error_results(); 
            else
                results = try_results; 
            return  try_rc; 
        }
        else                                                                              // finally: , but no catch: -- return most important results (finally: wins any tie) 
        {
            if (
                 (finally_rc != 0)                                                        // error from finally: block?
                 ||                                                                       // -or-
                 (finally_results.special_results)                                        // special results from finally block?
                 ||                                                                       // -or-
                 ( (try_rc == 0) && (!try_results.special_results) )                      // normal results from @TRY block?        
               )                                                                          // results from finally: are more important than results from @TRY block ??
            {
                // ignore results from regular @TRY evaluation -- use finally: results
       
                if (finally_rc != 0)
                    results = error_results();
                else
                    results = finally_results; 
                return  finally_rc;       
            } 
            else                                                                          // unusual results from @TRY block are more important than normal results from finally: block 
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
            // only @TRY and catch: blocks were run -- pass back results from catch: -- ignore @THROW resuls from @TRY block

            if (catch_rc != 0)
                results = error_results(); 
            else
                results = catch_results; 
            return  catch_rc;        
        }
        else                                                                           // @TRY, and both catch: and finally: were run -- return most important result (finally: wins any tie)
        {
            if (
                 (finally_rc != 0)                                                     // error from finally: block?
                 ||                                                                    // -or-
                 (finally_results.special_results)                                     // special results from finally block?
                 ||                                                                    // -or-
                 ( (catch_rc == 0) && (!catch_results.special_results) )               // normal results from catch: block?        
               )                                                                       // results from finally: are more important than results from catch: block ??
            {
                // ignore results from catch: evaluation -- use finally: results
       
                if (finally_rc != 0)
                    results = error_results();
                else
                    results = finally_results; 
                return  finally_rc;       
            } 
            else                                                                       // unusual results from catch: block are more important than normal results from finally: block 
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
//    @CONTINUE -- break out of block in loop -- but continue looping 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_continue(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_continue() called");)

    results                 = special_results(); 
    results.ignore_results  = true;                      // preserve any saved block execution results from prior expression evaluation
    results.continue_flag   = true;
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @END -- end evaluation of main block imediately 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_end(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // there may be an optional int32 right positional parm -- wmain() R/C

    int64_t wmain_rc = M_get_right_pos_int64_d(expression, 0, 0 );      // default wmain() R/C is 0 

    M__(M_out(L"verb_end() called");)

    results                 = special_results(); 
    results.int32           = (int32_t)wmain_rc;                       // known to be small enough due to 31-bit range check on @END verb definition
    results.end_flag        = true;
    return 0; 
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @EXIT -- issue exit() -- uncontrolled end 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_exit(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one optional right-side int32_t positional parm -- exit status
    // max of one of exit: _exit: quick_exit: abort: _Exit: terminate: present (exit: is default) 

    M__(M_out(L"verb_exit() called");)

    int32_t status = M_get_right_pos_int64_d(expression, 0, 1 );                       // default exit status is 1 = error

    auto _exit_rc       = get_right_keyword(expression, L"_exit"       );              // _exit_rc      = -1, if _exit:        keyword is not present, r/c = 0 if present
    auto _Exit_rc       = get_right_keyword(expression, L"_Exit"       );              // _Exit_rc      = -1, if _Exit:        keyword is not present, r/c = 0 if present
    auto abort_rc       = get_right_keyword(expression, L"abort"       );              // abort_rc      = -1, if abort:        keyword is not present, r/c = 0 if present
    auto quick_exit_rc  = get_right_keyword(expression, L"quick_exit"  );              // quick_exit_rc = -1, if quick_exit:   keyword is not present, r/c = 0 if present
    auto terminate_rc   = get_right_keyword(expression, L"terminate"   );              // terminate_rc   = -1, if quick_exit:   keyword is not present, r/c = 0 if present


    // invoke exit()-type routine as requested by caller

         if (_exit_rc        == 0 ) do__exit(     status);      
    else if (_Exit_rc        == 0 ) do__Exit(     status);
    else if (quick_exit_rc   == 0 ) do_quick_exit(status);
    else if (abort_rc        == 0 ) do_abort(     status);
    else if (terminate_rc    == 0 ) do_terminate( status);
    else                            do_exit(      status);      


    // following code is not reached
    // -----------------------------

    results                 = special_results(); 
    results.end_flag        = true;
    return 0; 
}
M_endf





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @ERROR -- increment error counter
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_error(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is no parms at all

    M__(M_out(L"verb_error() called");)  

    count_error();      
    results                 = no_results(); 
    return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @DO {block} -- block is required
//        continue:  
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_do(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional block

    M__(M_out(L"verb_do() called");)

    bool continue_running { false };                                                                      // default = end the run if parsing error is found
    M_get_right_keyword_nval(expression, L"continue", continue_running, true)                             // don't end the run if parsing error is found, if continue: keyword is present
           
    results_S do_results {}; 
    
    M__(M_out(L"verb_do() -- calling eval_block() ************************");)
    auto erc = eval_block(frame, *(expression.rparms.values.at(0).block_sp), do_results);                 // results (with any special flags) will be passed back directly (if no error)
    M__(M_out(L"verb_do() -- eval_block() returned -- rc=%d *****************") % erc;)
   
    if (do_results.multiple_results)
    {
        M__(M_out(L"verb_do() -- multiple results returned by eval_block()");)
    }


    // suppress the error r/c, if continue: option present -- substitute unit results
        
    if (erc != 0)
    {
        M__(M_out(L"verb_do() -- eval_block() returned error");)
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
        M__(M_out(L"verb_do() -- eval_block() returned OK -- continue:");)
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
//    @BLOCK left:[vlist] right:[vlist] {block} -- block is required, left: and right: are optional
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_block(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional block, and maybe left: and right: keywords, with vlist values

    M__(M_out(L"verb_block() called");)
        

    // get left: and right: vlists (if any)

    vlist_S   left_vlist    {}; 
    vlist_S   right_vlist   {}; 

    {
        value_S key_value {}; 
        auto grc = get_right_keyword(expression, L"left", key_value);  // r/c = -1, if left: keyword is not present 

        if (grc == 0)                                              // left: is present 
            left_vlist = *(key_value.vlist_sp); 
    } 
    {
        value_S key_value {}; 
        auto grc = get_right_keyword(expression, L"right", key_value); // r/c = -1, if right: keyword is not present 

        if (grc == 0)                                              // left: is present 
            right_vlist = *(key_value.vlist_sp); 
    } 


    // runs passed-in block in new stack frame, with parms from left: and right: keywords

    block_S   block              { *(expression.rparms.values.at(0)).block_sp }; 
    results_S frameblock_results {                                            };

    auto erc = eval_frame_block(frame, left_vlist, right_vlist, block, frameblock_results);
    if (erc != 0)
    {
        results = error_results();                               // error -- pass back error results 
        return erc;                                              // return with error r/c
    }

    results = frameblock_results;                                 // pass back any results (including all special flags)
    return 0;  
}
M_endf
 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @CASE when:(expression) when:(expression) ... when:(expression) {block} {block} ... {block} else:{block} 
//
//      -or-
//
//    @CASE when:(expression) {block} when:(expression) {block} ... when:(expression) {block} else:{block}
//
//
//  -- number of positional {block} parms must equal the number of when: keywords
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_case(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that all right-side positional parms are {block} and else: is {block}, and all when: keywords are integers or boolean (evaluated expressions)
    // also -- only one else: keyword, and no left parms at all

    results = unit_results();                                  // output value = unit (if no block executed) 

    M__(M_out(L"verb_case() called");)


    // complain, if number of when: keywords is not same as number of positional parms with {block}

    auto when_ct = expression.rparms.eval_kws.count(L"when");  // number of when: keywords
    auto pos_ct  = expression.rparms.values.size();            // number of positional parms ({block})         

    M__(M_out(L"when_ct=%d  pos_ct=%d") % when_ct % pos_ct;)

    if (when_ct != pos_ct)
    {
        M_out_emsg1(L"verb_case() -- number of when: keywords (%d) does not match the number of positional {block} parms (%d) -- case verb is not executed") % when_ct % pos_ct; 
        msgend_loc(expression.rparms, expression);
        results = error_results();                             // return error results
        return -1; 
    }


    // loop through all  when:  keywords and corresponding positional {block}s -- execute 1st matching one, then return
    // ----------------------------------------------------------------------------------------------------------------

    for (auto i = 0; i < pos_ct; i++)
    {
        // evaluate when: condition and evaluate corresponding block (returns immediately if error or special results flag is set)

        results_S when_results { }; 
        M_eval_cond(multimap_at(expression.rparms.eval_kws, std::wstring{L"when"}, i), L"@CASE when:", when_results)     // handles any special results from the when: evaluation

        if (is_value_true(when_results))                  // is condition "true" ?
        {
            results_S block_results {}; 

            auto erc = eval_block(frame, *(expression.rparms.values.at(i).block_sp), block_results);    // execute the matching {block} in the positional parms
            if (erc != 0) 
            {
                results = error_results();                // replace eval results with error results, becahse of bad R/C
                return erc;
            }


            // continue looping through when: and blocks if continue flag is set in block evaluation results

            if (block_results.continue_flag == true)
            {
                continue; 
            }
            else
            {
                if (block_results.break_flag)                //    ??????????????????????????????
                {
                    block_results.break_flag      = false;   // make sure break flag is not set before returning
                    block_results.special_results = false;   // make sure break flag is not set before returning     
                }  
                results = block_results;                     // other special results flags (error, leave, etc.) may still be on  
                return 0;
            }
        }
    }


    // loop fell through -- no matching when: tests (or last when: was continued) -- execute any {block} for else: keyword (if present)
     
    if (expression.rparms.eval_kws.count(L"else") > 0)
    {
        results_S block_results {};
        auto erc = eval_block(frame, *(multimap_at(expression.rparms.eval_kws, std::wstring{L"else"}).block_sp), block_results); 
        if (erc != 0) 
        {
            results = error_results();                      // replace eval results with error results, becahse of bad R/C
            return erc;
        }
        else
        {
            if (block_results.break_flag || block_results.continue_flag)
            {
                block_results.continue_flag   = false;      // make sure continue flag is not set before returning 
                block_results.break_flag      = false;      // make sure break flag is not set before returning
                block_results.special_results = false;      // make sure flag is not set before returning 
            }                                              
            results = block_results;                        // other special results flags (error, leave, etc.) may still be on               
            return 0; 
        }
    }
    

    // must be no block was executed -- return with unit results ??????

    results = unit_results(); 
    return 0; 
}
M_endf
   

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @IF   int-value then: {block} else: {block} -- both keywords are optional
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_if(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional integer parm -- then: and else: keywords are block's (if present)

    int rc = 0; 
    results_S if_results { unit_results() };                           // output value = unit (if no block executed) 
    M__(M_out(L"verb_if() called");)

    if ( is_value_true(expression.rparms.values.at(0)) )               // test condition is non-zero (true) ?   
    {    
        value_S key_value { }; 
        auto grc = get_right_keyword(expression, L"then", key_value);  // r/c = -1, if then: keyword is not present  
        if (grc == 0)                                                  // then: is present 
        {
            auto erc = eval_block(frame, *(key_value.block_sp), if_results); 
            if (erc != 0)
            {
                if_results = error_results(); 
                rc = erc; 
            }
        }
    }
    else                                                               // test condition is 0 (false)
    {
        value_S key_value { };   
        auto grc = get_right_keyword(expression, L"else", key_value);  // r/c = -1, if then: keyword is not present 
        if (grc == 0)                                                  // then: is present 
        {
            auto erc = eval_block(frame, *(key_value.block_sp), if_results); 
            if (erc != 0)
            {
                if_results = error_results(); 
                rc = erc; 
            }  
        }    
    }

    results = if_results;     // output results, directly from any successful then: or else: block evaluation -- special results flags mey be set
    return rc; 
}
M_endf
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @LOOP while:int-val {block} until:int-val -- block required
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_loop(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional block parm -- while: or until: kws may be present and is not yet evaluated (if identifier or nested expression) 

    M__(M_out(L"verb_loop() called");)

    results_S loop_results { unit_results() };                             // output value = unit (in case no blocks are executed)
                                                                           
    value_S while_value {};                                                // unevaluated value of while: keyword  
    value_S until_value {};                                                // unevaluated value of until: keyword 
    value_S init_value  {};                                                // block value of init: keyword 
    value_S next_value  {};                                                // block value of next: keyword 

    auto while_rc = get_right_keyword(expression, L"while", while_value);  // while_rc is -1, if no while: condition
    auto until_rc = get_right_keyword(expression, L"until", until_value);  // until_rc is -1, if no until: condition
    auto init_rc  = get_right_keyword(expression, L"init" , init_value );  // init_rc is -1 , if no init:  condition
    auto next_rc  = get_right_keyword(expression, L"next" , next_value );  // next_rc is -1 , if no next:  condition

    M__(display_value(while_value, L"while_value");)
    M__(display_value(until_value, L"until_value");)


    // always execute init: block (if present) before starting loop body
    // -----------------------------------------------------------------

    if (init_rc == 0)                                            // init: is present?
    {
        results_S init_results {}; 
        auto erc = eval_block(frame, *(init_value.block_sp), init_results); 
        if (erc != 0)
        {
            results = error_results(); 
            return erc;                                         // if error -- return error r/c and error results
        }  


        // handle special results  (support only continue (no-op) and break (don't run loop body at all) -- percolate all the others)

        if (init_results.special_results)
        {
            // if @BREAK -- just return with @BREAK results -- don't run loop body 

            if (init_results.break_flag)
            {
                init_results.break_flag      = false;       // consume the @BREAK results
                init_results.special_results = false; 
                results = init_results;                     // init block results  -- return value(s) from @BREAK
                return 0; 
            }


            // if not @CONTINUE, just percolate special results from init: block evaluation

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


        // ----------------------------------------------
        // while: is non-zero (true) -- execute loop body
        // ----------------------------------------------

        results_S block_results { };
        auto erc = eval_block(frame, *(expression.rparms.values.at(0).block_sp), block_results); 

        if (erc != 0)
        {
            M_out_emsg1(L"verb_loop() -- error running loop body -- loop ends"); 
            msgend_loc(expression);
            results = error_results();     // pass back error results 
            return -1; 
        }


        // end loop without evaluating until: if @BREAK ended the block evaluation  

        if (block_results.break_flag)
        {
            block_results.break_flag      = false;       // consume the @BREAK results
            block_results.special_results = false; 
            results = block_results;                     // pass back results from the @BREAK
            return 0;  
        }

        // if any other special flag is on besides @CONTINUE, return results with special flag(s) -- loop ends immediately without evaluating until: condition 

        if ( (block_results.special_results) && (!block_results.continue_flag) )
        {
            results = block_results; 
            return 0; 
        }


        // if block did not end because of @CONTINUE, save away block evaluation results to be used for @LOOP results, if there are no more normal loop passses   

        if (!block_results.continue_flag)
            loop_results = block_results;       // normal block completion -- save away results, in case this is last one  


        // end loop, if until: condition is true 
      
        if (until_rc == 0)
        {
            results_S until_results { }; 
            M_eval_cond(until_value, L"@LOOP until:", until_results)
            if ( is_value_true(until_results) )
                break; 
        }


        // execute next: block (if present) before starting next loop pass (which includes checking the :while results) 
        // ------------------------------------------------------------------------------------------------------------

        if (next_rc == 0)                                            // init: is present 
        {
            results_S next_results {}; 
            auto erc = eval_block(frame, *(next_value.block_sp), next_results); 
            if (erc != 0)
            {
                results = error_results(); 
                return erc;                                          // if error -- return error r/c and error results
            }  
       

            // handle special results  (support only continue (no-op) and break (don't run loop body at all) -- percolate all the others)
       
            if (next_results.special_results)
            {
                // if @BREAK -- end loop as if break occurred during the main block evaluation
       
                if (next_results.break_flag)
                {
                    next_results.break_flag      = false;            // consume the @BREAK results
                    next_results.special_results = false; 
                    results = next_results;                          // end the loop -- return results from @BREAK 
                    return 0;                                        
                }
                         
       
                // if not @CONTINUE, just percolate special results from next: block evaluation
       
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


    // return, with set up results from block evaluation -- should not have any unusual flags on 

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