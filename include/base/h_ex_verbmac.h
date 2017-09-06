// h_ex_verbmac.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////            ============== 
////            h_ex_verbmac.h -- Public ex-oriented MACROs for setting up pre-defind verbs 
////            ==============
////     
////   
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////  High-level MACROs for setting up verb keyword parms
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
///// MACROs to add keyword match sets to plist 
////////////////////////////////////////////////////////////////

// right-side match pair

#define M_vt_right_match_pair(v, n1, n2)                 \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_r_match_set(v, ms)                          \
}


// right-side match 3-way

#define M_vt_right_match_3way(v, n1, n2, n3)             \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_r_match_set(v, ms)                          \
}


// right-side match 4-way

#define M_vt_right_match_4way(v, n1, n2, n3, n4)         \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_match_kw(ms, n4)                            \
    M_vt_add_r_match_set(v, ms)                          \
}


// right-side match 5-way

#define M_vt_right_match_5way(v, n1, n2, n3, n4, n5)     \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_match_kw(ms, n4)                            \
    M_vt_add_match_kw(ms, n5)                            \
    M_vt_add_r_match_set(v, ms)                          \
}

///////////////////////////////////////////////////////////

// left-side match pair

#define M_vt_left_match_pair(v, n1, n2)                  \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_l_match_set(v, ms)                          \
}


// left-side match 3-way

#define M_vt_left_match_3way(v, n1, n2, n3)              \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_l_match_set(v, ms)                          \
}


// left-side match 4-way

#define M_vt_left_match_4way(v, n1, n2, n3, n4)          \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_match_kw(ms, n4)                            \
    M_vt_add_l_match_set(v, ms)                          \
}


// left-side match 5-way

#define M_vt_left_match_5way(v, n1, n2, n3, n4, n5)      \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_match_kw(ms, n4)                            \
    M_vt_add_match_kw(ms, n5)                            \
    M_vt_add_l_match_set(v, ms)                          \
}

///////////////////////////////////////////////////////////

// nested match pair

#define M_vt_nest_match_pair(pl, n1, n2)                 \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_nested_match_set(pl, ms)                    \
}

// nested match 3-way

#define M_vt_nest_match_3way(v, n1, n2, n3)              \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_nested_match_set(v, ms)                     \
}


// nested match 4-way

#define M_vt_nest_match_4way(v, n1, n2, n3, n4)          \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_match_kw(ms, n4)                            \
    M_vt_add_nested_match_set(v, ms)                     \
}


// nested match 5-way

#define M_vt_nest_match_5way(v, n1, n2, n3, n4, n5)      \
{                                                        \
    M_vt_match_set(ms)                                   \
    M_vt_add_match_kw(ms, n1)                            \
    M_vt_add_match_kw(ms, n2)                            \
    M_vt_add_match_kw(ms, n3)                            \
    M_vt_add_match_kw(ms, n4)                            \
    M_vt_add_match_kw(ms, n5)                            \
    M_vt_add_nested_match_set(v, ms)                     \
}

////////////////////////////////////////////////////////////////
///// MACROs to add keyword conflict sets to plist 
////////////////////////////////////////////////////////////////


// right-side conflict pair

#define M_vt_right_conflict_pair(v, n1, n2)  \
{                                            \
    M_vt_conflict_set(cs)                    \
    M_vt_add_conflict_kw(cs, n1)             \
    M_vt_add_conflict_kw(cs, n2)             \
    M_vt_add_r_conflict_set(v, cs)           \
}

#define M_vt_right_conflict_2way(v, n1, n2)  \
{                                            \
    M_vt_conflict_set(cs)                    \
    M_vt_add_conflict_kw(cs, n1)             \
    M_vt_add_conflict_kw(cs, n2)             \
    M_vt_add_r_conflict_set(v, cs)           \
}

// 3-way right-side conflict

#define M_vt_right_conflict_3way(v, n1, n2, n3) \
{                                               \
    M_vt_conflict_set(cs)                       \
    M_vt_add_conflict_kw(cs, n1)                \
    M_vt_add_conflict_kw(cs, n2)                \
    M_vt_add_conflict_kw(cs, n3)                \
    M_vt_add_r_conflict_set(v, cs)              \
}


// 4-way right-side conflict

#define M_vt_right_conflict_4way(v, n1, n2, n3, n4) \
{                                                   \
    M_vt_conflict_set(cs)                           \
    M_vt_add_conflict_kw(cs, n1)                    \
    M_vt_add_conflict_kw(cs, n2)                    \
    M_vt_add_conflict_kw(cs, n3)                    \
    M_vt_add_conflict_kw(cs, n4)                    \
    M_vt_add_r_conflict_set(v, cs)                  \
}


// 5-way right-side conflict

#define M_vt_right_conflict_5way(v, n1, n2, n3, n4, n5) \
{                                                       \
    M_vt_conflict_set(cs)                               \
    M_vt_add_conflict_kw(cs, n1)                        \
    M_vt_add_conflict_kw(cs, n2)                        \
    M_vt_add_conflict_kw(cs, n3)                        \
    M_vt_add_conflict_kw(cs, n4)                        \
    M_vt_add_conflict_kw(cs, n5)                        \
    M_vt_add_r_conflict_set(v, cs)                      \
}


// 6-way right-side conflict

#define M_vt_right_conflict_6way(v, n1, n2, n3, n4, n5, n6) \
{                                                           \
    M_vt_conflict_set(cs)                                   \
    M_vt_add_conflict_kw(cs, n1)                            \
    M_vt_add_conflict_kw(cs, n2)                            \
    M_vt_add_conflict_kw(cs, n3)                            \
    M_vt_add_conflict_kw(cs, n4)                            \
    M_vt_add_conflict_kw(cs, n5)                            \
    M_vt_add_conflict_kw(cs, n6)                            \
    M_vt_add_r_conflict_set(v, cs)                          \
}


// 7-way right-side conflict

#define M_vt_right_conflict_7way(v, n1, n2, n3, n4, n5, n6, n7) \
{                                                               \
    M_vt_conflict_set(cs)                                       \
    M_vt_add_conflict_kw(cs, n1)                                \
    M_vt_add_conflict_kw(cs, n2)                                \
    M_vt_add_conflict_kw(cs, n3)                                \
    M_vt_add_conflict_kw(cs, n4)                                \
    M_vt_add_conflict_kw(cs, n5)                                \
    M_vt_add_conflict_kw(cs, n6)                                \
    M_vt_add_conflict_kw(cs, n7)                                \
    M_vt_add_r_conflict_set(v, cs)                              \
}


// 8-way right-side conflict

#define M_vt_right_conflict_8way(v, n1, n2, n3, n4, n5, n6, n7, n8) \
{                                                                   \
    M_vt_conflict_set(cs)                                           \
    M_vt_add_conflict_kw(cs, n1)                                    \
    M_vt_add_conflict_kw(cs, n2)                                    \
    M_vt_add_conflict_kw(cs, n3)                                    \
    M_vt_add_conflict_kw(cs, n4)                                    \
    M_vt_add_conflict_kw(cs, n5)                                    \
    M_vt_add_conflict_kw(cs, n6)                                    \
    M_vt_add_conflict_kw(cs, n7)                                    \
    M_vt_add_conflict_kw(cs, n8)                                    \
    M_vt_add_r_conflict_set(v, cs)                                  \
}

// 9-way right-side conflict

#define M_vt_right_conflict_9way(v, n1, n2, n3, n4, n5, n6, n7, n8, n9) \
{                                                                       \
    M_vt_conflict_set(cs)                                               \
    M_vt_add_conflict_kw(cs, n1)                                        \
    M_vt_add_conflict_kw(cs, n2)                                        \
    M_vt_add_conflict_kw(cs, n3)                                        \
    M_vt_add_conflict_kw(cs, n4)                                        \
    M_vt_add_conflict_kw(cs, n5)                                        \
    M_vt_add_conflict_kw(cs, n6)                                        \
    M_vt_add_conflict_kw(cs, n7)                                        \
    M_vt_add_conflict_kw(cs, n8)                                        \
    M_vt_add_conflict_kw(cs, n9)                                        \
    M_vt_add_r_conflict_set(v, cs)                                      \
}


// 10-way right-side conflict

#define M_vt_right_conflict_10way(v, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) \
{                                                                             \
    M_vt_conflict_set(cs)                                                     \
    M_vt_add_conflict_kw(cs, n1)                                              \
    M_vt_add_conflict_kw(cs, n2)                                              \
    M_vt_add_conflict_kw(cs, n3)                                              \
    M_vt_add_conflict_kw(cs, n4)                                              \
    M_vt_add_conflict_kw(cs, n5)                                              \
    M_vt_add_conflict_kw(cs, n6)                                              \
    M_vt_add_conflict_kw(cs, n7)                                              \
    M_vt_add_conflict_kw(cs, n8)                                              \
    M_vt_add_conflict_kw(cs, n9)                                              \
    M_vt_add_conflict_kw(cs, n10)                                             \
    M_vt_add_r_conflict_set(v, cs)                                            \
}


///////////////////////////////////////////////////////////////////

// left-side conflict pair

#define M_vt_left_conflict_pair(v, n1, n2)  \
{                                           \
    M_vt_conflict_set(cs)                   \
    M_vt_add_conflict_kw(cs, n1)            \
    M_vt_add_conflict_kw(cs, n2)            \
    M_vt_add_l_conflict_set(v, cs)          \
}

#define M_vt_left_conflict_2way(v, n1, n2)  \
{                                           \
    M_vt_conflict_set(cs)                   \
    M_vt_add_conflict_kw(cs, n1)            \
    M_vt_add_conflict_kw(cs, n2)            \
    M_vt_add_l_conflict_set(v, cs)          \
}

// 3-way left-side conflict

#define M_vt_left_conflict_3way(v, n1, n2, n3) \
{                                              \
    M_vt_conflict_set(cs)                      \
    M_vt_add_conflict_kw(cs, n1)               \
    M_vt_add_conflict_kw(cs, n2)               \
    M_vt_add_conflict_kw(cs, n3)               \
    M_vt_add_l_conflict_set(v, cs)             \
}


// 4-way left-side conflict

#define M_vt_left_conflict_4way(v, n1, n2, n3, n4) \
{                                                  \
    M_vt_conflict_set(cs)                          \
    M_vt_add_conflict_kw(cs, n1)                   \
    M_vt_add_conflict_kw(cs, n2)                   \
    M_vt_add_conflict_kw(cs, n3)                   \
    M_vt_add_conflict_kw(cs, n4)                   \
    M_vt_add_l_conflict_set(v, cs)                 \
}


// 5-way left-side conflict

#define M_vt_left_conflict_5way(v, n1, n2, n3, n4, n5) \
{                                                      \
    M_vt_conflict_set(cs)                              \
    M_vt_add_conflict_kw(cs, n1)                       \
    M_vt_add_conflict_kw(cs, n2)                       \
    M_vt_add_conflict_kw(cs, n3)                       \
    M_vt_add_conflict_kw(cs, n4)                       \
    M_vt_add_conflict_kw(cs, n5)                       \
    M_vt_add_l_conflict_set(v, cs)                     \
}


// 6-way left-side conflict

#define M_vt_left_conflict_6way(v, n1, n2, n3, n4, n5, n6) \
{                                                          \
    M_vt_conflict_set(cs)                                  \
    M_vt_add_conflict_kw(cs, n1)                           \
    M_vt_add_conflict_kw(cs, n2)                           \
    M_vt_add_conflict_kw(cs, n3)                           \
    M_vt_add_conflict_kw(cs, n4)                           \
    M_vt_add_conflict_kw(cs, n5)                           \
    M_vt_add_conflict_kw(cs, n6)                           \
    M_vt_add_l_conflict_set(v, cs)                         \
}


// 7-way left-side conflict

#define M_vt_left_conflict_7way(v, n1, n2, n3, n4, n5, n6, n7) \
{                                                              \
    M_vt_conflict_set(cs)                                      \
    M_vt_add_conflict_kw(cs, n1)                               \
    M_vt_add_conflict_kw(cs, n2)                               \
    M_vt_add_conflict_kw(cs, n3)                               \
    M_vt_add_conflict_kw(cs, n4)                               \
    M_vt_add_conflict_kw(cs, n5)                               \
    M_vt_add_conflict_kw(cs, n6)                               \
    M_vt_add_conflict_kw(cs, n7)                               \
    M_vt_add_l_conflict_set(v, cs)                             \
}


// 8-way left-side conflict

#define M_vt_left_conflict_8way(v, n1, n2, n3, n4, n5, n6, n7, n8) \
{                                                                  \
    M_vt_conflict_set(cs)                                          \
    M_vt_add_conflict_kw(cs, n1)                                   \
    M_vt_add_conflict_kw(cs, n2)                                   \
    M_vt_add_conflict_kw(cs, n3)                                   \
    M_vt_add_conflict_kw(cs, n4)                                   \
    M_vt_add_conflict_kw(cs, n5)                                   \
    M_vt_add_conflict_kw(cs, n6)                                   \
    M_vt_add_conflict_kw(cs, n7)                                   \
    M_vt_add_conflict_kw(cs, n8)                                   \
    M_vt_add_l_conflict_set(v, cs)                                 \
}

