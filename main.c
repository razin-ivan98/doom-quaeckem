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

void	update(void *doom_ptr, int *pixels)
{
	t_doom *doom;

	doom = (t_doom *)doom_ptr;
	doom->scene.camera.orientation = multiply_m_m(make_oy_rot_matrix(doom->gamma), 
												make_ox_rot_matrix(doom->alpha));

	ft_bzero(pixels, sizeof(int) * HxW);
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
	t_mgl			*mgl;
	SDL_Surface		*tex;
	t_doom			doom;

	mgl = mgl_init("Doom_Quaekem", W, H);
	tex = create_texture("textures/lol.bmp");

	doom.scene.instance.model.new_tex = tex;

	level_init(&doom.scene);
	render_init(&doom.scene);
	clipping_planes_init(&doom.scene);
	controls_init(&doom);

	mgl_run(mgl, update, event_handle, &doom);

	mgl_quit(mgl);

	return (0);
}
