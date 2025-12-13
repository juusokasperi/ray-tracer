/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:00:00 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/13 19:46:09 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

bool	validate_object_count(t_data *data)
{
	if (!data)
		return (false);
	return (true);
}

void	setup_checkered_object(t_object *obj, t_line_context *ctx)
{
	t_checkerboard	*cb;

	if (!obj || !ctx)
		return ;
	obj->is_checkered = ctx->is_checkered;
	if (ctx->is_checkered)
	{
		cb = &obj->checkerboard;
		cb->scale = ctx->scale;
		cb->color_1 = obj->color;
		cb->color_2 = ctx->color_2;
	}
}

bool	parse_object_color(Arena *a, char *color_str, t_object *object,
		bool is_checkered, t_rgb *color_2)
{
	bool	result;

	if (!color_str || !object || !color_2)
		return (false);
	if (is_checkered)
		result = parse_dual_rgb(a, color_str, &object->color, color_2);
	else
		result = parse_rgb(a, color_str, &object->color);
	return (result);
}
