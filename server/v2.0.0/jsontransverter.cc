/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/25/2015
 * ********************************************************/

#include "jsontransverter.h"

Json::Value JsonTransverter::ParseJsonString(const string &str) {
	if (str.size() <= 0) {
		return Json::Value();
	}

	static Json::Reader *pjsonparser = new Json::Reader(Json::Features::strictMode());
	static Json::Value value;
	
	pjsonparser->parse(str, value);
	return value;
}


bool JsonTransverter::ToJsonString(const string &oldstr, string &newstr) {
	// open strict mode when  old is illegal, don't assert.
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


Json::Value ToJsonArray(const string &key, const std::map<string, string> &m) {
	Json::Value res;
	Json::Value array;
	Json::Value item;
	for(std::map<string, string>::const_iterator iter = m.cbegin();
			iter != m.cend();
			++iter) {
		item[iter->first] = iter->second;
		array.append(item);
	}
	res[key] = array;

	return res;
}
		
bool JsonTransverter::ToJsonString(const std::map<string, Json::Value>&m,
		string &str) {
	Json::Value res;
	
	for (std::map<string, Json::Value>::const_iterator iter = m.cbegin();
	iter != m.cend();
	++iter) {
		res[iter->first] = iter->second;
	}
	str = res.toStyledString();

	return true;
}
