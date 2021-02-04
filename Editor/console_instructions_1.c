/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_instructions_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:29:11 by Chorange          #+#    #+#             */
/*   Updated: 2021/01/15 19:22:42 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	write_instructions(t_map_editor *ed)
{
	if (ed->step == STEP_1_DRAW)
		ft_putendl("Посекторно нарисуйте карту");
	else if (ed->step == STEP_2_FLOOR)
		ft_putendl("Отредактируйте высоту пола секторов");
	else if (ed->step == STEP_3_CEIL)
		ft_putendl("Отредактируйте высоту потолка секторов");
	else if (ed->step == STEP_4_TEXTURES)
		ft_putendl("Отредактируйте текстуры секторов");
	else if (ed->step == STEP_5_PLAYER)
		ft_putendl("Рсположите игрока на карте");
	else if (ed->step == STEP_6_AMMO)
		ft_putendl("Расположите патроны на карте");
	else if (ed->step == STEP_7_AID)
		ft_putendl("Расположите аптечки на карте");
	else if (ed->step == STEP_8_AIM)
		ft_putendl("Расположите цель на карте");
	else if (ed->step == STEP_9_DECOR)
		ft_putendl("Расположите телевизор на каарте");
	else if (ed->step == STEP_10_OBJECTS)
		ft_putendl("Расположите объекты на карте");
	else if (ed->step == STEP_11_ENEMIES)
		ft_putendl("Расположите врагов на карте");
	else if (ed->step == STEP_12_SAVE)
		ft_putendl("Сохранение...");
}

void	write_object_num(t_map_editor *ed)
{
	char str[64];

	ft_putstr("Тип объекта: ");
	itoa(ed->curr_object, str);
	ft_putendl(str);
}
