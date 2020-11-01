/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIBUI_Disactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 12:07:10 by chorange          #+#    #+#             */
/*   Updated: 2019/06/29 12:14:29 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LIBUI.h"

void LIBUI_DisactiveAll(t_LIBUI_Edit *edits, int c_edits, t_LIBUI_List *lists, int c_lists)
{
    int i;

    i = 0;
    while (i < c_edits)
    {
        edits[i].is_active = 0;
        i++;
    }
    i = 0;
    while (i < c_lists)
    {
        lists[i].is_dropped = 0;
        i++;
    }
}