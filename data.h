#ifndef DATA_H
# define DATA_H

# include "data_types.h"
# include "my_graphics_lib/my_graphics_lib.h"

typedef struct	s_plane
{
	t_vertex	normal;
	float		distance;
}				t_plane;

typedef struct	s_triangle
{
	int			indexes[3];
	t_vertex	normal;
	int			*texture;
	int			**new_tex;

	SDL_Surface	*tex;

	int			color;
	t_point		uvs[3];
	int			used;
}				t_triangle;

typedef struct	s_anim
{
	SDL_Surface	*frames[64];
	int			length;
	float		speed;
	int			curr;
	float		curr_f;
	int			played;
}				t_anim;


typedef struct	s_model
{
//	t_point		*uvs;
	SDL_Surface	*new_tex[10];

	t_anim		*anim;
	
	t_vertex	*vertexes;
	t_point	*uvs;
	t_vertex	*normals;
	int normals_count;
	int			uvs_count;
	t_vertex	bounds_center;
	float		bounds_radius;
	int			vertexes_count;
	t_triangle	*triangles;
	int			triangles_count;
	t_point		*projected;
	
}				t_model;

typedef struct	s_instance
{
	t_model		model;
	t_vertex	*clipped;
	t_vertex	position;
	t_mat4x4	orientation;	
	float	scale;
	
	t_mat4x4	transform;

}				t_instance;

typedef struct	s_camera
{
	t_vertex	position;
	t_mat4x4	orientation;
}				t_camera;


typedef struct	s_float_array
{
	float	*array;
	int		length;
}				t_float_array;

typedef struct	s_edge_interpolate
{
	t_float_array v02;
	t_float_array v012;
}				t_edge_interpolate;

typedef struct	s_e_i_input
{
	int		y0;
	float	v0;
	int		y1;
	float	v1;
	int		y2;
	float	v2;
}				t_e_i_input;

typedef struct	s_e_i_output
{
	float	v02;
	float	v01;
	float	v12;
	float	limit;
}				t_e_i_output;


typedef struct	s_redner
{
	t_edge_interpolate	x;
	t_edge_interpolate	iz;
	t_edge_interpolate	uz;
	t_edge_interpolate	vz;

	t_float_array		zscan;
	t_float_array		uzscan;
	t_float_array		vzscan;

	t_float_array		v01;
	t_float_array		v12;
	t_float_array		v02;

	t_float_array		*x_left;
	t_float_array		*x_right;
	t_float_array		*iz_left;
	t_float_array		*iz_right;
	t_float_array		*uz_left;
	t_float_array		*uz_right;
	t_float_array		*vz_left;
	t_float_array		*vz_right;

	float				xl;
	float				xr;
	float				zl;
	float				zr;
	float				uzl;
	float				uzr;
	float				vzl;
	float				vzr;

	float				d_z;
	float				d_u;
	float				d_v;

	t_vertex			vers[3];
	t_point				pts[3];
	int					ids[3];

	t_model				rendered;

}				t_render;

typedef struct	s_level
{
	t_instance	instance;

	t_bsp		root;

}				t_level;

typedef struct	s_sprite
{
	t_instance	instance;

}				t_sprite;

typedef struct	s_object
{
	t_sprite	sprite;

	int			index;
	t_anim		anim;

}				t_object;

typedef struct	s_enemy
{
	t_sprite	sprite;

	t_vertex	start_pos;

	int			health;

	t_anim		walking_anims[8];
	t_anim		attak_anim;
	t_anim		damage_anim;
	t_anim		death_anim;

	int			damaged;
	int			in_attak;

	int			on_ground;

	float		g_speed;

	float		beta;
	
}				t_enemy;

typedef struct	s_scene
{
	t_level		level;
	t_sprite	*sprites;
	int			sprites_count;
	t_object	*objects;
	int			objects_count;

	int			depth_mode;

	t_enemy		enemies[40];
	int			enemies_count;

	t_model		*models;
	int			models_count;

	t_camera	camera;
	float		*z_buffer;

	t_plane		clipping_planes[5];

	t_render	*render_tr;

	t_render	f_render_tr;
	t_render	s_render_tr;

}				t_scene;

typedef	struct	s_bullet
{
	t_sprite	sprite;
	int			enable;
	t_vertex	first;
	float		gamma;
	float		alpha;

}				t_bullet;

typedef	struct	s_ammo
{
	t_sprite	sprite;
	int			enable;
	int			start_enable;

}				t_ammo;

typedef	struct	s_aid
{
	t_sprite	sprite;
	int			enable;
	int			start_enable;
}				t_aid;

typedef struct	s_pthread_data
{
	t_scene		scene;
	int			*image_data;
}				t_pthread_data;

typedef struct	s_menu
{
	SDL_Surface	*play;
	SDL_Surface	*difficulty_1;
	SDL_Surface	*difficulty_2;
	SDL_Surface	*difficulty_3;
	SDL_Surface *exit_b;
	int			active;
}				t_menu;

typedef struct	s_tv
{
	int			enable;
	t_anim		anim;
	t_vertex	pos;
	float		beta;
	t_sprite	sprite;
}				t_tv;

typedef struct	s_doom
{
	t_scene		scene;
	float		alpha;
	float		gamma;
	float		beta;
	int			prev_x;
	int			prev_y;
	int			mouse_pressed;
	int			mouse_right_pressed;
	int			w_pressed;
	int			s_pressed;
	int			a_pressed;
	int			d_pressed;
	int			solid;

	int			difficulty;

	t_menu		menu;

	t_tv		tv;

	int			sit;
	int			run;

	float		height;

	t_vertex	aim;

	SDL_Surface *menu_back;

	SDL_Surface *health_bar;
	SDL_Surface *ammo_bar;
	SDL_Surface *kills_bar;
	SDL_Surface *press_f;


	t_vertex	start_pos;

	int			lose;
	int			win;

	SDL_Surface	*win_surface;
	SDL_Surface	*lose_surface;

	int			kills;

	int			enemy_damage;
	float		enemy_speed;

	int			menu_opened;

	int			health;

	t_mat4x4	for_sprites;

	int			player_ammo;

	float		g_speed;
	int			on_ground;

	

	t_anim		drb_anim;

	t_enemy		enemies[40];
	int			enemies_count;

	t_bullet	bullets[40];
	t_ammo		ammo[40];
	t_aid		aid[40];

	t_object	objects[40];
	int			objects_count;

	t_mgl		*mgl;

}				t_doom;


typedef struct	s_clip_triangle
{
	int			outside_count;
	int			inside_count;
	int			outsides[3];
	int			insides[3];
	
	int			l;
	t_triangle	crop[6][40];

	t_plane		*planes;

	t_vertex	new1;
	t_vertex	new2;
	t_point		new_uv1;
	t_point		new_uv2;
}				t_clip_triangle;


#endif
