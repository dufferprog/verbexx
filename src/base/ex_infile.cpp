// ex_infile.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     ex_infile.cpp -- infile_C functions -and- instring_C functions
////     =============
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///     IIIII      NN    N      FFFFFF     IIIII     L         EEEEE              
///       I        N N   N      F            I       L         E              
///       I        N  N  N      FFFFF        I       L         EEEEE          
///       I        N   N N      F            I       L         E       
///     IIIII      N    NN      F          IIIII     LLLLLL    EEEEE          
///
///
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// default constructor (defaulted)
// ------------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
infile_C::infile_C()      
    : m_in_file_p      {nullptr}         
    , m_in_filename    {}
    , m_is_open_f      {false}
    , m_is_utf8        {false}
    , m_line_ok        {false}
    , m_eof_seen       {false}
    , m_error_seen     {false}
    , m_curr_line      {}
    , m_curr_wline     {}
    , m_curr_linenum   {0} 
    , m_curr_col       {0}
    , m_curr_pos       {0}
    , m_end_pos        {0}
{
    M_dmsg1("infile_C() called -- default constructor (own=%d) -- \"", (int)is_owner) M_dwide(m_in_filename) M_dmsg2("\"")      
    return;
}
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// string constructor
// ------------------ 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

infile_C::infile_C(const std::string& str) try    
{
    M__(M_out(L"infile_C(%s) called -- string constructor -- m_in_filename = \"%s\"") % to_wstring(str) % m_in_filename;)
    open(str);
    return;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// wstring constructor
// ------------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

infile_C::infile_C(const std::wstring& wstr)  try    
{
    M__(M_out(L"infile_C(\"%s\") called -- wstring constructor -- m_in_filename =  \"%s\"") % wstr % m_in_filename;) 
    open(wstr);
    return;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// copy constructor (deleted)
// ---------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
infile_C::infile_C(const infile_C& from)
    : m_in_file_p      { from.m_in_file_p      }
    , m_in_filename    { from.m_in_filename    }
    , m_is_open_f      { from.m_is_open_f      }
    , m_is_utf8        { from.m_is_utf8        }
    , m_line_ok        { from.m_line_ok        }
    , m_eof_seen       { from.m_eof_seen       }
    , m_error_seen     { from.m_error_seen     }
    , m_curr_line      { from.m_curr_line      }
    , m_curr_wline     { from.m_curr_wline     }
    , m_curr_linenum   { from.m_curr_linenum   }
    , m_curr_col       { from.m_curr_col       }
    , m_curr_pos       { from.m_curr_pos       }
    , m_end_pos        { from.m_end_pos        }
{
    M_msg1("infile_C(const infile_C&) called -- copy constructor (own=%d) -- \"", (int)is_owner) M_wide(m_in_filename) M_msg2("\"")  
    return;
}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// move constructor (deleted)
// ---------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
infile_C::infile_C(infile_C&& from)
    : m_in_file_p      { from.m_in_file_p              }
    , m_in_filename    { std::move(from.m_in_filename) }
    , m_is_open_f      { from.m_is_open_f              }
    , m_is_utf8        { from.m_is_utf8                }
    , m_line_ok        { from.m_line_ok                }
    , m_eof_seen       { from.m_eof_seen               }
    , m_error_seen     { from.m_error_seen             }
    , m_curr_line      { std::move(from.m_curr_line)   }
    , m_curr_wline     { std::move(from.m_curr_wline)  }
    , m_curr_linenum   { from.m_curr_linenum           }
    , m_curr_col       { from.m_curr_col               }
    , m_curr_pos       { from.m_curr_pos               }
    , m_end_pos        { from.m_end_pos                }
{
    M_msg1("infile_C(infile_C&&) called -- move constructor (own=%d) -- \"", (int)is_owner) M_wide(m_in_filename) M_msg2("\"")      
    from.is_owner = false;    // don't want to close the file in destructor   
    return; 
}
 */


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// destructor
// ----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

infile_C::~infile_C()       // destructor -- can't throw exceptions
{
    M__(M_out(L"~infile_C() called -- destructor -- m_in_filename = \"%s\"") % m_in_filename;)  

    close();                // make sure file is closed
    return; 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// copy assignment (self-assignment should be OK -- orig file gets closed, copy does not own the file (deleted)
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
infile_C& infile_C::operator=(const infile_C& from)
{
    M_msg1("operator=(const infile_C&) called -- copy_assignment (own=%d) -- \"", (int)is_owner) M_wide(from.m_in_filename) M_msg1("\"->\"") M_wide(m_in_filename) M_msg2("\"")
   
    infile_C temp {from};        // constructed copy of from -- from still owns the file (if any)
    std::swap(temp, *this);      // move copy of from into *this -- temp now has old file from *this -- may get closed when temp goes out of scope  
    return *this;                // *this is a copy of from -- from is still owner of file (if any)
}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// move assignment (deleted)
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
infile_C& infile_C::operator=(infile_C&& from)
{
    M_msg1("operator=(infile_C&&) called -- move_assignment (own=%d) -- \"", (int)is_owner) M_wide(from.m_in_filename) M_msg1("\"->\"") M_wide(m_in_filename) M_msg2("\"")
      
    std::swap(m_in_file_p    , from.m_in_file_p    );
    std::swap(m_in_filename  , from.m_in_filename  );
    std::swap(m_is_open_f    , from.m_is_open_f    );
    std::swap(m_is_utf8      , from.m_is_utf8      );
    std::swap(m_line_ok      , from.m_line_ok      );
    std::swap(m_eof_seen     , from.m_eof_seen     );
    std::swap(m_error_seen   , from.m_error_seen   );
    std::swap(m_curr_line    , from.m_curr_line    );
    std::swap(m_curr_wline   , from.m_curr_wline   );
    std::swap(m_curr_linenum , from.m_curr_linenum );
    std::swap(m_curr_col     , from.m_curr_col     );
    std::swap(m_curr_pos     , from.m_curr_pos     );
    std::swap(m_end_pos      , from.m_end_pos      ); 

    return *this;                
}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// open() file
// -----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//  "plain" string version 
//  ----------------------

int infile_C::open(const std::string& str) try
{
    M__(M_out(L"infile-C::open(%s) called -- m_in_filename = \"%s\"") % to_wstring(str) % m_in_filename;)  
 
    close();       // make sure any currently opened file is closed 


    // initialize flags and char position in new file (file name will be set below)

    m_is_utf8        = false;
    m_error_seen     = false;  
    m_eof_seen       = false; 
    m_curr_linenum   = 0; 
    m_curr_col       = 0; 
    m_curr_pos       = 0; 
    m_end_pos        = 0;  
    
    m_line_ok        = false;
    m_curr_line      = std::string{};
    m_curr_wline     = std::wstring{};


    // open() the file and save away file info  

    m_in_file_p = new std::ifstream(str); 
   
    if (!(*m_in_file_p).good())
    {  
        M_out_emsg(L"infile_C::open() -- Input file \"%s\" cannot be opened") % to_wstring(str);
        delete m_in_file_p; 
        m_in_file_p   = nullptr;
        m_error_seen  = true; 
        m_in_filename = std::wstring{}; 
        m_source_id   = ::add_cached_id(m_in_filename);
        return -1; 
    }
    else
    {
        m_in_filename = ::to_wstring(str); 
        m_is_open_f   = true;
    }

    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////

//  wstring version 
//  ---------------

int infile_C::open(const std::wstring& wstr) try
{
     M__(M_out(L"infile-C::open(%s) called -- m_in_filename = \"%s\"") % wstr % m_in_filename;)  

    close();       // make sure any currently opened file is closed 


    // initialize flags and char position in new file (file name will be set below)

    m_is_utf8        = false;
    m_error_seen     = false;  
    m_eof_seen       = false; 
    m_curr_linenum   = 0; 
    m_curr_col       = 0; 
    m_curr_pos       = 0; 
    m_end_pos        = 0;  
    
    m_line_ok        = false;
    m_curr_line      = std::string{};
    m_curr_wline     = std::wstring{};


    // open() the file and save away file info  

    m_in_file_p = new std::ifstream(wstr); 
   
    if (!(*m_in_file_p).good())
    {  
        M_out_emsg(L"infile_C::open() -- Input file \"%s\" cannot be opened") % M_ws(wstr);
        delete m_in_file_p; 
        m_in_file_p   = nullptr;
        m_error_seen  = true; 
        m_in_filename = std::wstring{}; 
        return -1; 
    }
    else
    {
        m_in_filename = wstr; 
        m_source_id   = ::add_cached_id(m_in_filename);
        m_is_open_f   = true;
    }

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    close() file 
//    ------------
//
//   -- leaves old filename, flage, position, line buffers, etc. from last read()
//  
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int infile_C::close() try
{
    M__(M_out(L"infile-C::close() called -- m_in_filename = \"%s\"") % m_in_filename;)

    if (m_is_open_f && (m_in_file_p != nullptr))
    {
        M__(M_out(L"infile_C::close() -- close()ing -- m_in_filename = \"%s\"") % m_in_filename;)

        m_in_file_p->close();
        m_is_open_f = false;       
    }

    if (m_in_file_p != nullptr) 
    {       
        delete m_in_file_p; 
        m_in_file_p = nullptr;       
    }
   
    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// is_open()
// ---------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool infile_C::is_open() const try
{
   return m_is_open_f;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// get_filename()
// --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring infile_C::get_filename() const try
{
    M__(M_out(L"get_filename(): m_in_filename=\"") % m_in_filename;) 
    return m_in_filename;
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// get_source_id()
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint64_t infile_C::get_source_id() const try
{
    return m_source_id; 
}
M_endf








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  fetch_char()  -- internal (file assumed to be open, and line_OK is true   -or-   m_error_seen/m_eof_seen is true)
//  ------------ 
//
//  sets line_OK to false if last character fetched
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int infile_C::fetch_char(in_char_S & in_char, bool advance) try
{
    M__(M_out(L"infile_C::fetch_char() -- called");)

    // return back existing data, if any -- don't go out to file (even if m_error_seen or m_eof_seen flags are on -- need to pass back old characters first)
        
    if (m_line_ok)    
    {
         if (m_curr_pos < m_end_pos)
         {
             out_char(in_char, char_E::normal, advance);             
             return 0;         
         }          
         else     // sitting past end of data, but haven't passed back EOL yet (EOF may have been seen, too -- will be passed back next time)
         {
             // Pass back end of line, now, and mark line as not OK
         
             out_char(in_char, char_E::eol, advance);
             if (advance)                                    // update state only if we're in advance mode 
                 m_line_ok = false;                          // will need to read in more next time              
             return 0; 
         }    
    }


    // Just pass back error, if prior read has returned fail(), and no data in line buffer 

    if (m_error_seen)
    {
        out_char(in_char, char_E::error, advance); 
        return -1;     
    }
    

    // Just pass back EOF, if prior read has returned EOF, and no data in line buffer 

    if (m_eof_seen)
    {
        out_char(in_char, char_E::eof, advance); 
        return 0;     
    }
    
    M_out_emsg(L"infile_C::fetch_char() called when no valid char could be fetched/returned -- file =\"%s\"") %  m_in_filename;
    out_char(in_char, char_E:: error, advance);  
    return -1;                                       // should not happen 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  
//  out_char() -- assume current line is OK and not at very end, when passed-in classs = "normal"
//  ---------
//
//      -- does not set proper type, subtype, family -- sets classs field only
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void infile_C::out_char(in_char_S & in_char, char_E classs, bool advance) try
{
    M__(M_out(L"infile_C::out_char()-- called : m_in_filename=\"%s\"")  % m_in_filename;)  

    in_char.source_id = m_source_id;                                           // capture current source ID
    in_char.lineno    = m_curr_linenum;                                        // capture line number (already 1-based)
    in_char.linepos   = m_curr_col + 1;                                        // capture column (1-based)
    in_char.classs    = classs;                                              
    in_char.family    = char_E::none;                                          // initilize to "none" -- will get filled in later
    in_char.type      = char_E::none;                                        
    in_char.subtype   = char_E::none;                                        
                                                                             
    size_t saved_pos = m_curr_pos;                                             // save away initial position for later rollback, in case advance parm = false
    size_t saved_col = m_curr_col;          

    switch (classs)
    {
    // normal characters -- fetch from line and convert to UTF-32, etc.

    case char_E::normal :

        // put in 1st or only wide char

        in_char.wch1 = m_curr_wline[m_curr_pos];
        ++m_curr_pos;                                           


        // add in trailing surrogate, if required  (note -- this assumes leading and trailing surrogate characters are on same line)
        // -------------------------------------------------------------------------------------------------------------------------

        if ( ((uint32_t)(in_char.wch1) & const_N::utf16_surrogate_mask) == const_N::utf16_leading_base)   // 1st char is a lead surrogate ?
        {
            if ( 
                (m_curr_pos < m_end_pos)
                &&
                (((uint32_t)(m_curr_wline[m_curr_pos]) & const_N::utf16_surrogate_mask) == const_N::utf16_trailing_base)
               )
            {
                 in_char.wch2 = m_curr_wline[m_curr_pos];      // put in trail surrogate
                 ++m_curr_pos;                
            }
            else    // trailing surrogate not present -- just set to 0's -- should not occur? 
            {
                M_out_note(L"infile_C::out_char() : Leading surrogate found, but not followed immediately by trailing surrogate -- file=\"%s\" line/col = %u/%llu -- \"") % m_in_filename % m_curr_linenum % m_curr_col;                
                in_char.wch2 = (wchar_t)0;                 
            }
        }
        else  // not a leading surrogate -- just put 0's in 2nd widechar 
        {
            in_char.wch2 = (wchar_t)0;     
        }           
           

        // get UTF-32 representation for the wide char(s)

        to_char32(in_char.wch1, in_char.wch2, in_char.ch32); 
        ++m_curr_col; 
        break;


    // EOL -- put in unicode NEL character

    case char_E::eol   :

        in_char.wch1  = (wchar_t)0x0085;
        in_char.wch2  = (wchar_t)0;
        in_char.ch32  = (char32_t)0x00000085;
        break;


    // special cases -- code points are all 0 -- only type is special

    case char_E::error :
    case char_E::eof   :
        in_char.wch1  = (wchar_t)0;
        in_char.wch2  = (wchar_t)0;
        in_char.ch32  = (char32_t)0;      
    }


    // undo any position advance, if the advance option is false 

    if (!advance)
    {
        m_curr_col = saved_col;
        m_curr_pos = saved_pos;
    }
     
    M__(M_out(L"infile_C::out_char().2 : m_in_filename=\"%s\"") : m_in_filename;) 
    return;    
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  
//  get_char() 
//  ---------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int infile_C::get_char(in_char_S& in_char, bool advance) try 
{
    M__(M_out(L"infile_C::get_char() -- called");)

    // pass back existing data, or preset error EOF indication, if any -- don't go out to file (which may be closed if eof/error seen)

    if ( m_line_ok || m_eof_seen || m_error_seen )    
        return fetch_char(in_char, advance);    


    //  Need to go out to file to refill the line buffer -- error if file is not open
    //  -----------------------------------------------------------------------------
    
    if (!(m_is_open_f))
    {
        M_out_emsg(L"infile_C::get_char() called when file is not open  -- \"%s\"") %  m_in_filename;
        out_char(in_char, char_E::error, advance);         
        return -1; 
    }       


    //  Read in next line of file -- handle EOF, I/O error -- maintain states, etc.
    //  ---------------------------------------------------------------------------

    m_curr_line  = std::string{}; 
    m_curr_wline = std::wstring{};
    m_end_pos    = 0; 

    std::getline(*m_in_file_p, m_curr_line); 

    if (m_in_file_p->bad())
    {
        M_out_emsg(L"infile_C::get_char(): bad() on after doing getline(), file -- \"%s\"") %  m_in_filename;
        m_error_seen = true;          // fetch_char() will return the error down below 
        close();                      // make sure no more I/O on this file
    }
    else if ((m_in_file_p->eof()) && m_curr_line.length() == 0)
    {
        m_eof_seen = true;            // fetch_char() will return the EOF down below 
        close();                      // make sure no more I/O on this file
    }
    else if (m_in_file_p->fail())
    {
        M_out_emsg(L"infile_C::get_char(): fail() bit is on after doing getline(), before EOF was reached -- file = \"%s\"") %  m_in_filename;
        m_error_seen = true;          // fetch_char() will return the error down below 
        close();                      // make sure no more I/O on this file       
    }
    

    // read() did not fail -- line is good, but may be empty, etc. -- may have reached EOF
    
    if ( m_eof_seen || m_error_seen )
        m_line_ok  = false;                
    else
        m_line_ok  = true;                // line might be empty


    //  If this is first line (and no error so far), -- skip past any UTF-8 flag bytes
    //  ------------------------------------------------------------------------------

    if ( (m_curr_linenum == 0) && m_line_ok )    
    {
        M__(M_out(L"0=%02X 1=%02X 2=%02X") % (uint32_t)(uint8_t)(m_curr_line[0]) % (uint32_t)(uint8_t)(m_curr_line[1]) % (uint32_t)(uint8_t)(m_curr_line[2]);)

        if ( 
            (m_curr_line.length() >= 3)
            && 
            ((uint8_t)(m_curr_line[0]) == 0xEFU)
            && 
            ((uint8_t)(m_curr_line[1]) == 0xBBU)
            && 
            ((uint8_t)(m_curr_line[2]) == 0xBFU)
            )
        {
            m_is_utf8 = true; 
            m_curr_line = m_curr_line.substr(3);        
        }
        else
        {
            m_is_utf8 = false; 
            M__(M_out_note(L"infile_C::get_char() -- UTF-8 BOM not seen at start of file -- plain 7-bit ASCII assumed (not code page 1252) -- \"%s\"") %  m_in_filename;) 
        }
    }


    //  ------------------------------------------------------------------------------
    //  Convert just-read string from UTF-8 (or codepage 1252(??)) to wchar_t (UTF-16) -- catch(...) any conversion failures and flag whole file in error
    //  ------------------------------------------------------------------------------

    if (m_line_ok)                 // bypass, if no valid data in line buffer 
    {
        if (m_is_utf8)
        {
            try       // do UTF-8 to UTF-16 conversion 
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > conv_utf8_utf16; 
                m_curr_wline = conv_utf8_utf16.from_bytes(m_curr_line);
                m_end_pos = m_curr_wline.length();                           // should be one past end
                ++m_curr_linenum;                                            // increment line number in file 
                m_curr_col = 0;
                m_curr_pos = 0; 
            }
            catch(...)
            {
                M_out_emsg(L"infile_C::get_char(): conv_utf8_utf16 failed, file name = \"%s\"  line=%u") %  m_in_filename % (m_curr_linenum + 1); 
                m_error_seen = true;                                       // fetch_char() will return the error down below 
                m_line_ok    = false;                                      // line contents are undefined now
                close();                                                   // make sure no more I/O on this file       
            }
        }
        else       
        {
            try       // do codepage 1252 (??) to UTF-16 conversion
            {
                m_curr_wline = to_wstring(m_curr_line);                      // use CP_ACP by defaulted 2nd parm -- CP_ACP = default system code page = 1252 (??)
                m_end_pos = m_curr_wline.length();                           // should be one past end
                ++m_curr_linenum;                                            // increment line number in file 
                m_curr_col = 0;
                m_curr_pos = 0; 
            }
            catch(...)
            {
                M_out_emsg(L"infile_C::get_char(): to_wstring() failed, file name = \"%s\"  line=%u") %  m_in_filename % (m_curr_linenum + 1); 
                m_error_seen = true;                                       // fetch_char() will return the error down below 
                m_line_ok    = false;                                      // line contents are undefined now
                close();                                                   // make sure no more I/O on this file       
            }
        }
    }


    //  At this point, line_OK is on --or-- m_eof_seen/m_error_seen might be on (line can be empty or in bad state in this case) 

    return fetch_char(in_char, advance);
}
M_endf






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///     IIIII      N   N       SSSS     TTTTT     RRRR      IIIII     N   N      GGGG           
///       I        NN  N      S           T       R   R       I       NN  N     G           
///       I        N N N       SSS        T       RRRR        I       N N N     G  GG    
///       I        N  NN          S       T       R  R        I       N  NN     G   G
///     IIIII      N   N      SSSS        T       R   R     IIIII     N   N      GGGG   
///
///
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// default constructor (defaulted)
// ------------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// string constructor 
// ------------------ 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// version with wstring name and input wstring

instring_C::instring_C(const std::wstring& str, const std::wstring& name) try    
{
    M__(M_out(L"instring_C(std::wstring, std::string, \"%s\") called -- string constructor") % name;)
    open(str, name);
    return;
}
M_endf


// version with wstring name

instring_C::instring_C(const std::string& str, const std::wstring& name) try    
{
    M__(M_out(L"instring_C(std::string, std::string, \"%s\") called -- string constructor") % name;)
    open(str, name);
    return;
}
M_endf


// version with string name

instring_C::instring_C(const std::string& str, const std::string& name) try    
{
    M__(M_out(L"instring_C(std::string, std::string, \"%s\") called -- string constructor") % to_wstring(name);)
    open(str, name);
    return;
}
M_endf





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// copy constructor (deleted)
// ---------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// move constructor (deleted)
// ---------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// destructor
// ----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

instring_C::~instring_C()     // destructor -- can't throw
{
    M__(M_out(L"~instring_C() called -- destructor -- m_in_name=\"%s\"") % m_in_name;)  

    close();   // make sure string is closed (empty)
    return; 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// copy assignment (self-assignment should be OK -- orig file gets closed, copy does not own the file (deleted)
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// move assignment (deleted)
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// open() string
// -------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// wstring name and data version 
// -----------------------------

int instring_C::open(const std::wstring& str, const std::wstring& name) try
{
    M__(M_out(L"instring_C::open(std::wstring, std::string, \"%s\") called") % name;)  

   // make sure instring_C is ready for reuse 

    close();       // make sure any current string data is cleared out, etc. 
    reset();       // reset to starting position, etc.


    // copy in the string data and save away string-related info

    m_in_string.str(str);             // store caller's string in stringstream
    if (!(m_in_string).good())
    {  
        M_out_emsg(L"instring_C::open() -- Input string \"%s\" cannot be set") % name;
        m_error_seen  = true; 
        return -1; 
    }
    else
    {
        m_in_name     = name; 
        m_source_id   = ::add_cached_id(m_in_name);
        m_is_open_s   = true;
        M__(M_out(L"m_in_name = \"%s\"") % m_in_name;)  
    }

    return 0; 
}
M_endf



// wstring name version 
// --------------------

int instring_C::open(const std::string& str, const std::wstring& name) try
{
    M__(M_out(L"instring_C::open(std::string, std::string, \"%s\") called") % name;)  

   // make sure instring_C is ready for reuse 

    close();       // make sure any current string data is cleared out, etc. 
    reset();       // reset to starting position, etc.


    // copy in the string data and save away string-related info

    m_in_string.str(::to_wstring(str));      // store caller's string in stringstream -- assumed to be CP 1252 
    if (!(m_in_string).good())
    {  
        M_out_emsg(L"instring_C::open() -- Input string \"%s\" cannot be set") % name;
        m_error_seen  = true; 
        return -1; 
    }
    else
    {
        m_in_name     = name;
        m_source_id   = ::add_cached_id(m_in_name);
        m_is_open_s   = true;
        M__(M_out(L"m_in_name = \"%s\"") % m_in_name;)  
    }

    return 0; 
}
M_endf


// string name and data version 
// ----------------------------

int instring_C::open(const std::string& str, const std::string& name) try
{
    M__(M_out(L"instring_C::openstd::string, (std::string, \"%s\") called") % to_wsring(name);)  


   // make sure instring_C is ready for reuse 

    close();       // make sure any current string data is cleared out, etc. 
    reset();       // reset to starting position, etc.


    // copy in the string data and save away string-related info
  
    m_in_string.str(::to_wstring(str));      // store caller's string in stringstream -- assumed to be CP 1252
    if (!(m_in_string).good())
    {  
        M_out_emsg(L"instring_C::open() -- Input string \"%s\" cannot be set") % to_wstring(name);
        m_error_seen  = true; 
        return -1; 
    }
    else
    {
        m_in_name     = ::to_wstring(name); 
        m_source_id   = ::add_cached_id(m_in_name);
        m_is_open_s   = true;
        M__(M_out(L"m_in_name = \"%s\"") % m_in_name;)  
    }

    return 0; 
}
M_endf








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    close() string 
//    --------------
//
//   -- leaves old filename, flage, position, line buffers, etc. from last read()
//  
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int instring_C::close() try
{
    M__(M_out(L"instring_C::close() called -- string name = \"%s\"") % m_in_name);

    if (m_is_open_s)
    {
        m_in_string.str(L"");    // empty out string -- no .close() member function
 
        m_is_open_s = false;       
    }

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    reset() instring_C fields for reuse 
//    -----------------------------------
//
//   -- resets everything that close() didn't -- assumes buffer has already been cleaned out, or will soon get reused
//  
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int instring_C::reset() try
{
    M__(M_out(L"instring_C::reset() called -- string name = \"%s\"") % m_in_name);


    // initialize flags and char position in new file and name

    m_error_seen     = false;  
    m_eof_seen       = false; 
    m_curr_linenum   = 0; 
    m_curr_col       = 0; 
    m_curr_pos       = 0; 
    m_end_pos        = 0;  
    
    m_line_ok        = false;
    m_curr_wline     = std::wstring{};
    m_in_name        = std::wstring{};
    
    return 0; 
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// is_open()
// ---------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool instring_C::is_open() const try
{
    return m_is_open_s;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// get_filename()
// --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring instring_C::get_name() const try
{
    return m_in_name;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// get_source_id()
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint64_t instring_C::get_source_id() const try
{
    return m_source_id; 
}
M_endf





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  fetch_char()  -- internal (string assumed to be open/set, and line_OK is true   -or-   m_error_seen/m_eof_seen is true)
//  ------------ 
//
//  sets line_OK to false if last character fetched
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int instring_C::fetch_char(in_char_S & in_char, bool advance) try
{
    M__(M_out(L"instring_C::fetch_char() -- called");)

    // return back existing data, if any -- don't go out to string (even if m_error_seen or m_eof_seen flags are on -- need to pass back old characters first)
        
    if (m_line_ok)    
    {
         if (m_curr_pos < m_end_pos)
         {
             out_char(in_char, char_E::normal, advance);             
             return 0;         
         }          
         else     // sitting past end of data, but haven't passed back EOL yet (EOF may have been seen, too -- will be passed back next time)
         {
             // Pass back end of line, now, and mark line as not OK
         
             out_char(in_char, char_E::eol, advance); 
             if (advance)                                // only update state if we're in advance mode -- state change will occur next time 
                 m_line_ok = false;                      // will need to read in more next time              
             return 0; 
         }    
    }


    // Just pass back error, if prior read has returned fail(), and no data in line buffer 

    if (m_error_seen)
    {
        out_char(in_char, char_E::error, advance); 
        return -1;     
    }
    

    // Just pass back EOF, if prior read has returned EOF, and no data in line buffer 

    if (m_eof_seen)
    {
        out_char(in_char, char_E::eof, advance); 
        return 0;     
    }
    
    M_out_emsg(L"infile_C::fetch_char() called when no valid char could be fetched/returned -- file =\"%s\"") %  m_in_name;
    out_char(in_char, char_E::error, advance);  
    return -1;                                       // should not happen 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  
//  out_char() -- assume current line is OK and not at very end, when passed-in classs = "normal"
//  ---------
//
//      -- does not set proper type, subtype, family -- sets classs field only
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void instring_C::out_char(in_char_S & in_char, char_E classs, bool advance) try    // need to share code with infile_C here ????????????????????????????????????????????????????????
{
    M__(M_out(L"instring_C::out_char() -- called");)

    in_char.source_id = m_source_id;                                          // capture current source ID
    in_char.lineno    = m_curr_linenum;                                       // capture line number (already 1-based)
    in_char.linepos   = m_curr_col + 1;                                       // capture column (1-based)
    in_char.classs    = classs;                                             
    in_char.family    = char_E::none;                                         // initilize to "none" -- will get filled in later
    in_char.type      = char_E::none;                                       
    in_char.subtype   = char_E::none;                                       
                                                                            
    size_t saved_pos = m_curr_pos;                                            // save away initial position for later rollback, in case advance parm = false
    size_t saved_col = m_curr_col;     
   
    switch (classs)
    {
    // normal characters -- fetch from line and convert to UTF-32, etc.

    case char_E::normal :

        // put in 1st or only wide char

        in_char.wch1 = m_curr_wline[m_curr_pos];
        ++m_curr_pos;                                           


        // add in trailing surrogate, if required
        // --------------------------------------

        if ( ((uint32_t)(in_char.wch1) & const_N::utf16_surrogate_mask) == const_N::utf16_leading_base)   // 1st char is a lead surrogate ?
        {
            if ( 
                (m_curr_pos < m_end_pos)
                &&
                (((uint32_t)(m_curr_wline[m_curr_pos]) & const_N::utf16_surrogate_mask) == const_N::utf16_trailing_base)
               )
            {
                 in_char.wch2 = m_curr_wline[m_curr_pos];      // put in trailing surrogate
                 ++m_curr_pos;                
            }
            else    // trailing surrogate not present -- just set to 0's -- should not occur? 
            {
                M_out_note(L"instring_C::out_char() : Leading surrogate found, but not followed by trailing surrogate -- (string)/file=\"%s\" line/col = %u/%llu -- \"") % m_in_name % m_curr_linenum % m_curr_col;                
                in_char.wch2 = (wchar_t)0;                 
            }
        }
        else  // not a leading surrogate -- just put 0's in 2nd widechar 
        {
            in_char.wch2 = (wchar_t)0;     
        }           
 

        // get UTF-32 representation for the wide char(s)

        to_char32(in_char.wch1, in_char.wch2, in_char.ch32);
        ++m_curr_col; 
        break;


    // EOL -- put in unicode NEL character

    case char_E::eol   :

        in_char.wch1  = (wchar_t)0x0085;
        in_char.wch2  = (wchar_t)0;
        in_char.ch32  = (char32_t)0x00000085;
        break;
  

    // special cases -- code points are all 0 -- only type is special

    case char_E::error :
    case char_E::eof   :
        in_char.wch1  = (wchar_t)0;
        in_char.wch2  = (wchar_t)0;
        in_char.ch32  = (char32_t)0;      
    }


    // undo any position advance, if the advance option is false 

    if (!advance)
    {
        m_curr_col = saved_col;
        m_curr_pos = saved_pos;
    }
    
    M__(M_out("infile_C::out_char().2 : m_in_filename=\"%s") % m_in_filename;) 
    return;    
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  
//  get_char() 
//  ---------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int instring_C::get_char(in_char_S& in_char, bool advance) try
{
    M__(M_out(L"instring_C::get_char() -- called");)

    // pass back existing data, or preset error EOF indication, if any -- don't go out to file (which may be closed if eof/error seen)

    if ( m_line_ok || m_eof_seen || m_error_seen )    
        return fetch_char(in_char, advance);    


    //  Need to go out to string to refill the line buffer -- error if string is not open
    //  ---------------------------------------------------------------------------------
    
    if (!(m_is_open_s))
    {
        M_out_emsg(L"instring_C::get_char() called when string is not open  -- \"%s\"") % m_in_name;
        out_char(in_char, char_E::error, advance);         
        return -1; 
    }       


    //  Read in next line of file -- handle EOF, I/O error -- maintain states, etc.
    //  ---------------------------------------------------------------------------

    m_curr_wline = std::wstring{};
    m_end_pos    = 0;

    std::getline(m_in_string, m_curr_wline); 

    if (m_in_string.bad())
    {
        M_out_emsg(L"instring_C::get_char(): bad() on after doing getline(), string name -- \"%s\"") %  m_in_name;
        m_error_seen = true;          // fetch_char() will return the error down below 
        close();                      // make sure no more I/O on this file
    }
    else if ((m_in_string.eof()) && m_curr_wline.length() == 0)
    {
        m_eof_seen = true;            // fetch_char() will return the EOF down below 
        close();                      // make sure no more I/O on this file
    }
    else if (m_in_string.fail())
    {
        M_out_emsg(L"instring_C::get_char(): fail() bit is on after doing getline(), before EOF was reached -- string name = \"%s\"") %  m_in_name;
        m_error_seen = true;          // fetch_char() will return the error down below 
        close();                      // make sure no more I/O on this file       
    }
 

    // read() did not fail -- line is good, but may be empty, etc. -- may have reached EOF
    
    if ( m_eof_seen || m_error_seen )
        m_line_ok  = false;                
    else
    {
        m_line_ok  = true;                     // line might be empty
        m_end_pos  = m_curr_wline.length();    // should be one past end
        ++m_curr_linenum;                      // increment line number in file 
        m_curr_col = 0;
        m_curr_pos = 0; 
        M__(M_out("m_curr_wline = \"%s\"") % m_curr_wline;) 
    }
   
    
    //  At this point, line_OK is on --or-- m_eof_seen/m_error_seen might be on (line can be empty or in bad state in this case) 

    return fetch_char(in_char, advance);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////