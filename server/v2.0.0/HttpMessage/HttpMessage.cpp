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

void HttpMessage::init () {

}
}
