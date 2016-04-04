/* ********************************************************
 * athor: waponx
 * email: 18826419224@163.com
 * time : 8/25/2015
 * ********************************************************/

#ifndef _SERVER_JSONTRANSVERTER_H
#define _SERVER_JSONTRANSVERTER_H

#include "./json/json.h"
#include "./include/NoCopyAndAssign.h"
#include <map>
#include <string>


using std::string;

class JsonTransverter {
	public :
		/*
		 * parse a string and generate a Json::Value object.
		 * But the function can not parse json array.
		 * @param str string which be parsed.
		 * @return Json::Value
		 */
		static Json::Value ParseJsonString(const string &str);

		/*
		 * Parse and format a string and output to a new string.
		 * But the function can not parse array.
		 * If oldstr is illegal, output is null.
		 * @param	oldstr string which be parse.
		 *			newstr string which save ressult.
		 * @return bool if success, return true.
		 */
		static bool ToJsonString(const string &oldstr, string &newstr);

		/**
		 * parse json object.
		 * @param m	m is a map include json object's key an value. The value is probably an
		 * original str or a json array, and so on.
		 *			str	save the result of parsing the m
		 * @return bool if success, return true.
		 */
		static bool ToJsonString(const std::map<string, Json::Value>& m, string &str);

		/*
		 * provide a interfece to transverte a map to a json array object.
		 * @param	key json array object's key
		 *			m json array object's value
		 * @return Json::Value 
		 * for example :
		 *		"array" : [
		 *			{
		 *				"key" : 0
		 *			},
		 *			{
		 *				"key" : 1
		 *			}
		 *		]
		 */
		static Json::Value ToJsonArray(const string &key, const std::map<string, string> &m);

	private :
		NOCOPYANDASSIGN(JsonTransverter);
};


#endif
