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

bool	parse_light(char *line, t_data *data)
{
	char	**parts;
	int		light_idx;
	Arena	*a;

	a = &data->arena;
	if (!line || !data || !validate_light_count(data))
		return (false);
	parts = arena_split_isspace(a, line);
	light_idx = data->scene.light_count;
	if (!validate_parts_count(parts, 3, "light")
		|| !parse_object_position(a, parts, &data->scene.lights[light_idx].pos)
		|| !parse_ratio(parts, 1, &data->scene.lights[light_idx].ratio,
			"Light ratio")
		|| !parse_rgb(a, parts[2], &data->scene.lights[light_idx].color))
		return (false);
	data->scene.light_count++;
	return (true);
}
