/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:38:22 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/10 14:37:26 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	check_file_extension(const char *filename, const char *extension)
{
	char	*dot;

	if (!filename || !extension)
		return (false);
	dot = strrchr(filename, '.');
	if (!dot || dot == filename)
		return (false);
	return (strcmp(dot, extension) == 0);
}

bool	validate_scene_file(const char *filename, int *fd)
{
	if (!check_file_extension(filename, ".rt"))
	{
		printf("Error\nInvalid file extension. Expected .rt\n");
		return (false);
	}
	*fd = open(filename, O_RDONLY);
	if (*fd < 0)
	{
		perror("Error opening file");
		return (false);
	}
	return (true);
}
