// ex_preparse.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                                                 unsigned_integer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ===============
////     ex_preparse.cpp -- options pre-parsing functions
////     ===============
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


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   pre_parse_C() -- default constructor 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""

pre_parse_C::pre_parse_C() try
{
    M__(M_out(L"Pre_parse_C default constructor called");)


    // set up default imbed folder rom IMBED_PATH envar, if set
    // --------------------------------------------------------

    set_imbed_folder(); 


    // set up required configuration parms for the token_stream
    // --------------------------------------------------------

    m_token_stream.set_eol_is_whitespace(     true ); 
    m_token_stream.set_eof_is_whitespace(     true ); 
    m_token_stream.set_comment_is_whitespace( true ); 
    m_token_stream.set_combine_whitespace(    true ); 
    m_token_stream.set_combine_strings(       true ); 
    m_token_stream.set_skip_whitespace(       true );
  

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!   Don't forget to make corresponding changes to   pre_parse_C::set_leading_sigils()  
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    std::vector<char32_t> leading_sigils  { const_N::ch_pp_sigil_action };
  
    m_token_stream.set_leading_sigils(  leading_sigils);
  

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
////""

void pre_parse_C::close() try
{
    M__(M_out(L"Pre_parse_C::close() -- called");)

    m_token_stream.close();      // close out token_stream() 
    m_symbol_table.clear();      // get rid of all variables


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
////""

void pre_parse_C::reuse() try
{
    M__(M_out(L"Pre_parse_C::reuse() -- called");)

    // leave token stream open

    // leave pre_parse_C symbol_table intact for reuse

    // reset pre_parse_C state for reuse 

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
////   display_settings() -- displays selected fields in the pre_parse_C structure 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_parse_C::display_settings() try
{
    //M_out(L"---------------------------------------------------------------------------------------");
    M_out(L"pre_parse_C::m_imbed folder      = [%S]") % m_imbed_folder;
    M_out(L"pre_parse_C::m_error_ct          = %d"  ) % m_error_ct; 
    M_out(L"pre_parse_C::m_substitution_ct   = %d"  ) % m_substitution_ct; 
    M_out(L"pre_parse_C::m_max_substitutions = %d"  ) % m_max_substitutions; 
    M_out(L"pre_parse_C::m_skipping          = %S"  ) % M_bool_cstr(m_skipping);
    M_out(L"pre_parse_C::m_skipto label      = [%S]") % m_skipto_label;
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
////   display_all_vars() -- displays all variables currently in the symbol table 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_parse_C::display_all_vars() try
{
    int n = 0; 

    M_out(L"---------------------------------------------------------------------------------------");
    M_out(L"All %d variables in pre-parser symbol table:") % m_symbol_table.size();

    for (auto elem : m_symbol_table)
    {
        M_out(L"n= %-4d   ID= %-20S    Value= [%S]") % n % elem.first % shorten_str(elem.second.value, 100); 
        n++;
    }

    M_out(L"");
    display_settings(); 

    M_out(L"---------------------------------------------------------------------------------------");

    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   set_var() -- sets value of passed-in variable -- overwrites/defines whatever is there 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_parse_C::set_var(const std::wstring& ident, const std::wstring& value, bool is_const) try
{
    var_S var { };

    var.value    = value;
    var.is_const = is_const;
    auto ct      = m_symbol_table.count(ident);


    // see if identifier already exists, and is constant

    if (ct > 1)     // something wrong -- should never be more than 1 in table
        M_throw_v("pre_parse_C::set_var() --  %d entries in symbol table found for variable %s") % ct % out_ws(ident) ));
    
    if (ct > 0)
    {
       if (m_symbol_table.at(ident).is_const)
       {
           M_out_emsg(L"pre_parse-C::set_var() -- cannot alter constant %s with new value \"%s\"") % ident % value; 
           return;
       }

       m_symbol_table.erase(ident);                   // get rid of old value before adding new one
    }

    // add in new variable/constant to symbol table

    auto rc = m_symbol_table.emplace(ident, var);     // add as variable (non-constant) 

    if (!rc.second)
        M_throw_v("pre_parse_C::set_var() -- unexpected m_symbol_table.emplace(%s, var) failure") % out_ws(ident) ));
    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   unset_var() -- removes passed-in variable (if it is present in symbol table) 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void pre_parse_C::unset_var(const std::wstring& ident) try
{
    // see if identifier already exists, and is constant

    auto ct = m_symbol_table.count(ident);

    if (ct > 1)     // something wrong -- should never be more than 1 in table
        M_throw_v("pre_parse_C::unset_var() --  %d entries in symbol table found for variable %s") % ct % out_ws(ident) ));
    
    if (ct > 0)
    {
       if (m_symbol_table.at(ident).is_const)
       {
           M_out_emsg(L"pre_parse_C::unset_var() -- cannot remove constant %s") % ident; 
           return;
       }

       m_symbol_table.erase(ident);                   // get rid of old value 
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
////   get_var() -- gets value of passed-in variable (-1 if not in symbol table) 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int pre_parse_C::get_var(const std::wstring& ident, std::wstring& value) try
{
    auto ct = m_symbol_table.count(ident); 

    if (ct > 1)     // something wrong -- should never be more than 1 in table
        M_throw_v("pre_parse_C::get_var() --  %d entries in symbol table found for variable %s") % ct % out_ws(ident) )); 


    if (ct == 0)   // return with R/C = -1 and empty string, if variable is not there 
    {
        value.clear();
        return -1;
    }

    // 1 value found -- pass it back

    value = m_symbol_table.at(ident).value;
    return 0; 
}
M_endf




////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   is_set_var() -- returns true if variable/constant is currently set 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

