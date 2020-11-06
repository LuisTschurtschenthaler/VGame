#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <ostream>
#include <sstream>
#include <string>

class Log {

private:
	static std::string _file;


public:
	enum Priority {
		DEBUG,
		CONFIG,
		INFO,
		WARN,
		ERROR
	};

	static void setFile(const std::string& file);
	static void write(const Priority priority, const std::string& message);
	static void clear();

private:
	static std::string _getLabel(Priority priority);

};

#endif // LOG_H