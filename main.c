#include "duke.h"
int		find_place_for_bullet(t_doom *doom)
{
	int i;

	i = 0;
	while (i < 40)
	{
		if (!doom->bullets[i].enable)
			return (i);
		i++;
	}
	return (-1);
}

void	fire(t_doom *doom)
{
	int index;

	if (doom->drb_anim.curr != -1 || doom->player_ammo == 0)
		return ;
	index = find_place_for_bullet(doom);
	if (index != -1)
	{
		doom->bullets[index].sprite.instance.position.x =
			doom->scene.camera.position.x - 0.2 * sin(doom->gamma / 180 * 3.1415);
		doom->bullets[index].sprite.instance.position.z =
			doom->scene.camera.position.z + 0.2 * cos(doom->gamma / 180 * 3.1415);
		doom->bullets[index].sprite.instance.position.y = doom->scene.camera.position.y;

		doom->bullets[index].first = doom->bullets[index].sprite.instance.position;
					
		doom->bullets[index].gamma = doom->gamma;
		doom->bullets[index].alpha = doom->alpha;

		doom->bullets[index].enable = 1;

		doom->player_ammo--;
		printf("ammo: %d\n", doom->player_ammo);
	}
	doom->drb_anim.curr = 0;
	doom->drb_anim.curr_f = 0.0;
	

}
void	clean_anim(t_anim *anim)
{
	int i;

	i = 0;

	SDL_Surface *ptr;
	
	while (i < anim->length)
	{
		ptr = anim->frames[i];
		SDL_FreeSurface(ptr);
		i++;
	}
}
void	clean_enemies(t_doom *doom)
{
	int i;
	int j;

	i = 0;
	printf(" count %d\n", doom->enemies_count);
	while (i < doom->enemies_count)
	{
		free(doom->enemies[i].sprite.instance.model.vertexes);
		free(doom->enemies[i].sprite.instance.model.triangles);

		clean_anim(&doom->enemies[i].death_anim);
		clean_anim(&doom->enemies[i].damage_anim);
		clean_anim(&doom->enemies[i].attak_anim);
		j = 0;
		while (j < 8)
		{
			clean_anim(&(doom->enemies[i].walking_anims[j]));
			j++;
		}

		i++;
	}
	doom->enemies_count = 0;
}

void	restart(t_doom *doom)
{
	doom->health = 100;
	doom->player_ammo = 10;
	doom->kills = 0;
	doom->drb_anim.curr_f = 0.0;
	doom->drb_anim.curr = -1;

	doom->scene.camera.position = doom->start_pos;
	
	int i;
	i = 0;
	while (i < 40)
	{
		doom->ammo[i].enable = doom->ammo[i].start_enable;
		doom->bullets[i].enable = 0;
		doom->aid[i].enable = doom->aid[i].start_enable;
		i++;
	}
	i = 0;
	while (i < doom->enemies_count)
	{
		doom->enemies[i].sprite.instance.position = doom->enemies[i].start_pos;
		doom->enemies[i].health = 100;
		i++;
	}
	
	controls_init(doom);
	// clean_enemies(doom);
	// clear_bsp(&doom->scene.level.root);
	// read_bsp(doom, "bsp_test/new_saved_bsp.json");
}

void	event_handle(SDL_Event *event, void *doom_ptr, int *quit)
{
	t_doom *doom;

	doom = (t_doom *)doom_ptr;

	if (doom->menu_opened)
	{
		if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.sym == SDLK_w)
			{
				doom->menu.active--;
				if (doom->menu.active < 0)
					doom->menu.active = 2;
			}
			else if (event->key.keysym.sym == SDLK_s)
			{
				doom->menu.active++;
				if (doom->menu.active > 2)
					doom->menu.active = 0;
			}
			else if (event->key.keysym.sym == SDLK_RETURN)
			{
				if (doom->menu.active == 0)
					doom->menu_opened = 0;
				else if (doom->menu.active == 1)
				{
					doom->difficulty++;
					if (doom->difficulty > 2)
						doom->difficulty = 0;

					if (doom->difficulty == 0)
					{
						doom->enemy_damage = 10;
						doom->enemy_speed = 0.04;
					}
					else if (doom->difficulty == 1)
					{
						doom->enemy_damage = 20;
						doom->enemy_speed = 0.07;
					}
					else
					{
						doom->enemy_damage = 30;
						doom->enemy_speed = 0.1;
					}
					
				}
				else if (doom->menu.active == 2)
				{
					*quit = 1;
					exit(-2);
				}
			}
		}
		return ;
	}

	if (doom->win || doom->lose)
	{
		if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_RETURN)
		{
			doom->lose = 0;
			doom->win = 0;
			doom->menu_opened = 1;
			restart(doom);
		}
		return ;
	}

	if (event->type == SDL_MOUSEBUTTONDOWN)
	{

		fire(doom);
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		doom->mouse_pressed = 0;
		doom->mouse_right_pressed = 0;
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		doom->gamma -= (event->motion.xrel) * 0.5;
		doom->alpha -= (event->motion.yrel) * 0.5;
		doom->prev_x = event->motion.xrel;
		doom->prev_y = event->motion.yrel;
	}
	else if (event->type == SDL_KEYDOWN )
	{
		if (event->key.keysym.sym == SDLK_f)
		{
			if (length(sub(doom->tv.sprite.instance.position, 
				doom->scene.camera.position)) < 2.0)
			{
				doom->tv.enable = doom->tv.enable ? 0 : 1;
			}					
		}
		else if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			doom->menu_opened = 1;
		}
		else if (event->key.keysym.sym == SDLK_SPACE)
		{
			if (doom->on_ground)
				doom->g_speed -= 0.1;
		}
		else if (event->key.keysym.sym == SDLK_LSHIFT)
		{
			t_vertex new_pos = (t_vertex)
				{
					doom->scene.camera.position.x,
					doom->scene.camera.position.z,
					doom->scene.camera.position.y + 0.6,
				};
			if (if_possible_to_move(new_pos, &doom->scene.level.root,
				PHISICS_MODE_PLAYER, 1.7))
			{
				doom->sit = 0;
				doom->height = 1.7;
				doom->run = 1;
				doom->height = 1.7;
			}
			
		}
		else if (event->key.keysym.sym == SDLK_c)
		{
			if (doom->sit == 0)
			{
				doom->run = 0;
				doom->sit = 1;
				doom->height = 1.1;
			}
			else
			{
				t_vertex new_pos = (t_vertex)
					{
						doom->scene.camera.position.x,
						doom->scene.camera.position.z,
						doom->scene.camera.position.y + 0.6,
					};
				if (if_possible_to_move(new_pos, &doom->scene.level.root,
					PHISICS_MODE_PLAYER, 1.7))
				{
					doom->sit = 0;
					doom->height = 1.7;
				}
			}
		}
		else if (event->key.keysym.sym == SDLK_h)
		{
			doom->solid = (doom->solid ? 0 : 1);
		}
		else if (event->key.keysym.sym == SDLK_w)
		{
			doom->w_pressed = 1;
		}
		else if (event->key.keysym.sym == SDLK_s)
		{
			doom->s_pressed = 1;
		}
		else if (event->key.keysym.sym == SDLK_a)
		{
			doom->a_pressed = 1;
		}
		else if (event->key.keysym.sym == SDLK_d)
		{
			doom->d_pressed = 1;
		}
	}
	else if (event->type == SDL_KEYUP)
	{
		if (event->key.keysym.sym == SDLK_w)
		{
			doom->w_pressed = 0;
		}
		else if (event->key.keysym.sym == SDLK_s)
		{
			doom->s_pressed = 0;
		}
		else if (event->key.keysym.sym == SDLK_a)
		{
			doom->a_pressed = 0;
		}
		else if (event->key.keysym.sym == SDLK_d)
		{
			doom->d_pressed = 0;
		}
		else if (event->key.keysym.sym == SDLK_LSHIFT)
		{
			doom->run = 0;
		}
	}
}

