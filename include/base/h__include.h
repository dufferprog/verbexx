// h__include.h

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////      
////
////            ============ 
////            h__include.h -- include imported headers and any local ones that don't change much (these go into pre-compiled header)  
////            ============
////                                                                            
////                     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#pragma message ("****************************") 
#pragma message ("** Compiling h__include.h **") 
#pragma message ("****************************")

// set up for windows 10  (not needed -- default is 0x0A00
// ---------------------

//#define WINVER       0x0A00
//#define _WIN32_WINNT 0x0A00  


// select windows.h or afx.h headers
// ---------------------------------
//
// note: to do compile with MFC headers (afx___.h), #define INCLUDE_MFC
//
                                                                       
#ifndef INCLUDE_MFC
  #ifndef INCLUDE_WINDOWS_H 
    #define INCLUDE_WINDOWS_H    
  #endif
#else
  #ifdef INCLUDE_WINDOWS_H 
    #undef INCLUDE_WINDOWS_H    
  #endif
#endif


// make sure not to use MFC in C++/CLI code

#ifdef _MANAGED
  #ifdef INCLUDE_MFC
    #undef INCLUDE_MFC
  #endif 
  #ifndef INCLUDE_WINDOWS_H
    #define INCLUDE_WINDOWS_H 
  #endif
#endif


// Microsoft Windows #defines  
//--------------------------- 

#ifndef UNICODE
  #define UNICODE
#endif

#ifndef _UNICODE
  #define _UNICODE
#endif

#ifndef _USE_MATH_DEFINES
  #define _USE_MATH_DEFINES
#endif


//--------------------------------------------------------------------------------------    
// C -----> C++ headers
//--------------------------------------------------------------------------------------    

#pragma message("#include C/C++ headers")

#include <cassert>          //     <assert.h>
#include <ccomplex>         //     <complex.h>
#include <cctype>           //     <ctype.h>
#include <cerrno>           //     <errno.h>
#include <cfenv>            //     <fenv.h>
#include <cfloat>           //     <float.h>
#include <cinttypes>        //     <inttypes.h>
#include <ciso646>          //     <iso646.h>
#include <climits>          //     <limits.h>
#include <clocale>          //     <locale.h>
#include <cmath>            //     <math.h>
#include <csetjmp>          //     <setjmp.h>
#include <csignal>          //     <signal.h>
//#include <cstdalign>      //     <stdalign.h>   missing
#include <cstdarg>          //     <stdarg.h>
#include <cstdbool>         //     <stdbool.h>    
#include <cstddef>          //     <stddef.h>
#include <cstdint>          //     <stdint.h>
#include <cstdio>           //     <stdio.h>
#include <cstdlib>          //     <stdlib.h>
#include <cstring>          //     <string.h>
#include <ctgmath>          //     <tgmath.h>     
#include <ctime>            //     <time.h>
#include <cuchar>           //     <uchar.h>     
#include <cwchar>           //     <wchar.h>
#include <cwctype>          //     <wctype.h>



//--------------------------------------------------------------------------------------    
// old C headers, including POSIX
//--------------------------------------------------------------------------------------    

#include <sys/types.h>      // needs to be in front of <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <malloc.h>
//#include <minmax.h>
#include <search.h>
#include <share.h>
#include <sys/locking.h>
#include <sys/stat.h>
#include <sys/timeb.h>
//#include <sys/types.h>    //   (alrady included, before <sys/stat.h>, as required) 
#include <sys/utime.h>



//------------------------------------------------------------------------------------
// Standard C++ headers 
//------------------------------------------------------------------------------------

#pragma message("#include standard C++ headers")

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <chrono>
#include <complex>
#include <codecvt>
#ifndef _MANAGED
  #include <condition_variable>      // not supported with /clr option
#endif
#include <deque>
#include <exception>
#include <forward_list>
#include <fstream>
#include <functional>
#ifndef _MANAGED
  #include <future>
