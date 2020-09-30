#include "map_editor.h"


int			get_i_plus_1(int i, int max)
{
	return ((i + 1) % max);
}

int			get_i_minus_1(int i, int max)
{
	if (i != 0)
		return (i - 1);
	return (max - 1);
}

int			get_i_plus_2(int i, int max)
{
	return ((i + 2) % max);
}


void		delete_by_index(t_pt_id *points, int index, int *count)
{
	int i;

	i = index;
	(*count)--;
	while (i < *count)
	{
		points[i] = points[i + 1];
		i++;
	}
}

void		insert_by_index(t_pt_id *points, int index, int *count, t_pt_id p)
{
	int i;

	i = *count;
	while (i > index)
	{
		points[i] = points[i - 1];///////////////
		i--;
	}
	points[index] = p;
	(*count)++;
}

int			add_vt(t_map_editor *ed, t_pt_id point)
{
	int i;

	i = 0;
	while (i < ed->map.vrts_count)
	{
		if (ed->map.vrts[i].id == point.id && ed->map.vrts[i].c_id == point.c_id)
			return (i);
		i++;
	}
	ed->map.vrts[ed->map.vrts_count].id = point.id;
	ed->map.vrts[ed->map.vrts_count].c_id = point.c_id;

	ed->map.vrts[ed->map.vrts_count].vt.x = point.pt.x;
	ed->map.vrts[ed->map.vrts_count].vt.y = point.pt.y;
	ed->map.vrts[ed->map.vrts_count].vt.z = -1.0;

	(ed->map.vrts_count)++;
	return (ed->map.vrts_count - 1);
}

void		create_tr(t_pt_id *pts, t_map_editor *ed, int *pts_count, int i)
{
	t_tr_idx	res;

	res.ids[0] = add_vt(ed, pts[i]);
	res.ids[1] = add_vt(ed, pts[get_i_plus_1(i, *pts_count)]);
	res.ids[2] = add_vt(ed, pts[get_i_plus_2(i, *pts_count)]);

	ed->map.trs_ids[ed->map.trs_ids_count] = res;


	(ed->map.trs_ids_count)++;

	delete_by_index(pts, get_i_plus_1(i, *pts_count), pts_count);
}

int sign(float a)
{
	if (a == 0.0)
		return (0);
	if (a > 0.0)
		return (1);
	return (-1);
}

int			if_intersect(t_vertex v11, t_vertex v12, t_vertex v21, t_vertex v22)
{


	t_vertex cut1;
	t_vertex cut2;

	t_vertex prod1;
	t_vertex prod2;

	cut1 = sub(v12, v11);
	cut2 = sub(v22, v21);

	

	prod1 = cross(cut1, sub(v21, v11));
	prod2 = cross(cut1, sub(v22, v11));


//	printf("prod11 %f\n", prod1.z);
	//printf("prod12 %f\n", prod2.z);

	if(sign(prod1.z) == sign(prod2.z) || sign(prod1.z) == 0 || sign(prod2.z) == 0) // Отсекаем также и пограничные случаи
		return 0;
	

	prod1 = cross(cut2, sub(v11, v21));
	prod2 = cross(cut2, sub(v12, v21));

//	printf("prod21 %f\n", prod1.z);
	//printf("prod22 %f\n\n", prod2.z);

	if(sign(prod1.z) == sign(prod2.z) || sign(prod1.z) == 0 || sign(prod2.z) == 0) // Отсекаем также и пограничные случаи
		return 0;

	// if (crossing) { // Проверяем, надо ли определять место пересечения
	// 	(*crossing)[X] = v11[X] + cut1[X]*fabs(prod1[Z])/fabs(prod2[Z]-prod1[Z]);
	// 	(*crossing)[Y] = v11[Y] + cut1[Y]*fabs(prod1[Z])/fabs(prod2[Z]-prod1[Z]);
	// }

	return 1;

}

