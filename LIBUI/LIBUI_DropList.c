/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIBUI_DropList.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/23 11:56:34 by krakharo          #+#    #+#             */
/*   Updated: 2019/06/29 11:42:22 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LIBUI.h"
/*
void		ft_redraw(void *rtv1_tmp)
{
	t_rtv1		*rtv1;
	SDL_Rect	rect;

	rtv1 = (t_rtv1*)rtv1_tmp;
	rect = (SDL_Rect){.x = rtv1->list->x, .y = rtv1->list->y};
	rect.w = 100;
	rect.h = 200;
	if (rtv1->list->expand == 1)
	{
		LIBUI_NewButton((t_but_constr){rtv1->list->x, rtv1->list->y + 30, \
	"Texture1", "Texture1", 0x0000ff55}, rtv1->buttons, &(rtv1->list->end));
		LIBUI_NewButton((t_but_constr){rtv1->list->x, rtv1->list->y + 60, \
	"Texture2", "Texture2", 0x0000ff55}, rtv1->buttons, &(rtv1->list->end));
		LIBUI_NewButton((t_but_constr){rtv1->list->x, rtv1->list->y + 90, \
	"Texture3", "Texture3", 0x0000ff55}, rtv1->buttons, &(rtv1->list->end));
	}
	else
	{
		rtv1->list->end = rtv1->list->start;
		SDL_FillRect(rtv1->ui, &rect, \
			SDL_MapRGB(rtv1->ui->format, 0xFF, 0xFF, 0x00));
	}
	rtv1->c_buttons = rtv1->list->end;
}*/
/*
void		LIBUI_drop_list(void *tmp_rtv1, int x, int y)
{
	t_rtv1	*rtv1;

	rtv1 = (t_rtv1*)tmp_rtv1;
	rtv1->list->x = x;
	rtv1->list->y = y;
	LIBUI_NewButton((t_but_constr){x, y, "List", "Expand", \
							0x0000ff55}, rtv1->buttons, &(rtv1->c_buttons));
	rtv1->list->expand = 0;
	rtv1->list->start = rtv1->c_buttons;
	rtv1->list->end = rtv1->c_buttons;
	rtv1->list->ft_redraw = ft_redraw;
	rtv1->list->ft_redraw(rtv1);
}
*/

t_list_item_addr LIBUI_IsListPressed(int x, int y, t_LIBUI_List *lists, int c_lists)
{
	int i;
	t_list_item_addr ret;

	i = 0;
	ret.item = -1;
	while (i < c_lists)
	{
		ret.list = i;
		ret.item = LIBUI_IsButtonPressed(x, y, lists[i].items, lists[i].c_items);
		if (!lists[i].is_dropped && ret.item != 0)
			ret.item = -1;
		if (ret.item != -1)
			break;
		i++;
	}
	return (ret);
}

t_LIBUI_List LIBUI_CreateList(t_list_constr input)
{
	t_LIBUI_List list;
	int i;
	t_but_constr tmp;


	list.x = input.x;
	list.y = input.y;
	list.is_dropped = 0;
	/*list.main.x = input.x;
	list.main.y = input.y;*/
	tmp.x = input.x;
	tmp.y = input.y;
	tmp.text = input.text;
	tmp.function = input.function;
	tmp.color = input.color;




	list.items = (t_LIBUI_Button *)malloc(sizeof(t_LIBUI_Button) * (input.c_items + 1));////////////
		list.items[0] = LIBUI_CreateButton(tmp);
	list.c_items = 1;

	i = 0;
	while (i < input.c_items)
	{
		tmp.x = list.x;
		tmp.y = list.y + 30 * (i + 1);
		tmp.text = input.items_text[i];
		tmp.function = input.items_function[i];
		tmp.color = input.color;
		LIBUI_NewButton(tmp, list.items, &list.c_items);
		//list.c_items++;
		i++;
	}

	return (list);

}



void LIBUI_NewList(t_list_constr list, t_LIBUI_List *lists, int *c_lists)
{

    lists[*c_lists] = LIBUI_CreateList(list);
    (*c_lists)++;
}