/* ********************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * ********************************************************/

#ifndef _REGISTERACTION_H
#define _REGISTERACTION_H

#include <boost/smart_ptr.hpp>
#include <string>

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &);\
	void operator=(const TypeName &)
#endif

class SimpleMySql;

class RegisterAction {
	public : 
		RegisterAction();
		virtual ~RegisterAction();
		virtual bool Init(boost::shared_ptr<SimpleMySql>spmysql_ptr);
		virtual std::string Register(const std::string &name,
				const std::string &password,
				const std::string &email,
				const std::string &sex,
				const std::string &authcode);
		virtual std::string GetAuthCode(const std::string &email) = 0;
	private :
		DISALLOW_COPY_AND_ASSIGN(RegisterAction);
	protected :
		boost::shared_ptr<SimpleMySql> _spmysql_ptr;
};

class HttpRegisterAction: public RegisterAction {
	public :
		HttpRegisterAction();
		virtual bool Init(boost::shared_ptr<SimpleMySql> spmyql);
		virtual ~HttpRegisterAction();
		virtual std::string Register(const std::string &name,
				const std::string &password,
				const std::string &email,
				const std::string &sex,
				const std::string &authcode);
		// 获取验证码的接口
		virtual std::string GetAuthCode(const std::string &email);
	private:
		DISALLOW_COPY_AND_ASSIGN(HttpRegisterAction);
	class AuthCodeNode {
		public :
			AuthCodeNode(AuthCodeNode *const parent,
					const std::string &email,
					const std::string &authcode);
			~AuthCodeNode();
			inline std::string GetEmail() const;
			inline std::string GetAuthCode() const;
			inline AuthCodeNode *GetLeftChild() const;
			inline AuthCodeNode *GetRightChild() const;
			inline AuthCodeNode *GetParent() const;
			void SetEmail(const std::string &email);
			void SetAuthCode(const std::string &authcode);
			void SetLeftChild(AuthCodeNode *const p);
			void SetRightChild(AuthCodeNode *const p);
			void SetParent(AuthCodeNode *const p);
		private :
			DISALLOW_COPY_AND_ASSIGN(AuthCodeNode);
		private	:
			std::string _email;
			std::string _authcode;
			AuthCodeNode *_lchild;
			AuthCodeNode *_rchild;
			AuthCodeNode *_parent;
	};
	class AuthCodeTree {
		//	使用平衡二叉查找树，减少查找结点的平均算法时间
		public :
			AuthCodeTree();
			~AuthCodeTree();
			//	获取随机生成的验证码并返回
			static std::string GetAuthCode();
			//	调用Delete方法
			bool Search(const std::string &email, const std::string &code);
			bool Add(const std::string &email);
			//  添加一个新的结点，如果成功则返回true
			//  如果这个邮箱已经对应了一个已存在的结点，则更新验证码
			//  返回true
			bool Add(const std::string &email, const std::string &code);
		private :
			DISALLOW_COPY_AND_ASSIGN(AuthCodeTree);
			bool Insert(AuthCodeNode *parent,
					AuthCodeNode *p,
					const std::string &email,
					const std::string &authcode);
			//	如果存在email和authcode在结点中匹配成功，就删除这个结点并返回true
			bool Delete(AuthCodeNode *p, 
					const std::string &email,
					const std::string &authcode);
			//  将parent的孩子结点设为newchild
			//  newchild将代替oldchild成为parent的孩子
			//  如果oldchild不是parent的孩子，则什么都不做
			void ChangeChild(AuthCodeNode *parent,
					AuthCodeNode *oldchild,
					AuthCodeNode *newchild);
			int Height(AuthCodeNode *p);
			//  左旋
			void RotateLeft(AuthCodeNode *r);
			//  右旋
			void RotateRight(AuthCodeNode *r);
		private :
			enum {AUTHCODELENGTH = 6};
			AuthCodeNode *_root;
	};
	AuthCodeTree *InitAtCTree();

	private :
		AuthCodeTree *_atctree;
};


#endif
