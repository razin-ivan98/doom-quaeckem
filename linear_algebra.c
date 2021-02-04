#include "data.h"
#include "linear_algebra.h"
#include "duke.h"

t_vertex	lines_intersect_p(t_vertex a1, t_vertex a2, t_vertex b1, t_vertex b2)
{
	t_vertex l1;
	t_vertex l2;
	t_vertex ret;

	l1 = get_line_by_points(a1, a2);
	l2 = get_line_by_points(b1, b2);
	ret = lines_intersect(l1, l2);

	return (ret);
}

t_vertex	get_line_by_points(t_vertex f, t_vertex s)
{
	t_vertex ret;

	ret.x = f.y - s.y;
	ret.y = s.x - f.x;
	ret.z = f.x * s.y - s.x * f.y;

	return (ret);
}

t_vertex	get_ort_line_by_point(t_vertex line, t_vertex p)
{
	t_vertex ret;

	ret.x = -line.y;
	ret.y = line.x;
	ret.z = -line.x * p.y + line.y * p.x;

	return (ret);
}

t_vertex	lines_intersect(t_vertex l1, t_vertex l2)
{
	t_vertex ret;
	if (l2.x * l1.y - l1.x * l2.y != 0)
	{
		ret.x = (l2.y * l1.z - l1.y * l2.z) /
				(l2.x * l1.y - l1.x * l2.y);
		if (l1.y != 0)
			ret.y = (-l1.x * ret.x - l1.z) / l1.y;
		else
			ret.y = (-l2.x * ret.x - l2.z) / l2.y;
	}
	else
	{
		ret.x = (l1.y * l2.z - l2.y * l1.z) /
				(l1.x * l2.y - l2.x * l1.y);
		if (l2.y != 0)
			ret.y = (-l2.x * ret.x - l2.z) / l2.y;
		else
			ret.y = (-l1.x * ret.x - l1.z) / l1.y;
	}

	ret.z = 0.0;

	return (ret);
}

t_vertex	multiply(t_vertex vec, float k)
{
	return ((t_vertex){vec.x * k, vec.y * k, vec.z * k});
}

