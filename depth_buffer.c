#include "duke.h"

float *clear_z_buffer(float *z_buffer)
{
	bzero((void *)z_buffer, sizeof(float) * H * W);////////////////
	return (z_buffer);
}

float *create_z_buffer()
{
	float *z_buffer;

	z_buffer = malloc(sizeof(float) * H * W);////////////////////////
	clear_z_buffer(z_buffer);
	return (z_buffer);
}

int set_z_buffer(float *z_buffer, int x, int y, float value)
{
	int index;
	
	index = (H / 2 - y - 1) * W + W / 2 + x;
	if (index < 0 || index > W * H)
		return (0);
	if (z_buffer[index] < value){
		z_buffer[index] = value;
		return (1);
	}
		
	return (0);
}