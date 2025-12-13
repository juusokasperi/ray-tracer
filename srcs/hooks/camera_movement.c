/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:42 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/12 20:22:46 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	move_camera(t_data *data, t_vector direction, float speed)
{
	t_vector	move;

	move = vector_multiply(direction, speed);
	data->cam.pos = vector_add(data->cam.pos, move);
	data->should_update = true;
}

bool	handle_camera_movement(t_data *data)
{
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_W))
		move_camera(data, data->cam.forward, data->cam_controls.move_speed);
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_S))
		move_camera(data, data->cam.forward, -data->cam_controls.move_speed);
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_A))
		move_camera(data, data->cam.right, -data->cam_controls.move_speed);
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_D))
		move_camera(data, data->cam.right, data->cam_controls.move_speed);
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_Q))
		move_camera(data, data->cam.up, data->cam_controls.move_speed);
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_E))
		move_camera(data, data->cam.up, -data->cam_controls.move_speed);
	return (data->should_update);
}
