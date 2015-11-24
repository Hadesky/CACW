/* ******************************************************************
 * author : waponx
 * email  : 18826419224@163.com
 * time   : 7.22.2015
 * ******************************************************************/

#include "enrollaction.h"
#include "jsontransverter.h"
//#include "multhreads.h"
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
	const int QLEN = 512;

	//  the buffer size.
	const int BUFFSIZE = 4096;
}



HttpServer::HttpServer() :
	_state_code(""),
	_isreuseaddr(0),
	_spmysql_ptr(),
	_isinit(false){

	}

HttpServer::~HttpServer() {
	delete _addr_ptr;
	//	delete _multhreads_ptr;
	delete _registeraction_ptr;
	delete _enrollaction_ptr;
}

bool HttpServer::Init(int sockfd, struct sockaddr_in *addr_ptr) {
#ifdef DEBUG
	printf("HttpServer initing\n");
#endif // ! DEBUG
	_addr_ptr = new struct sockaddr_in;
	_isreuseaddr = 1;
	//	_multhreads_ptr = new HttpMulThreads(this);
	_registeraction_ptr = new HttpRegisterAction();
	_enrollaction_ptr = new HttpEnrollAction();
	_spmysql_ptr = SimpleMySql::GetInstance("localhost",
			"root",
			"mysqllearning",
			"CACWDB");

	//	_multhreads_ptr->Init();
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
	_isinit = true;

	return true;
}

void HttpServer::Start() {
	if (!_isinit) {
		return ;
	}
	Loop();	
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
		const std::string &authcode){

	return _registeraction_ptr->Register(name,
			password,
			sex,
			email,
			authcode);
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
		const std::string &content,
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

	return Post(command, sessionid, content, res);
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
	else if ("004" == command) {
		// 获取验证码，并将验证码发送给email邮箱
		Json::Value value = JsonTransverter::ParseJsonString(content);
		return _registeraction_ptr->GetAuthCode(value["email"].asString());
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
		respone = Get(command, sessionid, content, res);
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

pthread_mutex_t &HttpServer::GetClientMutex() {
	static pthread_mutex_t s_client_mutex = PTHREAD_MUTEX_INITIALIZER;
	return s_client_mutex;
}

void *HttpServer::Start_rtn(void *arg) {
	pthread_detach(pthread_self());
	MulThreadMsg *msg = (MulThreadMsg *)arg;
	char command[BUFFSIZE];
	std::string content;
	//printf("client fd : %d\n", msg->GetClientFd());

	int len = recv(msg->GetClientFd(), command, BUFFSIZE, 0);
	//printf("sign is %d\n", len);
#ifdef DEBUG
	printf("Start_rtn command : \n%s\n", command);
#endif	// !DEBUG
	pthread_mutex_lock(&GetClientMutex());
	std::string respone(msg->GetServer()->Handle(command, content));
	std::string res(msg->GetServer()->GetHttpResponseHead("HTTP/1.1",
				"200",
				"OK"));
	static const std::string cookie_str("Set-Cookie: respone=");
	static const std::string content_len_str("Content-Length: ");

	HttpServer::AddFieldInHttpResponseHead(res,
			content_len_str + SizeToString(content.size()));
	HttpServer::AddFieldInHttpResponseHead(res, cookie_str + respone);
	for (std::string::size_type len = 0; len < res.size();) {
			//len += write(msg->GetClientFd(), res.c_str(), res.size());
			len += send(msg->GetClientFd(), res.c_str(), res.size(), MSG_CONFIRM);
	}
	//  TO DO : add your code
	pthread_mutex_unlock(&GetClientMutex());
	close(msg->GetClientFd());

	// 不能在其他地方delete，否则的话就会在线程没有执行完之前释放空
	delete msg;

	return NULL;
}

std::string HttpServer::SizeToString(const std::string::size_type &len) {
	static char buf[16];
	sprintf(buf, "%ld", len);

	return std::string(buf); 
}

void HttpServer::Loop() {
	int client_fd;
	socklen_t client_addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *client_addr_ptr;
	//	HttpServer *httpserver = (HttpServer *)arg;

	client_addr_ptr = new struct sockaddr_in;
	while(true) {
		//  accept http request and Handle the request;
		client_fd = Accept((struct sockaddr *)client_addr_ptr,
				&client_addrlen);
		MulThreadMsg *msg = new MulThreadMsg(client_fd, this);
		pthread_t ptid;
		if (pthread_create(&ptid,
					NULL,
					&Start_rtn,
					(void *)msg) != 0) {
			return ;	
		}
	}
}

// ********************************************************
// MulThreadMsg class


MulThreadMsg::MulThreadMsg(int fd, HttpServer *server):
	_clientfd(fd), _server(server) {

	}

int MulThreadMsg::GetClientFd() {
	return _clientfd;
}

HttpServer *MulThreadMsg::GetServer() {
	return _server;
}


// end MulThreadMsg class
// ********************************************************
