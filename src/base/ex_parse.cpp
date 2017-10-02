// ex_parse.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ============
////     ex_parse.cpp -- input stream parsing functions
////     ============
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
#include "h_ex_lex.h"
#include "h_ex_parse.h"
#include "h_ex_verb.h"


           
// ------------------
// internal functions
// ------------------

///////////////////// slist_S-oriented internal functions

static int     get_slist(          frame_S&, pre_parse_C&, slist_S&, bool, bool );
static int     get_unnested_slist( frame_S&, pre_parse_C&, slist_S&             );
static int     get_nested_slist(   frame_S&, pre_parse_C&, slist_S&             );


///////////////////// a_expression_S-oriented  internalfunctions

static int     get_verb_priority(frame_S&, const std::wstring&); 
static void    expression_add_token(           a_expression_S&, const token_C&);

static int     get_expression(                 frame_S&, pre_parse_C&, a_expression_S&, bool, bool, bool, bool );
static int     get_unnested_expression(        frame_S&, pre_parse_C&, a_expression_S&, bool = true            );
static int     get_nested_expression(          frame_S&, pre_parse_C&, a_expression_S&                         );
static int     get_nested_verbname_expression( frame_S&, pre_parse_C&, a_expression_S&                         );
static int     get_nested_kwname_expression(   frame_S&, pre_parse_C&, a_expression_S&                         );


///////////////////// vlist_S-oriented internal functions 

static int     get_vlist_value(   frame_S&, pre_parse_C&, a_expression_S&, vlist_S&, const token_C&);
static int     get_vlist(         frame_S&, pre_parse_C&, a_expression_S&, vlist_S&                );
static int     get_nested_vlist(  frame_S&, pre_parse_C&, a_expression_S&, vlist_S&                );


//////////////////////  value_S-oriented internal functions

static void    set_atom_token( value_S&, const token_C&, int64_t = -1);

static int     get_keyword(frame_S&,        pre_parse_C&, a_expression_S&, vlist_S&);


////////////////////////////////// parse-oriented internal functions

static int     peek_token(   pre_parse_C&, token_C&, bool = true);
static void    discard_token(pre_parse_C&                       );


////////////////////////////////// location string message

static std::wstring value_loc_str(int64_t, int64_t = -1);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// ----------------------
// local static variables
// ----------------------

namespace static_N
{
static int64_t                                token_ix         {-1   };     // will be incremented before each token is added  
static std::vector<token_C>                   token_list       {     };     // list of tokens -- may contain duplicates if a token is added to an expression more than once
static bool                                   just_peeked      {false};     // 1st time, peek gets a new token
}


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
////   process_cmdline() -- set up preparse_C based on command line parms  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
std::wstring process_cmdline(frame_S& frame, symtab_S& st, int argc, wchar_t *argv[]) try
{
    // set up pre-defined pre-processor constants from command line arguments
    // ----------------------------------------------------------------------

    //? this should come out ????

  //  for (auto i = 0; i < argc; i++) 
  //      pp.set_var(fmt_str(L"_%d", i), argv[i], true);    // set pre-defined pre-processor constants _nn from n-th arg on command line -- _0    _1    _2    _3   etc. 
    

    // set up topmost (main) stack frame with command line arguments
    // -------------------------------------------------------------

    frame_parms(frame, argc, argv);                      // set up main stack frame with command line args    
    M__(pp.display_all_vars();)


    //  determine name of main (top-level) imbed file
    //  ---------------------------------------------

    std::wstring imbed_filename { };
    
    if (argc > 1)        // 1st argument present?   
        imbed_filename = argv[1];
   

    M__(M_out(L"imbed_filename = \"%s\"") % imbed_filename;)

    if (imbed_filename.size() == 0)
    {
        count_error();
        M_out_emsg(L"parse_cmdline(): no main imbed filename provided");        
        return imbed_filename;
    }
 
    return imbed_filename; 
}
M_endf    



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   process_main_file() -- parse and execute the main input file --- external function
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

