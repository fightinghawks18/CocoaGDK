//
// Created by fightinghawks18 on 12/12/25.
//

#pragma once

#include "../core.h"

typedef struct CcoVulkanCore_T CcoVulkanCore_T;
typedef CcoVulkanCore_T *CcoVulkanCore;

typedef enum {
    CCO_GPU_POWER_PREFERENCE_LOW,
    CCO_GPU_POWER_PREFERENCE_MED,
    CCO_GPU_POWER_PREFERENCE_HI
} CcoGPUPowerPreference;

typedef enum { CCO_GPU_QUEUE_GRAPHICS, CCO_GPU_QUEUE_TRANSFER, CCO_GPU_QUEUE_COMPUTE } CcoGPUQueue;

typedef struct {
    CcoGPUPowerPreference gpuPowerPreference;
    CcoGPUQueue desiredQueues[3];
    u32 desiredQueueCount;
} CcoVulkanCoreDesc;

CcoResult ccoCreateVulkanCore(const CcoVulkanCoreDesc *desc, CcoVulkanCore *outCore);
void ccoDestroyVulkanCore(CcoVulkanCore core);