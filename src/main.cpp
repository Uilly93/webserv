// #pragma once

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../includes/Server.hpp"

int main() {
	Server a(8080);
	a.Server_start();
	a.epoll_client_add();
}