#include "./bsp.h"

/*
	алгоритм брейзенхема или кого там
*/

void draw_line(int *pixels, int x1, int y1, int x2, int y2, int color)
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;

	int error = deltaX - deltaY;

	put_pixel(pixels, x2, y2, color);

	while (x1 != x2 || y1 != y2)
	{
		put_pixel(pixels, x1, y1, color);
		const int error2 = error * 2;

		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

/*
	получаем индекс контура (i) и индекс второй точки нужной нам стены в этом контуре (j)
	определяем нормаль (нормаль определяется из направления обхода точек контура при его рисовании:
	если контур рисуется по часовой стрелке, то нормаль наружу, если против - внутрь. направление нормали можно
	изменить, поменяв normal_dir с 1 на -1. в эдиторе это делается на клавишу n)
*/

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

/*
	новый контур
*/

void map_new_circuit(t_map *map)
{
	map->circuits[map->circuits_count].points_count = 0;
	map->circuits[map->circuits_count].walls_count = 0;
	map->circuits[map->circuits_count].ceil = DEFAULT_CEIL;
	map->circuits[map->circuits_count].floor = DEFAULT_FLOOR;

	/*
		направление нормали
		по умолчаннию
	*/

	map->circuits[map->circuits_count].normal_dir = 1;

	(map->circuits_count)++;
}

t_vertex	get_nearest(t_vertex f, t_vertex s, float x, float y)
{
	t_vertex	ret;
	t_vertex	line;
	t_vertex	ort;

	line = get_line_by_points(f, s);
	ort = get_ort_line_by_point(line, (t_vertex){x, y, 0.0});
	ret = lines_intersect(line, ort);

	return (ret);
}

void map_new_point(t_map *map, float x, float y, int flag)
{
	int index;

	/*
		если мы не в режиме рисования контура, создаем новый и входим в режим рисования
	*/

	if (!map->active)
	{
		if (map->circuits_count > 0 && !map->on_point && !map->on_line)
		{
			return ;
		}
		map_new_circuit(map);
		map->active = 1;
	}
	if (map->on_line && !flag)
	{
		t_vertex first = map->circuits[map->on_circuit_i].points[map->on_point_i];
		t_vertex second = map->circuits[map->on_circuit_i].points[
				get_i_plus_1(map->on_point_i, map->circuits[map->on_circuit_i].points_count)];
		t_vertex new = get_nearest(first, second, x, y);

		insert_by_index(map->circuits[map->on_circuit_i].points,
			get_i_plus_1(map->on_point_i, map->circuits[map->on_circuit_i].points_count),
			&map->circuits[map->on_circuit_i].points_count,
			new);
		x = new.x;
		y = new.y;
	}
	else if (map->on_point && !flag)
	{
		x = map->circuits[map->on_circuit_i].points[map->on_point_i].x;
		y = map->circuits[map->on_circuit_i].points[map->on_point_i].y;
	}

	index = map->circuits[map->circuits_count - 1].points_count;
	map->circuits[map->circuits_count - 1].integrated = 0;
	map->circuits[map->circuits_count - 1].points[index].x = x;
	map->circuits[map->circuits_count - 1].points[index].y = y;
	map->circuits[map->circuits_count - 1].points_count++;
}

t_wall	get_big_wall(t_wall wall, t_bsp *bsp, t_vertex *line)
{
	float a;
	float b;
	float c;

	a = wall.points[0].y - wall.points[1].y;
	b = wall.points[1].x - wall.points[0].x;
	c = wall.points[0].x * wall.points[1].y - wall.points[1].x * wall.points[0].y;

	bsp->line.x = -(a / b);
	bsp->line.y = -(c / b);
	bsp->line.z = 0.0;

	line->x = a;
	line->y = b;
	line->z = c;


	if (fabsf(a) > fabsf(b))
	{
		wall.points[0].y = -9999.0;
		wall.points[1].y = 9999.0;
		wall.points[0].x = (-b * wall.points[0].y - c) / a;
		wall.points[1].x = (-b * wall.points[1].y - c) / a;
	}
	else
	{
		wall.points[0].x = -9999.0;
		wall.points[1].x = 9999.0;
		wall.points[0].y = (-a * wall.points[0].x - c) / b;
		wall.points[1].y = (-a * wall.points[1].x - c) / b;
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

/*
	только для полного многоугольника
*/

int			if_can_cut(t_circuit *circuits, int circuits_count, int cir, int wall)
{
	int i;
	int j;
	t_vertex cross1;
	t_vertex cross2;
	t_vertex base;

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
				puts("НАЙДЕН КАТТЕР ПО ГРАНИЦЕ");
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


	p = wall.points[0];

	ort_line = get_ort_line_by_point(line, wall.points[0]);

	in = lines_intersect(line, ort_line);
	t_vertex ort1 = sub(p, in);

	p = wall.points[1];

	ort_line.z = -line.x * wall.points[1].y + line.y * wall.points[1].x;

	in = lines_intersect(line, ort_line);

	t_vertex ort2 = sub(p, in);

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
		circuits[i].failed = 0;
		if (circuits[i].walls_count < 2)
		{
			circuits[i].failed = 1;
		}
		j = 0;
		printf("КОНТУР %d\n", i);
		while (j < circuits[i].walls_count)
		{
			printf("	Стена %d %f %f	%f %f\n", j, circuits[i].walls[j].points[0].x, circuits[i].walls[j].points[0].y,
													circuits[i].walls[j].points[1].x, circuits[i].walls[j].points[1].y);
			j++;
		}
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

				puts("РАЗРЫВ");
				// printf("Разрыв точка 1: %f %f \n", p1.x, p1.y);
				// printf("Разрыв точка 2: %f %f \n", p2.x, p2.y);

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
				puts("КОМПЛАНАРНОЕ ГОВНО");

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
				puts("ТОЧЕЧНАЯ СТЕНА");

				//circuits[i].walls[j].points[1] = circuits[i].walls[get_i_plus_1(j, circuits[i].walls_count)].points[1];
				delete_wall_by_index(circuits[i].walls, j, &circuits[i].walls_count);
				j--;
			}
			j++;
		}
		circuits[i].failed = 0;
		if (circuits[i].walls_count < 3)
		{
			circuits[i].failed = 1;
		}
		i++;
	}
}

