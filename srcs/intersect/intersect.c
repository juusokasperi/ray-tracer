/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:20:16 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/16 13:37:35 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "simd_math.h"

static float	sphere_ray_intersect(t_ray ray, t_sphere sphere);
static float	plane_ray_intersect(t_ray ray, t_plane plane);

float	ray_intersect(t_ray ray, t_object_geom *object)
{
	if (object->type == SPHERE)
		return (sphere_ray_intersect(ray, object->data.sphere));
	if (object->type == PLANE)
		return (plane_ray_intersect(ray, object->data.plane));
	if (object->type == CONE)
		return (cone_ray_intersect(ray, &object->data.cone));
	if (object->type == CYLINDER)
		return (cylinder_ray_intersect(ray, &object->data.cylinder));
	else
		return (-1);
}

static float	plane_ray_intersect(t_ray ray, t_plane plane)
{
	float	numerator;
	float	denominator;
	float	t;

	denominator = vector_dot(ray.direction, plane.normal);
	if (fabsf(denominator) < PLANE_EPSILON)
		return (-1);
	numerator = vector_dot(
			vector_subtract(plane.point, ray.origin), plane.normal);
	t = numerator / denominator;
	if (t < 0)
		return (-1);
	return (t);
}

static float	sphere_ray_intersect(t_ray ray, t_sphere sphere)
{
#if SIMD_AVAILABLE
	simd_vec4	ray_orig;
	simd_vec4	ray_dir;
	simd_vec4	sphere_center;
	simd_vec4	s;
	float		b;
	float		c;
	float		h;

	ray_orig = vec4_to_simd(ray.origin);
	ray_dir = vec4_to_simd(ray.direction);
	sphere_center = vec4_to_simd(sphere.center);
    s = simd_sub(ray_orig, sphere_center);
    b = simd_dot3(s, ray_dir);
    c = simd_dot3(s, s) - sphere.radius * sphere.radius;
    h = b * b - c;
    if (h < 0.0f)
        return (-1.0f);
    h = sqrtf(h);
    float t = -b - h;
    if (t <= EPSILON)
        t = -b + h;
    if (t <= EPSILON)
        return (-1.0f);
    return (t);
#else
	t_vector4	s;
	float		b;
	float		c;
	float		h;
	float		t;

	s = vector_subtract(ray.origin, sphere.center);
	b = vector_dot(s, ray.direction);
	c = vector_dot(s, s) - sphere.radius * sphere.radius;
	h = b * b - c;
	if (h < 0)
		return (-1);
	h = sqrtf(h);
	t = -b - h;
	if (t <= EPSILON)
		t = -b + h;
	if (t <= EPSILON)
		return (-1);
	return (t);
#endif
}
