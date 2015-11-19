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
	std::string code = AuthCodeTree::GetAuthCode();
	if (!_atctree->Add(email, code)) {
		//	往AuthCode数据结构中插入数据
		return std::string("001");
	}
	std::string command = "python mail.py " + email + " " + code;
	system(command.c_str());
	return std::string("000");
}


///////////////////////////////////////////////////////////
// class AuthCodeNode's implemention 

HttpRegisterAction::AuthCodeNode::AuthCodeNode(AuthCodeNode *const parent,
		const std::string &email,
		const std::string &authcode) 
	: _email(email),
	_authcode(authcode),
	_lchild(NULL),
	_rchild(NULL),
	_parent(parent) {

	}

HttpRegisterAction::AuthCodeNode::~AuthCodeNode() {
	delete _lchild;
	delete _rchild;
}

std::string HttpRegisterAction::AuthCodeNode::GetEmail() const {
	return _email;
}

std::string HttpRegisterAction::AuthCodeNode::GetAuthCode() const {
	return _authcode;
}

HttpRegisterAction::AuthCodeNode * HttpRegisterAction::AuthCodeNode::GetLeftChild() const {
	return _lchild;
}

HttpRegisterAction::AuthCodeNode * HttpRegisterAction::AuthCodeNode::GetRightChild() const {
	return _rchild;
}

HttpRegisterAction::AuthCodeNode *HttpRegisterAction::AuthCodeNode::GetParent() const{
	return _parent;
}

void HttpRegisterAction::AuthCodeNode::SetEmail(const std::string &email) {
	_email = email;		
}

void HttpRegisterAction::AuthCodeNode::SetAuthCode(const std::string &authcode) {
	_authcode = authcode;
}

void HttpRegisterAction::AuthCodeNode::SetLeftChild(AuthCodeNode *const p) {
	_lchild = p;
} 

void HttpRegisterAction::AuthCodeNode::SetRightChild(AuthCodeNode *const p) {
	_rchild = p;
}

void HttpRegisterAction::AuthCodeNode::SetParent(AuthCodeNode *const p) {
	_parent = p;
}

// end AuthCodeNode implemention
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//	AuthCodeTree implemention