// int		get_failed_circuit(t_circuit *circuits, int circuits_count)
// {
// 	int i;

// 	i = 0;
// 	while (i < circuits_count)
// 	{
// 		if (circuits[i].failed)
// 			return (i);
// 		i++;
// 	}
// 	return (-1);
// }

// void	delete_failed_circuits(t_circuit *circuits, int *circuits_count)
// {
// 	while (get_failed_circuit(circuits, *circuits_count) != -1)
// 	{
// 		delete_circuit_by_index(circuits,
// 					get_failed_circuit(circuits, *circuits_count),
// 					circuits_count);
// 	}
	
// }

void	bsp_recurse(t_bsp *bsp, t_circuit *circuits, int circuits_count, t_map *map)
{
	t_circuit	*front;
	t_circuit	*back;
	int		front_count;
	int		back_count;
	int		i;
	int		j;
	int		result;
	int		cutter_cir;
	int		cutter_wall;
	t_wall	cutter;

	t_wall	new1;
	t_wall	new2;


	t_vertex cutter_line;
	

	reconstruct_circuits(circuits, circuits_count);
//	delete_failed_circuits(circuits, &circuits_count);

	get_cutter(circuits, circuits_count, &cutter_cir, &cutter_wall);

	// printf("cutter_cir\t%d\n\n", cutter_cir);
	// printf("cutter_wall\t%d\n\n", cutter_wall);

	//printf("%f\t%f\t\t%f\t%f\n", circuits[cutter_cir].walls[cutter_wall].points[0].x, circuits[cutter_cir].walls[cutter_wall].points[0].y,
	//							circuits[cutter_cir].walls[cutter_wall].points[1].x, circuits[cutter_cir].walls[cutter_wall].points[1].y);

	front = NULL;
	back = NULL;

	bsp->front = NULL;
	bsp->back = NULL;

	if (cutter_cir == -1)
	{
		bsp->walls_count = 0;
		bsp->is_leaf = 1;
		i = 0;
		while (i < circuits_count) // наверное можно оптимизировать, ведь все стены в одном контуре
		{
			j = 0;
			while (j < circuits[i].walls_count)
			{
				bsp->floor = map->circuits[i].floor;
				bsp->ceil = map->circuits[i].ceil;

				printf("Нумер контура был: %d\n", bsp->circuit);
				bsp->circuit = i;
				printf("Нумер контура: %d\n", i);
				bsp->walls[bsp->walls_count] = circuits[i].walls[j];
				bsp->walls_count++;
				j++;
			}
			i++;
		}
		puts("ВСЕ");
		free (circuits);
		return ;
	}

	cutter = get_big_wall(circuits[cutter_cir].walls[cutter_wall], bsp, &cutter_line);
	// cutter_line = get_line_by_points(circuits[cutter_cir].walls[cutter_wall].points[0],
	// 				circuits[cutter_cir].walls[cutter_wall].points[1]);
	// bsp->line = cutter_line;

	front = malloc(sizeof(t_circuit) * circuits_count);
	back = malloc(sizeof(t_circuit) * circuits_count);

	ft_bzero(front, sizeof(t_circuit) * circuits_count);
	ft_bzero(back, sizeof(t_circuit) * circuits_count);
	//printf("%f\t%f\t\t%f\t%f\n", cutter.points[0].x, cutter.points[0].y, cutter.points[1].x, cutter.points[1].y);

	bsp->is_leaf = 0;
	bsp->walls_count = 0;
	bsp->normal = circuits[cutter_cir].walls[cutter_wall].normal;


	bsp->back = malloc(sizeof(t_bsp));
	bsp->front = malloc(sizeof(t_bsp));

	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			t_vertex inter;
			result = classify_wall(cutter_line, bsp->normal, circuits[i].walls[j], &inter);


			if (result == CUTTED)
			{
				new1 = circuits[i].walls[j];
				new2 = circuits[i].walls[j];

				puts("cutted");
				new1.points[0] = circuits[i].walls[j].points[0];
				// new1.points[1] = lines_intersect_p(cutter.points[0], cutter.points[1],
				// 	circuits[i].walls[j].points[0], circuits[i].walls[j].points[1]);
				new1.points[1] = inter;
				new2.points[0] = new1.points[1];
				new2.points[1] = circuits[i].walls[j].points[1];

					// printf("a: %f\tb: %f\n", -line.x/line.y, -line.z/line.y);
					// printf("p1: %f\tpy: %f\n", new1.points[0].x, new1.points[0].y);
					// printf("p1: %f\tpy: %f\n", new2.points[1].x, new2.points[1].y);

					// printf("in: %f\tiny: %f\n", inter.x, inter.y);

				result = classify_wall(cutter_line, bsp->normal, new1, &inter);
				// printf("new1 %d\n", result);
				if (result == FRONT)
				{
					front[i].walls[front[i].walls_count] = new1;
					front[i].walls_count++;
					back[i].walls[back[i].walls_count] = new2;
					back[i].walls_count++;
				}
				else
				{
					front[i].walls[front[i].walls_count] = new2;
					front[i].walls_count++;
					back[i].walls[back[i].walls_count] = new1;
					back[i].walls_count++;	
				}
			}
			else if (result == FRONT)
			{
				puts("front");
				front[i].walls[front[i].walls_count] = circuits[i].walls[j];
				front[i].walls_count++;
			}
			else if (result == BACK)
			{
				puts("back");
				back[i].walls[back[i].walls_count] = circuits[i].walls[j];
				back[i].walls_count++;
			}
			else
			{
				puts("complanar");
				cutter = circuits[cutter_cir].walls[cutter_wall];
				//if (circuits[i].walls[j].points[0].x == cutter.points[1].x &&
				//	circuits[i].walls[j].points[1].y == cutter.points[0].y)
				circuits[cutter_cir].walls[cutter_wall].border_id = -1;
				circuits[i].walls[j].border_id = -1;

				// if ((length(sub(circuits[i].walls[j].points[0], cutter.points[1])) < 0.01 &&
				// 	length(sub(circuits[i].walls[j].points[1], cutter.points[0])) < 0.01))
				// {
				// 	puts("complanar-back");
				// 	circuits[i].walls[j].used_in_bsp = 1;
				// 	if (circuits[i].walls[j].type == WALL_TYPE_SECTOR_BORDER)
				// 	{
				// 		circuits[cutter_cir].walls[cutter_wall].border_id = map->border_id;
				// 		circuits[i].walls[j].border_id = map->border_id;
				// 		map->border_id++;
				// 		circuits[i].walls[j].circuit = cutter_cir;//правильно ли здесь пишется
				// 		puts("Отмечаю промежуточек");
				// 	}
						
				// 	back[i].walls[back[i].walls_count] = circuits[i].walls[j];
				// 	back[i].walls_count++;
				// }
				// else if ((length(sub(circuits[i].walls[j].points[0], cutter.points[0])) < 0.01 &&
				// 	length(sub(circuits[i].walls[j].points[1], cutter.points[1])) < 0.01))
				// {
				// 	puts("complanar-front");
				// 	circuits[i].walls[j].used_in_bsp = 1;
				// 	if (circuits[i].walls[j].type == WALL_TYPE_SECTOR_BORDER)
				// 	{
				// 		circuits[i].walls[j].circuit = -1;//правильно ли здесь пишется
				// 		// puts("Отмечаю промежуточек");
				// 	}
						
				// 	front[i].walls[front[i].walls_count] = circuits[i].walls[j];
				// 	front[i].walls_count++;
				// }
				if (i != cutter_cir)
				{
					puts("complanar-back");
					circuits[i].walls[j].used_in_bsp = 1;
					if (circuits[i].walls[j].type == WALL_TYPE_SECTOR_BORDER)
					{
						circuits[i].walls[j].circuit = cutter_cir;//правильно ли здесь пишется
						puts("Отмечаю промежуточек");
					}
					back[i].walls[back[i].walls_count] = circuits[i].walls[j];
					back[i].walls_count++;
				}
				else 
				{
					puts("complanar-front");
					if (circuits[i].walls[j].type == WALL_TYPE_SECTOR_BORDER)
						circuits[i].walls[j].circuit = -1;
					circuits[i].walls[j].used_in_bsp = 1;
					front[i].walls[front[i].walls_count] = circuits[i].walls[j];
					front[i].walls_count++;
				}
			}
			j++;
		}
		i++;
	}
	free(circuits);
	bsp_recurse(bsp->back, back, circuits_count, map);
	bsp_recurse(bsp->front, front, circuits_count, map);
}

