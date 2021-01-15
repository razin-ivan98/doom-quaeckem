/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_json_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 22:52:01 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 22:54:32 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	write_data_3(t_bsp *node, int fd, int i)
{
	char	str[64];

	ft_putstr_fd("\n\t\t\t\"uv_ids\": [\n\t\t\t\t", fd);
	itoa(node->vt_trs[i].uv_ids[0], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].uv_ids[1], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].uv_ids[2], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
	ft_putstr_fd("\n\t\t\t\"type\": \n\t\t\t\t", fd);
	itoa(node->vt_trs[i].type, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("}", fd);
}

void	check_vt_trs(t_bsp *node, int i, int fd)
{
	char	str[64];

	ft_putstr_fd("{", fd);
	ft_putstr_fd("\n\t\t\t\"ids\": [\n\t\t\t\t", fd);
	itoa(node->vt_trs[i].ids[0], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].ids[1], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].ids[2], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
	ft_putstr_fd("\n\t\t\t\"n_ids\": [\n\t\t\t\t", fd);
	itoa(node->vt_trs[i].n_ids[0], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].n_ids[1], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].n_ids[2], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
	write_data_3(node, fd, i);
}

void	check_not_leaf(t_bsp *node, int fd)
{
	char str[64];

	ft_putstr_fd("\t\"isLeaf\": ", fd);
	itoa(node->is_leaf, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\n\t\"line\": [", fd);
	ftoa(node->line.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->line.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n\t\"normal\": [", fd);
	ftoa(node->normal.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->normal.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n", fd);
}

void	write_data_1(t_bsp *node, int fd)
{
	char str[64];

	ft_putstr_fd("\t\"isLeaf\": ", fd);
	itoa(node->is_leaf, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\t\"floor\": ", fd);
	ftoa(node->floor, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\t\"ceil\": ", fd);
	ftoa(node->ceil, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\t\"wall_tex\": ", fd);
	itoa(node->wall_tex, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\t\"floor_tex\": ", fd);
	itoa(node->floor_tex, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\t\"ceil_tex\": ", fd);
	itoa(node->ceil_tex, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\n\t\"walls\": [\n\t\t\t", fd);
}

void	write_data_2(t_bsp *node, int fd)
{
	int		i;

	i = 0;
	while (i < node->walls_count)
	{
		check_walls(node, i, fd);
		if (i != node->walls_count - 1)
			ft_putstr_fd(",\n\t\t\t", fd);
		i++;
	}
	ft_putstr_fd("\n\t\t]\n", fd);
	i = 0;
	ft_putstr_fd(",\n\t\"vt_trs\": [\n\t\t\t", fd);
	while (i < node->vt_trs_count)
	{
		check_vt_trs(node, i, fd);
		if (i != node->vt_trs_count - 1)
			ft_putstr_fd(",\n\t\t\t", fd);
		i++;
	}
}
