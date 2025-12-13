/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:06:49 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/02 20:22:00 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

bool	parse_ambient(char *line, t_data *data)
{
	char	**parts;
	Arena	*a;

	a = &data->arena;
	if (!line || !data)
		return (false);
	parts = arena_split_isspace(a, line);
	if (!validate_parts_count(parts, 2, "ambient light")
		|| !parse_ratio(parts, 0, &data->ambient.ratio, "Ambient light ratio")
		|| !parse_rgb(a, parts[1], &data->ambient.color))
		return (false);
	return (true);
}