float		dot(t_vertex v1, t_vertex v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_vertex	normalize(t_vertex v)
{
	return (multiply(v, 1.0 / length(v)));
}

t_vertex	sub(t_vertex v1, t_vertex v2)
{
	return ((t_vertex){
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	});
}

t_vertex	cross(t_vertex v1, t_vertex v2)
{
	return((t_vertex){
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	});
}

t_vertex	add(t_vertex v1, t_vertex v2)
{
	return ((t_vertex){
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	});
}

float		length(t_vertex vec)
{
	return (sqrt(dot(vec, vec)));
}

t_mat4x4	make_oy_rot_matrix(float degrees)
{
	float		sinus;
	float		cosinus;
	t_mat4x4	mat;


	sinus = sin(degrees * 3.1415f / 180.0f);
	cosinus = cos(degrees * 3.1415f / 180.0f);

	mat.mat[0][0] = cosinus;
	mat.mat[0][1] = 0;
	mat.mat[0][2] = -sinus;
	mat.mat[0][3] = 0;

	mat.mat[1][0] = 0;
	mat.mat[1][1] = 1;
	mat.mat[1][2] = 0;
	mat.mat[1][3] = 0;

	mat.mat[2][0] = sinus;
	mat.mat[2][1] = 0;
	mat.mat[2][2] = cosinus;
	mat.mat[2][3] = 0;

	mat.mat[3][0] = 0;
	mat.mat[3][1] = 0;
	mat.mat[3][2] = 0;
	mat.mat[3][3] = 1;

	return (mat);
}

t_mat4x4	make_ox_rot_matrix(float degrees)
{
	float		sinus;
	float		cosinus;
	t_mat4x4	mat;


	sinus = sin(degrees * 3.1415f / 180.0f);
	cosinus = cos(degrees * 3.1415f / 180.0f);

	mat.mat[0][0] = 1;
	mat.mat[0][1] = 0;
	mat.mat[0][2] = 0;
	mat.mat[0][3] = 0;

	mat.mat[1][0] = 0;
	mat.mat[1][1] = cosinus;
	mat.mat[1][2] = sinus;
	mat.mat[1][3] = 0;

	mat.mat[2][0] = 0;
	mat.mat[2][1] = -sinus;
	mat.mat[2][2] = cosinus;
	mat.mat[2][3] = 0;

	mat.mat[3][0] = 0;
	mat.mat[3][1] = 0;
	mat.mat[3][2] = 0;
	mat.mat[3][3] = 1;

	return (mat);
}

t_mat4x4	make_translation_matrix(t_vertex translation)
{
	t_mat4x4	mat;

	mat.mat[0][0] = 1;
	mat.mat[0][1] = 0;
	mat.mat[0][2] = 0;
	mat.mat[0][3] = translation.x;

	mat.mat[1][0] = 0;
	mat.mat[1][1] = 1;
	mat.mat[1][2] = 0;
	mat.mat[1][3] = translation.y;

	mat.mat[2][0] = 0;
	mat.mat[2][1] = 0;
	mat.mat[2][2] = 1;
	mat.mat[2][3] = translation.z;

	mat.mat[3][0] = 0;
	mat.mat[3][1] = 0;
	mat.mat[3][2] = 0;
	mat.mat[3][3] = 1;

	return (mat);
}

t_mat4x4	make_scaling_matrix(float scale)
{
	t_mat4x4	mat;

	mat.mat[0][0] = scale;
	mat.mat[0][1] = 0;
	mat.mat[0][2] = 0;
	mat.mat[0][3] = 0;

	mat.mat[1][0] = 0;
	mat.mat[1][1] = scale;
	mat.mat[1][2] = 0;
	mat.mat[1][3] = 0;

	mat.mat[2][0] = 0;
	mat.mat[2][1] = 0;
	mat.mat[2][2] = scale;
	mat.mat[2][3] = 0;

	mat.mat[3][0] = 0;
	mat.mat[3][1] = 0;
	mat.mat[3][2] = 0;
	mat.mat[3][3] = 1;

	return (mat);
}

t_vertex4	multiply_m_v(t_mat4x4 mat, t_vertex4 vec)
{
	float	result[4];
	float	tmp[4];
	int		i;
	int		j;

	ft_bzero(result, sizeof(float) * 4);

	tmp[0] = vec.x;
	tmp[1] = vec.y;
	tmp[2] = vec.z;
	tmp[3] = vec.w;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result[i] += mat.mat[i][j] * tmp[j];
			j++;
		}
		i++;
	}

	return ((t_vertex4){
		result[0],
		result[1],
		result[2],
		result[3],
	});
}

t_mat4x4	multiply_m_m(t_mat4x4 m1, t_mat4x4 m2)
{
	t_mat4x4	result;
	int			i;
	int			j;
	int			k;

	i = 0;

	ft_bzero(&result, sizeof(t_mat4x4));
	
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			k = 0;
			while (k < 4)
			{
				result.mat[i][j] += m1.mat[i][k] * m2.mat[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	return (result);
}

t_mat4x4	transposed_m(t_mat4x4 mat)
{
	t_mat4x4	result;
	int i;
	int j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result.mat[i][j] = mat.mat[j][i];
			j++;
		}
		i++;
	}
	return (result);
}

t_vertex	triangle_normal(t_vertex v0, t_vertex v1, t_vertex v2)
{
	t_vertex v0v1;
	t_vertex v0v2;
	t_vertex tmp;

	tmp = multiply(v0, -1.0);
	v0v1 = add(v1, tmp);

	v0v2 = add(v2, tmp);
	return (normalize(cross(v0v1, v0v2)));
}
///////////////убрать отсюда

t_point		viewport_to_canvas(t_point p){
	return ((t_point){
		p.x * W / CW,
		p.y * H / CH,
		0.0
	});
}
t_point		project_vertex(t_vertex v)
{
	return (viewport_to_canvas(
		(t_point){
			v.x * PROJ_PLANE_Z / v.z,
			v.y * PROJ_PLANE_Z / v.z,
			0.0
		}
	));
}

t_mat4x4	update_instance_transform(t_instance *instance)
{
	instance->transform = multiply_m_m(make_translation_matrix(instance->position),
					multiply_m_m(instance->orientation, make_scaling_matrix(instance->scale)));
	return (instance->transform);
}

float		sqr(float a)
{
	return (a * a);
}
