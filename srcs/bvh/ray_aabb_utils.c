/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_aabb_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:11:01 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/22 16:58:34 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/*
	@return Boolean stating whether the ray intersect the AABB.
*/
bool	ray_aabb_intersect(t_ray *ray, t_aabb_bounds bounds,
	float *t_enter, float *t_exit)
{
	float	t0;
	float	t1;
	float	t_min;
	float	t_max;

	t0 = (bounds.min.x - ray->origin.x) * ray->inv_dir.x;
	t1 = (bounds.max.x - ray->origin.x) * ray->inv_dir.x;
	t_min = fminf(t0, t1);
	t_max = fmaxf(t0, t1);
	t0 = (bounds.min.y - ray->origin.y) * ray->inv_dir.y;
	t1 = (bounds.max.y - ray->origin.y) * ray->inv_dir.y;
	t_min = fmaxf(t_min, fminf(t0, t1));
	t_max = fminf(t_max, fmaxf(t0, t1));
	t0 = (bounds.min.z - ray->origin.z) * ray->inv_dir.z;
	t1 = (bounds.max.z - ray->origin.z) * ray->inv_dir.z;
	t_min = fmaxf(t_min, fminf(t0, t1));
	t_max = fminf(t_max, fmaxf(t0, t1));
	*t_enter = t_min;
	*t_exit = t_max;
	return (t_max >= t_min && t_max > 0.0f);
}
