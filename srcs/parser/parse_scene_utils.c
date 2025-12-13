/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:45:00 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/10 14:37:25 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

bool	validate_range_float(float value, float min, float max,
		char *param_name)
{
	if (value < min || value > max)
	{
		printf("Error\n%s must be between %.1f and %.1f\n",
			param_name, min, max);
		return (false);
	}
	return (true);
}

bool	parse_ratio(char **parts, int index, float *ratio, char *param_name)
{
	bool	result;

	if (!parts || !parts[index] || !ratio || !param_name)
		return (false);
	result = parse_float(parts[index], ratio);
	if (!result)
		return (false);
	return (validate_range_float(*ratio, 0.0, 2.0, param_name));
}
