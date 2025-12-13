/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:40:45 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/19 17:50:29 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	init_ray_inv(t_ray *ray)
{
	if (fabsf(ray->direction.x) < EPSILON)
		ray->inv_dir.x = 1.0f / EPSILON;
	else
		ray->inv_dir.x = 1.0f / ray->direction.x;
	if (fabsf(ray->direction.y) < EPSILON)
		ray->inv_dir.y = 1.0f / EPSILON;
	else
		ray->inv_dir.y = 1.0f / ray->direction.y;
	if (fabsf(ray->direction.z) < EPSILON)
		ray->inv_dir.z = 1.0f / EPSILON;
	else
		ray->inv_dir.z = 1.0f / ray->direction.z;
	ray->sign[0] = (ray->inv_dir.x < 0);
	ray->sign[1] = (ray->inv_dir.y < 0);
	ray->sign[2] = (ray->inv_dir.z < 0);
}

/*
	Builds a ray from an intersection point towards a light source.

	Sets the distance to the light source into *out_dist.
*/
t_ray	build_light_ray(t_point hit_point, t_light light,
			t_vector normal, float *out_dist)
{
	t_ray		res;
	t_vector	raw_dir;

	res.origin = vector_add(hit_point, vector_multiply(normal, SHADOW_EPSILON));
	raw_dir = vector_subtract(light.pos, res.origin);
	*out_dist = vector_magnitude(raw_dir);
	if (*out_dist > EPSILON)
		res.direction = vector_multiply(raw_dir, 1.0f / *out_dist);
	else
		res.direction = raw_dir;
	init_ray_inv(&res);
	return (res);
}

/*
	Transfer a pixel to world coordinates.

	Offset by +0.5f to shift the sampling point to center of pixel
	instead of top-left corner.

	Division by width and height to map from pixel space [0, width] to
	normalized coordinates [0, 1].

	2.0f * ... - 1.0f to map to NDC (Normalized Device Coordinates), which is
	is a coordinate system where x and y coordinates range from -1 to 1.
		F.ex. left edge of screen is -1, center 0, right edge +1.
		For Y, 1.0f - ... to flip Y-axis (on screen Y goes down, but in
		our 3D world Y goes up).

	Right = .. to know how far to move in X direction from the center of screen.
	Up = .. in Y direction.

	Direction = .. adds right and up to the cam.forward (which is initially at
		center of screen, moving the ray along.)
*/
t_ray	get_ray_for_px(t_data *data, float px, float py)
{
	t_ray		ray;
	t_vector	right;
	t_vector	up;
	float		pixels[2];
	t_camera	camera;

	camera = data->cam;
	pixels[X] = (2.0f * px / (float)data->width - 1.0f);
	pixels[Y] = 1.0f - 2.0f * py / (float)data->height;
	right = vector_multiply(camera.right, camera.vp.width * pixels[X]);
	up = vector_multiply(camera.up, camera.vp.height * pixels[Y]);
	ray.direction = vector_add(camera.forward, vector_add(right, up));
	vector_normalize(&ray.direction);
	ray.origin = camera.pos;
	init_ray_inv(&ray);
	return (ray);
}

/*
	Returns the hitpoint 't' along ray.
*/
t_point	get_point(t_ray ray, float t)
{
	return (vector_add(ray.origin, vector_multiply(ray.direction, t)));
}
