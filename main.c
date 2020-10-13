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
			doom->gamma -= (event->motion.x - doom->prev_x) * 0.5;
			doom->alpha -= (event->motion.y - doom->prev_y) * 0.5;

				//	printf("event: %d gamma: %f\n",event.motion.x, gamma);
		}
		doom->prev_x = event->motion.x;
		doom->prev_y = event->motion.y;
	}
	else if (event->type == SDL_KEYDOWN )
	{
		if (event->key.keysym.sym == SDLK_w)
		{
			doom->w_pressed = 1;
		}
		else if (event->key.keysym.sym == SDLK_s)
		{
			doom->s_pressed = 1;
		}
		else if (event->key.keysym.sym == SDLK_a)
		{
			doom->a_pressed = 1;
		}
		else if (event->key.keysym.sym == SDLK_d)
		{
			doom->d_pressed = 1;
		}
	}
	else if (event->type == SDL_KEYUP)
	{
		if (event->key.keysym.sym == SDLK_w)
		{
			doom->w_pressed = 0;
		}
		else if (event->key.keysym.sym == SDLK_s)
		{
			doom->s_pressed = 0;
		}
		else if (event->key.keysym.sym == SDLK_a)
		{
			doom->a_pressed = 0;
		}
		else if (event->key.keysym.sym == SDLK_d)
		{
			doom->d_pressed = 0;
		}
	}
}

void	update(void *doom_ptr, int *pixels)
{
	t_doom *doom;

	doom = (t_doom *)doom_ptr;
	doom->scene.camera.orientation = multiply_m_m(make_oy_rot_matrix(doom->gamma), 
												make_ox_rot_matrix(doom->alpha));

	bzero(pixels, sizeof(int) * HxW);
	doom->scene.instance.orientation = make_oy_rot_matrix(doom->beta);
	clear_z_buffer(doom->scene.z_buffer);
	render_scene(pixels, &doom->scene);

	if (doom->w_pressed)
	{
		doom->scene.camera.position.z += 0.1 * cos(doom->gamma / 180 * 3.1415);
		doom->scene.camera.position.x -= 0.1 * sin(doom->gamma / 180 * 3.1415);
	}
	if (doom->s_pressed)
	{
		doom->scene.camera.position.z -= 0.1 * cos(doom->gamma / 180 * 3.1415);
		doom->scene.camera.position.x += 0.1 * sin(doom->gamma / 180 * 3.1415);
	}
	if (doom->a_pressed)
	{
		doom->scene.camera.position.z -= 0.1 * sin(doom->gamma / 180 * 3.1415);
		doom->scene.camera.position.x -= 0.1 * cos(doom->gamma / 180 * 3.1415);
	}
	if (doom->d_pressed)
	{
		doom->scene.camera.position.z += 0.1 * sin(doom->gamma / 180 * 3.1415);
		doom->scene.camera.position.x += 0.1 * cos(doom->gamma / 180 * 3.1415);
	}
}



int		main()
{
	
	t_mgl *mgl = mgl_init("Doom_Quaekem", W, H);


	SDL_Surface *tex = create_texture("textures/lol.bmp");

	t_scene scene;
	t_model model;

	model.vertexes = malloc(sizeof(t_vertex) * 30000);
	model.vertexes_count = 0;
	model.triangles = malloc(sizeof(t_triangle) * 30000);
	model.triangles_count = 0;
	model.uvs = malloc(sizeof(t_point) * 30000);
	model.uvs_count = 0;
	model.bounds_center = (t_vertex){0,0,0};
	model.bounds_radius = 100;
	model.new_tex = tex;

	read_obj(&model, "map_editor/map_ed.obj");

	scene.instance.model = &model;
	scene.instance.position = (t_vertex){ 0, 0, 10};
	scene.instance.clipped = malloc(sizeof(t_vertex) * 10);
	scene.instance.scale = 1.0;
	scene.instance.orientation = make_oy_rot_matrix(0.0);
	scene.instance.scale = 1.0;
	scene.instance.position = (t_vertex){0, 0, 0};



	scene.z_buffer = create_z_buffer();
	scene.instances_count = 1;
	scene.camera.orientation = make_oy_rot_matrix(360.0);
	scene.camera.position = (t_vertex){0,0, -2};


	render_init(&scene);




	clipping_planes_init(&scene);

	

	t_doom doom;
	doom.scene = scene;
	doom.beta = 0.0;
	doom.gamma = 0.0;
	doom.alpha = 0.0;

	doom.w_pressed = 0;
	doom.s_pressed = 0;
	doom.a_pressed = 0;
	doom.d_pressed = 0;


	doom.mouse_pressed = 0;
	doom.mouse_right_pressed = 0;

	mgl_run(mgl, update, event_handle, &doom);


	mgl_quit(mgl);
	return (0);
}
