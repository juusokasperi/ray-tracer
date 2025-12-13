/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phuocngu <phuocngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:04:59 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/13 20:15:15 by phuocngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

t_cam_controls	init_cam_controls(void)
{
	t_cam_controls	controls;

	controls.move_speed = 0.5f;
	controls.rotate_speed = 0.05f;
	return (controls);
}

void	print_controls(void)
{
	printf("\nCamera Controls:\n");
	printf("Move:\n");
	printf("  W: Forward\n");
	printf("  S: Backward\n");
	printf("  A: Left\n");
	printf("  D: Right\n");
	printf("  Q: Up\n");
	printf("  E: Down\n");
	printf("Rotate:\n");
	printf("  Up Arrow: Look up\n");
	printf("  Down Arrow: Look down\n");
	printf("  Left Arrow: Turn left\n");
	printf("  Right Arrow: Turn right\n");
	printf("Other:\n");
	printf("  ESC: Quit\n");
}

void	loop_hook(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (mlx_is_key_down(data->mlx_ptr, MLX_KEY_ESCAPE))
	{
		cleanup_data(data);
		mlx_terminate(data->mlx_ptr);
		exit(0);
	}
	handle_camera_movement(data);
	handle_camera_rotation(data);
	if (data->should_update == true)
		data->frame.sample_count = 0;
	if (data->frame.sample_count < MAX_SAMPLES)
		raycast_threaded(data);
	data->should_update = false;
}