int			check_intersection(t_vertex new1, t_vertex new2, t_pt_id *pts, int pts_count)
{
	int j;
	int k;
	t_vertex v1;
	t_vertex v2;

	j = 0;

	while (j < pts_count)//по еще не отсеченным
	{

		v1 = (t_vertex){
				pts[j].pt.x,
				pts[j].pt.y,
				0.0
					};
		v2 = (t_vertex){
				pts[get_i_plus_1(j, pts_count)].pt.x,
				pts[get_i_plus_1(j, pts_count)].pt.y,
				0.0
					};

		if (if_intersect(new1, new2, v1, v2) == 1)
			return (1);
		j++;
	}

	//по уже отсеченным
	
	return (-1);
}

int			check_intersection_with_trs(t_vtx_id new1, t_vtx_id new2, t_map_editor *ed)
{
	int j;

	j = 0;
	while (j < ed->map.trs_ids_count)
	{
		t_pt_id pts[3];

		pts[0] = (t_pt_id){ed->map.trs[j].points[0], 0};
		pts[1] = (t_pt_id){ed->map.trs[j].points[1], 0};
		pts[2] = (t_pt_id){ed->map.trs[j].points[2], 0};

		if (check_intersection(new1.vt, new2.vt, pts, 3) == 1)
			return (1);



		// k = 0;
		// while (k < 3)
		// {
		// 	v1 = (t_vertex){
		// 		ed->map.trs[j].points[k].x,
		// 		ed->map.trs[j].points[k].y,
		// 		0.0
		// 			};
		// 	v2 = (t_vertex){
		// 		ed->map.trs[j].points[get_i_plus_1(k, 3)].x,
		// 		ed->map.trs[j].points[get_i_plus_1(k, 3)].y,
		// 		0.0
		// 			};
		// 	if (if_intersect(new1, new2, v1, v2) == 1)
		// 		return (1);
		// 	k++;
		// }
		j++;
	}
	return (-1);
}

int			check_fictive_cut_in_figure(t_pt_id *pts, int pts_count, int i, t_map_editor *ed, t_pt_id new_p)
{
	t_vertex new;
	t_vertex base;
	t_vertex end;
	t_vertex sub1;
	t_vertex sub2;

	base = normalize((t_vertex){
						pts[get_i_minus_1(i, pts_count)].pt.x - pts[i].pt.x,
						pts[get_i_minus_1(i, pts_count)].pt.y - pts[i].pt.y,
						0.0
				});
	new = normalize((t_vertex){
						new_p.pt.x - pts[i].pt.x,
						new_p.pt.y - pts[i].pt.y,
						0.0
					});
	end = normalize((t_vertex){
						pts[get_i_plus_1(i, pts_count)].pt.x - pts[i].pt.x,
						pts[get_i_plus_1(i, pts_count)].pt.y - pts[i].pt.y,
						0.0
				});

	float angle1 = acosf(dot(base, new));
	float angle2 = acosf(dot(base, end));

	if (cross(base, new).z < 0.0)
		angle1 = 2 * M_PI - angle1;
	if (cross(base, end).z < 0.0)
		angle2 = 2 * M_PI - angle2;

	if (angle2 < angle1 && angle1 > 0.0)
		return (1);
	return (0);
}

int			check_in_figure(t_pt_id *pts, int pts_count, int i, t_map_editor *ed)
{
	t_vertex new;
	t_vertex base;
	t_vertex end;
	t_vertex sub1;
	t_vertex sub2;

	base = normalize((t_vertex){
						pts[get_i_minus_1(i, pts_count)].pt.x - pts[i].pt.x,
						pts[get_i_minus_1(i, pts_count)].pt.y - pts[i].pt.y,
						0.0
				});
	new = normalize((t_vertex){
						pts[get_i_plus_2(i, pts_count)].pt.x - pts[i].pt.x,
						pts[get_i_plus_2(i, pts_count)].pt.y - pts[i].pt.y,
						0.0
					});
	end = normalize((t_vertex){
						pts[get_i_plus_1(i, pts_count)].pt.x - pts[i].pt.x,
						pts[get_i_plus_1(i, pts_count)].pt.y - pts[i].pt.y,
						0.0
				});
	// sub1 = sub(base, new);
	// sub2 = sub(base, end);

	// if (atan2f(sub1.x, sub1.y) < atan2f(sub2.x, sub2.y))
	// 	return (1);
	// return (0);


	float angle1 = acosf(dot(base, new));
	float angle2 = acosf(dot(base, end));
	printf("cos1: %f \n", dot(base, new));
	printf("cos2: %f \n", dot(base, end));

	if (cross(base, new).z < 0.0)
		angle1 = 2 * M_PI - angle1;
	if (cross(base, end).z < 0.0)
		angle2 = 2 * M_PI - angle2;

	printf("a1: %f \n", angle1);
	printf("a2: %f \n", angle2);

	if (angle2 < angle1 && angle1 > 0.0)
		return (1);
	return (0);
	


//	prod = cross(new, v);

//	if (prod.z * ed->map.circuits[0].normal_dir > 0.0)
//		return (0);



//	prod = cross(new, v);

//	if (prod.z * ed->map.circuits[0].normal_dir < 0.0)
//		return (0);
	
//	return (1);
}

