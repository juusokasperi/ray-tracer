/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:31:51 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/18 10:55:06 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/*
	Calculates the size of each axis in the bounding box

	@return Integer (0,1,2) representing the largest axis.
*/
int	get_largest_axis(t_aabb_bounds *bounds)
{
	float	x_extent;
	float	y_extent;
	float	z_extent;

	x_extent = bounds->max.x - bounds->min.x;
	y_extent = bounds->max.y - bounds->min.y;
	z_extent = bounds->max.z - bounds->min.z;
	if (x_extent >= y_extent && x_extent >= z_extent)
		return (X);
	else if (y_extent >= x_extent && y_extent >= z_extent)
		return (Y);
	else
		return (Z);
}

/*
	Calculates center point for each axis in AABB
*/
static t_vector	get_object_centroid(t_object_geom *obj)
{
	t_aabb_bounds	bounds;
	t_vector		centroid;

	bounds = calculate_object_aabb(obj);
	centroid.x = (bounds.min.x + bounds.max.x) * 0.5f;
	centroid.y = (bounds.min.y + bounds.max.y) * 0.5f;
	centroid.z = (bounds.min.z + bounds.max.z) * 0.5f;
	return (centroid);
}

/*
	Returns the center point of specified axis
*/
float	get_centroid_info(t_object_geom *obj, int axis)
{
	t_vector	centroid;

	centroid = get_object_centroid(obj);
	if (axis == X)
		return (centroid.x);
	else if (axis == Y)
		return (centroid.y);
	else
		return (centroid.z);
}

void	swap_objects(t_object_geom *obj_1, t_object_geom *obj_2)
{
	t_object_geom	temp;

	temp = *obj_1;
	*obj_1 = *obj_2;
	*obj_2 = temp;
}

t_aabb_bounds	calculate_bounds(t_object_geom *objects, int start, int end)
{
	t_aabb_bounds	obj_bounds;
	t_aabb_bounds	bounds;

	bounds = calculate_object_aabb(&objects[start]);
	while (++start <= end)
	{
		obj_bounds = calculate_object_aabb(&objects[start]);
		bounds = aabb_union(bounds, obj_bounds);
	}
	return (bounds);
}
