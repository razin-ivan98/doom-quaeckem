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
	int			ters_count;
	t_vertex	centre;
	
	i = 0;
	while (i < instance->model.triangles_count)
	{
		curr = instance->model.triangles[i];
		ters_count = 1;
		centre = multiply(sub(instance->model.vertexes[curr.indexes[0]],
			scene->camera.position), -1.0);
		curr.tex = instance->model.new_tex;

		if (dot(centre, curr.normal) >= 0.0 || lol)
		{
			clip_triangle(&curr, &ters_count, scene->clipping_planes, model);
		}
		i++;
	}
}

t_model	*transform_and_clip(t_instance *instance,t_mat4x4 transform, t_scene *scene, int lol)
{
	t_model		*model;
	t_vertex4	tmp;
	t_vertex	center;

	model = &scene->render_tr.rendered;

	model->vertexes_count = 0;
	model->triangles_count = 0;
	model->uvs_count = 0;

	if (!check_bounds_radius(instance, scene, &transform))
	{
		return (NULL);
	}

	transform_vertexes(model, instance, &transform);
	
	clip_triangles(model, instance, scene, lol);

	return (model);
}

void	render_level(int *image_data, t_scene *scene, t_mat4x4 camera_mat)
{
	t_mat4x4	transform;
	t_model		*model;

	update_instance_transform(&scene->level.instance);

	transform = multiply_m_m(camera_mat, 
							scene->level.instance.transform);

	if (!(model = transform_and_clip(&scene->level.instance, transform, scene, 0)))
	{
		return ;
	}

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
		if (!(model = transform_and_clip(&scene->sprites[i].instance, transform, scene, 1)))
		{
			i++;
			continue ;
		}
		render_model(image_data, model, scene);
		i++;
	}
}
void	render_objects(int *image_data, t_scene *scene, t_mat4x4 camera_mat)
{
	t_mat4x4	transform;
	t_model		*model;
	int			i;

	i = 0;

	while (i < scene->objects_count)
	{
		update_instance_transform(&scene->objects[i].instance);

		transform = multiply_m_m(camera_mat, scene->objects[i].instance.transform);
		if (!(model = transform_and_clip(&scene->objects[i].instance, transform, scene, 1)))
		{
			i++;
			continue ;
		}
		render_model(image_data, model, scene);
		i++;
	}

}
void	render_scene(int *image_data, t_scene *scene)
{
	t_mat4x4	camera_mat;
	t_mat4x4	transform;
	t_model		*model;

	camera_mat = multiply_m_m(transposed_m(scene->camera.orientation),
		make_translation_matrix(multiply(scene->camera.position, -1.0)));

	render_level(image_data, scene, camera_mat);
	render_sprites(image_data, scene, camera_mat);
	render_objects(image_data, scene, camera_mat);

}

