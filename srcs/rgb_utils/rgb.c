/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:35:17 by jrinta-           #+#    #+#             */
/*   Updated: 2025/04/27 16:04:32 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

static int	clamp(int n)
{
	if (n > 255)
		return (255);
	else if (n < 0)
		return (0);
	else
		return (n);
}

t_rgb	rgb(int r, int g, int b)
{
	t_rgb	tmp;

	tmp.r = clamp(r);
	tmp.g = clamp(g);
	tmp.b = clamp(b);
	return (tmp);
}

uint32_t	rgb_to_uint(t_rgb a)
{
	return (clamp(a.r) << 24 | clamp(a.g) << 16 | clamp(a.b) << 8 | 0xFF);
}

t_vector	rgb_to_vec(t_rgb rgb)
{
	t_vector	vec;

	vec.x = rgb.r;
	vec.y = rgb.g;
	vec.z = rgb.b;
	vec.w = 0;
	return (vec);
}

t_rgb	vec_to_rgb(t_vector vec)
{
	t_rgb	rgb;

	rgb.r = (int)vec.x;
	rgb.g = (int)vec.y;
	rgb.b = (int)vec.z;
	return (rgb);
}
