/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_trs_by_bsp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 00:16:41 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 00:17:22 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void	draw_tr(t_bsp *node, int *pixels, int i)
{
	t_int_v p1;
	t_int_v p2;

	p1 = (t_int_v){node->trs[i].points[0].x * MAP_EDITOR_SCALE,
				node->trs[i].points[0].y * MAP_EDITOR_SCALE};
	p2 = (t_int_v){node->trs[i].points[1].x * MAP_EDITOR_SCALE,
				node->trs[i].points[1].y * MAP_EDITOR_SCALE};
	draw_line(pixels, p1, p2, 0xFFFFFF);
	p1 = (t_int_v){node->trs[i].points[1].x * MAP_EDITOR_SCALE,
				node->trs[i].points[1].y * MAP_EDITOR_SCALE};
	p2 = (t_int_v){node->trs[i].points[2].x * MAP_EDITOR_SCALE,
				node->trs[i].points[2].y * MAP_EDITOR_SCALE};
	draw_line(pixels, p1, p2, 0xFFFFFF);
	p1 = (t_int_v){node->trs[i].points[2].x * MAP_EDITOR_SCALE,
					node->trs[i].points[2].y * MAP_EDITOR_SCALE};
	p2 = (t_int_v){node->trs[i].points[0].x * MAP_EDITOR_SCALE,
					node->trs[i].points[0].y * MAP_EDITOR_SCALE};
	draw_line(pixels, p1, p2, 0xFFFFFF);
}

void		bsp_trs_draw_traversal(t_bsp *node, int *pixels)
{
	int i;

	if (node->is_leaf)
	{
		i = 0;
		while (i < node->trs_count)
		{
			draw_tr(node, pixels, i);
			i++;
		}
	}
	else
	{
		bsp_trs_draw_traversal(node->front, pixels);
		bsp_trs_draw_traversal(node->back, pixels);
	}
}
