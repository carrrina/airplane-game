#include "CreatePrimitives.h"

#include <Core/Engine.h>

Mesh* CreatePrimitives::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	// draw 2 triangles. Add the remaining 2 indices
	indices.push_back(0);
	indices.push_back(2);

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* CreatePrimitives::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	// draw 2 triangles. Add the remaining 2 indices
	indices.push_back(0);
	indices.push_back(2);

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* CreatePrimitives::CreateCircle(std::string name, float cx, float cy, float r, int num_segments, glm::vec3 color)
{

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	int i;
	for (i = 0; i < num_segments; i++) {
		float theta = 2.0f * M_PI * float(i) / float(num_segments);//get the current angle 
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 

		vertices.push_back(VertexFormat(glm::vec3(x + cx, y + cy, 0), color));
		indices.push_back(i);
	}

	Mesh* sine = new Mesh(name);
	sine->SetDrawMode(GL_TRIANGLE_FAN);
	sine->InitFromData(vertices, indices);
	return sine;
}

Mesh* CreatePrimitives::CreateTriangle(std::string name, glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(vertex1, color),
		VertexFormat(vertex2, color),
		VertexFormat(vertex3, color),
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };

	triangle->InitFromData(vertices, indices);
	return triangle;
}