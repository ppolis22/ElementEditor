#pragma once

#include "Mesh.h"
#include "engine/geom/Point3d.h"

class LineMeshBuilder {
public:
	Mesh buildPlaneLineMesh(Point3df p0, Point3df p1, Point3df p2, Point3df p3);
};