# include <SDL2/SDL.h>

# define W 800
# define H 800
# define H_2 400
# define W_2 400
# define HxW 640000


# define CH 2
# define CW 2
# define V_SIZE 1
# define PROJ_PLANE_Z 1

# include "data.h"
# include "linear_algebra.h"
# include <string.h>


void			put_pixel(int *image_data, int x, int y, int color);
void        	interpolate(float i0, float d0, float i1, float d1, t_float_array *dst);

void        	edge_interpolate(t_e_i_input in, t_render *render, t_edge_interpolate *edge);
t_e_i_output	get_edge_inter(t_e_i_input in);

void            concat(t_float_array *first, t_float_array *second);

void	draw_line(int *image_data, t_point *p0, t_point *p1, int color);



float *create_z_buffer();
float *clear_z_buffer(float *z_buffer);
int set_z_buffer(float *z_buffer, int x, int y, float value);
t_mat4x4	update_instance_transform(t_instance *instance);


// int		get_texel(int *image_data, float u, float v, int pitch);

int		get_texel(int *image_data, float u, float v, int pitch);

void	render_scene(int *image_data, t_scene *scene);





 float get_inter_d(float i0, float d0, float i1, float d1);