#endif
#include <initializer_list>    
#include <iomanip>
#include <ios>
#include <iosfwd>                    
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#ifndef _MANAGED
  #include <mutex>                   // not supported with /clr option
#endif
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <set>
#ifndef _MANAGED
  #include <shared_mutex>              // not supported with /clr option
#endif
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <strstream>                 // deprecated
#include <system_error>
#ifndef _MANAGED
  #include <thread>                  // not supported with /clr option
#endif
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <vector>


//--------------------------------------------------------------------------------------------------------------------------------
// Microsoft headers for windows, MFC, GDI+, Direct2D, DirectWrite, etc. -- need to be after algorithm.h defines std::min/std::max
//--------------------------------------------------------------------------------------------------------------------------------


// ----------------- Use windows.h, if requested -----------------------------------------
//
//    note: Windows.h      is in     Program files (x86)\Windows Kits\10\Include\10.X.X.X\um 
//    note: windowsx.h     is in     Program files (x86)\Windows Kits\10\Include\10.X.X.X\shared 
//    note: SDKDDKVer.h    is in     Program Files (x86)\Windows Kits\10\Include\10.X.X.X\shared 
//    note: gdiplus.h      is in     Program files (x86)\Windows Kits\10\Include\10.X.X.X\um 
//
//    Windows.h includes:
//
//     cderr.h;                       -- if not WIN32_LEAN_AND_MEAN                                       (Common Dialog error return codes)
//     commdlg.h                       -- if not NOGDI                                                     (Common Dialog APIs)
//       prsht.h                       -- if not GUID_DEFS_ONLY
//       commdlg.inl                   -- if ISOLATION_AWARE_ENABLED != 0 -and- not RC_INVOKED 
//     dde.h                           -- if not WIN32_LEAN_AND_MEAN                                       (Dynamic Data Exchange)
//       windef.h                      -- if not already included by windows.h
//     ddeml.h;                        -- if not WIN32_LEAN_AND_MEAN                                       (Dynamic Data Exchange Management Library (DDEML))
//     dlgs.h                          -- if not WIN32_LEAN_AND_MEAN                                       (UI dialog)
//     excpt.h                         --
//     imm.h                           -- if not NOIME                                                     (Input Method Manager)
//       ime_codes.h                  
//     lzexpand.h                      -- if not WIN32_LEAN_AND_MEAN
//     mcx.h;                          -- if not NOMCX                                                     (32-bit MCX APIs -- modems?)
//     mmsystem.h                      -- if not WIN32_LEAN_AND_MEAN                                       (MultiMedia APIs)
//       mmsyscom.h                    --
//       mciapi.h                      -- if not MMNOMCI                                                   (MCI support)
//       mmiscapi.h                    --                                                                  (Installable Driver Support)
//       mmiscapi2.h                   --                                                                  (Installable Driver Support)
//       playsoundapi.h                --                                                                  (Sound support)
//       mmeapi.h                      --
//       timeapi.h                     -- if not MMNOTIMER
//       joystickapi.h                 --                                                                  (Joystick API Set contract)
//     nb30.h;                         -- if not WIN32_LEAN_AND_MEAN                                       (portable NetBIOS 3.0)
//     ole.h;                          -- if INC_OLE1 defined                                              (Object Linking and Embedding)
//     Ole2.h                          -- if INC_OLE2 defined or INC_OLE1 not defined                      (Object Linking and Embedding 2)
//       winerror.h                    --
//       objbase.h                     --
//       oleauto.h                     --
//       coml2api.h                    --
//       oleidl.h                      --
//     rpc.h ?                         -- if not WIN32_LEAN_AND_MEAN
//     sdkddver.h                      --   
//     shellapi.h;                     -- if not WIN32_LEAN_AND_MEAN                                       (SHELL.DLL functions)
//     stdarg.h                        --
//     stralign.h ?                    --
//     WinBase.h                       --                                                                  (32-bit Windows Base APIs)
//       minwinbase.h                  -- 
//       processenv.h                  --
//       fileapi.h                     --
//       debugapi.h                    --
//       utilapiset.h                  --
//       handleapi.h                   --
//       errhandlingapi.h              --
//       fibersapi.h                   --
//       namedpipeapi.h                --
//       profileapi.h                  --
//       heapapi.h                     --
//       ioapiset.h                    --
//       synchapi.h                    --
//       interlockedapi.h              --
//       processthreadsapi.h           --
//       sysinfoapi.h                  --
//       memoryapi.h                   --
//       enclaveapi.h                  --
//       threadpoollegacyapiset.h      --
//       threadpoolapiset.h            --
//       jobapi.h                      --
//       jobapi2.h                     --
//       wow64apiset.h                 --
//       securitybaseapi.h             --
//       namespaceapi.h                --
//       systemtopologyapi.h           --
//       processtopologyapi.h          --
//       securityappcontainer.h        --
//       realtimeapiset.h              --
//       winerror.h                    --
//       timezoneapi.h                 --
//       winbase.inl                   -- if ISOLATION_AWARE_ENABLED != 0
//     wincon.h                        --                                                                   (NT console subsystem)
//       wingdi.h                      -- if not NOGDI  
//       consoleapi.h                  -- if not NOAPISET
//     wincrypt.h                      -- if NOCRYPT not defined                                            (Cryptographic API)
//       bcrypt.h                      --                                                                   (CNG APIs)
//       ncrypt.h                      --
//       dpapi.h                       -- if not _WINCRYPT_NO_DPAPI 
//     windef.h                        --                                                                   (basic Windows type definitions) 
//       minwindef.h                   --
//       winnt.h                       --
//     winefs.h;                       -- if NOCRYPT not defined                                            (EFS Data -- Encrypted FileSystem?) 
//     wingdi.h;                       --
//     winnetwk.h                      --                                                                  (WINNET header file)
//       wnnc.h                        --                                                                  (network types)    
//     WinNls.h                        -- if _WIN32NLS defined                                             (NLS component header)
//       datetimeapi.h                 -- if not NOAPISET                                                  (DateTime API set)
//     winperf.h;                      -- if not WIN32_LEAN_AND_MEAN                                       (Performance Monitor)
//     winreg.h                        -- if _WIN32REG defined                                             (Windows 32-bit registry API)
//       minwindef.h                   --
//       minwinbase.h                  --
//       reason.h                      --                                                                  (InitiateSystemShutdownEX reason codes)
//     winresrc.h                      -- if RC_INVOKED 
//     winscard.h                      -- if not NOCRYPT                                                   (Smart Card susbsystem)
//       wtypes.h                      --
//       winioctl.h                    --
//       winsmcrd.h                    --
//       SCardErr.h                    -- if not SCARD_S_SUCCESS
//     winsock.h                       -- if not WIN32_LEAN_AND_MEAN                                       (Windows sockets)
//       inadr.h                       --
//     winspool.h                      -- if NOGDI not defined                                             (Print APIs)
//       prsht.h                       -- if not _WINUSER_  
//     winsvc.h;                       -- if NOSERVICE not defined                                         (Service Control Manager)
//     WinUser.h                       --                                                                  (User Procedure declarations)
//       stdarg.h                      --
//       tvout.h                       --
//       winuser.inl                   -- if ISOLATION_AWARE_ENABLED != 0  
//     winver.h                        --                                                                  (Version Management functions)
//       verrsrc.h                     --
//     winwlm.h ???????                -- if _MAC
//
//
//     Included multiple times:
//     -----------------------
//     apiset.h;                                                                                           (API namespace management)
//     apisetcconv.h                                                                                       (APISET calling conventions)
//     libloaderapi.h                                                                                      (APISET contract for API-MS-WIN-core_libraryloader-l1) 
//       apiset.h;
//       apisetcconv.h;
//       minwindef.h;
//       minwinbase.h;
//     macwin32.h ????                                                                                     not present  _MAC only
//     poppack.h;
//     pshpack1.h;
//     pshpack2.h;
//     pshpack4.h;
//     pshpack8.h;
//     pshpck16.h;
//     specstrings.h
//       no_sal2.h
//       sal.h
//       specstrings_strict.h          -- if not RC_INVOKED and SORTPP_PASS defined 
//       driverspecs.h                 --
//     winfamilyapi.h                                                                                      (API family partitioning)
//       winpackagefamily.h
//
//
//



