/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/4/2015
 * ********************************************************/


#ifndef _SERCER_MULTHREADS_H
#define _SERCER_MULTHREADS_H

#include <pthread.h>

class HttpServer;

class MulThreads {
public :
	MulThreads();
	virtual ~MulThreads();
	virtual bool Init();
	virtual bool Create(const unsigned int nthreads) = 0;
	virtual void Loop() = 0;
	virtual void Closeall() = 0;
protected :
	static void *Start_rtn(void *);
public :
	static pthread_mutex_t s_client_mutex;
	unsigned int _nthreads;
	pthread_t *_threads_ptr;
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
protected :
	static void *Start_rtn(void *arg);
private :
	HttpServer *_httpserver;

};


#endif // !  _SERCER_MULTHREADS_H
