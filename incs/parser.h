/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrinta- <jrinta-@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:43:17 by phuocngu          #+#    #+#             */
/*   Updated: 2025/05/22 16:23:21 by jrinta-          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "structs.h"
# include <stdbool.h>
# include "memarena.h"

typedef struct s_line_context
{
	char	*line;
	int		idx;
	float	shininess;
	float	glossiness;
	float	refraction_index;
	float	reflectivity;
	bool	is_checkered;
	float	scale;
	t_rgb	color_2;
	t_data	*data;
}	t_line_context;

/* Main parsing functions */
bool	parse_scene(const char *filename, t_data *data);
bool	validate_scene_file(const char *filename, int *fd);
bool	parse_identifier(Arena *a, char *line, char **identifier, t_line_context *ctx);

/* Scene element parsing functions */
bool	validate_scene_content(t_data *data);
bool	allocate_scene_memory(t_data *data);
bool	cleanup(t_data *data);
bool	parse_ambient(char *line, t_data *data);
bool	parse_camera(char *line, t_data *data);
bool	parse_light(char *line, t_data *data, char *identifier);

/* Object parsing functions */
bool	parse_sphere(char *line, t_line_context *ctx);
bool	parse_plane(char *line, t_line_context *ctx);
bool	parse_cylinder(char *line, t_line_context *ctx);
bool	parse_cone(char *line, t_line_context *ctx);
bool	validate_object_count(t_data *data);

/* Object property parsing utilities */
bool	parse_object_position(Arena *a, char **parts, t_vector *position);
bool	parse_object_direction(Arena *a, char **parts, t_vector *direction);
bool	parse_object_color(Arena *a, char *color_str, t_object_mat *object,
		bool is_checkered, t_rgb *color_2);
void	apply_object_modifiers(t_object_mat *obj, t_line_context *ctx);

/* Common parsing utilities */
bool	validate_parts_count(char **parts, int expected_count, char *obj_name);
bool	validate_and_init_object(t_data *data, t_object_geom **obj_geom,
			t_object_mat **obj_mat, char **parts, t_shape type);
bool	parse_positive_float(char **parts, int index, float *value,
			char *error_msg);
bool	validate_range_float(float value, float min,
			float max, char *param_name);
bool	parse_ratio(char **parts, int index, float *ratio, char *param_name);
bool	parse_fov(char **parts, int index, int *fov);

/* Basic parsing functions */
bool	parse_float(char *str, float *result);
bool	parse_vector(Arena *a, char *str, t_vector *vec);
bool	parse_normalized_vector(Arena *a, char *str, t_vector *vec);
bool	parse_rgb(Arena *a, char *str, t_rgb *color);
bool	parse_dual_rgb(Arena *a, char *str, t_rgb *color_1, t_rgb *color_2);

/* String utilities */
bool	trim_line(Arena *a, char **line);

// arena/ utils 
char			*arena_strtrim(Arena *a, char const *s1, char const *set);
char 			**arena_split_isspace(Arena *a, const char *s);
char			**arena_split(Arena *a, const char *s, const char *delims);
char			*arena_strdup(Arena *a, const char *s);

#endif
