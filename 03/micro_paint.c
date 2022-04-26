#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BAD_ARGS "Error: argument"
#define BAD_FILE "Error: Operation file corrupted"

typedef struct s_canvas{
	int		width;
	int		height;
	char	background;
	char	*image;
}			t_canvas;

typedef struct s_rectangle{
	char	symb;
	float	x;
	float	y;
	float	widht;
	float	height;
	char	color;
}			t_rectangle;

int ft_strlen(char *str){
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void print_error(char *msg){
	char endl;

	endl = '\n';
	write(STDOUT_FILENO, msg, ft_strlen(msg));
	write(STDOUT_FILENO, &endl, 1);
}

void print_canvas(t_canvas *canvas){
	int		x;
	int		y;
	char	endl;

	endl = '\n';
	y = 0;
	while (y < canvas->height){
		x = 0;
		while (x < canvas->width){
			write(STDOUT_FILENO, &canvas->image[y * canvas->width] + x, 1);
			x++;
		}
		write(STDOUT_FILENO, &endl, 1);
		y++;
	}
}

FILE	*open_file(char *path){
	FILE *file;

	file = fopen(path, "r");
	if (!file){
		return (NULL);
	}
	return (file);
}

int	get_canvas(FILE *file, t_canvas **orig_canvas){
	int			count;
	t_canvas	*canvas;

	canvas = (t_canvas *)malloc(sizeof(t_canvas));
	if (!canvas){
		return (2);
	}
	*orig_canvas = canvas;
	count = fscanf(file, "%d %d %c\n", &canvas->width, &canvas->height, &canvas->background);
	if (canvas->width <= 0 || canvas->width > 300 || canvas->height <= 0 || canvas->height > 300 || count != 3){
		free(canvas);
		return (1);
	}
	canvas->image = (char *)malloc(canvas->width * canvas->height);
	memset(canvas->image, canvas->background, canvas->width * canvas->height);
	return (0);
}

int print_rectangles(FILE *file, t_canvas *canvas){
	int			x;
	int			y;
	int			count;
	t_rectangle	*rectangle;

	while (1){
		rectangle = (t_rectangle *)malloc(sizeof(t_rectangle));
		if (!rectangle){
			return (2);
		}

		count = fscanf(file, "%c %f %f %f %f %c\n", &rectangle->symb, &rectangle->x, &rectangle->y, &rectangle->widht, &rectangle->height, &rectangle->color);
		if (count <= 0){
			free(rectangle);
			return (0);
		}
		if (count != 6 || (rectangle->symb != 'r' && rectangle->symb != 'R') || rectangle->widht <= 0 || rectangle->height <= 0){
			free(rectangle);
			return (1);
		}

		y = 0;
		while (y < canvas->height){
			x = 0;
			while (x < canvas->width){
				if (x <= rectangle->x + rectangle->widht && x >= rectangle->x
						&& y <= rectangle->y + rectangle->height && y >= rectangle->y){				
					if (rectangle->symb == 'R'){
						canvas->image[y * canvas->width + x] = rectangle->color;
					} else if (rectangle->symb == 'r'){
						if (((sqrtf(powf(x - rectangle->x, 2)) < 1.0 || sqrtf(powf(x - rectangle->x - rectangle->widht, 2)) < 1.0)
							&& y <= rectangle->y + rectangle->height && y >= rectangle->y)
							|| ((sqrtf(powf(y - rectangle->y, 2)) < 1.0 || sqrtf(powf(y - rectangle->y - rectangle->height, 2)) < 1.0)
							&& x <= rectangle->x + rectangle->widht && x >= rectangle->x)){
								canvas->image[y * canvas->width + x] = rectangle->color;
						}
					}
				}
				x++;
			}
			y++;
		}

		free(rectangle);
	}
	return (0);
}

int main(int argc, char **argv){
	int			ret_val;
	FILE		*file;
	t_canvas	*canvas;
	
	if (argc != 2){
		print_error(BAD_ARGS);
		return (1);
	}

	file = open_file(argv[1]);
	if (!file){
		print_error(BAD_FILE);
		return (1);
	}

	ret_val = get_canvas(file, &canvas);
	if (ret_val == 1){
		print_error(BAD_FILE);
		fclose(file);
		return (1);
	}

	ret_val = print_rectangles(file, canvas);
	if (!ret_val){
		print_canvas(canvas);
	}
	free(canvas->image);
	free(canvas);
	fclose(file);
	if (ret_val == 1){
		print_error(BAD_FILE);
		return (1);
	} else if (ret_val == 2){
		return (1);
	}
	return (0);
}