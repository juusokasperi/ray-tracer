/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:10:09 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/02 20:22:07 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	parse_cylinder_attrs(Arena *a, char **parts, t_object_geom *geom,
				t_object_mat *mat, t_line_context *ctx)
{
	if (!parse_object_position(a, parts, &geom->data.cylinder.center)
		|| !parse_object_direction(a, parts, &geom->data.cylinder.axis)
		|| !parse_positive_float(parts, 2, &geom->data.cylinder.radius,
			"Cylinder radius must be positive")
		|| !parse_positive_float(parts, 3, &geom->data.cylinder.height,
			"Cylinder height must be positive")
		|| !parse_object_color(a, parts[4], mat, ctx->is_checkered,
			&ctx->color_2))
		return (false);
	geom->data.cylinder.radius /= 2.0;
	apply_object_modifiers(mat, ctx);
	return (true);
}

bool	parse_cylinder(char *line, t_line_context *ctx)
{
	char			**parts;
	t_object_geom	*geom;
	t_object_mat	*mat;
	t_data			*data;
	Arena			*a;

	data = ctx->data;
	a = &data->arena;
	mat = &data->scene.materials[ctx->mat_idx];
	if (!line || !data || !validate_object_count(data))
		return (false);
	parts = arena_split_isspace(a, line);
	if (!validate_and_init_object(data, &geom, &mat, parts, CYLINDER)
		|| !parse_cylinder_attrs(a, parts, geom, mat, ctx))
		return (false);
	data->scene.object_count++;
	geom->mat_idx = ctx->mat_idx;
	return (true);
}
