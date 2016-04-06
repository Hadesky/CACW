/********************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 10.11.2015
 * ******************************************************************/

#ifndef _MAIL_LOG_H
#define _MAIL_LOG_H

#include <string>
#include <time.h>

namespace waponx {

	/*
	 * gain current system time.
	 * @return std::string current time at string format.
	 */
	std::string GetTime();

	int OpenLogFile();
	void log (const char *str);

	class Log {
		public :
			/*
			 * open a file to save log information.
			 * @param path be opened file.
			 */
			Log(const char *path);
			
			/*
			 * if _fd is not close, the function will close it.
			 */
			~Log();

			/*
			 * log mail
			 * @param	str your extra description.
			 */
			void Appand(const char *str);

			/*
			 * provide a interface to close log file's file description.
			 */
			void CloseFile();

		private :
			/*
			 * Open the default log file.
			 * @return file description. if fail to open, return -1.
			 */
			int OpenLogFile(const char *path);

			void Init();

		private :
			int _fd;
			bool _isclosed;
	};

}	// namespace waponx

#endif	// _MAIL_LOG_H 
