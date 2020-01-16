#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"
#include <ctime>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	srand(time(NULL));
	thirdPersonCamera = false;

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("bomb");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "black-handbomb.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("teapot");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("ShaderSea");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/SeaVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/SeaFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	airscrewStep = 0.0f;
	airplaneStep = 0.0f;
	itemStep = 0.0f;
	airplaneOY = 0.0f;
	offset = 0;
	start = false;
	hasCollided = false;
	airplaneCenter = glm::vec3(0.1, airplaneOY, 0);
	airplaneHeight = 2;
	airplaneLength = 2;
	airplaneWidth = 2;
	nLives = 3;
	obstacleTime = 0.0f;
	obstacleX = 50.0f;
	obstacleY = 15.0f;
	fuelTime = 0.0f;
	fuelX = 50.0f;
	fuelY = 15.0f;
	fuel = 1.0f;
	fuelItems = glm::vec3(0, 0, 0);

	camera = new Tema::Camera();
	camera->Set(glm::vec3(5, 0, 15), glm::vec3(5, 0, 0), glm::vec3(0, 1, 0));
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	if (nLives == 0 || fuel <= 0)
	{
		airplaneOY -= 10 * deltaTimeSeconds;
		if (airplaneStep > -RADIANS(30))
			airplaneStep -= 2 * deltaTimeSeconds;
		itemStep += 4 * deltaTimeSeconds;
		DrawNature();
		DrawAirplane();
		//DrawItems();
		DrawStatus();
		return;
	}

	obstacleTime += deltaTimeSeconds;
	fuelTime += deltaTimeSeconds;
	obstacleX -= 10 * deltaTimeSeconds;
	fuelX -= 10 * deltaTimeSeconds;
	fuel -= 0.02 * deltaTimeSeconds;
	if (obstacleTime >= 3)
	{
		obstacleTime = 0.0f;
		obstacleX = 16.0f;
		obstacleY = -2 + rand() % (4 - -2);
		hasCollided = false;
	}
	if (fuelTime >= 14)
	{
		fuelTime = 0.0f;
		fuelX = 16;
		fuelY = -2 + rand() % (4 - -2);
		fuelItems = glm::vec3(0, 0, 0);
	}

	airscrewStep += 20 * deltaTimeSeconds;
	itemStep += 4 * deltaTimeSeconds;
	airplaneOY += offset * deltaTimeSeconds;
	if (thirdPersonCamera)
		camera->TranslateUpword(offset * deltaTimeSeconds);
	if (offset > 0)
	{
		if (airplaneStep < RADIANS(30))
			airplaneStep += 2 * deltaTimeSeconds;
	}
	if (offset < 0)
	{
		if (airplaneStep > -RADIANS(30))
			airplaneStep -= 2 * deltaTimeSeconds;
	}
	if (offset == 0)
	{
		if (airplaneStep > 0)
			airplaneStep -= deltaTimeSeconds;
		else if (airplaneStep < 0)
			airplaneStep += deltaTimeSeconds;
	}

	DrawNature();
	DrawAirplane();
	DrawItems();
	DrawStatus();

	offset = 0;
}

void Tema2::FrameEnd()
{

}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->program)
		return;

	//// render an object using the specified shader and the specified position
	//glUseProgram(shader->program);

	//// TODO : get shader location for uniform mat4 "Model"
	//int location = glGetUniformLocation(shader->GetProgramID(), "Model");

	//// TODO : set shader uniform "Model" to modelMatrix
	//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	//// TODO : get shader location for uniform mat4 "View"
	//location = glGetUniformLocation(shader->GetProgramID(), "View");

	//// TODO : set shader uniform "View" to viewMatrix
	//glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	//// TODO : get shader location for uniform mat4 "Projection"
	//location = glGetUniformLocation(shader->GetProgramID(), "Projection");

	//// TODO : set shader uniform "Projection" to projectionMatrix
	//glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	//location = glGetUniformLocation(shader->GetProgramID(), "Time");
	//glUniform1f(location, Engine::GetElapsedTime());

	//// Draw the object
	//glBindVertexArray(mesh->GetBuffers()->VAO);
	//glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	GLint obj_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(obj_color, 1, glm::value_ptr(color));

	int location = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(location, Engine::GetElapsedTime());

	mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !thirdPersonCamera)
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			camera->MoveForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->MoveForward(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C)
	{
		thirdPersonCamera = !thirdPersonCamera;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	float sensivityOX = 0.001f;
	float sensivityOY = 0.001f;

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (!thirdPersonCamera) {
			camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
		}

		if (thirdPersonCamera) {
			camera->RotateThirdPerson_OX(-deltaY * sensivityOY, airplaneCenter);
			camera->RotateThirdPerson_OY(-deltaX * sensivityOX, airplaneCenter);
		}
	}
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (!start)
			start = true;
		else
			offset = -deltaY;
	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

