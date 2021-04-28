#include "ElementEngineException.h"

ElementEngineException::ElementEngineException(const std::string &message) :
	message(message) {}

ElementEngineException::~ElementEngineException() {}

const char* ElementEngineException::what() const {
	return message.c_str();
}