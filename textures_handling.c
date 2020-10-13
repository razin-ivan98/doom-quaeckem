#include "duke.h"

SDL_Surface	*create_texture(char *filename)
{
	SDL_Surface *texture = SDL_LoadBMP(filename);

	SDL_Surface *tex = SDL_CreateRGBSurface(0, texture->w, texture->h, 32, 0, 0, 0, 0);

	char *ptr1 = (char *)tex->pixels;
	char *ptr2 = (char *)texture->pixels;

	for (int i = 0; i < tex->w * tex->h; i++){
		ptr1[i * 4] = ptr2[i * 3];
		ptr1[i * 4 + 1] = ptr2[i * 3 + 1];
		ptr1[i * 4 + 2] = ptr2[i * 3 + 2];
		ptr1[i * 4 + 3] = 255;
	}
	SDL_FreeSurface(texture);
	return tex;
}