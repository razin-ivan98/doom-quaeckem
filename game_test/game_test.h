#ifndef MAP_EDITOR_H
# define MAP_EDITOR_H

# include "my_graphics_lib.h"
# include "duke.h"

# define MAX(a,b) ((a) > (b) ? (a) : (b))

# define WALL_TYPE_SECTOR_BORDER 0
# define WALL_TYPE_WALL 1
# define WALL_TYPE_FICTIVE 2

typedef struct	s_wall
{
	t_vertex	points[2];
	t_vertex	normal;
	int			type;
	int			failed;
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

typedef struct	s_player
{
	t_vertex	pos;
	float		alpha;
	float		speed;
	int			health;
	int			ammo;
}               t_player;

typedef struct	s_enemy_2d
{
	t_vertex	pos;
	float		alpha;
	float		speed;
	int			health;
	int			in_attaсk;
}               t_enemy_2d;

typedef struct  s_map
{
	t_bsp		root;
	t_player	player;
}               t_map;


typedef struct	s_map_editor
{
	int			prev_x;
	int			prev_y;

	int			w_pressed;
	int			s_pressed;
	int			a_pressed;
	int			d_pressed;

	t_map		map;
}				t_map_editor;


void	save_json(t_bsp *root, int not_leaves);
char 	*ftoa(float value, int decimals, char *buf);
char 	*itoa(int value, char *buf);
int 	itoa_s(int value, char *buf);
void	update_game(t_map_editor *ed);
int		classify_point(t_vertex cam, t_vertex line, t_vertex normal);





void	bsp_trav_zero(t_bsp *node);
#endif
