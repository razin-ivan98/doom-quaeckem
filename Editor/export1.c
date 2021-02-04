/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 16:11:02 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/04 14:34:18 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void		do_vt(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	tatb->vt = (t_vertex){node->walls[i].points[0].x,
		node->walls[i].points[0].y, tatb->top};
	tatb->new.ids[0] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->walls[i].points[1].x,
		node->walls[i].points[1].y, tatb->bottom};
	tatb->new.ids[1] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->walls[i].points[1].x,
		node->walls[i].points[1].y, tatb->top};
	tatb->new.ids[2] = add_vt(map, tatb->vt);
	tatb->vt = triangle_normal(map->vts[tatb->new.ids[2]],
		map->vts[tatb->new.ids[1]], map->vts[tatb->new.ids[0]]);
	if (tatb->flag)
		tatb->vt = multiply(tatb->vt, -1);
	tatb->new.n_ids[0] = add_n(map, tatb->vt);
	tatb->new.n_ids[1] = tatb->new.n_ids[0];
	tatb->new.n_ids[2] = tatb->new.n_ids[0];
}
//верхний правый треугольник для полустенок (и для верхней и для нижней)

void		do_uv(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	tatb->uv = (t_vertex){0.0, tatb->bottom * TEXTURE_SCALE, 0.0};
	tatb->new.uv_ids[0] = add_uv(map, tatb->uv);
	tatb->uv = (t_vertex){0.0, tatb->top * TEXTURE_SCALE, 0.0};
	tatb->new.uv_ids[1] = add_uv(map, tatb->uv);
	tatb->uv = (t_vertex){length(sub(node->walls[i].points[0],
		node->walls[i].points[1])) * TEXTURE_SCALE,
		tatb->top * TEXTURE_SCALE, 0.0};
	tatb->new.uv_ids[2] = add_uv(map, tatb->uv);
	tatb->new.type = TR_TYPE_WALL;
	// tatb->link->vt_trs[tatb->link->vt_trs_count] = tatb->new;
	// (tatb->link->vt_trs_count)++;
}

void		do_circuit(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	tatb->link = node;
	if (map->circuits[node->circuit].ceil >
						map->circuits[node->walls[i].circuit].ceil)
	{
		tatb->top = map->circuits[node->circuit].ceil;
		tatb->bottom = map->circuits[node->walls[i].circuit].ceil;
		tatb->flag = 0;
	}
	else if (map->circuits[node->circuit].ceil <
						map->circuits[node->walls[i].circuit].ceil)
	{
		tatb->wall = node->walls[i];
		tatb->link = get_node_by_wall_traversal(tatb->root, tatb->wall);
		if (!tatb->link)
			tatb->link = node;
		tatb->bottom = map->circuits[node->circuit].ceil;
		tatb->top = map->circuits[node->walls[i].circuit].ceil;
		tatb->flag = 1;
	}
	do_vt_sec(tatb, node, map, i);
	do_uv(tatb, node, map, i);
	tatb->link->vt_trs[tatb->link->vt_trs_count] = tatb->new;
	(tatb->link->vt_trs_count)++;
	do_vt(tatb, node, map, i);
	do_uv_sec(tatb, node, map, i);
	tatb->link->vt_trs[tatb->link->vt_trs_count] = tatb->new;
	(tatb->link->vt_trs_count)++;
}

void		do_floor(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	if (map->circuits[node->circuit].floor >
				map->circuits[node->walls[i].circuit].floor)
	{
		tatb->wall = node->walls[i];
		tatb->link = get_node_by_wall_traversal(tatb->root, tatb->wall);
		if (!tatb->link)
			tatb->link = node;
		tatb->top = map->circuits[node->circuit].floor;
		tatb->bottom = map->circuits[node->walls[i].circuit].floor;
		tatb->flag = 1;
	}
	else if (map->circuits[node->circuit].floor <
				map->circuits[node->walls[i].circuit].floor)
	{
		tatb->bottom = map->circuits[node->circuit].floor;
		tatb->top = map->circuits[node->walls[i].circuit].floor;
		tatb->flag = 0;
	}
	do_vt_sec(tatb, node, map, i);
	do_uv(tatb, node, map, i);
	tatb->link->vt_trs[tatb->link->vt_trs_count] = tatb->new;
		(tatb->link->vt_trs_count)++;
	do_vt(tatb, node, map, i);
	do_uv_sec(tatb, node, map, i);
	tatb->link->vt_trs[tatb->link->vt_trs_count] = tatb->new;
		(tatb->link->vt_trs_count)++;
}
void		add_tops_bottoms(t_bsp *node, t_map *map, t_bsp *root)
{
	t_a_t_b	*tatb;
	int		i;

	i = 0;
	tatb = (t_a_t_b *)malloc(sizeof(t_a_t_b));
	tatb->root = root;
	tatb->link = node;
	while (i < node->walls_count)
	{
		tatb->link = node;
		if (node->walls[i].type == WALL_TYPE_SECTOR_BORDER &&
			node->walls[i].circuit != -1 &&
			map->circuits[node->circuit].floor !=
			map->circuits[node->walls[i].circuit].floor)
			do_floor(tatb, node, map, i);
		if (node->walls[i].type == WALL_TYPE_SECTOR_BORDER &&
			node->walls[i].circuit != -1 &&
			map->circuits[node->circuit].ceil !=
			map->circuits[node->walls[i].circuit].ceil)
			do_circuit(tatb, node, map, i);
		i++;
	}
	free(tatb);
}
