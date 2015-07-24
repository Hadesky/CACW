/********************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.22.2015
 * ******************************************************************/

#ifndef _SERVER_H
#define _SERVER_H

#include <netdb.h>
#include <stdlib.h>

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &);\
	void operator=(const TypeName &)
#endif

class SimpleMySql;


class Server {
public :
	Server();
	virtual ~Server();
	bool Init(int sockfd = -1, struct sockaddr_in *addr_ptr = NULL);
	virtual void Start() = 0;
	virtual void SetSocketfd(int sockfd) = 0;
	virtual int GetSockfd() = 0;
	virtual struct sockaddr_in *GetSocketAddress() = 0;
private :
	DISALLOW_COPY_AND_ASSIGN(Server);

public :
	int _sockfd;
	struct sockaddr_in *_addr_ptr;
};



class HttpServer: public Server {
public :
	HttpServer();
	virtual ~HttpServer();
	bool Init(int sockfd = -1, struct sockaddr_in *addr_ptr = NULL);
	virtual void Start();
	virtual void SetSocketfd(int sockfd);
	virtual int GetSockfd();
	virtual struct sockaddr_in *GetSocketAddress();
	void SetReuseAddr(bool flag);
	bool IsReuseAddr();
	void Get();
	void Post();

private :
	DISALLOW_COPY_AND_ASSIGN(HttpServer);
	int Socket();
	bool Bind();
	bool Listen(const int qlen);

public :
	socklen_t _isreuseaddr;
	SimpleMySql *_spmysql_ptr;
};

#endif
