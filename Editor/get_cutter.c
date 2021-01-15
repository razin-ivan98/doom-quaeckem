/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cutter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:00:58 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 19:34:47 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void		calc_can_cut(t_cc *c, t_circuit *circuits, int cir, int wall)
{
	if (cross(c->base, sub(circuits[c->i].walls[c->j].points[1],
					circuits[cir].walls[wall].points[0])).z > 0)
		c->res[0] = 1;
	if (cross(c->base, sub(circuits[c->i].walls[c->j].points[1],
					circuits[cir].walls[wall].points[0])).z < 0)
		c->res[1] = 1;
}

static int		if_can_cut(t_circuit *circuits, int circuits_count,
				int cir, int wall)
{
	t_cc c;

	c.res[0] = 0;
	c.res[1] = 0;
	c.base = sub(circuits[cir].walls[wall].points[1],
		circuits[cir].walls[wall].points[0]);
	c.i = 0;
	while (c.i < circuits_count)
	{
		c.j = 0;
		while (c.j < circuits[c.i].walls_count)
		{
			calc_can_cut(&c, circuits, cir, wall);
			if (c.res[0] && c.res[1])
				return (1);
			c.j++;
		}
		c.i++;
	}
	return (0);
}

static int		get_border_cutter(t_circuit *circuits, int circuits_count,
								int *cutter_cir, int *cutter_wall)
{
	int i;
	int j;

	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			if (circuits[i].walls[j].type == WALL_TYPE_SECTOR_BORDER &&
				!circuits[i].walls[j].used_in_bsp &&
							length(circuits[i].walls[j].normal))
			{
				*cutter_cir = i;
				*cutter_wall = j;
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

static int		get_other_cutter(t_circuit *circuits, int circuits_count,
								int *cutter_cir, int *cutter_wall)
{
	int i;
	int j;

	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			if (!circuits[i].walls[j].used_in_bsp &&
							length(circuits[i].walls[j].normal) &&
				if_can_cut(circuits, circuits_count, i, j))
			{
				*cutter_cir = i;
				*cutter_wall = j;
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

void			get_cutter(t_circuit *circuits, int circuits_count,
								int *cutter_cir, int *cutter_wall)
{
	if (get_border_cutter(circuits, circuits_count,
						cutter_cir, cutter_wall))
		return ;
	if (get_other_cutter(circuits, circuits_count,
						cutter_cir, cutter_wall))
		return ;
	*cutter_cir = -1;
	*cutter_wall = -1;
}
