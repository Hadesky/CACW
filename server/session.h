/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/4/2015
 * ********************************************************/

#ifndef _SERVER_SESSION_H_
#define _SERVER_SESSION_H_

#include "value.h"

#include <ctime>
#include <string>

using std::string;


class SessionNode;
typedef SessionNode *SessionNodePtr;

class SessionNode {
public :
	SessionNode();
	explicit SessionNode(const string &sid);
	~SessionNode();
	bool Init();
	//  invoke MD5 function to set field sessionid
	string SetSessionID();
	bool IsTimeout();
private :
	DISALLOW_COPY_AND_ASSIGN(SessionNode);
public :
	string _sessionid;
	string _userid;
	time_t _starttime;
	time_t _endtime;
	//const struct timeval _duration;
	SessionNodePtr _lchild;
	SessionNodePtr _rchild;
};

class Session {
public :
	//Session();
	~Session();
	static Session &GetInstance();
	static time_t GetDuration();
	void Insert(const string &sid);
	SessionNodePtr Delete(const string &sid);
	// 访问session，如果这个id不存在或过期，则返回false
	bool Visit(const string &sid) const;
	void SetTimeDuration(const time_t &d);
private :
	bool Init();
	explicit Session(const time_t &d);
	DISALLOW_COPY_AND_ASSIGN(Session);
	SessionNodePtr Search(const string &sid) const;
	SessionNodePtr FindSequela(SessionNodePtr p);
	SessionNodePtr ChangeChild(const SessionNodePtr &parent,
		const SessionNodePtr &oldchild,
		const SessionNodePtr &newchild);
private :
	SessionNode *_root;
	const time_t _duration;
};


#endif // !  _SERVER_SESSION_H_ 
