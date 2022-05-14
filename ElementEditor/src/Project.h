#pragma once

class Project {
public:
	Project();
	Project(int xDim, int yDim, int zDim);

	int getXDim();
	int getYDim();
	int getZDim();
	bool isBounded();

private:
	bool bounded = false;
	float xDim = 0, yDim = 0, zDim = 0;
};