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

void	level_init(t_scene *scene)
{

	read_bsp(&scene->level.root, "bsp_test/new_saved_bsp.json");

	scene->level.instance.model.vertexes = malloc(sizeof(t_vertex) * 30000);
	scene->level.instance.model.vertexes_count = 0;
	scene->level.instance.model.triangles = malloc(sizeof(t_triangle) * 30000);
	scene->level.instance.model.triangles_count = 0;
	scene->level.instance.model.uvs = malloc(sizeof(t_point) * 30000);
	scene->level.instance.model.uvs_count = 0;
	scene->level.instance.model.bounds_center = (t_vertex){0,0,0};
	scene->level.instance.model.bounds_radius = 100;

	read_obj(&scene->level.instance.model, "bsp_test/levl.obj");


	scene->level.instance.position = (t_vertex){ 0, 0, 10};
	scene->level.instance.clipped = malloc(sizeof(t_vertex) * 10);
	scene->level.instance.scale = 1.0;
	scene->level.instance.orientation = make_oy_rot_matrix(0.0);
	scene->level.instance.scale = 1.0;
	scene->level.instance.position = (t_vertex){0, 0, 0};



	scene->z_buffer = create_z_buffer();
	scene->camera.orientation = make_oy_rot_matrix(360.0);
	scene->camera.position = (t_vertex){0,0, -2};


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
	scene->clipping_planes[3] = (t_plane){(t_vertex){0.0,-coss,sins}, 0.0};//top
	scene->clipping_planes[4] = (t_plane){(t_vertex){0.0,coss,sins}, 0.0};//bottom
}

void	render_init(t_scene *scene)
{
	scene->render_tr.x.v012.array = malloc(sizeof(float) * H * 2);
	scene->render_tr.x.v02.array = malloc(sizeof(float) * H * 2);

	scene->render_tr.iz.v012.array = malloc(sizeof(float) * H * 2);
	scene->render_tr.iz.v02.array = malloc(sizeof(float) * H * 2);

	scene->render_tr.uz.v012.array = malloc(sizeof(float) * H * 2);
	scene->render_tr.uz.v02.array = malloc(sizeof(float) * H * 2);

	scene->render_tr.vz.v012.array = malloc(sizeof(float) * H * 2);
	scene->render_tr.vz.v02.array = malloc(sizeof(float) * H * 2);

	scene->render_tr.zscan.array = malloc(sizeof(float) * W * 2);
	scene->render_tr.uzscan.array = malloc(sizeof(float) * W * 2);
	scene->render_tr.vzscan.array = malloc(sizeof(float) * W * 2);

	scene->render_tr.v02.array = malloc(sizeof(float) * H * 2);
	scene->render_tr.v01.array = malloc(sizeof(float) * H * 2);
	scene->render_tr.v12.array = malloc(sizeof(float) * H * 2);


	scene->render_tr.rendered.vertexes = malloc(sizeof(t_vertex) * 30000);
	scene->render_tr.rendered.projected = malloc(sizeof(t_point) * 30000);
	scene->render_tr.rendered.triangles = malloc(sizeof(t_triangle) * 30000);
}
