/* ********************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * *********************************************************/

#include "md5.h"
#include "registeraction.h"
#include "simplemysql.h"

#ifdef DEBUG
#include <cstdio>
#endif	// !DEBUG


namespace {
	unsigned int ABS(int a) {
		return a > 0? a: -a;
	}
}

HttpRegisterAction::HttpRegisterAction() 
	: _atctree(InitAtCTree()) {
	
}

HttpRegisterAction::~HttpRegisterAction() {
	delete _atctree;
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
	std::string command = "python mail.py " + email + " " + code;
	system(command.c_str());
	return std::string("000");
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
