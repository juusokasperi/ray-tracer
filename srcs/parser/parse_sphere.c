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
	t_object_geom	*geom;
	t_object_mat	*mat;
	t_data			*data;
	float			diameter;
	Arena			*a;
	const int		unique_idx = 
		ctx->data->scene.object_count + ctx->data->scene.plane_count;

	data = ctx->data;
	a = &data->arena;
	mat = &data->scene.materials[unique_idx];
	if (!line || !data || !validate_object_count(data))
		return (false);
	parts = arena_split_isspace(a, line);
	if (!validate_and_init_object(data, &geom, &mat, parts, SPHERE)
		|| !parse_object_position(a, parts, &geom->data.sphere.center)
		|| !parse_positive_float(parts, 1, &diameter,
			"Sphere diameter must be positive")
		|| !parse_object_color(a, parts[2], mat, ctx->is_checkered,
			&ctx->color_2))
		return (false);
	geom->data.sphere.radius = diameter / 2;
	geom->mat_idx = unique_idx;
	apply_object_modifiers(mat, ctx);
	data->scene.object_count++;
	return (true);
}
