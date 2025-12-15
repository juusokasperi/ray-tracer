/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_rt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:53:22 by jrinta-           #+#    #+#             */
/*   Updated: 2025/05/22 16:58:15 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
# define MINI_RT_H

# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <pthread.h>
# include <stdio.h> // for printf
# include <unistd.h> // for read, close.
# include <fcntl.h> // for open
# include <ctype.h>
# include <sys/stat.h> // fstat
# include "memarena.h"
# include "structs.h"
# include "parser.h"
# include "bvh.h"
# include "vector_math.h"
# define WIDTH 800
# define HEIGHT 640
# define BOTTOM_DISK 0
# define TOP_DISK 1
# define X 0
# define Y 1
# define Z 2
# ifndef FLT_MAX
#  define FLT_MAX 340282346638528859811704183484516925440.0
# endif
# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define BACKGROUND_COLOR 0x000000FF

# define MAX_SAMPLES 512

# define MAX_DEPTH 5

# ifndef NUM_THREADS
#  define NUM_THREADS 4
# endif

# define TILE_SIZE 32

# define EPSILON 0.0001f
# define SHADOW_EPSILON 0.0001f
# define PLANE_EPSILON 0.00001f
# define BIGGER_EPSILON 0.001f

// threads
void			cleanup_thread_pool(t_data *data);
int				get_system_thread_count(void);
int				init_thread_pool(t_data *data);
void			print_threading_info(int thread_count);
void			raycast_threaded(t_data *data);
void			*thread_tile_worker(void *arg);

//	color/calculate_color.c
t_rgb	calculate_color(t_data *data, t_surface *surf, unsigned int *seed);

void			fill_black(t_data *data);
//	color/checkerboard.c
t_rgb	get_object_color(const t_object_geom *geom, const t_object_mat *mat, t_point world_hit);

// color/material.c
t_rgb			get_material_color(t_data *data, t_surface *surf,
					t_ray ray, int depth, unsigned int *seed);

//	hooks/resize.c
void			resize_hook(int width, int height, void *param);
//	hooks/camera_movement.c && camera_rotation.c
bool			handle_camera_movement(t_data *data);
bool			handle_camera_rotation(t_data *data);
//	hooks/camera_controls.c
t_cam_controls	init_cam_controls(void);
void			print_controls(void);
void			loop_hook(void *param);

//	intersect/intersect.c
float			ray_intersect(t_ray ray, t_object_geom *object);
t_ray 			transform_ray(t_ray ray, t_point center, t_vector4 axis);
//	intersect/cylinder.c
float			cylinder_ray_intersect(t_ray ray, t_cylinder *cylinder);
//	intersect/cone.c && cone_2.c
float			cone_ray_intersect(t_ray ray, t_cone *cone);
t_ray			transform_ray_cone(t_ray ray, t_cone cone);

// raycast/trace_ray.c
t_rgb			trace_ray(t_data *data, t_ray ray, int depth, unsigned int *seed);

//	rgb_utils/rgb.c && rgb_2.c
t_rgb			rgb_scalar(t_rgb a, t_rgb b);
t_rgb			rgb_scalar_multiply(t_rgb a, float scalar);
t_rgb			rgb_add(t_rgb a, t_rgb b);
t_rgb			rgb(int r, int g, int b);
uint32_t		rgb_to_uint(t_rgb a);
t_vector4		rgb_to_vec(t_rgb rgb);
t_rgb			vec_to_rgb(t_vector4 vec);

//	shadows/shadow_check.c && shadow_check_2.c
bool			light_visible(t_vector4 cam_pos, t_vector4 light_pos,
					const t_object_geom *obj);
bool			in_shadow(t_ray light_ray, t_data *data, float max_dist);
//	shadows/calculate_normals.c
t_point			calculate_normal(t_vector4 view_direction,
					t_point hit_point, t_object_geom *obj);

// shadows/soft_shadow.c)
float			calculate_shadow_factor(t_data *data, t_surface *surf, 
					t_light light, unsigned int *seed);

//	shadows/diffuse.c
t_rgb			calculate_light_contribution(t_light light, t_surface *surf,
					t_ray light_ray, float light_dist);
t_rgb			calculate_ambient(const t_object_mat *obj, t_ambient ambient);

//	utils/ray_utils.c
t_ray			build_light_ray(t_point hit_point, t_light light,
					t_vector4 normal, float *out_dist);
t_ray			get_ray_for_px(t_data *data, float px, float py);
t_point			get_point(t_ray ray, float t);
void			init_ray_inv(t_ray *ray);

//	utils/main_utils.c
void			setup_rendering(t_data *data);

//	debug/debug.c
void			print_cam_info(t_data data);
//	main.c
void			cleanup_data(t_data *data);
void			init_data(t_data *data);

#endif
