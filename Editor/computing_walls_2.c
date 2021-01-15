/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computing_walls_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 22:07:58 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 22:28:55 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_wall	get_big_wall(t_wall wall, t_bsp *bsp, t_vertex *line)
{
	line->x = wall.points[0].y - wall.points[1].y;
	line->y = wall.points[1].x - wall.points[0].x;
	line->z = wall.points[0].x * wall.points[1].y - wall.points[1].x *
								wall.points[0].y;
	bsp->line.x = -(line->x / line->y);
	bsp->line.y = -(line->z / line->y);
	bsp->line.z = 0.0;
	if (fabsf(line->x) > fabsf(line->y))
	{
		wall.points[0].y = -9999.0;
		wall.points[1].y = 9999.0;
		wall.points[0].x = (-line->y * wall.points[0].y - line->z) / line->x;
		wall.points[1].x = (-line->y * wall.points[1].y - line->z) / line->x;
	}
	else
	{
		wall.points[0].x = -9999.0;
		wall.points[1].x = 9999.0;
		wall.points[0].y = (-line->x * wall.points[0].x - line->z) / line->y;
		wall.points[1].y = (-line->x * wall.points[1].x - line->z) / line->y;
	}
	return (wall);
}

void	check_wall(t_vertex line, t_wall *wall, t_w_zz *z)
{
	z->p = wall->points[0];
	z->ort_line = get_ort_line_by_point(line, wall->points[0]);
	z->in = lines_intersect(line, z->ort_line);
	z->ort1 = sub(z->p, z->in);
	z->p = wall->points[1];
}

int		classify_wall(t_vertex line, t_vertex n, t_wall wall, t_vertex *inter)
{
	t_w_zz z;

	check_wall(line, &wall, &z);
	z.ort_line.z = -line.x * wall.points[1].y + line.y * wall.points[1].x;
	z.in = lines_intersect(line, z.ort_line);
	z.ort2 = sub(z.p, z.in);
	z.dot1 = dot(normalize(z.ort1), normalize(n));
	z.dot2 = dot(normalize(z.ort2), normalize(n));
	if (length(z.ort1) < 0.01 && length(z.ort2) < 0.01)
		return (COMPLANAR);
	else if (((z.dot1 < 0.0 && z.dot2 > 0.0) || (z.dot1 > 0.0 &&
								z.dot2 < 0.0)) &&
				length(z.ort1) > 0.01 && length(z.ort2) > 0.01)
	{
		z.ort_line = get_line_by_points(wall.points[0], wall.points[1]);
		*inter = lines_intersect(line, z.ort_line);
		return (CUTTED);
	}
	else if ((length(z.ort1) > 0.01 && z.dot1 > 0.0) ||
				(length(z.ort2) > 0.01 && z.dot2 > 0.0))
		return (FRONT);
	return (BACK);
}

int		sign(float a)
{
	if (a == 0.0)
		return (0);
	if (a > 0.0)
		return (1);
	return (-1);
}
