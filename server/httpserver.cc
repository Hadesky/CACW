/* ******************************************************************
 * author : waponx
 * email  : 18826419224@163.com
 * time   : 7.22.2015
 * ******************************************************************/

#include "enrollaction.h"
#include "jsontransverter.h"
#include "multhreads.h"
#include "registeraction.h"
#include "session.h"
#include "server.h"
#include "simplemysql.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#ifdef DEBUG
#include <cstdio>
#endif // ! DEBUG

namespace {
//  socket waiting queue's length.
const int QLEN = 10;

//  the buffer size.
const int BUFFSIZE = 4096;
}



HttpServer::HttpServer() 
	:_isreuseaddr(0),
	 _spmysql_ptr(),
	 _state_code("") {
	
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
		_addr_ptr->sin_addr.s_addr = htons(INADDR_ANY);
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

std::string HttpServer::Register(const std::string &name,
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

std::string HttpServer::Enroll(const std::string &name,
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

std::string HttpServer::Get(const std::string &command,
		const std::string &sessionid,
		const std::string &context,
		std::string &res){
//#ifdef DEBUG
//	printf("HttpServer::Get\n");
//	printf("Register : \n");
//	Register("hahaha", "123456789");
//	printf("Enroll : \n");
//	Enroll("hahaha", "123456789");
//#endif	// ! DEBUG
//	Json::Value value = JsonTransverter::ParseJsonString(context);
//	if ("001" == command) {
//		//  TO DO: Add your login code
//
//		return Enroll(value["username"].asString(),
//				value["password"].asString());
//		//JsonTransverter::ToJsonString("{Result: log success}", res);
//	}
//	else if ("002" == command) {
//		//  TO DO: Add your regiser code
//		//Json::Value value = JsonTransverter::ParseJsonString(context);
//		return Register(value["username"].asString(),
//				value["password"].asString());
//		//JsonTransverter::ToJsonString("{Result: register success}", res);
//	}
//	
//	return std::string("001");

	return Post(command, sessionid, context, res);
}

std::string HttpServer::Post(const std::string &command,
		const std::string &sessionid,
		const std::string &content,
		std::string &res) {
//#ifdef DEBUG
//	printf("HttpServer::Post:\n");
//	printf("Register : \n");
//	Register("hahaha", "123456789");
//	printf("Enroll : \n");
//	Enroll("hahaha", "123456789");
//#endif	// ! DEBUG
	if ("002" == command) {
		//  TO DO: Add your login code
		Json::Value value = JsonTransverter::ParseJsonString(content);

		return Enroll(value["username"].asString(),
				value["password"].asString());
		//JsonTransverter::ToJsonString("{Result: log success}", res);
	}
	else if ("003" == command) {
		//  TO DO: Add your regiser code
		Json::Value value = JsonTransverter::ParseJsonString(content);

		return Register(value["username"].asString(),
						value["password"].asString(),
						value["email"].asString(),
						value["sex"].asString(),
						value["AUTHcode"].asString());
		//JsonTransverter::ToJsonString("{Result: register success}", res);
	}
	else if ("005" == command) {
		
	}
	else if ("006" == command) {
		if (!Session::GetInstance().Visit(sessionid)) {
			return "001";
		}
		_spmysql_ptr->Query("select USERID, ");
	}
	
	return std::string("001");
}

std::string HttpServer::Handle(const std::string &request) {
	std::string method;
	std::string command;
	std::string sessionid = GetSeesionID(request);
	std::string context = GetRequestContent(request);
	std::string res;
	std::string respone("001");

	GetMethod(request, method);

#ifdef DEBUG
	printf("HttpServer::Handle...\n");
#endif	// !DEBUG
	GetCommand(request, command);
	if (method == "GET") {
		respone = Get(command, sessionid, context, res);
	}
	else if (method != "POST") {
		respone = Post(command, sessionid, context, res);
	}
	
	return respone;
}

std::string HttpServer::Handle(const std::string &request, std::string &res) {
	std::string method;
	std::string command;
	std::string sessionid = GetSeesionID(request);
	std::string content = GetRequestContent(request);
	std::string respone("001");
	GetMethod(request, method);
	GetCommand(request, command);
	if (method == "GET") {
		//return Get(command, res);
		respone = Get(command, content, res);
	}
	else if (method == "POST") {
		respone = Post(command, sessionid, content, res);	
	}
	
	return respone;
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

bool HttpServer::GetMethod(const std::string &request, std::string &method) {
	if (request.empty()) {
		return false;
	}
	std::size_t pos = request.find(" ");
	if (pos <= 0) {
		return false;
	}
	method = request.substr(0, pos);
#ifdef DEBUG
	printf("HttpServer::GetMethod : %s\n", method.c_str());
#endif	//!DEBUG

	return true;
}


bool HttpServer::GetCommand(const std::string &request, std::string &command) {
	if (request.empty()) {
		return false;
	}

	std::size_t start_pos = request.find("command");
	if (std::string::npos == start_pos) {
		return false;
	}
	start_pos += strlen("command: "); 
	command = request.substr(start_pos, 3);
#ifdef DEBUG
	printf("HttpServer::GetCommand : %s\n", command.c_str());
#endif	//!DEBUG
	
	return true;
}

std::string HttpServer::GetSeesionID(const std::string &request) {
	if (request.empty()) {
		return std::string("");
	}
	std::string::size_type start_pos = request.find("sessionid: ");
	if (std::string::npos == start_pos) {
		return string("");
	}
	std::string::size_type end_pos = request.find("\r\n", start_pos);
	if (std::string::npos == end_pos) {
		return std::string("");
	}
	start_pos += strlen("sessionid: ");

	return request.substr(start_pos, end_pos - start_pos);
}

std::string HttpServer::GetHttpResponseHead(const std::string &version,
		const std::string &state_code,
		const std::string &reason_phrase){
	return std::string(version + " " + state_code + " " + reason_phrase + "\r\n\r\n");
}

std::string HttpServer::GetHttpResponseHead(const std::string &version,
		const std::string &state_and_phrase_str) {
	return std::string(version + " " + state_and_phrase_str + "\r\n\r\n");
}

bool HttpServer::AddFieldInHttpResponseHead(std::string &httprsph,
		const std::string &field,
		const std::string &value) {
	std::size_t pos = httprsph.rfind("\r\n");
	if (pos == std::string::npos) {
#ifdef DEBUG
		printf("HttpServer::AddFieldInHttpResponseHead: not find \r\n");
#endif	// !DEBUG
		return false;
	}
	httprsph.insert(pos, field + ": " + value + "\r\n");

	return true;
}

bool HttpServer::AddFieldInHttpResponseHead(std::string &httprsph,
		const std::string &field_and_value) {
	std::size_t pos = httprsph.rfind("\r\n");
	if (pos == std::string::npos) {
#ifdef DEBUG
		printf("HttpServer::AddFieldInHttpResponseHead: not find \\r\\n");
#endif	// !DEBUG
		return false;
	}
	httprsph.insert(pos, field_and_value + "\r\n");

	return true;
}

const std::string HttpServer::GetStateCode() const {
	return _state_code;
}

const std::string HttpServer::GetDscrpt() const {
	return _dscrpt;
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

std::string HttpServer::GetRequestContent(const std::string &request) {
	std::string::size_type pos = request.find("\r\n\r\n");
	if (std::string::npos == pos) {
#ifdef DEBUG
		printf("HttpServer::GetRequestContext: can't find \\r\\n\\r\\n\n");
#endif	// DEBUG
		return std::string("");
	}

	return request.substr(pos +strlen("\r\n\r\n"));
}
