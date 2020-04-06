#include "duke.h"

void		interpolate(float i0, float d0, float i1, float d1, t_float_array *dst)
{
	//t_float_array	result;
	int				i;
	float			a;
	float			d;

	dst->length = 0;
	if (i0 == i1)
	{
		//result.array = malloc(1 * sizeof(float));////////////////////////////////
		dst->length = 1;
		dst->array[0] = d0;
		return ;
	}
	dst->length = (int)i1 - (int)i0 + 1;
	//dst->array = malloc(result.length * sizeof(float));////////////////////////////////
	i = 0;
	d = d0;
	a = (d1 - d0) / (i1 - i0);
	while (i < dst->length)
	{
		dst->array[i] = d;
		d += a;
		i++;
	}
	// return (result);
}
 float get_inter_d(float i0, float d0, float i1, float d1)
 {
	 return ((d1 - d0) / (i1 - i0));
 }

void		concat(t_float_array *first, t_float_array *second)
{
//	float	*ptr;
	int		length;

//	ptr = first->array;
	length = first->length + second->length;

	//first->array = malloc(sizeof(float) * length);///////////////////
	//memcpy(first->array, ptr, sizeof(float) * first->length);
	memcpy(first->array + first->length, second->array, sizeof(float) * second->length);
	first->length = length;
	second->length = 0;
//	free(ptr);
	//return (first);
}

void	edge_interpolate(t_e_i_input in, t_render *render, t_edge_interpolate *edge)
{
//	t_float_array v01;
//	t_float_array v12;
//	t_float_array v02;

	


	interpolate(in.y0, in.v0, in.y1, in.v1, &edge->v012);
	interpolate(in.y1, in.v1, in.y2, in.v2, &render->v12);
	// interpolate(in.y0, in.v0, in.y2, in.v2, &render->v02);
	interpolate(in.y0, in.v0, in.y2, in.v2, &edge->v02);


	edge->v012.length -= 1;
	concat(&edge->v012, &render->v12);
//	free(v12.array);
//	edge->v02 = render->v02;
//	edge->v012 = render->v01;
	//return ((t_edge_interpolate){v02, v01});
}


t_e_i_output	get_edge_inter(t_e_i_input in)
{
	t_e_i_output out;

	out.v01 = get_inter_d(in.y0, in.v0, in.y1, in.v1);
	out.v12 = get_inter_d(in.y1, in.v1, in.y2, in.v2);
	out.v02 = get_inter_d(in.y0, in.v0, in.y2, in.v2);
	out.limit = in.y1;

	return (out);

}