#ifdef INCLUDE_WINDOWS_H
  #pragma message("#include windows.h")
  #ifndef NOMINMAX
    #define NOMINMAX                                  // Don't want min() or max() MACROs -- conflict with STL
    #include <SDKDDKVer.h>
    #include <Windows.h>
    #include <windowsx.h>
    #undef  NOMINMAX
  #else
    #include <SDKDDKVer.h>
    #include <Windows.h>
    #include <windowsx.h>
  #endif
#endif


// ------------------ Use MFC headers if requested ----------------------------------

#ifdef INCLUDE_MFC 
  #define _AFXDLL                                    // can't use static MFC library with /MD compile option required for /clr option
  #pragma message("#include MFC console headers")
  #ifndef NOMINMAX
    #define NOMINMAX                                 // Don't want min() or max() MACROs -- conflict with STL
    #include <afx.h>
    #include <afxwin.h>
    #include <afxext.h>
    #undef  NOMINMAX
  #else
    #include <afx.h>
    #include <afxwin.h>
    #include <afxext.h>
  #endif
#endif
                                           
// -------------------------------------------------------------

#ifndef _MANAGED
  #pragma message("#include eh.h -- structured error handling header")
  #include <eh.h>                 // Structured error handling
#endif

// -------------------------------------------------------------

