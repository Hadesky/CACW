/* ****************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * ****************************************************/

#include "enrollaction.h"
#include "jsontransverter.h"
#include "md5.h"
#include "session.h"
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
	const string condition = "username=\'" + name +
								"\' AND " + "password=\'" + MD5::GetMD5String(password) + "\'";
#ifdef DEBUG
	printf("HttpEnrollAction::Enroll :\ncondition : %s\n", condition.c_str());
#endif	// ! DEBUG
	if( _spmysql_ptr->Search(string("CUser"), condition) == true) {
		typedef	std::vector<std::map<string, string> > JsonObj;
		JsonObj res;
		string temp;
		string str;
		if (_spmysql_ptr->GetAllResult(_spmysql_ptr->GetUseResult(), res)) {
			for (JsonObj::iterator iter = res.begin(); 
					iter != res.end();
					++iter) {
				// temp = *iter;
				JsonTransverter::ToJsonString(*iter, temp);
				str += temp;

			}
			_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
			Session::GetInstance().Insert(MD5::GetMD5String(name));
			return std::string("000, sessionid=" + MD5::GetMD5String(name));
		}
		_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
		return std::string("001");
	}
	else {
		_spmysql_ptr->FreeResult(_spmysql_ptr->GetUseResult());
		return std::string("001");
	}

	return std::string("001");
}
