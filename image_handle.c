#include "duke.h"

void	put_pixel(int *image_data, int x, int y, int color){
	int index = ((H_2 - y - 1) * W + W_2 + x);
	if (index >= 0 && index < HxW && x > -W_2 && x < W_2)
		image_data[index] = color;
}

int		get_texel(SDL_Surface *image_data, float u, float v)
{
	int pitch = image_data->w;

	u = u - floor(u);
	v = v - floor(v);

	int index = (int)(image_data->h * v) * pitch + (int)(pitch * u);
	int *pixels = (int *)image_data->pixels;
	if (index >= 0 && index < image_data->w * image_data->h) /////////optimize
		return (pixels[index]);
	return 0;
}
