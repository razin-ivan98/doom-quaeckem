#ifndef DATA_H
# define DATA_H

# include "my_graphics_lib/my_graphics_lib.h"

# define COMPLANAR 1
# define CUTTED 2
# define FRONT 3
# define BACK 4

typedef struct	s_int_v
{
	int			x;
	int			y;
}				t_int_v;

typedef struct	s_point
{
	float		x;
	float		y;
	float		h;
}				t_point;

typedef struct	s_vertex
{
	float x;
	float y;
	float z;
}				t_vertex;

typedef struct	s_vertex4
{
	float x;
	float y;
	float z;
	float w;
}				t_vertex4;

typedef struct	s_mat4x4
{
	float mat[4][4];
}				t_mat4x4;

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
	SDL_Surface	**frames;
	int			length;
	float		speed;
	int			curr;
}				t_anim;


typedef struct	s_model
{
//	t_point		*uvs;
	SDL_Surface	*new_tex;

	t_anim		*anim;
	
	t_vertex	*vertexes;
	t_point	*uvs;
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



}				t_level;

typedef struct	s_sprite
{
	t_instance	instance;


	

}				t_sprite;

typedef struct	s_object
{
	t_instance	instance;


	t_anim		anim;

}				t_object;

typedef struct	s_enemy
{
	t_sprite	sprite;

	int			health;

	t_anim		walking_anims[8];

	float		beta;
	
}				t_enemy;

typedef struct	s_scene
{
	t_level		level;
	t_sprite	*sprites;
	int			sprites_count;
	t_object	*objects;
	int			objects_count;

	t_enemy		enemy;

	t_model		*models;
	int			models_count;

	t_camera	camera;
	float		*z_buffer;

	t_plane		clipping_planes[5];

	t_render	render_tr;
}				t_scene;



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

	t_enemy		*enemies;
	int			enemies_count;

	t_object	*objects;
	int			objects_count;

	t_mgl		*mgl;

}				t_doom;




typedef struct			s_tr_list
{
	t_triangle			tr;
	struct s_tr_list	*next;
}						t_tr_list;


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
