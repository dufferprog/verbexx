// ex_environ.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ==============
////     ex_environ.cpp -- environment/stack frame oriented functions
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
#include "h_ex_parse.h"
#include "h_ex_verb.h"      


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  forward declarations for selected static functions 
//  --------------------------------------------------

static int          get_verb(                  environ_S& , const std::wstring&, verbset_S&               );
static int          get_identifier(            environ_S& , const std::wstring&, symval_S&                );



// local static variables
// ----------------------

namespace static_N
{
    uint64_t symval_ctr {0};                 // used for assigning symval_S serial numbers
}







//╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║                                          IIIIIIIIII        DDDDDDDD          EEEEEEEEEE        NN      NN       TTTTTTTTT
//║╳╳╳╳║                                          IIIIIIIIII        DDDDDDDDD         EEEEEEEEEE        NNN     NN       TTTTTTTTT
//║╳╳╳╳║                                              II            DD      DD        EE                NNNN    NN           TT   
//║╳╳╳╳║                                              II            DD      DD        EE                NN NN   NN           TT   
//║╳╳╳╳║                                              II            DD      DD        EEEEEEEE          NN  NN  NN           TT   
//║╳╳╳╳║                                              II            DD      DD        EEEEEEEE          NN   NN NN           TT   
//║╳╳╳╳║                                              II            DD      DD        EE                NN    NNNN           TT   
//║╳╳╳╳║                                              II            DD      DD        EE                NN     NNN           TT   
//║╳╳╳╳║                                          IIIIIIIIII        DDDDDDDDD         EEEEEEEEEE        NN      NN           TT   
//║╳╳╳╳║                                          IIIIIIIIII        DDDDDDDD          EEEEEEEEEE        NN      NN           TT        
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳║
//║╳╳╳╳╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   is_ident_sf_shadowed() -- determine if input identifier name would be shadowed in passed-in stack frame (internal function)  
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

bool is_ident_sf_shadowed(frame_S *target_frame_p, const std::wstring& ident) try
{
#ifdef M_EXPOSE_SUPPORT
        bool exposed_only { false };                                                   // true means variable is found only if is_exposed flag is on
#endif
                                                                                      
    M__(M_out(L"is_ident_sf_shadowed() -- called -- target_frame_p = %X") % target_frame_p;)      
 
    bool     is_shadowed  { true                       };                               // output R/C -- assume target_farme is shadowed, until it is reached in the identifier search                                                                                    
    frame_S *frame_p      {get_newest_sf()->search_sf_p};                               // point to the search starting SF for the newest SF  
                                                                                      
    
    // search nested non-global stack frames for requested identifier                 
    // -------------------------------------------------------------- 

    for(;;)                                                                             // main loop to look at all chained stack frames in the scope chain starting at passed-in frame_S
    {   
        // indicate target_sf is not shadowed, if search for the identifier has now reached the target_sf

        if (frame_p == target_frame_p)
            is_shadowed = false; 


        // return with boolean results, if identifier is found in this SF
        
        if ((frame_p->environ_p->symbols).count(ident) > 0)                             // identifier found in environment?
        {  
#ifdef M_EXPOSE_SUPPORT
             if (                                                                     
                 (!exposed_only)                                                        // all variables in this environment are visible to caller's frame_S ?   (i.e. this is local environ)  -or- lexical_scope, etc.
                 ||                                                                   
                 ((frame_p->environ_p->symbols).at(ident).is_exposed)                   // is this variable exposed ? 
                )
#endif
             {    
                 // this identifier is visible to passed-in stack frame -- return with shadowing results so far

                 return is_shadowed; 
             }
        }    
   

        // end loop after main frame_S has been searched or end of scope chain is reached  (don't look at global environ_S) 

        if ( (frame_p->is_main) || (frame_p->scope_sp.use_count() == 0) )
            break;

#ifdef M_EXPOSE_SUPPORT
        // if this is an frame_S using dynamic upward scoping (created by only verb evaluation), set exposed-only flag so that only exposed symbols are found from now on (except in global environment, whch are always exposed, anyway)  
        // (for lexical or dynall scoping, all upward symbols are visible)

        if (frame_p->dynamic_scope)
            exposed_only = true;
#endif

        // advance to next search-starting SF frame_S (using upwards scope pointer)

       frame_p = frame_p->scope_sp->search_sf_p; 
    }


    // identifier is not defined in non-global environments -- target SF is not shadowed for this identifier 
    // -----------------------------------------------------------------------------------------------------

    return false; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   is_global_ident_shadowed() -- determine if input identifier name would be shadowed in the global environment  
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

bool is_global_ident_shadowed(const std::wstring& ident) try
{
#ifdef M_EXPOSE_SUPPORT
        bool exposed_only { false };                                                   // true means variable is found only if is_exposed flag is on
#endif
                                                                                      
    M__(M_out(L"is_global_ident_sf_shadowed() -- called");)      
                                                                        
    frame_S *frame_p  {get_newest_sf()->search_sf_p};                                  // point to the search starting SF for the newest SF  
                                                                                      
    
    // search nested non-global stack frames for requested identifier                 
    // -------------------------------------------------------------- 

    for(;;)                                                                             // main loop to look at all chained stack frames in the scope chain starting at passed-in frame_S
    {   
        // indicate identifier is shadowed, if identifier is found in this SF
        
        if ((frame_p->environ_p->symbols).count(ident) > 0)                             // identifier found in environment?
        {  
#ifdef M_EXPOSE_SUPPORT
             if (                                                                     
                 (!exposed_only)                                                        // all variables in this environment are visible to caller's frame_S ?   (i.e. this is local environ)  -or- lexical_scope, etc.
                 ||                                                                   
                 ((frame_p->environ_p->symbols).at(ident).is_exposed)                   // is this variable exposed ? 
                )
#endif
             {    
                 // this identifier is visible to passed-in stack frame -- any global identifier would be shadowed

                 return true; 
             }
        }    
   

        // end loop after main frame_S has been searched or end of scope chain is reached  (don't look at global environ_S) 

        if ( (frame_p->is_main) || (frame_p->scope_sp.use_count() == 0) )
            break;

#ifdef M_EXPOSE_SUPPORT
        // if this is an frame_S using dynamic upward scoping (created by only verb evaluation), set exposed-only flag so that only exposed symbols are found from now on (except in global environment, whch are always exposed, anyway)  
        // (for lexical or dynall scoping, all upward symbols are visible)

        if (frame_p->dynamic_scope)
            exposed_only = true;
#endif

        // advance to next search-starting SF frame_S (using upwards scope pointer)

       frame_p = frame_p->scope_sp->search_sf_p; 
    }


    // identifier is not defined in non-global environments -- glopbal identifier is not shadowed 
    // ------------------------------------------------------------------------------------------

    return false; 
}
M_endf






////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   find_environ_ident() -- internal function -- find environ_S containing requested identifier (returns pointer to environment) 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int find_environ_ident(const frame_S& frame, const std::wstring& ident, environ_S *& environ_p) try
{
#ifdef M_EXPOSE_SUPPORT
    bool exposed_only { false };                                                       // true means variable is found only if is_exposed flag is on
#endif
                                                                                      
    M__(M_out(L"find_environ_ident() 1 -- called -- &frame = %X") % &frame;)          
                                                                                      
                                                                                      
    // search nested non-global stack frames for requested identifier                 
    // --------------------------------------------------------------                 
                                                                                      
    frame_S *frame_p  {frame.search_sf_p};                                              // point to the search starting SF for the passed-in frame_S  
                                                                                      
                                                                                      
    for(;;)                                                                             // main loop to look at all chained stack frames in the scope chain starting at passed-in frame_S
    {                                                                                 
        if ((frame_p->environ_p->symbols).count(ident) > 0)                             // identifier found in environment?
        {  
#ifdef M_EXPOSE_SUPPORT
             if (                                                                     
                 (!exposed_only)                                                       // all variables in this environment are visible to caller's frame_S ?   (i.e. this is local environ)  -or- lexical_scope, etc.
                 ||                                                                   
                 ((frame_p->environ_p->symbols).at(ident).is_exposed)                   // is this variable exposed ? 
                )
#endif
             {    
                 // this identifier is visible to passed-in stack frame

                 environ_p = frame_p->environ_p;                                        // pass back pointer to located environment
                 return 0; 
             }
        }    
    

        // end loop after main frame_S has been searched or end of scope chain is reached  (don't look at global environ_S in this loop -- see below) 

        if ( (frame_p->is_main) || (frame_p->scope_sp.use_count() == 0) )
            break;

#ifdef M_EXPOSE_SUPPORT
        // if this is an frame_S using dynamic upward scoping (created by only verb evaluation), set exposed-only flag so that only exposed symbols are found from now on (except in global environment, whch are always exposed, anyway)  
        // (for lexical or dynall scoping, all upward symbols are visible)

        if (frame_p->dynamic_scope)
            exposed_only = true;
#endif

        // advance to next search-starting SF frame_S (using upwards scope pointer)

       frame_p = frame_p->scope_sp->search_sf_p; 
    }


    // Look for identifier in global environ_S 
    // ---------------------------------------
    //
    // control gets here when all nested frame_S environments have been searched, and identifier is not found
    // see if variable is defined in global environment

    if ((get_global_environ()->symbols).count(ident) > 0)                           // identifier in global table?   (global variables are always exposed)
    {                                                                             
        environ_p = get_global_environ();                                           // pass back pointer to global environment
        return 0;                                                                 
    }                                                                             
    else                                                                            // identifier not defined in any local or global environment
    {                                                                             
        environ_p = nullptr;                                                        // pass back no pointer
        return -1;                                                                  // indicate failure
    } 
}
M_endf    




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   def_local_identifier()      -- sets value of passed-in identifier -- defines new variable/const/verb/typedef in local     environment 
////   def_verbmain_identifier()   -- sets value of passed-in identifier -- defines new variable/const/verb/typedef in verbmain  environment 
////   def_static_identifier()     -- sets value of passed-in identifier -- defines new variable/const/verb/typedef in static    environment 
////   def_global_identifier()     -- sets value of passed-in identifier -- defines new variable/const/verb/typedef in global    environment 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int def_identifier(environ_S& env, const std::wstring& ident, symval_S& symval, bool unshare) try
{
    M__(M_out(L"def_identifier() -- passed-in name = %s") % ident;)


    // error if this identifier already exists (in chosen local environment or global environment)

    auto ct = env.symbols.count(ident);

    if (ct > 0)
    {
        count_error(); 
        M_out_emsg(L"def_identifier() -- identifier = \"%s\" is already defined -- cannot be redefined") % ident; 
        return -1;      
    }


    // make sure any nested objects are unshared, if requested by caller
  
    if (unshare)                             
        unshare_value(*(symval.value_sp)); 


    // add in new identifier to environment

    symval.serno = ++(static_N::symval_ctr);
    auto rc = env.symbols.emplace(ident, symval);     

    if (!rc.second)
        M_throw_v("def_identifier() -- unexpected environ_p->symbols.emplace(%s, symval) failure") % out_ws(ident) ));

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_identifier(frame_S& frame, const std::wstring& ident, symval_S& symval, bool unshare) try
{  
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_local_identifier() -- current stack frame has no associated local environment -- identifier = \"%S\" cannot be defined as local: for this stack frame") % ident;     
        return -1;  
    }


    // make sure this identifier is not shadowed in any environment newer than the local environment (saved invocation parms)

    if (is_ident_sf_shadowed(frame.local_sf_p, ident))
    {                                                                          
        count_error(); 
        M_out_emsg(L"def_local_identifier() -- local identifier = \"%S\" would be shadowed by saved verb invocation parms -- it cannot be defined as a local identifier from this stack frame") % ident; 
        return -1;       
    }

    return def_identifier(*(frame.local_sf_p->environ_p), ident, symval, unshare); 
}
M_endf

