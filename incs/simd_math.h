#ifndef SIMD_MATH_H
# define SIMD_MATH_H

# include <stdbool.h>
# include "structs.h"

# if defined(__ARM_NEON) || defined(__aarch64__)
    # include <arm_neon.h>
    # define SIMD_AVAILABLE 1
    # define SIMD_NEON 1
    typedef float32x4_t simd_vec4;
    typedef uint32x4_t simd_mask4;
# elif defined(__SSE4_1__) || defined(__AVX__)
    # include <immintrin.h>
    # define SIMD_AVAILABLE 1
    # define SIMD_SSE 1
    typedef __m128 simd_vec4;
    typedef __m128 simd_mask4;
# else
    # define SIMD_AVAILABLE 0
    # define SIMD_SCALAR 1
    typedef struct { float x, y, z, w; } simd_vec4;
    typedef struct { float x, y, z, w; } simd_mask4;
# endif

// Load/Store operations
static inline simd_vec4 simd_load(const float *ptr);
static inline simd_vec4 simd_set(float x, float y, float z, float w);
static inline simd_vec4 simd_set1(float value);
static inline void simd_store(float *ptr, simd_vec4 v);

// Arithmetic operations
static inline simd_vec4 simd_add(simd_vec4 a, simd_vec4 b);
static inline simd_vec4 simd_sub(simd_vec4 a, simd_vec4 b);
static inline simd_vec4 simd_mul(simd_vec4 a, simd_vec4 b);
static inline simd_vec4 simd_div(simd_vec4 a, simd_vec4 b);

// Math operations
static inline simd_vec4 simd_min(simd_vec4 a, simd_vec4 b);
static inline simd_vec4 simd_max(simd_vec4 a, simd_vec4 b);
static inline simd_vec4 simd_sqrt(simd_vec4 v);

// Comparison operations
static inline simd_mask4 simd_cmplt(simd_vec4 a, simd_vec4 b);
static inline simd_mask4 simd_cmpgt(simd_vec4 a, simd_vec4 b);
static inline bool simd_any(simd_mask4 mask);
static inline bool simd_all(simd_mask4 mask);

// Utility operations
static inline float simd_hmin(simd_vec4 v); // Horizontal min
static inline float simd_hmax(simd_vec4 v); // Horizontal max
static inline float simd_dot3(simd_vec4 a, simd_vec4 b); // 3D dot product

/* ========================================================================= */
/* ARM NEON Implementation                                                   */
/* ========================================================================= */

#ifdef SIMD_NEON

static inline simd_vec4 simd_load(const float *ptr) {
    return vld1q_f32(ptr);
}

static inline simd_vec4 simd_set(float x, float y, float z, float w) {
    float data[4] = {x, y, z, w};
    return vld1q_f32(data);
}

static inline simd_vec4 simd_set1(float value) {
    return vdupq_n_f32(value);
}

static inline void simd_store(float *ptr, simd_vec4 v) {
    vst1q_f32(ptr, v);
}

static inline simd_vec4 simd_add(simd_vec4 a, simd_vec4 b) {
    return vaddq_f32(a, b);
}

static inline simd_vec4 simd_sub(simd_vec4 a, simd_vec4 b) {
    return vsubq_f32(a, b);
}

static inline simd_vec4 simd_mul(simd_vec4 a, simd_vec4 b) {
    return vmulq_f32(a, b);
}

static inline simd_vec4 simd_div(simd_vec4 a, simd_vec4 b) {
    return vdivq_f32(a, b);
}

static inline simd_vec4 simd_min(simd_vec4 a, simd_vec4 b) {
    return vminq_f32(a, b);
}

static inline simd_vec4 simd_max(simd_vec4 a, simd_vec4 b) {
    return vmaxq_f32(a, b);
}

static inline simd_vec4 simd_sqrt(simd_vec4 v) {
    return vsqrtq_f32(v);
}

static inline simd_mask4 simd_cmplt(simd_vec4 a, simd_vec4 b) {
    return vcltq_f32(a, b);
}

static inline simd_mask4 simd_cmpgt(simd_vec4 a, simd_vec4 b) {
    return vcgtq_f32(a, b);
}

static inline bool simd_any(simd_mask4 mask) {
    return vmaxvq_u32(mask) != 0;
}

static inline bool simd_all(simd_mask4 mask) {
    return vminvq_u32(mask) != 0;
}

static inline float simd_hmin(simd_vec4 v) {
    return vminvq_f32(v);
}