void		fictive_cut(t_pt_id *pts, int *pts_count, t_pt_id *hole_pts, int hole_pts_count ,int j, int k)
{
	int i;

	i = 0;
	//insert_by_index(pts, j + 1, pts_count, hole_pts[k]);
	while (i < hole_pts_count)
	{
//	printf("i: %d\t j: %d\n",i,j);
		
		insert_by_index(pts, j + 1 + i, pts_count, hole_pts[k]);
		k = get_i_plus_1(k, hole_pts_count);
		i++;
	}
	insert_by_index(pts, j + 1 + i, pts_count, hole_pts[k]);
	insert_by_index(pts, j + 2 + i, pts_count, pts[j]);

	puts("ïnsert");

}

int			check_intersection_with_all(t_vtx_id new1, t_vtx_id new2, t_map_editor *ed)
{
	int		i;
	t_pt_id	hole_pts[30];
	int		hole_pts_count;

	i = 0;
	while (i < ed->map.circuits_count)
	{
		ft_memcpy(hole_pts, ed->map.circuits[i].points, sizeof(t_pt_id) * (ed->map.circuits[i].points_count - 1));
		hole_pts_count = ed->map.circuits[i].points_count - 1;
		if (check_intersection(new1.vt, new2.vt, hole_pts, hole_pts_count) == 1)
			return (1);
		i++;
	}
	return (-1);
}

void		pts_rev(t_pt_id *pts, int pts_count)
{
	int		i;
	t_pt_id	swap;

	i = 0;
	while (1)
	{
		if (i - pts_count + i + 1 >= 0)
			break ;
		swap = pts[i];
		pts[i] = pts[pts_count - i - 1];
		pts[pts_count - i - 1] = swap;
		i++;
	}
}

int		integrate_hole(t_map_editor *ed, int i, t_pt_id *pts, int *pts_count)
{
	int j;
	int k;
	t_pt_id	hole_pts[30];
	int		hole_pts_count;
	t_vtx_id new1;
	t_vtx_id new2;

	ft_memcpy(hole_pts, ed->map.circuits[i].points, sizeof(t_pt_id) * (ed->map.circuits[i].points_count - 1));
	hole_pts_count = ed->map.circuits[i].points_count - 1;

	if (ed->map.circuits[i].normal_dir == -1)
	{
		pts_rev(hole_pts, hole_pts_count);
	}



	j = 0;
	while (j < *pts_count)
	{
		new1 = (t_vtx_id){(t_vertex){pts[j].pt.x, pts[j].pt.y, 0.0}, 0, 0};
		k = 0;
		while (k < hole_pts_count)
		{
			new2 = (t_vtx_id){(t_vertex){hole_pts[k].pt.x, hole_pts[k].pt.y, 0.0}, 0, 0};

			if (check_intersection(new1.vt, new2.vt, pts, *pts_count) == -1
				&& check_intersection(new1.vt, new2.vt, hole_pts, hole_pts_count) == -1
				&& check_intersection_with_all(new1, new2, ed) == -1
				&& check_fictive_cut_in_figure(pts, *pts_count, j, ed, hole_pts[k]) == 1)
			{
				fictive_cut(pts, pts_count, hole_pts, hole_pts_count, j, k);
				ed->map.circuits[i].integrated = 1;
				return (1);
			}
			k++;
		}
		j++;
	}
	return (0);
}



