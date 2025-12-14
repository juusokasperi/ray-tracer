/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:23:55 by jrinta-           #+#    #+#             */
/*   Updated: 2025/12/13 20:14:29 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/*
	@return	t_rgb struct for mlx_put_pixel.
*/
t_rgb	calculate_color(t_data *data, t_surface *surf, unsigned int *seed)
{
	t_rgb			final_color;
	t_rgb			light_contrib;
	t_ray			light_ray;
	float			light_dist;
	float			shadow_intensity;
	int				i;

	final_color = calculate_ambient(*surf->obj, data->ambient);
	i = -1;
	while (++i < data->scene.light_count)
	{
		shadow_intensity = calculate_shadow_factor(data, surf,
				data->scene.lights[i], seed);
		if (shadow_intensity > 0.0f)
		{
			light_ray = build_light_ray(surf->point, data->scene.lights[i],
				surf->normal, &light_dist);
			if (!light_visible(surf->ray_origin, data->scene.lights[i].pos, *surf->obj))
				continue ;
			light_contrib = calculate_light_contribution(data->scene.lights[i],
				surf, light_ray, light_dist);
			light_contrib = rgb_scalar_multiply(light_contrib, shadow_intensity);
			if (light_contrib.r > 0 || light_contrib.g > 0 || light_contrib.b > 0)
				final_color = rgb_add(final_color, light_contrib);
		}
	}
	return (final_color);
}

void	fill_black(t_data *data)
{
	int	x;
	int	y;

	y = -1;
	while (++y < data->height)
	{
		x = -1;
		while (++x < data->width)
			mlx_put_pixel(data->mlx_img, x, y, 0x000000FF);
	}
}
