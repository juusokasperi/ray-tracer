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
#include "structs.h"

static bool	parse_cone_attrs(Arena *a, char **parts, t_object_geom *geom,
				t_object_mat *mat, t_line_context *ctx)
{
	if (!parse_object_position(a, parts, &geom->data.cylinder.center)
		|| !parse_object_direction(a, parts, &geom->data.cylinder.axis)
		|| !parse_positive_float(parts, 2, &geom->data.cylinder.radius,
			"Cone radius must be positive")
		|| !parse_positive_float(parts, 3, &geom->data.cylinder.height,
			"Cone height must be positive")
		|| !parse_object_color(a, parts[4], mat, ctx->is_checkered,
			&ctx->color_2))
		return (false);
	geom->data.cone.radius /= 2.0;
	geom->data.cone.inv_height = 1.0f / geom->data.cone.height;
	apply_object_modifiers(mat, ctx);
	return (true);
}

bool	parse_cone(char *line, t_line_context *ctx)
{
	char			**parts;
	t_object_geom	*geom;
	t_object_mat	*mat;
	t_data			*data;
	Arena			*a;
	const int		unique_idx = 
		ctx->data->scene.object_count + ctx->data->scene.plane_count;

	data = ctx->data;
	a = &data->arena;
	mat = &data->scene.materials[unique_idx];
	if (!line || !data || !validate_object_count(data))
		return (false);
	parts = arena_split_isspace(a, line);
	if (!validate_and_init_object(data, &geom, &mat, parts, CONE)
		|| !parse_cone_attrs(a, parts, geom, mat, ctx))
		return (false);
	data->scene.object_count++;
	geom->mat_idx = unique_idx;
	return (true);
}