///////////////////////////////////////////////////////////////////////////////////////////////

int def_verbmain_identifier(frame_S& frame, const std::wstring& ident, symval_S& symval, bool unshare) try
{  
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_verbmain_identifier() -- current stack frame has no associated verbmain environment -- identifier = \"%S\" cannot be defined as verbmain: for this stack frame") % ident;     
        return -1;  
    }


    // make sure this identifier is not shadowed in any environment newer than the verbmain environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, ident))
    {                                                                         
        count_error(); 
        M_out_emsg(L"def_verbmain_identifier() -- verbmain identifier = \"%S\" would be shadowed in a local environment -- it cannot be defined as a verbmain identifier from this stack frame") % ident; 
        return -1;       
    }

    return def_identifier(*(frame.verbmain_sf_p->environ_p), ident, symval, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_static_identifier(frame_S& frame, const std::wstring& ident, symval_S& symval, bool unshare) try
{  
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_static_identifier() -- current stack frame has no associated static environment -- identifier = \"%S\" cannot be defined as static: for this stack frame") % ident;     
        return -1;  
    }

    
    // make sure this identifier is not shadowed in an environment newer than the static environment
    
    if (is_ident_sf_shadowed(frame.persist_sf_p, ident))         
    {
        count_error(); 
        M_out_emsg(L"def_static_identifierr() -- static identifier = \"%S\" would be shadowed in a local (non-static) environment -- it cannot be defined as a static identifier from this stack frame") % ident; 
        return -1;       
    }

    return def_identifier(*(frame.persist_sf_p->environ_p), ident, symval, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_identifier(const std::wstring& ident, symval_S& symval, bool unshare) try
{
    // make sure this identifier is not shadowed in an environment newer than the global environment

    if (is_global_ident_shadowed(ident))         
    {
        count_error(); 
        M_out_emsg(L"def_global_identifier() -- global identifier = \"%S\" would be shadowed in a non-global environment -- it cannot be defined as a global identifier from this stack frame") % ident; 
        return -1;       
    }


    // no shadowing -- go ahead and define identifier, if not already defined at the global level

    return def_identifier(*get_global_environ(), ident, symval, unshare); 
}
M_endf 


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   make_identifier_const()   -- marks this identifier as constant 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int make_identifier_const(environ_S& env, const std::wstring& ident) try
{
    // see if identifier already exists, and is constant

    auto ct = env.symbols.count(ident);

    if (ct > 0)
    {
        if (env.symbols.at(ident).is_verbset)
        {
            count_error();  
            M_out_emsg(L"make_identifier_const() -- identifier \"%s\" is a verb -- cannot make ito a constant") % ident;
            return -1; 
        }
        else if (env.symbols.at(ident).is_typdef)
        {
            count_error();
            M_out_emsg(L"make_identifier_const() -- identifier \"%s\" is a typdef -- cannot make ito a constant") % ident;
            return -1; 
        }
        else
        {
            env.symbols.at(ident).is_const = true;
            return 0;
        }
    }
    else
    {
        M_out_emsg(L"make_identifier_const() -- identifier \"%s\" was not found") % ident; 
        return -1;
    }
}
M_endf  


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   undef_local_identifier()      -- removes value of passed-in identifier -- undefines variable/const/verb in local    environment 
////   undef_verbmain_identifier()   -- removes value of passed-in identifier -- undefines variable/const/verb in verbmain environment 
////   undef_static_identifier()     -- removes value of passed-in identifier -- undefines variable/const/verb in static   environment 
////   undef_global_identifier()     -- removes value of passed-in identifier -- undefines variable/const/verb in global   environment 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int undef_identifier(environ_S& env, const std::wstring& ident) try
{
    // see if identifier already exists, and is undefinable

    auto ct = env.symbols.count(ident);

    if (ct > 0)
    {
       if (env.symbols.at(ident).no_undefine)     
       {
           if (env.symbols.at(ident).is_const)
           {
               count_error(); 
               M_out_emsg(L"undef_identifier() -- cannot remove constant \"%s\"") % ident; 
               return -1;
           }
         
           if (env.symbols.at(ident).is_builtin) 
           {
               count_error(); 
               M_out_emsg(L"undef_identifier() -- cannot remove built-in definition \"%s\"") % ident; 
               return -1;
           }

           if (env.symbols.at(ident).is_verbset) 
           {
               count_error(); 
               M_out_emsg(L"undef_identifier() -- cannot remove verbset definition \"%s\"") % ident; 
               return -1;
           }

           if (env.symbols.at(ident).is_typdef) 
           {
               count_error(); 
               M_out_emsg(L"undef_identifier() -- cannot remove typdef definition \"%s\"") % ident; 
               return -1;
           } 
           
           count_error(); 
           M_out_emsg(L"undef_identifier() -- identifier \"%s\" is flagged as not undefinable -- cannot be undefined") % ident; 
           return -1;
       }

       env.symbols.erase(ident);                         // get rid of old value 
    }

    return 0;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////

int undef_local_identifier(frame_S& frame, const std::wstring& ident) try
{  
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_local_identifier() -- current stack frame has no associated local environment -- local identifier = \"%S\" cannot be undefined for this stack frame") % ident;     
        return -1;  
    }

#if 0     
    // make sure this identifier is not shadowed in an environment newer than the local environment

    if (is_ident_sf_shadowed(frame.local_sf_p, ident))      
    {                                                           
        count_error(); 
        M_out_emsg(L"undef_local_identifier() -- local identifier = \"%S\" to be undefined is shadowed in a newer environment -- it cannot be undefined from this stack frame") % ident; 
        return -1;       
    }
#endif   
   
    return undef_identifier(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int undef_verbmain_identifier(frame_S& frame, const std::wstring& ident) try
{  
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_verbmain_identifier() -- current stack frame has no associated verbmain environment -- verbmain identifier = \"%S\" cannot be undefined for this stack frame") % ident;     
        return -1;  
    }
   
#if 0     
    // make sure this identifier is not shadowed in an environment newer than the verbmain environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, ident))      
    {                                                           
        count_error(); 
        M_out_emsg(L"undef_verbmain_identifier() -- verbmain identifier = \"%S\" to be undefined is shadowed in a local environment -- it cannot be undefined from this stack frame") % ident; 
        return -1;       
    }
#endif   

    return undef_identifier(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int undef_static_identifier(frame_S& frame, const std::wstring& ident) try
{  
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_static_identifier() -- current stack frame has no associated static environment -- static identifier = \"%S\" cannot be undefined for this stack frame") % ident;     
        return -1;  
    }
   
#if 0     
    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, ident))      
    {                                                           
        count_error(); 
        M_out_emsg(L"undef_static_identifier() -- static identifier = \"%S\" to be undefined is shadowed in a local (non-static) environment -- it cannot be undefined from this stack frame") % ident; 
        return -1;       
    }
#endif   

    return undef_identifier(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int undef_global_identifier(const std::wstring& ident) try
{
#if 0     
    // make sure this identifier is not shadowed in an environment newer than the global environment

    if (is_global_ident_shadowed(ident))      
    {                                                          
        count_error(); 
        M_out_emsg(L"undef_global_identifier() -- global identifier = \"%S\" to be undefined is shadowed in a non-global environment -- it cannot be undefined from this stack frame") % ident; 
        return -1;       
    }
#endif 

    return undef_identifier(*get_global_environ(), ident); 
}
M_endf 


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////          is_local_identifier_xxxx()     --  return flag settings for global   identifier
////          is_verbmain_identifier_xxxx()  --  return flag settings for verbmain identifier
////          is_static_identifier_xxxx()    --  return flag settings for static   identifier
////          is_global_identifier_xxxx()    --  return flag settings for local    identifier
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static bool is_identifier_defined(environ_S& env, const std::wstring& ident) try
{
    auto ct = env.symbols.count(ident); 

    if (ct > 1)                                     // something wrong -- should never be more than 1 identifier in environment
        M_throw_v("is_identifier_defined() --  %d entries in environment were found for identifier %s") % ct % out_ws(ident) )); 
    
    if (env.symbols.count(ident) <= 0)             // see if identifier is currently defined 
        return false;
    else
        return true;  
}
M_endf


////////////////////////////////////////////////////////////////////

static bool is_identifier_variable(environ_S& env, const std::wstring& ident) try
{
    if (!is_identifier_defined(env, ident))          
        return false;
    else
    {     
        if  (
             (env.symbols.at(ident).is_verbset )
             ||
             (env.symbols.at(ident).is_typdef  )
             ||
             (env.symbols.at(ident).is_const   )
            )
           return false; 
        else
            return true; 
    }
}
M_endf

//////////////////////////////////////////////////////////

static bool is_identifier_const(environ_S& env, const std::wstring& ident) try
{
    if (!is_identifier_defined(env, ident)) 
        return false;
    else
        return env.symbols.at(ident).is_const;  
}
M_endf

//////////////////////////////////////////////////////////

static bool is_identifier_verb(environ_S& env, const std::wstring& ident) try
{
    if (!is_identifier_defined(env, ident)) 
        return false;
    else
        return env.symbols.at(ident).is_verbset;  
}
M_endf

//////////////////////////////////////////////////////////

static bool is_identifier_typdef(environ_S& env, const std::wstring& ident) try
{
    if (!is_identifier_defined(env, ident)) 
        return false;
    else
        return env.symbols.at(ident).is_typdef;  
}
M_endf

//////////////////////////////////////////////////////////

static bool is_identifier_builtin(environ_S& env, const std::wstring& ident) try
{
    if (!is_identifier_defined(env, ident))
        return false;
    else
        return env.symbols.at(ident).is_builtin; 
}
M_endf


//////////////////////////////////////////////////////////

static bool is_identifier_alias(environ_S& env, const std::wstring& ident) try
{
    if (!is_identifier_defined(env, ident))
        return false;
    else
        return env.symbols.at(ident).is_alias;  
}
M_endf


////////////////////////////////////////////////////////////////////

static bool is_identifier_mutable(environ_S& env, const std::wstring& ident) try
{
    if (!is_identifier_defined(env, ident))
    {
        return false;
    }
    else
    {   
        // can't be updated if identifier itself is not mutable

        symval_S symval = env.symbols.at(ident);

        if (symval.no_update)
           return false; 
     

        // also can't be updated if this is a verbset that has one or more built-in verbs

        if (symval.is_verbset)
        {
            if (symval.value_sp->verbset_sp.get() != nullptr)               // should not be nullptr
            {
                if (symval.value_sp->verbset_sp->has_builtin == true)
                    return false;           
            }        
        }
        

        // identifier must be mutable

        return true; 
    }
}
M_endf 


////////////////////////////////////////////////////////////////////

