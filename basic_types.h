#ifndef _BASIC_TYPES_H_
#define _BASIC_TYPES_H_

#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <vector>
typedef uint64_t ulong;
typedef void* any_ptr;
typedef unsigned char byte;
typedef uint16_t ushort;
typedef uint32_t uint;
typedef int32_t int32;

using time_point = std::chrono::high_resolution_clock::time_point;
using system_clock = std::chrono::high_resolution_clock;

#endif // _BASIC_TYPES_H_