#include "duke.h"

void	put_pixel(int *image_data, int x, int y, int color){
	int index = ((H_2 - y - 1) * W + W_2 + x);
	if (index >= 0 && index < HxW && x > -W_2 && x < W_2)
		image_data[index] = color;
}

int		get_texel(int *image_data, float u, float v, int pitch)
{
	int index = (int)(pitch * v) * pitch + (int)(pitch * u);
	if (index >= 0 && index < 250000)
		return (image_data[index]);
	return 0;
}