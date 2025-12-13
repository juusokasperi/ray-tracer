/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:35:17 by jrinta-           #+#    #+#             */
/*   Updated: 2025/04/27 16:04:06 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

t_rgb	rgb_add(t_rgb a, t_rgb b)
{
	t_rgb	res;

	res.r = a.r + b.r;
	res.g = a.g + b.g;
	res.b = a.b + b.b;
	return (res);
}

t_rgb	rgb_scalar_multiply(t_rgb a, float scalar)
{
	t_rgb	res;

	res.r = a.r * scalar;
	res.g = a.g * scalar;
	res.b = a.b * scalar;
	return (res);
}

t_rgb	rgb_scalar(t_rgb a, t_rgb b)
{
	t_rgb	res;

	res.r = (a.r * b.r) / 255;
	res.g = (a.g * b.g) / 255;
	res.b = (a.b * b.b) / 255;
	return (res);
}
