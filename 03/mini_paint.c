#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BAD_ARGS "Error: argument"
#define BAD_FILE "Error: Operation file corrupted"

typedef struct s_canvas{
	int		width;
	int		height;
	char	background;
	char	*image;
}			t_canvas;

typedef struct s_circle{
	char	symb;
	float	x;
	float	y;
	float	r;
	char	color;
}			t_circle;

int ft_strlen(char *str){
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}

void print_error(char *msg){
	char endl = '\n';

	write(STDOUT_FILENO, msg, ft_strlen(msg));
	write(STDOUT_FILENO, &endl, 1);
}

FILE *open_file(char *path){
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
	canvas->height = -1;
	canvas->width = -1;
	count = fscanf(file, "%d %d %c\n", &canvas->width, &canvas->height, &canvas->background);
	if (canvas->width <= 0 || canvas->width > 300 || canvas->height <= 0 || canvas->height > 300 || count != 3){
		free(canvas);
		return (1);
	}
	canvas->image = (char *)malloc(sizeof(char) * canvas->width * canvas->height);
	if (!canvas->image){
		free(canvas);
		return (2);
	}
	memset(canvas->image, canvas->background, canvas->width * canvas->height);
	return (0);
}

int	print_circles(FILE *file, t_canvas *canvas){
	int			x;
	int			y;
	int 		count;
	float		dist;
	t_circle	*circle;

	while (1){
		circle = (t_circle *)malloc(sizeof(t_circle));
		if (!circle){
			return (2);
		}
		circle->symb = 'x';
		circle->r = 0.0;
		count = fscanf(file, "%c %f %f %f %c\n", &circle->symb, &circle->x, &circle->y, &circle->r, &circle->color);
		if (count <= 0){
			free(circle);
			return (0);
		}
		if (circle->r == 0.0 || (circle->symb != 'c' && circle->symb != 'C') || count != 5){
			free(circle);
			return (1);
		}

		y = 0;
		while (y < canvas->height){
			x = 0;
			while (x < canvas->width){
				if (sqrtf(powf(x - circle->x, 2) + powf(y - circle->y, 2))){
					if (circle->symb == 'C'){
						canvas->image[y * canvas->width + x] = circle->color;
					} else if (circle->symb == 'c'){
						dist = sqrtf(powf(x - circle->x, 2) + powf(y - circle->y, 2)) - circle->r;
						if (dist < 1.0){
							canvas->image[y * canvas->width + x] = circle->color;
						}
					}
				}
				x++;
			}
			y++;
		}

		free(circle);
	}
	return (0);
}

void print_canvas(t_canvas *canvas){
	int	x;
	int	y;
	char endl;

	endl = '\n';
	y = 0;
	while (y < canvas->height){
		x = 0;
		while (x < canvas->width){
			write(STDOUT_FILENO, &canvas->image[y * canvas->width + x], 1);
			x++;
		}
		write(STDOUT_FILENO, &endl, 1);
		y++;
	}
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
		fclose(file);
		print_error(BAD_FILE);
		return (1);
	} else if (ret_val == 2){
		return (1);
	}

	ret_val = print_circles(file, canvas);
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
}