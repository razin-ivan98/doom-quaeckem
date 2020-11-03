#include "./bsp_eval.h"

int b = 0;

int colors[3];

static int strlen_while_dig(char *str)
{
	int len;

	len = 0;
	while (str)
	{
		if (!isdigit(*str))
			break;
		str++;
		len++;
	}
	return (len);
}

float ft_atof(char *str)
{
	float ret;
	float mod;
	int neg;

	while ((*str == '\n') || (*str == '\t') || (*str == '\v') ||
		   (*str == ' ') || (*str == '\f') || (*str == '\r'))
		str++;
	neg = (*str == '-' ? 1 : 0);
	ret = 0.0;
	ret = (float)atoi(str);
	if (!strchr(str, '.'))
		return (ret);
	str = strchr(str, '.') + 1;
	if (isdigit(*str))
	{
		mod = (float)atoi(str);
		mod /= pow(10, strlen_while_dig(str));
		ret += (neg ? -mod : mod);
	}
	return (ret);
}

char *read_node(t_bsp *node, char *str);

char *add_wall(t_bsp *node, char *str)
{
	char key[32];
	char *ptr;

	ptr = str;

	while (*ptr)
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '{' || *ptr == ',')
			ptr++;

		if (*ptr == '"')
		{
			ptr++;
			ft_strncpy(key, ptr, ft_strchr(ptr, '"') - ptr);
			key[ft_strchr(ptr, '"') - ptr] = 0;
			ptr = ft_strchr(ptr, ':') + 1;

			if (!ft_strcmp(key, "points"))
			{
				ptr = ft_strchr(ptr, '[') + 1;
				ptr = ft_strchr(ptr, '[') + 1;
				node->walls[node->walls_count].points[0].x = ft_atof(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->walls[node->walls_count].points[0].y = ft_atof(ptr);

				ptr = ft_strchr(ptr, '[') + 1;
				node->walls[node->walls_count].points[1].x = ft_atof(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->walls[node->walls_count].points[1].y = ft_atof(ptr);
				ptr = ft_strchr(ptr, ']') + 1;
				ptr = ft_strchr(ptr, ']') + 1;
			}
			else if (!ft_strcmp(key, "normal"))
			{
				ptr = ft_strchr(ptr, '[') + 1;
				node->walls[node->walls_count].normal.x = ft_atof(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->walls[node->walls_count].normal.y = ft_atof(ptr);
				ptr = ft_strchr(ptr, ']') + 1;
			}
		}
		else if (*ptr == '}')
			break;
	}
	(node->walls_count)++;
	return (ft_strchr(ptr, '}') + 1);
}

char *read_walls(t_bsp *node, char *str)
{
	char *ptr;

	ptr = str;
	while (*ptr) //////////
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == ',')
			ptr++;

		if (*ptr == '{')
		{
			ptr = add_wall(node, ptr + 1);
		}
		else if (*ptr == ']')
			break;
	}
	return (ft_strchr(ptr, ']') + 1);
}

char *read_property(t_bsp *node, char *str)
{
	char key[32];
	char *ptr;

	ft_strncpy(key, str, ft_strchr(str, '"') - str);
	key[ft_strchr(str, '"') - str] = 0;

	puts(key);
	ptr = ft_strchr(str, ':') + 1;
	if (!ft_strcmp(key, "isLeaf"))
	{
		node->is_leaf = ft_atoi(ptr);
		while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
			ptr++;
		return (ptr + 1);
	}
	else if (!ft_strcmp(key, "line"))
	{
		ptr = ft_strchr(ptr, '[') + 1;
		node->line.x = ft_atof(ptr);
		ptr = ft_strchr(ptr, ',') + 1;
		node->line.y = ft_atof(ptr);
		return (ft_strchr(ptr, ']') + 1);
	}
	else if (!ft_strcmp(key, "normal"))
	{
		ptr = ft_strchr(ptr, '[') + 1;
		node->normal.x = ft_atof(ptr);
		ptr = ft_strchr(ptr, ',') + 1;
		node->normal.y = ft_atof(ptr);
		return (ft_strchr(ptr, ']') + 1);
	}
	else if (!ft_strcmp(key, "walls"))
	{
		ptr = ft_strchr(ptr, '[') + 1;
		ptr = read_walls(node, ptr);
		return (ptr);
	}
	else if (!ft_strcmp(key, "front"))
	{
		ptr = ft_strchr(ptr, '{') + 1;
		node->front = (t_bsp *)malloc(sizeof(t_bsp)); ////////
		return (read_node(node->front, ptr));
	}
	else if (!ft_strcmp(key, "back"))
	{
		ptr = ft_strchr(ptr, '{') + 1;
		node->back = (t_bsp *)malloc(sizeof(t_bsp)); ////////
		return (read_node(node->back, ptr));
	}
	else

		return (NULL);
}

