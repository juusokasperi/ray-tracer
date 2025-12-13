/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:53:52 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/19 17:47:48 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

bool	parse_sphere(char *line, t_line_context *ctx)
{
	char			**parts;
	t_object		*sphere;
	t_data			*data;
	float			diameter;
	Arena			*a;

	data = ctx->data;
	a = &data->arena;
	if (!line || !data || !validate_object_count(data))
		return (false);
	parts = arena_split_isspace(a, line);
	if (!validate_and_init_object(data, &sphere, parts, SPHERE)
		|| !parse_object_position(a, parts, &sphere->data.sphere.center)
		|| !parse_positive_float(parts, 1, &diameter,
			"Sphere diameter must be positive")
		|| !parse_object_color(a, parts[2], sphere, ctx->is_checkered,
			&ctx->color_2))
		return (false);
	sphere->data.sphere.radius = diameter / 2;
	apply_object_modifiers(sphere, ctx);
	data->scene.object_count++;
	return (true);
}