static bool is_identifier_removable(environ_S& env, const std::wstring& ident) try
{
    if (!is_identifier_defined(env, ident))
    {
        return false;
    }
    else
    {     
        // can't be removed if identifier itself is not undefinable

        symval_S symval = env.symbols.at(ident);

        if (symval.no_undefine)
           return false; 


        // also can't be removed if this is a verbset that has one or more built-in verbs

        if (symval.is_verbset)
        {
            if (symval.value_sp->verbset_sp.get() != nullptr)             // should not be nullptr
            {
                if (symval.value_sp->verbset_sp->has_builtin == true)
                    return false;           
            }        
        }


        // identifier must be removable

        return true; 
    }
}
M_endf 

                                   

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_defined(const frame_S& frame, const std::wstring& ident) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any environment
         return false;                                              // failure r/c
    else                                                            // environment found with identifier
        return true;                                                // proper environment found -- must be defined
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
bool is_identifier_variable(const frame_S& frame, const std::wstring& ident) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any environment
         return false;                                              // failure r/c
    else                                                            // environment found with identifier
        return is_identifier_variable(*p, ident);                   // proper environment found -- check for non-constant variable
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool is_identifier_const(const frame_S& frame, const std::wstring& ident) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any environment
         return false;                                              // failure r/c
    else                                                            // environment found with identifier
        return is_identifier_const(*p, ident);                      // proper environment found -- check constness
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_verb(const frame_S& frame, const std::wstring& ident) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any environment
         return false;                                              // failure r/c
    else                                                            // environment found with identifier
        return is_identifier_verb(*p, ident);                       // proper environment found -- check for verb
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_typdef(const frame_S& frame, const std::wstring& ident) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any environment
         return false;                                              // failure r/c
    else                                                            // environment found with identifier
        return is_identifier_typdef(*p, ident);                     // proper environment found -- check for typdef
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_builtin(const frame_S& frame, const std::wstring& ident) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any environment
         return false;                                              // failure r/c
    else                                                            // environment found with identifier
        return is_identifier_builtin(*p, ident);                    // proper environment found -- check for verb
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_alias(const frame_S& frame, const std::wstring& ident) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any environment
         return false;                                              // failure r/c
    else                                                            // environment found with identifier
        return is_identifier_alias(*p, ident);                      // proper environment found -- check for verb
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_mutable(const frame_S& frame, const std::wstring& ident) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any environment
         return false;                                              // failure r/c
    else                                                            // environment found with identifier
        return is_identifier_mutable(*p, ident);                    // proper environment found -- check for mutability
}
M_endf




////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool is_global_identifier_defined(   const std::wstring& ident) try   { return is_identifier_defined(   *get_global_environ()  , ident); } M_endf
bool is_global_identifier_variable(  const std::wstring& ident) try   { return is_identifier_variable(  *get_global_environ()  , ident); } M_endf
bool is_global_identifier_const(     const std::wstring& ident) try   { return is_identifier_const(     *get_global_environ()  , ident); } M_endf
bool is_global_identifier_verb(      const std::wstring& ident) try   { return is_identifier_verb(      *get_global_environ()  , ident); } M_endf
bool is_global_identifier_typdef(    const std::wstring& ident) try   { return is_identifier_typdef(    *get_global_environ()  , ident); } M_endf
bool is_global_identifier_builtin(   const std::wstring& ident) try   { return is_identifier_builtin(   *get_global_environ()  , ident); } M_endf
bool is_global_identifier_alias(     const std::wstring& ident) try   { return is_identifier_alias(     *get_global_environ()  , ident); } M_endf       
bool is_global_identifier_mutable(   const std::wstring& ident) try   { return is_identifier_mutable(   *get_global_environ()  , ident); } M_endf
bool is_global_identifier_removable( const std::wstring& ident) try   { return is_identifier_removable( *get_global_environ()  , ident); } M_endf


////////////////////////////////////////////////////////////////////////

