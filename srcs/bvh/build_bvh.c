/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_bvh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:08:42 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/22 11:35:51 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static int		partition_object(t_object *objects,
					int start, int end, int axis);
static uint32_t	build_leaf_node(t_bvh *bvh, t_object *objects,
					int start, int count);
static uint32_t	build_two_object_node(t_bvh *bvh, t_object *objects, int start);
static uint32_t	build_bvh(t_bvh *bvh, t_object *objects, int start, int end);

/*
	Creates a bounding volume hierarchy structure, which reduces
	the amount of computationally heavy intersection calculations.

	The objects in the scene are divided into axis-aligned bounding boxes
	which are then ordered in a tree-type structure which can eliminate
	a lot of intersection calculations.

	@return Generated BVH.
*/
t_bvh	init_bvh(t_data *data)
{
	t_bvh	bvh;
	size_t	size;

	if (data->scene.object_count == 0)
		size = sizeof(t_bvh_node) * 1;
	else
		size = sizeof(t_bvh_node) * (2 * data->scene.object_count - 1);
	bvh.nodes = arena_alloc(&data->arena, size);
	if (!bvh.nodes)
	{
		printf("Error\nMemory error in init_bvh().\n");
		cleanup_data(data);
		exit(1);
	}
	bvh.node_count = 0;
	build_bvh(&bvh, data->scene.objects, 0, data->scene.object_count - 1);
	return (bvh);
}

/*
	We do a quick sort from objects[start] to objects[end]
	based on the axis defined by the variable axis (X,Y,Z).

	@return Mid point of the array.
*/
static int	partition_object(t_object *objects, int start, int end, int axis)
{
	float		pivot_value;
	float		value;
	int			i;

	i = (start + end) / 2;
	pivot_value = get_centroid_info(objects[i], axis);
	swap_objects(&objects[i], &objects[end]);
	i = start;
	while (i < end)
	{
		value = get_centroid_info(objects[i], axis);
		if (value < pivot_value)
		{
			swap_objects(&objects[i], &objects[start]);
			start++;
		}
		i++;
	}
	swap_objects(&objects[start], &objects[end]);
	return (start);
}

/*
	Builds the leaf node, which in our case will always contain
	exactly 1 object.
*/
static uint32_t	build_leaf_node(t_bvh *bvh, t_object *objects,
					int start, int count)
{
	uint32_t		node_i;
	t_aabb_bounds	obj_bounds;
	t_aabb_bounds	bounds;
	int				i;

	node_i = bvh->node_count++;
	bounds = calculate_object_aabb(objects[start]);
	i = -1;
	while (++i < count)
	{
		obj_bounds = calculate_object_aabb(objects[start + i]);
		bounds = aabb_union(bounds, obj_bounds);
	}
	bvh->nodes[node_i].bounds = bounds;
	bvh->nodes[node_i].first_count = pack_first_count(start, count);
	bvh->nodes[node_i].left_right = pack_children(NO_CHILD, NO_CHILD);
	return (node_i);
}

/*
	If only two objects remain while building the bvh, we sort them and
	create leaf nodes for both of them.
*/
static uint32_t	build_two_object_node(t_bvh *bvh, t_object *objects, int start)
{
	uint32_t		node_i;
	uint32_t		left;
	uint32_t		right;
	t_aabb_bounds	bounds;
	int				axis;

	bounds = calculate_bounds(objects, start, start + 1);
	axis = get_largest_axis(bounds);
	node_i = bvh->node_count++;
	if (get_centroid_info(objects[start], axis)
		> get_centroid_info(objects[start + 1], axis))
		swap_objects(&objects[start], &objects[start + 1]);
	left = build_leaf_node(bvh, objects, start, 1);
	right = build_leaf_node(bvh, objects, start + 1, 1);
	bvh->nodes[node_i].bounds = bounds;
	bvh->nodes[node_i].first_count = pack_first_count(0, 0);
	bvh->nodes[node_i].left_right = pack_children(left, right);
	return (node_i);
}

/*
	Creates a BVH tree recursively.
*/
static uint32_t	build_bvh(t_bvh *bvh, t_object *objects, int start, int end)
{
	int			mid;
	uint32_t	node_i;
	int			count;
	uint32_t	left;
	uint32_t	right;

	count = end - start + 1;
	if (count <= 0)
		return (0);
	if (count == 1)
		return (build_leaf_node(bvh, objects, start, count));
	if (count == 2)
		return (build_two_object_node(bvh, objects, start));
	node_i = bvh->node_count++;
	bvh->nodes[node_i].bounds = calculate_bounds(objects, start, end);
	mid = partition_object(objects, start, end,
			get_largest_axis(bvh->nodes[node_i].bounds));
	if (mid == start || mid == end)
		mid = start + count / 2;
	left = build_bvh(bvh, objects, start, mid);
	right = build_bvh(bvh, objects, mid + 1, end);
	bvh->nodes[node_i].first_count = pack_first_count(0, 0);
	bvh->nodes[node_i].left_right = pack_children(left, right);
	return (node_i);
}
