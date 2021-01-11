#include "duke.h"
#include "bsp_test/bsp.h"


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
		puts("GOVONO2");

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
		puts("GOVONO");
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

void read_bsp(t_bsp *root, char *filename)
{
	int fd;
	char *read_str;
	int i;

	read_str = malloc(262144); /////////////

	fd = open(filename, O_RDWR);

	read_str[read(fd, read_str, 262143)] = 0;
	close(fd);

	read_node(root, ft_strchr(read_str, '{') + 1);
}
