#ifndef MAP_EDITOR_H
# define MAP_EDITOR_H

# include "my_graphics_lib.h"
# include "duke.h"

# define TRS 1
# define LINES 0

typedef struct	s_vtx_id
{
	t_vertex	vt;
	int			c_id;
	int			id;
}				t_vtx_id;

typedef struct	s_pt_id
{
	t_point		pt;
	int			c_id;
	int			id;
}				t_pt_id;

typedef struct	s_circuit
{
	int			integrated;
	t_pt_id		points[100];
	int			points_count;
	int			normal_dir;
}				t_circuit;

typedef struct	s_tr
{
	t_point		points[3];

}				t_tr;

typedef struct	s_tr_idx
{
	int			ids[3];
}				t_tr_idx;


typedef struct  s_map
{
    t_circuit   circuits[10];
	int			active;
    int         circuits_count;
	t_tr		trs[100];
	int			trs_count;

	t_vtx_id	vrts[10000];
	int			vrts_count;
	t_tr_idx	trs_ids[1000];
	int			trs_ids_count;

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
