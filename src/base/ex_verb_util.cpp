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
////     @EXPOSE
////     @UNEXPOSE
////
////     @AGG
////     @TYPE
////
////     @TO_xxxx
////     @IS_xxxx
////
////     #
////
////     @GETENV
////     @ARG_CT
////     @ARGS
////     @ARG
////     @AT 
////     @VL_CT
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
        results = error_results();     // pass back error results 
        return -1; 
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
        results = error_results();     // pass back error results  
        return -1; 
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
//    @VAR identifier identifier ... identifier     value: general-value  :global :expose share: unshare:    -- keywords optional
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_var(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional undefined identifier parms     and    maybe right value: and global: keywords

    int rc { 0 };  

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
                count_error();
                M_out_emsg1(L"verb_var() -- global identifier %s is already defined -- unable to define it again") % var.string;
                msgend_loc(var, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S  parm { };
                parm.unshare = (unshare_rc == 0); 

                auto rc = def_global_var(var.string, value_value, parm);                       // add new non-constant variable to environment -- global variables are always exposed
               
                // errors are unexpected here
               
                if (rc != 0)
                {
                    //count_error(); already counted in def_global_var()
                    M_out_emsg1(L"verb_var() -- error from def_global_var() -- unable to define new global variable = %s") % var.string;
                    msgend_loc(var, expression);
                    rc = -1; 
                }  
            }
        }
        else if (static_rc == 0)          // static: present -- define static variable
        {
            if (is_static_identifier_defined(frame, var.string))
            {
                count_error();
                M_out_emsg1(L"verb_var() -- static identifier %s is already defined -- unable to define it again") % var.string;
                msgend_loc(var, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S  parm { };
                parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif


                auto rc = def_static_var(frame, var.string, value_value, parm);                       // add new non-constant variable to environment  -- expose based on expose: kw rc
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_static_var()
                    M_out_emsg1(L"verb_var() -- error from def_static_var() -- unable to define new static variable = %s") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;  
                }  
            }
        }
        else if (verbmain_rc == 0)          // verbmain: present -- define verbmain variable
        {
            if (is_verbmain_identifier_defined(frame, var.string))
            {
                count_error();
                M_out_emsg1(L"verb_var() -- verbmain identifier %s is already defined -- unable to define it again") % var.string;
                msgend_loc(var, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S  parm { };
                parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif


                auto rc = def_verbmain_var(frame, var.string, value_value, parm);                       // add new non-constant variable to environment  -- expose based on expose: kw rc
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_verbmain_var()
                    M_out_emsg1(L"verb_var() -- error from def_verbmain_var() -- unable to define new verbmain variable = %s") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;  
                }  
            }
        }
        else          // global:, verbmain:, and static: were not present -- define local: variable
        {
            if (is_local_identifier_defined(frame, var.string))
            {
                count_error();
                M_out_emsg1(L"verb_var() -- local identifier %s is already defined -- unable to define it again") % var.string;
                msgend_loc(var, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S  parm { };
                parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
                parm.exposed  = (expose_rc  == 0);
#endif


                auto rc = def_local_var(frame, var.string, value_value, parm);                       // add new non-constant variable to environment  -- expose based on expose: kw rc
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_local_var()
                    M_out_emsg1(L"verb_var() -- error from def_local_var() -- unable to define new local variable = %s") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;  
                }  
            }
        }
    }    


    // return normally, or with error

    if (rc == 0)  
    {
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
    }
    else
        results = error_results();                                     // output results = error

    return rc; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @CONST   identifier   value: general-value   global: expose: unshare: share: -- value: keyword is required
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_const(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right positional undefined identifier parm     and    required right value: keyword  -- one optional global: keyword

    int rc { 0 }; 

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
            count_error();
            M_out_emsg1(L"verb_const() -- global identifier %s is already defined -- unable to define it again") % var_name;
            msgend_loc(expression.rparms.values.at(0), expression);
            rc = -1;        
        }
        else
        { 
            def_parm_S parm { }; 
            parm.unshare  = (unshare_rc == 0);
                          
            auto rc = def_global_const(var_name, value_value, parm);              // add new constant variable to environment -- global constant always exposed
           
            // errors expected here include duplicate global variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
           
            if (rc != 0)
            {
                //count_error(); already counted in def_global_const()
                M_out_emsg1(L"verb_const() -- error from def_global_const() -- unable to define new global constant = %s") % var_name;
                msgend_loc(expression.rparms.values.at(0), expression);
                rc = -1; 
            } 
        }
    }
    else if (static_rc == 0)          // static: present -- define static constant
    {
        if (is_static_identifier_defined(frame, var_name))
        {
            count_error();
            M_out_emsg1(L"verb_const() -- static identifier %s is already defined -- unable to define it again") % var_name;
            msgend_loc(expression.rparms.values.at(0), expression);
            rc = -1;        
        }
        else
        { 
            def_parm_S parm { }; 
            parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
            parm.exposed  = (expose_rc  == 0);
#endif

            auto rc = def_static_const(frame, var_name, value_value, parm);                           // add new non-constant variable to environment -- expose based on expose: kw rc
          
            // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
          
            if (rc != 0)
            {
                //count_error(); already counted in def_static_const()
                M_out_emsg1(L"verb_const() -- error from def_static_const() -- unable to define new static constant = %s") % var_name;
                msgend_loc(expression.rparms.values.at(0), expression);
                rc = -1;  
            } 
        }
    }
    else if (verbmain_rc == 0)          // verbmain: present -- define verbmain constant
    {
        if (is_verbmain_identifier_defined(frame, var_name))
        {
            count_error();
            M_out_emsg1(L"verb_const() -- verbmain identifier %s is already defined -- unable to define it again") % var_name;
            msgend_loc(expression.rparms.values.at(0), expression);
            rc = -1;        
        }
        else
        { 
            def_parm_S parm { }; 
            parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
            parm.exposed  = (expose_rc  == 0);
#endif
            auto rc = def_verbmain_const(frame, var_name, value_value, parm);                         // add new non-constant variable to environment -- expose based on expose: kw rc
          
            // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
          
            if (rc != 0)
            {
                //count_error(); already counted in def_verbmain_const()
                M_out_emsg1(L"verb_const() -- error from def_verbmain_const() -- unable to define new verbmain constant = %s") % var_name;
                msgend_loc(expression.rparms.values.at(0), expression);
                rc = -1;  
            } 
        }
    }
    else          // global: and static: were not present -- define local constant
    {
        if (is_local_identifier_defined(frame, var_name))
        {
            count_error();
            M_out_emsg1(L"verb_const() -- local identifier %s is already defined -- unable to define it again") % var_name;
            msgend_loc(expression.rparms.values.at(0), expression);
            rc = -1;        
        }
        else
        { 
            def_parm_S parm { }; 
            parm.unshare  = (unshare_rc == 0);
#ifdef M_EXPOSE_SUPPORT
            parm.exposed  = (expose_rc  == 0);
#endif

            auto rc = def_local_const(frame, var_name, value_value, parm);                           // add new non-constant variable to environment -- expose based on expose: kw rc
          
            // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
          
            if (rc != 0)
            {
                //count_error(); already counted in def_local_const()
                M_out_emsg1(L"verb_const() -- error from def_local_constr() -- unable to define new local constant = %s") % var_name;
                msgend_loc(expression.rparms.values.at(0), expression);
                rc = -1;  
            } 
        }
    }


    // return with/without error

    if (rc == 0)
        results = to_results(value_value);               // output results = value assigned to new constant
    else
        results = error_results();                       // output results = error          

    return rc; 
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @UNVAR identifier identifier ... identifier :global static: local: verbmain: -- keywords optional
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_unvar(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one or more right positional undefined identifier parms     and    maybe right global: keyword

    int rc { 0 };  

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
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- global variable %s is constant -- unable to undefine") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else if (is_global_identifier_verb(var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- global identifier %s is a verb -- unable to undefine using @UNVAR") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else if (is_global_identifier_typdef(var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- global identifier %s is a typedef -- unable to undefine using @UNVAR") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else
                {
                    auto rc = undef_global_var(var.string);    // undefine variable from global environment
               
                    // errors are unexpected here
                   
                    if (rc != 0)
                    {
                        //count_error(); already counted in def_global_var()
                        M_out_emsg1(L"verb_unvar() -- unexpected error from undef_global_var() -- unable to undefine global variable = %s") % var.string;
                        msgend_loc(var, expression);
                        rc = -1; 
                    } 
                }
            }
        }
        else if (static_rc == 0)           // static: present -- undefine static variable
        {
            if (is_static_identifier_defined(frame, var.string))             // don't bother, if static identifier is already undefined 
            {
                if (is_static_identifier_const(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- static variable %s is constant -- unable to undefine") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else if (is_static_identifier_verb(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- static identifier %s is a verb -- unable to undefine using @UNVAR") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else if (is_static_identifier_typdef(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- static identifier %s is a typdef -- unable to undefine using @UNVAR") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else
                {
                    auto rc = undef_static_var(frame, var.string);           // undefine variable from static environment
                   
                    // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (rc != 0)
                    {
                        //count_error(); already counted in def_local_var()
                        M_out_emsg1(L"verb_unvar() -- unexpected error from undef_local_var() -- unable to undefine static variable = %s") % var.string;
                        msgend_loc(var, expression);
                        rc = -1;  
                    } 
                }
            }
        }       
        else if (verbmain_rc == 0)           // verbmain: present -- undefine verbmain variable
        {
            if (is_verbmain_identifier_defined(frame, var.string))             // don't bother, if verbmain identifier is already undefined 
            {
                if (is_verbmain_identifier_const(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- verbmain identifier %s is constant -- unable to undefine") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else if (is_verbmain_identifier_verb(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- verbmain identifier %s is a verb -- unable to undefine using @UNVAR") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else if (is_verbmain_identifier_typdef(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- verbmain identifier %s is a typdef -- unable to undefine using @UNVAR") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else
                {
                    auto rc = undef_verbmain_var(frame, var.string);           // undefine variable from verbmain environment
                   
                    // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (rc != 0)
                    {
                        //count_error(); already counted in undef_verbdef_var()
                        M_out_emsg1(L"verb_unvar() -- unexpected error from undef_verbdef_var() -- unable to undefine verbdef identifier = %s") % var.string;
                        msgend_loc(var, expression);
                        rc = -1;  
                    } 
                }
            }
        }  
        else          // global: verbdef: and static: were not present -- undefine local variable
        {
            if (is_local_identifier_defined(frame, var.string))             // don't bother, if local identifier is already undefined 
            {
                if (is_local_identifier_const(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- local variable %s is constant -- unable to undefine") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else if (is_local_identifier_verb(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- local identifier %s is a verb -- unable to undefine using @UNVAR") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else if (is_local_identifier_typdef(frame, var.string))
                {
                    count_error(); 
                    M_out_emsg1(L"verb_unvar() -- local identifier %s is a typedef -- unable to undefine using @UNVAR") % var.string;
                    msgend_loc(var, expression);
                    rc = -1;        
                } 
                else
                {
                    auto rc = undef_local_var(frame, var.string);           // undefine variable from local environment
                   
                    // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
                   
                    if (rc != 0)
                    {
                        //count_error(); already counted in def_local_var()
                        M_out_emsg1(L"verb_unvar() -- unexpected error from undef_local_var() -- unable to undefine local variable = %s") % var.string;
                        msgend_loc(var, expression);
                        rc = -1;  
                    } 
                }
            }
        }        // global/local check
    }            // end of for() loop 


    // return normally, or with error

    if (rc == 0)  
        results = no_results();                                        // output results = none
    else
        results = error_results();                                     // output results = error

    return rc; 
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


    results.suppress_eval_once = true;                             // suppress evaluation on return from this verb
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

//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Aggregate-oriented and Type-oriented verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//---------------------------------------------------------------------------------
//   @AGG 
//---------------------------------------------------------------------------------

int verb_agg(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // see which parms are present -- at least one must be present: len: or type: (or both)
    // -----------------------------------------------------------

    value_S len_value  { };
    value_S type_value { };

    auto len_rc   = get_right_keyword(expression, L"len"  , len_value  );       // len_rc   = -1, if len:   is not present (meaning use length of type: for length of buffer 
    auto type_rc  = get_right_keyword(expression, L"type" , type_value );       // type_rc  = -1, if type:  is not present (meaning, make array of UINT8_T with len: elements


    // error, if neither len: nor type: keywords are present
    // -----------------------------------------------------

    if ( (len_rc == -1) && (type_rc == -1) )
    {
       count_error();
       M_out_emsg1(L"verb_agg() -- neither len: or type: keywords were present -- at least one is required -- aggregate cannot be constructed");
       msgend_loc(expression);

       results = results_S { };
       return -1; 
    }


    // error, if both len: and type: keywords are present, and length of typdef_S exceeds specified len:
    // -------------------------------------------------------------------------------------------------

    if (
        ( (len_rc == 0) && (type_rc == 0) )
        &&
        ( type_value.typdef_sp->tsize > len_value.int64)
       ) 
    {
       count_error();
       M_out_emsg1(L"verb_agg() -- length of type: (%d) exceeds specified len: (%d) -- aggregate cannot be constructed") % type_value.typdef_sp->tsize % len_value.int64;
       msgend_loc(expression);

       results = results_S { };
       return -1; 
    }


    // error, if typdef_S for type: keyword has zero length (i.e. is not fixed-length typdef_S)
    // ----------------------------------------------------------------------------------------

    if ( (type_rc == 0) && (type_value.typdef_sp->tsize <= 0) )
    {
       count_error();
       M_out_emsg1(L"verb_agg() -- length of type: is 0 (non fixed-length type) -- aggregate cannot be constructed");
       msgend_loc(expression);

       results = results_S { };
       return -1; 
    }  


    // get length of buf8_T
  
    size_t len { };

    if (len_rc == 0)
       len = len_value.int64; 
    else
       len = type_value.typdef_sp->tsize;


    // get typdef for this aggregate

    typdef_S typdef { };

    if (type_rc == 0)
    {
        typdef = *(type_value.typdef_sp);                                   // use caller's typdef, if provided
        unshare_typdef(typdef);                                             // make sure nothing shared,since std::move() will be used below
    }
    else    
    {
        make_array_typdef(len, make_atomic_typdef(type_E::uint8), typdef);  // use array of uint8, with len: elements
    }


    // construct aggregate results from typdef_S and buffer, and return normally

    buf8_T buffer { len }; 
    buffer.clear(); 

    results = to_results(  buffer_val(buffer, typdef, true)  ); 

    return 0; 
}
M_endf
         

//---------------------------------------------------------------------------------
//   @TYPE 
//---------------------------------------------------------------------------------

int verb_type(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    int rc {0}; 


     // process input parms
     // -------------------

     // known: optional typedef name on right side -- is unevaluated identifier
    
     value_S name_value  { };                                                      // name_value.string has name of undefined identifier for this typdef

     auto name_rc      = get_right_keyword(expression, L"name",  name_value );       // name_rc      = -1 if name:       is not present (meaning anonymous (returned) type 
     auto global_rc    = get_right_keyword(expression, L"global"            );       // global_rc    = -1, if global:    is not present 
     auto verbmain_rc  = get_right_keyword(expression, L"verbmain"          );       // verbmain rc  = -1, if verbmain:  is not present
     auto static_rc    = get_right_keyword(expression, L"static"            );       // static rc    = -1, if static:    is not present
#ifdef M_EXPOSE_SUPPORT
     auto expose_rc    = get_right_keyword(expression, L"expose"            );       // expose_rc  = -1, if expose:  is not present (meaning no expose)
#endif


     // known: no more than one of the following keywords is present:

     value_S array_value     { };            // value should be a vlist with two positional parms
     value_S struct_value    { };            // value should be a vlist with 1-N nested vlist values 

     auto     unit_rc = get_right_keyword(expression, L"unit"                      );  
     auto     bool_rc = get_right_keyword(expression, L"bool"                      );  
     auto     int8_rc = get_right_keyword(expression, L"int8"                      );    
     auto    uint8_rc = get_right_keyword(expression, L"uint8"                     );  
     auto    int16_rc = get_right_keyword(expression, L"int16"                     );    
     auto   uint16_rc = get_right_keyword(expression, L"uint16"                    ); 
     auto    int32_rc = get_right_keyword(expression, L"int32"                     );    
     auto   uint32_rc = get_right_keyword(expression, L"uint32"                    ); 
     auto    int64_rc = get_right_keyword(expression, L"int64"                     );    
     auto   uint64_rc = get_right_keyword(expression, L"uint64"                    ); 
     auto  float32_rc = get_right_keyword(expression, L"float32"                   );    
     auto  float64_rc = get_right_keyword(expression, L"float64"                   ); 
     auto    array_rc = get_right_keyword(expression, L"array"    ,     array_value);
     auto   struct_rc = get_right_keyword(expression, L"struct"   ,    struct_value);


     // set up typdef_S based on input parms
     // ------------------------------------

     typdef_S typdef { };              // typdef to be filled in 


     // (note: only one of these should have r/c = 0, if none are present, default typdef will be produced)

     if (    unit_rc == 0 )   make_atomic_typdef(type_E::unit    , typdef);
     if (    bool_rc == 0 )   make_atomic_typdef(type_E::boolean , typdef);
     if (    int8_rc == 0 )   make_atomic_typdef(type_E::int8    , typdef);
     if (   uint8_rc == 0 )   make_atomic_typdef(type_E::uint8   , typdef);
     if (   int16_rc == 0 )   make_atomic_typdef(type_E::int16   , typdef);
     if (  uint16_rc == 0 )   make_atomic_typdef(type_E::uint16  , typdef);
     if (   int32_rc == 0 )   make_atomic_typdef(type_E::int32   , typdef);
     if (  uint32_rc == 0 )   make_atomic_typdef(type_E::uint32  , typdef);
     if (   int64_rc == 0 )   make_atomic_typdef(type_E::int64   , typdef);
     if (  uint64_rc == 0 )   make_atomic_typdef(type_E::uint64  , typdef);
     if ( float32_rc == 0 )   make_atomic_typdef(type_E::float32 , typdef);
     if ( float64_rc == 0 )   make_atomic_typdef(type_E::float64 , typdef);



     // process array type
     // ------------------

     if (array_rc == 0)
     {
         uint64_t acount {0};  


         // array_value should be a vlist with 2 positional values -- int64 number of elements, and typdef_S for each element

         if (
             (array_value.ty != type_E::vlist)
             ||
             (array_value.vlist_sp->value_ct != 2)
            )
         {
              count_error();
              M_out_emsg1(L"verb_type() -- unexpected error -- invalid array: kw parm value");
              msgend_loc(array_value, expression);
              rc = -1;          
         }
         else if ( array_value.vlist_sp->values.at(0).ty != type_E::int64 )
         {
              count_error();
              M_out_emsg1(L"verb_type() -- unexpected error -- 1st positional value in array:[vlist] is not valid int64");
              msgend_loc(array_value.vlist_sp->values.at(0), expression);
              rc = -1;           
         }   
         else
         {
             acount = (uint64_t)(array_value.vlist_sp->values.at(0).int64);              // capture number of elements
         
             if ( array_value.vlist_sp->values.at(1).ty != type_E::typdef)
             {
                 count_error();
                 M_out_emsg1(L"verb_type() -- unexpected error -- 2nd positional value in array:[vlist] is not typdef");
                 msgend_loc(array_value.vlist_sp->values.at(1), expression);
                 rc = -1;          
             }
             else
             {   
                 M__(M_out(L"verb_type() -- array element tsize = %d") % array_value.vlist_sp->values.at(1).typdef_sp->tsize;)

                 if (array_value.vlist_sp->values.at(1).typdef_sp->tsize == 0)    // is typdef for array element a non-sized (invalid) typdef?
                 {
                     count_error();
                     M_out_emsg1(L"verb_type() -- unexpected error -- array element typdef is non-sized (not atomic or aggregate)");
                     msgend_loc(array_value.vlist_sp->values.at(1), expression);
                     rc = -1;             
                 }


                 // set up array typdef_S -- count and element type are OK for arrays

                 auto ma_rc = make_array_typdef(acount, *(array_value.vlist_sp->values.at(1).typdef_sp), typdef); //  builds array typdef_S into typdef output parm -- all values are unshared in typdef_S

                 if (ma_rc != 0)                                                                                  //  unexpected make_array_typdef() failure ???
                 {
                     count_error();
                     M_out_emsg1(L"verb_type() -- unexpected error -- make_array_typdef() failed");
                     msgend_loc(array_value.vlist_sp->values.at(1), expression);
                     rc = -1;             
                 }              
             }       // array element type is typdef 
         }           // number of array elements is OK   
     }              // array: present


     // process structure type      
     // ----------------------
   
     if (struct_rc == 0)
     {
         // build vector with decoded field parms for passing to make_structure_typdef()
         // ----------------------------------------------------------------------------

         std::vector<fieldparm_S> fieldparms { };


         // loop through nested vlist of field definitions (this is the vlist for the struct: keyword)  

         for (auto& elem : struct_value.vlist_sp->values)   // elem should be n-th positional value_S->vlist  
         {
              M__(display_vlist(*(elem.vlist_sp), L"*(elem.vlist_sp)");)


              // collect parm info about n-th field 

              fieldparm_S fieldparm { };

              fieldparm.fieldname = M_get_nest_pos_string(  *(elem.vlist_sp), 0);                  // 1st positional parm (0) is fieldname
              fieldparm.typdef_p  = M_get_nest_pos_typdef_p(*(elem.vlist_sp), 1);                  // 2nd positional parm (0) is typdef_S for this field

              value_S offset_value    { };
              value_S skip_value      { };
            
              auto offset_rc = get_vlist_keyword(*(elem.vlist_sp), L"offset", offset_value    );   // get offset:nnnn   (if present)
              auto skip_rc   = get_vlist_keyword(*(elem.vlist_sp), L"skip"  , skip_value      );   // get skip:nnnn     (if present)  
              auto same_rc   = get_vlist_keyword(*(elem.vlist_sp), L"same"                    );   // get same:         (if present)
              auto high_rc   = get_vlist_keyword(*(elem.vlist_sp), L"high"                    );   // get high:         (if present)

              if (offset_rc == 0)
              {
                  fieldparm.offset     = offset_value.int64; 
                  fieldparm.has_offset = true;
              }

              if (skip_rc == 0)
              {
                  fieldparm.skip       = skip_value.int64;
                  fieldparm.has_skip   = true; 
              }
            
              if (same_rc == 0)
                  fieldparm.same_as_prior = true; 
              
              if (high_rc == 0)
                  fieldparm.at_hi_watermark = true; 


              // add collected field parms for n-th field into field parms vector

              M__(M_out(L"verb_type() -- fieldname = %S  type=%S   offset=%d    skip=%d") % fieldparm.fieldname % type_str(fieldparm.typdef_p->kind) % fieldparm.offset % fieldparm.skip;)    
         
              fieldparms.push_back(fieldparm);    
         } 


         // set up structure typdef_S -- based on collected fieldparm info 

         auto mr_rc = make_structure_typdef(fieldparms, typdef);
         M__(M_out(L"verb_type() -- make_structure_typdef() returned");)

         if (mr_rc != 0)                                                                                  //  make_structure_typdef() failure ?
         {
             count_error();
             M_out_emsg1(L"verb_type() -- make_structure_typdef() failed -- see earlier error messages");
             msgend_loc(expression);
             rc = -1;             
         } 
     }


     // ------------------------------------------------------------------------------------------
     // if name was provided, add this type-name to  global verbmain, static, or local stack frame 
     // ------------------------------------------------------------------------------------------

     if (name_rc == 0)
     {
        if (global_rc == 0)                     // global: keyword was present
        {
            if (is_global_identifier_defined(name_value.string))
            {
                count_error();
                M_out_emsg1(L"verb_type() -- global identifier %s is already defined -- unable to define it again") % name_value.string;
                msgend_loc(name_value, expression);
                rc = -1;        
            }
            else
            {
                auto rc = def_global_typdef(name_value.string, typdef);               // add new type definition to environment -- global symbols are always exposed  -- don't bother unsharing -- types are immutable 
                M__(M_out(L"verb_type() -- def_global_typdef() returned");)

                // errors are unexpected here
               
                if (rc != 0)
                {
                    //count_error(); already counted in def_global_typdef()
                    M_out_emsg1(L"verb_type() -- unexpected error from def_global_typdef() -- unable to define new global identifier = %s") % name_value.string;
                    msgend_loc(name_value, expression);
                    rc = -1; 
                }  
            }
        }
        else if (static_rc == 0)          // static: keyword was present
        {
            if (is_static_identifier_defined(frame, name_value.string))
            {
                count_error();
                M_out_emsg1(L"verb_type() -- static identifier %s is already defined -- unable to define it again") % name_value.string;
                msgend_loc(name_value, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
                parm.exposed = (expose_rc == 0); 
#endif

                auto rc = def_static_typdef(frame, name_value.string, typdef, parm);          // add new non-constant variable to environment  -- expose based on expose: kw rc -- don't bother unsharing -- types are immutable
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_static_typdef()
                    M_out_emsg1(L"verb_type() -- unexpected error from def_static_typdef() -- unable to define new static identifier = %s") % name_value.string;
                    msgend_loc(name_value, expression);
                    rc = -1;  
                }  
            }
        }  
        else if (verbmain_rc == 0)          // verbmain: keyword was present
        {
            if (is_verbmain_identifier_defined(frame, name_value.string))
            {
                count_error();
                M_out_emsg1(L"verb_type() -- verbmain identifier %s is already defined -- unable to define it again") % name_value.string;
                msgend_loc(name_value, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
                parm.exposed = (expose_rc == 0); 
#endif

                auto rc = def_verbmain_typdef(frame, name_value.string, typdef, parm);          // add new non-constant variable to environment  -- expose based on expose: kw rc -- don't bother unsharing -- types are immutable
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_verbmain_typdef()
                    M_out_emsg1(L"verb_type() -- unexpected error from def_verbmain_typdef() -- unable to define new verbmain identifier = %s") % name_value.string;
                    msgend_loc(name_value, expression);
                    rc = -1;  
                }  
            }
        }     
        else          // neither global:, verbmain:, nor static: were not present -- define local typdef
        {
            if (is_local_identifier_defined(frame, name_value.string))
            {
                count_error();
                M_out_emsg1(L"verb_type() -- local identifier %s is already defined -- unable to define it again") % name_value.string;
                msgend_loc(name_value, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
                parm.exposed = (expose_rc == 0);
#endif

                auto rc = def_local_typdef(frame, name_value.string, typdef, parm);          // add new non-constant variable to environment  -- expose based on expose: kw rc -- don't bother unsharing -- types are immutable
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_local_typdef()
                    M_out_emsg1(L"verb_type() -- unexpected error from def_local_typdef() -- unable to define new local identifier = %s") % name_value.string;
                    msgend_loc(name_value, expression);
                    rc = -1;  
                }  
            }
        }          // local
     }             // name: provided


     // ---------------------------------------------
     // return with results = new typdef_S (or error)
     // ---------------------------------------------

     if (rc == 0)
     {
         value_S value { typdef_val(typdef) };           // put typdef_S into a value_S
      // unshare_value(value);                           // make sure all nested items are unshared -- shouldn't be required 
         results = to_results(value);                    // pass back new typdef_S definition
     }
     else
     {
         results = error_results();
     }

     M__(M_out(L"verb_type() -- returning");)
     return rc;  
}        
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @TO_XXXXX type conversion verbs
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>      bool verb__to_bool(   T x) {return (     bool)x;}
template<typename T>    int8_t verb__to_int8(   T x) {return (   int8_t)x;}
template<typename T>   int16_t verb__to_int16(  T x) {return (  int16_t)x;}
template<typename T>   int32_t verb__to_int32(  T x) {return (  int32_t)x;}
template<typename T>   int64_t verb__to_int64(  T x) {return (  int64_t)x;}
template<typename T>   uint8_t verb__to_uint8(  T x) {return (  uint8_t)x;}
template<typename T>  uint16_t verb__to_uint16( T x) {return ( uint16_t)x;}
template<typename T>  uint32_t verb__to_uint32( T x) {return ( uint32_t)x;}
template<typename T>  uint64_t verb__to_uint64( T x) {return ( uint64_t)x;}
template<typename T> float32_T verb__to_float32(T x) {return (float32_T)x;}
template<typename T> float64_T verb__to_float64(T x) {return (float64_T)x;}    


//---------------------------------------------------------------------------------
//   @TO_BOOL xxx
//---------------------------------------------------------------------------------

int verb_to_bool(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_bool() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert input value to boolean true/false value

    if (is_value_true(in_val))
        out_val = boolean_val(true , in_val.token_ix1, in_val.token_ix1);
    else
        out_val = boolean_val(false, in_val.token_ix1, in_val.token_ix1);
        

    results = to_results(out_val);
    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @TO_INT8 xxx
//---------------------------------------------------------------------------------

int verb_to_int8(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_int8() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = int8_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting  -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_int8, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to int8_t -- complain about any conversion errors

        int8_t  int8 { }; 
        auto trc = to_int8(in_val.string, int8);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_int8() -- error occurred when converting from string = «%s» to int8 value") % in_val.string; 
            msgend_loc(in_val, expression);
        
            results = error_results(); 
            return -1; 
        }   

        out_val = int8_val(int8, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_INT16 xxx
//---------------------------------------------------------------------------------

int verb_to_int16(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_int16() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = int16_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting  -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_int16, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to int16_t -- complain about any conversion errors

        int16_t  int16 { }; 
        auto trc = to_int16(in_val.string, int16);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_int16() -- error occurred when converting from string = «%s» to int16 value") % in_val.string; 
            msgend_loc(in_val, expression);   
        
            results = error_results(); 
            return -1; 
        }   

        out_val = int16_val(int16, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_INT32 xxx
//---------------------------------------------------------------------------------

int verb_to_int32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_int32() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = int32_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting     -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_int32, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to int32_t -- complain about any conversion errors

        int32_t  int32 { }; 
        auto trc = to_int32(in_val.string, int32);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_int32() -- error occurred when converting from string = «%s» to int32 value") % in_val.string; 
            msgend_loc(in_val, expression);
            results = error_results(); 
            return -1; 
        }   

        out_val = int32_val(int32, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_INT64 xxx
//---------------------------------------------------------------------------------

int verb_to_int64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_int64() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = int64_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting    -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_int64, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to int64_t -- complain about any conversion errors

        int64_t  int64 { }; 
        auto trc = to_int64(in_val.string, int64);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_int64() -- error occurred when converting from string = «%s» to int64 value") % in_val.string; 
            msgend_loc(in_val, expression);   
        
            results = error_results(); 
            return -1; 
        }   

        out_val = int64_val(int64, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_UINT8 xxx
//---------------------------------------------------------------------------------

int verb_to_uint8(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_uint8() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = uint8_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting    -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_uint8, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to uint8_t -- complain about any conversion errors

        uint8_t  uint8 { }; 
        auto trc = to_uint8(in_val.string, uint8);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_uint8() -- error occurred when converting from string = «%s» to uint8 value") % in_val.string; 
            msgend_loc(in_val, expression);   
        
            results = error_results(); 
            return -1; 
        }   

        out_val = uint8_val(uint8, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_UINT16 xxx
//---------------------------------------------------------------------------------

int verb_to_uint16(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_uint16() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = uint16_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting   -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_uint16, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to uint16_t -- complain about any conversion errors

        uint16_t  uint16 { }; 
        auto trc = to_uint16(in_val.string, uint16);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_uint16() -- error occurred when converting from string = «%s» to uint16 value") % in_val.string; 
            msgend_loc(in_val, expression);   
        
            results = error_results(); 
            return -1; 
        }   

        out_val = uint16_val(uint16, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf




//---------------------------------------------------------------------------------
//   @TO_UINT32 xxx
//---------------------------------------------------------------------------------

int verb_to_uint32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_uint32() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = uint32_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting   -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_uint32, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to uint32_t -- complain about any conversion errors

        uint32_t  uint32 { }; 
        auto trc = to_uint32(in_val.string, uint32);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_uint32() -- error occurred when converting from string = «%s» to uint32 value") % in_val.string; 
            msgend_loc(in_val, expression);  
        
            results = error_results(); 
            return -1; 
        }   

        out_val = uint32_val(uint32, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_UINT64 xxx
//---------------------------------------------------------------------------------

int verb_to_uint64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_uint64() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = uint64_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting    -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_uint64, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to uint64_t -- complain about any conversion errors

        uint64_t  uint64 { }; 
        auto trc = to_uint64(in_val.string, uint64);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_uint64() -- error occurred when converting from string = «%s» to uint64 value") % in_val.string; 
            msgend_loc(in_val, expression); 
        
            results = error_results(); 
            return -1; 
        }   

        out_val = uint64_val(uint64, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_FLOAT32 xxx
//---------------------------------------------------------------------------------

int verb_to_float32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_float32() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = float32_val(0.0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_float32, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to float32_t -- complain about any conversion errors

        float32_T  float32 { }; 
        auto trc = to_float32(in_val.string, float32);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_float32() -- error occurred when converting from string = «%s» to float32 value") % in_val.string; 
            msgend_loc(in_val, expression);  
        
            results = error_results(); 
            return -1; 
        }   

        out_val = float32_val(float32, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf
 

//---------------------------------------------------------------------------------
//   @TO_FLOAT64 xxx
//---------------------------------------------------------------------------------

int verb_to_float64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_float64() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = float64_val(0.0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_float64, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to float64_T -- complain about any conversion errors

        float64_T  float64 { }; 
        auto trc = to_float64(in_val.string, float64);
        if (trc != 0)
        {            
            M_out_emsg1(L"verb_to_float64() -- error occurred when converting from string = «%s» to float64 value") % in_val.string; 
            msgend_loc(in_val, expression);  
        
            results = error_results(); 
            return -1; 
        }   

        out_val = float64_val(float64, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf
 

//---------------------------------------------------------------------------------
//   @TO_STR xxx
//---------------------------------------------------------------------------------

int verb_to_str(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, string, or vlist  
    M__(M_out(L"verb_to_str() called");)
     
    results = to_results(string_val( str_value(expression.rparms.values.at(0), false, false, true) ));   
    return 0; 
}
M_endf  
 

//---------------------------------------------------------------------------------
//   @TO_IDENT "string"
//---------------------------------------------------------------------------------

int verb_to_ident(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be string 
    M__(M_out(L"verb_to_ident() called");)
    

    // make sure string contains valid identifier name -- complain and return error, if not

    std::wstring  id_str {expression.rparms.values.at(0).string };              // 1st right positional parm should be a string 


    //  std::wregex   reg    {L"([_]|[[:alpha:]])([_]|[[:alnum:]])*"};    // pattern for identifier names -- does this handle unicode properly???         
    //  if ( !(std::regex_match(id_str, reg)) )


    if (!is_valid_identifier(id_str))
    {
        M_out_emsg1(L"verb_to_ident() -- error occurred when converting from string = «%s» to identifier") % id_str; 
        msgend_loc(expression.rparms.values.at(0), expression); 
  
        results = error_results();    // return error results
        return -1;                    // failure r/c
    }


    //  identifier name is OK -- pass back identifier results 

    results = to_results(identifier_val(id_str));   
    return 0; 
}
M_endf  
 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @IS_XXXXX type testing verbs
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------------
//   @IS_BOOL xxx
//---------------------------------------------------------------------------------

int verb_is_bool(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_bool() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::boolean)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf




//---------------------------------------------------------------------------------
//   @IS_INT8 xxx
//---------------------------------------------------------------------------------

int verb_is_int8(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int8() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::int8)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @IS_INT16 xxx
//---------------------------------------------------------------------------------

int verb_is_int16(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int16() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::int16)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_INT32 xxx
//---------------------------------------------------------------------------------

int verb_is_int32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int32() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::int32)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @IS_INT64 xxx
//---------------------------------------------------------------------------------

int verb_is_int64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int64() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::int64)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_UINT8 xxx
//---------------------------------------------------------------------------------

int verb_is_uint8(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_uint8() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::uint8)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @IS_UINT16 xxx
//---------------------------------------------------------------------------------

int verb_is_uint16(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_uint16() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::uint16)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_UINT32 xxx
//---------------------------------------------------------------------------------

int verb_is_uint32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_uint32() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::uint32)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @IS_UINT64 xxx
//---------------------------------------------------------------------------------

int verb_is_uint64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_uint64() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::uint64)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_FLOAT32 xxxx
//---------------------------------------------------------------------------------

int verb_is_float32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_float32() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::float32)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf




//---------------------------------------------------------------------------------
//   @IS_FLOAT64 xxxx
//---------------------------------------------------------------------------------

int verb_is_float64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_float64() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::float64)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_INT xxxx
//---------------------------------------------------------------------------------

int verb_is_int(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int() called");)
          
    if ( is_value_integer(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_FLOAT xxxx
//---------------------------------------------------------------------------------

int verb_is_float(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_float() called");)
          
    if ( is_value_float(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_NUMERIC xxxx
//---------------------------------------------------------------------------------

int verb_is_numeric(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_numeric() called");)
          
    if ( is_value_arithmetic(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_ATOM xxxx
//---------------------------------------------------------------------------------

int verb_is_atom(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_atom() called");)
          
    if ( is_value_comparable(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_SIGNED xxxx
//---------------------------------------------------------------------------------

int verb_is_signed(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_signed() called");)
          
    if ( is_value_signed(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_UNSIGNED xxxx
//---------------------------------------------------------------------------------

int verb_is_unsigned(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_unsigned() called");)
          
    if ( is_value_unsigned(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_TRUE xxxx
//---------------------------------------------------------------------------------

int verb_is_true(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_true() called");)
          
    if ( is_value_true(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_FALSE xxxx
//---------------------------------------------------------------------------------

int verb_is_false(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_false() called");)
          
    if ( is_value_false(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_STRING xxxx
//---------------------------------------------------------------------------------

int verb_is_string(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_string() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::string)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_UNIT xxxx
//---------------------------------------------------------------------------------

int verb_is_unit(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_unit() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::unit)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf
 

//---------------------------------------------------------------------------------
//   @IS_ITEM xxxx
//---------------------------------------------------------------------------------

int verb_is_expression(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_expression() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::expression)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_VLIST xxxx
//---------------------------------------------------------------------------------

int verb_is_vlist(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_vlist() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::vlist)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_BLOCK xxxx
//---------------------------------------------------------------------------------

int verb_is_block(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_block() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::block)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_DEF xxxx
//---------------------------------------------------------------------------------

int verb_is_def(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side unevaluated identifier positional, and an optional keyword = all: global: local: (all: is default)
    M__(M_out(L"verb_is_def() called");)
    
    bool tf {false};                                                                          // output value                                 


    // see if global:, static:, verbmain:, or local: are defined -- only one of these (or none) should be defined
                                                                  
    auto global_rc   = get_right_keyword(expression, L"global");                              // get value of global:   keyword -- if not present, R/C is -1 
    auto static_rc   = get_right_keyword(expression, L"static");                              // get value of static:   keyword -- if not present, R/C is -1 
    auto verbmain_rc = get_right_keyword(expression, L"verbmain");                            // get value of verbmain: keyword -- if not present, R/C is -1 
    auto local_rc    = get_right_keyword(expression, L"local" );                              // get value of local:    keyword -- if not present, R/C is -1 

    if (global_rc == 0)                                                                       // global present?
        tf = is_global_identifier_defined(expression.rparms.values.at(0).string);             // identifier name should be in .string field 
    else if (static_rc == 0 )                                                                 // static: present?
        tf = is_static_identifier_defined(frame, expression.rparms.values.at(0).string);      // identifier name should be in .string field 
    else if (verbmain_rc == 0 )                                                               // verbmain: present?
        tf = is_verbmain_identifier_defined(frame, expression.rparms.values.at(0).string);    // identifier name should be in .string field 
    else if (local_rc == 0 )                                                                  // local: present?
        tf = is_local_identifier_defined(frame, expression.rparms.values.at(0).string);       // identifier name should be in .string field 
    else                                                                                      // neither global: verbmain: static: or local: -- must be all: or default
        tf = is_identifier_defined(frame, expression.rparms.values.at(0).string);             // identifier name should be in .string field  

    results = tf_results(tf);                                                                 // pass back 0 or 1 as output results
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_VAR xxxx
//---------------------------------------------------------------------------------

int verb_is_var(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side unevaluated identifier positional, and an optional keyword = all: global: local: (all: is default)
    M__(M_out(L"verb_is_var() called");)
    
    bool tf {false};                                                                          // output value                                 


    // see if global:, static:, verbmain:, or local: are defined -- only one of these (or none) should be defined
                                                                  
    auto global_rc   = get_right_keyword(expression, L"global");                              // get value of global:   keyword -- if not present, R/C is -1 
    auto static_rc   = get_right_keyword(expression, L"static");                              // get value of static:   keyword -- if not present, R/C is -1 
    auto verbmain_rc = get_right_keyword(expression, L"verbmain");                            // get value of verbmain: keyword -- if not present, R/C is -1 
    auto local_rc    = get_right_keyword(expression, L"local" );                              // get value of local:    keyword -- if not present, R/C is -1 


    // set tf to true if variable is defined, and not constant

    if (global_rc == 0)                                                                       // global present?
        tf = is_global_identifier_variable(expression.rparms.values.at(0).string);            // identifier name should be in .string field 
    else if (static_rc == 0)                                                                  // static: present?
        tf = is_static_identifier_variable(frame, expression.rparms.values.at(0).string);     // identifier name should be in .string field 
    else if (verbmain_rc == 0 )                                                               // verbmain: present?
        tf = is_verbmain_identifier_variable(frame, expression.rparms.values.at(0).string);   // identifier name should be in .string field 
    else if (local_rc == 0)                                                                   // local: present?
        tf = is_local_identifier_variable(frame, expression.rparms.values.at(0).string);      // identifier name should be in .string field 
    else                                                                                      // neither global: verbmain: static: or local: -- must be all: or default
        tf = is_identifier_variable(frame, expression.rparms.values.at(0).string);            // identifier name should be in .string field 

    results = tf_results(tf);                                                                 // pass back 0 or 1 as output results
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_CONST xxxx
//---------------------------------------------------------------------------------

int verb_is_const(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side unevaluated identifier positional, and an optional keyword = all: global: local: (all: is default)
    M__(M_out(L"verb_is_const() called");)
    
    bool tf {false};                                                                         // output value                                 


    // see if global:, static:, verbmain:, or local: are defined -- only one of these (or none) should be defined
                                                                  
    auto global_rc   = get_right_keyword(expression, L"global");                              // get value of global:   keyword -- if not present, R/C is -1 
    auto static_rc   = get_right_keyword(expression, L"static");                              // get value of static:   keyword -- if not present, R/C is -1 
    auto verbmain_rc = get_right_keyword(expression, L"verbmain");                            // get value of verbmain: keyword -- if not present, R/C is -1 
    auto local_rc    = get_right_keyword(expression, L"local" );                              // get value of local:    keyword -- if not present, R/C is -1 

    if (global_rc == 0)                                                                       // global present?
        tf = is_global_identifier_const(expression.rparms.values.at(0).string);               // identifier name should be in .string field 
    else if (static_rc == 0)                                                                  // static: present?
        tf = is_static_identifier_const(frame, expression.rparms.values.at(0).string);        // identifier name should be in .string field  
    else if (verbmain_rc == 0 )                                                               // verbmain: present?
        tf = is_verbmain_identifier_const(frame, expression.rparms.values.at(0).string);      // identifier name should be in .string field 
    else if (local_rc == 0)                                                                   // local: present?
        tf = is_local_identifier_const(frame, expression.rparms.values.at(0).string);         // identifier name should be in .string field  
    else                                                                                      // neither global: verbmain: static: or local: -- must be all: or default
        tf = is_identifier_const(frame, expression.rparms.values.at(0).string);               // identifier name should be in .string field 

    results = tf_results(tf);                                                                 // pass back 0 or 1 as output results
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
//║╳╳║      structure, array, argument and vlist extraction verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    array # nn -- array subscripting -- get n-th element
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_subscript(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    ref_S ref { };                                                   // resulting lvalue/rvalue reference from subscripting operation                  

    // -----------------------------------------------------------------------------------------------------
    // already known that there is one left-side positional parm  -- can be: unevaluated variable identifier
    //                                                                       unevaluated constant identifier
    //                                                                       unevaluated ref -- to array value      
    //                                                                       evaluated array value
    //
    // already known that there is one right-side positional parm -- type should be int64 
    // -----------------------------------------------------------------------------------------------------


    M__(M_out(L"verb_subscript() called");)


    // get values from left-side and right-side parms (known to be present)

    int64_t array_index  { M_get_right_pos_int64(expression, 0) };       // right-side parm should be index (int64_t)
    value_S source_value { M_get_left_pos(       expression, 0) };       // left-side parm is source 
  

    // handle case with variable or constant (unevaluated) identifier -- needs to have an array value
    // ==============================================================================================

    if (source_value.ty == type_E::identifier)
    {
        // fetch symval for identifier -- should be defined with array type value  -- error, if not 
        // ----------------------------------------------------------------------------------------

        symval_S symval { };                                             // should point to real value_s for the variable/constant
        auto get_rc = get_var(frame, source_value.string, symval);

        if (get_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- unexpected error -- identifier %S is not defined") % source_value.string; 
            msgend_loc(M_get_left_pos(expression, 0), expression);
       
            results = error_results(); 
            return -1; 
        }

        if (symval.value_sp->ty != type_E::array)
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- identifier %S is type <%S>, not array -- cannot apply subscript to a non-array value") % source_value.string % type_str(symval.value_sp->ty); 
            msgend_loc(M_get_left_pos(expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure array index is within range
        // -------------------------------------

        if ( (array_index < 0) || (array_index >= symval.value_sp->typdef_sp->acount) )
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- array index (%d) is out of bounds of array (0 - %d) currently assigned to identifier %S") % array_index %  (symval.value_sp->typdef_sp->acount - 1) % source_value.string ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for i-th array element
        // ----------------------------------------------


        //  get element typdef and compute offset for the i-th array element         

        typdef_S elem_typdef { *(symval.value_sp->typdef_sp->atype_sp) };                // local copy of array element typdef
        uint64_t elem_offset { array_index * elem_typdef.tsize         };                // starting offset for i-th array element 


        // create ref_S and fill in rvalue or lvalue flag based on symval_S 

        auto m_rc = make_reference(ref, symval.value_sp, elem_typdef, elem_offset);      // construct reference based on info from symval -- will have set in_buffer and auto_deref flags -- will not have set rvalue/lvalue flags
                               
        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        } 

        if (symval.is_const)                                                             // if constant, can't have lvalue reference
           ref.is_rvalue = true; 
        else                                                                             // must be variable -- can have lvalue
           ref.is_lvalue = true;   
    }


    // handle case with (anonymous) array value -- probably an immediately-subscripted    @AGGR array:[...] call
    // =========================================================================================================

    else  if (source_value.ty == type_E::array)
    {
        // make sure array index is within range
        // -------------------------------------

        if ( (array_index < 0) || (array_index >= source_value.typdef_sp->acount) )
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- array index (%d) is out of bounds of passed-in (anonymous) array (0 - %d)") % array_index %  (source_value.typdef_sp->acount - 1) ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for i-th array element
        // ----------------------------------------------
        
        //  get element typdef and compute offset for the i-th array element         

        typdef_S elem_typdef { *(source_value.typdef_sp->atype_sp)     };                // local copy of array element typdef
        uint64_t elem_offset { array_index * elem_typdef.tsize         };                // starting offset for i-th array element 


        // create ref_S (to a new copy of the anonymous value) and fill in as rvalue-only  

        std::shared_ptr<value_S> value_sp { new value_S };                               // new value_S that can be controlled via shared ptr
        *value_sp = source_value;                                                        // fill in new value_S, but leave everything pointed to by any shared_ptrs still shared (no unshare_value() issued) 

        auto m_rc = make_reference(ref, value_sp, elem_typdef, elem_offset);             // construct reference based on info from copied value_S -- will have set in_buffer and auto_deref flags -- will not have set rvalue/lvalue flags

        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        } 

        ref.is_rvalue = true;                                                            // always rvalue-only, since this is (anonymous) passed-in value     
    }


    // handle case with reference value -- needs to be to an auto_defer reference to array (probably nested subscripting) 
    // ==================================================================================================================

    else  if (source_value.ty == type_E::ref)
    {
        // verify that passed-in reference is to array
        // -------------------------------------------

        // make sure passed-in reference is not explicitly-created reference (that needs de-referencing first)
 
        if ( !source_value.ref_sp->auto_deref )
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- left-side explicit reference needs to be dereferenced (to an array type) before subscripting"); 
            msgend_loc(M_get_left_pos( expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure passed-in (auto-deref) reference is to an array (not structure, etc.)

        if ( source_value.ref_sp->typdef_sp->kind != type_E::array )
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- left-side parm refers to type <%S> -- it needs to refer to an array type before subscripting") % type_str(source_value.ref_sp->typdef_sp->kind);
            msgend_loc(M_get_left_pos( expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure array index is within range
        // -------------------------------------

        if ( (array_index < 0) || (array_index >= source_value.ref_sp->typdef_sp->acount) )
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- array index (%d) is out of bounds of referenced array (element) (0 - %d)") % array_index %  (source_value.ref_sp->typdef_sp->acount - 1) ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for i-th array element
        // ----------------------------------------------
        
        //  get element typdef and compute offset for the i-th array element         

        typdef_S elem_typdef { *(source_value.ref_sp->typdef_sp->atype_sp)                      };            // local copy of array element typdef
        uint64_t elem_offset { source_value.ref_sp->offset + array_index * elem_typdef.tsize    };            // starting offset for i-th array element (within prior reference's offsetted sub-buffer) 

        auto m_rc = make_reference(ref, source_value, elem_typdef, elem_offset );                             // construct reference based on info from value_S -- should have copied all required flags from source value_S

        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb_subscript() -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        }      
    }


    // handle unexpected value type -- error
    // =====================================

    else    
    {
        count_error(); 
        M_out_emsg1(L"verb_subscript() -- unexpected left-side value -- type = %S") % type_str(source_value.ty) ; 
        msgend_loc(M_get_left_pos(expression, 0), expression);
      
        results = error_results(); 
        return -1;    
    }   


    // pass back completed lvalue or rvalue reference

    results = to_results(ref_val(ref));
    return 0;  
 
}
M_endf  


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    struct . fieldname  -- structure field selection -- get named field
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_select(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    ref_S ref { };                                                            // resulting lvalue/rvalue reference from subscripting operation                  

    // -----------------------------------------------------------------------------------------------------
    // already known that there is one left-side positional parm  -- can be: unevaluated variable identifier
    //                                                                       unevaluated constant identifier
    //                                                                       unevaluated ref -- to struct value      
    //                                                                       evaluated struct value
    //
    // already known that there is one right-side positional parm -- type should be string (fieldname) 
    // -----------------------------------------------------------------------------------------------


    M__(M_out(L"verb_select() called");)


    // get values from left-side and right-side parms (known to be present)

    std::wstring field_name   { M_get_right_pos_string(expression, 0) };      // right-side parm should be field name (string)
    value_S      source_value { M_get_left_pos(        expression, 0) };      //  left-side parm is source 
  

    // handle case with variable or constant (unevaluated) identifier -- needs to have an structure value
    // ==================================================================================================

    if (source_value.ty == type_E::identifier)
    {
        // fetch symval for identifier -- should be defined with array type value  -- error, if not 
        // ----------------------------------------------------------------------------------------

        symval_S symval { };                                                  // should point to real value_s for the variable/constant
        auto get_rc = get_var(frame, source_value.string, symval);

        if (get_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- unexpected error -- identifier %S is not defined") % source_value.string; 
            msgend_loc(M_get_left_pos(expression, 0), expression);
       
            results = error_results(); 
            return -1; 
        }

        if (symval.value_sp->ty != type_E::structure)
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- identifier %S is type <%S>, not structy -- cannot apply field selection to a non-struct value") % source_value.string % type_str(symval.value_sp->ty); 
            msgend_loc(M_get_left_pos(expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure field name is defined in the structure
        // ------------------------------------------------
                
        if ( symval.value_sp->typdef_sp->fnames.count(field_name) <= 0 )
        {
            count_error(); 
            M_out_emsg1(L"verb_seelct() -- field-name (%S) is not defined in the struct-type aggregate currently assigned to identifier %S") % field_name % source_value.string ; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for selected field
        // ------------------------------------------

        //  get field typdef and compute offset for this field 

        auto      field_index  { symval.value_sp->typdef_sp->fnames.at(field_name) };    // get index of field definition in fdefs vector
        fieldef_S field_def    { symval.value_sp->typdef_sp->fdefs.at(field_index) };    // fetch field definition 
        typdef_S  field_typdef { *(field_def.ftype_sp)                             };    // local copy of field typdef_S
        uint64_t  field_offset { field_def.offset                                  };    // starting offset for selected field 


        // create ref_S and fill in rvalue or lvalue flag based on symval_S 

        auto m_rc = make_reference(ref, symval.value_sp, field_typdef, field_offset);    // construct reference based on info from symval -- will have set in_buffer and auto_deref flags -- will not have set rvalue/lvalue flags
                               
        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        } 

        if (symval.is_const)                                                             // if constant, can't have lvalue reference
           ref.is_rvalue = true; 
        else                                                                             // must be variable -- can have lvalue
           ref.is_lvalue = true;   
    }


    // handle case with (anonymous) array value -- probably an immediately-subscripted    @AGGR array:[...] call
    // =========================================================================================================

    else  if (source_value.ty == type_E::structure)
    {
        // make sure field-name is defined in the structure
        // ------------------------------------------------

        if ( source_value.typdef_sp->fnames.count(field_name) <= 0 )
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- field-name (%S) is not defined in passed-in (anonymous) struct") % field_name; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for selected field
        // ------------------------------------------

        //  get field typdef and compute offset for this field 

        auto      field_index  { source_value.typdef_sp->fnames.at(field_name) };              // get index of field definition in fdefs vector
        fieldef_S field_def    { source_value.typdef_sp->fdefs.at(field_index) };              // fetch field definition 
        typdef_S  field_typdef { *(field_def.ftype_sp)                         };              // local copy of field typdef_S
        uint64_t  field_offset { field_def.offset                              };              // starting offset for selected field 
        
        std::shared_ptr<value_S> value_sp { new value_S };                                     // new value_S that can be controlled via shared ptr
        *value_sp = source_value;                                                              // fill in new value_S, but leave everything pointed to by any shared_ptrs still shared (no unshare_value() issued) 

        auto m_rc = make_reference(ref, value_sp, field_typdef, field_offset);                 // construct reference based on info from copied value_S -- will have set in_buffer and auto_deref flags -- will not have set rvalue/lvalue flags

        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        } 

        ref.is_rvalue = true;                                                            // always rvalue-only, since this is (anonymous) passed-in value     
    }


    // handle case with reference value -- needs to be to an auto_defer reference to array (probably nested subscripting) 
    // ==================================================================================================================

    else  if (source_value.ty == type_E::ref)
    {
        // verify that passed-in reference is to array
        // -------------------------------------------

        // make sure passed-in reference is not explicitly-created reference (that needs de-referencing first)
 
        if ( !source_value.ref_sp->auto_deref )
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- left-side explicit reference needs to be dereferenced (to a struct type) before field selection"); 
            msgend_loc(M_get_left_pos( expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure passed-in (auto-deref) reference is to a structure (not array, etc.)

        if ( source_value.ref_sp->typdef_sp->kind != type_E::structure )
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- left-side parm refers to type <%S> -- it needs to refer to a struct type before field selection") % type_str(source_value.ref_sp->typdef_sp->kind);
            msgend_loc(M_get_left_pos( expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure field-name is defined in referred-to struct
        // -----------------------------------------------------

        if ( source_value.ref_sp->typdef_sp->fnames.count(field_name) <= 0 )
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- field-name (%S) is not defined in referenced struct") % field_name; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for selected field
        // ------------------------------------------

        //  get field typdef and compute offset for this field 

        auto      field_index  { source_value.ref_sp->typdef_sp->fnames.at(field_name) };    // get index of field definition in fdefs vector
        fieldef_S field_def    { source_value.ref_sp->typdef_sp->fdefs.at(field_index) };    // fetch field definition 
        typdef_S  field_typdef { *(field_def.ftype_sp)                                 };    // local copy of field typdef_S
        uint64_t  field_offset { field_def.offset                                      };    // starting offset for selected field (within prior reference's offsetted sub-buffer)

        auto m_rc = make_reference(ref, source_value, field_typdef, field_offset );          // construct reference based on info from value_S -- should have copied all required flags from source value_S

        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb_select() -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        }      
    }


    // handle unexpected value type -- error
    // =====================================

    else    
    {
        count_error(); 
        M_out_emsg1(L"verb_select() -- unexpected left-side value -- type = %S") % type_str(source_value.ty) ; 
        msgend_loc(M_get_left_pos(expression, 0), expression);
      
        results = error_results(); 
        return -1;    
    }   


    // pass back completed lvalue or rvalue reference

    results = to_results(ref_val(ref));
    return 0;  
 
}
M_endf  



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




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    keys_vlist() -- helper function for @ARG_ASSIGN and @VL_ASSIGN -- build positional value vlist from keyword instance values
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void keys_vlist(frame_S& frame, const e_expression_S& expression, const vlist_S& in_vlist, vlist_S& out_vlist, const std::wstring& keyname) try
{
    out_vlist = vlist_S { };      // clear out, in case no keyword values to add


    // return imemdiately with empty out_vlist, if keyword is not present in in_vlist 

    auto key_ct = in_vlist.eval_kws.count(keyname); 
    if (key_ct <= 0)
        return; 


    // loop to extract matching keywords from in_vlist

    auto it_k = in_vlist.eval_kws.find(keyname);         // it_k should be valid, since count was non-zero, above

    for (auto i = 0; i < key_ct; i++)
    {
        add_positional_value(out_vlist, it_k->second);  // add in value for this keyword instance       
        it_k++;                                         // advance to next instance of this keyword in in_vlist
    }  

    return; 
}
M_endf
      

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    assign_vlist() -- helper function for @ARG_ASSIGN and @VL_ASSIGN  -- assign 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int assign_vlist(frame_S& frame, const e_expression_S& expression, const vlist_S& vars_vlist, const vlist_S& values_vlist, const value_S& rest, bool define_vars = false, bool make_const = false) try
{ 
    M__(M_out(L"assign_vlist() -- called -- rest.string = %s    (rest.ty == type_E::identifier) = %d") % rest.string % (rest.ty == type_E::identifier);) 

    bool        rest_valid      {rest.ty == type_E::identifier};                      // rest is valid if it contains variable name as the value string                                                                                                                                 
    auto        var_ct       =  vars_vlist.value_ct;                                  // number of variable names in vars_vlist;     
    auto        value_ct     =  values_vlist.value_ct;                                // number of values to assign to variables in values_vlist; 
                                                                                     
    value_S     unit_value      { unit_val() };                                       // unit value for pre-defining variables, as required     
    vlist_S     rest_vlist      {};                                                   // vlist to be assigned to rest (if rest_valid)  
   

        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????


    // main loop to define/assign positional vlist values to corresponding variables
    //------------------------------------------------------------------------------
    
    for (auto i = 0; i < std::max(var_ct, value_ct); i++)
    {
        // if is still in range of variables vlist, define any variables that don't yet exist (if requested by caller), and do value assignment (if values haven't been exhausted)
    
        if (i < var_ct)
        {
            bool just_defined {false};                                                // flag to indicate if variable was defined during this loop pass

            std::wstring var_name = vars_vlist.values.at(i).string;                   // get variable name being defined/assigned-to
    
                                                
            // define this variable, if required
    
            if ( (define_vars) &&  (!is_local_identifier_defined(frame, var_name) ) ) // need to define local variable before assignment? 
            {
                auto src = def_local_var(frame, var_name, unit_value);                // define local var with unit value
                if (src != 0)
                {
                    count_error(); 
                    M_out_emsg1(L"assign_vlist() -- unexpected error from def_local_var(,%s,)") % var_name; 
                    msgend_loc(vars_vlist.values.at(i), expression);
                    return -1;             
                 }

                 just_defined = true;                                                 // indicate that variable was just defined during this loop pass
            }
            else
                just_defined = false;                                                 // variable was not just defined
             
    
            // if i is also in range of values vlist  -- do variable/constant assignment: variable-n = value-n 
    
            if (i < value_ct)
            {
                if (!is_local_identifier_variable(frame, var_name))
                {
                    count_error(); 
                    M_out_emsg1(L"assign_vlist() -- cannot update non-variable (constant or verb) identifier = %s") % var_name; 
                    msgend_loc(vars_vlist.values.at(i), expression);
                    return -1;         
                } 

                auto arc = update_local_var(frame, var_name, values_vlist.values.at(i), make_const);     // note: make_const should be true only when define_var is true
                if (arc != 0)
                {
                    count_error(); 
                    M_out_emsg1(L"assign_vlist() -- unexpected error from update_local_var(,%s,)") % var_name; 
                    msgend_loc(vars_vlist.values.at(i), expression);
                    return -1;             
                } 
            }
            else    // variable exists, but has no corresponding value  -- assign unit value to this variable (if not just defined above)
            {
                if (!just_defined)
                {
                    if (!is_local_identifier_variable(frame, var_name))
                    {
                        count_error(); 
                        M_out_emsg1(L"assign_vlist() -- cannot update non-variable (constant or verb) identifier = %s") % var_name; 
                        msgend_loc(vars_vlist.values.at(i), expression);
                        return -1;         
                    }  

                    auto arc = update_local_var(frame, var_name, unit_value, make_const);
                    if (arc != 0)
                    {
                        count_error(); 
                        M_out_emsg1(L"assign_vlist() -- unexpected error from update_local_var(,%s,) -- (unit value)") % var_name; 
                        msgend_loc(vars_vlist.values.at(i), expression);
                        return -1;             
                    } 
                }
            }   
        }              // i < var_ct
        else           // must be i >= var_ct but < value_ct -- value exists but has no correspinging variable 
        {
             // append this value to the vlist for the "rest" keyword (if any)
        
             if (rest_valid)
                add_positional_value(rest_vlist, values_vlist.values.at(i));   
        }  
    }           // end of main loop


    // if rest variable was provided, attach vlist to value and assign to the "rest" variable

    if (rest_valid)
    {
        M__(M_out(L"assign_vlist() -- rest_valid");)

        if ( (define_vars) &&  (!is_local_identifier_defined(frame, rest.string) ) )            // need to define local variable before assignment? 
        {
            auto src = def_local_var(frame, rest.string, vlist_val(rest_vlist));                // define local variable with rest vlist
            if (src != 0)
            {
                count_error(); 
                M_out_emsg1(L"assign_vlist() -- unexpected error from def_local_var(,%s,) -- (rest of vlist)") % rest.string; 
                msgend_loc(rest, expression);
                return -1;             
            }
        }
        else      // "rest" variable is already defined
        {
            M__(M_out(L"assign_vlist() -- update rest: var -- vlist.value_ct = %d") % rest_vlist.value_ct;)

            if (!is_local_identifier_variable(frame, rest.string))
            {
                count_error(); 
                M_out_emsg1(L"assign_vlist() -- cannot update non-variable (constant or verb) identifier = %s") % rest.string; 
                msgend_loc(expression);
                return -1;         
            }  

            auto arc = update_local_var(frame, rest.string, vlist_val(rest_vlist), make_const); 
            if (arc != 0)
            {
                count_error(); 
                M_out_emsg1(L"assign_vlist() -- unexpected error from update_local_var(,%s,) -- (rest of vlist)") % rest.string; 
                msgend_loc(expression);
                return -1;             
            }         
        }     
    }         

    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    v1 v2 v3 v4 ... v999 rest:v0 key:"string" @ARG_ASSIGN v11 v22 v33 ... v99999 rest:v00 key:"string"  -- assign positional values in arguments to variables on correspinging side 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_arg_assign(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_arg_assign() called");)

    // known that there are zero or more variables (undefined, or defined and non-constant0 on each side, along with optional rest: keyword, also with a single undefined or defined non-constant variable
    // also an optionsl key:"string"keyword can be present on each side     
    

    // do left side argument vlist, if any left-side parms are present 
    // ---------------------------------------------------------------

    if  ((expression.lparms.value_ct > 0) || (expression.lparms.kw_ct > 0) ) 
    {
        value_S key_val  { }; 
        value_S rest_val { }; 
        auto krc = get_left_keyword(expression, L"key" , key_val );                                     // get value of key: keyword  -- if not defined, value will be unit, and R/C -1 
        auto lrc = get_left_keyword(expression, L"rest", rest_val);                                     // get value of rest: keyword -- if not defined, value will be unit, and R/C -1

        M__(M_out(L"verb_arg_assign() -- left-side rest: = %s    key: = %s") % rest_val.string % key_val.string;)

        if (krc != 0)
        {
            // key: not present -- do positional values in vlist 

            auto arc = assign_vlist(frame, expression, expression.lparms, frame.lparms, rest_val, true, false); // define variables  -- non-const
            if (arc != 0)
            {
                results = error_results(); 
                return arc;
            }
        }
        else   
        {
            // key: present -- do values associated with key:"keyname"
    
            vlist_S key_vlist {}; 
            keys_vlist(frame, expression, frame.lparms, key_vlist, key_val.string);   // extract keyword instance values into key_vlist
     
            auto arc = assign_vlist(frame, expression, expression.lparms, key_vlist, rest_val, true, false);   // define variables  -- non-const
            if (arc != 0)
            {
                results = error_results(); 
                return arc;
            }     
        }
    }          // end of left-side processing


    // do right side argument vlist, if any right-side parms are present 
    // -----------------------------------------------------------------

    if  ((expression.rparms.value_ct > 0) || (expression.rparms.kw_ct > 0) ) 
    {
        value_S key_val  { }; 
        value_S rest_val { }; 
        auto krc = get_right_keyword(expression, L"key" , key_val );                                     // get value of key: keyword  -- if not defined, value will be unit, and R/C -1 
        auto lrc = get_right_keyword(expression, L"rest", rest_val);                                     // get value of rest: keyword -- if not defined, value will be unit, and R/C -1

        M__(M_out(L"verb_arg_assign() -- right-side rest: = %s    key: = %s") % rest_val.string % key_val.string;)

        if (krc != 0)
        {
            // key: not present -- do positional values in vlist 

            auto arc = assign_vlist(frame, expression, expression.rparms, frame.rparms, rest_val, true, false);        // define variables -- non-const
            if (arc != 0)
            {
                results = error_results(); 
                return arc;
            }
        }
        else   
        {
            // key: present -- do values associated with key:"keyname"
    
            vlist_S key_vlist {}; 
            keys_vlist(frame, expression, frame.rparms, key_vlist, key_val.string);                       // extract keyword instance values into key_vlist
     
            auto arc = assign_vlist(frame, expression, expression.rparms, key_vlist, rest_val , true, false);            // define variables -- non-const
            if (arc != 0)
            {
                results = error_results(); 
                return arc;
            }      
        }
    }         // end of right-side processing   


    // no error -- return normally
               
    results = unit_results();                   // return unit results
    return 0; 
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @ARG_CT    -or-     @ARG_CT left: key:"string"   -or    @ARG_CT right: key:"string"     -- get left-side or right-side positional or keyword argument counts for this frameblock/verb-block/mainblock
//
//
//          @ARG_CT left:               -- return total number of left-side  positional arguments
//          @ARG_CT right:              -- return total number of right-side positional arguments  
//          @ARG_CT left:  allkeys:     -- return total number of left-side  keyword arguments
//          @ARG_CT right: allkeys;     -- return total number of right-side keyword arguments     
//          @ARG_CT left:  key:"xxx"    -- return number of times left-side  keyword "xxx" appears
//          @ARG_CT right: key:"xxx"    -- return number of times right-side keyword "xxx" appears
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_arg_ct(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are no positional parms, and an optional right: or left: keyword 
    //    also -- an optional key:"xxxx" string keyword or allkeys: keyword (both allkeys: and key: cannot both be present   

    M__(M_out(L"verb_arg_ct() called");)
    value_S ct_value { }; 

    // see if left: or key:/allkeys: keywords are present on right side

    value_S left_value      { };
    value_S key_value       { };
    value_S allkeys_value   { };

    auto left_rc     = get_right_keyword(expression, L"left"    ,  left_value    ); // r/c = -1, if left: keyword is not present -- if left: not present, right: must be 
    auto key_rc      = get_right_keyword(expression, L"key"     ,  key_value     ); // r/c = -1, if key:     keyword is not present 
    auto allkeys_rc  = get_right_keyword(expression, L"allkeys" ,  allkeys_value ); // r/c = -1, if allkeys: keyword is not present


    // get right/left positional/keyword count, as requested

    if (key_rc == 0)                                                                // key: is present ?
    {
        // key:"xxx"-- get count of specified keyword
    
        if (left_rc == 0)                                                           // left: is present  
            ct_value = int64_val(frame.lparms.eval_kws.count(key_value.string));    // get number of left keywords 
        else                                                                        // left: is not present -- assume right: present, or neither left: or right: present (default is right-side count) 
            ct_value = int64_val(frame.rparms.eval_kws.count(key_value.string));    // get number of right keywords
    }
    else if (allkeys_rc == 0)                                                       // allkeys: present  ?
    {
        // allkeys: -- get count of all keywords
    
        if (left_rc == 0)                                                           // left: is present  
            ct_value = int64_val(frame.lparms.eval_kws.size());                     // get number of left keywords 
        else                                                                        // left: is not present -- assume right: present, or neither left: or right: present (default is right-side count) 
            ct_value = int64_val(frame.rparms.eval_kws.size());                     // get number of right keywords
    }
    else                                                                            // allkeys: and key: not present
    {
         // get count of positional parms

         if (left_rc == 0)                                                          // left: is present 
              ct_value = int64_val(frame.lparms.values.size());                     // get number of left positional parms 
         else                                                                       // left: is not present -- assume right: present, or neither left: or right: present (default is right-side count) 
              ct_value = int64_val(frame.rparms.values.size());                     // get number of right positional parms
    }
                                                                                   
    results = to_results(ct_value);                                                 // return left/right arg count as output value
    return 0;
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @ARGS left: right: -- return vlist value with either left-side or right-side vlist for this frameblock/verb-block/mainblock
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_args(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are no positional parms, and an optional right: or left: keyword  

    M__(M_out(L"verb_args() called");)
    value_S args_value { }; 

    // see if left: keyword is present on right side

    value_S left_value { };
    auto lrc = get_right_keyword(expression, L"left", left_value);  // r/c = -1, if left: keyword is not present 


    // get right/left vlist, as requested  

    if (lrc == 0)                                            // left: is present  
         args_value = vlist_val(frame.lparms);               // get left vlist for frameblock/verb-block/mainblock 
    else                                                     // left: is not present -- assume right: present, or neither left: or right: present (default is right-side count) 
         args_value = vlist_val(frame.rparms);               // get right vlist for frameblock/verb-block/mainblock

                                                                                   
    results = to_results(args_value);                        // return left/right frameblock/verb-block/mainblock parm vlist as output value
    return 0;
}
M_endf  


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//         @ARG n right: left: key:"string"           -- get n-th left_side/right-side positional/keyword parm for this frameblock/verb-block/mainblock
//
//               @ARG n left:               -- return n-th left-side  positional argument
//               @ARG n right:              -- return n-th right-side positional argument  
//               @ARG n left:  allkeys:     -- return n-th left-side  keyword arguments
//               @ARG n right: allkeys;     -- return n-th right-side keyword arguments     
//               @ARG n left:  key:"xxx"    -- return n-th left-side  keyword "xxx" argument
//               @ARG n right: key:"xxx"    -- return n-th right-side keyword "xxx" argument
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

int verb_arg(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional int64 parm (0-N)
    //  also -- an optional right: or left: keyword, and an optional key: string keyword or allkeys: keyword (both allkeys: and key: cannot both be present   

    M__(M_out(L"verb_arg() called");)
    value_S arg_value { unit_val() };                                                      // unit value, in case n-th parm does not exist


    // get positional parm number requested by caller -- 1st right parm should be present and be >= 0

    auto parm_n = expression.rparms.values.at(0).int64;          

 
    // see if left: or key:/allkeys: keywords are present on right side

    value_S left_value      { };
    value_S key_value       { };
    value_S allkeys_value   { };

    auto left_rc     = get_right_keyword(expression, L"left"    ,  left_value    );         // r/c = -1, if left: keyword is not present -- if left: not present, right: must be 
    auto key_rc      = get_right_keyword(expression, L"key"     ,  key_value     );         // r/c = -1, if key:     keyword is not present 
    auto allkeys_rc  = get_right_keyword(expression, L"allkeys" ,  allkeys_value );         // r/c = -1, if allkeys: keyword is not present
     

    if  (key_rc == 0)                                                                       // key:"xxxx" present -- caller wants keyword "key_value.string" parm 
    {
        // get n-th occurrence of key:"string" keyword 
     
        if (left_rc == 0)                                                                   // caller wants n-th occurrence of keyword  in left-side vlist 
            (void) get_vlist_keyword(frame.lparms, key_value.string, arg_value, parm_n);    // r/c=-1 and at_value is unit, if n-th occurrence of this keyword is not present
        else                                                                                // caller wants n-th occurrence of keyword  in left-side vlist
            (void) get_vlist_keyword(frame.rparms, key_value.string, arg_value, parm_n);    // r/c=-1 and at_value is unit, if n-th occurrence of this keyword is not present                                                                                                
    }
    else if  (allkeys_rc == 0)                                                              // allkeys: present -- caller wants value for n-th keyword of any name 
    {
        // get n-th keyword in vlist -- with any keyword name
     
        if (left_rc == 0)                                                                   // caller wants value from n-th keyword  in left-side vlist 
            (void) get_vlist_keyword(frame.lparms, arg_value, parm_n);                      // r/c=-1 and at_value is unit, if n-th occurrence of this keyword is not present
        else                                                                                // caller wants value from n-th keyword  in left-side vlist
            (void) get_vlist_keyword(frame.rparms, arg_value, parm_n);                      // r/c=-1 and at_value is unity, if n-th occurrence of this keyword is not present                                                                                                
    }  
    else                                                                                    // neither key:"xxxx" nor allkeys: present -- caller wants positional parm
    {
        // get n-th positional parm on requested side 
     
        if (left_rc == 0)                                                                   // caller wants n-th positional value in left-side vlist     
            (void)get_vlist_positional(frame.lparms, arg_value, parm_n);                    // r/c=-1 and at_value is unit, if n-th  parm not present  
        else                                                                                // caller wants n-th positional value in right-side vlist
            (void)get_vlist_positional(frame.rparms, arg_value, parm_n);                    // r/c=-1 and at_value is unit, if n-th  parm not present
    }

    results = to_results(arg_value);                        // return value from n-th left/right parm as output value
    return 0;
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    v1 v2 v3 v4 ... v999 rest:v0 @VL_ASSIGN [vlist] key:"string"  -- assign positional/keyword values in vlist to variables on left side 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_vl_assign(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_vl_assign() called");)

    // known that there are one or more variables (undefined, or defined and non-constant0 on left side, along with optional rest: keyword, also with a single undefined or defined non-constant variable
    // right-side is known to contain one mandatory vlist parm, and one optional key:"string" parm     
    
    value_S key_val  { }; 
    value_S rest_val { }; 
    auto krc = get_right_keyword(expression, L"key" , key_val );                                     // get value of key: keyword  -- if not defined, value will be unit, and R/C -1 
    auto lrc = get_left_keyword( expression, L"rest", rest_val);                                     // get value of rest: keyword -- if not defined, value will be unit, and R/C -1

    M__(M_out(L"verb_vl_assign() -- rest: = %s    key: = %s") % rest_val.string % key_val.string;)

    if (krc != 0)
    {
        // key: not present -- do positional values in vlist 

        auto arc = assign_vlist(frame, expression, expression.lparms, *(expression.rparms.values.at(0).vlist_sp), rest_val, false); // don't define variables
        if (arc != 0)
        {
            results = error_results(); 
            return arc;
        }
    }
    else   
    {
        // key: present -- do values associated with key:"keyname"
    
        vlist_S key_vlist {}; 
        keys_vlist(frame, expression, *(expression.rparms.values.at(0).vlist_sp), key_vlist, key_val.string);   // extract keyword instance values into key_vlist
     
        auto arc = assign_vlist(frame, expression, expression.lparms, key_vlist, rest_val, false);              // don't define variables
        if (arc != 0)
        {
            results = error_results(); 
            return arc;
        }     
    }


    // no error -- return normally
               
    results = unit_results();                       // return unit results
    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    [vlist] @AT n key:"string"  -or-    [vlist] ° n key:"string"         -- get n-th positional value in vlist
//
//      [vlist] @AT n              -- return n-th positional value in vlist
//      [vlist] @AT n allkeys:     -- return n-th keyword value in vlist
//      [vlist] @AT n key:"xxx"    -- return value from n-th occurence of keyword "xxx" vlist
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_at(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional int64 parm (0-N), and a left-side vlist positional parm.  
    //  also -- an optional key:"xxxx" keyword or allkeys: keyword (both allkeys: and key: cannot both be present  

    M__(M_out(L"verb_at() called");)
    value_S at_value { unit_val() };                                                                                 // unit value, in case n-th value does not exist


    // get positional value or keyword occurrence number  

    auto value_n = expression.rparms.values.at(0).int64; 


    // see if key:/allkeys: keywords are present on right side

    value_S key_value       { };
    value_S allkeys_value   { };

    auto key_rc      = get_right_keyword(expression, L"key"     ,  key_value     );                                   // r/c = -1, if key:     keyword is not present 
    auto allkeys_rc  = get_right_keyword(expression, L"allkeys" ,  allkeys_value );                                   // r/c = -1, if allkeys: keyword is not present
                                                                                                                     

    // extract requested positional or keyword value

    if       (key_rc     == 0)                                                                                        // key:":xxxx" present? -- caller wants n-th keyword "xxxx" value in vlist 
        (void)get_vlist_keyword(   *(expression.lparms.values.at(0).vlist_sp), key_value.string, at_value, value_n);  // r/c=-1 and at_value is unit, if n-th occurrence of this keyword is not present 
    else if  (allkeys_rc == 0)                                                                                        // allkeys: present ? -- caller wants value from n-th keyword overall
        (void)get_vlist_keyword(   *(expression.lparms.values.at(0).vlist_sp),                   at_value, value_n);  // r/c=-1 and at_value is unit, if n-th keyword overall is not present 
    else                                                                                                              // neither allkey: nor key:"xxxx" -- caller wants n-th positional value
        (void)get_vlist_positional(*(expression.lparms.values.at(0).vlist_sp),                   at_value, value_n);  // r/c=-1 and at_value is unit, if n-th  parm not present  
                                                                                                              
    
    results = to_results(at_value);                                                                                   // return value from n-th value in vlist as output value
    return 0;
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @VL_CT [vlist] key:"string"   -- get number of values in vlist, or number of occurrences of keyword "string"
//
//        @VL_CT [vlist]              - return total number of positional values in vlist
//        @VL_CT [vlist] allkeys:     - return total number of keywords in vlist
//        @VL_CT [vlist] key:"xxx"    - return number occurrences of keyword "xxx" in vlist//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_vl_ct(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional int64 parm (0-N), and a left-side vlist positional parm.    

    M__(M_out(L"verb_vl_ct() called");)
    value_S ct_value { unit_val() };                                                                                 // unit value, in case n-th value does not exist


    // see if key:/allkeys: keywords are present on right side

    value_S key_value       { };
    value_S allkeys_value   { };

    auto key_rc      = get_right_keyword(expression, L"key"     ,  key_value     );                                  // r/c = -1, if key:     keyword is not present 
    auto allkeys_rc  = get_right_keyword(expression, L"allkeys" ,  allkeys_value );                                  // r/c = -1, if allkeys: keyword is not present


    // determine requested positional or value count 

    if       (key_rc    == 0)                                                                                        // key:"xxxx"present -- caller wants number of occurrences of keyword "xxxx"
        ct_value = int64_val( ((expression.rparms.values.at(0).vlist_sp)->eval_kws).count(key_value.string) );       // number of occurrences of requested keyword 
    else if (allkeys_rc == 0)                                                                                        // allkey: present -- caller wants count of all keywords in vlist 
        ct_value = int64_val( ((expression.rparms.values.at(0).vlist_sp)->eval_kws).size()                  );       // total number of all keywords in vlist  
    else                                                                                                             // neither key:"xxxx"nor allkeys: present -- caller wants count of positional values
        ct_value = int64_val( ((expression.rparms.values.at(0).vlist_sp)->values).size()                    );       // number of positional values 
                                                                                                                                      
    
    results = to_results(ct_value);                                                                                  // return appropriate count
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
//║╳╳║      Output-oriented verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
                                 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @STR   value value value ...  debug:    
//    @SAY   value value value ...  debug: no_nl:
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
//   verb_str() -- @STR
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
//   verb_say() -- @SAY 
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
//
//    @INTERPOLATE "string"     
//    @FORMAT      "string" value_1 value_2 value_3 value_4 ... value_n         
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// -------------------------------------------
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
                                                                  



//---------------------------------------------------------------------------------
//   interpolate_string() -- helper function for string interpolation verbs
//---------------------------------------------------------------------------------

static int interpolate_string( i_parm_S&             iparm                                    // miscellaneous parms
                             , const value_S&        verb_in_value                            // value containing main input format string 
                             , results_S&            results                                  // results for main interpolation-type verb 
                             , int                 (*fcn_p)( frame_S&                         // pass through current frame for invoking code, etc.
                                                           , const e_expression_S&            // pass through expression for getting parms and for location messages 
                                                           , results_S&                       // (error) results to be returned from main verb, if R/C is non-0 
                                                           , const std::wstring&              // input string to be processed 
                                                           , std::wstring&                    // output string to be appended to main output string from verb
                                                           , const std::wstring&              // acccumulated main output string from verb (may not be used)
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
            count_error(); 
            M_out_emsg1(L"%S -- closing \"%S\" not found after opening \"%S\" at offset %d in string being processed") % iparm.ws_message % iparm.ws_end % iparm.ws_begin % idx1;
            msgend_loc(verb_in_value, *(iparm.expression_p));
            results = error_results(); 
            return -1;             
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
     
        std::wstring ws_call_in { ws_verb_in.substr(idx1s, idx2s - idx1s) };         // isolate string for parsing, etc. (with or without start/end delimiters)
       
        M__(M_out(L"interpolate_string() -- ws_inter = «%S»") % ws_call_in;)


        // call passed-in function to process this interpolated section
        // ============================================================

        std::wstring  ws_call_out  { }; 
        results_S     call_results { };

        auto crc = (*fcn_p)( *(iparm.frame_p), *(iparm.expression_p), call_results, ws_call_in, ws_call_out, ws_verb_out, parm_p); 


        // end loop and return imemdiately if called function returned with error

        if (crc != 0)
        {
             M_out_emsg1(L"%S -- string interpolation ending because called function failed for interpolated section = \"%S%S%S\"") % iparm.ws_message % iparm.ws_begin % ws_call_in % iparm.ws_end;
             msgend_loc(verb_in_value, *(iparm.expression_p));
             results = call_results;                                        // verb results = called function results 
             return crc;                                                    // verb R/C     = called function R/C
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



//---------------------------------------------------------------------------------
//   verb_interpolate() -- @INTERPOLATE
//---------------------------------------------------------------------------------


// inner function passed in to interpolate_string() to do actual processing for one interpolated section
// -----------------------------------------------------------------------------------------------------

static int verb_interpolate_1( frame_S&              frame
                             , const e_expression_S& expression
                             ,       results_S&      results
                             , const std::wstring&   ws_in
                             ,       std::wstring&   ws_out
                             , const std::wstring&   verb_out
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
        count_error(); 
        M_out_emsg1(L"verb_interpolate() -- parse_string() failed for interpolated section \"%S\"") % ws_in;
        msgend_loc(expression.rparms.values.at(0), expression);
        results = error_results(); 
        return -1;    
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
            count_error(); 
            M_out_emsg1(L"verb_interpolate_1() -- eval_block() failed for interpolated section \"%S\"") % ws_in;
            msgend_loc(expression.rparms.values.at(0), expression);
            results = error_results(); 
            return erc;
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


// ------------------------------------
// outer function for @INTERPOLATE verb
// ------------------------------------

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



//---------------------------------------------------------------------------------
//   verb_format() -- @FORMAT
//---------------------------------------------------------------------------------


// verb_format_l() -- inner function passed in to interpolate_string() to do actual processing for one interpolated section
// ------------------------------------------------------------------------------------------------------------------------

static int verb_format_1(frame_S& frame, const e_expression_S& expression, results_S& results, const std::wstring& ws_in, std::wstring& ws_out, const std::wstring& verb_out, void* in_p) try
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


    // just put out "%", if format spec is just "%%" -- don't pass this through the regex scanners  

    if (ws_fmt == L"%%")                                                                                        
    {   
        ws_out = L"%";                                      // just put out % -- similar to printf() %%
        return 0;                                           // don't need to advance the positional parm                                                    
    }                                                                                                        
   

    // ==============================================================================================================
    // extract variable length and precision ( examples: %*.10X    %10.*X   %0#-*.*X ), if present in the format spec  
    // ==============================================================================================================

    int v_1     { -1 };                                                                                       // -1 indicates that 1st variable is not being used 
    int v_2     { -1 };                                                                                       // -1 indicates that 2nd variable is not being used 
    int v_count {  0 };                                                                                       // number of variable parms in format spec 

    std::wregex  rpatv { LR"(\%([0\-\+\ \#]*)(\*|[[:digit:]]+)?(\.\*|\.[[:digit:]]+)?[acAeEfFgGdiuosxX])" };  // general pattern for a A c e E f F g G d i o u x X format specification -- final verification will take place later
    std::wsmatch mv    { };                                                                                   // match results for std::wstring

    auto foundv { std::regex_match(ws_fmt, mv, rpatv) };                                                      // see if passed-in format specification matches most general valid type


    // if format spec is not valid here, don't bother looking for *.* -- one of the more specific tests down below will fail (and the error will be reported then)

    if (foundv)
    {
        // format spec looks fairly good -- proceed with getting variable width/precision

         M__(M_out(    L"verb_format_1() -- mv.size()             = %d") % mv.size()                  ;) 
       
         for (auto i = 0; i < mv.size(); i++)
         {
             M__(M_out(L"verb_format_1() -- mv.length(%d)      = %d")     % i % mv.length(i)           ;)
             M__(M_out(L"verb_format_1() -- mv.position(%d)    = %d")     % i % mv.position(i)         ;)
             M__(M_out(L"verb_format_1() -- mv.str(%d)         = \"%S\"") % i % mv.str(i)              ;)   
         }

 
         // process variable width, if specified
         // ------------------------------------

         if ( (mv.size() >= 3) && (mv.str(2) == L"*") )                                         // check for variable width
         {
             // make sure needed positional parm exists

             if (*parm_index_p >= parm_count) 
             {
                 count_error(); 
                 M_out_emsg1(L"verb_format_1() -- variable width ('*') in format spec \"%S\"; refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                 msgend_loc(fmt_value, expression);
                 results = error_results();  
                 return -1;              
             }


             // make sure needed positional parm is of integer type  -- don't check values - let swprintf() do it's checking

             auto parm_val  { expression.rparms.values.at(*parm_index_p) } ;
             int  v_width   { -1                                         } ;

             if (!is_value_integer(parm_val))
             {
                 count_error(); 
                 M_out_emsg1(L"verb_format_1() -- variable width ('*') in format spec \"%S\"; refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_val.ty);
                 msgend_loc(fmt_value, expression);
                 results = error_results();  
                 return -1;            
             }
             
             auto gv_rc = get_val(parm_val, v_width);                                           // extract value_S integer type data into C++ int variable, for passing to swprintf() later

             if (gv_rc != 0)
             {
                 count_error(); 
                 M_out_emsg1(L"verb_format_1() -- variable width ('*') in format spec \"%S\"; refers to positional parm %d = %S -- cannot be converted to 31-bit signed integer value") % ws_orig_fmt % *parm_index_p % str_value(parm_val);
                 msgend_loc(fmt_value, expression);
                 results = error_results();  
                 return -1;               
             }
 

             //  valid integer value -- save away for later swprintf() call 

             v_count++;                                                                         // indicate we have at least one variable parm 
             v_1 = v_width;                                                                     // variable width is always 1st  
             (*parm_index_p)++;                                                                 // advance to next positional parm for outer verb 
         }                                                                                      // variable width specified
        

         // process variable precision, if specified
         // ----------------------------------------

         if ( (mv.size() >= 4) && (mv.str(3) == L".*") )                                        // check for variable precision
         {
             // make sure needed positional parm exists

             if (*parm_index_p >= parm_count) 
             {
                 count_error(); 
                 M_out_emsg1(L"verb_format_1() -- variable precision ('.*') in format spec \"%S\"; refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                 msgend_loc(fmt_value, expression);
                 results = error_results();  
                 return -1;              
             }


             // make sure needed positional parm is of integer type  -- don't check values - let swprintf() do it's checking

             auto parm_val      { expression.rparms.values.at(*parm_index_p) } ;
             int  v_precision   { -1                                         } ;

             if (!is_value_integer(parm_val))
             {
                 count_error(); 
                 M_out_emsg1(L"verb_format_1() -- variable precision ('*') in format spec \"%S\"; refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_val.ty);
                 msgend_loc(fmt_value, expression);
                 results = error_results();  
                 return -1;            
             }
             
             auto gv_rc = get_val(parm_val, v_precision);                                       // extract value_S integer type data into C++ int variable, for passing to swprintf() later

             if (gv_rc != 0)
             {
                 count_error(); 
                 M_out_emsg1(L"verb_format_1() -- variable precision ('*') in format spec \"%S\"; refers to positional parm %d = %S -- cannot be converted to 31-bit signed integer value") % ws_orig_fmt % *parm_index_p % str_value(parm_val);
                 msgend_loc(fmt_value, expression);
                 results = error_results();  
                 return -1;               
             }


             //  valid integer value -- save away for later swprintf() call 

             v_count++;                                                                         // indicate we have another variable parm 
                                                                                       
             if (v_count == 1)                                                         
                 v_1 = v_precision;                                                             // no variable width -- variable precision is 1st 
             else                                                                              
                 v_2 = v_precision;                                                             // had variable width -- variable precision is 2nd
                                                                                               
             (*parm_index_p)++;                                                                 // advance to next positional parm for outer verb 
         }                                                                                      // variable precision specified
    }                                                                                           // format spec looks reasonably correct 

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
        // process string type format specification
        // ----------------------------------------  
       
        case    L's' : 
        {
            std::wregex  rpat { LR"(\%([0\-]*)(\*|[[:digit:]]+)?(\.\*|\.[[:digit:]]+)?[s])" };          // pattern for s format specification
            std::wsmatch mr   { };                                                                      // match results for std::wstring

            auto found { std::regex_match(ws_fmt, mr, rpat) };                                          // see if passed-in format sp[ecification matches
           
            if (!found) 
            {
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- string-type format spec \"%S\"; is not valid") % ws_orig_fmt;
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;   
            }  

          
            // check the matching positional input parm to see if it matches the spec  

            if (*parm_index_p >= parm_count) 
            {
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- string-type format spec \"%S\"; refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;              
            } 


            // make sure n-th positional parm is type_E::string

            auto parm_type { expression.rparms.values.at(*parm_index_p).ty } ;

            if (parm_type != type_E::string)
            {
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- string-type format spec \"%S\"; refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_type);
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;             
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

                found = std::regex_match(ws_fmt, mr, rpat);                                                 // see if passed-in format specification matches the required pattern for d i u specs 
            }

            if (!found) 
            {
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- integer-type format spec \"%S\"; is not valid") % ws_orig_fmt;
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;   
            }    
                

            // check the matching positional input parm to see if it matches the spec  

            if (*parm_index_p >= parm_count) 
            {
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- integer-type format spec \"%S\"; refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;              
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
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- integer-type  format spec \"%S\"; refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_type);
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;             
            }  
             

            // special processing for wchar_t to be passed in to swprintf() as an int for %c type formatting
            // ---------------------------------------------------------------------------------------------

            if (last_char == L'c')
            {
                int wc_val { };                                                                                      // local integer variable to be passed to swprintf() for %c formatting 
                                                                                                               
                auto gv_rc = get_val(parm_value, wc_val);                                                            // convert n-th positional parm value to C++ int type, if possible -- complain if error

                if (gv_rc != 0)
                {
                    count_error(); 
                    M_out_emsg1(L"verb_format_1() -- character-type format spec \"%S\"; refers to positional parm %d = %S -- cannot be converted to 31-bit signed integer value") % ws_orig_fmt % *parm_index_p % str_value(parm_value);
                    msgend_loc(fmt_value, expression);
                    results = error_results();  
                    return -1;             
                } 


                // make sure this is a valid UTF-16 code point 
                //
                //   - high or low surrogate allowed -- caller needs to use two adjacent %c specs to format an extended character -- there is no policing of this here 
                //   - also allow plane-0 non-characters u+FFFE, u+FFFF, etc.
            
                if ( (wc_val < 0) || (wc_val > const_N::utf16_plane0_max) )
                {
                    count_error(); 
                    M_out_emsg1(L"verb_format_1() -- character-type format spec \"%S\"; refers to positional parm %d = %d -- UTF-16 code point outside of valid range (u+0000 to u+FFFF)") % ws_orig_fmt % *parm_index_p % wc_val;
                    msgend_loc(fmt_value, expression);
                    results = error_results();  
                    return -1;             
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
                        M_out_emsg1(L"verb_format_1() -- internal error -- unknown integer parm type = \"%S\"") % type_str(parm_type);
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
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- floating point-type format spec \"%S\"; is not valid") % ws_orig_fmt;
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;   
            } 

          
            // check the matching positional input parm to see if it matches the spec  

            if (*parm_index_p >= parm_count) 
            {
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- floating-point-type format spec \"%S\"; refers to non-existent input value -- value needed index = %d   number of input parms = %d") % ws_orig_fmt % *parm_index_p % parm_count;
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;              
            }


            // make sure n-th positional parm is valid floating-point type
            
            auto parm_type { expression.rparms.values.at(*parm_index_p).ty } ;

            if  (
                  (parm_type != type_E::float32 )
                  &&
                  (parm_type != type_E::float64 )
                )
            {
                count_error(); 
                M_out_emsg1(L"verb_format_1() -- floating-point-type  format spec \"%S\"; refers to mismatched positional parm %d of type = \"%S\"") % ws_orig_fmt % *parm_index_p % type_str(parm_type);
                msgend_loc(fmt_value, expression);
                results = error_results();  
                return -1;             
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


        // all other types are not supported -- 'p'  'n' , for example
        // -----------------------------------------------------------

        default : 
        {
            count_error(); 
            M_out_emsg1(L"verb_format_1() -- final character ('%c') in format spec \"%S\"; is unsupported -- must be:  'a'  'A'  'd'  'e'  'E'  'f'  'F'  'g'  'G'  'i'  'o'  's'  'u'  'x'  or  'X'") % ws_orig_fmt.back() % ws_fmt;
            msgend_loc(fmt_value, expression);
            results = error_results();  
            return -1; 
            break;
        }
    }
 

    // should not get here

    return 0; 
}
M_endf




// -------------------------------
// outer function for @FORMAT verb
// -------------------------------

int verb_format(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known there is required   fmt:   kw parm, and optional   begin:  and   end:  keyword parms + likely positional parms (not looked at here, but passed through to called function) 
    M__(M_out(L"verb_format() called");)

    /*
    int8_t    i8  { }; 
    int16_t   i16 { }; 
    int32_t   i32 { }; 
    int64_t   i64 { }; 
    uint8_t   u8  { }; 
    uint16_t  u16 { }; 
    uint32_t  u32 { }; 
    uint64_t  u64 { }; 


    int rc {1};  
    value_S { }; 

    rc = get_val( type_val(( int64_t)111          ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % 111     %  i8  % rc ;
    rc = get_val( type_val(( int64_t)222          ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % 222     %  i8  % rc ;
    rc = get_val( type_val(( int64_t)-111         ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % -111    %  i8  % rc ;
    rc = get_val( type_val(( int64_t)-222         ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % -222    %  i8  % rc ;
    rc = get_val( type_val(( int64_t)33333        ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % 33333   %  i8  % rc ;
    rc = get_val( type_val(( int64_t)-33333       ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % -33333  %  i8  % rc ;
    rc = get_val( type_val(( int64_t)77777        ), i8); M_out(L"u64=%d -> i8=%d   rc=%d") % 77777   %  i8  % rc ;

    rc = get_val( type_val(( int64_t)111          ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % 111     %  u8  % rc ;
    rc = get_val( type_val(( int64_t)222          ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % 222     %  u8  % rc ;
    rc = get_val( type_val(( int64_t)-111         ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % -111    %  u8  % rc ;
    rc = get_val( type_val(( int64_t)-222         ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % -222    %  u8  % rc ;
    rc = get_val( type_val(( int64_t)33333        ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % 33333   %  u8  % rc ;
    rc = get_val( type_val(( int64_t)-33333       ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % -33333  %  u8  % rc ;
    rc = get_val( type_val(( int64_t)77777        ), u8); M_out(L"u64=%d -> u8=%d   rc=%d") % 77777   %  u8  % rc ;
    
    rc = get_val( type_val(( int64_t)111          ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % 111     %  i16  % rc ;
    rc = get_val( type_val(( int64_t)222          ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % 222     %  i16  % rc ;
    rc = get_val( type_val(( int64_t)-111         ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % -111    %  i16  % rc ;
    rc = get_val( type_val(( int64_t)-222         ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % -222    %  i16  % rc ;
    rc = get_val( type_val(( int64_t)33333        ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % 33333   %  i16  % rc ;
    rc = get_val( type_val(( int64_t)-33333       ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % -33333  %  i16  % rc ;
    rc = get_val( type_val(( int64_t)77777        ), i16); M_out(L"u64=%d -> i16=%d   rc=%d") % 77777   %  i16  % rc ;

    rc = get_val( type_val(( int64_t)111          ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % 111     %  u16  % rc ;
    rc = get_val( type_val(( int64_t)222          ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % 222     %  u16  % rc ;
    rc = get_val( type_val(( int64_t)-111         ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % -111    %  u16  % rc ;
    rc = get_val( type_val(( int64_t)-222         ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % -222    %  u16  % rc ;
    rc = get_val( type_val(( int64_t)33333        ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % 33333   %  u16  % rc ;
    rc = get_val( type_val(( int64_t)-33333       ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % -33333  %  u16  % rc ;
    rc = get_val( type_val(( int64_t)77777        ), u16); M_out(L"u64=%d -> u16=%d   rc=%d") % 77777   %  u16  % rc ;


    add_separators_w(-123456789.789);
    add_separators_w( 123456789.789);
    add_separators_w( 123456789.);
    add_separators_w(-123456789.);
    add_separators_w( 123456789.E+66);
    add_separators_w(-123456789.E+66);

    add_separators_w(L"-1"                  ,  L',' , 3);
    add_separators_w(L"-12"                 ,  L',' , 3);
    add_separators_w(L"-123"                ,  L',' , 3);
    add_separators_w(L"-1234"               ,  L',' , 3);
    add_separators_w(L"-12345"              ,  L',' , 3);
    add_separators_w(L"-123456"             ,  L',' , 3);
    add_separators_w(L"-1234567"            ,  L',' , 3);
    add_separators_w(L"-12345678"           ,  L',' , 3);
    add_separators_w(L"-123456789"          ,  L',' , 3);
    add_separators_w(L"-1234567890"         ,  L',' , 3);
    add_separators_w(L"1"                   ,  L',' , 3);
    add_separators_w(L"12"                  ,  L',' , 3);
    add_separators_w(L"123"                 ,  L',' , 3);
    add_separators_w(L"1234"                ,  L',' , 3);
    add_separators_w(L"12345"               ,  L',' , 3);
    add_separators_w(L"123456"              ,  L',' , 3);
    add_separators_w(L"1234567"             ,  L',' , 3);
    add_separators_w(L"12345678"            ,  L',' , 3);
    add_separators_w(L"123456789"           ,  L',' , 3);
    add_separators_w(L"1234567890"          ,  L',' , 3);


    add_separators_w(L" 123456789");
    add_separators_w(L"-123456789");
    add_separators_w(L"+123456789");
    add_separators_w( L"123456789");

    add_separators_w(L"-123456789.789");
    add_separators_w(L"+123456789.789");
    add_separators_w(L" 123456789.789");
    add_separators_w( L"123456789.789");

    add_separators_w(L" 123456789.");
    add_separators_w(L"-123456789.");
    add_separators_w(L"+123456789.");
    add_separators_w( L"123456789.");

    add_separators_w(L" .123456789");
    add_separators_w(L"-.123456789");
    add_separators_w(L"+.123456789");
    add_separators_w( L".123456789");


    add_separators_w(L" 123456789E+66");
    add_separators_w(L"-123456789e+66");
    add_separators_w(L"+123456789E+66");
    add_separators_w( L"123456789e+66");

    add_separators_w(L" 123456789.E+66");
    add_separators_w(L"-123456789.e+66");
    add_separators_w(L"+123456789.E+66");
    add_separators_w( L"123456789.e+66");

    add_separators_w(L" .123456789E+66");
    add_separators_w(L"-.123456789e+66");
    add_separators_w(L"+.123456789E+66");
    add_separators_w( L".123456789e+66");
    return 0; 
    */

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
    iparm.ws_begin     =      L"%";                                     // format spec start s with %
    iparm.ws_end       =      L"%aAcdeEfFgGiosuxX";                     // format spec ends with any one of these characters
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @DISPLAY 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_display(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_display() called");)


    // get locale: and numerics: keywords (if present)  

    auto locale_rc           = get_right_keyword(expression, L"locale"           );     // r/c = 0, if locale:             keyword is present 
    auto numerics_rc         = get_right_keyword(expression, L"numerics"         );     // r/c = 0, if numerics:           keyword is present
    auto stack_rc            = get_right_keyword(expression, L"stack"            );     // r/c = 0, if stack:              keyword is present
    auto all_vars_rc         = get_right_keyword(expression, L"all_vars"         );     // r/c = 0, if all_vars:           keyword is present
    auto builtin_verbs_rc    = get_right_keyword(expression, L"builtin_verbs"    );     // r/c = 0, if builtin_verbs:      keyword is present
    auto added_verbs_rc      = get_right_keyword(expression, L"added_verbs"      );     // r/c = 0, if added_verbs:        keyword is present
    auto all_verbs_rc        = get_right_keyword(expression, L"all_verbs"        );     // r/c = 0, if all_verbs:          keyword is present
    auto builtin_types_rc    = get_right_keyword(expression, L"builtin_types"    );     // r/c = 0, if builtin_types:      keyword is present
    auto added_types_rc      = get_right_keyword(expression, L"added_types"      );     // r/c = 0, if added_types:        keyword is present
    auto all_types_rc        = get_right_keyword(expression, L"all_types"        );     // r/c = 0, if all_types:          keyword is present
    auto id_cache_rc         = get_right_keyword(expression, L"id_cache"         );     // r/c = 0, if id_cache:           keyword is present
    auto statistics_rc       = get_right_keyword(expression, L"statistics"       );     // r/c = 0, if statistics:         keyword is present
                                                                             
    auto vars_rc             = get_right_keyword(expression, L"vars"             );     // r/c = 0, if vars:[ ... ]        keyword is present
    auto verbs_rc            = get_right_keyword(expression, L"verbs"            );     // r/c = 0, if verbs:[ ... ]       keyword is present
    auto types_rc            = get_right_keyword(expression, L"types"            );     // r/c = 0, if types:[ ... ]       keyword is present

  
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