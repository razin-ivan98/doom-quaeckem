#include "duke.h"

static void	get_new1(t_clip_triangle *cl, t_model *model, int k, int i)
{
	float t;

	t = (-cl->planes[k].distance -
		dot(cl->planes[k].normal,
			model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]])
		) / dot(cl->planes[k].normal,
			sub(model->vertexes[cl->crop[k][i].indexes[cl->outsides[0]]],
				model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]]));

	cl->new1 = add(model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]],
		multiply(sub(model->vertexes[cl->crop[k][i].indexes[cl->outsides[0]]],
			model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]]), t));
	
	cl->new_uv1.x = cl->crop[k][i].uvs[cl->outsides[0]].x +
		(cl->crop[k][i].uvs[cl->insides[0]].x -
		cl->crop[k][i].uvs[cl->outsides[0]].x) * (1 - t);
	cl->new_uv1.y = cl->crop[k][i].uvs[cl->outsides[0]].y +
		(cl->crop[k][i].uvs[cl->insides[0]].y -
		cl->crop[k][i].uvs[cl->outsides[0]].y) * (1 - t);
}

static void	get_new2(t_clip_triangle *cl, t_model *model, int k, int i)
{
	float t;

	t = (-cl->planes[k].distance -
		dot(cl->planes[k].normal,
			model->vertexes[cl->crop[k][i].indexes[cl->insides[1]]])
		) / dot(cl->planes[k].normal,
			sub( model->vertexes[cl->crop[k][i].indexes[cl->outsides[0]]],
				model->vertexes[cl->crop[k][i].indexes[cl->insides[1]]]));

	cl->new2 = add(model->vertexes[cl->crop[k][i].indexes[cl->insides[1]]],
		multiply(sub(model->vertexes[cl->crop[k][i].indexes[cl->outsides[0]]],
		model->vertexes[cl->crop[k][i].indexes[cl->insides[1]]]), t));
	
	cl->new_uv2.x = cl->crop[k][i].uvs[cl->outsides[0]].x +
		(cl->crop[k][i].uvs[cl->insides[1]].x -
		cl->crop[k][i].uvs[cl->outsides[0]].x) * (1 - t);
	cl->new_uv2.y = cl->crop[k][i].uvs[cl->outsides[0]].y +
		(cl->crop[k][i].uvs[cl->insides[1]].y -
		cl->crop[k][i].uvs[cl->outsides[0]].y) * (1 - t);
}

t_triangle	get_new_tr1(t_clip_triangle *cl, t_model *model, int k, int i)
{
	t_triangle new_tr1;

	new_tr1 = cl->crop[k][i];

	new_tr1.indexes[0] = cl->crop[k][i].indexes[cl->insides[0]];


	new_tr1.uvs[0] = cl->crop[k][i].uvs[cl->insides[0]];
	new_tr1.indexes[1] = cl->crop[k][i].indexes[cl->insides[1]];
	new_tr1.uvs[1] = cl->crop[k][i].uvs[cl->insides[1]];


	model->vertexes[model->vertexes_count] = cl->new1;
	new_tr1.indexes[2] = model->vertexes_count;
	new_tr1.uvs[2] = cl->new_uv1;

	return (new_tr1);
}

t_triangle	get_new_tr2(t_clip_triangle *cl, t_model *model, int k, int i)
{
	t_triangle new_tr2;

	new_tr2 = cl->crop[k][i];

	new_tr2.indexes[0] = model->vertexes_count;

	new_tr2.uvs[0] = cl->new_uv1;

	model->vertexes_count++;

	new_tr2.indexes[1] = cl->crop[k][i].indexes[cl->insides[1]];
	new_tr2.uvs[1] = cl->crop[k][i].uvs[cl->insides[1]];

	model->vertexes[model->vertexes_count] = cl->new2;
	new_tr2.indexes[2] = model->vertexes_count;
	new_tr2.uvs[2] = cl->new_uv2;

	model->vertexes_count++;
	return (new_tr2);
}

void	clip_1_outside(t_clip_triangle *cl, t_model *model, int k, int i)
{
	get_new1(cl, model, k, i);
	get_new2(cl, model, k, i);

	cl->crop[k + 1][cl->l] = get_new_tr1(cl, model, k, i);;
	cl->l++;

	cl->crop[k + 1][cl->l] = get_new_tr2(cl, model, k, i);;
	cl->l++;
}
