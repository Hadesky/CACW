/********************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 10.11.2015
 * ******************************************************************/

#include "log.h"


#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>


namespace {
	pthread_mutex_t log_file_mutex;
	pthread_mutex_t log_class_mutex = PTHREAD_MUTEX_INITIALIZER;
}

namespace waponx {

	std::string GetTime() {
		time_t tt = time(NULL);

		return std::string(ctime(&tt));
	}
	
	int OpenLogFile() {
		int fd = 0;
		if ( (fd = open("../.log/mail.log", O_WRONLY | O_APPEND)) == -1) {
			printf("open log file error!\n");
			printf("please, make sure have a file called mail.log in the path\n");

			return -1;
		}

		return fd;	
	}
	
	
	void log(const char *str) {
		static int log_file_fd = OpenLogFile();

		std::string res(GetTime());
		res += "\t";
		res += str;
		res += "\n";

		pthread_mutex_lock(&log_file_mutex);
		write (log_file_fd, res.c_str(), res.length());
		pthread_mutex_unlock(&log_file_mutex);
		
	}

	Log::Log(const char *path)
	:	_fd(OpenLogFile(path)),
		_isclosed(false) {
		Init();	
	}

	Log::~Log() {
		if (false == _isclosed) {
			close(_fd);
		}
	}

	void Log::Init() {

	}

	int Log::OpenLogFile(const char *path) {
		int fd = 0;
		pthread_mutex_lock(&log_class_mutex);
		fd = open(path, O_WRONLY | O_APPEND);
		pthread_mutex_unlock(&log_class_mutex);
		if (fd == -1) {
			printf("open log file error!\n");
			printf("please, make sure have a file called mail.log in the path\n");

			return -1;
		}

		return fd;
	}

	void Log::Appand(const char *str) {
		std::string res(GetTime());
		res =  res + str + "\n";

		pthread_mutex_lock(&log_class_mutex);
		write (_fd, res.c_str(), res.length());
		pthread_mutex_unlock(&log_class_mutex);
	}
}
