/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:26:12 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/16 13:34:59 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	parse_cone_attrs(Arena *a, char **parts, t_object *cone,
				t_line_context *ctx)
{
	if (!parse_object_position(a, parts, &cone->data.cylinder.center)
		|| !parse_object_direction(a, parts, &cone->data.cylinder.axis)
		|| !parse_positive_float(parts, 2, &cone->data.cylinder.radius,
			"Cone radius must be positive")
		|| !parse_positive_float(parts, 3, &cone->data.cylinder.height,
			"Cone height must be positive")
		|| !parse_object_color(a, parts[4], cone, ctx->is_checkered,
			&ctx->color_2))
		return (false);
	cone->data.cone.radius /= 2.0;
	apply_object_modifiers(cone, ctx);
	return (true);
}

bool	parse_cone(char *line, t_line_context *ctx)
{
	char			**parts;
	t_object		*cone;
	t_data			*data;
	Arena			*a;

	data = ctx->data;
	a = &data->arena;
	if (!line || !data || !validate_object_count(data))
		return (false);
	parts = arena_split_isspace(a, line);
	if (!validate_and_init_object(data, &cone, parts, CONE)
		|| !parse_cone_attrs(a, parts, cone, ctx))
		return (false);
	data->scene.object_count++;
	return (true);
}
