//
// Created by fightinghawks18 on 12/30/25.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cocoa_gdk/core/core_types.h"
#include <stdlib.h>

static i16 *cco_resample_audio_i16(const i16 *input, const u32 input_frames, const u32 channels, const u32 input_rate,
                                   const u32 output_rate, u32 *output_frames) {

    *output_frames = (u32)((u64)input_frames * output_rate / input_rate);

    i16 *output = malloc(*output_frames * channels * sizeof(i16));

    const f64 ratio = (f64)input_frames / (f64)*output_frames;

    for (u32 i = 0; i < *output_frames; i++) {
        const f64 src_pos = i * ratio;
        const u32 src_idx = (u32)src_pos;
        const f64 frac = src_pos - src_idx;

        for (u32 ch = 0; ch < channels; ch++) {
            if (src_idx + 1 < input_frames) {
                const i16 sample1 = input[src_idx * channels + ch];
                const i16 sample2 = input[(src_idx + 1) * channels + ch];

                const f32 result = sample1 + (sample2 - sample1) * frac;
                output[i * channels + ch] = (i16)result;
            } else {
                output[i * channels + ch] = input[src_idx * channels + ch];
            }
        }
    }

    return output;
}

#ifdef __cplusplus
}
#endif