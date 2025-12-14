/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:42:45 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/22 16:22:52 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	process_identifier(char *identifier, t_line_context *ctx)
{
	bool	result;
	char	*current_line;

	current_line = ctx->line;
	if (strncmp(identifier, "A", 1) == 0)
		result = parse_ambient(&current_line[ctx->idx], ctx->data);
	else if (strncmp(identifier, "C", 1) == 0)
		result = parse_camera(&current_line[ctx->idx], ctx->data);
	else if (strncmp(identifier, "L", 1) == 0)
		result = parse_light(&current_line[ctx->idx], ctx->data, identifier);
	else if (strncmp(identifier, "sp", 2) == 0)
		result = parse_sphere(&current_line[ctx->idx], ctx);
	else if (strncmp(identifier, "pl", 2) == 0)
		result = parse_plane(&current_line[ctx->idx], ctx);
	else if (strncmp(identifier, "cy", 2) == 0)
		result = parse_cylinder(&current_line[ctx->idx], ctx);
	else if (strncmp(identifier, "co", 2) == 0)
		result = parse_cone(&current_line[ctx->idx], ctx);
	else
	{
		printf("Error\nUnknown identifier in line: %s\n", current_line);
		result = false;
	}
	return (result);
}

static bool	parse_line(char *line, t_data *data, int *mat_idx)
{
	Arena			*a;
	t_line_context	ctx;
	char			*identifier;
	bool			result;
	bool			is_object;

	a = &data->arena;
	if (!line || line[0] == '\0' || line[0] == '#')
		return (true);
	identifier = NULL;
	ctx.scale = 1.0f;
	ctx.color_2 = rgb(225, 225, 225);
	ctx.is_checkered = false;
	ctx.idx = 0;
	ctx.mat_idx = *mat_idx;
	ctx.line = line;
	ctx.data = data;
	if (!parse_identifier(a, line, &identifier, &ctx))
	{
		printf("Error\nInvalid identifier in line: %s\n", line);
		return (false);
	}
	is_object = (strcmp(identifier, "sp") == 0)
		|| (strcmp(identifier, "pl") == 0)
		|| (strcmp(identifier, "cy") == 0)
		|| (strcmp(identifier, "co") == 0);
	result = process_identifier(identifier, &ctx);
	if (result && is_object)
		(*mat_idx)++;
	return (result);
}

static bool	process_trimmed_line(char *trimmed, t_data *data)
{
	if (trimmed && trimmed[0] != '\0' && trimmed[0] != '#')
	{
		if (trimmed[0] == 'A')
			data->ambient_count++;
		else if (trimmed[0] == 'C')
			data->camera_count++;
		else if (trimmed[0] == 'L')
			data->scene.light_count++;
		else if (strncmp(trimmed, "pl", 2) == 0)
			data->scene.plane_count++;
		else if (strncmp(trimmed, "sp", 2) == 0 
			|| strncmp(trimmed, "cy", 2) == 0
			|| strncmp(trimmed, "co", 2) == 0)
			data->scene.object_count++;
	}
	return (true);
}

static void split_to_lines(Arena *a, char ***out_lines, char *buf)
{
	size_t	count;
	size_t	index;
	char	**lines;
	char	*line;
	char	*nl;

	count = 1;
	for (size_t i = 0; buf[i]; ++i)
		if (buf[i] == '\n')
			count++;
	lines = arena_alloc(a, sizeof(char *) * (count + 1));
	index = 0;
	line = buf;
	while (1)
	{
		lines[index++] = line;
		nl = strchr(line, '\n');
		if (!nl)
			break;
		*nl = '\0';
		line = nl + 1;
	}
	lines[index] = NULL;
	*out_lines = lines;
}

static bool	arena_read_file(Arena *a, int fd, char ***out_lines)
{
	struct stat	st;
	ssize_t		bytes_read;
	char		*buf;

	if (fstat(fd, &st) == -1)
	{
		close(fd);
		return (false);
	}
	buf = arena_alloc(a, st.st_size + 1);
	bytes_read = read(fd, buf, st.st_size);
	close(fd);
	if (bytes_read < 0)
		return (false);
	buf[st.st_size] = '\0';
	split_to_lines(a, out_lines, buf);
	return (true);
}

static bool	count_scene_elements(const char *filename, t_data *data)
{
	int			fd;
	char		**lines;
	char		*trimmed;

	if (!validate_scene_file(filename, &fd))
		return (false);
	data->scene.object_count = 0;
	data->scene.light_count = 0;
	data->camera_count = 0;
	data->ambient_count = 0;
	if (!arena_read_file(&data->arena, fd, &lines))
		return (false);
	while (*lines)
	{
		trimmed = arena_strtrim(&data->arena, *lines, " \t\n\r");
		process_trimmed_line(trimmed, data);
		lines++;
	}
	return (true);
}

bool	parse_scene(const char *filename, t_data *data)
{
	int		fd;
	char	**lines;
	bool	result;
	ArenaTemp	temp;
	int		mat_idx;

	temp = arena_temp_begin(&data->arena);
	if (!count_scene_elements(filename, data))
		return (false);
	if (!allocate_scene_memory(data))
		return (false);
	if (!validate_scene_file(filename, &fd))
		return (false);
	result = true;
	if (!arena_read_file(&data->arena, fd, &lines))
		return (false);
	mat_idx = 0;
	while (*lines && result)
	{
		if (!trim_line(&data->arena, lines))
			return (false);
		result = parse_line(*lines, data, &mat_idx);
		lines++;
	}
	if (!result)
		return (false);
	arena_temp_end(temp);
	return (validate_scene_content(data));
}
