#include "GameHelpers.h"
#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

void ResetGame()
{
	hasCollided = false;
	score = 0.0f;
	addToScore = 0.0f;
	cout << "Game was reset." << endl << "Score: 0" << endl;
	rectangleHeights = { 0, 0, 0, 0, 0 };
	remainingSpace = rectangleWidth;
	obstacleSpeed = 200.0f;
	gravity = 100.0f;
	birdSpeed = 0.0f;
	birdAcc = 0.0f;
	rotateStep = 0.0f;
	translateY = 0.0f;
}
