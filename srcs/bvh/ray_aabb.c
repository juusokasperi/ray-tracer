/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_aabb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:38:34 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/22 16:34:40 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	check_node_intersection(t_ray ray, t_bvh_node *node, float min_t);
static bool	process_leaf_node(t_ray ray, t_obj_t *context, t_bvh_node *node);
static void	intersect_planes(t_ray ray, t_scene scene, t_obj_t *context);

float	find_closest_intersection(t_ray ray, t_data *data, t_object *closest)
{
	t_obj_t		context;

	context.min_t = -1;
	context.closest = closest;
	context.objects = data->scene.objects;
	if (data->scene.plane_count > 0)
		intersect_planes(ray, data->scene, &context);
	if (data->scene.object_count > 0)
		bvh_traverse(ray, &data->bvh, &context, 0);
	if (context.min_t > 0)
		return (context.min_t);
	return (-1);
}

/*
	Checks for intersection against all planes in scene, since BVH struggles
	with planes.
*/
static void	intersect_planes(t_ray ray, t_scene scene, t_obj_t *context)
{
	int		i;
	float	t;

	t = -1;
	i = -1;
	while (++i < scene.plane_count)
	{
		t = ray_intersect(ray, &scene.planes[i]);
		if (t > EPSILON
			&& (t <= context->min_t || context->min_t == -1))
		{
			context->min_t = t;
			*(context->closest) = scene.planes[i];
		}
	}
}

/*
	Traverses the BVH tree for intersection between the view_ray and objects.

	Starts from the root node, and if intersection is found, traverses downwards
	in the tree, checking each child node.

	If an intersection with an object is found,
	sets context.min_t and context.closest.
*/
bool	bvh_traverse(t_ray ray, t_bvh *bvh, t_obj_t *context, uint32_t node_idx)
{
	t_bvh_node	*node;
	uint16_t	left_child;
	uint16_t	right_child;
	bool		hit_left;
	bool		hit_right;

	node = &bvh->nodes[node_idx];
	if (!check_node_intersection(ray, node, context->min_t))
		return (false);
	left_child = get_left_child(node->left_right);
	right_child = get_right_child(node->left_right);
	if (node->left_right == NO_CHILDREN)
		return (process_leaf_node(ray, context, node));
	hit_left = false;
	hit_right = false;
	if (left_child != NO_CHILD)
		hit_left = bvh_traverse(ray, bvh, context, left_child);
	if (right_child != NO_CHILD)
		hit_right = bvh_traverse(ray, bvh, context, right_child);
	return (hit_left || hit_right);
}

static bool	check_node_intersection(t_ray ray, t_bvh_node *node, float min_t)
{
	float	t[2];

	if (!ray_aabb_intersect(&ray, node->bounds, &t[0], &t[1]))
		return (false);
	if (t[1] < EPSILON)
		return (false);
	if (min_t != -1 && t[0] > min_t)
		return (false);
	return (true);
}

static bool	process_leaf_node(t_ray ray, t_obj_t *context, t_bvh_node *node)
{
	uint16_t	count;
	uint16_t	first;
	uint16_t	i;
	float		t;
	bool		hit;

	count = get_count(node->first_count);
	first = get_first(node->first_count);
	hit = false;
	i = -1;
	while (++i < count)
	{
		t = ray_intersect(ray, &context->objects[first + i]);
		if (t > EPSILON && (t < context->min_t || context->min_t == -1))
		{
			context->min_t = t;
			*(context->closest) = context->objects[first + i];
			hit = true;
		}
	}
	return (hit);
}