HttpRegisterAction::AuthCodeTree::AuthCodeTree() 
	: _root(NULL) {

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

bool HttpRegisterAction::AuthCodeTree::Search(const std::string &email, const std::string &authcode) {
	return Delete(_root, email, authcode);
}

bool HttpRegisterAction::AuthCodeTree::Add(const std::string &email) {
	return Insert(NULL, _root, email, GetAuthCode());
}

bool HttpRegisterAction::AuthCodeTree::Add(const std::string &email,
		const std::string &code) {
	return Insert(NULL, _root, email, code);
}

bool HttpRegisterAction::AuthCodeTree::Insert(AuthCodeNode *parent,
		AuthCodeNode *p,
		const std::string &email,
		const std::string &authcode) {
	if (NULL == p) {
		p = new AuthCodeNode(parent, email,authcode);
		if (parent == NULL) {
			_root = p;
		} else {
			ChangeChild(parent, NULL, p);
		}
		return true;
	}
	if (email == p->GetEmail()) {
		p->SetAuthCode(authcode);
		return true;
	}
	else if (email < p->GetEmail()) {
		if(false == Insert(p, p->GetLeftChild(), email, authcode)) {
			return false;
		}
		if (2 == ABS(Height(p->GetLeftChild()) - Height(p->GetRightChild())) ) {
			//  上一步的Insert操作保证了左孩子不会为NULL
			if (email < p->GetLeftChild()->GetEmail()) {
				//  在左左的情况下调整
				RotateLeft(p);
			} else {
				//	在左右的情况下调整
				RotateLeft(p);
				RotateRight(p);
			}
		}
	}
	else {
		if (false ==  Insert(p, p->GetRightChild(), email, authcode)) {
			return false;
		}
		if (2 == ABS(Height(p->GetRightChild()) - Height(p->GetLeftChild())) ) {
			//  上一步的Insert操作保证了左孩子不会为NULL
			if (email > p->GetLeftChild()->GetEmail()) {
				//  在右右的情况下调整
				RotateRight(p);
			} else {
				//	在右左的情况下调整
				RotateRight(p);
				RotateLeft(p);
			}
		}

	}

	return true;

}


bool HttpRegisterAction::AuthCodeTree::Delete(AuthCodeNode *p,
		const std::string &email,
		const std::string &authcode) {
	if (NULL == p) {
		return false;
	}
	if (email == p->GetEmail()) {
		if(authcode == p->GetAuthCode()) {
			if (p->GetLeftChild() != NULL && p->GetRightChild() != NULL) {
				//	如果有两个孩子结点
				// 找到右子树中最小的结点
				AuthCodeNode *temp = p->GetRightChild();
				while (temp->GetLeftChild() != NULL) {
					temp = temp->GetLeftChild();
				}
				p->SetEmail(temp->GetEmail());
				p->SetAuthCode(temp->GetAuthCode());
				Delete(p->GetRightChild(), temp->GetEmail(), temp->GetAuthCode());
				if (2 == ABS(Height(p->GetLeftChild() - Height(p->GetRightChild()))) ) {
					//	如果失衡，则只会有左右和左左两种情况，因为是在右子树删除结点。
					if (p->GetLeftChild()->GetRightChild() != NULL && 
							(Height(p->GetLeftChild()->GetRightChild()) 
							 > Height(p->GetLeftChild()->GetLeftChild()))) {
						RotateLeft(p);
						RotateRight(p);
					} else {
						RotateRight(p);
					}
				}
			} else {
				if (p->GetLeftChild() != NULL) {
					//  如果有左孩子
					//  则将左子树作为当前结点
					ChangeChild(p->GetParent(), p, p->GetLeftChild());
				}
				else if (p->GetRightChild() != NULL) {
					//  如果有右孩子
					//  则将右子树作为当前结点
					ChangeChild(p->GetParent(), p, p->GetLeftChild());	
				}
				else {
					ChangeChild(p->GetParent(), p, p->GetLeftChild());
				}
				delete p;
			}

			return false;
		}
	}
	else if (email < p->GetEmail()) {
		Delete(p->GetLeftChild(), email, authcode);
		if (2 ==  ABS(Height(p->GetLeftChild()) - Height(p->GetRightChild())) ) {
			if (p->GetLeftChild()->GetRightChild() != NULL &&
					Height(p->GetLeftChild()->GetRightChild()) > 
						Height(p->GetLeftChild()->GetLeftChild()) ) { 
				RotateRight(p);
				RotateLeft(p);
			} else {
				RotateLeft(p);
			}
		}
	}
	else {
		Delete(p->GetRightChild(), email, authcode);
		if (2 == ABS(Height(p->GetLeftChild() - Height(p->GetRightChild()))) ) {
			if (2 == ABS(Height(p->GetLeftChild() - Height(p->GetRightChild()))) ) {
				//	如果失衡，则只会有左右和左左两种情况，因为是在右子树删除结点。
				if (p->GetLeftChild()->GetRightChild() != NULL && 
						(Height(p->GetLeftChild()->GetRightChild()) 
						 > Height(p->GetLeftChild()->GetLeftChild()))) {
					RotateLeft(p);
					RotateRight(p);
				} else {
					RotateRight(p);
				}
			}
		}
	}

	return true;
}

void HttpRegisterAction::AuthCodeTree::ChangeChild(AuthCodeNode *parent,
		AuthCodeNode *oldchild,
		AuthCodeNode *newchild) {
	if (parent != NULL) {
		if (parent->GetLeftChild() == oldchild) {
			//  如果当前结点为其父结点的左孩子
			parent->SetLeftChild(newchild);
		} 
		else if (parent->GetRightChild() == oldchild) {
			parent->SetRightChild(newchild);
		}
		else {
			// 如果不是右孩子也不是左孩子，就什么都不做
			return ;
		}
		if (newchild != NULL) {
			newchild->SetParent(parent);
		}
	} else {
		_root = newchild;
	}
}

int HttpRegisterAction::AuthCodeTree::Height(AuthCodeNode *p) {
	if (NULL == p) {
		return 0;
	}
	int lh = Height(p->GetLeftChild()) + 1;
	int rh = Height(p->GetRightChild()) + 1;

	return lh > rh? lh: rh; 
}

void HttpRegisterAction::AuthCodeTree::RotateLeft(AuthCodeNode *p) {
	AuthCodeNode *rchild = p->GetRightChild();
	ChangeChild(p->GetParent(), p, p->GetLeftChild());
	p->SetLeftChild(rchild->GetRightChild());
	rchild->SetRightChild(p);
}

void HttpRegisterAction::AuthCodeTree::RotateRight(AuthCodeNode *p) {
	AuthCodeNode *lchild = p->GetLeftChild();
	ChangeChild(p->GetParent(), p, p->GetRightChild());
	p->SetRightChild(lchild->GetLeftChild());
	lchild->SetLeftChild(p);
}

//	end AuthCodeTree implemention
/////////////////////////////////////////////////////