// 9-way left-side conflict

#define M_vt_left_conflict_9way(v, n1, n2, n3, n4, n5, n6, n7, n8, n9) \
{                                                                      \
    M_vt_conflict_set(cs)                                              \
    M_vt_add_conflict_kw(cs, n1)                                       \
    M_vt_add_conflict_kw(cs, n2)                                       \
    M_vt_add_conflict_kw(cs, n3)                                       \
    M_vt_add_conflict_kw(cs, n4)                                       \
    M_vt_add_conflict_kw(cs, n5)                                       \
    M_vt_add_conflict_kw(cs, n6)                                       \
    M_vt_add_conflict_kw(cs, n7)                                       \
    M_vt_add_conflict_kw(cs, n8)                                       \
    M_vt_add_conflict_kw(cs, n9)                                       \
    M_vt_add_l_conflict_set(v, cs)                                     \
}


// 10-way left-side conflict

#define M_vt_left_conflict_10way(v, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) \
{                                                                            \
    M_vt_conflict_set(cs)                                                    \
    M_vt_add_conflict_kw(cs, n1)                                             \
    M_vt_add_conflict_kw(cs, n2)                                             \
    M_vt_add_conflict_kw(cs, n3)                                             \
    M_vt_add_conflict_kw(cs, n4)                                             \
    M_vt_add_conflict_kw(cs, n5)                                             \
    M_vt_add_conflict_kw(cs, n6)                                             \
    M_vt_add_conflict_kw(cs, n7)                                             \
    M_vt_add_conflict_kw(cs, n8)                                             \
    M_vt_add_conflict_kw(cs, n9)                                             \
    M_vt_add_conflict_kw(cs, n10)                                            \
    M_vt_add_l_conflict_set(v, cs)                                           \
}
 

//////////////////////////////////////////////////////////////////////


// nested conflict pair

#define M_vt_nest_conflict_pair(pl, n1, n2) \
{                                           \
    M_vt_conflict_set(cs)                   \
    M_vt_add_conflict_kw(cs, n1)            \
    M_vt_add_conflict_kw(cs, n2)            \
    M_vt_add_nested_conflict_set(pl, cs)    \
}

#define M_vt_nest_conflict_2way(pl, n1, n2) \
{                                           \
    M_vt_conflict_set(cs)                   \
    M_vt_add_conflict_kw(cs, n1)            \
    M_vt_add_conflict_kw(cs, n2)            \
    M_vt_add_nested_conflict_set(pl, cs)    \
}

// 3-way nested conflict

#define M_vt_nest_conflict_3way(pl, n1, n2, n3) \
{                                               \
    M_vt_conflict_set(cs)                       \
    M_vt_add_conflict_kw(cs, n1)                \
    M_vt_add_conflict_kw(cs, n2)                \
    M_vt_add_conflict_kw(cs, n3)                \
    M_vt_add_nested_conflict_set(pl, cs)        \
}


// 4-way nested conflict

#define M_vt_nest_conflict_4way(pl, n1, n2, n3, n4) \
{                                                   \
    M_vt_conflict_set(cs)                           \
    M_vt_add_conflict_kw(cs, n1)                    \
    M_vt_add_conflict_kw(cs, n2)                    \
    M_vt_add_conflict_kw(cs, n3)                    \
    M_vt_add_conflict_kw(cs, n4)                    \
    M_vt_add_nested_conflict_set(pl, cs)            \
}


// 5-way nested conflict

#define M_vt_nest_conflict_5way(pl, n1, n2, n3, n4, n5) \
{                                                       \
    M_vt_conflict_set(cs)                               \
    M_vt_add_conflict_kw(cs, n1)                        \
    M_vt_add_conflict_kw(cs, n2)                        \
    M_vt_add_conflict_kw(cs, n3)                        \
    M_vt_add_conflict_kw(cs, n4)                        \
    M_vt_add_conflict_kw(cs, n5)                        \
    M_vt_add_nested_conflict_set(pl, cs)                \
}


// 6-way nested conflict

#define M_vt_nest_conflict_6way(pl, n1, n2, n3, n4, n5, n6) \
{                                                           \
    M_vt_conflict_set(cs)                                   \
    M_vt_add_conflict_kw(cs, n1)                            \
    M_vt_add_conflict_kw(cs, n2)                            \
    M_vt_add_conflict_kw(cs, n3)                            \
    M_vt_add_conflict_kw(cs, n4)                            \
    M_vt_add_conflict_kw(cs, n5)                            \
    M_vt_add_conflict_kw(cs, n6)                            \
    M_vt_add_nested_conflict_set(pl, cs)                    \
}


// 7-way nested conflict

#define M_vt_nest_conflict_7way(pl, n1, n2, n3, n4, n5, n6, n7) \
{                                                               \
    M_vt_conflict_set(cs)                                       \
    M_vt_add_conflict_kw(cs, n1)                                \
    M_vt_add_conflict_kw(cs, n2)                                \
    M_vt_add_conflict_kw(cs, n3)                                \
    M_vt_add_conflict_kw(cs, n4)                                \
    M_vt_add_conflict_kw(cs, n5)                                \
    M_vt_add_conflict_kw(cs, n6)                                \
    M_vt_add_conflict_kw(cs, n7)                                \
    M_vt_add_nested_conflict_set(pl, cs)                        \
}


// 8-way nested conflict

#define M_vt_nest_conflict_8way(pl, n1, n2, n3, n4, n5, n6, n7, n8) \
{                                                                   \
    M_vt_conflict_set(cs)                                           \
    M_vt_add_conflict_kw(cs, n1)                                    \
    M_vt_add_conflict_kw(cs, n2)                                    \
    M_vt_add_conflict_kw(cs, n3)                                    \
    M_vt_add_conflict_kw(cs, n4)                                    \
    M_vt_add_conflict_kw(cs, n5)                                    \
    M_vt_add_conflict_kw(cs, n6)                                    \
    M_vt_add_conflict_kw(cs, n7)                                    \
    M_vt_add_conflict_kw(cs, n8)                                    \
    M_vt_add_nested_conflict_set(pl, cs)                            \
}

// 9-way nested conflict

#define M_vt_nest_conflict_9way(pl, n1, n2, n3, n4, n5, n6, n7, n8, n9) \
{                                                                       \
    M_vt_conflict_set(cs)                                               \
    M_vt_add_conflict_kw(cs, n1)                                        \
    M_vt_add_conflict_kw(cs, n2)                                        \
    M_vt_add_conflict_kw(cs, n3)                                        \
    M_vt_add_conflict_kw(cs, n4)                                        \
    M_vt_add_conflict_kw(cs, n5)                                        \
    M_vt_add_conflict_kw(cs, n6)                                        \
    M_vt_add_conflict_kw(cs, n7)                                        \
    M_vt_add_conflict_kw(cs, n8)                                        \
    M_vt_add_conflict_kw(cs, n9)                                        \
    M_vt_add_nested_conflict_set(pl, cs)                                \
}


// 10-way nested conflict

#define M_vt_nest_conflict_10way(pl, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) \
{                                                                             \
    M_vt_conflict_set(cs)                                                     \
    M_vt_add_conflict_kw(cs, n1)                                              \
    M_vt_add_conflict_kw(cs, n2)                                              \
    M_vt_add_conflict_kw(cs, n3)                                              \
    M_vt_add_conflict_kw(cs, n4)                                              \
    M_vt_add_conflict_kw(cs, n5)                                              \
    M_vt_add_conflict_kw(cs, n6)                                              \
    M_vt_add_conflict_kw(cs, n7)                                              \
    M_vt_add_conflict_kw(cs, n8)                                              \
    M_vt_add_conflict_kw(cs, n9)                                              \
    M_vt_add_conflict_kw(cs, n10)                                             \
    M_vt_add_nested_conflict_set(pl, cs)                                      \
}


// example -- for larger (nested) conflict sets: 
//
//  {
//    M_vt_conflict_set(                 cs        )
//    M_vt_add_conflict_kw(              cs, L"k01") 
//    M_vt_add_conflict_kw(              cs, L"k02") 
//    M_vt_add_conflict_kw(              cs, L"k03") 
//    M_vt_add_conflict_kw(              cs, L"k04") 
//                                           .
//                                           .
//                                           .
//    M_vt_add_conflict_kw(              cs, L"k99")
//    M_vt_add_nested_conflict_set(  pl, cs        ) 
//  }
//

////////////////////////////////////////////////////////////////
///// MACROs to add keyword choice sets to plist 
////////////////////////////////////////////////////////////////


// right-side choice pair

#define M_vt_right_choice_pair(v, n1, n2)  \
{                                          \
    M_vt_choice_set(cs)                    \
    M_vt_add_choice_kw(cs, n1)             \
    M_vt_add_choice_kw(cs, n2)             \
    M_vt_add_r_choice_set(v, cs)           \
}

#define M_vt_right_choice_2way(v, n1, n2)  \
{                                          \
    M_vt_choice_set(cs)                    \
    M_vt_add_choice_kw(cs, n1)             \
    M_vt_add_choice_kw(cs, n2)             \
    M_vt_add_r_choice_set(v, cs)           \
}

// 3-way right-side conflict

#define M_vt_right_choice_3way(v, n1, n2, n3) \
{                                             \
    M_vt_choice_set(cs)                       \
    M_vt_add_choice_kw(cs, n1)                \
    M_vt_add_choice_kw(cs, n2)                \
    M_vt_add_choice_kw(cs, n3)                \
    M_vt_add_r_choice_set(v, cs)              \
}


// 4-way right-side conflict

#define M_vt_right_choice_4way(v, n1, n2, n3, n4) \
{                                                 \
    M_vt_choice_set(cs)                           \
    M_vt_add_choice_kw(cs, n1)                    \
    M_vt_add_choice_kw(cs, n2)                    \
    M_vt_add_choice_kw(cs, n3)                    \
    M_vt_add_choice_kw(cs, n4)                    \
    M_vt_add_r_choice_set(v, cs)                  \
}




///////////////////////////////////////////////////////////////////

// left-side choice pair

#define M_vt_left_choice_pair(v, n1, n2)  \
{                                         \
    M_vt_choice_set(cs)                   \
    M_vt_add_choice_kw(cs, n1)            \
    M_vt_add_choice_kw(cs, n2)            \
    M_vt_add_l_choice_set(v, cs)          \
}

#define M_vt_left_choice_2way(v, n1, n2)  \
{                                         \
    M_vt_choice_set(cs)                   \
    M_vt_add_choice_kw(cs, n1)            \
    M_vt_add_choice_kw(cs, n2)            \
    M_vt_add_l_choice_set(v, cs)          \
}

// 3-way left-side choice

#define M_vt_left_choice_3way(v, n1, n2, n3) \
{                                            \
    M_vt_choice_set(cs)                      \
    M_vt_add_choice_kw(cs, n1)               \
    M_vt_add_choice_kw(cs, n2)               \
    M_vt_add_choice_kw(cs, n3)               \
    M_vt_add_l_choice_set(v, cs)             \
}


// 4-way left-side choice

#define M_vt_left_choice_4way(v, n1, n2, n3, n4) \
{                                                \
    M_vt_choice_set(cs)                          \
    M_vt_add_choice_kw(cs, n1)                   \
    M_vt_add_choice_kw(cs, n2)                   \
    M_vt_add_choice_kw(cs, n3)                   \
    M_vt_add_choice_kw(cs, n4)                   \
    M_vt_add_l_choice_set(v, cs)                 \
}




//////////////////////////////////////////////////////////////////////


// nested choice pair

#define M_vt_nest_choice_pair(pl, n1, n2) \
{                                         \
    M_vt_choice_set(cs)                   \
    M_vt_add_choice_kw(cs, n1)            \
    M_vt_add_choice_kw(cs, n2)            \
    M_vt_add_nested_choice_set(pl, cs)    \
}

#define M_vt_nest_choice_2way(pl, n1, n2) \
{                                         \
    M_vt_choice_set(cs)                   \
    M_vt_add_choice_kw(cs, n1)            \
    M_vt_add_choice_kw(cs, n2)            \
    M_vt_add_nested_choice_set(pl, cs)    \
}

// 3-way nested choice

#define M_vt_nest_choice_3way(pl, n1, n2, n3) \
{                                             \
    M_vt_choice_set(cs)                       \
    M_vt_add_choice_kw(cs, n1)                \
    M_vt_add_choice_kw(cs, n2)                \
    M_vt_add_choice_kw(cs, n3)                \
    M_vt_add_nested_choice_set(pl, cs)        \
}


// 4-way nested conflict

#define M_vt_nest_choice_4way(pl, n1, n2, n3, n4) \
{                                                 \
    M_vt_choice_set(cs)                           \
    M_vt_add_choice_kw(cs, n1)                    \
    M_vt_add_choice_kw(cs, n2)                    \
    M_vt_add_choice_kw(cs, n3)                    \
    M_vt_add_choice_kw(cs, n4)                    \
    M_vt_add_nested_choice_set(pl, cs)            \
}





