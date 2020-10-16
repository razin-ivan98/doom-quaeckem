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

void	animation_update(t_scene *scene, float curr_time, float gamma)
{
	int my = (int)gamma;
	my %= 360;

	if (my < 0)
		my += 360;

	int him = (int)(scene->enemy.beta);

	him %= 360;

	if (him < 0)
		him += 360;

	int d = (my - him);

	if (d < 0)
		d += 360;

	d /= 45;

	scene->enemy.sprite.instance.model.anim = &scene->enemy.walking_anims[d];

	if (!scene->enemy.sprite.instance.model.anim)
		return ;
	int frame = (int)(scene->enemy.sprite.instance.model.anim->speed * curr_time)
			% scene->enemy.sprite.instance.model.anim->length;
	if (frame < 0)
		frame = 0;
	scene->enemy.sprite.instance.model.new_tex =
			scene->enemy.sprite.instance.model.anim->frames[frame];
}

void	update(void *doom_ptr, int *pixels)
{
	t_doom *doom;

	doom = (t_doom *)doom_ptr;
	doom->scene.camera.orientation = multiply_m_m(make_oy_rot_matrix(doom->gamma), 
												make_ox_rot_matrix(doom->alpha));

	ft_bzero(pixels, sizeof(int) * HxW);
	// doom->scene.level.instance.orientation = make_oy_rot_matrix(doom->beta);
	doom->scene.enemy.sprite.instance.orientation = make_oy_rot_matrix(doom->gamma);

	clear_z_buffer(doom->scene.z_buffer);

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

t_enemy	create_enemy()
{
	t_enemy enemy;
	t_model enemy_model;

	enemy_model.bounds_center = (t_vertex) {0.0,0.0,0.0};
	enemy_model.bounds_radius = 2.0;
	enemy_model.triangles_count = 2;
	enemy_model.vertexes_count = 4;

	enemy_model.vertexes = malloc(sizeof(t_vertex) * 4);

	enemy_model.vertexes[0] = (t_vertex) {0.0,0.0,0.0};
	enemy_model.vertexes[1] = (t_vertex) {0.0,2.0,0.0};
	enemy_model.vertexes[2] = (t_vertex) {2.0,2.0,0.0};
	enemy_model.vertexes[3] = (t_vertex) {2.0,0.0,0.0};

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

	//t_anim anima = load_anim("textures/animm/front/", 4.0);


	enemy.sprite.instance.model = enemy_model;
	enemy.sprite.instance.scale = 1.0;
	enemy.sprite.instance.position = (t_vertex) {0.0, -1.0, 0.0};
	enemy.sprite.instance.orientation = make_oy_rot_matrix(0.0);

	enemy.beta = 0.0;

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

int		main()
{
	t_mgl			*mgl;

	t_doom			doom;

	mgl = mgl_init("Doom_Quaekem", W, H);

	doom.mgl = mgl;

	t_anim anim;

	anim = load_anim("textures/animm/left/", 4.0);

	doom.scene.level.instance.model.anim = NULL;//&anim;
	doom.scene.level.instance.model.new_tex = create_texture("textures/lol.bmp");

	t_enemy enemy = create_enemy();

	doom.scene.enemy = enemy;

	level_init(&doom.scene);
	render_init(&doom.scene);
	clipping_planes_init(&doom.scene);
	controls_init(&doom);

	mgl_run(mgl, update, event_handle, &doom);

	mgl_quit(mgl);

	return (0);
}
