// #pragma once
#include "../includes/Server.hpp"
#include "../includes/colors.hpp"
#include <algorithm>
#include <asm-generic/socket.h>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

// Constructors
Server::Server(int port) {
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		perror("Erreur lors de l'enregistrement du gestionnaire de signal");
		// close(server_socket);
		exit(EXIT_FAILURE);
	}
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << server_socket << RESET << std::endl;
	if (server_socket < 0) {
		std::cerr << "Erreur de création du socket." << std::endl;
		// close(4);
		exit(1);
	}

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_addr.s_addr = INADDR_ANY;
	socket_addr.sin_port = htons(port);
	// setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);
	std::cout << GREEN << "Server Default Constructor called" << RESET << std::endl;
}

// Destructor
Server::~Server() {
	std::cout << RED << "Server Destructor called " << RESET << std::endl;
	close(server_socket);
	for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (*it != -1)
			close(*it);
	}
}

std::string htmlPage =
	"<!DOCTYPE html>\n"
	"<html lang=\"fr\">\n"
	"<head>\n"
	"    <meta charset=\"UTF-8\">\n"
	"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
	"    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n"
	"    <title>WeebServ UwU</title>\n"
	"    <style>\n"
	"        body {\n"
	"            font-family: Arial, sans-serif;\n"
	"            background-color: #f4f4f4;\n"
	"            margin: 0;\n"
	"            padding: 0;\n"
	"        }\n"
	"        header {\n"
	"            background-color: #333;\n"
	"            color: white;\n"
	"            padding: 10px 0;\n"
	"            text-align: center;\n"
	"        }\n"
	"        main {\n"
	"            padding: 20px;\n"
	"        }\n"
	"        footer {\n"
	"            background-color: #333;\n"
	"            color: white;\n"
	"            text-align: center;\n"
	"            padding: 10px 0;\n"
	"            position: fixed;\n"
	"            bottom: 0;\n"
	"            width: 100%;\n"
	"        }\n"
	"        button_post {\n"
	"            padding: 10px 10px;\n"
	"            font-size: 16px;\n"
	"            cursor: pointer;\n"
	"            margin-top: 20px;\n"
	"            background-color: #007bff;\n"
	"            color: white;\n"
	"            border: none;\n"
	"            border-radius: 20px;\n"
	"        }\n"
	"        button_post:hover {\n"
	"            background-color: #0056b3;\n"
	"        }\n"
	"        button_delete {\n"
	"            padding: 10px 10px;\n"
	"            font-size: 16px;\n"
	"            cursor: pointer;\n"
	"            margin-top: 20px;\n"
	"            background-color: #007bff;\n"
	"            color: white;\n"
	"            border: none;\n"
	"            border-radius: 20px;\n"
	"        }\n"
	"        button_delete:hover {\n"
	"            background-color: #0056b3;\n"
	"        }\n"
	"        a {\n"
	"            color: #007bff;\n"
	"            text-decoration: none;\n"
	"        }\n"
	"        a:hover {\n"
	"            text-decoration: underline;\n"
	"        }\n"
	"    </style>\n"
	"</head>\n"
	"<body>\n"
	"    <header>\n"
	"        <h1>Welcome to wnocchi, vstineau and umerde 's webserv</h1>\n"
	"    </header>\n"
	"\n"
	"    <main>\n"
	"        <h2>WebServ</h2>\n"
	"        <p>choose the methode you want to use</p>\n"
	"        <button_post onclick=\"alert('you have choosed POST method"
	"');\">POST</button_post>\n"
	"        <button_delete onclick=\"alert('you have choosed DELETE method"
	"');\">DELETE</button_delete>\n"
	"        <p>Voici un <a href=\"https://www.example.com\" target=\"_blank\">lien vers un site "
	"externe</a>.</p>\n"
	"    </main>\n"
	"\n"
	"    <footer>\n"
	"        <p>&copy; WeebServ UwU Baka Pro Max club</p>\n"
	"    </footer>\n"
	"</body>\n"
	"</html>\n";

// Methods
bool client_already_logged(std::vector<int> client_list, int client_fd) {
	for (std::vector<int>::iterator it = client_list.begin(); it != client_list.end(); it++) {
		if (*it == client_fd)
			return true;
	}
	return false;
}

void sigint_handler(int sig) {
	(void)sig;
	// printf("\nSignal SIGINT reçu (Ctrl+C) !\n");
	// Vous pouvez aussi effectuer un nettoyage ou quitter proprement
	// exit(0);  // Décommentez cette ligne pour terminer le programme après avoir reçu SIGINT
}

std::string to_string(long i) {
	std::stringstream s;
	s << i;
	return s.str();
}

int send_response(std::string message, int fd) {
	std::string head =
		"HTTP/1.1 200 OK\r\nContent-Length: " + to_string(message.length()) + "\r\n\r\n" + message;
	return send(fd, head.c_str(), head.size(), MSG_NOSIGNAL);
}

// void Server::epoll_client_add() { /////////////////////////////////
// 	struct epoll_event event;
// 	struct epoll_event events[10];
// 	socklen_t len = sizeof(struct sockaddr_in);
// 	int client_fd = -1;
// 	event.events = EPOLLIN;
// 	event.data.fd = server_socket;
// 	epoll_instance = epoll_create(1);
// 	// std::cout << epoll_instance << RESET << std::endl;

