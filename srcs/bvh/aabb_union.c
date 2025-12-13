/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_union.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 01:04:06 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/18 00:13:58 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/*
	Creates a bigger AABB from two AABB boxes.

	@return Sum of two bounding boxes
*/
t_aabb_bounds	aabb_union(t_aabb_bounds a, t_aabb_bounds b)
{
	a.min.x = fminf(a.min.x, b.min.x);
	a.min.y = fminf(a.min.y, b.min.y);
	a.min.z = fminf(a.min.z, b.min.z);
	a.max.x = fmaxf(a.max.x, b.max.x);
	a.max.y = fmaxf(a.max.y, b.max.y);
	a.max.z = fmaxf(a.max.z, b.max.z);
	return (a);
}
