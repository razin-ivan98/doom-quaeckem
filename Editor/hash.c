/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razin-ivan98 <razin-ivan98@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 03:50:48 by razin-ivan9       #+#    #+#             */
/*   Updated: 2021/01/14 12:27:27 by razin-ivan9      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../duke.h"


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

	return hash;
}

void    set_hash()
{
	int		fd1;
	int		fd2;
	char	*str1;
	char	*str2;
	int		hash_n;


	if (!(str1 = (char *)malloc(1000001)) || !(str2 = (char *)malloc(500001)))
	{
		ft_putendl("Ошибка памяти");
		exit(-2);
	}
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
	if (!*str1 || !*str2)
	{
		ft_putendl("Ошибка сохранения");
		exit(-2);
	}
	ft_strcat(str1, str2);
	hash_n = hash(str1);
	fd1 = open("geometry.obj", O_RDWR);
	if (fd1 < 0)
	{
		ft_putendl("Ошибка сохранения");
		exit(-2);
	}
	str1[read(fd1, str1, 1000000)] = 0;
	close(fd1);
	fd1 = open("geometry.obj", O_RDWR | O_TRUNC);
	fd2 = open("data.json", O_RDWR | O_TRUNC);
	if (fd1 < 0 || fd2 < 0)
	{
		ft_putendl("Ошибка сохранения");
		exit(-2);
	}
	ft_putnbr_fd(hash_n, fd1);
	ft_putnbr_fd(hash_n, fd2);
	ft_putendl_fd("", fd1);
	ft_putendl_fd("", fd2);
	close(fd1);
	close(fd2);
	fd1 = open("geometry.obj", O_RDWR | O_APPEND);
	fd2 = open("data.json", O_RDWR | O_APPEND);
	if (fd1 < 0 || fd2 < 0)
	{
		ft_putendl("Ошибка сохранения");
		exit(-2);
	}
	ft_putstr_fd(str1, fd1);
	ft_putstr_fd(str2, fd2);
	close(fd1);
	close(fd2);
	free(str1);
	free(str2);
	ft_putendl("Сохранено!");

}

int		check_hash(char *foldername)
{
	char	path[1024];
	int		fd1;
	int		fd2;
	char 	*str1;
	char 	*str2;
	char	*ptr;
	int		hash_n;

	if (!(str1 = (char *)malloc(1000001)) || !(str2 = (char *)malloc(500001)))
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

	if (fd1 < 0 || fd2 < 0)
	{
		ft_putendl("Ошибка чтения");
		exit(-2);
	}
	str1[read(fd1, str1, 1000000)] = 0;
	str2[read(fd2, str2, 500000)] = 0;

	if ((hash_n = ft_atoi(str1)) != ft_atoi(str2))
	{
		ft_putendl("Файлы повреждены");
		exit(-2);
	}
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