#ifndef _MANAGED
  #pragma message("#include GDI+ headers")

  namespace Gdiplus
  {
    using std::min;              // Gdiplus needs min and max -- usually MACROs defined by including Windows.h
    using std::max;              // replace the MACROs with std::min and std:max in the Gdiplus namespace
  }

  #include <gdiplus.h>
#endif

//------------------------------------------------------------------------------------
// Other Windows headers 
//------------------------------------------------------------------------------------

#ifndef _MANAGED
  //#pragma message("#include DirectXMath, Direct2D, DirectWrite, WIC, etc. headers")   

  //#include <DirectXMath.h>              // DirectXMath headers
  //#include <DirectXPackedVector.h> 
  //#include <DirectXColors.h> 
  //#include <DirectXCollision.h>

  //#include <d3d12.h>                    // Direct3D -- needed for direct2d
  //#include <d2d1_1.h>                   // Direct2D headers
  //#include <d2d1_1helper.h>
  //#include <d2d1effects.h>
  //#include <d2d1effecthelpers.h>

  //#include <DXGI.h>                     // DXGI headers 

  //#include <Dshow.h>                    // DirectShow headers 

  //#ifndef DIRECTINPUT_VERSION
  //  #define DIRECTINPUT_VERSION 0x800   // prevent compiler message
  //#endif  

  //#include <dinput.h>                   // DirectInput headers               (deprecated)

  //#include <dsound.h>                   // DirectSound headers               (deprecated)
  //#include <Dmusicc.h>                  // DirectMusic headers               (deprecated)
  //#include <dwrite_1.h>                 // DirectWrite headers
  //#include <dcomp.h>                    // DirectComposition headers (COM)

  //#include <audioclient.h>              // WASAPI headers
  //#include <audiopolicy.h>

  //#include <wincodec.h>                 // WIC headers  (COM)
  //#include <wincodecsdk.h> 

  //#include <comdef.h>                   // COM headers  
  //#include <amp.h>                      // C++AMP headers  

  //#include <commctrl.h>                 // windows common controls

  //#include <agents.h>                   // concurrency runtime 
  //#include <concrt.h>
  //#include <concrtrm.h>
  //#include <concurrent_queue.h> 
  //#include <concurrent_priority_queue.h> 
  //#include <concurrent_unordered_map.h> 
  //#include <concurrent_unordered_set.h>
  //#include <concurrent_vector.h>
  //#include <ppl.h>
  //#include <pplcancellation_token.h>
  //#include <pplconcrt.h>              // trouble ??
  //#include <pplinterface.h>
  //#include <ppltasks.h>

  //#include <process.h>                  // legacy multi-threading support  

  //#include <omp.h>                      // Open MP

  #include <conio.h>                      // old console I/O 