/////////////////////////////////////////////////////////////////////////////////////////
///// MACROs to add positional parameters to verbdef (right/left-side) or to nested plist 
/////////////////////////////////////////////////////////////////////////////////////////


//////// positional parms in right-side plist  /////////////////////////////

#define            M_vt_any_right_pos(v        )            M_vt_any_x_pos( v,         r)
#define          M_vt_unit_right_pos( v        )           M_vt_unit_x_pos( v,         r)
#define          M_vt_int64_right_pos(v        )          M_vt_int64_x_pos( v,         r)
#define        M_vt_int64rc_right_pos(v, lo, hi)        M_vt_int64rc_x_pos( v, lo, hi, r)
#define        M_vt_float64_right_pos(v        )        M_vt_float64_x_pos( v,         r)
#define      M_vt_float64rc_right_pos(v, lo, hi)      M_vt_float64rc_x_pos( v, lo, hi, r)
#define         M_vt_string_right_pos(v        )         M_vt_string_x_pos( v,         r)
#define          M_vt_slist_right_pos(v        )          M_vt_slist_x_pos( v,         r)
#define        M_vt_verbdef_right_pos(v        )        M_vt_verbdef_x_pos( v,         r)
#define         M_vt_typdef_right_pos(v        )         M_vt_typdef_x_pos( v,         r)
#define          M_vt_array_right_pos(v        )          M_vt_array_x_pos( v,         r)
#define         M_vt_struct_right_pos(v        )         M_vt_struct_x_pos( v,         r)
#define            M_vt_ref_right_pos(v        )            M_vt_ref_x_pos( v,         r)
#define      M_vt_var_ident_right_pos(v        )      M_vt_var_ident_x_pos( v,         r)
#define         M_vt_lvalue_right_pos(v        )         M_vt_lvalue_x_pos( v,         r)
#define      M_vt_def_ident_right_pos(v        )      M_vt_def_ident_x_pos( v,         r)
#define    M_vt_undef_ident_right_pos(v        )    M_vt_undef_ident_x_pos( v,         r)
#define M_vt_nonconst_ident_right_pos(v        ) M_vt_nonconst_ident_x_pos( v,         r)
#define      M_vt_any_ident_right_pos(v        )      M_vt_any_ident_x_pos( v,         r)
#define      M_vt_raw_ident_right_pos(v        )      M_vt_raw_ident_x_pos( v,         r)
#define         M_vt_vlist0_right_pos(v        )         M_vt_vlist0_x_pos( v,         r)
#define          M_vt_vlist_right_pos(v, pln   )          M_vt_vlist_x_pos( v, pln,    r)
#define            M_vt_int_right_pos(v        )            M_vt_int_x_pos( v,         r)
#define          M_vt_arith_right_pos(v        )          M_vt_arith_x_pos( v,         r)
#define        M_vt_compare_right_pos(v        )        M_vt_compare_x_pos( v,         r)
#define     M_vt_assigntype_right_pos(v        )     M_vt_assigntype_x_pos( v,         r)
#define         M_vt_lvalue_right_pos(v        )         M_vt_lvalue_x_pos( v,         r)


//////// positional parms in left-side plist  ///////////////////////////////////////////////////////

#define            M_vt_any_left_pos( v        )            M_vt_any_x_pos( v,         l)
#define          M_vt_unit_left_pos(  v        )           M_vt_unit_x_pos( v,         l)
#define          M_vt_int64_left_pos( v        )          M_vt_int64_x_pos( v,         l)
#define        M_vt_int64rc_left_pos( v, lo, hi)        M_vt_int64rc_x_pos( v, lo, hi, l)
#define        M_vt_float64_left_pos( v        )        M_vt_float64_x_pos( v,         l)
#define      M_vt_float64rc_left_pos( v, lo, hi)      M_vt_float64rc_x_pos( v, lo, hi, l)
#define         M_vt_string_left_pos( v        )         M_vt_string_x_pos( v,         l)
#define          M_vt_slist_left_pos( v        )          M_vt_slist_x_pos( v,         l)
#define        M_vt_verbdef_left_pos( v        )        M_vt_verbdef_x_pos( v,         l)
#define         M_vt_typdef_left_pos( v        )         M_vt_typdef_x_pos( v,         l)
#define          M_vt_array_left_pos( v        )          M_vt_array_x_pos( v,         l)
#define         M_vt_struct_left_pos( v        )         M_vt_struct_x_pos( v,         l)
#define            M_vt_ref_left_pos( v        )            M_vt_ref_x_pos( v,         l)
#define      M_vt_var_ident_left_pos( v        )      M_vt_var_ident_x_pos( v,         l)
#define         M_vt_lvalue_left_pos( v        )         M_vt_lvalue_x_pos( v,         l)
#define      M_vt_def_ident_left_pos( v        )      M_vt_def_ident_x_pos( v,         l)
#define    M_vt_undef_ident_left_pos( v        )    M_vt_undef_ident_x_pos( v,         l)
#define M_vt_nonconst_ident_left_pos( v        ) M_vt_nonconst_ident_x_pos( v,         l)
#define      M_vt_any_ident_left_pos( v        )      M_vt_any_ident_x_pos( v,         l)
#define      M_vt_raw_ident_left_pos( v        )      M_vt_raw_ident_x_pos( v,         l)
#define         M_vt_vlist0_left_pos( v        )         M_vt_vlist0_x_pos( v,         l)
#define          M_vt_vlist_left_pos( v, pln   )          M_vt_vlist_x_pos( v, pln,    l)
#define            M_vt_int_left_pos( v        )            M_vt_int_x_pos( v,         l)
#define          M_vt_arith_left_pos( v        )          M_vt_arith_x_pos( v,         l)
#define        M_vt_compare_left_pos( v        )        M_vt_compare_x_pos( v,         l)
#define     M_vt_assigntype_left_pos( v        )     M_vt_assigntype_x_pos( v,         l)
#define         M_vt_lvalue_left_pos( v        )         M_vt_lvalue_x_pos( v,         l)
                                   

//////// positional parms in nested plist /////////////////////////////////////////////////

#define            M_vt_any_nest_pos(pl        )            M_vt_any_x_pos(pl,         nested)
#define          M_vt_unit_nest_pos( pl        )           M_vt_unit_x_pos(pl,         nested)
#define          M_vt_int64_nest_pos(pl        )          M_vt_int64_x_pos(pl,         nested)
#define        M_vt_int64rc_nest_pos(pl, lo, hi)        M_vt_int64rc_x_pos(pl, lo, hi, nested)
#define        M_vt_float64_nest_pos(pl        )        M_vt_float64_x_pos(pl,         nested)
#define      M_vt_float64rc_nest_pos(pl, lo, hi)      M_vt_float64rc_x_pos(pl, lo, hi, nested)
#define         M_vt_string_nest_pos(pl        )         M_vt_string_x_pos(pl,         nested)
#define          M_vt_slist_nest_pos(pl        )          M_vt_slist_x_pos(pl,         nested)
#define        M_vt_verbdef_nest_pos(pl        )        M_vt_verbdef_x_pos(pl,         nested)
#define         M_vt_typdef_nest_pos(pl        )         M_vt_typdef_x_pos(pl,         nested)
#define          M_vt_array_nest_pos(pl        )          M_vt_array_x_pos(pl,         nested)
#define         M_vt_struct_nest_pos(pl        )         M_vt_struct_x_pos(pl,         nested)
#define            M_vt_ref_nest_pos(pl        )            M_vt_ref_x_pos(pl,         nested)
#define      M_vt_var_ident_nest_pos(pl        )      M_vt_var_ident_x_pos(pl,         nested)
#define         M_vt_lvalue_nest_pos(pl        )         M_vt_lvalue_x_pos(pl,         nested)
#define      M_vt_def_ident_nest_pos(pl        )      M_vt_def_ident_x_pos(pl,         nested)
#define    M_vt_undef_ident_nest_pos(pl        )    M_vt_undef_ident_x_pos(pl,         nested)
#define M_vt_nonconst_ident_nest_pos(pl        ) M_vt_nonconst_ident_x_pos(pl,         nested)
#define      M_vt_any_ident_nest_pos(pl        )      M_vt_any_ident_x_pos(pl,         nested)
#define      M_vt_raw_ident_nest_pos(pl        )      M_vt_raw_ident_x_pos(pl,         nested)
#define         M_vt_vlist0_nest_pos(pl        )         M_vt_vlist0_x_pos(pl,         nested)
#define          M_vt_vlist_nest_pos(pl, pln   )          M_vt_vlist_x_pos(pl, pln,    nested)
#define            M_vt_int_nest_pos(pl        )            M_vt_int_x_pos(pl,         nested)
#define          M_vt_arith_nest_pos(pl        )          M_vt_arith_x_pos(pl,         nested)
#define        M_vt_compare_nest_pos(pl        )        M_vt_compare_x_pos(pl,         nested)
#define     M_vt_assigntype_nest_pos(pl        )     M_vt_assigntype_x_pos(pl,         nested)
#define         M_vt_lvalue_nest_pos(pl        )         M_vt_lvalue_x_pos(pl,         nested)


//////// positional parms in left/right/nested plist /////////////////////////////////////////////////


// any positional parm

#define M_vt_any_x_pos(pl, x)          \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.anything_ok = true;             \
    M_vt_add_x_pos(pl, pt, x);         \
}
 

// unit positional parm

#define M_vt_unit_x_pos(pl, x)         \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.unit_ok = true;                 \
    M_vt_add_x_pos(pl, pt, x);         \
}


//  int64 positional parm -- no range check 

#define M_vt_int64_x_pos(pl, x)        \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.int64_ok = true;                \
    M_vt_add_x_pos(pl, pt, x);         \
}


// int64 positional parm -- with range check 

#define M_vt_int64rc_x_pos(pl, lo, hi, x) \
{                                         \
    M_vt_pos_parm(pt)                     \
    pt.int64_ok      = true;              \
    pt.int64_range   = true;              \
    pt.int64_min     = (lo);              \
    pt.int64_max     = (hi);              \
    M_vt_add_x_pos(pl, pt, x);            \
}


//  float64 positional parm -- no range check 

#define M_vt_float64_x_pos(pl, x)      \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.float64_ok = true;              \
    M_vt_add_x_pos(pl, pt, x);         \
}


// float64 positional parm -- with range check 

#define M_vt_float64rc_x_pos(pl, lo, hi, x) \
{                                           \
    M_vt_pos_parm(pt)                       \
    pt.float64_ok      = true;              \
    pt.float64_range   = true;              \
    pt.float64_min     = (lo);              \
    pt.float64_max     = (hi);              \
    M_vt_add_x_pos(pl, pt, x);              \
}


//  string positional parm 

#define M_vt_string_x_pos(pl, x)       \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.string_ok = true;               \
    M_vt_add_x_pos(pl, pt, x);         \
}


// (unevaluated) var_ident positional parm 

#define M_vt_var_ident_x_pos(pl, x)    \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.var_ident_ok  = true;           \
    pt.no_eval_ident = true;           \
    M_vt_add_x_pos(pl, pt, x);         \
}


// lvalue -- (unevaluated) var_ident or (unevaluated) lvalue reference positional parm 

#define M_vt_lvalue_x_pos(pl, x)          \
{                                         \
    M_vt_pos_parm(pt)                     \
    pt.var_ident_ok  = true;              \
    pt.lvalue_ref_ok = true;              \
    pt.no_eval_ident = true;              \
    pt.no_eval_ref   = true;              \
    M_vt_add_x_pos(pl, pt, x);            \
}  


// (unevaluated) var_ident or const_ident (defined ident) positional parm 

#define M_vt_def_ident_x_pos(pl, x)    \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.var_ident_ok   = true;          \
    pt.const_ident_ok = true;          \
    pt.no_eval_ident  = true;          \
    M_vt_add_x_pos(pl, pt, x);         \
}



// (unevaluated) undef_ident positional parm 

#define M_vt_undef_ident_x_pos(pl, x)  \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.undef_ident_ok = true;          \
    pt.no_eval_ident  = true;          \
    M_vt_add_x_pos(pl, pt, x);         \
}
  

// (unevaluated) undef_ident or var_ident positional parm 

#define M_vt_nonconst_ident_x_pos(pl, x) \
{                                        \
    M_vt_pos_parm(pt)                    \
    pt.undef_ident_ok = true;            \
    pt.var_ident_ok   = true;            \
    pt.no_eval_ident  = true;            \
    M_vt_add_x_pos(pl, pt, x);           \
}


// (unevaluated) undef_ident, const_ident, or var_ident positional parm 

#define M_vt_any_ident_x_pos(pl, x)    \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.undef_ident_ok = true;          \
    pt.var_ident_ok   = true;          \
    pt.const_ident_ok = true;          \
    pt.no_eval_ident  = true;          \
    M_vt_add_x_pos(pl, pt, x);         \
}


// (unevaluated) raw ident -- type is not checked 

