// ex_verb_aggr.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     ================
////     ex_verb_aggr.cpp -- aggregate, container, and type-oriented verbs                          
////     ================
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////     @AGG
////     @TYPE
////     @TO_xxxx
////     @IS_xxxx
////
////     (@SELECT)       "."
////     (@SUBSCRIPT)    "#"
////
////     @ARG_CT
////     @ARGS
////     @ARG
////     @AT 
////     @VL_CT
////
////
////    
////
////
////
////
////
////    
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  


//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      Aggregate-oriented and Type-oriented verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


//---------------------------------------------------------------------------------
//   @AGG 
//---------------------------------------------------------------------------------

int verb_agg(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // see which parms are present -- at least one must be present: len: or type: (or both)
    // -----------------------------------------------------------

    value_S len_value  { };
    value_S type_value { };

    auto len_rc   = get_right_keyword(expression, L"len"  , len_value  );       // len_rc   = -1, if len:   is not present (meaning use length of type: for length of buffer 
    auto type_rc  = get_right_keyword(expression, L"type" , type_value );       // type_rc  = -1, if type:  is not present (meaning, make array of UINT8_T with len: elements


    // error, if neither len: nor type: keywords are present
    // -----------------------------------------------------

    if ( (len_rc == -1) && (type_rc == -1) )
    {
       count_error();
       M_out_emsg1(L"verb_agg() -- neither len: or type: keywords were present -- at least one is required -- aggregate cannot be constructed");
       msgend_loc(expression);

       results = results_S { };
       return -1; 
    }


    // error, if both len: and type: keywords are present, and length of typdef_S exceeds specified len:
    // -------------------------------------------------------------------------------------------------

    if (
        ( (len_rc == 0) && (type_rc == 0) )
        &&
        ( type_value.typdef_sp->tsize > len_value.int64)
       ) 
    {
       count_error();
       M_out_emsg1(L"verb_agg() -- length of type: (%d) exceeds specified len: (%d) -- aggregate cannot be constructed") % type_value.typdef_sp->tsize % len_value.int64;
       msgend_loc(expression);

       results = results_S { };
       return -1; 
    }


    // error, if typdef_S for type: keyword has zero length (i.e. is not fixed-length typdef_S)
    // ----------------------------------------------------------------------------------------

    if ( (type_rc == 0) && (type_value.typdef_sp->tsize <= 0) )
    {
       count_error();
       M_out_emsg1(L"verb_agg() -- length of type: is 0 (non fixed-length type) -- aggregate cannot be constructed");
       msgend_loc(expression);

       results = results_S { };
       return -1; 
    }  


    // get length of buf8_T
  
    size_t len { };

    if (len_rc == 0)
       len = len_value.int64; 
    else
       len = type_value.typdef_sp->tsize;


    // get typdef for this aggregate

    typdef_S typdef { };

    if (type_rc == 0)
    {
        typdef = *(type_value.typdef_sp);                                   // use caller's typdef, if provided
        unshare_typdef(typdef);                                             // make sure nothing shared,since std::move() will be used below
    }
    else    
    {
        make_array_typdef(len, make_atomic_typdef(type_E::uint8), typdef);  // use array of uint8, with len: elements
    }


    // construct aggregate results from typdef_S and buffer, and return normally

    buf8_T buffer { len }; 
    buffer.clear(); 

    results = to_results(  buffer_val(buffer, typdef, true)  ); 

    return 0; 
}
M_endf
         

//---------------------------------------------------------------------------------
//   @TYPE 
//---------------------------------------------------------------------------------

