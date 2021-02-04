/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_down.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 18:11:28 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 18:15:25 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	key_down_1(t_map_editor *ed, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_ESCAPE)
	{
		exit(1);
	}
	else if (event->key.keysym.sym == SDLK_RETURN)
		key_enter_down(ed);
	else if (event->key.keysym.sym == SDLK_EQUALS)
		key_plus_down(ed);
	else if (event->key.keysym.sym == SDLK_MINUS)
		key_minus_down(ed);
	else if (event->key.keysym.sym == SDLK_1)
	{
		if (ed->step == STEP_4_TEXTURES)
		{
			ed->edit_ceil_tex = 0;
			ed->edit_wall_tex = 1;
			ed->edit_floor_tex = 0;
			write_wall_tex(ed);
		}
	}
}

void	key_down_2(t_map_editor *ed, SDL_Event *event)
{
	if (event->key.keysym.sym == SDLK_2)
	{
		if (ed->step == STEP_4_TEXTURES)
		{
			ed->edit_ceil_tex = 0;
			ed->edit_wall_tex = 0;
			ed->edit_floor_tex = 1;
			write_floor_tex(ed);
		}
	}
	else if (event->key.keysym.sym == SDLK_3)
	{
		if (ed->step == STEP_4_TEXTURES)
		{
			ed->edit_ceil_tex = 1;
			ed->edit_wall_tex = 0;
			ed->edit_floor_tex = 0;
			write_ceil_tex(ed);
		}
	}
}

void	key_down(t_map_editor *ed, SDL_Event *event)
{
	key_down_1(ed, event);
	key_down_2(ed, event);

	if (event->key.keysym.sym == SDLK_e)
	{
		if (ed->step == STEP_1_DRAW)
		{
			map_new_point(&ed->map, ed->map.circuits[
				ed->map.circuits_count - 1].points[0].x,
			ed->map.circuits[ed->map.circuits_count -
								1].points[0].y, 1);
			ed->map.active = 0;
		}
	}
}