static inline float simd_hmax(simd_vec4 v) {
    return vmaxvq_f32(v);
}

static inline float simd_dot3(simd_vec4 a, simd_vec4 b) {
    simd_vec4 mul = vmulq_f32(a, b);
    // Zero out w component
    float32x4_t mask_data = {1.0f, 1.0f, 1.0f, 0.0f};
    mul = vmulq_f32(mul, mask_data);
    return vaddvq_f32(mul);
}

#endif // SIMD_NEON

/* ========================================================================= */
/* x86 SSE Implementation                                                    */
/* ========================================================================= */

#ifdef SIMD_SSE

static inline simd_vec4 simd_load(const float *ptr) {
    return _mm_loadu_ps(ptr);
}

static inline simd_vec4 simd_set(float x, float y, float z, float w) {
    return _mm_set_ps(w, z, y, x);
}

static inline simd_vec4 simd_set1(float value) {
    return _mm_set1_ps(value);
}

static inline void simd_store(float *ptr, simd_vec4 v) {
    _mm_storeu_ps(ptr, v);
}

static inline simd_vec4 simd_add(simd_vec4 a, simd_vec4 b) {
    return _mm_add_ps(a, b);
}

static inline simd_vec4 simd_sub(simd_vec4 a, simd_vec4 b) {
    return _mm_sub_ps(a, b);
}

static inline simd_vec4 simd_mul(simd_vec4 a, simd_vec4 b) {
    return _mm_mul_ps(a, b);
}

static inline simd_vec4 simd_div(simd_vec4 a, simd_vec4 b) {
    return _mm_div_ps(a, b);
}

static inline simd_vec4 simd_min(simd_vec4 a, simd_vec4 b) {
    return _mm_min_ps(a, b);
}

static inline simd_vec4 simd_max(simd_vec4 a, simd_vec4 b) {
    return _mm_max_ps(a, b);
}

static inline simd_vec4 simd_sqrt(simd_vec4 v) {
    return _mm_sqrt_ps(v);
}

static inline simd_mask4 simd_cmplt(simd_vec4 a, simd_vec4 b) {
    return _mm_cmplt_ps(a, b);
}

static inline simd_mask4 simd_cmpgt(simd_vec4 a, simd_vec4 b) {
    return _mm_cmpgt_ps(a, b);
}

static inline bool simd_any(simd_mask4 mask) {
    return _mm_movemask_ps(mask) != 0;
}

static inline bool simd_all(simd_mask4 mask) {
    return _mm_movemask_ps(mask) == 0xF;
}

static inline float simd_hmin(simd_vec4 v) {
    // Horizontal min: min(x,y,z,w)
    __m128 shuf = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 mins = _mm_min_ps(v, shuf);
    shuf = _mm_movehl_ps(shuf, mins);
    mins = _mm_min_ss(mins, shuf);
    return _mm_cvtss_f32(mins);
}

static inline float simd_hmax(simd_vec4 v) {
    __m128 shuf = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 3, 0, 1));
    __m128 maxs = _mm_max_ps(v, shuf);
    shuf = _mm_movehl_ps(shuf, maxs);
    maxs = _mm_max_ss(maxs, shuf);
    return _mm_cvtss_f32(maxs);
}

static inline float simd_dot3(simd_vec4 a, simd_vec4 b) {
    __m128 mul = _mm_mul_ps(a, b);
    // Zero out w component
    __m128 mask = _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f);
    mul = _mm_mul_ps(mul, mask);
    // Horizontal add
    __m128 shuf = _mm_movehdup_ps(mul);
    __m128 sums = _mm_add_ps(mul, shuf);
    shuf = _mm_movehl_ps(mul, sums);
    sums = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

#endif // SIMD_SSE

/* ========================================================================= */
/* Scalar Fallback Implementation                                            */
/* ========================================================================= */

#ifdef SIMD_SCALAR

static inline simd_vec4 simd_load(const float *ptr) {
    return (simd_vec4){ptr[0], ptr[1], ptr[2], ptr[3]};
}

static inline simd_vec4 simd_set(float x, float y, float z, float w) {
    return (simd_vec4){x, y, z, w};
}

static inline simd_vec4 simd_set1(float value) {
    return (simd_vec4){value, value, value, value};
}

static inline void simd_store(float *ptr, simd_vec4 v) {
    ptr[0] = v.x; ptr[1] = v.y; ptr[2] = v.z; ptr[3] = v.w;
}

