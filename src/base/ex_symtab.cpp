// ex_symtab.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =============
////     ex_symtab.cpp -- symbol-table/stack-frame-oriented functions
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
#include "h_ex_parse.h"
#include "h_ex_verb.h"      


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  forward declarations for selected static functions 
//  --------------------------------------------------

static std::wstring str_parmtype(        const parmtype_S&                                                );
static void         display_parmtype(    const parmtype_S&, const std::wstring&, const std::wstring& = L"");
static void         display_plist(       const plist_S&   , const std::wstring&, const std::wstring& = L"");


// local static variables
// ----------------------

namespace static_N
{
    uint64_t symval_ctr {0};                 // used for assigning symval_S serial numbers
}




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
////   display_slist() -- display passed-in slist 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_slist(const slist_S& slist, const std::wstring& ws, const std::wstring& mod, bool show_expressions, bool suppress_nesting, const std::wstring& nest) try
{
    // display data from slist header 

    M_out(L"%s %s %s: %80t slist: expression_ct=%d  main_label=«%s»") % ws % nest % mod % slist.expression_ct % slist.label;   


    // display all labels in this slist

    if (slist.labels.size() > 0)
        for (const auto& elem : slist.labels)
            M_out(L"%s %s %s %80t        label=«%s» %110t expression_index=%d") % ws % nest % mod % elem.first % elem.second;  
    

    // display expressions, if requested

    if ( (slist.expression_ct > 0) && (show_expressions) )
    {
        auto i = 0; 
        for (const auto& expression : slist.expressions)
        {
            M_out(L"- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
            display_expression(expression, ws, mod + L"{" + std::to_wstring(i++) + L"} ", suppress_nesting, nest + L"."); 
        }
        M_out(L"- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
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
////   display_expression() -- display passed-in expression, optionally with nested expressions, etc.
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_expression(const a_expression_S& expression, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    std::wstring type_s  {L"no verb   "}; 
     

    // set up type and location strings for main verb in this token

    if (expression.has_verb)   type_s = L"has verb  ";

    if (mod == L"") 
    {          
        M_out(L"%s %sexpression -- %-15s name=%s %77t -- parms:   %125t -- loc = %s -- main verb loc = %s" ) % ws % nest %      type_s % verb_name(expression) % expression_loc_str(expression) % verb_loc_str(expression);
    }
    else
    {
        M_out(L"%s %s%sexpression -- %-15s name=%s %77t -- parms: %125t -- loc = %s -- main verb loc = %s") % ws % nest % mod % type_s % verb_name(expression) % expression_loc_str(expression) % verb_loc_str(expression);
    }

    display_vlist(expression.lparms, ws, mod + L"(L )", suppress_nesting, nest + L".");
    display_vlist(expression.rparms, ws, mod + L"( R)", suppress_nesting, nest + L".");
        
    return; 
}
M_endf

///////////////////////////////////////////////////////

void display_expression(const e_expression_S& eval_expression, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    std::wstring type_s  {L"verb missing "}; 
     

    // set up type and location strings for main verb in this token

    if (eval_expression.has_verb)   type_s = L"has verb  ";

    if (mod == L"") 
    {          
        M_out(L"%s %sexpression -- %-15s name=%s %77t -- parms:   %125t -- loc = %s -- main verb loc = %s" ) % ws % nest %      type_s % verb_name(eval_expression) % expression_loc_str(eval_expression) % verb_loc_str(eval_expression);
    }
    else
    {
        M_out(L"%s %s%sexpression -- %-15s name=%s %77t -- parms: %125t -- loc = %s -- main verb loc = %s") % ws % nest % mod % type_s % verb_name(eval_expression) % expression_loc_str(eval_expression) % verb_loc_str(eval_expression);
    }

    display_vlist(eval_expression.lparms, ws, mod + L"(L )", suppress_nesting, nest + L".");
    display_vlist(eval_expression.rparms, ws, mod + L"( R)", suppress_nesting, nest + L".");
        
    return; 
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_vlist() -- display passed-in vlist 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_vlist(const vlist_S& vlist, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    // display data from vlist header 

    std::wstring flags {}; 

    if (vlist.val_mixed      ) flags += L"val_mixed "      ; 
    if (vlist.val_slist      ) flags += L"val_slist "      ;
    if (vlist.val_expression ) flags += L"val_expression " ;
    if (vlist.val_vlist      ) flags += L"val_vlist "      ; 
    if (vlist.val_verbdef    ) flags += L"val_verbdef "    ; 
    if (vlist.val_typdef     ) flags += L"val_typdef "     ; 
    if (vlist.val_array      ) flags += L"val_array "      ;
    if (vlist.val_structure  ) flags += L"val_structure "  ;
    if (vlist.val_ref        ) flags += L"val_ref "        ; 
    if (vlist.val_unit       ) flags += L"val_unit "       ;
    if (vlist.val_boolean    ) flags += L"val_boolean "    ;
    if (vlist.val_int8       ) flags += L"val_int8 "       ;
    if (vlist.val_int16      ) flags += L"val_int16 "      ;
    if (vlist.val_int32      ) flags += L"val_int32 "      ;
    if (vlist.val_int64      ) flags += L"val_int64 "      ; 
    if (vlist.val_uint8      ) flags += L"val_uint8 "      ;
    if (vlist.val_uint16     ) flags += L"val_uint16 "     ;
    if (vlist.val_uint32     ) flags += L"val_uint32 "     ;
    if (vlist.val_uint64     ) flags += L"val_uint64 "     ; 
    if (vlist.val_float64    ) flags += L"val_float64 "    ; 
    if (vlist.val_string     ) flags += L"val_string "     ; 
    if (vlist.val_identifier ) flags += L"val_identifier " ;

    M_out(L"%s %s%s -- %|80t| vlist: value_ct=%d  kw_ct=%d  %|125t| -- loc= %s -- flags= <%s>") % ws % nest % mod % vlist.value_ct % vlist.kw_ct % vlist_loc_str(vlist) % flags;   


    // display positional values, if any

    if (vlist.value_ct > 0)
    {
        auto i = 0; 
        for (const auto& value : vlist.values)
            display_value(value, ws, mod + L"[" + std::to_wstring(i++) + L"] ", suppress_nesting, nest + L".");     
    }


    // display evaluated/unevaluated keyword values, if any

    if (vlist.eval_kws.size() > 0)                    // display evaluated keywords, if any
    {
        for (const auto& elem : vlist.eval_kws)
            display_value(elem.second, ws, mod + L"[" + elem.first + L":] ", suppress_nesting, nest + L".");     
    }
    else                                              // no evaluated keywords 
    {
        if (vlist.keywords.size() > 0)                // display unevaluated keywords, if any
        {
            for (const auto& elem : vlist.keywords)
            {   
                if (elem.name.ty == type_E::string) 
                    display_value(elem.value, ws, mod + L"[<" + elem.name.string + L":>] ", suppress_nesting, nest + L"."); 
                else
                    display_value(elem.value, ws, mod + L"[<?expression?>:] "             , suppress_nesting, nest + L"."); 
            }
        }
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
////   display_value() -- display passed-in value 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_value(const value_S& value, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    
    // construct flags string
    // ----------------------

    std::wstring flags {}; 

    std ::wstring ix = std::to_wstring(value.token_ix1) + L":" + std::to_wstring(value.token_ix2); 

         if (value.ty == type_E::none        ) flags = L"none"         ; 
    else if (value.ty == type_E::no_value    ) flags = L"no_value "    ;
    else if (value.ty == type_E::identifier  ) flags = L"identifier "  ;
    else if (value.ty == type_E::verbname    ) flags = L"verbname "    ;
    else if (value.ty == type_E::keyname     ) flags = L"keyname "     ;
    else if (value.ty == type_E::vlist       ) flags = L"vlist ["      + fmt_ptr(value.vlist_sp.get()       ) + L"] "  ;
    else if (value.ty == type_E::expression  ) flags = L"expression (" + fmt_ptr(value.expression_sp.get()  ) + L") "  ;
    else if (value.ty == type_E::slist       ) flags = L"slist {"      + fmt_ptr(value.slist_sp.get()       ) + L"} "  ;
    else if (value.ty == type_E::verbdef     ) flags = L"verbdef <"    + fmt_ptr(value.verbdef_sp.get()     ) + L"> "  ;
    else if (value.ty == type_E::typdef      ) flags = L"typedef <"    + fmt_ptr(value.typdef_sp.get()      ) + L"> "  ;
    else if (value.ty == type_E::ref         ) flags = L"ref <"        + fmt_ptr(value.ref_sp.get()         ) + L"> "  ;
    else if (value.ty == type_E::array       ) flags = L"array <"      + fmt_ptr(value.buffer_sp.get()      ) + L"> "  ;
    else if (value.ty == type_E::structure   ) flags = L"structure <"  + fmt_ptr(value.buffer_sp.get()      ) + L"> "  ;
    else if (value.ty == type_E::unit        ) flags = L"unit "        ;
    else if (value.ty == type_E::boolean     ) flags = L"boolean "     ;
    else if (value.ty == type_E::int8        ) flags = L"int8 "        ; 
    else if (value.ty == type_E::int16       ) flags = L"int16 "       ; 
    else if (value.ty == type_E::int32       ) flags = L"int32 "       ; 
    else if (value.ty == type_E::int64       ) flags = L"int64 "       ; 
    else if (value.ty == type_E::uint8       ) flags = L"uint8 "       ; 
    else if (value.ty == type_E::uint16      ) flags = L"uint16 "      ; 
    else if (value.ty == type_E::uint32      ) flags = L"uint32 "      ; 
    else if (value.ty == type_E::uint64      ) flags = L"uint64 "      ;
    else if (value.ty == type_E::float32     ) flags = L"float32 "     ; 
    else if (value.ty == type_E::float64     ) flags = L"float64 "     ; 
    else if (value.ty == type_E::string      ) flags = L"string "      ; 
    else if (value.ty == type_E::error       ) flags = L"error "       ; 
    else if (value.ty == type_E::special     ) flags = L"special "     ; 
    else                                       flags = L" ???unknown value type???  " ; 
 
    M__(M_out(L"display_value() -- point 1 -- value.ty = %S") % type_str(value.ty));


    // display value of data for this value
    // ------------------------------------

    if      (value.ty == type_E::boolean )                                                                                                 
        M_out(L"%s %s%s %|80t|-- boolean    = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.boolean    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
    else if (value.ty == type_E::int8    )                                                                                                 
        M_out(L"%s %s%s %|80t|-- int8       = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.int8       % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::int16   )                                                                                                 
        M_out(L"%s %s%s %|80t|-- int16      = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.int16      % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::int32   )                                                                                                 
        M_out(L"%s %s%s %|80t|-- int32      = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.int32      % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::int64   )                                                                                                 
        M_out(L"%s %s%s %|80t|-- int64      = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.int64      % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if  (value.ty == type_E::uint8   )                                                                                                
        M_out(L"%s %s%s %|80t|-- uint8      = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.uint8      % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::uint16  )                                                                                                 
        M_out(L"%s %s%s %|80t|-- uint16     = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.uint16     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::uint32  )                                                                                                 
        M_out(L"%s %s%s %|80t|-- uint32     = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.uint32     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::uint64  )                                                                                                 
        M_out(L"%s %s%s %|80t|-- uint64     = %d    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.uint64     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::float32 )                                                                                                 
        M_out(L"%s %s%s %|80t|-- float32    = %.16f %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.float32    % fmt_ptr(&value) % flags % ix % value_loc_str(value);
                                                                                                                                                                              
    else if (value.ty == type_E::float64 )                                                                                                 
        M_out(L"%s %s%s %|80t|-- float64    = %.16f %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.float64    % fmt_ptr(&value) % flags % ix % value_loc_str(value);
                                                                                                                                          
    else if (value.ty == type_E::string  )                                                                                                 
        M_out(L"%s %s%s %|80t|-- string     = «%s»  %|125t| -- addr=%S  flags = <%s>   ix = %s -- %s")       % ws % nest % mod % value.string     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::identifier  )                                                                                             
        M_out(L"%s %s%s %|80t|-- identifier = «%s»  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.string     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::verbname    )                                                                                             
        M_out(L"%s %s%s %|80t|-- verbname   = «%s»  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.string     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::keyname     )                                                                                             
        M_out(L"%s %s%s %|80t|-- keyname    = «%s»  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")         % ws % nest % mod % value.string     % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
                                                                                                                                          
    else if (value.ty == type_E::vlist) 
    {
        M_out(L"%s %s%s %|80t|-- nested vlist %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")               % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 
        display_vlist(*(value.vlist_sp), ws, mod, suppress_nesting, nest);   
    }
 
    else if (value.ty == type_E::expression) 
    {
        M_out(L"%s %s%s %|80t|-- nested expression  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")              % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 

        if (!suppress_nesting)
            display_expression(*(value.expression_sp), ws, mod, suppress_nesting, nest);  
    }
 
    else if (value.ty == type_E::slist)
    { 
        M_out(L"%s %s%s %|80t|-- nested slist %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")               % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s %|80t|-- nested slist -- display suppressed")                                    % ws % nest % mod;
        else
            display_slist(*(value.slist_sp), ws, mod, true, suppress_nesting, nest);
    }
 
    else if (value.ty == type_E::verbdef)
    { 
        M_out(L"%s %s%s %|80t|-- nested verbdef %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")             % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s %|80t|-- nested verbdef -- display suppressed")                                  % ws % nest % mod;
        else
            display_verbdef( *(value.verbdef_sp) );  
    }
     
    else if (value.ty == type_E::typdef)
    { 
        M_out(L"%s %s%s %|80t|-- nested typedef %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")             % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s -- %|80t|-- nested typedef -- display suppressed")                               % ws % nest % mod;
        else
            display_typdef(L"", *(value.typdef_sp), L"" );                                           // empty type name, typedef_S, nest string (empty)  
    }
    
    else if (value.ty == type_E::ref)
    { 
        M_out(L"%s %s%s %|80t|-- nested ref %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                 % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s -- %|80t| nested ref -- display suppressed")                                     % ws % nest % mod;
        else
            display_ref(*(value.ref_sp));                                                           
    }
  
    else if (value.ty == type_E::array)
    { 
        M_out(L"%s %s%s %|80t|-- nested array %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")              % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s -- %|80t| nested typedef/buffer for array -- display suppressed")               % ws % nest % mod;
        else
        {
            display_typdef(L"array typedef", *(value.typdef_sp),      L"    " );                     // typedef_S, nest string (empty)      
            display_buffer(L"array buffer" , *(value.buffer_sp), 256, L"    " );                     // display (partial) data from buffer  
        }
    }

    else if (value.ty == type_E::structure)
    { 
        M_out(L"%s %s%s %|80t|-- nested structure %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")           % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);

        if (suppress_nesting)
            M_out(L"%s %s%s %|80t|-- nested typedef/buffer for structure -- display suppressed")             % ws % nest % mod;
        else
        {
            display_typdef(L"structure typedef", *(value.typdef_sp),      L"    " );                 // typedef_S, nest string (empty)      
            display_buffer(L"structure buffer" , *(value.buffer_sp), 256, L"    " );                 // display (partial) data from buffer  
        }                                                                                     
    }
   
    else if (value.ty == type_E::unit) 
        M_out(L"%s %s%s %|80t|-- unit     %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);  
    
    else if (value.ty == type_E::no_value) 
        M_out(L"%s %s%s %|80t|-- no_value %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 

    else if (value.ty == type_E::none) 
        M_out(L"%s %s%s %|80t|-- none     %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value); 

    else if (value.ty == type_E::error) 
        M_out(L"%s %s%s %|80t|-- error    %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);  
   
    else if (value.ty == type_E::special) 
        M_out(L"%s %s%s %|80t|-- special  %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s")                    % ws % nest % mod                    % fmt_ptr(&value) % flags % ix % value_loc_str(value);  

    else
        M_out(L"%s %s%s %|80t|-- ??unknown value type(%d)?? %|125t| -- addr=%S  flags = <%s>   ix=%s -- %s") % ws % nest % mod % (int)(value.ty)  % fmt_ptr(&value) % flags % ix % value_loc_str(value);  

    return; 
}
M_endf
 


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_results() -- display passed-in results 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_results(const results_S& results, const std::wstring& ws, const std::wstring& mod, bool suppress_nesting, const std::wstring& nest) try
{
    // display results flags and string

    std::wstring flags { }; 

    if (results.multiple_results             ) flags += L"multiple_results "           ; 
    if (results.no_results                   ) flags += L"no_results "                 ; 
    if (results.re_eval_expression_results   ) flags += L"re_eval_expression_results " ;
    if (results.builtin_verb_results         ) flags += L"builtin_verb_results "       ;
    if (results.special_results              ) flags += L"special_results "            ; 
    if (results.error                        ) flags += L"error "                      ; 
    if (results.break_flag                   ) flags += L"break_flag "                 ;
    if (results.continue_flag                ) flags += L"continue_flag "              ;
    if (results.quit_flag                    ) flags += L"quit_flag "                  ; 
    if (results.end_flag                     ) flags += L"end_flag "                   ;
    if (results.leave_flag                   ) flags += L"leave_flag "                 ;
    if (results.goto_flag                    ) flags += L"goto_flag "                  ;
    if (results.xctl_flag                    ) flags += L"xctl_flag "                  ;
    if (results.return_flag                  ) flags += L"return_flag "                ;
    if (results.throw_flag                   ) flags += L"throw_flag "                 ;
    if (results.skip_flag                    ) flags += L"skip_flag "                  ;

     M_out(L"%s %s%s %|80t|-- results.flags = <%s>"  ) % ws % nest % mod % flags;
     M_out(L"%s %s%s %|80t|-- results.str   = \"%S\"") % ws % nest % mod % results.str;

    // display the value_S structure imbedded in results_S

    display_value(results, ws, mod, suppress_nesting, nest);    
    return; 
}
M_endf


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   results_msg_string() -- pass back string fof error messages 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

std::wstring results_msg_string(const results_S& results) try
{
    // display results flags and string

    std::wstring ret_ws { }; 

    if (results.multiple_results             ) ret_ws += L"multiple_results "           ; 
    if (results.no_results                   ) ret_ws += L"no_results "                 ; 
    if (results.re_eval_expression_results   ) ret_ws += L"re_eval_expression_results " ;
    if (results.builtin_verb_results         ) ret_ws += L"builtin_verb_results "       ;
    if (results.special_results              ) ret_ws += L"special_results "            ; 
    if (results.error                        ) ret_ws += L"error "                      ; 
    if (results.break_flag                   ) ret_ws += L"@BREAK "                     ;
    if (results.continue_flag                ) ret_ws += L"@CONTINUE "                  ;
    if (results.quit_flag                    ) ret_ws += L"@QUIT "                      ; 
    if (results.end_flag                     ) ret_ws += L"@END "                       ;
    if (results.leave_flag                   ) ret_ws += L"@LEAVE "                     ;
    if (results.goto_flag                    ) ret_ws += L"@GOTO "                      ;
    if (results.xctl_flag                    ) ret_ws += L"@XCTL "                      ;
    if (results.return_flag                  ) ret_ws += L"@RETURN "                    ;
    if (results.throw_flag                   ) ret_ws += L"@THROW "                     ;
    if (results.skip_flag                    ) ret_ws += L"@SKIP "                      ;

    if (results.str.size() > 0)
        ret_ws += ( std::wstring { L" string = \""} + results.str + std::wstring { L"\"" });
    
    return ret_ws; 
}
M_endf

    

////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_buffer() -- display length and (partial) contents of buffer_C object 
////
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_buffer(const std::wstring& ws, const buf8_T& buffer, size_t max_bytes, const std::wstring& indent) try
{
    // put out heading line, if caller provided one

    if (ws.size() > 0)
        M_out(L"\n                  %S%S%80t -- buffer length = %d = 0x%04X  (displayed bytes = %d) -- addr=%S") % indent % ws % buffer.sz1 % buffer.sz1 % std::min(max_bytes, buffer.sz1) % fmt_ptr(&buffer); 


    // display buffer data -- blank lines before and after -- max bytes to display is set by caller

    M_out(L" "); 
    show_hex( (void *)(buffer.p1),  std::min(buffer.sz1, max_bytes), indent + L"                 ", false );  // false = suppress heading in show_hex()
    M_out(L" ");

    return; 
}
M_endf 
  


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_verbdef()   -- displays verb definition details  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// helper functions for display_verbdef()

static std::wstring str_parmtype(const parmtype_S& parmtype) try
{
    std::wstring parm_flags { };

    if (parmtype.int64_range           )   parm_flags += L"int64_range        "   ;    
    if (parmtype.float64_range         )   parm_flags += L"float64_range      "   ;
    if (parmtype.no_eval_ident         )   parm_flags += L"no_eval_ident      "   ;
    if (parmtype.no_eval_expression    )   parm_flags += L"no_eval_expression "   ;
    if (parmtype.no_eval_vlist         )   parm_flags += L"no_eval_vlist      "   ;
    if (parmtype.anything_ok           )   parm_flags += L"anything_ok        "   ;
    if (parmtype.nval_ok               )   parm_flags += L"nval_ok            "   ; 
    if (parmtype.unit_ok               )   parm_flags += L"unit_ok            "   ; 
    if (parmtype.int8_ok               )   parm_flags += L"int8_ok            "   ; 
    if (parmtype.int16_ok              )   parm_flags += L"int16_ok           "   ; 
    if (parmtype.int32_ok              )   parm_flags += L"int32_ok           "   ; 
    if (parmtype.int64_ok              )   parm_flags += L"int64_ok           "   ; 
    if (parmtype.uint8_ok              )   parm_flags += L"uint8_ok           "   ; 
    if (parmtype.uint16_ok             )   parm_flags += L"uint16_ok          "   ; 
    if (parmtype.uint32_ok             )   parm_flags += L"uint32_ok          "   ; 
    if (parmtype.uint64_ok             )   parm_flags += L"uint64_ok          "   ; 
    if (parmtype.float32_ok            )   parm_flags += L"float32_ok         "   ;
    if (parmtype.float64_ok            )   parm_flags += L"float64_ok         "   ; 
    if (parmtype.string_ok             )   parm_flags += L"string_ok          "   ; 
    if (parmtype.verbname_ok           )   parm_flags += L"verbname_ok        "   ; 
    if (parmtype.check_local_env_only  )   parm_flags += L"ck_local_env       "   ; 
    if (parmtype.check_global_env_only )   parm_flags += L"ck_global_env      "   ;
    if (parmtype.raw_ident_ok          )   parm_flags += L"raw_ident_ok       "   ;
    if (parmtype.var_ident_ok          )   parm_flags += L"var_ident_ok       "   ; 
    if (parmtype.const_ident_ok        )   parm_flags += L"const_ident_ok     "   ; 
    if (parmtype.undef_ident_ok        )   parm_flags += L"undef_ident_ok     "   ;
    if (parmtype.vlist_ok              )   parm_flags += L"vlist_ok           "   ;                 
    if (parmtype.expression_ok         )   parm_flags += L"expression_ok      "   ; 
    if (parmtype.slist_ok              )   parm_flags += L"slist_ok           "   ;
    if (parmtype.verbdef_ok            )   parm_flags += L"verbdef_ok         "   ;
    if (parmtype.typdef_ok             )   parm_flags += L"typdef_ok          "   ;
    if (parmtype.array_ok              )   parm_flags += L"array_ok           "   ;
    if (parmtype.structure_ok          )   parm_flags += L"structure_ok       "   ;
    if (parmtype.lvalue_ref_ok         )   parm_flags += L"lvalue_ref_ok      "   ;
    if (parmtype.rvalue_ref_ok         )   parm_flags += L"rvalue_ref_ok      "   ;
    
    return parm_flags; 
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_parmtype(const parmtype_S& parmtype, const std::wstring& ws, const std::wstring& nest) try
{
    // display summary info for this parmtype_S

    M__(M_out(L"display_parmtype() called -- ws = «%s»") % ws;)


    // gather up int64 and float64 ranges

    std::wstring ranges {};
    std::wstring minmax {};

    if (parmtype.int64_range)
        ranges += L" [" + std::to_wstring(parmtype.int64_min  )  + L" to " + std::to_wstring(parmtype.int64_max  ) + L"] ";

    if (parmtype.float64_range)
        ranges += L" [" + std::to_wstring(parmtype.float64_min)  + L" to " + std::to_wstring(parmtype.float64_max) + L"] ";


    // make printable string with keyword min/max counts (if not 0-0 = optional single keyword) 

    if ((parmtype.kw_min_ct != 0) || (parmtype.kw_max_ct != 0) )
    {
        minmax = L" min/max= " + std::to_wstring(parmtype.kw_min_ct) + L"/";

        if (parmtype.kw_max_ct == -1)
            minmax += L"N";
        else
            minmax += std::to_wstring(parmtype.kw_max_ct);
    }


    // put out one line with parmtype info

    M_out(L"%25t--%s%s%s%s %125t-- %s") % nest % ws % minmax % ranges % str_parmtype(parmtype); 


    // display any attached plist, with individual sub-parm requirement info 

    if (parmtype.plist_sp.get() != nullptr)
    {
        display_plist(*(parmtype.plist_sp), ws + L": nested plist", nest + L" ");     
    }

    return;
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_plist(const plist_S& plist, const std::wstring& ws, const std::wstring& nest) try
{
    M__(M_out(L"display_plist() called -- ws = «%s»") % ws;)

    std::wstring parm_flags { }; 

    if (plist.no_check_keywords          )   parm_flags += L"no_chk_keywords    "    ; 
    if (plist.no_check_keyword_names     )   parm_flags += L"no_chk_kw_names    "    ;
    if (plist.no_check_positional        )   parm_flags += L"no_chk_positional  "    ;
    if (plist.no_eval_ident              )   parm_flags += L"no_eval_ident      "    ;                 
    if (plist.no_eval_expression         )   parm_flags += L"no_eval_expression "    ; 
    if (plist.no_eval_vlist              )   parm_flags += L"no_eval_vlist      "    ; 

    std::wstring max_str{ }; 

    if (plist.max_ct == -1)
        max_str = L"N";
    else
        max_str = std::to_wstring(plist.max_ct);


    M_out(L"%25t--%s%s: parms min/max=%3d/%-3s  keywords=%-3d %125t-- %s") 
      % nest
      % ws
      % plist.min_ct 
      % max_str 
      % plist.keywords.size()
      % parm_flags;


    // display any keyword conflict sets for this plist 

    if (plist.conflicts.size() > 0)
    {
        int i {0}; 

        for (const auto& cset : plist.conflicts)   // loop through vector of conflict sets 
        {
            std::wstring kws {};                   // list of keywords in conflict set 
            
            for(const auto& kw : cset)             // loop through each keyword name in conflict set 
                kws += kw + L":   ";               // add keyword name to printable string              
      
            M_out(L"%25t--%s%s: kw conflict set [%d] : %125t-- %s") % nest % ws % i % kws; 
            i++; 
        }    
    }


    // display any keyword choice sets for this plist 

    if (plist.choices.size() > 0)
    {
        int i {0}; 

        for (const auto& cset : plist.choices)     // loop through vector of choice sets 
        {
            std::wstring kws {};                   // list of keywords in choice set 
            
            for(const auto& kw : cset)             // loop through each keyword name in choice set 
                kws += kw + L":   ";               // add keyword name to printable string              
      
            M_out(L"%25t--%s%s: kw choise(s) set [%d] : %125t-- %s") % nest % ws % i % kws; 
            i++; 
        }    
    }
   

    // display any keyword match sets for this plist 

    if (plist.matches.size() > 0)
    {
        int i {0}; 

        for (const auto& mset : plist.matches)     // loop through vector of match sets 
        {
            std::wstring kws {};                   // list of keywords in match set 
            
            for(const auto& kw : mset)             // loop through each keyword name in conflict set 
                kws += kw + L":   ";              // add keyword name to printable string              
      
            M_out(L"%25t--%s%s: kw match set    [%d] : %125t-- %s") % nest % ws % i % kws; 
            i++; 
        }    
    }


    // individually display and positional parms that have parmtype_S descriptions for this plist

    if (plist.values.size() > 0)
    {
        int i {0};  

        for (const auto& elem : plist.values) 
        {
            display_parmtype(elem, L"              parm[" + std::to_wstring(i) + L"]", nest + L" ");
            i++; 
        }
    }


    // individually display any keywords that are present for this plist 

    if (plist.keywords.size() > 0)
    {
        for (const auto& elem : plist.keywords)    
            display_parmtype(elem.second, L"              keyword = " + elem.first + L":", nest + L" ");
    }
        
    return;
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_argvar(const argvar_S& argvar, const std::wstring& ws) try
{
    M__(M_out(L"display_argvar() called -- ws = «%s»") % ws;)

    std::wstring arg_str {   };
    int var_ct           { 0 }; 


    // add variables for positional parms to string for display

    for (const auto var_name: argvar.pos)
    {
        arg_str += var_name; 
        arg_str += std::wstring { L" " };
        var_ct++;  
    }

    if (var_ct > 0)                           
        arg_str += std::wstring { L"   "};          // add extra space between positional and keyword variable names


    // add keyword:variable parms to string for display

    for (const auto elem: argvar.key)              // elem.first -- string = keyword name,  elem.second -- string = variable name
    {
        arg_str += elem.first;
        arg_str += std::wstring { L":" };
        arg_str += elem.second;
        arg_str += std::wstring { L" " };
        var_ct++;  
    }


    // display printable string with argvars, if any were present

    if (var_ct > 0)
        M_out(L"%25t--%S: argvars = %S") % ws % arg_str;
 
    return;
}
M_endf




//////////////////////////////////////////////////////////////////////////////////////////////////////
// display_verbdef()
//////////////////////////////////////////////////////////////////////////////////////////////////////

void display_verbdef(const verbdef_S& verbdef) try
{
    std::wstring parm_flags { }; 
  
    if (verbdef.simplified_call     )   parm_flags += L"simplified_call  "        ;
    if (verbdef.parms_same_type     )   parm_flags += L"parms_same_type  "        ;
    if (verbdef.parms_some_required )   parm_flags += L"parms_some_required  "    ;
    if (verbdef.parms_left_xor_right)   parm_flags += L"parms_left_xor_right  "   ;
    if (verbdef.parms_not_both_sides)   parm_flags += L"parms_not_both_sides  "   ;
    if (verbdef.right_associate     )   parm_flags += L"right_associate  "        ;
    if (verbdef.left_associate      )   parm_flags += L"left_associate  "         ;
    if (verbdef.lexical_scope       )   parm_flags += L"lexical_scope  "          ;
    if (verbdef.dynamic_scope       )   parm_flags += L"dynamic_scope  "          ;  
    if (verbdef.is_builtin          )   parm_flags += L"(built-in verb)  "        ;   


    // put out line with verbdef global info
    

    M_out(L"%25t-- fcn_p=%S  slist_p=%S     priority=%-8d %125t-- %s -- addr=%S") 
          % fmt_ptr(verbdef.fcn_p)
          % fmt_ptr(verbdef.slist_sp.get())
          % verbdef.priority
          % parm_flags
          % fmt_ptr(&verbdef)
          ;



    // display left-side and right-side argvar_S structures for this verb

    display_argvar(verbdef.lvars, L" left-side " );
    display_argvar(verbdef.rvars, L" right-side" );


    // display left-side and right-side plists for this verb

    display_plist(verbdef.lparms, L" left-side " );
    display_plist(verbdef.rparms, L" right-side" );

    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_typdef()   -- displays type definition details  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_typdef(const std::wstring& desc, const typdef_S& ty, const std::wstring& indent_str) try
{
    // put out line with typdef main info
 
    M_out(L"                  %S%S%80t-- kind=%d=%S%102t tsize=%-2d (%04X)  acount=%d  fdefs.size=%d -- addr=%S") 
         % indent_str
         % desc 
         % (int)(ty.kind)
         % type_str(ty.kind)
         % ty.tsize
         % ty.tsize
         % ty.acount
         % ty.fdefs.size()
         % fmt_ptr(&ty)
         ;

    // display nested array element types, or structure field types

    if (ty.atype_sp.get() != nullptr)
         display_typdef(L"array element:", *(ty.atype_sp), indent_str + L"    ");

    if (ty.fnames.size() > 0)
    {
        for (auto& elem : ty.fnames)     // elem.first = field name (std::wstring) , elem.second = uint64_t -- index into fdefs vector 
        { 
            if (ty.fdefs.size() > elem.second )
                display_fieldef(elem.first, ty.fdefs.at(elem.second), indent_str + L"    ");
            else
                M_out(L"%Sunexpected error -- ty.fdefs too small to display this field's fieldef_S") % indent_str;
        }
    }

    return; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////

void display_fieldef(const std::wstring& field_name, const fieldef_S& fd, const std::wstring& indent_str) try
{
    // put out line with fieldef/typdef main info  

   M_out(L"%S%S %80t-- field offset=%-4d (%04X)  typedef info:") 
        % indent_str 
        % field_name 
        % fd.offset
        % fd.offset
        ;

    if (fd.ftype_sp.get() != nullptr) 
        display_typdef(L"", *(fd.ftype_sp), indent_str + L"    ");
    else
        M_out(L"%Sunexpected error -- typdef_sp = not initialized") % indent_str;

    return; 
}
M_endf

 

////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_ref()   -- displays ref details  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_ref(const ref_S& ref) try
{
    // get printable flags string

    std::wstring flags { }; 

    if (ref.in_buffer   )        flags += L"in_buffer " ;
    if (ref.is_lvalue   )        flags += L"is_lvalue " ;
    if (ref.is_rvalue   )        flags += L"is_rvalue " ;
    if (ref.auto_deref  )        flags += L"auto_deref ";


    // put out line with ref main info
 
    M_out(L"%80t-- offset=%d (0x%04X)  flags=<%S>   typdef_sp=%S    value_sp=%S") 
         % ref.offset
         % ref.offset
         % flags 
         % fmt_ptr(ref.typdef_sp.get())
         % fmt_ptr(ref.value_sp.get())
         ;

    // display nested array element types, or structure field types

    if (ref.typdef_sp.get() != nullptr)
         display_typdef(L"ref -- associated typdef", *(ref.typdef_sp), L"");

    if (ref.value_sp.get() != nullptr)
         display_value(*(ref.value_sp), L"             ", L"ref -- associated value:", false, L"    ");

    return; 
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


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   symval_flag_str() -- creates printable string for symval_S flag bit settings  -- internal function
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static std::wstring symval_flag_str(const symval_S& symval) try
{
    std::wstring str    { };                    // start with empty string


    // add builtin prefix to (empty) string

    if (symval.is_builtin)
        str += L"built-in "; 
         

    // add basic type to string

    if (symval.is_verbdef)
    {
        str += L"verbdef";
    }
    else if (symval.is_typdef)
    {
        str += L"typedef";
    }
    else                                        // must be var or const 
    {  
        if (symval.is_const) 
            str += L"const"; 
        else
            str += L"var";
    }
             

    // add alias and exported flags (assume string is not empty at this point

    std::wstring spacer = { };                  // spacing string between output words -- starts off empty

    if (symval.is_alias)
    {
        str += L" -- alias";
        spacer = std::wstring { L" " };  
    }

    if (symval.is_exported)
        str += (spacer + L" -- exported"); 

    return str; 
}
M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_all_vars() -- displays all variables currently in the symbol table (omits fixed verb definitions, but includes variables holding a verb definition)
////   display_vars()     -- display selected verbs
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_all_vars(frame_S& frame_in) try
{
    int n = 0;                                // accumulated variable counter

    frame_S *frame_p  = frame_in.symbols_p;       // point to incoming frame_S valid local symbol table


    M_out(L"--------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Upward scope stack frame chain from starting stack-frame:%d") % frame_p->serial;


    // outer loop to display local symbol tables for each nested frame_S -- chain backward using upward scope pointer
    // ------------------------------------------------------------------------------------------------------------

    do
    {
        M_out(L"-------------------------------------------------------------\n");
        std::wstring flags { }; 

        if (frame_p->exports_done       ) flags += L"exports done  "       ;
        if (frame_p->is_verb            ) flags += L"is_verb  "            ;  
        if (frame_p->is_main            ) flags += L"is_main  "            ; 
        if (frame_p->is_block           ) flags += L"is_block  "           ;
        if (frame_p->is_preparse        ) flags += L"is_preparse  "        ;
        if (frame_p->symtab_valid       ) flags += L"symtab_valid  "       ;
        if (frame_p->lexical_scope      ) flags += L"lexical_scope  "      ;
        if (frame_p->dynamic_scope      ) flags += L"dynamic_scope  "      ;
        if (frame_p->block_scope        ) flags += L"block_scope  "        ;

        if (frame_p->is_main)
            M_out(L"Main stack frame:%d %30t-- total number of identifiers = %d %122t -- flags = %s  -- list of variables:\n")   % frame_p->serial   %  (frame_p->symtab.symbols.size()) % flags;
        else
            M_out(L"Nested stack frame:%d %30t-- total number of identifiers = %d %122t -- flags = %s  -- list of variables:\n") % frame_p->serial   %  (frame_p->symtab.symbols.size()) % flags;
        
        for(const auto& elem : frame_p->symtab.symbols)
        {
            std::wstring convar { }; 
      
            if ( !elem.second.is_alias )
            {
                if ( (elem.second.is_verbdef) || (elem.second.is_typdef) )    
                    continue;                                                       // skip over any verb/type definitions in symbol table -- include all aliases (even to verbdef or typdef)
            }

            M_out(L"n= %-3d ID= %-20s %79t (%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serial;
            display_value(*(elem.second.value_sp), L"                                                                  ", L"", false);
            n++;
        }

        M_out(L" ");

        if (frame_p->scope_sp.use_count() > 0)                                      // don't advance past main stack frame
           frame_p = frame_p->scope_sp->symbols_p;                                  // advance upward to next frame_S that has a symbol table (for now, all frame_S have valid symbol tables) 
        else                                                                      
           frame_p = nullptr;                                                       // no more sack frames
    }                                                                             
    while (frame_p != nullptr);                                                     // stop loop after all stack frames are done 
                                                                                  

    // display global symbol table
    // ---------------------------
         
    M_out(L"Global symbol table -- total number of identifiers = %d -- list of variables:\n") % get_global_symtab()->symbols.size() ;

    for(const auto& elem : get_global_symtab()->symbols) 
    {
        if (!elem.second.is_alias)
        {
            if ( (elem.second.is_verbdef) || (elem.second.is_typdef) )
                continue;                                                             // skip over any verb/type definitions in symbol table (but do show aliases, even of verbdef or typdef)
        }

        M_out(L"n= %-3d ID= %-20s %79t (%S)   use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serial;
        display_value(*(elem.second.value_sp),     L"                                                                  ", L"", false);
        n++; 
    }   
      
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");

    return; 
}
M_endf


////////////////////////////////////////////////////////////////////////

void display_vars(frame_S& frame, const vlist_S& vlist) try
{
    M_out(L"--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Selected variables/constants visible from stack-frame:%d") % frame.serial;
    M_out(L"---------------------------------------------------------------------\n");


    // loop to look at value_S structures visible from this scope for passed-in verb-name 

    int n = 0; 
    for (const auto& value : vlist.values)
    {
        symval_S symval { };                                                                                                                                         // will cause use_ct() to be one more than expected
        auto grc = get_var(frame, value.string, symval);
    
        if (grc == 0)
        {
            M_out(L"n=%-3d ID= %-20s %79t (%S)  use_ct=%d  serial=%d") % n % value.string % symval_flag_str(symval) % (symval.value_sp.use_count() - 1) % symval.serial;  // correct for extra use_ct()
            display_value(*(symval.value_sp),    L"                                                                  ", L"", false); 
        }
        else
            M_out(L"n=%-3d ID= %-20s %79t -- variable is undefined or not visible from this stack frame") % n % value.string; 

        M_out(L" ");
        n++; 
    }

    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"); 
    return; 
}
M_endf


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_all_verbdefs() -- displays all verbs currently in the verb table 
////   display_verbdefs()     -- displays subset of verbs  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_all_verbdefs(frame_S& frame_in, bool show_builtin, bool show_defined) try
{
    int n = 0;                                                     // accumulated verb counter
                                                                 
    frame_S *frame_p  = frame_in.symbols_p;                            // point to incoming frame_S with valid symbol table for passsed-in frame_S (should always be frame_in, at least for now)


    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"List of all verbs visible from starting scope/frame:%d") % frame_p->serial; 
   

    // outer loop to display local symbol tables for each nested frame_S (use upward_scope_sp to chain backwards through the stack frames, some of which might not be on the active stack)
    // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    do
    {
        M_out(L"----------------------------------------------------------------\n");

        std::wstring flags { }; 

        if (frame_p->exports_done       ) flags += L"exports_done  "       ;
        if (frame_p->is_verb            ) flags += L"is_verb  "            ; 
        if (frame_p->is_block           ) flags += L"is_block "            ; 
        if (frame_p->is_preparse        ) flags += L"is_preparse "         ; 
        if (frame_p->is_main            ) flags += L"is_main  "            ; 
        if (frame_p->symtab_valid       ) flags += L"symtab_valid  "       ;
        if (frame_p->lexical_scope      ) flags += L"lexical_scope  "      ;
        if (frame_p->dynamic_scope      ) flags += L"dynamic_scope  "      ;
        if (frame_p->block_scope        ) flags += L"block_scope  "        ;

        if (frame_p->is_main)
            M_out( L"Main stack frame [%d] %30t-- total number of identifiers = %d %122t -- flags = %s  -- list of verbs:\n") 
                 % frame_p->serial
                 % frame_p->symtab.symbols.size() 
                 % flags
                 ;
        else
            M_out( L"Stack frame [%d] %30t-- total number of identifiers = %d %122t -- flags = %s  -- list of verbs:\n")
                 % frame_p->serial 
                 % frame_p->symtab.symbols.size() 
                 % flags
                 ;
        
        for(const auto& elem : frame_p->symtab.symbols)
        {
            if (elem.second.is_verbdef)                             // only look at verb definitions (not variables currently containing verb definitions)
            {
                if (
                    (   elem.second.is_builtin  && show_builtin )
                    ||
                    ( (!elem.second.is_builtin) && show_defined )
                   )
                {
                    M_out(L"n=%-3d ID= %-13s (%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count();
                    display_verbdef(*(elem.second.value_sp->verbdef_sp)); 
                   // M_out(L" ");
                    n++;    
                }
            }
        }


        // chain upward to next scope (if any)

        if (frame_p->scope_sp.use_count() == 0)                      // don't advance past main stack frame  (which should have an uninitialized  upward scope pointer)
           frame_p = nullptr;                                        // no more frame_S's for this loop
        else                                                     
           frame_p = frame_p->scope_sp.get();                        // advance upward to next frame_S that has a symbol table (every frame_S has a synmbol table, for now)
    }                                                            
    while (frame_p != nullptr);                                      // stop loop after all frame_S's are done 


    // display verbs in global symbol table
    // ------------------------------------
         
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Global symbol table -- total number of identifiers = %d -- list of verbs:") % (get_global_symtab()->symbols.size()) ;
    M_out(L"------------------------------------------------------------------------------\n");

    for(const auto& elem : get_global_symtab()->symbols) 
    {
        if (elem.second.is_verbdef)                                  // only show verbs (not variables or constants currently containing a verb definition)
        {
            if (
                (   elem.second.is_builtin  && show_builtin )
                ||
                ( (!elem.second.is_builtin) && show_defined )
               )
            {
                M_out(L"n=%-3d ID= %-13s (%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serial;
                display_verbdef(*(elem.second.value_sp->verbdef_sp)); 
                n++;
            }
        }
    }   
      
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");

    return; 
}
M_endf


////////////////////////////////////////////////////////////////////////

void display_verbdefs(frame_S& frame, const vlist_S& vlist) try
{
    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Selected verbs visible from stack-frame:%d") % frame.serial;
    M_out(L"-------------------------------------------------------\n");


    // loop to look at verbdef_S structure visible from this scope for passed-in verb-name 

    int n = 0; 
    for (const auto& value : vlist.values)
    {
        symval_S symval { };                                                                                                                                         // will cause use_ct() to be one more than expected
        auto grc = get_verb(frame, value.string, symval);
    
        if (grc == 0)
        {
            M_out(L"n=%-3d ID= %-13s (%S)  use_ct=%d  serial=%d") % n % value.string % symval_flag_str(symval) % (symval.value_sp.use_count() - 1) % symval.serial;  // correct for extra use_ct()
            display_verbdef(*(symval.value_sp->verbdef_sp)); 
        }
        else
            M_out(L"n=%-3d ID= %s%25t-- verb is undefined or not visible from this stack frame") % n % value.string; 

        M_out(L" ");
        n++; 
    }

    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"); 
    return; 
}
M_endf



////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   display_all_typdefs() -- displays all verbs currently in the verb table 
////   display_typdefs()     -- displays subset of verbs  
////                   
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void display_all_typdefs(frame_S& frame_in, bool show_builtin, bool show_defined) try
{
    int n = 0;                                                         // accumulated verb counter
                                                                 
    frame_S *frame_p  = frame_in.symbols_p;                            // point to incoming frame_S with valid symbol table for passsed-in frame_S (should always be frame_in, at least for now)


    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"List of all typedefs visible from starting scope/frame:%d") % frame_p->serial; 
   

    // outer loop to display local symbol tables for each nested frame_S (use upward_scope_sp to chain backwards through the stack frames, some of which might not be on the active stack)
    // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    do
    {
        M_out(L"-------------------------------------------------------------------\n");

        std::wstring flags { }; 

        if (frame_p->exports_done       ) flags += L"exports_done  "       ;
        if (frame_p->is_verb            ) flags += L"is_verb  "            ; 
        if (frame_p->is_block           ) flags += L"is_block "            ; 
        if (frame_p->is_preparse        ) flags += L"is_preparse "         ; 
        if (frame_p->is_main            ) flags += L"is_main  "            ; 
        if (frame_p->symtab_valid       ) flags += L"symtab_valid  "       ;
        if (frame_p->lexical_scope      ) flags += L"lexical_scope  "      ;
        if (frame_p->dynamic_scope      ) flags += L"dynamic_scope  "      ;
        if (frame_p->block_scope        ) flags += L"block_scope  "        ;

        if (frame_p->is_main)
            M_out( L"Main stack frame [%d] %30t-- total number of identifiers = %d %122t -- flags = %s  -- list of typedefs:\n") 
                 % frame_p->serial
                 % frame_p->symtab.symbols.size() 
                 % flags
                 ;
        else
            M_out( L"Stack frame [%d] %30t-- total number of identifiers = %d %122t -- flags = %s  -- list of typedefs:\n")
                 % frame_p->serial 
                 % frame_p->symtab.symbols.size() 
                 % flags
                 ;
        
        for(const auto& elem : frame_p->symtab.symbols)
        {
            std::wstring convar { }; 
      
            if (elem.second.is_typdef)                               // only look at type definitions (not variables currently containing type definitions) -- include aliases, though
            {
                if (
                    (   elem.second.is_builtin  && show_builtin )
                    ||
                    ( (!elem.second.is_builtin) && show_defined )
                   )
                {
                    M_out(L"n= %-3d ID= %-13s %78t (%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serial;
                    display_typdef(L"", *(elem.second.value_sp->typdef_sp), L""); 
                    M_out(L" ");
                    n++;    
                }
            }
        }


        // chain upward to next scope (if any)

        if (frame_p->scope_sp.use_count() == 0)                      // don't advance past main stack frame  (which should have an uninitialized  upward scope pointer)
           frame_p = nullptr;                                        // no more frame_S's for this loop
        else                                                     
           frame_p = frame_p->scope_sp.get();                        // advance upward to next frame_S that has a symbol table (every frame_S has a synmbol table, for now)
    }                                                            
    while (frame_p != nullptr);                                      // stop loop after all frame_S's are done 


    // display typdefs in global symbol table
    // ---------------------------------------
         
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Global symbol table -- total number of identifiers = %d -- list of typedefs:") % (get_global_symtab()->symbols.size()) ;
    M_out(L"---------------------------------------------------------------------------------\n");

    for(const auto& elem : get_global_symtab()->symbols) 
    {
        if (elem.second.is_typdef)                                  // only show typdefs (not variables or constants currently containing a type definition) -- include aliases, though
        {
            if (
                (   elem.second.is_builtin  && show_builtin )
                ||
                ( (!elem.second.is_builtin) && show_defined )
               )
            {
                M_out(L"n= %-3d ID= %-13s %78t (%S)  use_ct=%d  serial=%d") % n % elem.first % symval_flag_str(elem.second) % elem.second.value_sp.use_count() % elem.second.serial;
                display_typdef(L"", *(elem.second.value_sp->typdef_sp), L""); 
                n++;
            }
        }
    }   
      
    M_out(L"------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");

    return; 
}
M_endf


////////////////////////////////////////////////////////////////////////

void display_typdefs(frame_S& frame, const vlist_S& vlist) try
{
    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
    M_out(L"Selected typedefs visible from stack-frame:%d") % frame.serial;
    M_out(L"----------------------------------------------------------\n");


    // loop to look at typdef_S structure visible from this scope for passed-in type-name 

    int n = 0; 
    for (const auto& value : vlist.values)
    {
        symval_S symval { };                                                                                                                                                     // will cause use_ct() to be one more than expected
        auto grc = get_typdef(frame, value.string, symval);
    
        if (grc == 0)
        {
            M_out(L"n=%-3d ID= %-13s %78t (%S)  use_ct=%d  serial=%d") % n % value.string % symval_flag_str(symval) % (symval.value_sp.use_count() - 1) % symval.serial;    // correct for extra use count
            display_typdef(L"", *(symval.value_sp->typdef_sp), L""); 
        }
        else
            M_out(L"n=%-3d ID= %s%25t-- typedef is undefined or not visible from this stack frame") % n % value.string; 

        M_out(L" ");
        n++; 
    }

    M_out(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"); 
    return; 
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



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   find_symtab_ident() -- internal function -- find symtab_S containing requested identifier (returns pointer to symbol table) 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int find_symtab_ident(frame_S& frame, const std::wstring& ident, symtab_S *& symtab_p) try
{
    bool exported_only { false };                                    // true means variable is found only if is_exported flag is on

    M__(M_out(L"find_symtab_ident() 1 -- called -- &frame = %X") % &frame;) 


    // search nested non-global stack frames for requested identifier 
    // --------------------------------------------------------------

    frame_S *frame_p  {frame.symbols_p};                             // point to the frame_S containing the active symbol table for the incoming frame_S  


    for(;;)                                                          // main loop to look at all chained stack frames in the scope chain starting at passed-in frame_S
    {
        if ((frame_p->symtab).symbols.count(ident) > 0)              // identifier found in symbol table?
        {
             if (!exported_only)                                     // all variables in this symbol table are visible to caller's frame_S ?   (i.e. this is local symtab)
             {
                 symtab_p = &(frame_p->symtab);                      // pass back pointer to located symbol table
                 return 0; 
             }

             //  identifiers in this symbol table are visible only if exported 

             if ((frame_p->symtab).symbols.at(ident).is_exported)   // is this variable exported ? 
             {
                 symtab_p = &(frame_p->symtab);                     // pass back pointer to located symbol table
                 return 0; 
             }  
        }    
    

        // end loop after main frame_S has been searched  (don't look at global symtab in this loop -- see below) 

        if ( (frame_p->is_main) || (frame_p->scope_sp.use_count() == 0) )
            break;


        // if this is an frame_S using dynamic upward scoping (created by only verb evaluation), set exported-only flag so that only exported symbols are found from now on (except in global symbol table, whch are always exported, anyway)  
        // (for lexical or block scoping, all upward symbols are visible)

        if (frame_p->dynamic_scope)
            exported_only = true;


        // advance to next frame_S (using upwards scope pointer) that has a symbol table (all frame_S should have symbol table, at least for now)

       frame_p = frame_p->scope_sp->symbols_p; 
    }


    // Look for identifier in global symtab_S 
    // --------------------------------------
    //
    // control gets here when all nested frame_S symbol tables have been searched, and identifier is not found
    // see if variable is defined in global symbol table

    if (((frame.global_p)->symbols).count(ident) > 0)               // identifier in global table?   (global variables are always exported)
    {                                                         
        symtab_p = frame.global_p;                                  // pass back pointer to global symbol table 
        return 0;
    }
    else                                                            // identifier not defined in any local or global symbol table
    {
        symtab_p = nullptr;                                         // pass back no pointer 
        return -1;                                                  // indicate failure
    } 
}
M_endf    



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   def_local_identifier()   -- sets value of passed-in identifier -- defines new variable/const/verb/typedef in local  symbol table 
////   def_global_identifier()  -- sets value of passed-in identifier -- defines new variable/const/verb/typedef in global symbol table 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int def_identifier(symtab_S& symtab, const std::wstring& ident, symval_S& symval, bool unshare) try
{
    M__(M_out(L"def_identifier() -- passed-in name = %s") % ident;)
 

    // error if this identifier already exists

    auto ct = symtab.symbols.count(ident);

    if (ct > 0)
    {
        count_error(); 
        M_out_emsg(L"def_identifier() -- identifier = \"%s\" is already defined -- cannot be redefined") % ident; 
        return -1;      
    }


    // make sure any nested objects are unshared, if requested by caller
  
    if (unshare)                             
        unshare_value(*(symval.value_sp)); 


    // add in new identifier to symbol table

    symval.serial = ++(static_N::symval_ctr);
    auto rc = symtab.symbols.emplace(ident, symval);     

    if (!rc.second)
        M_throw_v("def_identifier() -- unexpected symtab.symbols.emplace(%s, symval) failure") % out_ws(ident) ));

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_identifier(frame_S& frame, const std::wstring& ident, symval_S& symval, bool unshare) try
{  
    return def_identifier(frame.symbols_p->symtab, ident, symval, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_identifier(const std::wstring& ident, symval_S& symval, bool unshare) try
{
    return def_identifier(*get_global_symtab(), ident, symval, unshare); 
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

static int make_identifier_const(symtab_S& symtab, const std::wstring& ident) try
{
    // see if identifier already exists, and is constant

    auto ct = symtab.symbols.count(ident);

    if (ct > 0)
    {
        symtab.symbols.at(ident).is_const = true;
        return 0; 
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
////   undef_local_identifier()   -- removes value of passed-in identifier -- undefines new variable/const/verb in local symbol table 
////   undef_global_identifier()  -- removes value of passed-in identifier -- undefines new variable/const/verb in global symbol table 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int undef_identifier(symtab_S& symtab, const std::wstring& ident) try
{
    // see if identifier already exists, and is constant (and not alias, which is always deletable)

    auto ct = symtab.symbols.count(ident);

    if (ct > 0)
    {
       if (!symtab.symbols.at(ident).is_alias)       // don't complain about builtin or constant aliases
       {
           if (symtab.symbols.at(ident).is_const)
           {
               count_error(); 
               M_out_emsg(L"undef_identifier() -- cannot remove constant \"%s\"") % ident; 
               return -1;
           }
         
           if (symtab.symbols.at(ident).is_builtin) 
           {
               count_error(); 
               M_out_emsg(L"undef_identifier() -- cannot remove built-in verb \"%s\"") % ident; 
               return -1;
           }
       }

       symtab.symbols.erase(ident);                   // get rid of old value 
    }

    return 0;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////

int undef_local_identifier(frame_S& frame, const std::wstring& ident) try
{  
    return undef_identifier(frame.symbols_p->symtab, ident); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int undef_global_identifier(const std::wstring& ident) try
{
    return undef_identifier(*get_global_symtab(), ident); 
}
M_endf 


////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////          is_local_identifier_xxxx()   --  return flag settings for global identifier
////          is_global_identifier_xxxx()  --  return flag settings for local identifier
////   
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static bool is_identifier_defined(symtab_S& symtab, const std::wstring& ident) try
{
    auto ct = symtab.symbols.count(ident); 

    if (ct > 1)                                 // something wrong -- should never be more than 1 identifier in symbol table
        M_throw_v("is_identifier_defined() --  %d entries in symbol table were found for identifier %s") % ct % out_ws(ident) )); 
    
    if (symtab.symbols.count(ident) <= 0)          // see if identifier is currently defined 
        return false;
    else
        return true;  
}
M_endf


////////////////////////////////////////////////////////////////////

static bool is_identifier_variable(symtab_S& symtab, const std::wstring& ident) try
{
    if (!is_identifier_defined(symtab, ident))          
        return false;
    else
    {     
        if  (
             (symtab.symbols.at(ident).is_verbdef )
             ||
             (symtab.symbols.at(ident).is_typdef )
             ||
             (symtab.symbols.at(ident).is_const)
            )
           return false; 
        else
            return true; 
    }
}
M_endf

//////////////////////////////////////////////////////////

static bool is_identifier_const(symtab_S& symtab, const std::wstring& ident) try
{
    if (!is_identifier_defined(symtab, ident)) 
        return false;
    else
        return symtab.symbols.at(ident).is_const;  
}
M_endf

//////////////////////////////////////////////////////////

static bool is_identifier_verb(symtab_S& symtab, const std::wstring& ident) try
{
    if (!is_identifier_defined(symtab, ident)) 
        return false;
    else
        return symtab.symbols.at(ident).is_verbdef;  
}
M_endf

//////////////////////////////////////////////////////////

static bool is_identifier_typdef(symtab_S& symtab, const std::wstring& ident) try
{
    if (!is_identifier_defined(symtab, ident)) 
        return false;
    else
        return symtab.symbols.at(ident).is_typdef;  
}
M_endf

//////////////////////////////////////////////////////////

static bool is_identifier_builtin(symtab_S& symtab, const std::wstring& ident) try
{
    if (!is_identifier_defined(symtab, ident))
        return false;
    else
        return symtab.symbols.at(ident).is_builtin;  
}
M_endf


//////////////////////////////////////////////////////////

static bool is_identifier_alias(symtab_S& symtab, const std::wstring& ident) try
{
    if (!is_identifier_defined(symtab, ident))
        return false;
    else
        return symtab.symbols.at(ident).is_alias;  
}
M_endf


////////////////////////////////////////////////////////////////////

static bool is_identifier_mutable(symtab_S& symtab, const std::wstring& ident) try
{
    if (!is_identifier_defined(symtab, ident))
        return false;
    else
    {     
        if  (
             (symtab.symbols.at(ident).is_builtin)
             ||
             (symtab.symbols.at(ident).is_const  )
            )
           return false; 
        else
           return true; 
    }
}
M_endf 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_defined(frame_S& frame, const std::wstring& ident) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any symbol table
         return false;                                              // failure r/c
    else                                                            // symbol table found with identifier
        return true;                                                // proper symbol table found -- must be defined
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
bool is_identifier_variable(frame_S& frame, const std::wstring& ident) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any symbol table
         return false;                                              // failure r/c
    else                                                            // symbol table found with identifier
        return is_identifier_variable(*p, ident);                   // proper symbol table found -- check for non-constant variable
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool is_identifier_const(frame_S& frame, const std::wstring& ident) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any symbol table
         return false;                                              // failure r/c
    else                                                            // symbol table found with identifier
        return is_identifier_const(*p, ident);                      // proper symbol table found -- check constness
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_verb(frame_S& frame, const std::wstring& ident) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any symbol table
         return false;                                              // failure r/c
    else                                                            // symbol table found with identifier
        return is_identifier_verb(*p, ident);                       // proper symbol table found -- check for verb
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_typdef(frame_S& frame, const std::wstring& ident) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any symbol table
         return false;                                              // failure r/c
    else                                                            // symbol table found with identifier
        return is_identifier_typdef(*p, ident);                     // proper symbol table found -- check for typdef
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_builtin(frame_S& frame, const std::wstring& ident) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any symbol table
         return false;                                              // failure r/c
    else                                                            // symbol table found with identifier
        return is_identifier_builtin(*p, ident);                    // proper symbol table found -- check for verb
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_alias(frame_S& frame, const std::wstring& ident) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any symbol table
         return false;                                              // failure r/c
    else                                                            // symbol table found with identifier
        return is_identifier_alias(*p, ident);                      // proper symbol table found -- check for verb
}
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_identifier_mutable(frame_S& frame, const std::wstring& ident) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                                                    // identifier not found in any symbol table
         return false;                                              // failure r/c
    else                                                            // symbol table found with identifier
        return is_identifier_mutable(*p, ident);                    // proper symbol table found -- check for mutability
}
M_endf




////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool is_local_identifier_defined(    frame_S& frame, const std::wstring& ident) try   { return is_identifier_defined(   frame.symbols_p->symtab, ident); } M_endf
bool is_global_identifier_defined(                   const std::wstring& ident) try   { return is_identifier_defined(    *get_global_symtab()  , ident); } M_endf

bool is_local_identifier_variable(   frame_S& frame, const std::wstring& ident) try   { return is_identifier_variable(  frame.symbols_p->symtab, ident); } M_endf
bool is_global_identifier_variable(                  const std::wstring& ident) try   { return is_identifier_variable(   *get_global_symtab()  , ident); } M_endf

bool is_local_identifier_const(      frame_S& frame, const std::wstring& ident) try   { return is_identifier_const(     frame.symbols_p->symtab, ident); } M_endf
bool is_global_identifier_const(                     const std::wstring& ident) try   { return is_identifier_const(      *get_global_symtab()  , ident); } M_endf

bool is_local_identifier_verb(       frame_S& frame, const std::wstring& ident) try   { return is_identifier_verb(      frame.symbols_p->symtab, ident); } M_endf
bool is_global_identifier_verb(                      const std::wstring& ident) try   { return is_identifier_verb(       *get_global_symtab()  , ident); } M_endf

bool is_local_identifier_typdef(       frame_S& frame, const std::wstring& ident) try { return is_identifier_typdef(    frame.symbols_p->symtab, ident); } M_endf
bool is_global_identifier_typdef(                      const std::wstring& ident) try { return is_identifier_typdef(     *get_global_symtab()  , ident); } M_endf

bool is_local_identifier_builtin(    frame_S& frame, const std::wstring& ident) try   { return is_identifier_builtin(   frame.symbols_p->symtab, ident); } M_endf
bool is_global_identifier_builtin(                   const std::wstring& ident) try   { return is_identifier_builtin(    *get_global_symtab()  , ident); } M_endf

bool is_local_identifier_alias(      frame_S& frame, const std::wstring& ident) try   { return is_identifier_alias(     frame.symbols_p->symtab, ident); } M_endf
bool is_global_identifier_alias(                     const std::wstring& ident) try   { return is_identifier_alias(      *get_global_symtab()  , ident); } M_endf
           
bool is_local_identifier_mutable(    frame_S& frame, const std::wstring& ident) try   { return is_identifier_mutable(   frame.symbols_p->symtab, ident); } M_endf
bool is_global_identifier_mutable(                   const std::wstring& ident) try   { return is_identifier_mutable(    *get_global_symtab()  , ident); } M_endf




////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_identifier()        -- gets value of passed-in identifier (-1 if not in local or global symbol table, or is not visible in other symbol tables) 
////   get_local_identifier()  -- gets value of passed-in identifier (-1 if not in local symbol table)
////   get_global_identifier() -- gets value of passed-in identifier (-1 if not in global symbol table)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// internal function -- version that returns value from symval entry

static int get_identifier(symtab_S& symtab, const std::wstring& ident, value_S& value) try
{
    if (!is_identifier_defined(symtab, ident))              // return with R/C = -1 and unit value, if identifier is not there 
    {
        value = unit_val(); 
        return -1;
    }
    else                                                    // identifier is found -- might be verb or variable/constant
    {
        // identifier is defined -- return associated value

        value = *(symtab.symbols.at(ident).value_sp);      // value found -- pass it back
        return 0; 
    }
}
M_endf


// internal function -- version that returns complete symval entry

static int get_identifier(symtab_S& symtab, const std::wstring& ident, symval_S& symval) try
{
    if (!is_identifier_defined(symtab, ident))              // return with R/C = -1 and empty value, if identifier is not there 
    {
        symval = symval_S {}; 
        return -1;
    }
    else                                                    // identifier is found -- might be verb or variable/constant
    {
        // identifier is defined -- return associated value

        symval = symtab.symbols.at(ident);                 // value found -- pass it back
        return 0; 
    }
}
M_endf




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_local_identifier(frame_S& frame, const std::wstring& ident, value_S& value) try
{
    return get_identifier(frame.symbols_p->symtab, ident, value);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_global_identifier(const std::wstring& ident, value_S& value) try
{
    return get_identifier(*get_global_symtab(), ident, value);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

int get_identifier(frame_S& frame, const std::wstring& ident, value_S& value) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                     // variable not found in any symbol table
    {
        value = unit_val();          // pass back unit value
        return -1;                   // failure r/c
    }
    else                             // symbol table found with identifier
    {
        return get_identifier(*p, ident, value);    
    }
}
M_endf    



///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   export_local_identifier()  -- alter exported status of local identifier (global identifiers are always exported and can't be unexported)
////              
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int export_identifier(symtab_S& symtab, const std::wstring& ident, bool is_exported) try
{
    // error if identifier is not already in the symbol table

    auto ct = symtab.symbols.count(ident);

    if (!is_identifier_defined(symtab, ident))
    {
        count_error(); 
        M_out_emsg(L"export_identifier() -- identifier = \"%s\" to be exported/unexported is not defined") % ident; 
        return -1;  
    }


    // set is_exported flag in identifier in symbol table -- do in-place update

    symtab.symbols.at(ident).is_exported = is_exported;     // update is_exported flag in symval_S for identifier 

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int export_local_identifier(frame_S& frame, const std::wstring& ident,  bool is_exported) try
{  
    return export_identifier(frame.symbols_p->symtab, ident, is_exported); 
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



 ////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   def_local_verb()  -- defines new verb in local  symbol table (which might be the global symbol table if this is main frame_S)
////   def_global_verb() -- defines new verb in global symbol table (which might be the current local symbol table if this is main frame_S)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int def_verb(symtab_S& symtab, const std::wstring& verbname, const verbdef_S& verbdef_in, bool is_builtin, bool is_exported, bool unshare) try
{
    M__(M_out(L"def_verb() -- passed-in name = %s") % verbname;)
 

    // error if this verb is already defined

    if (is_identifier_defined(symtab, verbname))
    {
        count_error(); 
        M_out_emsg(L"def_verb() -- identifier = \"%s\" is already defined -- cannot be redefined as verb") % verbname; 
        return -1;      
    }


    // construct verbdef_S for new verb -- can't alter passed-in verbdef, so make new local one with the const/exported flags, and any unshared slist 

    verbdef_S verbdef { };

    verbdef              = verbdef_in;                      // any nested objects are still shared
 
 

    // add in new verb to passed-in symbol table

    symval_S                  symval    {             };   // local symtab_S to fill in 
    std::shared_ptr<value_S>  value_sp  { new value_S };   // temporary shared pointer -- will release ownership when this function returns 

    symval.is_verbdef  = true; 
    symval.is_builtin  = is_builtin;
    symval.value_sp    = value_sp;
    *(symval.value_sp) = verbdef_val(verbdef); 

    return def_identifier(symtab, verbname, symval, unshare);
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_verb(frame_S& frame, const std::wstring& verbname, const verbdef_S& verbdef, bool is_builtin, bool is_exported, bool unshare) try
{  
    return def_verb(frame.symbols_p->symtab, verbname, verbdef, is_builtin, is_exported, unshare); 
}
M_endf

///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_verb(const std::wstring& verbname, const verbdef_S& verbdef, bool is_builtin, bool unshare) try
{  
    return def_verb(*get_global_symtab(), verbname, verbdef, is_builtin, true, unshare); 
}
M_endf
 



///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   undef_local_verb()  -- removes passed-in verb (if it is present in local symbol table)    -- no complaint if not there
////   undef_global_verb() -- removes passed-in verb (if it is present in global symbol table)   -- no complaint if not there
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int undef_verb(symtab_S& symtab, const std::wstring& verbname) try
{
    // return immediately, if identifier is not defined

    if (!is_identifier_defined(symtab, verbname))
    {
        count_error(); 
        M_out_emsg(L"undef_verb() -- verb \"%s\" (to be removed) is not defined") % verbname; 
        return -1;
    }


    // make sure this identifier is for a verb, and not built-in (always allow alias of built-in verb to be removed)

    if (!is_identifier_verb(symtab, verbname))
    {
        count_error(); 
        M_out_emsg(L"undef_verb() -- identifier \"%s\" is not a verb -- cannot undefine") % verbname; 
        return -1;      
    }

    if ( is_identifier_builtin(symtab, verbname) && (!is_identifier_alias(symtab, verbname)) )
    {
        count_error(); 
        M_out_emsg(L"undef_verb() -- verb \"%s\" is built-in -- cannot undefine") % verbname; 
        return -1;      
    }

    return undef_identifier(symtab, verbname);   
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_local_verb(frame_S& frame, const std::wstring& verbname) try
{
    return undef_verb(frame.symbols_p->symtab, verbname);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_global_verb(const std::wstring& verbname) try
{
    return undef_verb(*get_global_symtab(), verbname);
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_verb()        -- gets verbdef_S of passed-in verbname (-1 if not in local or global symbol table, or is not visible in other symbol tables) 
////   get_local_verb()  -- gets verbdef_S of passed-in verbname (-1 if not in local symbol table)
////   get_global verb() -- gets verbdef_S of passed-in verbname (-1 if not in global symbol table)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ----------------------------
// version that returns verbdef
// ----------------------------

static int get_verb(symtab_S& symtab, const std::wstring& verbname, verbdef_S& verbdef) try
{
    if (!is_identifier_defined(symtab, verbname))           // return with R/C = -1 and empty verbdef, if verb is not there 
    {
        verbdef = verbdef_S { };
        return -1;                   
    }
    else
    {
        value_S value { }; 
        auto grc = get_identifier(symtab, verbname, value);

        if (grc == 0)
        {
            if (value.verbdef_sp.get() != nullptr)
            {
                verbdef = *(value.verbdef_sp);              // return verbdef from value associated with this verb identifier or variable 
                return 0;  
            }                                               
            else                                            // error -- verbdef_sp is empty -- no verbdef for this variable -- return default/empty verbdef
            {
                verbdef = verbdef_S { };
                return -1;             
            }
        }
        else                                                // error -- identifier not found -- return default/empty verbdef  ???? should not occur ?????
        {
            verbdef = verbdef_S { };
            return -1;           
        }
    }
}
M_endf


// --------------------------------------------
// version that returns complete symval_S entry
// --------------------------------------------

static int get_verb(symtab_S& symtab, const std::wstring& verbname, symval_S& out_symval) try
{
    if (!is_identifier_defined(symtab, verbname))           // return with R/C = -1 and empty value, if verb is not there 
    {
        out_symval = symval_S { };
        return -1;                   
    }
    else
    {
        symval_S symval { }; 
        auto grc = get_identifier(symtab, verbname, symval);

        if (grc == 0)
        {
            if (symval.value_sp->verbdef_sp.get() != nullptr)
            {
                out_symval = symval;                        // return symval or this verb identifier 
                return 0;  
            }                                               
            else                                            // error -- verbdef_sp is empty -- no verbdef for this variable -- return default/empty verbdef
            {
                out_symval = symval_S { };
                return -1;             
            }
        }
        else                                                // error -- identifier not found -- return default/empty verbdef  ???? should not occur ?????
        {
            out_symval = symval_S { };
            return -1;           
        }
    }
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_local_verb(frame_S& frame, const std::wstring& verbname, verbdef_S& verbdef) try
{
    return get_verb(frame.symbols_p->symtab, verbname, verbdef);
}
M_endf


int get_local_verb(frame_S& frame, const std::wstring& verbname, symval_S& symval) try
{
    return get_verb(frame.symbols_p->symtab, verbname, symval);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_global_verb(const std::wstring& verbname, verbdef_S& verbdef) try                                                                              
{
    return get_verb(*get_global_symtab(), verbname, verbdef);
}
M_endf


int get_global_verb(const std::wstring& verbname, symval_S& symval) try                                                                              
{
    return get_verb(*get_global_symtab(), verbname, symval);
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

int get_verb(frame_S& frame, const std::wstring& verbname, verbdef_S& verbdef) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, verbname, p);

    if (rc != 0)                                   // identifier not found in any symbol table
    {                                             
        verbdef = verbdef_S { };                   // pass back empty verbdef_S   
        return -1;                                 // failure r/c
    }                                             
    else                                           // symbol table found with verb
    {
        return get_verb(*p, verbname, verbdef);    
    }
}
M_endf  


int get_verb(frame_S& frame, const std::wstring& verbname, symval_S& symval) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, verbname, p);

    if (rc != 0)                                   // identifier not found in any symbol table
    {                                             
        symval = symval_S { };                     // pass back empty symval_S   
        return -1;                                 // failure r/c
    }                                             
    else                                           // symbol table found with verb
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



 ////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   def_local_typdef()  -- defines new typdef in local  symbol table (which might be the global symbol table if this is main frame_S)
////   def_global_typdef() -- defines new typdef in global symbol table (which might be the current local symbol table if this is main frame_S)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int def_typdef(symtab_S& symtab, const std::wstring& typdef_name, const typdef_S& typdef_in, bool is_builtin, bool is_exported, bool unshare) try
{
    M__(M_out(L"def_typdef() -- passed-in name = %s") % typdef_name;)
 

    // error if this typdef is already defined

    if (is_identifier_defined(symtab, typdef_name))
    {
        count_error(); 
        M_out_emsg(L"def_typdef() -- identifier = \"%s\" is already defined -- cannot be redefined as typedef") % typdef_name; 
        return -1;      
    }


    // construct typdef_S for new typdef -- can't alter passed-in typdef, so make new local one with the const/exported flags, and any unshared nested typdefs

    typdef_S typdef { };

    typdef = typdef_in;                                      // any nested objects are still shared      
 

    // add in new typdef to passed-in symbol table

    symval_S                 symval    {             };      // local symval_S to fill in 
    std::shared_ptr<value_S> value_sp  { new value_S };      // temporary shared pointer -- will release ownership when this function returns 

    symval.is_typdef     = true; 
    symval.is_builtin    = is_builtin;
    symval.value_sp      = value_sp;
    *(symval.value_sp)   = typdef_val(typdef); 

    return def_identifier(symtab, typdef_name, symval, unshare);
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_typdef(frame_S& frame, const std::wstring& typdef_name, const typdef_S& typdef, bool is_builtin, bool is_exported, bool unshare) try
{  
    return def_typdef(frame.symbols_p->symtab, typdef_name, typdef, is_builtin, is_exported, unshare); 
}
M_endf

///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_typdef(const std::wstring& typdef_name, const typdef_S& typdef, bool is_builtin, bool unshare) try
{  
    return def_typdef(*get_global_symtab(), typdef_name, typdef, is_builtin, true, unshare); 
}
M_endf
 



///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   undef_local_typdef()  -- removes passed-in typdef (if it is present in local symbol table)    -- no complaint if not there
////   undef_global_typdef() -- removes passed-in typdef (if it is present in global symbol table)   -- no complaint if not there
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int undef_typdef(symtab_S& symtab, const std::wstring& typdef_name) try
{
    // return immediately, if identifier is not defined

    if (!is_identifier_defined(symtab, typdef_name))
    {
        count_error(); 
        M_out_emsg(L"undef_typdef() -- typedef \"%s\" (to be removed) is not defined") % typdef_name; 
        return -1;
    }


    // make sure this identifier is for a typdef, and not built-in (but always allow alias of built-in typdef to be deleted)

    if (!is_identifier_typdef(symtab, typdef_name))
    {
        count_error(); 
        M_out_emsg(L"undef_typdef() -- identifier \"%s\" is not a typedef -- cannot undefine") % typdef_name; 
        return -1;      
    }

    if ( is_identifier_builtin(symtab, typdef_name) && (!is_identifier_alias(symtab, typdef_name)) )
    {
        count_error(); 
        M_out_emsg(L"undef_typdef() -- typedef \"%s\" is built-in -- cannot undefine") % typdef_name; 
        return -1;      
    }

    return undef_identifier(symtab, typdef_name);   
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_local_typdef(frame_S& frame, const std::wstring& typdef_name) try
{
    return undef_typdef(frame.symbols_p->symtab, typdef_name);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_global_typdef(const std::wstring& typdef_name) try
{
    return undef_typdef(*get_global_symtab(), typdef_name);
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_typdef()        -- gets typdef_S of passed-in typename (-1 if not in local or global symbol table, or is not visible in other symbol tables) 
////   get_local_typdef()  -- gets typdef_S of passed-in typename (-1 if not in local symbol table)
////   get_global_typdef() -- gets typdef_S of passed-in typename (-1 if not in global symbol table)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ----------------------------
// version that returns typedef
// ----------------------------  

static int get_typdef(symtab_S& symtab, const std::wstring& typdef_name, typdef_S& typdef) try
{
    if (!is_identifier_defined(symtab, typdef_name))           // return with R/C = -1 and empty typedef_S, if typdef is not there 
    {
        typdef = typdef_S { };
        return -1;                   
    }
    else
    {
        value_S value { }; 
        auto grc = get_identifier(symtab, typdef_name, value);

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
// version that returns complete symval_S entry
// --------------------------------------------

static int get_typdef(symtab_S& symtab, const std::wstring& typdef_name, symval_S& symval_out) try
{
    if (!is_identifier_defined(symtab, typdef_name))           // return with R/C = -1 and empty value, if typdef is not there 
    {
        symval_out = symval_S { };
        return -1;                   
    }
    else
    {
        symval_S symval { }; 
        auto grc = get_identifier(symtab, typdef_name, symval);

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
 
int get_local_typdef(frame_S& frame, const std::wstring& typdef_name, typdef_S& typdef) try
{
    return get_typdef(frame.symbols_p->symtab, typdef_name, typdef);
}
M_endf


int get_local_typdef(frame_S& frame, const std::wstring& typdef_name, symval_S& symval) try
{
    return get_typdef(frame.symbols_p->symtab, typdef_name, symval);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_global_typdef(const std::wstring& typdef_name, typdef_S& typdef) try                                                                              
{
    return get_typdef(*get_global_symtab(), typdef_name, typdef);
}
M_endf


int get_global_typdef(const std::wstring& typdef_name, symval_S& symval) try                                                                              
{
    return get_typdef(*get_global_symtab(), typdef_name, symval);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

int get_typdef(frame_S& frame, const std::wstring& typdef_name, typdef_S& typdef) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, typdef_name, p);

    if (rc != 0)                                   // identifier not found in any symbol table
    {                                             
        typdef = typdef_S { };                     // pass back empty typdef_S    
        return -1;                                 // failure r/c
    }                                             
    else                                           // symbol table found with typdef
    {
        return get_typdef(*p, typdef_name, typdef);    
    }
}
M_endf  


int get_typdef(frame_S& frame, const std::wstring& typdef_name, symval_S& symval) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, typdef_name, p);

    if (rc != 0)                                   // identifier not found in any symbol table
    {                                             
        symval = symval_S { };                     // pass back empty symval_S   
        return -1;                                 // failure r/c
    }                                             
    else                                           // symbol table found with typdef
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



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   def_local_var()          -- sets value of passed-in variable -- defines new variable/const in local symbol table (which might be the global symbol table if this is main frame_S)
////   def_global_var()         -- sets value of passed-in variable -- defines new variable/const in global symbol table (which might be the current local symbol table if this is main frame_S)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

static int def_variable(symtab_S& symtab, const std::wstring& ident, const value_S& value, bool is_const, bool is_exported, bool unshare) try
{  
    // construct symval_S for new local variable identifier

    symval_S                  symval    {             };   // local symtab_S to fill in 
    std::shared_ptr<value_S>  value_sp  { new value_S };   // temporary shared pointer -- will release ownership when this function returns 

    symval.value_sp      = value_sp;
    *(symval.value_sp)   = value;  
    symval.is_const      = is_const;
    symval.is_exported   = is_exported;
                                        
    return def_identifier(symtab, ident, symval, unshare); 
}
M_endf                          


///////////////////////////////////////////////////////////////////////////////////////////////

int def_local_var(frame_S& frame, const std::wstring& ident, const value_S& value, bool is_const, bool is_exported, bool unshare) try
{  
    return def_variable(frame.symbols_p->symtab, ident, value, is_const, is_exported, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int def_global_var(const std::wstring& ident, const value_S& value, bool is_const, bool unshare) try
{  
    return def_variable(*get_global_symtab(), ident, value, is_const, true, unshare); 
}
M_endf




///_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   undef_local_var()  -- removes passed-in variable (if it is present in local symbol table)   -- no complaint if not there
////   undef_global_var() -- removes passed-in variable (if it is present in global symbol table)   -- no complaint if not there
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int undef_variable(symtab_S& symtab, const std::wstring& ident) try
{
    // see if identifier already exists, and is constant

    auto ct = symtab.symbols.count(ident);

    if (is_identifier_defined(symtab, ident))
    {
       if (is_identifier_const(symtab,ident))
       {
           count_error(); 
           M_out_emsg(L"undef_var() -- cannot remove constant \"%s\"") % ident; 
           return -1;
       }

       if (is_identifier_verb(symtab, ident)) 
       {
           count_error(); 
           M_out_emsg(L"undef_var() -- cannot remove verb \"%s\"") % ident; 
           return -1;
       }

       undef_identifier(symtab, ident);                   // get rid of old value 
    }

    return 0;
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_local_var(frame_S& frame, const std::wstring& ident) try
{
    return undef_variable(frame.symbols_p->symtab, ident);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int undef_global_var(const std::wstring& ident) try
{
    return undef_variable(*get_global_symtab(), ident);
}
M_endf



////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   get_var()        -- gets value of passed-in variable (-1 if not in local or global symbol table, or is not visible in other symbol tables) 
////   get_local_var()  -- gets value of passed-in variable (-1 if not in local symbol table)
////   get_global var() -- gets value of passed-in variable (-1 if not in global symbol table)
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

// ----------------------------
// version that returns value_S
// ----------------------------
                               
static int get_variable(symtab_S& symtab, const std::wstring& ident, value_S& value) try
{
    if (!is_identifier_defined(symtab, ident))              // return with R/C = -1 and empty value, if identifier is not there 
    {
        value = value_S {}; 
        return -1;
    }
    else                                                    // identifier is found -- might be verb or variable/constant
    {
        if (
             is_identifier_verb(symtab, ident)              // don't pass back verbs found in symbol table
             ||
             is_identifier_typdef(symtab, ident)            // don't pass back typedefs found in symbol table
            )
        {
           value = value_S {}; 
           return -1;           
        }               


        // identifier is variable/constant -- return it

        value = *(symtab.symbols.at(ident).value_sp);       // value found -- pass it back
        return 0; 
    }
}
M_endf


// --------------------------------------------
// version that returns complete symval_S entry
// --------------------------------------------
                               
static int get_variable(symtab_S& symtab, const std::wstring& ident, symval_S& symval_out) try
{
    if (!is_identifier_defined(symtab, ident))              // return with R/C = -1 and empty symval, if identifier is not there 
    {
        symval_out = symval_S {}; 
        return -1;
    }
    else                                                    // identifier is found -- might be verb or variable/constant or typedef, etc.
    {
        if (
             is_identifier_verb(symtab, ident)              // don't pass back verbs found in symbol table
             ||
             is_identifier_typdef(symtab, ident)            // don't pass back typedefs found in symbol table
            )
        {
           symval_out = symval_S {}; 
           return -1;           
        }               


        // identifier is variable/constant -- return it

        symval_out = symtab.symbols.at(ident);              // symval_S found -- pass it back
        return 0; 
    }
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int get_local_var(frame_S& frame, const std::wstring& ident, value_S& value) try
{
    return get_variable(frame.symbols_p->symtab, ident, value);
}
M_endf


int get_local_var(frame_S& frame, const std::wstring& ident, symval_S symval) try
{
    return get_variable(frame.symbols_p->symtab, ident, symval);
}
M_endf   

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int get_global_var(const std::wstring& ident, value_S& value) try
{
    return get_variable(*get_global_symtab(), ident, value);
}
M_endf


int get_global_var(const std::wstring& ident, symval_S symval) try
{
    return get_variable(*get_global_symtab(), ident, symval);
}
M_endf

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

int get_var(frame_S& frame, const std::wstring& ident, value_S value) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                     // variable not found in any symbol table
    {
        value = value_S { };         // pass back empty value
        return -1;                   // failure r/c
    }
    else                             // symbol table found with identifier
    {
        return get_variable(*p, ident, value);    
    }
}
M_endf


int get_var(frame_S& frame, const std::wstring& ident, symval_S& symval) try
{
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);

    if (rc != 0)                     // variable not found in any symbol table
    {
        symval = symval_S { };       // pass back empty symval_S
        return -1;                   // failure r/c
    }
    else                             // symbol table found with identifier
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
////   update_local_var()  -- sets value of passed-in variable -- overwrites whatever is there in local symbol table (which might be the global symbol table if this is main frame_S)
////   update_global_var() -- sets value of passed-in variable -- overwrites whatever is there in global symbol table (which might be the current local symbol table if this is main frame_S)
////   update_var()        -- sets value of passed-in variable -- overwrites whatever is there in local or global symbol table
////                          (priority:  1 -- overwrites existing local variable, 2 -- overwrites existing global variable) 
////                   
////
////_________________________________________________________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int update_variable(symtab_S& symtab, const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{
    // error if identifier is not already in the symbol table

    auto ct = symtab.symbols.count(ident);

    if (!is_identifier_defined(symtab, ident))
    {
        count_error(); 
        M_out_emsg(L"update_var() -- identifier \"%s\" to be updated is not defined") % ident; 
        return -1;  
    }


    // identifier exists in symbol table -- make sure it's not defined as const or verb or typdef

    symval_S symval { };                                      // local copy of symval from symtab for this identifier
    get_identifier(symtab, ident, symval);                    // should not fail, since identifier is known to be defined in target symtab


    if (symval.is_const)
    {
        count_error(); 
        M_out_emsg(L"update_var() -- cannot alter/redefine constant \"%s\" with new value") % ident; 
        return -1;
    }
   
    if (symval.is_verbdef)
    {
        count_error(); 
        M_out_emsg(L"update_var() -- cannot alter/redefine verb definition \"%s\" with new value") % ident; 
        return -1;
    }   

    if (symval.is_typdef)
    {
        count_error(); 
        M_out_emsg(L"update_var() -- cannot alter/redefine type definition (typedef) \"%s\" with new value") % ident; 
        return -1;
    }   


    // update associated value for this symtab entry -- in place update -- don't change location  (value_sp should decrement use count and run destructors, etc.)    

    *(symval.value_sp) = value;                                


    // make sure any nested objects are unshared, if requested by caller
  
    if (unshare)                             
        unshare_value(*(symval.value_sp)); 


    // make this symtab entry in symbol table constant, if requested

    if (make_const)
        make_identifier_const(symtab, ident);  

    return 0; 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int update_local_var(frame_S& frame, const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{  
    return update_variable(frame.symbols_p->symtab, ident, value, make_const, unshare); 
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////

int update_global_var(const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{  
    return update_variable(*get_global_symtab(), ident, value, make_const, unshare); 
}
M_endf
 

///////////////////////////////////////////////////////////////////////////////////////////////     

int update_var(frame_S& frame, const std::wstring& ident, const value_S& value, bool make_const, bool unshare) try
{  
    symtab_S *p {nullptr};
    auto rc = find_symtab_ident(frame, ident, p);
                                                                     
    if (rc != 0)                                                        // identifier not found in any symbol table
        return -1;                                                      // failure r/c
    else                                                                // symbol table found with variable
        return update_variable(*p, ident, value, make_const, unshare);  // proper symbol table found -- update the variable in that symbol table
}
M_endf





//_________________________________________________________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""