
UNAME_S := $(shell uname -s)
FTPATH = ./libft
FRAMEWORKS = -F./frameworks -rpath ./frameworks -framework SDL2 \
	-framework OpenGL -framework AppKit

INCLUDES = -I./libft/ -I./frameworks/SDL2.framework/Headers -I./my_graphics_lib

SRCS = main.c image_handle.c draw.c interpolate.c linear_algebra.c depth_buffer.c \
				get_next_line/get_next_line.c load_obj.c my_graphics_lib/my_graphics_lib.c\
				render_init.c textures_handling.c clip_triangle_by_planes.c \
				clip_1_outside.c clip_2_outsides.c render_triangle.c

all:
ifeq ($(UNAME_S), Darwin)
	@echo MAC OS
	make -C $(FTPATH)
	gcc -g $(SRCS) -lm -L./libft -lft $(FRAMEWORKS) $(INCLUDES) -o DoomQuaeckem
else
	@echo Linux
	make -C $(FTPATH)
	gcc -g3 $(SRCS) -lSDL2 -lm -L ./libft -lft -o DoomQuaeckem
endif
	@echo ГОТОВО ЕПТА
