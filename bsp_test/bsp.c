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

	ret.y = 0.0;
	ret.z = sin(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);

	return (normalize(ret));
}

/*
	новый контур
*/

void map_new_circuit(t_map *map)
{
	map->circuits[map->circuits_count].points_count = 0;

	/*
		направление нормали
		по умолчаннию
	*/

	map->circuits[map->circuits_count].normal_dir = 1;

	(map->circuits_count)++;
}
void map_new_point(t_map *map, float x, float y)
{
	int index;

	/*
		если мы не в режиме рисования контура, создаем новый и входим в режим рисования
	*/

	if (!map->active)
	{
		map_new_circuit(map);
		map->active = 1;
	}

	index = map->circuits[map->circuits_count - 1].points_count;
	map->circuits[map->circuits_count - 1].integrated = 0;
	map->circuits[map->circuits_count - 1].points[index].x = x;
	map->circuits[map->circuits_count - 1].points[index].y = y;
	map->circuits[map->circuits_count - 1].points_count++;
}

t_wall	get_big_wall(t_wall wall)
{
	float dx;

	if (wall.points[0].y == wall.points[1].y)
	{
		wall.points[0].x = -99999.0;
		wall.points[1].x = 99999.0;
	}
	else if (wall.points[0].x == wall.points[1].x)
	{
		wall.points[0].y = -99999.0;
		wall.points[1].y = 99999.0;
	}
	else
	{
		dx = (wall.points[1].x - wall.points[0].x) /
					(wall.points[1].y - wall.points[0].y);
		wall.points[0].y = -99999.0;
		wall.points[1].y = 99999.0;
		wall.points[0].x = -99999.0 * dx;
		wall.points[1].x = 99999.0 * dx;
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

int if_intersect(t_vertex v11, t_vertex v12, t_vertex v21, t_vertex v22)
{
	t_vertex cut1;
	t_vertex cut2;

	t_vertex prod1;
	t_vertex prod2;

	cut1 = sub(v12, v11);
	cut2 = sub(v22, v21);

	prod1 = cross(cut1, sub(v21, v11));
	prod2 = cross(cut1, sub(v22, v11));

	if (sign(prod1.z) == sign(prod2.z) || sign(prod1.z) == 0 || sign(prod2.z) == 0) // Отсекаем также и пограничные случаи
		return 0;

	prod1 = cross(cut2, sub(v11, v21));
	prod2 = cross(cut2, sub(v12, v21));

	if (sign(prod1.z) == sign(prod2.z) || sign(prod1.z) == 0 || sign(prod2.z) == 0) // Отсекаем также и пограничные случаи
		return 0;
	return 1;
}

void	transform_data(t_map_editor *ed, t_wall *walls, int *walls_count)
{
	t_wall	curr;
	int		i;
	int		j;
	
	*walls_count = 0;
	i = 0;
	//while (i < ed->map.circuits_count)
	//{
		j = 1;
		while (j < ed->map.circuits[i].points_count)
		{
			curr.points[0] = ed->map.circuits[i].points[j - 1];
			curr.points[1] = ed->map.circuits[i].points[j];
			curr.normal = get_face_normal(ed, i, j);
			curr.used_in_bsp = 0;

			walls[*walls_count] = curr;
			(*walls_count)++;
			j++;
		}
		i++;
	//}
	printf("%d\n", *walls_count);
}

int check_in_figure(t_vertex curr, t_vertex prev, t_vertex next, t_vertex test)
{
	t_vertex new;
	t_vertex base;
	t_vertex end;
	t_vertex sub1;
	t_vertex sub2;

	base = normalize(sub(prev, curr));
	new = normalize(sub(test, curr));
	end = normalize(sub(next, curr));

	float angle1 = acosf(dot(base, new));
	float angle2 = acosf(dot(base, end));

	if (cross(base, new).z < 0.0)
		angle1 = 2 * M_PI - angle1;
	if (cross(base, end).z < 0.0)
		angle2 = 2 * M_PI - angle2;

	if (angle2 <= angle1 && angle1 >= 0.0)
		return (1);
	return (0);
}



int get_i_plus_1(int i, int max)
{
	return ((i + 1) % max);
}

int get_i_minus_1(int i, int max)
{
	if (i != 0)
		return (i - 1);
	return (max - 1);
}

int get_i_plus_2(int i, int max)
{
	return ((i + 2) % max);
}

/*
	только для полного многоугольника
*/
int		is_convex_polygon(t_wall *walls, int walls_count)
{
	int i;
	t_vertex curr;
	t_vertex test;
	t_vertex prev;
	t_vertex next;

	i = 0;
	while (i < walls_count)
	{
		curr = walls[i].points[1];
		test = walls[get_i_plus_2(i, walls_count)].points[1];
		next = walls[get_i_plus_1(i, walls_count)].points[1];
		prev = walls[i].points[0];


		// printf("prev: %f %f     %f\n", prev.x, prev.y, prev.z);
		// printf("curr: %f %f     %f\n", curr.x, curr.y, curr.z);
		// printf("next: %f %f     %f\n", next.x, next.y, next.z);
		// printf("test: %f %f     %f\n\n\n", test.x, test.y, test.z);


		if (!check_in_figure(curr, prev, next, test))
		{
			return (0);
		}
		i++;
	}

	return (1);
}

int		get_cutter(t_wall *walls, int walls_count)
{
	int i;

	i = 0;
	while (i < walls_count)
	{
		//if (!walls[i].used_in_bsp && )
		i++;
	}
}

// void	bsp_recurse(t_bsp *bsp, t_wall *walls, int walls_count)
// {
// 	t_wall	*front;
// 	t_wall	*back;
// 	int		front_count;
// 	int		back_count;
// 	int		i;
// 	int		result;
// 	int		cutter;

// 	if (is_convex_polygon(walls, walls_count))
// 	{
// 		front = NULL;
// 		front = NULL;
// 		bsp->is_leaf = 1;
// 		bsp->walls_count = walls_count;
// 		bsp->walls = walls;
// 		return ;
// 	}
// 	bsp->is_leaf = 0;

// 	front = malloc(sizeof(t_wall) * 3000);
// 	back = malloc(sizeof(t_wall) * 3000);

// 	cutter = get_cutter(walls, walls_count);

	
// }

void	bsp_compile(t_map_editor *ed)
{
	t_wall	walls[300];
	int		walls_count;
	int		i;
	

	transform_data(ed, (t_wall *)walls, &walls_count);

	if (is_convex_polygon((t_wall *)walls, walls_count))
		puts("CONVEX");
	else
	{
		puts("NOT");
	}
	

	//bsp_recurse(&ed->root, (t_wall *)walls, walls_count);
}

void delete_by_index(t_point *points, int index, int *count)
{
	int i;

	i = index;
	(*count)--;
	while (i < *count)
	{
		points[i] = points[i + 1];
		i++;
	}
}

void insert_by_index(t_point *points, int index, int *count, t_point p)
{
	int i;

	i = *count;
	while (i > index)
	{
		points[i] = points[i - 1]; ///////////////
		i--;
	}
	points[index] = p;
	(*count)++;
}

int check_fictive_cut_in_figure(t_point *pts, int pts_count, int i, t_map_editor *ed, t_point new_p)
{
	t_vertex new;
	t_vertex base;
	t_vertex end;
	t_vertex sub1;
	t_vertex sub2;

	base = normalize((t_vertex){
		pts[get_i_minus_1(i, pts_count)].x - pts[i].x,
		pts[get_i_minus_1(i, pts_count)].y - pts[i].y,
		0.0});
	new = normalize((t_vertex){
		new_p.x - pts[i].x,
		new_p.y - pts[i].y,
		0.0});
	end = normalize((t_vertex){
		pts[get_i_plus_1(i, pts_count)].x - pts[i].x,
		pts[get_i_plus_1(i, pts_count)].y - pts[i].y,
		0.0});

	float angle1 = acosf(dot(base, new));
	float angle2 = acosf(dot(base, end));

	if (cross(base, new).z < 0.0)
		angle1 = 2 * M_PI - angle1;
	if (cross(base, end).z < 0.0)
		angle2 = 2 * M_PI - angle2;

	if (angle2 < angle1 && angle1 > 0.0)
		return (1);
	return (0);
}

int check_intersection(t_vertex new1, t_vertex new2, t_point *pts, int pts_count)
{
	int j;
	int k;
	t_vertex v1;
	t_vertex v2;

	j = 0;

	while (j < pts_count) //по еще не отсеченным
	{

		v1 = (t_vertex){
			pts[j].x,
			pts[j].y,
			0.0};
		v2 = (t_vertex){
			pts[get_i_plus_1(j, pts_count)].x,
			pts[get_i_plus_1(j, pts_count)].y,
			0.0};

		if (if_intersect(new1, new2, v1, v2) == 1)
			return (1);
		j++;
	}

	//по уже отсеченным

	return (-1);
}

int check_intersection_with_all(t_vertex new1, t_vertex new2, t_map_editor *ed)
{
	int i;
	t_point hole_pts[30];
	int hole_pts_count;

	i = 0;
	while (i < ed->map.circuits_count)
	{
		ft_memcpy(hole_pts, ed->map.circuits[i].points, sizeof(t_point) * (ed->map.circuits[i].points_count - 1));
		hole_pts_count = ed->map.circuits[i].points_count - 1;
		if (check_intersection(new1, new2, hole_pts, hole_pts_count) == 1)
			return (1);
		i++;
	}
	return (-1);
}

void fictive_cut(t_point *pts, int *pts_count, t_point *hole_pts, int hole_pts_count, int j, int k)
{
	int i;

	i = 0;
	//insert_by_index(pts, j + 1, pts_count, hole_pts[k]);
	while (i < hole_pts_count)
	{
		//	printf("i: %d\t j: %d\n",i,j);

		insert_by_index(pts, j + 1 + i, pts_count, hole_pts[k]);
		k = get_i_plus_1(k, hole_pts_count);
		i++;
	}
	insert_by_index(pts, j + 1 + i, pts_count, hole_pts[k]);
	insert_by_index(pts, j + 2 + i, pts_count, pts[j]);

	puts("ïnsert");
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


int integrate_hole(t_map_editor *ed, int i, t_point *pts, int *pts_count)
{
	int j;
	int k;
	t_point hole_pts[30];
	int hole_pts_count;
	t_vertex new1;
	t_vertex new2;

	ft_memcpy(hole_pts, ed->map.circuits[i].points, sizeof(t_point) * (ed->map.circuits[i].points_count - 1));
	hole_pts_count = ed->map.circuits[i].points_count - 1;

	if (ed->map.circuits[i].normal_dir == -1)
	{
		pts_rev(hole_pts, hole_pts_count);
	}

	j = 0;
	while (j < *pts_count)
	{
		new1 = (t_vertex){pts[j].x, pts[j].y, 0.0};
		k = 0;
		while (k < hole_pts_count)
		{
			new2 = (t_vertex){hole_pts[k].x, hole_pts[k].y, 0.0};

			if (check_intersection(new1, new2, pts, *pts_count) == -1 && check_intersection(new1, new2, hole_pts, hole_pts_count) == -1 && check_intersection_with_all(new1, new2, ed) == -1 && check_fictive_cut_in_figure(pts, *pts_count, j, ed, hole_pts[k]) == 1)
			{
				fictive_cut(pts, pts_count, hole_pts, hole_pts_count, j, k);
				ed->map.circuits[i].integrated = 1;
				puts("POK");
				return (1);
			}
			k++;
		}
		j++;
	}
	puts("GOVOVNO");

	return (0);
}

void triangulate(t_map_editor *ed)
{
	t_point pts[100];
	int pts_count;
	int i;

	int p;

	t_vertex new1;
	t_vertex new2;

	ft_memcpy(pts, ed->map.circuits[0].points, sizeof(t_point) * (ed->map.circuits[0].points_count - 1));
	pts_count = ed->map.circuits[0].points_count - 1;

	if (ed->map.circuits[0].normal_dir == -1)
	{
		pts_rev(pts, pts_count);
	}

	int flag;
	flag = 0;
	while (flag == 0)
	{
		flag = 1;
		i = 1;
		while (i < ed->map.circuits_count)
		{
			puts("EEEEE");
			if (ed->map.circuits[i].integrated == 0)
				if (integrate_hole(ed, i, pts, &pts_count) == 0)
					flag = 0;
			i++;
		}
	}

	printf("%d\n", pts_count);

	ft_memcpy(ed->map.circuits[0].points, pts, sizeof(t_point) * pts_count);
	ed->map.circuits[0].points[pts_count] = ed->map.circuits[0].points[0];
	ed->map.circuits_count = 1;
	ed->map.circuits[0].points_count = pts_count + 1;
}

/*
	коллбэк событий
*/

void event_handle(SDL_Event *event, void *ed_ptr, int *quit)
{
	t_map_editor *ed;

	ed = (t_map_editor *)ed_ptr;

	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		/*
			новая точка
		*/
		ed->prev_x = event->button.x * 2;
		ed->prev_y = event->button.y * 2;

		map_new_point(&(ed->map), (float)(ed->prev_x - W_2) / 100.0,
						(float)(H_2 - ed->prev_y) / 100.0);
		printf("x: %f\ty: %f\n", (float)(ed->prev_x - W_2) / 100.0,
						(float)(H_2 - ed->prev_y) / 100.0);
	}
	else if (event->type == SDL_KEYDOWN)
	{

		if (event->key.keysym.sym == SDLK_e)
		{
			/*
				завершить контур (замкнуть и выйти из режима рисования контура)
			*/
			map_new_point(&ed->map, ed->map.circuits[ed->map.circuits_count - 1].points[0].x,
						  ed->map.circuits[ed->map.circuits_count - 1].points[0].y);
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
			triangulate(ed);
			bsp_compile(ed);
		}
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
		while (j < ed->map.circuits[i].points_count)
		{
			if (j == 0)
			{
				j++;
				continue;
			}

			p1.x = (int)(ed->map.circuits[i].points[j - 1].x * 100);
			p1.y = (int)(ed->map.circuits[i].points[j - 1].y * 100);

			p2.x = (int)(ed->map.circuits[i].points[j].x * 100);
			p2.y = (int)(ed->map.circuits[i].points[j].y * 100);

			draw_line(pixels, p1.x, p1.y, p2.x, p2.y, 0xFFFFFF);

			draw_line(pixels, p1.x + (p2.x - p1.x) / 2, p1.y + (p2.y - p1.y) / 2,
					  p1.x + (p2.x - p1.x) / 2 + 30 * cos(atan2(p2.y - p1.y, p2.x - p1.x) + (float)ed->map.circuits[i].normal_dir * M_PI / 2),
					  p1.y + (p2.y - p1.y) / 2 + 30 * sin(atan2(p2.y - p1.y, p2.x - p1.x) + (float)ed->map.circuits[i].normal_dir * M_PI / 2),
					  0xff0000);
			j++;
		}
		i++;
	}
}

void map_init(t_map *map)
{
	map->circuits_count = 0;
	map->active = 0;
}

int main(int ac, char **av)
{
	t_map_editor map_editor;

	/*
		тут маленькая "библиотека" - MyGraphicsLib
		чтобы избавить основной код от прямого взаимодействия с SDL
	*/

	t_mgl *mgl = mgl_init("Map Editor BSP", W, H);


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
