#include "duke.h"


static int		strlen_while_dig(char *str)
{
	int len;

	len = 0;
	while (str)
	{
		if (!isdigit(*str))
			break ;
		str++;
		len++;
	}
	return (len);
}

float			ft_atof(char *str)
{
	float	ret;
	float	mod;
	int		neg;

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


t_vertex read_coords(char *line)
{
    t_vertex ret;
    char *tmp;

    tmp = ft_strchr(line, ' ') + 1;
    ret.x = atof(tmp);
    tmp = ft_strchr(tmp, ' ') + 1;
    ret.y = atof(tmp);
    tmp = ft_strchr(tmp, ' ') + 1;
    ret.z = atof(tmp);

    return (ret);
}
t_point read_uvs(char *line)
{
    t_point ret;
    char *tmp;

    tmp = ft_strchr(line, ' ') + 1;
    ret.x = atof(tmp);
    tmp = ft_strchr(tmp, ' ') + 1;
    ret.y = atof(tmp);


    return (ret);
}

void get_normals(t_vertex *normals, int *count, char *file_name)
{
    int fd;
    int i;
    char *line;

    i = 0;
    if ((fd = open(file_name, O_RDONLY)) < 0)
		exit(-2);
    while (get_next_line(fd, &line) > 0)
    {
        if (line[0] == 'v' && line[1] == 'n')
        {
            normals[i] = read_coords(line);
            (*count)++;
            i++;
        }
        free(line);
    }
    close(fd);
}

void get_uvss(t_model *model, char *file_name)
{
    int fd;
    int i;
    char *line;

    i = 0;
    if ((fd = open(file_name, O_RDONLY)) < 0)
		exit(-2);
    while (get_next_line(fd, &line) > 0)
    {
        if (line[0] == 'v' && line[1] == 't')
        {
            model->uvs[i] = read_uvs(line);
            model->uvs_count++;
            i++;
        }
        free(line);
    }
    close(fd);
}

void get_points(t_model *model, char *file_name)
{
    int fd;
    int i;
    char *line;

    i = 0;
    if ((fd = open(file_name, O_RDONLY)) < 0)
		exit(-2);
    while (get_next_line(fd, &line) > 0)
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            model->vertexes[i] = read_coords(line);
            model->vertexes_count++;
            i++;
        }
        free(line);
    }

    close(fd);
}

t_triangle generate_polygon(char *line, t_model *model, t_vertex *normals)
{
    t_triangle ret;
    char *tmp;
    int number;

    tmp = ft_strchr(line, ' ') + 1;
    number = ft_atoi(tmp);
    ret.indexes[0] = number - 1;

    tmp = ft_strchr(tmp, '/') + 1;
    number = ft_atoi(tmp);
    ret.uvs[0] = model->uvs[number - 1];




    tmp = ft_strchr(tmp, ' ') + 1;
    number = ft_atoi(tmp);
    ret.indexes[1] = number - 1;

    tmp = ft_strchr(tmp, '/') + 1;
    number = ft_atoi(tmp);
    ret.uvs[1] = model->uvs[number - 1];




    tmp = ft_strchr(tmp, ' ') + 1;
    number = ft_atoi(tmp);
    ret.indexes[2] = number - 1;
    
    tmp = ft_strchr(tmp, '/') + 1;
    number = ft_atoi(tmp);
    ret.uvs[2] = model->uvs[number - 1];

    tmp = ft_strchr(tmp, '/') + 1;
    number = ft_atoi(tmp);
    ret.normal = normals[number - 1];



    ret.tex = model->new_tex[0];

    return (ret);
}

void get_polygons(t_model *model, char *file_name, t_vertex *normals)
{
    int fd;
    int i;
    char *line;

    i = 0;
    if ((fd = open(file_name, O_RDONLY)) < 0)
		exit(-2);
    while (get_next_line(fd, &line) > 0)
    {
        if (line[0] == 'f' && line[1] == ' ')
        {
            model->triangles[i] = generate_polygon(line, model, normals);
            model->triangles_count++;
            i++;
        }
        free(line);
    }
    close(fd);
}

void read_obj(t_model *model, char *file_name)
{
    model->normals_count = 0;

    if (!(model->normals = malloc(sizeof(t_vertex) * 3000)))
        exit(-2);

    model->vertexes_count = 0;
    get_points(model, file_name);

    model->uvs_count = 0;
    get_uvss(model, file_name);


    get_normals(model->normals, &model->normals_count, file_name);

    model->triangles_count = 0;
    get_polygons(model, file_name, model->normals);

    //free(normals);
}




