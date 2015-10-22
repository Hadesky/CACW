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

#ifdef DEBUG
#include <cstdio>
#endif // ! DEBUG

namespace {
	const int BUFFSIZE = 4096;
}

HttpMulThreads::HttpMulThreads() {
	_nthreads = 0;
	_httpserver = NULL;
	_threads_ptr = NULL;
}

HttpMulThreads::HttpMulThreads(HttpServer *httpserver) {
	_nthreads = 0;
	_threads_ptr = NULL;
	_httpserver = httpserver;
}

HttpMulThreads::~HttpMulThreads() {
	delete _threads_ptr;
}

bool HttpMulThreads::Init() {

	return true;
}

bool HttpMulThreads::Init(HttpServer *httpserver) {
	_httpserver = httpserver;

	return true;
}

bool HttpMulThreads::Create(const unsigned int nthreads) {
#ifdef DEBUG
	printf("HttpMulThreads Creating %d threads\n", nthreads);
#endif // ! DEBUG 
	_nthreads = nthreads;
	_threads_ptr = new pthread_t[_nthreads];
	for(unsigned int i = 0; i < nthreads; ++i) {
		if (pthread_create(&_threads_ptr[i],
					NULL,
					&Start_rtn,
					(void *)_httpserver) != 0) {
			return false;	
		}
	}

	return true;
}

void HttpMulThreads::Loop() {
#ifdef DEBUG
	printf("HttpMulThreads main thread is looping\n");
#endif // ! DEBUG
	Thread(_httpserver);
}


pthread_mutex_t &HttpMulThreads::GetClientMutex() {
	return MulThreads::GetClientMutex();
}

void *HttpMulThreads::Start_rtn(void *arg) {
	pthread_detach(pthread_self());
	Thread((HttpServer *)arg);

	return NULL;
}

std::string HttpMulThreads::SizeToString(const std::string::size_type &len) {
	static char buf[16];
	sprintf(buf, "%ld", len);
	
	return std::string(buf); 
}

void HttpMulThreads::Closeall() {
	MulThreads::Closeall();
}

void HttpMulThreads::Thread(HttpServer *httpserver) {
	int client_fd;
	socklen_t client_addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *client_addr_ptr;
//	HttpServer *httpserver = (HttpServer *)arg;
	char command[BUFFSIZE];

	client_addr_ptr = new struct sockaddr_in;
	while(true) {
		pthread_mutex_lock(&GetClientMutex());
		//  accept http request and Handle the request;
		client_fd = httpserver->Accept((struct sockaddr *)client_addr_ptr, &client_addrlen);
		read(client_fd, command, BUFFSIZE);
#ifdef DEBUG
		printf("Start_rtn command : \n%s\n", command);
#endif	// !DEBUG
		std::string content;
		std::string respone(httpserver->Handle(command, content));
		std::string res(httpserver->GetHttpResponseHead("HTTP/1.1",
					"200",
					"OK"));
		static const std::string cookie_str("Set-Cookie: respone=");
		static const std::string content_len_str("Content-Length: ");

		HttpServer::AddFieldInHttpResponseHead(res, content_len_str + SizeToString(content.size()));
		HttpServer::AddFieldInHttpResponseHead(res, cookie_str + respone);
		write(client_fd, res.c_str(), res.size());
		//  TO DO : add your code
		pthread_mutex_unlock(&GetClientMutex());
		close(client_fd);
	}
}

