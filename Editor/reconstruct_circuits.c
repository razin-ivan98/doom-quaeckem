/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reconstruct_circuits.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 17:50:59 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 18:03:16 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void	insert_cycle(t_circuit *circuits, int i)
{
	int			j;
	t_vertex	p1;
	t_vertex	p2;
	t_wall		wall;

	wall.type = WALL_TYPE_FICTIVE;
	wall.used_in_bsp = 1;
	j = 0;
	while (j < circuits[i].walls_count)
	{
		p1 = circuits[i].walls[get_i_plus_1(j,
						circuits[i].walls_count)].points[0];
		p2 = circuits[i].walls[j].points[1];
		if (!(length(sub(p1, p2)) < 0.01))
		{
			wall.points[0] = p2;
			wall.points[1] = p1;
			wall.normal = (t_vertex){0.0, 0.0, 0.0};
			insert_wall_by_index(circuits[i].walls, j + 1,
								&circuits[i].walls_count, wall);
		}
		j++;
	}
}

static void	union_cycle(t_circuit *circuits, int i)
{
	int j;

	j = 0;
	while (j < circuits[i].walls_count)
	{
		if (fabsf(dot(normalize(sub(circuits[i].walls[j].points[1],
		circuits[i].walls[j].points[0])), normalize(sub(circuits[i].walls[
			get_i_plus_1(j, circuits[i].walls_count)].points[1],
		circuits[i].walls[get_i_plus_1(j,
				circuits[i].walls_count)].points[0])))) > 0.9999 &&
		((circuits[i].walls[get_i_plus_1(j,
					circuits[i].walls_count)].type == WALL_TYPE_FICTIVE &&
		circuits[i].walls[j].type == WALL_TYPE_FICTIVE) ||
		(circuits[i].walls[get_i_plus_1(j,
					circuits[i].walls_count)].type == WALL_TYPE_WALL &&
		circuits[i].walls[j].type == WALL_TYPE_WALL)))
		{
			circuits[i].walls[j].points[1] = circuits[i].walls[get_i_plus_1(j,
							circuits[i].walls_count)].points[1];
			delete_wall_by_index(circuits[i].walls,
			get_i_plus_1(j, circuits[i].walls_count), &circuits[i].walls_count);
			j--;
		}
		j++;
	}
}

void		pointed_wals_cycle(t_circuit *circuits, int i)
{
	int j;

	j = 0;
	while (j < circuits[i].walls_count)
	{
		if (length(sub(circuits[i].walls[j].points[1],
							circuits[i].walls[j].points[0])) < 0.01)
		{
			delete_wall_by_index(circuits[i].walls, j,
					&circuits[i].walls_count);
			j--;
		}
		j++;
	}
}

void		reconstruct_circuits(t_circuit *circuits, int circuits_count)
{
	int			i;

	i = 0;
	while (i < circuits_count)
	{
		insert_cycle(circuits, i);
		union_cycle(circuits, i);
		pointed_wals_cycle(circuits, i);
		i++;
	}
}
