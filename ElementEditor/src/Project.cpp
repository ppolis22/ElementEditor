#include "Project.h"

Project::Project() {}

Project::Project(int xDim, int yDim, int zDim) 
	: xDim(xDim), yDim(yDim), zDim(zDim), bounded(true) {}

int Project::getXDim() {
	return xDim;
}

int Project:: getYDim() {
	return yDim;
}

int Project::getZDim() {
	return zDim;
}

bool Project::isBounded() {
	return bounded;
}