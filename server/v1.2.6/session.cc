/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/30/2015
 * ********************************************************/

#include "session.h"

namespace {
const time_t d = 30 * 60;
}



Session::Session(const time_t &d) 
	: _duration(d) {

}

Session::~Session() {
	delete _root;
}

Session &Session::GetInstance() {
	static Session s(d);

	return s;
}

bool Session::Init() {
	return true;
}

time_t Session::GetDuration() {
	return d;
}

void Session::Insert(const string &sid) {
	SessionNodePtr p = _root;
	SessionNodePtr parent = NULL;
	if (NULL == p) {
		_root = new SessionNode(sid);
		_root->Init();

		return ;
	}
	while (p != NULL) {
		parent = p;
		if (sid < p->_sessionid && p->_lchild != NULL) {
			p = p->_lchild;
		} 
		else if (sid >= p->_sessionid && p->_rchild != NULL) {
			p = p->_rchild;
		}
	}
	if (sid < p->_sessionid) {
		parent->_lchild = new SessionNode(sid);
		parent->_lchild->Init();
	} else {
		parent->_rchild = new SessionNode(sid);
		parent->_rchild->Init();
	}
}

SessionNodePtr Session::Delete(const string &sid) {
	SessionNodePtr p = _root;
	SessionNodePtr parent = NULL;
	while (p != NULL) {
		if (p->_sessionid == sid) {
			break;
		}
		parent = p;
		if (sid < p->_sessionid) {
			p = p->_lchild;
		} else {
			p = p->_rchild;
		}
	}
	if (NULL == p) {
		return p;
	}
	else if (p->_lchild != NULL && p->_rchild != NULL) {
		SessionNodePtr s = FindSequela(p);
		if (p->_rchild == s) {
			s->_lchild = p->_lchild;
			if (parent != NULL && parent->_lchild == p) {
				parent->_lchild = s;
			} 
			else if (parent != NULL && parent->_rchild == p){
				parent->_rchild = s;
			}
			return p;
		}
	}
	else {
		if (p->_lchild == NULL && p->_rchild == NULL) {
			return ChangeChild(parent, p, NULL);
		} 
		else if (p->_lchild != NULL) {
			return ChangeChild(parent, p ,p->_lchild);
		}
		else {
			return ChangeChild(parent, p, p->_rchild);
		}
	}

	return NULL;
}

bool Session::Visit(const string &sid) const{
	SessionNodePtr p = Search(sid);
	if (NULL == p) {
		return false;
	}

	return !p->IsTimeout();
}

SessionNodePtr Session::Search(const string &sid) const {
	SessionNodePtr p = _root;
	while (p != NULL) {
		if (p->_sessionid == sid) {
			return p;
		}
		else if (sid < p->_sessionid) {
			p = p->_lchild;
		}
		else {
			p = p->_rchild;
		}
	}

	return NULL;
}

SessionNodePtr Session::FindSequela(SessionNodePtr p) {
	while (p->_lchild != NULL) {
		p = p->_lchild;
	}
	
	return p;
}

SessionNodePtr Session::ChangeChild(const SessionNodePtr &parent,
		const SessionNodePtr &oldchild,
		const SessionNodePtr &newchild) {
	if (NULL == parent) {
		return NULL;
	}
	if(parent->_lchild == oldchild) {
		parent->_lchild = newchild;
	}
	else if (parent->_rchild == oldchild) {
		parent->_rchild = newchild;
	}
	else {
		return NULL;
	}

	return oldchild;
}
