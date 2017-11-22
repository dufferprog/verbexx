// ex_verb_util.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ================
////     ex_verb_util.cpp -- miscellaneous/utility pre-defined verbs                          
////     ================
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
////     @VAR
////     @CONST
////     @UNVAR
////     @NOEVAL
////     @EVAL
////     @UNSHARE
////     //@EXPOSE
////     //@UNEXPOSE
////     @GETENV
////     @STR
////     @SAY
////     @STDOUT
////     @STDERR
////     @STDIN
////     @INTERPOLATE 
////     @FORMAT
////
////     @DISPLAY numerics:
////              locale: 
////              stack:
////              vars:[ ... ]   
////              verbs:[ ... ]
////              types:[ ... ]
////              builtin_verbs:
////              added_verbs: 
////              builtin_types:
////              added_types:
////              all_vars: 
////              id_cache:
////              statistics:
////              token_list:
////              etc.
////
////
////    
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
//║╳╳║      Externalized utility functions
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   eval_cond() -- helper function to evaluate conditional expression 
////                   
////
////   r/c =  0 -- OK    -- condition evaluated to an integer -- value of condition is in results.int64
////   r/c = -1 -- error -- caller should return with unit results and r/c = -1
////   r/c =  1 -- special exit condition occurred during evaluation -- caller return with results and r/c = 0  
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int eval_cond(frame_S& frame, const value_S& value, const e_expression_S& expression, const std::wstring& ws, results_S& results) try
{
    M__(M_out(L"eval_cond() called");)
    M__(display_value(value, L"cond_value");)


    results_S cond_results {};
    auto vrc = eval_value(frame, value, cond_results, parmtype_S {});      // use default parmtype_S to do full evaluation of this value
         
    M__(M_out(L"eval_condp(): r/c from eval_value() = %d") % vrc);
       

    // return with error, if while evaluation failed

    if (vrc != 0)
    {
        M_out_emsg1(L"eval_cond() -- cannot evaluate value for condition (%s) -- verb=%s execution ends immediately") % ws % verb_name(expression); 
        msgend_loc(cond_results, expression);
        M_verb_error0(results) 
    }


    // return with R/C = 1, if evaluation returned a special results (exit) flag -- do not cconsume special results

    if (cond_results.special_results)
    {
        results = cond_results; 
        return 1; 
    }
        

    // return with error, if cond results are not integer or boolean 
    
    M__(display_value(cond_results, L"cond_results");)
   
    if ( !is_value_integer(cond_results) && !(is_value_boolean(cond_results)) )
    {
        M_out_emsg1(L"eval_cond() -- evaluated value for %s keyword is not integer -- verb=%s execution ends immediately") % ws % verb_name(expression); 
        display_value(cond_results, L"bad " + ws + L" value");
        msgend_loc(cond_results, expression);
        M_verb_error0(results)
    }


    //  cond_results is an integer or boolean value -- return normally 

    results = cond_results; 
    return 0; 
}
M_endf



