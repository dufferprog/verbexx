// h__global.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            =========== 
////            h__global.h -- global items  
////            ===========
////     
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h__types.h"            



//---------------------------------------------------------------------------------------------------------------------
//     Define or declare global variables based on M_DEFINE_GLOBALS manifest constant
//---------------------------------------------------------------------------------------------------------------------

#ifdef M_DEFINE_GLOBALS
 #ifndef M_EXTERN_TAG
  #define M_EXTERN_TAG
 #endif
 #ifndef M_GLOBAL_INIT
  #define M_GLOBAL_INIT(x) x
 #endif
#else
 #ifndef M_EXTERN_TAG
  #define M_EXTERN_TAG extern
 #endif
 #ifndef M_GLOBAL_INIT
  #define M_GLOBAL_INIT(x)
 #endif
#endif


//---------------------------------------------------------------------------------------------------------------------
//     Define or declare thread_local variables based on M_DEFINE_THREAD_LOCAL manifest constant
//---------------------------------------------------------------------------------------------------------------------

#ifdef M_DEFINE_THREAD_LOCAL
 #ifndef M_THREAD_LOCAL_EXTERN_TAG
  #define M_THREAD_LOCAL_EXTERN_TAG
 #endif
 #ifndef M_THREAD_LOCAL_INIT
  #define M_THREAD_LOCAL_INIT(x) x
 #endif
#else
 #ifndef M_THREAD_LOCAL_EXTERN_TAG
  #define M_THREAD_LOCAL_EXTERN_TAG extern
 #endif
 #ifndef M_THREAD_LOCAL_INIT
  #define M_THREAD_LOCAL_INIT(x)
 #endif
#endif







namespace global_N
{

// ===========================================         
// Global variables -- in namespace global_N:: 
// =========================================== 


// ----------------
// global variables
// ----------------

#ifndef _MANAGED
    M_CORE_IMPEXP
    M_EXTERN_TAG std::recursive_mutex                                    mx_cout               M_GLOBAL_INIT({ })               ;   // global mutex for serializing boost::format writes to cout  (debugging, etc.) -- locked/unlocked via lock_guard(), etc.
#endif



// ----------------------------- 
// thread-local global variables               (cannot share across DLLs -- must have one copy in each DLL)
// ----------------------------- 

#ifndef _MANAGED
    M_THREAD_LOCAL_EXTERN_TAG thread_local std::default_random_engine    random_engine         M_THREAD_LOCAL_INIT({ })  ;   // global random engine (one per thread) 
#endif


// ============================================

}



#undef M_EXTERN_TAG
#undef M_THREAD_LOCAL_EXTERN_TAG
#undef M_GLOBAL_INIT
#undef M_THREAD_LOCAL_INIT

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////