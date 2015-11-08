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

std::string HttpRegisterAction::Register(const std::string &name,
							  const std::string &password,
							  const std::string &email,
							  const std::string &sex,
							  const std::string &authcode){
	if (_spmysql_ptr->Search(std::string("CUser"), string("username"), name)) {
		_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
#ifdef DEBUG
		printf("HttpRegisterAction::Register the account is exist\n");
#endif	// !DEBUG
		return std::string("001");
	}
	static const string field("username,\
password,\
sex,\
email");
#ifdef DEBUG
	printf("HttpRegisterAction::Register\nfield : %s\n", field.c_str());
#endif	// ! DEBUG
	const string values = "\"" + name + "\",\"" +password +"\",\"" +
		sex + "\",\"" + email + "\""; 

#ifdef DEBUG
	printf("HttpRegisterAction::Register\nvalues : %s\n", values.c_str());
#endif	// ! DEBUG
	_spmysql_ptr->Insert(string("CACWUser"), field, values);

	return std::string("000");
}
