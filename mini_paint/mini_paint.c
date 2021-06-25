#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

typedef struct drawing drawing, *Pdrawing;

struct drawing
{
	int width;
	int height;
	char *matrice;
};

typedef struct circle circle, *Pcircle;

struct circle
{
	char type;
	float x;
	float y;
	float radius;
	char color;
};

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int print_info(drawing *draw)
{
	int i;

	i = 0;
	while (i < draw->height)
	{
		printf("%.*s\n", draw->width, draw->matrice + i * draw->width);
		i = i + 1;
	}
	return i;
}

float square(float a)
{
	return (a * a);
}

float sqrt_dist(float x1, float y1, float x2, float y2)
{
	float dist_x;
	float dist_y;

	dist_x = square(x1 - x2);
	dist_y = square(y1 - y2);
	return (sqrt(dist_x + dist_y));
}

int is_in_circle(float x, float y, circle *circle)
{
	float dist;
	float dist_sqrt;

	dist_sqrt = sqrt_dist(x, y, circle->x, circle->y);
	dist = dist_sqrt - circle->radius;
	if (dist <= 0.00000000)
	{
		if (dist <= -1.00000000)
			return (1); // Inside
		return (2);		// Border
	}
	return (0);
}

void execute_one(circle *circle, drawing *draw, int x, int y)
{
	int is_in;

	is_in = is_in_circle((float)x, (float)y, circle);
	if ((is_in == 2) || ((is_in == 1 && (circle->type == 'C'))))
		draw->matrice[x + y * draw->width] = circle->color;
}

int apply_op(circle *circle, drawing *draw)
{
	int j;
	int i;

	if ((circle->radius <= 0.00000000) || ((circle->type != 'C' && (circle->type != 'c'))))
		return (1);
	i = 0;
	while (i < draw->width)
	{
		j = 0;
		while (j < draw->height)
			execute_one(circle, draw, i, j++);
		i++;
	}
	return (0);
}

int get_info(FILE *file, drawing *draw)
{
	int scan_ret;
	int i;
	char background;

	scan_ret = fscanf(file, "%d %d %c\n", &draw->width, &draw->height, &background);
	if (scan_ret == 3)
	{
		if ((((draw->width < 1) || (300 < draw->width)) || (draw->height < 1)) || (300 < draw->height))
			return (1);
		draw->matrice = (char *)malloc(draw->width * draw->height);
		if (!draw->matrice)
			return (1);
		i = 0;
		while (i < draw->width * draw->height)
			draw->matrice[i++] = background;
		return (0);
	}
	return (1);
}

int execute(FILE *file)
{
	int scan_ret;
	circle circle;
	drawing draw;

	if (!get_info(file, &draw))
	{
		scan_ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color);
		while (scan_ret == 5)
		{
			if (apply_op(&circle, &draw))
				return (1);
			scan_ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color);
		}
		if (scan_ret == -1)
		{
			print_info(&draw);
			return (0);
		}
		return (1);
	}
	return (1);
}

int main(int argc, char **argv)
{
	int i;
	FILE *file;

	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (file && !execute(file))
			return (0);
		i = ft_strlen("Error: Operation file corrupted\n");
		write(1, "Error: Operation file corrupted\n", i);
	}
	else
	{
		i = ft_strlen("Error: argument\n");
		write(1, "Error: argument\n", i);
	}
	return (1);
}
