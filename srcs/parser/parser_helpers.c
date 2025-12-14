/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:19:39 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/22 16:22:55 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stddef.h>

bool	validate_scene_content(t_data *data)
{
	if (data->camera_count > 1)
	{
		printf("Error\nScene must contain exactly one camera (C)\n");
		return (false);
	}
	if (data->ambient_count > 1)
	{
		printf("Error\nScene must contain max one ambient light (A)\n");
		return (false);
	}
	return (true);
}

bool	allocate_scene_memory(t_data *data)
{
	size_t	align;
	size_t	material_count;

	align = 16;
	material_count = data->scene.object_count + data->scene.plane_count;
	if (data->scene.object_count > 0)
		data->scene.geometry = arena_alloc_aligned(&data->arena,
			sizeof(t_object_geom) * data->scene.object_count, align);
	if (data->scene.plane_count > 0)
	{
		data->scene.plane_geometry = arena_alloc_aligned(&data->arena,
			sizeof(t_object_geom) * data->scene.plane_count, align);
	}
	if (data->scene.light_count > 0)
		data->scene.lights = (t_light *)arena_alloc_aligned(&data->arena,
			sizeof(t_light) * data->scene.light_count, align);
	if (material_count > 0)
		data->scene.materials = arena_alloc_aligned(&data->arena,
			sizeof(t_object_mat) * material_count, align);
	data->scene.object_count = 0;
	data->scene.plane_count = 0;
	data->scene.light_count = 0;
	return (true);
}

bool	trim_line(Arena *a, char **line)
{
	char	*new;

	new = arena_strtrim(a, *line, " \t\n\r");
	if (!new)
	{
		*line = NULL;
		return (false);
	}
	*line = new;
	return (true);
}
