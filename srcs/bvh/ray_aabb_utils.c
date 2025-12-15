/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_aabb_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:11:01 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/22 16:58:34 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "simd_math.h"

#define MIN 0
#define MAX 1
#define MIN_OF_ALL 2
#define MAX_OF_ALL 3

/*
	@return Boolean stating whether the ray intersect the AABB.
*/
bool	ray_aabb_intersect(t_ray *ray, t_aabb_bounds *bounds,
	float *t_enter, float *t_exit)
{
#if SIMD_AVAILABLE
	simd_vec4	origin;
	simd_vec4	inv_dir;
	simd_vec4	b_minmax[2];
	simd_vec4	t[4];
	float		t_float[2];

	origin = simd_set(ray->origin.x, ray->origin.y, ray->origin.z, 0.0f);
	inv_dir = simd_set(ray->inv_dir.x, ray->inv_dir.y, ray->inv_dir.z, 1.0f);
	b_minmax[MIN] = simd_set(bounds->min.x, bounds->min.y, bounds->min.z, FLT_MAX);
	b_minmax[MAX] = simd_set(bounds->max.x, bounds->max.y, bounds->max.z, -FLT_MAX);
	t[MIN] = simd_mul(simd_sub(b_minmax[MIN], origin), inv_dir);
	t[MAX] = simd_mul(simd_sub(b_minmax[MAX], origin), inv_dir);
	t[MIN_OF_ALL] = simd_min(t[MIN], t[MAX]);
	t[MAX_OF_ALL] = simd_max(t[MIN], t[MAX]);
	t_float[MIN] = simd_hmax(t[MIN_OF_ALL]);
	t_float[MAX] = simd_hmin(t[MAX_OF_ALL]);
	*t_enter = t_float[MIN];
	*t_exit = t_float[MAX];

	return (t_float[MAX] >= t_float[MIN] && t_float[MAX] > 0.0f);
#else
	float	t0;
	float	t1;
	float	t_min;
	float	t_max;

	t0 = (bounds->min.x - ray->origin.x) * ray->inv_dir.x;
	t1 = (bounds->max.x - ray->origin.x) * ray->inv_dir.x;
	t_min = fminf(t0, t1);
	t_max = fmaxf(t0, t1);
	t0 = (bounds->min.y - ray->origin.y) * ray->inv_dir.y;
	t1 = (bounds->max.y - ray->origin.y) * ray->inv_dir.y;
	t_min = fmaxf(t_min, fminf(t0, t1));
	t_max = fminf(t_max, fmaxf(t0, t1));
	t0 = (bounds->min.z - ray->origin.z) * ray->inv_dir.z;
	t1 = (bounds->max.z - ray->origin.z) * ray->inv_dir.z;
	t_min = fmaxf(t_min, fminf(t0, t1));
	t_max = fminf(t_max, fmaxf(t0, t1));
	*t_enter = t_min;
	*t_exit = t_max;
	return (t_max >= t_min && t_max > 0.0f);
#endif
}

static inline t_vector4 aabb_center(t_aabb_bounds *bounds)
{
    t_vector4 center;
    center.x = (bounds->min.x + bounds->max.x) * 0.5f;
    center.y = (bounds->min.y + bounds->max.y) * 0.5f;
    center.z = (bounds->min.z + bounds->max.z) * 0.5f;
    center.w = 0.0f;
    return center;
}

void push_children_sorted(t_bvh *bvh, t_ray ray,
    uint32_t left_right, uint32_t *stack, int *stack_ptr)
{
    uint16_t left = get_left_child(left_right);
    uint16_t right = get_right_child(left_right);
    
    if (left == NO_CHILD && right == NO_CHILD)
        return;
    
    if (left == NO_CHILD) {
        stack[(*stack_ptr)++] = right;
        return;
    }
    if (right == NO_CHILD) {
        stack[(*stack_ptr)++] = left;
        return;
    }
    
    t_vector4 left_center = aabb_center(&bvh->nodes[left].bounds);
    t_vector4 right_center = aabb_center(&bvh->nodes[right].bounds);
    
    t_vector4 to_left = vector_subtract(left_center, ray.origin);
    t_vector4 to_right = vector_subtract(right_center, ray.origin);
    
#if SIMD_AVAILABLE
    float dist_left = vector_dot_simd(to_left, ray.direction);
    float dist_right = vector_dot_simd(to_right, ray.direction);
#else
    float dist_left = vector_dot(to_left, ray.direction);
    float dist_right = vector_dot(to_right, ray.direction);
#endif
    if (dist_left < dist_right) {
        stack[(*stack_ptr)++] = right;
        stack[(*stack_ptr)++] = left;
    } else {
        stack[(*stack_ptr)++] = left;
        stack[(*stack_ptr)++] = right;
    }
}
