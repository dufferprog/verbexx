// h_ex_lex.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////
////            ==========
////            h_ex_lex.h -- items for  ex_inchar.cpp,  ex_infile.cpp,  and  ex_intoken.cpp  
////            ==========
//// 
////
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
 

//   ------------------------------------------------------------ 
//   Default action characters for lexer, parser,  and pre-parser
//   ------------------------------------------------------------ 
                                                                                             
namespace const_N
{
constexpr char32_t  ch_pp_sigil_action             { utf32_N::QUESTION_MARK                                 };      // pre-processor leading  sigil for tokens of interest to pre-processor
constexpr char32_t  ch_pp_sigil_label              { utf32_N::COLON                                         };      // pre-processor trailing sigil for label
constexpr char32_t  ch_pp_sigil_subst              { utf32_N::QUESTION_MARK                                 };      // pre-processor trailing sigil for variable substitution
constexpr char32_t  ch_pp_sigil_quote_subst        { utf32_N::EXCLAMATION_MARK                              };      // pre-processor trailing sigil for variable substitution with added quotes 
                                                                                                          
constexpr char32_t  ch_sigil_label                 { utf32_N::EXCLAMATION_MARK                              };      // parser leading  sigil for label
constexpr char32_t  ch_sigil_keyword               { utf32_N::COLON                                         };      // parser trailing sigil for keyword
constexpr char32_t  ch_sigil_verb                  { utf32_N::COMMERCIAL_AT                                 };      // parser leading  sigil for verb
constexpr char32_t  ch_sigil_parm                  { utf32_N::DOLLAR_SIGN                                   };      // parser leading  sigil for parm    
                                                                                                          
constexpr char32_t  ch_trigraph                    { utf32_N::QUESTION_MARK                                 };      // (pre-processor) trigraph                         character (default)
constexpr char32_t  ch_vanishing_sep               { utf32_N::_BAD_CHARACTER_                               };      // (pre-processor) vanishing separator              character (default = none -- use () or , )
constexpr char32_t  ch_line_continuation           { utf32_N::_BAD_CHARACTER_                               };      // pre-processor line continuation                  character (default = none                ) 
constexpr char32_t  ch_always_sign                 { utf32_N::_BAD_CHARACTER_                               };      // always_sign char -- always becomes part of signed integer  (default = none                ) 
                             
constexpr char32_t  ch_type1_string_start          { utf32_N::QUOTATION_MARK                                };      // starting character fot type 1 string -- "
constexpr char32_t  ch_type1_string_end            { utf32_N::QUOTATION_MARK                                };      //   ending character fot type 1 string -- "
constexpr char32_t  ch_type2_string_start          { utf32_N::LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK     };      // starting character fot type 2 string -- «
constexpr char32_t  ch_type2_string_end            { utf32_N::RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK    };      //   ending character fot type 2 string -- »
constexpr char32_t  ch_type1_string_escape         { utf32_N::REVERSE_SOLIDUS                               };      // (pre-processor) type 1 string escape             character (default)
constexpr char32_t  ch_type2_string_escape         { utf32_N::ACUTE_ACCENT                                  };      // (pre-processor) type 2 string escape             character (default)
constexpr char32_t  ch_raw_string_prefix           { utf32_N::LATIN_CAPITAL_LETTER_R                        };      // prefix char for raw       string -- R"   "    or  R«   »  
constexpr char32_t  ch_multiline_string_prefix     { utf32_N::LATIN_CAPITAL_LETTER_M                        };      // prefix char for multiline string -- M"   "    or  M«   » 
constexpr char32_t  ch_word_string_start           { utf32_N::APOSTROPHE                                    };      // starting delimiter for word strings 

constexpr char32_t  ch_unechoed_line_comment       { utf32_N::_BAD_CHARACTER_                               };      // pre-processor unechoed line comment              character (default = none -- use   //    )
constexpr char32_t  ch_echoed_line_comment         { utf32_N::_BAD_CHARACTER_                               };      // pre-processor echoed line comment                character (default = none                )
                                                                                                          
constexpr char32_t  ch_comment_1st                 { utf32_N::SOLIDUS                                       };      // 1st char for all comments                 (   /*  //  /<  /~           )   
constexpr char32_t  ch_unechoed_line_comment_2nd   { utf32_N::SOLIDUS                                       };      // 2nd char for unechoed line  comment delim (   //                       ) 
constexpr char32_t  ch_echoed_line_comment_2nd     { utf32_N::_BAD_CHARACTER_                               };      // 2nd char for echoed line    comment delim (                            ) 
constexpr char32_t  ch_suppress_eol_comment_2nd    { utf32_N::_BAD_CHARACTER_                               };      // 2nd char for suppress EOL   comment delim (                            ) 
constexpr char32_t  ch_eof_comment_2nd             { utf32_N::TILDE                                         };      // 2nd char for EOF            comment delim (   /~                       )  
constexpr char32_t  ch_retained_line_comment_2nd   { utf32_N::NUMBER_SIGN                                   };      // 2nd char for retained line  comment delim (   /#                       )
                                                                                                          
constexpr char32_t  ch_block_comment_2nd           { utf32_N::ASTERISK                                      };      // 2nd char for block          comment delim (   /*          */           )  
constexpr char32_t  ch_block_comment_3rd           { utf32_N::ASTERISK                                      };      // 3rd char for block          comment delim (   /*          */           ) 
constexpr char32_t  ch_block_comment_4th           { utf32_N::SOLIDUS                                       };      // 4th char for block          comment delim (   /*          */           )  
constexpr char32_t  ch_nest_comment_2nd            { utf32_N::LESS_THAN_SIGN                                };      // 2nd char for nestable       comment delim (   /<  /<  >/  >/           ) 
constexpr char32_t  ch_nest_comment_3rd            { utf32_N::GREATER_THAN_SIGN                             };      // 3rd char for nestable       comment delim (   /<  /<  >/  >/           ) 
constexpr char32_t  ch_nest_comment_4th            { utf32_N::SOLIDUS                                       };      // 4th char for nestable       comment delim (   /<  /<  >/  >/           ) 
constexpr char32_t  ch_retained_block_comment_2nd  { utf32_N::LEFT_CURLY_BRACKET                            };      // 2nd char for retained_block comment delim (   /{          }/           ) 
constexpr char32_t  ch_retained_block_comment_3rd  { utf32_N::RIGHT_CURLY_BRACKET                           };      // 3rd char for retained_block comment delim (   /{          }/           ) 
constexpr char32_t  ch_retained_block_comment_4th  { utf32_N::SOLIDUS                                       };      // 4th char for retained_block comment delim (   /{          }/           ) 
                                                                                                          
constexpr char32_t  ch_backspace_escape            { utf32_N::LATIN_SMALL_LETTER_B                          };      // in-string escape char for backspace 
constexpr char32_t  ch_form_feed_escape            { utf32_N::LATIN_SMALL_LETTER_F                          };      // in-string escape char for form feed 
constexpr char32_t  ch_new_line_escape             { utf32_N::LATIN_SMALL_LETTER_N                          };      // in-string escape char for new line 
constexpr char32_t  ch_carriage_return_escape      { utf32_N::LATIN_SMALL_LETTER_R                          };      // in-string escape char for carriage return 
constexpr char32_t  ch_tab_escape                  { utf32_N::LATIN_SMALL_LETTER_T                          };      // in-string escape char for tab  
constexpr char32_t  ch_vertical_tab_escape         { utf32_N::LATIN_SMALL_LETTER_V                          };      // in-string escape char for vertical tab 
constexpr char32_t  ch_alarm_escape                { utf32_N::LATIN_SMALL_LETTER_A                          };      // in-string escape char for alarm 
constexpr char32_t  ch_hex_escape                  { utf32_N::LATIN_SMALL_LETTER_X                          };      // in-string escape char for hex data 
                                                                                                          
constexpr char32_t  ch_base_prefix                 { utf32_N::DIGIT_ZERO                                    };      // based   numeric literal prefix 0b 0o 0x etc.
constexpr char32_t  ch_base2_lower                 { utf32_N::LATIN_SMALL_LETTER_B                          };      // base 2  numeric literal prefix 0b 
constexpr char32_t  ch_base2_upper                 { utf32_N::LATIN_CAPITAL_LETTER_B                        };      // base 2  numeric literal prefix 0B
constexpr char32_t  ch_base8_lower                 { utf32_N::LATIN_SMALL_LETTER_O                          };      // base 8  numeric literal prefix 0o 
constexpr char32_t  ch_base8_upper                 { utf32_N::LATIN_CAPITAL_LETTER_O                        };      // base 8  numeric literal prefix 0O
constexpr char32_t  ch_base16_lower                { utf32_N::LATIN_SMALL_LETTER_X                          };      // base 16 numeric literal prefix 0x 
constexpr char32_t  ch_base16_upper                { utf32_N::LATIN_CAPITAL_LETTER_X                        };      // base 16 numeric literal prefix 0X
                                                                                                          
constexpr char32_t  ch_signed_lower                { utf32_N::LATIN_SMALL_LETTER_I                          };      // signed numeric literal suffix i32 etc.
constexpr char32_t  ch_signed_upper                { utf32_N::LATIN_CAPITAL_LETTER_I                        };      // signed numeric literal suffix I32 etc.
constexpr char32_t  ch_unsigned_lower              { utf32_N::LATIN_SMALL_LETTER_U                          };      // unsigned numeric literal suffix u32 etc.
constexpr char32_t  ch_unsigned_upper              { utf32_N::LATIN_CAPITAL_LETTER_U                        };      // unsigned numeric literal suffix U32 etc.
constexpr char32_t  ch_float_lower                 { utf32_N::LATIN_SMALL_LETTER_F                          };      // unsigned numeric literal suffix f64 etc.
constexpr char32_t  ch_float_upper                 { utf32_N::LATIN_CAPITAL_LETTER_F                        };      // unsigned numeric literal suffix F64 etc.
constexpr char32_t  ch_exponent_lower              { utf32_N::LATIN_SMALL_LETTER_E                          };      // numeric literal exponent suffix e-11 etc.
constexpr char32_t  ch_exponent_upper              { utf32_N::LATIN_CAPITAL_LETTER_E                        };      // numeric literal exponent suffix E+12 etc.

constexpr wchar_t  *chws_signed_lower              { L"i"                                                   };      // for debug mode output strings 
constexpr wchar_t  *chws_unsigned_lower            { L"u"                                                   };      // for debug mode output strings 
constexpr wchar_t  *chws_float_lower               { L"f"                                                   };      // for debug mode output strings 
constexpr wchar_t  *chws_string_start              { L"«"                                                   };      // for debug mode output strings 
constexpr wchar_t  *chws_string_end                { L"»"                                                   };      // for debug mode output strings 
} 

                                                                  
//   ---------------------------------------------------------------------------------------------------------------------------------------------------- 
//
//   trigraph characters  -- in and out
//   ----------------------------------
//
//      
//     ???   =  ?        GRAVE_ACCENT                                   lex: default trigraph char          (not in table) 
//     ??<   =  «        LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK      lex: start of type-2 string
//     ??>   =  »        RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK     lex: end   of type-2 string
//     ??'   =  ´        ACUTE_ACCENT                                   lex: default type 2 string escape char
//     ??~   =  ¬        NOT_SIGN                                       used in ¬= operator, etc.                         ????? out ????
//                                                                 
//     ??4   =  ¼        VULGAR_FRACTION_ONE_QUARTER                    for use in strings     
//     ??2   =  ½        VULGAR_FRACTION_ONE_HALF                       for use in strings
//     ??3   =  ¾        VULGAR_FRACTION_THREE_QUARTERS                 for use in strings
//     ??c   =  ¢        CENT_SIGN                                      for use in strings      
//
//
//   ----------------------------------------------------------------------------------------------------------------------------------------------------- 

