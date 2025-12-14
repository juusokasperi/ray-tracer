/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_check_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:12:54 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/20 00:00:20 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	is_point_inside(t_point point, t_object_geom *obj);

/*
	If f.ex. camera is inside a sphere and the light source is outside it,
	returns 'false' and no further checking for this light source is needed.
*/
bool	light_visible(t_vector cam_pos, t_vector light_pos, t_object_geom *obj)
{
	return (is_point_inside(cam_pos, obj) == is_point_inside(light_pos, obj));
}

/*
	1. Calculates vector from cylinder center to hit point, projects it
	onto the cylinder axis - if this projection > cyl.height / 2, it is outside
	2. Finds closest point on the cylinder axis to hit point, calculates
	the distance from that point the hit point and compares it against
	the cylinder radius squared.
*/
static bool	is_point_inside_cylinder(t_point point, const t_cylinder *cyl)
{
	t_vector	center_to_point;
	float		project_to_axis;
	t_vector	closest_point;
	t_vector	perpendicular;
	float		distance_sqrd;

	center_to_point = vector_subtract(point, cyl->center);
	project_to_axis = vector_dot(center_to_point, cyl->axis);
	if (fabsf(project_to_axis) > cyl->height / 2)
		return (false);
	closest_point = vector_add(
			cyl->center, vector_multiply(cyl->axis, project_to_axis));
	perpendicular = vector_subtract(point, closest_point);
	distance_sqrd = vector_dot(perpendicular, perpendicular);
	return (distance_sqrd < (cyl->radius * cyl->radius) - SHADOW_EPSILON);
}

/*
	1. Calculates vector from cone center to hit point, projects it
	onto the cone axis - if this projection > cone.height / 2, it is outside
	2. Finds closest point on the cone axis to hit point, calculates
	the distance from that point the hit point and compares it against
	the cone radius. Cone radius calculation uses normalized position
	for the radius; 0.0 = at tip, 1.0 = at base (full radius).
*/
static bool	is_point_inside_cone(t_point point, const t_cone *cone)
{
	t_vector	center_to_point;
	float		project_to_axis;
	t_vector	closest_point;
	t_vector	perpendicular;
	float		distance_sqrd;
	float		normalized_height;

	center_to_point = vector_subtract(point, cone->center);
	project_to_axis = vector_dot(center_to_point, cone->axis);
	if (fabsf(project_to_axis) > cone->height / 2)
		return (false);
	closest_point = vector_add(
			cone->center, vector_multiply(cone->axis, project_to_axis));
	perpendicular = vector_subtract(point, closest_point);
	distance_sqrd = vector_dot(perpendicular, perpendicular);
	normalized_height = (project_to_axis + cone->height * 0.5f) / cone->inv_height;
	return (distance_sqrd < (normalized_height * normalized_height) - SHADOW_EPSILON);
}

/*
	Creates a vector from sphere center to the point,
	calculates the squared distance between the point and center
	and compares it to the squared radius.
*/
static bool	is_point_inside_sphere(t_point point, t_sphere *sphere)
{
	t_vector	center_to_point;
	float		dist_sqrd;

	center_to_point = vector_subtract(point, sphere->center);
	dist_sqrd = vector_dot(center_to_point, center_to_point);
	return (dist_sqrd < (sphere->radius * sphere->radius)
		- SHADOW_EPSILON);
}

static bool	is_point_inside(t_point point, t_object_geom *obj)
{
	if (obj->type == PLANE)
		return (false);
	else if (obj->type == SPHERE)
		return (is_point_inside_sphere(point, &obj->data.sphere));
	else if (obj->type == CONE)
		return (is_point_inside_cone(point, &obj->data.cone));
	else if (obj->type == CYLINDER)
		return (is_point_inside_cylinder(point, &obj->data.cylinder));
	else
		return (false);
}
