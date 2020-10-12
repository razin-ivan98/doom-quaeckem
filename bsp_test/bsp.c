#include "./bsp.h"

/*
	алгоритм брейзенхема или кого там
*/

void	draw_line(int *pixels, int x1, int y1, int x2, int y2, int color)
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

t_vertex	get_face_normal(t_map_editor *editor, int i, int j)
{
	t_vertex ret;

	t_point p1;
	t_point p2;

	p1 = editor->map.circuits[i].points[j - 1];
	p2 = editor->map.circuits[i].points[j];

	ret.x = cos(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);

	ret.y = 0.0;
	ret.z = sin(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);

	return (ret);
}

/*
	новый контур
*/

void	map_new_circuit(t_map *map)
{
	map->circuits[map->circuits_count].points_count = 0;

	/*
		направление нормали
		по умолчаннию
	*/

	map->circuits[map->circuits_count].normal_dir = 1;

	(map->circuits_count)++;
}
void	map_new_point(t_map *map, float x, float y)
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
	map->circuits[map->circuits_count - 1].points[index].x = x;
	map->circuits[map->circuits_count - 1].points[index].y = y;
	map->circuits[map->circuits_count - 1].points_count++;
}

/*
	коллбэк событий
*/
void	event_handle(SDL_Event *event, void *ed_ptr, int *quit)
{
    t_map_editor *ed;

    ed = (t_map_editor *)ed_ptr;

    if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		/*
			новая точка
		*/
		ed->prev_x = event->button.x;
		ed->prev_y = event->button.y;

		map_new_point(&(ed->map), (float)(ed->prev_x - W_2 / 2) / 100.0,
							(float)(H_2 / 2 - ed->prev_y) / 100.0);//////////////////////поменят для норм экрана W_2 и H_2 на W и H
        printf("x: %f\ty: %f\n", (float)(ed->prev_x - W_2 / 2) / 100.0, (float)(H_2 / 2 - ed->prev_y) / 100.0);
	}
	else if (event->type == SDL_KEYDOWN )
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
		}
	}
		
}
/*
	коллбэк обновления экрана
*/
void	update(void *map_editor, int *pixels)
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
				continue ;
			}
				
			p1.x = (int)(ed->map.circuits[i].points[j - 1].x * 200);//////////////////////поменят для норм экрана 200 на 100
			p1.y = (int)(ed->map.circuits[i].points[j - 1].y * 200);

			p2.x = (int)(ed->map.circuits[i].points[j].x * 200);
			p2.y = (int)(ed->map.circuits[i].points[j].y * 200);


			draw_line(pixels, p1.x, p1.y, p2.x, p2.y, 0xFFFFFF);

			draw_line(pixels, p1.x + (p2.x - p1.x) / 2, p1.y + (p2.y - p1.y) / 2,
				p1.x + (p2.x - p1.x) / 2 + 30 * cos(atan2(p2.y - p1.y, p2.x - p1.x) +
											(float)ed->map.circuits[i].normal_dir * M_PI / 2),
				p1.y + (p2.y - p1.y) / 2 + 30 * sin(atan2(p2.y - p1.y, p2.x - p1.x) +
											(float)ed->map.circuits[i].normal_dir * M_PI / 2),
				0xff0000);
			j++;
		}
		i++;
	}
}


void	map_init(t_map *map)
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