namespace const_N
{
 constexpr char32_t  ch_trigraph_in_01             { utf32_N::APOSTROPHE                                    };     
 constexpr char32_t  ch_trigraph_out_01            { utf32_N::ACUTE_ACCENT                                  };     

 constexpr char32_t  ch_trigraph_in_02             { utf32_N::LESS_THAN_SIGN                                };     
 constexpr char32_t  ch_trigraph_out_02            { utf32_N::LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK     };     

 constexpr char32_t  ch_trigraph_in_03             { utf32_N::GREATER_THAN_SIGN                             };     
 constexpr char32_t  ch_trigraph_out_03            { utf32_N::RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK    };     

 constexpr char32_t  ch_trigraph_in_04             { utf32_N::TILDE                                         };     
 constexpr char32_t  ch_trigraph_out_04            { utf32_N::NOT_SIGN                                      };     

 constexpr char32_t  ch_trigraph_in_05             { utf32_N::LATIN_SMALL_LETTER_C                          };     
 constexpr char32_t  ch_trigraph_out_05            { utf32_N::CENT_SIGN                                     };     

 constexpr char32_t  ch_trigraph_in_06             { utf32_N::DIGIT_TWO                                     };    
 constexpr char32_t  ch_trigraph_out_06            { utf32_N::VULGAR_FRACTION_ONE_HALF                      }; 

 constexpr char32_t  ch_trigraph_in_07             { utf32_N::DIGIT_THREE                                   };     
 constexpr char32_t  ch_trigraph_out_07            { utf32_N::VULGAR_FRACTION_ONE_THIRD                     }; 

 constexpr char32_t  ch_trigraph_in_08             { utf32_N::DIGIT_FOUR                                    };     
 constexpr char32_t  ch_trigraph_out_08            { utf32_N::VULGAR_FRACTION_ONE_QUARTER                   };     

 constexpr char32_t  ch_trigraph_in_09             { utf32_N::DIGIT_FIVE                                    };     
 constexpr char32_t  ch_trigraph_out_09            { utf32_N::VULGAR_FRACTION_TWO_THIRDS                    };     

 constexpr char32_t  ch_trigraph_in_10             { utf32_N::DIGIT_SIX                                     };    
 constexpr char32_t  ch_trigraph_out_10            { utf32_N::VULGAR_FRACTION_ONE_SIXTH                     };  

 constexpr char32_t  ch_trigraph_in_11             { utf32_N::DIGIT_SEVEN                                   };    
 constexpr char32_t  ch_trigraph_out_11            { utf32_N::VULGAR_FRACTION_THREE_QUARTERS                }; 

 constexpr char32_t  ch_trigraph_in_12             { utf32_N::DIGIT_EIGHT                                   };     
 constexpr char32_t  ch_trigraph_out_12            { utf32_N::VULGAR_FRACTION_ONE_EIGHTH                    };  
 
 constexpr char32_t  ch_trigraph_in_13             { utf32_N::LATIN_SMALL_LETTER_T                          };     
 constexpr char32_t  ch_trigraph_out_13            { utf32_N::DAGGER                                        }; 

 constexpr char32_t  ch_trigraph_in_14             { utf32_N::LATIN_CAPITAL_LETTER_I                        };    
 constexpr char32_t  ch_trigraph_out_14            { utf32_N::DOUBLE_DAGGER                                 }; 

 constexpr char32_t  ch_trigraph_in_15             { utf32_N::LATIN_CAPITAL_LETTER_P                        };     
 constexpr char32_t  ch_trigraph_out_15            { utf32_N::PILCROW_SIGN                                  }; 

 constexpr char32_t  ch_trigraph_in_16             { utf32_N::LATIN_SMALL_LETTER_P                          };     
 constexpr char32_t  ch_trigraph_out_16            { utf32_N::REVERSED_PILCROW_SIGN                         }; 

 constexpr char32_t  ch_trigraph_in_17             { utf32_N::LATIN_CAPITAL_LETTER_S                        };     
 constexpr char32_t  ch_trigraph_out_17            { utf32_N::SECTION_SIGN                                  }; 

 constexpr char32_t  ch_trigraph_in_18             { utf32_N::LATIN_SMALL_LETTER_O                          };     
 constexpr char32_t  ch_trigraph_out_18            { utf32_N::CURRENCY_SIGN                                 }; 

 // unused trigraph sequences:

 constexpr char32_t  ch_trigraph_in_19             { utf32_N::_BAD_CHARACTER_1                              };     
 constexpr char32_t  ch_trigraph_out_19            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_20             { utf32_N::_BAD_CHARACTER_2                              };     
 constexpr char32_t  ch_trigraph_out_20            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_21             { utf32_N::_BAD_CHARACTER_3                              };     
 constexpr char32_t  ch_trigraph_out_21            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_22             { utf32_N::_BAD_CHARACTER_4                              };     
 constexpr char32_t  ch_trigraph_out_22            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_23             { utf32_N::_BAD_CHARACTER_5                              };     
 constexpr char32_t  ch_trigraph_out_23            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_24             { utf32_N::_BAD_CHARACTER_6                              };     
 constexpr char32_t  ch_trigraph_out_24            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_25             { utf32_N::_BAD_CHARACTER_7                              };     
 constexpr char32_t  ch_trigraph_out_25            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_26             { utf32_N::_BAD_CHARACTER_8                              };     
 constexpr char32_t  ch_trigraph_out_26            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_27             { utf32_N::_BAD_CHARACTER_9                              };     
 constexpr char32_t  ch_trigraph_out_27            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_28             { utf32_N::_BAD_CHARACTER_10                             };     
 constexpr char32_t  ch_trigraph_out_28            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_29             { utf32_N::_BAD_CHARACTER_11                             };     
 constexpr char32_t  ch_trigraph_out_29            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_30             { utf32_N::_BAD_CHARACTER_13                             };     
 constexpr char32_t  ch_trigraph_out_30            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_31             { utf32_N::_BAD_CHARACTER_14                             };     
 constexpr char32_t  ch_trigraph_out_31            { utf32_N::_BAD_CHARACTER_                               }; 

 constexpr char32_t  ch_trigraph_in_32             { utf32_N::_BAD_CHARACTER_15                             };     
 constexpr char32_t  ch_trigraph_out_32            { utf32_N::_BAD_CHARACTER_                               }; 
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  -----------------------------------------------------------
//  char_E -- character classses, families, types, and subtypes
//  -----------------------------------------------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//                                   classs = C     family = F      type = T      subtype = S

enum class char_E { none                      // C F T S   initialized -- not yet set
                  , eol                       // C F T S   both type and subtype fields  
                  , eof                       // C F T S   both type and subtype fields
                  , end                       // C F T S   both type and subtype fields
                  , error                     // C F T S   both type and subtype fields
                  , normal                    // C - - -   all characters with code points -- class only -- gets classified by Unicode codepoint   
                  
                  //////////////////////////// family, type, subtype only below ////////////////////////

                  , unsupported               // - F T S   completely unsupported control and other characters (allowed in comments only)
                  , whitespace                // - F T S   whitespace char -- blank, tabs, FF, LF, etc.
                  , alphanum                  // - F - -   numbers and letters (with/without accents, etc.) 
                  , punct                     // - F T -   all punctuation characters -- do not usually combine into tokens ( ) [ ] { } ! ? , . ; : ' "  
                  , oper                      // - F T -   operator characters - + = / < > | * ^ \ (normally combine into operator tokens, but not identifiers)  
                  , special                   // - F T -   special characters _ @ # % &, etc.  -- some can be in identifiers (note $ and currency are in this family, but currency type below)
                  , misc                      // - F T S   miscellaneous -- degree, copyright, TM, etc.   
                  , accent                    // - F T -   (pure) accent marks and diacritics, etc. ???? -- some may combine with letters, etc.

                  /////////////////////////// type and subtype only below ////////////////////////////

                  , num                       // - - T S   numeric digits (may be superscript, circled, etc.) (OK in identifiers) 
                  , alpha                     // - - T -   accented/unaccented alphabetic char (OK in identifiers) 
                  , currency                  // - - T S   currency symbols  $ and others (OK in identifiers??)
                  , sep                       // - - T S   separator characters (OK in numbers, identifiers, etc.) _, etc. 
                  , open_bracket              // - - T -   ( { [  etc.
                  , close_bracket             // - - T -   ) } ]  etc.
                  , quote                     // - - T -   ' "  etc.
                  , open_quote                // - - T -   «  etc.   
                  , close_quote               // - - T -   »  etc.

                  //////////////////////////// subtype only below ////////////////////////////////////

                  , f_continuation            // - - - S  variable purpose char -- function = line continuation   
                  , f_vanishing_separator     // - - - S  variable purpose char -- function = vanishing separator (to prevent combining of adjacent strings)
                  , f_always_sign             // - - - S  variable purpose char -- function = vanishing separator (to prevent combining of adjacent strings)

                  , f_type1_string_start      // - - - S  variable purpose char -- function = starting char for type 1 strings
                  , f_type2_string_start      // - - - S  variable purpose char -- function = starting char for type 2 strings

                  , f_line_comment            // - - - S  variable purpose char -- function = line comment (not echoed) 
                  , f_echoed_line_comment     // - - - S  variable purpose char -- function = echoed line comment
                  , f_comment_1st             // - - - S  variable purpose char -- function = 1st character of comment start sequence

                  , f_leading_sigil           // - - - S  variable purpose char -- function = leading sigil attached directly to an identifier
                      
                  , underscore                // - - - S   _
                  , full_stop                 // - - - S   .
                  , comma                     // - - - S   , 
                  , semicolon                 // - - - S   ;  
                  , colon                     // - - - S   :
                  , hyphen_minus              // - - - S   -
                  , plus                      // - - - S   +
                  , digit                     // - - - S   base 10 digit 0 - 9 
                  , alpha1                    // - - - S   non-accented characters a-z A-Z
                  , alpha2                    // - - - S   accented latin characters
                  , alpha3                    // - - - S   non-latin characters
                  , alpha4                    // - - - S   ideographs, etc. (CJK)
                  , special1                  // - - - S   special characters (007F and below) without special lexical significance -- @ # % & * (non-punctuation)
                  , special2                  // - - - S   special characters (0080 and above) without special lexical significance (non-punctuation)
                  , accent1                   // - - - S   accents (007F and below)
                  , accent2                   // - - - S   accents (0080 and above)
                  , oper1                     // - - - S   operator character (007F and below)    *  =  <  >    etc.  
                  , oper2                     // - - - S   operator character (0080 and above) ¦ etc.  
                  , punct1                    // - - - S   other punctuation (007F and below) ? ! etc.  
                  , punct2                    // - - - S   other punctuation (0080 and above) ¿ ¡ etc.  
                  

                  // open/close paren/brace/brackets, etc.

                  , open_paren1               // - - - S    (          (round) brackets
                  , close_paren1              // - - - S    ) 
                  , open_paren2               // - - - S    
                  , close_paren2              // - - - S   

                  , open_brace1               // - - - S    {          (curly) brackets
                  , close_brace1              // - - - S    }  
                  , open_brace2               // - - - S    
                  , close_brace2              // - - - S   

                  , open_bracket1             // - - - S    [          (square) brackets
                  , close_bracket1            // - - - S    ] 
                  , open_bracket2             // - - - S    
                  , close_bracket2            // - - - S   

                  , open_angle_bracket1       // - - - S              (straight) angle brackets       
                  , close_angle_bracket1      // - - - S                       
                  , open_angle_bracket2       // - - - S                     
                  , close_angle_bracket2      // - - - S                      

                  , open_shell_bracket1       // - - - S               tortoise shell brackets
                  , close_shell_bracket1      // - - - S                       
                  , open_shell_bracket2       // - - - S                     
                  , close_shell_bracket2      // - - - S                      

                  , open_curve_bracket1       // - - - S               curved angle brackets      
                  , close_curve_bracket1      // - - - S                       
                  , open_curve_bracket2       // - - - S                     
                  , close_curve_bracket2      // - - - S                       

                  , open_lens_bracket1        // - - - S               lenticular brackets      
                  , close_lens_bracket1       // - - - S                       
                  , open_lens_bracket2        // - - - S                     
                  , close_lens_bracket2       // - - - S                      

                  , open_corner_bracket1      // - - - S               corner brackets      
                  , close_corner_bracket1     // - - - S                       
                  , open_corner_bracket2      // - - - S                     
                  , close_corner_bracket2     // - - - S      

