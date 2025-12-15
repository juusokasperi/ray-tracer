/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:48:12 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/17 00:47:06 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void build_basis(t_vector4 axis_up, t_vector4 *right, t_vector4 *forward)
{
    t_vector4 helper = {0, 0, 1, 0};
    if (fabsf(axis_up.z) > 0.99f)
        helper = (t_vector4){1, 0, 0, 0};
    *right = vector_cross(helper, axis_up);
    vector_normalize(right);
    *forward = vector_cross(axis_up, *right);
    vector_normalize(forward);
}

t_ray transform_ray(t_ray ray, t_point center, t_vector4 axis)
{
    t_vector4 u, v, w;
    t_vector4 diff;
    t_ray     local_ray;

    v = axis; 
    build_basis(v, &u, &w);
    diff = vector_subtract(ray.origin, center);
    local_ray.origin.x = vector_dot(diff, u);
    local_ray.origin.y = vector_dot(diff, v);
    local_ray.origin.z = vector_dot(diff, w);
    local_ray.direction.x = vector_dot(ray.direction, u);
    local_ray.direction.y = vector_dot(ray.direction, v);
    local_ray.direction.z = vector_dot(ray.direction, w);
    return (local_ray);
}

