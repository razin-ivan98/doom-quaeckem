#include "duke.h"


void	sort_vertex_indexes(t_triangle *triangle, t_point *projected)
{
	int		swap;
	t_point	v_swap;
	if (projected[triangle->indexes[1]].y < projected[triangle->indexes[0]].y)
	{
		swap = triangle->indexes[0];
		triangle->indexes[0] = triangle->indexes[1];
		triangle->indexes[1] = swap;

		v_swap = triangle->uvs[0];
		triangle->uvs[0] = triangle->uvs[1];
		triangle->uvs[1] = v_swap;
	}
	if (projected[triangle->indexes[2]].y < projected[triangle->indexes[0]].y)
	{
		swap = triangle->indexes[0];
		triangle->indexes[0] = triangle->indexes[2];
		triangle->indexes[2] = swap;

		v_swap = triangle->uvs[0];
		triangle->uvs[0] = triangle->uvs[2];
		triangle->uvs[2] = v_swap;
	}
	if (projected[triangle->indexes[2]].y < projected[triangle->indexes[1]].y)
	{
		swap = triangle->indexes[1];
		triangle->indexes[1] = triangle->indexes[2];
		triangle->indexes[2] = swap;

		v_swap = triangle->uvs[1];
		triangle->uvs[1] = triangle->uvs[2];
		triangle->uvs[2] = v_swap;
	}
}

void	render_triangle(int *image_data, t_model *model, t_triangle *tr, t_scene *scene)
{
	sort_vertex_indexes(tr, model->projected);

	t_vertex vers[3] = {model->vertexes[tr->indexes[0]],
						model->vertexes[tr->indexes[1]],
						model->vertexes[tr->indexes[2]]};
	t_point pts[3] = {model->projected[tr->indexes[0]],
						model->projected[tr->indexes[1]],
						model->projected[tr->indexes[2]]};
	int ids[3] = {
						tr->indexes[0],
						tr->indexes[1],
						tr->indexes[2]
					};

	edge_interpolate((t_e_i_input){
							pts[0].y, pts[0].x,
							pts[1].y, pts[1].x,
							pts[2].y, pts[2].x,
					}, &scene->render_tr, &scene->render_tr.x);
	edge_interpolate((t_e_i_input){
							pts[0].y, 1.0 / model->vertexes[ids[0]].z,
							pts[1].y, 1.0 / model->vertexes[ids[1]].z,
							pts[2].y, 1.0 / model->vertexes[ids[2]].z,
					}, &scene->render_tr, &scene->render_tr.iz);

	edge_interpolate((t_e_i_input){
							pts[0].y, tr->uvs[0].x / model->vertexes[ids[0]].z,
							pts[1].y, tr->uvs[1].x / model->vertexes[ids[1]].z,
							pts[2].y, tr->uvs[2].x / model->vertexes[ids[2]].z,
					}, &scene->render_tr, &scene->render_tr.uz);

	edge_interpolate((t_e_i_input){
							pts[0].y, tr->uvs[0].y / model->vertexes[ids[0]].z,
							pts[1].y, tr->uvs[1].y / model->vertexes[ids[1]].z,
							pts[2].y, tr->uvs[2].y / model->vertexes[ids[2]].z,
					}, &scene->render_tr, &scene->render_tr.vz);

	int m = (int)(scene->render_tr.x.v02.length / 2);

	t_float_array *x_left, *x_right, *iz_left, *iz_right,
						*uz_left, *uz_right, *vz_left, *vz_right;

	if (scene->render_tr.x.v02.array[m] < scene->render_tr.x.v012.array[m])
	{
		x_left = &scene->render_tr.x.v02;
		x_right = &scene->render_tr.x.v012;
		iz_left = &scene->render_tr.iz.v02;
		iz_right = &scene->render_tr.iz.v012;
		uz_left = &scene->render_tr.uz.v02;
		uz_right = &scene->render_tr.uz.v012;
		vz_left = &scene->render_tr.vz.v02;
		vz_right = &scene->render_tr.vz.v012;

	}
	else
	{
		x_left = &scene->render_tr.x.v012;
		x_right = &scene->render_tr.x.v02;
		iz_left = &scene->render_tr.iz.v012;
		iz_right = &scene->render_tr.iz.v02;
		uz_left = &scene->render_tr.uz.v012;
		uz_right = &scene->render_tr.uz.v02;
		vz_left = &scene->render_tr.vz.v012;
		vz_right = &scene->render_tr.vz.v02;
	}


	int y_it = (int)pts[0].y;

	int x_it;

	while (y_it <= (int)pts[2].y)
	{
		float xl, xr;
		float zl, zr, uzl, uzr, vzl, vzr;

		xl = x_left->array[(y_it - (int)pts[0].y)];
		xr = x_right->array[(y_it - (int)pts[0].y)];
		zl = iz_left->array[(y_it - (int)pts[0].y)];
		zr = iz_right->array[(y_it - (int)pts[0].y)];
		uzl = uz_left->array[(y_it - (int)pts[0].y)];
		uzr = uz_right->array[(y_it - (int)pts[0].y)];
		vzl = vz_left->array[(y_it - (int)pts[0].y)];
		vzr = vz_right->array[(y_it - (int)pts[0].y)];

		float d_z = get_inter_d(xl, zl, xr, zr);

		float d_u = get_inter_d(xl, uzl, xr, uzr);
		float d_v = get_inter_d(xl, vzl, xr, vzr);
		
		x_it = (int)xl;


		float inv_z = zl;
		float u = uzl;
		float v = vzl;
		while (x_it <= (int)xr)
		{
			if (set_z_buffer(scene->z_buffer, x_it, y_it, inv_z))
			{
				int color = get_texel(tr->tex, u / inv_z, v / inv_z);
				//int color = tr->color;
				put_pixel(image_data, x_it, y_it, color);
			}
			x_it++;
			inv_z += d_z;
			u += d_u;
			v += d_v;
		}
		y_it++;
	}
}
