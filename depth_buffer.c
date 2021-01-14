#include "duke.h"

float *clear_z_buffer(float *z_buffer)
{
	ft_bzero((void *)z_buffer, sizeof(float) * HxW);////////////////
	return (z_buffer);
}

float *create_z_buffer()
{
	float *z_buffer;

	z_buffer = malloc(sizeof(float) * HxW);////////////////////////
	clear_z_buffer(z_buffer);
	return (z_buffer);
}

int set_z_buffer(float *z_buffer, int x, int y, float value)
{
	int index;
	
	index = (H_2 - y - 1) * W + W_2 + x;
	if (index < 0 || index >= HxW)
		return (0);
	if (z_buffer[index] < value){
		z_buffer[index] = value;
		return (1);
	}
	return (0);
}

int set_z_buffer_bsp(float *z_buffer, int x, int y, float value)
{
	int index;
	
	index = (H_2 - y - 1) * W + W_2 + x;
	if (index < 0 || index >= HxW)
		return (0);
	if (!z_buffer[index]){
		z_buffer[index] = value;
		return (1);
	}

	return (0);
}