bool pre_parse_C::is_set_var(const std::wstring& ident) try
{
    auto ct = m_symbol_table.count(ident); 

    if (ct > 1)            // something wrong -- should never be more than 1 in table
        M_throw_v("pre_parse_C::is_set_var() --  %d entries in symbol table found for variable %s") % ct % out_ws(ident) )); 
    
    if (ct == 0)          // true/false, depending on whether or not variable is set 
        return false;
    else
        return true;
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   import_dll() -- load DLL and call do_import() function in DLL 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int import_dll(const std::wstring& dll_name, const std::wstring& prefix) try
{
    HMODULE      hmodule   { nullptr };
    void        *fcn_p     { nullptr };    


    // load DLL -- load_dll() should put out any needed error messages

    auto ldrc = load_dll(dll_name, hmodule);  
    if (ldrc != 0) return ldrc; 


    // locate do_import() function in loaded DLL-- get_dll_function() should put out any needed error messages

    auto gfrc = get_dll_function(hmodule, std::wstring {L"do_import"}, fcn_p);
    if (gfrc != 0) return gfrc;


    return (*(do_import_T)fcn_p)(prefix); 
}
M_endf






////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////  peek_raw_token() -- peek next token from token_stream, and add user type field  
////  ==============      (consume invalid pre-parser tokens, with error message)
////
////
////      only errors, passback tokens, and valid pre-parse tokens get beyond this screening routine
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int pre_parse_C::peek_raw_token(token_C& t, size_t peek_level)
{
    M__(M_out(L"pre_parse_C::peek_raw_token(): called <------------------------");)

    token_C token; 


    // main loop to look for a token to be passed back -- invalid pre-parse tokens are consumed -- loop ends via function return only 

    for(;;)       
    {
        auto prc = m_token_stream.peek_token(token, peek_level);
        M__(token.display(L"pre_parse_C::peek_raw_token() -- calling token_stream_C::peek_token():");)
        if (prc != 0)
        {
               M_out_emsg(L"pre_parse_C::peek_raw_token() -- error R/C from peek token -- returning immediately");
               t = token;       // probably invalid
               m_error_ct++; 
               return -1;
        }
      

        // special handling for END token from token_stream

        if (token.type == token_E::end)
        {
             token.utype1 = tok_u1_E::end;     // pre-parser end token
             t = token; 
             M__(t.display(L"pre_parse_C::peek_raw_token() END:");)
             return 0;
        }

      
        // provisionally accept no-sigil identifiers and quoted strings -- may get passed back later, if they are of no interest to pre-parser
      
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


        // check for pre-parser action tokens ( leading sigil = ?, and trailing sigil = none, ?,  !,  or : ) -- restart loop to consume all invalid pre-parser tokens and loop back for next one
                   
        if (
             (token.has_leading_sigil)
             &&
             (token.leading_sigil == const_N::ch_pp_sigil_action)
           )
        {   
            // check for trailing sigil -- labels, variable substitution, or variable quoted string substitution 

            if (token.has_trailing_sigil)
            {
                     if (token.trailing_sigil == const_N::ch_pp_sigil_label        ) token.utype1 = tok_u1_E::label         ;
                else if (token.trailing_sigil == const_N::ch_pp_sigil_subst        ) token.utype1 = tok_u1_E::subst         ;
                else if (token.trailing_sigil == const_N::ch_pp_sigil_quote_subst  ) token.utype1 = tok_u1_E::subst_quote   ;
                else                                                                
                {   // leading ? sigil and unexpected trailing sigil -- bad token 
               
                    M_out_emsg1(L"pre_parse_C::peek_raw_token(): unexpected trailing sigil (%08X) present for pre-parser token:") % (uint32_t)(token.trailing_sigil); 
                    token.display( L"bad token"); 
                    M_out_emsg2(L"Invalid token will be discarded -- this may cause subsequent errors."); 
                    m_error_ct++;
                    m_token_stream.discard_token(peek_level);            // get rid of this invalid pre-parse token, so as to peek at the next one 
                    continue;                                            // loop back and get next token
                }
            }
            else                                                         // leading ? sigil, no trailing sigil -- must be pre-processor verb
            {
                 token.utype1 = tok_u1_E::verb;
            }


            // pass back pre-processor action token and return
            
            t = token; 
            M__(t.display(L"pre_parse_C::peek_raw_token():");)
            return 0; 
        }


        // other, non-pre-parser token (except for END) -- has trailing sigil only, or has non-pre-parser leading sigil -- may be invalid, error, etc.

        token.utype1 = tok_u1_E::passback; 
        t = token;
        M__(t.display(L"pre_parse_C::peek_raw_token():");)
        return 0;
   
    }  // end of main for(;;) loop


    M_throw("pre_parse_C::peek_raw_token(): main loop fell through")
    return 0;    // should not get here
}



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   peek_subst_token() -- peeks next token, handing any substititions 
////                   
////
////    - consumes any substitution tokens, leaves others queued up for caller to discard or re-peek()
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int pre_parse_C::peek_subst_token(token_C& t, size_t peek_level) try
{
    M__(M_out(L"pre_parse_C::peek_subst_token(): called <------------------------");)

    token_C token; 
    int     sub_ct {0};         // local substitution loop counter


    // main loop until text token that is not a variable substitution is seen
    // ----------------------------------------------------------------------

    for(;;)
    {
        // peek at next char in token stream -- immediate end if error -- leave error token unconsumed

        auto prc = peek_raw_token(token, peek_level);
        if (prc != 0)
        {
           t = token; 
           return -1;
        }
        

        // see if substititution-type token

        if ( (token.utype1 == tok_u1_E::subst) || (token.utype1 == tok_u1_E::subst_quote) )
        {
            m_token_stream.discard_token(peek_level);          // get rid of peek()ed token, since it's going to be processed here, or flagged as an error
        
            // end immediately, if apparent substitution loop
        
            if (++sub_ct > m_max_substitutions)
                M_throw_v("pre_parse_C::substitute_vars(): more than %d consecutive variable substitutions") % m_max_substitutions )); 
        
        
            // complain, if variable is not set
        
            std::wstring value { };
            auto grc = get_var(token.str, value);
            if (grc != 0)
            {
                M_out_emsg(L"pre_parse_C::substitute_vars(): variable \"%s\" not found") % token.orig_str;
                m_error_ct++; 
                continue;                    // loop back and do next token
            }
            else   // OK -- do the substitution
            {
                sub_ct++;                    // local consecutive substitutions counter
                m_substitution_ct++;         // total number of substitutions

                if (token.utype1 == tok_u1_E::subst_quote)    // quoted substitution ?
                {
                    M__(M_out(L"pre_parse_C::substitute_vars(): substituting: var=\"%s\"   value = \"«%s»\"") % token.orig_str % value;)
                    m_token_stream.attach_string( std::wstring(L"«") + value + std::wstring(L"»")
                                                , get_cached_id(token.source_id1) + std::wstring(L"[") + std::to_wstring(token.lineno1) + std::wstring(L":") + std::to_wstring(token.linepos1) + std::wstring(L"]-> var:«") + token.orig_str + std::wstring(L"»"));
                }
                else                                           // must be plain (unquoted) substitution    
                {
                    M__(M_out(L"pre_parse_C::substitute_vars(): substituting: var=\"%s\"   value = \"%s\"") % token.orig_str % value; )
                    m_token_stream.attach_string( value
                                                , get_cached_id(token.source_id1) + std::wstring(L"[") + std::to_wstring(token.lineno1) + std::wstring(L":") + std::to_wstring(token.linepos1) + std::wstring(L"]-> var:")  + token.orig_str);
                }
            }    
            continue;                       // loop back back and do next token 
        }
        else    // not variable substitution -- return to caller with unconsumed token (may be error, end, etc.)
        {
            t = token; 
            return 0;  
        }
    }

    t = token;
    return 0;      // should not get here 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   pre_parse_token() -- handle any pre-parser tokens -- end when 1st non-pre-parse token is seen (passed back in parm) 
