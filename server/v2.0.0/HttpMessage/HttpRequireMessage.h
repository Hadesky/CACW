#ifndef HTTPREQUIREMESSAGE_H
#define HTTPREQUIREMESSAGE_H

#include "HttpMessage.h"

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
   * @param  raw raw content will be used to construct a map.
   */
   HttpRequireMessage (const string &raw);

  /**
   * Empty Destructor
   */
  virtual ~HttpRequireMessage ();

  /**
   * get field by fieldname.
   * @return string
   * @param  fieldname
   */
  virtual string GetField (string fieldname) const;

  /**
   * format the message to string.
   * @return string
   */
  virtual string ToString () const;
public:


  // Private attribute accessor methods
  //  

private:



  /**
   * This function will init the map by raw.
   * @param  raw raw content.
   */
  void Init (const string &raw);

};
}; // end of package namespace

#endif // HTTPREQUIREMESSAGE_H
