#ifndef MATERIAL_UTILS_H
# define MATERIAL_UTILS_H

# include "structs.h"

t_vector4	random_in_unit_sphere(unsigned int *seed);
t_vector4	apply_glossiness(t_vector4 dir, float glossiness, unsigned int *seed);

// Reflection and refraction
t_vector4	reflect_dir(t_vector4 in, t_vector4 n);
t_vector4	refract_vec(t_vector4 uv, t_vector4 n, float etai_over_etat);
float		fresnel(float cosine, float ref_idx);

// Ray construction
t_ray		make_ray(t_point origin, t_vector4 dir);

// Color utilities
t_vector4	rgb_to_vec_norm(t_rgb c);

#endif
