// h_audio.h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// 
////     =========
////     h_audio.h -- items for audio.cpp
////     =========
//// 
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "h_draw_types.h"               // audio is dependent on draw types


////////////////////////////////////////////////////////////////////////////
//   Manifest Constants
//////////////////////////////////////////////////////////////////////////// 

// Musical pitch -- 12-tone, A4=440hz
// -------------

#define M_440_C0      16.35
#define M_440_CD0     17.32
#define M_440_D0      18.35
#define M_440_DE0     19.45
#define M_440_E0      20.60
#define M_440_F0      21.83
#define M_440_FG0     23.12
#define M_440_G0      24.50
#define M_440_GA0     25.96
#define M_440_A0      27.50
#define M_440_AB0     29.14
#define M_440_B0      30.87
#define M_440_C1      32.70
#define M_440_CD1     34.65
#define M_440_D1      36.71
#define M_440_DE1     38.89
#define M_440_E1      41.20
#define M_440_F1      43.65
#define M_440_FG1     46.25
#define M_440_G1      49.00
#define M_440_GA1     51.91
#define M_440_A1      55.00
#define M_440_AB1     58.27
#define M_440_B1      61.74
#define M_440_C2      65.41
#define M_440_CD2     69.30
#define M_440_D2      73.42
#define M_440_DE2     77.78
#define M_440_E2      82.41
#define M_440_F2      87.31
#define M_440_FG2     92.50
#define M_440_G2      98.00
#define M_440_GA2    103.83
#define M_440_A2     110.00
#define M_440_AB2    116.54
#define M_440_B2     123.47
#define M_440_C3     130.81
#define M_440_CD3    138.59
#define M_440_D3     146.83
#define M_440_DE3    155.56
#define M_440_E3     164.81
#define M_440_F3     174.61
#define M_440_FG3    185.00
#define M_440_G3     196.00
#define M_440_GA3    207.65
#define M_440_A3     220.00
#define M_440_AB3    233.08
#define M_440_B3     246.94
#define M_440_C4     261.63
#define M_440_CD4    277.18
#define M_440_D4     293.66
#define M_440_DE4    311.13
#define M_440_E4     329.63
#define M_440_F4     349.23
#define M_440_FG4    369.99
#define M_440_G4     392.00
#define M_440_GA4    415.30
#define M_440_A4     440.00
#define M_440_AB4    466.16
#define M_440_B4     493.88
#define M_440_C5     523.25
#define M_440_CD5    554.37
#define M_440_D5     587.33
#define M_440_DE5    622.25
#define M_440_E5     659.25
#define M_440_F5     698.46
#define M_440_FG5    739.99
#define M_440_G5     783.99
#define M_440_GA5    830.61
#define M_440_A5     880.00
#define M_440_AB5    932.33
#define M_440_B5     987.77
#define M_440_C6    1046.50
#define M_440_CD6   1108.73
#define M_440_D6    1174.66
#define M_440_DE6   1244.51
#define M_440_E6    1318.51
#define M_440_F6    1396.91
#define M_440_FG6   1479.98
#define M_440_G6    1567.98
#define M_440_GA6   1661.22
#define M_440_A6    1760.00
#define M_440_AB6   1864.66
#define M_440_B6    1975.53
#define M_440_C7    2093.00
#define M_440_CD7   2217.46
#define M_440_D7    2349.32
#define M_440_DE7   2489.02
#define M_440_E7    2637.02
#define M_440_F7    2793.83
#define M_440_FG7   2959.96
#define M_440_G7    3135.96
#define M_440_GA7   3322.44
#define M_440_A7    3520.00
#define M_440_AB7   3729.31
#define M_440_B7    3951.07
#define M_440_C8    4186.01
#define M_440_CD8   4434.92
#define M_440_D8    4698.63
#define M_440_DE8   4978.03
#define M_440_E8    5274.04
#define M_440_F8    5587.65
#define M_440_FG8   5919.91
#define M_440_G8    6271.93
#define M_440_GA8   6644.88
#define M_440_A8    7040.00
#define M_440_AB8   7458.62
#define M_440_B8    7902.13
#define M_440_C9    8372.01
#define M_440_CD9   8869.84   
#define M_440_D9    9397.27   
#define M_440_DE9   9956.06   
#define M_440_E9   10548.08   
#define M_440_F9   11175.30   
#define M_440_FG9  11839.82   
#define M_440_G9   12543.85   
#define M_440_GA9  13289.75    
#define M_440_A9   14080.00
#define M_440_AB9  14917.24
#define M_440_B9   15804.26
#define M_440_C10  16744.02


