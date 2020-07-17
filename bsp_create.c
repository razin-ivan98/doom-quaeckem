#include "duke.h"

t_plane		get_plane(t_triangle tr, t_model *model)
{
	t_plane plane;
	t_vertex f;
	t_vertex s;
	t_vertex t;

	f = model->vertexes[tr.indexes[0]];
	s = model->vertexes[tr.indexes[1]];
	t = model->vertexes[tr.indexes[2]];


	plane.normal.x = f.y * (s.z - t.z) + s.y * (t.z - f.z) + t.y * (f.z - s.z);
	plane.normal.y = f.z * (s.x - t.x) + s.z * (t.x - f.x) + t.z * (f.x - s.x);
	plane.normal.z = f.x * (s.y - t.y) + s.x * (t.y - f.y) + t.x * (f.y - s.y);
	plane.distance = -(    f.x * (s.y * t.z - t.y * s.z) + s.x * (t.y * f.z - f.y * t.z) + t.x * (f.y * s.z - s.y * f.z)    );



//	A = y1 (z2 - z3) + y2 (z3 - z1) + y3(z1 - z2)

//  B = z1 (x2 - x3) + z2 (x3 - x1) + z3(x1 - x2)

//    C = x1 (y2 - y3) + x2 (y3 - y1) + x3(y1 - y2)

 //   - D = x1(y2 z3 - y3 z2) + x2 (y3 z1 - y1 z3) + x3 (y1 z2 - y2 z1)
	return (plane);
}

void		add_node(t_bsp_node **ptr)
{
	*ptr = (t_bsp_node *)malloc(sizeof(t_bsp_node));//////////////////////
	(*ptr)->front = NULL;
	(*ptr)->back = NULL;
	(*ptr)->triangles_count = 0;
	(*ptr)->is_leaf = 0;

}

t_triangle *get_cutter(t_triangle *triangles, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		if (triangles[i].color == 0)
			return (&triangles[i]);
		i++;
	}
	return NULL;
}

