/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_float.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:52:10 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/19 13:08:08 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	parse_decimal_part(char **endptr, float *result, int sign)
{
	char	*decimal_start;
	int		decimal_places;
	float	decimal_part;

	(*endptr)++;
	decimal_start = *endptr;
	if (!isdigit(**endptr))
		return (true);
	decimal_part = (float)strtol(*endptr, endptr, 10);
	decimal_places = *endptr - decimal_start;
	if (sign < 0)
		*result = *result - (decimal_part / powf(10, decimal_places));
	else
		*result = *result + (decimal_part / powf(10, decimal_places));
	return (true);
}

bool	parse_float(char *str, float *result)
{
	char	*endptr;
	long	value;
	int		sign;

	if (!str || !result)
		return (false);
	if (str[0] == '-')
		sign = -1;
	else
		sign = 1;
	value = strtol(str, &endptr, 10);
	*result = (float)value;
	if (*endptr == '.')
		parse_decimal_part(&endptr, result, sign);
	if (sign == -1 && *result >= 0)
		*result *= -1;
	if (*endptr != '\0' && *endptr != ',')
	{
		printf("Error\nInvalid float value: %s\n", str);
		return (false);
	}
	return (true);
}