#define M_vt_raw_ident_x_pos(pl, x)    \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.raw_ident_ok = true;            \
    pt.no_eval_ident  = true;          \
    M_vt_add_x_pos(pl, pt, x);         \
}


// slist positional parm 

#define M_vt_slist_x_pos(pl, x)        \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.slist_ok = true;                \
    M_vt_add_x_pos(pl, pt, x);         \
}


// verbdef positional parm 

#define M_vt_verbdef_x_pos(pl, x)      \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.verbdef_ok = true;              \
    M_vt_add_x_pos(pl, pt, x);         \
}  


// typdef positional parm 

#define M_vt_typdef_x_pos(pl, x)       \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.typdef_ok = true;               \
    M_vt_add_x_pos(pl, pt, x);         \
}  


// array positional parm 

#define M_vt_array_x_pos(pl, x)        \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.array_ok = true;                \
    M_vt_add_x_pos(pl, pt, x);         \
} 


// struct positional parm 

#define M_vt_struct_x_pos(pl, x)       \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.structure_ok = true;            \
    M_vt_add_x_pos(pl, pt, x);         \
} 


// ref positional parm 

#define M_vt_ref_x_pos(pl, x)          \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.lvalue_ref_ok = true;           \
    pt.rvalue_ref_ok = true;           \
    M_vt_add_x_pos(pl, pt, x);         \
}  


// vlist positional parm  (no description for nested vlist) 

#define M_vt_vlist0_x_pos(pl, x)       \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.vlist_ok = true;                \
    M_vt_add_x_pos(pl, pt, x);         \
}


// vlist positional parm -- must pass in pre-built plist to be attached 

#define M_vt_vlist_x_pos(pl, pln, x)   \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.vlist_ok = true;                \
    M_vt_add_nested_plist(pt, pln)     \
    M_vt_add_x_pos(pl, pt, x)          \
}


//  integer positional parm  

#define M_vt_int_x_pos(pl, x)          \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.int8_ok    = true;              \
    pt.int16_ok   = true;              \
    pt.int32_ok   = true;              \
    pt.int64_ok   = true;              \
    pt.uint8_ok   = true;              \
    pt.uint16_ok  = true;              \
    pt.uint32_ok  = true;              \
    pt.uint64_ok  = true;              \
    M_vt_add_x_pos(pl, pt, x);         \
}


//  arithmetic positional parm  

#define M_vt_arith_x_pos(pl, x)        \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.int8_ok    = true;              \
    pt.int16_ok   = true;              \
    pt.int32_ok   = true;              \
    pt.int64_ok   = true;              \
    pt.uint8_ok   = true;              \
    pt.uint16_ok  = true;              \
    pt.uint32_ok  = true;              \
    pt.uint64_ok  = true;              \
    pt.float32_ok = true;              \
    pt.float64_ok = true;              \
    M_vt_add_x_pos(pl, pt, x);         \
}


//  comparable positional parm  

#define M_vt_compare_x_pos(pl, x)      \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.unit_ok    = true;              \
    pt.int8_ok    = true;              \
    pt.int16_ok   = true;              \
    pt.int32_ok   = true;              \
    pt.int64_ok   = true;              \
    pt.uint8_ok   = true;              \
    pt.uint16_ok  = true;              \
    pt.uint32_ok  = true;              \
    pt.uint64_ok  = true;              \
    pt.float32_ok = true;              \
    pt.float64_ok = true;              \
    pt.string_ok  = true;              \
    M_vt_add_x_pos(pl, pt, x);         \
}


//  assignable positional parm    

#define M_vt_assigntype_x_pos(pl, x)         \
{                                            \
    M_vt_pos_parm(pt)                        \
    pt.unit_ok        = true;                \
    pt.int8_ok        = true;                \
    pt.int16_ok       = true;                \
    pt.int32_ok       = true;                \
    pt.int64_ok       = true;                \
    pt.uint8_ok       = true;                \
    pt.uint16_ok      = true;                \
    pt.uint32_ok      = true;                \
    pt.uint64_ok      = true;                \
    pt.float32_ok     = true;                \
    pt.float64_ok     = true;                \
    pt.string_ok      = true;                \
    pt.undef_ident_ok = true;                \
    pt.var_ident_ok   = true;                \
    pt.const_ident_ok = true;                \
    pt.vlist_ok       = true;                \
    pt.slist_ok       = true;                \
    pt.verbdef_ok     = true;                \
    pt.typdef_ok      = true;                \
    pt.array_ok       = true;                \
    pt.structure_ok   = true;                \
    pt.lvalue_ref_ok  = true;                \
    pt.rvalue_ref_ok  = true;                \
    M_vt_add_x_pos(pl, pt, x);               \
}
 

// (unevaluated) var_ident positional parm   -or-  (evaluated) ref (lvalue ref only)  

#define M_vt_lvalue_x_pos(pl, x)       \
{                                      \
    M_vt_pos_parm(pt)                  \
    pt.var_ident_ok  = true;           \
    pt.lvalue_ref_ok = true;           \
    pt.no_eval_ident = true;           \
    pt.no_eval_ref   = true;           \
    M_vt_add_x_pos(pl, pt, x);         \
}
 



/////////////////////////////////////////////////////////////////////////////////////
///// MACROs to add optional keywords to verbdef (right/left-side) or to nested plist 
/////////////////////////////////////////////////////////////////////////////////////


//////// keywords in right-side plist  ///////////////////////////////////////////////////////

#define            M_vt_any_optional_right_kw(  v, n)                   M_vt_any_multi_right_kw(  v, n,       0, 1)
#define           M_vt_nval_optional_right_kw(  v, n)                  M_vt_nval_multi_right_kw(  v, n,       0, 1)
#define          M_vt_int64_optional_right_kw(  v, n)                 M_vt_int64_multi_right_kw(  v, n,       0, 1)
#define        M_vt_int64rc_optional_right_kw(  v, n, x, y)         M_vt_int64rc_multi_right_kw(  v, n, x, y, 0, 1)
#define        M_vt_float64_optional_right_kw(  v, n)               M_vt_float64_multi_right_kw(  v, n,       0, 1)
#define      M_vt_float64rc_optional_right_kw(  v, n, x, y)       M_vt_float64rc_multi_right_kw(  v, n, x, y, 0, 1)
#define         M_vt_string_optional_right_kw(  v, n)                M_vt_string_multi_right_kw(  v, n,       0, 1)
#define          M_vt_slist_optional_right_kw(  v, n)                 M_vt_slist_multi_right_kw(  v, n,       0, 1)
#define        M_vt_verbdef_optional_right_kw(  v, n)               M_vt_verbdef_multi_right_kw(  v, n,       0, 1)
#define         M_vt_typdef_optional_right_kw(  v, n)                M_vt_typdef_multi_right_kw(  v, n,       0, 1)
#define          M_vt_array_optional_right_kw(  v, n)                 M_vt_array_multi_right_kw(  v, n,       0, 1)
#define         M_vt_struct_optional_right_kw(  v, n)                M_vt_struct_multi_right_kw(  v, n,       0, 1)
#define            M_vt_ref_optional_right_kw(  v, n)                   M_vt_ref_multi_right_kw(  v, n,       0, 1)
#define      M_vt_var_ident_optional_right_kw(  v, n)             M_vt_var_ident_multi_right_kw(  v, n,       0, 1)
#define         M_vt_lvalue_optional_right_kw(  v, n)                M_vt_lvalue_multi_right_kw(  v, n,       0, 1)
#define    M_vt_undef_ident_optional_right_kw(  v, n)           M_vt_undef_ident_multi_right_kw(  v, n,       0, 1)
#define      M_vt_def_ident_optional_right_kw(  v, n)             M_vt_def_ident_multi_right_kw(  v, n,       0, 1)
#define M_vt_nonconst_ident_optional_right_kw(  v, n)        M_vt_nonconst_ident_multi_right_kw(  v, n,       0, 1)
#define      M_vt_any_ident_optional_right_kw(  v, n)             M_vt_any_ident_multi_right_kw(  v, n,       0, 1)
#define      M_vt_raw_ident_optional_right_kw(  v, n)             M_vt_raw_ident_multi_right_kw(  v, n,       0, 1)
#define         M_vt_vlist0_optional_right_kw(  v, n)                M_vt_vlist0_multi_right_kw(  v, n,       0, 1)
#define          M_vt_vlist_optional_right_kw(  v, n, pln)            M_vt_vlist_multi_right_kw(  v, n, pln,  0, 1)         // description for nested vlist must be passed in 
#define            M_vt_int_optional_right_kw(  v, n)                   M_vt_int_multi_right_kw(  v, n,       0, 1)
#define          M_vt_arith_optional_right_kw(  v, n)                 M_vt_arith_multi_right_kw(  v, n,       0, 1)
#define        M_vt_compare_optional_right_kw(  v, n)               M_vt_compare_multi_right_kw(  v, n,       0, 1)
#define     M_vt_assigntype_optional_right_kw(  v, n)            M_vt_assigntype_multi_right_kw(  v, n,       0, 1)
#define         M_vt_lvalue_optional_right_kw(  v, n)                M_vt_lvalue_multi_right_kw(  v, n,       0, 1)
                                                               

//////// keywords in left-side plist  ///////////////////////////////////////////////////////

#define            M_vt_any_optional_left_kw(  v, n)                    M_vt_any_multi_left_kw(   v, n,       0, 1)
#define           M_vt_nval_optional_left_kw(  v, n)                   M_vt_nval_multi_left_kw(   v, n,       0, 1)
#define          M_vt_int64_optional_left_kw(  v, n)                  M_vt_int64_multi_left_kw(   v, n,       0, 1)
#define        M_vt_int64rc_optional_left_kw(  v, n, x, y)          M_vt_int64rc_multi_left_kw(   v, n, x, y, 0, 1)
#define        M_vt_float64_optional_left_kw(  v, n)                M_vt_float64_multi_left_kw(   v, n,       0, 1)
#define      M_vt_float64rc_optional_left_kw(  v, n, x, y)        M_vt_float64rc_multi_left_kw(   v, n, x, y, 0, 1)
#define         M_vt_string_optional_left_kw(  v, n)                 M_vt_string_multi_left_kw(   v, n,       0, 1)
#define          M_vt_slist_optional_left_kw(  v, n)                  M_vt_slist_multi_left_kw(   v, n,       0, 1)
#define        M_vt_verbdef_optional_left_kw(  v, n)                M_vt_verbdef_multi_left_kw(   v, n,       0, 1)
#define         M_vt_typdef_optional_left_kw(  v, n)                 M_vt_typdef_multi_left_kw(   v, n,       0, 1)
#define          M_vt_array_optional_left_kw(  v, n)                  M_vt_array_multi_left_kw(   v, n,       0, 1)
#define         M_vt_struct_optional_left_kw(  v, n)                 M_vt_struct_multi_left_kw(   v, n,       0, 1)
#define            M_vt_ref_optional_left_kw(  v, n)                    M_vt_ref_multi_left_kw(   v, n,       0, 1)
#define      M_vt_var_ident_optional_left_kw(  v, n)              M_vt_var_ident_multi_left_kw(   v, n,       0, 1)
#define         M_vt_lvalue_optional_left_kw(  v, n)                 M_vt_lvalue_multi_left_kw(   v, n,       0, 1)
#define    M_vt_undef_ident_optional_left_kw(  v, n)            M_vt_undef_ident_multi_left_kw(   v, n,       0, 1)
#define      M_vt_def_ident_optional_left_kw(  v, n)              M_vt_def_ident_multi_left_kw(   v, n,       0, 1)
#define M_vt_nonconst_ident_optional_left_kw(  v, n)         M_vt_nonconst_ident_multi_left_kw(   v, n,       0, 1)
#define      M_vt_any_ident_optional_left_kw(  v, n)              M_vt_any_ident_multi_left_kw(   v, n,       0, 1)
#define      M_vt_raw_ident_optional_left_kw(  v, n)              M_vt_raw_ident_multi_left_kw(   v, n,       0, 1)
#define         M_vt_vlist0_optional_left_kw(  v, n)                 M_vt_vlist0_multi_left_kw(   v, n,       0, 1)
#define          M_vt_vlist_optional_left_kw(  v, n, pln)             M_vt_vlist_multi_left_kw(   v, n, pln,  0, 1)         // description for nested vlist must be passed in 
#define            M_vt_int_optional_left_kw(  v, n)                    M_vt_int_multi_left_kw(   v, n,       0, 1)
#define        M_vt_compare_optional_left_kw(  v, n)                M_vt_compare_multi_left_kw(   v, n,       0, 1)
#define     M_vt_assigntype_optional_left_kw(  v, n)             M_vt_assigntype_multi_left_kw(   v, n,       0, 1)
#define         M_vt_lvalue_optional_left_kw(  v, n)                 M_vt_lvalue_multi_left_kw(   v, n,       0, 1)


//////// keywords in nested plist /////////////////////////////////////////////////

