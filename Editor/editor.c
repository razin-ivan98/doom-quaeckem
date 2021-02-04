/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:42:13 by Chorange          #+#    #+#             */
/*   Updated: 2021/02/04 15:01:18 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./editor.h"

void	bsp_compile(t_map_editor *ed)
{
	t_circuit *circuits;

	if (!(circuits = malloc(sizeof(t_circuit) * ed->map.circuits_count)))
		exit(-2);

	transform_data(ed);
	ft_memcpy(circuits, ed->map.circuits, sizeof(t_circuit) *
				ed->map.circuits_count);
	bsp_recurse(&ed->root, circuits, ed->map.circuits_count, &ed->map);
}

void event_handle(SDL_Event *event, void *ed_ptr)
{
	t_map_editor *ed;

	ed = (t_map_editor *)ed_ptr;

	ed->prev_x = event->button.x * SCREEN_MULTIPLICATOR;
	ed->prev_y = event->button.y * SCREEN_MULTIPLICATOR;

	if (event->type == SDL_MOUSEMOTION)
	{
		mouse_motion(ed);
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		mouse_button_down(ed);
	}
	else if (event->type == SDL_KEYDOWN)
	{
		key_down(ed, event);
	}
}

void update(void *map_editor, int *pixels)
{
	t_map_editor	*ed;
	int				i;

	ed = (t_map_editor *)map_editor;
	ft_bzero(pixels, sizeof(int) * H * W);
	i = 0;
	while (i < ed->map.circuits_count)
	{
		if (ed->mode == RENDER_MODE_POINTS)
			draw_in_points_mode(i, pixels, map_editor);
		else if (ed->mode == RENDER_MODE_WALLS)
			draw_in_walls_mode(i, pixels, map_editor);
		else
			bsp_trs_draw_traversal(&ed->root, pixels);
		i++;
	}
	draw_elements(map_editor, pixels);
}

int		main(void)
{
	t_map_editor map_editor;
	t_mgl mgl;
	
	mgl = mgl_init("Map Editor BSP", W, H, SCREEN_MULTIPLICATOR);

	mgl.show_fps = 0;
	map_editor.cursor_surface = SDL_LoadBMP("../textures/cursor.bmp");
	map_editor.cursor =
			SDL_CreateColorCursor(map_editor.cursor_surface, 0, 0);
	SDL_SetCursor(map_editor.cursor);
	editor_init(&map_editor);
	write_instructions(&map_editor);
	mgl_run(&mgl, update, (void *)event_handle, (void *)&map_editor);
	mgl_quit(&mgl);
	return (0);
}
