/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/4/2015
 * ********************************************************/


#ifndef _SERVER_MULTHREADS_H
#define _SERVER_MULTHREADS_H

#include "value.h"

#include <pthread.h>
#include <string>


class HttpServer;

class MulThreads {
public :
	MulThreads();
	virtual ~MulThreads();
	virtual bool Init();
	virtual bool Create(const unsigned int nthreads) = 0;
	virtual void Loop() = 0;
	virtual void Closeall() = 0;
	static pthread_mutex_t &GetClientMutex();
protected :
	static void *Start_rtn(void *);
private :
	DISALLOW_COPY_AND_ASSIGN(MulThreads);
public :
	unsigned int _nthreads;
	pthread_t *_threads_ptr;
};

class MulThreadMsg {
public :

priavte :
	int _clientfd;
	HttpServer *_server;
};

class HttpMulThreads: public MulThreads {
public :
	HttpMulThreads();
	HttpMulThreads(HttpServer *httpserver);
	virtual ~HttpMulThreads();
	virtual bool Init();
	virtual bool Init(HttpServer *httpserver);
	virtual bool Create(const unsigned int nthreads);
	virtual void Loop();
	virtual void Closeall();
	inline static pthread_mutex_t &GetClientMutex();
protected :
	static void *Start_rtn(void *arg);
private :
	DISALLOW_COPY_AND_ASSIGN(HttpMulThreads);
	static std::string SizeToString(const std::string::size_type &len);
	static void Thread(HttpServer *httpserver);
private :
	HttpServer *_httpserver;
};


#endif // !  _SERVER_MULTHREADS_H