int verb_type(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    int rc {0}; 


     // process input parms
     // -------------------

     // known: optional typedef name on right side -- is unevaluated identifier
    
     value_S name_value  { };                                                      // name_value.string has name of undefined identifier for this typdef

     auto name_rc      = get_right_keyword(expression, L"name",  name_value );       // name_rc      = -1 if name:       is not present (meaning anonymous (returned) type 
     auto global_rc    = get_right_keyword(expression, L"global"            );       // global_rc    = -1, if global:    is not present 
     auto verbmain_rc  = get_right_keyword(expression, L"verbmain"          );       // verbmain rc  = -1, if verbmain:  is not present
     auto static_rc    = get_right_keyword(expression, L"static"            );       // static rc    = -1, if static:    is not present
#ifdef M_EXPOSE_SUPPORT
     auto expose_rc    = get_right_keyword(expression, L"expose"            );       // expose_rc  = -1, if expose:  is not present (meaning no expose)
#endif


     // known: no more than one of the following keywords is present:

     value_S array_value     { };            // value should be a vlist with two positional parms
     value_S struct_value    { };            // value should be a vlist with 1-N nested vlist values 

     auto     unit_rc = get_right_keyword(expression, L"unit"                      );  
     auto     bool_rc = get_right_keyword(expression, L"bool"                      );  
     auto     int8_rc = get_right_keyword(expression, L"int8"                      );    
     auto    uint8_rc = get_right_keyword(expression, L"uint8"                     );  
     auto    int16_rc = get_right_keyword(expression, L"int16"                     );    
     auto   uint16_rc = get_right_keyword(expression, L"uint16"                    ); 
     auto    int32_rc = get_right_keyword(expression, L"int32"                     );    
     auto   uint32_rc = get_right_keyword(expression, L"uint32"                    ); 
     auto    int64_rc = get_right_keyword(expression, L"int64"                     );    
     auto   uint64_rc = get_right_keyword(expression, L"uint64"                    ); 
     auto  float32_rc = get_right_keyword(expression, L"float32"                   );    
     auto  float64_rc = get_right_keyword(expression, L"float64"                   ); 
     auto    array_rc = get_right_keyword(expression, L"array"    ,     array_value);
     auto   struct_rc = get_right_keyword(expression, L"struct"   ,    struct_value);


     // set up typdef_S based on input parms
     // ------------------------------------

     typdef_S typdef { };              // typdef to be filled in 


     // (note: only one of these should have r/c = 0, if none are present, default typdef will be produced)

     if (    unit_rc == 0 )   make_atomic_typdef(type_E::unit    , typdef);
     if (    bool_rc == 0 )   make_atomic_typdef(type_E::boolean , typdef);
     if (    int8_rc == 0 )   make_atomic_typdef(type_E::int8    , typdef);
     if (   uint8_rc == 0 )   make_atomic_typdef(type_E::uint8   , typdef);
     if (   int16_rc == 0 )   make_atomic_typdef(type_E::int16   , typdef);
     if (  uint16_rc == 0 )   make_atomic_typdef(type_E::uint16  , typdef);
     if (   int32_rc == 0 )   make_atomic_typdef(type_E::int32   , typdef);
     if (  uint32_rc == 0 )   make_atomic_typdef(type_E::uint32  , typdef);
     if (   int64_rc == 0 )   make_atomic_typdef(type_E::int64   , typdef);
     if (  uint64_rc == 0 )   make_atomic_typdef(type_E::uint64  , typdef);
     if ( float32_rc == 0 )   make_atomic_typdef(type_E::float32 , typdef);
     if ( float64_rc == 0 )   make_atomic_typdef(type_E::float64 , typdef);



     // process array type
     // ------------------

     if (array_rc == 0)
     {
         uint64_t acount {0};  


         // array_value should be a vlist with 2 positional values -- int64 number of elements, and typdef_S for each element

         if (
             (array_value.ty != type_E::vlist)
             ||
             (array_value.vlist_sp->value_ct != 2)
            )
         {
              count_error();
              M_out_emsg1(L"@TYPE -- unexpected error -- invalid array: kw parm value");
              msgend_loc(array_value, expression);
              rc = -1;          
         }
         else if ( array_value.vlist_sp->values.at(0).ty != type_E::int64 )
         {
              count_error();
              M_out_emsg1(L"@TYPE -- unexpected error -- 1st positional value in array:[vlist] is not valid int64");
              msgend_loc(array_value.vlist_sp->values.at(0), expression);
              rc = -1;           
         }   
         else
         {
             acount = (uint64_t)(array_value.vlist_sp->values.at(0).int64);              // capture number of elements
         
             if ( array_value.vlist_sp->values.at(1).ty != type_E::typdef)
             {
                 count_error();
                 M_out_emsg1(L"@TYPE -- unexpected error -- 2nd positional value in array:[vlist] is not typdef");
                 msgend_loc(array_value.vlist_sp->values.at(1), expression);
                 rc = -1;          
             }
             else
             {   
                 M__(M_out(L"verb_type() -- array element tsize = %d") % array_value.vlist_sp->values.at(1).typdef_sp->tsize;)

                 if (array_value.vlist_sp->values.at(1).typdef_sp->tsize == 0)    // is typdef for array element a non-sized (invalid) typdef?
                 {
                     count_error();
                     M_out_emsg1(L"@TYPE -- unexpected error -- array element typdef is non-sized (not atomic or aggregate)");
                     msgend_loc(array_value.vlist_sp->values.at(1), expression);
                     rc = -1;             
                 }


                 // set up array typdef_S -- count and element type are OK for arrays

                 auto ma_rc = make_array_typdef(acount, *(array_value.vlist_sp->values.at(1).typdef_sp), typdef); //  builds array typdef_S into typdef output parm -- all values are unshared in typdef_S

                 if (ma_rc != 0)                                                                                  //  unexpected make_array_typdef() failure ???
                 {
                     count_error();
                     M_out_emsg1(L"@TYPE -- unexpected error -- make_array_typdef() failed");
                     msgend_loc(array_value.vlist_sp->values.at(1), expression);
                     rc = -1;             
                 }              
             }       // array element type is typdef 
         }           // number of array elements is OK   
     }              // array: present


     // process structure type      
     // ----------------------
   
     if (struct_rc == 0)
     {
         // build vector with decoded field parms for passing to make_structure_typdef()
         // ----------------------------------------------------------------------------

         std::vector<fieldparm_S> fieldparms { };


         // loop through nested vlist of field definitions (this is the vlist for the struct: keyword)  

         for (auto& elem : struct_value.vlist_sp->values)   // elem should be n-th positional value_S->vlist  
         {
              M__(display_vlist(*(elem.vlist_sp), L"*(elem.vlist_sp)");)


              // collect parm info about n-th field 

              fieldparm_S fieldparm { };

              fieldparm.fieldname = M_get_nest_pos_string(  *(elem.vlist_sp), 0);                  // 1st positional parm (0) is fieldname
              fieldparm.typdef_p  = M_get_nest_pos_typdef_p(*(elem.vlist_sp), 1);                  // 2nd positional parm (0) is typdef_S for this field

              value_S offset_value    { };
              value_S skip_value      { };
            
              auto offset_rc = get_vlist_keyword(*(elem.vlist_sp), L"offset", offset_value    );   // get offset:nnnn   (if present)
              auto skip_rc   = get_vlist_keyword(*(elem.vlist_sp), L"skip"  , skip_value      );   // get skip:nnnn     (if present)  
              auto same_rc   = get_vlist_keyword(*(elem.vlist_sp), L"same"                    );   // get same:         (if present)
              auto high_rc   = get_vlist_keyword(*(elem.vlist_sp), L"high"                    );   // get high:         (if present)

              if (offset_rc == 0)
              {
                  fieldparm.offset     = offset_value.int64; 
                  fieldparm.has_offset = true;
              }

              if (skip_rc == 0)
              {
                  fieldparm.skip       = skip_value.int64;
                  fieldparm.has_skip   = true; 
              }
            
              if (same_rc == 0)
                  fieldparm.same_as_prior = true; 
              
              if (high_rc == 0)
                  fieldparm.at_hi_watermark = true; 


              // add collected field parms for n-th field into field parms vector

              M__(M_out(L"verb_type() -- fieldname = %S  type=%S   offset=%d    skip=%d") % fieldparm.fieldname % type_str(fieldparm.typdef_p->kind) % fieldparm.offset % fieldparm.skip;)    
         
              fieldparms.push_back(fieldparm);    
         } 


         // set up structure typdef_S -- based on collected fieldparm info 

         auto mr_rc = make_structure_typdef(fieldparms, typdef);
         M__(M_out(L"verb_type() -- make_structure_typdef() returned");)

         if (mr_rc != 0)                                                                                  //  make_structure_typdef() failure ?
         {
             count_error();
             M_out_emsg1(L"@TYPE -- make_structure_typdef() failed -- see earlier error messages");
             msgend_loc(expression);
             rc = -1;             
         } 
     }


     // ------------------------------------------------------------------------------------------
     // if name was provided, add this type-name to  global verbmain, static, or local stack frame 
     // ------------------------------------------------------------------------------------------

     if (name_rc == 0)
     {
        if (global_rc == 0)                     // global: keyword was present
        {
            if (is_global_identifier_defined(name_value.string))
            {
                count_error();
                M_out_emsg1(L"@TYPE -- global identifier %s is already defined -- unable to define it again") % name_value.string;
                msgend_loc(name_value, expression);
                rc = -1;        
            }
            else
            {
                auto rc = def_global_typdef(name_value.string, typdef);               // add new type definition to environment -- global symbols are always exposed  -- don't bother unsharing -- types are immutable 
                M__(M_out(L"@TYPE -- def_global_typdef() returned");)

                // errors are unexpected here
               
                if (rc != 0)
                {
                    //count_error(); already counted in def_global_typdef()
                    M_out_emsg1(L"@TYPE -- unexpected error from def_global_typdef() -- unable to define new global identifier = %s") % name_value.string;
                    msgend_loc(name_value, expression);
                    rc = -1; 
                }  
            }
        }
        else if (static_rc == 0)          // static: keyword was present
        {
            if (is_static_identifier_defined(frame, name_value.string))
            {
                count_error();
                M_out_emsg1(L"@TYPE -- static identifier %s is already defined -- unable to define it again") % name_value.string;
                msgend_loc(name_value, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
                parm.exposed = (expose_rc == 0); 
#endif

                auto rc = def_static_typdef(frame, name_value.string, typdef, parm);          // add new non-constant variable to environment  -- expose based on expose: kw rc -- don't bother unsharing -- types are immutable
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_static_typdef()
                    M_out_emsg1(L"@TYPE -- unexpected error from def_static_typdef() -- unable to define new static identifier = %s") % name_value.string;
                    msgend_loc(name_value, expression);
                    rc = -1;  
                }  
            }
        }  
        else if (verbmain_rc == 0)          // verbmain: keyword was present
        {
            if (is_verbmain_identifier_defined(frame, name_value.string))
            {
                count_error();
                M_out_emsg1(L"@TYPE -- verbmain identifier %s is already defined -- unable to define it again") % name_value.string;
                msgend_loc(name_value, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
                parm.exposed = (expose_rc == 0); 
#endif

                auto rc = def_verbmain_typdef(frame, name_value.string, typdef, parm);          // add new non-constant variable to environment  -- expose based on expose: kw rc -- don't bother unsharing -- types are immutable
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_verbmain_typdef()
                    M_out_emsg1(L"@TYPE -- unexpected error from def_verbmain_typdef() -- unable to define new verbmain identifier = %s") % name_value.string;
                    msgend_loc(name_value, expression);
                    rc = -1;  
                }  
            }
        }     
        else          // neither global:, verbmain:, nor static: were not present -- define local typdef
        {
            if (is_local_identifier_defined(frame, name_value.string))
            {
                count_error();
                M_out_emsg1(L"@TYPE -- local identifier %s is already defined -- unable to define it again") % name_value.string;
                msgend_loc(name_value, expression);
                rc = -1;        
            }
            else
            {
                def_parm_S parm { }; 
#ifdef M_EXPOSE_SUPPORT
                parm.exposed = (expose_rc == 0);
#endif

                auto rc = def_local_typdef(frame, name_value.string, typdef, parm);          // add new non-constant variable to environment  -- expose based on expose: kw rc -- don't bother unsharing -- types are immutable
              
                // errors expected here include duplicate variable names in list -- 2nd one cannot be re-defined -- other errors caught by usual verb parm checking
              
                if (rc != 0)
                {
                    //count_error(); already counted in def_local_typdef()
                    M_out_emsg1(L"@TYPE -- unexpected error from def_local_typdef() -- unable to define new local identifier = %s") % name_value.string;
                    msgend_loc(name_value, expression);
                    rc = -1;  
                }  
            }
        }          // local
     }             // name: provided


     // ---------------------------------------------
     // return with results = new typdef_S (or error)
     // ---------------------------------------------

     if (rc == 0)
     {
         value_S value { typdef_val(typdef) };           // put typdef_S into a value_S
      // unshare_value(value);                           // make sure all nested items are unshared -- shouldn't be required 
         results = to_results(value);                    // pass back new typdef_S definition
     }
     else
     {
         results = error_results();
     }

     M__(M_out(L"verb_type() -- returning");)
     return rc;  
}        
M_endf


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @TO_XXXXX type conversion verbs
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>      bool verb__to_bool(   T x) {return (     bool)x;}
template<typename T>    int8_t verb__to_int8(   T x) {return (   int8_t)x;}
template<typename T>   int16_t verb__to_int16(  T x) {return (  int16_t)x;}
template<typename T>   int32_t verb__to_int32(  T x) {return (  int32_t)x;}
template<typename T>   int64_t verb__to_int64(  T x) {return (  int64_t)x;}
template<typename T>   uint8_t verb__to_uint8(  T x) {return (  uint8_t)x;}
template<typename T>  uint16_t verb__to_uint16( T x) {return ( uint16_t)x;}
template<typename T>  uint32_t verb__to_uint32( T x) {return ( uint32_t)x;}
template<typename T>  uint64_t verb__to_uint64( T x) {return ( uint64_t)x;}
template<typename T> float32_T verb__to_float32(T x) {return (float32_T)x;}
template<typename T> float64_T verb__to_float64(T x) {return (float64_T)x;}    


//---------------------------------------------------------------------------------
//   @TO_BOOL xxx
//---------------------------------------------------------------------------------

