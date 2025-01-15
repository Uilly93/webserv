// #pragma once
#include "../includes/Server.hpp"
#include "../includes/colors.hpp"
#include <algorithm>
#include <asm-generic/socket.h>
#include <cstddef>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>

// Constructors
Server::Server(int port) {
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		std::cerr << "Erreur de création du socket." << std::endl;
		exit(1);
	}

	// Définir l'adresse et le port du serveur
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_addr.s_addr = INADDR_ANY;
	socket_addr.sin_port = htons(port);
	// setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);
	std::cout << GREEN << "Server Default Constructor called" << RESET << std::endl;
}

// Destructor
Server::~Server() {
	std::cout << RED << "Server Destructor called " << RESET << std::endl;
}

// Methods
void Server::epoll_client_add(int client_fd){
	struct epoll_event event;
	struct epoll_event events[10];
	event.events = EPOLLIN;
	event.data.fd = client_fd;
	int epoll_instance = epoll_create(1);
	epoll_ctl(epoll_instance, EPOLL_CTL_ADD, client_fd, &event);
	while (true) {
		int nfds = epoll_wait(epoll_instance, events, 10, -1);
		if (nfds == -1) {
			std::cerr << "epoll_wait failed: " << std::endl;
			close(epoll_instance);
			return ;
		}
		for (int i = 0; i < nfds; ++i) {
			if (events[i].events & EPOLLIN) {
				std::cout << "Event detected on " << client_fd << " client " << std::endl;
				char buffer[1025];
				ssize_t count = read(client_fd, buffer, sizeof(buffer));
				if (count == -1) {
					std::cerr << "Read error: " << std::endl;
				} else {
					buffer[count] = 0;
					std::cout << "Read " << count << " bytes: " << std::string(buffer)
							  << std::endl;
				struct epoll_event out;
				out.data.fd = client_fd;
				out.events = EPOLLOUT;
				epoll_ctl(epoll_instance, EPOLL_CTL_MOD, client_fd, &out);
				}
			}
		}
	}
	// client_list[event] = client_fd;
}

void Server::accept_connections() {
	// struct epoll_event event;

	// while (true) {
	socklen_t len;
	int client_fd = accept(server_socket, (struct sockaddr *)&socket_addr, &len);
	if (client_fd == -1)
		std::cerr << "Erreur lors de l'acceptation de la connexion" << std::endl;
	epoll_client_add(client_fd);
}
void Server::Server_start() {
	int b = true;
	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &b, size_t(b)) == -1)
		std::cout << "ERROR sockopt" << RESET << std::endl;
	if (bind(server_socket, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0) {
		std::cerr << "Erreur while binding server" << std::endl;
		close(server_socket);
		exit(1);
	}
	if (listen(server_socket, 10) < 0) {
		std::cerr << "Error while listening" << std::endl;
		close(server_socket);
		exit(1);
	}
	std::cout << "Server listening on port " << ntohs(socket_addr.sin_port) << "..." << std::endl;
}
