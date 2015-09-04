/* *****************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.23.2015
 * *****************************************************************/

#include "simplemysql.h"

#include <assert.h>

//boost::shared_ptr<SimpleMySql> 
//	SimpleMySql::s_simplemysql_ptr = SimpleMySql::GetInstance(string("test"),
//															  string("test"),
//															  string("CACWDB"),
//															  string("localhost"));
//


SimpleMySql::SimpleMySql() {
	
}

SimpleMySql::~SimpleMySql() {
	assert(_mysql_ptr != NULL);
	mysql_close(_mysql_ptr);
	_mysql_ptr = NULL;
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
									  string user,
									  string pwd,
									  string db,
									  string server) {

	static boost::shared_ptr<SimpleMySql> s_simplemysql_ptr;

	if (s_simplemysql_ptr.use_count() == 0) {
		boost::shared_ptr<SimpleMySql> temp_ptr(new SimpleMySql());
		s_simplemysql_ptr = temp_ptr;
		if (!s_simplemysql_ptr->Init(user, pwd, db, server)) {
			s_simplemysql_ptr.reset();
		}
	}

	return s_simplemysql_ptr;
}

bool SimpleMySql::Query(const string cmd) {
	assert(GetInstance().use_count() > 0);
	assert(_mysql_ptr != NULL);
	if (0 == mysql_real_query(_mysql_ptr, cmd.c_str(), (unsigned int)cmd.length())) {
		return true;
	}

	return false;
}

bool SimpleMySql::Insert(const string table,
						 const string field, 
						 const string value) {
	assert(GetInstance().use_count() > 0);
	const string cmd = "insert into " + table + "(" + field + ")"
				+ " value" + "(" + value + ")";
	assert(_mysql_ptr != NULL);
	if (0 == mysql_real_query(_mysql_ptr,
							 cmd.c_str(),
							 (unsigned int)cmd.length())) {
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
		return true;
	}

	return false;
}


bool SimpleMySql::Search(const string table, const string condition) {
	assert(GetInstance().use_count() > 0);
	const string cmd = "select * from " + table + " where " + condition;
	assert(_mysql_ptr != NULL);
	if (0 == mysql_real_query(_mysql_ptr,
							  cmd.c_str(), 
							  (unsigned int)cmd.length())) {
		return true;
	}

	return false;
}

bool SimpleMySql::Update(const string table,
						 const string field,
						 const string value,
						 const string condition) {
	assert(GetInstance().use_count() > 0);
	const string cmd = "upadta " + table + " set " + field + "=" + value
				+ " where " + condition;
	assert(_mysql_ptr != NULL);
	if (0 == mysql_real_query(_mysql_ptr,
								cmd.c_str(),
								(unsigned int)cmd.length())) {
		return true;
	}

	return false;
}

bool SimpleMySql::Init(string user,
					   string pwd,
					   string db,
					   string server) {
	_mysql_ptr = new MYSQL;
	_user = user;
	_password = pwd;
	_db = db;
	_server = server;

	mysql_init(_mysql_ptr);
	if (!mysql_real_connect(_mysql_ptr,
							_server.c_str(),
							_user.c_str(),
							_password.c_str(),
							_db.c_str(),
							0, 
							NULL,
							0)){
		return false;
	}

	return true;
}
