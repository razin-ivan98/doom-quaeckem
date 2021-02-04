/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:34:24 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 19:19:08 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp_helpers.h"

void	delete_by_index(t_vertex *points, int index, int *count)
{
	int i;

	i = index;
	(*count)--;
	while (i < *count)
	{
		points[i] = points[i + 1];
		i++;
	}
}

void	insert_by_index(t_vertex *points, int index, int *count, t_vertex p)
{
	int i;

	i = *count;
	while (i > index)
	{
		points[i] = points[i - 1];
		i--;
	}
	points[index] = p;
	(*count)++;
}

void	delete_wall_by_index(t_wall *walls, int index, int *count)
{
	int i;

	i = index;
	(*count)--;
	while (i < *count)
	{
		walls[i] = walls[i + 1];
		i++;
	}
}

void	delete_circuit_by_index(t_circuit *circuits, int index, int *count)
{
	int i;

	i = index;
	(*count)--;
	while (i < *count)
	{
		circuits[i] = circuits[i + 1];
		i++;
	}
}

void	insert_wall_by_index(t_wall *walls, int index, int *count, t_wall wall)
{
	int i;

	i = *count;
	while (i > index)
	{
		walls[i] = walls[i - 1];
		i--;
	}
	walls[index] = wall;
	(*count)++;
}
