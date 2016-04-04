#include "HttpMessage.h"

namespace HttpMessage {
HttpMessage::HttpMessage () {
	init();
}

HttpMessage::~HttpMessage () {

}

string HttpMessage::GetField (string fieldname) const
{

	if (_field.find(fieldname) != _field.end()) {
		return _field.at(fieldname);
	}

	return string();
}

// Other methods
//  

string HttpMessage::ToString() const {
	return string();
}

HttpMessage *HttpMessage::Clone() const {
	HttpMessage *p_new_obj = new HttpMessage();
	Clone(p_new_obj);

	return p_new_obj;
}

HttpMessage *HttpMessage::Clone(HttpMessage *dest) const {
	if (dest != NULL) {
		NoCopyAndAssign::Clone(dest);
		dest->_field = _field;
	}
	return dest;
}

void HttpMessage::init () {

}

}	// end of HttpMessage namespace