void		cut_triangle(t_triangle *front, int *fr_count, t_triangle *back, int *b_count, t_triangle tr, t_plane pl, t_model *model)
{
	// int k;
	// int i;

	int i;

	int dot_v;
	// int j;
	int outside_count;
	int inside_count;
	int complanar_count;

	int	outsides[3];
	int insides[3];
	int complanars[3];
	// t_triangle *curr;
	// t_triangle crop[6][40];

	// int l;
	// crop[0][0] = trs[0];
	// int l_prev = 1;
	// l = 1;

	//bzero(mask, 40);///////////////
	
	outside_count = 0;
	inside_count = 0;
	complanar_count = 0;

	i = 0;

	while (i < 3)
	{
		dot_v = dot(pl.normal, model->vertexes[tr.indexes[i]]) + pl.distance;
		if (fabsf(dot_v) < EPSILON)
		{
			complanars[complanar_count] = i;
			complanar_count++;
		}
		else if (dot_v < 0.0)
		{
			outsides[outside_count] = i;
			outside_count++;
		}
		else
		{
			insides[inside_count] = i;
			inside_count++;
		}
		i++;
	}

	if (outside_count == 2)
	{
		t_vertex new1;
		t_vertex new2;

		t_triangle front_tr;
		t_triangle back_tr1;
		t_triangle back_tr2;

		float t1;
		float t2;

		t1 = (-pl.distance - dot(pl.normal, model->vertexes[tr.indexes[insides[0]]])) /
								dot(pl.normal, sub( model->vertexes[tr.indexes[outsides[0]]], model->vertexes[tr.indexes[insides[0]]]));
		new1 = add(model->vertexes[tr.indexes[insides[0]]], multiply(sub(model->vertexes[tr.indexes[outsides[0]]], model->vertexes[tr.indexes[insides[0]]]), t1));

		t2 = (-pl.distance - dot(pl.normal, model->vertexes[tr.indexes[insides[0]]])) /
								dot(pl.normal, sub( model->vertexes[tr.indexes[outsides[1]]], model->vertexes[tr.indexes[insides[0]]]));
		new2 = add(model->vertexes[tr.indexes[insides[0]]], multiply(sub(model->vertexes[tr.indexes[outsides[1]]], model->vertexes[tr.indexes[insides[0]]]), t2));


			
		model->vertexes[model->vertexes_count] = new1;////////////
		model->vertexes_count++;/////////////
		model->vertexes[model->vertexes_count] = new2;////////////
		model->vertexes_count++;/////////////


		front_tr = tr;
		back_tr1 = tr;
		back_tr2 = tr;

		front_tr.indexes[0] = tr.indexes[insides[0]];
		front_tr.indexes[1] = model->vertexes_count - 2;
		front_tr.indexes[2] = model->vertexes_count - 1;

		front_tr.uvs[0] = tr.uvs[insides[0]];

		front_tr.uvs[1].x = tr.uvs[outsides[0]].x + (tr.uvs[insides[0]].x - tr.uvs[outsides[0]].x) * (1 - t1);
		front_tr.uvs[1].y = tr.uvs[outsides[0]].y + (tr.uvs[insides[0]].y - tr.uvs[outsides[0]].y) * (1 - t1);

		front_tr.uvs[2].x = tr.uvs[outsides[1]].x + (tr.uvs[insides[0]].x - tr.uvs[outsides[1]].x) * (1 - t2);
		front_tr.uvs[2].y = tr.uvs[outsides[1]].y + (tr.uvs[insides[0]].y - tr.uvs[outsides[1]].y) * (1 - t2);



		back_tr1.indexes[0] = tr.indexes[outsides[0]];
		back_tr1.indexes[1] = model->vertexes_count - 2;
		back_tr1.indexes[2] = model->vertexes_count - 1;

		back_tr1.uvs[0] = tr.uvs[outsides[0]];
		back_tr1.uvs[1].x = front_tr.uvs[1].x;
		back_tr1.uvs[1].y = front_tr.uvs[1].y;

		back_tr1.uvs[2].x = front_tr.uvs[2].x;
		back_tr1.uvs[2].y = front_tr.uvs[2].y;



		back_tr2.indexes[0] = tr.indexes[outsides[0]];
		back_tr2.indexes[1] = model->vertexes_count - 1;
		back_tr2.indexes[2] = tr.indexes[outsides[1]];

		back_tr2.uvs[0] = tr.uvs[outsides[0]];
		back_tr2.uvs[1].x = front_tr.uvs[2].x;
		back_tr2.uvs[1].y = front_tr.uvs[2].y;
		back_tr2.uvs[2] = tr.uvs[outsides[1]];

		front[*fr_count] = front_tr;
		*fr_count++;
		back[*b_count] = back_tr1;
		*b_count++;
		back[*b_count] = back_tr2;
		*b_count++;
	}
	else if (complanar_count == 1 && inside_count == 1)
	{
		t_vertex new1;

		t_triangle front_tr;
		t_triangle back_tr;

		float t1;

		t1 = (-pl.distance - dot(pl.normal, model->vertexes[tr.indexes[insides[0]]])) /
								dot(pl.normal, sub( model->vertexes[tr.indexes[outsides[0]]], model->vertexes[tr.indexes[insides[0]]]));
		new1 = add(model->vertexes[tr.indexes[insides[0]]], multiply(sub(model->vertexes[tr.indexes[outsides[0]]], model->vertexes[tr.indexes[insides[0]]]), t1));

			
		model->vertexes[model->vertexes_count] = new1;////////////
		model->vertexes_count++;/////////////


		front_tr = tr;
		back_tr = tr;


		front_tr.indexes[0] = tr.indexes[insides[0]];
		front_tr.indexes[1] = tr.indexes[complanars[0]];
		front_tr.indexes[2] = model->vertexes_count - 1;

		front_tr.uvs[0] = tr.uvs[insides[0]];
		front_tr.uvs[1] = tr.uvs[complanars[0]];
		front_tr.uvs[2].x = tr.uvs[outsides[0]].x + (tr.uvs[insides[0]].x - tr.uvs[outsides[0]].x) * (1 - t1);
		front_tr.uvs[2].y = tr.uvs[outsides[0]].y + (tr.uvs[insides[0]].y - tr.uvs[outsides[0]].y) * (1 - t1);



		back_tr.indexes[0] = tr.indexes[outsides[0]];
		back_tr.indexes[1] = tr.indexes[complanars[0]];
		back_tr.indexes[2] = model->vertexes_count - 1;

		back_tr.uvs[0] = tr.uvs[outsides[0]];
		back_tr.uvs[1] = tr.uvs[complanars[0]];


		back_tr.uvs[2].x = front_tr.uvs[2].x;
		back_tr.uvs[2].y = front_tr.uvs[2].y;


		front[*fr_count] = front_tr;
		*fr_count++;
		back[*b_count] = back_tr;
		*b_count++;

	}
	else
	{
		t_vertex new1;
		t_vertex new2;

		t_triangle front_tr;
		t_triangle back_tr1;
		t_triangle back_tr2;

		float t1;
		float t2;

		pl.normal = multiply(pl.normal, -1.0);

		int sw[3];

	//	sw = insides;
	//	insides = outsides;
	//	outsides = sw;

		ft_memcpy(sw, insides, sizeof(int) * 3);
		ft_memcpy(insides, outsides, sizeof(int) * 3);
		ft_memcpy(outsides, sw, sizeof(int) * 3);


		t1 = (-pl.distance - dot(pl.normal, model->vertexes[tr.indexes[insides[0]]])) /
								dot(pl.normal, sub( model->vertexes[tr.indexes[outsides[0]]], model->vertexes[tr.indexes[insides[0]]]));
		new1 = add(model->vertexes[tr.indexes[insides[0]]], multiply(sub(model->vertexes[tr.indexes[outsides[0]]], model->vertexes[tr.indexes[insides[0]]]), t1));

		t2 = (-pl.distance - dot(pl.normal, model->vertexes[tr.indexes[insides[0]]])) /
								dot(pl.normal, sub( model->vertexes[tr.indexes[outsides[1]]], model->vertexes[tr.indexes[insides[0]]]));
		new2 = add(model->vertexes[tr.indexes[insides[0]]], multiply(sub(model->vertexes[tr.indexes[outsides[1]]], model->vertexes[tr.indexes[insides[0]]]), t2));


			
		model->vertexes[model->vertexes_count] = new1;////////////
		model->vertexes_count++;/////////////
		model->vertexes[model->vertexes_count] = new2;////////////
		model->vertexes_count++;/////////////


		front_tr = tr;
		back_tr1 = tr;
		back_tr2 = tr;

		front_tr.indexes[0] = tr.indexes[insides[0]];
		front_tr.indexes[1] = model->vertexes_count - 2;
		front_tr.indexes[2] = model->vertexes_count - 1;

		front_tr.uvs[0] = tr.uvs[insides[0]];

		front_tr.uvs[1].x = tr.uvs[outsides[0]].x + (tr.uvs[insides[0]].x - tr.uvs[outsides[0]].x) * (1 - t1);
		front_tr.uvs[1].y = tr.uvs[outsides[0]].y + (tr.uvs[insides[0]].y - tr.uvs[outsides[0]].y) * (1 - t1);

		front_tr.uvs[2].x = tr.uvs[outsides[1]].x + (tr.uvs[insides[0]].x - tr.uvs[outsides[1]].x) * (1 - t2);
		front_tr.uvs[2].y = tr.uvs[outsides[1]].y + (tr.uvs[insides[0]].y - tr.uvs[outsides[1]].y) * (1 - t2);



		back_tr1.indexes[0] = tr.indexes[outsides[0]];
		back_tr1.indexes[1] = model->vertexes_count - 2;
		back_tr1.indexes[2] = model->vertexes_count - 1;

		back_tr1.uvs[0] = tr.uvs[outsides[0]];
		back_tr1.uvs[1].x = front_tr.uvs[1].x;
		back_tr1.uvs[1].y = front_tr.uvs[1].y;

		back_tr1.uvs[2].x = front_tr.uvs[2].x;
		back_tr1.uvs[2].y = front_tr.uvs[2].y;



		back_tr2.indexes[0] = tr.indexes[outsides[0]];
		back_tr2.indexes[1] = model->vertexes_count - 1;
		back_tr2.indexes[2] = tr.indexes[outsides[1]];

		back_tr2.uvs[0] = tr.uvs[outsides[0]];
		back_tr2.uvs[1].x = front_tr.uvs[2].x;
		back_tr2.uvs[1].y = front_tr.uvs[2].y;
		back_tr2.uvs[2] = tr.uvs[outsides[1]];

		back[*b_count] = front_tr;
		*b_count++;
		front[*fr_count] = back_tr1;
		*fr_count++;
		front[*fr_count] = back_tr2;
		*fr_count++;
	}
	
}

