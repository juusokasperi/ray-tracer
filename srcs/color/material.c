#include "mini_rt.h"
#include "thread_tile_inline.h"

static t_vector	apply_glossiness(t_vector dir, float glossiness, unsigned int *seed)
{
	t_vector	random_dir;
	t_vector	res;
	float		theta;
	float		phi;
	float		r;

	if (glossiness < BIGGER_EPSILON)
		return (dir);
	theta = 2.0f * M_PI * random_float(seed);
	phi = acosf(2.0f * random_float(seed) - 1.0f);
	r = cbrtf(random_float(seed));
	random_dir.x = r * sinf(phi) * cosf(theta);
	random_dir.y = r * sinf(phi) * sinf(theta);
	random_dir.z = r * cosf(phi);

	res = vector_add(dir, vector_multiply(random_dir, glossiness));
	vector_normalize(&res);
	return (res);
}

static float	fresnel(float cosine, float ref_idx)
{
	float r0;

	r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1.0f - r0) * powf((1.0f - cosine), 5.0f));
}

static t_vector	refract_vec(t_vector uv, t_vector n, float etai_over_etat)
{
	float		cos_theta;
	t_vector	r_out_perp;
	float		r_out_parallel;

	cos_theta = fminf(vector_dot(vector_multiply(uv, -1.0f), n), 1.0f);
	r_out_perp = vector_multiply(
		vector_add(uv, vector_multiply(n, cos_theta)), etai_over_etat);
	r_out_parallel = -sqrtf(fabsf(1.0f - vector_dot(r_out_perp, r_out_perp)));
	return (vector_add(r_out_perp, vector_multiply(n, r_out_parallel)));
}

static inline t_vector	reflect_dir(t_vector in, t_vector n)
{
	return (vector_subtract(in, vector_multiply(n, 2.0f * vector_dot(in, n))));
}

static inline t_ray	make_ray(t_point origin, t_vector dir)
{
	t_ray	r;

	r.origin = origin;
	r.direction = dir;
	init_ray_inv(&r);
	return (r);
}

static inline t_rgb	tint_by_albedo(t_rgb c, t_rgb albedo)
{
	c.r *= albedo.r / 255.0f;
	c.g *= albedo.g / 255.0f;
	c.b *= albedo.b / 255.0f;
	return (c);
}

static t_rgb	handle_glass(t_data *data, t_surface *surf, t_ray in_ray,
					int depth, unsigned int *seed)
{
	float		eta;
	float		cos_theta;
	float		sin2_theta;
	t_vector	dir;
	t_rgb		color;

	if (vector_dot(in_ray.direction, surf->normal) > 0.0f)
	{
		surf->normal = vector_multiply(surf->normal, -1.0f);
		eta = surf->obj->refraction_index;
	}
	else
		eta = 1.0f / surf->obj->refraction_index;
	cos_theta = fminf(vector_dot(
		vector_multiply(in_ray.direction, -1.0f), surf->normal), 1.0f);
	sin2_theta = 1.0f - cos_theta * cos_theta;
	if ((eta * eta * sin2_theta) > 1.0f
		|| fresnel(cos_theta, eta) > random_float(seed))
		dir = reflect_dir(in_ray.direction, surf->normal);
	else
		dir = refract_vec(in_ray.direction, surf->normal, eta);
	dir = apply_glossiness(dir, surf->obj->glossiness, seed);
	color = trace_ray(data, make_ray(surf->point, dir), depth - 1, seed);
	return (rgb_scalar(color, surf->obj->color));
}

static t_rgb	handle_opaque(t_data *data, t_surface *surf, t_ray in_ray,
					int depth, unsigned int *seed)
{
	t_rgb		local;
	t_rgb		refl_rgb;
	t_vector	dir;
	t_ray		refl_ray;

	local = calculate_color(data, surf, seed);
	if (surf->obj->reflectivity < BIGGER_EPSILON)
		return (local);
	dir = reflect_dir(in_ray.direction, surf->normal);
	dir = apply_glossiness(dir, surf->obj->glossiness, seed);
	refl_ray = make_ray(surf->point, dir);
	refl_rgb = trace_ray(data, refl_ray, depth - 1, seed);
	refl_rgb = tint_by_albedo(refl_rgb, surf->obj->color);
	return (rgb_add(
		rgb_scalar_multiply(local, 1.0f - surf->obj->reflectivity),
		rgb_scalar_multiply(refl_rgb, surf->obj->reflectivity)
	));
}

t_rgb get_material_color(t_data *data, t_surface *surf, t_ray ray, int depth, unsigned int *seed)
{
	if (surf->obj->refraction_index > 0.0f)
		return (handle_glass(data, surf, ray, depth, seed));
	return (handle_opaque(data, surf, ray, depth, seed));
}
