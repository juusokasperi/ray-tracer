/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:09:26 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/19 18:32:07 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static bool	shadow_bvh(t_ray ray, t_data *data, float max_dist);

static bool shadow_planes(t_ray ray, t_data *data, float max_dist)
{
	int		i;
	float	t;

	i = -1;
	while (++i < data->scene.plane_count)
	{
		t = ray_intersect(ray, &data->scene.planes[i]);
		if (t > SHADOW_EPSILON && t < max_dist)
			return (true);
	}
	return (false);
}

/*
	Traverses through the BVH structure to find for possible
	intersections between the intersection point and the light source.

	@return boolean deciding whether or not the ray is in shadow.
*/
bool	in_shadow(t_ray light_ray, t_data *data, float max_dist)
{
	if (max_dist < EPSILON)
		return (false);
	if (data->scene.plane_count > 0)
		if (shadow_planes(light_ray, data, max_dist))
			return (true);
	if (data->scene.object_count > 0)
		if (shadow_bvh(light_ray, data, max_dist))
			return (true);
	return (false);
}

/*
	Checks for intersections between the ray and object(s) in the AABB.
*/
static bool	shadow_leaf_node(t_bvh_node *node, t_ray ray, t_object *objects,
			float max_dist)
{
	float		t;
	uint16_t	first;
	uint16_t	count;
	uint16_t	i;

	first = get_first(node->first_count);
	count = get_count(node->first_count);
	i = -1;
	while (++i < count)
	{
		t = ray_intersect(ray, &objects[first + i]);
		if (t > 0 && t < max_dist)
			return (true);
	}
	return (false);
}

static void	add_children_to_stack(uint32_t left_right,
	uint32_t stack[64], int *ptr)
{
	uint16_t	left;
	uint16_t	right;

	left = get_left_child(left_right);
	right = get_right_child(left_right);
	if (left != NO_CHILD)
		stack[(*ptr)++] = left;
	if (right != NO_CHILD)
		stack[(*ptr)++] = right;
}

/*
	Traverses the BVH tree in a non-recursive fashion using a stack.

	Init: pushing BVH node index 0 into the stack, incrementing
	the stack_ptr value.

	While loop;
		Decrements the stack_ptr value, sets the current node to
		bvh.nodes[node_i], checks for intersection in said node.
		In case of no intersection, jump back to start of while loop.

		In case of an intersection, we check whether the current node is a
		leaf node.
			If true, we check for an intersection in said node between
				ray and object. If intersection is found, return true.
		If it is not a leaf node, we add it's child(ren) to the stack and
		increment stack_ptr by 1 or 2, depending on the amount of children.
	*/
static bool	shadow_bvh(t_ray ray, t_data *data, float max_dist)
{
	t_bvh_node	*node;
	uint32_t	node_i;
	uint32_t	stack[64];
	int			stack_ptr;
	float		t[2];

	stack_ptr = 0;
	stack[stack_ptr++] = 0;
	while (stack_ptr > 0)
	{
		node_i = stack[--stack_ptr];
		node = &data->bvh.nodes[node_i];
		if (!ray_aabb_intersect(&ray, node->bounds, &t[0], &t[1])
			|| t[1] < SHADOW_EPSILON || t[0] > max_dist)
			continue ;
		if (node->left_right == NO_CHILDREN
			&& shadow_leaf_node(node, ray, data->scene.objects, max_dist))
			return (true);
		else
			add_children_to_stack(node->left_right, stack, &stack_ptr);
	}
	return (false);
}
