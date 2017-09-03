﻿// ex_parse.cpp

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


///////////////////// a_vexpr_S-oriented  internalfunctions

static int     get_verb_priority(frame_S&, const std::wstring&); 
static void    vexpr_add_token(            a_vexpr_S&, const token_C&);

static int     get_vexpr(                 frame_S&, pre_parse_C&, a_vexpr_S&, bool, bool, bool, bool );
static int     get_unnested_vexpr(        frame_S&, pre_parse_C&, a_vexpr_S&, bool = true            );
static int     get_nested_vexpr(          frame_S&, pre_parse_C&, a_vexpr_S&                         );
static int     get_nested_verbname_vexpr( frame_S&, pre_parse_C&, a_vexpr_S&                         );
static int     get_nested_kwname_vexpr(   frame_S&, pre_parse_C&, a_vexpr_S&                         );


///////////////////// vlist_S-oriented internal functions 

static int     get_vlist_value(   frame_S&, pre_parse_C&, a_vexpr_S&, vlist_S&, const token_C&);
static int     get_vlist(         frame_S&, pre_parse_C&, a_vexpr_S&, vlist_S&                );
static int     get_nested_vlist(  frame_S&, pre_parse_C&, a_vexpr_S&, vlist_S&                );


//////////////////////  value_S-oriented internal functions

static void    set_atom_token( value_S&, const token_C&, int64_t = -1);

static int     get_keyword(frame_S&,        pre_parse_C&, a_vexpr_S&, vlist_S&);


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
static std::vector<token_C>                   token_list       {     };     // list of tokens -- may contain duplicates if a token is added to an vexpr more than once
static bool                                   just_peeked      {false};     // 1st time, peek gets a new token
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   parse_setup() -- set up parsing environment -- external function
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

uint64_t parse_setup(pre_parse_C& pp, frame_S& frame) try
{
    // do additional pre-parser setup, with parse-oriented options 
                                                                             // note that ch_sigil_keyword and ch_sigil_label are the same character // 
    std::vector<char32_t> leading_sigils         { const_N::ch_sigil_verb,   const_N::ch_sigil_label,   const_N::ch_sigil_parm, const_N::ch_sigil_keyword  };       //  leading verb, parm, and label sigils (combined list)
    std::vector<char32_t> leading_ident_sigils   { const_N::ch_sigil_verb,   const_N::ch_sigil_label,   const_N::ch_sigil_parm                             };       //  leading verb, parm, and label sigils (for identifiers)
    std::vector<char32_t> leading_oper_sigils    {                                                      const_N::ch_sigil_parm                             };       //  leading       parm,           sigil  (for operators)
    std::vector<char32_t> trailing_ident_sigils  { const_N::ch_sigil_keyword                                                                               };       //  trailing keyword sigils              (for identifiers)
    std::vector<char32_t> paren_sigils           { const_N::ch_sigil_verb,   const_N::ch_sigil_keyword                                                     };       //  leading/trailing verb sigil          (for parenthesis)
                                                                                                                                                           
    pp.set_combine_strings(        false                 );                                                                                                         //  don't combine adjacent strings separated by whitespace    
                                                                                                                                                                  
    pp.set_leading_sigils(         leading_sigils       );                                                                                                          // add combined list of leading sigils          (in addition to any pre-parser sigils)
    pp.set_leading_ident_sigils(   leading_ident_sigils );                                                                                                          // add list of leading sigils  for identifiers  (in addition to any pre-parser sigils)
    pp.set_trailing_ident_sigils( trailing_ident_sigils );                                                                                                          // add list of trailing sigils for identifiers  (in addition to any pre-parser sigils)
    pp.set_leading_oper_sigils(    leading_oper_sigils  );                                                                                                          // add list of leading sigils  for operators    (in addition to any pre-parser sigils)
    pp.set_paren_sigils(           paren_sigils         );                                                                                                          // add list oif sigils for parenthesis          (in addition to any pre-parser sigils)
                                                                                                                                      

    // fill in ID=0 position in source-ID cache to catch uninitialized source ID fields
    
    add_cached_id(L"????????????????????? uninitialized source_id=0 ????????????????????????");             


    // set up predefined verbs and typdefs in global symbol table

    add_predefined_verbs();
    add_predefined_typdefs();

    return error_count(); 
}
M_endf


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
 
uint64_t process_cmdline(pre_parse_C& pp, frame_S& frame, symtab_S& st, int argc, wchar_t *argv[]) try
{
    // set up pre-defined pre-processor constants from command line arguments
    // ----------------------------------------------------------------------

    for (auto i = 0; i < argc; i++) 
        pp.set_var(fmt_str(L"_%d", i), argv[i], true);    // set pre-defined pre-processor constants _nn from n-th arg on command line -- _0    _1    _2    _3   etc. 
    

    // set up topmost (main) stack frame with command line arguments
    // -------------------------------------------------------------

    main_eval(frame, argc, argv);                         // set up main stack frame with command line args    
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
        return error_count();
    }


    // attach main imbed file to pre_parser stream -- check for errors 
    // --------------------------------------------------------------- 

    auto arc = pp.attach_file(imbed_filename);
    if (arc != 0) 
    {
        count_error();
        M_out_emsg(L"process_cmdline(): error R/C from pre_parse_C::attach_file(\"%S\") = %d -- cannot continue") % imbed_filename % arc;  
        return error_count();
    }

    return error_count(); 
}
M_endf    



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   process_input() -- parse and execute the passed-in input pre-parse stream --- external function
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

