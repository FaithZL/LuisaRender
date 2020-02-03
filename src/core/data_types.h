//
// Created by Mike Smith on 2020/1/30.
//

#pragma once

#ifndef LUISA_DEVICE_COMPATIBLE

// address spaces, ineffective on host
#define LUISA_CONSTANT_SPACE
#define LUISA_UNIFORM_SPACE
#define LUISA_THREAD_SPACE
#define LUISA_DEVICE_SPACE

// function scopes
#define LUISA_CONSTEXPR        constexpr
#define LUISA_KERNEL
#define LUISA_DEVICE_CALLABLE

#include <type_traits>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace luisa {

using uint = uint32_t;

using float2 = glm::aligned_highp_vec2;
using float3 = glm::aligned_highp_vec3;
using float4 = glm::aligned_highp_vec4;
using packed_float3 = glm::packed_highp_vec3;

using int2 = glm::aligned_highp_ivec2;
using int3 = glm::aligned_highp_ivec3;
using int4 = glm::aligned_highp_ivec4;
using packed_int3 = glm::packed_highp_ivec3;

using uint2 = glm::aligned_highp_uvec2;
using uint3 = glm::aligned_highp_uvec3;
using uint4 = glm::aligned_highp_uvec4;
using packed_uint3 = glm::packed_highp_uvec3;

using float3x3 = glm::aligned_highp_mat3x3;
using float4x4 = glm::aligned_highp_mat4x4;

template<typename DestT, typename SrcT, std::enable_if_t<sizeof(SrcT) == sizeof(DestT), int> = 0>
inline DestT as(SrcT s) noexcept {
    return reinterpret_cast<DestT &>(s);
}

}

#endif

// make sure the necessary memory space and function scope keywords are defined
#ifndef LUISA_CONSTANT_SPACE
#error Marco LUISA_CONSTANT_SPACE not defined.
#endif

#ifndef LUISA_UNIFORM_SPACE
#error Marco LUISA_UNIFORM_SPACE not defined.
#endif

#ifndef LUISA_THREAD_SPACE
#error Marco LUISA_THREAD_SPACE not defined.
#endif

#ifndef LUISA_DEVICE_SPACE
#error Marco LUISA_DEVICE_SPACE not defined.
#endif

#ifndef LUISA_CONSTEXPR
#error Marco LUISA_CONSTEXPR not defined.
#endif

#ifndef LUISA_KERNEL
#error Marco LUISA_KERNEL not defined.
#endif

#ifndef LUISA_DEVICE_CALLABLE
#error Marco LUISA_DEVICE_CALLABLE not defined.
#endif

