/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_graphics_lib.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 00:22:38 by razin-ivan9       #+#    #+#             */
/*   Updated: 2021/02/04 15:01:23 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_graphics_lib.h"

t_mgl		mgl_init(char *win_name, int w, int h, int multiplicator)
{
	t_mgl mgl;

	SDL_Init(SDL_INIT_EVERYTHING);
	mgl.window = SDL_CreateWindow(win_name, 50, 50, w / multiplicator,
							h / multiplicator, SDL_WINDOW_SHOWN);
	mgl.windows_count = 1;
	mgl.renderer = SDL_CreateRenderer(mgl.window, -1, 0);
	mgl.screen_surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	mgl.quit = 0;
	mgl.curr_time = SDL_GetTicks() / 1000.0;
	return (mgl);
}

void		mgl_quit(t_mgl *mgl)
{
	SDL_FreeSurface(mgl->screen_surface);
	SDL_DestroyRenderer(mgl->renderer);
	SDL_DestroyWindow(mgl->window);
	SDL_Quit();
}

SDL_Rect	get_rect(t_mgl *mgl)
{
	SDL_Rect rect;

	rect.h = mgl->screen_surface->h;
	rect.w = mgl->screen_surface->w;
	rect.x = 0;
	rect.y = 0;
	return (rect);
}

void		time_handle(t_mgl *mgl, int *frame)
{
	mgl->lst_time = mgl->curr_time;
	mgl->curr_time = SDL_GetTicks() / 1000.0;
	mgl->fps = 1.0 / (mgl->curr_time - mgl->lst_time);
	if ((*frame) == 60 && mgl->show_fps)
	{
		ft_putstr("fps: ");
		ft_putnbr((int)mgl->fps);
		ft_putendl("");
		(*frame) = 0;
	}
	(*frame)++;
}

void		mgl_run(t_mgl *mgl, void (*run_callback)(void *, int *),
			void (*event_callback)(SDL_Event *, void *, int *), void *param)
{
	SDL_Event	event;
	int			frame;
	int			quit;
	SDL_Rect	rect;

	quit = 0;
	frame = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = 1;
			event_callback(&event, param, &quit);
		}
		run_callback(param, mgl->screen_surface->pixels);
		time_handle(mgl, &frame);
		rect = get_rect(mgl);
		SDL_RenderClear(mgl->renderer);
		mgl->screen_texture = SDL_CreateTextureFromSurface(mgl->renderer,
							mgl->screen_surface);
		SDL_RenderCopy(mgl->renderer, mgl->screen_texture, NULL, &rect);
		SDL_RenderPresent(mgl->renderer);
		SDL_DestroyTexture(mgl->screen_texture);
	}
}
