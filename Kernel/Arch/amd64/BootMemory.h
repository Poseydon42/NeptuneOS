#pragma once

#include <stdint.h>

#define KERNEL_OFFSET 0xFFFFFF8000000000

#define V2P(a) ((uintptr_t)(a) & ~KERNEL_OFFSET)
#define P2V(a) ((uintptr_t)(a) | KERNEL_OFFSET)
