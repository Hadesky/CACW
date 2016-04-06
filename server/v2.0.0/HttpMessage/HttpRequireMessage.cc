#include "HttpRequireMessage.h"
#include "../include/log.h"

#include "../jsontransverter.h"
#include "../json/json.h"

namespace HttpMessage {
// Constructors/Destructors
//  

HttpRequireMessage::HttpRequireMessage (const string &raw) 
	: _raw(raw) {
	Init(raw);
}

HttpRequireMessage::~HttpRequireMessage () {

}


//  
// Methods
//  


// Accessor methods
//  


// Other methods
// 

string HttpRequireMessage::ToString() const {
	return _raw;
}


void HttpRequireMessage::Init(const string &raw) {
	using std::vector;

	vector<string> vec_str;
	SplitString(raw, "\r\n\r\n", vec_str);
	if (vec_str.size() <= 1) {
		waponx::log("incorrect http require message format.");
		return ;
	}

	_content = vec_str[1];
	//JsonTransverter::ParseJsonString(vec_str[1]);

	vector<string> head;
	SplitString(vec_str[0], "\r\n", head);

	vector<string> require_line;
	if (require_line.size() <= 1) {
		waponx::log("incorrect http require message format.");
		return ;
	}

	SplitString(head[0], " ", require_line);
	_method = require_line[0];
	_url = require_line[1];
	_version = require_line[2];

	InitField(head);
	
}

void HttpRequireMessage::InitField(const std::vector<string> &head) {
	using std::vector;

	vector<string>::const_iterator c_iter = head.cbegin();
	string::size_type pos = 0;

	++c_iter;
	for (; c_iter != head.cend(); ++c_iter) {
		if ( (pos = c_iter->find_first_of(':')) != string::npos) {
			_field[c_iter->substr(0, pos)] = c_iter->substr(pos + 1);
		}
	}
}


void HttpRequireMessage::SplitString(const string &src_str,
		const string &separate,
		std::vector<string> &output) {
	string::size_type pos = 0;
	string::size_type start = 0;
	string::size_type end = 0;
	string::size_type len_of_sep = separate.size();

	while ( (end = src_str.find(separate, pos)) != string::npos) {
		output.push_back(src_str.substr(start, end - start));
		pos = end + len_of_sep;
		start = pos;
	}
	
	output.push_back(src_str.substr(start, end - start));
}


}	//end namespace HttpMessage


