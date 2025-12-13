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

static bool	handle_identifiers(Arena *a, char **parts,
	char ***identifier_parts, char **identifier, int *idx)
{
	*identifier_parts = arena_split(a, parts[0], ":");
	if (!*identifier_parts)
	{
		printf("Error\nMemory allocation failed\n");
		return (false);
	}
	*identifier = arena_strdup(a, (*identifier_parts)[0]);
	if (!*identifier)
	{
		printf("Error\nMemory allocation failed\n");
		return (false);
	}
	*idx = strlen((*identifier_parts)[0]);
	return (true);
}

static bool	process_checkered_params(char **identifier_parts,
	t_line_context *ctx)
{
	if (!identifier_parts[1])
		return (true);
	if (strcmp(identifier_parts[1], "ck") != 0)
	{
		printf("Error\nInvalid checkered pattern identifier: %s\n",
			identifier_parts[1]);
		return (false);
	}
	ctx->is_checkered = true;
	ctx->idx += 3;
	if (identifier_parts[2] && parse_float(identifier_parts[2], &ctx->scale))
	{
		if (!validate_range_float(ctx->scale, 0.1, 20.0, "Checkered scale"))
			return (false);
		ctx->idx += strlen(identifier_parts[2]) + 1;
	}
	return (true);
}

bool	parse_identifier(Arena *a, char *line, char **identifier, t_line_context *ctx)
{
	char	**parts;
	char	**identifier_parts;

	if (!line || !identifier || !ctx)
		return (false);
	ctx->is_checkered = false;
	ctx->idx = 0;
	parts = arena_split_isspace(a, line);
	if (!parts || !parts[0])
		return (false);
	if (!handle_identifiers(a, parts, &identifier_parts, identifier, &ctx->idx))
		return (false);
	if (!process_checkered_params(identifier_parts, ctx))
		return (false);
	return (true);
}
