/* ******************************************************************
 * author : waponx
 * email  : 18826419224@163.com
 * time   : 7.22.2015
 * ******************************************************************/

#include "enrollaction.h"
#include "registeraction.h"
#include "server.h"
#include "simplemysql.h"


#include <assert.h>
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
	_addr_ptr = new struct sockaddr_in;
	_isreuseaddr = 1;

	if (sockfd < 0) {
		if ((sockfd = Socket()) < 0) {
			return false;
		}
	} else {
		_sockfd = sockfd;
	}

	memset(_addr_ptr, 0 , sizeof(*_addr_ptr));
	if (NULL == addr_ptr) {
		_addr_ptr->sin_family = AF_INET;
		_addr_ptr->sin_port = htonl(6666);
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
											string("CACWDB"),
											string("localhost"));

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
		assert(n > 0);
		buf[n] = '\0';
	}
}


void HttpServer::SetSocketfd(int sockfd) {
	if (sockfd >= 0) {
		_sockfd = sockfd;
	}
}

int HttpServer::GetSockfd() {
	return _sockfd;
}

struct sockaddr_in *HttpServer::GetSocketAddress() {
	return _addr_ptr;
}

bool HttpServer::Register(const std::string name,
						  const std::string password) {
	assert(_registeraction_ptr == NULL);

	return _registeraction_ptr->Register(name, password);
}

bool HttpServer::Enroll(const std::string name,
					    const std::string password) {
	assert(_enrollaction_ptr == NULL);

	return _enrollaction_ptr->Enroll(name, password);
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

void HttpServer::Get(const std::string url) {
	
}

void HttpServer::Post(const std::string url) {
	
}

bool HttpServer::Handle(const std::string request) {
	if (request.find("GET", 0, request.find(" ")) != std::string::npos) {
		Get(GetURL(request));
	}
	else if (request.find("GET", 0, request.find(" ")) != std::string::npos) {
		Post(GetURL(request));	
	}
	
	return false;
}

std::string HttpServer::GetURL(const std::string request) {
	std::size_t begin_pos = request.find(" ");
	std::size_t end_pos = request.find(" ", begin_pos + 1);
	string res = "";
	for (; begin_pos < end_pos; ++begin_pos) {
		res += request[begin_pos];
	}

	return res;
}

bool HttpServer::GetURL(const std::string request, std::string &url) {
	std::size_t begin_pos = request.find(" ");
	std::size_t end_pos = request.find(" ", begin_pos);
	for (url = ""; begin_pos < end_pos; ++begin_pos) {
		url += request[begin_pos];
	}

	return !url.empty();
}

void HttpServer::SetRegister(RegisterAction *ract_ptr) {
	_registeraction_ptr = ract_ptr;
}

void HttpServer::SetEnroll(EnrollAction *eact_ptr) {
	_enrollaction_ptr = eact_ptr;
}

RegisterAction *HttpServer::GetRegister() const {
	return _registeraction_ptr;
}

EnrollAction *HttpServer::GetEnroll() const {
	return _enrollaction_ptr;
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
