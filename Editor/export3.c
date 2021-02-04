/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 18:55:42 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/04 14:33:13 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void		do_vt_sec(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	tatb->vt = (t_vertex){node->walls[i].points[0].x,
		node->walls[i].points[0].y, tatb->top};
	tatb->new.ids[0] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->walls[i].points[0].x,
		node->walls[i].points[0].y, tatb->bottom};
	tatb->new.ids[1] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->walls[i].points[1].x,
		node->walls[i].points[1].y, tatb->bottom};
	tatb->new.ids[2] = add_vt(map, tatb->vt);
	tatb->vt = triangle_normal(map->vts[tatb->new.ids[2]],
		map->vts[tatb->new.ids[1]], map->vts[tatb->new.ids[0]]);
	if (tatb->flag)
		tatb->vt = multiply(tatb->vt, -1);
	tatb->new.n_ids[0] = add_n(map, tatb->vt);
	tatb->new.n_ids[1] = tatb->new.n_ids[0];
	tatb->new.n_ids[2] = tatb->new.n_ids[0];
}
//нижний левый треугольник для полустенок (и для верхней и для нижней)

void		do_uv_sec(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	tatb->uv = (t_vertex){0.0, tatb->bottom * TEXTURE_SCALE, 0.0};
	tatb->new.uv_ids[0] = add_uv(map, tatb->uv);
	tatb->uv = (t_vertex){length(sub(node->walls[i].points[0],
			node->walls[i].points[1])) * TEXTURE_SCALE,
			tatb->top * TEXTURE_SCALE, 0.0};
	tatb->new.uv_ids[1] = add_uv(map, tatb->uv);
	tatb->uv = (t_vertex){length(sub(node->walls[i].points[0],
			node->walls[i].points[1])) * TEXTURE_SCALE,
			tatb->bottom * TEXTURE_SCALE, 0.0};
	tatb->new.uv_ids[2] = add_uv(map, tatb->uv);
	tatb->new.type = TR_TYPE_WALL;
	// tatb->link->vt_trs[tatb->link->vt_trs_count] = tatb->new;
	// (tatb->link->vt_trs_count)++;
}

void		do_vt_2(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	tatb->vt = (t_vertex){node->trs[i].points[0].x, node->trs[i].points[0].y,
				map->circuits[node->circuit].floor};
	tatb->new.ids[0] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->trs[i].points[1].x, node->trs[i].points[1].y,
				map->circuits[node->circuit].floor};
	tatb->new.ids[1] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->trs[i].points[2].x, node->trs[i].points[2].y,
				map->circuits[node->circuit].floor};
	tatb->new.ids[2] = add_vt(map, tatb->vt);
	tatb->vt = triangle_normal(map->vts[tatb->new.ids[0]],
				map->vts[tatb->new.ids[1]], map->vts[tatb->new.ids[2]]);
	tatb->new.n_ids[0] = add_n(map, tatb->vt);
	tatb->new.n_ids[1] = tatb->new.n_ids[0];
	tatb->new.n_ids[2] = tatb->new.n_ids[0];
}
//это рисует треугольник на полу

void		do_vt_3(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	tatb->vt = (t_vertex){node->trs[i].points[0].x, node->trs[i].points[0].y,
			map->circuits[node->circuit].ceil};
	tatb->new.ids[0] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->trs[i].points[1].x, node->trs[i].points[1].y,
			map->circuits[node->circuit].ceil};
	tatb->new.ids[1] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->trs[i].points[2].x, node->trs[i].points[2].y,
			map->circuits[node->circuit].ceil};
	tatb->new.ids[2] = add_vt(map, tatb->vt);
	tatb->vt = triangle_normal(map->vts[tatb->new.ids[0]],
			map->vts[tatb->new.ids[1]], map->vts[tatb->new.ids[2]]);
	tatb->vt = multiply(tatb->vt, -1);
	tatb->new.n_ids[0] = add_n(map, tatb->vt);
	tatb->new.n_ids[1] = tatb->new.n_ids[0];
	tatb->new.n_ids[2] = tatb->new.n_ids[0];
}
//это рисует треугольник на потолке

void		do_vt_4(t_a_t_b *tatb, t_bsp *node, t_map *map, int i)
{
	tatb->vt = (t_vertex){node->walls[i].points[0].x,
					node->walls[i].points[0].y,
					map->circuits[node->circuit].ceil};
	tatb->new.ids[0] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->walls[i].points[0].x,
					node->walls[i].points[0].y,
					map->circuits[node->circuit].floor};
	tatb->new.ids[1] = add_vt(map, tatb->vt);
	tatb->vt = (t_vertex){node->walls[i].points[1].x,
					node->walls[i].points[1].y,
					map->circuits[node->circuit].floor};
	tatb->new.ids[2] = add_vt(map, tatb->vt);
	tatb->vt = triangle_normal(map->vts[tatb->new.ids[2]],
			map->vts[tatb->new.ids[1]], map->vts[tatb->new.ids[0]]);
	tatb->new.n_ids[0] = add_n(map, tatb->vt);
	tatb->new.n_ids[1] = tatb->new.n_ids[0];
	tatb->new.n_ids[2] = tatb->new.n_ids[0];
}
//это рисует нижний правыый треугольник для стенки
