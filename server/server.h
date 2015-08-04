/********************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.22.2015
 * ******************************************************************/

#ifndef _SERVER_H
#define _SERVER_H

#include <boost/shared_ptr.hpp>
#include <netdb.h>
#include <stdlib.h>
#include <string>

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &);\
	void operator=(const TypeName &)
#endif


class RegisterAction;
class EnrollAction;
class MulThreads;

class Server {
public :
	Server();
	virtual ~Server();
	virtual bool Init(int sockfd = -1, struct sockaddr_in *addr_ptr = NULL);
	virtual void Start() = 0;
	virtual void SetSocketfd(int sockfd) = 0;
	virtual int GetSockfd() = 0;
	virtual struct sockaddr_in *GetSocketAddress() = 0;
	virtual bool Register(std::string name,
						  std::string password) = 0;
	virtual bool Enroll(std::string name,
						std::string password) = 0;
	virtual void SetRegister(RegisterAction *ract_ptr) = 0;
	virtual void SetEnroll(EnrollAction *eact_ptr) = 0;
	virtual RegisterAction *GetRegister() const = 0;
	virtual EnrollAction *GetEnroll() const = 0;
private :
	DISALLOW_COPY_AND_ASSIGN(Server);

public :
	int _sockfd;
	struct sockaddr_in *_addr_ptr;
	RegisterAction *_registeraction_ptr;
	EnrollAction *_enrollaction_ptr;
	MulThreads *_multhreads_ptr;
};

class SimpleMySql;
#ifndef BUFSIZE
#define BUFSIZE 4096
#endif	//  ! BUFSIZE

#ifndef QLEN
#define QLEN 10
#endif	//  ! QLEN


class HttpServer: public Server {
public :
	HttpServer();
	virtual ~HttpServer();
	virtual bool Init(int sockfd = -1, struct sockaddr_in *addr_ptr = NULL);
	virtual void Start();
	virtual void SetSocketfd(int sockfd);
	virtual int GetSockfd();
	virtual struct sockaddr_in *GetSocketAddress();
	virtual bool Register(std::string name,
						  std::string password);
	virtual bool Enroll(std::string name,
						std::string password);
	void SetReuseAddr(bool flag);
	bool IsReuseAddr();
	void Get(const std::string url);
	void Post(const std::string url);
	bool Handle(const std::string request);
	bool Handle(char *request);
	std::string GetURL(const std::string request);
	bool GetURL(const std::string request, std::string &url);
	virtual void SetRegister(RegisterAction *ract_ptr);
	virtual void SetEnroll(EnrollAction *eact_ptr);
	virtual RegisterAction *GetRegister() const;
	virtual EnrollAction *GetEnroll() const;
	int Socket();
	int Accept(struct sockaddr *addr_ptr, socklen_t *len_ptr);
	bool Bind();
	bool Listen(const int qlen);

private :
	DISALLOW_COPY_AND_ASSIGN(HttpServer);

protected :
	socklen_t _isreuseaddr;
	boost::shared_ptr<SimpleMySql> _spmysql_ptr;
};

#endif
