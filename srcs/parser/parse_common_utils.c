/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_common_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:30:00 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/16 13:33:23 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

bool	validate_parts_count(char **parts, int expected_count, char *obj_name)
{
	int	i;

	if (!parts || !obj_name)
		return (false);
	i = 0;
	while (parts[i])
		i++;
	if (i < expected_count)
	{
		printf("Error\nInvalid %s format, expected %d parts\n",
			obj_name, expected_count);
		return (false);
	}
	return (true);
}

bool	validate_and_init_object(t_data *data, t_object **obj,
		char **parts, t_shape type)
{
	int		expected_count;
	char	*obj_type;

	if (!data || !obj || !parts)
		return (false);
	expected_count = 3;
	if (type == CYLINDER || type == CONE)
	{
		expected_count = 5;
		if (type == CYLINDER)
			obj_type = "cylinder";
		else
			obj_type = "cone";
	}
	if (type == SPHERE)
		obj_type = "sphere";
	else if (type == PLANE)
		obj_type = "plane";
	if (!validate_parts_count(parts, expected_count, obj_type))
		return (false);
	if (type == PLANE)
		*obj = &data->scene.planes[data->scene.plane_count];
	else
		*obj = &data->scene.objects[data->scene.object_count];
	(*obj)->type = type;
	return (true);
}

bool	parse_positive_float(char **parts, int index, float *value,
		char *error_msg)
{
	bool	result;

	if (!parts || !parts[index] || !value || !error_msg)
		return (false);
	result = parse_float(parts[index], value);
	if (!result || *value <= 0)
	{
		printf("Error\n%s\n", error_msg);
		return (false);
	}
	return (true);
}

bool	parse_object_position(Arena *a, char **parts, t_vector *position)
{
	bool	result;

	if (!parts || !parts[0] || !position)
		return (false);
	result = parse_vector(a, parts[0], position);
	return (result);
}

bool	parse_object_direction(Arena *a, char **parts, t_vector *direction)
{
	bool	result;

	if (!parts || !parts[1] || !direction)
		return (false);
	result = parse_normalized_vector(a, parts[1], direction);
	return (result);
}
