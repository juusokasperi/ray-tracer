/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:13:36 by jrinta-           #+#    #+#             */
/*   Updated: 2025/12/13 20:12:55 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "MLX42/MLX42.h"
# include <pthread.h>
# include <stdatomic.h>
# include <stdbool.h>
# include "memarena.h"

typedef enum e_shape
{
	NONE = -1,
	SPHERE,
	PLANE,
	CYLINDER,
	CONE
}	t_shape;

typedef struct s_vector
{
	float	x;
	float	y;
	float	z;
	float	w;
}	t_vector;

typedef struct s_interval
{
	float	min;
	float	max;
}	t_interval;

typedef struct s_aabb_bounds
{
	t_vector	min;
	t_vector	max;
}	t_aabb_bounds;

typedef struct s_bvh_node
{
	t_aabb_bounds	bounds;
	uint32_t		left_right;
	uint32_t		first_count;
}	t_bvh_node;

typedef struct s_bvh
{
	t_bvh_node	*nodes;
	uint32_t	node_count;
}	t_bvh;

typedef struct s_vector		t_point;

typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
}	t_rgb;

typedef struct s_viewport
{
	float		width;
	float		height;
	int			fov;
	float		distance;
}	t_viewport;

typedef struct s_camera
{
	t_vector	pos;
	t_vector	forward;
	t_vector	right;
	t_vector	up;
	t_viewport	vp;
}	t_camera;

typedef struct s_cam_controls
{
	float	move_speed;
	float	rotate_speed;
}	t_cam_controls;

typedef struct s_ambient
{
	t_rgb	color;
	float	ratio;
}	t_ambient;

typedef enum e_light_type
{
	LIGHT_POINT,
	LIGHT_AREA
}	t_light_type;

typedef struct s_light
{
	t_light_type	type;
	t_rgb			color;
	t_vector		pos;
	float			ratio;
	float			radius;
}	t_light;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
	t_vector	inv_dir;
	int			sign[3];
}	t_ray;

typedef struct s_checkerboard
{
	t_rgb	color_1;
	t_rgb	color_2;
	float	scale;
}	t_checkerboard;

typedef struct s_sphere
{
	t_point	center;
	float	radius;
}	t_sphere;

typedef struct s_plane
{
	t_point		point;
	t_vector	normal;
}	t_plane;

typedef struct s_cylinder
{
	t_vector	center;
	t_vector	axis;
	float		radius;
	float		height;
	t_point		local_hit;
}	t_cylinder;

typedef struct s_cylinder	t_cone;

typedef struct s_object
{
	t_shape			type;
	t_rgb			color;
	bool			is_checkered;
	t_checkerboard	checkerboard;
	float			shininess;
	float			glossiness;
	float			refraction_index;
	float			reflectivity;
	union u_data
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
		t_cone		cone;
	}	data;
}	t_object;

typedef struct s_scene
{
	t_object	*objects;
	int			object_count;
	t_object	*planes;
	int			plane_count;
	t_light		*lights;
	int			light_count;
}	t_scene;

typedef struct s_data		t_data;

typedef struct s_thread_pool
{
	pthread_t		*threads;
	int				thread_count;
	pthread_mutex_t	work_mutex;
	pthread_cond_t	work_cond;
	pthread_mutex_t	done_mutex;
	pthread_cond_t	done_cond;
	int				finished_count;
	bool			working;
	bool			stop;
	long			frame_id;
}	t_thread_pool;

typedef struct s_tile
{
	int	start_x;
	int	start_y;
	int	end_x;
	int	end_y;
}	t_tile;

typedef struct s_thread_context
{
	t_data			*data;
	_Atomic int		next_tile __attribute__((aligned(64)));
	unsigned int	rng_state;
}	t_thread_ctx;

typedef struct s_frame
{
	t_vector	*accum_buffer;
	Arena		f_arena;
	int			sample_count;
}	t_frame;

typedef struct s_data
{
	t_camera		cam;
	t_ambient		ambient;
	mlx_t			*mlx_ptr;
	mlx_image_t		*mlx_img;
	t_scene			scene;
	t_bvh			bvh;
	t_cam_controls	cam_controls;
	int				width;
	int				height;
	bool			should_update;
	int				camera_count;
	int				ambient_count;
	t_thread_pool	pool;
	t_thread_ctx	ctx;
	Arena			arena;
	t_frame			frame;
}	t_data;

typedef struct s_obj_t
{
	float		min_t;
	t_object	*closest;
	t_object	*objects;
}	t_obj_t;

typedef struct s_surface_info
{
	t_point		point;
	t_vector	normal;
	t_vector	view_dir;
	t_point		ray_origin;
	t_object	*obj;
}	t_surface;

typedef struct s_light_calc
{
	float		n_dot_l;
	float		r_dot_v;
	float		attenuation;
	t_vector	reflect_dir;
	t_rgb		diffuse_contrib;
	t_rgb		specular_contrib;
}	t_light_calc;

typedef struct s_pixel_ctx
{
	t_data			*data;
	unsigned int	*rng_state;
	int				x;
	int				y;
	int				idx;
}	t_pixel_ctx;

#endif
