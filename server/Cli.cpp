#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <sstream>
#include <cstring>
#include <iostream>
#include <thread>

#define PORT 8080

typedef struct s_data {
	int obj_socket;
	int reader;
	struct sockaddr_in serv_addr;
} t_data;

void	error_check(int err, int is_pton, std::string msg)
{
	if (is_pton)
	{
		if (err < 0 || (is_pton && err <= 0))
		{
			std::cout << "Client error: " << msg << std::endl;
			exit(EXIT_FAILURE);	
		}
	}
}

void	init_data(t_data *data)
{
	// getting the servers socket
	error_check((data->obj_socket = socket(AF_INET, SOCK_STREAM, 0)), 0,"creating a socket");
	data->serv_addr.sin_family = AF_INET;
	data->serv_addr.sin_port = htons(PORT);
	// Converting IPv4 and IPv6 addresses from text to binary form
	error_check(inet_pton(AF_INET, "127.0.0.1", &data->serv_addr.sin_addr), 1, "invalid adress");
}

void *client(void *arg)
{
	(void)arg;
	t_data data;
	std::string message = "REQUEST";
	char buffer[1024] = {0};

	init_data(&data);
	error_check(connect(data.obj_socket, (struct sockaddr *)&data.serv_addr, sizeof(data.serv_addr )), 0, "connection fail");
	send(data.obj_socket , message.c_str() , message.length() , 0);
	std::cout << "Message sent" << std::endl;
	data.reader = read(data.obj_socket, buffer, 1024);
	std::cout << std::string(buffer) << std::endl;
	return NULL;
}

int	get_num(char *arg)
{
	std::stringstream ss;
	int num = 0;

	std::string str(arg);
	ss << str;
	ss >> num;
	if (ss.fail())
	{
		std::cout << "Argument must be a number" << std::endl;
		return (-1);
	}
	return num;
}

int main(int argc, char **argv)
{
	int num = 0;

	if (argc != 2 || (num = get_num(argv[1])) < 0)
		return 1;
	pthread_t threads[num];
	for (int i = 0; i < num; i++)
	{
		pthread_create(&threads[i], NULL, client, NULL);
		sleep(1);
	}
	return 0;
}
