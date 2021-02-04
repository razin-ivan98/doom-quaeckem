/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:16:52 by Chorange          #+#    #+#             */
/*   Updated: 2021/02/04 15:03:40 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	draw_line(int *pixels, t_int_v f, t_int_v s, int color)
{
	t_int_v		delta;
	t_int_v		sign;
	int			error;
	int			error2;

	delta = (t_int_v){abs(s.x - f.x), abs(s.y - f.y)};
	sign.x = f.x < s.x ? 1 : -1;
	sign.y = f.y < s.y ? 1 : -1;
	error = delta.x - delta.y;
	put_pixel(pixels, s.x, s.y, color);
	while (f.x != s.x || f.y != s.y)
	{
		put_pixel(pixels, f.x, f.y, color);
		error2 = error * 2;
		if (error2 > -delta.y)
		{
			error -= delta.y;
			f.x += sign.x;
		}
		if (error2 < delta.x)
		{
			error += delta.x;
			f.y += sign.y;
		}
	}
}

void	draw_in_points_mode(int i, int *pixels, t_map_editor *ed)
{
	int			j;
	t_int_v		p1;
	t_int_v		p2;

	j = 1;
	while (j < ed->map.circuits[i].points_count)
	{
		p1.x = (int)(ed->map.circuits[i].points[j - 1].x * MAP_EDITOR_SCALE);
		p1.y = (int)(ed->map.circuits[i].points[j - 1].y * MAP_EDITOR_SCALE);
		p2.x = (int)(ed->map.circuits[i].points[j].x * MAP_EDITOR_SCALE);
		p2.y = (int)(ed->map.circuits[i].points[j].y * MAP_EDITOR_SCALE);
		draw_line(pixels, p1, p2, 0xFFFFFF);

		draw_line(pixels, (t_int_v){p1.x + (p2.x - p1.x) / 2, p1.y +
												(p2.y - p1.y) / 2},
			(t_int_v){p1.x + (p2.x - p1.x) / 2 + 30 * cos(atan2(p2.y -
												p1.y, p2.x - p1.x) +
			(float)ed->map.circuits[i].normal_dir * M_PI / 2),
			p1.y + (p2.y - p1.y) / 2 + 30 * sin(atan2(p2.y - p1.y,
														p2.x - p1.x) +
			(float)ed->map.circuits[i].normal_dir * M_PI / 2)},
			0xff0000);
		j++;
	}
}

void	draw_in_walls_mode(int i, int *pixels, t_map_editor *ed)
{
	int			j;
	t_vertex	n;
	int			color;
	t_int_v		p1;
	t_int_v		p2;

	j = -1;
	while (j++ < ed->map.circuits[i].walls_count)
	{
		p1.x = (int)(ed->map.circuits[i].walls[j].points[0].x *
				MAP_EDITOR_SCALE);
		p1.y = (int)(ed->map.circuits[i].walls[j].points[0].y *
				MAP_EDITOR_SCALE);
		p2.x = (int)(ed->map.circuits[i].walls[j].points[1].x *
				MAP_EDITOR_SCALE);
		p2.y = (int)(ed->map.circuits[i].walls[j].points[1].y *
				MAP_EDITOR_SCALE);
		n = ed->map.circuits[i].walls[j].normal;
		color = (i == ed->map.selected_circuit ? 0xFF0000 : 0xFFFF00);
		draw_line(pixels, p1, p2, color);
		draw_line(pixels,
			(t_int_v){p1.x + (p2.x - p1.x) / 2, p1.y + (p2.y - p1.y) / 2},
			(t_int_v){p1.x + (p2.x - p1.x) / 2 + 30 * n.x,
			p1.y + (p2.y - p1.y) / 2 + 30 * n.y}, 0xff0000);
	}
}

void	draw_elements(t_map_editor *ed, int *pixels)
{
	if (ed->step >= STEP_5_PLAYER)
		draw_point(multiply(ed->player, MAP_EDITOR_SCALE), 0x00ff00, pixels);
	if (ed->step >= STEP_6_AMMO)
		draw_ammo(ed, pixels);
	if (ed->step >= STEP_7_AID)
		draw_aid(ed, pixels);
	if (ed->step >= STEP_8_AIM)
		draw_point(multiply(ed->aim, MAP_EDITOR_SCALE), 0x0000ff, pixels);
	if (ed->step >= STEP_9_DECOR)
		draw_tv(ed, pixels);
	if (ed->step >= STEP_10_OBJECTS)
		draw_objects(ed, pixels);
	if (ed->step >= STEP_11_ENEMIES)
		draw_enemies(ed, pixels);
}

void	draw_point(t_vertex pos, int color, int *pixels)
{
	int i;
	int j;

	i = pos.x - 3;
	while (i < pos.x + 4)
	{
		j = pos.y - 3;
		while (j < pos.y + 4)
		{
			put_pixel(pixels, i, j, color);
			j++;
		}
		i++;
	}
}
