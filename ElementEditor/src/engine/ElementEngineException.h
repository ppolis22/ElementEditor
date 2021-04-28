#pragma once

#include <exception>
#include <string>

class ElementEngineException : public std::exception {
public:
	ElementEngineException(const std::string &message);
	virtual ~ElementEngineException();

	virtual const char* what() const override;

private:
	std::string message;
};