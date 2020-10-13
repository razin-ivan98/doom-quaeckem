#include "duke.h"

static void	get_new1(t_clip_triangle *cl, t_model *model, int k, int i)
{
	float t;

	t = (-cl->planes[k].distance -
		dot(cl->planes[k].normal,
			model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]])
		) / dot(cl->planes[k].normal,
			sub( model->vertexes[cl->crop[k][i].indexes[cl->outsides[0]]],
				model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]]));

	cl->new1 = add(model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]],
		multiply(sub(model->vertexes[cl->crop[k][i].indexes[cl->outsides[0]]],
		model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]]), t));
	
	model->vertexes[model->vertexes_count] = cl->new1;
	cl->crop[k][i].indexes[cl->outsides[0]] = model->vertexes_count;		
	cl->crop[k][i].uvs[cl->outsides[0]].x =
		cl->crop[k][i].uvs[cl->outsides[0]].x +
		(cl->crop[k][i].uvs[cl->insides[0]].x -
		cl->crop[k][i].uvs[cl->outsides[0]].x) * (1 - t);
	cl->crop[k][i].uvs[cl->outsides[0]].y =
		cl->crop[k][i].uvs[cl->outsides[0]].y +
		(cl->crop[k][i].uvs[cl->insides[0]].y -
		cl->crop[k][i].uvs[cl->outsides[0]].y) * (1 - t);
	model->vertexes_count++;
}

static void	get_new2(t_clip_triangle *cl, t_model *model, int k, int i)
{
	float t;

	t = (-cl->planes[k].distance -
		dot(cl->planes[k].normal,
			model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]])
		) / dot(cl->planes[k].normal,
			sub( model->vertexes[cl->crop[k][i].indexes[cl->outsides[1]]],
				model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]]));

	cl->new2 = add(model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]],
		multiply(sub(model->vertexes[cl->crop[k][i].indexes[cl->outsides[1]]],
			model->vertexes[cl->crop[k][i].indexes[cl->insides[0]]]), t));
	
	model->vertexes[model->vertexes_count] = cl->new2;
	cl->crop[k][i].indexes[cl->outsides[1]] = model->vertexes_count;
	cl->crop[k][i].uvs[cl->outsides[1]].x =
		cl->crop[k][i].uvs[cl->outsides[1]].x +
		(cl->crop[k][i].uvs[cl->insides[0]].x -
		cl->crop[k][i].uvs[cl->outsides[1]].x) * (1 - t);
	cl->crop[k][i].uvs[cl->outsides[1]].y =
		cl->crop[k][i].uvs[cl->outsides[1]].y +
		(cl->crop[k][i].uvs[cl->insides[0]].y -
		cl->crop[k][i].uvs[cl->outsides[1]].y) * (1 - t);
	model->vertexes_count++;
}

void	clip_2_outsides(t_clip_triangle *cl, t_model *model, int k, int i)
{
	get_new1(cl, model, k, i);
	get_new2(cl, model, k, i);
	
	cl->crop[k + 1][cl->l] = (cl->crop[k][i]);
	cl->l++;			
}