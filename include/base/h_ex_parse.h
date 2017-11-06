
// h_ex_parse.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ============ 
////            h_ex_parse.h -- declarations related to option file tokenization, parsing, etc. (ex_eval.cpp,  ex_parse.cpp  and  ex_preprocess.cpp)  
////            ============
////
////
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "h_ex_types.h"         
#include "h_ex_lex.h"



// path for imbedded files
// -----------------------

#define IMBED_PATH_ENVAR       L"VERBEXX_IMBED_PATH"            // IMBED_PATH environment variable name   
#define IMBED_PATH             L"..\\imbed\\"                   // default path for imbedded files, in case IMBED_PATH envar is not set  



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                                                                            
////
////
////            =============
////            pre processor (ex_preprocess.cpp)  
////            =============
////
////
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ----------------
//    var_S  structure    preprocess)
//    ----------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct var_S
{                            
    std::wstring     value                     ;      // value assigned to variable/constant  
    bool             is_const        { false } ;      // true : this identifier is constant
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    --------------
//    tok_u1_E  enum 
//    --------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class tok_u1_E { none              // not classified yet 
                    , end               // END token from token_stream 
                    , passback          // not recognized -- pass back up to caller
                    , evaluate          // text to be executed/evaluated by preprocess
                    , identifier        // name of identifier -- usually lvalue (no leading sigil) -- may get passed back 
                    , string            // may get passed back
                    };




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    --------------------
//    pre_process_C  class 
//    --------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// pending_attach_S  structure -- these are the elements on the m_pending_attach_stack   
// -----------------------------------------------------------------------------------

struct pending_attach_S
{   
    bool                 is_file     { false };        // true, if this is a pending attach of a file -- false, if this is a pending attach of a string 
    std::wstring         text        {       };        // string text  
    std::wstring         name        {       };        // filename or string name for error messages  
};

                    
// pre_process_C class
// ------------------------------------------------------------------------------------
                                                                                       
class pre_process_C
{
public:   
                      pre_process_C()                                                ;        // custom default constructor
                      pre_process_C(const pre_process_C&)                   = delete ;        // no copy constructor
                      pre_process_C(pre_process_C&&)                        = delete ;        // no move constructor
                     ~pre_process_C()                                       = default;        // compiler-generated destructor
                      pre_process_C&     operator=(const pre_process_C&)    = delete ;        // no copy assignment
                      pre_process_C&     operator=(pre_process_C&&)         = delete ;        // no move assignment


    // passthru or semi-passthru functions to imbedded token_string_C

    void              inherit_state(                                                                          );  // initialize skipping state from *m_upward_sp pre_process_C 
    void              close(                                                                                  );  // free up everything and reset states   
    void              reuse(              bool = false                                                        );  // reset leftover states to  can be reused   
    int               attach_file(        const std::wstring&                                                 );  // add new file to this token stream -- placed on top of filestack -- wide string filename version
    void              add_pending_attach( const std::wstring&                                                 );  // add new filename to pending attach stack,for later file_attach() processing
    void              add_pending_attach( const std::wstring&, const std::wstring&                            );  // add new string to pending attach stack,for later file_attach() processing

    int               attach_string(      const std::wstring&, const std::wstring&                            );  // add new string to this token stream -- placed on top of filestack -- wide string version
    int               attach_string(      const std::string& , const std::wstring&                            );  // add new string to this token stream -- placed on top of filestack -- wide string name version
    int               attach_string(      const std::string& , const std::string&                             );  // add new string to this token stream -- placed on top of filestack -- "plain" string name version
                                                                                                              
    int               get_token(          token_C&                                                            );  // get (and consume) next composite token from stream  
    int               peek_token(         token_C&, size_t = 1ULL                                             );  // return n-th composite token from stream/stack but do not consume it -- leave for next time  
    void              putback_token(const token_C&                                                            );  // putback composite token to stream 
    int               discard_token(                size_t = 1ULL                                             );  // Discard n-th token on putback queue (if any) R/C is -1 if putback q is not large enough 
                                                                                                             
    bool              is_empty()     const;                                                                       // return true, if put-back token queue is empty
    size_t            putback_size() const;                                                                       // return number of put-back tokens currently queued up 
                                                                                               
    void              set_quiet_mode(                      bool     = true                                    );  // set/reset quiet mode to suppress/show error messages 
    void              set_suppress_echo(                   bool     = true                                    );  // set/reset line comment echo suppression
    void              set_combine_strings(                 bool     = true                                    );  // set/reset combine adjacent strings flag
    void              set_always_attach_plus_minus(        bool     = true                                    );  // set/reset always_attach_plus_minus flag
    void              set_never_attach_plus_minus(         bool     = true                                    );  // set/reset never _attach_plus_minus flag
    void              set_allow_leading_op_sigils(         bool     = true                                    );  // set/reset allow_leading_op_sigils  flag
    void              set_allow_trailing_op_sigils(        bool     = true                                    );  // set/reset allow_trailing_op_sigils flag
    void              set_allow_leading_id_sigils(         bool     = true                                    );  // set/reset allow_leading_op_sigils  flag
    void              set_allow_trailing_id_sigils(        bool     = true                                    );  // set/reset allow_trailing_op_sigils flag
    void              set_allow_paren_sigils(              bool     = true                                    );  // set/reset allow_paren_sigils       flag
    void              set_allow_attached_paren(            bool     = true                                    );  // set/reset allow_attached_paren     flag
                         

    //                functions to set configurable characters 

    void              set_trigraph_char(                   char32_t = const_N::ch_trigraph                    );  // set trigraph char 
    void              set_vanishing_separator_char(        char32_t = const_N::ch_vanishing_sep               );  // set vanishing separator char 
    void              set_line_continuation_char(          char32_t = const_N::ch_line_continuation           );  // set line continuation char 
    void              set_always_sign_char(                char32_t = const_N::ch_always_sign                 );  // set always-sign char 
    
    void              set_type1_string_start_char(         char32_t = const_N::ch_type1_string_start          );  // open  quotes for type1 string  
    void              set_type2_string_start_char(         char32_t = const_N::ch_type2_string_start          );  // open  quotes for type2 string  
    void              set_type1_string_end_char(           char32_t = const_N::ch_type1_string_end            );  // close quotes for type1 string  
    void              set_type2_string_end_char(           char32_t = const_N::ch_type2_string_end            );  // close quotes for type2 string  
    void              set_type1_string_escape_char(        char32_t = const_N::ch_type1_string_escape         );  // set 1st type of string escape char    
    void              set_type2_string_escape_char(        char32_t = const_N::ch_type2_string_escape         );  // set 2nd type of string escape char 
    void              set_raw_string_prefix_char(          char32_t = const_N::ch_raw_string_prefix           );  // prefix char ( "R" ) for raw strings  
    void              set_multiline_string_prefix_char(    char32_t = const_N::ch_multiline_string_prefix     );  // prefix char ( "M" ) for multiline strings 
    void              set_word_string_start_char(          char32_t = const_N::ch_word_string_start           );  // open  quotes for type2 string  

    void              set_unechoed_line_comment_char(      char32_t = const_N::ch_unechoed_line_comment       );  // set unechoed line comment char       
    void              set_echoed_line_comment_char(        char32_t = const_N::ch_echoed_line_comment         );  // set echoed   line comment char     

    void              set_comment_1st_char(                char32_t = const_N::ch_comment_1st                 );  // set comment_1st                char 
    void              set_unechoed_line_comment_2nd_char(  char32_t = const_N::ch_unechoed_line_comment_2nd   );  // set unechoed line_comment_2nd  char 
    void              set_echoed_line_comment_2nd_char(    char32_t = const_N::ch_echoed_line_comment_2nd     );  // set echoed_line_comment_2nd    char 
    void              set_suppress_eol_comment_2nd_char(   char32_t = const_N::ch_suppress_eol_comment_2nd    );  // set suppress_eol_comment_2nd   char 
    void              set_eof_comment_2nd_char(            char32_t = const_N::ch_eof_comment_2nd             );  // set eof_comment_2nd            char 
    void              set_retained_line_comment_2nd_char(  char32_t = const_N::ch_retained_line_comment_2nd   );  // set retained_line_comment_2nd  char 
                   
    void              set_block_comment_2nd_char(          char32_t = const_N::ch_block_comment_2nd           );  // set block_comment_2nd          char 
    void              set_block_comment_3rd_char(          char32_t = const_N::ch_block_comment_3rd           );  // set block_comment_3rd          char 
    void              set_block_comment_4th_char(          char32_t = const_N::ch_block_comment_4th           );  // set block_comment_4th          char 
    void              set_nest_comment_2nd_char(           char32_t = const_N::ch_nest_comment_2nd            );  // set nest_comment_2nd           char 
    void              set_nest_comment_3rd_char(           char32_t = const_N::ch_nest_comment_3rd            );  // set nest_comment_3rd           char 
    void              set_nest_comment_4th_char(           char32_t = const_N::ch_nest_comment_4th            );  // set nest_comment_4th           char 
    void              set_retained_block_comment_2nd_char( char32_t = const_N::ch_retained_block_comment_2nd  );  // set retained_block_comment_2nd char 
    void              set_retained_block_comment_3rd_char( char32_t = const_N::ch_retained_block_comment_3rd  );  // set retained_block_comment_3rd char 
    void              set_retained_block_comment_4th_char( char32_t = const_N::ch_retained_block_comment_4th  );  // set retained_block_comment_4th char 

    void              set_leading_sigils(                  const std::vector<char32_t>&                       );   // set vector with sigils that can be attached to front of identifiers and operators and parens    
    void              set_leading_ident_sigils(            const std::vector<char32_t>&                       );   // set vector with sigils that can be attached to front of identifiers    
    void              set_trailing_ident_sigils(           const std::vector<char32_t>&                       );   // set vector with sigils that can be attached to back  of identifiers  
    void              set_leading_oper_sigils(             const std::vector<char32_t>&                       );   // set vector with sigils that can be attached to front of identifiers    
    void              set_trailing_oper_sigils(            const std::vector<char32_t>&                       );   // set vector with sigils that can be attached to back  of identifiers 
    void              set_paren_sigils(                    const std::vector<char32_t>&                       );   // set vector with sigils that can be attached to fron/back of parenthesis 

                             
    // non-passthru external functions

    void              set_skip(                    const std::wstring&                                        );   // set skipping mode with label 
  //void              set_skipall(                                                                            );   // set skipall mode (to end)   
    void              set_imbed_folder(            const std::wstring&                                        );   // set base folder for imbed files -- form with passed-in string
    void              set_imbed_folder(                                                                       );   // set default base folder for imbed files -- use envar if set
    void              display_settings(                                                                       );   // display preprocess settings


    // parsing/pre-parsing configuration flags (public)

    bool                                   m_allow_verbless_kws                       { true       };              // true -- allows keywords to be present in a verbless expression
    bool                                   m_allow_verbless_multi_parms               { true       };              // true -- allows more than one positional parm to be present in a verbless expression


    // preprocess stack-oriented public data items

    std::shared_ptr<pre_process_C>         m_upward_sp                                {             };             // pointer to next (upward) pre_process_C on preprocess stack
    frame_S                               *m_frame_p                                  {             };             // pointer to stack frame for this pre_process_C object (may be static pp frame) 
    uint64_t                               m_serial                                   {             };             // serial number for this PP               
    uint64_t                               m_pp_depth                                 {             };             // queue depth for this PP
    bool                                   m_remove_sf                                { false       };             // true -- m_frame_p needs to be removed from stack, when this PP is freed up


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

    token_stream_C                         m_token_stream                             {            };              // stream of incoming tokens    
    std::deque<token_C>                    m_token_stack                              {            };              // stack of tokens (that have been put back or peek()ed) 
    std::stack<pending_attach_S>           m_pending_attach_stack                     {            };              // stack of filenames waiting to be attached (when pre-processor text section ends)    
    bool                                   m_skipping                                 { false      };              // true -- pre-processor is in skipping mode 
    bool                                   m_inherited_skipping                       { false      };              // true -- pre-processor inherited skipping mode from upward pre_process_C  
    std::wstring                           m_skipto_label                             {            };              // label that preprocess is skipping to         
    std::wstring                           m_imbed_folder                             { IMBED_PATH };              // folder for imbedding files                               
                                                                                                           
                                                                                                                  
    // internal (private) functions                                                                               
                                                                                                                  
