/* *****************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.23.2015
 * *****************************************************************/

#include "simplemysql.h"

#ifdef DEBUG
#include <cstdio>
#endif	// ! DEBUG

//boost::shared_ptr<SimpleMySql> 
//	SimpleMySql::s_simplemysql_ptr = SimpleMySql::GetInstance(string("test"),
//															  string("test"),
//															  string("CACWDB"),
//															  string("localhost"));
//


SimpleMySql::SimpleMySql()
	:_mysql_ptr(NULL),
	 _use_result(NULL),
	 _user(""),
	 _password(""),
	 _db(""),
	 _server(""),
	 _isinit(false) {
	
}

SimpleMySql::~SimpleMySql() {
	if (_mysql_ptr != NULL) {
		mysql_close(_mysql_ptr);
		_mysql_ptr = NULL;
	}
}

//void SimpleMySql::SetLoginName(string name) {
//	_name = name;
//}
//
//void SimpleMySql::SetLoginName(char *name_ptr) {
//	_name = name_ptr;
//}
//
//void SimpleMySql::SetLoginPassword(string pwd) {
//	_password = pwd;
//}
//
//void SimpleMySql::SetLoginPassword(char *pwd_ptr) {
//	_password = pwd_ptr;
//}
//
//void SimpleMySql::SetDatabase(string db) {
//	_db = db;
//}
//
//void SimpleMySql::SetDatabase(char *db_ptr) {
//	_db	= db_ptr;	
//}
//
//void SimpleMySql::SetServer(string server) {
//	_server = server;
//}
//
//void SimpleMySql::SetServer(char *server_ptr) {
//	_server = server_ptr;
//}

string SimpleMySql::GetLoginName() {
	return _user;
}

string SimpleMySql::GetLoginPassword() {
	return _password;
}

string SimpleMySql::GetLoginDatabase() {
	return _db;
}

string SimpleMySql::GetServer() {
	return _server;
}

boost::shared_ptr<SimpleMySql> &SimpleMySql::GetInstance(
		string server,
		string user,
		string pwd,
		string db) {

	static boost::shared_ptr<SimpleMySql> s_simplemysql_ptr(new SimpleMySql());
	s_simplemysql_ptr->Init(user, pwd, db, server);
//#ifdef DEBUG
//	printf("SimpleMySql::GetInstance:\n\
//			s_simplemysql_ptr.use_count is %ld\n", s_simplemysql_ptr.use_count());
//#endif	// ! DEBUG
//	if (s_simplemysql_ptr.use_count() == 0) {
//		boost::shared_ptr<SimpleMySql> temp_ptr(new SimpleMySql());
//		s_simplemysql_ptr = temp_ptr;
//		if (!s_simplemysql_ptr->Init(user, pwd, db, server)) {
//#ifdef DEBUG
//			printf("SimpleMySql is initialized ...\n");
//#endif	// ! DEBUG
//			s_simplemysql_ptr.reset();
//		}
//	}

	return s_simplemysql_ptr;
}

bool SimpleMySql::Query(const string cmd) {
	if (GetInstance().use_count() == 0 || NULL == _mysql_ptr) {
#ifdef DEBUG
		printf("SimpleMySql::Query\n\
				use_count is %ld\n\
				_mysql_ptr is %p\n", GetInstance().use_count(), _mysql_ptr);
#endif	// ! DEBUG
		return false;
	}
	if (0 == mysql_real_query(_mysql_ptr, cmd.c_str(), (unsigned int)cmd.length())) {
		UpdateUseResult();
		return true;
	}

	return false;
}

bool SimpleMySql::Insert(const string &table,
						 const string &field, 
						 const string &value) {
	if(GetInstance().use_count() == 0 || NULL == _mysql_ptr) {
#ifdef DEBUG
		printf("SimpleMySql::Insert\n\
				use_count is %ld\n\
				_mysql_ptr is %p\n", GetInstance().use_count(), _mysql_ptr);
#endif	// ! DEBUG
		return false;
	}
	const string cmd = "insert into " + table + "(" + field + ")"
				+ " values" + "(" + value + ")";
#ifdef DEBUG
	printf("SimpleMySql::Insert command : %s\n", cmd.c_str());
#endif	// ! DEBUG
	if (0 == mysql_real_query(_mysql_ptr,
							 cmd.c_str(),
							 (unsigned int)cmd.length())) {
#ifdef DEBUG
		printf("SimpleMySql::Insert success...\n");
#endif	// ! DEBUG
		UpdateUseResult();
		return true;
	}

#ifdef DEBUG
		printf("SimpleMySql::Insert fail...\n");
		printf("%s\n", mysql_error(_mysql_ptr));
#endif	// ! DEBUG

	return false;
}

bool SimpleMySql::Search(const string &table,
						 const string &field,
						 const string &value) {
	if(GetInstance().use_count() == 0 || NULL == _mysql_ptr) {
#ifdef DEBUG
		printf("SimpleMySql::Search\n\
				use_count is %ld\n\
				_mysql_ptr is %p\n", GetInstance().use_count(), _mysql_ptr);
#endif	// ! DEBUG
		return false;
	}
	const string cmd = "select * from " + table +
						" where " + field + "=" + "\'" + value + "\'";
#ifdef DEBUG
	printf("SimpleMySql::Search\n%s\n", cmd.c_str());
#endif	// ! DEBUG
	if (0 == mysql_real_query(_mysql_ptr,
							   cmd.c_str(),
							   (unsigned int)cmd.length())) {
		UpdateUseResult();
		if (_use_result != NULL) {
			return true;
		} else {
			return false;
		}
	}
#ifdef DEBUG
		printf("SimpleMySql::Search fail...\n");
		printf("%s\n", mysql_error(_mysql_ptr));
#endif	// ! DEBUG

	return false;
}


