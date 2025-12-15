#include "mini_rt.h"
#include "thread_tile_inline.h"

static void	create_coordinate_system(t_vector4 n, t_vector4 *nt, t_vector4 *nb)
{
	if (fabs(n.x) > fabsf(n.y))
		*nt = vector(n.z, 0, -n.x);
	else
		*nt = vector(0, -n.z, n.y);
	vector_normalize(nt);
	*nb = vector_cross(n, *nt);
}

static t_vector4	get_random_point_on_light_disk(t_light light, t_point origin, unsigned int *seed)
{
	t_vector4	light_dir;
	t_vector4	u;
	t_vector4	v;
	t_vector4	point;
	float		theta;
	float		r;

	light_dir = vector_subtract(light.pos, origin);
	vector_normalize(&light_dir);
	create_coordinate_system(light_dir, &u, &v);
	r = light.radius * sqrtf(random_float(seed));
	theta = 2.0f * M_PI * random_float(seed);
	point = vector_add(light.pos, vector_multiply(u, r * cosf(theta)));
	point = vector_add(point, vector_multiply(v, r * sinf(theta)));
	return (point);
}

float	calculate_shadow_factor(t_data *data, t_surface *surf,
			t_light light, unsigned int *seed)
{
	t_vector4	sample_pt;
	t_ray		shadow_ray;
	float		dist;
	t_vector4	dir;

	if (light.type == LIGHT_POINT || light.radius <= EPSILON)
	{
		shadow_ray = build_light_ray(surf->point, light, surf->normal, &dist);
		shadow_ray.origin = vector_add(surf->point, vector_multiply(surf->normal, SHADOW_EPSILON));
		init_ray_inv(&shadow_ray);
		if (dist > SHADOW_EPSILON && !in_shadow(shadow_ray, data, dist - SHADOW_EPSILON))
			return (1.0f);
		return (0.0f);
	}
	sample_pt = get_random_point_on_light_disk(light, surf->point, seed);
	dir = vector_subtract(sample_pt, surf->point);
	dist = vector_magnitude(dir);
	shadow_ray.direction = vector_multiply(dir, 1.0f / dist);
	shadow_ray.origin = vector_add(surf->point,
			vector_multiply(surf->normal, SHADOW_EPSILON));
	init_ray_inv(&shadow_ray);
	if (in_shadow(shadow_ray, data, dist))
		return (0.0f);
	return (1.0f);
}
