/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_normals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:46:31 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/19 19:02:36 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/*
	To_hit		is a vector pointing from the cylinder center to the hit_point.
	Projection	measures how far up/down the hit point is along the objs axis.

	If projection at top disk, normal = cyl.axis, if in bottom -cyl.axis;
	If on the cylindrical surface, we calculate the correct normal.
*/
static t_point	cylinder_normal(t_point hit_point, t_cylinder *cyl)
{
	t_vector	to_hit;
	float		projection;
	t_point		surface_normal;

	to_hit = vector_subtract(hit_point, cyl->center);
	projection = vector_dot(to_hit, cyl->axis);
	if (fabsf(projection - cyl->height / 2) < EPSILON)
		surface_normal = cyl->axis;
	else if (fabsf(projection + cyl->height / 2) < EPSILON)
		surface_normal = vector_multiply(cyl->axis, -1);
	else
	{
		surface_normal = vector_add(cyl->center,
				vector_multiply(cyl->axis, projection));
		surface_normal = vector_subtract(hit_point, surface_normal);
	}
	return (surface_normal);
}

/*
	If hit on the base, normal points to -cone.axis.
	Else;
	Axis_point	calculates the closest point along the axis to the hit_point.
	The normal then points from this point in the axis to the hit point.
*/
static t_point	cone_normal(t_point hit_point, t_cone *cone)
{
	t_vector	to_hit;
	float		proj;
	t_point		axis_point;

	to_hit = vector_subtract(hit_point, cone->center);
	proj = vector_dot(to_hit, cone->axis);
	if (fabsf(proj - cone->height) < EPSILON)
		return (vector_multiply(cone->axis, -1));
	axis_point = vector_add(cone->center, vector_multiply(cone->axis, proj));
	return (vector_subtract(hit_point, axis_point));
}

t_point	calculate_normal(t_vector view_direction,
		t_point hit_point, t_object *obj)
{
	t_point	surface_normal;

	if (obj->type == SPHERE)
		surface_normal = vector_subtract(
				hit_point, obj->data.sphere.center);
	else if (obj->type == CONE)
		surface_normal = cone_normal(hit_point, &obj->data.cone);
	else if (obj->type == CYLINDER)
		surface_normal = cylinder_normal(hit_point, &obj->data.cylinder);
	else
	{
		surface_normal = obj->data.plane.normal;
		if (vector_dot(view_direction, surface_normal) > 0)
			surface_normal = vector_multiply(surface_normal, -1);
	}
	vector_normalize(&surface_normal);
	return (surface_normal);
}
