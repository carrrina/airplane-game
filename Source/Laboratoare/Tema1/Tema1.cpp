#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "CreatePrimitives.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	cout << "" << endl;
	cout << " --------------------------------------------------------  " << endl;
	cout << "|                                                        | " << endl;
	cout << "|   **** *    **** **** **** *   *    ***  * ***  ***    | " << endl;
	cout << "|   *    *    *  * *  * *  * *   *    *  * * *  * *  *   | " << endl;
	cout << "|   ***  *    **** **** **** *****    ***  * ***  *  *   | " << endl;
	cout << "|   *    *    *  * *    *      *      *  * * *  * *  *   | " << endl;
	cout << "|   *    **** *  * *    *      *      ***  * *  * ***    | " << endl;
	cout << "|														  | " << endl;
	cout << " --------------------------------------------------------  " << endl;
	cout << "         Press R to reset game or Q to quit." << endl << endl;
	cout << "Score: 0" << endl;

	// bird geometry 
	birdY = (float)resolution.y / 2.0f;
	bodyX = (float)resolution.x / 5.0f;
	bodyR = 15;
	headR = bodyR / 2;
	headX = bodyX + bodyR + headR;
	beakX = bodyX + bodyR + bodyR;
	beakR = headR / 2;
	approxBirdCircle = glm::vec3(bodyX - bodyR + (2 * bodyR + bodyR + 10) / 2, birdY, (2 * bodyR + bodyR + 10) / 2);
	glm::vec3 birdColor = glm::vec3(0.85, 0.85, 0.10), beakColor = glm::vec3(0.556863, 0.137255, 0.137255);
	Mesh* body = CreatePrimitives::CreateCircle("body", bodyX, birdY, bodyR, 1000, birdColor);
	Mesh* head = CreatePrimitives::CreateCircle("head", headX, birdY, headR, 1000, birdColor);
	Mesh* beak = CreatePrimitives::CreateTriangle("beak", glm::vec3(beakX, birdY - beakR, 0), 
														  glm::vec3(beakX + beakR, birdY, 0), 
														  glm::vec3(beakX, birdY + beakR, 0), beakColor);
	AddMeshToList(body);
	AddMeshToList(head);
	AddMeshToList(beak);

	// scene geometry
	rectangleHeights = { 0, 0, 0, 0, 0, 0 };
	maxHeight = 0.5f;
	rectangleWidth = (float)resolution.x / 10;
	betweenRectangles = 3 * rectangleWidth;
	glm::vec3 corner = glm::vec3(resolution.x - rectangleWidth, 0, 0);
	remainingSpace = rectangleWidth;
	obstacleSpeed = 200.0f;
	squareSide = rectangleWidth / 4;
	glm::vec3 retangleColor = glm::vec3(0.42, 0.26, 0.15);
	Mesh* rectangle = CreatePrimitives::CreateRectangle("rectangle", corner, rectangleWidth, 1, retangleColor);
	Mesh* square = CreatePrimitives::CreateSquare("square", corner, squareSide, retangleColor);
	AddMeshToList(rectangle);
	AddMeshToList(square);

	// bird animation
	gravity = 150.0f;
	birdSpeed = 0.0f;
	birdAcc = 0.0f;
	rotateStep = 0.0f;
	translateY = 0.0f;

	// gameplay variables
	hasCollided = false;
	score = 0.0f;
	addToScore = 0.0f;
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	if (!hasCollided)
	{
		// draw bird
		birdAcc += gravity * deltaTimeSeconds;
		if (birdAcc >= gravity)
			birdAcc = gravity;
		birdSpeed += birdAcc * deltaTimeSeconds;
		translateY -= birdSpeed * deltaTimeSeconds;
		rotateStep -= deltaTimeSeconds;
		if (rotateStep < -RADIANS(45))
			rotateStep = -RADIANS(45);
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(0, translateY);
		modelMatrix *= Transform2D::Translate(approxBirdCircle.x, approxBirdCircle.y);
		modelMatrix *= Transform2D::Rotate(rotateStep);
		modelMatrix *= Transform2D::Translate(-approxBirdCircle.x, -approxBirdCircle.y);
		RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

		remainingSpace += obstacleSpeed * deltaTimeSeconds;
		if (remainingSpace >= betweenRectangles) // add new rectangle
		{
			remainingSpace -= rectangleWidth + betweenRectangles;
			rectangleHeights.pop_back();
			float newHeight = rand() % (int)(maxHeight * resolution.y);
			if (newHeight <= 2 * squareSide)
				newHeight = 2 * squareSide;
			rectangleHeights.push_front(newHeight);
		}
		int index = 0;
		for (float height : rectangleHeights)
		{
			if (height != 0)
			{
				// bottom rectangle
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(-index * rectangleWidth - index * betweenRectangles - remainingSpace, 0);
				modelMatrix *= Transform2D::Translate(resolution.x - rectangleWidth / 2, 1 / 2);
				modelMatrix *= Transform2D::Scale(1, height);
				modelMatrix *= Transform2D::Translate(-resolution.x + rectangleWidth / 2, -1 / 2);
				RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(-index * rectangleWidth - index * betweenRectangles - remainingSpace - squareSide, height - squareSide);
				RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(-index * rectangleWidth - index * betweenRectangles - remainingSpace + rectangleWidth, height - squareSide);
				RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

				if (BirdHasCollided(resolution.x - rectangleWidth - index * rectangleWidth - index * betweenRectangles - remainingSpace,
					resolution.x - index * rectangleWidth - index * betweenRectangles - remainingSpace,
					0, height))
					hasCollided = true;
				if (BirdHasCollided(resolution.x - rectangleWidth - index * rectangleWidth - index * betweenRectangles - remainingSpace - squareSide,
					resolution.x - index * rectangleWidth - index * betweenRectangles - remainingSpace + squareSide,
					height - squareSide, height))
					hasCollided = true;

				// upper rectangle
				float height2 = resolution.y - height - approxBirdCircle.z * 2 * 3.5f;
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(-index * rectangleWidth - index * betweenRectangles - remainingSpace, resolution.y - height2);
				modelMatrix *= Transform2D::Translate(resolution.x - rectangleWidth / 2, 1 / 2);
				modelMatrix *= Transform2D::Scale(1, height2);
				modelMatrix *= Transform2D::Translate(-resolution.x + rectangleWidth / 2, -1 / 2);
				RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(-index * rectangleWidth - index * betweenRectangles - remainingSpace - squareSide, resolution.y - height2);
				RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(-index * rectangleWidth - index * betweenRectangles - remainingSpace + rectangleWidth, resolution.y - height2);
				RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

				if (BirdHasCollided(resolution.x - rectangleWidth - index * rectangleWidth - index * betweenRectangles - remainingSpace,
					resolution.x - index * rectangleWidth - index * betweenRectangles - remainingSpace,
					resolution.y - height2, resolution.y))
					hasCollided = true;
				if (BirdHasCollided(resolution.x - rectangleWidth - index * rectangleWidth - index * betweenRectangles - remainingSpace - squareSide,
					resolution.x - index * rectangleWidth - index * betweenRectangles - remainingSpace + squareSide,
					resolution.y - height2, squareSide))
					hasCollided = true;
			}
			index++;
		}

		if (hasCollided)
		{
			score += addToScore;
			cout << " ------------------------------------------------------------------------- " << endl;
			cout << "|    *****      *     *       * ******       ****  *       ****** ****    |" << endl;
			cout << "|   *          * *    * *   * * *           *    *  *     * *     *   *   |" << endl;
			cout << "|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |" << endl;
			cout << "|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |" << endl;
			cout << "|    *****  *       * *       * ******       ****      *    ***** *   *   |" << endl;
			cout << " ------------------------------------------------------------------------- " << endl;
			cout << endl;
			cout << "Final score: " << floor(score) << endl;
			cout << "Press R to reset or Q to quit." << endl;
		}
		else
		{
			addToScore += deltaTimeSeconds;
			if (addToScore > 5)
			{
				score += addToScore;
				addToScore = 0.0f;
				cout << "Score: " << floor(score) << endl;
				obstacleSpeed += 30 * deltaTimeSeconds;
				maxHeight += deltaTimeSeconds;
				if (maxHeight > 0.7f)
					maxHeight = 0.7f;
			}
		}
	}
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_SPACE))
	{
		birdSpeed = 0.0f;
		translateY += 300 * deltaTime;
		rotateStep += 4 * deltaTime;
		if (rotateStep < 0)
			rotateStep = 0;
		if (rotateStep > RADIANS(45))
			rotateStep = RADIANS(45);
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_R)
	{
		ResetGame();
	}
	if (key == GLFW_KEY_Q)
	{
		cout << "You quit." << endl;
		exit(0);
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

bool Tema1::BirdHasCollided(float minX, float maxX, float minY, float maxY)
{
	// bird has gone beyond the screen limits
	if ((approxBirdCircle.y + translateY - approxBirdCircle.z) < 0 ||
		(approxBirdCircle.y + translateY + approxBirdCircle.z) > resolution.y)
		return true;

	// get rectangle's closest point to the bird circle's center
	float x = std::max(minX, std::min(approxBirdCircle.x, maxX));
	float y = std::max(minY, std::min(approxBirdCircle.y + translateY, maxY));

	// distance between them
	float distance = sqrt((x - approxBirdCircle.x) * (x - approxBirdCircle.x) +
		(y - approxBirdCircle.y - translateY) * (y - approxBirdCircle.y - translateY));

	return distance < approxBirdCircle.z; // distance between them is less than bird circle's radius
}

void Tema1::ResetGame()
{
	hasCollided = false;
	score = 0.0f;
	addToScore = 0.0f;
	cout << "Game was reset." << endl << "Score: 0" << endl;
	rectangleHeights = { 0, 0, 0, 0, 0 };
	maxHeight = 0.5f;
	remainingSpace = rectangleWidth;
	obstacleSpeed = 200.0f;
	birdSpeed = 0.0f;
	birdAcc = 0.0f;
	rotateStep = 0.0f;
	translateY = 0.0f;
}