// MIDI note numbers
// -----------------

#define M_12_C_1     0 
#define M_12_CD_1    1 
#define M_12_D_1     2 
#define M_12_DE_1    3 
#define M_12_E_1     4 
#define M_12_F_1     5 
#define M_12_FG_1    6 
#define M_12_G_1     7 
#define M_12_GA_1    8 
#define M_12_A_1     9 
#define M_12_AB_1   10 
#define M_12_B_1    11 
#define M_12_C0     12 
#define M_12_CD0    13 
#define M_12_D0     14 
#define M_12_DE0    15 
#define M_12_E0     16 
#define M_12_F0     17 
#define M_12_FG0    18 
#define M_12_G0     19 
#define M_12_GA0    20 
#define M_12_A0     21 
#define M_12_AB0    22 
#define M_12_B0     23 
#define M_12_C1     24 
#define M_12_CD1    25 
#define M_12_D1     26 
#define M_12_DE1    27 
#define M_12_E1     28 
#define M_12_F1     29 
#define M_12_FG1    30 
#define M_12_G1     31 
#define M_12_GA1    32 
#define M_12_A1     33 
#define M_12_AB1    34 
#define M_12_B1     35 
#define M_12_C2     36 
#define M_12_CD2    37 
#define M_12_D2     38 
#define M_12_DE2    39 
#define M_12_E2     40
#define M_12_F2     41 
#define M_12_FG2    42 
#define M_12_G2     43 
#define M_12_GA2    44
#define M_12_A2     45
#define M_12_AB2    46
#define M_12_B2     47
#define M_12_C3     48
#define M_12_CD3    49
#define M_12_D3     50
#define M_12_DE3    51
#define M_12_E3     52
#define M_12_F3     53
#define M_12_FG3    54
#define M_12_G3     55
#define M_12_GA3    56
#define M_12_A3     57
#define M_12_AB3    58
#define M_12_B3     59
#define M_12_C4     60
#define M_12_CD4    61
#define M_12_D4     62
#define M_12_DE4    63
#define M_12_E4     64
#define M_12_F4     65
#define M_12_FG4    66
#define M_12_G4     67
#define M_12_GA4    68
#define M_12_A4     69
#define M_12_AB4    70
#define M_12_B4     71
#define M_12_C5     72
#define M_12_CD5    73
#define M_12_D5     74
#define M_12_DE5    75
#define M_12_E5     76
#define M_12_F5     77
#define M_12_FG5    78
#define M_12_G5     79
#define M_12_GA5    80
#define M_12_A5     81
#define M_12_AB5    82
#define M_12_B5     83
#define M_12_C6     84
#define M_12_CD6    85
#define M_12_D6     86
#define M_12_DE6    87
#define M_12_E6     88
#define M_12_F6     89
#define M_12_FG6    90
#define M_12_G6     91
#define M_12_GA6    92
#define M_12_A6     93
#define M_12_AB6    94
#define M_12_B6     95
#define M_12_C7     96
#define M_12_CD7    97
#define M_12_D7     98
#define M_12_DE7    99
#define M_12_E7    100
#define M_12_F7    101
#define M_12_FG7   102
#define M_12_G7    103
#define M_12_GA7   104
#define M_12_A7    105
#define M_12_AB7   106
#define M_12_B7    107
#define M_12_C8    108
#define M_12_CD8   109
#define M_12_D8    110
#define M_12_DE8   111
#define M_12_E8    112
#define M_12_F8    113
#define M_12_FG8   114
#define M_12_G8    115
#define M_12_GA8   116
#define M_12_A8    117
#define M_12_AB8   118
#define M_12_B8    119
#define M_12_C9    120
#define M_12_CD9   121
#define M_12_D9    122
#define M_12_DE9   123
#define M_12_E9    124
#define M_12_F9    125
#define M_12_FG9   126
#define M_12_G9    127
#define M_12_GA9   128     // extended MIDI numbers
#define M_12_A9    129
#define M_12_AB9   130
#define M_12_B9    131
#define M_12_C10   132  


////////////////////////////////////////////////////////////////////////////////////
// structure definitions
////////////////////////////////////////////////////////////////////////////////////

