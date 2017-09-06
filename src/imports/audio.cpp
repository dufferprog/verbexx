// audio.cpp

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////    
////     ===========
////     audio.cpp
////     ===========
//// 
//// 
////     note: Invocation:   map /options ... --    usually just:    map   o=option_file.txt
//// 
////
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "h__include.h"
#pragma hdrstop("../pch/pch_std.pch")

#define M_IN_AUDIO_DLL

#include "h__common.h"

#include "h_draw_canvas.h"
#include "h_draw_window.h"

#include "h_audio.h"    

 


#define M_HARMONIC(v, mult, ampl, spl, multstd, amplstd, adjint, adjstd, adjintstd, phstd)                    \
{                                                                                                             \
   harmonic_S h {};                                                                                           \
   h.freq_mult                      =  (mult      );                                                          \
   h.freq_stddev                    =  (multstd   );                                                          \
   h.amplitude                      =  (ampl      );                                                          \
   h.amplitude_stddev               =  (amplstd   );                                                          \
   h.split                          =  (spl       );                                                          \
   h.wavelength_adj_interval        =  (adjint    );                                                          \
   h.wavelength_adj_stddev          =  (adjstd    );                                                          \
   h.wavelength_adj_interval_stddev =  (adjintstd );                                                          \
   h.phase_stddev                   =  (phstd  )   ;                                                          \
   v.push_back(h);                                                                                            \
}








////////////////////////////////////////////////////////////////////////////
//   internal static functions
////////////////////////////////////////////////////////////////////////////  

static int  play_audio(                                                                                                audio_S&);
static void fill_envelope(         envelope_S&,                                                                        audio_S&);
static void add_harmonics(         wave_S&, const std::vector<harmonic_S>&,                                            audio_S&);
                                                                                                                     
static int  setup_note(            note_S&, const timbre_S&,                                                           audio_S&);
static int  setup_note(            note_S&, const wave_S& , const envelope_S&,                                         audio_S&);
static int  add_note(              note_S&, real_T,                                                                    audio_S&);
static int  add_note(              const wave_S&, const envelope_S&, real_T, real_T,                   real_T, real_T, audio_S&);
static int  add_note(              const timbre_S&,                  real_T, real_T,                   real_T, real_T, audio_S&); 
static int  add_note(              const timbre_S&,                  real_T, real_T, const scale_S&,  int32_t, real_T, audio_S&);
static int  add_note(              const rank_C&,                    int32_t, real_T, real_T, real_T,                  audio_S&);


static int  normalize_buffer(      buffer_C<real_T>&,    real_T );
static int  rescale_buffer(        buffer_C<real_T>&,    real_T );  
static int  maxsample_buffer(      buffer_C<real_T>&,    real_T&);

static int  normalize_vector(      std::vector<real_T>&, real_T );
static int  init_scale(            scale_S&, scale_pitch_E = scale_pitch_E::a_440, temperament_E = temperament_E::edo_12, int32_t = M_12_C4); 


