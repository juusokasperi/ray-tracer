/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_rotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:15:09 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/12 20:23:37 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

static void	rotate_camera_y(t_data *data, float angle)
{
	t_vector4	forward;
	t_vector4	right;

	forward = data->cam.forward;
	right = data->cam.right;
	data->cam.forward = rotate_vector(forward, vector(0, 1, 0), angle);
	data->cam.right = rotate_vector(right, vector(0, 1, 0), angle);
	data->cam.up = vector_cross(data->cam.right, data->cam.forward);
	vector_normalize(&data->cam.up);
	data->should_update = true;
}

static void	rotate_camera_x(t_data *data, float angle)
{
	t_vector4	forward;
	t_vector4	up;

	forward = data->cam.forward;
	up = data->cam.up;
	data->cam.forward = rotate_vector(forward, data->cam.right, angle);
	data->cam.up = rotate_vector(up, data->cam.right, angle);
	vector_normalize(&data->cam.forward);
	vector_normalize(&data->cam.up);
	data->cam.right = vector_cross(data->cam.forward, data->cam.up);
	vector_normalize(&data->cam.right);
	data->should_update = true;
}

bool	handle_camera_rotation(t_data *data)
{
	float	angle;

	angle = data->cam_controls.rotate_speed;
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_RIGHT))
		rotate_camera_y(data, -angle);
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_LEFT))
		rotate_camera_y(data, angle);
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_UP))
		rotate_camera_x(data, angle);
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_DOWN))
		rotate_camera_x(data, -angle);
	return (data->should_update);
}
