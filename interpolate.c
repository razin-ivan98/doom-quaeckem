#include "duke.h"

t_float_array		interpolate(float i0, float d0, float i1, float d1)
{
	t_float_array	result;
	int				i;
	float			a;
	float			d;

	if (i0 == i1)
	{
		result.array = malloc(1 * sizeof(float));////////////////////////////////
		result.length = 1;
		result.array[0] = d0;
		return (result);
	}
	result.length = (int)i1 - (int)i0 + 1;
	result.array = malloc(result.length * sizeof(float));////////////////////////////////
	i = 0;
	d = d0;
	a = (d1 - d0) / (float)(i1 - i0);
	while (i < result.length)
	{
		result.array[i] = d;
		d += a;
		i++;
	}
	return (result);
}

t_float_array		*concat(t_float_array *first, t_float_array *second)
{
	float	*ptr;
	int		length;

	ptr = first->array;
	length = first->length + second->length;

	first->array = malloc(sizeof(float) * length);///////////////////
	memcpy(first->array, ptr, sizeof(float) * first->length);
	memcpy(first->array + first->length, second->array, sizeof(float) * second->length);
	free(ptr);
	return (first);
}

t_edge_interpolate	edge_interpolate(t_e_i_input in)
{
	t_float_array v01;
	t_float_array v12;
	t_float_array v02;

	v01 = interpolate(in.y0, in.v0, in.y1, in.v1);
	v12 = interpolate(in.y1, in.v1, in.y2, in.v2);
	v02 = interpolate(in.y0, in.v0, in.y2, in.v2);

	v01.length -= 1;
	concat(&v01, &v12);
	free(v12.array);
	return ((t_edge_interpolate){v02, v01});
}