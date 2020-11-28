#include "./bsp.h"

t_vertex	lines_intersect(t_vertex a1, t_vertex a2, t_vertex b1, t_vertex b2)
{
	float A1, A2, B1, B2, C1, C2;

	A1 = a1.y - a2.y;
	B1 = a2.x - a1.x;
	C1 = a1.x * a2.y - a2.x * a1.y;

	A2 = b1.y - b2.y;
	B2 = b2.x - b1.x;
	C2 = b1.x * b2.y - b2.x * b1.y;

	float DivXY = A1 * B2 - A2 * B1;
 	float DivX = B1 * C2 - B2 * C1;
	float DivY = C1 * A2 - C2 * A1;
	return ((t_vertex){DivX / DivXY, DivY / DivXY, 0.0});
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

void insert_by_index(t_vertex *points, int index, int *count, t_vertex p)
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


	/*
		направление нормали
		по умолчаннию
	*/

	map->circuits[map->circuits_count].normal_dir = 1;

	(map->circuits_count)++;
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
		insert_by_index(map->circuits[map->on_circuit_i].points,
			get_i_plus_1(map->on_point_i, map->circuits[map->on_circuit_i].points_count),
			&map->circuits[map->on_circuit_i].points_count,
			(t_vertex){x, y, 0.0});
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

		if (!check_in_figure(curr, prev, next, test))
		{
			return (0);
		}
		i++;
	}

	return (1);
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
	*cutter_cir = -1;
	*cutter_wall = -1;
	////надо еще обычные каттеры
}

// int		classify_wall(t_wall cutter, t_wall wall)
// {
// 	t_vertex base;
// 	t_vertex p1;
// 	t_vertex p2;
// 	t_vertex cross1;
// 	t_vertex cross2;

// 	base = sub(cutter.points[1], cutter.points[0]);
// 	p1 = sub(cutter.points[1], wall.points[0]);
// 	p2 = sub(cutter.points[1], wall.points[1]);
// 	//printf("%f\n%f\n", cross(base, add(base, multiply(wall.normal, 9999))).z);
// 	if (cross(base, add(base, cutter.normal)).z > 0.0)
// 	{
// 		base = sub(cutter.points[0], cutter.points[1]);
// 		p1 = sub(cutter.points[0], wall.points[0]);
// 		p2 = sub(cutter.points[0], wall.points[1]);
// 	}

// 	base.z = 0.0;
// 	p1.z = 0.0;
// 	p2.z = 0.0;

		
// 	cross1 = cross(base, p1);
// 	cross2 = cross(base, p2);

// //	printf("cross1: %f\tcross2: %f\n", cross1.z, cross2.z);

// 	if (fabsf(cross1.z) < CUTTING_EPSILON && fabsf(cross2.z) < CUTTING_EPSILON)
// 		return (COMPLANAR);
// 	if ((cross1.z >= -CUTTING_EPSILON && cross2.z >= -CUTTING_EPSILON))
// 		return (FRONT);
// 	if ((cross1.z <= CUTTING_EPSILON && cross2.z <= CUTTING_EPSILON))
// 		return (BACK);
// 	if ((cross1.z > -CUTTING_EPSILON && cross2.z < CUTTING_EPSILON) ||
// 		(cross1.z < CUTTING_EPSILON && cross2.z > -CUTTING_EPSILON))
// 		return (CUTTED);
// }


