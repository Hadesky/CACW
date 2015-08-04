#include "multhreads.h"

pthread_mutex_t MulThreads::s_client_mutex = PTHREAD_MUTEX_INITIALIZER;

MulThreads::MulThreads() {
	
}

MulThreads::~MulThreads() {
	delete _threads_ptr;
}

bool MulThreads::Init() {
	_nthreads = 0;
	_threads_ptr = NULL;

	return true;
}

bool MulThreads::Create(const unsigned int nthreads) {
	_nthreads = nthreads;
	_threads_ptr = new pthread_t[_nthreads];
	for(unsigned int i = 0; i < nthreads; ++i) {
		if (pthread_create(&_threads_ptr[i], NULL, &Start_rtn, NULL) != 0) {
			return false;	
		}
	}

	return true;
}

void MulThreads::Loop() {
	for(; ; ){
	
	}
}

void MulThreads::Closeall() {
	for (unsigned int i = 0; i < _nthreads; ++i) {
		pthread_cancel(_threads_ptr[i]);
	}
}

void *MulThreads::Start_rtn(void *arg) {
	return NULL;	
}
