#include "HttpRequireMessage.h"

#include "../json/json.h"

namespace HttpMessage {
// Constructors/Destructors
//  

HttpRequireMessage::HttpRequireMessage (const string &raw) {
	Init(raw);
}

HttpRequireMessage::~HttpRequireMessage () {

}


//  
// Methods
//  


// Accessor methods
//  
string HttpRequireMessage::GetField(string fieldname) const {
    return HttpMessage::GetField(fieldname);
}


// Other methods
// 

void HttpRequireMessage::Init(const string &raw) {
	
}
}


