/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:44:48 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/22 10:33:40 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

void	print_cam_info(t_data data)
{
	t_camera	camera;

	camera = data.cam;
	printf("Position: (%.2f, %.2f, %.2f)\n",
		camera.pos.x, camera.pos.y, camera.pos.z);
	printf("Forward: (%.2f, %.2f, %.2f)\n",
		camera.forward.x, camera.forward.y, camera.forward.z);
	printf("Right: (%.2f, %.2f, %.2f)\n",
		camera.right.x, camera.right.y, camera.right.z);
	printf("Up: (%.2f, %.2f, %.2f)\n",
		camera.up.x, camera.up.y, camera.up.z);
	printf("Viewport: Width=%.2f, Height=%.2f\n",
		camera.vp.width, camera.vp.height);
}

void	print_threading_info(int thread_count)
{
	printf("Threading information:\n");
	printf("  - Thread count: %d\n", thread_count);
	printf("  - Threading approach: Tiles (%dx%d pixels)\n", TILE_SIZE,
		TILE_SIZE);
}
