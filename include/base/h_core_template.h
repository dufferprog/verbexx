// h_core_template.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ================= 
////            h_core_template.h -- define template items in Buffer_C, so all compiles have the source code available  
////            =================
////     
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h__types.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////               Miscellaneous convenience template functions 
////               ============================================
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// convenience functions using M_copyswap_xx MACROs
///////////////////////////////////////////////////////

template<typename T> 
T bigend2(const void *p)
{
    T x;
    M_copyswap2_p(&x, p)
    return x;
}

template<typename T> 
T bigend4(const void *p)
{
    T x;
    M_copyswap4_p(&x, p)
    return x;
}

template<typename T> 
T bigend8(const void *p)
{
    T x;
    M_copyswap8_p(&x, p)
    return x;
}


/////////////////////////////////////////////////
//   F_max(), F_min(), F_lim(), F_roundup()
/////////////////////////////////////////////////


// ??????????????????? should redo these with variadic templates ????????????????????????????

/////////// F_max(x,y) ///////////

template<typename T> inline  const T& 
F_max(const T& p1, const T& p2)         // use if max() is usurped via MFC MACRO definition (can use (max)(x,y), too)
{
    return (p1 > p2) ? p1 : p2;  
}

////////// F_min(x,y) ////////////////

template<typename T> inline const T& 
F_min(const T& p1, const T& p2)         // use if min() is usurped by MFC MACRO definition (can use (min)(x,y), too)
{
    return (p1 < p2) ? p1 : p2;  
}

///////////// F_max(x,y,z) ////////////////

template<typename T> inline const T& 
F_max( const T& p1
     , const T& p2
     , const T& p3
     )         
{
    return (p1 > p2) ? ( (p3 > p1) ? p3 : p1 )
                     : ( (p3 > p2) ? p3 : p2 )
                     ;
}

/////////// F_max(w,x,y,z) //////////////////

template<typename T> inline const T& 
F_max( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     )         
{
    return F_max(F_max(p1, p2), F_max(p3, p4));                     
}


/////////// F_max(v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_max( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     )         
{
    return F_max(F_max(p1, p2, p3), F_max(p4, p5));                     
}

/////////// F_max(u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_max( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     )         
{
    return F_max(F_max(p1, p2, p3), F_max(p4, p5, p6));                     
}

/////////// F_min(x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     )         
{
    return (p1 < p2) ? ( (p3 < p1) ? p3 : p1 )
                     : ( (p3 < p2) ? p3 : p2 )
                     ;
}


/////////// F_min(w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     )         
{
    return F_min(F_min(p1, p2), F_min(p3, p4));                     
}


/////////// F_min(v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5));                     
}

/////////// F_min(u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5, p6));                     
}

/////////// F_min(t,u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     , const T& p7
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5, p6), p7);                     
}

/////////// F_min(s,t,u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     , const T& p7
     , const T& p8
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5, p6), F_min(p7, p8));                     
}

/////////// F_min(r,s,t,u,v,w,x,y,z) //////////////////

template<typename T> inline const T& 
F_min( const T& p1
     , const T& p2
     , const T& p3
     , const T& p4
     , const T& p5
     , const T& p6
     , const T& p7
     , const T& p8
     , const T& p9
     )         
{
    return F_min(F_min(p1, p2, p3), F_min(p4, p5, p6), F_min(p7, p8, p9));                     
}



///////////// F_lim(lo,x,hi) //////////

template<typename T> inline const T& 
F_lim(const T& l, const T& p, const T& h)
{
    return (p < l) ? l
                   : ( (p > h) ? h : p )
                   ;  
}


///////////// F_roundup(x, r) ////////// (only works for integer types)

