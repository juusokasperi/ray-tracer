#include "mini_rt.h"

t_rgb	trace_ray(t_data *data, t_ray ray, int depth, unsigned int *seed)
{
	t_object	closest;
	t_surface	surf;
	float		t;

	if (depth <= 0)
		return ((t_rgb){0, 0, 0});

	closest.type = NONE;
	t = find_closest_intersection(ray, data, &closest);
	if (closest.type != NONE)
	{
		surf.obj = &closest;
		surf.point = get_point(ray, t);
		surf.view_dir = ray.direction;
		surf.normal = calculate_normal(ray.direction, surf.point, closest);
		check_checkerboard(&closest, surf.point);
		return (get_material_color(data, &surf, ray, depth, seed));
	}
	return ((t_rgb){0, 0, 0});
}
