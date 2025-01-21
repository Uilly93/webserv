// #pragma once
#include "../includes/Server.hpp"
#include "../includes/colors.hpp"
#include <algorithm>
#include <asm-generic/socket.h>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

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
void Server::epoll_client_add() {
	struct epoll_event event;
	struct epoll_event events[10];
	socklen_t len = sizeof(struct sockaddr_in);
	int client_fd = -1;
	event.events = EPOLLIN;
	event.data.fd = server_socket;
	int epoll_instance = epoll_create(1);
	if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, server_socket, &event) == -1)
		std::cerr << "EPOLL_CTL ADD error: " << std::endl;
	while (true) {
		int nfds = epoll_wait(epoll_instance, events, 10, -1);
		if (nfds == -1) {
			std::cerr << "epoll_wait failed: " << std::endl;
			close(epoll_instance);
			return;
		}
		for (int i = 0; i < nfds; ++i) {
			if (event.data.fd == server_socket) {
				client_fd = accept(server_socket, (struct sockaddr *)&socket_addr, &len);
				if (client_fd == -1)
					std::cerr << "Erreur lors de l'acceptation de la connexion" << std::endl;
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
						// write(client_fd, response.c_str(), response.length());
						std::string response =
							"HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
						if (send(client_fd, response.c_str(), response.size(), 0) == -1)
							std::cerr << "Send error: " << std::endl;
						struct epoll_event out;
						// if (epoll_ctl(epoll_instance, EPOLL_CTL_MOD, out.data.fd, &out) == -1)
						// 	perror("");
						out.events = EPOLLOUT;
						out.data.fd = client_fd;
						std::cout << client_fd << RESET << std::endl;
						// write(out.data.fd, response.c_str(), response.size());
						// std::string response = "HTTP/1.1 200 OK\r\r\n\r\nHello, World!";
						// } else {
						// 	std::cout << "Sent response to client "<< client_fd << ": " << response
						// << std::endl;
						// }
					}
				}
				// if (events->events & EPOLLET) {
				// std::cout << "ssssss" << RESET << std::endl;
				// ssize_t sent = send(client_fd, response.c_str(), response.length(), 0);
				// struct epoll_event out;
				// out.events = EPOLLIN;
				// out.data.fd = client_fd;
				// epoll_ctl(epoll_instance, EPOLL_CTL_MOD, client_fd, &out);
				// }
			}
		}
	}
	close(epoll_instance);
}

// void Server::accept_connections() {
// 	// struct epoll_event event;

// 	// while (true) {
// 	epoll_client_add(client_fd);
// 	write(client_fd, "recieved\n", 11);
// }
void Server::Server_start() {
	int b = true;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &b, sizeof(b)) == -1)
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