////                        passed-back token is not consumed
////
////
////   rc = 0  -- normal token passed back
////   rc = -1 -- error
////   rc = 1  -- simulated token_E::end token from ?END processing is passed back
////
////
////
//// 
////      Sigils:       ?XXXX  verb                                                   const_N::ch_pp_sigil_action
////                    ?xXxx: label                                                  const_N::ch_pp_sigil_label
////                    ?xXxx?  substitute                                            const_N::ch_pp_sigil_subst  
////                    ?xXxx!  substitute with added quotes «»                       const_N::ch_pp_sigil_quote_subst
////
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

////////////////////////////////////////
// helper functions for pre_parse_token: 
////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  get_parm() -- R/C = 0 if parm-type found (consume token), R/C = -1 otherwise (don't consume token)  
//  ==========
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int pre_parse_C::get_parm(token_C& parm_token, const token_C& verb_token, const std::wstring& verb_text, int parm_num, size_t peek_level) try
{
    token_C token {};

    // peek() top token -- should not be another pre-parser token (verb, label, etc.)

     auto prc = peek_subst_token(token, peek_level);      // should have been left unconsumed (unless error R/C??) 
     if (prc != 0)
     {
         M_out_emsg1(L"pre_parse_C::get_parm(): error from token_stream_C::peek_token() prevented parm %d fetch for verb \"%s\":") % parm_num % verb_text; 
         verb_token.display(L"verb_token");     
         M_out_emsg2(L"");
         return -1;          // token, if any, is unconsumed for pass back 
     }


     // error if peek()ed token is error, end, or another pre-parser token

     if ( (token.utype1 != tok_u1_E::identifier) && (token.utype1 != tok_u1_E::string) )
     {
         M_out_emsg1(L"pre_parse_C::get_parm(): unexpected token seen instead of parm %d for verb \"%s\":") % parm_num % verb_text; 
         verb_token.display( L"verb_token"); 
         token.display(      L"parm_token"); 
         M_out_emsgz(); 
         m_error_ct++;
         return -1;         // non-parm token is unconsumed -- might be passed back, or be label, or another verb, etc. 
     }


     // token will be considered a parm for this verb -- may be of unacceptable type

     parm_token = token;                            // pass back the token
     m_token_stream.discard_token(peek_level);        // consume token before returning it
     return 0; 
}
M_endf




////////////////////////////////////////////////////////////////
//
// parameter fetching and checking MACROs
//
////////////////////////////////////////////////////////////////

#define M_parm1(t)                                          \
token_C  parm1 {};                                          \
auto     grc1 = get_parm(parm1, token, t, 1, peek_level);   \
if (grc1 != 0)                                              \
   continue;                                               



#define M_parm2(t)                                          \
token_C  parm2 {};                                          \
auto     grc2 = get_parm(parm2, token, t, 2, peek_level);   \
if (grc2 != 0)                                              \
   continue;  


#define M_parm3(t)                                          \
token_C  parm3 {};                                          \
auto     grc3 = get_parm(parm3, token, t, 3, peek_level);   \
if (grc3 != 0)                                              \
   continue;  


#define M_parm1_string(t)                                                                                         \
token_C  parm1 {};                                                                                                \
auto     grc1 = get_parm(parm1, token, t, 1, peek_level);                                                         \
if (grc1 != 0)                                                                                                    \
   continue;                                                                                                      \
if (parm1.utype1 != tok_u1_E::string)                                                                             \
{                                                                                                                 \
    m_error_ct++;                                                                                                 \
    M_out_emsg1(L"pre_parse_C::pre_parse_token(): 1st parameter for " t L" verb is not a quoted string literal:");\
    parm1.display(L"parm1_token");                                                                                \
    M_out_emsg2(t L" is ignored -- parm token is consumed");                                                      \
    continue;                                                                                                     \
}



                                                                                                                  
#define M_parm1_identifier(t)                                                                                     \
token_C  parm1 {};                                                                                                \
auto     grc1 = get_parm(parm1, token, t, 1, peek_level);                                                         \
if (grc1 != 0)                                                                                                    \
   continue;                                                                                                      \
if (parm1.utype1 != tok_u1_E::identifier)                                                                         \
{                                                                                                                 \
    m_error_ct++;                                                                                                 \
    M_out_emsg1(L"pre_parse_C::pre_parse_token(): 1st parameter for " t L" verb is not an identifier:");          \
    parm1.display(L"parm1_token");                                                                                \
    M_out_emsg2(t L" is ignored -- parm token is consumed");                                                      \
    continue;                                                                                                     \
}




#define M_parm2_string(t)                                                                                         \
token_C  parm2 {};                                                                                                \
auto     grc2 = get_parm(parm2, token, t, 2, peek_level);                                                         \
if (grc2 != 0)                                                                                                    \
   continue;                                                                                                      \
