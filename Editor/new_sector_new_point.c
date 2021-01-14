/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_sector_new_point.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:11:10 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 14:12:00 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void		map_new_circuit(t_map *map)
{
	map->circuits[map->circuits_count].points_count = 0;
	map->circuits[map->circuits_count].walls_count = 0;
	map->circuits[map->circuits_count].ceil = DEFAULT_CEIL;
	map->circuits[map->circuits_count].floor = DEFAULT_FLOOR;
	map->circuits[map->circuits_count].wall_tex = 0;
	map->circuits[map->circuits_count].floor_tex = 0;
	map->circuits[map->circuits_count].ceil_tex = 0;
	map->circuits[map->circuits_count].normal_dir = 1;
	(map->circuits_count)++;
}

static t_vertex	get_nearest(t_vertex f, t_vertex s, float x, float y)
{
	t_vertex	ret;
	t_vertex	line;
	t_vertex	ort;

	line = get_line_by_points(f, s);
	ort = get_ort_line_by_point(line, (t_vertex){x, y, 0.0});
	ret = lines_intersect(line, ort);
	return (ret);
}

static void		correct_point_coords(t_map *map, float *x,
					float *y, int flag)
{
	t_vertex first;
	t_vertex second;
	t_vertex new;

	if (map->on_line && !flag)
	{
		first = map->circuits[map->on_circuit_i].points[map->on_point_i];
		second = map->circuits[map->on_circuit_i].points[
				get_i_plus_1(map->on_point_i,
					map->circuits[map->on_circuit_i].points_count)];
		new = get_nearest(first, second, *x, *y);
		insert_by_index(map->circuits[map->on_circuit_i].points,
			get_i_plus_1(map->on_point_i,
					map->circuits[map->on_circuit_i].points_count),
			&map->circuits[map->on_circuit_i].points_count,
			new);
		*x = new.x;
		*y = new.y;
	}
	else if (map->on_point && !flag)
	{
		*x = map->circuits[map->on_circuit_i].points[map->on_point_i].x;
		*y = map->circuits[map->on_circuit_i].points[map->on_point_i].y;
	}
}

void			map_new_point(t_map *map, float x, float y, int flag)
{
	int index;

	if (!map->active)
	{
		if (map->circuits_count > 0 && !map->on_point && !map->on_line)
		{
			return ;
		}
		if (map->circuits_count > 18)
		{
			ft_putendl("Больше нельзя");
			return ;
		}
		map_new_circuit(map);
		map->active = 1;
	}
	correct_point_coords(map, &x, &y, flag);
	index = map->circuits[map->circuits_count - 1].points_count;
	map->circuits[map->circuits_count - 1].integrated = 0;
	map->circuits[map->circuits_count - 1].points[index].x = x;
	map->circuits[map->circuits_count - 1].points[index].y = y;
	map->circuits[map->circuits_count - 1].points_count++;
}
