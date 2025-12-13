/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:31:09 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/19 17:57:52 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static char	*type(t_shape shape)
{
	if (shape == SPHERE)
		return ("sphere");
	if (shape == PLANE)
		return ("plane");
	if (shape == CONE)
		return ("cone");
	return ("cylinder");
}

static void	print_leaf_node(t_bvh_node node, int i, t_object *objects)
{
	uint16_t	first;
	uint16_t	count;
	uint16_t	j;

	first = get_first(node.first_count);
	count = get_count(node.first_count);
	printf("\033[0;32mNode [%d]:\033[0m\n", i);
	printf("  Is a leaf node\n");
	printf("  Contains %d objects.\n", count);
	j = -1;
	while (++j < count)
		printf("    Has an object[%d] of type: %s\n", first + j,
			type(objects[first + j].type));
}

static void	print_branch_node(t_bvh_node node, int i)
{
	uint16_t	left;
	uint16_t	right;

	printf("\033[0;32mNode [%d]:\033[0m\n", i);
	printf("  Is a parent node\n");
	left = get_left_child(node.left_right);
	right = get_right_child(node.left_right);
	if (left)
		printf("    Has a left child: %d\n", left);
	if (right)
		printf("    Has a right child: %d\n", right);
}

/*
	Prints out the structure of the BVH tree.
*/
void	debug_bvh(t_bvh bvh, t_object *objects)
{
	uint32_t	i;

	i = -1;
	while (++i < bvh.node_count)
	{
		if (bvh.nodes[i].left_right == NO_CHILDREN)
			print_leaf_node(bvh.nodes[i], i, objects);
		else
			print_branch_node(bvh.nodes[i], i);
	}
}