if (parm2.utype1 != tok_u1_E::string)                                                                             \
{                                                                                                                 \
    m_error_ct++;                                                                                                 \
    M_out_emsg1(L"pre_parse_C::pre_parse_token(): 2nd parameter for " t L" verb is not a quoted string literal:");\
    parm2.display(L"parm2_token");                                                                                \
    M_out_emsg2(t L" is ignored -- parm token is consumed");                                                      \
    continue;                                                                                                     \
}




#define M_parm2_identifier(t)                                                                                     \
token_C  parm2 {};                                                                                                \
auto     grc2 = get_parm(parm2, token, t, 2, peek_level);                                                         \
if (grc2 != 0)                                                                                                    \
   continue;                                                                                                      \
if (parm2.utype1 != tok_u1_E::identifier)                                                                         \
{                                                                                                                 \
    m_error_ct++;                                                                                                 \
    M_out_emsg1(L"pre_parse_C::pre_parse_token(): 2nd parameter for " t L" verb is not an identifier:");          \
    parm2.display(L"parm2_token");                                                                                \
    M_out_emsg2(t L" is ignored -- parm token is consumed");                                                      \
    continue;                                                                                                     \
}




#define M_parm3_string(t)                                                                                         \
token_C  parm3 {};                                                                                                \
auto     grc3 = get_parm(parm3, token, t, 3, peek_level);                                                         \
if (grc3 != 0)                                                                                                    \
   continue;                                                                                                      \
if (parm3.utype1!= tok_u1_E::string)                                                                              \
{                                                                                                                 \
    m_error_ct++;                                                                                                 \
    M_out_emsg1(L"pre_parse_C::pre_parse_token(): 3rd parameter for " t L" verb is not a quoted string literal:");\
    parm3.display(L"parm3_token");                                                                                \
    M_out_emsg2(t L" is ignored -- parm token is consumed");                                                      \
    continue;                                                                                                     \
}



#define M_parm3_identifier(t)                                                                                     \
token_C  parm3 {};                                                                                                \
auto     grc3 = get_parm(parm3, token, t, 3, peek_level);                                                         \
if (grc3 != 0)                                                                                                    \
   continue;                                                                                                      \
