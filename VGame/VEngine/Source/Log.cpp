#include <iostream>
#include <chrono>
#include <ctime>
#include "Log.h"

std::string Log::_file = "";


void Log::setFile(const std::string& file) {
	_file = file;
}

void Log::write(const Priority priority, const std::string& message) {
	std::ofstream stream;
	stream.open(_file, std::ios::app);

	if(!stream.is_open())
		throw std::iostream::failure("Cannot open file: " + _file);

	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char timestamp[26];
	ctime_s(timestamp, sizeof timestamp, &now);

	std::string timestampWithoutEndl(timestamp);
	timestampWithoutEndl = timestampWithoutEndl.substr(0, 24);

	stream << "[" << timestampWithoutEndl << " >> " << _getLabel(priority) << "]: " << message << std::endl;
	stream.close();
}

void Log::clear() {
	std::ofstream stream;
	stream.open(_file, std::ios::out | std::ios::trunc);
	stream.close();
}

std::string Log::_getLabel(Priority priority) {
	switch(priority) {
		case DEBUG: return "DEBUG";
		case INFO:  return "INFO";
		case WARN:  return "WARN";
		case ERROR: return "ERROR";
		default: return "";
	}
}