                  , open_quill_bracket        // - - - S               square brackets with quill      
                  , close_quill_bracket       // - - - S                       
              
                  , open_dot_bracket          // - - - S               angle brackets with dot      
                  , close_dot_bracket         // - - - S                       
             
                  , open_other_bracket        // - - - S               other brackets and bracket-line characters      
                  , close_other_bracket       // - - - S                       
                       

                  // open/close quotes, etc.
                        
                  , both_quote1               // - - - S     '
                  , both_quote2               // - - - S     "
                  , open_quote1               // - - - S                    
                  , close_quote1              // - - - S                    
                  , open_quote2               // - - - S                    
                  , close_quote2              // - - - S          
                  , open_angle_quote1         // - - - S       
                  , close_angle_quote1        // - - - S     
                  , open_angle_quote2         // - - - S     «                 
                  , close_angle_quote2        // - - - S     »               
                  , open_other_quote1         // - - - S       
                  , close_other_quote1        // - - - S     
                  , open_other_quote2         // - - - S                      
                  , close_other_quote2        // - - - S              
                  }; 




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    --------------------
//    in_char_S  structure
//    --------------------
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct in_char_S
{
    char_E          classs              {char_E::none};    // char class -- normal, error, EOF, EOL, etc. 
    char_E          family              {char_E::none};    // char family
    char_E          type                {char_E::none};    // char type  
    char_E          subtype             {char_E::none};    // detailed character type 

    char32_t        ch32                {0           };    // UTF-32 final character (after any trigraph processing)
    wchar_t         wch1                {0           };    // 1st wchar -- can be leading surrogate (after any trigraph processing) 
    wchar_t         wch2                {0           };    // second wchar -- 0000 if not trailing surrogate (after any trigraph processing)

    char32_t        ch32_orig           {0           };    // UTF-32 original 2nd character (before trigraph processing) -- same as final char, if no trigraph
    wchar_t         wch1_orig           {0           };    // 1st wchar -- can be leading surrogate (before trigraph processing) 
    wchar_t         wch2_orig           {0           };    // second wchar -- 0000 if not trailing surrogate (before trigraph processing)

    char32_t        ch32_trigraph       {0           };    // UTF-32 character for trigraph char (if present)
    wchar_t         wch1_trigraph       {0           };    // 1st wchar -- can be leading surrogate (trigraph char) 
    wchar_t         wch2_trigraph       {0           };    // second wchar -- 0000 if not trailing surrogate (trigraph char)

    uint32_t        trigraph_count      {0           };    // 0 -- no trigraph char present, >0 one or more trigraph chars present          
    uint64_t        source_id           {0           };    // unique source_id of source file or string 
    uint32_t        lineno              {0           };    // line number in file/string containing this char (for error messages) 1-based  -- both trigraph and 2nd char must be on same line
    size_t          linepos             {0           };    // position in line of start of this char (2nd one, if trigraph present)
    size_t          linepos_trigraph    {0           };    // position in line of start of this trigraph    

                in_char_S()                 = default;     // default constructor
                in_char_S(const in_char_S&) = default;     // copy constructor
                in_char_S(in_char_S&&)      = default;     // move constructor
               ~in_char_S()                 = default;     // destructor
    in_char_S&  operator=(const in_char_S&) = default;     // copy assignment
    in_char_S&  operator=(in_char_S&&)      = default;     // move assignment
};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  -----------------------------------------------------------
//  token_E -- character classes, families, types, and subtypes
//  -----------------------------------------------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class token_E { none                       // initial state -- no token (yet)
                   , error                      // error has occurred
                   , eof                        // end-of-file reached
                   , end                        // end of all input reached
                   , eol                        // unescaped end-of-line reached
                   , unsupported                // unsupported character seen
                   , invalid                    // invalid number or other token, etc.
                   , whitespace 
                   , line_comment
                   , retained_line_comment
                   , eof_comment
                   , block_comment
                   , retained_block_comment
                   , nestable_comment
                   , full_stop
                   , comma
                   , semicolon
                   , colon
                   , special_char
                   , misc_char
                   , punctuation_char
                   , accent_char
                   , open_paren
                   , close_paren
                   , open_bracket
                   , close_bracket
                   , open_brace
                   , close_brace
                   , open_angle_bracket
                   , close_angle_bracket
                   , open_shell_bracket
                   , close_shell_bracket 
                   , open_quill_bracket
                   , close_quill_bracket 
                   , open_dot_bracket
                   , close_dot_bracket
                   , open_misc_bracket
                   , close_misc_bracket
                   , number
                   , unit
                   , boolean
                   , int8
                   , uint8
                   , int16
                   , uint16
                   , int32
                   , uint32
                   , int64                     
                   , uint64 
                   , float32
                   , float64
                   , identifier
                   , numeric_identifier
                   , extended_identifier
                   , oper
                   , string
                   , vanishing_separator
                   };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ------------------
//    cache_S  structure 
//    ------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct cache_S
{
    uint64_t                  string_ct   {0}; 
    std::vector<std::wstring> strings     { };    // cached strings for source IDs                
};


// associated global functions ( member functions would cause users to type (for example) "global::id_cache_p->get_cached_id(9)" instead of just "get_cached_id(9)" )    