void Tema2::DrawStatus()
{
	// fuel bar 
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(19, 6.5 - 1.5 + 1.5 * fuel, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, fuel * 3, 0));
	RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1));

	// background bar
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(19, 6.5, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 3, 0));
	RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.196078, 0.8, 0.196078));

	// lives
	for (int i = 0; i < nLives; i++)
	{
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-9.5 + i, 7.5, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 0));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.85, 0.85, 0.10));
	}
}

bool Tema2::AirplaneHasCollided(glm::vec3 sphereCenter, float sphereRadius)
{
	// airplane coordinates
	float minX = -0.7, maxX = 0.9;
	float minY = airplaneOY - 1, maxY = airplaneOY + 1;
	float minZ = -0.875, maxZ = 0.875;

	// get box closest point to sphere center by clamping
	float x = max(minX, min(sphereCenter.x, maxX));
	float y = max(minY, min(sphereCenter.y, maxY));
	float z = max(minZ, min(sphereCenter.z, maxZ));

	// this is the same as isPointInsideSphere
	float distance = sqrt((x - sphereCenter.x) * (x - sphereCenter.x) +
		(y - sphereCenter.y) * (y - sphereCenter.y) +
		(z - sphereCenter.z) * (z - sphereCenter.z));

	return distance < sphereRadius;
}

void Tema2::DrawItems()
{
	// obstacle
	if (!hasCollided)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(obstacleX, obstacleY, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 1, 0));
		RenderSimpleMesh(meshes["bomb"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));

		if (AirplaneHasCollided(glm::vec3(obstacleX, obstacleY, 0), 1))
		{
			hasCollided = true;
			nLives--;
		}
	}

	// fuel
	if (fuelItems.x == 0)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(fuelX, fuelY, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, 0.75, 0.75));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(0, 0, 1));
		RenderSimpleMesh(meshes["teapot"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.678431, 0.917647, 0.917647));

		if (AirplaneHasCollided(glm::vec3(fuelX, fuelY, 0), 0.75))
		{
			if (fuel <= 0.9)
				fuel += 0.1;
			fuelItems.x = 1;
		}
	}

	if (fuelItems.y == 0)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(fuelX + 1, fuelY - 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, 0.75, 0.75));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(0, 0, 1));
		RenderSimpleMesh(meshes["teapot"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.678431, 0.917647, 0.917647));

		if (AirplaneHasCollided(glm::vec3(fuelX + 1, fuelY - 1, 0), 0.75))
		{
			if (fuel <= 0.9)
				fuel += 0.1;
			fuelItems.y = 1;
		}
	}

	if (fuelItems.z == 0)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(fuelX + 2, fuelY - 2, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, 0.75, 0.75));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(0, 0, 1));
		RenderSimpleMesh(meshes["teapot"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.678431, 0.917647, 0.917647));

		if (AirplaneHasCollided(glm::vec3(fuelX + 2, fuelY - 2, 0), 0.75))
		{
			if (fuel <= 0.9)
				fuel += 0.1;
			fuelItems.z = 1;
		}
	}
}