void	bsp_compile(t_map_editor *ed)
{
	t_circuit *circuits;

	circuits = malloc(sizeof(t_circuit) * ed->map.circuits_count);///////

	

	transform_data(ed);
	ft_memcpy(circuits, ed->map.circuits, sizeof(t_circuit) * ed->map.circuits_count);
	bsp_recurse(&ed->root, circuits, ed->map.circuits_count, &ed->map);
}

void pts_rev(t_point *pts, int pts_count)
{
	int i;
	t_point swap;

	i = 0;
	while (1)
	{
		if (i - pts_count + i + 1 >= 0)
			break;
		swap = pts[i];
		pts[i] = pts[pts_count - i - 1];
		pts[pts_count - i - 1] = swap;
		i++;
	}
}

/*
	коллбэк событий
*/

int		cursor_on_point(t_map_editor *ed)
{
	int i;
	int j;
	float x;
	float y;
	int max;

	i = 0;
	max = ed->map.circuits_count;
	if (ed->map.active)
		max--;

	while (i < max)
	{
		j = 0;
		while (j < ed->map.circuits[i].points_count)
		{
			x = ed->map.circuits[i].points[j].x * MAP_EDITOR_SCALE + W_2;
			y = H_2 - ed->map.circuits[i].points[j].y * MAP_EDITOR_SCALE;
			if ((ed->prev_x - x) * (ed->prev_x - x) +
				(ed->prev_y - y) * (ed->prev_y - y) <= 16)
			{
				ed->map.on_line = 0;
				ed->map.on_point = 1;
				ed->map.on_circuit_i = i;
				ed->map.on_point_i = j;
				return 1;
			}
			j++;
		}
		i++;
	}
	return 0;
}

