#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("archer");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Characters/Archer", "Archer.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("teapot");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	color = glm::vec3(0);
	myMesh = 'a';
	position = glm::vec3(3, 0, 0);
	up = true;
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(color.r, color.g, color.b, 1);
	
	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

	switch (myMesh) {
	case 'a':
		RenderMesh(meshes["archer"], position, glm::vec3(0.01f));
		break;
	case 't':
		RenderMesh(meshes["teapot"], position);
		break;
	default:
		RenderMesh(meshes["sphere"], position);
		break;
	}

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	int speed = 5;
	// treat continuous update based on input
	if (window->KeyHold(GLFW_KEY_D)) {
		position.x += speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		position.x -= speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_W)) {
		position.y += speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		position.y -= speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_J)) {
		if (position.y == 2)
			up = false;
		if (position.y == 0)
			up = true;
		if (up)
			position.y++;
		if (!up)
			position.y--;
	}
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		if (color == glm::vec3(0))
			color = glm::vec3(1, 0, 0);
		else
			color = glm::vec3(0);
	}
	if (key == GLFW_KEY_G) {
		switch (myMesh) {
		case 'a':
			myMesh = 't';
			break;
		case 't':
			myMesh = 's';
			break;
		default:
			myMesh = 'a';
			break;
		}
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
