/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 15:53:37 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/01/15 15:53:37 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	back_transform(t_bsp *node, t_vertex *pts, int *pts_count)
{
	int i;

	i = 0;
	*pts_count = 0;
	while (i < node->walls_count)
	{
		pts[i] = node->walls[i].points[1];
		(*pts_count)++;
		i++;
	}
}

int check_in_figure(t_vertex *pts, int pts_count, int i)
{
	t_vertex new;
	t_vertex base;
	t_vertex end;

	base = normalize((t_vertex){
		pts[get_i_minus_1(i, pts_count)].x - pts[i].x,
		pts[get_i_minus_1(i, pts_count)].y - pts[i].y,
		0.0});
	new = normalize((t_vertex){
		pts[get_i_plus_2(i, pts_count)].x - pts[i].x,
		pts[get_i_plus_2(i, pts_count)].y - pts[i].y,
		0.0});
	end = normalize((t_vertex){
		pts[get_i_plus_1(i, pts_count)].x - pts[i].x,
		pts[get_i_plus_1(i, pts_count)].y - pts[i].y,
		0.0});

	float angle1 = acosf(dot(base, new));
	float angle2 = acosf(dot(base, end));

	if (cross(base, new).z < 0.0)
		angle1 = 2 * M_PI - angle1;
	if (cross(base, end).z < 0.0)
		angle2 = 2 * M_PI - angle2;

	if (angle2 < angle1 && angle1 > 0.0)
		return (1);
	return (0);
}

int add_n(t_map *map, t_vertex n)
{
	int i;

	i = 0;
	while (i < map->nrmls_count)
	{

		if (map->nrmls[i].x == n.x && map->nrmls[i].y == n.y && map->nrmls[i].z == n.z)
			return (i);
		i++;
	}
	map->nrmls[map->nrmls_count] = n;
	(map->nrmls_count)++;

	return (map->nrmls_count - 1);
}

int add_vt(t_map *map, t_vertex vt)
{
	int i;

	i = 0;
	while (i < map->vts_count)
	{
		if (map->vts[i].x == vt.x && map->vts[i].y == vt.y && map->vts[i].z == vt.z)
			return (i);
		i++;
	}
	map->vts[map->vts_count] = vt;
	(map->vts_count)++;

	return (map->vts_count - 1);
}

int add_uv(t_map *map, t_vertex uv)
{
	int i;

	i = 0;
	while (i < map->uvs_count)
	{
		if (map->uvs[i].x == uv.x && map->uvs[i].y == uv.y)
			return (i);
		i++;
	}
	map->uvs[map->uvs_count] = uv;
	(map->uvs_count)++;

	return (map->uvs_count - 1);
}

t_bsp *get_node_by_wall_traversal(t_bsp *node, t_wall wall)
{
	int i;

	if (node->is_leaf)
	{
		i = 0;
		while (i < node->walls_count)
		{
			if (length(sub(wall.points[0], node->walls[i].points[1])) < 0.1 &&
				length(sub(wall.points[1], node->walls[i].points[0])) < 0.1)
			{
				return (node);
			}
			i++;
		}
	}
	else
	{
		return (get_node_by_wall_traversal(node->front, wall));
		return (get_node_by_wall_traversal(node->back, wall));
	}
	return (NULL);
}

t_tr create_tr(t_vertex *pts, int *pts_count, int i)
{
	t_tr res;

	res.points[0] = pts[i];
	res.points[1] = pts[get_i_plus_1(i, *pts_count)];
	res.points[2] = pts[get_i_plus_2(i, *pts_count)];
	delete_by_index(pts, get_i_plus_1(i, *pts_count), pts_count);
	return (res);
}

void triangulate(t_bsp *node, t_vertex *pts, int *pts_count)
{
	int i;

	int p;
	int counter;

	t_vertex new1;
	t_vertex new2;


	i = 0;
	p = 0;

	counter = 0;

	if (!(node->trs = malloc(sizeof(t_tr) * 2000)))
		exit(-2);

	node->trs_count = 0;

	while (*pts_count > 3)
	{
		new1 = (t_vertex){
			pts[i].x,
			pts[i].y,
			0.0};
		new2 = (t_vertex){
			pts[get_i_plus_2(i, *pts_count)].x,
			pts[get_i_plus_2(i, *pts_count)].y,
			0.0};
		if (check_in_figure(pts, *pts_count, i) == 1)
		{
			node->trs[node->trs_count] = create_tr(pts, pts_count, i);
			(node->trs_count)++;
		}
		else
		{
			i = get_i_plus_1(i, *pts_count);
		}
		counter++;
		if (counter > 1000000)
		{
			ft_putendl("Ошибка триангуляции");
			exit(-2);
		}
	}
	node->trs[node->trs_count] = create_tr(pts, pts_count, 0);
	(node->trs_count)++;
}

