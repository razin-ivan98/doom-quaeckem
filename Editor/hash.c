/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 03:50:48 by razin-ivan9       #+#    #+#             */
/*   Updated: 2021/01/15 19:21:55 by Chorange         ###   ########.fr       */
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

void	set_hash(void)
{
	int		fd1;
	char	*str1;
	char	*str2;
	int		hash_n;

	if (!(str1 = (char *)malloc(1000001)) ||
			!(str2 = (char *)malloc(500001)))
		exit(-2);
	set_hash_1(str1, str2);
	ft_strcat(str1, str2);
	hash_n = hash(str1);
	fd1 = open("geometry.obj", O_RDWR);
	if (fd1 < 0)
		exit(-2);
	str1[read(fd1, str1, 1000000)] = 0;
	close(fd1);
	set_hash_2(str1, str2, hash_n);
}

t_int_v	check_hash_1(char *foldername, char *path)
{
	t_int_v fd;

	ft_strcpy(path, foldername);
	ft_strcat(path, "/geometry.obj");
	fd.x = open(path, O_RDONLY);
	ft_strcpy(path, foldername);
	ft_strcat(path, "/data.json");
	fd.y = open(path, O_RDONLY);
	return (fd);
}

int		check_hash_free(char *str1, char *str2, int hash_n)
{
	char *ptr;

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
	t_int_v	fd;
	char	*str1;
	char	*str2;
	int		hash_n;

	if (!(str1 = (char *)malloc(1000001)) ||
			!(str2 = (char *)malloc(500001)))
		exit(-2);
	fd = check_hash_1(foldername, path);
	if (fd.x < 0 || fd.y < 0)
		exit(-2);
	str1[read(fd.x, str1, 1000000)] = 0;
	str2[read(fd.y, str2, 500000)] = 0;
	if ((hash_n = ft_atoi(str1)) != ft_atoi(str2))
		exit(-2);
	return (check_hash_free(str1, str2, hash_n));
}
