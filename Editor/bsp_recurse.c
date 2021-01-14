#include "editor.h"


int		if_leaf(t_bsp *bsp, t_map *map, t_int_v q, t_circuit *circuits)
{
	int i;
	int j;

	if (q.x == -1)
	{
		bsp->walls_count = 0;
		bsp->is_leaf = 1;
		i = 0;
		while (i < q.y)
		{
			j = 0;
			while (j < circuits[i].walls_count)
			{
				bsp->floor = map->circuits[i].floor;
				bsp->ceil = map->circuits[i].ceil;
				bsp->circuit = i;
				bsp->walls[bsp->walls_count++] = circuits[i].walls[j];
				j++;
			}
			i++;
		}
		free (circuits);
		return (1);
	}
	return (0);
}

void	bsp_recurse(t_bsp *bsp, t_circuit *circuits, int circuits_count, t_map *map)
{
	t_circuit	*front;
	t_circuit	*back;
	int		i;
	int		j;
	int		result;
	int		cutter_cir;
	int		cutter_wall;
	t_wall	cutter;

	t_wall	new1;
	t_wall	new2;


	t_vertex cutter_line;
	bsp->front = NULL;
	bsp->back = NULL;

	reconstruct_circuits(circuits, circuits_count);
	get_cutter(circuits, circuits_count, &cutter_cir, &cutter_wall);
	front = NULL;
	back = NULL;
	if (if_leaf(bsp, map, (t_int_v){cutter_cir, circuits_count}, circuits))
		return ;

	cutter = get_big_wall(circuits[cutter_cir].walls[cutter_wall], bsp, &cutter_line);

	if (!(front = malloc(sizeof(t_circuit) * circuits_count)) ||
			!(back = malloc(sizeof(t_circuit) * circuits_count)))
		exit(-2);

	ft_bzero(front, sizeof(t_circuit) * circuits_count);
	ft_bzero(back, sizeof(t_circuit) * circuits_count);

	bsp->is_leaf = 0;
	bsp->walls_count = 0;
	bsp->normal = circuits[cutter_cir].walls[cutter_wall].normal;


	if (!(bsp->back = malloc(sizeof(t_bsp))) || !(bsp->front = malloc(sizeof(t_bsp))))	
		exit(-2);

	i = 0;
	while (i < circuits_count)
	{
		j = 0;
		while (j < circuits[i].walls_count)
		{
			t_vertex inter;
			result = classify_wall(cutter_line, bsp->normal, circuits[i].walls[j], &inter);

			if (result == CUTTED)
			{
				new1 = circuits[i].walls[j];
				new2 = circuits[i].walls[j];

				new1.points[0] = circuits[i].walls[j].points[0];

				new1.points[1] = inter;
				new2.points[0] = new1.points[1];
				new2.points[1] = circuits[i].walls[j].points[1];

				result = classify_wall(cutter_line, bsp->normal, new1, &inter);
				if (result == FRONT)
				{
					front[i].walls[front[i].walls_count] = new1;
					front[i].walls_count++;
					back[i].walls[back[i].walls_count] = new2;
					back[i].walls_count++;
				}
				else
				{
					front[i].walls[front[i].walls_count] = new2;
					front[i].walls_count++;
					back[i].walls[back[i].walls_count] = new1;
					back[i].walls_count++;	
				}
			}
			else if (result == FRONT)
			{
				front[i].walls[front[i].walls_count] = circuits[i].walls[j];
				front[i].walls_count++;
			}
			else if (result == BACK)
			{
				back[i].walls[back[i].walls_count] = circuits[i].walls[j];
				back[i].walls_count++;
			}
			else
			{
				cutter = circuits[cutter_cir].walls[cutter_wall];
				circuits[cutter_cir].walls[cutter_wall].border_id = -1;
				circuits[i].walls[j].border_id = -1;


				if (i != cutter_cir)
				{
					circuits[i].walls[j].used_in_bsp = 1;
					if (circuits[i].walls[j].type == WALL_TYPE_SECTOR_BORDER)
					{
						circuits[i].walls[j].circuit = cutter_cir;
					}
					back[i].walls[back[i].walls_count] = circuits[i].walls[j];
					back[i].walls_count++;
				}
				else 
				{
					if (circuits[i].walls[j].type == WALL_TYPE_SECTOR_BORDER)
						circuits[i].walls[j].circuit = -1;
					circuits[i].walls[j].used_in_bsp = 1;
					front[i].walls[front[i].walls_count] = circuits[i].walls[j];
					front[i].walls_count++;
				}
			}
			j++;
		}
		i++;
	}
	free(circuits);
	bsp_recurse(bsp->back, back, circuits_count, map);
	bsp_recurse(bsp->front, front, circuits_count, map);
}