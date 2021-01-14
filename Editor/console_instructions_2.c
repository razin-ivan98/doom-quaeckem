#include "editor.h"

void	write_floor_height(t_map_editor *ed)
{
	char str[64];

	ft_putstr("Высота пола: ");
	ftoa(ed->map.circuits[ed->map.selected_circuit].floor, 8, str);
	ft_putendl(str);
}
void	write_ceil_height(t_map_editor *ed)
{
	char str[64];

	ft_putstr("Высота потолка: ");
	ftoa(ed->map.circuits[ed->map.selected_circuit].ceil, 8, str);
	ft_putendl(str);
}
void	write_floor_tex(t_map_editor *ed)
{
	char str[64];

	ft_putstr("Текстура пола: ");
	itoa(ed->map.circuits[ed->map.selected_circuit].floor_tex, str);
	ft_putendl(str);
}
void	write_ceil_tex(t_map_editor *ed)
{
	char str[64];

	ft_putstr("Текстура потолка: ");
	itoa(ed->map.circuits[ed->map.selected_circuit].ceil_tex, str);
	ft_putendl(str);
}
void	write_wall_tex(t_map_editor *ed)
{
	char str[64];

	ft_putstr("Текстура стен: ");
	itoa(ed->map.circuits[ed->map.selected_circuit].wall_tex, str);
	ft_putendl(str);
}