    int      peek_raw_token(   token_C&,   size_t = 1ULL                                            );             // get token trom token stream and do initial processing
    int      _token(           token_C&,   size_t = 1ULL                                            );             // process any preprocess values in token string
    int      get_parm(         token_C&, const token_C&, const std::wstring&, int, size_t = 1       );             // see if next token is stream is considered a parm for preceeding verb
};                                    






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////
////            ==================
////            parser/AST builder (ex_parse.cpp )  
////            ==================
////
////
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    --------------
//    tok_u2_E  enum 
//    --------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class tok_u2_E { none                    // not classified yet 
                    , end                     // END token or R/C from preprocess 
                    , verbname                // verbname                               (@identifier -or- ++ -- <<<<, etc.)
                    , keyname                 // keyword parm for verb                  (identifier:)
                    , value                   // quoted string, integer, floating pt, unit -- see main token type for details
                    , identifier              // plain identifier (variable, etc.)      (identifier or $identifier)
                    , label                   // label                                  (:identifier, etc.)
                    , open_paren              // main type passed-through
                    , close_paren             // main type passed-through
                    , open_bracket            // main type passed-through
                    , close_bracket           // main type passed-through
                    , open_brace              // main type passed-through
                    , close_brace             // main type passed-through
                    , open_verbname_bracket   // main   @ + open_paren type mapped to  open_verbname_bracket 
                    , close_verbname_bracket  // main close_paren + @  type mapped to close_verbname_bracket
                    , open_keyname_bracket    // main   : + open_paren type mapped to  open_keyname_bracket
                    , close_keyname_bracket   // main close_paren + :  type mapped to close_keyname_bracket
                    , semicolon               // main type passed-through
                    , other                   // unsupported token type
                    };






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------------
//    a_expression_S structure -- expression structure for AST from parsing
//    ------------------------
//
//
//    expression type                expected syntax
//    ---------------                -------------------------------------
//
//    verbless                       (value value value ...)
//    with verb                      (value/kw:value ... value/kw:value    @verbname              value/kw:value ... value/kw:value)   -- 0-N values or keyword:value pairs on either/both sides
//    with verb expression           (value/kw:value ... value/kw:value    @(verb_expression)@    value/kw:value ... value/kw:value)   -- 0-N values or keyword:value pairs on either/both sides
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct a_expression_S 
{
    bool                              has_verb         { false } ;      // true -- verb present,   false -- no verb found for this expression
    bool                              has_sigil        { false } ;      // true -- original verb had leading sigil
    char32_t                          sigil            {       } ;      // if has sigil is true, leading sigil chr is saved here 

    int                               priority         { 0     } ;      // verb priority (variable verbs use default priority, associativity, etc.)
    bool                              right_associate  { false } ;      // true = always right-to-left associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix 
    bool                              left_associate   { false } ;      // true = always left-to-right associativity, false = left-to-right when infix or postfix, right-to-left when prefix or nofix

    int64_t                           token_ix1        { -1    } ;      // starting token index for this expression, if known  
    int64_t                           token_ix2        { -1    } ;      // ending   token index for this expression, if known 
   
    value_S                           verb_value       {       } ;      // value with verb name (can be expression yielding a verb name)
    vlist_S                           lparms           {       } ;      // left-side  parameters for verb or option  
    vlist_S                           rparms           {       } ;      // right-side parameters for verb or option 
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------
//    block_S  structure
//    ------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct block_S
{
    uint64_t                          expression_ct      { 0     };      // number of expression in vector    
    std::vector<a_expression_S>       expressions                 ;      // vector of expressions (can be empty)

    int64_t                           token_ix1          { -1 }   ;      // starting token index for this expression, if known  
    int64_t                           token_ix2          { -1 }   ;      // ending   token index for this expression, if known  

    std::wstring                      label                       ;      // main label for this block (often empty string)
    std::map<std::wstring, uint64_t>  labels                      ;      // map with all labels (including main, if not empty) and associated expression indexes in this block
};




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////
////            ================
////            evaluation phase (ex_eval.cpp)  
////            ================
////
////
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    --------------------
//    environ_S  structure -- environment with verb names, type names, variable names, etc. 
//    --------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct environ_S
{
    int64_t                             sernum           { 0     };                  // serial number for next symbol in this environment -- for debugging  -- valid serial numbers start with -1 and go downward   
    bool                                is_global        { false };                  // true -- this environment is the global  environment)
    bool                                is_main          { false };                  // true -- this environment is the main (persistent)  environment)
    bool                                is_persistent    { false };                  // true -- this environment is for static or persistent environment for a user-defind verb   
    bool                                is_preprocess    { false };                  // true -- this environment is for pre-processor stack frame
    bool                                is_verbmain      { false };                  // true -- this environment is for the (non-persistent) stack frame of a user-defined verb
    bool                                is_frameblock    { false };                  // true -- this environment is for a frameblock -- internal to a verb, main, pre-process, etc.
    bool                                is_parmsonly     { false };                  // true -- this environment is for a parms-only SF -- same_scope: verb
    std::map<std::wstring, symval_S>    symbols          {       };                  // identifier names for this environment -- environment                       
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    -------------------
//    symval_S  structure -- symbol table entry
//    -------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct symval_S 
{
#ifdef M_EXPOSE_SUPPORT
    bool                           is_exposed                    { false };     // true -- this identifier is exposed  -- visible in nested dynamic scopes
#endif
    bool                           is_const                      { false };     // true -- identifier is constant       -- value cannot be updated, cannot be undefined  
    bool                           is_verbset                    { false };     // true -- identifier is a verbset      -- value cannot be updated if any built-in verbdefs in the set
    bool                           is_typdef                     { false };     // true -- identifier is a typdef       -- value cannot be updated, cannot be undefined
    bool                           is_builtin                    { false };     // true -- identifier is built-in       -- (verb/type/etc.) value cannot be updated, cannot be undefined 
    bool                           is_alias                      { false };     // true -- identifier is an alias       -- updatability depends in main identifier -- alias can always be deleted
                                                                                //                                         ???? not supported yet ?????? 
    bool                           no_shadow                     { false };     // true -- identifier cannot be shadowed (only valid for global identifiers) 
    bool                           no_undefine                   { false };     // true -- identifier cannot be undefined  
    bool                           no_update                     { false };     // true -- identifier cannot be updated  

    uint64_t                       serno                         { 0     };     // symval_S serial number      

    std::shared_ptr<value_S>       value_sp                      {       };     // pointer to associated value 
};
 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------
//    frame_S  structure -- stack fame for frameblock, mainblock verb-block, or preprocessor-block
//    ------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct frame_S 
{  
    std::shared_ptr<frame_S>                      parent_sp                  {         };     // shared pointer to parent stack frame -- owning pointer (not initialized, if this is top/main stack frame)
	frame_S                                      *child_p                    { nullptr };     // pointer to child stack frame -- non-owning pointer (0, if this is most deeply-nested stack frame -- non-owning ptr
    std::weak_ptr<frame_S>                        self_wp                    {         };     // non-owning self-pointer -- for use_count() tracking   
    std::shared_ptr<frame_S>                      scope_sp                   {         };     // shared owning pointer to upward scope stack frame  (used to keep the upward enclosing scopes (especially ones above the defining scope) around for closures) 

    environ_S                                    *environ_p                  { nullptr };     // pointer to local environ_S -- normally points to this->environ field below
    environ_S                                     env                        {         };     // local environment -- has with verb names and variable names for this stack frame (if environ_p points to here) 

	frame_S                                      *persist_sf_p               { nullptr };     // pointer to persistent SF for this verb (if any) - persistent SF pointer to itself 
    frame_S                                      *verbmain_sf_p              { nullptr };     // pointer to main SF for this verb  -- main verb SF points to itself  
    frame_S                                      *local_sf_p                 { nullptr };     // pointer to SF that has local identifiers (for new definitionas, etc.)
    frame_S                                      *search_sf_p                { nullptr };     // pointer to SF where search for identifiers should start

    uint64_t                                      serial                     {         };     // serial number for this stack frame -- for debugging

    bool                                          is_main                    { false   };     // true -- this is topmost/main stack frame
    bool                                          is_preprocess              { false   };     // true -- this stack frame belongs to pre-processor 
    bool                                          is_persistent              { false   };     // true -- this stack frame is the persistent SF for a user-defined verb 
    bool                                          is_verbmain                { false   };     // true -- this stack frame started because of verb execution -- is non-persistent  
    bool                                          is_frameblock              { false   };     // true -- this stack frame started because of frameblock execution 
    bool                                          is_parmsonly               { false   };     // true -- this stack frame is for saving verb invocation parms only (verb has same_scope:) 
   
    bool                                          lexical_scope              { false   };     // true -- this stack_frame has lexical scope upwards to parent stack frame  
    bool                                          dynamic_scope              { false   };     // true -- this stack_frame has dynamic scope upwards to parent stack frame 
#ifdef M_EXPOSE_SUPPORT
    bool                                          dynall_scope               { false   };     // true -- this stack_frame has dynall  scope upwards to parent stack frame -- like dynamic, but all variables visible, not just exposed ones 
#endif
    bool                                          no_scope                   { false   };     // true -- this stack_frame has no upwards scope to parent stack frame      -- no variables visible except global and local
    bool                                          same_scope                 { false   };     // true -- this stack frame is for parms-only -- upwards scope is dynamic/dynall
    bool                                          shared_environ             { false   };     // true -- environ_p points to a shared environment, not to this->environ 
#ifdef M_EXPOSE_SUPPORT
    bool                                          exposes_done               { false   };     // true -- one or more exposes have been done
#endif                                             
    uint64_t                                      verb_eval_ct               { 0       };     // number of verbs evaluated (for statistics) 
    std::wstring                                  verbname                   {         };     // name of verb being run (if frame_S started by verb) in this stack frame
    vlist_S                                       lparms                     {         };     // left-side  parameters for this frameblock/verb-block/mainblock  
    vlist_S                                       rparms                     {         };     // right-side parameters for this frameblock/verb-block/mainblock 

  //tracker_C                                     tracker                    {  111    };     // temp for storage leak debugging 
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    --------------------
//    results_S  structure
//    --------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct results_S : public value_S
{
    bool                                  multiple_results                 {false};     // multiple individual results (if any) are returned in vlist positional values  (value type may be none, since values are in vlist)
                                                                                        // this flag is also on when the no_results flag is on (meaning there is no vlist_sp at all) 
                                                                                        // this flag is on if there is a single result (or none) in the vlist 
                                                                                        // this lfag is off if the results value is returned toe notmal way -- in the imbedded value_S structure
    bool                                  no_results                       {false};     // no results -- vlist_sp is not initialized  (value type is "none")
    bool                                  ignore_results                   {false};     // true -- these results don't overwrite results from prior expression in the block (used for @CONTINUE, etc.)
    bool                                  re_eval_expression_results       {false};     // need to call eval_value() again after 1st call to eval_value() returned a vlist or identifier from evaluating a nested expression  
    bool                                  builtin_verb_results             {false};     // true, if these results came from a builtin verb   

    bool                                  special_results                  {false};     // this flag is on, if any of the following flags are on -- for quick testing
    bool                                  error                            {false}; 
    bool                                  break_flag                       {false};     // @BREAK out of lowest enclosing @LOOP
    bool                                  continue_flag                    {false};     // @CONTINUE -- end loop block, but keep looping
    bool                                  quit_flag                        {false};     // @QUIT -- lowest enclosing @BLOCK 
    bool                                  end_flag                         {false};     // @END --  immediately end the main block 
    bool                                  leave_flag                       {false};     // @LEAVE some active enclosing (perhaps-labelled) block -- optional label (if any) is in .str member
    bool                                  goto_flag                        {false};     // @GOTO some label in an active enclosing block -- required label is in .str member
    bool                                  xctl_flag                        {false};     // @XCTL pending to some verb -- new expression with verb to xctl-to is in value_S base struct
    bool                                  return_flag                      {false};     // @RETURN from lowest enclosing user-defined verb -- value is in value_S base struct
    bool                                  throw_flag                       {false};     // @THROW to nearest @TRY catch: verb              -- value is in value_S base struct
    bool                                  skip_flag                        {false};     // @SKIPTO -- special forward-only pre-processor branch  

    std::wstring                          str                                     ;     // @LEAVE or @GOTO label string, @XCTL verb name

    /////////////////////////////////////////////////////////////////////////

    bool                                  suppress_eval_once               {false};     // suppress evaluation once after value is returned from expression evaluation  


    /// rest of fields are inherited from value_S
};




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    -----------------------
//    parm_check_S  structure
//    -----------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct parm_check_S
{
    uint64_t                              first_parm_lineno                  { 0   };   // ending line number of 1st verb parm        (source ID not recorded)         
    uint64_t                              verb_lineno                        { 0   };   // starting line number of verb               (source ID not recorded) 
    uint64_t                              last_parm_lineno                   { 0   };   // starting line number of last verb parm     (source ID not recorded)
    
    uint64_t                              min_left_pos_parm                  { 0   };   // mimimum number of left  positional parms allowed by any member of overload set
    uint64_t                              max_left_pos_parm                  { 0   };   // maximum number of left  positional parms allowed by any member of overload set
    uint64_t                              min_right_pos_parm                 { 0   };   // mimimum number of right positional parms allowed by any member of overload set
    uint64_t                              max_right_pos_parm                 { 0   };   // maximum number of right positional parms allowed by any member of overload set

    std::set<std::wstring>                left_keywords                      {     };   // set of all allowed  left-side (top-level) keywords for any member of overload set               
    std::set<std::wstring>                right_keywords                     {     };   // set of all allowed right-side (top-level) keywords for any member of overload set 
};





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
/////
/////  ==============================================================
/////  Parse-oriented external function prototypes -- ex_preprocess.c
/////  ==============================================================
/////
/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////// external setup and processing functions

pre_process_C *add_new_pp();                  // add new pre_process_C to top of preprocess stack 
void           remove_pp();                   // remove newest pre_process_C on stack and free it 
pre_process_C *current_pp();                  // return pointer to newest (current) pre_process_C instance 



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
/////
/////  =========================================================
/////  Parse-oriented external function prototypes -- ex_parse.c
/////  =========================================================
/////
/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// preprocess control

uint64_t     preprocess_setup(pre_process_C&);  // customize configuration settings in pre_process_C object


/////////////////// external setup and processing functions

int           process_main_file(   frame_S& ,                               const std::wstring&                                     );
std::wstring  process_cmdline(     frame_S&, environ_S&, int, wchar_t *[]                                                           ); 
int           parse_string(        frame_S&, block_S&, const std::wstring&, const std::wstring& = L"parsed string", bool = false    );


//////////////// debug/statstical functions

float64_T    get_parse_elapsed();  
float64_T    get_eval_elapsed(); 


///////////////// location string and other debug-message-oriented routines

std::wstring   expression_loc_str(const a_expression_S& );
std::wstring         verb_loc_str(const a_expression_S& );
//std::wstring expression_loc_str(const e_expression_S& );   // in ex_interface.h
//std::wstring       verb_loc_str(const e_expression_S& );   // in ex_interface.h
std::wstring        vlist_loc_str(const vlist_S&        );
std::wstring        value_loc_str(const value_S&        ); 
std::wstring           kw_loc_str(const value_S&        );
  
void                msg_loc( const   value_S&                          );
void                msg_loc( const   value_S&, const std::wstring&     );
void                msg_loc( const   vlist_S&                          );
void                msg_loc( const   vlist_S&, const std::wstring&     );
void                msg_loc(                   const a_expression_S&   );
//void              msg_loc(                   const e_expression_S&   );   // in ex_interface.h
void                msg_loc( const   value_S&, const e_expression_S&   );
void                msg_loc( const   value_S&, const a_expression_S&   );
void                msg_loc( const   vlist_S&, const e_expression_S&   );
void                msg_loc( const   vlist_S&, const a_expression_S&   );

void             msg_kw_loc( const   value_S&                          );
                                                                       
void             msgend_loc( const   value_S&                          );
void             msgend_loc( const   value_S&, const std::wstring&     );
void             msgend_loc( const   vlist_S&                          );
void             msgend_loc( const   vlist_S&, const std::wstring&     );
void             msgend_loc(                   const a_expression_S&   );
//void           msgend_loc(                   const e_expression_S&   );   // in ex_interface.h
void             msgend_loc( const   value_S&, const e_expression_S&   );
void             msgend_loc( const   value_S&, const a_expression_S&   );
void             msgend_loc( const   vlist_S&, const e_expression_S&   );
void             msgend_loc( const   vlist_S&, const a_expression_S&   );

token_C token_list_at(std::vector<token_C>::size_type);


/////////////////// expression-oriented external functions

void expression_set_verb(frame_S&, a_expression_S&, const token_C&       ,  const verbset_S&  );   // version with passed-in verbname token + dummy? verbset_S
void expression_set_verb(frame_S&, a_expression_S&, const a_expression_S&,  bool = false      );   // version with passed-in verb sub-expression 


/////////////////// vlist-oriented external functions

void    set_vlist_flags(vlist_S&);        // should be moved to data.cpp ?????????

// see also add_positional_value(), below


/////////////////// value-oriented external functions

void    add_positional_value(vlist_S&, const value_S&,                    bool = false);
void    add_keyword_value(   vlist_S&, const value_S&, const value_S&                 );




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
/////
/////  =======================================
/////  Debug/Display functions -- ex_display.c
/////  =======================================
/////
/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////// printable string-oriented functions

std::wstring str_value( const value_S&, bool = false, bool = false, bool = false);
std::wstring str_vlist( const vlist_S&, bool = false, bool = false, bool = false);

std::wstring type_str(  type_E); 

std::wstring verb_name(     const a_expression_S&);
std::wstring verb_name(     const e_expression_S&);


//////////////// output/display-oriented functions

void    display_statistics();

void    display_vlist(      const vlist_S&        , const std::wstring& = L"vlist"        , const std::wstring& = L"",               bool = false, const std::wstring& = L"");
void    display_block(      const block_S&        , const std::wstring& = L"block"        , const std::wstring& = L"", bool = false, bool = false, const std::wstring& = L"");
void    display_expression( const a_expression_S& , const std::wstring& = L"expression"   , const std::wstring& = L"",               bool = true , const std::wstring& = L"");
void    display_expression( const e_expression_S& , const std::wstring& = L"expression"   , const std::wstring& = L"",               bool = true , const std::wstring& = L"");
void    display_value(      const value_S&        , const std::wstring& = L"value"        , const std::wstring& = L"",               bool = false, const std::wstring& = L"");
void    display_results(    const results_S&      , const std::wstring& = L"results"      , const std::wstring& = L"",               bool = false, const std::wstring& = L"");
void    display_buffer( const std::wstring&       , const buf8_T&     ,  size_t           , const std::wstring& = L""                                                       ); 

std::wstring results_msg_string( const results_S&);
               
void    display_ref(                          const ref_S&                                            );
void    display_verbset(                      const verbset_S&                                        );
void    display_typdef(  const std::wstring&, const typdef_S&   ,            const std::wstring& = L""); 
void    display_fieldef( const std::wstring&, const fieldef_S&  ,            const std::wstring& = L""); 

void    display_stack(        const frame_S&                          );
void    display_vars(         const frame_S&, const vlist_S&          );
void    display_verbsets(     const frame_S&, const vlist_S&          );
void    display_typdefs(      const frame_S&, const vlist_S&          );

void    display_all_verbsets( const frame_S&, bool = true, bool = true);
void    display_all_typdefs(  const frame_S&, bool = true, bool = true);
void    display_all_vars(     const frame_S&                          );
 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
/////
/////  =================================================================================
/////  Environment / stack frame - oriented external function prototypes -- ex_environ.c
/////  =================================================================================
/////
/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////// verbdef definition external functions --- 

struct def_parm_S
{
    bool          builtin         { false };        // true -- this is built-in definition
    bool          constant        { false };        // true -- this is a constant definition
#ifdef M_EXPOSE_SUPPORT
    bool          exposed         { false };        // true -- this is definition is exposed (forced on for globals)
#endif
    bool          unshare         { false };        // true -- value must be unshared when defined
    bool          no_shadow       { false };        // true -- this definition can't be shadowsd -- forced off for non-local dedinintions
    bool          no_update       { false };        // true -- this value can't be updated -- forced on for built-in, constant, or verb and type definitions
    bool          no_undefine     { false };        // true -- this value can't be undefined -- forced on for biult-in, constant, or type definitions
};


int     def_global_verb(                               const std::wstring&, const verbset_S&, const def_parm_S&  = def_parm_S { }        );
int     def_static_verb(                     frame_S&, const std::wstring&, const verbset_S&, const def_parm_S&  = def_parm_S { }        );
int   def_verbmain_verb(                     frame_S&, const std::wstring&, const verbset_S&, const def_parm_S&  = def_parm_S { }        );
int      def_local_verb(                     frame_S&, const std::wstring&, const verbset_S&, const def_parm_S&  = def_parm_S { }        );
                                                                                                                                        
int   undef_global_verb(                               const std::wstring&                                                               ); 
int   undef_static_verb(                     frame_S&, const std::wstring&                                                               ); 
int undef_verbmain_verb(                     frame_S&, const std::wstring&                                                               ); 
int    undef_local_verb(                     frame_S&, const std::wstring&                                                               ); 
                                                                                                                                        
int     get_global_verb(                               const std::wstring&,       verbset_S&                                             );
int     get_static_verb(               const frame_S&, const std::wstring&,       verbset_S&                                             ); 
int   get_verbmain_verb(               const frame_S&, const std::wstring&,       verbset_S&                                             ); 
int      get_local_verb(               const frame_S&, const std::wstring&,       verbset_S&                                             ); 
int            get_verb(               const frame_S&, const std::wstring&,       verbset_S&                                             ); 
int     get_global_verb(                               const std::wstring&,       symval_S&                                              );
int     get_static_verb(               const frame_S&, const std::wstring&,       symval_S&                                              );
int   get_verbmain_verb(               const frame_S&, const std::wstring&,       symval_S&                                              );
int      get_local_verb(               const frame_S&, const std::wstring&,       symval_S&                                              );
int            get_verb(               const frame_S&, const std::wstring&,       symval_S&                                              );  
                                                                                                                                        
                                                                                                                                        
/////////////////////////////////// typdef definition external functions ---                                                            
                                                                                                                                        
int     def_global_typdef(                             const std::wstring&, const typdef_S&,  const def_parm_S&  = def_parm_S { }        );
int     def_static_typdef(                   frame_S&, const std::wstring&, const typdef_S&,  const def_parm_S&  = def_parm_S { }        );
int   def_verbmain_typdef(                   frame_S&, const std::wstring&, const typdef_S&,  const def_parm_S&  = def_parm_S { }        );
int      def_local_typdef(                   frame_S&, const std::wstring&, const typdef_S&,  const def_parm_S&  = def_parm_S { }        );
                                                                                                                                        
int   undef_global_typdef(                             const std::wstring&                                                               ); 
int   undef_static_typdef(                   frame_S&, const std::wstring&                                                               ); 
int undef_verbmain_typdef(                   frame_S&, const std::wstring&                                                               ); 
int    undef_local_typdef(                   frame_S&, const std::wstring&                                                               ); 
                                                                                                                                        
int     get_global_typdef(                             const std::wstring&,       typdef_S&                                              );
int     get_static_typdef(             const frame_S&, const std::wstring&,       typdef_S&                                              ); 
int   get_verbmain_typdef(             const frame_S&, const std::wstring&,       typdef_S&                                              ); 
int      get_local_typdef(             const frame_S&, const std::wstring&,       typdef_S&                                              ); 
int            get_typdef(             const frame_S&, const std::wstring&,       typdef_S&                                              ); 
int     get_global_typdef(                             const std::wstring&,       symval_S&                                              );
int     get_static_typdef(             const frame_S&, const std::wstring&,       symval_S&                                              );
int   get_verbmain_typdef(             const frame_S&, const std::wstring&,       symval_S&                                              );
int      get_local_typdef(             const frame_S&, const std::wstring&,       symval_S&                                              );
int            get_typdef(             const frame_S&, const std::wstring&,       symval_S&                                              );     


/////////////////////////////////// variable/constant definition external functions ---

                                                                                             
int       def_local_var(                      frame_S&, const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
int       def_parms_var(                      frame_S&, const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
int    def_verbmain_var(                      frame_S&, const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
int      def_static_var(                      frame_S&, const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
int      def_global_var(                                const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
     
int     def_local_const(                      frame_S&, const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
int  def_verbmain_const(                      frame_S&, const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
int    def_static_const(                      frame_S&, const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
int    def_global_const(                                const std::wstring&, const value_S&,    const def_parm_S& = def_parm_S { }       );
                                    
int    undef_global_var(                                const std::wstring&                                                              );
int    undef_static_var(                      frame_S&, const std::wstring&                                                              );
int  undef_verbmain_var(                      frame_S&, const std::wstring&                                                              );
int     undef_local_var(                      frame_S&, const std::wstring&                                                              );
                                    
int      get_global_var(                                const std::wstring&,       value_S&                                              );
int      get_static_var(                const frame_S&, const std::wstring&,       value_S&                                              );
int    get_verbmain_var(                const frame_S&, const std::wstring&,       value_S&                                              );
int       get_local_var(                const frame_S&, const std::wstring&,       value_S&                                              );
int             get_var(                const frame_S&, const std::wstring&,       value_S&                                              );
int      get_global_var(                                const std::wstring&,       symval_S&                                             );
int      get_static_var(                const frame_S&, const std::wstring&,       symval_S&                                             );
int    get_verbmain_var(                const frame_S&, const std::wstring&,       symval_S&                                             );
int       get_local_var(                const frame_S&, const std::wstring&,       symval_S&                                             );
int             get_var(                const frame_S&, const std::wstring&,       symval_S&                                             );    
                                                                                            
                                                                                              /* make_const                 unshare  */
int   update_global_var(                                const std::wstring&, const value_S&,    bool = false,               bool = false ); 
int   update_static_var(                      frame_S&, const std::wstring&, const value_S&,    bool = false,               bool = false );
int update_verbmain_var(                      frame_S&, const std::wstring&, const value_S&,    bool = false,               bool = false );
int    update_local_var(                      frame_S&, const std::wstring&, const value_S&,    bool = false,               bool = false );
int          update_var(                      frame_S&, const std::wstring&, const value_S&,    bool = false,               bool = false );

       
/////////////////////////////////// identifier/symbol-oriented external functions

bool is_local_identifier_defined(       const frame_S&, const std::wstring&                                                              );
bool is_local_identifier_variable(      const frame_S&, const std::wstring&                                                              );
bool is_local_identifier_const(         const frame_S&, const std::wstring&                                                              );
bool is_local_identifier_verb(          const frame_S&, const std::wstring&                                                              );
bool is_local_identifier_typdef(        const frame_S&, const std::wstring&                                                              );
bool is_local_identifier_builtin(       const frame_S&, const std::wstring&                                                              );
bool is_local_identifier_alias(         const frame_S&, const std::wstring&                                                              );
bool is_local_identifier_mutable(       const frame_S&, const std::wstring&                                                              );
bool is_local_identifier_removable(     const frame_S&, const std::wstring&                                                              );

bool is_verbmain_identifier_defined(    const frame_S&, const std::wstring&                                                              );
bool is_verbmain_identifier_variable(   const frame_S&, const std::wstring&                                                              );
bool is_verbmain_identifier_const(      const frame_S&, const std::wstring&                                                              );
bool is_verbmain_identifier_verb(       const frame_S&, const std::wstring&                                                              );
bool is_verbmain_identifier_typdef(     const frame_S&, const std::wstring&                                                              );
bool is_verbmain_identifier_builtin(    const frame_S&, const std::wstring&                                                              );
bool is_verbmain_identifier_alias(      const frame_S&, const std::wstring&                                                              );
bool is_verbmain_identifier_mutable(    const frame_S&, const std::wstring&                                                              );
bool is_verbmain_identifier_removable(  const frame_S&, const std::wstring&                                                              );

bool is_static_identifier_defined(      const frame_S&, const std::wstring&                                                              );
bool is_static_identifier_variable(     const frame_S&, const std::wstring&                                                              );
bool is_static_identifier_const(        const frame_S&, const std::wstring&                                                              );
bool is_static_identifier_verb(         const frame_S&, const std::wstring&                                                              );
bool is_static_identifier_typdef(       const frame_S&, const std::wstring&                                                              );
bool is_static_identifier_builtin(      const frame_S&, const std::wstring&                                                              );
bool is_static_identifier_alias(        const frame_S&, const std::wstring&                                                              );
bool is_static_identifier_mutable(      const frame_S&, const std::wstring&                                                              );
bool is_static_identifier_removable(    const frame_S&, const std::wstring&                                                              );

bool is_global_identifier_defined(                      const std::wstring&                                                              );
bool is_global_identifier_variable(                     const std::wstring&                                                              );
bool is_global_identifier_const(                        const std::wstring&                                                              );
bool is_global_identifier_verb(                         const std::wstring&                                                              );
bool is_global_identifier_typdef(                       const std::wstring&                                                              );
bool is_global_identifier_builtin(                      const std::wstring&                                                              );
bool is_global_identifier_alias(                        const std::wstring&                                                              );   
bool is_global_identifier_mutable(                      const std::wstring&                                                              ); 
bool is_global_identifier_removable(                    const std::wstring&                                                              ); 

bool is_identifier_defined(             const frame_S&, const std::wstring&                                                              );
bool is_identifier_variable(            const frame_S&, const std::wstring&                                                              );
bool is_identifier_const(               const frame_S&, const std::wstring&                                                              );
bool is_identifier_verb(                const frame_S&, const std::wstring&                                                              );
bool is_identifier_typdef(              const frame_S&, const std::wstring&                                                              );
bool is_identifier_builtin(             const frame_S&, const std::wstring&                                                              );
bool is_identifier_alias(               const frame_S&, const std::wstring&                                                              );
bool is_identifier_mutable(             const frame_S&, const std::wstring&                                                              );
bool is_identifier_removable(           const frame_S&, const std::wstring&                                                              );
                                                                                                                                        
int  def_global_identifier(                             const std::wstring&,       symval_S&                           , bool = false    ); 
int  def_static_identifier(                   frame_S&, const std::wstring&,       symval_S&                           , bool = false    ); 
int  def_verbmain_identifier(                 frame_S&, const std::wstring&,       symval_S&                           , bool = false    ); 
int  def_local_identifier(                    frame_S&, const std::wstring&,       symval_S&                           , bool = false    ); 
                                                                                                                                        
int  undef_global_identifier(                           const std::wstring&                                                              ); 
int  undef_static_identifier(                 frame_S&, const std::wstring&                                                              ); 
int  undef_verbmain_identifier(               frame_S&, const std::wstring&                                                              ); 
int  undef_local_identifier(                  frame_S&, const std::wstring&                                                              );  
                                                                                                                                        
int  get_global_identifier(                             const std::wstring&,       value_S&                                              );
int  get_static_identifier(             const frame_S&, const std::wstring&,       value_S&                                              );
int  get_verbmain_identifier(           const frame_S&, const std::wstring&,       value_S&                                              );
int  get_local_identifier(              const frame_S&, const std::wstring&,       value_S&                                              );
int  get_identifier(                    const frame_S&, const std::wstring&,       value_S&                                              ); 

#ifdef M_EXPOSE_SUPPORT
int  expose_local_identifier(                 frame_S&, const std::wstring&                                            , bool = true     );
int  expose_verbdef_identifier(               frame_S&, const std::wstring&                                            , bool = true     );  
int  expose_static_identifier(                frame_S&, const std::wstring&                                            , bool = true     );                                                                                                                                        
#endif                             


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
/////
/////  ================================================================
/////  Data and type-oriented external function prototypes -- ex_data.c
/////  ================================================================
/////
/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// fieldparm_S parm for make_structure_typdef()
// --------------------------------------------

struct fieldparm_S
{                                                                     // note: default base/starting offset for this field is right after end of prior field
    bool                           has_offset      { false   } ;      // true, if offset value is valid -- explicit base/starting offset for this field (rather than default)  
    bool                           has_skip        { false   } ;      // true, if skip value is valid -- skip this many bytes past base/starting offset to get adjusted field starting offset
    bool                           same_as_prior   { false   } ;      // true, if base/starting offset for this field is same as start of prior field (instead of end of prior field)
    bool                           at_hi_watermark { false   } ;      // true, if base/starting offset for this field should be just past end of highest field seen so far
    std::wstring                   fieldname       {         } ;      // name of field
    typdef_S                      *typdef_p        { nullptr } ;      // type of field -- ptr to typdef
    int64_t                        offset          { 0       } ;      // offset for start of this field, if   has_offset   is true                        
    int64_t                        skip            { 0       } ;      // number of bytes to skip before this field, if   has_skip   is true
} ;


//////////////////////////////// results-oriented functions /////////////////////////////////////////////////////////////////

results_S       to_results( const value_S& );    
results_S    error_results(                ); 
results_S  special_results(                ); 
results_S       no_results(                );
results_S     unit_results(                );
results_S     true_results(                );
results_S    false_results(                );
results_S       tf_results( bool tf        );    


//////////////////////////////// value-oriented functions /////////////////////////////////////////////////////////////////

void      refresh_evaluated_vlist(vlist_S&); 

value_S   unit_val(                                                  int64_t = -1, int64_t = -1 );
value_S   nval_val(                                                  int64_t = -1, int64_t = -1 );     // only for keywords with no following value
value_S   boolean_val(     bool                                    , int64_t = -1, int64_t = -1 );
value_S   int8_val(        int8_t                                  , int64_t = -1, int64_t = -1 );
value_S   int16_val(       int16_t                                 , int64_t = -1, int64_t = -1 );
value_S   int32_val(       int32_t                                 , int64_t = -1, int64_t = -1 );
value_S   int64_val(       int64_t                                 , int64_t = -1, int64_t = -1 );
value_S   uint8_val(       uint8_t                                 , int64_t = -1, int64_t = -1 );
value_S   uint16_val(      uint16_t                                , int64_t = -1, int64_t = -1 );
value_S   uint32_val(      uint32_t                                , int64_t = -1, int64_t = -1 );
value_S   uint64_val(      uint64_t                                , int64_t = -1, int64_t = -1 );
value_S   float32_val(     float32_T                               , int64_t = -1, int64_t = -1 );
value_S   float64_val(     float64_T                               , int64_t = -1, int64_t = -1 );
value_S   string_val(      const wchar_t *                         , int64_t = -1, int64_t = -1 );       // no type_val version for this function
value_S   identifier_val(  const wchar_t *                         , int64_t = -1, int64_t = -1 );       // no type_val version for this function
value_S   string_val(      const std::wstring&                     , int64_t = -1, int64_t = -1 );
value_S   identifier_val(  const std::wstring&                     , int64_t = -1, int64_t = -1 );       // no type_val version for this function
value_S   vlist_val(       const vlist_S&                          , int64_t = -1, int64_t = -1 );
value_S   expression_val(  const a_expression_S&                   , int64_t = -1, int64_t = -1 );
value_S   block_val(       const block_S&                          , int64_t = -1, int64_t = -1 );
value_S   verbset_val(     const verbset_S&                        , int64_t = -1, int64_t = -1 );
value_S   typdef_val(      const typdef_S&                         , int64_t = -1, int64_t = -1 );
value_S   ref_val(         const ref_S&                            , int64_t = -1, int64_t = -1 );
value_S   buffer_val(      const buf8_T&       , const typdef_S&   , int64_t = -1, int64_t = -1 );
value_S   buffer_val(      const buf8_T&       , const typdef_S&   , bool);                              // special internal version that allows std::move()

value_S   type_val(        bool                                    , int64_t = -1, int64_t = -1 ); 
value_S   type_val(        int8_t                                  , int64_t = -1, int64_t = -1 );    
value_S   type_val(        int16_t                                 , int64_t = -1, int64_t = -1 );    
value_S   type_val(        int32_t                                 , int64_t = -1, int64_t = -1 );    
value_S   type_val(        int64_t                                 , int64_t = -1, int64_t = -1 );    
value_S   type_val(        uint8_t                                 , int64_t = -1, int64_t = -1 );    
value_S   type_val(        uint16_t                                , int64_t = -1, int64_t = -1 );    
value_S   type_val(        uint32_t                                , int64_t = -1, int64_t = -1 );    
value_S   type_val(        uint64_t                                , int64_t = -1, int64_t = -1 );    
value_S   type_val(        float32_T                               , int64_t = -1, int64_t = -1 );    
value_S   type_val(        float64_T                               , int64_t = -1, int64_t = -1 );    
value_S   type_val(        const std::wstring&                     , int64_t = -1, int64_t = -1 );    
value_S   type_val(        const vlist_S&                          , int64_t = -1, int64_t = -1 );    
value_S   type_val(        const a_expression_S&                   , int64_t = -1, int64_t = -1 );
value_S   type_val(        const block_S&                          , int64_t = -1, int64_t = -1 );
value_S   type_val(        const verbset_S&                        , int64_t = -1, int64_t = -1 );
value_S   type_val(        const typdef_S&                         , int64_t = -1, int64_t = -1 ); 
value_S   type_val(        const ref_S&                            , int64_t = -1, int64_t = -1 ); 
value_S   type_val(        const buf8_T&,        const typdef_S&   , int64_t = -1, int64_t = -1 );


int       get_val(         const value_S&,   int8_t&  );  
int       get_val(         const value_S&,  uint8_t&  );
int       get_val(         const value_S&,  int16_t&  );  
int       get_val(         const value_S&, uint16_t&  );
int       get_val(         const value_S&,  int32_t&  );  
int       get_val(         const value_S&, uint32_t&  );
int       get_val(         const value_S&,  int64_t&  );  
int       get_val(         const value_S&, uint64_t&  );



///////////////////  value setting and unsharing functions

void    set_vlist_value(       value_S&, const vlist_S&                     , bool = false);
void    set_expression_value(  value_S&, const a_expression_S&              , bool = false);
void    set_block_value(       value_S&, const block_S&                     , bool = false);
void    set_verbset_value(     value_S&, const verbset_S&                   , bool = false);
void    set_typdef_value(      value_S&, const typdef_S&                    , bool = false);
void    set_ref_value(         value_S&, const ref_S&                       , bool = false);
void    set_buffer_value(      value_S&, const buf8_T&    , const typdef_S& , bool = false);

void      unshare_value(       value_S&        ); 
void      unshare_vlist(       vlist_S&        );
void      unshare_expression(  a_expression_S& ); 
void      unshare_block(       block_S&        );
void      unshare_verbset(     verbset_S&      );
void      unshare_plist(       plist_S&        );
void      unshare_parmtype(    parmtype_S&     );
void      unshare_typdef(      typdef_S&       ); 


////////////////////  value testing functions

bool is_value_integer(    const value_S&);
bool is_value_signed(     const value_S&);
bool is_value_unsigned(   const value_S&);
bool is_value_float(      const value_S&);
bool is_value_arithmetic( const value_S&);
bool is_value_comparable( const value_S&);
bool is_value_boolean(    const value_S&);
bool is_value_true(       const value_S&);
bool is_value_false(      const value_S&);

bool is_same_class(       const value_S& , const value_S&);   


//////////////////// container/aggregate-oriented functions 


int                combine_vlists(vlist_S&, const vlist_S&, const vlist_S&, bool=false); 


/////////////////// type-oriented external functions

bool               is_type_valid(         type_E                                                           );   
bool               is_type_atomic(        type_E                                                           );
bool               is_type_aggregate(     type_E                                                           );
bool               is_type_fixed_size(    type_E                                                           );
bool               is_type_valid(         const typdef_S&                                                  );   
bool               is_type_atomic(        const typdef_S&                                                  );
bool               is_type_aggregate(     const typdef_S&                                                  );
bool               is_type_fixed_size(    const typdef_S&                                                  ); 

size_t             atomic_type_size(      type_E                                                           );

int                make_atomic_typdef(    type_E,                                         typdef_S&        );
typdef_S           make_atomic_typdef(    type_E                                                           );
int                make_array_typdef(     size_t,      const typdef_S&,                   typdef_S&        );
int                make_structure_typdef(              const std::vector<fieldparm_S>&,   typdef_S&        );

bool               typdefs_are_same(      const typdef_S&  , const typdef_S&                               ); 
bool               fieldefs_are_same(     const fieldef_S& , const fieldef_S&                              );


/////////////////// reference-oriented external functions

int                make_reference(     ref_S&, const std::shared_ptr<value_S>&, const typdef_S&, uint64_t       );
int                make_reference(     ref_S&, const value_S&,                  const typdef_S&, uint64_t       );
int                make_reference(     ref_S&, const environ_S&               , const std::wstring&             );

int                dereference_value(      value_S&, const value_S&                                             ); 
int                set_via_reference(const value_S&, const value_S&,                                   bool=true);






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
/////
/////  =============================================================
/////  Evaluation-oriented external function prototypes -- ex_eval.c
/////  =============================================================
/////
/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 


/////////////////////////////////// query and statistical functions

void       display_stack(const frame_S&);

uint64_t   get_eval_verb_count(      void);
uint64_t   get_eval_value_count(     void);
uint64_t   get_eval_block_count(     void);
uint64_t   get_eval_statement_count( void);

uint64_t   get_eval_frame_serial(    void);
uint64_t   get_eval_frame_depth(     void);
uint64_t   get_eval_frame_max_depth( void);

frame_S   *get_newest_sf(            void);        
                                                                              

/////////////////////////////////// verb parameter-oriented external functions   ---- can these be moved over to interface.h ???????? (functions to ex_interface.cpp)

int get_right_positional(      const e_expression_S& ,                       value_S&      , uint32_t = 0);
int get_left_positional(       const e_expression_S& ,                       value_S&      , uint32_t = 0);
int get_vlist_positional(      const vlist_S&        ,                       value_S&      , uint32_t = 0);
  
int get_right_keyword(         const e_expression_S& ,  const std::wstring&, value_S&      , uint32_t = 0);
int get_right_keyword(         const e_expression_S& ,  const std::wstring&, std::wstring& , uint32_t = 0);
int get_right_keyword(         const e_expression_S& ,  const std::wstring&, int64_t&      , uint32_t = 0);
int get_right_keyword(         const e_expression_S& ,  const std::wstring&, float64_T&    , uint32_t = 0);
int get_right_keyword(         const e_expression_S& ,  const std::wstring&,                 uint32_t = 0);

int get_left_keyword(          const e_expression_S& ,  const std::wstring&, value_S&      , uint32_t = 0);
int get_left_keyword(          const e_expression_S& ,  const std::wstring&, std::wstring& , uint32_t = 0);
int get_left_keyword(          const e_expression_S& ,  const std::wstring&, int64_t&      , uint32_t = 0);
int get_left_keyword(          const e_expression_S& ,  const std::wstring&, float64_T&    , uint32_t = 0);
int get_left_keyword(          const e_expression_S& ,  const std::wstring&,                 uint32_t = 0);

int get_vlist_keyword(         const vlist_S&        ,  const std::wstring&, value_S&      , uint32_t = 0);
int get_vlist_keyword(         const vlist_S&        ,                       value_S&      , uint32_t = 0);
int get_vlist_keyword(         const vlist_S&        ,  const std::wstring&,                 uint32_t = 0);


///////////////////////////////// principal evaluation functions

void           frame_cmdline_parms(      frame_S&, int, wchar_t *[] );

frame_S             *add_new_frame(                                            );
std::shared_ptr<frame_S> new_frame(                                            );
frame_S                 *add_frame(      const std::shared_ptr<frame_S>&       );
void                  remove_frame(                                            );
                         
int     eval_frame_block(      frame_S&,           const      vlist_S&, const vlist_S&, const   block_S&, results_S&);
int      eval_main_block(      frame_S&,                                                const   block_S&, int&      );
int  eval_verbinit_block(                                                               const verbdef_S&, results_S&);
int      eval_verb_block(      frame_S&,         const e_expression_S&,                 const verbdef_S&, results_S&);

int           eval_block(      frame_S&,  const        block_S&, results_S&                                   );
int           eval_expression( frame_S&,  const a_expression_S&, results_S&                    , bool = false );
int           eval_vlist(      frame_S&,               vlist_S&, results_S&, const    plist_S& , bool = false );   // passed-in vlist is updated in-place 
int           eval_value(      frame_S&,  const        value_S&, results_S&, const parmtype_S& , bool = false );







/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////
////
////                                                                                                                                                            
////             SSSS    U   U    M   M    M   M      A      RRRR     Y   Y                                                                                                          
////            S        U   U    MM MM    MM MM     A A     R   R     Y Y                                                                                                        
////             SSS     U   U    M M M    M M M    A   A    RRRR       Y                                                                                                        
////                S    U   U    M   M    M   M    AAAAA    R  R       Y                                                                                                       
////            SSSS      UUU     M   M    M   M    A   A    R   R      Y                                                                                                                          
////                                                                                                                                                            
////
////
////
////
////     -------------------------------------------------------------
////     special characters used by lex, preprocess, and parse phases:
////     -------------------------------------------------------------
////
////
////     LEX character subtypes and types:        
////
////
////     QQ    -- open/close quote
////     QQ1   -- open/close quote-1
////     QQ2   -- open/close quote-2
////
////     OQ    -- open quote
////     CQ    -- close quote
////     OQ1   -- open quote-1
////     CQ1   -- close quote-1
////     OQ2   -- open quote-2
////     CQ2   -- close quote-2
////     OAQ1  -- open angle quote-1
////     CAQ1  -- close angle quote-1
////     OAQ2  -- open angle quote-2
////     CAQ2  -- close angle quote-2
////     OOQ1  -- open other quote-1
////     COQ1  -- close other quote-1
////     OOQ2  -- open other quote-2
////     COQ2  -- close other quote-2
////
////     PU    -- punctuation
////     P1    -- punctuation-1
////     P2    -- punctuation-2  
////
////     OP    -- operator 
////     O1    -- operator-1
////     O2    -- operator-2
////
////     SP    -- special
////     S1    -- special-1 
////     S2    -- special-2 
////
////     AC    -- accent 
////     A1    -- accent-1
////     A2    -- accent-2
////
////     WS    -- whitespace
////     MM    -- miscellaneous supported char
////     SE    -- separator
////     $$    -- currency
////
////     OB    -- open bracket, paren, etc.  
////     CB    -- close bracket, paren, etc.
////     OP1   -- open parenthesis-1       
////     CP1   -- close parenthesis-1
////     OP2   -- open parenthesis-2       
////     CP2   -- close parenthesis-2
////     OK1   -- open bracket-1 
////     CK1   -- close bracket-1
////     OK2   -- open bracket-2 
////     CK2   -- close bracket-2
////     OC1   -- open brace-1 
////     CC1   -- close brace-1
////     OC2   -- open brace-2 
////     CC2   -- close brace-2
////     OAB1  -- open angle bracket-1 
////     CAB1  -- close angle bracket-1
////     OAB2  -- open angle bracket-2 
////     CAB2  -- close angle bracket-2
////     OSB1  -- open shell bracket-1 
////     CSB1  -- close shell bracket-1
////     OSB2  -- open shell bracket-2 
////     CSB2  -- close shell bracket-2
////     ODB   -- open dot bracket
////     CDB   -- close dot bracket
////     OQB   -- open quill bracket
////     CQB   -- close quill bracket
////     OCB1  -- open curved bracket-1 
////     CCB1  -- close curved bracket-1
////     OCB2  -- open curved bracket-2 
////     CCB2  -- close curved bracket-2
////     OKB1  -- open corner bracket-1 
////     CKB1  -- close corner bracket-1
////     OKB2  -- open corner bracket-2 
////     CKB2  -- close corner bracket-2
////     OLB1  -- open lens bracket-1 
////     CLB1  -- close lens bracket-1
////     OLB2  -- open lens bracket-2 
////     CLB2  -- close lens bracket-2
////     OOB   -- open other bracket 
////     COB   -- close other bracket
////
////
////     LEX token classes   (subset)
////    
////     fs    --  full_stop
////     comma --  comma
////     semi  --  semicolon
////     colon --  colon
////     spch  --  special_char
////     misc  --  misc_char
////     punct --  punctuation_char
////     o_par --  open_paren
////     c_par --  close_paren
////     o_bkt --  open_bracket
////     c_bkt --  close_bracket
////     o_brc --  open_brace
////     c_brc --  close_brace
////     o_abk --  open_angle_bracket
////     c_abk --  close_angle_bracket
////     o_dbk --  open_dot_bracket
////     c_dbk --  close_dot_bracket
////     o_qbk --  open_quill_bracket
////     c_qbk --  close_quill_bracket
////     o_msb --  open_misc_bracket
////     c_msb --  close_misc_bracket
////     op    --  operator 
////     van   --  vanishing separator (not passed back)
////     ws    --  whitespace (might not be passed back)
////     acc   --  accent char 
////     id    --  identifier
////     ustr  --  simple string -- no escape char 
////     estr1 --  string with escape char = `  
////     estr2 --  string with escape char = ´
////
////
////
////
////
////
////                                                                                                           Char Char     Lex
////      code  x  Unicode name                                       Keyboard entry                           SuTy Type     Token    Usage
////      ----  -  -------------------------------------------------- ---------------------------------------- ------------- -------- ------------------------------------------------------------------------------------------
////      0009     HT                                                                                          WS   WS       ws/*         
////      000A     LF                                                                                          WS   WS       ws/*         
////      000B     VT                                                                                          WS   WS       ws/*         
////      000C     FF                                                                                          WS   WS       ws/*         
////      000D     CR                                                                                          WS   WS       ws/*         
////      0020     SPACE                                                                                       WS   WS       ws/*         
////      0021  !  EXCLAMATION_MARK                                   !                                        O1   OP       op/*     preprocess: trailing sigil for quoted string variable substitution, ! and != verbs 
////      0022  "  QUOTATION_MARK                                     "                                        QQ2  QQ       estr1    lex: escaped string-1 start/end delimiter 
////      0023  #  NUMBER_SIGN                                        #                                        O1   OP       op/*     # verb (also @AT)
////      0024  $  DOLLAR_SIGN                                        $                                        $$   $$       id/*     can be in extended identifiers  (not supported by default) -- parm sigil for identifiers and operators 
////      0025  %  PERCENT_SIGN                                       %                                        O1   OP       op/*     % verb (also @REM)
////      0026  &  AMPERSAND                                          &                                        O1   OP       op/*     && operators
////      0027  '  APOSTROPHE                                         '                                        QQ1  QQ       ustr     start of word-string (ends with next whitespace char) 
////      0028  (  LEFT_PARENTHESIS                                   (                                        OP1  OB       o_par/1  expression open parenthesis
////      0029  )  RIGHT_PARENTHESIS                                  )                                        CP1  CB       c_par/1  expression close parenthesis
////      002A  *  ASTERISK                                           *                                        O1   OP      (op/*)    multiply verb ( * *= ),  --  lex: block comment delimiters ( /* */ ) 
////      002B  +  PLUS_SIGN                                          +                                        +    OP      (op/*)    add verb ( + ++ += ), etc.
////      002C  ,  COMMA                                              ,                                        ,    PU       comma/1  , verb (also @SEP)
////      002D  -  HYPHEN_MINUS                                       -                                        -    OP      (op/*)    minus/subtract verb ( - -- -= ) , etc.  (lex: can be attached sign)
////      002E  .  FULL_STOP                                          .                                        .    PU      (punct/1) lex: decimal point in floating point literals, line comment with newline suppression (...)
////      002F  /  SOLIDUS                                            /                                        /    OP      (op/*)    divide verb ( /  /= ), lex: start comment ( //    /~    /#     /< >/    /* */    /{ }/  )
////      003A  :  COLON                                              :                                        :    PU      (colon/1) trailing sigil for keyword identifiers and pre-processor labels
////      003B  ;  SEMICOLON                                          ;                                        ;    PU       semi/1   block expression separator
////      003C  <  LESS_THAN_SIGN                                     <                                        O1   OP      (op/*)    less than verb ( <  <= ), left assignment verb ( <<< ), etc. 
////      003D  =  EQUALS_SIGN                                        =                                        O1   OP       op/*     left assignment verb ( = ), equals verb ( == >= <= ¬=), etc.  
////      003E  >  GREATER_THAN_SIGN                                  >                                        O1   OP      (op/*)    greater than verb ( > >= ), right assignment verb ( >>> ) etc.
////      003F  ?  QUESTION_MARK                                      ?                                        O1   OP       op/*     pre-process: leading sigil for pre-processor action tokens, trailing sigil for variable substitution 
////      0040  @  COMMERCIAL_AT                                      @                                        O1   OP       op/*     leading sigil for verb            
////      005B  [  LEFT_SQUARE_BRACKET                                [                                        OK1  OB       o_brk/1  vlist open bracket                                                                    
////      005C  \  REVERSE_SOLIDUS                                    \                                        \    OP       (op/*)   lex: default escape character in "type 1 escaped strings"         
////      005D  ]  RIGHT_SQUARE_BRACKET                               ]                                        CK1  CB       c_brk/1  vlist close bracket
////      005E  ^  CIRCUMFLEX_ACCENT                                  ^                                        O1   OP       op/*     -----------------------
////      005F  _  LOW_LINE                                           _                                        _    SE       id/*     lex: visual separator in identifiers and numeric literals
////      0060  `  GRAVE_ACCENT                                       `                                        A1   AC       acc/1    ----------------------  
////      007B  {  LEFT_CURLY_BRACKET                                 {                                        OC1  OB       o_brc/1  block open brace
////      007C  |  VERTICAL_LINE                                      |                                        O1   OP       op/*     string concatenate verb  ( |  |= ),    logical or || 
////      007D  }  RIGHT_CURLY_BRACKET                                }                                        CC1  CB       c_brc/1  block close brace
////      007E  ~  TILDE                                              ~                                        O1   OP       op/*     -----------------------
////      00A1  ¡  INVERTED_EXCLAMATION_MARK                          AltGr + 1         Alt-173   Alt-0417     P2   PU       punct/1  -----------------------
////      00A2  ¢  CENT_SIGN                                          ---------------   Alt-155   Alt-0930     $$   $$       id/*     can be in extended identifiers (not supported by default)
////      00A3  £  POUND_SIGN                                         AltGr + $         Alt-3740  Alt-0419     $$   $$       id/*     can be in extended identifiers (not supported by default)
////      00A4  ¤  CURRENCY_SIGN                                      AltGr + 4         --------  Alt-01444    $$   $$       id/*     can be in extended identifiers (not supported by default)
////      00A5  ¥  YEN_SIGN                                           ---------------   Alt-157   Alt-0933     $$   $$       id/*     can be in extended identifiers (not supported by default)
////      00A6  ¦  BROKEN_BAR                                         AltGr+Shift + |   --------  Alt-0934     O2   OP       op/*     ------------------------
////      00A7  §  SECTION_SIGN                                       AltGr+Shift + S   Alt-533   Alt-0935     S2   SP       spch/1   ------------------------
////      00A8  ¨  DIAERESIS                                          AltGr+Shift + "   --------  Alt-01704    A2   AC       acc/1    ------------------------
////      00A9  ©  COPYRIGHT_SIGN                                     AltGr + c         --------  Alt-0937     MM   MM       misc/1   ------------------------
////      00AA  ª  FEMININE_ORDINAL_INDICATOR                         ---------------   Alt-934   Alt-0170     MM   MM       misc/1   ------------------------
////      00AB  «  LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK          AltGr + [         Alt-174   Alt-0171     OAQ2 OQ       estr2    lex: escaped string-2 starting delimiter
////      00AC  ¬  NOT_SIGN                                           AltGr + \         Alt-170   Alt-0940     O2   OP       op/*     not verb ( ¬ ), not equal verb ( ¬=  or !=)
////      00AE  ®  REGISTERED_SIGN                                    AltGr + r         --------  Alt-0174     MM   MM       misc/1   ------------------------
////      00AF  ¯  MACRON                                             ---------------   --------  Alt-0175     ¯    AC      (acc/1)   ------------------------
////      00B0  °  DEGREE_SIGN                                        AltGr+Shift + :   Alt-504   Alt-0944     MM   MM       misc/1   ------------------------
////      00B1  ±  PLUS_MINUS_SIGN                                    ---------------   Alt-497   Alt-0177     O2   OP       op/*     ------------------------
////      00B2  ²  SUPERSCRIPT_TWO                                    AltGr + 2         Alt-509   Alt-0434     O2   OP       op/*     ------------------------
////      00B3  ³  SUPERSCRIPT_THREE                                  AltGr + 3         --------  Alt-0179     O2   OP       op/*     ------------------------
////      00B4  ´  ACUTE_ACCENT                                       AltGr + '         --------  Alt-05044    A2   AC      (acc/1)   lex: default escape character in «type 2 escaped strings» 
////      00B5  µ  MICRO_SIGN                                         AltGr + m         --------  Alt-0437     MM   MM       misc/1   ------------------------
////      00B6  ¶  PILCROW_SIGN                                       AltGr + ;         Alt-1044  Alt-0950     S2   SP       spch/1   ------------------------
////      00B7  ·  MIDDLE_DOT                                         ---------------   Alt-1530  Alt-0951     O2   OP       op/*     ------------------------
////      00B8  ¸  CEDILLA                                            ---------------   --------  Alt-0440     A2   AC      (acc/1)   --------------------------
////      00B9  ¹  SUPERSCRIPT_ONE                                    AltGr+Shift + !   --------  Alt-0441     O2   OP       op/*     --------------------------
////      00BA  º  MASCULINE_ORDINAL_INDICATOR                        ---------------   Alt-935   Alt-0954     MM   MM       misc/1   --------------------------
////      00BB  »  RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK         AltGr + [         Alt-175   Alt-0443     CAQ2 CQ       estr2    lex: escaped string-2 ending delimiter
////      00BC  ¼  VULGAR_FRACTION_ONE_QUARTER                        AltGr + 6         Alt-940   Alt-0444     MM   MM       misc/1   --------------------------
////      00BD  ½  VULGAR_FRACTION_ONE_HALF                           AltGr + 7         Alt-171   Alt-0445     MM   MM       misc/1   --------------------------
////      00BE  ¾  VULGAR_FRACTION_THREE_QUARTERS                     AltGr + 8                   Alt-0190     MM   MM       misc/1   --------------------------
////      00BF  ¿  INVERTED_QUESTION_MARK                             AltGr + /         Alt-1704  Alt-0191     P2   PU       punct/1  --------------------------
////      00D7  ×  MULTIPICATION_SIGN                                 AltGr + =         --------  Alt-0471     O2   OP       op/*     --------------------------
////      00F7  ÷  DIVISION_SIGN                                      AltGr+Shift + +   Alt-1014  Alt-0503     O2   OP       op/*     --------------------------
////                                                                                                                                  
//// 2000-206F ----------------------------------------------------------------------------------------------- S2   SP       spch/1   Except for the following special cases:      
////      2016  ‖  DOUBLE_VERTICAL_LINE                               --------------    --------  ---------    O2   OP       op/*     ----------------------- 
////      2018  ‘  LEFT_SINGLE_QUOTATION_MARK                         AltGr + 9         --------  Alt-0145     OQ1  OQ       ustr ??    ----------------------
////      2019  ’  RIGHT_SINGLE_QUOTATION_MARK                        AltGr + 0         --------  Alt-0914     CQ1  CQ       ustr ??    ----------------------
////      201A  ‚  SINGLE_LOW_9_QUOTATION_MARK                        ---------------   --------  ---------    OQ1  OQ       ustr ??    ----------------------
////      201B  ‛  SINGLE_HIGH_REVERSED_9_QUOTATION_MARK              ---------------   --------  ---------    CQ1  CQ       ustr ??    ----------------------
////      201C  “  LEFT_DOUBLE_QUOTATION_MARK                         ---------------   --------  Alt-0147     OQ2  OQ       estr1    ----------------------
////      201D  ”  RIGHT_DOUBLE_QUOTATION_MARK                        ---------------   --------  Alt-0404     CQ2  CQ       estr1    ----------------------
////      201E  „  DOUBLE_LOW_9_QUOTATION_MARK                        ---------------   --------  --------     OQ2  OQ       estr1    ----------------------
////      201F  ‟  DOUBLE_HIGH_REVERSED_9_QUOTATION_MARK              ---------------   --------  --------     CQ2  CQ       estr1    ----------------------
////      2020  †  DAGGER                                             ---------------   --------  Alt-0134     S2   SP       spch/1   ----------------------
////      2021  ‡  DOUBLE_DAGGER                                      ---------------   --------  Alt-0135     S2   SP       spch/1   ----------------------
////      2022  •  BULLET (not used -- see 2219)                      ---------------   Alt-7     Alt-0149     S2   SP       spch/1   (not used -- see 2219 -- bullet operator
////      2026  …  HORIZONTAL_ELLIPSIS                                ---------------   --------  Alt-0133     O2   OP       op/*     ----------------------
////      2039  ‹  SINGLE_LEFT_POINTING_ANGLE_QUOTATION_MARK          ---------------   --------  Alt-0139     OAQ1 OQ       ustr     ----------------------
////      203A  ›  SINGLE_RIGHT_POINTING_ANGLE_QUOTATION_MARK         ---------------   --------  Alt-0155     CAQ1 CQ       ustr     ----------------------
////      203C  ‼  DOUBLE_EXCLAMATION_MARK                            ---------------   Alt-19    --------     P2   PU       punct/1  ---------------------- 
////      203D  ‽  INTERROBANG                                        ---------------   --------  --------     P2   PU       punct/1  ---------------------- 
////      2045  ⁅  LEFT_SQUARE_BRACKET_WITH_QUILL                     ---------------   --------  --------     OQB  OB       o_qbk/1  ----------------------
////      2046  ⁆  RIGHT_SQUARE_BRACKET_WITH_QUILL                    ---------------   --------  --------     CQB  CB       c_qbk/1  ----------------------
////      2047  ⁇  DOUBLE_QUESTION_MARK                               ---------------   --------  --------     P2   PU       punct/1  ----------------------
////      2048  ⁈  QUESTION_EXCLAMATION_MARK                          ---------------   --------  --------     P2   PU       punct/1  ----------------------
////      2049  ⁉  EXCLAMATION_QUESTION_MARK                          ---------------   --------  --------     P2   PU       punct/1  ----------------------
////      204F  ⁏   REVERSED_SEMICOLON                                 ---------------   --------  --------     P2   PU       punct/1  ----------------------
////      2052  ⁒  COMMERCIAL_MINUS_SIGN                              ---------------   --------  --------     O2   OP       op/*     ----------------------
////      205D  ⁝   TRICOLON                                           ---------------   --------  --------     O2   OP       op/*     ----------------------
////      205E  ⁞  VERTICAL_FOUR_DOTS                                 ---------------   --------  --------     O2   OP       op/*     ----------------------
//// 2070-209F ----------------------------------------------------------------------------------------------- MM   MM       misc/1   --------------------------------------------------------------------------------------------  
//// 20A0-20CF ----------------------------------------------------------------------------------------------- $$   $$       id/*     can appear in extended identifiers  (currently disabled)
////                                                                                                                                  
//// 2100-21FF ----------------------------------------------------------------------------------------------- MM   MM       misc/1   Except for the following special cases:
////      2117  ℗  SOUND_RECORDING_COPYRIGHT                          ---------------   --------  ---------    S2   SP       spch/1   ------------------------
////      214B  ⅋  TURNED_AMPERSAND                                   ---------------   --------  --------     O2   OP       op/*     ------------------------
////      214C  ⅌  PER_SIGN                                           ---------------   --------  --------     O2   OP       op/*     ------------------------
////      2190  ←  LEFTWARDS_ARROW                                    ---------------   --------  --------     O2   OP       op/*     ------------------------
////      2190  ↑  UPWARDS_ARROW                                      ---------------   --------  --------     O2   OP       op/*     ------------------------
////      2192  →  RIGHTWARDS_ARROW                                   ---------------   --------  --------     O2   OP       op/*     ------------------------  
////      2190  ↓  DOWNWARDS_ARROW                                    ---------------   --------  --------     O2   OP       op/*     ------------------------
////      2194  ↔  LEFT_RIGHT_ARROW                                   ---------------   --------  --------     O2   OP       op/*     ------------------------
////      2195  ↕  UP_DOWN_ARROW                                      ---------------   --------  --------     O2   OP       op/*     ------------------------
////                                                                                                                                  
//// 2200-22FF ----------------------------------------------------------------------------------------------- O2   OP       op/*     Except for the following special cases:    
////      2219  ∙  BULLET_OPERATOR                                    ---------------   Alt-505   --------     O2   OP       op/*     ------------------------
////      2227  ∧  LOGICAL_AND                                        ---------------   --------  --------     O2   OP       op/*     and verb                             (also @AND ) 
////      2228  ∨  LOGICAL_OR                                         ---------------   --------  --------     O2   OP       op/*     or verb                              (also @OR  )             
////      2248  ≈  ALMOST_EQUAL_TO                                    ---------------   --------  Alt-503      O2   OP       op/*     ------------------------
////      2260  ≠  NOT_EQUAL_TO                                       ---------------   --------  -------      O2   OP       op/*     not-equal compare verb               (also  ¬=  @NE != )
////      2261  ≡  IDENTICAL_TO                                       ---------------   --------  Alt-5104     O2   OP       op/*     equal compare verb                   (also  ==  @EQ    )
////      2264  ≤  LESS_THAN_OR_EQUAL_TO                              ---------------   --------  Alt-499      O2   OP       op/*     less than or equal compare verb      (also  <=  @LE    )
////      2265  ≥  GREATER_THAN_OR_EQUAL_TO                           ---------------   --------  Alt-754      O2   OP       op/*     greater than or equal compare verb   (also  >=  @GE    ) 
////      22BB  ⊻  XOR                                                ---------------   --------  --------     O2   OP       op/*     exclusive or verb                    (also      @XOR   )
////                                                                                                                                  
//// 2300-27FF ----------------------------------------------------------------------------------------------- MM   MM       misc/1   Except for the following special cases:
////      2045  ⁅  LEFT_SQUARE_BRACKET_WITH_QUILL                     ---------------   --------  --------     OQB  OB       o_qbk/1  ------------------------
////      2046  ⁆  RIGHT_SQUARE_BRACKET_WITH_QUILL                    ---------------   --------  --------     CQB  CB       c_qbk/1  ------------------------
////      2310  ⌐  REVERSED_NOT_SIGN                                  ---------------   --------  Alt-937      O2   OP       op/*     ----------------------------------- 
////      2329  〈  LEFT_POINTING_ANGLE_BRACKET                        ---------------   --------  --------     OAB1  OB      o_abk/1  -------------------------
////      232A  〉  RIGHT_POINTING_ANGLE_BRACKET                       ---------------   --------  --------     CAB1  CB      c_abk/1  -------------------------
////      25BA  ►  BLACK_RIGHT_POINTING_POINTER                       ---------------   --------  Alt-1040     O2   OP       op/*     -------------------------                             
////      25C4  ◄  BLACK_LEFT_POINTING_POINTER                        ---------------   --------  Alt-17       O2   OP       op/*     -------------------------           
////      2768  ❨  MEDIUM_LEFT_PARENTHESIS_ORNAMENT                   ---------------   --------  --------     OP1  OB       o_par/1  -------------------------
////      2769  ❩  MEDIUM_RIGHT_PARENTHESIS_ORNAMENT                  ---------------   --------  --------     CP1  CB       c_par/1  -------------------------
////      276A  ❪  MEDIUM_FLATTENED_LEFT_PARENTHESIS_ORNAMENT         ---------------   --------  --------     OOB  OB       o_msb/1  -------------------------
////      276B  ❫  MEDIUM_FLATTENED_RIGHT_PARENTHESIS_ORNAMENT        ---------------   --------  --------     COB  CB       c_msb/1  -------------------------
////      276C  ❬  MEDIUM_LEFT_POINTING_ANGLE_BRACKET_ORNAMENT        ---------------   --------  --------     OAB1 OB       o_abk/1  -------------------------
////      276D  ❭  MEDIUM_RIGHT_POINTING_ANGLE_BRACKET_ORNAMENT       ---------------   --------  --------     CAB1 CB       c_abk/1  -------------------------
////      276E  ❮  HEAVY_LEFT_POINTING_ANGLE_QUOTATION_MARK_ORNAMENT  ---------------   --------  --------     OAB1 OB       o_abk/1  -------------------------
////      276F  ❯  HEAVY_RIGHT_POINTING_ANGLE_QUOTATION_MARK_ORNAMENT ---------------   --------  --------     CAB1 CB       c_abk/1  -------------------------
////      2770  ❰  HEAVY_LEFT_POINTING_ANGLE_BRACKET_ORNAMENT         ---------------   --------  --------     OAB1 OB       o_abk/1  -------------------------
////      2771  ❱  HEAVY_RIGHT_POINTING_ANGLE_BRACKET_ORNAMENT        ---------------   --------  --------     CAB1 CB       c_abk/1  -------------------------
////      2772  ❲  LIGHT_LEFT_TORTOISE_SHELL_BRACKET_ORNAMENT         ---------------   --------  --------     OSB1 OB       o_msb/1  -------------------------
////      2773  ❳  LIGHT_RIGHT_TORTOISE_SHELL_BRACKET_ORNAMENT        ---------------   --------  --------     CSB1 CB       c_msb/1  ------------------------- 
////      2774  ❴  MEDIUM_LEFT_CURLY_BRACKET_ORNAMENT                 ---------------   --------  --------     OC1  OB       o_brc/1  -------------------------
////      2775  ❵  MEDIUM_RIGHT_CURLY_BRACKET_ORNAMENT                ---------------   --------  --------     CC1  OB       c_brc/1  -------------------------
////      27C5  ⟅  LEFT_S_SHAPED_BAG_DELIMITER                        ---------------   --------  --------     OOB  OB       o_msb/1  -------------------------
////      27C6  ⟆  RIGHT_S_SHAPED_BAG_DELIMITER                       ---------------   --------  --------     COB  CB       c_msb/1  -------------------------
////      27E6  ⟦  MATHEMATICAL_LEFT_WHITE_SQUARE_BRACKET             ---------------   --------  --------     OK2  OB       o_msb/1  -------------------------
////      27E7  ⟧  MATHEMATICAL_RIGHT_WHITE_SQUARE_BRACKET            ---------------   --------  --------     CK2  CB       c_msb/1  -------------------------
////      27E8  ⟨  MATHEMATICAL_LEFT_ANGLE_BRACKET                    ---------------   --------  --------     OAB1 OB       o_abk/1  -------------------------
////      27E9  ⟩  MATHEMATICAL_RIGHT_ANGLE_BRACKET                   ---------------   --------  --------     CAB1 CB       c_abk/1  -------------------------
////      27EA  ⟪  MATHEMATICAL_LEFT_DOUBLE_ANGLE_BRACKET             ---------------   --------  --------     OAB2 OB       o_msb/1  -------------------------
////      27EB  ⟫  MATHEMATICAL_RIGHT_DOUBLE_ANGLE_BRACKET            ---------------   --------  --------     CAB2 CB       c_msb/1  -------------------------
////      27EC  ⟬  MATHEMATICAL_LEFT_WHITE_TORTOISE_SHELL_BRACKET     ---------------   --------  --------     OSB2 OB       o_msb/1  -------------------------
////      27ED  ⟭  MATHEMATICAL_RIGHT_WHITE_TORTOISE_SHELL_BRACKET    ---------------   --------  --------     CSB2 CB       c_msb/1  -------------------------
////      27EE  ⟮  MATHEMATICAL_LEFT_FLATTENED_PARENTHESIS            ---------------   --------  --------     OOB  OB       o_msb/1  -------------------------
////      27EF  ⟯  MATHEMATICAL_RIGHT_FLATTENED_PARENTHESIS           ---------------   --------  --------     COB  CB       c_msb/1  -------------------------

////                                                                                                                                  
//// 2900-29FF ----------------------------------------------------------------------------------------------- MM   MM       misc/1   -----------------------------------------------------------------------
////      2983  ⦃  LEFT_WHITE_CURLY_BRACKET                           ---------------   --------  --------     OC2  OB       o_msb/1   -----------------------------------------------------------------------
////      2984  ⦄  RIGHT_WHITE_CURLY_BRACKET                          ---------------   --------  --------     CC2  CB       c_msb/1   -----------------------------------------------------------------------
////      2985  ⦅  LEFT_WHITE_PARENTHESIS                             ---------------   --------  --------     OP2  OB       o_msb/1   -----------------------------------------------------------------------
////      2986  ⦆  RIGHT_WHITE_PARENTHESIS                            ---------------   --------  --------     CP2  CB       c_msb/1   -----------------------------------------------------------------------
////      2987  ⦇  Z_NOTATION_LEFT_IMAGE_BRACKET                      ---------------   --------  --------     OOB  OB       o_msb/1   -----------------------------------------------------------------------
////      2988  ⦈  Z_NOTATION_RIGHT_IMAGE_BRACKET                     ---------------   --------  --------     COB  CB       c_msb/1   -----------------------------------------------------------------------
////      2989  ⦉  Z_NOTATION_LEFT_BINDING_BRACKET                    ---------------   --------  --------     OOB  OB       o_msb/1   -----------------------------------------------------------------------
////      298A  ⦊  Z_NOTATION_RIGHT_BINDING_BRACKET                   ---------------   --------  --------     COB  CB       c_msb/1   -----------------------------------------------------------------------
////      298B  ⦋  LEFT_SQUARE_BRACKET_WITH_UNDERBAR                  ---------------   --------  --------     OOB  OB       o_msb/1   -----------------------------------------------------------------------
////      298C  ⦌  RIGHT_SQUARE_BRACKET_WITH_UNDERBAR                 ---------------   --------  --------     COB  CB       c_msb/1   -----------------------------------------------------------------------
////      298D  ⦍  LEFT_SQUARE_BRACKET_WITH_TICK_IN_TOP_CORNER        ---------------   --------  --------     OOB  OB       o_msb/1   -----------------------------------------------------------------------
////      298E  ⦎  RIGHT_SQUARE_BRACKET_WITH_TICK_IN_BOTTOM_CORNER    ---------------   --------  --------     COB  CB       c_msb/1   -----------------------------------------------------------------------
////      298F  ⦏  LEFT_SQUARE_BRACKET_WITH_TICK_IN_BOTTOM_CORNER     ---------------   --------  --------     OOB  OB       o_msb/1   -----------------------------------------------------------------------
////      2990  ⦐  RIGHT_SQUARE_BRACKET_WITH_TICK_IN_TOP_CORNER       ---------------   --------  --------     COB  CB       c_msb/1   -----------------------------------------------------------------------
////      2991  ⦑  LEFT_ANGLE_BRACKET_WITH_DOT                        ---------------   --------  --------     ODB  OB       o_dbk/1   -----------------------------------------------------------------------
////      2992  ⦒  RIGHT_ANGLE_BRACKET_WITH_DOT                       ---------------   --------  --------     CDB  CB       c_dbk/1   -----------------------------------------------------------------------
////      2993  ⦓  LEFT_ARC_LESS_THAN_BRACKET                        ---------------   --------  --------     OOB  OB       o_msb/1   -----------------------------------------------------------------------
////      2994  ⦔  RIGHT_ARC_GREATER_THAN_BRACKET                    ---------------   --------  --------     COB  CB       c_msb/1   -----------------------------------------------------------------------
////      2995  ⦕  DOUBLE_LEFT_ARC_GREATER_THAN_BRACKET              ---------------   --------  --------     OOB  OB       o_msb/1   -----------------------------------------------------------------------
////      2996  ⦖  DOUBLE_RIGHT_ARC_LESS_THAN_BRACKET                ---------------   --------  --------     COB  CB       c_msb/1   -----------------------------------------------------------------------
////      2997  ⦗  LEFT_BLACK_TORTOISE_SHELL_BRACKET                  ---------------   --------  --------     OSB1 OB       o_msb/1   -----------------------------------------------------------------------
////      2998  ⦘  RIGHT_BLACK_TORTOISE_SHELL_BRACKET                 ---------------   --------  --------     CSB1 CB       c_msb/1   -----------------------------------------------------------------------
////      29FC  ⧼  LEFT_POINTING_CURVED_ANGLE_BRACKET                 ---------------   --------  --------     OCB1 OB       o_msb/1   -----------------------------------------------------------------------
////      29FD  ⧽  RIGHT_POINTING_CURVED_ANGLE_BRACKET                ---------------   --------  --------     CCB1 CB       c_msb/1   -----------------------------------------------------------------------
////
//// 2A00-2AFF ----------------------------------------------------------------------------------------------- OP2  OP       op/*     -----------------------------------------------------------------------
////
//// 2B00-2BFF ----------------------------------------------------------------------------------------------- MM   MM       misc/1   -----------------------------------------------------------------------
////
//// 2E00-2E7F ----------------------------------------------------------------------------------------------- S2   SP       spch/1   -----------------------------------------------------------------------
////      2E18  ⸘  INVERTED_INTERROBANG                               ---------------   --------  --------     P2   PU       punct/1  ----------------------
////      2E22  ⸢  TOP_LEFT_HALF_BRACKET                              ---------------   --------  --------     OKB  OB       o_msb/1  ----------------------
////      2E23  ⸣  TOP_RIGHT_HALF_BRACKET                             ---------------   --------  --------     CKB  CB       c_msb/1  ----------------------
////      2E24  ⸤  BOTTOM_LEFT_HALF_BRACKET                           ---------------   --------  --------     OKB  OB       o_msb/1  ----------------------
////      2E25  ⸥  BOTTOM_RIGHT_HALF_BRACKET                          ---------------   --------  --------     CKB  CB       c_msb/1  ----------------------
////      2E26  ⸦  LEFT_SIDEWAYS_U_BRACKET                            ---------------   --------  --------    OOB  OB       o_msb/1    ---------------------
////      2E27  ⸧  RIGHT_SIDEWAYS_U_BRACKET                           ---------------   --------  --------    COB  CB       c_msb/1    ---------------------
////      2E28  ⸨  LEFT_DOUBLE_PARENTHESIS                            ---------------   --------  --------     OP2  OB       o_msb/1   ---------------------
////      2E29  ⸩  RIGHT_DOUBLE_PARENTHESIS                           ---------------   --------  --------     CP2  CB       c_msb/1   ---------------------
////      2E2E  ⸮  REVERSED_QUESTION_MARK                             ---------------   --------  --------     P2   PU       punct/1  ----------------------                                                                                                                                 
////                                                                                                                                  
////                                                                                                                                  
////                                                                                                                             
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  
////
////
////     Trigraph characters: 
////     -------------------
////          
////         ?????? =  ??       QUESTION_MARK                                  lex: default trigraph char    (must type ?????? to get ??)
////         ??<    =  «        LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK      lex: start of type-2 string
////         ??>    =  »        RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK     lex: end   of type-2 string
////         ??'    =  ´        ACUTE_ACCENT                                   lex: default type 2 string escape char
////         ??~    =  ¬        NOT_SIGN                                       used in ¬= operator, etc.   ?????????????????????????????    replace with != once pre-processor comments are in place
////                                                                      
////         ??4    =  ¼        VULGAR_FRACTION_ONE_QUARTER                    for use in strings     ????????????????????????????    replace with interpolation and pre-defined special char constants
////         ??2    =  ½        VULGAR_FRACTION_ONE_HALF                       for use in strings
////         ??3    =  ¾        VULGAR_FRACTION_THREE_QUARTERS                 for use in strings
////         ??c    =  ¢        CENT_SIGN                                      for use in strings      
////
////                                                                                                                                  
////
////     Special characters (recognized individually):
////     ---------------------------------------------  
////
////                                                              
////          QUESTION_MARK                                       ?        -- lex: default trigraph character
////          REVERSE_SOLIDUS                                     \        -- lex: default escape char in type-1 strings   -- "" strings
////          ACUTE_ACCENT                    ??'                 ´        -- lex: default escape char in type-2 strings   -- «» strings      ??????? this should become the grave accent
////          LEFT_PARENTHESIS                                    (        -- open  expression  paren  -- also   @(   and   :( 
////          RIGHT_PRENTHESIS                                    )        -- close expression  paren  -- also   )@   and   ):
////          LEFT_CURLY_BRACKET                                  {        -- open  block paren   
////          RIGHT_CURLY_BRACKET                                 }        -- close block paren 
////          LEFT_SQUARE_BRACKET                                 [        -- open  vlist paren   
////          RIGHT_SQUARE_BRACKET                                ]        -- close vlist paren   
////          SEMICOLON                                           ;        -- expression separator in block
////                                                      
////
////
////
////   pre-processor sigils:
////   --------------------
////          
////          QUESTION_MARK                                      ?        -- leading  sigil -- pre-process action token                            ?VERB            ????? these need to go awat ????????                         
////          QUESTION_MARK                                       ?       -- trailing sigil -- pre-process substitute variable                     ?variable?       ????? these need to go awat ????????         
////          EXCLAMATION_MARK                                    !       -- trailing sigil -- pre-process quoted substitute variable              ?variable!       ????? these need to go awat ????????                                          
////          COLON                                               :       -- trailing sigil -- pre-process label                                   ?label:          ????? these need to go awat ????????           
////                             
////          
////    parser sigils:
////    -------------
////          
////          COMMERCIAL_AT                                      @        -- leading  sigil -- verb marker (on identifiers and open parenthesis)    @VERB   -or-    @( 'string )@
////          COMMERCIAL_AT                                       @       -- trailing sigil -- verb marker (on close parenthesis)                   )@
////          DOLLAR_SIGN                                        $        -- leading  sigil -- parm/operand marker (on verbnames)                   $++++
////          COLON                                               :       -- trailing sigil -- keyword marker                                       kwname: -or-    :( 'keyword_name ):
////          COLON                                              :        -- leading  sigil -- @GOTO label marker                                   :label          :(
////
////
////
////    Operator characters:
////    ------------------- 
////
////         entered directly by keyboard --  
////                                                     !   = 0021
////                                                     #   = 0023
////                                                     %   = 0025
////                                                     &   = 0026
////                                                     *   = 002A
////                                                     +   = 002B
////                                                     ,   = 002C    ( , ,, ,,, ,,,, etc., only )  (uniform operator name)
////                                                     -   = 002D
////                                                     .   = 002E    ( . .. ... .... etc., only )  (uniform operator name)
////                                                     /   = 002F
////                                                     :   = 003A    ( : :: ::: :::: etc., only )  (uniform operator name)
////                                                     <   = 003C
////                                                     =   = 003D
////                                                     >   = 003E
////                                                     ?   = 003F
////                                                     @   = 0040
////                                                     ^   = 005E
////                                                     |   = 007C
////                                                     ~   = 007F
////
////         entered by code value --                  
////                                                     ¦   = 00A6  Alt-0934              
////                                                     ¬   = 00AC  Alt-170   AltGr + \   ( @NOT )     ??????????? needs to be replaced with  !   and   !=
////                                                     °   = 00B0  AltGr+Shift + :       
////                                                     ±   = 00B1  Alt-0177
////                                                     ²   = 00B2
////                                                     ³   = 00B3
////                                                     ·   = 00B7  Alt-0951
////                                                     ¹   = 00B9         
////                                                     ×   = 00D7  Alt-0471              
////                                                     ÷   = 00F7  Alt-0503              
////                                                     ⁒   = 2052                        
////                                                     ⁝    = 205D                   
////                                                     ⁞   = 205E                   
////                                                     ⅋   = 214B                   
////                                                     ⅌   = 214C 
////                                                     ←   = 2190
////                                                     →   = 2192
////                                                     ↔   = 2194
////                                                     ∙   = 2219  Alt-505          
////                                                     ∨   = 2228                         
////                                                     ∧   = 2227                          
////          (all 22xx are operators)                   ≠   = 2260                        
////          (selected ones shown here)                 ≡   = 2261                        
////                                                     ≤   = 2264  Alt-499               
////                                                     ≥   = 2265  Alt-754               
////                                                     ⊻   = 22BB                         
////                                                     ⋀   = 22C0                        
////                                                     ⋁   = 22C1                        
////                                                     ⌐   = 2310  Alt-937          
////                                                     ►   = 25BA                        
////                                                     ◄   = 25C4                        
////
////
////
////
////
////   Tokens:  
////   -------
////
////
////       String literals:
////                                'xx                            -- lex: word string -- no escape processing , trigraphs processed
////                                "xx"                           -- lex: string with escape - 1 (\ = default), trigraphs processed
////                                «xx»                           -- lex: string with escape - 2 (´ = default), trigraphs processed
////                               M"xx"                           -- lex: string with escape - 1 (\ = default), trigraphs processed  -- string can span multiple lines
////                               M«xx»                           -- lex: string with escape - 2 (´ = default), trigraphs processed  -- string can span multiple lines
////                               R"xx"                           -- lex: no escape processing,                 trigraphs not processed
////                               R«xx»                           -- lex: no escape processing,                 trigraphs not processed
//// 
////
////       Comments:                
////                                // xxx                         -- lex: line comment -- don't suppress EOL -- don't echo  -- trigraphs not processed
////                                /^ xxx                         -- lex: line comment --       suppress EOL -- don't echo  -- trigraphs not processed
////                                /~ xxx                         -- lex: comment to EOF                     -- don't echo  -- trigraphs not processed
////                                                            
////                                /* xxx */                      -- lex: block comment -- not nestable      -- don't echo  -- trigraphs not processed
////                                /< xxx /< xxx >/ >/            -- lex: block comment -- nestable          -- don't echo  -- trigraphs not processed
////                                /#                             -- lex: retained comment to EOL            -- passed up to parser -- trigraphs processed 
////                                /{  /{  }/  }/                 -- lex: retained block comment (nestable)  -- passed up to parser -- trigraphs processed 
////
////
////  lexer output tokens
////  -------------------
////
////  @ -- any character, other than string ending or string escape character 
////  1 -- decimal numeric digit
////  A -- alpha character
////  B -- number basing char -- b B o O x X 
////  X -- valid based digit    0-1 for   0b or 0B,       0-7 for   0o or 0O,        0-9 or A-F or a-f for    0x or 0X   
////  _ -- separator character
////  $ -- currency character
////  ` -- accent mark character
////  * -- operator character
////  « -- start escaped string char
////  » -- end escaped string char
////  \ -- escape character
////  ' -- start simple string char
////  " -- end simple string character 
////  + -- plus  (when attachable to decimal integer)
////  - -- minus (when attachable to decimal integer)
////  . -- decimal point 
////
////
////
////
////
////
//// [vvv] -- optional 0-N occurrences
//// <vvv> -- optional 0-1 occurrences
//// {   } -- required 1 occurrence
////
////
////
////
////
////    identifier                      --    <leading_sigil> {_|A} [_|A|1] <trailing_sigil>
////
////    extended_identifier             --    <leading_sigil> {_|A|$|`} [_|A|$|`|1] <trailing_sigil>  -- not supported by default
////
////    operator                        --    {*} [*] 
////
////    signed decimal integer          --    {+|-|¯} {1} [1|_] <S|s>  
////    unsigned decimal integer        --    {1} [1|_] {U|u}
////
////
////    based integer
////
////    floating point                  --    <+|-|¯>  {.} {1} [1] < {E|e} <+|-> {1}[1]>                      -- decimal point -- optional e-nnn
////                                    --    <+|-|¯>  {1} [1] {.} [1] < {E|e} <+|-> {1}[1]>                  -- decimal point -- optional E-nnn
////                                    --    <+|-|¯>  {1} [1] {E|e} <+|-> {1}[1]                             -- no decimal point, but E+nnn required
////
////    simple string
////
////    escaped string
////
////
////
//// 
////
////   
//// 
////
////
//// preprocess verbs:
//// ----------------  
////
////   ?ERROR
////   ?ABORT 
////   ?EXIT
////   ?END
////   ?END_DEF                identifier 
////   ?END_UNDEF              identifier
////   ?SAY
////   ?SAY_SKIP
////   ?SAY_ALWAYS
////   ?DISPLAY_ALL_VARS
////   ?SET                    identifier   string
////   ?VAR                    identifier   string 
////   ?CONST                  identifier   string
////   ?UNSET                  identifier
////   ?IMBED                  string 
////   ?SKIPTO                 identifier
////   ?SKIPTO_DEF             identifier   identifier
////   ?SKIPTO_UNDEF           identifier   identifier
////   ?SKIPTO_EQ              identifier   identifier    identifier
////   ?SKIPTO_NE              identifier   identifier    identifier
////
////
////
////   parser verbs:
////   ------------
////
////   @DISPLAY_VERBS
////   @BLOCK                  @BLOCK {block} left:[vlist] right:[vlist]
////   @ARG                    
////   @AT °                   [vlist] @AT int key:"string"  
////   @DO                     @DO {block};
////   @GOTO                   @GOTO ????
////   @IF                     @IF (expression)         then:{block} else:{block};
////   @CASE                   @CASE when:(expression)  {block}   when:(expression) {block}   ...   when:(expression) {block}   else:{block};   
////   @LOOP                   @LOOP while:(expression) {block};
////   @SAY                    @SAY simple-values;
////   @STR                    @STR simple-values;
////   @VAR                    identifier @VAR   value:val;
////   @CONST                  identifier @CONST value:val;
////   >>>   ►                 value         >>>    identifier; 
////   <<< = ◄                 identifier    =      value;
////   ++ +¹                   identifier ++   or   ++ identifier 
////   -- -¹                   identifier --   or   -- identifier 
////
////   +=                      identifier += arith_type
////   -=                      identifier -= arith_type
////   *= ×=                   identifier *= arith_type
////   /= ÷=                   identifier /= arith_type
////   ^=                      identifier ^= arith_type
////   ⁒=                      identifier ⁒= arith_type  
////
////   +                       arith_type  +  arith_type 
////   -                       arith_type  -  arith_type 
////   *  ×                    arith_type  *  arith_type 
////   /  ÷                    arith_type  /  arith_type 
////   ^                       arith_type  ^  arith type 
////   ⁒  @REM                 arith_type  ⁒  arith_type 
////
////   ,  @SEP                 any-type ...  ,  any-type ...
////
////   |                       string | string
////   ==  ≡                   value == value
////   <                       value < value
////   >                       value > value
////   <=  ≤                   value <= value
////   >=  ≥                   value >= value
////   ¬=  ≠                   value ¬= value
////       
////   ∧  @AND                value ∧ value
////   ∨  @OR                 value ∨ value
////   ⊻  @XOR                value ⊻ value
////   ¬  @NOT                ¬ value   
////
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////