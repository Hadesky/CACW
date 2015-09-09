/* ********************************************************
 * author: waponx
 * email : 18826419224
 * time  : 7.25.2015
 * *********************************************************/

#include "registeraction.h"
#include "simplemysql.h"

#include <assert.h>

RegisterAction::RegisterAction() {
	
}

RegisterAction::~RegisterAction() {

}

bool RegisterAction::Init(boost::shared_ptr<SimpleMySql> spmysql_ptr) {
	_spmysql_ptr = spmysql_ptr;

	return true;
}

bool RegisterAction::Register(const std::string &name,
							  const std::string &password,
							  const std::string &sex,
							  const std::string &email,
							  const std::string &address,
							  const std::string &phonenumber,
							  const std::string &shortphonenumber
							  ) {
	return true;		
}
