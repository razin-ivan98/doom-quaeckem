#include "duke.h"

void	put_pixel(int *image_data, int x, int y, int color){
	int index = ((H / 2 - y - 1) * W + W / 2 + x);
	if (index >= 0 && index < W * H && x > -W / 2 && x < W / 2)
		image_data[index] = color;
}

int		get_texel(int *image_data, float u, float v, int pitch, t_float_array *uzscan, t_float_array *vzscan)
{
	int index = (int)(pitch * v) * pitch + (int)(pitch * u);
	if (index >= 0 && index < pitch * pitch)
		return (image_data[index]);
	return 0;
}