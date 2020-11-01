#include "LIBUI.h"

int LIBUI_GetButtonWithFunction(t_LIBUI_Button *buttons, int c_buttons, char *function)
{
    int i;

    i = 0;
    while (i < c_buttons)
    {
        if (!ft_strcmp(buttons[i].function, function))
            return (i);
        i++;
    }
    return (-1);
}

int LIBUI_GetListWithFunction(t_LIBUI_List *lists, int c_lists, char *function)
{
    int i;

    i = 0;
    while (i < c_lists)
    {
        if (!ft_strcmp(lists[i].items[0].function, function))
            return (i);
        i++;
    }
    return (-1);
}

int LIBUI_GetEditWithFunction(t_LIBUI_Edit *edits, int c_edits, char *function)
{
    int i;

    i = 0;
    while (i < c_edits)
    {
        if (!ft_strcmp(edits[i].function, function))
            return (i);
        i++;
    }
    return (-1);
}