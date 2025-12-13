/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:48:12 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/17 00:47:06 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	set_rotation_and_angle(t_vector *rot_axis, float *angle, float y)
{
	if (y > 0)
	{
		*rot_axis = vector(0, 0, 0);
		*angle = 0;
	}
	else
	{
		*rot_axis = vector(1, 0, 0);
		*angle = M_PI;
	}
}

t_ray	transform_ray_cone(t_ray ray, t_cone cone)
{
	t_vector	rot_axis;
	t_vector	up;
	float		angle;
	float		magnitude_rot_axis;

	up = vector(0, 1, 0);
	rot_axis = vector_cross(up, cone.axis);
	magnitude_rot_axis = vector_magnitude(rot_axis);
	if (magnitude_rot_axis < EPSILON)
		set_rotation_and_angle(&rot_axis, &angle, cone.axis.y);
	else
	{
		vector_normalize(&rot_axis);
		angle = acosf(vector_dot(up, cone.axis));
	}
	ray.origin = vector_subtract(ray.origin, cone.center);
	if (magnitude_rot_axis > EPSILON || cone.axis.y < 0)
	{
		ray.origin = rotate_vector(ray.origin, rot_axis, -angle);
		ray.direction = rotate_vector(ray.direction, rot_axis, -angle);
	}
	ray.origin.y -= cone.height / 2;
	return (ray);
}
