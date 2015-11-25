/* ********************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * *********************************************************/

#include "md5.h"
#include "registeraction.h"
#include "simplemysql.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#ifdef DEBUG
#include <cstdio>
#endif	// !DEBUG


namespace {
	unsigned int ABS(int a) {
		return a > 0? a: -a;
	}

	// 定义一个地址类型
	typedef struct addrinfo AddrType;
	//	从地址中获取套接字，如果成功链接就返回对应的套接字，如果没有就返回-1
	int Socket(AddrType *res) {
		int fd;
//		bool reuseaddr = true;
		while (res != NULL) {
			fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			if (fd > 0) {
//				if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(bool))  == 0) {
//					if (connect(fd, res->ai_addr, sizeof(struct sockaddr)) == 0){
//						return fd;
//					}
//				}
				if (connect(fd, res->ai_addr, sizeof(struct sockaddr)) == 0) {
					return fd;
				}
				close(fd);
				fd = -1;
			}
			res = res->ai_next;
		}

		return fd;
	}
	//  修改php服务的地址，如果flag为true则释放空间，返回NULL
	//	如果地址（res）为NULL，就重新获取php服务器地址
	AddrType *DoAddr(bool flag) {
		static struct addrinfo *res = NULL;
		if (flag == true) {
			if (res != NULL) {
				freeaddrinfo(res);
			}
			return NULL;
		}
		if (res != NULL) {
			return res;
		}
		AddrType *p = new AddrType;
		if (p == NULL) {
			return NULL;
		}
		memset(p, 0, sizeof(AddrType));
		//p->ai_flags = AI_CANONNAME;
		p->ai_family = AF_UNSPEC;
		p->ai_socktype = SOCK_STREAM;

		int n = getaddrinfo("dzyone.sinaapp.com", "80", p, &res);
		if (n != 0) {
			printf("addrinfo error: %s\n", gai_strerror(n));
			if (res != NULL) {
				freeaddrinfo(res);
			}
			return NULL;
		}
		if (NULL == res) {
#ifdef DEBUG 
			printf("HttpRegisterAction::Start_rtn_sendmail:\n addr pionter is NULL \n");
#endif 
		}

		return res;
	}
}

HttpRegisterAction::HttpRegisterAction() 
	: _atctree(InitAtCTree()) {
	
}

HttpRegisterAction::~HttpRegisterAction() {
	delete _atctree;
	DoAddr(true);
}

bool HttpRegisterAction::Init(boost::shared_ptr<SimpleMySql> spmysql_ptr) {
	_spmysql_ptr = spmysql_ptr;

	return true;
}

std::string HttpRegisterAction::Register(const std::string &name,
		const std::string &password,
		const std::string &email,
		const std::string &sex,
		const std::string &authcode){
	std::vector<std::map<std::string, std::string> > row;

	if (_spmysql_ptr->Search(std::string("CUser"), std::string("username"), name) 
			&& _spmysql_ptr->GetAllResult(_spmysql_ptr->GetUseResult(), row)) {
#ifdef DEBUG
		printf("HttpRegisterAction::Register account is exist or search error\n");
#endif	// !DEBUG			

		_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
		return std::string("001");
	}

	if (!_atctree->Search(email, authcode)) {
		return std::string("001");
	}
	static const string field("username,password,sex,email");
#ifdef DEBUG
	printf("HttpRegisterAction::Register\nfield : %s\n", field.c_str());
#endif	// ! DEBUG
	const string values = "\"" + name + "\",\"" +password +"\",\"" +
		sex + "\",\"" + email + "\""; 

#ifdef DEBUG
	printf("HttpRegisterAction::Register\nvalues : %s\n", values.c_str());
#endif	// ! DEBUG
	_spmysql_ptr->Insert(string("CUser"), field, values);
	return std::string("000");
}

HttpRegisterAction::AuthCodeTree *HttpRegisterAction::InitAtCTree() {
	return new AuthCodeTree();
}


std::string HttpRegisterAction::GetAuthCode(const std::string &email) {
	if (email.empty()) {
		return std::string("001");
	}
	std::string code = AuthCodeTree::GetAuthCode();
	if (!_atctree->Add(email, code)) {
		//	往AuthCode数据结构中插入数据
		return std::string("001");
	}
//	std::string command = "python mail.py " + email + " " + code;
//	system(command.c_str());
	
//	Msg *msg = new Msg(email, code);
//	pthread_t ptid;
//	if (pthread_create(&ptid, NULL, &Start_rtn_sendmail, (void *)msg) != 0) {
//#ifdef DEBUG
//		printf("fail to create phread \n");
//#endif
//		return std::string("001");
//	}
	Msg *msg = new Msg(email, code);
	Start_rtn_sendmail((void *)msg);
	delete msg;

	return std::string("000");
}

