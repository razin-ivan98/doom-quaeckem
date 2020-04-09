#include "duke.h"

void create_box(t_model *model, int **new_tex, int *tex)
{
	model->vertexes[0] = (t_vertex){ 1, 1, 1};
	model->vertexes[1] = (t_vertex){-1, 1, 1};
	model->vertexes[2] = (t_vertex){-1,-1, 1};
	model->vertexes[3] = (t_vertex){ 1,-1, 1};
	model->vertexes[4] = (t_vertex){ 1, 1,-1};
	model->vertexes[5] = (t_vertex){-1, 1,-1};
	model->vertexes[6] = (t_vertex){-1,-1,-1};
	model->vertexes[7] = (t_vertex){ 1,-1,-1};

	model->bounds_center = (t_vertex){0,0,0};
	model->bounds_radius = sqrt(3.0);

	model->triangles[0].indexes[0] = 0;
	model->triangles[0].indexes[1] = 1;
	model->triangles[0].indexes[2] = 2;
	model->triangles[0].normal = (t_vertex){ 0,0,1};
	model->triangles[0].texture = tex;
	model->triangles[0].new_tex = new_tex;
	model->triangles[0].color = 0xff0000;
	model->triangles[0].uvs[0] = (t_point){0,0, 0};
	model->triangles[0].uvs[1] = (t_point){1,0, 0};
	model->triangles[0].uvs[2] = (t_point){1,1, 0};


	model->triangles[1].indexes[0] = 0;
	model->triangles[1].indexes[1] = 2;
	model->triangles[1].indexes[2] = 3;
	model->triangles[1].normal = (t_vertex){ 0,0,1};
	model->triangles[1].texture = tex;
	model->triangles[1].new_tex = new_tex;
	model->triangles[1].color = 0xff0000;
	model->triangles[1].uvs[0] = (t_point){0,0, 0};
	model->triangles[1].uvs[1] = (t_point){1,1, 0};
	model->triangles[1].uvs[2] = (t_point){0,1, 0};


	model->triangles[2].indexes[0] = 4;
	model->triangles[2].indexes[1] = 0;
	model->triangles[2].indexes[2] = 3;
	model->triangles[2].normal = (t_vertex){ 1,0,0};
	model->triangles[2].texture = tex;
	model->triangles[2].new_tex = new_tex;
	model->triangles[2].color = 0x00ff00;
	model->triangles[2].uvs[0] = (t_point){0,0, 0};
	model->triangles[2].uvs[1] = (t_point){1,0, 0};
	model->triangles[2].uvs[2] = (t_point){1,1, 0};


	model->triangles[3].indexes[0] = 4;
	model->triangles[3].indexes[1] = 3;
	model->triangles[3].indexes[2] = 7;
	model->triangles[3].normal = (t_vertex){ 1,0,0};
	model->triangles[3].texture = tex;
	model->triangles[3].new_tex = new_tex;
	model->triangles[3].color = 0x00ff00;
	model->triangles[3].uvs[0] = (t_point){0,0, 0};
	model->triangles[3].uvs[1] = (t_point){1,1, 0};
	model->triangles[3].uvs[2] = (t_point){0,1, 0};


	model->triangles[4].indexes[0] = 5;
	model->triangles[4].indexes[1] = 4;
	model->triangles[4].indexes[2] = 7;
	model->triangles[4].normal = (t_vertex){0,0,-1};
	model->triangles[4].texture = tex;
	model->triangles[4].new_tex = new_tex;
	model->triangles[4].color = 0x0000ff;
	model->triangles[4].uvs[0] = (t_point){0,0, 0};
	model->triangles[4].uvs[1] = (t_point){1,0, 0};
	model->triangles[4].uvs[2] = (t_point){1,1, 0};


	model->triangles[5].indexes[0] = 5;
	model->triangles[5].indexes[1] = 7;
	model->triangles[5].indexes[2] = 6;
	model->triangles[5].normal = (t_vertex){0,0,-1};
	model->triangles[5].texture = tex;
	model->triangles[5].new_tex = new_tex;
	model->triangles[5].color = 0x0000ff;
	model->triangles[5].uvs[0] = (t_point){0,0, 0};
	model->triangles[5].uvs[1] = (t_point){1,1, 0};
	model->triangles[5].uvs[2] = (t_point){0,1, 0};





	model->triangles[6].indexes[0] = 1;
	model->triangles[6].indexes[1] = 5;
	model->triangles[6].indexes[2] = 6;
	model->triangles[6].normal = (t_vertex){-1,0,0};
	model->triangles[6].texture = tex;
	model->triangles[6].new_tex = new_tex;
	model->triangles[6].color = 0xffff00;
	model->triangles[6].uvs[0] = (t_point){0,0, 0};
	model->triangles[6].uvs[1] = (t_point){1,0, 0};
	model->triangles[6].uvs[2] = (t_point){1,1, 0};


	model->triangles[7].indexes[0] = 1;
	model->triangles[7].indexes[1] = 6;
	model->triangles[7].indexes[2] = 2;
	model->triangles[7].normal = (t_vertex){-1,0,0};
	model->triangles[7].texture = tex;
	model->triangles[7].new_tex = new_tex;
	model->triangles[7].color = 0xffff00;
	model->triangles[7].uvs[0] = (t_point){0,0, 0};
	model->triangles[7].uvs[1] = (t_point){1,1, 0};
	model->triangles[7].uvs[2] = (t_point){0,1, 0};


	model->triangles[8].indexes[0] = 1;
	model->triangles[8].indexes[1] = 0;
	model->triangles[8].indexes[2] = 5;
	model->triangles[8].normal = (t_vertex){0,1,0};
	model->triangles[8].texture = tex;
	model->triangles[8].new_tex = new_tex;
	model->triangles[8].color = 0x00ffff;
	model->triangles[8].uvs[0] = (t_point){0,0, 0};
	model->triangles[8].uvs[1] = (t_point){1,0, 0};
	model->triangles[8].uvs[2] = (t_point){1,1, 0};


	model->triangles[9].indexes[0] = 5;
	model->triangles[9].indexes[1] = 0;
	model->triangles[9].indexes[2] = 4;
	model->triangles[9].normal = (t_vertex){0,1,0};
	model->triangles[9].texture = tex;
	model->triangles[9].new_tex = new_tex;
	model->triangles[9].color = 0x00ffff;
	model->triangles[9].uvs[0] = (t_point){0,1, 0};
	model->triangles[9].uvs[1] = (t_point){1,1, 0};
	model->triangles[9].uvs[2] = (t_point){0,0, 0};


	model->triangles[10].indexes[0] = 2;
	model->triangles[10].indexes[1] = 6;
	model->triangles[10].indexes[2] = 7;
	model->triangles[10].normal = (t_vertex){0,-1,0};
	model->triangles[10].texture = tex;
	model->triangles[10].new_tex = new_tex;
	model->triangles[10].color = 0xff00ff;
	model->triangles[10].uvs[0] = (t_point){0,0, 0};
	model->triangles[10].uvs[1] = (t_point){1,0, 0};
	model->triangles[10].uvs[2] = (t_point){1,1, 0};


	model->triangles[11].indexes[0] = 2;
	model->triangles[11].indexes[1] = 7;
	model->triangles[11].indexes[2] = 3;
	model->triangles[11].normal = (t_vertex){0,-1,0};

	model->triangles[11].texture = tex;
	model->triangles[11].new_tex = new_tex;

	model->triangles[11].color = 0xff00ff;

	
	model->triangles[11].uvs[0] = (t_point){0,0, 0};
	model->triangles[11].uvs[1] = (t_point){1,1, 0};
	model->triangles[11].uvs[2] = (t_point){0,1, 0};

	model->triangles_count = 12;
	model->vertexes_count = 8;

}