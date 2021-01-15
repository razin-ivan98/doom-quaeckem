/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computing_walls_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:46:26 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 22:17:33 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int			walls_cmp(t_wall w1, t_wall w2)
{
	if ((w1.points[0].x == w2.points[0].x &&
				w1.points[0].y == w2.points[0].y &&
		w1.points[1].x == w2.points[1].x &&
				w1.points[1].y == w2.points[1].y) ||
		(w1.points[0].x == w2.points[1].x &&
				w1.points[0].y == w2.points[1].y &&
		w1.points[1].x == w2.points[0].x
				&& w1.points[1].y == w2.points[0].y))
		return (1);
	return (0);
}

void		analyze_wall(t_wall *wall, t_map_editor *ed, int n)
{
	int i;
	int j;

	i = 0;
	while (i < ed->map.circuits_count)
	{
		if (i == n)
		{
			i++;
			continue ;
		}
		j = 0;
		while (j < ed->map.circuits[i].walls_count)
		{
			if (walls_cmp(ed->map.circuits[i].walls[j], *wall))
				wall->type = WALL_TYPE_SECTOR_BORDER;
			j++;
		}
		i++;
	}
}

t_vertex	get_face_normal(t_map_editor *editor, int i, int j)
{
	t_vertex ret;
	t_vertex p1;
	t_vertex p2;

	p1 = editor->map.circuits[i].points[j - 1];
	p2 = editor->map.circuits[i].points[j];
	ret.x = cos(atan2(p2.y - p1.y, p2.x - p1.x) +
			(float)editor->map.circuits[i].normal_dir * M_PI / 2);
	ret.y = sin(atan2(p2.y - p1.y, p2.x - p1.x) +
			(float)editor->map.circuits[i].normal_dir * M_PI / 2);
	ret.z = 0.0;
	return (normalize(ret));
}

t_wall		inside_cycle(t_wall *curr, t_map_editor *ed,
							int *walls_count)
{
	int i;
	int j;

	i = 0;
	while (i < ed->map.circuits_count)
	{
		j = 1;
		*walls_count = 0;
		while (j < ed->map.circuits[i].points_count)
		{
			curr->points[0] = ed->map.circuits[i].points[j - 1];
			curr->points[0].z = 0.0;
			curr->points[1] = ed->map.circuits[i].points[j];
			curr->points[1].z = 0.0;
			curr->normal = get_face_normal(ed, i, j);
			curr->used_in_bsp = 0;
			curr->type = WALL_TYPE_WALL;
			ed->map.circuits[i].walls[*walls_count] = *curr;
			(*walls_count)++;
			j++;
		}
		ed->map.circuits[i].walls_count = *walls_count;
		i++;
	}
	return (*curr);
}

void		transform_data(t_map_editor *ed)
{
	t_wall	curr;
	int		walls_count;
	int		i;
	int		j;

	inside_cycle(&curr, ed, &walls_count);
	i = 0;
	while (i < ed->map.circuits_count)
	{
		j = 0;
		while (j < ed->map.circuits[i].walls_count)
		{
			analyze_wall(&ed->map.circuits[i].walls[j], ed, i);
			j++;
		}
		i++;
	}
}
