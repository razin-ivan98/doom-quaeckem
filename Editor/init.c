/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:08:46 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 14:08:48 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void	map_init(t_map *map)
{
	map->circuits_count = 0;
	map->active = 0;
	map->on_line = 0;
	map->on_point = 0;
	map->border_id = 0;
	map->selected_circuit = 0;
}

void		editor_init(t_map_editor *ed)
{
	ed->mode = RENDER_MODE_POINTS;
	ed->edit_ceil = 0;
	ed->edit_floor = 0;
	ed->edit_wall_tex = 0;
	ed->edit_ceil_tex = 0;
	ed->edit_floor_tex = 0;
	ed->ammo_count = 0;
	ed->enemies_count = 0;
	ed->objects_count = 0;
	ed->aid_count = 0;
	ed->curr_object = 0;
	ed->tv_beta = 0.0;
	ed->player = (t_vertex){0.0, -3.0, 2.78};
	ed->tv = (t_vertex){0.0, 0.0, 0.0};
	ed->aim = (t_vertex){0.0, 10.0, 0.0};
	ed->step = STEP_1_DRAW;
	map_init(&ed->map);
}
