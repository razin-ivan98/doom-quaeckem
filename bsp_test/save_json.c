#include "bsp.h"

// void	check_state(int state, int fd)
// {
// 	if (state == 1)
// 		ft_putstr_fd("\"front\": {\n", fd);
// 	else if (state == 2)
// 		ft_putstr_fd("\"back\": {\n", fd);
// }

void	check_walls(t_bsp *node, int i, int fd)
{
	char *str = malloc(64000);

	ft_putstr_fd("{\n\t\t\t\"points\": [\n\t\t\t\t[", fd);
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


// char *bsp_json_save_traversal(t_bsp *node, int state, int fd, int nl)
// {
// 	char *str = malloc(64000);
// 	int i = 0;

// 	if (node->is_leaf == 0)
// 	{
// 		//nl--;
// 		check_state(state, fd);
// 		check_not_leaf(node, fd);
// 		bsp_json_save_traversal(node->front, 1, fd, nl);
// 		bsp_json_save_traversal(node->back, 2, fd, nl);
// 	}
// 	else
// 	{
// 		check_state(state, fd);
// 		ft_putstr_fd("\t\"isLeaf\": ", fd);
// 		str = itoa(node->is_leaf, str);
// 		ft_putstr_fd(str, fd);
// 		ft_putstr_fd(",\n\t\"walls\": [\n\t\t\t", fd);
// 		while (i < node->walls_count)
// 		{
// 			check_walls(node, i, fd);
// 			if (i == node->walls_count - 1)
// 			{
// 				ft_putstr_fd("\n\t\t]\n\t}", fd);
// 				if (!(nl == 0 && state == 2))
// 					ft_putstr_fd(",\n", fd);
// 			}
// 			else
// 				ft_putstr_fd(",\n\t\t\t", fd);
// 			i++;
// 		}
// 	}
// 	return (str);
// }

void	bsp_save_traversal(t_bsp *node, int fd);

void	bsp_save_traversal(t_bsp *node, int fd)
{
	ft_putstr_fd("{\n", fd);

	char *str = malloc(64000);
	int i = 0;

	if (node->is_leaf == 0)
	{
		//check_state(state, fd);
		check_not_leaf(node, fd);
		ft_putstr_fd("\"front\": ", fd);

		bsp_save_traversal(node->front, fd);
		ft_putstr_fd(",\n\"back\": ", fd);

		bsp_save_traversal(node->back, fd);
	}
	else
	{
		//check_state(state, fd);
		ft_putstr_fd("\t\"isLeaf\": ", fd);
		str = itoa(node->is_leaf, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(",\n\t\"walls\": [\n\t\t\t", fd);
		while (i < node->walls_count)
		{
			check_walls(node, i, fd);
			if (i == node->walls_count - 1)
			{
				ft_putstr_fd("\n\t\t]\n", fd);
				//if (!(nl == 0 && state == 2))
					//ft_putstr_fd(",\n", fd);
			}
			else
				ft_putstr_fd(",\n\t\t\t", fd);
			i++;
		}
	}
	ft_putstr_fd("}\n", fd);
}

void	save_json(t_bsp *root, int not_leaves)
{
	int fd;
	char *str;
	int i;

	i = 0;
	fd = open("new_saved_bsp.json", O_CREAT|O_RDWR|O_TRUNC, 0644);
	// ft_putstr_fd("{\n", fd);
	bsp_save_traversal(root, fd);
	//while (i < not_leaves)
//	{
		// ft_putstr_fd("}\n", fd);
//		i++;
//	}
	close(fd);
}