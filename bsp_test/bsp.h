#ifndef BSP_H
# define BSP_H

# include "my_graphics_lib.h"
#include "../duke.h"


# define WALL_TYPE_SECTOR_BORDER 0
# define WALL_TYPE_WALL 1
# define WALL_TYPE_FICTIVE 2


# define RENDER_MODE_POINTS 0
# define RENDER_MODE_WALLS 1
# define RENDER_MODE_TRS 2


# define DEFAULT_CEIL 4.0
# define DEFAULT_FLOOR -1.0


# define CUTTING_EPSILON 5.0

# define MAX(a,b) ((a) > (b) ? (a) : (b))

// typedef struct	s_wall
// {
// 	t_vertex points[2];
// 	t_vertex normal;

// 	int			type;

// 	int			circuit; // (только для границ секторов) номер сектора, с которым граничит эта стена

// 	int			used_in_bsp;
// }				t_wall;

// typedef struct	s_tr
// {
// 	t_vertex	points[3];
// }				t_tr;


// /*
// 	контур. предстваляет из себя замкнутый многоугольник.
// 	первая и последняя точки одинаковы
// 	нормаль контура опрделяется по направлению обхода точек при рисовании
// 	по часовой стрелке - нормаль наружу, против - внутрь
// 	normal_dir  по умолчанию = 1
// 	если установить -1 (клавиша n в редакторе), то нормаль инвертируется
// */
// typedef struct	s_circuit
// {
// 	t_vertex	points[100];
// 	t_wall		walls[100];
// 	int			walls_count;
// 	int			points_count;
// 	int			normal_dir;
// 	int			integrated;//////
// 	int			failed;

// 	float		ceil;
// 	float		floor;

// }				t_circuit;

// /*
// 	первый контур в массиве контуров - основной (контур помещения)
// 	последующие - "дырки" (колонны и т д)
// 	поэтому у первого контура нормаль должна смотреть внутрь
// 	а у остальных наружу
// */

// typedef struct s_map
// {
// 	t_circuit circuits[10];

// 	t_vertex vts[10000];
// 	int	vts_count;
// 	t_vertex nrmls[10000];
// 	int	nrmls_count;
// 	t_vertex uvs[10000];
// 	int	uvs_count;


// 	int active;					// режим рисования контура
// 	int selected_circuit;
// 	int circuits_count;

// 	int on_point;
// 	int on_line;

// 	int on_circuit_i;
// 	int on_point_i;

// } t_map;

// typedef struct	s_vt_tr
// {
// 	int			ids[3];
// 	int			n_ids[3];
// 	int			uv_ids[3];
// }				t_vt_tr;


// /* вот такая структура для bsp-узла */

// typedef struct s_bsp
// {
// 	int is_leaf;			// является ли листом (то есть конечным узлом, 
// 							// в котором записаны все стены и у которого нет детей)

// 	t_vertex line;			// (только для не листовых узлов) содержит уравнение прямой,
// 							// по которой происходит деление оставшегося пространства

// 	t_vertex normal;		// (только для не листовых узлов)
// 							// нормаль линии разреза (соответствует нормали выбранной cutter-стены)

// 	t_wall		walls[100];		// (только для листов) массив стен в листе
// 	int			walls_count;

// 	t_tr		*trs;
// 	t_vt_tr		*vt_trs;
// 	int			trs_count;
// 	int			vt_trs_count;


// 	int			circuit;	// (только для листов) номер сектора, к которому принадлежит лист

// 	struct s_bsp *front;	// (только для не листовых узлов)
// 	struct s_bsp *back;		// (только для не листовых узлов)
// } t_bsp;


// // typedef struct	s_bsp_comp
// // {
// // 	t_circuit	circuits[20];
// // 	t_circuit	circuits_back[20];

// // 	int			circuits_front_count;
// // 	int			circuits_back_count;
	
// // }				t_bsp_comp;


// typedef struct s_map_editor
// {
// 	int prev_x;
// 	int prev_y;

// 	t_map map;

	

// 	SDL_Surface *cursor_surface;
// 	SDL_Cursor *cursor;

// 	t_bsp	root;

// 	int		edit_floor;

// 	int		edit_ceil;




// 	int mode;
// }				t_map_editor;



# include "bsp_helpers.h"

void	export_map(t_map_editor *ed);

void	save_json(t_bsp *root);
char 	*ftoa(float value, int decimals, char *buf);
char 	*itoa(int value, char *buf);
int 	itoa_s(int value, char *buf);

#endif
