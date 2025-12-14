#include "material_utils.h"
#include "thread_tile_inline.h"

t_vector random_in_unit_sphere(unsigned int *seed)
{
	t_vector p;

	while (true)
	{
		p.x = random_float(seed) * 2.0f - 1.0f;
		p.y = random_float(seed) * 2.0f - 1.0f;
		p.z = random_float(seed) * 2.0f - 1.0f;
		p.w = 0.0f;
		if (vector_dot(p, p) < 1.0f)
			return (p);
	}
}

t_vector apply_glossiness(t_vector dir, float glossiness, unsigned int *seed)
{
	t_vector random_dir;
	t_vector res;

	if (glossiness < EPSILON)
		return (dir);
	random_dir = random_in_unit_sphere(seed);
	res = vector_add(dir, vector_multiply(random_dir, glossiness));
	vector_normalize(&res);
	return (res);
}

float fresnel(float cosine, float ref_idx)
{
	float r0;
	float x;
	float x2;

	r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
	r0 = r0 * r0;
	x = 1.0f - cosine;
	x2 = x * x;
	x = x2 * x2 * x;
	return (r0 + (1.0f - r0) * x);
}

t_vector refract_vec(t_vector uv, t_vector n, float etai_over_etat)
{
	float cos_theta;
	t_vector r_out_perp;
	float r_out_parallel;

	cos_theta = fminf(vector_dot(vector_multiply(uv, -1.0f), n), 1.0f);
	r_out_perp = vector_multiply(
		vector_add(uv, vector_multiply(n, cos_theta)), etai_over_etat);
	r_out_parallel = -sqrtf(fabsf(1.0f - vector_dot(r_out_perp, r_out_perp)));
	return (vector_add(r_out_perp, vector_multiply(n, r_out_parallel)));
}

t_vector reflect_dir(t_vector in, t_vector n)
{
	return (vector_subtract(in, vector_multiply(n, 2.0f * vector_dot(in, n))));
}

t_ray make_ray(t_point origin, t_vector dir)
{
	t_ray r;

	r.origin = origin;
	r.direction = dir;
	init_ray_inv(&r);
	return (r);
}

t_vector rgb_to_vec_norm(t_rgb c)
{
	return (vector(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f));
}