#endif


//------------------------------------------------------------------------------------
// Other C/C++ headers (Microsoft)
//------------------------------------------------------------------------------------

//#include <allocators>
//#include <filesystem>
//#include <hash_map>               deprecated -- gives compiler warning
//#include <hash_set>               deprecated -- gives compiler warning
//#include <mbstring.h>
//#include <tchar.h>
//#include <xcomplex>               gives compile errors 
//#include <xdebug>                 does not exist
//#include <xfacet>
//#include <xfunctional>
//#include <xhash>
//#include <xiosbase>
//#include <xlocale>
//#include <xlocbuf>
//#include <xlocinfo>
//#include <xlocmon>
//#include <xlocnum>
//#include <xloctime>
//#include <xmemory>
//#include <xmemory0>
//#include <xrefwrap>               does not exist
//#include <xstddef>
//#include <xstring>
//#include <xtr1common>
//#include <xtree>
//#include <xutility>
//#include <xxatomic>               gives compile errors
//#include <xtree>     



//------------------------------------------------------------------------------------
// Boost and fftw headers   
//------------------------------------------------------------------------------------


#pragma message("#include boost and fftw headers")

#include <boost/algorithm/string.hpp>
#include <boost/endian/arithmetic.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/format.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include <fftw3.h>



#pragma message("--------------------------------------------------------------------------------")  

//-----------------------------    
// Display configuration MACROs
//----------------------------- 

#define M_str2(x) #x
#define M_str1(x) M_str2(x)

////////////////////////

#ifdef _ATL_VER
#pragma message("---- _ATL_VER                  = " M_str1(_ATL_VER))
#else
#pragma message("---- _ATL_VER                  - (not defined)")
#endif

#ifdef _AMD64_
#pragma message("---- _AMD64_                   - ****Defined****")
#else                                       
#pragma message("---- _AMD64_                   - (not defined)")
#endif  

#ifdef _ARM64_
#pragma message("---- _ARM64_                   - ****Defined****")
#else                                       
#pragma message("---- _ARM64_                   - (not defined)")
#endif 

#ifdef _CHAR_UNSIGNED
#pragma message("---- _CHAR_UNSIGNED            - ****Defined****")
#else                                       
#pragma message("---- _CHAR_UNSIGNED            - (not defined)")
#endif                                      
                                            
#ifdef _CLR_VER                             
#pragma message("---- _CLR_VER                  = " M_str1(_CLR_VER))
#else                                       
#pragma message("---- _CLR_VER                  - (not defined)")
#endif                                      
                                            
#ifdef _CPPRTTI                             
#pragma message("---- _CPPRTTI                  - ****Defined****")
#else                                       
#pragma message("---- _CPPRTTI                  - (not defined)")
#endif                                      
                                            
#ifdef _CPPUNWIND                           
#pragma message("---- _CPPUNWIND                - ****Defined****")
#else                                       
#pragma message("---- _CPPUNWIND                - (not defined)")
#endif                                      
                                            
#ifdef _DEBUG                               
#pragma message("---- _DEBUG                    - ****Defined****")
#else                                       
#pragma message("---- _DEBUG                    - (not defined)")
#endif                                      

