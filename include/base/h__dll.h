// h__dll.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ========
////            h__dll.h -- set up export/import control for DLLs  
////            ========
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h__types.h"   


//---------------------------------------------------------------------------------------------------------------------
//     set up DLL import/EXPORT controls based on M_IN_xxx_DLL manifest constants (if not already set up) 
//---------------------------------------------------------------------------------------------------------------------

// set up to import or export items in audio.dll
// ---------------------------------------------

#ifdef M_IN_AUDIO_DLL
  #ifndef M_AUDIO_DLL_EXPORT
    #define M_AUDIO_DLL_EXPORT
  #endif
#else
  #ifndef M_AUDIO_DLL_IMPORT
    #define M_AUDIO_DLL_IMPORT
  #endif
#endif


// set up to import or export items in core.dll
// --------------------------------------------

#ifdef M_IN_CORE_DLL
  #ifndef M_CORE_DLL_EXPORT
    #define M_CORE_DLL_EXPORT
  #endif
#else
  #ifndef M_CORE_DLL_IMPORT
    #define M_CORE_DLL_IMPORT
  #endif
#endif


// set up to import or export items in draw.dll
// --------------------------------------------

#ifdef M_IN_DRAW_DLL
  #ifndef M_DRAW_DLL_EXPORT
    #define M_DRAW_DLL_EXPORT
  #endif
#else
  #ifndef M_DRAW_DLL_IMPORT
    #define M_DRAW_DLL_IMPORT
  #endif
#endif


// set up to import or export items in ex.dll
// ------------------------------------------

#ifdef M_IN_EX_DLL
  #ifndef M_EX_DLL_EXPORT
    #define M_EX_DLL_EXPORT
  #endif
#else
  #ifndef M_EX_DLL_IMPORT
    #define M_EX_DLL_IMPORT
  #endif
#endif


// set up to import or export items in map.dll
// -------------------------------------------

#ifdef M_IN_MAP_DLL
  #ifndef M_MAP_DLL_EXPORT
    #define M_MAP_DLL_EXPORT
  #endif
#else
  #ifndef M_MAP_DLL_IMPORT
    #define M_MAP_DLL_IMPORT
  #endif
#endif


//---------------------------------------------------------------------------------------------------------------------
//     set up dllexport and dllimport controls for external items (if not already set up)
//---------------------------------------------------------------------------------------------------------------------

#define M_DLL_EXP      __declspec( dllexport )
#define M_DLL_IMP      __declspec( dllimport )  


// set up dllexport/dllimport manifest constant for items in audio.dll
// -------------------------------------------------------------------

#ifndef M_AUDIO_IMPEXP
  #ifdef M_AUDIO_DLL_EXPORT
    #define   M_AUDIO_IMPEXP  M_DLL_EXP
  #else 
    #ifdef M_AUDIO_DLL_IMPORT
      #define M_AUDIO_IMPEXP  M_DLL_IMP
    #else
      #define M_AUDIO_IMPEXP 
    #endif
  #endif
#endif 


// set up dllexport/dllimport manifest constant for items in core.dll
// ------------------------------------------------------------------

#ifndef M_CORE_IMPEXP
  #ifdef M_CORE_DLL_EXPORT
    #define   M_CORE_IMPEXP  M_DLL_EXP
  #else 
    #ifdef M_CORE_DLL_IMPORT
      #define M_CORE_IMPEXP  M_DLL_IMP
    #else
      #define M_CORE_IMPEXP 
    #endif
  #endif
#endif 


// set up dllexport/dllimport manifest constant for items in draw.dll
// ------------------------------------------------------------------

#ifndef M_DRAW_IMPEXP
  #ifdef M_DRAW_DLL_EXPORT
    #define   M_DRAW_IMPEXP  M_DLL_EXP
  #else 
    #ifdef M_DRAW_DLL_IMPORT
      #define M_DRAW_IMPEXP  M_DLL_IMP
    #else
      #define M_DRAW_IMPEXP 
    #endif
  #endif
#endif 


// set up dllexport/dllimport manifest constant for items in ex.dll
// ----------------------------------------------------------------

#ifndef M_EX_IMPEXP
  #ifdef M_EX_DLL_EXPORT
    #define   M_EX_IMPEXP  M_DLL_EXP
  #else 
    #ifdef M_EX_DLL_IMPORT
      #define M_EX_IMPEXP  M_DLL_IMP
    #else
      #define M_EX_IMPEXP 
    #endif
  #endif
#endif 


// set up dllexport/dllimport manifest constant for items in map.dll
// ----------------------------------------------------------------

#ifndef M_MAP_IMPEXP
  #ifdef M_MAP_DLL_EXPORT
    #define   M_MAP_IMPEXP  M_DLL_EXP
  #else 
    #ifdef M_MAP_DLL_IMPORT
      #define M_MAP_IMPEXP  M_DLL_IMP
    #else
      #define M_MAP_IMPEXP 
    #endif
  #endif
#endif 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////