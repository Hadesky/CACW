#ifndef HTTPREQUIREMESSAGE_H
#define HTTPREQUIREMESSAGE_H

#include "HttpMessage.h"
#include <vector>

namespace HttpMessage {
	/**
	 * class HttpRequireMessage
	 * 
	 */

	class HttpRequireMessage : public HttpMessage
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
			virtual string GetField(string fieldname) const {
				return HttpMessage::GetField(fieldname);
			}

			// Accessor methods
			//
			
			virtual string content() const {
				return _content;
			}

			virtual string method() const {
				return _method;
			}

			virtual string url() const {
				return _url;
			}

			virtual string version() const {
				return _version;
			}

			/**
			 * format the message to string.
			 * @return string
			 */
			virtual string ToString () const;

		private:
			/**
			 * This function will init the map by raw.
			 * @param  raw raw content.
			 */
			void Init (const string &raw);

			/*
			 * init http require message field.
			 * @param head http require message's head
			 */
			void InitField(const std::vector<string> &head);

			/*
			 * Because C++ standard don't provide a function to splite string.
			 * So the function's purpose is splite string.
			 * @param src_str be splited string
			 *		sepatate be used to separate string
			 *		output svae result.
			 */
			void SplitString(const string &src_str,
					const string &separate,
					std::vector<string> &output);

		private :
			string _method;
			string _url;
			string _version;		// http version
			string _content;		// http requirement's content
			const string _raw;		// store raw http requirement
	};
}; // end of package namespace

#endif // HTTPREQUIREMESSAGE_H