namespace luisa {

static_assert(sizeof(float) == 4ul);
static_assert(sizeof(float2) == 8ul);
static_assert(sizeof(float3) == 16ul);
static_assert(sizeof(float4) == 16ul);
static_assert(sizeof(packed_float3) == 12ul);

static_assert(alignof(float) == 4ul);
static_assert(alignof(float2) == 8ul);
static_assert(alignof(float3) == 16ul);
static_assert(alignof(float4) == 16ul);
static_assert(alignof(packed_float3) == 4ul);

static_assert(sizeof(int) == 4ul);
static_assert(sizeof(int2) == 8ul);
static_assert(sizeof(int3) == 16ul);
static_assert(sizeof(int4) == 16ul);
static_assert(sizeof(packed_int3) == 12ul);

static_assert(alignof(int) == 4ul);
static_assert(alignof(int2) == 8ul);
static_assert(alignof(int3) == 16ul);
static_assert(alignof(int4) == 16ul);
static_assert(alignof(packed_int3) == 4ul);

static_assert(sizeof(uint) == 4ul);
static_assert(sizeof(uint2) == 8ul);
static_assert(sizeof(uint3) == 16ul);
static_assert(sizeof(uint4) == 16ul);
static_assert(sizeof(packed_uint3) == 12ul);

static_assert(alignof(uint) == 4ul);
static_assert(alignof(uint2) == 8ul);
static_assert(alignof(uint3) == 16ul);
static_assert(alignof(uint4) == 16ul);
static_assert(alignof(packed_uint3) == 4ul);

static_assert(sizeof(float3x3) == 48ul);
static_assert(sizeof(float4x4) == 64ul);
static_assert(alignof(float3x3) == 16ul);
static_assert(alignof(float4x4) == 16ul);

LUISA_DEVICE_CALLABLE inline float2 make_float2() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline float2 make_float2(float s) noexcept { return {s, s}; }
LUISA_DEVICE_CALLABLE inline float2 make_float2(float x, float y) noexcept { return {x, y}; }
LUISA_DEVICE_CALLABLE inline float2 make_float2(float3 v) noexcept { return {v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline float2 make_float2(float4 v) noexcept { return {v.x, v.y}; }

LUISA_DEVICE_CALLABLE inline float3 make_float3() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(float s) noexcept { return {s, s, s}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(float x, float y, float z) noexcept { return {x, y, z}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(float2 v, float z) noexcept { return {v.x, v.y, z}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(float x, float2 v) noexcept { return {x, v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(float4 v) noexcept { return {v.x, v.y, v.z}; }

LUISA_DEVICE_CALLABLE inline float4 make_float4() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(float s) noexcept { return {s, s, s, s}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(float x, float y, float z, float w) noexcept { return {x, y, z, w}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(float2 v, float z, float w) noexcept { return {v.x, v.y, z, w}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(float x, float y, float2 v) noexcept { return {x, y, v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(float x, float2 v, float w) noexcept { return {x, v.x, v.y, w}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(float2 v, float2 u) noexcept { return {v.x, v.y, u.x, u.y}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(float3 v, float w) noexcept { return {v.x, v.y, v.z, w}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(float x, float3 v) noexcept { return {x, v.x, v.y, v.z}; }

LUISA_DEVICE_CALLABLE inline int2 make_int2() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline int2 make_int2(int s) noexcept { return {s, s}; }
LUISA_DEVICE_CALLABLE inline int2 make_int2(int x, int y) noexcept { return {x, y}; }
LUISA_DEVICE_CALLABLE inline int2 make_int2(int3 v) noexcept { return {v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline int2 make_int2(int4 v) noexcept { return {v.x, v.y}; }

LUISA_DEVICE_CALLABLE inline int3 make_int3() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(int s) noexcept { return {s, s, s}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(int x, int y, int z) noexcept { return {x, y, z}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(int2 v, int z) noexcept { return {v.x, v.y, z}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(int x, int2 v) noexcept { return {x, v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(int4 v) noexcept { return {v.x, v.y, v.z}; }

LUISA_DEVICE_CALLABLE inline int4 make_int4() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(int s) noexcept { return {s, s, s, s}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(int x, int y, int z, int w) noexcept { return {x, y, z, w}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(int2 v, int z, int w) noexcept { return {v.x, v.y, z, w}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(int x, int y, int2 v) noexcept { return {x, y, v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(int x, int2 v, int w) noexcept { return {x, v.x, v.y, w}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(int2 v, int2 u) noexcept { return {v.x, v.y, u.x, u.y}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(int3 v, int w) noexcept { return {v.x, v.y, v.z, w}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(int x, int3 v) noexcept { return {x, v.x, v.y, v.z}; }

LUISA_DEVICE_CALLABLE inline uint2 make_uint2() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline uint2 make_uint2(uint s) noexcept { return {s, s}; }
LUISA_DEVICE_CALLABLE inline uint2 make_uint2(uint x, uint y) noexcept { return {x, y}; }
LUISA_DEVICE_CALLABLE inline uint2 make_uint2(uint3 v) noexcept { return {v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline uint2 make_uint2(uint4 v) noexcept { return {v.x, v.y}; }

LUISA_DEVICE_CALLABLE inline uint3 make_uint3() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(uint s) noexcept { return {s, s, s}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(uint x, uint y, uint z) noexcept { return {x, y, z}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(uint2 v, uint z) noexcept { return {v.x, v.y, z}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(uint x, uint2 v) noexcept { return {x, v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(uint4 v) noexcept { return {v.x, v.y, v.z}; }

LUISA_DEVICE_CALLABLE inline uint4 make_uint4() noexcept { return {}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(uint s) noexcept { return {s, s, s, s}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(uint x, uint y, uint z, uint w) noexcept { return {x, y, z, w}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(uint2 v, uint z, uint w) noexcept { return {v.x, v.y, z, w}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(uint x, uint y, uint2 v) noexcept { return {x, y, v.x, v.y}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(uint x, uint2 v, uint w) noexcept { return {x, v.x, v.y, w}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(uint2 v, uint2 u) noexcept { return {v.x, v.y, u.x, u.y}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(uint3 v, uint w) noexcept { return {v.x, v.y, v.z, w}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(uint x, uint3 v) noexcept { return {x, v.x, v.y, v.z}; }

LUISA_DEVICE_CALLABLE inline float2 make_float2(int2 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y)}; }
LUISA_DEVICE_CALLABLE inline float2 make_float2(uint2 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y)}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(int3 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(uint3 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(int4 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z), static_cast<float>(v.w)}; }
LUISA_DEVICE_CALLABLE inline float4 make_float4(uint4 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z), static_cast<float>(v.w)}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(packed_int3 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(packed_uint3 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)}; }
LUISA_DEVICE_CALLABLE inline float3 make_float3(packed_float3 v) noexcept { return {v.x, v.y, v.z}; }

LUISA_DEVICE_CALLABLE inline int2 make_int2(float2 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y)}; }
LUISA_DEVICE_CALLABLE inline int2 make_int2(uint2 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y)}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(float3 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(uint3 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(float4 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), static_cast<int>(v.w)}; }
LUISA_DEVICE_CALLABLE inline int4 make_int4(uint4 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), static_cast<int>(v.w)}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(packed_int3 v) noexcept { return {v.x, v.y, v.z}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(packed_uint3 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)}; }
LUISA_DEVICE_CALLABLE inline int3 make_int3(packed_float3 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)}; }

LUISA_DEVICE_CALLABLE inline uint2 make_uint2(float2 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y)}; }
LUISA_DEVICE_CALLABLE inline uint2 make_uint2(int2 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y)}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(float3 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z)}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(int3 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z)}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(float4 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z), static_cast<uint>(v.w)}; }
LUISA_DEVICE_CALLABLE inline uint4 make_uint4(int4 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z), static_cast<uint>(v.w)}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(packed_int3 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z)}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(packed_uint3 v) noexcept { return {v.x, v.y, v.z}; }
LUISA_DEVICE_CALLABLE inline uint3 make_uint3(packed_float3 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z)}; }

LUISA_DEVICE_CALLABLE inline packed_float3 make_packed_float3(int3 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_float3 make_packed_float3(uint3 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_float3 make_packed_float3(float3 v) noexcept { return {v.x, v.y, v.z}; }
LUISA_DEVICE_CALLABLE inline packed_float3 make_packed_float3(packed_int3 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_float3 make_packed_float3(packed_uint3 v) noexcept { return {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)}; }

LUISA_DEVICE_CALLABLE inline packed_int3 make_packed_int3(int3 v) noexcept { return {v.x, v.y, v.z}; }
LUISA_DEVICE_CALLABLE inline packed_int3 make_packed_int3(uint3 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_int3 make_packed_int3(float3 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_int3 make_packed_int3(packed_uint3 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_int3 make_packed_int3(packed_float3 v) noexcept { return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)}; }

LUISA_DEVICE_CALLABLE inline packed_uint3 make_packed_uint3(uint3 v) noexcept { return {v.x, v.y, v.z}; }
LUISA_DEVICE_CALLABLE inline packed_uint3 make_packed_uint3(int3 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_uint3 make_packed_uint3(float3 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_uint3 make_packed_uint3(packed_int3 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z)}; }
LUISA_DEVICE_CALLABLE inline packed_uint3 make_packed_uint3(packed_float3 v) noexcept { return {static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z)}; }

LUISA_DEVICE_CALLABLE inline auto make_float3x3(float val = 1.0f) noexcept {
    return float3x3{val};
}

LUISA_DEVICE_CALLABLE inline auto make_float3x3(float3 c0, float3 c1, float3 c2) noexcept {
    return float3x3{c0, c1, c2};
}

LUISA_DEVICE_CALLABLE inline auto make_float3x3(
    float m00, float m01, float m02,
    float m10, float m11, float m12,
    float m20, float m21, float m22) noexcept {
    
    return float3x3{m00, m01, m02, m10, m11, m12, m20, m21, m22};
}

LUISA_DEVICE_CALLABLE inline auto make_float3x3(float4x4 m) noexcept {
    return make_float3x3(make_float3(m[0]), make_float3(m[1]), make_float3(m[2]));
}

LUISA_DEVICE_CALLABLE inline auto make_float4x4(float val = 1.0f) noexcept {
    return float4x4{val};
}

LUISA_DEVICE_CALLABLE inline auto make_float4x4(float4 c0, float4 c1, float4 c2, float4 c3) noexcept {
    return float4x4{c0, c1, c2, c3};
}

LUISA_DEVICE_CALLABLE inline auto make_float4x4(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33) noexcept {
    
    return float4x4{
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33};
}

LUISA_DEVICE_CALLABLE inline auto make_float4x4(float3x3 m) noexcept {
    return make_float4x4(
        make_float4(m[0], 0.0f),
        make_float4(m[1], 0.0f),
        make_float4(m[2], 0.0f),
        make_float4(0.0f, 0.0f, 0.0f, 1.0f));
}

LUISA_DEVICE_CALLABLE inline auto to_fixed_point(float v, float scale = 1024.0f) noexcept {
    return static_cast<int>(round(v * scale));
}

LUISA_DEVICE_CALLABLE inline auto from_fixed_point(int v, float scale = 1024.0f) noexcept {
    return static_cast<float>(v) / scale;
}

LUISA_DEVICE_CALLABLE inline auto make_u64(uint2 v) noexcept {
    return (static_cast<uint64_t>(v.x) << 32u) | static_cast<uint64_t>(v.y);
}

LUISA_DEVICE_CALLABLE inline auto make_uint2(uint64_t v) noexcept {
    return make_uint2(static_cast<uint>(v >> 32u), static_cast<uint>(v));
}

}

#ifndef LUISA_DEVICE_COMPATIBLE

#include <atomic>
#define LUISA_STD_ATOMIC_COMPATIBLE

namespace luisa::_impl {

using atomic_int = std::atomic_int;
using atomic_uint = std::atomic_uint;

using std::atomic_load_explicit;
using std::atomic_store_explicit;
using std::atomic_exchange_explicit;
using std::atomic_fetch_add_explicit;
using std::atomic_fetch_sub_explicit;
using std::atomic_fetch_or_explicit;
using std::atomic_fetch_and_explicit;
using std::atomic_fetch_xor_explicit;

using std::memory_order_relaxed;

}

#endif

#ifdef LUISA_STD_ATOMIC_COMPATIBLE

namespace luisa {

namespace _impl {

template<typename T>
struct BuiltinAtomicTypeImpl {};

template<>
struct BuiltinAtomicTypeImpl<int> { using Type = atomic_int; };

template<>
struct BuiltinAtomicTypeImpl<uint> { using Type = atomic_uint; };

}

template<typename T>
using Atomic = typename _impl::BuiltinAtomicTypeImpl<T>::Type;

}

#define luisa_atomic_load(a)            luisa::_impl::atomic_load_explicit(&a, luisa::_impl::memory_order_relaxed)
#define luisa_atomic_store(a, val)      luisa::_impl::atomic_store_explicit(&a, val, luisa::_impl::memory_order_relaxed)
#define luisa_atomic_exchange(a, val)   luisa::_impl::atomic_exchange_explicit(&a, val, luisa::_impl::memory_order_relaxed)
#define luisa_atomic_fetch_add(a, val)  luisa::_impl::atomic_fetch_add_explicit(&a, val, luisa::_impl::memory_order_relaxed)
#define luisa_atomic_fetch_sub(a, val)  luisa::_impl::atomic_fetch_sub_explicit(&a, val, luisa::_impl::memory_order_relaxed)
#define luisa_atomic_fetch_and(a, val)  luisa::_impl::atomic_fetch_and_explicit(&a, val, luisa::_impl::memory_order_relaxed)
#define luisa_atomic_fetch_xor(a, val)  luisa::_impl::atomic_fetch_xor_explicit(&a, val, luisa::_impl::memory_order_relaxed)
#define luisa_atomic_fetch_or(a, val)   luisa::_impl::atomic_fetch_or_explicit(&a, val, luisa::_impl::memory_order_relaxed)

#endif

namespace luisa {

struct AtomicCounter {
    Atomic<uint> hi;
    Atomic<uint> lo;
};

#define luisa_atomic_counter_increase(ac)                        \
    do {                                                         \
        if (luisa_atomic_fetch_add(ac.lo, 1u) == 0xffffffffu) {  \
            luisa_atomic_fetch_add(ac.hi, 1u);                   \
        }                                                        \
    } while (false)

}