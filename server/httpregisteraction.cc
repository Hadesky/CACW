/* ********************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * *********************************************************/

#include "registeraction.h"
#include "simplemysql.h"

#ifdef DEBUG
#include <cstdio>
#endif	// !DEBUG

HttpRegisterAction::HttpRegisterAction() {
	
}

HttpRegisterAction::~HttpRegisterAction() {
	
}

bool HttpRegisterAction::Init(boost::shared_ptr<SimpleMySql> spmysql_ptr) {
	_spmysql_ptr = spmysql_ptr;

	return true;
}

bool HttpRegisterAction::Register(const std::string &name,
							  const std::string &password,
							  const std::string &sex,
							  const std::string &email,
							  const std::string &address,
							  const std::string &phonenumber,
							  const std::string &shortphonenumber){
	if (_spmysql_ptr->Search(std::string("CACWUser"), string("UserName"), name)) {
		return false;
	}
	const string field("UserName,\
UserPassword,\
sex,\
email,\
address,\
phonenumber,\
shortphonenumber");
#ifdef DEBUG
	printf("HttpRegisterAction::Register\nfield : %s\n", field.c_str());
#endif	// ! DEBUG
	const string values = "\"" + name + "\",\"" +password +"\",\"" +
		sex + "\",\"" + email + "\",\"" + address + "\",\"" +
		phonenumber + "\",\"" + shortphonenumber + "\",\""; 
#ifdef DEBUG
	printf("HttpRegisterAction::Register\nvalues : %s\n", values.c_str());
#endif	// ! DEBUG
	return _spmysql_ptr->Insert(string("CACWUser"), field, values);
}