if (parm3.utype1 != tok_u1_E::identifier)                                                                         \
{                                                                                                                 \
    m_error_ct++;                                                                                                 \
    M_out_emsg1(L"pre_parse_C::pre_parse_token(): 3rd parameter for " t L" verb is not an identifier:");          \
    parm3.display(L"parm3_token");                                                                                \
    M_out_emsg2(t L" is ignored -- parm token is consumed");                                                      \
    continue;                                                                                                     \
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int pre_parse_C::pre_parse_token(token_C& t, size_t peek_level) try
{
    M__(M_out(L"pre_parse_token() called <--------------------------");)

    // =========    ------------------------------------------------- 
    // main loop -- ends when pre-parser finds something to pass back
    // =========    ------------------------------------------------- 

    for(;;)
    {
        // peek() at next pre-substituted,classified token 
        // -----------------------------------------------
      
        token_C token { };
        auto prc = peek_subst_token(token, peek_level);
        M__(M_out(L"pre_parse_token() peek_subst_token() returned -------------------------->");)
        M__(token.display(L"subst_token");)
        M__(M_out(L"<-------------------------------");)
      

        // =========================
        // peek_susbt_token() failed -- return with error from peek_token()
        // =========================
                        
        if (prc != 0)    
        {       
            M__(M_out(L"pre_parse_token(): error R/C from token_stream_C::peek_token()");)
            m_skipping = false;                           // error ends any active skipping mode 
            t          = token;                           // might be a filled-in token (or not) -- don't consume token
            return -1;                                    // return immediately -- lower routines should have incremented m_error_ct                          
        }


        // =========================
        // peek_subst_token() OK -- need to process token
        // =========================


        // special handling for END tokens, in skipping mode
        // -------------------------------------------------

        if (token.utype1 == tok_u1_E::end)      // END token? 
        {            
            if (m_skipping)
            {
                m_skipping = false;
                M_out_emsg(L"pre_parse_C::pre_parse_token():  end of input reached while skipping to label = \"%s\"") % m_skipto_label;                      
                m_error_ct++; 
            }
            t = token; 
            m_token_stream.discard_token(peek_level);   //   consume the END token before returning
            return 0; 
        }


        // pass back non-pre-parser tokens (if not skipping)
        // -------------------------------

        if (
             (token.utype1 == tok_u1_E::passback  )      // non-pre-parser token 
             ||
             (token.utype1 == tok_u1_E::string    )      // string which is not a ?VERB parm
             ||
             (token.utype1 == tok_u1_E::identifier)      // identifier which is not a ?VERB parm
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
                m_token_stream.discard_token(peek_level); // consume token before looping back
                continue;                               // loop back to process next token   
            }
        }


        // ================
        // label processing  -- end skip, if this is the SKIPTO target label
        // ================  

        if (token.utype1 == tok_u1_E::label)
        {
            if ( (m_skipping) && (token.str == m_skipto_label) )
            {
                M__(M_out(L"ending ?SKIPTO %s") % m_skipto_label;)                             
                m_skipping = false;
                set_suppress_echo(false);    /// re-enable whole-line echoed comments
                m_skipto_label = L"";             
            }
            m_token_stream.discard_token(peek_level);   // get rid of label token  
            continue;                                 // loop back to process next token
        }


        // handle any non-verb tokens that get here -- should not happen
        // ----------------------------------------

        if (token.utype1 != tok_u1_E::verb)
        {
             m_error_ct++; 
             M_out_emsg1(L"pre_parse_C::pre_parse_token(): unexpected token type seen -- internal error:");
             token.display(L"bad token");
             M_out_emsg2(L"pre-parser will end now");
             M_throw("pre_parse_C:pre_parse_token() received an unexpected token type");
             return -1;              // should not get here
        }


        ///////////////////////////////////////////////////////////////////////////////////////////
        //
        //                                  =========
        //                                  V E R B S
        //                                  =========
        //
        ///////////////////////////////////////////////////////////////////////////////////////////

        m_token_stream.discard_token(peek_level);   //   consume the valid or invalid verb token



        // ======================
        // ?ABORT verb processing  -- issue do_abort() = abort() with message 
        // ======================
  
        if (token.str == std::wstring(L"ABORT"))
        {
            if (!m_skipping)
                do_abort(); 
            continue;                     
        }   
     


        // ======================
        // ?CONST verb processing -- define constant "variable" -- must not already exist 
        // ======================

        else if (token.str == std::wstring(L"CONST"))
        {
            // get two parms for ?SET -- 1st should be identifier and 2nd ishould be quoted string (after any substitution) 

            M_parm1_identifier(L"?CONST")
            M_parm2_string(L"?CONST")


            // parms are OK -- set the constant (if not skipping)

            if (!m_skipping)
                if (is_set_var(parm1.str))   // make sure this identifier is not already in use
                {
                    m_error_ct++;                                                                                            
                    M_out_emsg1(L"pre_parse_C::pre_parse_token(): can't define ?CONST \"%s\" (with value \"%s\"), since identifier is already in use:") % parm1.str % parm2.str ; 
                    token.display();
                    parm1.display();    
                    parm2.display(); 
                    M_out_emsg2(L" ?CONST is ignored -- all ?CONST parm tokens are consumed"); 
                }
                else
                {
                    set_var(parm1.str, parm2.str, true);
                }
            continue;
        }



        // =================================
        // ?DISPLAY_ALL_VARS verb processing
        // =================================

        else if (token.str == std::wstring(L"DISPLAY_ALL_VARS"))
        {
            if (!m_skipping)
                display_all_vars();
            continue;                    
        }



        // =====================
        // ?END verb processing  -- close() pre_parse_C object and pass back token_E::end -- caller should not come back  
        // =====================
     
        else if (token.str == std::wstring(L"END"))
        {
            if (!m_skipping)
            {
                close();                                    // should free up everything
                token.type               = token_E::end;
                token.has_leading_sigil  = false;           // no complaints from token_C::display()
                token.has_trailing_sigil = false; 
                t                        = token;           // pass back token (consumed already) 
                return 1;                                   // special R/C to indicate tokn is already consumed -- passing back simulated END token
            }
            continue;                      
        }   
        
         

        // ========================
        // ?END_DEF verb processing  -- if identifier is def, close() pre_parse_C object and pass back toekn_E::end -- caller should not come back  
        // ========================
     
        else if (token.str == std::wstring(L"END_DEF"))
        {
            M_parm1_identifier(L"?END_DEF")
          
            if ( (!m_skipping) &&  is_set_var(parm1.str) )
            {
                close();                                    // should free up everything
                token.type               = token_E::end;
                token.has_leading_sigil  = false;           // no complaints from token_C::display()
                token.has_trailing_sigil = false; 
                t                        = token;           // pass back token (consumed already) 
                return 1;                                   // special R/C to indicate tokn is already consumed -- passing back simulated END token
            }
            continue;                      
        }   


        
        // ==========================
        // ?END_UNDEF verb processing  -- if identifier is not defined, close() pre_parse_C object and pass back toekn_E::end -- caller should not come back  
        // ==========================
     
        else if (token.str == std::wstring(L"END_UNDEF"))
        {
            M_parm1_identifier(L"?END_UNDEF")
          
            if ( (!m_skipping) &&  (!is_set_var(parm1.str)) )
            {
                close();                                    // should free up everything
                token.type               = token_E::end;
                token.has_leading_sigil  = false;           // no complaints from token_C::display()
                token.has_trailing_sigil = false; 
                t                        = token;           // pass back token (consumed already) 
                return 1;                                   // special R/C to indicate tokn is already consumed -- passing back simulated END token
            }
            continue;                      
        }   
  
          

        // ======================
        // ?ERROR verb processing  -- just increment error counter 
        // ======================
  
        else if (token.str == std::wstring(L"ERROR"))
        {
            if (!m_skipping)
                m_error_ct++;                    
            continue;                    
        }  
           


        // =====================
        // ?EXIT verb processing  -- issue do_exit() = exit(-1) with message 
        // =====================
     
        else if (token.str == std::wstring(L"EXIT"))
        {
            if (!m_skipping)
                do_exit(); 
            continue;                     
        }   
     


        // ======================
        // ?_EXIT verb processing  -- issue do__exit() = _exit(-1) with message 
        // ======================
     
        else if (token.str == std::wstring(L"_EXIT"))
        {
            if (!m_skipping)
                do__exit(); 
            continue;                     
        }  

             

        // ======================
        // ?IMBED verb processing 
        // ======================
 
        else if (token.str == std::wstring(L"IMBED"))
        {
            // get 1st (and only) parm for ?IMBED -- should be quoted string (after any substitution) 
 
            M_parm1_string(L"?IMBED")
 
 
            // process the ?IMBED, if not in skipping mode
 
            if (!m_skipping)
            {
                auto arc = m_token_stream.attach_file(m_imbed_folder + parm1.str);   
                if (arc != 0)
                {
                     M__(M_out(L"pre_parse_C::pre_parse_token() : returning immediately due to bad R/C from token_stream_C::attach_file()");) 
 
                     token.type = token_E::error;              // pass back error token and -1 R/C
                     t = token; 
                     return arc; 
                }
            }      
            continue;   
        }
         


        // =======================
        // ?IMPORT verb processing 
        // =======================
 
        else if (token.str == std::wstring(L"IMPORT"))
        {
            // get 1st (and only) parm for ?IMPORT -- should be quoted string (after any substitution) = DLL name = "xxxx" means xxxx.dll in current order of search
 
            M_parm1_string(L"?IMPORT")
 
 
            // process the ?IMPORT, if not in skipping mode
 
            if (!m_skipping)
            {
                auto irc = import_dll(parm1.str, L"");   
                if (irc != 0)
                {
                     M__(M_out(L"pre_parse_C::pre_parse_token() : returning immediately due to bad R/C from import_dll(\"%S\", \"%s\")") % parm1.str % L"";) 
 
                     token.type = token_E::error;              // pass back error token and -1 R/C
                     t = token; 
                     return irc; 
                }
            }      
            continue;   
        }




        // ===========================
        // ?QUICK_EXIT verb processing  -- issue do_quick_exit() = exit(-1) with message 
        // ===========================
     
        else if (token.str == std::wstring(L"QUICK_EXIT"))
        {
            if (!m_skipping)
                do_quick_exit(); 
            continue;                     
        }   

           
   
        // ====================
        // ?SAY verb processing  -- write out string, if not skipping 
        // ====================

        else if(token.str == std::wstring(L"SAY"))
        {
            // get 1st (and only) parm for ?SAY -- should be quoted string (after any substitution) 

            M_parm1_string(L"?SAY")

            if (!m_skipping)
                M_out(L"%s" ) % parm1.str;   // put out ?SAY text (if not skipping)                       
            continue;                    
        }



        // ===========================
        // ?SAY_ALWAYS verb processing -- write out string always, if skipping, or not
        // ===========================

         else if(token.str == std::wstring(L"SAY_ALWAYS"))
        {
            // get 1st (and only) parm for ?SAY_ALWAYS -- should be quoted string (after any substitution) 

            M_parm1_string(L"?SAY_ALWAYS")
                  
            M_out(L"%s" ) % parm1.str;               // put out ?SAY_SKIP always -- skipping or not                       
            continue;                    
        }



        // =========================
        // ?SAY_SKIP verb processing -- write out string, only if skipping
        // =========================

        else if(token.str == std::wstring(L"SAY_SKIP"))
        {
            // get 1st (and only) parm for ?SAY_SKIP -- should be quoted string (after any substitution) 

            M_parm1_string(L"?SAY_SKIP")

            if (m_skipping)
                M_out(L"%s" ) % parm1.str;         // put out ?SAY_SKIP text (only if skipping)                       
            continue;                    
        }



        // ====================
        // ?SET verb processing -- set variables -- defines variable, if needed (can't alter a constant)
        // ====================

        else if (token.str == std::wstring(L"SET"))
        {
            // get two parms for ?SET -- 1st should be identifier and 2nd ishould be quoted string (after any substitution) 

            M_parm1_identifier(L"?SET")
            M_parm2_string(L"?SET")


            // parms are OK -- set the variable (if not skipping)

            if (!m_skipping)
                set_var(parm1.str, parm2.str, false); 
            continue;
        }

 

        // =======================
        // ?SKIPTO verb processing 
        // =======================
        
        else if (token.str == std::wstring(L"SKIPTO"))
        {
            // get parm for ?SKIPTO -- should be identifier 

            M_parm1_identifier(L"?SKIPTO")


            // parms are OK -- start skipping 

            if (!m_skipping)
            {
                m_skipto_label = parm1.str; 
                set_suppress_echo();         /// shut off whole-line echoed comments
                m_skipping     = true; 
            }
            continue;
        }



        // ===========================
        // ?SKIPTO_DEF verb processing -- skip to label (parm1), if variable (parm2) is set
        // ===========================
        
        else if (token.str == std::wstring(L"SKIPTO_DEF"))
        {
            // get parms for ?SKIPTO_SET -- should be identifiers 

            M_parm1_identifier(L"?SKIPTO_DEF")
            M_parm2_identifier(L"?SKIPTO_DEF")

            // parms are OK -- start skipping 

            if (!m_skipping)
            {
                if ( is_set_var(parm2.str) )
                {
                    m_skipto_label = parm1.str;
                    set_suppress_echo();         /// shut off whole-line echoed comments
                    m_skipping     = true;
                }
            }
            continue;
        }
                    


        // =============================
        // ?SKIPTO_UNDEF verb processing -- skip to label (parm1), if variable (parm2) is not set
        // =============================
        
        else if (token.str == std::wstring(L"SKIPTO_UNDEF"))
        {
            // get parms for ?SKIPTO_SET -- should be identifiers 

            M_parm1_identifier(L"?SKIPTO_UNDEF")
            M_parm2_identifier(L"?SKIPTO_UNDEF")

            // parms are OK -- start skipping 

            if (!m_skipping)
            {
                if ( !is_set_var(parm2.str)  )
                {
                    m_skipto_label = parm1.str;
                    set_suppress_echo();         /// shut off whole-line echoed comments
                    m_skipping     = true;
                }
            }
            continue;
        }



        // ==========================
        // ?SKIPTO_EQ verb processing -- skip to label (parm1), if string parms 2 and 3 are equal
        // ==========================
        
        else if (token.str == std::wstring(L"SKIPTO_EQ"))
        {
            // get parms for ?SKIPTO_SET -- should be identifier and 2 stringss 

            M_parm1_identifier(L"?SKIPTO_EQ")
            M_parm2_string(L"?SKIPTO_EQ")
            M_parm3_string(L"?SKIPTO_EQ")

            // parms are OK -- start skipping 

            if (!m_skipping)
            {
                if (parm2.str == parm3.str)
                {
                    m_skipto_label = parm1.str;
                    set_suppress_echo();         /// shut off whole-line echoed comments
                    m_skipping     = true;
                }
            }
            continue;
        }
    


        // ==========================
        // ?SKIPTO_NE verb processing -- skip to label (parm1), if string parms 2 and 3 are not equal
        // ==========================
        
        else if (token.str == std::wstring(L"SKIPTO_NE"))
        {
            // get parms for ?SKIPTO_SET -- should be identifier and 2 stringss 

            M_parm1_identifier(L"?SKIPTO_NE")
            M_parm2_string(L"?SKIPTO_NE")
            M_parm3_string(L"?SKIPTO_NE")

            // parms are OK -- start skipping 

            if (!m_skipping)
            {
                if (parm2.str != parm3.str)
                {
                    m_skipto_label = parm1.str;
                    set_suppress_echo();         /// shut off whole-line echoed comments
                    m_skipping     = true;
                }
            }
            continue;
        }



        // ======================
        // ?UNDEF verb processing 
        // ======================
 
        else if (token.str == std::wstring(L"UNDEF"))
        {
            // get parm for ?UNDEF -- should be identifier 
 
            M_parm1_identifier(L"?UNDEF")                                                 
 
 
            // parms are OK -- unset the variable (if not skipping)
 
            if (!m_skipping)                      
                unset_var(parm1.str); 
            continue;
            
        }

            

        // ====================
        // ?VAR verb processing -- define variable -- must not already exist 
        // ====================

        else if (token.str == std::wstring(L"VAR"))
        {
            // get two parms for ?SET -- 1st should be identifier and 2nd ishould be quoted string (after any substitution) 

            M_parm1_identifier(L"?VAR")
            M_parm2_string(L"?VAR")


            // parms are OK -- set the variable (if not skipping)

            if (!m_skipping)
                if (is_set_var(parm1.str))   // make sure this identifier is not already in use
                {
                    m_error_ct++;                                                                                            
                    M_out_emsg1(L"pre_parse_C::pre_parse_token(): can't define ?VAR \"%s\" (with value \"%s\"), since identifier is already in use:") % parm1.str % parm2.str ; 
                    token.display();
                    parm1.display();    
                    parm2.display(); 
                    M_out_emsg2(L" ?VAR is ignored -- all ?VAR parm tokens are consumed"); 
                }
                else
                {
                    set_var(parm1.str, parm2.str, true);
                }
            continue;
        }



        // ========================
        // unknown verb -- complain -- don't consume any parm tokens, since number of parms is unknown
        // ========================

        else        // error -- not one of the known verbs
        {
            M_out_emsg1(L"pre_parse_token(): unknown pre-parse verb (\"%s\") seen:") % token.orig_str; 
            token.display();
            M_out_emsg2(L"unknown verb is not processed, any parms are not consumed"); 
            m_error_ct++; 
            continue;                    
        }
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
////   get_token() -- get next pre-parsed token 
////                   
////
////   rc = 0  -- normal token passed back
////   rc = -1 -- error
////   rc = 1  -- simulated token_E::end token from ?END processing is passed back (1st time only, not after putback) 
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
  
int pre_parse_C::get_token(token_C& token) try               // get (and consume) next token from stream  
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

    int rc = pre_parse_token(token);                         // get next token from the input token stream
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
  
int pre_parse_C::peek_token(token_C& token, size_t n) try              // get (and consume) next character from stream  
{
    M__(M_out(L"pre_parse_C::peek_token(%d) -- called -- M_token_stack.size() = %d") % n % m_token_stack.size();)


    // make sure 'n' is greater than 0 

    if (n <= 0)
    {
        M_out_emsg(L"pre_parse_C::peek_token(token, %u) called with bad char position parm (%)") % n % n;
        return -1; 
    }


    // pass back requested token, if it is on the existing putback stack -- oldest are at start [0], so n-th oldest has index [n-1]  

    size_t sz = m_token_stack.size();                            // number of put-back tokens on stack 

    if (sz >= n)
    {
        m_token_stack.at(n-1).peek_count ++;                     // increment peek counter
        token = m_token_stack.at(n-1);                           // get n-th oldest (composite) token  
           
        M__(M_out(L"pre_parse_C::peek_token() returning token from stack -------------------------------------->");)
        M__(token.display();)
        M__(M_out(L"<-------------------------------------------");)


        // leave token on stack

        if (token.type == token_E::error)
            return -1;                                         // if returning prior error, return with error R/C
        else
            return 0;                                          // else returned token is OK             
    }                                                          // do we need to pass back R/C = 1 here for simulated end????
       

    // Loop to add enough tokens to new end of putback stack, so that n-th oldest token is on the putback stack
    // --------------------------------------------------------------------------------------------------------
    
    int     rc  {0}; 
    token_C tok {}; 

    for (auto i = 0;  i < (n - sz); ++i)
    {
        int rci = pre_parse_token(tok);           // get next composite token     
        if (rci < 0) rc = rci;                    // remember any bad R/C   -- do we need to handle +1 R/C too ??????????????????????????????
        m_token_stack.push_back(tok);             // add newest composite token to back end of putback stack
    }

    if (m_token_stack.size() != n)
    {
        M_out_emsg(L"pre_parse_C::peek_token(token, %u) -- unexpected number of elements (%u) on putback stack") % n % m_token_stack.size();
        rc = -1;
    }


    //  Desired token should be at the back of the stack now -- return it but leave it on the stack 

    m_token_stack.back().peek_count ++;            // increment peek counter
    token = m_token_stack.back();                  // passing back last (newest) token on stack -- should be the right one, if R/C is 0  
    M__(M_out(L"pre_parse_C::peek_token() ---------------------------------------------------->");)
    M__(token.display();)
    M__(M_out(L"<------------------------------------");)
    

    M__(M_out(L"pre_parse_C::peek_token(%d) -- returning -- r/c = %d") % n % rc;)
    return rc; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   putback_token() -- putback last pre-parsed token onto token stack -- returned from get_token(), or earlier peek()
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

void pre_parse_C::putback_token(const token_C& token) try      // putback token to stream -- no need to be connected to a file
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

int pre_parse_C::discard_token(size_t n) try                      
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

void pre_parse_C::set_imbed_folder() try    
{
    if ( is_env_set(IMBED_PATH_ENVAR) )
        m_imbed_folder = get_env(IMBED_PATH_ENVAR);
    else
        m_imbed_folder = std::wstring { IMBED_PATH };  
}
M_endf



// form with passed-in string
// --------------------------

void pre_parse_C::set_imbed_folder(const std::wstring& i_f) try    
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
////   passthru functions to token_stream_C 
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""


int pre_parse_C::attach_file(const std::wstring& wn) try
{
    return m_token_stream.attach_file(m_imbed_folder + wn); 
}
M_endf

int pre_parse_C::attach_file(const std::string& n) try
{
    return m_token_stream.attach_file(n); 
}
M_endf


int pre_parse_C::attach_string(const std::wstring& ws, const std::wstring& wn) try
{
    return m_token_stream.attach_string(ws, wn);
}
M_endf


int pre_parse_C::attach_string(const std::string& s, const std::wstring& wn) try
{
    return m_token_stream.attach_string(s, wn);
}
M_endf


int pre_parse_C::attach_string(const std::string& s, const std::string& n) try
{
    return m_token_stream.attach_string(s, n);
}
M_endf

 

// query functions

bool     pre_parse_C::is_empty(    ) const try { return              m_token_stream.is_empty(    ) ; }  M_endf
size_t   pre_parse_C::putback_size() const try { return              m_token_stream.putback_size() ; }  M_endf
uint64_t pre_parse_C::errors(      ) const try { return m_error_ct + m_token_stream.errors(      ) ; }  M_endf


// subset of token_string_C configuration functions (other setings are required by pre-parse)

void pre_parse_C::set_quiet_mode(              bool     tf           ) try { return m_token_stream.set_quiet_mode(                           tf  );      }  M_endf
void pre_parse_C::set_suppress_echo(           bool     tf           ) try { return m_token_stream.set_suppress_echo(                        tf  );      }  M_endf
void pre_parse_C::set_combine_strings(         bool     tf           ) try { return m_token_stream.set_combine_strings(                      tf  );      }  M_endf
void pre_parse_C::set_always_attach_plus_minus(bool     tf           ) try { return m_token_stream.set_always_attach_plus_minus(             tf  );      }  M_endf
void pre_parse_C::set_never_attach_plus_minus( bool     tf           ) try { return m_token_stream.set_never_attach_plus_minus(              tf  );      }  M_endf
void pre_parse_C::set_allow_leading_op_sigils( bool     tf           ) try { return m_token_stream.set_allow_leading_op_sigils(              tf  );      }  M_endf
void pre_parse_C::set_allow_trailing_op_sigils(bool     tf           ) try { return m_token_stream.set_allow_trailing_op_sigils(             tf  );      }  M_endf
void pre_parse_C::set_allow_leading_id_sigils( bool     tf           ) try { return m_token_stream.set_allow_leading_id_sigils(              tf  );      }  M_endf
void pre_parse_C::set_allow_trailing_id_sigils(bool     tf           ) try { return m_token_stream.set_allow_trailing_id_sigils(             tf  );      }  M_endf
void pre_parse_C::set_allow_paren_sigils(      bool     tf           ) try { return m_token_stream.set_allow_paren_sigils(                   tf  );      }  M_endf

void pre_parse_C::set_digraph_char(            char32_t ch32         ) try { return m_token_stream.set_digraph_char(                         ch32);      }  M_endf
void pre_parse_C::set_string_escape_char1(     char32_t ch32         ) try { return m_token_stream.set_string_escape_char1(                  ch32);      }  M_endf
void pre_parse_C::set_string_escape_char2(     char32_t ch32         ) try { return m_token_stream.set_string_escape_char2(                  ch32);      }  M_endf
void pre_parse_C::set_line_continuation_char(  char32_t ch32         ) try { return m_token_stream.set_line_continuation_char(               ch32);      }  M_endf
void pre_parse_C::set_line_comment_char(       char32_t ch32         ) try { return m_token_stream.set_line_comment_char(                    ch32);      }  M_endf
void pre_parse_C::set_echoed_line_comment_char(char32_t ch32         ) try { return m_token_stream.set_echoed_line_comment_char(             ch32);      }  M_endf
void pre_parse_C::set_vanishing_separator_char(char32_t ch32         ) try { return m_token_stream.set_vanishing_separator_char(             ch32);      }  M_endf                                                                                   


///// ----------------------------------------------------------------------------------------------------------------                                                                                                                      
///// modified configuration functions (certain sigils are needed by pre-parse itself, in addition to caller's sigils)
///// ----------------------------------------------------------------------------------------------------------------

void pre_parse_C::set_leading_sigils(const std::vector<char32_t>& ls) try 
{
    std::vector<char32_t> leading_sigils = ls; 
    leading_sigils.push_back(const_N::ch_pp_sigil_action);               // add on required leading sigil for pre-parser actions -- '?'

    return m_token_stream.set_leading_sigils(leading_sigils); 
}
M_endf


void pre_parse_C::set_leading_ident_sigils(const std::vector<char32_t>& ls) try 
{
    std::vector<char32_t> leading_sigils = ls; 
    leading_sigils.push_back(const_N::ch_pp_sigil_action);               // add on required leading sigil for pre-parser actions -- '?'
                                                                                                                          
    if (leading_sigils.size() > 0)
        m_token_stream.set_allow_leading_id_sigils(true );               // need to allow leading identifier sigils
    else
        m_token_stream.set_allow_leading_id_sigils(false);               // don't allow leading identifier sigils 


    return m_token_stream.set_leading_ident_sigils(leading_sigils); 
}
M_endf


void pre_parse_C::set_leading_oper_sigils(const std::vector<char32_t>& leading_sigils) try 
{
    if (leading_sigils.size() > 0)
        m_token_stream.set_allow_leading_op_sigils(true );               // need to allow leading operator sigils
    else
        m_token_stream.set_allow_leading_op_sigils(false);               // don't allow leading operator sigils 


    return m_token_stream.set_leading_oper_sigils(leading_sigils); 
}
M_endf



void pre_parse_C::set_trailing_ident_sigils(const std::vector<char32_t>& ts) try 
{
    std::vector<char32_t> trailing_sigils = ts; 

    trailing_sigils.push_back(const_N::ch_pp_sigil_subst                );    // add on required traliing sigil for pre-parser  -- '?' = pre-parser variable substitution        trailing sigil
    trailing_sigils.push_back(const_N::ch_pp_sigil_quote_subst          );    // add on required traliing sigil for pre-parser  -- '!' = pre-parser string variable substitution trailing sigil
    trailing_sigils.push_back(const_N::ch_pp_sigil_label                );    // add on required traliing sigil for pre-parser  -- ':' = pre-parser label                        trailing sigil  
    
    if (trailing_sigils.size() > 0)
        m_token_stream.set_allow_trailing_id_sigils(true );                   // need to allow trailing identifier sigils
    else
        m_token_stream.set_allow_trailing_id_sigils(false);                   // don't allow trailing identifier sigils 

    return m_token_stream.set_trailing_ident_sigils(trailing_sigils); 
}
M_endf



void pre_parse_C::set_trailing_oper_sigils(const std::vector<char32_t>& trailing_sigils) try 
{
    if (trailing_sigils.size() > 0)
        m_token_stream.set_allow_trailing_op_sigils(true );                   // need to allow trailing operator sigils
    else
        m_token_stream.set_allow_trailing_op_sigils(false);                   // don't allow trailing operator sigils 


    return m_token_stream.set_trailing_oper_sigils(trailing_sigils); 
}
M_endf



void pre_parse_C::set_paren_sigils(const std::vector<char32_t>& sigils) try 
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