#define            M_vt_any_optional_nest_kw( pl, n)                   M_vt_any_multi_nest_kw(  pl, n,       0, 1)
#define          M_vt_nval_optional_nest_kw(  pl, n)                  M_vt_nval_multi_nest_kw(  pl, n,       0, 1)
#define          M_vt_int64_optional_nest_kw( pl, n)                 M_vt_int64_multi_nest_kw(  pl, n,       0, 1)
#define        M_vt_int64rc_optional_nest_kw( pl, n, x, y)         M_vt_int64rc_multi_nest_kw(  pl, n, x, y, 0, 1)
#define        M_vt_float64_optional_nest_kw( pl, n)               M_vt_float64_multi_nest_kw(  pl, n,       0, 1)
#define      M_vt_float64rc_optional_nest_kw( pl, n, x, y)       M_vt_float64rc_multi_nest_kw(  pl, n, x, y, 0, 1)
#define         M_vt_string_optional_nest_kw( pl, n)                M_vt_string_multi_nest_kw(  pl, n,       0, 1)
#define          M_vt_slist_optional_nest_kw( pl, n)                 M_vt_slist_multi_nest_kw(  pl, n,       0, 1)
#define        M_vt_verbdef_optional_nest_kw( pl, n)               M_vt_verbdef_multi_nest_kw(  pl, n,       0, 1)
#define         M_vt_typdef_optional_nest_kw( pl, n)                M_vt_typdef_multi_nest_kw(  pl, n,       0, 1)
#define          M_vt_array_optional_nest_kw( pl, n)                 M_vt_array_multi_nest_kw(  pl, n,       0, 1)
#define         M_vt_struct_optional_nest_kw( pl, n)                M_vt_struct_multi_nest_kw(  pl, n,       0, 1)
#define            M_vt_ref_optional_nest_kw( pl, n)                   M_vt_ref_multi_nest_kw(  pl, n,       0, 1)
#define      M_vt_var_ident_optional_nest_kw( pl, n)             M_vt_var_ident_multi_nest_kw(  pl, n,       0, 1)
#define         M_vt_lvalue_optional_nest_kw( pl, n)                M_vt_lvalue_multi_nest_kw(  pl, n,       0, 1)
#define    M_vt_undef_ident_optional_nest_kw( pl, n)           M_vt_undef_ident_multi_nest_kw(  pl, n,       0, 1)
#define      M_vt_def_ident_optional_nest_kw( pl, n)             M_vt_def_ident_multi_nest_kw(  pl, n,       0, 1)
#define M_vt_nonconst_ident_optional_nest_kw( pl, n)        M_vt_nonconst_ident_multi_nest_kw(  pl, n,       0, 1)
#define      M_vt_any_ident_optional_nest_kw( pl, n)             M_vt_any_ident_multi_nest_kw(  pl, n,       0, 1)
#define      M_vt_raw_ident_optional_nest_kw( pl, n)             M_vt_raw_ident_multi_nest_kw(  pl, n,       0, 1)
#define         M_vt_vlist0_optional_nest_kw( pl, n)                M_vt_vlist0_multi_nest_kw(  pl, n,       0, 1) 
#define          M_vt_vlist_optional_nest_kw( pl, n, pln)            M_vt_vlist_multi_nest_kw(  pl, n, pln,  0, 1)         // description for nested vlist must be passed in 
#define            M_vt_int_optional_nest_kw( pl, n)                   M_vt_int_multi_nest_kw(  pl, n,       0, 1)
#define          M_vt_arith_optional_nest_kw( pl, n)                 M_vt_arith_multi_nest_kw(  pl, n,       0, 1)
#define        M_vt_compare_optional_nest_kw( pl, n)               M_vt_compare_multi_nest_kw(  pl, n,       0, 1)
#define     M_vt_assigntype_optional_nest_kw( pl, n)            M_vt_assigntype_multi_nest_kw(  pl, n,       0, 1)
#define         M_vt_lvalue_optional_nest_kw( pl, n)                M_vt_lvalue_multi_nest_kw(  pl, n,       0, 1)


/////////////////////////////////////////////////////////////////////////////////////
///// MACROs to add required keywords to verbdef (right/left-side) or to nested plist 
/////////////////////////////////////////////////////////////////////////////////////

//////// keywords in right-side plist  ///////////////////////////////////////////////////////

#define             M_vt_any_required_right_kw(  v, n)                    M_vt_any_multi_right_kw(  v, n,       1, 1)
#define            M_vt_nval_required_right_kw(  v, n)                   M_vt_nval_multi_right_kw(  v, n,       1, 1)
#define           M_vt_int64_required_right_kw(  v, n)                  M_vt_int64_multi_right_kw(  v, n,       1, 1)
#define         M_vt_int64rc_required_right_kw(  v, n, x, y)          M_vt_int64rc_multi_right_kw(  v, n, x, y, 1, 1)
#define         M_vt_float64_required_right_kw(  v, n)                M_vt_float64_multi_right_kw(  v, n,       1, 1)
#define       M_vt_float64rc_required_right_kw(  v, n, x, y)        M_vt_float64rc_multi_right_kw(  v, n, x, y, 1, 1)
#define          M_vt_string_required_right_kw(  v, n)                 M_vt_string_multi_right_kw(  v, n,       1, 1)
#define           M_vt_slist_required_right_kw(  v, n)                  M_vt_slist_multi_right_kw(  v, n,       1, 1)
#define         M_vt_verbdef_required_right_kw(  v, n)                M_vt_verbdef_multi_right_kw(  v, n,       1, 1)
#define          M_vt_typdef_required_right_kw(  v, n)                 M_vt_typdef_multi_right_kw(  v, n,       1, 1)
#define           M_vt_array_required_right_kw(  v, n)                  M_vt_array_multi_right_kw(  v, n,       1, 1)
#define          M_vt_struct_required_right_kw(  v, n)                 M_vt_struct_multi_right_kw(  v, n,       1, 1)
#define             M_vt_ref_required_right_kw(  v, n)                    M_vt_ref_multi_right_kw(  v, n,       1, 1)
#define       M_vt_var_ident_required_right_kw(  v, n)              M_vt_var_ident_multi_right_kw(  v, n,       1, 1)
#define          M_vt_lvalue_required_right_kw(  v, n)                 M_vt_lvalue_multi_right_kw(  v, n,       1, 1)
#define     M_vt_undef_ident_required_right_kw(  v, n)            M_vt_undef_ident_multi_right_kw(  v, n,       1, 1)
#define       M_vt_def_ident_required_right_kw(  v, n)              M_vt_def_ident_multi_right_kw(  v, n,       1, 1)
#define  M_vt_nonconst_ident_required_right_kw(  v, n)         M_vt_nonconst_ident_multi_right_kw(  v, n,       1, 1)
#define       M_vt_any_ident_required_right_kw(  v, n)              M_vt_any_ident_multi_right_kw(  v, n,       1, 1)
#define       M_vt_raw_ident_required_right_kw(  v, n)              M_vt_raw_ident_multi_right_kw(  v, n,       1, 1)
#define          M_vt_vlist0_required_right_kw(  v, n)                 M_vt_vlist0_multi_right_kw(  v, n,       1, 1)
#define           M_vt_vlist_required_right_kw(  v, n, pln)             M_vt_vlist_multi_right_kw(  v, n, pln,  1, 1)         // description for nested vlist must be passed in 
#define             M_vt_int_required_right_kw(  v, n)                    M_vt_int_multi_right_kw(  v, n,       1, 1)
#define           M_vt_arith_required_right_kw(  v, n)                  M_vt_arith_multi_right_kw(  v, n,       1, 1)
#define         M_vt_compare_required_right_kw(  v, n)                M_vt_compare_multi_right_kw(  v, n,       1, 1)
#define      M_vt_assigntype_required_right_kw(  v, n)             M_vt_assigntype_multi_right_kw(  v, n,       1, 1)
#define          M_vt_lvalue_required_right_kw(  v, n)                 M_vt_lvalue_multi_right_kw(  v, n,       1, 1)


//////// keywords in left-side plist  ///////////////////////////////////////////////////////

#define            M_vt_any_required_left_kw(  v, n)                    M_vt_any_multi_left_kw(   v, n,       1, 1)
#define           M_vt_nval_required_left_kw(  v, n)                   M_vt_nval_multi_left_kw(   v, n,       1, 1)
#define          M_vt_int64_required_left_kw(  v, n)                  M_vt_int64_multi_left_kw(   v, n,       1, 1)
#define        M_vt_int64rc_required_left_kw(  v, n, x, y)          M_vt_int64rc_multi_left_kw(   v, n, x, y, 1, 1)
#define        M_vt_float64_required_left_kw(  v, n)                M_vt_float64_multi_left_kw(   v, n,       1, 1)
#define      M_vt_float64rc_required_left_kw(  v, n, x, y)        M_vt_float64rc_multi_left_kw(   v, n, x, y, 1, 1)
#define         M_vt_string_required_left_kw(  v, n)                 M_vt_string_multi_left_kw(   v, n,       1, 1)
#define          M_vt_slist_required_left_kw(  v, n)                  M_vt_slist_multi_left_kw(   v, n,       1, 1)
#define        M_vt_verbdef_required_left_kw(  v, n)                M_vt_verbdef_multi_left_kw(   v, n,       1, 1)
#define         M_vt_typdef_required_left_kw(  v, n)                 M_vt_typdef_multi_left_kw(   v, n,       1, 1)
#define          M_vt_array_required_left_kw(  v, n)                  M_vt_array_multi_left_kw(   v, n,       1, 1)
#define         M_vt_struct_required_left_kw(  v, n)                 M_vt_struct_multi_left_kw(   v, n,       1, 1)
#define            M_vt_ref_required_left_kw(  v, n)                    M_vt_ref_multi_left_kw(   v, n,       1, 1)
#define      M_vt_var_ident_required_left_kw(  v, n)              M_vt_var_ident_multi_left_kw(   v, n,       1, 1)
#define         M_vt_lvalue_required_left_kw(  v, n)                 M_vt_lvalue_multi_left_kw(   v, n,       1, 1)
#define    M_vt_undef_ident_required_left_kw(  v, n)            M_vt_undef_ident_multi_left_kw(   v, n,       1, 1)
#define      M_vt_def_ident_required_left_kw(  v, n)              M_vt_def_ident_multi_left_kw(   v, n,       1, 1)
#define M_vt_nonconst_ident_required_left_kw(  v, n)         M_vt_nonconst_ident_multi_left_kw(   v, n,       1, 1)
#define      M_vt_any_ident_required_left_kw(  v, n)              M_vt_any_ident_multi_left_kw(   v, n,       1, 1)
#define      M_vt_raw_ident_required_left_kw(  v, n)              M_vt_raw_ident_multi_left_kw(   v, n,       1, 1)
#define         M_vt_vlist0_required_left_kw(  v, n)                 M_vt_vlist0_multi_left_kw(   v, n,       1, 1)
#define          M_vt_vlist_required_left_kw(  v, n, pln)             M_vt_vlist_multi_left_kw(   v, n, pln,  1, 1)         // description for nested vlist must be passed in
#define            M_vt_int_required_left_kw(  v, n)                    M_vt_int_multi_left_kw(   v, n,       1, 1)
#define          M_vt_arith_required_left_kw(  v, n)                  M_vt_arith_multi_left_kw(   v, n,       1, 1)
#define        M_vt_compare_required_left_kw(  v, n)                M_vt_compare_multi_left_kw(   v, n,       1, 1)
#define     M_vt_assigntype_required_left_kw(  v, n)             M_vt_assigntype_multi_left_kw(   v, n,       1, 1)
#define         M_vt_lvalue_required_left_kw(  v, n)                 M_vt_lvalue_multi_left_kw(   v, n,       1, 1)


//////// keywords in nested plist /////////////////////////////////////////////////