bool SimpleMySql::Search(const string &table, const string &condition) {
	if(GetInstance().use_count() == 0 || NULL == _mysql_ptr) {
#ifdef DEBUG
		printf("SimpleMySql::Search\n\
				use_count is %ld\n\
				_mysql_ptr is %p\n", GetInstance().use_count(), _mysql_ptr);
#endif	// ! DEBUG
		return false;
	}
	const string cmd = "select * from " + table + " where " + condition;
	if (0 == mysql_real_query(_mysql_ptr,
							  cmd.c_str(), 
							  (unsigned int)cmd.length())) {
		UpdateUseResult();
		if (_use_result != NULL) {
			return true;
		}
		return false;
	}

#ifdef DEBUG
		printf("SimpleMySql::Search fail...\n");
		printf("%s\n", mysql_error(_mysql_ptr));
#endif	// ! DEBUG
	return false;
}


bool SimpleMySql::Search(const string &table,
						 const string &field,
						 const string &value,
						 JsonObj &obj) {
	if (!Search(table, field, value)) {
		return false;
	}

	return GetAllResult(GetUseResult(), obj);
}

bool SimpleMySql::Search(const string &table,
		const string &condition,
		JsonObj &obj) {
	if (!Search(table, condition)) {
		return false;
	}
	return GetAllResult(GetUseResult(), obj);
}

bool SimpleMySql::Update(const string table,
						 const string field,
						 const string value,
						 const string condition) {
	if(GetInstance().use_count() == 0 || NULL == _mysql_ptr) {
#ifdef DEBUG
		printf("SimpleMySql::Insert\n\
				use_count is %ld\n\
				_mysql_ptr is %p\n", GetInstance().use_count(), _mysql_ptr);
#endif	// ! DEBUG
		return false;
	}
	const string cmd = "update " + table + " set " + field + "=" + value
				+ " where " + condition;
	if (0 == mysql_real_query(_mysql_ptr,
								cmd.c_str(),
								(unsigned int)cmd.length())) {
		//UpdateUseResult();
		return true;
	}

#ifdef DEBUG
		printf("SimpleMySql::Update fail...\n");
		printf("%s\n", mysql_error(_mysql_ptr));
#endif	// ! DEBUG
	return false;
}

bool SimpleMySql::GetAllResult(MYSQL_RES *mysqlres, std::vector<std::map<string, string> >&res) {
	std::map<string, string> row;
#ifdef DEBUG
	printf("SimpleMySql::GetAllResult\n");
#endif	// !DEBUG
	while (FetchOneRow(mysqlres,row)) {
#ifdef DEBUG
		for (std::map<string, string>::iterator it = row.begin();
				it != row.end();
				++it) {
			printf("key : %s, values : %s\n", it->first.c_str(), it->second.c_str());
		}
#endif	// ! DEBUG
		res.push_back(row);
	}
	if (res.size() == 0) {
		//	如果查询结果为空集
		return false;
	}
	return true;
}


MYSQL_RES *SimpleMySql::GetUseResult() const{
	return _use_result; 
}

MYSQL_RES *SimpleMySql::GetStoreResult() {
	if (false == _isinit) {
		return NULL;
	}

	return mysql_store_result(_mysql_ptr);
}

bool SimpleMySql::FreeResult(MYSQL_RES *mysqlres) {
	if (false == _isinit || NULL == mysqlres) {
		return false;
	}
	mysql_free_result(mysqlres);

	return true;
}


bool SimpleMySql::Init(string user,
		string pwd,
		string db,
		string server){
	if (true == _isinit) {
		return false;
	}
	_user = user;
	_password = pwd;
	_db = db;
	_server = server;
	_mysql_ptr = new MYSQL;
	mysql_init(_mysql_ptr);
	if (!mysql_real_connect(_mysql_ptr,
				_server.c_str(),
				_user.c_str(),
				_password.c_str(),
				_db.c_str(),
				0, 
				NULL,
				0)){

#ifdef DEBUG
		printf("SimpleMySql::connected fail...\n");
#endif	// ! DEBUG
		return false;
	}
	_isinit = true;
#ifdef DEBUG
		printf("SimpleMySql::connected success...\n");
#endif	// ! DEBUG

	return true;
}

bool SimpleMySql::FetchOneRow(MYSQL_RES *mysqlres, std::map<string, string> &row) {
	MYSQL_ROW mysqlrow_ptr = mysql_fetch_row(mysqlres);
	unsigned int column = mysql_num_fields(mysqlres);
	MYSQL_FIELD *fields = mysql_fetch_fields(mysqlres);
	
//#ifdef DEBUG
//		printf("mysqlrow_ptr:%p\ncolumn:%d\nfields:%p\n", mysqlrow_ptr, column, fields);
//#endif	// !DEBUG
	if (NULL == mysqlrow_ptr || column <= 0 || NULL == fields) {
		return false;
	}

	for (unsigned int i = 0; i < column; ++i) {
		row[fields[i].name] = mysqlrow_ptr[i];
	}
	
	return true;
}

void SimpleMySql::UpdateUseResult() {
	_use_result = mysql_use_result(_mysql_ptr);
}
