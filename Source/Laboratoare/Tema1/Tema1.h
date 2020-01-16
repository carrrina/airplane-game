#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	bool BirdHasCollided(float rectangleMinX, float rectangleMaxX, float rectangleMinY, float rectangleMax);
	void ResetGame();
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::ivec2 resolution;
	glm::mat3 modelMatrix;

	float birdY, bodyX, bodyR, headX, headR, beakX, beakR;
	glm::vec3 approxBirdCircle;
	float gravity, birdSpeed, birdAcc, rotateStep, translateY;

	float rectangleWidth, betweenRectangles, remainingSpace, obstacleSpeed, squareSide, maxHeight;
	std::list<float> rectangleHeights;

	float score, addToScore;
	bool hasCollided;
};
