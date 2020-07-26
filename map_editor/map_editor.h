#ifndef MAP_EDITOR_H
# define MAP_EDITOR_H

# include "my_graphics_lib.h"
# include "duke.h"

typedef struct	s_circuit
{
	t_point		points[20];
	int			points_count;
	int			normal_dir;
}				t_circuit;

typedef struct  s_map
{
    t_circuit   circuits[10];
	int			active;
    int         circuits_count;   
}               t_map;

typedef struct	s_map_editor
{
	int			prev_x;
	int			prev_y;

	int			normals_count;
	int			vertexes_count;
	int			uvss_count;

	t_map		map;

}				t_map_editor;

#endif
