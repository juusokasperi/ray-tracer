#ifndef THREAD_TILE_INLINE_H
# define THREAD_TILE_INLINE_H

static inline float random_float(unsigned int *state)
{
	unsigned int result;

	*state = *state * 747796405u + 2891336453u;
	result = ((*state >> ((*state >> 28u) + 4u)) ^ *state) * 277803737u;
	result = (result >> 22u) ^ result;
	return (result & 0xFFFFFF) / 16777216.0f;
}

static inline int	get_next_tile_index(t_thread_ctx *ctx, int max_tiles)
{
	int	tile_index;

	tile_index = atomic_fetch_add(&ctx->next_tile, 1);
	if (tile_index >= max_tiles)
		return (-1);
	return (tile_index);
}

static inline t_tile	get_tile(int width, int height, int tile_index, int tiles_x)
{
	t_tile	tile;
	int		tile_x;
	int		tile_y;

	tile_x = tile_index % tiles_x;
	tile_y = tile_index / tiles_x;
	tile.start_x = tile_x * TILE_SIZE;
	tile.start_y = tile_y * TILE_SIZE;
	tile.end_x = (tile_x + 1) * TILE_SIZE;
	tile.end_y = (tile_y + 1) * TILE_SIZE;
	if (tile.end_x > width)
		tile.end_x = width;
	if (tile.end_y > height)
		tile.end_y = height;
	return (tile);
}

static inline int	get_max_tiles(int width, int height, int *tiles_x)
{
	int	tiles_y;

	*tiles_x = (width + TILE_SIZE - 1) / TILE_SIZE;
	tiles_y = (height + TILE_SIZE - 1) / TILE_SIZE;
	return (*tiles_x * tiles_y);
}


#endif
