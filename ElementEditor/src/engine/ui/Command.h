#pragma once

#include <functional>

class Command {
public:
	Command(std::function<void()> action);

	void execute();

private:
	std::function<void()> action;
};