#ifndef LINEAR_ALGEBRA_H
# define LINEAR_ALGEBRA_H

# include "data.h"
# include <math.h>


t_vertex	multiply(t_vertex vec, float k);
float		dot(t_vertex v1, t_vertex v2);
t_vertex	cross(t_vertex v1, t_vertex v2);
t_vertex	add(t_vertex v1, t_vertex v2);
t_vertex	sub(t_vertex v1, t_vertex v2);
float		length(t_vertex vec);
t_mat4x4	make_oy_rot_matrix(float degrees);
t_mat4x4	make_translation_matrix(t_vertex translation);
t_mat4x4	make_scaling_matrix(float scale);
t_vertex4	multiply_m_v(t_mat4x4 mat, t_vertex4 vec);
t_mat4x4	multiply_m_m(t_mat4x4 m1, t_mat4x4 m2);
t_mat4x4	transposed_m(t_mat4x4 mat);
t_vertex	normalize(t_vertex v);

t_vertex	triangle_normal(t_vertex v0, t_vertex v1, t_vertex v2);

t_point		project_vertex(t_vertex v);




#endif