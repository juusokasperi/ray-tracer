/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:50:01 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/22 10:54:10 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_H
# define BVH_H

# include "structs.h"

# define NO_CHILD 0xFFFF
# define NO_CHILDREN 0xFFFFFFFF

//	aabb_union.c
t_aabb_bounds	aabb_union(t_aabb_bounds a, t_aabb_bounds b);
//	build_bvh.c
t_bvh			init_bvh(t_data *data);
//	calculate_aabb.c
t_aabb_bounds	calculate_object_aabb(t_object *obj);
//	destroy_bvh.c
void			destroy_bvh(t_bvh *bvh);
//	helpers_build.c
int				get_largest_axis(t_aabb_bounds *bounds);
float			get_centroid_info(t_object *obj, int axis);
void			swap_objects(t_object *obj_1, t_object *obj_2);
t_aabb_bounds	calculate_bounds(t_object *objects, int start, int end);
//	helpers_first_count.c
uint16_t		get_first(uint32_t first_count);
uint16_t		get_count(uint32_t first_count);
uint32_t		pack_first_count(uint16_t first, uint16_t count);
//	helpers_left_right.c
uint16_t		get_left_child(uint32_t left_right);
uint16_t		get_right_child(uint32_t left_right);
uint32_t		pack_children(uint16_t left, uint16_t right);
//	ray_aabb_utils.c
bool			ray_aabb_intersect(t_ray *ray, t_aabb_bounds *bounds,
					float *t_enter, float *t_exit);
//	ray_aabb.c
float			find_closest_intersection(t_ray ray, t_data *data,
					t_object *closest);
bool			bvh_traverse(t_ray ray, t_bvh *bvh,
					t_obj_t *context, uint32_t node_idx);
//	../debug/bvh_debug.c
void			debug_bvh(t_bvh bvh, t_object *objects);

#endif
