/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/4/2015
 * ********************************************************/

#include "server.h"
#include "multhreads.h"

#include <assert.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>

HttpMulThreads::HttpMulThreads() {
	
}

HttpMulThreads::HttpMulThreads(HttpServer *httpserver) {
	_httpserver = httpserver;
}

HttpMulThreads::~HttpMulThreads() {
	delete _threads_ptr;
}

bool HttpMulThreads::Init() {
	_nthreads = 0;
	_threads_ptr = NULL;

	return true;
}

bool HttpMulThreads::Init(HttpServer *httpserver) {
	_nthreads = 0;
	_threads_ptr = NULL;
	_httpserver = httpserver;

	return true;
}

bool HttpMulThreads::Create(const unsigned int nthreads) {
	assert(_httpserver != NULL);
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
	int client_fd;
	socklen_t client_addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *client_addr_ptr;

	client_addr_ptr = new struct sockaddr_in;
	while(true) {
		pthread_mutex_lock(&s_client_mutex);
		client_fd = _httpserver->Accept((struct sockaddr *)client_addr_ptr, &client_addrlen);
		//  TO DO : add your code
		pthread_mutex_unlock(&s_client_mutex);
		close(client_fd);
	}	
}


void *HttpMulThreads::Start_rtn(void *arg) {
	pthread_detach(pthread_self());
	int client_fd;
	socklen_t client_addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *client_addr_ptr;
	HttpServer *httpserver = (HttpServer *)arg;

	client_addr_ptr = new struct sockaddr_in;
	while(true) {
		pthread_mutex_lock(&s_client_mutex);
		client_fd = httpserver->Accept((struct sockaddr *)client_addr_ptr, &client_addrlen);
		//  TO DO : add your code
		pthread_mutex_unlock(&s_client_mutex);
		close(client_fd);
	}
}

void HttpMulThreads::Closeall(){
	MulThreads::Closeall();
}

