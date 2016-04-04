#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#include "../include/NoCopyAndAssign.h"

#include <map>
#include <string>
#include <vector>

namespace HttpMessage {
	using std::string;
	using std::map;
	/**
	 * class HttpMessage
	 * author:waponx
	 * email:453589103@qq.com
	 * time:Mar Thirty 2016
	 */

	class HttpMessage : public NoCopyAndAssign
	{
		public:

			/**
			 * Empty Constructor
			 */
			HttpMessage ();

			/**
			 * Empty Destructor
			 */
			virtual ~HttpMessage ();
			/**
			 * @return string
			 * @param  fieldname get a field's value by fieldname
			 */

			virtual string GetField (string fieldname) const;
			/**
			 * format the message to string.
			 * @return string
			 */
			virtual string ToString () const;

			virtual HttpMessage *Clone() const;
			HttpMessage *Clone(HttpMessage *) const;
		private:
			void init () ;

		protected:
			map<string, string> _field;
	};
} // end of package namespace

#endif // HTTPMESSAGE_H