#define            M_vt_any_required_nest_kw( pl, n)                    M_vt_any_multi_nest_kw(  pl, n,       1, 1)
#define           M_vt_nval_required_nest_kw( pl, n)                   M_vt_nval_multi_nest_kw(  pl, n,       1, 1)
#define          M_vt_int64_required_nest_kw( pl, n)                  M_vt_int64_multi_nest_kw(  pl, n,       1, 1)
#define        M_vt_int64rc_required_nest_kw( pl, n, x, y)          M_vt_int64rc_multi_nest_kw(  pl, n, x, y, 1, 1)
#define        M_vt_float64_required_nest_kw( pl, n)                M_vt_float64_multi_nest_kw(  pl, n,       1, 1)
#define      M_vt_float64rc_required_nest_kw( pl, n, x, y)        M_vt_float64rc_multi_nest_kw(  pl, n, x, y, 1, 1)
#define         M_vt_string_required_nest_kw( pl, n)                 M_vt_string_multi_nest_kw(  pl, n,       1, 1)
#define          M_vt_slist_required_nest_kw( pl, n)                  M_vt_slist_multi_nest_kw(  pl, n,       1, 1)
#define        M_vt_verbdef_required_nest_kw( pl, n)                M_vt_verbdef_multi_nest_kw(  pl, n,       1, 1)
#define         M_vt_typdef_required_nest_kw( pl, n)                 M_vt_typdef_multi_nest_kw(  pl, n,       1, 1)
#define          M_vt_array_required_nest_kw( pl, n)                  M_vt_array_multi_nest_kw(  pl, n,       1, 1)
#define         M_vt_struct_required_nest_kw( pl, n)                 M_vt_struct_multi_nest_kw(  pl, n,       1, 1)
#define            M_vt_ref_required_nest_kw( pl, n)                    M_vt_ref_multi_nest_kw(  pl, n,       1, 1)
#define      M_vt_var_ident_required_nest_kw( pl, n)              M_vt_var_ident_multi_nest_kw(  pl, n,       1, 1)
#define         M_vt_lvalue_required_nest_kw( pl, n)                 M_vt_lvalue_multi_nest_kw(  pl, n,       1, 1)
#define    M_vt_undef_ident_required_nest_kw( pl, n)            M_vt_undef_ident_multi_nest_kw(  pl, n,       1, 1)
#define      M_vt_def_ident_required_nest_kw( pl, n)              M_vt_def_ident_multi_nest_kw(  pl, n,       1, 1)
#define M_vt_nonconst_ident_required_nest_kw( pl, n)         M_vt_nonconst_ident_multi_nest_kw(  pl, n,       1, 1)
#define      M_vt_any_ident_required_nest_kw( pl, n)              M_vt_any_ident_multi_nest_kw(  pl, n,       1, 1)
#define      M_vt_raw_ident_required_nest_kw( pl, n)              M_vt_raw_ident_multi_nest_kw(  pl, n,       1, 1)
#define         M_vt_vlist0_required_nest_kw( pl, n)                 M_vt_vlist0_multi_nest_kw(  pl, n,       1, 1)
#define          M_vt_vlist_required_nest_kw( pl, n, pln)             M_vt_vlist_multi_nest_kw(  pl, n, pln,  1, 1)         // description for nested vlist must be passed in 
#define            M_vt_int_required_nest_kw( pl, n)                    M_vt_int_multi_nest_kw(  pl, n,       1, 1)
#define          M_vt_arith_required_nest_kw( pl, n)                  M_vt_arith_multi_nest_kw(  pl, n,       1, 1)
#define        M_vt_compare_required_nest_kw( pl, n)                M_vt_compare_multi_nest_kw(  pl, n,       1, 1)
#define     M_vt_assigntype_required_nest_kw( pl, n)             M_vt_assigntype_multi_nest_kw(  pl, n,       1, 1)
#define         M_vt_lvalue_required_nest_kw( pl, n)                 M_vt_lvalue_multi_nest_kw(  pl, n,       1, 1)
 

///////////////////////////////////////////////////////////////////////////////////////////////
///// MACROs to add (multi-occurrence) keywords to verbdef (right/left-side) or to nested plist 
///////////////////////////////////////////////////////////////////////////////////////////////


//             right side multi-occurrence keywords ////////////////////////////////////////////////////////////////

#define            M_vt_any_multi_right_kw(v, n,         c1, c2)             M_vt_any_multi_x_kw( v, n,         c1, c2, r)
#define           M_vt_nval_multi_right_kw(v, n,         c1, c2)            M_vt_nval_multi_x_kw( v, n,         c1, c2, r)
#define          M_vt_int64_multi_right_kw(v, n,         c1, c2)           M_vt_int64_multi_x_kw( v, n,         c1, c2, r)
#define        M_vt_int64rc_multi_right_kw(v, n, lo, hi, c1, c2)         M_vt_int64rc_multi_x_kw( v, n, lo, hi, c1, c2, r)
#define        M_vt_float64_multi_right_kw(v, n,         c1, c2)         M_vt_float64_multi_x_kw( v, n,         c1, c2, r)
#define      M_vt_float64rc_multi_right_kw(v, n, lo, hi, c1, c2)       M_vt_float64rc_multi_x_kw( v, n, lo, hi, c1, c2, r)
#define         M_vt_string_multi_right_kw(v, n,         c1, c2)          M_vt_string_multi_x_kw( v, n,         c1, c2, r)
#define          M_vt_slist_multi_right_kw(v, n,         c1, c2)           M_vt_slist_multi_x_kw( v, n,         c1, c2, r)
#define        M_vt_verbdef_multi_right_kw(v, n,         c1, c2)         M_vt_verbdef_multi_x_kw( v, n,         c1, c2, r)
#define         M_vt_typdef_multi_right_kw(v, n,         c1, c2)          M_vt_typdef_multi_x_kw( v, n,         c1, c2, r)
#define          M_vt_array_multi_right_kw(v, n,         c1, c2)           M_vt_array_multi_x_kw( v, n,         c1, c2, r)
#define         M_vt_struct_multi_right_kw(v, n,         c1, c2)          M_vt_struct_multi_x_kw( v, n,         c1, c2, r)
#define            M_vt_ref_multi_right_kw(v, n,         c1, c2)             M_vt_ref_multi_x_kw( v, n,         c1, c2, r)
#define      M_vt_var_ident_multi_right_kw(v, n,         c1, c2)       M_vt_var_ident_multi_x_kw( v, n,         c1, c2, r)
#define         M_vt_lvalue_multi_right_kw(v, n,         c1, c2)          M_vt_lvalue_multi_x_kw( v, n,         c1, c2, r)
#define    M_vt_undef_ident_multi_right_kw(v, n,         c1, c2)     M_vt_undef_ident_multi_x_kw( v, n,         c1, c2, r)
#define      M_vt_def_ident_multi_right_kw(v, n,         c1, c2)       M_vt_def_ident_multi_x_kw( v, n,         c1, c2, r)
#define M_vt_nonconst_ident_multi_right_kw(v, n,         c1, c2)  M_vt_nonconst_ident_multi_x_kw( v, n,         c1, c2, r)
#define      M_vt_any_ident_multi_right_kw(v, n,         c1, c2)       M_vt_any_ident_multi_x_kw( v, n,         c1, c2, r)
#define      M_vt_raw_ident_multi_right_kw(v, n,         c1, c2)       M_vt_raw_ident_multi_x_kw( v, n,         c1, c2, r)
#define         M_vt_vlist0_multi_right_kw(v, n,         c1, c2)          M_vt_vlist0_multi_x_kw( v, n,         c1, c2, r)
#define          M_vt_vlist_multi_right_kw(v, n, pln,    c1, c2)           M_vt_vlist_multi_x_kw( v, n, pln,    c1, c2, r)
#define            M_vt_int_multi_right_kw(v, n,         c1, c2)             M_vt_int_multi_x_kw( v, n,         c1, c2, r)
#define          M_vt_arith_multi_right_kw(v, n,         c1, c2)           M_vt_arith_multi_x_kw( v, n,         c1, c2, r)
#define        M_vt_compare_multi_right_kw(v, n,         c1, c2)         M_vt_compare_multi_x_kw( v, n,         c1, c2, r)
#define     M_vt_assigntype_multi_right_kw(v, n,         c1, c2)      M_vt_assigntype_multi_x_kw( v, n,         c1, c2, r)
#define         M_vt_lvalue_multi_right_kw(v, n,         c1, c2)          M_vt_lvalue_multi_x_kw( v, n,         c1, c2, r)           


//          left side multi-occurrence keywords ////////////////////////////////////////////////////////////////////

#define            M_vt_any_multi_left_kw( v, n,         c1, c2)             M_vt_any_multi_x_kw( v, n,         c1, c2, l) 
#define           M_vt_nval_multi_left_kw( v, n,         c1, c2)            M_vt_nval_multi_x_kw( v, n,         c1, c2, l)
#define          M_vt_int64_multi_left_kw( v, n,         c1, c2)           M_vt_int64_multi_x_kw( v, n,         c1, c2, l)
#define        M_vt_int64rc_multi_left_kw( v, n, lo, hi, c1, c2)         M_vt_int64rc_multi_x_kw( v, n, lo, hi, c1, c2, l)
#define        M_vt_float64_multi_left_kw( v, n,         c1, c2)         M_vt_float64_multi_x_kw( v, n,         c1, c2, l)
#define      M_vt_float64rc_multi_left_kw( v, n, lo, hi, c1, c2)       M_vt_float64rc_multi_x_kw( v, n, lo, hi, c1, c2, l)
#define         M_vt_string_multi_left_kw( v, n,         c1, c2)          M_vt_string_multi_x_kw( v, n,         c1, c2, l)
#define          M_vt_slist_multi_left_kw( v, n,         c1, c2)           M_vt_slist_multi_x_kw( v, n,         c1, c2, l)
#define        M_vt_verbdef_multi_left_kw( v, n,         c1, c2)         M_vt_verbdef_multi_x_kw( v, n,         c1, c2, l)
#define         M_vt_typdef_multi_left_kw( v, n,         c1, c2)          M_vt_typdef_multi_x_kw( v, n,         c1, c2, l)
#define          M_vt_array_multi_left_kw( v, n,         c1, c2)           M_vt_array_multi_x_kw( v, n,         c1, c2, l)
#define         M_vt_struct_multi_left_kw( v, n,         c1, c2)          M_vt_struct_multi_x_kw( v, n,         c1, c2, l)
#define            M_vt_ref_multi_left_kw( v, n,         c1, c2)             M_vt_ref_multi_x_kw( v, n,         c1, c2, l)
#define      M_vt_var_ident_multi_left_kw( v, n,         c1, c2)       M_vt_var_ident_multi_x_kw( v, n,         c1, c2, l)
#define         M_vt_lvalue_multi_left_kw( v, n,         c1, c2)          M_vt_lvalue_multi_x_kw( v, n,         c1, c2, l)
#define    M_vt_undef_ident_multi_left_kw( v, n,         c1, c2)     M_vt_undef_ident_multi_x_kw( v, n,         c1, c2, l)
#define      M_vt_def_ident_multi_left_kw( v, n,         c1, c2)       M_vt_def_ident_multi_x_kw( v, n,         c1, c2, l)
#define M_vt_nonconst_ident_multi_left_kw( v, n,         c1, c2)  M_vt_nonconst_ident_multi_x_kw( v, n,         c1, c2, l)
#define      M_vt_any_ident_multi_left_kw( v, n,         c1, c2)       M_vt_any_ident_multi_x_kw( v, n,         c1, c2, l)
#define      M_vt_raw_ident_multi_left_kw( v, n,         c1, c2)       M_vt_raw_ident_multi_x_kw( v, n,         c1, c2, l)
#define         M_vt_vlist0_multi_left_kw( v, n,         c1, c2)          M_vt_vlist0_multi_x_kw( v, n,         c1, c2, l)
#define          M_vt_vlist_multi_left_kw( v, n, pln,    c1, c2)           M_vt_vlist_multi_x_kw( v, n, pln,    c1, c2, l)
#define            M_vt_int_multi_left_kw( v, n,         c1, c2)             M_vt_int_multi_x_kw( v, n,         c1, c2, l)
#define          M_vt_arith_multi_left_kw( v, n,         c1, c2)           M_vt_arith_multi_x_kw( v, n,         c1, c2, l)
#define        M_vt_compare_multi_left_kw( v, n,         c1, c2)         M_vt_compare_multi_x_kw( v, n,         c1, c2, l)
#define     M_vt_assigntype_multi_left_kw( v, n,         c1, c2)      M_vt_assigntype_multi_x_kw( v, n,         c1, c2, l) 
#define         M_vt_lvalue_multi_left_kw( v, n,         c1, c2)          M_vt_lvalue_multi_x_kw( v, n,         c1, c2, l)

//       nested multi-occurrence keywords //////////////////////////////////////////////////////////////////////////