void *HttpRegisterAction::Start_rtn_sendmail(void *arg) {
//	pthread_detach(pthread_self());
	Msg *msg = (Msg *) arg;
	int clientfd;

	if ( (clientfd = Socket(DoAddr(false))) < 0) {
#ifdef DEBUG 
		printf("HttpRegisterAction::Start_rtn_sendmail:\nconnect error \n");
#endif
		return NULL;
	}	
//	struct sockaddr_in server_addr;
//	memset(&server_addr, 0, sizeof(server_addr));
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_port = htons(80);
//	server_addr.sin_addr.s_addr = inet_addr("dzyone.sinaapp.com");
//	
//	clientfd = socket(AF_INET, SOCK_STREAM, 0);
//
//#ifdef DEBUG 
//	printf("HttpRegisterAction::Start_rtn_sendmail:\nclientfd is %d\n", clientfd);
//#endif
//	if (connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
//#ifdef DEBUG 
//		printf("HttpRegisterAction::Start_rtn_sendmail:\nconnect error %d\n", clientfd);
//#endif 
//		close(clientfd);
//	}
	std::string request("\
POST /email.php HTTP/1.1\r\n\
Host: dzyone.sinaapp.com\r\n");
	const std::string email("email: " + msg->_email + "\r\n");
	const std::string code("code: " + msg->_authcode + "\r\n");
	request += email + code;
	request += "Content-Length: 0\r\n\r\n";
#ifdef DEBUG
	printf("send to :\n%s", request.c_str());
#endif 
	send(clientfd, request.c_str(), request.length(), MSG_CONFIRM);

	return NULL;
}

// ******************************************************************
// AuthCodeNode implement

HttpRegisterAction::AuthCodeNode::AuthCodeNode(
		AuthCodeNode *const parent,
		AuthCodeNode *const next,
		const std::string &email,
		const std::string &code): _email(email),
	_authcode(code),
	_parent(parent),
	_next(next) {

}

HttpRegisterAction::AuthCodeNode::~AuthCodeNode() {
	delete _next;
}

HttpRegisterAction::AuthCodeNode *HttpRegisterAction::AuthCodeNode::GetNext(){
	return _next;
}

HttpRegisterAction::AuthCodeNode *HttpRegisterAction::AuthCodeNode::GetParent() {
	return _parent;
}
std::string HttpRegisterAction::AuthCodeNode::GetEmail() {
	return _email;
}

std::string HttpRegisterAction::AuthCodeNode::GetAuthCode() {
	return _authcode;
}
void HttpRegisterAction::AuthCodeNode::SetNext(AuthCodeNode *const next) {
	_next = next;
}

void HttpRegisterAction::AuthCodeNode::SetParent(AuthCodeNode *const p) {
	_parent = p;
}

void HttpRegisterAction::AuthCodeNode::SetEmail(const std::string &email) {
	_email = email;
}

void HttpRegisterAction::AuthCodeNode::SetAuthCode(const std::string &code) {
	_authcode = code;
}

// end AuthCodeNode implement
// ******************************************************************


// ******************************************************************
// AuthCodeTree implement

HttpRegisterAction::AuthCodeTree::AuthCodeTree() {
	_root = new AuthCodeNode(NULL, NULL, "", "");
}

HttpRegisterAction::AuthCodeTree::~AuthCodeTree() {
	delete _root;
}

std::string HttpRegisterAction::AuthCodeTree::GetAuthCode() {
	std::string code;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < HttpRegisterAction::AuthCodeTree::AUTHCODELENGTH; ++i) {
		code.push_back(rand() % 10 + '0');
	}

	return code;
}

bool HttpRegisterAction::AuthCodeTree::Search(const std::string &email,
		const std::string &code) {
	return Delete(email, code);
}

bool HttpRegisterAction::AuthCodeTree::Add(const std::string &email) {
	return Insert(email, GetAuthCode());
}

bool HttpRegisterAction::AuthCodeTree::Add(const std::string &email, const std::string &code) { 
	return Insert(email, code);
}


bool HttpRegisterAction::AuthCodeTree::Insert(const std::string &email,
		const std::string &authcode) {
	AuthCodeNode *p = _root;
	if (NULL == p) {
		p = new AuthCodeNode(NULL, NULL, "", "");
	}

	while (p->GetNext() != NULL) {
		if (p->GetNext()->GetEmail() == email) {
			p->GetNext()->SetAuthCode(authcode);
			return true;
		}
	}
	AuthCodeNode *newnode = new AuthCodeNode(p, NULL, email, authcode);
	p->SetNext(newnode);
	
	return true;
}

bool HttpRegisterAction::AuthCodeTree::Delete(const std::string &email, const std::string &authcode) {
	AuthCodeNode *p = _root;
	if (p == NULL) {
		p = new AuthCodeNode(NULL, NULL, "", "");
	}

	while (p->GetNext() != NULL) {
		if (p->GetNext()->GetEmail() == email && p->GetNext()->GetAuthCode() == authcode) {
			AuthCodeNode *oldnode = p->GetNext();
			oldnode->GetNext()->SetParent(p);
			p->SetNext(oldnode->GetNext());
			delete oldnode;
			return true;
		}
	}

	return false;
}

// end AuthCodeTree implement
// ******************************************************************
