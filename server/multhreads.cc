#include "multhreads.h"

#ifdef DEBUG
#include <cstdio>
#endif // ! DEBUG


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

//bool MulThreads::Create(const unsigned int nthreads) {
//#ifdef DEBUG
//	printf("MulThreads creating %d threads...\n", nthreads);
//#endif // ! DEBUG
//	_nthreads = nthreads;
//	_threads_ptr = new pthread_t[_nthreads];
//	for(unsigned int i = 0; i < nthreads; ++i) {
//		if (pthread_create(&_threads_ptr[i], NULL, &Start_rtn, NULL) != 0) {
//			return false;	
//		}
//	}
//
//	return true;
//}

void MulThreads::Loop() {
#ifdef DEBUG
	printf("MulThreads main thread looping...\n");
#endif // ! DEBUG
	for(; ; ){
	
	}
}

//void MulThreads::Closeall() {
//	for (unsigned int i = 0; i < _nthreads; ++i) {
//		pthread_cancel(_threads_ptr[i]);
//	}
//}



pthread_mutex_t &MulThreads::GetClientMutex() {
	static pthread_mutex_t s_client_mutex = PTHREAD_MUTEX_INITIALIZER;
	return s_client_mutex;
}

void *MulThreads::Start_rtn(void *arg) {
	return NULL;	
}
