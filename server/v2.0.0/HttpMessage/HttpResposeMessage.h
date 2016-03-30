
#ifndef HTTPRESPOSEMESSAGE_H
#define HTTPRESPOSEMESSAGE_H
#include "HttpMessageFolder/HttpMessage.h"

#include <string>

namespace HttpMessageFolder {


/**
  * class HttpResposeMessage
  * author:waponx
  * email:453589103@qq.com
  * time:Mar Thirty 2016
  */

class HttpResposeMessage : virtual public HttpMessage
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  HttpResposeMessage ();

  /**
   * Empty Destructor
   */
  virtual ~HttpResposeMessage ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   */
  void HttpResponseMessage ()
  {
  }


  /**
   */
  virtual void _HttpResponseMessage ()
  {
  }


  /**
   * @param  text
   */
  void SetText (undef text)
  {
    void SetText(const string &text) {
    	_field["text"] = text;
    }
  }


  /**
   * @return string
   * @param  fieldname
   */
  string Getfiled (undef fieldname) const
  {
    string GetField(const string &fieldname) const {
    	if (_filed.find(filedname) != _field.end()) {
    		return _field[fieldname];
    	}
    	
    	return string();
    }
  }


  /**
   * @param  name
   * @param  value
   */
  void SetField (undef name, undef value)
  {
    void SetField(const string &name, const string &value) {
    	_field[name] = value;
    }
  }


  /**
   * format the message to string.
   * @return string
   */
  virtual string ToString () const
  {
  }

protected:

  // Static Protected attributes
  //  

  // Protected attributes
  //  

public:


  // Protected attribute accessor methods
  //  

protected:

public:


  // Protected attribute accessor methods
  //  

protected:


private:

  // Static Private attributes
  //  

  // Private attributes
  //  

public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  

private:



};
}; // end of package namespace

#endif // HTTPRESPOSEMESSAGE_H