template<typename T> inline T 
F_roundup(const T& x, const T& r)
{
    return ((x + r - 1) / r) * r; 
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////               STL container-oriented convenience template functions 
////               =====================================================
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
//
// multimap_at() -- return n-th value in a multimap (exception, if item is not present)
//
////////////////////////////////////////////////////////////////////////////////////

template<typename T1, typename T2>
T2 multimap_at(const std::multimap<T1, T2>& mm, const T1& key, uint64_t n = 0)
{
    // simple case when n = 0

    if (n == 0)
    {
        auto it = mm.find(key);

        if (it == mm.end())
        {
            M_out_emsg(L"multimap_at() -- passed-in key was not present in multimap");
            M_throw(    "multimap_at() -- passed-in key was not present in multimap")
        }
        else
            return it->second; 
    }


    // general case -- cause exception if n-th key is not present in the multimap

    auto ct = mm.count(key); 

    if (ct <= n)
    {
        M_out_emsg(L"multimap_at() -- passed-in key[%d] was not present in multimap") % n;
        M_throw_v(  "multimap_at() -- passed-in key[%d] was not present in multimap") % n));
    }


    // find position of n-th element for passed-in key 

    auto it = mm.find(key);

    if (n > 0)  
        for (auto i = 0; i < n; i++) it++;

    return it->second;    // return value for n-th element in multi-map  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// multimap_deref() -- return n-th key/value pair in a multimap (exception, if 'n' is past end of multimap)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T1, typename T2>
