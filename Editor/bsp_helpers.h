/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_helpers.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razin-ivan98 <razin-ivan98@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 07:53:54 by razin-ivan9       #+#    #+#             */
/*   Updated: 2021/01/14 08:17:38 by razin-ivan9      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSP_HELPERS_H
# define BSP_HELPERS_H

# include "editor.h"

void	delete_by_index(t_vertex *points, int index, int *count);
void	insert_by_index(t_vertex *points, int index, int *count, t_vertex p);
void	delete_wall_by_index(t_wall *walls, int index, int *count);
void	insert_wall_by_index(t_wall *walls, int index, int *count, t_wall wall);
void	delete_circuit_by_index(t_circuit *circuits, int index, int *count);

int		get_i_plus_2(int i, int max);
int		get_i_minus_1(int i, int max);
int		get_i_plus_1(int i, int max);

#endif