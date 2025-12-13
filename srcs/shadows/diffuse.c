/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:18:26 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/22 10:39:49 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/*
 *
*/
t_rgb	calculate_light_contribution(t_light light, t_surface *surf,
			t_ray light_ray, float light_dist)
{
	t_rgb		diffuse;
	t_rgb		specular;
	t_vector	reflect_dir;
	float		n_dot_l;
	float		r_dot_v;
	float		attenuation;
	float		spec_intensity;
	float		effective_ratio;

	n_dot_l = vector_dot(surf->normal, light_ray.direction);
	if (n_dot_l <= EPSILON)
		return ((t_rgb){0, 0, 0});
	attenuation = 1.0f / (1.0f + 0.001f
			* light_dist + 0.0001f * light_dist * light_dist);
	effective_ratio = light.ratio * attenuation;
	diffuse = rgb_scalar_multiply(surf->obj->color, effective_ratio * n_dot_l);
	diffuse = rgb_scalar(diffuse, light.color);
	reflect_dir = vector_subtract(
			vector_multiply(surf->normal, 2.0f * n_dot_l),
			light_ray.direction);
	r_dot_v = vector_dot(reflect_dir, vector_multiply(surf->view_dir, -1.0f));
	if (r_dot_v > 0)
	{
		spec_intensity = powf(r_dot_v, 50.0f);
		specular = rgb_scalar_multiply(
				light.color, effective_ratio * spec_intensity);
		diffuse = rgb_add(diffuse, specular);
	}
	return (diffuse);
}

/*
Calculates the color that is present always whether the object is in
shadow or not.
*/
t_rgb	calculate_ambient(t_object obj, t_ambient ambient)
{
	t_rgb	res;

	res = rgb_scalar_multiply(obj.color, ambient.ratio);
	res = rgb_scalar(res, ambient.color);
	return (res);
}
