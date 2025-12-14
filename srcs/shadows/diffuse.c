/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:18:26 by jrinta-           #+#    #+#             */
/*   Updated: 2025/12/13 19:49:17 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static inline float light_atten(float d)
{
	return (1.0f / (1.0f + 0.001f
		* d + 0.0001f * d * d));
}

static inline t_rgb	diffuse_term(t_light l, t_surface *s, float ndl, float eff)
{
	t_rgb	d;

	d = rgb_scalar_multiply(s->obj->color, eff * ndl);
	return (rgb_scalar(d, l.color));
}

static inline t_rgb	specular_term(t_light l, t_surface *s,
				t_ray lr, float ndl, float eff)
{
	t_vector	r;
	float		rdv;

	if (s->obj->shininess <= 0.f)
		return ((t_rgb){0, 0, 0});
	r = vector_subtract(vector_multiply(s->normal, 2.0f * ndl), lr.direction);
	rdv = vector_dot(r, vector_multiply(s->view_dir, -1.0f));
	if (rdv <= 0)
		return ((t_rgb){0, 0, 0});
	return (rgb_scalar_multiply(l.color, eff * powf(rdv, s->obj->shininess)));
}

t_rgb	calculate_light_contribution(t_light light, t_surface *surf,
			t_ray light_ray, float light_dist)
{
	float	n_dot_l;
	float	effective_ratio;

	n_dot_l = vector_dot(surf->normal, light_ray.direction);
	if (n_dot_l <= EPSILON)
		return ((t_rgb){0, 0, 0});
	effective_ratio = light.ratio * light_atten(light_dist);
	return (rgb_add(
		diffuse_term(light, surf, n_dot_l, effective_ratio),
		specular_term(light, surf, light_ray, n_dot_l, effective_ratio)
	));
}

/*
Calculates the color that is present always whether the object is in
shadow or not.
*/
t_rgb	calculate_ambient(t_object *obj, t_ambient ambient)
{
	t_rgb	res;

	res = rgb_scalar_multiply(obj->color, ambient.ratio);
	res = rgb_scalar(res, ambient.color);
	return (res);
}