int circle_with_cut(t_vertex a, t_vertex b, t_vertex m)
{
	float aa = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
	float bb = 2 * ((b.x - a.x) * (a.x - m.x) + (b.y - a.y) * (a.y - m.y));
	float cc = m.x * m.x  + m.y * m.y + a.x * a.x +
		a.y * a.y - 2 * (m.x * a.x + m.y * a.y) - 16;

	if ( -bb < 0)
	{
		return (cc < 0);
	}

	if ( -bb < (2 * aa))
	{
		return (4 * aa * cc - bb * bb < 0);
	}

	return (aa + bb + cc < 0);

}

int		cursor_on_line(t_map_editor *ed)
{
	int i;
	int j;
	t_vertex a;
	t_vertex b;
	int max;

	i = 0;
	max = ed->map.circuits_count;
	if (ed->map.active)
		max--;

	while (i < max)
	{
		j = 0;
		while (j < ed->map.circuits[i].points_count)
		{
			a = ed->map.circuits[i].points[get_i_minus_1(j, ed->map.circuits[i].points_count)];
			b = ed->map.circuits[i].points[j];

			a.x = a.x * MAP_EDITOR_SCALE + W_2;
			b.x = b.x * MAP_EDITOR_SCALE + W_2;
			a.z = 0;
			b.z = 0;
			a.y = H_2 - a.y * MAP_EDITOR_SCALE;
			b.y = H_2 - b.y * MAP_EDITOR_SCALE;

			if (circle_with_cut(a, b, (t_vertex){ed->prev_x, ed->prev_y, 0.0}))
			{
				ed->map.on_line = 1;
				ed->map.on_point = 0;
				ed->map.on_circuit_i = i;
				ed->map.on_point_i = get_i_minus_1(j, ed->map.circuits[i].points_count);
				return 1;
			}
				
			j++;
		}
		i++;
	}
	return 0;
}
int classify_point(t_vertex cam, t_vertex line, t_vertex normal)
{
	t_vertex new;

	cam.z = 0.0;
	normal.z = 0.0;

	new = (t_vertex){cam.x, line.x * cam.x + line.y, 0.0};

	if (dot(sub(new, cam), normal) < 0.0)
		return (BACK);
	return (FRONT);
}