static inline simd_vec4 simd_add(simd_vec4 a, simd_vec4 b) {
    return (simd_vec4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static inline simd_vec4 simd_sub(simd_vec4 a, simd_vec4 b) {
    return (simd_vec4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static inline simd_vec4 simd_mul(simd_vec4 a, simd_vec4 b) {
    return (simd_vec4){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

static inline simd_vec4 simd_div(simd_vec4 a, simd_vec4 b) {
    return (simd_vec4){a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

static inline simd_vec4 simd_min(simd_vec4 a, simd_vec4 b) {
    return (simd_vec4){fminf(a.x, b.x), fminf(a.y, b.y), 
                       fminf(a.z, b.z), fminf(a.w, b.w)};
}

static inline simd_vec4 simd_max(simd_vec4 a, simd_vec4 b) {
    return (simd_vec4){fmaxf(a.x, b.x), fmaxf(a.y, b.y),
                       fmaxf(a.z, b.z), fmaxf(a.w, b.w)};
}

static inline simd_vec4 simd_sqrt(simd_vec4 v) {
    return (simd_vec4){sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w)};
}

static inline simd_mask4 simd_cmplt(simd_vec4 a, simd_vec4 b) {
    return (simd_mask4){
        (a.x < b.x) ? 1.0f : 0.0f, (a.y < b.y) ? 1.0f : 0.0f,
        (a.z < b.z) ? 1.0f : 0.0f, (a.w < b.w) ? 1.0f : 0.0f
    };
}

static inline simd_mask4 simd_cmpgt(simd_vec4 a, simd_vec4 b) {
    return (simd_mask4){
        (a.x > b.x) ? 1.0f : 0.0f, (a.y > b.y) ? 1.0f : 0.0f,
        (a.z > b.z) ? 1.0f : 0.0f, (a.w > b.w) ? 1.0f : 0.0f
    };
}

static inline bool simd_any(simd_mask4 mask) {
    return (mask.x != 0.0f || mask.y != 0.0f || 
            mask.z != 0.0f || mask.w != 0.0f);
}

static inline bool simd_all(simd_mask4 mask) {
    return (mask.x != 0.0f && mask.y != 0.0f && 
            mask.z != 0.0f && mask.w != 0.0f);
}

static inline float simd_hmin(simd_vec4 v) {
    float min = v.x;
    if (v.y < min) min = v.y;
    if (v.z < min) min = v.z;
    if (v.w < min) min = v.w;
    return min;
}

static inline float simd_hmax(simd_vec4 v) {
    float max = v.x;
    if (v.y > max) max = v.y;
    if (v.z > max) max = v.z;
    if (v.w > max) max = v.w;
    return max;
}

static inline float simd_dot3(simd_vec4 a, simd_vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

#endif // SIMD_SCALAR

/* ========================================================================= */
/* High-Level Vector Operations                                              */
/* ========================================================================= */

// Convert between t_vector4 and simd_vec4
static inline simd_vec4 vec4_to_simd(t_vector4 v) {
    return simd_set(v.x, v.y, v.z, v.w);
}

static inline t_vector4 simd_to_vec4(simd_vec4 v) {
    t_vector4 result;
    float temp[4];
    simd_store(temp, v);
    result.x = temp[0];
    result.y = temp[1];
    result.z = temp[2];
    result.w = temp[3];
    return result;
}

// SIMD-accelerated vector operations
static inline t_vector4 vector_add_simd(t_vector4 a, t_vector4 b) {
    simd_vec4 va = vec4_to_simd(a);
    simd_vec4 vb = vec4_to_simd(b);
    return simd_to_vec4(simd_add(va, vb));
}

static inline t_vector4 vector_sub_simd(t_vector4 a, t_vector4 b) {
    simd_vec4 va = vec4_to_simd(a);
    simd_vec4 vb = vec4_to_simd(b);
    return simd_to_vec4(simd_sub(va, vb));
}

static inline t_vector4 vector_mul_simd(t_vector4 a, float scalar) {
    simd_vec4 va = vec4_to_simd(a);
    simd_vec4 vs = simd_set1(scalar);
    return simd_to_vec4(simd_mul(va, vs));
}

static inline float vector_dot_simd(t_vector4 a, t_vector4 b) {
    simd_vec4 va = vec4_to_simd(a);
    simd_vec4 vb = vec4_to_simd(b);
    return simd_dot3(va, vb);
}

static inline float vector_magnitude_simd(t_vector4 a) {
    float dot = vector_dot_simd(a, a);
    return sqrtf(dot);
}

#endif
