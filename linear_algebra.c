#include "data.h"
#include "linear_algebra.h"
#include "duke.h"


t_vertex	*multiply(t_vertex *dst, t_vertex *vec, float k)
{
	dst->x = vec->x * k;
	dst->y = vec->y * k;
	dst->z = vec->z * k;
	return (dst);
}

float		dot(t_vertex *v1, t_vertex *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

t_vertex	*cross(t_vertex *dst, t_vertex *v1, t_vertex *v2)
{
	dst->x = v1->y * v2->z - v1->z * v2->y;
	dst->y = v1->z * v2->x - v1->x * v2->z;
	dst->z = v1->x * v2->y - v1->y * v2->x;
	return (dst);
}

t_vertex	*add(t_vertex *dst, t_vertex *v1, t_vertex *v2)
{
	//return ((t_vertex){
	dst->x = v1->x + v2->x;
	dst->y = v1->y + v2->y;
	dst->z = v1->z + v2->z;
//	});
	return (dst);
}

float		length(t_vertex *vec)
{
	return (sqrt(dot(vec, vec)));
}

t_mat4x4	*make_oy_rot_matrix(t_mat4x4 *dst, float degrees)
{
	float		sinus;
	float		cosinus;
//	t_mat4x4	mat;


	sinus = sin(degrees * 3.1415f / 180.0f);
	cosinus = cos(degrees * 3.1415f / 180.0f);

	dst->mat[0][0] = cosinus;
	dst->mat[0][1] = 0;
	dst->mat[0][2] = -sinus;
	dst->mat[0][3] = 0;

	dst->mat[1][0] = 0;
	dst->mat[1][1] = 1;
	dst->mat[1][2] = 0;
	dst->mat[1][3] = 0;

	dst->mat[2][0] = sinus;
	dst->mat[2][1] = 0;
	dst->mat[2][2] = cosinus;
	dst->mat[2][3] = 0;

	dst->mat[3][0] = 0;
	dst->mat[3][1] = 0;
	dst->mat[3][2] = 0;
	dst->mat[3][3] = 1;

	return (dst);
}

t_mat4x4	*make_translation_matrix(t_mat4x4 *dst, t_vertex *translation)
{
//	t_mat4x4	*mat;

//	mat = malloc(sizeof(t_mat4x4));/////////////////////////
	dst->mat[0][0] = 1;
	dst->mat[0][1] = 0;
	dst->mat[0][2] = 0;
	dst->mat[0][3] = translation->x;

	dst->mat[1][0] = 0;
	dst->mat[1][1] = 1;
	dst->mat[1][2] = 0;
	dst->mat[1][3] = translation->y;

	dst->mat[2][0] = 0;
	dst->mat[2][1] = 0;
	dst->mat[2][2] = 1;
	dst->mat[2][3] = translation->z;

	dst->mat[3][0] = 0;
	dst->mat[3][1] = 0;
	dst->mat[3][2] = 0;
	dst->mat[3][3] = 1;

	return (dst);
}

t_mat4x4	*make_scaling_matrix(t_mat4x4 *dst, float scale)
{
	//t_mat4x4	*mat;

//	mat = malloc(sizeof(t_mat4x4));/////////////////////////
	dst->mat[0][0] = scale;
	dst->mat[0][1] = 0;
	dst->mat[0][2] = 0;
	dst->mat[0][3] = 0;

	dst->mat[1][0] = 0;
	dst->mat[1][1] = scale;
	dst->mat[1][2] = 0;
	dst->mat[1][3] = 0;

	dst->mat[2][0] = 0;
	dst->mat[2][1] = 0;
	dst->mat[2][2] = scale;
	dst->mat[2][3] = 0;

	dst->mat[3][0] = 0;
	dst->mat[3][1] = 0;
	dst->mat[3][2] = 0;
	dst->mat[3][3] = 1;

	return (dst);
}

t_vertex4	*multiply_m_v(t_vertex4 *dst, t_mat4x4 *mat, t_vertex4 *vec)
{
	float	result[4];
	float	tmp[4];
	int		i;
	int		j;

	bzero(result, sizeof(float) * 4);///////////////////////
	

	tmp[0] = vec->x;
	tmp[1] = vec->y;
	tmp[2] = vec->z;
	tmp[3] = vec->w;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result[i] += mat->mat[i][j] * tmp[j];
			j++;
		}
		i++;
	}
	dst->x = result[0];
	dst->y = result[1];
	dst->z = result[2];
	dst->w = result[3];

	// return ((t_vertex4){
	// 	result[0],
	// 	result[1],
	// 	result[2],
	// 	result[3],
	// });
	return (dst);
}

t_mat4x4	*multiply_m_m(t_mat4x4 *dst, t_mat4x4 *m1, t_mat4x4 *m2)
{
	//t_mat4x4	result;
	int			i;
	int			j;
	int			k;

	i = 0;

	
	t_mat4x4	tmp1;
	t_mat4x4	tmp2;

	tmp1 = *m1;
	tmp2 = *m2;
	bzero(dst, sizeof(t_mat4x4));//////////////////////////////
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			k = 0;
			while (k < 4)
			{
				dst->mat[i][j] += tmp1.mat[i][k] * tmp2.mat[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (dst);
}

t_mat4x4	*transposed_m(t_mat4x4 *dst, t_mat4x4 *src)
{
	// t_mat4x4	result;
	int			i;
	int			j;
	t_mat4x4	tmp;

	i = 0;
	tmp = *src;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			dst->mat[i][j] = tmp.mat[j][i];
			j++;
		}
		i++;
	}
	return (dst);
}

// t_vertex	triangle_normal(t_vertex v0, t_vertex v1, t_vertex v2)
// {
// 	t_vertex v0v1;
// 	t_vertex v0v2;
// 	t_vertex tmp;

// 	tmp = multiply(v0, -1.0);
// 	v0v1 = add(v1, tmp);

// 	v0v2 = add(v2, tmp);
// 	return (cross(v0v1, v0v2));
// }
///////////////убрать отсюда

t_point		viewport_to_canvas(t_point p){
	return ((t_point){
		p.x * W / CW,
		p.y * H / CH
	});
}
t_point		project_vertex(t_vertex v)
{
	return (viewport_to_canvas(
		(t_point){
			v.x * PROJ_PLANE_Z / v.z,
			v.y * PROJ_PLANE_Z / v.z
		}
	));
}

t_mat4x4	update_instance_transform(t_instance *instance)
{
	t_mat4x4 scale;
	t_mat4x4 transition;
	
	multiply_m_m(&instance->transform,
					make_translation_matrix(&transition, &instance->position),
					multiply_m_m(&instance->transform, &instance->orientation,
					make_scaling_matrix(&scale, instance->scale)));
}