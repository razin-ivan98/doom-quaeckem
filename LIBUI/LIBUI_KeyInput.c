/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIBUI_KeyInput.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 12:31:28 by chorange          #+#    #+#             */
/*   Updated: 2019/06/29 13:14:39 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LIBUI.h"



void LIBUI_InputLetter(int key, t_LIBUI_Edit *edits, int number, int shift)
{
    int len;

    len = ft_strlen(edits[number].text);
    if (key > 32 && key < 127)
    {
        if (len < 11)
        {
            if (shift && (key >= 'a' && key <= 'z'))
                key -= 32;
            edits[number].text[len] = key;
        }
    }
    else if (key == 8 && len > 0)
        edits[number].text[len - 1] = 0;

    LIBUI_EditRefresh(edits, number);
}