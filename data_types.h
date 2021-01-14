
#ifndef DATA_TYPES_H
# define DATA_TYPES_H

# define COMPLANAR 1
# define CUTTED 2
# define FRONT 3
# define BACK 4

# define WALL_TYPE_SECTOR_BORDER 0
# define WALL_TYPE_WALL 1
# define WALL_TYPE_FICTIVE 2

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

typedef struct	s_tr
{
	t_vertex	points[3];
}				t_tr;

typedef struct	s_wall
{
	t_vertex points[2];
	t_vertex normal;

	int			failed;

	int			type;

	int			border_id;

	int			circuit;

	int			used_in_bsp;
}				t_wall;

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

typedef struct	s_circuit
{
	t_vertex	points[100];
	t_wall		walls[100];
	int			walls_count;
	int			points_count;
	int			normal_dir;
	int			integrated;

	float		ceil;
	float		floor;
	int			sky;

	int			wall_tex;
	int			floor_tex;
	int			ceil_tex;


}				t_circuit;

typedef struct	s_vt_tr
{
	int			ids[3];
	int			n_ids[3];
	int			uv_ids[3];
	int			type;
}				t_vt_tr;

typedef struct s_bsp
{
	int is_leaf;

	t_vertex line;

	t_vertex normal;

	t_wall		walls[100];
	int			walls_count;

	int			wall_tex;
	int			floor_tex;
	int			ceil_tex;

	t_tr		*trs;
	t_vt_tr		*vt_trs;
	int			trs_count;
	int			vt_trs_count;

	float		floor;
	float		ceil;

	int			circuit;

	struct s_bsp *front;
	struct s_bsp *back;
} t_bsp;

#endif