// 	if (epoll_instance == -1) {
// 		std::cerr << "Failed to create epoll instance: " << std::endl;
// 		return;
// 	}
// 	if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, server_socket, &event) == -1)
// 		std::cerr << "EPOLL_CTL ADD error: " << std::endl;
// 	while (true) {
// 		int nfds = epoll_wait(epoll_instance, events, 10, -1);
// 		// std::cout << "nfds = " << nfds << RESET << std::endl;
// 		if (nfds == -1) {
// 			std::cerr << "epoll_wait failed: " << std::endl;
// 			close(epoll_instance);
// 			return;
// 		}
// 		std::cout << "2" << RESET << std::endl;
// 		for (int i = 0; i < nfds; ++i) {
// 			if (events[i].data.fd == server_socket) {
// 				// std::cout << "3" << RESET << std::endl;
// 				client_fd = accept(server_socket, (struct sockaddr *)&socket_addr, &len);
// 				if (client_fd == -1)
// 					std::cerr << "error while accepting new connexion" << std::endl;
// 				if (!client_already_logged(clients, client_fd)) {
// 					event.data.fd = client_fd;
// 					event.events = EPOLLIN | EPOLLRDHUP;
// 					if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, event.data.fd, &event) == -1) {
// 						std::cerr << "Failed to add new client_fd to epoll: " << std::endl;
// 						close(client_fd);
// 						continue;
// 					}
// 					clients.push_back(client_fd);
// 				}
// 			} else if (events[i].events & EPOLLIN) {
// 				// std::cout << "4" << RESET << std::endl;
// 				std::cout << "Event detected on " << event.data.fd << " client " << std::endl;
// 				char buffer[1025];
// 				ssize_t count = read(event.data.fd, buffer, sizeof(buffer));
// 				if (count == -1) {
// 					std::cerr << "Read error: " << std::endl;
// 				} else if (count == 0) {
// 					// Le client a fermé la connexion
// 					printf("Client %d disconnected\n", events[i].data.fd);
// 					close(events[i].data.fd);
// 				} else {
// 					buffer[count] = 0;
// 					std::cout << "Read " << count << " bytes on fd [" << events[i].data.fd << "]\n"
// 							  << std::string(buffer);
// 					struct epoll_event out;
// 					out.events = EPOLLOUT | EPOLLRDHUP;
// 					out.data.fd = events[i].data.fd;
// 					if (send_response(htmlPage, events[i].data.fd) == -1)
// 						std::cerr << "Send error: " << std::endl;
// 					// close(events[i].data.fd);
// 					// out.events = EPOLLIN | EPOLLRDHUP;
// 				}
// 			} else if (events[i].events & EPOLLRDHUP) {
// 				std::cout << "5" << RESET << std::endl;
// 				std::cout << "Client disconnected: " << events[i].data.fd << std::endl;
// 				// close(events[i].data.fd);
// 			}
// 		}
// 		// close(epoll_instance);
// 	}
// } ////////////////////////////

void Server::epoll_client_add() {
    struct epoll_event event;
    struct epoll_event events[10];
    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in socket_addr;
    int client_fd = -1;
    event.events = EPOLLIN | EPOLLRDHUP;
    event.data.fd = server_socket;
    int epoll_instance = epoll_create(1);
    if (epoll_instance == -1) {
        std::cerr << "Failed to create epoll instance: " << std::endl;
        return;
    }
    if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, server_socket, &event) == -1) {
        std::cerr << "Failed to add server_socket to epoll: " << std::endl;
        close(epoll_instance);
        return;
    }
    while (true) {
        int nfds = epoll_wait(epoll_instance, events, 10, -1);
        if (nfds == -1) {
            std::cerr << "epoll_wait failed: " << std::endl;
            close(epoll_instance);
            return;
        }
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_socket) {
                client_fd = accept(server_socket, (struct sockaddr *)&socket_addr, &len);
                if (client_fd == -1) {
                    std::cerr << "Error while accepting new connection: " << std::endl;
                    continue;
                }
                std::cout << "Accepted new connection with fd: " << client_fd << std::endl;
                event.data.fd = client_fd;
                event.events = EPOLLIN | EPOLLRDHUP;
                if (epoll_ctl(epoll_instance, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    std::cerr << "Failed to add new client_fd to epoll: " << std::endl;
                    close(client_fd);
                    continue;
                }
                clients.push_back(client_fd);
            } else if (events[i].events & EPOLLIN) {
                std::cout << "Event detected on " << events[i].data.fd << " client " << std::endl;
                char buffer[1025];
                ssize_t count = read(events[i].data.fd, buffer, sizeof(buffer));
                if (count == -1) {
                    std::cerr << "Read error: " << std::endl;
                } else if (count == 0) {
                    std::cout << "Client " << events[i].data.fd << " disconnected" << std::endl;
                    close(events[i].data.fd);
                } else {
                    buffer[count] = 0;
                    std::cout << "Read " << count << " bytes on fd [" << events[i].data.fd << "]\n"
                              << std::string(buffer);
                    struct epoll_event out;
                    out.events = EPOLLOUT | EPOLLRDHUP;
                    out.data.fd = events[i].data.fd;
                    if (send_response(htmlPage, events[i].data.fd) == -1) {
                        std::cerr << "Send error: " << std::endl;
                    }
                }
            } else if (events[i].events & EPOLLRDHUP) {
                std::cout << "Client disconnected: " << events[i].data.fd << std::endl;
                close(events[i].data.fd);
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
