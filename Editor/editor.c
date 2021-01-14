#include "./editor.h"

t_vertex get_face_normal(t_map_editor *editor, int i, int j)
{
	t_vertex ret;

	t_vertex p1;
	t_vertex p2;

	p1 = editor->map.circuits[i].points[j - 1];
	p2 = editor->map.circuits[i].points[j];

	ret.x = cos(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);

	ret.y = sin(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);
	ret.z = 0.0;

	return (normalize(ret));
}


t_wall	get_big_wall(t_wall wall, t_bsp *bsp, t_vertex *line)
{
	line->x = wall.points[0].y - wall.points[1].y;
	line->y = wall.points[1].x - wall.points[0].x;
	line->z = wall.points[0].x * wall.points[1].y - wall.points[1].x * wall.points[0].y;
	bsp->line.x = -(line->x / line->y);
	bsp->line.y = -(line->z / line->y);
	bsp->line.z = 0.0;

	if (fabsf(line->x) > fabsf(line->y))
	{
		wall.points[0].y = -9999.0;
		wall.points[1].y = 9999.0;
		wall.points[0].x = (-line->y * wall.points[0].y - line->z) / line->x;
		wall.points[1].x = (-line->y * wall.points[1].y - line->z) / line->x;
	}
	else
	{
		wall.points[0].x = -9999.0;
		wall.points[1].x = 9999.0;
		wall.points[0].y = (-line->x * wall.points[0].x - line->z) / line->y;
		wall.points[1].y = (-line->x * wall.points[1].x - line->z) / line->y;
	}
	return (wall);
}

int sign(float a)
{
	if (a == 0.0)
		return (0);
	if (a > 0.0)
		return (1);
	return (-1);
}

int		walls_cmp(t_wall w1, t_wall w2)
{
	if ((w1.points[0].x == w2.points[0].x && w1.points[0].y == w2.points[0].y &&
		w1.points[1].x == w2.points[1].x && w1.points[1].y == w2.points[1].y) || 
		(w1.points[0].x == w2.points[1].x && w1.points[0].y == w2.points[1].y &&
		w1.points[1].x == w2.points[0].x && w1.points[1].y == w2.points[0].y))
		return (1);
	return (0);
}

void	analyze_wall(t_wall *wall, t_map_editor *ed, int n)
{
	int i;
	int j;

	i = 0;
	while (i < ed->map.circuits_count)
	{
		if (i == n)
		{
			i++;
			continue ;
		}
		j = 0;
		while (j < ed->map.circuits[i].walls_count)
		{
			if (walls_cmp(ed->map.circuits[i].walls[j], *wall))
			{
				wall->type = WALL_TYPE_SECTOR_BORDER;
			}
			j++;
		}
		i++;
	}
}

void	transform_data(t_map_editor *ed)
{
	t_wall	curr;
	int		walls_count;
	int		i;
	int		j;

	i = 0;
	while (i < ed->map.circuits_count)
	{
		j = 1;
		walls_count = 0;
		while (j < ed->map.circuits[i].points_count)
		{
			curr.points[0] = ed->map.circuits[i].points[j - 1];
			curr.points[0].z = 0.0;

			curr.points[1] = ed->map.circuits[i].points[j];
			curr.points[1].z = 0.0;

			curr.normal = get_face_normal(ed, i, j);
			curr.used_in_bsp = 0;

			curr.type = WALL_TYPE_WALL;

			ed->map.circuits[i].walls[walls_count] = curr;
			walls_count++;
			j++;
		}
		ed->map.circuits[i].walls_count = walls_count;
		i++;
	}
	i = 0;
	while (i < ed->map.circuits_count)
	{
		j = 0;
		while (j < ed->map.circuits[i].walls_count)
		{
			analyze_wall(&ed->map.circuits[i].walls[j], ed, i);
			j++;
		}
		i++;
	}
}