void	update_sprites(t_doom *doom, float gamma)
{
	int i;
	int	my;
	int him;
	int d;

	t_sprite sprite;

	i = 0;
		
	doom->scene.sprites_count = 0;

	while (i < doom->enemies_count)
	{
		sprite = doom->enemies[i].sprite;

		if (doom->enemies[i].damaged && doom->enemies[i].health <= 0)
		{
			sprite.instance.model.anim = &doom->enemies[i].death_anim;
			if ((int)sprite.instance.model.anim->curr_f >= sprite.instance.model.anim->length - 1)
			{
				sprite.instance.model.anim->played = 0;
				sprite.instance.model.new_tex[0] = doom->enemies[i].death_anim.frames[5];
			}
		}
		else if (doom->enemies[i].damaged)
		{
			sprite.instance.model.anim = &doom->enemies[i].damage_anim;
			if ((int)sprite.instance.model.anim->curr_f >= sprite.instance.model.anim->length - 1)
			{
				doom->enemies[i].damaged = 0;
			}
		}
		else if (doom->enemies[i].in_attak)
		{
			sprite.instance.model.anim = &doom->enemies[i].attak_anim;
			if ((int)sprite.instance.model.anim->curr_f >= sprite.instance.model.anim->length - 1)
			{
				doom->enemies[i].in_attak = 0;
			}
		}
		else
		{
			my = (int)gamma + 22;
			my %= 360;
			if (my < 0)
				my += 360;
			him = (int)(doom->enemies[i].beta);
			him %= 360;
			if (him < 0)
				him += 360;
			d = (my - him);
			if (d < 0)
				d += 360;
			d /= 45;

			sprite.instance.model.anim = &doom->enemies[i].walking_anims[d];
		}
		
		
		
		sprite.instance.orientation = doom->for_sprites;

		doom->scene.sprites[i] = sprite;
		doom->scene.sprites_count++;
		i++;
	}
	i = 0;
	while (i < 40)
	{
		if (doom->bullets[i].enable)
		{

			doom->scene.sprites[doom->scene.sprites_count] = doom->bullets[i].sprite;
			doom->scene.sprites[doom->scene.sprites_count].instance.orientation =
					doom->for_sprites;
			doom->scene.sprites_count++;
		}
		
		i++;
	}
	i = 0;
	while (i < 40)
	{
		if (doom->ammo[i].enable)
		{
			doom->scene.sprites[doom->scene.sprites_count] = doom->ammo[i].sprite;
			doom->scene.sprites[doom->scene.sprites_count].instance.orientation =
					doom->for_sprites;
			doom->scene.sprites_count++;
		}
		
		i++;
	}
	i = 0;
	while (i < 40)
	{
		if (doom->aid[i].enable)
		{
			doom->scene.sprites[doom->scene.sprites_count] = doom->aid[i].sprite;
			doom->scene.sprites[doom->scene.sprites_count].instance.orientation =
					doom->for_sprites;
			doom->scene.sprites_count++;
		}
		
		i++;
	}
	i = 0;
	while (i < doom->objects_count)
	{
		doom->scene.sprites[doom->scene.sprites_count] = doom->objects[i].sprite;
		doom->scene.sprites[doom->scene.sprites_count].instance.orientation =
					doom->for_sprites;
		doom->scene.sprites_count++;
		i++;
	}
	doom->scene.sprites[doom->scene.sprites_count] = doom->tv.sprite;
	if (doom->tv.enable)
		doom->scene.sprites[doom->scene.sprites_count].instance.model.anim = &doom->tv.anim;
	else
	{
		doom->scene.sprites[doom->scene.sprites_count].instance.model.anim = NULL;
		doom->scene.sprites[doom->scene.sprites_count].instance.model.new_tex[0] =
				doom->tv.anim.frames[(int)doom->tv.anim.curr_f % doom->tv.anim.length];
	}
	
	doom->scene.sprites_count++;
}



void	update_scene(t_doom *doom, float gamma)
{
	update_sprites(doom, gamma);
}

void	animation_update(t_scene *scene, float curr_time)
{
	int i;
	int frame;

	i = 0;
	while (i < scene->sprites_count)
	{
		if (!scene->sprites[i].instance.model.anim || !scene->sprites[i].instance.model.anim->played)
		{
			i++;
			continue ;
		}
		
		scene->sprites[i].instance.model.anim->curr_f += scene->sprites[i].instance.model.anim->speed * curr_time;


		frame = (int)(scene->sprites[i].instance.model.anim->curr_f)
				% scene->sprites[i].instance.model.anim->length;

		// printf("а здесь %d\n\n", scene->sprites[i].instance.model.anim->length);
		if (frame < 0)
			frame = 0;
		scene->sprites[i].instance.model.new_tex[0] =
				scene->sprites[i].instance.model.anim->frames[frame];
		i++;
	}
	// i = 0;
	// while (i < scene->objects_count)
	// {
	// 	if (!scene->objects[i].instance.model.anim || !scene->objects[i].instance.model.anim)
	// 	{
	// 		i++;
	// 		continue ;
	// 	}
		
	// 	scene->objects[i].instance.model.anim->curr_f += scene->objects[i].instance.model.anim->speed * curr_time;


	// 	frame = (int)(scene->objects[i].instance.model.anim->curr_f)
	// 			% scene->objects[i].instance.model.anim->length;
	// 	// if (frame < 0)
	// 	// 	frame = 0;
	// 	scene->objects[i].instance.model.new_tex[0] =
	// 			scene->objects[i].instance.model.anim->frames[frame];
	// 	i++;
	// }
}

