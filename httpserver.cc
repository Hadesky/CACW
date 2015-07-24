/* ******************************************************************
 * author : waponx
 * email  : 18826419224@163.com
 * time   : 7.22.2015
 * ******************************************************************/

#include "server.h"
#include "simplemysql.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 4096
#define QLEN 10

HttpServer::HttpServer() {
	
}

HttpServer::~HttpServer() {
	delete _addr_ptr;
}

bool HttpServer::Init(int sockfd, struct sockaddr_in *addr_ptr) {
	_sockfd = sockfd;
	_addr_ptr = new struct sockaddr_in;
	_isreuseaddr = 1;

	if ( (sockfd = Socket()) < 0) {
		return false;
	}

	memset(_addr_ptr, 0 , sizeof(*_addr_ptr));
	if (NULL == addr_ptr) {
		_addr_ptr->sin_family = AF_INET;
		_addr_ptr->sin_port = htonl(80);
		_addr_ptr->sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		_addr_ptr->sin_family = addr_ptr->sin_family;
		_addr_ptr->sin_port = addr_ptr->sin_port;
		_addr_ptr->sin_addr.s_addr = addr_ptr->sin_addr.s_addr;
	}
	
	setsockopt(_sockfd,
			   SOL_SOCKET,
			   SO_REUSEADDR,
			   &_isreuseaddr,
			   sizeof(_isreuseaddr));

	_spmysql_ptr = SimpleMySql::GetInstance(string("test"),
											string("test"),
											string("mysql"),
											string("localhst"));

	return true;
}

void HttpServer::Start() {
	if (!Bind() || !Listen(QLEN)) {
		return ;
	}
	
	char buf[BUFSIZE];
	struct sockaddr_in client_addr;

	while (true) {
		int clientfd;
		socklen_t client_addr_len;
		if ( (clientfd = accept(_sockfd, 
								(struct sockaddr *)&client_addr,
								&client_addr_len)) < 0) {
			close(_sockfd);
			return ;
		}
		int n = recv(clientfd, buf, BUFSIZE, MSG_ERRQUEUE);
		buf[n] = '\0';
	}
}


void HttpServer::SetSocketfd(int sockfd) {
	if (sockfd >= 0) {
		_sockfd = sockfd;
	}
}

void HttpServer::SetReuseAddr(bool flag) {
	if (true == flag) {
		_isreuseaddr = 1;
	} else  {
		_isreuseaddr = 0;
	}
}

bool HttpServer::IsReuseAddr() {
	return _isreuseaddr;
}

void HttpServer::Get() {
	
}

void HttpServer::Post() {
	
}


bool HttpServer::Bind() {
	if (bind(_sockfd,
			 (struct sockaddr *)_addr_ptr,
			 sizeof(*_addr_ptr)) < 0) {
		if (_sockfd >= 0) {
			close(_sockfd);
		}

		return false;
	}

	return true;
}

bool HttpServer::Listen(const int qlen) {
	if (listen(_sockfd, qlen) < 0) {
		if (_sockfd >= 0) {
			close(_sockfd);
		}

		return false;
	}	

	return true;
}

int HttpServer::Socket() {
	return socket(AF_INET, SOCK_STREAM, 0);
}