bool is_local_identifier_defined(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_defined(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


bool is_local_identifier_variable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_variable(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


bool is_local_identifier_const(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_const(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


bool is_local_identifier_verb(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_verb(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


bool is_local_identifier_typdef(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_typdef(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


bool is_local_identifier_builtin(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_builtin(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


bool is_local_identifier_alias(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_alias(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


bool is_local_identifier_mutable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_mutable(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


bool is_local_identifier_removable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.local_sf_p == nullptr)
        return false;
    else
        return is_identifier_removable(*(frame.local_sf_p->environ_p), ident); 
}
M_endf


////////////////////////////////////////////////////////////////////////

bool is_verbmain_identifier_defined(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_defined(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


bool is_verbmain_identifier_variable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_variable(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


bool is_verbmain_identifier_const(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_const(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


bool is_verbmain_identifier_verb(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_verb(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


bool is_verbmain_identifier_typdef(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_typdef(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


bool is_verbmain_identifier_builtin(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_builtin(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


bool is_verbmain_identifier_alias(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_alias(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


bool is_verbmain_identifier_mutable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_mutable(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


bool is_verbmain_identifier_removable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.verbmain_sf_p == nullptr)
        return false;
    else
        return is_identifier_removable(*(frame.verbmain_sf_p->environ_p), ident); 
}
M_endf


////////////////////////////////////////////

bool is_static_identifier_defined(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_defined(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


bool is_static_identifier_variable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_variable(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


bool is_static_identifier_const(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_const(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


bool is_static_identifier_verb(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_verb(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


bool is_static_identifier_typdef(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_typdef(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


bool is_static_identifier_builtin(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_builtin(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


bool is_static_identifier_alias(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_alias(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


bool is_static_identifier_mutable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_mutable(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


bool is_static_identifier_removable(const frame_S& frame, const std::wstring& ident) try  
{
    if (frame.persist_sf_p == nullptr)
        return false;
    else
        return is_identifier_removable(*(frame.persist_sf_p->environ_p), ident); 
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_identifier()           -- gets value of passed-in identifier (-1 if not in local or global environment, or is not visible in other environments) 
////   get_local_identifier()     -- gets value of passed-in identifier (-1 if not in local     environment)
////   get_verbmain_identifier()  -- gets value of passed-in identifier (-1 if not in verbmain  environment)
////   get_static_identifier()    -- gets value of passed-in identifier (-1 if not in static    environment)
////   get_global_identifier()    -- gets value of passed-in identifier (-1 if not in global    environment)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// internal function -- version that returns identifier value from passed-in environment (UNIT, if not found)

static int get_identifier(environ_S& env, const std::wstring& ident, value_S& value) try
{
    if (!is_identifier_defined(env, ident))                 // return with R/C = -1 and unit value, if identifier is not there 
    {
        value = unit_val(); 
        return -1;
    }
    else                                                    // identifier is found -- might be verb or variable/constant
    {
        // identifier is defined -- return associated value

        value = *(env.symbols.at(ident).value_sp);          // value found -- pass it back
        return 0; 
    }
}
M_endf


// internal function -- version that returns complete symval entry (or empty symval, if not found)

static int get_identifier(environ_S& env, const std::wstring& ident, symval_S& symval) try
{
    if (!is_identifier_defined(env, ident))                 // return with R/C = -1 and empty value, if identifier is not there 
    {
        symval = symval_S {}; 
        return -1;
    }
    else                                                    // identifier is found -- might be verb or variable/constant
    {
        // identifier is defined -- return associated value

        symval = env.symbols.at(ident);                     // value found -- pass it back
        return 0; 
    }
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// looks for this identifier in local environment

int get_local_identifier(const frame_S& frame, const std::wstring& ident, value_S& value) try
{
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_local_identifier() -- current stack frame has no associated local environment -- local identifier = \"%S\" cannot be found for this stack frame") % ident;     
        return -1;  
    }

    return get_identifier(*(frame.local_sf_p->environ_p), ident, value);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// looks for this identifier in verbmain environment

int get_verbmain_identifier(const frame_S& frame, const std::wstring& ident, value_S& value) try
{
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_verbmain_identifier() -- current stack frame has no associated verbmain environment -- verbmain identifier = \"%S\" cannot be found for this stack frame") % ident;     
        return -1;  
    }

    return get_identifier(*(frame.verbmain_sf_p->environ_p), ident, value);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// looks for this identifier in static environment associated with passed-in environment

int get_static_identifier(const frame_S& frame, const std::wstring& ident, value_S& value) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_static_identifier() -- current stack frame has no associated static environment -- static identifier = \"%S\" cannot be found for this stack frame") % ident;     
        return -1;  
    }
   
    return get_identifier(*(frame.persist_sf_p->environ_p), ident, value);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// looks for this identifier only in the global environment

int get_global_identifier(const std::wstring& ident, value_S& value) try
{
    return get_identifier(*get_global_environ(), ident, value);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

// searches upward scope chain for 1st occurrence of this identifier   

int get_identifier(const frame_S& frame, const std::wstring& ident, value_S& value) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                                                 // variable not found in any environment
    {                                                          
        value = unit_val();                                      // pass back unit value
        return -1;                                               // failure r/c
    }                                                          
    else                                                         // environment found with identifier
    {
        return get_identifier(*p, ident, value);    
    }
}
M_endf    


#ifdef M_EXPOSE_SUPPORT
///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   expose_local_identifier()     -- alter exposed status of local     identifier 
////   expose_verbmain_identifier()  -- alter exposed status of verbmain  identifier 
////   expose_static_identifier()    -- alter exposed status of static    identifier 
////                                    (global identifiers are always exposed and can't be unexposed)
////              
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int expose_identifier(environ_S& env, const std::wstring& ident, bool is_exposed) try
{
    // error if identifier is not already in the environment

    auto ct = env.symbols.count(ident);

    if (!is_identifier_defined(env, ident))
    {
        count_error(); 
        M_out_emsg(L"expose_identifier() -- identifier = \"%s\" to be exposed/unexposed is not defined") % ident; 
        return -1;  
    }


    // set is_exposed flag in identifier in environment -- do in-place update

    env.symbols.at(ident).is_exposed = is_exposed;              // update is_exposed flag in symval_S for identifier 

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int expose_local_identifier(frame_S& frame, const std::wstring& ident,  bool is_exposed) try
{  
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"expose_local_identifier() -- current stack frame has no associated local environment -- expose/unexpose status of identifier = \"%S\" cannot be changed in this stack frame") % ident;     
        return -1;  
    }

 #if 0     
    // make sure this identifier is not shadowed in an environment newer than the local environment

    if (is_ident_sf_shadowed(frame.local_sf_p, ident))      
    {                                                          
        count_error(); 
        M_out_emsg(L"expose_local_identifier() -- local identifier = \"%S\" to be exposed/unexposed is shadowed in a newer environment -- it cannot be exposed/unexposed from this stack frame") % ident; 
        return -1;       
    }
#endif 


    return expose_identifier(*(frame.local_sf_p->environ_p), ident, is_exposed); 
}
M_endf 


///////////////////////////////////////////////////////////////////////////////////////////////

int expose_verbmain_identifier(frame_S& frame, const std::wstring& ident,  bool is_exposed) try
{  
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"expose_verbmain_identifier() -- current stack frame has no associated verbmain environment -- expose/unexpose status of identifier = \"%S\" cannot be changed in this stack frame") % ident;     
        return -1;  
    }

 #if 0     
    // make sure this identifier is not shadowed in an environment newer than the local environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, ident))      
    {                                                          
        count_error(); 
        M_out_emsg(L"expose_verbmain_identifier() -- verbmain identifier = \"%S\" to be exposed/unexposed is shadowed in a newer environment -- it cannot be exposed/unexposed from this stack frame") % ident; 
        return -1;       
    }
#endif 


    return expose_identifier(*(frame.verbmain_sf_p->environ_p), ident, is_exposed); 
}
M_endf 



///////////////////////////////////////////////////////////////////////////////////////////////

int expose_static_identifier(frame_S& frame, const std::wstring& ident,  bool is_exposed) try
{  

    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"expose_static_identifier() -- current stack frame has no associated static environment -- expose/unexpose status of identifier = \"%S\" cannot be changed in this stack frame") % ident;     
        return -1;  
    }
   

#if 0     
    // make sure this identifier is not shadowed in an environment newer than the local environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, ident))      
    {                                                          
        count_error(); 
        M_out_emsg(L"expose_static_identifier() -- static identifier = \"%S\" to be exposed/unexposed is shadowed in a local/non-static environment -- it cannot be exposed/unexposed from this stack frame") % ident; 
        return -1;       
    }
#endif 


    return expose_identifier(*(frame.persist_sf_p->environ_p), ident, is_exposed); 
}
M_endf 
#endif



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            V   V     EEEEE    RRRR     BBBB                                                  
////            V   V     E        R   R    B   B                                                 
////             V V      EEEE     RRRR     BBBB                                                  
////             V V      E        R  R     B   B                                                 
////              V       EEEEE    R   R    BBBB                                                  
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   def_local_verb()     -- defines new verb in local     environment 
////   def_verbmain_verb()  -- defines new verb in verbmain  environment 
////   def_static_verb()    -- defines new verb in static    environment 
////   def_global_verb()    -- defines new verb in global    environment 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int def_verb(environ_S& env, const std::wstring& verbname, const verbset_S& verbset_inparm, const def_parm_S& parm) try
{
    M__(M_out(L"def_verb() -- 1  passed-in name = %s") % verbname;)

    // get local copy of verbset, so it can be modified, if required

    verbset_S verbset_in {verbset_inparm }; 
    

    // ---------------------------------------------------------------------------------------------
    // verbset is already defined -- add verbdefs from passed-in verbset_S to the existing verbset_S
    // ---------------------------------------------------------------------------------------------
    //  
    //     error, if existing identifier is not a verbset
    //     error, if either existing verbset or passed-in verbset calls for "custom eval"
    //     error, if priority and associativity don't match existing verbset
    //     error, if no_eval flags from existing verbset and passed-in verbset do not match exactly
    //     

    if (is_identifier_defined(env, verbname))
    {
        // locate existing verbset definition in environment -- verify that it's for a verb 
        // ---------------------------------------------------------------------------------

        symval_S symval { };
        auto grc = get_identifier(env, verbname, symval);

        if (grc != 0)
        {
            count_error(); 
            M_out_emsg(L"def_verb() -- !!!!!! internal error -- unexpected non-0 R/C from get_identifier() when looking for existing verbset = \"%S\"") % verbname; 
            return -1;        
        }
        
        if (!symval.is_verbset)  
        {
            count_error(); 
            M_out_emsg(L"def_verb() -- identifier \"%S\" is already defined, but is not a verbset -- cannot add a new verb definition with this name") % verbname; 
            return -1;      
        }


        verbset_S *verbset_p = symval.value_sp->verbset_sp.get();      

        if (verbset_p == nullptr)
        {
            count_error(); 
            M_out_emsg(L"def_verb() -- !!!!!! internal error -- verbset_sp.get() is nullptr when symval.is_verbset is true for existing verbset = \"%S\"") % verbname; 
            return -1;         
        }


        // make sure both existing and new verbset have same priority and left/right associativity
        // ---------------------------------------------------------------------------------------

        if (verbset_in.priority != verb_pri_N::_not_specified)                     // only check if priority in new verbset is specified -- otherwise defaults to same as existing verbset
        {
            if (verbset_p->priority != verbset_in.priority)
            {
                count_error(); 
                M_out_emsg( L"def_verb() -- existing verbset for \"%S\" has priority %d, but new verbset being added has priority %d -- new verbset cannot be added to the overload set") 
                          % verbname % verbset_p->priority % verbset_in.priority
                          ; 
                return -1;         
            }
        }
        
        if (verbset_p->right_associate != verbset_in.right_associate)
        {
            count_error(); 
            M_out_emsg( L"def_verb() -- right_associativity for existing verbset for \"%S\" differs from the right associativity of the new verbset being added -- new verbset cannot be added to the overload set") 
                      % verbname 
                      ; 
            return -1;         
        }

        if (verbset_p->left_associate != verbset_in.left_associate)
        {
            count_error(); 
            M_out_emsg( L"def_verb() -- left_associativity for existing verbset for \"%S\" differs from the left associativity of the new verbset being added -- new verbset cannot be added to the overload set") 
                      % verbname 
                      ; 
            return -1;         
        }


        // make sure both existing verbset and new verbset do not specifiy custom evaluation
        // ---------------------------------------------------------------------------------

        if (verbset_in.custom_eval)
        {
            count_error(); 
            M_out_emsg(L"def_verb() -- new verbset \"%S\" being added specifies custom lazy evaluation -- no overload is possible -- cannot add to existing overload set") % verbname; 
            return -1;        
        }

        if (verbset_p->custom_eval)
        {
            count_error(); 
            M_out_emsg(L"def_verb() -- existing verbset \"%S\" specifies custom lazy evaluation -- no overload is possible -- cannot add a second verbdef to the overload set") % verbname; 
            return -1;        
        }


        // make sure new verbset and existing verbset have same evaluation flags (left-side and right-side)
        // ------------------------------------------------------------------------------------------------

        if (
            (verbset_p->left_eval.no_eval_ident      != verbset_in.left_eval.no_eval_ident         )
            ||
            (verbset_p->left_eval.no_eval_expression != verbset_in.left_eval.no_eval_expression    )
            ||
            (verbset_p->left_eval.no_eval_vlist      != verbset_in.left_eval.no_eval_vlist         )
            ||
            (verbset_p->left_eval.no_eval_ref        != verbset_in.left_eval.no_eval_ref           )
           )
        {
            count_error(); 
            M_out_emsg( L"def_verb() -- new and existing verbset \"%S\" have differing left-side lazy evaluation flags: ident/expr/vlist/ref -- new:%s%s%s%s  existing:%s%s%s%s -- cannot add new verbset to existing overload set") 
                      % verbname 
                      % (verbset_in.left_eval.no_eval_ident       ? L"T" : L"F") 
                      % (verbset_in.left_eval.no_eval_expression  ? L"T" : L"F") 
                      % (verbset_in.left_eval.no_eval_vlist       ? L"T" : L"F") 
                      % (verbset_in.left_eval.no_eval_ref         ? L"T" : L"F") 
                      % (verbset_p->left_eval.no_eval_ident       ? L"T" : L"F") 
                      % (verbset_p->left_eval.no_eval_expression  ? L"T" : L"F") 
                      % (verbset_p->left_eval.no_eval_vlist       ? L"T" : L"F") 
                      % (verbset_p->left_eval.no_eval_ref         ? L"T" : L"F") 
                      ;
            return -1;          
        }

        if (
            (verbset_p->right_eval.no_eval_ident      != verbset_in.right_eval.no_eval_ident         )
            ||
            (verbset_p->right_eval.no_eval_expression != verbset_in.right_eval.no_eval_expression    )
            ||
            (verbset_p->right_eval.no_eval_vlist      != verbset_in.right_eval.no_eval_vlist         )
            ||
            (verbset_p->right_eval.no_eval_ref        != verbset_in.right_eval.no_eval_ref           )
           )
        {
            count_error(); 
            M_out_emsg( L"def_verb() -- new and existing verbset \"%S\" have differing left-side lazy evaluation flags: ident/expr/vlist/ref -- new:%s%s%s%s  existing:%s%s%s%s -- cannot add new verbset to existing overload set") 
                      % verbname 
                      % (verbset_in.right_eval.no_eval_ident      ? L"T" : L"F") 
                      % (verbset_in.right_eval.no_eval_expression ? L"T" : L"F") 
                      % (verbset_in.right_eval.no_eval_vlist      ? L"T" : L"F") 
                      % (verbset_in.right_eval.no_eval_ref        ? L"T" : L"F") 
                      % (verbset_p->right_eval.no_eval_ident      ? L"T" : L"F") 
                      % (verbset_p->right_eval.no_eval_expression ? L"T" : L"F") 
                      % (verbset_p->right_eval.no_eval_vlist      ? L"T" : L"F") 
                      % (verbset_p->right_eval.no_eval_ref        ? L"T" : L"F") 
                      ;
            return -1;          
        }


        // set "has_builtin" flag in existing verbset, if verbset being added contains any builtin verbs

        if (verbset_in.has_builtin)
            verbset_p->has_builtin = true; 


        // make sure any nested objects are unshared, if requested by caller (need to do this here, since we're not calling def_identifier() with the "unshare" flag) 
 
        if (parm.unshare)                             
            unshare_verbset(verbset_in);


        // if all is OK, append all verbdefs from passed-in verbset to those already in the existing verbset (priority, associativity, evaluation flags in existing verbset are not changed)

        (verbset_p->verbs).insert((verbset_p->verbs).end(), verbset_in.verbs.begin(), verbset_in.verbs.end());
        return 0;
    }


    M__(last_place(L"def_verb() 2");) 


    // ----------------------------------------------------------------------------
    // identifier is not yet defined -- add in new verbset to passed-in environment
    // ----------------------------------------------------------------------------

    symval_S                  symval    {             };       // local environ_S to fill in 
    std::shared_ptr<value_S>  value_sp  { new value_S };       // temporary shared pointer -- will release ownership when this function returns 

    M__(last_place(L"def_verb() 5");) 

    symval.is_verbset  = true; 
    symval.is_builtin  = parm.builtin;
    symval.value_sp    = value_sp;


    // use default priority, if priority is not specified in the passed-in verbset

    if (verbset_in.priority == verb_pri_N::_not_specified)
        verbset_in.priority = verb_pri_N::_default;              // replace "not_specified" with "default"

    M__(last_place(L"def_verb() 6");) 


    // add symval with new verbset_S to the sepcified environment

    *(symval.value_sp) = verbset_val(verbset_in);                // everything still shared  
    return def_identifier(env, verbname, symval, parm.unshare);  // make sure everything is unshared  
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_verb(frame_S& frame, const std::wstring& verbname, const verbset_S& verbset, const def_parm_S& inparm) try
{  
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_local_verb() -- current stack frame has no associated local environment -- identifier = \"%S\" cannot be defined as local: for this stack frame") % verbname;     
        return -1;  
    }

    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                                       // non-globals can't be no_shadow
    parm.builtin    = false;                                                                       // non-globals can't be builtin
    
    // (no need for shadowing check here, since typdefs can't be defined in a parms-only SF)

    return def_verb(*(frame.local_sf_p->environ_p), verbname, verbset, parm); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_verbmain_verb(frame_S& frame, const std::wstring& verbname, const verbset_S& verbset, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                                       // non-globals can't be no_shadow
    parm.builtin    = false;                                                                       // non-globals can't be builtin
    

    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_verbmain_verb() -- current stack frame has no associated verbmain environment -- identifier = \"%S\" cannot be defined as verbmain: in this stack frame") % verbname;     
        return -1;  
    }
    

    // make sure this identifier is not shadowed in an environment newer than the verbmain environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, verbname))     
    {                                                           
        count_error(); 
        M_out_emsg(L"def_verbmain_verb() -- verbmain verb identifier = \"%S\" would be shadowed in a local environment -- it cannot be defined as a verbmain identifier from this stack frame") % verbname; 
        return -1;       
    }

    return def_verb(*(frame.verbmain_sf_p->environ_p), verbname, verbset, parm);       
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////

int def_static_verb(frame_S& frame, const std::wstring& verbname, const verbset_S& verbset, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                                       // non-globals can't be no_shadow
    parm.builtin    = false;                                                                       // non-globals can't be builtin
    

    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_static_verb() -- current stack frame has no associated static environment -- identifier = \"%S\" cannot be defined as static: in this stack frame") % verbname;     
        return -1;  
    }
    

    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, verbname))     
    {                                                           
        count_error(); 
        M_out_emsg(L"def_static_verb() -- static verb identifier = \"%S\" would be shadowed in a local (non-static) environment -- it cannot be defined as a static identifier from this stack frame") % verbname; 
        return -1;       
    }

    return def_verb(*(frame.persist_sf_p->environ_p), verbname, verbset, parm);       
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_verb(const std::wstring& verbname, const verbset_S& verbset, const def_parm_S& inparm) try
{  
    // make sure this verbname is not shadowed in an environment newer than the global environment

    if (is_global_ident_shadowed(verbname))  
    {                                           
        count_error(); 
        M_out_emsg(L"def_global_verb() -- global verbname = \"%S\" would be shadowed in a non-global environment -- it cannot be defined as a global verbset from this stack frame") % verbname; 
        return -1;       
    }
 

    def_parm_S parm = inparm; 
#ifdef M_EXPOSE_SUPPORT
    parm.exposed   = true;                                                                        // globals are always exposed
#endif
   
    return def_verb(*get_global_environ(), verbname, verbset, parm); 
}
M_endf
 



///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   undef_local_verb()     -- removes passed-in verbset in local    environment
////   undef_verbmain_verb()  -- removes passed-in verbset in verbmain environment
////   undef_static_verb()    -- removes passed-in verbset in static   environment
////   undef_global_verb()    -- removes passed-in verbset in global   environment
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int undef_verb(environ_S& env, const std::wstring& verbname) try
{
    // return immediately, if identifier is not defined

    if (!is_identifier_defined(env, verbname))
    {
        count_error(); 
        M_out_emsg(L"undef_verb() -- verbset \"%s\" (to be removed) is not defined") % verbname; 
        return -1;
    }


    // make sure this identifier is for a verb

    if (!is_identifier_verb(env, verbname))
    {
        count_error(); 
        M_out_emsg(L"undef_verb() -- identifier \"%s\" is not a verbset -- remove verbset request failed") % verbname; 
        return -1;      
    }


    // make sure this identifier is removable

    if (!is_identifier_removable(env, verbname))      // this flag may never be set for a verbdef
    {
        count_error(); 
        M_out_emsg(L"undef_verb() -- verbset \"%s\" is not removable -- remove request failed") % verbname; 
        return -1;      
    }

    return undef_identifier(env, verbname);   
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_local_verb(frame_S& frame, const std::wstring& verbname) try
{
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_local_verb() -- current stack frame has no associated local environment -- local verb identifier = \"%S\" cannot be undefined from this stack frame") % verbname;     
        return -1;  
    }

    // (don't need to do shadowing check for verbdef, since thay cannot appear in parms-only SFs)  

    return undef_verb(*(frame.local_sf_p->environ_p), verbname);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_verbmain_verb(frame_S& frame, const std::wstring& verbname) try
{
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_verbmain_verb() -- current stack frame has no associated verbmain environment -- identifier = \"%S\" cannot be undefined as verbmain: in this stack frame") % verbname;     
        return -1;  
    }


#if 0
    // make sure this identifier is not shadowed in an environment newer than the verbmain environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, verbname))     
    {                                                           
        count_error(); 
        M_out_emsg(L"undef_verbmain_verb() -- verbmain verb identifier = \"%S\" is shadowed in a local environment -- it cannot be undefined from this stack frame") % verbname; 
        return -1;       
    }
#endif
   
    return undef_verb(*(frame.verbmain_sf_p->environ_p), verbname);
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_static_verb(frame_S& frame, const std::wstring& verbname) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_static_verb() -- current stack frame has no associated static environment -- identifier = \"%S\" cannot be undefined as static: in this stack frame") % verbname;     
        return -1;  
    }


#if 0
    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, verbname))     
    {                                                           
        count_error(); 
        M_out_emsg(L"undef_static_verb() -- static verb identifier = \"%S\" is shadowed in a local (non-static) environment -- it cannot be undefined from this stack frame") % verbname; 
        return -1;       
    }
#endif
   
    return undef_verb(*(frame.persist_sf_p->environ_p), verbname);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_global_verb(const std::wstring& verbname) try
{
#if 0
    // make sure this identifier is not shadowed in an environment newer than gllobal environment

    if (is_global_ident_shadowed(verbname))     
    {                                                           
        count_error(); 
        M_out_emsg(L"undef_global_verb() -- global verb identifier = \"%S\" is shadowed in a non-global environment -- it cannot be undefined from this stack frame") % verbname; 
        return -1;       
    }
#endif

    return undef_verb(*get_global_environ(), verbname);
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_verb()           -- gets verbdef_S of passed-in verbname (-1 if not in local or global environment, or is not visible in other environments) 
////   get_local_verb()     -- gets verbdef_S of passed-in verbname (-1 if not in local     environment)
////   get_verbmain_verb()  -- gets verbdef_S of passed-in verbname (-1 if not in verbmain  environment)
////   get_static_verb()    -- gets verbdef_S of passed-in verbname (-1 if not in static    environment)
////   get_global verb()    -- gets verbdef_S of passed-in verbname (-1 if not in global    environment)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ----------------------------
// version that returns verbset
// ----------------------------

static int get_verb(environ_S& env, const std::wstring& verbname, verbset_S& verbset) try
{
    if (!is_identifier_defined(env, verbname))           // return with R/C = -1 and empty verbset, if verb is not there 
    {
        verbdef_S verbdef     { }; 
        verbset = verbset_S   { };
        verbset.verbs.push_back(verbdef); 
        return -1;                   
    }
    else
    {
        value_S value { }; 
        auto grc = get_identifier(env, verbname, value);

        if (grc == 0)
        {
            if (value.verbset_sp.get() != nullptr)
            {
                verbset = *(value.verbset_sp);              // return versbdef from value associated with this verb identifier or variable 
                return 0;  
            }                                               
            else                                            // error -- verbset_sp is empty -- no verbset for this variable -- return default/empty verbdef
            {
                verbdef_S verbdef     { }; 
                verbset = verbset_S   { };
                verbset.verbs.push_back(verbdef); 
                return -1;             
            }
        }
        else                                                // error -- identifier not found -- return default/empty verbdef  ???? should not occur ?????
        {
            verbdef_S verbdef     { }; 
            verbset = verbset_S   { };
            verbset.verbs.push_back(verbdef); 
            return -1;           
        }
    }
}
M_endf


// --------------------------------------------
// version that returns complete symval_S entry
// --------------------------------------------

static int get_verb(environ_S& env, const std::wstring& verbname, symval_S& out_symval) try
{
    if (!is_identifier_defined(env, verbname))           // return with R/C = -1 and empty value, if verb is not there 
    {
        out_symval = symval_S { };
        return -1;                   
    }
    else
    {
        symval_S symval { }; 
        auto grc = get_identifier(env, verbname, symval);

        if (grc == 0)
        {
            if (symval.value_sp->verbset_sp.get() != nullptr)
            {
                out_symval = symval;                        // return symval for this verbset identifier 
                return 0;  
            }                                               
            else                                            // error -- verbset_sp is empty -- no verbset for this variable -- return default/empty verbset
            {
                out_symval = symval_S { };
                return -1;             
            }
        }
        else                                                // error -- identifier not found -- return default/empty verbset  ???? should not occur ?????
        {
            out_symval = symval_S { };
            return -1;           
        }
    }
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_local_verb(const frame_S& frame, const std::wstring& verbname, verbset_S& verbset) try
{
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_local_verb() -- current stack frame has no associated local environment -- value of local identifier = \"%S\" cannot be obtained in this stack frame") % verbname;     
        return -1;  
    }

    return get_verb(*(frame.local_sf_p->environ_p), verbname, verbset);
}
M_endf


int get_local_verb(const frame_S& frame, const std::wstring& verbname, symval_S& symval) try
{
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_local_verb() -- current stack frame has no associated local environment -- value of local identifier = \"%S\" cannot be obtained in this stack frame") % verbname;     
        return -1;  
    }

    return get_verb(*(frame.local_sf_p->environ_p), verbname, symval);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_verbmain_verb(const frame_S& frame, const std::wstring& verbname, verbset_S& verbset) try
{
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_verbmain_verb() -- current stack frame has no associated verbmain environment -- value of verbmain identifier = \"%S\" cannot be obtained in this stack frame") % verbname;     
        return -1;  
    }

    return get_verb(*(frame.verbmain_sf_p->environ_p), verbname, verbset);
}
M_endf


int get_verbmain_verb(const frame_S& frame, const std::wstring& verbname, symval_S& symval) try
{
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_verbmain_verb() -- current stack frame has no associated verbmain environment -- value of local identifier = \"%S\" cannot be obtained in this stack frame") % verbname;     
        return -1;  
    }

    return get_verb(*(frame.verbmain_sf_p->environ_p), verbname, symval);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_static_verb(const frame_S& frame, const std::wstring& verbname, verbset_S& verbset) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_static_verb() -- current stack frame has no associated static environment -- value of static identifier = \"%S\" cannot be obtained in this stack frame") % verbname;     
        return -1;  
    }
   
    return get_verb(*(frame.persist_sf_p->environ_p), verbname, verbset);
}
M_endf


int get_static_verb(const frame_S& frame, const std::wstring& verbname, symval_S& symval) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_static_verb() -- current stack frame has no associated static environment -- value of static identifier = \"%S\" cannot be obtained in this stack frame") % verbname;     
        return -1;  
    }
   
    return get_verb(*(frame.persist_sf_p->environ_p), verbname, symval);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_global_verb(const std::wstring& verbname, verbset_S& verbset) try                                                                              
{
    return get_verb(*get_global_environ(), verbname, verbset);
}
M_endf


int get_global_verb(const std::wstring& verbname, symval_S& symval) try                                                                              
{
    return get_verb(*get_global_environ(), verbname, symval);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

int get_verb(const frame_S& frame, const std::wstring& verbname, verbset_S& verbset) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, verbname, p);

    if (rc != 0)                                   // identifier not found in any environment
    {
        verbdef_S verbdef     { }; 
        verbset = verbset_S   { };
        verbset.verbs.push_back(verbdef);          // pass back empty verbdef_S  
 
        return -1;                                 // failure r/c
    }                                             
    else                                           // environment found with verb
    {
        return get_verb(*p, verbname, verbset);    
    }
}
M_endf  


int get_verb(const frame_S& frame, const std::wstring& verbname, symval_S& symval) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, verbname, p);

    if (rc != 0)                                   // identifier not found in any environment
    {                                             
        symval = symval_S { };                     // pass back empty symval_S   
        return -1;                                 // failure r/c
    }                                             
    else                                           // environment found with verb
    {
        return get_verb(*p, verbname, symval);    
    }
}
M_endf  



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            TTTTT     Y   Y    PPPP      EEEEE                                            
////              T        Y Y     P   P     E                                                
////              T         Y      PPPP      EEEE                                             
////              T         Y      P         E                                                
////              T         Y      P         EEEEE                                            
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




 ////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   def_local_typdef()     -- defines new typdef in local    environment 
////   def_verbmain_typdef()  -- defines new typdef in verbmain environment 
////   def_static_typdef()    -- defines new typdef in static   environment 
////   def_global_typdef()    -- defines new typdef in global   environment 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int def_typdef(environ_S& env, const std::wstring& typdef_name, const typdef_S& typdef_in, const def_parm_S& parm) try
{
    M__(M_out(L"def_typdef() -- passed-in name = %s") % typdef_name;)
 
 
    // error if this typdef is already defined

    if (is_identifier_defined(env, typdef_name))
    {
        count_error(); 
        M_out_emsg(L"def_typdef() -- identifier = \"%s\" is already defined -- cannot be redefined as typedef") % typdef_name; 
        return -1;      
    }


    // construct typdef_S for new typdef -- can't alter passed-in typdef, so make new local one with the const/exposed flags, and any unshared nested typdefs

    typdef_S typdef { };

    typdef = typdef_in;                                      // any nested objects are still shared      
 

    // add in new typdef to passed-in environment

    symval_S                 symval    {             };      // local symval_S to fill in 
    std::shared_ptr<value_S> value_sp  { new value_S };      // temporary shared pointer -- will release ownership when this function returns 

    symval.is_typdef     = true; 
    symval.no_update     = true;
    symval.no_undefine   = true;
    symval.is_builtin    = parm.builtin;
#ifdef M_EXPOSE_SUPPORT
    symval.is_exposed    = parm.exposed;
#endif
    symval.no_shadow     = parm.no_shadow;
    symval.value_sp      = value_sp;
    *(symval.value_sp)   = typdef_val(typdef); 

    return def_identifier(env, typdef_name, symval, parm.unshare);
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_typdef(frame_S& frame, const std::wstring& typdef_name, const typdef_S& typdef, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                                       // non-globals can't be no_shadow
    parm.builtin    = false;                                                                       // non-globals can't be builtin


    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_local_typdef() -- current stack frame has no associated local environment -- identifier = \"%S\" cannot be defined as local: for this stack frame") % typdef_name;     
        return -1;  
    }
    
    // (no need for shadowing check here, since typdefs can't be defined in a parms-only SF)

    return def_typdef(*(frame.local_sf_p->environ_p), typdef_name, typdef, parm); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_verbmain_typdef(frame_S& frame, const std::wstring& typdef_name, const typdef_S& typdef, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                                       // non-globals can't be no_shadow
    parm.builtin    = false;                                                                       // non-globals can't be builtin
 

    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_verbmain_typdef() -- current stack frame has no associated verbmain environment -- identifier = \"%S\" cannot be defined as verbmain: in this stack frame") % typdef_name;     
        return -1;  
    }
    

    // make sure this identifier is not shadowed in an environment newer than the verbmain environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, typdef_name))  
    {                                                   
        count_error(); 
        M_out_emsg(L"def_verbmain_typdef() -- verbmain typdef identifier = \"%S\" would be shadowed in a local environment -- it cannot be defined as a verbmain identifier from this stack frame") % typdef_name; 
        return -1;       
    }
  
    return def_typdef(*(frame.verbmain_sf_p->environ_p), typdef_name, typdef, parm);       
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_static_typdef(frame_S& frame, const std::wstring& typdef_name, const typdef_S& typdef, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                                       // non-globals can't be no_shadow
    parm.builtin    = false;                                                                       // non-globals can't be builtin
 

    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_static_typdef() -- current stack frame has no associated static environment -- identifier = \"%S\" cannot be defined as static: in this stack frame") % typdef_name;     
        return -1;  
    }
    

    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, typdef_name))  
    {                                                   
        count_error(); 
        M_out_emsg(L"def_static_typdef() -- static typdef identifier = \"%S\" would be shadowed in a local (non-static) environment -- it cannot be defined as a static identifier from this stack frame") % typdef_name; 
        return -1;       
    }
  
    return def_typdef(*(frame.persist_sf_p->environ_p), typdef_name, typdef, parm);       
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_typdef(const std::wstring& typdef_name, const typdef_S& typdef, const def_parm_S& inparm) try
{  
    // make sure this type_name is not shadowed in an environment newer than the global environment

    if (is_global_ident_shadowed(typdef_name))  
    {                                            
        count_error(); 
        M_out_emsg(L"def_global_typdef() -- global typdef identifier = \"%S\" would be shadowed in a non-global environment -- it cannot be defined as a global type_name from this stack frame") % typdef_name; 
        return -1;       
    }

    def_parm_S parm = inparm; 
#ifdef M_EXPOSE_SUPPORT
    parm.exposed   = true;                                                                        // globals are always exposed
#endif
    
    return def_typdef(*get_global_environ(), typdef_name, typdef, parm);           
}
M_endf
 



///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   undef_local_typdef()    -- removes passed-in typdef from local     environment
////   undef_verbmain_typdef() -- removes passed-in typdef from verbmain  environment
////   undef_static_typdef()   -- removes passed-in typdef from static    environment
////   undef_global_typdef()   -- removes passed-in typdef from global    environment
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int undef_typdef(environ_S& env, const std::wstring& typdef_name) try
{
    // return immediately, if identifier is not defined

    if (!is_identifier_defined(env, typdef_name))
    {
        count_error(); 
        M_out_emsg(L"undef_typdef() -- typedef \"%s\" (to be removed) is not defined") % typdef_name; 
        return -1;
    }


    // make sure this identifier is for a typdef

    if (!is_identifier_typdef(env, typdef_name))
    {
        count_error(); 
        M_out_emsg(L"undef_typdef() -- identifier \"%s\" is not a typedef -- remove typdef request failed") % typdef_name; 
        return -1;      
    }
  

    // make sure this identifier is removable
    
    if (!is_identifier_removable(env, typdef_name))
    {
        count_error(); 
        M_out_emsg(L"undef_typdef() -- typedef \"%s\" is not removable -- remove request failed") % typdef_name; 
        return -1;
    }

    return undef_identifier(env, typdef_name);   
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_local_typdef(frame_S& frame, const std::wstring& typdef_name) try
{
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_local_typdef() -- current stack frame has no associated local environment -- identifier = \"%S\" cannot be undefined as local: in this stack frame") % typdef_name;     
        return -1;  
    }

    // (don't need to do the shadow check for loca typdef, since parms-only SF cannot have typedefs)

    return undef_typdef(*(frame.local_sf_p->environ_p), typdef_name);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_verbmain_typdef(frame_S& frame, const std::wstring& typdef_name) try
{
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_verbmainc_typdef() -- current stack frame has no associated verbmain environment -- verbmain identifier = \"%S\" cannot be undefined in this stack frame") % typdef_name;     
        return -1;  
    }

#if 0    
    // make sure this identifier is not shadowed in an environment newer than the verbmain environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, typdef_name))  
    {                                                   
        count_error(); 
        M_out_emsg(L"undef_verbmain_typdef() -- verbmain typdef identifier = \"%S\" is shadowed in a local environment -- it cannot be undefined from this stack frame") % typdef_name; 
        return -1;       
    }
#endif
   
    return undef_typdef(*(frame.verbmain_sf_p->environ_p), typdef_name);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_static_typdef(frame_S& frame, const std::wstring& typdef_name) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_static_typdef() -- current stack frame has no associated static environment -- static identifier = \"%S\" cannot be undefined in this stack frame") % typdef_name;     
        return -1;  
    }

#if 0    
    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, typdef_name))  
    {                                                   
        count_error(); 
        M_out_emsg(L"undef_static_typdef() -- static typdef identifier = \"%S\" is shadowed in a local (non-static) environment -- it cannot be undefined from this stack frame") % typdef_name; 
        return -1;       
    }
#endif
   
    return undef_typdef(*(frame.persist_sf_p->environ_p), typdef_name);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_global_typdef(const std::wstring& typdef_name) try
{
#if 0    
    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_global_ident_shadowed(typdef_name))  
    {                                                   
        count_error(); 
        M_out_emsg(L"undef_global_typdef() -- global typdef identifier = \"%S\" is shadowed in a non-global environment -- it cannot be undefined from this stack frame") % typdef_name; 
        return -1;       
    }
#endif

    return undef_typdef(*get_global_environ(), typdef_name);
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_typdef()          -- gets typdef_S of passed-in typename (-1 if not in local or global environment, or is not visible in other environments) 
////   get_local_typdef()    -- gets typdef_S of passed-in typename (-1 if not in local    environment)
////   get_verbmain_typdef() -- gets typdef_S of passed-in typename (-1 if not in verbmain environment)
////   get_static_typdef()   -- gets typdef_S of passed-in typename (-1 if not in static   environment)
////   get_global_typdef()   -- gets typdef_S of passed-in typename (-1 if not in global   environment)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ----------------------------
// version that returns typedef (internal function)
// ----------------------------  

static int get_typdef(environ_S& env, const std::wstring& typdef_name, typdef_S& typdef) try
{
    if (!is_identifier_defined(env, typdef_name))           // return with R/C = -1 and empty typedef_S, if typdef is not there 
    {
        typdef = typdef_S { };
        return -1;                   
    }
    else
    {
        value_S value { }; 
        auto grc = get_identifier(env, typdef_name, value);

        if (grc == 0)
        {
            if (value.typdef_sp.use_count() > 0)
            {
                typdef = *(value.typdef_sp);                // return typdef from value associated with this typdef identifier or variable 
                return 0;  
            }                                               
            else                                            // error -- typdef_sp is empty -- no typdef for this variable -- return default/empty typdef
            {
                typdef = typdef_S { };
                return -1;             
            }
        }
        else                                                // error -- identifier not found -- return default/empty typdef  ???? should not occur ?????
        {
            typdef = typdef_S { };
            return -1;           
        }
    }
}
M_endf
    

// --------------------------------------------
// version that returns complete symval_S entry (internal function)
// --------------------------------------------

static int get_typdef(environ_S& env, const std::wstring& typdef_name, symval_S& symval_out) try
{
    if (!is_identifier_defined(env, typdef_name))          // return with R/C = -1 and empty value, if typdef is not there 
    {
        symval_out = symval_S { };
        return -1;                   
    }
    else
    {
        symval_S symval { }; 
        auto grc = get_identifier(env, typdef_name, symval);

        if (grc == 0)
        {
            if (symval.value_sp->typdef_sp.use_count() > 0)
            {
                symval_out = symval;                        // return symval_S associated with this typdef identifier or variable 
                return 0;  
            }                                               
            else                                            // error -- typdef_sp is empty -- no typdef for this variable -- return default/empty symval_S
            {
                symval_out = symval_S { };
                return -1;             
            }
        }
        else                                                // error -- identifier not found -- return default/empty symval_S  ???? should not occur ?????
        {
            symval_out = symval_S { };
            return -1;           
        }
    }
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_local_typdef(const frame_S& frame, const std::wstring& typdef_name, typdef_S& typdef) try
{
    // make sure local environemnt exists for this stack frame

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_local_typdef() -- current stack frame has no associated local environment -- the value of local identifier = \"%S\" cannot be obtained from this stack frame") % typdef_name;     
        return -1;  
    }

    return get_typdef(*(frame.local_sf_p->environ_p), typdef_name, typdef);
}
M_endf


