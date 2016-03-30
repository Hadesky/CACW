/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/4/2015
 * ********************************************************/

#include "session.h"

SessionNode::SessionNode()
	:	_sessionid(""),
		_userid(""),
		_starttime(),
		_endtime(),
		//_duration(),
		_lchild(NULL),
		_rchild(NULL){
		
}

SessionNode::SessionNode(const string &sid)
	:	_sessionid(sid),
		_userid(""),
		_starttime(time(NULL)),
		_endtime(_starttime + Session::GetDuration()),
		_lchild(NULL),
		_rchild(NULL) {
	
}

SessionNode::~SessionNode() {
	delete _lchild;
	delete _rchild;
}

bool SessionNode::Init() {
	// Add your code.
	
	return true;
}

string SessionNode::SetSessionID() {
	// Use MD5 algorithm to get sessionid and set it
	return string("");
}

bool SessionNode::IsTimeout() {
	return time(NULL) > _endtime? true: false;
}
