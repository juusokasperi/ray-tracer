/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:53:43 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/13 19:46:00 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	validate_light_count(t_data *data)
{
	if (!data)
		return (false);
	return (true);
}

bool	parse_light(char *line, t_data *data, char *identifier)
{
	char	**parts;
	int		light_idx;
	t_light	*light;
	Arena	*a;

	a = &data->arena;
	if (!line || !data || !validate_light_count(data))
		return (false);
	parts = arena_split_isspace(a, line);
	light_idx = data->scene.light_count;
	light = &data->scene.lights[light_idx];
	
	if (strlen(identifier) == 2 && strncmp(identifier, "LA", 2) == 0)
	{
		light->type = LIGHT_AREA;
		if (!validate_parts_count(parts, 4, "area light"))
			return (false);
		if (!parse_positive_float(parts, 3, &light->radius, "Light radius"))
			return (false);
		light->radius /= 2.0f;
	}
	else
	{
		light->type = LIGHT_POINT;
		light->radius = 0.0f;
		if (!validate_parts_count(parts, 3, "point light"))
			return (false);
	}
	if (!parse_object_position(a, parts, &light->pos)
		|| !parse_ratio(parts, 1, &light->ratio, "Light ratio")
		|| !parse_rgb(a, parts[2], &light->color))
		return (false);
	data->scene.light_count++;
	return (true);
}
