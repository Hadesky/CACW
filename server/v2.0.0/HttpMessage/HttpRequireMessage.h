#ifndef HTTPREQUIREMESSAGE_H
#define HTTPREQUIREMESSAGE_H

#include "HttpMessage.h"

#include <string>

namespace HttpMessage {


/**
  * class HttpRequireMessage
  * 
  */

class HttpRequireMessage : virtual public HttpMessage
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  HttpRequireMessage ();

  /**
   * Empty Destructor
   */
  virtual ~HttpRequireMessage ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * @param  raw raw content will be used to construct a map.
   */
   HttpRequireMessage (string raw)
  {
  }


  /**
   * get field by fieldname.
   * @return string
   * @param  fieldname
   */
  virtual string GetField (undef fieldname) const
  {
    string GetField(const string &fieldname) const{
    	return HttpMessage::GetField(fieldname);
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



  /**
   * This function will init the map by raw.
   * @param  raw raw content.
   */
  void Init (undef raw)
  {
  }


};
}; // end of package namespace

#endif // HTTPREQUIREMESSAGE_H
