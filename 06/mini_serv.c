#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/errno.h>

typedef struct s_client{
    int                 id;
    int                 fd;
    struct s_client*    next;
} t_client;

int server_fd, last_id;
fd_set server_fd_set, write_fd_set, read_fd_set;
t_client* clients_list;


int extract_message(char **buf, char **msg){
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}
char *str_join(char *buf, char *add){
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}


void fatalError(){
    const char* fatalErrorMsg = "Fatal error\n";

    write(STDERR_FILENO, fatalErrorMsg, strlen(fatalErrorMsg));
    exit(1);
}
void badArgsError(){
    const char* badArgsErrMsg = "Wrong number of arguments\n";

    write(STDERR_FILENO, badArgsErrMsg, strlen(badArgsErrMsg));
    exit(1);
}


int get_max_fd(){
    int max = server_fd;

    t_client* tmp = clients_list;
    while (tmp){
        if (tmp->fd > max) max = tmp->fd;
        tmp = tmp->next;
    }

    return max;
}
void send_all(int author_fd, char* msg){
    t_client* tmp = clients_list;

    while (tmp){
        if (tmp->fd != author_fd){
            if (send(tmp->fd, msg, strlen(msg), 0) == -1) fatalError();
        }
        tmp = tmp->next;
    }
}


void add_client(){
    char buf[4096];
    struct sockaddr_in clientaddr;

    socklen_t len = sizeof(clientaddr);
    int client_fd = accept(server_fd, (struct sockaddr *)&clientaddr, &len);
    if (client_fd == -1) fatalError();

    t_client* new_client = malloc(sizeof(t_client));
    if (!new_client) fatalError();

    new_client->id = last_id++;
    new_client->fd = client_fd;
    new_client->next = NULL;

    if (!clients_list) clients_list = new_client;
    else {
        t_client* tmp = clients_list;
        while (tmp->next) tmp = tmp->next;
        tmp->next = new_client;
    }

    FD_SET(new_client->fd, &server_fd_set);
    sprintf(buf, "server: client %d just arrived\n", new_client->id);
    send_all(new_client->fd, buf);
}
int rm_client(int fd){
    t_client* prev = NULL;
    t_client* cur = clients_list;

    while (cur){
        if (cur->fd == fd){
            if (prev) prev->next = cur->next;
            int id = cur->id;
            free(cur);
            return id;
        }
        prev = cur;
        cur = cur->next;
    }

    return -1;
}


int main(int argc, char** argv){
    if (argc < 2) badArgsError();
    
    last_id = 0;
    clients_list = NULL;

    int port = atoi(argv[1]);
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433);
    servaddr.sin_port = htons(port);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) fatalError();
    if (bind(server_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) fatalError();
    if (listen(server_fd, 0) == -1) fatalError();

    FD_ZERO(&server_fd_set);
    FD_SET(server_fd, &server_fd_set);
    while (1){
        write_fd_set = read_fd_set = server_fd_set;
        if (select(get_max_fd() + 1, &read_fd_set, &write_fd_set, NULL, NULL) < 0) continue;
        for (int fd = 0; fd < get_max_fd(); fd++){
            if (!FD_ISSET(fd, &read_fd_set)) continue;
            if (fd == server_fd){
                add_client();
                break;
            } else {
                const int buffer_size = 1024;
                char buffer[buffer_size];
                char* msg = NULL;

                int read_bytes = 0, size = 0;
                while (1){
                    read_bytes = recv(fd, buffer, buffer_size - 1, 0);
                    if (read_bytes <= 0) break;
                    size += read_bytes;
                    buffer[read_bytes] = 0;
                    msg = str_join(msg, buffer);
                    if (!msg) fatalError();
                }

                if (size <= 0){
                    char buff[1024];
                    int id = rm_client(fd);
                    sprintf(buff, "server: client %d just left\n", id);                    
                    FD_CLR(fd, &server_fd_set);
                    close(fd);
                    break;
                } else {
                    while (1){
                        char* line;
                        int ret = extract_message(&msg, &line);
                        if (ret == -1) fatalError();

                        char* str = (char *)malloc(21 + strlen(line));
                        if (!str) fatalError();

                        sprintf(str, "client %d: %s", fd, line);
                        send_all(fd, str);
                        free(str);

                        if (ret == 0) break;
                    }
                }
            }
        }
    }
}
