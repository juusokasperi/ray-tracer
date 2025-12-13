/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dual_rgb.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:05:00 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/10 14:37:17 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	parse_first_color(Arena *a, char **colors, t_rgb *color_1)
{
	bool	result;

	result = parse_rgb(a, colors[0], color_1);
	if (!result)
		return (false);
	return (true);
}

static bool	parse_second_color(Arena *a, char **colors, t_rgb *color_2)
{
	bool	result;

	if (colors[1])
	{
		result = parse_rgb(a, colors[1], color_2);
		if (!result)
			return (false);
	}
	else
		*color_2 = rgb(225, 225, 225);
	return (true);
}

bool	parse_dual_rgb(Arena *a, char *str, t_rgb *color_1, t_rgb *color_2)
{
	char	**colors;

	if (!str || !color_1 || !color_2)
		return (false);
	colors = arena_split(a, str, "/");
	if (!colors || !colors[0])
	{
		printf("Error\nInvalid color format. Expected: R,G,B/R,G,B\n");
		return (false);
	}
	if (!parse_first_color(a, colors, color_1)
		|| !parse_second_color(a, colors, color_2))
		return (false);
	return (true);
}
