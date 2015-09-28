/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/25/2015
 * ********************************************************/

#include "jsontransverter.h"
#include "./json/json.h"

JsonTransverter::JsonTransverter()
	:_str(""){
	
}

JsonTransverter::JsonTransverter(std::string str) 
	: _str(str){

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
	
//void JsonTransverter::ToJsonString(const std::string &prestr, std::string &str) {
//	Json::Reader *pjsonparser = new Json::Reader(Json::Features::strictMode());
//	Json::Value value;
//	Json::StyledWriter sw;
//	if (!pjsonparser->parse(prestr, value)) {
//		str = "";
//	} else {
//		str = sw.write(value);
//	}
//	delete pjsonparser;
//}

void JsonTransverter::SetString(std::string str) {
	_str = str;
}
	
void JsonTransverter::GetString(std::string &str) {
	str = _str;
}

void JsonTransverter::Append(const std::string &str) {
	_str.append(str);
}


bool JsonTransverter::ToJsonString(const string &oldstr, string &newstr) {
	static Json::Reader jsonparser = Json::Reader(Json::Features::strictMode());
	static Json::Value value;
	static Json::StyledWriter sw;
	
	if (!jsonparser.parse(oldstr, value)) {
		newstr.clear();
		return false;
	} else {
		newstr = sw.write(value);

		return true;
	}

	return false;
}

bool JsonTransverter::ToJsonString(const std::map<string, string>&dir, string &str) {
	static Json::StyledWriter sw;
	Json::Value value;
	
	for (auto it = dir.cbegin(); it != dir.cend(); ++it) {
		value[it->first] = it->second;
	}
	str = sw.write(value);
	
	return true;
}
