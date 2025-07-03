#pragma once
#include <stdexcept>
using std::string;
class ReadFailException : public std::runtime_error {
public:
	ReadFailException(const string& _Message) :
		runtime_error(_Message) {
	}
};

class WriteFailException : public std::runtime_error {
public:
	WriteFailException(const string& _Message) :
		runtime_error(_Message) {
	}
};