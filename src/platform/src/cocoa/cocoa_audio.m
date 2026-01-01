//
// Created by fightinghawks18 on 12/29/25.
//

#include "cocoa_gdk/platform/audio.h"

#import <AudioToolbox/AudioToolbox.h>
#import <CoreAudio/CoreAudio.h>
#import <stdatomic.h>

#include <stdlib.h>

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define MAX_STREAMS 250
#define SAMPLE_RATE 44100

#include "cocoa_gdk/platform/audio_utils.h"
#include "cocoa_gdk/core/log.h"
#include "cocoa_gdk/math/utilities.h"
#include "cocoa_gdk/math/angles.h"

struct cco_audio_pcm_t {
    i16 *samples;
    u32 pcm_frame_count;
    u32 sample_rate;
    u16 bits_per_sample;
    cco_audio_channel channel;
};

struct cco_audio_stream_t {
    _Atomic cco_audio_pcm pcm;
    _Atomic cco_bool looped;
    _Atomic f32 pan;
    _Atomic f32 volume;
    _Atomic i32 position;
    _Atomic cco_audio_state state;
    _Atomic i32 stream_index;
};

typedef struct {
    AudioComponentInstance audio_unit;
    cco_audio_stream streams[MAX_STREAMS];
} audio_system;

static audio_system g_audio_system = {0};

f32 determine_pan_angle(f32 pan) {
    return (pan + 1.0f) * 0.25f * (f32)PI;
}

f32 determine_pan_l(f32 pan) {
    return cosf(determine_pan_angle(pan));
}

f32 determine_pan_r(f32 pan) {
    return sinf(determine_pan_angle(pan));
}

i32 calculate_frame_with_time(cco_audio_stream stream, f32 time) {
    cco_audio_pcm pcm = atomic_load(&stream->pcm);
    
    i32 frame = (i32)(time * (f32)pcm->sample_rate);
    return CCO_CLAMP(frame, 0, pcm->pcm_frame_count - 1);
}

cco_bool handle_end_of_stream(cco_audio_stream stream) {
    cco_audio_pcm pcm = atomic_load(&stream->pcm);
    i32 pos = atomic_load(&stream->position);
    cco_bool looping = atomic_load(&stream->looped);
    cco_bool reached_end = pos > pcm->pcm_frame_count;
    
    if (reached_end && looping) {
        atomic_store(&stream->position, 0);
        return CCO_NO;
    } else if (reached_end && !looping) {
        atomic_store(&stream->state, CCO_AUDIO_STATE_STOPPED);
        return CCO_YES;
    }
    return CCO_NO;
}

void handle_stereo(cco_audio_stream stream, f32 *left, f32 *right, u32 in_number_frames) {
    cco_audio_pcm pcm = atomic_load(&stream->pcm);
    f32 vol = atomic_load(&stream->volume);
    f32 pan = atomic_load(&stream->pan);
    f32 pan_l = determine_pan_l(pan);
    f32 pan_r = determine_pan_r(pan);
    
    for (u32 i = 0; i < in_number_frames; ++i) {
        if (handle_end_of_stream(stream))
            return;
        
        i32 pos = atomic_load(&stream->position);
        
        i32 sample_index = pos * pcm->channel;
        
        f32 sample_left = pcm->samples[sample_index] * (1.0f / 32768.0f);
        f32 sample_right = pcm->samples[sample_index + 1] * (1.0f / 32768.0f);
        
        left[i] += (sample_left * pan_l) * vol;
        right[i] += (sample_right * pan_r) * vol;
        
        atomic_fetch_add(&stream->position, 1);
    }
}

