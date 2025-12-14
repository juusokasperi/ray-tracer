#ifndef MATERIAL_UTILS_H
# define MATERIAL_UTILS_H

# include "structs.h"

t_vector	random_in_unit_sphere(unsigned int *seed);
t_vector	apply_glossiness(t_vector dir, float glossiness, unsigned int *seed);

// Reflection and refraction
t_vector	reflect_dir(t_vector in, t_vector n);
t_vector	refract_vec(t_vector uv, t_vector n, float etai_over_etat);
float		fresnel(float cosine, float ref_idx);

// Ray construction
t_ray		make_ray(t_point origin, t_vector dir);

// Color utilities
t_vector	rgb_to_vec_norm(t_rgb c);

#endif
