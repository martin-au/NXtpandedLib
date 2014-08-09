#ifndef SOUND_H_
#define SOUND_H_

#include "mytypes.h"

void sound_init();
void sound_interrupt_enable();
void sound_interrupt_disable();
void sound_enable();
void sound_disable();
void sound_isr_C();

void sound_freq(U32 freq, U32 ms);
void sound_freq_vol(U32 freq, U32 ms, int vol);
void sound_play_sample(U8 *data, U32 length, U32 freq, int vol);
void sound_play_stream_setup(U32 freq, int vol);
void sound_play_stream(U8 *data, U32 length, U8 (*callback)());
void sound_play_stream_next(U8 *data, U32 length);
U8 sound_get_play_status();
void sound_set_volume(int vol);
int sound_get_volume();
int sound_get_time();

#define MAXVOL 100

#endif /*SOUND_H_*/
