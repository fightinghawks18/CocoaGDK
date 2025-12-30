//
// Created by fightinghawks18 on 12/29/2025.
//

#pragma once

#include "export.h"
#include "core/core_types.h"
#include "core/results.h"

typedef enum {
    CCO_AUDIO_CHANNEL_UNKNOWN,
    CCO_AUDIO_CHANNEL_MONO,
    CCO_AUDIO_CHANNEL_STEREO
} cco_audio_channel;

typedef enum {
    CCO_AUDIO_STATE_UNKNOWN,
    CCO_AUDIO_STATE_PLAYING,
    CCO_AUDIO_STATE_PAUSED,
    CCO_AUDIO_STATE_STOPPED
} cco_audio_state;

typedef struct cco_audio_stream_t cco_audio_stream_t;
typedef cco_audio_stream_t *cco_audio_stream;

typedef struct cco_audio_pcm_t cco_audio_pcm_t;
typedef cco_audio_pcm_t *cco_audio_pcm;

CCO_PLATFORM_API cco_result cco_audio_initialize();
CCO_PLATFORM_API void cco_audio_quit();

CCO_PLATFORM_API cco_result cco_audio_pcm_from_file(const char *path, cco_audio_pcm *out_pcm);
CCO_PLATFORM_API void cco_audio_unload_pcm(cco_audio_pcm pcm);

CCO_PLATFORM_API cco_result cco_audio_build_stream_from_pcm(cco_audio_pcm pcm, cco_audio_stream *out_stream);
CCO_PLATFORM_API void cco_audio_unload_stream(cco_audio_stream stream);

CCO_PLATFORM_API void cco_audio_stream_link_pcm(cco_audio_stream stream, cco_audio_pcm pcm);
CCO_PLATFORM_API void cco_audio_stream_play(cco_audio_stream stream);
CCO_PLATFORM_API void cco_audio_stream_play_at_position(cco_audio_stream stream, f32 position);
CCO_PLATFORM_API void cco_audio_stream_pause(cco_audio_stream stream);
CCO_PLATFORM_API void cco_audio_stream_stop(cco_audio_stream stream);
CCO_PLATFORM_API void cco_audio_stream_set_position(cco_audio_stream stream, f32 position);
CCO_PLATFORM_API void cco_audio_stream_set_volume(cco_audio_stream stream, f32 volume);
CCO_PLATFORM_API void cco_audio_stream_set_pan(cco_audio_stream stream, f32 pan);
CCO_PLATFORM_API void cco_audio_stream_set_looped(cco_audio_stream stream, cco_bool looped);
CCO_PLATFORM_API cco_audio_state cco_audio_stream_get_state(cco_audio_stream stream);