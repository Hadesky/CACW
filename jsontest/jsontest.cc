#include "../json/json.h"
#include <iostream>
#include <string>

int main() {
	std::string test = "{\"id\":1, \"name\":\"ha\"}";
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(test, value)) {
		if (!value["id"].isNull()) {
			std::cout << value["id"].asInt() << std::endl;
			std::cout << value["name"].asString() << std::endl;
		}
	}

	return 0;
}

