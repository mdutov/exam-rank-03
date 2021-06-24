#include "micro_paint.h"

int ft_strlen(char *s)
{
    int i;

    i = 0;
    while(s[i])
        i++;
    return(i);
}

void print_info(drawing *draw)
{
    int j;

    j = 0;
    while(j < draw->height)
    {
        write(1, draw->matrice + j * draw->width, draw->width);
        write(1, "\n", 1);
        j++;
    }
}

void clear_all(FILE *file, drawing *draw)
{
    fclose(file);
    if (draw->matrice)
        free(draw->matrice);
}

int is_in_rect(rectangle *rect, float x, float y)
{
    if (x < rect->x || rect->x + rect->width < x || y < rect->y || rect->y + rect->height < y)
        return (0);
    if (x - rect->x < 1.0 || rect->x + rect->width - x < 1.0 || y - rect->y < 1.0 || rect->y + rect->height - y < 1.0)
        return(2);
    return(1);
}

void execute_one(drawing *draw, rectangle *rect, int x, int y)
{
    int is_in;

    is_in = is_in_rect(rect, (float)x, (float)y);
    if (is_in == 2 || (is_in == 1 && (rect->type == 'R')))
        draw->matrice[x + y * draw->width] = rect->color;
}

int apply_op(drawing *draw, rectangle *rect)
{
    int x, y; 

    x = 0;
    y = 0;
    if(rect->width <= 0.0 || rect->height <= 0.0 || (rect->type != 'R' && rect->type != 'r'))
        return(1);
    while (y < draw->height)
    {
        x = 0;
        while(x < draw->width)
            execute_one(draw, rect, x++, y);
        y++;
    }
    return(0);
}

int get_info(FILE *file, drawing *draw)
{
    char background;
    int i;

    i = 0;
    if(fscanf(file, "%d %d %c\n", &draw->width, &draw->height, &background))
    {
        if(draw->width < 1 || draw->width > 300 || draw->height < 1 || draw->height > 300)
            return(1);
        draw->matrice = (char *)malloc(sizeof(char) * draw->width * draw->height);
        if (draw->matrice)
            while(i < draw->height * draw->width)
                draw->matrice[i++] = background;
        return(0);
    }
    return(1);
}

int execute(FILE *file)
{
   drawing draw;
   rectangle rect;
   int scan_res;

   if(!get_info(file, &draw))
   {
       scan_res = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
       while(scan_res == 6)
       {
           if (apply_op(&draw, &rect))
                return(1);
            scan_res = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
       }
       if (scan_res == -1)
       {
           print_info(&draw);
           clear_all(file, &draw);
           return(0);
       }
   }
    return(1);
}

int main(int argc, char **argv)
{
    (void)argv;
    FILE *file;

    if(argc == 2)
    {
        file = fopen(argv[1],"r");
        if (file && !execute(file))
                return(0);
        write(1, "Error: Operation file corrupted\n", ft_strlen("Error: Operation file corrupted\n"));
        return(1);
    }
    write(1, "Error: argument\n", ft_strlen("Error: argument\n"));
    return(1);
}