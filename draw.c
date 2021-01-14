#include "duke.h"

void render_model(int *image_data, t_model *model, t_scene *scene)
{
	int i;

	i = 0;
	while (i < model->vertexes_count)
	{
		model->projected[i] = project_vertex(model->vertexes[i]);
		i++;
	}
	i = 0;
	while (i < model->triangles_count)
	{	
		render_triangle(image_data, model, &model->triangles[i], scene);
		i++;
	}
}


int		check_bounds_radius(t_instance *instance, t_scene *scene, t_mat4x4 *transform)
{
	t_vertex4	tmp;
	t_vertex	center;
	float		radius2;
	int			i;
	float		distance2;

	radius2 = instance->model.bounds_radius * instance->scale;
	
	tmp = multiply_m_v(*transform, (t_vertex4){
									instance->model.bounds_center.x,
									instance->model.bounds_center.y,
									instance->model.bounds_center.z,
									1.0
									});


	center = (t_vertex){tmp.x, tmp.y, tmp.z};
	i = 0;
	while (i < 5)
	{
		distance2 = dot(scene->clipping_planes[i].normal, center) +
							scene->clipping_planes[i].distance;
		if (distance2 < -radius2)
			return (0);
		i++;
	}
	return (1);
}

void	transform_vertexes(t_model *model, t_instance *instance, t_mat4x4 *transform)
{
	int			i;
	t_vertex4	tmp;

	i = 0;
	while (i < instance->model.vertexes_count)
	{
		tmp = (t_vertex4){
			instance->model.vertexes[i].x,
			instance->model.vertexes[i].y,
			instance->model.vertexes[i].z,
			1.0
		};
		tmp = multiply_m_v(*transform, tmp);
		model->vertexes[i] = (t_vertex){tmp.x, tmp.y, tmp.z};
		model->vertexes_count++;
		i++;
	}
}

void	clip_triangles( t_model *model, t_instance *instance, t_scene *scene, int lol)///////
{
	int			i;
	t_triangle	curr;
	t_vertex	centre;
	
	i = 0;
	while (i < instance->model.triangles_count)
	{
		curr = instance->model.triangles[i];
		centre = multiply(sub(instance->model.vertexes[curr.indexes[0]],
			scene->camera.position), -1.0);
		curr.tex = instance->model.new_tex[0];

		if (dot(centre, curr.normal) >= 0.0 || lol)
		{
			clip_triangle(&curr, scene->clipping_planes, model);
		}
		i++;
	}
}

int classify_point(t_vertex cam, t_vertex line, t_vertex normal)
{
	t_vertex new;

	cam.z = 0.0;
	normal.z = 0.0;

	new = (t_vertex){cam.x, line.x * cam.x + line.y, 0.0};

	if (dot(sub(new, cam), normal) < 0.0)
		return (BACK);
	return (FRONT);
}


void	bsp_render_traversal(t_bsp *node, t_scene *scene, t_instance *instance, t_model *model)
{
	int i;
	t_triangle	curr;
	t_vertex	centre;

	if (node->is_leaf)
	{
		i = 0;
		while (i < node->vt_trs_count)
		{
			curr.normal = instance->model.normals[node->vt_trs[i].n_ids[0]];
			curr.indexes[0] = node->vt_trs[i].ids[0];
			curr.indexes[1] = node->vt_trs[i].ids[1];
			curr.indexes[2] = node->vt_trs[i].ids[2];

			curr.uvs[0] = instance->model.uvs[node->vt_trs[i].uv_ids[0]];
			curr.uvs[1] = instance->model.uvs[node->vt_trs[i].uv_ids[1]];
			curr.uvs[2] = instance->model.uvs[node->vt_trs[i].uv_ids[2]];

			centre = multiply(sub(instance->model.vertexes[curr.indexes[0]],
				scene->camera.position), -1.0);
			if (node->vt_trs[i].type == TR_TYPE_WALL)
				curr.tex = instance->model.new_tex[node->wall_tex];
			else if (node->vt_trs[i].type == TR_TYPE_FLOOR)
				curr.tex = instance->model.new_tex[node->floor_tex];
			else
				curr.tex = instance->model.new_tex[node->ceil_tex];

			if (dot(centre, curr.normal) >= 0.0)
			{
				clip_triangle(&curr, scene->clipping_planes, model);
			}
			i++;
		}
	}
	else
	{
		if (classify_point((t_vertex){scene->camera.position.x,
			scene->camera.position.z, 0.0}, node->line, node->normal) == FRONT)
		{
			bsp_render_traversal(node->back, scene, instance, model);
			bsp_render_traversal(node->front, scene, instance, model);
		}
		else
		{
			bsp_render_traversal(node->front, scene, instance, model);
			bsp_render_traversal(node->back, scene, instance, model);
		}
	}
}


