#include "duke.h"

int get_sizes(char *name, int *h, int *w)
{
    int i = 0;
    int fd;
    char *line;
    if ((fd = open(name, O_RDONLY)) < 0)
		exit(-2);
    while (get_next_line(fd, &line) > 0)
    {
        *w = ft_strlen(line);
        i++;
    }
    *h = i;
    close(fd);
}

void create_ver_poly(t_model *model, int y, int x, int **tex, int *texture)
{
    t_triangle tr1;
    t_triangle tr2;
    t_vertex v;



    v = (t_vertex){(float)(x), 0.5, (float)(y - 1)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[2] = model->vertexes_count;
    tr1.uvs[2] = (t_point){0.0, 0.0, 0.0};
    model->vertexes_count++;
    
    v = (t_vertex){(float)(x), -0.5, (float)(y - 1)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[0] = model->vertexes_count;
    tr1.uvs[0] = (t_point){0.0, 1.0, 0.0};
    tr2.indexes[0] = model->vertexes_count;
    tr2.uvs[0] = (t_point){0.0, 1.0, 0.0};
    model->vertexes_count++;

    v = (t_vertex){(float)(x), 0.5, (float)(y)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[1] = model->vertexes_count;
    tr1.uvs[1] = (t_point){1.0, 0.0, 0.0};
    tr2.indexes[1] = model->vertexes_count;
    tr2.uvs[1] = (t_point){1.0, 0.0, 0.0};
    model->vertexes_count++;

    v = (t_vertex){(float)(x), -0.5, (float)(y)};
    tr2.indexes[2] = model->vertexes_count;
    tr2.uvs[2] = (t_point){1.0, 1.0, 0.0};
    model->vertexes[model->vertexes_count] = v;
    model->vertexes_count++;

    tr1.new_tex = tex;
    tr1.texture = texture;

    tr2.new_tex = tex;
    tr2.texture = texture;



    model->triangles[model->triangles_count] = tr1;
    model->triangles_count++;
    model->triangles[model->triangles_count] = tr2;
    model->triangles_count++;
}

void create_gor_poly(t_model *model, int y, int x, int **tex, int *texture)
{
    t_triangle tr1;
    t_triangle tr2;
    t_vertex v;

    v = (t_vertex){(float)(x - 1), 0.5, (float)(y)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[2] = model->vertexes_count;
    tr1.uvs[2] = (t_point){0.0, 0.0, 0.0};
    model->vertexes_count++;
    
    v = (t_vertex){(float)(x - 1), -0.5, (float)(y)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[0] = model->vertexes_count;
    tr1.uvs[0] = (t_point){0.0, 1.0, 0.0};
    tr2.indexes[0] = model->vertexes_count;
    tr2.uvs[0] = (t_point){0.0, 1.0, 0.0};
    model->vertexes_count++;

    v = (t_vertex){(float)(x), 0.5, (float)(y)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[1] = model->vertexes_count;
    tr1.uvs[1] = (t_point){1.0, 0.0, 0.0};
    tr2.indexes[1] = model->vertexes_count;
    tr2.uvs[1] = (t_point){1.0, 0.0, 0.0};
    model->vertexes_count++;

    v = (t_vertex){(float)(x), -0.5, (float)(y)};
    tr2.indexes[2] = model->vertexes_count;
    tr2.uvs[2] = (t_point){1.0, 1.0, 0.0};
    model->vertexes[model->vertexes_count] = v;
    model->vertexes_count++;

    tr1.new_tex = tex;
    tr1.texture = texture;

    tr2.new_tex = tex;
    tr2.texture = texture;



    model->triangles[model->triangles_count] = tr1;
    model->triangles_count++;
    model->triangles[model->triangles_count] = tr2;
    model->triangles_count++;

}

void create_top_bottom(t_model *model, int x, int y, int **tex, int *texture)
{
    t_triangle tr1;
    t_triangle tr2;
    t_vertex v;

    v = (t_vertex){(float)(0.0), -0.5, (float)(0.0)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[2] = model->vertexes_count;
    tr1.uvs[2] = (t_point){0.0, 0.0, 0.0};
    model->vertexes_count++;
    
    v = (t_vertex){(float)(x), -0.5, (float)(0.0)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[0] = model->vertexes_count;
    tr1.uvs[0] = (t_point){0.0, 1.0, 0.0};
    tr2.indexes[0] = model->vertexes_count;
    tr2.uvs[0] = (t_point){0.0, 1.0, 0.0};
    model->vertexes_count++;

    v = (t_vertex){(float)(0.0), -0.5, (float)(y)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[1] = model->vertexes_count;
    tr1.uvs[1] = (t_point){1.0, 0.0, 0.0};
    tr2.indexes[1] = model->vertexes_count;
    tr2.uvs[1] = (t_point){1.0, 0.0, 0.0};
    model->vertexes_count++;

    v = (t_vertex){(float)(x), -0.5, (float)(y)};
    tr2.indexes[2] = model->vertexes_count;
    tr2.uvs[2] = (t_point){1.0, 1.0, 0.0};
    model->vertexes[model->vertexes_count] = v;
    model->vertexes_count++;

    tr1.new_tex = tex;
    tr1.texture = texture;

    tr2.new_tex = tex;
    tr2.texture = texture;



    model->triangles[model->triangles_count] = tr1;
    model->triangles_count++;
    model->triangles[model->triangles_count] = tr2;
    model->triangles_count++;






v = (t_vertex){(float)(0.0), 0.5, (float)(0.0)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[2] = model->vertexes_count;
    tr1.uvs[2] = (t_point){0.0, 0.0, 0.0};
    model->vertexes_count++;
    
    v = (t_vertex){(float)(x), 0.5, (float)(0.0)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[0] = model->vertexes_count;
    tr1.uvs[0] = (t_point){0.0, 1.0, 0.0};
    tr2.indexes[0] = model->vertexes_count;
    tr2.uvs[0] = (t_point){0.0, 1.0, 0.0};
    model->vertexes_count++;

    v = (t_vertex){(float)(0.0), 0.5, (float)(y)};
    model->vertexes[model->vertexes_count] = v;
    tr1.indexes[1] = model->vertexes_count;
    tr1.uvs[1] = (t_point){1.0, 0.0, 0.0};
    tr2.indexes[1] = model->vertexes_count;
    tr2.uvs[1] = (t_point){1.0, 0.0, 0.0};
    model->vertexes_count++;

    v = (t_vertex){(float)(x), 0.5, (float)(y)};
    tr2.indexes[2] = model->vertexes_count;
    tr2.uvs[2] = (t_point){1.0, 1.0, 0.0};
    model->vertexes[model->vertexes_count] = v;
    model->vertexes_count++;

    tr1.new_tex = tex;
    tr1.texture = texture;

    tr2.new_tex = tex;
    tr2.texture = texture;



    model->triangles[model->triangles_count] = tr1;
    model->triangles_count++;
    model->triangles[model->triangles_count] = tr2;
    model->triangles_count++;
}

void read_map(char *name, t_model *model, int **tex, int *texture)
{
    char *line;
    int fd;
    model->vertexes_count = 0;
    model->triangles_count = 0;
    model->uvs_count = 0;

    int w, h;

    get_sizes(name, &h, &w);

    if ((fd = open(name, O_RDONLY)) < 0)
		exit(-2);

    int i;

    i = 0;

    int **map;
    map = malloc(sizeof(int*) * h);

    while (i < h)
    {
        map[i] = malloc(sizeof(int) * w);

        get_next_line(fd, &line);
        
        int j = 0;
        while (line[j])
        {
            if (line[j] == '1')
                map[i][j] = 1;
            else
                map[i][j] = 0;
            
            j++;
        }
        i++;
    }
    create_top_bottom(model, w, h, tex, texture);
  /*  for (int i = 1; i < h; i++)
    {
        for (int j = 1; j < w; j++)
        {
            if (map[i][j] == 1 && map[i][j - 1] == 1)
                create_gor_poly(model, i, j, tex, texture);
            if (map[i][j] == 1 && map[i - 1][j] == 1)
                create_ver_poly(model, i, j, tex, texture);
        }
    }*/

    
    close(fd);

   
}