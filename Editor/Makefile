
NAME = Editor

FLAGS = -Wextra -Wall -Werror -fsanitize=address


OBJECTS = editor.o my_graphics_lib.o image_handle.o linear_algebra.o \
				ftoa.o helpers.o bsp_helpers.o export.o export1.o export2.o export3.o \
				hash.o init.o \
				draw_elements.o draw.o draw_trs_by_bsp.o mouse_handle.o add_elements_to_map.o\
				console_instructions_1.o console_instructions_2.o cursor_on_line_on_point.o \
				new_sector_new_point.o select_sector_by_bsp.o plus_minus_enter_keys.o bsp_recurse.o\
				reconstruct_circuits.o key_down.o get_cutter.o computing_walls_1.o computing_walls_2.o \
				save_json_1.o save_json_2.o save_json_3.o hash_2.o \

SOURCES = editor.c ../my_graphics_lib/my_graphics_lib.c ../image_handle.c ../linear_algebra.c \
				../ftoa.c ../helpers.c bsp_helpers.c export.c export1.c export2.c export3.c \
				hash.c init.c \
				draw_elements.c draw.c draw_trs_by_bsp.c mouse_handle.c add_elements_to_map.c \
				console_instructions_1.c console_instructions_2.c cursor_on_line_on_point.c \
				new_sector_new_point.c select_sector_by_bsp.c plus_minus_enter_keys.c bsp_recurse.c \
				reconstruct_circuits.c key_down.c get_cutter.c computing_walls_1.c computing_walls_2.c \
				save_json_1.c save_json_2.c save_json_3.c hash_2.c \

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