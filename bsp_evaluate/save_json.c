#include "bsp_eval.h"

/*
	понадобится рекурсия
	что то вроде

	char *bsp_json_save_traversal(t_bsp *node)
	{
		char *str = malloc(64000)

		записываем свойства этого узла

		рекурсивно получаем запись дочерних узлов

		char *front = bsp_traversal(node->front)
		char *back = bsp_traversal(node->back)

		ft_strcat(str, "front: ")
		ft_strcat(str, front)

		ft_strcat(str, ",")
	
		ft_strcat(str, "back: ")
		ft_strcat(str, back)

		return (str)
	}
*/

void	save_json(t_bsp *root)
{

}