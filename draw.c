#include "duke.h"

// void	draw_line(int *image_data, t_point *p0, t_point *p1, int color)
// {
// 	float			dx;
// 	float			dy;
// 	t_float_array	arr;
// 	float			swap;

// 	t_point			*ptr0;
// 	t_point			*ptr1;

// 	int 			i;

// 	i = 0;
// 	ptr0 = p0;
// 	ptr1 = p1;


// 	dx = p1->x - p0->x;
// 	dy = p1->y - p0->y;
// 	if (abs(dx) > abs(dy)) {
// 		if (dx < 0)
// 		{
// 			ptr0 = p1;
// 			ptr1 = p0; 
// 		}

// 		arr = interpolate(p0->x, (float)p0->y, p1->x, (float)p1->y);
// 		i = p0->x;
// 		while (i <= p1->x) {
// 			put_pixel(image_data, i, arr.array[i - p0->x], color);
// 			i++;
// 		}
// 	} else {
// 		if (dy < 0)
// 		{
// 			ptr0 = p1;
// 			ptr1 = p0;
// 		}
// 		arr = interpolate(p0->y, (float)p0->x, p1->y, (float)p1->x);
// 		i = p0->y;
// 		while (i <= p1->y) {
// 			put_pixel(image_data, arr.array[i - p0->y], i, color);
// 			i++;
// 		}
// 	}
// }