// audio buffer
// ------------

struct audio_S
{
    real_T                            duration          {1.0  };    // duration of buffer (seconds)
    int32_t                           sample_rate       {48000};    // sample rate
    int32_t                           sample_depth      {16   };    // bits per sample (signed integer) -- don't change this
    int32_t                           channels          {1    };    // number of channels (1 or 2) -- don't change this
    int32_t                           sample_size       {2    };    // number of bytes in 1 sample -- don't change this 
    int32_t                           max_amplitude     {32767};    // maximum sample value + and -32767 is default -- don't change this 
    int32_t                           hi_ix             {0    };    // highest used index in buf
                                     
    buffer_C<real_T>                  buf               {     };    // PCM samples buffer -- must be allocated before use based on duration and sample rate  

    real_T                            pitch_stddev      {0.00 };    // 1.0-based standard deviation for note pitch
    real_T                            waveheight_stddev {0.00 };    // 1.0-based standard deviation for noise added to note waveheight
}; 


// envelope buffer
// ---------------

struct envelope_S
{
     real_T delay_time                {0.0 };                  // number of seconds of initial delay (with no sound) 
     real_T attack_time               {0.0 };                  // number of seconds for the attack
     real_T peak_time                 {0.0 };                  // time of peak intensity after attack, before decay 
     real_T decay_time                {0.0 };                  // number of seconds for the decay   
     real_T release_time              {0.0 };                  // number of seconds for the release 
                                     
     real_T peak                      {1.0 };                  // peak amplitude of attack/decay
     real_T sustain                   {1.0 };                  // amplitude of sustain

     buffer_C<real_T> ad_buf          {    };                  // attack/decay amplitude buffer          
     buffer_C<real_T> r_buf           {    };                  // release amplitude buffer 
};


// harmonic specification (base freq amplitude = 1.0) -- these are elements of a vector
// --------------------------------------------------

struct harmonic_S
{
    int32_t    split                              {1  };      // split this harmonic into this many subwaves (useful only with non-0 stddev settings ) 
    real_T     freq_mult                          {1.0};      // relative frequency -- 1.0 = main harmonic
    real_T     freq_stddev                        {0.0};      // (multiplicative) standard deviation of freq_mult (only used with multiple subwaves) 
    real_T     amplitude                          {0.0};      // relative amplitude of this harmonic (sum of n subwaves)
    real_T     amplitude_stddev                   {0.0};      // multiplicative stddev for relative amplitude of this harmonic (sum of n subwaves)
    real_T     wavelength_adj_stddev              {0.0};      // standard deviation of random adjustment of wavelength (in wave buffer)
    real_T     wavelength_adj_interval            {0.0};      // adjust wavelength (in wave buffer) after this many samples / freq_mult (used only with non-0 wavelength_adj_stddev)
    real_T     wavelength_adj_interval_stddev     {0.0};      // std dev for wavelength_adj_interval for this subwave
    real_T     phase_stddev                       {0.0};      // std deviation of initial phase for each harmonic     
};                                               


// musical scale
// -------------

enum class scale_pitch_E
{
    c_256,                                                    // C4 = 256 hz      (philosophical, scientific)
    a_415,                                                    // A4 = 415 hz      (baroque)   
    a_430,                                                    // A4 = 430 hz      (classical) 
    a_432,                                                    // A4 = 432 hz      ??? magical ??? 
    a_433,                                                    // A4 = 433 hz      (England Philharmonic Pitch 1813-1842) 
    a_435,                                                    // A4 = 435 hz      (Diapason Normal, Austrian 1885; French 1859)
    a_436,                                                    // A4 = 436 hz      (1878 London standard for church organs)
    a_439,                                                    // A4 = 439 hz      (England, New Philharmonic Pitch, 1896)
    a_440,                                                    // A4 = 440 hz      (stutgart pitch (1834), International standard (ISO 16) ISA 1939, ISO 1955, 1974; US 1917, 1926, 1936, concert pitch)
    a_441,                                                    // A4 = 441 hz      (Boston Symphony)
    a_442,                                                    // A4 = 442 hz      (New York Philharmonic, Denmark, France, Hungary, Italy, Norway, Switzerland)
    a_443,                                                    // A4 = 443 hz      (Orchestras in Germany, Austria, Rissua, Sweden, Spain)
    a_452_5,                                                  // A4 = 452.5 hz    (England, Old Philharmonic Pitch (Military bands 1890-1928)
    a_466                                                     // A4 = 466 hz      (Chorton, renaissance pitch)
};

