/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_tile_worker.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:41:05 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/13 23:56:33 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"
#include "thread_tile_inline.h"

static inline t_vector	accumulate_sample(t_data *data, int idx, t_vector new_color)
{
	t_vector	accum;
	float		weight;

	if (data->frame.sample_count == 0)
	{
		data->frame.accum_buffer[idx] = new_color;
		return (new_color);
	}
	weight = (float)(data->frame.sample_count + 1);
	accum = data->frame.accum_buffer[idx];
	accum = vector_multiply(accum, weight - 1.0f);
	accum = vector_add(accum, new_color);
	accum = vector_multiply(accum, 1.0f / weight);
	data->frame.accum_buffer[idx] = accum;
	return (accum);
}

static inline t_vector	trace_pixel(t_pixel_ctx *ctx)
{
	t_ray		ray;
	t_object	closest;
	t_rgb		color;
	float		jitter[2];
	float		t;

	jitter[0] = random_float(ctx->rng_state);
	jitter[1] = random_float(ctx->rng_state);
	closest.type = NONE;
	ray = get_ray_for_px(ctx->data, ctx->x + jitter[0], ctx->y + jitter[1]);
	t = find_closest_intersection(ray, ctx->data, &closest);
	if (closest.type != NONE)
		color = calculate_color(ctx->data, closest, ray, t);
	else
		color = (t_rgb){0, 0, 0};
	return (rgb_to_vec(color));
}

static void	raytrace_tile(t_data *data, t_tile tile, unsigned int *rng_state)
{
	t_pixel_ctx	ctx;
	t_vector	new_color;
	t_vector	accum_color;
	t_rgb		final_rgb;

	ctx.data = data;
	ctx.rng_state = rng_state;
	ctx.y = tile.start_y - 1;
	while(++ctx.y < tile.end_y)
	{
		ctx.x = tile.start_x - 1;
		while (++ctx.x < tile.end_x)
		{
			ctx.idx = ctx.y * data->width + ctx.x;
			new_color = trace_pixel(&ctx);
			accum_color = accumulate_sample(ctx.data, ctx.idx, new_color);
			final_rgb = vec_to_rgb(accum_color);
			mlx_put_pixel(ctx.data->mlx_img, 
					ctx.x, ctx.y, rgb_to_uint(final_rgb));
		}
	}
}

void	*thread_tile_worker(void *arg)
{
	t_thread_ctx		*ctx;
	t_data				*data;
	int					tile_index;
	int					tiles_x;
	int					max_tiles;
	t_tile				tile;
	long				local_frame_id;
	unsigned int		rng_state;

	ctx = (t_thread_ctx *)arg;
	data = ctx->data;
	local_frame_id = -1;
	rng_state = (unsigned int)(uintptr_t)pthread_self() ^ (uintptr_t)time(NULL);
	while (1)
	{
		pthread_mutex_lock(&data->pool.work_mutex);
		while ((local_frame_id == data->pool.frame_id 
			|| !data->pool.working) && !data->pool.stop)
			pthread_cond_wait(&data->pool.work_cond, &data->pool.work_mutex);
		if (data->pool.stop)
		{
			pthread_mutex_unlock(&data->pool.work_mutex);
			break ;
		}
		local_frame_id = data->pool.frame_id;
		pthread_mutex_unlock(&data->pool.work_mutex);
		max_tiles = get_max_tiles(data->width, data->height, &tiles_x);
		while (1)
		{
			tile_index = get_next_tile_index(ctx, max_tiles);
			if (tile_index == -1)
				break ;
			tile = get_tile(
					ctx->data->width, ctx->data->height, tile_index, tiles_x);
			raytrace_tile(ctx->data, tile, &rng_state);
		}
		pthread_mutex_lock(&data->pool.done_mutex);
		data->pool.finished_count++;
		if (data->pool.finished_count == data->pool.thread_count)
			pthread_cond_signal(&data->pool.done_cond);
		pthread_mutex_unlock(&data->pool.done_mutex);
	}
	return (NULL);
}
