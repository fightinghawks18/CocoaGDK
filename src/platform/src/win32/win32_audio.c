//
// Created by fightinghawks18 on 12/29/2025.
//

#include "platform/audio.h"
#include <xaudio2.h>

#include "math/angles.h"
#include "math/utilities.h"
#include "core/log.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

struct cco_audio_pcm_t {
    void *samples;
    cco_audio_channel channel;
    u32 sample_rate;
    u16 bits_per_sample;
    u32 pcm_frame_count;
    WAVEFORMATEX fmt;
};

struct cco_audio_stream_t {
    IXAudio2VoiceCallback callbacks;
    IXAudio2SourceVoice *voice;
    cco_audio_pcm pcm;
    cco_audio_state state;
    f32 volume;
    f32 pan;
    cco_bool looped;
};

typedef struct {
    IXAudio2 *xaudio;
    IXAudio2MasteringVoice *master_voice;
    volatile LONG running;
} audio_system;

static audio_system g_audio = {0};

void submit_source_buffer(cco_audio_stream stream, const f32 offset) {

    cco_audio_pcm pcm = stream->pcm;

    const u32 frame_offset = CCO_CLAMP(offset * pcm->sample_rate, 0.0f, pcm->pcm_frame_count - 1);
    const u32 byte_offset = frame_offset * pcm->channel * sizeof(i16);

    const u32 audio_bytes = (pcm->pcm_frame_count - frame_offset) * pcm->channel * sizeof(i16);

    XAUDIO2_BUFFER audio_buffer = {0};
    audio_buffer.pAudioData = (BYTE *)pcm->samples + byte_offset;
    audio_buffer.AudioBytes = audio_bytes;
    audio_buffer.pContext = NULL;
    audio_buffer.LoopCount = stream->looped ? XAUDIO2_LOOP_INFINITE : 0;
    audio_buffer.LoopBegin = 0;
    audio_buffer.LoopLength = 0;
    audio_buffer.Flags = XAUDIO2_END_OF_STREAM;

    IXAudio2SourceVoice_SubmitSourceBuffer(stream->voice, &audio_buffer, NULL);
}

f32 get_playback_position(cco_audio_stream stream) {
    if (stream->state == CCO_AUDIO_STATE_STOPPED)
        return 0.0f;

    XAUDIO2_VOICE_STATE state;
    IXAudio2SourceVoice_GetState(stream->voice, &state, 0);

    const u64 samples_played = state.SamplesPlayed;
    const u32 sample_rate = stream->pcm->sample_rate;
    return (f32)samples_played / (f32)sample_rate;
}

CCO_PLATFORM_API cco_result cco_audio_initialize() {
    HRESULT hr = XAudio2Create(&g_audio.xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) {
        CCO_LOG("Failed to create XAudio2 device!");
        CoUninitialize();
        return CCO_FAIL_AUDIO_INIT_ERROR;
    }

    hr = IXAudio2_CreateMasteringVoice(g_audio.xaudio, &g_audio.master_voice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE,
                                       0, NULL, NULL, AudioCategory_GameEffects);
    if (FAILED(hr)) {
        CCO_LOG("Failed to create master voice for XAudio2");
        cco_audio_quit();
        return CCO_FAIL_AUDIO_INIT_ERROR;
    }

    return CCO_SUCCESS;
}

CCO_PLATFORM_API void cco_audio_quit() {
    if (g_audio.master_voice) {
        IXAudio2MasteringVoice_DestroyVoice(g_audio.master_voice);
        g_audio.master_voice = CCO_NIL;
    }
    IXAudio2_Release(g_audio.xaudio);
}

CCO_PLATFORM_API cco_result cco_audio_pcm_from_file(const char *path, cco_audio_pcm *out_pcm) {
    drwav wav;
    if (!drwav_init_file(&wav, path, NULL)) {
        CCO_LOG("Failed to load audio file %s! Does this path exist and is a .wav file?", path);
        return CCO_FAIL_AUDIO_CREATE_PCM_ERROR;
    }

    cco_audio_pcm pcm = malloc(sizeof(cco_audio_pcm_t));
    if (!pcm) {
        drwav_uninit(&wav);
        return CCO_FAIL_AUDIO_CREATE_PCM_ERROR;
    }

    const u32 total_sample_count = wav.totalPCMFrameCount * wav.channels;
    pcm->samples = malloc(sizeof(i16) * total_sample_count);
    drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pcm->samples);

    pcm->channel = wav.channels;
    pcm->pcm_frame_count = wav.totalPCMFrameCount;
    pcm->bits_per_sample = 16;
    pcm->sample_rate = wav.sampleRate;
    pcm->fmt.wFormatTag = WAVE_FORMAT_PCM;
    pcm->fmt.nChannels = pcm->channel;
    pcm->fmt.nSamplesPerSec = pcm->sample_rate;
    pcm->fmt.wBitsPerSample = pcm->bits_per_sample;
    pcm->fmt.nBlockAlign = (pcm->channel * pcm->bits_per_sample) / 8;
    pcm->fmt.nAvgBytesPerSec = pcm->sample_rate * pcm->fmt.nBlockAlign;
    pcm->fmt.cbSize = 0;

    drwav_uninit(&wav);

    *out_pcm = pcm;
    return CCO_SUCCESS;
}

CCO_PLATFORM_API void cco_audio_unload_pcm(cco_audio_pcm pcm) {
    if (pcm->samples) {
        free(pcm->samples);
        pcm->samples = CCO_NIL;
    }
}

