#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "my_graphics_lib.h"
#include "duke.h"

/*
	контур. предстваляет из себя замкнутый многоугольник.
	первая и последняя точки одинаковы
	нормаль контура опрделяется по направлению обхода точек при рисовании
	по часовой стрелке - нормаль наружу, против - внутрь
	normal_dir  по умолчанию = 1
	если установить -1 (клавиша n в редакторе), то нормаль инвертируется
*/
typedef struct s_circuit
{
	t_point points[100];
	int points_count;
	int normal_dir;

} t_circuit;

/*
	первый контур в массиве контуров - основной (контур помещения)
	последующие - "дырки" (колонны и т д)
	поэтому у первого контура нормаль должна смотреть внутрь
	а у остальных наружу
*/

typedef struct s_map
{
	t_circuit circuits[10];
	int active;					// режим рисования контура
	int circuits_count;
} t_map;

typedef struct s_map_editor
{
	int prev_x;
	int prev_y;

	t_map map;
} t_map_editor;

typedef struct s_wall
{
	t_vertex points[2];
	t_vertex normal;
} t_wall;

/* вот такая структура для bsp-узла */

typedef struct s_bsp
{
	int is_leaf;			// является ли листом (то есть конечным узлом, 
							// в котором записаны все стены и у которого нет детей)

	t_vertex line;			// (только для не листовых узлов) содержит уравнение прямой,
							// по которой происходит деление оставшегося пространства

	t_vertex normal;		// (только для не листовых узлов)
							// нормаль линии разреза (соответствует нормали выбранной cutter-стены)

	t_wall walls[100];		// (только для листов) массив стен в листе

	int walls_count;

	struct s_bsp *front;	// (только для не листовых узлов)

	struct s_bsp *back;		// (только для не листовых узлов)
} t_bsp;

#endif