int		classify_point_s(t_vertex cam, t_vertex line, t_vertex normal)
{
	t_vertex new;
	t_vertex ort;

	cam.z = 0.0;
	normal.z = 0.0;

	ort = get_ort_line_by_point(line, cam);
	new = lines_intersect(line, ort);

	// printf("line %f\t%f\t%f\n", line.x, line.y, line.z);
	// printf("ort %f\t%f\t%f\n", ort.x, ort.y, ort.z);

	new.z = 0.0;

	new = sub(cam, new);
	
	// printf("%f\t%f\t%f\n", new.x, new.y, new.z);
	// printf("%f\t%f\t%f\n", normal.x, normal.y, normal.z);
	// printf("%f\n\n", dot(new, normal));

	if (dot(new, normal) < 0.0)
		return (BACK);
	return (FRONT);
}

int		check_leaf(t_bsp *node, t_vertex pos)
{
	int			i;
	t_vertex	line;

	i = 0;
	while (i < node->walls_count)
	{
		node->walls[i].failed = 0;
		if (node->walls[i].type == WALL_TYPE_WALL)
		{
			node->walls[i].failed = 1;
			line = get_line_by_points(node->walls[i].points[0],
							node->walls[i].points[1]);
			if (classify_point_s(pos, line, node->walls[i].normal) == BACK)
			{
				node->walls[i].failed = 2;
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int		bsp_solid_traversal(t_bsp *node, t_vertex pos, int mode, float height)
{
	if (node->is_leaf)
	{
		if (mode == PHISICS_MODE_PLAYER)
		{
			if (pos.z - (height - 0.2 - 0.4) < node->floor ||
				pos.z + 0.2 > node->ceil)
				return (0);
		}
		else
		{
			if (pos.z < node->floor || pos.z > node->ceil)
				return (0);
		}
		
		

		return (check_leaf(node, pos));
	}
	else
	{
		if (classify_point(pos, node->line, node->normal) == BACK)
		{
			return (bsp_solid_traversal(node->front, pos, mode, height));
		}
		else
		{
			return (bsp_solid_traversal(node->back, pos, mode, height));
		}
	}
}


int		if_possible_to_move(t_vertex pos, t_bsp *node, int mode, float height)
{
	return (bsp_solid_traversal(node, pos, mode, height));
}

void		get_floor_seil_traversal(t_bsp *node, t_vertex pos, float *floor, float *ceil)
{
	if (node->is_leaf)
	{
		*floor = node->floor;
		*ceil = node->ceil;
	}
	else
	{
		if (classify_point(pos, node->line, node->normal) == BACK)
		{
			get_floor_seil_traversal(node->front, pos, floor, ceil);
		}
		else
		{
			get_floor_seil_traversal(node->back, pos, floor, ceil);
		}
	}
}

void	update_enemies(t_doom *doom)
{
	char			str[160];
	int				i;
	unsigned int	*intrand;
	t_vertex		new_pos;
	int				fd;

	fd = open("/dev/urandom", O_RDONLY);
	read(fd, str, 160);
	close(fd);

	intrand = (unsigned int *)str;

	float speed = doom->enemy_speed;

	float floor;
	float ceil;

	i = 0;
	while (i < doom->enemies_count)
	{
		//printf("rand: %u\n", intrand[i]);
		if (doom->enemies[i].health <= 0  || doom->enemies[i].damaged || doom->enemies[i].in_attak)
		{
			i++;
			continue ;
		}
		if (length(sub(doom->scene.camera.position,
			doom->enemies[i].sprite.instance.position)) < 2.0)
		{
			doom->enemies[i].in_attak = 1;
			doom->enemies[i].attak_anim.curr_f = 0.0;
			doom->health -= doom->enemy_damage;
			if (doom->health <= 0)
			{
				doom->lose = 1;
			}
		}
		else if (length(sub(doom->scene.camera.position,
			doom->enemies[i].sprite.instance.position)) < 15.0)
		{
			new_pos = sub(doom->scene.camera.position,
					doom->enemies[i].sprite.instance.position);
			doom->enemies[i].beta = atan2(new_pos.x, -new_pos.z) / M_PI * 180;
		}
		else if (intrand[i] < 96256808)
		{
			doom->enemies[i].beta += intrand[i] % 180;
		}
		

		get_floor_seil_traversal(&doom->scene.level.root,
			(t_vertex){doom->enemies[i].sprite.instance.position.x,
			doom->enemies[i].sprite.instance.position.z,
			0.0}, &floor, &ceil);

		doom->enemies[i].on_ground = 0;
		doom->enemies[i].g_speed += 0.4 * (doom->mgl->curr_time - doom->mgl->lst_time);
		doom->enemies[i].sprite.instance.position.y -= doom->enemies[i].g_speed;

		if (doom->enemies[i].sprite.instance.position.y - 1.0 < floor)
		{
			doom->enemies[i].on_ground = 1;
			doom->enemies[i].sprite.instance.position.y = 1.0 + floor;
			doom->enemies[i].g_speed = 0;
		}

		new_pos.x = doom->enemies[i].sprite.instance.position.x +
				speed * sin(doom->enemies[i].beta / 180 * M_PI);
		new_pos.y = doom->enemies[i].sprite.instance.position.z -
				speed * cos(doom->enemies[i].beta / 180 * M_PI);
		new_pos.z = doom->enemies[i].sprite.instance.position.y;

		if (if_possible_to_move(new_pos, &doom->scene.level.root,
				PHISICS_MODE_PLAYER, 1))
		{
			doom->enemies[i].sprite.instance.position.x = new_pos.x;
			doom->enemies[i].sprite.instance.position.z = new_pos.y;
		}

		i++;
	}
}

int		check_enemies(t_doom *doom, t_vertex pos)
{
	int i;
	int damage;

	i = 0;
	while (i < doom->enemies_count)
	{
		if (doom->enemies[i].health > 0)
		{
			if (length(sub(doom->enemies[i].sprite.instance.position,
				pos)) < 1.0)
			{
				damage = 100 / length(sub(doom->scene.camera.position,
				pos));
				printf("damage: %d\n", damage);
				doom->enemies[i].health -= damage;
				if (doom->enemies[i].health <= 0)
					doom->kills++;

				doom->enemies[i].damage_anim.curr_f = 0.0;
				doom->enemies[i].damaged = 1;
				

				return (1);
			}
		}
		i++;
	}
	return (0);
}

void	update_bullets(t_doom *doom)
{
	int i;
	t_vertex new_pos;
	float path;

	float speed = 10;

	path = (doom->mgl->curr_time - doom->mgl->lst_time) * speed;
	i = 0;
	while (i < 40)
	{
		if (doom->bullets[i].enable)
		{
			
			new_pos = (t_vertex)
			{
				doom->bullets[i].sprite.instance.position.x - path * sin(doom->bullets[i].gamma / 180 * 3.1415),
				doom->bullets[i].sprite.instance.position.z + path * cos(doom->bullets[i].gamma / 180 * 3.1415),
				doom->bullets[i].sprite.instance.position.y + path * sin(doom->bullets[i].alpha  / 180 * 3.1415)
 			};
			if (if_possible_to_move(new_pos, &doom->scene.level.root,
					PHISICS_MODE_BULLET, 1) || doom->solid)
			{
				doom->bullets[i].sprite.instance.position.z = new_pos.y;
				doom->bullets[i].sprite.instance.position.x = new_pos.x;
				doom->bullets[i].sprite.instance.position.y = new_pos.z;
			}
			else
			{
				doom->bullets[i].enable = 0;
			}
			if (check_enemies(doom, doom->bullets[i].sprite.instance.position))
				doom->bullets[i].enable = 0;
			
		}
		i++;
	}
}

void	drb_animation_update(t_doom *doom)
{
	int frame;

	if (doom->drb_anim.curr == -1)
		frame = 0;
	else
	{
		doom->drb_anim.curr_f += doom->drb_anim.speed *
				(doom->mgl->curr_time - doom->mgl->lst_time);

		frame = (int)(doom->drb_anim.curr_f);
		// printf("compframe %d\n", frame);
		
		if (frame >= doom->drb_anim.length)
		{
			doom->drb_anim.curr = -1;

			frame = 0;
		}
		else
		{
			doom->drb_anim.curr = frame;
		}
	}
	SDL_Rect rect;

	rect.x = -64;
	rect.y = -64;
	rect.h = 192;
	rect.w = 192;
	
	SDL_BlitScaled(doom->drb_anim.frames[frame], &rect, doom->mgl->screen_surface, NULL);
	

		
}

void	update_ammo(t_doom *doom)
{
	int i;

	i = 0;
	while (i < 40)
	{
		if (doom->ammo[i].enable)
		{
			if (length(sub(doom->ammo[i].sprite.instance.position,
				doom->scene.camera.position)) < 1.5)
			{
				doom->player_ammo += 10;
				printf("ammo: %d\n", doom->player_ammo);
				doom->ammo[i].enable = 0;
			}
		}
		i++;
	}
}
void	update_aid(t_doom *doom)
{
	int i;

	i = 0;
	while (i < 40)
	{
		if (doom->aid[i].enable)
		{
			if (length(sub(doom->aid[i].sprite.instance.position,
				doom->scene.camera.position)) < 1.5)
			{
				doom->health += 30;
				printf("health: %d\n", doom->health);
				doom->aid[i].enable = 0;
			}
		}
		i++;
	}
}

SDL_Surface* renderText(char *message, char *font_file,
        SDL_Color color, int fontSize)
{
        //Открываем шрифт
        TTF_Font *font = TTF_OpenFont(font_file, fontSize);
        if (font == NULL){
                puts("TTF_OpenFontError");
                return NULL;
        }       

        SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);

        TTF_CloseFont(font);
        return surf;
}

void	draw_menu(t_doom *doom)
{
	SDL_Rect rect;
	int step;
	int i;
	int j;

	step = (H) / 5;

	rect.x = -100;
	rect.y = 0;
	rect.w = W + 200;
	rect.h = H;

	SDL_BlitScaled(doom->menu_back, NULL, doom->mgl->screen_surface, NULL);
	rect.y -= step;
	SDL_BlitScaled(doom->menu.play, &rect, doom->mgl->screen_surface, NULL);
	rect.y -= step;
	if (doom->difficulty == 0)
		SDL_BlitScaled(doom->menu.difficulty_1, &rect, doom->mgl->screen_surface, NULL);
	else if (doom->difficulty == 1)
		SDL_BlitScaled(doom->menu.difficulty_2, &rect, doom->mgl->screen_surface, NULL);
	else
		SDL_BlitScaled(doom->menu.difficulty_3, &rect, doom->mgl->screen_surface, NULL);
	rect.y -= step;
	SDL_BlitScaled(doom->menu.exit_b, &rect, doom->mgl->screen_surface, NULL);

	i = -25;
	while (i < 24)
	{
		j = -25;
		while (j < 24)
		{
			put_pixel(doom->mgl->screen_surface->pixels, i - W_2 + 50, -j - (doom->menu.active + 1.5) * step + H_2, 0xcea322);
			j++;
		}
		i++;
	}
}

void	draw_hud(t_doom *doom)
{
	char str[62];
	char str2[62];
	SDL_Color color;

	color.r = 35;
	color.g = 151;
	color.b = 35;

	ft_strcpy(str, "+ ");
	itoa(doom->health, str2);
	ft_strcat(str, str2);
	
	doom->health_bar = renderText(str, "fonts/DoomsDay.ttf", color, 50);
	ft_strcpy(str, "||| ");
	itoa(doom->player_ammo, str2);
	ft_strcat(str, str2);
	color.r = 188;
	color.g = 143;
	color.b = 43;
	doom->ammo_bar = renderText(str, "fonts/DoomsDay.ttf", color, 50);
	ft_strcpy(str, "KILLS ");
	itoa(doom->kills, str2);
	ft_strcat(str, str2);
	
	color.r = 94;
	color.g = 6;
	color.b = 6;
	doom->kills_bar = renderText(str, "fonts/DoomsDay.ttf", color, 50);

	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = W;
	rect.h = H;

	SDL_BlitScaled(doom->health_bar, &rect, doom->mgl->screen_surface, NULL);
	rect.y -= 50;
	SDL_BlitScaled(doom->ammo_bar, &rect, doom->mgl->screen_surface, NULL);
	rect.y = 0;
	rect.x = -W + 200;
	SDL_BlitScaled(doom->kills_bar, &rect, doom->mgl->screen_surface, NULL);
	SDL_FreeSurface(doom->health_bar);
	SDL_FreeSurface(doom->ammo_bar);
	SDL_FreeSurface(doom->kills_bar);


}

void	draw_lose(t_doom *doom)
{
	SDL_BlitScaled(doom->lose_surface, NULL,
			doom->mgl->screen_surface, NULL);
}
void	draw_win(t_doom *doom)
{
	SDL_BlitScaled(doom->win_surface, NULL,
			doom->mgl->screen_surface, NULL);
}
void	draw_press_f(t_doom *doom)
{
	SDL_Rect rect;

	rect.x = -400;
	rect.w = 1000;
	rect.y = -500;
	rect.h = 720;
	if (length(sub(doom->tv.sprite.instance.position, 
				doom->scene.camera.position)) < 2.0)
		SDL_BlitScaled(doom->press_f, &rect,
			doom->mgl->screen_surface, NULL);
}

void	update(void *doom_ptr, int *pixels)
{
	t_doom *doom;

	

	doom = (t_doom *)doom_ptr;

	if (doom->menu_opened)
	{
		draw_menu(doom);
		return;
	}
	if (doom->lose)
	{
		draw_lose(doom);
		return ;
	}
	if (doom->win)
	{
		draw_win(doom);
		return ;
	}



	doom->for_sprites = make_oy_rot_matrix(doom->gamma);
	doom->scene.camera.orientation = multiply_m_m(doom->for_sprites,
												make_ox_rot_matrix(doom->alpha));

	ft_bzero(pixels, sizeof(int) * HxW);

	

	clear_z_buffer(doom->scene.z_buffer);



	update_bullets(doom);
	update_ammo(doom);
	update_aid(doom);


	update_enemies(doom);

	

	update_scene(doom, doom->gamma);

	animation_update(&doom->scene, doom->mgl->curr_time - doom->mgl->lst_time);

	// SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_BGRA32);
	// SDL_Surface	*tex = SDL_ConvertSurface(doom->menu_back, format, 0);

	// SDL_FreeSurface(tex);

	// tex = SDL_ConvertSurface(doom->menu_back, format, 0);

	// SDL_FreeSurface(tex);

	// tex = SDL_ConvertSurface(doom->menu_back, format, 0);
	// SDL_FreeSurface(tex);

	// SDL_FreeFormat(format);
	



	pthread_render(pixels, doom);

	//  render_scene(pixels, &doom->scene);

	drb_animation_update(doom);

	draw_hud(doom);

	draw_press_f(doom);

	float speed = 6.0;
	if (doom->sit)
		speed -= 2.0;
	else if (doom->run)
		speed += 4.0;

	float path = (doom->mgl->curr_time - doom->mgl->lst_time) * speed;

	t_vertex new_pos;


	float floor;
	float ceil;
	get_floor_seil_traversal(&doom->scene.level.root,
			(t_vertex){doom->scene.camera.position.x,
			doom->scene.camera.position.z, 0.0}, &floor, &ceil);
	//printf("on_ground %d\n", doom->on_ground);

	doom->on_ground = 0;

	if (!doom->on_ground)
	{
		doom->g_speed += 0.4 * (doom->mgl->curr_time - doom->mgl->lst_time);
	}
	new_pos = (t_vertex)
		{
			doom->scene.camera.position.x,
			doom->scene.camera.position.z,
			doom->scene.camera.position.y - doom->g_speed
		};
	if (if_possible_to_move(new_pos, &doom->scene.level.root,
		PHISICS_MODE_PLAYER, doom->height) || doom->solid)
	{
		doom->scene.camera.position.y -= doom->g_speed;
	}
	else
	{
		doom->g_speed = 0;
	}
	
	

	if (doom->scene.camera.position.y - (doom->height - 0.2) < floor)
	{
		doom->on_ground = 1;
		doom->scene.camera.position.y = doom->height - 0.2 + floor;
		doom->g_speed = 0;
	}


	///doom->scene.camera.position.y = floor + 1.0;

	//printf ("player: %f %f\n", doom->scene.camera.position.x, doom->scene.camera.position.z);

	if (doom->w_pressed)
	{
		new_pos = (t_vertex)
		{
			doom->scene.camera.position.x - path * sin(doom->gamma / 180 * 3.1415),
			doom->scene.camera.position.z,
			doom->scene.camera.position.y
		};
		if (if_possible_to_move(new_pos, &doom->scene.level.root,
			PHISICS_MODE_PLAYER, doom->height) || doom->solid)
		{
			
			doom->scene.camera.position.x = new_pos.x;
		}
		new_pos = (t_vertex)
		{
			doom->scene.camera.position.x,
			doom->scene.camera.position.z + path * cos(doom->gamma / 180 * 3.1415),
			doom->scene.camera.position.y
		};
		if (if_possible_to_move(new_pos, &doom->scene.level.root,
			PHISICS_MODE_PLAYER, doom->height) || doom->solid)
		{
			doom->scene.camera.position.z = new_pos.y;
		}
	}
	if (doom->s_pressed)
	{
		new_pos = (t_vertex)
		{
			doom->scene.camera.position.x + path * sin(doom->gamma / 180 * 3.1415),
			doom->scene.camera.position.z,
			doom->scene.camera.position.y
		};
		if (if_possible_to_move(new_pos, &doom->scene.level.root,
			PHISICS_MODE_PLAYER, doom->height) || doom->solid)
		{
			doom->scene.camera.position.x = new_pos.x;
		}
		new_pos = (t_vertex)
		{
			doom->scene.camera.position.x,
			doom->scene.camera.position.z - path * cos(doom->gamma / 180 * 3.1415),
			doom->scene.camera.position.y
		};
		if (if_possible_to_move(new_pos, &doom->scene.level.root,
			PHISICS_MODE_PLAYER, doom->height) || doom->solid)
		{
			doom->scene.camera.position.z = new_pos.y;
		}
	}
	if (doom->a_pressed)
	{
		new_pos = (t_vertex)
		{
			doom->scene.camera.position.x - path * cos(doom->gamma / 180 * 3.1415),
			doom->scene.camera.position.z,
			doom->scene.camera.position.y
		};
		if (if_possible_to_move(new_pos, &doom->scene.level.root,
			PHISICS_MODE_PLAYER, doom->height) || doom->solid)
		{
			doom->scene.camera.position.x = new_pos.x;
		}
		new_pos = (t_vertex)
		{
			doom->scene.camera.position.x,
			doom->scene.camera.position.z - path * sin(doom->gamma / 180 * 3.1415),
			doom->scene.camera.position.y
		};
		if (if_possible_to_move(new_pos, &doom->scene.level.root,
			PHISICS_MODE_PLAYER, doom->height) || doom->solid)
		{
			doom->scene.camera.position.z = new_pos.y;
		}
	}
	if (doom->d_pressed)
	{
		new_pos = (t_vertex)
		{
			doom->scene.camera.position.x + path * cos(doom->gamma / 180 * 3.1415),
			doom->scene.camera.position.z,
			doom->scene.camera.position.y
		};
		if (if_possible_to_move(new_pos, &doom->scene.level.root,
			PHISICS_MODE_PLAYER, doom->height) || doom->solid)
		{
			doom->scene.camera.position.x = new_pos.x;
		}
		new_pos = (t_vertex)
		{
			doom->scene.camera.position.x,
			doom->scene.camera.position.z + path * sin(doom->gamma / 180 * 3.1415),
			doom->scene.camera.position.y
		};
		if (if_possible_to_move(new_pos, &doom->scene.level.root,
			PHISICS_MODE_PLAYER, doom->height) || doom->solid)
		{
			doom->scene.camera.position.z = new_pos.y;
		}
	}
	if (length(sub((t_vertex){doom->scene.camera.position.x, 0.0,
			doom->scene.camera.position.z}, doom->aim)) < 3.0)
	{
		doom->win = 1;
	}
}

t_enemy	create_enemy(t_vertex pos, float beta)
{
	t_enemy enemy;
	t_model enemy_model;

	enemy_model.bounds_center = (t_vertex) {0.0,0.0,0.0};;
	enemy_model.bounds_radius = 2.0;
	enemy_model.triangles_count = 2;
	enemy_model.vertexes_count = 4;

	enemy_model.vertexes = malloc(sizeof(t_vertex) * 4);//////

	enemy_model.vertexes[0] = (t_vertex) {-1.0,-1.0,0.0};
	enemy_model.vertexes[1] = (t_vertex) {-1.0,1.0,0.0};
	enemy_model.vertexes[2] = (t_vertex) {1.0,1.0,0.0};
	enemy_model.vertexes[3] = (t_vertex) {1.0,-1.0,0.0};

	enemy_model.triangles = malloc(sizeof(t_triangle) * 2);
	enemy_model.triangles[0].indexes[0] = 0;
	enemy_model.triangles[0].indexes[1] = 1;
	enemy_model.triangles[0].indexes[2] = 2;

	enemy_model.triangles[1].indexes[0] = 0;
	enemy_model.triangles[1].indexes[1] = 2;
	enemy_model.triangles[1].indexes[2] = 3;

	enemy_model.triangles[0].normal = (t_vertex) {0.0,0.0,1.0};
	enemy_model.triangles[1].normal = (t_vertex) {0.0,0.0,1.0};

	enemy_model.triangles[0].uvs[0] = (t_point) {0.0, 1.0,0.0};
	enemy_model.triangles[0].uvs[1] = (t_point) {0.0, 0.0,0.0};
	enemy_model.triangles[0].uvs[2] = (t_point) {1.0, 0.0,0.0};

	enemy_model.triangles[1].uvs[0] = (t_point) {0.0, 1.0,0.0};
	enemy_model.triangles[1].uvs[1] = (t_point) {1.0, 0.0,0.0};
	enemy_model.triangles[1].uvs[2] = (t_point) {1.0, 1.0,0.0};


	enemy.sprite.instance.model = enemy_model;
	enemy.sprite.instance.scale = 1.0;
	enemy.sprite.instance.position = pos;
	enemy.sprite.instance.orientation = make_oy_rot_matrix(0.0);

	enemy.beta = beta;

	enemy.walking_anims[0] = load_anim("textures/animm/front/", 4.0, 0xff00ffff);
	enemy.walking_anims[1] = load_anim("textures/animm/front-left/", 4.0, 0xff00ffff);
	enemy.walking_anims[2] = load_anim("textures/animm/left/", 4.0, 0xff00ffff);
	enemy.walking_anims[3] = load_anim("textures/animm/back-left/", 4.0, 0xff00ffff);
	enemy.walking_anims[4] = load_anim("textures/animm/back/", 4.0, 0xff00ffff);
	enemy.walking_anims[5] = load_anim("textures/animm/back-right/", 4.0, 0xff00ffff);
	enemy.walking_anims[6] = load_anim("textures/animm/right/", 4.0, 0xff00ffff);
	enemy.walking_anims[7] = load_anim("textures/animm/front-right/", 4.0, 0xff00ffff);

	enemy.attak_anim = load_anim("textures/attak/", 4.0, 0xff00ffff);
	enemy.damage_anim = load_anim("textures/damage/", 4.0, 0xff00ffff);
	enemy.death_anim = load_anim("textures/death/", 4.0, 0xff00ffff);

	enemy.sprite.instance.model.anim = &enemy.walking_anims[0];
	enemy.sprite.instance.model.new_tex[0] = enemy.sprite.instance.model.anim->frames[0];

	enemy.damaged = 0;
	enemy.in_attak = 0;

	enemy.start_pos = pos;

	enemy.health = 100;

	return (enemy);
}

t_object	create_object(t_vertex pos, int index)
{
	t_object object;
	t_model object_model;

	object_model.bounds_center = (t_vertex) {0.0,0.0,0.0};;
	object_model.bounds_radius = 3.0;
	object_model.triangles_count = 2;
	object_model.vertexes_count = 4;

	object_model.vertexes = malloc(sizeof(t_vertex) * 4);

	object_model.vertexes[0] = (t_vertex) {-1.1,-0.8,0.0};
	object_model.vertexes[1] = (t_vertex) {-1.1,0.8,0.0};
	object_model.vertexes[2] = (t_vertex) {1.1,0.8,0.0};
	object_model.vertexes[3] = (t_vertex) {1.1,-0.8,0.0};

	object_model.triangles = malloc(sizeof(t_triangle) * 2);
	object_model.triangles[0].indexes[0] = 0;
	object_model.triangles[0].indexes[1] = 1;
	object_model.triangles[0].indexes[2] = 2;

	object_model.triangles[1].indexes[0] = 0;
	object_model.triangles[1].indexes[1] = 2;
	object_model.triangles[1].indexes[2] = 3;

	object_model.triangles[0].normal = (t_vertex) {0.0,0.0,1.0};
	object_model.triangles[1].normal = (t_vertex) {0.0,0.0,1.0};

	object_model.triangles[0].uvs[0] = (t_point) {0.0, 1.0,0.0};
	object_model.triangles[0].uvs[1] = (t_point) {0.0, 0.0,0.0};
	object_model.triangles[0].uvs[2] = (t_point) {1.0, 0.0,0.0};

	object_model.triangles[1].uvs[0] = (t_point) {0.0, 1.0,0.0};
	object_model.triangles[1].uvs[1] = (t_point) {1.0, 0.0,0.0};
	object_model.triangles[1].uvs[2] = (t_point) {1.0, 1.0,0.0};

	char str[64];
	char str2[64];

	ft_strcpy(str, "textures/sprites/");
	itoa(index, str2);
	ft_strcat(str, str2);
	ft_strcat(str, ".bmp");

	object_model.new_tex[0] = create_texture(str, 0xffff00ff);///////////////

	object.sprite.instance.model = object_model;
	object.sprite.instance.scale = 1.0;
	object.sprite.instance.position = pos;
	object.sprite.instance.orientation = make_oy_rot_matrix(0.0);
	object.sprite.instance.model.anim = NULL;

	return (object);
}

void	create_tv(t_doom *doom)
{
	t_model tv_model;

	tv_model.bounds_center = (t_vertex) {0.0,0.0,0.0};;
	tv_model.bounds_radius = 3.0;
	tv_model.triangles_count = 2;
	tv_model.vertexes_count = 4;
	tv_model.vertexes = malloc(sizeof(t_vertex) * 4);
	tv_model.vertexes[0] = (t_vertex) {-1.2,-0.8,0.0};
	tv_model.vertexes[1] = (t_vertex) {-1.2,0.8,0.0};
	tv_model.vertexes[2] = (t_vertex) {1.2,0.8,0.0};
	tv_model.vertexes[3] = (t_vertex) {1.2,-0.8,0.0};
	tv_model.triangles = malloc(sizeof(t_triangle) * 2);
	tv_model.triangles[0].indexes[0] = 0;
	tv_model.triangles[0].indexes[1] = 1;
	tv_model.triangles[0].indexes[2] = 2;
	tv_model.triangles[1].indexes[0] = 0;
	tv_model.triangles[1].indexes[1] = 2;
	tv_model.triangles[1].indexes[2] = 3;
	tv_model.triangles[0].normal = (t_vertex) {0.0,0.0,1.0};
	tv_model.triangles[1].normal = (t_vertex) {0.0,0.0,1.0};
	tv_model.triangles[0].uvs[0] = (t_point) {0.0, 1.0,0.0};
	tv_model.triangles[0].uvs[1] = (t_point) {0.0, 0.0,0.0};
	tv_model.triangles[0].uvs[2] = (t_point) {1.0, 0.0,0.0};
	tv_model.triangles[1].uvs[0] = (t_point) {0.0, 1.0,0.0};
	tv_model.triangles[1].uvs[1] = (t_point) {1.0, 0.0,0.0};
	tv_model.triangles[1].uvs[2] = (t_point) {1.0, 1.0,0.0};

	
	doom->tv.anim = load_anim("textures/anim1/", 15.0, 0);
	doom->tv.anim.played = 1;
	doom->tv.sprite.instance.model.anim = &doom->tv.anim;
	doom->tv.anim.curr_f = 0.0;
	

	doom->tv.sprite.instance.model = tv_model;
	doom->tv.sprite.instance.model.new_tex[0] = doom->tv.anim.frames[0];
	doom->tv.sprite.instance.scale = 1.0;
	doom->tv.sprite.instance.orientation = make_oy_rot_matrix(doom->tv.beta);
	doom->tv.sprite.instance.model.anim = NULL;

}
void	bullets_init(t_doom *doom)
{
	t_model bullet_model;
	t_bullet bullet;
	int i;
	
	bullet_model.bounds_center = (t_vertex) {0.0,0.0,0.0};
	bullet_model.bounds_radius = 2.0;
	bullet_model.triangles_count = 2;
	bullet_model.vertexes_count = 4;

	bullet_model.vertexes = malloc(sizeof(t_vertex) * 4);

	bullet_model.vertexes[0] = (t_vertex) {-0.1,-0.1,0.0};
	bullet_model.vertexes[1] = (t_vertex) {-0.1,0.1,0.0};
	bullet_model.vertexes[2] = (t_vertex) {0.1,0.1,0.0};
	bullet_model.vertexes[3] = (t_vertex) {0.1,-0.1,0.0};

	bullet_model.triangles = malloc(sizeof(t_triangle) * 2);
	bullet_model.triangles[0].indexes[0] = 0;
	bullet_model.triangles[0].indexes[1] = 1;
	bullet_model.triangles[0].indexes[2] = 2;

	bullet_model.triangles[1].indexes[0] = 0;
	bullet_model.triangles[1].indexes[1] = 2;
	bullet_model.triangles[1].indexes[2] = 3;

	bullet_model.triangles[0].normal = (t_vertex) {0.0,0.0,1.0};
	bullet_model.triangles[1].normal = (t_vertex) {0.0,0.0,1.0};

	bullet_model.triangles[0].uvs[0] = (t_point) {0.0, 1.0,0.0};
	bullet_model.triangles[0].uvs[1] = (t_point) {0.0, 0.0,0.0};
	bullet_model.triangles[0].uvs[2] = (t_point) {1.0, 0.0,0.0};

	bullet_model.triangles[1].uvs[0] = (t_point) {0.0, 1.0,0.0};
	bullet_model.triangles[1].uvs[1] = (t_point) {1.0, 0.0,0.0};
	bullet_model.triangles[1].uvs[2] = (t_point) {1.0, 1.0,0.0};

	bullet_model.new_tex[0] = create_texture("textures/bullet.bmp", 0xfffaff08);
	bullet_model.new_tex[0]->flags = 0xfffaff08;

	bullet_model.anim = NULL;
	
	bullet.sprite.instance.model = bullet_model;
	bullet.sprite.instance.scale = 1.0;
	bullet.enable = 0;

	bullet.sprite.instance.orientation = make_oy_rot_matrix(0.0);

	i = 0;
	while (i < 40)
	{
		doom->bullets[i] = bullet;
		i++;
	}
}

void	ammo_init(t_doom *doom)
{
	t_model ammo_model;
	t_ammo ammo;
	int i;
	
	ammo_model.bounds_center = (t_vertex) {0.0,0.0,0.0};
	ammo_model.bounds_radius = 2.0;
	ammo_model.triangles_count = 2;
	ammo_model.vertexes_count = 4;

	ammo_model.vertexes = malloc(sizeof(t_vertex) * 4);

	ammo_model.vertexes[0] = (t_vertex) {-0.2,-0.2,0.0};
	ammo_model.vertexes[1] = (t_vertex) {-0.2,0.2,0.0};
	ammo_model.vertexes[2] = (t_vertex) {0.2,0.2,0.0};
	ammo_model.vertexes[3] = (t_vertex) {0.2,-0.2,0.0};

	ammo_model.triangles = malloc(sizeof(t_triangle) * 2);
	ammo_model.triangles[0].indexes[0] = 0;
	ammo_model.triangles[0].indexes[1] = 1;
	ammo_model.triangles[0].indexes[2] = 2;

	ammo_model.triangles[1].indexes[0] = 0;
	ammo_model.triangles[1].indexes[1] = 2;
	ammo_model.triangles[1].indexes[2] = 3;

	ammo_model.triangles[0].normal = (t_vertex) {0.0,0.0,1.0};
	ammo_model.triangles[1].normal = (t_vertex) {0.0,0.0,1.0};

	ammo_model.triangles[0].uvs[0] = (t_point) {0.0, 1.0,0.0};
	ammo_model.triangles[0].uvs[1] = (t_point) {0.0, 0.0,0.0};
	ammo_model.triangles[0].uvs[2] = (t_point) {1.0, 0.0,0.0};

	ammo_model.triangles[1].uvs[0] = (t_point) {0.0, 1.0,0.0};
	ammo_model.triangles[1].uvs[1] = (t_point) {1.0, 0.0,0.0};
	ammo_model.triangles[1].uvs[2] = (t_point) {1.0, 1.0,0.0};

	ammo_model.new_tex[0] = create_texture("textures/ammo.bmp", 0xff00feff);/////
	ammo_model.new_tex[0]->flags = 0xff00feff;

	ammo_model.anim = NULL;
	
	ammo.sprite.instance.model = ammo_model;
	ammo.sprite.instance.scale = 2.0;
	ammo.enable = 0;

	ammo.sprite.instance.orientation = make_oy_rot_matrix(0.0);

	ammo.start_enable = 0;
	i = 0;
	while (i < 40)
	{
		doom->ammo[i] = ammo;
		i++;
	}
}
void	aid_init(t_doom *doom)
{
	t_model aid_model;
	t_aid aid;
	int i;
	
	aid_model.bounds_center = (t_vertex) {0.0,0.0,0.0};
	aid_model.bounds_radius = 2.0;
	aid_model.triangles_count = 2;
	aid_model.vertexes_count = 4;

	aid_model.vertexes = malloc(sizeof(t_vertex) * 4);

	aid_model.vertexes[0] = (t_vertex) {-0.2,-0.2,0.0};
	aid_model.vertexes[1] = (t_vertex) {-0.2,0.2,0.0};
	aid_model.vertexes[2] = (t_vertex) {0.2,0.2,0.0};
	aid_model.vertexes[3] = (t_vertex) {0.2,-0.2,0.0};

	aid_model.triangles = malloc(sizeof(t_triangle) * 2);
	aid_model.triangles[0].indexes[0] = 0;
	aid_model.triangles[0].indexes[1] = 1;
	aid_model.triangles[0].indexes[2] = 2;

	aid_model.triangles[1].indexes[0] = 0;
	aid_model.triangles[1].indexes[1] = 2;
	aid_model.triangles[1].indexes[2] = 3;

	aid_model.triangles[0].normal = (t_vertex) {0.0,0.0,1.0};
	aid_model.triangles[1].normal = (t_vertex) {0.0,0.0,1.0};

	aid_model.triangles[0].uvs[0] = (t_point) {0.0, 1.0,0.0};
	aid_model.triangles[0].uvs[1] = (t_point) {0.0, 0.0,0.0};
	aid_model.triangles[0].uvs[2] = (t_point) {1.0, 0.0,0.0};

	aid_model.triangles[1].uvs[0] = (t_point) {0.0, 1.0,0.0};
	aid_model.triangles[1].uvs[1] = (t_point) {1.0, 0.0,0.0};
	aid_model.triangles[1].uvs[2] = (t_point) {1.0, 1.0,0.0};

	aid_model.new_tex[0] = create_texture("textures/aid.bmp", 0xff00feff);
	aid_model.new_tex[0]->flags = 0xff00feff;

	aid_model.anim = NULL;
	
	aid.sprite.instance.model = aid_model;
	aid.sprite.instance.scale = 2.0;
	aid.enable = 0;
	aid.start_enable = 0;

	aid.sprite.instance.orientation = make_oy_rot_matrix(0.0);

	i = 0;
	while (i < 40)
	{
		doom->aid[i] = aid;
		i++;
	}
}

void	drb_init(t_doom *doom)
{
	doom->drb_anim = load_anim("textures/drb/", 15, 0xff00ffff);
	
	doom->drb_anim.curr = -1;
}


int		main(int ac, char **av)
{
	t_mgl			mgl;

	t_doom			doom;

	mgl = mgl_init("Doom_Quaekem", W, H, SCREEN_MULTIPLICATOR);
	
	if (ac < 2)
		exit(-2);


	mgl.show_fps = 1;
	// SDL_Surface *cursor_surface = SDL_LoadBMP("textures/cursor.bmp");
	// SDL_Cursor *cursor = SDL_CreateColorCursor(cursor_surface, 0, 0);
	// SDL_SetCursor(cursor);
	SDL_SetRelativeMouseMode(SDL_TRUE);


	doom.mgl = &mgl;

	doom.solid = 0;
	doom.g_speed = 0;


	doom.scene.level.instance.model.anim = NULL;
	doom.scene.level.instance.model.new_tex[0] = create_texture("textures/1.bmp", 0);
	doom.scene.level.instance.model.new_tex[1] = create_texture("textures/2.bmp", 0);
	doom.scene.level.instance.model.new_tex[2] = create_texture("textures/3.bmp", 0);
	doom.scene.level.instance.model.new_tex[3] = create_texture("textures/4.bmp", 0);
	doom.scene.level.instance.model.new_tex[4] = create_texture("textures/5.bmp", 0);
	doom.scene.level.instance.model.new_tex[5] = create_texture("textures/6.bmp", 0);

	

	doom.scene.sprites = malloc(sizeof(t_sprite) * 200);////
	doom.scene.sprites_count = 0;

	doom.scene.objects = malloc(sizeof(t_object) * 200);/////
	doom.scene.objects_count = 0;

	// doom.objects = malloc(sizeof(t_object) * 3);
	// doom.objects[0] = create_object((t_vertex){0.0,0.0,3.2});
	// doom.objects_count = 0;

	doom.health = 100;
	doom.player_ammo = 10;

	doom.height = 1.7;

	doom.sit = 0;
	doom.run = 0;

	doom.menu_opened = 1;

	doom.difficulty = 0;
	doom.enemy_damage = 10;
	doom.enemy_speed = 0.04;
	doom.kills = 0;

	doom.health_bar = NULL;
	doom.ammo_bar = NULL;
	doom.kills_bar = NULL;

	doom.win = 0;
	doom.lose = 0;


	
	if (!check_hash(av[1]))
	{
		ft_putendl("Файлы повреждены");
		exit(-2);
	}
	puts("OK");
		
	


	bullets_init(&doom);
	ammo_init(&doom);
	aid_init(&doom);
	drb_init(&doom);

	level_init(&doom, av[1]);
	render_init(&doom.scene);
	clipping_planes_init(&doom.scene);
	controls_init(&doom);

	TTF_Init();

	SDL_Color color;
	color.r = 0;
	color.g =0;
	color.b = 0;


	SDL_Surface *temp;

	doom.menu_back = create_texture("textures/menu.bmp", 0);//renderText("lol", "fonts/DoomsDay.ttf", color, 300);

	doom.menu.active = 0;
	doom.menu.play = renderText("PLAY", "fonts/DoomsDay.ttf", color, 100);
	doom.menu.difficulty_1 = renderText("DIFFICULTY LVL 1", "fonts/DoomsDay.ttf", color, 100);
	doom.menu.difficulty_2 = renderText("DIFFICULTY LVL 2", "fonts/DoomsDay.ttf", color, 100);
	doom.menu.difficulty_3 = renderText("DIFFICULTY LVL 3", "fonts/DoomsDay.ttf", color, 100);
	doom.menu.exit_b = renderText("EXIT", "fonts/DoomsDay.ttf", color, 100);
	color.r = 255;
	color.g =255;
	color.b = 255;
	doom.press_f = renderText("Press F", "fonts/DoomsDay.ttf", color, 70);


	doom.win_surface = create_texture("textures/menu.bmp", 0);
	doom.lose_surface = create_texture("textures/menu.bmp", 0);


	SDL_Rect rect;

	rect.x = -200;
	rect.y = -100;
	rect.w = W + 400;
	rect.h = H + 200;


	color.r = 35;
	color.g = 151;
	color.b = 35;
	
	temp = renderText("WIN", "fonts/DoomsDay.ttf", color, 300);
	SDL_BlitScaled(temp, &rect, doom.win_surface, NULL);
	
	color.r = 94;
	color.g = 6;
	color.b = 6;

	SDL_FreeSurface(temp);
	temp = renderText("LOSE", "fonts/DoomsDay.ttf", color, 300);
	SDL_BlitScaled(temp, &rect, doom.lose_surface, NULL);

	SDL_FreeSurface(temp);



	mgl_run(&mgl, update, event_handle, &doom);

	mgl_quit(&mgl);

	return (0);
}