CCO_PLATFORM_API cco_result cco_audio_build_stream_from_pcm(cco_audio_pcm pcm, cco_audio_stream *out_stream) {
    cco_audio_stream stream = malloc(sizeof(cco_audio_stream_t));
    if (!stream)
        return CCO_FAIL_OUT_OF_MEMORY;

    IXAudio2VoiceCallback callbacks = {0};

    stream->pcm = pcm;
    stream->volume = 1.0f;
    stream->pan = 0.0f;
    stream->looped = CCO_NO;
    stream->callbacks = callbacks;
    stream->state = CCO_AUDIO_STATE_STOPPED;

    const HRESULT hr = IXAudio2_CreateSourceVoice(g_audio.xaudio, &stream->voice, &pcm->fmt, 0, XAUDIO2_MAX_FREQ_RATIO, CCO_NIL, CCO_NIL,
                               CCO_NIL);
    if (FAILED(hr)) {
        CCO_LOG("Failed to create source voice! %ld", hr);
        free(stream);
        return CCO_FAIL_AUDIO_STREAM_CREATE_ERROR;
    }
    submit_source_buffer(stream, 0.0f);

    *out_stream = stream;
    return CCO_SUCCESS;
}

CCO_PLATFORM_API void cco_audio_unload_stream(cco_audio_stream stream) {
    if (stream->voice) {
        IXAudio2SourceVoice_DestroyVoice(stream->voice);
        stream->voice = CCO_NIL;
    }
    free(stream);
}

CCO_PLATFORM_API void cco_audio_stream_link_pcm(cco_audio_stream stream, cco_audio_pcm pcm) {
    const cco_bool sample_rate_matches = stream->pcm->fmt.nSamplesPerSec == pcm->fmt.nSamplesPerSec;
    const cco_bool bit_rate_matches = stream->pcm->fmt.wBitsPerSample == pcm->fmt.wBitsPerSample;
    const cco_bool channel_matches = stream->pcm->fmt.nChannels == pcm->fmt.nChannels;
    if (!sample_rate_matches || !bit_rate_matches || !channel_matches) {
        CCO_LOG("Attempted to link an audio stream with a format that it doesn't support!");
        CCO_LOG("Sample Rate match? %s\n Bit Rate match? %s\n Channel match? %s\n ", sample_rate_matches ? "YES" : "NO",
                bit_rate_matches ? "YES" : "NO", channel_matches ? "YES" : "NO");
        return;
    }
    stream->pcm = pcm;
    submit_source_buffer(stream, 0.0f);
}

CCO_PLATFORM_API void cco_audio_stream_play(cco_audio_stream stream) {
    IXAudio2SourceVoice_Start(stream->voice, 0, XAUDIO2_COMMIT_NOW);
    stream->state = CCO_AUDIO_STATE_PLAYING;
}

CCO_PLATFORM_API void cco_audio_stream_play_at_position(cco_audio_stream stream, const f32 position) {
    if (position > 0.0f)
        cco_audio_stream_set_position(stream, position);
    cco_audio_stream_play(stream);
}

CCO_PLATFORM_API void cco_audio_stream_pause(cco_audio_stream stream) {
    IXAudio2SourceVoice_Stop(stream->voice, 0, XAUDIO2_COMMIT_NOW);
    stream->state = CCO_AUDIO_STATE_PAUSED;
}

CCO_PLATFORM_API void cco_audio_stream_stop(cco_audio_stream stream) {
    IXAudio2SourceVoice_Stop(stream->voice, 0, XAUDIO2_COMMIT_NOW);
    IXAudio2SourceVoice_FlushSourceBuffers(stream->voice);
    stream->state = CCO_AUDIO_STATE_STOPPED;
}

CCO_PLATFORM_API void cco_audio_stream_set_position(cco_audio_stream stream, const f32 position) {
    const cco_audio_state old_state = stream->state;
    if (stream->state != CCO_AUDIO_STATE_STOPPED)
        cco_audio_stream_stop(stream);

    submit_source_buffer(stream, position);

    if (old_state == CCO_AUDIO_STATE_PLAYING)
        cco_audio_stream_play(stream);
}

CCO_PLATFORM_API void cco_audio_stream_set_volume(cco_audio_stream stream, const f32 volume) {
    IXAudio2SourceVoice_SetVolume(stream->voice, CCO_CLAMP(volume, 0.0f, 1.0f), XAUDIO2_COMMIT_NOW);
}

CCO_PLATFORM_API void cco_audio_stream_set_pan(cco_audio_stream stream, f32 pan) {
    const f32 angle = (pan + 1.0f) * 0.25f * (f32)PI;

    const f32 left_pan = cosf(angle);
    const f32 right_pan = sinf(angle);

    const f32 output_matrix[4] = {left_pan, 0.0f, 0.0f, right_pan};

    IXAudio2SourceVoice_SetOutputMatrix(stream->voice, NULL, 2, 2, output_matrix, XAUDIO2_COMMIT_NOW);
}

CCO_PLATFORM_API void cco_audio_stream_set_looped(cco_audio_stream stream, cco_bool looped) {
    const f32 last_position = get_playback_position(stream);
    const cco_audio_state old_state = stream->state;
    if (stream->state != CCO_AUDIO_STATE_STOPPED)
        cco_audio_stream_stop(stream);

    stream->looped = looped;
    submit_source_buffer(stream, last_position);

    if (old_state == CCO_AUDIO_STATE_PLAYING)
        cco_audio_stream_play(stream);
}

CCO_PLATFORM_API cco_audio_state cco_audio_stream_get_state(cco_audio_stream stream) { return stream->state; }