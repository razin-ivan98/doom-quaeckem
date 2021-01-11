#ifndef BSP_HELPERS_H
# define BSP_HELPERS_H

# include "bsp.h"

void delete_by_index(t_vertex *points, int index, int *count);
void insert_by_index(t_vertex *points, int index, int *count, t_vertex p);
void delete_wall_by_index(t_wall *walls, int index, int *count);
void insert_wall_by_index(t_wall *walls, int index, int *count, t_wall wall);
void delete_circuit_by_index(t_circuit *circuits, int index, int *count);

int get_i_plus_2(int i, int max);
int get_i_minus_1(int i, int max);
int get_i_plus_1(int i, int max);



#endif