/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/25/2015
 * ********************************************************/

#ifndef _SERVER_JSONTRANSVERTER_H
#define _SERVER_JSONTRANSVERTER_H

#include "value.h"

#include <map>
#include <string>

using std::string;

class JsonTransverter {
public :
	JsonTransverter();
	JsonTransverter(std::string str);
	~JsonTransverter();
	bool Init();
	void ToJsonString(std::string &str) const;
	//void ToJsonString(const std::string &prestr, std::string &str);
	void SetString(std::string str);
	void GetString(std::string &str);
	void Append(const std::string &str);
	static bool ToJsonString(const string &oldstr, string &newstr);
	static bool ToJsonString(const std::map<string, string>&dir, string &str);
private :
	DISALLOW_COPY_AND_ASSIGN(JsonTransverter);
private :
	std::string _str;

};


#endif
