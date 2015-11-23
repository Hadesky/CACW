/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/4/2015
 * ********************************************************/

#include "server.h"
#include "multhreads.h"

#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
#include <cstdio>
#endif // ! DEBUG

namespace {
	const int BUFFSIZE = 4096;
}

// ********************************************************
// MulThreadMsg class implement

MulThreadMsg::MulThreadMsg(int fd, HttpServer *server) :
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



HttpMulThreads::HttpMulThreads() {
	_nthreads = 0;
	_httpserver = NULL;
}

HttpMulThreads::HttpMulThreads(HttpServer *httpserver) {
	_nthreads = 0;
	_threads_ptr = NULL;
	_httpserver = httpserver;
}

HttpMulThreads::~HttpMulThreads() {

}

bool HttpMulThreads::Init() {

	return true;
}

bool HttpMulThreads::Init(HttpServer *httpserver) {
	_httpserver = httpserver;

	return true;
}


void HttpMulThreads::Loop() {
	int client_fd;
	socklen_t client_addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *client_addr_ptr;
//	HttpServer *httpserver = (HttpServer *)arg;

	client_addr_ptr = new struct sockaddr_in;
	while(true) {
		//  accept http request and Handle the request;
		client_fd = _httpserver->Accept((struct sockaddr *)client_addr_ptr,
				&client_addrlen);
		MulThreadMsg *msg = new MulThreadMsg(client_fd, _httpserver);
		pthread_t ptid;
		if (pthread_create(&ptid,
					NULL,
					&Start_rtn,
					(void *)msg) != 0) {
			return ;	
		}
	}
}


pthread_mutex_t &HttpMulThreads::GetClientMutex() {
	return MulThreads::GetClientMutex();
}

void *HttpMulThreads::Start_rtn(void *arg) {
	pthread_detach(pthread_self());
	MulThreadMsg *msg = (MulThreadMsg *)arg;
	char command[BUFFSIZE];
	read(msg->GetClientFd(), command, BUFFSIZE);
	//		if (strlen(command) <= 0) {
	//			continue;
	//		}
#ifdef DEBUG
	printf("Start_rtn command : \n%s\n", command);
#endif	// !DEBUG
	pthread_mutex_lock(&GetClientMutex());
	std::string content;
	std::string respone(msg->GetServer()->Handle(command, content));
	std::string res(msg->GetServer()->GetHttpResponseHead("HTTP/1.1",
				"200",
				"OK"));
	static const std::string cookie_str("Set-Cookie: respone=");
	static const std::string content_len_str("Content-Length: ");

	HttpServer::AddFieldInHttpResponseHead(res,
			content_len_str + SizeToString(content.size()));
	HttpServer::AddFieldInHttpResponseHead(res, cookie_str + respone);
	write(msg->GetClientFd(), res.c_str(), res.size());
	//  TO DO : add your code
	pthread_mutex_unlock(&GetClientMutex());
	close(msg->GetClientFd());
	
	// 不能在其他地方delete，否则的话就会在线程没有执行完之前释放空
	delete msg;

	return NULL;
}

std::string HttpMulThreads::SizeToString(const std::string::size_type &len) {
	static char buf[16];
	sprintf(buf, "%ld", len);
	
	return std::string(buf); 
}
