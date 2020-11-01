#ifndef LIBUI_H
# define LIBUI_H
# include "frameworks/SDL2.framework/Headers/SDL.h"
#include "frameworks/SDL2_image.framework/Headers/SDL_image.h"
#include "frameworks/SDL2_ttf.framework/Headers/SDL_ttf.h"
#include "../libft/libft.h"

typedef struct s_but_constr
{
    int x;
    int y;
    char *text;
    char *function;
    int color;
}               t_but_constr;

typedef struct s_edit_constr
{
    int x;
    int y;
    char *text;
    char *function;
    int color;
}               t_edit_constr;

typedef struct s_list_constr
{
    int x;
    int y;
    char *text;
    char *function;
    int color;
	char items_text[20][64];
	char items_function[20][64];
	int				c_items;
}               t_list_constr;

typedef struct  s_LIBUI_Button
{
    int x;
    int y;
    SDL_Surface *surface;
    SDL_Surface *pressed;
    char function[64];
    int is_pressed;
}               t_LIBUI_Button;

typedef struct		s_LIBUI_Edit
{
	int				x;
	int				y;
	int				is_active;
	char			text[64];
	SDL_Surface		*surface;
	SDL_Surface		*active;
	SDL_Surface		*with_text;
	SDL_Surface		*with_text_active;
	char			function[64];
}					t_LIBUI_Edit;

typedef struct		s_LIBUI_List
{
	int				x;
	int				y;
	/*int				width;
	int				height;*/
	/*int				start;
	int				end;*/
	/*int				item;
	int				select_item;*/
	/*char			function[128];*/
	int				is_dropped;
	/*int				expand;*/
	/*void			(*ft_redraw)(void *rtv1);*/
	/*SDL_Surface		*surface;
	SDL_Surface		*pressed;*/

	//t_LIBUI_Button	main;
	t_LIBUI_Button	*items;
	int				c_items;
}					t_LIBUI_List;

typedef struct		s_list_item_addr
{
	int list;
	int item;
}					t_list_item_addr;


void		LIBUI_drop_list(void *rtv1, int x, int y);

    int LIBUI_IsButtonPressed(int x, int y, t_LIBUI_Button *buttons, int c_buttons);
    void LIBUI_NewButton(t_but_constr button, t_LIBUI_Button *buttons, int *c_buttons);

	t_LIBUI_Button LIBUI_CreateButton(t_but_constr input);
	void LIBUI_NewList(t_list_constr list, t_LIBUI_List *lists, int *c_lists);

	t_list_item_addr LIBUI_IsListPressed(int x, int y, t_LIBUI_List *lists, int c_lists);


	void LIBUI_NewEdit(t_edit_constr edit, t_LIBUI_Edit *edits, int *c_edits);
	int LIBUI_IsEditPressed(int x, int y, t_LIBUI_Edit *edits, int c_edits);
	void LIBUI_EditRefresh(t_LIBUI_Edit *edits, int number);


	void LIBUI_DisactiveAll(t_LIBUI_Edit *edits, int c_edits, t_LIBUI_List *lists, int c_lists);

	void LIBUI_InputLetter(int key, t_LIBUI_Edit *edits, int number, int shift);


	int LIBUI_GetButtonWithFunction(t_LIBUI_Button *buttons, int c_buttons, char *function);
	int LIBUI_GetListWithFunction(t_LIBUI_List *lists, int c_lists, char *function);
	int LIBUI_GetEditWithFunction(t_LIBUI_Edit *edits, int c_edits, char *function);
#endif