#ifdef DIRECTINPUT_VERSION                             
#pragma message("---- DIRECTINPUT_VERSION       = " M_str1(DIRECTINPUT_VERSION))
#else                                       
#pragma message("---- DIRECTINPUT_VERSION       - (not defined)")
#endif

#ifdef _DLL                                 
#pragma message("---- _DLL                      - ****Defined****")
#else                                       
#pragma message("---- _DLL                      - (not defined)")
#endif                                      
 
#ifdef _GDI32_                                 
#pragma message("---- _GDI32_                   - ****Defined****")
#else                                       
#pragma message("---- _GDI32_                   - (not defined)")
#endif                       

#ifdef INC_OLE1                             
#pragma message("---- INC_OLE1                  - ****Defined****")
#else                                       
#pragma message("---- INC_OLE1                  - (not defined)")
#endif                                      
                                            
#ifdef INC_OLE2                             
#pragma message("---- INC_OLE2                  - ****Defined****")
#else                                       
#pragma message("---- INC_OLE2                  - (not defined)")
#endif 

#ifdef _INTEGRAL_MAX_BITS
#pragma message("---- _INTEGRAL_MAX_BITS        = " M_str1(_INTEGRAL_MAX_BITS))
#else
#pragma message("---- _INTEGRAL_MAX_BITS        - (not defined)")
#endif 

#ifdef ISOLATION_AWARE_ENABLED
#pragma message("---- ISOLATION_AWARE_ENABLED   = " M_str1(ISOLATION_AWARE_ENABLED))
#else
#pragma message("---- ISOLATION_AWARE_ENABLED   - (not defined)")
#endif 

#ifdef _M_X64
#pragma message("---- _M_X64                    - ****Defined****")
#else                                     
#pragma message("---- _M_X64                    - (not defined)")
#endif                                    
                                          
#ifdef _MAC                               
#pragma message("---- _MAC                      = " M_str1(_MANAGED))
#else                                     
#pragma message("---- _MAC                      - (not defined)")
#endif                                    
                                          
#ifdef _MANAGED                           
#pragma message("---- _MANAGED                  = " M_str1(_MANAGED))
#else                                     
#pragma message("---- _MANAGED                  - (not defined)")
#endif                                    
                                          
#ifdef _MFC_VER                           
#pragma message("---- _MFC_VER                  = " M_str1(_MFC_VER))
#else                                     
#pragma message("---- _MFC_VER                  - (not defined)")
#endif                                    
                                          
#ifdef _MSC_BUILD                         
#pragma message("---- _MSC_BUILD                = " M_str1(_MSC_BUILD))
#else
#pragma message("---- _MSC_BUILD                - (not defined)")
#endif                                      
                                            
#ifdef _MSC_EXTENSIONS                      
#pragma message("---- _MSC_EXTENSIONS           = " M_str1(_MSC_EXTENSIONS))
#else                                       
#pragma message("---- _MSC_EXTENSIONS           - (not defined)")
#endif                                      
                                            
#ifdef _MSC_FULL_VER                        
#pragma message("---- _MSC_FULL_VER             = " M_str1(_MSC_FULL_VER))
#else                                       
#pragma message("---- _MSC_FULL_VER             - (not defined)")
#endif                                      
                                            
#ifdef _MSC_VER                             
#pragma message("---- _MSC_VER                  = " M_str1(_MSC_VER))
#else                                       
#pragma message("---- _MSC_VER                  - (not defined)")
#endif                                      
                                            
#ifdef _MT                                  
#pragma message("---- _MT                       = " M_str1(_MT))
#else                                       
#pragma message("---- _MT                       - (not defined)")
#endif                                      
 
#ifdef NOAPISET                              
#pragma message("---- NOAPISET                  - ****Defined****")
#else                                       
#pragma message("---- NOAPISET                  - (not defined)")
#endif 

