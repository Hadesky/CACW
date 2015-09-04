/* ******************************************
 * author : waponx
 * email  : 18826419224@163.com
 * time   : 7.23.2015
 * *****************************************/


#ifndef _SIMPLEMYSQL_H
#define _SIMPLEMYSQL_H

#include <boost/shared_ptr.hpp>
#include <mysql/mysql.h>
#include <string>

using std::string;

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &);\
	void operator=(const TypeName &)
#endif

class SimpleMySql {
public :
	virtual ~SimpleMySql();
//	static SimpleMySql *GetInstance();
	static boost::shared_ptr<SimpleMySql> & 
		GetInstance(string user = "",
					string pwd = "",
					string db = "",
					string server = "");
	bool Query(const string str);
	bool Insert(const string table, const string field, const string value);
	bool Search(const string table, const string field, const string value);
	bool Search(const string table, const string condition);
//	bool Alter(const string str);
	bool Update(const string table,
				const string field,
				const string value,
				const string condition);
//	void SetLoginName(string name);
//	void SetLoginName(char *name);
//	void SetLoginPassword(string pwd);
//	void SetLoginPassword(char *pwd_ptr);
//	void SetDatabase(string db);
//	void SetDatabase(char *db_ptr);
//	void SetServer(string server);
//	void SetServer(char *server_ptr);
	string GetLoginName();
	string GetLoginPassword();
	string GetLoginDatabase();
	string GetServer();
private :
	SimpleMySql();
	//  SimpleMySql(const SimpleMySql &);
	//  void operator = (const SimpleMySql &);
	DISALLOW_COPY_AND_ASSIGN(SimpleMySql);
	bool Init(string user,
			  string pwd,
			  string db,
			  string server);
private :
	MYSQL *_mysql_ptr;
	string _user;
	string _password;
	string _db;
	string _server;
};

#endif // !_SIMPLEMYSQL_H