////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
////
////
////   test_audio()
////
////
////_____________________________________________________________________________________________________________________
////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
/////\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int test_audio() try
{
   


    // temporary windows test 
    // ----------------------

    canvas_C canvas1 {}; 
    canvas1.init(1500, 50200, 10);  

    window_C  w1 {}; 

   // w1.m_debug = 2;
    w1.init(1, 1, std::wstring{L"c1"}, std::wstring{L"w1"}); 
    w1.set_window_text(L"This is window w1"); 
    w1.attach_canvas(&canvas1); 
 
 
 
    w1.reset_xy();
  //  w1.display();
  //  M_out(L"---------------------------------------------------");

    w1.set_area(100.0, 200.0, 10200.0, 500.00);
    w1.set_xy_limits(-200.0, 50500.0, -0.1, 2.1);
    w1.display(); 

    w1.clear(); 
    w1.draw_circle(rgb_S { 111, 222, 0  }, 127,          111.0,  1.5,                  100.0      );
    w1.draw_ring(  rgb_S { 111, 222, 0  }, 127,  5.0,    211.0,  1.75,                  90.0      );
    w1.draw_box(   rgb_S { 111, 111, 0  }, 127,           11.0,  1.0,                  300.0,  0.5);
    w1.draw_frame( rgb_S { 222, 111, 0  }, 127,  10.0,   511.0,  0.9,                 1000.0, 0.75);
    w1.draw_line(  rgb_S { 222, 0, 222  }, 127,   7.5,   711.0,  1.11, 811.0,  1.555              );

    std::vector<xy_S> points; { };

    points.push_back(xy_S{2.5 * 110.10, 0.11100}); 
    points.push_back(xy_S{2.5 * 220.20, 1.2220}); 
    points.push_back(xy_S{2.5 * 330.30, 0.2330}); 
    points.push_back(xy_S{2.5 * 440.40, 1.1710}); 
    points.push_back(xy_S{2.5 * 550.50, 0.11100}); 
    points.push_back(xy_S{2.5 * 660.60, 2.0220}); 
    points.push_back(xy_S{2.5 * 770.70, 0.4330}); 
    points.push_back(xy_S{2.5 * 880.80, 1.1710}); 
    points.push_back(xy_S{2.5 * 990.90, 0.11100}); 
    points.push_back(xy_S{2.5 *1120.20, 2.01}); 
    points.push_back(xy_S{2.5 *1230.30, 0.2130}); 
    points.push_back(xy_S{2.5 *1340.40, 1.1110}); 
    points.push_back(xy_S{2.5 *1440.40, 1.1220}); 

    w1.draw_lines(          rgb_S { 255,    0,    0 }, 255,  20.0,   points); 
    w1.draw_curve(          rgb_S { 255,  255,    0 }, 127,  15.0,   points);
    w1.draw_beziers(        rgb_S { 255,    0,  255 }, 127,  10.0,   points);
    w1.draw_polygon(        rgb_S { 127,    0,  127 }, 127,   7.0,   points);
    w1.draw_closed_curve(   rgb_S { 127,  127,  127 }, 127,   5.0,   points);
    w1.fill_closed_curve(   rgb_S { 0,    255,  255 },  63,          points);
    w1.fill_polygon(        rgb_S { 0,    255,    0 },  63,          points);
   
    w1.show();
    message_loop();
    w1.term(); 
    canvas1.term();
    return 0; 
  

    // set up main audio buffer
    // ------------------------

    audio_S          audio    { }; // audio test parms -- use default values 
  
    audio.duration          = 1000.0; 
    audio.sample_rate       = 48000;
    audio.pitch_stddev      = 0.0;    // pitch in rank_S is pre-randomized 
    audio.waveheight_stddev = 0.0;

    audio.buf.alloc( 1 + (int32_t)(audio.duration * (real_T)(audio.sample_rate * audio.channels))  );
    audio.buf.clear();  


    ///////////// set up timbre_S //////////////////

   

   // build main envelope buffers 

   envelope_S envelope1 { }; 

   envelope1.peak          = 2.0; 
   envelope1.sustain       = 1.0;
   envelope1.attack_time   = 0.04;
   envelope1.peak_time     = 0.00;
   envelope1.decay_time    = 0.04;
   envelope1.release_time  = 0.75;

   fill_envelope(envelope1, audio);


   envelope_S envelope2 { }; 

   envelope2.peak          = 2.0; 
   envelope2.sustain       = 1.0;
   envelope2.attack_time   = 0.03;
   envelope2.peak_time     = 0.00;
   envelope2.decay_time    = 0.03;
   envelope2.release_time  = 0.50;

   fill_envelope(envelope2, audio);


   envelope_S envelope3 { }; 

   envelope3.peak          = 2.0; 
   envelope3.sustain       = 1.0;
   envelope3.attack_time   = 0.02;
   envelope3.peak_time     = 0.00;
   envelope3.decay_time    = 0.02;
   envelope3.release_time  = 0.25;

   fill_envelope(envelope3, audio);


  

   // build 1st wave buffer for use below 2K fundamental frequency (with 48000 sample rate)

 //  harmonic_S harmonic  {}; 

   // flute

  //  harmonic.freq_mult = 1.00; harmonic.amplitude = 1.00;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 2.00; harmonic.amplitude = 0.61;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 3.00; harmonic.amplitude = 0.10;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 4.00; harmonic.amplitude = 0.24;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 5.00; harmonic.amplitude = 0.11;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 6.00; harmonic.amplitude = 0.09;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 7.00; harmonic.amplitude = 0.00;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 8.00; harmonic.amplitude = 0.02;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult =11.00; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);

      // experimental flute -- 

  // harmonic.freq_mult =  1.00; harmonic.amplitude = 1.00;  harmonics.push_back(harmonic);
  //                      
  // harmonic.freq_mult =  1.90; harmonic.amplitude = 0.05;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  2.00; harmonic.amplitude = 0.61;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  2.10; harmonic.amplitude = 0.05;  harmonics.push_back(harmonic);
  //                      
  // harmonic.freq_mult =  3.00; harmonic.amplitude = 0.10;  harmonics.push_back(harmonic);
  // 
  // harmonic.freq_mult =  3.80; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  3.90; harmonic.amplitude = 0.03;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  4.00; harmonic.amplitude = 0.24;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  4.10; harmonic.amplitude = 0.03;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  4.20; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
  //                      
  // harmonic.freq_mult =  5.00; harmonic.amplitude = 0.11;  harmonics.push_back(harmonic);
  //                      
  // harmonic.freq_mult =  5.80; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  5.90; harmonic.amplitude = 0.03;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  6.00; harmonic.amplitude = 0.09;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  6.10; harmonic.amplitude = 0.03;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  6.20; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
  //                      
  //                      
  // harmonic.freq_mult =  7.00; harmonic.amplitude = 0.00;  harmonics.push_back(harmonic);
  //                      
  // harmonic.freq_mult =  7.90; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  8.00; harmonic.amplitude = 0.02;  harmonics.push_back(harmonic);
  // harmonic.freq_mult =  8.10; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
  //                      
  // harmonic.freq_mult =  9.00; harmonic.amplitude = 0.00;  harmonics.push_back(harmonic);
  //                      
  // harmonic.freq_mult = 10.00; harmonic.amplitude = 0.00;  harmonics.push_back(harmonic);
  //                      
  // harmonic.freq_mult = 10.90; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
  // harmonic.freq_mult = 11.00; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);


     // clarinet
     // harmonic.freq_mult =  1.0; harmonic.amplitude = 1.00;  harmonics.push_back(harmonic);
     // harmonic.freq_mult =  3.0; harmonic.amplitude = 0.75;  harmonics.push_back(harmonic);
     // harmonic.freq_mult =  5.0; harmonic.amplitude = 0.50;  harmonics.push_back(harmonic);
     // harmonic.freq_mult =  7.0; harmonic.amplitude = 0.14;  harmonics.push_back(harmonic);
     // harmonic.freq_mult =  9.0; harmonic.amplitude = 0.50;  harmonics.push_back(harmonic);
     // harmonic.freq_mult = 11.0; harmonic.amplitude = 0.12;  harmonics.push_back(harmonic);
     // harmonic.freq_mult = 13.0; harmonic.amplitude = 0.17;  harmonics.push_back(harmonic);

     // trumpet -- organ -- augmented 3rd harmonic
     //     harmonic.freq_mult =  1.0; harmonic.amplitude = 1.00;  harmonics.push_back(harmonic);
     //     harmonic.freq_mult =  2.0; harmonic.amplitude = 0.90;  harmonics.push_back(harmonic);
     //     harmonic.freq_mult =  3.0; harmonic.amplitude = 1.80;  harmonics.push_back(harmonic);  
     //     harmonic.freq_mult =  4.0; harmonic.amplitude = 0.30;  harmonics.push_back(harmonic);
     //     harmonic.freq_mult =  5.0; harmonic.amplitude = 0.20;  harmonics.push_back(harmonic);   
     //     harmonic.freq_mult =  6.0; harmonic.amplitude = 0.25;  harmonics.push_back(harmonic);
     //     harmonic.freq_mult =  7.0; harmonic.amplitude = 0.03;  harmonics.push_back(harmonic);   
     //     harmonic.freq_mult =  8.0; harmonic.amplitude = 0.09;  harmonics.push_back(harmonic);
     //     harmonic.freq_mult =  9.0; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
     //     harmonic.freq_mult = 10.0; harmonic.amplitude = 0.02;  harmonics.push_back(harmonic);
     //     harmonic.freq_mult = 11.0; harmonic.amplitude = 0.01;  harmonics.push_back(harmonic);
     //     harmonic.freq_mult = 12.0; harmonic.amplitude = 0.00;  harmonics.push_back(harmonic);

  
            // violin
  //  harmonic.freq_mult =  1.0; harmonic.amplitude = 1.00;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult =  2.0; harmonic.amplitude = 0.75;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult =  3.0; harmonic.amplitude = 1.80;  harmonics.push_back(harmonic);   // was 1.80
  //  harmonic.freq_mult =  4.0; harmonic.amplitude = 0.55;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult =  5.0; harmonic.amplitude = 0.20;  harmonics.push_back(harmonic);   // was 0.20
  //  harmonic.freq_mult =  6.0; harmonic.amplitude = 0.45;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult =  7.0; harmonic.amplitude = 0.03;  harmonics.push_back(harmonic);   // was .03
  //  harmonic.freq_mult =  8.0; harmonic.amplitude = 0.35;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult =  9.0; harmonic.amplitude = 0.30;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 10.0; harmonic.amplitude = 0.25;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 11.0; harmonic.amplitude = 0.25;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 12.0; harmonic.amplitude = 0.20;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 13.0; harmonic.amplitude = 0.15;  harmonics.push_back(harmonic);
  //  harmonic.freq_mult = 14.0; harmonic.amplitude = 0.15;  harmonics.push_back(harmonic);

  //    harmonic.freq_mult =  1.0; harmonic.amplitude = 1.000;  harmonics.push_back(harmonic);
 //     harmonic.freq_mult =  2.0; harmonic.amplitude = 0.5000;  harmonics.push_back(harmonic);
  //    harmonic.freq_mult =  3.0; harmonic.amplitude = 0.500;  harmonics.push_back(harmonic);
  //    harmonic.freq_mult =  4.0; harmonic.amplitude = 0.500;  harmonics.push_back(harmonic);
   //    harmonic.freq_mult =  5.0; harmonic.amplitude = 0.75;  harmonics.push_back(harmonic);
  //    harmonic.freq_mult =  6.0; harmonic.amplitude = 0.500;  harmonics.push_back(harmonic);
    //  harmonic.freq_mult =  7.0; harmonic.amplitude = 1.700;  harmonics.push_back(harmonic);
  //    harmonic.freq_mult =  8.0; harmonic.amplitude = 0.500;  harmonics.push_back(harmonic);
    //  harmonic.freq_mult =  9.0; harmonic.amplitude = 1.400;  harmonics.push_back(harmonic);
  //    harmonic.freq_mult = 10.0; harmonic.amplitude = 0.33;  harmonics.push_back(harmonic);
    //  harmonic.freq_mult = 11.0; harmonic.amplitude = 1.0;  harmonics.push_back(harmonic);
   //   harmonic.freq_mult = 12.0; harmonic.amplitude = 0.25;  harmonics.push_back(harmonic);
    //  harmonic.freq_mult = 13.0; harmonic.amplitude = 0.50;  harmonics.push_back(harmonic);
    //  harmonic.freq_mult = 14.0; harmonic.amplitude = 0.15;  harmonics.push_back(harmonic);

  
          // experimental flute

           std::vector<harmonic_S> harmonics1   {  };
           harmonics1.clear();

           //                       mult     ampl  split  mult_stddev   amp_stddev      adj_intv   adj_stddev  adjint_stddev     phase_stddev                  
         //    M_HARMONIC(harmonics1, 1.0,     1.0,   15.0,    0.00000,        0.0200,       200,      0.0400,     0.1500,            100.00         )
           //  M_HARMONIC(harmonics1, 2.0,     0.3,   15.0,    0.00000,        0.0200,       200,      0.0600,     0.1500,            100.00         )
          //   M_HARMONIC(harmonics1, 3.0,     0.5,   15.0,    0.00000,        0.0200,       200,      0.0400,     0.1500,            100.00         )
          //   M_HARMONIC(harmonics1, 4.0,     0.2,   15.0,    0.00000,        0.0200,       200,      0.0600,     0.1500,            100.00         )
          //   M_HARMONIC(harmonics1, 5.0,     0.7,   15.0,    0.00000,        0.0200,       200,      0.0400,     0.1500,            100.00         )
         //    M_HARMONIC(harmonics1, 6.0,     0.1,   15.0,    0.00000,        0.0200,       200,      0.0600,     0.1500,            100.00         )
           //  M_HARMONIC(harmonics1, 7.0,     0.5,   15.0,    0.00000,        0.0200,       200,      0.0400,     0.1500,            100.00         )
        //     M_HARMONIC(harmonics1, 8.0,     0.1,   15.0,    0.00000,        0.0200,       200,      0.0600,     0.1500,            100.00         )
           //  M_HARMONIC(harmonics1, 9.0,     0.4,   15.0,    0.00000,        0.0200,       200,      0.0400,     0.1500,            100.00         )


        //                          mult     ampl  split  mult_stddev     amp_stddev      adj_intv   adj_stddev  adjint_stddev     phase_stddev  
           //    M_HARMONIC(harmonics1, 1.20,     1.0,   1.0,    0.00000,        0.0000,       200,      0.0000,     0.1500,            00.00         )
          //   M_HARMONIC(harmonics1, 1.10,     1.0,   1.0,    0.00000,        0.0000,       200,      0.0000,     0.1500,            00.00         )
             M_HARMONIC(harmonics1, 1.0,     1.0,   10.0,    0.00000,        0.0000,       200,      0.04000,     0.1500,            00.00         )
          //   M_HARMONIC(harmonics1, 0.90,     1.0,   1.0,    0.00000,        0.0000,       200,      0.0000,     0.1500,            00.00         )
         //   M_HARMONIC(harmonics1, 0.80,     1.0,   1.0,    0.00000,        0.0000,       200,      0.0000,     0.1500,            00.00         )
          //   M_HARMONIC(harmonics1, 2.0,     0.25,   1.0,    0.00000,        0.0200,       200,      0.0000,     0.1500,            00.00         )
           //  M_HARMONIC(harmonics1, 2.1,     0.25,   1.0,    0.00000,        0.0200,       200,      0.0000,     0.1500,            00.00         )
             M_HARMONIC(harmonics1, 3.0,     0.17,   10.0,    0.00000,        0.0200,       200,      0.04000,     0.1500,            00.00         )
           //  M_HARMONIC(harmonics1, 3.15,     0.17,   1.0,    0.00000,        0.0200,       200,      0.0000,     0.1500,            0.00         )
          //   M_HARMONIC(harmonics1, 4.0,     0.13,   1.0,    0.00000,        0.0200,       200,      0.0000,     0.1500,            00.00         )
             M_HARMONIC(harmonics1, 5.0,     0.08,   10.0,    0.00000,        0.0200,       200,      0.04000,     0.1500,            00.00         )
          //   M_HARMONIC(harmonics1, 6.0,     0.08,   15.0,    0.00000,        0.0200,       200,      0.0100,     0.1500,            00.00         )
            M_HARMONIC(harmonics1, 7.0,     0.03,   10.0,    0.00000,        0.0200,       200,      0.0400,     0.1500,            00.00         )
          //   M_HARMONIC(harmonics1, 8.0,     0.06,   15.0,    0.00000,        0.0200,       200,      0.0100,     0.1500,            00.00         )
         //    M_HARMONIC(harmonics1, 9.0,     0.05,  15.0,    0.00000,        0.0200,       200,      0.0100,     0.1500,            00.00         )


              wave_S wave1 { };        
              wave1.cycles_main      = 200.0;    
              wave1.cycles_splice    = 199.0;    
              wave1.wavelength       = 5000.0; 
              wave1.n_skip           = 12345;   

              add_harmonics(wave1  , harmonics1  , audio);




            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // chiffing sounds
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

            // build chiff envelope buffers 
            // ----------------------------

            envelope_S envelope_c0 { };           
            envelope_c0.peak          = 0.08; 
            envelope_c0.sustain       = 0.0;
            envelope_c0.attack_time   = 0.15;
            envelope_c0.decay_time    = 0.20;
            envelope_c0.release_time  = 0.1;          
            fill_envelope(envelope_c0, audio); 
          
   
            envelope_S envelope_c1 { };           
            envelope_c1.peak          = 0.15; 
            envelope_c1.sustain       = 0.0;
            envelope_c1.attack_time   = 0.08;
            envelope_c1.decay_time    = 0.12;
            envelope_c1.release_time  = 0.1;          
            fill_envelope(envelope_c1, audio);

            envelope_S envelope_c2 { };           
            envelope_c2.peak          = 0.20; 
            envelope_c2.sustain       = 0.0;
            envelope_c2.attack_time   = 0.06;
            envelope_c2.decay_time    = 0.09;
            envelope_c2.release_time  = 0.1;          
            fill_envelope(envelope_c2, audio);          
   
            envelope_S envelope_c3 { };           
            envelope_c3.peak          = 0.25; 
            envelope_c3.sustain       = 0.0;
            envelope_c3.attack_time   = 0.04;
            envelope_c3.decay_time    = 0.06;
            envelope_c3.release_time  = 0.1;          
            fill_envelope(envelope_c3, audio);

            envelope_S envelope_c4 { };           
            envelope_c4.peak          = 0.20; 
            envelope_c4.sustain       = 0.0;
            envelope_c4.attack_time   = 0.03;
            envelope_c4.decay_time    = 0.03;
            envelope_c4.release_time  = 0.1;          
            fill_envelope(envelope_c4, audio);          
   
            envelope_S envelope_c5 { };           
            envelope_c5.peak          = 0.20; 
            envelope_c5.sustain       = 0.0;
            envelope_c5.attack_time   = 0.02;
            envelope_c5.decay_time    = 0.02;
            envelope_c5.release_time  = 0.1;          
            fill_envelope(envelope_c5, audio);
  





            // build chiff wave_S
            // ------------------


            std::vector<harmonic_S> harmonics_c0  {  };
            std::vector<harmonic_S> harmonics_c1  {  };
            std::vector<harmonic_S> harmonics_c2  {  };
            std::vector<harmonic_S> harmonics_c3  {  };
            std::vector<harmonic_S> harmonics_c4  {  };
            std::vector<harmonic_S> harmonics_c5  {  };



             //                        mult     ampl  split  mult_stddev     amp_stddev      adj_intv   adj_stddev  adjint_stddev     phase_stddev  
             harmonics_c0.clear();
             M_HARMONIC(harmonics_c0,  3.0,    1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )
             M_HARMONIC(harmonics_c0,  4.0,    1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )
             M_HARMONIC(harmonics_c0,  8.0,    1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )
             M_HARMONIC(harmonics_c0, 16.0,    1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )
             M_HARMONIC(harmonics_c0, 24.0,    1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.2000,            100.00         )
          //   M_HARMONIC(harmonics_c0, 64.0,    1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )
          //   M_HARMONIC(harmonics_c0,128.0,    1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )
                        
            harmonics_c1.clear();
             M_HARMONIC(harmonics_c1, 2.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1400,            100.00         )
             M_HARMONIC(harmonics_c1, 4.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1400,            100.00         )
             M_HARMONIC(harmonics_c1, 8.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1400,            100.00         )
             M_HARMONIC(harmonics_c1,16.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1400,            100.00         )
      //       M_HARMONIC(harmonics_c1,32.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )
          //   M_HARMONIC(harmonics_c1,64.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )

             harmonics_c2.clear();
             M_HARMONIC(harmonics_c2, 1.5,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1300,            100.00         )
             M_HARMONIC(harmonics_c2, 2.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1300,            100.00         )
             M_HARMONIC(harmonics_c2, 4.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1300,            100.00         )
             M_HARMONIC(harmonics_c2, 8.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1300,            100.00         )
          //   M_HARMONIC(harmonics_c2,12.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1300,            100.00         )
          //   M_HARMONIC(harmonics_c2,32.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )
               
            harmonics_c3.clear();
             M_HARMONIC(harmonics_c3, 1.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1200,            100.00         )
             M_HARMONIC(harmonics_c3, 2.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1200,            100.00         )
             M_HARMONIC(harmonics_c3, 4.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1200,            100.00         )
         //    M_HARMONIC(harmonics_c3, 8.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1200,            100.00         )
         //    M_HARMONIC(harmonics_c3,16.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1500,            100.00         )

             harmonics_c4.clear();
           //  M_HARMONIC(harmonics_c4, 0.5,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1000,            100.00         )
             M_HARMONIC(harmonics_c4, 1.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1000,            100.00         )
             M_HARMONIC(harmonics_c4, 2.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1000,            100.00         )
         //    M_HARMONIC(harmonics_c4, 4.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1000,            100.00         )
         //    M_HARMONIC(harmonics_c4, 6.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.1000,            100.00         )

             harmonics_c5.clear();
           //  M_HARMONIC(harmonics_c5, 0.25,    1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.0800,            100.00         )
          //   M_HARMONIC(harmonics_c5, 0.5,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.0800,            100.00         )
             M_HARMONIC(harmonics_c5, 1.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.0800,            100.00         )
         //    M_HARMONIC(harmonics_c5, 2.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.0800,            100.00         )
         //    M_HARMONIC(harmonics_c5, 4.0,     1.0,   15.0,    0.00000,        0.0300,      100,      0.1500,      0.0800,            100.00         )
           
                 
   
   
             wave_S wave_c0 { };  
             wave_c0.dc_adjust        = true;
             wave_c0.cycles_main      = 3.0;    
             wave_c0.cycles_splice    = 3.0;    
             wave_c0.wavelength       = 32000.0; 
             wave_c0.n_skip           = 12345;  
            
             wave_S wave_c1 { };  
             wave_c1.dc_adjust        = true;
             wave_c1.cycles_main      = 6.0;    
             wave_c1.cycles_splice    = 6.0;    
             wave_c1.wavelength       = 16000.0; 
             wave_c1.n_skip           = 12345;  

             wave_S wave_c2 { };  
             wave_c2.dc_adjust        = true;
             wave_c2.cycles_main      = 12.0;    
             wave_c2.cycles_splice    = 12.0;    
             wave_c2.wavelength       = 8000.0; 
             wave_c2.n_skip           = 12345;  
            
             wave_S wave_c3 { };  
             wave_c3.dc_adjust        = true;
             wave_c3.cycles_main      = 25.0;    
             wave_c3.cycles_splice    = 25.0;    
             wave_c3.wavelength       = 4000.0; 
             wave_c3.n_skip           = 12345;  

             wave_S wave_c4 { };  
             wave_c4.dc_adjust        = true;
             wave_c4.cycles_main      = 50.0;    
             wave_c4.cycles_splice    = 50.0;    
             wave_c4.wavelength       = 2000.0; 
             wave_c4.n_skip           = 12345;  
            
             wave_S wave_c5 { };  
             wave_c5.dc_adjust        = true;
             wave_c5.cycles_main      = 100.0;    
             wave_c5.cycles_splice    = 100.0;    
             wave_c5.wavelength       = 1000.0; 
             wave_c5.n_skip           = 12345; 

             add_harmonics(wave_c0  , harmonics_c0  , audio);
             add_harmonics(wave_c1  , harmonics_c1  , audio);
             add_harmonics(wave_c2  , harmonics_c2  , audio);
             add_harmonics(wave_c3  , harmonics_c3  , audio);
             add_harmonics(wave_c4  , harmonics_c4  , audio);
             add_harmonics(wave_c5  , harmonics_c5  , audio);


             // build timbres that combine chiff and main wave


             timbre_S timbre0   {    };
             timbre0.ct = 2; 
             timbre0.waves.push_back(wave1);
             timbre0.waves.push_back(wave_c0);
             timbre0.envelopes.push_back(envelope1);
             timbre0.envelopes.push_back(envelope_c0);

             timbre_S timbre1   {    };
             timbre1.ct = 2; 
             timbre1.waves.push_back(wave1);
             timbre1.waves.push_back(wave_c1);
             timbre1.envelopes.push_back(envelope2);
             timbre1.envelopes.push_back(envelope_c1);

             timbre_S timbre2   {    };
             timbre2.ct = 2; 
             timbre2.waves.push_back(wave1);
             timbre2.waves.push_back(wave_c2);
             timbre2.envelopes.push_back(envelope3);
             timbre2.envelopes.push_back(envelope_c2);

             timbre_S timbre3   {    };
             timbre3.ct = 2; 
             timbre3.waves.push_back(wave1);
             timbre3.waves.push_back(wave_c3);
             timbre3.envelopes.push_back(envelope3);
             timbre3.envelopes.push_back(envelope_c3);

             timbre_S timbre4   {    };
             timbre4.ct = 2; 
             timbre4.waves.push_back(wave1);
             timbre4.waves.push_back(wave_c4);
             timbre4.envelopes.push_back(envelope3);
             timbre4.envelopes.push_back(envelope_c4);

             timbre_S timbre5   {    };
             timbre5.ct = 2; 
             timbre5.waves.push_back(wave1);
             timbre5.waves.push_back(wave_c5);
             timbre5.envelopes.push_back(envelope3);
             timbre5.envelopes.push_back(envelope_c5);





    ///// set up ranks /////////////
   
    rank_C  rank1  {}; 
    scale_S scale1 {};

    //init_scale(scale1, scale_pitch_E::a_440, temperament_E::ji_7limit, M_12_C4);
      init_scale(scale1, scale_pitch_E::a_440, temperament_E::edo_12, M_12_C4);

    rank1.set_pitch(scale1, M_12_C0, M_12_C9, 0.5, 0.010);
    rank1.add_timbre(timbre0, M_12_C0, M_12_B0, 1.0);
    rank1.add_timbre(timbre1, M_12_C1, M_12_B1, 1.0);
    rank1.add_timbre(timbre2, M_12_C2, M_12_B2, 1.0);
    rank1.add_timbre(timbre3, M_12_C3, M_12_B3, 1.0);
    rank1.add_timbre(timbre4, M_12_C4, M_12_B4, 1.0);
    rank1.add_timbre(timbre5, M_12_C5, M_12_C9, 1.0);
    rank1.display();




 /*
       // do DFT plot

       std::vector<real_T> amplitudes   {}; 
       std::vector<real_T> phases       {};

       std::vector<real_T> values {}; 

       for (auto i = 0; i < wave1.n; i++)
       {
            values.push_back(wave2.buf.p[i]);
       }
       

       r2c_dft(values, amplitudes, phases);

             rgb_S rgb1 { 0, 0, 0};
             plot_graph( canvas1
                       , rgb1
                       , 50.0                // top
                       , 50.0                // left
                       , 950.0               // bottom
                       , 49950.0             // right
                       , amplitudes 
                       , 0
                       , 8000
                       );




    
       
              rgb_S rgb2 {255,0 , 0};


              rcf_T width_main        = 4000.0;
              rcf_T width_splice      = (wave1.cycles_splice / wave1.cycles_main) * width_main;  
              rcf_T wave_width        = width_main / wave1.cycles_main;  
  
          //    plot_graph(canvas1, rgb1, 50.0, 50.0, 950.0, 50.0 + width_main   , wave1.buf, 0           , wave1.n_main - 1); 
          //    plot_graph(canvas1, rgb2, 50.0, 50.0, 950.0, 50.0 + width_splice , wave1.buf, wave1.n_main, wave1.n - 1     ); 
         
      

               // initialize debug local note structure
          
        //     note_S notex {};
          
        //     notex.pitch       = M_440_C4;  
        //     notex.duration    = 5.0; 
        //     notex.volume      = 1.0;
          
        //     setup_note(notex, wave1, envelopex, audio);
          
          
        //     M_out(L"splice_ix1 = %d  wrap_ix1 = %d notex.wavelength = %.6f") % notex.splice_ix1 %  notex.wrap_ix1 % notex.wavelength; 

         //    rgb_S rgb3 {0,255 , 0}; 

          //   plot_graph( canvas1, rgb3, 50.0, 50.0, 950.0, 50.0 + 4.0 * width_splice , notex.buf 
          //             , notex.splice_ix1
          //             , notex.splice_ix1 + 4 * (notex.wrap_ix1 - notex.splice_ix1));
          

            


          //     plot_graph( canvas1, rgb3, 50.0, 50.0, 950.0, 50.0 + 70.0 * wave_width , notex.buf
           //              , 0
            //             , 70 * (int32_t)(notex.wavelength)
             //            );
   */

                             
  
   // build 2nd alternative wave for comparisons
    /*
       harmonics.clear();
       harmonic.phase_stddev            = 5.0;
       harmonic.wavelength_adj_interval = 500;
       harmonic.freq_mult =  1.0;  harmonic.amplitude = 1.0; harmonic.wavelength_adj_stddev = 0.0100;harmonic.split = 10; harmonics.push_back(harmonic);
    //   harmonic.freq_mult =  1.01;  harmonic.amplitude = 0.9; harmonic.wavelength_adj_stddev = 0.1400;harmonic.split = 10; harmonics.push_back(harmonic);
       harmonic.freq_mult =  3.0;  harmonic.amplitude = 1.0; harmonic.wavelength_adj_stddev = 0.0200;harmonic.split = 10; harmonics.push_back(harmonic);  
       harmonic.freq_mult =  5.0;  harmonic.amplitude = 1.0; harmonic.wavelength_adj_stddev = 0.0200;harmonic.split = 10; harmonics.push_back(harmonic);  
       harmonic.freq_mult =  7.0;  harmonic.amplitude = 0.5; harmonic.wavelength_adj_stddev = 0.0100;harmonic.split = 10; harmonics.push_back(harmonic);   
                                                                                                
   wave_S wave1a { };    // default initialize

   wave1a.cycles_main     = 60.0; 
   wave1a.cycles_splice   = 30.0; 
   wave1a.wavelength      = 1000.0;
 
   add_harmonics(wave1a, harmonics, audio);
   */

  /*
   // build 2nd wave buffer for 2K-4K fundamental frequency (with 48000 sample rate)

   harmonics.clear(); 

   harmonic.freq_mult = 2.0; harmonic.amplitude = 0.10;  harmonic.wavelength_adj_stddev = 0.0; harmonics.push_back(harmonic);
   harmonic.freq_mult = 3.0; harmonic.amplitude = 0.33;  harmonic.wavelength_adj_stddev = 0.0; harmonics.push_back(harmonic);
   harmonic.freq_mult = 4.0; harmonic.amplitude = 0.05;  harmonic.wavelength_adj_stddev = 0.0; harmonics.push_back(harmonic);

   wave_S wave2 { };                               // default initialize
   wave2.cycles      = 4.0; 
   wave2.wavelength  = audio.sample_rate / 128.0;

   add_harmonics(wave2, harmonics, audio);



 // build 3rd wave buffer for 4K-8K fundamental frequency (with 48000 sample rate)

   harmonics.clear(); 

   harmonic.freq_mult = 2.0; harmonic.amplitude = 0.10;  harmonics.push_back(harmonic);


   wave_S wave3 { };               // default initialize
   wave3.cycles      = 2.0; 
   wave3.wavelength  = audio.sample_rate / 256.0;  

   add_harmonics(wave3, harmonics, audio);
   */



   // test code to set up notes 
   // -------------------------

 //  note_S note1 {};     
 //  note_S note2 {}; 
   
 //  note1.duration = 10.0; 
 //  note2.duration = 10.0;

 //  setup_note(note1, wave1,  envelope1, audio);
 //  setup_note(note2, wave1a, envelope1, audio);

 //  for (auto i = 0; i < wave1a.n; i++)
 //  {
 //     //  if ( (i <= 300) || ( i % 1000 == 0) )
  //          M_out(L"i=%d  wave1[%d]=%.12f  wave1a[%d] = %.12f") % i % (i % wave1.n) %  wave1.buf.p[i % wave1.n] % (i % wave1a.n) % wave1a.buf.p[i % wave1a.n];
 //  }




 //  for (auto i = 0; i < note1.buf.sz; i++)
 //  {
 //       if ( (i <= 100) || ( i % 4000 == 0) )
 //           M_out(L"i=%d  note1=%.12f  note2 = %.12f") % i % note1.buf.p[i] % note2.buf.p[i];
 //  }





   // add notes using waveform and envelope data

   real_T t = 0.1; 
  
   real_T d = 2.0;     
   real_T e = 0.9*d;
/*  
   add_note(rank1, M_12_C0, t, e, 10000.00, audio);
   add_note(rank1, M_12_E0, t, e, 10000.00, audio);
   add_note(rank1, M_12_G0, t, e, 10000.00, audio);
   add_note(rank1, M_12_C1, t, e, 10000.00, audio);
   add_note(rank1, M_12_E1, t, e, 10000.00, audio);
   add_note(rank1, M_12_G1, t, e, 10000.00, audio);
   add_note(rank1, M_12_C2, t, e, 10000.00, audio);
   add_note(rank1, M_12_E2, t, e, 10000.00, audio);
   add_note(rank1, M_12_G2, t, e, 10000.00, audio);
   add_note(rank1, M_12_C3, t, e, 10000.00, audio);
   add_note(rank1, M_12_E3, t, e, 10000.00, audio);
   add_note(rank1, M_12_G3, t, e, 10000.00, audio);
   add_note(rank1, M_12_C4, t, e, 10000.00, audio);
   add_note(rank1, M_12_E4, t, e, 10000.00, audio);
   add_note(rank1, M_12_G4, t, e, 10000.00, audio);
   add_note(rank1, M_12_C5, t, e, 10000.00, audio);
   add_note(rank1, M_12_E5, t, e, 10000.00, audio);
   add_note(rank1, M_12_G5, t, e, 10000.00, audio);
   add_note(rank1, M_12_C6, t, e, 10000.00, audio);
   add_note(rank1, M_12_E6, t, e, 10000.00, audio);
   add_note(rank1, M_12_G6, t, e, 10000.00, audio);
   add_note(rank1, M_12_C7, t, e, 10000.00, audio);
   add_note(rank1, M_12_E7, t, e, 10000.00, audio);
   add_note(rank1, M_12_G7, t, e, 10000.00, audio);
  */

   d = 3.00; e = 0.5 * d;// t += 3.0       ;


   for (auto i=48; i <= 108; i++)
   {
       t += d ; 
       add_note(rank1, i, t, e, 7000.00, audio);
   /*    t += d ; 
       add_note(rank1, i+7, t, e, 7000.00, audio);

       t += d ; 
       add_note(rank1, i, t, e, 7000.00, audio);
       t += d ; 
       add_note(rank1, i+7, t, e, 7000.00, audio);
       t += d ; 
       add_note(rank1, i, t, e, 7000.00, audio);
       t += d ; 
       add_note(rank1, i+7, t, e, 7000.00, audio);

       t += d ; 
       add_note(rank1, i, t, e, 7000.00, audio);
       t += d ; 
       add_note(rank1, i+7, t, e, 7000.00, audio);
        t += d ; 
        add_note(rank1, i, t, e, 7000.00, audio);
        t += d ; 
        add_note(rank1, i+7, t, e, 7000.00, audio);
       
        t += d ; 
        add_note(rank1, i, t, e, 7000.00, audio);
        t += d ; 
        add_note(rank1, i+7, t, e, 7000.00, audio);
        t += d ; 
        add_note(rank1, i, t, e, 7000.00, audio);
        t += d ; 
        add_note(rank1, i+7, t, e, 7000.00, audio);
       
        t += d ; 
        add_note(rank1, i, t, e, 7000.00, audio);
        t += d ; 
        add_note(rank1, i+7, t, e, 7000.00, audio);
        */
   }



   /*
   add_note(timbre1, t, e, M_440_C0 , 10000.0, audio); add_note(timbre1, t, e, M_440_G0 , 10000.0, audio); add_note(timbre1, t , e, M_440_C1 , 10000.0, audio); add_note(timbre1, t, e, M_440_G1 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_C2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t , e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio);
   add_note(timbre1, t, e, M_440_E0 , 10000.0, audio); add_note(timbre1, t, e, M_440_AB0, 10000.0, audio); add_note(timbre1, t , e, M_440_E1 , 10000.0, audio); add_note(timbre1, t, e, M_440_AB1,  5000.0, audio); add_note(timbre1,  t , e, M_440_E2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_AB2,  5000.0, audio); add_note(timbre1, t, e, M_440_E3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_AB3,  5000.0, audio); add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_AB4 ,  5000.0, audio); add_note(timbre1, t , e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio);
   add_note(timbre1, t, e, M_440_C0 , 10000.0, audio); add_note(timbre1, t, e, M_440_G0 , 10000.0, audio); add_note(timbre1, t , e, M_440_C1 , 10000.0, audio); add_note(timbre1, t, e, M_440_G1 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_C2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t , e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio);
   add_note(timbre1, t, e, M_440_E0 , 10000.0, audio); add_note(timbre1, t, e, M_440_AB0, 10000.0, audio); add_note(timbre1, t , e, M_440_E1 , 10000.0, audio); add_note(timbre1, t, e, M_440_AB1,  5000.0, audio); add_note(timbre1,  t , e, M_440_E2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_AB2,  5000.0, audio); add_note(timbre1, t, e, M_440_E3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_AB3,  5000.0, audio); add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_AB4 ,  5000.0, audio); add_note(timbre1, t , e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio);  t += d ;
 
    d = 1.5;  
    e = 0.9*d;
   
   
   add_note(timbre1, t, e, M_440_CD0, 20000.0, audio); add_note(timbre1, t, e, M_440_GA0, 15000.0, audio); add_note(timbre1, t , e, M_440_CD1, 10000.0, audio); add_note(timbre1, t, e, M_440_GA1,  5000.0, audio); add_note(timbre1,  t , e, M_440_CD2,  5000.0, audio); add_note(timbre1, t, e, M_440_GA2,  5000.0, audio); add_note(timbre1, t, e, M_440_CD3,  5000.0, audio); add_note(timbre1, t, e, M_440_GA3,  5000.0, audio); add_note(timbre1, t, e, M_440_CD4,  5000.0, audio); add_note(timbre1, t, e, M_440_GA4,  5000.0, audio); add_note(timbre1, t , e, M_440_CD5,  5000.0, audio); add_note(timbre1, t, e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_D0 , 20000.0, audio); add_note(timbre1, t, e, M_440_A0 , 15000.0, audio); add_note(timbre1, t , e, M_440_D1 , 10000.0, audio); add_note(timbre1, t, e, M_440_A1 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_D2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A4 ,  5000.0, audio); add_note(timbre1, t , e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_DE0, 20000.0, audio); add_note(timbre1, t, e, M_440_AB0, 15000.0, audio); add_note(timbre1, t , e, M_440_DE1, 10000.0, audio); add_note(timbre1, t, e, M_440_AB1,  5000.0, audio); add_note(timbre1,  t , e, M_440_DE2,  5000.0, audio); add_note(timbre1, t, e, M_440_AB2,  5000.0, audio); add_note(timbre1, t, e, M_440_DE3,  5000.0, audio); add_note(timbre1, t, e, M_440_AB3,  5000.0, audio); add_note(timbre1, t, e, M_440_DE4,  5000.0, audio); add_note(timbre1, t, e, M_440_AB4,  5000.0, audio); add_note(timbre1, t , e, M_440_DE5,  5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_E0 , 20000.0, audio); add_note(timbre1, t, e, M_440_B0 , 15000.0, audio); add_note(timbre1, t , e, M_440_E1 , 10000.0, audio); add_note(timbre1, t, e, M_440_B1 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_E2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B4 ,  5000.0, audio); add_note(timbre1, t , e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_F0 , 20000.0, audio); add_note(timbre1, t, e, M_440_C1 , 15000.0, audio); add_note(timbre1, t , e, M_440_F1 , 10000.0, audio); add_note(timbre1, t, e, M_440_C2 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_F2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t , e, M_440_F5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_FG0, 20000.0, audio); add_note(timbre1, t, e, M_440_CD1, 15000.0, audio); add_note(timbre1, t , e, M_440_FG1, 10000.0, audio); add_note(timbre1, t, e, M_440_CD2,  5000.0, audio); add_note(timbre1,  t , e, M_440_FG2,  5000.0, audio); add_note(timbre1, t, e, M_440_CD3,  5000.0, audio); add_note(timbre1, t, e, M_440_FG3,  5000.0, audio); add_note(timbre1, t, e, M_440_CD4,  5000.0, audio); add_note(timbre1, t, e, M_440_FG4,  5000.0, audio); add_note(timbre1, t, e, M_440_CD5,  5000.0, audio); add_note(timbre1, t , e, M_440_FG5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_G0 , 20000.0, audio); add_note(timbre1, t, e, M_440_D1 , 15000.0, audio); add_note(timbre1, t , e, M_440_G1 , 10000.0, audio); add_note(timbre1, t, e, M_440_D2 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_G2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t , e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_GA0, 20000.0, audio); add_note(timbre1, t, e, M_440_DE1, 15000.0, audio); add_note(timbre1, t , e, M_440_GA1, 10000.0, audio); add_note(timbre1, t, e, M_440_DE2,  5000.0, audio); add_note(timbre1,  t , e, M_440_GA2,  5000.0, audio); add_note(timbre1, t, e, M_440_DE3,  5000.0, audio); add_note(timbre1, t, e, M_440_GA3,  5000.0, audio); add_note(timbre1, t, e, M_440_DE4,  5000.0, audio); add_note(timbre1, t, e, M_440_GA4,  5000.0, audio); add_note(timbre1, t, e, M_440_DE5,  5000.0, audio); add_note(timbre1, t , e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio); add_note(timbre1, t, e, M_440_GA6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_A0 , 20000.0, audio); add_note(timbre1, t, e, M_440_E1 , 15000.0, audio); add_note(timbre1, t , e, M_440_A1 , 10000.0, audio); add_note(timbre1, t, e, M_440_E2 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_A2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t , e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_AB0, 20000.0, audio); add_note(timbre1, t, e, M_440_F1,  15000.0, audio); add_note(timbre1, t , e, M_440_AB1, 10000.0, audio); add_note(timbre1, t, e, M_440_F2,   5000.0, audio); add_note(timbre1,  t , e, M_440_AB2,  5000.0, audio); add_note(timbre1, t, e, M_440_F3,   5000.0, audio); add_note(timbre1, t, e, M_440_AB3,  5000.0, audio); add_note(timbre1, t, e, M_440_F4,   5000.0, audio); add_note(timbre1, t, e, M_440_AB4,  5000.0, audio); add_note(timbre1, t, e, M_440_F5,   5000.0, audio); add_note(timbre1, t , e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_F6,   5000.0, audio); add_note(timbre1, t, e, M_440_AB6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_B0 , 20000.0, audio); add_note(timbre1, t, e, M_440_FG1, 15000.0, audio); add_note(timbre1, t , e, M_440_B1 , 10000.0, audio); add_note(timbre1, t, e, M_440_FG2,  5000.0, audio); add_note(timbre1,  t , e, M_440_B2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG3,  5000.0, audio); add_note(timbre1, t, e, M_440_B3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG4,  5000.0, audio); add_note(timbre1, t, e, M_440_B4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG5,  5000.0, audio); add_note(timbre1, t , e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio); add_note(timbre1, t, e, M_440_B6 ,  5000.0, audio);  t += d ;
   
    d = 1.0;  
    e = 0.8*d;

   add_note(timbre1, t , e, M_440_C1 , 10000.0,audio); add_note(timbre1, t, e, M_440_G1 ,  5000.0, audio); add_note(timbre1, t , e, M_440_C2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G2 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_C3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_CD1, 10000.0,audio); add_note(timbre1, t, e, M_440_GA1,  5000.0, audio); add_note(timbre1, t , e, M_440_CD2,  5000.0, audio); add_note(timbre1, t, e, M_440_GA2,  5000.0, audio); add_note(timbre1,  t , e, M_440_CD3,  5000.0, audio); add_note(timbre1, t, e, M_440_GA3,  5000.0, audio); add_note(timbre1, t, e, M_440_CD4,  5000.0, audio); add_note(timbre1, t, e, M_440_GA4,  5000.0, audio); add_note(timbre1, t, e, M_440_CD5,  5000.0, audio); add_note(timbre1, t, e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_D1 , 10000.0,audio); add_note(timbre1, t, e, M_440_A1 ,  5000.0, audio); add_note(timbre1, t , e, M_440_D2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A2 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_D3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_DE1, 10000.0,audio); add_note(timbre1, t, e, M_440_AB1,  5000.0, audio); add_note(timbre1, t , e, M_440_DE2,  5000.0, audio); add_note(timbre1, t, e, M_440_AB2,  5000.0, audio); add_note(timbre1,  t , e, M_440_DE3,  5000.0, audio); add_note(timbre1, t, e, M_440_AB3,  5000.0, audio); add_note(timbre1, t, e, M_440_DE4,  5000.0, audio); add_note(timbre1, t, e, M_440_AB4,  5000.0, audio); add_note(timbre1, t, e, M_440_DE5,  5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_E1 , 10000.0,audio); add_note(timbre1, t, e, M_440_B1 ,  5000.0, audio); add_note(timbre1, t , e, M_440_E2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B2 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_E3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_F1 , 10000.0,audio); add_note(timbre1, t, e, M_440_C2 ,  5000.0, audio); add_note(timbre1, t , e, M_440_F2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_F3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_FG1, 10000.0,audio); add_note(timbre1, t, e, M_440_CD2,  5000.0, audio); add_note(timbre1, t , e, M_440_FG2,  5000.0, audio); add_note(timbre1, t, e, M_440_CD3,  5000.0, audio); add_note(timbre1,  t , e, M_440_FG3,  5000.0, audio); add_note(timbre1, t, e, M_440_CD4,  5000.0, audio); add_note(timbre1, t, e, M_440_FG4,  5000.0, audio); add_note(timbre1, t, e, M_440_CD5,  5000.0, audio); add_note(timbre1, t, e, M_440_FG5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_G1 , 10000.0,audio); add_note(timbre1, t, e, M_440_D2 ,  5000.0, audio); add_note(timbre1, t , e, M_440_G2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_G3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_GA1, 10000.0,audio); add_note(timbre1, t, e, M_440_DE2,  5000.0, audio); add_note(timbre1, t , e, M_440_GA2,  5000.0, audio); add_note(timbre1, t, e, M_440_DE3,  5000.0, audio); add_note(timbre1,  t , e, M_440_GA3,  5000.0, audio); add_note(timbre1, t, e, M_440_DE4,  5000.0, audio); add_note(timbre1, t, e, M_440_GA4,  5000.0, audio); add_note(timbre1, t, e, M_440_DE5,  5000.0, audio); add_note(timbre1, t, e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio); add_note(timbre1, t, e, M_440_GA6,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_A1 , 10000.0,audio); add_note(timbre1, t, e, M_440_E2 ,  5000.0, audio); add_note(timbre1, t , e, M_440_A2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_A3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_AB1, 10000.0,audio); add_note(timbre1, t, e, M_440_F2,   5000.0, audio); add_note(timbre1, t , e, M_440_AB2,  5000.0, audio); add_note(timbre1, t, e, M_440_F3,   5000.0, audio); add_note(timbre1,  t , e, M_440_AB3,  5000.0, audio); add_note(timbre1, t, e, M_440_F4,   5000.0, audio); add_note(timbre1, t, e, M_440_AB4,  5000.0, audio); add_note(timbre1, t, e, M_440_F5,   5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_F6,   5000.0, audio); add_note(timbre1, t, e, M_440_AB6,  5000.0, audio);  t += d ;
   add_note(timbre1, t , e, M_440_B1 , 10000.0,audio); add_note(timbre1, t, e, M_440_FG2,  5000.0, audio); add_note(timbre1, t , e, M_440_B2 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG3,  5000.0, audio); add_note(timbre1,  t , e, M_440_B3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG4,  5000.0, audio); add_note(timbre1, t, e, M_440_B4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG5,  5000.0, audio); add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio); add_note(timbre1, t, e, M_440_B6 ,  5000.0, audio);  t += d ;
 
    d = 0.50;  
    e = 0.7*d;

   add_note(timbre1, t, e, M_440_C2 ,  8000.0, audio); add_note(timbre1, t, e, M_440_G2 ,  5000.0, audio); add_note(timbre1, t , e, M_440_C3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_C4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_CD2,  8000.0, audio); add_note(timbre1, t, e, M_440_GA2,  5000.0, audio); add_note(timbre1, t , e, M_440_CD3,  5000.0, audio); add_note(timbre1, t, e, M_440_GA3,  5000.0, audio); add_note(timbre1,  t , e, M_440_CD4,  5000.0, audio); add_note(timbre1, t, e, M_440_GA4,  5000.0, audio); add_note(timbre1, t, e, M_440_CD5,  5000.0, audio); add_note(timbre1, t, e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_D2 ,  8000.0, audio); add_note(timbre1, t, e, M_440_A2 ,  5000.0, audio); add_note(timbre1, t , e, M_440_D3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_D4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_DE2,  8000.0, audio); add_note(timbre1, t, e, M_440_AB2,  5000.0, audio); add_note(timbre1, t , e, M_440_DE3,  5000.0, audio); add_note(timbre1, t, e, M_440_AB3,  5000.0, audio); add_note(timbre1,  t , e, M_440_DE4,  5000.0, audio); add_note(timbre1, t, e, M_440_AB4,  5000.0, audio); add_note(timbre1, t, e, M_440_DE5,  5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_E2 ,  8000.0, audio); add_note(timbre1, t, e, M_440_B2 ,  5000.0, audio); add_note(timbre1, t , e, M_440_E3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_F2 ,  8000.0, audio); add_note(timbre1, t, e, M_440_C3 ,  5000.0, audio); add_note(timbre1, t , e, M_440_F3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_F4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_FG2,  8000.0, audio); add_note(timbre1, t, e, M_440_CD3,  5000.0, audio); add_note(timbre1, t , e, M_440_FG3,  5000.0, audio); add_note(timbre1, t, e, M_440_CD4,  5000.0, audio); add_note(timbre1,  t , e, M_440_FG4,  5000.0, audio); add_note(timbre1, t, e, M_440_CD5,  5000.0, audio); add_note(timbre1, t, e, M_440_FG5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_G2 ,  8000.0, audio); add_note(timbre1, t, e, M_440_D3 ,  5000.0, audio); add_note(timbre1, t , e, M_440_G3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_GA2,  8000.0, audio); add_note(timbre1, t, e, M_440_DE3,  5000.0, audio); add_note(timbre1, t , e, M_440_GA3,  5000.0, audio); add_note(timbre1, t, e, M_440_DE4,  5000.0, audio); add_note(timbre1,  t , e, M_440_GA4,  5000.0, audio); add_note(timbre1, t, e, M_440_DE5,  5000.0, audio); add_note(timbre1, t, e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio); add_note(timbre1, t, e, M_440_GA6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_A2 ,  8000.0, audio); add_note(timbre1, t, e, M_440_E3 ,  5000.0, audio); add_note(timbre1, t , e, M_440_A3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_A4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_AB2,  8000.0, audio); add_note(timbre1, t, e, M_440_F3,   5000.0, audio); add_note(timbre1, t , e, M_440_AB3,  5000.0, audio); add_note(timbre1, t, e, M_440_F4,   5000.0, audio); add_note(timbre1,  t , e, M_440_AB4,  5000.0, audio); add_note(timbre1, t, e, M_440_F5,   5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_F6,   5000.0, audio); add_note(timbre1, t, e, M_440_AB6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_B2 ,  8000.0, audio); add_note(timbre1, t, e, M_440_FG3,  5000.0, audio); add_note(timbre1, t , e, M_440_B3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG4,  5000.0, audio); add_note(timbre1,  t , e, M_440_B4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG5,  5000.0, audio); add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio); add_note(timbre1, t, e, M_440_B6 ,  5000.0, audio); t += d ;
     
    d = 0.375;  
    e = 0.7*d;

   add_note(timbre1, t, e, M_440_C3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_C4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_CD3,  5000.0, audio); add_note(timbre1, t, e, M_440_GA3,  5000.0, audio); add_note(timbre1,  t , e, M_440_CD4,  5000.0, audio); add_note(timbre1, t, e, M_440_GA4,  5000.0, audio); add_note(timbre1,  t , e, M_440_CD5,  5000.0, audio); add_note(timbre1, t, e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_D3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_D4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_DE3,  5000.0, audio); add_note(timbre1, t, e, M_440_AB3,  5000.0, audio); add_note(timbre1,  t , e, M_440_DE4,  5000.0, audio); add_note(timbre1, t, e, M_440_AB4,  5000.0, audio); add_note(timbre1,  t , e, M_440_DE5,  5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_E3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B3 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_F3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_F4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C5 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_F5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_F6 ,  5000.0, audio);  t += d ;   
   add_note(timbre1, t, e, M_440_FG3,  5000.0, audio); add_note(timbre1, t, e, M_440_CD4,  5000.0, audio); add_note(timbre1,  t , e, M_440_FG4,  5000.0, audio); add_note(timbre1, t, e, M_440_CD5,  5000.0, audio); add_note(timbre1,  t , e, M_440_FG5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_G3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D5 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_GA3,  5000.0, audio); add_note(timbre1, t, e, M_440_DE4,  5000.0, audio); add_note(timbre1,  t , e, M_440_GA4,  5000.0, audio); add_note(timbre1, t, e, M_440_DE5,  5000.0, audio); add_note(timbre1,  t , e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio); add_note(timbre1, t, e, M_440_GA6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_A3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_A4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E5 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A6 ,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_AB3,  5000.0, audio); add_note(timbre1, t, e, M_440_F4,   5000.0, audio); add_note(timbre1,  t , e, M_440_AB4,  5000.0, audio); add_note(timbre1, t, e, M_440_F5,   5000.0, audio); add_note(timbre1,  t , e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_F6,   5000.0, audio); add_note(timbre1, t, e, M_440_AB6,  5000.0, audio);  t += d ;
   add_note(timbre1, t, e, M_440_B3 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG4,  5000.0, audio); add_note(timbre1,  t , e, M_440_B4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG5,  5000.0, audio); add_note(timbre1,  t , e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio); add_note(timbre1, t, e, M_440_B6 ,  5000.0, audio);  t += d ;
   
    d = 0.250;  
    e = 0.7*d;

   add_note(timbre1, t, e, M_440_C4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio);  add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_CD4,  5000.0, audio); add_note(timbre1, t, e, M_440_GA4,  5000.0, audio); add_note(timbre1,  t , e, M_440_CD5,  5000.0, audio); add_note(timbre1, t, e, M_440_GA5,  5000.0, audio);  add_note(timbre1, t, e, M_440_CD6,  5000.0, audio); t += d ;  
   add_note(timbre1, t, e, M_440_D4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio);  add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_DE4,  5000.0, audio); add_note(timbre1, t, e, M_440_AB4,  5000.0, audio); add_note(timbre1,  t , e, M_440_DE5,  5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio);  add_note(timbre1, t, e, M_440_DE6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_E4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B4 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio);  add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_F4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C5 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_F5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio);  add_note(timbre1, t, e, M_440_F6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_FG4,  5000.0, audio); add_note(timbre1, t, e, M_440_CD5,  5000.0, audio); add_note(timbre1,  t , e, M_440_FG5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio);  add_note(timbre1, t, e, M_440_FG6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_G4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D5 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio);  add_note(timbre1, t, e, M_440_G6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_GA4,  5000.0, audio); add_note(timbre1, t, e, M_440_DE5,  5000.0, audio); add_note(timbre1,  t , e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio);  add_note(timbre1, t, e, M_440_GA6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_A4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E5 ,  5000.0, audio); add_note(timbre1,  t , e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio);  add_note(timbre1, t, e, M_440_A6 ,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_AB4,  5000.0, audio); add_note(timbre1, t, e, M_440_F5,   5000.0, audio); add_note(timbre1,  t , e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_F6,   5000.0, audio);  add_note(timbre1, t, e, M_440_AB6,  5000.0, audio); t += d ;
   add_note(timbre1, t, e, M_440_B4 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG5,  5000.0, audio); add_note(timbre1,  t , e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio);  add_note(timbre1, t, e, M_440_B6 ,  5000.0, audio); t += d ;
  
    d = 0.1250;  
    e = 0.7*d;

   add_note(timbre1, t, e, M_440_C5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1,  t, e, M_440_C6 ,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_CD5,  5000.0, audio); add_note(timbre1, t, e, M_440_GA5,  5000.0, audio); add_note(timbre1,  t, e, M_440_CD6,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_D5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio); add_note(timbre1,  t, e, M_440_D6 ,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_DE5,  5000.0, audio); add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1,  t, e, M_440_DE6,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_E5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio); add_note(timbre1,  t, e, M_440_E6 ,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_F5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio); add_note(timbre1,  t, e, M_440_F6 ,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_FG5,  5000.0, audio); add_note(timbre1, t, e, M_440_CD6,  5000.0, audio); add_note(timbre1,  t, e, M_440_FG6,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_G5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio); add_note(timbre1,  t, e, M_440_G6 ,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_GA5,  5000.0, audio); add_note(timbre1, t, e, M_440_DE6,  5000.0, audio); add_note(timbre1,  t, e, M_440_GA6,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_A5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio); add_note(timbre1,  t, e, M_440_A6 ,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_AB5,  5000.0, audio); add_note(timbre1, t, e, M_440_F6,   5000.0, audio); add_note(timbre1,  t, e, M_440_AB6,  5000.0, audio);   t += d ;
   add_note(timbre1, t, e, M_440_B5 ,  5000.0, audio); add_note(timbre1, t, e, M_440_FG6,  5000.0, audio); add_note(timbre1,  t, e, M_440_B6 ,  5000.0, audio);   t += d ;
  
    d = 0.0625;  
    e = 0.7*d;

   add_note(timbre1, t, e, M_440_C6 ,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_CD6,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_D6 ,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_DE6,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_E6 ,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_F6 ,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_FG6,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_G6 ,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_GA6,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_A6 ,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_AB6,  5000.0, audio);    t += d ;
   add_note(timbre1, t, e, M_440_B6 ,  5000.0, audio);    t += d ;
   */

  /*
   t += e; 

   real_T f1 {M_440_C4}; 
   real_T f2 {M_440_D4};
   real_T f3 {M_440_E4};

   f2 = f1;  f3 = f1; 

   real_T h1 {d}; 
   real_T h2 {e}; 
   */

  // h1 = d; 
  // h2 = e; 
   /*
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f3       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f3       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f3       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f3       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f3       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f3       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f3       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f3       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
   add_note(wave1,  envelope1, t += h1, h2, f1       ,  4000.0, audio);
   add_note(wave1a, envelope1, t += h1, h2, f2       ,  4000.0, audio);
     */


   /*
   add_note(wave1, envelope1, t += d, e, M_440_C6 ,  3000.0, audio);
   add_note(wave1, envelope1, t += d, e, M_440_CD6,  3000.0, audio);
   add_note(wave1, envelope1, t += d, e, M_440_D6 ,  3000.0, audio);
   add_note(wave1, envelope1, t += d, e, M_440_DE6,  3000.0, audio);
   add_note(wave1, envelope1, t += d, e, M_440_E6 ,  3000.0, audio);
   add_note(wave1, envelope1, t += d, e, M_440_F6 ,  3000.0, audio);
   add_note(wave1, envelope1, t += d, e, M_440_FG6,  3000.0, audio);     */
   /*
   add_note(wave2, envelope1, t += d, e, M_440_G6 ,  3000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_GA6,  3000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_A6 ,  3000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_AB6,  3000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_B6 ,  3000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_C7 ,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_CD7,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_D7 ,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_DE7,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_E7 ,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_F7 ,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_FG7,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_G7 ,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_GA7,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_A7 ,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_AB7,  2000.0, audio);
   add_note(wave2, envelope1, t += d, e, M_440_B7 ,  2000.0, audio);
   */
   /*
   add_note(wave3, envelope1, t += d, e, M_440_C8 , 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_CD8, 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_D8 , 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_DE8, 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_E8 , 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_F8 , 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_FG8, 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_G8 , 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_GA8, 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_A8 , 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_AB8, 5000.0, audio);
   add_note(wave3, envelope1, t += d, e, M_440_B8 , 5000.0, audio);
    */             
  
   real_T f {0.5};
   real_T g {1.5};

 //  add_note(wave1, envelope1,t+=f,10.0,M_440_C0, 16000.0, audio); 
 //  add_note(wave1, envelope1,t+=f,10.0,M_440_E0, 14000.0, audio); 
 //  add_note(wave1, envelope1,t+=f,10.0,M_440_G0, 12000.0, audio); 
// add_note(wave1, envelope1,t+=g,30.0,M_440_C1, 9000.0, audio); 
// add_note(wave1, envelope1,t+=f,30.0,M_440_E1 ,6000.0, audio); 
// add_note(wave1, envelope1,t+=f,30.0,M_440_G1 ,4000.0, audio); 
// add_note(wave1, envelope1,t+=g,30.0,M_440_C2 ,3500.0, audio);     
// add_note(wave1, envelope1,t+=f,30.0,M_440_E2 ,2500.0, audio);     
// add_note(wave1, envelope1,t+=f,30.0,M_440_G2 ,2000.0, audio); 
// add_note(wave1, envelope1,t+=g,30.0,M_440_C3 ,1800.0, audio); 
// add_note(wave1, envelope1,t+=f,30.0,M_440_E3 ,1500.0, audio);     
// add_note(wave1, envelope1,t+=f,30.0,M_440_G3 ,1200.0, audio); 
// add_note(wave1, envelope1,t+=g,30.0,M_440_C4 ,1000.0, audio); 
// add_note(wave1, envelope1,t+=f,30.0,M_440_E4 , 800.0, audio); 
// add_note(wave1, envelope1,t+=f,30.0,M_440_G4 , 600.0, audio); 
// add_note(wave1, envelope1,t+=g,30.0,M_440_C5 , 400.0, audio); 
// add_note(wave1, envelope1,t+=f,30.0,M_440_E5 , 300.0, audio); 
// add_note(wave1, envelope1,t+=f,30.0,M_440_G5 , 275.0, audio); 
// add_note(wave2, envelope1,t+=g,30.0,M_440_C6 , 250.0, audio); 
 //  add_note(wave2, envelope1,t+=f,30.0,M_440_E6 , 225.0, audio); 
 //  add_note(wave2, envelope1,t+=f,30.0,M_440_G6 , 200.0, audio); 
 //  add_note(wave2, envelope1,t+=f,30.0,M_440_C7 , 150.0, audio); 
 



   /*        
         w1.show();
       //  w2.show(); 
         message_loop();
                  
         w1.term(); 
         w2.term(); 
         canvas1.term(); 
    */




   auto rc = play_audio(audio);
   return rc;  
}  
M_endf




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




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   play_audio() -- play the data in the main audio buffer
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int play_audio(audio_S& audio) try
{
    int rc {0}; 


    // return, if audio buffer is not allocated
    // ----------------------------------------

    if (audio.buf.sz <= 0)
        return -1; 


    //  normalize main audio buffer -- convert to 16-bit samples for playing
    //  --------------------------------------------------------------------

    normalize_buffer(audio.buf, 20000.0);   // can't get too close to 32767 for some reason

    buffer_C<int16_t> buf {  };
    buf.alloc(audio.buf.sz);

    for (auto i = 0; i < buf.sz; i++)
    {
        buf.p[i] = (int16_t)(audio.buf.p[i]);
    }
    

    // Set up WAVEFORMATEX structure based on filled-in audio structure
    // ----------------------------------------------------------------

    WAVEFORMATEX wfx { };  
 
    wfx.nSamplesPerSec   = audio.sample_rate;     // sample rate 
    wfx.wBitsPerSample   = audio.sample_depth;    // sample size 
    wfx.nChannels        = audio.channels;        // channels
    wfx.cbSize           = 0;                     // no _extra_ info 
    wfx.wFormatTag       = WAVE_FORMAT_PCM;
    wfx.nBlockAlign      = (wfx.wBitsPerSample/8) * wfx.nChannels;
    wfx.nAvgBytesPerSec  = wfx.nBlockAlign * wfx.nSamplesPerSec;


    // open default audio device and save audio handle
    // -----------------------------------------------
    
    HWAVEOUT h_audio { }; // device handle   

    auto worc = waveOutOpen( &h_audio 
                           , WAVE_MAPPER 
                           , &wfx 
                           , 0 
                           , 0 
                           , CALLBACK_NULL
                           );
    M_out(L"waveOutOpen() r/c = %d") % worc;

    if (worc != MMSYSERR_NOERROR)
       return -1;         


    // prepare header
    // --------------
     
    WAVEHDR wh { };    // hopefully cleared 
     
    wh.dwBufferLength = (audio.hi_ix + 1) * audio.sample_size;
    wh.lpData         = (LPSTR)(buf.p);
  
    auto wprc = waveOutPrepareHeader(h_audio, &wh, sizeof wh);  
    M_out(L"waveOutPrepareHeader() r/c = %d") % wprc;
  

    if (wprc == MMSYSERR_NOERROR)
    {
        // write out audio buffer, and wait for expected duration
        // ------------------------------------------------------
     
        auto wwrc = waveOutWrite(h_audio, &wh, sizeof wh);
        M_out(L"waveOutWrite() r/c = %d") % wwrc;
     
        if (wwrc == MMSYSERR_NOERROR)
        {
            Sleep( ((int32_t)(audio.duration * (((real_T)(audio.hi_ix)) / ((real_T)(audio.buf.sz)))   * 1000.0)) + 500);     // ????   needs to change to monitor progress, etc.
        }
        else
        {
            rc = -1; 
        }

     
        // unprepare the header
        // --------------------
     
        auto wurc = waveOutUnprepareHeader(h_audio, &wh, sizeof wh);  
        M_out(L"waveOutUnprepareHeader() r/c = %d") % wurc;
    }
    else
    {
         rc = -1; 
    }


    // close audio handle
    // ------------------

    auto wcrc = waveOutClose(h_audio);
    M_out(L"waveOutClose() r/c = %d") % wcrc;


    // return OK or with error depending in WaveOut results
    // ----------------------------------------------------

    return rc; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   add_note() -- add note to main audio buffer
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// form with passed-in completed note  + start time

static int add_note(note_S& note, real_T t, audio_S& audio) try
{
    int32_t start_ix = audio.sample_rate * t; 
    int32_t end_ix   = start_ix + note.buf.sz - 1;

    M__(M_out(L"add_note()  -- start_ix = %d   end_ix = %d") % start_ix % end_ix;)


    // complain, if this note will not fit in audio buffer
    // ---------------------------------------------------

    if (start_ix < 0)
    {
        M_out_emsg(L"add_note() -- went past start of main audio buffer");
        return -1;   
    }  
     
    if (end_ix >= audio.buf.sz)
    {
        M_out_emsg(L"add_note() -- went past end of main audio buffer");
        return -1;   
    }  


    // update the hi-watermark, if required
    // ------------------------------------

    if (end_ix > audio.hi_ix)
        audio.hi_ix = end_ix; 


    // add this note into the audio buffer   (should be able to do block move ??)
    // -----------------------------------

    int32_t note_ix  {0}; 

    for (auto i = start_ix; i < end_ix; i++)
    {
        audio.buf.p[i] += note.buf.p[note_ix++];    
    }
      
    return 0; 
}
M_endf

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// form with waveform, envelope, duration, pitch, volume, time 


static int add_note(const wave_S& wave, const envelope_S& envelope, real_T t, real_T duration, real_T pitch, real_T volume, audio_S& audio) try
{
    // initialize local note structure

    note_S note {};

    note.pitch       = pitch;  
    note.duration    = duration; 
    note.volume      = volume;


    // fill in rest of local note    (allocates note buffer)

    note.buf_ok = false;      // force buffer reallocation

    auto s_rc = setup_note(note, wave, envelope, audio);
    if (s_rc != 0)
        return -1; 


    // add completed note to main audio buffer

    return add_note(note, t, audio);

}
M_endf

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// form with timbre_S, envelope, duration, pitch, volume, time 

static int add_note(const timbre_S& timbre, real_T t, real_T duration, real_T pitch, real_T volume, audio_S& audio) try
{
    // initialize local note structure

    note_S note {};

    note.pitch       = pitch;  
    note.duration    = duration; 
    note.volume      = volume;


    // fill in rest of local note    (allocates note buffer)

    note.buf_ok = false;      // force buffer reallocation

    auto s_rc = setup_note(note, timbre, audio);
    if (s_rc != 0)
        return -1; 


    // add completed note to main audio buffer

    return add_note(note, t, audio);

}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// form with timbre_S, envelope, duration, scale_S+note_number, volume, time 

static int add_note(const timbre_S& timbre, real_T t, real_T duration, const scale_S& scale, int32_t note_number, real_T volume, audio_S& audio) try
{
    // initialize local note structure

    note_S note {};

    note.pitch       = scale.pitches.at(note_number);  
    note.duration    = duration; 
    note.volume      = volume;


    // fill in rest of local note    (allocates note buffer)

    note.buf_ok = false;      // force buffer reallocation
    auto s_rc = setup_note(note, timbre, audio);
    if (s_rc != 0)
        return -1; 


    // add completed note to main audio buffer

    return add_note(note, t, audio);

}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// form with rank_C, note_number, start time, duration, volume 

static int add_note(const rank_C& rank, int32_t note_number, real_T t, real_T duration, real_T volume, audio_S& audio) try
{
    // get pitch, volume, timbre for this note_number from from rank_S
    // ---------------------------------------------------------------


    const timbre_S    *timbre_p    {nullptr};                // returned pointer to timbre_S inside rank_C object
    real_T             loudness    {0.0    };                // relative loudness
    real_T             pitch       {0.0    };                // actual pitch of this note number for this rank

    auto i_rc = rank.note_info(note_number, timbre_p, pitch, loudness); 

    if (i_rc != 0)
        return -1; 


    M__(M_out(L"add_note -- timbre_p = %p <=================================================================================") % timbre_p;)


    // initialize local note structure
    //--------------------------------

    note_S note {};

    note.pitch       = pitch;               // actual pitch from rank_S
    note.duration    = duration; 
    note.volume      = volume * loudness;   // final volume = input volume * relative loudness from rank_S


    // fill in rest of local note    (allocates note buffer)
    // --------------------------

    note.buf_ok = false;                    // force buffer reallocation
    auto s_rc = setup_note(note, *timbre_p, audio);
    if (s_rc != 0)
        return -1; 


    // add completed note to main audio buffer
    // ---------------------------------------

    return add_note(note, t, audio);  
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////     ============    --------------------------------------------------------------------------
////     setup_note() -- fill in note_S area based on waveform and envelope and pitch for this note
////     ============    --------------------------------------------------------------------------
////
////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// timbre_S form, with composite timbre input
// ----------------------------------------

static int setup_note(note_S& note, const timbre_S& timbre, audio_S& audio) try
{
    note.buf_ok = false; 

    M__(M_out(L"setup_note(composite) --  setting note.buf_ok = false");) 

    for (auto i = 0; i < timbre.ct; i++)
    {
        auto s_rc = setup_note(note, timbre.waves.at(i), timbre.envelopes.at(i), audio); 
        if (s_rc != 0)
            return s_rc; 
    }

    return 0; 
}
M_endf
 



// ---------------------------
// single wave + envelope form   (called in loop from above form)
// ---------------------------

static int setup_note(note_S& note, const wave_S& wave, const envelope_S& envelope, audio_S& audio) try
{
    M__(M_out(L"setup_note(single 1) -- note.duration=%.6f envelope.r_buf.sz=%d") % note.duration % envelope.r_buf.sz;)

    // allocate note buffer large enough for note + release period, which follows the note duration

    int32_t samples   = (int32_t)(audio.sample_rate * note.duration);     // number of samples before release phase starts
    int32_t samples_r = samples + envelope.r_buf.sz;                      // total number of samples in note + release (note: envelope buffer and note buffer have same sample rate)     

    if (samples_r <= 1)
    {
        M_out_emsg(L"setup_note(single 2) -- total samples = 0 -- cannot allocate note buffer with 0 length");
        return -1;    
    }
    
    if (!note.buf_ok)
    {
        M__(M_out(L"setup_note(single 3) -- allocating note buffer this time -- samples_r=%d") % samples_r;)

        note.buf.alloc(samples_r);
        note.buf.clear();
        note.buf_ok = true; 
    }
    else       // buffer allocated -- make sure it's big enough
    {
        if (samples_r > note.buf.sz)
        {
            M_out_emsg(L"setup_note() -- existing note buffer too small -- size=%d  need=%d") % note.buf.sz % samples_r;
            return -1;
        }
    }


    // do pitch randomization  (if required)
    // ---------------------- 

    std::lognormal_distribution<double> pitch_dist(0.0, audio.pitch_stddev);      // multiplicative randomization

    real_T pitch {note.pitch};                                                    // default is not randomized 

    if (audio.pitch_stddev != 0.0)
        pitch = note.pitch * pitch_dist(global_N::random_engine); 
       

    // compute wavelength in note buffer (same sample rate as main audio buffer) for this note (depends on pitch and sample rate)

    note.wavelength = audio.sample_rate / note.pitch; 
    M__(M_out(L"setup_note(single 4) -- sample_rate = %.6f   pitch = %.6f   note.wavelength = %.6f") % audio.sample_rate % note.pitch % note.wavelength;)
    

    // set up for waveheight randomization
    // ----------------------------------- 

    std::lognormal_distribution<real_T> waveheight_dist(0.0, audio.waveheight_stddev);


    // =============================
    // main loop to fill note buffer - one iteration per output sample added
    // =============================

    int32_t w_samples         { wave.n                                                 };         // total number of samples in wave buffer 
    real_T  wf_samples        { (real_T)(wave.n)                                       };         // total number of samples in wave buffer 
    int32_t w_main_samples    { wave.n_main                                            };         // number of samples in main part of wave buffer
    real_T  wf_main_samples   { (real_T)(wave.n_main)                                  };         // number of samples in main part of wave buffer
    int32_t w_splice_samples  { wave.n_splice                                          };         // number of samples in splice part of wave buffer
    real_T  wf_splice_samples { (real_T)(wave.n_splice)                                };         // number of samples in splice part of wave buffer
    real_T  wave_if           { 0.0                                                    };         // starting ideal index in wave buffer
    real_T  ix_step           { wave.wavelength / note.wavelength                      };         // wave index advancement factor, for each +1 set in note buffer
    real_T  last_env          {                                                        };         // last waveheight added to note buffer before release phase


    M__(M_out(L"setup_note(single 5): w_samples=%d/%d/%d    wave.wavelength = %.6f   note.wavelength = %.6f   ix_step = %.6f")
              % w_samples % w_main_samples % w_splice_samples % wave.wavelength % note.wavelength % ix_step;)
    

    if ( (w_samples <= 0) || (w_main_samples <= 0) )
    {
        M_out_emsg(L"setup_note(single 6) -- empty wave_S -- w_samples=%d  w_main_samples=%d") % w_samples % w_main_samples;
        return -1; 
    }
       
                                                                                      
    for (auto i = 0; i < samples_r - 1; i++)                                               // loop once per sample in note buffer
    {    
        // --------------------------------------------------------------
        // get waveheight from main part of wave buffer, or splice region
        // --------------------------------------------------------------
        
        real_T  waveheight        {  };          // computed waveheight from wave buffer
        real_T  waveheight_main   {  };

            
        real_T  wave_ifx { std::fmod(wave_if, wf_main_samples) };                          // compute ideal wave buffer index (non-integer)  -- main region only
      

        // get waveheight from main region     
        // -------------------------------
        
        int32_t wave_i0  { M_round_toward_zero(wave_if) %  w_main_samples  };              // compute 1st actual index in waveshape buffer to use -- wrapped) 
        int32_t wave_i1  { M_round_away_zero(  wave_if) %  w_main_samples  };              // compute 2nd actual index in waveshape buffer to use -- wrapped) 
        real_T diff0     { wave_ifx - (real_T)wave_i0                      };              // difference between ideal wave index and lower integer wave index
     
        waveheight_main = (1.0 - diff0) * wave.buf.p[wave_i0] + diff0 * wave.buf.p[wave_i1];   

    

        // see if wave height from splice region needs to be combined
        // -----------------------------------------------------------
        
        if ( 
            (wave_if >  wf_main_samples)                                                         // this is not 1st pass through wave buffer (no splicing needed yet)
            &&                                                                                   // -and-
            (wave_ifx < wf_splice_samples)                                                       // still in splicing overlap region?   (assume this region is smaller than main region)
           )
        {
            real_T waveheight_splice { };                                                       // waveheight from splice part of buffer
        

            // get waveheight from corresponding part of splice region (assume sample region has at least 2 samples)
            // -----------------------------------------------------------------------------------------------------

            int32_t wave_i0s { wave_i0  + w_main_samples      }; 
            int32_t wave_i1s { wave_i0s + 1                   }; 
            real_T diff0s    { diff0                          };                                           // difference between ideal wave index and lower integer wave index
             
            waveheight_splice = (1.0 - diff0s) * wave.buf.p[wave_i0s] + diff0s * wave.buf.p[wave_i1s];     // do linear interpolation between 1st and 2nd integer indexes in splice region  


            // combine two waveheights based on splice factor

            real_T splice_factor = 1.0 - wave_ifx / wf_splice_samples;  

            waveheight = splice_factor * waveheight_splice + (1.0 - splice_factor) * waveheight_main;   
        }
        else   
        {   
            // just get waveheight from main part of wave buffer  (might overflow into 1st sample in splice region)
            // -------------------------------------------------
 
            waveheight = waveheight_main;     // do linear interpolation between 1st and 2nd integer indexes  
         
            M__(M_out(L"setup_note(single 7): i=%d  wave_if=%.6f  wave_ifx=%.6f  wave_i0=%d  wave_i1=%d  diff0=%.6f  wave=%.6f/%.6f  waveheight=%.6f")
                      % i % wave_if % wave_ifx % wave_i0 % wave_i1 % diff0 % wave.buf.p[wave_i0] %wave.buf.p[wave_i1] % waveheight; )
        }
        

        // ------------------------------------------------------------------
        // randomize (if required) and apply envelope to extracted waveheight
        // ------------------------------------------------------------------

        // randomize the waveheight  (if required)

        if (audio.waveheight_stddev != 0.0)
            waveheight +=  waveheight_dist(global_N::random_engine);


        // if still in release portion of note, get relative amplitude from r_buf    (rd_buf and note have same sampling rate, so indexes are the same)
  
        if (i >= samples)                                              // are we in release period?
        {
            note.buf.p[i] += envelope.r_buf.p[i - samples] * waveheight *  note.volume * last_env;   
        }
        else                           // if still in attack/decay part of note, get relative amplitude from ad_buf    (ad_buf and note have same sampling rate, so indexes are the same)
        if (i < envelope.ad_buf.sz)
        {
            last_env      = envelope.ad_buf.p[i];
            note.buf.p[i] += envelope.ad_buf.p[i] * waveheight *  note.volume; 

        }
        else                           // must be in sustain period
        {
            last_env      = envelope.sustain;
            note.buf.p[i] += envelope.sustain * waveheight * note.volume; 
        }


        // ----------------------------
        // get ready for next iteration
        // ----------------------------
                
        wave_if += ix_step;         // step to next ideal place in wave buffer
    }   

    return 0; 
}
M_endf







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                                                                                                                                                                                                        
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                     
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||    Math-oriented functions
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 

//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       r2c_dft() -- real to complex forward DFT using fftw3
//||       =========    ---------------------------------------
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

int r2c_dft(const std::vector<real_T>& in_values, std::vector<real_T>& out_amp, std::vector<real_T>& out_phase) try
{
    int  n_in    { (int)(in_values.size()) };       // number of input values
    int  n_out   { n_in / 2 + 1            };       // number of output values 
   

    // return immediately, if no data (empty vector)
    // ------------------------------

    if (n_in <= 0)
    {       
        M_out_emsg(L"forward_dft() -- input vector is empty");
        return -1; 
    }


    // allocate input/output areas and copy over caller's data 
    // -------------------------------------------------------

    double               *in_p    { fftw_alloc_real(   n_in ) }; 
    fftw_complex         *out_p   { fftw_alloc_complex(n_out) }; 

    std::memcpy( (void *)in_p
               , (void *)(in_values.data())
               , sizeof (double) * n_in
               );                                                       // copy input values into properly-aligned storage 

    // create plan and do the dft
    // --------------------------

    fftw_plan plan  { fftw_plan_dft_r2c_1d( n_in, in_p, out_p, FFTW_ESTIMATE) }; 


    std::memcpy( (void *)in_p
               , (void *)(in_values.data())
               , sizeof (double) * n_in
               );                                                           // refresh input data after fftw_plan() clobbered the array 

    fftw_execute(plan);                  


    // pass back output amplitude and phase (in outout vectors)
    // --------------------------------------------------------

    out_amp.clear();
    out_phase.clear();  

    real_T normalize = 1.0 / sqrt((real_T)n_in);

    real_T ampl_max {0.0}; 

    for (auto j = 0; j < n_out; j++)
    {
        double r = out_p[j][0]; 
        double i = out_p[j][1]; 

        real_T ampl { normalize * sqrt(r*r + i*i) }; 

        if (ampl > ampl_max)                   // remember largest amplitude passed back 
           ampl_max = ampl; 

        out_amp   . push_back(ampl       );    // pass back i-th amplitude         
        out_phase . push_back(atan2(i, r));    // pass back i-th phase

        M__(M_out(L"r2c_dft():  j=%d    r=%.18f   i=%.18f     amp=%.18f   phase=%.18f") % j % r % i % out_amp.at(j) % out_phase.at(j);) 
    }

    int i {0}; 


#if 0 
    for (auto ampl : out_amp)
    {
        if (ampl > ampl_max / 10.0 )
            M_out(L"r2c_dft():  i=%d    amp=%.18f   phase=%.18f") % i % ampl % out_phase.at(i);
        i++; 
    }
#endif
    

    // free up fftw3 resources before returning
    // ----------------------------------------

    fftw_destroy_plan(plan);
    fftw_free(out_p);    
    fftw_free(in_p);  

    return 0; 
}
M_endf










///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   normalize_buffer() -- adjust samples in caller's buffer to normalized values
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int normalize_buffer(buffer_C<real_T>& buf, real_T max_sample) try
{
    if (buf.sz < 1)
       return -1; 
    

    // look for largest and smallest sample   
    // ------------------------------------
    
    real_T s_max { -M_real_max };         // start with smallest real_T number
    real_T s_min {  M_real_max };         // start with largest real_T number

    for (auto i = 0; i < buf.sz; i++)
    {
        if (buf.p[i] > s_max)
            s_max = buf.p[i];   
        if (buf.p[i] < s_min)
            s_min = buf.p[i];      
    }


    // Compute sample rescale factor based on max amplitude found above
    // ----------------------------------------------------------------

    real_T rescale   {1.0};                                            // default rescale, if all samples are zero
    real_T s_min_max {std::max(std::abs(s_max), std::abs(s_min))};     // largest amplitude

    if (s_min_max != 0.0)
        rescale = max_sample / s_min_max;


    // rescale samples to normalized values
    // ------------------------------------

    M_y(M_out(L"normalize_buffer() -- s_max = %.16f   s_min = %.16f   s_min_max = %.16f   rescale = %.16f") % s_max % s_min % s_min_max % rescale;)   

    for (auto i = 0; i < buf.sz; i++)
    {
        buf.p[i] *= rescale;         
    }
 
    return 0;
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   normalize_vector() -- adjust values in caller's vector to normalized values within specified range
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int normalize_vector(std::vector<real_T>& vect, real_T min_norm, real_T max_norm) try
{
    if (vect.size() < 1)
    {
       M_out_emsg(L"normlize_vector() -- input vector is empty");
       return -1; 
    }

    if (max_norm <= min_norm)
    {
        M_out_emsg(L"normlize_vector() -- max normalized value (%.8d) does not exceed min normalized value (%.8d)") % max_norm % min_norm; 
        return -1;     
    }
    

    // get largest and smallest values in vector
    // -----------------------------------------

    real_T min_value { };         // filled in by minmax_value_vector()
    real_T max_value { };         // filled in by minmax_value_vector()

    minmax_value_vector(vect, min_value, max_value);

    
    // Compute sample rescale add/mult factors so as to match caller's requested normalized values
    // -------------------------------------------------------------------------------------------   

    real_T add      { min_norm - min_value    };           // add this amount to adjust minimum value to normalized minimum value  
    real_T multiply { 1.0                     };           // start with 1.0, in case all data in vector is same value (can't be rescaled) 
    
    if (max_value > min_value)
    {
        multiply = (max_norm - min_norm) / (max_value - min_value);     
    }
    

    // rescale values in vector to requested normalized values
    // -------------------------------------------------------

    M_y(M_out(L"normalize_vector() -- min_value=%.16f   max_value=%.16f   add=%.16f   multiply=%.16f") % min_value % max_value % add % multiply;)   

    rescale_vector(vect, add, multiply); 
    return 0;
}
M_endf




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   maxsample_buffer() -- get maximum amplitude (absolute value) of any sample in buffer
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int maxsample_buffer(buffer_C<real_T>& buf, real_T& max_sample) try
{
    if (buf.sz < 1)
       return -1; 


    // look for largest and smallest sample
    // ------------------------------------

    real_T s_max { -M_real_max };         // start with smallest real_T number
    real_T s_min {  M_real_max };         // start with largest real_T number

    for (auto i = 0; i < buf.sz; i++)
    {
        if (buf.p[i] > s_max)
            s_max = buf.p[i];   
        if (buf.p[i] < s_min)
            s_min = buf.p[i];      
    }

    max_sample = std::max(std::abs(s_max), std::abs(s_min));   // largest amplitude
      
    return 0;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   minmax_value_vector() -- get minimum and maximum sample in vector
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int minmax_value_vector(std::vector<real_T>& vect, real_T& min_value, real_T& max_value) try
{
    if (vect.size() < 1)
    {
       min_value = 0.0; 
       max_value = 0.0;
       return -1; 
    }


    // look for largest and smallest value in vector
    // ---------------------------------------------

    real_T s_max { -M_real_max };         // start with smallest real_T number
    real_T s_min {  M_real_max };         // start with largest real_T number

    for (auto i = 0; i < vect.size(); i++)
    {
        if (vect.at(i) > s_max)
            s_max = vect.at(i);   
        if (vect.at(i) < s_min)
            s_min = vect.at(i);      
    }

    min_value = s_min;
    max_value = s_max;
      
    return 0;
}
M_endf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   rescale_buffer() -- rescale samples by caller's rescale factor
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int rescale_buffer(buffer_C<real_T>& buf, real_T rescale) try
{
    for (auto i = 0; i < buf.sz; i++)
    {
        buf.p[i] *= rescale;         
    }
 
    return 0;
}
M_endf 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   rescale_vector() -- rescale values by caller's rescale factor, with optional additive amount
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rescale_vector(std::vector<real_T>& vect, real_T multiply) try
{
    return rescale_vector(vect, 0.0, multiply);
}
M_endf 

 
int rescale_vector(std::vector<real_T>& vect, real_T add, real_T multiply) try
{
    for (auto i = 0; i < vect.size(); i++)
    {
        vect.at(i) *= multiply; 
        vect.at(i) += add; 
    }
 
    return 0;
}
M_endf   


//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       plot_graph() -- plot caller's data on graph
//||       ============    ---------------------------
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   plot_graph() -- buffer_C<real_T> version -- does not normalize caller's input data -- assumed to be in +1 to -1 limits
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  

int  plot_graph(canvas_C& canvas, const rgb_S& rgb, rcf_T top, rcf_T left, rcf_T bottom, rcf_T right, const buffer_C<real_T>& data_in, uint32_t ix1, uint32_t ix2) try
{
    // error, if less than two points asked for, of if ix2 exceeds size of caller's buffer 
    // -----------------------------------------------------------------------------------

    uint32_t n { ix2 - ix1 }; 

    if (ix2 < 2)
    {
        M_out_emsg(L"plot_graph -- number of data points requested = %d -- minimum of 2 required") % n; 
        return -1; 
    }

    if (n >= data_in.sz)
    {
        M_out_emsg(L"plot_graph -- number of data points requested = %d -- max number in data = %d") % n % data_in.sz; 
        return -1; 
    }

     
    // get copy of caller's data normalized to +-1.0
    // --------------------------------------------

    //buffer_C<real_T> data {};
    //data = data_in; 

    //normalize_buffer(data, 1.0); 


    // compute col_increment so caller's data fits in plot limits
    // ----------------------------------------------------------

    rcf_T col_incr = (right - left) / (real_T)(n - 1); 


    // scale data in buffer so +1.0 is at top and -1.0 is at bottom of plot and place in vector for draw_lines()   -- caller should have pre-scaled the data before calling plot_data()
    // ---------------------------------------------------------------------------------------------------------

    rcf_T half_ht  = 0.5 * (bottom - top);                   // height of plot
    rcf_T baseline = top + half_ht;                          // y-value of 0.0 line 

    rcf_T col = left;                                        // start at left edge
    std::vector<Gdiplus::PointF> points{ };

    for (auto i = ix1; i <= ix2; i++)
    {
        points.push_back(Gdiplus::PointF { (Gdiplus::REAL)col, (Gdiplus::REAL)(baseline + half_ht * data_in.p[i]) } );
        col += col_incr; 
    } 

    canvas.draw_lines(rgb, 1.0, points);

    return 0; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   plot_graph() -- std::vector<real_T> version -- normalizes callers input data
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  

int  plot_graph(canvas_C& canvas, const rgb_S& rgb, rcf_T top, rcf_T left, rcf_T bottom, rcf_T right, const std::vector<real_T>& data_in, uint32_t ix1, uint32_t ix2) try
{
    // error, if less than two points asked for, of if ix2 exceeds size of caller's buffer 
    // -----------------------------------------------------------------------------------

    uint32_t n { ix2 - ix1 }; 

    if (n < 2)
    {
        M_out_emsg(L"plot_graph -- number of data points requested = %d -- minimum of 2 required") % n; 
        return -1; 
    }

    if (ix2 >= data_in.size())
    {                  
        M_out_emsg(L"plot_graph -- number of data points requested = %d -- max number in data = %d") % n % data_in.size(); 
        return -1; 
    }

     
    // get copy of caller's data normalized to 0 to bottom-top
    // -------------------------------------------------------

    std::vector<real_T> data {};
    data = data_in; 

    normalize_vector(data, 0.0, (real_T)(bottom-top)); 


    // compute col_increment so caller's data fits in plot limits
    // ----------------------------------------------------------

    rcf_T col_incr = (right - left) / (real_T)(n - 1); 


    // scale data in buffer so +1.0 is at top and -1.0 is at bottom of plot and place in vector for draw_lines()   -- caller should have pre-scaled the data before calling plot_data()
    // ---------------------------------------------------------------------------------------------------------

    rcf_T col = left;                                        // start at left edge
    std::vector<Gdiplus::PointF> points{ };

    for (auto i = ix1; i <= ix2; i++)
    {
        points.push_back(Gdiplus::PointF { (Gdiplus::REAL)col, (Gdiplus::REAL)((real_T)bottom - data.at(i)) } );
        col += col_incr; 
    } 

    canvas.draw_lines(rgb, 1.0, points);

    return 0; 
}
M_endf      



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   fill_envelope() -- 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
static void fill_envelope(envelope_S& envelope, audio_S& audio) try
{
    // allocate buffers before starting
    // --------------------------------

    int32_t n_delay   = (int32_t)( envelope.delay_time    * (real_T)(audio.sample_rate) ); 
    int32_t n_attack  = (int32_t)( envelope.attack_time   * (real_T)(audio.sample_rate) ); 
    int32_t n_peak    = (int32_t)( envelope.peak_time     * (real_T)(audio.sample_rate) );
    int32_t n_decay   = (int32_t)( envelope.decay_time    * (real_T)(audio.sample_rate) );
    int32_t n_release = (int32_t)( envelope.release_time  * (real_T)(audio.sample_rate) );

    M_out(L"fill_envelope() -- n_attack = %d   n_decay = %d   n_release = %d") % n_attack % n_decay % n_release;
    
    envelope.ad_buf.alloc(n_attack + n_peak + n_decay);
    envelope.r_buf.alloc (n_release                  );


    // fill in attack/decay buffer (assume attack, etc. has more than one sample, so there is no divide by zero)
    // ---------------------------

    for (auto i = 0; i < n_delay; i++)
    {
        envelope.ad_buf.p[i] =  0.0;     
    }

    for (auto i = n_delay; i < n_delay + n_attack; i++)
    {
        real_T fract = ((real_T)(i - n_delay)) / (real_T)(n_attack - 1);       // attack progress

        envelope.ad_buf.p[i] =  fract * envelope.peak;     
    }

    for (auto i = n_delay + n_attack; i < n_delay + n_attack + n_peak; i++)
    {
        envelope.ad_buf.p[i] =  envelope.peak;     
    }
    
    for (auto i = n_delay + n_attack + n_peak; i < n_delay + n_attack + n_peak + n_decay; i++)
    {
        real_T fract = ((real_T)(i - (n_delay + n_attack + n_peak) )) / (real_T)(n_decay - 1);   

        envelope.ad_buf.p[i] =  (1.0 - fract) * (envelope.peak - envelope.sustain) + envelope.sustain;  

        //M_out(L"fill_envelope() -- ad_buf[%d] = %.6f") % i % envelope.ad_buf.p[i]; 
    }

    for (auto i = 0; i < n_release; i++)
    {
        real_T fract = ((real_T)i) / (real_T)(n_release - 1);   

        envelope.r_buf.p[i] =  (1.0 - fract)   /*  * envelope.sustain */ ;     
    }



    return; 
}
M_endf



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   add_harmonics() -- adds vector of harminics to the caller's wave buffer 
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void add_harmonics(wave_S& wave, const std::vector<harmonic_S>& harmonics, audio_S& audio) try
{
    // allocate wave buffer -- compute size based on number of cycles and wavelength  
    // --------------------  

    wave.n_main    =  M_round(wave.cycles_main   * wave.wavelength); 
    wave.n_splice  =  M_round(wave.cycles_splice * wave.wavelength);  
    wave.n         =  wave.n_main + wave.n_splice;   

    M__(M_out(L"add_harmonics() -- wavelength = %.12f   cycles = %.6f/%.6f   samples = %d/%d   total samples = %d") 
              % wave.wavelength % wave.cycles_main % wave.cycles_splice % wave.n_main % wave.n_splice % wave.n;) 

    wave.buf.alloc(wave.n);
    wave.buf.clear();             // make sure we start at 0.0
         
 
    // ==========    ----------------------------------
    // outer loop -- once for each harmonic_S in vector
    // ==========    ----------------------------------

    int32_t hn {0};     // for debug printouts

    for (auto harmonic : harmonics)
    {
        int32_t n_subwaves                    { M_round(harmonic.split)                           };    // number of subwaves for this harmonic
        real_T  base_angle_step               { harmonic.freq_mult * 2.0 * M_PI / wave.wavelength };    // base angle step amount for basic freqency for this harmonis 
        real_T  base_amplitude                { harmonic.amplitude / harmonic.split               };    // compute proper 1/n_subwaves base amplitude for this harmonic
        real_T  base_wavelength_adj_interval  { harmonic.wavelength_adj_interval                  };    // compute proper 1/n_subwaves base amplitude for this harmonic

        M__(M_out(L"add_harmonics() -- hn=%d   n_subwaves=%d    angle_step=%.15f   2pi/angle_step=%.15f" ) % hn % n_subwaves % angle_step % (2.0 * M_PI / angle_step);)

        std::lognormal_distribution<real_T> freq_mult_dist(          0.0, harmonic.freq_stddev                    );
        std::lognormal_distribution<real_T> amplitude_dist(          0.0, harmonic.amplitude_stddev               ); 
        std::lognormal_distribution<real_T> wavelength_dist(         0.0, harmonic.wavelength_adj_stddev          ); 
        std::lognormal_distribution<real_T> wavelength_interval_dist(0.0, harmonic.wavelength_adj_interval_stddev ); 
        std::   normal_distribution<real_T> phase_dist(              0.0, harmonic.phase_stddev                   ); 


        // randomize initial phase for this harmonic, if requested
                
        real_T phase {0.0}; 
                
        if (harmonic.phase_stddev != 0.0)
            phase += 2.0 * M_PI * phase_dist(global_N::random_engine);  
    
         
         // ---------------------------------------------------
         // middle loop to split harmonic into several subwaves
         // ---------------------------------------------------

         for (auto n = 0; n < n_subwaves; n++)
         {   
             M__(M_out(L"add_harmonics() -- hn=%d  sub=%d   amplitude = %.6f   phase_stddev = %.6f w_adj_stddev = %.6f w_adj_interval = %d" ) 
                      % hn % n % amplitude % harmonic.phase_stddev % harmonic.wavelength_adj_stddev % harmonic.wavelength_adj_interval;)


             // put in this harmonic -- n-th subwave (only if base amplitude is non-0)
             // ----------------------------------------------------------------------
          
             if (base_amplitude > 0.0)
             {  
                 // randomize frequency for this subwave, if required

                 real_T angle_step { base_angle_step };

                 if (harmonic.freq_stddev != 0.0)
                     angle_step *= freq_mult_dist(global_N::random_engine);  
                                                 

                 // randomize amplitude for this subwave, if required

                 real_T amplitude {base_amplitude};

                 if (harmonic.amplitude_stddev != 0.0)
                     amplitude *= amplitude_dist(global_N::random_engine);  


                 // randomize wavelength_adj_interval for this subwave, if required

                 real_T wavelength_adj_interval {base_wavelength_adj_interval};

                 if (harmonic.wavelength_adj_interval_stddev != 0.0)
                     wavelength_adj_interval *= wavelength_interval_dist(global_N::random_engine);  

                 int32_t wavelength_adj_intv { M_round(wavelength_adj_interval) };


                 // set up statring phase and angle step values for inner loop
                
                 real_T x           {phase};                   // starting phase for this subwave
                 real_T step_factor {1.0  };                   // default angle step factor, if no periodic wavelength adjustment requested
                
                 M__(M_out(L"add_harmonics() -- hn=%d sub=%d  phase = %.6f  step_factor = %.12f" ) % hn % n % phase % step_factor;)


                 // ------------------------------------------------------------------------
                 // inner loop to fill each sample in wave buffer with data for this subwave  (fills main zone, splice zone + any extra padding)
                 // ------------------------------------------------------------------------                                  

                 for (auto i = -wave.n_skip; i < wave.n; i++)
                 {
                     // adjust wavelength factor for next wavelength adjust interval, if random wavelength is requested
                                         
                     if (harmonic.wavelength_adj_stddev != 0.0)                             // don't get random number unless required
                     {
                         if ((wavelength_adj_intv > 0) && (i % wavelength_adj_intv == 0))   // only update wavelength factor every so often 
                         {
                             step_factor = wavelength_dist(global_N::random_engine);                   
                             M__(M_out(L"add_harmonics() -- hn=%d  sub=%d  i=%d  adjusted step_factor=%.6f" ) % hn % n % i % step_factor;)
                         }
                     }
                     

                     // record this waveheight if skip zone has passed

                     if (i >= 0)
                     {
                         wave.buf.p[i] += amplitude  * sin(x);                     // add in data point for this subwave
                         x             += angle_step * step_factor;                // increment angle for next sample   
                     }
                  

                    // if (i < 15)
                         M__(M_out(L"add_harmonics() -- hn=%d  sub=%d  x=%.16f  sin(x)=%.16f   wave.buf.p[%d] = %.16f") % hn % n % x % sin(x) % i % wave.buf.p[i];)
                 }  
             }
         }       // end of middle loop

         hn++;   // for debugging    
    }            // end of outer loop 


    // if requested, look for starting and ending DC offsets (first cycle and last cycle -- includes main and sample regions)
    // ---------------------------------------------------------------------------------------------------------------------

    if (wave.dc_adjust)
    {
        if (wave.cycles_main + wave.cycles_splice > 3)
        {
             int32_t n = M_round(wave.wavelength);        // number of samples to look at at start and end
        
             real_T  start_sum {0.0};
             real_T  end_sum   {0.0};
        
             for (auto i = 0; i < n; i++)
             {
                 start_sum += wave.buf.p[   i      ]; 
                 end_sum   += wave.buf.p[wave.n - i];  
             }
        
             real_T start_avg  { start_sum / wave.wavelength };
             real_T end_avg    { end_sum   / wave.wavelength };
        
             M_y(M_out(L"add_harmonics() -- start_avg = %.12d    end_avg = %.12d (before)") % start_avg % end_avg;)
        
        
            // adjust wave samples to make starting and ending DC offset = 0.00
            // ---------------------------------------------------------------
        
            for (auto i = 0; i < wave.n; i++)
            {
                real_T sub = start_avg * (real_T)(wave.n - i)/(real_T)(wave.n) 
                           + end_avg   * (real_T)i /(real_T)(wave.n);
        
                wave.buf.p[i] -= sub; 
            }

            //////////////
         
            start_sum = 0.0; 
            end_sum = 0.0;
         
            for (auto i = 0; i < n; i++)
            {
                start_sum += wave.buf.p[   i      ]; 
                end_sum   += wave.buf.p[wave.n - i];  
            }
         
            start_avg =  start_sum / wave.wavelength ;
            end_avg   =  end_sum   / wave.wavelength ;
         
            M_y(M_out(L"add_harmonics() -- start_avg = %.12d    end_avg = %.12d (after)") % start_avg % end_avg;) 
        }
    } 


    // normalize waveform to +-1 max
    // -----------------------------

    normalize_buffer(wave.buf, 1.0); 

    M__(M_out(L"add_harmonics(): 1st waveheight = %.6f    last waveheight   = %.6f    penultimate waveheight = %.6f") % wave.buf.p[0] % wave.buf.p[wave.n - 1]        % wave.buf.p[wave.n - 2]);

    return; 
}
M_endf






//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||
//||       init_scale() -- initialize calle's scale_S structure (12 tones / octave only)
//||       ============    ------------------------------------
//||
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

static int init_scale(scale_S& scale, scale_pitch_E pitch, temperament_E temperament, int32_t base_note) try
{
    // initialize fields in scale_S

    scale.pitch         = pitch; 
    scale.temperament   = temperament;
    scale.lowest_note   = M_12_C_1;
    scale.base_note     = base_note; 
    scale.highest_note  = M_12_C10;
           
    scale.pitches.clear();                // get rid of any leftovers 


    // ---------------------------------------------------
    // compute pitch for C4 (middle C) = note 60 (M_12_C4)
    // ---------------------------------------------------

    real_T freq_c4  { };   

    switch (pitch)
    {
    case scale_pitch_E::c_256 :
        freq_c4 = 256.0;          
        break; 

    case scale_pitch_E::a_415 :
        freq_c4 = 415.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 

    case scale_pitch_E::a_430 :
        freq_c4 = 430.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 

    case scale_pitch_E::a_432 :
        freq_c4 = 433.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 
  
    case scale_pitch_E::a_433 :
        freq_c4 = 433.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 

    case scale_pitch_E::a_435 :
        freq_c4 = 435.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 

    case scale_pitch_E::a_436 :
        freq_c4 = 436.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break;               

    case scale_pitch_E::a_439 :
        freq_c4 = 439.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break;        

    case scale_pitch_E::a_440 :
        freq_c4 = 440.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 

    case scale_pitch_E::a_441 :
        freq_c4 = 441.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 

    case scale_pitch_E::a_442 :
        freq_c4 = 442.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 

    case scale_pitch_E::a_443 :
        freq_c4 = 442.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 

    case scale_pitch_E::a_452_5 :
        freq_c4 = 452.5 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break; 
  
    case scale_pitch_E::a_466 :
        freq_c4 = 466.0 * std::pow( 2.0, ((real_T)(M_12_C4 - M_12_A4))/12.0);         
        break;   

    default : 
        M_out_emsg(L"init_scale() -- unknown scale_pitch_E parm = %d passed in") % (int)pitch; 
        return -1;
    }  

    M__(M_out(L"init_scale() -- freq_c4 = %.12f") % freq_c4;) 



    // ---------------------------------------------
    // compute  12 basic tone ratios for main octave
    // ---------------------------------------------

    int32_t              base_note {     }; 
    buffer_C<real_T>     ratios    {12ULL};     // buffer with 12 pitch ratios above base note


    switch (temperament)
    {

    // fill out ratios array for 12-tone equal temperament
    // ---------------------------------------------------

    case temperament_E::edo_12 :

       // base_note = M_12_C4;                            // base octave starts at C4  (doesn't really matter due to equally-spaced ratios)

        for (auto i = 0; i < 12; i++)
        {
            ratios.p[i] = pow(2.0, ((real_T)i)/12.0);
        }  

        break;
                
    
     // fill out ratios array for 12-tone pythagorean temperament -- keep augmented 4th, leave out diminished 5th
     // ---------------------------------------------------------------------------------------------------------   

     case temperament_E::pythag_aug4 :

     //   base_note = M_12_D4;                            // base octave starts at D4  

         ratios.p[ 0]  = 1.0;                            // D           -- unison            (assuming base octave starts at D)
         ratios.p[ 1]  = 256.0/243.0;                    // E flat      -- minor second
         ratios.p[ 2]  = 9.0/8.0;                        // E           -- major second
         ratios.p[ 3]  = 32.0/27.0;                      // F           -- minor third
         ratios.p[ 4]  = 81.0/64.0;                      // F sharp     -- major third
         ratios.p[ 5]  = 4.0/3.0;                        // G           -- perfect fourth 
         ratios.p[ 6]  = 729.0/512.0;                    // G sharp     -- augmented fourth  
      // ratios.p[ 6]  = 1024.0/729.0;                   // A flat      -- diminished fifth  
         ratios.p[ 7]  = 3.0/2.0;                        // A           -- perfect fifth 
         ratios.p[ 8]  = 128.0/81.0;                     // B flat      -- minor sixth
         ratios.p[ 9]  = 27.0/16.0;                      // B           -- major sixth
         ratios.p[10]  = 16.0/9.0;                       // C           -- minor seventh 
         ratios.p[11]  = 243.0/128.0;                    // C sharp     -- major seventh 
        
         break;
    

     // fill out ratios array for 12-tone pythagorean temperament -- keep diminished 5th, leave out augmented 4th
     // ---------------------------------------------------------------------------------------------------------   

     case temperament_E::pythag_dim5 :

      //   base_note = M_12_D4;                            // base octave starts at D4  
    
         ratios.p[ 0]  = 1.0;                            // D           -- unison            (assuming base octave starts at D)
         ratios.p[ 1]  = 256.0/243.0;                    // E flat      -- minor second
         ratios.p[ 2]  = 9.0/8.0;                        // E           -- major second
         ratios.p[ 3]  = 32.0/27.0;                      // F           -- minor third
         ratios.p[ 4]  = 81.0/64.0;                      // F sharp     -- major third
         ratios.p[ 5]  = 4.0/3.0;                        // G           -- perfect fourth 
      // ratios.p[ 6]  = 729.0/512.0;                    // G sharp     -- augmented fourth  
         ratios.p[ 6]  = 1024.0/729.0;                   // A flat      -- diminished fifth  
         ratios.p[ 7]  = 3.0/2.0;                        // A           -- perfect fifth 
         ratios.p[ 8]  = 128.0/81.0;                     // B flat      -- minor sixth
         ratios.p[ 9]  = 27.0/16.0;                      // B           -- major sixth
         ratios.p[10]  = 16.0/9.0;                       // C           -- minor seventh 
         ratios.p[11]  = 243.0/128.0;                    // C sharp     -- major seventh 
       
        break;
     


     // fill out ratios array for 5-limit Just intonation temperament (asymmetric standard version, keep augmented 4th) 
     // -------------------------------------------------------------   

     case temperament_E::ji_5limit :

      //   base_note = M_12_D4;                            // base octave starts at C4  
    
         ratios.p[ 0]  = 1.0;                            // C           -- unison            (assuming base octave starts at C)
         ratios.p[ 1]  = 16.0/15.0;                      // D flat      -- minor second
         ratios.p[ 2]  = 9.0/8.0;                        // D           -- major second
    //   ratios.p[ 2]  = 10.0/9.0;                       // D           -- major second      (from symmetric version)
         ratios.p[ 3]  = 6.0/5.0;                        // E flat      -- minor third
         ratios.p[ 4]  = 5.0/4.0;                        // E           -- major third
         ratios.p[ 5]  = 4.0/3.0;                        // F           -- perfect fourth 
         ratios.p[ 6]  = 45.0/32.0;                      // F sharp     -- augmented fourth  
   //    ratios.p[ 6]  = 64.0/45.0;                      // G flat      -- diminished fifth  
         ratios.p[ 7]  = 3.0/2.0;                        // G           -- perfect fifth 
         ratios.p[ 8]  = 8.0/5.0;                        // A flat      -- minor sixth
         ratios.p[ 9]  = 5.0/3.0;                        // A           -- major sixth
         ratios.p[10]  = 9.0/5.0;                        // B flat      -- minor seventh 
     //  ratios.p[10]  = 16.0/9.0;                       // B flat      -- minor seventh     (from symmetric version)
         ratios.p[11]  = 15.0/8.0;                       // B           -- major seventh 
       
        break;


     // fill out ratios array for 5-limit Just intonation temperament (asymmetric extended version, keep augmented 4th) 
     // -------------------------------------------------------------   

     case temperament_E::ji_5limit_ex :

      //   base_note = M_12_D4;                            // base octave starts at C4  
    
         ratios.p[ 0]  = 1.0;                            // C           -- unison            (assuming base octave starts at C)
         ratios.p[ 1]  = 16.0/15.0;                      // D flat      -- minor second
         ratios.p[ 2]  = 9.0/8.0;                        // D           -- major second
         ratios.p[ 3]  = 6.0/5.0;                        // E flat      -- minor third
         ratios.p[ 4]  = 5.0/4.0;                        // E           -- major third
         ratios.p[ 5]  = 4.0/3.0;                        // F           -- perfect fourth 
         ratios.p[ 6]  = 25.0/18.0;                      // F sharp     -- augmented fourth  
   //    ratios.p[ 6]  = 36.0/25.0;                      // G flat      -- diminished fifth  
         ratios.p[ 7]  = 3.0/2.0;                        // G           -- perfect fifth 
         ratios.p[ 8]  = 8.0/5.0;                        // A flat      -- minor sixth
         ratios.p[ 9]  = 5.0/3.0;                        // A           -- major sixth
         ratios.p[10]  = 9.0/5.0;                        // B flat      -- minor seventh 
         ratios.p[11]  = 15.0/8.0;                       // B           -- major seventh 
       
        break;


     // fill out ratios array for 7-limit Just intonation temperament (keep augmented 4th) 
     // -------------------------------------------------------------   

     case temperament_E::ji_7limit :

      //   base_note = M_12_D4;                            // base octave starts at C4  
    
         ratios.p[ 0]  = 1.0;                            // C           -- unison            (assuming base octave starts at C)
         ratios.p[ 1]  = 16.0/15.0;                      // D flat      -- minor second
         ratios.p[ 2]  = 8.0/7.0;                        // D           -- major second
         ratios.p[ 3]  = 6.0/5.0;                        // E flat      -- minor third
         ratios.p[ 4]  = 5.0/4.0;                        // E           -- major third
         ratios.p[ 5]  = 4.0/3.0;                        // F           -- perfect fourth 
         ratios.p[ 6]  = 7.0/5.0;                        // F sharp     -- augmented fourth  
   //    ratios.p[ 6]  = 10.0/7.0;                       // G flat      -- diminished fifth  
         ratios.p[ 7]  = 3.0/2.0;                        // G           -- perfect fifth 
         ratios.p[ 8]  = 8.0/5.0;                        // A flat      -- minor sixth
         ratios.p[ 9]  = 5.0/3.0;                        // A           -- major sixth
         ratios.p[10]  = 7.0/4.0;                        // B flat      -- minor seventh 
         ratios.p[11]  = 15.0/8.0;                       // B           -- major seventh 
       
        break;




    default : 
        M_out_emsg(L"init_scale() -- unknown temperament parm = %d  passed in") % (int)temperament; 
        return -1;            
    }   
    
    
    // debug display for ratios
#if 0     
    for (auto i = 0; i < 12; i++)
    {
         M__(M_out(L"init_scale() -- base_note=%d   ratios.p[%d] = %.12f") % base_note % i % ratios.p[i];)
    }  
#endif
 
    // get frequency of base note in this scale
    // ----------------------------------------

    int32_t octave_diff_c4    { M_divide_rounddown(M_12_C4 - base_note, 12) };   // number of octaves between octave containing c4 and start of base octave for this scale
    int32_t note_position_c4n { (600 + M_12_C4 - base_note) % 12            };   // get c4n position within base octave for this scale (600 makes for positive number input to % operator)
    real_T  octave_mult_c4    { pow(2.0, (real_T)octave_diff_c4)            };   // c4 octave frequency multiplier = c4/c4n
    real_T  ratio_c4n         { ratios.p[note_position_c4n]                 };   // get ratio for c4n/base_note in base octave
    real_T  base_note_freq    { freq_c4 / (octave_mult_c4 * ratio_c4n)      };   // frequency of base_note in base octave 


    M__(M_out(L"init_scale() -- octave_dif_c4=%d  note_position_c4n=%d  octave_mult_c4=%.8f  ratio_c4n=%.12f  base_note=%d  base_note_freq=%.12f ")
              % octave_diff_c4 % note_position_c4n % octave_mult_c4 % ratio_c4n % base_note % base_note_freq;
       )



    // fill in scale from lowest to highest note
    // -----------------------------------------   

    for (auto i = scale.lowest_note; i <= scale.highest_note; i++)
    {
        int32_t octave_diff    { M_divide_rounddown(i - base_note, 12) };         // get number of octaves between octave containing this note and the start of base octave for this scale 
        int32_t note_position  { (600 + i - base_note) % 12            };         // get note position within base octave for this scale (600 makes for positive number input to % operator) 
        real_T  octave_mult    { pow(2.0, (real_T)octave_diff)         };         // get octave frequency multiplier for note-i / note-i(n)  within base octave
        real_T  ratio_i        { ratios.p[note_position]               };         // get ratio for note i(n)/base_note in base octave
        real_T  i_freq         { base_note_freq * ratio_i * octave_mult};         // get frequency for this note 

         M__(M_out(L"init_scale() -- i=%-3d  octave_dif=%d  note_position=%-2d  octave_mult=%-11.8f  ratio_i=%-14.12f  i_freq=%.12f ")
              % i % octave_diff % note_position % octave_mult % ratio_i % i_freq;
             )
        
        scale.pitches.push_back(i_freq);    
    }
    

    // debug display of whole scale
#if 0
    int i              = scale.lowest_note; 
    real_T prior_pitch = 100000000000000000000000000.0;                       // 1st ratio is invalid
    for (auto pitch : scale.pitches)
    {          
        M_out(L"init_scale() -- note=%-3d  pitch=%-15.8f  ratio=%.18f  cents=%.18f") % i % pitch % (pitch / prior_pitch) % (1200.0 * std::log2(pitch / prior_pitch)); 
        prior_pitch = pitch; 
        i++;
    }
#endif
    return 0; 
}
M_endf



//_____________________________________________________________________________________________________________________
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
////"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//||
//||     ===========
//||     R A N K _ C
//||     ===========
//||
//\\___________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//       rank_C::clean_up() -- clear out any resources and reset to inityial state
//       ==================    ---------------------------------------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rank_C::clean_up() try
{
    // clean out anything in the vectors
    // ---------------------------------

    loudness.clear();  
    pitches.clear();   
    timbre_ix.clear();  
    timbres.clear();    


    // reset POD fields to initial values
    // ----------------------------------

    add_stddev   = 0.0;   
    mult_stddev  = 0.0;    
    pitch_set    = false; 
    timbre_set    = false; 
    
    lowest_note  = -1;   
    highest_note = -1;

    return 0; 
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//       rank_C::display() -- display data items in this rank_C
//       =================    ---------------------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rank_C::display() try
{
    // display POD fields
    // ------------------  

    M_out(L"rank_C::display() -- pitch_set      = %d"  ) % (int)pitch_set;
    M_out(L"rank_C::display() -- timbre_set     = %d"  ) % (int)timbre_set;
    M_out(L"rank_C::display() -- lowest_note    = %d"  ) % lowest_note; 
    M_out(L"rank_C::display() -- highest_note   = %d"  ) % highest_note;
    M_out(L"rank_C::display() -- add_stddev     = %.8f") % add_stddev;
    M_out(L"rank_C::display() -- mult_stddev    = %.8f") % mult_stddev;


    // display pitch, loudness, timbre_ix vectors -- should all be same size
    // --------------------------------------------------------------------

    if (pitch_set)
    {
        for (auto i = lowest_note; i <= highest_note; i++)
        {
            M_out(L"rank_C::display() -- i=%-3d   timbre_ix=%-2d   pitch=%-20.12f  loudness=%-20.12f") % i % timbre_ix.at(i - lowest_note) % pitches.at(i - lowest_note) % loudness.at(i - lowest_note);        
        }  
    }

    return 0; 
}
M_endf  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//       rank_C::set_pitch() -- initialize randomized pitch vector for this rank_C
//       ===================    --------------------------------------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rank_C::set_pitch(const scale_S& scale, int32_t lowest_note_parm, int32_t highest_note_parm, real_T add_stddev_parm, real_T mult_stddev_parm) try
{
    // clear out any leftovers from prior use
    // --------------------------------------

    if (pitch_set)
        clean_up(); 


    // validate lowest/highest note input parms
    // ----------------------------------------

    if (
        (lowest_note_parm < M_12_C0)
        ||
        (lowest_note_parm > M_12_C9)
        ||
        (highest_note_parm < M_12_C0)
        ||
        (highest_note_parm > M_12_C9)
        ||
        (highest_note_parm < lowest_note_parm) 
       )
    {
        M_out_emsg(L"rank_C::set_pitch() -- lowest_note (%d) and/or highest note (%d) parms are invalid -- valid range = %d to %d") % lowest_note % highest_note % M_12_C0 % M_12_C9; 
        return -1;    
    }


    // make sure input scale covers at least lowest note through highest note
    // ----------------------------------------------------------------------

    if ( (scale.lowest_note > lowest_note_parm) || (scale.highest_note < highest_note_parm) )
    {
        M_out_emsg(L"rank_C::set_pitch() -- scale range (notes %d to %d) does not cover range from lowest_note (%d) to highest note (%d) input parms") % scale.lowest_note % scale.highest_note % lowest_note % highest_note; 
        return -1;      
    }


    //  Save notes-based info into rank_C
    //  ---------------------------------

    lowest_note  = lowest_note_parm; 
    highest_note = highest_note_parm; 
    add_stddev   = add_stddev_parm;
    mult_stddev  = mult_stddev_parm;
    pitch_set    = true; 


    // place randomized pitches from scale into pitches vector  also, initializetimbre_ix vector to all -1 (invalid) values and loudness vector to 0.0 
    // ----------------------------------------------------------------------------------------------------------------------------------------------  

    real_T rand_mult   {1.0}; 
    real_T rand_add    {0.0}; 

    std::lognormal_distribution<real_T> mult_dist(0.0, mult_stddev); 
    std::   normal_distribution<real_T> add_dist( 0.0, add_stddev);  


    // loop to set up vectors
    // ----------------------

    for (auto i = lowest_note; i <= highest_note; i++ )
    {
        // compute random additive and multiplicative factors, if requested
        // ----------------------------------------------------------------

        if (add_stddev != 0.0)
            rand_add =  add_dist(global_N::random_engine);  

        if (mult_stddev != 0.0)
            rand_mult = mult_dist(global_N::random_engine);  


        // update vectors
        // -------------- 

        pitches.push_back(rand_add + rand_mult * scale.pitches.at(i - scale.lowest_note));     // save randomized pitch
        timbre_ix.push_back(-1);                                                                // all timbre_ix entries to -1
        loudness.push_back(0.0);                                                               // all loudness entries to 0.0 
    }  

    return 0; 
}
M_endf  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//       rank_C::add_timbre() -- add composite timbre for a note range to this rank_C object
//       ===================    ----------------------------------------------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rank_C::add_timbre(const timbre_S& timbre, int32_t lowest_note_parm, int32_t highest_note_parm, real_T loudness_parm) try
{
    // clear out any leftovers from prior use
    // --------------------------------------

    if (!pitch_set)
    {  
        M_out_emsg(L"rank_C::add_timbre() -- set_pitch() was not done for this rank_C object"); 
        return -1;           
    }

    
    // validate lowest/highest note input parms
    // ----------------------------------------

    if (
        (lowest_note_parm < lowest_note)
        ||
        (lowest_note_parm > highest_note)
        ||
        (highest_note_parm < lowest_note)
        ||
        (highest_note_parm > highest_note)
        ||
        (highest_note_parm < lowest_note_parm) 
       )
    {
        M_out_emsg(L"rank_C::add_note() -- lowest_note (%d) and/or highest note (%d) parms are invalid -- valid range = %d to %d") % lowest_note_parm % highest_note_parm % lowest_note % highest_note; 
        return -1;    
    }


    // add caller's composite timbre_S to timbre vector and save away index to new timbre
    // -----------------------------------------------------------------------------

    timbres.push_back(timbre);
    int32_t new_ix  { (int32_t)(timbres.size() - 1) }; 


    // set timbre_ix to point to new timbre_S for caller's range of notes -- also save loudness for this note range  
    // ----------------------------------------------------------------------------------------------------------

    for (auto i = lowest_note_parm; i <= highest_note_parm; i++)
    {
        timbre_ix.at(i - lowest_note) = new_ix;
        loudness.at(i - lowest_note) = loudness_parm;
    }


    // if timbre_ix has been fully set, mark timbres valid in this rank_S
    // ----------------------------------------------------------------

    auto found = false;        // assume no -1s present

    for (auto i = 0; i < timbre_ix.size(); i++)
    {
        if (timbre_ix.at(i) == -1)
        {
             found = true; 
             break;
        }
    }

    if (!found)
        timbre_set = true;             // no -1s -- timbre_ix is completely set up
  

    return 0;
}
M_endf



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//       rank_C::note_info() -- extract note_info from this rank_C object, given input note number
//       ===================    ------------------------------------------------------------------
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
int rank_C::note_info(int32_t note, const timbre_S*& timbre_p, real_T& pitch_out, real_T& loudness_out) const try
{
    M__(M_out(L"note_info() -- note=%d  lowest_note=%d  highest_note=%d") % note % lowest_note % highest_note;)


    // validate input parms and state of rank_C object
    // -----------------------------------------------

    if (!timbre_set)
    {
        M_out_emsg(L"rank_C::note_info() -- this rank_C object is not fully set up");
        return -1; 
    }

    if ( (note < lowest_note) || (note > highest_note) )
    {
        M_out_emsg(L"rank_C::note_info() -- requested note (%d) not in valid bounds for this rank_C object (%d to %d)") % note % lowest_note % highest_note;
        return -1; 
    }

    pitch_out      =   pitches  .at(note - lowest_note) ; 
    loudness_out   =   loudness .at(note - lowest_note) ; 
    auto ix        =   timbre_ix.at(note - lowest_note) ; 
    timbre_p       =   &(timbres.at(ix                ));

    return 0; 
}
M_endf








//_____________________________________________________________________________________________________________________
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""