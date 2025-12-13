/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:09:02 by phuocngu          #+#    #+#             */
/*   Updated: 2025/06/25 19:37:41 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	parse_camera_vectors(char **parts, t_data *data)
{
	bool	result;
	Arena	*a;

	a = &data->arena;
	result = parse_object_position(a, parts, &data->cam.pos);
	if (!result)
		return (false);
	result = parse_object_direction(a, parts, &data->cam.forward);
	if (!result)
		return (false);
	return (true);
}

bool	parse_fov(char **parts, int index, int *fov)
{
	float	temp_fov;
	bool	result;

	if (!parts || !parts[index] || !fov)
		return (false);
	result = parse_float(parts[index], &temp_fov);
	if (!result)
		return (false);
	if (!validate_range_float(temp_fov, 0.0, 180.0, "FOV"))
		return (false);
	if (temp_fov > 179.0f)
		*fov = 179;
	else
		*fov = (int)temp_fov;
	return (true);
}

static void	setup_camera_basis(t_data *data)
{
	t_vector	world_up;

	vector_normalize(&data->cam.forward);
	world_up.x = 0;
	world_up.y = 1;
	world_up.z = 0;
	if (fabsf(data->cam.forward.x) == 0 && fabsf(data->cam.forward.z) == 0)
	{
		world_up.x = 1;
		world_up.y = 0;
	}
	data->cam.right = vector_cross(data->cam.forward, world_up);
	vector_normalize(&data->cam.right);
	data->cam.up = vector_cross(data->cam.right, data->cam.forward);
}

bool	parse_camera(char *line, t_data *data)
{
	char	**parts;
	Arena	*a;

	a = &data->arena;
	if (!line || !data)
		return (false);
	parts = arena_split_isspace(a, line);
	if (!validate_parts_count(parts, 3, "camera")
		|| !parse_camera_vectors(parts, data)
		|| !parse_fov(parts, 2, &data->cam.vp.fov))
		return (false);
	setup_camera_basis(data);
	return (true);
}
