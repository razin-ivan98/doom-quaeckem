/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Chorange <Chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 16:11:05 by Chorange          #+#    #+#             */
/*   Updated: 2021/02/04 13:28:22 by Chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H



# define RENDER_MODE_POINTS 0
# define RENDER_MODE_WALLS 1
# define RENDER_MODE_TRS 2

# define DEFAULT_CEIL 4.0
# define DEFAULT_FLOOR -1.0

# define CUTTING_EPSILON 5.0

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

# define MAP_EDITOR_SCALE 10

# include "my_graphics_lib.h"
# include "../data_types.h"
# include "bsp_helpers.h"
# include "../duke.h"

typedef struct		s_bs_cc
{
	t_vt_tr			new;
	t_vertex		vt;
	t_vertex		uv;
	float 			top;
	float			bottom;
	int				flag;
	int				i;
	t_wall			wall;
	t_bsp			*link;
}					t_bs_cc;


typedef struct		s_cc
{
	int				i;
	int				j;
	t_vertex		base;
	int 			res[2];
}					t_cc;

typedef struct		s_w_zz
{
	t_vertex 		p;
	t_vertex		ort_line;
	t_vertex		in;
	t_vertex		ort1;
	t_vertex		ort2;
	float			dot1;
	float			dot2;
	int				ret;
}					t_w_zz;


typedef struct		s_req_s
{
	t_circuit		*front;
	t_circuit		*back;
	int				i;
	int				j;
	int				result;
	int				cutter_cir;
	int				cutter_wall;
	t_wall			cutter;
	t_wall			new1;
	t_wall			new2;
	t_vertex 		cutter_line;
	t_vertex		inter;
}					t_req_s;




typedef struct		s_m_e_object
{
	t_vertex		pos;
	int				index;
}					t_m_e_object;

typedef struct		s_map
{
	t_circuit		circuits[20];

	t_vertex		*vts;
	int				vts_count;
	t_vertex		*nrmls;
	int				nrmls_count;
	t_vertex		*uvs;
	int				uvs_count;

	int				border_id;

	int				active;
	int				selected_circuit;
	int				circuits_count;

	int				on_point;
	int				on_line;

	int				on_circuit_i;
	int				on_point_i;

}					t_map;

typedef struct 		s_a_t_b
{
	t_vt_tr 		new;
	t_vertex 		vt;
	t_vertex 		uv;
	float 			top;
	float 			bottom;
	int 			flag;
	t_bsp 			*link;
	t_wall 			wall;
	t_bsp 			*root;
}					t_a_t_b;


typedef struct		s_map_editor
{
	int				prev_x;
	int				prev_y;

	t_map			map;

	SDL_Surface		*cursor_surface;
	SDL_Cursor		*cursor;

	t_bsp			root;

	int				edit_floor;

	int				edit_ceil;

	int				edit_ceil_tex;
	int				edit_floor_tex;
		int			edit_wall_tex;

	t_vertex		tv;
	float			tv_beta;

	t_vertex		ammo[40];
	int				ammo_count;
	t_vertex		aid[40];
	int				aid_count;
	t_vertex		enemies[40];
	int				enemies_count;
	t_vertex		player;
	t_vertex		aim;
	t_m_e_object	objects[40];
	int				objects_count;


	int				curr_object;

	int				step;

	int				mode;
}					t_map_editor;

void				export_map(t_map_editor *ed);

void				save_json(t_bsp *root, t_map_editor *ed);
char 				*ftoa(float value, int decimals, char *buf);
char 				*itoa(int value, char *buf);
int 				itoa_s(int value, char *buf);

void				editor_init(t_map_editor *ed);

void    			set_hash();


void				draw_line(int *pixels, t_int_v f, t_int_v s, int color);


void				draw_objects(t_map_editor *ed, int *pixels);
void				draw_ammo(t_map_editor *ed, int *pixels);
void				draw_aid(t_map_editor *ed, int *pixels);
void				draw_enemies(t_map_editor *ed, int *pixels);
void				draw_tv(t_map_editor *ed, int *pixels);

void				bsp_trs_draw_traversal(t_bsp *node, int *pixels);
void				draw_in_points_mode(int i, int *pixels, t_map_editor *ed);
void				draw_in_walls_mode(int i, int *pixels, t_map_editor *ed);

void				draw_elements(t_map_editor *ed, int *pixels);

void				draw_point(t_vertex pos, int color, int *pixels);



void				mouse_motion(t_map_editor *ed);
void				mouse_button_down(t_map_editor *ed);




void				add_aid(t_vertex pos, t_map_editor *ed);
void				add_ammo(t_vertex pos, t_map_editor *ed);
void				add_enemy(t_vertex pos, t_map_editor *ed);
void				add_object(t_vertex pos, int number, t_map_editor *ed);



void				write_instructions(t_map_editor *ed);
void				write_object_num(t_map_editor *ed);
void				write_floor_height(t_map_editor *ed);
void				write_ceil_height(t_map_editor *ed);
void				write_floor_tex(t_map_editor *ed);
void				write_ceil_tex(t_map_editor *ed);
void				write_wall_tex(t_map_editor *ed);



int					cursor_on_line(t_map_editor *ed);
int					cursor_on_point(t_map_editor *ed);



int					bsp_select_circuit_traversal(t_bsp *node, t_vertex pos);


void				map_new_point(t_map *map, float x, float y, int flag);


void				key_minus_down(t_map_editor *ed);
void				key_plus_down(t_map_editor *ed);
void				key_enter_down(t_map_editor *ed);



void				bsp_compile(t_map_editor *ed);
t_wall				get_big_wall(t_wall wall, t_bsp *bsp, t_vertex *line);
void				bsp_recurse(t_bsp *bsp, t_circuit *circuits, int circuits_count, t_map *map);
int					classify_wall(t_vertex line, t_vertex n, t_wall wall, t_vertex *inter);
void				get_cutter(t_circuit *circuits, int circuits_count, int *cutter_cir, int *cutter_wall);
void				reconstruct_circuits(t_circuit *circuits, int circuits_count);


void	add_tops_bottoms(t_bsp *node, t_map *map, t_bsp *root);
t_bsp *get_node_by_wall_traversal(t_bsp *node, t_wall wall);
int add_vt(t_map *map, t_vertex vt);
int add_n(t_map *map, t_vertex n);
int add_uv(t_map *map, t_vertex uv);

void		do_vt_sec(t_a_t_b *tatb, t_bsp *node, t_map *map, int i);
void		do_uv_sec(t_a_t_b *tatb, t_bsp *node, t_map *map, int i);

void		do_vt_2(t_a_t_b *tatb, t_bsp *node, t_map *map, int i);
void		do_vt_3(t_a_t_b *tatb, t_bsp *node, t_map *map, int i);
void		do_vt_4(t_a_t_b *tatb, t_bsp *node, t_map *map, int i);

void to_obj_format(t_bsp *node, t_map *map);


void				key_down(t_map_editor *ed, SDL_Event *event);


void				get_cutter(t_circuit *circuits, int circuits_count,
								int *cutter_cir, int *cutter_wall);
void				transform_data(t_map_editor *ed);



void				check_enemies(t_map_editor *ed, int fd);
void				check_ammo(t_map_editor *ed, int fd);
void				check_objects(t_map_editor *ed, int fd);
void				check_aid(t_map_editor *ed, int fd);
void				check_player(t_map_editor *ed, int fd);
void				check_aim(t_map_editor *ed, int fd);
void				check_tv(t_map_editor *ed, int fd);
void				check_not_leaf(t_bsp *node, int fd);
void				write_data_1(t_bsp *node, int fd);
void				write_data_2(t_bsp *node, int fd);
void				check_walls(t_bsp *node, int i, int fd);
void				bsp_save_traversal(t_bsp *node, int fd);



void				floor_tr(t_bsp *node, t_map *map, t_vt_tr *new, int i);

void				ceil_tr(t_bsp *node, t_map *map, t_vt_tr *new, int i);
void				ceil_tr_vts(t_bsp *node, t_map *map, t_vt_tr *new, int i);

void					err(char *str);
void				set_hash_1(char *str1, char *str2);
void				set_hash_2(char *str1, char *str2, int hash_n);



#endif
