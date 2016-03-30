#include "HttpServer.h"

// Constructors/Destructors
//  

HttpServer::HttpServer () {
initAttributes();
}

HttpServer::~HttpServer () { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

void HttpServer::initAttributes () {
  _addr = null;
  _isinit = false;
  _mysql = null;
}

