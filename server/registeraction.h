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
				const std::string &sex = "",
				const std::string &email = "",
				const std::string &address = "",
				const std::string &phonenumber = "",
				const std::string &shortphonenumber = "");
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
				const std::string &sex = "",
				const std::string &email = "",
				const std::string &address = "",
				const std::string &phonenumber = "",
				const std::string &shortphonenumber = "");
	private:
		DISALLOW_COPY_AND_ASSIGN(HttpRegisterAction);
};


#endif