void		triangulate(t_map_editor *ed)
{
	t_pt_id		pts[100];
	int			pts_count;
	int 		i;

	int p;

	t_vtx_id new1;
	t_vtx_id new2;

	ed->map.trs_count = 0;

	i = 0;
	while (i < ed->map.circuits_count)
	{
		p = 0;
		while (p < ed->map.circuits[i].points_count)
		{
			ed->map.circuits[i].points[p].id = p;
			p++;
		}
		i++;
	}
	
	ft_memcpy(pts, ed->map.circuits[0].points, sizeof(t_pt_id) * (ed->map.circuits[0].points_count - 1));
	pts_count = ed->map.circuits[0].points_count - 1;

	if (ed->map.circuits[0].normal_dir == -1)
	{
		pts_rev(pts, pts_count);
	}


	int flag;
	flag = 0;
	while (flag == 0) {
		flag = 1;
		i = 1;
		while (i < ed->map.circuits_count)
		{
			if (ed->map.circuits[i].integrated == 0)
				if (integrate_hole(ed, i, pts, &pts_count) == 0)//////
					flag = 0;
			i++;
		}
	}
	
	//ed->map.circuits_count = 1;
	i = 0;
	// while (i < pts_count)
	// {
	// 	ed->map.circuits[0].points[i] = pts[i];
	// 	i++;
	// }
	// ed->map.circuits[0].points[i] = pts[0];
	// ed->map.circuits[0].points_count = pts_count + 1;


	printf("pts_count: %d\n", pts_count);
	i = 0;
	p = 0;
	while (pts_count > 3)
	{
		new1 = (t_vtx_id)
			{
				(t_vertex)
				{
					pts[i].pt.x,
					pts[i].pt.y,
					0.0
				},
				pts[i].id
			};
			
		new2 = (t_vtx_id)
			{
				(t_vertex)
				{
					pts[get_i_plus_2(i, pts_count)].pt.x,
					pts[get_i_plus_2(i, pts_count)].pt.y,
					0.0
				},
				pts[get_i_plus_2(i, pts_count)].id,
			};
		
			
		if (check_intersection(new1.vt, new2.vt, pts, pts_count) == -1 &&
						check_intersection_with_trs(new1, new2, ed) == -1 &&
						check_in_figure(pts, pts_count, i, ed) == 1)
		{
			create_tr(pts, ed, &pts_count, i);
		}
		else
		{
			i = get_i_plus_1(i, pts_count);
		//	puts("SHIT");
			if (p == 100000)
			{
				puts("GOVONO");
				ed->error = 1;

				i = 0;
				while (i < pts_count)
				{
					ed->map.circuits[0].points[i] = pts[i];
					ed->map.circuits_count = pts_count + 1;
					i++;
				}
				ed->map.circuits[0].points[i] = pts[0];

				
				return;
			}
			p++;
		}
		
	}
	create_tr(pts, ed, &pts_count, 0);
}

void		zeros(int f, char *ret)
{
	int i;
	int j;

	i = 0;
	while (f > 0)
	{
		f /= 10;
		i++;
	}
	j = 0;
	while (j < 5 - i)
	{
		ft_strcat(ret, "0");
		j++;
	}

}

char		*ft_ftoa(float n)
{
	char *tmp;
	char *ret;
	int i;
	int f;

	ret = (char *)malloc(64);//////////////////////////
	i = (int)n;
	f = (int)((n - (double)((int)n)) * 100000.0) * (n < 0.0 ? -1.0 : 1.0);

	tmp = ft_itoa(i);
	if (!ft_strchr(tmp, '-') && n < 0.0)
		ft_strcpy(tmp, "-0");
	ft_strcpy(ret, tmp);

	free(tmp);
	

	tmp = ft_itoa(f);
	ft_strcat(ret, ".");

	zeros(f, ret);


	ft_strcat(ret, tmp);
	free(tmp);
	
	return (ret);
}