void	write_vt_trs_to_file(t_bsp *node, int fd)
{
	int i;

	i = 0;
	if (!node->is_leaf)
		return ;
	while (i < node->vt_trs_count)
	{
		ft_putstr_fd("f ", fd);
		ft_putnbr_fd(node->vt_trs[i].ids[0] + 1, fd);
		ft_putstr_fd("/", fd);
		ft_putnbr_fd(node->vt_trs[i].uv_ids[0] + 1, fd);
		ft_putstr_fd("/", fd);
		ft_putnbr_fd(node->vt_trs[i].n_ids[0] + 1, fd);
		ft_putstr_fd(" ", fd);
		ft_putnbr_fd(node->vt_trs[i].ids[1] + 1, fd);
		ft_putstr_fd("/", fd);
		ft_putnbr_fd(node->vt_trs[i].uv_ids[1] + 1, fd);
		ft_putstr_fd("/", fd);
		ft_putnbr_fd(node->vt_trs[i].n_ids[1] + 1, fd);
		ft_putstr_fd(" ", fd);
		ft_putnbr_fd(node->vt_trs[i].ids[2] + 1, fd);
		ft_putstr_fd("/", fd);
		ft_putnbr_fd(node->vt_trs[i].uv_ids[2] + 1, fd);
		ft_putstr_fd("/", fd);
		ft_putnbr_fd(node->vt_trs[i].n_ids[2] + 1, fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
	ft_putendl_fd("", fd);
}

void	write_to_file(t_map *map, int fd)
{
	int i;
	char *ptr;

	ptr = malloc(256);

	i = 0;
	while (i < map->uvs_count)
	{
		ft_putstr_fd("vt ", fd);
		ptr = ftoa(map->uvs[i].x, 8, ptr);
		ft_putstr_fd(ptr, fd);
		ft_putstr_fd(" ", fd);

		ptr = ftoa(map->uvs[i].y, 8, ptr);
		ft_putstr_fd(ptr, fd);
		ft_putstr_fd("\n", fd);

		i++;
	}

	i = 0;
	while (i < map->nrmls_count)
	{
		ft_putstr_fd("vn ", fd);
		ptr = ftoa(map->nrmls[i].x, 8, ptr);
		ft_putstr_fd(ptr, fd);
		ft_putstr_fd(" ", fd);
		ptr = ftoa(map->nrmls[i].z, 8, ptr);
		ft_putstr_fd(ptr, fd);
		ft_putstr_fd(" ", fd);
		ptr = ftoa(map->nrmls[i].y, 8, ptr);
		ft_putstr_fd(ptr, fd);
		ft_putstr_fd("\n", fd);

		i++;
	}
	i = 0;
printf("%d\n\n", map->vts_count);
	while (i < map->vts_count)
	{
		ft_putstr_fd("v ", fd);
		ptr = ftoa(map->vts[i].x, 8, ptr);
		ft_putstr_fd(ptr, fd);
		ft_putstr_fd(" ", fd);
		ptr = ftoa(map->vts[i].z, 8, ptr);
		ft_putstr_fd(ptr, fd);
		ft_putstr_fd(" ", fd);
		ptr = ftoa(map->vts[i].y, 8, ptr);
		ft_putstr_fd(ptr, fd);
		ft_putstr_fd("\n", fd);

		i++;
	}
	free(ptr);
}

void	bsp_export_traversal(t_bsp *node, t_map *map, t_bsp *root)
{
	t_vertex	pts[100];
	int			pts_count;

	if (node->is_leaf)
	{
		node->floor = map->circuits[node->circuit].floor;
		node->ceil = map->circuits[node->circuit].ceil;
		node->ceil_tex = map->circuits[node->circuit].ceil_tex;
		node->floor_tex = map->circuits[node->circuit].floor_tex;
		node->wall_tex = map->circuits[node->circuit].wall_tex;

		back_transform(node, pts, &pts_count);
		triangulate(node, pts, &pts_count);
		to_obj_format(node, map);
		return ;
	}
	else
	{
		bsp_export_traversal(node->front, map, root);
		bsp_export_traversal(node->back, map, root);
	}
}
void	bsp_top_bottom_traversal(t_bsp *node, t_map *map, t_bsp *root)
{
	if (node->is_leaf)
	{
		add_tops_bottoms(node, map, root);
		return ;
	}
	else
	{
		bsp_top_bottom_traversal(node->front, map, root);
		bsp_top_bottom_traversal(node->back, map, root);
	}
}
void	bsp_write_traversal(t_bsp *node, t_map *map, t_bsp *root, int fd)
{
	if (node->is_leaf)
	{
		write_vt_trs_to_file(node, fd);
		return ;
	}
	else
	{
		bsp_write_traversal(node->front, map, root, fd);
		bsp_write_traversal(node->back, map, root, fd);
	}
}

void	export_map(t_map_editor *ed)
{
	int fd;
	char *str;

	if (!(str = malloc(100001)))
		exit(-2);
	fd = open("geometry.obj", O_RDWR | O_TRUNC | O_CREAT, 0640);
	ed->map.uvs_count = 0;
	ed->map.vts_count = 0;
	ed->map.nrmls_count = 0;

	if (!(ed->map.uvs = malloc(sizeof(t_vertex) * 3000)) ||
		!(ed->map.vts = malloc(sizeof(t_vertex) * 3000)) || 
		!(ed->map.nrmls = malloc(sizeof(t_vertex) * 3000)))
		exit(-2);

	bsp_export_traversal(&ed->root, &ed->map, &ed->root);
	bsp_top_bottom_traversal(&ed->root, &ed->map, &ed->root);
	bsp_write_traversal(&ed->root, &ed->map, &ed->root, fd);
	close(fd);

	fd = open("geometry.obj", O_RDWR);
	if (fd < 0)
	{
		ft_putendl("Ошибка сохранения");
		exit(-2);
	}
	str[read(fd, str, 100000)] = 0;
	close(fd);

	ft_putendl(str);

	fd = open("geometry.obj", O_RDWR | O_TRUNC);

	if (fd < 0)
	{
		ft_putendl("Ошибка сохранения");
		exit(-2);
	}
	write_to_file(&ed->map, fd);
	close(fd);

	fd = open("geometry.obj", O_RDWR | O_APPEND);
	if (fd < 0)
	{
		ft_putendl("Ошибка сохранения");
		exit(-2);
	}
	ft_putendl_fd("", fd);
	ft_putendl_fd(str, fd);
	close(fd);
	free (str);
}