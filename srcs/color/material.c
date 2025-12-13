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
	r_out_perp = vector_multiply(vector_add(uv, vector_multiply(n, cos_theta)), etai_over_etat);
	r_out_parallel = -sqrtf(fabsf(1.0f - vector_dot(r_out_perp, r_out_perp)));
	return (vector_add(r_out_perp, vector_multiply(n, r_out_parallel)));
}

static t_rgb	handle_glass(t_data *data, t_surface *surf, t_ray in_ray,
					int depth, unsigned int *seed)
{
	float		refraction_ratio;
	t_vector	unit_dir;
	float		cos_theta;
	float		sin_theta;
	t_vector	direction;
	bool		cannot_refract;

	unit_dir = in_ray.direction;
	if (vector_dot(unit_dir, surf->normal) > 0.0f)
	{
		surf->normal = vector_multiply(surf->normal, -1.0f);
		refraction_ratio = surf->obj->refraction_index;
	}
	else
		refraction_ratio = 1.0f / surf->obj->refraction_index;
	cos_theta = fminf(vector_dot(vector_multiply(unit_dir, -1.0f), surf->normal), 1.0f);
    sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
    cannot_refract = (refraction_ratio * sin_theta) > 1.0f;
	if (cannot_refract || fresnel(cos_theta, refraction_ratio) > random_float(seed))
        direction = vector_subtract(unit_dir, vector_multiply(surf->normal, 2.0f * vector_dot(unit_dir, surf->normal)));
    else
        direction = refract_vec(unit_dir, surf->normal, refraction_ratio);
	direction = apply_glossiness(direction, surf->obj->glossiness, seed);
    t_ray next_ray;
    next_ray.origin = surf->point; // NOTE: For refraction, offset is tricky. Usually 0 or pushed slightly along direction.
    next_ray.direction = direction;
    init_ray_inv(&next_ray);
    t_rgb color = trace_ray(data, next_ray, depth - 1, seed);
    return (rgb_scalar(color, surf->obj->color));
}

static t_rgb handle_opaque(t_data *data, t_surface *surf, t_ray in_ray, int depth, unsigned int *seed)
{
    // 1. Calculate Local Phong Color (Direct Light)
    // IMPORTANT: Your calculate_color needs the function signature update we discussed previously
    // passing 'seed' for soft shadows.
    t_rgb local_color = calculate_color(data, *surf->obj, in_ray, 0, seed); // t is unused inside now if we pass point in surf

    // 2. If Reflective, add reflection
    if (surf->obj->reflectivity > 0.0f)
    {
        t_vector reflect_dir = vector_subtract(in_ray.direction, 
            vector_multiply(surf->normal, 2.0f * vector_dot(in_ray.direction, surf->normal)));
        
        reflect_dir = apply_glossiness(reflect_dir, surf->obj->glossiness, seed);

        t_ray ref_ray;
        ref_ray.origin = vector_add(surf->point, vector_multiply(surf->normal, EPSILON));
        ref_ray.direction = reflect_dir;
        init_ray_inv(&ref_ray);

        t_rgb reflected_color = trace_ray(data, ref_ray, depth - 1, seed);

        // Mix: (1 - reflectivity) * local + reflectivity * reflected
        // Or for metals, often the reflection IS the color. Simple mix:
        t_rgb part1 = rgb_scalar_multiply(local_color, (1.0f - surf->obj->reflectivity));
        t_rgb part2 = rgb_scalar_multiply(reflected_color, surf->obj->reflectivity);
        return rgb_add(part1, part2);
    }

    return local_color;
}

t_rgb get_material_color(t_data *data, t_surface *surf, t_ray ray, int depth, unsigned int *seed)
{
    // Dielectric (Glass/Water)
    if (surf->obj->refraction_index > 0.0f)
        return handle_glass(data, surf, ray, depth, seed);
    
    // Opaque (Metal/Plastic)
    return handle_opaque(data, surf, ray, depth, seed);
}
