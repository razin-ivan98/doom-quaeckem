/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_graphics_lib.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 00:05:10 by razin-ivan9       #+#    #+#             */
/*   Updated: 2021/01/23 12:14:30 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_GRAPHICS_LIB_H
# define MY_GRAPHICS_LIB_H

# include "../frameworks/SDL2.framework/Headers/SDL.h"
# include "../frameworks/SDL2_ttf.framework/Headers/SDL_ttf.h"

# include "../libft/libft.h"

typedef struct		s_mgl
{
	SDL_Window		*window;
	int				windows_count;
	SDL_Renderer	*renderer;
	SDL_Surface		*screen_surface;
	SDL_Texture		*screen_texture;
	SDL_Event		event;

	int				quit;

	int				show_fps;

	float			lst_time;
	float			curr_time;
	float			fps;

	int				trs_count;
	int				vs_count;
}					t_mgl;

t_mgl		mgl_init(char *win_name, int w, int h, int multiplicator);
void				mgl_quit(t_mgl *mgl);
void				mgl_run(t_mgl *mgl,
				void (*run_callback)(void *, int *),
				void (*event_callback)(SDL_Event *, void *, int *),
				void *param);

#endif
