#include "./game_test.h"

int		classify_point_s(t_vertex cam, t_vertex line, t_vertex normal)
{
	t_vertex new;
	t_vertex ort;

	cam.z = 0.0;
	normal.z = 0.0;

	ort = get_ort_line_by_point(line, cam);
	new = lines_intersect(line, ort);

	printf("line %f\t%f\t%f\n", line.x, line.y, line.z);
	printf("ort %f\t%f\t%f\n", ort.x, ort.y, ort.z);

	new.z = 0.0;

	new = sub(cam, new);
	
	printf("%f\t%f\t%f\n", new.x, new.y, new.z);
	printf("%f\t%f\t%f\n", normal.x, normal.y, normal.z);
	printf("%f\n\n", dot(new, normal));

	if (dot(new, normal) < 0.0)
		return (BACK);
	return (FRONT);
}

int		check_leaf(t_bsp *node, t_vertex pos)
{
	int			i;
	t_vertex	line;

	i = 0;
	while (i < node->walls_count)
	{
		node->walls[i].failed = 0;
		if (node->walls[i].type == WALL_TYPE_WALL)
		{
			node->walls[i].failed = 1;
			line = get_line_by_points(node->walls[i].points[0],
							node->walls[i].points[1]);
			if (classify_point_s(pos, line, node->walls[i].normal) == BACK)
			{
				node->walls[i].failed = 2;
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int		bsp_solid_traversal(t_bsp *node, t_vertex pos)
{
	if (node->is_leaf)
	{
		return (check_leaf(node, pos));
	}
	else
	{
		if (classify_point(pos, node->line, node->normal) == BACK)
		{
			return (bsp_solid_traversal(node->front, pos));
		}
		else
		{
			return (bsp_solid_traversal(node->back, pos));
		}
	}
}

void	bsp_trav_zero(t_bsp *node)
{
	if (node->is_leaf)
	{
		int i = 0;
		while (i < node->walls_count)
		{
			node->walls[i].failed = 0;
			i++;
		}
	}
	else
	{
		bsp_trav_zero(node->front);
		bsp_trav_zero(node->back);
	}
	
}

int		if_possible_to_move(t_vertex pos, t_bsp *node)
{
	bsp_trav_zero(node);
	return (bsp_solid_traversal(node, pos));
}

void	update_game(t_map_editor *ed)
{
	t_vertex new_pos;

	if (ed->a_pressed)
	{
		new_pos = ed->map.player.pos;
		new_pos.x -= ed->map.player.speed;
		if (if_possible_to_move(new_pos, &ed->map.root))
			ed->map.player.pos = new_pos;
	}	
	if (ed->w_pressed)
	{
		new_pos = ed->map.player.pos;
		new_pos.y += ed->map.player.speed;
		if (if_possible_to_move(new_pos, &ed->map.root))
			ed->map.player.pos = new_pos;
	}
	if (ed->d_pressed)
	{
		new_pos = ed->map.player.pos;
		new_pos.x += ed->map.player.speed;
		if (if_possible_to_move(new_pos, &ed->map.root))
			ed->map.player.pos = new_pos;
	}
	if (ed->s_pressed)
	{
		new_pos = ed->map.player.pos;
		new_pos.y -= ed->map.player.speed;
		if (if_possible_to_move(new_pos, &ed->map.root))
			ed->map.player.pos = new_pos;
	}
}
