/* ****************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * ****************************************************/

#include "enrollaction.h"
#include "jsontransverter.h"
#include "simplemysql.h"

#include <vector>

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

std::string HttpEnrollAction::Enroll(const std::string &name,
							  const std::string &password){
	const string condition = "UserName=\'" + name +
								"\' AND " + "UserPassword=\'" + password + "\'";
#ifdef DEBUG
	printf("HttpEnrollAction::Enroll :\ncondition : %s", condition.c_str());
#endif	// ! DEBUG
	if( _spmysql_ptr->Search(string("CACWUser"), condition) == false) {
		typedef	std::vector<std::map<string, string> > JsonObj;
		JsonObj res;
		string temp;
		string str;
		_spmysql_ptr->GetAllResult(_spmysql_ptr->GetUseResult(), res);
		for (JsonObj::iterator iter = res.begin(); 
				iter != res.end();
				++iter) {
			// temp = *iter;
			JsonTransverter::ToJsonString(*iter, temp);
			str += temp;
		}
		_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
		return std::string("000");
	}
	else {
		_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
		return std::string("001");
	}

	return std::string("001");
}
