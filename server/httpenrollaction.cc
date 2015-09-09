/* ****************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * ****************************************************/

#include "enrollaction.h"
#include "simplemysql.h"

#ifdef DEBUG
#include <cstdio>
#endif	// ! DEBUG

HttpEnrollAction::HttpEnrollAction() {
	
}

HttpEnrollAction::~HttpEnrollAction() {

}


bool HttpEnrollAction::Init(boost::shared_ptr<SimpleMySql> spmysql_ptr) {
	_spmysql_ptr = spmysql_ptr;

	return true;
}

bool HttpEnrollAction::Enroll(const std::string &name,
							  const std::string &password){
	const string condition = "UserName=\'" + name +
								"\' AND " + "UserPassword=\'" + password + "\'";
#ifdef DEBUG
	printf("HttpEnrollAction::Enroll :\ncondition : %s", condition.c_str());
#endif	// ! DEBUG
	return _spmysql_ptr->Search(string("CACWUser"), condition);
}