static OSStatus proc_audio(void *in_ref_con, AudioUnitRenderActionFlags *io_action_flags,
                           const AudioTimeStamp *in_time_stamp, u32 in_bus_number,
                           u32 in_number_frames, AudioBufferList *io_data) {
    f32 *left = (f32 *)io_data->mBuffers[0].mData;
    f32 *right = (f32 *)io_data->mBuffers[1].mData;
    
    memset(left, 0, in_number_frames * sizeof(f32));
    memset(right, 0, in_number_frames * sizeof(f32));
    
    for (usize s = 0; s < MAX_STREAMS; ++s) {
        cco_audio_stream stream = g_audio_system.streams[s];
        if (!stream)
            continue;
        
        if (atomic_load(&stream->state) != CCO_AUDIO_STATE_PLAYING)
            continue;
        handle_stereo(stream, left, right, in_number_frames);
    }
    
    return noErr;
}

cco_result cco_audio_initialize() {
    AudioComponentDescription audio_component_desc = {
        .componentType = kAudioUnitType_Output,
        .componentSubType = kAudioUnitSubType_DefaultOutput,
        .componentManufacturer = kAudioUnitManufacturer_Apple
    };
    
    AudioComponent com = AudioComponentFindNext(CCO_NIL, &audio_component_desc);
    AudioComponentInstanceNew(com, &g_audio_system.audio_unit);
    
    AudioStreamBasicDescription fmt = {
        .mSampleRate = SAMPLE_RATE,
        .mFormatID = kAudioFormatLinearPCM,
        .mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked |
        kAudioFormatFlagIsNonInterleaved,
        .mBytesPerPacket = sizeof(f32),
        .mFramesPerPacket = 1,
        .mBytesPerFrame = sizeof(f32),
        .mChannelsPerFrame = 2,
        .mBitsPerChannel = 32
    };
    
    AudioUnitSetProperty(g_audio_system.audio_unit, kAudioUnitProperty_StreamFormat,
                         kAudioUnitScope_Input, 0, &fmt, sizeof(fmt));
    AURenderCallbackStruct cb = { .inputProc = proc_audio, .inputProcRefCon = &g_audio_system };
    AudioUnitSetProperty(g_audio_system.audio_unit, kAudioUnitProperty_SetRenderCallback,
                         kAudioUnitScope_Input, 0, &cb, sizeof(cb));
    
    AudioUnitInitialize(g_audio_system.audio_unit);
    AudioOutputUnitStart(g_audio_system.audio_unit);
    
    return CCO_SUCCESS;
}

void cco_audio_quit() {
    AudioOutputUnitStop(g_audio_system.audio_unit);
    AudioUnitUninitialize(g_audio_system.audio_unit);
    AudioComponentInstanceDispose(g_audio_system.audio_unit);
}

cco_result cco_audio_pcm_from_file(const char *path, cco_audio_pcm *out_pcm) {
    drwav wav;
    if (!drwav_init_file(&wav, path, NULL)) {
        CCO_LOG("Failed to load audio file %s! Does this path exist and is a .wav file?", path);
        return CCO_FAIL_AUDIO_CREATE_PCM_ERROR;
    }
    
    cco_audio_pcm pcm = malloc(sizeof(cco_audio_pcm_t));
    if (!pcm)
        return CCO_FAIL_OUT_OF_MEMORY;
    
    const u32 total_sample_count = (u32)wav.totalPCMFrameCount * (u32)wav.channels;
    pcm->samples = malloc(sizeof(i16) * total_sample_count);
    
    drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pcm->samples);
    
    if (wav.totalPCMFrameCount != SAMPLE_RATE) {
        u32 new_frames = 0;
        
        i16 *resampled = cco_resample_audio_i16(pcm->samples, (u32)wav.totalPCMFrameCount, wav.channels,
                                            wav.sampleRate, SAMPLE_RATE,
                                       &new_frames);
        free(pcm->samples);
        pcm->samples = resampled;
        pcm->pcm_frame_count = new_frames;
    } else {
        pcm->pcm_frame_count = (u32)wav.totalPCMFrameCount;
    }
    

    pcm->channel = wav.channels;
    pcm->bits_per_sample = 16;
    pcm->sample_rate = wav.sampleRate;
    
    drwav_uninit(&wav);
    
    *out_pcm = pcm;
    return CCO_SUCCESS;
}

