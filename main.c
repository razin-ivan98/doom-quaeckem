#include "duke.h"




int		main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("lol", 50, 50, W, H, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);


	SDL_Surface *surface = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
	SDL_Surface *tex = SDL_CreateRGBSurface(0, 500, 500, 32, 0, 0, 0, 0);

	SDL_Surface *texture = SDL_LoadBMP("lol.bmp");

//	SDL_Surface *tex = SDL_CreateRGBSurface(0, 500, 500, 32, 0, 0, 0, 0);
	t_scene scene;
	t_model model;

	char *ptr1 = (char *)tex->pixels;
	char *ptr2 = (char *)texture->pixels;
	for (int i = 0; i < 500 * 500; i++){
		ptr1[i * 4] = ptr2[i * 3];
		ptr1[i * 4 + 1] = ptr2[i * 3 + 1];
		ptr1[i * 4 + 2] = ptr2[i * 3 + 2];
		ptr1[i * 4 + 3] = 255;

	}

	model.vertexes = malloc(sizeof(t_vertex) * 10);
	model.vertexes_count = 8;
	model.triangles = malloc(sizeof(t_triangle) * 12);
	model.triangles_count = 12;

	model.vertexes[0] = (t_vertex){ 1, 1, 1};
	model.vertexes[1] = (t_vertex){-1, 1, 1};
	model.vertexes[2] = (t_vertex){-1,-1, 1};
	model.vertexes[3] = (t_vertex){ 1,-1, 1};
	model.vertexes[4] = (t_vertex){ 1, 1,-1};
	model.vertexes[5] = (t_vertex){-1, 1,-1};
	model.vertexes[6] = (t_vertex){-1,-1,-1};
	model.vertexes[7] = (t_vertex){ 1,-1,-1};

	model.bounds_center = (t_vertex){0,0,0};
	model.bounds_radius = sqrt(3.0);

	model.triangles[0].indexes[0] = 0;
	model.triangles[0].indexes[1] = 1;
	model.triangles[0].indexes[2] = 2;
	model.triangles[0].normal = (t_vertex){ 0,0,1};

	
	model.triangles[0].texture = tex->pixels;
	model.triangles[0].color = 0xff0000;
	
	model.triangles[0].uvs[0] = (t_point){0,0, 0};
	model.triangles[0].uvs[1] = (t_point){1,0, 0};
	model.triangles[0].uvs[2] = (t_point){1,1, 0};


	model.triangles[1].indexes[0] = 0;
	model.triangles[1].indexes[1] = 2;
	model.triangles[1].indexes[2] = 3;
	model.triangles[1].normal = (t_vertex){ 0,0,1};


	model.triangles[1].texture = tex->pixels;
	model.triangles[1].color = 0xff0000;

	
	model.triangles[1].uvs[0] = (t_point){0,0, 0};
	model.triangles[1].uvs[1] = (t_point){1,1, 0};
	model.triangles[1].uvs[2] = (t_point){0,1, 0};


	model.triangles[2].indexes[0] = 4;
	model.triangles[2].indexes[1] = 0;
	model.triangles[2].indexes[2] = 3;
	model.triangles[2].normal = (t_vertex){ 1,0,0};

	model.triangles[2].texture = tex->pixels;
	model.triangles[2].color = 0x00ff00;

	
	model.triangles[2].uvs[0] = (t_point){0,0, 0};
	model.triangles[2].uvs[1] = (t_point){1,0, 0};
	model.triangles[2].uvs[2] = (t_point){1,1, 0};


	model.triangles[3].indexes[0] = 4;
	model.triangles[3].indexes[1] = 3;
	model.triangles[3].indexes[2] = 7;
	model.triangles[3].normal = (t_vertex){ 1,0,0};

	model.triangles[3].texture = tex->pixels;
	model.triangles[3].color = 0x00ff00;

	
	model.triangles[3].uvs[0] = (t_point){0,0, 0};
	model.triangles[3].uvs[1] = (t_point){1,1, 0};
	model.triangles[3].uvs[2] = (t_point){0,1, 0};


	model.triangles[4].indexes[0] = 5;
	model.triangles[4].indexes[1] = 4;
	model.triangles[4].indexes[2] = 7;
	model.triangles[4].normal = (t_vertex){0,0,-1};

	model.triangles[4].texture = tex->pixels;
	model.triangles[4].color = 0x0000ff;

	
	model.triangles[4].uvs[0] = (t_point){0,0, 0};
	model.triangles[4].uvs[1] = (t_point){1,0, 0};
	model.triangles[4].uvs[2] = (t_point){1,1, 0};


	model.triangles[5].indexes[0] = 5;
	model.triangles[5].indexes[1] = 7;
	model.triangles[5].indexes[2] = 6;
	model.triangles[5].normal = (t_vertex){0,0,-1};

	model.triangles[5].texture = tex->pixels;
	model.triangles[5].color = 0x0000ff;

	
	model.triangles[5].uvs[0] = (t_point){0,0, 0};
	model.triangles[5].uvs[1] = (t_point){1,1, 0};
	model.triangles[5].uvs[2] = (t_point){0,1, 0};





	model.triangles[6].indexes[0] = 1;
	model.triangles[6].indexes[1] = 5;
	model.triangles[6].indexes[2] = 6;
	model.triangles[6].normal = (t_vertex){-1,0,0};

	model.triangles[6].texture = tex->pixels;
	model.triangles[6].color = 0xffff00;


	model.triangles[6].uvs[0] = (t_point){0,0, 0};
	model.triangles[6].uvs[1] = (t_point){1,0, 0};
	model.triangles[6].uvs[2] = (t_point){1,1, 0};


	model.triangles[7].indexes[0] = 1;
	model.triangles[7].indexes[1] = 6;
	model.triangles[7].indexes[2] = 2;
	model.triangles[7].normal = (t_vertex){-1,0,0};

	model.triangles[7].texture = tex->pixels;
	model.triangles[7].color = 0xffff00;

	
	model.triangles[7].uvs[0] = (t_point){0,0, 0};
	model.triangles[7].uvs[1] = (t_point){1,1, 0};
	model.triangles[7].uvs[2] = (t_point){0,1, 0};


	model.triangles[8].indexes[0] = 1;
	model.triangles[8].indexes[1] = 0;
	model.triangles[8].indexes[2] = 5;
	model.triangles[8].normal = (t_vertex){0,1,0};

	model.triangles[8].texture = tex->pixels;
	model.triangles[8].color = 0x00ffff;

	
	model.triangles[8].uvs[0] = (t_point){0,0, 0};
	model.triangles[8].uvs[1] = (t_point){1,0, 0};
	model.triangles[8].uvs[2] = (t_point){1,1, 0};


	model.triangles[9].indexes[0] = 5;
	model.triangles[9].indexes[1] = 0;
	model.triangles[9].indexes[2] = 4;
	model.triangles[9].normal = (t_vertex){0,1,0};

	model.triangles[9].texture = tex->pixels;
	model.triangles[9].color = 0x00ffff;

	
	model.triangles[9].uvs[0] = (t_point){0,1, 0};
	model.triangles[9].uvs[1] = (t_point){1,1, 0};
	model.triangles[9].uvs[2] = (t_point){0,0, 0};


	model.triangles[10].indexes[0] = 2;
	model.triangles[10].indexes[1] = 6;
	model.triangles[10].indexes[2] = 7;
	model.triangles[10].normal = (t_vertex){0,-1,0};

	model.triangles[10].texture = tex->pixels;
	model.triangles[10].color = 0xff00ff;

	
	model.triangles[10].uvs[0] = (t_point){0,0, 0};
	model.triangles[10].uvs[1] = (t_point){1,0, 0};
	model.triangles[10].uvs[2] = (t_point){1,1, 0};


	model.triangles[11].indexes[0] = 2;
	model.triangles[11].indexes[1] = 7;
	model.triangles[11].indexes[2] = 3;
	model.triangles[11].normal = (t_vertex){0,-1,0};

	model.triangles[11].texture = tex->pixels;
	model.triangles[11].color = 0xff00ff;

	
	model.triangles[11].uvs[0] = (t_point){0,0, 0};
	model.triangles[11].uvs[1] = (t_point){1,1, 0};
	model.triangles[11].uvs[2] = (t_point){0,1, 0};



	t_instance instance;
	instance.model = &model;
	instance.position = (t_vertex){ 0, 0, 2};
	//instance.projected = malloc(sizeof(t_point) * 10);
	instance.clipped = malloc(sizeof(t_vertex) * 10);
	instance.scale = 1.0;
	make_oy_rot_matrix(&instance.orientation, 0.0);

	scene.instances = malloc(sizeof(t_instance) * 30);
	scene.instances[0] = instance;
	scene.instances[0].position = (t_vertex){0, 0, 2};
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
	make_oy_rot_matrix(&scene.camera.orientation, 360.0);
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


	scene.render_tr.rendered.vertexes = malloc(sizeof(t_vertex) * 100);
	scene.render_tr.rendered.projected = malloc(sizeof(t_point) * 100);
	scene.render_tr.rendered.triangles = malloc(sizeof(t_triangle) * 100);



	render_scene(surface->pixels, &scene);

	SDL_Texture *text;
	SDL_Rect rect;
	rect.h = H;
	rect.w = W;
	rect.x = 0;
	rect.y = 0;
	//SDL_FreeSurface(bmp);
