/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIBUI_Edit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 11:16:04 by chorange          #+#    #+#             */
/*   Updated: 2019/07/31 14:41:28 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LIBUI.h"

t_LIBUI_Edit LIBUI_CreateEdit(t_edit_constr input)
{
    t_LIBUI_Edit edit;

    edit.is_active = 0;
    edit.x = input.x;
    edit.y = input.y;
    ft_strcpy(edit.function, input.function);
    ft_strcpy(edit.text, input.text);
    edit.surface = SDL_CreateRGBSurface(0, 100, 30, 32,0,0,0,0);
    SDL_FillRect(edit.surface, &((SDL_Rect){0, 0, 100, 30}), input.color);
    SDL_FillRect(edit.surface, &((SDL_Rect){2, 2, 96, 26}), 0x00aaaaaa);

    edit.active = SDL_CreateRGBSurface(0, 100, 30, 32,0,0,0,0);
    SDL_FillRect(edit.active, &((SDL_Rect){0, 0, 100, 30}), input.color);
    SDL_FillRect(edit.active, &((SDL_Rect){2, 2, 96, 26}), 0x00ffffff);

    TTF_Init();
    SDL_Surface *text;
    TTF_Font *font = TTF_OpenFont("lato/Lato-Bold.ttf", 12);
    text = TTF_RenderText_Blended(font, input.text, (SDL_Color){0, 0, 0, 0});
    TTF_CloseFont(font);
    TTF_Quit();

    edit.with_text = SDL_CreateRGBSurface(0, 100, 30, 32,0,0,0,0);
    edit.with_text_active = SDL_CreateRGBSurface(0, 100, 30, 32,0,0,0,0);
    SDL_BlitSurface(edit.surface, NULL, edit.with_text, NULL);
    SDL_BlitSurface(edit.active, NULL, edit.with_text_active, NULL);

    SDL_BlitSurface(text, &((SDL_Rect){ -8 , -8 ,100, 30 }), edit.with_text, NULL);
    SDL_BlitSurface(text, &((SDL_Rect){ -8 , -8 ,100, 30 }), edit.with_text_active, NULL);

    SDL_FreeSurface(text);
    return(edit);
}

void LIBUI_NewEdit(t_edit_constr edit, t_LIBUI_Edit *edits, int *c_edits)
{
    edits[*c_edits] = LIBUI_CreateEdit(edit);
    (*c_edits)++;
}

int LIBUI_IsEditPressed(int x, int y, t_LIBUI_Edit *edits, int c_edits)
{
    int i = 0;
    while (i < c_edits)
    {
        if ((edits[i]).x < x && (edits[i]).y < y && (edits[i]).y + 30 > y && (edits[i]).x + 100 > x)
        {
            edits[i].is_active = 1;
            return(i);
        }
        i++;
    }
    return (-1);
}


void LIBUI_EditRefresh(t_LIBUI_Edit *edits, int number)
{
    TTF_Init();
    SDL_Surface *text;
    TTF_Font *font = TTF_OpenFont("lato/Lato-Bold.ttf", 12);
    text = TTF_RenderText_Blended(font, edits[number].text, (SDL_Color){0, 0, 0, 0});
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_FreeSurface(edits[number].with_text);
    SDL_FreeSurface(edits[number].with_text_active);

    edits[number].with_text = SDL_CreateRGBSurface(0, 100, 30, 32,0,0,0,0);
    edits[number].with_text_active = SDL_CreateRGBSurface(0, 100, 30, 32,0,0,0,0);
    SDL_BlitSurface(edits[number].surface, NULL, edits[number].with_text, NULL);
    SDL_BlitSurface(edits[number].active, NULL, edits[number].with_text_active, NULL);

    SDL_BlitSurface(text, &((SDL_Rect){ -8 , -8 ,100, 30 }), edits[number].with_text, NULL);
    SDL_BlitSurface(text, &((SDL_Rect){ -8 , -8 ,100, 30 }), edits[number].with_text_active, NULL);
    SDL_FreeSurface(text);
}