void cco_audio_unload_pcm(cco_audio_pcm pcm) {
    if (pcm->samples) {
        free(pcm->samples);
        pcm->samples = CCO_NIL;
    }
    free(pcm);
}

cco_result cco_audio_build_stream_from_pcm(cco_audio_pcm pcm, cco_audio_stream *out_stream) {
    i32 stream_index = -1;
    for (i32 i = 0; i < MAX_STREAMS; i++) {
        if (!g_audio_system.streams[i]) {
            stream_index = i;
            break;
        }
    }
    if (stream_index == -1) {
        CCO_LOG("Unable to create new stream, max limit %d has been reached!", MAX_STREAMS);
        return CCO_FAIL_AUDIO_STREAM_CREATE_ERROR;
    }
    
    cco_audio_stream stream = malloc(sizeof(cco_audio_stream_t));
    if (!stream)
        return CCO_FAIL_OUT_OF_MEMORY;
    
    atomic_store(&stream->pcm, pcm);
    atomic_store(&stream->volume, 1.0f);
    atomic_store(&stream->pan, 0.0f);
    atomic_store(&stream->looped, CCO_NO);
    atomic_store(&stream->state, CCO_AUDIO_STATE_STOPPED);
    atomic_store(&stream->position, 0.0f);
    atomic_store(&stream->stream_index, stream_index);
    
    g_audio_system.streams[stream_index] = stream;
    
    *out_stream = stream;
    return CCO_SUCCESS;
}
void cco_audio_unload_stream(cco_audio_stream stream) {
    i32 stream_index = atomic_load(&stream->stream_index);
    if (g_audio_system.streams[stream_index] != stream) {
        free(stream);
        return;
    }
    
    atomic_store(&stream->state, CCO_AUDIO_STATE_STOPPED);
    atomic_store(&stream->pcm, CCO_NIL);
    
    g_audio_system.streams[atomic_load(&stream->stream_index)] = CCO_NIL;
    free(stream);
}

void cco_audio_stream_link_pcm(cco_audio_stream stream, cco_audio_pcm pcm) {
    atomic_store(&stream->pcm, pcm);
}

void cco_audio_stream_play(cco_audio_stream stream) {
    atomic_store(&stream->state, CCO_AUDIO_STATE_PLAYING);
}

void cco_audio_stream_play_at_position(cco_audio_stream stream, f32 position) {
    atomic_store(&stream->position, position);
    atomic_store(&stream->state, CCO_AUDIO_STATE_PLAYING);
}

void cco_audio_stream_pause(cco_audio_stream stream) {
    atomic_store(&stream->state, CCO_AUDIO_STATE_PAUSED);
}

void cco_audio_stream_stop(cco_audio_stream stream) {
    atomic_store(&stream->state, CCO_AUDIO_STATE_STOPPED);
    atomic_store(&stream->position, 0);
}

void cco_audio_stream_set_position(cco_audio_stream stream, f32 position) {
    i32 frame = calculate_frame_with_time(stream, position);
    atomic_store(&stream->position, frame);
}

void cco_audio_stream_set_volume(cco_audio_stream stream, f32 volume) {
    atomic_store(&stream->volume, CCO_CLAMP(volume, 0.0f, 2.0f));
}

void cco_audio_stream_set_pan(cco_audio_stream stream, f32 pan) {
    atomic_store(&stream->pan, CCO_CLAMP(pan, -1.0f, 1.0f));
}

void cco_audio_stream_set_looped(cco_audio_stream stream, cco_bool looped) {
    atomic_store(&stream->looped, looped);
}

cco_audio_state cco_audio_stream_get_state(cco_audio_stream stream) { return atomic_load(&stream->state); }
