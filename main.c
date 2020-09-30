#include "duke.h"

void	event_handle(SDL_Event *event, void *doom_ptr, int *quit)
{
	t_doom *doom;

	doom = (t_doom *)doom_ptr;
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
			doom->mouse_pressed = 1;
		else if (event->button.button == SDL_BUTTON_RIGHT)
			doom->mouse_right_pressed = 1;
		doom->prev_x = event->button.x;
		doom->prev_y = event->button.y;
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
				//if (event.button.button == SDL_BUTTON_LEFT)
		doom->mouse_pressed = 0;
		doom->mouse_right_pressed = 0;
	
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		// if (doom->mouse_pressed)
		// {
		// 			//rect.x += event.motion.x - prev_x;
		// 			//rect.y += event.motion.y - prev_y;
		// 	doom->beta += (event->motion.x - doom->prev_x);
		// 		//	scene.instances[0].position.x += (event.motion.x - prev_x) * 0.05;
		// 	//doom->scene.instances[0].position.y -= (event->motion.y - doom->prev_y) * 0.005;
		// 		//	for (int h = 0; h < 27; h++)
		// 		//	printf("event: %d beta: %f\n",event.motion.x, beta);
						
		// }
		if (doom->mouse_pressed)
		{
			doom->gamma += (event->motion.x - doom->prev_x) * 0.5;
				//	printf("event: %d gamma: %f\n",event.motion.x, gamma);
		}
		doom->prev_x = event->motion.x;
		doom->prev_y = event->motion.y;
	}
	else if (event->type == SDL_KEYDOWN )
	{
		if (event->key.keysym.sym == SDLK_w)
		{
			doom->scene.camera.position.z += 0.1 * cos(doom->gamma / 180 * 3.1415);
			doom->scene.camera.position.x -= 0.1 * sin(doom->gamma / 180 * 3.1415);
		}
		else if (event->key.keysym.sym == SDLK_s)
		{
			doom->scene.camera.position.z -= 0.1 * cos(doom->gamma / 180 * 3.1415);
			doom->scene.camera.position.x += 0.1 * sin(doom->gamma / 180 * 3.1415);
		}
		else if (event->key.keysym.sym == SDLK_a)
			doom->scene.camera.position.x -= 0.1;
		else if (event->key.keysym.sym == SDLK_d)
			doom->scene.camera.position.x += 0.1;
	}
}

void	update(void *doom_ptr, int *pixels)
{
	t_doom *doom;

	doom = (t_doom *)doom_ptr;
	doom->scene.camera.orientation =	make_oy_rot_matrix(doom->gamma);
	bzero(pixels, sizeof(int) * HxW);
	doom->scene.instances[0].orientation = make_oy_rot_matrix(doom->beta);
	clear_z_buffer(doom->scene.z_buffer);
	render_scene(pixels, &doom->scene);
}



