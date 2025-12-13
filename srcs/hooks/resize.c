/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:27:46 by jrinta-           #+#    #+#             */
/*   Updated: 2025/06/25 19:30:45 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	resize_hook(int width, int height, void *param)
{
	t_data	*data;

	data = param;
	mlx_resize_image(data->mlx_img, width, height);
	arena_reset(&data->frame.f_arena);
	data->frame.accum_buffer = (t_vector *)arena_alloc_zeroed_aligned(&data->frame.f_arena,
			sizeof(t_vector) * data->width * data->height, 16);
	data->width = width;
	data->height = height;
	data->cam.vp.width = 2.0f * tanf(data->cam.vp.fov / 2 * M_PI / 180.0f);
	data->cam.vp.height = data->cam.vp.width
		/ ((float)data->width / (float)data->height);
	data->should_update = true;
}
