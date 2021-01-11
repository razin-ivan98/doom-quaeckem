#ifndef map_game_editor_H
# define map_game_editor_H

# include "my_graphics_lib.h"
# include "../duke.h"

# define MAX(a,b) ((a) > (b) ? (a) : (b))

# define wall_TYPE_SECTOR_BORDER 0
# define wall_TYPE_wall 1
# define wall_TYPE_FICTIVE 2

// typedef struct	s_wall
// {
// 	t_vertex	points[2];
// 	t_vertex	normal;
// 	int			type;
// 	int			failed;
// 	int			circuit;
// }				t_wall;


// typedef struct	s_bsp
// {
// 	int				is_leaf;
// 	t_vertex		line;
// 	t_vertex		normal;
// 	t_wall			walls[100];
// 	int				walls_count;
// 	int				circuit;
// 	struct s_bsp	*front;
// 	struct s_bsp	*back;
// }				t_bsp;

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

typedef struct  s_map_game
{
	t_bsp		root;
	t_player	player;
}               t_map_game;


typedef struct	s_map_game_editor
{
	int			prev_x;
	int			prev_y;

	int			w_pressed;
	int			s_pressed;
	int			a_pressed;
	int			d_pressed;

	t_map_game		map_game;
}				t_map_game_editor;


char 	*ftoa(float value, int decimals, char *buf);
char 	*itoa(int value, char *buf);
int 	itoa_s(int value, char *buf);
void	update_game(t_map_game_editor *ed);
int		classify_point(t_vertex cam, t_vertex line, t_vertex normal);





void	bsp_trav_zero(t_bsp *node);
#endif