int		classify_wall(t_vertex line, t_vertex n, t_wall wall, t_vertex *inter)
{
	t_vertex p;
	t_vertex ort_line;
	t_vertex in;


	p = wall.points[0];

	ort_line.x = -line.y;
	ort_line.y = line.x;
	ort_line.z = -line.x * wall.points[0].y + line.y * wall.points[0].x;

	in.x = (ort_line.y * line.z - line.y * ort_line.z) /
			(ort_line.x * line.y - line.x * ort_line.y);
	in.y = (-line.x * in.x - line.z) / line.y;
	in.z = 0.0;


	// printf("a: %f\tb: %f\n", -line.x/line.y, -line.z/line.y);
	// printf("px: %f\tpy: %f\n", p.x, p.y);
	// printf("inx: %f\tiny: %f\n", in.x, in.y);



	t_vertex ort1 = sub(p, in);

	p = wall.points[1];

	ort_line.z = -line.x * wall.points[1].y + line.y * wall.points[1].x;

	in.x = (ort_line.y * line.z - line.y * ort_line.z) /
			(ort_line.x * line.y - line.x * ort_line.y);
	in.y = (-line.x * in.x - line.z) / line.y;
	in.z = 0.0;

	t_vertex ort2 = sub(p, in);

	float dot1 = dot(normalize(ort1), normalize(n));
	float dot2 = dot(normalize(ort2), normalize(n));

	// printf("%f\n", dot1);
	// printf("%f\n\n", dot2);
	// printf("length %f\n\n", length(ort1));
	// printf("length %f\n\n", length(ort2));


	if (length(ort1) < 0.01 && length(ort2) < 0.01)
		return (COMPLANAR);
	else if (((dot1 < 0.0 && dot2 > 0.0) || (dot1 > 0.0 && dot2 < 0.0)) && 
				length(ort1) > 0.01 && length(ort2) > 0.01)
	{
		ort_line.x = wall.points[0].y - wall.points[1].y;
		ort_line.y = wall.points[1].x - wall.points[0].x;
		ort_line.z = wall.points[0].x * wall.points[1].y - wall.points[1].x * wall.points[0].y;

		inter->x = (ort_line.y * line.z - line.y * ort_line.z) /
			(ort_line.x * line.y - line.x * ort_line.y);
		inter->y = (-line.x * in.x - line.z) / line.y;
		inter->z = 0.0;

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


	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			p1 = circuits[i].walls[get_i_plus_1(j, circuits[i].walls_count)].points[0];
			p2 = circuits[i].walls[j].points[1];
			if (!(p1.x == p2.x && p1.y == p2.y))
				puts("РАЗРЫВ");
			j++;
		}
		i++;
	}
}

void	bsp_recurse(t_bsp *bsp, t_circuit *circuits, int circuits_count)
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
	

	// if (circuits_count == 1 && is_convex_polygon(walls, walls_count))
	// {
	// 	front = NULL;
	// 	back = NULL;
	// 	bsp->is_leaf = 1;
	// 	bsp->walls_count = walls_count;
	// 	bsp->walls = walls;
	// 	return ;
	// }

	reconstruct_circuits(circuits, circuits_count);

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
		while (i < circuits_count)
		{
			j = 0;
			while (j < circuits[i].walls_count)
			{
				bsp->walls[bsp->walls_count] = circuits[i].walls[j];
				bsp->walls_count++;
				j++;
			}
			i++;
		}
		puts("ВСЕ");
		return ;
	}

	cutter = get_big_wall(circuits[cutter_cir].walls[cutter_wall], bsp, &cutter_line);

	front = malloc(sizeof(t_circuit) * circuits_count);
	back = malloc(sizeof(t_circuit) * circuits_count);

	ft_bzero(front, sizeof(t_circuit) * circuits_count);
	ft_bzero(back, sizeof(t_circuit) * circuits_count);
	//printf("%f\t%f\t\t%f\t%f\n", cutter.points[0].x, cutter.points[0].y, cutter.points[1].x, cutter.points[1].y);

	bsp->is_leaf = 0;
	bsp->walls_count = 0;
	bsp->normal = cutter.normal;


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
				new1.points[1] = lines_intersect(cutter.points[0], cutter.points[1],
					circuits[i].walls[j].points[0], circuits[i].walls[j].points[1]);
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
				circuits[i].walls[j].used_in_bsp = 1;
				front[i].walls[front[i].walls_count] = circuits[i].walls[j];
				front[i].walls_count++;
			}

			j++;
		}
		i++;
	}

	bsp_recurse(bsp->back, back, circuits_count);
	bsp_recurse(bsp->front, front, circuits_count);
}