void	draw_line(int *pixels, int x1, int y1, int x2, int y2, int color)
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
    //
	int error = deltaX - deltaY;
    //
	put_pixel(pixels, x2, y2, color);

	while(x1 != x2 || y1 != y2) 
	{
		put_pixel(pixels, x1, y1, color);
		const int error2 = error * 2;
        //
		if(error2 > -deltaY) 
		{
			error -= deltaY;
			x1 += signX;
		}
		if(error2 < deltaX) 
		{
			error += deltaX;
			y1 += signY;
		}
//		puts("lol");
	}


}

t_vertex	get_face_normal(t_map_editor *editor, int i, int j)
{
	t_vertex ret;

	t_point p1;
	t_point p2;

	p1 = editor->map.circuits[i].points[j - 1].pt;
	p2 = editor->map.circuits[i].points[j].pt;

	ret.x = cos(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);
				printf("cos = %f\n", ret.x);
	ret.y = 0.0;
	ret.z = sin(atan2(p2.y - p1.y, p2.x - p1.x) +
				(float)editor->map.circuits[i].normal_dir * M_PI / 2);
				printf("sin = %f\n", ret.z);



	return (ret);
}

void	save_map(t_map_editor *editor)
{
	int fd;
	int i;
	int j;

	char *ptr;

	fd = open("map_ed.obj", O_RDWR | O_TRUNC);

	i = 0;

	// ft_putendl_fd("v -4.0 -1.0 4.0", fd);
	// ft_putendl_fd("v -4.0 -1.0 -4.0", fd);///низ
	// ft_putendl_fd("v 4.0 -1.0 -4.0", fd);
	// ft_putendl_fd("v 4.0 -1.0 4.0", fd);

	// ft_putendl_fd("v -4.0 1.0 4.0", fd);
	// ft_putendl_fd("v -4.0 1.0 -4.0", fd);///верх 
	// ft_putendl_fd("v 4.0 1.0 -4.0", fd);
	// ft_putendl_fd("v 4.0 1.0 4.0", fd);

	// ft_putendl_fd("vt 0.0 0.0", fd);
	// ft_putendl_fd("vt 0.0 1.0", fd);
	// ft_putendl_fd("vt 1.0 1.0", fd);
	// ft_putendl_fd("vt 1.0 0.0", fd);

	// ft_putendl_fd("vn 0.0 1.0 0.0", fd);
	// ft_putendl_fd("vn 0.0 -1.0 0.0", fd);

	// ft_putendl_fd("f 1/1/1 2/2/1 3/3/1", fd);//низ
	// ft_putendl_fd("f 1/1/1 3/3/1 4/4/1", fd);

	// ft_putendl_fd("f 5/1/2 6/2/2 7/3/2", fd);//верх
	// ft_putendl_fd("f 5/1/2 7/3/2 8/4/2", fd);

	// editor->vertexes_count = 8;
	// editor->normals_count = 2;
	// editor->uvss_count = 4;

	i = 0;

	while (i < editor->map.trs_count)
	{
/////
		i++;
	}

	i = 0;

	ft_putendl_fd("", fd);

	while (i < editor->map.circuits_count)
	{
		j = 0;
		while (j < editor->map.circuits[i].points_count)
		{
			if (j == 0)
			{
				j++;
				continue ;
			}


			ft_putstr_fd("v ", fd);//////1
			ptr = ft_ftoa(editor->map.circuits[i].points[j - 1].pt.x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" 1.0 ", fd);
			ptr = ft_ftoa(editor->map.circuits[i].points[j - 1].pt.y);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n", fd);
			
			ft_putstr_fd("v ", fd);//////2
			ptr = ft_ftoa(editor->map.circuits[i].points[j - 1].pt.x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" -1.0 ", fd);
			ptr = ft_ftoa(editor->map.circuits[i].points[j - 1].pt.y);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n", fd);

			ft_putstr_fd("v ", fd);//////3
			ptr = ft_ftoa(editor->map.circuits[i].points[j].pt.x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" -1.0 ", fd);
			ptr = ft_ftoa(editor->map.circuits[i].points[j].pt.y);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n", fd);

			ft_putstr_fd("v ", fd);//////4
			ptr = ft_ftoa(editor->map.circuits[i].points[j].pt.x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" 1.0 ", fd);
			ptr = ft_ftoa(editor->map.circuits[i].points[j].pt.y);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n\n", fd);



			ft_putstr_fd("vn ", fd);////normal

			t_vertex normal = get_face_normal(editor, i, j);
			ptr = ft_ftoa(normal.x);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd(" 0.0 ", fd);
			ptr = ft_ftoa(normal.z);
			ft_putstr_fd(ptr, fd);
			free(ptr);
			ft_putstr_fd("\n\n", fd);

			editor->vertexes_count += 4;
			editor->normals_count += 1;


			ft_putstr_fd("f ", fd);///face1
			ft_putnbr_fd(editor->vertexes_count - 3, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(1, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(editor->vertexes_count - 2, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(2, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(editor->vertexes_count - 1, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(3, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd("\n", fd);



			ft_putstr_fd("f ", fd);////face2
			ft_putnbr_fd(editor->vertexes_count - 3, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(1, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(editor->vertexes_count - 1, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(3, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(editor->vertexes_count, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(4, fd);
			ft_putstr_fd("/", fd);
			ft_putnbr_fd(editor->normals_count, fd);
			ft_putstr_fd("\n\n\n", fd);







			j++;
		}
		
		i++;
	}

	close(fd);
}
void	map_new_circuit(t_map *map)
{
	map->circuits[map->circuits_count].points_count = 0;
	map->circuits[map->circuits_count].normal_dir = 1;
	map->circuits[map->circuits_count].integrated = 0;

	(map->circuits_count)++;
}
void	map_new_point(t_map *map, float x, float y)
{
	int index;
	if (!map->active)
	{
		map_new_circuit(map);
		map->active = 1;
	}

	index = map->circuits[map->circuits_count - 1].points_count;
	map->circuits[map->circuits_count - 1].points[index].pt.x = x;
	map->circuits[map->circuits_count - 1].points[index].pt.y = y;
	map->circuits[map->circuits_count - 1].points[index].c_id = map->circuits_count - 1;


	map->circuits[map->circuits_count - 1].points_count++;
}

void	event_handle(SDL_Event *event, void *map_ptr, int *quit)
{
    t_map_editor *map;

    map = (t_map_editor *)map_ptr;

    if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		map->prev_x = event->button.x;
		map->prev_y = event->button.y;

		map_new_point(&(map->map), (float)(map->prev_x - W_2 / 2) / 100.0,
							(float)(H_2 / 2 - map->prev_y) / 100.0);//////////////////////поменят для норм экрана
        printf("x: %f\ty: %f\n", (float)(map->prev_x - W_2 / 2) / 100.0, (float)(H_2 / 2 - map->prev_y) / 100.0);
	}
	else if (event->type == SDL_KEYDOWN )
	{
		if (event->key.keysym.sym == SDLK_s)
		{
			save_map(map);
		}
		else if (event->key.keysym.sym == SDLK_e)
		{
			map_new_point(&map->map, map->map.circuits[map->map.circuits_count - 1].points[0].pt.x,
									map->map.circuits[map->map.circuits_count - 1].points[0].pt.y);
			map->map.active = 0;
		}
		else if (event->key.keysym.sym == SDLK_n)
		{
			if (map->map.active)
			{
				map->map.circuits[map->map.circuits_count - 1].normal_dir *= -1;
			}
		}
		else if (event->key.keysym.sym == SDLK_t)
		{
			if (!map->map.active)
			{
				if (map->mode == LINES)
				{
					triangulate(map);
					map->mode = TRS;
				}
				else
				{
					map->mode = LINES;
					int i = 0;
					while (i < map->map.circuits_count) {
						map->map.circuits[i].integrated = 0;
						i++;
					}
				}
				
			}
		}
		// else if (event->key.keysym.sym == SDLK_p)
		// {
		// 	map->curr_tr++;
		// 	if (map->curr_tr == map->map.trs_count)
		// 		map->curr_tr = -1;
		// }
		else if (event->key.keysym.sym == SDLK_c)
		{
			map->error = 0;
			map->curr_tr = -1;
			map->mode = LINES;
			map->map.active = 0;
			map->map.circuits_count = 0;
			map->map.trs_count = 0;
			map->map.trs_ids_count = 0;
			map->map.trs_ids_count = 0;
			map->map.vrts_count = 0;

		}
	}
		
}

void	update(void *map_editor, int *pixels)
{
	t_map_editor *ed;
	int i;
	int j;

	t_int_v p1;
	t_int_v p2;


	ed = (t_map_editor *)map_editor;
	
	bzero(pixels, sizeof(int) * HxW);

	i = 0;

	if (ed->mode == TRS)
	{
		while (i < ed->map.trs_ids_count)
		{//////переписать
			t_point pt_1;
			t_point pt_2;
			t_point pt_3;


			pt_1 = (t_point)
				{
					ed->map.vrts[ed->map.trs_ids[i].ids[0]].vt.x,
					ed->map.vrts[ed->map.trs_ids[i].ids[0]].vt.y,
				};
			pt_2 = (t_point)
				{
					ed->map.vrts[ed->map.trs_ids[i].ids[1]].vt.x,
					ed->map.vrts[ed->map.trs_ids[i].ids[1]].vt.y,
				};
			pt_3 = (t_point)
				{
					ed->map.vrts[ed->map.trs_ids[i].ids[2]].vt.x,
					ed->map.vrts[ed->map.trs_ids[i].ids[2]].vt.y,
				};
			draw_line(pixels, (int)(pt_1.x * 200), (int)(pt_1.y * 200),
							(int)(pt_2.x * 200), (int)(pt_2.y * 200), 0xFFFF00);
			draw_line(pixels, (int)(pt_2.x * 200), (int)(pt_2.y * 200),
							(int)(pt_3.x * 200), (int)(pt_3.y * 200), 0xFFFF00);
			draw_line(pixels, (int)(pt_3.x * 200), (int)(pt_3.y * 200),
							(int)(pt_1.x * 200), (int)(pt_1.y * 200), 0xFFFF00);
			i++;
		}
		
		if (ed->error)
		{
			j = 0;
			while (j < ed->map.circuits[0].points_count)
			{
				if (j == 0)
				{
					j++;
					continue ;
				}
					
				p1.x = (int)(ed->map.circuits[0].points[j - 1].pt.x * 200);///справтить для норм экрана
				p1.y = (int)(ed->map.circuits[0].points[j - 1].pt.y * 200);

				p2.x = (int)(ed->map.circuits[0].points[j].pt.x * 200);
				p2.y = (int)(ed->map.circuits[0].points[j].pt.y * 200);


				draw_line(pixels, p1.x, p1.y, p2.x, p2.y, 0x00FF00);

				/*draw_line(pixels, p1.x + (p2.x - p1.x) / 2, p1.y + (p2.y - p1.y) / 2,
					p1.x + (p2.x - p1.x) / 2 + 30 * cos(atan2(p2.y - p1.y, p2.x - p1.x) +
												(float)ed->map.circuits[0].normal_dir * M_PI / 2),
					p1.y + (p2.y - p1.y) / 2 + 30 * sin(atan2(p2.y - p1.y, p2.x - p1.x) +
												(float)ed->map.circuits[0].normal_dir * M_PI / 2),
					0xff0000);
*/
				//printf("angle: %f\n", atan2(p2.x - p1.x, p2.y - p1.y) / M_PI);
				j++;
			}
		}
		if (ed->curr_tr != -1)
		{
			j = 0;
			// while (j < ed->curr_tr)
			// {
			// 	draw_line(pixels, (int)(ed->map.trs[j].points[0].pt.x * 200), (int)(ed->map.trs[j].points[0].pt.y * 200),
			// 				(int)(ed->map.trs[j].points[1].pt.x * 200), (int)(ed->map.trs[j].points[1].pt.y * 200), 0x0000ff);
			// 	draw_line(pixels, (int)(ed->map.trs[j].points[1].x * 200), (int)(ed->map.trs[j].points[1].pt.y * 200),
			// 				(int)(ed->map.trs[j].points[2].pt.x * 200), (int)(ed->map.trs[j].points[2].pt.y * 200), 0x0000ff);
			// 	draw_line(pixels, (int)(ed->map.trs[j].points[2].x * 200), (int)(ed->map.trs[j].points[2].pt.y * 200),
			// 				(int)(ed->map.trs[j].points[0].pt.x * 200), (int)(ed->map.trs[j].points[0].pt.y * 200), 0x0000ff);
			// 	j++;
			// }
			// draw_line(pixels, (int)(ed->map.trs[ed->curr_tr].points[0].pt.x * 200), (int)(ed->map.trs[ed->curr_tr].points[0].pt.y * 200),
			// 				(int)(ed->map.trs[ed->curr_tr].points[1].pt.x * 200), (int)(ed->map.trs[ed->curr_tr].points[1].pt.y * 200), 0xff0000);
			// draw_line(pixels, (int)(ed->map.trs[ed->curr_tr].points[1].pt.x * 200), (int)(ed->map.trs[ed->curr_tr].points[1].pt.y * 200),
			// 				(int)(ed->map.trs[ed->curr_tr].points[2].pt.x * 200), (int)(ed->map.trs[ed->curr_tr].points[2].pt.y * 200), 0xff0000);
			// draw_line(pixels, (int)(ed->map.trs[ed->curr_tr].points[2].pt.x * 200), (int)(ed->map.trs[ed->curr_tr].points[2].pt.y * 200),
			// 				(int)(ed->map.trs[ed->curr_tr].points[0].pt.x * 200), (int)(ed->map.trs[ed->curr_tr].points[0].pt.y * 200), 0xff0000);
		}
		return ;
	}


	while (i < ed->map.circuits_count)
	{
		j = 0;
		while (j < ed->map.circuits[i].points_count)
		{
			if (j == 0)
			{
				j++;
				continue ;
			}
				
			p1.x = (int)(ed->map.circuits[i].points[j - 1].pt.x * 200);///справтить для норм экрана
			p1.y = (int)(ed->map.circuits[i].points[j - 1].pt.y * 200);

			p2.x = (int)(ed->map.circuits[i].points[j].pt.x * 200);
			p2.y = (int)(ed->map.circuits[i].points[j].pt.y * 200);


			draw_line(pixels, p1.x, p1.y, p2.x, p2.y, 0xFFFFFF);

			draw_line(pixels, p1.x + (p2.x - p1.x) / 2, p1.y + (p2.y - p1.y) / 2,
				p1.x + (p2.x - p1.x) / 2 + 30 * cos(atan2(p2.y - p1.y, p2.x - p1.x) +
											(float)ed->map.circuits[i].normal_dir * M_PI / 2),
				p1.y + (p2.y - p1.y) / 2 + 30 * sin(atan2(p2.y - p1.y, p2.x - p1.x) +
											(float)ed->map.circuits[i].normal_dir * M_PI / 2),
				0xff0000);

			//printf("angle: %f\n", atan2(p2.x - p1.x, p2.y - p1.y) / M_PI);
			j++;
		}
		i++;
	}
}


void	map_init(t_map *map)
{
	map->circuits_count = 0;
	map->active = 0;
	map->trs_ids_count = 0;
	map->vrts_count = 0;
	
}

int main(int ac, char **av)
{
	
	t_map_editor map_editor;

	map_editor.normals_count = 0;
	map_editor.uvss_count = 0;
	map_editor.vertexes_count = 0;
	map_editor.mode = LINES;
	map_editor.error = 0;
	map_editor.curr_tr = -1;


	map_editor.map.trs_ids_count = 0;
	map_editor.map.vrts_count = 0;

	t_mgl *mgl = mgl_init("Map Editor", W, H);


	mgl_run(mgl, update, event_handle, &map_editor);

	mgl_quit(mgl);
	return (0);

}