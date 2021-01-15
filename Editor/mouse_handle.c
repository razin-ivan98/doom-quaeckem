/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:10:02 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 19:22:07 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void		mouse_motion(t_map_editor *ed)
{
	if (cursor_on_point(ed))
	{
		SDL_FreeSurface(ed->cursor_surface);
		SDL_FreeCursor(ed->cursor);
		ed->cursor_surface = SDL_LoadBMP("../textures/cursor-2.bmp");
		ed->cursor = SDL_CreateColorCursor(ed->cursor_surface, 5, 5);
		SDL_SetCursor(ed->cursor);
	}
	else if (cursor_on_line(ed))
	{
		SDL_FreeSurface(ed->cursor_surface);
		SDL_FreeCursor(ed->cursor);
		ed->cursor_surface = SDL_LoadBMP("../textures/cursor.bmp");
		ed->cursor = SDL_CreateColorCursor(ed->cursor_surface, 5, 5);
		SDL_SetCursor(ed->cursor);
	}
	else
	{
		ed->map.on_line = 0;
		ed->map.on_point = 0;
		SDL_FreeCursor(ed->cursor);
		ed->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		SDL_SetCursor(ed->cursor);
	}
}

static void	mouse_b_d_select_sector(t_map_editor *ed)
{
	ed->map.selected_circuit = bsp_select_circuit_traversal(&ed->root,
		(t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
				(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0});
	if (ed->step == STEP_2_FLOOR)
		write_floor_height(ed);
	else if (ed->step == STEP_3_CEIL)
		write_ceil_height(ed);
	else if (ed->step == STEP_4_TEXTURES)
	{
		if (ed->edit_wall_tex)
			write_wall_tex(ed);
		else if (ed->edit_floor_tex)
			write_floor_tex(ed);
		else if (ed->edit_ceil_tex)
			write_ceil_tex(ed);
	}
}

static void	mouse_b_d_new_point(t_map_editor *ed)
{
	if (ed->map.circuits[ed->map.active].points_count > 90)
	{
		ft_putendl("Больше нельзя");
	}
	map_new_point(&ed->map, (float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
				(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0);
}

void		mouse_button_down(t_map_editor *ed)
{
	if (ed->step == STEP_1_DRAW)
		mouse_b_d_new_point(ed);
	else if (ed->step >= STEP_2_FLOOR && ed->step <= STEP_4_TEXTURES)
		mouse_b_d_select_sector(ed);
	else if (ed->step == STEP_5_PLAYER)
		ed->player = (t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
					(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0};
	else if (ed->step == STEP_8_AIM)
		ed->aim = (t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
					(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0};
	else if (ed->step == STEP_6_AMMO)
		add_ammo((t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
					(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0}, ed);
	else if (ed->step == STEP_7_AID)
		add_aid((t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
					(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0}, ed);
	else if (ed->step == STEP_9_DECOR)
		ed->tv = (t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
					(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0};
	else if (ed->step == STEP_10_OBJECTS)
		add_object((t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
	(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0}, ed->curr_object, ed);
	else if (ed->step == STEP_11_ENEMIES)
		add_enemy((t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
					(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0}, ed);
}
