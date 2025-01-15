// #pragma once

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../includes/Server.hpp"

// int create_socket() {
// 	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	sockaddr_in serverAddress;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_port = htons(8080);
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;
// }

int main() {
	Server a(8080);
	a.Server_start();
	a.accept_connections();
	// struct epoll_event event;
	// event.events = EPOLLIN;
	// // std::ifstream file("test.txt");
	// event.data.fd = STDIN_FILENO;
	// int epoll_instance = epoll_create(1);
	// epoll_ctl(epoll_instance, EPOLL_CTL_ADD, STDIN_FILENO, &event);
	// while (true) {
	// 	struct epoll_event events[10];
	// 	int nfds = epoll_wait(epoll_instance, events, 10, -1);
	// 	if (nfds == -1) {
	// 		std::cerr << "epoll_wait failed: " << std::endl;
	// 		close(epoll_instance);
	// 		return 1;
	// 	}

	// 	for (int i = 0; i < nfds; ++i) {
	// 		if (events[i].data.fd == STDIN_FILENO) {
	// 			std::cout << "Event detected on stdin" << std::endl;
	// 			char buffer[1024];
	// 			ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer));
	// 			if (count == -1) {
	// 				std::cerr << "Read error: " << std::endl;
	// 			} else {
	// 				std::cout << "Read " << count << " bytes: " << std::string(buffer, count)
	// 						  << std::endl;
	// 			}
	// 		}
	// 	}
	// }
	// return 0;
}