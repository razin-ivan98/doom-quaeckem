all: 
	gcc -g3 main.c image_handle.c draw.c interpolate.c linear_algebra.c depth_buffer.c\
				get_next_line/get_next_line.c load_obj.c read_map.c my_graphics_lib.c create_box.c\
				-lSDL2 -lm -L./libft -lft