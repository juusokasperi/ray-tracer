/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:59:48 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/19 18:50:01 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool		checkerboard_sphere(t_point point,
					t_vector center, float scale);
static bool		checkerboard_cylinder(t_cylinder cyl, float scale);
static bool		checkerboard_cone(t_cone cone, float scale);
static bool		checkerboard_plane(t_point point, float scale);

/*
	Applies checkerboard pattern to an object based on the type of the object.
*/

t_rgb	get_object_color(const t_object *obj, t_point hit_point)
{
    bool is_even;

    // If not checkered, just return the base color immediately
    if (!obj->is_checkered)
        return (obj->color);

    // Reuse your existing static logic (checkerboard_sphere, etc.)
    if (obj->type == SPHERE)
        is_even = checkerboard_sphere(hit_point, obj->data.sphere.center, obj->checkerboard.scale);
    else if (obj->type == PLANE)
        is_even = checkerboard_plane(hit_point, obj->checkerboard.scale);
    else if (obj->type == CYLINDER)
        is_even = checkerboard_cylinder(obj->data.cylinder, obj->checkerboard.scale);
    else
        is_even = checkerboard_cone(obj->data.cone, obj->checkerboard.scale);

    return (is_even ? obj->checkerboard.color_2 : obj->checkerboard.color_1);
}

/*
	Calculates checkerboard pattern for a plane.
*/
static bool	checkerboard_plane(t_point point, float scale)
{
	int	xyz[3];

	xyz[0] = (int)(floorf(scale * (point.x + SHADOW_EPSILON)));
	xyz[1] = (int)(floorf(scale * (point.y + SHADOW_EPSILON)));
	xyz[2] = (int)(floorf(scale * (point.z + SHADOW_EPSILON)));
	return ((xyz[0] + xyz[1] + xyz[2]) % 2 == 0);
}

/*
	Calculates ck pattern for a sphere using spherical coordinates.
	- Longitude; angle around the y-axis
	- Latitude; angle from the y-axis
*/
static bool	checkerboard_sphere(t_point point, t_vector center, float scale)
{
	int	longitude;
	int	latitude;

	point = vector_subtract(point, center);
	vector_normalize(&point);
	longitude = (int)(floorf(
				(0.5f + atan2f(point.z, point.x) / (2.0f * M_PI)) * scale));
	latitude = (int)(floorf(
				(0.5f - asinf(point.y) / M_PI) * scale));
	return ((longitude % 2) == (latitude % 2));
}

/*
	If hit on the cap, creates a radial grid pattern on the base.
	If on the cone surface, maps using angular position around the cone
		and height along the cone axis.
*/
static bool	checkerboard_cone(t_cone cone, float scale)
{
	float	theta;
	int		xy[2];
	t_point	point;

	point = cone.local_hit;
	if (fabsf(point.y + cone.height) < SHADOW_EPSILON)
	{
		xy[0] = (int)floorf((point.x / cone.radius) * (scale / 1.5f));
		xy[1] = (int)floorf((point.z / cone.radius) * (scale / 1.5f));
	}
	else
	{
		theta = atan2f(point.x, point.z);
		if (theta < 0)
			theta += 2 * M_PI;
		xy[0] = (int)floorf((theta / (2 * M_PI) * (scale * 4)));
		xy[1] = (int)floorf((point.y + cone.height) / cone.height * scale);
	}
	return ((xy[0] + xy[1]) % 2 == 0);
}

/*
	If hit on the caps; create a radial grid pattern on the ends.
	If hit on the cylindrical surface; map using the angular position
	around the cylinder and height along the axis.
*/
static bool	checkerboard_cylinder(t_cylinder cyl, float scale)
{
	float	height;
	int		xy[2];
	float	theta;
	t_point	point;

	point = cyl.local_hit;
	height = cyl.height / 2.0f;
	if (fabsf(point.y - height) < SHADOW_EPSILON
		|| fabsf(point.y + height) < SHADOW_EPSILON)
	{
		xy[0] = (int)floorf((point.x / cyl.radius) * (scale / 1.5f));
		xy[1] = (int)floorf((point.z / cyl.radius) * (scale / 1.5f));
	}
	else
	{
		theta = atan2f(point.x, point.z);
		if (theta < 0)
			theta += 2 * M_PI;
		xy[0] = (int)floorf((theta / (2 * M_PI)) * (scale * 4));
		xy[1] = (int)floorf(((point.y + height) / cyl.height) * scale);
	}
	return ((xy[0] + xy[1]) % 2 == 0);
}