void           display_id_cache();
uint64_t       add_cached_id(const std::wstring&);
std::wstring   get_cached_id(uint64_t);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    --------------
//    token_C  class 
//    --------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class token_C
{
public: 
                    token_C()                  = default;    // default constructor
                    token_C(const token_C&)    = default;    // copy constructor
                    token_C(token_C&&)         = default;    // move constructor 
                   ~token_C()                  = default;    // destructor
    token_C&        operator=(const token_C&)  = default;    // copy assignment
    token_C&        operator=(token_C&&)       = default;    // move assignment 


    // public data members

    token_E         type                         {token_E::none } ;             // token type
    union                                                                       // user-defined token type -- not used by lexer 
    {                                                                          
        tok_ut_E    utype                        {(tok_ut_E)-1  } ;             // initialize to bad value
        tok_u1_E    utype1                                        ;             
        tok_u2_E    utype2                                        ;
        tok_u3_E    utype3                                        ;
        tok_u4_E    utype4                                        ;
        tok_u5_E    utype5                                        ;
        tok_u6_E    utype6                                        ;
        tok_u7_E    utype7                                        ;
        tok_u8_E    utype8                                        ;
        tok_u9_E    utype9                                        ;
    };
    union                                                                        // value -- only if numeric literal
    {                                                                           
        bool        boolean;                                                     // valid only if type is token_E::boolean
        int8_t      int8;                                                        // valid only if type is token_E::int8 
        int16_t     int16;                                                       // valid only if type is token_E::int16 
        int32_t     int32;                                                       // valid only if type is token_E::int32 
        int64_t     int64;                                                       // valid only if type is token_E::int64 
        uint8_t     uint8;                                                       // valid only if type is token_E::uint8 
        uint16_t    uint16;                                                      // valid only if type is token_E::uint16 
        uint32_t    uint32;                                                      // valid only if type is token_E::uint32 
        uint64_t    uint64                       {0             } ;              // valid only if type is token_E::uint64
        float32_T   float32;                                                     // valid only if type is token_E::float32 
        float64_T   float64;                                                     // valid only if type is token_E::float64  
    };                                                            
       
    bool            attached_paren               { false        } ;              // true indicates that this identifier token was ended by an open paren with no whitespace before it 
    bool            has_leading_sigil            { false        } ;              // true indicates leading sigil present
    bool            has_trailing_sigil           { false        } ;              // true indicates trailing sigil present
    char32_t        only_char                    { 0            } ;              // valid only when this is a single character token
    char32_t        leading_sigil                { 0            } ;              // leading sigil character 
    char32_t        trailing_sigil               { 0            } ;              // trailing sigil character 
    std::wstring    str                          {              } ;              // text (lexeme) for the token  (to be passed upward)
    std::wstring    orig_str                     {              } ;              // original string (for error messages) -- has any sigils present, but not any attached parenthesis       
    uint32_t        lineno1                      { 0            } ;              // line number in file containing start of token (for error messages)
    uint32_t        lineno2                      { 0            } ;              // line number in file containing end of token (for error messages)
    size_t          linepos1                     { 0            } ;              // position in line of start of this token 
    size_t          linepos2                     { 0            } ;              // position in line of end of this token  
    uint64_t        source_id1                   { 0            } ;              // source ID for file/string containing start of this token 
    uint64_t        source_id2                   { 0            } ;              // source ID for file/string containing end of this token (intermediate source ID's are lost for multi-source toekns)
    uint64_t        peek_count                   { 0            } ;              // number of times this token has been peek()ed 


    // external functions

    void            display(const std::wstring& = std::wstring(L"token"), bool = false) const;  // display token
    std::wstring    loc_str() const; 


    // internal (private functions)

private: 
    std::wstring type_str() const;                                               // get printable token type (with any sigils)
};






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ---------------
//    infile_C  class 
//    ---------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class infile_C
{
public:
                  infile_C()                    = default;      // default constructor
    explicit      infile_C(const std::string&);                 // string constructor
    explicit      infile_C(const std::wstring&);                // wstring constructor
                  infile_C(const infile_C&)     = delete;       // copy constructor
                  infile_C(infile_C&&)          = delete;       // move constructor
                 ~infile_C();                                   // destructor
    infile_C&     operator=(const infile_C&)    = delete;       // copy assignment
    infile_C&     operator=(infile_C&&)         = delete;       // move assignment


   // external functions

    int           open(const std::wstring&);                    // open file -- wide string
    int           open(const std::string&);                     // open file -- plain string
    int           close();                                      // close file 
    bool          is_open() const;                              // return true if file is open
    int           get_char(in_char_S&, bool=true);              // get next character from file
    std::wstring  get_filename() const;                         // get filename of open file (if any)  
    uint64_t      get_source_id() const;                        // get source_id of open file (if any) 

private:
    std::ifstream*         m_in_file_p     {nullptr};           // current input file
    std::wstring           m_in_filename   {       };           // name for current input file 
    uint64_t               m_source_id     {0      };           // source ID for passing forward                     
    bool                   m_is_open_f     {false  };           // true : current file is open and valid
    bool                   m_is_utf8       {false  };           // true: UTF-8 flag bytes seen at start of file
    bool                   m_line_ok       {false  };           // true: valid data in curr_line (including empty line from file) 
    bool                   m_eof_seen      {false  };           // EOF: seen on read()
    bool                   m_error_seen    {false  };           // fail() bit set after read()
    std::string            m_curr_line     {       };           // current line from file (UTF-8 or 1252)
    std::wstring           m_curr_wline    {       };           // current line from file (wchar_t = UTF-16)
    uint32_t               m_curr_linenum  {0      };           // current line number
    size_t                 m_curr_col      {0      };           // current column within line
    size_t                 m_curr_pos      {0      };           // current position (offset) in current wline 
    size_t                 m_end_pos       {0      };           // end offset (of line -- one byte past the end of data 
    

    // internal (private) functions

    int         fetch_char(in_char_S&,         bool=true);      // fetch next char in curr_line 
    void        out_char(  in_char_S&, char_E, bool=true);      // set up in_char_S for output char based on passed-in type
};




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    -----------------
//    instring_C  class 
//    -----------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class instring_C
{
public:
                  instring_C()                    = default;                         // default constructor
                  instring_C(const std::string& , const std::string& );              // string constructor (string is the name to be used in error messages)
                  instring_C(const std::string& , const std::wstring&);              // string constructor (wstring is the name to be used in error messages)
                  instring_C(const std::wstring&, const std::wstring&);              // string constructor (wstring is the name to be used in error messages)
                  instring_C(const instring_C&)   = delete;                          // copy constructor
                  instring_C(instring_C&&)        = delete;                          // move constructor
                 ~instring_C();                                                      // destructor
    instring_C&   operator=(const instring_C&)    = delete;                          // copy assignment
    instring_C&   operator=(instring_C&&)         = delete;                          // move assignment
      

    // external functions

    int           open(const std::string& , const std::string&);                     // open string -- input string , string for string name
    int           open(const std::string& , const std::wstring&);                    // open string -- input string , wstring for string name
    int           open(const std::wstring&, const std::wstring&);                    // open string -- input wstring, wstring for string name
    int           close();                                                           // close string
    bool          is_open() const;                                                   // return true if string is open
    int           get_char(in_char_S&, bool=true);                                   // get next character from string
    std::wstring  get_name() const;                                                  // get name of open string (if any) 
    uint64_t      get_source_id() const;                                             // get source_id of open file (if any) 

private:
    std::wistringstream    m_in_string    {      };                                  // current input string
    std::wstring           m_in_name      {      };                                  // name for current input string (for error messages) 
    uint64_t               m_source_id    {0     };                                  // source ID for passing forward 
    bool                   m_is_open_s    {false };                                  // true : current string is valid
    bool                   m_line_ok      {false };                                  // true: valid data in curr_line (including empty line from string) 
    bool                   m_eof_seen     {false };                                  // EOF: seen at end of string
    bool                   m_error_seen   {false };                                  // error fetching from string
    std::wstring           m_curr_wline   {      };                                  // current line from string (wchar_t = UTF-16)
    uint32_t               m_curr_linenum {0     };                                  // current line number
    size_t                 m_curr_col     {0     };                                  // current column within line
    size_t                 m_curr_pos     {0     };                                  // current position (offset) in current wline 
    size_t                 m_end_pos      {0     };                                  // end offset (of line -- one byte past the end of data 
    

    // internal (private) functions 

    int         reset();                                                             // close string
    int         fetch_char(in_char_S&        , bool=true);                           // fetch next char in curr_line 
    void        out_char(  in_char_S&, char_E, bool=true);                           // set up in_char_S for output char based on passed-in type
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    -------------------
//    char_stream_C class 
//    -------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class char_stream_C
{
public:
                    char_stream_C()                      = default;     // default constructor
                    char_stream_C(const char_stream_C&)  = delete;      // copy constructor
                    char_stream_C(char_stream_C&&)       = delete;      // move constructor
                   ~char_stream_C()                      = default;     // destructor
    char_stream_C&  operator=(const char_stream_C&)      = delete;      // copy assignment
    char_stream_C&  operator=(char_stream_C&&)           = delete;      // move assignment
    

    // external functions

    void            attach_file(           infile_C *                       )  ;        // assign pre-opened file to this char stream (file is not owned here)
    void            attach_string(         instring_C *                     )  ;        // assign pre-filled string to this char stream 
    void            set_trigraph_char(     char32_t = const_N::ch_trigraph  )  ;        // set trigraph char                  
    void            set_process_trigraphs( bool = true                      )  ;        // enable/disable trigraph processing -- default is enabled
    int             get_char(              in_char_S&                       )  ;        // get (and consume) next character from stream (may return previously put-bach char)  
    int             get_source_char(       in_char_S&                       )  ;        // get (and consume) next character from source  
    int             peek_char(             in_char_S&, size_t = 1ULL        )  ;        // return next (or n-th next) char from stream but do not consume it -- leave for next time  
    void            putback_char(    const in_char_S&                       )  ;        // putback character to stream 
    int             discard_char(          size_t = 1ULL                    )  ;        // Discard top n chars on putback queue (if any) R/C is -1 if not enough on queue 
    void            close(                                              )      ;        // free up storage, etc. (does not close files, which are not owned here) 

    bool            is_empty(                                           ) const;        // return true, if put-back character queue is empty
    size_t          size(                                               ) const;        // return number of put-back characters currently queued up

private:
    bool                   m_process_trigraphs        { true                   };       // enable/disable trigraph processing 
    char32_t               m_trigraph_char            { const_N::ch_trigraph   };       // trigraph char                                         
    uint64_t               m_source_id                { 0                      };       // unique source ID for current file or string
    infile_C              *m_in_file_p                { nullptr                };       // current in_file for the stream -- can be 0, if reading from string
    instring_C            *m_in_string_p              { nullptr                };       // current in_string for the stream -- can be 0, if reading from file
    std::wstring           m_in_filename              {                        };       // name for current input file/string --  
    std::deque<in_char_S>  m_char_stack               {                        };       // stack of characters (empty, unless putback has been done)    
};




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ----------------
//    instream_C class       (there is a queue of these in the token_stream_C structure)
//    ----------------
//
//
//    - the instream_C structure should point to either an infile_C or an instring_C object, but not both
//
//    - there is no code (member functions) for this class 
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class instream_C
{
public:
                 instream_C()                   = default;    // default constructor
                 instream_C(const instream_C&)  = default;    // copy constructor
                 instream_C(instream_C&&)       = default;    // move constructor 
                ~instream_C()                   = default;    // destructor
    instream_C&  operator=(const instream_C&)   = default;    // copy assignment
    instream_C&  operator=(instream_C&&)        = default;    // move assignment 

    infile_C*                infile_p         {nullptr};      // pointer to infile_C  for this character stream  (if any -- can be 0)
    instring_C*              instring_p       {nullptr};      // pointer to insting_C for this character stream  (if any -- can be 0)
    char_stream_C*           char_stream_p    {nullptr};      // pointer to char_stream_C 
    std::deque<token_C>      token_stack      {       };      // saved token stack from old token_stream_C -- when this token stream was added to queue  
    std::deque<token_C>      raw_token_stack  {       };      // saved raw_token_stack from token_stream_C -- when this token stream was added to queue 
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    --------------------
//    token_stream_C class 
//    --------------------
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class token_stream_C
{
public: 
                     token_stream_C()                       = default;                                               // default constructor
                     token_stream_C(const token_stream_C&)  = delete;                                                // copy constructor
                     token_stream_C(token_stream_C&&)       = delete;                                                // move constructor
                    ~token_stream_C();                                                                               // destructor
    token_stream_C&  operator=(const token_stream_C&)       = delete;                                                // copy assignment
    token_stream_C&  operator=(token_stream_C&&)            = delete;                                                // move assignment
   

    // external functions

    void             close();                                                                                        // clean everything out -- leave configutation set -- don't reset statistics
    void             refresh();                                                                                      // refresh token stram for reuse, perhaps after error 
    int              attach_file(  const std::wstring&                      );                                       // add new file to this token stream -- placed on top of filestack -- wide string filename version
    int              attach_file(  const std::string&                       );                                       // add new file to this token stream -- placed on top of filestack -- "plain" string filename version
    int              attach_string(const std::wstring&, const std::wstring& );                                       // add new string to this token stream -- placed on top of filestack -- wide string version
    int              attach_string(const std::string& , const std::wstring& );                                       // add new string to this token stream -- placed on top of filestack -- wide string name version
    int              attach_string(const std::string& , const std::string&  );                                       // add new string to this token stream -- placed on top of filestack -- "plain" string name version
    int              get_token(           token_C&                );                                                 // get (and consume) next composite token from stream  
    int              peek_token(          token_C&, size_t = 1ULL );                                                 // return n-th composite token from stream/stack but do not consume it -- leave for next time  
    void             putback_token(const token_C&                 );                                                 // putback newest composite token to stream/stack 
    int              discard_token(                 size_t = 1ULL );                                                 // Discard n-th composite token on putback queue (if any) R/C is -1 if putback queue not large enough 
    bool             is_empty()     const;                                                                           // return true, if put-back token queue is empty
    size_t           putback_size() const;                                                                           // return number of put-back tokens currently queued up 
    uint64_t         errors()       const;                                                                           // return number of errors so far  


    // functions to set configuration parms

    void             set_quiet_mode(                      bool     = true                                    );      // set/reset quiet mode to suppress/show error messages 
    void             set_suppress_echo(                   bool     = true                                    );      // set/reset line comment echo suppression
    void             set_eol_is_whitespace(               bool     = true                                    );      // set/reset "eol is whitespace" flag   
    void             set_eof_is_whitespace(               bool     = true                                    );      // set/reset "eof is whitespace" flag 
    void             set_comment_is_whitespace(           bool     = true                                    );      // set/reset "comment is whitespace" flag                           
    void             set_combine_whitespace(              bool     = true                                    );      // set/reset flag to cause consecutive whitespace tokens to be combined                       
    void             set_combine_strings(                 bool     = true                                    );      // set/reset flag to cause consecutive strings to get combined                               
    void             set_skip_whitespace(                 bool     = true                                    );      // set/reset flag to cause whotespace tokens to not be passed back                            
    void             set_allow_num_identifier(            bool     = true                                    );      // set/reset flag to allow numeric identifiers (11J, etc.) 
    void             set_allow_extd_identifier(           bool     = true                                    );      // set/reset flag to allow extended identifiers ($abc, etc.) 
    void             set_report_extd_identifier(          bool     = true                                    );      // set/reset flag to cause reporting of extended vs standard identifiers  
    void             set_preserve_sigils(                 bool     = true                                    );      // set/reset flag to preserve sigils in passed-back identifier tokens  
    void             set_always_attach_plus_minus(        bool     = true                                    );      // set/reset flag to regard all directly attached plus and minus, as sign for negative/positive literals 
    void             set_never_attach_plus_minus(         bool     = true                                    );      // set/reset flag to prevent + or - from being attached to numeric literals as the sign 
    void             set_allow_leading_op_sigils(         bool     = true                                    );      // set/reset flag to allow operator   tokens to have leading  sigils 
    void             set_allow_trailing_op_sigils(        bool     = true                                    );      // set/reset flag to allow operator   tokens to have trailing sigils 
    void             set_allow_leading_id_sigils(         bool     = true                                    );      // set/reset flag to allow identifier tokens to have leading  sigils 
    void             set_allow_trailing_id_sigils(        bool     = true                                    );      // set/reset flag to allow identifier tokens to have trailing sigils 
    void             set_allow_paren_sigils(              bool     = true                                    );      // set/reset flag to allow parentheis tokens to have leading/trailing sigils
    void             set_allow_attached_paren(            bool     = true                                    );      // set/reset flag to allow attached parentheis to identifier tokens (token role is verb)


    //               configurable character setting functions  

    void             set_trigraph_char(                   char32_t = const_N::ch_trigraph                    );      // set trigraph char                  
    void             set_vanishing_separator_char(        char32_t = const_N::ch_vanishing_sep               );      // set vanishing separator char  
    void             set_line_continuation_char(          char32_t = const_N::ch_line_continuation           );      // set line continuation character  
    void             set_always_sign_char(                char32_t = const_N::ch_always_sign                 );      // set always-sign character  

    void             set_type1_string_start_char(         char32_t = const_N::ch_type1_string_start          );      // open  quotes for type1 string  
    void             set_type2_string_start_char(         char32_t = const_N::ch_type2_string_start          );      // open  quotes for type2 string  
    void             set_type1_string_end_char(           char32_t = const_N::ch_type1_string_end            );      // close quotes for type1 string  
    void             set_type2_string_end_char(           char32_t = const_N::ch_type2_string_end            );      // close quotes for type2 string 
    void             set_type1_string_escape_char(        char32_t = const_N::ch_type1_string_escape         );      // set 1st type of string escape     
    void             set_type2_string_escape_char(        char32_t = const_N::ch_type2_string_escape         );      // set 2nd type of string escape 
    void             set_raw_string_prefix_char(          char32_t = const_N::ch_raw_string_prefix           );      // prefix char for raw string  
    void             set_multiline_string_prefix_char(    char32_t = const_N::ch_multiline_string_prefix     );      // prefix char for multiline string  
    void             set_word_string_start_char(          char32_t = const_N::ch_type2_string_start          );      // open  quote for word string  

    void             set_unechoed_line_comment_char(      char32_t = const_N::ch_unechoed_line_comment       );      // set line comment character        
    void             set_echoed_line_comment_char(        char32_t = const_N::ch_echoed_line_comment         );      // set echoed line comment char      

    void             set_comment_1st_char(                char32_t = const_N::ch_comment_1st                 );      // set comment_1st                char 
    void             set_unechoed_line_comment_2nd_char(  char32_t = const_N::ch_unechoed_line_comment_2nd   );      // set line_comment_2nd           char 
    void             set_echoed_line_comment_2nd_char(    char32_t = const_N::ch_echoed_line_comment_2nd     );      // set echoed_line_comment_2nd    char 
    void             set_suppress_eol_comment_2nd_char(   char32_t = const_N::ch_suppress_eol_comment_2nd    );      // set suppress_eol_comment_2nd   char 
    void             set_eof_comment_2nd_char(            char32_t = const_N::ch_eof_comment_2nd             );      // set eof_comment_2nd            char 
    void             set_retained_line_comment_2nd_char(  char32_t = const_N::ch_retained_line_comment_2nd   );      // set retained_line_comment_2nd  char 

    void             set_block_comment_2nd_char(          char32_t = const_N::ch_block_comment_2nd           );      // set block_comment_2nd          char 
    void             set_block_comment_3rd_char(          char32_t = const_N::ch_block_comment_3rd           );      // set block_comment_3rd          char 
    void             set_block_comment_4th_char(          char32_t = const_N::ch_block_comment_4th           );      // set block_comment_4th          char 
    void             set_nest_comment_2nd_char(           char32_t = const_N::ch_nest_comment_2nd            );      // set nest_comment_2nd           char 
    void             set_nest_comment_3rd_char(           char32_t = const_N::ch_nest_comment_3rd            );      // set nest_comment_3rd           char 
    void             set_nest_comment_4th_char(           char32_t = const_N::ch_nest_comment_4th            );      // set nest_comment_4th           char 
    void             set_retained_block_comment_2nd_char( char32_t = const_N::ch_retained_block_comment_2nd  );      // set retained_block_comment_2nd char 
    void             set_retained_block_comment_3rd_char( char32_t = const_N::ch_retained_block_comment_3rd  );      // set retained_block_comment_3rd char 
    void             set_retained_block_comment_4th_char( char32_t = const_N::ch_retained_block_comment_4th  );      // set retained_block_comment_4th char  
                                                                                                                    
    void             set_leading_sigils(                  const std::vector<char32_t>&                       );      // set vector with sigils that can be attached to front      of identifiers, operators, and parens   
    void             set_leading_ident_sigils(            const std::vector<char32_t>&                       );      // set vector with sigils that can be attached to front      of identifiers    
    void             set_trailing_ident_sigils(           const std::vector<char32_t>&                       );      // set vector with sigils that can be attached to back       of identifiers  
    void             set_leading_oper_sigils(             const std::vector<char32_t>&                       );      // set vector with sigils that can be attached to front      of identifiers    
    void             set_trailing_oper_sigils(            const std::vector<char32_t>&                       );      // set vector with sigils that can be attached to back       of identifiers 
    void             set_paren_sigils(                    const std::vector<char32_t>&                       );      // set vector with sigils that can be attached to front/back of parenthesis 


private:
    std::stack<instream_C*>  m_instream_stack                {       };                                              // stack of pointers to instreams for this token stream
    std::wstring             m_in_filename                   {       };                                              // name for current input file (for error messages)
    uint64_t                 m_source_id                     {0      };                                              // source ID for passing forward 
    instream_C              *m_instream_p                    {nullptr};                                              // pointer to current instream_C for character input operations 
    char_stream_C           *m_char_stream_p                 {nullptr};                                              // pointer to current char_stream for character input operations 
    std::deque<token_C>      m_token_stack                   {       };                                              // stack of (composite) tokens (that have been put back or peek()ed) 
    std::deque<token_C>      m_raw_token_stack               {       };                                              // stack of raw tokens (0 or 1 token normally) for use when combining tokens  
    bool                     m_error_seen                    {false  };                                              // true means prior I/O error has been seen 
    bool                     m_attach_mode                   {true   };                                              // next + or - will be attached to numeric literal, not infix operator
    uint64_t                 m_error_ct                      {0      };                                              // number of errors seen (may not have been passed back yet, or may have been put back) 
                                                                                                                   

    //                       configuration parms

    int                      m_max_instream_nest              {100  };                                               // maximum number of queued instream_C objects in the instream_stack
    bool                     m_quiet_mode                     {false};                                               // true == suppress error messages 
    bool                     m_suppress_echo                  {false};                                               // true == suppress echoed line comments 
    bool                     m_eol_is_whitespace              {false};                                               // treat EOL as whitespace 
    bool                     m_eof_is_whitespace              {false};                                               // treat EOF as whitespace 
    bool                     m_comment_is_whitespace          {false};                                               // treat comments as whitespace 
    bool                     m_combine_whitespace             {false};                                               // combine consecutive whitespace tokens into a single token 
    bool                     m_combine_strings                {false};                                               // combine consecutive strings (separated only by whitespace) into one longer string
    bool                     m_skip_whitespace                {false};                                               // do not return whitespace tokens at all
    bool                     m_allow_num_identifier           {false};                                               // allow numeric identifiers like   11J   (but not +11J or -11J or 7.11J) 
    bool                     m_allow_extd_identifier          {false};                                               // allow extended identifiers like   $abc
    bool                     m_report_extd_identifier         {false};                                               // distinguish between plain and extended identifiers (xx$xx vs xxxxx)  
    bool                     m_preserve_sigils                {false};                                               // keep sigils attached to returned identifier tokens
    bool                     m_always_attach_plus_minus       {false};                                               // all directly attached plus and minus signs in front of numeric literals are considered signs  
    bool                     m_never_attach_plus_minus        {false};                                               // don't attach + or - to start of numeric literals as the sign 
    bool                     m_allow_leading_id_sigils        {false};                                               // true: allow leading          sigil characters on identifier    tokens 
    bool                     m_allow_trailing_id_sigils       {false};                                               // true: allow trailing         sigil characters on identifier    tokens 
    bool                     m_allow_leading_op_sigils        {false};                                               // true: allow leading          sigil characters on operator      tokens 
    bool                     m_allow_trailing_op_sigils       {false};                                               // true: allow trailing         sigil characters on operator      tokens 
    bool                     m_allow_paren_sigils             {false};                                               // true: allow leading/trailing sigil characters on parenthesis   tokens 
    bool                     m_allow_attached_paren           {false};                                               // true: allow attached parenthesis to identifier tokens -- token is treated as a verb 
        

    //                       configurable characters

    char32_t                 m_trigraph_char                  {const_N::ch_trigraph                    };            // trigraph char                                         
    char32_t                 m_line_continuation_ch           {const_N::ch_line_continuation           };            // line continuation char  
    char32_t                 m_always_sign_ch                 {const_N::ch_always_sign                 };            // always_sign character (like macron) -- always becomes part of signed integer                                           

    char32_t                 m_type1_string_start_ch          {const_N::ch_type1_string_start          };            // starting character for type1 quoted string                     
    char32_t                 m_type2_string_start_ch          {const_N::ch_type2_string_start          };            // starting character for type2 quoted string
    char32_t                 m_type1_string_end_ch            {const_N::ch_type1_string_end            };            //   ending character for type1 quoted string                     
    char32_t                 m_type2_string_end_ch            {const_N::ch_type2_string_end            };            //   ending character for type2 quoted string
    char32_t                 m_type1_string_escape_ch         {const_N::ch_type1_string_escape         };            // escape character in most quoted strings  --  "" etc  
    char32_t                 m_type2_string_escape_ch         {const_N::ch_type2_string_escape         };            // escape character in other quoted strings --  «»       
    char32_t                 m_raw_string_prefix_ch           {const_N::ch_raw_string_prefix           };            // prefix character for raw string:         R"  "     or    R«    » 
    char32_t                 m_multiline_string_prefix_ch     {const_N::ch_multiline_string_prefix     };            // prefix character for multiline string:   M"  "     or    M«    »  
    char32_t                 m_word_string_start_ch           {const_N::ch_word_string_start           };            // start character for word string:         'words 

    //                       configurable one-character comment starters                                            
                                                                                                       
    char32_t                 m_unechoed_line_comment_ch       {const_N::ch_unechoed_line_comment       };            // line comment char (no echo)                          
    char32_t                 m_echoed_line_comment_ch         {const_N::ch_echoed_line_comment         };            // echoed line comment char                             
    char32_t                 m_vanishing_separator_ch         {const_N::ch_vanishing_sep               };            // vanishing (string) separator character 
                                    

    //                       configurable two-character comment markers                                             
                                                                                                       
    char32_t                 m_comment_1st_ch                 {const_N::ch_comment_1st                 };            // 1st char in comment starting sequence (1st "/" in "//", "/*", "/<", etc., for example) 
    char32_t                 m_unechoed_line_comment_2nd_ch   {const_N::ch_unechoed_line_comment_2nd   };            // 2nd char in line comment starting sequence (2nd "/" in "//", for example) 
    char32_t                 m_echoed_line_comment_2nd_ch     {const_N::ch_echoed_line_comment_2nd     };            // 2nd char in echoed line comment starting sequence 
    char32_t                 m_suppress_eol_comment_2nd_ch    {const_N::ch_suppress_eol_comment_2nd    };            // 2nd char in starting sequence for comment that suppresses the EOL
    char32_t                 m_eof_comment_2nd_ch             {const_N::ch_eof_comment_2nd             };            // 2nd char in EOF-ended      comment starting sequence     ( "~" in "/~", for example)
    char32_t                 m_retained_line_comment_2nd_ch   {const_N::ch_retained_line_comment_2nd   };            // 2nd char in retained line  comment starting sequence     ( "#" in "/#", for example)

    char32_t                 m_block_comment_2nd_ch           {const_N::ch_block_comment_2nd           };            // 2nd char in block          comment starting sequence     ( "*" in "/*", for example)
    char32_t                 m_block_comment_3rd_ch           {const_N::ch_block_comment_3rd           };            // 3rd char in block          comment starting sequence     ( "*" in "*/", for example)
    char32_t                 m_block_comment_4th_ch           {const_N::ch_block_comment_4th           };            // 4th char in block          comment starting sequence     ( "/" in "*/", for example)
    char32_t                 m_nest_comment_2nd_ch            {const_N::ch_nest_comment_2nd            };            // 2nd char in nestable block comment starting sequence     ( "<" in "/<", for example)
    char32_t                 m_nest_comment_3rd_ch            {const_N::ch_nest_comment_3rd            };            // 3rd char in nestable block comment starting sequence     ( ">" in ">/", for example)
    char32_t                 m_nest_comment_4th_ch            {const_N::ch_nest_comment_4th            };            // 4th char in nestable block comment starting sequence     ( "/" in ">/", for example)    
    char32_t                 m_retained_block_comment_2nd_ch  {const_N::ch_retained_block_comment_2nd  };            // 2nd char in retained block comment starting sequence     ( "{" in "/{", for example)
    char32_t                 m_retained_block_comment_3rd_ch  {const_N::ch_retained_block_comment_3rd  };            // 3rd char in retained block comment starting sequence     ( "}" in "}/", for example)
    char32_t                 m_retained_block_comment_4th_ch  {const_N::ch_retained_block_comment_4th  };            // 4th char in retained block comment starting sequence     ( "/" in "}/", for example)


    //                       configurable lists of sigil characters

    std::vector<char32_t>    m_leading_sigils                 {     };                                               // vector with all allowed leading sigil characters                           -- default is no sigils                               
    std::vector<char32_t>    m_leading_ident_sigils           {     };                                               // vector with all allowed leading          sigil characters for identifiers  -- default is no sigils                               
    std::vector<char32_t>    m_trailing_ident_sigils          {     };                                               // vector with all allowed trailing         sigil characters for identifiers  -- default is no sigils 
    std::vector<char32_t>    m_leading_oper_sigils            {     };                                               // vector with all allowed leading          sigil characters for operators    -- default is no sigils                               
    std::vector<char32_t>    m_trailing_oper_sigils           {     };                                               // vector with all allowed trailing         sigil characters for operators    -- default is no sigils 
    std::vector<char32_t>    m_paren_sigils                   {     };                                               // vector with all allowed leading/trailing sigil characters for parentheiss  -- default is no sigils 
             

    // internal (private) functions

    int              attach_string_common(instring_C *                                                      );       // common code for the attach_string() functions
    int              get_char(          in_char_S&                                                          );       // call char_stream->get_char(), if char_stream_p is not nullptr  
    int              peek_char(         in_char_S&,   size_t = 1ULL                                         );       // call char_stream->get_char(), if char_stream_p is not nullptr  
    void             putback_char(const in_char_S&                                                          );       // call char_stream->putback_char(), if char_stream_p is not nullptr
    int              discard_char(                    size_t = 1ULL                                         );       // call char_stream->discard_char(), if char_stream_p is not nullptr
                                                                                                                 
    int              fetch_raw_token(        token_C&                                                       );       // build (uncombined) token from input characters from file 
    int              fetch_pop_raw_token(    token_C&                                                       );       // call fetch_raw_token() or get raw token from raw token stack 
    int              fetch_composite_token(  token_C&                                                       );       // build (uncombined) token from input characters from file 
    int              fetch_non_whitespace(   token_C&                                                       );       // helper function for fetch_composite token 
    void             start_token(            token_C&, token_E                                              );       // start a token of the specified type
    int              addto_token(            token_C&,          const in_char_S&                            );       // append char to token (both orig_str and str)
    int              addto_token_subst(      token_C&,          const in_char_S&, const std::wstring&       );       // append char to token (str only) -- substitute string provided
    int              addto_token_subst(      token_C&,          const in_char_S&, char32_t                  );       // append char to token (str only) -- substitute code point provided
    void             addto_orig_token(       token_C&,          const in_char_S&                            );       // append char to token (only orig_str)
    void             add_leading_sigil(      token_C&,          const in_char_S&                            );       // add leading  sigil character to passed-in token
    void             add_trailing_sigil(     token_C&,          const in_char_S&                            );       // add trailing sigil character to passed-in token
    int              past_end_token(         token_C&,          const in_char_S&                            );       // went past end of token -- put char back, handle errors
    void             one_char_token(         token_C&, token_E, const in_char_S&                            );       // make a complete token of the specified type 
    void             escaped_char_token(     token_C&, token_E, const in_char_S&, const in_char_S&, char32_t);       // make complete token with escaped char
    int              get_integer_value(      token_C&, int, bool, int = 10                                  );       // convert digit string in token to integer value in proper token value field  
    int              numeric_literal(        token_C&                                                       );       // gather up numeric literal token 
    int              based_numeric_literal(  token_C&                                                       );       // gather up based numeric literal  
    int              operator_token(         token_C&,                             bool = false             );       // gather up a operator token 
    int              same_operator_token(    token_C&,          const in_char_S&,  bool = false             );       // gather up a dot/comma, etc. operator token -- all characters are same in operator name
    int              line_comment(           token_C&, bool, bool = false, size_t = 1                       );       // gather up a line comment, with/without echo  
    int              retained_line_comment(  token_C&                                                       );       // gather up a retained line comment (passed up to parser) 
    int              block_comment(          token_C&                                                       );       // gather up a block comment 
    int              nestable_comment(       token_C&                                                       );       // gather up a nestable block comment
    int              retained_block_comment( token_C&                                                       );       // gather up a retained block comment (passed up to parser) 
    int              eof_comment(            token_C&                                                       );       // gather up a EOF comment 
    int              whitespace_token(       token_C&                                                       );       // gather up whitespace token 
    int              identifier_token(       token_C&, bool = false                                         );       // gather up identifier token, with/without leading sigil 
    int              word_string(            token_C&                                                       );       // gather up single-word string
    int              simple_string(          token_C&, char32_t                                             );       // gather up simple string
    int              escaped_string(         token_C&, char32_t, char32_t,         bool = false             );       // gather up string with escapes
    void             invalid_token(          token_C&, const wchar_t *                                      );       // mark token as invalid, and put out token error message  
                                                                                                                     
    void             close_all_instreams();                                                                          // close all in_file_C and in_string_C items on the instream_C stack 
    void             close_current_instream();                                                                       // close current in_file_C item at top of file_stack
};                                                                                                      



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//      global free functions  

char_E        char_subtype(char32_t);
char_E        char_type(   char32_t);
            

//      global functions ot get lexer statistical counters  

uint64_t get_raw_token_count(void);
uint64_t get_character_count(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////