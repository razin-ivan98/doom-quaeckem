#include "duke.h"

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