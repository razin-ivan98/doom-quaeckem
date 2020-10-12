#ifndef MAP_EDITOR_H
# define MAP_EDITOR_H

# include "my_graphics_lib.h"
# include "duke.h"

# define TRS 1
# define LINES 0

typedef struct	s_circuit
{
	t_point		points[100];
	int			points_count;
	int			normal_dir;
	int			integrated;
}				t_circuit;

typedef struct	s_tr
{
	t_point		points[3];
}				t_tr;

typedef struct	s_vt_tr
{
	int			ids[3];
	int			n_ids[3];
	int			uv_ids[3];
}				t_vt_tr;


typedef struct  s_map
{
    t_circuit   circuits[10];
	int			active;
    int         circuits_count;
	t_tr		trs[100];
	int			trs_count;

	t_vt_tr		vt_trs[1000];
	t_vertex	vts[10000];
	t_vertex	nrmls[1000];
	t_point		uvs[1000];


	int			vts_count;
	int			vt_trs_count;
	int			nrmls_count;
	int			uvs_count;

}               t_map;

typedef struct	s_map_editor
{
	int			prev_x;
	int			prev_y;

	int			normals_count;
	int			vertexes_count;
	int			uvss_count;

	int			mode;

	t_map		map;

	int			error;
	int			curr_tr;

}				t_map_editor;

#endif
