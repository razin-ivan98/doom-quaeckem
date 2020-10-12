
UNAME_S := $(shell uname -s)
FTPATH = ./../libft
FRAMEWORKS = -F./../frameworks -rpath ./../frameworks -framework SDL2 \
	-framework OpenGL -framework AppKit

INCLUDES = -I./../libft/ -I./../frameworks/SDL2.framework/Headers -I./../my_graphics_lib -I./../

all:
ifeq ($(UNAME_S), Darwin)
	@echo MAC OS
	make re -C $(FTPATH)
	gcc -g map_editor.c ../my_graphics_lib/my_graphics_lib.c ../image_handle.c ../linear_algebra.c \
	-lm -L./../libft -lft \
	$(FRAMEWORKS) $(INCLUDES) -o MapEditor
else
	@echo Linux
	make -C $(FTPATH)
	gcc -g3 -I $(INCLUDES) map_editor.c ../my_graphics_lib/my_graphics_lib.c ../image_handle.c ../linear_algebra.c -lSDL2 -lm -L./../libft -lft -o MapEditor
endif
	@echo ГОТОВО ЕПТА


	
	
