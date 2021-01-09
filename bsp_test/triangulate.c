#include "bsp.h"

void    trangulate(t_bsp *node)
{
	int i;
	t_vertex base;
	t_vertex f;
	t_vertex s;

	base = node->walls[0].points[0];

	t_tr *trs = malloc(sizeof(t_tr) * 100);
	int trs_count = 0;

	i = 0;
	while (i < node->walls_count - 1)
	{
		f = node->walls[i].points[1];
		s = node->walls[i + 1].points[1];

		if (!())
		trs[trs_count] = create_tr(base, f, s);
		trs_count++;
		i++;
	}

}
t_tr create_tr(t_vertex v1, t_vertex v2, t_vertex v3)
{
	t_tr res;

	res.points[0] = v1;
	res.points[1] = v2;
	res.points[2] = v3;
}
void triangulate(t_map_editor *ed)
{
	t_point pts[100];
	int pts_count;
	int i;

	int p;

	t_vertex new1;
	t_vertex new2;


	i = 0;
	p = 0;
	while (pts_count > 3)
	{
		new1 = (t_vertex){
			pts[i].x,
			pts[i].y,
			0.0};
		new2 = (t_vertex){
			pts[get_i_plus_2(i, pts_count)].x,
			pts[get_i_plus_2(i, pts_count)].y,
			0.0};
		if (check_intersection(new1, new2, pts, pts_count) == -1 &&
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