int		main()
{
	
	t_mgl *mgl = mgl_init("Doom_Quaekem", W, H);


	SDL_Surface *tex = SDL_CreateRGBSurface(0, 2048, 2048, 32, 0, 0, 0, 0);

	SDL_Surface *texture = SDL_LoadBMP("brick.bmp");

	
	t_scene scene;
	t_model model;

	char *ptr1 = (char *)tex->pixels;
	char *ptr2 = (char *)texture->pixels;

	int **new_tex;

	new_tex = malloc(sizeof(int *) * 2048);

	char *tmptr;

	for (int a = 0; a < 2048; a++)
	{
		new_tex[a] = malloc(sizeof(int) * 2048);
		for (int u = 0; u < 2048; u++)
		{
			int inds = 2048 * a + u;

			tmptr = (char *)&new_tex[a][u];

			tmptr[0] = ptr2[inds * 3];
			tmptr[1] = ptr2[inds * 3 + 1];
			tmptr[2] = ptr2[inds * 3 + 2];
			tmptr[3] = 255;
		}
	}


	for (int i = 0; i < 2048 * 2048; i++){
		ptr1[i * 4] = ptr2[i * 3];
		ptr1[i * 4 + 1] = ptr2[i * 3 + 1];
		ptr1[i * 4 + 2] = ptr2[i * 3 + 2];
		ptr1[i * 4 + 3] = 255;

	}

	model.vertexes = malloc(sizeof(t_vertex) * 30000);
	model.vertexes_count = 0;
	model.triangles = malloc(sizeof(t_triangle) * 30000);
	model.triangles_count = 0;
	model.uvs = malloc(sizeof(t_point) * 30000);
	model.uvs_count = 0;
	model.bounds_center = (t_vertex){0,0,0};
	model.bounds_radius = 100;
	model.new_tex = tex->pixels;

	read_obj(&model, "map_editor/map_ed.obj");
	
//	read_map("easy.map", &model, new_tex, tex->pixels);

	//printf("Init: %d\n", model.triangles_count);


//	create_box(&model, new_tex, tex->pixels);

	t_instance instance;
	instance.model = &model;
	instance.position = (t_vertex){ 0, 0, 10};
	//instance.projected = malloc(sizeof(t_point) * 10);
	instance.clipped = malloc(sizeof(t_vertex) * 10);
	instance.scale = 1.0;
	instance.orientation = make_oy_rot_matrix(0.0);

	scene.instances = malloc(sizeof(t_instance) * 30);
	scene.instances[0] = instance;
	scene.instances[0].scale = 1.0;
	scene.instances[0].position = (t_vertex){0, 0, 0};
	scene.instances[1] = instance;
	scene.instances[1].position = (t_vertex){0, 0.5, 2};
	scene.instances[2] = instance;
	scene.instances[2].position = (t_vertex){0.5, 0.5, 2};
	scene.instances[3] = instance;
	scene.instances[3].position = (t_vertex){-0.5, 0, 2};
	scene.instances[4] = instance;
	scene.instances[4].position = (t_vertex){0.5, 0, 2};
	scene.instances[5] = instance;
	scene.instances[5].position = (t_vertex){-0.5, -0.5, 2};
	scene.instances[6] = instance;
	scene.instances[6].position = (t_vertex){0, -0.5, 2};
	scene.instances[7] = instance;
	scene.instances[7].position = (t_vertex){0.5, -0.5, 2};
	scene.instances[8] = instance;
	scene.instances[8].position = (t_vertex){0, 0, 2};
//	scene.instances[8].scale = 0.5;

	scene.instances[9] = instance;
	scene.instances[9].position = (t_vertex){-0.5, 0.5, 3};
	scene.instances[10] = instance;
	scene.instances[10].position = (t_vertex){0, 0.5, 3};
	scene.instances[11] = instance;
	scene.instances[11].position = (t_vertex){0.5, 0.5, 3};
	scene.instances[12] = instance;
	scene.instances[12].position = (t_vertex){-0.5, 0, 3};
	scene.instances[13] = instance;
	scene.instances[13].position = (t_vertex){0.5, 0, 3};
	scene.instances[14] = instance;
	scene.instances[14].position = (t_vertex){-0.5, -0.5, 3};
	scene.instances[15] = instance;
	scene.instances[15].position = (t_vertex){0, -0.5, 3};
	scene.instances[16] = instance;
	scene.instances[16].position = (t_vertex){0.5, -0.5, 3};
	scene.instances[17] = instance;
	scene.instances[17].position = (t_vertex){0, 0, 3};

	scene.instances[18] = instance;
	scene.instances[18].position = (t_vertex){-0.5, 0.5, 4};
	scene.instances[19] = instance;
	scene.instances[19].position = (t_vertex){0, 0.5, 4};
	scene.instances[20] = instance;
	scene.instances[20].position = (t_vertex){0.5, 0.5, 4};
	scene.instances[21] = instance;
	scene.instances[21].position = (t_vertex){-0.5, 0, 4};
	scene.instances[22] = instance;
	scene.instances[22].position = (t_vertex){0.5, 0, 4};
	scene.instances[23] = instance;
	scene.instances[23].position = (t_vertex){-0.5, -0.5, -2};
	scene.instances[24] = instance;
	scene.instances[24].position = (t_vertex){0, -0.5, 4};
	scene.instances[25] = instance;
	scene.instances[25].position = (t_vertex){0.5, -0.5, 4};
	scene.instances[26] = instance;
	scene.instances[26].position = (t_vertex){0, 0, 2};


	scene.z_buffer = create_z_buffer();
	scene.instances_count = 1;
	scene.camera.orientation = make_oy_rot_matrix(360.0);
	scene.camera.position = (t_vertex){0,0, -2};


	scene.render_tr.x.v012.array = malloc(sizeof(float) * H * 2);
	scene.render_tr.x.v02.array = malloc(sizeof(float) * H* 2);

	scene.render_tr.iz.v012.array = malloc(sizeof(float) * H* 2);
	scene.render_tr.iz.v02.array = malloc(sizeof(float) * H* 2);

	scene.render_tr.uz.v012.array = malloc(sizeof(float) * H* 2);
	scene.render_tr.uz.v02.array = malloc(sizeof(float) * H* 2);

	scene.render_tr.vz.v012.array = malloc(sizeof(float) * H* 2);
	scene.render_tr.vz.v02.array = malloc(sizeof(float) * H* 2);

	scene.render_tr.zscan.array = malloc(sizeof(float) * W* 2);
	scene.render_tr.uzscan.array = malloc(sizeof(float) * W* 2);
	scene.render_tr.vzscan.array = malloc(sizeof(float) * W* 2);

	scene.render_tr.v02.array = malloc(sizeof(float) * H* 2);
	scene.render_tr.v01.array = malloc(sizeof(float) * H* 2);
	scene.render_tr.v12.array = malloc(sizeof(float) * H* 2);


	scene.render_tr.rendered.vertexes = malloc(sizeof(t_vertex) * 30000);
	scene.render_tr.rendered.projected = malloc(sizeof(t_point) * 30000);
	scene.render_tr.rendered.triangles = malloc(sizeof(t_triangle) * 30000);

	scene.bsp_model = create_bsp(&model);
//если это включить получается сегфолт
	int quit = 0;
	

	int mouse_pressed = 0;
	int mouse_right_pressed = 0;
	int prev_x = 0, prev_y = 0;
	float beta = 0;
	float gamma = 0;

	float s2 = sqrt(2.0) / 2.0;

	float sins = 1 / sqrt(5);
	float coss = sins * 2;
	scene.clipping_planes[0] = (t_plane){(t_vertex){0.0,0.0,1.0}, /*-1.0*/0.0}; //near
	scene.clipping_planes[1] = (t_plane){(t_vertex){coss,0.0,sins}, 0.0}; //left
	scene.clipping_planes[2] = (t_plane){(t_vertex){-coss,0.0,sins}, 0.0};//right
	scene.clipping_planes[3] = (t_plane){(t_vertex){0.0,-coss,sins}, 0.0};//top
	scene.clipping_planes[4] = (t_plane){(t_vertex){0.0,coss,sins}, 0.0};//bottom

	

	t_doom doom;
	doom.scene = scene;
	doom.beta = 0.0;
	doom.gamma = 0.0;
	doom.mouse_pressed = 0;
	doom.mouse_right_pressed = 0;

	mgl_run(mgl, update, event_handle, &doom);


	mgl_quit(mgl);
	return (0);
}
