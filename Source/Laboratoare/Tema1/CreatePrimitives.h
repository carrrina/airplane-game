#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace CreatePrimitives
{
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color);
	Mesh* CreateCircle(std::string name, float cx, float cy, float r, int num_segments, glm::vec3 color);
	Mesh* CreateTriangle(std::string name, glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3, glm::vec3 color);
}

