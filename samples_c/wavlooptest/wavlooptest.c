/* wavtest.c */
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "ecrobot_private.h"

/* OSEK declarations */
DeclareTask(Task1); 

/* LEJOS OSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){}

/*
 * a wav file can be accessed by using following macros:
 * E.g lego_mindstorms_nxt.wav
 * EXTERNAL_WAV_DATA(file name without extension); <- This is external declarations
 * WAV_DATA_START(file name without extension)     <- start address of a wav file
 * WAV_DATA_END(file name without extension)       <- end address of a wav file
 * WAV_DATA_SIZE(file name without extension)      <- size of a wav file 
 */
EXTERNAL_WAV_DATA(lego_mindstorms_nxt);

typedef struct {
    U32 chunkID;
    U32 chunkSize;
    U8  data[];
} __attribute__((packed)) WAV_DATA;

static U8 play_flag = 0;
static U8 *wav_data = 0;
static U32 wav_samples = 0;

U8 wav_end_callback(void)
{
    if (play_flag != 0) {
        sound_play_stream_next(wav_data, wav_samples);
        return 1;
    } else {
        return 0; /* Stop */
    }
}

TASK(Task1)
{
    U32 wav_srate;
    WAV *wav = (WAV *)WAV_DATA_START(lego_mindstorms_nxt);

    display_clear(0);
    display_goto_xy(0, 0);
    display_string("WAV LOOP TEST");
    display_goto_xy(0, 2);
    display_string("PRESS ENTR");
    display_update();

    if ((wav->riff.chunkID == RIFF_CHUNK_ID) &&
        (wav->riff.format == RIFF_FORMAT) &&
        (wav->fmt.chunkID == FMT_CHUNK_ID) &&
        (wav->fmt.audioFormat == 0x0001) && /* PCM */
        (wav->fmt.numChannels == 0x0001) && /* mono channel */
        (wav->fmt.bitsPerSample == 0x0008)) { /* 8bit */
        WAV_DATA *data = (WAV_DATA*)((U8*)&(wav->fmt.audioFormat) + wav->fmt.chunkSize);
        wav_data = data->data;
        wav_samples = data->chunkSize & ~(7);
        wav_srate = wav->fmt.sampleRate;
    } else {
        goto TASK_END;
    }

    sound_play_stream_setup(wav_srate, 70);
    while(1)
    {
        if (ecrobot_is_ENTER_button_pressed())
        {
            if (play_flag == 0) {
                sound_play_stream(wav_data, 
                                  wav_samples,
                                  wav_end_callback);
            }
            play_flag ^= 1;

            while (ecrobot_is_ENTER_button_pressed())
                systick_wait_ms(10);
        }
        systick_wait_ms(10);
    }

TASK_END:
    TerminateTask();
}
