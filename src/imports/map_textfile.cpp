// map_textfile.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ================
////     map_textfile.cpp -- text-file related functions
////     ================
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

#define M_IN_MAP_DLL

#include "h__common.h"

#include "h_map_types.h"
#include "h_map_global.h"
#include "h_map_textfile.h"


// local static functions
// ----------------------

static int          tokenize_string(  const std::wstring&, std::vector<std::wstring>&, const std::wstring& = std::wstring{L" "});
static std::wstring decode_dms_string(const std::wstring&);
  

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///     TTTTT    EEEEE   X    X   TTTTT    FFFFFF     IIIII     L         EEEEE              
///       T      E        X  X      T      F            I       L         E              
///       T      EEEE      XX       T      FFFFF        I       L         EEEEE          
///       T      E        X  X      T      F            I       L         E       
///       T      EEEEE   X    X     T      F          IIIII     LLLLLL    EEEEE          
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
// wstring constructor
// ------------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

textfile_C::textfile_C(const std::wstring& wstr)  try    
{
    M__(M_out(L"textfile_C(\"%s\") called -- wstring constructor -- in_filename =  \"%s\"") % wstr % in_filename;) 
    readin(wstr);
    return;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// readin() file
// -------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int textfile_C::readin(const std::wstring& wstr) try
{
    M__(M_out(L"textfile_C::readin(%S) called -- in_filename = \"%S\"") % wstr % in_filename;)  



    freeup();      // make sure any old data is gone


    // open() the file and save away file info  
    
    std::ifstream in_file {wstr}; 
   
    if (!(in_file.good()))
    {  
        M_out_emsg(L"tetxfile_C::readin() -- Input file \"%S\" cannot be opened") % wstr;
        return -1; 
    }
                                            
    in_filename  = wstr; 
 

    //  --------------------------------------------------------------------------------
    //  Read in all text lines in file -- handle EOF, I/O error -- maintain states, etc.
    //  --------------------------------------------------------------------------------

    uint32_t line_num            {0      };      // file line number (for error messages 1 = 1st line in file) 
    bool     is_utf8             {false  };      // true: UTF-8 byte sequence seen at start of file  

    for (;;)                                     // loop will end by   return 0   or   return -1 
    {
        std::string  curr_line  { };             // UTF-8 or ASCII input line from file 
        std::wstring curr_wline { };             // converted UTF-16 line for saving in lines vector
 

        // read in next line from file and return if errors found
        // ------------------------------------------------------

        std::getline(in_file, curr_line); 

        if (in_file.bad())
        {
            M_out_emsg(L"textfile_C::readin(): bad() flag is on after doing getline(), file -- \"%S\"") %  in_filename;
            freeup();                   // clean out everything before returning
            return -1; 
        }
     
        if ((in_file.eof()) && curr_line.length() == 0)
        {
            read_in = true;             // flag lines bufer as coming from readin()
            return 0;                   // normal exit from loop
        }

        if (in_file.fail())
        {
            M_out_emsg(L"textfile_C::readin(): fail() bit is on after doing getline(), before EOF was reached -- file = \"%S\"") %  in_filename;
            freeup();                   // clean out everything before returning
            return -1; 
        }

        line_num++;                       // increment line number for error messages

        // / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / 
        // getline() did not fail -- line is good, but may be empty, etc. -- may have reached EOF
        // / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / 
  
    
        //  If this is first line -- skip past any UTF-8 flag bytes
        //  -------------------------------------------------------

        if (line_num == 1)    
        {
            M_y(M_out(L"textfile_C::readin() -- 0=%02X 1=%02X 2=%02X") % (uint32_t)(uint8_t)(curr_line[0]) % (uint32_t)(uint8_t)(curr_line[1]) % (uint32_t)(uint8_t)(curr_line[2]);)

            if ( 
                (curr_line.length() >= 3)
                && 
                ((uint8_t)(curr_line[0]) == 0xEFU)
                && 
                ((uint8_t)(curr_line[1]) == 0xBBU)
                && 
                ((uint8_t)(curr_line[2]) == 0xBFU)
                )
            {
                is_utf8 = true; 
                curr_line = curr_line.substr(3);        
            }
            else
            {
                is_utf8 = false; 
                M_y(M_out_note(L"textfile_C::readin() -- UTF-8 BOM not seen at start of file -- plain 7-bit ASCII assumed (not code page 1252) -- \"%S\"") %  in_filename;) 
            }
        }


        //  ---------------------------------------------------------------------------
        //  Convert just-read line from UTF-8 (or codepage 1252(??)) to wchar_t (UTF-16) -- catch(...) any conversion failures and flag whole file in error
        //  ----------------------------------------------------------------------------

        if (curr_line.size() > 0)                 // bypass, if inp ut line is empty 
        {
            if (is_utf8)
            {
                try       // do UTF-8 to UTF-16 conversion 
                {
                    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t> > conv_utf8_utf16; 
                    curr_wline = conv_utf8_utf16.from_bytes(curr_line);
                }
                catch(...)
                {
                    M_out_emsg(L"textfile_C::radin(): conv_utf8_utf16 failed, file name = \"%S\"  line=%u") %  in_filename % line_num; 
                    freeup();                                               // get rid of everything before returning
                    return -1; 
                }
            }  
            else       
            {
                try       // do codepage 1252 (??) to UTF-16 conversion
                {
                    curr_wline = to_wstring(curr_line);                      // use CP_ACP by defaulted 2nd parm -- CP_ACP = default system code page = 1252 (??)  
                }
                catch(...)
                {
                    M_out_emsg(L"textfile_C::readin(): to_wstring() failed, file name = \"%S\"  line=%u") %  in_filename % line_num; 
                }
            }
        }
        else
        {
            curr_wline = L"";         // wchar line is empty
        }


        // add converted wstring line to end of lines vector
        // -------------------------------------------------

        lines.push_back(curr_wline);

    }    // end of loop to do getline() once per text line in the file

    return -2;     // should not get here
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    freeup() file 
//    --------------
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int textfile_C::freeup() try
{
    M__(M_out(L"textfile_C::freeup() called -- in_filename = \"%S\"") % in_filename;)

    lines.clear();                      // get rid of any data in lines vector

    in_filename          = L"";         // clear out input filename

    read_in              = false;       // no input data now
    put_in               = false;       // no output data now
   
    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    line_count() -- get n-th text line in lines vector  
//    ------------
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t textfile_C::line_count() try
{
    M__(M_out(L"textfile_C::line_count() called -- in_filename = \"%S\"") % in_filename;)

    return (uint32_t)(lines.size());    
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    get_line() -- get n-th text line in lines vector  
//    ----------
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int textfile_C::get_line(std::wstring& ws, uint32_t n) try
{
    M__(M_out(L"textfile_C::get_line(%u) called -- in_filename = \"%S\"") % in_filename % n;)

    if (n >= lines.size())
    {
         M_out_emsg(L"textfile_C::getline() -- requested line number = %u  max line number = %u") % n % lines.size();
         return -1; 
    }

    // pass back n-th line number from file 

    ws = lines.at(n);    

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    put_line() -- append text line to back of line vector  
//    ----------
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int textfile_C::put_line(std::wstring& ws) try
{
    M_y(M_out(L"textfile_C::put_line(\"%S\") called -- in_filename = \"%S\"") % ws % in_filename;)

    put_in = true; 

    // add new text line to back of line vector 

    lines.push_back(ws);    

    return 0; 
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//     decode_dms_string() -- change input deg min sec string to standard form 
//     ===================
//
//
//
//         input strings accepted   --     ddd°mm'ss"N  (N n S s E e W w allowed)
//                                  --     ddd°mm'ss"   (N or E assumed)  
//                                  --    -ddd°mm'ss"   (S or W assumed) 
//                                  --     ddd°mm'N     (N n S s E e W w allowed)
//                                  --     ddd°mm'      (N or E assumed) 
//                                  --    -ddd°mm'      (S or W assumed) 
//                                  --     ddd.dddN     (N n S s E e W w allowed)
//                                  --     ddd.ddd      (N or E assumed)
//                                  --    -ddd.ddd      (W or W assumed)  
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static std::wstring decode_dms_string(const std::wstring& ws) try
{
     std::wstring temp  {ws};      // get local copy of caller's input string 
     double       dsign {  };      // sign digit multiplier for output string

     bool leading_minus   {false}; 
     bool leading_plus    {false}; 
     bool trailing_ne     {false};
     bool trailing_sw     {false};
    


     // return error, if empty input string
     // -----------------------------------

     if (ws.size() == 0)
     {
         M_out_emsg(L"decode_dms_string() -- empty input string = \"%S\"") % ws; 
         return std::wstring {L"Error"}; 
     }


     // check for leading minus and/or trailing N e E e S s or W w
     // ----------------------------------------------------------

     auto ws_back = ws.back(); 

     if (ws.front() == L'-')        leading_minus = true;
     if (ws.front() == L'+')        leading_plus  = true;
     if (ws_back    == L'N')        trailing_ne   = true;
     if (ws_back    == L'n')        trailing_ne   = true;
     if (ws_back    == L'E')        trailing_ne   = true;
     if (ws_back    == L'e')        trailing_ne   = true;
     if (ws_back    == L'S')        trailing_sw   = true;
     if (ws_back    == L's')        trailing_sw   = true;
     if (ws_back    == L'W')        trailing_sw   = true;
     if (ws_back    == L'w')        trailing_sw   = true;


     // Make sure don't have both leading +- and trailing N E S W
     // ---------------------------------------------------------

     if (
           (leading_plus || leading_minus)
           &&
           (trailing_ne || trailing_sw)
        )
     {
          M_out_emsg(L"decode_dms_string() -- leading + or -, and trailing N E S W -- input string = \"%S\"") % ws;  
          return std::wstring {L"Error"};         
     }


     // get rid of any leading sign letter -- error if rest of string is empty
     // ----------------------------------------------------------------------

     if (leading_plus || leading_minus) 
     {
         if (temp.size() <= 1)
         {
             M_out_emsg(L"decode_dms_string() -- leading + or -, but no characters following -- input string = \"%S\"") % ws;  
             return std::wstring {L"Error"};
         }

         temp = temp.substr(1);  
     }


     // get rid of any trailing N E S W letter -- error if rest of string is empty
     // --------------------------------------------------------------------------

     if (trailing_ne || trailing_sw) 
     {
         if (temp.size() <= 1)
         {
             M_out_emsg(L"decode_dms_string() -- Trailing N E S or W, but no prior characters -- input string = \"%S\"") % ws;  
             return std::wstring {L"Error"};
         }

         temp = temp.substr(0, temp.size() - 1);    // strip off trailing letter  
     }


     // set up sign of outpuot latitude or longitude
     // --------------------------------------------

    if (leading_minus || trailing_sw)
        dsign = -1.0; 
    else
        dsign = 1.0; 

    
    // ------------------------------------------------------------------------------------------------------------
    // split compute lat/lon degrees minutes and seconds substrings -- these should be valid floating point numbers
    // ------------------------------------------------------------------------------------------------------------

    std::vector<std::wstring>   tokens { };       // vector of split tokens
    std::wstring                deg    { };       // string with degrees
    std::wstring                min    { };       // string with minutes
    std::wstring                sec    { };       // string with seconds


    // check final remaining letter -- should be number or . " ' or ° 
    // --------------------------------------------------------------

    auto temp_back = temp.back(); 


    // handle case where there are no minutes or seconds present -- only degrees
    // -------------------------------------------------------------------------

    if (
        (temp_back == L'.')
        ||
        (temp_back == L'°')
        ||
        (std::isdigit(temp_back))
       )
    {
        if (temp_back = L'°')
        {
            if (temp.size() <= 1)
            {
                M_out_emsg(L"decode_dms_string() -- degree sign found, but empty string before it -- input string = \"%S\"") % ws;  
                return std::wstring {L"Error"};
            } 
        
            temp = temp.substr(0, temp.size() - 1);    // strip off trailing degree sign         
        }


        //  What's left in temp should be a valid number of degrees -- assume minutes and seconds are 0.0
        //  ---------------------------------------------------------------------------------------------

        deg = temp; 
        min = std::wstring {L"0.0"};
        sec = std::wstring {L"0.0"};
    }


    //  handle case where only degrees and minutes are present
    //  ------------------------------------------------------

    else if (temp_back == L'\'')
    {
        if (temp.size() <= 1)
        {
             M_out_emsg(L"decode_dms_string() -- minute sign found at end, but empty string before it -- input string = \"%S\"") % ws;  
             return std::wstring {L"Error"};
        }  

        temp = temp.substr(0, temp.size() - 1);    // strip off trailing minutes sign  -- only things left should be valid numbers and one degrees sign separating the two numbers


        // break temp string  into tokens using degree sign -- should be two tokens only  (1st one is degrees, 2nd one is minutes)
        // -----------------------------------------------------------------------------

        tokenize_string(temp, tokens, std::wstring {L"°"} );

        if (tokens.size() != 2)
        {
            M_out_emsg(L"decode_dms_string() -- minutes sign found but number of degree signs found was not 1 (tokens.size() = %d) -- input string = \"%S\"") % tokens.size() % ws;  
            return std::wstring {L"Error"};          
        }

        deg = tokens.at(0);           // 1st token is degrees
        min = tokens.at(1);           // 2nd token is minutes 
        sec = std::wstring {L"0.0"};  // assume no seconds
    }


    // handle case where degrees, minutes and seconds are all present
    // --------------------------------------------------------------

    else if (temp_back == L'"')
    {
        if (temp.size() <= 1)
        {
             M_out_emsg(L"decode_dms_string() -- seconds sign found at end, but empty string before it -- input string = \"%S\"") % ws;  
             return std::wstring {L"Error"};
        }  

        temp = temp.substr(0, temp.size() - 1);    // strip off trailing seconds sign  -- only things left should be valid numbers and one degrees and one minutes sign separating the three numbers
    

        // break temp string  into tokens using degree sign -- should be two tokens only  (1st one is degrees, 2nd one is minutes minute sign and seconds)
        // -----------------------------------------------------------------------------

        tokenize_string(temp, tokens, std::wstring {L"°"} );

        if (tokens.size() != 2)
        {
            M_out_emsg(L"decode_dms_string() -- minutes and seconds signs found but number of degree signs found was not 1 (tokens.size() = %d) -- input string = \"%S\"") % tokens.size() % ws;  
            return std::wstring {L"Error"};          
        }


        // break 2nd token into subtokens using minutes sign -- should be two tokens only  (1st one is minutes, 2nd one is seconds)
        // ------------------------------------------------------------------------------
                                                                                        
        std::vector<std::wstring> tokens2 {};     

        tokenize_string(tokens.at(1), tokens2, std::wstring {L"'"}   );

        if (tokens2.size() != 2)
        {
            M_out_emsg(L"decode_dms_string() -- minutes and seconds signs found but number of minutes signs found was not 1 (tokens2.size() = %d) -- input string = \"%S\"") % tokens2.size() % ws;  
            return std::wstring {L"Error"};          
        }

        deg = tokens.at( 0);           // 1st token is degrees
        min = tokens2.at(0);           // 2nd token is minutes 
        sec = tokens2.at(1);           // 3rd token is minutes        
    }
    else      // last digit is not valid
    {
         M_out_emsg(L"decode_dms_string() -- Last or 2nd-to-last character should be ' \" ° . or a digit -- input string = \"%S\"") % ws;  
         return std::wstring {L"Error"};         
    }   

    M__(M_out(L"decode_dms_string() -- ws=\"%S\"  deg=\"%S\"  min=\"%S\"  sec=\"%S\"") % ws % deg % min % sec;)


    //  ---------------------------------------------------------------------------------
    //  convert deg min sec substrings along with dsign into single floating point number
    //  --------------------------------------------------------------------------------- 

    double                     degrees  { };       // output degrees value
    double                     minutes  { };       // output minutes value 
    double                     seconds  { };       // output seconds value 
    std::size_t                idx      { };       // output char count from stod() conversion  


    // convert degrees substring to double
    // -----------------------------------

    try 
    {
        degrees = std::stod(deg, &idx); 
    }
    catch (...)
    {
        M_out_emsg(L"decode_dms_string() -- cannnot convert degrees digits in input string to double -- input string = \"%S\"") % ws;  
        return std::wstring {L"Error"};          
    }

    if (idx != deg.size())
    {
        M_out_emsg(L"decode_dms_string() -- conversion of degrees digits to double in input string did not consume all digits -- input string = \"%S\"") % ws;  
        return std::wstring {L"Error"}; 
    }


    // convert minutes substring to double
    // -----------------------------------
    
    try 
    {
        minutes = std::stod(min, &idx); 
    }
    catch (...)
    {
        M_out_emsg(L"decode_dms_string() -- cannnot convert minutes digits in input string to double -- input string = \"%S\"") % ws;  
        return std::wstring {L"Error"};          
    }

    if (idx != min.size())
    {
        M_out_emsg(L"decode_dms_string() -- conversion of minutes digits to double in input string did not consume all digits -- input string = \"%S\"") % ws;  
        return std::wstring {L"Error"}; 
    }


    // convert seconds substring to double
    // -----------------------------------
    
    try 
    {
        seconds = std::stod(sec, &idx); 
    }
    catch (...)
    {
        M_out_emsg(L"decode_dms_string() -- cannnot convert seconds digits in input string to double -- input string = \"%S\"") % ws;  
        return std::wstring {L"Error"};          
    }

    if (idx != sec.size())
    {
        M_out_emsg(L"decode_dms_string() -- conversion of seconds digits to double in input string did not consume all digits -- input string = \"%S\"") % ws;  
        return std::wstring {L"Error"}; 
    }


    // compute final latitude or longitude and convert to string 
    // ---------------------------------------------------------

    double ll    = dsign * (degrees + minutes / 60.0 + seconds / 3600.00); 
    auto   llout = std::to_wstring(ll);  

    M__(M_out(L"decode_dms_string() -- ws=\"%S\"  deg=\"%S\"  min=\"%S\"  sec=\"%S\"  d/m/s = %.6f/%.6f/%.6f   ll=%.6f   llout=\"%S\"") % ws % deg % min % sec % degrees% minutes % seconds % ll % llout; )

    return llout; 
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//     find_tokens() -- build output vector containing all tokens from input vector that contain passed-in string (or char) 
//     =============
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int find_tokens(const std::vector<std::wstring>& in_tokens, std::vector<std::wstring>& out_tokens, const std::wstring& ws) try
{
    out_tokens.clear();          // make sure output vector is empty before starting

    std::string::size_type idx1 {0};
    std::string::size_type idx2 {0};

    for(auto in_token : in_tokens)
    {
        if (in_token.find(ws) != std::string::npos) 
            out_tokens.push_back(in_token);     
    }

    return 0; 
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//     tokenize_string() -- break input std::wstring into vector of wstrings, 1 per token 
//     =================
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
static int tokenize_string(const std::wstring& ws, std::vector<std::wstring>& vec, const std::wstring& val) try
{
    vec.clear();          // make sure output vector is empty before starting

    std::string::size_type idx1 {0};
    std::string::size_type idx2 {0};


    // main loop to add tokens to output vector
    // ----------------------------------------

    for (;;)
    {
        M__(M_out(L"tokenize_wstring() -- top of loop -- idx1 = %d idx2 = %d") % idx1 % idx2;)

        idx1 = ws.find_first_not_of(val, idx2);     
    
        if (idx1 != std::string::npos)
        {
            idx2 = ws.find_first_of(val, idx1);

            if (idx2 != std::string::npos)
            {
                vec.push_back(ws.substr(idx1, idx2 - idx1)); 
            }
            else
            {
                vec.push_back(ws.substr(idx1));      // next token goes out to end of string
                return 0; 
            }  
        }
        else   // search stopped at end of string
        {
            return 0;   // end of string reached without finding another token
        }      
    }
    return 0; 
}
M_endf

 ///////////////////////////////////////////////////////////


int test_textfile(const mapparm_S& parm) try
{
    M_y(M_out(L"test_textfile() called");)

    textfile_C intext  {}; 
    textfile_C outtext {}; 

    //intext.readin(L"E:\\Data01\\Text\\Isolated_peaks_NA.txt");
    //intext.readin(L"E:\\Data01\\Text\\Isolated_peaks_canada.txt");
    
    intext.readin(L"E:\\Data01\\Text\\High_andes.txt");

#if 0 
    uint32_t lc = intext.line_count();  

    M_out(L"test_textfile() -- number of lines = %u") % lc;
   
    std::wstring              longline  {}; 
    std::vector<std::wstring> longlines {};

     for (auto i = 0; i < lc; i++)
     {
         std::wstring    line    {   }; 
         intext.get_line(line, i);

         M__(M_out(L"line[%-4u] = \"%S\"") % i % line;)  

         if (line.size() <= 0)
         {
             if (longline.size() > 0)
                 longlines.push_back(longline);
             longline = std::wstring {}; 
         }
         else
         {
             longline += (std::wstring {L" "} + line); 
         }
     }


    // flush out final line, if any

    if (longlines.size() > 0)
        longlines.push_back(longline);  

    //////////////////////////////////////////////////////////

    int ixl {0}; 

    std::vector<std::wstring> tokv {}; 

    for (auto ll : longlines)
    {
        ixl++;
        M__(M_out(L"line[%-4u] = \"%S\"") % ixl++ % ll;)  

        auto trc = tokenize_string(ll, tokv);

        std::wstring namews {};


        // find name of peak -- start with 2nd token and end when a token with [ is found 


        for (auto ixt = 1; ixt < tokv.size(); ixt++)
        {
            std::wstring tok = tokv.at(ixt);

            auto paren_pos = tok.find_first_of(L"[");
            if (paren_pos != std::string::npos)
            {
                namews += (std::wstring {L" "} + tok.substr(0, paren_pos));
                break; 
            }
            else
            {
                namews += (std::wstring{L" "} + tok);   // add whole token to peak name
            }                                 
        } 

        namews = namews.substr(namews.find_first_not_of(L" "));  
        namews = L"L\"" + namews + L"\""; 
     //   M_out(L"namews = \"%S\"") % namews;     


        // find elevation of peak -- token before the 1st "m"

        std::wstring elevws {}; 

        for (auto ixt = 0; ixt < tokv.size(); ixt++)
        {
            if (tokv.at(ixt) == L"m")
            {
                elevws =  tokv.at(ixt-1); 
                break;      
            }
        }


        // get rid of any decimal point (just truncate the elevation) 

        auto ixpt = elevws.find_first_of(L".");
        if (ixpt != std::string::npos)
            elevws = elevws.substr(0,ixpt);  


        // M_out(L"elev = \"%S\"") % elevws; 
        // final two tokens should be the lat and lon

        std::wstring latws {tokv.at(tokv.size() - 2)};
        std::wstring lonws {tokv.at(tokv.size() - 1)};

        if (latws.back() == L'S')
           latws = L"-" + latws; 
         if (lonws.back() == L'W')
           lonws = L"-" + lonws; 

        latws = latws.substr(0, latws.size() - 2);
        lonws = lonws.substr(0, lonws.size() - 2);  

        //   M_out(L"lat = \"%S\"") % latws; 
        //   M_out(L"lon = \"%S\"") % lonws;  

        M_out(L"    M_db_deg( %-50S ,  %-5S ,  %-12S ,  %-12S  )  //    %d") % namews % elevws % latws % lonws % ixl; 
    }   
#endif


    uint32_t lc = intext.line_count();  

    M_out(L"test_textfile() -- number of lines = %u") % lc;
   

    std::vector<std::wstring> lines {};

     for (auto i = 0; i < lc; i++)
     {
         std::wstring    line      {   }; 
         intext.get_line(line, i); 
         lines.push_back(line);

         M__(M_out(L"line[%-4u] = \"%S\"") % i % line;)  
     }

     ///////////////////////////////

     int linect {0}; 

     for (auto line : lines)
     {
         linect++;    // line numbers start at 1 for debugging    

         // get tokens in this line of file
         // -------------------------------

         std::vector<std::wstring> tokens { };  
         tokenize_string(line, tokens); 

         if (tokens.size() < 4)
         {
              M_out(L"test_textfile() -- Line[%d] only has %d blank-delimited tokens") % linect % tokens.size() ;
              continue;
         }


         // extract elevation and lat/lon

         auto elevws = tokens.at(0);     // 1st token is elevation

         std::vector<std::wstring> ll_tokens { }; 

         find_tokens(tokens, ll_tokens, std::wstring {L"°"});

         if (ll_tokens.size() != 2)
         {
             M_out(L"    // test_textfile() -- Line[%d] has %d tokens that look like lat/lon") % linect % ll_tokens.size() ;
             continue; 
         }
     
         auto lat = ll_tokens.at(0);
         auto lon = ll_tokens.at(1); 

         auto latws = decode_dms_string(lat);
         auto lonws = decode_dms_string(lon);


         // extract name  -- start at 2nd token, looking for [ token

         std::wstring namews           {    }; 
         bool         first_name_token {true}; 
            
         auto tokens2 = tokens; 

         tokens2.erase(tokens2.begin());   // remove 1st token  

         for (auto token2 : tokens2)
         {
              if (token2 == std::wstring {L"["})    
                 break;

              if (first_name_token)
              {
                  namews = token2; 
                  first_name_token = false; 
              }
              else
                  namews += (std::wstring {L" "} + token2);                  
         }
 
         namews = L"L\"" + namews + L"\""; 

         M__(M_out(L" elev = \"%S\"      lat = \"%S\" = \"%S\"       lon = \"%S\" = \"%S\"    name = \"%S\"") % elevws % lat % latws % lon % lonws % namews;)  

         M_out(L"    M_db_deg( %-50S ,  %-5S ,  %-12S ,  %-12S  )  //    %d") % namews % elevws % latws % lonws % linect; 






     }


    return 0; 
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////