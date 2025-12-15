/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_aabb.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:48:36 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/19 17:55:37 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "vector_math.h"

static t_aabb_bounds	calculate_aabb_sphere(t_sphere *sphere)
{
	t_aabb_bounds	bounds;

	bounds.min.x = sphere->center.x - sphere->radius;
	bounds.min.y = sphere->center.y - sphere->radius;
	bounds.min.z = sphere->center.z - sphere->radius;
	bounds.max.x = sphere->center.x + sphere->radius;
	bounds.max.y = sphere->center.y + sphere->radius;
	bounds.max.z = sphere->center.z + sphere->radius;
	return (bounds);
}

/*
-- Unused, we don't add planes to BVH --
static t_aabb_bounds	calculate_aabb_plane(const t_plane *plane)
{
	float			limit;
	float			thickness;
	t_aabb_bounds	bounds;

	limit = 1000.0f;
	thickness = 0.1f;
	bounds.min = vector3(-limit, -limit, -limit);
	bounds.max = vector3(limit, limit, limit);
	if (fabsf(plane->normal.x) > 0.5f)
	{
		bounds.min.x = plane->point.x - thickness;
		bounds.max.x = plane->point.x + thickness;
	}
	if (fabsf(plane->normal.y) > 0.5f)
	{
		bounds.min.y = plane->point.y - thickness;
		bounds.max.y = plane->point.y + thickness;
	}
	if (fabsf(plane->normal.z) > 0.5f)
	{
		bounds.min.z = plane->point.z - thickness;
		bounds.max.z = plane->point.z + thickness;
	}
	return (bounds);
}
*/

static t_aabb_bounds	calculate_aabb_cylinder(const t_cylinder *cyl)
{
	t_vector4		p_1;
	t_vector4		p_2;
	t_aabb_bounds	bounds;

	p_1 = vector(
			cyl->center.x + cyl->axis.x * cyl->height * 0.5f,
			cyl->center.y + cyl->axis.y * cyl->height * 0.5f,
			cyl->center.z + cyl->axis.z * cyl->height * 0.5f);
	p_2 = vector(
			cyl->center.x - cyl->axis.x * cyl->height * 0.5f,
			cyl->center.y - cyl->axis.y * cyl->height * 0.5f,
			cyl->center.z - cyl->axis.z * cyl->height * 0.5f);
	bounds.min = vector3(fminf(p_1.x, p_2.x), fminf(p_1.y, p_2.y),
			fminf(p_1.z, p_2.z));
	bounds.max = vector3(fmaxf(p_1.x, p_2.x), fmaxf(p_1.y, p_2.y),
			fmaxf(p_1.z, p_2.z));
	bounds.min.x -= cyl->radius;
	bounds.min.y -= cyl->radius;
	bounds.min.z -= cyl->radius;
	bounds.max.x += cyl->radius;
	bounds.max.y += cyl->radius;
	bounds.max.z += cyl->radius;
	return (bounds);
}

static t_aabb_bounds	calculate_aabb_cone(const t_cone *cone)
{
	t_vector4		base;
	t_vector4		tip;
	t_aabb_bounds	bounds;

	base = vector(
			cone->center.x - cone->axis.x * cone->height * 0.5f,
			cone->center.y - cone->axis.y * cone->height * 0.5f,
			cone->center.z - cone->axis.z * cone->height * 0.5f);
	tip = vector(
			cone->center.x + cone->axis.x * cone->height * 0.5f,
			cone->center.y + cone->axis.y * cone->height * 0.5f,
			cone->center.z + cone->axis.z * cone->height * 0.5f);
	bounds.min = vector3(fminf(base.x, tip.x), fminf(base.y, tip.y),
			fminf(base.z, tip.z));
	bounds.max = vector3(fmaxf(base.x, tip.x), fmaxf(base.y, tip.y),
			fmaxf(base.z, tip.z));
	bounds.min.x -= cone->radius;
	bounds.min.y -= cone->radius;
	bounds.min.z -= cone->radius;
	bounds.max.x += cone->radius;
	bounds.max.y += cone->radius;
	bounds.max.z += cone->radius;
	return (bounds);
}

/*
	Creates an axis-aligned bounding box based on the type of the object.

	@return AABB box
*/
t_aabb_bounds	calculate_object_aabb(t_object_geom *obj)
{
	if (obj->type == SPHERE)
		return (calculate_aabb_sphere(&obj->data.sphere));
	else if (obj->type == CONE)
		return (calculate_aabb_cone(&obj->data.cone));
	else
		return (calculate_aabb_cylinder(&obj->data.cylinder));
}
// else if (obj->type == PLANE)
//		return (calculate_aabb_plane(&obj->data.plane));