int		bsp_select_circuit_traversal(t_bsp *node, t_vertex pos)
{
	if (node->is_leaf)
	{
		return (node->circuit);
	}
	else
	{
		if (classify_point(pos, node->line, node->normal) == BACK)
		{
			return (bsp_select_circuit_traversal(node->front, pos));
		}
		else
		{
			return (bsp_select_circuit_traversal(node->back, pos));
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
		if (cursor_on_point(ed))
		{
			SDL_FreeSurface(ed->cursor_surface);
			SDL_FreeCursor(ed->cursor);
			ed->cursor_surface = SDL_LoadBMP("../textures/cursor-2.bmp");
			ed->cursor = SDL_CreateColorCursor(ed->cursor_surface, 5, 5);
			SDL_SetCursor(ed->cursor);

		}
		else if (cursor_on_line(ed))
		{
			SDL_FreeSurface(ed->cursor_surface);
			SDL_FreeCursor(ed->cursor);
			ed->cursor_surface = SDL_LoadBMP("../textures/cursor.bmp");
			ed->cursor = SDL_CreateColorCursor(ed->cursor_surface, 5, 5);
			SDL_SetCursor(ed->cursor);
		}
		else
		{
			ed->map.on_line = 0;
			ed->map.on_point = 0;

			SDL_FreeCursor(ed->cursor);
			ed->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
			SDL_SetCursor(ed->cursor);
		}
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		ed->edit_floor = 0;
		ed->edit_ceil = 0;
		if (ed->mode == RENDER_MODE_WALLS)
		{
			ed->map.selected_circuit = bsp_select_circuit_traversal(&ed->root, 
				(t_vertex){(float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
						(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0.0});
			printf("Выбран контур %d\n", ed->map.selected_circuit);
			printf("Содержит %d стен\n", ed->map.circuits[ed->map.selected_circuit].walls_count);
			printf("Пол %f \n", ed->map.circuits[ed->map.selected_circuit].floor);
			printf("Потолокл %f \n\n", ed->map.circuits[ed->map.selected_circuit].ceil);



			return ;
		}

		/*
			новая точка
		*/

		map_new_point(&(ed->map), (float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
						(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE, 0);
		printf("x: %f\ty: %f\n", (float)(ed->prev_x - W_2) / MAP_EDITOR_SCALE,
						(float)(H_2 - ed->prev_y) / MAP_EDITOR_SCALE);
	}
	else if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_EQUALS)
		{
			if (ed->edit_ceil)
			{
				ed->map.circuits[ed->map.selected_circuit].ceil += 0.1;
				printf("ceil: %f\n", ed->map.circuits[ed->map.selected_circuit].ceil);
			}
			if (ed->edit_floor)
			{
				ed->map.circuits[ed->map.selected_circuit].floor += 0.1;
				printf("floor: %f\n", ed->map.circuits[ed->map.selected_circuit].floor);
			}
		}
		else if (event->key.keysym.sym == SDLK_MINUS)
		{
			if (ed->edit_ceil)
			{
				ed->map.circuits[ed->map.selected_circuit].ceil -= 0.1;
				printf("ceil: %f\n", ed->map.circuits[ed->map.selected_circuit].ceil);
			}
			if (ed->edit_floor)
			{
				ed->map.circuits[ed->map.selected_circuit].floor -= 0.1;
				printf("floor: %f\n", ed->map.circuits[ed->map.selected_circuit].floor);
			}
		}
		else if (event->key.keysym.sym == SDLK_s)
		{
			puts("SAVE JSON");
			
			export_map(ed);
			save_json(&ed->root);
			ed->mode = RENDER_MODE_TRS;
		}
		else if (event->key.keysym.sym == SDLK_f)//пол
		{
			if (ed->mode == RENDER_MODE_WALLS)
			{
				ed->edit_floor = 1;
				ed->edit_ceil = 0;
				puts("Режим редактирования пола");
			}
			
		}
		else if (event->key.keysym.sym == SDLK_p)//потолок
		{
			if (ed->mode == RENDER_MODE_WALLS)
			{
				ed->edit_floor = 0;
				ed->edit_ceil = 1;
				puts("Режим редактирования потолка");

			}
		}
		else if (event->key.keysym.sym == SDLK_e)
		{
			/*
				завершить контур (замкнуть и выйти из режима рисования контура)
			*/
			map_new_point(&ed->map, ed->map.circuits[ed->map.circuits_count - 1].points[0].x,
						  ed->map.circuits[ed->map.circuits_count - 1].points[0].y, 1);
			ed->map.active = 0;
		}
		else if (event->key.keysym.sym == SDLK_n)
		{
			/*
				инвертировать нормаль рисуемого сейчас контура
			*/
			if (ed->map.active)
			{
				ed->map.circuits[ed->map.circuits_count - 1].normal_dir *= -1;
			}
		}
		else if (event->key.keysym.sym == SDLK_c)
		{
			/*
				все сбросить
			*/
			ed->map.active = 0;
			ed->map.circuits_count = 0;
		}
		else if (event->key.keysym.sym == SDLK_b)
		{
			/* здеся должен быть вызов bsp-компилятора */
			// triangulate(ed);
			bsp_compile(ed);
			ed->mode = RENDER_MODE_WALLS;

		}
	}
}

void bsp_trs_draw_traversal(t_bsp *node, int *pixels)
{
	t_int_v p1;
	t_int_v p2;

	int i;
	
	if (node->is_leaf)
	{
		i = 0;
		while (i < node->trs_count)
		{
			p1 = (t_int_v){node->trs[i].points[0].x * MAP_EDITOR_SCALE, node->trs[i].points[0].y * MAP_EDITOR_SCALE};
			p2 = (t_int_v){node->trs[i].points[1].x * MAP_EDITOR_SCALE, node->trs[i].points[1].y * MAP_EDITOR_SCALE};
			draw_line(pixels, p1.x, p1.y, p2.x, p2.y, 0xFFFFFF);

			p1 = (t_int_v){node->trs[i].points[1].x * MAP_EDITOR_SCALE, node->trs[i].points[1].y * MAP_EDITOR_SCALE};
			p2 = (t_int_v){node->trs[i].points[2].x * MAP_EDITOR_SCALE, node->trs[i].points[2].y * MAP_EDITOR_SCALE};
			draw_line(pixels, p1.x, p1.y, p2.x, p2.y, 0xFFFFFF);

			p1 = (t_int_v){node->trs[i].points[2].x * MAP_EDITOR_SCALE, node->trs[i].points[2].y * MAP_EDITOR_SCALE};
			p2 = (t_int_v){node->trs[i].points[0].x * MAP_EDITOR_SCALE, node->trs[i].points[0].y * MAP_EDITOR_SCALE};
			draw_line(pixels, p1.x, p1.y, p2.x, p2.y, 0xFFFFFF);

			i++;
		}
	}
	else
	{
		bsp_trs_draw_traversal(node->front, pixels);
		bsp_trs_draw_traversal(node->back, pixels);
	}
}

/*
	коллбэк обновления экрана
*/
void update(void *map_editor, int *pixels)
{
	t_map_editor *ed;
	int i;
	int j;

	t_int_v p1;
	t_int_v p2;

	ed = (t_map_editor *)map_editor;

	bzero(pixels, sizeof(int) * HxW);

	i = 0;
	while (i < ed->map.circuits_count)
	{
		j = 0;
		if (ed->mode == RENDER_MODE_POINTS)
			while (j < ed->map.circuits[i].points_count)
			{
				if (j == 0)
				{
					j++;
					continue;
				}

				p1.x = (int)(ed->map.circuits[i].points[j - 1].x * MAP_EDITOR_SCALE);
				p1.y = (int)(ed->map.circuits[i].points[j - 1].y * MAP_EDITOR_SCALE);

				p2.x = (int)(ed->map.circuits[i].points[j].x * MAP_EDITOR_SCALE);
				p2.y = (int)(ed->map.circuits[i].points[j].y * MAP_EDITOR_SCALE);

				draw_line(pixels, p1.x, p1.y, p2.x, p2.y, 0xFFFFFF);

				draw_line(pixels, p1.x + (p2.x - p1.x) / 2, p1.y + (p2.y - p1.y) / 2,
						p1.x + (p2.x - p1.x) / 2 + 30 * cos(atan2(p2.y - p1.y, p2.x - p1.x) + (float)ed->map.circuits[i].normal_dir * M_PI / 2),
						p1.y + (p2.y - p1.y) / 2 + 30 * sin(atan2(p2.y - p1.y, p2.x - p1.x) + (float)ed->map.circuits[i].normal_dir * M_PI / 2),
						0xff0000);
				j++;
			}
		else if (ed->mode == RENDER_MODE_WALLS)
			while (j < ed->map.circuits[i].walls_count)
			{
				p1.x = (int)(ed->map.circuits[i].walls[j].points[0].x * MAP_EDITOR_SCALE);
				p1.y = (int)(ed->map.circuits[i].walls[j].points[0].y * MAP_EDITOR_SCALE);

				p2.x = (int)(ed->map.circuits[i].walls[j].points[1].x * MAP_EDITOR_SCALE);
				p2.y = (int)(ed->map.circuits[i].walls[j].points[1].y * MAP_EDITOR_SCALE);

				t_vertex n = ed->map.circuits[i].walls[j].normal;

				int color = (i == ed->map.selected_circuit ? 0xFF0000 : 0xFFFF00);

				draw_line(pixels, p1.x, p1.y, p2.x, p2.y, color);

				draw_line(pixels,
					p1.x + (p2.x - p1.x) / 2,
					p1.y + (p2.y - p1.y) / 2,
					p1.x + (p2.x - p1.x) / 2 + 30 * n.x,
					p1.y + (p2.y - p1.y) / 2 + 30 * n.y,
					0xff0000);
				j++;
			}
		else
			bsp_trs_draw_traversal(&ed->root, pixels);
		
		i++;
	}
}

void map_init(t_map *map)
{
	map->circuits_count = 0;
	map->active = 0;
	map->on_line = 0;
	map->on_point = 0;
	map->border_id = 0;
}



int main(int ac, char **av)
{
	t_map_editor map_editor;

	map_editor.mode = RENDER_MODE_POINTS;
	map_editor.edit_ceil = 0;
	map_editor.edit_floor = 0;
	/*
		тут маленькая "библиотека" - MyGraphicsLib
		чтобы избавить основной код от прямого взаимодействия с SDL
	*/

	t_mgl *mgl = mgl_init("Map Editor BSP", W, H);

	mgl->show_fps = 0;


	map_editor.cursor_surface = SDL_LoadBMP("../textures/cursor.bmp");
	map_editor.cursor = SDL_CreateColorCursor(map_editor.cursor_surface, 0, 0);
	SDL_SetCursor(map_editor.cursor);


	map_init(&map_editor.map);

	/*
		запускаем цикл отрисовки
		туда отправляем два коллбэка (указатели на функцию)

		они вызываются изнтри этой "библиотеки"

		первый выполняется при каждом обновлении экрана
		второй - при каждом отлове события

		и туда же отправляем структуру, в которой всё))))))
	*/

	mgl_run(mgl, update, event_handle, &map_editor);

	mgl_quit(mgl);
	return (0);
}
