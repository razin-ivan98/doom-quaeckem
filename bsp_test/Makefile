
UNAME_S := $(shell uname -s)
FTPATH = ./../libft
FRAMEWORKS = -F./../frameworks -rpath ./../frameworks -framework SDL2 \
	-framework OpenGL -framework AppKit

INCLUDES = -I./../libft/ -I./../frameworks/SDL2.framework/Headers -I./../my_graphics_lib -I./../

SRCS = bsp.c ../my_graphics_lib/my_graphics_lib.c ../image_handle.c ../linear_algebra.c \
	save_json.c ftoa.c ../helpers.c bsp_helpers.c export.c

all:
ifeq ($(UNAME_S), Darwin)
	@echo MAC OS
	make -C $(FTPATH)
	gcc -g3 $(SRCS) -lm -L./../libft -lft $(FRAMEWORKS) $(INCLUDES) -o BSP
else
	@echo Linux
	make -C $(FTPATH)
	gcc -I -g3 $(INCLUDES) $(SRCS) -lSDL2 -lm -L./../libft -lft -o BSP
endif
	@echo ГОТОВО ЕПТА


	
	
