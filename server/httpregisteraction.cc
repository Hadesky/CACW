/* ********************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * *********************************************************/

#include "registeraction.h"
#include "simplemysql.h"

HttpRegisterAction::HttpRegisterAction() {
	
}

HttpRegisterAction::~HttpRegisterAction() {
	
}

bool HttpRegisterAction::Init(boost::shared_ptr<SimpleMySql> spmysql_ptr) {
	_spmysql_ptr = spmysql_ptr;

	return true;
}

bool HttpRegisterAction::Register(const std::string name,
								  const std::string password) {
	const string condition = "UserName=\'" + name +
								"\' AND " + "UserPassword=\'" + password + "\'";
	return _spmysql_ptr->Search(string("User"), condition);
}


