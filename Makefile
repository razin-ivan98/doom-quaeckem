all: 
	gcc -g main.c image_handle.c draw.c interpolate.c linear_algebra.c depth_buffer.c  -lSDL2 -lm