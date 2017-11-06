// ex_preprocess.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                                                 unsigned_integer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =================
////     ex_preprocess.cpp -- options pre-parsing functions
////     =================
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
#include "h_ex_lex.h"
#include "h_ex_parse.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 
//    static variables 
//    ----------------

namespace static_N
{
    // preprocess statistics and counters

uint64_t                        pp_serial       {0};                  // serial number of most-recently allocated (or filled-in) pre_process_C item
uint64_t                        pp_depth        {0};                  // current number of pre_process_C items on stack 
uint64_t                        pp_max_depth    {0};                  // maximum number of pre_process_C items on stack (high-water mark) 

std::shared_ptr<pre_process_C>    newest_pp_sp  { };                  // owning pointer to newest pre_process_C on stack (keeps ref-count positive)  
}




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_new_pp() -- allocate new pre_process_C and place on top of stack  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

pre_process_C *add_new_pp()
{
    frame_S *pp_frame_p { nullptr };                                                                      // will be filled in with pointer to current/new PP stack frame 

    M__(M_out(L"add_new_pp() -- called -- pp_depth = %d   pp_max_depth = %d") % static_N::pp_depth % static_N::pp_depth;)


    // add newly-allocated (default-initialized) pre_process_C object to top of  stack  
                                                                                                             
    auto new_pp_sp              = std::make_shared<pre_process_C>();                                      // allocate new pp item
    new_pp_sp->m_upward_sp      = static_N::newest_pp_sp;                                                 // set new pp's upward stack pointer to current newest pp (owning ptr !!!!!!!!!!!!!!!!!!!!!)
    static_N::newest_pp_sp      = new_pp_sp;                                                              // update newest PP pointer to point to this one (owning ptr !!!!!!!!!!!!!!!!!!!!!!!!!!)  
                                                                                                        
    static_N::pp_depth++;                                                                                 // increment PP nesting depth
    if (static_N::pp_depth > static_N::pp_max_depth)                                                    
        static_N::pp_max_depth = static_N::pp_depth;                                                      // set new high-watermark 
                                                                                                        
                                                                                       
    // if this is top-level pp, add (static/main) PP frame to stack                                     
                                                                                                        
    if (static_N::pp_depth == 1)                                                                          // is this an un-nested preprocess_C?
    {
        M__(M_out(L"add_new_pp() -- top-level PP: calling add_frame()-- pp_depth = %d   pp_max_depth = %d") % static_N::pp_depth % static_N::pp_depth;)

        // point to main/static PP stack frame                                                              
                                                                                                        
        auto pp_frame_sp        = get_pp_frame();                                                         // locate/allocate preprocess stack frame
        pp_frame_p              = pp_frame_sp.get();                                                      // setup regular pointer
        new_pp_sp->m_remove_sf  = true;                                                                   // need to remove associated PP stack frame 


        // add main PP stack frame to top of stack  (PP frames have no upward scope into non PP frames, other than the global scope)  


        add_frame(pp_frame_sp);                                                                           // add static/only pp frame_S to top of stack (after currently-running function's SF, if any)  
     }


    // not top-level PP - if current top SF is non-preprocess SF, add new preprocessor SF with shared environment, etc. 
    // ----------------------------------------------------------------------------------------------------------------

    else
    {
        M__(M_out(L"add_new_pp() -- nested PP: pp_depth = %d   pp_max_depth = %d") % static_N::pp_depth % static_N::pp_depth;)
    
        auto newest_sf_p = get_newest_sf();                                                               // find current top SF

        if (newest_sf_p->is_preprocess)                                                                   // current top SF is a preprocess SF? 
        {
            pp_frame_p = newest_sf_p;                                                                     // will just use current newest SF for this PP instance
        }
        else                                                                                              // current top SF is not a preprocessor SF
        {    
            auto main_pp_sf_p = get_pp_frame().get();                                                     // convenience ptr to main/static PP SF


            // need to get new SF and add it to stack -- new preprocess SF will share the main PP environment

            pp_frame_p = add_new_frame();                                                                 // add new SF to top of stack (use this as stack frame for current PP instance)
            pp_frame_p->is_preprocess  = true;                                                            // flag this one as preprocessor SF
            pp_frame_p->no_scope       = true;                                                            // no access to current active function's (or main SF) variables
            pp_frame_p->scope_sp.reset();                                                                 // no upward scope pointer from here (only the global scope) 
            pp_frame_p->environ_p      = main_pp_sf_p->environ_p;                                         // point to environ in main/first PP stack frame
            pp_frame_p->shared_environ = true;                                                            // indicate this SF uses shared environ
            pp_frame_p->persist_sf_p   = nullptr;                                                         // no persistent SF for preprocessor SFs
            pp_frame_p->verbmain_sf_p  = nullptr;                                                         // no verb main SF ptr for preprocessor SFs
            new_pp_sp->m_remove_sf     = true;                                                            // need to remove associated PP stack frame when this PP instance ends 
        }      
    }


    // initialize/customize  new pre_process_C                                                   
    
    new_pp_sp->m_frame_p  = pp_frame_p;                                                                   // save pointer to PP stack frame in pre_process_C 
    new_pp_sp->m_serial   = ++static_N::pp_serial;                                                        // put new serial number in new preprocess_C 
    new_pp_sp->m_pp_depth = static_N::pp_depth;                                                           // capture PP depth, for debugging
                                                                                                       
    preprocess_setup(*(new_pp_sp.get()));                                                                 // initialize/customize new pre_process_C object    (must be done after m_upward_sp is filled in)    
     
    M__(display_stack(*pp_frame_p);)                                                                      // debugging stack display

    return new_pp_sp.get();                                                                               // pass back plain pointer to new pre_process_C object
}



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   remove_pp() -- remove (and free) newest pre_process_C object at top of  stack  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void remove_pp() try
{
    M__(M_out(L"remove_pp() -- called -- pp_depth = %d   pp_max_depth = %d") % static_N::pp_depth % static_N::pp_depth;)


    // remove PP stack frame from stack, one was allocated for this PP

    if (static_N::newest_pp_sp->m_remove_sf)
    {
        M__(M_out( L"remove_pp() -- calling remove_frame() -- pp_depth = %d   pp_max_depth = %d   SF use_count = %d")
                 % static_N::pp_depth % static_N::pp_depth % get_newest_sf()->self_wp.use_count();
           )
        get_pp_frame()->scope_sp.reset();                                                        // shouldn't normally cause de-allocation of parent SF (which should still be on the stack) 
        remove_frame();                                                                          // remove PP SF from stack 
        M__(M_out( L"remove_pp() -- remove_frame() returned -- pp_depth = %d   pp_max_depth = %d")
                 % static_N::pp_depth % static_N::pp_depth;
           )
    }
      
    
    // get rid of this pre_process_C

    static_N::newest_pp_sp = static_N::newest_pp_sp->m_upward_sp;                                // remove top pre_process_C from queue (should cause it to get freed, too)
    static_N::pp_depth--;                                                                        // decrement PP nesting depth
 
    return;                                                                                     
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   current_pp() -- return pointer current (latest) pre_process_C on stack  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

pre_process_C *current_pp() try
{
    return static_N::newest_pp_sp.get();                                                                                     
}
M_endf
            




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   preprocess_setup() -- set up preprocessing environment with custom configuration flags, etc.
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

uint64_t preprocess_setup(pre_process_C& pp) try
{
    // inherit skipping state from upward pre_process_C (if any)

    pp.inherit_state();


    // do additional preprocessor setup, with parse-oriented options 
                                                                             // note that ch_sigil_keyword and ch_sigil_label are the same character // 
    std::vector<char32_t> leading_sigils         { const_N::ch_sigil_verb,   const_N::ch_sigil_label,   const_N::ch_sigil_parm, const_N::ch_sigil_keyword  }; //  leading verb, parm, and label sigils (combined list)
    std::vector<char32_t> leading_ident_sigils   { const_N::ch_sigil_verb,   const_N::ch_sigil_label,   const_N::ch_sigil_parm                             }; //  leading verb, parm, and label sigils (for identifiers)
    std::vector<char32_t> leading_oper_sigils    {                                                      const_N::ch_sigil_parm                             }; //  leading       parm,           sigil  (for operators)
    std::vector<char32_t> trailing_ident_sigils  { const_N::ch_sigil_keyword                                                                               }; //  trailing keyword sigils              (for identifiers)
    std::vector<char32_t> paren_sigils           { const_N::ch_sigil_verb,   const_N::ch_sigil_keyword                                                     }; //  leading/trailing verb sigil          (for parenthesis)
                                                                                                                                                           
    pp.set_combine_strings(        false                );                                                                                                    // don't combine adjacent strings separated by whitespace    
    pp.set_allow_attached_paren(   true                 );                                                                                                    // allow attached parens to identifier tokens

    pp.set_leading_sigils(                leading_sigils );                                                                                                   // add combined list of leading sigils          (in addition to any preprocess sigils)
    pp.set_leading_ident_sigils(    leading_ident_sigils );                                                                                                   // add list of leading sigils  for identifiers  (in addition to any preprocess sigils)
    pp.set_trailing_ident_sigils(  trailing_ident_sigils );                                                                                                   // add list of trailing sigils for identifiers  (in addition to any preprocess sigils)
    pp.set_leading_oper_sigils(      leading_oper_sigils );                                                                                                   // add list of leading sigils  for operators    (in addition to any preprocess sigils)
    pp.set_paren_sigils(                    paren_sigils );                                                                                                   // add list of sigils for parenthesis           (in addition to any preprocess sigils)
    
    return error_count(); 
}
M_endf







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   pre_process_C() -- default constructor 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

pre_process_C::pre_process_C() try
{
    M__(M_out(L"Pre_parse_C default constructor called");)


    // set up default imbed folder from IMBED_PATH envar, if set
    // ---------------------------------------------------------

    set_imbed_folder(); 


    // set up required configuration parms for the token_stream
    // --------------------------------------------------------

    m_token_stream.set_eol_is_whitespace(     true ); 
    m_token_stream.set_eof_is_whitespace(     true ); 
    m_token_stream.set_comment_is_whitespace( true ); 
    m_token_stream.set_combine_whitespace(    true ); 
    m_token_stream.set_combine_strings(       true ); 
    m_token_stream.set_skip_whitespace(       true );
 
    return;
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   inherit_state() -- inhert skipping state upwardpp
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_process_C::inherit_state() try
{
    // inherit skipping state from 2 generations upwrd -- upward pre_process_C (if any)

    if (m_upward_sp.get() != nullptr) 
    {
        m_inherited_skipping = m_upward_sp->m_skipping;            // indicate parent PP is skipping (but not this one, for now)
        m_skipto_label       = m_upward_sp->m_skipto_label;
    }
    return;  
}
M_endf
           


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   close() -- free up everything and reset state -- leave configuration and statistics intact 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_process_C::close() try
{
    M__(M_out(L"Pre_parse_C::close() -- called");)

    m_token_stream.close();      // close out token_stream() 


    // reset state

    m_token_stack.clear(); 
    m_skipto_label.clear();
    m_skipping = false; 

    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   reuse() -- clean up leftovers and reset state -- leave configuration and statistics intact 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_process_C::reuse(bool refresh) try
{
    M__(M_out(L"Pre_parse_C::reuse() -- called");)


    // leave token stream open, but do optional refresh()

    if (refresh)
       m_token_stream.refresh();


    // leave pre_process_C symbol_table intact for reuse

    // reset pre_process_C state for reuse 

    m_token_stack.clear(); 
    m_skipto_label.clear();
    m_skipping = false; 

    return; 
}
M_endf







////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_settings() -- displays selected fields in the pre_process_C structure 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_process_C::display_settings() try
{
    //M_out(L"---------------------------------------------------------------------------------------");
    M_out(L"pre_process_C::m_imbed folder         = [%S]") % m_imbed_folder;
    M_out(L"pre_process_C::m_skipping             = %S"  ) % M_bool_cstr(m_skipping);
    M_out(L"pre_process_C::m_inherited_skipping   = %S"  ) % M_bool_cstr(m_inherited_skipping);
    M_out(L"pre_process_C::m_skipto label         = [%S]") % m_skipto_label;
    //M_out(L"---------------------------------------------------------------------------------------");

    return; 
}
M_endf   



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////  peek_raw_token() -- peek next token from token_stream, and add user type field  
////  ================      (consume invalid preprocess tokens, with error message)
////
////
////      only errors, passback tokens, and valid preprocess tokens get beyond this screening routine
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int pre_process_C::peek_raw_token(token_C& t, size_t peek_level)
{
    M__(M_out(L"pre_process_C::peek_raw_token():1 called <------------------------");)

    token_C token; 


    // main loop to look for a token to be passed back -- invalid preprocess tokens are consumed -- loop ends via function return only 

    for(;;)       
    {
        auto prc = m_token_stream.peek_token(token, peek_level);
        M__(token.display(L"pre_process_C::peek_raw_token() -- token_stream_C::peek_token() returned");)
        if (prc != 0)
        {
               M_out_emsg(L"pre_process_C::peek_raw_token():2 -- error R/C from peek_token() -- returning immediately");
               t = token;       // probably invalid
               count_error();
               return -1;
        }
     

        // special handling for END token from token_stream

        if (token.type == token_E::end)
        {
             token.utype1 = tok_u1_E::end;                           // preprocess end token
             t = token; 
             M__(t.display(L"pre_process_C::peek_raw_token() END:");)
             return 0;
        }

        
        // pass back retained comments

        if  ( (token.type == token_E::retained_line_comment) || (token.type == token_E::retained_block_comment) )
        {
            M__(M_out(L"pre_process_C::peek_raw_token():2 -- processing retained comment");)   
            token.utype1 = tok_u1_E::evaluate;                      // text for evaluation by pre-processor
            t = token; 
            M__(t.display(L"pre_process_C::peek_raw_token() evaluate:");)
            return 0;
        }          


        // provisionally accept no-sigil identifiers and quoted strings -- may get passed back later, if they are of no interest to preprocess
      
        if ( (!token.has_leading_sigil) && (!token.has_trailing_sigil) )
        {
            if ( (token.type == token_E::identifier) || (token.type == token_E::extended_identifier) )
            {
                token.utype1 = tok_u1_E::identifier;
            }
            else
            {
              if (token.type == token_E::string) 
                token.utype1 = tok_u1_E::string;
              else
                token.utype1 = tok_u1_E::passback;
            }
            t = token; 
            M__(t.display(L"peek_raw_token():");)
            return 0; 
        }



        // other, non-preprocess token (except for END) -- has trailing sigil only, or has non-preprocess leading sigil -- may be invalid, error, etc.

        token.utype1 = tok_u1_E::passback; 
        t = token;
        M__(t.display(L"pre_process_C::peek_raw_token():");)
        return 0;

    }  // end of main for(;;) loop


    M_throw("pre_process_C::peek_raw_token(): main loop fell through")
    return 0;    // should not get here
}





////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   _token() -- handle any preprocess tokens -- end when 1st non-preprocess token is seen (passed back in parm) 
////                        passed-back token is not consumed
////
////
////   rc = 0  -- normal token passed back
////   rc = -1 -- error
////   rc = 1  -- simulated token_E::end token from @END special results processing is passed back
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int pre_process_C::_token(token_C& t, size_t peek_level) try
{
    M__(M_out(L"_token() called <-------------------------- serial = %d  depth = %d -- skipping = %d  inh_skipping = %d") % m_serial % m_pp_depth % m_skipping % m_inherited_skipping;)

    // =========    ------------------------------------------------- 
    // main loop -- ends when preprocess finds something to pass back
    // =========    ------------------------------------------------- 

    for(;;)
    {
        // peek() at next pre-substituted,classified token 
        // -----------------------------------------------
      
        token_C token { };
        auto prc = peek_raw_token(token, peek_level);
        M__(M_out(L"_token() peek_raw_token() returned -------------------------->");)
        M__(token.display(L"pp token");)
        M__(M_out(L"<-------------------------------");)
      

        // =======================
        // peek_raw_token() failed -- return with error from peek_raw_token()
        // =======================
                        
        if (prc != 0)    
        {       
            M__(M_out(L"_token(): error R/C from token_stream_C::peek_raw_token()");)
            m_skipping = false;                           // error ends any active skipping mode 
            t          = token;                           // might be a filled-in token (or not) -- don't consume token
            count_error(); 
            return -1;                                    // return immediately -- lower routines should have done count_error()                         
        }


        // ===================
        // peek_raw_token() OK -- need to process token
        // ===================


        // special handling for END tokens, in skipping mode
        // -------------------------------------------------

        if (token.utype1 == tok_u1_E::end)                // END token? 
        {            
            if (m_skipping && !m_inherited_skipping)
            {
                m_skipping = false;
                M_out_emsg(L"pre_process_C::_token():  end of input reached while skipping to label = \"%s\"") % m_skipto_label;                      
                count_error();
            }
            t = token; 
            m_token_stream.discard_token(peek_level);     //   consume the END token before returning
            return 0; 
        }


        // pass back non-preprocess tokens (if not skipping)
        // -------------------------------

        if (
             (token.utype1 == tok_u1_E::passback  )        // non-preprocess token 
             ||
             (token.utype1 == tok_u1_E::string    )        // string 
             ||
             (token.utype1 == tok_u1_E::identifier)        // identifier 
           )
        {
            if (!m_skipping) 
            {
                m_token_stream.discard_token(peek_level);  //   consume token before returning
                t = token; 
                return 0; 
            }
            else
            {   
                m_token_stream.discard_token(peek_level);  // consume token before looping back
                continue;                                  // loop back to process next token   
            }
        }

                    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //        process retained comments (now converted to tok_u1_E::evaluate tokens)
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
        if  (token.utype1 == tok_u1_E::evaluate)
        {
            M__(M_out(L"pre_process_C::_token():1 -- processing evaluate token");)   
      

            // bypass both parsing and evaluating block if upward PP is in skipping mode
                
            if (m_inherited_skipping)
            {
                m_token_stream.discard_token(peek_level);    
                continue;  
            }


            // parse the retained comment, even in local skipping mode (to check for matching @SKIPTO target labels)
            
            std::wstring info { L"preprocesssor text - imbedded from <<<"};
            info += token.loc_str();  
            info += L">>>  position in preprocessor text";
            
            block_S out_block{ };                                                                                                 // block_S to be filled in by parse_string()  
            auto src = parse_string(*m_frame_p, out_block, token.str, info, false);                                               // don't continue after error
            m_token_stream.discard_token(peek_level);                                                                             // always consume token after parsing


            // handle error results from parse phase

            if (src != 0)                                                                                                         // return with error, if parse_string() failed
            {
                M_out_emsg(L"pre_process_C::_token() -- parse_string() failed for pre-processor text");
                token.display();
                M_out_emsg2(L"    pre-processing ends"); 
                count_error(); 
                return -1;    
            }  


            // need to check main (first token) label in block -- end skipping mode if match

            if (m_skipping)  
            {
                M__(M_out(L"pre_process_C::_token() -- serial= %d  --  M_skipto_label = \"%S\" -- out_block.label = \"%S\"") % m_serial % m_skipto_label % out_block.label;) 
                if (m_skipto_label == out_block.label)              // @SKIPTO target label is at start of this retained comment? tar
                {
                    m_skipping = false;                             // end skipping mode 
                    m_skipto_label.clear() ; 
                }   
            }
             

            // bypass evaluating block resulting from the parse, if still in skipping mode, or upward PP is in skipping mode
                
            if (m_skipping)
                continue;                                                                                                         // loop back to process next token 


            // not in skipping mode -- evaluate block from the retained comment

            results_S out_results {}; 
    
            M__(M_out(L"pre_process_C::_token()  -- calling eval_block() ************************");)
            auto erc = eval_block(*m_frame_p, out_block, out_results);                                                        // results (with any special flags) will be cause preprocess error
            M__(M_out(L"pre_process_C::_token()  -- eval_block() returned -- rc=%d *****************") % erc;)  
                       
             
            // return immediately, if error returned by eval_block()
            
            if (erc != 0)
            {
                M__(M_out(L"pre_process_C::_token() -- eval_block() returned error");)
                M_out_emsg1(L"pre_process_C::_token() -- eval_block() failed for preprocessor text");
                token.display();
                M_out_emsg2(L"    pre-processing ends"); 
                count_error(); 
                return -1;
            }


            // return immediately, if unexpected special results (like @GOTO) returned by eval_block())
            // ----------------------------------------------------------------------------------------
                 
            if (out_results.special_results)
            {
                if (out_results.end_flag)
                {
                    close();                                    // should free up everything
                    token.type               = token_E::end;
                    token.has_leading_sigil  = false;           // no complaints from token_C::display()
                    token.has_trailing_sigil = false; 
                    t                        = token;           // pass back token (consumed already) 
                    return 1;                                   // special R/C to indicate token is already consumed -- passing back simulated END token
                }
                else if (out_results.skip_flag)
                {
                                                                // skip results are expected here, as normal exit from retained comment
                }
                else                                            // not @END or @SKIP/@SKIPTO -- unexpected special results
                {                               
                    M_out_emsg1(L"pre_process_C::_token() -- eval_block() for preprocessor text returned unexpected special results: %S") % results_msg_string(out_results); 
                    token.display();
                    M_out_emsg2(L"    pre-processing ends"); 
                    count_error(); 
                    return -1;
                }
            }
                        

            // process any filenames/strings on the pending_attach_stack
            // ---------------------------------------------------------

            while (m_pending_attach_stack.size() > 0)
            {
                auto pend = m_pending_attach_stack.top();
                m_pending_attach_stack.pop();

                if (pend.is_file)
                {
                    M__(M_out(L"pre_process_C::_token() -- file from m_pending_attach_stack = \"%S\"") % pend.name;) 
                    auto arc = attach_file(pend.name);

                    if (arc != 0)
                    {
                        M_out_emsg(L"pre_process_C::_token() -- attach_file() failed, trying to attach file \"%S\" requested by pre-processor text") % pend.name;
                        token.display();
                        M_out_emsg2(L"    pre-processing ends"); 
                        count_error(); 
                        return -1;    
                    } 
                }
                else                  // must be attaching a string
                {
                    M__(M_out(L"pre_process_C::_token() -- string ID from m_pending_attach_stack = \"%S\"") % pend.name;) 
                    auto arc = attach_string(pend.text, pend.name);

                    if (arc != 0)
                    {
                        M_out_emsg(L"pre_process_C::_token() -- attach_string() failed, trying to attach string ID \"%S\" requested by pre-processor text") % pend.name;
                        token.display();
                        M_out_emsg2(L"    pre-processing ends"); 
                        count_error(); 
                        return -1;    
                    } 
                }
            }


            // normal results (or @SKIP) from eval_block() -- just keep looping

            continue; 

        }   // retained comment for preprocess evaluation

 

          // ----------------
    }     // end of main loop
          // ----------------

    return 0;       // should not get here
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_token() -- get next preprocessed token 
////                   
////
////   rc = 0  -- normal token passed back
////   rc = -1 -- error
////   rc = 1  -- simulated token_E::end token from ?END processing is passed back (1st time only, not after putback) 
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
  
int pre_process_C::get_token(token_C& token) try               // get (and consume) next token from stream  
{
    // pass back top put-back token (if any on stack)

    if (!m_token_stack.empty())
    {
        token = m_token_stack.front();                          // should have been fileld in before is was put back   
        m_token_stack.pop_front();                              // consume token 

        if (token.type == token_E::error)
            return -1;                                        // if returning prior error, return with error R/C
        else
            return 0;                                         // else returned char is OK                   
    } /// might need to return 1 here, if utype is verb???????????????????????????????????????????
    

    // token putback stack is empty -- need to get next token from the token stream

    int rc = _token(token);                         // get next token from the input token stream
    return rc; 
}
M_endf 



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////    peek_token() -- return next (or n-th next) token without consuming it -- ends up on putback stack (or is left on putback stack)  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
  
int pre_process_C::peek_token(token_C& token, size_t n) try              // get (and consume) next character from stream  
{
    M__(M_out(L"pre_process_C::peek_token(%d) -- called -- M_token_stack.size() = %d") % n % m_token_stack.size();)


    // make sure 'n' is greater than 0 

    if (n <= 0)
    {
        M_out_emsg(L"pre_process_C::peek_token(token, %u) called with bad char position parm (%)") % n % n;
        return -1; 
    }


    // pass back requested token, if it is on the existing putback stack -- oldest are at start [0], so n-th oldest has index [n-1]  

    size_t sz = m_token_stack.size();                            // number of put-back tokens on stack 

    if (sz >= n)
    {
        m_token_stack.at(n-1).peek_count ++;                     // increment peek counter
        token = m_token_stack.at(n-1);                           // get n-th oldest (composite) token  
           
        M__(M_out(L"pre_process_C::peek_token() returning token from stack -------------------------------------->");)
        M__(token.display();)
        M__(M_out(L"<-------------------------------------------");)


        // leave token on stack

        if (token.type == token_E::error)
            return -1;                                           // if returning prior error, return with error R/C
        else
            return 0;                                            // else returned token is OK             
    }                                                            // do we need to pass back R/C = 1 here for simulated end????
       

    // Loop to add enough tokens to new end of putback stack, so that n-th oldest token is on the putback stack
    // --------------------------------------------------------------------------------------------------------
    
    int     rc  {0}; 
    token_C tok {}; 

    for (auto i = 0;  i < (n - sz); ++i)
    {
        int rci = _token(tok);                          // get next composite token     
        if (rci < 0) rc = rci;                                   // remember any bad R/C   -- do we need to handle +1 R/C too ??????????????????????????????
        m_token_stack.push_back(tok);                            // add newest composite token to back end of putback stack
    }

    if (m_token_stack.size() != n)
    {
        M_out_emsg(L"pre_process_C::peek_token(token, %u) -- unexpected number of elements (%u) on putback stack") % n % m_token_stack.size();
        rc = -1;
    }


    //  Desired token should be at the back of the stack now -- return it but leave it on the stack 

    m_token_stack.back().peek_count ++;                          // increment peek counter
    token = m_token_stack.back();                                // passing back last (newest) token on stack -- should be the right one, if R/C is 0  
    M__(M_out(L"pre_process_C::peek_token() ---------------------------------------------------->");)
    M__(token.display();)
    M__(M_out(L"<------------------------------------");)
    

    M__(M_out(L"pre_process_C::peek_token(%d) -- returning -- r/c = %d") % n % rc;)
    return rc; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   putback_token() -- putback last preprocessed token onto token stack -- returned from get_token(), or earlier peek()
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_process_C::putback_token(const token_C& token) try      // putback token to stream -- no need to be connected to a file
{
    m_token_stack.push_front(token);                           // put at oldest position on stack   
    return;
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   discard_token() -- discard n-th token on putback/peek queue -- 1 means top element
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int pre_process_C::discard_token(size_t n) try                      
{
     // discard top or n-th put-back character (if any)

     if ( (n > 0) && (m_token_stack.size() >= n) )
     {
         if (n == 1)
         {
             m_token_stack.pop_front();                               // remove oldest composite token on putback queue 
         }
         else
         {
             auto it = m_token_stack.begin();                         
             m_token_stack.erase(it + (n - 1));                       // remove n-th oldest composite token on putback queue         
         }
         return 0;                                                       
     }
      

     // n is bad, or not enough characters on putback queue -- don't do anything -- R/C is -1 
 
     return -1;     
}
M_endf 



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_imbed_folder()  -- set base folder for imbedded/included files
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


// form with no parms -- set default imbed path using environment variable (if set)
// --------------------------------------------------------------------------------

void pre_process_C::set_imbed_folder() try    
{
    if ( is_env_set(IMBED_PATH_ENVAR) )
        m_imbed_folder = get_env(IMBED_PATH_ENVAR);
    else
        m_imbed_folder = std::wstring { IMBED_PATH };  
}
M_endf



// form with passed-in string
// --------------------------

void pre_process_C::set_imbed_folder(const std::wstring& i_f) try    
{
    m_imbed_folder = i_f; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_skip() -- set pre-processor skipping mode (with target label)
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_process_C::set_skip(const std::wstring& to_label) try    
{
    m_skipto_label = to_label;           // don't bother checking to see if we are already skipping
    m_skipping     = true; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   attach_file() -- validate requested input file and attach it to the token stream 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


int pre_process_C::attach_file(const std::wstring& wn) try
{
    std::wstring final_pathname { }; 


    // get final filename for imbed -- prepend m_imbed_folder, if passed-in filename appears not to be an absolute pathname 

    if ( (wn.size() >= 1) && ((wn.at(0) == L'\\') || (wn.at(0) == L'/')) )
        final_pathname = wn; 

    else if ( (wn.size() >= 2) &&  (wn.at(1) == L':' ) ) 
        final_pathname = wn; 
    else
        final_pathname = m_imbed_folder + wn; 
    

    // attach this file to the token stream -=- complain if errors passed back

    api_err_S    err  { }; 
    auto arc = m_token_stream.attach_file(final_pathname, err);  

    if ( (arc != 0) || (err.error_occurred) )
    {
        M_out_emsg(L"pre_process_C::attach_file() -- input file \"%S\" cannot be used -- error text = <<%S>>") % wn % err.error_text;     
        return -1; 
    }  

   // arc = m_token_stream.attach_file(m_imbed_folder + L"test_attach1.txt", err);  
   // arc = m_token_stream.attach_file(m_imbed_folder + L"test_attach2.txt", err);  
   // arc = m_token_stream.attach_file(m_imbed_folder + L"test_attach3.txt", err);    

    return 0;                            // attach is OK
}
M_endf






////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_pending_attach() -- add filename/string to pending attach stack (at front) 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
                 
// version that adds a file to the pending attach stack
// ----------------------------------------------------

void pre_process_C::add_pending_attach(const std::wstring& filename) try
{
    // add this filename to front of pending attach stack (order of attach()ed file reading will be reversed when attach()es are actioned later

    pending_attach_S pend { };
    pend.name      = filename;
    pend.is_file   = true;

    m_pending_attach_stack.push(pend);  
    return;      
}
M_endf


// version that adds a string to the pending attach stack
// ------------------------------------------------------

void pre_process_C::add_pending_attach(const std::wstring& text, const std::wstring& id) try
{
    // add this string to front of pending attach stack (order of attach()ed string processing will be reversed when attach()es are actioned later

    pending_attach_S pend { };
    pend.text      = text;
    pend.name      = id;
    pend.is_file   = false;

    m_pending_attach_stack.push(pend);  
    return;      
}
M_endf
  


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   passthru functions to token_stream_C 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int pre_process_C::attach_string(const std::wstring& ws, const std::wstring& wn) try
{
    return m_token_stream.attach_string(ws, wn);
}
M_endf


int pre_process_C::attach_string(const std::string& s, const std::wstring& wn) try
{
    return m_token_stream.attach_string(s, wn);
}
M_endf


int pre_process_C::attach_string(const std::string& s, const std::string& n) try
{
    return m_token_stream.attach_string(s, n);
}
M_endf

 

// query functions

bool     pre_process_C::is_empty(    ) const try { return              m_token_stream.is_empty(    ) ; }  M_endf
size_t   pre_process_C::putback_size() const try { return              m_token_stream.putback_size() ; }  M_endf


// subset of token_string_C configuration functions (other setings are required by preprocess)

void pre_process_C::set_quiet_mode(                      bool     tf           ) try { return m_token_stream.set_quiet_mode(                           tf  );      }  M_endf
void pre_process_C::set_suppress_echo(                   bool     tf           ) try { return m_token_stream.set_suppress_echo(                        tf  );      }  M_endf
void pre_process_C::set_combine_strings(                 bool     tf           ) try { return m_token_stream.set_combine_strings(                      tf  );      }  M_endf
void pre_process_C::set_always_attach_plus_minus(        bool     tf           ) try { return m_token_stream.set_always_attach_plus_minus(             tf  );      }  M_endf
void pre_process_C::set_never_attach_plus_minus(         bool     tf           ) try { return m_token_stream.set_never_attach_plus_minus(              tf  );      }  M_endf
void pre_process_C::set_allow_leading_op_sigils(         bool     tf           ) try { return m_token_stream.set_allow_leading_op_sigils(              tf  );      }  M_endf
void pre_process_C::set_allow_trailing_op_sigils(        bool     tf           ) try { return m_token_stream.set_allow_trailing_op_sigils(             tf  );      }  M_endf
void pre_process_C::set_allow_leading_id_sigils(         bool     tf           ) try { return m_token_stream.set_allow_leading_id_sigils(              tf  );      }  M_endf
void pre_process_C::set_allow_trailing_id_sigils(        bool     tf           ) try { return m_token_stream.set_allow_trailing_id_sigils(             tf  );      }  M_endf
void pre_process_C::set_allow_paren_sigils(              bool     tf           ) try { return m_token_stream.set_allow_paren_sigils(                   tf  );      }  M_endf
void pre_process_C::set_allow_attached_paren(            bool     tf           ) try { return m_token_stream.set_allow_attached_paren(                 tf  );      }  M_endf
                                                       
void pre_process_C::set_trigraph_char(                   char32_t ch32         ) try { return m_token_stream.set_trigraph_char(                        ch32);      }  M_endf
void pre_process_C::set_vanishing_separator_char(        char32_t ch32         ) try { return m_token_stream.set_vanishing_separator_char(             ch32);      }  M_endf  
void pre_process_C::set_line_continuation_char(          char32_t ch32         ) try { return m_token_stream.set_line_continuation_char(               ch32);      }  M_endf
void pre_process_C::set_always_sign_char(                char32_t ch32         ) try { return m_token_stream.set_always_sign_char(                     ch32);      }  M_endf

void pre_process_C::set_type1_string_start_char(         char32_t ch32         ) try { return m_token_stream.set_type1_string_start_char(              ch32);      }  M_endf
void pre_process_C::set_type2_string_start_char(         char32_t ch32         ) try { return m_token_stream.set_type2_string_start_char(              ch32);      }  M_endf
void pre_process_C::set_type1_string_end_char(           char32_t ch32         ) try { return m_token_stream.set_type1_string_end_char(                ch32);      }  M_endf
void pre_process_C::set_type2_string_end_char(           char32_t ch32         ) try { return m_token_stream.set_type2_string_end_char(                ch32);      }  M_endf
void pre_process_C::set_type1_string_escape_char(        char32_t ch32         ) try { return m_token_stream.set_type1_string_escape_char(             ch32);      }  M_endf
void pre_process_C::set_type2_string_escape_char(        char32_t ch32         ) try { return m_token_stream.set_type2_string_escape_char(             ch32);      }  M_endf
void pre_process_C::set_raw_string_prefix_char(          char32_t ch32         ) try { return m_token_stream.set_raw_string_prefix_char(               ch32);      }  M_endf
void pre_process_C::set_multiline_string_prefix_char(    char32_t ch32         ) try { return m_token_stream.set_multiline_string_prefix_char(         ch32);      }  M_endf
void pre_process_C::set_word_string_start_char(          char32_t ch32         ) try { return m_token_stream.set_word_string_start_char(               ch32);      }  M_endf
 
void pre_process_C::set_unechoed_line_comment_char(      char32_t ch32         ) try { return m_token_stream.set_unechoed_line_comment_char(           ch32);      }  M_endf
void pre_process_C::set_echoed_line_comment_char(        char32_t ch32         ) try { return m_token_stream.set_echoed_line_comment_char(             ch32);      }  M_endf
                                                       
void pre_process_C::set_comment_1st_char(                char32_t ch32         ) try { return m_token_stream.set_comment_1st_char(                     ch32);      }  M_endf       
void pre_process_C::set_unechoed_line_comment_2nd_char(  char32_t ch32         ) try { return m_token_stream.set_unechoed_line_comment_2nd_char(       ch32);      }  M_endf       
void pre_process_C::set_echoed_line_comment_2nd_char(    char32_t ch32         ) try { return m_token_stream.set_echoed_line_comment_2nd_char(         ch32);      }  M_endf       
void pre_process_C::set_suppress_eol_comment_2nd_char(   char32_t ch32         ) try { return m_token_stream.set_suppress_eol_comment_2nd_char(        ch32);      }  M_endf       
void pre_process_C::set_eof_comment_2nd_char(            char32_t ch32         ) try { return m_token_stream.set_eof_comment_2nd_char(                 ch32);      }  M_endf       
void pre_process_C::set_retained_line_comment_2nd_char(  char32_t ch32         ) try { return m_token_stream.set_retained_line_comment_2nd_char(       ch32);      }  M_endf       
                                                                                           
void pre_process_C::set_block_comment_2nd_char(          char32_t ch32         ) try { return m_token_stream.set_block_comment_2nd_char(               ch32);      }  M_endf       
void pre_process_C::set_block_comment_3rd_char(          char32_t ch32         ) try { return m_token_stream.set_block_comment_3rd_char(               ch32);      }  M_endf       
void pre_process_C::set_block_comment_4th_char(          char32_t ch32         ) try { return m_token_stream.set_block_comment_4th_char(               ch32);      }  M_endf       
void pre_process_C::set_nest_comment_2nd_char(           char32_t ch32         ) try { return m_token_stream.set_nest_comment_2nd_char(                ch32);      }  M_endf       
void pre_process_C::set_nest_comment_3rd_char(           char32_t ch32         ) try { return m_token_stream.set_nest_comment_3rd_char(                ch32);      }  M_endf       
void pre_process_C::set_nest_comment_4th_char(           char32_t ch32         ) try { return m_token_stream.set_nest_comment_4th_char(                ch32);      }  M_endf       
void pre_process_C::set_retained_block_comment_2nd_char( char32_t ch32         ) try { return m_token_stream.set_retained_block_comment_2nd_char(      ch32);      }  M_endf       
void pre_process_C::set_retained_block_comment_3rd_char( char32_t ch32         ) try { return m_token_stream.set_retained_block_comment_3rd_char(      ch32);      }  M_endf       
void pre_process_C::set_retained_block_comment_4th_char( char32_t ch32         ) try { return m_token_stream.set_retained_block_comment_4th_char(      ch32);      }  M_endf        
                                                                                                                                                     





///// ----------------------------------------------------------------------------------------------------------------                                                                                                                      
///// modified configuration functions (certain sigils are needed by preprocess itself, in addition to caller's sigils)
///// ----------------------------------------------------------------------------------------------------------------

void pre_process_C::set_leading_sigils(const std::vector<char32_t>& ls) try 
{
    std::vector<char32_t> leading_sigils = ls; 
   
    return m_token_stream.set_leading_sigils(leading_sigils); 
}
M_endf


void pre_process_C::set_leading_ident_sigils(const std::vector<char32_t>& ls) try 
{
    std::vector<char32_t> leading_sigils = ls; 
                                                                                                                              
    if (leading_sigils.size() > 0)
        m_token_stream.set_allow_leading_id_sigils(true );               // need to allow leading identifier sigils
    else
        m_token_stream.set_allow_leading_id_sigils(false);               // don't allow leading identifier sigils 


    return m_token_stream.set_leading_ident_sigils(leading_sigils); 
}
M_endf


void pre_process_C::set_leading_oper_sigils(const std::vector<char32_t>& leading_sigils) try 
{
    if (leading_sigils.size() > 0)
        m_token_stream.set_allow_leading_op_sigils(true );               // need to allow leading operator sigils
    else
        m_token_stream.set_allow_leading_op_sigils(false);               // don't allow leading operator sigils 


    return m_token_stream.set_leading_oper_sigils(leading_sigils); 
}
M_endf



void pre_process_C::set_trailing_ident_sigils(const std::vector<char32_t>& ts) try 
{
    std::vector<char32_t> trailing_sigils = ts; 
    
    if (trailing_sigils.size() > 0)
        m_token_stream.set_allow_trailing_id_sigils(true );                   // need to allow trailing identifier sigils
    else
        m_token_stream.set_allow_trailing_id_sigils(false);                   // don't allow trailing identifier sigils 

    return m_token_stream.set_trailing_ident_sigils(trailing_sigils); 
}
M_endf



void pre_process_C::set_trailing_oper_sigils(const std::vector<char32_t>& trailing_sigils) try 
{
    if (trailing_sigils.size() > 0)
        m_token_stream.set_allow_trailing_op_sigils(true );                   // need to allow trailing operator sigils
    else
        m_token_stream.set_allow_trailing_op_sigils(false);                   // don't allow trailing operator sigils 


    return m_token_stream.set_trailing_oper_sigils(trailing_sigils); 
}
M_endf



void pre_process_C::set_paren_sigils(const std::vector<char32_t>& sigils) try 
{
    if (sigils.size() > 0)
        m_token_stream.set_allow_paren_sigils(true );                         // need to allow paren sigils
    else                                                                  
        m_token_stream.set_allow_paren_sigils(false);                         // don't allow paren sigils 


    return m_token_stream.set_paren_sigils(sigils); 
}
M_endf






//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""