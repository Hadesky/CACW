/* ******************************************************
 * author : waponx
 * email  : 18826419224@163.com
 * time   : 7.22.2015
 * ******************************************************/

#include "server.h"

Server::Server() {
	
}

Server::~Server() {
	delete _addr_ptr;
}

bool Server::Init(int sockfd, struct sockaddr_in *addr_ptr) {
	_sockfd = sockfd;
	_addr_ptr = addr_ptr;
	_registeraction_ptr = NULL;
	_enrollaction_ptr = NULL;

	return true;
}

