/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_elements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:20:26 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 19:37:28 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	draw_ammo(t_map_editor *ed, int *pixels)
{
	int i;

	i = 0;
	while (i < ed->ammo_count)
	{
		draw_point(multiply(ed->ammo[i], MAP_EDITOR_SCALE),
				0xffff00, pixels);
		i++;
	}
}

void	draw_aid(t_map_editor *ed, int *pixels)
{
	int i;

	i = 0;
	while (i < ed->aid_count)
	{
		draw_point(multiply(ed->aid[i], MAP_EDITOR_SCALE),
			0xffffff, pixels);
		i++;
	}
}

void	draw_enemies(t_map_editor *ed, int *pixels)
{
	int i;

	i = 0;
	while (i < ed->enemies_count)
	{
		draw_point(multiply(ed->enemies[i], MAP_EDITOR_SCALE),
			0xff0000, pixels);
		i++;
	}
}

void	draw_objects(t_map_editor *ed, int *pixels)
{
	int i;

	i = 0;
	while (i < ed->objects_count)
	{
		draw_point(multiply(ed->objects[i].pos, MAP_EDITOR_SCALE),
			0xff00ff, pixels);
		i++;
	}
}

void	draw_tv(t_map_editor *ed, int *pixels)
{
	t_int_v p1;
	t_int_v p2;

	p1 = (t_int_v){(ed->tv.x + 2 * cos(ed->tv_beta / 180 * M_PI)) *
													MAP_EDITOR_SCALE,
			(ed->tv.y +  2 * sin(ed->tv_beta / 180 * M_PI)) *
													MAP_EDITOR_SCALE};
	p2 = (t_int_v){(ed->tv.x -  2 * cos(ed->tv_beta / 180 * M_PI)) *
													MAP_EDITOR_SCALE,
			(ed->tv.y -  2 * sin(ed->tv_beta / 180 * M_PI)) *
													MAP_EDITOR_SCALE};
	draw_line(pixels, p1, p2, 0x00ffff);
}
