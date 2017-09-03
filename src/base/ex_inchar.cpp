// ex_inchar.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     ex_inchar.cpp -- functions, etc. for char_S and char_stream_C 
////     =============
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       local static items
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace static_N
{
static uint64_t character_count      {0};       // number of characters passed up from input reader to tokenizer 
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//       global functions to access statistrical counters 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint64_t get_character_count(void     ) {return static_N::character_count;       }




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////      +----------------------------------------+
////      | Static function to classify characters |
////      +----------------------------------------+
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  ------------------------------------------------------------
//  Fast classification table for chars 0x00000000 to 0x000000FF 
//  ------------------------------------------------------------

static const char_E subtype_table[256]
{
    char_E::unsupported,              // 00 -- NUL 
    char_E::unsupported,              // 01 --   
    char_E::unsupported,              // 02 --  
    char_E::unsupported,              // 03 --     
    char_E::unsupported,              // 04 --      
    char_E::unsupported,              // 05 --  
    char_E::unsupported,              // 06 --  
    char_E::unsupported,              // 07 -- BEL  
    char_E::unsupported,              // 08 -- BS 
    char_E::whitespace,               // 09 -- HT 
    char_E::whitespace,               // 0A -- LF
    char_E::whitespace,               // 0B -- VT
    char_E::whitespace,               // 0C -- FF
    char_E::whitespace,               // 0D -- CR
    char_E::unsupported,              // 0E -- SO
    char_E::unsupported,              // 0F -- SI
    char_E::unsupported,              // 10 
    char_E::unsupported,              // 11 
    char_E::unsupported,              // 12
    char_E::unsupported,              // 13
    char_E::unsupported,              // 14
    char_E::unsupported,              // 15
    char_E::unsupported,              // 16
    char_E::unsupported,              // 17
    char_E::unsupported,              // 18
    char_E::unsupported,              // 19
    char_E::unsupported,              // 1A
    char_E::unsupported,              // 1B
    char_E::unsupported,              // 1C
    char_E::unsupported,              // 1D
    char_E::unsupported,              // 1E
    char_E::unsupported,              // 1F
    char_E::whitespace,               // 20 -- SP
    char_E::oper1,                    // 21 -- !
    char_E::both_quote2,              // 22 -- "
    char_E::oper1,                    // 23 -- #
    char_E::currency,                 // 24 -- $
    char_E::oper1,                    // 25 -- %
    char_E::oper1,                    // 26 -- &
    char_E::both_quote1,              // 27 -- '
    char_E::open_paren1,              // 28 -- (
    char_E::close_paren1,             // 29 -- )
    char_E::oper1,                    // 2A -- *
    char_E::plus,                     // 2B -- +
    char_E::comma,                    // 2C -- ,
    char_E::hyphen_minus,             // 2D -- -
    char_E::full_stop,                // 2E -- .
    char_E::oper1,                    // 2F -- /
    char_E::digit,                    // 30 -- 0
    char_E::digit,                    // 31 -- 1
    char_E::digit,                    // 32 -- 2
    char_E::digit,                    // 33 -- 3
    char_E::digit,                    // 34 -- 4
    char_E::digit,                    // 35 -- 5
    char_E::digit,                    // 36 -- 6
    char_E::digit,                    // 37 -- 7
    char_E::digit,                    // 38 -- 8
    char_E::digit,                    // 39 -- 9
    char_E::colon,                    // 3A -- :             
    char_E::semicolon,                // 3B -- ;
    char_E::oper1,                    // 3C -- <
    char_E::oper1,                    // 3D -- =
    char_E::oper1,                    // 3E -- >
    char_E::oper1,                    // 3F -- ?
    char_E::oper1,                    // 40 -- @
    char_E::alpha1,                   // 41 -- A
    char_E::alpha1,                   // 42 -- B
    char_E::alpha1,                   // 43 -- C
    char_E::alpha1,                   // 44 -- D
    char_E::alpha1,                   // 45 -- E
    char_E::alpha1,                   // 46 -- F
    char_E::alpha1,                   // 47 -- G
    char_E::alpha1,                   // 48 -- H
    char_E::alpha1,                   // 49 -- I
    char_E::alpha1,                   // 4A -- J
    char_E::alpha1,                   // 4B -- K
    char_E::alpha1,                   // 4C -- L
    char_E::alpha1,                   // 4D -- M
    char_E::alpha1,                   // 4E -- N
    char_E::alpha1,                   // 4F -- O
    char_E::alpha1,                   // 50 -- P
    char_E::alpha1,                   // 51 -- Q
    char_E::alpha1,                   // 52 -- R
    char_E::alpha1,                   // 53 -- S
    char_E::alpha1,                   // 54 -- T
    char_E::alpha1,                   // 55 -- U
    char_E::alpha1,                   // 56 -- V
    char_E::alpha1,                   // 57 -- W
    char_E::alpha1,                   // 58 -- X
    char_E::alpha1,                   // 59 -- Y
    char_E::alpha1,                   // 5A -- Z
    char_E::open_bracket1,            // 5B -- [
    char_E::oper1,                    // 5C -- \ 
    char_E::close_bracket1,           // 5D -- ]
    char_E::oper1,                    // 5E -- ^
    char_E::underscore,               // 5F -- _
    char_E::accent1,                  // 60 -- `
    char_E::alpha1,                   // 61 -- a
    char_E::alpha1,                   // 62 -- b
    char_E::alpha1,                   // 63 -- c
    char_E::alpha1,                   // 64 -- d
    char_E::alpha1,                   // 65 -- e
    char_E::alpha1,                   // 66 -- f
    char_E::alpha1,                   // 67 -- g
    char_E::alpha1,                   // 68 -- h
    char_E::alpha1,                   // 69 -- i
    char_E::alpha1,                   // 6A -- j
    char_E::alpha1,                   // 6B -- k
    char_E::alpha1,                   // 6C -- l
    char_E::alpha1,                   // 6D -- m
    char_E::alpha1,                   // 6E -- n
    char_E::alpha1,                   // 6F -- o
    char_E::alpha1,                   // 70 -- p
    char_E::alpha1,                   // 71 -- q
    char_E::alpha1,                   // 72 -- r
    char_E::alpha1,                   // 73 -- s
    char_E::alpha1,                   // 74 -- t
    char_E::alpha1,                   // 75 -- u
    char_E::alpha1,                   // 76 -- v
    char_E::alpha1,                   // 77 -- w
    char_E::alpha1,                   // 78 -- x
    char_E::alpha1,                   // 79 -- y
    char_E::alpha1,                   // 7A -- z
    char_E::open_brace1,              // 7B -- {
    char_E::oper1,                    // 7C -- |
    char_E::close_brace1,             // 7D -- }
    char_E::oper1,                    // 7E -- ~
    char_E::unsupported,              // 7F -- DEL
    char_E::unsupported,              // 80  
    char_E::unsupported,              // 81    
    char_E::unsupported,              // 82   
    char_E::unsupported,              // 83      
    char_E::unsupported,              // 84       
    char_E::unsupported,              // 85   
    char_E::unsupported,              // 86   
    char_E::unsupported,              // 87   
    char_E::unsupported,              // 88  
    char_E::unsupported,              // 89  
    char_E::unsupported,              // 8A 
    char_E::unsupported,              // 8B 
    char_E::unsupported,              // 8C 
    char_E::unsupported,              // 8D 
    char_E::unsupported,              // 8E 
    char_E::unsupported,              // 8F 
    char_E::unsupported,              // 90 
    char_E::unsupported,              // 91 
    char_E::unsupported,              // 92
    char_E::unsupported,              // 93
    char_E::unsupported,              // 94
    char_E::unsupported,              // 95
    char_E::unsupported,              // 96
    char_E::unsupported,              // 97
    char_E::unsupported,              // 98
    char_E::unsupported,              // 99
    char_E::unsupported,              // 9A
    char_E::unsupported,              // 9B
    char_E::unsupported,              // 9C
    char_E::unsupported,              // 9D
    char_E::unsupported,              // 9E
    char_E::unsupported,              // 9F
    char_E::unsupported,              // A0 --    no break space
    char_E::punct2,                   // A1 -- ¡  inverted exclamation mark
    char_E::currency,                 // A2 --    cent  
    char_E::currency,                 // A3 --    pound sterling
    char_E::currency,                 // A4 --    international currency
    char_E::currency,                 // A5 --    yen
    char_E::oper2,                    // A6 -- ¦  split bar
    char_E::special2,                 // A7 -- §  section
    char_E::accent2,                  // A8 -- ¨  umlaut
    char_E::misc,                     // A9 -- ©  copyright                
    char_E::misc,                     // AA --    feminine ordinal 
    char_E::open_angle_quote2,        // AB -- «  open angle quotes
    char_E::oper2,                    // AC -- ¬  not sign
    char_E::unsupported,              // AD --    soft hyphen 
    char_E::misc,                     // AE -- ®  registered trademark
    char_E::accent2,                  // AF -- ¯  macron 
    char_E::misc,                     // B0 --    degree
    char_E::oper2,                    // B1 --    plus/minus
    char_E::oper2,                    // B2 --    2 superscript
    char_E::oper2,                    // B3 --    3 superscript
    char_E::accent2,                  // B4 -- ´  acute accent
    char_E::misc,                     // B5 -- µ  micro sign
    char_E::special2,                 // B6 -- ¶  paragraph/pilcrow
    char_E::oper2,                    // B7 --    middle dot
    char_E::accent2,                  // B8 --    cedilla
    char_E::oper2,                    // B9 --    1 superscript
    char_E::misc,                     // BA --    masculine ordinal
    char_E::close_angle_quote2,       // BB -- »  close angle quotes
    char_E::misc,                     // BC --    1/4
    char_E::misc,                     // BD --    1/2
    char_E::misc,                     // BE --    3/4
    char_E::punct2,                   // BF -- ¿  inverted question mark
    char_E::alpha2,                   // C0 
    char_E::alpha2,                   // C1    
    char_E::alpha2,                   // C2 
    char_E::alpha2,                   // C3 
    char_E::alpha2,                   // C4 
    char_E::alpha2,                   // C5 
    char_E::alpha2,                   // C6 
    char_E::alpha2,                   // C7 
    char_E::alpha2,                   // C8 
    char_E::alpha2,                   // C9 
    char_E::alpha2,                   // CA 
    char_E::alpha2,                   // CB 
    char_E::alpha2,                   // CC 
    char_E::alpha2,                   // CD 
    char_E::alpha2,                   // CE 
    char_E::alpha2,                   // CF 
    char_E::alpha2,                   // D0 
    char_E::alpha2,                   // D1 
    char_E::alpha2,                   // D2 
    char_E::alpha2,                   // D3 
    char_E::alpha2,                   // D4 
    char_E::alpha2,                   // D5 
    char_E::alpha2,                   // D6 
    char_E::oper2,                    // D7 --   multiply
    char_E::alpha2,                   // D8 
    char_E::alpha2,                   // D9 
    char_E::alpha2,                   // DA 
    char_E::alpha2,                   // DB 
    char_E::alpha2,                   // DC 
    char_E::alpha2,                   // DD 
    char_E::alpha2,                   // DE 
    char_E::alpha2,                   // DF 
    char_E::alpha2,                   // E0 
    char_E::alpha2,                   // E1 
    char_E::alpha2,                   // E2 
    char_E::alpha2,                   // E3 
    char_E::alpha2,                   // E4 
    char_E::alpha2,                   // E5 
    char_E::alpha2,                   // E6 
    char_E::alpha2,                   // E7 
    char_E::alpha2,                   // E8 
    char_E::alpha2,                   // E9 
    char_E::alpha2,                   // EA 
    char_E::alpha2,                   // EB 
    char_E::alpha2,                   // EC 
    char_E::alpha2,                   // ED 
    char_E::alpha2,                   // EE 
    char_E::alpha2,                   // EF 
    char_E::alpha2,                   // F0 
    char_E::alpha2,                   // F1 
    char_E::alpha2,                   // F2 
    char_E::alpha2,                   // F3 
    char_E::alpha2,                   // F4 
    char_E::alpha2,                   // F5 
    char_E::alpha2,                   // F6 
    char_E::oper2,                    // F7 --  divide
    char_E::alpha2,                   // F8 
    char_E::alpha2,                   // F9 
    char_E::alpha2,                   // FA 
    char_E::alpha2,                   // FB 
    char_E::alpha2,                   // FC 
    char_E::alpha2,                   // FD 
    char_E::alpha2,                   // FE 
    char_E::alpha2                    // FF 
};


//  --------------------------------------------------------------------------
//  Classification map for selected other characters with special significance (if not in here, use range-based classification
//  --------------------------------------------------------------------------   

static const std::unordered_map<char32_t, char_E>  subtype_map
{
  { (char32_t)0x00002016,  char_E::oper2                }   //   ‖
, { (char32_t)0x00002018,  char_E::open_quote1          }   //   ‘     LEFT_SINGLE_QUOTATION_MARK                   
, { (char32_t)0x00002019,  char_E::close_quote1         }   //   ’     RIGHT_SINGLE_QUOTATION_MARK                  
, { (char32_t)0x0000201A,  char_E::open_quote1          }   //   ‘     SINGLE_LOW_9_QUOTATION_MARK                  
, { (char32_t)0x0000201B,  char_E::close_quote1         }   //   ’     SINGLE_HIGH_REVERSED_9_QUOTATION_MARK        
, { (char32_t)0x0000201C,  char_E::open_quote2          }   //   “     LEFT_DOUBLE_QUOTATION_MARK                   
, { (char32_t)0x0000201D,  char_E::close_quote2         }   //   ”     RIGHT_DOUBLE_QUOTATION_MARK                  
, { (char32_t)0x0000201E,  char_E::open_quote2          }   //   „     DOUBLE_LOW_9_QUOTATION_MARK                  
, { (char32_t)0x0000201F,  char_E::close_quote2         }   //   ‟     DOUBLE_HIGH_REVERSED_9_QUOTATION_MARK        
, { (char32_t)0x00002026,  char_E::oper2                }   //   …
, { (char32_t)0x00002039,  char_E::open_angle_quote1    }   //   ‹
, { (char32_t)0x0000203A,  char_E::close_angle_quote1   }   //   ›
, { (char32_t)0x0000203C,  char_E::punct2               }   //   ‼
, { (char32_t)0x0000203D,  char_E::punct2               }   //   ‽
, { (char32_t)0x00002045,  char_E::open_quill_bracket   }   //   ⁅
, { (char32_t)0x00002046,  char_E::close_quill_bracket  }   //   ⁆
, { (char32_t)0x00002047,  char_E::punct2               }   //   ⁇
, { (char32_t)0x00002048,  char_E::punct2               }   //   ⁈
, { (char32_t)0x00002049,  char_E::punct2               }   //   ⁉
, { (char32_t)0x0000204F,  char_E::punct2               }   //   ⁏
, { (char32_t)0x00002052,  char_E::oper2                }   //   ⁒
, { (char32_t)0x0000205D,  char_E::oper2                }   //   ⁝
, { (char32_t)0x0000205E,  char_E::oper2                }   //   ⁞
, { (char32_t)0x0000214B,  char_E::oper2                }   //   ⅋
, { (char32_t)0x0000214C,  char_E::oper2                }   //   ⅌
, { (char32_t)0x00002190,  char_E::oper2                }   //   ←
, { (char32_t)0x00002191,  char_E::oper2                }   //   ↑
, { (char32_t)0x00002192,  char_E::oper2                }   //   →
, { (char32_t)0x00002193,  char_E::oper2                }   //   ↓
, { (char32_t)0x00002194,  char_E::oper2                }   //   ↔
, { (char32_t)0x00002195,  char_E::oper2                }   //   ↕
, { (char32_t)0x00002310,  char_E::oper2                }   //   ⌐
, { (char32_t)0x00002329,  char_E::open_angle_bracket1  }   //   〈        LEFT_POINTING_ANGLE_BRACKET
, { (char32_t)0x0000232A,  char_E::close_angle_bracket1 }   //   〉        RIGHT_POINTING_ANGLE_BRACKET
, { (char32_t)0x000025BA,  char_E::oper2                }   //   ►
, { (char32_t)0x000025C4,  char_E::oper2                }   //   ◄ 
, { (char32_t)0x00002768,  char_E::open_paren2          }   //   ❨        MEDIUM_LEFT_PARENTHESIS_ORNAMENT            
, { (char32_t)0x00002769,  char_E::close_paren2         }   //   ❩        MEDIUM_RIGHT_PARENTHESIS_ORNAMENT           
, { (char32_t)0x0000276A,  char_E::open_other_bracket   }   //   ❪        MEDIUM_FLATTENED_LEFT_PARENTHESIS_ORNAMENT  
, { (char32_t)0x0000276B,  char_E::close_other_bracket  }   //   ❫        MEDIUM_FLATTENED_RIGHT_PARENTHESIS_ORNAMENT 
, { (char32_t)0x0000276C,  char_E::open_angle_bracket1  }   //   ❬        MEDIUM_LEFT_POINTING_ANGLE_BRACKET_ORNAMENT
, { (char32_t)0x0000276D,  char_E::close_angle_bracket1 }   //   ❭        MEDIUM_RIGHT_POINTING_ANGLE_BRACKET_ORNAMENT
, { (char32_t)0x0000276E,  char_E::open_angle_bracket1  }   //   ❮        HEAVY_LEFT_POINTING_ANGLE_QUOTATION_MARK_ORNAMENT 
, { (char32_t)0x0000276F,  char_E::close_angle_bracket1 }   //   ❯        HEAVY_RIGHT_POINTING_ANGLE_QUOTATION_MARK_ORNAMENT
, { (char32_t)0x00002770,  char_E::open_angle_bracket1  }   //   ❰        HEAVY_LEFT_POINTING_ANGLE_BRACKET_ORNAMENT        
, { (char32_t)0x00002771,  char_E::close_angle_bracket1 }   //   ❱        HEAVY_RIGHT_POINTING_ANGLE_BRACKET_ORNAMENT       
, { (char32_t)0x00002772,  char_E::open_shell_bracket1  }   //   ❲        LIGHT_LEFT_TORTOISE_SHELL_BRACKET_ORNAMENT              
, { (char32_t)0x00002773,  char_E::close_shell_bracket1 }   //   ❳        LIGHT_RIGHT_TORTOISE_SHELL_BRACKET_ORNAMENT             
, { (char32_t)0x00002774,  char_E::open_brace2          }   //   ❴        MEDIUM_LEFT_CURLY_BRACKET_ORNAMENT                      
, { (char32_t)0x00002775,  char_E::close_brace2         }   //   ❵        MEDIUM_RIGHT_CURLY_BRACKET_ORNAMENT                     
, { (char32_t)0x000027C5,  char_E::open_other_bracket   }   //   ⟅        LEFT_S_SHAPED_BAG_DELIMITER       
, { (char32_t)0x000027C6,  char_E::close_other_bracket  }   //   ⟆        RIGHT_S_SHAPED_BAG_DELIMITER      
, { (char32_t)0x000027E6,  char_E::open_bracket2        }   //   ⟦        MATHEMATICAL_LEFT_WHITE_SQUARE_BRACKET                      
, { (char32_t)0x000027E7,  char_E::close_bracket2       }   //   ⟧        MATHEMATICAL_RIGHT_WHITE_SQUARE_BRACKET                     
, { (char32_t)0x000027E8,  char_E::open_angle_bracket1  }   //   ⟨        MATHEMATICAL_LEFT_ANGLE_BRACKET             
, { (char32_t)0x000027E9,  char_E::close_angle_bracket1 }   //   ⟩        MATHEMATICAL_RIGHT_ANGLE_BRACKET            
, { (char32_t)0x000027EA,  char_E::open_angle_bracket2  }   //   ⟪        MATHEMATICAL_LEFT_DOUBLE_ANGLE_BRACKET                
, { (char32_t)0x000027EB,  char_E::close_angle_bracket2 }   //   ⟫        MATHEMATICAL_RIGHT_DOUBLE_ANGLE_BRACKET               
, { (char32_t)0x000027EC,  char_E::open_shell_bracket2  }   //   ⟬        MATHEMATICAL_LEFT_WHITE_TORTOISE_SHELL_BRACKET        
, { (char32_t)0x000027ED,  char_E::close_shell_bracket2 }   //   ⟭        MATHEMATICAL_RIGHT_WHITE_TORTOISE_SHELL_BRACKET       
, { (char32_t)0x000027EE,  char_E::open_other_bracket   }   //   ⟮        MATHEMATICAL_LEFT_FLATTENED_PARENTHESIS               
, { (char32_t)0x000027EF,  char_E::close_other_bracket  }   //   ⟯        MATHEMATICAL_RIGHT_FLATTENED_PARENTHESIS              
, { (char32_t)0x00002983,  char_E::open_brace2          }   //   ⦃        LEFT_WHITE_CURLY_BRACKET                          
, { (char32_t)0x00002984,  char_E::close_brace2         }   //   ⦄        RIGHT_WHITE_CURLY_BRACKET                         
, { (char32_t)0x00002985,  char_E::open_paren2          }   //   ⦅        LEFT_WHITE_PARENTHESIS                            
, { (char32_t)0x00002986,  char_E::close_paren2         }   //   ⦆        RIGHT_WHITE_PARENTHESIS                           
, { (char32_t)0x00002987,  char_E::open_other_bracket   }   //   ⦇        Z_NOTATION_LEFT_IMAGE_BRACKET                     
, { (char32_t)0x00002988,  char_E::close_other_bracket  }   //   ⦈        Z_NOTATION_RIGHT_IMAGE_BRACKET                    
, { (char32_t)0x00002989,  char_E::open_other_bracket   }   //   ⦉        Z_NOTATION_LEFT_BINDING_BRACKET                   
, { (char32_t)0x0000298A,  char_E::close_other_bracket  }   //   ⦊        Z_NOTATION_RIGHT_BINDING_BRACKET                  
, { (char32_t)0x0000298B,  char_E::open_other_bracket   }   //   ⦋        LEFT_SQUARE_BRACKET_WITH_UNDERBAR                 
, { (char32_t)0x0000298C,  char_E::close_other_bracket  }   //   ⦌        RIGHT_SQUARE_BRACKET_WITH_UNDERBAR                
, { (char32_t)0x0000298D,  char_E::open_other_bracket   }   //   ⦍        LEFT_SQUARE_BRACKET_WITH_TICK_IN_TOP_CORNER       
, { (char32_t)0x0000298E,  char_E::close_other_bracket  }   //   ⦎        RIGHT_SQUARE_BRACKET_WITH_TICK_IN_BOTTOM_CORNER   
, { (char32_t)0x0000298F,  char_E::open_other_bracket   }   //   ⦏        LEFT_SQUARE_BRACKET_WITH_TICK_IN_BOTTOM_CORNER    
, { (char32_t)0x00002990,  char_E::close_other_bracket  }   //   ⦐        RIGHT_SQUARE_BRACKET_WITH_TICK_IN_TOP_CORNER      
, { (char32_t)0x00002991,  char_E::open_dot_bracket     }   //   ⦑        LEFT_ANGLE_BRACKET_WITH_DOT                       
, { (char32_t)0x00002992,  char_E::close_dot_bracket    }   //   ⦒        RIGHT_ANGLE_BRACKET_WITH_DOT                      
, { (char32_t)0x00002993,  char_E::open_other_bracket   }   //   ⦓       LEFT_ARC_LESS_THAN_BRACKET                        
, { (char32_t)0x00002994,  char_E::close_other_bracket  }   //   ⦔       RIGHT_ARC_GREATER_THAN_BRACKET                    
, { (char32_t)0x00002995,  char_E::open_other_bracket   }   //   ⦕       DOUBLE_LEFT_ARC_GREATER_THAN_BRACKET              
, { (char32_t)0x00002996,  char_E::close_other_bracket  }   //   ⦖       DOUBLE_RIGHT_ARC_LESS_THAN_BRACKET                
, { (char32_t)0x00002997,  char_E::open_shell_bracket1  }   //   ⦗        LEFT_BLACK_TORTOISE_SHELL_BRACKET                 
, { (char32_t)0x00002998,  char_E::close_shell_bracket1 }   //   ⦘        RIGHT_BLACK_TORTOISE_SHELL_BRACKET                
, { (char32_t)0x000029FC,  char_E::open_curve_bracket1  }   //   ⧼        LEFT_POINTING_CURVED_ANGLE_BRACKET         
, { (char32_t)0x000029FD,  char_E::close_curve_bracket1 }   //   ⧽        RIGHT_POINTING_CURVED_ANGLE_BRACKET        
, { (char32_t)0x00002E18,  char_E::punct2               }   //   ⸘
, { (char32_t)0x00002E22,  char_E::open_corner_bracket1 }   //   ⸢        TOP_LEFT_HALF_BRACKET                        
, { (char32_t)0x00002E23,  char_E::close_corner_bracket1}   //   ⸣        TOP_RIGHT_HALF_BRACKET                       
, { (char32_t)0x00002E24,  char_E::open_corner_bracket1 }   //   ⸤        BOTTOM_LEFT_HALF_BRACKET                     
, { (char32_t)0x00002E25,  char_E::close_corner_bracket1}   //   ⸥        BOTTOM_RIGHT_HALF_BRACKET                    
, { (char32_t)0x00002E26,  char_E::open_other_bracket   }   //   ⸦        LEFT_SIDEWAYS_U_BRACKET                      
, { (char32_t)0x00002E27,  char_E::close_other_bracket  }   //   ⸧        RIGHT_SIDEWAYS_U_BRACKET                     
, { (char32_t)0x00002E28,  char_E::open_paren2          }   //   ⸨        LEFT_DOUBLE_PARENTHESIS                      
, { (char32_t)0x00002E29,  char_E::close_paren2         }   //   ⸩        RIGHT_DOUBLE_PARENTHESIS                     
, { (char32_t)0x00002E2E,  char_E::punct2               }   //   ⸮

/****** omit CJK/ideographic parens/brackets, etc.  
, { (char32_t)0x00003008,  char_E::open_angle_bracket1  }   //  〈
, { (char32_t)0x00003009,  char_E::close_angle_bracket1 }   //   〉
, { (char32_t)0x0000300A,  char_E::open_angle_bracket2  }   //  《
, { (char32_t)0x0000300B,  char_E::close_angle_bracket2 }   //   》
, { (char32_t)0x0000300C,  char_E::open_corner_bracket1 }   //  「
, { (char32_t)0x0000300D,  char_E::close_corner_bracket1}   //   」
, { (char32_t)0x0000300E,  char_E::open_corner_bracket2 }   //  『
, { (char32_t)0x0000300F,  char_E::close_corner_bracket2}   //   』
, { (char32_t)0x00003010,  char_E::open_lens_bracket1   }   //  【
, { (char32_t)0x00003011,  char_E::close_lens_bracket1  }   //   】
, { (char32_t)0x00003014,  char_E::open_shell_bracket1  }   //  〔
, { (char32_t)0x00003015,  char_E::close_shell_bracket1 }   //   〕
, { (char32_t)0x00003016,  char_E::open_lens_bracket2   }   //  〖
, { (char32_t)0x00003017,  char_E::close_lens_bracket2  }   //   〗
, { (char32_t)0x00003018,  char_E::open_shell_bracket2  }   //  〘
, { (char32_t)0x00003019,  char_E::close_shell_bracket2 }   //   〙
, { (char32_t)0x0000301A,  char_E::open_bracket2        }   //  〚
, { (char32_t)0x0000301B,  char_E::close_bracket2       }   //   〛
, { (char32_t)0x0000301D,  char_E::open_quote2          }   //
, { (char32_t)0x0000301E,  char_E::close_quote2         }   //
***************************/
/************* omit special foreign language forms
, { (char32_t)0x0000FF02,  char_E::both_quote2          }   //
, { (char32_t)0x0000FF07,  char_E::both_quote1          }   //
, { (char32_t)0x0000FF08,  char_E::open_paren1          }   //
, { (char32_t)0x0000FF09,  char_E::close_paren1         }   //
, { (char32_t)0x0000FF3B,  char_E::open_bracket1        }   //
, { (char32_t)0x0000FF3D,  char_E::close_bracket1       }   //
, { (char32_t)0x0000FD3E,  char_E::open_other_bracket   }   //
, { (char32_t)0x0000FD3F,  char_E::close_other_bracket  }   //
, { (char32_t)0x0000FF5B,  char_E::open_brace1          }   //
, { (char32_t)0x0000FF5D,  char_E::close_brace1         }   //
, { (char32_t)0x0000FF5F,  char_E::open_paren1          }   //
, { (char32_t)0x0000FF60,  char_E::close_paren1         }   //
, { (char32_t)0x0000FF62,  char_E::open_corner_bracket1 }   //
, { (char32_t)0x0000FF63,  char_E::close_corner_bracket1}   //
************************/

};

//  ---------------------------------
//  character subtype -> type mapping
//  --------------------------------- 

static const std::unordered_map<char_E, char_E>  type_map
{
// classses
  { char_E::none                  , char_E::none                          }
, { char_E::eol                   , char_E::eol                           }
, { char_E::eof                   , char_E::eof                           }
, { char_E::end                   , char_E::end                           }
, { char_E::error                 , char_E::error                         }
, { char_E::normal                , char_E::normal                        }   // ?????
// families
, { char_E::unsupported           , char_E::unsupported                   }
, { char_E::whitespace            , char_E::whitespace                    }
, { char_E::alphanum              , char_E::alphanum                      }   // ???????
, { char_E::punct                 , char_E::punct                         }   // ?????? 
, { char_E::oper                  , char_E::oper                          }   // ??????
, { char_E::special               , char_E::special                       }   // ???????
, { char_E::misc                  , char_E::misc                          }
, { char_E::accent                , char_E::accent                        }   // ??????
// types
, { char_E::num                   , char_E::num                           }  
, { char_E::alpha                 , char_E::alpha                         }   // ????
, { char_E::currency              , char_E::currency                      }
, { char_E::sep                   , char_E::sep                           }
, { char_E::open_bracket          , char_E::open_bracket                  }  // ????
, { char_E::close_bracket         , char_E::close_bracket                 }  // ???? 
, { char_E::quote                 , char_E::quote                         }  // ???
, { char_E::open_quote            , char_E::open_quote                    }  // ???
, { char_E::close_quote           , char_E::close_quote                   }  // ???
// subtypes
, { char_E::underscore            , char_E::sep                           }
, { char_E::full_stop             , char_E::punct                         }
, { char_E::comma                 , char_E::punct                         }
, { char_E::semicolon             , char_E::punct                         }
, { char_E::colon                 , char_E::punct                         }
, { char_E::hyphen_minus          , char_E::oper                          } 
, { char_E::plus                  , char_E::oper                          } 
, { char_E::digit                 , char_E::num                           }
, { char_E::alpha1                , char_E::alpha                         }
, { char_E::alpha2                , char_E::alpha                         }
, { char_E::alpha3                , char_E::alpha                         }
, { char_E::alpha4                , char_E::alpha                         }
, { char_E::special1              , char_E::special                       }    // note: special1 is not currently used
, { char_E::special2              , char_E::special                       }
, { char_E::accent1               , char_E::accent                        } 
, { char_E::accent2               , char_E::accent                        } 
, { char_E::oper1                 , char_E::oper                          } 
, { char_E::oper2                 , char_E::oper                          } 
, { char_E::punct1                , char_E::punct                         }    // note: punct1 is not currently used
, { char_E::punct2                , char_E::punct                         } 
, { char_E::open_paren1           , char_E::open_bracket                  }
, { char_E::close_paren1          , char_E::close_bracket                 }
, { char_E::open_paren2           , char_E::open_bracket                  }
, { char_E::close_paren2          , char_E::close_bracket                 }
, { char_E::open_brace1           , char_E::open_bracket                  }
, { char_E::close_brace1          , char_E::close_bracket                 }
, { char_E::open_brace2           , char_E::open_bracket                  }
, { char_E::close_brace2          , char_E::close_bracket                 }
, { char_E::open_bracket1         , char_E::open_bracket                  }
, { char_E::close_bracket1        , char_E::close_bracket                 }
, { char_E::open_bracket2         , char_E::open_bracket                  }
, { char_E::close_bracket2        , char_E::close_bracket                 }
, { char_E::open_angle_bracket1   , char_E::open_bracket                  }
, { char_E::close_angle_bracket1  , char_E::close_bracket                 }
, { char_E::open_angle_bracket2   , char_E::open_bracket                  }
, { char_E::close_angle_bracket2  , char_E::close_bracket                 }
, { char_E::open_shell_bracket1   , char_E::open_bracket                  }
, { char_E::close_shell_bracket1  , char_E::close_bracket                 }
, { char_E::open_shell_bracket2   , char_E::open_bracket                  }
, { char_E::close_shell_bracket2  , char_E::close_bracket                 }
, { char_E::open_curve_bracket1   , char_E::open_bracket                  }
, { char_E::close_curve_bracket1  , char_E::close_bracket                 }
, { char_E::open_curve_bracket2   , char_E::open_bracket                  }
, { char_E::close_curve_bracket2  , char_E::close_bracket                 }
, { char_E::open_lens_bracket1    , char_E::open_bracket                  }
, { char_E::close_lens_bracket1   , char_E::close_bracket                 }
, { char_E::open_lens_bracket2    , char_E::open_bracket                  }
, { char_E::close_lens_bracket2   , char_E::close_bracket                 }
, { char_E::open_corner_bracket1  , char_E::open_bracket                  }
, { char_E::close_corner_bracket1 , char_E::close_bracket                 }
, { char_E::open_corner_bracket2  , char_E::open_bracket                  }
, { char_E::close_corner_bracket2 , char_E::close_bracket                 }
, { char_E::open_quill_bracket    , char_E::open_bracket                  }
, { char_E::close_quill_bracket   , char_E::close_bracket                 }
, { char_E::open_dot_bracket      , char_E::open_bracket                  }
, { char_E::close_dot_bracket     , char_E::close_bracket                 }
, { char_E::open_other_bracket    , char_E::open_bracket                  }
, { char_E::close_other_bracket   , char_E::close_bracket                 }
, { char_E::both_quote1           , char_E::quote                         }
, { char_E::both_quote2           , char_E::quote                         }
, { char_E::open_quote1           , char_E::open_quote                    }
, { char_E::close_quote1          , char_E::close_quote                   }
, { char_E::open_quote2           , char_E::open_quote                    }
, { char_E::close_quote2          , char_E::close_quote                   }
, { char_E::open_angle_quote1     , char_E::open_quote                    }
, { char_E::close_angle_quote1    , char_E::close_quote                   }
, { char_E::open_angle_quote2     , char_E::open_quote                    }
, { char_E::close_angle_quote2    , char_E::close_quote                   }
, { char_E::open_other_quote1     , char_E::open_quote                    }
, { char_E::close_other_quote1    , char_E::close_quote                   }
, { char_E::open_other_quote2     , char_E::open_quote                    }
, { char_E::close_other_quote2    , char_E::close_quote                   }
};


//  --------------------------------
//  character type -> family mapping
//  -------------------------------- 

static const std::unordered_map<char_E, char_E>  family_map
{
  { char_E::none                  , char_E::none                          }
, { char_E::eol                   , char_E::eol                           }
, { char_E::eof                   , char_E::eof                           }
, { char_E::end                   , char_E::end                           }
, { char_E::error                 , char_E::error                         }
, { char_E::normal                , char_E::normal                        }  // ???

, { char_E::unsupported           , char_E::unsupported                   }
, { char_E::whitespace            , char_E::whitespace                    }
, { char_E::alphanum              , char_E::alphanum                      }  // ???
, { char_E::punct                 , char_E::punct                         }    
, { char_E::oper                  , char_E::oper                          }   
, { char_E::special               , char_E::special                       }   
, { char_E::misc                  , char_E::misc                          }
, { char_E::accent                , char_E::accent                        } 

, { char_E::num                   , char_E::alphanum                      }  
, { char_E::alpha                 , char_E::alphanum                      }  // ???
, { char_E::currency              , char_E::special                       }
, { char_E::sep                   , char_E::special                       }
, { char_E::open_bracket          , char_E::punct                         }  // ???
, { char_E::close_bracket         , char_E::punct                         }  // ???
, { char_E::quote                 , char_E::punct                         }  // ???
, { char_E::open_quote            , char_E::punct                         }  // ???
, { char_E::close_quote           , char_E::punct                         }  // ???
};




//  ----------------------------------------------------------------
//  character classs/family/type/subtype to printable string mapping
//  ---------------------------------------------------------------- 

static const std::unordered_map<char_E, std::wstring>  type_str_map
{
  { char_E::none                  , L"none                    "   }
, { char_E::eol                   , L"eol                     "   }
, { char_E::eof                   , L"eof                     "   }
, { char_E::end                   , L"end                     "   }
, { char_E::error                 , L"error                   "   }
, { char_E::normal                , L"normal                  "   } 

, { char_E::unsupported           , L"<<<<<<<unsupported>>>>>>"   }
, { char_E::whitespace            , L"whitespace              "   }
, { char_E::alphanum              , L"alphanum                "   }   
, { char_E::punct                 , L"punct                   "   }   
, { char_E::oper                  , L"oper                    "   }   
, { char_E::special               , L"special                 "   }   
, { char_E::misc                  , L"misc                    "   }
, { char_E::accent                , L"accent                  "   }   
, { char_E::num                   , L"num                     "   }  
, { char_E::alpha                 , L"alpha                   "   }   
, { char_E::currency              , L"currency                "   }
, { char_E::sep                   , L"sep                     "   }
, { char_E::open_bracket          , L"open_bracket            "   }  
, { char_E::close_bracket         , L"close_bracket           "   }  
, { char_E::quote                 , L"quote                   "   }  
, { char_E::open_quote            , L"open_quote              "   }  
, { char_E::close_quote           , L"close_quote             "   } 
, { char_E::underscore            , L"underscore              "   }
, { char_E::full_stop             , L"full_stop               "   }
, { char_E::comma                 , L"comma                   "   }
, { char_E::semicolon             , L"semicolon               "   }
, { char_E::colon                 , L"colon                   "   }
, { char_E::hyphen_minus          , L"hyphen_minus            "   }
, { char_E::plus                  , L"plus                    "   }
, { char_E::digit                 , L"digit                   "   }
, { char_E::alpha1                , L"alpha1                  "   }
, { char_E::alpha2                , L"alpha2                  "   }
, { char_E::alpha3                , L"alpha3                  "   }
, { char_E::alpha4                , L"alpha4                  "   }
, { char_E::special1              , L"special1                "   }
, { char_E::special2              , L"special2                "   }
, { char_E::accent1               , L"accent1                 "   }
, { char_E::accent2               , L"accent2                 "   }
, { char_E::oper1                 , L"oper1                   "   }
, { char_E::oper2                 , L"oper2                   "   }
, { char_E::punct1                , L"punct1                  "   }
, { char_E::punct2                , L"punct2                  "   }
, { char_E::open_paren1           , L"open_paren1             "   }
, { char_E::close_paren1          , L"close_paren1            "   }
, { char_E::open_paren2           , L"open_paren2             "   }
, { char_E::close_paren2          , L"close_paren2            "   }
, { char_E::open_brace1           , L"open_brace1             "   }
, { char_E::close_brace1          , L"close_brace1            "   }
, { char_E::open_brace2           , L"open_brace2             "   }
, { char_E::close_brace2          , L"close_brace2            "   }
, { char_E::open_bracket1         , L"open_bracket1           "   }
, { char_E::close_bracket1        , L"close_bracket1          "   }
, { char_E::open_bracket2         , L"open_bracket2           "   }
, { char_E::close_bracket2        , L"close_bracket2          "   }
, { char_E::open_angle_bracket1   , L"open_angle_bracket1     "   }
, { char_E::close_angle_bracket1  , L"close_angle_bracket1    "   }
, { char_E::open_angle_bracket2   , L"open_angle_bracket2     "   }
, { char_E::close_angle_bracket2  , L"close_angle_bracket2    "   }
, { char_E::open_shell_bracket1   , L"open_shell_bracket1     "   }
, { char_E::close_shell_bracket1  , L"close_shell_bracket1    "   }
, { char_E::open_shell_bracket2   , L"open_shell_bracket2     "   }
, { char_E::close_shell_bracket2  , L"close_shell_bracket2    "   }
, { char_E::open_curve_bracket1   , L"open_curve_bracket1     "   }
, { char_E::close_curve_bracket1  , L"close_curve_bracket1    "   }
, { char_E::open_curve_bracket2   , L"open_curve_bracket2     "   }
, { char_E::close_curve_bracket2  , L"close_curve_bracket2    "   }
, { char_E::open_lens_bracket1    , L"open_lens_bracket1      "   }
, { char_E::close_lens_bracket1   , L"close_lens_bracket1     "   }
, { char_E::open_lens_bracket2    , L"open_lens_bracket2      "   }
, { char_E::close_lens_bracket2   , L"close_lens_bracket2     "   }
, { char_E::open_corner_bracket1  , L"open_corner_bracket1    "   }
, { char_E::close_corner_bracket1 , L"close_corner_bracket1   "   }
, { char_E::open_corner_bracket2  , L"open_corner_bracket2    "   }
, { char_E::close_corner_bracket2 , L"close_corner_bracket2   "   }
, { char_E::open_other_bracket    , L"open_other_bracket      "   }
, { char_E::close_other_bracket   , L"close_other_bracket     "   }
, { char_E::both_quote1           , L"both_quote1             "   }
, { char_E::both_quote2           , L"both_quote2             "   }
, { char_E::open_quote1           , L"open_quote1             "   }
, { char_E::close_quote1          , L"close_quote1            "   }
, { char_E::open_quote2           , L"open_quote2             "   }
, { char_E::close_quote2          , L"close_quote2            "   }
, { char_E::open_angle_quote1     , L"open_angle_quote1       "   }
, { char_E::close_angle_quote1    , L"close_angle_quote1      "   }
, { char_E::open_angle_quote2     , L"open_angle_quote2       "   }
, { char_E::close_angle_quote2    , L"close_angle_quote2      "   }
, { char_E::open_other_quote1     , L"open_other_quote1       "   }
, { char_E::close_other_quote1    , L"close_other_quote1      "   }
, { char_E::open_other_quote2     , L"open_other_quote2       "   }
, { char_E::close_other_quote2    , L"close_other_quote2      "   }
};                                                           
 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   char_subtype() -- set character subtype -- external free function   
//   --------------
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char_E char_subtype(char32_t in_char) try
{
    uint32_t cp       { (uint32_t)(in_char) };
    char_E   subtype  { char_E::unsupported };   

    if (cp <  0x00000100U) 
    {
        subtype = subtype_table[cp];          // most common characters -- use table for speed               
    }
    else   // not 00 to FF
    {
        // check subtype map for characters of special significance 

        auto st_pos = subtype_map.find(cp);

        if (st_pos != subtype_map.end()) 
        {
             subtype = st_pos->second; 
             M__(M_out(L"char_subtype() -- subtype_map.find(%08X)->second = %d") % cp % (int)subtype;)                
        }
        else  // Character is not of special significance -- do range-based classification
        {
            if      ( (cp >= 0x00000100U) && (cp <= 0x0000017FU) )       subtype = char_E::alpha2;                      // Latin extended A
            else if ( (cp >= 0x00000180U) && (cp <= 0x0000024FU) )       subtype = char_E::alpha2;                      // Latin extended B
            else if ( (cp >= 0x00000250U) && (cp <= 0x000002AFU) )       subtype = char_E::alpha2;                      // IPA extensions
          //else if ( (cp >= 0x000002B0U) && (cp <= 0x000002FFU) )       subtype = char_E::unsupported;                 // Spacing modifier letters
            else if ( (cp >= 0x00000300U) && (cp <= 0x0000036FU) )       subtype = char_E::accent2;                     // Combining diacritical marks
          //else if ( (cp >= 0x00000370U) && (cp <= 0x00001AAFU) )       subtype = char_E::unsupported;                 // Various non-latin alphabets
          //else if ( (cp >= 0x00001AB0U) && (cp <= 0x00001AFFU) )       subtype = char_E::unsupported;                 // Combining diacritical marks extended
          //else if ( (cp >= 0x00001B00U) && (cp <= 0x00001CFFU) )       subtype = char_E::unsupported;                 // various non-latin alphabets
            else if ( (cp >= 0x00001D00U) && (cp <= 0x00001D7FU) )       subtype = char_E::alpha2;                      // Phonetic extensions
            else if ( (cp >= 0x00001D80U) && (cp <= 0x00001DBFU) )       subtype = char_E::alpha2;                      // Phonetic extensions supplement
            else if ( (cp >= 0x00001DC0U) && (cp <= 0x00001DFFU) )       subtype = char_E::accent2;                     // Supplemental diacritical marks
            else if ( (cp >= 0x00001E00U) && (cp <= 0x00001EFFU) )       subtype = char_E::alpha2;                      // Latin extended additional
          //else if ( (cp >= 0x00001F00U) && (cp <= 0x00001FFFU) )       subtype = char_E::unsupported;                 // Various non-latin alphabets
            else if ( (cp >= 0x00002000U) && (cp <= 0x0000206FU) )       subtype = char_E::special2;                    // General punctuation
            else if ( (cp >= 0x00002070U) && (cp <= 0x0000209FU) )       subtype = char_E::misc;                        // Superscripts and subscripts
            else if ( (cp >= 0x000020A0U) && (cp <= 0x000020CFU) )       subtype = char_E::currency;                    // Currency symbols
          //else if ( (cp >= 0x000020D0U) && (cp <= 0x000020FFU) )       subtype = char_E::unsupported;                 // Combining diacritical for symbols
            else if ( (cp >= 0x00002100U) && (cp <= 0x0000214FU) )       subtype = char_E::misc;                        // Letterlike symbols
            else if ( (cp >= 0x00002150U) && (cp <= 0x0000218FU) )       subtype = char_E::misc;                        // Number forms
            else if ( (cp >= 0x00002190U) && (cp <= 0x000021FFU) )       subtype = char_E::misc;                        // Arrows
            else if ( (cp >= 0x00002200U) && (cp <= 0x000022FFU) )       subtype = char_E::oper2;                       // Mathematical operators
            else if ( (cp >= 0x00002300U) && (cp <= 0x000023FFU) )       subtype = char_E::misc;                        // Miscellaneous technical
            else if ( (cp >= 0x00002400U) && (cp <= 0x0000243FU) )       subtype = char_E::misc;                        // Control Pictures
            else if ( (cp >= 0x00002440U) && (cp <= 0x0000245FU) )       subtype = char_E::misc;                        // Optical Character Recognition
            else if ( (cp >= 0x00002460U) && (cp <= 0x000024FFU) )       subtype = char_E::misc;                        // Enclosed alphanumerics
            else if ( (cp >= 0x00002500U) && (cp <= 0x0000257FU) )       subtype = char_E::misc;                        // Box drawing
            else if ( (cp >= 0x00002580U) && (cp <= 0x0000259FU) )       subtype = char_E::misc;                        // Block elements
            else if ( (cp >= 0x000025A0U) && (cp <= 0x000025FFU) )       subtype = char_E::misc;                        // Geometric shapes
            else if ( (cp >= 0x00002600U) && (cp <= 0x000026FFU) )       subtype = char_E::misc;                        // Miscellaneous symbols
            else if ( (cp >= 0x00002700U) && (cp <= 0x000027BFU) )       subtype = char_E::misc;                        // Dingbats
            else if ( (cp >= 0x000027C0U) && (cp <= 0x000027EFU) )       subtype = char_E::misc;                        // Miscellaneous mathematical symbols - A
            else if ( (cp >= 0x000027F0U) && (cp <= 0x000027FFU) )       subtype = char_E::misc;                        // Supplemental arrows - A
          //else if ( (cp >= 0x00002800U) && (cp <= 0x000028FFU) )       subtype = char_E::unsupported;                 // Braille patterns
            else if ( (cp >= 0x00002900U) && (cp <= 0x0000297FU) )       subtype = char_E::misc;                        // Supplemental arrows - B
            else if ( (cp >= 0x00002980U) && (cp <= 0x000029FFU) )       subtype = char_E::misc;                        // Miscellaneous mathematical symbols - B
            else if ( (cp >= 0x00002A00U) && (cp <= 0x00002AFFU) )       subtype = char_E::oper2;                       // Supplemental mathematical operators
            else if ( (cp >= 0x00002B00U) && (cp <= 0x00002BFFU) )       subtype = char_E::misc;                        // Miscellaneous symbols and arrows
          //else if ( (cp >= 0x00002C00U) && (cp <= 0x00002C5FU) )       subtype = char_E::unsupported;                 // Various non-latin alphabets
            else if ( (cp >= 0x00002C60U) && (cp <= 0x00002C7FU) )       subtype = char_E::alpha2;                      // Latin extended C
          //else if ( (cp >= 0x00002C80U) && (cp <= 0x00002DFFU) )       subtype = char_E::unsupported;                 // Various non-latin alphabets
            else if ( (cp >= 0x00002E00U) && (cp <= 0x00002E7FU) )       subtype = char_E::special2;                    // Supplemental punctuation
          //else if ( (cp >= 0x00002E80U) && (cp <= 0x0000A71FU) )       subtype = char_E::unsupported;                 // CJK and various foreign languages
            else if ( (cp >= 0x0000A720U) && (cp <= 0x0000A7FFU) )       subtype = char_E::alpha2;                      // Latin extended D
          //else if ( (cp >= 0x0000A800U) && (cp <= 0x0000AB2FU) )       subtype = char_E::unsupported;                 // CJK and various foreign languages
            else if ( (cp >= 0x0000AB30U) && (cp <= 0x0000AB6FU) )       subtype = char_E::alpha2;                      // Latin extended E
                     // ?????  0000AB70    -          0000ABBF  ???? //
          //else if ( (cp >= 0x0000AC00U) && (cp <= 0x0000D7FFU) )       subtype = char_E::unsupported;                 // CJK and various foreign languages
          //else if ( (cp >= 0x0000D800U) && (cp <= 0x0000DFFFU) )       subtype = char_E::unsupported;                 // High and low surrogates
          //else if ( (cp >= 0x0000E000U) && (cp <= 0x0000F8FFU) )       subtype = char_E::unsupported;                 // Private use area
          //else if ( (cp >= 0x0000F900U) && (cp <= 0x0000FAFFU) )       subtype = char_E::unsupported;                 // CJK and various foreign languages
          //else if ( (cp >= 0x0000FB00U) && (cp <= 0x0000FB4FU) )       subtype = char_E::unsupported;                 // Alphabetic presentation forms
          //else if ( (cp >= 0x0000FB50U) && (cp <= 0x0000FFEFU) )       subtype = char_E::unsupported;                 // Various foreign languages
          //else if ( (cp >= 0x0000FFF0U) && (cp <= 0x0000FFFFU) )       subtype = char_E::unsupported;                 // Specials          
          //else if ( (cp >= 0x0000D400U) && (cp <= 0x0001D7FFU) )       subtype = char_E::unsupported;                 // Mathematical alphanumeric symbols
            else                                                         subtype = char_E::unsupported;                 // must be an unsupported character                
        }
     }       
      
     //M_y(M_out(L"char_subtype(): in_char = %08X  subtype = %d" ) % cp % (int)subtype;)


     return subtype; 
}           
M_endf      
            

 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   char_type() -- set character type -- external free function   
//   -----------
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//  internal version with input subtype (char_E)
//  --------------------------------------------

static char_E char_type(char_E subtype) try
{
    char_E type { };


    // get char type based on passed-in subtype -- error if unexpected error if char subtype is not in the table

    auto t_pos = type_map.find(subtype); 
    if (t_pos != type_map.end()) 
    {
        type = t_pos->second; 
        M__(M_out(L"type_map.find(%08X)->second = %d") % (int32_t)in_char % (int)type;)                
    }
    else  // error -- subtype should have been in table
    {
        type = char_E::error; 
        M_out_emsg(L"char_subtype() -- unexpected error -- char subtype %d not found in type_map") % (int)subtype;                
    }  

    return type; 
}
M_endf


// external version with passed-in char32_t
// ----------------------------------------

char_E char_type(char32_t in_char) try
{
    return char_type(char_subtype(in_char)); 
}
M_endf                              



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//   char_classify() -- set character type, subtype, family, etc.  (internal free function)
//   --------------
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void char_classify(in_char_S& in_char)
{
    //  Get char subtype based on Unicode codepoint

    if (in_char.classs == char_E::normal)
        in_char.subtype = char_subtype(in_char.ch32);
    else                                             
        in_char.subtype = in_char.classs;             // non normal char class -- don't go through mapping tables, etc. -- just set subtype to class

    

    //   Get type based on subtype -- error if subtype is not in the table

    in_char.type = char_type(in_char.subtype);  


    // get family based on type -- error if type is not in table

    auto f_pos = family_map.find(in_char.type); 
    if (f_pos != family_map.end()) 
    {
        in_char.family = f_pos->second; 
        M__(M_out(L"char_classify() -- family_map.find(%08X)->second = %d") % cp % (int)(in_char.family);)                
    }
    else  // error -- type should have been in table
    {
        in_char.family = char_E::error; 
        M_out_emsg(L"char_classify() -- char type %d not found in family_map") % (int)(in_char.type);                
    }

    return; 
}  




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
///      CCCC     H    H     AAA     RRRR            SSSS   TTTTT   RRRR    EEEEE    AAA     M     M           
///     C         H    H    A   A    R   R          S         T     R   R   E       A   A    MM   MM     
///     C         HHHHHH    AAAAA    RRRR            SSS      T     RRRR    EEEEE   AAAAA    M M M M    
///     C         H    H    A   A    R  R               S     T     R  R    E       A   A    M  M  M 
///      CCCC     H    H    A   A    R   R   =====  SSSS      T     R   R   EEEEE   A   A    M     M      
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
char_stream_C::char_stream_C()      
    : m_in_file_p      {nullptr}         
    , m_in_filename    {}
    , m_char_stack     {}    
{
    M_dmsg("char_stream_C() called -- default constructor")
    return;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// copy constructor (deleted)
// ---------------- 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
char_stream_C::char_stream_C(const char_stream_C& from)
    : m_in_file_p      { from.m_in_file_p      }
    , m_in_filename    { from.m_in_filename    }
    , m_char_stack     { from.m_char_stack     }
{
    M_msg1("char_stream_C(const char_stream_C&) called -- copy constructor -- \"") M_wide(m_in_filename) M_msg2("\"")
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
char_stream_C::char_stream_C(char_stream_C&& from)
    : m_in_file_p      { from.m_in_file_p              }
    , m_in_filename    { std::move(from.m_in_filename) }
    , m_char_stack     { std::move(from.m_char_stack)  }
{
    M_msg1("char_stream_C(char_stream_C&&) called -- move constructor -- \"") M_wide(from.m_in_filename) M_msg2("\"")
    return; 
}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// destructor (defaulted)
// ----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
char_stream_C::~char_stream_C()                    // destructor
{
    M_msg("~char_stream_C() called -- destructor -- \"%s\"", M_ws(m_in_filename))

    m_in_filename = std::wstring {};                // empty the string    
    m_char_stack  = std::deque<in_char_S> {};       // empty out the putback queue 
    return; 
}
*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// copy assignment (deleted)
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
char_stream_C& char_stream_C::operator=(const char_stream_C& from)
{
    M_msg1("operator=(const char_stream_C &) called -- copy assignment -- \"") M_wide(from.m_in_filename) M_msg1("\" -> \"") M_wide(m_in_filename) M_msg2("\"")


    char_stream_C temp {from};   // constructed copy of from
    std::swap(temp, *this);      // move copy of from into *this -- temp now stuff from *this   
    return *this;                // *this is a copy of from 
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
char_stream_C& char_stream_C::operator=(char_stream_C&& from)
{
    M_msg1("operator=(char_stream_C &&) called -- move assignment -- \"") M_wide(from.m_in_filename) M_msg1("\" -> \"") M_wide(m_in_filename) M_msg2("\"")
        
    std::swap(m_in_file_p    , from.m_in_file_p    );
    std::swap(m_in_filename  , from.m_in_filename  );
    std::swap(m_char_stack   , from.m_char_stack   );
    return *this;                
}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// close() -- reset char_stream back to initial state
// -------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void char_stream_C::close() try  
{
    m_in_string_p = nullptr;
    m_in_file_p   = nullptr;
    m_source_id   = 0; 

    m_in_filename.clear();
    m_char_stack.clear();  
    return;
}
M_endf 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// set_xxxxx() -- functions to set condiguration parms
// -----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void char_stream_C::set_digraph_char(      char32_t   ch32      ) try { m_digraph_char     = ch32  ;                                                                                    return; } M_endf
void char_stream_C::set_process_digraphs(  bool       tf        ) try { m_process_digraphs = tf    ; M__(M_out(L"char_stream_C::set_process_digraphs(%d) -- called") % (int)tf;)        return; } M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// attach_file() -- assign pre-opened file to this char stream (will not own the file)
// -------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void char_stream_C::attach_file(infile_C* file_p) try  
{
    if (m_in_string_p != nullptr)                           // make sure there's no string already present
    {
        M_throw("char_stream_C::attach_file(): m_in_string_p is not 0 -- trying to get characters from both file and string at once")
        return;     // should not get here       
    }

    m_in_file_p   = file_p;                                 // just replace any existing file -- for reading more characters 

    if (m_in_file_p != nullptr)                             // real file attached?  
    {
        m_in_filename = m_in_file_p->get_filename();        // just replace any existing file -- for error messages
        m_source_id   = m_in_file_p->get_source_id();       // matching source ID 
    }
    else                                                    // must be clearing old file -- no new file
    {
        m_in_filename = std::wstring{};                     // just set empty string
        m_source_id   = 0; 
    } 

    return;
}
M_endf 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// attach_string() -- attach pre-filled input string to this char stream
// ---------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void char_stream_C::attach_string(instring_C* string_p) try     
{
    if (m_in_file_p != nullptr)                                  // make sure there's no file already present
    {
        M_throw("char_stream_C::attach_string(): m_in_file_p not 0 -- trying to get characters from both file and string at once")
        return;     // should not get here       
    }

    m_in_string_p   = string_p;                             // just replace any existing file -- for reading more characters 


    if (m_in_string_p != nullptr)                           // real string attached?  
    {
        m_in_filename = m_in_string_p->get_name();          // just replace any existing name -- for error messages
        m_source_id   = m_in_string_p->get_source_id();     // matching source ID 
    }
    else                                                    // must be clearing old file -- no new file
    {
        m_in_filename = std::wstring{};                     // just set empty string
        m_source_id   = 0;                                  // matching source ID 
    }

    return;
}
M_endf 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// get_source_char() -- obtain next character from file or string source (does digraph processing)  
// -----------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int char_stream_C::get_source_char(in_char_S& in_char)  try
{
    M__(M_out(L"char_stream_C::get_source_char() -- called");)


    // error if no source (file of string) is currently attached
    // ---------------------------------------------------------

    if ( (m_in_file_p == nullptr) && (m_in_string_p == nullptr) )
    {
        M_out_emsg(L"get_source_char() needed a character from the file or string, but no file or string was attached");

        in_char.source_id = 0; 
        in_char.lineno    = 0;                // no file/string 
        in_char.linepos   = 0;                // no file/string

        in_char.classs = char_E::error;
        ::char_classify(in_char);             // set type, subtype field to char_E::error, too

        in_char.wch1    = (wchar_t)0;         // no codepoint
        in_char.wch2    = (wchar_t)0;
        in_char.ch32    = (char32_t)0;

        return -1; 
    }
    
    // get next raw char from input source
    // -----------------------------------

    int rc;
        
    if (m_in_file_p != nullptr)        
        rc = m_in_file_p->get_char(in_char);   // should always return a char, even if error seen and R/C = -1  
    else
        rc = m_in_string_p->get_char(in_char); // should always return a char, even if error seen and R/C = -1 
    

    // handle digraph case  (if rc from above is non-0, shouldn't have digraph char in_char)
    // -------------------

    if  (
         (in_char.ch32 == m_digraph_char)
         &&
         m_process_digraphs
        )
    {
        M__(M_out(L"char_stream_C::get_source_char() -- digraph char seen");)
    
        // save away info and position of digraph char (for debugging)  -- call to get_char() below will overlay this info with info for following char 

        in_char.linepos_digraph = in_char.linepos;
        in_char.ch32_digraph    = in_char.ch32;
        in_char.wch1_digraph    = in_char.wch1;
        in_char.wch2_digraph    = in_char.wch2;
        in_char.digraph_count   = 1;


        // get char following the digraph char (if any)

        if (m_in_file_p != nullptr)        
            rc = m_in_file_p->get_char(in_char);   // should always return a char, even if error seen and R/C = -1  
        else
            rc = m_in_string_p->get_char(in_char); // should always return a char, even if error seen and R/C = -1 


        // save away 2nd char info before digraph processing

        in_char.ch32_orig    = in_char.ch32;
        in_char.wch1_orig    = in_char.wch1;
        in_char.wch2_orig    = in_char.wch2;


        // replace raw char following the digraph char with altered code point  
        
        switch (in_char.ch32)
        {
            case utf32_N::APOSTROPHE                :        in_char.ch32 = utf32_N::ACUTE_ACCENT                                ;   break;       //        `'   replaced with ´       -- escape char in type-2 strings
            case utf32_N::LESS_THAN_SIGN            :        in_char.ch32 = utf32_N::LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK   ;   break;       //        `<   replaced with «       -- start  char in type-2 strings
            case utf32_N::GREATER_THAN_SIGN         :        in_char.ch32 = utf32_N::RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK  ;   break;       //        `>   replaced with »       -- end    char in type-2 strings
            case utf32_N::TILDE                     :        in_char.ch32 = utf32_N::NOT_SIGN                                    ;   break;       //        `~   replaced with ¬       -- used in ¬= etc.
                                                                                                                                 
            case utf32_N::LATIN_SMALL_LETTER_C      :        in_char.ch32 = utf32_N::CENT_SIGN                                   ;   break;       //        `c   replaced with ¢       -- for use in strings
            case utf32_N::DIGIT_FOUR                :        in_char.ch32 = utf32_N::VULGAR_FRACTION_ONE_QUARTER                 ;   break;       //        `4   replaced with ¼       -- for use in strings
            case utf32_N::DIGIT_TWO                 :        in_char.ch32 = utf32_N::VULGAR_FRACTION_ONE_HALF                    ;   break;       //        `2   replaced with ½       -- for use in strings
            case utf32_N::DIGIT_THREE               :        in_char.ch32 = utf32_N::VULGAR_FRACTION_THREE_QUARTERS              ;   break;       //        `3   replaced with ¾       -- for use in strings
     
        

            // handle error cases -- unexpected data following digraph character (or another digraph char)
            // -------------------------------------------------------------------------------------------
           
            default :             
                if (in_char.ch32 != m_digraph_char)                 // just map 2nd digraph char to itself
                {
                    if (in_char.classs != char_E::normal)
                    {
                        M_out_emsg(L"char_stream_C::get_source_char() --  end-of-line, end-of_input, or I/O error (in_char_S.classs=%d) seen right after digraph character ('%c') -- source=(%d) = «%S»   lineno=%d linepos=%d")
                        % (int)(in_char.classs) % (wchar_t)(in_char.ch32_digraph) % in_char.source_id % get_cached_id(in_char.source_id) % in_char.lineno % in_char.linepos; 
                    }
                    else
                    {
                        M_out_emsg(L"char_stream_C::get_source_char() --  unsupported char '%c' following digraph char ('%c') -- source_id=(%d) = «%S»    lineno=%d linepos=%d")
                        % (wchar_t)(in_char.ch32) % (wchar_t)(in_char.ch32_digraph) % in_char.source_id % get_cached_id(in_char.source_id) % in_char.lineno % in_char.linepos;
                    }
           
                    in_char.classs = char_E::error;    // pass back error char
                    rc = -1; 
                    break; 
                }
        }   


        // if no error, update wide chars in in_char_S to match the altered ch32 value from digraph processing 
        
        if (in_char.classs == char_E::normal)
            to_wchars(in_char.ch32, in_char.wch1, in_char.wch2);
    }
    else                                                          // not digraph character
    {
        // inchar_S is already set up with 0's in digraph_count, digraph chars, etc. -- just need to copy orig char fields from final char fields

        in_char.ch32_orig = in_char.ch32; 
        in_char.wch1_orig = in_char.wch1; 
        in_char.wch2_orig = in_char.wch2;     
    }
             

    //  set in_char_S types, subtypes, family, etc. before passing back to upper layers 
    //  -------------------------------------------------------------------------------

    ::char_classify(in_char);                     // set subtype field (based on final results of any digraph processing) 
    return rc;  
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// get_char() -- get next (or any put-back) character in stream  
// ----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int char_stream_C::get_char(in_char_S& in_char)  try               // get (and consume) next character from stream  
{
     M__(M_out(L"char_stream_C::get_char() -- called");)


    // pass back top put-back character (if any)

    if (!m_char_stack.empty())
    {
        M__(M_out(L"char_stream_C::get_char() -- returning top char from stack");)
        in_char = m_char_stack.front();                            // get oldest in_char on stack -- subtype should have been filled in before it was put back earlier  
        m_char_stack.pop_front();                                  // remove oldest in_char on stack  
        return 0; 
    }
 

    // putback queue is empty -- need to go out to file/string to get next char
    // ------------------------------------------------------------------------
 
    static_N::character_count++;                                   // accumulate static character count
    return get_source_char(in_char); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// peek_char() -- return next (or n-th next) char without consuming it -- ends up on putback stack (or is left on putback stack)  
// -----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int char_stream_C::peek_char(in_char_S& in_char, size_t n) try  
{
    M__(M_out(L"char_stream_C::peek_char(%d) called") % n;) 


    // make sure 'n' is greater than 0 

    if (n <= 0)
    {
        M_out_emsg(L"char_stream_C::peek_char(in_char, %u) called with bad char position parm (%u)") % n % n;
        return -1; 
    }


    // pass back requested char, if it is on the existing putback stack -- oldest are at start [0], so n-th oldest has index [n-1]  

    size_t sz = m_char_stack.size();                                // number of put-back characters on stack 

    if (sz >= n)
    {                                                            
        in_char = m_char_stack.at(n-1);                             // get n-th oldest in_char -- subtype should have been filled in before it was put back earlier  
                                                                    // leave character on stack
        if (in_char.type == char_E::error)
            return -1;                                              // if returning prior error, return with error R/C
        else
        {
            return 0;                                               // else returned char is OK     
        }
    }


    // putback stack does not have enough characters -- need to go out to file to get next in_char's and put newest end of stack  

    if  ( (m_in_file_p == nullptr) && (m_in_string_p == nullptr) )
    {
        M_out_emsg(L"char_stream_C::peek_char(in_char, %u) needed an input character from the file or string, but no file or string was attached") % n;
        return -1; 
    }


    // Loop to add enough in_char's to new end of putback stack, so that n-th oldest character is on the putback stack
    // ---------------------------------------------------------------------------------------------------------------
    
    int rc = 0; 
    in_char_S ch;

    for (auto i = 0;  i < (n - sz); ++i)
    {
        int rci;

        rci = get_source_char(ch); 
        if (rci != 0) rc = rci;            // remember any non-zero R/C from infile_C::get_char()
        ::char_classify(ch);               // fill in type, subtype, etc. in in_char_S before putting it on putback stack.  
        m_char_stack.push_back(ch);        // add newest char to back end of putback stack
    }

    if (m_char_stack.size() != n)
    {
        M_out_emsg(L"char_stream_C::peek_char(in_char, %u) -- unexpected number of elements (%u) on putback stack") % n % m_char_stack.size();
        rc = -1;
    }

    in_char = m_char_stack.back();   // passing back last (newest) in_char on stack -- should be the right one, if R/C is 0   
    return rc; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// discard_char() -- discard top 'n'chars on putback stack (if any)  
// --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
int char_stream_C::discard_char(size_t n)  try             // discard next 'n' characters from stream  
{
    M__(M_out(L"char_stream_C::discard_char() -- called");)

    // make sure 'n' is greater than 0 

    if (n <= 0)
    {
        M_out_emsg(L"char_stream_C::discard_char(in_char, %llu) called with bad char count (%ull)") % n % n;
        return -1; 
    }
     

    //  see if there are enough characters on stack to discard all requested ones

    int rc = 0; 
    size_t nd = M_min(n, m_char_stack.size());
    if (nd < n) rc = -1; 

    
    //  discard first n put-back characters (if any)
    
     if (nd > 0)
     {
         auto iter = m_char_stack.begin();           // get random access iterator pointing to start of stack (oldest chars) 
         m_char_stack.erase(iter, iter + nd);        // remove first (oldest) nd in_char's on stack (does not remove iter+nd itself)                                                             
     }
     
     return rc;     
}
M_endf

 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// putback_char() -- put back char (onto queued character stack)  -- subtype is assumed to already be filled in from original get_char()
// --------------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
 
void char_stream_C::putback_char(const in_char_S& in_char) try       // putback character to stream -- no need to be connected to a file
{
    M__(M_out(L"char_stream_C::putback_char() -- called");)
    m_char_stack.push_front(in_char);      // push char onto stack -- becomes oldest on on stack
    return;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// is_empty() -- return true, if putback stack is empty  
// ----------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
bool char_stream_C::is_empty() const try
{     
    return m_char_stack.empty();
}
M_endf


 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// size() -- return number of put-back characters on queue  
// ------
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
size_t char_stream_C::size() const try
{     
    return m_char_stack.size();
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////