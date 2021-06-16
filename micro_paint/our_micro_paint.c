#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct drawing drawing, *Pdrawing;

struct drawing
{
	int width;
	int height;
	char *matrice;
};

typedef struct rectangle rectangle, *Prectangle;

struct rectangle
{
	char type;
	float x;
	float y;
	float width;
	float height;
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

void print_info(drawing *drawing)
{
	int i;

	i = 0;
	while (i < drawing->height)
		printf("%.*s\n", drawing->width, drawing->matrice + i++ * drawing->width);
}

int is_in_rect(rectangle *rect, float x, float y)
{
	if (x < rect->x || rect->x + rect->width < x || y < rect->y || rect->y + rect->height < y)
		return(0);
	if (x - rect->x < 1.0 || rect->x + rect->width - x < 1.0 || y - rect->y < 1.0  || rect->y + rect->height - y < 1.0)
		return(2);
	return (1);
}

void execute_one(rectangle *rect, drawing *drawing, int x, int y)
{
	int is_in;

	is_in = is_in_rect(rect, (float)x, (float)y);
	if(is_in == 2 || (is_in == 1 && rect->type == 'R'))
		drawing->matrice[x + y * drawing->width] = rect->color;
}

int apply_op(rectangle *rect, drawing *drawing)
{
	int j;
	int i;

	if (rect->width <= 0.0 || rect->height <= 0.0 || (rect->type != 'R' && rect->type != 'r'))
		return(1);
	j = 0;
	while(j < drawing->height)
	{
		i = 0;
		while(i < drawing->width)
			execute_one(rect, drawing, i++, j);
		j++;
	}
	return (0);
}

int get_info(FILE *file, drawing *drawing)
{
	int i;
	char background;

	if(fscanf(file, "%d %d %c\n", &drawing->width, &drawing->height, &background) == 3)
	{
		if ((drawing->width < 1) || (drawing->width > 300) || (drawing->height < 1) || (drawing->height > 300))
			return(1);
		drawing->matrice = (char *)malloc(sizeof(char) * drawing->width * drawing->height);
		if (!drawing->matrice)
			return(1);
		i = 0;
		while (i < (drawing->width * drawing->height))
			drawing->matrice[i++] = background;
		return(0);
	}
	return(1);
}

int execute(FILE *file)
{
	int scan_res;
	rectangle rect;
	drawing drawing;

	if(!get_info(file, &drawing))
	{
		scan_res = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
		while (scan_res == 6)
		{
			if(apply_op(&rect, &drawing))
				return(1);
			scan_res = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
		}
		if (scan_res == -1)
		{
			print_info(&drawing);
			return(0);
		}
	}
	return(1);
}

int main(int argc, char **argv)
{
	int i;
	FILE *file;

	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (file && !execute(file))
			return(0);
		i = ft_strlen("Error: Operation file corrupted\n");
		write(1, "Error: Operation file corrupted\n", i);
	}
	else
	{
		i = ft_strlen("Error: argument\n");
		write(1, "Error: argument\n", i);
	}
	return(1);
}
