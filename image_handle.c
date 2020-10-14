#include "duke.h"

void	put_pixel(int *image_data, int x, int y, int color){
	int index = ((H_2 - y - 1) * W + W_2 + x);
	if (index >= 0 && index < HxW && x > -W_2 && x < W_2)
		image_data[index] = color;
}

int		get_texel(SDL_Surface *image_data, float u, float v)
{
	int pitch = image_data->w;
	int index = (int)(image_data->h * v) * pitch + (int)(pitch * u);
	int *pixels = (int *)image_data->pixels;
	if (index >= 0 && index < image_data->w * image_data->h) /////////optimize
		return (pixels[index]);
	return 0;
}

int		get_texel_n(int **image_data, float u, float v, int pitch)
{
	//int index = (int)(pitch * v) * pitch + (int)(pitch * u);
	//if (index >= 0 && index < 250000)
	int x = (int)(u * pitch);
	int y = (int)(v * pitch);
	if (x >= 0 && y >= 0 && x < 500 && y < 500)
		return (image_data[y][x]);
	return 0;
}