#ifdef NOCRYPT                              
#pragma message("---- NOCRYPT                   - ****Defined****")
#else                                       
#pragma message("---- NOCRYPT                   - (not defined)")
#endif                                      

#ifdef NOGDI
#pragma message("---- NOGDI                     - ****Defined****")
#else                                   
#pragma message("---- NOGDI                     - (not defined)")
#endif                                  
                                        
#ifdef NOIME                            
#pragma message("---- NOIME                     - ****Defined****")
#else                                   
#pragma message("---- NOIME                     - (not defined)")
#endif                                  
                                        
#ifdef NOMCX                            
#pragma message("---- NOMCX                     - ****Defined****")
#else                                   
#pragma message("---- NOMCX                     - (not defined)")
#endif                                  
    
#ifdef NOMINMAX                        
#pragma message("---- NOMINMAX                  - ****Defined****")
#else                                   
#pragma message("---- NOMINMAX                  - (not defined)")
#endif 

#ifdef NOSERVICE                        
#pragma message("---- NOSERVICE                 - ****Defined****")
#else                                   
#pragma message("---- NOSERVICE                 - (not defined)")
#endif                                  
                                        
#ifdef NTDDI_VERSION                    
#pragma message("---- NTDDI_VERSION             = " M_str1(NTDDI_VERSION))
#else                                   
#pragma message("---- NTDDI_VERSION             - (not defined)")
#endif                                  

#ifdef _NTOS_                          
#pragma message("---- _NTOS_                    - ****Defined****")
#else                                   
#pragma message("---- _NTOS_                    - (not defined)")
#endif  

#ifdef RC_INVOKED                          
#pragma message("---- RC_INVOKED                - ****Defined****")
#else                                   
#pragma message("---- RC_INVOKED                - (not defined)")
#endif                                  

#ifdef _SPOOL32_                                 
#pragma message("---- _SPOOL32_                 - ****Defined****")
#else                                       
#pragma message("---- _SPOOL32_                 - (not defined)")
#endif 

#ifdef UNICODE
#pragma message("---- UNICODE                   - ****Defined****")
#else                                        
#pragma message("---- UNICODE                   - (not defined)")
#endif                                       
                                             
#ifdef _UNICODE                                
#pragma message("---- _UNICODE                  - ****Defined****")
#else                                        
#pragma message("---- _UNICODE                  - (not defined)")
#endif                                       
                                             
#ifdef _USE_MATH_DEFINES                     
#pragma message("---- _USE_MATH_DEFINES         - ****Defined****")
#else                                        
#pragma message("---- _USE_MATH_DEFINES         - (not defined)")
#endif                                       
                                             
#ifdef WIN16                                 
#pragma message("---- WIN16                     - ****Defined****")
#else                                        
#pragma message("---- WIN16                     - (not defined)")
#endif                                       
                                             
#ifdef WIN32                                 
#pragma message("---- WIN32                     - ****Defined****")
#else                                        
#pragma message("---- WIN32                     - (not defined)")
#endif  

#ifdef WIN32_LEAN_AND_MEAN
#pragma message("---- WIN32_LEAN_AND_MEAN       - ****Defined****")
#else                                         
#pragma message("---- WIN32_LEAN_AND_MEAN       - (not defined)")
#endif                                        
                                              
#ifdef _WIN32NLS                              
#pragma message("---- _WIN32NLS                 - ****Defined****")
#else                                         
#pragma message("---- _WIN32NLS                 - (not defined)")
#endif                                        
                                              
#ifdef _WIN32REG                              
#pragma message("---- _WIN32REG                 - ****Defined****")
#else                                         
#pragma message("---- _WIN32REG                 - (not defined)")
#endif                                        
                                              
#ifdef _WIN32_WINNT                           
#pragma message("---- _WIN32_WINNT              = " M_str1(_WIN32_WINNT))
#else                                         
#pragma message("---- _WIN32_WINNT              - (not defined)")
#endif                                        
                                              
