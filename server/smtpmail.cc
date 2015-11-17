/********************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 10.11.2015
 * ******************************************************************/

#include "base64encode.h"
#include "log.h"
#include "smtpmail.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>


namespace {
	const std::string SMTPPORT("25");
}


namespace Mail {
	SMTPMail::SMTPMail (const String &username,
		const String &password,
		const String &host) :
	_islogin(false),
	_reuseaddr(true),
	_fd(-1),
	_paddrfilter(InitAddrInfoFilter()),
	_username(username),
	_password(Base64Encode(password)),
	_host(host){
		//_paddrfilter = InitAddrInfoFilter();
	
	}


	SMTPMail::~SMTPMail() {
		if (_fd > 0) {
			close(_fd);
		}
		if (_paddrfilter != NULL) {
			freeaddrinfo(_paddrfilter);
		}
	}

	SMTPMail::AddrType *SMTPMail::InitAddrInfoFilter() {
		AddrType *p = new AddrType;
		if (p == NULL) {
			return NULL;
		}
		memset(p, 0, sizeof(AddrType));
		//p->ai_flags = AI_CANONNAME;
		p->ai_family = AF_UNSPEC;
		p->ai_socktype = SOCK_STREAM;	
		struct addrinfo *res = NULL;
		int n = getaddrinfo(_host.c_str(), SMTPPORT.c_str(), p, &res);
		if (n != 0) {
			const char *str = gai_strerror(n);
			log(str, strlen(str));
			return NULL;
		}
		if (!Socket(res)) {
			freeaddrinfo(res);
			const char *str = strerror(errno);
			log(str, strlen(str));
			return NULL;
		}

		return p;
	}

	bool SMTPMail::Socket(AddrType *res) {
		while (res != NULL) {
			_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			if (_fd > 0) {
				if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &_reuseaddr, sizeof(_reuseaddr))  == 0) {
					if (connect(_fd, res->ai_addr, sizeof(struct sockaddr)) == 0){
						return true;
					}
				}
				close(_fd);
				_fd = -1;
			}
			res = res->ai_next;
		}
		return false;	
	}
	
	bool SMTPMail::Login() {
		Send(String("EHLO "+ _username +"\r\n"));
		if (!(Check("EHLO", Recv(), "250"))) {
			return false;		
		}
		
		Send(String("auth login\r\n"));
		if (!Check("auth login", Recv(), "334")) {
			return false;
		}
		
		Send(Base64Encode(_username) + "\r\n");
		if (!Check("enter username", Recv(), "334")) {
			return false;
		}

		Send(String(_password + "\r\n"));
		if (!Check("enter password", Recv(), "235")) {
			return false;
		}

		return true;
	}

	void SMTPMail::SendEmail(const String &to, const String &text) {
		if (false == _islogin) {
			return ;
		}

		Check("start", Recv(), "220");
		if (!Login()) {
			return ;
		}

		Send(String("MAIL FROM: <" + _username + ">\r\n"));
		if (!Check("MAIL FROM: ", Recv(), "250")){
			return ;
		}
	
		Send(String("RCPT TO: <" + to + ">\r\n"));
		if (!Check("RCPT TO: ", Recv(), "250")) {
			return ;
		}
		
		Send(String("DATA\r\n"));
		if (!Check("DATA: ", Recv(), "354")) {
			return ;
		}

		Send(String("to:" + to + "\r\n" + text + "\r\n.\r\n"));
		if (!Check("to : ", Recv(), "250")) {
			return ;
		}

		Send(String("QUIT\r\n"));
		if (!Check("QUIT", Recv(), "221")) {
			return ;
		}
	}

	void SMTPMail::Send(const String &msg) {
		if(send(_fd, msg.c_str(), msg.length(), MSG_CONFIRM) < 0) {
			const char *str = "send error\n";
			log(str, strlen(str));
		}
	}

	SMTPMail::String SMTPMail::Recv() {
		const size_t buflen = 512;
		static char buf[buflen];
		if (recv(_fd, buf, buflen, 0) < 0) {
			const char *str = "recv error\n";
			log(str, strlen(str));

			return String("");
		}

		return String(buf);
	}
	bool SMTPMail::Check(const String &cmd,
			const String &res,
			const String &mode) {
		if (res.substr(0, 3) != mode) {
			String l (cmd + " command error: " + res); 
			log(l.c_str(), l.length());
			return false;
		}
		return true;
	};
}