void	sort_vertex_indexes(t_triangle *triangle, t_point *projected)
{
	int swap;
	t_point v_swap;
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
	t_vertex normal;
	t_vertex centre;

	// normal = triangle_normal(instance->model->vertexes[tr->indexes[0]],
	// 						instance->model->vertexes[tr->indexes[1]],
	// 						instance->model->vertexes[tr->indexes[2]]);
	
	// centre = multiply(add(add(instance->model->vertexes[tr->indexes[0]],
	// 				instance->model->vertexes[tr->indexes[1]]),
	// 				instance->model->vertexes[tr->indexes[2]]), -1.0f / 3.0f);
	// //printf("%f\n", dot(centre, normal));
	// if (dot(centre, normal) < 0)
    // 	return ;

	


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
		// float x_left, x_right, iz_left, iz_right,
		// 				uz_left, uz_right, vz_left, vz_right;
		// float x_l_d, x_r_d, z_l_d, z_r_d, u_l_d, u_r_d, v_l_d, v_r_d;

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

			int color = get_texel_n(tr->new_tex, u / inv_z, v / inv_z, 500);

				//   int color = tr->color;
			//	unsigned char *ttt = (unsigned char *)&color;



				// float intensity = (100.0 - 1.0 / inv_z) / 100.0;//(inv_z < 1.0 ? 1.0 - inv_z  : 0.0);
				// if (intensity < 0.0)
				// 	intensity = 0.0;


				// ttt[0] = (unsigned char)(ttt[0] * intensity);
				// ttt[1] = (unsigned char)(ttt[1] * intensity);
				// ttt[2] = (unsigned char)(ttt[2] * intensity);



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


void render_model(int *image_data, t_model *model, t_scene *scene)
{
	int i;

	i = 0;

	// for (int y = 0; y < model->triangles_count; y++)
	// {
	// 	if (model->triangles[y].indexes[0] > 1000 || model->triangles[y].indexes[0] < 0 ||
	// 		model->triangles[y].indexes[1] > 1000 || model->triangles[y].indexes[1] < 0 ||
	// 		model->triangles[y].indexes[2] > 1000 || model->triangles[y].indexes[2] < 0)
	// 		printf("renderModelPreALARM %d\n", y);
	// }
	while (i < model->vertexes_count)
	{
		model->projected[i] = project_vertex(model->vertexes[i]);

		i++;
	}
	i = 0;
	// for (int y = 0; y < model->triangles_count; y++)
	// {
	// 	if (model->triangles[y].indexes[0] > 1000 || model->triangles[y].indexes[0] < 0 ||
	// 		model->triangles[y].indexes[1] > 1000 || model->triangles[y].indexes[1] < 0 ||
	// 		model->triangles[y].indexes[2] > 1000 || model->triangles[y].indexes[2] < 0)
	// 		printf("renderModelProjectedALARM %d\n", y);
	// }
	while (i < model->triangles_count)
	{
		// for (int y = 0; y < model->triangles_count; y++)
		// {
		// 	if (model->triangles[i].indexes[0] > 1000 || model->triangles[i].indexes[0] < 0 ||
		// 	model->triangles[i].indexes[1] > 1000 || model->triangles[i].indexes[1] < 0 ||
		// 	model->triangles[i].indexes[2] > 1000 || model->triangles[i].indexes[2] < 0)
		// 		printf("runtimeALARM %d %d\n", y, i);
		// }

		// if (model->triangles[i].indexes[0] > 1000 || model->triangles[i].indexes[0] < 0 ||
		// 	model->triangles[i].indexes[1] > 1000 || model->triangles[i].indexes[1] < 0 ||
		// 	model->triangles[i].indexes[2] > 1000 || model->triangles[i].indexes[2] < 0)
		// {
		// 	printf("1ssanyo %d %d\n", i, model->triangles_count);
		// 	printf("%f", model->vertexes[model->triangles[i].indexes[0]]);
		// }
				
		render_triangle(image_data, model, &model->triangles[i], scene);
		i++;
	}
	i = 0;

}

int		clip_triangle(t_triangle *trs, int *count, t_plane *planes, t_model *model)
{
	int k;
	int i;
	int j;
	int flag = 1;
	int outside_count;
	int inside_count;
	int	outsides[3];
	int insides[3];
	t_triangle *curr;
	t_triangle crop[6][40];

	int l;
	crop[0][0] = trs[0];
	int l_prev = 1;
	l = 1;

	//bzero(mask, 40);///////////////
	
	float distances[5][3];
	k = 0;
	while (k < 5)
	{	
		i = 0;
		l = 0;
		while (i < l_prev)
		{
			// distances[k][0] = (dot(planes[k].normal, model->vertexes[trs[i].indexes[0]]) + planes[k].distance > 0);
			// distances[k][1] = (dot(planes[k].normal, model->vertexes[trs[i].indexes[1]]) + planes[k].distance > 0);
			// distances[k][2] = (dot(planes[k].normal, model->vertexes[trs[i].indexes[2]]) + planes[k].distance > 0);
			outside_count = 0;
			inside_count = 0;
			if (dot(planes[k].normal, model->vertexes[crop[k][i].indexes[0]]) + planes[k].distance < 0)
			{
				outsides[outside_count] = 0;
				outside_count++;
			}
			else
			{
				insides[inside_count] = 0;
				inside_count++;
			}
			
			if (dot(planes[k].normal, model->vertexes[crop[k][i].indexes[1]]) + planes[k].distance < 0)
			{
				outsides[outside_count] = 1;
				outside_count++;
			}
			else
			{
				insides[inside_count] = 1;
				inside_count++;
			}
			
			if (dot(planes[k].normal, model->vertexes[crop[k][i].indexes[2]]) + planes[k].distance < 0)
			{
				outsides[outside_count] = 2;
				outside_count++;
			}
			else
			{
				insides[inside_count] = 2;
				inside_count++;
			}
			if (outside_count > 2)
			{
				i++;
				continue;
			}
			else if (outside_count == 0)
			{
				crop[k + 1][l] = crop[k][i];
				l++;
			}
			else if (outside_count == 2)
			{
				t_vertex new1;
				t_vertex new2;

				float t;

				t = (-planes[k].distance - dot(planes[k].normal, model->vertexes[crop[k][i].indexes[insides[0]]])) /
											dot(planes[k].normal, sub( model->vertexes[crop[k][i].indexes[outsides[0]]], model->vertexes[crop[k][i].indexes[insides[0]]]));
			//printf("%f / %f = %f\n",dot(planes[k].normal, model->vertexes[crop[k][i].indexes[insides[0]]]), dot(planes[k].normal, sub( model->vertexes[crop[k][i].indexes[outsides[0]]], model->vertexes[crop[k][i].indexes[insides[0]]])), t);
				new1 = add(model->vertexes[crop[k][i].indexes[insides[0]]], multiply(sub(model->vertexes[crop[k][i].indexes[outsides[0]]], model->vertexes[crop[k][i].indexes[insides[0]]]), t));
				
				
				model->vertexes[model->vertexes_count] = new1;
				crop[k][i].indexes[outsides[0]] = model->vertexes_count;
				
				crop[k][i].uvs[outsides[0]].x = crop[k][i].uvs[outsides[0]].x + (crop[k][i].uvs[insides[0]].x - crop[k][i].uvs[outsides[0]].x) * (1 - t);
				crop[k][i].uvs[outsides[0]].y = crop[k][i].uvs[outsides[0]].y + (crop[k][i].uvs[insides[0]].y - crop[k][i].uvs[outsides[0]].y) * (1 - t);

				model->vertexes_count++;


		//		if (crop[k][i].indexes[0] > 100 || crop[k][i].indexes[0] < 0)
		//			printf("1ssanyo\n");


				t = (-planes[k].distance - dot(planes[k].normal, model->vertexes[crop[k][i].indexes[insides[0]]])) /
											dot(planes[k].normal, sub( model->vertexes[crop[k][i].indexes[outsides[1]]], model->vertexes[crop[k][i].indexes[insides[0]]]));

				new2 = add(model->vertexes[crop[k][i].indexes[insides[0]]], multiply(sub(model->vertexes[crop[k][i].indexes[outsides[1]]], model->vertexes[crop[k][i].indexes[insides[0]]]), t));


				model->vertexes[model->vertexes_count] = new2;
				crop[k][i].indexes[outsides[1]] = model->vertexes_count;
				crop[k][i].uvs[outsides[1]].x = crop[k][i].uvs[outsides[1]].x + (crop[k][i].uvs[insides[0]].x - crop[k][i].uvs[outsides[1]].x) * (1 - t);
				crop[k][i].uvs[outsides[1]].y = crop[k][i].uvs[outsides[1]].y + (crop[k][i].uvs[insides[0]].y - crop[k][i].uvs[outsides[1]].y) * (1 - t);
				model->vertexes_count++;
				crop[k][l].color += 100;
				crop[k + 1][l] = (crop[k][i]);
			//	if (crop[k + 1][l].indexes[0] > 2024)
		//			printf("ssanyo\n");
				l++;
				
			}
			else if (outside_count == 1)
			{
				t_vertex new1;
				t_vertex new2;
				t_point new_uv1;
				t_point new_uv2;


				float t;

				t = (-planes[k].distance - dot(planes[k].normal, model->vertexes[crop[k][i].indexes[insides[0]]])) /
											dot(planes[k].normal, sub( model->vertexes[crop[k][i].indexes[outsides[0]]], model->vertexes[crop[k][i].indexes[insides[0]]]));
			//printf("%f / %f = %f\n",dot(planes[k].normal, model->vertexes[crop[k][i].indexes[insides[0]]]), dot(planes[k].normal, sub( model->vertexes[crop[k][i].indexes[outsides[0]]], model->vertexes[crop[k][i].indexes[insides[0]]])), t);
				new1 = add(model->vertexes[crop[k][i].indexes[insides[0]]], multiply(sub(model->vertexes[crop[k][i].indexes[outsides[0]]], model->vertexes[crop[k][i].indexes[insides[0]]]), t));
				
				
			//	model->vertexes[model->vertexes_count] = new1;
			//	crop[k][i].indexes[outsides[0]] = model->vertexes_count;
				new_uv1.x = crop[k][i].uvs[outsides[0]].x + (crop[k][i].uvs[insides[0]].x - crop[k][i].uvs[outsides[0]].x) * (1 - t);
				new_uv1.y = crop[k][i].uvs[outsides[0]].y + (crop[k][i].uvs[insides[0]].y - crop[k][i].uvs[outsides[0]].y) * (1 - t);

			//	model->vertexes_count++;


				t = (-planes[k].distance - dot(planes[k].normal, model->vertexes[crop[k][i].indexes[insides[1]]])) /
											dot(planes[k].normal, sub( model->vertexes[crop[k][i].indexes[outsides[0]]], model->vertexes[crop[k][i].indexes[insides[1]]]));

				new2 = add(model->vertexes[crop[k][i].indexes[insides[1]]], multiply(sub(model->vertexes[crop[k][i].indexes[outsides[0]]], model->vertexes[crop[k][i].indexes[insides[1]]]), t));


				//model->vertexes[model->vertexes_count] = new2;
			////	crop[k][i].indexes[outsides[1]] = model->vertexes_count;
				new_uv2.x = crop[k][i].uvs[outsides[0]].x + (crop[k][i].uvs[insides[1]].x - crop[k][i].uvs[outsides[0]].x) * (1 - t);
				new_uv2.y = crop[k][i].uvs[outsides[0]].y + (crop[k][i].uvs[insides[1]].y - crop[k][i].uvs[outsides[0]].y) * (1 - t);
			//	model->vertexes_count++;


				t_triangle new_tr1 = crop[k][i];
				t_triangle new_tr2 = crop[k][i];;

				new_tr2.color += 100;
				new_tr1.color += 300;



				new_tr1.indexes[0] = crop[k][i].indexes[insides[0]];

		//		if (new_tr1.indexes[0] > 100 || new_tr1.indexes[0] < 0)
		//			printf("2ssanyo\n");


				new_tr1.uvs[0] = crop[k][i].uvs[insides[0]];
				new_tr1.indexes[1] = crop[k][i].indexes[insides[1]];
				new_tr1.uvs[1] = crop[k][i].uvs[insides[1]];


				model->vertexes[model->vertexes_count] = new1;
				new_tr1.indexes[2] = model->vertexes_count;
				new_tr1.uvs[2] = new_uv1;


				new_tr2.indexes[0] = model->vertexes_count;
		//		if (new_tr2.indexes[0] > 100 || new_tr2.indexes[0] < 0)
		//			printf("3ssanyo\n");
				new_tr2.uvs[0] = new_uv1;

				model->vertexes_count++;

				new_tr2.indexes[1] = crop[k][i].indexes[insides[1]];
				new_tr2.uvs[1] = crop[k][i].uvs[insides[1]];

				model->vertexes[model->vertexes_count] = new2;
				new_tr2.indexes[2] = model->vertexes_count;
				new_tr2.uvs[2] = new_uv2;


				model->vertexes_count++;


				crop[k + 1][l] = new_tr1;
				l++;

				crop[k + 1][l] = new_tr2;
				l++;
			}

			

			i++;
		}
		l_prev = l;
		
		k++;
	}
	// model->triangles_count = 0;
	for (int r = 0; r < l_prev; r++)
	{
		model->triangles[model->triangles_count] = crop[5][r];
		model->triangles[model->triangles_count].color = r;
	//	if (model->triangles[model->triangles_count].indexes[0] > 100 || model->triangles[model->triangles_count].indexes[0] < 0)
	//				printf("1s23sanyo\n");
		model->triangles_count++;
	}
		
}

t_model *transform_and_clip(t_instance *instance,t_mat4x4 transform, t_scene *scene)
{
	t_model *model = &scene->render_tr.rendered;

//	model = (t_model *)malloc(sizeof(t_model));
//	model->vertexes = malloc(sizeof(t_vertex) * 1000);
//	model->projected = malloc(sizeof(t_point) * 1000);
//	model->triangles = malloc(sizeof(t_triangle) * 1000);

//	model = malloc(sizeof(t_model));
//	model->triangles = malloc(sizeof(t_triangle) * instance->model->triangles_count);/////
//	model->vertexes = malloc(sizeof(t_vertex) * instance->model->vertexes_count);/////
	//model->projected = malloc(sizeof(t_vertex) * instance->model->vertexes_count);/////

	model->vertexes_count = 0;
	model->triangles_count = 0;
	model->uvs_count = 0;

	t_vertex4 tmp;
	tmp = multiply_m_v(transform, (t_vertex4){
									instance->model->bounds_center.x,
									instance->model->bounds_center.y,
									instance->model->bounds_center.z,
									1.0
									});
	t_vertex center = (t_vertex){tmp.x, tmp.y, tmp.z};
	float radius2 = instance->model->bounds_radius * instance->scale;
	
	int i;

	i = 0;
	while (i < 5)
	{
		float distance2 = dot(scene->clipping_planes[i].normal, center) + scene->clipping_planes[i].distance;
		if (distance2 < -radius2)
			return (NULL);
		i++;
	}

	i = 0;
	while (i < instance->model->vertexes_count)
	{
		tmp = (t_vertex4){
			instance->model->vertexes[i].x,
			instance->model->vertexes[i].y,
			instance->model->vertexes[i].z,
			1.0
		};
		tmp = multiply_m_v(transform, tmp);
	//	printf("tmp: x: %f, y: %f, z: %f\n", tmp.x, tmp.y, tmp.z);
		//instance->clipped[i] = (t_vertex){tmp.x, tmp.y, tmp.z};
		model->vertexes[i] = (t_vertex){tmp.x, tmp.y, tmp.z};
		model->vertexes_count++;
		i++;
	}
	i = 0;
//	int distances[5][3];
	t_triangle curr;
	while (i < instance->model->triangles_count)
	{
		curr = instance->model->triangles[i];
		


	// 	t_vertex tt;
	// 	add(&tt, &model->vertexes[curr.indexes[0]], &model->vertexes[curr.indexes[1]]);
	// 	add(&tt, &model->vertexes[curr.indexes[2]], &tt);
	// 	multiply(&tt, &tt, -1.0 / 3.0);
	// 	t_vertex4 tp = (t_vertex4){
	// 		curr.normal.x,
	// 		curr.normal.y,
	// 		curr.normal.z,
	// 		1,
	// 	};
	// //	printf("%f %f %f\n", curr.normal.x, curr.normal.y,curr.normal.z);
	// 	multiply_m_v(&tp, &instance->orientation, &tp);
	// 	curr.normal = (t_vertex){tp.x, tp.y, tp.z};
	//	printf("%f %f %f\n", curr.normal.x, curr.normal.y,curr.normal.z);
		int k = 0;
	//	int flag = 1;
		
		// if (dot(&tt, &curr.normal) < 0)
		// 	flag = 0;
		//t_triangle ters[40];
		int ters_count = 1;
		//ters[0] = curr;

		//while (k < 5)
		//{
		clip_triangle(&curr, &ters_count, scene->clipping_planes, model);

	//	model->triangles[model->triangles_count] = instance->model->triangles[i];;
	//	model->triangles_count++;
		/*if (model->triangles[i].indexes[0] > 1000 || model->triangles[i].indexes[0] < 0 ||
			model->triangles[i].indexes[1] > 1000 || model->triangles[i].indexes[1] < 0 ||
			model->triangles[i].indexes[2] > 1000 || model->triangles[i].indexes[2] < 0)
			printf("TransformClipALARMMMM\n");*/
		//	k++;
		//}
	//	if (flag)
	//	{
		//	model->triangles[model->triangles_count] = curr;
		//	model->triangles_count++;
		//	printf("O");
			// i++;
			// continue ;
	//	}
		// k = 0;
		
		// t_triangle triangles_stack[20];
		// int stack_count = 1;
		// triangles_stack[0] = instance->model->triangles[i];
		// while (k < 5)
		// {
		// 	if (distances[k][0] && distances[k][1] && distances[k][2])
		// 	{
		// 		k++;
		// 		continue ;
		// 	}
		// 	int count = 0;
		// 	int outside_indexes[2];
		// 	for (int g = 0; g < 3; g++)
		// 	{
		// 		if (!distances[k][g])
		// 		{
		// 			outside_indexes[count] = g;
		// 			count++;
		// 		}
		// 	}
			
		// 	if (count == 1)
		// 	{

		// 	} else if (count == 2)
		// 	{

		// 	}


		// 	k++;
		// }

		i++;
	}
//	printf("\n");
	return (model);
}

void	render_scene(int *image_data, t_scene *scene)
{
	t_mat4x4 camera_mat;
	t_mat4x4 translation;
	t_mat4x4 transposed;
	t_vertex tmp;
	t_model *model;


	camera_mat = multiply_m_m(transposed_m(scene->camera.orientation), 
		make_translation_matrix(multiply(scene->camera.position, -1.0)));

	int i;
	i = 0;

	while (i < scene->instances_count)
	{
		update_instance_transform(&scene->instances[i]);
		t_mat4x4 transform;
		transform = multiply_m_m(camera_mat, 
									scene->instances[i].transform);
		///////////////////////////////clipped
	//	for (int j = 0; j < 4; j++){
	//		for (int k = 0; k < 4; k++){
	//			printf("%f ", transform.mat[j][k]);
	//		}
	//		printf("\n");
	//	}
//	printf("ok\n");
		if (!(model = transform_and_clip(&scene->instances[i], transform, scene))){
			i++;
			continue;
		}
//		printf("%d\n", i);
		

		// for (int y = 0; y < model->triangles_count; y++)
		// {
		// 	if (model->triangles[y].indexes[0] > 1000 || model->triangles[y].indexes[0] < 0 ||
		// 	model->triangles[y].indexes[1] > 1000 || model->triangles[y].indexes[1] < 0 ||
		// 	model->triangles[y].indexes[2] > 1000 || model->triangles[y].indexes[2] < 0)
		// 		printf("RenderSceneALARMMMM\n");
		// }

		render_model(image_data, model, scene);
	//	free(model->vertexes);
	//	free(model->triangles);
	//	free(model->projected);
	//	free(model);
		i++;
	}
	// for (int i = 0; i < 50; i++){
	// 	for (int j = 0; j < 50; j++){
	// 		put_pixel(image_data, i, j, 0xffff00);
	// 	}
	// }
}

