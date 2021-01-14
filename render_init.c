#include "duke.h"

void	controls_init(t_doom *doom)
{
	doom->beta = 0.0;
	doom->gamma = 0.0;
	doom->alpha = 0.0;

	doom->w_pressed = 0;
	doom->s_pressed = 0;
	doom->a_pressed = 0;
	doom->d_pressed = 0;


	doom->mouse_pressed = 0;
	doom->mouse_right_pressed = 0;
}

void	correct_objects_height(t_doom *doom)
{
	int i;
	float floor;
	float ceil;

	i = 0;
	while (i < 40)
	{
		if (doom->aid[i].enable)
		{
			get_floor_seil_traversal(&doom->scene.level.root,
				(t_vertex){doom->aid[i].sprite.instance.position.x,
				doom->aid[i].sprite.instance.position.z, 0.0}, &floor, &ceil);
			doom->aid[i].sprite.instance.position.y = floor + 0.3;
		}
		if (doom->ammo[i].enable)
		{
			get_floor_seil_traversal(&doom->scene.level.root,
				(t_vertex){doom->ammo[i].sprite.instance.position.x,
				doom->ammo[i].sprite.instance.position.z, 0.0}, &floor, &ceil);
			doom->ammo[i].sprite.instance.position.y = floor + 0.3;
		}
		i++;
	}
	
	i = 0;
	while (i < doom->objects_count)
	{
		get_floor_seil_traversal(&doom->scene.level.root,
				(t_vertex){doom->objects[i].sprite.instance.position.x,
				doom->objects[i].sprite.instance.position.z, 0.0}, &floor, &ceil);
			doom->objects[i].sprite.instance.position.y = floor + 0.8;
		
		i++;
	}
	get_floor_seil_traversal(&doom->scene.level.root,
				(t_vertex){doom->tv.sprite.instance.position.x,
				doom->tv.sprite.instance.position.z, 0.0}, &floor, &ceil);
			doom->tv.sprite.instance.position.y = floor + 1.5;
}

void	level_init(t_doom *doom, char* foldername)
{
	t_scene *scene;

	scene = &doom->scene;

	char path[1024];

	scene->z_buffer = create_z_buffer();
	scene->camera.orientation = make_oy_rot_matrix(360.0);
	scene->camera.position = (t_vertex){0,0, -2};
	//puts(path);
	ft_strcpy(path, foldername);
	ft_strcat(path, "/data.json");
	read_bsp(doom, path);

	correct_objects_height(doom);

	scene->level.instance.model.vertexes = malloc(sizeof(t_vertex) * 30000);
	scene->level.instance.model.vertexes_count = 0;
	scene->level.instance.model.triangles = malloc(sizeof(t_triangle) * 30000);
	scene->level.instance.model.triangles_count = 0;
	scene->level.instance.model.uvs = malloc(sizeof(t_point) * 30000);
	scene->level.instance.model.uvs_count = 0;
	scene->level.instance.model.bounds_center = (t_vertex){0,0,0};
	scene->level.instance.model.bounds_radius = 100;
	
	ft_strcpy(path, foldername);
	ft_strcat(path, "/geometry.obj");
	read_obj(&scene->level.instance.model, path);
//	puts(path);

	scene->level.instance.position = (t_vertex){ 0, 0, 10};
	scene->level.instance.clipped = malloc(sizeof(t_vertex) * 10);
	scene->level.instance.scale = 1.0;
	scene->level.instance.orientation = make_oy_rot_matrix(0.0);
	scene->level.instance.scale = 1.0;
	scene->level.instance.position = (t_vertex){0, 0, 0};


}

void	clipping_planes_init(t_scene *scene)
{
	float sins;
	float coss;

	sins = 1 / sqrt(5);
	coss = sins * 2;

	scene->clipping_planes[0] = (t_plane){(t_vertex){0.0,0.0,1.0}, /*-1.0*/0.0}; //near
	scene->clipping_planes[1] = (t_plane){(t_vertex){coss,0.0,sins}, 0.0}; //left
	scene->clipping_planes[2] = (t_plane){(t_vertex){-coss,0.0,sins}, 0.0};//right
	// scene->clipping_planes[3] = (t_plane){(t_vertex){0.0,-coss,sins}, 0.0};//top
	scene->clipping_planes[3] = (t_plane){(t_vertex){0.0,-coss,sins}, 0.0};//top

	scene->clipping_planes[4] = (t_plane){(t_vertex){0.0,coss,sins}, 0.0};//bottom
}

void	render_init(t_scene *scene)
{
	scene->f_render_tr.x.v012.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.x.v02.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.iz.v012.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.iz.v02.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.uz.v012.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.uz.v02.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.vz.v012.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.vz.v02.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.zscan.array = malloc(sizeof(float) * W * 20);
	scene->f_render_tr.uzscan.array = malloc(sizeof(float) * W * 20);
	scene->f_render_tr.vzscan.array = malloc(sizeof(float) * W * 20);
	scene->f_render_tr.v02.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.v01.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.v12.array = malloc(sizeof(float) * H * 20);
	scene->f_render_tr.rendered.vertexes = malloc(sizeof(t_vertex) * 30000);
	scene->f_render_tr.rendered.projected = malloc(sizeof(t_point) * 30000);
	scene->f_render_tr.rendered.triangles = malloc(sizeof(t_triangle) * 30000);

	scene->s_render_tr.x.v012.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.x.v02.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.iz.v012.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.iz.v02.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.uz.v012.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.uz.v02.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.vz.v012.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.vz.v02.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.zscan.array = malloc(sizeof(float) * W * 20);
	scene->s_render_tr.uzscan.array = malloc(sizeof(float) * W * 20);
	scene->s_render_tr.vzscan.array = malloc(sizeof(float) * W * 20);
	scene->s_render_tr.v02.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.v01.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.v12.array = malloc(sizeof(float) * H * 20);
	scene->s_render_tr.rendered.vertexes = malloc(sizeof(t_vertex) * 30000);
	scene->s_render_tr.rendered.projected = malloc(sizeof(t_point) * 30000);
	scene->s_render_tr.rendered.triangles = malloc(sizeof(t_triangle) * 30000);
}
