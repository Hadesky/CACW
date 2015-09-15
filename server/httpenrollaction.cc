/* ****************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * ****************************************************/

#include "enrollaction.h"
#include "simplemysql.h"

#ifdef DEBUG
#include <cstdio>
#include "jsontransverter.h"
#include <vector>
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
	if( _spmysql_ptr->Search(string("CACWUser"), condition)) {
#ifdef DEBUG
		std::vector<std::map<string, string> >res;
		std::map<string, string> temp;
		string str;
		_spmysql_ptr->GetAllResult(_spmysql_ptr->GetUseResult(), res);
		for (std::vector<std::map<string, string> >::iterator iter = res.begin(); 
				iter != res.end();
				++iter) {
			temp = *iter;
			JsonTransverter::ToJsonString(temp, str);
			printf("%s\n", str.c_str());
		}
#endif	//  !DEBUG
		_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
		return true;
	}
	else {
		_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
		return false;
	}

	return false;
}
