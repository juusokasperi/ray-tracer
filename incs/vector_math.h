/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_math.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:13:36 by jrinta-           #+#    #+#             */
/*   Updated: 2025/12/12 11:57:10 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_MATH_H
# define VECTOR_MATH_H

# include "structs.h"
# include <math.h>

static inline t_vector	vector(float x, float y, float z)
{
	t_vector	res;

	res.x = x;
	res.y = y;
	res.z = z;
	res.w = 0;
	return (res);
}

static inline t_vector	vector_add(t_vector a, t_vector b)
{
	t_vector	res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return (res);
}

static inline t_vector	vector_subtract(t_vector a, t_vector b)
{
	t_vector	res;

	res.x = a.x - b.x;
	res.y = a.y - b.y;
	res.z = a.z - b.z;
	return (res);
}

static inline t_vector	vector_multiply(t_vector a, float scalar)
{
	t_vector	res;

	res.x = scalar * a.x;
	res.y = scalar * a.y;
	res.z = scalar * a.z;
	return (res);
}

static inline float	vector_dot(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

static inline t_vector	vector_cross(t_vector a, t_vector b)
{
	t_vector	res;

	res.x = a.y * b.z - b.y * a.z;
	res.y = a.z * b.x - b.z * a.x;
	res.z = a.x * b.y - b.x * a.y;
	return (res);
}

static inline float	vector_magnitude(t_vector a)
{
	return (sqrtf(a.x * a.x + a.y * a.y + a.z * a.z));
}

static inline void	vector_normalize(t_vector *a)
{
	float	magnitude;
	float	inv_mag;

	magnitude = vector_magnitude(*a);
	if (magnitude > 0)
	{
		inv_mag = 1.0f / magnitude;
		a->x *= inv_mag;
		a->y *= inv_mag;
		a->z *= inv_mag;
	}
	else
	{
		a->x = 0;
		a->y = 0;
		a->z = 0;
	}
}

static inline t_vector	rotate_vector(t_vector v, t_vector k, float angle)
{
	t_vector	res;
	t_vector	cross;
	float		cos_a;
	float		sin_a;

	cos_a = cosf(angle);
	sin_a = sinf(angle);
	res = vector_multiply(v, cos_a);
	cross = vector_cross(k, v);
	cross = vector_multiply(cross, sin_a);
	res = vector_add(res, cross);
	k = vector_multiply(k, vector_dot(k, v) * (1.0f - cos_a));
	res = vector_add(res, k);
	return (res);
}

static inline t_vector vec_mul(t_vector a, t_vector b)
{
	return (vector(a.x * b.x, a.y * b.y, a.z * b.z));
}

#endif
