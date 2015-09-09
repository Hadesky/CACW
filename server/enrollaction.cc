/* ********************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * ********************************************************/

#include "enrollaction.h"

EnrollAction::EnrollAction() {
	
}

EnrollAction::~EnrollAction() {
	
}

bool EnrollAction::Init(boost::shared_ptr<SimpleMySql> spmysql_ptr) {
	_spmysql_ptr = spmysql_ptr;

	return true;
}

bool EnrollAction::Enroll(const std::string &name,
		const std::string &password) {
	return true;
}
