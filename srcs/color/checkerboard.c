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
#include "structs.h"

static bool		checkerboard_sphere(t_point point,
					t_vector center, float scale);
static bool		checkerboard_cylinder(t_cylinder cyl, t_point world_hit, float scale);
static bool		checkerboard_cone(t_cone cone, t_point world_hit, float scale);
static bool		checkerboard_plane(t_point point, float scale);
static t_point	get_local_hit(t_point world_hit, t_cylinder cyl);

/*
	Applies checkerboard pattern to an object based on the type of the object.
*/

t_rgb	get_object_color(t_object_geom *geom, t_object_mat *mat, t_point world_hit)
{
    bool is_even;

    if (geom->type == SPHERE)
        is_even = checkerboard_sphere(world_hit, geom->data.sphere.center, mat->ck_scale);
    else if (geom->type == PLANE)
        is_even = checkerboard_plane(world_hit, mat->ck_scale);
    else if (geom->type == CYLINDER)
        is_even = checkerboard_cylinder(geom->data.cylinder, world_hit, mat->ck_scale);
    else
        is_even = checkerboard_cone(geom->data.cone, world_hit, mat->ck_scale);

    return (is_even ? mat->color_2 : mat->color);
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
static bool	checkerboard_cone(t_cone cone, t_point world_hit, float scale)
{
	float	theta;
	int		xy[2];
	t_point	point;

	point = get_local_hit(world_hit, cone);
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
static bool	checkerboard_cylinder(t_cylinder cyl, t_point world_hit, float scale)
{
	float	height;
	int		xy[2];
	float	theta;
	t_point	point;

	point = get_local_hit(world_hit, cyl);
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

static t_point get_local_hit(t_point world_hit, t_cylinder cyl)
{
    t_point local_hit = vector_subtract(world_hit, cyl.center);
    t_vector rot_axis = vector_cross(vector(0, 1, 0), cyl.axis);
    float magnitude = vector_magnitude(rot_axis);
    if (magnitude > EPSILON)
    {
        vector_normalize(&rot_axis);
        float angle = acosf(vector_dot(vector(0, 1, 0), cyl.axis));
        local_hit = rotate_vector(local_hit, rot_axis, -angle);
    }
    return (local_hit);
}
