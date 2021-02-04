/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_recurse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 16:48:58 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 19:19:42 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int		if_leaf(t_bsp *bsp, t_map *map, t_int_v q, t_circuit *circuits)
{
	int i;
	int j;

	if (q.x == -1)
	{
		bsp->walls_count = 0;
		bsp->is_leaf = 1;
		i = 0;
		while (i < q.y)
		{
			j = 0;
			while (j < circuits[i].walls_count)
			{
				bsp->floor = map->circuits[i].floor;
				bsp->ceil = map->circuits[i].ceil;
				bsp->circuit = i;
				bsp->walls[bsp->walls_count++] = circuits[i].walls[j];
				j++;
			}
			i++;
		}
		free(circuits);
		return (1);
	}
	return (0);
}

void	if_cutted(t_bsp *bsp, t_circuit *circuits, t_req_s *s)
{
	s->new1 = circuits[s->i].walls[s->j];
	s->new2 = circuits[s->i].walls[s->j];
	s->new1.points[0] = circuits[s->i].walls[s->j].points[0];
	s->new1.points[1] = s->inter;
	s->new2.points[0] = s->new1.points[1];
	s->new2.points[1] = circuits[s->i].walls[s->j].points[1];
	s->result = classify_wall(s->cutter_line, bsp->normal, s->new1, &s->inter);
	if (s->result == FRONT)
	{
		s->front[s->i].walls[s->front[s->i].walls_count] = s->new1;
		s->front[s->i].walls_count++;
		s->back[s->i].walls[s->back[s->i].walls_count] = s->new2;
		s->back[s->i].walls_count++;
	}
	else
	{
		s->front[s->i].walls[s->front[s->i].walls_count] = s->new2;
		s->front[s->i].walls_count++;
		s->back[s->i].walls[s->back[s->i].walls_count] = s->new1;
		s->back[s->i].walls_count++;
	}
}

void	if_complanar(t_circuit *circuits, t_req_s *s)
{
	s->cutter = circuits[s->cutter_cir].walls[s->cutter_wall];
	circuits[s->cutter_cir].walls[s->cutter_wall].border_id = -1;
	circuits[s->i].walls[s->j].border_id = -1;
	if (s->i != s->cutter_cir)
	{
		circuits[s->i].walls[s->j].used_in_bsp = 1;
		if (circuits[s->i].walls[s->j].type == WALL_TYPE_SECTOR_BORDER)
		{
			circuits[s->i].walls[s->j].circuit = s->cutter_cir;
		}
		s->back[s->i].walls[s->back[s->i].walls_count] =
					circuits[s->i].walls[s->j];
		s->back[s->i].walls_count++;
	}
	else
	{
		if (circuits[s->i].walls[s->j].type == WALL_TYPE_SECTOR_BORDER)
			circuits[s->i].walls[s->j].circuit = -1;
		circuits[s->i].walls[s->j].used_in_bsp = 1;
		s->front[s->i].walls[s->front[s->i].walls_count] =
			circuits[s->i].walls[s->j];
		s->front[s->i].walls_count++;
	}
}

void	cycle(t_bsp *bsp, t_circuit *circuits, int circuits_count,
			t_req_s *s)
{
	s->i = 0;
	while (s->i < circuits_count)
	{
		s->j = 0;
		while (s->j < circuits[s->i].walls_count)
		{
			s->result = classify_wall(s->cutter_line, bsp->normal,
					circuits[s->i].walls[s->j], &s->inter);
			if (s->result == CUTTED)
				if_cutted(bsp, circuits, s);
			else if (s->result == FRONT)
			{
				s->front[s->i].walls[s->front[s->i].walls_count++] =
						circuits[s->i].walls[s->j];
			}
			else if (s->result == BACK)
				s->back[s->i].walls[s->back[s->i].walls_count++] =
						circuits[s->i].walls[s->j];
			else
				if_complanar(circuits, s);
			s->j++;
		}
		s->i++;
	}
}

void	bsp_recurse(t_bsp *bsp, t_circuit *circuits, int circuits_count,
						t_map *map)
{
	t_req_s s;

	reconstruct_circuits(circuits, circuits_count);
	get_cutter(circuits, circuits_count, &s.cutter_cir, &s.cutter_wall);
	s.front = NULL;
	s.back = NULL;
	if (if_leaf(bsp, map, (t_int_v){s.cutter_cir, circuits_count}, circuits))
		return ;
	s.cutter = get_big_wall(circuits[s.cutter_cir].walls[s.cutter_wall], bsp,
				&s.cutter_line);
	if (!(s.front = malloc(sizeof(t_circuit) * circuits_count)) ||
			!(s.back = malloc(sizeof(t_circuit) * circuits_count)))
		exit(-2);
	ft_bzero(s.front, sizeof(t_circuit) * circuits_count);
	ft_bzero(s.back, sizeof(t_circuit) * circuits_count);
	bsp->is_leaf = 0;
	bsp->walls_count = 0;
	bsp->normal = circuits[s.cutter_cir].walls[s.cutter_wall].normal;
	if (!(bsp->back = malloc(sizeof(t_bsp))) ||
			!(bsp->front = malloc(sizeof(t_bsp))))
		exit(-2);
	cycle(bsp, circuits, circuits_count, &s);
	free(circuits);
	bsp_recurse(bsp->back, s.back, circuits_count, map);
	bsp_recurse(bsp->front, s.front, circuits_count, map);
}
