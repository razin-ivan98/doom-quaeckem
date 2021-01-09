#include "bsp.h"

void	check_walls(t_bsp *node, int i, int fd)
{
	char *str = malloc(64000);

	ft_putstr_fd("{\n\t\t\t\"type\":", fd);
	str = itoa(node->walls[i].type, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ft_putstr_fd("\n\t\t\t\"points\": [\n\t\t\t\t[", fd);
	str = ftoa(node->walls[i].points[0].x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	str = ftoa(node->walls[i].points[0].y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n\t\t\t\t[", fd);
	str = ftoa(node->walls[i].points[1].x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	str = ftoa(node->walls[i].points[1].y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("]\n\t\t\t],\n\t\t\t\"normal\": [", fd);
	str = ftoa(node->walls[i].normal.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	str = ftoa(node->walls[i].normal.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("]\n\t\t\t}", fd);
	free(str);
}

void	check_not_leaf(t_bsp *node, int fd)
{
	char *str = malloc(64000);

	ft_putstr_fd("\t\"isLeaf\": ", fd);
	str = itoa(node->is_leaf, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\n\t\"line\": [", fd);
	str = ftoa(node->line.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	str = ftoa(node->line.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n\t\"normal\": [", fd);
	str = ftoa(node->normal.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	str = ftoa(node->normal.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n", fd);
	free(str);
}


void	bsp_save_traversal(t_bsp *node, int fd)
{
	ft_putstr_fd("{\n", fd);

	char *str = malloc(64000);
	int i = 0;

	if (node->is_leaf == 0)
	{
		check_not_leaf(node, fd);
		ft_putstr_fd("\"front\": ", fd);

		bsp_save_traversal(node->front, fd);
		ft_putstr_fd(",\n\"back\": ", fd);

		bsp_save_traversal(node->back, fd);
	}
	else
	{
		ft_putstr_fd("\t\"isLeaf\": ", fd);
		str = itoa(node->is_leaf, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(",\n\t\"walls\": [\n\t\t\t", fd);
		while (i < node->walls_count)
		{
			check_walls(node, i, fd);
			if (i != node->walls_count - 1)
			
				ft_putstr_fd(",\n\t\t\t", fd);
			i++;
		}
		ft_putstr_fd("\n\t\t]\n", fd);
	}
	ft_putstr_fd("}\n", fd);
}

void	save_json(t_bsp *root)
{
	int fd;
	char *str;
	int i;

	i = 0;
	fd = open("new_saved_bsp.json", O_CREAT|O_RDWR|O_TRUNC, 0644);
	bsp_save_traversal(root, fd);

	close(fd);
}