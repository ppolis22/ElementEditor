#pragma once

#include <functional>

struct Point3df {
	float x, y, z;
};

struct Point2df {
	float x, y;
};

struct Point3di {
	int x, y, z;

	bool operator==(const Point3di& other) const {
		return (this->x == other.x && this->y == other.y && this->z == other.z);
	}

	struct HashFunction {
		size_t operator()(const Point3di& point) const {
			size_t res = 17;
			res = res * 31 + std::hash<int>()(point.x);
			res = res * 31 + std::hash<int>()(point.y);
			res = res * 31 + std::hash<int>()(point.z);
			return res;
		}
	};
};