/* ****************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * ****************************************************/

#include "enrollaction.h"
#include "simplemysql.h"


HttpEnrollAction::HttpEnrollAction() {
	
}

HttpEnrollAction::~HttpEnrollAction() {

}


bool HttpEnrollAction::Init(boost::shared_ptr<SimpleMySql> spmysql_ptr) {
	_spmysql_ptr = spmysql_ptr;

	return true;
}

bool HttpEnrollAction::Enroll(const std::string name,
							  const std::string password) {
	if (_spmysql_ptr->Search(std::string("User"), string("UserName"), name)) {
		return false;
	}
	const string field("UserName, UserPassword");
	const string value = "\"" + name + "\",\"" + password + "\"";
	return _spmysql_ptr->Insert(string("User"), field, value);
}
