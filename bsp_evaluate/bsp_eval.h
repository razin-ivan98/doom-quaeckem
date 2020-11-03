#ifndef MAP_EDITOR_H
# define MAP_EDITOR_H

# include "my_graphics_lib.h"
# include "duke.h"

 #define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct	s_wall
{
	t_vertex	points[2];
	t_vertex	normal;
}				t_wall;


typedef struct	s_bsp
{
	int				is_leaf;
	t_vertex		line;
	t_vertex		normal;
	t_wall			walls[100];
	int				walls_count;
	struct s_bsp	*front;
	struct s_bsp	*back;
}				t_bsp;

typedef struct  s_map
{
	t_bsp		root;
	t_vertex	player;
}               t_map;


typedef struct	s_map_editor
{
	int			prev_x;
	int			prev_y;

	t_map		map;
}				t_map_editor;


void	save_json(t_bsp *root, int not_leaves);
char 	*ftoa(float value, int decimals, char *buf);
char 	*itoa(int value, char *buf);
int 	itoa_s(int value, char *buf);


#endif
