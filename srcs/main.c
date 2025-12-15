/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:38:16 by jrinta-           #+#    #+#             */
/*   Updated: 2025/06/25 19:30:34 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define MEMARENA_IMPLEMENTATION
#include "memarena.h"
#include "mini_rt.h"

void	init_data(t_data *data)
{
	data->should_update = false;
	data->width = WIDTH;
	data->height = HEIGHT;
	data->cam.vp.width = 2.0f * tanf(data->cam.vp.fov / 2 * M_PI / 180.0f);
	data->cam.vp.height = data->cam.vp.width
		/ ((float)data->width / (float)data->height);
	data->mlx_ptr = mlx_init(data->width, data->height, "miniRT", true);
	data->mlx_img = mlx_new_image(data->mlx_ptr, data->width, data->height);
	mlx_image_to_window(data->mlx_ptr, data->mlx_img, 0, 0);
}

static bool	check_arguments(int argc, char **argv, char **scene_file)
{
	if (argc != 2)
	{
		printf("Usage: %s <scene_file.rt>\n", argv[0]);
		return (false);
	}
	*scene_file = argv[1];
	return (true);
}

static bool	setup_scene(t_data *data, char *scene_file)
{
	bool	parse_status;

	parse_status = parse_scene(scene_file, data);
	if (!parse_status)
		return (false);
	data->bvh = init_bvh(data);
	return (true);
}

void	cleanup_data(t_data *data)
{
	cleanup_thread_pool(data);
	arena_free(&data->arena);
	arena_free(&data->frame.f_arena);
}

int	main(int argc, char **argv)
{
	t_data		data;
	char		*scene_file;

	printf("Size of object geom is: %zu\n", sizeof(t_object_geom));
	printf("Size of object mat is: %zu\n", sizeof(t_object_mat));
	printf("Size of bvh is: %zu\n", sizeof(t_bvh_node));

	memset(&data, 0, sizeof(t_data));
	data.arena = arena_init(PROT_READ | PROT_WRITE);
	data.frame.f_arena = arena_init(PROT_READ | PROT_WRITE);
	data.frame.accum_buffer = (t_vector4 *)arena_alloc_aligned(&data.frame.f_arena,
			sizeof(t_vector4) * WIDTH * HEIGHT, 16);
	if (!check_arguments(argc, argv, &scene_file))
		return (1);
	if (!setup_scene(&data, scene_file))
		return (1);
	if (!init_thread_pool(&data))
	{
		cleanup_data(&data);
		return (1);
	}
	setup_rendering(&data);
	mlx_loop_hook(data.mlx_ptr, loop_hook, &data);
	mlx_resize_hook(data.mlx_ptr, &resize_hook, &data);
	mlx_loop(data.mlx_ptr);
	cleanup_data(&data);
	mlx_terminate(data.mlx_ptr);
	return (0);
}
