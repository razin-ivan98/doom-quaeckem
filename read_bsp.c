#include "duke.h"


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

static float ft_atof(char *str) /////разобраться
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
			else if (!ft_strcmp(key, "type"))
			{
				node->walls[node->walls_count].type = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
			}
		}
		else if (*ptr == '}')
			break;
	}
	(node->walls_count)++;
	return (ft_strchr(ptr, '}') + 1);
}


char *add_vt_tr(t_bsp *node, char *str)
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

			if (!ft_strcmp(key, "ids"))
			{
				ptr = ft_strchr(ptr, '[') + 1;
				node->vt_trs[node->vt_trs_count].ids[0] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->vt_trs[node->vt_trs_count].ids[1] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->vt_trs[node->vt_trs_count].ids[2] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ']') + 1;
			}
			else if (!ft_strcmp(key, "n_ids"))
			{
				ptr = ft_strchr(ptr, '[') + 1;
				node->vt_trs[node->vt_trs_count].n_ids[0] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->vt_trs[node->vt_trs_count].n_ids[1] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->vt_trs[node->vt_trs_count].n_ids[2] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ']') + 1;
			}
			else if (!ft_strcmp(key, "uv_ids"))
			{
				ptr = ft_strchr(ptr, '[') + 1;
				node->vt_trs[node->vt_trs_count].uv_ids[0] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->vt_trs[node->vt_trs_count].uv_ids[1] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ',') + 1;
				node->vt_trs[node->vt_trs_count].uv_ids[2] = ft_atoi(ptr);
				ptr = ft_strchr(ptr, ']') + 1;
			}
			else if (!ft_strcmp(key, "type"))
			{
				node->vt_trs[node->vt_trs_count].type = ft_atoi(ptr);
				while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == ',' || ft_isdigit(*ptr))
					ptr++;
			}
		}
		else if (*ptr == '}')
			break;
	}
	(node->vt_trs_count)++;
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

char *read_vt_trs(t_bsp *node, char *str)
{
	char *ptr;

	ptr = str;
	while (*ptr) //////////
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == ',')
			ptr++;

		if (*ptr == '{')
		{
			ptr = add_vt_tr(node, ptr + 1);
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
	else if (!ft_strcmp(key, "floor"))
	{
		node->floor = ft_atof(ptr);
		while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
			ptr++;
		return (ft_strchr(ptr, ','));
	}
	else if (!ft_strcmp(key, "ceil"))
	{
		node->ceil = ft_atof(ptr);
		while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
			ptr++;
		return (ft_strchr(ptr, ','));
	}
	else if (!ft_strcmp(key, "wall_tex"))
	{
		node->wall_tex = ft_atoi(ptr);
		while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
			ptr++;
		return (ft_strchr(ptr, ','));
	}
	else if (!ft_strcmp(key, "ceil_tex"))
	{
		node->ceil_tex = ft_atoi(ptr);
		while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
			ptr++;
		return (ft_strchr(ptr, ','));
	}
	else if (!ft_strcmp(key, "floor_tex"))
	{
		node->floor_tex = ft_atoi(ptr);
		while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
			ptr++;
		return (ft_strchr(ptr, ','));
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
	else if (!ft_strcmp(key, "vt_trs"))
	{
		ptr = ft_strchr(ptr, '[') + 1;
		ptr = read_vt_trs(node, ptr);
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
	node->vt_trs_count = 0;
	node->vt_trs = malloc(sizeof(t_vt_tr) * 1000);////////////


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

char	*add_enemy(t_doom *doom, char *ptr)
{
	t_vertex pos;

	pos.x = ft_atof(ptr);
	ptr = ft_strchr(ptr, ',') + 1;
	pos.z = ft_atof(ptr);

	doom->enemies[doom->enemies_count] = create_enemy(pos, 0.0);
	doom->enemies_count++;
	
	return (ft_strchr(ptr, ']') + 1);
}

char	*add_object(t_doom *doom, char *ptr)
{
	t_vertex	pos;
	int			index;

	ptr = ft_strchr(ptr, ':') + 1;

	index = ft_atoi(ptr);
	ptr = ft_strchr(ptr, '[') + 1;
	pos.x = ft_atof(ptr);
	ptr = ft_strchr(ptr, ',') + 1;
	pos.z = ft_atof(ptr);


	doom->objects[doom->objects_count] = create_object(pos, index);
	doom->objects_count++;
	
	return (ft_strchr(ptr, '}') + 1);
}

char	*read_enemies(t_doom *doom, char *ptr)
{
	doom->enemies_count = 0;
	ptr = ft_strchr(ptr, '[') + 1;

	while (*ptr) //////////
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == ',')
			ptr++;

		if (*ptr == '[')
		{
			ptr = add_enemy(doom, ptr + 1);
		}
		else if (*ptr == ']')
			break;
	}
	return (ft_strchr(ptr, ']') + 1);
}
char	*read_objects(t_doom *doom, char *ptr)
{
	doom->objects_count = 0;
	ptr = ft_strchr(ptr, '[') + 1;

	while (*ptr) //////////
	{
		// puts("lol");//
		// puts(ptr);//
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == ',')
			ptr++;

		if (*ptr == '{')
		{
			ptr = add_object(doom, ptr + 1);
		}
		else if (*ptr == ']')
			break;
	}
	return (ft_strchr(ptr, ']') + 1);
}
char	*read_tv(t_doom *doom, char *ptr)
{
	
	ptr = ft_strchr(ptr, '[') + 1;
	doom->tv.sprite.instance.position.x = ft_atof(ptr);
	ptr = ft_strchr(ptr, ',') + 1;
	doom->tv.sprite.instance.position.z = ft_atof(ptr);
	ptr = ft_strchr(ptr, ':') + 1;
	doom->tv.sprite.instance.position.y = 0.0;
	doom->tv.beta = ft_atof(ptr);
	create_tv(doom);

	return (ft_strchr(ptr, '}') + 1);
}
char	*add_ammo(t_doom *doom, char *ptr, int *ammo_count)
{
	t_vertex pos;

	pos.x = ft_atof(ptr);
	ptr = ft_strchr(ptr, ',') + 1;
	pos.z = ft_atof(ptr);

	doom->ammo[*ammo_count].enable = 1;
	doom->ammo[*ammo_count].sprite.instance.position = pos;
	doom->ammo[*ammo_count].start_enable = 1;


	(*ammo_count)++;
	
	return (ft_strchr(ptr, ']') + 1);
}

