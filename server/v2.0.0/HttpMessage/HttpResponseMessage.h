/**
 * class HttpResposeMessage
 * author:waponx
 * email:453589103@qq.com
 * updatetime:Mar Thirty 2016
 */


#ifndef HTTPRESPOSEMESSAGE_H
#define HTTPRESPOSEMESSAGE_H

#include "HttpMessage.h"

#include "../json/json.h"

#include <string>

namespace HttpMessage {
	class HttpResponseMessage: public HttpMessage {
		public:
			// Constructors/Destructors
			/**
			 * Empty Constructor
			 */
			HttpResponseMessage();

			/**
			 * Empty Destructor
			 */
			virtual ~HttpResponseMessage ();

			void SetText (const string &text) {
				_field["text"] = text;
			}

			/**
			 * @return string
			 * @param  fieldname
			 */
			string Getfiled (const string &fieldname) const {
				return HttpMessage::GetField(fieldname);
			}

			/**
			 * @param  name
			 * @param  value
			 */
			void SetField (const string &name, const string &value) {
				_field[name] = value;
			}
			
			/*
			 * Add a value to content if value is a string, not a array
			 * @param	key json's key
			 *			value json'value
			 */
			void AddContent(const string &key, const string &value);

			/*
			 * Add a array object to content.
			 * @param	key json array's key.
			 *			value json array.
			 */
			void AddContent(const string &key, const Json::Value &value);
			
			/**
			 * format the message to string.
			 * @return string
			 */
			virtual string ToString () const;

			/**
			 * clone the object to a new object.
			 * @return new object pointer.
			 */
			virtual HttpResponseMessage *Clone() const;

			/*
			 * clone the object to dest.
			 * @param dest point to a memoty which will be cover.
			 * @return return dest.
			 */
			virtual HttpResponseMessage *Clone(HttpResponseMessage *dest) const;
		public :
			string _version;
			string _response_code;
			string _reason;
			Json::Value _content;
	};
}

#endif // HTTPRESPOSEMESSAGE_H