t_model	*transform_and_clip(t_instance *instance,t_mat4x4 transform, t_scene *scene, int mode)
{
	t_model		*model;

	model = &scene->render_tr->rendered;

	model->vertexes_count = 0;
	model->triangles_count = 0;
	model->uvs_count = 0;

	if (!check_bounds_radius(instance, scene, &transform))
	{
		return (NULL);
	}

	transform_vertexes(model, instance, &transform);
	
	if (mode == 1)//////bsp
		bsp_render_traversal(&scene->level.root, scene, instance, model);
	else
		clip_triangles(model, instance, scene, 1);

	return (model);
}

void	render_level(int *image_data, t_scene *scene, t_mat4x4 camera_mat)
{
	t_mat4x4	transform;
	t_model		*model;

	update_instance_transform(&scene->level.instance);

	transform = multiply_m_m(camera_mat, 
							scene->level.instance.transform);

	if (!(model = transform_and_clip(&scene->level.instance, transform, scene, 1)))
	{
		return ;
	}
	scene->depth_mode = 1;
	render_model(image_data, model, scene);
}
void	render_sprites(int *image_data, t_scene *scene, t_mat4x4 camera_mat)
{
	t_mat4x4	transform;
	t_model		*model;
	int			i;

	i = 0;

	while (i < scene->sprites_count)
	{
		update_instance_transform(&scene->sprites[i].instance);

		transform = multiply_m_m(camera_mat, scene->sprites[i].instance.transform);
		if (!(model = transform_and_clip(&scene->sprites[i].instance, transform, scene, 0)))
		{
			i++;
			continue ;
		}
		scene->depth_mode = 0;

		render_model(image_data, model, scene);
		i++;
	}
}

void	render_scene(int *image_data, t_scene *scene)
{
	t_mat4x4	camera_mat;

	camera_mat = multiply_m_m(transposed_m(scene->camera.orientation),
		make_translation_matrix(multiply(scene->camera.position, -1.0)));

	render_level(image_data, scene, camera_mat);
	render_sprites(image_data, scene, camera_mat);
}



void	*pthread_fun(void *ptr)
{
	t_pthread_data *data;

	data = (t_pthread_data *)ptr;

	render_scene(data->image_data, &data->scene);
	return (NULL);
}

void	pthread_render(int *image_data, t_doom *doom)
{
	t_pthread_data f;
	t_pthread_data s;
	pthread_t thread1;
	pthread_t thread2;

	f.scene = doom->scene;
	s.scene = doom->scene;

	f.image_data = image_data;
	s.image_data = image_data;


	f.scene.clipping_planes[4] = (t_plane){(t_vertex){0.0,1.0,0.0}, 0.0};//bottom
	s.scene.clipping_planes[3] = (t_plane){(t_vertex){0.0,-1.0,0.0}, 0.0};//top

	f.scene.render_tr = &s.scene.f_render_tr;
	s.scene.render_tr = &s.scene.s_render_tr;






	pthread_create(&(thread1), NULL, pthread_fun, &f);
	pthread_create(&(thread2), NULL, pthread_fun, &s);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}

