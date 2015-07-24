/* *****************************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.23.2015
 * *****************************************************************/

#include "simplemysql.h"

SimpleMySql::SimpleMySql() {
	
}

SimpleMySql::~SimpleMySql() {
	delete s_simplemysql_ptr;
	delete _mysql_ptr;
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

bool SimpleMySql::Init(string user,
					   string pwd,
					   string db,
					   string server) {
	s_simplemysql_ptr = NULL;
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
							0))
	{
		return false;
	}

	return true;
}

SimpleMySql *SimpleMySql::GetInstance(string user,
									  string pwd,
									  string db,
									  string server) {
	if (NULL == s_simplemysql_ptr) {
		s_simplemysql_ptr = new SimpleMySql();
		if (!s_simplemysql_ptr->Init(user, pwd, db, server)) {
					return NULL;
				}
	}

	return s_simplemysql_ptr;
}

bool SimpleMySql::Query() {

	return true;
}

bool SimpleMySql::Insert() {
	return true;
}


