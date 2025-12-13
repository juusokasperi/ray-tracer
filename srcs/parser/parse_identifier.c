/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_identifier.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:15:00 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/12 18:14:12 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/*
    Splits the identifier string by ':' to separate type and modifiers.
	Example: "sp:ck:0.5:sh:100:gl:0.5" 
		-> ["sp", "ck", "0.5", "sh", "100", "gl", "0.5"]
*/
static bool	get_identifier_parts(Arena *a, char *token, char **type, char ***parts)
{
	*parts = arena_split(a, token, ":");
	if (!(*parts) || !(*parts)[0])
	{
		printf("Error\nMemory allocation or invalid identifier format\n");
		return (false);
	}
	*type = arena_strdup(a, (*parts)[0]);
	if (!(*type))
		return (false);
	return (true);
}

/*
    Helper to parse a float value for a specific modifier.
    Checks if the value exists and is within range.
*/
static bool	parse_modifier_value(char **args, int i, float *dest, char *name)
{
	if (!args[i + 1])
	{
		printf("Error\nMissing value for modifier '%s'\n", name);
		return (false);
	}
	if (!parse_float(args[i + 1], dest))
		return (false);
	return (true);
}

/*
  Iterates through the split parts and handles each modifier.
  - ck: checkerboard
  - sh: shininess
  - gl: glossiness
  - ri: refraction index
*/
static bool	process_modifiers(char **args, t_line_context *ctx)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (strcmp(args[i], "ck") == 0)
		{
			ctx->is_checkered = true;
			if (!parse_modifier_value(args, i, &ctx->scale, "ck")
				|| !validate_range_float(ctx->scale, 0.1f, 20.f, "Checkered scale"))
				return (false);
		}
		else if (strcmp(args[i], "sh") == 0)
		{
			if (!parse_modifier_value(args, i, &ctx->shininess, "sh")
				|| !validate_range_float(ctx->shininess, 0.f, 1000.f, "Shininess"))
				return (false);
		}
		else if (strcmp(args[i], "gl") == 0)
		{
			if (!parse_modifier_value(args, i, &ctx->glossiness, "gl")
				|| !validate_range_float(ctx->glossiness, 0.f, 1.f, "Glossiness"))
				return (false);
		}
		else if (strcmp(args[i], "ri") == 0)
		{
			if (!parse_modifier_value(args, i, &ctx->refraction_index, "ri")
				|| !validate_range_float(ctx->refraction_index, 1.f, 3.f, "Refraction index"))
				return (false);
		}
		else if (strcmp(args[i], "re") == 0)
		{
			if (!parse_modifier_value(args, i, &ctx->reflectivity, "re")
				|| !validate_range_float(ctx->reflectivity, 0.f, 1.f, "Reflectivity"))
				return (false);
		}
		else
		{
			printf("Error\nUnknown identifier modifier: %s\n", args[i]);
			return (false);
		}
		i += 2;
	}
	return (true);
}

bool	parse_identifier(Arena *a, char *line, char **identifier, t_line_context *ctx)
{
	char	**parts;
	char	**modifier_parts;

	if (!line || !identifier || !ctx)
		return (false);
	ctx->is_checkered = false;
	ctx->shininess = -1.0f;
	ctx->glossiness = -1.0f;
	ctx->refraction_index = -1.0f;
	ctx->reflectivity = -1.0f;
	ctx->idx = 0;
	parts = arena_split_isspace(a, line);
	if (!parts || !parts[0])
		return (false);
	ctx->idx = strlen(parts[0]);
	if (!get_identifier_parts(a, parts[0], identifier, &modifier_parts))
		return (false);
	if (!process_modifiers(modifier_parts, ctx))
		return (false);
	return (true);
}
