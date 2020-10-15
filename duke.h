// # include <SDL2/SDL.h>

#define W 720
#define H 720
#define H_2 360
#define W_2 360
#define HxW 518400
#define EPSILON 0.000001

#define CH 1
#define CW 1
#define V_SIZE 1
#define PROJ_PLANE_Z 1

#include "my_graphics_lib/my_graphics_lib.h"

#include "data.h"
#include "linear_algebra.h"
#include <string.h>
#include <fcntl.h>
#include "libft/libft.h"
#include "get_next_line/get_next_line.h"

void	render_init(t_scene *scene);
void	clipping_planes_init(t_scene *scene);
void	level_init(t_scene *scene);
void	controls_init(t_doom *doom);
SDL_Surface	*create_texture(char *filename);
t_anim		load_anim(char *foldername, float speed);

void	clip_1_outside(t_clip_triangle *cl, t_model *model, int k, int i);
void	clip_2_outsides(t_clip_triangle *cl, t_model *model, int k, int i);


int		clip_triangle(t_triangle *trs, int *count, t_plane *planes, t_model *model);

void	render_triangle(int *image_data, t_model *model, t_triangle *tr, t_scene *scene);



void put_pixel(int *image_data, int x, int y, int color);
void interpolate(float i0, float d0, float i1, float d1, t_float_array *dst);

void edge_interpolate(t_e_i_input in, t_render *render, t_edge_interpolate *edge);
t_e_i_output get_edge_inter(t_e_i_input in);

void concat(t_float_array *first, t_float_array *second);

//void	draw_line(int *image_data, t_point *p0, t_point *p1, int color);

float *create_z_buffer();
float *clear_z_buffer(float *z_buffer);
int set_z_buffer(float *z_buffer, int x, int y, float value);
t_mat4x4 update_instance_transform(t_instance *instance);

// int		get_texel(int *image_data, float u, float v, int pitch);

int get_texel(SDL_Surface *image_data, float u, float v);
int get_texel_n(int **image_data, float u, float v, int pitch);

void render_scene(int *image_data, t_scene *scene);

float get_inter_d(float i0, float d0, float i1, float d1);

void read_obj(t_model *model, char *file_name);

//void read_map(char *name, t_model *model, int **tex, int *texture);

////void create_box(t_model *model, int **new_tex, int *tex);

//t_bsp_node *create_bsp(t_model *model);
