#include "my_graphics_lib.h"

t_mgl	*mgl_init(char *win_name, int w, int h)
{
	t_mgl	*mgl;

	mgl = (t_mgl*)malloc(sizeof(t_mgl));///////////////////////////////////////////

	if (!mgl)
		return (NULL);

	SDL_Init(SDL_INIT_EVERYTHING);

	mgl->window = SDL_CreateWindow(win_name, 50, 50, w, h, SDL_WINDOW_SHOWN);
	mgl->renderer = SDL_CreateRenderer(mgl->window, -1, 0);
	mgl->screen_surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

	mgl->quit = 0;

	mgl->trs = (t_triangle *)malloc(sizeof(t_triangle) * 30000);//////////////////////////
	mgl->vs = (t_vertex *)malloc(sizeof(t_vertex) * 30000);/////////////////////////////////

	mgl->trs_count = 0;
	mgl->vs_count = 0;

	return (mgl);
}

void	mgl_quit(t_mgl *mgl)
{
	SDL_FreeSurface(mgl->screen_surface);
	SDL_DestroyRenderer(mgl->renderer);
	//SDL_DestroyTexture(mgl->screen_texture);
	SDL_DestroyWindow(mgl->window);
	SDL_Quit();
}

static void	mgl_draw(t_mgl *mgl, void *param, int *pixels)
{
	mgl->trs_count = 0;
	mgl->vs_count = 0;

	
}

void	mgl_run(t_mgl *mgl, void (*run_callback)(void *, int *), void (*event_callback)(SDL_Event *, void *, int *), void *param)
{
	SDL_Event	event;
	SDL_Rect	rect;
	int			frame;
	int			quit;

	rect.h = mgl->screen_surface->h;
	rect.w = mgl->screen_surface->w;
	rect.x = 0;
	rect.y = 0;
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
		
		mgl_draw(mgl, param, mgl->screen_surface->pixels);

		if (frame == 60)
		{
			mgl->curr_time = SDL_GetTicks() / 1000.0;

			mgl->fps = 1.0 / (mgl->curr_time - mgl->lst_time) * 60.0;
			mgl->lst_time = mgl->curr_time;
			printf("fps: %f\n", mgl->fps);
			frame = 0;;
		}
		frame++;

		SDL_RenderClear(mgl->renderer);
		mgl->screen_texture = SDL_CreateTextureFromSurface(mgl->renderer, mgl->screen_surface);
		SDL_RenderCopy(mgl->renderer, mgl->screen_texture, NULL, &rect);
		SDL_RenderPresent(mgl->renderer);
		SDL_DestroyTexture(mgl->screen_texture);
	}
}