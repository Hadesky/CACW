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
	:_mysql_ptr(NULL){
	
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
#ifdef DEBUG
	printf("SimpleMySql::GetInstance:\n\
			s_simplemysql_ptr.use_count is %ld\n", s_simplemysql_ptr.use_count());
#endif	// ! DEBUG
	if (s_simplemysql_ptr.use_count() == 0) {
		boost::shared_ptr<SimpleMySql> temp_ptr(new SimpleMySql());
		s_simplemysql_ptr = temp_ptr;
		if (!s_simplemysql_ptr->Init(user, pwd, db, server)) {
#ifdef DEBUG
			printf("SimpleMySql is initialized ...\n");
#endif	// ! DEBUG
			s_simplemysql_ptr.reset();
		}
	}

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
		return true;
	}

	return false;
}

bool SimpleMySql::Insert(const string table,
						 const string field, 
						 const string value) {
	if(GetInstance().use_count() == 0 || NULL == _mysql_ptr) {
#ifdef DEBUG
		printf("SimpleMySql::Insert\n\
				use_count is %ld\n\
				_mysql_ptr is %p\n", GetInstance().use_count(), _mysql_ptr);
#endif	// ! DEBUG
		return false;
	}
	const string cmd = "insert into " + table + "(" + field + ")"
				+ " value" + "(" + value + ")";
	if (0 == mysql_real_query(_mysql_ptr,
							 cmd.c_str(),
							 (unsigned int)cmd.length())) {
#ifdef DEBUG
		printf("SimpleMySql::Insert success...\n");
#endif	// ! DEBUG
		return true;
	}

	return false;
}

bool SimpleMySql::Search(const string table,
						 const string field,
						 const string value) {
	assert(GetInstance().use_count() > 0);
	const string cmd = "selete * from " + table +
						" where " + field + "=" + value;
	assert(_mysql_ptr != NULL);
	if ( 0 == mysql_real_query(_mysql_ptr,
							   cmd.c_str(),
							   (unsigned int)cmd.length())) {
#ifdef DEBUG
		printf("SimpleMySql::Search success...\n");
#endif	// ! DEBUG
		return true;
	}

	return false;
}


bool SimpleMySql::Search(const string table, const string condition) {
	if(GetInstance().use_count() == 0 || NULL == _mysql_ptr) {
#ifdef DEBUG
		printf("SimpleMySql::Insert\n\
				use_count is %ld\n\
				_mysql_ptr is %p\n", GetInstance().use_count(), _mysql_ptr);
#endif	// ! DEBUG
		return false;
	}
	const string cmd = "select * from " + table + " where " + condition;
	if (0 == mysql_real_query(_mysql_ptr,
							  cmd.c_str(), 
							  (unsigned int)cmd.length())) {
#ifdef DEBUG
		printf("SimpleMySql::Search success...\n");
#endif	// ! DEBUG
		return true;
	}

	return false;
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
	const string cmd = "upadta " + table + " set " + field + "=" + value
				+ " where " + condition;
	if (0 == mysql_real_query(_mysql_ptr,
								cmd.c_str(),
								(unsigned int)cmd.length())) {
#ifdef DEBUG
		printf("SimpleMySql::Update success...\n");
#endif	// ! DEBUG
		return true;
	}

	return false;
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

	return true;
}