enum class temperament_E
{
    edo_12,                                                   // 12-tone Equally Divided Octave (equal temperament) 
    pythag_aug4,                                              // 12-tone Pythagorean temperament                        -- keep augmented fourth, leave out diminished fifth 
    pythag_dim5,                                              // 12-tone Pythagorean temperament                        -- keep diminished fifth, leave out augmented fourth 
    ji_5limit,                                                // 12-tone 5-limit (asymmetric standard) just intonation  -- keep augmented fourth, leave out diminished fifth
    ji_5limit_ex,                                             // 12-tone 5-limit (asymmetric extended) just intonation  -- keep augmented fourth, leave out diminished fifth
    ji_7limit                                                 // 12-tone 7-limit just intonation                        -- keep augmented fourth, leave out diminished fifth
};



struct scale_S
{
    temperament_E        temperament               {         };      // type of scale temperment
    scale_pitch_E        pitch                     {         };      // basis pitch for this scale
    int32_t              lowest_note               {       0 };      // default = C_1
    int32_t              base_note                 {      60 };      // default = C4 
    int32_t              highest_note              {     132 };      // default = C10 
    std::vector<real_T>  pitches                   {         };      // empty vector -- tones need to be added
};  



// =================
// wave shape buffer
// =================
//
//  if any anharmonics, cycles should give 1-second or more
//  Number of samples per cycle should yield about 2 per sample rate
//
//
//       sample_rate      frequency          cycles needed     samples per cycle
//       -----------      ---------          -------------     -----------------
//       48000            16                 16                6144
//                        64                 64                1536
//                        256                256                384
//                        1024               1024                96
//                        4096               4096                24
//                        16384              16384                6
//
//
//    recommended wave.n and wave.cycles_main by octave: 
//
//
//    1) only harmonics present are exact multiples of fundamental frequency:    (for 48000 sample rate)
//
//
//    octave       wave.n      wave.cycles_main         (for 48000 sample rate)
//    ----------   ------      ----------------
//    16-32         6144+1          1.0
//    32-64         3072+1          1.0
//    64-128        1536+1          1.0
//    128-256        768+1          1.0
//    256-512        384+1          1.0
//    512-1024       192+1          1.0
//    1024-2048       96+1          1.0
//    2048-4096       48+1          1.0
//    4096-8192       24+1          1.0
//    8192-16384      12+1          1.0
//
//    General formula (supports all frequencies) wave.n      = 1 + (2 * sample_rate) / lowest_frequency
//                                               wace.cycles = 1
//
//    sample_rate    wave.n
//    -----------    ------
//     48000          6145
//     96000         12289
//    192000         24577
//
//
//    Anharmonic case:    (for 48000 sample rate)
//
//    octave       wave.n/cycles         wave.cycles_main    wave.n
//    ----------   -------------         ----------------    ---------
//    16-32         6144                        32.0         196608+1
//    32-64         3072                        64.0         196608+1
//    64-128        1536                       128.0         196608+1
//    128-256        768                       256.0         196608+1
//    256-512        384                       512.0         196608+1
//    512-1024       192                      1024.0         196608+1
//    1024-2048       96                      2048.0         196608+1
//    2048-4096       48                      4096.0         196608+1
//    4096-8192       24                      8192.0         196608+1
//    8192-16384      12                     16384.0         196608+1
//
//    general formula (for octave):  wave.cycles_main = max_octave_fequency
//                                   wave.n      = 1 + (2 * wave.cycles_main * sample_rate) / min_octave_frequency   = 1 + 4 * sample_rate * 2^number_of_octaves_covered 
//
//
//
//
//
//

struct wave_S
{
    int32_t                n                               {0     };    // total number of samples in buf  
    int32_t                n_main                          {0     };    // number of samples in main wave sample
    int32_t                n_splice                        {0     };    // number of samples in splice part of buffer (after main area) 
    int32_t                n_skip                          {0     };    // number of samples to generate at start, but don't save them in output wave buffer 
    real_T                 wavelength                      {1000.0};    // wavelength (in samples) of fundamental harmonic in wave buffer
    real_T                 cycles_main                     {1.0   };    // number of fundamental harmonic cycles in wave buffer 
    real_T                 cycles_splice                   {0.0   };    // number of fundamental harmonic cycles in splice portion of buffer 
    bool                   dc_adjust                       {false };    // true -- adjust waveform so first and last cycle have same DC offset
    buffer_C<real_T>       buf                             {      };    // buffer with samples 
};