char *read_node(t_bsp *node, char *str)
{
	char *ptr;

	node->walls_count = 0;

	ptr = str;
	while (*ptr) //////////
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '{' || *ptr == ',')
			ptr++;

		if (*ptr == '"')
		{
			ptr = read_property(node, ptr + 1);
		}
		else if (*ptr == '}')
			break;
	}

	return (ft_strchr(ptr, '}') + 1);
}

void read_bsp_tree(t_map *map, char *filename)
{
	int fd;
	char *read_str;
	int i;

	read_str = malloc(262144); /////////////

	fd = open(filename, O_RDWR);

	read_str[read(fd, read_str, 262143)] = 0;
	close(fd);

	read_node(&map->root, ft_strchr(read_str, '{') + 1);
}

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
	int i = 0;

	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		ed->prev_x = event->button.x;
		ed->prev_y = event->button.y;

		ed->map.player.x = (ed->prev_x - W_2) / 100.0;
		ed->map.player.y = (-ed->prev_y + H_2) / 100.0;
	}
	else if (event->type == SDL_KEYDOWN )
	{
		if (event->key.keysym.sym == SDLK_s)
		{
			puts("SAVE JSON");
			i = check_levels(&ed->map.root, i);
			save_json(&ed->map.root, i - 1);
		}
	}
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

void bsp_obhod(t_bsp *node, int *pixels, t_vertex cam)
{
	int i;
	t_vertex p1;
	t_vertex p2;
	t_vertex n;

	colors[0] = 0xff0000;
	colors[1] = 0x00ff00;
	colors[2] = 0x0000ff;

	if (node->is_leaf)
	{
		i = 0;
		while (i < node->walls_count)
		{
			p1 = node->walls[i].points[0];
			p2 = node->walls[i].points[1];
			n = multiply(node->walls[i].normal, 0.3);

			draw_line(pixels, p1.x * 100, p1.y * 100, p2.x * 100, p2.y * 100, colors[b]);
			draw_line(pixels, (p1.x + (p2.x - p1.x) / 2) * 100, (p1.y + (p2.y - p1.y) / 2) * 100,
					  (p1.x + (p2.x - p1.x) / 2 + n.x) * 100,
					  (p1.y + (p2.y - p1.y) / 2 + n.y) * 100,
					  0xffff00);
			i++;
		}
		b++;
	}
	else
	{
		if (classify_point(cam, node->line, node->normal) == FRONT)
		{

			bsp_obhod(node->back, pixels, cam);
			bsp_obhod(node->front, pixels, cam);
		}
		else
		{
			bsp_obhod(node->front, pixels, cam);
			bsp_obhod(node->back, pixels, cam);
		}
	}
}

void update(void *map_editor, int *pixels)
{
	t_map_editor *ed;
	int i;
	int j;
	b = 0;
	ed = (t_map_editor *)map_editor;

	bzero(pixels, sizeof(int) * HxW);

	bsp_obhod(&(ed->map.root), pixels, ed->map.player);

	i = -2;
	while (i < 3)
	{
		j = -2;
		while (j < 3)
		{
			put_pixel(pixels, ed->map.player.x * 100 + i, ed->map.player.y * 100 + j, 0x00ff00);
			j++;
		}
		i++;
	}
}

int main(int ac, char **av)
{
	t_map_editor map_editor;

	t_mgl *mgl = mgl_init("Map Editor BSP", W, H);

	read_bsp_tree(&map_editor.map, av[1]);

	mgl_run(mgl, update, event_handle, &map_editor);

	mgl_quit(mgl);
	return (0);
}
