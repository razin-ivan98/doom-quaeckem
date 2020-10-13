
UNAME_S := $(shell uname -s)
FTPATH = ./libft
FRAMEWORKS = -F./frameworks -rpath ./frameworks -framework SDL2 \
	-framework OpenGL -framework AppKit

INCLUDES = -I./libft/ -I./frameworks/SDL2.framework/Headers -I./my_graphics_lib

all:
ifeq ($(UNAME_S), Darwin)
	@echo MAC OS
	make re -C $(FTPATH)
	gcc -g main.c image_handle.c draw.c interpolate.c linear_algebra.c depth_buffer.c \
				get_next_line/get_next_line.c load_obj.c my_graphics_lib/my_graphics_lib.c\
				render_init.c textures_handling.c\
				-lm -L./libft -lft $(FRAMEWORKS) $(INCLUDES) -o DoomQuaeckem
else
	@echo Linux
	make -C $(FTPATH)
	gcc -g3 main.c image_handle.c draw.c interpolate.c linear_algebra.c depth_buffer.c \
				get_next_line/get_next_line.c load_obj.c my_graphics_lib/my_graphics_lib.c\
				render_init.c textures_handling.c\
				-lSDL2 -lm -L ./libft -lft -o DoomQuaeckem
endif
	@echo ГОТОВО ЕПТА
