#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct t_executable Executable;
typedef enum e_type Type;

enum e_type {
	TYPE_PIPE,
	TYPE_SEMICOLON,
	TYPE_NONE
};

struct t_executable {
	char		**argv;
	int			io_buffer[2];
	Type		type;
	Executable	*next;
	Executable	*prev;
};

size_t str_len(char *str){
	if (!str)
		return (0);
	
	size_t size = 0;
	while (*str){
		size++;
		str++;
	}
	return (size);
}

size_t arr_len(char **arr){
	if (!arr){
		return (0);
	}

	size_t size = 0;
	while (*arr){
		size++;
		arr++;
	}
	return (size);
}

int cmp_strings(char *s1, char *s2){
	if (!s1 || !s2){
		return (0);
	}

	size_t s1_len = str_len(s1);
	size_t s2_len = str_len(s2);
	if (s1_len != s2_len){
		return (0);
	}

	size_t i = 0;
	while (i < s1_len){
		if (s1[i] != s2[i]){
			return (0);
		}
		i++;
	}
	return (1);
}

void destroy_list(Executable *list){
	if (!list){
		return;
	}
	
	Executable *next;
	while (list){
		next = list->next;
		free(list->argv);
		close(list->io_buffer[STDIN_FILENO]);
		close(list->io_buffer[STDOUT_FILENO]);
		free(list);
		list = next;
	}
}

void SYSTEM_CALL_ERROR(){
	write(STDERR_FILENO, "error: fatal\n", 13);
	exit(1);
}

void EXECVE_ERROR(char *path){
	write(STDERR_FILENO, "error: cannot execute ", 22);
	write(STDERR_FILENO, path, str_len(path));
	write(STDERR_FILENO, "\n", 1);
}

void executable_push_back(Executable **list, Executable *elem){
	if (!elem){
		return;
	}

	if (!*list){
		*list = elem;
		return;
	}
	Executable *list_ptr = *list;
	while (list_ptr->next){
		list_ptr = list_ptr->next;
	}
	elem->prev = list_ptr;
	list_ptr->next = elem;
}

Executable *get_executable(char **argv, int begin, int end, Type type){
	Executable *executable = (Executable *)malloc(sizeof(Executable));
	if (!executable){
		return (NULL);
	}

	executable->argv = (char **)malloc(sizeof(char *) * (end - begin + 1));
	if (!executable->argv){
		free(executable);
		return (NULL);
	}

	int iter = 0;
	while (begin != end){
		executable->argv[iter++] = argv[begin++];
	}
	executable->argv[iter] = NULL;
	executable->type = type;
	executable->next = NULL;
	executable->prev = NULL;

	return (executable);
}

Executable *parse_input(int argc, char **argv){
	(void)argc;

	int begin = 1, end = 1;
	Type type = TYPE_NONE;
	Executable *elem = NULL;
	Executable *list = NULL;

	while (1){
		if (!argv[end] || cmp_strings(argv[end], ";")){
			type = TYPE_SEMICOLON;
		} else if (cmp_strings(argv[end], "|")){
			type = TYPE_PIPE;
		} else {
			type = TYPE_NONE;
		}

		if (type != TYPE_NONE){
			if (begin != end){
				elem = get_executable(argv, begin, end, type);
				if (!elem){
					destroy_list(list);
					SYSTEM_CALL_ERROR();
				}
				executable_push_back(&list, elem);
			}
			if (!argv[end]){
				break;
			}
			begin = end + 1;
		}
		end++;
	}

	return (list);
}

void execute_list(Executable *list, char **envp){
	if (!list){
		return;
	}

	Executable *list_ptr = list;
	while (list_ptr){
		if (cmp_strings(list_ptr->argv[0], "cd")){
			if (arr_len(list_ptr->argv) != 2){
				write(STDERR_FILENO, "error: cd: bad arguments\n", 25);
			} else if (chdir(list_ptr->argv[1])){
				write(STDERR_FILENO, "error: cd: cannot change directory to ", 38);
				write(STDERR_FILENO, list_ptr->argv[1], str_len(list_ptr->argv[1]));
				write(STDERR_FILENO, "\n", 1);
			}
		} else {
			if (list_ptr->type == TYPE_PIPE && pipe(list_ptr->io_buffer)){
				destroy_list(list);
				SYSTEM_CALL_ERROR();
			}

			pid_t pid = fork();
			if (pid < 0){
				destroy_list(list);
				SYSTEM_CALL_ERROR();
			}

			if (pid == 0){
				if (list_ptr->prev && list_ptr->prev->type == TYPE_PIPE){
					dup2(list_ptr->prev->io_buffer[STDIN_FILENO], STDIN_FILENO);
					close(list_ptr->prev->io_buffer[STDIN_FILENO]);
				}
				if (list_ptr->type == TYPE_PIPE){
					dup2(list_ptr->io_buffer[STDOUT_FILENO], STDOUT_FILENO);
					close(list_ptr->io_buffer[STDIN_FILENO]);
					close(list_ptr->io_buffer[STDOUT_FILENO]);
				}

				if (execve(list_ptr->argv[0], list_ptr->argv, envp)){
					EXECVE_ERROR(list_ptr->argv[0]);
					exit(1);
				}
			}

			waitpid(pid, NULL, 0);
		}
		if (list_ptr->prev && list_ptr->prev->type == TYPE_PIPE){
			close(list_ptr->prev->io_buffer[STDIN_FILENO]);
			close(list_ptr->prev->io_buffer[STDOUT_FILENO]);
		}
		if (list_ptr->type == TYPE_PIPE){
			close(list_ptr->io_buffer[STDOUT_FILENO]);
		}

		list_ptr = list_ptr->next;
	}
}

int main(int argc, char **argv, char **envp){
	if (argc == 1){
		return (0);
	}

	Executable *list = parse_input(argc, argv);
	if (!list){
		return (0);
	}

	execute_list(list, envp);
	destroy_list(list);
}