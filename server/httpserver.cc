/* ******************************************************************
 * author : waponx
 * email  : 18826419224@163.com
 * time   : 7.22.2015
 * ******************************************************************/

#include "enrollaction.h"
#include "multhreads.h"
#include "registeraction.h"
#include "server.h"
#include "simplemysql.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#ifdef DEBUG
#include <cstdio>
#endif // ! DEBUG

HttpServer::HttpServer() {
	
}

HttpServer::~HttpServer() {
	delete _addr_ptr;
	delete _multhreads_ptr;
}

bool HttpServer::Init(int sockfd, struct sockaddr_in *addr_ptr) {
#ifdef DEBUG
	printf("HttpServer initing\n");
#endif // ! DEBUG
	_addr_ptr = new struct sockaddr_in;
	_isreuseaddr = 1;
	_multhreads_ptr = new HttpMulThreads(this);
	_registeraction_ptr = new HttpRegisterAction();
	_enrollaction_ptr = new HttpEnrollAction();
	_spmysql_ptr = SimpleMySql::GetInstance("localhost",
			"root",
			"mysqllearning",
			"CACWDB");

	_multhreads_ptr->Init();
	_registeraction_ptr->Init(_spmysql_ptr);
	_enrollaction_ptr->Init(_spmysql_ptr);
	if (sockfd < 0) {
		if ( (_sockfd = Socket()) < 0) {
			return false;
		}
	} else {
		_sockfd = sockfd;
	}
#ifdef DEBUG
	printf("server fd : %d\n", _sockfd);
#endif	// !DEBUG
	memset(_addr_ptr, 0 , sizeof(struct sockaddr_in));
	if (NULL == addr_ptr) {
		_addr_ptr->sin_family = AF_INET;
		_addr_ptr->sin_port = htons(8000);
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

	if (!Bind()) {
		return false;
	}
	if (!Listen(QLEN)) {
		return false;
	}

	return true;
}

void HttpServer::Start() {
	_multhreads_ptr->Create(NTHREADS);
	_multhreads_ptr->Loop();
	
//	char buf[BUFSIZE];
//	struct sockaddr_in client_addr;
//
//	while (true) {
//		int clientfd;
//		socklen_t client_addr_len;
//		if ( (clientfd = accept(_sockfd, 
//								(struct sockaddr *)&client_addr,
//								&client_addr_len)) < 0) {
//			close(_sockfd);
//			return ;
//		}
//		int n = recv(clientfd, buf, BUFSIZE, MSG_ERRQUEUE);
//		buf[n] = '\0';
//	}


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

bool HttpServer::Register(const std::string &name,
		const std::string &password,
		const std::string &sex,
		const std::string &email,
		const std::string &address,
		const std::string &phonenumber,
		const std::string &shortphonenumber){

	return _registeraction_ptr->Register(name,
			password,
			sex,
			email,
			address,
			phonenumber,
			shortphonenumber);
}

bool HttpServer::Enroll(const std::string &name,
		const std::string &password){
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

std::string HttpServer::Get(const std::string url) {
	std::string res("");
#ifdef DEBUG
	printf("HttpServer::Get\n");
	printf("Register : \n");
	Register("hahaha", "123456789");
	printf("Enroll : \n");
	Enroll("hahaha", "123456789");
#endif	// ! DEBUG
	if ("./register" == url) {
		//  TO DO: Add your register code
	}
	else if ("./eroll" == url) {
		//  TO DO: Add your enroll code
	}
	
	return res;
}

std::string HttpServer::Post(const std::string url) {
	std::string res("");
#ifdef DEBUG
	printf("HttpServer::Post:\n");
	printf("Register : \n");
	Register("hahaha", "123456789");
	printf("Enroll : \n");
	Enroll("hahaha", "123456789");
#endif	// ! DEBUG
	if ("./register" == url) {
		//  TO DO: Add your register code
	}
	else if ("./enroll" == url) {
		//  TO DO: Add your enroll code
		// _enrollaction_ptr->Enroll("", "");
	}

	return res;
}

std::string HttpServer::Handle(const std::string request) {
	if (request.find("GET", 0, request.find(" ")) != std::string::npos) {
		Get(GetURL(request));
	}
	else if (request.find("GET", 0, request.find(" ")) != std::string::npos) {
		Post(GetURL(request));	
	}
	
	return "";
}

std::string HttpServer::GetURL(const std::string request) {
	std::size_t begin_pos = request.find(" ");
	std::size_t end_pos = request.find(" ", begin_pos + 1);
	string res("");
	for (; begin_pos < end_pos; ++begin_pos) {
		res += request[begin_pos];
	}
#ifdef DEBUG
	printf("HttpServer::GetURL : the URL is :%s\n", res.c_str());
#endif 
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
#ifdef DEBUG
	printf("bind success...\n");
#endif	// ! DEBUG
	return true;
}

bool HttpServer::Listen(const int qlen) {
	if (listen(_sockfd, qlen) < 0) {
		if (_sockfd >= 0) {
			close(_sockfd);
		}

		return false;
	}	
#ifdef DEBUG
	printf("listen success...\n");
#endif	// ! DEBUG

	return true;
}

int HttpServer::Socket() {
	//  make sure 0, 1 and 2 file description are opened
	dup2(0, 0);
	dup2(1, 1);
	dup2(2, 2);

	return socket(AF_INET, SOCK_STREAM, 0);
}

int HttpServer::Accept(struct sockaddr *addr_ptr, socklen_t *len_ptr) {
	return accept(_sockfd, addr_ptr, len_ptr);
} 
