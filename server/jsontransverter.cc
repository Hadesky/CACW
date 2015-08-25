/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/25/2015
 * ********************************************************/

#include "jsontransverter.h"

JsonTransverter::JsonTransverter() {
	_str = "";
}

JsonTransverter::JsonTransverter(std::string str) {
	_str = str;
}
JsonTransverter::~JsonTransverter() {
	
}
bool JsonTransverter::Init() {
	return true;
}

void JsonTransverter::ToJsonString(std::string &str) const{
	Json::Reader *pjsonparser = new Json::Reader(Json::Features::strictMode());
	Json::Value value;
	Json::StyledWriter sw;
	if (!pjsonparser->parse(_str, value)) {
		str = "";
	} else {
		str = sw.write(value);
	}
}
	
void JsonTransverter::ToJsonString(const std::string prestr, std::string &str) {
	Json::Reader *pjsonparser = new Json::Reader(Json::Features::strictMode());
	Json::Value value;
	Json::StyledWriter sw;
	if (!pjsonparser->parse(prestr, value)) {
		str = "";
	} else {
		str = sw.write(value);
	}
	
}

void JsonTransverter::SetString(std::string str) {
	_str = str;
}
	
void JsonTransverter::GetString(std::string &str) {
	str = _str;
}

void JsonTransverter::Append(const std::string &str) {
	_str.append(str);
}