void Tema2::DrawAirplane()
{
	{
		// body
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(airplaneCenter.x, airplaneOY, 0));
		modelMatrix = glm::rotate(modelMatrix, airplaneStep, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-airplaneCenter.x, -airplaneOY, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, airplaneOY, 0));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.62352, 0.372549, 0.623529));

		// head
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(airplaneCenter.x, airplaneOY, 0));
		modelMatrix = glm::rotate(modelMatrix, airplaneStep, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-airplaneCenter.x, -airplaneOY, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.6, airplaneOY, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.9, 0.9));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.658824, 0.658824, 0.658824));

		// airscrew stand
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(airplaneCenter.x, airplaneOY, 0));
		modelMatrix = glm::rotate(modelMatrix, airplaneStep, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-airplaneCenter.x, -airplaneOY, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.75, airplaneOY, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.25, 0.25));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.35, 0.16, 0.14));

		// airscrew
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(airplaneCenter.x, airplaneOY, 0));
		modelMatrix = glm::rotate(modelMatrix, airplaneStep, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-airplaneCenter.x, -airplaneOY, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.85, airplaneOY, 0));
		modelMatrix = glm::rotate(modelMatrix, airscrewStep, glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.85, -airplaneOY, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.85, airplaneOY, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 2, 0.2));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.62352, 0.372549, 0.623529));

		// tail
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(airplaneCenter.x, airplaneOY, 0));
		modelMatrix = glm::rotate(modelMatrix, airplaneStep, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-airplaneCenter.x, -airplaneOY, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.6, 0.5 + airplaneOY, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.5, 0.25));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.35, 0.16, 0.14));

		// right wing
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(airplaneCenter.x, airplaneOY, 0));
		modelMatrix = glm::rotate(modelMatrix, airplaneStep, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-airplaneCenter.x, -airplaneOY, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, airplaneOY, 0.5));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, 0.1, 0.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.35, 0.16, 0.14));

		// left wing
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(airplaneCenter.x, airplaneOY, 0));
		modelMatrix = glm::rotate(modelMatrix, airplaneStep, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-airplaneCenter.x, -airplaneOY, 0));

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, airplaneOY, -0.5));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75, 0.1, 0.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.35, 0.16, 0.14));
	}
}

void Tema2::DrawNature()
{
	{
		// cloud 1 - component 1
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-6, 5, 0));
		modelMatrix = glm::rotate(modelMatrix, -RADIANS(20), glm::vec3(1, 0, 1.5));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.560784, 0.560784, 0.737255));

		// cloud 1 - component 2
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-5, 5, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 0, 1.5));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.560784, 0.560784, 0.737255));

		// cloud 1 - component 3
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-4, 5, 0));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.560784, 0.560784, 0.737255));

		// cloud 1 - component 4
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 5, 0));
		modelMatrix = glm::rotate(modelMatrix, -RADIANS(30), glm::vec3(1, 3, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.560784, 0.560784, 0.737255));
	}

	{
		// cloud 2 - component 1
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 8, -3));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.184314, 0.184314, 0.309804));

		// cloud 2 - component 2
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 8, -3));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 0, 1.5));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.184314, 0.184314, 0.309804));

		// cloud 2 - component 3
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 7, -3));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.184314, 0.184314, 0.309804));

		// cloud 2 - component 4
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3, 8, -3));
		modelMatrix = glm::rotate(modelMatrix, -RADIANS(30), glm::vec3(1, 3, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.184314, 0.184314, 0.309804));
	}

	{
		// cloud 3 - component 1
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(7, 8, -3));
		modelMatrix = glm::rotate(modelMatrix, -RADIANS(20), glm::vec3(1, 0, 1.5));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.560784, 0.560784, 0.737255));

		// cloud 3 - component 2
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(8, 8, -3));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.560784, 0.560784, 0.737255));

		// cloud 3 - component 3
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(9, 7, -3));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.560784, 0.560784, 0.737255));

		// cloud 3 - component 4
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 8, -3));
		modelMatrix = glm::rotate(modelMatrix, -RADIANS(30), glm::vec3(1, 3, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.560784, 0.560784, 0.737255));
	}

	{
		// cloud 4 - component 1
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(16, 6, -5));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.184314, 0.184314, 0.309804));

		// cloud 4 - component 2
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 7, -5));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.184314, 0.184314, 0.309804));

		// cloud 4 - component 3
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(18, 6, -5));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.184314, 0.184314, 0.309804));

		// cloud 4 - component 4
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(19, 7, -5));
		modelMatrix = glm::rotate(modelMatrix, itemStep, glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.75, 1.75, 1.75));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.184314, 0.184314, 0.309804));
	
		// sea
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(5, -10, 5));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(25, 10, 40));
		modelMatrix = glm::rotate(modelMatrix, itemStep / 4, glm::vec3(0, 0, 1));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderSea"], modelMatrix, glm::vec3(0.196078, 0.6, 0.8));
	}
}