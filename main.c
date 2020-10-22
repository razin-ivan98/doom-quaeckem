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
		doom->mouse_pressed = 0;
		doom->mouse_right_pressed = 0;
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		if (doom->mouse_pressed)
		{
			doom->gamma -= (event->motion.x - doom->prev_x) * 0.5;
			doom->alpha -= (event->motion.y - doom->prev_y) * 0.5;
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

void	update_sprites(t_doom *doom, float gamma)
{
	int i;
	int	my;
	int him;
	int d;

	t_sprite sprite;

	i = 0;
		
	doom->scene.sprites_count = 0;

	while (i < doom->enemies_count)
	{
		sprite = doom->enemies[i].sprite;

		my = (int)gamma;
		my %= 360;

		if (my < 0)
			my += 360;

		him = (int)(doom->enemies[i].beta);

		him %= 360;

		if (him < 0)
			him += 360;

		d = (my - him);

		if (d < 0)
			d += 360;

		d /= 45;

		sprite.instance.model.anim = &doom->enemies[i].walking_anims[d];

		doom->scene.sprites[i] = sprite;
		doom->scene.sprites_count++;
		i++;
	}
}

void	update_objects(t_doom *doom)
{
	int i;

	i = 0;
	doom->scene.objects_count = 0;
	while (i < doom->objects_count)
	{
		doom->scene.objects[i] = doom->objects[i];
		doom->scene.objects_count++;
		i++;
	}
}

void	update_scene(t_doom *doom, float gamma)
{
	update_sprites(doom, gamma);

	update_objects(doom);



}

void	animation_update(t_scene *scene, float curr_time, float gamma)
{
	int i;
	int frame;

	i = 0;
	while (i < scene->sprites_count)
	{
		if (!scene->sprites[i].instance.model.anim)
		{
			i++;
			continue ;
		}
		
		frame = (int)(scene->sprites[i].instance.model.anim->speed * curr_time)
				% scene->sprites[i].instance.model.anim->length;
		if (frame < 0)
			frame = 0;
		scene->sprites[i].instance.model.new_tex =
				scene->sprites[i].instance.model.anim->frames[frame];
		scene->sprites[i].instance.orientation = make_oy_rot_matrix(gamma);
		i++;
	}
	i = 0;
	while (i < scene->objects_count)
	{
		if (!scene->objects[i].instance.model.anim)
		{
			i++;
			continue ;
		}
		
		frame = (int)(scene->objects[i].instance.model.anim->speed * curr_time)
				% scene->objects[i].instance.model.anim->length;
		if (frame < 0)
			frame = 0;
		scene->objects[i].instance.model.new_tex =
				scene->objects[i].instance.model.anim->frames[frame];
		i++;
	}
}

void	update(void *doom_ptr, int *pixels)
{
	t_doom *doom;

	doom = (t_doom *)doom_ptr;
	doom->scene.camera.orientation = multiply_m_m(make_oy_rot_matrix(doom->gamma), 
												make_ox_rot_matrix(doom->alpha));

	ft_bzero(pixels, sizeof(int) * HxW);

	

	clear_z_buffer(doom->scene.z_buffer);

	update_scene(doom, doom->gamma);
	animation_update(&doom->scene, doom->mgl->curr_time, doom->gamma);

	render_scene(pixels, &doom->scene);

	float speed = 6.0;

	float path = (doom->mgl->curr_time - doom->mgl->lst_time) * speed;

	if (doom->w_pressed)
	{
		doom->scene.camera.position.z += path * cos(doom->gamma / 180 * 3.1415);
		doom->scene.camera.position.x -= path * sin(doom->gamma / 180 * 3.1415);
	}
	if (doom->s_pressed)
	{
		doom->scene.camera.position.z -= path * cos(doom->gamma / 180 * 3.1415);
		doom->scene.camera.position.x += path * sin(doom->gamma / 180 * 3.1415);
	}
	if (doom->a_pressed)
	{
		doom->scene.camera.position.z -= path * sin(doom->gamma / 180 * 3.1415);
		doom->scene.camera.position.x -= path * cos(doom->gamma / 180 * 3.1415);
	}
	if (doom->d_pressed)
	{
		doom->scene.camera.position.z += path * sin(doom->gamma / 180 * 3.1415);
		doom->scene.camera.position.x += path * cos(doom->gamma / 180 * 3.1415);
	}
	
}

t_enemy	create_enemy(t_vertex pos, float beta)
{
	t_enemy enemy;
	t_model enemy_model;

	enemy_model.bounds_center = (t_vertex) {0.0,0.0,0.0};;
	enemy_model.bounds_radius = 2.0;
	enemy_model.triangles_count = 2;
	enemy_model.vertexes_count = 4;

	enemy_model.vertexes = malloc(sizeof(t_vertex) * 4);

	enemy_model.vertexes[0] = (t_vertex) {-1.0,-1.0,0.0};
	enemy_model.vertexes[1] = (t_vertex) {-1.0,1.0,0.0};
	enemy_model.vertexes[2] = (t_vertex) {1.0,1.0,0.0};
	enemy_model.vertexes[3] = (t_vertex) {1.0,-1.0,0.0};

	enemy_model.triangles = malloc(sizeof(t_triangle) * 2);
	enemy_model.triangles[0].indexes[0] = 0;
	enemy_model.triangles[0].indexes[1] = 1;
	enemy_model.triangles[0].indexes[2] = 2;

	enemy_model.triangles[1].indexes[0] = 0;
	enemy_model.triangles[1].indexes[1] = 2;
	enemy_model.triangles[1].indexes[2] = 3;

	enemy_model.triangles[0].normal = (t_vertex) {0.0,0.0,1.0};
	enemy_model.triangles[1].normal = (t_vertex) {0.0,0.0,1.0};

	enemy_model.triangles[0].uvs[0] = (t_point) {0.0, 1.0,0.0};
	enemy_model.triangles[0].uvs[1] = (t_point) {0.0, 0.0,0.0};
	enemy_model.triangles[0].uvs[2] = (t_point) {1.0, 0.0,0.0};

	enemy_model.triangles[1].uvs[0] = (t_point) {0.0, 1.0,0.0};
	enemy_model.triangles[1].uvs[1] = (t_point) {1.0, 0.0,0.0};
	enemy_model.triangles[1].uvs[2] = (t_point) {1.0, 1.0,0.0};


	enemy.sprite.instance.model = enemy_model;
	enemy.sprite.instance.scale = 1.0;
	enemy.sprite.instance.position = pos;
	enemy.sprite.instance.orientation = make_oy_rot_matrix(0.0);

	enemy.beta = beta;

	enemy.walking_anims[0] = load_anim("textures/animm/front/", 4.0);
	enemy.walking_anims[1] = load_anim("textures/animm/front-left/", 4.0);
	enemy.walking_anims[2] = load_anim("textures/animm/left/", 4.0);
	enemy.walking_anims[3] = load_anim("textures/animm/back-left/", 4.0);
	enemy.walking_anims[4] = load_anim("textures/animm/back/", 4.0);
	enemy.walking_anims[5] = load_anim("textures/animm/back-right/", 4.0);
	enemy.walking_anims[6] = load_anim("textures/animm/right/", 4.0);
	enemy.walking_anims[7] = load_anim("textures/animm/front-right/", 4.0);

	enemy.sprite.instance.model.anim = &enemy.walking_anims[0];
	enemy.sprite.instance.model.new_tex = enemy.sprite.instance.model.anim->frames[0];

	return (enemy);
}

t_object	create_object(t_vertex pos)
{
	t_object object;
	t_model object_model;

	object_model.bounds_center = (t_vertex) {0.0,0.0,0.0};;
	object_model.bounds_radius = 3.0;
	object_model.triangles_count = 2;
	object_model.vertexes_count = 4;

	object_model.vertexes = malloc(sizeof(t_vertex) * 4);

	object_model.vertexes[0] = (t_vertex) {-1.1,-0.8,0.0};
	object_model.vertexes[1] = (t_vertex) {-1.1,0.8,0.0};
	object_model.vertexes[2] = (t_vertex) {1.1,0.8,0.0};
	object_model.vertexes[3] = (t_vertex) {1.1,-0.8,0.0};

	object_model.triangles = malloc(sizeof(t_triangle) * 2);
	object_model.triangles[0].indexes[0] = 0;
	object_model.triangles[0].indexes[1] = 1;
	object_model.triangles[0].indexes[2] = 2;

	object_model.triangles[1].indexes[0] = 0;
	object_model.triangles[1].indexes[1] = 2;
	object_model.triangles[1].indexes[2] = 3;

	object_model.triangles[0].normal = (t_vertex) {0.0,0.0,1.0};
	object_model.triangles[1].normal = (t_vertex) {0.0,0.0,1.0};

	object_model.triangles[0].uvs[0] = (t_point) {0.0, 1.0,0.0};
	object_model.triangles[0].uvs[1] = (t_point) {0.0, 0.0,0.0};
	object_model.triangles[0].uvs[2] = (t_point) {1.0, 0.0,0.0};

	object_model.triangles[1].uvs[0] = (t_point) {0.0, 1.0,0.0};
	object_model.triangles[1].uvs[1] = (t_point) {1.0, 0.0,0.0};
	object_model.triangles[1].uvs[2] = (t_point) {1.0, 1.0,0.0};

	t_anim anima = load_anim("textures/anim1/", 15.0);


	object.instance.model = object_model;
	object.instance.scale = 1.0;
	object.instance.position = pos;
	object.instance.orientation = make_oy_rot_matrix(0.0);

	object.anim = anima;
	object.instance.model.anim = &object.anim;

	return (object);
}


int		main()
{
	t_mgl			*mgl;

	t_doom			doom;

	mgl = mgl_init("Doom_Quaekem", W, H);


	// SDL_Surface *cursor_surface = SDL_LoadBMP("textures/cursor.bmp");
	// SDL_Cursor *cursor = SDL_CreateColorCursor(cursor_surface, 0, 0);
	// SDL_SetCursor(cursor);


	doom.mgl = mgl;


	doom.scene.level.instance.model.anim = NULL;
	doom.scene.level.instance.model.new_tex = create_texture("textures/lol.bmp");


	doom.scene.sprites = malloc(sizeof(t_sprite) * 3);
	doom.scene.sprites_count = 0;

	doom.scene.objects = malloc(sizeof(t_object) * 3);
	doom.scene.objects_count = 0;

	doom.enemies = malloc(sizeof(t_enemy) * 3);
	doom.enemies[0] =  create_enemy((t_vertex){0.0,0.0,0.0}, 0.0);
	doom.enemies[1] =  create_enemy((t_vertex){6.0,0.0,6.0}, 0.0);
	doom.enemies[2] =  create_enemy((t_vertex){3.0,0.0,3.0}, 0.0);
	doom.enemies_count = 1;

	doom.objects = malloc(sizeof(t_object) * 3);
	doom.objects[0] = create_object((t_vertex){0.0,0.0,3.2});
	doom.objects_count = 1;


	level_init(&doom.scene);
	render_init(&doom.scene);
	clipping_planes_init(&doom.scene);
	controls_init(&doom);

	mgl_run(mgl, update, event_handle, &doom);

	mgl_quit(mgl);

	return (0);
}