uint64_t process_main_file(frame_S& frame, const std::wstring& main_filename) try
{
     M_start_time(t1)                                                                  // start parse-phase timer

    // add predefined verbs and typdefs to global symtab_S

    add_predefined_verbs();                                                            // needed for verb priority lookup
    add_predefined_typdefs();


    // add new (top-plevel) pre_parse_C object to the pre-parser stack and initialize cached IDs

    auto pp_p = add_new_pp(); 
    add_cached_id(L"????????????????????? uninitialized source_id=0 ????????????????????????"); 
              

    // attach main imbed file to pre_parser stream -- check for errors 
    // --------------------------------------------------------------- 

    auto arc = pp_p->attach_file(main_filename);
    if (arc != 0) 
    {
        count_error();
        M_out_emsg(L"process_main_file(): error R/C from pre_parse_C::attach_file(\"%S\") = %d -- cannot continue") % main_filename % arc;  
        return error_count();
    }
              
  
    // -------------------------------------------
    // parse the main slist_S in input file stream
    // -------------------------------------------

    slist_S slist {};                                                                   // main expression list 

    auto grc = get_unnested_slist(frame, *pp_p, slist); 
    

    // remove top-level pre_parse_C object from the pre-parser stack

    remove_pp();  

    auto et1 = M_elapsed_time(t1); 

    M__(display_slist(slist, L"main slist", L"", true, false);)


    // -------------------------------------------------------------
    // execute the parsed main slist_S (if no errors during parsing)
    // -------------------------------------------------------------

    if (error_count() == 0)
    {
        M_start_time(t2)
        auto erc = eval_main_block(frame, slist); 
        if (erc != 0)
        {
            M_out_emsg(L"main slist evaluation ended with R/C = %d") % erc; 
        }        

        auto et2 = M_elapsed_time(t2);

        if (log_statistics() != log_E::none)
        {
            M_out( L" "                                                                                   ) ;
            M_out( L"-----------------------------------------------------------------------------------" ) ;
            M_out( L"                : characters=%d  tokens=%d"                                          )  % get_character_count()   % get_raw_token_count() ;
            M_out( L"                : slists=%d  statements=%d  verbs=%d  values=%d"                     )  % get_eval_slist_count()  % get_eval_statement_count() % get_eval_verb_count() % get_eval_value_count() ;
            M_out( L"                : stack frames = %d, max stack depth = %d"                           )  % get_eval_frame_serial() % get_eval_frame_max_depth() ;
            M_out( L"parse      phase: elapsed_time = %.6f seconds"                                       )  % et1 ; 
            M_out( L"evaluation phase: elapsed_time = %.6f seconds   verbs/sec = %.1f"                    )  % et2                     % (((float64_T)(get_eval_verb_count()))/et2) ;
            M_out( L"-----------------------------------------------------------------------------------" ) ;
        }
    }
    else
    {
        M_out_emsg(L"parse_input() -- bypassing evaluation phase because of %d error(s) during parse and pre-parse phases") % error_count();     
    }

    M__(display_id_cache();)

    return error_count(); 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   parse_string() -- parse passed-in string and pass_back slist --- external function
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int parse_string(frame_S& frame, slist_S& out_slist, const std::wstring& in_string, const std::wstring& in_name, bool refresh) try
{
    M__(M_out(L"parse_string() -- called");)


    // create and initialize a new pre_parse_C object (nested) 

    auto pp_p = add_new_pp();
    

    // attach passed-in string to pre_parse_C -- if error, return empty slist_S and error r/c
    // --------------------------------------------------------------------------------------
 
    auto arc = pp_p->attach_string(in_string, in_name);

    if (arc != 0)
    {
        M_out_emsg(L"parse_string() -- error trying to attach passed-in string -- empty slist is being returned");

        out_slist = slist_S { };      // return empty slist
        return -1;                    // error r/c
    }
  

    // parse newly-attached string (in new pre_parse_C) using passed-in stack frame to obtain unnested slist
    // -----------------------------------------------------------------------------------------------------

    auto grc = get_unnested_slist(frame, *pp_p, out_slist); 
    
    M__(display_slist(out_slist, L"parsed slist", L"", true, false);)


    // get rid of pre_parse_C object allocated above

    remove_pp(); 


    // if parsing error, return empty slist_S and r/c other than END
    // -------------------------------------------------------------

    if (grc != 1)
    {
        M_out_emsg(L"parse_string() -- error r/c = %d during parsing of passed-in string -- empty slist is being returned") % grc;
        out_slist = slist_S { };      // return empty slist
        return -1;                    // error r/c
    }

    return 0; 
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
////   discard_token() -- discard token in pre-parser -- allows next peek_token() to get a new token
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void discard_token(pre_parse_C& pp) try
{
    pp.discard_token();
    static_N::just_peeked = false;         // next peek_token() should see new token; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   peek_token() -- get next token from pre-parser -- check for error, end
////
////
////      r/c = -1 -- error token             -- error message -- no token passed back
////      r/c = 0  -- normal token            -- token passed back
////      r/c = 1  -- END token (expected)    -- END token passed back
////      r/c = 1  -- END token (unexpected)  -- error message, END token passsed back (may be empty 
////                                             except for user type field, if pre-parser passed back no token)
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int peek_token(pre_parse_C& pp, token_C& tok, bool end_not_expected) try
{
    // peek at next token in stream 

    token_C token; 
    auto prc = pp.peek_token(token);


    // add to token list, if last peek()ed token was discarded

    if (!static_N::just_peeked)
    {
        static_N::token_list.push_back(token); 
        static_N::token_ix++; 
        static_N::just_peeked = true; 
    }


    // check for error R/C or error token from pre-parser

    if ( (prc > 1) || (prc < 0) || (token.type == token_E::error) )
    {
        count_error();
        M_out_emsg1(L"peek_token(): error R/C from pre_parse_C::peek_token() = %d -- cannot continue") % prc; 

        if (prc == 0)
            token.display();

        M_out_emsgz();

        return -1;
    }


    // check for END -- complain if unexpected

    if ( (prc == 1) || (token.type == token_E::end) )
    {
        token.utype2 = tok_u2_E::end;      // set user type first

        if (end_not_expected)     // caller was not expecting END
        {
            count_error();
            M_out_emsg1(L"peek_token(): unexpected END token (end of input) was seen"); 
            token.display();
            M_out_emsg2(L"unexpected END halts all parsing immediately");

            tok = token;            // pass back token (may be empty except for user type)
            return 1;
        } 
        else
        {
            tok = token;            // pass back token (may be empty except for user type)
            return 1;
        }
    }


    // otherwise, token is normal -- need to set user type field based on main type field
    // ---------------------------------------------------------------------------------- 

    // handle tokens without sigils 

    if ( (!token.has_leading_sigil) && (!token.has_trailing_sigil) )
    {
        // check identifier tokens with no sigils -- identifier

        if ( (token.type == token_E::identifier) || (token.type == token_E::extended_identifier) )
        {
            if ( (token.type == token_E::identifier) && (token.attached_paren) )
                token.utype2 = tok_u2_E::verbname;      // this identifier is being used as a verb
            else
                token.utype2 = tok_u2_E::identifier;    // "plain identifier"  (not being used as a verb)
        }


        // check operator tokens with no sigils -- verbname

        else if (token.type == token_E::oper)          // operators are verbs
        {
            token.utype2 = tok_u2_E::verbname;         // verb to be invoked
        }


        // check for value-type or literal  tokens -- expect only:      10000,      -10000,      1.2345,    "abcd efg",      -or-     °    ( unit value)      ?????????????????????????????????????

        else if (
                 (token.type == token_E::string          )
                 ||
                 (token.type == token_E::unit            ) 
                 ||
                 (token.type == token_E::boolean         ) 
                 ||
                 (token.type == token_E::uint8           ) 
                 ||
                 (token.type == token_E::int8            ) 
                 ||
                 (token.type == token_E::uint16          ) 
                 ||
                 (token.type == token_E::int16           ) 
                 ||
                 (token.type == token_E::uint32          ) 
                 ||
                 (token.type == token_E::int32           ) 
                 ||
                 (token.type == token_E::uint64          ) 
                 ||
                 (token.type == token_E::int64           ) 
                 ||
                 (token.type == token_E::float64         ) 
                 ||
                 (token.type == token_E::float32         ) 
                )
        {
            token.utype2 = tok_u2_E::value;
        }

        // check for single-char tokens -- expect only:    ( )     [ ]     { }    -or-    ;

        else if (token.type == token_E::open_paren)
        {
            token.utype2 = tok_u2_E::open_paren;
        }
        else if (token.type == token_E::close_paren)
        {
            token.utype2 = tok_u2_E::close_paren;
        }
        else if (token.type == token_E::open_bracket)
        {
            token.utype2 = tok_u2_E::open_bracket;
        }
        else if (token.type == token_E::close_bracket)
        {
            token.utype2 = tok_u2_E::close_bracket;
        }
        else if (token.type == token_E::open_brace)
        {
            token.utype2 = tok_u2_E::open_brace;
        }
        else if (token.type == token_E::close_brace)
        {
            token.utype2 = tok_u2_E::close_brace;
        }
        else if (token.type == token_E::semicolon)
        {
            token.utype2 = tok_u2_E::semicolon;
        }
        else       // unsupported token (without sigils)
        {
            token.utype2 = tok_u2_E::other;
        }
    }


    // handle tokens with leading and/or trailing sigil

    else       
    {
        //  for identifiers, expect only:      @verb   -or-    keyword:  -or-    :label   -or-   $parm 
        //  ------------------------------------------------------------------------------------------

        if ( (token.type == token_E::identifier) || (token.type == token_E::extended_identifier) )
        {
            if (      ( token.has_leading_sigil) && (!token.has_trailing_sigil) && (token.leading_sigil  == const_N::ch_sigil_verb      ) )        // check for @xxxx    => verb name
            {
                token.utype2 = tok_u2_E::verbname;                // verb to be invoked
            }
            else if ( ( token.has_leading_sigil) && (!token.has_trailing_sigil) && (token.leading_sigil  == const_N::ch_sigil_label     ) )        // check for :xxxx    => label
            {
                token.utype2 = tok_u2_E::label;                  
            }
            else if ( (!token.has_leading_sigil) && ( token.has_trailing_sigil) && (token.trailing_sigil == const_N::ch_sigil_keyword  ) )        // check for xxxx:  => keyword parm (0 or more values)
            {
                token.utype2 = tok_u2_E::keyname; 
            }
            else if ( ( token.has_leading_sigil) && (!token.has_trailing_sigil) && (token.leading_sigil  == const_N::ch_sigil_parm      ) )        // check for $xxxx  => parm (no change -- same as with no sigils)
            {
                token.utype2 = tok_u2_E::identifier; 
            }
            else                                                  // identifier with unexpected sigil combination (error)
            {
                if (token.peek_count <= 1)                        // only complain 1st time we peek() this token
                {
                    M_out_emsg1(L"peek_token() -- unexpected sigil combination seen for identifier token" );
                    token.display();
                    M_out_emsg2(L"token will not be accepted");
                }

                token.utype2 = tok_u2_E::other; 
            }
        }


        //  for operators, expect only:    $++   etc. 
        //  -----------------------------------------

        else if (token.type == token_E::oper)
        {
            if ( ( token.has_leading_sigil) && (!token.has_trailing_sigil) && (token.leading_sigil  == const_N::ch_sigil_parm      ) )        // check for $xxxx    => treat operaor token as parm, not verb
            {
                token.utype2 = tok_u2_E::identifier;               // handle like an identifier associated with verbdef 
            }
            else
            {
                if (token.peek_count <= 1)                         // only complain 1st time we peek() this token
                {
                    M_out_emsg1(L"peek_token() -- unexpected sigil combination seen for operator token" );
                    token.display();
                    M_out_emsg2(L"token will not be accepted");
                }

                token.utype2 = tok_u2_E::other;  
            }
        }


        //  for open parenthesis, expect only:    @(   or  :(
        //  -------------------------------------------------

        else if (token.type == token_E::open_paren)
        {
            if      ( ( token.has_leading_sigil) && (!token.has_trailing_sigil) && (token.leading_sigil  == const_N::ch_sigil_verb      ) )        // check for @(    => treat enclosed expression as a verbname
            {
                token.utype2 = tok_u2_E::open_verbname_bracket;    // handle whole expression like a verbname 
            }
            else if ( ( token.has_leading_sigil) && (!token.has_trailing_sigil) && (token.leading_sigil  == const_N::ch_sigil_keyword   ) )        // check for :(    => treat enclosed expression as a keyword name
            {
                token.utype2 = tok_u2_E::open_keyname_bracket;     // handle whole expression like a keyword name 
            }
            else
            {
                if (token.peek_count <= 1)                         // only complain 1st time we peek() this token
                {
                    M_out_emsg1(L"peek_token() -- unexpected sigil combination seen for open parenthesis token" );
                    token.display();
                    M_out_emsg2(L"token will not be accepted");
                }

                token.utype2 = tok_u2_E::other;  
            }
        }


        //  for close parenthesis, expect only:    )@   or ):
        //  -------------------------------------------------

        else if (token.type == token_E::close_paren)
        {
            if      ( (!token.has_leading_sigil) && ( token.has_trailing_sigil) && (token.trailing_sigil  == const_N::ch_sigil_verb      ) )        // check for )@    => treat enclosed expression as a verbname
            {
                token.utype2 = tok_u2_E::close_verbname_bracket;    // handle whole expression like a verbname 
            }
            else if ( (!token.has_leading_sigil) && ( token.has_trailing_sigil) && (token.trailing_sigil  == const_N::ch_sigil_keyword   ) )        // check for ):    => treat enclosed expression as a keyword name
            {
                token.utype2 = tok_u2_E::close_keyname_bracket;     // handle whole expression like a keyword name 
            }
            else
            {
                if (token.peek_count <= 1)                          // only complain 1st time we peek() this token
                {
                    M_out_emsg1(L"peek_token() -- unexpected sigil combination seen for close parenthesis token" );
                    token.display();
                    M_out_emsg2(L"token will not be accepted");
                }

                token.utype2 = tok_u2_E::other;  
            }
        }


        // all other token types -- no sigils expected
        // -------------------------------------------

        else
        {
            if (token.peek_count <= 1)                             // only complain 1st time we peek() this token
            {
                M_out_emsg1(L"peek_token() -- sigils were present for a token which is not an identifier, operator, or parenthesis" );
                token.display();
                M_out_emsg2(L"token will not be accepted");
            }

            token.utype2 = tok_u2_E::other;                        // non-identifier/operator/parenthesis token had sigils -- shouldn't see this 
        }
    }


    // normal return to caller with peek()ed token
   
    tok = token; 
    return 0;   
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//        SSSS     L         IIIII      SSSS      TTTTT
//       S         L           I       S            T  
//        SSS      L           I        SSS         T  
//           S     L           I           S        T  
//       SSSS      LLLLL     IIIII     SSSS         T  
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_slist() -- get list of expressions (statements)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""




// this routine does not consume END token)

static int get_unnested_slist(frame_S& frame, pre_parse_C& pp, slist_S& slist) try
{
    return get_slist(frame, pp, slist, false, true);
}
M_endf




// initial open bracket must be consumed already -- this routine consumes matching close bracket 

static int get_nested_slist(frame_S& frame, pre_parse_C& pp, slist_S& slist) try
{
    return get_slist(frame, pp, slist, true, false);
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int get_slist(frame_S& frame, pre_parse_C& pp, slist_S& slist, bool end_by_brace, bool end_expected) try
{
    int rc = 0;

    M__(M_out(L"==============================> get_slist() called -- end_by_brace=%s  end_expected=%s") % M_bool_cstr(end_by_brace) % M_bool_cstr(end_expected);) 

    // ======================================================================== 
    // main loop to fetch expressions/statements, until end of slist is reached
    // ========================================================================
    
    bool first_pass {true}; 

    for (;;)
    {
        a_expression_S expression {}; 


        // do peek() to check for ending token

        token_C token {}; 
        M__(M_out(L"get_slist() calling peek_token");)
        auto prc = peek_token(pp, token, !end_expected);
        M__(M_out(L"get_slist() -- peek token r/c=%d") % prc;) 
        M__(token.display(L"get_slist() -- after peek_token()");) 


        // handle expected/unexpected END and other errors from peek token

        if (prc != 0) 
        {
            if (rc != 0)
            {
                M_out_emsg1(L"get_slist(): slist parsing ending with failure, after prior error/ unexpected END token was detected");
                M_out_emsg2(L"           : empty slist is being returned rather than an incomplete one");
                slist = slist_S { };                            // pass back empty slist, rather than incomplete one 
                return rc; 
            }

            if (prc == 1)                                       // END -- expected or not
            {
                if (!end_expected)
                {
                    M_out_emsg1(L"get_slist(): unexpected END of input seen while parsing nested slist -- possible missing closing brace");
                    M_out_emsg2(L"           : empty slist is being returned rather than an incomplete one");
                    slist = slist_S { };                        // pass back empty slist, rather than incomplete one                   
                }   
            }
            else                                                // token processing error
            {
                M_out_emsg1(L"get_slist(): unexpected token error seen while parsing nested slist");
                M_out_emsg2(L"           : empty slist is being returned rather than an incomplete one");
                slist = slist_S { };                            // pass back empty slist, rather than incomplete one          
            }                                                 
                                                              
            return prc;                                         // return immediately, if error or END -- pass back whatever has been accumulated so far in slist (if no prior error was seen)
        }


        if (token.utype2 == tok_u2_E::close_brace)
        {
            if (end_by_brace)
            {
                a_expression_S dummy_expression  { };           // dummy expression to add ending brace token to (for ending tokens index  in debug messages) 
                expression_add_token(dummy_expression, token);

                discard_token(pp);                              // consume ending brace

                if (rc != 0)
                {
                   M_out_emsg(L"get_slist(): slist parsing ending with failure, after error was detected -- empty slist is being returned rather than an incomplete one"); 
                }


                M__(M_out(L"<=================== get_slist() returning after closing brace -- r/c=%d") % rc;)
                return rc;                                      // return with whatever has been accumulated so far      
            }                                               
            else                                                // closing brace is unexpected (unnested slist)
            {
                count_error(); 
                M_out_emsg1(L"get_slist(): unexpected closing brace found in unnested slist"); 
                token.display(L"unexpected");
                M_out_emsg2(L"closing brace is ignored");  
                discard_token(pp);                              // get rid of unexpected brace, so loop can continue
                rc = -1;                                        // remember error 
            }
        }


        // handle labels

        if (token.utype2 == tok_u2_E::label)
        {
            M__(M_out(L"get_slist() -- label token seen -- first_pass = %d") % (int)first_pass ;)        

            // it this is first token, set main label for this slist

            if (first_pass)
                slist.label = token.str; 


            //  add label to labels list -- expression index will be at()-index for next expression going into expression vector (may point one past end of expressions, if no expressions follow this label)

            slist.labels.emplace(token.str, slist.expressions.size()); 
        
            discard_token(pp);  
            first_pass = false; 
            continue; 
        }


        // get next expression in list -- return immediately, if error or END

        auto grc = get_unnested_expression(frame, pp, expression, end_expected);    // r/c:  0=ok, 1=END, -1=error, 2=bad expression -- for r/c=0 or2,  ending semicolon consumed, closing brace not
        M__(M_out(L"get_slist() -- r/c from get_expression() = %d") % grc;)
        M__(M_out(L"-------- start of expression ------------");)
        M__(display_expression(expression, L"get_slist() -- from get_expression()");) 
        M__(M_out(L"========= end of expression =============");)


        if (grc < 0) rc = -1;             // error?  (end is expected, so valid expression should have been passed back) -- END is unconsumed  
        

        // handle valid expression passed back from get_expression()

        if ( (grc == 0) || (grc == 1) )            // normal token or normal token + END
        {
            M__(M_out(L"get_slist() -- adding expression passed-back from get_expression() to slist");)
            M__(display_expression(expression, L"get_slist() -- added expression");) 

            slist.expressions.push_back(expression);
            slist.expression_ct++; 
        }
    
        first_pass = false; 

        //   ================ 
    }   //   end of main loop 
        //   ================

    M__(M_out(L"<=================== get_slist() returning ???????????? -- r/c=%d") % rc;)
    return rc;   // shouldn't get here
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//          EEEEE    X   X    PPPP     RRRR     EEEEE     SSSS     SSSS    IIIII     OOO     N   N
//          E         X X     P   P    R   R    E        S        S          I      O   O    NN  N 
//          EEEE       X      PPPP     RRRR     EEEE      SSS      SSS       I      O   O    N N N  
//          E         X X     P        R  R     E            S        S      I      O   O    N  NN 
//          EEEEE    X   X    P        R   R    EEEEE    SSSS     SSSS     IIIII     OOO     N   N
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   expression_add_token() -- add token to expression start/end token indexes -- for debugging
////   ======================
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void expression_add_token(a_expression_S& expression, const token_C& token) try
{  
    // update first/last token indexes in expression

    if (expression.token_ix1 == -1)                                                   // not set yet?
        expression.token_ix1 = static_N::token_ix;
    else
        expression.token_ix1 = std::min(static_N::token_ix, expression.token_ix1);    // lowest token index yet seen
    
    expression.token_ix2 = std::max(static_N::token_ix, expression.token_ix2);        // highest token index yet seen  

    return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_verb_priority() -- return defined verb priority, or 0, if not defined
////   ===================
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_verb_priority(frame_S& frame, const std::wstring& name) try
{
    verbdef_S verbdef { };

    auto gvrc = get_verb(frame, name, verbdef); 

    if (gvrc == 0)
        return verbdef.priority;
    else
        return 0; 
}

M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   expression_set_verb() -- add simple or complex verbname-oriented fields to newly constructed expression
////   =====================
////
////   Assume there is no verb already in the expression (left plist may be present in some cases)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

///////////////////////////////
// version with passed-in token -- token type must be verbname
///////////////////////////////

void expression_set_verb(frame_S& frame, a_expression_S& expression, const token_C& token, const verbdef_S& verbdef) try
{  
    // make sure there is no verb already saved away in this expression

    if (expression.has_verb)
    {
        M_out_emsg1(L"expression_set_verb() -- trying to set verb into the expression, but a verb is already present for this expression");
        display_expression(expression, L"already-has-verb");
        M_out_emsgz(); 
        M_throw("expression_set_verb() -- trying to set verb into the expression, but a verb is already present for this expression")
    }


    //    make sure passed-in token is a simple verbmame

    if (token.utype2 != tok_u2_E::verbname)
    {
        M_out_emsg1(L"expression_set_verb() -- trying to set verb into the expression, but token is not a verb name");
        M_out_emsgz(); 
        M_throw("expression_set_verb() -- trying to set verb into the expression, but token is not verb name")
    }
      

    // save away verbname token, etc. in this expression

    expression.has_verb = true;
    expression_add_token(expression, token);                          // add verb token to expression    
    set_atom_token(expression.verb_value, token, static_N::token_ix); // fill in verb_value with simple verb name value
    
    if (token.has_leading_sigil)
    {
        expression.has_sigil = true;                                  // set sigil flag for debug messsages
        expression.sigil     = token.leading_sigil;                   // save away sigil, too
    }  


    // save away verb priority and associativity (if verb is defined at this point -- otherwise empty verbdef is passed in) 

    expression.priority        = verbdef.priority;                    // save away verb priority 
    expression.right_associate = verbdef.right_associate;             // save away verb associativity 
    expression.left_associate  = verbdef.left_associate;              // save away verb associativity

    return; 
}
M_endf


/////////////////////////////////////////////
// version with passed-in verb sub-expression  (evaluation of this must yield a string later on, or else a run-time error occurs)  
/////////////////////////////////////////////

void expression_set_verb(frame_S& frame, a_expression_S& expression, const a_expression_S& verb_expression, bool move_ok) try
{  
    // make sure there is no verb already saved away in this expression

    if (expression.has_verb)
    {
        M_out_emsg1(L"expression_set_verb() -- trying to set verb into the expression, but a verb is already present for this expression");
        display_expression(expression, L"already-has-verb");
        M_out_emsgz(); 
        M_throw("expression_set_verb() -- trying to set verb into the expression, but a verb is already present for this expression")
    }
      

    // save away verbname token, etc. in this expression

    expression.has_verb = true; 
    set_expression_value(expression.verb_value, verb_expression, move_ok);   


    // update location info in passed-in main token to include verb sub-token limits

    if (expression.token_ix1 == -1)                                                           // not set yet?
        expression.token_ix1 = verb_expression.token_ix1;                                     // verb sub-expression must be at start of main expression 
    else
        expression.token_ix1 = std::min(verb_expression.token_ix1, expression.token_ix1);     // lowest token index yet seen
    
    expression.token_ix2 = std::max(verb_expression.token_ix2, expression.token_ix2);         // highest token index yet seen   


    // use default settings for has_sigil, verb priority, associations, etc.

    return; 
}
M_endf


///________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_expression() -- fetch next expression from input token stream 
////
////
////   rc = -2  -- should-not occur error
////   rc = -1  -- error from pre_parse
////   rc = 0   -- normal 
////   rc = 1   -- END seen (not consumed)
////   rc = 2   -- empty expression passed back because of unexpected tokens, etc.error
////
////
////   note: input/output is expression is expected to be completely empty or contain left parms and verb. (no leftovers from last time) 
////
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// this routine consumes ending semicolon (but not END token)

static int get_unnested_expression(frame_S& frame, pre_parse_C& pp, a_expression_S& expression, bool end_expected) try
{
    M__(M_out(L"get_unnested_expression() called");)
    return get_expression(frame, pp, expression, false, false, false, end_expected);
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initial open paren must be consumed already -- this routine consumes matching close paren 

static int get_nested_expression(frame_S& frame, pre_parse_C& pp, a_expression_S& expression) try
{
    M__(M_out(L"get_nested_expression() called");)
    return get_expression(frame, pp, expression, true, false, false, false);
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initial open verb bracket must be consumed already -- this routine consumes matching close verb bracket  

static int get_nested_verbname_expression(frame_S& frame, pre_parse_C& pp, a_expression_S& expression) try
{
    M__(M_out(L"get_nested_verbname_expression() called");)
    return get_expression(frame, pp, expression, false, true, false, false);
}
M_endf

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initial open keyword bracket must be consumed already -- this routine consumes matching close keyword bracket  

static int get_nested_kwname_expression(frame_S& frame, pre_parse_C& pp, a_expression_S& expression) try
{
    M__(M_out(L"get_nested_kwname_expression() called");)
    return get_expression(frame, pp, expression, false, false, true, false);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////  helper function for main routine

static int get_expression_verb(frame_S& frame, pre_parse_C& pp, a_expression_S& expression, const token_C& token, const verbdef_S& verbdef) try
{
   if (token.utype2 == tok_u2_E::verbname)
   {
       expression_set_verb(frame, expression, token, verbdef);                 // add token directly to expression verbname value field   
       discard_token(pp);                                                      // get rid of verb/option token
   }
   else                                                                        // open angle bracket is start of verb sub-expression
   {                                                                   
       // add in <verb sub-expression > to passed-in (main) expression

       discard_token(pp);                                                      // get rid of open angle bracket token before calling get_nested_verbname_expression()

       a_expression_S verb_expression { };                                     // local a_expression_S (in autodata) for gathering up verb_expression
       auto grc = get_nested_verbname_expression(frame, pp, verb_expression);  // fill in verb_expression 
       if (grc != 0)
           return grc;                                                         // pass back error -- let get_expression() handle error appropriately (should go into flush mode)  

       expression_set_verb(frame, expression, verb_expression, true);          // OK to move data from verb_expression, since it is going away very soon  
   } 
   
   return 0 ;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////// main routine, called by above three routines //////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int get_expression(frame_S& frame, pre_parse_C& pp, a_expression_S& expression, bool end_by_paren, bool end_by_verbname_bracket, bool end_by_keyname_bracket, bool end_expected) try
{
    bool flushing           { false };                                                // true -- flushing tokens until end reashed 
    bool left_parms_done    { false };                                                // true -- left side parms have been seen
    bool right_parms_done   { false };                                                // true -- right side parms have been seen
    bool verb_done          { false };                                                // true -- verb has been added to expression 
    bool priority_recursion { false };                                                // true -- recursive call due to finding higher priority verb 
    int  error_count        { 0     };                                                // local counter -- number of errors so far accumulating this expression  
    
    M__(M_out(L"get_expression() called - end_by_paren=%d  end_by_verbname_bracket=%d  end_by_keyname_bracket=%d  end_expected=%d") % end_by_paren % end_by_verbname_bracket % end_by_keyname_bracket % end_expected;)


    // set expression gathering flags before starting main loop, if verb (and left parms) are already set in the incoming expression

    if (expression.has_verb)
    {
        M__(M_out(L"get_expression() -- priority recursion -- left_parms and verb already done when called");)
        left_parms_done    = true; 
        verb_done          = true; 
        priority_recursion = true; 
    }


    // if this is a nested expression, set starting token index to accound for already-discarded open paren or open angle bracket token

    if (end_by_paren || end_by_verbname_bracket || end_by_keyname_bracket)
        expression.token_ix1 = static_N::token_ix; 


    // =================================================================================== 
    // main loop to fetch tokens, until a complete expression (or error/END) can be passed back
    // ===================================================================================

    for(;;)
    {
        // look at next token -- end immediately (perhaps with empty expression), if error or unexpected end 

        token_C token {}; 
        M__(M_out(L"get_expression() calling peek_token");)
        auto prc = peek_token(pp, token, !end_expected);

        if ( (prc < 0) || (prc > 1) )
        {
            M_out_emsg1(L"get_expression(): unexpected input error while processing expression"); 
            display_expression(expression, L"error token");
            M_out_emsg2(L"empty expression is being substituted");
            expression = a_expression_S {};                                                         // pass back empty expression 
            return prc;                                                                             // leave unexpected token unconsumed for next time
        }                                                                                          
                                                                                                   
        if (prc == 1)                                                                               // END seen?
        {                                                                                          
            if (!end_expected)                                                                      // END not expected?
            {
                M_out_emsg1(L"get_expression(): unexpected END of input reached while processing nested expression -- possible missing close parenthesis"); 
                display_expression(expression, L"unexpected END");
                M_out_emsg2(L"empty expression is being substituted");
                expression = a_expression_S {};                                                     // pass back empty expression if end if not expected (i.e. nested expression) 
                return -1;                                                                          // return error if end is not expected
            }                                                                                      
            else                                                                                    // END is expected
            {
                if (flushing)
                {
                     M_out_emsg1(L"get_expression(): END of input reached -- expression was flushed after first error"); 
                     display_expression(expression, L"flushed");
                     M_out_emsg2(L"empty expression is being substituted");
                     expression = a_expression_S {};                                                // pass back empty expression 
                     return 2;                                                                      // indicate bad expression 
                }
                else if ( (!verb_done) && (left_parms_done) )
                { 
                    if (
                        ( (expression.lparms.kw_ct == 0   ) || (pp.m_allow_verbless_kws        ) ) // no keywords present or keywords are allowed in verbless expressions
                        &&
                        ( (expression.lparms.value_ct <= 1) || (pp.m_allow_verbless_multi_parms) ) // not too many parms, dpending on how many are allowed
                       )
                    {    
                        M__(M_out(L"get_expression() -- normal return due to END of input -- no verb present");)   
                        return  (error_count > 0) ? 2 : 0;                                         // verbless expression is OK  
                    }                                                                        
                    else                                                                           // disallowed positional or keyword parms in verbless expression
                    {  
                        count_error(); 

                        if ( (expression.lparms.kw_ct > 0) && (!pp.m_allow_verbless_kws) )
                            M_out_emsg1(L"get_expression(): END of input reached during expression parsing, but no verb was present following one or more left-side keyword parm(s)"  );   // disallowed keywords meg takes priority  
                        else
                            M_out_emsg1(L"get_expression(): END of input reached during expression parsing, but no verb was present following more than one left-side positionsl parm"); 

                        display_expression(expression, L"no-verb");
                        M_out_emsg2(L"empty expression is being substituted");
                        expression = a_expression_S {};                                            // pass back empty expression
                        return 2;                                                                  // indicate bad expression
                    }                                                                      
                }                                                                          
                                                                                           
                M__(M_out(L"get_expression() -- normal return due to END of input -- verb present");)           
                return  (error_count > 0) ? 2 : 1;                                                 // return with normal END, or indicate bad expression
            }           
        }

        M__(token.display(L"get_expression()");) 
        M__(M_out(L"get_expression() -- token.utype2 = %d") % (int)(token.utype2);)


        //  ----------------------     
        //  check for ending token
        //  ----------------------

        if (end_by_paren || end_by_verbname_bracket || end_by_keyname_bracket)                     // paren/bracket mode -- nested expression -- look only for close paren/bracket to end vlist (END should have been filtered out above)
        {
            // check for matching close paren or close verbname/keyname bracket
            // ----------------------------------------------------------------

            if  (
                  ( end_by_paren             && (token.utype2 == tok_u2_E::close_paren                                                        ) )
                  ||
                  ( end_by_verbname_bracket  && ((token.utype2 == tok_u2_E::close_verbname_bracket) || (token.utype2 == tok_u2_E::close_paren)) )    // can have either @( ... )@  -or-  @( ... )
                  ||
                  ( end_by_keyname_bracket   && ((token.utype2 == tok_u2_E::close_keyname_bracket ) || (token.utype2 == tok_u2_E::close_paren)) )    // can have either :( ... ):  -or-  :( ... )
                )
            {
                M__(M_out(L"get_(nested)_expression(): ending token seen");) 

                expression_add_token(expression, token);                                           // add to token list for this expression  

                if (!priority_recursion)                                                           // leave ending token for upper-level get_expression() call, if priority recursion is active
                {
                    M__(M_out(L"get_(nested)_expression() -- discarding matching close paren/bracket");)
                    discard_token(pp);                                                             // get rid of closing parenthesis (if not priority recursion)
                }
   
                if (flushing)
                {
                     M_out_emsg1(L"get_expression(): expression was flushed after first error"); 
                     display_expression(expression, L"flushed");
                     M_out_emsg2(L"empty expression is being substituted");
                     expression = a_expression_S {};                                               // pass back empty expression 
                     return 2;                                                                     // indicate bad expression 
                }
                else if ( (!verb_done) && (left_parms_done) )
                { 
                    if (
                        ( (expression.lparms.kw_ct == 0   ) || (pp.m_allow_verbless_kws        ) ) // no keywords present or keywords are allowed in verbless expressions
                        &&
                        ( (expression.lparms.value_ct <= 1) || (pp.m_allow_verbless_multi_parms) ) // not too many parms, dpending on how many are allowed
                       )
                    {    
                        M__(M_out(L"get_expression() -- normal return due to close paren/bracket -- no verb present");)
                        return  (error_count > 0) ? 2 : 0;                                         // verbless expression is OK  
                    }                                                                        
                    else                                                                           // disallowed positional or keyword parms in verbless expression
                    {  
                        count_error(); 

                        if ( (expression.lparms.kw_ct > 0) && (!pp.m_allow_verbless_kws) )
                            M_out_emsg1(L"get_expression(): close paren or bracket reached during expression parsing, but no verb was present following one or more left-side keyword parm(s)"  );   // disallowed keywords meg takes priority  
                        else
                            M_out_emsg1(L"get_expression(): close paren or bracket reached during expression parsing, but no verb was present following more than one left-side positionsl parm"); 

                        display_expression(expression, L"no-verb");
                        M_out_emsg2(L"empty expression is being substituted");
                        expression = a_expression_S {};                                            // pass back empty expression
                        return 2;                                                                  // indicate bad expression
                    }  
                }
               
                M__(M_out(L"get_expression() -- normal return due to close paren/bracket -- verb present");)
                return  (error_count > 0) ? 2 : 0;     
            }
     

            // check for mismatched close paren or close verbname/keyname bracket
            
            if  (
                  ( end_by_paren             && ((token.utype2 == tok_u2_E::close_verbname_bracket) || (token.utype2 == tok_u2_E::close_keyname_bracket ))  )    // ()@  or ():  is not valid                                            )
                  ||
                  ( end_by_verbname_bracket  && (token.utype2 == tok_u2_E::close_keyname_bracket)                                                           )    // @():         is not valid
                  ||
                  ( end_by_keyname_bracket   && (token.utype2 == tok_u2_E::close_verbname_bracket)                                                          )    // :()@         is not valid
                )
            {
                M__(M_out(L"get_(nested)_expression(): mismatched ending parenthesis/bracket seen");) 

                expression_add_token(expression, token);                                  // add to token list for this expression -- for error message only  

                if (!priority_recursion)                                                  // leave ending token for upper-level get_expression() call, if priority recursion is active
                    discard_token(pp);                                                    // get rid of closing parenthesis (if not priority recursion)

                count_error(); 
                M_out_emsg1(L"get_expression(): starting and ending parenthesis types do not match for this expression"); 
                display_expression(expression, L"mismatched");
                M_out_emsg2(L"empty expression is being substituted"); 
                expression = a_expression_S {};                                           // pass back empty expression
                return 2;                                                                 // indicate bad expression   
            }                                                                
 

            // check for unexpected (ending) semicolon

            if (token.utype2 == tok_u2_E::semicolon)                                      // unexpected semicolon -- overrides parens, marks end of expression -- consume semicolon
            {                                                                       
                expression_add_token(expression, token);                                  // add to token list for this expression  -- for error message only 
                                                                                    
                if (!priority_recursion)                                                  // leave ending token for upper-level get_expression() call, if priority recursion is active
                    discard_token(pp);                                                    // consume the semicolon, since this must be top level expression within braces
            
                count_error(); 
                M_out_emsg1(L"get_expression(): semicolon reached before the closing parenthesis (or verbname/keyname bracket) for the expression was seen"); 
                display_expression(expression, L"incomplete");
                M_out_emsg2(L"empty expression is being substituted"); 
                expression = a_expression_S {};                                           // pass back empty expression
                return 2;                                                                 // indicate bad expression
            }                                                                          
                                                                                       
                                                                                       
            // check for unexpected (ending) close brace                               
                                                                                       
            if (token.utype2 == tok_u2_E::close_brace)                                    // closing brace -- must be end of expression list, without final semicolon
            {                                                                          
                expression_add_token(expression, token);                                  // add to token list for this expression  -- for error message only
                                                                                          // don't consume closing brace          
                count_error(); 
                M_out_emsg1(L"get_expression(): closing brace reached before closing parenthesis (or verbname/keyname bracket) for expression was seen"); 
                display_expression(expression, L"incomplete");
                M_out_emsg2(L"empty expression is being substituted");          
                expression = a_expression_S {};                                           // pass back empty expression
                return 2;                                                                 // indicate bad expression
            }


            // check for unexpected (ending) close paren, if this is a verb-type or keyword-type expression (this is now allowed -- see above) 

 //         if ( (end_by_verbname_bracket || end_by_keyname_bracket) && (token.utype2 == tok_u2_E::close_paren) )  // closing paren while in verb sub-expression?-- must be end of whole (outer) expression, without missing close verbname/keyname bracket
 //         {                                               
 //             expression_add_token(expression, token);                                   // add to token list for this expression  -- for error message only
 //                                                                                        // don't consume closing paren, since this must be a sub-expression and caller needs the close paren to exit properly           
 //             count_error(); 
 //             M_out_emsg1(L"get_expression(): closing paren reached before closing verbname/keyname bracket for verb-type or keyword-type expression was seen"); 
 //             display_expression(expression, L"incomplete");
 //             M_out_emsg2(L"empty expression is being substituted");           
 //             expression = a_expression_S {};                                            // pass back empty expression
 //             return 2;                                                                  // indicate bad expression
 //         }   
        }         // ------------------------------------------------------------------
        else      // not in paren or verbname/keyname bracket mode -- not nested expression 
                  // ------------------------------------------------------------------
        {
            if (token.utype2 == tok_u2_E::semicolon)                                      // semicolon -- consume semicolon at end
            {                                                                      
                expression_add_token(expression, token);                                  // add to token list for this expression 
                                                                                   
                if (!priority_recursion)                                                  // leave ending token for upper-level get_expression() call, if priority recursion is active
                    discard_token(pp);                                                    // consume the semicolon, since this must be top level expression within braces
                                                                
                if (flushing)
                {
                     M_out_emsg1(L"get_expression(): expression was flushed after first error"); 
                     display_expression(expression, L"flushed");
                     M_out_emsg2(L"empty expression is being substituted");
                     expression = a_expression_S {};                                      // pass back empty expression 
                     return 2;                                                            // indicate bad expression 
                }                                                                      
                else if ( (!verb_done) && (left_parms_done) )
                { 
                    if (
                        ( (expression.lparms.kw_ct == 0   ) || (pp.m_allow_verbless_kws        ) ) // no keywords present or keywords are allowed in verbless expressions
                        &&
                        ( (expression.lparms.value_ct <= 1) || (pp.m_allow_verbless_multi_parms) ) // not too many parms, dpending on how many are allowed
                       )
                    {    
                        M__(M_out(L"get_expression() -- normal return due to semicolon -- no verb present");)
                        return  (error_count > 0) ? 2 : 0;                                         // verbless expression is OK  
                    }                                                                        
                    else                                                                           // disallowed positional or keyword parms in verbless expression
                    {  
                        count_error(); 

                        if ( (expression.lparms.kw_ct > 0) && (!pp.m_allow_verbless_kws) )
                            M_out_emsg1(L"get_expression(): semicolon reached during expression parsing, but no verb was present following one or more left-side keyword parm(s)"  );   // disallowed keywords meg takes priority  
                        else
                            M_out_emsg1(L"get_expression(): semicolon reached during expression parsing, but no verb was present following more than one left-side positionsl parm"); 

                        display_expression(expression, L"no-verb");                            
                        M_out_emsg2(L"empty expression is being substituted");
                        expression = a_expression_S {};                                            // pass back empty expression
                        return 2;                                                                  // indicate bad expression
                    }  
                }              
     
                M__(M_out(L"get_expression() -- normal return due to semicolon -- verb present");)
                return  (error_count > 0) ? 2 : 0;                                                 // return normally, or indicate bad expression
            }                                                                               
                                                                                            
            if (token.utype2 == tok_u2_E::close_brace)                                             // closing brace -- must be end of expression list, without final semicolon
            {
                // closing brace is not part of this expression -- don't add to expression, and don't consume the brace                               
             
                if (flushing)
                {
                     M_out_emsg1(L"get_expression(): expression was flushed after first error"); 
                     display_expression(expression, L"flushed");
                     M_out_emsg2(L"empty expression is being substituted");
                     expression = a_expression_S {};                                                // pass back empty expression 
                     return 2;                                                                      // indicate bad expression 
                }

                if ( (!verb_done) && (left_parms_done) )
                {
                    M__(M_out(L"get_expression() -- kw_ct = %d,    value_ct = %d") % expression.lparms.kw_ct % expression.lparms.value_ct;)

                    if (
                        ( (expression.lparms.kw_ct == 0   ) || (pp.m_allow_verbless_kws        ) )  // no keywords present or keywords are allowed in verbless expressions
                        &&
                        ( (expression.lparms.value_ct <= 1) || (pp.m_allow_verbless_multi_parms) )  // not too many parms, dpending on how many are allowed
                       )
                    {    
                        M__(M_out(L"get_expression() -- normal return due to closing brace -- no verb present");)
                        return  (error_count > 0) ? 2 : 0;                                          // verbless expression is OK  
                    }                                                                        
                    else                                                                            // disallowed positional or keyword parms in verbless expression
                    {  
                        count_error(); 

                        if ( (expression.lparms.kw_ct > 0) && (!pp.m_allow_verbless_kws) )
                            M_out_emsg1(L"get_expression(): closing brace reached during expression parsing, but no verb was present following one or more left-side keyword parm(s)"  );   // disallowed keywords meg takes priority  
                        else
                            M_out_emsg1(L"get_expression(): closing brace reached during expression parsing, but no verb was present following more than one left-side positionsl parm"); 

                        display_expression(expression, L"no-verb");
                        M_out_emsg2(L"empty expression is being substituted");
                        expression = a_expression_S {};                                            // pass back empty expression
                        return 2;                                                                  // indicate bad expression
                    }  
                }   
  
                M__(M_out(L"get_expression() -- normal return due to closing brace -- verb present");)
                return  (error_count > 0) ? 2 : 0;                                                 // return normally, or indicate bad expression
            }
        }


        // if error already seen, just continue loop to check for end -- flush tokens  

        if (error_count > 0)
        {
            flushing = true;                                                                       // indicate flushing mode -- for final error message selection
            expression_add_token(expression, token);                                               // add to expression -- for error messages only  
            discard_token(pp);                                                                     // flush the non-ending token
            continue;                                                                              // loop back and look for ending token
        }


        //  get left-side vlist (if any)  
        //  ----------------------------

        if (!left_parms_done && !verb_done)
        {
            left_parms_done = true; 

            M__(M_out(L"get_expression() -- calling get_vlist() for left-side vlist");)
            auto grc = get_vlist(frame, pp, expression, expression.lparms); 
            if (grc != 0)
                error_count++; 
            continue;                                                                               // loop back to look for ending token (flush), if error seen -- should not have consumed ending token 
        }


        // handle verb -- add in verb, if not done yet -- reshuffle expressions that already have verbs based on verb priorities
        // ---------------------------------------------------------------------------------------------------------------------

        if ( (token.utype2 == tok_u2_E::verbname) ||  (token.utype2 == tok_u2_E::open_verbname_bracket) )
        {
            // get priority of verb (if known at parse time) -- some verbs are undefined at this point -- use default priority (0) for these
            // use special high priority if this verb was indicated by an attached parenthesis -- this works right, only if all vlists have full comma punctuation, and only when unary/binary operators are used without commas 

            int       priority    { verb_priority_default };                            // start with default verb priority     ????????????????? where is this used ????????????????
            verbdef_S verbdef     { };                                                  // temporary verbdef to be filled in

            if (token.utype2 == tok_u2_E::verbname)                                     // can get real priority only for simple verbdef expressions
            {   
                auto gvrc = get_verb(frame, token.str, verbdef);                        // look up verb name in symtab and extract verbdef info to local copy 

                if (gvrc == 0)
                    priority = verbdef.priority;                                        // can use real priority only if verb is pre-defined (built-in) or defined at the time of this parser invocation


                if (token.attached_paren)                                               // this verb indicated by attached paren -- example   verb(a, b, c)
                {
                    priority         = verb_priority_attached_paren;                    // use special high priority for verbs indicated by attached parenthesis 
                    verbdef.priority = priority;                                        // override usual priority for this verb
                }
            }


            // if no verb yet seen for this expression, just add the verb value to the expression

            if (!verb_done)
            {
                auto vrc = get_expression_verb(frame, pp, expression, token, verbdef);  // fill in expression verb-oriented fields for main expression 
                if (vrc != 0)                                                          
                    error_count++;                                                      // cause flush mode to start because of error
                                                                                       
                verb_done = true;                                                       // verb for current (main) token is now done -- this flag must be set after calling verb_get_expression, to avoid errroneous priority_recursion flag setting (in case of resursion)      
                continue;                                                               // loop back to get next token (or start flushing) 
            }
            else                             
            {
                // must have expression with multiple verbs -- prior verb or new verb needs to be pushed down based on verb priorities and associativity, as if (implicit) parens are present
                // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                if (
                    (expression.priority > verbdef.priority)                            // prior verb goes first, because of higher priority ?
                    ||
                    (
                     (expression.priority == verbdef.priority)                          // prior expression may go first, depending on assiciatiity and prefix/infix/postfix positioning
                     &&
                     (
                      (verbdef.left_associate)                                          // always left-associate ? (note: left_associate and right_associate flags should never both be on -- left_associate takes priority, though)
                      ||
                      (
                       (!verbdef.right_associate)                                       // don't always right-associate ?
                       &&
                       (expression.rparms.value_ct > 0)                                 // new verb appears to be infix (or postfix) ? -- left-associate by default
                      )
                     )
                    )                                                                   // prior verb goes first because of equal priorities and new verb's effective left-associativity ?
                   )
                {
                    // --------------------------------------------------------------------------------------------------------------------------------------------------------
                    //
                    // new verb has lower effective priority than verb already in in the expression: 
                    //
                    //  - save the results from prior verb's execution as the first (only) left-side positional parm for new verb
                    //
                    // -------------------------------------------------------------------------------------------------------------------------------------------------------- 

                    // copy everything from current expression into a new expression, clear out current expression, and attach new expression as the first (only) left positional parm of current expression 

                    value_S value    {      };                                           // temp value for set_expression_value()
                    a_expression_S left_expression { expression };                       // copy everything to new a_expression_S for adding in   (including set up verb and vlists, etc.)
                                                                                   
                    expression = a_expression_S {};                                      // clear out existing expression                   
                    set_expression_value(value, left_expression, true);                  // move OK -- temp_expression no longer needed 
                    add_positional_value(expression.lparms, value);                      // add to start of left positional parms vector in cleared-out original expression 
                    expression.lparms.token_ix1 = left_expression.token_ix1;             // vlist location is same is prior verb's expression location
                    expression.lparms.token_ix2 = left_expression.token_ix2;             // vlist location is same is prior verb's expression location
                                                                                         // !!!!!!!!!!!!!! note: that new expression's location does not include left parms in this case
                    
                    // set up cleared-out existing expression with the new verb   

                    auto vrc = get_expression_verb(frame, pp, expression, token, verbdef);       // fill in expression verb-oriented fields for main expression 
                    if (vrc != 0)                                                 
                    {                                                             
                        error_count++;                                                   // cause flush mode to start because of error
                        continue;                                                        // start flushing 
                    }


                    // reset expression processing flags (for new verb) indicating left parms and verb are done, and right parms are not yet done

                    right_parms_done = false;                                            // any right parms were moved into left_expression, so updated original expression doesn't have any yet   
                    left_parms_done  = true;                                             // left parms for original expression are done (just the one nested expression)
                    verb_done        = true;                                             // 2nd verb becomes 1st verb for original expression 
                    continue;                                                            // loop back to get next token -- should be right parms for update original expression
                }
                else // new verb priority is higher (or prior expression is effectively right-associative, and both verbs have same priority)
                {
                    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    //
                    // new verb needs to be evaluated before the verb already in in the expression: 
                    //
                    //  - transfer existing right-side parms to new verb's left parms, and set prior verb's only right-side positional parm = results from new hi-priority verb's execution
                    //
                    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
                
                    value_S    value       {    };                                      
                    a_expression_S   right_expression  {    };                                    // expression for new (hi-priority) verb 


                    // transfer prior verb's right-side parms (positional and kw) to left sideparms for new higher-priority verb

                    right_expression.lparms = expression.rparms;                                   // new verb's left parms are prior verb's right parms
                    expression.rparms       = vlist_S {};                                          // clear out right parms for prior verb (will be set below to results from new hi-priority verb);


                    // set up new right_expression with the new higher-priority verb   
                    
                    auto vrc = get_expression_verb(frame, pp, right_expression, token, verbdef);   // fill in expression verb-oriented fields for main expression 
                    if (vrc != 0)
                    {                                                                
                        error_count++;                                                            // cause flush mode to start because of error
                        continue;                                                                 // loop back to start flushing
                    }                                                                      


                    // do priority-recursive call to get_expression() 
                    //
                    //   -- pass thru ending flags from current call
                    //   -- partially-complete right_expression parm should cause nested get_expression() to go into "priority_recursive" mode, where ending token is left unconsumed for top-level get_expression() call 
                    //

                    M__(M_out(L"get_expression() -- doing priority-recursive call to get_expression()");)
                    auto grc = get_expression(frame, pp, right_expression, end_by_paren, end_by_verbname_bracket, end_by_keyname_bracket, end_expected);
                    if (grc != 0)
                        error_count++; 


                    // add completed higher-priority expression from priority-recursive get_expression() call to current expression's right-side positional parms -- will be the only right-side parm

                    set_expression_value(value, right_expression, true);                           // value points to completed higher-priority expression -- move is OK, since right_expression will soon go away
                    add_positional_value(expression.rparms, value);                                // first (only) right-size positional parm for prior verb = results from higher-priority verb
                    expression.rparms.token_ix1 = right_expression.token_ix1;                      // vlist location is same is higher_priority verb's expression location
                    expression.rparms.token_ix2 = right_expression.token_ix2;                      // vlist location is same is higher_priority verb's expression location
                                                                                       
                    right_parms_done = true;                                                       // expression for prior verb is done -- should next see end token (left unconsumed from recursive call to get_expression())
                    continue;                                                                      // loop back to get next token -- should be unconsumed ending token
                }                                                                   
            }
        }


        // process right-side parm list (if any)

        if (left_parms_done && verb_done && !right_parms_done)
        {
            right_parms_done = true; 
            auto grc = get_vlist(frame, pp, expression, expression.rparms); 
            if (grc != 0)
                error_count++; 
            continue;                                                                      // loop back to look for ending token if error seen -- should not consume ending token 
        }


        //   shouldn't get here -- some sort of extraneous token -- complain and continue with loop to flush until ending token seen
        //   token might be non-matching close_verbname_bracket or closing_keyname_backet

        error_count++; 
        count_error(); 
        M_out_emsg1(L"get_expression(): unexpected token « %s » seen -- expecting verb or option to begin new expression") % token.orig_str; 
        token.display(L"unexpected token");
        display_expression(expression, L"unexpected token");
        M_out_emsg2(L"bypassing this token -- flushing rest of expression");
        discard_token(pp);                                                                 // get rid of unexpected token
        expression = a_expression_S {};                                                    // replace incomplete expression for pass-back with empty expression
        continue;

        //   ================ 
    }   //   end of main loop 
        //   ================

    return -2;      // should not get here
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//       V   V     L         IIIII      SSSS      TTTTT
//       V   V     L           I       S            T  
//        V V      L           I        SSS         T  
//        V V      L           I           S        T  
//         V       LLLLL     IIIII     SSSS         T  
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// see also add_positional_value() -- adds positional value to passed-in vlist 


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_vlist_flags() -- set/refresh vlist flags based on values present 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void set_vlist_flags(vlist_S& vlist) try
{
    // first, reset all current flags

    vlist.val_unit            = false; 
    vlist.val_boolean         = false; 
    vlist.val_int8            = false;
    vlist.val_int16           = false;
    vlist.val_int32           = false;
    vlist.val_int64           = false;
    vlist.val_uint8           = false;
    vlist.val_uint16          = false;
    vlist.val_uint32          = false;
    vlist.val_uint64          = false;
    vlist.val_float32         = false;
    vlist.val_float64         = false; 
    vlist.val_string          = false; 
    vlist.val_identifier      = false; 
    vlist.val_vlist           = false; 
    vlist.val_expression      = false; 
    vlist.val_slist           = false; 
    vlist.val_verbdef         = false; 
    vlist.val_typdef          = false;
    vlist.val_array           = false;
    vlist.val_structure       = false;
    vlist.val_ref             = false;
    
    vlist.val_mixed           = false; 

    vlist.kw_expression       = false; 
    vlist.kw_vlist            = false;
    vlist.kw_identifier       = false; 

                                // note: does not set kw_eval_done flag


    // scan positional parms, turning on flags

    for (const value_S& value : vlist.values)
    {
             if (value.ty == type_E::unit         ) vlist.val_unit        = true; 
        else if (value.ty == type_E::boolean      ) vlist.val_boolean     = true;
        else if (value.ty == type_E::int8         ) vlist.val_int8        = true; 
        else if (value.ty == type_E::int16        ) vlist.val_int16       = true; 
        else if (value.ty == type_E::int32        ) vlist.val_int32       = true; 
        else if (value.ty == type_E::int64        ) vlist.val_int64       = true; 
        else if (value.ty == type_E::uint8        ) vlist.val_uint8       = true; 
        else if (value.ty == type_E::uint16       ) vlist.val_uint16      = true; 
        else if (value.ty == type_E::uint32       ) vlist.val_uint32      = true; 
        else if (value.ty == type_E::uint64       ) vlist.val_uint64      = true; 
        else if (value.ty == type_E::float32      ) vlist.val_float32     = true; 
        else if (value.ty == type_E::float64      ) vlist.val_float64     = true; 
        else if (value.ty == type_E::string       ) vlist.val_string      = true; 
        else if (value.ty == type_E::identifier   ) vlist.val_identifier  = true; 
        else if (value.ty == type_E::vlist        ) vlist.val_vlist       = true; 
        else if (value.ty == type_E::expression   ) vlist.val_expression  = true; 
        else if (value.ty == type_E::slist        ) vlist.val_slist       = true;     
        else if (value.ty == type_E::verbdef      ) vlist.val_verbdef     = true;   
        else if (value.ty == type_E::typdef       ) vlist.val_typdef      = true;
        else if (value.ty == type_E::array        ) vlist.val_array       = true;
        else if (value.ty == type_E::structure    ) vlist.val_structure   = true;
        else if (value.ty == type_E::ref          ) vlist.val_ref         = true;   
    }


    // scan evaluated / unevaluated keyword parms, turning on flags 

    if (vlist.eval_kws.size() > 0)       // if any evaluated keywords, use them
    {
        for (const auto& elem : vlist.eval_kws)           // elem is std::pair<kw name , value>
        {
                if (elem.second.ty == type_E::identifier    )   vlist.kw_identifier = true; 
           else if (elem.second.ty == type_E::expression    )   vlist.kw_expression = true; 
           else if (elem.second.ty == type_E::vlist         )   vlist.kw_vlist      = true;
        }      
    }
    else                                // no evaluated keywords -- do unevaluated ones (if any) instead 
    {
        for (const auto& elem : vlist.keywords)           // elem is keyword_S
        {
                if (elem.value.ty == type_E::identifier    )   vlist.kw_identifier = true; 
           else if (elem.value.ty == type_E::expression    )   vlist.kw_expression  = true; 
           else if (elem.value.ty == type_E::vlist         )   vlist.kw_vlist      = true;
        } 
    }


    // set "mixed" flag if more than one type of positional parm is present
    // --------------------------------------------------------------------

    int type_ct {0}; 
  
    if (vlist.val_unit         ) type_ct++;
    if (vlist.val_boolean      ) type_ct++;
    if (vlist.val_int8         ) type_ct++;
    if (vlist.val_int16        ) type_ct++;
    if (vlist.val_int32        ) type_ct++;
    if (vlist.val_int64        ) type_ct++;
    if (vlist.val_uint8        ) type_ct++;
    if (vlist.val_uint16       ) type_ct++;
    if (vlist.val_uint32       ) type_ct++;
    if (vlist.val_uint64       ) type_ct++; 
    if (vlist.val_float32      ) type_ct++; 
    if (vlist.val_float64      ) type_ct++; 
    if (vlist.val_string       ) type_ct++; 
    if (vlist.val_identifier   ) type_ct++; 
    if (vlist.val_vlist        ) type_ct++; 
    if (vlist.val_expression   ) type_ct++;
    if (vlist.val_slist        ) type_ct++;
    if (vlist.val_verbdef      ) type_ct++;
    if (vlist.val_typdef       ) type_ct++;
    if (vlist.val_array        ) type_ct++;
    if (vlist.val_structure    ) type_ct++;
    if (vlist.val_ref          ) type_ct++;

    if (type_ct > 1) 
        vlist.val_mixed = true; 

    return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_vlist_value() -- helper function to add value of pre-peek()ed token to passed-in vlist, expression, etc.
////                     -- handles one value -- called in loop    
////
////
////    -- passed in (unconsumed) token should be: 
////    -- expected tokens : 
////       -- literal value       -- positional parm added
////       -- keyword             -- keyword parm added (keyword name is literal)
////       -- open dot bracket    -- keyword parm added (keyword name is expression)
////       -- open paren          -- nested expression     -- becomes positional parm (to be evaluated and replaced by results later)
////       -- open bracket        -- nested vlist          -- becomes positional parm
////       -- open brace          -- nested slist          -- becomes positional parm  
////  
////       -- anything else is internal error 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_vlist_value(frame_S& frame, pre_parse_C& pp, a_expression_S& expression, vlist_S& vlist, const token_C& token) try
{ 
    // literal or identifier token -- process positional value
    // -------------------------------------------------------

    if ( (token.utype2 == tok_u2_E::value) || (token.utype2 == tok_u2_E::identifier) )
    {
        value_S value {}; 

        expression_add_token(expression, token);               // add to token list for passed-in expression 
        set_atom_token(value, token, static_N::token_ix);      // fill in value_S based on contents of peek()ed value token
        discard_token(pp);                                     // get rid of token with value
        add_positional_value(vlist, value);                    // add to positional parms vector in passed-in vlist  
        return 0;                                              // caller should loop back to look at next token           
    }


    // keyword token         -- process literal keyword name token   + following value
    // open_keyname_bracket  -- process keyword with name expression + following value
    // -------------------------------------------------------------------------------

    if ( (token.utype2 == tok_u2_E::keyname) || (token.utype2 == tok_u2_E::open_keyname_bracket) )
    {
        auto grc = get_keyword(frame, pp, expression, vlist);  // add keyword plus atom or nested vlist/expression/slist to keyword section of passed-in vlist
        if (grc != 0)                                          // end immediately if unexpected end or error
        {
            vlist = vlist_S {};                                //  return empty vlist 
            return -1;                                        
        } 

        return 0;                                              // caller should loop back to look at next token           
    }


    // open brace -- process nested slist 
    // ----------------------------------

    if (token.utype2 == tok_u2_E::open_brace)
    {
        value_S value        {};  
        slist_S new_slist    {}; 
        a_expression_S  dummy_expression {};                   // dummy expression to hold open brace token (for debug messages) 

        expression_add_token(dummy_expression, token);         // put open paren on brface stack for dummy expression 
        int64_t ix1 = static_N::token_ix;                      // capture starting token index for this value 
        discard_token(pp);                                     // get_nested_slist() does not want open brace -- will consume matching close brace 
        auto grc = get_nested_slist(frame, pp, new_slist);     // tokens for the nested slist don't go into current expression's token list  -- should consume ending brace
        if (grc != 0)                                          // end immediately if unexpected end or error
        {
            vlist = vlist_S {};                                //  return empty vlist 
            return -1; 
        } 

        int64_t ix2 = static_N::token_ix;                      // capture ending token index for this value
        set_slist_value(value, new_slist, true);               // move OK -- new_slist no longer needed 
        value.token_ix1 = ix1;                                 // save away starting token index                                
        value.token_ix2 = ix2;                                 // save away ending token index
        add_positional_value(vlist, value);                    // add to positional parms vector in passed-in vlist 
        return 0;                                              // caller should loop back to look at next token          
    }
    

    // open parenthesis -- process nested expression 
    // ----------------------------------------

    if (token.utype2 == tok_u2_E::open_paren)
    {
        value_S    value      {};  
        a_expression_S  new_expression  {}; 

        expression_add_token(new_expression, token);                       // put open paren on token stack for new expression 
        int64_t ix1 = static_N::token_ix;                                  // capture starting token index for this value 
        discard_token(pp);                                                 // get_nested_expression() does not want open paren -- will consume matching close paren 
        auto grc = get_nested_expression(frame, pp, new_expression);       // tokens for the nested expression don't go into current expression's token list  -- should consume ending paren
        if (grc != 0)                                                      // end immediately if unexpected end or error
        {                                                         
            vlist = vlist_S {};                                            //  return empty vlist 
            return -1;                                            
        }                                                         
                                                                  
        int64_t ix2 = static_N::token_ix;                                  // capture ending token index for this value
        set_expression_value(value, new_expression, true);                 // move OK -- new_expression no longer needed 
        value.token_ix1 = ix1;                                             // save away starting token index                                
        value.token_ix2 = ix2;                                             // save away ending token index
        add_positional_value(vlist, value);                                // add to positional parms vector in passed-in vlist 
        return 0;                                                          // caller should loop back to look at next token          
    }
        

    // open bracket -- process nested vlist 
    // ------------------------------------

    if (token.utype2 == tok_u2_E::open_bracket)
    {
        value_S value      {};  
        vlist_S new_vlist  {}; 

        expression_add_token(expression, token);                           // put open bracket on token list for existing expression 
        int64_t ix1 = static_N::token_ix;                                  // capture starting token index for this value 
        discard_token(pp);                                                 // get rid of open bracket
                                                                   
        auto grc = get_nested_vlist(frame, pp, expression, new_vlist);     // should consume ending bracket
        if (grc != 0)                                                      // end immediately if unexpected end or error
        {                                                          
            vlist = vlist_S {};                                            // return empty vlist 
            return -1;                                                     
        }                                                                  
                                                                           
        int64_t ix2 = static_N::token_ix;                                  // capture ending token index for this value
        set_vlist_value(value, new_vlist, true);                           // move OK -- new_vlist no longer needed 
        value.token_ix1 = ix1;                                             // save away starting token index                                
        value.token_ix2 = ix2;                                             // save away ending token index
        add_positional_value(vlist, value);                                // add to positional parms vector in passed-in vlist 
        return 0;                                                          // caller should loop back to look at next token           
    }


    // error -- input token is not expected -- should not happen (caller pre-screens all tokens)
    // -----------------------------------------------------------------------------------------

    M_out_emsg1(L"get_vlist_token(): passed-in token is of unexpected type:");
    token.display(L"unexpectred token");
    M_out_emsgz();
    M_throw("get_vlist_token(): passed-in token is of unexpected type")
    return -2;            // should not get here   
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_vlist() -- get list of positional and/or keyword values -- not nested
////                                                                  ----------
////
////    -- caller should not have consumed any tokens before calling this routine
////    -- expected tokens : 
////       -- semicolon              -- end vlist (not consumed)
////       -- verb                   -- end vlist (not consumed) -- verb name which ends this (left side) vlist  
////       -- open_verbname_bracket  -- end vlist (not consumed) -- start of verb which ends thls (lesft side) vlist 
////       -- END of input token     -- end vlist (not consumed)
////       -- close paren            -- end vlist (not consumed) -- normal end of nested expression containing this vlist 
////       -- close_verbname_bracket -- end vlist (not consumed) -- normal end of verb sub-expression containing this lvist  
////       -- close_keyname_bracket  -- end vlist (not consumed) -- normal end of keyword name sub-expression containing this vlist  
////       -- close brace            -- end vlist (not consumed) -- normal end of nested slist containing this vlist  
////       -- literal value          -- positional parm                                                              -- added to vlist positional parms list 
////       -- keyword                -- keyword parm (keyword name is literal)                                       -- added to vlist keyword parms list 
////       -- open_keyname_bracket   -- keyword parm (keyword name is expression that must be evaluated later)       -- added to vlist keyword parms list 
////       -- open paren             -- nested expression -- becomes positional parm (to be evaluated later)         -- added to vlist positional parms list 
////       -- open bracket           -- nested vlist -- becomes positional parm                                      -- added to vlist positional parms list 
////       -- open brace             -- nested slist -- becomes positional parm -- passed to verb with no evaluation -- added to vlist positional parms list 
////
////       -- note: close bracket  is not expected in unnested vlist -- will cause error 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_vlist(frame_S& frame, pre_parse_C& pp, a_expression_S& expression, vlist_S& vlist) try
{    
    // make sure vlist is not already in use

    if ( (vlist.value_ct != 0) || (vlist.kw_ct != 0) )
    {
        M_out_emsg1(L"get_vlist(): passed-in vlist is not empty: ");
        display_vlist(vlist, L"non-empty vlist");
        M_out_emsgz();
        M_throw("get_vlist(): passed-in vlist is not empty")
    }
 
    M__(M_out(L"get_vlist() -- token_ix at start = %d") % static_N::token_ix); 

    int64_t token_ix1 = -1;         // needs to be filled in after 1st peek    


    // ===========================================================================================
    // loop to process values in vlist -- should get 0 or more values, and then valid ending token
    // ===========================================================================================

    for(;;)
    {
        // look at next token -- end immediately (perhaps with empty expression), if error -- END is expected here 

        token_C token; 
        M__(M_out(L"get_vlist() calling peek_token");)
        auto prc = peek_token(pp, token, false);

        if (token_ix1 == -1) token_ix1 = static_N::token_ix;                   // capture starting token index on first pass only  

        if ((prc < 0))
        {
            M_out_emsg1(L"get_vlist(): Input token error while parsing un-nested vlist"); 
            token.display(L"bad-token", true);
            vlist.token_ix1 = std::min(static_N::token_ix, token_ix1);         // capture starting token index for vlist -- for error message
            vlist.token_ix2 = std::max(static_N::token_ix, token_ix1);         // capture ending token index for vlist   -- for error message 
            display_vlist(vlist, L"incomplete vlist");
            M_out_emsg2(L"Passing back empty vlist in place of incomplete vlist");
            
            vlist = vlist_S {};
            vlist.token_ix1 = std::min(static_N::token_ix, token_ix1);         // capture starting token index for vlist
            vlist.token_ix2 = std::max(static_N::token_ix, token_ix1);         // capture ending token index for vlist
            
            return prc;                                                        // leave unexpected token unconsumed for next time
        }           

        M__(token.display(L"get_vlist()");) 


        //  check for ending token or R/C
        //  -----------------------------

        if (
            (prc == 1)                                                           // END r/c
            ||                                                                  
            (token.utype2 == tok_u2_E::end)                                      // END 
            ||                                                                  
            (token.utype2 == tok_u2_E::semicolon)                                // semicolon -- must be end of this expression
            ||                                                                  
            (token.utype2 == tok_u2_E::close_brace)                              // closing brace -- must be end of whole slist  
            ||                                                                  
            (token.utype2 == tok_u2_E::verbname)                                 // verbname 
            ||                                                                  
            (token.utype2 == tok_u2_E::open_verbname_bracket)                    // start of verb sub-expression (handled like verbname) 
            ||                                                                  
            (token.utype2 == tok_u2_E::close_paren)                              // close paren            - normal end for nested expression 
            ||                                                                  
            (token.utype2 == tok_u2_E::close_verbname_bracket)                   // close verbname bracket - normal end for verb sub-expression 
            ||                                                                  
            (token.utype2 == tok_u2_E::close_keyname_bracket)                    // close keyname bracket  - normal end for keyword name sub-expression
           )
        {
            M__(M_out(L"get_vlist() -- ending normally: token_ix1=%d") % token_ix1;)

            vlist.token_ix1 = std::min(static_N::token_ix - 1, token_ix1);       // capture starting token index for vlist
            vlist.token_ix2 = std::max(static_N::token_ix - 1, token_ix1);       // capture ending token index for vlist
            return 0;                                                            // return normally, with whatever has been accumulated -- do not consume ending token
        }


        // process values in vlist -- call helper function

        if (
            (token.utype2 == tok_u2_E::value)
            ||
            (token.utype2 == tok_u2_E::open_paren)
            ||
            (token.utype2 == tok_u2_E::keyname)
            ||
            (token.utype2 == tok_u2_E::open_keyname_bracket)
            ||
            (token.utype2 == tok_u2_E::open_bracket)
            ||
            (token.utype2 == tok_u2_E::open_brace)
            ||
            (token.utype2 == tok_u2_E::identifier)
           )
        {
            auto rc = get_vlist_value(frame, pp, expression, vlist, token);        // pass through all parms
            if (rc != 0) 
            {
                vlist.token_ix1 = std::min(static_N::token_ix, token_ix1);         // capture starting token index for vlist
                vlist.token_ix2 = std::max(static_N::token_ix, token_ix1);         // capture ending token index for vlist
                return rc;                                                         // just return with error -- get_vlist_value() does all needed error processing 
            }
            continue;
        }
        

        // unexpected token seen -- pass back expression, as-is, after complaining  

        count_error();
        M_out_emsg1(L"get_vlist(): Unexpected token « %s » seen in un-nested vlist") % token.orig_str; 
        token.display(L"bad-token", true);
        vlist.token_ix1 = std::min(static_N::token_ix, token_ix1);                 // capture starting token index for vlist -- for error message
        vlist.token_ix2 = std::max(static_N::token_ix, token_ix1);                 // capture ending token index for vlist   -- for error message 
        display_vlist(vlist, L"incomplete vlist");                                  
        M_out_emsg2(L"Passing back empty vlist in place of incomplete vlist");      
                                                                                    
        vlist = vlist_S {};                                                        // return empty vlist
        vlist.token_ix1 = std::min(static_N::token_ix, token_ix1);                 // capture starting token index for vlist
        vlist.token_ix2 = std::max(static_N::token_ix, token_ix1);                 // capture ending token index for vlist
                                                                                    
        return -1;                                                                 // don't discard unexpected token -- leave for next call in get_expression()                 
                          // ================                                       
    }                     // end of main loop 
                          // ================
  
    return -2;            // should not get here
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_nested_vlist() -- get list of positional and/or keyword values 
////
////
////   -- caller must have already consumed starting bracket.
////   -- this routine consumes ending paren (if any)
////   -- don't expect semicolons or END tokens before ending close bracket.
////   -- don't expect verb or open_verbname_bracket in vlist, except for nested expression enclosed in parens
////   
////   -- expect the following: 
////      -- close bracket        (end of vlist)                 -- consumed before return  
////      -- literal value        (positional parm)
////      -- keyword              (keyword with literal name)
////      -- open dot bracket     (keyword with name that needs evaluation)
////      -- open paren           (nested expression)            -- becomes positional parm (to be evaluated and replaced by results later)
////      -- open bracket         (nested vlist)                 -- becomes positional parm
////      -- open brace           (nested slist)                 -- becomes positional parm -- passed to verb with no evaluation
////
////
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_nested_vlist(frame_S& frame, pre_parse_C& pp, a_expression_S& expression, vlist_S& vlist) try
{    
    // make sure vlist is not already in use

    if ( (vlist.value_ct != 0) || (vlist.kw_ct != 0) )
    {
        M_out_emsg1(L"get_nested_vlist(): passed-in vlist is not empty: ");
        display_vlist(vlist, L"non-empty vlist");
        M_out_emsgz();
        M_throw("get_nested_vlist(): passed-in vlist is not empty")
    }
 
    int64_t token_ix1 = static_N::token_ix;         // save starting token index -- should be opening brace (already consumed by caller)


    // ========================================================================================
    // loop to process values in vlist -- should get 0 or more values, and then a close bracket
    // ========================================================================================

    for(;;)
    {
        // look at next token -- end immediately (perhaps with empty expression), if error or unexpected end 

        token_C token; 

        M__(M_out(L"get_nested_vlist() calling peek_token");)
        auto prc = peek_token(pp, token, true);  // END is not expected here   


        // complain about unexpected END or error tokens

        if (prc != 0) 
        {
            if (prc == 1)
            {
                M_out_emsg1(L"get_vlist(): Unexpected END of input seen while parsing nested vlist -- possible missing closing bracket"); 
                token.display(L"unexpected END", true);
                vlist.token_ix1 = token_ix1;                                       // capture starting token index for vlist -- for error message
                vlist.token_ix2 = static_N::token_ix;                              // capture ending token index for vlist   -- for error message 
                display_vlist(vlist, L"incomplete vlist");
                M_out_emsg2(L"Passing back empty vlist in place of incomplete vlist");
            }
            else
            {  
                M_out_emsg1(L"get_vlist(): Input token error while parsing nested vlist"); 
                token.display(L"bad-token", true);
                vlist.token_ix1 = token_ix1;                                       // capture starting token index for vlist -- for error message
                vlist.token_ix2 = static_N::token_ix;                              // capture ending token index for vlist   -- for error message 
                display_vlist(vlist, L"incomplete vlist");
                M_out_emsg2(L"Passing back empty vlist in place of incomplete vlist");
            }
                                                       
            // pass back empty vlist with best guess for starting and ending token index
 
            vlist = vlist_S {};                                                    //  return empty vlist 
            vlist.token_ix1 = token_ix1;                                           //  set starting token index
            vlist.token_ix2 = static_N::token_ix;                                  //  set ending token index
            return -1;                                                             //  leave unexpected token (END) unconsumed for next time
        }           

        M__(token.display(L"get_nested_vlist()");) 


        //  check for ending close bracket

        if (token.utype2 == tok_u2_E::close_bracket)
        {
            expression_add_token(expression, token);                                // add to token list for this expression  
            discard_token(pp);                                                      // get rid of closing bracket 
            vlist.token_ix1 = token_ix1;                                            // set starting token index
            vlist.token_ix2 = static_N::token_ix;                                   // set ending token index
            return 0;                                                               // pass back whatever has already been accumulated in output vlist
        }    


        // process values in vlist -- call helper function

        if (
            (token.utype2 == tok_u2_E::value)
            ||
            (token.utype2 == tok_u2_E::open_paren)
            ||
            (token.utype2 == tok_u2_E::keyname)
            ||
            (token.utype2 == tok_u2_E::open_keyname_bracket)
            ||
            (token.utype2 == tok_u2_E::open_bracket)
            ||
            (token.utype2 == tok_u2_E::open_brace)
            ||
            (token.utype2 == tok_u2_E::identifier)
           )
        {
            auto rc = get_vlist_value(frame, pp, expression, vlist, token);         // pass through all parms
            if (rc != 0)                                                           
            {                                                                      
                vlist.token_ix1 = token_ix1;                                        // set starting token index
                vlist.token_ix2 = static_N::token_ix;                               // set ending token index
                return rc;                                                          // just return with error -- get_vlist_value() does all needed error processing    
            }
            continue;
        }


        // unexpected token seen -- pass back expression, as-is, after complaining  (includes semicolons, close_brace, etc.

        count_error();
        M_out_emsg1(L"get_nested_vlist(): Unexpected token « %s » seen in nested vlist") % token.orig_str; 
        display_expression(expression, L"bad-token");
        token.display(L"bad-token", true);
        display_vlist(vlist, L"incomplete vlist");
        M_out_emsg2(L"Passing back empty vlist in place of incomplete vlist");

        vlist = vlist_S {};                                                          // return empty vlist
        vlist.token_ix1 = token_ix1;                                                 // set starting token index
        vlist.token_ix2 = static_N::token_ix;                                        // set ending token index
                                                                                   
        return -1;                                                                   // don't discard unexpected token -- leave for next caller to consume  
                          // ================
    }                     // end of main loop 
                          // ================
  
    return -2;            // should not get here
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
 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//       V   V       A        L         U   U     EEEEE
//       V   V      A A       L         U   U     E    
//        V V      A   A      L         U   U     EEEE 
//        V V      AAAAA      L         U   U     E    
//         V       A   A      LLLLL      UUU      EEEEE
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_atom_token() -- put passed-in atom value (from token) into caller's value_S structure 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void set_atom_token(value_S& value, const token_C& token, int64_t ix) try
{
    // make sure value is not already set

    if (value.ty != type_E::none)
    {
        M_out_emsg1(L"set_atom(): passed-in value_S has already been set: ");
        display_value(value, L"already-set value");
        token.display(); 
        M_out_emsgz();
        M_throw("set_atom(): passed-in value_S has already been set")
    }
    

    // save away token starting and ending index, if known                                                                                                                                                                

    value.token_ix1 = ix; 
    value.token_ix2 = ix;


    // set proper flags and value, based on caller's token

    if      (token.type == token_E::unit)
    {
        value.ty        = type_E::unit; 
    } 
    else if (token.type == token_E::boolean)
    {
        value.boolean   = token.boolean; 
        value.ty        = type_E::boolean; 
    }   
    else if (token.type == token_E::int8)
    {
        value.int8      = token.int8; 
        value.ty        = type_E::int8; 
    }
    else if (token.type == token_E::int16)
    {
        value.int16     = token.int16; 
        value.ty        = type_E::int16;
    }
    else if (token.type == token_E::int32)
    {
        value.int32     = token.int32; 
        value.ty        = type_E::int32;
    }
    else if (token.type == token_E::int64)
    {
        value.int64     = token.int64; 
        value.ty        = type_E::int64;
    }
    else if (token.type == token_E::uint8)
    {
        value.uint8     = token.uint8; 
        value.ty        = type_E::uint8;
    }
    else if (token.type == token_E::uint16)
    {
        value.uint16    = token.uint16; 
        value.ty        = type_E::uint16;
    }
    else if (token.type == token_E::uint32)
    {
        value.uint32    = token.uint32; 
        value.ty        = type_E::uint32;
    }
    else if (token.type == token_E::uint64)
    {
        value.uint64    = token.uint64; 
        value.ty        = type_E::uint64;
    }
    else if (token.type == token_E::float32)
    {
        value.float32   = token.float32; 
        value.ty        = type_E::float32;
    }
    else if (token.type == token_E::float64)
    {
        value.float64   = token.float64; 
        value.ty        = type_E::float64;
    }
    else if (token.type == token_E::string)
    {
        value.string    = token.str;  
        value.ty        = type_E::string;
    }
    else if (token.utype2 == tok_u2_E::identifier)
    {
        value.string    = token.str;  
        value.ty        = type_E::identifier;
    }
    else if (token.utype2 == tok_u2_E::verbname)
    {
        value.string    = token.str;  
        value.ty        = type_E::verbname;
    }
    else if (token.utype2 == tok_u2_E::keyname)
    {
        value.string    = token.str;  
        value.ty        = type_E::keyname;
    }
    else
    {
        M_out_emsg1(L"set_atom_token(): unexpected token seen:"); 
        token.display();
        M_out_emsgz(); 
        M_throw("set_atom_token(): unexpected token seen")     
    }

    return; 
}
M_endf






////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_keyword_value() -- add passed-in keyword name/value to passed-in vlist 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
                                                                 
                                                         
void add_keyword_value(vlist_S& vlist, const value_S& kw_name, const value_S& kw_value) try
{
    // add new keyword to vector of unevaluated keywords 

    vlist.keywords.push_back(keyword_S { kw_name, kw_value });
    vlist.kw_ct++; 


    // set flag if this value is an identifier or anything else than needs evaluation 

         if (kw_value.ty == type_E::identifier    ) vlist.kw_identifier = true; 
    else if (kw_value.ty == type_E::expression    ) vlist.kw_expression = true; 
    else if (kw_value.ty == type_E::vlist         ) vlist.kw_vlist      = true; 

    return; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   add_positional_value() -- add positional passed-in value to passed-in vlist 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void add_positional_value(vlist_S& vlist, const value_S& value, bool update_token_ix) try
{
    //  Add passed-in value to back of vlist positional values vector  

    vlist.values.push_back(value);


    // update token indexes in vlist, if requested

    if (update_token_ix)
    {
        if (value.token_ix1 >= 0)               // only if incoming token_ix1 is valid
        {
            if ( (vlist.token_ix1 < 0) || ( value.token_ix1 < vlist.token_ix1) )
                vlist.token_ix1 = value.token_ix1;    
        }
      
        if (value.token_ix2 >= 0)              // only if incoming token_ix2 is valid
        {
            if ( (vlist.token_ix2 < 0) || ( value.token_ix2 > vlist.token_ix2) )
                vlist.token_ix2 = value.token_ix2;    
        }
    }


    // set type and "mixed" flags in vlist structure

         if (value.ty == type_E::int8       ) vlist.val_int8        = true;
    else if (value.ty == type_E::int16      ) vlist.val_int16       = true;
    else if (value.ty == type_E::int32      ) vlist.val_int32       = true;
    else if (value.ty == type_E::int64      ) vlist.val_int64       = true;
    else if (value.ty == type_E::uint8      ) vlist.val_uint8       = true;
    else if (value.ty == type_E::uint16     ) vlist.val_uint16      = true;
    else if (value.ty == type_E::uint32     ) vlist.val_uint32      = true;
    else if (value.ty == type_E::uint64     ) vlist.val_uint64      = true;
    else if (value.ty == type_E::float32    ) vlist.val_float32     = true;
    else if (value.ty == type_E::float64    ) vlist.val_float64     = true;
    else if (value.ty == type_E::string     ) vlist.val_string      = true;
    else if (value.ty == type_E::identifier ) vlist.val_identifier  = true;
    else if (value.ty == type_E::vlist      ) vlist.val_vlist       = true;
    else if (value.ty == type_E::expression ) vlist.val_expression  = true;
    else if (value.ty == type_E::slist      ) vlist.val_slist       = true;
    else if (value.ty == type_E::verbdef    ) vlist.val_verbdef     = true;
    else if (value.ty == type_E::typdef     ) vlist.val_typdef      = true; 
    else if (value.ty == type_E::array      ) vlist.val_array       = true;
    else if (value.ty == type_E::structure  ) vlist.val_structure   = true;
    else if (value.ty == type_E::ref        ) vlist.val_ref         = true;
    else if (value.ty == type_E::unit       ) vlist.val_unit        = true;
    else if (value.ty == type_E::boolean    ) vlist.val_boolean     = true;

    else     // should not get here
    {       
       M_out_emsg1(L"add_positional_value(): unexpected value passed in:");
       display_value(value, L"bad value");
       M_out_emsgz(); 
       M_throw("add_positional_value(): unexpected value passed in")      
    }


    // set "mixed"flag if appropriate -- note: identifier doesn't count here

    int type_ct {0}; 
    if (vlist.val_int8         ) type_ct++; 
    if (vlist.val_int16        ) type_ct++; 
    if (vlist.val_int32        ) type_ct++; 
    if (vlist.val_int64        ) type_ct++; 
    if (vlist.val_uint8        ) type_ct++; 
    if (vlist.val_uint16       ) type_ct++; 
    if (vlist.val_uint32       ) type_ct++; 
    if (vlist.val_uint64       ) type_ct++; 
    if (vlist.val_float32      ) type_ct++; 
    if (vlist.val_float64      ) type_ct++; 
    if (vlist.val_string       ) type_ct++; 
    if (vlist.val_vlist        ) type_ct++;
    if (vlist.val_expression   ) type_ct++;
    if (vlist.val_slist        ) type_ct++; 
    if (vlist.val_verbdef      ) type_ct++; 
    if (vlist.val_typdef       ) type_ct++; 
    if (vlist.val_array        ) type_ct++;
    if (vlist.val_structure    ) type_ct++;
    if (vlist.val_ref          ) type_ct++;
    if (vlist.val_unit         ) type_ct++;
    if (vlist.val_boolean      ) type_ct++;
    
    if (type_ct > 1) 
        vlist.val_mixed = true; 

    vlist.value_ct++;  
    return; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_keyword() -- get next token (expected to be keyword -or- open_keyname_bracket), and then get vlist (if any) 
////
////                               add keyword/value pair to vlist  
////
////
////
////   -- caller should not consume keyword/open_keyname_bracket token before calling this routine 
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_keyword(frame_S& frame, pre_parse_C& pp, a_expression_S& expression, vlist_S& vlist) try
{
    // get next token in stream, and check for end or errors -- should be the keyword itself -- end not expected

    token_C kw_token; 
    M__(M_out(L"get_keyword() calling peek_token");)
    auto prc = peek_token(pp, kw_token, true);
    if (prc != 0) return -1;
    int64_t kw_token_ix = static_N::token_ix;                         // starting index for keyword token 


    // --------------------------------------------------------------------------------------
    // Process keyword name token or expression following the keyword name or name expression
    // --------------------------------------------------------------------------------------
   
    value_S kw_name { };

    // handle literal keyword name

    if (kw_token.utype2 == tok_u2_E::keyname)
    {
        // build atomic (string) value from simple keyword name token

        set_atom_token(kw_name, kw_token, kw_token_ix);                       // save away simple keyword name token 


        // add this token to expression token list and discard it from the token stream

        expression_add_token(expression, kw_token);                           // add keyword token to expression token list 
        discard_token(pp);                                                    // get rid of keyword token, so value can be peek()ed      
    }   

    // handle keyword name expression

    else if (kw_token.utype2 == tok_u2_E::open_keyname_bracket)
    {
        // handle keyword-name expression --  <. keyword-name expression .> 

        a_expression_S kwname_expression { };                                  // local a_expression_S (in autodata) for gathering up verb_expression
                                                                              
        expression_add_token(kwname_expression, kw_token);                     // put open dot bracket on token stack for kw_expression 
        int64_t ix1 = static_N::token_ix;                                      // get starting token index for keyword name
        discard_token(pp);                                                     // get rid of open_keyname_bracket token before calling get_nested_kwname_expression()

        auto grc = get_nested_kwname_expression(frame, pp, kwname_expression); // fill in local kwname_expression  -- will consume ending close_keyname_bracket 
        if (grc != 0)
            return grc;                                                        // pass back error -- let caller handle error appropriately   
                                                                              
        int64_t ix2 = static_N::token_ix;                                      // get ending token index
        set_expression_value(kw_name, kwname_expression, true);                        // move OK -- kwname_expression no longer needed
        kw_name.token_ix1    = ix1;                                            // save away starting token index
        kw_name.token_ix2    = ix2;                                            // save away ending   token index
        kw_name.kw_token_ix  = kw_token_ix;                                    // save away keyword value token starting index  ???????
    }   

    // error -- unexpected token seen instead of keyword name of start of keyword name expression

    else
    {
        count_error();
        M_out_emsg1(L"get_keyword(): Unexpected token « %s » seen -- expecting keyword name token or open_keyname_bracket token") % kw_token.orig_str; 
        kw_token.display();
        display_expression(expression, L"unexpected token");
        M_out_emsgz();
        return -1;
    }


    // -------------------------------------------------------------------------------
    // Process value token or expression following the keyword name or name expression
    // -------------------------------------------------------------------------------

    // peek at next token following the keyword.  Expected tokens are: 
    //
    //   value token                  -- value for the keyword is the value token (consume token)
    //   identifier token             -- value for the keyword is the name of the identifier token (consume token)
    //   open paren                   -- value for this keyword is an expression (expression with verbs) 
    //   open bracket                 -- value for this keyword is a vlist (with 0-N values)
    //   open brace                   -- value for this keyword is an slist (with 0-N expressions/statements)
    //   END                          -- value for this keyword is nval (don't consume token)
    //   another keyword              -- value for this keyword is nval (don't consume token)
    //   another open_keyname_bracket -- value for this keyword is nval (don't consume token)
    //   close bracket                -- value for this keyword is nval (don't consume token)
    //   verb, option                 -- value for this keyword is nval (don't consume token)
    //   enything else                -- value for this keyword is nval (don't consume token)
    //
    //

    value_S kw_value {};                                              // new value_S for keyword value 


    // get next token from token stream  

    token_C value_token; 
    M__(M_out(L"get_keyword() calling peek_token");)
    prc = peek_token(pp, value_token, false);
    if (prc < 0) return prc;                                         // exit if error only -- continue processing if END -- will be passed to get_atom_vlist(), which will create empty vlist  
    

    // handle keyword value = value or identifier

    if (
         (value_token.utype2 == tok_u2_E::value)                     // atomic value for this keyword 
         ||
         (value_token.utype2 == tok_u2_E::identifier)                // value for this keyword is an identifier name (treat as atomic value) 
       )
    {
        expression_add_token(expression, value_token);               // add value token to expression tokens list 
        set_atom_token(kw_value, value_token, static_N::token_ix);   // value token is already available
        kw_value.kw_token_ix = kw_token_ix;                          // save away keyword token index
        discard_token(pp);                                           // get rid of value token, so next token can be peek()ed   
    }

    // handle keyword value = nested expression 

    else if (value_token.utype2 == tok_u2_E::open_paren)             // should be a (nested) expression as the keyword value 
    {                                                             
        a_expression_S new_expression {};                                     
                                                                  
        expression_add_token(new_expression, value_token);           // put open paren on token stack for new expression 
        int64_t ix1 = static_N::token_ix;                            // get starting token index
        discard_token(pp);                                           // get_nested_expression() does not want open paren -- will consume matching close paren 
                                                                  
        auto grc = get_nested_expression(frame, pp, new_expression); // tokens for the nested expression don't go into current expression's token list  -- should consume ending paren
        if (grc != 0)                                             
           return grc;                                               // pass back error -- let caller handle error appropriately 
                                                                  
        int64_t ix2 = static_N::token_ix;                            // get ending token index
        set_expression_value(kw_value, new_expression, true);                // move OK -- new_expression no longer needed
        kw_value.token_ix1    = ix1;                                 // save away starting token index
        kw_value.token_ix2    = ix2;                                 // save away ending   token index
        kw_value.kw_token_ix  = kw_token_ix;                         // save away keyword value token starting index
    }

    // handle keyword value = nested slist 

    else if (value_token.utype2 == tok_u2_E::open_brace)             // should be a (nested) slist as the keyword value 
    {                                                               
        slist_S new_slist {};                                       
        a_expression_S  dummy_expression {};                                    
                                                                    
        expression_add_token(dummy_expression, value_token);         // put open brace on dummy token stack for new expression 
        int64_t ix1 = static_N::token_ix;                            // get starting token index
        discard_token(pp);                                           // get_nested_slist() does not want open brace -- will consume matching close brace 
                                                                    
        auto grc = get_nested_slist(frame, pp, new_slist);           // tokens for the nested expression don't go into current expression's token list  -- should consume ending brace
        if (grc != 0)                                               
           return grc;                                               // pass back error -- let caller handle error appropriately 
                                                                    
        int64_t ix2 = static_N::token_ix;                            // get ending token index
        set_slist_value(kw_value, new_slist, true);                  // move OK -- new_slist no longer needed
        kw_value.token_ix1   = ix1;                                  // save away starting token index
        kw_value.token_ix2   = ix2;                                  // save away ending   token index
        kw_value.kw_token_ix = kw_token_ix;                          // save away keyword value token starting index
    }                                                               
                                                                    
    // handle keyword value = nested vlist                          
                                                                    
    else if (value_token.utype2 == tok_u2_E::open_bracket)           // should be (nested) vlist  as the keyword value
    {                                                               
        vlist_S new_vlist {};                                       
                                                                    
        expression_add_token(expression, value_token);               // put open bracket on token stack for new expression 
        int64_t ix1 = static_N::token_ix;                            // get starting token index
        discard_token(pp);                                           // get rid of open bracket before calling get_nested_vlist()

        auto grc = get_nested_vlist(frame, pp, expression, new_vlist);   // should consume ending bracket
        if (grc != 0)
           return grc;                                               // pass back error -- let caller handle error appropriately 
                                                                   
        int64_t ix2 = static_N::token_ix;                            // get ending token index
        set_vlist_value(kw_value, new_vlist, true);                  // move OK -- temp_vlist no longer needed
        kw_value.token_ix1   = ix1;                                  // save away starting token index
        kw_value.token_ix2   = ix2;                                  // save away ending   token index
        kw_value.kw_token_ix = kw_token_ix;                          // save away keyword  value token starting index
    }                                                              
                                                                   
    // handle keyword with no following value                             
                                                                   
    else                                                             // any other token
    {
        // no value for this keyword -- nval value_S will be added down below
        // stopping token is not consumed (or added to passed-in expression token-list) 
      
        kw_value = nval_val();
        kw_value.kw_token_ix = kw_token_ix;                          // save away keyword token index in value_S
    }   


    // ------------------------------------------------------------------------------------------------------
    // Add keyword and temporary value to passed-in vlist -- value will be copied into multimap via emplace()
    // ------------------------------------------------------------------------------------------------------

    add_keyword_value(vlist, kw_name, kw_value);                     // add keyword-name + value to keyword vector in vlist 

    return 0; 
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


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   token_list_at() -- access token list -- return n-th token
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

token_C token_list_at(std::vector<token_C>::size_type n) try
{
    // return empty token, if n it past end of token list 

    if ( (n < 0) || (n >= static_N::token_list.size()) )
        return token_C { };                                  // return empty token, if n it past end of static_N::token_list 
    else
        return static_N::token_list.at(n);                   // otherwise, return located token from static_N::token_list 
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   loc_str() -- return printable start/end location based on passed-in start/end token indexes (helper routine)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static std::wstring loc_str(int64_t ix1, int64_t ix2 = -1) try
{
    std::wstring start_pos    {}; 
    std::wstring start_source {}; 
    std::wstring end_pos      {}; 
    std::wstring end_source   {};

    int64_t start_source_id   {-1}; 
    int64_t   end_source_id   {-1};
   

    // handle case where neither token index is known

    if ( (ix1 < 0) && (ix2 < 0) )
        return L"unknown location in input stream";     
    

    // set locations when only 1st token index is known

    if ( (ix1 >= 0) && (ix2 < 0) )
    {
        token_C token = static_N::token_list.at(ix1);
    
        start_source_id = token.source_id1;  
        start_source    = L"«" + get_cached_id(token.source_id1) + L"»";
        start_pos       = L" -- start line:column = " + std::to_wstring(token.lineno1) + L":" + std::to_wstring(token.linepos1); 
        end_source_id   = token.source_id2;  
        end_source      = L"«" + get_cached_id(token.source_id2) + L"»";
        end_pos         = L" -- end line:column = "   + std::to_wstring(token.lineno2) + L":" + std::to_wstring(token.linepos2);
    }
    
    
    // set locations when only 2nd token index is known

    if ( (ix1 < 0) && (ix2 >= 0) )
    {
        token_C token = static_N::token_list.at(ix2);
     
        start_source_id = token.source_id1;  
        start_source    = L"«" + get_cached_id(token.source_id1) + L"»";
        start_pos       = L" -- start line:column = " + std::to_wstring(token.lineno1) + L":" + std::to_wstring(token.linepos1);
        end_source_id   = token.source_id2;  
        end_source      = L"«" + get_cached_id(token.source_id2) + L"»";
        end_pos         = L" -- end line:column = "   + std::to_wstring(token.lineno2) + L":" + std::to_wstring(token.linepos2);    
    }


    // set locations when both 2nd token index is known

    if ( (ix1 >= 0) && (ix2 >= 0) )
    {
        token_C token1 = static_N::token_list.at(ix1);
        token_C token2 = static_N::token_list.at(ix2);
     
        start_source_id = token1.source_id1;  
        start_source    = L"«" + get_cached_id(token1.source_id1) + L"»";
        start_pos       = L" -- start line:column = " + std::to_wstring(token1.lineno1) + L":" + std::to_wstring(token1.linepos1);
        end_source_id   = token2.source_id2;  
        end_source      = L"«" + get_cached_id(token2.source_id2) + L"»";
        end_pos         = L" -- end line:column = "   + std::to_wstring(token2.lineno2) + L":" + std::to_wstring(token2.linepos2);    
    }

    std::wstring loc {}; 

    if (start_source_id != end_source_id)
        return  L"starting source = " + start_source + start_pos + L" -- ending_source = " + end_source + end_pos;   
    else    
        return  L"input source = "    + start_source + start_pos                                        + end_pos;       
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   value_loc_str() -- return printable location of start and end of value in input source 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring value_loc_str(const value_S& value) try
{
    return loc_str(value.token_ix1, value.token_ix2);    
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   kw_loc_str() -- return printable location of start and end of keyword in input source 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring kw_loc_str(const value_S& value) try
{
    return loc_str(value.kw_token_ix);
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   vlist_loc_str() -- return printable location of start and end of value in input source 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring vlist_loc_str(const vlist_S& vlist) try
{
    return loc_str(vlist.token_ix1, vlist.token_ix2);    
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   verb_loc_str() -- return printable location of start and end of verb in expression in input source 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring verb_loc_str(const a_expression_S& expression) try
{
    return loc_str(expression.verb_value.token_ix1, expression.verb_value.token_ix2);      
}
M_endf

M_EX_IMPEXP                                                          // usable via of ex_interface.h
std::wstring verb_loc_str(const e_expression_S& eval_expression) try
{
    return loc_str(eval_expression.verb_token_ix1, eval_expression.verb_token_ix2);      
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   expression_loc_str() -- return printable location of start and end of expression in input source 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring expression_loc_str(const a_expression_S& expression) try
{
    return loc_str(expression.token_ix1, expression.token_ix2);   
}
M_endf

M_EX_IMPEXP                                                          // usable via of ex_interface.h
std::wstring expression_loc_str(const e_expression_S& eval_expression) try
{
    return loc_str(eval_expression.token_ix1, eval_expression.token_ix2);   
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////   functions to add location data to error messages
////   ------------------------------------------------
////
////   msg_loc(xx_expression_S          ) -- put out part of an error message with verb and expression location 
////   msg_loc(value_S, xx_expression_S ) -- put out part of an error message with value, verb, and expression location 
////   msg_loc(value_S                  ) -- put out part of an error message with value location 
////   msg_loc(value_S, string          ) -- put out part of an error message with value location -- use passed-in text instead of "value"  
////   msg_loc(vlist_S                  ) -- put out part of an error message with vlist location 
////
////   note: msgend_xxx() versions also close out the open multi-line error message
////
////   msg_kw_loc(value_S               ) -- put out part of an error message with keyword value location (can't overload, since input type is same as regular value)  
////   
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


void msg_loc(const value_S& value) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"       } % value_loc_str(value);   
}
M_endf



void msg_loc(const value_S& value, const std::wstring& ws) try
{
    M_out(L"%20s location -- %s" ) % ws % value_loc_str(value);   
}
M_endf



void msg_loc(const vlist_S& vlist) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"       } % vlist_loc_str(vlist);   
}
M_endf



void msg_loc(const vlist_S& vlist, const std::wstring& ws) try
{
    M_out(L"%20s location -- %s" ) % ws % vlist_loc_str(vlist);   
}
M_endf



void msg_loc(const a_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);     
}
M_endf



M_EX_IMPEXP                                                          // usable outside of ex_dll, via ex_interface.h
void msg_loc(const e_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);        
}
M_endf



void msg_loc(const value_S& value, const a_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"      } %      value_loc_str(value     );
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);     
}
M_endf



void msg_loc(const value_S& value, const e_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"      } %      value_loc_str(value     );
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);     
}
M_endf



void msg_loc(const vlist_S& vlist, const a_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"      } %      vlist_loc_str(vlist     );
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);     
}
M_endf



void msg_loc(const vlist_S& vlist, const e_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"      } %      vlist_loc_str(vlist     );
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);     
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////


void msg_kw_loc(const value_S& value) try
{
    M_out(L"%20s location -- %s" ) % std::wstring{ L"keyword"      } % kw_loc_str(value);   
}
M_endf
      

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void msgend_loc(const value_S& value) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"       } % value_loc_str(value);
    M_out_emsgz();
}
M_endf



void msgend_loc(const value_S& value, const std::wstring& ws) try
{
    M_out(L"%20s location -- %s" ) % ws % value_loc_str(value); 
    M_out_emsgz();
}
M_endf



void msgend_loc(const vlist_S& vlist) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"       } % vlist_loc_str(vlist);
    M_out_emsgz();
}
M_endf



void msgend_loc(const vlist_S& vlist, const std::wstring& ws) try
{
    M_out(L"%20s location -- %s" ) % ws % vlist_loc_str(vlist);
    M_out_emsgz();
}
M_endf


void msgend_loc(const a_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);
    M_out_emsgz();
}
M_endf



M_EX_IMPEXP                                                          // usable outside of ex_dll, via ex_interface.h
void msgend_loc(const e_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);
    M_out_emsgz();
}
M_endf



void msgend_loc(const value_S& value, const a_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"      } %      value_loc_str(value     );
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);
    M_out_emsgz();
}
M_endf



void msgend_loc(const value_S& value, const e_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"      } %      value_loc_str(value     );
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression); 
    M_out_emsgz();
}
M_endf


void msgend_loc(const vlist_S& vlist, const a_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"      } %      vlist_loc_str(vlist     );
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);
    M_out_emsgz();
}
M_endf



void msgend_loc(const vlist_S& vlist, const e_expression_S& expression) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"      } %      vlist_loc_str(vlist     );
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %       verb_loc_str(expression);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % expression_loc_str(expression);
    M_out_emsgz();
}
M_endf





//_________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""