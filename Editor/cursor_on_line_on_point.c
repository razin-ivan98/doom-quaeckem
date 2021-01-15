/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_on_line_on_point.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:30:54 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 19:19:25 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int			check_on_point(t_map_editor *ed, int i, int j)
{
	float	x;
	float	y;

	x = ed->map.circuits[i].points[j].x * MAP_EDITOR_SCALE + W_2;
	y = H_2 - ed->map.circuits[i].points[j].y * MAP_EDITOR_SCALE;
	if ((ed->prev_x - x) * (ed->prev_x - x) +
		(ed->prev_y - y) * (ed->prev_y - y) <= 16)
	{
		ed->map.on_line = 0;
		ed->map.on_point = 1;
		ed->map.on_circuit_i = i;
		ed->map.on_point_i = j;
		return (1);
	}
	return (0);
}

int			cursor_on_point(t_map_editor *ed)
{
	int		i;
	int		j;
	int		max;

	i = 0;
	max = ed->map.circuits_count;
	if (ed->map.active)
		max--;
	while (i < max)
	{
		j = 0;
		while (j < ed->map.circuits[i].points_count)
		{
			if (check_on_point(ed, i, j))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

static int	circle_with_cut(t_vertex a, t_vertex b, t_vertex m)
{
	float aa;
	float bb;
	float cc;

	aa = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
	bb = 2 * ((b.x - a.x) * (a.x - m.x) + (b.y - a.y) * (a.y - m.y));
	cc = m.x * m.x + m.y * m.y + a.x * a.x +
		a.y * a.y - 2 * (m.x * a.x + m.y * a.y) - 16;
	if (-bb < 0)
		return (cc < 0);
	if (-bb < (2 * aa))
		return (4 * aa * cc - bb * bb < 0);
	return (aa + bb + cc < 0);
}

static int	check_on_line(t_map_editor *ed, int i, int j)
{
	t_vertex a;
	t_vertex b;

	a = ed->map.circuits[i].points[get_i_minus_1(j,
					ed->map.circuits[i].points_count)];
	b = ed->map.circuits[i].points[j];
	a.x = a.x * MAP_EDITOR_SCALE + W_2;
	b.x = b.x * MAP_EDITOR_SCALE + W_2;
	a.z = 0;
	b.z = 0;
	a.y = H_2 - a.y * MAP_EDITOR_SCALE;
	b.y = H_2 - b.y * MAP_EDITOR_SCALE;
	if (circle_with_cut(a, b, (t_vertex){ed->prev_x, ed->prev_y, 0.0}))
	{
		ed->map.on_line = 1;
		ed->map.on_point = 0;
		ed->map.on_circuit_i = i;
		ed->map.on_point_i = get_i_minus_1(j,
									ed->map.circuits[i].points_count);
		return 1;
	}
	return (0);
}

int			cursor_on_line(t_map_editor *ed)
{
	int i;
	int j;
	int max;

	i = 0;
	max = ed->map.circuits_count;
	if (ed->map.active)
		max--;
	while (i < max)
	{
		j = 0;
		while (j < ed->map.circuits[i].points_count)
		{
			if (check_on_line(ed, i, j))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}
