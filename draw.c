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

	// // normal = triangle_normal(instance->model->vertexes[tr->indexes[0]],
	// // 						instance->model->vertexes[tr->indexes[1]],
	// // 						instance->model->vertexes[tr->indexes[2]]);
	
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

	t_edge_interpolate x = edge_interpolate((t_e_i_input){
												pts[0].y, pts[0].x,
												pts[1].y, pts[1].x,
												pts[2].y, pts[2].x,
											});
	t_edge_interpolate iz = edge_interpolate((t_e_i_input){
												pts[0].y, 1.0 / model->vertexes[ids[0]].z,
												pts[1].y, 1.0 / model->vertexes[ids[1]].z,
												pts[2].y, 1.0 / model->vertexes[ids[2]].z,
											});

	t_edge_interpolate uz = edge_interpolate((t_e_i_input){
												pts[0].y, tr->uvs[0].x / model->vertexes[ids[0]].z,
												pts[1].y, tr->uvs[1].x / model->vertexes[ids[1]].z,
												pts[2].y, tr->uvs[2].x / model->vertexes[ids[2]].z,
											});
										//	printf("ids: %d %d %d", ids[0], ids[1], ids[2]);
										//	printf("uz: %f %f %f", tr->uvs[ids[0]].x, tr->uvs[ids[1]].x, tr->uvs[ids[2]].x);

	t_edge_interpolate vz = edge_interpolate((t_e_i_input){
												pts[0].y, tr->uvs[0].y / model->vertexes[ids[0]].z,
												pts[1].y, tr->uvs[1].y / model->vertexes[ids[1]].z,
												pts[2].y, tr->uvs[2].y / model->vertexes[ids[2]].z,
											});

	int m = (int)(x.v02.length / 2);

	t_float_array *x_left, *x_right, *iz_left, *iz_right,
						*uz_left, *uz_right, *vz_left, *vz_right;

	if (x.v02.array[m] < x.v012.array[m])
	{
		x_left = &x.v02;
		x_right = &x.v012;
		iz_left = &iz.v02;
		iz_right = &iz.v012;
		uz_left = &uz.v02;
		uz_right = &uz.v012;
		vz_left = &vz.v02;
		vz_right = &vz.v012;

	}
	else
	{
		x_left = &x.v012;
		x_right = &x.v02;
		iz_left = &iz.v012;
		iz_right = &iz.v02;
		uz_left = &uz.v012;
		uz_right = &uz.v02;
		vz_left = &vz.v012;
		vz_right = &vz.v02;
	}

	int y_it = (int)pts[0].y;

	//printf("Ytop: %d\t Ybottom: %d\n", y_it, pts[2].y);
	//printf("1: %d\t 2: %d\t 3: %d\n", pts[0].y, pts[1].y, pts[2].y);

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

		// if (uzl < 0.0f){
		// 	printf("error: uzl: %f", uzl);
		// 	exit(-2);
		// }

		t_float_array zscan = interpolate(xl, zl, xr, zr);

		t_float_array uzscan = interpolate(xl, uzl, xr, uzr);
		t_float_array vzscan = interpolate(xl, vzl, xr, vzr);

		x_it = (int)xl;
		while (x_it <= (int)xr)
		{
			float inv_z = zscan.array[(x_it - (int)xl)];
			if (set_z_buffer(scene->z_buffer, x_it, y_it, inv_z))
			{
				float u, v;
				u = ((uzscan.array[(x_it - (int)xl)] / zscan.array[x_it - (int)xl]));
				v = ((vzscan.array[(x_it - (int)xl)] / zscan.array[x_it - (int)xl]));
			//a	printf("%f, %f\n", u, v);
			int color = get_texel(tr->texture, u, v, 500, &uzscan, &vzscan);
			//	 int color = tr->color;
				 put_pixel(image_data, x_it, y_it, color);
			}
			x_it++;
		}
		y_it++;

		free(zscan.array);
		free(uzscan.array);
		free(vzscan.array);
	}
	free(x.v012.array);
	free(x.v02.array);

	free(iz.v012.array);
	free(iz.v02.array);

	free(uz.v012.array);
	free(uz.v02.array);

	free(vz.v012.array);
	free(vz.v02.array);
}


void render_model(int *image_data, t_model *model, t_scene *scene)
{
	int i;

	i = 0;
	while (i < model->vertexes_count)
	{
		model->projected[i] = project_vertex(model->vertexes[i]);
	//	printf("origin x: %f, y: %f, z: %f\n", instance->model->vertexes[i].x, instance->model->vertexes[i].y, instance->model->vertexes[i].z);

		//printf("projected x: %f, y: %f\n", instance->projected[i].x, instance->projected[i].y);
		i++;
	}
	i = 0;
	while (i < model->triangles_count)
	{
		render_triangle(image_data, model, &model->triangles[i], scene);
		i++;
	}
	i = 0;

}

t_model *transform_and_clip(t_instance *instance,t_mat4x4 transform, t_scene *scene)
{
	t_model *model;

	model = malloc(sizeof(t_model));
	model->triangles = malloc(sizeof(t_triangle) * instance->model->triangles_count);/////
	model->vertexes = malloc(sizeof(t_vertex) * instance->model->vertexes_count);/////
	model->projected = malloc(sizeof(t_vertex) * instance->model->vertexes_count);/////
	model->vertexes_count = 0;
	model->triangles_count = 0;

	t_vertex4 tmp = multiply_m_v(transform, (t_vertex4){
									instance->model->bounds_center.x,
									instance->model->bounds_center.y,
									instance->model->bounds_center.z,
									1.0
									});
	t_vertex center = (t_vertex){tmp.x, tmp.y, tmp.z};
	float radius2 = instance->model->bounds_radius;
	
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
	while (i < instance->model->triangles_count)
	{
		model->triangles[i] = instance->model->triangles[i];
		model->triangles_count++;
		i++;
	}
	return (model);
}

void	render_scene(int *image_data, t_scene *scene)
{
	t_mat4x4 camera_mat;
	t_model *model;


	camera_mat = multiply_m_m(transposed_m(scene->camera.orientation), 
		make_translation_matrix(multiply(scene->camera.position, -1.0)));

	int i;
	i = 0;

	while (i < scene->instances_count)
	{
		update_instance_transform(&scene->instances[i]);
		t_mat4x4 transform = multiply_m_m(camera_mat, 
									scene->instances[i].transform);
		///////////////////////////////clipped
	//	for (int j = 0; j < 4; j++){
	//		for (int k = 0; k < 4; k++){
	//			printf("%f ", transform.mat[j][k]);
	//		}
	//		printf("\n");
	//	}
		if (!(model = transform_and_clip(&scene->instances[i], transform, scene))){
			i++;
			continue;
		}
		//printf("%d\n", i);
		render_model(image_data, model, scene);
		free(model->vertexes);
		free(model->triangles);
		free(model->projected);
		free(model);
		i++;
	}
	// for (int i = 0; i < 50; i++){
	// 	for (int j = 0; j < 50; j++){
	// 		put_pixel(image_data, i, j, 0xffff00);
	// 	}
	// }
}

