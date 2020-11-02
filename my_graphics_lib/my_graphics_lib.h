#ifndef MY_GRAPHICS_LIB_H
# define MY_GRAPHICS_LIB_H

#include <SDL2/SDL.h>
// #include "data.h"

typedef struct      s_mgl
{
    SDL_Window		*window;
	int				windows_count;
	SDL_Renderer	*renderer;
	SDL_Surface		*screen_surface;
	SDL_Texture		*screen_texture;
	SDL_Event		event;

	int				quit;

	float			lst_time;
	float			curr_time;
	float			fps;

//	t_triangle		*trs;
//	t_vertex		*vs;

	int				trs_count;
	int				vs_count;
}                   t_mgl;

t_mgl	*mgl_init(char *win_name, int w, int h);
void	mgl_quit(t_mgl *mgl);
void	mgl_run(t_mgl *mgl,
				void (*run_callback)(void *, int *),
				void (*event_callback)(SDL_Event *, void *, int *),
				void *param);


#endif