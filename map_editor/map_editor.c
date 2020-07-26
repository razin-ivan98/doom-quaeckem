#include "map_editor.h"

void		zeros(int f, char *ret)
{
	int i;
	int j;

	i = 0;
	while (f > 0)
	{
		f /= 10;
		i++;
	}
	j = 0;
	while (j < 5 - i)
	{
		ft_strcat(ret, "0");
		j++;
	}

}

char		*ft_ftoa(float n)
{
	char *tmp;
	char *ret;
	int i;
	int f;

	ret = (char *)malloc(64);//////////////////////////
	i = (int)n;
	f = (int)((n - (double)((int)n)) * 100000.0) * (n < 0.0 ? -1.0 : 1.0);

	tmp = ft_itoa(i);
	if (!ft_strchr(tmp, '-') && n < 0.0)
		ft_strcpy(tmp, "-0");
	ft_strcpy(ret, tmp);

	free(tmp);
	

	tmp = ft_itoa(f);
	ft_strcat(ret, ".");

	zeros(f, ret);


	ft_strcat(ret, tmp);
	free(tmp);
	
	return (ret);
}


void	draw_line(int *pixels, int x1, int y1, int x2, int y2, int color)
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
    //
	int error = deltaX - deltaY;
    //
	put_pixel(pixels, x2, y2, color);

	while(x1 != x2 || y1 != y2) 
	{
		put_pixel(pixels, x1, y1, color);
		const int error2 = error * 2;
        //
		if(error2 > -deltaY) 
		{
			error -= deltaY;
			x1 += signX;
		}
		if(error2 < deltaX) 
		{
			error += deltaX;
			y1 += signY;
		}
//		puts("lol");
	}


}

t_vertex	get_face_normal(t_map_editor *editor, int i, int j)
{
	t_vertex ret;

	t_point p1;
	t_point p2;

	p1 = editor->map.circuits[i].points[j - 1];
	p2 = editor->map.circuits[i].points[j];

	ret.x = cos(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);
				printf("cos = %f\n", ret.x);
	ret.y = 0.0;
	ret.z = sin(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);
				printf("sin = %f\n", ret.z);



	return (ret);
}

