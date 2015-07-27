/* *****************************************************
 * author: waponx
 * email : 18826419224@163.com
 * time  : 7.25.2015
 * *****************************************************/

#ifndef _ENROLLACTION_H
#define _ENROLLACTION_H

#include <boost/shared_ptr.hpp>
#include <string>

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &);\
	void operator=(const TypeName &)
#endif //  ! DISALLOW_COPY_AND_ASSIGN


class SimpleMySql;

class EnrollAction {
public :
	EnrollAction();
	virtual ~EnrollAction();
	virtual bool Init(boost::shared_ptr<SimpleMySql> spmysql_ptr);
	virtual bool Enroll(const std::string name,
						const std::string password);
private :
	DISALLOW_COPY_AND_ASSIGN(EnrollAction);

protected :
	boost::shared_ptr<SimpleMySql> _spmysql_ptr;
};


class HttpEnrollAction: public EnrollAction {
public :
	HttpEnrollAction();
	virtual ~HttpEnrollAction();
	virtual bool Init(boost::shared_ptr<SimpleMySql> spmysql_ptr);
	virtual bool Enroll(const std::string name,
						const std::string password);
private :
	DISALLOW_COPY_AND_ASSIGN(HttpEnrollAction);
};

#endif // ! _ENROLLACTION_H

