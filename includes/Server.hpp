#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <sys/epoll.h>

class Server {
	public:
	// Constructor
	Server(int port);
	// Server(parametric constructor);
	// Server(const Server &src);

	// Destructor
	~Server();

	// Methods
	void Server_start();
	void accept_connections();
	void epoll_client_add(int client_fd);

	int server_socket;
	sockaddr_in socket_addr;
	// std::map<struct epoll_event, int> client_list;

};

#endif