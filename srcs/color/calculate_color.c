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
#include "thread_tile_inline.h"

static bool should_skip_lighting(t_object *obj)
{
	return (obj->reflectivity > 0.8f || obj->refraction_index > 0.0f);
}

static bool should_sample_light(int light_idx, int light_count,
		float reflectivity, unsigned int *seed)
{
	if (reflectivity < 0.5f)
		return (true);
	if (light_count <= 2)
		return (true);
	if (light_idx == 0)
		return (true);
	if (reflectivity > 0.5f && reflectivity < 0.8f)
		return (random_float(seed) < 0.3f);
	return (false);
}

static bool	process_light(t_data *data, t_surface *surf, t_light light,
		unsigned int *seed, t_rgb *final_color)
{
	t_ray	light_ray;
	float	light_dist;
	float	shadow_intensity;
	t_rgb	light_contrib;

	if (!light_visible(surf->ray_origin, light.pos, surf->obj))
		return (false);
	light_ray = build_light_ray(surf->point, light, surf->normal, &light_dist);
	shadow_intensity = calculate_shadow_factor(data, surf, light, seed);
	if (shadow_intensity <= 0.0f)
		return (false);
	light_contrib = calculate_light_contribution(light, surf,
			light_ray, light_dist);
	if (light_contrib.r == 0 && light_contrib.g == 0 && light_contrib.b == 0)
		return (false);
	light_contrib = rgb_scalar_multiply(light_contrib, shadow_intensity);
	*final_color = rgb_add(*final_color, light_contrib);
	return (true);
}

/*
	@return	t_rgb struct for mlx_put_pixel.
	
	Optimizations:
	- Skip most direct lighting for glass/mirrors (they rely on bounced light)
	- Early visibility check before shadow rays
	- Sample fewer lights for glossy surfaces
	- Reordered checks for early exits
*/
t_rgb	calculate_color(t_data *data, t_surface *surf, unsigned int *seed)
{
	t_rgb	final_color;
	int		i;

	final_color = calculate_ambient(surf->obj, data->ambient);
	if (should_skip_lighting(surf->obj))
		return (final_color);
	i = -1;
	while (++i < data->scene.light_count)
	{
		if (!should_sample_light(i, data->scene.light_count,
				surf->obj->reflectivity, seed))
			continue ;
		process_light(data, surf, data->scene.lights[i], seed, &final_color);
	}
	return (final_color);
}

/*
	@return	t_rgb struct for mlx_put_pixel.
t_rgb	calculate_color(t_data *data, t_surface *surf, unsigned int *seed)
{
	t_rgb			final_color;
	t_rgb			light_contrib;
	t_ray			light_ray;
	float			light_dist;
	float			shadow_intensity;
	int				i;

	final_color = calculate_ambient(surf->obj, data->ambient);
	i = -1;
	while (++i < data->scene.light_count)
	{
		shadow_intensity = calculate_shadow_factor(data, surf,
				data->scene.lights[i], seed);
		if (shadow_intensity > 0.0f)
		{
			light_ray = build_light_ray(surf->point, data->scene.lights[i],
				surf->normal, &light_dist);
			if (!light_visible(surf->ray_origin, data->scene.lights[i].pos, surf->obj))
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
*/

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