// note buffer
// -----------

struct note_S
{
    real_T                 duration                        {1.0   };                                                                          
    real_T                 pitch                           {128.0 };
    real_T                 volume                          {5000.0};
    real_T                 wavelength                      {      };    // output from setup_note() 
    bool                   buf_ok                          {false };    // true -- buffer already allocated -- assume correct size
    buffer_C<real_T>       buf                             {      };    // buffer with samples at otput audio sample rate -- is allocated by setup_note() before use 
};


// Composite timbre  (waves and envelopes)
// ----------------

struct timbre_S
{
    int32_t                      ct                        {       };   // number of wave/envelope pairs saved in the vectors  
    std::vector<wave_S>          waves                     {       };   // wave_S components of this timbre
    std::vector<envelope_S>      envelopes                 {       };   // envelopes corresponding samples in waves
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//    ============
//    class rank_C -- one rank of pipes with pitch, loudness, timbre corresponding to each covered note number
//    ============    ----------------------------------------------------------------------------------------
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class rank_C
{ 
public:
                                rank_C()                   = default;   // default constructor -- need to use set_pitch() and add_timbre() below before using this rank_C
                                rank_C(const rank_C&)      = delete;    // copy constructor
                                rank_C(rank_C&&)           = delete;    // move constructor
                               ~rank_C()                   = default;   // default destructor
     rank_C&                    operator=(const rank_C&)   = delete;    // copy assignment
     rank_C&                    operator=(rank_C&&)        = delete;    // move assignment   


     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
     int32_t                    lowest_note                {  -1  };    // lowest  note for this rank 
     int32_t                    highest_note               {  -1  };    // highest note for this rank 

     std::vector<real_T>        loudness                   {      };    // relative loudness of this rank -- indexed by note   
     std::vector<real_T>        pitches                    {      };    // pitches for the notes covered by this rank -- with randomized pitches specific to this rank 
     std::vector<int32_t>       timbre_ix                  {      };    // indexes to timbre_S item in timbres vector for each note in range
     std::vector<timbre_S>      timbres                    {      };    // vector of composite timbres for this rank -- indexed by timbre_ix.at(note - lowest_note) 
     real_T                     add_stddev                 { 0.0  };    // additive       pitch standard deviation for each pitch in randomized scale  
     real_T                     mult_stddev                { 0.0  };    // multiplicatave pitch standard deviation for each pitch in randomized scale      
     bool                       pitch_set                  {false };    // true = set_pitch() has been done 
     bool                       timbre_set                 {false };    // true = timbre_ix completely fileld in 
 

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
     int                        clean_up();                                                             // cleanup all resources -- reset to empty state   
     int                        display();                                                              // display data items in this rank_C  
     int                        set_pitch(         const scale_S&,    int32_t, int32_t, real_T, real_T); // set up randomized pitches vector for this rank_S
     int                        add_timbre(        const timbre_S&,   int32_t, int32_t, real_T);         // add timbre and set timbre_ix  
     int                        note_info(int32_t, const timbre_S*&,  real_T&, real_T&) const;           // extract note_info from this rank, given input note number
};



/////////////////////////////////////////////////////////////////////////
//    External functions in audio.c
/////////////////////////////////////////////////////////////////////////

int  rescale_vector(            std::vector<real_T>&, real_T            ); 
int  rescale_vector(            std::vector<real_T>&, real_T,  real_T   ); 
int  minmax_value_vector(       std::vector<real_T>&, real_T&, real_T&  );
int  normalize_vector(          std::vector<real_T>&, real_T,  real_T   );

int  plot_graph(canvas_C&, const rgb_S&, rcf_T, rcf_T, rcf_T, rcf_T, const    buffer_C<real_T>&, uint32_t, uint32_t);
int  plot_graph(canvas_C&, const rgb_S&, rcf_T, rcf_T, rcf_T, rcf_T, const std::vector<real_T>&, uint32_t, uint32_t);


int r2c_dft(const std::vector<real_T>&, std::vector<real_T>&, std::vector<real_T>&);




int test_audio();              // ????????????????????????????????????????? temp ??????????????????????????????????




//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""