#ifdef _WIN64                                 
#pragma message("---- _WIN64                    - ****Defined****")
#else                                         
#pragma message("---- _WIN64                    - (not defined)")
#endif                                        

#ifdef WINAPI_FAMILY_DESKTOP_APP                                 
#pragma message("---- WINAPI_FAMILY_DESKTOP_APP - ****defined****")
#else                                         
#pragma message("---- WINAPI_FAMILY_DESKTOP_APP - (not defined)")
#endif 

#ifdef WINAPI_FAMILY_ONECORE_APP                                 
#pragma message("---- WINAPI_FAMILY_ONECORE_APP - ****defined****")
#else                                         
#pragma message("---- WINAPI_FAMILY_ONECORE_APP - (not defined)")
#endif 

#ifdef WINAPI_FAMILY_PC_APP                                 
#pragma message("---- WINAPI_FAMILY_PC_APP      - ****defined****")
#else                                         
#pragma message("---- WINAPI_FAMILY_PC_APP      - (not defined)")
#endif 

#ifdef WINAPI_FAMILY_PHONE_APP                                 
#pragma message("---- WINAPI_FAMILY_PHONE_APP   - ****defined****")
#else                                         
#pragma message("---- WINAPI_FAMILY_PHONE_APP   - (not defined)")
#endif 

#ifdef WINAPI_FAMILY_SERVER                                 
#pragma message("---- WINAPI_FAMILY_SERVER      - ****defined****")
#else                                         
#pragma message("---- WINAPI_FAMILY_SERVER      - (not defined)")
#endif 

#ifdef WINAPI_FAMILY_SYSTEM                                 
#pragma message("---- WINAPI_FAMILY_SYSTEM      - ****defined****")
#else                                         
#pragma message("---- WINAPI_FAMILY_SYSTEM      - (not defined)")
#endif 

#ifdef WINVER                                 
#pragma message("---- WINVER                    = " M_str1(WINVER))
#else                                         
#pragma message("---- WINVER                    - (not defined)")
#endif                                        

#ifdef _X86_                                 
#pragma message("---- _X86_                     - ****Defined****")
#else                                         
#pragma message("---- _X86_                     - (not defined)")
#endif 

#pragma message("--------------------------------------------------------------------------------")  


#pragma message("conio.h             " M_str1(_INC_CONIO<<<<        )) 
#pragma message("ole.h               " M_str1(_INC_OLE<<<<          )) 
#pragma message("Ole2.h              " M_str1(_OLE2_H_<<<<          )) 
#pragma message("Windows.h           " M_str1(_WINDOWS_<<<<         )) 
#pragma message("windowsx.h          " M_str1(_INC_WINDOWSX<<<<     )) 
#pragma message("winefs.h            " M_str1(__WINEFS_H__<<<<      )) 
#pragma message("wingdi.h            " M_str1(_WINGDI_<<<<          )) 
#pragma message("winioctl.h          " M_str1(_WINIOCTL_<<<<        )) 
#pragma message("winnls.h            " M_str1(_WINNLS_<<<<          )) 
#pragma message("winnt.h             " M_str1(_WINNT_<<<<           )) 
#pragma message("winperf.h           " M_str1(_WINPERF_<<<<         )) 
#pragma message("winreg.h            " M_str1(_WINREG_<<<<          )) 
#pragma message("winscard.h          " M_str1(_WINSCARD_H_<<<<      ))
#pragma message("winscard.h          " M_str1(_WINSPOOL_<<<<        ))
#pragma message("winsvc.h            " M_str1(_WINSVC_<<<<          ))
#pragma message("winuser.h           " M_str1(_WINUSER_<<<<         ))



#pragma message("--------------------------------------------------------------------------------") 

//------------------------------------------------------------------------------------
// Local headers
//------------------------------------------------------------------------------------

#pragma message("#include large stable local headers")

#include "h__utf32.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////