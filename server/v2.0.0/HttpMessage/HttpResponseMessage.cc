#include "HttpResponseMessage.h"
//#include "../jsontransverter.h"

namespace HttpMessage {

HttpResponseMessage::HttpResponseMessage()
	:_version("HTTP/1.1") {
	
}

HttpResponseMessage::~HttpResponseMessage() {
	
}

void HttpResponseMessage::AddContent(const string &key, const string &value) {
	_content[key] = value;
}

void HttpResponseMessage::AddContent(const string &key, const Json::Value &value) {
	_content[key] = value;
}

string HttpResponseMessage::ToString()  const {
	string res;

	res += _version;
	res += " ";
	res += _response_code;;
	res += " ";
	res += _reason;
	res += "\r\n";

	for (map<string, string>::const_iterator iter = _field.cbegin();
			iter != _field.cend();
			++iter) {
		res += iter->first;
		res += ":";
		res += iter->second;
		res += "\r\n";
	}
	res += "\r\n";
	res += _content.toStyledString();

	return res;
}

HttpResponseMessage *HttpResponseMessage::Clone() const {
	HttpResponseMessage	*p_new_obj  = new HttpResponseMessage();
	Clone(p_new_obj);

	return p_new_obj;
}

HttpResponseMessage *HttpResponseMessage::Clone(HttpResponseMessage *dest) const {
	if (dest != NULL) {
		HttpMessage::Clone(dest);
	}

	return dest;
}

}