char	*read_ammo(t_doom *doom, char *ptr)
{
	int ammo_count;

	ammo_count = 0;
	ptr = ft_strchr(ptr, '[') + 1;

	while (*ptr) //////////
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == ',')
			ptr++;

		if (*ptr == '[')
		{
			ptr = add_ammo(doom, ptr + 1, &ammo_count);
		}
		else if (*ptr == ']')
			break;
	}
	return (ft_strchr(ptr, ']') + 1);
}


char	*add_aid(t_doom *doom, char *ptr, int *aid_count)
{
	t_vertex pos;

	pos.x = ft_atof(ptr);
	ptr = ft_strchr(ptr, ',') + 1;
	pos.z = ft_atof(ptr);

	doom->aid[*aid_count].enable = 1;
	doom->aid[*aid_count].sprite.instance.position = pos;
	doom->aid[*aid_count].start_enable = 1;


	(*aid_count)++;
	
	return (ft_strchr(ptr, ']') + 1);
}

char	*read_aid(t_doom *doom, char *ptr)
{
	int aid_count;

	aid_count = 0;
	ptr = ft_strchr(ptr, '[') + 1;

	while (*ptr) //////////
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == ',')
			ptr++;

		if (*ptr == '[')
		{
			ptr = add_aid(doom, ptr + 1, &aid_count);
		}
		else if (*ptr == ']')
			break;
	}
	return (ft_strchr(ptr, ']') + 1);
}


char *read_data_property(t_doom *doom, char *str)
{
	char key[32];
	char *ptr;

	ft_strncpy(key, str, ft_strchr(str, '"') - str);
	key[ft_strchr(str, '"') - str] = 0;

	puts(key);
	ptr = ft_strchr(str, ':') + 1;
	if (!ft_strcmp(key, "player"))
	{
		ptr = ft_strchr(ptr, '[') + 1;
		doom->scene.camera.position.x = ft_atof(ptr);
		ptr = ft_strchr(ptr, ',') + 1;
		doom->scene.camera.position.z = ft_atof(ptr);
		doom->start_pos = doom->scene.camera.position;
		return (ft_strchr(ptr, ']') + 1);
	}
	else if (!ft_strcmp(key, "aim"))
	{
		ptr = ft_strchr(ptr, '[') + 1;
		doom->aim.x = ft_atof(ptr);
		ptr = ft_strchr(ptr, ',') + 1;
		doom->aim.z = ft_atof(ptr);
		doom->aim.y = 0.0;
		return (ft_strchr(ptr, ']') + 1);
	}
	else if (!ft_strcmp(key, "enemies"))
	{
		return (read_enemies(doom, ptr));
	}
	else if (!ft_strcmp(key, "tv"))
	{
		return (read_tv(doom, ptr));
	}
	else if (!ft_strcmp(key, "ammo"))
	{
		return (read_ammo(doom, ptr));
	}
	else if (!ft_strcmp(key, "aid"))
	{
		return (read_aid(doom, ptr));
	}
	else if (!ft_strcmp(key, "objects"))
	{
		return (read_objects(doom, ptr));
	}
	else if (!ft_strcmp(key, "bsp"))
		read_node(&doom->scene.level.root, ft_strchr(ptr, '{') + 1);
	return (NULL);
}
void	read_data(t_doom *doom, char *str)
{
	char *ptr;

	ptr = str;
	ptr = ft_strchr(str, '{') + 1;
	while (*ptr) //////////
	{
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '{' || *ptr == ',')
			ptr++;

		if (*ptr == '"')
		{
			ptr = read_data_property(doom, ptr + 1);
			if (!ptr)
				return;
		}
		else if (*ptr == '}')
			break;
	}
}


void read_bsp(t_doom *doom, char *filename)
{
	int fd;
	char *read_str;

	read_str = malloc(262144); /////////////наверное надо больше

	fd = open(filename, O_RDWR);

	if (fd < 0)
	{
		ft_putendl("Ошибка чтения файла");
		exit(-2);
	}

	read_str[read(fd, read_str, 262143)] = 0;
	close(fd);

	

	read_data(doom, read_str);

	// read_node(&scene->root, ft_strchr(ptr, '{') + 1);
	free(read_str);
}

void clear_bsp(t_bsp *node)
{
	free(node->vt_trs);
	if (node->is_leaf)
	{
		return ;
	}
	
	clear_bsp(node->front);
	clear_bsp(node->back);
	free(node->front);
	free(node->back);
}