void	save_map(t_map_editor *editor)
{
	int fd;
	int i;
	int j;

	char *ptr;

	fd = open("map_ed.obj", O_RDWR | O_TRUNC);

	i = 0;

	ft_putendl_fd("v -4.0 -1.0 4.0", fd);
	ft_putendl_fd("v -4.0 -1.0 -4.0", fd);///низ
	ft_putendl_fd("v 4.0 -1.0 -4.0", fd);
	ft_putendl_fd("v 4.0 -1.0 4.0", fd);

	ft_putendl_fd("v -4.0 1.0 4.0", fd);
	ft_putendl_fd("v -4.0 1.0 -4.0", fd);///верх 
	ft_putendl_fd("v 4.0 1.0 -4.0", fd);
	ft_putendl_fd("v 4.0 1.0 4.0", fd);

	ft_putendl_fd("vt 0.0 0.0", fd);
	ft_putendl_fd("vt 0.0 1.0", fd);
	ft_putendl_fd("vt 1.0 1.0", fd);
	ft_putendl_fd("vt 1.0 0.0", fd);

	ft_putendl_fd("vn 0.0 1.0 0.0", fd);
	ft_putendl_fd("vn 0.0 -1.0 0.0", fd);

	ft_putendl_fd("f 1/1/1 2/2/1 3/3/1", fd);//низ
	ft_putendl_fd("f 1/1/1 3/3/1 4/4/1", fd);

	ft_putendl_fd("f 5/1/2 6/2/2 7/3/2", fd);//верх
	ft_putendl_fd("f 5/1/2 7/3/2 8/4/2", fd);

	editor->vertexes_count = 8;
	editor->normals_count = 2;
	editor->uvss_count = 4;

	ft_putendl_fd("", fd);

	while (i < editor->map.circuits_count)
	{
		j = 0;
		while (j < editor->map.circuits[i].points_count)
		{
			if (j == 0)
			{
				j++;
				continue ;
			}


			ft_putstr_fd("v ", fd);//////1
			ptr = ft_ftoa(editor->map.circuits[i].points[j - 1].x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" 1.0 ", fd);
			ptr = ft_ftoa(editor->map.circuits[i].points[j - 1].y);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n", fd);
			
			ft_putstr_fd("v ", fd);//////2
			ptr = ft_ftoa(editor->map.circuits[i].points[j - 1].x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" -1.0 ", fd);
			ptr = ft_ftoa(editor->map.circuits[i].points[j - 1].y);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n", fd);

			ft_putstr_fd("v ", fd);//////3
			ptr = ft_ftoa(editor->map.circuits[i].points[j].x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" -1.0 ", fd);
			ptr = ft_ftoa(editor->map.circuits[i].points[j].y);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n", fd);

			ft_putstr_fd("v ", fd);//////4
			ptr = ft_ftoa(editor->map.circuits[i].points[j].x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" 1.0 ", fd);
			ptr = ft_ftoa(editor->map.circuits[i].points[j].y);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n\n", fd);



			ft_putstr_fd("vn ", fd);////normal

			t_vertex normal = get_face_normal(editor, i, j);
			ptr = ft_ftoa(normal.x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" 0.0 ", fd);
			ptr = ft_ftoa(normal.z);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n\n", fd);

			editor->vertexes_count += 4;
			editor->normals_count += 1;


			ft_putstr_fd("f ", fd);///face1
			ft_putnbr_fd(editor->vertexes_count - 3, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(1, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(editor->vertexes_count - 2, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(2, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(editor->vertexes_count - 1, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(3, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd("\n", fd);



			ft_putstr_fd("f ", fd);////face2
			ft_putnbr_fd(editor->vertexes_count - 3, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(1, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(editor->vertexes_count - 1, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(3, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(editor->vertexes_count, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(4, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd("\n\n\n", fd);







			j++;
		}
		
		i++;
	}

	close(fd);
}
void	map_new_circuit(t_map *map)
{
	map->circuits[map->circuits_count].points_count = 0;
	map->circuits[map->circuits_count].normal_dir = 1;
	(map->circuits_count)++;
}
void	map_new_point(t_map *map, float x, float y)
{
	int index;
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

void	event_handle(SDL_Event *event, void *map_ptr, int *quit)
{
    t_map_editor *map;

    map = (t_map_editor *)map_ptr;

    if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		map->prev_x = event->button.x;
		map->prev_y = event->button.y;

		map_new_point(&(map->map), (float)(map->prev_x - W_2 / 2) / 100.0,
							(float)(H_2 / 2 - map->prev_y) / 100.0);//////////////////////поменят для норм экрана
        printf("x: %d\ty: %d\n", map->prev_x, map->prev_y);
	}
	else if (event->type == SDL_KEYDOWN )
	{
		if (event->key.keysym.sym == SDLK_s)
		{
			save_map(map);
		}
		else if (event->key.keysym.sym == SDLK_e)
		{
			map_new_point(&map->map, map->map.circuits[map->map.circuits_count - 1].points[0].x,
									map->map.circuits[map->map.circuits_count - 1].points[0].y);
			map->map.active = 0;
		}
		else if (event->key.keysym.sym == SDLK_n)
		{
			if (map->map.active)
			{
				map->map.circuits[map->map.circuits_count - 1].normal_dir *= -1;
			}
		}
	}
		
}

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
				
			p1.x = (int)(ed->map.circuits[i].points[j - 1].x * 200);///справтить для норм экрана
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

			//printf("angle: %f\n", atan2(p2.x - p1.x, p2.y - p1.y) / M_PI);
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

	map_editor.normals_count = 0;
	map_editor.uvss_count = 0;
	map_editor.vertexes_count = 0;

	t_mgl *mgl = mgl_init("Map Editor", W, H);



	mgl_run(mgl, update, event_handle, &map_editor);


	mgl_quit(mgl);
	return (0);

}