//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Variable-oriented verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @VAR identifier identifier ... identifier     value: general-value  global: local: verbmain: static: expose: share: unshare:    -- keywords optional
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_var(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional undefined identifier parms     and    maybe right value: and global: keywords

    M__(M_out(L"verb_var() called");)


    // fetch keyword values 

    value_S value_value  { };                                                   // value to be assigned -- will be unit if value: keyword not present 
    auto value_rc     = get_right_keyword(expression, L"value" , value_value );   // key_value will be unit, if value: keyword is not present 
    auto global_rc    = get_right_keyword(expression, L"global"              );   // rc = -1, if global:    keyword is not present
    auto static_rc    = get_right_keyword(expression, L"static"              );   // rc = -1, if static:    keyword is not present
    auto verbmain_rc  = get_right_keyword(expression, L"verbmain"            );   // rc = -1, if verbmain:  keyword is not present
#ifdef M_EXPOSE_SUPPORT
    auto expose_rc  = get_right_keyword(expression, L"expose"              );   // rc = -1, if expose:  keyword is not present  (assumed if this is a global variable) 
#endif
    auto unshare_rc = get_right_keyword(expression, L"unshare"             );   // rc = -1, if unshare: keyword is not present  


    // define new variables in loop -- each one initialized to value: 

    for (const auto& var : expression.rparms.values)
    {
        M__(M_out(L"verb_var() -- range for -- var.string = %s") % var.string; )

        if (global_rc == 0)
        {
            if (is_global_identifier_defined(var.string))
            {
                M_out_emsg1(L"@VAR -- global identifier \"%S\" is already defined -- unable to define it again") % var.string;
                M_verb_error1_loc(results, var, expression)      
            }
            else
            {
                def_parm_S  parm { };
                parm.unshare = (unshare_rc == 0); 

                auto drc = def_global_var(var.string, value_value, parm);                       // add new non-constant variable to environment -- global variables are always exposed
               
                // errors are unexpected here
               
                if (drc != 0)
                {
                    //count_error(); already counted in def_global_var()
                    M_out_emsg1(L"@VAR -- error from def_global_var() -- unable to define new global variable = %s") % var.string;
                    M_verb_error0_loc(results, var, expression)
                }  
            }
        }
        else if (static_rc == 0)          // static: present -- define static variable
        {
            if (is_static_identifier_defined(frame, var.string))
            {
                M_out_emsg1(L"@VAR -- static identifier \"%S\" is already defined -- unable to define it again") % var.string;
                M_verb_error1_loc(results, var, expression)      
            }
            else
            {
                def_parm_S  parm { };
                parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif


                auto drc = def_static_var(frame, var.string, value_value, parm);                       // add new non-constant variable to environment  -- expose based on expose: kw rc
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (drc != 0)
                {
                    //count_error(); already counted in def_static_var()
                    M_out_emsg1(L"@VAR -- error from def_static_var() -- unable to define new static variable = %s") % var.string;
                    M_verb_error0_loc(results, var, expression)  
                }  
            }
        }
        else if (verbmain_rc == 0)          // verbmain: present -- define verbmain variable
        {
            if (is_verbmain_identifier_defined(frame, var.string))
            {
                M_out_emsg1(L"@VAR -- verbmain identifier \"%S\" is already defined -- unable to define it again") % var.string;
                M_verb_error1_loc(results, var, expression)     
            }
            else
            {
                def_parm_S  parm { };
                parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif


                auto drc = def_verbmain_var(frame, var.string, value_value, parm);                       // add new non-constant variable to environment  -- expose based on expose: kw rc
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (drc != 0)
                {
                    //count_error(); already counted in def_verbmain_var()
                    M_out_emsg1(L"@VAR -- error from def_verbmain_var() -- unable to define new verbmain variable = %s") % var.string;
                    M_verb_error0_loc(results, var, expression) 
                }  
            }
        }
        else          // global:, verbmain:, and static: were not present -- define local: variable
        {
            if (is_local_identifier_defined(frame, var.string))
            {
                M_out_emsg1(L"@VAR -- local identifier \"%S\" is already defined -- unable to define it again") % var.string;
                M_verb_error1_loc(results, var, expression)     
            }
            else
            {
                def_parm_S  parm { };
                parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif


                auto drc = def_local_var(frame, var.string, value_value, parm);                       // add new non-constant variable to environment  -- expose based on expose: kw rc
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (drc != 0)
                {
                    //count_error(); already counted in def_local_var()
                    M_out_emsg1(L"@VAR -- error from def_local_var() -- unable to define new local variable = %s") % var.string;
                    M_verb_error0_loc(results, var, expression)
                }  
            }
        }
    }    


    // pass back variables -- not the values -- set up single results (1 variable present) or multiple results (0 or more than 1 variable present) 
    // -------------------------------------------------------------------------------------------------------------------------------------------
  
    if (expression.rparms.values.size() == 1)                                                                          // just one variable to be assigned
    {
        results = to_results(expression.rparms.values.at(0));                                                          // output value = single variable
    }
    else                                                                                                               // 0 or more than 1 value -- need to pass back multiple results
    {
        vlist_S vlist { };                                                                                             // clean results vlist -- no global:, value: keywords, etc. 
        vlist.values = expression.rparms.values;                                                                       // copy over just the right-side positional parms = all values to be assigned   
        results = to_results(vlist_val(vlist));                                                                        // convert vlist to results
        results.multiple_results = true;                                                                               // pass back vlist as multiple results
    }

    return 0; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @CONST   identifier   value: general-value   global: local: verbmain: static: expose:  unshare: share: -- value: keyword is required
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_const(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional undefined identifier parm     and    required right value: keyword  -- one optional global: keyword

    M__(M_out(L"verb_const() called");)

    std::wstring var_name { expression.rparms.values.at(0).string };           // get name of variable being defined 


    // add new constant with value set to "unit" or value from value: keyword


    // fetch keyword values 

    value_S value_value  { };                                                     // value to be assigned -- should get/set -- value: keyword is required 
    auto value_rc     = get_right_keyword(expression, L"value" , value_value );   // value: keyword should be present 
    auto global_rc    = get_right_keyword(expression, L"global"              );   // rc = -1, if global:   keyword is not present
    auto verbmain_rc  = get_right_keyword(expression, L"verbmain"            );   // rc = -1, if verbmain: keyword is not present
    auto static_rc    = get_right_keyword(expression, L"static"              );   // rc = -1, if static:   keyword is not present
#ifdef M_EXPOSE_SUPPORT
    auto expose_rc  = get_right_keyword(expression, L"expose"                );   // rc = -1, if expose:  keyword is not present  (always assumed, if this is global constant)
#endif
    auto unshare_rc = get_right_keyword(expression, L"unshare"               );   // rc = -1, if unshare: keyword is not present  

 
    // set global, static, verbmain, or local constant, depending on global: static: local: or verbmain: options

    if (global_rc == 0)
    {
        if (is_global_identifier_defined(var_name))
        {
            M_out_emsg1(L"@CONST -- global identifier \"%S\" is already defined -- unable to define it again") % var_name;
            M_verb_error1_loc(results, expression.rparms.values.at(0), expression)      
        }
        else
        { 
            def_parm_S parm { }; 
            parm.unshare  = (unshare_rc == 0);
                          
            auto drc = def_global_const(var_name, value_value, parm);              // add new constant variable to environment -- global constant always exposed
           
            // errors expected here include duplicate global variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
           
            if (drc != 0)
            {
                //count_error(); already counted in def_global_const()
                M_out_emsg1(L"@CONST -- error from def_global_const() -- unable to define new global constant = %s") % var_name;
                M_verb_error0_loc(results, expression.rparms.values.at(0), expression) 
            } 
        }
    }
    else if (static_rc == 0)          // static: present -- define static constant
    {
        if (is_static_identifier_defined(frame, var_name))
        {
            M_out_emsg1(L"@CONST -- static identifier \"%S\" is already defined -- unable to define it again") % var_name;
            M_verb_error1_loc(results, expression.rparms.values.at(0), expression)     
        }
        else
        { 
            def_parm_S parm { }; 
            parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
            parm.exposed  = (expose_rc  == 0);
#endif

            auto drc = def_static_const(frame, var_name, value_value, parm);                           // add new non-constant variable to environment -- expose based on expose: kw rc
          
            // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
          
            if (drc != 0)
            {
                //count_error(); already counted in def_static_const()
                M_out_emsg1(L"@CONST -- error from def_static_const() -- unable to define new static constant = %s") % var_name;
                M_verb_error0_loc(results, expression.rparms.values.at(0), expression) 
            } 
        }
    }
    else if (verbmain_rc == 0)          // verbmain: present -- define verbmain constant
    {
        if (is_verbmain_identifier_defined(frame, var_name))
        {
            M_out_emsg1(L"@CONST -- verbmain identifier \"%S\" is already defined -- unable to define it again") % var_name;
            M_verb_error1_loc(results, expression.rparms.values.at(0), expression)      
        }
        else
        { 
            def_parm_S parm { }; 
            parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
            parm.exposed  = (expose_rc  == 0);
#endif
            auto drc = def_verbmain_const(frame, var_name, value_value, parm);                         // add new non-constant variable to environment -- expose based on expose: kw rc
          
            // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
          
            if (drc != 0)
            {
                //count_error(); already counted in def_verbmain_const()
                M_out_emsg1(L"@CONST -- error from def_verbmain_const() -- unable to define new verbmain constant = %s") % var_name;
                M_verb_error0_loc(results, expression.rparms.values.at(0), expression) 
            } 
        }
    }
    else          // global: and static: were not present -- define local constant
    {
        if (is_local_identifier_defined(frame, var_name))
        {
            M_out_emsg1(L"@CONST -- local identifier \"%S\" is already defined -- unable to define it again") % var_name;
            M_verb_error1_loc(results, expression.rparms.values.at(0), expression)      
        }
        else
        { 
            def_parm_S parm { }; 
            parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
            parm.exposed  = (expose_rc  == 0);
#endif

            auto drc = def_local_const(frame, var_name, value_value, parm);                           // add new non-constant variable to environment -- expose based on expose: kw rc
          
            // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
          
            if (drc != 0)
            {
                //count_error(); already counted in def_local_const()
                M_out_emsg1(L"@CONST -- error from def_local_constr() -- unable to define new local constant = %s") % var_name;
                M_verb_error0_loc(results, expression.rparms.values.at(0), expression) 
            } 
        }
    }


    // return with value assigned to new constant
    // ------------------------------------------

    results = to_results(value_value);               // output results = value assigned to new constant 
    return 0; 
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @UNVAR identifier identifier ... identifier :global static: local: verbmain: -- keywords optional
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_unvar(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional defined identifier parms     and    maybe right global: keyword

    M__(M_out(L"verb_unvar() called");)


    // fetch global: verbmain: static: keywords 

    value_S global_value { };                                                  // not looked at 
    auto global_rc   = get_right_keyword(expression, L"global", global_value);   // rc = -1, if global:    keyword is not present
    auto verbmain_rc = get_right_keyword(expression, L"verbmain"            );   // rc = -1, if verbmain:  keyword is not present
    auto static_rc   = get_right_keyword(expression, L"static"              );   // rc = -1, if static:    keyword is not present


    // undefine variables in loop, if they are defined and non_constant 

    for (const auto& var : expression.rparms.values)
    {
        M__(M_out(L"verb_unvar() -- range for -- var.string = %s") % var.string; )

        if (global_rc == 0)                                              // if global: keyword present, undefine global variable
        {
            if (is_global_identifier_defined(var.string))                // don't bother, if global variable is already undefined 
            {
                if (is_global_identifier_const(var.string))
                {
                    M_out_emsg1(L"@UNVAR -- global variable \"%S\" is constant -- unable to undefine") % var.string;
                    M_verb_error1_loc(results, var, expression)    
                } 
                else if (is_global_identifier_verbset(var.string))
                {
                    M_out_emsg1(L"@UNVAR -- global identifier \"%S\" is a verb -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)  
                } 
                else if (is_global_identifier_typdef(var.string))
                {
                    M_out_emsg1(L"@UNVAR -- global identifier \"%S\" is a typedef -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)      
                } 
                else if (is_global_identifier_alias(var.string))
                {
                    M_out_emsg1(L"@UNVAR -- global identifier \"%S\" is an alias -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)     
                } 
                else if (!is_global_identifier_removable(var.string))
                {
                    M_out_emsg1(L"@UNVAR -- global identifier \"%S\" cannot be removed -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)       
                } 
                else
                {
                    auto urc = undef_global_var(var.string);    // undefine variable from global environment
               
                    // errors are unexpected here
                   
                    if (urc != 0)
                    {
                        //count_error(); already counted in def_global_var()
                        M_out_emsg1(L"@UNVAR -- unexpected error from undef_global_var() -- unable to undefine global variable = %s") % var.string;
                        M_verb_error0_loc(results, var, expression)  
                    } 
                }
            }
            else
            {
                M_out_emsg1(L"@UNVAR -- global identifier \"%S\" is not defined -- unable to undefine") % var.string;
                M_verb_error1_loc(results, var, expression)     
            }
        }
        else if (static_rc == 0)           // static: present -- undefine static variable
        {
            if (is_static_identifier_defined(frame, var.string))             // don't bother, if static identifier is already undefined 
            {
                if (is_static_identifier_const(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- static variable \"%S\" is constant -- unable to undefine") % var.string;
                    M_verb_error1_loc(results, var, expression)       
                } 
                else if (is_static_identifier_verbset(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- static identifier \"%S\" is a verb -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)     
                } 
                else if (is_static_identifier_typdef(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- static identifier \"%S\" is a typdef -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)       
                } 
                else if (is_static_identifier_alias(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- static identifier \"%S\" is an alias -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)      
                } 
                else if (!is_static_identifier_removable(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- static identifier \"%S\" cannot be removed -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)       
                } 
                else
                {
                    auto urc = undef_static_var(frame, var.string);           // undefine variable from static environment
                   
                    // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (urc != 0)
                    {
                        //count_error(); already counted in def_local_var()
                        M_out_emsg1(L"@UNVAR -- unexpected error from undef_local_var() -- unable to undefine static variable = %s") % var.string;
                        M_verb_error0_loc(results, var, expression)  
                    } 
                }
            }
            else
            {
                M_out_emsg1(L"@UNVAR -- static identifier \"%S\" is not defined -- unable to undefine") % var.string;
                M_verb_error1_loc(results, var, expression)   
            }
        }       
        else if (verbmain_rc == 0)           // verbmain: present -- undefine verbmain variable
        {
            if (is_verbmain_identifier_defined(frame, var.string))             // don't bother, if verbmain identifier is already undefined 
            {
                if (is_verbmain_identifier_const(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- verbmain identifier \"%S\" is constant -- unable to undefine") % var.string;
                    M_verb_error1_loc(results, var, expression)      
                } 
                else if (is_verbmain_identifier_verbset(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- verbmain identifier \"%S\" is a verb -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)      
                } 
                else if (is_verbmain_identifier_typdef(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- verbmain identifier \"%S\" is a typdef -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)       
                } 
                else if (is_verbmain_identifier_alias(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- verbmain identifier \"%S\" is an alias -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)        
                } 
                else if (!is_verbmain_identifier_removable(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- verbmain identifier \"%S\" cannot be removed -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)       
                } 
                else
                {
                    auto urc = undef_verbmain_var(frame, var.string);           // undefine variable from verbmain environment
                   
                    // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (urc != 0)
                    {
                        //count_error(); already counted in undef_verbdef_var()
                        M_out_emsg1(L"@UNVAR -- unexpected error from undef_verbdef_var() -- unable to undefine verbdef identifier = %s") % var.string;
                        M_verb_error0_loc(results, var, expression)  
                    } 
                }
            }
            else
            {
                M_out_emsg1(L"@UNVAR -- verbmain identifier \"%S\" is not defined -- unable to undefine") % var.string;
                M_verb_error1_loc(results, var, expression)  
            }
        }  
        else          // global: verbdef: and static: were not present -- undefine local variable
        {
            if (is_local_identifier_defined(frame, var.string))             // don't bother, if local identifier is already undefined 
            {
                if (is_local_identifier_const(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- local variable \"%S\" is constant -- unable to undefine") % var.string;
                    M_verb_error1_loc(results, var, expression)     
                } 
                else if (is_local_identifier_verbset(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- local identifier \"%S\" is a verb -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)       
                } 
                else if (is_local_identifier_typdef(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- local identifier \"%S\" is a typedef -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)        
                } 
                else if (is_local_identifier_alias(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- local identifier \"%S\" is a alias -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)      
                } 
                else if (!is_local_identifier_removable(frame, var.string))
                {
                    M_out_emsg1(L"@UNVAR -- local identifier \"%S\" cannot be removed -- unable to undefine using @UNVAR") % var.string;
                    M_verb_error1_loc(results, var, expression)        
                } 
                else
                {
                    auto urc = undef_local_var(frame, var.string);           // undefine variable from local environment
                   
                    // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (urc != 0)
                    {
                        //count_error(); already counted in def_local_var()
                        M_out_emsg1(L"@UNVAR -- unexpected error from undef_local_var() -- unable to undefine local variable = %s") % var.string;
                        M_verb_error0_loc(results, var, expression)  
                    } 
                }
            }
            else
            {
                M_out_emsg1(L"@UNVAR -- local identifier \"%S\" is not defined -- unable to undefine") % var.string;
                M_verb_error1_loc(results, var, expression)  
            }
        }        // global/local check
    }            // end of for() loop 


    // return normally, with no results
  
    results = no_results();                                        // output results = none  
    return 0; 
}
M_endf
 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                          ------
//    identifier identifier ... identifier  @ALIAS identifier identifier ... identifier      global: local: verbmain: static: expose:   -- keywords optional
//                                          ------
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_alias(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more left  positional undefined identifier parms
    // already known that there is one or more right positional undefined identifier parms     and    maybe right value: and global: keywords
    // same number of left and right positional parms 

    M__(M_out(L"verb_alias() called");)


    // fetch keyword values 

    value_S value_value  { };                                                      // value to be assigned -- will be unit if value: keyword not present 
    auto weak_rc      = get_right_keyword(expression, L"weak"                );    // rc = -1, if weak:      keyword is not present
    auto global_rc    = get_right_keyword(expression, L"global"              );    // rc = -1, if global:    keyword is not present
    auto static_rc    = get_right_keyword(expression, L"static"              );    // rc = -1, if static:    keyword is not present
    auto verbmain_rc  = get_right_keyword(expression, L"verbmain"            );    // rc = -1, if verbmain:  keyword is not present
#ifdef M_EXPOSE_SUPPORT
    auto expose_rc  = get_right_keyword(expression, L"expose"              );      // rc = -1, if expose:  keyword is not present  (assumed if this is a global alias) 
#endif
  

    // ----------------------------------------------------------------------------
    // define new aliases in loop -- each one "pointing" to an existing identifier: 
    // ----------------------------------------------------------------------------

    auto alias_ct = std::min(expression.lparms.values.size(), expression.rparms.values.size() );     // left and right counts should bethe same

    for (auto i = 0; i < alias_ct; i++)
    {
        auto alias_v = expression.lparms.values.at(i);
        auto ident_v = expression.rparms.values.at(i); 
        auto alias = alias_v.string; 
        auto ident = ident_v.string; 

        M__(M_out(L"@ALIAS -- i=%d  alias=\"%S\"  ident=\"%S\"") % i % alias % ident;)

        if (!is_identifier_defined(frame, ident))
        {
            M_out_emsg1(L"@ALIAS -- identifier \"%S\" is is not defined -- unable to create an alias \"%S\" for it") % ident % alias;
            M_verb_error1_loc(results, ident_v, expression)     
        }
        else if (global_rc == 0)
        {
            if (is_global_identifier_defined(alias))
            {
                M_out_emsg1(L"@ALIAS -- global identifier \"%S\" is already defined -- unable to define it again as an alias") % alias;
                M_verb_error1_loc(results, alias_v, expression)   
            }
            else if (!is_identifier_defined(frame, ident))
            {
                M_out_emsg1(L"@ALIAS -- identifier \"%S\" is is not defined -- unable to create an alias \"%S\" for it") % ident % alias;
                M_verb_error1_loc(results, ident_v, expression)       
            }
            else
            {
                def_parm_S  parm { };


                if (weak_rc == -1)
                {
                    auto drc = def_global_alias(ident, alias, parm);                       // add new non-constant alias to environment -- global alias are always exposed
                   
                    // errors are unexpected here
                   
                    if (drc != 0)
                    {
                        //count_error(); already counted in def_global_alias()
                        M_out_emsg1(L"@ALIAS -- error from def_global_alias() -- unable to define new global alias = %s") % alias;
                        M_verb_error0_loc(results, alias_v, expression)
                    } 
                }
                else
                {
                    auto drc = def_global_weak_alias(ident, alias, parm);                  // add new non-constant alias to environment -- global alias are always exposed
                   
                    // errors are unexpected here
                   
                    if (drc != 0)
                    {
                        //count_error(); already counted in def_global_weak_alias()
                        M_out_emsg1(L"@ALIAS -- error from def_global_weak_alias() -- unable to define new weak global alias = %s") % alias;
                        M_verb_error0_loc(results, alias_v, expression)
                    } 
                }
            }
        }
        else if (static_rc == 0)          // static: present -- define static alias
        {
            if (is_static_identifier_defined(frame, alias))
            {
                M_out_emsg1(L"@ALIAS -- static identifier \"%S\" is already defined -- unable to define it again as an alias") % alias;
                M_verb_error1_loc(results, alias_v, expression)   
            }
            else
            {
                def_parm_S  parm { };
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif
                if (weak_rc == -1)
                {
                    auto drc = def_static_alias(frame, ident, alias, parm);                       // add new non-constant alias to environment  -- expose based on expose: kw rc
                  
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                  
                    if (drc != 0)
                    {
                        //count_error(); already counted in def_static_alias()
                        M_out_emsg1(L"@ALIAS -- error from def_static_alias() -- unable to define new static alias = %s") % alias;
                        M_verb_error0_loc(results, alias_v, expression)
                    }  
                }
                else
                {
                    auto drc = def_static_weak_alias(frame, ident, alias, parm);                  // add new non-constant weak alias to environment  -- expose based on expose: kw rc
                  
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                  
                    if (drc != 0)
                    {
                        //count_error(); already counted in def_static_alias()
                        M_out_emsg1(L"@ALIAS -- error from def_static_weak_alias() -- unable to define new weak static alias = %s") % alias;
                        M_verb_error0_loc(results, alias_v, expression)
                    }  
                }
            }
        }
        else if (verbmain_rc == 0)          // verbmain: present -- define verbmain alias
        {
            if (is_verbmain_identifier_defined(frame, alias))
            {
                M_out_emsg1(L"@ALIAS -- verbmain identifier \"%S\" is already defined -- unable to define it again as an alias") % alias;
                M_verb_error1_loc(results, alias_v, expression)   
            }
            else
            {
                def_parm_S  parm { };
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif
                if (weak_rc == -1)
                {
                    auto drc = def_verbmain_alias(frame, ident, alias, parm);                       // add new non-constant alias to environment  -- expose based on expose: kw rc
                  
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                  
                    if (drc != 0)
                    {
                        //count_error(); already counted in def_verbmain_alias()
                        M_out_emsg1(L"@ALIAS -- error from def_verbmain_alias() -- unable to define new verbmain alias = %s") % alias;
                        M_verb_error0_loc(results, alias_v, expression)
                    }  
                }
                else
                {
                    auto drc = def_verbmain_weak_alias(frame, ident, alias, parm);                 // add new non-constant alias to environment  -- expose based on expose: kw rc
                  
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                  
                    if (drc != 0)
                    {
                        //count_error(); already counted in def_verbmain_alias()
                        M_out_emsg1(L"@ALIAS -- error from def_verbmain_weak_alias() -- unable to define new weak verbmain alias = %s") % alias;
                        M_verb_error0_loc(results, alias_v, expression)
                    }  
                }
            }
        }
        else          // global:, verbmain:, and static: were not present -- define local: alias
        {
            if (is_local_identifier_defined(frame, alias))
            {
                M_out_emsg1(L"@ALIAS -- local identifier \"%S\" is already defined -- unable to define it again as an alias") % alias;
                M_verb_error1_loc(results, alias_v, expression)   
            }
            else
            {
                def_parm_S  parm { };
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif

                if (weak_rc == -1)
                {
                    auto drc = def_local_alias(frame, ident, alias, parm);                       // add new non-constant alias to environment  -- expose based on expose: kw rc
                  
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                  
                    if (drc != 0)
                    {
                        //count_error(); already counted in def_local_alias()
                        M_out_emsg1(L"@ALIAS -- error from def_local_alias() -- unable to define new local alias = %s") % alias;
                        M_verb_error0_loc(results, alias_v, expression)
                    }  
                }
                else
                {
                    auto drc = def_local_weak_alias(frame, ident, alias, parm);                       // add new non-constant alias to environment  -- expose based on expose: kw rc
                  
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                  
                    if (drc != 0)
                    {
                        //count_error(); already counted in def_local_alias()
                        M_out_emsg1(L"@ALIAS -- error from def_local_weak_alias() -- unable to define new weak local alias = %s") % alias;
                        M_verb_error0_loc(results, alias_v, expression)
                    }                  
                }
            }
        }
    }    


    // normal return -- pass back aliases -- not the values -- set up single results (1 alias present) or multiple results (0 or more than 1 alias present) 
    // ----------------------------------------------------------------------------------------------------------------------------------------------------
   
    if (expression.rparms.values.size() == 1)                                                                          // just one alias to be assigned
    {
        results = to_results(expression.lparms.values.at(0));                                                          // output value = single alias
    }
    else                                                                                                               // 0 or more than 1 alias -- need to pass back multiple results
    {
        vlist_S vlist { };                                                                                             // clean results vlist -- no global:, value: keywords, etc. 
        vlist.values = expression.lparms.values;                                                                       // copy over just the right-side positional parms = all values to be assigned   
        results = to_results(vlist_val(vlist));                                                                        // convert vlist to results
        results.multiple_results = true;                                                                               // pass back vlist as multiple results
    }  

    return 0;                                                                                                          // return normally
}
M_endf






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @UNALIAS identifier identifier ... identifier :global static: local: verbmain: -- keywords optional
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_unalias(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional defined identifier parms     and    maybe right global: keyword

    M__(M_out(L"verb_unalias() called");)


    // fetch global: verbmain: static: keywords 

    value_S global_value { };                                                  // not looked at 
    auto global_rc   = get_right_keyword(expression, L"global", global_value);   // rc = -1, if global:    keyword is not present
    auto verbmain_rc = get_right_keyword(expression, L"verbmain"            );   // rc = -1, if verbmain:  keyword is not present
    auto static_rc   = get_right_keyword(expression, L"static"              );   // rc = -1, if static:    keyword is not present


    // undefine aliases in loop, if they are defined and non_constant 

    for (const auto& alias : expression.rparms.values)
    {
        M__(M_out(L"verb_unalias() -- range for -- alias.string = %s") % alias.string; )

        if (global_rc == 0)                                              // if global: keyword present, undefine global alias
        {
            if (is_global_identifier_defined(alias.string))             // don't bother, if global alias is already undefined 
            {
                if (!is_global_identifier_alias(alias.string))
                {
                    if (is_global_identifier_const(alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- global alias \"%S\" is constant -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)       
                    } 
                    else if (is_global_identifier_verbset(alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- global alias \"%S\" is a verb -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)        
                    } 
                    else if (is_global_identifier_typdef(alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- global alias \"%S\" is a typedef -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)         
                    } 
                    else if (!is_global_identifier_removable(alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- global alias \"%S\" cannot be removed -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    }
                    else
                    {
                        M_out_emsg1(L"@UNALIAS -- global identifier \"%S\" is not an alias -- unable to undefine using @UNALIAS") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    }
                }    
                else
                {
                    auto urc = undef_global_alias(alias.string);    // undefine alias from global environment
               
                    // errors are unexpected here
                   
                    if (urc != 0)
                    {
                        //count_error(); already counted in undef_global_alias()
                        M_out_emsg1(L"@UNALIAS -- unexpected error from undef_global_alias() -- unable to undefine global alias = %s") % alias.string;
                        M_verb_error0_loc(results, alias, expression)   
                    } 
                }
            }
            else
            {
                M_out_emsg1(L"@UNALIAS -- global identifier \"%S\" is not defined -- unable to undefine") % alias.string;
                M_verb_error1_loc(results, alias, expression)     
            }
        }
        else if (static_rc == 0)           // static: present -- undefine static alias
        {
            if (is_static_identifier_defined(frame, alias.string))             // don't bother, if static identifier is already undefined 
            {
                if (!is_static_identifier_alias(frame, alias.string))
                {
                    if (is_static_identifier_const(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- static alias \"%S\" is constant -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    } 
                    else if (is_static_identifier_verbset(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- static identifier \"%S\" is a verb -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)       
                    } 
                    else if (is_static_identifier_typdef(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- static identifier \"%S\" is a typdef -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)        
                    } 
                    else if (!is_static_identifier_removable(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- static identifier \"%S\" cannot be removed -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)        
                    }
                    else
                    {
                        M_out_emsg1(L"@UNALIAS -- static identifier \"%S\" is not an alias -- unable to undefine using @UNALIAS") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    }
                }
                else
                {
                    auto urc = undef_static_alias(frame, alias.string);           // undefine alias from static environment
                   
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (urc != 0)
                    {
                        //count_error(); already counted in def_local_alias()
                        M_out_emsg1(L"@UNALIAS -- unexpected error from undef_local_alias() -- unable to undefine static alias = %s") % alias.string;
                        M_verb_error0_loc(results, alias, expression)  
                    } 
                }
            }
            else
            {
                M_out_emsg1(L"@UNALIAS -- static identifier \"%S\" is not defined -- unable to undefine") % alias.string;
                M_verb_error1_loc(results, alias, expression)    
            }
        }       
        else if (verbmain_rc == 0)           // verbmain: present -- undefine verbmain alias
        {
            if (is_verbmain_identifier_defined(frame, alias.string))             // don't bother, if verbmain identifier is already undefined 
            {
                if (!is_verbmain_identifier_alias(frame, alias.string))
                {
                    if (is_verbmain_identifier_const(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- verbmain identifier \"%S\" is constant -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    } 
                    else if (is_verbmain_identifier_verbset(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- verbmain identifier \"%S\" is a verb -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    } 
                    else if (is_verbmain_identifier_typdef(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- verbmain identifier \"%S\" is a typdef -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    } 
                    else if (!is_verbmain_identifier_removable(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- verbmain identifier \"%S\" cannot be removed -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)           
                    } 
                    else
                    {
                        M_out_emsg1(L"@UNALIAS -- verbmain identifier \"%S\" is not an alias -- unable to undefine using @UNALIAS") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    }
                }
                else
                {
                    auto urc = undef_verbmain_alias(frame, alias.string);           // undefine alias from verbmain environment
                   
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (urc != 0)
                    {
                        //count_error(); already counted in undef_verbdef_alias()
                        M_out_emsg1(L"@UNALIAS -- unexpected error from undef_verbdef_alias() -- unable to undefine verbdef identifier = %s") % alias.string;
                        M_verb_error0_loc(results, alias, expression)    
                    } 
                }
            }
            else
            {
                M_out_emsg1(L"@UNALIAS -- verbmain identifier \"%S\" is not defined -- unable to undefine") % alias.string;
                M_verb_error1_loc(results, alias, expression)   
            }
        }  
        else          // global: verbdef: and static: were not present -- undefine local alias
        {
            if (is_local_identifier_defined(frame, alias.string))             // don't bother, if local identifier is already undefined 
            {
                if (!is_local_identifier_alias(frame, alias.string))
                {
                    if (is_local_identifier_const(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- local alias \"%S\" is constant -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)       
                    } 
                    else if (is_local_identifier_verbset(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- local identifier \"%S\" is a verb -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)         
                    } 
                    else if (is_local_identifier_typdef(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- local identifier \"%S\" is a typedef -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)         
                    } 
                    else if (!is_local_identifier_removable(frame, alias.string))
                    {
                        M_out_emsg1(L"@UNALIAS -- local identifier \"%S\" cannot be removed -- unable to undefine") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    }
                    else
                    {
                        M_out_emsg1(L"@UNALIAS -- local identifier \"%S\" is not an alias -- unable to undefine using @UNALIAS") % alias.string;
                        M_verb_error1_loc(results, alias, expression)          
                    }
                }
                else
                {
                    auto urc = undef_local_alias(frame, alias.string);           // undefine alias from local environment
                   
                    // errors expected here include duplicate alias names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (urc != 0)
                    {
                        //count_error(); already counted in def_local_alias()
                        M_out_emsg1(L"@UNALIAS -- unexpected error from undef_local_alias() -- unable to undefine local alias = %s") % alias.string;
                        M_verb_error0_loc(results, alias, expression)    
                    } 
                }
            }
            else
            {
                M_out_emsg1(L"@UNALIAS -- local identifier \"%S\" is not defined -- unable to undefine") % alias.string;
                M_verb_error1_loc(results, alias, expression)     
            }
        }        // global/local check
    }            // end of for() loop 


    // return normally, with no results

    results = no_results();                                        // output results = none 
    return 0; 
}
M_endf
 


#ifdef M_EXPOSE_SUPPORT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    expose_unexpose() -- helper function for @EXPOSE and @UNEXPOSE  
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int expose_unexpose(frame_S& frame, const e_expression_S& expression, bool is_exposed) try
{
    // already known that there is one or more right positional defined identifier parms  

    int rc { 0 };  

    // get setting for static: (vs local:) environment

    auto static_rc    = get_right_keyword(expression, L"static"              );   // rc = -1, if static:    keyword is not present
    auto verbmain_rc  = get_right_keyword(expression, L"verbmain"            );   // rc = -1, if verbmain:  keyword is not present


    // set/reset exposed flag for variables in loop -

    for (const auto& var : expression.rparms.values)
    {
        M__(M_out(L"expose_unexpose() -- range for -- var.string = %s") % var.string; )
         

        if (static_rc == 0)                   // static: is present -- expose/unexpose static identifiers
        {
            if (!is_static_identifier_defined(frame, var.string)) 
            {
                count_error();
                M_out_emsg1(L"expose_unexpose() -- unable to expose/unexpose undefined static identifier = %s") % var.string;
                msgend_loc(expression);
                rc = -1; 
            }
            else                              // static: identifier not defined 
            {
                auto rc = expose_static_identifier(frame, var.string, is_exposed);        // set/reset is_exposed flag in static identifier
                  
                if (rc != 0)
                {
                    //count_error(); already counted in expose_var()
                    M_out_emsg1(L"expose_unexpose() --  error from expose_static_var() -- unable to expose/unexpose static identifier = %s") % var.string;
                    msgend_loc(expression);
                    rc = -1;  
                } 
            }
        }
        else if (verbmain_rc == 0)                   // verbmain: is present -- expose/unexpose verbmain identifiers
        {
            if (!is_verbmain_identifier_defined(frame, var.string)) 
            {
                count_error();
                M_out_emsg1(L"expose_unexpose() -- unable to expose/unexpose undefined verbmain identifier = %s") % var.string;
                msgend_loc(expression);
                rc = -1; 
            }
            else                              // verbmain: identifier not defined 
            {
                auto rc = expose_verbmain_identifier(frame, var.string, is_exposed);        // set/reset is_exposed flag in verbmain identifier
                  
                if (rc != 0)
                {
                    //count_error(); already counted in expose_var()
                    M_out_emsg1(L"expose_unexpose() --  error from expose_verbmain_var() -- unable to expose/unexpose verbmain identifier = %s") % var.string;
                    msgend_loc(expression);
                    rc = -1;  
                } 
            }
        }
        else   // not static: or verbmain: -- must be local: (since global: identifiers can't be exposed or unexposed)
        {
            if (!is_local_identifier_defined(frame, var.string)) 
            {
                count_error();
                M_out_emsg1(L"expose_unexpose() -- unable to expose/unexpose undefined local identifier = %s") % var.string;
                msgend_loc(expression);
                rc = -1; 
            }
            else                              // static: not present -- expose/unexpose local identifiers 
            {
                auto rc = expose_local_identifier(frame, var.string, is_exposed);        // set/reset is_exposed flag in local identifier
               
                if (rc != 0)
                {
                    //count_error(); already counted in expose_var()
                    M_out_emsg1(L"expose_unexpose() -- error from expose_local_var() -- unable to expose/unexpose local identifier = %s") % var.string;
                    msgend_loc(expression);
                    rc = -1;  
                } 
            }
        }
    }    
  
    return rc;                      // return normally, or with error
}
M_endf
 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @EXPOSE identifier identifier ... identifier  
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_expose(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional defined identifier parms  
    M__(M_out(L"verb_expose() called");)
    
    auto rc = expose_unexpose(frame, expression, true);                      // expose all variables in right vlist
    
    // return normally, or with error

    if (rc == 0)  
        results = no_results();                                              // output results = none
    else
        results = error_results();                                           // output results = error

    return rc; 
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @UNEXPOSE identifier identifier ... identifier  
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_unexpose(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional defined identifier parms  
    M__(M_out(L"verb_unexpose() called");)
    
    auto rc = expose_unexpose(frame, expression, false);                     // unexpose all variables in right vlist
    
    // return normally, or with error

    if (rc == 0)  
        results = no_results();                                              // output results = none
    else
        results = error_results();                                           // output results = error

    return rc; 
}
M_endf
#endif



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @PASS identifier
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_pass(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one positional identifier parm -- optional share: noshare: keyword 
    M__(M_out(L"verb_pass() called");)

    results = to_results(expression.rparms.values.at(0));          // just copy 1st positional parm value to results 
    results.suppress_eval_once = true;                             // suppress evaluation once when next verb is called (even if verb calls for evaluation)
    return 0 ; 
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @NOEVAL identifier :share :unshare
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_noeval(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one positional identifier parm -- optional share: noshare: keyword 
    M__(M_out(L"verb_noeval() called");)


    // see if caller wants unshared copy

    if (0 == get_right_keyword(expression, L"unshare"))
    {
        value_S value {expression.rparms.values.at(0)};            // just copy 1st positional parm value to local var -- nested items are still shared
        unshare_value(value);
        results = to_results(value); 
    }
    else
    {
        results = to_results(expression.rparms.values.at(0));      // just copy 1st positional parm value to results
    }


  //  results.suppress_eval_once = true;                             // suppress evaluation on return from this verb
    return 0 ; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @EVAL -- return (already) evaluated input parms
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_eval(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are 1-N right-side positional parms of any type 

    M__(M_out(L"verb_eval() called");)

    results = to_results(vlist_val(expression.rparms));                                         // place input vlist in results (should have been evaluated before verb_eval() was called) 
    results.multiple_results = true;                                                            // indicate that mutiple results are perhaps being returned
    return 0;
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @UNSHARE value
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_unshare(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one positional assignable type parm
    M__(M_out(L"verb_unshare() called");)

    value_S value {expression.rparms.values.at(0)};      // just copy 1st positional parm value to local var -- nested items are still shared
    unshare_value(value);
    results = to_results(value); 
    return 0 ; 
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @GETENV "envar_name" -- get setting for envronment variable "envar_name" (if any -- unit results if not set)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_getenv(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be string 
    M__(M_out(L"verb_getenv() called");)
    

    if (is_env_set(expression.rparms.values.at(0).string))
    {       
        results = to_results(string_val(get_env(expression.rparms.values.at(0).string)));
        return 0; 
    }
    else
    {
        results = unit_results(); 
        return 0;
    }
    // should not get here
}
M_endf  




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Output-oriented verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
                                 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @STR   value value value ...  debug:    
//    @SAY   value value value ...  debug: no_nl:
//    @TEXT  block
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------
//   build_ws() -- helper function for verb_str() and verb_say()
//---------------------------------------------------------------------------------

static std::wstring build_ws(const vlist_S& vlist, bool debug_present, bool debugx_present) try
{
    // passed-in vlist may contain unwanted debug:/debugx: keywords that are not to be put in output string

    vlist_S nokey_vlist {vlist};                                                  // make copy of vlist with debug: and debugx: keywords
    nokey_vlist.eval_kws.clear();                                                 // clear out evaluated keyword map in copied-over vlist, so debug: and debugx: keywords are not formatted   
    return str_vlist(nokey_vlist, debug_present, debugx_present, true);           // format vlist (with debug:/debugx: keywords) + nesting    
}
M_endf




//---------------------------------------------------------------------------------
//   @TEXT -- verb_text()
//---------------------------------------------------------------------------------

int verb_text(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there is 1 right positional block parm   

    M__(M_out(L"verb_text() called");)
    M__(display_vlist(expression.rparms, L"verb_text() -- expression.rparms");)
        
    value_S value = M_get_right_pos(expression, 0);


    // create output string from right positional block parm -- known to be present

    text_control_S ctl { }; 

    results = to_results(string_val(text_block(*(value.block_sp), ctl)));   
    return 0; 
}
M_endf





//---------------------------------------------------------------------------------
//   @STR -- verb_str()
//---------------------------------------------------------------------------------

int verb_str(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there are 0 or more right positional parms that are printable  -- optional debug: and debugx: keywords   
    M__(M_out(L"verb_str() called");)
    M__(display_vlist(expression.rparms, L"verb_str() -- expression.rparms");)

    // get debug: and debugx: keywords (if present)  
                
    auto debug_rc  = get_right_keyword(expression, L"debug" );     // r/c = -1, if debug:  keyword is not present 
    auto debugx_rc = get_right_keyword(expression, L"debugx");     // r/c = -1, if debugx: keyword is not present 

    // create string from right positional vlist (without debug: and debugx: keywords)

    results = to_results(string_val(build_ws(expression.rparms, debug_rc == 0, debugx_rc == 0)));   
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @SAY -- verb_say() 
//---------------------------------------------------------------------------------

int verb_say(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there are 0 or more right positional parms that are printable  -- optional debug:  and debugx: keywords 
    M__(M_out(L"verb_say() called");)
    M__(display_vlist(expression.rparms, L"verb_say() -- expression.rparms");)
    std::wstring ws { }; 


    // get debug: and debugx: keywords (if present)  
                                
    auto debug_rc  = get_right_keyword(expression, L"debug" );     // r/c = -1, if debug:  keyword is not present 
    auto debugx_rc = get_right_keyword(expression, L"debugx");     // r/c = -1, if debugx: keyword is not present
    auto no_nl_rc  = get_right_keyword(expression, L"no_nl" );     // r/c = -1, if no_nl:  keyword is not present


    // write out formatted string from right vlist (without debug: and debugx: keywords) 

    ws = build_ws(expression.rparms, debug_rc == 0, debugx_rc == 0);               // format right vlist (without the debug: and debugx: keywords) in debug/non-debug mode  

    if (no_nl_rc == 0)
       write_stdout(ws);                                                           // put out formatted string without NL
    else
       write_stdout(ws + L"\n");                                                   // put out formatted string with NL

    results = to_results(string_val(ws));                                          // output value is the formatted string
    return 0;
}
M_endf



//---------------------------------------------------------------------------------
//   verb_stdout() -- @STDOUT (write string to stdout)  
//---------------------------------------------------------------------------------

int verb_stdout(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there is one right positional string parameter 
    M__(M_out(L"verb_stdout() called");)
       
    write_stdout(expression.rparms.values.at(0).string);                         // put out passed-in string   (no added nl)
    results = no_results();                                                      // no output value
    return 0;
}
M_endf



//---------------------------------------------------------------------------------
//   verb_stderr() -- @STDERR (write string to stderr)  
//---------------------------------------------------------------------------------

int verb_stderr(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there is one right positional string parameter 
    M__(M_out(L"verb_stderr() called");)
       
    write_stderr(expression.rparms.values.at(0).string);                         // put out passed-in string   (no added nl)
    results = no_results();                                                      // no output value
    return 0;
}
M_endf





//---------------------------------------------------------------------------------
//   verb_stdin() -- @STDIN (return one line read in from stdin)
//---------------------------------------------------------------------------------

int verb_stdin(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there are no parameters 
    M__(M_out(L"verb_stdin() called");)

    std::wstring ws{}; 
    auto rc = read_stdin(ws); 
            

    // if input error, return unit value

    if (rc != 0)
    {
        results = unit_results();                                              // output value = unit
        return 0;  
    }


    // no error -- return string read in from console
      
    results = to_results(string_val(ws));
    return 0;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////    @INTERPOLATE "string"     
////    @FORMAT      fmt:"string" value_1 value_2 value_3 value_4 ... value_n         
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// input parm structure for interpolate_string
// -------------------------------------------

struct i_parm_S
{
          frame_S         *frame_p            { nullptr };                                   // passed-in frame, in case called function needs to run some code 
    const e_expression_S  *expression_p       { nullptr };                                   // passed-in expression, for getting parms and for location messages, etc.
          std::wstring     ws_begin           {         };                                   // beginning marker for interpolated values
          std::wstring     ws_end             {         };                                   // ending    marker for interpolated values
          std::wstring     ws_message         {         };                                   // string to start error messages with
          bool             begin_any          { false   };                                   // delimited string section begins when any single char from begin string is found
          bool             end_any            { false   };                                   // delimited string section ends   when any single char from end   string is found
          bool             keep_begin         { false   };                                   // prepend delimited string with character/string that started it 
          bool             keep_end           { false   };                                   // append  delimited string with character/string that ended   it
};



//=========================================================================================================================
//
//   interpolate_string() -- internal helper function for string interpolation verbs
//
//=========================================================================================================================

static int interpolate_string( i_parm_S&             iparm                                    // miscellaneous parms
                             , const value_S&        verb_in_value                            // value containing main input format string 
                             , results_S&            results                                  // results for main interpolation-type verb 
                             , int                 (*fcn_p)( frame_S&                         // pass through current frame for invoking code, etc.
                                                           , const e_expression_S&            // pass through expression for getting parms and for location messages 
                                                           , results_S&                       // (error) results to be returned from main verb, if R/C is non-0 
                                                           , const std::wstring&              // input string to be processed 
                                                           , std::wstring&                    // output string to be appended to main output string from verb
                                                           , std::wstring&                    // acccumulated main output string from verb (may not be used)
                                                           , void *                           // pointer to parm passed through from main verb
                                                           )                                  // function to call to process interpolated sections of the string -- R/C = 0 continue, -1 = return from main verb immediately
                             , void *parm_p                                                   // pointer to parm area passed through to called function 
                             )
try
{
    // already known there is 1 right string parm + optional  begin:  and  end:  keyword parms + possible positional parms (not looked at here, but passed through to called function) 
    M__(M_out(L"interpolate_string() called");)
    
    std::wstring ws_verb_in  {verb_in_value.string};                   // main input string for outer verb  
    std::wstring ws_verb_out {  };                                     // main verb output gets accumulated here 


    // compute stepping amount, based on whether whole delimiter string or any single character in it is the delimiter
    // ---------------------------------------------------------------------------------------------------------------

    auto begin_step = iparm.ws_begin.size();                           // assumes whole string is delimiter  
    auto   end_step = iparm.ws_end  .size();                           // assumes whole string is delimiter

    if (iparm.begin_any)                                               // start delimiter is any single char in ws_begin ??
        begin_step = 1;                                                // if so, just step past one char in string after match

    if (iparm.end_any)                                                 // ending delimiter is any single char in ws_end ??
        end_step = 1;                                                  // if so, just step past one char in string after match


    // main loop to locate and process interpolation sections
    // ======================================================
        
    std::wstring::size_type  find_idx { 0 }; 
    std::wstring::size_type  out_idx  { 0 };
    std::wstring::size_type  idx1     { 0 };  
    std::wstring::size_type  idx2     { 0 };  

    for (;;)
    {
        // find start of next interpolated string section
        // ----------------------------------------------

        if (iparm.begin_any)                                                        // any single char in ws_begin is the starting delimiter? 
            idx1  = ws_verb_in.find_first_of(iparm.ws_begin, find_idx);             // if so, look for match with any char in start delimiter string
        else                                                                        // whole start delimiter string needs to match
            idx1  = ws_verb_in.find(iparm.ws_begin, find_idx);                      // look for whole-string match with start delimiter string

        if (idx1 == std::wstring::npos)                                             // normal exit when no starting delimiter found 
        {
            ws_verb_out += ws_verb_in.substr(out_idx, ws_verb_in.size() - out_idx); // copy over last part of input string
            break;
        }
    

        // append latest chunk of non-interpolated string to the output string 

        ws_verb_out += ws_verb_in.substr(out_idx, idx1 - out_idx); 
        M__(M_out(L"interpolate_string() -- ws_out   = «%S»") % ws_verb_out;)


        // search for ending delimiter to mark end of interpolated area 

        find_idx = idx1 + begin_step;                                               // start search right after the starting delimiter (atstart of interpolated section)
        
        if (iparm.end_any)                                                          // any single char in ws_begin is the ending delimiter?
            idx2 = ws_verb_in.find_first_of(iparm.ws_end, find_idx);                // if so, look for match with any char in end delimiter string
        else                                                                        // whole end delimiter string needs to match
            idx2 = ws_verb_in.find(iparm.ws_end, find_idx);                         // look for whole-string match with end delimiter string


        // error -- unbalanced interpolation delimiters if closing delimiter is not found 

        if (idx2 == std::wstring::npos)
        {
            M_out_emsg1(L"%S -- closing \"%S\" not found after opening \"%S\" at offset %d in string being processed") % iparm.ws_message % iparm.ws_end % iparm.ws_begin % idx1;
            M_verb_error1_loc(results, verb_in_value, *(iparm.expression_p))              
        } 


        M__(M_out(L"interpolate_string() -- idx1 = %d   idx2 = %d   find_idx = %d   out_idx = %d") % idx1 % idx2 % find_idx % out_idx;)
        M__(M_out(L"interpolate_string() -- ws_out   = «%S»") % ws_verb_out;)


        // isolate string to be processed  -- idx1 is offset of start delimiter (1st char)
        //                                    idx2 is offset of end delimiter   (1st char)

        auto idx1s  = idx1;                                                           // save unstepped index, in case start delimiter needs to be kept in ws_call_in
        idx1       += begin_step;                                                     // step past the starting delimiter
        auto idx2s  = idx2;                                                           // 2nd copy of ending index
   
        if (!iparm.keep_begin)                                                        // should we pass the starting delimiter through?
            idx1s = idx1;                                                             // if not, start passthrough at stepped index right after delimiter

        if (iparm.keep_end)                                                           // should we pass the ending delimiter through?
            idx2s = idx2 + end_step;                                                  // if so, end passthrough at stepped index right after delimiter
     
        std::wstring ws_call_in { ws_verb_in.substr(idx1s, idx2s - idx1s) };          // isolate string for parsing, etc. (with or without start/end delimiters)
       
        M__(M_out(L"interpolate_string() -- ws_inter = «%S»") % ws_call_in;)


        // call passed-in function to process this interpolated section
        // ============================================================

        std::wstring  ws_call_out  { }; 
        results_S     call_results { };

        auto crc = (*fcn_p)( *(iparm.frame_p), *(iparm.expression_p), call_results, ws_call_in, ws_call_out, ws_verb_out, parm_p); 


        // end loop and return imemdiately if called function returned with error
        
        if (crc != 0)
        {
            if (iparm.keep_begin || iparm.keep_end)
                M_out_emsg1(L"%S -- string interpolation ending because called function failed for interpolated section = \"%S\"") % iparm.ws_message % ws_call_in;
            else
                M_out_emsg1(L"%S -- string interpolation ending because called function failed for interpolated section = \"%S%S%S\"") % iparm.ws_message % iparm.ws_begin % ws_call_in % iparm.ws_end;

            msgend_loc(verb_in_value, *(iparm.expression_p));
            results = call_results;                                                     // verb results = called function results 
            return crc;                                                                 // verb R/C     = called function R/C
        }


        // end loop and return immediately, if special results (like @GOTO) returned called function
          
        M__(M_out(L"interpolate_string() -- called function returned OK -- check for special results");)
        if (call_results.special_results)
        {
            M__(M_out(L"interpolate_string() --  returning special results from called function");)  
            results = call_results;                                         // pass back special results
            return 0;
        }

        M__(M_out(L"interpolate_string() --  string from (*fcn_p)() call = \"%S\"") % call_out_ws;)  

        // no problems from called function -- just append results string to main output string and continue the interpolation loop

        ws_verb_out += ws_call_out;                                         // add outpout string from called function to main output string 
        M__(M_out(L"interpolate_string() --  in-loop ws_out = \"%S\"") % ws_out;)  


        // update index for find() for next loop pass 

        find_idx = idx2 + end_step;
        out_idx  = find_idx; 
    }

              
    // normal return with interpolated string
    // --------------------------------------


    M__(M_out(L"interpolate_string() --  final ws_out = \"%S\"") % ws_verb_out;)  
    results = to_results(string_val(ws_verb_out));  
    return 0;  
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////
//
//      verb_interpolate() -- @INTERPOLATE
//
/////////////////////////////////////////////////////////////////////////////////////


//=================================================================================================================================
//
// verb_interpolate_1() -- inner function passed in to interpolate_string() to do actual processing for one interpolated section
//
//=================================================================================================================================

static int verb_interpolate_1( frame_S&              frame
                             , const e_expression_S& expression
                             ,       results_S&      results
                             , const std::wstring&   ws_in
                             ,       std::wstring&   ws_out
                             ,       std::wstring&   verb_out
                             ,       void*           parm_p
                             )
try
{
    std::wstring ws_verb_out {  };                                                                                // main verb output gets accumulated here 


    // parse the passed-in interpolation section from the input string  

    block_S out_block{ };                                                                                         // block_S to be filled in by parse_string()  
    auto prc = parse_string(frame, out_block, ws_in, L"string interpolation ", false);                            // don't continue after error
 
    if (prc != 0)                                                                                                 // return with error, if parse_string() failed
    {
        M__(M_out(L"verb_interpolate() -- error rc from parse_string()");)       
        M_out_emsg1(L"@INTERPOLATE -- parse_string() failed for interpolated section \"%S\"") % ws_in;
        M_verb_error1_loc(results, expression.rparms.values.at(0), expression)    
    }
   

    // evaluate the block returned by parse_string

    results_S out_results {}; 
    
    M__(M_out(L"verb_interpolate_1() -- calling eval_block() ************************");)
    auto erc = eval_block(frame, out_block, out_results);                                                        // results (with any special flags) will be passed back directly (if no error)
    M__(M_out(L"verb_interpolate_1() -- eval_block() returned -- rc=%d *****************") % erc;)
    
  
     // return immediately, if error returned by eval_block()
     
     if (erc != 0)
        {
            M__(M_out(L"verb_interpolate_1() -- eval_block() returned error");)
            M__(M_out(L"verb_interpolate_1() -- returning error");)
            M_out_emsg1(L"@INTERPOLATE -- eval_block() failed for interpolated section \"%S\"") % ws_in;
            M_verb_error1_loc(results, expression.rparms.values.at(0), expression)  
        }
    

        // return immediately, if special results (like @GOTO) returned by eval_block()
          
        M__(M_out(L"verb_interpolate_1() -- eval_block() returned OK -- check for special results");)
        if (out_results.special_results)
        {
            M__(M_out(L"verb_interpolate_1() --  returning special results");)  
            results = out_results;    // pass back special results
            return 0;
        }

        // convert normal results (single, or multiple) to string

        if (out_results.multiple_results)
        {
            M__(display_results(out_results, L"multiple results flag on from eval_block()");)
            if (out_results.no_results)                   // vlist exists in this results_S
            {
                ws_out = std::wstring { };                // if not, put out empty string
            }
            else                                          // otherwise, need to format the (perhaps empty) vlist
            {
                M__(M_out(L"verb_interpolate_1() --  multiple results from eval_block()");)
                ws_out = str_vlist(*(out_results.vlist_sp), false, false, true);
            }
        }
        else                                              // single results -- results not in vlist
        {
            ws_out = str_value(out_results, false, false, true);
        }

        M__(M_out(L"verb_interpolate_1() -- output string = \"%S\"") % ws_out;)  
        return 0; 
}
M_endf


//=========================================================================================================================
//
// verb_interpolate() -- outer function for @INTERPOLATE verb
//
//=========================================================================================================================

int verb_interpolate(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there is 1 right string parm + optional  begin:  and  end:  keyword parms + possible positional parms (not looked at here, but passed through to called function) 
    M__(M_out(L"verb_interpolate() called");)


    // get strings from verb parms -- main string, and optional begin marker/end marker strings
 
    std::wstring ws_verb_in    { expression.rparms.values.at(0).string } ;

    std::wstring ws_verb_begin { L"{" };                               // default begin interpolation string
    std::wstring ws_verb_end   { L"}" };                               // default end   interpolation string
        
    get_right_keyword(expression, L"begin" , ws_verb_begin );          // override default begin interpolation marker string
    get_right_keyword(expression, L"end"   , ws_verb_end   );          // override default end   interpolation marker string   
                                                                 

    // just call string_interpolate() with specialized function to process each interpolated segment

    i_parm_S iparm { };

    iparm.frame_p      =      &frame;
    iparm.expression_p =      &expression;
    iparm.ws_begin     =      ws_verb_begin; 
    iparm.ws_end       =      ws_verb_end;
    iparm.ws_message   =      L"@INTERPOLATE string";
    iparm.begin_any    =      false;                                    // whole string is start delimiter
    iparm.end_any      =      false;                                    // whole string is end delimiter
    iparm.keep_begin   =      false;                                    // don't pass through start delimiter
    iparm.keep_end     =      false;                                    // don't pass through end    delimiter


    return interpolate_string( iparm
                             , expression.rparms.values.at(0)
                             , results
                             , &verb_interpolate_1
                             , nullptr
                             ); 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////
//
//   verb_format() -- @FORMAT
//
///////////////////////////////////////////////////////////////////////////////////

//=========================================================================================================================
//
// format_eval() -- internal helper function to evaluete passed-in block parms
//
//=========================================================================================================================
//
//  R/C = 0 , if normal results (single value)
//  R/C = 1 , if special results -- output results will have all the special results info 
//  R/C = -1, if error results -- output parm_results will be set up with results for @FORMAT verb to return 
//  R/C = -1, if normal results, but 0 or more than 1 value 

static 
int format_eval(frame_S& frame, const e_expression_S& expression, const value_S& parm_value, results_S& parm_results, const std::wstring& estr) try
{
    // if passed-in value is not for a block, just return that value as plain results

    if (parm_value.ty != type_E::block)
    {
        parm_results = to_results(parm_value);    
        return 0; 
    }


    // need to evaluate block passed in to @FORMAT verb
    // ------------------------------------------------

    results_S block_results { }; 

    auto erc = eval_block(frame, *(parm_value.block_sp), block_results);  


    // return, if evaluation error, or special results

    if (erc != 0)
    {
        M_out_emsg1(L"%S -- error evaluating passed-in block value") % estr;
        M_verb_error1_loc(parm_results, parm_value, expression)    
    }

    if (block_results.special_results)
    {   
        parm_results = block_results; 
        M__(M_out(L"format_eval() -- R/C = 1 -- special results");)
        return 1;                              // no error message, since special results may be wanted
    }
   

    // normal results, no error R/C -- return with error, if no results or error results
    // ---------------------------------------------------------------------------------

    if (block_results.error)
    {
        M_out_emsg1(L"%S -- evaluation of passed-in block value returned error results") % estr;
        M_verb_error1_loc(parm_results, parm_value, expression)   
    }

    if (block_results.no_results)
    {
        M_out_emsg1(L"%S -- evaluation of passed-in block value returned no (empty) results") % estr;
        M_verb_error1_loc(parm_results, parm_value, expression)  
    }


    // convert normal results to "plain" results, if possible -- complain if error occurs 

    auto src = to_single_results(block_results);          // if r/c = 0, block results has been converted to single-type results, if required

    if (src != 0)                                         // must be that passed-in block results had 0 values, multiple values, or keyword values     
    {
        M_out_emsg1(L"%S -- results from evaluation of passed-in block contained 0 values, contained more than 1 value, or contained keyword values") % estr;
        M_verb_error1_loc(parm_results, parm_value, expression)   
    }
 

    // return normally, if block evaluation results cound be converted to single results (or were already single results) 
 
    parm_results = block_results;                        // block results are OK as-is
    return 0; 
}
M_endf



//=========================================================================================================================
//
// verb_format_1() -- inner function passed in to interpolate_string() to do actual processing for one interpolated section
//
//=========================================================================================================================

static int verb_format_1(frame_S& frame, const e_expression_S& expression, results_S& results, const std::wstring& ws_in, std::wstring& ws_out, std::wstring& verb_out, void* in_p) try
{
    M__(M_out(L"verb_format_1(): called -- ws_in = \"%S\"") % ws_in;)


    int *parm_index_p = (int *)in_p;                                                                          // get typed pointer to persistent parm index
                                                                                                             
    int parm_count = expression.rparms.values.size();                                                         // get number of right side positional parms passed to the outer @FORMAT verb                                                               
                                                                                                              
    value_S fmt_value { };                                                                                   
    get_right_keyword(expression, L"fmt", fmt_value);                                                         // get input value containing main format string -- for location messsage 
                                                                                                             
                                                                                                             
    // preliminary format string processing
    // ------------------------------------
 
    // replace leading % that was removed by interpolation delimiting routine, and append ";" to format spec for error messages

    std::wstring ws_orig_fmt { ws_in                         };                                                  // copy over input format spec -- non-modifiable/original copy -- for error messages 
    std::wstring ws_fmt      { ws_in                         };                                                  // modifiable format spec string -- for passing to swprintf, etc., after potential modifications 
    auto         fmt_size    { ws_orig_fmt.size()            };                                                  // capture original format spec length for convenience
      

    M__(M_out(L"verb_format_1(): called  --  ws_in = \"%S\"  --  ws_fmt = \"%S\"   --  ws_orig_fmt = \"%S\"") % ws_in % ws_fmt % ws_orig_fmt;) 


    // =====================================================================================================================
    // process non-standard format specifications  -- %%   %n  %N etc.  -- don't pass these through the regex scanners 
    // =====================================================================================================================
    

    // ----------------------------------------------------------------
    // just put out "%", if format spec is "%%" -- no modifiers allowed
    // ----------------------------------------------------------------

    if (ws_fmt == L"%%")                                                                                        
    {   
        ws_out = L"%";                                      // just put out % -- similar to printf() %%
        return 0;                                           // don't need to advance the positional parm                                                    
    }                                                                                                        
   
    // ---------------------------------------------------------
    // special handling for  "%n" and %N -- no modifiers allowed
    // ---------------------------------------------------------

    if ( (ws_fmt == L"%n") || (ws_fmt == L"%N") )                                                                                        
    {   
        // make sure needed positional parm exists

        if (*parm_index_p >= parm_count) 
        {
            M_out_emsg1(L"@FORMAT -- output variable for format spec \"%S\" refers to non-existent parm -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
            M_verb_error1_loc(results, fmt_value, expression)              
        }


        // make sure n-th positional parm is an (unevaluated) identifier

        auto parm_val  { expression.rparms.values.at(*parm_index_p) } ;

        if (parm_val.ty != type_E::identifier)
        {
            M_out_emsg1(L"@FORMAT -- output parameter for format spec \"%S\" at parm %d is not an unevaluated identifier (variable name) -- value type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_val.ty);
            M_verb_error1_loc(results, fmt_value, expression)         
        }


        // process the %N or %n spec
        // =========================

        if (ws_fmt == L"%n")
        { 
            // process %n format spec -- assign number of wchar_t items in formatted string (so far) to caller's variable 

            auto urc = update_var( frame, parm_val.string, int64_val(verb_out.length()) );
          
            if (urc != 0)
            {
                M_out_emsg1(L"@FORMAT -- could not set output variable \"%S\" for format spec \"%S\" using parm number %d to output string width = %d") % parm_val.string % ws_orig_fmt % *parm_index_p % verb_out.length();
                M_verb_error1_loc(results, fmt_value, expression)         
            }
        }
        else                                                                                               // must be %N
        {
            // process %N format spec -- assign formatted string (so far) to caller's variable

            auto urc = update_var(frame, parm_val.string, string_val(verb_out));
          
            if (urc != 0)
            {
                M_out_emsg1(L"@FORMAT -- could not set output variable \"%S\" for format spec \"%S\" using parm number %d to output string data = \"%S\"") % parm_val.string % ws_orig_fmt % *parm_index_p % shorten_str(verb_out, 50);
                M_verb_error1_loc(results, fmt_value, expression)           
            }
        }


        // advance parm index by 1 and return normally 
        // -------------------------------------------

        (*parm_index_p)++;                                  // advance to next positional parm for outer verb 
        return 0; 
    }     
   

    // ==============================================================================================================
    // extract variable length and precision ( examples: %*.10X    %10.*X   %0#-*.*X ), if present in the format spec  
    // ==============================================================================================================

    int v_1         { -1  };                                                                                                   // -1 indicates that 1st variable is not being used 
    int v_2         { -1  };                                                                                                   // -1 indicates that 2nd variable is not being used 
    int v_count     {  0  };                                                                                                   // number of variable parms in format spec 
                    
    bool w_valid    {false};                                                                                                   // true indicates width is present
    bool p_valid    {false};                                                                                                   // true indicates precision is present 
    
    int width       {  -1 };                                                                                                   // width     (variable or fixed)
    int precision   {  -1 };                                                                                                   // precision (variable or fixed)
                                                                                                                              
    std::wregex  rpatv { LR"(\%([0\-\+\ \#]*)(\*|[[:digit:]]+)?(\.\*|\.[[:digit:]]+)?[aAceEfFgGdiostuxX])" };                  // general pattern for a A c e E f F g G d i o s t u x X format specification -- final verification done later
    std::wsmatch mv    { };                                                                                                    // match results for std::wstring
                                                                                                                              
    auto foundv { std::regex_match(ws_fmt, mv, rpatv) };                                                                       // see if passed-in format specification matches most general valid type


    // if format spec is not valid here, don't bother looking for *.* -- one of the more specific tests down below will fail (and the error will be reported then)

    if (foundv)
    {
        // format spec looks fairly good -- proceed with getting width/precision

        M__(M_out(    L"verb_format_1() -- mv.size()             = %d") % mv.size()                   ;) 
      
        for (auto i = 0; i < mv.size(); i++)
        {
            M__(M_out(L"verb_format_1() -- mv.length(%d)      = %d")     % i % mv.length(i)           ;)
            M__(M_out(L"verb_format_1() -- mv.position(%d)    = %d")     % i % mv.position(i)         ;)
            M__(M_out(L"verb_format_1() -- mv.str(%d)         = \"%S\"") % i % mv.str(i)              ;)   
        }


        // ---------------------------
        // process width, if specified
        // ---------------------------

        if (mv.size() >= 3)
        {
            if (mv.str(2) == L"*")                                                                // check for variable width
            {
                // make sure needed positional parm exists
               
                if (*parm_index_p >= parm_count) 
                {
                    M_out_emsg1(L"@FORMAT -- variable width ('*') in format spec \"%S\" refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                    M_verb_error1_loc(results, fmt_value, expression)            
                }
               
               
                // evaluate any passed-in block parm, which neeeds to evaluate to an integer type 
               
                auto      parm_value    { expression.rparms.values.at(*parm_index_p) } ;         // capture current @FORMAT parm
                results_S parm_results  {  };                                                    // empty results to be filled in by format_eval()
               
                std::wstring estr { L"@FORMAT -- variable width ('*') in format spec \"" + ws_orig_fmt + L"\" refers to positional parm " +  fmt_str(L"%d", *parm_index_p) };
                auto erc = format_eval(frame, expression, parm_value, parm_results, estr);
                
                if (erc != 0)                                                                    // just pass back any error or special results to caller
                {
                    M__(M_out(L"verb_format_1() -- erc = %d") % erc;)
                    results = parm_results;                                                      // format_eval() has already prepared appropriate return results for @FORMAT verb
                    return std::min(0, erc);                                                     // convert R/C = 1 to 0, -1 stays at -1  -- format_eval() has put out any require error messages 
                }
               
               
                // make sure needed positional parm is of integer type  -- don't check values - let swprintf() do it's checking
               
                if (!is_value_integer(parm_results))
                {
                    M_out_emsg1(L"@FORMAT -- variable width ('*') in format spec \"%S\" refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_results.ty);
                    M_verb_error1_loc(results, fmt_value, expression)          
                }
                
               
                int  v_width   { -1  } ;
                auto gv_rc = get_val(parm_results, v_width);                                      // extract value_S integer type data into C++ int variable, for passing to swprintf() later
               
                if (gv_rc != 0)
                {
                    M_out_emsg1(L"@FORMAT -- variable width ('*') in format spec \"%S\" refers to positional parm %d = %S -- cannot be converted to 31-bit signed integer value") % ws_orig_fmt % *parm_index_p % str_value(parm_results);
                    M_verb_error1_loc(results, fmt_value, expression)               
                }
               

                // set width field, and indicate that width field is valid 

                width   = v_width;
                w_valid = true; 

               
                //  valid integer value -- save away for later swprintf() call 
               
                v_count++;                                                                         // indicate we have at least one variable parm 
                v_1 = v_width;                                                                     // variable width is always 1st  
                (*parm_index_p)++;                                                                 // advance to next positional parm for outer verb 
            }                                                                                      // variable width field present
            else if (mv.str(2) != L"")                                                             // should be fixed width -- if empty string, precision is not present
            {
               // try to convert fixed width string to int32 -- complain if this fails

               auto trc = to_int32(mv.str(2), width);                                              // try to convert string to int32_t                 
            
               if (trc != 0)
               {
                    M_out_emsg1(L"@FORMAT -- fixed width (\"%S\") in format spec \"%S\" cannot be converted to 31-bit signed integer value") % mv.str(2) % ws_orig_fmt;
                    M_verb_error1_loc(results, fmt_value, expression)  
               }

               w_valid = true;                                                                     // integer is OK -- indicate that precision is valid            
            }                                                                                      // fixed width field present
        }                                                                                          // width field present in this format spec


        // ----------------------------------------
        // process variable precision, if specified
        // ----------------------------------------

        if (mv.size() >= 4)
        {
            if (mv.str(3) == L".*")                                                                // check for variable precision
            {
                // make sure needed positional parm exists
           
                if (*parm_index_p >= parm_count) 
                {
                    M_out_emsg1(L"@FORMAT -- variable precision ('.*') in format spec \"%S\" refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                    M_verb_error1_loc(results, fmt_value, expression)            
                }
           
           
                // evaluate any passed-in block parm, which neeeds to evaluate to an integer type 
           
                auto      parm_value    { expression.rparms.values.at(*parm_index_p) } ;         // capture current @FORMAT parm
                results_S parm_results  {  };                                                    // empty results to be filled in by format_eval()
           
                std::wstring estr { L"@FORMAT -- variable precision ('*') in format spec \"" + ws_orig_fmt + L"\" refers to positional parm " +  fmt_str(L"%d", *parm_index_p) };
                auto erc = format_eval(frame, expression, parm_value, parm_results, estr);
                
                if (erc != 0)                                                                    // just pass back any error or special results to caller
                {
                    results = parm_results;                                                      // format_eval() has already prepared appropriate retuen results for @FORMAT verb
                    return erc;                                                                  // format_eval() has put out any require error messages 
                }
                             
           
                // make sure needed positional parm is of integer type  -- don't check values - let swprintf() do it's checking
           
                if (!is_value_integer(parm_results))
                {
                    M_out_emsg1(L"@FORMAT -- variable precision ('*') in format spec \"%S\" refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_results.ty);
                    M_verb_error1_loc(results, fmt_value, expression)            
                }
                
                int  v_precision { -1 } ;
                auto gv_rc = get_val(parm_results, v_precision);                                   // extract value_S integer type data into C++ int variable, for passing to swprintf() later
           
                if (gv_rc != 0)
                {
                    M_out_emsg1(L"@FORMAT -- variable precision ('*') in format spec \"%S\" refers to positional parm %d = %S -- cannot be converted to 31-bit signed integer value") % ws_orig_fmt % *parm_index_p % str_value(parm_results);
                    M_verb_error1_loc(results, fmt_value, expression)             
                }
           

                // set precision field, and indicate that precision field is valid 

                precision  = v_precision;
                p_valid    = true; 

           
                //  valid integer value -- save away for later swprintf() call 
           
                v_count++;                                                                         // indicate we have another variable parm 
                                                                                          
                if (v_count == 1)                                                         
                    v_1 = v_precision;                                                             // no variable width -- variable precision is 1st 
                else                                                                              
                    v_2 = v_precision;                                                             // had variable width -- variable precision is 2nd
                                                                                                  
                (*parm_index_p)++;                                                                 // advance to next positional parm for outer verb 
            }                                                                                      // variable precision specified
            else if (mv.str(3) != L"")                                                             // should be fixed precision -- empty string means no precision 
            {
               // try to convert fixed precision string to int32 -- complain if this fails

               auto trc = to_int32(mv.str(3).substr(1), precision);                                // try to convert string to int32_t                 
            
               if (trc != 0)
               {
                    M_out_emsg1(L"@FORMAT -- fixed precision (\"%S\") in format spec \"%S\" cannot be converted to 31-bit signed integer value") % mv.str(3).substr(1) % ws_orig_fmt;
                    M_verb_error1_loc(results, fmt_value, expression)  
               }

               p_valid = true;                                                                     // integer is OK -- indicate that precision is valid            
            }                                                                                      // fixed precision field
        }                                                                                          // precision field present 
    }                                                                                              // format spec looks reasonably correct 

    M__(M_out( L"verb_format_1() -- v_count = %d    v_1 = %d      v_2 = %d   *parm_index_p = %d ") % v_count % v_1 % v_2 % *parm_index_p;) 


    // ------------------------------------------------------------------------------------------------------------------------------------
    //
    //  process this format spec: 
    //
    // decode final spec character, verify if rest of spec is OK, then call str_fmt() with the format spec and n-th positional parm value 
    // (only a subset of printf() functions are supported here)
    //
    // note that operand length specifications should not be present (like %lld) -- they will be supplied before wsprintf() is called 
    // printf() parm numbers (like %1$d) are not supported. %% is not supported.   ' (grouping characters) are not supported
    //
    // -------------------------------------------------------------------------------------------------------------------------------------

    auto last_char { ws_fmt.back() };                                                                   // get last chr in format specification  
    switch ( last_char )
    {
        // process non-simple %n or %N format specification -- invalid -- only "%n" or "%N" is allowed and has been taken care of earlier
        // ------------------------------------------------------------------------------------------------------------------------------

        case    L'n' :
        {
            M_out_emsg1(L"@FORMAT -- apparent %%n-type format spec \"%S\" is not valid") % ws_orig_fmt;
            M_verb_error1_loc(results, fmt_value, expression) 
        }


        case    L'N' :
        {
            M_out_emsg1(L"@FORMAT -- apparent %%N-type format spec \"%S\" is not valid") % ws_orig_fmt;
            M_verb_error1_loc(results, fmt_value, expression) 
        }
        

        // process %t -- tab to position format specification (similar to %t in boost::format)
        // -----------------------------------------------------------------------------------
        //
        //   %t      -- does nothing (spaces over to current length)
        //   %99t    -- spaces over to position 99 or current length
        //   %.20t   -- adds 20 spaces (spaces over to current length, making sure to add at least 20 spaces)
        //   %99.20t -- spaces over to position 99 or current length, making sure to add at least 20 spaces
        //   %*t     -- same as above, but uses n-th positional parm to get width and/or precision
        //   %.*t       ''     note: negative precision is invalid
        //   %*.*t      ''     note: negative precision is invalid
        //                     note: negative width says chop off current string to absolute value of width, if string is longer already  

        case    L't' :
        {                       
            std::wregex  rpat { LR"(\%(\*|[[:digit:]]+)?(\.\*|\.[[:digit:]]+)?[t])" };                  // more restrictive pattern for %t format specification
            std::wsmatch mr   { };                                                                      // match results for std::wstring

            auto found { std::regex_match(ws_fmt, mr, rpat) };                                          // see if passed-in format sp[ecification matches
           
            if (!found) 
            {
                M_out_emsg1(L"@FORMAT -- Apparent %%t-type format spec \"%S\" is not valid") % ws_orig_fmt;
                M_verb_error1_loc(results, fmt_value, expression) 
            }  


            // set up working length, width, and precision values
            // --------------------------------------------------

            bool   truncate { false             };                                                       // default is no truncation 
            int    w        { 0                 };                                                       // default width = 0
            size_t wide     { 0                 };                                                       // default width = 0
            int    p        { 0                 };                                                       // default precision = 0
            size_t prec     { 0                 };                                                       // default precision = 0
            size_t len      { verb_out.length() };                                                       // current length of output string     


            // set up working width and truncate flag (negative width means truncate/pad to length = -w, if required -- positive width means pad to length = w, if required -- never truncate)

            if (w_valid) 
                w = width; 

            if (w < 0)
            {
                w        = -w;
                truncate = true; 
            }

            wide = w; 


            // set up working precision -- negative precision is invalid
            
            if (p_valid) 
                p = precision;

            if (p < 0)
            {
                M_out_emsg1(L"@FORMAT -- Precision value in %t-type format spec \"%S\" is %d -- negative values are not valid") % ws_orig_fmt % p;
                M_verb_error1_loc(results, fmt_value, expression)   
            }

            prec = p;


            // truncate existing output string, if required -- refresh length value

            if ( truncate && (len > wide) )
            {
                verb_out.erase(wide);
                len = verb_out.length();            
            }
          

            // compute number of characters need to pad existing string to desired length

            size_t pad {0};

            if (wide > len)
                pad = wide - len;                          // need this much padding to reach desired width

            pad = std::max(pad, prec);                     // also make sure to pad by at least prec characters


            // output string is just a string of 'pad' blanks to be added to end of string

            ws_out = std::wstring { };                     // start out with empty uotput string

            if (pad > 0)
               ws_out.append(pad, L' ');                   // output string = 'pad' blanks

            return 0; 
            break;
        }
                                                                                          

        // process string type format specification
        // ----------------------------------------  
       
        case    L's' : 
        {
            std::wregex  rpat { LR"(\%([0\-]*)(\*|[[:digit:]]+)?(\.\*|\.[[:digit:]]+)?[s])" };          // pattern for s format specification
            std::wsmatch mr   { };                                                                      // match results for std::wstring

            auto found { std::regex_match(ws_fmt, mr, rpat) };                                          // see if passed-in format sp[ecification matches
           
            if (!found) 
            {
                M_out_emsg1(L"@FORMAT -- string-type format spec \"%S\" is not valid") % ws_orig_fmt;
                M_verb_error1_loc(results, fmt_value, expression) 
            }  

          
            // check the matching positional input parm to see if it matches the spec  

            if (*parm_index_p >= parm_count) 
            {
                M_out_emsg1(L"@FORMAT -- string-type format spec \"%S\" refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                M_verb_error1_loc(results, fmt_value, expression)            
            } 


            // make sure n-th positional parm is type_E::string

            auto parm_type { expression.rparms.values.at(*parm_index_p).ty } ;

            if (parm_type != type_E::string)
            {
                M_out_emsg1(L"@FORMAT -- string-type format spec \"%S\" refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_type);
                M_verb_error1_loc(results, fmt_value, expression)         
            }  


            // pass format spec string and input string to fmt_str() for formatting with this spec

            if (v_count == 0)        ws_out = fmt_str(ws_fmt,           expression.rparms.values.at(*parm_index_p).string);
            if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      expression.rparms.values.at(*parm_index_p).string);
            if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, expression.rparms.values.at(*parm_index_p).string);

            (*parm_index_p)++;                                                                             // advance to next positional parm for next time 
            return 0;
            break; 
        }


        // process integer type format specification (including character type)
        // -----------------------------------------

        case    L'c' :
        case    L'd' : 
        case    L'i' : 
        case    L'o' : 
        case    L'u' :
        case    L'x' :
        case    L'X' :
        {
            // do basic validity check in integer format specificion

            bool found { false }; 


            if ( (last_char == L'x') || (last_char == L'X') )
            {
                std::wregex  rpat { LR"(\%([0\-\+\ \#]*)(\*|[[:digit:]]+)?(\.\*|\.[[:digit:]]+)?[xX])" };   // pattern for x, X format specification
                std::wsmatch mr   { };                                                                      // match results for std::wstring

                found = std::regex_match(ws_fmt, mr, rpat);                                                 // see if passed-in format specification matches the required pattern for x or X specs
            
            
            }
            else
            {
                std::wregex  rpat { LR"(\%([0\-\+\ ]*)(\*|[[:digit:]]+)?(\.\*|\.[[:digit:]]+)?[cdiou])" };  // pattern for c, d, i, o, u format specification   (note # not supported here for o -- would need special code to prepend 0o, not just 0)
                std::wsmatch mr   { };                                                                      // match results for std::wstring

                found = std::regex_match(ws_fmt, mr, rpat);                                                 // see if passed-in format specification matches the required pattern for c d i o u specs 
            }

            if (!found) 
            {
                M_out_emsg1(L"@FORMAT -- integer-type format spec \"%S\" is not valid") % ws_orig_fmt;
                M_verb_error1_loc(results, fmt_value, expression)   
            }    
                

            // check the matching positional input parm to see if it matches the spec 
            // ----------------------------------------------------------------------

            // make sure n-th positional parm is present

            if (*parm_index_p >= parm_count) 
            {
                M_out_emsg1(L"@FORMAT -- integer-type format spec \"%S\" refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                M_verb_error1_loc(results, fmt_value, expression)           
            }

         
            // make sure n-th positional parm is valid integer type
            
            value_S parm_value { expression.rparms.values.at(*parm_index_p) } ;                                // get n-th positional value into local var 
            auto    parm_type  { parm_value.ty                              } ;   

            if  (
                  is_value_integer(parm_value)
                  ||
                  ( is_value_float(parm_value) && ((last_char == L'x' ) || (last_char == L'X'))  )             // allow floating point input value with X or x format specification only
                )
            {
                // value/type is OK -- no error at this point
            }
            else
            {
                M_out_emsg1(L"@FORMAT -- integer-type  format spec \"%S\" refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_type);
                M_verb_error1_loc(results, fmt_value, expression)            
            }  
             

            // special processing for wchar_t to be passed in to swprintf() as an int for %c type formatting
            // ---------------------------------------------------------------------------------------------

            if (last_char == L'c')
            {
                int wc_val { };                                                                                      // local integer variable to be passed to swprintf() for %c formatting 
                                                                                                               
                auto gv_rc = get_val(parm_value, wc_val);                                                            // convert n-th positional parm value to C++ int type, if possible -- complain if error

                if (gv_rc != 0)
                {
                    M_out_emsg1(L"@FORMAT -- character-type format spec \"%S\" refers to positional parm %d = %S -- cannot be converted to 31-bit signed integer value") % ws_orig_fmt % *parm_index_p % str_value(parm_value);
                    M_verb_error1_loc(results, fmt_value, expression)           
                } 


                // make sure this is a valid UTF-16 code point 
                //
                //   - high or low surrogate allowed -- caller needs to use two adjacent %c specs to format an extended character -- there is no policing of this here 
                //   - also allow plane-0 non-characters u+FFFE, u+FFFF, etc.
            
                if ( (wc_val < 0) || (wc_val > const_N::utf16_plane0_max) )
                {
                    count_error(); 
                    M_out_emsg1(L"@FORMAT -- character-type format spec \"%S\" refers to positional parm %d = %d -- UTF-16 code point outside of valid range (u+0000 to u+FFFF)") % ws_orig_fmt % *parm_index_p % wc_val;
                    M_verb_error1_loc(results, fmt_value, expression)           
                } 


                // call swprintf() for %c-type formatting 

                if (v_count == 0)        ws_out = fmt_str(ws_fmt,           wc_val);
                if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      wc_val);
                if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, wc_val);           
            }
            else                                                                                                // all formatting other than %c
            { 
                // for int64_t and uint64_t (or float64_ being formatted with X) , change final  d  i  o u  x  or  X  to  lld  lli  llu llx or llX
            
                if  ( (parm_type == type_E::int64) || (parm_type == type_E::uint64) || (parm_type == type_E::float64) ) 
                {
                    ws_fmt.pop_back();                                                                          // remove final character from format spec (still available in last_char)
                    ws_fmt.append(std::wstring { L"ll" });                                                      // append "ll" 
                    ws_fmt.push_back(last_char);                                                                // put back the final char, after the "ll"
                }
            
            
                // call fmt_str() to format the n-th positional parm using call to swprintf() 
            
                switch (parm_type) 
                {
                    case type_E::int8    : 
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.int8   );
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.int8   );
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.int8   );
                        break; 
            
                    case type_E::int16   :  
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.int16  );
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.int16  );
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.int16  );
                        break; 
            
                    case type_E::int32   :  
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.int32  );
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.int32  );
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.int32  );
                        break; 

                    case type_E::int64   :  
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.int64  );
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.int64  );
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.int64  );
                        break; 
                
                    case type_E::uint8   :  
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.uint8  );
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.uint8  );
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.uint8  );
                        break; 
                
                    case type_E::uint16  :  
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.uint16 );
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.uint16 );
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.uint16 );
                        break; 
                
                    case type_E::uint32  :  
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.uint32 );
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.uint32 );
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.uint32 );
                        break; 
                
                    case type_E::uint64  :  
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.uint64 );
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.uint64 );
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.uint64 );
                        break; 
                
                    case type_E::float32 :                                          // format floating point number as unsigned integer (X or x type only)
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.float32);
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.float32);
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.float32);
                        break;      
               
                    case type_E::float64 :                                          // format floating point number as unsigned integer (X or x type only)
                        if (v_count == 0)        ws_out = fmt_str(ws_fmt,           parm_value.float64);
                        if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      parm_value.float64);
                        if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, parm_value.float64);
                        break;         
                
                    default: 
                    {
                        count_error(); 
                        M_out_emsg1(L"@FORMAT -- internal error -- unknown integer parm type = \"%S\"") % type_str(parm_type);
                        msgend_loc(fmt_value, expression);                                                                                                              
                        M_throw_v(   "verb_format_1() -- internal error -- unknown integer parm type = \"%S\"") % out_ws(type_str(parm_type)) )); 
                        break;                                                      // should not get here
                    }
                
                }
            }


            // control gets here after valid fomatting spec is processed 

            (*parm_index_p)++;                                                  // advance to next positional parm for next time 
            return 0;
            break;
        }
  

        // process floating-point type format specification
        // ------------------------------------------------ 

        case    L'a' : 
        case    L'A' :
        case    L'e' : 
        case    L'E' : 
        case    L'f' :
        case    L'F' :
        case    L'g' :
        case    L'G' :
        {
            std::wregex  rpat { LR"(\%([0\-\+\ \#]*)(\*|[[:digit:]]+)?(\.\*|\.[[:digit:]]+)?[aAeEfFgG])" };   // pattern for a A e E f F g G format specification
            std::wsmatch mr   { };                                                                            // match results for std::wstring

            auto found { std::regex_match(ws_fmt, mr, rpat) };                                                // see if passed-in format sp[ecification matches

            if (!found) 
            {
                M_out_emsg1(L"@FORMAT -- floating point-type format spec \"%S\" is not valid") % ws_orig_fmt;
                M_verb_error1_loc(results, fmt_value, expression)  
            } 

          
            // check the matching positional input parm to see if it matches the spec  

            if (*parm_index_p >= parm_count) 
            {
                M_out_emsg1(L"@FORMAT -- floating-point-type format spec \"%S\" refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                M_verb_error1_loc(results, fmt_value, expression)            
            }


            // make sure n-th positional parm is valid floating-point type
            
            auto parm_type { expression.rparms.values.at(*parm_index_p).ty } ;

            if  (
                  (parm_type != type_E::float32 )
                  &&
                  (parm_type != type_E::float64 )
                )
            {
                M_out_emsg1(L"@FORMAT -- floating-point-type  format spec \"%S\" refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_type);
                M_verb_error1_loc(results, fmt_value, expression)           
            } 


            // pass format spec string and input floating point value to fmt_str() for formatting with this spec

            if (parm_type == type_E::float32)
            {
               if (v_count == 0)        ws_out = fmt_str(ws_fmt,           expression.rparms.values.at(*parm_index_p).float32);
               if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      expression.rparms.values.at(*parm_index_p).float32);
               if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, expression.rparms.values.at(*parm_index_p).float32);
            }
            else
            {
               if (v_count == 0)        ws_out = fmt_str(ws_fmt,           expression.rparms.values.at(*parm_index_p).float64);
               if (v_count == 1)        ws_out = fmt_str(ws_fmt, v_1,      expression.rparms.values.at(*parm_index_p).float64);
               if (v_count == 2)        ws_out = fmt_str(ws_fmt, v_1, v_2, expression.rparms.values.at(*parm_index_p).float64);
            }

            (*parm_index_p)++;                                                                                        // advance to next positional parm for next time 
            return 0;                                                                                               
            break; 
         }


        // all other types are not supported -- 'p'  'z'  for example
        // ----------------------------------------------------------

        default : 
        {
            M_out_emsg1(L"@FORMAT -- final character ('%c') in format spec \"%S\" is unsupported -- must be:  a,  A,  d,  e,  E,  f,  F,  g,  G,  i,  n,  o,  s,  u,  x,  or  X") % ws_orig_fmt.back() % ws_fmt;
            M_verb_error1_loc(results, fmt_value, expression) 
            break;
        }
    }
 

    // should not get here

    return 0; 
}
M_endf




//=========================================================================================================================
//
//  verb_format() -- outer function for @FORMAT verb
//
//=========================================================================================================================

int verb_format(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there is required   fmt:   kw parm, and optional   begin:  and   end:  keyword parms + likely positional parms (not looked at here, but passed through to called function) 
    M__(M_out(L"verb_format() called");)


    // get strings from verb parms -- main string, and optional begin marker/end marker strings

    value_S      verb_in_value {      };                               // get full value_S for location messages
    std::wstring ws_verb_in    {      };                               // main format string for verb
   
    get_right_keyword(expression, L"fmt"   , verb_in_value );          // main format string, as a value -- with location for error messages
    get_right_keyword(expression, L"fmt"   , ws_verb_in    );          // main format string
 
                                                                 
  
    // just call string_interpolate() with specialized function to process each interpolated segment

    int  parm_index { 0 };                                             // work value for verb_format_1() to save current parameter number to process next time
 
    i_parm_S iparm { };

    iparm.frame_p      =      &frame;
    iparm.expression_p =      &expression;
    iparm.ws_begin     =      L"%";                                     // format spec starts with %
    iparm.ws_end       =      L"%aAcdeEfFgGinNostuxX";                  // format spec ends with any one of these characters
    iparm.ws_message   =      L"@FORMAT string";
    iparm.begin_any    =      false;                                    // whole string is start delimiter
    iparm.end_any      =      true;                                     // any single char in ws_end is the end delimiter for the format spec
    iparm.keep_begin   =      true;                                     // pass through start delimiter (%)
    iparm.keep_end     =      true;                                     // pass through end delimiter (d x u, F, etc.)


    return interpolate_string( iparm
                             , verb_in_value
                             , results
                             , &verb_format_1
                             , (void *)&parm_index
                             ); 
   }
M_endf   



/*************************************************************************************************************************************
*
*   test variations for get_val()
*   ----------------------------- 
*
*   int8_t    i8  { }; 
*   int16_t   i16 { }; 
*   int32_t   i32 { }; 
*   int64_t   i64 { }; 
*   uint8_t   u8  { }; 
*   uint16_t  u16 { }; 
*   uint32_t  u32 { }; 
*   uint64_t  u64 { }; 
*
*
*   int rc {1};  
*   value_S { }; 
*
*   rc = get_val( type_val(( int64_t)111          ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % 111     %  i8  % rc ;
*   rc = get_val( type_val(( int64_t)222          ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % 222     %  i8  % rc ;
*   rc = get_val( type_val(( int64_t)-111         ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % -111    %  i8  % rc ;
*   rc = get_val( type_val(( int64_t)-222         ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % -222    %  i8  % rc ;
*   rc = get_val( type_val(( int64_t)33333        ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % 33333   %  i8  % rc ;
*   rc = get_val( type_val(( int64_t)-33333       ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % -33333  %  i8  % rc ;
*   rc = get_val( type_val(( int64_t)77777        ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % 77777   %  i8  % rc ;
*
*   rc = get_val( type_val(( int64_t)111          ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % 111     %  u8  % rc ;
*   rc = get_val( type_val(( int64_t)222          ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % 222     %  u8  % rc ;
*   rc = get_val( type_val(( int64_t)-111         ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % -111    %  u8  % rc ;
*   rc = get_val( type_val(( int64_t)-222         ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % -222    %  u8  % rc ;
*   rc = get_val( type_val(( int64_t)33333        ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % 33333   %  u8  % rc ;
*   rc = get_val( type_val(( int64_t)-33333       ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % -33333  %  u8  % rc ;
*   rc = get_val( type_val(( int64_t)77777        ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % 77777   %  u8  % rc ;
*   
*   rc = get_val( type_val(( int64_t)111          ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % 111     %  i16  % rc ;
*   rc = get_val( type_val(( int64_t)222          ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % 222     %  i16  % rc ;
*   rc = get_val( type_val(( int64_t)-111         ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % -111    %  i16  % rc ;
*   rc = get_val( type_val(( int64_t)-222         ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % -222    %  i16  % rc ;
*   rc = get_val( type_val(( int64_t)33333        ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % 33333   %  i16  % rc ;
*   rc = get_val( type_val(( int64_t)-33333       ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % -33333  %  i16  % rc ;
*   rc = get_val( type_val(( int64_t)77777        ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % 77777   %  i16  % rc ;
*
*   rc = get_val( type_val(( int64_t)111          ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % 111     %  u16  % rc ;
*   rc = get_val( type_val(( int64_t)222          ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % 222     %  u16  % rc ;
*   rc = get_val( type_val(( int64_t)-111         ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % -111    %  u16  % rc ;
*   rc = get_val( type_val(( int64_t)-222         ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % -222    %  u16  % rc ;
*   rc = get_val( type_val(( int64_t)33333        ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % 33333   %  u16  % rc ;
*   rc = get_val( type_val(( int64_t)-33333       ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % -33333  %  u16  % rc ;
*   rc = get_val( type_val(( int64_t)77777        ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % 77777   %  u16  % rc ;
*
*
*   add_separators() -- test variations
*
*   add_separators_w(-123456789.789);
*   add_separators_w( 123456789.789);
*   add_separators_w( 123456789.);
*   add_separators_w(-123456789.);
*   add_separators_w( 123456789.E+66);
*   add_separators_w(-123456789.E+66);
*
*   add_separators_w(L"-1"                  ,  L',' , 3);
*   add_separators_w(L"-12"                 ,  L',' , 3);
*   add_separators_w(L"-123"                ,  L',' , 3);
*   add_separators_w(L"-1234"               ,  L',' , 3);
*   add_separators_w(L"-12345"              ,  L',' , 3);
*   add_separators_w(L"-123456"             ,  L',' , 3);
*   add_separators_w(L"-1234567"            ,  L',' , 3);
*   add_separators_w(L"-12345678"           ,  L',' , 3);
*   add_separators_w(L"-123456789"          ,  L',' , 3);
*   add_separators_w(L"-1234567890"         ,  L',' , 3);
*   add_separators_w(L"1"                   ,  L',' , 3);
*   add_separators_w(L"12"                  ,  L',' , 3);
*   add_separators_w(L"123"                 ,  L',' , 3);
*   add_separators_w(L"1234"                ,  L',' , 3);
*   add_separators_w(L"12345"               ,  L',' , 3);
*   add_separators_w(L"123456"              ,  L',' , 3);
*   add_separators_w(L"1234567"             ,  L',' , 3);
*   add_separators_w(L"12345678"            ,  L',' , 3);
*   add_separators_w(L"123456789"           ,  L',' , 3);
*   add_separators_w(L"1234567890"          ,  L',' , 3);
*
*
*   add_separators_w(L" 123456789");
*   add_separators_w(L"-123456789");
*   add_separators_w(L"+123456789");
*   add_separators_w( L"123456789");
*
*   add_separators_w(L"-123456789.789");
*   add_separators_w(L"+123456789.789");
*   add_separators_w(L" 123456789.789");
*   add_separators_w( L"123456789.789");
*
*   add_separators_w(L" 123456789.");
*   add_separators_w(L"-123456789.");
*   add_separators_w(L"+123456789.");
*   add_separators_w( L"123456789.");
*
*   add_separators_w(L" .123456789");
*   add_separators_w(L"-.123456789");
*   add_separators_w(L"+.123456789");
*   add_separators_w( L".123456789");
*
*
*   add_separators_w(L" 123456789E+66");
*   add_separators_w(L"-123456789e+66");
*   add_separators_w(L"+123456789E+66");
*   add_separators_w( L"123456789e+66");
*
*   add_separators_w(L" 123456789.E+66");
*   add_separators_w(L"-123456789.e+66");
*   add_separators_w(L"+123456789.E+66");
*   add_separators_w( L"123456789.e+66");
*
*   add_separators_w(L" .123456789E+66");
*   add_separators_w(L"-.123456789e+66");
*   add_separators_w(L"+.123456789E+66");
*   add_separators_w( L".123456789e+66");
*
***********************************************************************************************************************/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @DISPLAY 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_display(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_display() called");)


    // get keywords (if present)  

    value_S token_list_value { }; 

    auto locale_rc           = get_right_keyword(expression, L"locale"                                    );     // r/c = 0, if locale:             keyword is present 
    auto numerics_rc         = get_right_keyword(expression, L"numerics"                                  );     // r/c = 0, if numerics:           keyword is present
    auto stack_rc            = get_right_keyword(expression, L"stack"                                     );     // r/c = 0, if stack:              keyword is present
    auto all_vars_rc         = get_right_keyword(expression, L"all_vars"                                  );     // r/c = 0, if all_vars:           keyword is present
    auto builtin_verbs_rc    = get_right_keyword(expression, L"builtin_verbs"                             );     // r/c = 0, if builtin_verbs:      keyword is present
    auto added_verbs_rc      = get_right_keyword(expression, L"added_verbs"                               );     // r/c = 0, if added_verbs:        keyword is present
    auto all_verbs_rc        = get_right_keyword(expression, L"all_verbs"                                 );     // r/c = 0, if all_verbs:          keyword is present
    auto builtin_types_rc    = get_right_keyword(expression, L"builtin_types"                             );     // r/c = 0, if builtin_types:      keyword is present
    auto added_types_rc      = get_right_keyword(expression, L"added_types"                               );     // r/c = 0, if added_types:        keyword is present
    auto all_types_rc        = get_right_keyword(expression, L"all_types"                                 );     // r/c = 0, if all_types:          keyword is present
    auto id_cache_rc         = get_right_keyword(expression, L"id_cache"                                  );     // r/c = 0, if id_cache:           keyword is present
    auto statistics_rc       = get_right_keyword(expression, L"statistics"                                );     // r/c = 0, if statistics:         keyword is present
    auto token_list_rc       = get_right_keyword(expression, L"token_list"    , token_list_value          );     // r/c = 0, if token_list:         keyword is present
                                                                                       
    auto vars_rc             = get_right_keyword(expression, L"vars"                                      );     // r/c = 0, if vars:[ ... ]        keyword is present
    auto verbs_rc            = get_right_keyword(expression, L"verbs"                                     );     // r/c = 0, if verbs:[ ... ]       keyword is present
    auto types_rc            = get_right_keyword(expression, L"types"                                     );     // r/c = 0, if types:[ ... ]       keyword is present
                                                                                       
  
    M_out(L"\n------------------------------------------------------------------------------------------------------------------------------------------");

    if (locale_rc == 0)
    {
         M_out(L" ");
         display_locale();
    }

    if (numerics_rc == 0) 
    {
         M_out(L" ");
         display_numerics();
    }

    if (stack_rc == 0) 
    {
         M_out(L" ");
         display_stack(frame);
    } 

    if (all_vars_rc == 0) 
    {
         M_out(L" ");
         display_all_vars(frame);
    } 

    if (added_verbs_rc == 0) 
    {
         M_out(L" ");
         display_all_verbsets(frame, false, true);
    } 

    if (builtin_verbs_rc == 0) 
    {
         M_out(L" ");
         display_all_verbsets(frame, true, false);
    } 

    if (all_verbs_rc == 0) 
    {
         M_out(L" ");
         display_all_verbsets(frame, true, true);
    }  

    if (added_types_rc == 0) 
    {
         M_out(L" ");
         display_all_typdefs(frame, false, true);
    } 

    if (builtin_types_rc == 0) 
    {
         M_out(L" ");
         display_all_typdefs(frame, true, false);
    } 

    if (all_types_rc == 0) 
    {
         M_out(L" ");
         display_all_typdefs(frame, true, true);
    }   

    if (id_cache_rc == 0) 
    {
         M_out(L" ");
         display_id_cache();
    }  

    if (statistics_rc == 0) 
    {
         M_out(L" ");
         display_statistics();
    }  
      
    if (token_list_rc == 0) 
    {
         M_out(L" ");
         display_token_list(token_list_value.int64);
    } 




    // vars:[ ... ]   verbs:[ ...]    types:[     ]  options -- nested list with 1 or more strings is present with identifiers/verbnames to be displayed 
    // -------------------------------------------------------------------------------------------------------------------------------------------------

    if (vars_rc == 0)
    {
        M_get_right_keyword_vlist(expression, L"vars", vars_vlist) 
        M_out(L" ");
        display_vars(frame, vars_vlist);                            // display selected vars     
    }

    if (verbs_rc == 0)
    {
        M_get_right_keyword_vlist(expression, L"verbs", verbs_vlist) 
        M_out(L" ");
        display_verbsets(frame, verbs_vlist);                       // display selected verbs definitiona     
    }

    if (types_rc == 0)
    {
        M_get_right_keyword_vlist(expression, L"types", types_vlist) 
        M_out(L" ");
        display_typdefs(frame, types_vlist);                        // display selected type definitions     
    } 

    M_out(L"\n------------------------------------------------------------------------------------------------------------------------------------------\n");

    results = no_results();                                         // output value = none
    return 0; 
}
M_endf  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @DEBUG -- set debug display settings 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_debug(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_debug() called");)


    //  handle log_verbs-oriented keywords
    //  ----------------------------------

    auto no_verbs_rc           = get_right_keyword(expression, L"no_verbs"    );     // r/c = 0, if no_verbs:          keyword is present 
    auto few_verbs_rc          = get_right_keyword(expression, L"few_verbs"   );     // r/c = 0, if few_verbs:         keyword is present 
    auto some_verbs_rc         = get_right_keyword(expression, L"some_verbs"  );     // r/c = 0, if some_verbs:        keyword is present 
    auto most_verbs_rc         = get_right_keyword(expression, L"most_verbs"  );     // r/c = 0, if most_verbs:        keyword is present 
    auto all_verbs_rc          = get_right_keyword(expression, L"all_verbs"   );     // r/c = 0, if all_verbs:         keyword is present
                                            
    if (no_verbs_rc            == 0)      log_verbs(log_E::none ); 
    if (few_verbs_rc           == 0)      log_verbs(log_E::few  ); 
    if (some_verbs_rc          == 0)      log_verbs(log_E::some ); 
    if (most_verbs_rc          == 0)      log_verbs(log_E::most ); 
    if (all_verbs_rc           == 0)      log_verbs(log_E::all  ); 
     

    M__(M_out(L"verb_debug() -- log_verbs = %d )") % (int)log_verbs();)

    results = no_results();                                                     // output value = none
    return 0; 
}
M_endf  




//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""