int get_local_typdef(const frame_S& frame, const std::wstring& typdef_name, symval_S& symval) try
{
    // make sure local environemnt exists for this stack frame

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_local_typdef() -- current stack frame has no associated local environment -- the value of local identifier = \"%S\" cannot be obtained from this stack frame") % typdef_name;     
        return -1;  
    }

    return get_typdef(*(frame.local_sf_p->environ_p), typdef_name, symval);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_verbmain_typdef(const frame_S& frame, const std::wstring& typdef_name, typdef_S& typdef) try
{
    // make sure verbmain environemnt exists for this stack frame

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_verbmain_typdef() -- current stack frame has no associated verbmain environment -- the value of verbmain identifier = \"%S\" cannot be obtained from this stack frame") % typdef_name;     
        return -1;  
    }

    return get_typdef(*(frame.verbmain_sf_p->environ_p), typdef_name, typdef);
}
M_endf


int get_verbmain_typdef(const frame_S& frame, const std::wstring& typdef_name, symval_S& symval) try
{
    // make sure verbmain environemnt exists for this stack frame

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_verbmain_typdef() -- current stack frame has no associated verbmain environment -- the value of verbmain identifier = \"%S\" cannot be obtained from this stack frame") % typdef_name;     
        return -1;  
    }

    return get_typdef(*(frame.verbmain_sf_p->environ_p), typdef_name, symval);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_static_typdef(const frame_S& frame, const std::wstring& typdef_name, typdef_S& typdef) try
{
    // make sure static environemnt exists for this stack frame

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_static_typdef() -- current stack frame has no associated static environment -- the value of static identifier = \"%S\" cannot be obtained from this stack frame") % typdef_name;     
        return -1;  
    }
   
    return get_typdef(*(frame.persist_sf_p->environ_p), typdef_name, typdef);
}
M_endf


