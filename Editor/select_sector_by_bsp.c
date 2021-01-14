#include "editor.h"

int			classify_point(t_vertex cam, t_vertex line, t_vertex normal)
{
	t_vertex new;

	cam.z = 0.0;
	normal.z = 0.0;

	new = (t_vertex){cam.x, line.x * cam.x + line.y, 0.0};

	if (dot(sub(new, cam), normal) < 0.0)
		return (BACK);
	return (FRONT);
}


int			bsp_select_circuit_traversal(t_bsp *node, t_vertex pos)
{
	if (node->is_leaf)
	{
		return (node->circuit);
	}
	else
	{
		if (classify_point(pos, node->line, node->normal) == BACK)
		{
			return (bsp_select_circuit_traversal(node->front, pos));
		}
		else
		{
			return (bsp_select_circuit_traversal(node->back, pos));
		}
	}
}