#define            M_vt_any_multi_nest_kw(pl, n,         c1, c2)             M_vt_any_multi_x_kw(pl, n,         c1, c2, nested)
#define           M_vt_nval_multi_nest_kw(pl, n,         c1, c2)            M_vt_nval_multi_x_kw(pl, n,         c1, c2, nested)
#define          M_vt_int64_multi_nest_kw(pl, n,         c1, c2)           M_vt_int64_multi_x_kw(pl, n,         c1, c2, nested)
#define        M_vt_int64rc_multi_nest_kw(pl, n, lo, hi, c1, c2)         M_vt_int64rc_multi_x_kw(pl, n, lo, hi, c1, c2, nested)
#define        M_vt_float64_multi_nest_kw(pl, n,         c1, c2)         M_vt_float64_multi_x_kw(pl, n,         c1, c2, nested)
#define      M_vt_float64rc_multi_nest_kw(pl, n, lo, hi, c1, c2)       M_vt_float64rc_multi_x_kw(pl, n, lo, hi, c1, c2, nested)
#define         M_vt_string_multi_nest_kw(pl, n,         c1, c2)          M_vt_string_multi_x_kw(pl, n,         c1, c2, nested)
#define          M_vt_slist_multi_nest_kw(pl, n,         c1, c2)           M_vt_slist_multi_x_kw(pl, n,         c1, c2, nested)
#define        M_vt_verbdef_multi_nest_kw(pl, n,         c1, c2)         M_vt_verbdef_multi_x_kw(pl, n,         c1, c2, nested)
#define         M_vt_typdef_multi_nest_kw(pl, n,         c1, c2)          M_vt_typdef_multi_x_kw(pl, n,         c1, c2, nested)
#define          M_vt_array_multi_nest_kw(pl, n,         c1, c2)           M_vt_array_multi_x_kw(pl, n,         c1, c2, nested)
#define         M_vt_struct_multi_nest_kw(pl, n,         c1, c2)          M_vt_struct_multi_x_kw(pl, n,         c1, c2, nested)
#define            M_vt_ref_multi_nest_kw(pl, n,         c1, c2)             M_vt_ref_multi_x_kw(pl, n,         c1, c2, nested)
#define      M_vt_var_ident_multi_nest_kw(pl, n,         c1, c2)       M_vt_var_ident_multi_x_kw(pl, n,         c1, c2, nested)
#define         M_vt_lvalue_multi_nest_kw(pl, n,         c1, c2)          M_vt_lvalue_multi_x_kw(pl, n,         c1, c2, nested)
#define    M_vt_undef_ident_multi_nest_kw(pl, n,         c1, c2)     M_vt_undef_ident_multi_x_kw(pl, n,         c1, c2, nested)
#define      M_vt_def_ident_multi_nest_kw(pl, n,         c1, c2)       M_vt_def_ident_multi_x_kw(pl, n,         c1, c2, nested)
#define M_vt_nonconst_ident_multi_nest_kw(pl, n,         c1, c2)  M_vt_nonconst_ident_multi_x_kw(pl, n,         c1, c2, nested)
#define      M_vt_any_ident_multi_nest_kw(pl, n,         c1, c2)       M_vt_any_ident_multi_x_kw(pl, n,         c1, c2, nested)
#define      M_vt_raw_ident_multi_nest_kw(pl, n,         c1, c2)       M_vt_raw_ident_multi_x_kw(pl, n,         c1, c2, nested)
#define         M_vt_vlist0_multi_nest_kw(pl, n,         c1, c2)          M_vt_vlist0_multi_x_kw(pl, n,         c1, c2, nested)
#define          M_vt_vlist_multi_nest_kw(pl, n, pln,    c1, c2)           M_vt_vlist_multi_x_kw(pl, n, pln,    c1, c2, nested)
#define            M_vt_int_multi_nest_kw(pl, n,         c1, c2)             M_vt_int_multi_x_kw(pl, n,         c1, c2, nested)
#define          M_vt_arith_multi_nest_kw(pl, n,         c1, c2)           M_vt_arith_multi_x_kw(pl, n,         c1, c2, nested)
#define        M_vt_compare_multi_nest_kw(pl, n,         c1, c2)         M_vt_compare_multi_x_kw(pl, n,         c1, c2, nested)
#define     M_vt_assigntype_multi_nest_kw(pl, n,         c1, c2)      M_vt_assigntype_multi_x_kw(pl, n,         c1, c2, nested)
#define         M_vt_lvalue_multi_nest_kw(pl, n,         c1, c2)          M_vt_lvalue_multi_x_kw(pl, n,         c1, c2, nested)



///////////////////////////       left/right/nested multi-occurrence keywords    ////////////////////////////////////////////////////


//  multi-occurrence keyword of any type  

#define M_vt_any_multi_x_kw(pl, n, c1, c2, x)    \
{                                                \
    M_vt_multi_kw_parm(pt, c1, c2)               \
    pt.anything_ok = true;                       \
    M_vt_add_x_kw(n, pl, pt, x);                 \
}



//  multi-occurrence unit keyword  

#define M_vt_nval_multi_x_kw(pl, n, c1, c2, x)   \
{                                                \
    M_vt_multi_kw_parm(pt, c1, c2)               \
    pt.nval_ok = true;                           \
    M_vt_add_x_kw(n, pl, pt, x);                 \
}


//  multi-occurrence nested int64 keyword -- no range check 

#define M_vt_int64_multi_x_kw(pl, n, c1, c2, x)    \
{                                                  \
    M_vt_multi_kw_parm(pt, c1, c2)                 \
    pt.int64_ok = true;                            \
    M_vt_add_x_kw(n, pl, pt, x);                   \
}


//  multi-occurrence int64 keyword -- with range check 

#define M_vt_int64rc_multi_x_kw(pl, n, lo, hi, c1, c2, x) \
{                                                         \
    M_vt_multi_kw_parm(pt, c1, c2)                        \
    pt.int64_ok      = true;                              \
    pt.int64_range   = true;                              \
    pt.int64_min     = (lo);                              \
    pt.int64_max     = (hi);                              \
    M_vt_add_x_kw(n, pl, pt, x);                          \
}


// multi-occurrence float64 keyword -- no range check 

#define M_vt_float64_multi_x_kw(pl, n, c1, c2, x)     \
{                                                     \
    M_vt_multi_kw_parm(pt, c1, c2)                    \
    pt.float64_ok = true;                             \
    M_vt_add_x_kw(n, pl, pt, x);                      \
}


//  multi-occurrence float64 keyword -- with range check 

#define M_vt_float64rc_multi_x_kw(pl, n, lo, hi, c1, c2, x) \
{                                                           \
    M_vt_multi_kw_parm(pt, c1, c2)                          \
    pt.float64_ok      = true;                              \
    pt.float64_range   = true;                              \
    pt.float64_min     = (lo);                              \
    pt.float64_max     = (hi);                              \
    M_vt_add_x_kw(n, pl, pt, x);                            \
}


// multi-occurrence string keyword 

#define M_vt_string_multi_x_kw(pl, n, c1, c2, x)    \
{                                                   \
    M_vt_multi_kw_parm(pt, c1, c2)                  \
    pt.string_ok = true;                            \
    M_vt_add_x_kw(n, pl, pt, x);                    \
}                                                 


// multi-occurrence slist keyword 

#define M_vt_slist_multi_x_kw(pl, n, c1, c2, x)    \
{                                                  \
    M_vt_multi_kw_parm(pt, c1, c2)                 \
    pt.slist_ok = true;                            \
    M_vt_add_x_kw(n, pl, pt, x);                   \
}


// multi-occurrence verbdef keyword 

#define M_vt_verbdef_multi_x_kw(pl, n, c1, c2, x)  \
{                                                  \
    M_vt_multi_kw_parm(pt, c1, c2)                 \
    pt.verbdef_ok = true;                          \
    M_vt_add_x_kw(n, pl, pt, x);                   \
}  


// multi-occurrence typdef keyword 

#define M_vt_typdef_multi_x_kw(pl, n, c1, c2, x)   \
{                                                  \
    M_vt_multi_kw_parm(pt, c1, c2)                 \
    pt.typdef_ok = true;                           \
    M_vt_add_x_kw(n, pl, pt, x);                   \
}  


// multi-occurrence array keyword 

#define M_vt_array_multi_x_kw(pl, n, c1, c2, x)    \
{                                                  \
    M_vt_multi_kw_parm(pt, c1, c2)                 \
    pt.array_ok = true;                            \
    M_vt_add_x_kw(n, pl, pt, x);                   \
}  


// multi-occurrence struct keyword 

#define M_vt_struct_multi_x_kw(pl, n, c1, c2, x)   \
{                                                  \
    M_vt_multi_kw_parm(pt, c1, c2)                 \
    pt.structure_ok = true;                        \
    M_vt_add_x_kw(n, pl, pt, x);                   \
}  


// multi-occurrence ref keyword 

#define M_vt_ref_multi_x_kw(pl, n, c1, c2, x)      \
{                                                  \
    M_vt_multi_kw_parm(pt, c1, c2)                 \
    pt.lvalue_ref_ok = true;                       \
    pt.rvalue_ref_ok = true;                       \
    M_vt_add_x_kw(n, pl, pt, x);                   \
}  


// multi-occurrence (unevaluated) var_ident keyword 

#define M_vt_var_ident_multi_x_kw(pl, n, c1, c2, x)    \
{                                                      \
    M_vt_multi_kw_parm(pt, c1, c2)                     \
    pt.var_ident_ok  = true;                           \
    pt.no_eval_ident = true;                           \
    M_vt_add_x_kw(n, pl, pt, x);                       \
}


// lvalue -- multi-occurrence (unevaluated) var_ident keyword -or- (unevaluated) lvalue reference 

#define M_vt_lvalue_multi_x_kw(pl, n, c1, c2, x)       \
{                                                      \
    M_vt_multi_kw_parm(pt, c1, c2)                     \
    pt.var_ident_ok  = true;                           \
    pt.lvalue_ref_ok = true;                           \
    pt.no_eval_ident = true;                           \
    pt.no_eval_ref   = true;                           \
    M_vt_add_x_kw(n, pl, pt, x);                       \
}


// multi-occurrence (unevaluated) undef_ident keyword 

#define M_vt_undef_ident_multi_x_kw(pl, n, c1, c2, x)    \
{                                                        \
    M_vt_multi_kw_parm(pt, c1, c2)                       \
    pt.undef_ident_ok = true;                            \
    pt.no_eval_ident  = true;                            \
    M_vt_add_x_kw(n, pl, pt, x);                         \
}


// multi-occurrence (unevaluated) var_ident const_ident (defined ident) keyword 

#define M_vt_def_ident_multi_x_kw(pl, n, c1, c2, x)      \
{                                                        \
    M_vt_multi_kw_parm(pt, c1, c2)                       \
    pt.var_ident_ok = true;                              \
    pt.const_ident_ok = true;                            \
    pt.no_eval_ident  = true;                            \
    M_vt_add_x_kw(n, pl, pt, x);                         \
}


// multi-occurrence (unevaluated) undef_ident or var_ident keyword 

#define M_vt_nonconst_ident_multi_x_kw(pl, n, c1, c2, x)    \
{                                                           \
    M_vt_multi_kw_parm(pt, c1, c2)                          \
    pt.undef_ident_ok = true;                               \
    pt.var_ident_ok = true;                                 \
    pt.no_eval_ident  = true;                               \
    M_vt_add_x_kw(n, pl, pt, x);                            \
}


// multi-occurrence (unevaluated) undef_ident const_ident,or var_ident keyword 

#define M_vt_any_ident_multi_x_kw(pl, n, c1, c2, x)         \
{                                                           \
    M_vt_multi_kw_parm(pt, c1, c2)                          \
    pt.undef_ident_ok = true;                               \
    pt.var_ident_ok = true;                                 \
    pt.const_ident_ok = true;                               \
    pt.no_eval_ident  = true;                               \
    M_vt_add_x_kw(n, pl, pt, x);                            \
}


// multi-occurrence (unevaluated) raw_ident -- type is not checked 

#define M_vt_raw_ident_multi_x_kw(pl, n, c1, c2, x)         \
{                                                           \
    M_vt_multi_kw_parm(pt, c1, c2)                          \
    pt.raw_ident_ok = true;                                 \
    pt.no_eval_ident  = true;                               \
    M_vt_add_x_kw(n, pl, pt, x);                            \
}


// multi-occurrence vlist keyword (no passed-in vlist -- no nested parm descriptions)

#define M_vt_vlist0_multi_x_kw(pl, n, c1, c2, x)     \
{                                                    \
    M_vt_multi_kw_parm(pt, c1, c2)                   \
    pt.vlist_ok = true;                              \
    M_vt_add_x_kw(n, pl, pt, x);                     \
}


// multi-occurrence vlist keyword -- must pass in pre-built plist to be attached 

#define M_vt_vlist_multi_x_kw(pl, n, pln, c1, c2, x) \
{                                                    \
    M_vt_multi_kw_parm(pt, c1, c2)                   \
    pt.vlist_ok = true;                              \
    M_vt_add_nested_plist(pt, pln)                   \
    M_vt_add_x_kw(n, pl, pt, x)                      \
}


//  multi-occcurrence integer keyword    

#define M_vt_int_multi_x_kw(pl, n, c1, c2, x) \
{                                             \
    M_vt_multi_kw_parm(pt, c1, c2)            \
    pt.int8_ok        = true;                 \
    pt.int16_ok       = true;                 \
    pt.int32_ok       = true;                 \
    pt.int64_ok       = true;                 \
    pt.uint8_ok       = true;                 \
    pt.uint16_ok      = true;                 \
    pt.uint32_ok      = true;                 \
    pt.uint64_ok      = true;                 \
    M_vt_add_x_kw(n , pl, pt, x);             \
}


//  multi-occcurrence arithmetic keyword    

#define M_vt_arith_multi_x_kw(pl, n, c1, c2, x) \
{                                               \
    M_vt_multi_kw_parm(pt, c1, c2)              \
    pt.int8_ok        = true;                   \
    pt.int16_ok       = true;                   \
    pt.int32_ok       = true;                   \
    pt.int64_ok       = true;                   \
    pt.uint8_ok       = true;                   \
    pt.uint16_ok      = true;                   \
    pt.uint32_ok      = true;                   \
    pt.uint64_ok      = true;                   \
    pt.float32_ok     = true;                   \
    pt.float64_ok     = true;                   \
    M_vt_add_x_kw(n , pl, pt, x);               \
}


//  multi-occcurrence comparable keyword    

