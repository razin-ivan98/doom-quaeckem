/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 02:41:02 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 02:47:29 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	set_hash_1(char *str1, char *str2)
{
	int fd1;
	int fd2;

	fd1 = open("geometry.obj", O_RDWR);
	fd2 = open("data.json", O_RDWR);
	if (fd1 < 0 || fd2 < 0)
	{
		ft_putendl("Ошибка сохранения");
		exit(-2);
	}
	str1[read(fd1, str1, 1000000)] = 0;
	str2[read(fd2, str2, 500000)] = 0;
	close(fd1);
	close(fd2);
}

void	set_hash_2(char *str1, char *str2, int hash_n)
{
	int fd1;
	int fd2;

	fd1 = open("geometry.obj", O_RDWR | O_TRUNC);
	fd2 = open("data.json", O_RDWR | O_TRUNC);
	if (fd1 < 0 || fd2 < 0)
		exit(-2);
	ft_putnbr_fd(hash_n, fd1);
	ft_putnbr_fd(hash_n, fd2);
	ft_putendl_fd("", fd1);
	ft_putendl_fd("", fd2);
	close(fd1);
	close(fd2);
	fd1 = open("geometry.obj", O_RDWR | O_APPEND);
	fd2 = open("data.json", O_RDWR | O_APPEND);
	if (fd1 < 0 || fd2 < 0)
		exit(-2);
	ft_putstr_fd(str1, fd1);
	ft_putstr_fd(str2, fd2);
	close(fd1);
	close(fd2);
	free(str1);
	free(str2);
	ft_putendl("Сохранено!");
}