uint64_t process_input(pre_parse_C& pp, frame_S& frame) try
{
    M_start_time(t1)
 

    // -------------------------------------------
    // parse the main slist_S in input file stream
    // -------------------------------------------
                                                                                    
    slist_S slist {};               // main vexpr list 

    auto grc = get_unnested_slist(frame, pp, slist); 
    count_error(pp.errors()); 

    M_out(L"pre-process and parse phases complete: %50t elapsed_time = %.6f seconds") % M_elapsed_time(t1); 
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

        auto et = M_elapsed_time(t2);

        M_out( L"evaluation phase complete: verb calls = %d %50t elapsed_time = %.6f seconds -- verbs/sec = %.1f" ) 
               % frame.verb_eval_ct % et % (((float64_T)(frame.verb_eval_ct))/et);
        M_out( L"                         : %50t total stack frames = %d, max stack depth = %d" ) 
               % get_frame_serial() % get_frame_max_depth(); 
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

int parse_string(pre_parse_C& pp, frame_S& frame, slist_S& out_slist, const std::wstring& in_string, const std::wstring& in_name) try
{
    M__(M_out(L"parse_string() -- called");)


    // attach passed-in string to pre_parse_C -- if error, return empty slist_S and error r/c
    // --------------------------------------------------------------------------------------

    pp.reuse();                       // prepare pre_parse_C for reuse 

    auto arc = pp.attach_string(in_string, in_name);

    if (arc != 0)
    {
        M_out_emsg(L"parse_string() -- error trying to attach passed-in string -- empty slist_S is being returned");

        out_slist = slist_S { };      // return empty slist
        return -1;                    // error r/c
    }
  

    // parse newly-attached string (in passed-in pre_parse_C) using passed-in stack frame to obtain unnested slist
    // -----------------------------------------------------------------------------------------------------------

    auto grc = get_unnested_slist(frame, pp, out_slist); 
    
    M__(display_slist(out_slist, L"parsed slist", L"", true, false);)


    // if parsing error, return empty slist_S and r/c other than END
    // -------------------------------------------------------------

    if (grc != 1)
    {
        M_out_emsg(L"parse_string() -- error r/c = %d during parsing of passed-in string -- empty slist_S is being returned") % grc;

        out_slist = slist_S { };      // return empty slist
        return -1;                    // error r/c
    }

    return 0; 
}
M_endf





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                                                       


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
////      r/c = -1 -- error token   -- error message -- no token passewd back
////      r/c = 0  -- normal token  -- token passed back
////      r/c = 1  -- END token     -- error message if end_expected parm = false, token passsed back (may be empty 
////                                   except for user type field, if pre-parser passed back no token)
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
            M_out_emsg1(L"peek_token(): unexpected END token (end of input) from pre-parser was seen"); 
            token.display();
            M_out_emsg2(L"END halts all expression processing immediately");
        }        

        tok = token;            // pass back token (may be empty except for user type)
        return 1;
    }


    // otherwise, token is normal -- need to set user type field based on main type field
    // ---------------------------------------------------------------------------------- 

    // handle tokens without sigils 

    if ( (!token.has_leading_sigil) && (!token.has_trailing_sigil) )
    {
        // check identifier tokens with no sigils -- identifier

        if ( (token.type == token_E::identifier) || (token.type == token_E::extended_identifier) )
        {
            token.utype2 = tok_u2_E::identifier;    // "plain identifier" 
        }


        // check operator tokens with no sigils -- verbname

        else if (token.type == token_E::oper)      // operators are verbs
        {
            token.utype2 = tok_u2_E::verbname;     // verb to be invoked
        }


        // check for value-type or literal  tokens -- expect only:      10000,      -10000,      1.2345,    "abcd efg",      -or-     °    ( empty value)

        else if (
                 (token.type == token_E::string          )
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
                 ||
                 ( (token.type == token_E::misc_char) && (token.only_char == const_N::ch_empty_value) )    // empty value char
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
                token.utype2 = tok_u2_E::open_keyname_bracket;    // handle whole expression like a keyword name 
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
 
 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
////   get_slist() -- get list of vexprs
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

    // ============================================================================ 
    // main loop to fetch vexprs/statements, until end of statement list is reached
    // ============================================================================
    
    bool first_pass {true}; 

    for (;;)
    {
        a_vexpr_S vexpr {}; 


        // do peek() to check for ending token

        token_C token {}; 
        auto prc = peek_token(pp, token, !end_expected);
        M__(M_out(L"get_slist() -- peek token r/c=%d") % prc;) 
        M__(token.display(L"get_slist() -- after peek_token()");) 

        if (prc != 0) return prc;           // return immediately, if error or END -- pass back whatever has been accumulated so far in slist 
        

        if (token.utype2 == tok_u2_E::close_brace)
        {
            if (end_by_brace)
            {
                a_vexpr_S dummy_vexpr  {};   // dummy vexpr to add ending brace token to (for ending tokens index  in debug messages) 
                vexpr_add_token(dummy_vexpr, token);

                discard_token(pp);           // consume ending brace
                M__(M_out(L"<=================== get_slist() returning after closing brace -- r/c=%d") % rc;)
                return rc;                   // return with whatever has been accumulated so far      
            }
            else                             // brace is unexpected
            {
                count_error(); 
                M_out_emsg1(L"get_slist(): unexpected closing brace found in unnested slist"); 
                token.display(L"unexpected");
                M_out_emsg2(L"closing brace is ignored");  
                discard_token(pp);          // get rid of unexpected brace, so loop can continue
                rc = -1;                    // remember error 
            }
        }


        // handle labels

        if (token.utype2 == tok_u2_E::label)
        {
            M__(M_out(L"get_slist() -- label token seen");)        

            // it this is first token, set main label for this slist

            if (first_pass)
                slist.label = token.str; 


            //  add label to labels list -- vexpr index will be at()-index for next vexpr going into vexpr vector (may point one past end of vexprs, if no vexprs follow this label)

            slist.labels.emplace(token.str, slist.vexprs.size()); 
        
            discard_token(pp);  
            first_pass = false; 
            continue; 
        }


        // get next vexpr in list -- return immediately, if error or END

        auto grc = get_unnested_vexpr(frame, pp, vexpr, end_expected);    // r/c:  0=ok, 1=END, -1=error, 2=bad vexpr -- for r/c=0 or2,  ending semicolon consumed, closing brace not
        M__(M_out(L"get_slist() -- r/c from get_vexpr() = %d") % grc;)
        M__(M_out(L"-------- start of expression ------------");)
        M__(display_vexpr(vexpr, L"get_slist() -- from get_vexpr()");) 
        M__(M_out(L"========= end of expression =============");)


        if (grc < 0) rc = -1;             // error?  (end is expected, so valid vexpr should have been passed back) -- END is unconsumed  
        

        // handle valid vexpr passed back from get_vexpr()

        if ( (grc == 0) || (grc == 1) )            // normal token or normal token + END
        {
            M__(M_out(L"get_slist() -- adding expression passed-back from get_vexpr() to slist");)
            M__(display_vexpr(vexpr, L"get_slist() -- added vexpr");) 

            slist.vexprs.push_back(vexpr);
            slist.vexpr_ct++; 
        }
    
        first_pass = false; 

        //   ================ 
    }   //   end of main loop 
        //   ================

    M__(M_out(L"<=================== get_slist() returning ???????????? -- r/c=%d") % rc;)
    return rc;   // shouldn't get here
}
M_endf





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//        V   V     CCCC      A      L         L    
//        V   V    C         A A     L         L    
//        V   V    C        A   A    L         L    
//         V V     C        AAAAA    L         L    
//          V       CCCC    A   A    LLLLL     LLLLL
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   vexpr_add_token() -- add token to vexpr start/end token indexes -- for debugging
////   =================
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void vexpr_add_token(a_vexpr_S& vexpr, const token_C& token) try
{  
    // update first/last token indexes in vexpr

    if (vexpr.token_ix1 == -1)     // not set yet?
        vexpr.token_ix1 = static_N::token_ix;
    else
        vexpr.token_ix1 = std::min(static_N::token_ix, vexpr.token_ix1);    // lowest token index yet seen
    
    vexpr.token_ix2 = std::max(static_N::token_ix, vexpr.token_ix2);        // highest token index yet seen  

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
////   vexpr_set_verb() -- add simple or complex verbname-oriented fields to newly constructed vexpr
////   ================
////
////   Assume there is no verb already in the vexpr (left plist may be present in some cases)
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

///////////////////////////////
// version with passed-in token -- token type must be verbname
///////////////////////////////

void vexpr_set_verb(frame_S& frame, a_vexpr_S& vexpr, const token_C& token, const verbdef_S& verbdef) try
{  
    // make sure there is no verb already saved away in this vexpr

    if (vexpr.has_verb)
    {
        M_out_emsg1(L"vexpr_set_verb() -- trying to set verb into the expression, but a verb is already present for this expression");
        display_vexpr(vexpr, L"already-has-verb");
        M_out_emsgz(); 
        M_throw("vexpr_set_verb() -- trying to set verb into the expression, but a verb is already present for this expression")
    }


    //    make sure passed-in token is a simple verbmame

    if (token.utype2 != tok_u2_E::verbname)
    {
        M_out_emsg1(L"vexpr_set_verb() -- trying to set verb into the expression, but token is not a verb name");
        M_out_emsgz(); 
        M_throw("vexpr_set_verb() -- trying to set verb into the expression, but token is not verb name")
    }
      

    // save away verbname token, etc. in this vexpr

    vexpr.has_verb = true;
    vexpr_add_token(vexpr, token);                               // add verb token to vexpr    
    set_atom_token(vexpr.verb_value, token, static_N::token_ix); // fill in verb_value with simple verb name value
    
    if (token.has_leading_sigil)
    {
        vexpr.has_sigil = true;                                  // set sigil flag for debug messsages
        vexpr.sigil     = token.leading_sigil;                   // save away sigil, too
    }  


    // save away verb priority and associativity (if verb is defined at this point -- otherwise empty verbdef is passed in) 

    vexpr.priority        = verbdef.priority;                    // save away verb priority 
    vexpr.right_associate = verbdef.right_associate;             // save away verb associativity 
    vexpr.left_associate  = verbdef.left_associate;              // save away verb associativity

    return; 
}
M_endf


///////////////////////////////////////
// version with passed-in verb sub-vexpr  (evaluation of this must yield a string later on, or else a run-time error occurs)  
///////////////////////////////////////

void vexpr_set_verb(frame_S& frame, a_vexpr_S& vexpr, const a_vexpr_S& verb_vexpr, bool move_ok) try
{  
    // make sure there is no verb already saved away in this vexpr

    if (vexpr.has_verb)
    {
        M_out_emsg1(L"vexpr_set_verb() -- trying to set verb into the expression, but a verb is already present for this expression");
        display_vexpr(vexpr, L"already-has-verb");
        M_out_emsgz(); 
        M_throw("vexpr_set_verb() -- trying to set verb into the expression, but a verb is already present for this expression")
    }
      

    // save away verbname token, etc. in this vexpr

    vexpr.has_verb = true; 
    set_vexpr_value(vexpr.verb_value, verb_vexpr, move_ok);   


    // update location info in passed-in main token to include verb sub-token limits

    if (vexpr.token_ix1 == -1)                                                 // not set yet?
        vexpr.token_ix1 = verb_vexpr.token_ix1;                                // verb sub-vexpr must be at start of main vexpr 
    else
        vexpr.token_ix1 = std::min(verb_vexpr.token_ix1, vexpr.token_ix1);     // lowest token index yet seen
    
    vexpr.token_ix2 = std::max(verb_vexpr.token_ix2, vexpr.token_ix2);         // highest token index yet seen   


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
////   get_vexpr() -- fetch next vexpr from input token stream 
////
////
////   rc = -2  -- should-not occur error
////   rc = -1  -- error from pre_parse
////   rc = 0   -- normal 
////   rc = 1   -- END seen (not consumed)
////   rc = 2   -- empty vexpr passed back because of unexpected tokens, etc.error
////
////
////   note: input/output is vexpr is expected to be completely empty or contain left parms and verb. (no leftovers from last time) 
////
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// this routine consumes ending semicolon (but not END token)

static int get_unnested_vexpr(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr, bool end_expected) try
{
    M__(M_out(L"get_unnested_vexpr() called");)
    return get_vexpr(frame, pp, vexpr, false, false, false, end_expected);
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initial open paren must be consumed already -- this routine consumes matching close paren 

static int get_nested_vexpr(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr) try
{
    M__(M_out(L"get_nested_vexpr() called");)
    return get_vexpr(frame, pp, vexpr, true, false, false, false);
}
M_endf


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initial open angle bracket must be consumed already -- this routine consumes matching close angle bracket  

static int get_nested_verbname_vexpr(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr) try
{
    M__(M_out(L"get_nested_verbname_vexpr() called");)
    return get_vexpr(frame, pp, vexpr, false, true, false, false);
}
M_endf

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// initial open dot bracket must be consumed already -- this routine consumes matching close dot bracket  

static int get_nested_kwname_vexpr(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr) try
{
    M__(M_out(L"get_nested_kwname_vexpr() called");)
    return get_vexpr(frame, pp, vexpr, false, false, true, false);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////  helper function for main routine

static int get_vexpr_verb(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr, const token_C& token, const verbdef_S& verbdef) try
{
   if (token.utype2 == tok_u2_E::verbname)
   {
       vexpr_set_verb(frame, vexpr, token, verbdef);                // add token directly to vexpr verbname value field   
       discard_token(pp);                                           // get rid of verb/option token
   }
   else                                                             // open angle bracket is start of verb sub-vexpr
   {
       // add in <verb sub-vexpr > to passed-in (main) vexpr

       discard_token(pp);                                           // get rid of open angle bracket token before calling get_nested_verbname_vexpr()

       a_vexpr_S verb_vexpr { };                                    // local a_vexpr_S (in autodata) for gathering up verb_vexpr
       auto grc = get_nested_verbname_vexpr(frame, pp, verb_vexpr); // fill in verb_vexpr 
       if (grc != 0)
           return grc;                                              // pass back error -- let get_vexpr() handle error appropriately (should go into flush mode)  

       vexpr_set_verb(frame, vexpr, verb_vexpr, true);              // OK to move data from verb_vexpr, since it is going away very soon  
   } 
   
   return 0 ;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////// main routine, called by above three routines //////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int get_vexpr(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr, bool end_by_paren, bool end_by_verbname_bracket, bool end_by_keyname_bracket, bool end_expected) try
{
    bool flushing           { false };                                                // true -- flushing tokens until end reashed 
    bool left_parms_done    { false };                                                // true -- left side parms have been seen
    bool right_parms_done   { false };                                                // true -- right side parms have been seen
    bool verb_done          { false };                                                // true -- verb has been added to vexpr 
    bool priority_recursion { false };                                                // true -- recursive call due to finding higher priority verb 
    int  error_count        { 0     };                                                // local counter -- number of errors so far accumulating this vexpr  
    
    M__(M_out(L"get_vexpr() called - end_by_paren=%d  end_by_verbname_bracket=%d  end_by_keyname_bracket=%d  end_expected=%d") % end_by_paren % end_by_verbname_bracket % end_by_keyname_bracket % end_expected;)


    // set vexpr gathering flags before starting main loop, if verb (and left parms) are already set in the incoming vexpr

    if (vexpr.has_verb)
    {
        M__(M_out(L"get_vexpr() -- priority recursion -- left_parms and verb already done when called");)
        left_parms_done    = true; 
        verb_done          = true; 
        priority_recursion = true; 
    }


    // if this is a nested vexpr, set starting token index to accound for already-discarded open paren or open angle bracket token

    if (end_by_paren || end_by_verbname_bracket || end_by_keyname_bracket)
        vexpr.token_ix1 = static_N::token_ix; 


    // =================================================================================== 
    // main loop to fetch tokens, until a complete vexpr (or error/END) can be passed back
    // ===================================================================================

    for(;;)
    {
        // look at next token -- end immediately (perhaps with empty vexpr), if error or unexpected end 

        token_C token {}; 
        auto prc = peek_token(pp, token, !end_expected);

        if ( (prc < 0) || (prc > 1) )
        {
            vexpr = a_vexpr_S {};                                                       // pass back empty vexpr 
            return prc;                                                                 // leave unexpected token unconsumed for next time
        }                                                                           
                                                                                    
        if (prc == 1)                                                                   // END seen?
        {                                                                           
            if (!end_expected)                                                          // END not expected?
            {
                M_out_emsg(L"get_vexpr(): unexpected END of input reached while processing nested expression"); 
                display_vexpr(vexpr, L"unexpected");
                vexpr = a_vexpr_S {};                                                   // pass back empty vexpr if end if not expected (i.e. nested vexpr) 
                return -1;                                                              // return error if end is not expected
            }                                                                      
            else                                                                        // END is expected
            {
                if (flushing)
                {
                     M_out_emsg1(L"get_vexpr(): END of input reached -- expression was flushed after first error"); 
                     display_vexpr(vexpr, L"flushed");
                     M_out_emsg2(L"vexpr is ignored");
                     vexpr = a_vexpr_S {};                                              // pass back empty vexpr 
                     return 2;                                                          // indicate bad vexpr 
                }
                else if ( (!verb_done) && (left_parms_done) )
                { 
                    //if ( (vexpr.lparms.kw_ct == 0) && (vexpr.lparms.value_ct <= 1) )  // no more than a single positional parm?
                    if (vexpr.lparms.kw_ct == 0)                                        // no keyword parms?
                    {                                                                 
                         return  (error_count > 0) ? 2 : 0;                             // just positional value(s) enclosed by paren -- return normally, or indicate bad vexpr  
                    }
                    else                                                                // not just positional values enclosed by parens -- has keyword parms -- incomplete vexpr -- error
                    {  
                        count_error(); 
                        M_out_emsg1(L"get_vexpr(): END of input reached,  but no verb was present following the left-side keyword parm(s)");  
                        display_vexpr(vexpr, L"no-verb");
                        M_out_emsg2(L"vexpr is ignored");
                        vexpr = a_vexpr_S {};                                           // pass back empty vexpr
                        return 2;                                                       // indicate bad vexpr
                    }                  
                }

                M__(M_out(L"get_vexpr() -- normal return due to END of input");)
                return  (error_count > 0) ? 2 : 1;                                      // return with normal END, or indicate bad vexpr
            }           
        }

        M__(token.display(L"get_vexpr()");) 
        M__(M_out(L"get_vexpr() -- token.utype2 = %d") % (int)(token.utype2);)


        //  ----------------------     
        //  check for ending token
        //  ----------------------

        if (end_by_paren || end_by_verbname_bracket || end_by_keyname_bracket)        // paren/bracket mode -- nested vexpr -- look only for close paren/bracket to end vlist (END should have been filtered out above)
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
                M__(M_out(L"get_(nested)_vexpr(): ending token seen");) 

                vexpr_add_token(vexpr, token);                                          // add to token list for this vexpr  

                if (!priority_recursion)                                                // leave ending token for upper-level get_vexpr() call, if priority recursion is active
                {
                    M__(M_out(L"get_(nested)_vexpr() -- discarding matching close paren/bracket");)
                    discard_token(pp);                                                  // get rid of closing parenthesis (if not priority recursion)
                }
   
                if (flushing)
                {
                     M_out_emsg1(L"get_vexpr(): expression was flushed after first error"); 
                     display_vexpr(vexpr, L"flushed");
                     M_out_emsg2(L"vexpr is ignored");
                     vexpr = a_vexpr_S {};                                              // pass back empty vexpr 
                     return 2;                                                          // indicate bad vexpr 
                }
                else if ( (!verb_done) && (left_parms_done) )
                { 
                    M__(M_out(L"get_vexpr() -- normal return due to close paren/bracket -- no verb present");)
                    return  (error_count > 0) ? 2 : 0;                                  // just a single/multiple value(s) enclosed by paren -- return normally, or indicate bad vexpr  

                    //if ( (vexpr.lparms.kw_ct == 0) && (vexpr.lparms.value_ct <= 1) )  // no more than a single positional parm?
                 //   if (vexpr.lparms.kw_ct == 0)                                      // no keyword parms?
                 //   {                                                                 
                 //        return  (error_count > 0) ? 2 : 0;                           // just a single value enclosed by paren -- return normally, or indicate bad vexpr  
                 //   }
                 //   else                                                              // not just positional values enclosed by parens -- has keyword parms -- incomplete vexpr -- error
                 //   {  
                 //       count_error(); 
                 //       M_out_emsg1(L"get_vexpr(): closing paren (or verbname/keyname bracket) at end of vexpr was reached, but no verb was present following the left-side keyword/positional parms"); 
                 //       display_vexpr(vexpr, L"no-verb");
                 //       M_out_emsg2(L"vexpr is ignored");
                  //      vexpr = a_vexpr_S {};                                         // pass back empty vexpr
                  //      return 2;                                                     // indicate bad vexpr
                  //  }                                                                
                }

                // note: completely empty vexpr is OK

                M__(M_out(L"get_vexpr() -- normal return due to close paren/bracket -- verb present");)
                return  (error_count > 0) ? 2 : 0;                                      // return normally, or indicate bad vexpr 
            }
             

            // check for unexpected (ending) semicolon

            if (token.utype2 == tok_u2_E::semicolon)                                    // unexpected semicolon -- overrides parens, marks end of vexpr -- consume semicolon
            {                                                                       
                vexpr_add_token(vexpr, token);                                          // add to token list for this vexpr  -- for error message only 
                                                                                    
                if (!priority_recursion)                                                // leave ending token for upper-level get_vexpr() call, if priority recursion is active
                    discard_token(pp);                                                  // consume the semicolon, since this must be top level vexpr within braces
            
                count_error(); 
                M_out_emsg1(L"get_vexpr(): semicolon reached before closing parenthesis (or verbname/keyname bracket) for expression was seen"); 
                display_vexpr(vexpr, L"incomplete");
                M_out_emsg2(L"expression is ignored"); 
                vexpr = a_vexpr_S {};                                                   // pass back empty vexpr
                return 2;                                                               // indicate bad vexpr
            }                                                                          
                                                                                       
                                                                                       
            // check for unexpected (ending) close brace                               
                                                                                       
            if (token.utype2 == tok_u2_E::close_brace)                                  // closing brace -- must be end of vexpr list, without final semicolon
            {                                                                          
                vexpr_add_token(vexpr, token);                                          // add to token list for this vexpr  -- for error message only
                                                                                        // don't consume closing brace          
                count_error(); 
                M_out_emsg1(L"get_vexpr(): closing brace reached before closing parenthesis (or verbname/keyname bracket) for expression was seen"); 
                display_vexpr(vexpr, L"incomplete");
                M_out_emsg2(L"expression is ignored");            
                vexpr = a_vexpr_S {};                                                   // pass back empty vexpr
                return 2;                                                               // indicate bad vexpr
            }


            // check for unexpected (ending) close paren, if this is a verb-type or keyword-type vexpr

            if ( (end_by_verbname_bracket || end_by_keyname_bracket) && (token.utype2 == tok_u2_E::close_paren) )  // closing paren while in verb sub-vexpr?-- must be end of whole (outer) vexpr, without missing close verbname/keyname bracket
            {                                               
                vexpr_add_token(vexpr, token);                                          // add to token list for this vexpr  -- for error message only
                                                                                        // don't consume closing paren, since this must be a sub-vexpr and caller needs the close paren to exit properly           
                count_error(); 
                M_out_emsg1(L"get_vexpr(): closing paren reached before closing verbname/keyname bracket for verb-type or keyword-type expression was seen"); 
                display_vexpr(vexpr, L"incomplete");
                M_out_emsg2(L"vexpr is ignored");            
                vexpr = a_vexpr_S {};                                                   // pass back empty vexpr
                return 2;                                                               // indicate bad vexpr
            }   
        }         // -----------------------------------------------------------------
        else      // not in paren or verbname/keyname bracket mode -- not nested vexpr 
                  // -----------------------------------------------------------------
        {
            if (token.utype2 == tok_u2_E::semicolon)                                    // semicolon -- consume semicolon at end
            {                                                                      
                vexpr_add_token(vexpr, token);                                          // add to token list for this vexpr 
                                                                                   
                if (!priority_recursion)                                                // leave ending token for upper-level get_vexpr() call, if priority recursion is active
                    discard_token(pp);                                                  // consume the semicolon, since this must be top level vexpr within braces
                                                                
                if (flushing)
                {
                     M_out_emsg1(L"get_vexpr(): expression was flushed after first error"); 
                     display_vexpr(vexpr, L"flushed");
                     M_out_emsg2(L"expression is ignored");
                     vexpr = a_vexpr_S {};                                              // pass back empty vexpr 
                     return 2;                                                          // indicate bad vexpr 
                }                                                                      
                else if ( (!verb_done) && (left_parms_done) )
                { 
                    //if ( (vexpr.lparms.kw_ct == 0) && (vexpr.lparms.value_ct <= 1) )  // no more than a single positional parm?
                    if (vexpr.lparms.kw_ct == 0)                                        // no keyword parms present?
                    {
                         return  (error_count > 0) ? 2 : 0;                             // just a positional value(s) enclosed by paren -- has keywords -- return normally, or indicate bad vexpr  
                    }
                    else                                                                // not just a positional value(s) -- incomplete vexpr -- error
                    {
                        count_error(); 
                        M_out_emsg1(L"get_vexpr(): semicolon reached,  but no verb was present following the left-side keyword/positional parms"); 
                        display_vexpr(vexpr, L"no-verb");
                        M_out_emsg2(L"vexpr is ignored");
                        vexpr = a_vexpr_S {};                                           // pass back empty vexpr
                        return 2;                                                       // indicate bad vexpr
                    }
                }

                M__(M_out(L"get_vexpr() -- normal return due to semicolon");)
                return  (error_count > 0) ? 2 : 0;                                      // return normally, or indicate bad vexpr
            }

            if (token.utype2 == tok_u2_E::close_brace)                                  // closing brace -- must be end of vexpr list, without final semicolon
            {
                // closing brace is not part of this vexpr -- don't add to vexpr, and don't consume the brace                               
             
                if (flushing)
                {
                     M_out_emsg1(L"get_vexpr(): expression was flushed after first error"); 
                     display_vexpr(vexpr, L"flushed");
                     M_out_emsg2(L"vexpr is ignored");
                     vexpr = a_vexpr_S {};                                              // pass back empty vexpr 
                     return 2;                                                          // indicate bad vexpr 
                }
                if ( (!verb_done) && (left_parms_done) )
                {
                    M__(M_out(L"get_vexpr() -- kw_ct = %d,    value_ct = %d") % vexpr.lparms.kw_ct % vexpr.lparms.value_ct;)

                  //if ( (vexpr.lparms.kw_ct == 0) && (vexpr.lparms.value_ct <= 1) )    // no more than a single positional parm?
                    if (vexpr.lparms.kw_ct == 0)                                        // no keyword values present? 
                    {
                         return  (error_count > 0) ? 2 : 0;                             // just positional values enclosed by paren -- return normally, or indicate bad vexpr  
                    }
                    else                                                                // not just posiotional value(s) -- has keywords -- incomplete vexpr -- error
                    {
                        count_error(); 
                        M_out_emsg1(L"get_vexpr(): closing brace reached,  but no verb was present following the left-side keyword/positional parms"); 
                        display_vexpr(vexpr, L"no-verb");
                        M_out_emsg2(L"vexpr is ignored");
                        vexpr = a_vexpr_S {};                                           // pass back empty vexpr
                        return 2;                                                       // indicate bad vexpr
                    }
                }

                M__(M_out(L"get_vexpr() -- normal return due to closing brace");)
                return  (error_count > 0) ? 2 : 0;                                      // return normally, or indicate bad vexpr
            }
        }


        // if error already seen, just continue loop to check for end -- flush tokens  

        if (error_count > 0)
        {
            flushing = true;                                                            // indicate flushing mode -- for final error message selection
            vexpr_add_token(vexpr, token);                                              // add to vexpr -- for error messages only  
            discard_token(pp);                                                          // flush the non-ending token
            continue;                                                                   // loop back and look for ending token
        }


        //  get left-side vlist (if any)  
        //  ----------------------------

        if (!left_parms_done && !verb_done)
        {
            left_parms_done = true; 

            M__(M_out(L"get_vexpr() -- calling get_vlist() for left-side vlist");)
            auto grc = get_vlist(frame, pp, vexpr, vexpr.lparms); 
            if (grc != 0)
                error_count++; 
            continue;                                                                   // loop back to look for ending token (flush), if error seen -- should not have consumed ending token 
        }


        // handle verb -- add in verb, if not done yet -- reshuffle vexprs that already have verbs based on verb priorities
        // ---------------------------------------------------------------------------------------------------------------

        if ( (token.utype2 == tok_u2_E::verbname) ||  (token.utype2 == tok_u2_E::open_verbname_bracket) )
        {
            // get priority of verb (if known at parse time) -- some verbs are undefined at this point -- use default priority (0) for these

            int       priority    {0}; 
            verbdef_S verbdef     { };

            if (token.utype2 == tok_u2_E::verbname)                                     // can get real priority only for simple verbdef vexprs
            {                                                   
                auto gvrc = get_verb(frame, token.str, verbdef); 

                if (gvrc == 0)
                    priority = verbdef.priority;                                        // can use real priority only if verb is pre-defined (built-in) or defined at the time of this parser invocation
            }


            // if no verb yet seen for this vexpr, just add the verb value to the vexpr

            if (!verb_done)
            {
                auto vrc = get_vexpr_verb(frame, pp, vexpr, token, verbdef);            // fill in vexpr verb-oriented fields for main vexpr 
                if (vrc != 0)                                                          
                    error_count++;                                                      // cause flush mode to start because of error
                                                                                       
                verb_done = true;                                                       // verb for current (main) token is now done -- this flag must be set after calling verb_get_vexpr, to avoid errroneous priority_recursion flag setting (in case of resursion)      
                continue;                                                               // loop back to get next token (or start flushing) 
            }
            else                             
            {
                // must have expression with multiple verbs -- prior verb or new verb needs to be pushed down based on verb priorities and associativity, as if (implicit) parens are present
                // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

                if (
                    (vexpr.priority > verbdef.priority)                                 // prior verb goes first, because of higher priority ?
                    ||
                    (
                     (vexpr.priority == verbdef.priority)                               // prior vexpr may go first, depending on assiciatiity and prefix/infix/postfix positioning
                     &&
                     (
                      (verbdef.left_associate)                                          // always left-associate ? (note: left_associate and right_associate flags should never both be on -- left_associate takes priority, though)
                      ||
                      (
                       (!verbdef.right_associate)                                       // don't always right-associate ?
                       &&
                       (vexpr.rparms.value_ct > 0)                                      // new verb appears to be infix (or postfix) ? -- left-associate by default
                      )
                     )
                    )                                                                   // prior verb goes first because of equal priorities and new verb's effective left-associativity ?
                   )
                {
                    // --------------------------------------------------------------------------------------------------------------------------------------------------------
                    //
                    // new verb has lower effective priority than verb already in in the vexpr: 
                    //
                    //  - save the results from prior verb's execution as the first (only) left-side positional parm for new verb
                    //
                    // -------------------------------------------------------------------------------------------------------------------------------------------------------- 

                    // copy everything from current vexpr into a new vexpr, clear out current vexpr, and attach new vexpr as the first (only) left positional parm of current vexpr 

                    value_S value    {      };                                           // temp value for set_vexpr_value()
                    a_vexpr_S left_vexpr { vexpr };                                      // copy everything to new a_vexpr_S for adding in   (including set up verb and vlists, etc.)
                                                                                   
                    vexpr = a_vexpr_S {};                                                // clear out existing vexpr                   
                    set_vexpr_value(value, left_vexpr, true);                            // move OK -- temp_vexpr no longer needed 
                    add_positional_value(vexpr.lparms, value);                           // add to start of left positional parms vector in cleared-out original vexpr 
                    vexpr.lparms.token_ix1 = left_vexpr.token_ix1;                       // vlist location is same is prior verb's vexpr location
                    vexpr.lparms.token_ix2 = left_vexpr.token_ix2;                       // vlist location is same is prior verb's vexpr location
                                                                                         // !!!!!!!!!!!!!! note: that new vexpr's location does not include left parms in this case
                    
                    // set up cleared-out existing vexpr with the new verb   

                    auto vrc = get_vexpr_verb(frame, pp, vexpr, token, verbdef);         // fill in vexpr verb-oriented fields for main vexpr 
                    if (vrc != 0)                                                 
                    {                                                             
                        error_count++;                                                   // cause flush mode to start because of error
                        continue;                                                        // start flushing 
                    }


                    // reset vexpr processing flags (for new verb) indicating left parms and verb are done, and right parms are not yet done

                    right_parms_done = false;                                            // any right parms were moved into left_vexpr, so updated original vexpr doesn't have any yet   
                    left_parms_done  = true;                                             // left parms for original vexpr are done (just the one nested vexpr)
                    verb_done        = true;                                             // 2nd verb becomes 1st verb for original vexpr 
                    continue;                                                            // loop back to get next token -- should be right parms for update original vexpr
                }
                else // new verb priority is higher (or prior vexpr is effectively right-associative, and both verbs have same priority)
                {
                    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    //
                    // new verb needs to be evaluated before the verb already in in the vexpr: 
                    //
                    //  - transfer existing right-side parms to new verb's left parms, and set prior verb's only right-side positional parm = results from new hi-priority verb's execution
                    //
                    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
                
                    value_S    value       {    };
                    a_vexpr_S   right_vexpr  {    };                                     // vexpr for new (hi-priority) verb 


                    // transfer prior verb's right-side parms (positional and kw) to left sideparms for new higher-priority verb

                    right_vexpr.lparms = vexpr.rparms;                                   // new verb's left parms are prior verb's right parms
                    vexpr.rparms       = vlist_S {};                                     // clear out right parms for prior verb (will be set below to results from new hi-priority verb);


                    // set up new right_vexpr with the new higher-priority verb   
                    
                    auto vrc = get_vexpr_verb(frame, pp, right_vexpr, token, verbdef);   // fill in vexpr verb-oriented fields for main vexpr 
                    if (vrc != 0)
                    {                                                                
                        error_count++;                                                   // cause flush mode to start because of error
                        continue;                                                        // loop back to start flushing
                    }


                    // do priority-recursive call to get_vexpr() 
                    //
                    //   -- pass thru ending flags from current call
                    //   -- partially-complete right_vexpr parm should cause nested get_vexpr() to go into "priority_recursive" mode, where ending token is left unconsumed for top-level get_vexpr() call 
                    //

                    M__(M_out(L"get_vexpr() -- doing priority-recursive call to get_vexpr()");)
                    auto grc = get_vexpr(frame, pp, right_vexpr, end_by_paren, end_by_verbname_bracket, end_by_keyname_bracket, end_expected);
                    if (grc != 0)
                        error_count++; 


                    // add completed higher-priority vexpr from priority-recursive get_vexpr() call to current vexpr's right-side positional parms -- will be the only right-side parm

                    set_vexpr_value(value, right_vexpr, true);                           // value points to completed higher-priority vexpr -- move is OK, since right_vexpr will soon go away
                    add_positional_value(vexpr.rparms, value);                           // first (only) right-size positional parm for prior verb = results from higher-priority verb
                    vexpr.rparms.token_ix1 = right_vexpr.token_ix1;                      // vlist location is same is higher_priority verb's vexpr location
                    vexpr.rparms.token_ix2 = right_vexpr.token_ix2;                      // vlist location is same is higher_priority verb's vexpr location
                                                                                       
                    right_parms_done = true;                                             // vexpr for prior verb is done -- should next see end token (left unconsumed from recursive call to get_vexpr())
                    continue;                                                            // loop back to get next token -- should be unconsumed ending token
                }                                                                   
            }
        }


        // process right-side parm list (if any)

        if (left_parms_done && verb_done && !right_parms_done)
        {
            right_parms_done = true; 
            auto grc = get_vlist(frame, pp, vexpr, vexpr.rparms); 
            if (grc != 0)
                error_count++; 
            continue;                                                                    // loop back to look for ending token if error seen -- should not consume ending token 
        }


        //   shouldn't get here -- some sort of extraneous token -- complain and continue with loop to flush until ending token seen
        //   token might be non-matching close_verbname_bracket or closing_keyname_backet

        error_count++; 
        count_error(); 
        M_out_emsg1(L"get_vexpr(): unexpected token « %s » seen -- expecting verb or option to begin new expression") % token.orig_str; 
        token.display(L"unexpected token");
        display_vexpr(vexpr, L"unexpected token");
        M_out_emsg2(L"bypassing this token -- flushing rest of expression");
        discard_token(pp);                                                               // get rid of unexpected token
        vexpr = a_vexpr_S {};                                                            // replace incomplete vexpr for pass-back with empty vexpr
        continue;

        //   ================ 
    }   //   end of main loop 
        //   ================

    return -2;      // should not get here
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

    vlist.val_empty           = false; 
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
    vlist.val_vexpr           = false; 
    vlist.val_slist           = false; 
    vlist.val_verbdef         = false; 
    vlist.val_typdef          = false;
    vlist.val_array           = false;
    vlist.val_structure       = false;
    vlist.val_ref             = false;
    
    vlist.val_mixed           = false; 

    vlist.kw_vexpr            = false; 
    vlist.kw_vlist            = false;
    vlist.kw_identifier       = false; 

                                // note: does not set kw_eval_done flag


    // scan positional parms, turning on flags

    for (const value_S& value : vlist.values)
    {
             if (value.ty == type_E::empty        ) vlist.val_empty       = true; 
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
        else if (value.ty == type_E::vexpr        ) vlist.val_vexpr       = true; 
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
                if (elem.second.ty == type_E::identifier)   vlist.kw_identifier = true; 
           else if (elem.second.ty == type_E::vexpr     )   vlist.kw_vexpr      = true; 
           else if (elem.second.ty == type_E::vlist     )   vlist.kw_vlist      = true;
        }      
    }
    else                                // no evaluated keywords -- do unevaluated ones (if any) instead 
    {
        for (const auto& elem : vlist.keywords)           // elem is keyword_S
        {
                if (elem.value.ty == type_E::identifier)   vlist.kw_identifier = true; 
           else if (elem.value.ty == type_E::vexpr     )   vlist.kw_vexpr      = true; 
           else if (elem.value.ty == type_E::vlist     )   vlist.kw_vlist      = true;
        } 
    }


    // set "mixed" flag if more than one type of positional parm is present
    // --------------------------------------------------------------------

    int type_ct {0}; 
  
    if (vlist.val_empty        ) type_ct++;
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
    if (vlist.val_vexpr        ) type_ct++;
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
////   get_vlist_value() -- helper function to add value of pre-peek()ed token to passed-in vlist, vexpr, etc.
////                     -- handles one value -- called in loop    
////
////
////    -- passed in (unconsumed) token should be: 
////    -- expected tokens : 
////       -- literal value       -- positional parm added
////       -- keyword             -- keyword parm added (keyword name is literal)
////       -- open dot bracket    -- keyword parm added (keyword name is expression)
////       -- open paren          -- nested vexpr     -- becomes positional parm (to be evaluated and replaced by results later)
////       -- open bracket        -- nested vlist     -- becomes positional parm
////       -- open brace          -- nested slist     -- becomes positional parm  
////  
////       -- anything else is internal error 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int get_vlist_value(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr, vlist_S& vlist, const token_C& token) try
{ 
    // literal or identifier token -- process positional value
    // -------------------------------------------------------

    if ( (token.utype2 == tok_u2_E::value) || (token.utype2 == tok_u2_E::identifier) )
    {
        value_S value {}; 

        vexpr_add_token(vexpr, token);                         // add to token list for passed-in vexpr 
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
        auto grc = get_keyword(frame, pp, vexpr, vlist);       // add keyword plus atom or nested vlist/vexpr/slist to keyword section of passed-in vlist
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
        a_vexpr_S  dummy_vexpr {};                             // dummy vexpr to hold open brace token (for debug messages) 

        vexpr_add_token(dummy_vexpr, token);                   // put open paren on brface stack for dummy vexpr 
        int64_t ix1 = static_N::token_ix;                      // capture starting token index for this value 
        discard_token(pp);                                     // get_nested_slist() does not want open brace -- will consume matching close brace 
        auto grc = get_nested_slist(frame, pp, new_slist);     // tokens for the nested slist don't go into current vexpr's token list  -- should consume ending brace
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
    

    // open parenthesis -- process nested vexpr 
    // ----------------------------------------

    if (token.utype2 == tok_u2_E::open_paren)
    {
        value_S    value      {};  
        a_vexpr_S  new_vexpr  {}; 

        vexpr_add_token(new_vexpr, token);                     // put open paren on token stack for new vexpr 
        int64_t ix1 = static_N::token_ix;                      // capture starting token index for this value 
        discard_token(pp);                                     // get_nested_vexpr() does not want open paren -- will consume matching close paren 
        auto grc = get_nested_vexpr(frame, pp, new_vexpr);     // tokens for the nested vexpr don't go into current vexpr's token list  -- should consume ending paren
        if (grc != 0)                                          // end immediately if unexpected end or error
        {
            vlist = vlist_S {};                                //  return empty vlist 
            return -1; 
        } 

        int64_t ix2 = static_N::token_ix;                      // capture ending token index for this value
        set_vexpr_value(value, new_vexpr, true);               // move OK -- new_vexpr no longer needed 
        value.token_ix1 = ix1;                                 // save away starting token index                                
        value.token_ix2 = ix2;                                 // save away ending token index
        add_positional_value(vlist, value);                    // add to positional parms vector in passed-in vlist 
        return 0;                                              // caller should loop back to look at next token          
    }
        

    // open bracket -- process nested vlist 
    // ------------------------------------

    if (token.utype2 == tok_u2_E::open_bracket)
    {
        value_S value      {};  
        vlist_S new_vlist  {}; 

        vexpr_add_token(vexpr, token);                           // put open bracket on token list for existing vexpr 
        int64_t ix1 = static_N::token_ix;                        // capture starting token index for this value 
        discard_token(pp);                                       // get rid of open bracket

        auto grc = get_nested_vlist(frame, pp, vexpr, new_vlist);// should consume ending bracket
        if (grc != 0)                                            // end immediately if unexpected end or error
        {
            vlist = vlist_S {};                                   // return empty vlist 
            return -1; 
        } 

        int64_t ix2 = static_N::token_ix;                         // capture ending token index for this value
        set_vlist_value(value, new_vlist, true);                  // move OK -- new_vlist no longer needed 
        value.token_ix1 = ix1;                                    // save away starting token index                                
        value.token_ix2 = ix2;                                    // save away ending token index
        add_positional_value(vlist, value);                       // add to positional parms vector in passed-in vlist 
        return 0;                                                 // caller should loop back to look at next token           
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
////       -- close paren            -- end vlist (not consumed) -- normal end of nested vexpr containing this vlist 
////       -- close_verbname_bracket -- end vlist (not consumed) -- normal end of verb sub-vexpr containing this lvist  
////       -- close_keyname_bracket  -- end vlist (not consumed) -- normal end of keyword name sub-vexpr containing this vlist  
////       -- close brace            -- end vlist (not consumed) -- normal end of nested slist containing this vlist  
////       -- literal value          -- positional parm                                                              -- added to vlist positional parms list 
////       -- keyword                -- keyword parm (keyword name is literal)                                       -- added to vlist keyword parms list 
////       -- open_keyname_bracket   -- keyword parm (keyword name is vexpr that must be evaluated later)            -- added to vlist keyword parms list 
////       -- open paren             -- nested vexpr -- becomes positional parm (to be evaluated later)              -- added to vlist positional parms list 
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

static int get_vlist(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr, vlist_S& vlist) try
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
        // look at next token -- end immediately (perhaps with empty vexpr), if error -- END is expected here 

        token_C token; 
        auto prc = peek_token(pp, token, false);

        if (token_ix1 == -1) token_ix1 = static_N::token_ix;                   // capture starting token index on first pass only  

        if ((prc < 0))
        {
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
            (token.utype2 == tok_u2_E::semicolon)                                // semicolon -- must be end of this vexpr
            ||                                                                  
            (token.utype2 == tok_u2_E::close_brace)                              // closing brace -- must be end of whole slist  
            ||                                                                  
            (token.utype2 == tok_u2_E::verbname)                                 // verbname 
            ||                                                                  
            (token.utype2 == tok_u2_E::open_verbname_bracket)                    // start of verb sub-vexpr (handled like verbname) 
            ||                                                                  
            (token.utype2 == tok_u2_E::close_paren)                              // close paren            - normal end for nested vexpr 
            ||                                                                  
            (token.utype2 == tok_u2_E::close_verbname_bracket)                   // close verbname bracket - normal end for verb sub-vexpr 
            ||                                                                  
            (token.utype2 == tok_u2_E::close_keyname_bracket)                    // close keyname bracket  - normal end for keyword name sub-vexpr
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
            auto rc = get_vlist_value(frame, pp, vexpr, vlist, token);             // pass through all parms
            if (rc != 0) 
            {
                vlist.token_ix1 = std::min(static_N::token_ix, token_ix1);         // capture starting token index for vlist
                vlist.token_ix2 = std::max(static_N::token_ix, token_ix1);         // capture ending token index for vlist
                return rc;                                                         // just return with error -- get_vlist_value() does all needed error processing 
            }
            continue;
        }
        

        // unexpected token seen -- pass back vexpr, as-is, after complaining  

        count_error();
        M_out_emsg1(L"get_vlist(): Unexpected token « %s » seen in un-nested vlist") % token.orig_str; 
        token.display(L"bad-token", true);
        vlist.token_ix1 = std::min(static_N::token_ix, token_ix1);             // capture starting token index for vlist -- for error message
        vlist.token_ix2 = std::max(static_N::token_ix, token_ix1);             // capture ending token index for vlist   -- for error message 
        display_vlist(vlist, L"incomplete vlist");
        M_out_emsg2(L"Passing back empty vlist in place of incomplete vlist");
        
        vlist = vlist_S {};                                                    // return empty vlist
        vlist.token_ix1 = std::min(static_N::token_ix, token_ix1);             // capture starting token index for vlist
        vlist.token_ix2 = std::max(static_N::token_ix, token_ix1);             // capture ending token index for vlist

        return -1;                                                             // don't discard unexpected token -- leave for next call in get_vexpr()                 
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
////   -- don't expect verb or open_verbname_bracket in vlist, except for nested vexprs enclosed in parens
////   
////   -- expect the following: 
////      -- close bracket        (end of vlist)                 -- consumed before return  
////      -- literal value        (positional parm)
////      -- keyword              (keyword with literal name)
////      -- open dot bracket     (keyword with name that needs evaluation)
////      -- open paren           (nested vexpr)                 -- becomes positional parm (to be evaluated and replaced by results later)
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

static int get_nested_vlist(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr, vlist_S& vlist) try
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
        // look at next token -- end immediately (perhaps with empty vexpr), if error or unexpected end 

        token_C token; 
        auto prc = peek_token(pp, token, true);  // END is not expected here

        if (prc != 0) 
        {
            vlist = vlist_S {};                  //  return empty vlist 
            vlist.token_ix1 = token_ix1;         //  set starting token index
            vlist.token_ix2 = static_N::token_ix;//  set ending token index
            return -1;                           //  leave unexpected token (END) unconsumed for next time
        }           

        M__(token.display(L"get_nested_vlist()");) 


        //  check for ending close bracket

        if (token.utype2 == tok_u2_E::close_bracket)
        {
            vexpr_add_token(vexpr, token);       // add to token list for this vexpr  
            discard_token(pp);                   // get rid of closing bracket 
            vlist.token_ix1 = token_ix1;         // set starting token index
            vlist.token_ix2 = static_N::token_ix;// set ending token index
            return 0;                            // pass back whatever has already been accumulated in output vlist
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
            auto rc = get_vlist_value(frame, pp, vexpr, vlist, token);     // pass through all parms
            if (rc != 0) 
            {
                vlist.token_ix1 = token_ix1;                               // set starting token index
                vlist.token_ix2 = static_N::token_ix;                      // set ending token index
                return rc;                                                 // just return with error -- get_vlist_value() does all needed error processing    
            }
            continue;
        }


        // unexpected token seen -- pass back vexpr, as-is, after complaining  (includes semicolons, close_brace, etc.

        count_error();
        M_out_emsg1(L"get_nested_vlist(): Unexpected token « %s » seen in nested vlist") % token.orig_str; 
        display_vexpr(vexpr, L"bad-token");
        token.display(L"bad-token", true);
        display_vlist(vlist, L"incomplete vlist");
        M_out_emsg2(L"Passing back empty vlist in place of incomplete vlist");

        vlist = vlist_S {};                                                // return empty vlist
        vlist.token_ix1 = token_ix1;                                       // set starting token index
        vlist.token_ix2 = static_N::token_ix;                              // set ending token index
                                                                         
        return -1;                                                         // don't discard unexpected token -- leave for next caller to consume  
                          // ================
    }                     // end of main loop 
                          // ================
  
    return -2;            // should not get here
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 

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
////   set_atom_token() -- put passed-in atom value (from token) into caller's (empty) value_S structure 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static void set_atom_token(value_S& value, const token_C& token, int64_t ix) try
{
    // make sure value is not already set

    if (value.ty != type_E::empty)
    {
        M_out_emsg1(L"set_atom(): passed-in value is not empty: ");
        display_value(value, L"non-empty value");
        token.display(); 
        M_out_emsgz();
        M_throw("set_atom(): passed-in value is not empty")
    }
    

    // save away token starting and ending index, if known                                                                                                                                                                

    value.token_ix1 = ix; 
    value.token_ix2 = ix;


    // set proper flags and value, based on caller's token

    if      (token.type == token_E::int8 )
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
    else if ( (token.type == token_E::misc_char) && (token.only_char == const_N::ch_empty_value) )    // empty value char
    {
        value.ty        = type_E::empty;
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

         if (kw_value.ty == type_E::identifier) vlist.kw_identifier = true; 
    else if (kw_value.ty == type_E::vexpr     ) vlist.kw_vexpr      = true; 
    else if (kw_value.ty == type_E::vlist     ) vlist.kw_vlist      = true; 

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
    else if (value.ty == type_E::vexpr      ) vlist.val_vexpr       = true;
    else if (value.ty == type_E::slist      ) vlist.val_slist       = true;
    else if (value.ty == type_E::verbdef    ) vlist.val_verbdef     = true;
    else if (value.ty == type_E::typdef     ) vlist.val_typdef      = true; 
    else if (value.ty == type_E::array      ) vlist.val_array       = true;
    else if (value.ty == type_E::structure  ) vlist.val_structure   = true;
    else if (value.ty == type_E::ref        ) vlist.val_ref         = true;
    else if (value.ty == type_E::empty      ) vlist.val_empty       = true;

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
    if (vlist.val_vexpr        ) type_ct++;
    if (vlist.val_slist        ) type_ct++; 
    if (vlist.val_verbdef      ) type_ct++; 
    if (vlist.val_typdef       ) type_ct++; 
    if (vlist.val_array        ) type_ct++;
    if (vlist.val_structure    ) type_ct++;
    if (vlist.val_ref          ) type_ct++;
    if (vlist.val_empty        ) type_ct++;
    
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

static int get_keyword(frame_S& frame, pre_parse_C& pp, a_vexpr_S& vexpr, vlist_S& vlist) try
{
    // get next token in stream, and check for end or errors -- should be the keyword itself -- end not expected

    token_C kw_token; 
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

        set_atom_token(kw_name, kw_token, kw_token_ix);             // save away simple keyword name token 


        // add this token to vexpr token list and discard it from the token stream

        vexpr_add_token(vexpr, kw_token);                           // add keyword token to vexpr token list 
        discard_token(pp);                                          // get rid of keyword token, so value can be peek()ed      
    }   

    // handle keyword name expression

    else if (kw_token.utype2 == tok_u2_E::open_keyname_bracket)
    {
        // handle keyword-name expression --  <. keyword-name vexpr .> 

        a_vexpr_S kwname_vexpr { };                                  // local a_vexpr_S (in autodata) for gathering up verb_vexpr

        vexpr_add_token(kwname_vexpr, kw_token);                     // put open dot bracket on token stack for kw_vexpr 
        int64_t ix1 = static_N::token_ix;                            // get starting token index for keyword name
        discard_token(pp);                                           // get rid of open_keyname_bracket token before calling get_nested_kwname_vexpr()

        auto grc = get_nested_kwname_vexpr(frame, pp, kwname_vexpr); // fill in local kwname_vexpr  -- will consume ending close_keyname_bracket 
        if (grc != 0)
            return grc;                                              // pass back error -- let caller handle error appropriately   

        int64_t ix2 = static_N::token_ix;                            // get ending token index
        set_vexpr_value(kw_name, kwname_vexpr, true);                // move OK -- kwname_vexpr no longer needed
        kw_name.token_ix1    = ix1;                                  // save away starting token index
        kw_name.token_ix2    = ix2;                                  // save away ending   token index
        kw_name.kw_token_ix  = kw_token_ix;                          // save away keyword value token starting index  ???????
    }   

    // error -- unexpected token seen instead of keyword name of start of keyword name expression

    else
    {
        count_error();
        M_out_emsg1(L"get_keyword(): Unexpected token « %s » seen -- expecting keyword name token or open_keyname_bracket token") % kw_token.orig_str; 
        kw_token.display();
        display_vexpr(vexpr, L"unexpected token");
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
    //   open paren                   -- value for this keyword is an vexpr (expression with verbs) 
    //   open bracket                 -- value for this keyword is a vlist (with 0-N values)
    //   open brace                   -- value for this keyword is an slist (with 0-N vexprs)
    //   END                          -- value for this keyword is empty (don't consume token)
    //   another keyword              -- value for this keyword is empty (don't consume token)
    //   another open_keyname_bracket -- value for this keyword is empty (don't consume token)
    //   close bracket                -- value for this keyword is empty (don't consume token)
    //   verb, option                 -- value for this keyword is empty (don't consume token)
    //   enything else                -- value for this keyword is empty (don't consume token)
    //
    //

    value_S kw_value {};                                              // new value_S for keyword value 


    // get next token from token stream  

    token_C value_token; 
    prc = peek_token(pp, value_token, false);
    if (prc < 0) return prc;                                         // exit if error only -- continue processing if END -- will be passed to get_atom_vlist(), which will create empty vlist  
    

    // handle keyword value = value or identifier

    if (
         (value_token.utype2 == tok_u2_E::value)                     // atomic value for this keyword 
         ||
         (value_token.utype2 == tok_u2_E::identifier)                // value for this keyword is an identifier name (treat as atomic value) 
       )
    {
        vexpr_add_token(vexpr, value_token);                         // add value token to vexpr tokens list 
        set_atom_token(kw_value, value_token, static_N::token_ix);   // value token is already available
        kw_value.kw_token_ix = kw_token_ix;                          // save away keyword token index
        discard_token(pp);                                           // get rid of value token, so next token can be peek()ed   
    }

    // handle keyword value = nested vexpr 

    else if (value_token.utype2 == tok_u2_E::open_paren)             // should be a (nested) vexpr as the keyword value 
    {                                                             
        a_vexpr_S new_vexpr {};                                     
                                                                  
        vexpr_add_token(new_vexpr, value_token);                     // put open paren on token stack for new vexpr 
        int64_t ix1 = static_N::token_ix;                            // get starting token index
        discard_token(pp);                                           // get_nested_vexpr() does not want open paren -- will consume matching close paren 
                                                                  
        auto grc = get_nested_vexpr(frame, pp, new_vexpr);           // tokens for the nested vexpr don't go into current vexpr's token list  -- should consume ending paren
        if (grc != 0)                                             
           return grc;                                               // pass back error -- let caller handle error appropriately 
                                                                  
        int64_t ix2 = static_N::token_ix;                            // get ending token index
        set_vexpr_value(kw_value, new_vexpr, true);                  // move OK -- new_vexpr no longer needed
        kw_value.token_ix1    = ix1;                                 // save away starting token index
        kw_value.token_ix2    = ix2;                                 // save away ending   token index
        kw_value.kw_token_ix  = kw_token_ix;                         // save away keyword value token starting index
    }

    // handle keyword value = nested slist 

    else if (value_token.utype2 == tok_u2_E::open_brace)             // should be a (nested) slist as the keyword value 
    {                                                               
        slist_S new_slist {};                                       
        a_vexpr_S  dummy_vexpr {};                                    
                                                                    
        vexpr_add_token(dummy_vexpr, value_token);                   // put open brace on dummy token stack for new vexpr 
        int64_t ix1 = static_N::token_ix;                            // get starting token index
        discard_token(pp);                                           // get_nested_slist() does not want open brace -- will consume matching close brace 
                                                                    
        auto grc = get_nested_slist(frame, pp, new_slist);           // tokens for the nested vexpr don't go into current vexpr's token list  -- should consume ending brace
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
                                                                    
        vexpr_add_token(vexpr, value_token);                         // put open bracket on token stack for new vexpr 
        int64_t ix1 = static_N::token_ix;                            // get starting token index
        discard_token(pp);                                           // get rid of open bracket before calling get_nested_vlist()

        auto grc = get_nested_vlist(frame, pp, vexpr, new_vlist);   // should consume ending bracket
        if (grc != 0)
           return grc;                                               // pass back error -- let caller handle error appropriately 
                                                                   
        int64_t ix2 = static_N::token_ix;                            // get ending token index
        set_vlist_value(kw_value, new_vlist, true);                  // move OK -- temp_vlist no longer needed
        kw_value.token_ix1   = ix1;                                  // save away starting token index
        kw_value.token_ix2   = ix2;                                  // save away ending   token index
        kw_value.kw_token_ix = kw_token_ix;                          // save away keyword  value token starting index
    }                                                              
                                                                   
    // handle empty (no) keyword value                             
                                                                   
    else                                                             // any other token
    {
        // no value for this keyword -- empty value_S will be added down below
        // stopping token is not consumed (or added to passed-in vexpr token-list) 
      
        kw_value.kw_token_ix = kw_token_ix;                          // save away keyword token index in empty value_S
    }   


    // ------------------------------------------------------------------------------------------------------
    // Add keyword and temporary value to passed-in vlist -- value will be copied into multimap via emplace()
    // ------------------------------------------------------------------------------------------------------

    add_keyword_value(vlist, kw_name, kw_value);                     // add keyword-name + value to keyword vector in vlist 

    return 0; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
////   verb_loc_str() -- return printable location of start and end of verb in vexpr in input source 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring verb_loc_str(const a_vexpr_S& vexpr) try
{
    return loc_str(vexpr.verb_value.token_ix1, vexpr.verb_value.token_ix2);      
}
M_endf

M_EX_IMPEXP                                                          // usable via of ex_interface.h
std::wstring verb_loc_str(const e_vexpr_S& eval_vexpr) try
{
    return loc_str(eval_vexpr.verb_token_ix1, eval_vexpr.verb_token_ix2);      
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   vexpr_loc_str() -- return printable location of start and end of vexpr in input source 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring vexpr_loc_str(const a_vexpr_S& vexpr) try
{
    return loc_str(vexpr.token_ix1, vexpr.token_ix2);   
}
M_endf

M_EX_IMPEXP                                                          // usable via of ex_interface.h
std::wstring vexpr_loc_str(const e_vexpr_S& eval_vexpr) try
{
    return loc_str(eval_vexpr.token_ix1, eval_vexpr.token_ix2);   
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
////   msg_loc(xx_vexpr_S          ) -- put out part of an error message with verb and vexpr location 
////   msg_loc(value_S, xx_vexpr_S ) -- put out part of an error message with value, verb, and vexpr location 
////   msg_loc(value_S             ) -- put out part of an error message with value location 
////   msg_loc(value_S, string     ) -- put out part of an error message with value location -- use passed-in text instead of "value"  
////   msg_loc(vlist_S             ) -- put out part of an error message with vlist location 
////
////   note: msgend_xxx() versions also close out the open multi-line error message
////
////   msg_kw_loc(value_S          ) -- put out part of an error message with keyword value location (can't overload, since input type is same as regular value)  
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



void msg_loc(const a_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);     
}
M_endf



M_EX_IMPEXP                                                          // usable outside of ex_dll, via ex_interface.h
void msg_loc(const e_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);        
}
M_endf



void msg_loc(const value_S& value, const a_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"      } % value_loc_str(value);
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);     
}
M_endf



void msg_loc(const value_S& value, const e_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"      } % value_loc_str(value);
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);     
}
M_endf



void msg_loc(const vlist_S& vlist, const a_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"      } % vlist_loc_str(vlist);
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);     
}
M_endf



void msg_loc(const vlist_S& vlist, const e_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"      } % vlist_loc_str(vlist);
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);     
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


void msgend_loc(const a_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);
    M_out_emsgz();
}
M_endf



M_EX_IMPEXP                                                          // usable outside of ex_dll, via ex_interface.h
void msgend_loc(const e_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);
    M_out_emsgz();
}
M_endf



void msgend_loc(const value_S& value, const a_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"      } % value_loc_str(value);
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);
    M_out_emsgz();
}
M_endf



void msgend_loc(const value_S& value, const e_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"value"      } % value_loc_str(value);
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr); 
    M_out_emsgz();
}
M_endf


void msgend_loc(const vlist_S& vlist, const a_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"      } % vlist_loc_str(vlist);
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);
    M_out_emsgz();
}
M_endf



void msgend_loc(const vlist_S& vlist, const e_vexpr_S& vexpr) try
{
    M_out(L"%20s location -- %s" ) % std::wstring { L"vlist"      } % vlist_loc_str(vlist);
    M_out(L"%20s location -- %s" ) % std::wstring { L"verb"       } %  verb_loc_str(vexpr);
    M_out(L"%20s location -- %s" ) % std::wstring { L"expression" } % vexpr_loc_str(vexpr);
    M_out_emsgz();
}
M_endf





//_________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""