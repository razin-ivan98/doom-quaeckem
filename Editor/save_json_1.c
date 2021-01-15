/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_json_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 22:49:11 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/14 22:55:19 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	check_aid(t_map_editor *ed, int fd)
{
	char	str[64];
	int		i;

	ft_putstr_fd("\"aid\": [", fd);
	i = 0;
	while (i < ed->aid_count)
	{
		ft_putstr_fd("[", fd);
		ftoa(ed->aid[i].x, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ftoa(ed->aid[i].y, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("]", fd);
		if (i < ed->aid_count - 1)
		{
			ft_putstr_fd(",\n", fd);
		}
		i++;
	}
	ft_putstr_fd("],", fd);
}

void	check_player(t_map_editor *ed, int fd)
{
	char str[64];

	ft_putstr_fd("\"player\": [", fd);
	ftoa(ed->player.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(ed->player.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
}

void	check_aim(t_map_editor *ed, int fd)
{
	char str[64];

	ft_putstr_fd("\"aim\": [", fd);
	ftoa(ed->aim.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(ed->aim.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
}

void	check_tv(t_map_editor *ed, int fd)
{
	char str[64];

	ft_putstr_fd("\"tv\": { \"pos\":", fd);
	ft_putstr_fd("[", fd);
	ftoa(ed->tv.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(ed->tv.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
	ft_putstr_fd("\"beta\":", fd);
	ftoa(ed->tv_beta, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("},", fd);
}

void	save_json(t_bsp *root, t_map_editor *ed)
{
	int fd;

	fd = open("data.json", O_CREAT | O_RDWR | O_TRUNC, 0644);
	ft_putstr_fd("{\n", fd);
	check_player(ed, fd);
	check_aim(ed, fd);
	check_tv(ed, fd);
	check_enemies(ed, fd);
	check_ammo(ed, fd);
	check_objects(ed, fd);
	check_aid(ed, fd);
	ft_putstr_fd("\"bsp\" : ", fd);
	bsp_save_traversal(root, fd);
	ft_putstr_fd("}\n", fd);
	close(fd);
}
