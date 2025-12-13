/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:00:00 by phuocngu          #+#    #+#             */
/*   Updated: 2025/12/13 16:51:08 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

bool	validate_object_count(t_data *data)
{
	if (!data)
		return (false);
	return (true);
}

static void	setup_checkered_object(t_object *obj, t_line_context *ctx)
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

void	apply_object_modifiers(t_object *obj, t_line_context *ctx)
{
	if (!obj || !ctx)
		return ;
	setup_checkered_object(obj, ctx);
	if (ctx->shininess > -0.5f)
		obj->shininess = ctx->shininess;
	if (ctx->glossiness > -0.5f)
		obj->glossiness = ctx->glossiness;
	if (ctx->refraction_index > -0.5f)
		obj->refraction_index = ctx->refraction_index;
	if (ctx->reflectivity > -0.5f)
		obj->reflectivity = ctx->reflectivity;
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

void	setup_object_optionals(t_object *obj, t_line_context *ctx)
{
	if (!obj || !ctx)
		return ;

}