int			classify_tr(t_triangle tr, t_plane pl, t_model *model)
{
	int outside_count;
	int inside_count;
	int complanar_count;
	float dot_v;
	int i;

	outside_count = 0;
	inside_count = 0;
	complanar_count = 0;
	i = 0;

	while (i < 3)
	{
		dot_v = dot(pl.normal, model->vertexes[tr.indexes[i]]) + pl.distance;
		if (fabsf(dot_v) < EPSILON)
			complanar_count++;
		else if (dot_v < 0.0)
			outside_count++;
		else
			inside_count++;
		i++;
	}
	if (complanar_count > 2)
		return (COMPLANAR);
	if (inside_count == 0)
		return (BACK);
	if (outside_count == 0)
		return (FRONT);
	return (CUTTED);
}

void		bsp_recurse(t_bsp_node *node, t_triangle *triangles, int trs_count, t_model *model)
{
	t_triangle *cutter;
	t_triangle curr;

	t_triangle *front_trs;
	t_triangle *back_trs;

	int front_count;
	int back_count;

	front_count = 0;
	back_count = 0;

	int i;
	int result;

	front_trs = (t_triangle *)malloc(sizeof(t_triangle) * 1000);//////////////////
	back_trs =  (t_triangle *)malloc(sizeof(t_triangle) * 1000);///////////////////


	///if (trs_count == 0)
	if (trs_count == 0)
	{
		node->is_leaf = -1;
		free(triangles);
		return ;
	}

	cutter = get_cutter(triangles, trs_count);/////////if NULL

	if (cutter == NULL)
	{
		node->is_leaf = 1;
		node->triangles = triangles;
		node->triangles_count = trs_count;
		return ;
	}


	cutter->color = 1;

	node->plane = get_plane(*cutter, model);
	cutter->color = 1;

	//front_trs[0] = *cutter;
	//front_count = 1;

	i = 0;
	while (i < trs_count)
	{
		curr = triangles[i];
		
		result = classify_tr(curr, node->plane, model);

		if (result == COMPLANAR)
		{
			curr.color = 1;
			front_trs[front_count] = curr;
			front_count++;
		}
		else if (result == CUTTED)
		{
			cut_triangle(front_trs, &front_count, back_trs, &back_count, curr, node->plane, model);
		}
		else if (result == FRONT)
		{
			front_trs[front_count] = curr;
			front_count++;
		}
		else if (result == BACK)
		{
			back_trs[back_count] = curr;
			back_count++;
		}
		i++;
	}
	
	free(triangles);
	add_node(&node->back);
	bsp_recurse(node->back, back_trs, back_count, model);

	add_node(&node->front);
	bsp_recurse(node->front, front_trs, front_count, model);

	

}

t_bsp_node	*create_bsp(t_model *model)
{
	t_bsp_node *root;

	t_triangle *trs;

	trs = (t_triangle *)malloc(sizeof(t_triangle) * 2000);//////////////////

	int i;

	i = 0;
	while (i < model->triangles_count)
	{
		trs[i] = model->triangles[i];
		trs[i].color = 0;
		i++;
	}

	add_node(&root);

	bsp_recurse(root, trs, model->triangles_count, model);

	return (root);
}
