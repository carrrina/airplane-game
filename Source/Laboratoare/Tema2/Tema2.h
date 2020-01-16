#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "Camera.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, const glm::vec3 &color = glm::vec3(1, 0, 0));

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		void DrawStatus();
		bool AirplaneHasCollided(glm::vec3 sphereCenter, float sphereRadius);
		void DrawNature();
		void DrawAirplane();
		void DrawItems();

	protected:
		Tema::Camera *camera;
		glm::mat4 projectionMatrix;
		glm::ivec2 resolution;

		glm::mat4 modelMatrix;
		GLenum polygonMode;

		float airplaneStep, airscrewStep, itemStep, airplaneOY, offset;
		bool start, hasCollided;
		glm::vec3 airplaneCenter, fuelItems;
		float airplaneHeight, airplaneLength, airplaneWidth;
		int nLives;
		float obstacleX, obstacleY, obstacleTime, fuelX, fuelY, fuelTime;
		float fuel;
		bool thirdPersonCamera;
};