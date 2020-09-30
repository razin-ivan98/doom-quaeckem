
# include <stdio.h>
# include <math.h>

typedef struct	s_vertex
{
	float x;
	float y;
	float z;
}				t_vertex;


t_vertex	sub(t_vertex v1, t_vertex v2)
{
	return ((t_vertex){
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	});
}
t_vertex	cross(t_vertex v1, t_vertex v2)
{
	return((t_vertex){
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	});
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

	if(sign(prod1.z) == sign(prod2.z) || sign(prod1.z) == 0 || sign(prod2.z) == 0) // Отсекаем также и пограничные случаи
		return 0;
	

	prod1 = cross(cut2, sub(v11, v21));
	prod2 = cross(cut2, sub(v12, v21));

	if(sign(prod1.z) == sign(prod2.z) || sign(prod1.z) == 0 || sign(prod2.z) == 0) // Отсекаем также и пограничные случаи
		return 0;

	// if (crossing) { // Проверяем, надо ли определять место пересечения
	// 	(*crossing)[X] = v11[X] + cut1[X]*fabs(prod1[Z])/fabs(prod2[Z]-prod1[Z]);
	// 	(*crossing)[Y] = v11[Y] + cut1[Y]*fabs(prod1[Z])/fabs(prod2[Z]-prod1[Z]);
	// }

	return 1;

}


int main()
{
	t_vertex new1, new2, v1, v2;

	new1 = (t_vertex){-1.97, 2.25, 0.0};
	new2 = (t_vertex){-0.84, -1.33, 0.0};
	v2 = (t_vertex){-1.11, 1.52, 0.0};
	v1 = (t_vertex){1.44, 1.32, 0.0};

	if (if_intersect(new1, new2, v1, v2))
		puts("ŸES");
	else
	{
			puts("NO");
	}
	

}