int get_static_typdef(const frame_S& frame, const std::wstring& typdef_name, symval_S& symval) try
{
    // make sure static environemnt exists for this stack frame

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_static_typdef() -- current stack frame has no associated static environment -- the value of static identifier = \"%S\" cannot be obtained from this stack frame") % typdef_name;     
        return -1;  
    }
   
    return get_typdef(*(frame.persist_sf_p->environ_p), typdef_name, symval);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_global_typdef(const std::wstring& typdef_name, typdef_S& typdef) try                                                                              
{
    return get_typdef(*get_global_environ(), typdef_name, typdef);
}
M_endf


int get_global_typdef(const std::wstring& typdef_name, symval_S& symval) try                                                                              
{
    return get_typdef(*get_global_environ(), typdef_name, symval);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

int get_typdef(const frame_S& frame, const std::wstring& typdef_name, typdef_S& typdef) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, typdef_name, p);

    if (rc != 0)                                   // identifier not found in any environment
    {                                             
        typdef = typdef_S { };                     // pass back empty typdef_S    
        return -1;                                 // failure r/c
    }                                             
    else                                           // environment found with typdef
    {
        return get_typdef(*p, typdef_name, typdef);    
    }
}
M_endf  


int get_typdef(const frame_S& frame, const std::wstring& typdef_name, symval_S& symval) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, typdef_name, p);

    if (rc != 0)                                   // identifier not found in any environment
    {                                             
        symval = symval_S { };                     // pass back empty symval_S   
        return -1;                                 // failure r/c
    }                                             
    else                                           // environment found with typdef
    {
        return get_typdef(*p, typdef_name, symval);    
    }
}
M_endf 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            V   V       A       RRRR                                               
////            V   V      A A      R   R                                              
////             V V      A   A     RRRR                                               
////             V V      AAAAA     R  R                                               
////              V       A   A     R   R                                              
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   def_local_var()          -- defines new variable/const in local    environment 
////   def_parms_var()          -- defines new variable/const in search   environment -- used only for defining invocation parms variables 
////   def_verbmain_var()       -- defines new variable/const in verbmain environment 
////   def_static_var()         -- defines new variable/const in static   environment 
////   def_global_var()         -- defines new variable/const in global   environment 
////   def_local_const()        -- defines new          const in global   environment 
////   def_verbmain_const()     -- defines new          const in verbmain environment 
////   def_static_const()       -- defines new          const in static   environment
////   def_global_const()       -- defines new          const in local    environment 
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

