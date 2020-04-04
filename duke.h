# include <SDL2/SDL.h>

# define W 1000
# define H 500
#define CH 1
# define CW 2
# define V_SIZE 1
# define PROJ_PLANE_Z 1

# include "data.h"
# include "linear_algebra.h"


void			put_pixel(int *image_data, int x, int y, int color);
t_float_array	interpolate(float i0, float d0, float i1, float d1);

t_edge_interpolate	edge_interpolate(t_e_i_input in);

t_float_array	*concat(t_float_array *first, t_float_array *second);

void	draw_line(int *image_data, t_point *p0, t_point *p1, int color);



float *create_z_buffer();
float *clear_z_buffer(float *z_buffer);
int set_z_buffer(float *z_buffer, int x, int y, float value);
t_mat4x4	update_instance_transform(t_instance *instance);


// int		get_texel(int *image_data, float u, float v, int pitch);

int		get_texel(int *image_data, float u, float v, int pitch, t_float_array *uzscan, t_float_array *vzscan);

void	render_scene(int *image_data, t_scene *scene);