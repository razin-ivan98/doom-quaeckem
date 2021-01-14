/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 03:50:48 by razin-ivan9       #+#    #+#             */
/*   Updated: 2021/01/14 14:08:19 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../duke.h"
#include "editor.h"

int		hash(const char *str)
{
	unsigned int	hash;

	hash = 0;
	while (*str)
	{
		hash += (unsigned char)(*str);
		hash += (hash << 10);
		hash ^= (hash >> 6);
		str++;
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return (hash);
}

void	s(char *str1, char *str2, int *fd1, int *fd2)
{
	int hash_n;

	hash_n = hash(str1);
	*fd1 = open("geometry.obj", O_RDWR);
	if (*fd1 < 0)
		err("Ошибка сохранения");
	str1[read(*fd1, str1, 1000000)] = 0;
	close(*fd1);
	*fd1 = open("geometry.obj", O_RDWR | O_TRUNC);
	*fd2 = open("data.json", O_RDWR | O_TRUNC);
	if (*fd1 < 0 || *fd2 < 0)
		err("Ошибка сохранения");
	ft_putnbr_fd(hash_n, *fd1);
	ft_putnbr_fd(hash_n, *fd2);
	ft_putendl_fd("", *fd1);
	ft_putendl_fd("", *fd2);
	close(*fd1);
	close(*fd2);
	*fd1 = open("geometry.obj", O_RDWR | O_APPEND);
	*fd2 = open("data.json", O_RDWR | O_APPEND);
	if (*fd1 < 0 || *fd2 < 0)
		err("Ошибка сохранения");
	ft_putstr_fd(str1, *fd1);
	ft_putstr_fd(str2, *fd2);
	close(*fd1);
}

void	set_hash(void)
{
	int		fd1;
	int		fd2;
	char	*str1;
	char	*str2;

	str1 = (char *)malloc(1000001);
	str2 = (char *)malloc(500001);
	if (!str1 || !str2)
		err("Ошибка памяти");
	fd1 = open("geometry.obj", O_RDWR);
	fd2 = open("data.json", O_RDWR);
	if (fd1 < 0 || fd2 < 0)
		err("Ошибка сохранения");
	str1[read(fd1, str1, 1000000)] = 0;
	str2[read(fd2, str2, 500000)] = 0;
	close(fd1);
	close(fd2);
	if (!*str1 || !*str2)
		err("Ошибка сохранения");
	ft_strcat(str1, str2);
	s(str1, str2, &fd1, &fd2);
	close(fd2);
	free(str1);
	free(str2);
	ft_putendl("Сохранено!");
}

int		check_files(char *str1, char *str2, int fd1, int fd2)
{
	int		hash_n;
	char	*ptr;

	if (fd1 < 0 || fd2 < 0)
		err("Ошибка памяти");
	str1[read(fd1, str1, 1000000)] = 0;
	str2[read(fd2, str2, 500000)] = 0;
	if ((hash_n = ft_atoi(str1)) != ft_atoi(str2))
		err("Файлы повреждены");
	ptr = ft_strchr(str1, '\n') + 1;
	ft_strcpy(str1, ptr);
	ptr = ft_strchr(str2, '\n') + 1;
	ft_strcat(str1, ptr);
	if (hash_n == hash(str1))
	{
		free(str1);
		free(str2);
		return (1);
	}
	free(str1);
	free(str2);
	return (0);
}

int		check_hash(char *foldername)
{
	char	path[1024];
	int		fd1;
	int		fd2;
	char	*str1;
	char	*str2;

	str1 = (char *)malloc(1000001);
	str2 = (char *)malloc(500001);
	if (!str1 || !str2)
	{
		ft_putendl("Ошибка памяти");
		exit(-2);
	}
	ft_strcpy(path, foldername);
	ft_strcat(path, "/geometry.obj");
	fd1 = open(path, O_RDONLY);
	ft_strcpy(path, foldername);
	ft_strcat(path, "/data.json");
	fd2 = open(path, O_RDONLY);
	if (check_files(str1, str2, fd1, fd2))
		return (1);
	return (0);
}
