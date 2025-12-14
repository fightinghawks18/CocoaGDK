//
// Created by fightinghawks18 on 12/14/25.
//

#pragma once

typedef enum {
    CCO_GPU_POWER_PREFERENCE_LOW,
    CCO_GPU_POWER_PREFERENCE_MED,
    CCO_GPU_POWER_PREFERENCE_HI
} CcoGPUPowerPreference;

typedef enum {
    CCO_GPU_QUEUE_NONE,
    CCO_GPU_QUEUE_GRAPHICS,
    CCO_GPU_QUEUE_TRANSFER,
    CCO_GPU_QUEUE_COMPUTE
} CcoGPUQueueType;
typedef u32 CcoGPUQueueTypeFlags;

typedef struct {
    CcoGPUPowerPreference gpuPowerPreference;
    CcoGPUQueueTypeFlags *desiredQueues;
    u32 desiredQueueCount;
} CcoVulkanCoreDesc;