std::pair<T1, T2> multimap_deref(const std::multimap<T1, T2>& mm, uint64_t n)
{
    // make sure 'n' is not past end of multimap

    if (n >= mm.size())
    {
         M_out_emsg(L"multimap_deref() -- passed-in index (n=%d) is past end of multimap (size()=%d)") % n % mm.size();
         M_throw(    "multimap_deref() -- passed-in index is past end of multimap")       
    }


    //   use incrementation to get iterator that points to n-th item (0-based) 

    auto it = mm.cbegin(); 

    if (n > 0) 
        for (auto i = 0; i < n; i++) it++;

    return *it;        
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// is_in_vector() -- return true, if passed-in item is found in passed-in vector
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool is_in_vector(const T item, const std::vector<T>& vect)
{
    return std::find(vect.begin(), vect.end(), item) != vect.end();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////
////////
////////
////////
////////    ============
////////    Buffer Class -- template functions that need to be included in util.h
////////    ============
////////
////////
////////
////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// default constructor is defaulted




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    copy constructor
//    ---------------- 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
buffer_C<T>::buffer_C(const buffer_C<T>& from)
    : p              { nullptr          }  
    , sz             { from.sz          } 
    , sz1            { from.sz1         }
    , copy_ct        { from.copy_ct + 1U} 
{
#ifdef M_OUT_WIDE
    M__(M_out(L"<<COPY>> <<COPY>> <<COPY>> <<COPY>> <<COPY>> <<COPY>> <<COPY>> <<COPY>> buffer_C(const buffer_C&) called -- copy constructor -- sz=%s  sz1=%s  copy_ct=%u\"") % M_sep(sz) % M_sep(sz1) % copy_ct;)
#else
    M__(M_out( "<<COPY>> <<COPY>> <<COPY>> <<COPY>> <<COPY>> <<COPY>> <<COPY>> <<COPY>> buffer_C(const buffer_C&) called -- copy constructor -- sz=%s  sz1=%s  copy_ct=%u\"") % M_sep(sz) % M_sep(sz1) % copy_ct;)
#endif

    if (sz1 > 0)
    {
       this->alloc1(sz1);
       memcpy((void *)p1, (void *)(from.p1), (size_t)sz1);
    }

    return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    move constructor
//    ---------------- 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
buffer_C<T>::buffer_C(buffer_C<T>&& from)
    : p              { from.p         } 
    , p1             { from.p1        }
    , sz             { from.sz        } 
    , sz1            { from.sz1       } 
    , copy_ct        { from.copy_ct   } 
{
#ifdef M_OUT_WIDE
    M__(M_out(L"<<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> buffer_C(buffer_C&&) called -- move constructor -- sz=%s  sz1=%s copy_ct=%u\"") % M_sep(sz) % M_sep(sz1) % copy_ct;)
#else
    M__(M_out( "<<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> <<MOVE>> buffer_C(buffer_C&&) called -- move constructor -- sz=%s  sz1=%s copy_ct=%u\"") % M_sep(sz) % M_sep(sz1) % copy_ct;)
#endif

    from.sz  = 0ULL;              // data area in from has been moved out 
    from.sz1 = 0ULL;              // data area in from has been moved out 
    from.p   = nullptr;           // data area is now anchored off this
    from.p1  = nullptr; 

    return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  uint64_t constructor -- allocate buffer with size "sz" -- in units of <T>
//  --------------------  
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
buffer_C<T>::buffer_C(uint64_t sz)      
//  : p      {nullptr}                 // defaulted in class declaration -- need to zero these first before callin alloc() below
//  : p1     {nullptr}                 // defaulted in class declaration -- need to zero these first before callin alloc() below
//  , sz     {0ULL} 
{
    M__(M_out(L"buffer_C(%s) called -- constructor") % M_sep(sz);)
    
    this->alloc(sz);
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  copy assignment -- copy contents of one buffer to another
//  ---------------  
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
buffer_C<T>& buffer_C<T>::operator=(const buffer_C<T>& from)
{
#ifdef M_OUT_WIDE
    M__(M_out(L"<< =COPY= >> << =COPY= >>  buffer_C& operator=(const buffer_C&) called -- copy assignment -- sz=%s  sz1=%s  copy_ct=%u\"") % M_sep(sz) % M_sep(sz1) % copy_ct;)
#else
    M__(M_out( "<< =COPY= >> << =COPY= >>  buffer_C& operator=(const buffer_C&) called -- copy assignment -- sz=%s  sz1=%s  copy_ct=%u\"") % M_sep(sz) % M_sep(sz1) % copy_ct;)
#endif

    //  return immediately, if self-assignment

    if (this == &from)
        return *this;    


    // copy data (if any) from from to this

    this->alloc1(from.sz1);                                     // get rid of current buffer and replace with one large enough to hold data to be copied  -- should set p, p1, sz, sz1 member fields

    if (sz1 > 0)                                                // sz1 should be same as from.sz1 -- p, p1 should not be nullptr if sz/sz1 > 0 
        memcpy((void *)p1, (void *)(from.p1), (size_t)sz1);
    
    return *this; 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  move assignment -- move contents of one buffer to another ("from" buffer ends up empty)
//  ---------------  
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
buffer_C<T>& buffer_C<T>::operator=(buffer_C<T>&& from)
{
#ifdef M_OUT_WIDE
    M__(M_out(L"<< =MOVE= >>   << =MOVE= >>   buffer_C& operator=(buffer_C&&) called -- move assignment -- sz=%s  sz1=%s  copy_ct=%u\"") % M_sep(sz) % M_sep(sz1) % copy_ct;)
#else
    M__(M_out( "<< =MOVE= >>   << =MOVE= >>   buffer_C& operator=(buffer_C&&) called -- move assignment -- sz=%s  sz1=%s  copy_ct=%u\"") % M_sep(sz) % M_sep(sz1) % copy_ct;)
#endif
        
    //  return immediately, if self-assignment

    if (this == &from)
        return *this    
   

    // replace this's  buffer pointers and counts by from's -- data automatically moves with the pointer/counter update  

    this->free();                                               // get rid of current buffer before switching pointers to from's buffer

    p         =   from.p       ;   
    p1        =   from.p1      ;  
    sz        =   from.sz      ;   
    sz1       =   from.sz1     ;   
    copy_ct   =   from.copy_ct ;   
    

    // clear out fields in from -- from's old buffer is now anchored in this

    from.sz  = 0ULL;              // data area in from has been moved out 
    from.sz1 = 0ULL;              // data area in from has been moved out 
    from.p   = nullptr;           // data area is now anchored off this
    from.p1  = nullptr; 
                        
    return *this; 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  destructor -- free buffer, etc. 
//  ----------  
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
buffer_C<T>::~buffer_C()      
{
    M__(M_out(L"~buffer_c() called -- destructor -- sz=%s  sz1=%s  copy_ct=%u") % M_sep(sz) % M_sep(sz1) % copy_ct;)
    
    this->free();
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  free() -- free buffer 
//  ------  
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void buffer_C<T>::free()      
{
    M__(M_out(L"buffer_C::free() called -- sz=%s  sz1=%s") % M_sep(sz) % M_sep(sz1);)
    
    if (p1 != nullptr)
       delete[] p1; 
    
    sz  = 0; 
    sz1 = 0; 
    p   = nullptr; 
    p1  = nullptr;

    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  alloc() -- allocate buffer (free any existing buffer, first) 
//  -------  
//
//  note: input length parm is always in units of <T> 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void buffer_C<T>::alloc(uint64_t sz_in)      
{
    M__(M_out(L"buffer_C::alloc(%s) called -- sz=%s  sz1=%s") % M_sep(sz_in) % M_sep(sz) % M_sep(sz1);)
    
    this->free();

    if (sz_in > 0)
    {
        p1  = new uint8_t[sz_in * sizeof (T)];
        p  = (T *)p1; 
        sz  = sz_in; 
        sz1 = sz * sizeof (T);
    }
    
    return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  alloc1() -- allocate buffer (free any existing buffer, first) 
//  -------  
//
//  note: input length parm is always in bytes
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void buffer_C<T>::alloc1(uint64_t sz1_in)      
{
    M__(M_out("buffer_C::alloc1(%s) called -- sz=%s  sz1=%s") % M_sep(sz1_in) % M_sep(sz) % M_sep(sz1);)
    
    this->free();

    if (sz1_in > 0)
    {
        p1   = new uint8_t[sz1_in];
        p    = (T *)p1; 
        sz1  = sz1_in; 
        sz   = sz1 / sizeof (T);          // round sz down, as required
    }
    
    return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  realloc() -- re-allocate buffer, if new requested size is larger than existing size 
//  ---------  
//
//  note: input length parm is always in units of <T> 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void buffer_C<T>::realloc(uint64_t sz_in)      
{
    M__(M_out("buffer_C::realloc(%s) called -- sz=%s  sz1=%s") % M_sep(sz_in) % M_sep(sz) % M_sep(sz1);)
    
    if (sz_in > sz)
       this->alloc(sz_in);        
    
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  realloc1() -- re-allocate buffer, if new requested size is larger than existing size 
//  ----------  
//
//  note: input length parm is always in bytes 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void buffer_C<T>::realloc1(uint64_t sz1_in)      
{
    M__(M_out("buffer_C::realloc1(%s) called -- sz=%s  sz1=%s") % M_sep(sz1_in) % M_sep(sz) % M_sep(sz1);)
    
    if (sz1_in > sz1)
       this->alloc1(sz1_in);        
    
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  clear() -- zero out entire buffer area (if any) 
//  -------  
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void buffer_C<T>::clear()      
{
    M__(M_out("buffer_C::clear() called -- sz=%s  sz1=%s") % M_sep(sz) % M_sep(sz1);)
    
    if ( (sz1 > 0) && (p1 != nullptr) )
       ::memset((void *)p1, (char)0, (size_t)sz1);          
    
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  assign1() -- assign all bytes in buffer to same value 
//  ---------  
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void buffer_C<T>::assign1(char ch)      
{
    M__(M_out("buffer_C::assign1(%02X) called -- sz=%s  sz1=%s") % (uint32_t)ch % M_sep(sz) % M_sep(sz1);)
    
    if ( (sz1 > 0) && (p1 != nullptr) )
       ::memset((void *)p1, ch, (size_t)sz1);          
    
    return;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////