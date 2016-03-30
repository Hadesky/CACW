
#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <string>
#include vector



namespace HttpServerFolder {


/**
  * class HttpServer
  * 
  */

class HttpServer
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  HttpServer ();

  /**
   * Empty Destructor
   */
  virtual ~HttpServer ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * @param  new_parameter listen queue's len.
   * @param  isreuseaddr mark the addr is reusable or not.
   */
   HttpServer (undef new_parameter = 1024, bool isreuseaddr = true)
  {
  }


  /**
   */
  void Start ()
  {
  }


  /**
   * @param  req_mes
   */
  void Post (const HttpRequireMessage & req_mes) const
  {
  }


  /**
   * @param  req_meg
   */
  void Get (const HttpRequireMessage & req_meg) const
  {
  }


  /**
   * @param  rsp_msg
   */
  void Send (const HttpResponseMessage & rsp_msg)
  {
  }


  /**
   * Sign up a user to database.
   * @return HttpResponseMassage
   * @param  username
   * @param  password
   * @param  sex
   * @param  email
   */
  HttpResponseMassage SignUp (const string & username, const string & password, const string & sex, const string & email)
  {
  }


  /**
   * Sign in.
   * @return HttpResponseMessage
   * @param  username
   * @param  password
   */
  HttpResponseMessage SignIn (string username, string password) const
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

  int _serverfd;
  struct sock_addr_in _addr;
  bool _isinit;
  shared_ptr<SimpleMySql> _mysql;
  shared_ptr<SessionManager> _session_manager;
public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  


  /**
   * Set the value of _serverfd
   * @param new_var the new value of _serverfd
   */
  void set_serverfd (int new_var)   {
      _serverfd = new_var;
  }

  /**
   * Get the value of _serverfd
   * @return the value of _serverfd
   */
  int get_serverfd ()   {
    return _serverfd;
  }

  /**
   * Set the value of _addr
   * @param new_var the new value of _addr
   */
  void set_addr (struct sock_addr_in new_var)   {
      _addr = new_var;
  }

  /**
   * Get the value of _addr
   * @return the value of _addr
   */
  struct sock_addr_in get_addr ()   {
    return _addr;
  }

  /**
   * Set the value of _isinit
   * @param new_var the new value of _isinit
   */
  void set_isinit (bool new_var)   {
      _isinit = new_var;
  }

  /**
   * Get the value of _isinit
   * @return the value of _isinit
   */
  bool get_isinit ()   {
    return _isinit;
  }

  /**
   * Set the value of _mysql
   * @param new_var the new value of _mysql
   */
  void set_mysql (shared_ptr<SimpleMySql> new_var)   {
      _mysql = new_var;
  }

  /**
   * Get the value of _mysql
   * @return the value of _mysql
   */
  shared_ptr<SimpleMySql> get_mysql ()   {
    return _mysql;
  }

  /**
   * Set the value of _session_manager
   * @param new_var the new value of _session_manager
   */
  void set_session_manager (shared_ptr<SessionManager> new_var)   {
      _session_manager = new_var;
  }

  /**
   * Get the value of _session_manager
   * @return the value of _session_manager
   */
  shared_ptr<SessionManager> get_session_manager ()   {
    return _session_manager;
  }
private:



  /**
   * @param  listen_len listen queue' s length.
   * @param  isreuseadd mark the address is reusable or not.
   */
  void init (int listen_len, bool isreuseadd)
  {
  }

  void initAttributes () ;

};
}; // end of package namespace

#endif // HTTPSERVER_H
