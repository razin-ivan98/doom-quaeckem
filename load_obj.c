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


void get_uvss(t_model *model, char *file_name)
{
    int fd;
    int i;
    char *str;
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
        printf("%d\n", i);
    }
    close(fd);
}

void get_points(t_model *model, char *file_name)
{
    int fd;
    int i;
    char *str;
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
        printf("%d\n", i);
        free(line);
    }
    printf("readed\n");
    close(fd);
}

t_triangle generate_polygon(char *line, t_model *model)
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

    ret.new_tex = model->new_tex;
    ret.color = 0xffff00;

    return (ret);
}

void get_polygons(t_model *model, char *file_name)
{
    int fd;
    int i;
    char *str;
    char *line;

    i = 0;
    if ((fd = open(file_name, O_RDONLY)) < 0)
		exit(-2);
    while (get_next_line(fd, &line) > 0)
    {
        if (line[0] == 'f' && line[1] == ' ')
        {
            model->triangles[i] = generate_polygon(line, model);
            model->triangles_count++;
            i++;
        }
        free(line);
        printf("%d\n", i);
    }
    printf("%d\n", model->triangles_count);
    close(fd);
}

void read_obj(t_model *model, char *file_name)
{

 //   points = (t_vector *)malloc(sizeof(t_vector) * 30000);

 //   rtv1->scene.camera.center = (t_vector){0.0, 1.0, -3.0};
 //   rtv1->scene.camera.dir = (t_vector){0.0, 0.0, 1.0};

  //  rtv1->scene.lights[0].type = point;
  //  rtv1->scene.lights[0].center = (t_vector){1.0, 1.0, -2.0};
  //  rtv1->scene.lights[0].intensity = 0.7;

  //  rtv1->scene.lights[1].type = ambient;
  //  rtv1->scene.lights[1].intensity = 0.3;

  //  rtv1->scene.c_lights = 2;
   printf("Start\n");
    model->vertexes_count = 0;
    get_points(model, file_name);
 printf("OK");
    model->uvs_count = 0;
   get_uvss(model, file_name);
 printf("OK");
  //  printf("x = %f y = %f z = %f", points[2].x, points[2].y, points[2].z);
    model->triangles_count = 0;
    get_polygons(model, file_name);
   // printf("x = %f y = %f z = %f", rtv1->scene.objs[1].center.x, rtv1->scene.objs[1].center.y, rtv1->scene.objs[1].center.z);
   printf("OK");
}




