#include "duke.h"

t_anim		load_anim(char *foldername, float speed, int alpha)
{
	t_anim anim;
	char str[128];
	char name[32];
	char *ptr;
	int i;

	i = 0;
	anim.length = 0;
	anim.speed = speed;
	// anim.frames;
	anim.curr_f = 0.0;
	anim.played = 1;
	puts("begin");
	while (i < 64)
	{
		ft_strcpy(str, foldername);
		ptr = ft_itoa(i + 1);
		if (ft_strlen(ptr) == 1)
		{
			ft_strcpy(name, "000");
		}
		else if (ft_strlen(ptr) == 2)
		{
			ft_strcpy(name, "00");
		}
		ft_strcat(name, ptr);
		if (ptr)
			free(ptr);
		ft_strcat(name, ".bmp");
		ft_strcat(str, name);
		anim.frames[i] = create_texture(str, alpha);

		
		if (!anim.frames[i])
			break ;
		anim.frames[i]->flags = alpha;
		i++;
		anim.length++;
	}
	puts("end");
	printf("%d\n\n", anim.length);

	return (anim);
}

SDL_Surface	*create_texture(char *filename, int alpha)
{
	SDL_Surface *texture = SDL_LoadBMP(filename);

	if (!texture)
	{
		return (NULL);
	}



	SDL_Surface *tex;


	SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_BGRA32);
	

	tex = SDL_ConvertSurface(texture, format, 0);

	
	puts("mall");
	SDL_SetColorKey(tex, SDL_TRUE, alpha);
	SDL_FreeFormat(format);
	SDL_FreeSurface(texture);
	tex->flags = alpha;
	return tex;
}