int			if_can_cut(t_circuit *circuits, int circuits_count, int cir, int wall)
{
	int			i;
	int			j;
	t_vertex	base;

	int res[2];
	res[0] = 0;
	res[1] = 0;
	base = sub(circuits[cir].walls[wall].points[1], circuits[cir].walls[wall].points[0]);
	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			if (cross(base, sub(circuits[i].walls[j].points[1],
					circuits[cir].walls[wall].points[0])).z > 0)
				res[0] = 1;
			if (cross(base, sub(circuits[i].walls[j].points[1],
					circuits[cir].walls[wall].points[0])).z < 0)
				res[1] = 1;
			if (res[0] && res[1])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

void		get_cutter(t_circuit *circuits, int circuits_count, int *cutter_cir, int *cutter_wall)
{
	int i;
	int j;

	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			if (circuits[i].walls[j].type == WALL_TYPE_SECTOR_BORDER &&
				!circuits[i].walls[j].used_in_bsp && length(circuits[i].walls[j].normal))
			{
				*cutter_cir = i;
				*cutter_wall = j;
				return ;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			if (!circuits[i].walls[j].used_in_bsp && length(circuits[i].walls[j].normal) &&
				if_can_cut(circuits, circuits_count, i, j))
			{
				*cutter_cir = i;
				*cutter_wall = j;
				return ;
			}
			j++;
		}
		i++;
	}
	*cutter_cir = -1;
	*cutter_wall = -1;
}


int		classify_wall(t_vertex line, t_vertex n, t_wall wall, t_vertex *inter)
{
	t_vertex p;
	t_vertex ort_line;
	t_vertex in;
	t_vertex ort1;
	t_vertex ort2 ;

	p = wall.points[0];
	ort_line = get_ort_line_by_point(line, wall.points[0]);
	in = lines_intersect(line, ort_line);
	ort1 = sub(p, in);
	p = wall.points[1];
	ort_line.z = -line.x * wall.points[1].y + line.y * wall.points[1].x;
	in = lines_intersect(line, ort_line);
	ort2 = sub(p, in);
	float dot1 = dot(normalize(ort1), normalize(n));
	float dot2 = dot(normalize(ort2), normalize(n));
	if (length(ort1) < 0.01 && length(ort2) < 0.01)
		return (COMPLANAR);
	else if (((dot1 < 0.0 && dot2 > 0.0) || (dot1 > 0.0 && dot2 < 0.0)) && 
				length(ort1) > 0.01 && length(ort2) > 0.01)
	{
		ort_line = get_line_by_points(wall.points[0], wall.points[1]);
		*inter = lines_intersect(line, ort_line);
		return (CUTTED);
	}
	else if ((length(ort1) > 0.01 && dot1 > 0.0) ||
				(length(ort2) > 0.01 && dot2 > 0.0))
		return (FRONT);
	else
		return (BACK);
}

void	reconstruct_circuits(t_circuit *circuits, int circuits_count)
{
	int i;
	int j;

	t_vertex p1;
	t_vertex p2;

	t_wall wall;

	wall.type = WALL_TYPE_FICTIVE;
	wall.used_in_bsp = 1;

	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			p1 = circuits[i].walls[get_i_plus_1(j, circuits[i].walls_count)].points[0];
			p2 = circuits[i].walls[j].points[1];
			if (!(length(sub(p1, p2)) < 0.01))
			{
				wall.points[0] = p2;
				wall.points[1] = p1;

				wall.normal = (t_vertex){0.0, 0.0, 0.0};

				insert_wall_by_index(circuits[i].walls, j + 1, &circuits[i].walls_count, wall);
			}	
			j++;
		}

		j = 0;
		while (j < circuits[i].walls_count)
		{
			if (fabsf(dot(normalize(sub(circuits[i].walls[j].points[1], circuits[i].walls[j].points[0])),
				normalize(sub(circuits[i].walls[get_i_plus_1(j, circuits[i].walls_count)].points[1],
				circuits[i].walls[get_i_plus_1(j, circuits[i].walls_count)].points[0])))) > 0.9999 && 
				((circuits[i].walls[get_i_plus_1(j, circuits[i].walls_count)].type == WALL_TYPE_FICTIVE &&
				circuits[i].walls[j].type == WALL_TYPE_FICTIVE) ||
				(circuits[i].walls[get_i_plus_1(j, circuits[i].walls_count)].type == WALL_TYPE_WALL &&
				circuits[i].walls[j].type == WALL_TYPE_WALL)))
			{
				circuits[i].walls[j].points[1] = circuits[i].walls[get_i_plus_1(j, circuits[i].walls_count)].points[1];
				delete_wall_by_index(circuits[i].walls, get_i_plus_1(j, circuits[i].walls_count), &circuits[i].walls_count);
				j--;
			}
			j++;
		}
		j = 0;
		while (j < circuits[i].walls_count)
		{
			if (length(sub(circuits[i].walls[j].points[1], circuits[i].walls[j].points[0])) < 0.01)
			{
				delete_wall_by_index(circuits[i].walls, j, &circuits[i].walls_count);
				j--;
			}
			j++;
		}
		i++;
	}
}



