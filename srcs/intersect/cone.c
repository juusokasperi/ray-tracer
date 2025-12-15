/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:48:12 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/17 16:30:20 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "structs.h"

static float	disk_intersection(t_ray ray, t_cone cone);
static float	infinite_cone_intersection(t_ray ray, t_cone cone);

/*
	Instead of checking in the actual 'world space', we transform the ray
	so that the cone is pointing at 0,1,0 and the tip is at 0,0,0.

	Then we check for intersections in the curved surface of the cone and the
	base disk. The one that returns the smaller t > 0 value, is the closest
	intersection.
*/
float	cone_ray_intersect(t_ray ray, t_cone *cone)
{
	float		t[2];
	int			i;
	float		min;
	t_vector4	tip_pos;

	tip_pos = vector_add(cone->center, vector_multiply(cone->axis, cone->height * 0.5f));
	ray = transform_ray(ray, tip_pos, cone->axis);
	t[0] = infinite_cone_intersection(ray, *cone);
	t[1] = disk_intersection(ray, *cone);
	min = -1;
	i = -1;
	while (++i < 2)
		if ((t[i] > 0 && t[i] < min) || (min == -1 && t[i] > 0))
			min = t[i];
	return (min);
}

static float	disk_intersection(t_ray ray, t_cone cone)
{
	float	t;
	float	x;
	float	z;

	if (fabsf(ray.direction.y) < EPSILON)
		return (-1);
	t = (-cone.height - ray.origin.y) / ray.direction.y;
	if (t < EPSILON)
		return (-1);
	x = ray.origin.x + t * ray.direction.x;
	z = ray.origin.z + t * ray.direction.z;
	if (x * x + z * z <= (cone.radius * cone.radius) + EPSILON)
		return (t);
	return (-1);
}

static bool	solve_quadratic_cone(float t[2], t_ray ray, t_cone cone)
{
	float	a;
	float	b;
	float	d;
	float	k;

	t[0] = -1;
	t[1] = -1;
	k = (cone.radius / cone.height) * (cone.radius / cone.height);
	a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z
		- k * ray.direction.y * ray.direction.y;
	b = 2 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z
			- k * ray.origin.y * ray.direction.y);
	d = b * b - 4 * a * (ray.origin.x * ray.origin.x
			+ ray.origin.z * ray.origin.z - k * ray.origin.y * ray.origin.y);
	if (d < 0 || fabsf(a) < EPSILON)
		return (false);
	d = sqrtf(d);
	t[0] = (-b - d) / (2 * a);
	t[1] = (-b + d) / (2 * a);
	return (true);
}

static float	infinite_cone_intersection(t_ray ray, t_cone cone)
{
	float	t[2];
	float	y;
	int		i;

	if (solve_quadratic_cone(t, ray, cone) == false)
		return (-1);
	i = -1;
	while (++i < 2)
	{
		if (t[i] > EPSILON)
		{
			y = ray.origin.y + t[i] * ray.direction.y;
			if (y >= -cone.height - EPSILON
				&& y <= EPSILON)
				return (t[i]);
		}
	}
	return (-1);
}