//	SDL_RenderClear(renderer);
//	SDL_RenderCopy(renderer, text, NULL, &rect);
//	SDL_RenderPresent(renderer);

	int quit = 0;
	

	int mouse_pressed = 0;
	int mouse_right_pressed = 0;
	int prev_x = 0, prev_y = 0;
	float beta = 0;
	float gamma = 0;

	float s2 = sqrt(2.0) / 2.0;
	scene.clipping_planes[0] = (t_plane){(t_vertex){0.0,0.0,1.0}, -1.0}; //near
	scene.clipping_planes[1] = (t_plane){(t_vertex){s2,0.0,s2}, 0.0}; //left
	scene.clipping_planes[2] = (t_plane){(t_vertex){-s2,0.0,s2}, 0.0};//right
	scene.clipping_planes[3] = (t_plane){(t_vertex){0.0,-s2,s2}, 0.0};//top
	scene.clipping_planes[4] = (t_plane){(t_vertex){0.0,s2,s2}, 0.0};//bottom

	// printf("%f %f %f, %f", scene.clipping_planes[3].normal.x,
	// 						scene.clipping_planes[3].normal.y,
	// 						scene.clipping_planes[3].normal.z,
	// 						scene.clipping_planes[3].distance);



	
	float lstTime = 0.0 ;
  	float currTime;
 SDL_Event event;
	// lstTime = 1.0;
	//printf("%d\n", last_time);
	 float fps;
	 int frame = 0;
	while (!quit)
	{
		if (frame == 60)
		{
			currTime = SDL_GetTicks() / 1000.0;

			fps = 1.0 / (currTime - lstTime) * 60.0;
			lstTime = currTime;
			printf("fps: %f\n", fps);
			frame = 0;;
		}
		frame++;	

		while (SDL_PollEvent(&event))
		{
			//SDL_PumpEvents();
			if (event.type == SDL_QUIT)
				quit = 1;
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
					mouse_pressed = 1;
				else if (event.button.button == SDL_BUTTON_RIGHT)
					mouse_right_pressed = 1;
				prev_x = event.button.x;
				prev_y = event.button.y;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				//if (event.button.button == SDL_BUTTON_LEFT)
					mouse_pressed = 0;
					mouse_right_pressed = 0;
	
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				if (mouse_pressed)
				{
					//rect.x += event.motion.x - prev_x;
					//rect.y += event.motion.y - prev_y;
					beta += (event.motion.x - prev_x);
				//	scene.instances[0].position.x += (event.motion.x - prev_x) * 0.05;
					scene.instances[0].position.y -= (event.motion.y - prev_y) * 0.005;
				//	for (int h = 0; h < 27; h++)
				//	printf("event: %d beta: %f\n",event.motion.x, beta);
						
				} else 
				{
					gamma += (event.motion.x - prev_x) * 0.5;
				//	printf("event: %d gamma: %f\n",event.motion.x, gamma);
				}
				prev_x = event.motion.x;
				prev_y = event.motion.y;
			} else if (event.type == SDL_KEYDOWN )
			{
				if (event.key.keysym.sym == SDLK_w)
					scene.camera.position.z += 0.1;
				else if (event.key.keysym.sym == SDLK_s)
					scene.camera.position.z -= 0.1;
				else if (event.key.keysym.sym == SDLK_a)
					scene.camera.position.x -= 0.1;
				else if (event.key.keysym.sym == SDLK_d)
					scene.camera.position.x += 0.1;
			}

		}
		//	for (int h = 0; h < 27; h++)
				make_oy_rot_matrix(&scene.instances[0].orientation, beta);


			
			make_oy_rot_matrix(&scene.camera.orientation, gamma);
			bzero(surface->pixels, sizeof(int) * HxW);
	//		instance.orientation = make_oy_rot_matrix(beta);
			clear_z_buffer(scene.z_buffer);
			render_scene(surface->pixels, &scene);


			SDL_RenderClear(renderer);
			text = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_RenderCopy(renderer, text, NULL, &rect);
			SDL_RenderPresent(renderer);
			SDL_DestroyTexture(text);

		//	curr_tim = time(NULL);
		
		//	printf("fps: %d", fps);

			//quit = 1;
	}





	SDL_DestroyTexture(text);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	return (0);
}
