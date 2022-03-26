#pragma once

#include "ActionEvent.h"

class ActionListner {
public:
	virtual void actionPerformed(const ActionEvent& e) = 0;
};