static int def_variable(environ_S& env, const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm  = inparm; 
    

    // set flags so that built-ins and constants can't be updated or undefined

    if (inparm.constant)    parm.no_undefine  = true; 
    if (inparm.constant)    parm.no_update    = true; 
    if (inparm.builtin )    parm.no_undefine  = true; 
    if (inparm.builtin )    parm.no_update    = true; 


    // construct symval_S for new local variable identifier

    symval_S                  symval    {             };   // local environ_S to fill in 
    std::shared_ptr<value_S>  value_sp  { new value_S };   // temporary shared pointer -- will release ownership when this function returns 

    symval.value_sp      = value_sp;
    *(symval.value_sp)   = value;  
    symval.is_builtin    = parm.builtin;
    symval.is_const      = parm.constant;
#ifdef M_EXPOSE_SUPPORT
    symval.is_exposed    = parm.exposed;
#endif
    symval.no_shadow     = parm.no_shadow;
    symval.no_update     = parm.no_update;                       
    symval.no_undefine   = parm.no_update;                       
                                        
    return def_identifier(env, ident, symval, parm.unshare); 
}
M_endf                          


///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_var(frame_S& frame, const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                       // non-globals can't be no_shadow
    parm.builtin    = false;                                                       // non-globals can't be builtin
 

    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_local_var() -- current stack frame has no associated local environment -- identifier = \"%S\" cannot be defined as local: in this stack frame") % ident;     
        return -1;  
    }


    // make sure this identifier is not shadowed in a parms-only environment newer than the local environment

    if (is_ident_sf_shadowed(frame.local_sf_p, ident))  
    {                                                                          
        count_error(); 
        M_out_emsg(L"def_local_var() -- local identifier = \"%S\" would be shadowed by saved verb invocation parms -- it cannot be defined as a local identifier from this stack frame") % ident; 
        return -1;       
    }

    return def_variable(*(frame.local_sf_p->environ_p), ident, value, parm);     
}
M_endf

///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_const(frame_S& frame, const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.constant   = true;                                                        // mark this as constant
    
    //  ( local_sf_p check and shadowing check is in def_local_var() ) 

    return def_local_var(frame, ident, value, parm);     
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

// special version for defining verb invocation parms in search stack frame rather than local stack frame as usual

int def_parms_var(frame_S& frame, const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                       // non-globals can't be no_shadow
    parm.builtin    = false;                                                       // non-globals can't be builtin
     
    return def_variable(*(frame.search_sf_p->environ_p), ident, value, parm);      // search_p is always newest SF -- no shadowing check needed
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////

int def_verbmain_var(frame_S& frame, const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                                    // non-globals can't be no_shadow
    parm.builtin    = false;                                                                    // non-globals can't be builtin
    

    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_verbmain_var() -- current stack frame has no associated verbmain environment -- identifier = \"%S\" cannot be defined as verbmain: in this stack frame") % ident;     
        return -1;  
    }
    

    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, ident))  
    {                                                                                     
        count_error(); 
        M_out_emsg(L"def_verbmain_var() -- verbmain identifier = \"%S\" would be shadowed in a local environment -- it cannot be defined as a verbmain identifier from this stack frame") % ident; 
        return -1;       
    }

    return def_variable(*(frame.verbmain_sf_p->environ_p), ident, value, parm);       
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_verbmain_const(frame_S& frame, const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.constant   = true;

    // ( shadowing check is in def_static_var() ) 

    return def_verbmain_var(frame, ident, value, parm); 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////

int def_static_var(frame_S& frame, const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.no_shadow  = false;                                                                    // non-globals can't be no_shadow
    parm.builtin    = false;                                                                    // non-globals can't be builtin
    

    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"def_static_var() -- current stack frame has no associated static environment -- identifier = \"%S\" cannot be defined as static: in this stack frame") % ident;     
        return -1;  
    }
    

    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, ident))  
    {                                                                                     
        count_error(); 
        M_out_emsg(L"def_static_var() -- static identifier = \"%S\" would be shadowed in a local (non-static) environment -- it cannot be defined as a static identifier from this stack frame") % ident; 
        return -1;       
    }

    return def_variable(*(frame.persist_sf_p->environ_p), ident, value, parm);       
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_static_const(frame_S& frame, const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.constant   = true;

    // ( shadowing check is in def_static_var() ) 

    return def_static_var(frame, ident, value, parm); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_var(const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    // make sure this identifier is not shadowed in an environment newer than the global environment

    if (is_global_ident_shadowed(ident))  
    {                                             // any global identifier is/would be shadowed
        count_error(); 
        M_out_emsg(L"def_global_var() -- global identifier = \"%S\" would be shadowed in a non-global environment -- it cannot be defined as a global identifier from this stack frame") % ident; 
        return -1;       
    }

    def_parm_S parm = inparm; 
#ifdef M_EXPOSE_SUPPORT
    parm.exposed    = true;                                                        // globals are always exposed
#endif

    return def_variable(*get_global_environ(), ident, value, parm);       
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_const(const std::wstring& ident, const value_S& value, const def_parm_S& inparm) try
{  
    def_parm_S parm = inparm; 
    parm.constant   = true;

    // ( shadowing check is in def_global_val() ) 

    return def_global_var(ident, value, parm); 
}
M_endf

 
///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   undef_local_var()    -- removes passed-in variable (if it is present in local    environment) 
////   undef_verbmain_var() -- removes passed-in variable (if it is present in verbmain environment) 
////   undef_static_var()   -- removes passed-in variable (if it is present in static   environment) 
////   undef_global_var()   -- removes passed-in variable (if it is present in global   environment) 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int undef_variable(environ_S& env, const std::wstring& ident) try
{
    // see if identifier already exists, and is constant

    auto ct = env.symbols.count(ident);

    if (!is_identifier_defined(env, ident))
    {
        count_error(); 
        M_out_emsg(L"undef_typdef() -- variable identifier \"%s\" (to be removed) is not defined") % ident; 
        return -1;
    }

    if (is_identifier_typdef(env, ident))
    {
        count_error(); 
        M_out_emsg(L"undef_var() -- cannot remove typdef \"%s\"") % ident; 
        return -1;
    }
    
    if (is_identifier_verb(env, ident)) 
    {
        count_error(); 
        M_out_emsg(L"undef_var() -- cannot remove verb \"%s\"") % ident; 
        return -1;
    }


    if (is_identifier_builtin(env, ident)) 
    {
        count_error(); 
        M_out_emsg(L"undef_var() -- cannot remove builtin identifier \"%s\"") % ident; 
        return -1;
    }
    
    if (is_identifier_const(env,ident))
    {
        count_error(); 
        M_out_emsg(L"undef_var() -- cannot remove constant identifier \"%s\"") % ident; 
        return -1;
    }  

    if (!is_identifier_removable(env, ident))
    {
        count_error(); 
        M_out_emsg(L"undef_var() -- identifier \"%s\" is not removable -- remove request failed") % ident; 
        return -1;
    }
 
    undef_identifier(env, ident);                     // get rid of old value 
    return 0;
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_local_var(frame_S& frame, const std::wstring& ident) try
{
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_local_var() -- current stack frame has no associated local environment -- identifier = \"%S\" cannot be undefined as local: in this stack frame") % ident;     
        return -1;  
    }

#if 0
    // make sure this identifier is not shadowed in a parms-only environment newer than the local environment

    if (is_ident_sf_shadowed(frame.local_sf_p, ident))  
    {                                                                          
        count_error(); 
        M_out_emsg(L"undef_local_var() -- local identifier = \"%S\" is shadowed by saved verb invocation parms -- it cannot be undefined from this stack frame") % ident; 
        return -1;       
    }
#endif


    return undef_variable(*(frame.local_sf_p->environ_p), ident);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_verbmain_var(frame_S& frame, const std::wstring& ident) try
{
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_verbmain_var() -- current stack frame has no associated verbmain environment -- identifier = \"%S\" cannot be undefined as verbmain: in this stack frame") % ident;     
        return -1;  
    }

#if 0
    // make sure this identifier is not shadowed in an environment newer than the verbmain environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, ident))  
    {                                                                                     
        count_error(); 
        M_out_emsg(L"undef_verbmain_var() -- verbmain identifier = \"%S\" is shadowed in a local environment -- it cannot be undefined from this stack frame") % ident; 
        return -1;       
    }
#endif
    
    return undef_variable(*(frame.verbmain_sf_p->environ_p), ident);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_static_var(frame_S& frame, const std::wstring& ident) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"undef_static_var() -- current stack frame has no associated static environment -- identifier = \"%S\" cannot be undefined as static: in this stack frame") % ident;     
        return -1;  
    }

#if 0
    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, ident))  
    {                                                                                     
        count_error(); 
        M_out_emsg(L"undef_static_var() -- static identifier = \"%S\" is shadowed in a local (non-static) environment -- it cannot be undefined from this stack frame") % ident; 
        return -1;       
    }
