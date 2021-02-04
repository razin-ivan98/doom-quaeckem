/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_elements_to_map.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:35:35 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 19:22:48 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	add_aid(t_vertex pos, t_map_editor *ed)
{
	if (ed->aid_count == 39)
	{
		ft_putendl("Больше нельзя");
		return ;
	}
	ed->aid[ed->aid_count] = pos;
	ed->aid_count++;
}

void	add_ammo(t_vertex pos, t_map_editor *ed)
{
	if (ed->ammo_count == 39)
	{
		ft_putendl("Больше нельзя");
		return ;
	}
	ed->ammo[ed->ammo_count] = pos;
	ed->ammo_count++;
}

void	add_enemy(t_vertex pos, t_map_editor *ed)
{
	if (ed->enemies_count == 39)
	{
		ft_putendl("Больше нельзя");
		return ;
	}
	ed->enemies[ed->enemies_count] = pos;
	ed->enemies_count++;
}

void	add_object(t_vertex pos, int number, t_map_editor *ed)
{
	if (ed->objects_count == 39)
	{
		ft_putendl("Больше нельзя");
		return ;
	}
	ed->objects[ed->objects_count] = (t_m_e_object){pos, number};
	ed->objects_count++;
}
