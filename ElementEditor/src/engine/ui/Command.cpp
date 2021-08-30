#include "Command.h"

Command::Command(std::function<void()> action)
	:action(action) {}

void Command::execute() {
	action();
}