#endif
    
    return undef_variable(*(frame.persist_sf_p->environ_p), ident);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_global_var(const std::wstring& ident) try
{

#if 0
    // make sure this identifier is not shadowed in a non-global environment

    if (is_global_ident_shadowed(ident))  
    {                                                                                     
        count_error(); 
        M_out_emsg(L"global_static_var() -- global identifier = \"%S\" is shadowed in a non-global environment -- it cannot be undefined from this stack frame") % ident; 
        return -1;       
    }
#endif

    return undef_variable(*get_global_environ(), ident);
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_var()          -- gets value of passed-in variable (-1 if not found at all)            -- searches all environments in the upward scope chain) 
////   get_local_var()    -- gets value of passed-in variable (-1 if not in local    environment) -- searches only local  environment (latest frameblock or verb)
////   get_verbmain_var() -- gets value of passed-in variable (-1 if not in verbmain environment) -- searches only static environment (associated with latest frameblock or verb)
////   get_static_var()   -- gets value of passed-in variable (-1 if not in static   environment) -- searches only static environment (associated with latest frameblock or verb)
////   get_global var()   -- gets value of passed-in variable (-1 if not in global   environment) -- searches only global environment
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ----------------------------
// version that returns value_S (internal function)
// ----------------------------
                               
static int get_variable(environ_S& env, const std::wstring& ident, value_S& value) try
{
    if (!is_identifier_defined(env, ident))                 // return with R/C = -1 and empty value, if identifier is not there 
    {
        value = value_S {}; 
        return -1;
    }
    else                                                    // identifier is found -- might be verb or variable/constant
    {
        if (
             is_identifier_verb(env, ident)                 // don't pass back verbs found in environment
             ||
             is_identifier_typdef(env, ident)               // don't pass back typedefs found in environment
            )
        {
           value = value_S {}; 
           return -1;           
        }               


        // identifier is variable/constant -- return it

        value = *(env.symbols.at(ident).value_sp);         // value found -- pass it back
        return 0; 
    }
}
M_endf


// --------------------------------------------
// version that returns complete symval_S entry (internal function)
// --------------------------------------------
                               
static int get_variable(environ_S& env, const std::wstring& ident, symval_S& symval_out) try
{
    if (!is_identifier_defined(env, ident))                 // return with R/C = -1 and empty symval, if identifier is not there 
    {
        symval_out = symval_S {}; 
        return -1;
    }
    else                                                    // identifier is found -- might be verb or variable/constant or typedef, etc.
    {
        if (
             is_identifier_verb(env, ident)                 // don't pass back verbs found in environment
             ||
             is_identifier_typdef(env, ident)               // don't pass back typedefs found in environment
            )
        {
           symval_out = symval_S {}; 
           return -1;           
        }               


        // identifier is variable/constant -- return it

        symval_out = env.symbols.at(ident);                 // symval_S found -- pass it back
        return 0; 
    }
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_local_var(const frame_S& frame, const std::wstring& ident, value_S& value) try
{
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_local_var() -- current stack frame has no associated local environment -- value of local identifier = \"%S\" cannot be obtained from in this stack frame") % ident;     
        return -1;  
    }

    return get_variable(*(frame.local_sf_p->environ_p), ident, value);
}
M_endf


int get_local_var(const frame_S& frame, const std::wstring& ident, symval_S symval) try
{
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_local_var() -- current stack frame has no associated local environment -- value of local identifier = \"%S\" cannot be obtained from in this stack frame") % ident;     
        return -1;  
    }

    return get_variable(*(frame.local_sf_p->environ_p), ident, symval);
}
M_endf   


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_verbmain_var(const frame_S& frame, const std::wstring& ident, value_S& value) try
{
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_verbmain_var() -- current stack frame has no associated verbmain environment -- value of verbmain identifier = \"%S\" cannot be obtained from in this stack frame") % ident;     
        return -1;  
    }
   
    return get_variable(*(frame.verbmain_sf_p->environ_p), ident, value);
}
M_endf


int get_verbmain_var(const frame_S& frame, const std::wstring& ident, symval_S symval) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_verbmain_var() -- current stack frame has no associated verbmain environment -- value of verbmain identifier = \"%S\" cannot be obtained from this stack frame") % ident;     
        return -1;  
    }
    
    return get_variable(*(frame.verbmain_sf_p->environ_p), ident, symval);
}
M_endf   


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_static_var(const frame_S& frame, const std::wstring& ident, value_S& value) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_static_var() -- current stack frame has no associated static environment -- value of static identifier = \"%S\" cannot be obtained from in this stack frame") % ident;     
        return -1;  
    }
   
    return get_variable(*(frame.persist_sf_p->environ_p), ident, value);
}
M_endf


int get_static_var(const frame_S& frame, const std::wstring& ident, symval_S symval) try
{
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"get_static_var() -- current stack frame has no associated static environment -- value of static identifier = \"%S\" cannot be obtained from this stack frame") % ident;     
        return -1;  
    }
    
    return get_variable(*(frame.persist_sf_p->environ_p), ident, symval);
}
M_endf   


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_global_var(const std::wstring& ident, value_S& value) try
{
    return get_variable(*get_global_environ(), ident, value);
}
M_endf


int get_global_var(const std::wstring& ident, symval_S symval) try
{
    return get_variable(*get_global_environ(), ident, symval);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

int get_var(const frame_S& frame, const std::wstring& ident, value_S value) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                     // variable not found in any environment
    {
        value = value_S { };         // pass back empty value
        return -1;                   // failure r/c
    }
    else                             // environment found with identifier
    {
        return get_variable(*p, ident, value);    
    }
}
M_endf


int get_var(const frame_S& frame, const std::wstring& ident, symval_S& symval) try
{
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);

    if (rc != 0)                     // variable not found in any environment
    {
        symval = symval_S { };       // pass back empty symval_S
        return -1;                   // failure r/c
    }
    else                             // environment found with identifier
    {
        return get_variable(*p, ident, symval);    
    }
}
M_endf
      



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   update_local_var()    -- sets value of passed-in variable -- overwrites whatever is there in local  environment 
////   update_verbmain_var() -- sets value of passed-in variable -- overwrites whatever is there in static environment 
////   update_static_var()   -- sets value of passed-in variable -- overwrites whatever is there in static environment 
////   update_global_var()   -- sets value of passed-in variable -- overwrites whatever is there in global environment 
////   update_var()          -- sets value of passed-in variable -- overwrites whatever is there in local, verbmain, static, or global environment
////                                             
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int update_variable(environ_S& env, const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{
    // error if identifier is not already in the environment

    auto ct = env.symbols.count(ident);

    if (!is_identifier_defined(env, ident))
    {
        count_error(); 
        M_out_emsg(L"update_var() -- identifier \"%s\" to be updated is not defined") % ident; 
        return -1;  
    }


    // identifier exists in environment -- make sure it's not defined as const or verb or typdef

    symval_S symval { };                                      // local copy of symval from environ for this identifier
    get_identifier(env, ident, symval);                       // should not fail, since identifier is known to be defined in target environment


    if (symval.is_const)
    {
        count_error(); 
        M_out_emsg(L"update_var() -- cannot alter/redefine constant \"%s\" with new value") % ident; 
        return -1;
    }
   
    if (symval.is_verbset)  // ??????? allow verbset assigned to a variable to be updated -- just not a real verbset
    {
        count_error(); 
        M_out_emsg(L"update_var() -- cannot alter/redefine verb definition \"%s\" with new value") % ident; 
        return -1;
    }   

    if (symval.is_typdef)   // ??? allow typedef assigned to a variable to be updated -- just not a real typedef
    {
        count_error(); 
        M_out_emsg(L"update_var() -- cannot alter/redefine type definition (typedef) \"%s\" with new value") % ident; 
        return -1;
    }   


    // update associated value for this symval entry -- in place update -- don't change location  (value_sp should decrement use count and run destructors, etc.)    

    *(symval.value_sp) = value;                                


    // make sure any nested objects are unshared, if requested by caller
  
    if (unshare)                             
        unshare_value(*(symval.value_sp)); 


    // make this symval entry constant, if requested

    if (make_const)
        make_identifier_const(env, ident);  

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int update_local_var(frame_S& frame, const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{  
    // make sure current stack frame has an associated local enviromment

    if (frame.local_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"update_local_var() -- current stack frame has no associated local environment -- identifier = \"%S\" cannot be modified as a local variable in this stack frame") % ident;     
        return -1;  
    }

#if 0
    // make sure this identifier is not shadowed in an environment newer than the local environment

    if (is_ident_sf_shadowed(frame.local_sf_p, ident))  
    {                                                          
        count_error(); 
        M_out_emsg(L"update_local_var() -- local identifier = \"%S\" would be shadowed by saved invocation parms -- it cannot be updated from this stack frame") % ident; 
        return -1;       
    }
#endif

    return update_variable(*(frame.local_sf_p->environ_p), ident, value, make_const, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

// note: this routine is never called ???

int update_verbmain_var(frame_S& frame, const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{  
    // make sure current stack frame has an associated verbmain enviromment

    if (frame.verbmain_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"update_verbmain_var() -- current stack frame has no associated verbmain environment -- identifier = \"%S\" cannot be modified as a verbmain variable in this stack frame") % ident;     
        return -1;  
    }

#if 0
    // make sure this identifier is not shadowed in an environment newer than the verbmain environment

    if (is_ident_sf_shadowed(frame.verbmain_sf_p, ident))  
    {                                                           
        count_error(); 
        M_out_emsg(L"update_verbmain_var() -- verbmain identifier = \"%S\" would be shadowed in a local (non-static) environment -- it cannot be verbmain from this stack frame") % ident; 
        return -1;       
    }
#endif

    return update_variable(*(frame.verbmain_sf_p->environ_p), ident, value, make_const, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

// note: this routine is never called ???

int update_static_var(frame_S& frame, const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{  
    // make sure current stack frame has an associated static enviromment

    if (frame.persist_sf_p == nullptr)
    {
        count_error(); 
        M_out_emsg(L"update_static_var() -- current stack frame has no associated static environment -- identifier = \"%S\" cannot be modified as a static variable in this stack frame") % ident;     
        return -1;  
    }

#if 0
    // make sure this identifier is not shadowed in an environment newer than the static environment

    if (is_ident_sf_shadowed(frame.persist_sf_p, ident))  
    {                                                           
        count_error(); 
        M_out_emsg(L"update_static_var() -- static identifier = \"%S\" would be shadowed in a local (non-static) environment -- it cannot be updated from this stack frame") % ident; 
        return -1;       
    }
#endif

    return update_variable(*(frame.persist_sf_p->environ_p), ident, value, make_const, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

// note: this routine is never called ???

int update_global_var(const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{  
#if 0
    // make sure this identifier is not shadowed in a non-global environment

    if (is_global_ident_shadowed(ident))  
    {                                                         
        count_error(); 
        M_out_emsg(L"update_global_var() -- global identifier = \"%S\" is shadowed in a non-global environment -- it cannot be updated from this stack frame") % ident; 
        return -1;       
    }
#endif

    return update_variable(*get_global_environ(), ident, value, make_const, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////     

int update_var(frame_S& frame, const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{  
    environ_S *p {nullptr};
    auto rc = find_environ_ident(frame, ident, p);
                                                                     
    if (rc != 0)                                                        // identifier not found in any environment
        return -1;                                                      // failure r/c
    else                                                                // environment found with variable
        return update_variable(*p, ident, value, make_const, unshare);  // proper environment found -- update the variable in that environment
}
M_endf





//_________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""