void	bsp_compile(t_map_editor *ed)
{
	t_circuit *circuits;

	if (!(circuits = malloc(sizeof(t_circuit) * ed->map.circuits_count)))
		exit(-2);

	transform_data(ed);
	ft_memcpy(circuits, ed->map.circuits, sizeof(t_circuit) * ed->map.circuits_count);
	bsp_recurse(&ed->root, circuits, ed->map.circuits_count, &ed->map);
}

void	key_down(t_map_editor *ed, SDL_Event *event, int *quit)
{
	if (event->key.keysym.sym == SDLK_ESCAPE)
	{
		*quit = 0;
		exit(1);
	}
	else if (event->key.keysym.sym == SDLK_RETURN)
		key_enter_down(ed);
	else if (event->key.keysym.sym == SDLK_EQUALS)
		key_plus_down(ed);
	else if (event->key.keysym.sym == SDLK_MINUS)
		key_minus_down(ed);
	else if (event->key.keysym.sym == SDLK_1)
	{
		if (ed->step == STEP_4_TEXTURES)
		{
			ed->edit_ceil_tex = 0;
			ed->edit_wall_tex = 1;
			ed->edit_floor_tex = 0;
			write_wall_tex(ed);
		}
	}
	else if (event->key.keysym.sym == SDLK_2)
	{
		if (ed->step == STEP_4_TEXTURES)
		{
			ed->edit_ceil_tex = 0;
			ed->edit_wall_tex = 0;
			ed->edit_floor_tex = 1;
			write_floor_tex(ed);
		}
	}
	else if (event->key.keysym.sym == SDLK_3)
	{
		if (ed->step == STEP_4_TEXTURES)
		{
			ed->edit_ceil_tex = 1;
			ed->edit_wall_tex = 0;
			ed->edit_floor_tex = 0;
			write_ceil_tex(ed);
		}
	}
	else if (event->key.keysym.sym == SDLK_e)
	{
		if (ed->step == STEP_1_DRAW)
		{
			map_new_point(&ed->map, ed->map.circuits[ed->map.circuits_count - 1].points[0].x,
						ed->map.circuits[ed->map.circuits_count - 1].points[0].y, 1);
			ed->map.active = 0;
		}
	}
}

void event_handle(SDL_Event *event, void *ed_ptr, int *quit)
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
		key_down(ed, event, quit);
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
	t_mgl mgl = mgl_init("Map Editor BSP", W, H);
	mgl.show_fps = 0;
	map_editor.cursor_surface = SDL_LoadBMP("../textures/cursor.bmp");
	map_editor.cursor = SDL_CreateColorCursor(map_editor.cursor_surface, 0, 0);
	SDL_SetCursor(map_editor.cursor);
	editor_init(&map_editor);
	write_instructions(&map_editor);
	mgl_run(&mgl, update, event_handle, &map_editor);
	mgl_quit(&mgl);
	return (0);
}
