
UNAME_S := $(shell uname -s)
FTPATH = ./../libft
FRAMEWORKS = -F./../frameworks -rpath ./../frameworks -framework SDL2 \
	-framework OpenGL -framework AppKit

INCLUDES = -I./../libft/ -I./../frameworks/SDL2.framework/Headers -I./../my_graphics_lib -I./../

SRCS = save_json.c bsp_eval.c ../my_graphics_lib/my_graphics_lib.c ../image_handle.c ../linear_algebra.c

all:
ifeq ($(UNAME_S), Darwin)
	@echo MAC OS
	make -C $(FTPATH)
	gcc -g $(SRCS) -lm -L./../libft -lft $(FRAMEWORKS) $(INCLUDES) -o BSPEval
else
	@echo Linux
	make -C $(FTPATH)
	gcc -g3 -I $(INCLUDES) $(SRCS) -lSDL2 -lm -L./../libft -lft -o BSPEval
endif
	@echo ГОТОВО ЕПТА


	
	
