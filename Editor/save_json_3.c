/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_json_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 22:52:01 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 23:03:28 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	check_walls(t_bsp *node, int i, int fd)
{
	char str[64];

	ft_putstr_fd("{\n\t\t\t\"type\":", fd);
	itoa(node->walls[i].type, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ft_putstr_fd("\n\t\t\t\"points\": [\n\t\t\t\t[", fd);
	ftoa(node->walls[i].points[0].x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->walls[i].points[0].y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n\t\t\t\t[", fd);
	ftoa(node->walls[i].points[1].x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->walls[i].points[1].y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("]\n\t\t\t],\n\t\t\t\"normal\": [", fd);
	ftoa(node->walls[i].normal.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->walls[i].normal.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("]\n\t\t\t}", fd);
}

void	check_objects(t_map_editor *ed, int fd)
{
	int		i;
	char	str[64];

	i = 0;
	ft_putstr_fd("\"objects\": [", fd);
	while (i < ed->objects_count)
	{
		ft_putstr_fd("{\n\t\t\t\"index\":", fd);
		itoa(ed->objects[i].index, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ft_putstr_fd("\n\t\t\t\"pos\": [", fd);
		ftoa(ed->objects[i].pos.x, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ftoa(ed->objects[i].pos.y, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("]}", fd);
		if (i < ed->objects_count - 1)
			ft_putstr_fd(",", fd);
		i++;
	}
	ft_putstr_fd("],", fd);
}

void	bsp_save_traversal(t_bsp *node, int fd)
{
	int		i;

	i = 0;
	ft_putstr_fd("{\n", fd);
	if (node->is_leaf == 0)
	{
		check_not_leaf(node, fd);
		ft_putstr_fd("\"front\": ", fd);
		bsp_save_traversal(node->front, fd);
		ft_putstr_fd(",\n\"back\": ", fd);
		bsp_save_traversal(node->back, fd);
	}
	else
	{
		write_data_1(node, fd);
		write_data_2(node, fd);
		ft_putstr_fd("\n\t\t]\n", fd);
	}
	ft_putstr_fd("}\n", fd);
}

void	check_enemies(t_map_editor *ed, int fd)
{
	char	str[64];
	int		i;

	ft_putstr_fd("\"enemies\": [", fd);
	i = 0;
	while (i < ed->enemies_count)
	{
		ft_putstr_fd("[", fd);
		ftoa(ed->enemies[i].x, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ftoa(ed->enemies[i].y, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("]", fd);
		if (i < ed->enemies_count - 1)
		{
			ft_putstr_fd(",\n", fd);
		}
		i++;
	}
	ft_putstr_fd("],", fd);
}

void	check_ammo(t_map_editor *ed, int fd)
{
	char	str[64];
	int		i;

	ft_putstr_fd("\"ammo\": [", fd);
	i = 0;
	while (i < ed->ammo_count)
	{
		ft_putstr_fd("[", fd);
		ftoa(ed->ammo[i].x, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ftoa(ed->ammo[i].y, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("]", fd);
		if (i < ed->ammo_count - 1)
		{
			ft_putstr_fd(",\n", fd);
		}
		i++;
	}
	ft_putstr_fd("],", fd);
}
