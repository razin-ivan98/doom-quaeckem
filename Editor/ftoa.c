/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 13:27:12 by razin-ivan9       #+#    #+#             */
/*   Updated: 2021/01/14 13:37:56 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int				itoa_s(int value, char *buf)
{
	int index;
	int i;

	index = 0;
	i = value % 10;
	if (value >= 10)
	{
		index += itoa_s(value / 10, buf);
	}
	buf[index] = i + 0x30;
	index++;
	return (index);
}

char			*itoa(int value, char *buf)
{
	int len;

	len = itoa_s(value, buf);
	buf[len] = '\0';
	return (buf);
}

static int		insert_zeros(int ival, int decimals, char *buf)
{
	int res;
	int i;

	res = 0;
	while (ival)
	{
		ival /= 10;
		res++;
	}
	res = decimals - res;
	i = 0;
	ft_strcpy(buf, "");
	while (i < res)
	{
		ft_strcat(buf, "0");
		i++;
	}
	return (res);
}

static void		f(float *value, char *buf, int *index)
{
	if (*value < 0)
	{
		buf[(*index)++] = '-';
		*value = -*value;
	}
}

char			*ftoa(float value, int decimals, char *buf)
{
	int			i;
	int			index;
	float		rounding;
	int			ival;

	index = 0;
	f(&value, buf, &index);
	rounding = 0.5;
	i = 0;
	while (i++ < decimals)
		rounding /= 10.0;
	value += rounding;
	index += itoa_s((int)(value), buf + index);
	buf[index++] = '.';
	value = value - (int)(value);
	ival = 1;
	i = 0;
	while (i++ < decimals)
		ival *= 10;
	ival *= value;
	index += insert_zeros(ival, decimals, buf + index);
	index += itoa_s(ival, buf + index);
	buf[index] = '\0';
	return (buf);
}
