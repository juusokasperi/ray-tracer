#include "mini_rt.h"
#include "thread_tile_inline.h"

static t_vector	get_random_point_on_light(t_light light, unsigned int *seed)
{
	t_vector	point;
	float		theta;
	float		phi;
	float		r;

	theta = 2.0f * M_PI * random_float(seed);
	phi = acosf(2.0f * random_float(seed) - 1.0f);
	r = light.radius * cbrtf(random_float(seed));
	point.x = r * sinf(phi) * cosf(theta);
	point.y = r * sinf(phi) * sinf(theta);
	point.z = r * cosf(phi);
	return (vector_add(light.pos, point));
}

float	calculate_shadow_factor(t_data *data, t_surface *surf,
			t_light light, unsigned int *seed)
{
	t_vector	sample_pt;
	t_ray		shadow_ray;
	float		dist;
	t_vector	dir;

	if (light.type == LIGHT_POINT || light.radius <= EPSILON)
	{
		shadow_ray = build_light_ray(surf->point, light, surf->normal, &dist);
		return (in_shadow(shadow_ray, data, dist) ? 0.0f : 1.0f);
	}
	sample_pt = get_random_point_on_light(light, seed);
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
