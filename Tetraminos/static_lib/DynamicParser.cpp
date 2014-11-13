#include "DynamicParser.h"

bool DynamicParser::parseFile(std::string filePath) {
	std::ifstream file(filePath);
	std::string line;
	if (file.is_open()) {
		while(std::getline(file, line)) {
			char *pch;
			unsigned int timestamp_delimiter = line.find(" - ");
			// save the timestamp into a string var
			std::string timestamp = line.substr(0, timestamp_delimiter);
			//printf("timestamp: %s - ", timestamp.c_str());
			std::string afterTimeStamp = line.substr(timestamp_delimiter+3, line.length());

			unsigned int first_comma = afterTimeStamp.find(",");
			std::string className = afterTimeStamp.substr(0, first_comma);

			std::string afterClassName = afterTimeStamp.substr(first_comma+1, afterTimeStamp.length());

			unsigned int second_comma = afterClassName.find(",");
			std::string functionName = afterClassName.substr(0, second_comma);

			std::string on = afterClassName.substr(second_comma+1, afterClassName.length());

			const char *timeDeets = timestamp.c_str();

			struct tm tm;

			strptime(timeDeets, "%m/%d/%Y %T:")
			//printf("timestamp: %s - class: %s - function: %s - on/off: %s\n",
			//	timestamp.c_str(),
			//	className.c_str(),
			//	functionName.c_str(),
			//	on.c_str());

			// create a map using func_name as the key



			//printf("bool: %s\n", pch);
		}
	}
	return true;
}

int main() {
	DynamicParser *parser = new DynamicParser();
	parser->parseFile("testfile.txt");
	return 1;
}