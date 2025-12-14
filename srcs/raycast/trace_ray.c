#include "mini_rt.h"
#include "material_utils.h"
#include "structs.h"
#include "thread_tile_inline.h"

static void prepare_surface(t_surface *surf, t_ray ray, t_object_geom *geom,
			float t, t_scene *scene)
{
	surf->geom = geom;
	surf->mat = &scene->materials[geom->mat_idx];
	surf->point = get_point(ray, t);
	surf->view_dir = ray.direction;
	surf->ray_origin = ray.origin;
	surf->normal = calculate_normal(ray.direction, surf->point, geom);
	surf->mat->color = get_object_color(geom, surf->mat, surf->point);
}

static void adjust_normal_and_eta(t_vector ray_dir, t_vector *normal, 
								  float refraction_idx, float *eta)
{
	if (vector_dot(ray_dir, *normal) > 0.0f)
	{
		*normal = vector_multiply(*normal, -1.0f);
		*eta = refraction_idx;
	}
	else
		*eta = 1.0f / refraction_idx;
}

static bool should_reflect(t_vector ray_dir, t_vector normal, 
						   float eta, unsigned int *seed)
{
	float cos_theta;
	float sin2_theta;
	bool cannot_refract;

	cos_theta = fminf(vector_dot(vector_multiply(ray_dir, -1.0f), normal), 1.0f);
	sin2_theta = 1.0f - cos_theta * cos_theta;
	cannot_refract = (eta * eta * sin2_theta) > 1.0f;
	return (cannot_refract || fresnel(cos_theta, eta) > random_float(seed));
}

static t_ray handle_glass(t_ray ray, t_surface *surf,
						  t_vector *throughput, unsigned int *seed)
{
	t_vector surface_color;
	float eta;
	t_vector normal;
	t_vector r_dir;

	surface_color = rgb_to_vec_norm(surf->mat->color);
	*throughput = vec_mul(*throughput, surface_color);
	normal = surf->normal;
	adjust_normal_and_eta(ray.direction, &normal, surf->mat->refraction_index, &eta);
	if (should_reflect(ray.direction, normal, eta, seed))
		r_dir = reflect_dir(ray.direction, normal);
	else
		r_dir = refract_vec(ray.direction, normal, eta);
	return (make_ray(surf->point, apply_glossiness(r_dir, surf->mat->glossiness, seed)));
}

static t_ray	handle_opaque(t_data *data, t_ray ray, t_surface *surf, t_vector *throughput,
						t_vector *accum, bool *done, unsigned int *seed)
{
	t_rgb		direct_rgb;
	t_vector	direct_light;
	t_vector	diffuse;
	t_vector	dir;

	direct_rgb = calculate_color(data, surf, seed);
	direct_light = rgb_to_vec(direct_rgb);
	diffuse = vector_multiply(direct_light, 1.0f - surf->mat->reflectivity);
	diffuse = vec_mul(diffuse, *throughput);
	*accum = vector_add(*accum, diffuse);
	if (surf->mat->reflectivity < BIGGER_EPSILON)
	{
		*done = true;
		return (ray);
	}
	*throughput = vector_multiply(*throughput, surf->mat->reflectivity);
	*throughput = vec_mul(*throughput, rgb_to_vec_norm(surf->mat->color));
	dir = reflect_dir(ray.direction, surf->normal);
	return (make_ray(surf->point, apply_glossiness(dir, surf->mat->glossiness, seed)));
}

static bool russian_roulette(t_vector *throughput, int depth, unsigned int *seed)
{
	float p;

	if (depth <= 3)
		return (false);
	p = fmaxf(throughput->x, fmaxf(throughput->y, throughput->z));
	if (random_float(seed) > p)
		return (true);
	*throughput = vector_multiply(*throughput, 1.0f / p);
	return (false);
}

t_rgb trace_ray(t_data *data, t_ray ray, int max_depth, unsigned int *seed)
{
	t_vector	 	throughput;
	t_vector	 	accum_color;
	t_object_geom 	closest;
	t_surface 		surf;
	float	 		t;
	int				depth;
	bool			done;
	float			max_intensity;

	throughput = vector(1.0f, 1.0f, 1.0f);
	accum_color = vector(0.0f, 0.0f, 0.0f);
	depth = 0;
	while (depth < max_depth)
	{
		if (depth > 1)
		{
			max_intensity = fmaxf(throughput.x, fmaxf(throughput.y, throughput.z));
			if (max_intensity < 0.015f)
				break;
		}
		closest.type = NONE;
		t = find_closest_intersection(ray, data, &closest);
		if (closest.type == NONE)
			break;
		prepare_surface(&surf, ray, &closest, t, &data->scene);
		done = false;
		if (surf.mat->refraction_index > 0.0f)
			ray = handle_glass(ray, &surf, &throughput, seed);
		else
			ray = handle_opaque(data, ray, &surf, &throughput, 
								&accum_color, &done, seed);
		if (done || russian_roulette(&throughput, depth, seed))
			break;
		depth++;
	}
	return (vec_to_rgb(accum_color));
}

