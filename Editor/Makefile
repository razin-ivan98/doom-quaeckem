
NAME = Editor

FLAGS = -Wextra -Wall -Werror


OBJECTS = editor.o my_graphics_lib.o image_handle.o linear_algebra.o \
				save_json.o ftoa.o helpers.o bsp_helpers.o export.o hash.o init.o \
				draw_elements.o draw.o draw_trs_by_bsp.o mouse_handle.o add_elements_to_map.o\
				console_instructions_1.o console_instructions_2.o cursor_on_line_on_point.o \
				new_sector_new_point.o select_sector_by_bsp.o plus_minus_enter_keys.o bsp_recurse.o


SOURCES = editor.c ../my_graphics_lib/my_graphics_lib.c ../image_handle.c ../linear_algebra.c \
				save_json.c ftoa.c ../helpers.c bsp_helpers.c export.c hash.c init.c \
				draw_elements.c draw.c draw_trs_by_bsp.c mouse_handle.c add_elements_to_map.c \
				console_instructions_1.c console_instructions_2.c cursor_on_line_on_point.c \
				new_sector_new_point.c select_sector_by_bsp.c plus_minus_enter_keys.c bsp_recurse.c

LIBS = -L ../libft -lft -lm -framework OpenGL\
	-framework AppKit -framework OpenCL -framework OpenGL

INCLUDES = -I ../libft/ -I ../frameworks/SDL2.framework/Headers -I ../my_graphics_lib

FRAMEWORKS = -F ../frameworks -rpath ../frameworks -framework SDL2 \
	-framework SDL2_ttf -framework SDL2_mixer -framework SDL2_image

FTPATH = ../libft

all: $(NAME)

$(NAME): $(OBJECTS)
	gcc $(FLAGS) $(OBJECTS) $(INCLUDES) $(LIBS) $(FRAMEWORKS) -o $(NAME)

$(OBJECTS):
	make -C $(FTPATH)
	gcc $(FLAGS) $(INCLUDES) -c $(SOURCES) 
	

clean:
	make -C $(FTPATH) clean
	/bin/rm -f $(OBJECTS)

fclean: clean
	make -C $(FTPATH) fclean
	/bin/rm -f $(NAME)

re: fclean all