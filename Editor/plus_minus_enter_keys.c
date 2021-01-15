/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plus_minus_enter_keys.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:12:35 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 19:21:16 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void			key_enter_down(t_map_editor *ed)
{
	ed->step++;
	if (ed->step == STEP_2_FLOOR)
	{
		bsp_compile(ed);
		ed->mode = RENDER_MODE_WALLS;
	}
	else if (ed->step == STEP_4_TEXTURES)
	{
		ed->edit_wall_tex = 1;
		ed->edit_floor_tex = 0;
		ed->edit_ceil_tex = 0;
	}
	else if (ed->step == STEP_12_SAVE)
	{
		export_map(ed);
		save_json(&ed->root, ed);
		set_hash();
		ed->mode = RENDER_MODE_TRS;
	}
	write_instructions(ed);
}

static void		key_minus_down_2(t_map_editor *ed)
{
	if (ed->step == STEP_3_CEIL)
	{
		ed->map.circuits[ed->map.selected_circuit].ceil -= 0.1;
		write_ceil_height(ed);
	}
	if (ed->step == STEP_2_FLOOR)
	{
		ed->map.circuits[ed->map.selected_circuit].floor -= 0.1;
		write_floor_height(ed);
	}
	if (ed->edit_floor_tex && ed->step == STEP_4_TEXTURES)
	{
		ed->map.circuits[ed->map.selected_circuit].floor_tex -= 1;
		if (ed->map.circuits[ed->map.selected_circuit].floor_tex < 0)
			ed->map.circuits[ed->map.selected_circuit].floor_tex = 5;
		write_floor_tex(ed);
	}
	if (ed->edit_ceil_tex && ed->step == STEP_4_TEXTURES)
	{
		ed->map.circuits[ed->map.selected_circuit].ceil_tex -= 1;
		if (ed->map.circuits[ed->map.selected_circuit].ceil_tex < 0)
			ed->map.circuits[ed->map.selected_circuit].ceil_tex = 5;
		write_ceil_tex(ed);
	}
}

void			key_minus_down(t_map_editor *ed)
{
	key_minus_down_2(ed);
	if (ed->edit_wall_tex && ed->step == STEP_4_TEXTURES)
	{
		ed->map.circuits[ed->map.selected_circuit].wall_tex -= 1;
		if (ed->map.circuits[ed->map.selected_circuit].wall_tex < 0)
			ed->map.circuits[ed->map.selected_circuit].wall_tex = 5;
		write_wall_tex(ed);
	}
	if (ed->step == STEP_10_OBJECTS)
	{
		ed->curr_object -= 1;
		if (ed->curr_object < 0)
			ed->curr_object = 3;
		write_object_num(ed);
	}
	if (ed->step == STEP_9_DECOR)
	{
		ed->tv_beta -= 5.0;
	}
}

static void	key_plus_down_2(t_map_editor *ed)
{
	if (ed->edit_ceil_tex && ed->step == STEP_4_TEXTURES)
	{
		ed->map.circuits[ed->map.selected_circuit].ceil_tex += 1;
		if (ed->map.circuits[ed->map.selected_circuit].ceil_tex > 5)
			ed->map.circuits[ed->map.selected_circuit].ceil_tex = 0;
		write_ceil_tex(ed);
	}
	if (ed->edit_wall_tex && ed->step == STEP_4_TEXTURES)
	{
		ed->map.circuits[ed->map.selected_circuit].wall_tex += 1;
		if (ed->map.circuits[ed->map.selected_circuit].wall_tex > 5)
			ed->map.circuits[ed->map.selected_circuit].wall_tex = 0;
		write_wall_tex(ed);
	}
	if (ed->step == STEP_10_OBJECTS)
	{
		ed->curr_object += 1;
		if (ed->curr_object > 3)
			ed->curr_object = 0;
		write_object_num(ed);
	}
	if (ed->step == STEP_9_DECOR)
		ed->tv_beta += 10.0;
}

void			key_plus_down(t_map_editor *ed)
{
	key_plus_down_2(ed);
	if (ed->step == STEP_3_CEIL)
	{
		ed->map.circuits[ed->map.selected_circuit].ceil += 0.1;
		write_ceil_height(ed);
	}
	if (ed->step == STEP_2_FLOOR)
	{
		ed->map.circuits[ed->map.selected_circuit].floor += 0.1;
		write_floor_height(ed);
	}
	if (ed->edit_floor_tex && ed->step == STEP_4_TEXTURES)
	{
		ed->map.circuits[ed->map.selected_circuit].floor_tex += 1;
		if (ed->map.circuits[ed->map.selected_circuit].floor_tex > 5)
			ed->map.circuits[ed->map.selected_circuit].floor_tex = 0;
		write_floor_tex(ed);
	}
}
