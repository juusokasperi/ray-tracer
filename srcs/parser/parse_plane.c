/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:10:36 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/02 20:23:32 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

bool	parse_plane(char *line, t_line_context *ctx)
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
	if (!validate_and_init_object(data, &geom, &mat, parts, PLANE)
		|| !parse_object_position(a, parts, &geom->data.plane.point)
		|| !parse_object_direction(a, parts, &geom->data.plane.normal)
		|| !parse_object_color(a, parts[2], mat, ctx->is_checkered,
			&ctx->color_2))
		return (false);
	apply_object_modifiers(mat, ctx);
	geom->mat_idx = ctx->mat_idx;
	data->scene.plane_count++;
	return (true);
}
