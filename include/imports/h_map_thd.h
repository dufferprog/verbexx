﻿// h_map_thd.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ===========
////            h_map_thd.h -- thread parm structure -- thread-oriented functions
////            ===========
////
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_map_types.h"


//---------------------------------------------------------------------------------------------------------------------
//     Thread parms structure 
//---------------------------------------------------------------------------------------------------------------------

struct thp_S
{
    int32_t         thread_n;
 

    // row/col slice info

    rc_T            e_r0;
    rc_T            e_rm;
    rc_T            e_c0;
    rc_T            e_cm;


    // other input parms -- routine-dependent

    int32_t         p32;
    int64_t         p64;
 

    // return data area -- routine-dependent 

    int32_t         r32;
}; 

        
// thread parms array

struct thpa_S
{
    int32_t n_threads;
    thp_S   thp[MAX_SLICES];  //// should become a vector<thp_S>
}; 



//---------------------------------------------------------------------------------------------------------------------
//     Thread-oriented functions
//---------------------------------------------------------------------------------------------------------------------

void     slice_row(    thpa_S&, rc_T, rc_T, rc_T, rc_T, int32_t, bool, const mapparm_S&);
void     slice_col(    thpa_S&, rc_T, rc_T, rc_T, rc_T, int32_t, bool, const mapparm_S&);
void     slice_row_col(thpa_S&, rc_T, rc_T, rc_T, rc_T, int32_t, bool, const mapparm_S&);    
