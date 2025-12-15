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

static void	intersect_planes(t_ray ray, t_scene scene, t_obj_t *context);

float	find_closest_intersection(t_ray ray, t_data *data, 
			t_object_geom *closest)
{
	t_obj_t		context;

	context.min_t = -1;
	context.closest = closest;
	context.objects = data->scene.geometry;
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
		t = ray_intersect(ray, &scene.plane_geometry[i]);
		if (t > EPSILON
			&& (t <= context->min_t || context->min_t == -1))
		{
			context->min_t = t;
			*(context->closest) = scene.plane_geometry[i];
		}
	}
}

/*
	Traverses the BVH tree for intersection between the view_ray and objects.
*/
bool bvh_traverse(t_ray ray, t_bvh *bvh, t_obj_t *context, 
                           uint32_t start_node)
{
    t_bvh_node  *node;
    uint32_t    node_i;
    uint32_t    stack[64];
    int         stack_ptr;
    float       t_enter, t_exit;
    bool        hit_any = false;

    stack_ptr = 0;
    stack[stack_ptr++] = start_node;

    while (stack_ptr > 0)
    {
        node_i = stack[--stack_ptr];
        node = &bvh->nodes[node_i];

        if (!ray_aabb_intersect(&ray, &node->bounds, &t_enter, &t_exit))
            continue;
        
        if (t_exit < EPSILON)
            continue;
        if (context->min_t > 0 && t_enter > context->min_t)
            continue;

        if (node->left_right == NO_CHILDREN)
        {
            uint16_t first = get_first(node->first_count);
            uint16_t count = get_count(node->first_count);
            
            for (uint16_t i = 0; i < count; i++)
            {
                float t = ray_intersect(ray, &context->objects[first + i]);
                if (t > EPSILON && (t < context->min_t || context->min_t == -1))
                {
                    context->min_t = t;
                    *(context->closest) = context->objects[first + i];
                    hit_any = true;
                }
            }
        }
        else
            push_children_sorted(bvh, ray, node->left_right, 
                               stack, &stack_ptr);
    }
    return (hit_any);
}
