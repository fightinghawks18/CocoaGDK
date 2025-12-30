//
// Created by fightinghawks18 on 12/26/2025.
//

#pragma once

#include <stddef.h>
#include <stdio.h>

#ifdef DEBUG
#define CCO_LOG(str, ...) printf(str "\n", ##__VA_ARGS__);
#else
#define CCO_LOG(str, ...)
#endif