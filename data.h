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
	float	h;
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

typedef struct	s_model
{
//	t_point		*uvs;
	SDL_Surface	*new_tex;
	
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


	t_model				rendered;

}				t_render;

typedef struct	s_scene
{
	t_model		*models;
	int			models_count;

	t_instance	instance;
	int			instances_count;
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