#define M_vt_compare_multi_x_kw(pl, n, c1, c2, x) \
{                                                 \
    M_vt_multi_kw_parm(pt, c1, c2)                \
    pt.unit_ok        = true;                     \
    pt.int8_ok        = true;                     \
    pt.int16_ok       = true;                     \
    pt.int32_ok       = true;                     \
    pt.int64_ok       = true;                     \
    pt.uint8_ok       = true;                     \
    pt.uint16_ok      = true;                     \
    pt.uint32_ok      = true;                     \
    pt.uint64_ok      = true;                     \
    pt.float32_ok     = true;                     \
    pt.float64_ok     = true;                     \
    pt.string_ok      = true;                     \
    M_vt_add_x_kw(n , pl, pt, x);                 \
}


//  multi-occurrence assignable keyword    

#define M_vt_assigntype_multi_x_kw(pl, n, c1, c2, x) \
{                                                    \
    M_vt_multi_kw_parm(pt, c1, c2)                   \
    pt.unit_ok        = true;                        \
    pt.int8_ok        = true;                        \
    pt.int16_ok       = true;                        \
    pt.int32_ok       = true;                        \
    pt.int64_ok       = true;                        \
    pt.uint8_ok       = true;                        \
    pt.uint16_ok      = true;                        \
    pt.uint32_ok      = true;                        \
    pt.uint64_ok      = true;                        \
    pt.float32_ok     = true;                        \
    pt.float64_ok     = true;                        \
    pt.string_ok      = true;                        \
    pt.undef_ident_ok = true;                        \
    pt.var_ident_ok   = true;                        \
    pt.const_ident_ok = true;                        \
    pt.vlist_ok       = true;                        \
    pt.slist_ok       = true;                        \
    pt.verbdef_ok     = true;                        \
    pt.typdef_ok      = true;                        \
    pt.array_ok       = true;                        \
    pt.structure_ok   = true;                        \
    pt.lvalue_ref_ok  = true;                        \
    pt.rvalue_ref_ok  = true;                        \
    M_vt_add_x_kw(n, pl, pt, x);                     \
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
//// Low-level  MACROs for setting up verbdef_S structures in verb_table
////
////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// add verb to verb table

#define M_vt_add(n, v, f)      \
{                              \
    v.fcn_p = (void *)&f;      \
    add_predefined_verb(n, v); \
}


#define M_vt_add_simplified(n, v, f)      \
{                                         \
    v.fcn_p = (void *)&f;                 \
    v.simplified_call = true;             \
    add_predefined_verb(n, v);            \
}


// ----------------------------------------------------------------------------
// MACROs for adding parms to nested or main plists 
// ----------------------------------------------------------------------------

#define M_vt_add_x_pos(  v, pt, x)       M_vt_add_##x##_pos(   v, pt)
#define M_vt_add_x_kw(n, v, pt, x)       M_vt_add_##x##_kw( n, v, pt)


// add pos or kw parm MACROs for unnested Expressions

#define M_vt_add_l_kw(n, v, pt) v.lparms.keywords.emplace(n, pt);
#define M_vt_add_r_kw(n, v, pt) v.rparms.keywords.emplace(n, pt);


#define M_vt_add_l_pos(v, pt)          \
{                                      \
    v.lparms.values.push_back(pt);     \
}

#define M_vt_add_r_pos(v, pt)          \
{                                      \
    v.rparms.values.push_back(pt);     \
}


// add pos or kw MACROs for nested Expressions

#define M_vt_add_nested_pos(pl, pt)     \
{                                       \
    pl.values.push_back(pt);            \
}


#define M_vt_add_nested_kw(n, pl, pt) pl.keywords.emplace(n, pt);


//------------------------------------------------------------------

#define M_vt_add_nested_plist(pt, pl)   \
{                                       \
    pt.vlist_ok = true;                 \
    (void)attach_plist(pt, pl);         \
}


// ----------------------------------------------------------------------------
// starting MACROs for nested plist_S with various positional parm requirements 
// ----------------------------------------------------------------------------

#define M_vt_nested_plist(pl, c1, c2) \
plist_S pl {};                        \
pl.min_ct = (c1);                     \
pl.max_ct = (c2);


// ------------------------------------------------------------------
// starting MACROs for parmtype_S with optional or required (kw) parm 
// ------------------------------------------------------------------
 
#define M_vt_pos_parm(pt) \
parmtype_S pt {};         \
pt.kw_min_ct = 0;         \
pt.kw_max_ct = 0;


#define M_vt_optional_kw_parm(pt) M_vt_multi_kw_parm(pt, 0, 1)
#define M_vt_required_kw_parm(pt) M_vt_multi_kw_parm(pt, 1, 1)


#define M_vt_multi_kw_parm(pt, c1, c2) \
parmtype_S pt {};                      \
pt.kw_min_ct = (c1);                   \
pt.kw_max_ct = (c2);
  

// -----------------------------------------------------------------------
// starting MACROs for verbdef_S with various positional parm requirements 
// -----------------------------------------------------------------------


// verb with no parms (nofix) 

#define M_vt_nofix(v) \
verbdef_S v {};       \
v.lparms.min_ct = 0;  \
v.lparms.max_ct = 0;  \
v.rparms.min_ct = 0;  \
v.rparms.max_ct = 0;


// prefix verb                    -- no leftside parms -- right-side positional parms = x to y

#define M_vt_prefix(v, x, y)       \
verbdef_S v {};                    \
v.lparms.min_ct = 0;               \
v.lparms.max_ct = 0;               \
v.rparms.min_ct = (x);             \
v.rparms.max_ct = (y);


// postfix verb                   -- no right-side parms -- left-side positional parms = x to y 

#define M_vt_postfix(v, x, y)      \
verbdef_S v {};                    \
v.lparms.min_ct = (x);             \
v.lparms.max_ct = (y);             \
v.rparms.min_ct = 0;               \
v.rparms.max_ct = 0;


// infix verb                    -- w  to  x  left-side parms --  y  to  z  right-side positional parms

#define M_vt_anyfix(v, w, x, y, z)  \
verbdef_S v {};                     \
v.lparms.min_ct = (w);              \
v.lparms.max_ct = (x);              \
v.rparms.min_ct = (y);              \
v.rparms.max_ct = (z);



 //////////// unary/binary verbs /////////////////////////


// unary prefix verb                -- 1 right-side parm

#define M_vt_unary_prefix(v)   \
verbdef_S v {};                \
v.lparms.min_ct = 0;           \
v.lparms.max_ct = 0;           \
v.rparms.min_ct = 1;           \
v.rparms.max_ct = 1;


// unary postfix verb               -- 1 left-side parm

#define M_vt_unary_postfix(v)  \
verbdef_S v {};                \
v.lparms.min_ct = 1;           \
v.lparms.max_ct = 1;           \
v.rparms.min_ct = 0;           \
v.rparms.max_ct = 0;


// binary infix verb                 -- 1 parm on each side

#define M_vt_binary_infix(v)   \
verbdef_S v {};                \
v.lparms.min_ct = 1;           \
v.lparms.max_ct = 1;           \
v.rparms.min_ct = 1;           \
v.rparms.max_ct = 1;



// unary anyfix                      -- no more than one parameter on any side (can have no parms, or one parm, or two parms) 

#define M_vt_unary_anyfix(v)   \
verbdef_S v {};                \
v.lparms.min_ct = 0;           \
v.lparms.max_ct = 1;           \
v.rparms.min_ct = 0;           \
v.rparms.max_ct = 1;


// unary prefix or postfix verb      -- single parm on one side only -- either side 

#define M_vt_unary_onefix(v)   \
verbdef_S v {};                \
v.parms_left_xor_right = true; \
v.lparms.min_ct = 0;           \
v.lparms.max_ct = 1;           \
v.rparms.min_ct = 0;           \
v.rparms.max_ct = 1;


// unary prefix or postfix or infix verb   -- one or both sides has one parm --  

#define M_vt_unary_somefix(v)  \
verbdef_S v {};                \
v.parms_some_required = true;  \
v.lparms.min_ct = 0;           \
v.lparms.max_ct = 1;           \
v.rparms.min_ct = 0;           \
v.rparms.max_ct = 1;
                               

// unary notinfix verb                 -- 1 parm on either side or no parms at all (but not parms on both sides)

#define M_vt_unary_notinfix(v) \
verbdef_S v {};                \
v.parms_not_both_sides = true; \
v.lparms.min_ct = 1;           \
v.lparms.max_ct = 1;           \
v.rparms.min_ct = 1;           \
v.rparms.max_ct = 1;



///////////////////// n-ary verbs  ///////////////////////////////////

// n-ary prefix verb                 -- 0 to N right-side positional parms, no left-side parms 

#define M_vt_nary_prefix(v)    \
verbdef_S v {};                \
v.lparms.min_ct = 0;           \
v.lparms.max_ct = 0;           \
v.rparms.min_ct = 0;           \
v.rparms.max_ct = -1;


// n-ary postfix verb                -- 0 to N left-side positional parms, no right-side parms

#define M_vt_nary_postfix(v)   \
verbdef_S v {};                \
v.lparms.min_ct = 0;           \
v.lparms.max_ct = -1;          \
v.rparms.min_ct = 0;           \
v.rparms.max_ct = 0;


// n-ary infix verb                  -- 1 to N left-side and 1 to N right-side parms   (parms on both sides)

#define M_vt_nary_infix(v)     \
verbdef_S v {};                \
v.lparms.min_ct = 1;           \
v.lparms.max_ct = -1;          \
v.rparms.min_ct = 1;           \
v.rparms.max_ct = -1;
                     
                                
// n-ary anyfix verb                  -- 0 to N left-side and/or right-side parms   (parms on no side, one side, or both sides) 

#define M_vt_nary_anyfix(v)    \
verbdef_S v {};                \
v.lparms.min_ct = 0;           \
v.lparms.max_ct = -1;          \
v.rparms.min_ct = 0;           \
v.rparms.max_ct = -1;


// n-ary somefix verb                  -- at least one positional parm, on either left or right sides  (or both sides)

#define M_vt_nary_somefix(v)   \
verbdef_S v {};                \
v.parms_some_required = true;  \
v.lparms.min_ct =  0;          \
v.lparms.max_ct = -1;          \
v.rparms.min_ct =  0;          \
v.rparms.max_ct = -1;


// n-ary onefix verb              -- at least one positional parm, on either left or right sides  (parms on one side or the other, but not both sides)

#define M_vt_nary_onefix(v)    \
verbdef_S v {};                \
v.parms_left_xor_right = true; \
v.lparms.min_ct =  0;          \
v.lparms.max_ct = -1;          \
v.rparms.min_ct =  0;          \
v.rparms.max_ct = -1;


// n-ary notinfix verb            -- 0-N positional parms on left side or right side, but not both sides

#define M_vt_nary_notinfix(v)  \
verbdef_S v {};                \
v.parms_not_both_sides = true; \
v.lparms.min_ct =  0;          \
v.lparms.max_ct = -1;          \
v.rparms.min_ct =  0;          \
v.rparms.max_ct = -1;






// -------------------------------------------
// MACROs for adding keywords to conflict sets 
// -------------------------------------------

// define std::set for a set of conflicting keywords

#define M_vt_conflict_set(s)  std::set<std::wstring> s {}; 


// add a keyword to the conflict set

#define M_vt_add_conflict_kw(s, n) s.insert(n);


// add conflict set to verbdef right/left plist

#define M_vt_add_r_conflict_set(v, s)  v.rparms.conflicts.push_back(s); 
#define M_vt_add_l_conflict_set(v, s)  v.lparms.conflicts.push_back(s);


// add conflict set to nested plist

#define M_vt_add_nested_conflict_set(p, s)  p.conflicts.push_back(s); 



// --------------------------------------------------
// MACROs for adding keywords to multiple choice sets 
// --------------------------------------------------

// define std::set for a set of multiple choice keywords

#define M_vt_choice_set(s)  std::set<std::wstring> s {}; 


// add a keyword to the multiple_choice set

#define M_vt_add_choice_kw(s, n) s.insert(n);


// add multiple choice set to verbdef right/left plist

#define M_vt_add_r_choice_set(v, s)  v.rparms.choices.push_back(s); 
#define M_vt_add_l_choice_set(v, s)  v.lparms.choices.push_back(s);


// add multiple choice set to nested plist

#define M_vt_add_nested_choice_set(p, s)  p.choices.push_back(s); 



// ----------------------------------------
// MACROs for adding keywords to match sets 
// ----------------------------------------

// define std::set for a set of matching keywords

#define M_vt_match_set(s)  std::set<std::wstring> s {}; 


// add a keyword to the match set

#define M_vt_add_match_kw(s, n) s.insert(n);


// add match set to verbdef right/left plist

#define M_vt_add_r_match_set(v, s)  v.rparms.matches.push_back(s); 
#define M_vt_add_l_match_set(v, s)  v.lparms.matches.push_back(s);


// add match set to nested plist

#define M_vt_add_nested_match_set(p, s)  p.matches.push_back(s); 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////