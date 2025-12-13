/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:53:06 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/19 18:52:22 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static float	disk_intersection(t_ray ray, t_cylinder cylinder, int side);
static float	infinite_cylinder_intersection(t_ray ray, t_cylinder cyl);
static void		get_cylinder_t(float t[3], t_ray ray, t_cylinder cylinder);
static t_ray	transform_ray(t_ray ray, t_cylinder cylinder);

/*
	Instead of checking in the actual 'world space', we transform the ray
	so that the cylinder normal is pointing at 0,1,0 and the center is at 0,0,0.

	Then we check for intersections in the curved surface of the cone and the
	bottom and top disk. The one that returns the smaller t > 0 value
	is the closest intersection.
*/
float	cylinder_ray_intersect(t_ray ray, t_cylinder *cylinder)
{
	float	t[3];
	int		i;
	float	min;

	ray = transform_ray(ray, *cylinder);
	get_cylinder_t(t, ray, *cylinder);
	min = -1;
	i = -1;
	while (++i < 3)
		if ((t[i] > 0 && t[i] < min) || (min == -1 && t[i] > 0))
			min = t[i];
	if (min > 0)
		cylinder->local_hit = get_point(ray, min);
	return (min);
}

static void	get_cylinder_t(float t[3], t_ray ray, t_cylinder cylinder)
{
	float	y;

	t[0] = infinite_cylinder_intersection(ray, cylinder);
	if (t[0] > 0)
	{
		y = ray.origin.y + t[0] * ray.direction.y;
		if (y < -cylinder.height / 2 - EPSILON
			|| y > cylinder.height / 2 + EPSILON)
			t[0] = -1;
	}
	t[1] = disk_intersection(ray, cylinder, TOP_DISK);
	t[2] = disk_intersection(ray, cylinder, BOTTOM_DISK);
}

static float	disk_intersection(t_ray ray, t_cylinder cylinder, int side)
{
	float	t;
	float	x;
	float	z;

	if (fabsf(ray.direction.y) > EPSILON)
	{
		if (side == TOP_DISK)
			t = (cylinder.height / 2 - ray.origin.y) / ray.direction.y;
		else
			t = (-cylinder.height / 2 - ray.origin.y) / ray.direction.y;
		if (t > EPSILON)
		{
			x = ray.origin.x + t * ray.direction.x;
			z = ray.origin.z + t * ray.direction.z;
			if (x * x + z * z <= (cylinder.radius * cylinder.radius) + EPSILON)
				return (t);
		}
	}
	return (-1);
}

static t_ray	transform_ray(t_ray ray, t_cylinder cylinder)
{
	t_vector	rot_axis;
	t_vector	up;
	float		angle;
	float		magnitude_rot_axis;

	up = vector(0, 1, 0);
	rot_axis = vector_cross(up, cylinder.axis);
	magnitude_rot_axis = vector_magnitude(rot_axis);
	if (magnitude_rot_axis < EPSILON)
	{
		if (cylinder.axis.y > 0)
			rot_axis = vector(0, 0, 0);
		else
			rot_axis = vector(1, 0, 0);
	}
	else
		vector_normalize(&rot_axis);
	ray.origin = vector_subtract(ray.origin, cylinder.center);
	if (magnitude_rot_axis > EPSILON)
	{
		angle = acosf(vector_dot(up, cylinder.axis));
		ray.origin = rotate_vector(ray.origin, rot_axis, -angle);
		ray.direction = rotate_vector(ray.direction, rot_axis, -angle);
	}
	return (ray);
}

static float	infinite_cylinder_intersection(t_ray ray, t_cylinder cyl)
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	t;

	t = -1;
	a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
	b = 2 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
	c = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z
		- cyl.radius * cyl.radius;
	d = b * b - 4 * a * c;
	if (d >= 0 && a > EPSILON)
	{
		d = sqrtf(d);
		t = (-b - d) / (2 * a);
		if (t < 0)
			t = (-b + d) / (2 * a);
		if (t < 0)
			t = -1;
	}
	return (t);
}