void	bsp_compile(t_map_editor *ed)
{
	int		i;
	

	transform_data(ed);

	// if (is_convex_polygon((t_wall *)walls, walls_count))
	// 	puts("CONVEX");
	// else
	// {
	// 	puts("NOTX");
	// }
	

	bsp_recurse(&ed->root, ed->map.circuits, ed->map.circuits_count);
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
			x = ed->map.circuits[i].points[j].x * 100 + W_2;
			y = H_2 - ed->map.circuits[i].points[j].y * 100;
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

			a.x = a.x * 100 + W_2;
			b.x = b.x * 100 + W_2;
			a.z = 0;
			b.z = 0;
			a.y = H_2 - a.y * 100;
			b.y = H_2 - b.y * 100;

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
int check_levels(t_bsp *node, int levels)
{
	if (node == NULL)
		return (levels);
	return (MAX(check_levels(node->front, levels + 1), check_levels(node->back, levels + 1)));
}
void event_handle(SDL_Event *event, void *ed_ptr, int *quit)
{
	t_map_editor *ed;

	ed = (t_map_editor *)ed_ptr;

	ed->prev_x = event->button.x * 2;
	ed->prev_y = event->button.y * 2;

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
		/*
			новая точка
		*/

		map_new_point(&(ed->map), (float)(ed->prev_x - W_2) / 100.0,
						(float)(H_2 - ed->prev_y) / 100.0, 0);
		printf("x: %f\ty: %f\n", (float)(ed->prev_x - W_2) / 100.0,
						(float)(H_2 - ed->prev_y) / 100.0);
	}
	else if (event->type == SDL_KEYDOWN)
	{

		if (event->key.keysym.sym == SDLK_s)
		{
			puts("SAVE JSON");
			int i = check_levels(&ed->root, i);
			save_json(&ed->root, i - 1);
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
		else
			while (j < ed->map.circuits[i].walls_count)
			{
				p1.x = (int)(ed->map.circuits[i].walls[j].points[0].x * 100);
				p1.y = (int)(ed->map.circuits[i].walls[j].points[0].y * 100);

				p2.x = (int)(ed->map.circuits[i].walls[j].points[1].x * 100);
				p2.y = (int)(ed->map.circuits[i].walls[j].points[1].y * 100);

				t_vertex n = ed->map.circuits[i].walls[j].normal;

				int color = (ed->map.circuits[i].walls[j].type == WALL_TYPE_WALL ? 0xFFFF00 : 0x00FF00);

				draw_line(pixels, p1.x, p1.y, p2.x, p2.y, color);

				draw_line(pixels,
					p1.x + (p2.x - p1.x) / 2,
					p1.y + (p2.y - p1.y) / 2,
					p1.x + (p2.x - p1.x) / 2 + 30 * n.x,
					p1.y + (p2.y - p1.y) / 2 + 30 * n.y,
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
	map->on_line = 0;
	map->on_point = 0;
}



int main(int ac, char **av)
{
	// t_wall wall, cutter;

	// wall.points[0] = (t_vertex){2 ,1 ,0.0};
	// wall.points[1] = (t_vertex){0 ,1 ,0.0};

	// cutter.points[0] = (t_vertex){10 ,10 ,0.0};
	// cutter.points[1] = (t_vertex){11 ,11 ,0.0};
	// cutter.normal = (t_vertex){-1 ,1 ,0.0};

	// //cutter = get_big_wall(cutter);

	// printf("%f\t%f\n%f\t%f\n\n", cutter.points[0].x, cutter.points[0].y, cutter.points[1].x, cutter.points[1].y);


	// printf("%d\n\n", classify_wall(cutter, wall));

	// t_vertex in = lines_intersect(wall.points[0], wall.points[1], cutter.points[0], cutter.points[1]);


	
	// printf("%f\t%f\n\n", in.x, in.y);
	t_map_editor map_editor;

	map_editor.mode = RENDER_MODE_POINTS;
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
