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

# define STEP_1_DRAW 1
# define STEP_2_FLOOR 2
# define STEP_3_CEIL 3
# define STEP_4_TEXTURES 4
# define STEP_5_PLAYER 5
# define STEP_6_AMMO 6
# define STEP_7_AID 7
# define STEP_8_AIM 8
# define STEP_9_DECOR 9
# define STEP_10_OBJECTS 10
# define STEP_11_ENEMIES 11
# define STEP_12_SAVE 12




# include "bsp_helpers.h"

void	export_map(t_map_editor *ed);

void	save_json(t_bsp *root, t_map_editor *ed);
char 	*ftoa(float value, int decimals, char *buf);
char 	*itoa(int value, char *buf);
int 	itoa_s(int value, char *buf);

#endif
