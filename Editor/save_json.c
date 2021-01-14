#include "editor.h"

void	check_walls(t_bsp *node, int i, int fd)
{
	char str[64];

	ft_putstr_fd("{\n\t\t\t\"type\":", fd);
	itoa(node->walls[i].type, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ft_putstr_fd("\n\t\t\t\"points\": [\n\t\t\t\t[", fd);
	ftoa(node->walls[i].points[0].x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->walls[i].points[0].y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n\t\t\t\t[", fd);
	ftoa(node->walls[i].points[1].x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->walls[i].points[1].y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("]\n\t\t\t],\n\t\t\t\"normal\": [", fd);
	ftoa(node->walls[i].normal.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->walls[i].normal.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("]\n\t\t\t}", fd);
}

void	check_objects(t_map_editor *ed, int fd)
{
	int i;
	char str[64];

	i = 0;
	ft_putstr_fd("\"objects\": [", fd);
	while (i < ed->objects_count)
	{
		ft_putstr_fd("{\n\t\t\t\"index\":", fd);
		itoa(ed->objects[i].index, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ft_putstr_fd("\n\t\t\t\"pos\": [", fd);
		ftoa(ed->objects[i].pos.x, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ftoa(ed->objects[i].pos.y, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("]}", fd);
		if (i < ed->objects_count - 1)
			ft_putstr_fd(",", fd);
		i++;
	}
	ft_putstr_fd("],", fd);

}

void	check_vt_trs(t_bsp *node, int i, int fd)
{
	char str[64];
	ft_putstr_fd("{", fd);

	ft_putstr_fd("\n\t\t\t\"ids\": [\n\t\t\t\t", fd);
	itoa(node->vt_trs[i].ids[0], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].ids[1], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].ids[2], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);

	ft_putstr_fd("\n\t\t\t\"n_ids\": [\n\t\t\t\t", fd);
	itoa(node->vt_trs[i].n_ids[0], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].n_ids[1], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].n_ids[2], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);

	ft_putstr_fd("\n\t\t\t\"uv_ids\": [\n\t\t\t\t", fd);
	itoa(node->vt_trs[i].uv_ids[0], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].uv_ids[1], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	itoa(node->vt_trs[i].uv_ids[2], str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);

	ft_putstr_fd("\n\t\t\t\"type\": \n\t\t\t\t", fd);
	itoa(node->vt_trs[i].type, str);
	ft_putstr_fd(str, fd);


	ft_putstr_fd("}", fd);
}

void	check_not_leaf(t_bsp *node, int fd)
{
	char str[64];

	ft_putstr_fd("\t\"isLeaf\": ", fd);
	itoa(node->is_leaf, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(",\n\t\"line\": [", fd);
	ftoa(node->line.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->line.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n\t\"normal\": [", fd);
	ftoa(node->normal.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(node->normal.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],\n", fd);
}


void	bsp_save_traversal(t_bsp *node, int fd)
{
	

	char str[64];

	int i = 0;

	ft_putstr_fd("{\n", fd);

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
		itoa(node->is_leaf, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(",\t\"floor\": ", fd);
		ftoa(node->floor, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(",\t\"ceil\": ", fd);
		ftoa(node->ceil, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(",\t\"wall_tex\": ", fd);
		itoa(node->wall_tex, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(",\t\"floor_tex\": ", fd);
		itoa(node->floor_tex, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(",\t\"ceil_tex\": ", fd);
		itoa(node->ceil_tex, str);
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

		i = 0;
		ft_putstr_fd(",\n\t\"vt_trs\": [\n\t\t\t", fd);
		while (i < node->vt_trs_count)
		{
			check_vt_trs(node, i, fd);
			if (i != node->vt_trs_count - 1)
			
				ft_putstr_fd(",\n\t\t\t", fd);
			i++;
		}
		ft_putstr_fd("\n\t\t]\n", fd);
	}
	ft_putstr_fd("}\n", fd);
}

void	check_enemies(t_map_editor *ed, int fd)
{
	char str[64];
	int i;

	ft_putstr_fd("\"enemies\": [", fd);
	i = 0;
	while (i < ed->enemies_count)
	{
		ft_putstr_fd("[", fd);
		ftoa(ed->enemies[i].x, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ftoa(ed->enemies[i].y, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("]", fd);
		if (i < ed->enemies_count - 1)
		{
			ft_putstr_fd(",\n", fd);
		}
		i++;
	}
	
	ft_putstr_fd("],", fd);
}


void	check_ammo(t_map_editor *ed, int fd)
{
	char str[64];
	int i;

	ft_putstr_fd("\"ammo\": [", fd);
	i = 0;
	while (i < ed->ammo_count)
	{
		ft_putstr_fd("[", fd);
		ftoa(ed->ammo[i].x, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ftoa(ed->ammo[i].y, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("]", fd);
		if (i < ed->ammo_count - 1)
		{
			ft_putstr_fd(",\n", fd);
		}
		i++;
	}
	
	ft_putstr_fd("],", fd);
}

void	check_aid(t_map_editor *ed, int fd)
{
	char str[64];
	int i;

	ft_putstr_fd("\"aid\": [", fd);
	i = 0;
	while (i < ed->aid_count)
	{
		ft_putstr_fd("[", fd);
		ftoa(ed->aid[i].x, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd(", ", fd);
		ftoa(ed->aid[i].y, 8, str);
		ft_putstr_fd(str, fd);
		ft_putstr_fd("]", fd);
		if (i < ed->aid_count - 1)
		{
			ft_putstr_fd(",\n", fd);
		}
		i++;
	}
	
	ft_putstr_fd("],", fd);
}

void	check_player(t_map_editor *ed, int fd)
{
	char str[64];

	ft_putstr_fd("\"player\": [", fd);
	ftoa(ed->player.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(ed->player.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
}
void	check_aim(t_map_editor *ed, int fd)
{
	char str[64];

	ft_putstr_fd("\"aim\": [", fd);
	ftoa(ed->aim.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(ed->aim.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
}

void	check_tv(t_map_editor *ed, int fd)
{
	char str[64];

	ft_putstr_fd("\"tv\": { \"pos\":", fd);
	ft_putstr_fd("[", fd);
	ftoa(ed->tv.x, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd(", ", fd);
	ftoa(ed->tv.y, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("],", fd);
	ft_putstr_fd("\"beta\":", fd);
	ftoa(ed->tv_beta, 8, str);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("},", fd);

}

void	save_json(t_bsp *root, t_map_editor *ed)
{
	int fd;

	fd = open("data.json", O_CREAT | O_RDWR| O_TRUNC, 0644);
	ft_putstr_fd("{\n", fd);
	check_player(ed, fd);
	check_aim(ed, fd);
	check_tv(ed, fd);
	check_enemies(ed, fd);
	check_ammo(ed, fd);
	check_objects(ed, fd);
	check_aid(ed, fd);
	ft_putstr_fd("\"bsp\" : ", fd);
	bsp_save_traversal(root, fd);
	ft_putstr_fd("}\n", fd);

	close(fd);
}