int verb_to_bool(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_bool() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert input value to boolean true/false value

    if (is_value_true(in_val))
        out_val = boolean_val(true , in_val.token_ix1, in_val.token_ix1);
    else
        out_val = boolean_val(false, in_val.token_ix1, in_val.token_ix1);
        

    results = to_results(out_val);
    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @TO_INT8 xxx
//---------------------------------------------------------------------------------

int verb_to_int8(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_int8() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = int8_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting  -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_int8, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to int8_t -- complain about any conversion errors

        int8_t  int8 { }; 
        auto trc = to_int8(in_val.string, int8);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_INT8 -- error occurred when converting from string = «%s» to int8 value") % in_val.string; 
            msgend_loc(in_val, expression);
        
            results = error_results(); 
            return -1; 
        }   

        out_val = int8_val(int8, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_INT16 xxx
//---------------------------------------------------------------------------------

int verb_to_int16(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_int16() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = int16_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting  -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_int16, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to int16_t -- complain about any conversion errors

        int16_t  int16 { }; 
        auto trc = to_int16(in_val.string, int16);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_INT16 -- error occurred when converting from string = «%s» to int16 value") % in_val.string; 
            msgend_loc(in_val, expression);   
        
            results = error_results(); 
            return -1; 
        }   

        out_val = int16_val(int16, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_INT32 xxx
//---------------------------------------------------------------------------------

int verb_to_int32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_int32() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = int32_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting     -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_int32, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to int32_t -- complain about any conversion errors

        int32_t  int32 { }; 
        auto trc = to_int32(in_val.string, int32);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_INT32 -- error occurred when converting from string = «%s» to int32 value") % in_val.string; 
            msgend_loc(in_val, expression);
            results = error_results(); 
            return -1; 
        }   

        out_val = int32_val(int32, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_INT64 xxx
//---------------------------------------------------------------------------------

int verb_to_int64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_int64() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = int64_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting    -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_int64, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to int64_t -- complain about any conversion errors

        int64_t  int64 { }; 
        auto trc = to_int64(in_val.string, int64);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_INT64 -- error occurred when converting from string = «%s» to int64 value") % in_val.string; 
            msgend_loc(in_val, expression);   
        
            results = error_results(); 
            return -1; 
        }   

        out_val = int64_val(int64, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_UINT8 xxx
//---------------------------------------------------------------------------------

int verb_to_uint8(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_uint8() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = uint8_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting    -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_uint8, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to uint8_t -- complain about any conversion errors

        uint8_t  uint8 { }; 
        auto trc = to_uint8(in_val.string, uint8);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_UINT8 -- error occurred when converting from string = «%s» to uint8 value") % in_val.string; 
            msgend_loc(in_val, expression);   
        
            results = error_results(); 
            return -1; 
        }   

        out_val = uint8_val(uint8, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_UINT16 xxx
//---------------------------------------------------------------------------------

int verb_to_uint16(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_uint16() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = uint16_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting   -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_uint16, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to uint16_t -- complain about any conversion errors

        uint16_t  uint16 { }; 
        auto trc = to_uint16(in_val.string, uint16);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_UINT16 -- error occurred when converting from string = «%s» to uint16 value") % in_val.string; 
            msgend_loc(in_val, expression);   
        
            results = error_results(); 
            return -1; 
        }   

        out_val = uint16_val(uint16, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf




//---------------------------------------------------------------------------------
//   @TO_UINT32 xxx
//---------------------------------------------------------------------------------

int verb_to_uint32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_uint32() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = uint32_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting   -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_uint32, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to uint32_t -- complain about any conversion errors

        uint32_t  uint32 { }; 
        auto trc = to_uint32(in_val.string, uint32);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_UINT32 -- error occurred when converting from string = «%s» to uint32 value") % in_val.string; 
            msgend_loc(in_val, expression);  
        
            results = error_results(); 
            return -1; 
        }   

        out_val = uint32_val(uint32, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_UINT64 xxx
//---------------------------------------------------------------------------------

int verb_to_uint64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_uint64() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = uint64_val(0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting    -- ???????????????????????? need to use get_val() functions instead of casting for integer conversions ?????????????????????????? 

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_uint64, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to uint64_t -- complain about any conversion errors

        uint64_t  uint64 { }; 
        auto trc = to_uint64(in_val.string, uint64);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_UINT64 -- error occurred when converting from string = «%s» to uint64 value") % in_val.string; 
            msgend_loc(in_val, expression); 
        
            results = error_results(); 
            return -1; 
        }   

        out_val = uint64_val(uint64, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @TO_FLOAT32 xxx
//---------------------------------------------------------------------------------

int verb_to_float32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_float32() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = float32_val(0.0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_float32, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to float32_t -- complain about any conversion errors

        float32_T  float32 { }; 
        auto trc = to_float32(in_val.string, float32);
        if (trc != 0)
        {            
            M_out_emsg1(L"@TO_FLOAT32 -- error occurred when converting from string = «%s» to float32 value") % in_val.string; 
            msgend_loc(in_val, expression);  
        
            results = error_results(); 
            return -1; 
        }   

        out_val = float32_val(float32, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf
 

//---------------------------------------------------------------------------------
//   @TO_FLOAT64 xxx
//---------------------------------------------------------------------------------

int verb_to_float64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, or string 
    M__(M_out(L"verb_to_float64() called");)
          
    value_S in_val  {expression.rparms.values.at(0)};
    value_S out_val {                              };


    // convert unit value to 0

    if (in_val.ty == type_E::unit)
    {
        out_val = float64_val(0.0, in_val.token_ix1, in_val.token_ix1);
    }


    // convert arithmetic values with C/C++ casting

    else if (is_value_arithmetic(in_val))
    {
         M_verb_unary_fvalix_arith(out_val, verb__to_float64, in_val, in_val.token_ix1, in_val.token_ix2)
    } 


    // special conversion to numeric for strings

    else if (in_val.ty == type_E::string)
    {
        // convert from string to float64_T -- complain about any conversion errors

        float64_T  float64 { }; 
        auto trc = to_float64(in_val.string, float64);
        if (trc != 0)
        {            
            M_out_emsg1(L"TO_FLOAT64 -- error occurred when converting from string = «%s» to float64 value") % in_val.string; 
            msgend_loc(in_val, expression);  
        
            results = error_results(); 
            return -1; 
        }   

        out_val = float64_val(float64, in_val.token_ix1, in_val.token_ix1);
    }  

    results = to_results(out_val);
    return 0; 
}
M_endf
 

//---------------------------------------------------------------------------------
//   @TO_STR xxx
//---------------------------------------------------------------------------------

int verb_to_str(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be unit, int, float, string, or vlist  
    M__(M_out(L"verb_to_str() called");)
     
    results = to_results(string_val( str_value(expression.rparms.values.at(0), false, false, true) ));   
    return 0; 
}
M_endf  
 

//---------------------------------------------------------------------------------
//   @TO_IDENT "string"
//---------------------------------------------------------------------------------

int verb_to_ident(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be string 
    M__(M_out(L"verb_to_ident() called");)
    

    // make sure string contains valid identifier name -- complain and return error, if not

    std::wstring  id_str {expression.rparms.values.at(0).string };              // 1st right positional parm should be a string 


    if (!is_valid_identifier(id_str))
    {
        M_out_emsg1(L"@TO_IDENT -- string = \"%S\" cannot be converted to an identifier") % id_str; 
        msgend_loc(expression.rparms.values.at(0), expression); 
  
        results = error_results();    // return error results
        return -1;                    // failure r/c
    }


    //  identifier name is OK -- pass back identifier results 

    results = to_results(identifier_val(id_str));   
    return 0; 
}
M_endf  
 


//---------------------------------------------------------------------------------
//   @TO_VERBNAME "string"
//---------------------------------------------------------------------------------

int verb_to_verbname(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm -- type should be string 
    M__(M_out(L"verb_to_verbname() called");)
    

    // make sure string contains valid verbname -- complain and return error, if not

    std::wstring  id_str {expression.rparms.values.at(0).string };              // 1st right positional parm should be a string 


    if (!is_valid_verbname(id_str))
    {
        M_out_emsg1(L"@TO_VERBNAME -- string = \"%S\" cannot be converted to a verbname") % id_str; 
        msgend_loc(expression.rparms.values.at(0), expression); 
  
        results = error_results();    // return error results
        return -1;                    // failure r/c
    }


    //  verbname name is OK -- pass back verbname results 

    results = to_results(verbname_val(id_str));   
    return 0; 
}
M_endf  





//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @IS_XXXXX type testing verbs
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------------
//   @IS_BOOL xxx
//---------------------------------------------------------------------------------

int verb_is_bool(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_bool() called");)
          
    if ( is_value_boolean(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf




//---------------------------------------------------------------------------------
//   @IS_INT8 xxx
//---------------------------------------------------------------------------------

int verb_is_int8(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int8() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::int8)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @IS_INT16 xxx
//---------------------------------------------------------------------------------

int verb_is_int16(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int16() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::int16)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_INT32 xxx
//---------------------------------------------------------------------------------

int verb_is_int32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int32() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::int32)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @IS_INT64 xxx
//---------------------------------------------------------------------------------

int verb_is_int64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int64() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::int64)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_UINT8 xxx
//---------------------------------------------------------------------------------

int verb_is_uint8(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_uint8() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::uint8)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @IS_UINT16 xxx
//---------------------------------------------------------------------------------

int verb_is_uint16(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_uint16() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::uint16)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_UINT32 xxx
//---------------------------------------------------------------------------------

int verb_is_uint32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_uint32() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::uint32)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf



//---------------------------------------------------------------------------------
//   @IS_UINT64 xxx
//---------------------------------------------------------------------------------

int verb_is_uint64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_uint64() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::uint64)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_FLOAT32 xxxx
//---------------------------------------------------------------------------------

int verb_is_float32(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_float32() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::float32)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf




//---------------------------------------------------------------------------------
//   @IS_FLOAT64 xxxx
//---------------------------------------------------------------------------------

int verb_is_float64(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_float64() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::float64)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_INT xxxx
//---------------------------------------------------------------------------------

int verb_is_int(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_int() called");)
          
    if ( is_value_integer(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_FLOAT xxxx
//---------------------------------------------------------------------------------

int verb_is_float(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_float() called");)
          
    if ( is_value_float(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_NUMERIC xxxx
//---------------------------------------------------------------------------------

int verb_is_numeric(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_numeric() called");)
          
    if ( is_value_arithmetic(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_ATOM xxxx
//---------------------------------------------------------------------------------

int verb_is_atom(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_atom() called");)
          
    if ( is_value_comparable(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_SIGNED xxxx
//---------------------------------------------------------------------------------

int verb_is_signed(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_signed() called");)
          
    if ( is_value_signed(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_UNSIGNED xxxx
//---------------------------------------------------------------------------------

int verb_is_unsigned(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_unsigned() called");)
          
    if ( is_value_unsigned(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_TRUE xxxx
//---------------------------------------------------------------------------------

int verb_is_true(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_true() called");)
          
    if ( is_value_true(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_FALSE xxxx
//---------------------------------------------------------------------------------

int verb_is_false(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_false() called");)
          
    if ( is_value_false(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_STRING xxxx
//---------------------------------------------------------------------------------

int verb_is_string(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_string() called");)
          
    if ( is_value_string(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_UNIT xxxx
//---------------------------------------------------------------------------------

int verb_is_unit(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_unit() called");)
          
    if ( is_value_unit(expression.rparms.values.at(0)) )
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf
 

//---------------------------------------------------------------------------------
//   @IS_IDENTIFIER xxxx
//---------------------------------------------------------------------------------

int verb_is_identifier(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_identifier() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::identifier)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf
      

//---------------------------------------------------------------------------------
//   @IS_VERBNAME xxxx
//---------------------------------------------------------------------------------

int verb_is_verbname(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_verbname() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::verbname)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf
      

//---------------------------------------------------------------------------------
//   @IS_EXPRESSION xxxx
//---------------------------------------------------------------------------------

int verb_is_expression(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_expression() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::expression)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_VLIST xxxx
//---------------------------------------------------------------------------------

int verb_is_vlist(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_vlist() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::vlist)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf
 

//---------------------------------------------------------------------------------
//   @IS_BLOCK xxxx
//---------------------------------------------------------------------------------

int verb_is_block(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional parm of any type
    M__(M_out(L"verb_is_block() called");)
          
    if (expression.rparms.values.at(0).ty == type_E::block)
        results = true_results();
    else
        results = false_results();

    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_DEF xxxx
//---------------------------------------------------------------------------------

int verb_is_def(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side unevaluated identifier positional, and an optional keyword = all: global: local: (all: is default)
    M__(M_out(L"verb_is_def() called");)
    
    bool tf {false};                                                                          // output value                                 


    // see if global:, static:, verbmain:, or local: are defined -- only one of these (or none) should be defined
                                                                  
    auto global_rc   = get_right_keyword(expression, L"global");                              // get value of global:   keyword -- if not present, R/C is -1 
    auto static_rc   = get_right_keyword(expression, L"static");                              // get value of static:   keyword -- if not present, R/C is -1 
    auto verbmain_rc = get_right_keyword(expression, L"verbmain");                            // get value of verbmain: keyword -- if not present, R/C is -1 
    auto local_rc    = get_right_keyword(expression, L"local" );                              // get value of local:    keyword -- if not present, R/C is -1 

    if (global_rc == 0)                                                                       // global present?
        tf = is_global_identifier_defined(expression.rparms.values.at(0).string);             // identifier name should be in .string field 
    else if (static_rc == 0 )                                                                 // static: present?
        tf = is_static_identifier_defined(frame, expression.rparms.values.at(0).string);      // identifier name should be in .string field 
    else if (verbmain_rc == 0 )                                                               // verbmain: present?
        tf = is_verbmain_identifier_defined(frame, expression.rparms.values.at(0).string);    // identifier name should be in .string field 
    else if (local_rc == 0 )                                                                  // local: present?
        tf = is_local_identifier_defined(frame, expression.rparms.values.at(0).string);       // identifier name should be in .string field 
    else                                                                                      // neither global: verbmain: static: or local: -- must be all: or default
        tf = is_identifier_defined(frame, expression.rparms.values.at(0).string);             // identifier name should be in .string field  

    results = tf_results(tf);                                                                 // pass back 0 or 1 as output results
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_VAR xxxx
//---------------------------------------------------------------------------------

int verb_is_var(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side unevaluated identifier positional, and an optional keyword = all: global: local: (all: is default)
    M__(M_out(L"verb_is_var() called");)
    
    bool tf {false};                                                                          // output value                                 


    // see if global:, static:, verbmain:, or local: are defined -- only one of these (or none) should be defined
                                                                  
    auto global_rc   = get_right_keyword(expression, L"global");                              // get value of global:   keyword -- if not present, R/C is -1 
    auto static_rc   = get_right_keyword(expression, L"static");                              // get value of static:   keyword -- if not present, R/C is -1 
    auto verbmain_rc = get_right_keyword(expression, L"verbmain");                            // get value of verbmain: keyword -- if not present, R/C is -1 
    auto local_rc    = get_right_keyword(expression, L"local" );                              // get value of local:    keyword -- if not present, R/C is -1 


    // set tf to true if variable is defined, and not constant

    if (global_rc == 0)                                                                       // global present?
        tf = is_global_identifier_variable(expression.rparms.values.at(0).string);            // identifier name should be in .string field 
    else if (static_rc == 0)                                                                  // static: present?
        tf = is_static_identifier_variable(frame, expression.rparms.values.at(0).string);     // identifier name should be in .string field 
    else if (verbmain_rc == 0 )                                                               // verbmain: present?
        tf = is_verbmain_identifier_variable(frame, expression.rparms.values.at(0).string);   // identifier name should be in .string field 
    else if (local_rc == 0)                                                                   // local: present?
        tf = is_local_identifier_variable(frame, expression.rparms.values.at(0).string);      // identifier name should be in .string field 
    else                                                                                      // neither global: verbmain: static: or local: -- must be all: or default
        tf = is_identifier_variable(frame, expression.rparms.values.at(0).string);            // identifier name should be in .string field 

    results = tf_results(tf);                                                                 // pass back 0 or 1 as output results
    return 0; 
}
M_endf


//---------------------------------------------------------------------------------
//   @IS_CONST xxxx
//---------------------------------------------------------------------------------

int verb_is_const(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side unevaluated identifier positional, and an optional keyword = all: global: local: (all: is default)
    M__(M_out(L"verb_is_const() called");)
    
    bool tf {false};                                                                         // output value                                 


    // see if global:, static:, verbmain:, or local: are defined -- only one of these (or none) should be defined
                                                                  
    auto global_rc   = get_right_keyword(expression, L"global");                              // get value of global:   keyword -- if not present, R/C is -1 
    auto static_rc   = get_right_keyword(expression, L"static");                              // get value of static:   keyword -- if not present, R/C is -1 
    auto verbmain_rc = get_right_keyword(expression, L"verbmain");                            // get value of verbmain: keyword -- if not present, R/C is -1 
    auto local_rc    = get_right_keyword(expression, L"local" );                              // get value of local:    keyword -- if not present, R/C is -1 

    if (global_rc == 0)                                                                       // global present?
        tf = is_global_identifier_const(expression.rparms.values.at(0).string);               // identifier name should be in .string field 
    else if (static_rc == 0)                                                                  // static: present?
        tf = is_static_identifier_const(frame, expression.rparms.values.at(0).string);        // identifier name should be in .string field  
    else if (verbmain_rc == 0 )                                                               // verbmain: present?
        tf = is_verbmain_identifier_const(frame, expression.rparms.values.at(0).string);      // identifier name should be in .string field 
    else if (local_rc == 0)                                                                   // local: present?
        tf = is_local_identifier_const(frame, expression.rparms.values.at(0).string);         // identifier name should be in .string field  
    else                                                                                      // neither global: verbmain: static: or local: -- must be all: or default
        tf = is_identifier_const(frame, expression.rparms.values.at(0).string);               // identifier name should be in .string field 

    results = tf_results(tf);                                                                 // pass back 0 or 1 as output results
    return 0; 
}
M_endf
 



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//║╳╳╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳║
//║╳╳║      structure, array, argument and vlist extraction verbs
//║╳╳║
//║╳╳╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
//║╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳╳
//╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    (@SUBSCRIPT)  = "#" -- array # nn -- array subscripting -- get n-th element
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_subscript(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    ref_S ref { };                                                   // resulting lvalue/rvalue reference from subscripting operation                  

    // -----------------------------------------------------------------------------------------------------
    // already known that there is one left-side positional parm  -- can be: unevaluated variable identifier
    //                                                                       unevaluated constant identifier
    //                                                                       unevaluated ref -- to array value      
    //                                                                       evaluated array value
    //
    // already known that there is one right-side positional parm -- type should be int64 
    // -----------------------------------------------------------------------------------------------------


    M__(M_out(L"verb_subscript() called");)


    // get values from left-side and right-side parms (known to be present)

    int64_t array_index  { M_get_right_pos_int64(expression, 0) };       // right-side parm should be index (int64_t)
    value_S source_value { M_get_left_pos(       expression, 0) };       // left-side parm is source 
  

    // handle case with variable or constant (unevaluated) identifier -- needs to have an array value
    // ==============================================================================================

    if (source_value.ty == type_E::identifier)
    {
        // fetch symval for identifier -- should be defined with array type value  -- error, if not 
        // ----------------------------------------------------------------------------------------

        symval_S symval { };                                             // should point to real value_s for the variable/constant
        auto get_rc = get_var(frame, source_value.string, symval);

        if (get_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb # -- unexpected error -- identifier %S is not defined") % source_value.string; 
            msgend_loc(M_get_left_pos(expression, 0), expression);
       
            results = error_results(); 
            return -1; 
        }


        // check to see if weak pointer in symval is still valid 
      
        if (symval.value_wp.expired())
        {
            count_error(); 
            M_out_emsg(L"verb # -- symval (weak alias?) for identifier (\"%S\") no longer points to a valid value -- cannot continue") % source_value.string;   
          
            results = error_results(); 
            return -1;
        }


        if (symval.value_wp.lock()->ty != type_E::array)
        {
            count_error(); 
            M_out_emsg1(L"verb # -- identifier %S is type <%S>, not array -- cannot apply subscript to a non-array value") % source_value.string % type_str(symval.value_wp.lock()->ty); 
            msgend_loc(M_get_left_pos(expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure array index is within range
        // -------------------------------------

        if ( (array_index < 0) || (array_index >= symval.value_wp.lock()->typdef_sp->acount) )
        {
            count_error(); 
            M_out_emsg1(L"verb # -- array index (%d) is out of bounds of array (0 - %d) currently assigned to identifier %S") % array_index %  (symval.value_wp.lock()->typdef_sp->acount - 1) % source_value.string ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for i-th array element
        // ----------------------------------------------


        //  get element typdef and compute offset for the i-th array element         

        typdef_S elem_typdef { *(symval.value_wp.lock()->typdef_sp->atype_sp) };                // local copy of array element typdef
        uint64_t elem_offset { array_index * elem_typdef.tsize         };                       // starting offset for i-th array element 


        // create ref_S and fill in rvalue or lvalue flag based on symval_S 

        auto m_rc = make_reference(ref, symval.value_wp.lock(), elem_typdef, elem_offset);      // construct reference based on info from symval -- will have set in_buffer and auto_deref flags -- will not have set rvalue/lvalue flags
                               
        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb # -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        } 

        if (symval.is_const)                                                             // if constant, can't have lvalue reference
           ref.is_rvalue = true; 
        else                                                                             // must be variable -- can have lvalue
           ref.is_lvalue = true;   
    }


    // handle case with (anonymous) array value -- probably an immediately-subscripted    @AGGR array:[...] call
    // =========================================================================================================

    else  if (source_value.ty == type_E::array)
    {
        // make sure array index is within range
        // -------------------------------------

        if ( (array_index < 0) || (array_index >= source_value.typdef_sp->acount) )
        {
            count_error(); 
            M_out_emsg1(L"verb # -- array index (%d) is out of bounds of passed-in (anonymous) array (0 - %d)") % array_index %  (source_value.typdef_sp->acount - 1) ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for i-th array element
        // ----------------------------------------------
        
        //  get element typdef and compute offset for the i-th array element         

        typdef_S elem_typdef { *(source_value.typdef_sp->atype_sp)     };                // local copy of array element typdef
        uint64_t elem_offset { array_index * elem_typdef.tsize         };                // starting offset for i-th array element 


        // create ref_S (to a new copy of the anonymous value) and fill in as rvalue-only  

        std::shared_ptr<value_S> newval_sp { new value_S };                              // new value_S that can be controlled via shared ptr
        *newval_sp = source_value;                                                       // fill in new value_S, but leave everything pointed to by any shared_ptrs still shared (no unshare_value() issued) 

        auto m_rc = make_reference(ref, newval_sp, elem_typdef, elem_offset);            // construct reference based on info from copied value_S -- will have set in_buffer and auto_deref flags -- will not have set rvalue/lvalue flags

        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb # -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        } 

        ref.is_rvalue = true;                                                            // always rvalue-only, since this is (anonymous) passed-in value     
    }


    // handle case with reference value -- needs to be to an auto_defer reference to array (probably nested subscripting) 
    // ==================================================================================================================

    else  if (source_value.ty == type_E::ref)
    {
        // verify that passed-in reference is to array
        // -------------------------------------------

        // make sure passed-in reference is not explicitly-created reference (that needs de-referencing first)
 
        if ( !source_value.ref_sp->auto_deref )
        {
            count_error(); 
            M_out_emsg1(L"verb # -- left-side explicit reference needs to be dereferenced (to an array type) before subscripting"); 
            msgend_loc(M_get_left_pos( expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure passed-in (auto-deref) reference is to an array (not structure, etc.)

        if ( source_value.ref_sp->typdef_sp->kind != type_E::array )
        {
            count_error(); 
            M_out_emsg1(L"verb # -- left-side parm refers to type <%S> -- it needs to refer to an array type before subscripting") % type_str(source_value.ref_sp->typdef_sp->kind);
            msgend_loc(M_get_left_pos( expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure array index is within range
        // -------------------------------------

        if ( (array_index < 0) || (array_index >= source_value.ref_sp->typdef_sp->acount) )
        {
            count_error(); 
            M_out_emsg1(L"verb # -- array index (%d) is out of bounds of referenced array (element) (0 - %d)") % array_index %  (source_value.ref_sp->typdef_sp->acount - 1) ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for i-th array element
        // ----------------------------------------------
        
        //  get element typdef and compute offset for the i-th array element         

        typdef_S elem_typdef { *(source_value.ref_sp->typdef_sp->atype_sp)                      };            // local copy of array element typdef
        uint64_t elem_offset { source_value.ref_sp->offset + array_index * elem_typdef.tsize    };            // starting offset for i-th array element (within prior reference's offsetted sub-buffer) 

        auto m_rc = make_reference(ref, source_value, elem_typdef, elem_offset );                             // construct reference based on info from value_S -- should have copied all required flags from source value_S

        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb # -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"array");
            msg_loc(M_get_right_pos(expression, 0), L"index");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        }      
    }


    // handle unexpected value type -- error
    // =====================================

    else    
    {
        count_error(); 
        M_out_emsg1(L"verb # -- unexpected left-side value -- type = %S") % type_str(source_value.ty) ; 
        msgend_loc(M_get_left_pos(expression, 0), expression);
      
        results = error_results(); 
        return -1;    
    }   


    // pass back completed lvalue or rvalue reference

    results = to_results(ref_val(ref));
    return 0;  
 
}
M_endf  


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    (@SELECT)  "."  -- struct . fieldname  -- structure field selection -- get named field
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_select(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    ref_S ref { };                                                            // resulting lvalue/rvalue reference from subscripting operation                  

    // -----------------------------------------------------------------------------------------------------
    // already known that there is one left-side positional parm  -- can be: unevaluated variable identifier
    //                                                                       unevaluated constant identifier
    //                                                                       unevaluated ref -- to struct value      
    //                                                                       evaluated struct value
    //
    // already known that there is one right-side positional parm -- type should be string (fieldname) 
    // -----------------------------------------------------------------------------------------------


    M__(M_out(L"verb_select() called");)


    // get values from left-side and right-side parms (known to be present)

    std::wstring field_name   { M_get_right_pos_string(expression, 0) };      // right-side parm should be field name (string)
    value_S      source_value { M_get_left_pos(        expression, 0) };      //  left-side parm is source 
  

    // handle case with variable or constant (unevaluated) identifier -- needs to have an structure value
    // ==================================================================================================

    if (source_value.ty == type_E::identifier)
    {
        // fetch symval for identifier -- should be defined with array type value  -- error, if not 
        // ----------------------------------------------------------------------------------------

        symval_S symval { };                                                  // should point to real value_s for the variable/constant
        auto get_rc = get_var(frame, source_value.string, symval);

        if (get_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb . -- unexpected error -- identifier %S is not defined") % source_value.string; 
            msgend_loc(M_get_left_pos(expression, 0), expression);
       
            results = error_results(); 
            return -1; 
        }


        // check to see if weak pointer in symval is still valid 
      
        if (symval.value_wp.expired())
        {
            count_error(); 
            M_out_emsg(L"verb . -- symval (weak alias?) for identifier (\"%S\") no longer points to a valid value -- cannot continue") % source_value.string;   
          
            results = error_results(); 
            return -1;
        }
 

        if (symval.value_wp.lock()->ty != type_E::structure)
        {
            count_error(); 
            M_out_emsg1(L"verb . -- identifier %S is type <%S>, not structy -- cannot apply field selection to a non-struct value") % source_value.string % type_str(symval.value_wp.lock()->ty); 
            msgend_loc(M_get_left_pos(expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure field name is defined in the structure
        // ------------------------------------------------
                
        if ( symval.value_wp.lock()->typdef_sp->fnames.count(field_name) <= 0 )
        {
            count_error(); 
            M_out_emsg1(L"verb . -- field-name (%S) is not defined in the struct-type aggregate currently assigned to identifier %S") % field_name % source_value.string ; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for selected field
        // ------------------------------------------

        //  get field typdef and compute offset for this field 

        auto      field_index  { symval.value_wp.lock()->typdef_sp->fnames.at(field_name) };    // get index of field definition in fdefs vector
        fieldef_S field_def    { symval.value_wp.lock()->typdef_sp->fdefs.at(field_index) };    // fetch field definition 
        typdef_S  field_typdef { *(field_def.ftype_sp)                                    };    // local copy of field typdef_S
        uint64_t  field_offset { field_def.offset                                         };    // starting offset for selected field 


        // create ref_S and fill in rvalue or lvalue flag based on symval_S 

        auto m_rc = make_reference(ref, symval.value_wp.lock(), field_typdef, field_offset);    // construct reference based on info from symval -- will have set in_buffer and auto_deref flags -- will not have set rvalue/lvalue flags
                               
        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb . -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        } 

        if (symval.is_const)                                                             // if constant, can't have lvalue reference
           ref.is_rvalue = true; 
        else                                                                             // must be variable -- can have lvalue
           ref.is_lvalue = true;   
    }


    // handle case with (anonymous) array value -- probably an immediately-subscripted    @AGGR array:[...] call
    // =========================================================================================================

    else  if (source_value.ty == type_E::structure)
    {
        // make sure field-name is defined in the structure
        // ------------------------------------------------

        if ( source_value.typdef_sp->fnames.count(field_name) <= 0 )
        {
            count_error(); 
            M_out_emsg1(L"verb . -- field-name (%S) is not defined in passed-in (anonymous) struct") % field_name; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for selected field
        // ------------------------------------------

        //  get field typdef and compute offset for this field 

        auto      field_index  { source_value.typdef_sp->fnames.at(field_name) };              // get index of field definition in fdefs vector
        fieldef_S field_def    { source_value.typdef_sp->fdefs.at(field_index) };              // fetch field definition 
        typdef_S  field_typdef { *(field_def.ftype_sp)                         };              // local copy of field typdef_S
        uint64_t  field_offset { field_def.offset                              };              // starting offset for selected field 
        
        std::shared_ptr<value_S> newval_sp { new value_S };                                    // new value_S that can be controlled via shared ptr
        *newval_sp = source_value;                                                             // fill in new value_S, but leave everything pointed to by any shared_ptrs still shared (no unshare_value() issued) 

        auto m_rc = make_reference(ref, newval_sp, field_typdef, field_offset);                // construct reference based on info from copied value_S -- will have set in_buffer and auto_deref flags -- will not have set rvalue/lvalue flags

        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb . -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        } 

        ref.is_rvalue = true;                                                            // always rvalue-only, since this is (anonymous) passed-in value     
    }


    // handle case with reference value -- needs to be to an auto_defer reference to array (probably nested subscripting) 
    // ==================================================================================================================

    else  if (source_value.ty == type_E::ref)
    {
        // verify that passed-in reference is to array
        // -------------------------------------------

        // make sure passed-in reference is not explicitly-created reference (that needs de-referencing first)
 
        if ( !source_value.ref_sp->auto_deref )
        {
            count_error(); 
            M_out_emsg1(L"verb . -- left-side explicit reference needs to be dereferenced (to a struct type) before field selection"); 
            msgend_loc(M_get_left_pos( expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure passed-in (auto-deref) reference is to a structure (not array, etc.)

        if ( source_value.ref_sp->typdef_sp->kind != type_E::structure )
        {
            count_error(); 
            M_out_emsg1(L"verb . -- left-side parm refers to type <%S> -- it needs to refer to a struct type before field selection") % type_str(source_value.ref_sp->typdef_sp->kind);
            msgend_loc(M_get_left_pos( expression, 0), expression);
       
            results = error_results(); 
            return -1;         
        }


        // make sure field-name is defined in referred-to struct
        // -----------------------------------------------------

        if ( source_value.ref_sp->typdef_sp->fnames.count(field_name) <= 0 )
        {
            count_error(); 
            M_out_emsg1(L"verb . -- field-name (%S) is not defined in referenced struct") % field_name; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;         
        }


        // create output reference for selected field
        // ------------------------------------------

        //  get field typdef and compute offset for this field 

        auto      field_index  { source_value.ref_sp->typdef_sp->fnames.at(field_name) };    // get index of field definition in fdefs vector
        fieldef_S field_def    { source_value.ref_sp->typdef_sp->fdefs.at(field_index) };    // fetch field definition 
        typdef_S  field_typdef { *(field_def.ftype_sp)                                 };    // local copy of field typdef_S
        uint64_t  field_offset { field_def.offset                                      };    // starting offset for selected field (within prior reference's offsetted sub-buffer)

        auto m_rc = make_reference(ref, source_value, field_typdef, field_offset );          // construct reference based on info from value_S -- should have copied all required flags from source value_S

        if (m_rc != 0)
        {
            count_error(); 
            M_out_emsg1(L"verb . -- make_reference() function failed -- see above error message for reason") ; 
            msg_loc(M_get_left_pos( expression, 0), L"struct"    );
            msg_loc(M_get_right_pos(expression, 0), L"field-name");
            msgend_loc(expression);
       
            results = error_results(); 
            return -1;           
        }      
    }


    // handle unexpected value type -- error
    // =====================================

    else    
    {
        count_error(); 
        M_out_emsg1(L"verb . -- unexpected left-side value -- type = %S") % type_str(source_value.ty) ; 
        msgend_loc(M_get_left_pos(expression, 0), expression);
      
        results = error_results(); 
        return -1;    
    }   


    // pass back completed lvalue or rvalue reference

    results = to_results(ref_val(ref));
    return 0;  
 
}
M_endf  


#if 0 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    keys_vlist() -- helper function for @ARG_ASSIGN and @VL_ASSIGN -- build positional value vlist from keyword instance values
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void keys_vlist(frame_S& frame, const e_expression_S& expression, const vlist_S& in_vlist, vlist_S& out_vlist, const std::wstring& keyname) try
{
    out_vlist = vlist_S { };      // clear out, in case no keyword values to add


    // return imemdiately with empty out_vlist, if keyword is not present in in_vlist 

    auto key_ct = in_vlist.eval_kws.count(keyname); 
    if (key_ct <= 0)
        return; 


    // loop to extract matching keywords from in_vlist

    auto it_k = in_vlist.eval_kws.find(keyname);         // it_k should be valid, since count was non-zero, above

    for (auto i = 0; i < key_ct; i++)
    {
        add_positional_value(out_vlist, it_k->second);  // add in value for this keyword instance       
        it_k++;                                         // advance to next instance of this keyword in in_vlist
    }  

    return; 
}
M_endf
      

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    assign_vlist() -- helper function for @ARG_ASSIGN and @VL_ASSIGN  -- assign 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int assign_vlist(frame_S& frame, const e_expression_S& expression, const vlist_S& vars_vlist, const vlist_S& values_vlist, const value_S& rest, bool define_vars = false, bool make_const = false) try
{ 
    M__(M_out(L"assign_vlist() -- called -- rest.string = %s    (rest.ty == type_E::identifier) = %d") % rest.string % (rest.ty == type_E::identifier);) 

    bool        rest_valid      {rest.ty == type_E::identifier};                      // rest is valid if it contains variable name as the value string                                                                                                                                 
    auto        var_ct       =  vars_vlist.value_ct;                                  // number of variable names in vars_vlist;     
    auto        value_ct     =  values_vlist.value_ct;                                // number of values to assign to variables in values_vlist; 
                                                                                     
    value_S     unit_value      { unit_val() };                                       // unit value for pre-defining variables, as required     
    vlist_S     rest_vlist      {};                                                   // vlist to be assigned to rest (if rest_valid)  
   

        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????
        // ???? does this need local: static: verbmain: keywords ????


    // main loop to define/assign positional vlist values to corresponding variables
    //------------------------------------------------------------------------------
    
    for (auto i = 0; i < std::max(var_ct, value_ct); i++)
    {
        // if is still in range of variables vlist, define any variables that don't yet exist (if requested by caller), and do value assignment (if values haven't been exhausted)
    
        if (i < var_ct)
        {
            bool just_defined {false};                                                // flag to indicate if variable was defined during this loop pass

            std::wstring var_name = vars_vlist.values.at(i).string;                   // get variable name being defined/assigned-to
    
                                                
            // define this variable, if required
    
            if ( (define_vars) &&  (!is_local_identifier_defined(frame, var_name) ) ) // need to define local variable before assignment? 
            {
                auto src = def_local_var(frame, var_name, unit_value);                // define local var with unit value
                if (src != 0)
                {
                    count_error(); 
                    M_out_emsg1(L"assign_vlist() -- unexpected error from def_local_var(,%S,)") % var_name; 
                    msgend_loc(vars_vlist.values.at(i), expression);
                    return -1;             
                 }

                 just_defined = true;                                                 // indicate that variable was just defined during this loop pass
            }
            else
            {
                just_defined = false;                                                 // variable was not just defined

                if (make_const)
                {
                    count_error(); 
                    M_out_emsg1(L"assign_vlist() -- cannot make pre-existing variable (%S) into a constant") % var_name;      // making existing variable constant would cause trouble for @ALIAS
                    msgend_loc(vars_vlist.values.at(i), expression);
                    return -1; 
                }
            }
             
    
            // if i is also in range of values vlist  -- do variable/constant assignment: variable-n = value-n 
    
            if (i < value_ct)
            {
                if (!is_local_identifier_variable(frame, var_name))
                {
                    count_error(); 
                    M_out_emsg1(L"assign_vlist() -- cannot update non-variable (constant or verb) identifier = %S") % var_name; 
                    msgend_loc(vars_vlist.values.at(i), expression);
                    return -1;         
                } 

                auto arc = update_local_var(frame, var_name, values_vlist.values.at(i), make_const);     // note: make_const should be true only when define_var is true 
                if (arc != 0)
                {
                    count_error(); 
                    M_out_emsg1(L"assign_vlist() -- unexpected error from update_local_var(,%S,)") % var_name; 
                    msgend_loc(vars_vlist.values.at(i), expression);
                    return -1;             
                } 
            }
            else    // variable exists, but has no corresponding value  -- assign unit value to this variable (if not just defined above)
            {
                if (!just_defined)
                {
                    if (!is_local_identifier_variable(frame, var_name))
                    {
                        count_error(); 
                        M_out_emsg1(L"assign_vlist() -- cannot update non-variable (constant or verb) identifier = %S") % var_name; 
                        msgend_loc(vars_vlist.values.at(i), expression);
                        return -1;         
                    }  

                    auto arc = update_local_var(frame, var_name, unit_value, make_const);
                    if (arc != 0)
                    {
                        count_error(); 
                        M_out_emsg1(L"assign_vlist() -- unexpected error from update_local_var(,%S,) -- (unit value)") % var_name; 
                        msgend_loc(vars_vlist.values.at(i), expression);
                        return -1;             
                    } 
                }
            }   
        }              // i < var_ct
        else           // must be i >= var_ct but < value_ct -- value exists but has no correspinging variable 
        {
             // append this value to the vlist for the "rest" keyword (if any)
        
             if (rest_valid)
                add_positional_value(rest_vlist, values_vlist.values.at(i));   
        }  
    }           // end of main loop


    // if rest variable was provided, attach vlist to value and assign to the "rest" variable

    if (rest_valid)
    {
        M__(M_out(L"assign_vlist() -- rest_valid");)

        if ( (define_vars) &&  (!is_local_identifier_defined(frame, rest.string) ) )            // need to define local variable before assignment? 
        {
            auto src = def_local_var(frame, rest.string, vlist_val(rest_vlist));                // define local variable with rest vlist
            if (src != 0)
            {
                count_error(); 
                M_out_emsg1(L"assign_vlist() -- unexpected error from def_local_var(,%s,) -- (rest of vlist)") % rest.string; 
                msgend_loc(rest, expression);
                return -1;             
            }
        }
        else      // "rest" variable is already defined
        {
            M__(M_out(L"assign_vlist() -- update rest: var -- vlist.value_ct = %d") % rest_vlist.value_ct;)

            if (!is_local_identifier_variable(frame, rest.string))
            {
                count_error(); 
                M_out_emsg1(L"assign_vlist() -- cannot update non-variable (constant or verb) identifier = %s") % rest.string; 
                msgend_loc(expression);
                return -1;         
            }  

            if (make_const)
            {
                count_error(); 
                M_out_emsg1(L"assign_vlist() -- cannot make pre-existing variable (%S) into a constant") % rest.string;      // making existing variable constant would cause trouble for @ALIAS
                msgend_loc(expression);
                return -1; 
            }  

            auto arc = update_local_var(frame, rest.string, vlist_val(rest_vlist), make_const); 
            if (arc != 0)
            {
                count_error(); 
                M_out_emsg1(L"assign_vlist() -- unexpected error from update_local_var(,%s,) -- (rest of vlist)") % rest.string; 
                msgend_loc(expression);
                return -1;             
            }         
        }     
    }         

    return 0; 
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    v1 v2 v3 v4 ... v999 rest:v0 key:"string" @ARG_ASSIGN v11 v22 v33 ... v99999 rest:v00 key:"string"  -- assign positional values in arguments to variables on correspinging side 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_arg_assign(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_arg_assign() called");)

    // known that there are zero or more variables (undefined, or defined and non-constant0 on each side, along with optional rest: keyword, also with a single undefined or defined non-constant variable
    // also an optionsl key:"string"keyword can be present on each side     
    

    // do left side argument vlist, if any left-side parms are present 
    // ---------------------------------------------------------------

    if  ((expression.lparms.value_ct > 0) || (expression.lparms.kw_ct > 0) ) 
    {
        value_S key_val  { }; 
        value_S rest_val { }; 
        auto krc = get_left_keyword(expression, L"key" , key_val );                                     // get value of key: keyword  -- if not defined, value will be unit, and R/C -1 
        auto lrc = get_left_keyword(expression, L"rest", rest_val);                                     // get value of rest: keyword -- if not defined, value will be unit, and R/C -1

        M__(M_out(L"verb_arg_assign() -- left-side rest: = %s    key: = %s") % rest_val.string % key_val.string;)

        if (krc != 0)
        {
            // key: not present -- do positional values in vlist 

            auto arc = assign_vlist(frame, expression, expression.lparms, frame.lparms, rest_val, true, false); // define variables  -- non-const
            if (arc != 0)
            {
                results = error_results(); 
                return arc;
            }
        }
        else   
        {
            // key: present -- do values associated with key:"keyname"
    
            vlist_S key_vlist {}; 
            keys_vlist(frame, expression, frame.lparms, key_vlist, key_val.string);   // extract keyword instance values into key_vlist
     
            auto arc = assign_vlist(frame, expression, expression.lparms, key_vlist, rest_val, true, false);   // define variables  -- non-const
            if (arc != 0)
            {
                results = error_results(); 
                return arc;
            }     
        }
    }          // end of left-side processing


    // do right side argument vlist, if any right-side parms are present 
    // -----------------------------------------------------------------

    if  ((expression.rparms.value_ct > 0) || (expression.rparms.kw_ct > 0) ) 
    {
        value_S key_val  { }; 
        value_S rest_val { }; 
        auto krc = get_right_keyword(expression, L"key" , key_val );                                     // get value of key: keyword  -- if not defined, value will be unit, and R/C -1 
        auto lrc = get_right_keyword(expression, L"rest", rest_val);                                     // get value of rest: keyword -- if not defined, value will be unit, and R/C -1

        M__(M_out(L"verb_arg_assign() -- right-side rest: = %s    key: = %s") % rest_val.string % key_val.string;)

        if (krc != 0)
        {
            // key: not present -- do positional values in vlist 

            auto arc = assign_vlist(frame, expression, expression.rparms, frame.rparms, rest_val, true, false);        // define variables -- non-const
            if (arc != 0)
            {
                results = error_results(); 
                return arc;
            }
        }
        else   
        {
            // key: present -- do values associated with key:"keyname"
    
            vlist_S key_vlist {}; 
            keys_vlist(frame, expression, frame.rparms, key_vlist, key_val.string);                       // extract keyword instance values into key_vlist
     
            auto arc = assign_vlist(frame, expression, expression.rparms, key_vlist, rest_val , true, false);            // define variables -- non-const
            if (arc != 0)
            {
                results = error_results(); 
                return arc;
            }      
        }
    }         // end of right-side processing   


    // no error -- return normally
               
    results = unit_results();                   // return unit results
    return 0; 
}
M_endf
#endif



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @ARG_CT    -or-     @ARG_CT left: key:"string"   -or    @ARG_CT right: key:"string"     -- get left-side or right-side positional or keyword argument counts for this frameblock/verb-block/mainblock
//
//
//          @ARG_CT left:               -- return total number of left-side  positional arguments
//          @ARG_CT right:              -- return total number of right-side positional arguments  
//          @ARG_CT left:  allkeys:     -- return total number of left-side  keyword arguments
//          @ARG_CT right: allkeys;     -- return total number of right-side keyword arguments     
//          @ARG_CT left:  key:"xxx"    -- return number of times left-side  keyword "xxx" appears
//          @ARG_CT right: key:"xxx"    -- return number of times right-side keyword "xxx" appears
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_arg_ct(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are no positional parms, and an optional right: or left: keyword 
    //    also -- an optional key:"xxxx" string keyword or allkeys: keyword (both allkeys: and key: cannot both be present   

    M__(M_out(L"verb_arg_ct() called");)
    value_S ct_value { }; 

    // see if left: or key:/allkeys: keywords are present on right side

    value_S left_value      { };
    value_S key_value       { };
    value_S allkeys_value   { };

    auto left_rc     = get_right_keyword(expression, L"left"    ,  left_value    ); // r/c = -1, if left: keyword is not present -- if left: not present, right: must be 
    auto key_rc      = get_right_keyword(expression, L"key"     ,  key_value     ); // r/c = -1, if key:     keyword is not present 
    auto allkeys_rc  = get_right_keyword(expression, L"allkeys" ,  allkeys_value ); // r/c = -1, if allkeys: keyword is not present


    // get right/left positional/keyword count, as requested

    if (key_rc == 0)                                                                // key: is present ?
    {
        // key:"xxx"-- get count of specified keyword
    
        if (left_rc == 0)                                                           // left: is present  
            ct_value = int64_val(frame.lparms.eval_kws.count(key_value.string));    // get number of left keywords 
        else                                                                        // left: is not present -- assume right: present, or neither left: or right: present (default is right-side count) 
            ct_value = int64_val(frame.rparms.eval_kws.count(key_value.string));    // get number of right keywords
    }
    else if (allkeys_rc == 0)                                                       // allkeys: present  ?
    {
        // allkeys: -- get count of all keywords
    
        if (left_rc == 0)                                                           // left: is present  
            ct_value = int64_val(frame.lparms.eval_kws.size());                     // get number of left keywords 
        else                                                                        // left: is not present -- assume right: present, or neither left: or right: present (default is right-side count) 
            ct_value = int64_val(frame.rparms.eval_kws.size());                     // get number of right keywords
    }
    else                                                                            // allkeys: and key: not present
    {
         // get count of positional parms

         if (left_rc == 0)                                                          // left: is present 
              ct_value = int64_val(frame.lparms.values.size());                     // get number of left positional parms 
         else                                                                       // left: is not present -- assume right: present, or neither left: or right: present (default is right-side count) 
              ct_value = int64_val(frame.rparms.values.size());                     // get number of right positional parms
    }
                                                                                   
    results = to_results(ct_value);                                                 // return left/right arg count as output value
    return 0;
}
M_endf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    @ARGS left: right: -- return vlist value with either left-side or right-side vlist for this frameblock/verb-block/mainblock
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_args(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there are no positional parms, and an optional right: or left: keyword  

    M__(M_out(L"verb_args() called");)
    value_S args_value { }; 

    // see if left: keyword is present on right side

    value_S left_value { };
    auto lrc = get_right_keyword(expression, L"left", left_value);  // r/c = -1, if left: keyword is not present 


    // get right/left vlist, as requested  

    if (lrc == 0)                                            // left: is present  
         args_value = vlist_val(frame.lparms);               // get left vlist for frameblock/verb-block/mainblock 
    else                                                     // left: is not present -- assume right: present, or neither left: or right: present (default is right-side count) 
         args_value = vlist_val(frame.rparms);               // get right vlist for frameblock/verb-block/mainblock

                                                                                   
    results = to_results(args_value);                        // return left/right frameblock/verb-block/mainblock parm vlist as output value
    return 0;
}
M_endf  


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//         @ARG n right: left: key:"string"           -- get n-th left_side/right-side positional/keyword parm for this frameblock/verb-block/mainblock
//
//               @ARG n left:               -- return n-th left-side  positional argument
//               @ARG n right:              -- return n-th right-side positional argument  
//               @ARG n left:  allkeys:     -- return n-th left-side  keyword arguments
//               @ARG n right: allkeys;     -- return n-th right-side keyword arguments     
//               @ARG n left:  key:"xxx"    -- return n-th left-side  keyword "xxx" argument
//               @ARG n right: key:"xxx"    -- return n-th right-side keyword "xxx" argument
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

int verb_arg(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional int64 parm (0-N)
    //  also -- an optional right: or left: keyword, and an optional key: string keyword or allkeys: keyword (both allkeys: and key: cannot both be present   

    M__(M_out(L"verb_arg() called");)
    value_S arg_value { unit_val() };                                                      // unit value, in case n-th parm does not exist


    // get positional parm number requested by caller -- 1st right parm should be present and be >= 0

    auto parm_n = expression.rparms.values.at(0).int64;          

 
    // see if left: or key:/allkeys: keywords are present on right side

    value_S left_value      { };
    value_S key_value       { };
    value_S allkeys_value   { };

    auto left_rc     = get_right_keyword(expression, L"left"    ,  left_value    );         // r/c = -1, if left: keyword is not present -- if left: not present, right: must be 
    auto key_rc      = get_right_keyword(expression, L"key"     ,  key_value     );         // r/c = -1, if key:     keyword is not present 
    auto allkeys_rc  = get_right_keyword(expression, L"allkeys" ,  allkeys_value );         // r/c = -1, if allkeys: keyword is not present
     

    if  (key_rc == 0)                                                                       // key:"xxxx" present -- caller wants keyword "key_value.string" parm 
    {
        // get n-th occurrence of key:"string" keyword 
     
        if (left_rc == 0)                                                                   // caller wants n-th occurrence of keyword  in left-side vlist 
            (void) get_vlist_keyword(frame.lparms, key_value.string, arg_value, parm_n);    // r/c=-1 and at_value is unit, if n-th occurrence of this keyword is not present
        else                                                                                // caller wants n-th occurrence of keyword  in left-side vlist
            (void) get_vlist_keyword(frame.rparms, key_value.string, arg_value, parm_n);    // r/c=-1 and at_value is unit, if n-th occurrence of this keyword is not present                                                                                                
    }
    else if  (allkeys_rc == 0)                                                              // allkeys: present -- caller wants value for n-th keyword of any name 
    {
        // get n-th keyword in vlist -- with any keyword name
     
        if (left_rc == 0)                                                                   // caller wants value from n-th keyword  in left-side vlist 
            (void) get_vlist_keyword(frame.lparms, arg_value, parm_n);                      // r/c=-1 and at_value is unit, if n-th occurrence of this keyword is not present
        else                                                                                // caller wants value from n-th keyword  in left-side vlist
            (void) get_vlist_keyword(frame.rparms, arg_value, parm_n);                      // r/c=-1 and at_value is unity, if n-th occurrence of this keyword is not present                                                                                                
    }  
    else                                                                                    // neither key:"xxxx" nor allkeys: present -- caller wants positional parm
    {
        // get n-th positional parm on requested side 
     
        if (left_rc == 0)                                                                   // caller wants n-th positional value in left-side vlist     
            (void)get_vlist_positional(frame.lparms, arg_value, parm_n);                    // r/c=-1 and at_value is unit, if n-th  parm not present  
        else                                                                                // caller wants n-th positional value in right-side vlist
            (void)get_vlist_positional(frame.rparms, arg_value, parm_n);                    // r/c=-1 and at_value is unit, if n-th  parm not present
    }

    results = to_results(arg_value);                        // return value from n-th left/right parm as output value
    return 0;
}
M_endf



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#if 0 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    v1 v2 v3 v4 ... v999 rest:v0 @VL_ASSIGN [vlist] key:"string"  -- assign positional/keyword values in vlist to variables on left side 
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_vl_assign(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    M__(M_out(L"verb_vl_assign() called");)

    // known that there are one or more variables (undefined, or defined and non-constant0 on left side, along with optional rest: keyword, also with a single undefined or defined non-constant variable
    // right-side is known to contain one mandatory vlist parm, and one optional key:"string" parm     
    
    value_S key_val  { }; 
    value_S rest_val { }; 
    auto krc = get_right_keyword(expression, L"key" , key_val );                                     // get value of key: keyword  -- if not defined, value will be unit, and R/C -1 
    auto lrc = get_left_keyword( expression, L"rest", rest_val);                                     // get value of rest: keyword -- if not defined, value will be unit, and R/C -1

    M__(M_out(L"verb_vl_assign() -- rest: = %s    key: = %s") % rest_val.string % key_val.string;)

    if (krc != 0)
    {
        // key: not present -- do positional values in vlist 

        auto arc = assign_vlist(frame, expression, expression.lparms, *(expression.rparms.values.at(0).vlist_sp), rest_val, false); // don't define variables
        if (arc != 0)
        {
            results = error_results(); 
            return arc;
        }
    }
    else   
    {
        // key: present -- do values associated with key:"keyname"
    
        vlist_S key_vlist {}; 
        keys_vlist(frame, expression, *(expression.rparms.values.at(0).vlist_sp), key_vlist, key_val.string);   // extract keyword instance values into key_vlist
     
        auto arc = assign_vlist(frame, expression, expression.lparms, key_vlist, rest_val, false);              // don't define variables
        if (arc != 0)
        {
            results = error_results(); 
            return arc;
        }     
    }


    // no error -- return normally
               
    results = unit_results();                       // return unit results
    return 0; 
}
M_endf
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    [vlist] @AT n key:"string"   -- get n-th positional value in vlist
//
//      [vlist] @AT n              -- return n-th positional value in vlist
//      [vlist] @AT n allkeys:     -- return n-th keyword value in vlist
//      [vlist] @AT n key:"xxx"    -- return value from n-th occurence of keyword "xxx" vlist
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_at(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional int64 parm (0-N), and a left-side vlist positional parm.  
    //  also -- an optional key:"xxxx" keyword or allkeys: keyword (both allkeys: and key: cannot both be present  

    M__(M_out(L"verb_at() called");)
    value_S at_value { unit_val() };                                                                                 // unit value, in case n-th value does not exist


    // get positional value or keyword occurrence number  

    auto value_n = expression.rparms.values.at(0).int64; 


    // see if key:/allkeys: keywords are present on right side

    value_S key_value       { };
    value_S allkeys_value   { };

    auto key_rc      = get_right_keyword(expression, L"key"     ,  key_value     );                                   // r/c = -1, if key:     keyword is not present 
    auto allkeys_rc  = get_right_keyword(expression, L"allkeys" ,  allkeys_value );                                   // r/c = -1, if allkeys: keyword is not present
                                                                                                                     

    // extract requested positional or keyword value

    if       (key_rc     == 0)                                                                                        // key:":xxxx" present? -- caller wants n-th keyword "xxxx" value in vlist 
        (void)get_vlist_keyword(   *(expression.lparms.values.at(0).vlist_sp), key_value.string, at_value, value_n);  // r/c=-1 and at_value is unit, if n-th occurrence of this keyword is not present 
    else if  (allkeys_rc == 0)                                                                                        // allkeys: present ? -- caller wants value from n-th keyword overall
        (void)get_vlist_keyword(   *(expression.lparms.values.at(0).vlist_sp),                   at_value, value_n);  // r/c=-1 and at_value is unit, if n-th keyword overall is not present 
    else                                                                                                              // neither allkey: nor key:"xxxx" -- caller wants n-th positional value
        (void)get_vlist_positional(*(expression.lparms.values.at(0).vlist_sp),                   at_value, value_n);  // r/c=-1 and at_value is unit, if n-th  parm not present  
                                                                                                              
    
    results = to_results(at_value);                                                                                   // return value from n-th value in vlist as output value
    return 0;
}
M_endf



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     @VL_CT [vlist] key:"string"   -- get number of values in vlist, or number of occurrences of keyword "string"
//
//        @VL_CT [vlist]              - return total number of positional values in vlist
//        @VL_CT [vlist] allkeys:     - return total number of keywords in vlist
//        @VL_CT [vlist] key:"xxx"    - return number occurrences of keyword "xxx" in vlist//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int verb_vl_ct(frame_S& frame, const e_expression_S& expression, const verbdef_S& verbdef, results_S& results) try
{
    // already known that there is one right-side positional int64 parm (0-N), and a left-side vlist positional parm.    

    M__(M_out(L"verb_vl_ct() called");)
    value_S ct_value { unit_val() };                                                                                 // unit value, in case n-th value does not exist


    // see if key:/allkeys: keywords are present on right side

    value_S key_value       { };
    value_S allkeys_value   { };

    auto key_rc      = get_right_keyword(expression, L"key"     ,  key_value     );                                  // r/c = -1, if key:     keyword is not present 
    auto allkeys_rc  = get_right_keyword(expression, L"allkeys" ,  allkeys_value );                                  // r/c = -1, if allkeys: keyword is not present


    // determine requested positional or value count 

    if       (key_rc    == 0)                                                                                        // key:"xxxx"present -- caller wants number of occurrences of keyword "xxxx"
        ct_value = int64_val( ((expression.rparms.values.at(0).vlist_sp)->eval_kws).count(key_value.string) );       // number of occurrences of requested keyword 
    else if (allkeys_rc == 0)                                                                                        // allkey: present -- caller wants count of all keywords in vlist 
        ct_value = int64_val( ((expression.rparms.values.at(0).vlist_sp)->eval_kws).size()                  );       // total number of all keywords in vlist  
    else                                                                                                             // neither key:"xxxx"nor allkeys: present -- caller wants count of positional values
        ct_value = int64_val( ((expression.rparms.values.at(0).vlist_sp)->values).size()                    );       // number of positional values 
                                                                                                                                      
    
    results = to_results(ct_value);                                                                                  // return appropriate count
    return 0;
}
M_endf


//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""